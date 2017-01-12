/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003, 2002,2003,2005 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
* FILENAME:   dvRptKey.c
*
* DESCRIPTION
*   This file contains buffer related routines
*
*
*   $Revision: 37 $
*   $Date: 09/06/09 4:52p $
*
* CHANGES:
**************************** SUNEXT CONFIDENTIAL *******************************/

//#pragma   section HIF

/*********************************************************************/
/*  INCLUDE FILES */
/*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\rd_util.h"
#include ".\hif\bf_util.h"
#include ".\DVD\dvGvars.h"
#include ".\DVD\dvCSS.h"
#include ".\DVD\dvrpc.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvdiscrd.h"
#include ".\oem\oem_tbl.h"
#include ".\al\REG_dvd_decoder.h"

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

extern BYTE WaitForCssCompleted(void);

#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
extern void ReportKeyCmd_AACS(void);            // Samsung CE-Auth.
#endif

/*********************************************************************
*   ReportKeyCommand
*   Report Key command handler
*
* The REPORT KEY Command requests the start of the authentication process and provides data necessary for
* authentication and for generating a Bus Key for the DVD Logical Unit. This command, in conjunction with SEND KEY
* Command, is intended to perform authentication for Logical Units which conform to DVD content protection scheme
* and to generate a Bus Key as the result of authentication.
* The REPORT KEY Command also requests the DVD Logical Unit to transfer TITLE KEY data, obfuscated by a Bus
* Key, to the Host.
*   Returns:    void
*   Params:     none
*********************************************************************/
void ReportKeyCommand(void)
{
    RPC_NON_VOLATILE_VARS RpcSettings;
    BYTE *  DramAddressP;
    WORD    HostTransferLength;
    BYTE    i,KeyFormat,SentAgid;
    BYTE    rpcStateByte0,rpcStateByte1,rpcStateByte2;  /* RPC data variables */

    /* force Stop DVD Decoder to make sure the CSS authentication process working well*/
    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;

    // Disable Read Ahead will make the buffer is almost empty, and that will cause speed up.
    // Clear SpeedCount to avoid speeding up.
    SpeedCount = 0;

#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
    if(Iram_Cmd_block.bf[7] == 0x33)            /*Key Class 0x33*/
    {
        ReportKeyCmd_AACS();                    // Samsung CE-Auth.
        return;
    }
#endif

#if (VCPS_TEST ==1)
    if(Iram_Cmd_block.bf[7] == VCPS_AUTHENTICATION)
    {
        if(VCPS_disc == FALSE)
            BUILD_SENSE(0x05,0x55,0x00,0x1b);    // ILLEGAL REQUEST/SYSTEM RESOURCE FAILURE
        else
            VCPS_ReportKey();

        AutoCommandComplete();
        return;
    }
#endif

#if (ENABLE_AACS == 1)
    if(Iram_Cmd_block.bf[7] == 0x02) /*Key Class 0x02*/
    {
        AACSReportKey();
        return;
    }
#endif

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    /*Get Key Format*/
    KeyFormat = Iram_Cmd_block.bf[10] & 0x3F;
    if ((pdbDiscLoaded() != DVD_DISC) && (KeyFormat != 0x08))
    {
        BUILD_SENSE(0x05,0x20,0x00,0x31);        /* INVALID COMMAND OP CODE */
        AutoCommandComplete();
        return;
    }
    SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6; //  Get AGID (Authentication Grant ID) from command block

    switch(KeyFormat)
    {
        case 0: // Format 0 :Returns an AUTHENTICATION GRANT ID for Authentication for CSS/CPPM
            if(cssGetAgid()== TRUE)
            {   // AGID is granted
                dvdCss.eCssState = CSS_START,

                // Set up scratch area for parameter transfer
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                *DramAddressP++ = 0;
                *DramAddressP++ = 6;

                for (i=0;i<5;i++)
                {
                    *DramAddressP++ = 0;      // Fill reserved bytes.
                }

                *DramAddressP++ = (dvdCss.AuthenticationGrantId << 6);

                // Set up for transfer to host
                //start == CPRM fail of FWTEST2. 
                if(pdbDVDDiscCSS() != eCSS_CPS)
                {
                    BUILD_SENSE(0x05,0x6F,0x01,0x1d);           
                    dvdCss.CssFlag.AgidInUse = FALSE;
                    AutoCommandComplete();
                    break;                    
                }

                if(HostTransferLength == 0)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0x2c);    
                    dvdCss.CssFlag.AgidInUse = FALSE;
                    AutoCommandComplete();
                    break;                    
                }
                //end == CPRM fail of FWTEST2. 

                SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(8,HostTransferLength), TRUE);
            }
            else
            {   // AGID is not granted.
                BUILD_SENSE(0x05,0x6F,0x01,0x1c);           
                dvdCss.eCssState = CSS_START,
                dvdCss.CssFlag.AgidInUse = FALSE;
                AutoCommandComplete();
            }
            break;
        case 1: //  Format 1 : Returns a Challenge KEY
            if(cssValidateAgid(SentAgid))
            {
                if(dvdCss.eCssState != DRIVE_RESPONSE_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x1d);            // COMMAND SEQUENCE ERROR
                    AutoCommandComplete();
                    break;
                }

                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateChallenge); // get decoder chal
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                dvdCss.eCssState = DECCHAL;

                if(WaitForCssCompleted() == FAIL)
                {                                      // Time out error
                    BUILD_SENSE(0x04,0x44,0x00,0x01);       //  INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }
                cssCommon();
                AutoCommandComplete();
            }
            else
            {
                                                      // Invalid AGID
                BUILD_SENSE(0x05,0x24,0x00,0x22);          //   INVALID FIELD IN CDB
                AutoCommandComplete();
            }
            break;
        case 2:  // Format 2 :Returns a KEY1
            if(cssValidateAgid(SentAgid))
            {
                if(dvdCss.eCssState != DRIVE_CHALLENGE)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x1e);            // COMMAND SEQUENCE ERROR
                    AutoCommandComplete();
                    break;
                }

                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateResponse); // get drive response
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                dvdCss.eCssState = DRIVE_RESPONSE;       // Drive Response (Key1)

                if(WaitForCssCompleted() == FAIL)
                {                                      // Time out error
                    BUILD_SENSE(0x04,0x44,0x00,0x02);       //  INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }
                cssCommon();      // Return KEY1 (5 bytes) to host.
                AutoCommandComplete();
            }
            else
            {                                         // Invalid AGID
                BUILD_SENSE(0x05,0x24,0x00,0x23);          //   INVALID FIELD IN CDB
                AutoCommandComplete();
            }
            break;
        case 4:   // Format 4 : Returns a TITLE KEY obfuscated by a Bus Key
            if (cssValidateAgid(SentAgid))
            {
                /*Check for valid key exchange*/
                if (dvdCss.eCssState != AUTHENTICATION_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x1f);            // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    break;
                }

                cssValidateRegion();                        /*check Region code match*/

                #if (CEProduct == 0) /* For CEProduct, region control is doing by backend */
                /*Check for valid region*/
                if (cssRegionMatch() == FALSE )
                {
                    BUILD_SENSE(0x05,0x6F,0x04,0x20);          // MEDIA REGION CODE IS MISMATCHED TO LOGICAL UNIT REGION
                    dvdCss.CssFlag.AuthenticationSuccess = FALSE;                  /*Invalidate Authentication Flag */
                    AutoCommandComplete();
                    break;
                }
                #endif

                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateBusKey); // CSS hardware: Generate Bus Key.
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                dvdCss.eCssState = READ_TITLE_KEY;

                if(WaitForCssCompleted() == FAIL)
                {                                     // Time out error
                    BUILD_SENSE(0x04,0x44,0x00,0x03);    // INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }
                cssCommon();
            }
            else
            {
                BUILD_SENSE(0x05,0x24,0x00,0x24);          //  INVALID FIELD IN CDB
                AutoCommandComplete();
            }
            break;
        case 5:  // Format 5: Returns the current state of the ASF (Authentication Success Flags) for CSS/CPPM
            DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = 0;                                 /*allocation length Hi-byte*/
            *DramAddressP++ = 6;                                 /*allocation length Lo-byte*/
            for (i=0;i<5;i++)
                *DramAddressP++ = 0;

            *DramAddressP++ = dvdCss.CssFlag.AuthenticationSuccess;

            /* set up for transfer to host */
            SendParameterDataToHost(DVD_CSS_SCRATCH,MINIMUM(8,HostTransferLength), TRUE);
            break;
        case 8: // Format 8: Report drive region settings
            RpcSettings = RpcGetRegion();    // Get RPC settings from FLASH or Serial EEPROM
    /*****************************
    *       Get RPC Type Code
    *       Byte 0 : bits 6 & 7
    *****************************/
            rpcStateByte0 = RpcSettings.RPC.State << 6;

    /********************************************************
    *       Get number of user controlled changes available
    *       Byte 0 : bits 3 - 5
    ********************************************************/
            rpcStateByte0 |= (RpcSettings.RPC.VendorResetsAvailable & 0x07) << 3;

    /********************************************************
    *       Get number of user controlled changes available
    *       Byte 0 : bits 0 - 2
    ********************************************************/
            rpcStateByte0 |= (RpcSettings.RPC.nCount & 0x07);

    /*********************************
    *       Get region mask. Byte 1
    *********************************/
            rpcStateByte1 = RpcSettings.RPC.LuRegion;

    /********************************************
    *       Determine RPC functionality. Byte 2
    *******************************************/
#define RPC_SCHEME_UNKNOWN      0x00
#define RPC_SCHEME_PHASE_II     0x01
            rpcStateByte2 = RPC_SCHEME_PHASE_II;

            /***********************************
            *Set DRAM pointer to scratch area
            **********************************/
            if (pdbDiscLoaded() == CD_DISC)
            	DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);
            else
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

            /*********************************
            *Write RPC data to scratch area
            ********************************/
            *DramAddressP++ = 0;
            *DramAddressP++ = 6;
            *DramAddressP++ = 0;
            *DramAddressP++ = 0;
            *DramAddressP++ = rpcStateByte0;
            *DramAddressP++ = rpcStateByte1;
            *DramAddressP++ = rpcStateByte2;
            *DramAddressP++ = 0;

            if (pdbDiscLoaded() == CD_DISC)
            	SendParameterDataToHost(BUFFER_SCRATCH ,MINIMUM(8,HostTransferLength) , TRUE);
            else
            	SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(8,HostTransferLength) , TRUE);

            break;
#if (DVD_RAM_READ == 1)
        case 0x09:  // Format 9:  Returns an AGID (AUTHENTICATION GRANT ID) for Authentication for CPRM
            if(cssGetAgid())
            {
                dvdCss.eCssState = CSS_START,

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work / scratch area in DRAM for parameter transfer.
                *DramAddressP++ = 0;
                *DramAddressP++ = 6;

                for (i=0;i<5;i++)
                {
                    *DramAddressP++ = 0;
                }

                *DramAddressP++ = (dvdCss.AuthenticationGrantId << 6);

                /* set up for transfer to host */
                SendParameterDataToHost(DVD_CSS_SCRATCH,MINIMUM(8,HostTransferLength), TRUE);  // and send Command Complete
            }
            else
            {
                                                    // AGID already in use
                BUILD_SENSE(0x05,0x2C,0x00,0x21);        //  COMMAND SEQUENCE ERROR
                dvdCss.eCssState = CSS_START,
                dvdCss.CssFlag.AgidInUse = FALSE;
                AutoCommandComplete();
            }
            break;
#endif
#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
        case 0x11: // Format 0x11 :Returns an AUTHENTICATION GRANT ID for Authentication for CPRM
            if(cssGetAgid()== TRUE)
            {   // AGID is granted
                dvdCss.eCssState = CSS_START,

                // Set up scratch area for parameter transfer
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                *DramAddressP++ = 0;
                *DramAddressP++ = 6;

                for (i=0;i<5;i++)
                {
                    *DramAddressP++ = 0;      // Fill reserved bytes.
                }

                *DramAddressP++ = (dvdCss.AuthenticationGrantId << 6);

                // Set up for transfer to host
                SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(8,HostTransferLength), TRUE);
            }
            else
            {   // AGID is not granted.
                BUILD_SENSE(0x05,0x2C,0x00,0x1f);            // AGID is already in use
                dvdCss.eCssState = CSS_START,
                dvdCss.CssFlag.AgidInUse = FALSE;
                AutoCommandComplete();
            }
            break;
#endif  /* ENABLE_CPRM == 1 && BCA_ENABLE == 1 */
        case 0x3F: // Format 0x3F: Invalidate Specified AGID.
                                // Invalidating an invalid AGID shall not be considered an error.
                                // An AGID that has not been granted shall be considered invalid.
            //***************************************
            // Service this command only if AGID valid
            // AGID invalid not considered as error
            // but command is not serviced
            //***************************************
            if (cssValidateAgid(SentAgid))
            {
                cssInvalidateAgid();    // FAIL
            }
            AutoCommandComplete();
            break;
        default:                                      // Invalid format
            BUILD_SENSE(0x05,0x24,0x00,0x25);              //  INVALID FIELD IN CDB                /* invalid format 05/24/00  */
            AutoCommandComplete();
            break;
    }

    /* force Stop DVD Decoder to make sure the buffer control well*/
    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;
}
