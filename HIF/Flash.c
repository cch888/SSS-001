//****************************************************************************
//*                (c) Copyright 2002 - 2008 SUNEXT DESIGN, INC
//*                           All Rights Reserved
//*
//* $Revision: 130 $
//* $Date: 11/03/16 6:29p $
//*
//* DESCRIPTION
//*
//*
//**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\hif\flash.h"
#include ".\hif\MMC.h"
#include ".\common\sdram.h"
#include ".\common\macros.h"
#include ".\common\scomm.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_global.h"
#include ".\al\reg_periph.h"
#include ".\al\reg_sflash.h"
#if (SUN_FUNC_TEST == 1)
#include ".\al\reg_write_strategy.h"
#endif

#define SEND_FLASH_DEBUG_MSG 1 //For send debug message
#define SF_VERIFY_WRITEDATA 0 //for serial flash verify write data
extern void ResetHandler(void);

void FlashWriteFW( void );
static void FlashFirmware( void );
static void FlashParaTable( void );
void FlashEraseSector(ULONG address, ULONG size);

void SFlashWriteData(ULONG address, ULONG length, ULONG sourceaddr,USHORT sector_size);

static void BackupParaToSDRAM( void );
static void RestoreParaFromSDRAM( void );

//#if (PCB == 710)||(PCB == 711)||(PCB == 817)
//USHORT CalculateEEPROMCheckSUM(const BYTE*);
//void WriteEEPROMCheckSUMIntoSDRAM(void);
//BYTE CheckEEPROMTagProtection(BYTE);
//void WriteEEPROMTagProtectionIntoSDRAM(void);
//#endif

#if (SUN_FUNC_TEST == 1)
USHORT CalculateEEPROMCheckSUM(const BYTE*);
void WriteEEPROMCheckSUMIntoSDRAM(void);
BYTE CheckEEPROMTagProtection(BYTE);
void WriteEEPROMTagProtectionIntoSDRAM(void);
BYTE CheckEEPROMSumProtection(BYTE eTable);
BYTE CheckDramSourceValidity(void);
void ChangeLEDStatus(void);
#endif

typedef void (*pFunc)(void);

typedef struct {
    ULONG size;
    USHORT count;
}FLASH_ROM_MAP;

/********************************************/
/*** Supported Serial Flash ROMs Commands ***/
/********************************************/
#define sfWriteEnable           0x06    // Write Enable command - no address
#define sfWriteDisable          0x04    // Write Disable command - no address
#define sfReadStatusReg         0x05    // Read Status Register - SI7-SI0
#define sfWriteStatusReg        0x01    // Write Status Register - SO7-SO0
#define sfReadData              0x03    // Read Data - A23-A16:A15-A8:A7-A0
#define sfFastRead              0x0B    // Read Fast Data - A23-A16:A15-A8:A7-A0
#define sfFastReadDualOutput    0x3B    // Read Fast Dual Output Data - A23-A16:A15-A8:A7-A0
#define sfProgramOneDataByte    0x02    // Program one data Byte
#define sfPageProgram           0x02    // Page Program - A23-A16:A15-A8:A7-A0
#define sf64KBlockErase         0xD8    // Block Erase - A23-A16:A15-A8:A7-A0
#define sfSectorErase           0x20    // Sector Erase - A23-A16:A15-A8:A7-A0
#define sfChipErase             0xC7    // Chip Erase - no address
#define sfPowerDown             0xB9    // Power Down
#define sfReleasePDandDevID     0xAB    // Release Power-down and Device ID - A23-A16:A15-A8:A7-A0 (dummy)
#define sfManfDevID             0x90    // Manufacturer and Device ID - A23-A16(dummy):A15-A8(dummy):00h
#define sfJEDECID               0x9F    // JEDEC ID -Manf: Mem Type: Capacity

#if(FLASH_ROM_SIZE == FLASH_ROM_1MB)
// FLASH ROM TYPE == SST25VF080B         /* ----- SST25VF080 ----- */
// FLASH ROM TYPE == MX25LV8005         /* ----- MX25LV8005 ----- */
/* Erase & Write BLOCK MASK (first 16 blocks)*/
//#define ERASE_BLOCK_MASK_6  0x07FE  /* b'0000011111111100 *//* 1 = Not Erase:para, This must match .SCF file */
#if (PCB == 710)||(PCB == 711)||(PCB == 815)||(PCB == 817)
#if(BOOT_CODE_SUPPORT)
#define ERASE_BLOCK_MASK_6  0xFFF0  /* b'11111111 11110000 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*   |||||||| ||||||||               */
                                    /*   |||||||| |||||||+ -0 boot+flash.o         */
                                    /*   |||||||| ||||||+ --1 bootcode */
                                    /*   |||||||| |||||+ ---2 bootcode */
                                    /*   |||||||| ||||+ ----3 bootcode */
                                    /*   |||||||| |||+ ---- 4 PARA table */
                                    /*   |||||||| ||+ ----  5 PARA table */
                                    /*   |||||||| |+ ----   6 keypara table */
                                    /*   |||||||| + ----    7 keypara table */
                                    /*   ++++++++ ----      8 MKB table */
#else
#define ERASE_BLOCK_MASK_6  0x0000  /* b'1111111111110000 *//* 1 = Not Erase:para, This must match .SCF file */
#endif
#else
#define ERASE_BLOCK_MASK_6  0x07FE  /* b'0000011111111100 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*        |||||||||||               */
                                    /*        ||||||||||+- boot         */
                                    /*        |||||||||+-- flash        */
                                    /*        ||||||||+--- para/eeprom  */
                                    /*        ++++++++---- mkb          */
#endif
const FLASH_ROM_MAP FlashBlockMap_6[1] = {
    { 0x1000, 256 },                /* 4kbyte * 256 sectors = 1 MByte */
};

// FLASH ROM TYPE == ST M25PE80         /* ----- ST M25PE80 ----- */
/* Erase & Write BLOCK MASK (first 16 blocks)*/
#if (PCB == 710)||(PCB == 711)||(PCB == 815)||(PCB == 817)
#define ERASE_BLOCK_MASK_8  0x0001  /* b'0000000000000000 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*        |||||||||||               */
                                    /*        ||||||||||+- boot         */
                                    /*        |||||||||+-- flash        */
                                    /*        ||||||||+--- para/eeprom  */
                                    /*        ++++++++---- mkb          */
#else
#define ERASE_BLOCK_MASK_8  0x0000  /* b'0000000000000000 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*        |||||||||||               */
                                    /*        ||||||||||+- boot         */
                                    /*        |||||||||+-- flash        */
                                    /*        ||||||||+--- para/eeprom  */
                                    /*        ++++++++---- mkb          */
#endif
const FLASH_ROM_MAP FlashBlockMap_8[1] = {
    { 0x10000, 16 },                /* 64kbyte * 16 sectors = 1 MByte */
};
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_2MB)
// FLASH ROM TYPE == MXIC MX25L160D         /* ----- MXIC MX25L160D ----- */
/* Erase & Write BLOCK MASK (first 8 blocks)*/
#define ERASE_BLOCK_MASK_9  0x0000  /* b'0000000000000000 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*        |||||||||||               */
                                    /*        ||||||||||+- boot         */
                                    /*        |||||||||+-- flash        */
                                    /*        ||||||||+--- para/eeprom  */
                                    /*        ++++++++---- mkb          */

const FLASH_ROM_MAP FlashBlockMap_9[1] = {
    { 0x20000, 16 },                /* 2 MByte */
};
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
// FLASH ROM TYPE == SST25VF040B         /* ----- SST25VF040 ----- */
/* Erase & Write BLOCK MASK (first 16 blocks)*/
#define ERASE_BLOCK_MASK_7  0x07FE  /* b'0000011111111110 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*        |||||||||||               */
                                    /*        ||||||||||+- boot         */
                                    /*        |||||||||+-- flash        */
                                    /*        ||||||||+--- para/eeprom  */
                                    /*        ++++++++---- mkb          */
const FLASH_ROM_MAP FlashBlockMap_7[1] = {
    { 0x1000, 128 },                /* 4kbyte * 128 sector = 512KBytes */
};

// FLASH ROM TYPE == SPANSION botton boot 512K        /* ----- SPANSION S25FL040A ----- */
/* Erase & Write BLOCK MASK */
#define ERASE_BLOCK_MASK_10  0x0000  /* b'0000000000000000 *//* 1 = Not Erase:para, This must match .SCF file */
                                    /*        |||||||||||               */
                                    /*        ||||||||||+- boot         */
                                    /*        |||||||||+-- flash        */
                                    /*        ||||||||+--- para/eeprom  */
                                    /*        ++++++++---- mkb          */
const FLASH_ROM_MAP FlashBlockMap_10[4] = {
    { 0x4000, 2 },
    { 0x1000, 2 },
    { 0x3000, 2 },
    { 0x10000, 7 },
};
#endif
#define ERASE_BLOCK_MASK_4K_2M  0xFFF0  /* b'11111111 11110000 *//* 1 = Not Erase:para, This must match .SCF file */
                                        /*   |||||||| ||||||||               */
                                        /*   |||||||| |||||||+ -0 boot+flash.o         */
                                        /*   |||||||| ||||||+ --1 bootcode */
                                        /*   |||||||| |||||+ ---2 bootcode */
                                        /*   |||||||| ||||+ ----3 bootcode */
                                        /*   |||||||| |||+ ---- 4 PARA table */
                                        /*   |||||||| ||+ ----  5 PARA table */
                                        /*   |||||||| |+ ----   6 keypara table */
                                        /*   |||||||| + ----    7 keypara table */
                                        /*   ++++++++ ----      8 MKB table */

const FLASH_ROM_MAP FlashBlockMap_4K_2M[2] = {
    { 0x1000, 32 },                /* 4kbyte * 32 sectors = 128 KByte */
    { 0x10000, 30 },                /* 64kbyte * 30 sectors = 1920 KByte */
};
BYTE    gFlashTableNum;
#if (SUN_FUNC_TEST == 1)
BYTE    fgEEPROMChg;
#endif
#if(BOOT_CODE_SUPPORT)
BYTE    WriteBootCode;
#endif
#if (STORE_MSG_TO_SDRAM == 1)
ULONG   flashpara_Store_msg_addr;
BYTE    flashpara_LoggingFull;
#endif
/* MACRO defines */
#define FlashWriteB( addr, data )   (*(vBYTE *)(FLASH_BASE + addr) = (BYTE)(data))
#define FlashReadB( addr )          (*(vBYTE *)(FLASH_BASE + addr))

#define SramAddressFunc( addr )     ((pFunc)(BYTE *)((BYTE *)(addr) - (BYTE *)FLASH_BLK_START + (BYTE *)FLASH_PROG))()

#define RESET_HANDLER_ADDR          0x30000000    /* ResetHandler     0x30000010   ARM Code  0  CSTART.o(Cstart) */
#define Jump_To_Reset_Handler       ((pFunc)(BYTE *)(RESET_HANDLER_ADDR))

#define END_OF_BLOCK        FLASH_ROM_SIZE      /* boundary of last block */

#if (SEND_FLASH_DEBUG_MSG == 1)
#define SERIAL_INTERFACE_ADDRESS  0xCF000000
static void delay_msgf(void)
{
   BYTE i;

   for (i = 0; i < 100; i++);
}

static void send_msgf(BYTE d1)
{
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_1 ) = 0x00;  /*  SYNC_PATT      */
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = 0xAA;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = 0x54;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d1;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d1;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d1;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d1;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_1 ) = 0x55;
    delay_msgf();
}

static void send_msgf1(BYTE d1,BYTE d2,BYTE d3,BYTE d4)
{
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_1 ) = 0x00;  /*  SYNC_PATT      */
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = 0xAA;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = 0x54;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d1;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d2;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d3;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_2 ) = d4;
    *(vBYTE *)(SERIAL_INTERFACE_ADDRESS + ADAPTER_ADDR_1 ) = 0x55;
    delay_msgf();
}
#else
#define send_msgf(d1) {}
#define send_msgf1(d1,d2,d3,d4) {}
#endif


//*************************************************************************
//* IdentifyChip()
//*
//* Automatically Identify Flash ROM Type.
//* Currently Supports the following Flash ROMS:
//*   SST     SST39LF040
//*   Atmel   AT49F004
//*   MXIC    MX29LV004B
//*   FUJISTU MBM29LV400BC(IDAT49F004)
//* Returns:  Flash Rom Type
//* Params:   none
//*************************************************************************/

WORD IdentifyChip(void)
{
    BYTE manuf_code, device_code;
    BYTE capacity_code;
    USHORT id_code;

#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('I','D','F','Y');
#endif

#if (EN_DOSPI_SUPPORT == 1)
    /*this workaround is for dual out put enable, but JEDEC ID command cann't working well
       see bugzilla #5228 */
    WRITE_FIELD(HAL_AHB_SFLASH_SF2OUT, 0);   /* Clear Dual Out Put SPI flash */

    {
            vBYTE  *flash_addr,i, data;

            flash_addr = (vBYTE *)FLASH_BASE;
            for(i = 0; i <0xF0; i++)
                data += (vBYTE)flash_addr[i];  /*Read some data from Sflash*/
    }
#endif

    capacity_code = 0;
    /*-----------------------------------------*/
    /*      CHIP IDENTIFICATION SEQUENCE       */
    /*-----------------------------------------*/

    /* Send read JEDEC ID command to SFlash ROM */
    WRITE_FIELD(HAL_AHB_SFLASH_ADDRREG, 0x000000);
    WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfJEDECID);

    /*** The ARM will hold and automatically wait for return data ***/

    /* Read Manufacturer and device id from the device */
    manuf_code  = READ_FIELD(HAL_AHB_SFLASH_MANUFACT);
    id_code = READ_FIELD(HAL_AHB_SFLASH_IDREG);
    capacity_code = BLOW(id_code);
    device_code = BHIGH(id_code);

#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1(0xb1,manuf_code,device_code,capacity_code);
#endif

#if(FLASH_ROM_SIZE == FLASH_ROM_1MB)
/***   ESMT Serial Flash ROM. Capacity is 1MBytes   ***/
/*** manuf_code = 8Ch, device_code = 20h, capacity_code = 14h ***/
    if ((manuf_code == MAN_ESMT) && (device_code == F25L08PA_TYPE)
	&& (capacity_code == F25L08PA_CAPACITY))
	{

        return (WORD)F25L08PA;
	}

/***    SPANSION Serial Flash ROM. Capacity is 2MB, treated as 1MB  ***/
/*** manuf_code = 01h, device_code = 02h, capacity_code = 14h ***/
    if ((manuf_code == MAN_SPANSION) && (device_code == S25FL016A_TYPE)
    && (capacity_code == S25FL016A_CAPACITY))   //2MB
    {
        return (WORD)STM25P80;
    }

/***    SST 25VF080B Serial Flash ROM. Capacity is 1MBytes    ***/
/*** manuf_code = BFh, device_code = 25h, capacity_code = 8Eh ***/
    if ((manuf_code == MAN_SST) && (device_code == SST_25VF)
    && (capacity_code == SST_SST25VF080_ID))    // 1MB SST
    {
        return (WORD)SST25VF080;
    }

/***    ST M25PE80 Serial Flash ROM. Capacity is 1MBytes    ***/
/*** manuf_code = 20h, device_code = 80h, capacity_code = 14h ***/
    if ((manuf_code == MAN_ST) && (device_code == ST_M25PE80_ID)
    && (capacity_code == ST_CAPACITY))    // 1MB SST
    {
        return (WORD)STM25PE80;
    }

    if ((manuf_code == MAN_M25P80) && (device_code == ST_M25P80_ID)
    && (capacity_code == ST_M25P80_CAPACITY))    // 1MB SST
    {
        return (WORD)STM25P80;
    }

/***    Winbond  Serial Flash ROM. Capacity is 1MBytes    ***/
/*** manuf_code = EFh, device_code = 30h, capacity_code = 14h ***/
    if ((manuf_code == MAN_WINBOND) && (device_code == WB_W25X80_ID)
    && (capacity_code == WB_CAPACITY))    // 1MB MXIC
    {
        return (WORD)WBW25X80;
    }

/***    Winbond  Serial Flash ROM. Capacity is 1MBytes    ***/
/*** manuf_code = EFh, device_code = 40h, capacity_code = 14h ***/
    if ((manuf_code == MAN_WINBOND) && (device_code == WB_W25Q80_ID)
    && (capacity_code == WB_CAPACITY))    // 1MB MXIC
    {
        return (WORD)WBW25Q80;
    }

/***    MXIC  Serial Flash ROM. Capacity is 1MBytes    ***/
/*** manuf_code = C2h, device_code = 20h, capacity_code = 14h ***/
    if ((manuf_code == MAN_MXIC) && (device_code == MXIC_20VF)
    && (capacity_code == MXIC_MX25LV8005_ID))    // 1MB MXIC
    {
        return (WORD)MX25LV8005;
    }

    if ((manuf_code == MAN_MXIC) && (device_code == MXIC_20VF)
    && (capacity_code == MXIC_MX25L8006_ID))    // 1MB MXIC
    {
        return (WORD)MX25L8006;
    }

/***    SANYO Serial Flash ROM. Capacity is 1MBytes    ***/
/*** manuf_code = 62h, device_code = 26h, capacity_code = 00h ***/
    if ((manuf_code == MAN_SANYO) && (device_code == SANYO_LE25FW806_ID)
    && (capacity_code == SANYO_CAPACITY))    // 1MB MXIC
    {
        return (WORD)LE25FW806MA;
    }
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_2MB)
/***    MXIC  Serial Flash ROM. Capacity is 2MBytes    ***/
/*** manuf_code = C2h, device_code = 20h, capacity_code = 15h ***/
    if ((manuf_code == MAN_MXIC) && (device_code == MXIC_MX25L160D_ID)
    && (capacity_code == MXIC_CAPACITY_2M))    // 2MB MXIC
    {
        return (WORD)MX25L1605D;
    }
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
/***   SST 25VF040B Serial Flash ROM. Capacity is 512Bytes    ***/
/*** manuf_code = BFh, device_code = 25h, capacity_code = 8Dh ***/
    if ((manuf_code == MAN_SST) && (device_code == SST_25VF)
    && (capacity_code == SST_SST25VF040_ID))    // 512KB SST
    {
        return (WORD)SST25VF040;
    }

/***    SPANSION Serial Flash ROM. Capacity is 512KBytes    ***/
/*** manuf_code = 01h, device_code = 02h, capacity_code = 26h ***/
    if ((manuf_code == MAN_SPANSION) && (device_code == S25FL040A)
    && (capacity_code == SS_CAPACITY))  // 512KB
    {
        return (WORD)S25FL040AB;
    }
#endif
#if (PCB==817)
/***    Winbond  Serial Flash ROM. Capacity is 2MBytes    ***/
/*** manuf_code = EFh, device_code = 40h, capacity_code = 15h ***/
    if ((manuf_code == MAN_WINBOND) && (device_code == WB_W25Q16BV_ID)
    && (capacity_code == MXIC_CAPACITY_2M))    // 2MB MXIC
    {
        return (WORD)WBW25Q16BV;
    }
#endif    
    /*******************************************************************/
    /* Can't find known device type! DEAD LOOP HERE: Failure indicated */
    /*******************************************************************/
#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('I','D','E','R');
#endif

    while (1) ;
}
//*************************************************************************
//* ChangeChipStatus(BYTE status)
//* Turn on/off serial flash status register write protection.
//* Returns:
//* Params:  status register content
//*************************************************************************/
void ChangeChipStatus(BYTE status)
{
    LONG timercount;

    timercount = 15000;
    /* Clear Status Register */
    WRITE_FIELD(HAL_AHB_SFLASH_SFEXTW,0);                       //Enable Auto Page program.
    WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteEnable);           // Block Write protection
    WRITE_REG(HAL_AHB_SFLASHREG_MAP->STATEREG, status);
    WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfWriteStatusReg);       // Write Status Register command

    if(*FLASH_TYPE != F25L08PA)
    {
        do
        {
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfReadStatusReg);    // Read Status Register command
            timercount--;
        }while((READ_FIELD(HAL_AHB_SFLASH_SFBUSY) == FALSE)&&(timercount > 0));         // Wait for BUSY
    }

    do
    {
        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfReadStatusReg);     // Read Status Register command
    }while(READ_FIELD(HAL_AHB_SFLASH_SFBUSY));                  // Wait for Not BUSY
    WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteDisable);           // Block Write protection
    WRITE_FIELD(HAL_AHB_SFLASH_SFEXTW,1);                       //Disable Auto Page program.
}
//*************************************************************************
//* FlashFirmware()
//*
//* Main Flash ROM Erase/Write Procedure
//*   This code is copied to the Internal SRAM where it is executed.
//*   Before this is called the new image has been transferred to SDRAM and
//*   CRC checked.
//* Returns:  void
//* Params:   none
//*************************************************************************/
static void FlashFirmware( void )
{
    ULONG Addr;
    ULONG NextAddr;
    USHORT blkCnt = 0;
    USHORT blkPhase = 0;
    BYTE data;
    USHORT  program_size;
    vBYTE *flash_base_addr;
    BYTE *dest;
    USHORT MaxCount=0;

    ULONG i, *flash_write_addr, *sd_write_addr;
    BYTE writeflash, firsterase;


    FLASH_ROM_MAP *FlashBlockSize;
    data=0;
    firsterase = FALSE;
#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('F','F','L','H');
#endif

/***************************************************/
/***            Identify Flash Rom               ***/
/***************************************************/
    *FLASH_TYPE = IdentifyChip();

#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('T','P',BHIGH(*FLASH_TYPE),BLOW(*FLASH_TYPE));
#endif

    /* Already copy Flash code and RO data from Flash(0x30001000-0x30001B00) to SRAM(0x1000-0x1B00) */
    switch (*FLASH_TYPE)
    {
#if(FLASH_ROM_SIZE == FLASH_ROM_1MB)
        case SST25VF080:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_6;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_6 - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =1;    //ByteProgram
            break;
        case MX25LV8005:
        case MX25L8006:
        case F25L08PA:
        case LE25FW806MA:
        case WBW25X80:
        case WBW25Q80:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_6;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_6 - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =256;    //PageProgram
            break;
        case STM25PE80:
        case STM25P80:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_8;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_8 - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =256;    //PageProgram
            break;
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
        case SST25VF040:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_7;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_7 - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =1;    //ByteProgram
            break;
        case S25FL040AB:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_10;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_10 - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =256;    //PageProgram
            break;
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_2MB)
        case MX25L1605D:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_9;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_9 - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =256;//PageProgram
            break;
#endif
#if (PCB==817)
        case WBW25Q16BV:
            *ERASE_BLOCK_MASK=ERASE_BLOCK_MASK_4K_2M;
            FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_4K_2M - (ULONG)FLASH_BASE - (ULONG)FLASH_CODE_OFFSET));
            program_size =256;//PageProgram
            break;
#endif
    }

#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('S','B','L','K');
#endif

#if (PCB == 710)||(PCB == 711)||(PCB == 815)||(PCB == 817)
        #define PROTECT_START PARA_BLK_START_OFFSET //This must match .SCF file
        #define PROTECT_LENGTH ( PARA_BLK_SIZE + KEY_PARA_SIZE + MKB_BLK_SIZE) //This must match .SCF file
#if (SUN_FUNC_TEST == 0)
        #define CHK_EE_ADDRESS 0x4400L      //This must match .SCF file
#else
        #define CHK_EE_ADDRESS 0x12400L      //This must match .SCF file
#endif

        flash_base_addr  = (vBYTE *)FLASH_BASE + CHK_EE_ADDRESS;

        if((*flash_base_addr == 'S') && (*(flash_base_addr+1) == 'U') && (*(flash_base_addr+2) == 'N'))
        {
            flash_base_addr  = (vBYTE *)FLASH_BASE + PROTECT_START;

            dest = (BYTE *)SCSDRAM + PROTECT_START;
            for (blkCnt = 0; blkCnt < PROTECT_LENGTH; blkCnt++)
            {
                *dest = *flash_base_addr;
                dest++;
                flash_base_addr++;
            }
            send_msgf1('C','O','P','Y');
            *ERASE_BLOCK_MASK = 0xFFF0; //update bootcode only.
        }
#if(BOOT_CODE_SUPPORT)
        if(WriteBootCode)//mctest
        {
            *ERASE_BLOCK_MASK = 0x0000;/* b'00000000 00000000 *//* 1 = Not Erase:*/
        }                              /*   |||||||| ||||||||                    */
                                       /*   |||||||| |||||||+ -0 boot+flash.o    */
                                       /*   |||||||| ||||||+ --1 bootcode */
                                       /*   |||||||| |||||+ ---2 bootcode */
                                       /*   |||||||| ||||+ ----3 bootcode */
                                       /*   |||||||| |||+ ---- 4 PARA table */
                                       /*   |||||||| ||+ ----  5 PARA table */
                                       /*   |||||||| |+ ----   6 keypara table */
                                       /*   |||||||| + ----    7 keypara table */
                                       /*   ++++++++ ----      8 MKB table */
#endif
#else
        #define SLWSO_START_ADDRESS 0x1000L //This must match .SCF file
        #define SLWSO_LENGTH 0x1000         //This must match .SCF file
#if ((PCB == 690)||(PCB == 692)||(PCB == 812)||(PCB == 1600)||(PCB == 691))
        #define CHK_EE_ADDRESS 0x2400L      //This must match .SCF file
#else // what else?
        #define CHK_EE_ADDRESS 0x2280L      //This must match .SCF file
#endif //((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 1600)||(PCB == 691)||(PCB == 815)||(PCB == 817))
        #define EE_START_ADDRESS 0x2000L    //This must match .SCF file
        #define EE_LENGTH 0x1000            //This must match .SCF file
        #define MKB_START_ADDRESS 0x3000L   //This must match .SCF file
        #define MKB_LENGTH 0x8000           //This must match .SCF file



        flash_base_addr  = (vBYTE *)FLASH_BASE + SLWSO_START_ADDRESS;
        dest = (BYTE *)SCSDRAM + SLWSO_START_ADDRESS;
        for (blkCnt = 0; blkCnt < SLWSO_LENGTH; blkCnt++)
        {
            *dest = *flash_base_addr;
            dest++;
            flash_base_addr++;
        }

        flash_base_addr  = (vBYTE *)FLASH_BASE + CHK_EE_ADDRESS;

        if((*flash_base_addr == 'S') && (*(flash_base_addr+1) == 'U') && (*(flash_base_addr+2) == 'N'))
        {
            flash_base_addr  = (vBYTE *)FLASH_BASE + EE_START_ADDRESS;
            dest = (BYTE *)SCSDRAM + EE_START_ADDRESS;
            for( blkCnt = 0;  blkCnt < EE_LENGTH; blkCnt++)
            {
                *dest = *flash_base_addr;
                dest++;
                flash_base_addr++;
            }
            send_msgf1('C','O','P','Y');
        }

        flash_base_addr  = (vBYTE *)FLASH_BASE + MKB_START_ADDRESS;
        dest = (BYTE *)SCSDRAM + MKB_START_ADDRESS;
        for (blkCnt = 0; blkCnt < MKB_LENGTH; blkCnt++)
        {
            *dest = *flash_base_addr;
            dest++;
            flash_base_addr++;
        }
#endif

        ChangeChipStatus(0x00);

/***************************************************/
/***                 Segment Write               ***/
/***************************************************/
    MaxCount = FlashBlockSize[0].count;//initial first blkPhase count

    for( Addr=0L, blkCnt=0, blkPhase=0, NextAddr=0 ; Addr < END_OF_BLOCK ; )
    {

        if( blkCnt >= MaxCount)
        {
            blkPhase++;
            MaxCount+=FlashBlockSize[blkPhase].count;
                send_msgf1('%',blkCnt,blkPhase,MaxCount);

        }
        NextAddr += FlashBlockSize[blkPhase].size;
        if(blkCnt < 16)
        {
            /* check mask block (16 blocks are maskable at the top of the memory) */
            if(((*ERASE_BLOCK_MASK) & (0x01 << blkCnt)) == 0)   /* Check Erase/Write Block */
            {  /* block is not masked and is writable */

                flash_write_addr = (ULONG *)(FLASH_BASE + Addr);
                sd_write_addr = (ULONG *)(SCSDRAM_START_ADDR + Addr);
                writeflash = FALSE;
                for (i = 0; i < (FlashBlockSize[blkPhase].size)/4; i++)
                {
                    if (flash_write_addr[i] != sd_write_addr[i])    /*Compare Flashrom and Dram data*/
                    {
                        writeflash = TRUE;
                        break;
                    }
                }

                if(writeflash == TRUE)  /*Compare fail, update flash rom data */
                {
                    if(firsterase == FALSE)
                    {
                        FlashEraseSector( END_OF_BLOCK - 0x1000, 0x1000); //Erase last sector to destroy EOF tag
                        firsterase = TRUE;
                    }
                    FlashEraseSector( Addr, FlashBlockSize[blkPhase].size );
                    SFlashWriteData(Addr,(ULONG)FlashBlockSize[blkPhase].size,Addr,program_size);
                }

            }
        }
        else
        {   /* Non Maskable block */

            flash_write_addr = (ULONG *)(FLASH_BASE + Addr);
            sd_write_addr = (ULONG *)(SCSDRAM_START_ADDR + Addr);
            writeflash = FALSE;
            for (i = 0; i < (FlashBlockSize[blkPhase].size)/4; i++)
            {
                if (flash_write_addr[i] != sd_write_addr[i])    /*Compare Flashrom and Dram data*/
                {
                    writeflash = TRUE;
                    break;
                }
            }

            if(writeflash == TRUE)  /*Compare fail, update flash rom data */
            {
                if(firsterase == FALSE)
                {
                    FlashEraseSector( END_OF_BLOCK - 0x1000, 0x1000); //Erase last sector to destroy EOF tag
                    firsterase = TRUE;
                }
                FlashEraseSector( Addr, FlashBlockSize[blkPhase].size );
                SFlashWriteData(Addr,(ULONG)FlashBlockSize[blkPhase].size,Addr,program_size);
            }


        }

        if((Addr%0x10000L) == 0)
        {
	#if(SUN_FUNC_TEST == 0)
            opReadyLED = (~ opReadyLED);
	#else
            ChangeLEDStatus();
       #endif
        }

        blkCnt++;
        Addr = NextAddr;
    }
    ChangeChipStatus(0x1C);

#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf(0xf9);
#endif

    /*** Flashing is complete! Go to Drive Restart ***/
    Jump_To_Reset_Handler();
}

///*************************************************************************
//* FlashParaTable()
//*
//* Performs Block Erase Followed by ROM Write
//*
//* Returns:  void
//* Params:   none
//*************************************************************************/
static void FlashParaTable( void )
{
    BYTE *dest;
    BYTE *source = (BYTE *)DRVPARA_DATA;
    LONG   dwsize;
    USHORT  program_size;
    LONG   MkbSize,tempsize,index;
#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
    BYTE    blkCnt,blkPhase;
    ULONG Addr;
    ULONG NextAddr;
    FLASH_ROM_MAP *FlashBlockSize;
#endif
    MkbSize = PARA_BLK_SIZE;
    switch( gFlashTableNum)
    {
#if (SUN_FUNC_TEST == 1)
        case eEEPROM_Formal_Table:
            dest = (BYTE *)AGAIN_SL_OFFSET;
            source = (BYTE *)DRVPARA_DATA;
            //MkbSize = AGAIN_SL_SIZE;
            break;
        case eEEPROM_Backup_Table:
            dest = (BYTE *)AGAIN_SL_BKUP_OFFSET;
            source = (BYTE *)DRVPARA_DATA;
            //MkbSize = AGAIN_SL_BKUP_SIZE;
            break;
        case eKey_parameter_Backup_Table:
            dest = (BYTE *)KEY_PARA_BKUP_OFFSET;
            source = (BYTE *)DRVPARA_DATA;
            break;
#endif
#if  SERVOKEYPARAMETER == 1
        case eKey_parameter_Table:
            dest = (BYTE *)KEY_PARA_OFFSET;
            source = (BYTE *)DRVPARA_DATA;
            //MkbSize = KEY_PARA_SIZE;
            break;
#endif
#if (STORE_MSG_TO_FLASHROM == 1)
        case eFlash_Debugging_Table:
            dest = (BYTE *)DEBUG_MSG_OFFSET;
            source = (BYTE *)MSG_DATA;
            MkbSize = DEBUG_MSG_SIZE;
            break;
#endif
        case ePersistent_Talbe:
            dest = (BYTE *)PARA_BLK_START_OFFSET;
            source = (BYTE *)DRVPARA_DATA;
            //MkbSize = PARA_BLK_SIZE;
            break;

#if (SUN_FUNC_TEST == 1)
        case ePersistent_Backup_Talbe:
            dest = (BYTE *)PARA_BLK_BKUP_START_OFFSET;
            source = (BYTE *)DRVPARA_DATA;
            break;            
#endif

#if (ENABLE_AACS == 1)
        case eMKB_Table:
            dest = (BYTE *)MKB_BLK_START_OFFSET;
            source = (BYTE *)MKBPARA_DATA;
            MkbSize = MKB_BLK_SIZE;
            break;
        case eAACS_Table:
            dest = (BYTE *)PARA_BLK_START_OFFSET;
            break;
#endif
        default:
            send_msgf1('N','O','S','P');
            return;
    }

    *FLASH_TYPE = IdentifyChip();

    ChangeChipStatus(0x00);
    switch (*FLASH_TYPE)
    {
#if(FLASH_ROM_SIZE == FLASH_ROM_1MB)
        case SST25VF080:
            program_size = 1;
            dwsize = 0x1000; /* 4K */
            break;
        case MX25LV8005:
        case MX25L8006:
        case F25L08PA:
        case LE25FW806MA:
        case WBW25X80:
        case WBW25Q80:
            program_size = 256;
            dwsize = 0x1000; /* 4K */
            break;
#endif
#if (PCB==817)
        case WBW25Q16BV:
            program_size = 256;
            dwsize = 0x1000; /* 4K */
            break;
#endif
#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
        case SST25VF040:
            program_size = 1 ;
            dwsize = 0x1000; /* 4K */
            break;
#endif
//64K sector
        case STM25PE80:
        case STM25P80:
        case MX25L1605D:
        case S25FL040AB:
            // need to modify the code when add (0Ah) instruction into SC5230/SC5220 chip
            {
                BYTE Status;
                ULONG OffsetAddr,maxwsize;
                BYTE *tempdest;
                BYTE *tempsource;

#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
                if((*FLASH_TYPE == S25FL040AB)&&(gFlashTableNum != eMKB_Table))
                    maxwsize = 0x4000L;
                else
#endif
                    maxwsize = 0x10000L;
                program_size = 256;
                OffsetAddr =((ULONG)dest & maxwsize);//switch sector start
                tempsource  = (BYTE *)FLASH_BASE+OffsetAddr ;
                tempdest = (BYTE *)SCSDRAM+OffsetAddr;
                for( dwsize = 0;  dwsize < maxwsize; dwsize++)
                {
                    *tempdest = *tempsource;
                    tempdest++;
                    tempsource++;
                }

                send_msgf1('C','O','P','Y');

#if (ENABLE_AACS == 1)
                if (gFlashTableNum == eAACS_Table)
                {
                    tempdest = (BYTE *)SCSDRAM + PARA_BLK_START_OFFSET + PARA_AACSKEY_OFFSET;
                    for( dwsize = PARA_AACSKEY_OFFSET;  dwsize < (PARA_AACSKEY_OFFSET + FLASH_AACSKEY_SIZE); dwsize++)
                    {
                        *tempdest = SCSDRAM[DRVPARA_DATA+dwsize];
                        tempdest++;
                    }
                }
                else if (gFlashTableNum == eMKB_Table)
                {
                    tempdest = (BYTE *)SCSDRAM + MKB_BLK_START_OFFSET;
                    for( dwsize = 0;  dwsize < MKB_BLK_SIZE; dwsize++)
                    {
                        *tempdest = SCSDRAM[dwsize+MKBPARA_DATA];
                        tempdest++;
                    }
                }
                else
#endif
#if  SERVOKEYPARAMETER == 1
                if (gFlashTableNum == eKey_parameter_Table)
                {
                    tempdest = (BYTE *)SCSDRAM + KEY_PARA_OFFSET;
                    for( dwsize = 0;  dwsize < KEY_PARA_SIZE; dwsize++)
                    {
                        *tempdest = SCSDRAM[DRVPARA_DATA+dwsize];
                        tempdest++;
                    }
                }
                else
#endif
#if (SUN_FUNC_TEST == 1)
                if (gFlashTableNum == eEEPROM_Formal_Table)
                {
                    tempdest = (BYTE *)SCSDRAM + AGAIN_SL_OFFSET;
                    for( dwsize = 0;  dwsize < AGAIN_SL_SIZE; dwsize++)
                    {
                        *tempdest = SCSDRAM[DRVPARA_DATA+dwsize];
                        tempdest++;
                    }
                }
                else if (gFlashTableNum == eEEPROM_Backup_Table)
                {
                    tempdest = (BYTE *)SCSDRAM + AGAIN_SL_BKUP_OFFSET;
                    for( dwsize = 0;  dwsize < PARA_BLK_BKUP_SIZE; dwsize++)
                    {
                        *tempdest = SCSDRAM[DRVPARA_DATA+dwsize];
                        tempdest++;
                    }
                }
                else
#endif

#if (STORE_MSG_TO_FLASHROM == 1)
                if (gFlashTableNum == eFlash_Debugging_Table)
                {
                    tempdest = (BYTE *)SCSDRAM + DEBUG_MSG_OFFSET;
                    index = flashpara_Store_msg_addr - LOGGING_MSG_START_ADDR;
                    if(flashpara_LoggingFull == TRUE)
                    {
                       for( dwsize = index;  dwsize < (DEBUG_MSG_SIZE); dwsize++)
                        {
                            *tempdest = SCSDRAM[LOGGING_MSG_START_ADDR+dwsize];
                            tempdest++;
                        }
                    }
                    for( dwsize = 0;  dwsize < index; dwsize++)
                    {
                        *tempdest = SCSDRAM[LOGGING_MSG_START_ADDR+dwsize];
                        tempdest++;
                    }
                }
                else
#endif
                {
                    tempdest = (BYTE *)SCSDRAM + PARA_BLK_START_OFFSET;
                    for( dwsize = 0;  dwsize < PARA_BLK_SIZE - 0x200; dwsize++)
                    {
                        *tempdest = SCSDRAM[DRVPARA_DATA+dwsize];
                        tempdest++;
                    }
                }

                //do 64KBlockErase (0xD8)
#if (ENABLE_AACS == 1)
#if(FLASH_ROM_SIZE == FLASH_ROM_512KB)
                if((gFlashTableNum == eMKB_Table)&&(*FLASH_TYPE == S25FL040AB))
                {
                    FlashBlockSize = (FLASH_ROM_MAP *)((ULONG)SRAM_BASE + ((ULONG)FlashBlockMap_10 - (ULONG)FLASH_BASE));
                    for( Addr=0L, blkCnt=0, blkPhase=0, NextAddr=0 ; Addr<maxwsize ; )
                    {

                        if( blkCnt >= FlashBlockSize[blkPhase].count ){
                            blkPhase++;
                        }
                        NextAddr += FlashBlockSize[blkPhase].size;
                        do
                        {
                            WRITE_FIELD(HAL_AHB_SFLASH_ADDRREG, Addr);
                            WRITE_REG(HAL_AHB_SFLASHREG_MAP->STATEREG, 0x00);//  Remove
                            if (*FLASH_TYPE == STM25P80)
                                WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfWriteStatusReg);    // Write Status Register command
                            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteEnable); //     Block Write protection
                            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfReadStatusReg); // Read Status Register command     0x05
                            Status = READ_REG(HAL_AHB_SFLASHREG_MAP->STATEREG);
                        } while(((Status & 0x02) == FALSE) | (Status & 0x3C) != 0);      // Wait for "Device Write Enabled" and "Block Write Protection = off"

                        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sf64KBlockErase); // 0xd8   Start erasing Sector.
                        do
                        {
                            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfReadStatusReg);  // Read Status Register command
                        }while(READ_FIELD(HAL_AHB_SFLASH_SFBUSY) == FALSE);      // Wait for BUSY

                        blkCnt++;
                        Addr = NextAddr;
                    }

                }
                else
#endif
#endif
                {
                    do
                    {
                        WRITE_FIELD(HAL_AHB_SFLASH_ADDRREG, 0x000000);
                        WRITE_REG(HAL_AHB_SFLASHREG_MAP->STATEREG, 0x00);//  Remove
                        if (*FLASH_TYPE == STM25P80)
                            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfWriteStatusReg);    // Write Status Register command
                        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteEnable); //     Block Write protection
                        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfReadStatusReg); // Read Status Register command     0x05
                        Status = READ_REG(HAL_AHB_SFLASHREG_MAP->STATEREG);
                    } while(((Status & 0x02) == FALSE) | (Status & 0x3C) != 0);      // Wait for "Device Write Enabled" and "Block Write Protection = off"

                    WRITE_FIELD(HAL_AHB_SFLASH_ADDRREG, OffsetAddr);
                    WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sf64KBlockErase); // 0xd8   Start erasing Sector.

                    do
                    {
                        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfReadStatusReg);  // Read Status Register command
                    }while(READ_FIELD(HAL_AHB_SFLASH_SFBUSY) == FALSE);      // Wait for BUSY
                }
                WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteDisable);  // Disable writing to SFlash

                send_msgf1('S','T','W','R');
// dest = flash Sector offset , source = DRAM offset
                SFlashWriteData((ULONG)OffsetAddr,(ULONG)maxwsize,(ULONG)OffsetAddr,(USHORT)program_size);

                send_msgf1('W','R','E','D');
                ChangeChipStatus(0x1C);
                return;
            }
            break;
    }

#if (ENABLE_AACS == 1)
    if (gFlashTableNum == eAACS_Table)
    {
        source  = (BYTE *)FLASH_BASE + PARA_BLK_START_OFFSET;
        dest = (BYTE *)SCSDRAM + DRVPARA_DATA;
        for( tempsize = 0;  tempsize < 0xE00L; tempsize++)
        {
            *dest = *source;
            dest++;
            source++;
        }

        dest   = (BYTE *)PARA_BLK_START_OFFSET;
        source = (BYTE *)DRVPARA_DATA;
    }
    else if (gFlashTableNum == ePersistent_Talbe)
    {
        source  = (BYTE *)FLASH_BASE + PARA_BLK_START_OFFSET + PARA_AACSKEY_OFFSET;
        dest = (BYTE *)SCSDRAM + DRVPARA_DATA + PARA_AACSKEY_OFFSET;
        for( tempsize = 0;  tempsize < FLASH_AACSKEY_SIZE; tempsize++)
        {
            *dest = *source;
            dest++;
            source++;
        }

        dest   = (BYTE *)PARA_BLK_START_OFFSET;
        source = (BYTE *)DRVPARA_DATA;
    }
#endif

    /* Block Erase Command */
    index = 0;
    tempsize = MkbSize;
    while (tempsize > 0)
    {
        FlashEraseSector((ULONG)dest + (ULONG)index*dwsize, dwsize);
        tempsize -= dwsize;
        index++;
    }
    /* Rom Write */
#if 1 // TBD? need to double check conditional to support SEEPROM writes!
    {
        SFlashWriteData((ULONG)dest,(ULONG)MkbSize,(ULONG)source,(USHORT)program_size);
    }
#else
    #if (SEEPROM==0)
    for( count=0 ; count < PARA_BLK_SIZE ; count++ ){
    #else
    for( count=0 ; count < FLASH_PARA_SIZE ; count++ ){
    #endif
        FlashWriteData((ULONG)dest,*(SCSDRAM + (long)source));
        dest++;
        source++;

        for(i = 0L;i < 0x000000FFL;i++){
            j = i * 2;
        }

        for(i = 0L;i < 0x000000FFL;i++){
            j = i * 2;
        }
    }
#endif
    ChangeChipStatus(0x1C);
}

///*************************************************************************
//* FlashEraseSector()
//*
//* Sector Erase followed by WAIT
//*
//* Returns:  void
//* Params:   address, size
//*************************************************************************/
void FlashEraseSector(ULONG address, ULONG size)
{
    BYTE Status;
#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('E','R','A','S');
    send_msgf1((BYTE)(address >> 24),(BYTE)(address >> 16), (BYTE)(address >> 8),(BYTE)(address));
    send_msgf1((BYTE)(size    >> 24),(BYTE)(size    >> 16), (BYTE)(size    >> 8),(BYTE)(size   ));
#endif

    /*--------------------------------------------------------------------*/
    /*        Sector Erase Command Sequence  :  SST25VF080B/040B          */
    /*--------------------------------------------------------------------*/
    // Variable address contains the start offset address into the Serial Flash chip
    //  where erase should start.
    // Variable size contains the number of Bytes on Serial Flash to erase.
    // Erase the number of required sectors in SFlash starting at "address"
    // E.g. chip SST25VF080 allows a 4-KByte Sector-erase

        do
        {
            WRITE_FIELD(HAL_AHB_SFLASH_ADDRREG, 0x000000);
            WRITE_REG(HAL_AHB_SFLASHREG_MAP->STATEREG, 0x00);//  Remove
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfWriteStatusReg);    // Write Status Register command
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteEnable); //     Block Write protection
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfReadStatusReg); // Read Status Register command     0x05
            Status = READ_REG(HAL_AHB_SFLASHREG_MAP->STATEREG);
        } while(((Status & 0x02) == FALSE) | (Status & 0x3C) != 0);      // Wait for "Device Write Enabled" and "Block Write Protection = off"

        WRITE_FIELD(HAL_AHB_SFLASH_ADDRREG, address);
        WRITE_REG(HAL_AHB_SFLASHREG_MAP->STATEREG, 0x00);

        //check the FlashROM support erase size
        if(size == 0x1000)
        {
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfSectorErase); // 0x20     4K Sector erase.
        }
        else if(size ==0x10000)
        {
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sf64KBlockErase); // 0xD8   64K Sector erase.
        }
        else
        {
            send_msgf1('E','R','F','L');
            return;
        }
        do
        {
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG, sfReadStatusReg);  // Read Status Register command
        }while(READ_FIELD(HAL_AHB_SFLASH_SFBUSY) == FALSE);      // Wait for BUSY
         /* Wait for Erase to complete */
        do
        {
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfReadStatusReg);  // Read Status Register command
        }while(READ_FIELD(HAL_AHB_SFLASH_SFBUSY));      // Wait for Not BUSY
        // Sector has completed erasure.

        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteDisable);  // Disable writing to SFlash
}

///*************************************************************************
//* SFlashWriteData()
//*
//* Write followed program_size data to serial flash rom
//*
//* Returns:  void
//* Params:   address:  dest address
//            length: request length
//            sourceaddr: source address
//            program_size: byte_program=1 or page_program = 256
//*************************************************************************/
void SFlashWriteData(ULONG address, ULONG length,ULONG sourceaddr,USHORT program_size)
{
    vBYTE *flash_write_addr;
    ULONG Len;
    USHORT i;
#if (SF_VERIFY_WRITEDATA ==1)
    BYTE ReadBack,readfail;
    ULONG vrfsrcaddr;
    vrfsrcaddr = sourceaddr;
#endif
#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('W','r','S','F');
    send_msgf1((BYTE)(address >> 24),(BYTE)(address >> 16), (BYTE)(address >> 8),(BYTE)(address));
    send_msgf1((BYTE)(length    >> 24),(BYTE)(length    >> 16), (BYTE)(length    >> 8),(BYTE)(length   ));
#endif

    flash_write_addr = (vBYTE *)FLASH_BASE + address;

    for(Len = 0; Len < length; Len +=program_size)
    {
        /* Write Enable */
        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteStatusReg);
        WRITE_FIELD(HAL_AHB_SFLASH_SFEXTW,0);   // Enable Auto Page program.
        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteEnable);

        for (i = 0; i < program_size; i++,flash_write_addr++)
            *flash_write_addr = SCSDRAM[sourceaddr+i];

        do
        {  /* Wait for Write to complete */
            WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfReadStatusReg);
        }while(READ_FIELD(HAL_AHB_SFLASH_SFBUSY));      // Wait for Not BUSY

        WRITE_FIELD(HAL_AHB_SFLASH_CMDREG,sfWriteDisable); // Disable writing to SFlash
        WRITE_FIELD(HAL_AHB_SFLASH_SFEXTW,1);       // Disable Auto Page program.
        sourceaddr+=program_size;
    }

#if (SF_VERIFY_WRITEDATA ==1)
    readfail =0;
    flash_write_addr = (vBYTE *)FLASH_BASE + address;
    for(Len = 0; Len < length; Len +=program_size)
    {
        for (i = 0; i < program_size; i++,flash_write_addr++)
        {
             ReadBack = *flash_write_addr;
             if(ReadBack != SCSDRAM[vrfsrcaddr+i])
             {
                readfail=1;
                    send_msgf1('R','F',ReadBack,SCSDRAM[vrfsrcaddr+i]);
                break;
             }
        }
        if(readfail)
            break;
        vrfsrcaddr+=program_size;
    }

#endif
}


///*************************************************************************
//* BackupParaToSDRAM()
//*
//* Copies current program to DRAM for backup
//*
//* Returns:  void
//* Params:   none
//*************************************************************************/
static void BackupParaToSDRAM( void )
{
    BYTE *dest   = (BYTE *)FLASH_DRAM_OFFSET;
    BYTE *source = (BYTE *)FLASH_WORK;
    int count;

    for( count = 0 ; count < FLASH_WORK_END ; count++ ){
        *(SCSDRAM + (long)dest) = *source;
        dest++;
        source++;
    }
}

///*************************************************************************
//* RestoreParaFromSDRAM()
//*
//* Copies saved program from DRAM to WORK AREA
//*
//* Returns:  void
//* Params:   none
//*************************************************************************/
static void RestoreParaFromSDRAM( void )
{
    BYTE *dest   = (BYTE *)FLASH_WORK;
    BYTE *source = (BYTE *)FLASH_DRAM_OFFSET;
    int count;

    for( count = 0 ; count < FLASH_WORK_END ; count++ ){
        *dest = *(SCSDRAM + (ULONG)source);
        dest++;
        source++;
    }
}

#pragma arm section code = "FlashFW"
///*************************************************************************
//* FlashWriteFW()
//*
//* Copies saved program from SDRAM to WORK AREA
//*
//* Returns:  void
//* Params:   none
//*************************************************************************/
void FlashWriteFW( void )
{

    BYTE *dest  = (BYTE *)FLASH_PROG;
    BYTE *source = (BYTE *)FLASH_BLK_START;
    long count;

    DISABLE_INTERRUPT();

    /* Copy Flash Firmware routine to SRAM */
    for( count = 0 ; count < FLASH_WORK_END ; count++ ){
        *dest++ = *source++;
    }

#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf1('F','L','S','T');
#endif

    WRITE_FIELD(HAL_AHB_SFLASH_SFENCACH,0); /* Disable Cache during Flash updates */
    WRITE_FIELD(HAL_AHB_SFLASH_SFTAGRST,1); /* Reset Cache tags during Flash updates */

    SramAddressFunc(FlashFirmware);         /* Jump to flash write sequence in SRAM */
#if (SEND_FLASH_DEBUG_MSG == 1)
    send_msgf(0xCC);
#endif
    /*** Flashing is complete! Go to Drive Restart ***/
    Jump_To_Reset_Handler();
}
#pragma arm section code

#pragma arm section code = "FlashPara"
///*************************************************************************
//* FlashWriteParaTbl()
//*
//* Copies saved program from DRAM to WRITE PROGRAM AREA
//*
//* Returns:  void
//* Params:   none
//*************************************************************************/
char FlashWriteParaTbl( BYTE eTable )
{
    BYTE *dest   = (BYTE *)FLASH_PROG;
    BYTE *source = (BYTE *)FLASH_BLK_START;
    int count;
    BYTE error_flag;

#if (SUN_FUNC_TEST == 1)
    if((fgEEPROMChg==FALSE)&&(eTable == eEEPROM_Formal_Table))
    {
            SendMsgCnB(SEND_ATAPI_CMD,1,0x800F0C,B1B(fgEEPROMChg)); //EEPROM data NO change
            *FLASH_ERR_FLAG = 0;
            ENABLE_INTERRUPT();
            return 0;
    }
#endif

    DISABLE_INTERRUPT();

#if(SUN_FUNC_TEST == 1)
    if(eTable == eEEPROM_Formal_Table || eTable == eEEPROM_Backup_Table)
    {
        WriteEEPROMCheckSUMIntoSDRAM();
        if (CheckDramSourceValidity() != PASS)
        {
            SendMsgCnB(SEND_ATAPI_CMD,1,0x800F0B,B1B(eTable)); //write EEPROM fails
            *FLASH_ERR_FLAG = 1;
            ENABLE_INTERRUPT();
            return 1;
        }
    }
#endif

//#if ( PCB == 710) ||(PCB ==711)||(PCB == 817)
//    if(eTable == ePersistent_Talbe )
//        WriteEEPROMCheckSUMIntoSDRAM();
//#endif

    BackupParaToSDRAM();                       /* RAM back up */

    /* Copy Flash write sequence to SRAM */
    for( count = 0 ; count < FLASH_WORK_END; count++ ){
        *dest++ = *source++;
    }

    *FLASH_ERR_FLAG = 0;                    /* Clear Error Flag */

    gFlashTableNum = eTable;

    SramAddressFunc(FlashParaTable);        /* Jump to flash write sequence in SRAM */

    error_flag = *FLASH_ERR_FLAG;

    RestoreParaFromSDRAM();                      /* RAM restore */

    ENABLE_INTERRUPT();

#if (SUN_FUNC_TEST == 1)
    fgEEPROMChg=FALSE;//20090320_p.25==start==Add a flag for EEPROM is changed.
#endif

    return error_flag;
}
#pragma arm section code

#if (SUN_FUNC_TEST == 1)
///*********************************************************************
//* Function:    ChangeLEDStatus
//* Description: Change the current LED status: 0->1, 1->0
//* Input:       None
//* Exceptions:  None
//* Output:      None
//*********************************************************************/

void ChangeLEDStatus(void)
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

///*********************************************************************
//* Function:   CalculateEEPROMCheckSUM
//* Description:
//* Input:
//* Exceptions:
//* Output:
//*********************************************************************/
USHORT CalculateEEPROMCheckSUM(const BYTE* addr)
{
    USHORT check_sum, i;

    check_sum = 0;
    for(i = 0; i < 0x1000 - 4; i++) //Size of eeprom table.
    {
        check_sum += *addr;
        addr++;
    }

    return check_sum;
}

///*********************************************************************
//* Function:   WriteEEPROMCheckSUM
//* Description:
//* Input:
//* Exceptions:
//* Output:
//*********************************************************************/
void WriteEEPROMCheckSUMIntoSDRAM(void)
{
    USHORT check_sum;

    check_sum = CalculateEEPROMCheckSUM((const BYTE*)(SCSDRAM + DRVPARA_DATA));

    SCSDRAM[DRVPARA_DATA+PARA_BLK_SIZE - 4] = check_sum;
    SCSDRAM[DRVPARA_DATA+PARA_BLK_SIZE - 3] = check_sum >> 8;
}

///*********************************************************************
//* Function:    CheckTagProtection
//* Description:
//* Input:
//* Exceptions: None
//* Output:      None
//*********************************************************************/
BYTE CheckEEPROMTagProtection(BYTE eTable)
{
    BYTE result, tag[4];
    BYTE* source;

    if(eTable == eEEPROM_Formal_Table)
        source = AGAIN_SL_START;
    else if(eTable == eEEPROM_Backup_Table)
        source = AGAIN_SL_BKUP_START;
    else
        return PASS;

    tag[0] = *source;
    tag[1] = *(source + 1);
    tag[2] = *(source + AGAIN_SL_SIZE - 2);
    tag[3] = *(source + AGAIN_SL_SIZE - 1);

    result = FAIL;
    if(tag[0] == 'L' && tag[1] == 'T')
    {
        if(tag[2] == 'L' && tag[3] == 'T')
            result = PASS;
    }

    if(result == FAIL)
        SendMsgCnB(SEND_ATAPI_CMD,1,0x800F03,B1B(eTable)); //Check Tag Error

    return result;
}

///*********************************************************************
//* Function:    WriteEEPROMTagProtectionIntoSDRAM
//* Description:
//* Input:
//* Exceptions: None
//* Output:      None
//*********************************************************************/
void WriteEEPROMTagProtectionIntoSDRAM(void)
{
    SCSDRAM[DRVPARA_DATA] = 'L';
    SCSDRAM[DRVPARA_DATA+1] = 'T';
    SCSDRAM[DRVPARA_DATA+PARA_BLK_SIZE - 2] = 'L';
    SCSDRAM[DRVPARA_DATA+PARA_BLK_SIZE - 1] = 'T';

    send_msg5S(1,"WEEProtect");
    send_msg80(1,0x710300);
}

/*********************************************************************
* Function:    CheckEEPROMSumProtection
* Description:
* Input:
* Exceptions: None
* Output:      None
*********************************************************************/
BYTE CheckEEPROMSumProtection(BYTE eTable)
{
    USHORT check_sum, check_sum2;
    BYTE* source;

    if(eTable == eEEPROM_Formal_Table)
        source = AGAIN_SL_START;
    else if(eTable == eEEPROM_Backup_Table)
        source = AGAIN_SL_BKUP_START;
    else
        return PASS;

    check_sum = CalculateEEPROMCheckSUM((const BYTE*)source);
    check_sum2 = (*(BYTE*)(source + AGAIN_SL_SIZE - 3)) << 8;
    check_sum2 |= (*(BYTE*)(source + AGAIN_SL_SIZE - 4));
    //send_msg5SValue(1,"#cs",4, (check_sum << 16)+check_sum2);
    if(check_sum == check_sum2)
    {
        //send_msg1('#','C','o','k');
        SendMsgCnB(SEND_ATAPI_CMD,4,0x800F04,B2B(check_sum),B2B(check_sum2));
        return PASS;
    }
    else
    {
        SendMsgCnB(SEND_ATAPI_CMD,4,0x800F05,B2B(check_sum),B2B(check_sum2));
        //send_msg1('#','N','o','k');
        return FAIL;
    }
}

/************************************************************************
    Name:       CheckDramSourceValidity()

    Description:
    Returns:
    note:
*************************************************************************/
#define FT_CHECK_SUM_INITIAL_PATTERN 0x01
#define FT_CHECK_SUM_INITIAL_PATTERN_OFFSET 0xFF0

BYTE CheckDramSourceValidity(void)
{
    USHORT check_sum, check_sum2;
    BYTE *source;

    if ((SCSDRAM[DRVPARA_DATA] != 'L') || (SCSDRAM[DRVPARA_DATA + 1] != 'T')
        || (SCSDRAM[DRVPARA_DATA+AGAIN_SL_SIZE - 2] != 'L')
        || (SCSDRAM[DRVPARA_DATA+AGAIN_SL_SIZE - 1] != 'T')
        || (SCSDRAM[DRVPARA_DATA + FT_CHECK_SUM_INITIAL_PATTERN_OFFSET] != FT_CHECK_SUM_INITIAL_PATTERN))
    {
        send_msg80(1,0x800F00); //CheckDramSourceValidity no good
        return FAIL;
    }
    source = (BYTE *)(SCSDRAM_ADDRESS + DRVPARA_DATA);
    check_sum = CalculateEEPROMCheckSUM((const BYTE*)source);
    check_sum2 = (*(BYTE*)(source + AGAIN_SL_SIZE - 3)) << 8;
    check_sum2 |= (*(BYTE*)(source + AGAIN_SL_SIZE - 4));
    if(check_sum == check_sum2)
    {
        SendMsgCnB(SEND_ATAPI_CMD,4,0x800F01,B2B(check_sum),B2B(check_sum2));
        return PASS;
    }
    else
    {
        SendMsgCnB(SEND_ATAPI_CMD,4,0x800F02,B2B(check_sum),B2B(check_sum2));
        return FAIL;
    }
}

#endif
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
