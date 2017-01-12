/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   SVO_IM.C
*
* DESCRIPTION
*
*   $Revision: 198 $
*   $Date: 11/02/22 3:06p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\servo\svoMacro.h"
#include ".\servo\svo_step.h"
#include ".\servo\svo_isr.h"
#include ".\servo\svo.h"
#include ".\common\error.h"
#include ".\player\plrdb.h"
#include ".\servo\svo_com.h"
#include ".\servo\svovar.h"
#include ".\common\commath.h"
#include ".\common\stdlib.h"
#include ".\servo\isr.h"
#include ".\servo\svo_lib.h"
#if (SRC_RESET_DC_LPF == 1)
#include ".\AL\HAL_dsp.h"
extern BYTE  SRC_need_ON;
#endif //(SRC_RESET_DC_LPF == 1)

/***** Prototypes *****/
void SetSCRegField_1B(ULONG addr, BYTE mask, BYTE start_bit, BYTE field_val);
BYTE GetSCRegField_1B(USHORT addr, BYTE mask, BYTE start_bit);

#if (ENABLE_FIQ == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoSInt_FIQ
 * Description: servo FIQ interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void    svoSInt_FIQ(void)
{
// The FIQ is not currently used. It has been reserved for future Work Arounds when a fast high priority is required!
}
#endif

#if DO_HW_SUPPORT_STEP == 0
/*-----------------------------------------------------------------------------
 *
 * Module:
 * Description:
 *
 *----------------------------------------------------------------------------*/
void Timer5Isr(void)
{
    CLR_TIMER_STATUS(TIMER5);

    svoSTEPInt();
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoSTEPInt()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoSTEPInt( void )
{
    SBYTE stp_sled;
    SBYTE thresh_hold_pos, thresh_hold_neg;
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    BYTE  step_size = 1;
#endif

    TIMER5.TSTATUS.BIT.TINTF = 0;

    switch(Iram_stp_TimerMode)
    {
        case STEP_ROUGH_TIMER:
#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
        case STEP_ROUGH_TIMER_READ:
#endif
            SetTimerXCompareValue(TIMER5,0xFFFF);
            svoStp_JumpControl();
            break;

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
        case STEP_FINE_TIMER:
#endif
        case STEP_READ_TIMER:
            if (svoStp_busy())
            {
                //prevent STEP following IRQ occurs in the very beginning of a sled jump
                return;
            }

#if (TRO_INVERSION == 1)
            stp_sled = READ_FIELD(HAL_SRV_SLED);
#else
            stp_sled = -READ_FIELD(HAL_SRV_SLED);
#endif

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            if (Iram_stp_TimerMode == STEP_FINE_TIMER)
            {
                //----------------------------------------------------------;
                // Iram_stp_TimerMode is STEP_FINE_TIMER                    ;
                //----------------------------------------------------------;

                if (!FokStatusOK())
                    return;

                //-----------------------------------------------------------------;
                // In FINE mode, do stepping only if sled is outside window        ;
                //-----------------------------------------------------------------;
                if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
                {
                    thresh_hold_pos = SEEK_THRESHOLD_POS_DVD;
                    thresh_hold_neg = SEEK_THRESHOLD_NEG_DVD;
                    if ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW))
                    {
                        thresh_hold_pos = SEEK_THRESHOLD_POS_DVDR;
                        thresh_hold_neg = SEEK_THRESHOLD_NEG_DVDR;
                    }
    #if (DVD_RAM_READ == 1)
                    else if (DVDRAM)
                    {
                        thresh_hold_pos = SEEK_THRESHOLD_POS_DVDRAM;
                        thresh_hold_neg = SEEK_THRESHOLD_NEG_DVDRAM;
                    }
    #endif
                }
                else
                {
                    thresh_hold_pos = SEEK_THRESHOLD_POS_CD;
                    thresh_hold_neg = SEEK_THRESHOLD_NEG_CD;
                }
                step_size = 4;
            }
            else
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            {
                //----------------------------------------------------------;
                // Iram_stp_TimerMode is STEP_READ_TIMER                    ;
                //----------------------------------------------------------;
                if (!FokStatusOK())
                {
                    svoStp_HoldSledge();
                    return;
                }

                /* If the absolute magnitude of SLO exceed SLED_THRESHOLD_MAX many times continuously, the protection flow will start.*/
                if (ABS(stp_sled) > SLED_THRESHOLD_MAX)
                {
                    svoStepVar.SledCounter++;
#if (SRC_RESET_DC_LPF == 1)
    #if (SRC_ENABLE == 1)
                    if ((svoSpeedVar.SpdSRCEnable == TRUE)&&(SRC_need_ON==0xff)&&(svoStepVar.SledCounter>5))
                    {
                        //send_msg5S(1,"==SRC rst==");
                        HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_OFF);
                        HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET);
                        SRC_need_ON=1;
                    }
    #endif //(SRC_ENABLE == 1)
#endif //(SRC_RESET_DC_LPF == 1)

                    if (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD)
                    {
                        svoStp_HoldSledge();
                        return;
                    }
                }
                else
                {
#if (SRC_RESET_DC_LPF == 1)
    #if (SRC_ENABLE == 1)
                    if ((svoSpeedVar.SpdSRCEnable == TRUE)&&(SRC_need_ON==1)&&(ABS(stp_sled) < SLED_THRESHOLD_MAX/3))
                    {
                        //send_msg5S(1,"==SRC can ON==");
                        SRC_need_ON=2;
                    }
    #endif //(SRC_ENABLE == 1)
#endif// (SRC_RESET_DC_LPF == 1)

                    if (svoStepVar.SledCounter > 0)
                        svoStepVar.SledCounter--;
                }

                //-----------------------------------------------------------------;
                // In READ , do stepping only if sled is outside the window        ;
                //-----------------------------------------------------------------;
                if (++svoStepVar.StepThresholdDelay >= svoStepVar.StepThresholdDelayTarget)
                {
                    svoStepVar.StepThresholdDelay = 0;
                    svoStepVar.StepThresholdNeg += (SHORT)((float)SLED_THRESHOLD_INC_DEC / SLO_SENSITIVITY + 0.5);
                    if (svoStepVar.StepThresholdNeg > svoStepVar.StepThresholdNegTarget)
                    {
                        svoStepVar.StepThresholdNeg = svoStepVar.StepThresholdNegTarget;
                    }
                    svoStepVar.StepThresholdPos -= (SHORT)((float)SLED_THRESHOLD_INC_DEC / SLO_SENSITIVITY + 0.5);

                    if (svoStepVar.StepThresholdPos < svoStepVar.StepThresholdPosTarget)
                    {
                        svoStepVar.StepThresholdPos = svoStepVar.StepThresholdPosTarget;
                    }
                }
                thresh_hold_pos = svoStepVar.StepThresholdPos;
                thresh_hold_neg = svoStepVar.StepThresholdNeg;
            }

            if (stp_sled > thresh_hold_pos)
            {
                svoStepVar.StepThresholdPos = stp_sled + (SHORT)((float)SLED_THRESHOLD_INC_DEC / SLO_SENSITIVITY + 0.5);
                if (svoStepVar.StepThresholdPos > 127)
                {
                    svoStepVar.StepThresholdPos = 127;
                }
                svoStepVar.StepThresholdDelay = svoStepVar.StepThresholdDelayTarget - SLED_MIN_FOLLOW_DELAY;
                if (Iram_stp_CurrentPosition < UM_2_SLED_STEP(MAX_SLED_FOLLOW_RADIUS))
                {
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
                    svoStp_MoveStep(step_size);
#else
                    svoStp_MoveStep((stp_sled - svoStepVar.StepThresholdPosTarget) / (LONG)SLED_STEP_2_UM(3 / SLO_SENSITIVITY) + 1);
#endif
                    return;
                }
                else
                {
                    svoStepVar.SledCounter = TRACK_SLIP_DETECT_THRESHOLD;
                }
            }

            if (stp_sled < thresh_hold_neg)
            {
                svoStepVar.StepThresholdNeg = stp_sled - (SHORT)((float)SLED_THRESHOLD_INC_DEC / SLO_SENSITIVITY + 0.5);
#if (TRO_INVERSION == 1)
                if (svoStepVar.StepThresholdNeg < -128)
                {
                    svoStepVar.StepThresholdNeg = -128;
                }
#else
                if (svoStepVar.StepThresholdNeg < -127)
                {
                    svoStepVar.StepThresholdNeg = -127;
                }
#endif
                svoStepVar.StepThresholdDelay = svoStepVar.StepThresholdDelayTarget - SLED_MIN_FOLLOW_DELAY;

                if (Iram_stp_CurrentPosition > UM_2_SLED_STEP(MIN_SLED_FOLLOW_RADIUS))
                {
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
                    svoStp_MoveStep(-step_size);
#else
                    svoStp_MoveStep((stp_sled - svoStepVar.StepThresholdNegTarget) / (LONG)SLED_STEP_2_UM(3 / SLO_SENSITIVITY) - 1);
#endif
                    return;
                }
                else
                {
                    svoStepVar.SledCounter = TRACK_SLIP_DETECT_THRESHOLD;
                }
            }

            if ((svoStepVar.StepThresholdDelay == (svoStepVar.StepThresholdDelayTarget - (SLED_MIN_FOLLOW_DELAY - 1))) || (svoStepVar.StepThresholdDelay == 0))
            {
                svoStp_HoldSledge();
            }
            break;

        default:
            break;
    }
}//end of svoSTEPInt

#endif // DO_HW_SUPPORT_STEP == 0


#if DO_HW_SUPPORT_STEP == 1
//*****************************************************************************
//                                                                            *
//  TBD!                                                                      *
//*****************************************************************************
void hw_step_motor_normal_play(void)
{
    BYTE stp_pos;

    WRITE_REG(HAL_SRVREG_MAP->STPDCO,0x00);     // select 8 x 1378 Hz DCO clk for speed
    FillMicroSteppingTable(200);                 //   (unit 2^-8)

    WRITE_REG(HAL_SRVREG_MAP->STPSIZE, 0x00);                // set step size to 1
    stp_pos  = READ_REG(HAL_SRVREG_MAP->STPPOS);
/***** TBD? Why is this register written Twice with the same value? *****/
/*****  Is this a work around for a chip issue? *****/
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (HAL_SRV_INTCLR + stp_pos));    // goes to internal control mode
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (HAL_SRV_INTCLR + stp_pos));    // goes to internal control mode
/***** TBD? Why is this register written Twice with the same value? *****/
}// end of hw_step_motor_normal_play

/*********************************************************
* Module: hw_step_motor_fine_seek()
*
* Description:  TBD!
*
* Returns: void
* Params:  void
**********************************************************/
void hw_step_motor_fine_seek(void)
{
    BYTE stp_pos;

    WRITE_REG(HAL_SRVREG_MAP->STPDCO,  halSRV_STPDCOCK_1378_X_16);  // select 16 x 1378 Hz DCO clk for speed
    FillMicroSteppingTable(250);                    // (unit 2^-8)
    WRITE_REG(HAL_SRVREG_MAP->STPSIZE, 0x00);                 // set step size to 1
    stp_pos  = READ_REG(HAL_SRVREG_MAP->STPPOS);
/***** TBD? Why is this register written Twice with the same value? *****/
/*****  Is this a work around for a chip issue? *****/
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (HAL_SRV_INTCLR + stp_pos));     // goes to internal control mode
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (HAL_SRV_INTCLR + stp_pos));     // goes to internal control mode
/***** TBD? Why is this register written Twice with the same value? *****/

}// end of hw_step_motor_normal_play
#endif //DO_HW_SUPPORT_STEP

/*********************************************************
* Module: getsubQcode()
*
* Description: TBD!
*
* Returns: void
* Params:  subqdata
**********************************************************/
void getsubQcode(BYTE *subqdata)
{
    BYTE    i;

    (ULONG) * subqdata = READ_ADR_4B(HAL_CDD_SUBQ0_ADR);
    (ULONG) * (subqdata + 4) = READ_ADR_4B(HAL_CDD_SUBQ4_ADR);
    (WORD) * (subqdata + 8) = READ_ADR_2B(HAL_CDD_SUBQ8_ADR);

    i = subqdata[0];
    ((SubQCodeS *)subqdata)->Addr = i & 0x0F;
    ((SubQCodeS *)subqdata)->Con  = i >> 4;

    pdbSetSubCode((SubQCodeS *)subqdata);
}

/*********************************************************
* Module: subQmode1()
*
* Description: TBD!
*
* Returns: void
* Params:  subqdata
**********************************************************/
void subQmode1(SubQCodeS *subqdata)
{
    BYTE    check_minute;

    if((pdbGetCDRW()) &&
      (subqdata->Minute == 0xFF) &&
      (subqdata->Second == 0xFF) &&
      (subqdata->Frame  == 0xFF)){
        check_minute = FALSE;
    }else{
        check_minute = TRUE;
    }

    if(check_minute == TRUE){
        if(CheckBCDTime((TimeS *)&(subqdata->Minute)) == FALSE){
            pdbSetSubCodeOK(FALSE);
            return;
        }
    }
    if((subqdata->Point & 0xF0) != 0xA0){
        if(CheckBCDTime((TimeS *)&(subqdata->PMinute)) == FALSE){
            pdbSetSubCodeOK(FALSE);
            return;
        }
    }
    pdbSetSubQCode(subqdata);
    svoIramVar.subQReadyFlag = TRUE;
    if(svoIramVar.subQEnableFlag == 0){
        WRITE_FIELD(HAL_CDD_SCOREN, 0);     /* Disable SUB-Q interrupt */
    }

}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
