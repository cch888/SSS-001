/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRdStru.c $
*
* DESCRIPTION   This file contains Read DVD Structure command
*
* $Revision: 63 $
* $Date: 10/12/21 4:15p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*********************************************************************
  INCLUDE FILES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"

#include ".\hif\Gp_ram.h"
#include ".\hif\Rd_Stru.h"
#include ".\hif\Gp_mac.h"
#include ".\hif\sv_util.h"
#include ".\player\Plrdb.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdRdStru.h"
#include ".\bd\bdMacro.h"

#if (BCA_ENABLE == 1)
#include ".\al\REG_dfe.h"
#include ".\al\HAL_afe.h"
#include ".\al\HAL_dfe.h"
#include ".\al\LAL.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_step.h"
#include ".\al\REG_global.h"
#include ".\al\HAL_global.h"
#endif

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

#if (BD_ENABLE)
#include ".\defmgmt\bdwritable\bddfmg.h"

#include ".\BD\bdDiscRd.h"
#include ".\dvd\dvDiscInit.h"
#include ".\dvd\dvDef.h"
#include ".\AL\REG_demod.h"
BYTE * DFLDataPtr;
extern ULONG DFLStrAddr;

/*******************************************************************************

    FUNCTION        ReadBDStructureCommand

    DESCRIPTION     Read BD Structure command handler

    RETURNS         None

********************************************************************************/
void ReadBDStructureCommand(void)
{
    StLongU  TempL, AddressField;
    ULONG    TotalSASize, PAC_ID;
    BYTE     *DramAddressP;
    WORD     HostTransferLength;
    BYTE     SentAgid;
    BYTE     FormatField, PACFormat;
    BYTE     MediaType;
    BYTE     NrOfPackages;
    BYTE     result,i,pg_offset;
    tBdPacS  *tempPAC;
    if( BD_BUFFERING() == ON )
        bdStopDiscTransfer();

    bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    ClearDataBuffer();

    AddressField.byte.u = Iram_Cmd_block.bf[2];
    AddressField.byte.h = Iram_Cmd_block.bf[3];
    AddressField.byte.m = Iram_Cmd_block.bf[4];
    AddressField.byte.l = Iram_Cmd_block.bf[5];

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    /* Media Type */
    MediaType = Iram_Cmd_block.bf[1] & 0x0F;

    /* Get Format */
    FormatField = Iram_Cmd_block.bf[7];

    /* Get AGID */
    SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6;

    if (FormatField == BD_RAW_DFL && AddressField.Long > 15)
    {
        BUILD_SENSE(0x05,0x24,0x00,0xbd);       /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }
    else if ((FormatField == BD_DDS) && ((pdbGetDiscKind() == eBDROMDisc) || (pdbGetDiscKind() == eBDROMDLDisc))) //2009.0821
    {
        BUILD_SENSE(0x05,0x24,0x00,0xbd);       /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    if(MediaType == 0x01) //BD DISC
    {
        switch(FormatField)
        {
            case BD_DISC_INFO:
                //-------------------------------------------------
                // Disc Information (DI)
                //-------------------------------------------------
                /* Insure that maximum transfer length is not exceeded */
                HostTransferLength = MINIMUM(4100,HostTransferLength);

                HostTransferLength -= 2;

                // Point to work area in DRAM
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);

                // Prepare Header for host
                *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                // Send header to host.
                SendParameterDataToHost(BUFFER_SCRATCH, 4 , FALSE);

                // Transfer Disc Manufacturing Information to host.
                SendParameterDataToHost((ULONG)BD_DI_ADR, HostTransferLength - 2, TRUE);    // Transfer data and send Command Complete.
                break;

            case BD_DDS:
                //-------------------------------------------------
                // Disc Definition Structure (DDS)
                //-------------------------------------------------
                /* Insure that maximum transfer length is not exceeded */
                HostTransferLength = MINIMUM(2052,HostTransferLength);

                HostTransferLength -= 2;

                // Point to work area in DRAM
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);

                // Prepare Header for host
                *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                // Send header to host.
                SendParameterDataToHost(BUFFER_SCRATCH, 4 , FALSE);

                // Transfer Disc Manufacturing Information to host.
                SendParameterDataToHost((ULONG)pDDS, HostTransferLength - 2, TRUE);    // Transfer data and send Command Complete.
                break;

            case BD_CARTRIDGE_STS:
                //-------------------------------------------------
                // Cartridge Status
                //-------------------------------------------------
                /* Insure that maximum transfer length is not exceeded */
                HostTransferLength = MINIMUM(8,HostTransferLength);

                HostTransferLength -= 2;

                // Point to work area in DRAM
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);

                // Prepare Header for host
                *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                /* b7: Cartridge bit = 0, medium is not in a cartridge
                   b6: Out bit = 0, Cartridge bit is set to zero, the Out bit shall be set to zero
                   b5-3: Reserved
                   b2: Cartridge Write Protection (CWP) bit = 0, Cartridge bit is set to zero, the CWP bit shall be set to zero
                   b1-0: Reserved */
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                // Transfer Disc Manufacturing Information to host.
                SendParameterDataToHost((ULONG)Info.Media.BD.DiscInfoP_L0_Fmt1, HostTransferLength + 2, TRUE);    // Transfer data and send Command Complete.
                break;

            case BD_SA_INFO:
                //-------------------------------------------------
                // Spare Area Information
                //-------------------------------------------------
                /* Insure that maximum transfer length is not exceeded */
                HostTransferLength = MINIMUM(16,HostTransferLength);

                HostTransferLength -= 2;

                // Point to work area in DRAM
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);

                // Prepare Header for host
                *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                /* B0-3: Reserved */
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                TotalSASize = pdbGetBDIsa0Size() + pdbGetBDIsa1Size() + 2 * pdbGetBDOsaSize();

                /* B4-8: Number of Free Spare Blocks */
                if (pdbGetBDType() == BD_R)
                    TempL.Long = TotalSASize /* - spare size used for TDMA */  - (pdbGetBdrRadCnt() * SECTORS_PER_CLUSTER);
                else
                    TempL.Long = pdbGetBdreSpareCnt() * SECTORS_PER_CLUSTER;
                *DramAddressP++ = TempL.byte.u;
                *DramAddressP++ = TempL.byte.h;
                *DramAddressP++ = TempL.byte.m;
                *DramAddressP++ = TempL.byte.l;

                /* B9-11: Number of Allocated Spare Blocks */
                TempL.Long = TotalSASize;
                *DramAddressP++ = TempL.byte.u;
                *DramAddressP++ = TempL.byte.h;
                *DramAddressP++ = TempL.byte.m;
                *DramAddressP++ = TempL.byte.l;

                // Transfer Disc Manufacturing Information to host.
                SendParameterDataToHost((ULONG)Info.Media.BD.DiscInfoP_L0_Fmt1, HostTransferLength + 2, TRUE);    // Transfer data and send Command Complete.

                break;

            case BD_RAW_DFL:
                //-------------------------------------------------
                // Raw Defect List (DFL)
                //-------------------------------------------------
                if (!((pdbGetBDType() == BD_R) || (pdbGetBDType() == BD_RE)))
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xbe);       /* INVALID FIELD IN CDB */
                    AutoCommandComplete();
                    return;
                }

                // One Package is 32K.
                if (pdbGetBDLayers() == BD_SINGLE_LAYER)
                    NrOfPackages = 8;   //For SL, max DFL is 4 clusters, that is 8 packages.
                else
                    NrOfPackages = 16;  //For DL, max DFL is 8 clusters, that is 16 packages.

                if (AddressField.Long >= NrOfPackages)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xbf);       /* INVALID FIELD IN CDB */
                    AutoCommandComplete();
                    return;
                }

                /* Insure that maximum transfer length is not exceeded */
                HostTransferLength = MINIMUM(0x8004,HostTransferLength);

                HostTransferLength -= 2;

                // Point to work area in DRAM
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);

                // Prepare Header for host
                *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = NrOfPackages;                   // NrOfPackages

                // Send header to host.
                SendParameterDataToHost(BUFFER_SCRATCH, 4 , FALSE);

                // Point to Dfl Assembled area in DRAM
    //read DFL again. Startup flow doesn't add all info to entry, only RAD info.
                if(BDReadDFL()==BUF_GOOD)
                {
                    DramAddressP = DFLDataPtr;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc0);       /* INVALID FIELD IN CDB */
                    AutoCommandComplete();
                    return;
                }
                // Transfer Raw Defect List to host.
                SendParameterDataToHost((ULONG)(DramAddressP + AddressField.Long * 0x8000), HostTransferLength - 2, TRUE);    // Transfer data and send Command Complete.
                break;

            case BD_PAC:  //TBD !!
                //-------------------------------------------------
                // Physical Access Control (PAC)
                //-------------------------------------------------
                PAC_ID = AddressField.Long >> 8;
                PACFormat= (BYTE) AddressField.Long;

                /* Insure that maximum transfer length is not exceeded */
                HostTransferLength = MINIMUM(2052,HostTransferLength);

                // Point to work area in DRAM
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);

                // Prepare Header for host
                *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                *DramAddressP++ = 0;                              // Reserved
                *DramAddressP++ = 0;                              // Reserved

                // Send header to host.
                SendParameterDataToHost(BUFFER_SCRATCH, 4 , FALSE);
                HostTransferLength -= 4;
                LinearPatternFill(0,0,1,0x800/2,0);//first buffer start page fill zero for transfer zero data.
    //
                if(PAC_ID == (ULONG)BD_ALL_PAC_HEADER) //Return a list of PAC headers of all PACs that are written
                {
                    for(i=0;i<PACcnt;i++)
                    {
                        if(HostTransferLength> BD_PAC_HEADER_LENGTH)
                        {
                            SendParameterDataToHost((ULONG)BD_PRM_PAC_ADR+BD_SECTOR_SIZE*i, BD_PAC_HEADER_LENGTH, FALSE);
                            HostTransferLength -= BD_PAC_HEADER_LENGTH;
                        }
                        else
                        {
                                SendParameterDataToHost((ULONG)BD_PRM_PAC_ADR+BD_SECTOR_SIZE*i, HostTransferLength, TRUE);
                                HostTransferLength = 0;
                            break;
                        }
                    }
                    if(HostTransferLength!=0)
                        SendParameterDataToHost((ULONG)SCSDRAM_ADDRESS, HostTransferLength, TRUE);
                }
                else if (PAC_ID == (ULONG)BD_KNOWN_PAC_HEADER)  //Return a list of PAC headers of all PACs that are known to the Drive
                {
                        SendParameterDataToHost((ULONG)BD_PRM_PAC_ADR, HostTransferLength, TRUE);
                }
                else  //The PAC information of the addressed PAC shall be returned.
                {
                    pg_offset = CheckPACHeader((BYTE*)&PAC_ID);
                    if(pg_offset == 0xFF)
                    {
                        send_msg5S(1,"undefine PAC ID");
                        BUILD_SENSE(0x05,0x24,0x00,0x20);        // INVALID FIELD IN CDB
                        AutoCommandComplete();
                    }
                    else
                    {
                        tempPAC = (tBdPacS *)(BD_PRM_PAC_ADR+pg_offset*BD_SECTOR_SIZE);
                        result = (SWAPEND_ULONG(tempPAC->UnknowPACRules) >> ePACArea) & eRead;
                        if(result == 0)
                        {
                            SendParameterDataToHost((ULONG)BD_PRM_PAC_ADR+BD_SECTOR_SIZE*pg_offset, HostTransferLength, TRUE);
                        }
                        else
                        {
                            if(HostTransferLength > BD_PAC_HEADER_LENGTH)
                            {
                                SendParameterDataToHost((ULONG)BD_PRM_PAC_ADR+BD_SECTOR_SIZE*pg_offset, BD_PAC_HEADER_LENGTH, FALSE);
                                SendParameterDataToHost((ULONG)SCSDRAM_ADDRESS,HostTransferLength-BD_PAC_HEADER_LENGTH, TRUE);
                            }
                            else
                            {
                                SendParameterDataToHost((ULONG)BD_PRM_PAC_ADR+BD_SECTOR_SIZE*pg_offset, HostTransferLength, TRUE);
                            }
                        }
                    }
                }
                break;

            default:
                BUILD_SENSE(0x05,0x24,0x00,0x21);        // INVALID FIELD IN CDB
                AutoCommandComplete();
                break;
        }
    }
    else
    {
        BUILD_SENSE(0x05,0x30,0x04,0x21);        // INVALID FIELD IN CDB
        AutoCommandComplete();
    }
}

#if (BCA_ENABLE == 1)
/*******************************************************************************

    FUNCTION        BDReadBCA

    DESCRIPTION     Read BD BCA Area

    RETURNS         PASS / FAIL

********************************************************************************/

BYTE BDReadBCA(void)
{
    SBYTE       BCA_retry;
    BYTE        *DramAddressP;
    SHORT       orig_position;
    ULONG       temp;
    BYTE        ContentCode, UnitSeqNr, SubId, Length, Category;
    BYTE        BCAFREQBD;
#if (ENABLE_AACS == 1)
    BYTE        i;
#endif
#if (ENABLE_BDRLTH == 1)
    BYTE        DPP_Polarity_Flag,Mark_Polarity_Flag;
#endif

//start ==To judge BD BCA valid or not.
#if(JudgeBDBCAValid == 1)
    if(ReadBDBCAValid == TRUE)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x420026); //Read BD BCA Done
        return PASS;
    }
#endif
//end ==To judge BD BCA valid or not.

    BCA_retry = 10;//must >= 2

    /*------------------------------------
        initial read bca
    --------------------------------------*/
    if( BD_BUFFERING() == ON )
        bdStopDiscTransfer();

    bdResetDiscFlags();

    /*-------------------------------------
        seek to Data area
    --------------------------------------*/
    if (Layer_Index != 0)//improve BCA reading
    {//seek to L0
        if((bdSeek(svoRadius2PBA(Iram_stp_CurrentPosition,0), eLayer0)) == Error)
        {
            /*---------------------------------------------------------------
                Seek fail
            ----------------------------------------------------------------*/
            SendMsg80(SHOW_DEBUG_MSG,0x420014); //Read BCA SEEK FAIL

            bdErrorFlag.fSEEK_FAIL = TRUE;
            BUILD_SENSE(0x03,0x02,0x00,0x0f);    //no seek complete
            return FAIL;
        }
    }
    orig_position = Iram_stp_CurrentPosition;
    spdHoldRPM(svoSpeedVar.SpdInRange? svoSpeedVar.TargetRpm : svoSpeedVar.CurrentRpm);

    Defect_Enable(DEFECT_STATE_FREEZE_ALL_OFF);
    ServoBCASeek(BCA_TARGET_BD,OFF);
    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable);
    HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_AfeControl);  /* AFE control on AGC/AOC */
    HAL_GLOBAL_SetBcaMode();

    HAL_BD_CODEC_ENABLE_BCA_DECODER(); //enable BCA decoder

    HAL_BD_CODEC_SET_BCA_BUFFER_ADR(0);//Set BCA SDRAM start address

    //Enable the interrupts
    HAL_BD_CODEC_ENABLE_BCA_DECODER_INTS();

    while(1)
    {
        HAL_DFE_SetBCAFreq(111);
        HAL_BD_CODEC_BCA_STOP_DECODE();
        HAL_BD_CODEC_CLEAR_DECODER_STATUS();
        LAL_BD_CODEC_SetBCADecoder();
        HAL_BD_CODEC_BCA_START_DECODE();

        BCAFREQBD = READ_FIELD(HAL_DFE_BCAFREQ);

        StartTimer(&temp);
        while (1)
        {
            if (bdDecIntFlag.fBCA_DONE_INT == SET)
            {
                bdDecIntFlag.fBCA_DONE_INT = CLEAR;
                break;
            }
            if (ReadTimer(&temp) > (420000 / svoSpeedVar.TargetRpm))//timeout = 7 revolutions
            {
                bdErrorFlag.fBCA_ERROR = SET;
                break;
            }
            else if (ReadTimer(&temp) > (300000 / svoSpeedVar.TargetRpm))//timeout = 5 revolutions
            {
                if (READ_FIELD(HAL_DFE_BCAFREQ) != BCAFREQBD)
                {
                    HAL_DFE_SetBCAFreq((BCA_retry & 0x01)? 133 : 95);
                    HAL_BD_CODEC_BCA_STOP_DECODE();
                    HAL_BD_CODEC_CLEAR_DECODER_STATUS();
                    LAL_BD_CODEC_SetBCADecoder();
                    HAL_BD_CODEC_BCA_START_DECODE();
                    BCAFREQBD = READ_FIELD(HAL_DFE_BCAFREQ);
                }
            }
            else if  (ReadTimer(&temp) > (180000 / svoSpeedVar.TargetRpm))//timeout = 3 revolutions
            {
                if (READ_FIELD(HAL_DFE_BCAFREQ) == BCAFREQBD)
                {
                    HAL_DFE_SetBCAFreq((BCA_retry & 0x01)? 95 : 133);
                    HAL_BD_CODEC_BCA_STOP_DECODE();
                    HAL_BD_CODEC_CLEAR_DECODER_STATUS();
                    LAL_BD_CODEC_SetBCADecoder();
                    HAL_BD_CODEC_BCA_START_DECODE();
                }
            }
            ExitProcess();
        }

        if (bdErrorFlag.fBCA_ERROR == SET)
        {
            bdErrorFlag.fBCA_ERROR = CLEAR;
            if (--BCA_retry < 0)
            {
                break;
            }
            if (BCA_retry < 2)//improve BCA reading
            {//shift position for last 2 tries
                ServoBCASeek((BCA_retry == 1)? (BCA_TARGET_BD + UM_2_SLED_STEP(200)) : (BCA_TARGET_BD - UM_2_SLED_STEP(200)),OFF);
            }
        }
        else
        {
            DramAddressP = (BYTE *)SCSDRAM_ADDRESS;
            if ( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
                 pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
            {
                for (temp = 0; temp < 128; temp+=32)
                {
#if 0 //Don't delete. For dumping all 128 bytes of BCA content
                    send_msg5SValue(1, "temp", 1, temp);
                    for (i=0;i<32;i+=4)
                    {
                        send_msg54(1,(*(DramAddressP+temp+i)),(*(DramAddressP+temp+i+1)),(*(DramAddressP+temp+i+2)),(*(DramAddressP+temp+i+3)));
                    }
#endif
                    ContentCode = ((*(DramAddressP+temp)) & 0xFC) >> 2;
                    UnitSeqNr = (*(DramAddressP+temp)) & 0x03;
                    SubId = ((*(DramAddressP+temp+1)) & 0xF0) >> 4;
                    Length = (*(DramAddressP+temp+1)) & 0x0F;
                    Category = ((*(DramAddressP+temp+2)) & 0xF0) >> 4;

                    if (ContentCode == 0x01 && UnitSeqNr == 0x00 && SubId == 0x01 && Length == 0x0E)
                    {
#if (ENABLE_AACS == 1)
                        for (i = 0; i < 16; i++)
                            AacsAuth.ID.MediaID[i] = *(DramAddressP+temp+i);
#endif  /* ENABLE_AACS == 1 */
                    }
                    else if (ContentCode == 0x3F)
                    {
                        // Basic Disc Information
#if (ENABLE_BDRLTH == 1)  //temp edwin
                        DPP_Polarity_Flag = (*(DramAddressP+temp+7));
                        Mark_Polarity_Flag = (*(DramAddressP+temp+8));
                        if ((DPP_Polarity_Flag != 0)||( Mark_Polarity_Flag != 0))
                        {
                            svoIramVar.LTHDisc = 1;
                            SendMsg80(SHOW_DEBUG_MSG,0x42001A); //BDRLTH by BCA
                        }
                        else
                        {
                            svoIramVar.LTHDisc = 0;
                        }
#endif
#if (ENABLE_BDIH == 1)
                        //BDIH disc adjudgment by BCA       2009.0925
                        if(*(DramAddressP+temp+1) == 'B' && *(DramAddressP+temp+2) == 'D' && *(DramAddressP+temp+3) == 'H')
                        {
                            svoIramVar.IHDisc = 1;
                            send_msg5S(SHOW_DEBUG_MSG,"BDIH by BCA");
                        }
                        else
                        {
                            svoIramVar.IHDisc = 0;
                            send_msg5S(SHOW_DEBUG_MSG,"Not BDIH");
                        }
#endif  //#if (ENABLE_BDIH == 1)
                    }
                }
            }
            else
            {
#if (ENABLE_AACS == 1)
                for (temp = 0; temp < 128; temp+=32)
                {
                    ContentCode = ((*(DramAddressP+temp)) & 0xFC) >> 2;
                    if (ContentCode == 0x01)
                    {
                        for (i = 0; i < 16; i++)
                            AacsAuth.MediaSN[i] = *(DramAddressP+temp+i);

                        break;
                    }
                }
#endif  /* ENABLE_AACS == 1 */
            }
            break;
        }
    }

    HAL_BD_CODEC_BCA_STOP_DECODE(); //Stop decoding
    HAL_GLOBAL_ClrBcaMode();     // TBD    Need to revisit this code.

    HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_DfeControl);  /* DFE control on AGC/AOC */
    ServoBCASeek(orig_position,ON); //improve BCA jump (handle focus off during jump)
    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);
    spdUnhold();
    Defect_Enable(DEFECT_STATE_UNFREEZE);

    if (BCA_retry >= 0)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x420015); //Read BCA OK
        //start ==To judge BD BCA valid or not.
        #if(JudgeBDBCAValid == 1)
        ReadBDBCAValid = TRUE;
        #endif
        //end ==To judge BD BCA valid or not.
        return PASS;
    }
    else
    {
        SendMsg80(SHOW_DEBUG_MSG,0x420016); //Read BCA FAIL
        return FAIL;
    }
}

#endif
/*****************************************************************************

    FUNCTION        CheckPACHeader

    DESCRIPTION     Check Header for different types

    PARAMETERS      Data start pointer, Header type

    RETURNS         Buffer page that contains the header

*****************************************************************************/
BYTE CheckPACHeader(BYTE *HeaderType)
{
    tBdPacS *ptr;
    BYTE pg_no;

    /* check for header */
    for(pg_no = 0; pg_no < PACcnt; pg_no++)
    {
        ptr = (tBdPacS *) (BD_PRM_PAC_ADR + pg_no * BD_SECTOR_SIZE);
        if((ptr->PACIDL == *HeaderType) && (ptr->PACIDM == *(HeaderType+1)) && (ptr->PACIDH == *(HeaderType+2)))
        {
            return(pg_no);
        }
    }
    return(0xFF);

}

/*******************************************************************************

    FUNCTION        BDReadDFL

    DESCRIPTION     Read BD DFL Area

    RETURNS         PASS / FAIL

********************************************************************************/
BYTE BDReadDFL(void)
{

    USHORT pg_offset;
    BYTE  Valid_Info;
    DiscAccessStatusE result;
    HeaderTypeE Header;

    /* read DFL Info */
    WRITE_FIELD(HAL_BDC_ADNOTAUN, 1);  //wobble disc, use ADIP lock
    #if (ENABLE_DISCARD_ICE == 1)
    HAL_BD_CODEC_Disable_DiscardIce();          /* Disable discard iced clusters */
    #endif
    #if (HF_BASED_CUTOFF == 1)
    WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //Initial relinking cut off base on demodulator
    WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //Initial don't force in run mode in runin area
    #endif

    bdDecFlag.fREADING_LEADIN = TRUE;
    Valid_Info = FALSE;
    BdwtDisc.ReadDataMode = eNOT_USERDATA;
    /* DFL is 4 cluster long for BD SL, 8 cluster long for BD DL*/
    /* DFL valid location is specified at DDS */
    /*---------------------------------------------------------------
        Set up the Disc Transfer control parameters
    ----------------------------------------------------------------*/
    bdDiscRequest.Type            = INIT_READ;
    bdDiscRequest.TargetLayer     = eLayer0;

    switch (pdbGetBDLayers())
    {
        case BD_SINGLE_LAYER:
        default:
            bdDiscRequest.ClusterCount = 4;
            break;

        case BD_DUAL_LAYER:
            bdDiscRequest.ClusterCount = 8;
            break;
    }

    switch (pdbGetBDType())
    {
        case BD_R:
        default:
            {
                result = GEN_FAIL;

                {
                    ClearDataBuffer();
                    send_msg5SValue(SEND_SStpTask_MSG, "R-DFL at", 4, DFLStrAddr);


                    bdDiscRequest.TargetID.Long = DFLStrAddr&0x00FFFFFF;
                    if (DFLStrAddr&0x10000000)// TDMA
                        bdDiscRequest.ClusterCount = 1;
                    result = bdStartDiscTransfer();
                    DFLDataPtr = (BYTE*) SCSDRAM_ADDRESS;

                    if (result == BUF_GOOD)
                    {
                        /* check for DFL header */
                        Header = eDFL;
                        pg_offset = CheckHeader(DFLDataPtr, &Header);
                        send_msg5SValue(1,"pg_off",4,pg_offset);
                        if(Header == eDFL)
                        {   /* Valid DFL Info */
                            //pg_offset is the DFL start page
                            send_msg5SValue(SEND_SStpTask_MSG, "pg_off", 2, pg_offset);

                            DFLDataPtr = (BYTE *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE);
                            Valid_Info = TRUE;
                        }
                    }
                }
            }
            break;

        case BD_RE: /* BD-RE read DFL only 1 time */
            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            ClearDataBuffer();
            bdDiscRequest.TargetID.Long   = pdbGetBDDflPSN();

            send_msg5SValue(SEND_SStpTask_MSG, "RE-DFL at", 4, bdDiscRequest.TargetID.Long);

            result = bdStartDiscTransfer();
            DFLDataPtr = (BYTE*) SCSDRAM_ADDRESS;
            if (result == BUF_GOOD)
            {
                /* check for DFL header */
                Header = eDFL;
                pg_offset = CheckHeader(DFLDataPtr, &Header);

                if(Header == eDFL)
                {   /* Valid DFL Info */
                    //pg_offset is the DFL start page
                    send_msg5SValue(SEND_SStpTask_MSG, "pg_off", 2, pg_offset);

                    DFLDataPtr = (BYTE *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE);
                    Valid_Info = TRUE;
                }
            }
            break;
    }

    bdDecFlag.fREADING_LEADIN = FALSE;

    if (result != BUF_GOOD || !Valid_Info)
        return GEN_FAIL;

    send_msg80(SEND_SStpTask_MSG,0x7100D7);  //DFL Valid

    result = BUF_GOOD;

    return result;
}

#endif  //BD_ENABLE

