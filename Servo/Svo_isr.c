/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   SVO_ISR.C
*
* DESCRIPTION
*
*   $Revision: 240 $
*   $Date: 11/04/01 9:33a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\servo\svo.h"
#include ".\servo\svoMacro.h"
#include ".\common\error.h"
#include ".\hif\gp_ram.h"
#include ".\player\plrdb.h"
#include ".\player\plrinfo.h"
#include ".\common\comMath.h"
#include ".\common\system.h"
#include ".\servo\svo_com.h"
#include ".\servo\svomath.h"
#include ".\servo\svo_lib.h"
#include ".\common\sdram.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\isr.h"
#include ".\servo\svo_isr.h"
#include ".\servo\svo_dsp.h"
#include ".\al\REG_WOBBLE.h"
#include ".\al\HAL_wobble.h"
#include ".\al\HAL_servo.h"
#include ".\al\REG_dfe.h"
#include ".\al\coefs.h"
#include ".\servo\svo_step.h"
#if (XDFCT_ENABLE == 1)
#include ".\AL\LAL_dsp.h"
#endif //(XDFCT_ENABLE == 1)

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#endif
#if (EN_ADI == 1)
#include ".\AL\HAL_dsp.h"
#endif

extern BYTE Send_timef;

#if (USING_C1 == 1)
extern void C1ErrorIntProc();
#endif


/*-----------------------------------------------------------------------------
 *
 * Module: svoSEEKIsr
 * Description: servo read SEEKC interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void svoSEEKIsr(void)
{
    WRITE_FIELD(HAL_SRV_FAKERX, 0);
    WRITE_FIELD(HAL_SRV_FORBLK, 0);

    svoIramVar.fineSeekDoneFlag = TRUE;

    if (READ_FIELD(HAL_SRV_HYS2) == TRUE)
    {
        WRITE_FIELD(HAL_AFE_RXTOK, 0); //Enable XTOK mute RX to prevent wrong hysteresis action which is caused by wrong RX.
    }

    CLEAR_INT_FIELD(HAL_SRV_SEEKC, 0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoDefectIsr
 * Description: servo Defect Length interrupt service routine
 *
 *----------------------------------------------------------------------------*/
#if DEFECT_LENGTH_MEASURE == 1
    BYTE    CD_Scratch_Disc;
    #if (HORROR_DISC_IMPROVEMENT == 1)
extern BYTE    FixC2NOC1FLG;
extern BYTE Kp,Ki,TryDFE_Param;
    #endif
#endif
#define DIAG_VENDOR_READ                0xF4    /* Vendor Read Commands */
#define VELOCITY_CONSTANT_DEF_P1  UM_2_SLED_STEP(2021395)      // ((33868800/16)*60)/(2*pi*10)
void svoDefectIsr(void)
{

#if (DEFECT_LENGTH_MEASURE == 1)
    USHORT cur_length;

    cur_length = (ULONG)READ_FIELD(HAL_SRV_DLCNT) * (svoSpeedVar.CurrentRpm * Iram_stp_CurrentPosition / 10) / VELOCITY_CONSTANT_DEF_P1;
    if (cur_length > svoDfMeasVar.temp_length)
{
        svoDfMeasVar.temp_length = cur_length;
    }
    svoDfMeasVar.temp_count++;
    //send_msg5SValue(1,"defect_length", 2, cur_length);
#endif


#if DEFECT_LENGTH_MEASURE == 1

    if(((svoIramVar.svoInState & TRN_MASK) == SVO_PLAY)||((svoIramVar.svoInState & TRN_MASK) == TRN_056))
    {
#if((PCB == 710)||(PCB == 815)||(PCB == 711)||(PCB == 817))

        if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
        {

                if(cur_length >= ALLOW_DEFECT_LENGTH)
                {

                    if(Iram_Cmd_block.bf[0]== DIAG_VENDOR_READ)//DIAG_VENDOR_READ command
                        WRITE_FIELD(HAL_CDD_C1CG1,0);

                    CD_Scratch_Disc=1;
                    //libDisableDFCTInt();
    #if (HORROR_DISC_IMPROVEMENT == 1)
                    WRITE_FIELD(HAL_DFE_OFFSETBND, 0x02);   //set AOC bound
                    WRITE_FIELD(HAL_CDD_C2NOC1FLG,0);
                    FixC2NOC1FLG = 0;
                    if( READ_FIELD(HAL_DFE_FPLLKICFG)<4 )//For CD_Scratch_Disc == 1
                    {
                        Kp = READ_FIELD(HAL_DFE_FPLLKPCFG);
                        Ki = 4;
                        WRITE_FIELD(HAL_DFE_FPLLKPCFG,Kp);
                        WRITE_FIELD(HAL_DFE_FPLLKICFG,Ki);
                        TryDFE_Param |= 0x01;
                    }
    #endif
                }

        }                                          ////== end == modify for TVD-621  // ==end== modify for SCD-2383
#endif

    }

//    if (((svoIramVar.svoInState & TRN_MASK) < READY_TRN) || (!FokStatusOK()))
//    {
//       libDisableDFCTInt();
//    }
#endif
   // CLEAR_INT_FIELD(HAL_SRV_DLINTR, 0);
}

#if ((DETECT_FE_DISTURB==1)|| (DETECT_TE_OFFTRACK==1)||DETECT_DVD_FE_DISTURB)
/*-----------------------------------------------------------------------------
 *
 * Module: svoFSHCIsr
 * Description: servo read FSHC interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void svoFSHCIsr(void)
{
    #if DEBUG_FE_DISTURB
    LONG packet_size;
    #endif // DEBUG_FE_DISTURB
    SendMsg80(DEBUG_DVD_WRITE_MSG,0x33FF49);//svoFSHCIsr
    #if DETECT_DVD_FE_DISTURB
    if (pdbDiscLoaded() == DVD_DISC)            /* commands for DVD Disc */
    {
        if(READ_FIELD(HAL_WS_WGATEMON))
        {
            Iram_RecordState = eRECORD_UNDERRUN;
            RequestSpeedDown = TRUE+1;
            SendMsg80(DEBUG_DVD_WRITE_MSG,0x33FF49);//svoFSHCIsr
        }
    }
    else
    #endif // DETECT_DVD_FE_DISTURB
    {
    #if DETECT_FE_DISTURB
        #if DEBUG_FE_DISTURB
        if(Disc_Speed.Write2 == e24XCAV) //for 24XCAV
        {
            WRITE_FIELD(HAL_SRV_INTRACT, 0);      // disable focus shock
            WRITE_FIELD(HAL_SRV_WINDOW, 0);       // disable focus shock
            WRITE_FIELD(HAL_SRV_FSHCEN, 0);       /* Disable FE disturbance */
            WRITE_FIELD(HAL_SRV_INTRACT, 1);      // enable FE disturbance window
            WRITE_FIELD(HAL_SRV_WINDOW, 1);       // enable FE disturbance window
            WRITE_FIELD(HAL_SRV_FSHCEN, 1);       /* Enable FE disturbance */
            return;
        }

        SendMsgCn(DEBUG_CD_WRITE_MSG,1,0x374000,A1B(Disc_Speed.Write2));

        if((ReadSCReg_1B(rCDEncMon) & mskEncMode) == bEncMode_WrNoPCA){   /* Encoding */
            if(Record.Data == eHostData){
                Iram_RecordState = eRECORD_UNDERRUN;
            }
            if((GetWriteMethod() == FPKT) && (Record.Data == eHostData)){
                packet_size = pdbGetTrackPacketSize(Record.tno);
                Iram_Record_pause_lba -= ((Iram_Record_pause_lba - Iram_CurrEncodeLBA) / (packet_size + 7L) -1L) * (packet_size + 7L);
            }else{
                ExacWritePause();
            }
        }else{
            WRITE_FIELD(HAL_CDE_STASTOP, halCDE_ManualStop);            /* Manual Stop CD Encoder */
        }
        WRITE_FIELD(HAL_SRV_FSHCEN, 0);      /* Disable FE disturbance */
        WRITE_FIELD(HAL_SRV_INTRACT, 0);  // disable focus shock
        WRITE_FIELD(HAL_SRV_WINDOW, 0);   // disable focus shock

        if(GetSpeedValue(Disc_Speed.Write2) >= GetSpeedValue(e24X)){
            //If Write occur TE/FE error, speed down use PCAV write
            Disc_Speed.Write2 = Disc_Speed.WriteMarginal = GetLowerDriveWriteSpeed(Disc_Speed.Write2);         /* Set write speed */

            WrtChangeSPWaitTimer = 800;
            RequestSpinDown = TRUE;
        }
        #else // DEBUG_FE_DISTURB
        if((ReadSCReg_1B(rCDEncMon) & mskEncMode) != bEncMode_WrNoPCA){   /* Encoding */
            WRITE_FIELD(HAL_CDE_STASTOP, halCDE_ManualStop);            /* Manual Stop CD Encoder */
            WRITE_FIELD(HAL_SRV_FSHCEN, 0);      /* Disable FE disturbance */
            WRITE_FIELD(HAL_SRV_INTRACT, 0);  // disable focus shock
            WRITE_FIELD(HAL_SRV_WINDOW, 0);   // disable focus shock
            if(GetSpeedValue(Disc_Speed.Write2) >= GetSpeedValue(e40XCAV)){
                Disc_Speed.Write2 = Disc_Speed.WriteMarginal = e32XCAV;                             /* Set write speed */
                RequestSpinDown = TRUE;
            }
        }

        #endif // DEBUG_FE_DISTURB
    #endif // DETECT_FE_DISTURB
    }
}
#endif // ((DETECT_FE_DISTURB==1)|| (DETECT_TE_OFFTRACK==1)||DETECT_DVD_FE_DISTURB)

#if (DETECT_TE_OFFTRACK||DVD_OFF_TRACK_TRIGGER)
/*-----------------------------------------------------------------------------
 *
 * Module: svoMIRRIsr
 * Description: servo TRACK OFF interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void svoMIRRIsr(void)
{
    #if DETECT_TE_OFFTRACK
    LONG packet_size;
    #endif
    #if DVD_OFF_TRACK_TRIGGER
    if (pdbDiscLoaded() == DVD_DISC)            /* commands for DVD Disc */
    {
        SendMsg80(DEBUG_DVD_WRITE_MSG,0x33FF4A);//svoMIRRIsr
        if(READ_FIELD(HAL_WS_WGATEMON))
        {
            Iram_RecordState = eRECORD_UNDERRUN;
            RequestSpeedDown = TRUE+2;
            SendMsg80(DEBUG_DVD_WRITE_MSG,0x33FF4A);//svoMIRRIsr
        }
        WRITE_FIELD(HAL_SRV_MIRREN, 0);      /* Disable Off Track */
    }
    else
    {
    #endif
    #if DETECT_TE_OFFTRACK
        if((READ_FIELD(HAL_SRV_SEEKCEN)||(Iram_RecordState != eRECORDING)) //Seek process just return
        {
            return;
        }

        if(Disc_Speed.Write2 == e24XCAV)//for 24XCAV
            || (Off_Track_Counter++ <= OFF_TRACK_COUNTER_THRESHOLD))
        {
            //send_msg81(DEBUG_CD_WRITE_MSG,0x574011,Off_Track_Counter);
            CLEAR_INT_FIELD(HAL_SRV_MIRR, 0);   /* Clear off track interrupt */
            WRITE_FIELD(HAL_SRV_MIRREN, 1);      /* Enable MIRR disturbance */
            return;
        }

        SendMsgCn(DEBUG_CD_WRITE_MSG,1,0x374001,A1B(Disc_Speed.Write2));

        if((ReadSCReg_1B(rCDEncMon) & mskEncMode) == bEncMode_WrNoPCA){   /* Encoding */
            if(Record.Data == eHostData){
                Iram_RecordState = eRECORD_UNDERRUN;
            }
            if((GetWriteMethod() == FPKT) && (Record.Data == eHostData)){
                packet_size = pdbGetTrackPacketSize(Record.tno);
                Iram_Record_pause_lba -= ((Iram_Record_pause_lba - Iram_CurrEncodeLBA) / (packet_size + 7L) -1L) * (packet_size + 7L);
            }else{
                ExacWritePause();
            }
        }else{
            WRITE_FIELD(HAL_CDE_STASTOP, halCDE_ManualStop);            /* Manual Stop CD Encoder */
        }
        WRITE_FIELD(HAL_SRV_MIRREN, 0);      /* Disable Off Track */
        if((Disc_Speed.Write2&eSpeedValue) >= e24X){
            //If Write occur TE/FE error, speed down use PCAV write
            Disc_Speed.Write2 = Disc_Speed.WriteMarginal = GetLowerDriveWriteSpeed(Disc_Speed.Write2);        /* Spin Down */

            WrtChangeSPWaitTimer = 800;
            RequestSpinDown = TRUE;
        }
    #endif // DETECT_TE_OFFTRACK
    #if DVD_OFF_TRACK_TRIGGER
    }
    #endif // DVD_OFF_TRACK_TRIGGER
} /* svoMIRRIsr */
#endif //  (DETECT_TE_OFFTRACK||DVD_OFF_TRACK_TRIGGER)

#if (NEW_FDP_DETECTION != 0)
/*-----------------------------------------------------------------------------
 *
 * Module: svoFDROPIsr
 * Description: servo Focus drop interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void svoFDropIsr(void)
{
    WRITE_FIELD(HAL_SRV_FNOKEN,0); //turn off Focus drop interrupt, will be enabled in focus_on_chk()
#if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6EN,0);     //prevent duplicate svoFDropIsr()
    WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
#endif

    #if (CHIP_REV < 0xC0)
    HAL_SERVO_SetFoos(FDP_FOO_OS);
    #else
    HAL_SERVO_SetProtFoos(svoPrvtVar_ProtFoos);
    #if (SCRATCH_PROT_TIME > 0)
    WRITE_DSP_COEF(FEDG_thr, svoPrvtVar_FEDG_thr / 2);
    #endif
    WRITE_DSP_COEF(kf18,FOCUS_SEARCH_KF18);
    HAL_SERVO_SetFcs(READ_FIELD(HAL_SRV_DROPFOO) * 0x8000 / FOCUS_SEARCH_KF18);
    WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
    WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
    HAL_SERVO_SetFocDn(0);
    HAL_SERVO_SetFocStp();
    #endif

    #if (NEW_FDP_DETECTION == 2)
    WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
    #endif

    #if (FOOFFSET2_ENABLE == 1)//bug fixed: this action must be done after LDOFF/ON when FOOFFSET2_ENABLE == 1
    WRITE_DSP_COEF(kf18,0);
    HAL_SERVO_SetFocDn(0);
    #endif

    svoIramVar.focusDropIsrFlag = TRUE;

    SendMsg80(SHOW_DEBUG_MSG,0x33FF35); //FDopIsr

#if (XDFCT_ENABLE == 1)
    LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
#endif //(XDFCT_ENABLE == 1)

    #if (DVD_RAM_READ == 1) && (CHIP_REV >= 0xC0) && ((DVDRAM_FREEZE_FLYWHEEL == 1) || (DVDRAM_FREEZE_FLYWHEEL == 2))
    if (DVDRAM_rewritable && (land_flywheel_state == LAND_FLYWHEEL_FREEZE))
    {
        land_flywheel_state = LAND_FLYWHEEL_HUNT;
        WRITE_FIELD(HAL_DVDD_TRACKCTRL, 0);
        WRITE_FIELD(HAL_WBL_HDRFLYRD, 0);
    }
    #endif
}
#endif
/*-----------------------------------------------------------------------------
 * Module: svoSInt
 * Description: servo interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void    svoSInt(void)
{
    ULONG  data0, data1;
#if (EN_WOBBLE == 1)
    ULONG  IntStatus;
#endif // (EN_WOBBLE == 1)

    //SDINT0
    data0 = READ_REG(HAL_SRVREG_MAP->SDINT);
    data1 = data0 & READ_REG(HAL_SRVREG_MAP->SDINTEN);
    if(data1)
    {
        if (data1 & HAL_SRV_SEEKC) svoSEEKIsr();

#if (DETECT_FE_DISTURB||DETECT_DVD_FE_DISTURB)
        if (data1 & HAL_SRV_FSHC) svoFSHCIsr();
#endif

#if (DETECT_TE_OFFTRACK||DVD_OFF_TRACK_TRIGGER)
        if (data1 & HAL_SRV_MIRR) svoMIRRIsr();
#endif

#if DEFECT_LENGTH_MEASURE == 1
        if (data1 & HAL_SRV_DLINTR)
        {
            svoDefectIsr();
        }

#endif

#if (NEW_FDP_DETECTION != 0)
        if (data1 & HAL_SRV_FNOK) svoFDropIsr();
#endif
    }
    WRITE_REG(HAL_SRVREG_MAP->SDINT, (~data0));

#if ((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))

    #if (EN_POWER_CONTROL == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) && (!WobbleCLKStop))
    #else
    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    #endif
    {
        //CDWBLINTE
        // check the CD WOBBLE INTERRUPT REGISTER
        IntStatus = HAL_WOBBLE_GetCdWblIntReg();

        if (IntStatus)
        {
            if ((IntStatus & HAL_WBL_ATNEWADI) == HAL_WBL_ATNEWADI)
            {
                ATIPMSFReadyIntProc();
            }
        }
    }
#endif //((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))
    HAL_WOBBLE_SetCdWblIntReg(0);

//WBLINT
#if (EN_WOBBLE == 1)
    // check the WOBBLE INTERRUPT REGISTER
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    #else
        #if (DVD_RAM_READ == 1)
    if (!DVDRAM)
        #endif
    #endif
    {
        IntStatus = HAL_WOBBLE_GetWblIntReg();

        if (IntStatus)
        {
            if ((IntStatus & HAL_WBL_NEWADDRI) == HAL_WBL_NEWADDRI)
            {
                IDValidIntProc();
                // IDSynIntProc is already taken care in state_pll_chk (wobble and flywheel lock checked)
            }
    #if (RESYNC_MESSAGES == 1)
            if (((IntStatus & HAL_WBL_WDSYOLCKI) == HAL_WBL_WDSYOLCKI)||((IntStatus & HAL_WBL_BTSYOLCKI) == HAL_WBL_BTSYOLCKI)||((IntStatus & HAL_WBL_WDSYRESYI) == HAL_WBL_WDSYRESYI)||((IntStatus & HAL_WBL_BTSYRESYI) == HAL_WBL_BTSYRESYI))
            {
                IDSyncLostIntProc();
            }
    #else
            if ((IntStatus & HAL_WBL_WDSYOLCKI) == HAL_WBL_WDSYOLCKI)
            {
                IDSyncLostIntProc();
            }
    #endif
            //Now no action need to be done when this interrupt happened
            //if ((IntStatus & HAL_WBL_ADDRRCHI) == HAL_WBL_ADDRRCHI)
            //{
            //    IDAddrReachIntProc();
            //}
        }
    }
#endif // (EN_WOBBLE == 1)
    HAL_WOBBLE_SetWblIntReg(0);

    data0 = READ_REG(HAL_SRVREG_MAP->LCIRQ);
    data1 = data0 & READ_REG(HAL_SRVREG_MAP->LCINTEN);
    if(data1)
    {
#if (NEW_FDP_DETECTION == 2)
        if (data1 & HAL_SRV_DSPI6) svoFDropIsr();
#endif

#if (EN_ADI == 1)
        if (data1 & HAL_SRV_DSPI3) servo_status |= SERVO_STATUS_ACTDISS_FOC;
        if (data1 & HAL_SRV_DSPI2) servo_status |= SERVO_STATUS_ACTDISS_TRK;
#endif
    }
    WRITE_REG(HAL_SRVREG_MAP->LCIRQ, (~data0));
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoPSNIsr
 * Description: servo read PSN (Physical Sector Number) interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void svoPSNIsr(void)
{
/* TBD? No Code! */
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoFGInt
 * Description: servo interrupt service routine
 *
 *----------------------------------------------------------------------------*/
#if (CHIP_REV < 0xC0)
#define FG_LIMIT   (60 * INPUT_CLOCK_HZ / (20000 * 2 * DISK_NFGPLS * FG_TIMER_PRESCALER))//20000RPM
#define TSTOP0     (60 * INPUT_CLOCK_HZ / (RPM_STOP0 * 2 * DISK_NFGPLS * FG_TIMER_PRESCALER))
#define TSTOP1     (60 * INPUT_CLOCK_HZ / (RPM_STOP1 * 2 * DISK_NFGPLS * FG_TIMER_PRESCALER))
#define TSTOP2     (60 * INPUT_CLOCK_HZ / (RPM_STOP2 * 2 * DISK_NFGPLS * FG_TIMER_PRESCALER))
#define TSTOP3     (60 * INPUT_CLOCK_HZ / (RPM_STOP3 * 2 * DISK_NFGPLS * FG_TIMER_PRESCALER))
#else
#define FG_LIMIT   (((SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / 20000) / READ_FIELD(HAL_SRV_MDIV))//20000RPM
#define TSTOP0     (((SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / RPM_STOP0) / READ_FIELD(HAL_SRV_MDIV))
#define TSTOP1     (((SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / RPM_STOP1) / READ_FIELD(HAL_SRV_MDIV))
#define TSTOP2     (((SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / RPM_STOP2) / READ_FIELD(HAL_SRV_MDIV))
#define TSTOP3     (((SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / RPM_STOP3) / READ_FIELD(HAL_SRV_MDIV))
#endif

void    svoFGInt(void)
{
    USHORT  temp;
    BYTE    temp_index;

    CLR_TIMER_STATUS(TIMER2);
#if (CHIP_REV < 0xC0)
    /* This interrupt take place every 2 FG. Timer2 return the duration of 1 FG. */
    temp = GetTimerXValue(TIMER2);
    STOP_TIMER(TIMER2);
    START_TIMERS(TIMER2);
    if ((TIMER2.TSTATUS.BIT.TOVFL == 1) || (temp < FG_LIMIT))
#else
    temp = READ_FIELD(HAL_SRV_FGCNT);
    if ((temp == 0xFFFF) || (temp < FG_LIMIT))
#endif
    {
        return;
    }

    svoSpeedVar.fgPulseTime = temp;

    if (svoSpeedVar.fgCount)
    {
        svoSpeedVar.fgCount--;
        svoSpeedVar.fgTotalTime += temp;

        temp_index = svoSpeedVar.fgCount % DISK_NFGPLS;
        if (svoSpeedVar.fg_pulse[temp_index] != 0)
        {
            svoSpeedVar.fg_pulse_rev -= svoSpeedVar.fg_pulse[temp_index];
        }
        svoSpeedVar.fg_pulse[temp_index] = temp;
        svoSpeedVar.fg_pulse_rev += temp;   

#if (DETECT_ECC_DISC == 1)
        if (svoVar_EccCheckEnable > 0)
        {
            if (svoSpeedVar.fgCount == DISK_NFGPLS * svoVar_EccCheckEnable)
            {
                // Disc Eccentricity check start
                svoRunOutCTN = 0;
                if (READ_FIELD(HAL_SRV_CNT) == TRUE)
                {
                    WRITE_REG(HAL_SRVREG_MAP->CTN,0xFFFF);  // For BD ECC150, need to use higher initial value
                    (void)READ_REG(HAL_SRVREG_MAP->CTN);
                    HAL_SERVO_CenterControlON(FORW);
    #if (TRO_INVERSION == 0)// set twice to make sure that CTN has been reseted.
                    WRITE_REG(HAL_SRVREG_MAP->PUFWD, HAL_SRV_TSVFWD);
                    WRITE_REG(HAL_SRVREG_MAP->PUFWD, HAL_SRV_TSVFWD);
    #else
                    WRITE_REG(HAL_SRVREG_MAP->PUFWD, HAL_SRV_TSVBWD);
                    WRITE_REG(HAL_SRVREG_MAP->PUFWD, HAL_SRV_TSVBWD);
    #endif
                }
                else
                {
                    svoRunOutCTN = READ_REG(HAL_SRVREG_MAP->CTN);
                }
            }
            else if (svoSpeedVar.fgCount == 0)
            {
                if (READ_FIELD(HAL_SRV_CNT) == TRUE)
                {
                    svoSpeedVar.fgCount = DISK_NFGPLS * svoVar_EccCheckEnable + 1;
                    SendMsg80(SHOW_DEBUG_MSG,0x310121);
                    return;
                }
                // Disc Eccentricity check end
                svoVar_EccCheckEnable = 0;
                HAL_SERVO_CenterControlOFF();
                if (svoRunOutCTN == 0)
                {
                    svoRunOut = (0xFFFF-READ_REG(HAL_SRVREG_MAP->CTN)) / 4;
                }
                else
                {
                    svoRunOut = (svoRunOutCTN-READ_REG(HAL_SRVREG_MAP->CTN)) / 4;
                }
            }
        }
#endif
    }

    if (svoIramVar.spindleStopFlag == DSTOPPING)
    {
        if (svoIramVar.spindleStopChkCnt < 1)
        {
            //ignore first fgPulseTime due to possible CAVM change
            svoIramVar.spindleStopChkCnt++;
            return;
        }

        if (svoSpeedVar.fgPulseTime < TSTOP0)
        {
            if ((svoIramVar.spindleStopChkCnt & 0xF0) != 0x10)
            {
                svoIramVar.spindleStopChkCnt = 0x10;
            }
            if (svoIramVar.spindleStopChkCnt < 0x1F)
            {
                if ((++svoIramVar.spindleStopChkCnt) == 0x15)
                {
                    SetMotorDriverState(SHORT_BRAKE);
                    WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE0);
                }
            }
        }
        else if (svoSpeedVar.fgPulseTime < TSTOP1)
        {
            if ((svoIramVar.spindleStopChkCnt & 0xF0) != 0x20)
            {
                svoIramVar.spindleStopChkCnt = 0x20;
            }
            if (svoIramVar.spindleStopChkCnt < 0x2F)
            {
                if (++svoIramVar.spindleStopChkCnt == 0x25)
                {
                    SetMotorDriverState(SHORT_BRAKE);
                    WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE1);
                }
            }
        }
        else if (svoSpeedVar.fgPulseTime < TSTOP2)
        {
            if ((svoIramVar.spindleStopChkCnt & 0xF0) != 0x30)
            {
                svoIramVar.spindleStopChkCnt = 0x30;
            }
            if (svoIramVar.spindleStopChkCnt < 0x3F)
            {
                if (++svoIramVar.spindleStopChkCnt == 0x35)
                {
                    SetMotorDriverState(REVERSE_BRAKE);
                    WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE2);
                }
            }
        }
        else
        {
            if ((svoIramVar.spindleStopChkCnt & 0xF0) != 0x40)
            {
                svoIramVar.spindleStopChkCnt = 0x40;
                SetMotorDriverState(REVERSE_BRAKE);
                WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE3);
            }
            if (svoSpeedVar.fgPulseTime >= TSTOP3)
            {
                if (++svoIramVar.spindleStopChkCnt >= 0x43)
                {
                    svoIramVar.spindleStopChkCnt = 0;
                    discstop();
                    svoIramVar.spindleStopFlag = DSTOPPED;
                }
            }
        }
    }

    if ((svoIramVar.spindleStopFlag != DSTOPPING) && (svoSpeedVar.fgCount == 0))
    {
        StopFGTimer();
    }
}

#if DO_HW_SUPPORT_STEP == 0
/*-----------------------------------------------------------------------------
 *
 * Module: svoSLEDInt
 * Description: servo interrupt service routine
 *
 *----------------------------------------------------------------------------*/
void svoSLEDInt(void)
{
    svoSTEPInt();
}
#endif // DO_HW_SUPPORT_STEP == 0

#if (DVD_RAM_READ == 1) && (CHIP_REV >= 0xC0)
void land_flywheek_isr()
{
    const StructureOfGroupsType *pCurrentZone;
    BYTE                        SectorCount;
    BYTE                        HDRFLYCFD_cur;

    HDRFLYCFD_cur = READ_FIELD(HAL_WBL_HDRFLYCFD);

    pCurrentZone = &DVDRAMZoneTable[DVDRAM_PSN2ZONE(RWDisc.dvRAM.CurrentPID)];
    SectorCount  = ((RWDisc.dvRAM.CurrentPID - pCurrentZone->StartSectorPSN) % pCurrentZone->SectorsPerRev) + 1;

    #if (DVDRAM_FREEZE_FLYWHEEL == 1)
    if (land_flywheel_state == LAND_FLYWHEEL_LOCK)
    {
        if ((READ_FIELD(HAL_DVDD_SECTCNVLD) && (SectorCount == READ_FIELD(HAL_DVDD_SECTCNT))) &&
            (READ_FIELD(HAL_DVDD_TRACKSZVLD) && (pCurrentZone->SectorsPerRev == READ_FIELD(HAL_DVDD_TRACKSIZE))) &&
            READ_FIELD(HAL_WBL_PLLLCKS))
        {
            if (HDRFLYCFD_cur >= READ_FIELD(HAL_WBL_HDRFLYMXCFD))
            {
                WRITE_FIELD(HAL_DVDD_TRACKCTRL, 1);         // Freeze land-flywheel of decoder.
                WRITE_FIELD(HAL_WBL_HDRFLYRD, 1);           // Freeze header-flywheel of wobble processor.
                WRITE_FIELD(HAL_WBL_HEADSLIPMGN, 0);        // No wobble slipping is allowed.

                land_flywheel_state = LAND_FLYWHEEL_FREEZE;
                //SendMsg80(DEBUG_SEEK_MSG,0x310706);       // LAND FLYWHEEL: Freeze!
            }
        }
        else
        {
            land_flywheel_state = LAND_FLYWHEEL_HUNT;
            //SendMsg80(DEBUG_SEEK_MSG,0x310704);       // LAND FLYWHEEL: Sector-count or track-size is invalid!
        }
    }
    #endif

    if (land_flywheel_state == LAND_FLYWHEEL_CHECK2)
    {
        if ((READ_FIELD(HAL_DVDD_SECTCNVLD) && (SectorCount == READ_FIELD(HAL_DVDD_SECTCNT))) &&
            (READ_FIELD(HAL_DVDD_TRACKSZVLD) && (pCurrentZone->SectorsPerRev == READ_FIELD(HAL_DVDD_TRACKSIZE))) &&
            READ_FIELD(HAL_WBL_PLLLCKS))
        {
            if ((HDRFLYCFD_cur > HDRFLYCFD_pre) || (HDRFLYCFD_cur >= READ_FIELD(HAL_WBL_HDRFLYMXCFD)))
            {
                slip_confidence_level++;
            }
            else
            {
                WRITE_FIELD(HAL_WBL_HEADSLIPMGN,1);         // One wobble period slip
                land_flywheel_state = LAND_FLYWHEEL_CHECK1;
                return;
            }
            HDRFLYCFD_pre = HDRFLYCFD_cur;

            if (slip_confidence_level >= 3)
            {
                WRITE_FIELD(HAL_WBL_HDRFLYRD,0);            // Enable resyc header
                WRITE_FIELD(HAL_WBL_HEADSLIPMGN,0);         // Exactly the same wobble time
                land_flywheel_state = LAND_FLYWHEEL_LOCK;
                slip_confidence_level = 0;
                //SendMsg80(DEBUG_SEEK_MSG,0x310703);       // LAND FLYWHEEL: Lock!
            }
        }
        else
        {
            land_flywheel_state = LAND_FLYWHEEL_CHECK1;
            //SendMsg80(DEBUG_SEEK_MSG,0x310704);       // LAND FLYWHEEL: Sector-count or track-size is invalid!
        }
    }

    if (land_flywheel_state == LAND_FLYWHEEL_CHECK1)
    {
        if ((READ_FIELD(HAL_DVDD_SECTCNVLD) && (SectorCount == READ_FIELD(HAL_DVDD_SECTCNT))) &&
            (READ_FIELD(HAL_DVDD_TRACKSZVLD) && (pCurrentZone->SectorsPerRev == READ_FIELD(HAL_DVDD_TRACKSIZE))) &&
            READ_FIELD(HAL_WBL_PLLLCKS))
        {
    #if (SRV_LAND_CONTROL == 1)
            if (READ_FIELD(HAL_SRV_TESRCEN))
            {
                HAL_DSP_TELandInvert(DEC_LAND);         // TE polarity is switched with DEC_LAND.
            }
    #endif

            land_flywheel_state = LAND_FLYWHEEL_CHECK2;
            slip_confidence_level = 0;
            HDRFLYCFD_pre = HDRFLYCFD_cur;
        }
        else
        {
            land_flywheel_state = LAND_FLYWHEEL_HUNT;
            //SendMsg80(DEBUG_SEEK_MSG,0x310704);       // LAND FLYWHEEL: Sector-count or track-size is invalid!
        }
    }

    if (land_flywheel_state == LAND_FLYWHEEL_HUNT)
    {
        if (SectorCount == pCurrentZone->SectorsPerRev)
        {
            SectorCount = 0;
        }

        // Program track-size
        WRITE_FIELD(HAL_DVDD_TRACKSIZE, pCurrentZone->SectorsPerRev);

        // Program sector-count ID.
        WRITE_ADR_1B(HAL_DVDD_SECTTID_ADR,  RWDisc.dvRAM.CurrentPID);
        WRITE_ADR_1B(HAL_DVDD_SECTTID1_ADR, RWDisc.dvRAM.CurrentPID>>8);
        WRITE_ADR_1B(HAL_DVDD_SECTTID2_ADR, RWDisc.dvRAM.CurrentPID>>16);

        // Program sector-count.
        WRITE_FIELD(HAL_DVDD_SECTOR_COUNT_FW, SectorCount);

        WRITE_FIELD(HAL_DVDD_MANLAND,0);

        land_flywheel_state = LAND_FLYWHEEL_CHECK1;

        // LAND FLYWHEEL: Sector-count/Track-size have been programmed (PSN = 0x%06X, sector-count = %d)!
        //SendMsgCnB(DEBUG_SEEK_MSG,5,0x310701,B4B(RWDisc.dvRAM.CurrentPID),B1B(SectorCount));
    }
}

void seqPID_Error_Isr()
{
    BYTE    PIDPRD_value;

    if (ABS((RWDisc.dvRAM.DiffPID)) != 0)
    {
        PIDPRD_value = 1 << READ_FIELD(HAL_DVDD_PIDPRD);
        RWDisc.dvRAM.DiffPID = ABS(RWDisc.dvRAM.CurrentPID-(RWDisc.dvRAM.PreviousPID-RWDisc.dvRAM.DiffPID+PIDPRD_value));
        //20101026_02RO==start==Modified the threshold level for judging fake track slip during writing.
        if (RWDisc.dvRAM.DiffPID >= (svoWrParam.DVDRAMSectors*10)) // 10*track
        {
            // Fake Track Slip count (sectors) = %2d, at PID = 0x%06X
            SendMsgCnB(SHOW_DEBUG_MSG,8,0x801052, B4B(RWDisc.dvRAM.DiffPID), B4B(RWDisc.dvRAM.CurrentPID));
        }
        else
        {
    #if 1//(DVDRAM_SHOCK_WRITE_BREAK == 0)
            RWDisc.dvRAM.HeadIdNoGood = TRUE;
    #endif
            // Track Slip count (sectors) = %2d, at PID = 0x%06X
            SendMsgCnB(SHOW_DEBUG_MSG,8,0x801051, B4B(RWDisc.dvRAM.DiffPID), B4B(RWDisc.dvRAM.CurrentPID));
        }

        RWDisc.dvRAM.DiffPID = 0;
    }
    else
    {
        if (READ_FIELD(HAL_DVDD_DRSEQIDI) == TRUE)
        {
            RWDisc.dvRAM.DiffPID = (LONG)(RWDisc.dvRAM.CurrentPID - RWDisc.dvRAM.PreviousPID);
            // Slip count (sectors) = %2d, at Previous PID = 0x%06X
            SendMsgCnB(SHOW_DEBUG_MSG,8,0x801053, B4B(RWDisc.dvRAM.DiffPID), B4B(RWDisc.dvRAM.PreviousPID));
        }
    }

    RWDisc.dvRAM.PreviousPID = RWDisc.dvRAM.CurrentPID;
}
#endif // #if (DVD_RAM_READ == 1) && (CHIP_REV >= 0xC0)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
