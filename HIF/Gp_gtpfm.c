/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:  gp_gtpfm.c
*
* DESCRIPTION:
*    Processes Get Performance Command
*
*   $Revision: 58 $
*   $Date: 11/03/28 2:37p $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\hif\gp_ram.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_gtpfm.h"
#include ".\hif\sv_util.h"
#include ".\hif\bf_util.h"
#include ".\servo\svo.h"
#include ".\player\plrdb.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\al\reg_intctrl.h"

/*---------------------------------------------------------------
    "Except" Field definitions
---------------------------------------------------------------*/
#define EX_NOMINAL_PERFORMANCE           0x00
#define EX_EXCEPT_PERFORMANCE_01         0x01
#define EX_EXCEPT_PERFORMANCE_10         0x02

extern USHORT   GetTransferSpeed      ( BYTE );
extern const USHORT SelDVDReqSpeed_tbl[];
extern const USHORT SelCDReqSpeed_tbl[];

/*********************************************************************
 GLOBAL VARIABLES
*********************************************************************/
/* Nominal Speed Variables */
BYTE    RD_speed_setting;   /* this variable is set by ServoSetSpeed()/svr_set_to_highest_speed()  */


/*********************************************************************
 LOCAL TYPEDEF
*********************************************************************/
/* Nominal Performance Descriptor */
typedef union {
    BYTE    BYTE[16];
    struct {
        ULONG   startLBA;           /* the first logical block address             */
        ULONG   startPerformance;   /* drive performance at the startLBA in KB/sec */
        ULONG   endLBA;             /* the last logical block address              */
        ULONG   endPerformance;     /* drive performance at the endLBA in KB/sec   */
    } field;
} uNominalPerformanceDescriptor;

/* Exceptions Performance Descriptor */
typedef union {
    BYTE    BYTE[6];
    struct {
        ULONG   exceptLBA;          /* Seek Delay between (exceptLBA - 1) and exceptLBA            */
        USHORT  exceptTime;         /* additional delay from nominal, in units of 100usec(= 0.1ms) */
    } field;
} uExceptPerformanceDescriptor;


/*********************************************************************
 EXCEPTION DATA
*********************************************************************/
static const USHORT exceptList[] = {
        0x0000,  SD_1X,  SD_2X,  0x0000,
        SD_4X,   0x0000, 0x0000, 0x0000,
        SD_8X,   0x0000, SD_10X, 0x0000,
        SD_12X,  0x0000, 0x0000, 0x0000,
        SD_16X,  0x0000, 0x0000, 0x0000,
        0x0000,  0x0000, 0x0000, 0x0000,
        SD_24X,  0x0000, 0x0000, 0x0000,
        0x0000,  0x0000, 0x0000, 0x0000,
        SD_32X,  0x0000, 0x0000, 0x0000,
        0x0000,  0x0000, 0x0000, 0x0000,
        SD_40X
};


/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
static void     CreatePerformanceHeader     ( ULONG, BYTE );
static USHORT   GetNominalPerformance       ( BYTE );
static void     SetDefaultReadPerformance   ( void );
static USHORT   GetExceptionPerformance     ( ULONG, BYTE, BYTE );
static USHORT   CalcCAVStartPerformance     ( USHORT );
static ULONG    GetPerformanceAtEndLBA      ( void );


/*********************************************************************
 LOCAL DEFINITION
*********************************************************************/
#define PFM_NOMINAL_SIZE        16                          /* Size of Nominal performance descriptor   */
#define PFM_EXCEPT_SIZE         6                           /* Size of Exception performance descriptor */
#define REQ_LBA_ERROR           0xFF


/*********************************************************************
*   Function :
*       GetPerformanceCommand()
*
*   Description :
*       This is the command handler for the Get Performance command
*
*   !!!!! CAUTION !!!!!
*       StartLBA field is valid ONLY when ExceptField = 01b !!
*
*********************************************************************/
void GetPerformanceCommand()
{
    ULONG   TransferLength;                 // Performance data to transfer to host(in bytes).
    ULONG   StartLBA;                       // Starting point for performance data returned.
    USHORT  NumOfDescriptors;               // Number of descriptors that the drive returns.
    USHORT  MaxNumDescriptors;              // Max. number of descriptors that the drive returns.
    StLongU   SpindleSpeed;
    BYTE    ToleranceField, WriteField, ExceptField;   // Bit field variables.
    BYTE    TypeField;
    BYTE    ResponseHdrBitField;
    BYTE    IndexI;

    /*---------------------------------------------------------------
        Initialize local variables
    ---------------------------------------------------------------*/
    StartLBA                = 0L;
    TransferLength          = 0L;
    MaxNumDescriptors       = 0;
    NumOfDescriptors        = 0;
    TypeField               = 0;

    /*---------------------------------------------------------------
        Parse the Command Descriptor Block (CDB)
    ---------------------------------------------------------------*/
    ToleranceField              = ((Iram_Cmd_block.bf[1] & TOLERANCE_MASK) >> 3);
    WriteField                  = ((Iram_Cmd_block.bf[1] & WRITE_MASK) >> 2);
    ExceptField                 = (Iram_Cmd_block.bf[1] & EXCEPT_MASK);
    StartLBA                    = (ULONG)(PacketCommandToLBALEN4( 2 ));/* case of "ExceptField = 01b" ONLY use */
    BHIGH( MaxNumDescriptors )  = Iram_Cmd_block.bf[8];
    BLOW ( MaxNumDescriptors )  = Iram_Cmd_block.bf[9];
    TypeField                   = Iram_Cmd_block.bf[10];


    if((TypeField != 0x03) && (TypeField != 0))     // Type in CDB Byte 10 != Write Speed Descriptor && != Performance Data.
    {
        // These Data Types are not implemented.
        BUILD_SENSE(0x05,0x24,0x00,0xf0);        /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }


    Iram_Buf_dram_addr = (PERFORM_ADR + 8);
    ResponseHdrBitField = 0;
    /*---------------------------------------------------------------
        Determine if transfer of performance data is necessary.
    ---------------------------------------------------------------*/
    if (MaxNumDescriptors == 0)
    {                               /* no data xfer, not an error */
        CreatePerformanceHeader(0L, ResponseHdrBitField);
        SendParameterDataToHost(PERFORM_ADR, PERFORM_HEADER , TRUE);
        return;
    }

    if (St_no_disc)   /* no media is mounted */
    {
       //****************************************************************
       //
       //  No disc in tray. Return maximum speed for DVD ????? CD ????
       //
       //
       //****************************************************************
        Iram_Buf_dram_addr = (PERFORM_ADR + 8);

        if(TypeField == 0x03)        // Type in CDB Byte 10 = Write Speed Descriptor.
        {
            for(IndexI = 8; IndexI > 0; IndexI--)
            {
                WriteSDRAM(0x00);   // Initialize descriptor BYTE 0 - 7
            }

            // Maximum DVD Read speed
            SpindleSpeed.Long = SelDVDReqSpeed_tbl[DVD_SPD_COLMNS - 1];
            WriteSDRAM(SpindleSpeed.byte.u);
            WriteSDRAM(SpindleSpeed.byte.h);
            WriteSDRAM(SpindleSpeed.byte.m);
            WriteSDRAM(SpindleSpeed.byte.l);

            // Maximum DVD Write speed
            SpindleSpeed.Long = SelDVDReqSpeed_tbl[DVD_SPD_COLMNS - 1];
            WriteSDRAM(SpindleSpeed.byte.u);
            WriteSDRAM(SpindleSpeed.byte.h);
            WriteSDRAM(SpindleSpeed.byte.m);
            WriteSDRAM(SpindleSpeed.byte.l);

            NumOfDescriptors = 1;
            TransferLength = 0x10;
        }
        else
        {
            // TBD? Here we need code for TypeField = 0
            //     Not implemented.
            BUILD_SENSE(0x05,0x24,0x00,0xf0);        /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
    }
    else
#if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC)
    {
        //****************************************************************
        //
        //  A BD is in the tray.
        //
        //****************************************************************
        if(TypeField == 0x03)        // Type in CDB Byte 10 = Write Speed Descriptor.
        {
            /*********************************************************/
            /* Type 3: Get Performance - Write Speed                 */
            /*********************************************************/
            BUILD_SENSE(0x05,0x24,0x00,0x51);        /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
        else if(TypeField == 0x00)   // Type in CDB Byte 10 = Performance Data.
        {
            /*********************************************************/
            /* Assume Type 0: Get Performance                        */
            /*********************************************************/

            /*---------------------------------------------------------------
                Check "Tolerance" field (bit 4-3), return error if this field is NOT 10b.
            ---------------------------------------------------------------*/
            if ( ToleranceField != 0x02 )       // From CDB[1]
            {
                BUILD_SENSE(0x05,0x24,0x00,0x52);        /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }

            /*---------------------------------------------------------------
                Response Data Header Setting
            ---------------------------------------------------------------*/
            if ( WriteField )         // From CDB[1]
            {
                ResponseHdrBitField += 0x02;     /* bit 1 SET */
            }
            if ( ExceptField )       // From CDB[1]
            {
                ResponseHdrBitField += 0x01;     /* bit 0 SET */
            }

            /*---------------------------------------------------------------
                Determine "Except" type and then call the appropriate
                function to write the performance data to DRAM
            ---------------------------------------------------------------*/
            Iram_Buf_dram_addr = (PERFORM_ADR + 8);
            switch ( ExceptField )      // From CDB[1]
            {
            case EX_NOMINAL_PERFORMANCE:             /* Except field = 00b */
                NumOfDescriptors   = GetNominalPerformance( WriteField );
                TransferLength = (ULONG)(NumOfDescriptors * PFM_NOMINAL_SIZE);
                break;

            case EX_EXCEPT_PERFORMANCE_01:           /* Except field = 01b */
            case EX_EXCEPT_PERFORMANCE_10:           /* Except field = 10b */
                NumOfDescriptors   = GetExceptionPerformance( StartLBA, WriteField, ExceptField );
                if ( NumOfDescriptors == REQ_LBA_ERROR )
                {
                    BUILD_SENSE(0x05,0x21,0x00,0x01);/* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
                    AutoCommandComplete();
                    return;
                }
                TransferLength = (ULONG)(NumOfDescriptors * PFM_EXCEPT_SIZE);
                break;

            default:                            /* "Except field = 11b" is Reserved value */
                BUILD_SENSE(0x05,0x24,0x00,0x53);        /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }
        }
    }
    else
#endif
    if (pdbDiscLoaded() == DVD_DISC)
    {
        //****************************************************************
        //
        //
        //  A DVD is in the tray.
        //
        //
        //****************************************************************
        if(TypeField == 0x03)        // Type in CDB Byte 10 = Write Speed Descriptor.
        {
            /*********************************************************/
            /* Type 3: Get Performance - Write Speed                 */
            /*********************************************************/
            BUILD_SENSE(0x05,0x24,0x00,0xfc);        /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
        else if(TypeField == 0x00)   // Type in CDB Byte 10 = Performance Data.
        {
            /*********************************************************/
            /* Assume Type 0: Get Performance                        */
            /*********************************************************/

            /*---------------------------------------------------------------
                Check "Tolerance" field (bit 4-3), return error if this field is NOT 10b.
            ---------------------------------------------------------------*/
            if ( ToleranceField != 0x02 )       // From CDB[1]
            {
                BUILD_SENSE(0x05,0x24,0x00,0xfc);        /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }

            /*---------------------------------------------------------------
                Response Data Header Setting
            ---------------------------------------------------------------*/
            if ( WriteField )         // From CDB[1]
            {
                ResponseHdrBitField += 0x02;     /* bit 1 SET */
            }
            if ( ExceptField )       // From CDB[1]
            {
                ResponseHdrBitField += 0x01;     /* bit 0 SET */
            }

            /*---------------------------------------------------------------
                Determine "Except" type and then call the appropriate
                function to write the performance data to DRAM
            ---------------------------------------------------------------*/
            Iram_Buf_dram_addr = (PERFORM_ADR + 8);
            switch ( ExceptField )      // From CDB[1]
            {
            case EX_NOMINAL_PERFORMANCE:             /* Except field = 00b */
                NumOfDescriptors   = GetNominalPerformance( WriteField );
                TransferLength = (ULONG)(NumOfDescriptors * PFM_NOMINAL_SIZE);
                break;

            case EX_EXCEPT_PERFORMANCE_01:           /* Except field = 01b */
            case EX_EXCEPT_PERFORMANCE_10:           /* Except field = 10b */
                NumOfDescriptors   = GetExceptionPerformance( StartLBA, WriteField, ExceptField );
                if ( NumOfDescriptors == REQ_LBA_ERROR )
                {
                    BUILD_SENSE(0x05,0x21,0x00,0xef);/* ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE */
                    AutoCommandComplete();
                    return;
                }
                TransferLength = (ULONG)(NumOfDescriptors * PFM_EXCEPT_SIZE);
                break;

            default:                            /* "Except field = 11b" is Reserved value */
                BUILD_SENSE(0x05,0x24,0x00,0xf0);        /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }
        }
    }
    else
    {
        //****************************************************************
        //
        //
        //  A CD is in the tray.
        //
        //
        //****************************************************************

        if(TypeField == 0x03)        // Type in CDB Byte 10 = Write Speed Descriptor.
        {
            BUILD_SENSE(0x05,0x24,0x00,0xba);       // INVALID FIELD IN CDB
            AutoCommandComplete();
            return;
        }
        else if(TypeField == 0x00)   // Type in CDB Byte 10 = Performance Data.
        {
            /*********************************************************/
            /* Assume Type 0: Get Performance                        */
            /*********************************************************/

            /*---------------------------------------------------------------
                Check "Tolerance" field (bit 4-3), return error if this field is NOT 10b.
            ---------------------------------------------------------------*/
            if ( ToleranceField != 0x02 )       // From CDB[1]
            {
                BUILD_SENSE(0x05,0x24,0x00,0xfc);        /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }

            /*---------------------------------------------------------------
                Response Data Header Setting
            ---------------------------------------------------------------*/
            if ( WriteField )         // From CDB[1]
            {
                ResponseHdrBitField += 0x02;     /* bit 1 SET */
            }
            if ( ExceptField )        // From CDB[1]
            {
                ResponseHdrBitField += 0x01;     /* bit 0 SET */
            }

            /*---------------------------------------------------------------
                Determine "Except" type and then call the appropriate
                function to write the performance data to DRAM
            ---------------------------------------------------------------*/
            Iram_Buf_dram_addr = (PERFORM_ADR + 8);
            switch ( ExceptField )        // From CDB[1]
            {
            case EX_NOMINAL_PERFORMANCE:             /* Except field = 00b, NOMINAL PERFORMANCE */
                NumOfDescriptors   = GetNominalPerformance( WriteField );
                TransferLength = (ULONG)(NumOfDescriptors * PFM_NOMINAL_SIZE);
                break;

            case EX_EXCEPT_PERFORMANCE_01:           // Except field = 01b, ENTIRE PERFORMANCE LIST
            case EX_EXCEPT_PERFORMANCE_10:           // Except field = 10b, PERFORMANCE EXEPTIONS ONLY
                NumOfDescriptors   = GetExceptionPerformance( StartLBA, WriteField, ExceptField );
                if ( NumOfDescriptors == REQ_LBA_ERROR )
                {
                    BUILD_SENSE(0x05,0x21,0x00,0xef);  // ILLEGAL REQUEST, LOGICAL BLOCK ADDRESS OUT OF RANGE
                    AutoCommandComplete();
                    return;
                }
                TransferLength = (ULONG)(NumOfDescriptors * PFM_EXCEPT_SIZE);
                break;

            default:                            // "Except field = 11b" is Reserved value
                BUILD_SENSE(0x05,0x24,0x00,0xf0);        // INVALID FIELD IN CDB
                AutoCommandComplete();
                return;
            }
        }
    }

    //****************************************************************
    //
    //
    //
    // Following code is common to CD, DVD, or No Disc in tray.
    //
    //
    //
    //****************************************************************
    /*---------------------------------------------------------------
        Check allocation length
    ---------------------------------------------------------------*/
    if ( NumOfDescriptors > MaxNumDescriptors )
    {
        if ( ExceptField == EX_NOMINAL_PERFORMANCE )      // From CDB[1]
        {
            TransferLength -= (ULONG)((NumOfDescriptors - MaxNumDescriptors) * PFM_NOMINAL_SIZE);
        }
        else
        {
            TransferLength -= (ULONG)((NumOfDescriptors - MaxNumDescriptors) * PFM_EXCEPT_SIZE);
        }
    }

    /*---------------------------------------------------------------
        Test for max amount of data for single transfer (64kbytes)
    ---------------------------------------------------------------*/
    if ( (TransferLength + PERFORM_HEADER) > GETPFM_MAX_BYTES )
        TransferLength = GETPFM_MAX_BYTES;

    /*---------------------------------------------------------------
        Create Performance header in SDRAM, fill data length
    ---------------------------------------------------------------*/
    CreatePerformanceHeader( TransferLength, ResponseHdrBitField );

    /*---------------------------------------------------------------
        Set buffer starting address and BYTE count
    ---------------------------------------------------------------*/
    SendParameterDataToHost(PERFORM_ADR,(TransferLength + PERFORM_HEADER), TRUE);
}


/*********************************************************************
*   Function :
*       CreatePerformanceHeader()
*
*   Description :
*       This function creates the Performance Header in DRAM area.
*
*   Parameters :
*       DataSize -  Size of the return performance data to be
*                   transferred to HOST
*       BitField -  Bit field in header Byte4 setting
*                       bit 1 : Write(= 1)/Read(= 0) performance data
*                       bit 0 : Except(= 1)/Nominal(= 0) performance data
*   << Notes >>
*       This header is 8 Bytes.
*********************************************************************/
static void CreatePerformanceHeader( ULONG DataSize, BYTE BitField )
{
    BYTE    temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();            /* disable interrupts */
    DISABLE_INTERRUPT();

    /* point to PERFORM address in SDRAM */
    Iram_Buf_dram_addr = PERFORM_ADR;
    /* Write performance data size (bytes 0-3) */
    DataSize += 4;                              /* "Reserved(3Bytes) + Write & Except Field(1Bytes)" Add */
    WriteSDRAM( (DataSize & 0xFF000000) >> 24 );
    WriteSDRAM( (DataSize & 0x00FF0000) >> 16 );
    WriteSDRAM( (DataSize & 0x0000FF00) >>  8 );
    WriteSDRAM( (DataSize & 0x000000FF) );
    /* Write performance data bit field (bytes 4) */
    WriteSDRAM( BitField );

    /* fill 0 to reserved bytes(bytes 5-7) */
    WriteSDRAM( 0x00 );
    WriteSDRAM( 0x00 );
    WriteSDRAM( 0x00 );

    SET_INT_ENABLE_MASK( temp_IE );     /* restore interrupts */
}

/*********************************************************************
*   Function :
*       GetNominalPerformance()
*
*   Description :
*       This function creates the nominal Performance data for either CD or DVD in DRAM area.
*       All numbers are NOMINAL.
*       On CD media, all sectors shall be reported as 2352Byte sectors.
*
*   Parameters :
*       mode       : returning performance for READ? or WRITE?
*
*   Return value:
*       NumReturnDescriptors  : Number of descriptors in returning DATA
*
*   << NOTE >>
*       For example, a 4X-6X CD-ROM Logical Unit (CAV/CLV combination)
*       with a data disc loaded may return two nominal performance descriptors.
*       The first would indicate a Start LBA of 0, Start Performance of 706 KB/s,
*       and an end LBA in the middle and a performance of 1058 KB/s.
*       The second would indicate a start LBA adjacent to the ending LBA
*       of the previous descriptor, a start performance of 1058 KB/s,
*       and an end LBA at the end of the medium and an ending performance of 1058 KB/s.
*       The data rate may vary according to the mounted medium,
*       i.e. CD Audio Tracks may have a different spin rate than Data Tracks.
*       1kB/s is 1000 Bytes per second.
*
*********************************************************************/
#define READ_MODE       0
#define WRITE_MODE      1

static USHORT GetNominalPerformance( BYTE ReadWriteMode )
{
    USHORT              NumReturnDescriptors = 0;
    ULONG               TempRateStart, TempRateEnd;
    ULONG               TempLBA;
    uNominalPerformanceDescriptor     TempData;
    BYTE                Count, TempNominalSpeed;
    TimeS               TempMSF;

    /* Now medium is changing, return default performance */
    if ( (Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE) )
    {
        if ( ReadWriteMode == WRITE_MODE )
        {
            return( 0 );
        }
        NumReturnDescriptors++;                // Increment Descriptors available.
        SetDefaultReadPerformance();    /* set default performance data to DRAM */
        return( NumReturnDescriptors );
    }
    /*
     * case of medium is ROM disc
     *      write performance : return No data
     * case of medium is not present
     *      write performance : return No data
     *      read performance  : return Default data
     */
    switch ( pdbServoGetDiscType() )
    {
    case eNonATIPDisc:
        if ( ReadWriteMode == WRITE_MODE )
        {
            return( 0 );
        }
        break;

    case eATIPDisc:
        break;

    default:
        if ( ReadWriteMode == WRITE_MODE )
        {
            return( 0 );
        }
        NumReturnDescriptors++;                // Increment Descriptors available.
        SetDefaultReadPerformance();    /* set default performance data to DRAM */
        return( NumReturnDescriptors );
    }

    /* Start LBA set (Fixed) */
    TempData.field.startLBA = 0L;

    /* End LBA set */
    if ( ReadWriteMode == READ_MODE )
    {
        TempLBA = GetPerformanceAtEndLBA();
    }
    else
    {
        TempMSF = pdbGetATIPLeadOut();
        TempLBA = MSF2LBA( &TempMSF );
    }

    TempData.field.endLBA  = SWAPEND_ULONG(TempLBA);

    /* Start/End Performance data set */
    switch ( ReadWriteMode )
    {
    case READ_MODE:
        TempNominalSpeed = RD_speed_setting;
        break;

    case WRITE_MODE:
        break;

    default:
        return( 0 );
    }

    TempRateEnd = (ULONG)GetTransferSpeed( TempNominalSpeed );
    TempData.field.endPerformance = SWAPEND_ULONG(TempRateEnd);

    if ( TempNominalSpeed & e_CAV )
    {   /* CAV */
        TempRateStart = (ULONG)CalcCAVStartPerformance( (USHORT)TempRateEnd );
        TempData.field.startPerformance = SWAPEND_ULONG(TempRateStart);
    }
    else
    {                               /* CLV */
        TempData.field.startPerformance = SWAPEND_ULONG(TempRateEnd);
    }
    NumReturnDescriptors++;         // Increment Descriptors available.
    /* set performance data to SDRAM */
    for( Count = 0; Count < PFM_NOMINAL_SIZE; Count++ )
    {
        WriteSDRAM( TempData.BYTE[Count] );
    }

    return( NumReturnDescriptors );
}

/**********************************************************************
*   Function : SetDefaultReadPerformance for CD and DVD
*   Description:
*   Input:      None
*   Output:     None
*
**********************************************************************/
static void SetDefaultReadPerformance( void )
{
    StLongU     MaxLba;
    USHORT      StartRate, EndRate;
    BYTE      * SdRamAddr;

    SdRamAddr   = (BYTE *) (SCSDRAM_ADDRESS + PERFORM_ADR + 8);

    #if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC)
    {
        EndRate   = GetTransferSpeed( Disc_Speed.Read );  // ==change reason fixed return wrong current speed to host
        StartRate = CalcCAVStartPerformance( EndRate );
    }
    else
    #endif  //(BD_ENABLE)
    if (pdbDiscLoaded() == DVD_DISC)
    {
        EndRate   = GetTransferSpeed( Disc_Speed.Read );  //20090224_g.21==change reason fixed return wrong current speed to host
        StartRate = CalcCAVStartPerformance( EndRate );
    }
    else
    {
        EndRate   = GetTransferSpeed( MAX_READ_SPEED );
        StartRate = CalcCAVStartPerformance( EndRate );
    }
    /* Set Start default LBA(= 00'00'00) Byte 0~3 */
    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = 0x00;

    /* Set Start default Performance Byte 4~7 */

    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = BHIGH( StartRate );
    *SdRamAddr++ = BLOW ( StartRate );

    MaxLba.Long = *(ULONG *)(SCSDRAM_ADDRESS + CAPACITY_ADR);
    *SdRamAddr++ = MaxLba.byte.l ;  // Note! MaxLba was stored in Little Endian form in memory
    *SdRamAddr++ = MaxLba.byte.m ;  //       swap bytes for host
    *SdRamAddr++ = MaxLba.byte.h ;
    *SdRamAddr++ = MaxLba.byte.u ;

    /* Set End default Performance Byte 12~15 */
    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = 0x00;
    *SdRamAddr++ = BHIGH( EndRate ) ;
    *SdRamAddr++ = BLOW ( EndRate ) ;
}


/*********************************************************************
*   Function :
*       GetExceptionPerformance()
*
*   Description :
*       This function creates the exception Performance data in DRAM area.
*       Exception time indicates the excepted additional delay between
*       (StartPoint - 1) and StartPoint, in units of tenths of msec.
*
*   Parameters :
*       StartPoint : starting point of returning performance
*       ReadWriteMode       : returning performance for READ? or WRITE?
*       ExceptionType     : requested exception type
*
*   Return value:
*       NumReturnDescriptors  : Number of descriptors in returning DATA
*
*********************************************************************/
static USHORT GetExceptionPerformance( ULONG StartPoint, BYTE ReadWriteMode, BYTE ExceptionType )
{
    USHORT              NumReturnDescriptors = 0;
    BYTE                TempNominalSpeed, TempSetSpeed;
    uExceptPerformanceDescriptor      TempData;
    BYTE                Count;
    TimeS               TempMSF;
    ULONG               TempLBA;

    if ( (Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE) )
    {
        // Medium is changing, return default performance
        return( 0 );
    }
    /*
     * case of medium is ROM disc
     *      write performance : return No data
     * case of medium is not present
     *      wr/rd performance : return No data
     */
    switch ( pdbServoGetDiscType() )
    {
        case eNonATIPDisc:
            if ( ReadWriteMode == WRITE_MODE )
            {
                return( 0 );
            }
            break;

        case eATIPDisc:
            break;

        default:
            return( 0 );
    }

    /* Exception LBA set */
    if ( ExceptionType == EX_EXCEPT_PERFORMANCE_01 )
    {       /* except 01b */
        if ( ReadWriteMode == READ_MODE )
        {
            TempLBA = GetPerformanceAtEndLBA();
        }
        else
        {
            TempMSF = pdbGetATIPLeadOut();
            TempLBA = MSF2LBA( &TempMSF );
        }
        if ( TempLBA < StartPoint )
        {
            return( REQ_LBA_ERROR );
        }
        TempData.field.exceptLBA = SWAPEND_ULONG(StartPoint);
    }
    else
    {                                   /* except 10b */
        if ( ReadWriteMode == READ_MODE )
        {
            TempLBA = GetPerformanceAtEndLBA();
        }
        else
        {
            TempMSF = pdbGetATIPLeadOut();
            TempLBA = MSF2LBA( &TempMSF );
        }
        TempData.field.exceptLBA = SWAPEND_ULONG(TempLBA);
    }

    /* Exception Performance data set */
    switch ( ReadWriteMode )
    {
        case READ_MODE:
            TempNominalSpeed  = RD_speed_setting;
            TempSetSpeed   = Disc_Speed.Read;
            break;

        case WRITE_MODE:
            TempSetSpeed   = Disc_Speed.Write;
            break;

        default:
            return( 0 );
    }

    TempData.field.exceptTime = SWAP_WORD(SD_32X);

    NumReturnDescriptors++;                    // Increment Descriptors available.

    /* set performance data to DRAM */
    for( Count = 0; Count < PFM_EXCEPT_SIZE; Count++ )
    {
        WriteSDRAM( TempData.BYTE[Count] );
    }

    return( NumReturnDescriptors );
}


/*********************************************************************
*   Function :
*       CalcCAVStartPerformance()
*
*   Description :
*       This function calculates the transfer rate of first addressable
*       point on Disc in CAV.
*
*   Parameters :
*       OutsideRate  -  Transfer rate of last addressable point on Disc
*
*   Return value:
*       InsideRate   -  Transfer rate of firast addressable point on Disc
*
*   << NOTE >>
*       Lowest Speed : Highest Speed = 1 : 2.5
*           Lowest Speed  - speed of first addressable point on Disc
*           Highest Speed - speed of last addressable point on Disc
*
*********************************************************************/
static USHORT CalcCAVStartPerformance( USHORT OutsideRate )
{
    USHORT  InsideRate;
    ULONG   CalculatedRate;

    /* divide HiSpeed by 2.5, we get LoSpeed */
    CalculatedRate = (ULONG)OutsideRate;
    CalculatedRate = ((CalculatedRate * 2) / 5);
    InsideRate = (USHORT)CalculatedRate;
    return( InsideRate );
}


/*********************************************************************
*   Function :
*       GetPerformanceAtEndLBA()
*
*   Description :
*       This function checks the last logical block address.
*
*   Parameters :
*       None
*
*   Return value:
*       LastLBA  : the last logical block address
*
*   << NOTE >>
*       The last LBA excluding Lead-Out area.
*
*********************************************************************/
static ULONG GetPerformanceAtEndLBA( void )
{
    ULONG   TempPacketSize;
    ULONG   TempLBA;
    BYTE    TrkNo;
    BYTE    TrkSta;
    ULONG   LastLBA;
    TimeS   LastMSF;

    TrkNo  = pdbGetFinalSessLastTrackNumber();
    TrkSta = pdbGetTrackState( TrkNo );

    switch( TrkSta )
    {
    case eTRACK_STATE_INCOMPLETE:
        LastLBA = pdbGetTrackNWA( TrkNo );
        break;

    case eTRACK_STATE_COMPLETE:
        LastMSF = pdbGetStopTime( TrkNo );
        LastLBA = MSF2LBA( &LastMSF );

        if ( (pdbServoGetDiscType() == eATIPDisc) )
        {
            if ( (pdbGetTrackStatus( TrkNo ) & eTS_FixedPacket) && (pdbGetCDRW() == TRUE) )
            {
                TempLBA = LastLBA;
                TempPacketSize = pdbGetTrackPacketSize( TrkNo );
                LastLBA = (LastLBA + 8L) / (TempPacketSize + 7L) * TempPacketSize;
                /* case of CD-RW & Fixed Packet Write */
            }
        }
        break;

    case eTRACK_STATE_BLANK:
    case eTRACK_STATE_EMPTY:
        if ( (TrkNo - 1) > 0 )
        {
            LastMSF = pdbGetStopTime( (TrkNo - 1) );
        }
        else
        {
            LastMSF = pdbGetATIPLeadOut();  /* case of CD-R/RW Blank */
        }
        LastLBA = MSF2LBA( &LastMSF );
        break;

    default:
        LastMSF = pdbGetATIPLeadOut();
        LastLBA = MSF2LBA( &LastMSF );
        break;
    }

    return( LastLBA );
}


