/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   ISR.C
*
* DESCRIPTION: Interupt Service Routines
*
*   $Revision: 210 $
*   $Date: 11/03/23 7:20p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\error.h"
#include ".\servo\svo.h"
#include ".\player\plrDB.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\servo\svoMacro.h"
#include ".\hif\gp_equ.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_lib.h"
#include ".\servo\isr.h"
#include ".\al\REG_WOBBLE.h"
#include ".\al\HAL_wobble.h"
#include ".\al\HAL_demod.h"
#include ".\al\HAL_lasc.h"
#include ".\al\HAL_servo.h"
#include ".\al\REG_sdram.h"

extern void svoFGInt(void);
extern void svoSLEDInt(void);
#if DO_HW_SUPPORT_STEP == 0
extern void    svoSTEPInt(void);
#endif // DO_HW_SUPPORT_STEP == 0

#if (RP_AUTO_ADJUST == 1)
#define RPAdjustThreshold       75
#endif
extern BYTE Send_timef;
#if (FLASH_BY_DISC == 1)
void SaveCurrentTransferSettings(BYTE Mode);  // Mode 0: normal , Mode 1: disc flush
#else
void SaveCurrentTransferSettings(void);
#endif
#if (RP_AUTO_ADJUST == 1)
LONG  RPMean;
SHORT CurRP;
#endif
/*-----------------------------------------------------------------------------
 *
 * Module: Timer1Isr
 * Description:
 *
 *----------------------------------------------------------------------------*/
void Timer1Isr(void)
{
    CLR_TIMER_STATUS(TIMER1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: Timer0Isr
 * Description: 1ms
 *
 *----------------------------------------------------------------------------*/
extern BYTE bottom_of_heap;                             /* defined in heap.s */
extern void ResetHandler(void);
void sendFIS34(void)
{
#if (EN_AUTO_FIS34 == 1)
    if (bottom_of_heap != 1)
    {
        if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
        {
            if(bottom_of_heap)
                bottom_of_heap--;
            if (bottom_of_heap == 2 || ((bottom_of_heap == 0) && (Iram_system_timer > 100)))
            {
                //send_msg5S(1,"m_sendFIS34");
                send_msg80(1,0x710605);
                bottom_of_heap = 2;
                LinkTxRxErr = 0;
                ATAClearBusy();
                WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
                WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
            }
            else if (bottom_of_heap == 1)
            {
                if (LinkTxRxErr || ((READ_REG(HAL_HSTREG_MAP->TXREQ)&HAL_HST_TXFISREQ)&&(READ_REG(HAL_HSTREG_MAP->RQTXFSTYP)==0x34)))
                {
                    bottom_of_heap = 5;
                    LinkTxRxErr = 0;
                    WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
                    WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
                }
            }
        }
    }
#endif  /* EN_AUTO_FIS34 == 1 */
}

//TBD!
void Timer0Isr(void)
{
    ULONG svAddr;

    svAddr = Iram_Buf_dram_addr;
    CLR_TIMER_STATUS(TIMER0);
    Iram_system_timer++;

#if (EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3)
    /*Auto Fis34 Hot Plug Work Around!*/
    /*This WA is for ICH5, after PHY Ready, but COMRESET is not issued case.*/
    if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
    {
        if(SendFis34Already == 0)
        {
            if (SATA_FE & bINTFPWR)
            {
                //For B3 chip, if SATA PHY's state changes from not ready to ready , chip will send a FIS34 to host automatically.
                //This autoFIS34 is allowed only in cases of Reset or Hotplug, but not in waking up from sleeping.
                //We should disable autoFIS34 function here.
                WRITE_FIELD(HAL_HST_AFIS34DIS, 1);

                SendMsg80(1,0x710609);                              // SATA re-connected after sleeping
                send_msg81(1,0x71060F,SATA_intf_pwr_state);         // SATA Intf Power state
                SendFis34Already = 1;
            }

            /*PHY Ready and after 50ms manual issue FIS34*/
            if(Iram_system_timer >= BGFis34Timer + 50) //PHY Ready Over 50msec
            {
                ATAClearBusy();
                WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
                WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
                WRITE_FIELD(HAL_HST_AFIS34DIS, 1);
                SendFis34Already = 1;
                SATA_FE &= (BYTE)(~bINTFPWR);
                //send_msg5S(1,"BG SendFIS34 1");
                send_msg80(1,0x710606);

            }
            else if(Iram_system_timer >= (~BGFis34Timer + 50))
            {
                ATAClearBusy();
                WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
                WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
                WRITE_FIELD(HAL_HST_AFIS34DIS, 1);
                SendFis34Already = 1;
                SATA_FE &= (BYTE)(~bINTFPWR);
                //send_msg5S(1,"BG SendFIS34 2");
                send_msg80(1,0x710607);
            }
        }
    }
    else
    {
        BGFis34Timer = Iram_system_timer; //PHY Not Ready, Reset Timer.
        if(SendFis34Already == 1)
        {
            SendFis34Already = 0; //Reset Flag
            //Boot CD fail in Boston,NARRA5 PC.
            if ((St_cmd_inproc == TRUE)  ||(Iram_St_fast_cache_hit == TRUE))//|| READ_REG(HAL_HSTREG_MAP->RAMWDCCL)
            {
                BIST_In_Process = 0;
                #if (ENABLE_SATA_FE == 1)
                if (SATA_FE & bSSP)
        #if (FLASH_BY_DISC == 1)
                    SaveCurrentTransferSettings(0);
        #else
                    SaveCurrentTransferSettings();
        #endif
                else SATA_FE |= bSSP;
                #endif
                WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
                WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
                ATAPIDeviceResetNoFIS34();
            }
            else if((SATA_FE & bINTFPWR) != bINTFPWR)//WarnBoot fail in Southbridge is ICH5
            {
                WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
                WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
            }

            //send_msg5S(1,"Disconnected");
            send_msg80(1,0x710608);
        }
    }
#endif // #if (EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3)

#if (DEFECT_LENGTH_MEASURE == 1)
    if (svoDfMeasVar.duration)
    {
        if (--svoDfMeasVar.duration == 0)
        {
            libDisableDFCTInt();
            svoDfMeasVar.defect_length = svoDfMeasVar.temp_length;
            svoDfMeasVar.defect_count = svoDfMeasVar.temp_count;

        }
    }
#endif


    if ((Iram_system_timer % 20) == 0)
    {
#if ((EN_AUTO_FIS34 == 1) && (CHIP_REV >= 0xC0))
        if ((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
        {
            if (SATAConnect == 0)
            {
                SATAConnect = 1;
                SendMsg80(1,0x710611);                           // SATA Cable Conneted
            }
        }
        else
        {
            if (SATAConnect == 1)
            {
                SATAConnect = 0;
                SendMsg80(1,0x710608);                           // SATA Cable Disconneted
                //Boot CD fail in Boston,NARRA5 PC. TC modify, Elvis comfirmed.
                if ((St_cmd_inproc == TRUE)  ||(Iram_St_fast_cache_hit == TRUE))//|| READ_REG(HAL_HSTREG_MAP->RAMWDCCL)
                {
                    BIST_In_Process = 0;
                    #if (ENABLE_SATA_FE == 1)
                    if (SATA_FE & bSSP)
                        ;//SaveCurrentTransferSettings();
                    else
                        SATA_FE |= bSSP;
                    #endif
                    WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
                    WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
                    ATAPIDeviceResetNoFIS34();
                }
#if 0 // if enable this, sometimes Host command will be lost after FWRST.
                #if (HIPM_ENABLED == 0)
                else if ((SATA_FE & bINTFPWR) != bINTFPWR)
                {
                    SendMsg80(SEND_ATAPI_CMD,0x710004);
                     WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
                     WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
                }
                #endif
#endif
            }
        }
#endif
        /* 20 msec */
       if ((Iram_system_timer % 100) == 0)
       {
#if (DEFECT_LENGTH_MEASURE == 1)
            if (svoDfMeasVar.duration == 0)
            {
                if (svoSpeedVar.CurrentRpm > 240)
                {
                    //svoDfMeasVar.duration = 60000 / svoSpeedVar.CurrentRpm + 1;
                    svoDfMeasVar.duration = 60000 * 2 / svoSpeedVar.CurrentRpm + 1;
                    svoDfMeasVar.temp_length = 0;
                    svoDfMeasVar.temp_count = 0;
                    libEnableDFCTInt();
                }
            }
#endif
       }


        if ((Iram_system_timer % 1000) == 0)
        {
#if (FDP_TRKSLIP_MONITOR == 1)
                if (svoCalVar.discChkDoneBits & start_end)
                {
                    if (svoMiscVar.focusDropCount > 0)
                    {
                        svoMiscVar.focusDropCount--;
                    }
                    if (svoMiscVar.trackSlipCount > 0)
                    {
                        svoMiscVar.trackSlipCount--;
                    }
                }
#endif
#if (NEW_SPINDLE_CONTROL == 1)
                if ((svoSpeedVar.TargetRpm > 0) && svoSpeedVar.SpdInRange)
                {
                    if (svoSpeedVar.SpdInRangeCnt++ == 4)//svoSpeedVar.SpdInRange for 4~5 seconds
                    {
                        spdAdaptIntegrator();
                    }
                }
#endif
                if (svoVar_bByPassFOKCheck == FALSE)
                {
#if (CHIP_REV >= 0xC0)
                    //clear above layer
                    WRITE_DSP_COEF(FEDG_pol, 0);
                    WRITE_DSP_COEF(FEDG_thr, 0);
                    while ((READ_DSP_COEF(FOO_above_layer) & 0x0004));
                    WRITE_DSP_COEF(FEDG_thr, svoPrvtVar_FEDG_thr);
    #if (FOO_INVERSION == 0)
                    WRITE_DSP_COEF(FEDG_pol,0x7FFF);
    #else
                    WRITE_DSP_COEF(FEDG_pol,0x8000);
    #endif
#endif
                }
        }

        if (Iram_enable_kernel_timer == 1)
        {
            KernelTimer1Update();
            KernelTimer3Update();
        }

        if (svoIramVar.spindleStopFlag == DSTARTED)
        {
    #if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
            if (svoSpeedVar.TargetRpmReal != svoSpeedVar.TargetRpm)
            {
        #if (NEW_SPINDLE_CONTROL == 1)
                svoSpeedVar.SpdInRangeCnt = 0;
        #endif
                if ((svoSpeedVar.SpdUp) && (SERVO_CLOCK * (60 * 4 / 12) < svoSpeedVar.TargetRpmReal * (0xCCCC / 12) * READ_FIELD(HAL_SRV_MDIV)))
                {
                    USHORT cur_rpm;

                    cur_rpm = spdGetCurrentRPM();
                    if (cur_rpm >= svoSpeedVar.MinimumRpm)
                    {
                        spdSetCAVM(svoSpeedVar.MinimumRpm);
                        svoSpeedVar.SpdUp = FALSE;
                    }
                    else
                    {
                        spdSetCAVM(cur_rpm);
                    }
                }
                spdSetFGFreq(svoSpeedVar.TargetRpm);
            }
    #endif
            if (!svoSpeedVar.SpdInRange)
            {
    #if (NEW_SPINDLE_CONTROL == 1)
                svoSpeedVar.SpdInRangeCnt = 0;
    #endif
                if (svoSpeedVar.SpdWaitCnt < 0xFFFF)
                {
                    svoSpeedVar.SpdWaitCnt++;
                }
                if ((svoSpeedVar.SpdWaitCnt & 0x01) == 0x00)
                {
                    svoSpeedVar.SpdMonitor = TRUE;
                }
            }
        }
    }

#if 0  //Send SATA and SDRAM settings out
    if ((Iram_system_timer % 5000) == 0)
    {
        //SATA
        send_msg5SValue(1, "PPHYCFGL", 1, READ_REG(HAL_HSTREG_MAP->PPHYCFGL));
        send_msg5SValue(1, "PPHYCFGM", 1, READ_REG(HAL_HSTREG_MAP->PPHYCFGM));
        send_msg5SValue(1, "PPHYCFGH", 1, READ_REG(HAL_HSTREG_MAP->PPHYCFGH));
        send_msg5SValue(1, "GPHYCFGL", 1, READ_REG(HAL_HSTREG_MAP->GPHYCFGL));
        send_msg5SValue(1, "GPHYCFGH", 1, READ_REG(HAL_HSTREG_MAP->GPHYCFGH));
        send_msg5S(1, "=============");
        //SDRAM
        send_msg5SValue(1, "SDCONF0", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF0));
        send_msg5SValue(1, "SDCONF1", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF1));
        send_msg5SValue(1, "SDCONF2", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF2));
        send_msg5SValue(1, "SDDELAY0", 1, READ_REG(HAL_SDRAMREG_MAP->SDDELAY0));
        send_msg5SValue(1, "SDDELAY1", 1, READ_REG(HAL_SDRAMREG_MAP->SDDELAY1));
        send_msg5S(1, "xxxxxxxxxxxxx");
    }
#endif

    Iram_Buf_dram_addr = svAddr;
#if (RP_AUTO_ADJUST == 1)
   if ((((RPOfsAdjust & RP_ADJ_R_MASK) == RP_ADJ_R_MASK) || ((RPOfsAdjust & RP_ADJ_L_MASK) == RP_ADJ_L_MASK))
     && (HAL_GLOBAL_GetRfPresent() == TRUE))
   {
       RPAcc = RPAccPrv - (RPAccPrv >> 4);
       CurRP = ReadAverageAdcmV(ADC_RP, 3);
       RPAcc = RPAcc + CurRP;
       RPMean = (RPAcc + RPAccPrv) >> 5;
       RPAccPrv = RPAcc;

       if ((RPMean > (vref_fm_mux3_mV+RPAdjustThreshold)) || (RPMean < (vref_fm_mux3_mV-RPAdjustThreshold)))
       {
           LONG CurRp2Ofs,RPErr;

           CurRp2Ofs = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RP2OS),8);
           RPErr = RPMean - vref_fm_mux3_mV;
           CurRp2Ofs = CurRp2Ofs + ((RPErr * CalRP2Gain[Layer_Index]) >> 16);
           WRITE_FIELD(HAL_AFE_RP2OS,calSBYTE2DacCount(CurRp2Ofs,8));
           if (RPErr > 0)
           {
               RPAcc = RPAccPrv = (vref_fm_mux3_mV + RPAdjustThreshold) << 4;
           }
           else
           {
               RPAcc = RPAccPrv = (vref_fm_mux3_mV - RPAdjustThreshold) << 4;
           }
       }
   }
#endif // #if (RP_AUTO_ADJUST == 1)
}


/*-----------------------------------------------------------------------------
 *
 * Module: Timer2Isr
 * Description: FG
 *
 *----------------------------------------------------------------------------*/
void Timer2Isr(void)
{
    CLR_TIMER_STATUS(TIMER2);
    svoFGInt();
}


/*-----------------------------------------------------------------------------
 *
 * Module: Timer3Isr
 * Description: 240us
 *
 *----------------------------------------------------------------------------*/
void Timer3Isr(void)
{
    CLR_TIMER_STATUS(TIMER3);

    if (fCheck_Pluse_Power == TRUE)
    {
        // 1sec=10000*100us
        if (fConut_NUM == 10000)
        {
            EFM_SMP_Control(EFMSTOP,SMP_AUTO);

            DISABLE_TIMER_INT(TIMER1);
            STOP_TIMER(TIMER1);

            fCheck_Pluse_Power = FALSE;
            send_msg1('1','0','0','0');
        }
        fConut_NUM ++;
    }
    else
    {
        Iram_timer3_counter++;
    }
}

#if ((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))
/*-----------------------------------------------------------------------------
 *
 * Module: ATIPMSFReadyIntProc
 * Description: read ATIP interrupt service routine
 *
 *----------------------------------------------------------------------------*/
    #if (MSG_OUT_MODE == MSG_USBPORT)
vBYTE SecWas;
    #endif
void ATIPMSFReadyIntProc(void)
{

    ULONG  atipStatus;
    BYTE    msbATIP;
    BYTE    CdAtipData[3];
    ULONG   CdAtipAddress;
    TimeS   atipdataBCD;
    #if ((CHECK_ATIP_QUALITY == 1) || (FORCE_ATIP_TRIGGER == 1))
    LONG    packet_size;
    #endif
    #if FORCE_ATIP_TRIGGER
    BYTE residue;
    #endif


/* *************************************************************************
 *  bypass 1 Subq/Atip Isr in case too many Subq/Atip Isr routine will occupy all CPU resource,
 *  which keep normal F/W flow hanging
 *******************************************************************/
    #if 1 // TBD? why is this here?
    if ((svoCalVar.discChkDoneBits & start_end) && (SpeedMD != Write_f) && (svoIramVar.subQEnableFlag) && (svoIramVar.svoCurrentSpeed >= _40AX)
        &&((svoVar_discKindT.fDiscKind == eCDRDisc)||(svoVar_discKindT.fDiscKind == eCDRWDisc)))
    {
        if(svoIramVar.SubqAtipBypass!=0)
        {
            svoIramVar.SubqAtipBypass--;
            // when will this be enabled again?
            WRITE_FIELD(HAL_CDD_SCOREN, 0);         /* Disable SUB-Q interrupt */
            return;
        }
        svoIramVar.SubqAtipBypass=1;
        WRITE_FIELD(HAL_CDD_SCOR, 0);               /* Clear SUB-Q interrupt */
        WRITE_FIELD(HAL_CDD_SCOREN, 1);             /* Enable SUB-Q interrupt */
    }
    #endif

    if (!HAL_DEMOD_GetFePllLocked())
    {   // Sync pattern was not detected !!     /* Recorded area? */
        if(pdbGetSubQIntStatus())
            pdbSetSubQIntStatus(FALSE);
        else
            pdbSetEFMOnDisc(FALSE);
    }

    atipStatus   = READ_REG(HAL_WBLREG_MAP->ATADBFS);

    if ((atipStatus & HAL_WBL_CDERRFREE ) != HAL_WBL_CDERRFREE )
    {
        pdbSetAtipOk(FALSE);
        svoIramVar.atipReadyFlag = FALSE;

        /* Count ATER */
        //pdbGetATIPTime(&atipdataBCD);       /* do not use pdbGetATIPTime() here! */
        atipdataBCD = Iram_DiscInfo.CD.ATIP;  /* use this variable to avoid PCA's ATIP gap being added twice */
        mathIncremetBCDTime(&atipdataBCD);
        pdbSetATIPTime(&atipdataBCD);
        Iram_AtipAdipLppErrorCount++;
        if (atipdataBCD.Frame==0x74)
        {
            Iram_AtipAdipLppErrorAmount = Iram_AtipAdipLppErrorCount;
            Iram_AtipAdipLppErrorCount = 0;
        }
    	atipdataBCD.Minute = READ_FIELD(HAL_WBL_ATFLYMIN);
    	atipdataBCD.Second = READ_FIELD(HAL_WBL_ATFLYSEC);
    	atipdataBCD.Frame  = READ_FIELD(HAL_WBL_ATFLYFRM);
        atipStatus = READ_REG(HAL_WBLREG_MAP->ATADBFRDD); //0xb0001240
        atipStatus = READ_REG(HAL_WBLREG_MAP->ATADBFRDE); //clear ATIP buffer 0xb0001244
        return;
    }
    else
    {
        Iram_AtipAdipLppErrorCount = 0;
    }

    //if (!HAL_WOBBLE_IsWblAddReady())
    if ( (atipStatus&0x40) != 0x40 )    //because of status can read only one times
    {
        return;
    }
    else
    {
        CdAtipAddress = HAL_WOBBLE_GetWblAdd(CdAtipData, atipStatus);
        atipdataBCD.Minute = CdAtipData[0];
        atipdataBCD.Second = CdAtipData[1];
        atipdataBCD.Frame  = CdAtipData[2];

        //andy c m ,because of status can read only one times//if(HAL_WOBBLE_GetMSBAtipInfoAvail())
        if((atipStatus&1))
        {
            msbATIP =   ((atipdataBCD.Minute >> 7) << 2) |
                        ((atipdataBCD.Second >> 7) << 1) |
                         (atipdataBCD.Frame  >> 7);
        }
        else
        {
            msbATIP = 0;
        }
    }

    #if (MSG_OUT_MODE == MSG_USBPORT)
    if (hifDebugMsgFlag & SEND_ATIP_WOBBLE)
    {
       if(Send_timef)
       {
          //send_msg54(SEND_ATIP_WOBBLE,atipdataBCD.Minute,atipdataBCD.Second,atipdataBCD.Frame,msbATIP);
          SendMsgCnB(SEND_ATIP_WOBBLE,4,0x800B00,B1B(atipdataBCD.Minute),B1B(atipdataBCD.Second),B1B(atipdataBCD.Frame),B1B(atipdataBCD.Frame),B1B(msbATIP));
       }
       else
       {
          if (SecWas != atipdataBCD.Second)
          {
             //send_msg54(SEND_ATIP_WOBBLE,atipdataBCD.Minute,atipdataBCD.Second,atipdataBCD.Frame,msbATIP);
             SendMsgCnB(SEND_ATIP_WOBBLE,4,0x800B00,B1B(atipdataBCD.Minute),B1B(atipdataBCD.Second),B1B(atipdataBCD.Frame),B1B(atipdataBCD.Frame),B1B(msbATIP));
             SecWas = atipdataBCD.Second;
          }
       }
    }
    #endif

    /* count ATER */
    if (atipdataBCD.Frame==0x74)
    {
        Iram_AtipAdipLppErrorAmount = Iram_AtipAdipLppErrorCount;
        Iram_AtipAdipLppErrorCount = 0;
    }

    #if FORCE_ATIP_TRIGGER
    if((atipdataBCD.Minute >= 1) && (CheckWGateAtip == TRUE)){
        switch(Disc_Speed.Write2){
            case e52XCAV:
            case e48XCAV:
                residue=0;
                break;
            case e40XCAV:
                residue=1;
                break;
            case e32XCAV:
                residue=2;
                break;
            default:
                residue=3;
                break;
        }
        if((((atipdataBCD.Minute & 0x0F) % 5) == residue) && (atipdataBCD.Second == 0x15)){   /* ATIP time */
            if(READ_FIELD(HAL_WS_WGATEMON))
            {
                if((GetWriteMethod() == FPKT) && (Record.Data == eHostData)){
                    packet_size = pdbGetTrackPacketSize(Record.tno);
                    Iram_Record_pause_lba -= ((Iram_Record_pause_lba - Iram_CurrEncodeLBA) / (packet_size + 7L) -1L) * (packet_size + 7L);
                }else{
                    ExacWritePause();
                }
                if(Record.Data == eHostData){
                    Iram_RecordState = eRECORD_UNDERRUN;
                }
            }else{
                WRITE_FIELD(HAL_CDE_STASTOP, halCDE_ManualStop);        /* Manual Stop CD Encoder */
            }
            if(GetSpeedValue(Disc_Speed.Write2) >= GetSpeedValue(e32XCAV)){  // modify form 40X to 32X
                Disc_Speed.Write2 = Disc_Speed.WriteMarginal = GetLowerDriveWriteSpeed(Disc_Speed.Write2);         /* Set write speed */  // ychsiao_sunnyvale
                WrtChangeSPWaitTimer = 800;
                RequestSpinDown = TRUE;
            }
            CheckWGateAtip = FALSE;
            if (svoIramVar.opcIsrEnableFlag == 0)
                svoIramVar.subQEnableFlag = 0;
        }
    }
    #endif

    #if CHECK_ATIP_QUALITY
    if(CheckAtipQuality == TRUE){
        if((atipStatus & 0x0C) == bASEARCH){
            AtipSearchModeCount++;
        }else if((atipStatus & 0x0C) == bAPROTECT){
            AtipSearchModeCount = 0xFF;
        }
        if(AtipSearchModeCount != 0xFF){
            if(AtipSearchModeCount >= MAX_ATIP_SEARCH_COUNT){
                if(READ_FIELD(HAL_WS_WGATEMON))
                {
                    if((GetWriteMethod() == FPKT) && (Record.Data == eHostData)){
                        packet_size = pdbGetTrackPacketSize(Record.tno);
                        Iram_Record_pause_lba -= ((Iram_Record_pause_lba - Iram_CurrEncodeLBA) / (packet_size + 7L) -1L) * (packet_size + 7L);
                    }else{
                        ExacWritePause();
                    }
                    if(Record.Data == eHostData){
                        Iram_RecordState = eRECORD_UNDERRUN;
                    }
                }else{
                    WRITE_FIELD(HAL_CDE_STASTOP, halCDE_ManualStop);        /* Manual Stop CD Encoder */
                }
                Disc_Speed.Write2 = Disc_Speed.WriteMarginal = GetLowerDriveWriteSpeed(Disc_Speed.Write2);        /* Spin Down */
                WrtChangeSPWaitTimer = 800;
                RequestSpinDown = TRUE;
                CheckAtipQuality = FALSE;
                if (svoIramVar.opcIsrEnableFlag == 0)
                    svoIramVar.subQEnableFlag = 0;
            }
        }
    }
    #endif
    /* atip data is ready and CRC OK */
    /* save in database */
    switch (msbATIP){
        case eATIPProgramAreaAndLeadOut:           /* Current ATIP Time code in Program Area and Lead Out Area */
        case eATIPAndPCAAndPMAAndLeadIn:           /* Current ATIP Time code in PCA, PMA, and LeadIn Areas */
            pdbSetAtipOk(TRUE);
            pdbSetATIPTime(&atipdataBCD);
            svoIramVar.atipReadyFlag = TRUE;
            if(svoIramVar.subQEnableFlag == 0)
            {
                HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);
            }
            break;
        default:
            /* Update ATIP special info */
            pdbSetATIPInfo(msbATIP, &atipdataBCD);
            svoIramVar.atipReadyFlag = FALSE;
            break;
    } // switch
}
#endif //((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
