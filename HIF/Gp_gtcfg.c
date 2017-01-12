/****************************************************************************
*                (c) Copyright 2001-2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:  gp_gtcfg.c
*
* DESCRIPTION:
*    Processes Get Configuration Command
*
*   $Revision: 121 $
*   $Date: 11/03/16 5:59p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\oem\oem_tbl.h"
#include ".\hif\gp_ram.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_gtcfg.h"
#include ".\hif\sv_util.h"
#include ".\servo\svo.h"
#include ".\player\plrdb.h"
#include ".\al\reg_intctrl.h"

#include ".\dvd\dvgvars.h"
#if (VCPS_TEST ==1)
#include ".\dvd\dvvcps.h"
#endif

#if (EN_LS == 1)
#include ".\LS\LS_hif.h"
#endif // End of (EN_LS == 1)

#include ".\oem\oem_cfg_tbl.h"

#if (BD_ENABLE)
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#endif

#if (ENABLE_AACS)
#include ".\crypt\aacs.h"
#endif

#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_oem_tbl.h"
#endif
/*********************************************************************
 GLOBAL DEFINITION
*********************************************************************/

/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
static ULONG    GetAllFeatures          ( USHORT );
static ULONG    GetActiveFeatures       ( USHORT );
static ULONG    GetSingleFeature        ( USHORT, BYTE * );
static void     CreateFeatureHeader     ( ULONG );

static void     setCfgDATAtoDRAM        ( BYTE *, BYTE *, ULONG, BYTE );
static USHORT   getCurrentMediumInfo    ( void );
static void     setCurrentMediumBit     ( BYTE );


/*********************************************************************
*   Function :
*       GetConfigurationCommand()
*
*   Description :
*       This is the command handler for the Get Configuration command
*********************************************************************/
void GetConfigurationCommand()
{
    ULONG   CfgDataLen;             /* Config data size in bytes */
    ULONG   CfgDataFullLen;         /* Config data size in bytes (with Header) */
    BYTE    RequestType;            /* host request set of Feature Descriptors */
    BYTE    StCurrent;
    USHORT  StartFeatureNum;        /* The 1st retrurned feature number */
    USHORT  AllocationLen;          /* max. BYTE length response data to host */

    /*---------------------------------------------------------------
        Initial local variables
    ---------------------------------------------------------------*/
    CfgDataLen    = 0L;
    AllocationLen = 0;

    /*---------------------------------------------------------------
        Parse the command block
    ---------------------------------------------------------------*/
    RequestType              = (Iram_Cmd_block.bf[1] & REQUEST_TYPE_MASK);
    BHIGH( StartFeatureNum ) = Iram_Cmd_block.bf[2];
    BLOW ( StartFeatureNum ) = Iram_Cmd_block.bf[3];
    BHIGH( AllocationLen )   = Iram_Cmd_block.bf[7];
    BLOW ( AllocationLen )   = Iram_Cmd_block.bf[8];

    /*---------------------------------------------------------------
        Determine if a transfer is necessary and check if even xfer.
    ---------------------------------------------------------------*/
    if ( AllocationLen == 0 )
    {                               /* no data xfer, not an error */
        AutoCommandComplete();
        return;
    }
    else if ( AllocationLen & 0x01) /* must be even transfer */
     AllocationLen--;

    /*---------------------------------------------------------------
        Determine request type and then call the appropriate
        function to write the configuration data to DRAM
    ---------------------------------------------------------------*/
    switch ( RequestType )
    {
        case RT_ALL_FEATURES:
            CfgDataLen = GetAllFeatures( StartFeatureNum );
            break;

        case RT_ACTIVE_FEATURES:
            CfgDataLen = GetActiveFeatures( StartFeatureNum );
            break;

        case RT_SINGLE_FEATURE:
            Iram_Buf_dram_addr = (CFGDATA_ADR + 8);
            CfgDataLen    = GetSingleFeature( StartFeatureNum, &StCurrent );
            break;

        default:                                /* invalid request type */
            BUILD_SENSE(0x05,0x24,0x00,0xff);    /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
    }

    /*---------------------------------------------------------------
        Test for maximum config data return size (excludes header size)
    ---------------------------------------------------------------*/
    if ( CfgDataLen > MAX_CFGDATA_LEN )
    {
        BUILD_SENSE(0x05,0x24,0x00,0xfe);        /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    /*---------------------------------------------------------------
        Test for max amount of data for single transfer (64kbytes)
    ---------------------------------------------------------------*/
    if ( (CfgDataLen + FEATURE_HEADER) > GETCFG_MAX_BYTES )
        CfgDataLen = GETCFG_MAX_BYTES;

    /*---------------------------------------------------------------
        Create Feature header in DRAM, fill data length
    ---------------------------------------------------------------*/
    CreateFeatureHeader( CfgDataLen );


    /* check allocation length */
    CfgDataFullLen = (CfgDataLen + FEATURE_HEADER);

    if ( CfgDataFullLen > (ULONG)AllocationLen )
    {
        CfgDataFullLen = (ULONG)AllocationLen;
    }

    //SendParameterDataToHost(CFGDATA_ADR ,(USHORT)CfgDataFullLen  , TRUE);
    Iram_Linear_Xfer_addr = CFGDATA_ADR;
    Iram_shATB  = (USHORT)CfgDataFullLen;
    CheckDataTransferLimit(Iram_shATB); /* check if transfer data in burst mode */

#if (PCB == 815)
    #if (MSG_OUT_MODE == MSG_USBPORT)
    {
        BYTE * pInfo;
        USHORT idx;
        pInfo = (BYTE *) (SCSDRAM_ADDRESS + CFGDATA_ADR);
        for (idx = 0; idx < CfgDataFullLen; idx += 4)
            send_msg54(SEND_READ_DVD_MSG, pInfo[idx+0], pInfo[idx+1], pInfo[idx+2], pInfo[idx+3]);
    }
    #endif
#endif

    Knl_send_compl = TRUE;

    Iram_Buf_hxfer_blk = 1;
                                        /* setup H/W xfr */
    if(Ata_sub_burst == TRUE)
    {
        Send_pardata_piece_by_piece();
    }

    St_send_pardata = TRUE;
    Knl_Atapi_Avail = TRUE;
    TransferToHost();

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        Knl_Atapi_Avail = FALSE;
        BUILD_SENSE(0x00,0x00,0x00,0x18);    // NO ADDITIONAL SENSE INFO
        AutoCommandComplete();
    }
#endif
}


/*********************************************************************
*   Function :
*       CreateFeatureHeader()
*
*   Description :
*       This function creates the Feature Header in DRAM area
*
*   Parameters :
*       DataSize -  Size of the return configuration data to be
*                   transferred to host
*
*   << Notes >>
*       This header is 8 bytes.
*********************************************************************/
static void CreateFeatureHeader( ULONG DataSize )
{
    BYTE    temp_IE;
    USHORT  CurrentDisc;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();

    /* point to CFGDATA address in SDRAM */
    Iram_Buf_dram_addr = CFGDATA_ADR;

    /* Write config data size (bytes 0-3) */
    DataSize += 4;                              /* "Reserved(2Bytes) + Current Profile(2Bytes)" Add */
    WriteSDRAM( (DataSize & 0xFF000000) >> 24 );
    WriteSDRAM( (DataSize & 0x00FF0000) >> 16 );
    WriteSDRAM( (DataSize & 0x0000FF00) >>  8 );
    WriteSDRAM( (DataSize & 0x000000FF) );

    /* fill 0 to reserved bytes(bytes 4-5) */
    WriteSDRAM( 0x00 );
    WriteSDRAM( 0x00 );

    /* fill current profile num(bytes 6-7) */
#if (EN_LS == 1)
    /*ls Disc profile */
    if (pdbGetDiscKind() == eLSDisc)
        CurrentDisc = 0xFFFF;
    else
#endif // End of (EN_LS == 1)

    switch(pdbDiscLoaded())
    {
        /* CD PROFILE */
        case CD_DISC:
            switch ( (getCurrentMediumInfo() & 0x1F) )
            {
                case CUR_CD_ROM:                        /* CD-ROM Present */
                    CurrentDisc = PROFILE_CDROM;
                    break;

                case CUR_CD_R:                          /* CD-R Present */
                    CurrentDisc = PROFILE_CDR;
                    break;

                case CUR_CD_RW:
                case CUR_CD_HRW:
                case CUR_CD_USRW:                       /* CD-RW Present */
                    CurrentDisc = PROFILE_CDRW;
                    break;

                default:                                /* Medium NOT Present */
                    CurrentDisc = PROFILE_DEFAULT;
                    break;
            }
            break;

        /* DVD PROFILE */
        case DVD_DISC:
#if (CEProduct == 1)
            switch(pdbGetDVDType())
            {
                case DVD_ROM:
                    CurrentDisc = PROFILE_DVDROM;
                    break;

                case DVD_MINUS_R:
                    if (pdbGetDVDLayers() == SINGLE_LAYER)
                        CurrentDisc = PROFILE_DVDMINUSR;
                    else
                        CurrentDisc = PROFILE_DVDMINUSRDL;
                    break;

                case DVD_RAM:
                    CurrentDisc = PROFILE_DVDRAM;
                    break;

                case DVD_MINUS_RW:
                    CurrentDisc = PROFILE_DVDMINUSRWS;
                    break;

                case DVD_PLUS_RW:
                    CurrentDisc = PROFILE_DVDPLUSRW;
                    break;

                case DVD_PLUS_R:
                    CurrentDisc = PROFILE_DVDPLUSR;
                    break;

                case DVD_PLUS_R_DUAL:
                    CurrentDisc = PROFILE_DVDPLUSRDL;
                    break;

                case DVD_PLUS_RW_DUAL:
                    CurrentDisc = PROFILE_DVDPLUSRWDL;
                    break;
            }
#else
                CurrentDisc = PROFILE_DVDROM;
#endif  /* CEProduct == 1 */
            break;

#if (BD_ENABLE)
        /* BD PROFILE */
        case BD_DISC:
            switch(pdbGetDiscKind())
            {
                case eBDROMDisc:
                case eBDROMDLDisc:
                    CurrentDisc = PROFILE_BDROM;
                    break;

                case eBDREDisc:
                case eBDREDLDisc:
                    CurrentDisc = PROFILE_BDRE;
                    break;

                case eBDRDisc:
                case eBDRDLDisc:
                    if (pdbGetBDRecMode() & 0x01)   // 0: SRM, 1: RRM
                        CurrentDisc = PROFILE_BDR_RRM;
                    else
                        CurrentDisc = PROFILE_BDR_SRM;
                    break;
            }
            break;
#endif

#if (ENABLE_HDDVD)
        /* HD-DVD PROFILE */
        case HD_DISC:
            CurrentDisc = PROFILE_HDROM;
            break;
#endif
    }

    WriteSDRAM( BHIGH( CurrentDisc ) );
    WriteSDRAM( BLOW ( CurrentDisc ) );

    SET_INT_ENABLE_MASK( temp_IE );     /* restore interrupts */
}


/*********************************************************************
*   Function :
*       GetAllFeatures()
*
*   Description :
*       Writes all the feature descriptors starting from
*       the given feature number
*
*   Parameters :
*       StartingFeatureNum
*********************************************************************/
static ULONG GetAllFeatures( USHORT StartingFeatureNum )
{
    ULONG   Addr;               /* DRAM address to write feature descriptor     */
    ULONG   DataLengthCount;    /* Total length in bytes of configuration data  */
    ULONG   DataLenTmp;         /* length in bytes of specified feature data    */
    BYTE    FeatureCnt, StCurrent;

    /*---------------------------------------------------------------
        Initial starting address location to write data and
        data length count
    ---------------------------------------------------------------*/
    Addr            = CFGDATA_ADR + 8;
    DataLengthCount = 0;
    DataLenTmp      = 0;

    /*---------------------------------------------------------------
        Write each feature to DRAM
    ---------------------------------------------------------------*/
    for ( FeatureCnt = 0; FeatureCnt < NUMBER_OF_FEATURES; FeatureCnt++ )
    {
        if ( Features[FeatureCnt] < StartingFeatureNum )
            continue;

        /*-----------------------------------------------------------
            Set h/w register pointer to DRAM
        -----------------------------------------------------------*/
        Iram_Buf_dram_addr = Addr;

        /*-----------------------------------------------------------
            Write feature to DRAM
        -----------------------------------------------------------*/
        DataLenTmp = GetSingleFeature( Features[FeatureCnt], &StCurrent );

        /*-----------------------------------------------------------
            Adjust DRAM pointer for writing next feature,
            and add data BYTE to Total length
        ------------------------------------------------------------*/
        Addr            += DataLenTmp;
        DataLengthCount += DataLenTmp;
    }

    return( DataLengthCount );
}


/*********************************************************************
*   Function :
*       GetActiveFeatures()
*
*   Description :
*       Writes all the active feature descriptors with the
*       current bit set starting from the given feature number
*********************************************************************/
static ULONG GetActiveFeatures( USHORT StartingFeatureNum )
{
    ULONG   Addr;
    ULONG   tempLength, DataLengthCount;
    BYTE    FeatureCnt, StCurrent;

    DataLengthCount = 0;                            /* init data length */
    Addr            = CFGDATA_ADR + 8;              /* addr to fill cofig data */

    /* write each feature to SDRAM */
    for ( FeatureCnt = 0; FeatureCnt < NUMBER_OF_FEATURES; FeatureCnt++ )
    {
        if ( Features[FeatureCnt] < StartingFeatureNum )
            continue;

        /*-----------------------------------------------------------
            Set h/w register pointer to DRAM
        -----------------------------------------------------------*/
        Iram_Buf_dram_addr = Addr;

        /*-----------------------------------------------------------
            Write feature to DRAM
        -----------------------------------------------------------*/
        tempLength = GetSingleFeature( Features[FeatureCnt], &StCurrent );

        if ( (StCurrent & bit0) )
        {                   /* if current bit set */
            /*-----------------------------------------------------------
                Adjust DRAM pointer for writing next feature,
                and add data BYTE to Total length
            ------------------------------------------------------------*/
            Addr            += tempLength;
            DataLengthCount += tempLength;
        }
    }

    return( DataLengthCount );
}


/*********************************************************************
*   Function :
*       GetSingleFeature()
*
*   Description :
*       This function writes a description of a given feature into SDRAM.
*
*   Parameters :
*       FeatureNumber   Feature number to be written in DRAM
*
*   Return value:
*       addr of Current status - used to check current bit setting
*       specified feature description size in bytes
*********************************************************************/
static ULONG GetSingleFeature( USHORT FeatureNumber, BYTE *StCurAdr )
{
    ULONG   FeatureLen;
    BYTE    temp_IE;
    BYTE    curStatus;
    USHORT  curDisc;
#if (SUN_FUNC_TEST == 0)
    BYTE    i;
#endif

    /*-------------------------------------------------------
        Disable interrupts
    -------------------------------------------------------*/
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    /*** Determine Feature Number ***/
    switch ( FeatureNumber )
    {
        /*-------------------------------------------------------
            Profile List                            Feature 0000h
        -------------------------------------------------------*/
        case FL_PROFILE_LIST:

            FeatureLen = sizeof( ProfileList );
            setCfgDATAtoDRAM((BYTE *) ProfileList,StCurAdr,FeatureLen,CUR_BIT_FIX );

            curDisc = getCurrentMediumInfo();
            Iram_Buf_dram_addr = CFGDATA_ADR + 8;
            setCurrentMediumBit( (curDisc & 0x1F) );
            break;

        /*-------------------------------------------------------
            Core                                    Feature 0001h
        -------------------------------------------------------*/
        case FL_CORE:

            FeatureLen = sizeof( Core );
            setCfgDATAtoDRAM((BYTE *) Core, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            Morphing                                Feature 0002h
        -------------------------------------------------------*/
        case FL_MORPHING:

            FeatureLen = sizeof( Morphing );
            setCfgDATAtoDRAM((BYTE *) Morphing, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            Removable Medium                        Feature 0003h
        -------------------------------------------------------*/
        case FL_REMOVABLE_MEDIA:

            FeatureLen = sizeof( RemovableMedium );
            setCfgDATAtoDRAM((BYTE *) RemovableMedium, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            Random Readable                         Feature 0010h
        -------------------------------------------------------*/
        case FL_RANDOM_READABLE:
            curStatus = CUR_BIT_OFF;
            if ( St_blank_disc == TRUE )
            {
                curStatus = CUR_BIT_OFF;
            }
            else if ( ! (getCurrentMediumInfo() & CUR_DISC_AUDIO) )
            {
                RandomReadable[9]=0x01;
                curStatus = CUR_BIT_ON;             /* Random readable == NOT Audio(cf.RedBook) */
            }

            if (pdbDiscLoaded() == DVD_DISC){
                RandomReadable[9]=0x10;
                curStatus = CUR_BIT_ON;
            }

            FeatureLen = sizeof( RandomReadable );
            setCfgDATAtoDRAM((BYTE *) RandomReadable, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            Multi Read                              Feature 001Dh
                        If this feature is to be considered,
                        the command READ TRACK/RZONE INFORMATION
                        needs to be implemented
        -------------------------------------------------------*/
        case FL_MULTI_READ:
            if((pdbServoGetDiscDiameter() == eNoDisc) || (pdbServoGetDiscDiameter() == eUnknownDiameter))      /* No disc */
                curStatus = CUR_BIT_OFF;
            else
                curStatus = CUR_BIT_ON;

            FeatureLen = sizeof( MultiRead );
            setCfgDATAtoDRAM((BYTE *) MultiRead, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            CD Read                                 Feature 001Eh
        -------------------------------------------------------*/
        case FL_CD_READ:
            if (pdbDiscLoaded() == CD_DISC)
            {
                if((pdbServoGetDiscDiameter() == eNoDisc) || (pdbServoGetDiscDiameter() == eUnknownDiameter))
                    curStatus = CUR_BIT_OFF;
                else
                    curStatus = CUR_BIT_ON;
            }
            else
                curStatus = CUR_BIT_OFF;
            FeatureLen = sizeof( cdRead );
            setCfgDATAtoDRAM((BYTE *) cdRead, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            DVD Read                                Feature 001Fh
        -------------------------------------------------------*/
        case FL_DVD_READ:
            if ((pdbDiscLoaded() == DVD_DISC) && (St_blank_disc != TRUE))
                curStatus = CUR_BIT_ON;
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( dvdRead );
            setCfgDATAtoDRAM( (BYTE*) dvdRead, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            Defect Management                       Feature 0024h
        -------------------------------------------------------*/
        case FL_DEFECT_MGT:
                            // DVD TBD?   DVD-RAM Defect Management. Code for DVD-RAM is not yet implemented in this feature.

            curStatus = CUR_BIT_OFF;

#if (DVD_RAM_READ == 1)
            if(pdbDiscLoaded() == DVD_DISC && pdbGetDVDType() == DVD_RAM)
            {
                curStatus = CUR_BIT_ON;
            }
#endif
            FeatureLen = sizeof( DefectMgt );
            setCfgDATAtoDRAM((BYTE *) DefectMgt, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            DVD+RW                                  Feature 002Ah
        -------------------------------------------------------*/
        case FL_DVD_PLUS_RW:

            if (pdbDiscLoaded() == DVD_DISC)
            {
                if (pdbGetDVDType()  == DVD_PLUS_RW || pdbGetDVDType() == DVD_PLUS_RW_DUAL)
                    curStatus = CUR_BIT_ON;
                else
                    curStatus = CUR_BIT_OFF;
            }
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( DvdPlusRW );
            setCfgDATAtoDRAM((BYTE *) DvdPlusRW, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            DVD+R                                   Feature 002Bh
        -------------------------------------------------------*/
        case FL_DVD_PLUS_R:

            if (pdbDiscLoaded() == DVD_DISC){
                if (pdbGetDVDType()  == DVD_PLUS_R)
                    curStatus = CUR_BIT_ON;
                else
                    curStatus = CUR_BIT_OFF;
            }
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( DvdPlusR );
            setCfgDATAtoDRAM((BYTE *) DvdPlusR, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            DVD+R                                   Feature 003Bh
        -------------------------------------------------------*/
        case FL_DVD_PLUS_R_DUAL:

            if (pdbDiscLoaded() == DVD_DISC){
                if (pdbGetDVDType() == DVD_PLUS_R_DUAL)
                    curStatus = CUR_BIT_ON;
                else
                    curStatus = CUR_BIT_OFF;
            }
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( DvdPlusRDL );
            setCfgDATAtoDRAM((BYTE *) DvdPlusRDL, StCurAdr, FeatureLen, curStatus );
            break;

#if (BD_READ)
        /*-------------------------------------------------------
            BD READ                                  Feature 0040h
        -------------------------------------------------------*/
        case FL_BD_READ:
            if (pdbDiscLoaded() == BD_DISC)
                curStatus = CUR_BIT_ON;
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( BdRead );
            setCfgDATAtoDRAM((BYTE *) BdRead, StCurAdr, FeatureLen, curStatus );
            break;
#endif
#if (HD_READ)
        /*-------------------------------------------------------
            HD READ                                 Feature 0050h
        -------------------------------------------------------*/
        case FL_HD_READ:
            if (pdbDiscLoaded() == HD_DISC)
                curStatus = CUR_BIT_ON;
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( HdRead );
            setCfgDATAtoDRAM((BYTE *) HdRead, StCurAdr, FeatureLen, curStatus );
            break;
#endif
        /*-------------------------------------------------------
            Power Management                        Feature 0100h
        -------------------------------------------------------*/
        case FL_POWER_MGT:

            FeatureLen = sizeof( PowerManagement );
            setCfgDATAtoDRAM((BYTE *) PowerManagement, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            CD Audio analog play                    Feature 0103h
        -------------------------------------------------------*/
        case FL_CD_AUDIO_ANALOG:

            curStatus = CUR_BIT_OFF;
#if (CEProduct != 1)
            if ( (getCurrentMediumInfo() & CUR_DISC_AUDIO) )
            {
                curStatus = CUR_BIT_ON;
            }
#endif

            FeatureLen = sizeof( cdAudioPlay );
            setCfgDATAtoDRAM((BYTE *) cdAudioPlay, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            Microcode Upgrade                       Feature 0104h
        -------------------------------------------------------*/
        case FL_MICROCODE_UPGRADE:

            FeatureLen = sizeof( MicroCodeUpgrade );
            setCfgDATAtoDRAM((BYTE *) MicroCodeUpgrade, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            Time-Out                                Feature 0105h
        -------------------------------------------------------*/
        case FL_TIME_OUT:

            FeatureLen = sizeof( TimeOut );
            setCfgDATAtoDRAM((BYTE *) TimeOut, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            DVD CSS                                 Feature 0106h
        -------------------------------------------------------*/
        case FL_DVD_CSS:

        #if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
            if ((pdbDiscLoaded() == DVD_DISC) && (pdbDVDDiscCSS() == eCSS_CPS))
        #else
            if (pdbDiscLoaded() == DVD_DISC)
        #endif
                curStatus = CUR_BIT_ON;
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( DVD_CSS );
            setCfgDATAtoDRAM( (BYTE*) DVD_CSS, StCurAdr, FeatureLen, curStatus );
            break;

        /*-------------------------------------------------------
            Real-Time Streaming                     Feature 0107h
        -------------------------------------------------------*/
        case FL_REALTIME_STREAMING:

            FeatureLen = sizeof( RealTimeStreaming );
            setCfgDATAtoDRAM((BYTE *) RealTimeStreaming, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

        /*-------------------------------------------------------
            Logical Unit Serial Number              Feature 0108h
        -------------------------------------------------------*/
        case FL_SERIAL_NUMBER:

#if (SUN_FUNC_TEST == 1)
            {
                BYTE temp,len,addlen;
                len = 0;
                addlen = 0;
                FeatureLen = 4;
                setCfgDATAtoDRAM((BYTE *) DefaultSerialNumber, StCurAdr, FeatureLen, CUR_BIT_FIX );
                while (len <= OEM_DRIVE_SN_MAX_SIZE_IN_EE) //the SN length define from sun710_oem_tbl.h,
                {

                    temp = GetEEPROMTable1B(OEM_DRIVE_SerialNumber_ADDR+len);

                    if(temp == 0xFF)
                        break;
                    else
                        WriteSDRAM(temp);
                    len++;
                }
                //calculate additional length, additional length must multiple of 4.
                addlen = 4 - (len%4);
                if(addlen!=4)
                {
                    len = len+addlen;
                    //fill ' ' in remainder byte.
                    while(addlen>0)
                    {
                        WriteSDRAM(0x20);
                        addlen--;
                    }
                }

                Iram_Buf_dram_addr = Iram_Buf_dram_addr - len - 1; //additional length /
                WriteSDRAM(len);//fill the correct additional length.
                FeatureLen = len + 4;
            }
#else
            FeatureLen = 4;
            setCfgDATAtoDRAM((BYTE *) DefaultSerialNumber, StCurAdr, FeatureLen, CUR_BIT_FIX );
            //FeatureLen = 0x14;
            FeatureLen = sizeof( DefaultSerialNumber );
            for( i = 0; i < FeatureLen-4; i++ )
            {
                if (i<0x10)
                {
                    WriteSDRAM(GetPersistentTable1B(SerialNumber_ADDR+i));
                }
                else
                    WriteSDRAM( 0x20 );
            }
#endif
            break;

        #if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
        /*-------------------------------------------------------
            DVD CPRM                               Feature 010Bh
        -------------------------------------------------------*/
        case FL_DVD_CPRM:

            if ((pdbDiscLoaded() == DVD_DISC) && (pdbDVDDiscCSS() == eCPRM_CPS))
            {
                curStatus = CUR_BIT_ON;
            }
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( DvdCprm );
            setCfgDATAtoDRAM((BYTE *) DvdCprm, StCurAdr, FeatureLen, curStatus );
            break;
        #endif

        /*-------------------------------------------------------
            Firmware Information                   Feature 010Ch
        -------------------------------------------------------*/
        case FL_FIRMWARE_INFO:

            FeatureLen = sizeof( FirmwareInfo );
            setCfgDATAtoDRAM((BYTE *) FirmwareInfo, StCurAdr, FeatureLen, CUR_BIT_FIX );
            break;

#if(ENABLE_AACS)
        /*-------------------------------------------------------
            AACS                                   Feature 010Dh
        -------------------------------------------------------*/
        case FL_AACS:
        #if ((CEProduct == 1) && (ENABLE_AACS_FEATURE == 0))
            curStatus = CUR_BIT_OFF;
        #else
            if (AacsStatus.AacsFlag.IsMediumCompliant == TRUE)
                curStatus = CUR_BIT_ON;
            else
                curStatus = CUR_BIT_OFF;
        #endif

            FeatureLen = sizeof(Aacs);
            setCfgDATAtoDRAM((BYTE *) Aacs, StCurAdr, FeatureLen, curStatus);
            break;
#endif

        #if (VCPS_TEST ==1)
        /*-------------------------------------------------------
            DVD VCPS                               Feature 0110h
        -------------------------------------------------------*/
        case FL_DVD_VCPS:

            if( (pdbDiscLoaded() == DVD_DISC) && (VCPS_disc == TRUE) )
            {
                curStatus = CUR_BIT_ON;
            }
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof( DvdVcps );

            setCfgDATAtoDRAM((BYTE *) DvdVcps, StCurAdr, FeatureLen, curStatus );
            break;
        #endif

#if (EN_LS == 1)
        /*-------------------------------------------------------
            OEM Feature 2
        -------------------------------------------------------*/
        case FL_LS:
            if (pdbGetDiscKind() == eLSDisc)
                curStatus = CUR_BIT_ON;
            else
                curStatus = CUR_BIT_OFF;

            FeatureLen = sizeof(LSFeature);
            setCfgDATAtoDRAM((BYTE *) LSFeature, StCurAdr, FeatureLen, curStatus);
            break;
#endif // End of (EN_LS == 1)

        /*-------------------------------------------------------
            Set size to 0 for non implemented feature
        -------------------------------------------------------*/
        default:

            FeatureLen = 0;
            break;
    }

    /*----------------------------------------
        Restore interrupts
    ----------------------------------------*/
    SET_INT_ENABLE_MASK( temp_IE );

    return ( FeatureLen );
}


/*********************************************************************
*   Function :
*       setCfgDATAtoDRAM()
*
*   Description :
*       This function writes a description in DRAM of a given feature
*       variable table.
*
*   Parameters :
*       featureTblPtr   - Feature table pointer to be written in DRAM
*       current_Status  - Specified feature's current bit status
*       len             - Feature table length to be written in DRAM
*       currentBit      - Specified feature's current bit setting
*
*   Return value:
*********************************************************************/
static void setCfgDATAtoDRAM( BYTE *featureTblPtr, BYTE *current_Status, ULONG len, BYTE currentBit )
{
    BYTE    data_len;

    for( data_len = 0; data_len < len; data_len++ )
    {
        if ( data_len == 2 )
        {
            WriteSDRAM( (featureTblPtr[data_len] | currentBit) );
            *current_Status = (featureTblPtr[data_len] | currentBit);
        }
        else
        {
            WriteSDRAM( featureTblPtr[data_len] );
        }
    }
}


/*********************************************************************
*   Function :
*       getCurrentMediumInfo()
*********************************************************************/
static USHORT getCurrentMediumInfo( void )
{
    USHORT CurrentMedium;
    BYTE dataType;

    CurrentMedium = CUR_CD_NO;

    /* Now medium is changing, default configuration return */
    if ( (Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE) )
    {
        return( CurrentMedium );
    }

    if (pdbDiscLoaded() == DVD_DISC)
    {
        switch (pdbGetDiscKind())
        {
            case eDVDROMDisc:
            case eDVDROMDLDisc:
                CurrentMedium = CUR_DVD_ROM;          /* DVD-ROM Present */
                break;

            case eDVDPlusRDisc:
            case eDVDPlusRDLDisc:
            case eDVDMinusRDisc:
            case eDVDMinusRDLDisc:
                CurrentMedium = CUR_DVD_RECORDABLE;   /* DVD+-R Present */
                break;

            case eDVDPlusRWDisc:
            case eDVDPlusRWDLDisc:
            case eDVDMinusRWDisc:
            case eDVDRAMDisc:
                CurrentMedium = CUR_DVD_REWRITABLE;   /* DVD+-RW Present */
                break;
        }
        return ( CurrentMedium );
    }

    //here is the CD disc medium
    switch (pdbGetDiscKind())
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
            CurrentMedium = CUR_CD_ROM;                         /* CD-ROM Present */
            break;

        default:
            CurrentMedium = CUR_CD_NO;                          /* Medium NOT Present */
            return( CurrentMedium );
    }

    /* If medium present in drive, check data type */
    if ( CurrentMedium == CUR_CD_ROM )
    {
        if ( St_cdrom_data )
        {
            CurrentMedium |= CUR_DISC_DATA;                     /* Data Disc */
        }
        else
        {
            CurrentMedium |= CUR_DISC_AUDIO;                    /* NOT Data Disc */
        }
    }
    else if ( CurrentMedium )
    {

            dataType = pdbGetFinalSessFormat();
            if ( ! dataType )
            {
                CurrentMedium |= CUR_DISC_AUDIO;                    /* Audio Disc */
            }
            else
            {
                CurrentMedium |= CUR_DISC_DATA;                     /* NOT Audio Disc */
            }
    }

    return ( CurrentMedium );
}


/*********************************************************************
*   Function :
*       setCurrentMediumBit()
*********************************************************************/
static void setCurrentMediumBit( BYTE mediumType )
{
    ULONG address;
    BYTE count, tmpData, discprofile1, discprofile2, discprofile3;

    discprofile1 = 0xFF;
    discprofile2 = 0xFF;
    discprofile3 = 0xFF;

    /*------------------------------------------------------
    //refernce to ProfileList[]
    //if you change ProfileList[] table, you have to modify this function!
    -------------------------------------------------------*/
    switch(pdbDiscLoaded())
    {
        case CD_DISC:         /*  CD DISC  */
            switch ( mediumType )
            {
                case CUR_CD_ROM:
                case CUR_CD_R:
                case CUR_CD_RW:
                case CUR_CD_HRW:
                case CUR_CD_USRW:
                    discprofile1 = PROFILE_CDROM;
                    break;

                default:
                    return;
            }
            break;

        case DVD_DISC:         /*  DVD DISC  */
            switch(pdbGetDVDType())
            {
                case DVD_ROM:
                case DVD_MINUS_R:
                case DVD_MINUS_RW:
                case DVD_PLUS_RW:
                case DVD_PLUS_RW_DUAL:
                case DVD_PLUS_R:
                case DVD_PLUS_R_DUAL:
    #if (SUPPORT_DVD_WRITE_ABILITY == P_SL_DISC)
                case DVD_MINUS_R:
                case DVD_MINUS_RW:
    #endif
                    discprofile1 = PROFILE_DVDROM;
                    break;
    #if (DVD_RAM_READ == 1)
                case DVD_RAM:
                    discprofile1 = PROFILE_DVDRAM;
                    break;
    #endif
                default:
                    return;
            }
            break;

#if (ENABLE_HDDVD)
        case HD_DISC:          /*  HD DVD DISC*/
            discprofile1 = PROFILE_HDROM;
            break;
#endif

#if (BD_ENABLE)
        case BD_DISC:          /*  BD  DISC*/
            discprofile1 = PROFILE_BDROM;
            break;
#endif
    }

    Iram_Buf_dram_addr += 1;
    address = Iram_Buf_dram_addr;
    for(count = 0; count < (sizeof(ProfileList)/4) ; count ++)
    {
        Iram_Buf_dram_addr = address;
        tmpData = ReadSDRAM();
        if( (tmpData == discprofile1)||(tmpData == discprofile2)
            ||(tmpData == discprofile3))
        {
            tmpData = ReadSDRAM();
            Iram_Buf_dram_addr--;
            WriteSDRAM( (tmpData | 0x01) );
         }
        address += 4;
    }


}

/*----------------------------------------- End Of File -----------------------------------------*/
