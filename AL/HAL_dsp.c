/* ******************************************************************************************
*
*    (c) Copyright 2006 - 2007 Sunext Design, INC.
*                  All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*   HAL_dsp.c
*
*  Description:
*   This file contains functions for the DSP BIST and NORMAL execution
*   the following:
*       HAL_DSP_ClockInit
*       HAL_DSP_CoefRAMInit
*       HAL_DSP_Load
*       HAL_DSP_BISTPass
*       HAL_DSP_TachoMultEnable
*       HAL_DSP_TachoMultConfig
*       HAL_DSP_TachoMultReset
*       HAL_DSP_SRCConfig
*       HAL_DSP_SRCEnable
*       HAL_DSP_SRCHold
*       HAL_DSP_ACTDISSConfig
*       HAL_DSP_ACTDISSReset
*       HAL_DSP_SetDebug
*       HAL_DSP_SetDebugMode
*       HAL_DSP_SetDebugForce
*       HAL_DSP_ClearSRCMemory
* * Notes:
*
*   $Revision: 78 $
*   $Date: 11/03/11 2:32p $
*
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\macros.h"
#include ".\common\DAC.h"
#include ".\common\Timer.h"
#include ".\common\ComMath.h"

#include ".\servo\svo_dsp.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_lib.h"

#include ".\AL\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_ssi.h"
#include ".\AL\HAL_dsp.h"
#include ".\AL\HAL_lasc.h"

USHORT FocusLockAidTable[2][eDiscLast+1][eFCSLockAidLast+1] =
{
    { //FOCUS SEARCH
        FCS_VEL_FS_UNKNOWN,
        FCS_VEL_FS_CD,
        FCS_VEL_FS_DVD,
        FCS_VEL_FS_LS,
        FCS_VEL_FS_BD,
        FCS_VEL_FS_HD
    },
    { //LAYER JUMP
        FCS_VEL_LJ_UNKNOWN,
        FCS_VEL_LJ_CD,
        FCS_VEL_LJ_DVD,
        FCS_VEL_LJ_LS,
        FCS_VEL_LJ_BD,
        FCS_VEL_LJ_HD
    }
};

/*** Local macros ***/
#define FCS_VEL_THRESH(DStd)        FocusLockAidTable[FokStatusOK()][DStd][eFCSLockAidThr]
#if (FOO_INVERSION == 0)
#define FCS_VEL_GAIN_UP(DStd)       -FocusLockAidTable[FokStatusOK()][DStd][eFCSLockAidGn]
#define FCS_VEL_GAIN_DOWN(DStd)     FocusLockAidTable[FokStatusOK()][DStd][eFCSLockAidGn]
#else
#define FCS_VEL_GAIN_UP(DStd)       FocusLockAidTable[FokStatusOK()][DStd][eFCSLockAidGn]
#define FCS_VEL_GAIN_DOWN(DStd)     -FocusLockAidTable[FokStatusOK()][DStd][eFCSLockAidGn]
#endif

/*** Local defines ***/
#define SRV_CLK_SRC_SIZE            2
#define DSP_PWMODE_NUM              4
#define SRV_PLL_CFG_TBL             8
#define DSP_CODE_BASE_ADDRESS       HAL_SRVDSP_BaseAddress

#define DSP_PHASE_NUM               8


/*** Local static variables ***/

/*** global variables ***/

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_DSP_Init

    Description:    Initialization of the DSP module/block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_Init(void)
{
#if (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SVSTOP, 1);
#else
    WRITE_FIELD(HAL_APB_SSI_SVSTOP, 0);
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_ClockInit
    Description:
        Prior to loading the DSP code, this function initializes the servo clock frequencies.
        These clock frequencies are depending on the desired power mode.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_ClockInit(void)
{
//    BYTE Pwmode;

    //Get DSP power mode, normally from DSP code
//    Pwmode=DSP_POWER_MODE;

    //Set DSP Power mode
    WRITE_FIELD(HAL_SRV_PWMODE, DSP_POWER_MODE);
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_CoefRAMInit
    Description:
        This function loads an initial "boot-up" version of the Coef RAM, depending on the
        selected configuration. This routine must be executed before the DSP code is running.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_CoefRAMInit(BYTE DspMode)
{
//ULONG CoefValue;
    ULONG i;
#ifdef JTT
    USHORT mux_1,mux_2,mux_3,bpt,tst_x,tst_y;
#endif

 //select coef RAM for initialization
    WRITE_FIELD(HAL_SRV_DSPLOAD, 1);  //workaround for ARM hang issue during loading coef
    WRITE_FIELD(HAL_SRV_COEFSEL, 1);
    WRITE_FIELD(HAL_SRV_DSPSTART, 0); //reset DSP: otherwise, it can corrupt coef ram init

#ifdef JTT
    mux_1 = READ_DSP_COEF(reserved_3D0);
    mux_2 = READ_DSP_COEF(reserved_3D4);
    mux_3 = READ_DSP_COEF(reserved_3D8);
    bpt   = READ_DSP_COEF(reserved_3E0);
    tst_x = READ_DSP_COEF(testxx);
    tst_y = READ_DSP_COEF(testyy);
#endif

 //clean coef RAM(0x600-0xDFF)
//    for(i=0; i< MAX_COEF_RAM_SIZE; i++)
//    {
//        WRITE_REG(*(USHORT *)(COEF_RAM_BASE_ADDRESS + 4*i), 0x0000);
//    }

 //check coef RAM result
//    for(i=0; i< MAX_COEF_RAM_SIZE; i++)
//    {
//        CoefValue=READ_REG(*(vUSHORT *)(COEF_RAM_BASE_ADDRESS + 4*i));
//        if(CoefValue)    return (DSP_COEFRAM_BLANK_ERROR);
//    }

 //fill coef RAM(0x600-0xDFF) with DSP coef data
    switch (DspMode)
    {
        case DSP_NORMAL_MODE:
            for(i=0; i< MAX_COEF_RAM_SIZE; i++)
            {
                //execute filling
#if (CHIP_REV >= 0xC0)
                WRITE_DSP_COEF(4*i, Cx_ServoDSP_coef[i]);
#else
                WRITE_DSP_COEF(4*i, Bx_ServoDSP_coef[i]);
#endif
//                CoefValue=READ_DSP_COEF(4*i);
//                if(!(CoefValue==ServoDSP_coef[i]))    return (DSP_NORMAL_COEFRAM_LOAD_ERROR);
            }
            break;

        case DSP_BIST_MODE:
            for(i=0; i< BIST_COEF_INPUT_SIZE; i++)
            {
                //execute filling
#if (CHIP_REV >= 0xC0)
                WRITE_DSP_COEF(4*i, Cx_BIST_ServoDSP_input[i]);
#else
                WRITE_DSP_COEF(4*i, Bx_BIST_ServoDSP_input[i]);
#endif
//                CoefValue=READ_DSP_COEF(4*i);
//                if(!(CoefValue==ServoDSP_coef[i]))    return (DSP_BIST_COEFRAM_LOAD_ERROR);
            }

            for(i=0; i< BIST_CHECK_SIZE; i++)
            {
                WRITE_DSP_COEF(BIST_CHECK_START_ADDRESS + 4*i, 0);
//                CoefValue=READ_DSP_COEF(BIST_CHECK_SIZE+4*i);
//                if(!(CoefValue==0))    return (DSP_BIST_COEFRAM_LOAD_ERROR);
            }
            // clear loop counter address field
            WRITE_DSP_COEF(BIST_LOOP_COUNTER_ADDRESS, 0);
            break;
    }

#ifdef JTT
    WRITE_DSP_COEF(reserved_3D0,mux_1);
    WRITE_DSP_COEF(reserved_3D4,mux_2);
    WRITE_DSP_COEF(reserved_3D8,mux_3);
    WRITE_DSP_COEF(reserved_3E0,bpt  );
    WRITE_DSP_COEF(testxx,tst_x);
    WRITE_DSP_COEF(testyy,tst_y);
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_Load
    Description:
        There are 2 versions of servo DSP code that need to be loaded:
        "    DSP BIST code:    loading self-test configuration
        "    Normal DSP code:     loading normal configuration
        Prior to loading DSP code, the servo clocks need to be initialized depending on the power mode.
        This function loads and starts the DSP code, depending on the selected configuration.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_Load(BYTE DspMode)
{
    USHORT i;

 //Prepare DSP loading
    WRITE_FIELD(HAL_SRV_COEFSEL, 0); //select instruction RAM for initialization (DSP code loading)
    WRITE_FIELD(HAL_SRV_DSPSTART, 0); //reset DSP
    WRITE_FIELD(HAL_SRV_DSPLOAD, 1); // load DSP code (instruction RAM is written by ARM)

 //clean instruction RAM(0x600-0xFFF)
    for(i=0; i<MAX_DSP_INSTRUCTIONS; i++)
    {
        WRITE_REG(*(ULONG *)(DSP_CODE_BASE_ADDRESS + 4*i), 0);
    }

 //Start filling instruction RAM(0x600-0xFFF) with DSP code data
 //fill instruction RAM(0x600-0xFFF) with DSP code data
    switch (DspMode)
    {
        case DSP_NORMAL_MODE:
            for(i=0; i<MAX_DSP_INSTRUCTIONS; i++)
            {
#if (CHIP_REV >= 0xC0)
                WRITE_REG(*(ULONG *)(DSP_CODE_BASE_ADDRESS + 4*i), Cx_ServoDSP_code[i]);
#else
                WRITE_REG(*(ULONG *)(DSP_CODE_BASE_ADDRESS + 4*i), Bx_ServoDSP_code[i]);
#endif
            }
            WRITE_REG(HAL_SRVREG_MAP->PHAADDR, DSP_PHAADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHB1ADDR, DSP_PHB1ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHB2ADDR, DSP_PHB2ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC1ADDR, DSP_PHC1ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC2ADDR, DSP_PHC2ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC3ADDR, DSP_PHC3ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC4ADDR, DSP_PHC4ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHDADDR, DSP_PHDADDR);
            break;

        case DSP_BIST_MODE:
            for(i=0; i<BIST_DSP_INSTRUCTION_SIZE; i++)
            {
#if (CHIP_REV >= 0xC0)
                WRITE_REG(*(ULONG *)(DSP_CODE_BASE_ADDRESS + 4*i), Cx_BIST_ServoDSP_code[i]);
#else
                WRITE_REG(*(ULONG *)(DSP_CODE_BASE_ADDRESS + 4*i), Bx_BIST_ServoDSP_code[i]);
#endif
            }
            WRITE_REG(HAL_SRVREG_MAP->PHAADDR, BIST_DSP_PHAADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHB1ADDR, BIST_DSP_PHB1ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHB2ADDR, BIST_DSP_PHB2ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC1ADDR, BIST_DSP_PHC1ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC2ADDR, BIST_DSP_PHC2ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC3ADDR, BIST_DSP_PHC3ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHC4ADDR, BIST_DSP_PHC4ADDR);
            WRITE_REG(HAL_SRVREG_MAP->PHDADDR, BIST_DSP_PHDADDR);
            break;
    }

 //Start DSP
    WRITE_FIELD(HAL_SRV_DSPLOAD, 0);    //give DSP access to instruction RAM
    WRITE_FIELD(HAL_SRV_DSPSTART, 1);   //run DSP
    WRITE_FIELD(HAL_SRV_COEFSEL, 1);    //select coef RAM for ARM access
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_BISTPass
    Description:
        There are 2 versions of servo DSP code that need to be loaded:
        "    DSP BIST code:    loading self-test configuration
        "    Normal DSP code:     loading normal configuration
        Prior to loading DSP code, the servo clocks need to be initialized depending on the power mode.
        This function checks the result of the BIST test and will return also the PASS/FAIL status.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
BOOL HAL_DSP_BISTPass(void)
{
    BOOL   PassFlag;
    ULONG  DSPBISTCount;
    ULONG  RegValue;
    USHORT i;

    PassFlag = TRUE;
    DSPBISTCount=10000;
    Delay100uS(1); //deliberately added this delay to make BIST pass each time

 //check the BIST loop counter is not 0(time-out need to be > 25 usec)
//    while((READ_REG(*(COEF_RAM_BASE_ADDRESS + BIST_LOOP_COUNTER_ADDRESS))!=0)&&(ErrorFlag==FALSE))
    while(1)
    {
        RegValue=(USHORT)READ_DSP_COEF(BIST_LOOP_COUNTER_ADDRESS);
        if((RegValue==0)&&(PassFlag==TRUE))
        {
            DSPBISTCount--;
            if (DSPBISTCount>0)
            {
                PassFlag=TRUE;
            }
            else
            {
                PassFlag=FALSE;
                //add next line for debug
                send_msg5S(SHOW_DEBUG_MSG,"DSP BIST error: Loop count timeout");
                break;
//                return (DSP_BIST_TIMEOUT_ERROR);
            }
        }
        else
        {
            break;
        }
    }

 //read coef RAM results & compare with pre-defined BIST output pattern
    if (PassFlag==TRUE)
    {
        for(i=0;i<BIST_CHECK_SIZE;i++)
        {
            RegValue=(USHORT)READ_DSP_COEF(BIST_CHECK_START_ADDRESS + 4*i);
#if (CHIP_REV >= 0xC0)
            if (RegValue != Cx_BIST_ServoDSP_output[i])
#else
            if (RegValue != Bx_BIST_ServoDSP_output[i])
#endif
            {
                send_msg5SValue(SHOW_DEBUG_MSG,"DSP BIST error: ",1,i);
                PassFlag=FALSE;
//                return (DSP_BIST_OUTPUT_ERROR);
                break;
            }
        }
    }
    if (PassFlag==TRUE)
    {
        //add next line for debug
        send_msg5S(SHOW_DEBUG_MSG,"DSP BIST OK !");
    }
    return (PassFlag);
    //reset BIST loop counter
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_TachoMultConfig
    Description:
                This function initializes the tacho multiplier.
    Inputs:
                Tacho Mode, Sync Mode, Number of Tacho Pulses
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_TachoMultConfig(BYTE TachoMode, BYTE SyncMode, BYTE NrTachoPulses)
{
    ULONG SRCFgValue, SRCXValue;
    USHORT SRCN, SRCQ, SRCM, LocNrTachoPulses;

    WRITE_FIELD(HAL_SRV_FR2X,DISK_FR2X);

    LocNrTachoPulses = (DISK_FR2X + 1) * NrTachoPulses;

    switch(TachoMode)
    {
        case SRC_TACHO_LABELFLASH:
        case SRC_TACHO_LIGHTSCRIBE:
            break;

        case SRC_TACHO_NORMAL:
            SRCN = SRC_MEMORY_DEPTH;
            SRCQ = LocNrTachoPulses-1;
            SRCM = (SRC_MEMORY_DEPTH / LocNrTachoPulses) - 1;
            WRITE_FIELD(HAL_SRV_SRCN, SRCN);
            WRITE_FIELD(HAL_SRV_SRCQ, SRCQ);
            WRITE_FIELD(HAL_SRV_SRCM, SRCM);

            SRCFgValue = 32768 / SRC_MEMORY_DEPTH;
            WRITE_DSP_COEF(src_foc_fg, SRCFgValue);
            WRITE_DSP_COEF(src_trk_fg, SRCFgValue);
            SRCXValue = 32768*2 - SRCX_CONSTANT;
            WRITE_DSP_COEF(src_x, SRCXValue);

            WRITE_FIELD(HAL_SRV_FGINTERPOL, 1);
            //WRITE_FIELD(HAL_SRV_AUTOGEAR, 0); //does not exist anymore
            //READ_MOD_WRITE(HAL_SRVREG_MAP->FGSRC, (HAL_SRV_FGINTERPOL_MASK | HAL_SRV_AUTOGEAR_MASK),
            //                                      (HAL_SRV_FGINTERPOL | ~HAL_SRV_AUTOGEAR));

            switch(SyncMode)
            {
                case SRC_SYNC_LEGACY:
                    WRITE_FIELD(HAL_SRV_FGSYNCSEL, 0);
                    //READ_MOD_WRITE(HAL_SRVREG_MAP->FGSRC, HAL_SRV_FGSYNCSEL_MASK, ~HAL_SRV_FGSYNCSEL);
                    break;

                case SRC_SYNC_NEW_1X:
                    WRITE_FIELD(HAL_SRV_FGSYNCSEL, 1);
                    WRITE_FIELD(HAL_SRV_FG3XSEL, 0);
                    //READ_MOD_WRITE(HAL_SRVREG_MAP->FGSRC, (HAL_SRV_FG3XSEL_MASK|HAL_SRV_FGSYNCSEL), (~HAL_SRV_FG3XSEL|HAL_SRV_FGSYNCSEL));
                    break;

                case SRC_SYNC_NEW_3X:
                    WRITE_FIELD(HAL_SRV_FGSYNCSEL, 1);
                    WRITE_FIELD(HAL_SRV_FG3XSEL, 1);
                    //READ_MOD_WRITE(HAL_SRVREG_MAP->FGSRC, (HAL_SRV_FG3XSEL_MASK|HAL_SRV_FGSYNCSEL), (HAL_SRV_FG3XSEL|HAL_SRV_FGSYNCSEL));
                    break;
            }
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:       HAL_DSP_TachoMultEnable
    Description:
                This routine controls the enable state of the tacho multiplier.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_TachoMultEnable(BOOL State)
{
    switch(State)
    {
        case ON:
            WRITE_FIELD(HAL_SRV_FGMULEN, 1);
            break;

        case OFF:
            WRITE_FIELD(HAL_SRV_FGMULEN, 0);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:       HAL_DSP_TachoMultReset
    Description:
                This function resets the FGCOUNT counter related to the tacho multiplier.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_TachoMultReset(BOOL State)
{
    USHORT FgCountValue;

    switch(State)
    {
        case ON:
            WRITE_FIELD(HAL_SRV_FGMULEN, 1);
            WRITE_FIELD(HAL_SRV_RSTFGCNT, 1);
            //READ_MOD_WRITE(HAL_SRVREG_MAP->FGSRC, (HAL_SRV_FGMULEN_MASK|HAL_SRV_RSTFGCNT_MASK),
            //                                      (HAL_SRV_FGMULEN|HAL_SRV_RSTFGCNT));
            break;

        case OFF:
            //for while loop issue, need to be checked feature
            while((FgCountValue=READ_FIELD(HAL_SRV_FGCOUNT))!=0); //from SRCSTA register
            WRITE_FIELD(HAL_SRV_RSTFGCNT, 0);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:       HAL_DSP_SRCConfig
    Description:
                This function configures the SRC loops according to defined strength.
                For a complete account on the SRC, please refer to DataBook2 of the SC6230.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_SRCConfig(BYTE SrcLoop, BYTE SrcStrength)
{
    ULONG SrcBalanceFactor;

    //add calculation of SrcBalanceFactor
    SrcBalanceFactor = (ULONG)((256 * SRC_MEMORY_DEPTH * SRC_BALANCE_FACTOR)/(100 * SrcStrength));
    if(SrcBalanceFactor > 32767)    SrcBalanceFactor = 32767;

    switch(SrcLoop)
    {
        case SRC_LOOP_FOCUS:
            WRITE_DSP_COEF(src_foc_tap, SrcStrength);
            WRITE_DSP_COEF(src_foc_balance, SrcBalanceFactor);
            break;

        case SRC_LOOP_TRACKING:
            WRITE_DSP_COEF(src_trk_tap, SrcStrength);
            WRITE_DSP_COEF(src_trk_balance, SrcBalanceFactor);
            break;

        case SRC_LOOP_ALL:
            WRITE_DSP_COEF(src_foc_tap, SrcStrength);
            WRITE_DSP_COEF(src_foc_balance, SrcBalanceFactor);
            WRITE_DSP_COEF(src_trk_tap, SrcStrength);
            WRITE_DSP_COEF(src_trk_balance, SrcBalanceFactor);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:       HAL_DSP_SRCEnable
    Description:
                Enables/disables the applicable SRC function.
                If reset, SRC is disabled & SRC memory is cleared.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_SRCEnable(BYTE SrcLoop, BYTE SrcStatus)
{
    BYTE    Org_FwHold;

    switch(SrcLoop)
    {
        case SRC_LOOP_FOCUS:
            switch(SrcStatus)
            {
                case SRC_STATE_OFF:
                    WRITE_FIELD(HAL_SRV_SRCFOCEN, 0);
                    break;

                case SRC_STATE_ON:
                    WRITE_FIELD(HAL_SRV_SRCFOCEN, 1);
                    break;

                case SRC_STATE_RESET:
                    if( READ_FIELD(HAL_SRV_SRCFOCEN) == 1)
                    {
                        WRITE_FIELD(HAL_SRV_SRCFOCEN, 0);
                        HAL_DSP_ClearSRCMemory(SRC_LOOP_FOCUS);
                        WRITE_FIELD(HAL_SRV_SRCFOCEN, 1);
                    }
                    else
                    {
                        HAL_DSP_ClearSRCMemory(SRC_LOOP_FOCUS);
                    }
                    break;
            }
            break;

        case SRC_LOOP_TRACKING:
            switch(SrcStatus)
            {
                case SRC_STATE_OFF:
                    WRITE_FIELD(HAL_SRV_SRCTRKEN, 0);
                    break;

                case SRC_STATE_ON:
                    WRITE_FIELD(HAL_SRV_SRCTRKEN, 1);
                    break;

                case SRC_STATE_RESET:
                    Org_FwHold = READ_FIELD(HAL_SRV_TRKFWHLD);
                    WRITE_FIELD(HAL_SRV_TRKFWHLD, 1);
                    if (READ_FIELD(HAL_SRV_SRCTRKEN) == 1)
                    {
                        WRITE_FIELD(HAL_SRV_SRCTRKEN, 0);
                        HAL_DSP_ClearSRCMemory(SRC_LOOP_TRACKING);
                        WRITE_FIELD(HAL_SRV_SRCTRKEN, 1);
                    }
                    else
                    {
                        HAL_DSP_ClearSRCMemory(SRC_LOOP_TRACKING);
                    }
                    WRITE_FIELD(HAL_SRV_TRKFWHLD, Org_FwHold);
                    break;
            }
            break;

        case SRC_LOOP_ALL:
            switch(SrcStatus)
            {
                case SRC_STATE_OFF:
                    WRITE_FIELD(HAL_SRV_SRCFOCEN, 0);
                    WRITE_FIELD(HAL_SRV_SRCTRKEN, 0);
                    break;

                case SRC_STATE_ON:
                    WRITE_FIELD(HAL_SRV_SRCFOCEN, 1);
                    WRITE_FIELD(HAL_SRV_SRCTRKEN, 1);
                    break;

                case SRC_STATE_RESET:
                    WRITE_FIELD(HAL_SRV_SRCFOCEN, 0);
                    WRITE_FIELD(HAL_SRV_SRCTRKEN, 0);
                    WRITE_FIELD(HAL_SRV_TESRCEN, 0);
                    HAL_DSP_ClearSRCMemory(SRC_LOOP_FOCUS);
                    HAL_DSP_ClearSRCMemory(SRC_LOOP_TRACKING);
                    break;
            }
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_SRCHold
    Description:
                This HAL function changes hold status of SRC function.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_SRCHold(BYTE SrcLoop, BOOL HWState, BOOL DFCState, BOOL FWState)
{
    switch(SrcLoop)
    {
        case SRC_LOOP_FOCUS:
            WRITE_FIELD(HAL_SRV_FOCHWHLD, HWState);
            WRITE_FIELD(HAL_SRV_FOCFWHLD, FWState);
            WRITE_FIELD(HAL_SRV_FOCDFCHLD, DFCState);
            break;

        case SRC_LOOP_TRACKING:
            WRITE_FIELD(HAL_SRV_TRKHWHLD, HWState);
            WRITE_FIELD(HAL_SRV_TRKFWHLD, FWState);
            WRITE_FIELD(HAL_SRV_TRKDFCHLD, DFCState);
            break;

        case SRC_LOOP_ALL:
            WRITE_FIELD(HAL_SRV_FOCHWHLD, HWState);
            WRITE_FIELD(HAL_SRV_FOCFWHLD, FWState);
            WRITE_FIELD(HAL_SRV_FOCDFCHLD, DFCState);
            WRITE_FIELD(HAL_SRV_TRKHWHLD, HWState);
            WRITE_FIELD(HAL_SRV_TRKFWHLD, FWState);
            WRITE_FIELD(HAL_SRV_TRKDFCHLD, DFCState);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_SetDebug
    Description:
                There are 4 SSI debug registers, which can be selected by the user.
                The debug board output gain of ch2,ch3 are adjustable, 1X:Gain_x_8=0x8; 2X:Gain_x_8=0x10
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_SetDebug(BYTE DebugChannel, BYTE DebugSource, USHORT Address, USHORT Gain_x_8)
{
    USHORT OrigValue,NewValue;

    if (DebugSource == DEBUG_SOURCE_RGLUE)
    {
        Address |= 0x80;
    }

    switch (DebugChannel)
    {
        case DEBUG_CH0:
        case DEBUG_CH1:
            OrigValue = (USHORT)READ_DSP_COEF(radr_acu1);
            break;

        case DEBUG_CH2:
        case DEBUG_CH3:
            OrigValue = (USHORT)READ_DSP_COEF(radr_acu2);
            break;
    }

    switch (DebugChannel)
    {
        case DEBUG_CH2:
            WRITE_DSP_COEF(debug2_gain,Gain_x_8);
        case DEBUG_CH0:
            NewValue = OrigValue & 0xFF00 | Address;
            break;

        case DEBUG_CH3:
            WRITE_DSP_COEF(debug3_gain,Gain_x_8);
        case DEBUG_CH1:
            NewValue = OrigValue & 0x00FF | (Address << 8);
            break;
    }

    if (NewValue != OrigValue)
    {
        switch (DebugChannel)
        {
            case DEBUG_CH0:
            case DEBUG_CH1:
                WRITE_DSP_COEF(radr_acu1, NewValue);
                break;

            case DEBUG_CH2:
            case DEBUG_CH3:
                WRITE_DSP_COEF(radr_acu2, NewValue);
                break;
        }
        Delay100uS(1);
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_SetDebugMode
    Description:
                Enable or disable servo DSP debug channel
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_SetDebugMode(BOOL State)
{
    switch(State)
    {
        case ON:
            WRITE_FIELD(HAL_APB_SSI_SVSTOP, 0);
            break;

        case OFF:
            WRITE_FIELD(HAL_APB_SSI_SVSTOP, 1);
            break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_SetDebugForce
    Description:
                Force Servo DSP debug channel to be the only client of SSI
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_SetDebugForce(BOOL State)
{
    switch(State)
    {
        case ON:
            WRITE_FIELD(HAL_APB_SSI_SVFORCE, 1);
            break;

        case OFF:
            WRITE_FIELD(HAL_APB_SSI_SVFORCE, 0);
            break;
    }
}

#if (EN_ADI == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_ACTDISSConfig(USHORT Time)
    Description:
                There are 2 ACTDISS measurements implemented in the servo DSP code:
                "    Focus
                "    Tracking
    Inputs: Time (ms)
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_ACTDISSConfig(void)
{
    UINT FocPwrLimitInverted, TrkPwrLimitInverted;
    LONG Tau, TimeScale;

    WRITE_FIELD(HAL_SRV_ACTDSSRST, 1); //keep all loops in reset until everything is configured properly
    FocPwrLimitInverted     = FOC_ACT_DISS_FACTOR / (PWR_FOC * FOC_ACT_R);
    TrkPwrLimitInverted     = TRK_ACT_DISS_FACTOR / (PWR_TRK * TRK_ACT_R);

    Tau=(long)(ACTDISS_TAU * DSP_FREQ_PHASED) / 1000;
    TimeScale=(long)(32767 * 32767) / Tau;
    if(Tau > 32767)    Tau = 32767;
    if(TimeScale > 32767) TimeScale = 32767;

    WRITE_DSP_COEF(foc_pwr_limit_inv, FocPwrLimitInverted);
    WRITE_DSP_COEF(trk_pwr_limit_inv, TrkPwrLimitInverted);
    WRITE_DSP_COEF(actdiss_tau, Tau);
    WRITE_DSP_COEF(actdiss_timescale, TimeScale);
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_ACTDISSReset
    Description:
                There are 2 ACTDISS loops implemented in the servo DSP code:
                "    Focus
                "    Tracking
                The programming of each loop is very comparable & therefore we will define common
                HAL functions to configure them. This routine resets all ACTDISS loops.
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_ACTDISSReset(void)
{
    ULONG   DelayTimer;

//    send_msg5S(SHOW_DEBUG_MSG, "ACT_DISS_RESET");
    StartTimer(&DelayTimer);
    WRITE_FIELD(HAL_SRV_ACTDSSRST, 1);
    while(READ_DSP_COEF(foc_actdiss_acc_msb) != 0) //This wil take max 1 44kHz cycle
    {
        if (ReadTimer(&DelayTimer) >= 100)
        {
            send_msg5S(1, "DSP_reset TOUT");
            break;
        }
    }
    WRITE_FIELD(HAL_SRV_ACTDSSRST, 0);
}

#endif

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_ClearSRCMemory
    Description:
                clear SRC memory
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_ClearSRCMemory(BYTE SrcLoop)
{
    USHORT SRCStartAddr;
    USHORT i;

    switch(SrcLoop)
    {
        case SRC_LOOP_FOCUS:
            SRCStartAddr = foc_roc_0;
            break;

        case SRC_LOOP_TRACKING:
            SRCStartAddr = trk_roc_0;
            break;

        case SRC_LOOP_TE_OFFSET:
            SRCStartAddr = te_roc_0;
            break;
        default:
            DEBUG_ASSERT(FALSE, eASSERT_ILLEGAL_SWITCH_PARAMETER);
            break;
    }

    for(i=0; i<SRC_MEMORY_DEPTH; i++)
    {
        WRITE_DSP_COEF((SRCStartAddr + 4*i), 0);
    }
}

void HAL_DSP_EnableOutputs(void)
{
    ULONG temp_value;
    temp_value = READ_REG(HAL_SRVREG_MAP->PDMCNT);
    //enable all PDM servo channels (active low)
    temp_value &= ~( COLL_DIS|ALPHA_DIS|DMO_DIS|STEP_DIS|TILT_DIS|TRO_DIS|FOO_DIS|VREF_DIS);
    //bring all PDM servo channels to low-power (active high)
    temp_value |= ( VREF_LPWR|ALPHA1_LPWR|ALPHA2_LPWR|DMO_LPWR|SLED2_LPWR|SLED1_LPWR|TILT_LPWR|TRO_LPWR|FOO_LPWR);
    WRITE_REG(HAL_SRVREG_MAP->PDMCNT, temp_value);
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_FocusLockAid
    Description: apply (abs(dFE/dT) - FCS_VEL_THRESH) * FCS_VEL_GAIN to foo during FSRCH
                 as a velocity dependent brake pulse.
    Inputs:      BOOL enable, BOOL updown
    Outputs:     void
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_FocusLockAid(BOOL enable, BOOL updown)
{
    switch(enable)
    {
        case ON:
            WRITE_DSP_COEF(fcs_vel_thresh, FCS_VEL_THRESH(mDiscKind.Bit.DiscStd));
            if (updown == UP)
            {
                WRITE_DSP_COEF(fcs_vel_gain, FCS_VEL_GAIN_UP(mDiscKind.Bit.DiscStd));
            }
            else
            {
                WRITE_DSP_COEF(fcs_vel_gain, FCS_VEL_GAIN_DOWN(mDiscKind.Bit.DiscStd));
            }
            WRITE_FIELD(HAL_SRV_FOLCKAID, 1);
            break;
        case OFF:
            WRITE_FIELD(HAL_SRV_FOLCKAID, 0);
            break;
        default:
        break;
    }
}

/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_LayerJumpSurf
    Description: Hold FOCLFG output and add FOCLFG * SRC_KF2 to FOO during FSRCH (FOCLFG must be used).
                 Must turn off during focus ramp and focus capture.
    Inputs:      BOOL enable
    Outputs:     void
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_LayerJumpSurf(BOOL enable)
{
    WRITE_FIELD(HAL_SRV_LJMPSURF, enable);
}

#if(SUN_FUNC_TEST == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_LED
    Description:
                Force Servo DSP debug channel to be the only client of SSI
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_LED(BOOL State)
{
    switch(State)
    {
        case ON:
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One);
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_One);
            break;

        case OFF:
           WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_Zero);
            break;
    }
}


/*-----------------------------------------------------------------------------------------------------
    Name:        HAL_DSP_INVERT_LED
    Description:
                Force Servo DSP debug channel to be the only client of SSI
    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
void HAL_DSP_INVERT_LED(void)
{
    switch(READ_FIELD(HAL_WS_GAINSWR))
    {
        case OFF:
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One);
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_One);
            break;

        default:
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_Zero);
            break;
    }
}
#endif






/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
