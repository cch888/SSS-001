/* ******************************************************************************************
*
*   (c) Copyright 2006 - 2007 Sunext Design, INC.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
* $Revision: 200 $
* $Date: 11/03/11 2:32p $
*
* Description:
*   This file contains functions for the Servodsp subsystem and includes
*   the following:
*          HAL_LASC_Init
*          HAL_LASC_SetLaserReadPower
*          HAL_LASC_SetLaserOn
*
* Notes:
*
*
-----------------------------------------------------------------------------------------------------*/
#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\OEM\oem_info.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_servodsp.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_afe.h"
#include ".\AL\REG_ssi.h"
#include ".\AL\HAL_lasc.h"
#include ".\AL\coefs.h"
#include ".\AL\LAL_dsp.h"
#include ".\AL\HAL_servo.h"
#include ".\common\hwdef.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_dsp.h"
#include ".\common\chip.h"
#include ".\Common\ComMath.h"
#if (DEBUG_LASER_OPEN_LOOP == 1)
#include ".\common\macros.h"
#include ".\oem\oem_tbl.h"
#endif
#include ".\servo\svoPclib.h"
#include ".\AL\HAL_dsp.h"

#include ".\Common\macros.h"
#include ".\OEM\oem_tbl.h"

#if (SUN_FUNC_TEST == 1)
#include ".\common\iram.h"
#include ".\hif\at_equ.h"
#endif

/*** Local defines ***/
#define MAX_LCTHMAX  0x7FFF          /* Max value an USHORT can have */

//Field defines of the PDMOENB field

/*** Local static variables */
#if (OPU == KEM350AAA)
static BOOL opu_eeprom_valid;
#endif
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_LASC_Init

    Description:    Initialization of the laser control block
    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_LASC_Init( void )
{
    //Set Laser Control Coef values to zero (Sequenctial from Offset0_0 to Power_15, except 7A0, 7B4
    WRITE_DSP_COEF(Offset0_0, 0x0000);
    WRITE_DSP_COEF(Offset0_1, 0x0000);
    WRITE_DSP_COEF(Offset1_0, 0x0000);
    WRITE_DSP_COEF(Offset1_1, 0x0000);
    WRITE_DSP_COEF(Offset2_0, 0x0000);
    WRITE_DSP_COEF(Offset2_1, 0x0000);

    WRITE_DSP_COEF(Pread0RS, 0x0000);
    WRITE_DSP_COEF(Pread1RS, 0x0000);
    WRITE_DSP_COEF(Pbias0RS, 0x0000);
    WRITE_DSP_COEF(Pbias1RS, 0x0000);
    WRITE_DSP_COEF(Pbias0pRS, 0x0000);
    WRITE_DSP_COEF(Pbias1pRS, 0x0000);
    WRITE_DSP_COEF(PRead0WS, 0x0000);
    WRITE_DSP_COEF(PRead1WS, 0x0000);
    WRITE_DSP_COEF(Pbias0WS, 0x0000);
    WRITE_DSP_COEF(Pbias1WS, 0x0000);
    WRITE_DSP_COEF(Pbias0pWS, 0x0000);
    WRITE_DSP_COEF(Pbias1pWS, 0x0000);

    WRITE_DSP_COEF(Gthr_read, 0x0000);
    WRITE_DSP_COEF(Gthr_write, 0x0000);
    WRITE_DSP_COEF(Gdelta_write, 0x0000);
    WRITE_DSP_COEF(Thresh_reference, 0x0000);
    WRITE_DSP_COEF(Thresh_open_loop_value, 0x0000);
    WRITE_DSP_COEF(Delta_open_loop_value, 0x0000);

    WRITE_DSP_COEF(Att0_0, 0x0000);
    WRITE_DSP_COEF(Att0_1, 0x0000);
    WRITE_DSP_COEF(Att1_0, 0x0000);
    WRITE_DSP_COEF(Att1_1, 0x0000);
    WRITE_DSP_COEF(Att2_0, 0x0000);
    WRITE_DSP_COEF(Att2_1, 0x0000);

    WRITE_DSP_COEF(thresh_P_gain, 0x0000);//7A0
    WRITE_DSP_COEF(T2D, 0x0000);         //7A4
    WRITE_DSP_COEF(power_in, 0x0000);
    WRITE_DSP_COEF(Thresh_error_msb, 0x0000);
    WRITE_DSP_COEF(Delta_error_msb, 0x0000);
//    WRITE_DSP_COEF(value_4000, 0x0000);

    WRITE_DSP_COEF(power_ref0, 0x0000);
    WRITE_DSP_COEF(power_ref1, 0x0000);
    WRITE_DSP_COEF(power_0, 0x0000);
    WRITE_DSP_COEF(power_1, 0x0000);
    WRITE_DSP_COEF(power_2, 0x0000);
    WRITE_DSP_COEF(power_3, 0x0000);
    WRITE_DSP_COEF(power_4, 0x0000);
    WRITE_DSP_COEF(power_5, 0x0000);
    WRITE_DSP_COEF(power_6, 0x0000);
    WRITE_DSP_COEF(power_7, 0x0000);
    WRITE_DSP_COEF(power_8, 0x0000);
    WRITE_DSP_COEF(power_9, 0x0000);
    WRITE_DSP_COEF(power_10, 0x0000);
    WRITE_DSP_COEF(power_11, 0x0000);
    WRITE_DSP_COEF(power_12, 0x0000);
    WRITE_DSP_COEF(power_13, 0x0000);
    WRITE_DSP_COEF(power_14, 0x0000);
    WRITE_DSP_COEF(power_15, 0x0000);

#if (OPU == KEM350AAA)
    //Put OPU in initialisation state
    //First initialize the serial interface
    WRITE_FIELD(HAL_APB_SSI_LALSB2MSB, 0);
    WRITE_FIELD(HAL_APB_SSI_LDLSB2MSB, 0);
    WRITE_FIELD(HAL_APB_SSI_LSCLKINV, 1);
    WRITE_REG(HAL_APB_SSIREG_MAP->LCDCLKDIV, 0x18);
    WRITE_FIELD(HAL_APB_SSI_LMODEPOL, 0);
    WRITE_FIELD(HAL_APB_SSI_LMODELOC, 0);
    WRITE_FIELD(HAL_APB_SSI_LDATA1ST, 0);

    //Then reset the OPU. This erases content of 0x50 to 0x70 to 0
    WriteOPU(OPU_RESET_ADDR, RESET_STATE);
    WriteOPU(OPU_RESET_ADDR, FREE_STATE);

    WriteOPU(LASER_SELECT, DEFAULT_LASER_SELECT_VALUE); //Default laser select value
    SetLCD(LCDSW_OFF);                                  //Switch LCD off

    //Simple EEPROM check
    if ((ReadOPU(ADDFACDATEL) && 0xF0) != 0x80)
    {
        opu_eeprom_valid = FALSE;
        send_msg5S(SHOW_DEBUG_MSG,"OPU-EEPROM CONTENTS WRONG");
    }
    else
    {
        opu_eeprom_valid = TRUE;
    }
#endif
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_LASC_SetReadMode

    Description:    Sets the OPU related settings for laser

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: OPU related LDD settings
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
#if (OPU == KES410A)
#define OPU_REG0  0x1000   //LCD port
#define OPU_REG1  0x1004
#define OPU_REG2  0x1008
#define OPU_REG3  0x100C
#define OPU_REG4  0x1010
#define OPU_REG5  0x1014
#define OPU_REG6  0x1018
#define OPU_REG7  0x101C
#elif (OPU == BDPL2)
#define OPU_REG0  0x1000   //LCD port
#define OPU_REG1  0x1004
#define OPU_REG2  0x1008
#define OPU_REG3  0x100C
#define OPU_REG4  0x1010
#define OPU_REG5  0x1014
#define OPU_REG6  0x1018
#define OPU_REG7  0x101C
#elif (OPU == KEM350AAA)
#define CD_freq_def   0x09
#define DVD_freq_def  0x09
#define BD_freq_def   0x09
#define CD_ampl_def   0x40
#define DVD_ampl_def  0xC0
#define BD_ampl_def   0x22
#endif
void HAL_LASC_SetReadMode(void)
{
    SHORT laser_pw = 0;
#if (OPU == KEM350AAA)
    BYTE  freq;
    BYTE  amplitude;
    BYTE  laser_select;
    BYTE  iinr_value;
    BYTE  fpd_select;
#endif

#if (OPU == KES410A)
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            //mode0 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), ((OPUBarcode.IVG_CDDVD&0x70)<<1)|PDC2|HFM_OFF|LD_CD);  /* Adress 00  */
            //mode1 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), MODEA_LOW | MODEB_HIGH |MODEC_OFF |OFST_M_DIS |OFST_R_DIS |APC_MODE);  /* Adress 01  */
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);/* Adress 02  */

         //   SetPDICGmodeB(0);//0:CD, 1:DVD, 2:BD
         //   SetPDICGmodeA(0);//only for BD , 1: high gain , 0 : low gain

            //read power setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.READPOWER_CD);  /* Adress 03  */
            //FFC setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.FFCG_FFC_CD);  /* Adress 04  */
            //HFMP setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), 0x00);  /* Adress 05  */
            //HFMF setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), 0x00);  /* Adress 06  */
            //spectrum diffusion setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), 0x00);  /* Adress 07  */
            break;

        case eDiscDVD:
            //mode0 seDVDMinusRDLDisc :
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), ((OPUBarcode.IVG_CDDVD&0x07)<<5)|PDC2|HFM_OFF|LD_DVD);  /* Adress 00  */
            //mode1 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), MODEA_LOW | MODEB_LOW | MODEC_OFF |OFST_M_DIS |OFST_R_DIS |APC_MODE);  /* Adress 01  */
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);/* Adress 02  */

        //    SetPDICGmodeB(1);//0:CD, 1:DVD, 2:BD
        //    SetPDICGmodeA(0);//only for BD , 1: high gain , 0 : low gain

            //read power setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.READPOWER_DVD);  /* Adress 03  */
            //FFC setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.FFCG_FFC_DVD);  /* Adress 04  */
            //HFMP setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), 0x00);  /* Adress 05  */
            //HFMF setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), 0x00);  /* Adress 06  */
            //spectrum diffusion setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), 0x00);  /* Adress 07  */
            break;

        case eDiscBD:
            //mode0 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), ((OPUBarcode.IVG_BD&0x07) <<5)|PDC1|HFM_ON|LD_BD);  /* Adress 00  */
            //mode1 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), MODEA_HIGH | MODEB_MIDDLE | MODEC_ON |OFST_M_DIS |OFST_R_DIS |APC_MODE);  /* Adress 01  */
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);/* Adress 02  */

     //       SetPDICGmodeB(2);//0:CD, 1:DVD, 2:BD
     //       SetPDICGmodeA(1);//only for BD , 1: high gain , 0 : low gain

            if (mDiscKind.Bit.Layers == eLayersDouble)
            {
                //read power setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.READPOWER_BDL1);  /* Adress 03  */
                //HFMP setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.HFMP_BD1DL);  /* Adress 05  */
                laser_pw = 60;
            }
            else
            {
                //read power setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.READPOWER_BDL0);  /* Adress 03  */
                //HFMP setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.HFMP_BD0SL);  /* Adress 05  */
            }

            //read power setting
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.READPOWER_BDL0);  /* Adress 03  */
            //FFC setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.FFCG_FFC_BD);  /* Adress 04  */
            //HFMP setting
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.HFMP_BD0SL);  /* Adress 05  */
            //HFMF setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), OPUBarcode.HFMF_BD);  /* Adress 06  */
            //spectrum diffusion setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), OPUBarcode.HFMF_SS_BD);  /* Adress 07  */
            break;

        default:
            send_msg5SValue(SHOW_DEBUG_MSG,"LDD setting NG",1,mDiscKind.Bit.DiscStd);
            break;
    }
#elif (OPU == BDPL2)
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            //mode0 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), OPUBarcode.CD_IV|0x01);   /* Adress 00  */
            //mode1 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), OPUBarcode.CD_IO);   /* Adress 01  */
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);             /* Adress 02  */
            //read power setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.CD_P);    /* Adress 03  */
            send_msg5SValue(1, "CD APCtarget= ", 1, (ULONG)OPUBarcode.CD_P    );
            //FFC setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.CD_IGAIN);/* Adress 04  */
            //HFMP setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.CD_HMA);  /* Adress 05  */
            //HFMF setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), OPUBarcode.CD_HMF);  /* Adress 06  */
            //spectrum diffusion setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), 0x00);               /* Adress 07  */
            break;

        case eDiscDVD:
            //mode0 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), OPUBarcode.DV_IV|0x02);   /* Adress 00  */
            //mode1 setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), OPUBarcode.DV_IO);   /* Adress 01  */
            //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);             /* Adress 02  */
            //read power setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), OPUBarcode.DV_P);    /* Adress 03  */
            send_msg5SValue(1, "DVD APCtarget= ", 1, (ULONG)OPUBarcode.DV_P    );
            //FFC setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.DV_IGAIN);/* Adress 04  */
            //HFMP setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.DV_HMA);  /* Adress 05  */
            //HFMF setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), OPUBarcode.DV_HMF);  /* Adress 06  */
            //spectrum diffusion setting
            WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), 0x00);               /* Adress 07  */
            break;

        case eDiscBD:
            if (mDiscKind.Bit.Layers == eLayersDouble)
            {
                //mode0 setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), OPUBarcode.BDDL_IV|0x03|0x04);   /* Adress 00  */
                //mode1 setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), OPUBarcode.BDDL_IO);   /* Adress 01  */
                //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);               /* Adress 02  */
                //read power setting
                //APCtarget = OPUBarcode.BDDL_PL1+(OPUBarcode.SA_ACT_BD_L0-141)*((float)(OPUBarcode.BDDL_PL0-OPUBarcode.BDDL_PL1)/(246-141));
                HAL_LASC_SetAPCTarget(((Layer_Index == 0)? OPUBarcode.SA_ACT_BD_L0 : OPUBarcode.SA_ACT_BD_L1) * 32);
                //FFC setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.BDDL_IGAIN);/* Adress 04  */
                //HFMP setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.BDDL_HMA);  /* Adress 05  */
                //HFMF setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), OPUBarcode.BDDL_HMF);  /* Adress 06  */
                //spectrum diffusion setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), 0x00);                 /* Adress 07  */
                laser_pw = 60;
            }
            else
            {
                //mode0 setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG0), OPUBarcode.BDSL_IV|0x03|0x04);   /* Adress 00  */
                //mode1 setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), OPUBarcode.BDSL_IO);   /* Adress 01  */
                //WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG2), 0x02);               /* Adress 02  */
                //read power setting
                //APCtarget = OPUBarcode.BDSL_PL1+(OPUBarcode.SA_ACT_BD_L0-141)*((float)(OPUBarcode.BDSL_PL0-OPUBarcode.BDSL_PL1)/(246-141));
                HAL_LASC_SetAPCTarget(OPUBarcode.SA_ACT_BD_L0 * 32);
                //FFC setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG4), OPUBarcode.BDSL_IGAIN);/* Adress 04  */
                //HFMP setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG5), OPUBarcode.BDSL_HMA);  /* Adress 05  */
                //HFMF setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG6), OPUBarcode.BDSL_HMF);  /* Adress 06  */
                //spectrum diffusion setting
                WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG7), 0x00);                 /* Adress 07  */
            }
            break;

        default:
            send_msg5SValue(SHOW_DEBUG_MSG,"LDD setting NG",1,mDiscKind.Bit.DiscStd);
            break;
    }
#elif (OPU == SFBD412)||(OPU == SFBD414)
    WRITE_FIELD(HAL_SRV_LASCREAD, 1);
    WRITE_DSP_COEF(Thresh_open_loop_value,0x0000);
    WRITE_DSP_COEF(Thresh_reference,0x0000);
    WRITE_FIELD(HAL_SRV_OPENTHLP,0);
    WRITE_DSP_COEF(T2D, 0x0000);
    WRITE_DSP_COEF(power_ref0, 0x0000);
    WRITE_DSP_COEF(power_ref1, 0x0000);
#if (SUN_FUNC_TEST == 1)
    if (!(Iram_Cmd_block.bf[0] == DIAG_LASER))
#endif
    {
    WRITE_DSP_COEF(Pread0RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
    WRITE_DSP_COEF(Pread1RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
    }

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
    #if USE_LDEL2_FOR_CD_LD // Use LDEL2 To Control CD LD power
            WRITE_DSP_COEF(Offset2_0,0x0000);
            WRITE_DSP_COEF(Offset2_1,0x0000);
            WRITE_DSP_COEF(Att2_0,0x7FFF);
            WRITE_DSP_COEF(Att2_1,0x7FFF);
    #else // Use LTHR To Control CD LD power
            WRITE_FIELD(HAL_SRV_LASCREAD, 0); //is resetting this bit correct ?
    #endif
    #if (LASER_DRIVER == SAC1036SP)
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);//Output2 for CD
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_Zero);
    #endif
            //Switch FPD to CD
            CDLD_EN;
    #if(OPU == SFBD414)
            WaitTimer(10);
    #endif
            DVDLD_DIS;
            DelaymS(15);
            break;

        case eDiscDVD :
            if (svoVar_bLascMode == 1)
            {
                WRITE_DSP_COEF(Pread0RS,0x2900);//0x2900->0.13mw  0x3500->0.16mw
                WRITE_DSP_COEF(Pread1RS,0x2900);
                laser_pw = -18;
            }
    #if (LASER_DRIVER == SAC1036SP)
            WRITE_DSP_COEF(Offset2_0,0x0000);
            WRITE_DSP_COEF(Offset2_1,0x0000);
            WRITE_DSP_COEF(Att2_0,0x7FFF);
            WRITE_DSP_COEF(Att2_1,0x7FFF);
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_HiZ3);//Output2 for DVD
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_HiZ3);
    #elif (LASER_DRIVER == NO_LASER_DRIVER)
            WRITE_DSP_COEF(Offset0_0,0x0000);
            WRITE_DSP_COEF(Offset0_1,0x0000);
            WRITE_DSP_COEF(Att0_0,0x7FFF);
            WRITE_DSP_COEF(Att0_1,0x7FFF);
    #endif
            //Switch FPD to DVD
            DVDLD_EN;
            CDLD_DIS;
            DelaymS(15);
            break;

        case eDiscBD :
        case eDiscHD :
    #if (LASER_DRIVER == SAC1036SP)
            WRITE_DSP_COEF(Offset2_0,0x0000);
            WRITE_DSP_COEF(Offset2_1,0x0000);
            WRITE_DSP_COEF(Att2_0,0x7FFF);
            WRITE_DSP_COEF(Att2_1,0x7FFF);
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One);//Output1 for BD
            WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_One);
    #elif (LASER_DRIVER == NO_LASER_DRIVER)
            WRITE_DSP_COEF(Offset1_0,0x0000);
            WRITE_DSP_COEF(Offset1_1,0x0000);
            WRITE_DSP_COEF(Att1_0,0x7FFF);
            WRITE_DSP_COEF(Att1_1,0x7FFF);
    #endif
            DVDLD_EN;
            CDLD_DIS;
            if (mDiscKind.Bit.Layers == eLayersDouble)
            {
                laser_pw = 60;//According Spec.: 8.4mW/4.2mW = 2 (6dB)
            }
            break;
    }
    LAL_WriteCoef(SRV_LCTHRESH_LCTHMAX);
    LAL_WriteCoef(SRV_LCDELTA0_DELTA0MAX);
    LAL_WriteCoef(SRV_LCDELTA1_DELTA1MAX);
    LAL_WriteCoef(SRV_LCDELTA2_DELTA2MAX);
    WRITE_FIELD(HAL_SRV_LPCTHOLD, 0);
    HAL_LASC_Wait_Threshold_Stable();
#elif (OPU == HOPB1300)
    #if 1 //Debug for openloop
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
            WRITE_DSP_COEF(Thresh_open_loop_value,COEF_GetValue(SRV_LCTHRESH_LCTHMAX));
            WRITE_FIELD(HAL_SRV_OPENTHLP, 1);
            //WaitTimer(500L);
            break;

        case eDiscDVD :
            WRITE_DSP_COEF(Att0_0,0x0000);
            WRITE_DSP_COEF(Att0_1,0x0000);
            WRITE_DSP_COEF(Offset0_0,0x0000);
            WRITE_DSP_COEF(Offset0_1,0x0000);
            WRITE_FIELD(HAL_SRV_OPENDLLP, 1);
            WRITE_DSP_COEF(Offset0_0,COEF_GetValue(SRV_LCDELTA0_DELTA0MAX));
            WRITE_DSP_COEF(Offset0_1,COEF_GetValue(SRV_LCDELTA0_DELTA0MAX));
            //WaitTimer(500L);
            break;

        case eDiscBD :
        case eDiscHD :
            WRITE_DSP_COEF(Att1_0,0x0000);
            WRITE_DSP_COEF(Att1_1,0x0000);
            WRITE_DSP_COEF(Offset1_0,0x0000);
            WRITE_DSP_COEF(Offset1_1,0x0000);
            WRITE_FIELD(HAL_SRV_OPENDLLP, 1);
            WRITE_DSP_COEF(Offset1_0,COEF_GetValue(SRV_LCDELTA1_DELTA1MAX));
            WRITE_DSP_COEF(Offset1_1,COEF_GetValue(SRV_LCDELTA1_DELTA1MAX));
            if (mDiscKind.Bit.Layers == eLayersDouble)
            {
                laser_pw = 60;//According Spec.: 8.4mW/4.2mW = 2 (6dB)
            }
            //WaitTimer(7000L);
            break;
    }
    #else //close loop APC control
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
            WRITE_DSP_COEF(Thresh_open_loop_value,0x0000);
            WRITE_DSP_COEF(Thresh_reference,0x4000);
            WRITE_DSP_COEF(Pread0RS,0x0000);
            WRITE_DSP_COEF(Pread1RS,0x0000);

            WRITE_FIELD(HAL_SRV_OPENTHLP, 0);
            WRITE_DSP_COEF(Pread0RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
            WRITE_DSP_COEF(Pread1RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
            break;

        case eDiscDVD :
            WRITE_DSP_COEF(Thresh_open_loop_value,0x0000);
            WRITE_DSP_COEF(Pread0RS,0x0000);
            WRITE_DSP_COEF(Pread1RS,0x0000);
            WRITE_DSP_COEF(Att0_0,0x7FFF);
            WRITE_DSP_COEF(Att0_1,0x7FFF);
            WRITE_DSP_COEF(Offset0_0,0x0000);
            WRITE_DSP_COEF(Offset0_1,0x0000);

            WRITE_DSP_COEF(T2D, 0x7FFF);
            WRITE_DSP_COEF(power_ref0, 0x7FFF);
            WRITE_DSP_COEF(power_ref1, 0x7FFF);

            WRITE_DSP_COEF(Pread0RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
            WRITE_DSP_COEF(Pread1RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
            break;

        case eDiscBD :
        case eDiscHD :
            WRITE_DSP_COEF(Thresh_open_loop_value,0x0000);
            WRITE_DSP_COEF(Pread0RS,0x0000);
            WRITE_DSP_COEF(Pread1RS,0x0000);
            WRITE_DSP_COEF(Att1_0,0x7FFF);
            WRITE_DSP_COEF(Att1_1,0x7FFF);
            WRITE_DSP_COEF(Offset1_0,0x0000);
            WRITE_DSP_COEF(Offset1_1,0x0000);

            WRITE_DSP_COEF(T2D, 0x7FFF);
            WRITE_DSP_COEF(power_ref0, 0x4000);
            WRITE_DSP_COEF(power_ref1, 0x4000);

            WRITE_DSP_COEF(Pread0RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
            WRITE_DSP_COEF(Pread1RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
            if (mDiscKind.Bit.Layers == eLayersDouble)
            {
                laser_pw = 60;//According Spec.: 8.4mW/4.2mW = 2 (6dB)
            }
            break;
    }
    #endif
#elif (OPU == DXX811)
    #if (DEBUG_LASER_OPEN_LOOP == 1)
    //if (ReadPower>=1024) ReadPower = 0;  // protection.

    //Calculate and write the setpoint
    WRITE_DSP_COEF(Pread0RS, 0x6A8);
    WRITE_DSP_COEF(Pread1RS, 0x6A8);

    //Calculate the Threshold reference
    WRITE_DSP_COEF(Thresh_reference, 0x500);

    //Set MAX and MIN , must fine tune
    WRITE_FIELD(HAL_SRV_LCTHMIN, 0x2A00);
    WRITE_FIELD(HAL_SRV_LCTHMAX, 0x2A00);

    //Set laser control gain
    WRITE_DSP_COEF(Gthr_read, 0x0080);
    //send_msg5SValue(1,"Gthr_read",4,READ_DSP_COEF(Gthr_read));

    //SSI debug function
    WRITE_DSP_COEF(radr_acu1, 0xB394);
    WRITE_DSP_COEF(radr_acu2, 0x3837);
    #endif

    //set HFM mode
    SetHFMMode(DXX811_HFM_MODE2);
    //send_msg1('S','H','F','M');
#elif (OPU == KEM350AAA)
//TBD: replace OPUSerialWrite by other function

//Program disckind dependent HFM osc frequency/amplitude
//Fetch Laser_select value, Fetch IINR value depending on discstd.
    switch (mDiscKind.Bit.DiscStd)
    {

//            for (i=0x00; i<= 0xff; i++)
//            {
//               ReadOPU(i);
//            }
        case eDiscCD :
            freq = ReadOPU(EEPROM_HFM_FREQ_CD);
            amplitude = ReadOPU(EEPROM_HFM_AMPL_CD);
            laser_select = SEL_LD_CD;
            fpd_select = FPD_LD_CD;
            iinr_value = IINR_READMODE_CD;
            if (opu_eeprom_valid == FALSE)
            {
                freq = CD_freq_def;
                amplitude = CD_ampl_def;
            }
            break;

        case eDiscDVD :

            freq = ReadOPU(EEPROM_HFM_FREQ_DVD);
            amplitude = ReadOPU(EEPROM_HFM_AMPL_DVD);
            laser_select = SEL_LD_DVD;
            fpd_select = FPD_LD_DVD;
            iinr_value = IINR_READMODE_DVD;
            if (opu_eeprom_valid == FALSE)
            {
                freq = DVD_freq_def;
                amplitude = DVD_ampl_def;
            }
            break;

        case eDiscBD :

            freq = ReadOPU(EEPROM_HFM_FREQ_BD);
            amplitude = ReadOPU(EEPROM_HFM_AMPL_BDSL);
            laser_select = SEL_LD_BD;
            fpd_select = FPD_LD_BD;
            iinr_value = IINR_READMODE_BD;
            if (opu_eeprom_valid == FALSE)
            {
                freq = BD_freq_def;
                amplitude = BD_ampl_def;
            }
            break;

        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
            break;
    }
    WriteOPU(HFM_FREQ, freq);
    WriteOPU(HFM_AMPL, amplitude);

    //Program correct laser
    WriteOPU(LASER_SELECT, (DEFAULT_LASER_SELECT_VALUE | laser_select));
    WriteOPU(FPD_SELECT, fpd_select);

    //Program bandwidth and input resistance of OPU current AMP
    WriteOPU(IINR_RES_BW, iinr_value);
    WriteOPU(IIN32_RES_BW, IIN32_READMODE);
    WriteOPU(IIN54_RES_BW, IIN54_READMODE);

    //Set Oscillator on, Set readchannel on, Set writechannels off
    #if (DEBUG_LASER_OPEN_LOOP == 1)
    WriteOPU(CHANNEL_SELECT, TEMP_CHANNEL_READ_MODE);     //Temporary
    #else
    WriteOPU(CHANNEL_SELECT, CHANNEL_READ_MODE);
    #endif

#elif (OPU == SOHBP7)
    WRITE_FIELD(HAL_SRV_LASCREAD, 1);
    WRITE_DSP_COEF(Thresh_open_loop_value,0x0000);
    WRITE_DSP_COEF(Thresh_reference,0x0000);
    WRITE_FIELD(HAL_SRV_OPENTHLP,0);
    WRITE_DSP_COEF(T2D, 0x0000);
    WRITE_DSP_COEF(power_ref0, 0x0000);
    WRITE_DSP_COEF(power_ref1, 0x0000);
    WRITE_DSP_COEF(Pread0RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));
    WRITE_DSP_COEF(Pread1RS,COEF_GetValue(SRV_CALC_C_LD_APC_Setpoint));

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
            // Use LDEL0 To Control CD LD power
            WRITE_DSP_COEF(Offset0_0,0x0000);
            WRITE_DSP_COEF(Offset0_1,0x0000);
            WRITE_DSP_COEF(Att0_0,0x7FFF);
            WRITE_DSP_COEF(Att0_1,0x7FFF);
            //Switch LDD to CD
            BD_SLEEP_EN;
            CDLD_EN;
            DVDLD_DIS;
            DelaymS(15);
            break;

        case eDiscDVD :
            // Use LDEL1 To Control DVD LD power
            if (svoVar_bLascMode == 1)
            {
                WRITE_DSP_COEF(Pread0RS,0x2900);//0x2900->0.13mw  0x3500->0.16mw
                WRITE_DSP_COEF(Pread1RS,0x2900);
                laser_pw = -18;
            }
            WRITE_DSP_COEF(Offset1_0,0x0000);
            WRITE_DSP_COEF(Offset1_1,0x0000);
            WRITE_DSP_COEF(Att1_0,0x7FFF);
            WRITE_DSP_COEF(Att1_1,0x7FFF);
            //Switch LDD to DVD
            BD_SLEEP_EN;
            DVDLD_EN;
            CDLD_DIS;
            DelaymS(15);
            break;

        case eDiscBD :
        case eDiscHD :
            // Use LDEL2 To Control BD LD power
            WRITE_DSP_COEF(Offset2_0,0x0000);
            WRITE_DSP_COEF(Offset2_1,0x0000);
            WRITE_DSP_COEF(Att2_0,0x7FFF);
            WRITE_DSP_COEF(Att2_1,0x7FFF);
            //Switch LDD to BD
            BD_SLEEP_DIS;
            DVDLD_EN;
            CDLD_DIS;
            if (mDiscKind.Bit.Layers == eLayersDouble)
            {
                laser_pw = 60;//According Spec.: 8.4mW/4.2mW = 2 (6dB)
            }
            break;
    }
    LAL_WriteCoef(SRV_LCTHRESH_LCTHMAX);
    LAL_WriteCoef(SRV_LCDELTA0_DELTA0MAX);
    LAL_WriteCoef(SRV_LCDELTA1_DELTA1MAX);
    LAL_WriteCoef(SRV_LCDELTA2_DELTA2MAX);
    WRITE_FIELD(HAL_SRV_LPCTHOLD, 0);
    HAL_LASC_Wait_Threshold_Stable();
#endif
    calSetMisc(0,LASER_PW,laser_pw);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_LASC_SetLaserOn

    Description:    Sets the laser ON, enabling interrupt and closing the suitable loops from laser control block

    Inputs:         OnOff boolean indicating the required action

    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
#if (OPU == SFBD412)||(OPU == SFBD414)||(OPU == SOHBP7)
#define LDOFF_TIMER     500//500mS.
#define CDLASER         0
#define DVDLASER        1
#define BDLASER         2
#endif
void HAL_LASC_SetLaserOn(BOOL LaserOn)
{
    #if (OPU == SFBD412)||(OPU == SFBD414)||(OPU == SOHBP7)
    static BYTE lasertype;
    static ULONG LDoffclock;
    #endif

    if (svoPrvtVar_LD_ON_Flag == LaserOn)
    {
        return;
    }
#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB==711)||(PCB == 812)||(PCB == 815)||(PCB == 817))
    #if((OPU != HOPB1300)&&(OPU != SFBD412)&&(OPU != SFBD414)&&(OPU != SOHBP7))
    //Only CD or DVD laser possible!!
    if (mDiscKind.Bit.DiscStd == eDiscCD)
        opSELCD = SELCDLASER;
    else
        opSELCD = SELDVDLASER;
    #endif
    //All flags 0, next need to become 1, independent of OnOff
    //Set Laser control bits
    WRITE_FIELD(HAL_SRV_LCTSTIRQ, 1);
    WRITE_FIELD(HAL_SRV_LDGRSEL, 1);
    WRITE_FIELD(HAL_SRV_LDGRPRESET, 0);
    WRITE_FIELD(HAL_SRV_WGATESEL, 1);
    WRITE_FIELD(HAL_SRV_WGPRESET, 0);

    if (LaserOn)
    {
        //Hold integrator
        WRITE_FIELD(HAL_SRV_LPCTHOLD, 1); // 1 is still a magic number, required is an enum for this field, like enable, disable or on, off, active_low, active_high
        WRITE_FIELD(HAL_SRV_LPCDHOLD, 1);

        //Reset integrator
        WRITE_FIELD(HAL_SRV_RSTLPCD, 1);
        WRITE_FIELD(HAL_SRV_RSTLPCT, 1);

        //Close suitable loops
        WRITE_FIELD(HAL_SRV_OPENTHLP, 1);
        WRITE_FIELD(HAL_SRV_OPENDLLP, 1);

        //Set the read path integrator
        WRITE_FIELD(HAL_SRV_RSTLPCT, 0);
        WRITE_FIELD(HAL_SRV_RSTLPCD, 1);

        //Set integrators
    #if (OPU != SFBD412)&&(OPU != SFBD414)&&(OPU != SOHBP7)
        WRITE_FIELD(HAL_SRV_LPCTHOLD, 0);
    #endif
        WRITE_FIELD(HAL_SRV_LPCDHOLD, 1);

        //Interrupt: Enable lapcr and disable lapcw0, lapcw1, lapcw2
        //WRITE_FIELD(HAL_SRV_THRESHIEN, 1);
        //WRITE_FIELD(HAL_SRV_DELTA0IEN, 0);
        //WRITE_FIELD(HAL_SRV_DELTA1IEN, 0);
        //WRITE_FIELD(HAL_SRV_DELTA2IEN, 0);
        //WRITE_FIELD(HAL_SRV_LDON, 0);     /* set LD to ON  */
    #if((OPU != HOPB1300)&&(OPU != SFBD412)&&(OPU != SFBD414)&&(OPU != SOHBP7))
        //for SONY KES-410/Pioneer BDPL2       //LCD port use SEN3
        WRITE_FIELD(HAL_APB_SSI_LALSB2MSB, 1);         //0xcf00140c.5 adress LSB to MSB
        WRITE_FIELD(HAL_APB_SSI_LDLSB2MSB, 1);         //0xcf00140c.4 data LSB to MSB
        WRITE_FIELD(HAL_APB_SSI_LSCLKINV, 1);          //0xcf00140c.4 SCLK invert
        WRITE_REG(HAL_APB_SSIREG_MAP->LCDCLKDIV, 0x0F);//0xcf001408 SSIclock=armclk/(1+clkdiv) , armclk=65MHz , SCLK=65M/16=4MHz

        WRITE_FIELD(HAL_APB_SSI_LMODEPOL, 1);        //0xcf001418.3 1: write=1 , read=0 for SONY OPU
        WRITE_FIELD(HAL_APB_SSI_LMODELOC, 1);        //0xcf001418.2 1: last bit of address for SONY OPU
        WRITE_FIELD(HAL_APB_SSI_LDATA1ST, 0);        //0xcf001418.1 0: address is send first for SONY OPU
                                                      //SONY KES-410 serial port work freq should < 20MHz
        //OPU XRST = 0 --> 1 , reset cansel
        opXRST = 1;
        //OPU register setting temp   //LCD port use SEN3
        //while(READ_FIELD(HAL_APB_SSI_LFIFEMP) == 0); //0xcf001410.4 empty?
    #endif
        //set LDD parameter
//    #if (OPU == BDPL2)
//        opLDDENB = 1;
//    #endif

    #if (OPU == SFBD412)||(OPU == SFBD414)
        switch(mDiscKind.Bit.DiscStd)
        {
            case eDiscCD:
            case eDiscDVD:
                while (lasertype == ((mDiscKind.Bit.DiscStd == eDiscDVD)?CDLASER:DVDLASER))
                {
                    if (ReadTimer(&LDoffclock) > LDOFF_TIMER)
                    {
                        break;
                    }
                }
                if (mDiscKind.Bit.DiscStd == eDiscDVD)
                {
                    lasertype = DVDLASER;
                }
                else
                {
                    lasertype = CDLASER;
                }
                WRITE_FIELD(HAL_AFE_VREFBSEL, halAFE_VREFBSEL_2p1V);
                break;

            case eDiscBD:
            default:
                lasertype = BDLASER;
        #if (OPU == SFBD414)
                WRITE_FIELD(HAL_AFE_VREFBSEL, halAFE_VREFBSEL_2p1V);		//for BD signal set the Vref to 2.1v too
        #else
                WRITE_FIELD(HAL_AFE_VREFBSEL, halAFE_VREFBSEL_2p4V);
        #endif
                break;
        }
    #elif (OPU == SOHBP7)
        switch(mDiscKind.Bit.DiscStd)
        {
            case eDiscCD:
            case eDiscDVD:
                while (lasertype == ((mDiscKind.Bit.DiscStd == eDiscDVD)?CDLASER:DVDLASER))
                {
                    if (ReadTimer(&LDoffclock) > LDOFF_TIMER)
                    {
                        break;
                    }
                }
                if (mDiscKind.Bit.DiscStd == eDiscDVD)
                {
                    lasertype = DVDLASER;
                }
                else
                {
                    lasertype = CDLASER;
                }
                WRITE_FIELD(HAL_AFE_VREFBSEL, halAFE_VREFBSEL_1p65V); //for BD signal set the VREFB to 1.65v
                break;

            case eDiscBD:
            default:
                lasertype = BDLASER;
                WRITE_FIELD(HAL_AFE_VREFBSEL, halAFE_VREFBSEL_2p1V);  //for BD signal set the VREFB to 2.1v
                break;
        }
    #endif

    #if (LASER_DRIVER == SAC1036SP)
        //OPU LDEN = 0 --> 1
        WRITE_FIELD(HAL_SRV_LDON, 1);     /* set LD to ON  */
    #endif
        //set LDD parameter
        HAL_LASC_SetReadMode();
    #if (OPU == BDPL2)
        opLDDENB = 1;
    #endif
    #if (LASER_DRIVER == NO_LASER_DRIVER)
        //OPU LDEN = 0 --> 1
        WRITE_FIELD(HAL_SRV_LDON, 1);     /* set LD to ON  */
    #endif
        svoPrvtVar_LD_ON_Flag = ON;
        SendMsg80(DEBUG_LASER_MSG,0x3FAA16); // Laser Diode ON
    #if (PCB == 812)
        if (mDiscKind.Bit.DiscStd == eDiscBD)
             WaitTimer(4000);
        else
             WaitTimer(500);
    #endif // (PCB == 812)
   }
   else
   {
    #if (FOOFFSET2_ENABLE == 1)
        HAL_SERVO_SetFoos(0);
    #endif
    #if (AFE_NORMALIZER == 1)
        afeSvognDisable();
    #endif
    #if (ENABLE_LEARN_DEFECT == 1)
        LAL_DSP_SetLearnDefect(lalDSP_Off);
    #endif
    #if (NEW_FDP_DETECTION != 0)
        WRITE_FIELD(HAL_SRV_FNOKEN,0); //turn off Focus drop interrupt, will be enabled in focus_on_chk()
        #if (NEW_FDP_DETECTION == 2)
        WRITE_FIELD(HAL_SRV_DSPI6EN,0);
        #endif
    #endif
        //Clear LCTSTIRQ flag.....
        WRITE_FIELD(HAL_SRV_LCTSTIRQ, 0);
        WRITE_FIELD(HAL_SRV_LPCTHOLD, 1);
        WRITE_FIELD(HAL_SRV_LPCDHOLD, 1);
        WRITE_FIELD(HAL_SRV_RSTLPCT, 1);
        WRITE_FIELD(HAL_SRV_RSTLPCD, 1);
        WRITE_FIELD(HAL_SRV_OPENTHLP, 1);
        WRITE_FIELD(HAL_SRV_OPENDLLP, 1);

        //Disable lapcr, lapcw0, lapcw1, lapcw2
//        WRITE_FIELD(HAL_SRV_THRESHIEN, 0);
//        WRITE_FIELD(HAL_SRV_DELTA0IEN, 0);
//        WRITE_FIELD(HAL_SRV_DELTA1IEN, 0);
//        WRITE_FIELD(HAL_SRV_DELTA2IEN, 0);

        //WRITE_FIELD(HAL_SRV_LDOFF, 0);     /* set LD to OFF  */
        //HAL_LASC_Init();
        LAL_LASC_ClearLaserOutputs(); //Clear the output of all channel
    #if (OPU == SFBD412)||(OPU == SFBD414)
        if (svoPrvtVar_LD_ON_Flag==ON)
            StartTimer(&LDoffclock);
    #endif
        //OPU LDEN = 1 --> 0
        WRITE_FIELD(HAL_SRV_LDOFF, 0);     /* set LD to OFF  */
        svoPrvtVar_LD_ON_Flag = OFF;
        svoVar_bByPassFOKCheck = TRUE;
    #if (OPU != SOHBP7)
        opXRST = 0;
    #endif
    #if (OPU == BDPL2)
        opLDDENB = 0;
    #endif
        SendMsg80(DEBUG_LASER_MSG,0x3FAA17); // Laser Diode OFF
    }
#elif (PCB == 691)
    //set LDD parameter
    HAL_LASC_SetReadMode();
//    send_msg1('S','L','D','M');

    //All flags 0, next need to become 1, independent of OnOff
    //Set Laser control bits
    WRITE_FIELD(HAL_SRV_LCTSTIRQ, ON);    //Laser Control Test Interrupt Request
    WRITE_FIELD(HAL_SRV_LDGRSEL, ON);     //Land Groove Selection
    WRITE_FIELD(HAL_SRV_LDGRPRESET, OFF); //Land Groove Preset(groove)
    WRITE_FIELD(HAL_SRV_WGATESEL, ON);    //Write Gate Selection(normal)
    WRITE_FIELD(HAL_SRV_WGPRESET, OFF);   //WGATE Preset(0)

    if (LaserOn)
    {
        /* Select CD/DVD/HD laser */
        /* opSEL2 high level voltage must 4.1~5.0V so connect SEL2(PA0) to a transistor */
        /* and then output to OPU but opSEL2 setting must invert */
    #if(OPSEL_INVERT==1)
        if (mDiscKind.Bit.DiscStd == eDiscCD)
        {
            opSEL1 = 1;
            opSEL2 = 0;
        }
        else if (mDiscKind.Bit.DiscStd == eDiscDVD)
        {
            opSEL1 = 0;
            opSEL2 = 1;
        }
        else //eDiscHD
        {
            opSEL1 = 0;
            opSEL2 = 0;
        }
    #else
        if (mDiscKind.Bit.DiscStd == eDiscCD)
        {
            opSEL1 = 0;
            opSEL2 = 0;
        }
        else if (mDiscKind.Bit.DiscStd == eDiscDVD)
        {
            opSEL1 = 1;
            opSEL2 = 1;
        }
        else //eDiscHD
        {
            opSEL1 = 1;
            opSEL2 = 0;
        }
    #endif

        //Hold integrator
        WRITE_FIELD(HAL_SRV_LPCTHOLD, ON); //Laser Power Control Threshold Hold
        WRITE_FIELD(HAL_SRV_LPCDHOLD, ON); //Laser Power Control Delta Hold

        //Reset integrator
        WRITE_FIELD(HAL_SRV_RSTLPCD, ON); //Reset LPCD Loop
        WRITE_FIELD(HAL_SRV_RSTLPCT, ON); //Reset LPCT Loop

        //Close suitable loops
        WRITE_FIELD(HAL_SRV_OPENTHLP, OFF); //Open Threshold Loop
        WRITE_FIELD(HAL_SRV_OPENDLLP, ON); //Open Delta Loop

        //Set the read path integrator
        WRITE_FIELD(HAL_SRV_RSTLPCT, OFF); //Reset LPCT Loop
        WRITE_FIELD(HAL_SRV_RSTLPCD, ON); //Reset LPCD Loop

        //Set integrators
        WRITE_FIELD(HAL_SRV_LPCTHOLD, OFF); //Laser Power Control Threshold Hold
        WRITE_FIELD(HAL_SRV_LPCDHOLD, ON); //Laser Power Control Delta Hold

        //Interrupt: Enable lapcr and disable lapcw0, lapcw1, lapcw2
        WRITE_FIELD(HAL_SRV_THRESHIEN, OFF); //No laser interrupt for now. TBD make interrupt function
        WRITE_FIELD(HAL_SRV_DELTA0IEN, OFF);
        WRITE_FIELD(HAL_SRV_DELTA1IEN, OFF);
        WRITE_FIELD(HAL_SRV_DELTA2IEN, OFF);

        WRITE_FIELD(HAL_SRV_LDON, 0);     /* set LD to ON  */
        svoPrvtVar_LD_ON_Flag = ON;
        //send_msg1('L','D','O','N');
        send_msg80(DEBUG_LASER_MSG,0x710508);
   }
   else
   {
#if (FOOFFSET2_ENABLE == 1)
        HAL_SERVO_SetFoos(0);
#endif
#if (AFE_NORMALIZER == 1)
        afeSvognDisable();
#endif
#if (ENABLE_LEARN_DEFECT == 1)
        LAL_DSP_SetLearnDefect(lalDSP_Off);
#endif
#if (NEW_FDP_DETECTION != 0)
        WRITE_FIELD(HAL_SRV_FNOKEN,0); //turn off Focus drop interrupt, will be enabled in focus_on_chk()
    #if (NEW_FDP_DETECTION == 2)
        WRITE_FIELD(HAL_SRV_DSPI6EN,0);
    #endif
#endif
        /* opSEL2 high level voltage must 4.1~5.0V so connect SEL2(PA0) to a transistor */
        /* and then output to OPU but opSEL2 setting must invert */
    #if(OPSEL_INVERT==1)
        opSEL1 = 1;
        opSEL2 = 1;
    #else
        opSEL1 = 0;
        opSEL2 = 1;
    #endif

        //Clear LCTSTIRQ flag.....
        WRITE_FIELD(HAL_SRV_LCTSTIRQ, 0); //Laser Control Test Interrupt Request

        //Set integrators
        WRITE_FIELD(HAL_SRV_LPCTHOLD, ON); //Laser Power Control Threshold Hold
        WRITE_FIELD(HAL_SRV_LPCDHOLD, ON); //Laser Power Control Delta Hold

        //Reset integrator
        WRITE_FIELD(HAL_SRV_RSTLPCT, ON); //Reset LPCT Loop
        WRITE_FIELD(HAL_SRV_RSTLPCD, ON); //Reset LPCD Loop

        //Close suitable loops
        WRITE_FIELD(HAL_SRV_OPENTHLP, ON); //Open Threshold Loop
        WRITE_FIELD(HAL_SRV_OPENDLLP, ON); //Open Delta Loop

        //Disable lapcr, lapcw0, lapcw1, lapcw2
        WRITE_FIELD(HAL_SRV_THRESHIEN, OFF); //No laser interrupt for now. TBD make interrupt function
        WRITE_FIELD(HAL_SRV_DELTA0IEN, OFF);
        WRITE_FIELD(HAL_SRV_DELTA1IEN, OFF);
        WRITE_FIELD(HAL_SRV_DELTA2IEN, OFF);

        WRITE_FIELD(HAL_SRV_LDOFF, 0);     /* set LD to OFF  */
        svoPrvtVar_LD_ON_Flag = OFF;
        //send_msg1('L','D','O','F');
        send_msg80(DEBUG_LASER_MSG,0x710509);
        svoVar_bByPassFOKCheck = TRUE;
    }
#endif
}


/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_LASC_Wait_Threshold_Stable()

    Description:    Wait threshold channel stable for APC.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_LASC_Wait_Threshold_Stable(void)
{
    USHORT i;
    SHORT read_error,read_errorini;

    HAL_DSP_SetDebug(DEBUG_CH1, DEBUG_SOURCE_RFILE, 0x38,0x08); //read_error
    DelaymS(1);

    read_errorini = (SHORT)READ_DSP_COEF(debug1);
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x3FAA10,A2B(read_errorini));
    for (i=0;i<1000;i++)
    {
        read_error=(SHORT)READ_DSP_COEF(debug1);
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x3FAA0F,A2B(read_error));

        if (read_error<=(SHORT)((LONG)read_errorini*3/100))
        {
            break;
        }
        DelaymS(2);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: HAL_LASC_SetPdic()
 * Description: Set PDIC mode
 *
 *----------------------------------------------------------------------------*/
#if (PDIC_BASE_ON_SRFO == 0)
__inline void SetPDICMode(BYTE pdic_mode)
{
#if (OPU == KEM350AAA)
    PDIC_MODE_t GO5, GO4;

    //Remark : PDIC of KES350 expect RWn : Read = high while Wgate gives read = low
    GO5.mode.inL = SW_LOW;
    GO4.mode.inL = SW_LOW;
    GO5.mode.inH = SW_LOW;
    GO4.mode.inH = SW_LOW;
    GO5.mode.syn = 1;
    GO4.mode.syn = 1;
    GO5.mode.out = 3;
    GO4.mode.out = 3;
    GO5.mode.reserved = 0;
    GO4.mode.reserved = 0;

    switch (pdic_mode)
    {//Read
        case 3:
            send_msg5S(SHOW_DEBUG_MSG,"rgn_eHighGain");
            GO5.mode.inL = SW_HIGH;   //sw1
            GO4.mode.inL = SW_HIGH;   //sw2
            break;
        case 2:
            send_msg5S(SHOW_DEBUG_MSG,"rgn_eMiddleGain");
            GO5.mode.inL = SW_HIGH;   //sw1
            GO4.mode.inL = SW_LOW;    //sw2
            break;
        case 1:
            send_msg5S(SHOW_DEBUG_MSG,"rgn_eLowGain");
            GO5.mode.inL = SW_HIGH;   //sw1
            GO4.mode.inL = SW_HI_Z;   //sw2
            break;
        case 0:
            send_msg5S(SHOW_DEBUG_MSG,"rgn_eLowestGain");
            GO5.mode.inL = SW_LOW;
            GO4.mode.inL = SW_HIGH;
            break;
    }

    //write to device
    WriteOPU(PDIC_MODESETTING1, GO4.b);
    WriteOPU(PDIC_MODESETTING2, GO5.b);
//    send_msg5SValue(1, "GO5.b= ", 4, GO5.b);
//    send_msg5SValue(1, "GO4.b= ", 4, GO4.b);
    WRITE_FIELD(HAL_WS_GAINSW2R, halWS_GAINSW2R_HiZ1);
    WRITE_FIELD(HAL_WS_GAINSWR,  pdic_mode);
#elif (OPU == SFBD412)||(OPU == SFBD414)
    switch (pdic_mode)
    {
        case 0:
            //BD photo diode sleep mode
            SWB1(1);
            SWB2(1);
            //Switch OEIC to CD
            SW_OUTPUT();
            opSW = 1;
            break;

        case 1:
            //BD photo diode sleep mode
            SWB1(1);
            SWB2(1);
            //Switch OEIC to DVD
            SW_OUTPUT();
            opSW = 0;
            break;

        case 2:
            SW_INPUT();
            SWB1(0);
            SWB2(1);
            SendMsg80(SHOW_DEBUG_MSG,0x33FF15); // BDPDIC Mid-gain!
            break;

        case 3:
            SW_INPUT();
            SWB1(1);
            SWB2(0);
            SendMsg80(SHOW_DEBUG_MSG,0x33FF16);
            break;
    }
#elif (OPU == BDPL2)
    switch (pdic_mode)
    {
        case 0:
            OEICSW1M();
            OEICSW2L();
            break;

        case 1:
            OEICSW1M();
            OEICSW2M();
            break;

        case 2:
            OEICSW1L();
            OEICSW2L();
            break;

        case 3:
            OEICSW1L();
            OEICSW2H();
            break;

        case 4:
            OEICSW1M();
            OEICSW2H();
            break;

        case 5:
            OEICSW1H();
            OEICSW2L();
            break;

        case 6:
            OEICSW1H();
            OEICSW2M();
            break;

        case 7:
            OEICSW1H();
            OEICSW2H();
            break;
    }
#elif (OPU == HOPB1300)
    WRITE_FIELD(HAL_APB_SSI_EXTSEN,1); //Enable SEN as output
    switch (pdic_mode)
    {
        case 0:
//            WRITE_FIELD(HAL_PERIPH_PB4DATA,0); //Set MIO0 output = low , select PM(CD)
//            WRITE_FIELD(HAL_PERIPH_PB7DATA,0); //Set MIO3 output = high , select SL(BD) Sleep
            WRITE_FIELD(HAL_PERIPH_PB3DATA,0); //Set PB3 output = low , select PM(CD)
            WRITE_FIELD(HAL_APB_SSI_PSEN,0); //Set SEN output = low , select SL(BD) Sleep
            break;

        case 1:
//            WRITE_FIELD(HAL_PERIPH_PB4DATA,1); //Set MIO0 output = high , select PM(DVD)
//            WRITE_FIELD(HAL_PERIPH_PB7DATA,0); //Set MIO3 output = high , select SL(BD) Sleep
            WRITE_FIELD(HAL_PERIPH_PB3DATA,1); //Set PB3 output = high , select PM(DVD)
            WRITE_FIELD(HAL_APB_SSI_PSEN,0); //Set SEN output = low , select SL(BD) Sleep
            break;

        case 2:
            WRITE_FIELD(HAL_APB_SSI_PSEN,1); //Set SEN output = high , select SL(BD) Active
            break;
    }
#elif (OPU == DXX811)
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
            opMIO3 = 0; //temp use the same with A0
            break;

        case eDiscDVD :
        case eDiscHD :
            opMIO3 = 1; //temp use the same with A0
            break;

        case eDiscBD :
            break;
    }

    switch (pdic_mode)
    {
        case eHighGain :
            opPDG1 = 1;
            opPDG2 = 1;
            break;

        case eMiddleGain :
            opPDG1 = 1;
            opPDG2 = 0;
            break;

        case eLowGain :
            opPDG1 = 0;
            opPDG2 = 1;
            break;

        case eLowestGain :
            opPDG1 = 0;
            opPDG2 = 0;
            break;
    }
#endif
}

SHORT const PDICGain[]     = {OPU_GAIN};

typedef enum
{
    OPU_GAIN_INDEX_CDR = 0,
    OPU_GAIN_INDEX_CDRW,
    OPU_GAIN_INDEX_DVDR,
    OPU_GAIN_INDEX_DVDRDL,
    OPU_GAIN_INDEX_DVDRW,
    OPU_GAIN_INDEX_DVDRWDL,
    OPU_GAIN_INDEX_DVDRAM,
#if (BD_ENABLE == 1)
    OPU_GAIN_INDEX_BDROM,
    OPU_GAIN_INDEX_BDROMDL,
    OPU_GAIN_INDEX_BDRE,
    OPU_GAIN_INDEX_BDREDL
#endif
} eOPUGainIndex;

AfeOPUGain const OPUGain[] = {{OPU_GAIN_SELECT_CDR},       //CDROM/R
                              {OPU_GAIN_SELECT_CDRW},      //CDRW
                              {OPU_GAIN_SELECT_DVDR},      //DVDROM/R SL
                              {OPU_GAIN_SELECT_DVDRDL},    //DVDROM/R DL
                              {OPU_GAIN_SELECT_DVDRW},     //DVDRW SL
                              {OPU_GAIN_SELECT_DVDRWDL},   //DVDRW DL
                              {OPU_GAIN_SELECT_DVDRAM},    //DVDRAM
#if (BD_ENABLE == 1)
                              {OPU_GAIN_SELECT_BD_RO_SL},  //BDROM SL
                              {OPU_GAIN_SELECT_BD_RO_DL},  //BDROM DL
                              {OPU_GAIN_SELECT_BD_RE_SL},  //BDR/RE SL
                              {OPU_GAIN_SELECT_BD_RE_DL},  //BDR/RE DL
#endif
                             };

void HAL_LASC_SetPdic(void)
{
    eOPUGainIndex index;
    BYTE   bOPUReadMode;

    switch (mDiscKind.Bit.DiscStd)
    {
#if (BD_ENABLE == 1)
        case eDiscBD:
            switch (mDiscKind.Bit.Media)
            {
                case eMediaRW:
                case eMediaR:
                    if (mDiscKind.Bit.Layers == eLayersDouble)
                    {
                        index = OPU_GAIN_INDEX_BDREDL;
                    }
                    else
                    {
                        index = OPU_GAIN_INDEX_BDRE;
                    }
                    break;

                case eMediaROM:
                default:
                    if (mDiscKind.Bit.Layers == eLayersDouble)
                    {
                        index = OPU_GAIN_INDEX_BDROMDL;
                    }
                    else
                    {
                        index = OPU_GAIN_INDEX_BDROM;
                    }
                    break;
            }
            break;
#endif
        case eDiscDVD:
            switch (mDiscKind.Bit.Media)
            {
                case eMediaRAM:
                    index = OPU_GAIN_INDEX_DVDRAM;
                    break;

                case eMediaRW:
                    if (mDiscKind.Bit.Layers == eLayersDouble) //DL
                    {
                        index = OPU_GAIN_INDEX_DVDRWDL;
                    }
                    else //SL
                    {
                        index = OPU_GAIN_INDEX_DVDRW;
                    }
                    break;

                case eMediaR:
                case eMediaROM:
                default:
                    if (mDiscKind.Bit.Layers == eLayersDouble)
                    {
                        index = OPU_GAIN_INDEX_DVDRDL;
                    }
                    else
                    {
                        index = OPU_GAIN_INDEX_DVDR;
                    }
                    break;
            }
            break;

        case eDiscCD:
            switch (mDiscKind.Bit.Media)
            {
                case eMediaRW:
                    index = OPU_GAIN_INDEX_CDRW;
                    break;

                case eMediaR:
                case eMediaROM:
                default:
                    index = OPU_GAIN_INDEX_CDR;
                    break;
            }
            break;
    }
    bOPUReadMode = OPUGain[index].ReadGain;

    SendMsgCnB(DEBUG_VENDOR_WRITE_MSG|DEBUG_CD_WRITE_MSG,7,0x3B000A,B1B(bOPUReadMode),B1B(READ_FIELD(HAL_AFE_MBGUPSEL)),
        B1B(READ_FIELD(HAL_AFE_SBGUPSEL)),F4B((float)(PDICGain[bOPUReadMode]) / 10));

    if (svoCalMisc2Var.pdic_gn != PDICGain[bOPUReadMode])
    {
        track_off();
        svoFocusOff();
    }

    SetPDICMode(bOPUReadMode);
    calSetMisc(0,PDIC_GN, PDICGain[bOPUReadMode]);
}// end of HAL_LASC_SetPdic
#else
void HAL_LASC_SetPdic(void)
{
#if (OPU == SFBD412)||(OPU == SFBD414)//692
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            //BD photo diode sleep mode
            SWB1(1);
            SWB2(1);
            //Switch OEIC to CD
            SW_OUTPUT();
            opSW = 1;
            break;

        case eDiscDVD:
            //BD photo diode sleep mode
            SWB1(1);
            SWB2(1);
            //Switch OEIC to DVD
            SW_OUTPUT();
            opSW = 0;
            break;

        case eDiscBD:
            SW_INPUT();
            if (svoVar_bPdicMode == 0)
            {
                SWB1(0);
                SWB2(1);
                SendMsg80(SHOW_DEBUG_MSG,0x33FF15); // BDPDIC Mid-gain!
            }
            else
            {
                SWB1(1);
                SWB2(0);
                SendMsg80(SHOW_DEBUG_MSG,0x33FF16);
            }
            break;
    }
#elif (OPU == BDPL2)//815
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            OEICSW1L();
            if (svoVar_bPdicMode == 0)
            {
                OEICSW2L();
            }
            else
            {
                OEICSW2H();
            }
            break;

        case eDiscDVD:
            OEICSW1M();
            if (svoVar_bPdicMode == 0)
            {
                OEICSW2L();
            }
            else
            {
                OEICSW2H();
            }
            break;

        case eDiscBD:
            OEICSW1H();
            if (svoVar_bPdicMode == 0)
            {
                OEICSW2L();
            }
            /*else if (svoVar_bPdicMode == 1)
            {
                OEICSW2M();
            }*/
            else
            {
                OEICSW2H();
            }
            break;
    }
#elif (OPU == HOPB1300)//812
    WRITE_FIELD(HAL_APB_SSI_EXTSEN,1); //Enable SEN as output
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
//            WRITE_FIELD(HAL_PERIPH_PB4DATA,0); //Set MIO0 output = low , select PM(CD)
//            WRITE_FIELD(HAL_PERIPH_PB7DATA,0); //Set MIO3 output = high , select SL(BD) Sleep
            WRITE_FIELD(HAL_PERIPH_PB3DATA,0); //Set PB3 output = low , select PM(CD)
            WRITE_FIELD(HAL_APB_SSI_PSEN,0); //Set SEN output = low , select SL(BD) Sleep
            break;

        case eDiscDVD:
//            WRITE_FIELD(HAL_PERIPH_PB4DATA,1); //Set MIO0 output = high , select PM(DVD)
//            WRITE_FIELD(HAL_PERIPH_PB7DATA,0); //Set MIO3 output = high , select SL(BD) Sleep
            WRITE_FIELD(HAL_PERIPH_PB3DATA,1); //Set PB3 output = high , select PM(DVD)
            WRITE_FIELD(HAL_APB_SSI_PSEN,0); //Set SEN output = low , select SL(BD) Sleep
            break;

        case eDiscBD:
            WRITE_FIELD(HAL_APB_SSI_PSEN,1); //Set SEN output = high , select SL(BD) Active
            break;
    }
#elif (OPU == SOHBP7)
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD:
            //BD photo diode sleep mode
            BD_SLEEP_EN;
            //Switch OEIC to CD
            DVDCD_STB_CD;
            break;

        case eDiscDVD:
            //BD photo diode sleep mode
            BD_SLEEP_EN;
            //Switch OEIC to DVD
            DVDCD_STB_DVD;
            break;

        case eDiscBD:
            //BD PDIC higigain mode
            BD_SLEEP_DIS;
            //Switch DVD/CD OEIC to standby mode
            DVDCD_STB_STB;
            break;
    }
#endif
}
#endif

#if (OPU == BDPL2)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_LASC_SetAPCTarget()

    Description:    update laser power control target for given collimator position

    Inputs:         collimator position (micro step)
    Outputs:        None
    Global Inputs:  None
    Global Outputs: APCtarget
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_LASC_SetAPCTarget(SHORT colli_pos)
{
    USHORT APCtarget;

    //Pioneer suggest curve interpolate
    //func2=    (1-(APC0-32)/(APC1-32))*(CLx-CL0)
    //func1=    1+func2/(CL0-CL1)
    //APCx= (APC0-32)/func1 +32
    //APCx=     ((APC1-32)*(CL0-CL1)*(APC0-32))/((APC1-32)*(CL0-CL1)+(CLx-CL0)*(APC1-APC0))+32

    if (mDiscKind.Bit.DiscStd == eDiscBD)
    {
        colli_pos /= 32;
        if (mDiscKind.Bit.Layers == eLayersDouble)
        {
            APCtarget = (RoundDivide((OPUBarcode.BDDL_PL1-32) * (246-141) * (OPUBarcode.BDDL_PL0-32),
                                     (OPUBarcode.BDDL_PL1-32) * (246-141) + (OPUBarcode.BDDL_PL1-OPUBarcode.BDDL_PL0) * (colli_pos - 246)) + 32);
        }
        else
        {
            APCtarget = (RoundDivide((OPUBarcode.BDSL_PL1-32) * (246-141) * (OPUBarcode.BDSL_PL0-32),
                                     (OPUBarcode.BDSL_PL1-32) * (246-141) + (OPUBarcode.BDSL_PL1-OPUBarcode.BDSL_PL0) * (colli_pos - 246)) + 32);
        }
        if(APCtarget >= 0xF8)
        {
            send_msg5SValue(SHOW_DEBUG_MSG, "BD APCtarget saturate ", 2, APCtarget);
            APCtarget = 0xF8;
        }
        WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG3), APCtarget);  /* Adress 03  */
        //send_msg5SValue(SHOW_DEBUG_MSG, "BD APCtarget= ", 1, APCtarget);
    }
}
#endif

#if (OPU == DXX811)
//parameters*10
OPUBarcodeP const Barcode_OL[12] =
{
    { 1,    0, 1000,     0,  -10,  -10,    0},  //OL1
    { 2,    0, 1000,     0,   10,  -10,    0},  //OL2
    { 3, 2000,    0,     6,    0,    0,    0},  //OL3
    { 4, 2000,    0,    -3,    0,    0,    0},  //OL4
    { 5, 2000,    0,    -5,    0,    0,    0},  //OL5
    { 6, 2000,    0,    -2,    0,    0,    0},  //OL6
    { 7, 2000,    0,    10,    0,    0,    0},  //OL7
    { 8,    0, 1000,     0,   10,   10,    0},  //OL8
    { 9,    0, 1000,     0,  -10,   10,    0},  //OL9
    {10,    0, 1000,     0,    0,    0,  -10},  //OL10
    {11,    0, 1000,     0,    0,    0,    0},  //OL11
    {12,    0, 1000,     0,    0,    0,   10},  //OL12
};

/*-----------------------------------------------------------------------------
 *
 * Module: HAL_LASC_SetLCD
 * Description: Set TBD! on TDK HD OPU Dxx811
 *
 *----------------------------------------------------------------------------*/
void HAL_LASC_SetLCD(void)
{
    BYTE i;
    SHORT OL_temp[12];
    BYTE init_U, init_L;
    SBYTE SA_value, AS0_value, AS45_value, COMA_value;
    send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x535500); // LCD setting start
    //LCD reset
    //opMIO2 = 1; //opRSTB

    //Calculate LCD value
    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
        case eDiscDVD :
        // -----------------------------------------------------------
        //| initial value U | initial value L | SA | AS0 |AS45 | COMA |
        //|       200       |       100       | 0  |  0  |  0  |  0   |
        // -----------------------------------------------------------
            //LCD date should from presistent table
            init_U = OPUBarcode.LC_UPPER_ELEMENT;
            init_L = OPUBarcode.LC_LOWER_ELEMENT;
            SA_value = OPUBarcode.LC_SA;
            AS0_value = OPUBarcode.LC_ASO;
            AS45_value = OPUBarcode.LC_AS45;
            COMA_value = OPUBarcode.LC_COMA;
//            send_msg81(SHOW_DEBUG_MSG,0x535501, init_U);
//            send_msg81(SHOW_DEBUG_MSG,0x535502, init_L);
//            send_msg81(SHOW_DEBUG_MSG,0x535503, SA_value);
//            send_msg81(SHOW_DEBUG_MSG,0x535504, AS0_value);
//            send_msg81(SHOW_DEBUG_MSG,0x535505, AS45_value);
//            send_msg81(SHOW_DEBUG_MSG,0x535506, COMA_value);

            for(i=0;i<12;i++)
            {
                OL_temp[i] = (USHORT)(Barcode_OL[i].Initial_value_U +
                                      Barcode_OL[i].Initial_value_L +
                                      Barcode_OL[i].PARA_SA * SA_value +
                                      Barcode_OL[i].PARA_AS0 * AS0_value +
                                      Barcode_OL[i].PARA_AS45 * AS45_value +
                                      Barcode_OL[i].PARA_COMA * COMA_value);
                OL_temp[i] = (OL_temp[i]%10) > 5 ? (OL_temp[i]/10+1) : (OL_temp[i]/10);

//                send_msg81(SHOW_DEBUG_MSG,0x535507, OL_temp[i]);
            }
            break;

        case eDiscHD :
        // -----------------------------------------------------------
        //| initial value U | initial value L | SA | AS0 |AS45 | COMA |
        //|       200       |       100       | 1  | -3  |  0  |  0   |
        // -----------------------------------------------------------
            init_U = OPUBarcode.LC_UPPER_ELEMENT;
            init_L = OPUBarcode.LC_LOWER_ELEMENT;
            SA_value = OPUBarcode.LC_SA;
            AS0_value = OPUBarcode.LC_ASO;
            AS45_value = OPUBarcode.LC_AS45;
            COMA_value = OPUBarcode.LC_COMA;

//            send_msg74(SHOW_DEBUG_MSG,0x535501, init_U);
//            send_msg74(SHOW_DEBUG_MSG,0x535502, init_L);
//            send_msg74(SHOW_DEBUG_MSG,0x535503, SA_value);
//            send_msg74(SHOW_DEBUG_MSG,0x535504, AS0_value);
//            send_msg74(SHOW_DEBUG_MSG,0x535505, AS45_value);
//            send_msg74(SHOW_DEBUG_MSG,0x535506, COMA_value);

            for(i=0;i<12;i++)
            {
                OL_temp[i] = (USHORT)(Barcode_OL[i].Initial_value_U +
                                      Barcode_OL[i].Initial_value_L +
                                      Barcode_OL[i].PARA_SA * SA_value +
                                      Barcode_OL[i].PARA_AS0 * AS0_value +
                                      Barcode_OL[i].PARA_AS45 * AS45_value +
                                      Barcode_OL[i].PARA_COMA * COMA_value);
                OL_temp[i] = (OL_temp[i]%10) > 5 ? (OL_temp[i]/10+1) : (OL_temp[i]/10);

//                send_msg74(SHOW_DEBUG_MSG,0x535507, OL_temp[i]);
            }
            break;
    }

    //Set LCD SSI value
    for(i=0;i<12;i++)
    {
        WriteXWSReg_1B((LCD_OL1_ADDR+i), OL_temp[i]); //temp use the same with A0
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: SetHFMMode
 * Description: Set HFM mode on TDK HD OPU Dxx811
 * Input: TBD!
 *----------------------------------------------------------------------------*/
void SetHFMMode(BYTE mode)
{
    switch (mode)
    {
#if 0 //OSCEN without NPN (F/W default setting)
        case DXX811_HFM_OFF :
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero); // GAINSW Output Level During Read = Low
            break;

        case DXX811_HFM_MODE1 :
            //WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_HiZ1); // GAINSW Output Level During Read = Hi-Z
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_HiZ3); // GAINSW Output Level During Read = Hi-Z
            break;

        case DXX811_HFM_MODE2:
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One);  // GAINSW Output Level During Read = High
            break;
#else //OSCEN with NPN
        case DXX811_HFM_OFF :
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One); // GAINSW Output Level During Read = Low
            break;

        case DXX811_HFM_MODE1 :
            //WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_HiZ1); // GAINSW Output Level During Read = Hi-Z
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_HiZ3); // GAINSW Output Level During Read = Hi-Z
            break;

        case DXX811_HFM_MODE2:
            WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);  // GAINSW Output Level During Read = High
            break;
#endif
    }
}
#endif // (OPU == DXX811
#if (OPU == KEM350AAA)
/*-----------------------------------------------------------------------------
 *
 * Module: HAL_LASC_SetLCD
 * Description: Set TBD! on KEM350AAA
 *
 *----------------------------------------------------------------------------*/
void HAL_LASC_SetLCD(void)
{
    SBYTE ALPHA, BETA, GAMMA, ZETA;
    LCD_CONTRL_t * p = (LCD_CONTRL_t *)OL;
    LCL_ITEM_t lcl, lcl_prev, x;
    BYTE i;

    switch (mDiscKind.Bit.DiscStd)
    {
        case eDiscCD :
        {
            ALPHA = ReadOPU(ADDLCDTANCD);
            BETA  = ReadOPU(ADDLCDRADCD);
            GAMMA = 0;
            ZETA  = 0;
            break;
        }
        case eDiscDVD:
        {
            ALPHA = ReadOPU(ADDLCDTANDVD);
            BETA  = 0;
            GAMMA = ReadOPU(ADDLCDAS00DVD);
            ZETA  = ReadOPU(ADDLCDAS45DVD);
            break;
        }

        case eDiscBD : //STILL TODO
            ALPHA = 0;
            BETA  = 0;
            GAMMA = 0;
            ZETA  = 0;
            break;
    }

    for (i = 0; i < LCLEND; i++)
    {
        switch(i)
        {
            case LCL1:  x.us = LCD_C2 - ALPHA               ; break;
            case LCL2:  x.us = LCD_C1 + BETA                ; break;
            case LCL3:  x.us = LCD_C2 - ALPHA + GAMMA - ZETA; break;
            case LCL4:  x.us = LCD_C1                       ; break;
            case LCL5:  x.us = LCD_C2 + ALPHA               ; break;
            case LCL6:  x.us = LCD_C2                       ; break;
            case LCL7:  x.us = LCD_C2 - ALPHA + GAMMA + ZETA; break;
            case LCL8:  x.us = LCD_C1 - BETA                ; break;
            case LCL9:  x.us = LCD_C2 - GAMMA - ZETA        ; break;
            case LCL10: x.us = LCD_C2 - GAMMA + ZETA        ; break;
            case LCL11: x.us = LCD_C2 + ALPHA + GAMMA - ZETA; break;
            case LCL12: x.us = LCD_C2 + ALPHA + GAMMA + ZETA; break;
        }

        lcl.s.high = x.orig.high8bits;
        if (i%2 == 1)
        {
            //odd
            lcl.s.low.addOdd      = x.orig.low2bits;
            lcl.s.low.addEven     = lcl_prev.s.low.addEven;
            lcl_prev.s.low.addOdd = x.orig.low2bits;

            p->us[i - 1] = lcl_prev.us;
            p->us[i]     = lcl.us;
        }
        else
        {
            //even
            lcl.s.low.addEven = x.orig.low2bits;
            lcl_prev = lcl;
        }
    }

    //Set LCD SSI value
    for(i=0;i<12;i++)
    {
        WriteOPU((LCD_OL1_ADDR + i), ((LCD_CONTRL_t *)OL)->lcl[i].s.high);
    }
    for(i = 0; i < 12/2; i++)
    {
        WriteOPU((LCD_OL1_OL2_LOWBITS + i), ((LCD_CONTRL_t *)OL)->lcl[i * 2].b[0]); //every 2nd entry!!!
    }
    SetLCD(LCDSW_ON);
}

void SetLCD(eLCDSW_OFF_ON_t cmd)
{
    LCD_SW_t x;
    x.s.lcd_sw = cmd;
    WriteOPU(LCD_SW_OFF_ON, x.b);
}

/*-----------------------------------------------------------------------------
 *
 * Module: HAL_LASC_SetCollimotorLED()
 * Description: Set TBD! on KEM350AAA
 *
 *----------------------------------------------------------------------------*/
void HAL_LASC_SetCollimotorLED(eOPTOCOUPLER_LED_t cmd)
{
    SA_LED_t x = {0};
    x.s.out = cmd;
    WriteOPU(ADD_SA_LED_CTRL, x.b);
}

/*********************************************************
* Module: HAL_LASC_ActivateNDFilter()
*
* Description:  opens or closes an optical filter by
*               exerting a pulse in positive, negative
*               direction. Pulse lasts for some defined
*               time.
* Input:        x: OPENCLOSE_t
* Returns: void
**********************************************************/

#define ND_TIMERA   50 //ND_TIMERA must less than 90mS.
#define ND_TIMERB   550//ND_TIMERA/ND_TIMERA must less than 10%.
void HAL_LASC_ActivateNDFilter(NDFILTEROPEN_CLOSE_t x)
{
    switch (x)
    {
        case NDFILTEROPEN:
            opND_PLUS = TRUE;
            opND_MINUS = FALSE;
            break;

        case NDFILTERCLOSE:
            opND_PLUS = FALSE;
            opND_MINUS = TRUE;
            break;

        default:
            opND_PLUS = FALSE;
            opND_MINUS = FALSE;
            break;
    }

    if ((x == NDFILTEROPEN) || (x == NDFILTERCLOSE))
    {
        WaitTimer(ND_TIMERA);//yields control to other threats
                           //if elapsed time smaller than
                           //defined time.
        opND_PLUS = FALSE;
        opND_MINUS = FALSE;
        WaitTimer(ND_TIMERB);//yields duty to make sure (ND_TIMERB/ND_TIMERA) < 10%
    }
}
#endif
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
