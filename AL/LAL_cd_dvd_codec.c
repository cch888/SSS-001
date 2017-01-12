/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2007 Sunext Design, INC.
*                 All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*   LAL_cd_dvd_codec.c
*
*  Description:
*   This file contains functions for the LAL CD&DVD CODEC layers and includes
*   the following:
*
*             .
*
* Notes:
*
*
*   $Revision: 27 $
*   $Date: 11/03/11 2:32p $
*
-----------------------------------------------------------------------------------------------------*/
#include ".\oem\oem_info.h"
#include ".\Common\globals.h" /* Needed for types of parameters */
#include ".\common\chip.h"
#include ".\common\vu_cfg.h"
#include ".\hif\gp_ram.h"
#include ".\player\plrdb.h"
#include ".\AL\coefs.h"
#include ".\AL\LAL_cd_dvd_codec.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_host.h"
#include ".\AL\reg_dvd_decoder.h"
#include ".\AL\reg_dvd_encoder.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_intctrl.h"
#include ".\AL\REG_host.h"
#include ".\AL\REG_cpycmp.h"

/*** Local defines ***/

/*-----------------------------------------------------------------------------------------------------
    Name:           LAL_CD_DVD_CODEC_Init

    Description:    Initialise the appropriate CD&DVD CODEC registers.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: The CD&DVD CODEC Registers are set.
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void LAL_CD_DVD_CODEC_Init( void )
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    WRITE_FIELD(HAL_GLOBAL_DECSRST,1);  /* Reset cd decoder regs */
    WRITE_FIELD(HAL_GLOBAL_DECSRST,0);  /* Clear cd decoder reset */
    WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, 0);  /* Decoder disable */
    WRITE_FIELD(HAL_CDD_DPCLR, 1);    /* Set Decoder partial clear */
    WRITE_FIELD(HAL_CDD_DPCLR, 0);    /* Clear Decoder partial clear */

    WRITE_FIELD(HAL_HST_PKTSIZE, halHST_PktSize_12byte);         /* Set default ATAPI packet size = 12 bytes */

    /*Automatic change from 2.5K to 3k page for C2PO*/
    WRITE_FIELD(HAL_CDD_MLY, halCDD_BufPage_2P5KB);        /* Memory layout 2.5K */
    WRITE_FIELD(HAL_CDD_C2P, halCDD_C2P_ErrCnt);
    /*Automatic change from 2.5K to 3k page for C2PO*/

    WRITE_FIELD(HAL_HST_DOUTEN, 1);

    WRITE_FIELD(HAL_HST_INHTRW, 0);             /* bInhTRW = 0 */

    WRITE_REG(HAL_CDDREG_MAP->CTRLECC, HAL_CDD_M2F2EDCDIS | HAL_CDD_E01RQ | HAL_CDD_AUTORQ | HAL_CDD_PRQ | HAL_CDD_QRQ);

    /* Enable auto EDAC mode- write address, erasure, enhanced decoder
        control and status enable, NOCOR ignores M2F2 EDAC or inhibited
        correction write-back */
    WRITE_REG(HAL_CDDREG_MAP->CTRLAM0, HAL_CDD_AUTOEM | HAL_CDD_DINFWE | HAL_CDD_IHDRW); /* YBM2 Work Around */

    WRITE_FIELD(HAL_HST_ADSC, 1);        /* Enable adsc during auto-A0 */
    WRITE_FIELD(HAL_HST_CDRDY, 1);       /* set DRDY on status completion */

    WRITE_REG(HAL_HSTREG_MAP->ATCMD, 0);               /* Disable scheck*/

    WRITE_REG(HAL_HSTREG_MAP->HOPT, HAL_HST_AUTOA0 | HAL_HST_AUTOAC | HAL_HST_AUTODC); /* Init auto features */

    /*Automatic change from 2.5K to 3k page for C2PO*/
    Iram_shHDDir = HAL_HST_HMLY | HAL_HST_HBODIS;   /* 2.5K Host Layout*/


    Iram_shHDDir |= HAL_HST_BUFPGSEL;

    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);      /* 24-bit addr, PIO xfr type */

    if(Iram_Send_Giveup_F == FALSE)
    {
        Iram_usiRingSP  = 0;
        RING_END_PAGE  = RING_END_PAGE_2p5K;
        Iram_usiRingEP  = RING_END_PAGE;
    }

    /**** Host Block Initialization ****/
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFSPGL, Iram_usiRingSP);      /* host start blk. */
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFEPGL, Iram_usiRingEP);      /* Host end blk. */
    WRITE_FIELD(HAL_HST_HSTBYTAC, 0);
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, 0);
    WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, 0);

    /**** CD Decoder Block Initialization ****/
    WRITE_REG(HAL_CDDREG_MAP->DSKBUFSPGL, Iram_usiRingSP);         /* ring start blk. */
    WRITE_REG(HAL_CDDREG_MAP->DSKBUFEPGL, Iram_usiRingEP);         /* ring end blk. */
    WRITE_FIELD(HAL_CDD_DSUBPG, 0);
    WRITE_FIELD(HAL_CDD_DECPG, 0);
    WRITE_REG(HAL_CDDREG_MAP->SUBQC, HAL_CDD_BUFSBQ | HAL_CDD_QCRCE | HAL_CDD_SUBQITPL);    /* Enable Interpolation */
    WRITE_FIELD(HAL_CDD_SCOREN, 0);     /* Disable SUB-Q interrupt */
    //ClrBitsSCReg_1B(rwSubCD, bSCEn);      /* Disable subcode logic for CD decoder */
    WRITE_FIELD(HAL_CDD_SCEN, 1);        /* read data with subcode */
    WRITE_REG(HAL_CDDREG_MAP->CTRLEDC1, HAL_CDD_ECCINCEN);
    WRITE_FIELD(HAL_CDD_DPRDPG, 2);

    /**** Copy Block Initialization */
    WRITE_FIELD(HAL_CPYCMP_CPYSPG, Iram_usiRingSP);         /* Copy Buffer Start Page */
    WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);         /* Copy Buffer End Page */

#if (AUDIO_PLAYBACK == 1)
    /**** Audio Block Initialization */
    WRITE_FIELD(HAL_AUD_ASTPG, Iram_usiRingSP);
    WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
    WRITE_FIELD(HAL_AUD_DACMUTE, 1);
    WRITE_REG(HAL_AUDREG_MAP->ATTLA, 0);
#endif
    /* init UDMA regs */
    WRITE_REG(HAL_HSTREG_MAP->USTATR, HAL_HST_CUCRCW);
    WRITE_REG(HAL_HSTREG_MAP->USTATR, HAL_HST_CUCRCW | HAL_HST_CLRCRCE);
    WRITE_REG(HAL_HSTREG_MAP->USTATR, HAL_HST_CUCRCW);

/*** Host Interrupt Initialization ***/
    WRITE_REG(HAL_HSTREG_MAP->HINTEN0, HAL_HST_SRSTEN | HAL_HST_ARSTEN | HAL_HST_DTEEN | HAL_HST_PCMDEN | HAL_HST_CMDEN );
    WRITE_REG(HAL_HSTREG_MAP->HINT0, 0);

/*** CD Decoder Interrupt Initialization ***/
    WRITE_REG(HAL_CDDREG_MAP->DINTEN0, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINTEN1, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINTEN2, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINT0, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINT1, 0);
    WRITE_REG(HAL_CDDREG_MAP->DINT2, 0);
    WRITE_REG(HAL_CPYCMPREG_MAP->CPYINTEN, HAL_CPYCMP_CPYIEN | HAL_CPYCMP_CMPIEN | HAL_CPYCMP_PFILIEN| HAL_CPYCMP_DCPYIEN| HAL_CPYCMP_DIFFIEN);

                                            /* Sector Processor/CD Interface regs */
    WRITE_FIELD(HAL_SRV_WGP, halSRV_bWGP_Dis);
    WRITE_FIELD(HAL_SRV_WGN, halSRV_bWGN_Dis);

/*** Clock Initialization ***/
    WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0, 0);        /* normal power mode, all blocks enabled */
    WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);         /* Enable EFM and OSCEN IO pads */
    WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN2, 0);
    WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN3, 0);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, 0x00);    // Enable all Clocks
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, 0x00);

    if ((Iram_Send_Giveup_F == FALSE) || (mDiscKind.Bit.DiscStd == eDiscDVD)) /*  use CD disc make access time test shut down in CD-T */
    {
        dvdDspSystemInit();     /* Initialize DVD system registers */
        if (mDiscKind.Bit.DiscStd == eDiscDVD)
    {
        WRITE_FIELD(HAL_CDD_SCEN, 0);   /* Disable subcode logic for CD decoder */
    }
    }

    /* Motor Speed, ATIP Interface regs rest to default value, will be
       initialized in servo module */

    SET_INT_ENABLE_MASK(temp_IE);
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
