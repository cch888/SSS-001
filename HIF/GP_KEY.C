/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   gp_key.c
*
* DESCRIPTION
* This file contains command handler of ATAPI command
*
* NOTES:
*
*
* $Revision: 46 $
* $Date: 11/04/08 9:41a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\iram.h"
#include ".\common\intctl.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\hif\rd_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_key.h"
#include ".\hif\mmc.h"
#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#endif

struct TrayManagerInfo_S TrayManagerInfo;
#if EPO_SLOT_IN
BYTE   disc_inside;
BYTE   disc_outside;
#endif

/*********************************************************************
* Function:  void CheckPushTray(void)
* Description: This function checks whether tray is pushed when it is TRAY_STATE_OUT_OK.
* Exceptions:
* Input:
* Output:
*********************************************************************/
void CheckPushTray(void)
{
    if(ipOpenSW && ipCloseSW && TrayManagerInfo.TrayState == TRAY_STATE_OUT_OK)
    {
        if(TrayManagerInfo.Pushed < 0xFF)
            TrayManagerInfo.Pushed++;
    }
}

/*********************************************************************
* Function:  void CheckEjectSwitch(void)
* Description: Check eject swtich is pressed for a while. If it is true, EjectKeyInfo.PressedValid
                    wil be set to TRUE!
* Exceptions:
* Input:
* Output:
*********************************************************************/
void CheckEjectKey(void)
{
#if (SET_SLOT_IN == 1)
		if((EjectKeyInfo.Pressed)&&((ipOpenSW == 1) || (ipSW2 == 1))) //[BTC][Q36]disc in four times light and ejecy disc out abnormal
#else
    if(EjectKeyInfo.Pressed)
#endif
    {
        if(EjectKeyInfo.PressedTime == EJECT_KEY_PRESSED_VALID_TIME)
        {
            EjectKeyInfo.PressedValid = TRUE;
            EjectKeyInfo.PressedTime++;// avoid that user presses the key long time to cause EjectKey.PressedVaild always is TRUE!!!

            if(HIF.PlayerAccept != plrIdleCmd)
            {
                svoIramVar.emergencyStopFlag = TRUE;
            }
        }
        else if(EjectKeyInfo.PressedTime < EJECT_KEY_PRESSED_VALID_TIME)
        {
            EjectKeyInfo.PressedTime++;
        }

#if (LED_NUM == 1)
        ReadyLED(LED_ON);
#elif (LED_NUM == 2)
        ErrorLED(LED_ON1S);
#endif
    }
    else
    {
        EjectKeyInfo.PressedTime = 0;
    }

    EjectKeyInfo.Pressed = FALSE;
    ENABLE_EXT_INT();
}

#if (USE_TWO_KEY_FUNCTION==1)
/*********************************************************************
* CheckPlaySwitch()
*
* Description : H/W play key handler.
*********************************************************************/
void CheckPlaySwitch(void)
{
    if(Iram_close_process_num > 0 || Iram_SyncCacheExeFlag == TRUE
        || Iram_erase_process_num > 0 || Iram_format_process_num > 0
        || Iram_implied_synccache_num > 0 || Iram_write_command_count > 0
        || Iram_repair_process_num > 0)
    {
#if (LED_NUM == 1)
        ReadyLED(LED_ON);
#endif /* (LED_NUM == 1) */

#if (LED_NUM == 2)
        ErrorLED(LED_ON1S);
#endif /* LED_NUM == 2 */

        return;
             }

    if (ipPLAY == 0)
    {
        if (Iram_PlayPressdTM < 255)   /* to prevent from overflow */
            Iram_PlayPressdTM++;
    }
    else
    {
        if (Iram_PlayPressdTM > PLAYSW_VALID_TIME)
        {
            gSt_play_sw = 1;
        }
        Iram_PlayPressdTM = 0;
    }
}
#endif
/*********************************************************************
* Function:  void TrayManager(void)
* Description: This function checks eject-switch, play-switch and TrayManagerInfo.Command
                    status and then determines which operation will be executed!
* Exceptions:  Recovery mechansim is only for eject-key!!
* Input:     None.
* Output:   None.
*********************************************************************/
void TrayManager(void)
{
    BYTE result, cmd, eject_key, retry_count, retry;

    if(TrayManagerInfo.Executing == TRUE)
        return;// avoid reenter!!!!!!
    else
        TrayManagerInfo.Executing = TRUE;

    retry_count = 0;

    do
    {
        //send_msg5SValue(1,"TrayMifo",4, (EjectKeyInfo.PressedValid << 24) + (TrayManagerInfo.Pushed << 16) + (TrayManagerInfo.Command << 8) + (TrayManagerInfo.TrayPreventAllowState));// for Debug

        if(TrayManagerInfo.TrayPreventAllowState & TRAY_PREVENT_ALLOW_STATE_LOCKED)
        {
            if(TrayManagerInfo.TrayState == TRAY_STATE_IN_OK)
            {
                EjectKeyInfo.PressedValid = FALSE;
                TrayManagerInfo.Command = TRAY_COMMAND_COPIED;
                break;
            }
        }

        /* The priority of TrayManagerInfo.Command is the highest:
             If TrayManagerInfo.Command was set, EjectKeyInfo.PressedValid will be igonred and cleared.
             If TrayManagerInfo.Command wasn't set, it will check EjectKeyInfo to determine whether
             TrayManagerInfo.Command will be set. */

        eject_key = EjectKeyInfo.PressedValid;

        EjectKeyInfo.PressedValid = FALSE;// clear it here to avoid pressing eject-key again without affect.

        if(TrayManagerInfo.Command == TRAY_COMMAND_COPIED)
        {// there is no new command, so check the eject-key and play-key.
            if(eject_key)
            {
                if(CheckDiscStatusForEjectKeyPressed() == TRUE)
                {
                    ataKeyIsPressed = TRUE;

       #if (SET_SLOT_IN == 1)//[BTC][Q36]check loader no disc and eject disc in
                    if((TrayManagerInfo.TrayState <= TRAY_STATE_OUT_ERROR)&&(ipCloseSW == 1)) 
       #else
                    if(TrayManagerInfo.TrayState <= TRAY_STATE_OUT_ERROR)
       #endif
                        TrayManagerInfo.Command= TRAY_COMMAND_TRAY_IN;
                    else
                        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
                }
            }
        #if (SET_SLOT_IN == 0) //[BTC][Q36]
            else if(TrayManagerInfo.Pushed >= TRAY_PUSHED_VALID_TIME)
            {// tray was pushed, so tray-in !!
                 TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
                 TrayManagerInfo.Pushed = 0;
            }
        #endif
            else
            {

        #if (SET_SLOT_IN == 1)//[BTC][Q36]check loader have disc and eject disc out
                if(((ipOpenSW == 1) && (ipSW2 == 1) && (ipCloseSW == 0)) || ((ipOpenSW == 0) && (ipSW2 == 1) && (ipCloseSW == 0))) 
                {
                    if(EjectKeyInfo.Pressed) //disc in/out continue issue
                    {
                        while(1) 
                        {
                        if(!EjectKeyInfo.Pressed)
                        break;
                        }
                        //DiscInFlag = 0;//[B10E]
                        /* slot out */
                        //Iram_eject_no = EJECT_SW_WATCH;
                        ataKeyIsPressed = TRUE;
                        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
                        EjectKeyInfo.PressedTime = 0;
                        ENABLE_EXT_INT();						
                        break;
                    }				
                }
        #endif

#if (USE_TWO_KEY_FUNCTION==1)
                if(gSt_play_sw)
                {/* Play key is pressed. */
                    if(St_door_open == TRUE)
                        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
                }
#endif
            }
        }

        retry = FALSE;
        cmd = TrayManagerInfo.Command;// TrayManagerInfo.Command may be changed in other Process, so copy it!!!
        TrayManagerInfo.Command = TRAY_COMMAND_COPIED;

        if(cmd != TRAY_COMMAND_COPIED)
        {
#if EN_POWER_CONTROL
            PowerControl(eIdlePower);
#endif
            StopDecoder();

            svoIramVar.emergencyStopFlag = TRUE;
            while (Iram_Svr_player_mode != STOP_MODE )
            {
                ExitProcess();
                if(St_become_ready != TRUE)
                {
                    while (SIF.ServoExecutionState != Ready )
                        ExitProcess();
                    break;
                }
            }
            svoIramVar.emergencyStopFlag = FALSE;

            if(cmd == TRAY_COMMAND_TRAY_OUT)
            {
                TrayManagerInfo.EjectRequestEvent = FALSE;
                TrayManagerInfo.TrayState = TRAY_STATE_OUT_EXECUTING;
                result = ServoTrayOut();
            }
            else
            {
                TrayManagerInfo.TrayState = TRAY_STATE_IN_EXECUTING;
                result = ServoTrayIn();
            }

            //send_msg1('$', 'T', result, retry_count);// for debug
            //send_msg1(svoIramVar.emergencyStopFlag, cmd,TrayManagerInfo.Command, EjectKeyInfo.PressedValid);// for debug

            if(result == PASS)
            {
                TrayManagerInfo.TrayState--;// from TRAY_STATE_IN/OUT_EXECUTING to TRAY_STATE_IN_OK or TRAY_STATE_OUT_OK

#if (ENABLE_SATA_FE == 1)	/* Notify host that media change happens */
                if(cmd == TRAY_COMMAND_TRAY_OUT)
                {
                    if((St_cmd_inproc == FALSE) && (SATA_FE & bASYNNOTI))
                    {

                        if (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL) || SATA_intf_pwr_state&(bIPMACK_P|bIPMACK_S))
                        {
                            SATA_intf_pwr_state = 0;
                            WRITE_FIELD(HAL_HST_DWCLKON, 1);
                            WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
                            DelaymS(1);
                        }
                        if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                        {
                            WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0xA1);         /* Device to Host Set Device Bits FIS */
                            WRITE_FIELD(HAL_HST_NOTIFYREQ, 1);
                        }
                    }
                }
#endif
            }
            else
            {
                TrayManagerInfo.TrayState++;// from TRAY_STATE_IN/OUT_EXECUTING to TRAY_STATE_IN_ERROR or TRAY_STATE_OUT_ERROR
#if(EJECT_KEY_TRAY_RECOVERY_MAX_COUNT)
#if(PCB == 710)||(PCB == 711)||(PCB == 817)
                if(retry_count < EJECT_KEY_TRAY_RECOVERY_MAX_COUNT )
#else
                if(retry_count < EJECT_KEY_TRAY_RECOVERY_MAX_COUNT && eject_key )
#endif
                {// eject-key launched this tray-in/out and it failed, so do the recovery.
                
                    #if(SET_SLOT_IN == 1)//[BTC][Q36]disc in/out continue issue
                      if(EjectKeyInfo.Pressed) 
                      { 
                        while(1)
						{
						  if(!EjectKeyInfo.Pressed)
						  break;
						}
                      }	
                    #endif
                    retry_count++;
                    retry = TRUE;
                    #if(SET_SLOT_IN == 0) //[BTC][Q36]ejectkey retry disc in
                    EjectKeyInfo.PressedValid = 2;// simulate that eject-key was pressed!
                    #endif
                    send_msg5SValue(1,"TrayRecovery", 1, retry_count);// for debug
                }
#endif
            }
        }
		
#if (SET_SLOT_IN == 1) //[BTC][Q36] for slot in
			{	//////For slot in 
				////////Disc in Loader////////
					if((ipSW2 == 1) && (ipCloseSW == 0)) //disc in loader state
						 {
						 
						 //send_msg5S(1,"dan_1");
						  DiscInFlag = 1;
						  SwitchFlag = 2;
						  break;				
						 }
				////////No Disc////////
					if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1)) //no disc state
					{
					//send_msg5S(1,"dan_2");
						DiscInFlag = 0;
						DiscInRetryFlag = 0;
						SwitchFlag = 0;
						DiscRetryFlag = 0;
						/* Tray SW ON */
						//Iram_eject_no = EJECT_SW_WATCH;
						EjectKeyInfo.PressedTime = 0;
						ENABLE_EXT_INT();
						break;				
					}
				////////12 and 8 Disc In////////
					if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1) && ((DiscInFlag == 0) || (EjectKeyInfo.Pressed))) //disc in state
					{	
					
					//send_msg5S(1,"dan_3");
						EjectKeyInfo.PressedTime++;
						if(EjectKeyInfo.PressedTime > 5)
						{
							svoIramVar.emergencyStopFlag = TRUE;
							//Iram_eject_no = EJECT_EXECUTING;			  /* set the key in process indicator */
						if(EjectKeyInfo.Pressed) //disc in/out continue issue
							{
								while(1)
								{
									if(!EjectKeyInfo.Pressed)
										break;
								}
							}
							TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN; 				  
							DiscInFlag = 1;
							SwitchFlag = 2;
							EjectKeyInfo.PressedTime = 0;
							ENABLE_EXT_INT();
					}
						break;				
					}
					else 
					{
					if ((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1) && (DiscInFlag == 1)) //[Q36]first stage disc in protect
						{		
						
						//send_msg5S(1,"dan_4");
						TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
								DiscRetryFlag++;				
								SwitchFlag = 1;
								break;				
					}
					else if((ipSW2 == 1) && (ipCloseSW == 1) && (DiscInFlag == 1)) //second state disc in protect
					{				
					
					//send_msg5S(1,"dan_5");
						TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;				
						DiscRetryFlag++;				
						SwitchFlag = 2;
						break;				
					}			
				}
		////////Disc In Retry////////		
				if(((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1) && (DiscRetryFlag == 1) && (DiscInRetryFlag == 0))) //disc in retry state
					{			
					
					//send_msg5S(1,"dan_6");
						TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN; 				  
						DiscInFlag = 2;
						DiscInRetryFlag = 1;		   
						SwitchFlag = 2;
						break;				
					}
				else
					{
					if ((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1) && (DiscInFlag == 2)) //first stage disc in retry protect
						{				
						
						//send_msg5S(1,"dan_7");
						TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
						SwitchFlag = 1;
						break;				
						}			
		
					else if((ipSW2 == 1) && (ipCloseSW == 1) && (DiscInFlag == 2)) //second state disc in retry protect
						{					
						
						//send_msg5S(1,"dan_8");
						TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
						SwitchFlag = 2;
						break;				
						}
					}
				////////12 and 8 Drawin////////
					if((ipSW2 == 1) && (ipCloseSW == 1) && ((DiscInFlag == 1) || (DiscInFlag == 2) || (DiscInFlag == 3))) //drawin state
					{	
					
					//send_msg5S(1,"dan_9");
						EjectKeyInfo.PressedTime++;
						if(EjectKeyInfo.PressedTime > 5)
						{
							svoIramVar.emergencyStopFlag = TRUE;
							//Iram_eject_no = EJECT_EXECUTING;			  /* set the key in process indicator */
							TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN; 				 
							DiscInFlag = 0;
							SwitchFlag = 2;			
							EjectKeyInfo.PressedTime = 0;
							ENABLE_EXT_INT();
						}				
				}
					else
					{			 
						/* Tray SW ON */
						//Iram_eject_no = EJECT_SW_WATCH;
						EjectKeyInfo.PressedTime = 0;
						ENABLE_EXT_INT();
				if ((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1) && (DiscInFlag == 0)) //drawin first state disc in retry protect
				{			
				
				//send_msg5S(1,"dan_10");
					TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
					SwitchFlag = 1;
					DiscInFlag = 5;
					break;				
				}			
				else if((ipSW2 == 1) && (ipCloseSW == 1) && (DiscInFlag == 0)) //drawin second state disc in retry protect
				{	
				
				//send_msg5S(1,"dan_11");
					TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
					SwitchFlag = 2;
					DiscInFlag = 5;
					break;				
				}
		
					}
				////////No Disc Protect////////
					if((ipOpenSW == 0)&&(ipSW2 == 0)&&(ipCloseSW == 0)&&(EjectKeyInfo.Pressed)) //no disc protect
					{	
					
					//send_msg5S(1,"dan_12");
						TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
						break;				
					}
					break;
				}
		//////For slot in end
#endif
    }while(retry);

    TrayManagerInfo.Executing = FALSE;
}

