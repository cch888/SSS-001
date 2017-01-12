/****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME:     $Workfile: dvISR.c $
*
*   DESCRIPTION:    Manages the DVD services for DVD host transfers and
*                   DVD-DSP interrupts
*
*
*   $Revision: 275 $
*   $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*******************************************************************
    INCLUDE FILES
*******************************************************************/
#include ".\common\globals.h"
#include ".\common\commath.h"
#include ".\dvd\dvCSS.h"
#include ".\common\macros.h"
#include ".\common\iram.h"
#include ".\common\com_im.h"
#include ".\hif\gp_util.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvIsr.h"
#include ".\dvd\dvBufMgr.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvDiscRd.h"
#include ".\dvd\dvdiscinit.h"
#include ".\servo\svo_com.h"
#if (VCPS_TEST ==1)
#include ".\dvd\dvvcps.h"
#endif
#include ".\dvd\dvutil.h"
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
#include ".\al\reg_dfe.h"
#endif

#include ".\servo\svo_dsp.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\ramread.h"
#include ".\defmgmt\dvdram\ramExtern.h"
#include ".\al\hal_dfe.h"
#endif

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#endif
#include ".\al\reg_dvd_decoder.h"
#include ".\al\reg_dvd_encoder.h"
#include ".\al\reg_global.h"

#if (ENABLE_AACS == 1)
#include ".\crypt\aacsdef.h"
#endif

#if (DPD_COMPENSATE_OFFSET == 1)
#include ".\servo\svoVar.h"
#endif//#if (DPD_COMPENSATE_OFFSET == 1)

#if (READ_MODIFY_WRITE == 1)
extern BYTE gReadModifyWrite;
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_function_test.h"
#endif

extern BYTE Send_timef;
extern BOOL ResetDecoder;
/*******************************************************************
    PROTOTYPES
*******************************************************************/
void DVDDecoderIntProc(void);
void dvdProcessEccBlock(void);
void dvdCheckForHostTransfer(void);

#if (DVD_VERIFY_ID == 1)
BYTE dvdVerifyID(void);
#endif

/*******************************************************************
    MODULE VARIABLES
********************************************************************/

// PI/PO LOGGING MODULE VARIABLES
//BYTE *BufferStart = (BYTE *)(SCSDRAM_START_ADDR + 0x150000L);
//BYTE *BufferEnd = (BYTE *)(SCSDRAM_START_ADDR + 0x1E0000L);
//BYTE *BufferPtr = (BYTE *)(SCSDRAM_START_ADDR + 0x150000L);

#if (ENABLE_AACS == 1)
extern BOOL fBypassAuth;
extern BOOL fReadingMid;
extern BYTE gMidKeyIndex;
extern BYTE MidProcessData[MID_BYTE_LENGTH];
extern AacsS AacsStatus;
#endif

#if (EN_DVD_HIGH_JITTER_DETECT  == 1)
extern BYTE	HIGH_JITTER_DISC;
#endif

/*******************************************************************************

    FUNCTION        DVDDecoderIntProc

    DESCRIPTION     Handles DVD Decoder interrupts and status

********************************************************************************/
BOOL vwcTargetValueRead;
//extern SHORT vwdc_closed_loop_DAC;//zephyr

void DVDDecoderIntProc()
{
#if (E_DIAGS == 1)
    USHORT temp_Hstbuf;
#endif
    BYTE dvdIrqStatus1;
    BYTE dvdIrqStatus2;
    BYTE dvdIrqStatus3;
    BYTE dvdError;
    #if((Read_RETRY_ERROR_INDEX_RESEARCH == 1) && (HORROR_DISC_IMPROVEMENT == 1))
    BYTE *  AuxP;
    #endif

    /*---------------------------------------------------------------
        Get all DVD Decoder Status &   Get all DVD Dncoder IRQ Mask status
    ---------------------------------------------------------------*/
    dvdIrqStatus1 = READ_REG(HAL_DVDDREG_MAP->DVDIRQS1) & READ_REG(HAL_DVDDREG_MAP->DVDIRQC1);
    dvdIrqStatus2 = READ_REG(HAL_DVDDREG_MAP->DVDIRQS2) & READ_REG(HAL_DVDDREG_MAP->DVDIRQC2);
    dvdIrqStatus3 = READ_REG(HAL_DVDDREG_MAP->DVDIRQS3) & READ_REG(HAL_DVDDREG_MAP->DVDIRQC3);


    CLEAR_DVD_DECODER_STATUS();

    //this flag means interrupt has happened, it is used for decoder overshoot
    dvdDecIntFlag.fNO_INT = FALSE;

    /*---------------------------------------------------------------
        Clear error flag
    ---------------------------------------------------------------*/
    dvdError = FALSE;

#if (DVD_RAM_READ == 1)
    if ((dvdIrqStatus1 & HAL_DVDD_BUFFI) && (pdbGetDVDType() == DVD_RAM))
    {
        //send_msg5SValue(SEND_MRW_MSG,"BUFI",4,RWDisc.dvRAM.CurrentPID);
        //send_msg5SValue(SEND_MRW_MSG, "CurDsPtr", 2, DataBuffer.CurDiscPtr);
        RWDisc.dvRAM.IceHasBeenDetected = CLEAR;
    }
#endif

    /*---------------------------------------------------------------
        Disable sync lost interrupt if all blocks have been bufferred
    ---------------------------------------------------------------*/
    if( dvdIrqStatus1 & HAL_DVDD_BUFDNI )
    {
        dvdDecIntFlag.fBUF_DONE_INT = SET;      /* Set flag         */
    }

#if (E_DIAGS == 1)
    /*-----------------------------------------------------------------
        Read PIPO value keep away CURECC & BufPgCMP pointer
        Decoder will never stop if there is no Error occured
    ------------------------------------------------------------------*/
    if(READ_PI_PO == 1)
    {
        temp_Hstbuf = READ_FIELD(HAL_DVDD_CURECC);
        temp_Hstbuf += 5*ONE_DVD_BLOCK;     /* Advance the  pointer */
        temp_Hstbuf %= MAX_DVD_SECTORS;     /* Insure buffer wrap */
        WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL,temp_Hstbuf);
    }
#endif // E_DIAGS

    /*---------------------------------------------------------------
        Check ECC Block interrupt
    ---------------------------------------------------------------*/
    if( dvdIrqStatus1 & HAL_DVDD_ECCI )  // Raw ECC Block Check
    {
        DISABLE_TARGET_MISS_INT();                  /* Disable TID miss int */

        dvdDecIntFlag.fECC_BLK_INT = SET;           /* Set flag         */

#if (BCA_ENABLE == 1)
        if(READ_FIELD(HAL_GLOBAL_BCAMODE)==1)
            return;
#endif

        #if (READ_MODIFY_WRITE == 1)
        if(gReadModifyWrite == FALSE)   //do not update DataBuffer.CurDiscPtr when execute ReadModifyWrite()
        #endif
        {
        DataBuffer.CurDiscPtr += ONE_DVD_BLOCK;     /* Advance the disc pointer */
        DataBuffer.CurDiscPtr %= MAX_DVD_SECTORS;   /* Insure buffer wrap */
        }

        /*---------------------------------------------------------------
            Conditional code to gather PI/PO information
        ---------------------------------------------------------------*/
#if (DVD_ZONE_TEST == TRUE)
        if (dvdTest.TestComplete != TRUE)
        {
            dvdTest.PICCount += READ_REG(HAL_DVDDREG_MAP->PISCORL);
            dvdTest.PIUCount += READ_REG(HAL_DVDDREG_MAP->PIUCOR);
            dvdTest.POCCount += READ_REG(HAL_DVDDREG_MAP->POSCORL);
            dvdTest.POUCount += READ_REG(HAL_DVDDREG_MAP->POUCOR);
            dvdTest.BlockCount++;
        }
#endif

//        if (dvdDecFlag.fLOG_PIPO == TRUE)
//        {
//            Auxptr = (BYTE *) ((READ_REG(HAL_DVDDREG_MAP->CUREAUXL) << 8) + SCSDRAM_START_ADDR);
//            ++Auxptr;
//
//            *BufferPtr++ = 0xEE;
//            *BufferPtr++ = *Auxptr++;
//            *BufferPtr++ = *Auxptr++;
//            *BufferPtr++ = *Auxptr++;
//            *BufferPtr++ = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
//            *BufferPtr++ = READ_REG(HAL_DVDDREG_MAP->POUCOR);
//            *BufferPtr++ = dvdIrqStatus1;
//            *BufferPtr++ = dvdIrqStatus2;
//
//            if( BufferPtr >= BufferEnd )
//                BufferPtr = BufferStart;
//        }

#if (DVD_RAM_READ == 1)
        if (DVDRAM)
         {
            PI_C = READ_REG(HAL_DVDDREG_MAP->PISCORL );
            PI_UN_C = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
            PO_C = READ_REG(HAL_DVDDREG_MAP->POSCORL);
            PO_UN_C = READ_REG(HAL_DVDDREG_MAP->POUCOR);
            PI_E = READ_REG(HAL_DVDDREG_MAP->PIRXTH) + PI_UN_C;

            //send_msgA2(SEND_MRW_MSG,0xFFF000,PI_C);
            //send_msgA2(SEND_MRW_MSG,0xFFF001,PI_UN_C);
            //send_msgA2(SEND_MRW_MSG,0xFFF002,PO_C);
            //send_msgA2(SEND_MRW_MSG,0xFFF003,PO_UN_C);

            if(PO_UN_C == 0 && RWDisc.dvRAM.ReadSkip == 1)
            {
                RWDisc.dvRAM.ReadSkip = 0;
            }

    #if (E_DIAGS == 1)
            //if (READ_PI_PO == 0)
            {
    #endif
                if (ramVerifyID() == FALSE)
                {
                    send_msg80(SEND_ATAPI_CMD,0x710029);
                    dvdErrorFlag.fID_MISMATCH = SET;
                    dvdError = TRUE;
                }
    #if (E_DIAGS == 1)
            }
    #endif

    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
            if(PO_UN_C == 0 && IceFound == TRUE)
            {
                ClearDVDRAMIceIndicatorAll();
            }
    #endif
         }
#endif // #if (DVD_RAM_READ == 1)

#if (DVD_RAM_READ == 1) && (DVD_RAM_CHECK_DEFECT)
         /* Check the status of all sectors in the ECC Block */
         /* If it reaches to the threshold of the defective sector, we will mark this sector as 'defect' */
         if (pdbGetDVDType() == DVD_RAM)
         {
             if (RWDisc.dvRAM.ReplaceMode == eSLIP_REPLACEMENT)
                 RamCheckDefectiveSector();
             else if (RWDisc.dvRAM.ReplaceMode == eLINEAR_REPLACEMENT)
                 RamCheckDefectiveBlock();
             RWDisc.dvRAM.DefectiveBitMap = 0;  /* Reset defective bitmap */
         }
#endif

        /*---------------------------------------------------------------
            Check for uncorrectable ecc error
        ---------------------------------------------------------------*/
        if( dvdIrqStatus1 & HAL_DVDD_UNECCI )
        {
            dvdErrorFlag.fUNCOR_ECC = SET;          /* Set flag         */
            dvdError = TRUE;                        /* Mark error       */


#if (E_DIAGS == 1)
            if(READ_PI_PO == 1)
            {
                WAIT_PI_PO = 0;
            }
#endif

#if (CEProduct == 0)
    #if (DPD_COMPENSATE_OFFSET == 1)
        #if (DVD_RAM_READ == 1)
            if (!DVDRAM)
        #endif
            {
                if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                {
                    if(SpeedCount == (DVD_SPEED_DOWN_READY+2))//just before speed down
                    {
//                        cur_zone = calRadius2Zone(Iram_stp_CurrentPosition);
//                        svoCalTiltVar[Layer_Index][cur_zone].status &= (~CAL_R_DONE);         //Need Re-Cal Tilt//Clear done flag, then can do this calibration again in next retry
//                        svoCalFebcVar[Layer_Index][cur_zone].status &= (~CAL_R_DONE);         //Need Re-Cal FEBC//Clear done flag, then can do this calibration again in next retry
                        svoCalDpdVar[Layer_Index][svoDpdReg2Bank[READ_FIELD(HAL_AFE_DPDHSISCL)]][READ_FIELD(HAL_AFE_DPDHSFCH)].status |= CAL_REQUIRED;//Need Re-Cal DPD Offset//Clear done flag, then can do this calibration again in next retry
                    }
                }
            }
    #endif  //#if (DPD_COMPENSATE_OFFSET == 1)
#endif

#if (DVD_RAM_READ == 1)
            send_msg84L(SEND_MRW_MSG,0x580703,RWDisc.dvRAM.CurrentPID);// "PID xx"
#endif
            send_msg80(SEND_ATAPI_CMD|DEBUG_DVD_WRITE_MSG, 0x710136);
#if((Read_RETRY_ERROR_INDEX_RESEARCH == 1) && (HORROR_DISC_IMPROVEMENT == 1))
            PI_C = READ_REG(HAL_DVDDREG_MAP->PISCORL );
            PI_UN_C = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
            PO_C = READ_REG(HAL_DVDDREG_MAP->POSCORL);
            PO_UN_C = READ_REG(HAL_DVDDREG_MAP->POUCOR);
            //PI_E = READ_REG(HAL_DVDDREG_MAP->PIRXTH) + PI_UN_C;
            PI_E = READ_FIELD(HAL_DMOD_DMDERACNT);
            AuxP = (BYTE *) (CUR_AUX_ADDRESS() - (READ_REG(HAL_DVDDREG_MAP->STARTBKL) * 16));
            PIPO_CURRENT_ID.byte.u = *AuxP++;
            PIPO_CURRENT_ID.byte.h = *AuxP++;
            PIPO_CURRENT_ID.byte.m = *AuxP++;
            PIPO_CURRENT_ID.byte.l = *AuxP++;

            //PI = %d, PI_U = %d, PO_C = %d, PO_U = %d, DMDERACNT = %d
            SendMsgCnB(SEND_ATAPI_CMD|SEND_READ_DVD_MSG,10,0x710141,B2B(PI_C),B2B(PI_UN_C),B2B(PO_C),B2B(PO_UN_C),B2B(PI_E));
            SendMsgCnB(SEND_ATAPI_CMD|SEND_READ_DVD_MSG,4,0x710143,B4B(PIPO_CURRENT_ID.Long));               
#endif
        }

        /*---------------------------------------------------------------
            Check for EDC error
        ---------------------------------------------------------------*/
        if( dvdIrqStatus2 & HAL_DVDD_EDCERI )
        {
            dvdErrorFlag.fEDC_ERROR = SET;          /* Set flag         */
            dvdError = TRUE;                        /* Mark error       */
            send_msg80(SEND_ATAPI_CMD|DEBUG_DVD_WRITE_MSG, 0x710137);
#if (DVD_RAM_READ == 1) && (WORKAROUND_FOR_SEEDMISTHR == 1)
            if (DVDRAM)
            {
                ResetSeedMisThr = TRUE;
                if((PO_C >= 0x7D0) && (PO_UN_C == 0) && (dvdErrorFlag.fID_ERROR == CLEAR) && ( dvdErrorFlag.fUNCOR_ECC == CLEAR))
                {
                    ChangeSeedMisThr = TRUE;
                    send_msg80(SEND_ATAPI_CMD|DEBUG_DVD_WRITE_MSG, 0x71002F);
                }
            }
#endif
#if((Read_RETRY_ERROR_INDEX_RESEARCH == 1) && (HORROR_DISC_IMPROVEMENT == 1))
            if( dvdIrqStatus1 & HAL_DVDD_UNECCI )
            {
                PI_C = READ_REG(HAL_DVDDREG_MAP->PISCORL );
                PI_UN_C = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
                PO_C = READ_REG(HAL_DVDDREG_MAP->POSCORL);
                PO_UN_C = READ_REG(HAL_DVDDREG_MAP->POUCOR);
                //PI_E = READ_REG(HAL_DVDDREG_MAP->PIRXTH) + PI_UN_C;
                PI_E = READ_FIELD(HAL_DMOD_DMDERACNT);
                AuxP = (BYTE *) (CUR_AUX_ADDRESS() - (READ_REG(HAL_DVDDREG_MAP->STARTBKL) * 16));
                PIPO_CURRENT_ID.byte.u = *AuxP++;
                PIPO_CURRENT_ID.byte.h = *AuxP++;
                PIPO_CURRENT_ID.byte.m = *AuxP++;
                PIPO_CURRENT_ID.byte.l = *AuxP++;

                //PI = %d, PI_U = %d, PO_C = %d, PO_U = %d, DMDERACNT = %d
                SendMsgCnB(SEND_ATAPI_CMD|SEND_READ_DVD_MSG,10,0x710141,B2B(PI_C),B2B(PI_UN_C),B2B(PO_C),B2B(PO_UN_C),B2B(PI_E));
                SendMsgCnB(SEND_ATAPI_CMD|SEND_READ_DVD_MSG,4,0x710143,B4B(PIPO_CURRENT_ID.Long));  
            }
#endif
        }

#if (SUN_FUNC_TEST == 1)
        if (fStartBuffer == TRUE)
        {
            usBlerCount++;
            PO_UN_C = READ_REG(HAL_DVDDREG_MAP->POUCOR);
            PI_UN_C = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
            PI_E = READ_REG(HAL_DVDDREG_MAP->PIRXTH) + PI_UN_C;
            usSUM_PI_C1 += PI_E;
            usSUM_PO_C2 += PO_UN_C;
            if (usBlerCount == usBLER_UNIT)
            {
                usUnitCount++;
                usBlerCount = 0;   
            }                            
        }
#endif
        

    } /* End if for ECC Block interrupt check*/
    /*---------------------------------------------------------------
        Check for ECC buffering done status/interrupt
    ---------------------------------------------------------------*/
    if( dvdIrqStatus1 & HAL_DVDD_ECCDNI )
    {
        dvdDecIntFlag.fECC_DONE_INT = SET;         /* Set flag         */
        send_msg80(SEND_READ_DVD_MSG,0x710139);
    }


    /*---------------------------------------------------------------
        Check for BUFI if enabled
    ---------------------------------------------------------------*/
    if( dvdDecFlag.fBUFI_ENABLED == TRUE )
    {
        if (dvdIrqStatus1 & HAL_DVDD_BUFFI)       // One block interrupt
        {
            dvdDecIntFlag.fBUF_BLK_INT = SET;     /* Set flag         */
        }

        if (dvdIrqStatus1 & HAL_DVDD_BUFDNI)      // All blocks done interrput
        {
            dvdDecIntFlag.fBUF_DONE_INT = SET;    /* Set flag         */
        }
    }

    /*---------------------------------------------------------------
        Check for target ID miss interrupt
    ---------------------------------------------------------------*/
    if( dvdIrqStatus2 & HAL_DVDD_TIDMISI )
    {
        if (pdbGetDVDType() != DVD_RAM)
        {
            dvdErrorFlag.fTID_MISS = SET;               /*  Set flag        */
            dvdError = TRUE;                            /*  Mark error      */
            //send_msg80(SEND_ATAPI_CMD|DEBUG_DVD_WRITE_MSG, 0x710032);
#if (HORROR_DISC_IMPROVEMENT == 1)
            PO_UN_C = 4444;
#endif
            SendMsg80(SEND_ATAPI_CMD|DEBUG_DVD_WRITE_MSG,0x71013A);     // DVD Read - Target ID Miss
        }
#if (DVD_RAM_READ == 1)
        else
        {
            // DVD_RAM disc
            if (RWDisc.dvRAM.CurrentPID <= READ_REG(HAL_DVDDREG_MAP->TID0))
            {
                //It is following now, we should ignore wrong TIDMISS interrupt
                return;
            }
            else
            {
                //It is real over-shoot
                dvdErrorFlag.fTID_MISS = SET;               /*  Set flag        */
                dvdError = TRUE;                            /*  Mark error      */
                send_msg80(SEND_ATAPI_CMD,0x710033);
            }
        }
#endif
#if 0//#if((Read_RETRY_ERROR_INDEX_RESEARCH == 1) && (HORROR_DISC_IMPROVEMENT == 1))
                PI_C = READ_REG(HAL_DVDDREG_MAP->PISCORL );
                PI_UN_C = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
                PO_C = READ_REG(HAL_DVDDREG_MAP->POSCORL);
                PO_UN_C = READ_REG(HAL_DVDDREG_MAP->POUCOR);
                //PI_E = READ_REG(HAL_DVDDREG_MAP->PIRXTH) + PI_UN_C;
                PI_E = READ_FIELD(HAL_DMOD_DMDERACNT);
                AuxP = (BYTE *) (CUR_AUX_ADDRESS() - (READ_REG(HAL_DVDDREG_MAP->STARTBKL) * 16));
                PIPO_CURRENT_ID.byte.u = *AuxP++;
                PIPO_CURRENT_ID.byte.h = *AuxP++;
                PIPO_CURRENT_ID.byte.m = *AuxP++;
                PIPO_CURRENT_ID.byte.l = *AuxP++;

                //PI = %d, PI_U = %d, PO_C = %d, PO_U = %d, DMDERACNT = %d
                SendMsgCnB(SEND_ATAPI_CMD|SEND_READ_DVD_MSG,10,0x710141,B2B(PI_C),B2B(PI_UN_C),B2B(PO_C),B2B(PO_UN_C),B2B(PI_E));
                SendMsgCnB(SEND_ATAPI_CMD|SEND_READ_DVD_MSG,4,0x710143,B4B(PIPO_CURRENT_ID.Long));  
#endif
    }

#if (E_DIAGS == 1)
    if (dvdDecIntFlag.fECC_BLK_INT == SET)
    {
        if(READ_PI_PO == 1)
        {

            if(WAIT_PI_PO == 0)
            {
                #if((Read_RETRY_ERROR_INDEX_RESEARCH == 1) && (HORROR_DISC_IMPROVEMENT == 1))
                BYTE *  AuxP;
                #endif

                WAIT_PI_PO = 1;
                PI_C = READ_REG(HAL_DVDDREG_MAP->PISCORL );
                PI_UN_C = READ_REG(HAL_DVDDREG_MAP->PIUCOR);
                PO_C = READ_REG(HAL_DVDDREG_MAP->POSCORL);
                PO_UN_C = READ_REG(HAL_DVDDREG_MAP->POUCOR);
                PI_E = READ_REG(HAL_DVDDREG_MAP->PIRXTH) + PI_UN_C;

                AuxP = (BYTE *) (CUR_AUX_ADDRESS() - (READ_REG(HAL_DVDDREG_MAP->STARTBKL) * 16));
                PIPO_CURRENT_ID.byte.u = *AuxP++;
                PIPO_CURRENT_ID.byte.h = *AuxP++;
                PIPO_CURRENT_ID.byte.m = *AuxP++;
                PIPO_CURRENT_ID.byte.l = *AuxP++;

    #if (DVD_RAM_READ == 1)
                if (pdbGetDVDType() == DVD_RAM)
                    RWDisc.dvRAM.BLER_ID = eRamData_ID;
    #endif

                /*
                Work around for bug # TBD? BUG!
                When PI is small but PO UC = 182, or PI UC is always 208
                , this is a bug of DVD decoder, should reset dvd decoder to do buffering again.
                Or the PO UC & PI UC value would be always 182 and 208.
                this problem is on SC5220 and SC5120.
                */
                if((PI_C <= 100) && ((PO_UN_C == 182) || (PI_UN_C == 208)))
                {
                    ResetDecoder = TRUE;
                }
                else
                {
                    ResetDecoder = FALSE;
                }
    #if (MSG_OUT_MODE == MSG_USBPORT)
                if (Send_timef && (hifDebugMsgFlag & SEND_QCODE_DATAID))
                {
                    send_msg5SValue(SEND_QCODE_DATAID,"Data ID",4,PIPO_CURRENT_ID.Long);
                }
    #endif
            }
        }
    }

    if ((svoVar_discKindT.Bit.Media != eMediaROM) && (svoVar_discKindT.Bit.Media != eMediaRAM))
    {
        if (READ_PI_PO == 1)
        {
            return;
        }
    }
#endif  // E_DIAGS

    /*----------------------------------------------------------------------------

        Process Handling

    -----------------------------------------------------------------------------*/
    /*****************************************************************************

        Process: ECC block

    *****************************************************************************/
    if( dvdDecIntFlag.fECC_BLK_INT || dvdDecIntFlag.fBUF_BLK_INT )
    {

        dvdDecIntFlag.fECC_BLK_INT = CLEAR;
        dvdDecIntFlag.fBUF_BLK_INT = CLEAR;

#if ((DVD_VERIFY_ID == 1) && (BypassDecoderEr == 0))
        /*---------------------------------------------------------------
            Conditional code to verify correct block ID
        ---------------------------------------------------------------*/
        if (dvdDecFlag.fREADING_LEADIN == FALSE)
        {
    #if (E_DIAGS == 1)
            if (READ_PI_PO == 0)
            {
    #endif
                if (pdbGetDVDType() != DVD_RAM)
                {
                    if (dvdVerifyID() == FALSE)
                    {
                        dvdErrorFlag.fID_MISMATCH = SET;
                        dvdError = TRUE;
                    }
                }
    #if (E_DIAGS == 1)
            }
    #endif
        }
#endif /* DVD_VERIFY_ID */

#if (DVD_DISABLE_CSS_CHECK == FALSE)
        /*---------------------------------------------------------------
            CSS

            Check for title keys if disc is protected and authentication
            not set. If any sector in the ECC block has a title key
            under this condition post a read error
        ---------------------------------------------------------------*/

    #if (E_DIAGS == 1)
        if (READ_PI_PO == 0)
    #endif

    #if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
        if( (pdbDVDDiscCSS() == eCSS_CPS) && (dvdCss.CssFlag.AuthenticationSuccess == FALSE) )
    #else
        if( (pdbDVDDiscCSS() == TRUE) && (dvdCss.CssFlag.AuthenticationSuccess == FALSE) )
    #endif

            if ((dvdDecFlag.fREADING_LEADIN != TRUE)&&(dvdCss.eCssState != READ_TITLE_KEY)) // Bypass if in read title key state
                // Title Key information can be extracted from SDRAM memory in CPR_MAI area (located in Aux area for each sector of ECC Block)
                if (TitleKeyStatus())        // Check for the existence of a Title Key.
                {
                    dvdError = TRUE;
                    dvdErrorFlag.fUNAUTH_READ_ERROR = SET;
                }

#endif // DVD_DISABLE_CSS_CHECK

#if (VCPS_TEST ==1)
        /*---------------------------------------------------------------
            VCPS - Read validation (OK TO READ DKB WITHOUT AUTHORIZATION
        ---------------------------------------------------------------*/
        if ( (VcpsKeysValid == TRUE) && (pdbDVDDiscVCPS() == TRUE ) && (vcpsData.Authorized == FALSE) )
            if( vcpsData.DKBState != DKB_READING )
            {
                    dvdError = TRUE;
                    dvdErrorFlag.fUNAUTH_READ_ERROR = SET;
            }
#endif // VCPS_TEST

#if (ENABLE_AACS == 1 && BAPASS_AACS_AUTH == 0)
        if ( (fBypassAuth == FALSE) && (AacsStatus.AacsFlag.IsMediumCompliant == TRUE) && (AacsStatus.AacsFlag.AuthenticationSuccess == FALSE) )
        {
            if (dvdDecFlag.fREADING_LEADIN != TRUE)
            {
                dvdError = TRUE;
                dvdErrorFlag.fUNAUTH_READ_ERROR = SET;
            }
        }
#endif

        /*---------------------------------------------------------------
            For lead in blocks we will continue to buffer error blocks
            until an error free block is buffered or the requested
            number of lead-in blocks have been reached.
        ---------------------------------------------------------------*/
        if( dvdDecFlag.fREADING_LEADIN == TRUE )
        {
            if (dvdError == TRUE)
            {
                /*---------------------------------------------------------------
                    If a lead in block has an error reset the dvdError flag
                    and all dvd error flags.  This will force buffering to continue.
                    If it is the last block of the request and an error has occured
                    leave the error condition.
                ----------------------------------------------------------------*/
                if (dvdDiscRequest.EccBlockCount != (dvdDiscRequest.EccBlocksBuffered + 1L))
                {
                    send_msg80(SEND_READ_DVD_MSG,0x710034);

                    //temporary disable leadin pass when read error to let retry can do to prevent get wrong controldata zone data.
                    //dvdResetErrorFlags();
                    //dvdError = FALSE;
                }

            }
            else
            {
                /*---------------------------------------------------------------
                 If a lead in block is good (error free) mark that buffering is
                 done and stop buffering
                -----------------------------------------------------------------*/
                {
                    DISABLE_DVD_BUFFERING();
                    dvdDecIntFlag.fECC_DONE_INT = SET;
                }
            }
        }

#if (DVD_ZONE_TEST == TRUE)
        /*---------------------------------------------------------------
            DISPLAY PIPO INFORMATION TEST CODE. Do not flag any
            block error
        ---------------------------------------------------------------*/
        if (dvdTest.TestComplete != TRUE)
            dvdError = FALSE;
#endif

#if (DVD_RAM_READ == 1) && (ENABLE_RAM_DFE_ICE_DETECT == 1)
        if ((pdbGetDVDType() == DVD_RAM))
        {
            USHORT page;
            BYTE  *IceIndicator;

            if(DataBuffer.CurDiscPtr == 0)
                page = MAX_DVD_SECTORS - ONE_DVD_BLOCK;
            else
                page = DataBuffer.CurDiscPtr - ONE_DVD_BLOCK;

            IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR + (page >> 4));  //IceIndicator is one byte revolution

            /*---------------------------------------------------------------
                If a DVD-RAM ICE block is decoded, ignore the decoder errors.
            ---------------------------------------------------------------*/
            if ((*IceIndicator & eICE_BLOCK) || (RWDisc.dvRAM.SkippedBlock))
            {
                //send_msg5S(SEND_DVDRAM_ICE, "True ICE");

                #if E_DIAGS
                if(RWDisc.dvRAM.ReadDataMode == eREAD_ERROR_RATE)
                {
                    if(READ_PI_PO == 1)
                    {
                        if(dvdError && WAIT_PI_PO)
                            WAIT_PI_PO = 0;

                        if(WAIT_PI_PO == 0)
                        {
                            WAIT_PI_PO = 1;
                            RWDisc.dvRAM.BLER_ID = eRamHeader_ID;
                        }
                    }

                    PI_C = 230;
                    PO_C = 0;
                    PI_UN_C = 0;
                    PO_UN_C = 0;
                    PI_E = 0;
                }
                #endif

                dvdResetErrorFlags();
                dvdError = FALSE;
                if (RWDisc.dvRAM.ReadingDMA == TRUE)
                {
                    send_msg80(SEND_MRW_MSG,0x580706);// "Blank DMA"
                    RWDisc.dvRAM.BlankDMA = SET;
                }
            }
        }
#endif // #if (DVD_RAM_READ == 1)

#if (ENABLE_AACS == 1)
        if (fReadingMid == TRUE)
        {
            USHORT SecEdcSts;
            BYTE index;

            send_msg80(SEND_READ_DVD_MSG,0x710035);
            dvdErrorFlag.fUNCOR_ECC = CLEAR;          /* Set flag         */
            dvdErrorFlag.fEDC_ERROR = CLEAR;          /* Set flag         */
            dvdErrorFlag.fID_ERROR = CLEAR;           /* Set flag         */
            dvdErrorFlag.fSEQ_ID_ERROR = CLEAR;       /* Set flag         */
            dvdError = FALSE;                         /* Mark error       */
            SecEdcSts = (USHORT)READ_REG(HAL_DVDDREG_MAP->EDCERRL);
            for (index = 0; index < 16; index++)
            {
                if (((SecEdcSts >> index) & 0x01) == 0)    // EDC Ok
                {
                    // clear the bit
                    MidProcessData[(gMidKeyIndex+index) / MID_BITS_IN_BYTE] &= ~(BYTE)(1 << ((MID_BITS_IN_BYTE - 1) - ((gMidKeyIndex+index) % MID_BITS_IN_BYTE)));
                }
                else
                {
                    // set the bit
                    MidProcessData[(gMidKeyIndex+index) / MID_BITS_IN_BYTE] |= (BYTE)(1 << ((MID_BITS_IN_BYTE - 1) - ((gMidKeyIndex+index) % MID_BITS_IN_BYTE)));
                }
            }
            gMidKeyIndex += 16;
        }
#endif  /* ENABLE_AACS == 1 */

        /*---------------------------------------------------------------
            Process good ECC block else stop buffering
        ----------------------------------------------------------------*/
        if( dvdError == FALSE )
        {
            dvdProcessEccBlock();
#if (BypassDecoderEr == 1)
            if(ReadGoodCnt<0xFFFF)
                ReadGoodCnt++;
#endif
        }
        else
        {
            /*---------------------------------------------------------------
                Set up recovery for next read. This function sets up the
                read to be performed at a lower speed
            ---------------------------------------------------------------*/
#if (BypassDecoderEr == 1)
            ReadGoodCnt=0;
#endif
            RecoverDecoderError();

            if (dvdDecFlag.fREAD_AHEAD == TRUE)
            {
                /*---------------------------------------------------------------
                    If read ahead clear read ahead flag and Set system for pause
                ----------------------------------------------------------------*/
                Svr_pause_reqire        = TRUE;
                Iram_Svr_player_mode    = PAUSING_MODE;
            }

            /*---------------------------------------------------------------
                Stop buffering process if there is an error and no host blocks
                are being buffered. Any error on a host block being buffered
                will be handled outside of this isr.
            ---------------------------------------------------------------*/
            /*---------------------------------------------------------------
                Stop buffering
            ----------------------------------------------------------------*/
            dvdStopDiscTransfer();

            /*---------------------------------------------------------------
                Reset CurDiscPtr to previous location
            ----------------------------------------------------------------*/
#if (READ_MODIFY_WRITE == 1)
            if(gReadModifyWrite == FALSE)   //do not update DataBuffer.CurDiscPtr when execute ReadModifyWrite()
#endif
            {
            if( DataBuffer.CurDiscPtr == 0 )
                DataBuffer.CurDiscPtr =  (MAX_DVD_SECTORS - ONE_DVD_BLOCK);
            else
                DataBuffer.CurDiscPtr -= ONE_DVD_BLOCK;
            }
        }
        return;
    }

    /*****************************************************************************

        Process: Sync Lost

    *****************************************************************************/
    if( dvdErrorFlag.fSYNC_LOST )
    {

#if (DVD_ZONE_TEST == TRUE)
        /*---------------------------------------------------------------
            DISPLAY PIPO INFORMATION TEST CODE.
        ---------------------------------------------------------------*/
        if (dvdTest.TestComplete != TRUE)
            DVD_MESSAGE(PRN_DVDHIF,"** SYNC LOST **");
#endif

        /*---------------------------------------------------------------
            Set up recovery for next read. This function sets up the
            read to be performed at a lower speed
        ---------------------------------------------------------------*/
        RecoverDecoderError();

        if (dvdDecFlag.fREAD_AHEAD == TRUE)
        {
            /*---------------------------------------------------------------
                If read ahead set system for pause
            ----------------------------------------------------------------*/
            Svr_pause_reqire        = TRUE;
            Iram_Svr_player_mode    = PAUSING_MODE;
        }

        /*---------------------------------------------------------------
            Stop buffering
        ----------------------------------------------------------------*/
#if (E_DIAGS == 1)
        if (READ_PI_PO == 0)
#endif
        dvdStopDiscTransfer();

        return;
    }

    /*****************************************************************************

        Process: Target ID Miss

    *****************************************************************************/
    if( dvdErrorFlag.fTID_MISS )
    {

        if (dvdDecFlag.fREAD_AHEAD == TRUE)
        {
            /*---------------------------------------------------------------
                If read ahead set system for pause
            ----------------------------------------------------------------*/
            Svr_pause_reqire        = TRUE;
            Iram_Svr_player_mode    = PAUSING_MODE;
        }

        /*---------------------------------------------------------------
            Stop buffering
        ----------------------------------------------------------------*/
#if (E_DIAGS == 1)
        if (READ_PI_PO == 0)
#endif
        dvdStopDiscTransfer();

        return;
    }
}

/*******************************************************************************

    FUNCTION        dvdProcessEccBlock

    DESCRIPTION     Processes a valid Ecc block

********************************************************************************/
void dvdProcessEccBlock()
{
    /*---------------------------------------------------------------
        Reset DVD Block Timer
    ---------------------------------------------------------------*/
    START_DVD_BLOCK_TIMER ((pdbGetDVDType() == DVD_RAM) ? DVDRAM_BLOCK_TIMEOUT_VALUE : DVD_BLOCK_TIMEOUT_VALUE);

    /*---------------------------------------------------------------
        Update count of blocks transferred, buffer count and
        last PSN in buffer.
    ---------------------------------------------------------------*/
    ++dvdDiscRequest.EccBlocksBuffered;
    DataBuffer.SectorCount  += ONE_DVD_BLOCK;
    dvdLastDecodedPSN       += ONE_DVD_BLOCK;
#if (DVD_RAM_READ == 1)
    dvdramLastDecodedLSN    += ONE_DVD_BLOCK;
#endif
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    dvdramDecodedStrPSN     += ONE_DVD_BLOCK;
#endif

#if (DVD_RAM_READ == 1)
    if(pdbGetDVDType() == DVD_RAM)
    {
        if (RWDisc.dvRAM.ReadDataMode == ePRE_BUFFERING)
        {
            RWDisc.dvRAM.StartBufferingOK = TRUE;
            /* Register new decoded Segment for DVD-RAM pre-buffering */
            DVDRAMAppendPreBuffer();
        }
        else if (RWDisc.dvRAM.ReadDataMode == eREAD_ERROR_RATE)
        {
            BYTE Zone;

            send_msg84L(SEND_MRW_MSG,0x580708,RWDisc.dvRAM.CurrentPID);// "CurPSN-1"
            Zone = DVDRAM_PSN2ZONE(RWDisc.dvRAM.CurrentPID);
            if (RWDisc.dvRAM.CurrentPID >= (DVDRAMZoneTable[Zone].EndUserSectorPSN + 1)) //we are reaching to Guard Area.
            {
                extern ULONG ANEW_SEEK_Target;
                ULONG  NextZoneStartPSN;

                NextZoneStartPSN = DVDRAMZoneTable[Zone + 1].StartUserSectorPSN;
                dvdLastDecodedPSN = NextZoneStartPSN - 1;    //update read error retry seek PSN
                ANEW_SEEK_Target =  NextZoneStartPSN;

                send_msg80(SEND_MRW_MSG,0x580709);// "Zone Cross"
                dvdStopDiscTransfer();
                ANEW_SEEK = TRUE;
            }
        }
    }
#endif

    /*--------------------------------------------------------------
        Adjust the SpeedCount and SpeedUpReady.
    ---------------------------------------------------------------*/
    if(SpeedCount < SPEED_UP_COUNT_LIMIT- ONE_DVD_BLOCK)
    {
    #if (EN_DVD_HIGH_JITTER_DETECT)
            if( (SpeedCount >= 0)&&((HIGH_JITTER_DISC&0x03)!=0x03) )
    #else
        if( SpeedCount >= 0 )
    #endif
            SpeedCount += ONE_DVD_BLOCK;
        else
            SpeedCount = ONE_DVD_BLOCK;
    }

    if(SpeedUpReady > DVD_SPEED_UP_READY)
        SpeedUpReady--;

    /*---------------------------------------------------------------
        Reset DVD ECC Error Count
    ----------------------------------------------------------------*/

#if (DVD_READ_AHEAD_ENABLED)
    /*---------------------------------------------------------------
        Check if all DVD blocks to satisfy the host request have
        been buffered. If so, indicate that pre-fetching has started
    ----------------------------------------------------------------*/
        if (dvdReadAheadStartPSN == (dvdLastDecodedPSN + 1))
        {
            dvdDecFlag.fREAD_AHEAD = TRUE;
        }
#endif // DVD_READ_AHEAD_ENABLED

    /*---------------------------------------------------------------
        Check if all blocks programmed in BKTOBUF have been
        transferred and corrected.
    ----------------------------------------------------------------*/
    if( dvdDecIntFlag.fECC_DONE_INT )
    {
        /*---------------------------------------------------------------
            Disable DVD interrupts and clear interrupts and enable flags
        ---------------------------------------------------------------*/
        DISABLE_DVD_DECODER_INTS();
        DISABLE_DVD_SYNC_LOST_INT();    /* Disable sync lost interrupts     */

        dvdDecFlag.fSYNC_INTERRUPT_ENABLED = FALSE;

        STOP_DVD_BLOCK_TIMER();
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
        HAL_DFE_DisableICEDetect();
        HAL_DFE_ClusterStateDisableInt();
        RWDisc.dvRAM.ContinuteIceCount = 0;
    #endif

        /*---------------------------------------------------------------
            When a lead in block is buffered mark transfer complete
            and clear the data buffer sector count
        ---------------------------------------------------------------*/
        if (dvdDecFlag.fREADING_LEADIN == TRUE)
        {
            {
                dvdDiscRequest.State    = TRANSFER_COMPLETE;    /* Transfer is complete     */
                Svr_pause_reqire        = TRUE;                 /* set system for pause     */
                Iram_Svr_player_mode    = PAUSING_MODE;
                DataBuffer.SectorCount  = 0;                    /* Clear buffer count       */
                return;
            }
        }

        /*---------------------------------------------------------------
            If the transfer request is not complete set state to WAIT
        ----------------------------------------------------------------*/
        if( dvdDiscRequest.EccBlocksBuffered < dvdDiscRequest.EccBlockCount)
            dvdDiscRequest.State = WAITING_FOR_TRANSFER;

#if (DVD_READ_AHEAD_ENABLED)
        /*---------------------------------------------------------------
            If buffering completed during pre-fetching indicate read
            ahead is now off
        ----------------------------------------------------------------*/
            dvdDecFlag.fREAD_AHEAD = FALSE;
#endif
    }

#if (DVD_RAM_READ == 1)
    if((pdbGetDVDType() == DVD_RAM) && (RWDisc.dvRAM.ReadingDMA == FALSE))
    {
        /* In DVDRAM_NEW_CACHE, we don't use dvdDiscRequest info, and Never SendHostBlock in ISR. */
        /* All we need to do is update CacheEndLBA */
        if (DataBuffer.CacheEndLBA == 0L)
            DataBuffer.CacheEndLBA = 0x0FL;
        else
            DataBuffer.CacheEndLBA += ONE_DVD_BLOCK;

        if((dvdDecIntFlag.fECC_DONE_INT) && (RWDisc.dvRAM.ReadDataMode == ePRE_BUFFERING))
        {
            Svr_pause_reqire        = TRUE;                 /* set system for pause     */
        }
        return;
    }
#endif

    /*---------------------------------------------------------------
        Check if all requested blocks have been transferred
    ----------------------------------------------------------------*/
    if( dvdDiscRequest.EccBlocksBuffered == dvdDiscRequest.EccBlockCount)
    {
        dvdDiscRequest.State    = TRANSFER_COMPLETE;    /* Transfer is complete     */
#if (E_DIAGS == 1)
        if(READ_PI_PO == 0)
        {
            Svr_pause_reqire        = TRUE;                 /* set system for pause     */
            Iram_Svr_player_mode    = PAUSING_MODE;
        }
#endif
    }

    /*---------------------------------------------------------------
        Reset retry count
    ----------------------------------------------------------------*/
   if (dvdDecFlag.fREAD_AHEAD == FALSE)
    {
         dvdReadRetryCount = MAX_DVD_READ_RETRIES;
    }
    /*---------------------------------------------------------------
        For read long command set the defined sector count
    ----------------------------------------------------------------*/
    if(dvdDecFlag.fREAD_LONG_COMMAND == TRUE)
        DataBuffer.SectorCount = DVD_SECTORS_PER_READ_LONG_BLOCK;

    /*---------------------------------------------------------------
        Check to see if a host transfer is needed. If this is a
        read lead in block there is no need for a host transfer
    ----------------------------------------------------------------*/
    if (dvdDecFlag.fREADING_LEADIN == FALSE)
        dvdCheckForHostTransfer();

}

/*******************************************************************************

    FUNCTION        dvdCheckForHostTransfer

    DESCRIPTION     Updates the host side of the buffer and determines if a
                    host transfer should be initiated

********************************************************************************/
void dvdCheckForHostTransfer()
{
    ULONG OffsetAdjustment; /* Adjustment of host pointer into first block */

    /*---------------------------------------------------------------
        Update last lba in the cache
    ---------------------------------------------------------------*/
    if (DataBuffer.CacheEndLBA == 0L)
        DataBuffer.CacheEndLBA = 0x0FL;
    else
        DataBuffer.CacheEndLBA += ONE_DVD_BLOCK;

    /*---------------------------------------------------------------
        Exit if host is not waiting for a transfer to be initiated.
    ---------------------------------------------------------------*/
    if( HostXfer.State != WAITING_FOR_TRANSFER)
        return;

    /*---------------------------------------------------------------
        Intitiate a host transfer
    ---------------------------------------------------------------*/

    /*-------------------------------------------------------------------
        If this is the first ecc block loaded (on cache miss only):

            1. Clear first block flag
            2. Reset the error count for speed error recovery
            3. Adjust the current host pointer to the starting LBA
               within this first block and adjust SectorCount.

    --------------------------------------------------------------------*/
    if ( dvdDecFlag.fFIRST_HOST_BLOCK )
    {
        dvdDecFlag.fFIRST_HOST_BLOCK = FALSE;

        OffsetAdjustment = DataBuffer.CacheStartLBA - dvdGetBlockAddress(DataBuffer.CacheStartLBA);
        DataBuffer.CurHostPtr += OffsetAdjustment;
        DataBuffer.SectorCount -= OffsetAdjustment;
    }

    //send_msg54(SEND_MRW_MSG,'S','n','d','1');
    SendHostBlocks();
}


#if (DVD_VERIFY_ID == 1)
/*******************************************************************************

    FUNCTION        dvdVerifyID

    DESCRIPTION     Verifies the actual ID with target

********************************************************************************/
BYTE dvdVerifyID()
{
    ULONG TargetID;
    ULONG ActualID;
    ULONG *AuxPtr;

    TargetID = dvdLastDecodedPSN + 1;

    AuxPtr = (ULONG *) (CUR_AUX_ADDRESS());

    ActualID = *AuxPtr;
    ActualID = SWAPEND_ULONG(ActualID) & 0x00FFFFFF;

    if (TargetID == ActualID)
        return TRUE;
    else
        return FALSE;
}
#endif /* DVD_VERIFY_ID */

