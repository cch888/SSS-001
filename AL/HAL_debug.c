/* ******************************************************************************************
*
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                             All Rights Reserved
*
*
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*   HAL_debug.c
*
*  Description:
*   This file contains debug functions and includes
*   the following:
*
*
* Notes:
*
*
*   $Revision: 1 $
*   $Date: 11/01/07 1:05p $
*
-----------------------------------------------------------------------------------------------------*/

#include "..\al\HAL_debug.h"


void HAL_debug_MIO0(ULONG signal)
{
#if (ENABLE_MIO0 == 1)
ULONG BlkSel, DbgMode;
BYTE BusSel, MuxSel, WrdSel, BitSel;

    BlkSel =  (signal & BLKSEL_MASK);
    DbgMode = (signal & DBGMODE_MASK);
    BusSel =  (BYTE)((signal & BUS_MASK) >> BUS_STARTBIT);
    MuxSel =  (BYTE)((signal & MUX_MASK) >> MUX_STARTBIT);

    if (DbgMode == MIO32SEL) //MIO32SEL mode
    {
        BitSel =  (BYTE)(signal & 0x00FF);
        WrdSel = 0;
        switch (BlkSel)
        {
            case AHB_BLK:
                //transparant mode (via BusSel)
                break;
            case BD_BLK:
                break;
            case DFE_BLK:
                break;
            case WBL_BLK:
                WRITE_FIELD(HAL_WBL_WBLMIOSEL, BitSel);
                break;
            case DEM_BLK:
                break;
            case AFE_BLK:
                WRITE_FIELD(HAL_AFE_AFEMIOSEL, BitSel);
                if (BitSel == halAFE_AFE_MIO_SEL2) WRITE_FIELD(HAL_AFE_MUXTP1SEL, MuxSel);
                else if (BitSel == halAFE_AFE_MIO_SEL3) WRITE_FIELD(HAL_AFE_MUXTP2SEL, MuxSel);
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_TPMUX, BitSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEMUX, BitSel);
                break;
            case WS_BLK:
                WRITE_FIELD(HAL_WS_WSMIO0, BitSel);
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSMIOSEL, BitSel);
                break;
            case CDD_BLK:
                break;
            case CDE_BLK:
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCMIO, BitSel);
                break;
            case HOST_BLK:
                WRITE_FIELD(HAL_HST_SATADEBUG, BitSel);
                WRITE_FIELD(HAL_HST_ENADBGHI, MuxSel);
                break;
            case BVCI_BLK:
                break;
            case SERVO_BLK:
                WRITE_FIELD(HAL_SRV_SVMIO0SEL, BitSel);
                break;
        }
        WRITE_FIELD(HAL_GLOBAL_M0SEL, BusSel);
        WRITE_FIELD(HAL_GLOBAL_MIO0SEL, TRUE);             //enable MIO
    }
#if (CHIP_REV >= 0xC0)
    else //DBG64SEL mode
    {
        BitSel = (BYTE)(signal & 0x000F);
        WrdSel = (BYTE)((signal>>4) & 0x0003);
        switch (BlkSel)
        {
            case AHB_BLK:
                WRITE_FIELD(HAL_GLOBAL_AHBDG48SL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDG16SL, MuxSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDBGEN, 1);
                break;
            case BD_BLK:
                if (MuxSel == 1) //16-bit bus repeated: use L
                {
                    WrdSel = 0;
                    WRITE_FIELD(HAL_BDC_BDDBGLSEL,(BusSel>>4));
                }
                WRITE_FIELD(HAL_BDC_BDDBGSEL, (BusSel&0x0F));
                break;
            case DFE_BLK:
                WrdSel = 0;                                //use DBG64[0..15] for MIO0
                WRITE_FIELD(HAL_DFE_DFEDBGSEL0, BusSel);   //use SEL0 (0..15) for MIO0
                break;
            case WBL_BLK:
                WrdSel = 0;                                //use DBG64[0..15] for MIO0
                WRITE_FIELD(HAL_WBL_WBLDBG1SEL, BusSel);   //use SEL1 (0..15) for MIO0
                break;
            case DEM_BLK:
                WRITE_FIELD(HAL_DMOD_DMDDBGSEL, BusSel);
                if (BusSel == 2)
                {
                    WrdSel = 0;
                    WRITE_FIELD(HAL_DMOD_DMDDBGLSEL, MuxSel);
                }
                else if (BusSel == 4)
                {
                    WRITE_FIELD(HAL_DMOD_DMDDBGLSEL, MuxSel);
                }
                break;
            case AFE_BLK:
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_DVDDBG48C, BusSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEDBGSL, (BusSel>>4));
                WRITE_FIELD(HAL_DVDE_DVDEDBGMD, (BusSel&0x0F));
                break;
            case WS_BLK:
                WrdSel = 0;                             //use DBG64[0..15] for MIO0
                WRITE_FIELD(HAL_WS_WSDBSELC, BusSel);   //use SELC (0..15) for MIO0
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSDBGSEL, BusSel);
                break;
            case CDD_BLK:
                WRITE_FIELD(HAL_CDD_CDDDBGSEL, BusSel);
                break;
            case CDE_BLK:
                WRITE_FIELD(HAL_CDE_CDEDBGMD, BusSel);
                WRITE_FIELD(HAL_CDE_CDEDBGSEL, MuxSel);
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCDBGSEL, BusSel);
                WRITE_FIELD(HAL_OPC_OPCDBGEN, 1);
                break;
            case HOST_BLK:
                if (BusSel < 20) //16-bit debug bus
                {
                    WrdSel = 0;                                //use DBG64[0..15] for MIO0
                    WRITE_FIELD(HAL_HST_HSTDBGMXLSEL, BusSel); //use LSEL (0..15) for MIO0
                    WRITE_FIELD(HAL_HST_HSTDBGMXLEN, 1);       //enable LSEL
                }
                else //32-bit debug bus
                {
                    if (signal & 0x0010)
                    {
                        WrdSel = 1;                                //use DBG64[31..16] for MIO0
                        WRITE_FIELD(HAL_HST_HSTDBGMXMSEL, BusSel); //use MSEL (16..31) for MIO0
                        WRITE_FIELD(HAL_HST_HSTDBGMXMEN, 1);       //enable MSEL
                    }
                    else
                    {
                        WrdSel = 0;                                //use DBG64[0..15] for MIO0
                        WRITE_FIELD(HAL_HST_HSTDBGMXLSEL, BusSel); //use LSEL (0..15) for MIO0
                        WRITE_FIELD(HAL_HST_HSTDBGMXLEN, 1);       //enable LSEL
                    }
                }
                break;
            case BVCI_BLK:
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGSL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGEN, 1);
                break;
            case SERVO_BLK:
                WrdSel = 0;                                //use DBG64[0..15] for MIO0
                WRITE_FIELD(HAL_SRV_DBGBUSSELA, MuxSel);   //use SELA (0..15) for MIO0
                WRITE_FIELD(HAL_SRV_DBGBUSEN, 1);          //enable servo dbg bus
                break;
        }
        //use DGBBUSL for MIO0
        WRITE_FIELD(HAL_GLOBAL_DBGBUSLBLKSEL, (BlkSel>>BLKSEL_STARTBIT)); //select block
        WRITE_FIELD(HAL_GLOBAL_DBGBUSLWDSEL, WrdSel);                     //select word
        WRITE_FIELD(HAL_GLOBAL_DBGBUSLEN, 1);                             //enable bus
        WRITE_FIELD(HAL_GLOBAL_M0SEL, (0x40|BitSel));                     //select signal
        WRITE_FIELD(HAL_GLOBAL_MIO0SEL, TRUE);                            //enable MIO
    }
#endif //(CHIP_REV >= 0xC0)
#endif //(ENABLE_MIO0 == 1)
}


void HAL_debug_MIO1(ULONG signal)
{
#if (ENABLE_MIO1 == 1)
ULONG BlkSel, DbgMode;
BYTE BusSel, MuxSel, WrdSel, BitSel;

    BlkSel =  (signal & BLKSEL_MASK);
    DbgMode = (signal & DBGMODE_MASK);
    BusSel =  (BYTE)((signal & BUS_MASK) >> BUS_STARTBIT);
    MuxSel =  (BYTE)((signal & MUX_MASK) >> MUX_STARTBIT);

    if (DbgMode == MIO32SEL) //MIO32SEL mode
    {
        BitSel =  (BYTE)(signal & 0x00FF);
        WrdSel = 0;
        switch (BlkSel)
        {
            case AHB_BLK:
                //transparant mode (via BusSel)
                break;
            case BD_BLK:
                break;
            case DFE_BLK:
                break;
            case WBL_BLK:
                WRITE_FIELD(HAL_WBL_WBLMIOSEL, BitSel);
                break;
            case DEM_BLK:
                break;
            case AFE_BLK:
                WRITE_FIELD(HAL_AFE_AFEMIOSEL, BitSel);
                if (BitSel == halAFE_AFE_MIO_SEL2) WRITE_FIELD(HAL_AFE_MUXTP1SEL, MuxSel);
                else if (BitSel == halAFE_AFE_MIO_SEL3) WRITE_FIELD(HAL_AFE_MUXTP2SEL, MuxSel);
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_TPMUX, BitSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEMUX, BitSel);
                break;
            case WS_BLK:
                WRITE_FIELD(HAL_WS_WSMIO1, BitSel);
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSMIOSEL, BitSel);
                break;
            case CDD_BLK:
                break;
            case CDE_BLK:
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCMIO, BitSel);
                break;
            case HOST_BLK:
                WRITE_FIELD(HAL_HST_SATADEBUG, BitSel);
                WRITE_FIELD(HAL_HST_ENADBGHI, MuxSel);
                break;
            case BVCI_BLK:
                break;
            case SERVO_BLK:
                WRITE_FIELD(HAL_SRV_SVMIO1SEL, BitSel);
                break;
        }
        WRITE_FIELD(HAL_GLOBAL_M1SEL, BusSel);
        WRITE_FIELD(HAL_GLOBAL_MIO1SEL, TRUE);             //enable MIO
    }
#if (CHIP_REV >= 0xC0)
    else //DBG64SEL mode
    {
        BitSel = (BYTE)(signal & 0x000F);
        WrdSel = (BYTE)((signal>>4) & 0x0003);
        switch (BlkSel)
        {
            case AHB_BLK:
                WRITE_FIELD(HAL_GLOBAL_AHBDG48SL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDG16SL, MuxSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDBGEN, 1);
                break;
            case BD_BLK:
                if (MuxSel == 1) //16-bit bus repeated: use M
                {
                    WrdSel = 1;
                    WRITE_FIELD(HAL_BDC_BDDBGMSEL,(BusSel>>4));
                }
                WRITE_FIELD(HAL_BDC_BDDBGSEL, (BusSel&0x0F));
                break;
            case DFE_BLK:
                WrdSel = 1;                                //use DBG64[16..31] for MIO1
                WRITE_FIELD(HAL_DFE_DFEDBGSEL1, BusSel);   //use SEL1 (16..31) for MIO1
                break;
            case WBL_BLK:
                WrdSel = 1;                                //use DBG64[16..31] for MIO1
                WRITE_FIELD(HAL_WBL_WBLDBG2SEL, BusSel);   //use SEL2 (16..31) for MIO1
                break;
            case DEM_BLK:
                WRITE_FIELD(HAL_DMOD_DMDDBGSEL, BusSel);
                if (BusSel == 2)
                {
                    WrdSel = 1;
                    WRITE_FIELD(HAL_DMOD_DMDDBGMSEL, MuxSel);
                }
                else if (BusSel == 4)
                {
                    WRITE_FIELD(HAL_DMOD_DMDDBGLSEL, MuxSel);
                }
                break;
            case AFE_BLK:
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_DVDDBG48C, BusSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEDBGSL, (BusSel>>4));
                WRITE_FIELD(HAL_DVDE_DVDEDBGMD, (BusSel&0x0F));
                break;
            case WS_BLK:
                WrdSel = 1;                             //use DBG64[16..31] for MIO1
                WRITE_FIELD(HAL_WS_WSDBSELB, BusSel);   //use SELB (16..31) for MIO1
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSDBGSEL, BusSel);
                break;
            case CDD_BLK:
                WRITE_FIELD(HAL_CDD_CDDDBGSEL, BusSel);
                break;
            case CDE_BLK:
                WRITE_FIELD(HAL_CDE_CDEDBGMD, BusSel);
                WRITE_FIELD(HAL_CDE_CDEDBGSEL, MuxSel);
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCDBGSEL, BusSel);
                WRITE_FIELD(HAL_OPC_OPCDBGEN, 1);
                break;
            case HOST_BLK:
                if (BusSel < 20) //16-bit debug bus
                {
                    WrdSel = 1;                                //use DBG64[31..16] for MIO1
                    WRITE_FIELD(HAL_HST_HSTDBGMXMSEL, BusSel); //use MSEL (31..16) for MIO1
                    WRITE_FIELD(HAL_HST_HSTDBGMXMEN, 1);       //enable MSEL
                }
                else //32-bit debug bus
                {
                    if (signal & 0x0010)
                    {
                        WrdSel = 1;                                //use DBG64[31..16] for MIO1
                        WRITE_FIELD(HAL_HST_HSTDBGMXMSEL, BusSel); //use MSEL (16..31) for MIO1
                        WRITE_FIELD(HAL_HST_HSTDBGMXMEN, 1);       //enable MSEL
                    }
                    else
                    {
                        WrdSel = 0;                                //use DBG64[0..15] for MIO1
                        WRITE_FIELD(HAL_HST_HSTDBGMXLSEL, BusSel); //use LSEL (0..15) for MIO1
                        WRITE_FIELD(HAL_HST_HSTDBGMXLEN, 1);       //enable LSEL
                    }
                }
                break;
            case BVCI_BLK:
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGSL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGEN, 1);
                break;
            case SERVO_BLK:
                WrdSel = 1;                                //use DBG64[16..31] for MIO1
                WRITE_FIELD(HAL_SRV_DBGBUSSELB, MuxSel);   //use SELB (16..31) for MIO1
                WRITE_FIELD(HAL_SRV_DBGBUSEN, 1);          //enable servo dbg bus
                break;
        }
        //use DGBBUSM for MIO1
        WRITE_FIELD(HAL_GLOBAL_DBGBUSMBLKSEL, (BlkSel>>BLKSEL_STARTBIT)); //select block
        WRITE_FIELD(HAL_GLOBAL_DBGBUSMWDSEL, WrdSel);                     //select word
        WRITE_FIELD(HAL_GLOBAL_DBGBUSMEN, 1);                             //enable bus
        WRITE_FIELD(HAL_GLOBAL_M1SEL, (0x50|BitSel));                     //select signal
        WRITE_FIELD(HAL_GLOBAL_MIO1SEL, TRUE);                            //enable MIO
    }
#endif //(CHIP_REV >= 0xC0)
#endif //(ENABLE_MIO1 == 1)
}


void HAL_debug_MIO2(ULONG signal)
{
#if (ENABLE_MIO2 == 1)
ULONG BlkSel, DbgMode;
BYTE BusSel, MuxSel, WrdSel, BitSel;

    BlkSel =  (signal & BLKSEL_MASK);
    DbgMode = (signal & DBGMODE_MASK);
    BusSel =  (BYTE)((signal & BUS_MASK) >> BUS_STARTBIT);
    MuxSel =  (BYTE)((signal & MUX_MASK) >> MUX_STARTBIT);

    if (DbgMode == MIO32SEL) //MIO32SEL mode
    {
        BitSel =  (BYTE)(signal & 0x00FF);
        WrdSel = 0;
        switch (BlkSel)
        {
            case AHB_BLK:
                //transparant mode (via BusSel)
                break;
            case BD_BLK:
                break;
            case DFE_BLK:
                break;
            case WBL_BLK:
                WRITE_FIELD(HAL_WBL_WBLMIOSEL, BitSel);
                break;
            case DEM_BLK:
                break;
            case AFE_BLK:
                WRITE_FIELD(HAL_AFE_AFEMIOSEL, BitSel);
                if (BitSel == halAFE_AFE_MIO_SEL2) WRITE_FIELD(HAL_AFE_MUXTP1SEL, MuxSel);
                else if (BitSel == halAFE_AFE_MIO_SEL3) WRITE_FIELD(HAL_AFE_MUXTP2SEL, MuxSel);
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_TPMUX, BitSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEMUX, BitSel);
                break;
            case WS_BLK:
                WRITE_FIELD(HAL_WS_WSMIO2, BitSel);
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSMIOSEL, BitSel);
                break;
            case CDD_BLK:
                break;
            case CDE_BLK:
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCMIO, BitSel);
                break;
            case HOST_BLK:
                WRITE_FIELD(HAL_HST_SATADEBUG, BitSel);
                WRITE_FIELD(HAL_HST_ENADBGHI, MuxSel);
                break;
            case BVCI_BLK:
                break;
            case SERVO_BLK:
                WRITE_FIELD(HAL_SRV_SVMIO2SEL, BitSel);
                break;
        }
        WRITE_FIELD(HAL_GLOBAL_M2SEL, BusSel);
        WRITE_FIELD(HAL_GLOBAL_MIO2SEL, TRUE);             //enable MIO
    }
#if (CHIP_REV >= 0xC0)
    else //DBG64SEL mode
    {
        BitSel = (BYTE)(signal & 0x000F);
        WrdSel = (BYTE)((signal>>4) & 0x0003);
        switch (BlkSel)
        {
            case AHB_BLK:
                WRITE_FIELD(HAL_GLOBAL_AHBDG48SL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDG16SL, MuxSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDBGEN, 1);
                break;
            case BD_BLK:
                if (MuxSel == 1) //16-bit bus repeated: use H
                {
                    WrdSel = 2;
                    WRITE_FIELD(HAL_BDC_BDDBGHSEL,(BusSel>>4));
                }
                WRITE_FIELD(HAL_BDC_BDDBGSEL, (BusSel&0x0F));
                break;
            case DFE_BLK:
                WrdSel = 2;                                //use DBG64[32..47] for MIO2
                WRITE_FIELD(HAL_DFE_DFEDBGSEL2, BusSel);   //use SEL2 (32..47) for MIO2
                break;
            case WBL_BLK:
                WrdSel = 2;                                //use DBG64[32..47] for MIO2
                WRITE_FIELD(HAL_WBL_WBLDBG3SEL, BusSel);   //use SEL3 (32..47) for MIO2
                break;
            case DEM_BLK:
                WRITE_FIELD(HAL_DMOD_DMDDBGSEL, BusSel);
                if (BusSel == 2)
                {
                    WrdSel = 2;
                    WRITE_FIELD(HAL_DMOD_DMDDBGHSEL, MuxSel);
                }
                else if (BusSel == 4)
                {
                    WRITE_FIELD(HAL_DMOD_DMDDBGLSEL, MuxSel);
                }
                break;
            case AFE_BLK:
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_DVDDBG48C, BusSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEDBGSL, (BusSel>>4));
                WRITE_FIELD(HAL_DVDE_DVDEDBGMD, (BusSel&0x0F));
                break;
            case WS_BLK:
                WrdSel = 2;                             //use DBG64[32..47] for MIO2
                WRITE_FIELD(HAL_WS_WSDBSELA, BusSel);   //use SELA (32..47) for MIO2
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSDBGSEL, BusSel);
                break;
            case CDD_BLK:
                WRITE_FIELD(HAL_CDD_CDDDBGSEL, BusSel);
                break;
            case CDE_BLK:
                WRITE_FIELD(HAL_CDE_CDEDBGMD, BusSel);
                WRITE_FIELD(HAL_CDE_CDEDBGSEL, MuxSel);
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCDBGSEL, BusSel);
                WRITE_FIELD(HAL_OPC_OPCDBGEN, 1);
                break;
            case HOST_BLK:
                if (BusSel < 20) //16-bit debug bus
                {
                    WrdSel = 2;                                //use DBG64[32..47] for MIO2
                    WRITE_FIELD(HAL_HST_HSTDBGMXHSEL, BusSel); //use HSEL (32..47) for MIO2
                    WRITE_FIELD(HAL_HST_HSTDBGMXHEN, 1);       //enable HSEL
                }
                else //32-bit debug bus
                {
                    if (signal & 0x0010)
                    {
                        WrdSel = 3;                                //use DBG64[48..63] for MIO2
                        WRITE_FIELD(HAL_HST_HSTDBGMXUSEL, BusSel); //use USEL (48..63) for MIO2
                        WRITE_FIELD(HAL_HST_HSTDBGMXUEN, 1);       //enable USEL
                    }
                    else
                    {
                        WrdSel = 2;                                //use DBG64[32..47] for MIO2
                        WRITE_FIELD(HAL_HST_HSTDBGMXHSEL, BusSel); //use HSEL (32..47) for MIO2
                        WRITE_FIELD(HAL_HST_HSTDBGMXHEN, 1);       //enable HSEL
                    }
                }
                break;
            case BVCI_BLK:
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGSL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGEN, 1);
                break;
            case SERVO_BLK:
                WrdSel = 2;                                //use DBG64[32..47] for MIO2
                WRITE_FIELD(HAL_SRV_DBGBUSSELC, MuxSel);   //use SELC (32..47) for MIO2
                WRITE_FIELD(HAL_SRV_DBGBUSEN, 1);          //enable servo dbg bus
                break;
        }
        //use DGBBUSH for MIO2
        WRITE_FIELD(HAL_GLOBAL_DBGBUSHBLKSEL, (BlkSel>>BLKSEL_STARTBIT)); //select block
        WRITE_FIELD(HAL_GLOBAL_DBGBUSHWDSEL, WrdSel);                     //select word
        WRITE_FIELD(HAL_GLOBAL_DBGBUSHEN, 1);                             //enable bus
        WRITE_FIELD(HAL_GLOBAL_M2SEL, (0x60|BitSel));                     //select signal
        WRITE_FIELD(HAL_GLOBAL_MIO2SEL, TRUE);                            //enable MIO
    }
#endif //(CHIP_REV >= 0xC0)
#endif //(ENABLE_MIO2 == 1)
}


void HAL_debug_MIO3(ULONG signal)
{
#if (ENABLE_MIO3 == 1)
ULONG BlkSel, DbgMode;
BYTE BusSel, MuxSel, WrdSel, BitSel;

    BlkSel =  (signal & BLKSEL_MASK);
    DbgMode = (signal & DBGMODE_MASK);
    BusSel =  (BYTE)((signal & BUS_MASK) >> BUS_STARTBIT);
    MuxSel =  (BYTE)((signal & MUX_MASK) >> MUX_STARTBIT);

    if (DbgMode == MIO32SEL) //MIO32SEL mode
    {
        BitSel =  (BYTE)(signal & 0x00FF);
        WrdSel = 0;
        switch (BlkSel)
        {
            case AHB_BLK:
                //transparant mode (via BusSel)
                break;
            case BD_BLK:
                break;
            case DFE_BLK:
                break;
            case WBL_BLK:
                WRITE_FIELD(HAL_WBL_WBLMIOSEL, BitSel);
                break;
            case DEM_BLK:
                break;
            case AFE_BLK:
                WRITE_FIELD(HAL_AFE_AFEMIOSEL, BitSel);
                if (BitSel == halAFE_AFE_MIO_SEL2) WRITE_FIELD(HAL_AFE_MUXTP1SEL, MuxSel);
                else if (BitSel == halAFE_AFE_MIO_SEL3) WRITE_FIELD(HAL_AFE_MUXTP2SEL, MuxSel);
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_TPMUX, BitSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEMUX, BitSel);
                break;
            case WS_BLK:
                WRITE_FIELD(HAL_WS_WSMIO3, BitSel);
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSMIOSEL, BitSel);
                break;
            case CDD_BLK:
                break;
            case CDE_BLK:
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCMIO, BitSel);
                break;
            case HOST_BLK:
                WRITE_FIELD(HAL_HST_SATADEBUG, BitSel);
                WRITE_FIELD(HAL_HST_ENADBGHI, MuxSel);
                break;
            case BVCI_BLK:
                break;
            case SERVO_BLK:
                WRITE_FIELD(HAL_SRV_SVMIO3SEL, BitSel);
                break;
        }
        WRITE_FIELD(HAL_GLOBAL_M3SEL, BusSel);
        WRITE_FIELD(HAL_GLOBAL_MIO3SEL, TRUE);             //enable MIO
    }
#if (CHIP_REV >= 0xC0)
    else //DBG64SEL mode
    {
        BitSel = (BYTE)(signal & 0x000F);
        WrdSel = (BYTE)((signal>>4) & 0x0003);
        switch (BlkSel)
        {
            case AHB_BLK:
                WRITE_FIELD(HAL_GLOBAL_AHBDG48SL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDG16SL, MuxSel);
                WRITE_FIELD(HAL_GLOBAL_AHBDBGEN, 1);
                break;
            case BD_BLK:
                if (MuxSel == 1) //16-bit bus repeated: use F
                {
                    WrdSel = 3;
                    WRITE_FIELD(HAL_BDC_BDDBGFSEL,(BusSel>>4));
                }
                WRITE_FIELD(HAL_BDC_BDDBGSEL, (BusSel&0x0F));
                break;
            case DFE_BLK:
                WrdSel = 0;                                //use DBG64[0..15] for MIO3
                WRITE_FIELD(HAL_DFE_DFEDBGSEL0, BusSel);   //use SEL0 (0..15) for MIO3
                break;
            case WBL_BLK:
                WrdSel = 0;                                //use DBG64[0..15] for MIO3
                WRITE_FIELD(HAL_WBL_WBLDBG1SEL, BusSel);   //use SEL1 (0..15) for MIO3
                break;
            case DEM_BLK:
                WRITE_FIELD(HAL_DMOD_DMDDBGSEL, BusSel);
                if (BusSel == 2)
                {
                    WrdSel = 3;
                    WRITE_FIELD(HAL_DMOD_DMDDBGFSEL, MuxSel);
                }
                else if (BusSel == 4)
                {
                    WRITE_FIELD(HAL_DMOD_DMDDBGLSEL, MuxSel);
                }
                break;
            case AFE_BLK:
                break;
            case DVDD_BLK:
                WRITE_FIELD(HAL_DVDD_DVDDBG48C, BusSel);
                break;
            case DVDE_BLK:
                WRITE_FIELD(HAL_DVDE_DVDEDBGSL, (BusSel>>4));
                WRITE_FIELD(HAL_DVDE_DVDEDBGMD, (BusSel&0x0F));
                break;
            case WS_BLK:
                WrdSel = 3;                             //use DBG64[48..63] for MIO3
                WRITE_FIELD(HAL_WS_WSDBSELD, BusSel);   //use SELD (48..63) for MIO3
                break;
            case LS_BLK:
                WRITE_FIELD(HAL_LS_LSDBGSEL, BusSel);
                break;
            case CDD_BLK:
                WRITE_FIELD(HAL_CDD_CDDDBGSEL, BusSel);
                break;
            case CDE_BLK:
                WRITE_FIELD(HAL_CDE_CDEDBGMD, BusSel);
                WRITE_FIELD(HAL_CDE_CDEDBGSEL, MuxSel);
                break;
            case OPC_BLK:
                WRITE_FIELD(HAL_OPC_OPCDBGSEL, BusSel);
                WRITE_FIELD(HAL_OPC_OPCDBGEN, 1);
                break;
            case HOST_BLK:
                if (BusSel < 20) //16-bit debug bus
                {
                    WrdSel = 3;                                //use DBG64[32..47] for MIO3
                    WRITE_FIELD(HAL_HST_HSTDBGMXUSEL, BusSel); //use USEL (48..63) for MIO3
                    WRITE_FIELD(HAL_HST_HSTDBGMXUEN, 1);       //enable USEL
                }
                else //32-bit debug bus
                {
                    if (signal & 0x0010)
                    {
                        WrdSel = 3;                                //use DBG64[48..63] for MIO3
                        WRITE_FIELD(HAL_HST_HSTDBGMXUSEL, BusSel); //use USEL (48..63) for MIO3
                        WRITE_FIELD(HAL_HST_HSTDBGMXUEN, 1);       //enable USEL
                    }
                    else
                    {
                        WrdSel = 2;                                //use DBG64[32..47] for MIO3
                        WRITE_FIELD(HAL_HST_HSTDBGMXHSEL, BusSel); //use HSEL (32..47) for MIO3
                        WRITE_FIELD(HAL_HST_HSTDBGMXHEN, 1);       //enable HSEL
                    }
                }
                break;
            case BVCI_BLK:
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGSL, BusSel);
                WRITE_FIELD(HAL_GLOBAL_BVCIDBGEN, 1);
                break;
            case SERVO_BLK:
                WrdSel = 3;                                //use DBG64[48..63] for MIO3
                WRITE_FIELD(HAL_SRV_DBGBUSSELD, MuxSel);   //use SELD (48..63) for MIO3
                WRITE_FIELD(HAL_SRV_DBGBUSEN, 1);          //enable servo dbg bus
                break;
        }
        //use DGBBUSU for MIO3
        WRITE_FIELD(HAL_GLOBAL_DBGBUSUBLKSEL, (BlkSel>>BLKSEL_STARTBIT)); //select block
        WRITE_FIELD(HAL_GLOBAL_DBGBUSUWDSEL, WrdSel);                     //select word
        WRITE_FIELD(HAL_GLOBAL_DBGBUSUEN, 1);                             //enable bus
        WRITE_FIELD(HAL_GLOBAL_M3SEL, (0x70|BitSel));                     //select signal
        WRITE_FIELD(HAL_GLOBAL_MIO3SEL, TRUE);                            //enable MIO
    }
#endif //(CHIP_REV >= 0xC0)
#endif //(ENABLE_MIO3 == 1)
}



/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
