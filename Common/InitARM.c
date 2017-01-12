/****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
* $Revision: 213 $
* $Date: 11/03/23 7:20p $
*
* DESCRIPTION
*   This file contains all modules used to initialize at start up.
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include "..\oem\oem_info.h"
#include "..\common\globals.h"
#include "..\common\globtype.h"
#include "..\common\chip.h"
#include "..\common\stdlib.h"
#include "..\common\hwdef.h"
#include "..\servo\svo.h"
#include "..\al\LAL.h"
#include "..\al\LAL_pll.h"
#include "..\al\LAL_pll.h"
#include "..\al\REG_intctrl.h"
#include "..\al\REG_afe.h"
#include "..\al\REG_global.h"
#include "..\al\REG_servo.h"
#include "..\al\REG_arm2sc.h"
#include "..\al\REG_sdram.h"
#include "..\al\REG_sflash.h"
#include "..\al\REG_host.h"
#include "..\al\REG_periph.h"
#include "..\al\REG_ssi.h"

#if (EN_DOSPI_SUPPORT == 1)
#include "..\al\reg_sflash.h"
#endif

#if (EN_POWER_CONTROL == 1)
#include "..\hif\powerctl.h"
#endif

void InitializeARM(void);
void InitializeSystem(void);
void InitializeIO(void);
void InitializeInterrupts(void);
void InitializeSFlash(void);
void InitializeSATA(void);

void ResetModules(void);
void InitializeChip(void);
void init_UART(void);

extern unsigned int bottom_of_heap;     /* defined in heap.s */

#define TURN_OFF_SATA_TX()          WRITE_FIELD(HAL_HST_TXCHOFF, 1)
#define TURN_ON_SATA_SSC()          WRITE_FIELD(HAL_HST_SSCCTL, halHST_SSCCtl_SSCON)
#define TURN_OFF_SATA_SSC()         WRITE_FIELD(HAL_HST_SSCCTL, halHST_SSCCtl_SSCOFF)


// Here a few macros to assemble and write values to SDRAM Interface Registers.
// They are meant to be used from within function InitializeSystem() only
// Their only purpose is to make the code more readable.
#define SET_SDMODE(SDCMD)                           (WRITE_FIELD(HAL_SDRAM_SDCMD, SDCMD))
#define SET_SDCONF0(CASLAT, SDWDTH32, SDZIZE)       (WRITE_REG(HAL_SDRAMREG_MAP->SDCONF0 , (CASLAT<<6 | SDWDTH32<<5 | SDZIZE)))
#define SET_SDCONF1(TRFC, TWR, TRAS)                (WRITE_REG(HAL_SDRAMREG_MAP->SDCONF1 , (TRFC<<4 | TWR<<3 | TRAS)))
#define SET_SDCONF2(REVINTV)                        (WRITE_FIELD(HAL_SDRAM_REFINTV , REVINTV))
#define SET_SDCONF3(CKE)                            (WRITE_FIELD(HAL_SDRAM_CKE , CKE))
#define SET_SDCONF4(TRP, TRCD, NOVL, BUS32)         (WRITE_REG(HAL_SDRAMREG_MAP->SDCONF4 , (TRP<<HAL_SDRAM_TRP_LSB | TRCD<<HAL_SDRAM_TRCD_LSB | NOVL<<HAL_SDRAM_NOVL_LSB | BUS32)))
#define SET_SDDELAY0(SDLCHDLY, SDCKODLY)            (WRITE_REG(HAL_SDRAMREG_MAP->SDDELAY0, (SDLCHDLY << HAL_SDRAM_SDLCHDLY_LSB | SDCKODLY)))
#define SET_SDDELAY1(SDINCKEL)                      (WRITE_FIELD(HAL_SDRAM_SDINCKSEL, SDINCKEL))


/******************************************/
/*  ARM Development Suite 1.2 Requirement */
/*                BEGIN                   */
/******************************************/

#include "..\common\rt_misc.h"

__value_in_regs struct __initial_stackheap __user_initial_stackheap(unsigned R0, unsigned SP, unsigned R2, unsigned SL)
{

    struct __initial_stackheap config;

    /* defined in heap.s, placed by scatterfile */
    config.heap_base = (unsigned int)&bottom_of_heap;

    /* inherit SP from the execution environment */
    config.stack_base = SP;

#if (MSG_OUT_MODE == MSG_DISABLE)
    __use_no_heap_region();
#endif

    return config;
}
/******************************************/
/*  ARM Development Suite 1.2 Requirement */
/*                 END                    */
/******************************************/


/****************************************************************************
* Name:     InitializeARM
*
* Description:  This function initializes the ARM microcontroller and system
*
* Global Outputs: Initialized IO Ports and SFRs.
*****************************************************************************/
void InitializeARM()
{
    vWakeUpFromPowerSaveMode();
#if (EN_POWER_CONTROL == 1)
    PowerControl(eIdlePower);
#endif
    InitGPDAC();
    InitializeInterrupts();
    ResetModules();
    InitializeChip();
    init_UART();
    InitializeSATA();

    /* Assert on initialisation error */
    if (!LAL_InitPlatform())
     {
         DEBUG_ASSERT(FALSE,eASSERT_DSP_BIST_TEST_FAILED);
     }

#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == SSI_INTERFACE)
    //init_ssi(); This function had been moved to InitializeSystem().
#endif

    init_serial();

    //InitializeSFlash(); This function had been moved to InitializeSystem().

#if 0 //for debugging
    send_msg5SValue(1, "SDCONF0", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF0));
    send_msg5SValue(1, "SDCONF1", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF1));
    send_msg5SValue(1, "SDCONF2", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF2));
    send_msg5SValue(1, "SDCONF3", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF3));
    send_msg5SValue(1, "SDCONF4", 1, READ_REG(HAL_SDRAMREG_MAP->SDCONF4));
    send_msg5SValue(1, "SDDELAY0", 1, READ_REG(HAL_SDRAMREG_MAP->SDDELAY0));
    send_msg5SValue(1, "SDDELAY1", 1, READ_REG(HAL_SDRAMREG_MAP->SDDELAY1));
#endif

}


typedef struct
{
    BYTE CasLat;
    BYTE SdWdth32;
    BYTE SdSize;
    BYTE Trfc;
    BYTE Twr;
    BYTE Tras;
    BYTE Trp;
    BYTE Trcd;
    BYTE Novl;
    BYTE Bus32;
    BYTE RefIntv;
    BYTE SDLchDly;
    BYTE SDCkoDly;
    BYTE SDInCkSel;
} SdramParameters_t;



/****************************************************************************
* Name:     InitializeSystem
*
* Description:  This function initializes the ARM microcontroller
*
* Global Outputs: Initialized IO Ports and SFRs.
*****************************************************************************/
#pragma arm section code = "INIT_SYS"
void InitializeSystem(void)
{
    volatile BYTE     Delay;
    SdramParameters_t SdramParams;
    BYTE sd_backup[8];

    /******************* Work Around for ARM hang on PRST *********************
    All clocks are stopped as default setting on PRST
    asserted. The delay "for" loops below uses an SDRAM location as the loop
    variable.  If the memory clock stop (CLKSTP1.MEMCKSTP) is not disabled (=0)
    then the ARM will hang when accessing the SDRAM */
    /**************************************************************************/

    /*-----Enable AFE Bolck Power------*/
    WRITE_FIELD(HAL_AFE_PDWOBALL, 0);       /* set power on input wobble       */
    WRITE_FIELD(HAL_AFE_PDWOB,    0);       /* set power on wobble porcessor   */
    WRITE_FIELD(HAL_AFE_PDRFALL,  0);       /* set power on DPD&RF             */
    WRITE_FIELD(HAL_AFE_PDEQBUF,  1);       /* set power down EQDOP&N Buffer   */
    WRITE_FIELD(HAL_AFE_PDAPCALL, 0);       /* set power on APC block          */
    //WRITE_FIELD(HAL_AFE_PDFVREF, 0);      /* set power on FVREF              */
    WRITE_FIELD(HAL_AFE_PDVREF06, 0);       /* set power on VREF06             */
    WRITE_FIELD(HAL_AFE_PDSVOALL, 0);       /* set power on servo block        */

    /*-----Enable Global Clock------*/
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, 0x00);          //Enable the clocks
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, 0x00);

#if 0
    // Check the revision of chip, and generate assert if it doesn't match before damage OPU
    if (READ_FIELD(HAL_GLOBAL_CHIPREV) != CHIP_REV)
    {
        DEBUG_ASSERT(FALSE, eASSERT_WRONG_CHIP_REVISION);
    }
#endif

    //Initialize SATA PHY and GHY settings
    //If these settings are changed, you have to update here and InitializeSATA() at the same time.
    WRITE_REG(HAL_HSTREG_MAP->PPHYCFGL, 0x17);      // Increase TX main swing to reduce jitter.
    WRITE_FIELD(HAL_HST_SATPLLBIS, 0x00);           // Set charge pump to lowest current
    WRITE_REG(HAL_HSTREG_MAP->PPHYCFGH, 0xF0);      // SATA EQ HP and LP
#if (CHIP_REV >= 0xC0)
    WRITE_FIELD(HAL_HST_REXT, 0x01);                // Use external resistor at REXT pin
#endif

    WRITE_REG(HAL_HSTREG_MAP->SATAINTEN, (HAL_HST_BISTIPEN | HAL_HST_RXPMACKEN | HAL_HST_RXPMNAKEN | HAL_HST_ERRTXRXEN));
#if (DIPM_BY_FW == 0)
    WRITE_FIELD(HAL_HST_RXWKUPREQEN, 1);
    WRITE_FIELD(HAL_HST_IDLESEN, 1);
    WRITE_FIELD(HAL_HST_IDLEPEN, 1);
    WRITE_FIELD(HAL_HST_PMIDLEP, 0x69);
//#if ((VENDOR_NAME == VERIFY) || (VENDOR_NAME == LENOVO))
//    WRITE_FIELD(HAL_HST_PMIDLES, 0x6B);
//#else
    WRITE_FIELD(HAL_HST_PMIDLES, 0x69);
//#endif
#endif

    //We should use WRITE_FIELD here instead of WRITE_REG.
    //WRITE_REG(HAL_HSTREG_MAP->SHCTRL, HAL_HST_DWCLKON|HAL_HST_COMRSTDIS);
    WRITE_FIELD(HAL_HST_DWCLKON, 1);
    WRITE_FIELD(HAL_HST_COMRSTDIS, 1);
	
    /*Set Default Value and set DISF34CNT = 0 in Cstart.s*/
    WRITE_FIELD(HAL_HST_F34RDYCNT,0x1); /* For a JMicro SATA bridge, we should return FIS34 earlier, elvis_20110323 */
    //WRITE_FIELD(HAL_HST_F34RDYCNT,0x27); /* Set FIS34 Ready Count 0x27, PHYRDY 500ms issue FIS34 */

    WRITE_FIELD(HAL_HST_NIENMEN, halHST_NIEN_Manual_En); /*Enable NIEN Manual Control*/

    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG0, PLLCFG0);      // Set up parameters for SYSPLL
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG1, PLLCFG1);      // Set up parameters for SYSPLL
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG2, PLLCFG2);      // Set up parameters for SYSPLL
    WRITE_FIELD(HAL_GLOBAL_SYSPLLN, PLLCON);                // Set up parameters for SYSPLL
    WRITE_FIELD(HAL_GLOBAL_SYSPLLM, PLLCOM);                // Set up parameters for SYSPLL
    WRITE_FIELD(HAL_GLOBAL_SYSPLLK, PLLCOK);                // Set up parameters for SYSPLL

    for (Delay = 0; Delay < 25; Delay++){ }                 // Delay 25 instructions to allow SYSCLK to stablize at Frequency

    // Now set up the clock frequency for the Serial Flash Interface.
    WRITE_FIELD(HAL_AHB_SFLASH_SFCLKSEL,SFCLKSEL);

    // Patch for write PLL lockup
    WRITE_FIELD(HAL_GLOBAL_WPLLPWD, 1);
    WRITE_REG(HAL_GLOBALREG_MAP->DISCSEL, 0x27);            // DVD+R

    /*************************************************************************************/
    /* The SDRAM SDCONFx registers should be programmed at the default frequency of 33 MHz */
    /* Always intialize these registers before changing the frequency on the SYSCLK PLL */
    /*************************************************************************************/
    switch (READ_FIELD(HAL_GLOBAL_EMBEDMEMS))               // Check for the type of embedded SDRAM that is installed.
    {
        case SDRAM_M12L16161AFK1:
            SdramParams.CasLat      = M12L16161AFK1_CASLAT;
            SdramParams.SdWdth32    = M12L16161AFK1_SDWDTH32;
            SdramParams.SdSize      = M12L16161AFK1_SDSIZE;
            SdramParams.Trfc        = M12L16161AFK1_TRFC;
            SdramParams.Twr         = M12L16161AFK1_TWR;
            SdramParams.Tras        = M12L16161AFK1_TRAS;
            SdramParams.Trp         = M12L16161AFK1_TRP;
            SdramParams.Trcd        = M12L16161AFK1_TRCD;
            SdramParams.Novl        = M12L16161AFK1_NOVL;
            SdramParams.Bus32       = M12L16161AFK1_BUS32;
            SdramParams.RefIntv     = M12L16161AFK1_REFINTV;
            SdramParams.SDLchDly    = M12L16161AFK1_SDLCHDLY;
            SdramParams.SDCkoDly    = M12L16161AFK1_SDCKODLY;
            SdramParams.SDInCkSel   = M12L16161AFK1_SDINCKSEL;
            break;

        case SDRAM_M12L32321ADM1:
            //SdramParams.CasLat      = M12L32321ADM1_CASLAT;
            SdramParams.CasLat      = 1;  // CAS Latency = 3, for ESTRON 512K*32bit SDRAM, 20100805_elvis
            SdramParams.SdWdth32    = M12L32321ADM1_SDWDTH32;
            SdramParams.SdSize      = M12L32321ADM1_SDSIZE;

            /************************************************************/
            //SdWdth32 = 1, 32 bit BandWidth
            //SdSize   = 2, 2 MBytes SDRAM
            //There are 2 kinds of 32bit BW SDRAM embeded in SUN720 platform, they are 1M*32bit and 512K*32bit respectively.
            //These 2 kinds of SDRAM use the same bonding regnition.
            //So, we have to use "hard code" here to set the SDRAM configuration.
            /************************************************************/

            SdramParams.Trfc        = M12L32321ADM1_TRFC;
            SdramParams.Twr         = M12L32321ADM1_TWR;
            SdramParams.Tras        = M12L32321ADM1_TRAS;
            //SdramParams.Trp         = M12L32321ADM1_TRP;
            SdramParams.Trp         = 0x01;   //for ESTRON 512K*32bit SDRAM, 20100805_elvis
            //SdramParams.Trcd        = M12L32321ADM1_TRCD;
            SdramParams.Trcd        = 0x01;   //for ESTRON 512K*32bit SDRAM, 20100805_elvis
            SdramParams.Novl        = M12L32321ADM1_NOVL;
            SdramParams.Bus32       = M12L32321ADM1_BUS32;
            //SdramParams.RefIntv     = M12L32321ADM1_REFINTV;
            SdramParams.RefIntv     = 0x30;   //for ESTRON 512K*32bit SDRAM, 20100805_elvis
            SdramParams.SDLchDly    = M12L32321ADM1_SDLCHDLY;
            SdramParams.SDCkoDly    = M12L32321ADM1_SDCKODLY;
            SdramParams.SDInCkSel   = M12L32321ADM1_SDINCKSEL;
            break;

        case SDRAM_M12L32162A:
            SdramParams.CasLat      = M12L32162A_CASLAT;
            SdramParams.SdWdth32    = M12L32162A_SDWDTH32;
            SdramParams.SdSize      = M12L32162A_SDSIZE;
            SdramParams.Trfc        = M12L32162A_TRFC;
            SdramParams.Twr         = M12L32162A_TWR;
            SdramParams.Tras        = M12L32162A_TRAS;
            SdramParams.Trp         = M12L32162A_TRP;
            SdramParams.Trcd        = M12L32162A_TRCD;
            SdramParams.Novl        = M12L32162A_NOVL;
            SdramParams.Bus32       = M12L32162A_BUS32;
            SdramParams.RefIntv     = M12L32162A_REFINTV;
            SdramParams.SDLchDly    = M12L32162A_SDLCHDLY;
            SdramParams.SDCkoDly    = M12L32162A_SDCKODLY;
            SdramParams.SDInCkSel   = M12L32162A_SDINCKSEL;
            break;

        case SDRAM_EXTERNAL:
            SdramParams.CasLat      = EXTERNALSDRAM_CASLAT;
            SdramParams.SdWdth32    = EXTERNALSDRAM_SDWDTH32;
            SdramParams.SdSize      = EXTERNALSDRAM_SDSIZE;
            SdramParams.Trfc        = EXTERNALSDRAM_TRFC;
            SdramParams.Twr         = EXTERNALSDRAM_TWR;
            SdramParams.Tras        = EXTERNALSDRAM_TRAS;
            SdramParams.Trp         = EXTERNALSDRAM_TRP;
            SdramParams.Trcd        = EXTERNALSDRAM_TRCD;
            SdramParams.Novl        = EXTERNALSDRAM_NOVL;
            SdramParams.Bus32       = EXTERNALSDRAM_BUS32;
            SdramParams.RefIntv     = EXTERNALSDRAM_REFINTV;
            SdramParams.SDLchDly    = EXTERNALSDRAM_SDLCHDLY;
            SdramParams.SDCkoDly    = EXTERNALSDRAM_SDCKODLY;
            SdramParams.SDInCkSel   = EXTERNALSDRAM_SDINCKSEL;
            break;

        default:
            while (1) { }           // Not defined. Dead end. Just sit here.
}

    SET_SDCONF0(SdramParams.CasLat, SdramParams.SdWdth32, SdramParams.SdSize);      // Embedded SDRAM: ESMT 16Mbit = 2MByte  16-bit access
    SET_SDCONF1(SdramParams.Trfc,   SdramParams.Twr,      SdramParams.Tras);        // Other settings
    SET_SDCONF4(SdramParams.Trp,    SdramParams.Trcd,     SdramParams.Novl, SdramParams.Bus32);
    WRITE_FIELD(HAL_SDRAM_CKE , 1);                         // Enable self-refresh

    for (Delay = 0; Delay < 200; Delay++) {  }              // Delay 200 instructions

    WRITE_FIELD(HAL_SDRAM_REFINTV, 0);                      // REFINTV - Refresh Disabled
    WRITE_FIELD(HAL_SDRAM_SDCMD, 0x01);                     // Precharge All Banks
    WRITE_FIELD(HAL_SDRAM_SDCMD, 0x00);                     // Auto Refresh
    WRITE_FIELD(HAL_SDRAM_SDCMD, 0x00);                     // Auto Refresh
    WRITE_FIELD(HAL_SDRAM_SDCMD, 0x02);                     // Load Mode Register
    WRITE_FIELD(HAL_SDRAM_REFINTV,  SdramParams.RefIntv);   // Set SDRAM Refresh Interval
    SET_SDDELAY0(SdramParams.SDLchDly, SdramParams.SDCkoDly);
    SET_SDDELAY1(SdramParams.SDInCkSel);
    WRITE_FIELD(HAL_SDRAM_CKE , 1);                         // Enable self-refresh

    sd_backup[0] = SCSDRAM[0];
    sd_backup[1] = SCSDRAM[1];
    sd_backup[2] = SCSDRAM[2];
    sd_backup[3] = SCSDRAM[3];

    sd_backup[4] = SCSDRAM[200];
    sd_backup[5] = SCSDRAM[201];
    sd_backup[6] = SCSDRAM[202];
    sd_backup[7] = SCSDRAM[203];

    SCSDRAM[0x00] = 'S';
    SCSDRAM[0x01] = 'U';
    SCSDRAM[0x02] = 'N';

    if((SCSDRAM[0x200] == 'S')&&(SCSDRAM[0x201] == 'U')&&(SCSDRAM[0x202] == 'N'))
    {
        SCSDRAM[0] = sd_backup[0];
        SCSDRAM[1] = sd_backup[1];
        SCSDRAM[2] = sd_backup[2];
        SCSDRAM[3] = sd_backup[3];
        SCSDRAM[200] = sd_backup[4];
        SCSDRAM[201] = sd_backup[5];
        SCSDRAM[202] = sd_backup[6];
        SCSDRAM[203] = sd_backup[7];

        for (Delay = 0; Delay < 200; Delay++) {  }              // Delay 200 instructions
        SET_SDCONF0(SdramParams.CasLat, SdramParams.SdWdth32, 2);      // Embedded SDRAM: ESMT 16Mbit = 2MByte  16-bit access

        WRITE_FIELD(HAL_SDRAM_CKE , 1);                         // Enable self-refresh

        for (Delay = 0; Delay < 200; Delay++) {  }              // Delay 200 instructions

        WRITE_FIELD(HAL_SDRAM_REFINTV, 0);                      // REFINTV - Refresh Disabled
        WRITE_FIELD(HAL_SDRAM_SDCMD, 0x01);                     // Precharge All Banks
        WRITE_FIELD(HAL_SDRAM_SDCMD, 0x00);                     // Auto Refresh
        WRITE_FIELD(HAL_SDRAM_SDCMD, 0x00);                     // Auto Refresh
        WRITE_FIELD(HAL_SDRAM_SDCMD, 0x02);                     // Load Mode Register
        WRITE_FIELD(HAL_SDRAM_REFINTV,  SdramParams.RefIntv);   // Set SDRAM Refresh Interval
        SET_SDDELAY0(SdramParams.SDLchDly, SdramParams.SDCkoDly);
        SET_SDDELAY1(SdramParams.SDInCkSel);
        WRITE_FIELD(HAL_SDRAM_CKE , 1);                         // Enable self-refresh

    }
    else
    {
        SCSDRAM[0] = sd_backup[0];
        SCSDRAM[1] = sd_backup[1];
        SCSDRAM[2] = sd_backup[2];
        SCSDRAM[3] = sd_backup[3];
        SCSDRAM[200] = sd_backup[4];
        SCSDRAM[201] = sd_backup[5];
        SCSDRAM[202] = sd_backup[6];
        SCSDRAM[203] = sd_backup[7];
    }

    // End of setting up SDRAM

    InitializeSFlash();

    /* Initialize SSI interface */

    /* Baud rate setting */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSICLKDIV,bSSICLKDIV);            /* Clock divider */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSICFG0,HAL_APB_SSI_SSCLKINV);    /* Configure communications Format Invert clock on SSI Interface */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSIINTEN,0);                      /* disable all the interupts */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSIINT,0);                        /* Serial Channel Interrupt Clear */

#if (CHIP_REV >= 0xC0)
    #if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SMODE2, 0);
    #elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SMODE2, 1);
    #endif
#endif

    /*** Host Interrupt Initialization ***/
    WRITE_REG(HAL_HSTREG_MAP->HINTEN0, HAL_HST_SRSTEN | HAL_HST_ARSTEN | HAL_HST_DTEEN | HAL_HST_PCMDEN | HAL_HST_CMDEN ); //shaya
    WRITE_REG(HAL_HSTREG_MAP->HINT0, 0); //shaya

}
/**********************************************************************************
* Name:     InitializeSFlash
*
* Description:  This function initializes the SFlash Interface and Cache controller
*
***********************************************************************************/
void InitializeSFlash(void)
{
    WRITE_FIELD(HAL_AHB_SFLASH_SFEXTW,1);   // Disable Auto Page program.
    WRITE_REG(HAL_AHB_SFLASHREG_MAP->SFCTRL2, HAL_AHB_SFLASH_SFTAGRST);    /* Reset Cache tags and disable cache */
    WRITE_FIELD(HAL_AHB_SFLASH_SFCLKSEL,SFCLKSEL);                         /* Select clock frequency for the Serial Flash Interface. */

#if (ARMC == ENABLE_CACHE_4K)
    WRITE_FIELD(HAL_AHB_SFLASH_SFENCACH, 1);                        /*  bSFCHTEST - Cache Test mode disabled */
                                                                    /*  bSFSLOWRD - Slow Read Disabled */
                                                                    /*  bSFTAGRST - Do not reset Cache tags */
                                                                    /*  bSF2OUT   - Dual Output SPI Flash not supported */
                                                                    /*  bSF8KCACH - Select 4K Cache */
                                                                    /*# bSFENCACH - Serial Cache enabled */
    WRITE_FIELD(HAL_AHB_SFLASH_SFTAGRST, 0);                        /* Clear Cache Tag reset bit */

#elif (ARMC == ENABLE_CACHE_8K)
    WRITE_FIELD(HAL_AHB_SFLASH_SF8KCACH, 1);
    WRITE_FIELD(HAL_AHB_SFLASH_SFENCACH, 1);
                                                                    /*  bSFCHTEST - Cache Test mode disabled */
                                                                    /*  bSFSLOWRD - Slow Read Disabled */
                                                                    /*  bSFTAGRST - Do not reset Cache tages */
                                                                    /*  bSF2OUT   - Dual Output SPI Flash not supported */
                                                                    /*# bSF8KCACH - Select 8K Cache */
                                                                    /*# bSFENCACH - Serial Cache enabled */
    WRITE_FIELD(HAL_AHB_SFLASH_SFTAGRST, 0);                        /* Clear Cache Tag reset bit */

#elif (ARMC == DISABLE_CACHE)
                                                                    /*  bSFCHTEST - Cache Test mode disabled */
                                                                    /*  bSFSLOWRD - Slow Read Disabled */
                                                                    /*  bSFTAGRST - Do not reset Cache tages */
                                                                    /*  bSF2OUT   - Dual Output SPI Flash not supported */
                                                                    /*  bSF8KCACH - Select 4K Cache */
                                                                    /*  bSFENCACH - Serial Cache disabled */
#endif // #elif (ARMC == DISABLE_CACHE)

#if (CHIP_REV >= 0xC0)
    //WRITE_FIELD(HAL_AHB_SFLASH_SF2IO, 1);   /* Enable Dual I/O SPI flash (0xBB)*/
#if (PCB == 710)
   // WRITE_FIELD(HAL_AHB_SFLASH_SF2OUT, 1);   /* Enable Dual Ouput SPI flash */
#else
    WRITE_FIELD(HAL_AHB_SFLASH_SF2OUT, 1);   /* Enable Dual Ouput SPI flash */
#endif
#endif
}


#pragma arm section code

/****************************************************************************
* Name:     InitializeIO
*
* Description:  This function initializes the IOs
*
* Global Outputs: Initialized IO Ports and SFRs.
*****************************************************************************/
#pragma arm section code = "INIT_IO"

//void InitializeIO(void)
//{
//    /*** Configure IO Ports ***/
//    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,PADATA_VALUE);
//    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,PACTRL_VALUE);
//
//#if (PCB == 720)
//    #if (PCB2 == 0)                                 // SUN720 PCB V0.3/V0.4 + B3
//    if (READ_FIELD(HAL_GLOBAL_PARTNUM) == 0x6230)
//    {
//        // For 720BGA PCB
//        WRITE_FIELD(HAL_SRV_PDMCHSEL,1);
//        WRITE_FIELD(HAL_SRV_ALPHASW,0);
//        WRITE_FIELD(HAL_SRV_COLLISW,1);
//    }
//    else
//    {
//        // For 720LQFP PCB
//        WRITE_FIELD(HAL_SRV_PDMCHSEL,0);
//        WRITE_FIELD(HAL_SRV_ALPHASW,1);
//        WRITE_FIELD(HAL_SRV_COLLISW,0);
//    }
//    #else                                           // Others
//    WRITE_FIELD(HAL_SRV_PDMCHSEL,0);
//    WRITE_FIELD(HAL_SRV_ALPHASW,1);
//    WRITE_FIELD(HAL_SRV_COLLISW,0);
//    #endif
//    #if (PCB2 >= 4)
//    // For PCB2 >= 4, TI Colli power-driver, TPIC1391, is used.
//    WRITE_FIELD(HAL_SRV_PDMALPHA1, 0x200);          // Set both ALPHASIN and ALPHACOS to LOW to mute
//    WRITE_FIELD(HAL_SRV_PDMALPHA2, 0x200);          // power driver TPIC1391.
//    #endif
//#else
//    #error No Port A definition.
//#endif
//
//    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,PBDATA_VALUE);
//    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,PBCTRL_VALUE);
//
//    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,PCDATA_VALUE);
//    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,PCCTRL_VALUE);
//
//    /*** Configure Port Muxes ***/
//    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, PAMUX0_VALUE);
//    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX1, PAMUX1_VALUE);
//
//    WRITE_REG(HAL_GLOBALREG_MAP->PBMUX0,PBMUX0_VALUE);
//    WRITE_REG(HAL_GLOBALREG_MAP->PBMUX1, PBMUX1_VALUE);
//
//    /********** GPIOMUX **********/
//#if (PCB == 720)
//    #if (CHIP_REV >= 0xC0)
//    WRITE_REG(HAL_GLOBALREG_MAP->GPIOMUX, GPIOMUX_VALUE);    // Enable VREFPC1EN and EXTINTEN
//    #endif
//#endif
//
//    /********** Pin SEN & SEN3 **********/
//#if (PCB == 720)
//    #if (PCB2 == 0) || (PCB2 == 1)                  // SUN720 PCB V0.3/V0.4 + B3 / V0.4 + C0
//    WRITE_FIELD(HAL_APB_SSI_EXTSEN,1);              // Use PSEN as SEN.
//    WRITE_FIELD(HAL_APB_SSI_PSEN,0);                // Set MDEN3 to 0 (Colli power-driver off).
//    #elif (PCB2 >= 4)                               // DH12E3LH PCB V5A/V5B/V0A/V0B/V2A + B3/C0
//    WRITE_FIELD(HAL_APB_SSI_EXTSEN,1);              // Use PSEN as SEN.
//    WRITE_FIELD(HAL_APB_SSI_PSEN,1);                // Set BDLDON# to 1 (BD LDD off).
//    WRITE_FIELD(HAL_APB_SSI_EXTSEN3,1);             // Use PSEN3 as SEN3.
//    WRITE_FIELD(HAL_APB_SSI_PSEN3,0);               // Set LS to 0.
//    #endif
//#endif
//
//#if (DEBUG_MIO_SETTING == 1)
//    Set_MIO0_TO_GPIO();
//    GIO0(0);
//    Set_MIO1_TO_GPIO();
//    GIO1(0);
//    Set_MIO2_TO_GPIO();
//    GIO2(0);
//    #if (EN_LS == 0)
//    Set_MIO3_TO_GPIO();
//    GIO3(0);
//    #endif
//#endif
//}
//#pragma arm section code

void InitializeIO(void)
        {
/*** Configure IO Ports ***/
/* sun673 Definitions for Port A pins   sun690 Definitions for Port A pins   sun691 Definitions for Port A pins   BCM1600 Definitions for Port A pins    SUN692 Definitions for Port A pins     SUN830 Definitions for Port A pins     SUN812 Definitions for Port A pins   SUN710 Definitions for Port A pins   SUN815 Definitions for Port A pins   SUN817 Definitions for Port A pins
   ** PA.7 : ipCloseSW     1 - input    ** PA.7 : ipCloseSW     1 - input    ** PA.7 : ipCloseSW     1 - input    ** PA.7 : opMMUTE        0 - output    ** PA.7 : opMMUTE        0 - output    ** PA.7 : MMUTE            0 - output  ** PA.7 : opMMUTE        0 - output  ** PA.7 : opMMUTE        0 - output  ** PA.7 : opMMUTE        0 - output  ** PA.7 : ipHOME         1 - input
   ** PA.6 : ipEJECT       1 - input    ** PA.6 : ipEJECT       1 - input    ** PA.6 : ipEJECT       1 - input    ** PA.6 : ipEJECT        1 - input     ** PA.6 : ipEJECT        1 - input     ** PA.6 : EJECT            1 - input   ** PA.6 : ipEJECT        1 - input   ** PA.6 : ipEJECT        1 - input   ** PA.6 : ipEJECT        1 - input   ** PA.6 : ipEJECT        1 - input
   ** PA.5 : ipOpen        1 - input    ** PA.5 : ipOpen        1 - input    ** PA.5 : ipOpen        1 - input    ** PA.5 : opSMUTE        0 - output    ** PA.5 : opSMUTE        0 - output    ** PA.5 : SMUTE            0 - output  ** PA.5 : opSMUTE        0 - output  ** PA.5 : opSMUTE        0 - output  ** PA.5 : opSMUTE        0 - output  ** PA.5 : opMDEN2        0 - output
   ** PA.4 : opMDEN2       0 - output   ** PA.4 : opMDEN2       0 - output   ** PA.4 : opMDEN2       0 - output   ** PA.4 : ipALPCOSOUT    1 - input     ** PA.4 : Alpha COS      1 - input     ** PA.4 : ALPHACOS/COL_COS 0 - output  ** PA.4 : Alpha COS      1 - input   ** PA.4 : Alpha COS      1 - input   ** PA.4 : Alpha COS      1 - input   ** PA.4 : Alpha COS      0 - output
   ** PA.3 : opMDEN1       0 - output   ** PA.3 : opMDEN1       0 - output   ** PA.3 : opMDEN1       0 - output   ** PA.3 : ipALPSINOUT    1 - input     ** PA.3 : Alpha SIN      1 - input     ** PA.3 : ALPHASIN/COL_SIN 0 - output  ** PA.3 : Alpha SIN      1 - input   ** PA.3 : Alpha SIN      1 - input   ** PA.3 : Alpha SIN      1 - input   ** PA.3 : Alpha SIN      0 - output
   ** PA.2 : ipURXD        1 - input    ** PA.2 : ipURXD        1 - input    ** PA.2 : ipURXD        1 - input    ** PA.2 : ipURXD         1 - input     ** PA.2 : ipURXD         1 - input     ** PA.2 : UARTRXD/RXD      1 - input   ** PA.2 : ipURXD         1 - input   ** PA.2 : ipURXD         1 - input   ** PA.2 : ipURXD         1 - input   ** PA.2 : ipURXD         1 - input
   ** PA.1 : opUTXD        0 - output   ** PA.1 : opUTXD        0 - output   ** PA.1 : opUTXD        0 - output   ** PA.1 : opUTXD         0 - output    ** PA.1 : opUTXD/SEN4    0 - output    ** PA.1 : UARTTXD/TXD      0 - output  ** PA.1 : opUTXD/SEN4    0 - output  ** PA.1 : opUTXD/SEN4    0 - output  ** PA.1 : opUTXD/SEN4    0 - output  ** PA.1 : opUTXD/SEN4    0 - output
   ** PA.0 : opACTRST      0 - output   ** PA.0 : opDRIVE_ON    0 - output   ** PA.0 : opSEL2        0 - output   ** PA.0 : ipTRAYCEN      1 - input     ** PA.0 : ipSA_INT       1 - input     ** PA.0 : DVDCLK/+ND       0 - output  ** PA.0 : ipSA_INT       1 - input   ** PA.0 : ipSA_INT       1 - input   ** PA.0 : ipOEICSW1      1 - input   ** PA.0 : opMDEN1        0 - output
*/
#if ((PCB == 690) || (PCB == 691))
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x01);      /* 0000,0001 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0xE4);      /* 1: input, 0:output */
#elif (PCB == 1600)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5D);      /* 1: input, 0:output */
#elif (PCB == 692)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5D);      /* 1: input, 0:output */
#elif (PCB == 812)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5D);      /* 1: input, 0:output */
#elif (PCB == 710)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5D);      /* 1: input, 0:output */
    WRITE_FIELD(HAL_AFE_PDFVREF,1);                 /* set power down FVREF  */
#elif (PCB == 711)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5C);      /* 1: input, 0:output */
    WRITE_FIELD(HAL_AFE_PDFVREF,1);                 /* set power down FVREF  */
#elif (PCB == 815)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5D);      /* 1: input, 0:output */
    WRITE_FIELD(HAL_AFE_PDFVREF,1);                 /* set power down FVREF  */
#elif (PCB == 817)
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0xC4);      /* 1: input, 0:output */
    WRITE_FIELD(HAL_AFE_PDFVREF,1);                 /* set power down FVREF  */
#else
    ...Add here for new platform.
#endif

/* sun673 Definitions for Port B pins   sun690 Definitions for Port B pins   sun691 Definitions for Port B pins   BCM1600 Definitions for Port B pins    SUN692 Definitions for Port B pins     SUN830 Definitions for Port B pins      SUN812 Definitions for Port B pins   SUN710 Definitions for Port B pins   SUN815 Definitions for Port B pins      SUN817 Definitions for Port B pins
   ** PB.7 : MIO3         1 - input     ** PB.7 : MIO3          1 - input    ** PB.7 : MIO3          1 - input    ** PB.7 : opMIO3        1 - input      ** PB.7 : MIO3           1 - input     ** PB.7 : MIO3/OENC/ND-  0 - output     ** PB.7 : MIO3           1 - input   ** PB.7 : opSWB2         0 - output  ** PB.7 : MIO3           1 - output     ** PB.7 : opPWRSTS       0 - output
   ** PB.6 : MIO2         1 - input     ** PB.6 : MIO2          1 - input    ** PB.6 : MIO2          1 - input    ** PB.6 : opMIO2        1 - input      ** PB.6 : ipOpenSw/MIO2  1 - input     ** PB.6 : MIO2/OPEN      1 - input      ** PB.6 : ipOpenSw/MIO2  1 - input   ** PB.6 : ipOpenSw/MIO2  1 - input   ** PB.6 : ipOpenSw/MIO2  1 - input      ** PB.6 : opNTRST        0 - output
   ** PB.5 : MIO1         1 - input     ** PB.5 : MIO1          1 - input    ** PB.5 : MIO1          1 - input    ** PB.5 : opMIO1        1 - input      ** PB.5 : ipCloseSw/MIO1 1 - input     ** PB.5 : MIO1/CLOSE     1 - input      ** PB.5 : ipCloseSw/MIO1 1 - input   ** PB.5 : ipCloseSw/MIO1 1 - input   ** PB.5 : ipCloseSw/MIO1 1 - input      ** PB.5 : opTDO          0 - output
   ** PB.4 : MIO0         1 - input     ** PB.4 : MIO0          1 - input    ** PB.4 : MIO0          1 - input    ** PB.4 : opMIO0        1 - input      ** PB.4 : MIO0           1 - input     ** PB.4 : MIO0/TDI       0 - output     ** PB.4 : MIO0           1 - input   ** PB.4 : opCD_EN        0 - output  ** PB.4 : MIO0           1 - output     ** PB.4 : opTDI          1 - input
   ** PB.3 : ipPB3        1 - input     ** PB.3 : opLDEN        0 - output   ** PB.3 : opPB3         0 - output   ** PB.3 : opCD_RLD      0 - output     ** PB.3 : opPI_CONT      0 - output    ** PB.3 : WGATE/RxW      0 - output     ** PB.3 : opDVD_CD       0 - output  ** PB.3 : opDVD_EN       0 - output  ** PB.3 : opLDDENB       0 - output     ** PB.3 : ipOpenSW       1 - input
   ** PB.2 : ipPB2        1 - input     ** PB.2 : opXRST        0 - output   ** PB.2 : opSEL1        0 - output   ** PB.2 : opDVD_RLD     0 - output     ** PB.2 : opXRST         0 - output    ** PB.2 : SA_INT         1 - input      ** PB.2 : opPB2          0 - output  ** PB.2 : ipHOLD         1 - input   ** PB.2 : ipPB2          1 - input      ** PB.2 : ipCloseSW      1 - input
   ** PB.1 : ipACTFLG     1 - input     ** PB.1 : ipHOME        1 - input    ** PB.1 : ipHOME        1 - input    ** PB.1 : opBD_RLD      0 - output     ** PB.1 : ipHOME         1 - input     ** PB.1 : LAND/HOME      1 - input      ** PB.1 : ipHOME         1 - input   ** PB.1 : opDRIVE_ON     0 - output  ** PB.1 : ipOEICSW2      1 - output     ** PB.1 : indet          1 - input
   ** PB.0 : opReadyLED   0 - output    ** PB.0 : opReadyLED    0 - output   ** PB.0 : opReadyLED    0 - output   ** PB.0 : opReadyLED    0 - output     ** PB.0 : opReadyLED     0 - output    ** PB.0 : LED            0 - output     ** PB.0 : opReadyLED     0 - output  ** PB.0 : opReadyLED     0 - output  ** PB.0 : opReadyLED     0 - output     ** PB.0 : opTRAYSTS      0 - output
*/

#if (PCB == 690)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0xF2);      /* 1: input, 0:output */
#elif (PCB == 1600)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0xF0);      /* 1: input, 0:output */
#elif (PCB == 691)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0xF2);      /* 1: input, 0:output */
#elif (PCB == 692)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x08);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0xF2);      /* 1: input, 0:output */
#elif (PCB == 812)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0xF2);      /* 1: input, 0:output */
#elif (PCB == 710)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x08);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0x64);      /* 1: input, 0:output */
#elif (PCB == 711)
  #if (SET_SLOT_IN == 1) //[Q36] for InitializeIO
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x2A);      /* 0010, 1010 */   
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0x66);	    /* 1: input, 0:output */
  #else
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x08);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0x64);      /* 1: input, 0:output */
  #endif
#elif (PCB == 815)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0xF6);      /* 1: input, 0:output */
#elif (PCB == 817)
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 1000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0x1E);      /* 1: input, 0:output */
#else
    ...Add here for new platform.
#endif

 /* sun673 Definitions for Port C pins   sun690 Definitions for Port C pins   sun691 Definitions for Port C pins   BCM1600 Definitions for Port C pins    SUN692 Definitions for Port C pins    SUN830 Definitions for Port C pins SUN812 Definitions for Port C pins  SUN710 Definitions for Port C pins SUN815 Definitions for Port C pins  SUN817 Definitions for Port C pins
    ** PC.3 : ipPC3         1 - input    ** PC.3 : ipSA_INT      1 - input    ** PC.3 : opPDG2        0 - output   ** PC.3 : ipSA_INT      1 - input      ** PC.3 : NU      1 - input           ** PC.3 : NU      1 - input        ** PC.3 : NU      1 - input         ** PC.3 : NU      1 - input        ** PC.3 : NU      1 - input         ** PC.3 : NU         1 - input
    ** PC.2 : opSPGS        0 - output   ** PC.2 : ipPC2         1 - input    ** PC.2 : ipPC2         1 - input    ** PC.2 : ipCloseSW     1 - input      ** PC.2 : NU      1 - input           ** PC.2 : NU      1 - input        ** PC.2 : NU      1 - input         ** PC.2 : NU      1 - input        ** PC.2 : NU      1 - input         ** PC.2 : NU         1 - input
    ** PC.1 : opSELCD       0 - output   ** PC.1 : opPI_CONT     0 - output   ** PC.1 : opPDG1        0 - output   ** PC.1 : ipOpenSW      1 - input      ** PC.1 : NU      1 - input           ** PC.1 : NU      0 - output       ** PC.1 : NU      1 - input         ** PC.1 : NU      1 - input        ** PC.1 : NU      1 - input         ** PC.1 : opDVD_RLD  0 - output
    ** PC.0 : OENC          1 - input    ** PC.0 : ipPC0         1 - input    ** PC.0 : ipPC0         1 - output   ** PC.0 : opDRIVE_ON    0 - output     ** PC.0 : NU      1 - input           ** PC.0 : NU      1 - input        ** PC.0 : NU      1 - input         ** PC.0 : NU      1 - input        ** PC.0 : NU      1 - input         ** PC.0 : opSEN      0 - output
 **/
#if (PCB == 690)
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0D);      /* 1: input, 0:output */
#elif (PCB == 1600)
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x01);      /* 0000, 0001 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0E);      /* 1: input, 0:output */
#elif (PCB == 691)
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x05);      /* 1: input, 0:output */
#elif (PCB == 692) // Port C not used on SUN692. This PCB uses 216/128 LQFP package
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0F);      /* 1: input, 0:output */
#elif (PCB == 812) // Port C not used on SUN812. This PCB uses 128 LQFP package
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0C);      /* 1: input, 0:output */
#elif (PCB == 710) // Port C not used on SUN710. This PCB uses 128 LQFP package
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0C);      /* 1: input, 0:output */
#elif (PCB == 711) // Port C not used on SUN711. This PCB uses 128 LQFP package.
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0C);      /* 1: input, 0:output */
#elif (PCB == 815) // Port C not used on SUN815. This PCB uses 128 LQFP package
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0F);      /* 1: input, 0:output */
#elif (PCB == 817) // Port C on SUN817. This PCB uses 128 LQFP package
    WRITE_REG(HAL_PERIPHREG_MAP->PCDATA,0x00);      /* 0000, 0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PCCTRL,0x0C);      /* 1: input, 0:output */
#else // PCB
      ...Add here for new platform.
#endif // PCB

/*** Configure Port Muxes ***/

/********** Pins PA0-PA7 ***********/
/*
register rwPAMux0 definition
    Bit 7 (R/W) Reserved
    Bit 6 (R/W) EINTIN - External Interrupt input select
        1: Enable PA.6 input to interrupt
        0: PA.6 is normal Input/Output port
    Bit 5 (R/W) - PANICIN - PANICIN input
        1: PANICIN Input
        0: PA5 input/output  (Default)
    Bit 4 (R/W) ALPCOSOUT - Alpha Cosine Output
        1: Select ALPCOSOUT output
        0: PA.4 is normal Input/Output port
    Bit 3 (R/W) ALPSINOUT - Alpha Sine Output
        1: Select ALPSINOUT output
        0: PA.3 is normal Input/Output port
    Bit 2:(R/W) - UARTRXIN - RxD UART selection
        1: Select RxD as Input
        0: PA.2 is normal Input/Output port
    Bit 1:(R/W) - UARTTXOUT - Txd UART selection
        1: Select Txd as Output
        0: PA.1 is normal Input/Output port
    Bit 0 (R/W) GNSW2OUT - Gain Switch 2 output
        1: Select GNSW2OUT as Output
        0: PA.0 is normal Input/Output port
*/

#if (PCB == 1600)
    #if (ENABLE_UART_TXD == 1)
        #if ENABLE_UART_RXD == 1
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5E);  //EINTIEN, ALPCOSOUT, ALPSINOUT, RX, TX
        #else
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5A);  //EINTIEN, ALPCOSOUT, ALPSINOUT, TX
        #endif
    #elif(ENABLE_UART_RXD == 1)
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5C);  //EINTIEN, ALPCOSOUT, ALPSINOUT, RX
    #else
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x58);  //EINTIEN, ALPCOSOUT, ALPSINOUT
    #endif
#elif ((PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 815)||(PCB == 817))
    #if (ENABLE_UART_TXD == 1)
        #if (ENABLE_UART_RXD == 1)
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5E);  // EINTIEN, RX, TX, Alpha SIN, Alpha COS
        #else
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5A);  // EINTIEN, TX, Alpha SIN, Alpha COS
        #endif
    #elif(ENABLE_UART_RXD == 1)
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5C);  // EINTIEN, RX, Alpha SIN, Alpha COS
    #else
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x58);  // EINTIEN, Alpha SIN, Alpha COS
    #endif

#else //((PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812))
    #if (ENABLE_UART_TXD == 1)
        #if ENABLE_UART_RXD == 1
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x46);  //EINTIEN, RX, TX
        #else
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x42);  //EINTIEN, TX
        #endif
    #elif(ENABLE_UART_RXD == 1)
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x44);  //EINTIEN, RX
    #else
    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x40);  //EINTIEN
    #endif
#endif // PCB

/********** Pins PB0-PB7 **********/
/* register rwPBMux0 definition
    Bit 7 (R/W) - MIO3SEL - MIO3 Input/Output on PB7 Pin
        1: MIO3 input/output
        0: PB7 input/output  (Default)
    Bit 6 (R/W) - MIO2SEL - MIO2 Input/Output on PB6 Pin
        1: MIO2 input/output
        0: PB6 input/output  (Default)
    Bit 5 (R/W) - MIO1SEL - MIO1 Input/Output on PB5 Pin
        1: MIO1 input/output
        0: PB5 input/output  (Default)
    Bit 4 (R/W) - MIO0SEL - MIO0 Input/Output on PB4 Pin
        1: MIO0 input/output
        0: PB4 input/output  (Default)
    Bit 3-0: (R/W) - SATDBGSEL(3-0) - SATA Debug bus
        TBD? COMMENT! need to add pins for debug bus
*/

/*** The MIO pins should be set as GPIO Inputs to reduce PCB IO noise ***/
/*** When using the MIO pins for debug you must first configure them as GPIOs **/
    WRITE_REG(HAL_GLOBALREG_MAP->PBMUX0, 0x00);      /* Default the MIOs should be inputs to reduce IO noise */


/********** Pins PB7-PB1 **********/
/* register rwPBMux0 definition
    Bit 7 (R/W) - OENCEN
        1: OENCEN as input on PB.7
        0: PB.7 input/output
    Bit 3 (R/W) - WGATEOUT -
        1: Select WGATE as output on PB.3
        0: PB.3 is used as normal IO pin
    Bit 2 (R/W) - DVDCLKOUT
        1: Select DVD Clock as output on PB.2
        0: PB.2 is used as normal IO pin
    Bit 1 (R/W) - LANDGOUT
        1: Select DVDRAM Land/Groove as output on PB.1
        0: PB.1 is used as normal IO pin

**/

    WRITE_REG(HAL_GLOBALREG_MAP->PBMUX1, 0x04);  // Use GPIOs as default I/O pins //[Q36] for Aging test

    /********** GPIOMUX **********/
#if (PCB == 711) || (PCB == 710) || (PCB == 817)
    #if (CHIP_REV >= 0xC0)
    WRITE_REG(HAL_GLOBALREG_MAP->GPIOMUX, GPIOMUX_VALUE);    // Enable VREFPC1EN and EXTINTEN
    #endif
#endif

/* BCM1600 board require to set extra port pins */
#if (PCB == 1600)
    /* Set SEN pin high to disable Blue PDIC */
    WRITE_REG(HAL_APB_SSIREG_MAP->XWSCFG0, (HAL_APB_SSI_EXTSEN | HAL_APB_SSI_PSEN));

    /* Set GAINSW to Hi-Z to disable DVD and CD PDIC. */
    WRITE_REG(HAL_WSREG_MAP->GAINSWCTL, ((halWS_GAINSW2W_HiZ1<<HAL_WS_GAINSW2W_LSB)|(halWS_GAINSW2W_HiZ1<<HAL_WS_GAINSW2R_LSB)|(halWS_GAINSW2W_HiZ1<<HAL_WS_GAINSWW_LSB)|halWS_GAINSW2W_HiZ1));

    /* Disable Laser Oscillation */
    WRITE_REG(HAL_WSREG_MAP->OSCENCTL, (HAL_WS_MANOSCEN));
    //WRITE_REG(HAL_WSREG_MAP->OSCENCTL, (HAL_WS_MANOSCEN | HAL_WS_OSCENI));
#endif // (PCB == 1600)


#if 0 // Debug code to monitor the cache hit, miss and valid signals on MIOs
    Set_MIO0_TO_MIO();
    Set_MIO1_TO_MIO();
    Set_MIO2_TO_MIO();
    Set_MIO3_TO_GPIO();
    WRITE_REG(HAL_GLOBAL_M0SEL,0x1f);    // Cache Hit
    WRITE_REG(HAL_GLOBAL_M1SEL,0x1f);    // ~Cache Miss
    WRITE_REG(HAL_GLOBAL_M2SEL,0x1f);    // Valid Tag
    GIO3(0);                            // Set trigger low
#endif // 0

    }
#pragma arm section code

/****************************************************************************
* Name:     InitializeInterrupts
*
* Description:  This function initializes the interrupt controller
*
*****************************************************************************/
//void InitializeInterrupts(void)//720
//{
//    WRITE_FIELD(HAL_AHB_INTCTRL_ITEN, 1);                       // Disable global interrupt.
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICINTSEL, 0);             // Select all Interrupts as IRQ (not FIQ !)
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICINTEN , 0);             // Disable all interrupts.
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICIENCLR, 0xFFFFFFFF);    // Clear all Interrupt Enable s
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICSWINT , 0);             // Clear all Sowtware Interrupt Enables
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICSWICLR, 0xFFFFFFFF);    // Clear all Sowtware Interrupts
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICPROT  , 0);             // Disable Protection. for all Interrupt Controller Registers.
//
//    //-----------------------------------------------------------------------------;
//    // These priority settings have to agree with the setting inside               ;
//    // "func_isr_tbl", which is inside file named "intctl.c"                       ;
//    //-----------------------------------------------------------------------------;
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP02MSK, ( HAL_AHB_INTCTRL_OPCINT2_MASK    ));  // Auto OPC Int.
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP03MSK, (  HAL_AHB_INTCTRL_ENCINT3_MASK
//                                                 | HAL_AHB_INTCTRL_CPYINT3_MASK
//                                                 | HAL_AHB_INTCTRL_WSINT3_MASK    ));  // CD/DVD Encoder Int, Copy Int, Write Strategy Int  Priority  2 Mask Register   Bits specified are corresponding Interrupt Numbers 0-31
//
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP04MSK, (  HAL_AHB_INTCTRL_DECINT4_MASK   ));  // CD/DVD Decoder Int                                Priority  3 Mask Register   Bits specified are corresponding Interrupt Numbers 0-31
//
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP05MSK, (  HAL_AHB_INTCTRL_HSTINT5_MASK
//                                                 | HAL_AHB_INTCTRL_AUDINT5_MASK   ));  // Host, Audio
//
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP06MSK, (  HAL_AHB_INTCTRL_EXTINT6_MASK   ));  // External (Eject switch)
//
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP07MSK, (  HAL_AHB_INTCTRL_SVDINT7_MASK
//                                                 | HAL_AHB_INTCTRL_SVSINT7_MASK
//                                                 | HAL_AHB_INTCTRL_WBLINT7_MASK   ));  // Servo DSP, Servo Hardware Int, Wobble Int.
//
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP08MSK, ( HAL_AHB_INTCTRL_T2INT8_MASK     ));  // Timer 2 (Servo FG input)
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP09MSK, ( HAL_AHB_INTCTRL_T0INT9_MASK     ));  // Timer 0 (Kernel timer)
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP10MSK, ( HAL_AHB_INTCTRL_T1INT10_MASK    ));  // Timer 1 (Player Timer)
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP11MSK, ( HAL_AHB_INTCTRL_T3INT11_MASK    ));  // Timer 3 (ROPC Timer)
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP12MSK, ( HAL_AHB_INTCTRL_DFEINT12_MASK   ));  // Digital Front End (contains Jitter HW )
//
//    #if (ENABLE_HDDVD && ENABLE_AACS)
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP13MSK, ( HAL_AHB_INTCTRL_DMDINT13_MASK   ));  // demod
//    #else
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP13MSK, ( HAL_AHB_INTCTRL_LSINT13_MASK    ));  // Light Scribe
//    #endif
//
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP14MSK, ( HAL_AHB_INTCTRL_T5INT14_MASK    ));  // Timer 5
//    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP15MSK, ( HAL_AHB_INTCTRL_UARTINT15_MASK  ));  // UART
//
//}

void InitializeInterrupts(void)//710
{
    WRITE_FIELD(HAL_AHB_INTCTRL_ITEN, 1);                       // Disable global interrupt.
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICINTSEL, 0);             // Select all Interrupts as IRQ (not FIQ !)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICINTEN , 0);             // Disable all interrupts.
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICIENCLR, 0xFFFFFFFF);    // Clear all Interrupt Enable s
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICSWINT , 0);             // Clear all Sowtware Interrupt Enables
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICSWICLR, 0xFFFFFFFF);    // Clear all Sowtware Interrupts
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICPROT  , 0);             // Disable Protection. for all Interrupt Controller Registers.

    //-----------------------------------------------------------------------------;
    // These priority settings have to agree with the setting inside               ;
    // "func_isr_tbl", which is inside file named "INTCTL.C"                       ;
    //-----------------------------------------------------------------------------;
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP02MSK, (  HAL_AHB_INTCTRL_ENCINT2_MASK
                                                 | HAL_AHB_INTCTRL_CPYINT2_MASK
                                                 | HAL_AHB_INTCTRL_WSINT2_MASK    ));  // CD/DVD Encoder Int, Copy Int, Write Strategy Int  Priority  2 Mask Register   Bits specified are corresponding Interrupt Numbers 0-31

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP03MSK, (  HAL_AHB_INTCTRL_DECINT3_MASK   ));  // CD/DVD Decoder Int                                Priority  3 Mask Register   Bits specified are corresponding Interrupt Numbers 0-31

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP04MSK, (  HAL_AHB_INTCTRL_HSTINT4_MASK
                                                 | HAL_AHB_INTCTRL_AUDINT4_MASK   ));  // Host, Audio

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP05MSK, (  HAL_AHB_INTCTRL_EXTINT5_MASK   ));  // External (Eject switch)

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP06MSK, (  HAL_AHB_INTCTRL_SVDINT6_MASK
                                                 | HAL_AHB_INTCTRL_SVSINT6_MASK
                                                 | HAL_AHB_INTCTRL_WBLINT6_MASK   ));  // Servo DSP, Servo Hardware Int, Wobble Int.

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP07MSK, ( HAL_AHB_INTCTRL_T2INT7_MASK     ));  // Timer 2 (Servo FG input)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP08MSK, ( HAL_AHB_INTCTRL_T0INT8_MASK     ));  // Timer 0 (Kernel timer)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP09MSK, ( HAL_AHB_INTCTRL_T1INT9_MASK     ));  // Timer 1 (Player Timer)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP10MSK, ( HAL_AHB_INTCTRL_T3INT10_MASK    ));  // Timer 3 (ROPC Timer)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP11MSK, ( HAL_AHB_INTCTRL_DFEINT11_MASK   ));  // Digital Front End (contains Jitter HW )

#if(ENABLE_HDDVD && ENABLE_AACS)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP12MSK, ( HAL_AHB_INTCTRL_DMDINT12_MASK    ));  // demod
#else // (ENABLE_HDDVD && ENABLE_AACS)
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP12MSK, ( HAL_AHB_INTCTRL_LSINT12_MASK    ));  // Light Scribe
#endif // (ENABLE_HDDVD && ENABLE_AACS)

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP13MSK, ( HAL_AHB_INTCTRL_OPCINT13_MASK   ));  // Auto OPC Int.
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP14MSK, ( HAL_AHB_INTCTRL_T5INT14_MASK    ));  // Timer 5
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICP15MSK, ( HAL_AHB_INTCTRL_UARTINT15_MASK  ));  // UART
}


void InitializeSATA(void)
{
#if 0 //Chip default settings
    WRITE_REG(HAL_HSTREG_MAP->PPHYCFGM, 0x00);
    WRITE_REG(HAL_HSTREG_MAP->PPHYCFGH, 0xB4);
    WRITE_REG(HAL_HSTREG_MAP->GPHYCFGL, 0x40);
    WRITE_REG(HAL_HSTREG_MAP->GPHYCFGH, 0x2C);
#endif


    WRITE_REG(HAL_HSTREG_MAP->PPHYCFGL, 0x17);      // Increase TX main swing to reduce jitter.
    WRITE_FIELD(HAL_HST_SATPLLBIS, 0x00);           // Set charge pump to lowest current
    WRITE_REG(HAL_HSTREG_MAP->PPHYCFGH, 0xF0);      // SATA EQ HP and LP

#if (CHIP_REV >= 0xC0)
    WRITE_FIELD(HAL_HST_REXT, 0x01);                // Use external resistor at REXT pin
#endif
#if 0
    WRITE_FIELD(HAL_HST_SSCCTL, e_SSCCTL_ON);
#endif

#if (DIPM_BY_FW == 0)
    WRITE_FIELD(HAL_HST_PMAUTO_EN,0x01);  //Enable HW DIPM Auto function
    WRITE_FIELD(HAL_HST_DIPM_EN,0x00);    //Disable DIPM by default, power-on
#endif

#if (SSC_ENABLED == 1)
    WRITE_FIELD(HAL_HST_SSCCTL,    e_SSCCTL_ON);
    WRITE_FIELD(HAL_HST_SSCDEVSEL,    0); //20101110_02UX == Modify SSC setting to improve SATA compatibility, Elvis modify, Elvis Confirm
#endif
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
