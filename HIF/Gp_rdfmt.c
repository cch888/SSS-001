/****************************************************************************
*                (c) Copyright 2001 -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:  gp_rdfmt.c
*
* DESCRIPTION:
*    Processes Read Format Capacities Command
*
* $Revision: 43 $
* $Date: 09/12/23 1:55p $
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
#include ".\hif\gp_rdfmt.h"
#include ".\hif\sv_util.h"
#include ".\servo\svo.h"
#include ".\player\plrdb.h"
#include ".\dvd\dvGvars.h"
#include ".\al\reg_intctrl.h"

#if (BD_ENABLE)
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#endif  //(BD_ENABLE)

/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
static void     CreateReadFmtCapacitiesHdr      ( BYTE );
static BYTE     GetCurMaxCapacities             ( void );
static BYTE     GetDescriptorType               ( void );
static BYTE     GetFormattableCapacities        ( void );

/*********************************************************************
 LOCAL DEFINITION
*********************************************************************/
#define RDFMTCAP_HDR_SIZE   0x04    /* size of capacity header    */
#define DESCRIPTOR_SIZE     0x08    /* size of capacity descriptor */

/* User Data Area Size Definition */
#define DATA_SIZE_M2F1      0x800   /* in CD-ROM XA */
#define DATA_SIZE_DA        0x930   /* in CD-DA     */
/* Format Type Definition */
#define FMT_TYPE_FULL_FMT   0x10

#if (BD_ENABLE)
#define FMT_TYPE_BDRE_BLOCK_SA_FMT     0x30
#define FMT_TYPE_BDRE_BLOCK_NO_SA_FMT  0x31
#define FMT_TYPE_BDR_BLOCK_FMT         0x32
#endif  /* BD_ENABLE */

/* Fixed Packet Format Size Definition */
#define FIX_PACKET_SIZE     0x20

/*********************************************************************
*   Function :
*       ReadFormatCapacitiesCommand()
*
*   Description :
*       This is the command handler for the Read Format Capacities command
*
*********************************************************************/
void ReadFormatCapacitiesCommand()
{
    vUSHORT Temp;
    USHORT  allocLen;               /* max num of bytes fot xfer data to HOST */
    BYTE    numOfRes, listLen;      /* length in bytes of Capacities Desciptor */

    if((TrayManagerInfo.TrayState == TRAY_STATE_OUT_EXECUTING) ||(TrayManagerInfo.TrayState == TRAY_STATE_OUT_OK))
    {//Fixed Nero Info Tool 5 show incorrect value(126X) in read speed item.
        BUILD_SENSE(0x02,0x3A,0x02,0x04); /* MEDIUM NO PRESENT, TRAY OPEN */
        AutoCommandComplete();
        return;
    }
    /*---------------------------------------------------------------
        Initial local variables
    ---------------------------------------------------------------*/
    allocLen = 0;

    /*---------------------------------------------------------------
        Parse the command block
    ---------------------------------------------------------------*/
    allocLen = ((USHORT)(Iram_Cmd_block.bf[7] << 8) + (USHORT)Iram_Cmd_block.bf[8]);

    /*---------------------------------------------------------------
        Determine if a transfer is necessary.
    ---------------------------------------------------------------*/
    if ( allocLen == 0 )
    {               /* no data xfer, not an error */
        AutoCommandComplete();
        return;
    }

    /*---------------------------------------------------------------
        Call the function to write the Current/Maximum capacities
        data into SDRAM
    ---------------------------------------------------------------*/
    numOfRes = GetCurMaxCapacities();

    /*---------------------------------------------------------------
        Check allocation length
    ---------------------------------------------------------------*/
    listLen = (numOfRes * DESCRIPTOR_SIZE);     /* size of Descriptor is 8 Bytes */

    /*---------------------------------------------------------------
        Create Capacity List header in SDRAM, fill data length
    ---------------------------------------------------------------*/
    CreateReadFmtCapacitiesHdr( listLen );

    /*---------------------------------------------------------------
        Set buffer starting address and BYTE count
    ---------------------------------------------------------------*/
    Temp = (listLen + RDFMTCAP_HDR_SIZE);

    /* check allocation length */
    if ( Temp > allocLen )
    {
        Temp = allocLen;
    }

    SendParameterDataToHost(RDFMTCAP_ADR,Temp , TRUE);  // Send Data and Command Complete.
}

/*********************************************************************
*   Function :
*       CreateReadFmtCapacitiesHdr()
*
*   Description :
*       This function creates the Capacity List Header in SDRAM area.
*
*   Parameters :
*       ListLen  -  Size of the return Capacity List to be
*                   transferred to HOST
*
*   Return value:
*       None
*
*   << NOTE >>
*       This header is 4 Bytes.
*
*********************************************************************/
static void CreateReadFmtCapacitiesHdr( BYTE ListLen )
{
    BYTE    temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();

    /* point to Format Capacity address in SDRAM */
    Iram_Buf_dram_addr = RDFMTCAP_ADR;

    /* fill 0 to reserved bytes(bytes 0-2) */
    WriteSDRAM( 0x00 );
    WriteSDRAM( 0x00 );
    WriteSDRAM( 0x00 );

    /* Write Capacity List size (BYTE 3) */
    WriteSDRAM( ListLen );

    SET_INT_ENABLE_MASK( temp_IE );     /* restore interrupts */
}

/*********************************************************************
*   Function :
*       UpdateFormatDescriptor()
*
*   Description :
*       This function writes the format descriptor to SDRAM buffer
*
*   Parameters :
*       address - SDRAM address
*       numOfBlk - Number of blocks
*       type - Format/descriptor type
*       param - Type dependent parameter
*
*   Return value:
*       None
*********************************************************************/
void UpdateFormatDescriptor(ULONG address, ULONG numOfBlk, BYTE type, ULONG param)
{
    Iram_Buf_dram_addr = address;
    WriteSDRAM((BYTE)(numOfBlk >> 24));    /* number of addressable blocks */
    WriteSDRAM((BYTE)(numOfBlk >> 16));
    WriteSDRAM((BYTE)(numOfBlk >> 8));
    WriteSDRAM((BYTE)numOfBlk);
    WriteSDRAM(type);                      /* descriptor/format type */
    WriteSDRAM((BYTE)(param >> 16));       /* Type dependent parameters */
    WriteSDRAM((BYTE)(param >> 8));
    WriteSDRAM((BYTE)param);
}

/*********************************************************************
*   Function :
*       GetCurMaxCapacities()
*
*   Description :
*       This function creates the Current/Maximum Capacity data
*       in SDRAM area.
*
*   Parameters :
*       None
*
*   Return value:
*       numOfList - Number of descriptors in returning DATA
*
*   << NOTE >>
*       numOfList is added GetFormattableCapacities()'s return value.
*
*********************************************************************/
static BYTE GetCurMaxCapacities( void )
{
    BYTE        numOfList;
    BYTE        curDiscType;
    BYTE        TrNo;
#if (BD_ENABLE)
    BYTE        lastTrackNum;
#endif

    TimeS       lastMSF;
    /* Default values when medium NOT present */
    ULONG   numOfBlk = 0x3FB000L;       /* the Max Capacity that the drive can read  */
    BYTE    type     = 0x03;            /* Descriptor/Format type                    */
    ULONG   blkLen   = 0x800L;          /* the length in bytes  of each logical block */

    /* Initial Number of descriptors */
    numOfList = 0;

    /* Now medium is chainging or NOT present, default performance return */
    if ( (Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE) )
    {
        UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE), numOfBlk,
                type, blkLen);
        numOfList++;                    /* Count Up Descriptor */
        return( numOfList );
    }

    /* Make Current/Maximum Descriptor */
    curDiscType = pdbGetDiscKind();
    switch( curDiscType )
    {
        case eCDROMDisc:
            TrNo    = pdbGetFinalSessLastTrackNumber();
            lastMSF = pdbGetStopTime( TrNo );
            numOfBlk = MSF2LBA( &lastMSF );
            if ( St_cdrom_audio == TRUE )
            {
                blkLen = DATA_SIZE_DA;      /* Audio Disc */
            }
            else
            {
                blkLen = DATA_SIZE_M2F1;    /* other Disc */
            }
            break;

        case eCDRDisc:
            if ( pdbGetFinalSessCloseInfo() == eSessionFinalized )
            {   /* Closed Disc */
                TrNo    = pdbGetFinalSessLastTrackNumber();
                lastMSF = pdbGetStopTime( TrNo );
            }
            else
            {                                                   /* NOT Closed Disc */
                lastMSF = pdbGetATIPLeadOut();
            }
            numOfBlk = MSF2LBA( &lastMSF );

            if ( St_cdrom_audio == TRUE )
            {
                blkLen = DATA_SIZE_DA;      /* Audio Disc */
            }
            else
            {
                blkLen = DATA_SIZE_M2F1;    /* other Disc */
            }
            break;

        case eCDRWDisc:
            lastMSF = pdbGetATIPLeadOut();
            numOfBlk = MSF2LBA( &lastMSF );
            blkLen   = DATA_SIZE_M2F1;
            break;

        case eDVDROMDisc:
            numOfBlk = pdbGetDVDMaxLBA();
            blkLen = 0x800;
            break;

        default:
            return( numOfList );
    }

    type = GetDescriptorType();

    #if (BD_ENABLE)
    if(type == 0x01)    //blank disc
    {
        switch( curDiscType )
        {
            case eBDRDisc:
                blkLen = BDR_MAX_ISA0_SIZE + BDR_MAX_OSA_SIZE;
                numOfBlk = PAA2PSN_FACTOR * (pdbGetBDL0EndPAA() + 0x02) -
                    (BD_DATAZONE_START_PSN + BDR_MAX_ISA0_SIZE * BD_CLUSTER_SIZE);
                break;

            case eBDRDLDisc:
                blkLen = BDR_MAX_ISA0_SIZE + 2 * BDR_MAX_OSA_SIZE + BDR_MAX_ISA1_SIZE;
                numOfBlk = PAA2PSN_FACTOR * (pdbGetBDL0EndPAA() + 0x02) -
                    (BD_DATAZONE_START_PSN + BDR_MAX_ISA0_SIZE * BD_CLUSTER_SIZE);
                numOfBlk += PAA2PSN_FACTOR * (pdbGetBDL1EndPAA() + 0x02 - pdbGetBDL1StartPAA());
                break;

            case eBDREDisc:
                blkLen = BDRE_MAX_ISA0_SIZE + BDRESL_MAX_OSA_SIZE;
                numOfBlk = PAA2PSN_FACTOR * (pdbGetBDL0EndPAA() + 0x02) -
                    (BD_DATAZONE_START_PSN + BDRE_MAX_ISA0_SIZE * BD_CLUSTER_SIZE);
                break;

            case eBDREDLDisc:
                blkLen = BDRE_MAX_ISA0_SIZE + 2 * BDREDL_MAX_OSA_SIZE + BDRE_MAX_ISA1_SIZE;;
                numOfBlk = PAA2PSN_FACTOR * (pdbGetBDL0EndPAA() + 0x02) -
                    (BD_DATAZONE_START_PSN + BDRE_MAX_ISA0_SIZE * BD_CLUSTER_SIZE);
                numOfBlk += PAA2PSN_FACTOR * (pdbGetBDL1EndPAA() + 0x02 - pdbGetBDL1StartPAA());
                break;
        }
    }
    else if (type == 0x02)  /* formatted disc */
    {
        switch( curDiscType )
        {
            case eBDROMDisc:
            case eBDROMDLDisc:
                numOfBlk = pdbGetBDMaxLBA();
                blkLen = 0x800;
                break;

            case eBDRDisc:
                blkLen = pdbGetBDIsa0Size() + pdbGetBDOsaSize();

                switch(pdbGetBDRecMode())
                {
                    case SRM_NO_LOW:

                        if ((pdbGetBDFinalSessCloseInfo() == eSessionFinalized) || (pdbGetBDFinalSessCloseInfo() == eSessionClosed))
                            lastTrackNum = pdbGetDVDFinalSessionNumber();
                        else
                            lastTrackNum = pdbGetDVDFinalSessionNumber() - 1;

                        numOfBlk = 0;
                        for (TrNo = 1; TrNo < lastTrackNum; TrNo++)
                        {
                            numOfBlk += pdbGetBDTrackEndPSN(TrNo) - pdbGetBDTrackStartPSN(TrNo) + 1;
                        }
                        break;

                    case SRM_AND_LOW:
                    case RRM_AND_LOW:
                    case RRM_NO_LOW:
                        numOfBlk = pdbGetBDEndLSN() + 1;
                        break;
                }
                break;

            case eBDRDLDisc:
                blkLen = pdbGetBDIsa0Size() + pdbGetBDIsa1Size() + 2* pdbGetBDOsaSize();

                switch(pdbGetBDRecMode())
                {
                    case SRM_NO_LOW:
                        if ((pdbGetBDFinalSessCloseInfo() == eSessionFinalized) || (pdbGetBDFinalSessCloseInfo() == eSessionClosed))
                            lastTrackNum = pdbGetDVDFinalSessionNumber();
                        else
                            lastTrackNum = pdbGetDVDFinalSessionNumber() - 1;

                        numOfBlk = 0;
                        for (TrNo = 1; TrNo < lastTrackNum; TrNo++)
                        {
                            numOfBlk += pdbGetBDTrackEndPSN(TrNo) - pdbGetBDTrackStartPSN(TrNo) + 1;    //track across layer ??
                        }
                        break;

                    case SRM_AND_LOW:
                    case RRM_AND_LOW:
                    case RRM_NO_LOW:
                        numOfBlk = pdbGetBDEndLSN() + 1;
                        break;
                }
                break;

            case eBDREDisc:
                blkLen = pdbGetBDIsa0Size() + pdbGetBDOsaSize();
                numOfBlk = pdbGetBDEndLSN() + 1;
                break;

            case eBDREDLDisc:
                blkLen = pdbGetBDIsa0Size() + pdbGetBDIsa1Size() + 2* pdbGetBDOsaSize();
                numOfBlk = pdbGetBDEndLSN() + 1;
                break;
        }
    }
    #endif  //(BD_ENABLE)

    /* Copy Current/Maximum Descriptor to SDRAM area */
    UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE),
            numOfBlk, type, blkLen);

    numOfList++;                        /* Count Up Descriptor */

    /* If current disc is CD-RW/MRW, capacity list is pluse Formattable Descriptor */
    if ( pdbGetCDRW() || pdbGetDVDRW() || (pdbDiscLoaded() == DVD_DISC)&&(pdbGetDVDType() == DVD_RAM))
    {
        numOfList += GetFormattableCapacities();
    }

#if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC && (pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc
        || pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc))
    {
        numOfList += GetFormattableCapacities();
    }
#endif  //(BD_ENABLE)

    return( numOfList );
}

/*---------------------------------------------------------
    Function :
        GetDescriptorType()
---------------------------------------------------------*/
static BYTE GetDescriptorType( void )
{
    BYTE    type;

    switch(pdbServoGetDiscType())
    {
        case eNonATIPDisc:
            type = 0x02;            /* Formatted Disc */
            break;

        case eATIPDisc:
            if ( St_blank_disc == TRUE )
            {
                type = 0x01;        /* Unformatted Disc */
            }
            else
            {
                if ( pdbGetFinalSessCloseInfo() == eNotSessionClosed )
                {
                    type = 0x03;    /* NOT Close Session Disc */
                }
                else
                {
                    type = 0x02;    /* Formatted Disc */
                }
            }
            break;

#if (BD_ENABLE)
        case eBDDisc:
            if ( St_blank_disc == TRUE )
            {
                type = 0x01;        /* Unformatted Disc */
            }
            else
            {
                if ( pdbGetBDFinalSessCloseInfo() == eNotSessionClosed )
                {
                    type = 0x03;    /* NOT Close Session Disc */
                }
                else
                {
                    type = 0x02;    /* Formatted Disc */
                }
            }
            break;
#endif

    }

    return( type );
}

/*---------------------------------------------------------
    Function :
        GetFormattableCapacities()
---------------------------------------------------------*/
static BYTE GetFormattableCapacities( void )
{
    BYTE        numOfFmtList;
    TimeS       lastMSF;
    ULONG       lastLBA, packet_size;
    ULONG       numOfBlk;   /* the number of addressable blocks          */
    BYTE        type;       /* Descriptor/Format type                    */
    ULONG       blkLen;     /* the length in bytes of each logical block */

    /* Initial Number of descriptors */
    numOfFmtList = 0;

#if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC)
    {
        switch(pdbGetDiscKind())
        {
            case eBDRDisc:
            case eBDRDLDisc:
                /***** Format 0x00: totla number of addressable blocks + SA size in cluster *****/
                if (pdbGetBDLayers() == BD_DUAL_LAYER)
                    blkLen   = pdbGetBDIsa0Size() + pdbGetBDIsa1Size() + 2* pdbGetBDOsaSize();   //SA in cluster
                else
                    blkLen   = pdbGetBDIsa0Size() + pdbGetBDOsaSize();   //SA in cluster

                numOfBlk = pdbGetBDEndLSN() + 1 + blkLen;
                type     = 0;   /* Full format */

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                /***** Format 0x32: totla number of addressable blocks *****/
                //1st descriptor, vendor preferred
                type     = FMT_TYPE_BDR_BLOCK_FMT;
                numOfBlk = 0;
                blkLen   = 0;

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                //2nd descriptor, maximum Spare Area sizes
                if (pdbGetBDLayers() == BD_DUAL_LAYER)
                    numOfBlk = BDR_MAX_ISA0_SIZE + 2 * BDR_MAX_OSA_SIZE + BDR_MAX_ISA1_SIZE;
                else
                    numOfBlk = BDR_MAX_ISA0_SIZE + BDR_MAX_OSA_SIZE;
                //blkLen   = 0;

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                //3rd descriptor, minimum (but non-zero) Spare Area sizes
                numOfBlk = BDR_MAX_ISA0_SIZE;
                //blkLen   = 0;

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);

                break;

            case eBDREDisc:
            case eBDREDLDisc:
                /***** Format 0x00: total number of addressable blocks + SA size in Cluster *****/
                type     = 0;   /* Full format */
                if (pdbGetBDLayers() == BD_DUAL_LAYER)
                {
                    numOfBlk = BDRE_FULL_FORMAT_ISA0_SIZE + 2 * BDRE_FULL_FORMAT_OSA_SIZE + BDRE_FULL_FORMAT_ISA1_SIZE;
                    blkLen   = pdbGetBDIsa0Size() + pdbGetBDIsa1Size() + 2* pdbGetBDOsaSize();   //SA in cluster
                }
                else
                {
                    numOfBlk = BDRE_FULL_FORMAT_ISA0_SIZE + BDRE_FULL_FORMAT_OSA_SIZE;
                    blkLen   = pdbGetBDIsa0Size() + pdbGetBDOsaSize();   //SA in cluster
                }

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                /***** Format 0x30: totla number of addressable blocks + SA size in cluster *****/
                //1st descriptor, vendor preferred
                type     = FMT_TYPE_BDRE_BLOCK_SA_FMT;
                numOfBlk = 0;   //vendor preferred
                if (pdbGetBDLayers() == BD_DUAL_LAYER)
                    blkLen = pdbGetBDIsa0Size() + 2 * pdbGetBDIsa0Size() + pdbGetBDIsa1Size();
                else
                    blkLen = pdbGetBDIsa0Size() + pdbGetBDOsaSize();

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                //2nd descriptor, maximum Spare Area sizes
                //blkLen remains the same
                if (pdbGetBDLayers() == BD_DUAL_LAYER)
                    numOfBlk = BDRE_MAX_ISA0_SIZE + 2 * BDREDL_MAX_OSA_SIZE + BDRE_MAX_ISA1_SIZE;
                else
                    numOfBlk = BDRE_MAX_ISA0_SIZE + BDRESL_MAX_OSA_SIZE;

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                //3rd descriptor, minimum (but non-zero) Spare Area sizes
                //blkLen remains the same
                numOfBlk = BDRE_MAX_ISA0_SIZE;

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);


                /***** Format 0x31: total number of addressable blocks and
                        block size used for formatting the whole media *****/
                type     = FMT_TYPE_BDRE_BLOCK_NO_SA_FMT;
                //totla number of addressable blocks without SA
                numOfBlk = PAA2PSN_FACTOR * (pdbGetBDL0EndPAA() + 0x02) - BD_DATAZONE_START_PSN;
                blkLen   = numOfBlk * BD_SECTOR_SIZE;

                /* Copy Descriptor to SDRAM area */
                numOfFmtList++;             /* Count Up Descriptor */
                UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + numOfFmtList * DESCRIPTOR_SIZE),
                    numOfBlk, type, blkLen);

                break;
        }
    }
    else
#endif  //(BD_ENABLE)
    if ((pdbDiscLoaded() == CD_DISC) && pdbGetCDRW())
    {
        /* convert Method-1 LBA into Method-2 LBA */
        lastMSF     = pdbGetATIPLeadOut();
        lastLBA     = MSF2LBA( &lastMSF );
        packet_size = FIX_PACKET_SIZE;
        lastLBA     = (lastLBA + 8L) / (packet_size + 7L) * packet_size;

        /* Make Formattable Capacitites Descriptor */
        numOfBlk = lastLBA;
        type     = (FMT_TYPE_FULL_FMT << 2);
        blkLen   = FIX_PACKET_SIZE;

        /* Copy Current/Maximum Descriptor to SDRAM area */
        UpdateFormatDescriptor((RDFMTCAP_ADR + RDFMTCAP_HDR_SIZE + DESCRIPTOR_SIZE),
                numOfBlk, type, blkLen);

        numOfFmtList++;             /* Count Up Descriptor */
    }
    return( numOfFmtList );
}
