/****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   dvSndKey.c
*
* DESCRIPTION
*   This file contains Send Key command
*
*   $Revision: 28 $
*   $Date: 08/12/09 5:21p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

//#pragma   section HIF

/*********************************************************************
  INCLUDE FILES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\bf_util.h"
#include ".\hif\rd_util.h"
#include ".\DVD\dvGvars.h"
#include ".\DVD\dvCSS.h"
#include ".\DVD\dvSndKey.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvdiscrd.h"
#include ".\al\REG_dvd_decoder.h"
#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

/*********************************************************************
    FUNCTION PROTOTYPES
*********************************************************************/

static BYTE CheckRegionSetting (BYTE RegionSetting);
extern BYTE WaitForCssCompleted(void);
extern void RpcSetRegion(BYTE NewRegion);

#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
extern void SendKeyCmd_AACS(void);              // Samsung CE-Auth.
#endif

/*********************************************************************
*   SendKeyCommand
*   Send Key command handler
*
*   Returns:    void
*   Params:     none
**********************************************************************/
void SendKeyCommand(void)
{
    volatile BYTE * DramAddressP;
    WORD    HostTransferLength;
    BYTE    i,KEY_FMT,RegionCode,SentAgid,TempData;
    HAL_Reg8 *RegPtr;

    /* force Stop DVD Decoder to make sure the CSS authentication process working well*/
    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;

#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
    if(Iram_Cmd_block.bf[7] == 0x33)            /*Key Class 0x33*/
    {
        SendKeyCmd_AACS();                      // Samsung CE-Auth.
        return;
    }
#endif

#if (VCPS_TEST ==1)
    if(Iram_Cmd_block.bf[7] == VCPS_AUTHENTICATION)
    {
        if(VCPS_disc == FALSE)
            BUILD_SENSE(0x05,0x55,0x00,0x22);    // ILLEGAL REQUEST/SYSTEM RESOURCE FAILURE
        else
            VCPS_SendKey();

        AutoCommandComplete();
        return;
    }
#endif

#if (ENABLE_AACS == 1)
    if(Iram_Cmd_block.bf[7] == 0x02) /*Key Class 0x02*/
    {
        AACSSendKey();
        return;
    }
#endif

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6;   // Set AGID

    KEY_FMT = Iram_Cmd_block.bf[10] & 0x3F;       // Get KEY Format
    if ((pdbDiscLoaded() != DVD_DISC) && (KEY_FMT != 0x06))
    {
        BUILD_SENSE(0x05,0x20,0x00,0x32);        /* INVALID COMMAND OP CODE */
        AutoCommandComplete();
        return;
    }

    switch(KEY_FMT)     /*Key Format check*/
    {
        case 0x01:  /*Send Key Format 1 : Set Challenge Key*/
            if(cssValidateAgid(SentAgid))
            {
                if(dvdCss.eCssState != CSS_START)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x23);      // COMMAND SEQUENCE ERROR
                    AutoCommandComplete();
                    break;
                }
                if(HostTransferLength != 16)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0x26);     // INVALID FIELD IN CDB
                    AutoCommandComplete();
                    break;
                }


                GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                WRITE_FIELD( HAL_DVDD_RSTCSS, 1 );

                /*Set Challenge key*/
                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_CheckResponse); // Set CSSCMD1 to 1
                
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);   // Point to work area in DRAM                
                RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL9));
                for(i = 0;i< 10;i++)
                {
                    TempData = *DramAddressP++;
                    *RegPtr++ = TempData;
                }
                dvdCss.eCssState = DRIVE_CHALLENGE;
                AutoCommandComplete();
            }
            else
            {
                BUILD_SENSE(0x05,0x24,0x00,0x27);                /*INVALID FIELD IN COMMAND PACKET*/
                AutoCommandComplete();
            }
            break;
        case 0x03:/*    Send KEY Format B'000011(03)    KEY2*/
            if(cssValidateAgid(SentAgid))
            {
                if (dvdCss.eCssState!= DECCHALCMPLT)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x24);      // COMMAND SEQUENCE ERROR
                    AutoCommandComplete();
                    break;
                }
                if(HostTransferLength != 12)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0x28);     // INVALID FIELD IN CDB
                    AutoCommandComplete();
                    break;
                }

                GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                /*Set KEY2*/
                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_CheckResponse); // Set CSSCMD1 to 1
                
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH +4 );   // Point to work area in DRAM
                RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSRES4));
                for(i= 0;i<5;i++)
                {
                    TempData = *DramAddressP++;           
                    RegPtr[i] = TempData;
                }
                                
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                dvdCss.eCssState = DECRSP;

                if (WaitForCssCompleted() == FAIL)
                {                                    // Time out error
                    BUILD_SENSE(0x04,0x44,0x00,0x04);     // INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }
                cssCommon();
            }
            else
            {
                BUILD_SENSE(0x05,0x24,0x00,0x29);                /*INVALID FIELD IN COMMAND PACKET*/
                AutoCommandComplete();
            }
            break;
        case 0x06:/*    Send KEY Format B'000110(06)    RPC Structure*/
            if(HostTransferLength != 8)
            {
                BUILD_SENSE(0x05,0x24,0x00,0x2a);       // INVALID FIELD IN CDB
                AutoCommandComplete();
            }

            if (pdbDiscLoaded() == CD_DISC)
            {
                GetParameterDataFromHost(BUFFER_SCRATCH, HostTransferLength);
                RegionCode = *(BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH +4 );    // Get new Preferred Drive Region Code.
            }
            else
            {
                GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);
                RegionCode = *(BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH +4 );    // Get new Preferred Drive Region Code.
            }

            if ( CheckRegionSetting( RegionCode ) == INVALID_REGION_SETTING )
            {
                /*Error in receiving host data. Report NOT READY*/
                    BUILD_SENSE(0x02,0x3A,0x00,0x02);     // INVALID FIELD IN CDB
                    AutoCommandComplete();
                    return;
            }
            RpcSetRegion(RegionCode);
            AutoCommandComplete();
            break;
        case 0x3F:/*    Send KEY Format B'111111(3F)    Release*/
            if(cssValidateAgid(SentAgid))
            {
                cssInvalidateAgid();
            }
            AutoCommandComplete();
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0x2b);                    /*INVALID FIELD IN COMMAND PACKET*/
            AutoCommandComplete();
            break;
    }

    /* force Stop DVD Decoder to make sure the buffer control well*/
    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

}

/*********************************************************************
*
*   Function:   CheckRegionSetting
*
*   Description:
*
*       This function checks if a region setting variable is valid.
*       In order for the variable to be valid, one and only one region must
*       be set. A region is set if it's respective bit is set to 0.
*
*   Parameter
*
*       RegionSetting - Byte setting to be checked
*
*   Return value
*
*       INVALID_REGION_SETTING  - More than one region or no region is set
*       VALID_REGION_SETTING    - Only one region has been set
*
*********************************************************************/
static BYTE CheckRegionSetting( BYTE RegionSetting )
{
    BYTE i, count;

    /*-------------------------------------------------------
        Initialize variables
    --------------------------------------------------------*/
    count = 0;
    /*-------------------------------------------------------
        Test each bit of the RegionSetting. One and only only one bit
        should be set to 0. If more than one bit or no bit is set to 0,
        return INVALID_RPC_SETTING.
    --------------------------------------------------------*/
    for ( i = 0; i < 8; i++)
    {
        if((RegionSetting & 0x01) == 0)
            count++;
        RegionSetting = RegionSetting >> 1;
    }
    if(count == 1 || count == 8)
        return VALID_REGION_SETTING;
    else
        return INVALID_REGION_SETTING;
}
