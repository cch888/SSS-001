/****************************************************************************
*                (c) Copyright 2001  -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_tim2.c
*
* DESCRIPTION
*
* This file contains timer interrupt service routines.
*
* NOTES:
*
*   $Revision: 90 $
*   $Date: 11/03/28 2:37p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_util.h"
#include ".\hif\at_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_equ.h"

#if (BD_ENABLE == 1)
#include ".\bd\bddef.h"
#include ".\bdvars.h"
#endif

#include ".\dvd\dvgvars.h"
#include ".\hif\gp_key.h"


private ULONG   system_10ms_clock;
private BYTE    system_timer_10msec;
private BOOL    system_timer_10msec_enable;

void    PowerSave_ISR(void);
#if (ENABLE_SATA_FE == 1)
void    SATAPowerSave_ISR(void);
#endif

void    KernelTimer1Update(void);
void    KernelTimer2Update(void);
void    KernelTimer3Update(void);

extern vBYTE Iram_enable_kernel_timer;
extern BYTE gPowerEvent;        /* power event */
extern BYTE gPowerStatus;       /* current power status */
/*********************************************************************
* UpdateSystemTimer()
*
*********************************************************************/
void UpdateSystemTimer(void)
{

    if(system_timer_10msec_enable == FALSE)
    {
        system_timer_10msec = 0;
        system_timer_10msec_enable = TRUE;
        StartTimer(&system_10ms_clock);
        return;
    }

    if(ReadTimer(&system_10ms_clock) > 10L)
    {       /* 10 msec interval */

        StartTimer(&system_10ms_clock);                /* restart timer    */

        system_timer_10msec++;

        if(system_timer_10msec & 0x01)
        {
            if(Iram_enable_kernel_timer == 1)
            {       /* 20 msec interval */
                KernelTimer2Update();
            }
        }

        if((system_timer_10msec % 10) == 0)
        {           /* 100 msec */
            if(ataPowerMode == IDLE_MODE)
            {
                PowerSave_ISR();                /* 100msec interval */
            }
#if(ENABLE_SATA_FE == 1)
            if (SATA_FE & bINTFPWR)
	    {
		    SATAPowerSave_ISR();
	    }
#endif
            CheckPushTray();
        }

        if(system_timer_10msec == 100)
        {               /* 1 sec */
            system_timer_10msec = 0;
        }
    }
}


/*********************************************************************
* PowerSave_ISR()
*
*********************************************************************/
void PowerSave_ISR(void)
{
    #ifdef BYPASS_POWERSAVE_ISR
    if(svoFgBaseOnPowerSaveFlag) {
    #endif
    if (svoVar_powerSaveFlag)
    {
        Iram_Knl_save_counter -= 1;
#if(CEProduct == 0)
        if(Iram_Knl_save_counter % 100  == 0)// every 10 seconds
        {
            if(SpeedMD == AutoMode)
                SpeedCount = SPEED_DOWN_FOR_POWER_SAVE;    // speed down
        }
#endif

        if(Iram_Knl_save_counter == 0)
        {
            SendMsg80(SEND_ATAPI_CMD,0x710400);
            SpeedMD = Standby_f;
        }
        else if(Iram_Knl_save_counter < CMD_IDLE_CNT)
        {
            if(gPowerStatus != 2)
            {
                gPowerEvent = 1;
                gPowerStatus = 2; // power status = idle
            }
#if (STORE_MSG_TO_FLASHROM==1)
            if(SaveLogToROMSwitch)
            {
                SaveLogToROMSwitch = FALSE;
                SaveLogToROMatIdle = TRUE;   
            }            
#endif
        }
        else if(gPowerStatus != 1)
        {
            gPowerEvent = 1;
            gPowerStatus = 1; // power status = active
        }
    }
    #ifdef BYPASS_POWERSAVE_ISR
    }
    #endif
}

/*********************************************************************
* PowerSave_ISR()
*
*********************************************************************/
#if (ENABLE_SATA_FE == 1)
void SATAPowerSave_ISR(void)
{
    if (Iram_SATA_Idle_counter != 0)
    {
        Iram_SATA_Idle_counter--;

        if(Iram_SATA_Idle_counter == (SATA_STANDBYCNT-40))
        {
            //send_msg5S(SEND_ATAPI_CMD,  "SATAPowerISR");
            send_msg80(SEND_ATAPI_CMD,0x710401);
            send_msg54(1, (SATA_FE & bINTFPWR), (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL)),SATA_intf_pwr_state, (bIPMACK_P|bIPMACK_S));
            if (((READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL)) == 0) && ((SATA_intf_pwr_state & (bIPMACK_P|bIPMACK_S))==0))
            {
                //send_msg5S(SEND_ATAPI_CMD,  "PMREQ_P");
                send_msg80(SEND_ATAPI_CMD,0x710402);
                SATA_intf_pwr_state = bIPMREQ_P;
                WRITE_FIELD(HAL_HST_PARTLREQ, 1);
            }
        }

        if(Iram_SATA_Idle_counter == 0)
        {
            //send_msg5S(SEND_ATAPI_CMD,  "SATAPowerISR");
            send_msg80(SEND_ATAPI_CMD,0x710401);
            send_msg54(1, (SATA_FE & bINTFPWR), (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL)),SATA_intf_pwr_state, (bIPMACK_P|bIPMACK_S));
            if (((READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER/*|HAL_HST_PARTIAL*/)) == 0) && (SATA_intf_pwr_state & bIPMACK_P))
            {
                //send_msg5S(SEND_ATAPI_CMD, "PMREQ_S");
                send_msg80(SEND_ATAPI_CMD,0x710403);
                WRITE_FIELD(HAL_HST_DWCLKON, 1);
                WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
                DelaymS(1);

                SATA_intf_pwr_state = bIPMREQ_S;
                WRITE_FIELD(HAL_HST_SLMBERREQ, 1);
            }
        }
    }
}
#endif
/*********************************************************************
* KernelTimer1Update()
*
*  >Knl wait timer count down
*  >host timeup count down
*  >Check Eject switch status
*********************************************************************/
void    KernelTimer1Update(void)
{
    Iram_Knl_timer_counter -= 1;                    /* Decrement Kernel Timer tick */
    if (Iram_Knl_timer_counter == 0)
    {
        Knl_timeout = TRUE;                     /* mark time out */
    }

    /*-----------------------------------------------------------------
       DVD block interrupt timer
    ------------------------------------------------------------------*/
    if (dvdBlockInterruptTimer.state == ON)
    {
        --dvdBlockInterruptTimer.count;
        if (dvdBlockInterruptTimer.count == 0)
        {
            dvdBlockInterruptTimer.state = OFF;
            dvdBlockInterruptTimer.fTIMEOUT = TRUE;
#if (HORROR_DISC_IMPROVEMENT == 1)
            PO_UN_C = 1111;
#endif
            send_msg80(SEND_READ_DVD_MSG,0x800008);
        }
    }

#if (BD_ENABLE == 1)
    /*-----------------------------------------------------------------
       BD cluster interrupt timer
    ------------------------------------------------------------------*/
    if (bdClusterInterruptTimer.state == ON)
    {
        --bdClusterInterruptTimer.count;
        if (bdClusterInterruptTimer.count == 0)
        {
            bdClusterInterruptTimer.state = OFF;
            bdClusterInterruptTimer.fTIMEOUT = TRUE;
            send_msg80(SEND_READ_DVD_MSG,0x800008);
        }
    }
#endif  /* BD_ENABLE == 1 */



    if( Iram_Svr_player_mode != EXECUTING_WRITE_MODE)
    {
        if ((Knl_cmd_avail)||(St_cmd_inproc == TRUE))  //PC Dr cann't detect drive after DVD defect disc testing.
        {
            if (svoIramVar.knlTimeupCount != 0)
            {
                 svoIramVar.knlTimeupCount -= 1;        /* Decrement Kernel Timer tick */
            }
        }
    }
}

/*********************************************************************
* KernelTimer2Update()
* (non operation during seek)
*  >Disc speed control DI timer
*********************************************************************/
void    KernelTimer2Update(void)
{
    if(READ_FIELD(HAL_CDD_DECEN))
    {
        if(DecTimeCnt < SPECIAL_SPD_MAX_TIME )
        {
            DecTimeCnt++;
        }
    }
    else if(((St_cmd_inproc == FALSE)||(  (Iram_Cmd_block.bf[0] != SBC_READ_10)
                                        &&(Iram_Cmd_block.bf[0] != SBC_READ_12)
                                        &&(Iram_Cmd_block.bf[0] != MMC_READ_CD)
                                        &&(Iram_Cmd_block.bf[0] != MMC_READ_CD_MSF))) && (SpeedMD != Write_f))
    {
        DecTimeCnt--;
        if(DecTimeCnt < 0L )
        {
            if((St_cdrom_data == TRUE) || (Rw_cdda_disc == TRUE))
            {
                DecTimeCnt = 0L;
            }
            else if(DecTimeCnt < PLAYBACK_SPD_MIN_TIME)
            {
                DecTimeCnt  = PLAYBACK_SPD_MIN_TIME;
            }
        }
    }
}


/*********************************************************************
* KernelTimer3Update()
*
*  >Knl wait timer count down
*  >host timeup count down
*  >Check Eject switch status
*  >LED Control
*  >Hold pin Control
*********************************************************************/
void    KernelTimer3Update(void)
{
    /* Change the Led status */
    /***** LED Control *****/
    if (St_led_flash_error)
    {
#if  (BURN_IN == 1 || MODULE_TEST == 1)
        if (Burn_in_LED == ON)
        {
            if (Burn_in_Msg_OFF_timer)
            {
                Burn_in_Msg_OFF_timer--;
            }
            else
            {
                if (Iram_LED_Flash_timer == 0)
                {
                    /* Led flash interval: Iram_LED_Flash_timer*2(on & off) */
                    /* Led off interval: Burn_in_Msg_OFF_timer */
	#if(SUN_FUNC_TEST == 0)
                    opReadyLED = (~ opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif
                    //reset to 20ms, not using Iram_Gp_led_timer(=0)
                    Iram_LED_Flash_timer =  400/20;
                    Burn_in_LED_Blink_count++;
                    if(Burn_in_error_ID && Burn_in_LED_Blink_count >= Burn_in_error_ID*2)   //on & off
                    {
                        Burn_in_LED_Blink_count=0;

	#if(SUN_FUNC_TEST == 0)
                        opReadyLED = LED_OFF;
	#else
              HAL_DSP_LED(OFF);
       #endif
                        //set 1 sec interval between showing message
                        Burn_in_Msg_OFF_timer = 1000/20;    /* 1sec */
                    }
                }
                else
                    Iram_LED_Flash_timer--;
            }
        }
        else
        {
#endif  //#if  (BURN_IN == 1 || MODULE_TEST == 1)

#if (LED_NUM == 1)
            if (Iram_Gp_led_at_timer == 0)
            {
                St_led_flash_error = 0;
	#if(SUN_FUNC_TEST == 0)
                opReadyLED = LED_OFF;
	#else
              HAL_DSP_LED(OFF);
       #endif
            }
            else
            {
                if (Iram_LED_Flash_timer == 0)
                {
	#if(SUN_FUNC_TEST == 0)
                    opReadyLED = (~ opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif
                    Iram_LED_Flash_timer = PlayLED_RATE;        /* 500ms */
                }
                else
                    Iram_LED_Flash_timer--;

                Iram_Gp_led_at_timer--;
            }
#elif (LED_NUM == 2)
            if (Iram_LED_Flash_timer == 0)
            {
                St_led_flash_error = 0;
                opErrorLED = LED_OFF;
	#if(SUN_FUNC_TEST == 0)
                opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(OFF);
       #endif
            }
            else
                Iram_LED_Flash_timer --;
#endif /* LED_NUM == 2 */
#if  (BURN_IN == 1 || MODULE_TEST == 1)
        }
#endif  //#if  (BURN_IN == 1 || MODULE_TEST == 1)
    }
    else
    {
        if ((St_led_flash_ready) || (St_led_flash_write))
        {
            if (Iram_LED_Flash_timer == 0)
            {
                if (St_led_flash_ready)
                {
	#if(SUN_FUNC_TEST == 0)
                    opReadyLED = (~ opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif
                }
#if (LED_NUM == 2)
                if (St_led_flash_write)
                {
                opWriteLED = (~ opWriteLED);
                }
#endif
                Iram_LED_Flash_timer = Iram_Gp_led_timer;
            }
            else
                Iram_LED_Flash_timer--;
        }
#if (LED_NUM == 2)
        if (St_led_at_ready)
        {
            if (Iram_Gp_led_at_timer ==0)
            {
                St_led_at_ready = 0;
                St_led_flash_ready = 0;
                ReadyLED(LED_ON);
            }
            else
            Iram_Gp_led_at_timer--;
        }
#endif /* LED_NUM == 2 */
    }

#if (SET_SLOT_IN == 1)
	if(fgSRSTEj) //[Q36]Jam when power on ,Disc auto tray out.
	{	
		//SendMsg5S(1, "TIMER ");
		EjectKeyInfo.Pressed = TRUE;	
		if((ipOpenSW == 1)&&(ipSW2 == 0)&&(ipCloseSW == 1))
		{	
			//SendMsg5S(1, "JJJJJ "); 
			if(Iram_Auto_TrayIn_timer == 0)
			{
				fgSRSTEj = FALSE;
				TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
			}				
			else
				Iram_Auto_TrayIn_timer--;
		}
		else if((ipOpenSW == 0)&&(ipSW2 == 0)&&(ipCloseSW == 1))
		{	fgSRSTEj = FALSE; }
	}
#endif

    CheckEjectKey();
#if (USE_TWO_KEY_FUNCTION==1)
    CheckPlaySwitch();
#endif
}
