/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:  powerctl.c
*
* DESCRIPTION
*
* This file contains routines to control the power of he chip.
*
* $Revision: 149 $
* $Date: 11/03/21 10:10a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globtype.h"
#include ".\common\sdram.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_buf.h"
#include ".\hif\powerctl.h"
#include ".\oem\oem_sys_clk.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_global.h"
#include ".\al\reg_sdram.h"
#include ".\common\intctl.h"
#include ".\al\reg_servo.h"
#include ".\al\REG_write_strategy.h"

volatile extern BYTE SdramRefreshInterval;
volatile extern BYTE SdramRefreshIntervalSleep;
extern BYTE gPowerEvent;        /* power event */
extern BYTE gPowerStatus;       /* current power status */

#if (EN_POWER_CONTROL == 1)
#include ".\servo\Svo_lib.h"

#if(BD_ENABLE)
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#endif

// Local functions
void AFEPowerDown();
void AFEPowerUp();
void AFEWblPowerUp();
void SystemClockNormalSpeed();
void SystemClockLowSpeed();

#if (EN_DVD_HIGH_JITTER_DETECT  == 1)
extern BYTE	HIGH_JITTER_DISC;
#endif

BYTE CurrentPowerMode = eChipResetPower;
BOOL PowerControl(BYTE PwrMode)
{
    BYTE    Temp;
    vBOOL temp_IE;


    if(PwrMode == CurrentPowerMode)
        return (PASS);

    // Protect host send standby power from sleep power.
// if power is sleep mode, don't change to standby mode.
    if((PwrMode == eStandbyPower)&&(CurrentPowerMode == eSleepPower))
        return (PASS);

    switch (PwrMode)
    {
        case eStartUpPower:
            /**************************************************************************
                Start up Power
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eStartUpPower;
            SendMsg80(SHOW_DEBUG_MSG,0x710900);     // Power Control ==> eStartUpPower

            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEPowerUp();
            WRITE_FIELD(HAL_AFE_PDRFALL, 1);        /* set power down DPD&RF, STM don't need them */
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   *///711 original

             /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))//from 720
            {
                /*PWRDWN0 Wobble ADC low speed, RF ADC low speed, Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS | HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
            /*PWRDWN0 Wobble ADC low speed, RF ADC low speed*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Sleep Wobble ADC, RF ADC */
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC | HAL_GLOBAL_SLPRFADC ); //   Sleep Wobble ADC, RF ADC,

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL//from 720

            /* Clock Stop */
            /*CLKSTP1 only enable Host and SDRAM */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP| HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 only enable peripheral module */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WSCKSTP | HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop WobbleCLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, Demod, CD/DVD Demod, BD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_DMODCKSTP |
                                                  HAL_GLOBAL_CDDMCKSTP | HAL_GLOBAL_BDDMCKSTP | HAL_GLOBAL_HDDMCKSTP);

            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Power down TBG power */

            /* Encoder PLL output nothing */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI channel.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* System Clock */
            SystemClockNormalSpeed();//from 720

            SET_INT_ENABLE_MASK(temp_IE);
            break;

        case eStandbyPower:
            /**************************************************************************
            Standby Power is the lowest Power setting when the disc is not spinning.
             In this mode, drives must respond to commands. Most PLLs will be put in
            Sleep mode, and the clocks will be stopped on most blocks.
            ***************************************************************************/
            ataPowerMode = STANDBY_MODE;
            CurrentPowerMode = eStandbyPower;
            gPowerStatus = 3; // power status = standby
            gPowerEvent = 1;
#if(EN_DVD_HIGH_JITTER_DETECT ==1)
            HIGH_JITTER_DISC |= 0x02;
#endif

            SendMsg80(SHOW_DEBUG_MSG,0x710913);     // Power Control ==> eStandbyPower

            /*Mute Power Driver first*/
            if (get_driver_state() != ALL_MUTE_ON)
            {
                SetMotorDriverState(ALL_MUTE_ON);
                svo_Var_motorDriverState = ALL_MUTE_ON;
            }

            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            //Prevent false focus drop.//from 720
            CLEAR_INT_FIELD(HAL_SRV_FNOK,0);
    #if (NEW_FDP_DETECTION == 2)
            WRITE_FIELD(HAL_SRV_DSPI6,0);
    #endif

            // Make sure LDON off.//from 720
            WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0);

            // Halt Servo PLL will cause LASC output maximum voltage.
            // It can damage laser diodes. So clear output is necessary.
            LAL_LASC_ClearLaserOutputs(); //Clear the output of all channel

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x13C0);    // Disable COLLI, Delta2, Delta1, Delta0, Thresh channels.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x07C0);    // Disable Delta2, Delta1, Delta0 and Thresh channels.
    #endif

            /*Disable function before Power halt and CLK stop*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 0 );        // HALT Servo DSP

            for (Temp = 0; Temp < 50; Temp++);                             /* Delay 50 loops */

            /* Clock Stop */
            /*CLKSTP1 only enable Host and SDRAM */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_AFECKSTP | HAL_GLOBAL_SRVCKSTP | HAL_GLOBAL_DVDCKSTP |
                                                  HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP| HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 only enable peripheral module */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WBLACKSTP | HAL_GLOBAL_WBLCKSTP | HAL_GLOBAL_WSCKSTP |
                                                  HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP | HAL_GLOBAL_HFCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop Wobble, DFE CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_WBLICKSTP | HAL_GLOBAL_WBLLCKSTP | HAL_GLOBAL_EFMCKSTP |
                                                  HAL_GLOBAL_DFEOCKSTP | HAL_GLOBAL_DFEBCKSTP | HAL_GLOBAL_DFEICKSTP |
                                                  HAL_GLOBAL_DFESCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, Demod, CD/DVD Demod, BD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_DMODCKSTP |
                                                  HAL_GLOBAL_CDDMCKSTP | HAL_GLOBAL_BDDMCKSTP | HAL_GLOBAL_HDDMCKSTP);

             /* Power Down Registers */
            /*PWRDWN0 Wobble ADC low speed, RF ADC low speed, Servo CLK PLL halt, ADC CLK PLL halt*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS | HAL_GLOBAL_HLTSVPLL | HAL_GLOBAL_HLTADPLL);

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            //WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);//711 original
            /*PWRDWN1 Disable EFM and OSCEN IO pads*///from 720
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCDIS);

            /*PWRDWN2 Sleep Wobble ADC, RF ADC, Servo ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC | HAL_GLOBAL_SLPRFADC | HAL_GLOBAL_SLPSRVADC ); //   Sleep Wobble ADC, RF ADC, and Servo ADC


            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Power down TBG power */

            /* Encoder PLL output nothing */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /*AFE block*/
            AFEPowerDown();
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   *///711 original

            /* System Clock *///from 720
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;

        case eCDReadPower:
            /**************************************************************************
             CD Read Power enables all the blocks and PLLs to allow reading of data
             from a CD disc.
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eCDReadPower;

            if (gPowerStatus != 1)//from 720
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x710902);     // Power Control ==> eCDReadPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEPowerUp();
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   *///711 original

            WRITE_FIELD(HAL_AFE_PDDPD, 0);      /* set power on DPD */

            /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))//from 720
            {
                /*PWRDWN0 Wobble ADC low speed, RF ADC low speed, Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS | HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
            /*PWRDWN0 Wobble ADC low speed,RF ADC low speed*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS|HAL_GLOBAL_RFADCLS);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Sleep Wobble ADC, Wake up RF ADC, and Servo ADC*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL//from 720

            /*CLKSTP1 Stop DVD Decoder, CD Encoder, Copy Compare CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_DVDCKSTP | HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Wobble ADC, Wobble Processor, Write Strategy, DVD Encoder, Audio, OPC CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WBLACKSTP | HAL_GLOBAL_WBLCKSTP | HAL_GLOBAL_WSCKSTP |
                                                  HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP | HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop Wobble Interface, Wobble Outer Loop, EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_WBLICKSTP | HAL_GLOBAL_WBLLCKSTP |HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, BD Demod, HD Demod CLK. Wake up Demod, CD/DVD Demod*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_BDDMCKSTP |
                                                  HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs - excluding SYSPLL */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Power down TBG power */

            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /* System Clock *///from 720
            SystemClockNormalSpeed();

            /*AFE block*///711 original
            WRITE_FIELD(HAL_AFE_PDREADWOB, 1);   /* power down read input wobble */
            WRITE_FIELD(HAL_AFE_PDINPWBL , 1);   /* power down input wobble */

            SET_INT_ENABLE_MASK(temp_IE);
            break;
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
        case eCDReadWblPower:
            /**************************************************************************
             CD Read Power enables all the blocks, Wobble and PLLs to allow reading of
             data from a CD disc.
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eCDReadWblPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x710903);     // Power Control ==> eCDReadWblPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEWblPowerUp();
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   *///711 original

            if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                WRITE_FIELD(HAL_AFE_PDDPD, 0);      /* set power on DPD */
            else
                WRITE_FIELD(HAL_AFE_PDDPD, 1);      /* set power down DPD */

            /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Wobble ADC low speed, RF ADC low speed, Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS | HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
            /*PWRDWN0 Wobble ADC low speed,RF ADC low speed*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS|HAL_GLOBAL_RFADCLS);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Wake up Wobble ADC, RF ADC, and Servo ADC*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN2, 0);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /*CLKSTP1 Stop DVD Decoder, CD Encoder, Copy Compare CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_DVDCKSTP | HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Write Strategy, DVD Encoder, Audio, OPC CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WSCKSTP | HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, BD Demod, HD Demod CLK. Wake up Demod, CD/DVD Demod*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_BDDMCKSTP |
                                                  HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs - excluding SYSPLL */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Power down TBG power */

            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /* System Clock *///from 720
            SystemClockNormalSpeed();

            /*AFE block*///711 original
            WRITE_FIELD(HAL_AFE_PDREADWOB, 0);  /* power on read input wobble */
            WRITE_FIELD(HAL_AFE_PDWRTWOB , 1);  /* set power down write input wobble */
            WRITE_FIELD(HAL_AFE_PDWPP12 , 1);   /* power down WPP input buffer */

            SET_INT_ENABLE_MASK(temp_IE);
            break;
#endif
        case eDVDReadPower:
            /**************************************************************************
             DVD Read Power enables all the blocks and PLLs to allow reading of data
             from a DVD disc.
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eDVDReadPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x710904);     // Power Control ==> eDVDReadPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEPowerUp();
            WRITE_FIELD(HAL_AFE_PDDPD, 0);          /* set power on DPD */
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   *///711 original

             /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Wobble ADC low speed, Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_SLPSYPLL | HAL_GLOBAL_RFADCLS);
            }
            else
            {
            /*PWRDWN0 Wobble ADC low speed*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Sleep Wobble ADC, Wake up RF ADC, and Servo ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC );

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /*CLKSTP1 Stop CD Encoder, CD Decoder, Copy Compare CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP| HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Wobble ADC, Wobble Processor, Write Strategy, DVD Encoder, Audio, OPC CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WBLACKSTP | HAL_GLOBAL_WBLCKSTP | HAL_GLOBAL_WSCKSTP |
                                                  HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP | HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop Wobble Interface, Wobble Outer Loop, EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_WBLICKSTP | HAL_GLOBAL_WBLLCKSTP | HAL_GLOBAL_EFMCKSTP );

            /*CLKSTP5 Stop BD Modulator, HD Modulator,  BD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_BDDMCKSTP |
                                                  HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Power down TBG power */

            /* PLLs */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1); /* set Write PLL down*/

            /* System Clock */
            SystemClockNormalSpeed();

            WRITE_FIELD(HAL_AFE_PDREADWOB, 1);   /* power down read input wobble */
            WRITE_FIELD(HAL_AFE_PDINPWBL , 1);   /* power down input wobble */

            SET_INT_ENABLE_MASK(temp_IE);
            break;
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
        case eDVDReadWblPower:
            /**************************************************************************
             DVD Read Power enables all the blocks Wobble and PLLs to allow reading of
             data from a DVD disc.
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eDVDReadWblPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x710905);     // Power Control ==> eDVDReadWblPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEWblPowerUp();
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   */
            if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                WRITE_FIELD(HAL_AFE_PDDPD, 0);      /* set power on DPD */
            else
                WRITE_FIELD(HAL_AFE_PDDPD, 1);      /* set power down DPD */
            
             /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Wobble ADC low speed, Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_SLPSYPLL | HAL_GLOBAL_RFADCLS);
            }
            else
            {
            /*PWRDWN0 Wobble ADC low speed*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Wake up Wobble ADC, RF ADC, and Servo ADC*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN2, 0);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /*CLKSTP1 Stop CD Encoder, CD Decoder, Copy Compare CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP| HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Write Strategy, DVD Encoder, Audio, OPC CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WSCKSTP | HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_EFMCKSTP );

            /*CLKSTP5 Stop BD Modulator, HD Modulator,  BD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_BDDMCKSTP |
                                                  HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs - excluding SYSPLL */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Power down TBG power */

            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /* System Clock */
            SystemClockNormalSpeed();

            WRITE_FIELD(HAL_AFE_PDREADWOB, 0);  /* power on read input wobble */
            WRITE_FIELD(HAL_AFE_PDWRTWOB , 1);  /* set power down write input wobble */
            WRITE_FIELD(HAL_AFE_PDWPP12 , 1);   /* power down WPP input buffer */

            SET_INT_ENABLE_MASK(temp_IE);
            break;
#endif
#if (AUDIO_PLAYBACK == 1)
        case ePlayAudioPower:
            /**************************************************************************
            Play Audio Power enables all the blocks and PLLs to allow reading of data
             from a CD disc and Playing it back through the Audio DAC. This assumes
             that the previous power state is already Active.
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = ePlayAudioPower;
            WRITE_FIELD(HAL_GLOBAL_AUDCKSTP,0);
            SendMsg80(SHOW_DEBUG_MSG,0x710906);     // Power Control ==> ePlayAudioPower
            break;
#endif
        case eIdlePower:
            /**************************************************************************
            Idle Power is an active state with the disc spinning, full power support.
            ***************************************************************************/

            if(gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }
            CurrentPowerMode = eIdlePower;
            ataPowerMode = IDLE_MODE;

            SendMsg80(SHOW_DEBUG_MSG,0x71090A);     // Power Control ==> eIdlePower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEWblPowerUp();
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   */

            /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
            /*PWRDWN0  Normal Operation*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, 0);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Wake up Wobble ADC, RF ADC, and Servo ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, 0);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /* Clock Stop */
            /*CLKSTP1  enable all CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, 0);

            /*CLKSTP2  enable all CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, 0);

            /*CLKSTP3  enable all CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, 0);

            /*CLKSTP4  enable all CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, 0);

            /*CLKSTP5  enable all CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, 0);

            /*Enable function after Power On and CLK enable*/
            if (ServoPllInited == TRUE)
            {
                WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif                
            }

            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,1);                            /* Turn On TBG power */

            /* Encoder PLL On */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 0);     /* Set Write PLL power on */

            /* System Clock */
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;

        case eSleepPower:
            /**************************************************************************
            Sleep Power is induced by an ATA command only. This is the lowest power
            state possible that still allows the device to respond to HRST, SRST, and
            Device Reset. No data must be retained.
            ***************************************************************************/

            SendMsg80(SHOW_DEBUG_MSG,0x71090B);     // Power Control ==> eSleepPower
            if(Ata_sleep_wait)
            {
                temp_IE = GET_INT_ENABLE_MASK();
                DISABLE_INTERRUPT();

                CurrentPowerMode = eSleepPower;
                ataPowerMode = SLEEP_MODE;

                //from 720
                WRITE_FIELD(HAL_SDRAM_REFINTV, SdramRefreshIntervalSleep);    // Set SDRAM Refresh Interval to "sleep" value
                for (Temp = 0; Temp < 50; Temp++);  /* Delay 50 loops */

                WRITE_FIELD(HAL_GLOBAL_SLPSYPLL,1);                            /* sleep system PLL */

                WRITE_FIELD(HAL_GLOBAL_HLTSYPLL, 1);/* Halt system PLL */

                SET_INT_ENABLE_MASK(temp_IE);
            }
            Ata_sleep_wait = FALSE;
            break;

        case eBDReadPower:
            /**************************************************************************
                BD Read Power
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eBDReadPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x71090C);     // Power Control ==> eBDReadPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEPowerUp();
            WRITE_FIELD(HAL_AFE_PDDPD, 0);          /* set power on DPD */
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   */

            /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
                /*PWRDWN0 Normal Operation*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, 0);//711 original set wobadc low speed
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Sleep Wobble ADC, wake up RF ADC, Servo ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /* Clock Stop */
            /*CLKSTP1 stop DVD Decoder, CD Encoder, CD Decoder, Copy Compare CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_DVDCKSTP | HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP |
                                                  HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Wobble ADC, Wobble Processor, Write Strategy, DVD Encoder, Audio, OPC CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WBLACKSTP | HAL_GLOBAL_WBLCKSTP | HAL_GLOBAL_WSCKSTP |
                                                  HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP | HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD Serializer BD Encoder and LS module CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop Wobble Interface, Wobble Outer Loop, EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_WBLICKSTP | HAL_GLOBAL_WBLLCKSTP | HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, Demod, CD/DVD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_CDDMCKSTP |
                                                  HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Turn Off TBG power */

            /* Encoder PLL output nothing */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /* System Clock */
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;

        case eBDReadWblPower:
            /**************************************************************************
                BD Read Wobble Power
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eBDReadWblPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x71090D);     // Power Control ==> eBDReadWblPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEWblPowerUp();
            //WRITE_FIELD(HAL_AFE_PDDPD, 1);        /* set power off DPD */
            WRITE_FIELD(HAL_AFE_PDEQBUF , 1);    /* set power down EQDOP&N Buffer   */

            /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
                /*PWRDWN0 Normal Operation*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, 0);//711 original set wbladc low speed
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Wake up Wobble ADC, RF ADC, and Servo ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, 0);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /* Clock Stop */
            /*CLKSTP1 stop DVD Decoder, CD Encoder, CD Decoder, Copy Compare CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_DVDCKSTP | HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP |
                                                  HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 stop Write Strategy, DVD Encoder, Audio, OPC CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WSCKSTP | HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD Serializer BD Encoder and LS module CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, Demod, CD/DVD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_CDDMCKSTP |
                                                  HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Turn Off TBG power */

            /* Encoder PLL output On */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1); /* set Write PLL power down */

            /* System Clock */
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;

    #if (ENABLE_HDDVD == 1)
        case eHDReadPower:
            /**************************************************************************
                HD Read Power
            ***************************************************************************/

            ataPowerMode = STANDBY_MODE;
            CurrentPowerMode = eHDReadPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x71090F);     // Power Control ==> eHDReadPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEPowerUp();

             /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
                /*PWRDWN0 Normal Operation*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, 0);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Sleep Wobble ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC );

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /* Clock Stop */
            /*CLKSTP1 Stop CD Encoder, CD Decoder, Copy compare CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP| HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Write Strategy, DVD Encoder, Audio, OPC CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WSCKSTP | HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, CD/DVD Demod, BD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_CDDMCKSTP |
                                                  HAL_GLOBAL_BDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Turn Off TBG power */

            /* Encoder PLL output nothing */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* set Write PLL power down */

            /* System Clock */
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;

        case eHDReadWblPower:
            /**************************************************************************
                HD Read Wobble Power
            ***************************************************************************/

            ataPowerMode = STANDBY_MODE;
            CurrentPowerMode = eHDReadWblPower;

            if (gPowerStatus != 1)
            {
                gPowerEvent = 1;
                gPowerStatus = 1; // power status = active
            }

            SendMsg80(SHOW_DEBUG_MSG,0x710910);     // Power Control ==> eHDReadWblPower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEWblPowerUp();

             /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
            /*PWRDWN0 Normal Operation*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, 0);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Wake up Wobble ADC, RF ADC, and Servo ADC*/
            WRITE_REG( HAL_GLOBALREG_MAP->PWRDWN2, 0 );

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /* Clock Stop */
            /*CLKSTP1 Stop CD Encoder, CD Decoder, Copy compare CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP| HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Write Strategy, DVD Encoder, Audio, OPC CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WSCKSTP | HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD and LS module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP | HAL_GLOBAL_LSCKSTP);

            /*CLKSTP4 Stop EFM CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_EFMCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, CD/DVD Demod, BD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_CDDMCKSTP |
                                                  HAL_GLOBAL_BDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            /* PLLs */
            WRITE_FIELD(HAL_WS_TBGPWR,0);   /* Turn Off TBG power */

            /* Encoder PLL output nothing */
            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /* System Clock */
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;


    #endif  // #if (ENABLE_HDDVD == 1)

#if (EN_LS == 1)
        case eLSWritePower:
            /**************************************************************************
            Write DVD Power enables all the blocks and PLLs to allow writing of data
            to a Light Scribe disc.
            ***************************************************************************/

            ataPowerMode = IDLE_MODE;
            CurrentPowerMode = eLSWritePower;

            SendMsg80(SHOW_DEBUG_MSG,0x710912);     // Power Control ==> eLSWritePower
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /* Enable AFE Servo Block first */
            AFEPowerUp();

            /* Power Down Registers */
            if (READ_FIELD(HAL_GLOBAL_HLTSYPLL))
            {
                /*PWRDWN0 Wobble ADC low speed, RF ADC low speed, Sleep system PLL*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS | HAL_GLOBAL_SLPSYPLL);
            }
            else
            {
                /*PWRDWN0 Wobble ADC low speed, RF ADC low speed*/
                WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, HAL_GLOBAL_WBLADCLS | HAL_GLOBAL_RFADCLS);
            }

            /*PWRDWN1 Enable EFM and OSCEN IO pads*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);

            /*PWRDWN2 Sleep Wobble and RF ADC*/
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN2, HAL_GLOBAL_SLPWBLADC | HAL_GLOBAL_SLPRFADC);

            // Wait for PLLs being stable.
            DelaymS(1);

            WRITE_FIELD(HAL_GLOBAL_SLPSYPLL, 0);    // Wake up system PLL

            /* Clock Stop */
            /*CLKSTP1 stop CD Encoder, CD Decoder, Copy Compare CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, HAL_GLOBAL_DVDCKSTP | HAL_GLOBAL_ENCCKSTP | HAL_GLOBAL_DECCKSTP |
                                                  HAL_GLOBAL_CPYCKSTP);

            /*CLKSTP2 Stop Write Strategy, DVD Encoder, Audio, HF, OPC CLK */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, HAL_GLOBAL_WBLACKSTP | HAL_GLOBAL_WBLCKSTP | HAL_GLOBAL_WSCKSTP |
                                                  HAL_GLOBAL_DVDECKSTP | HAL_GLOBAL_AUDCKSTP | HAL_GLOBAL_HFCKSTP |
                                                  HAL_GLOBAL_OPCCKSTP);

            /*CLKSTP3 Disable BD module*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, HAL_GLOBAL_BDSCKSTP | HAL_GLOBAL_BDECKSTP | HAL_GLOBAL_BDDCKSTP |
                                                  HAL_GLOBAL_BDCCKSTP | HAL_GLOBAL_BDCKSTP);

            /*CLKSTP4 Stop EFM, DFE CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, HAL_GLOBAL_WBLICKSTP | HAL_GLOBAL_WBLLCKSTP | HAL_GLOBAL_EFMCKSTP |
                                                  HAL_GLOBAL_DFEOCKSTP | HAL_GLOBAL_DFEBCKSTP | HAL_GLOBAL_DFESCKSTP);

            /*CLKSTP5 Stop BD Modulator, HD Modulator, Demod, CD/DVD Demod, BD Demod, HD Demod CLK*/
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, HAL_GLOBAL_BDMCKSTP | HAL_GLOBAL_HDMCKSTP | HAL_GLOBAL_DMODCKSTP |
                                                  HAL_GLOBAL_CDDMCKSTP | HAL_GLOBAL_BDDMCKSTP | HAL_GLOBAL_HDDMCKSTP);

            /*Enable function after Power On and CLK enable*/
            WRITE_FIELD ( HAL_SRV_DSPSTART , 1 );        // START Servo DSP

    #if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_SRV_PDMOENB,0x1000);    // Enable all PDM channels, except COLLI.
    #else
            WRITE_FIELD(HAL_SRV_PDMOENB,0x0000);    // Enable all PDM channels.
    #endif

            WRITE_FIELD(HAL_WS_TBGPWR, 0);          /* Power down TBG power */

            WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);     /* Set Write PLL power down */

            /* System Clock */
            SystemClockNormalSpeed();

            SET_INT_ENABLE_MASK(temp_IE);
            break;
#endif // End of (EN_LS == 1)

        default:
            return (FAIL);
    }

/*** Delay to allow PLLs to stabalize ***/
    for (Temp = 0; Temp < 150; Temp++);                             /* Delay 150 loops(~200usec) */

#if 0
    send_msg5SValue(1,"PWRDWN0",1,READ_REG(HAL_GLOBALREG_MAP->PWRDWN0));
    send_msg5SValue(1,"PWRDWN1",1,READ_REG(HAL_GLOBALREG_MAP->PWRDWN1));
    send_msg5SValue(1,"PWRDWN2",1,READ_REG(HAL_GLOBALREG_MAP->PWRDWN2));

    send_msg5SValue(1,"CLKSTP1",1,READ_REG(HAL_GLOBALREG_MAP->CLKSTP1));
    send_msg5SValue(1,"CLKSTP2",1,READ_REG(HAL_GLOBALREG_MAP->CLKSTP2));
    send_msg5SValue(1,"CLKSTP3",1,READ_REG(HAL_GLOBALREG_MAP->CLKSTP3));
    send_msg5SValue(1,"CLKSTP4",1,READ_REG(HAL_GLOBALREG_MAP->CLKSTP4));
    send_msg5SValue(1,"CLKSTP5",1,READ_REG(HAL_GLOBALREG_MAP->CLKSTP5));

    send_msg5SValue(1,"WS__TBG",1,READ_FIELD(HAL_WS_TBGPWR));
    send_msg5SValue(1,"WPLLPWD",1,READ_FIELD(HAL_GLOBAL_WPLLPWD));
#endif
    return (PASS);
}

/*-----------------------------------------------------------------------------
 *
 * Module: SetDiscReadPowerMode()
 * Description:
 *
 *
 *----------------------------------------------------------------------------*/
void SetDiscReadPowerMode(void)
{
    if (St_no_disc)
        return;

    switch(pdbDiscLoaded())
    {
        case CD_DISC:
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (pdbServoGetDiscType() != eNonATIPDisc)
            {
                PowerControl(eCDReadWblPower);
            }
            else
#endif
            {
                PowerControl(eCDReadPower);
            }
            break;

        case DVD_DISC:
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    #if (DVD_RAM_READ == 1)
            if ((pdbServoGetDiscType() != eNonADIPLPPDisc) || (DVDRAM))
    #else
            if (pdbServoGetDiscType() != eNonADIPLPPDisc)
    #endif
            {
                PowerControl(eDVDReadWblPower);
            }
            else
#endif
            {
    #if (DVD_RAM_READ == 1)                
                if(pdbGetDiscKind() == eDVDRAMDisc)
                    PowerControl(eDVDReadWblPower);
                else
    #endif                
                    PowerControl(eDVDReadPower);
            }
            break;
#if(BD_ENABLE)
        case BD_DISC:
            if (pdbGetBDType() == BD_ROM)
                PowerControl(eBDReadPower);
            else
                PowerControl(eBDReadWblPower);
            break;
#endif
#if (ENABLE_HDDVD)
        case HD_DISC:
            if (pdbGetHDType() == HDDVD_ROM)
                PowerControl(eHDReadPower);
            else
                PowerControl(eHDReadWblPower);
            break;
#endif
        default:
            PowerControl(eIdlePower);
            break;
    }
}
#endif


/*************************************************/
/* Power Control Functions from svo_sta.c        */
/*************************************************/

void vWakeUpChipOnlyADDA(void)
{
    WRITE_FIELD(HAL_GLOBAL_SLPWBLADC, 0);
    WRITE_FIELD(HAL_GLOBAL_SLPRFADC,  0);
    WRITE_FIELD(HAL_GLOBAL_SLPSRVADC, 0);
    // Note! No Analog DACs in 6230
}

void vWakeUpFromPowerSaveMode(void)
{
    WRITE_FIELD(HAL_AFE_PDRFALL,   0);   // Power Up all Circuits in AFE RF & DPD Blocks.
    WRITE_FIELD(HAL_AFE_PDRF,      0);   // TBD? COMMENT! this line can possibly be removed, function may be covered with line above. Power Up the whole AFE RF  Block.
    WRITE_FIELD(HAL_AFE_PDDPD,     0);   // TBD? COMMENT! this line can possibly be removed, function may be covered with line above. Power Up the whole AFE DPD Block.

    vWakeUpChipOnlyADDA();
    WRITE_FIELD(HAL_GLOBAL_DVDCKSTP, 0);
    WRITE_FIELD(HAL_GLOBAL_ENCCKSTP, 0);
    WRITE_FIELD(HAL_GLOBAL_DECCKSTP, 0);
    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0);
}



#if (EN_POWER_CONTROL == 1)
/*************************************************/
/* Power Control Functions from svo_afe.c        */
/*************************************************/
/*-----------------------------------------------------------------------------
 *
 * Module: AFEPowerDown()
 * Description: Global Power Down of the AFE block
 *
 *----------------------------------------------------------------------------*/
void AFEPowerDown(void)
{
    WRITE_FIELD(HAL_AFE_PDWOBALL, 1);       /* set power down input wobble     */
    WRITE_FIELD(HAL_AFE_PDWOB,    1);       /* set power down wobble porcessor */
    WRITE_FIELD(HAL_AFE_PDSVOALL, 1);       /* set power down servo block      */
    WRITE_FIELD(HAL_AFE_PDRFALL,  1);       /* set power down DPD&RF           */
    WRITE_FIELD(HAL_AFE_PDAPCALL, 1);       /* set power down APC block        */
    //WRITE_FIELD(HAL_AFE_PDFVREF, 1);      /* set power down FVREF            */
    WRITE_FIELD(HAL_AFE_PDVREF06, 1);       /* set power down VREF06           */
}

/*-----------------------------------------------------------------------------
 *
 * Module: AFEPowerUp()
 * Description: Global Power Up of the AFE block
 *
 *----------------------------------------------------------------------------*/
void AFEPowerUp(void)
{
    WRITE_FIELD(HAL_AFE_PDWOBALL, 1);       /* set power down input wobble     */
    WRITE_FIELD(HAL_AFE_PDWOB,    1);       /* set power down wobble porcessor */
    WRITE_FIELD(HAL_AFE_PDRFALL,  0);       /* set power on DPD&RF             */
    WRITE_FIELD(HAL_AFE_PDAPCALL, 0);       /* set power on APC block          */
    //WRITE_FIELD(HAL_AFE_PDFVREF, 0);      /* set power on FVREF              */
    WRITE_FIELD(HAL_AFE_PDVREF06, 0);       /* set power on VREF06             */
    WRITE_FIELD(HAL_AFE_PDSVOALL, 0);       /* set power on servo block        */
}

/*-----------------------------------------------------------------------------
 *
 * Module: AFEWblPowerUp()
 * Description: Global Power Up of the AFE block
 *
 *----------------------------------------------------------------------------*/
void AFEWblPowerUp(void)
{
    WRITE_FIELD(HAL_AFE_PDWOBALL, 0);       /* set power on input wobble       */
    WRITE_FIELD(HAL_AFE_PDWOB,    0);       /* set power on wobble porcessor   */
    WRITE_FIELD(HAL_AFE_PDRFALL,  0);       /* set power on DPD&RF             */
    WRITE_FIELD(HAL_AFE_PDAPCALL, 0);       /* set power on APC block          */
    //WRITE_FIELD(HAL_AFE_PDFVREF, 0);      /* set power on FVREF              */
    WRITE_FIELD(HAL_AFE_PDVREF06, 0);       /* set power on VREF06             */
    WRITE_FIELD(HAL_AFE_PDSVOALL, 0);       /* set power on servo block        */
}

/*-----------------------------------------------------------------------------
 *
 * Module: SystemClockNormalSpeed()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void SystemClockNormalSpeed(void)
{
    int Delay;

    if (READ_FIELD(HAL_GLOBAL_SYSPLLK) == PLLCOK) return;

    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG0, PLLCFG0);          // Setup parameters for SYSPLL
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG1, PLLCFG1);          //
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG2, PLLCFG2);          //
    WRITE_FIELD(HAL_GLOBAL_SYSPLLN, PLLCON);                    //
    WRITE_FIELD(HAL_GLOBAL_SYSPLLM, PLLCOM);                    //
    WRITE_FIELD(HAL_GLOBAL_SYSPLLK, PLLCOK);                    //

    WRITE_FIELD(HAL_APB_UART_DLAB, 1);                          // Enable Divisor loading
    WRITE_FIELD(HAL_APB_UART_UARTDLL, BAUD_DIV&0xFF);           // Load Divisor Low Byte
    WRITE_FIELD(HAL_APB_UART_UARTDLH, BAUD_DIV>>8);             // Load Divisor High Byte
    WRITE_FIELD(HAL_APB_UART_DLAB, 0);                          // Disable Divisor loading

    for (Delay = 0; Delay < 200; Delay++){ }                    // Delay 200 instructions to allow SYSCLK to stablize at Frequency

    WRITE_FIELD(HAL_SDRAM_REFINTV, SdramRefreshInterval);       // Set SDRAM Refresh Interval to non-"sleep" value
}

/*-----------------------------------------------------------------------------
 *
 * Module: SystemClockLowSpeed()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#define SYSPLLCFG0_LOW                  0x81                    // QP Level = 16
#define SYSPLLCFG1_LOW                  0x57                    // KO = 1, Rset = 2, QP Range = 7
#define SYSPLLCFG2_LOW                  0x00                    // Disable output buffer
#define SYSPLLN_LOW                     0x54                    // F = 25 * (84+1)/(5+1)/2^4 = 22.1354 (MHz)
#define SYSPLLM_LOW                     0x05                    //
#define SYSPLLK_LOW                     0x04                    //
    #if (UART_BAUD_RATE == 57600)
#define BAUD_DIV_LOW                    6                       // +0.077%
    #elif (UART_BAUD_RATE == 115200)
#define BAUD_DIV_LOW                    3                       // +0.077%
    #elif (UART_BAUD_RATE == 345600)
#define BAUD_DIV_LOW                    1                       // +0.077%
    #endif

void SystemClockLowSpeed(void)
{
    int Delay;

    WRITE_FIELD(HAL_SDRAM_REFINTV, SdramRefreshIntervalSleep);  // Set SDRAM Refresh Interval to "sleep" value

    for (Delay = 0; Delay < 200; Delay++){ }                    // Delay 200 instructions to allow refresh-interval to be reset.

    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG0, SYSPLLCFG0_LOW);   // Setup parameters for SYSPLL
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG1, SYSPLLCFG1_LOW);   //
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG2, SYSPLLCFG2_LOW);   //
    WRITE_FIELD(HAL_GLOBAL_SYSPLLM, SYSPLLM_LOW);               // Set M first to avoid short-period too-high frequency.
    WRITE_FIELD(HAL_GLOBAL_SYSPLLN, SYSPLLN_LOW);               //
    WRITE_FIELD(HAL_GLOBAL_SYSPLLK, SYSPLLK_LOW);               //

    WRITE_FIELD(HAL_APB_UART_DLAB, 1);                          // Enable Divisor loading
    WRITE_FIELD(HAL_APB_UART_UARTDLL, BAUD_DIV_LOW&0xFF);       // Load Divisor Low Byte
    WRITE_FIELD(HAL_APB_UART_UARTDLH, BAUD_DIV_LOW>>8);         // Load Divisor High Byte
    WRITE_FIELD(HAL_APB_UART_DLAB, 0);                          // Disable Divisor loading

    for (Delay = 0; Delay < 200; Delay++){ }                    // Delay 200 instructions to allow SYSCLK to stablize at Frequency
}

/***************************************************/
/* Power Control Functions for Data Recovery Block */
/***************************************************/

#endif // EN_POWER_CONTROL
