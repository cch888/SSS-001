/****************************************************************************
*               (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   dvRdStru.c
*
* DESCRIPTION
*   This file contains Read DVD Structure command
*
*   $Revision: 151 $
*   $Date: 10/12/21 4:17p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
  INCLUDE FILES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\com_im.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_inqry.h"
#include ".\hif\gp_util.h"
#include ".\hif\rd_util.h"
#include ".\dvd\dvgvars.h"
#include ".\dvd\dvcss.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvdiscinit.h"
#include ".\dvd\dvbufmgr.h"
#include ".\hif\Rd_Stru.h"
#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#endif

#include ".\oem\oem_tbl.h"
#include ".\hif\Rd_Stru.h"
#include ".\dvd\dvRdStru.h"
#include ".\al\reg_hal.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\al\REG_dfe.h"
#include ".\al\HAL_afe.h"
#include ".\al\HAL_dfe.h"
#if (ENABLE_HDDVD == 1)
#include ".\hd\hdDB.h"
#endif
#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

#if(BCA_ENABLE == 1)
#include ".\dvd\dvDiscRd.h"
#include ".\al\REG_global.h"
#include ".\AL\HAL_global.h"
#include ".\Player\PlrWatch.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_step.h"
#include ".\servo\svo_speed.h"
BYTE gBcaInfoAvailable;

#define DVDBCATarget  UM_2_SLED_STEP(22900) //BCA area from 22.3mm to 23.5mm
#define DVDNBCATarget UM_2_SLED_STEP(23000) //BCA area from 22.7mm to 23.5mm
#define MAX_BCA_BYTES 188
#define MAX_BCA_RETRY_TIMES 4
#define BCA_FREQ_INIT       115
#define BCA_FREQ_HIGH       130
#define BCA_FREQ_LOW        100
#endif

extern const STRU_FMT_LIST_T StruFmtList[];


/*********************************************************************
* Cmd_read_dvd_structure
*
* Read DVD Structure command handler
*
* Returns:      void
* Params:       none
*********************************************************************/
void ReadDVDStructureCommand(void)
{
    tPhysicalFormatInformationS *DataPtr;
    DiscAccessStatusE            DiscReadStatus;
    STRU_FMT_LIST_T    const    *base;


#if (DVD_RAM_READ == 1)
    USHORT      TempW;
#endif

#if (ENABLE_AACS)
    StLongU Address;
#endif
#if (ENABLE_CPRM == 1) && (BCA_ENABLE == 1)
    HAL_Reg8   *RegPtr;
#endif
#if (BCA_ENABLE == 1)
    BYTE BCARetryTimes;
#endif
    BYTE       *DramAddressP;
    BYTE       *DramTempP;
    WORD        HostTransferLength;
    BYTE        BufferData;
    BYTE        SentAgid;
    BYTE        FormatField;
    BYTE        SectorNumber;

#if (EN_POWER_CONTROL == 1)
    if (CurrentPowerMode == eStandbyPower)
    {
        SetDiscReadPowerMode();
    }
#endif

    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    ClearDataBuffer();

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    /*Get Format*/
    FormatField = Iram_Cmd_block.bf[7] & 0xFF;

    /*Get AGID*/
    SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6;

    switch(FormatField)
    {
        case DVD_DISC_KEY:
        case DVDHD_DISC_MANUF_INFO:
        case DVD_COPYRIGHT_MANAGE:
        case DVDHDRAM_MEDIA_REC_TYPE:
        case DVDHD_LAST_BO_RMD:
        case DVD_LAST_RMA:
            break;
        default:
            //ServoPause();
            break;
    }

    if (Iram_Cmd_block.bf[6] > pdbGetDVDLayers())
    {
        BUILD_SENSE(0x05,0x24,0x00,0x0a);        // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }

    if ((Iram_Cmd_block.bf[7] == 0x10 ) && (pdbServoGetDiscType() != eLPPDisc))
    {
        BUILD_SENSE(0x05,0x24,0x00,0x0b);        // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }

    //start== HP CMPLY test TDR840 fail.
    if (HostTransferLength == 0)
    {
        AutoCommandComplete();      // An Allocation Length of 0 is not considered an error.
        return;                     //  Just complete command and return.
    }
    //end==

    switch(FormatField)
    {
        case DVDHD_PHY_FMT_INFO:
            //-------------------------------------------------
            // Transfer DVD/HD-DVD Physical Format Information to Host
            //-------------------------------------------------

            if ((St_blank_disc)&&(pdbServoGetDiscType()==eADIPDisc)&&(Info.Media.DVD.fPFInfoValid==FALSE))  //No Lead-in data.
            {
                BUILD_SENSE(0x05,0x24,0x00,0x0c);                   // INVALID FIELD IN CDB
                AutoCommandComplete();
                return;
            }

            //start== HP CMPLY test TDR840 fail.
            if (HostTransferLength < 4)
            {
                AutoCommandComplete();      // An Allocation Length of 0 is not considered an error.
                return;                     //  Just complete command and return.
            }
            //end==

            /* Insure that maximum transfer length is not exceeded */
            HostTransferLength = MINIMUM(2052,HostTransferLength);

            /* Do not include the data transfer length information in transfer length (4 bytes) */
            HostTransferLength -= 2;

            switch(pdbDiscLoaded())
            {
                case DVD_DISC:
                    if(pdbServoGetDiscType() == eLPPDisc)
                    {
                        CopyBufferToBuffer(DVD_PHYSICAL_FORMAT_INFORMATION_PAGE, 0, 0, 0, 1);
                        DataPtr = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS);

                        if (St_blank_disc == TRUE)
                        {
                           CopyBufferToBuffer(DVD_LOGICAL_FORMAT_INFORMATION_PAGE, 0, 0, 0, 1);
                           DramTempP     = (BYTE *) (SCSDRAM_ADDRESS);
                           DramTempP    += 0x20;
                           *DramTempP++  = 0x00;      // Byte 32
                           *DramTempP++  = 0x00;      // Byte 33
                           *DramTempP++  = 0x00;      // Byte 34
                           *DramTempP++  = 0x00;      // Byte 35

                           *DramTempP++  = 0x00;      // Byte 36
                           *DramTempP++  = 0x00;      // Byte 37
                           *DramTempP++  = 0x00;      // Byte 38
                           *DramTempP++  = 0x00;      // Byte 39
                        }
                    }
                    else
                    {
                        if(pdbGetDiscKind() == eDVDPlusRWDisc || pdbGetDiscKind() == eDVDPlusRWDLDisc)
                            CopyBufferToBuffer(DVD_PHYSICAL_FORMAT_INFORMATION_PAGE, 0, 0, 0, 1);
                        else
                            CopyBufferToBuffer(DVD_LOGICAL_FORMAT_INFORMATION_PAGE, 0, 0, 0, 1);

                        // Point to the newly copied physical format information in SDRAM Read Buffer
                        DataPtr = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS);

                        #if (CEProduct == 1) && (MTK_BE == 1)     //for MTK backend
                        if(pdbGetDVDType() == DVD_PLUS_RW)
                        {
                            BYTE * pPtr;
                            pPtr = (BYTE *) DataPtr;
                            #if ((MSG_OUT_MODE==MSG_USBPORT)&&(DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE))
                            send_uart1(1,'*','R',*(pPtr+9),*(pPtr+10),*(pPtr+11));
                            #endif
                            send_msg5SValue(SEND_DVDPLUSRW_MSG, "OrgMaxPSN", 4, SWAPEND_ULONG(DataPtr->MaxPSN));
                            DataPtr->MaxPSN = SWAPEND_ULONG(DVD_PLUSRW_DATAZONE_END);
                        }
                        #endif

                        // For PTP dual layer disc only. Set the max PSN if request is for layer 0
                        if ( (pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC) )
                        {
                            if (Iram_Cmd_block.bf[6] == 0)
                                DataPtr->MaxPSN = SWAPEND_ULONG(pdbGetDVDMaxPSN(0));
                            else
                                DataPtr->MaxPSN = SWAPEND_ULONG(pdbGetDVDMaxPSN(1));
                        }

                        #if (MSG_OUT_MODE == MSG_USBPORT)
                        {
                            BYTE * pInfo;
                            BYTE idx;
                            pInfo = (BYTE *) (SCSDRAM_ADDRESS);
                            for (idx = 0; idx < 24; idx += 4)
                                send_msg54(SEND_READ_DVD_MSG, pInfo[idx+0], pInfo[idx+1], pInfo[idx+2], pInfo[idx+3]);
                        }
                        #endif
                    }
                    break;
#if(HD_READ)
                case HD_DISC:

                        CopyBufferToBuffer(HDDVD_LOGICAL_FORMAT_INFORMATION_PAGE, 0, 0, 0, 1);

                        // Point to the newly copied physical format information in SDRAM Read Buffer
                        DataPtr = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS);

                        // For PTP dual layer disc only. Set the max PSN if request is for layer 0
                        if ( (pdbGetHDLayers() == DUAL_LAYER) && (pdbGetHDTrackPath() == DVDHD_PTP_DISC) )
                        {
                            if (Iram_Cmd_block.bf[6] == 0)
                                DataPtr->MaxPSN = SWAPEND_ULONG(pdbGetHDMaxPSN(0));
                            else
                                DataPtr->MaxPSN = SWAPEND_ULONG(pdbGetHDMaxPSN(1));
                        }
                    break;
#endif
            }

            //  Create Header data in scratch area
            DramAddressP    = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = BHIGH(HostTransferLength);
            *DramAddressP++ = BLOW(HostTransferLength);
            *DramAddressP++ = 0;
            *DramAddressP++ = 0;

            // Send Header data to host
            SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);

            // Transfer data and send Command Complete.

            SendParameterDataToHost(((ULONG) DataPtr), HostTransferLength - 2 , TRUE);

            break;

        case DVD_COPYRIGHT_INFO:
            //-------------------------------------------------
            // Transfer DVD Copyright information to host
            //-------------------------------------------------

            if (St_blank_disc)// Avoid the WIN98 misjudge the blank DVD as audio disc
            {
                BUILD_SENSE(0x05,0x30,0x02,0x16);                   // CANNOT READ MEDIUM - INCOMPATIBLE FORMAT
                AutoCommandComplete();
                return;
            }

            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
            // Prepare Header data for host.
            *DramAddressP++ = 0;    // DVD Structure
            *DramAddressP++ = 6;    // Data Length.
            *DramAddressP++ = 0;    // Reserved.
            *DramAddressP++ = 0;    // Reserved.

            // Prepare actual structure data for host.
            *DramAddressP++ = pdbDVDDiscCSS();              // Copy Protection System Type.
            *DramAddressP++ = pdbGetDVDRegionControl();     // Region Management Information RPC(Region Playback Control)
            *DramAddressP++ = 0;                            // Reserved.
            *DramAddressP++ = 0;                            // Reserved.

            SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(8,HostTransferLength), TRUE);    // Transfer data and send Command Complete.
            break;

        case DVD_DISC_KEY:
            //-------------------------------------------------
            //    Transfer Disc Key to host.
            //-------------------------------------------------

            if(cssValidateAgid(SentAgid))
            {
                if (dvdCss.eCssState!= AUTHENTICATION_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x17);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    return;
                }

                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateBusKey); // CSS hardware: Generate Bus Key.
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                if(WaitForCssCompleted() == FAIL)
                {                                              // CSS Hardware Time out.
                    BUILD_SENSE(0x04,0x44,0x00,0x07);               // INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }

                dvdCss.eCssState = SEND_DISC_KEY_STRUCTURES_TO_HOST;
                cssCommon();
            }
            else
            {                                                  // Invalid AGID (Authentication Grant ID)
                BUILD_SENSE(0x05,0x24,0x00,0x0c);                   // INVALID FIELD IN CDB
            }
            AutoCommandComplete();
            break;

        case DVDHD_BCA_INFO:
            //-------------------------------------------------
            //  Transfer BCA Data to host.
            //-------------------------------------------------

            switch(pdbDiscLoaded())
            {
                case DVD_DISC:

                    if (pdbDVDDiscBCA() == FALSE)  // Check if the BCA field is available - this is from the lead-in information
                    {   // BCA does not exist
                        BUILD_SENSE(0x05,0x24,0x00,0x0d);                   // INVALID FIELD IN CDB
                        AutoCommandComplete();
                        return;
                    }

                    // READ BCA
                    dvdErrorFlag.fBCA_ERROR = FALSE;

#if (BCA_ENABLE == 1)
                    ////gBcaInfoAvailable = FALSE;
                    if (gBcaInfoAvailable == FALSE)
                    {
                        //ServoStartUp();
                        BCARetryTimes = 0;
                        while(BCARetryTimes<ATAPI_BCA_RETRY_TIMES){
                            if(DVDReadBCA() == FAIL)
                            {
                                BCARetryTimes++;
                            }else{
                                break;
                            }
                        }


                        if(BCARetryTimes >= ATAPI_BCA_RETRY_TIMES){
                            BUILD_SENSE(0x03,0x02,0x00,0x0a);                   // NO SEEK COMPLETE
                            AutoCommandComplete();
                            return;
                        }
//                    if(DVDReadBCA() == FAIL)
//                    {
//                        BUILD_SENSE(0x03,0x02,0x00,0x0a);                   // NO SEEK COMPLETE
//                        AutoCommandComplete();
//                        return;
//                    }

                    }
#endif
                    /* Insure that maximum transfer length is not exceeded */
                    HostTransferLength = MINIMUM(188,HostTransferLength);
                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                    // Set up the BCA transfer length = BCA data length + 2 reserved bytes.
                    HostTransferLength = MINIMUM(HostTransferLength, dvdDiscRequest.BcaLen);
                    if (HostTransferLength >=4)
                        HostTransferLength = HostTransferLength -4; /* the header length */

                    *DramAddressP++ = (GET_BHIGH(HostTransferLength));     // Prepare Header
                    *DramAddressP++ = (GET_BLOW(HostTransferLength));
                    *DramAddressP++ = 0;
                    *DramAddressP++ = 0;

                    // First send Header to host.
                    SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);         // Transfer data do not send Command Complete yet.

                    // Now transfer the BCA data to host.
                    SendParameterDataToHost(DVD_BCA, HostTransferLength, TRUE);    // Transfer data and send Command Complete.
                    break;
#if(HD_READ)
                case HD_DISC:
                #if (BCA_ENABLE == 1)
                    if( pdbGetDiscKind()==eHDROMDisc )
                    {
                        if (gBcaInfoAvailable == FALSE)
                        {
                            BCARetryTimes = 0;
                            while(BCARetryTimes<ATAPI_BCA_RETRY_TIMES)
                            {
                                if(DVDReadBCA() == FAIL)
                                {
                                    BCARetryTimes++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if(BCARetryTimes >= ATAPI_BCA_RETRY_TIMES)
                            {
                                BUILD_SENSE(0x03,0x02,0x00,0x0a);                   // NO SEEK COMPLETE
                                AutoCommandComplete();
                                return;
                            }

                            //if(DVDReadBCA() == FAIL)
                            //{
                            //    send_msg5S(SEND_READ_DVD_MSG,"Read BCA Fail");
                            //}
                            //else
                            //{
                            //    send_msg5S(SEND_READ_DVD_MSG,"Read BCA OK");
                            //}
                        }
                    }
                #endif
                    break;
#endif
            }
            break;

        case DVDHD_DISC_MANUF_INFO:
            //-------------------------------------------------
            // Disc Manufacturing Information
            //-------------------------------------------------

            /* Insure that maximum transfer length is not exceeded */
            HostTransferLength = MINIMUM(2052,HostTransferLength);

            HostTransferLength -= 2;

            switch(pdbDiscLoaded())
            {
                case DVD_DISC:
                    // Read Lead-in control block for specified layer
                    if(pdbGetDVDTrackPath() == DVDHD_OTP_DISC)
                        DiscReadStatus = dvdBufferControlDataBlock(0, START_ADDRESS_OF_CONTROL_DATA);
                    else
                        DiscReadStatus = dvdBufferControlDataBlock(Iram_Cmd_block.bf[6], START_ADDRESS_OF_CONTROL_DATA);

                    if (DiscReadStatus != BUF_GOOD)
                    {
                        BUILD_SENSE(0x04,0x44,0x00,0x08);    // INTERNAL TARGET FAILURE
                        AutoCommandComplete();
                        return;
                    }

                    // Point to work area in DRAM
                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);

                    // Prepare Header for host
                    *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                    *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                    *DramAddressP++ = 0;                              // Reserved
                    *DramAddressP++ = 0;                              // Reserved

                    // Send header to host.
                    SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);

                    // Get the absolute address of the control data block and set to sector 1
                    DramAddressP = (BYTE *)(CUR_ECC_ADDRESS() + DVD_SECTOR_SIZE);

                    // Transfer Disc Manufacturing Information to host.
                    SendParameterDataToHost((ULONG)DramAddressP, HostTransferLength - 2, TRUE);    // Transfer data and send Command Complete.
                    break;
#if(HD_READ)
                case HD_DISC:
                    // Read Lead-in control block for specified layer
                    if(pdbGetHDTrackPath() == DVDHD_OTP_DISC)
                        DiscReadStatus = HDdvdBufferControlDataBlock(0, HD_CONTROL_DATA_1_START_ADDRESS);
                    else
                        DiscReadStatus = HDdvdBufferControlDataBlock(Iram_Cmd_block.bf[6], HD_CONTROL_DATA_1_START_ADDRESS);

                    if (DiscReadStatus != BUF_GOOD)
                    {
                        BUILD_SENSE(0x04,0x44,0x00,0x08);    // INTERNAL TARGET FAILURE
                        AutoCommandComplete();
                        return;
                    }

                    // Point to work area in DRAM
                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);

                    // Prepare Header for host
                    *DramAddressP++ = BHIGH(HostTransferLength);      // HD Structure
                    *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                    *DramAddressP++ = 0;                              // Reserved
                    *DramAddressP++ = 0;                              // Reserved

                    // Send header to host.
                    SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);

                    // Get the absolute address of the control data block and set to sector 1
                    DramAddressP = (BYTE *)HDDVD_DISC_MANUFACTURE_INFORMATION;

                    // Transfer Disc Manufacturing Information to host.
                    SendParameterDataToHost((ULONG)DramAddressP, HostTransferLength - 2, TRUE);    // Transfer data and send Command Complete.
                    break;
#endif
            }
            break;

        case DVD_COPYRIGHT_MANAGE:
            //-------------------------------------------------
            // Copyright Management Information
            //-------------------------------------------------

            // We need to read the sector specified in the "Address" part. (Byte 2...5) of CDB
            // Then we need to extract CPR_MAI information for this sector from the "Aux Area"
            // and transfer this information with its header to the host.
            // Now we need to read an ecc block of data from the requested LBA
            HostXfer.StartBlock.byte.u = Iram_Cmd_block.bf[2];   // Get the host request starting LBA from the command block
            HostXfer.StartBlock.byte.h = Iram_Cmd_block.bf[3];
            HostXfer.StartBlock.byte.m = Iram_Cmd_block.bf[4];
            HostXfer.StartBlock.byte.l = Iram_Cmd_block.bf[5];

            SectorNumber               = (Iram_Cmd_block.bf[5] & 0x0F);   // Sector number is needed later on to calculate offset into AUX area

            HostXfer.BlockCount.Long = 1L;                      // Set the requested host transfer length
            if (dvdValidReadRequest() == FALSE)
            {
                BUILD_SENSE(0x05,0x21,0x00,0x07);
                AutoCommandComplete();
                return;
            }

            DiscReadStatus = dvdLoadBuffer( HostXfer, INIT_READ );    // Read one ecc block into buffer
            if (DiscReadStatus != BUF_GOOD)    // Read one ecc block into buffer
            // Read data should now be in Data Buffer
            {
                 BUILD_SENSE(0x03,0x11,0x0B,0x0d);               // UNRECOVERED READ ERROR
                 AutoCommandComplete();
                 return;
            }

            // Now we get CPR_MAI information for this sector from the "Aux Area". There are 6 bytes.
            DramAddressP = (BYTE *)(CUR_AUX_ADDRESS() + ((DVD_AUX_PTR_RESOLUTION * SectorNumber) + 6));
            BufferData = *DramAddressP;     // Read first byte

            if((BufferData & MASK_CPM) == 0)
            {
                BufferData = 0x00;
            }
            else
            {
                if((BufferData & MASK_CP_SEC) == 0)
                    BufferData &= ~MASK_CP_MOD;           /*CP_MOD clear*/
            }

            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

            // Prepare Header for host
            *DramAddressP++ = 0;         // DVD Structure
            *DramAddressP++ = 6;         // Data Length
            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;         // Reserved

            *DramAddressP++ = BufferData;      // Copyright Management Information
            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;         // Reserved
            *DramAddressP++ = 0;

            SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(8,HostTransferLength), TRUE);    // Transfer data and send Command Complete.
            break;

#if (ENABLE_CPRM == 1) && (BCA_ENABLE == 1)
        case DVD_MEDIA_ID:
            //-------------------------------------------------
            //  Media Identifer
            //-------------------------------------------------

            if (pdbDVDDiscCSS() != eCPRM_CPS)
            {
                dvdCss.CssFlag.AgidInUse = FALSE;
                dvdCss.eCssState = ILLEGAL_CSS_STATE;
                BUILD_SENSE(0x05,0x6F,0x01,0x03);    // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT
                AutoCommandComplete();
                return;
            }

            if(cssValidateAgid(SentAgid))
            {
                BYTE i;
                if (dvdCss.eCssState != AUTHENTICATION_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x18);  // COPY PROTECTION KEY EXCHANGE FAILURE: KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    return;
                }

                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateBusKey); // CSS hardware: Generate Bus Key.
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                if(WaitForCssCompleted() == FAIL)
                {                                              // CSS Hardware Time out.
                    BUILD_SENSE(0x04,0x44,0x00,0x0e);               // INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }

                if (gBcaInfoAvailable == FALSE)
                {
                    BCARetryTimes = 0;
                    while(BCARetryTimes<ATAPI_BCA_RETRY_TIMES)
                    {
                        if(DVDReadBCA() == FAIL)
                        {
                            BCARetryTimes++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(BCARetryTimes >= ATAPI_BCA_RETRY_TIMES)
                    {
                        BUILD_SENSE(0x03,0x02,0x00,0x0a);                   // NO SEEK COMPLETE
                        AutoCommandComplete();
                        return;
                    }

                    //if(DVDReadBCA() == FAIL)
                    //{
                    //    BUILD_SENSE(0x03,0x02,0x00,0x0a);           // NO SEEK COMPLETE
                    //    AutoCommandComplete();
                    //    return;
                    //}
                }

                RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL7));
                DramAddressP = (BYTE *)(SCSDRAM_START_ADDR + DVD_BCA);
                /* write media ID */
                for (i = 0; i < 8; i++)
                    RegPtr[i] = DramAddressP[4+i];                   //Media ID is at SCSDRAM[4~11]

                WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateCPRMMAC); // CSS hardware: Generate CPRM MAC.
                WRITE_FIELD( HAL_DVDD_CSSRUN , 1);          //  Start execution of CSS Authentication command.

                if(WaitForCssCompleted() == FAIL)
                {                                           // CSS Hardware Time out.
                    BUILD_SENSE(0x04,0x44,0x00,0x09);       // INTERNAL TARGET FAILURE
                    AutoCommandComplete();
                    return;
                }

                dvdCss.eCssState = GENERATE_BUS_KEY_FOR_MEDIA_IDENTIFIER;
                cssCommon();  // Assemble and transfer Media Identifier to host and complete command.
            }
            else
            {                                               // Invalid AGID (Authentication Grant ID)
                BUILD_SENSE(0x05,0x24,0x00,0x0e);           // INVALID FIELD IN CDB
            }
            AutoCommandComplete();
            break;

        case DVD_MEDIA_KEY_BLOCK:
            //-------------------------------------------------
            //  Media Key Block
            //-------------------------------------------------

            if (pdbDVDDiscCSS() != eCPRM_CPS)
            {
                dvdCss.CssFlag.AgidInUse = FALSE;
                dvdCss.eCssState = ILLEGAL_CSS_STATE;
                BUILD_SENSE(0x05,0x6F,0x01,0x04);    // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT
                AutoCommandComplete();
                return;
            }

            if(cssValidateAgid(SentAgid))
            {
                StLongU MKBNo;
                BYTE i;

                if (dvdCss.eCssState != AUTHENTICATION_COMPLETE)
                {
                    // COPY PROTECTION KEY EXCHANGE FAILURE: KEY NOT ESTABLISHED
                    BUILD_SENSE(0x05,0x6F,0x02,0x19);       // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    return;
                }
                else
                {
                    // Get the requested MKB pack#
                    MKBNo.byte.u = Iram_Cmd_block.bf[2];
                    MKBNo.byte.h = Iram_Cmd_block.bf[3];
                    MKBNo.byte.m = Iram_Cmd_block.bf[4];
                    MKBNo.byte.l = Iram_Cmd_block.bf[5];

                    if (MKBNo.Long == 0x00)
                    {
                        WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateBusKey); // CSS hardware: Generate Bus Key.
                        WRITE_FIELD( HAL_DVDD_CSSRUN , 1);            //  Start execution of CSS Authentication command.

                        if(WaitForCssCompleted() == FAIL)
                        {                                              // CSS Hardware Time out.
                            BUILD_SENSE(0x04,0x44,0x00,0x0f);               // INTERNAL TARGET FAILURE
                            AutoCommandComplete();
                            return;
                        }

                        if (pdbGetDVDType() == DVD_MINUS_R || pdbGetDVDType() == DVD_MINUS_RW)
                        {
                            if (gBcaInfoAvailable == FALSE)
                            {
                                BCARetryTimes = 0;
                                while(BCARetryTimes<ATAPI_BCA_RETRY_TIMES)
                                {
                                    if(DVDReadBCA() == FAIL)
                                    {
                                        BCARetryTimes++;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                if(BCARetryTimes >= ATAPI_BCA_RETRY_TIMES)
                                {
                                    BUILD_SENSE(0x03,0x02,0x00,0x0a);                   // NO SEEK COMPLETE
                                    AutoCommandComplete();
                                    return;
                                }

                                //if(DVDReadBCA() == FAIL)
                                //{
                                //    BUILD_SENSE(0x03,0x02,0x00,0x0a);           // NO SEEK COMPLETE
                                //    AutoCommandComplete();
                                //    return;
                                //}
                            }

                            RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL7));
                            DramAddressP = (BYTE *)(SCSDRAM_START_ADDR + DVD_BCA);
                            /* write MKB hash */
                            for (i = 0; i < 8; i++)
                                RegPtr[i] = DramAddressP[16+i];                   //MKB Hash is at SCSDRAM[16~23]
                        }

                        dvdDiscRequest.TargetID.Long = START_ADDRESS_OF_CONTROL_DATA;

                        do
                        {
                            dvdDiscRequest.Type            = INIT_READ;
                            dvdDiscRequest.TargetLayer     = eLayer0;
                            dvdDiscRequest.EccBlockCount   = 1L;
                            dvdDecFlag.fREADING_LEADIN= FALSE;

                            DiscReadStatus = dvdStartDiscTransfer();

                            if (DiscReadStatus != BUF_GOOD)
                            {
                                if((dvdDiscRequest.TargetID.Long + 16*ONE_DVD_BLOCK) < DVD_BUFFER_ZONE_2_ADDR)
                                {
                                    dvdDiscRequest.TargetID.Long  += 16*ONE_DVD_BLOCK;
                                    ClearDataBuffer();          // Clear the DVD buffer
                                }
                                else
                                {
                                    BUILD_SENSE(0x05,0x6F,0x02,0x01);   // INTERNAL TARGET FAILURE
                                    AutoCommandComplete();
                                    return;
                                }
                            }
                        } while (DiscReadStatus != BUF_GOOD);

    #if (DVD_RAM_READ == 1)
                        if (pdbGetDVDType() == DVD_RAM)
                        {
                            /* write MKB hash */

                            RegPtr = (HAL_Reg8 *)(&(HAL_DVDDREG_MAP->CSSCHAL7));
                            /* write media ID */
                            for (i = 0; i < 8; i++)
                                RegPtr[i] = SCSDRAM[i];                 //MKB Hash is at SCSDRAM[0~7]
                        }
    #endif

                        WRITE_FIELD( HAL_DVDD_CSSCMD, halDVDD_GenerateCPRMMAC); // CSS hardware: Generate CPRM MAC.
                        WRITE_FIELD( HAL_DVDD_CSSRUN , 1);      //  Start execution of CSS Authentication command.

                        if (WaitForCssCompleted() == FAIL)
                        {                                       // CSS Hardware Time out.
                            BUILD_SENSE(0x04,0x44,0x00,0x0a);   // INTERNAL TARGET FAILURE
                            AutoCommandComplete();
                            return;
                        }

                        dvdCss.eCssState = GENBKFORMK;
                        cssCommon();   // Read Media Block Key and send it to host and complete command.
                    }
                    else
                    {
                        dvdDiscRequest.TargetID.Long = START_ADDRESS_OF_CONTROL_DATA+MKBNo.Long*ONE_DVD_BLOCK;

                        do
                        {
                            dvdDiscRequest.Type            = INIT_READ;
                            dvdDiscRequest.TargetLayer     = eLayer0;
                            dvdDiscRequest.EccBlockCount   = 1L;
                            dvdDecFlag.fREADING_LEADIN= FALSE;

                            DiscReadStatus = dvdStartDiscTransfer();

                            if (DiscReadStatus != BUF_GOOD)
                            {
                                if((dvdDiscRequest.TargetID.Long + 16*ONE_DVD_BLOCK) < DVD_BUFFER_ZONE_2_ADDR)
                                {
                                    dvdDiscRequest.TargetID.Long  += 16*ONE_DVD_BLOCK;
                                    ClearDataBuffer();          // Clear the DVD buffer
                                }
                                else
                                {
                                    BUILD_SENSE(0x05,0x6F,0x02,0x02);   // INTERNAL TARGET FAILURE
                                    AutoCommandComplete();
                                    return;
                                }
                            }
                        } while (DiscReadStatus != BUF_GOOD);

                        DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                        // Prepare Header
                        *DramAddressP++ = 0x60;                         // Allocation
                        *DramAddressP++ = 0x02;                         // Length
                        *DramAddressP++ = 0;                            // Reserved
                        *DramAddressP++ = Info.Media.DVD.TotalMKBPacks; // Total MKB Packs Used

                        // Data transfer to host
                        SendParameterDataToHost(DVD_CSS_SCRATCH, 0x04, FALSE);  // No Command Complete

                        for(i = 4; i <= 15; i++)
                            SendParameterDataToHost(i*0x800, 0x800, FALSE);     // No Command Complete
                    }
                }
            }
            else
            {                                           // Invalid AGID (Authentication Grant ID)
                BUILD_SENSE(0x05,0x24,0x00,0x0f);       // INVALID FIELD IN CDB
            }
            AutoCommandComplete();
            break;
#endif  // #if (ENABLE_CPRM == 1) && (BCA_ENABLE == 1)

#if (DVD_RAM_READ == 1)
        case DVDHDRAM_DDS:  /*DDS*/
            if (pdbGetDVDType() == DVD_RAM)
            {
                HostTransferLength = MINIMUM(2052,HostTransferLength);
                HostTransferLength -= 2;

                //  Create Header data in scratch area
                DramAddressP    = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                *DramAddressP++ = BHIGH(HostTransferLength);
                *DramAddressP++ = BLOW(HostTransferLength);
                *DramAddressP++ = 0;
                *DramAddressP++ = 0;

                HostTransferLength -= 2;

                // Send Header data to host
                SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);

                // Transfer DSS data and send Command Complete.
                SendParameterDataToHost(DMA_START_ADDR, HostTransferLength, TRUE);    // Transfer data and send Command Complete.
            }
            break;

        case DVDHDRAM_MEDIA_STS:  /*DVD-RAM media status*/
            if (pdbGetDVDType() == DVD_RAM)
            {
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                // Prepare Header data for host.
                *DramAddressP++ = 0;    // DVD Structure
                *DramAddressP++ = 6;    // Data Length.
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // PWP bit clear
                *DramAddressP++ = 0x10; // Disc Type Identification
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // RAM SWI Information

                SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(8,HostTransferLength), TRUE);    // Transfer data and send Command Complete.
            }
            break;

        case DVDHDRAM_SPARE_INFO:  /*Spare Areaa Information*/
            if (pdbGetDVDType() == DVD_RAM)
            {
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                // Prepare Header data for host.
                *DramAddressP++ = 0;    // DVD Structure
                *DramAddressP++ = 14;    // Data Length.
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // Reserved.

                // Number of unused Primary Spare blocks
                *DramAddressP++ = 0;
                *DramAddressP++ = 0;
                *DramAddressP++ = BHIGH(RWDisc.dvRAM.PrimarySARemainBlk);
                *DramAddressP++ = BLOW(RWDisc.dvRAM.PrimarySARemainBlk);

                // Number of unused Supplementary Spare blocks
                *DramAddressP++ = 0;
                *DramAddressP++ = 0;
                *DramAddressP++ = BHIGH(RWDisc.dvRAM.SupSARemainBlk);
                *DramAddressP++ = BLOW(RWDisc.dvRAM.SupSARemainBlk);

                // Number of allocated Supplementary Spare blocks
                TempW =(USHORT) ((DVDRAM_TOTAL_LSN - SWAPEND_ULONG(RWDisc.dvRAM.StSDLp->NumberOfLSN))/ONE_DVD_BLOCK);

                *DramAddressP++ = 0;
                *DramAddressP++ = 0;
                *DramAddressP++ = BHIGH(TempW);
                *DramAddressP++ = BLOW(TempW);

                SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(16, HostTransferLength), TRUE);    // Transfer data and send Command Complete.
            }
            break;

        case DVDHDRAM_MEDIA_REC_TYPE:  /*DVD-RAM Recording Type Information*/
            if (pdbGetDVDType() == DVD_RAM)
            {
                if(pdbGetDVDVersion() < DVD_VER_2_1)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0x10);        // INVALID FIELD IN CDB
                    AutoCommandComplete();
                    return;
                }

                /* Recording Type Bit is the Bit 4 of Data field information byte of Data ID */
                /* We need to read one block to get the Data ID */
                /*----------------------------------------------------------------*/
                dvdDiscRequest.Type            = INIT_READ;
                dvdDiscRequest.TargetID.Long   = ConvertLSNToPSN(0);  //read LSN = 0
                dvdDiscRequest.TargetLayer     = eLayer0;
                dvdDiscRequest.EccBlockCount   = 1;
                dvdramDiscRequestStartLSN      = 0;

                DiscReadStatus = dvdStartDiscTransfer();
                if (DiscReadStatus == BUF_GOOD)
                {
                    DramTempP = (BYTE *) (DVDRAM_ABS_AUX_SADDR + 0); //first aux, first byte
                    BufferData = *DramTempP;

                    if (BufferData & RECORDING_TYPE_BIT)  //bit 4
                        BufferData = RECORDING_TYPE_BIT;  //Modify by Mt. Fuji spec. revision 0.91
                    else
                        BufferData = 0;
                }
                else
                {
                    BUILD_SENSE(0x03,0x11,0x0B,0x0e);               // UNRECOVERED READ ERROR
                    AutoCommandComplete();
                    return;
                }
                /*----------------------------------------------------------------*/

                 DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                // Prepare Header data for host.
                *DramAddressP++ = 0;    // DVD Structure
                *DramAddressP++ = 6;    // Data Length.
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // Reserved.

                *DramAddressP++ = BufferData;    // Recording Type
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // Reserved.

                SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(8, HostTransferLength), TRUE);    // Transfer data and send Command Complete.
            }
            break;
#endif  // #if (DVD_RAM_READ == 1)

#if(HD_READ)
    case HD_COPYRIGHT_INFO:

        HostTransferLength = MINIMUM(2052,HostTransferLength);

        /* Do not include the data transfer length information in transfer length (4 bytes) */
        HostTransferLength -= 2;

        //  Create Header data in scratch area
        DramAddressP    = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
        *DramAddressP++ = BHIGH(HostTransferLength);
        *DramAddressP++ = BLOW(HostTransferLength);
        *DramAddressP++ = 0;
        *DramAddressP++ = 0;

        // Send Header data to host
        SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);

        // Transfer data and send Command Complete.
        DramAddressP = (BYTE *)HDDVD_COPYRIGHT_PROTECTION_INFORMATION;
        SendParameterDataToHost((ULONG) DramAddressP, HostTransferLength - 2 , TRUE);
        break;
#endif

#if (ENABLE_AACS)
    case DVDHD_AACS_COPYRIGHT_DATA:
        Address.byte.u = Iram_Cmd_block.bf[2];
        Address.byte.h = Iram_Cmd_block.bf[3];
        Address.byte.m = Iram_Cmd_block.bf[4];
        Address.byte.l = Iram_Cmd_block.bf[5];

        switch(pdbDiscLoaded())
        {
            case DVD_DISC:
                HostTransferLength = MINIMUM(28672 + 8, HostTransferLength);
                break;
    #if(HD_READ)
            case HD_DISC:
                HostTransferLength = MINIMUM(63488 + 8, HostTransferLength);
                break;
    #endif
        }


            /* Do not include the data transfer length information in transfer length (2 bytes) */
            HostTransferLength -= 2;

            // Point to work area in DRAM
            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);

            // Prepare Header for host
            *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
            *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
            *DramAddressP++ = 0;                              // Reserved
            *DramAddressP++ = 0;                              // Reserved


            dvdDiscRequest.TargetID.Long = Address.Long;
            dvdDiscRequest.Type            = INIT_READ;

            if(dvdDiscRequest.TargetID.Long > 0x800000)
                dvdDiscRequest.TargetLayer     = eLayer1;
            else
                dvdDiscRequest.TargetLayer     = eLayer0;

            switch(pdbDiscLoaded())
            {
                case DVD_DISC:
                    dvdDiscRequest.EccBlockCount   = 1L;
                    break;
    #if(HD_READ)
                case HD_DISC:
                    dvdDiscRequest.EccBlockCount   = 2L;
                    break;
    #endif
            }

            DiscReadStatus = dvdStartDiscTransfer();

            if (DiscReadStatus != BUF_GOOD)
            {
                ClearDataBuffer();                              // Clear the DVD buffer
                BUILD_SENSE(0x04,0x44,0x00,0x0c);               // INTERNAL TARGET FAILURE
                AutoCommandComplete();
                return;
            }

            // Send header to host.
            SendParameterDataToHost(DVD_CSS_SCRATCH, 8 , FALSE);


        switch(pdbDiscLoaded())
        {
            case DVD_DISC:
                DramAddressP = (BYTE *) (DVD_DATA_BUFFER_START + (2*DVD_SECTOR_SIZE));
                break;
    #if(HD_READ)
            case HD_DISC:
                DramAddressP = (BYTE *) (DVD_DATA_BUFFER_START);
                break;
    #endif
        }

        // Transfer AACS Copyright Information to host.
        SendParameterDataToHost((ULONG)DramAddressP, HostTransferLength - 6, TRUE);    // Transfer data and send Command Complete.
        break;
#endif
        case 0xFF:  /*Structure List*/
            base = StruFmtList;
            HostTransferLength = MINIMUM(sizeof(STRU_FMT_LIST_T), HostTransferLength);
            HostTransferLength -= 2;

            DramAddressP    = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);
            *DramAddressP++ = BHIGH(HostTransferLength);                      //  this data will later be transferred to host.
            *DramAddressP++ = BLOW(HostTransferLength);
            *DramAddressP++ = 0;
            *DramAddressP++ = 0;

            for(; base->FmtCode < 0xFF; base++)
            {
                if (base->FmtCode <= 0x07)
                {
                    *DramAddressP++ = base->FmtCode;
                    *DramAddressP++ = base->SDS_RDS;
                    *DramAddressP++ = BHIGH(base->Length);
                    *DramAddressP++ = BLOW(base->Length);
                }

                if ((base->FmtCode >= 0x08) && (base->FmtCode <= 0x0B))
                {
#if (DVD_RAM_READ == 1)
                    *DramAddressP++ = base->FmtCode;

                    if (pdbServoGetfDiscKind() == eDVDRAMDisc)
                        *DramAddressP++ = base->SDS_RDS;
                    else
                        *DramAddressP++ = 0x00;

                    *DramAddressP++ = BHIGH(base->Length);
                    *DramAddressP++ = BLOW(base->Length);
#endif
                }
            }

            SendParameterDataToHost(DVD_CSS_SCRATCH, HostTransferLength + 2, TRUE);
            break;

        case 0xC0:  /*Write Protection*/
#if (WRITE_PROTECT == 1)
            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = 0;    // DVD Structure
            *DramAddressP++ = 6;    // Data Length.
            *DramAddressP++ = 0;    // Reserved.
            *DramAddressP++ = 0;    // Reserved.

            if((pdbServoGetfDiscKind() == eDVDMinusRWDisc) && (Info.Media.DVD.RMAData.RMD.Format2.DiscStatus & bit7))
                *DramAddressP++ = bit1;
            else
                *DramAddressP++ = 0;
            *DramAddressP++ = 0;                            // Reserved.
            *DramAddressP++ = 0;                            // Reserved.
            *DramAddressP++ = 0;                            // Reserved.
            SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(8,HostTransferLength), TRUE);    // Transfer data and send Command Complete.
            break;
#endif

        default:
            BUILD_SENSE(0x05,0x24,0x00,0x21);        // INVALID FIELD IN CDB
            AutoCommandComplete();
            break;
    }
}

#if (BCA_ENABLE == 1)
/*********************************************************************
* Cmd_read_dvd_structure
*
* Read DVD Structure command handler
*
* Returns:      void
* Params:       none
*********************************************************************/
BYTE DVDReadBCA(void)
{
    ULONG   temp_timer;
    SHORT   BCATarget,orig_position;
    BYTE    BCACount, Count, BCA_retry, BCATimeOut;
    BYTE    *BufferPtrSrc, *BufferPtrDest;
    BOOL    bca_error;

    /*------------------------------------
        initial read bca
    --------------------------------------*/
    if (DVD_BUFFERING() == ON)
        dvdStopDiscTransfer();

    StartTimer(&temp_timer);
    if ((pdbGetDVDType() == DVD_ROM) && (pdbGetDVDLayers() == DUAL_LAYER))
    {
        while(1)
        {
            if (pdbGetDVDTrackPath() == DVDHD_OTP_DISC)
            {
                if (ServoSeek(0xFD0000,eLayer1) == PASS)
                    break;
            }
            else
            {
                if (ServoSeek(0x830000,eLayer1) == PASS)
                    break;
            }
            if (ReadTimer(&temp_timer) >= 700)
            {     //1 sec
                SendMsg80(SHOW_DEBUG_MSG,0x420014); //Read BCA SEEK FAIL
                return (FAIL);
            }
        }
    }
    else
    {
        while(1)
        {
            if (ServoSeek(0x030000,eLayer0) == PASS)
                break;
            if (ReadTimer(&temp_timer) >= 700)
            {     //1 sec
                SendMsg80(SHOW_DEBUG_MSG,0x420014); //Read BCA SEEK FAIL
                return (FAIL);
            }
        }
    }
    switch (ForBCALoadedDiscKind.fDiscKind)
    {
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
            WRITE_FIELD(HAL_DFE_BCANBCA,1);    //NBCA
            BCATarget = DVDNBCATarget;
            break;

        case eDVDROMDisc:
        case eDVDRAMDisc:
        #if (ENABLE_HDDVD == 1)
        case eHDROMDisc:
        #endif
        default:
            WRITE_FIELD(HAL_DFE_BCANBCA,0);    //BCA
            BCATarget = DVDBCATarget;
            break;
    }
    orig_position = Iram_stp_CurrentPosition;
    spdHoldRPM(svoSpeedVar.SpdInRange? svoSpeedVar.TargetRpm : svoSpeedVar.CurrentRpm);
    ServoBCASeek(BCATarget,OFF);

    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable);
    HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_AfeControl);  /* AFE control on AGC/AOC */

    BCACount = 0;
    BCA_retry = 0;
    dvdResetDiscFlags();
    CLEAR_DVD_DECODER_STATUS();
    SET_DVD_BLOCKS_TO_BUFFER(0x00); //reset buffer count
    START_DVD_BLOCK_TIMER(FIRST_BLOCK_TIMEOUT_VALUE);
    ENABLE_DVD_DECODER_INTS();

// DVDDECODER related
// BCA / NBCA functions moved to Host Module
// TBD    Need to revisit this code.
    HAL_GLOBAL_SetBcaMode();

    /* force tracking on */
    WRITE_FIELD(HAL_DVDD_FRCTRK,1);

    /* clear BCA information data */
    BufferPtrSrc  = (BYTE *) (SCSDRAM_START_ADDR + ((DataBuffer.CurDiscPtr) * DVD_SECTOR_SIZE));
    BufferPtrDest = (BYTE *) (SCSDRAM_START_ADDR + DVD_BCA);

    for (Count = 0; Count < MAX_BCA_BYTES; Count++)
    {
        *BufferPtrDest++ = *BufferPtrSrc++ = 0;
    }

    #if 0
                WRITE_FIELD(HAL_AFE_PDEQBUF, 0);    //open EQDOP
                //WRITE_FIELD(HAL_AFE_EQDOSEL, 2);    //open wobble
                WRITE_FIELD(HAL_AFE_EQDOSEL, 6);    //open RFO
                //WRITE_FIELD(HAL_AFE_WOBTEST, 5);    //wob after LPF
                //WRITE_FIELD(HAL_AFE_WOBVGA3EN,  1); //open DFE control wob AGC

                //show Defect flag
                //WRITE_FIELD(HAL_GLOBAL_MIO2SEL,1);      //open MIO2
                //WRITE_FIELD(HAL_GLOBAL_M2SEL,0x14);      //open 0x14 of MIO2
                //WRITE_FIELD(HAL_AFE_AFEMIOSEL,2);      //open TP1
                //WRITE_FIELD(HAL_AFE_MUXTP2SEL,1);      //open item 1 of TP2, include defecti
                //WRITE_FIELD(HAL_AFE_MUXTP1SEL,1);      //open item 1 of TP1, include defect
    #endif
    while(1)
    {
        HAL_DFE_SetBCAFreq(BCA_FREQ_INIT);

        WRITE_FIELD(HAL_GLOBAL_DVDSRST,1);  //reset decoder
        WRITE_FIELD(HAL_GLOBAL_DVDSRST,0);  //reset decoder
        SET_DVD_BLOCKS_TO_BUFFER(0x01);  /* reset buffer count */
        SET_DISC_BUFFER_PTR(DataBuffer.CurDiscPtr);
        WRITE_FIELD(HAL_DVDD_ENBUF,1);   /* trigger BCA decoding */
        WRITE_FIELD(HAL_DVDD_ENECCE,1);

        /*-------------------------------------
            wait for decode bca complete
        ---------------------------------------*/
        bca_error = FALSE;
        BCATimeOut = 0;
        StartTimer(&temp_timer);
        while (1)
        {
            /*---------------------------------------------------------------
                Check for buffered block errors posted by DVD ISR
            ----------------------------------------------------------------*/
            if (DVD_BLK_ERROR)
            {
                send_msg5S(1,"DVD_BLK_ERROR");
                bca_error = TRUE;
            }
            /*---------------------------------------------------------------
                Check for buffering errors (non block errors)
            ----------------------------------------------------------------*/
            if (DVD_BUFFER_ERROR)
            {
                send_msg5S(1,"DVD_BUFFER_ERROR");
                bca_error = TRUE;
            }

            /*---------------------------------------------------------------
                Check if all requested blocks have been buffered
            ----------------------------------------------------------------*/
            if (dvdDecIntFlag.fECC_BLK_INT)
            {
                dvdDecIntFlag.fECC_BLK_INT = CLEAR;
                break;
            }
    #if (ENABLE_HDDVD == 1)
            if (dvdDecIntFlag.fCLUSTER_BLK_INT)
                break;
    #endif
            switch (BCATimeOut)
            {
                case 0:
                    if (ReadTimer(&temp_timer) > (180000 / svoSpeedVar.TargetRpm))//timeout = 3 revolutions
                    {
                        //send_msg5S(1,"time1");
                        BCATimeOut = 1;
                        BufferPtrSrc  = (BYTE *)(SCSDRAM_START_ADDR + ((DataBuffer.CurDiscPtr) * DVD_SECTOR_SIZE));
                        BufferPtrDest = (BYTE *)(SCSDRAM_START_ADDR + DVD_BCA);
                        WRITE_FIELD(HAL_DVDD_ENBUF,0);   /* disable BCA decoding */
                        DISABLE_DVD_BUFFERING();
                        HAL_DFE_SetBCAFreq(BCA_FREQ_HIGH);
                        WRITE_FIELD(HAL_GLOBAL_DVDSRST,1);  //reset decoder
                        WRITE_FIELD(HAL_GLOBAL_DVDSRST,0);  //reset decoder
                        SET_DVD_BLOCKS_TO_BUFFER(0x01);     /* reset buffer count */
                        SET_DISC_BUFFER_PTR(DataBuffer.CurDiscPtr);
                        WRITE_FIELD(HAL_DVDD_ENBUF,1);      /* trigger BCA decoding */
                        WRITE_FIELD(HAL_DVDD_ENECCE,1);
                    }
                    break;

                case 1:
                    if (ReadTimer(&temp_timer) > (300000 / svoSpeedVar.TargetRpm))//timeout = 5 revolutions
                    {
                        //send_msg5S(1,"time2");
                        BCATimeOut = 2;
                        BufferPtrSrc  = (BYTE *)(SCSDRAM_START_ADDR + ((DataBuffer.CurDiscPtr) * DVD_SECTOR_SIZE));
                        BufferPtrDest = (BYTE *)(SCSDRAM_START_ADDR + DVD_BCA);
                        WRITE_FIELD(HAL_DVDD_ENBUF,0);   /* disable BCA decoding */
                        DISABLE_DVD_BUFFERING();
                        HAL_DFE_SetBCAFreq(BCA_FREQ_LOW);
                        WRITE_FIELD(HAL_GLOBAL_DVDSRST,1);  //reset decoder
                        WRITE_FIELD(HAL_GLOBAL_DVDSRST,0);  //reset decoder
                        SET_DVD_BLOCKS_TO_BUFFER(0x01);     /* reset buffer count */
                        SET_DISC_BUFFER_PTR(DataBuffer.CurDiscPtr);
                        WRITE_FIELD(HAL_DVDD_ENBUF,1);      /* trigger BCA decoding */
                        WRITE_FIELD(HAL_DVDD_ENECCE,1);
                    }
                    break;

                case 2:
                    if (ReadTimer(&temp_timer) > (420000 / svoSpeedVar.TargetRpm))//timeout = 7 revolutions
                    {
                        BCATimeOut = 3;
                    }
                    break;
            }

            if (BCATimeOut == 3)
            {
                send_msg5S(1,"BCA TIMEOUT");
                bca_error = TRUE;
            }

            /*---------------------------------------------------------------
                Check for eject
            ----------------------------------------------------------------*/
            if (EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
            {
                send_msg5S(1,"BCA TRAY PRESS");
                BCA_retry = MAX_BCA_RETRY_TIMES;
                bca_error = TRUE;
                break;
            }

            if (bca_error == TRUE)
            {
                break;
            }

            ExitProcess();
        }

        if (READ_FIELD(HAL_DVDD_BCAERR))
        {
            send_msg5S(1,"BCAERR");
            bca_error = TRUE;
            WRITE_FIELD(HAL_DVDD_BCAERR,0);
        }
        if (bca_error)
        {
            BufferPtrSrc  = (BYTE *)(SCSDRAM_START_ADDR + ((DataBuffer.CurDiscPtr) * DVD_SECTOR_SIZE));
            BufferPtrDest = (BYTE *)(SCSDRAM_START_ADDR + DVD_BCA);
            WRITE_FIELD(HAL_DVDD_ENBUF,0);   /* disable BCA decoding */
            DISABLE_DVD_BUFFERING();
            SendMsg80(SHOW_DEBUG_MSG,0x420016); //Read BCA FAIL
            if (BCA_retry < MAX_BCA_RETRY_TIMES)
            {
                BCA_retry++;
                //shift position
                ServoBCASeek((BCA_retry & 0x01)? (BCATarget + (BCA_retry / 2 + 1) * UM_2_SLED_STEP(200)) : (BCATarget - (BCA_retry / 2) * UM_2_SLED_STEP(200)),OFF);
            }
            else
            {
                break;
            }
        }
        else
        {
            SendMsg80(SHOW_DEBUG_MSG,0x420015); //Read BCA OK
            break;
        }
    }
    BCACount = (16 * READ_FIELD(HAL_DVDD_BCACNT)) - 4;

    /* force tracking off */
    WRITE_FIELD(HAL_DVDD_FRCTRK,0);
                                 // DVDDECODER related
    HAL_GLOBAL_ClrBcaMode();     // TBD    Need to revisit this code.

    spdUnhold();
    HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_DfeControl);  /* AFE control on AGC/AOC */
    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);
    ServoBCASeek(orig_position,ON);//improve BCA jump (handle focus off during jump)

    Svr_pause_reqire = TRUE;

    if(BCA_retry >= MAX_BCA_RETRY_TIMES)
    {
        gBcaInfoAvailable = FALSE;
        return FAIL;
    }

    dvdDiscRequest.BcaLen = BCACount;

    BufferPtrSrc  = (BYTE *) (SCSDRAM_START_ADDR + ((DataBuffer.CurDiscPtr) * DVD_SECTOR_SIZE));
    BufferPtrDest = (BYTE *) (SCSDRAM_START_ADDR + DVD_BCA);

    for (Count = 0; Count < BCACount; Count++)
    {
        *BufferPtrDest++ = *BufferPtrSrc++;
    }

#if (ENABLE_AACS == 1) && (ENABLE_AVCREC == 1)
    if ((pdbGetDiscKind() != eDVDROMDisc) && (pdbGetDiscKind() != eDVDROMDLDisc))
    {
        BYTE mediaid[] = {0x25,0xB9,0x46,0xEB,0xC0,0xB3,0x61,0x73};

        BufferPtrDest = (BYTE *) (SCSDRAM_START_ADDR + DVD_BCA + 4);
        for (Count = 0; Count < 8; Count++)
        {
            AacsAuth.ID.MediaID[Count] = mediaid[Count];
            AacsAuth.ID.MediaID[Count+8] = *BufferPtrDest++;
        }
    }
#endif

#if 0 //show BCA information from sunMSG
    BufferPtrDest = (BYTE *) (SCSDRAM_START_ADDR + DVD_BCA);

    for (Count = 0; Count < BCACount; Count++)
    {
        send_msg82(1,0x5F0048,Count,*BufferPtrDest++);
    }
#endif

    gBcaInfoAvailable = TRUE;

    return PASS;
}
#endif // BCA_ENABLE
