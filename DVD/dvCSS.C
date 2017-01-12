/*****************************************************************************
*                 (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME:   dvCSS.C
*
* DESCRIPTION
*   This file contains css common routines
*
* $Revision: 45 $
* $Date: 10/12/21 4:17p $

**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
  INCLUDE FILES
*********************************************************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\scomm.h"
#include ".\hif\rd_util.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvCSS.h"
#include ".\dvd\dvdiscinit.h"
#include ".\dvd\dvMMC.h"
#include ".\oem\oem_tbl.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvbufmgr.h"
#include ".\al\reg_dvd_decoder.h"

/*********************************************************************
 GLOBAL VARIABLES
*********************************************************************/

static BOOL cssSendTitleKey(void);
static void cssComplete(void);
void cssValidateRegion(void);
/*********************************************************************
* Name:         cssInit
*
* Description:  Initialize global variables used by CSS
*               Reset AUTHEN
*
* Params:       none
*
* Returns:      none
*
* Global outputs: dvdCss.AuthenticationGrantId, fAgidInUse, fAuthenticationSuccess, dvdCss.eCssState
*
* Notes:        This must be performed prior to any CSS activity,
*               called by power-on-reset and atapi reset
*
*********************************************************************/
void cssInit(void)
{
    dvdCss.AuthenticationGrantId         = CLEAR;
    dvdCss.CssFlag.AgidInUse             = FALSE;
    dvdCss.CssFlag.AuthenticationSuccess = FALSE;
    dvdCss.eCssState                     = CSS_START;
}

/*********************************************************************
* Name:         WaitForCssCompleted
*
* Description:  poll until bCSSBSY is reset or 20 msec expired
*
* Params:       none
*
* Returns:      0 = done
*               1 = timer expired
*
* Global outputs: none
*
* Notes:
*
*********************************************************************/
BYTE WaitForCssCompleted(void)
{
    ULONG clock;

    StartTimer(&clock);
    while(1)
    {
        if(ReadTimer(&clock) > 10L)
        {
            return(FAIL);     // 10 ms timeout has occured.
        }

        if (READ_FIELD( HAL_DVDD_CSSBSY) == 0)
        {
            return(PASS);     //  CSS Authentication command has completed
        }
    }
}

/*********************************************************************
* Name:         cssValidateAgid
*
* Description:  Validate AGID for CSS authentication
*
* Returns:      0 if Agid invalid
*               1 if Agid valid
*
* Notes:        The Agid passed in cmd packet is checked
*               for validity.
*
*********************************************************************/
int cssValidateAgid(BYTE SentAgid)
{
    if (SentAgid == dvdCss.AuthenticationGrantId)
        return(FAIL);
    else
        return(PASS);
}

/*********************************************************************
* Name:         cssCommon
*
* Description:  This routine continues processing after chip
*               processing is complete, and proceed per dvdCss.eCssState
*
* Returns:      void
* Params:       none
*
* Global inputs:    dvdCss.eCssState
* Global outputs:   dvdCss.eCssState (updated)
*
* Notes:
*
*********************************************************************/
BOOL cssCommon(void)
{
    StLongU ActualID;
    DiscAccessStatusE DiscReadStatus;
    WORD   HostTransferLength;
    volatile BYTE *DramAddressP;
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
    volatile BYTE *BcaInfoP;
#endif
    ULONG ControlBlockAddress;
    BYTE i;

    HAL_Reg8 *RegPtr;

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    switch (dvdCss.eCssState)
    {
        case DRIVE_RESPONSE:        // Drive's response to host's Drive Challenge
            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

            //  Prepare Header
            *DramAddressP++ = 0;         // Allocation
            *DramAddressP++ = 0x0A;      // Length
            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;         // Reserved

            RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSRES4));
            for (i = 0; i < 5; i++)
                *DramAddressP++ = RegPtr[i];

            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;         // Reserved

            // Data transfer to host
            SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(12,HostTransferLength), FALSE);  // No Command Complete

            dvdCss.eCssState = DRIVE_RESPONSE_COMPLETE;   // New CSS State
            break;

        case DECCHAL:
            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

            *DramAddressP++ = 0x00;      // Allocation
            *DramAddressP++ = 0x0E;      // Length
            *DramAddressP++ = 0x00;      // Reserved
            *DramAddressP++ = 0x00;      // Reserved

            RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL9));
            for (i = 0; i < 10; i++)
                *DramAddressP++ = RegPtr[i];

            *DramAddressP++ = 0x00;      // Reserved
            *DramAddressP++ = 0x00;      // Reserved

            // Data transfer to host
            SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(16,HostTransferLength), FALSE);  // No Command Complete

            dvdCss.eCssState = DECCHALCMPLT;
            break;

        case DECRSP:    // Send key format 3
            if (READ_FIELD( HAL_DVDD_CSSERR))                   // Test for CSS Error
            {   //   CSS Error
                dvdCss.CssFlag.AuthenticationSuccess = FALSE;
                dvdCss.eCssState = AUTHENTICATION_FAILED;   // CSS key xchg failure
                BUILD_SENSE(0x05,0x6F,0x00,0x04);                // COPY PROTECTION KEY EXCHANGE FAILURE - AUTHENTICATION FAILURE
            }
            else
            {                                               // CSS success
                dvdCss.CssFlag.AuthenticationSuccess = TRUE;
                dvdCss.eCssState = AUTHENTICATION_COMPLETE;
            }
            AutoCommandComplete();
            break;

        case READ_TITLE_KEY:    /*TITLE KEY*/

            if (cssSendTitleKey() == PASS);    //  Read Title Key and send to host.
                dvdCss.eCssState = TITLE_KEY_HAS_BEEN_SENT;

            cssComplete();
            AutoCommandComplete();
            break;

        case SEND_DISC_KEY_STRUCTURES_TO_HOST:
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
            if(pdbDVDDiscCSS() == eNO_CPS)
#else
            if(pdbDVDDiscCSS() == FALSE)
#endif
            {
                dvdCss.CssFlag.AgidInUse = FALSE;
                dvdCss.eCssState = ILLEGAL_CSS_STATE;
                BUILD_SENSE(0x05,0x6F,0x01,0x01);    // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT
                return(FAIL);
            }

            send_msg80(SEND_READ_DVD_MSG,0x710102);
            // Now read Leadin block
            ControlBlockAddress = START_ADDRESS_OF_CONTROL_DATA;
            DiscReadStatus = dvdBufferControlDataBlock(0, ControlBlockAddress);       // Read Leadin area of layer 0 into Data Buffer
            if (DiscReadStatus != BUF_GOOD)
            {
                dvdCss.CssFlag.AgidInUse = FALSE;
                dvdCss.eCssState = ILLEGAL_CSS_STATE;
                BUILD_SENSE(0x04,0x44,0x00,0x05);               // INTERNAL TARGET FAILURE
                return(FAIL);
            }


            /*-----------------------------------------------------------
                Read the ID for sector 2 in the AUX area.
            ------------------------------------------------------------*/
            DramAddressP = (BYTE *)(CUR_AUX_ADDRESS() + AUX_SECTOR2_OFFSET);

            ActualID.byte.u = *DramAddressP++;
            ActualID.byte.h = *DramAddressP++;
            ActualID.byte.m = *DramAddressP++;
            ActualID.byte.l = *DramAddressP;
            DVD_MESSAGE1(PRN_DVDHIF, "CSS Read DiscKey Structures success actual ID = ", 4, ActualID.Long);

            /*Read Disc KEY*/
            HostTransferLength = MINIMUM(0x804, HostTransferLength);
            HostTransferLength -= 4;

            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = BHIGH(HostTransferLength);
            *DramAddressP++ = BLOW(HostTransferLength);
            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;         // Reserved

            // Data transfer to host
            SendParameterDataToHost(DVD_CSS_SCRATCH, 4, FALSE);             // Transfer Header to host. No Command Complete

            WRITE_FIELD( HAL_HST_CSSENCS, 0);       // Specify CSS encrypted area start position
            WRITE_FIELD( HAL_HST_CSSENCE, 2047);    // Specify CSS encrypted area end position
            WRITE_FIELD( HAL_HST_CSSSCREN, 1);      // Enable CSS Scrambling

            SendParameterDataToHost(DVD_DATA_BUFFER_START + (2 * 0x800), HostTransferLength , FALSE);   // Transfer Disc Key Structures.

            WRITE_FIELD( HAL_HST_CSSSCREN, 0);      // Disable CSS Scrambling

            ClearDataBuffer();      // Clear the DVD buffer
            dvdCss.eCssState = CSS_DISC_KEY_SENT;
            cssComplete();

            break;

#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
        case GENERATE_BUS_KEY_FOR_MEDIA_IDENTIFIER:   // Media Identifier
            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

            //  Prepare Header
            *DramAddressP++ = 0;        // Allocation
            *DramAddressP++ = 0x16;     // Length
            *DramAddressP++ = 0;        // Reserved
            *DramAddressP++ = 0;        // Reserved

            BcaInfoP = (BYTE *)(SCSDRAM_START_ADDR + DVD_BCA);
            for(i =0 ; i < 8; i++)
            {
                *DramAddressP++ = BcaInfoP[4+i];     // Read 8 bytes Media ID
            }

            RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL9));
            for (i = 0; i < 10; i++)
                *DramAddressP++ = RegPtr[i];

            *DramAddressP++ = 0x00;     // Reserved
            *DramAddressP++ = 0x00;     // Reserved

            // Data transfer to host
            SendParameterDataToHost(DVD_CSS_SCRATCH, 24, FALSE);  // No Command Complete

            dvdCss.eCssState = AUTHENTICATION_COMPLETE;

            break;

        case GENBKFORMK:                        // Read Media Key Block
                                                // LeadIn data
            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

            //  Prepare Header
            *DramAddressP++ = 0x60;                         // Allocation
            *DramAddressP++ = 0x02;                         // Length
            *DramAddressP++ = 0;                            // Reserved
            *DramAddressP++ = Info.Media.DVD.TotalMKBPacks; // Total MKB Packs Used

            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + 4*0x800);
            RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL9));
            for (i = 0; i < 10; i++)
                *DramAddressP++ = RegPtr[i];     // Read 10 bytes DVD-MAC (MKB Hash)

            // Data transfer to host
            SendParameterDataToHost(DVD_CSS_SCRATCH, 4, FALSE);  // No Command Complete

            for(i = 4; i <= 15; i++)
                SendParameterDataToHost(i*0x800, 0x800, FALSE);   // No Command Complete

            dvdCss.eCssState = AUTHENTICATION_COMPLETE;

            break;
#endif  //(ENABLE_CPRM == 1 && BCA_ENABLE == 1)

        default:
            dvdCss.eCssState = ILLEGAL_CSS_STATE;       // Illegal state - should never get here.
    }/*ENDSWITCH: dvdCss.eCssState for cssCommon */
    return(PASS);
}

/*********************************************************************
* Name:         cssInvalidateAgid
*
* Description:  Invalidate AGID for CSS authentication
*
* Params:       none
*
* Returns:      none
*
* Global outputs:
*               fAgidInUse, DvdCss.AuthenticationGrantId, fAuthenticationSuccess, DvdCss.eCssState
*
* Notes:        The AGID passed in the cmd packet must
*               first be checked for validity
*
*               According to Mt Fuji Rev0.95, this command
*               allows the host to reset hung authentication
*               processes in the drive, so CSS Reset is activated
*********************************************************************/
void cssInvalidateAgid(void)
{
    dvdCss.AuthenticationGrantId    = CLEAR;
    dvdCss.CssFlag.AgidInUse        = FALSE;
    dvdCss.eCssState                = CSS_START;

    WRITE_FIELD(HAL_DVDD_RSTCSS,1);   // Reset CSS hardware
    WaitTimer(1L);                    // Wait 1ms */
    WRITE_FIELD(HAL_DVDD_RSTCSS,0);

#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
    WRITE_FIELD(HAL_DVDD_ACCODE, 0x09);
#endif
}

/*********************************************************************
* Name:         cssSendTitleKey
*
* Description:  Function to send CSS Title Key to host
*
* Returns:      void
* Params:       none
*
*
* Notes:
*
*********************************************************************/
static BOOL cssSendTitleKey(void)
{
    StLongU ActualID;
    BYTE  * AuxDataP;
    BYTE  * DramAddressP;
    WORD    HostTransferLength;
    BYTE    SectorNumber;
    BYTE    BufRetry;
    volatile DiscAccessStatusE DiscReadStatus;

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    // Now we need to read an ecc block of data from the requested LBA
    ClearDataBuffer();
    HostXfer.StartBlock.byte.u = Iram_Cmd_block.bf[2];   // Get the host request starting LBA from the command block
    HostXfer.StartBlock.byte.h = Iram_Cmd_block.bf[3];
    HostXfer.StartBlock.byte.m = Iram_Cmd_block.bf[4];
    HostXfer.StartBlock.byte.l = Iram_Cmd_block.bf[5];

    SectorNumber               = (Iram_Cmd_block.bf[5] & 0x0F);   // Sector number is needed later on to calculate offset into AUX area

    HostXfer.BlockCount.Long = 1L;                      // Set the requested host transfer length

    DVD_MESSAGE1(PRN_DVDHIF, "CSS Read Css Title Key at LBA = ", 4, HostXfer.StartBlock.lba);
    send_msg80(SEND_READ_DVD_MSG,0x710103);
    BufRetry = 3;
    while(BufRetry)
    {
        dvdInitializeHostBuffer(HostXfer.StartBlock.lba);
        DiscReadStatus = dvdLoadBuffer( HostXfer, INIT_READ );    // Read one ecc block into buffer
        if(DiscReadStatus == BUF_GOOD)
            break;
        else
            BufRetry--;
    }
    if (DiscReadStatus != BUF_GOOD)    // Read one ecc block into buffer
    // Read data should now be in Data Buffer
    {
         BUILD_SENSE(0x03,0x11,0x0B,0x01);               // UNRECOVERED READ ERROR
        dvdCss.eCssState = AUTHENTICATION_FAILED;
        AutoCommandComplete();
         return (FAIL);
    }

    /*-----------------------------------------------------------
        Read the ID for this sector in the AUX area.
    ------------------------------------------------------------*/
    DramAddressP = (BYTE *)(CUR_AUX_ADDRESS() + (SectorNumber * ONE_AUX_DVD_ENTRY_LEN) );

    ActualID.byte.u = *DramAddressP++;
    ActualID.byte.h = *DramAddressP++;
    ActualID.byte.m = *DramAddressP++;
    ActualID.byte.l = *DramAddressP;


// TBD? PORT? Title Key hardware was removed. Hardware function replaced with code.
//     Title Key information can be extracted from SDRAM memory in CPR_MAI area (located in Aux area for each sector of ECC Block)
//    Don't check title, because this function always send after Authentication Success.
//    if Authentication fail, this will detect in dvISR.c.
//    if (TitleKeyStatus())        // Check for the existence of a Title Key.
//    {
//        BUILD_SENSE(0x05,0x6F,0x01,0x02);                // COPY PROTECTION KEY EXCHANGE FAILURE - AUTHENTICATION FAILURE
//        return(FAIL);
//    }

    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);  // Point to work area in DRAM
                                       // Write the 4-byte header of the TitleKey
    *DramAddressP++ = 0;               // Allocation
    *DramAddressP++ = 0x0A;            //    Length
    *DramAddressP++ = 0;               // Reserved
    *DramAddressP++ = 0;               // Reserved

    /*-----------------------------------------------------------
        Read the ID for this sector in the AUX area.
    ------------------------------------------------------------*/
    AuxDataP = (BYTE *)( CUR_AUX_ADDRESS() + (SectorNumber * ONE_AUX_DVD_ENTRY_LEN) + TITLE_KEY_OFFSET );

    *DramAddressP++ = *AuxDataP++;    // Get Title Key from Aux area into CSS Scratch area.
    *DramAddressP++ = *AuxDataP++;
    *DramAddressP++ = *AuxDataP++;
    *DramAddressP++ = *AuxDataP++;
    *DramAddressP++ = *AuxDataP++;
    *DramAddressP++ = *AuxDataP++;

    *DramAddressP++ = 0;   // Reserved bytes.
    *DramAddressP++ = 0;   // Reserved bytes.

    WRITE_FIELD( HAL_HST_CSSENCS, 5);   // Specify CSS encrypted area start position
    WRITE_FIELD( HAL_HST_CSSENCE, 9);   // Specify CSS encrypted area end position
    WRITE_FIELD( HAL_HST_CSSSCREN, 1);  // Enable CSS Scrambling

    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(12,HostTransferLength), FALSE);   // Data transfer to host.

    WRITE_FIELD( HAL_HST_CSSSCREN, 0);   // Disable CSS Scrambling

    return(PASS);
}



/*********************************************************************
*
*   Function:   cssRegionMatch()
*
*   Description:
*
*       Returns the status of the region match between device and disc
*
*   Dependent module variables:
*
*       dvdCss.RegionMatch
*
*   Return Values:
*
*       TRUE  = The device and disc region settings match
*       FALSE = The device and disc region settings do not match
*
*********************************************************************/
BYTE cssRegionMatch(void)
{
    return dvdCss.RegionMatch;
}

/*********************************************************************
* Name:         cssGetAgid
*
* Description:  Get the AuthenticationGrantId for CSS authentication
*
* Params:       none
*
* Returns:      0 = AGID (if not granted)
*               1 = AGID granted (AGID = 0)
*
* Global outputs: fAgidInUse set to 1
*
* Notes:        AGID hard-coded as zero
*               This function merely checks if AGID is in use,
*               and if not, mark as in use (set fAgidInUse to 1)
*********************************************************************/
BYTE cssGetAgid(void)
{
    if (dvdCss.CssFlag.AgidInUse == TRUE)
    {
        return(PASS);
    }
    else
    {
        dvdCss.CssFlag.AgidInUse = TRUE;
        return(FAIL);
    }
}

/*********************************************************************
* Name:         cssComplete
*
* Description:  Return to initial state on completion of Authentication,
*               after BK used to obfuscate other keys
*
* Params:       none
*
* Returns:      none
*
* Global outputs:
*               dvdCss.eCssState = CSS_START
*               fAgidInUse       = FALSE
*
* Notes:        fAuthenticationSuccess not reset
*
*********************************************************************/
static void cssComplete(void)
{
    dvdCss.CssFlag.AgidInUse = FALSE;
    dvdCss.eCssState         = CSS_START;
}


/*********************************************************************
*
*   Function:   cssValidateRegion()
*
*   Description:
*
*       Determines if the regions between the disc and the device match
*       Test each bit of the disc and device region byte. If any corresponding
*       bit matches (both are equal to zero), then dvdCss.RegionMatch is set to
*       TRUE.
*
*   Dependent global variables:
*
*       Rma, Persistent Table
*
*   Modified module variables:
*
*       dvdCss.RegionMatch
*
*           FALSE - There is no matching region with the disc and device
*           TRUE  - A region matches between the disc and device
*
*
*********************************************************************/
void cssValidateRegion()
{
    BYTE DiscRegion, DeviceRegion, count;

    DiscRegion =  pdbGetDVDRegionControl();

    DeviceRegion = GetPersistentTable1B(RPC_REGION_OFFSET);     // Get current Region setting from FLASH


    dvdCss.RegionMatch = FALSE;

    for ( count = 0; count < 8; count++ )
    {
        if ( ((DiscRegion | DeviceRegion) & 0x01) == 0)
        {
            dvdCss.RegionMatch = TRUE;
            return;
        }

        DiscRegion >>= 1;
        DeviceRegion >>= 1;
    }
}

/*********************************************************************
*
*   Function:   TitleKeyStatus()
*
*   Description:
*
*       Will check the presence of a Title Key within any of the 16 DVD sectors
*       of the last ECC Block.
*
*   Dependent global variables:
*
*   Modified global variables:  none
*
*   Return Values:
*
*       TRUE  = Title Key is     present in one or more sectors of ECC Block.
*       FALSE = Title Key is not present in one or more sectors of ECC Block.
*
*
*
*********************************************************************/
BYTE TitleKeyStatus()
{
    BYTE  * AuxDataP;
    BYTE    Sector;
    BYTE    blkcnt;

    if(dvdDecFlag.fREADING_LEADIN == TRUE)
    {
        blkcnt = ONE_DVD_BLOCK;
    }
    else
    {
        if(HostXfer.BlockCount.Long>0)
            blkcnt = HostXfer.BlockCount.Long -1;
        else
            blkcnt = 0;
    }
//    send_msg5SValue(1,"Startaddr",4,HostXfer.StartBlock.byte.l&0x0F);
//    send_msg5SValue(1,"length",4,HostXfer.BlockCount.Long);

    AuxDataP = (BYTE *)( CUR_AUX_ADDRESS()  + ((HostXfer.StartBlock.byte.l&0x0F) * DVD_AUX_PTR_RESOLUTION));
//    send_msg54(1,*AuxDataP,*(AuxDataP+1),*(AuxDataP+2),*(AuxDataP+3));
    AuxDataP = (BYTE *)(AuxDataP +  TITLE_KEY_OFFSET);
    for (Sector = 0; Sector <= blkcnt; Sector++)
    {
//        send_msg54(1,*AuxDataP,*(AuxDataP+1),*(AuxDataP+2),*(AuxDataP+3));
        if((*AuxDataP & 0x80) !=0)
            if((*AuxDataP & 0x40) !=0)
            return (TRUE);                    // Bit CPM in this sector is 1 hence this sector contains copyrighted material,
        AuxDataP += ONE_AUX_DVD_ENTRY_LEN;    // Next sector
        if((ULONG)AuxDataP>(ULONG)DVD_AUX_END_ADDR)
            AuxDataP=(BYTE *)(DVD_ABS_AUX_START_ADDR+TITLE_KEY_OFFSET); //ring
    }
    return (FALSE);
}
