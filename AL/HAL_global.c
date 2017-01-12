/* ******************************************************************************************
*
*	(c) Copyright 2002 - 2006 Sunext Design, INC.
*		          All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
* 	HAL_global.c
*
*  Description:
*	This file contains functions for the Global subsystem and includes
* 	the following:
*
*
* Notes:
*
*
*   $Revision: 16 $
*   $Date: 11/03/15 2:21p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\HAL_global.h"

#include ".\AL\REG_bd_codec.h"
#include ".\AL\REG_dfe.h"
#include ".\AL\REG_wobble.h"
#include ".\AL\REG_demod.h"
#include ".\AL\REG_dvd_decoder.h"
#include ".\AL\REG_dvd_encoder.h"
#include ".\AL\REG_cd_decoder.h"
#include ".\AL\REG_servo.h"
#if(PCB == 711) || (PCB == 710) || (PCB == 817)
#include ".\AL\REG_host.h"
#include ".\AL\REG_write_strategy.h"
#endif
#if (DEBUG16_OUTPUT_FROM_MIO==1)
#include ".\AL\REG_cd_encoder.h"
#endif
void HAL_GLOBAL_ResetBDCodec(void)
{
    WRITE_FIELD(HAL_GLOBAL_BDSRST, SET);
    WRITE_FIELD(HAL_GLOBAL_BDSRST, CLEAR);
}

#if 0//currently not be use
#if (CHIP_REV >= 0xC0)
void HAL_GLOBAL_SetBlockOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel);

void HAL_GLOBAL_SetDebugBusLOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel)
{
    WRITE_FIELD(HAL_GLOBAL_DBGBUSLEN,     TRUE);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSLBLKSEL, DbgBusBlkSel);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSLWDSEL,  DbgBusWordSel);
    HAL_GLOBAL_SetBlockOutput(DbgBusBlkSel, DbgBusWordSel, BusSel);
}
void HAL_GLOBAL_SetDebugBusMOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel)
{
    WRITE_FIELD(HAL_GLOBAL_DBGBUSMEN,     TRUE);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSMBLKSEL, DbgBusBlkSel);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSMWDSEL,  DbgBusWordSel);
    HAL_GLOBAL_SetBlockOutput(DbgBusBlkSel, DbgBusWordSel, BusSel);
}
void HAL_GLOBAL_SetDebugBusHOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel)
{
    WRITE_FIELD(HAL_GLOBAL_DBGBUSHEN,     TRUE);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSHBLKSEL, DbgBusBlkSel);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSHWDSEL,  DbgBusWordSel);
    HAL_GLOBAL_SetBlockOutput(DbgBusBlkSel, DbgBusWordSel, BusSel);
}
void HAL_GLOBAL_SetDebugBusUOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel)
{
    WRITE_FIELD(HAL_GLOBAL_DBGBUSUEN,     TRUE);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSUBLKSEL, DbgBusBlkSel);
    WRITE_FIELD(HAL_GLOBAL_DBGBUSUWDSEL,  DbgBusWordSel);
    HAL_GLOBAL_SetBlockOutput(DbgBusBlkSel, DbgBusWordSel, BusSel);
}

void HAL_GLOBAL_SetBlockOutput(halGLOBAL_DbgbusBlkSel_t DbgBusBlkSel, halGLOBAL_DbgbusWordSel_t DbgBusWordSel, BYTE BusSel)
{
    switch (DbgBusBlkSel)
    {
        case halGLOBAL_DbgBus_AHB:      // 0: AHB
            WRITE_FIELD(HAL_GLOBAL_AHBDBGEN, 1);
            WRITE_FIELD(HAL_GLOBAL_AHBDG48SL, BusSel);
            break;

        case halGLOBAL_DbgBus_BD:       // 1: BD codec
            WRITE_FIELD(HAL_DMOD_DMDDBGSEL, (BusSel>>4));
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_BDC_BDDBGLSEL, (BusSel&0x0F));
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_BDC_BDDBGMSEL, (BusSel&0x0F));
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_BDC_BDDBGHSEL, (BusSel&0x0F));
                    break;
                case halGLOBAL_DbgBus_63_48:
                    WRITE_FIELD(HAL_BDC_BDDBGFSEL, (BusSel&0x0F));
                    break;
            }
            break;

        case halGLOBAL_DbgBus_DFE:      // 2: DFE
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_DFE_DFEDBGSEL0, BusSel);
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_DFE_DFEDBGSEL1, BusSel);
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_DFE_DFEDBGSEL2, BusSel);
                    break;
            }
            break;

        case halGLOBAL_DbgBus_WBL:      // 3: Wobble
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_WBL_WBLDBG1SEL, BusSel);
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_WBL_WBLDBG2SEL, BusSel);
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_WBL_WBLDBG3SEL, BusSel);
                    break;
            }
            break;

        case halGLOBAL_DbgBus_DEM:      // 4: Demod
            WRITE_FIELD(HAL_DMOD_DMDDBGSEL, (BusSel>>4));
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_DMOD_DMDDBGLSEL, (BusSel&0x0F));
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_DMOD_DMDDBGMSEL, (BusSel&0x0F));
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_DMOD_DMDDBGHSEL, (BusSel&0x0F));
                    break;
                case halGLOBAL_DbgBus_63_48:
                    WRITE_FIELD(HAL_DMOD_DMDDBGFSEL, (BusSel&0x0F));
                    break;
            }
            break;

        case halGLOBAL_DbgBus_ADC:      // 5: ADC
            break;

        case halGLOBAL_DbgBus_DVDD:     // 6: DVD/HD-Decoder
            WRITE_FIELD(HAL_DVDD_DVDDBG48C, BusSel);
            break;

        case halGLOBAL_DbgBus_DVDE:     // 7: DVD/HD-Encoder
            WRITE_FIELD(HAL_DVDE_DVDEDBGSL, (BusSel>>4));
            WRITE_FIELD(HAL_DVDE_DVDEDBGMD, (BusSel&0x0F));
            break;

        case halGLOBAL_DbgBus_WS:       // 8: Write Strategy
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_WS_WSDBSELC, BusSel);
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_WS_WSDBSELB, BusSel);
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_WS_WSDBSELA, BusSel);
                    break;
                case halGLOBAL_DbgBus_63_48:
                    WRITE_FIELD(HAL_WS_WSDBSELD, BusSel);
                    break;
            }
            break;

//        case halGLOBAL_DbgBus_LS:       // 9: Light Scribe
//            WRITE_FIELD(HAL_LS_LSDBGSEL, BusSel);
//            break;

        case halGLOBAL_DbgBus_CDD:      // 10: CD-Decoder
            WRITE_FIELD(HAL_CDD_CDDDBGSEL, BusSel);
            break;

        case halGLOBAL_DbgBus_CDE:      // 11: CD-Encoder
            WRITE_FIELD(HAL_CDE_CDEDBGSEL, (BusSel>>3));
            WRITE_FIELD(HAL_CDE_CDEDBGMD,  (BusSel&0x07));
            break;

//        case halGLOBAL_DbgBus_OPC:      // 12: OPC
//            WRITE_FIELD(HAL_OPC_OPCDBGEN, 1);
//            WRITE_FIELD(HAL_OPC_OPCDBGSEL, BusSel);
//            break;

        case halGLOBAL_DbgBus_HOST:     // 13: Host
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_HST_HSTDBGMXLEN, 1);
                    WRITE_FIELD(HAL_HST_HSTDBGMXLSEL, BusSel);
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_HST_HSTDBGMXMEN, 1);
                    WRITE_FIELD(HAL_HST_HSTDBGMXMSEL, BusSel);
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_HST_HSTDBGMXHEN, 1);
                    WRITE_FIELD(HAL_HST_HSTDBGMXHSEL, BusSel);
                    break;
                case halGLOBAL_DbgBus_63_48:
                    WRITE_FIELD(HAL_HST_HSTDBGMXUEN, 1);
                    WRITE_FIELD(HAL_HST_HSTDBGMXUSEL, BusSel);
                    break;
            }
            break;

        case halGLOBAL_DbgBus_BVCI:     // 14: BVCI
            WRITE_FIELD(HAL_GLOBAL_BVCIDBGEN, 1);
            WRITE_FIELD(HAL_GLOBAL_BVCIDBGSL, BusSel);
            break;

        case halGLOBAL_DbgBus_SERVO:    // 15: Servo
            WRITE_FIELD(HAL_SRV_DBGBUSEN, 1);
            switch (DbgBusWordSel)
            {
                case halGLOBAL_DbgBus_15_0:
                    WRITE_FIELD(HAL_SRV_DBGBUSSELA, BusSel);
                    break;
               case halGLOBAL_DbgBus_31_16:
                    WRITE_FIELD(HAL_SRV_DBGBUSSELB, BusSel);
                    break;
                case halGLOBAL_DbgBus_47_32:
                    WRITE_FIELD(HAL_SRV_DBGBUSSELC, BusSel);
                    break;
                case halGLOBAL_DbgBus_63_48:
                    WRITE_FIELD(HAL_SRV_DBGBUSSELD, BusSel);
                    break;
            }
            break;
    }
}
#else // #if (CHIP_REV >= 0xC0)
void HAL_GLOBAL_SetDebug64Output(halGLOBAL_Dbg64Sel_t Dbg64Sel, BYTE Dbg64SignalSel)
{
    WRITE_FIELD(HAL_GLOBAL_DBG64EN,TRUE);
    WRITE_FIELD(HAL_GLOBAL_DBG64SL,Dbg64Sel);

    switch (Dbg64Sel)
    {
        case halGLOBAL_Dbg64Sel_AHB: //0:AHB
            //need check
            break;

        case halGLOBAL_Dbg64Sel_BD: //1:BD codec
            WRITE_FIELD(HAL_BDC_BDDBGSEL,Dbg64SignalSel);
            break;

        case halGLOBAL_Dbg64Sel_DFE: //2:DFE
            WRITE_FIELD(HAL_DFE_DFEDBGSEL,Dbg64SignalSel);
            break;

        case halGLOBAL_Dbg64Sel_WBL: //3:Wobble
            WRITE_FIELD(HAL_WBL_WBLDBGSEL,Dbg64SignalSel);
            break;

        case halGLOBAL_Dbg64Sel_DEM: //4:Demod
            WRITE_FIELD(HAL_DMOD_DMDDBGSEL,Dbg64SignalSel);
            break;

        case halGLOBAL_Dbg64Sel_ADC: //5:ADC
            //need check
            break;

        case halGLOBAL_Dbg64Sel_DVDD: //6:DVD/HD-Decoder
            WRITE_FIELD(HAL_DVDD_DVDDBG64C,Dbg64SignalSel);
            break;

        case halGLOBAL_Dbg64Sel_DVDE: //7:DVD/HD-Encoder
            //fixed output
            break;

        case halGLOBAL_Dbg64Sel_WS: //8:Write Strategy
            //fixed output
            break;

        case halGLOBAL_Dbg64Sel_LS: //9:Light Scribe
            //fixed output
            break;

        case halGLOBAL_Dbg64Sel_CDD: //10:CD-Decoder
            WRITE_FIELD(HAL_CDD_CDD64DEB,Dbg64SignalSel);
            break;

        case halGLOBAL_Dbg64Sel_OPC: //12:OPC
            //fixed output
            break;

    #if (DEBUG16_OUTPUT_FROM_MIO == 0)
        case halGLOBAL_Dbg64Sel_M16://15:Multi-16 Bus
            HAL_GLOBAL_SetDebug16Output(D16D64SEL Dbg16Sel, halGLOBAL_D16D64Sel0_t D16D64Sel);
            break;
    #endif
    }
}

    #if (DEBUG16_OUTPUT_FROM_MIO == 1)
//This is used for deneb 6230 B0
//note that only signals selected by "one Dbg16Sel" can be monitored at the same time from MIO0~3
//Example
//HAL_GLOBAL_SetDebug16Output(halGLOBAL_Dbg16Sel_DFE,halDFE_BlackDotAppDbg,0,14);//set MIO0-> early blackdot  flag
//HAL_GLOBAL_SetDebug16Output(halGLOBAL_Dbg16Sel_DFE,halDFE_BlackDotAppDbg,1,12);//set MIO1-> blank flag
//HAL_GLOBAL_SetDebug16Output(halGLOBAL_Dbg16Sel_DFE,halDFE_BlackDotAppDbg,2, 9);//set MIO2-> whitedot defect flag
//HAL_GLOBAL_SetDebug16Output(halGLOBAL_Dbg16Sel_DFE,halDFE_BlackDotAppDbg,3,10);//set MIO3-> blackdot flag
void HAL_GLOBAL_SetDebug16Output(halGLOBAL_Dbg16Sel_t Dbg16Sel, BYTE Dbg16SignalSel, BYTE MioNumber, BYTE MonitorPin)
{
    WRITE_FIELD(HAL_GLOBAL_DBG16SL,Dbg16Sel);
    switch (Dbg16Sel)
    {
        case halGLOBAL_Dbg16Sel_AHB:
            WRITE_FIELD(HAL_GLOBAL_AHBDBGSL,Dbg16SignalSel);
            break;

        case halGLOBAL_Dbg16Sel_BD:
            WRITE_FIELD(HAL_BDC_BDDBGSEL,Dbg16SignalSel);
            break;

        case halGLOBAL_Dbg16Sel_DFE:
            WRITE_FIELD(HAL_DFE_DFEAPPDSL,Dbg16SignalSel);
            break;

        case halGLOBAL_Dbg16Sel_WBL:
            WRITE_FIELD(HAL_WBL_WBLAPPSEL,Dbg16SignalSel);
            break;

        case halGLOBAL_Dbg16Sel_DEM:
            //about signa of pin, please see Book1 1000079C
            break;

        case halGLOBAL_Dbg16Sel_OPC:
            //about signa of pin, please see Book1 1000079C
            break;

        case halGLOBAL_Dbg16Sel_CDE:
            WRITE_FIELD(HAL_CDE_CDEDBGSEL,Dbg16SignalSel);
            break;
    }

    switch (MioNumber)
    {
        case 0:
            WRITE_FIELD(HAL_GLOBAL_MIO0SEL,TRUE);
            WRITE_FIELD(HAL_GLOBAL_M0SEL, 0x20 | MonitorPin);
            break;

        case 1:
            WRITE_FIELD(HAL_GLOBAL_MIO1SEL,TRUE);
            WRITE_FIELD(HAL_GLOBAL_M1SEL, 0x20 | MonitorPin);
            break;

        case 2:
            WRITE_FIELD(HAL_GLOBAL_MIO2SEL,TRUE);
            WRITE_FIELD(HAL_GLOBAL_M2SEL, 0x20 | MonitorPin);
            break;

        case 3:
            WRITE_FIELD(HAL_GLOBAL_MIO3SEL,TRUE);
            WRITE_FIELD(HAL_GLOBAL_M3SEL, 0x20 | MonitorPin);
            break;
    }
}
    #else // #if (DEBUG16_OUTPUT_FROM_MIO == 1)
//This is used for deneb 6230 BGA package, debug16 monitoring signal output from debug64 pin
//But debug16 was not connect properly in A0 version
void HAL_GLOBAL_SetDebug16Output(D16D64SEL Dbg16Sel, halGLOBAL_D16D64Sel0_t D16D64Sel)
{
    WRITE_FIELD(HAL_GLOBAL_DBG64EN,TRUE);
    WRITE_FIELD(HAL_GLOBAL_DBG64SL,halGLOBAL_Dbg64Sel_M16); //15:Multi-16 Bus

    switch (Dbg16Sel)
    {
        case D16D64SL0:
            WRITE_FIELD(HAL_GLOBAL_D16D64SL0,D16D64Sel);
            break;

        case D16D64SL1:
            WRITE_FIELD(HAL_GLOBAL_D16D64SL1,D16D64Sel);
            break;

        case D16D64SL2:
            WRITE_FIELD(HAL_GLOBAL_D16D64SL2,D16D64Sel);
            break;

        case D16D64SL3:
            WRITE_FIELD(HAL_GLOBAL_D16D64SL3,D16D64Sel);
            break;
    }
}
    #endif // #if (DEBUG16_OUTPUT_FROM_MIO == 1)
#endif // #if (CHIP_REV >= 0xC0)

#endif //currently not be use
