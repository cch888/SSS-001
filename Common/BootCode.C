/*****************************************************************************
*
* FILENAME:   Boot.c
*
* DESCRIPTION
*   This file contains definitions for boot code
*
*
*
*
*****************************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\SDRAM.h"
#include ".\common\scomm.h"
#include ".\HIF\GP_RAM.h"
#include ".\HIF\flash.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_SDRAM.h"
#include ".\AL\REG_host.h"
#include ".\AL\REG_sflash.h"
#include ".\AL\REG_SSI.h"
#include ".\AL\REG_afe.h"
#include ".\AL\REG_cd_decoder.h"
#include ".\AL\REG_cpycmp.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"
#include ".\HIF\AT_equ.h"
#include ".\firmw_id.h"
#if (BOOT_CODE_SUPPORT == 1)
/*
#define BT_FIRMW_ID_SN ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', \
                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', \
                    ' ', ' ', ' ', ' '

#define BT_FIRMW_ID    'B', 'T', '0', '1'
BYTE const BT_PLDS_Ver_Table[16] =
{
    BT_FIRMW_ID,    // Boot Code Revision
    '0','0','0','0',    // WS Table Version Number
    ' ',' ',' ',' ',    // OEM Version Number
    ' ',' ',' ',' '     // PLDS Internal FW Revision//mctest
};
*/
/*---------------------------------------------------------------------------
        DEFINES
----------------------------------------------------------------------------*/
#define BT_XFER_LIMIT_SIZE          0x2000
#define BT_XFER_PAGE_SIZE           0x1000
/*---------------------------------------------------------------------------
        VARIABLES
----------------------------------------------------------------------------*/
vULONG BT_Iram_system_timer = 0, BT_BGFis34Timer = 0;
vULONG Bt_Buf_dram_addr, Bt_Linear_Xfer_addr,Bt_FlashLinearAdress;
vUSHORT Bt_shATB, Bt_Buf_hxfer_blk,Bt_Xfer_Residue,Bt_shHstWdC,Bt_shHstBytAC;
USHORT Bt_Program_Size;
#if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))
vULONG BT_BGFis34Timer = 0;
vBYTE   BT_SendFis34Already;
#endif
vBYTE Bt_shHDDir;
volatile SENSE_T Bt_Sense;            /* Request Sense return data */
BYTE BT_Cmd_block[12];
BYTE BT_cmd_avail;
BYTE BT_manuf_code, BT_device_code,BT_capacity_code;
BYTE BT_tranData[15];

#define MINIMUM(PARAM1, PARAM2) (((PARAM1) < (PARAM2)) ? (PARAM1) : (PARAM2)) //  Compares two parameters, return minimum.
//BYTE bc_no_update = TRUE;
/*---------------------------------------------------------------------------
    Debug Message function for boot code
----------------------------------------------------------------------------*/
#define SEND_BOOT_DEBUG_MSG 1
#if (SEND_BOOT_DEBUG_MSG == 1)
#define SSI_ADDRESS  0xCF000000
static void delay_msgB(void)
{
   BYTE i;

   for (i = 0; i < 100; i++);
}


static void send_msgB1(BYTE d1,BYTE d2,BYTE d3,BYTE d4)
{
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_1 ) = 0x00;  /*  SYNC_PATT      */
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = 0xAA;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = 0x54;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = d1;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = d2;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = d3;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = d4;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_1 ) = 0x55;
    delay_msgB();
}
static void send_msgB2(void)
{
    BYTE k;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_1 ) = 0x00;  /*  SYNC_PATT      */
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = 0xAA;
    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = 0x6C;

    for(k = 0; k < 12; k++)
    {
      *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_2 ) = BT_Cmd_block[k];        /* ATAPI Command */
    }


    *(vBYTE *)(SSI_ADDRESS + ADAPTER_ADDR_1 ) = 0x55;


}

#else
#define send_msgB1(d1,d2,d3,d4) {}
#define send_msgB2() {}
#endif

/*---------------------------------------------------------------------------
    Boot code Table
----------------------------------------------------------------------------*/
/*-----------------------------------------*/
/* ATAPI Identify Drive return information */
/*-----------------------------------------*/
#define BT_WORD50_HI     0x40
#define BT_WORD51_HI     0x00   /* PIO Data Transfer Cycle Timing */
#define BT_WORD52_HI     0x02   /* DMA Data Transfer Cycle Timing */
#define BT_WORD53_LO     0x06   /* ENTER Field Validity including UDMA */
#define BT_WORD62_HI     0x00   /* Single Word DMA Transfer */
#define BT_WORD62_LO     0x07
#define BT_WORD63_HI     0x00   /* Multi Word DMA Transfer */
#define BT_WORD63_LO     0x07
#define BT_WORD64_LO     0x03   /* Enhanced PIO Mode */
#define BT_WORD65_HI     0x00   /* Minimum Multi_word DMA Transfer Cycle Time per Word */
#define BT_WORD65_LO     0x78
#define BT_WORD66_HI     0x00   /* Manufacturer's Recommended Multi_word DMA Transfer Cycle Time */
#define BT_WORD66_LO     0x78
#define BT_WORD67_HI     0x00   /* Minimum PIO Transfer Cycle Time Without Flow Control */
#define BT_WORD67_LO     0x78
#define BT_WORD68_HI     0x00   /* Minimum PIO Transfer Cycle Time with IORDY Flow Control */
#define BT_WORD68_LO     0x78

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
#define BT_WORD76_HI     0x00
#define BT_WORD76_LO     0x02   /* Support SATA Gen1 signaling speed*/
#define BT_WORD78_HI     0x00   /* Reserved */
#define BT_WORD78_LO     0x48   /* Support SSP/Device support IPM*/
#define BT_WORD79_HI     0x00   /* Reserved*/
#define BT_WORD79_LO     0x48   /* SSP enable/DIPM enable*/
#endif

#define BT_WORD80_HI     0x01   /* Major version number */
#define BT_WORD80_LO     0xF0   /* Supproted ATA/ATAPI-7 */
#define BT_WORD81_HI     0x00   /* Major version number */
#define BT_WORD81_LO     0x1A   /* Supproted ATA/ATAPI-7 1532D Rev1 */
#define BT_WORD82_HI     0x42   /* Command set supproted */
#define BT_WORD82_LO     0x10
#define BT_WORD83_HI     0x40   /* Command set supproted */
#define BT_WORD83_LO     0x10
#define BT_WORD84_HI     0x40   /* Command set/feature supproted extension */
#define BT_WORD84_LO     0x00
#define BT_WORD85_HI     0x42   /* Command set/feature enabled */
#define BT_WORD85_LO     0x10
#define BT_WORD86_HI     0x00   /* Command set/feature enabled */
#define BT_WORD86_LO     0x00
#define BT_WORD87_HI     0x40   /* Command set/feature default */
#define BT_WORD87_LO     0x00
#define BT_WORD88_HI     0x20   /* ENTER Ultra DMA active mode */
#define BT_WORD88_LO     0x3F   /* ENTER Modes 5 and below are supported */
#define BT_WORD94_HI     0x80   /* Current AAM value */
#define BT_WORD94_LO     0x00   /* (Vender specific) */
#define BT_WORD255_LO    0xA5

#define BT_FIRMW_ID_SN ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', \
                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', \
                    ' ', ' ', ' ', ' '
#if BIOS_MODEL == 0
#define BT_FIRMW_ID    'B', 'T', '0', '1'
#elif BIOS_MODEL == 2
#define BT_FIRMW_ID    '0', 'V', 'T', '1'
#endif

#define BT_FIRMW_ID_REVISION   BT_FIRMW_ID, ' ', ' ', ' ', ' '

#define BT_FIRMW_ID_MODEL      FIRMW_ID_MODEL


#define BT_FIRMW_INQ_VENDOR    FIRMW_INQ_VENDOR//'S','U','N','E','X','T',' ',' '


#define BT_FIRMW_INQ_PRODUCT   FIRMW_INQ_PRODUCT//'B','D',' ','R','E','A','D','E','R',' ','S','U','N','7','1','0'


#define BT_FIRMW_INQ_REVISION      BT_FIRMW_ID

#define StartOfFW	'S','T','A','R','T','O','F','F','I','R','M','W','A','R','E',' '



BYTE const SigStringSOF[16] =
{
    	StartOfFW
};

BYTE const SigStringEOF[16] =
{
    	BT_FIRMW_INQ_PRODUCT
};

BYTE const Bt_Identify_tbl[] = {
    0xC0,0x85,                                          /* word 00:     000 ~ 001 General configuration bit */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 01-04:  002 ~ 009 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 05-08:  010 ~ 017 */
    0x00,0x00,                                          /* word 09:     018 ~ 019 */
    BT_FIRMW_ID_SN,                                        /* word 10-19:  020 ~ 039 */
    0x00,0x00,                                          /* word 20:     040 ~ 041 */
    0x00,0x00,                                          /* word 21:     042 ~ 043 */
    0x00,0x00,                                          /* word 22:     044 ~ 045 */
    BT_FIRMW_ID_REVISION,                                  /* word 23-26:  046 ~ 053 Firmware revision */
    BT_FIRMW_ID_MODEL,                                     /* word 27-46:  054 ~ 093 Model number */
    0x00,0x00,                                          /* word 47:     094 ~ 095 */
    0x00,0x00,                                          /* word 48:     096 ~ 097 */
    0x00,0x0F,                                          /* word 49:     098 ~ 099 Capabilities */
    0x00,BT_WORD50_HI,                                  /* word 50:     100 ~ 101 Capabilities */
    0x00,BT_WORD51_HI,                                  /* word 51:     102 ~ 103 PIO cycle timing */
    0x00,BT_WORD52_HI,                                  /* word 52:     104 ~ 105 DMA cycle timing */
    BT_WORD53_LO,0x00,                                  /* word 53:     106 ~ 107 Field validity */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 54-57:  108 ~ 115 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 58-61:  116 ~ 123 */
    BT_WORD62_LO,BT_WORD62_HI,                          /* word 62:     124 ~ 125 Singleword DMA transfer mode */
    BT_WORD63_LO,BT_WORD63_HI,                          /* word 63:     126 ~ 127 Multiword DMA transfer mode */
    BT_WORD64_LO,0x000,                                 /* word 64:     128 ~ 129 Advanced PIO transfer mode */
    BT_WORD65_LO,BT_WORD65_HI,                          /* word 65:     130 ~ 131 Min. Multiword DMA cycle timing */
    BT_WORD66_LO,BT_WORD66_HI,                          /* word 66:     132 ~ 133 Manufacture recommended Multiword DMA cycle timing */
    BT_WORD67_LO,BT_WORD67_HI,                          /* word 67:     134 ~ 135 Min. PIO cycle timing w/o flow control */
    BT_WORD68_LO,BT_WORD68_HI,                          /* word 68:     136 ~ 137 Min. PIO cycle timing w/ IORDY flow control */
    0x00,0x00,                                          /* word 69:     138 ~ 139 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 70-73:  140 ~ 147 */

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
    0x00,0x00,0x00,0x00,BT_WORD76_LO,BT_WORD76_HI,0x00,0x00,            /* word 74-77:  148 ~ 155 */
    BT_WORD78_LO,BT_WORD78_HI,BT_WORD79_LO,BT_WORD79_HI,                                /* word 78-79:  156 ~ 159 SATA support*/
#else
    0x00,0x00,0x00,0x00,0x02,0x02,0x00,0x00,            /* word 74-77:  148 ~ 155 */
    0x48,0x00,0x40,0x00,                                /* word 78-79:  156 ~ 159 SATA support*/
#endif

    BT_WORD80_LO,BT_WORD80_HI,                          /* word 80:     160 ~ 161 Minor version number */
    BT_WORD81_LO,BT_WORD81_HI,                          /* word 81:     162 ~ 163 Minor version number */
    BT_WORD82_LO,BT_WORD82_HI,BT_WORD83_LO,BT_WORD83_HI,/* word 82-83:  164 ~ 167 Command set supported */
    BT_WORD84_LO,BT_WORD84_HI,                          /* word 84:     168 ~ 169 Command set/feature supported extension */
    BT_WORD85_LO,BT_WORD85_HI,BT_WORD86_LO,BT_WORD86_HI,/* word 85-86:  170 ~ 173 Command set/feature enabled */
    BT_WORD87_LO,BT_WORD87_HI,                          /* word 87:     174 ~ 175 Command set/feature default */
    BT_WORD88_LO,BT_WORD88_HI,                          /* word 88:     176 ~ 177 Ultra DMA transfer mode */
    0x00,0x00,                                          /* word 89:     178 ~ 179 */
    0x00,0x00,0x00,0x00,0x00,0x00,                      /* word 90-92:  180 ~ 185 */
    0x00,0x00,                                          /* word 93:     186 ~ 187 Hardware reset result */
    BT_WORD94_LO,BT_WORD94_HI,                          /* word 94:     188 ~ 189 Current AAM value */
    0x00,0x00,0x00,0x00,0x00,0x00,                      /* word 96-97:  190 ~ 195 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 98-101: 196 ~ 203 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 102-105:204 ~ 211 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 106-109:212 ~ 219 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 110-113:220 ~ 227 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 114-117:228 ~ 235 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 118-121:236 ~ 243 */
    0x00,0x00,0x00,0x00,0x00,0x00,                      /* word 122-124:244 ~ 249 */
    0x00,0x00,                                          /* word 125:    250 ~ 251 ATAPI BYTE count = 0*/
    0x00,0x00,                                          /* word 126:    252 ~ 253 */
    0x00,0x00,                                          /* word 127:    254 ~ 255 Removable Media Status Notification feature set supprt */
    0x00,0x00,                                          /* word 128:    256 ~ 257 Security status */
    0x00,0x00,                                          /* word 129:    258 ~ 259 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 130-133:260 ~ 267 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 134-137:268 ~ 275 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 138-141:276 ~ 283 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 142-145:284 ~ 291 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 146-149:292 ~ 299 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 150-153:300 ~ 307 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 154-157:308 ~ 315 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 158-161:316 ~ 323 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 162-165:324 ~ 331 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 166-169:332 ~ 339 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 170-173:340 ~ 347 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 174-177:348 ~ 355 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 178-181:356 ~ 363 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 182-185:364 ~ 371 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 186-189:372 ~ 379 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 190-193:380 ~ 387 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 194-197:388 ~ 395 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 198-201:396 ~ 403 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 202-205:404 ~ 411 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 206-209:412 ~ 419 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 210-213:420 ~ 427 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 214-217:428 ~ 435 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 218-221:436 ~ 443 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 222-225:444 ~ 451 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 226-229:452 ~ 459 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 230-233:460 ~ 467 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 234-237:468 ~ 475 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 238-241:476 ~ 483 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 242-245:484 ~ 491 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 246-249:492 ~ 499 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 250-253:500 ~ 507 */
    0x00,0x00,                                          /* word 254:    508 ~ 509 */
    BT_WORD255_LO,0x00                                  /* word 255:    510 ~ 511 Integrity word */
};

BYTE const Bt_Inquiry_tbl[]={
    0x05,0x80,0x00,0x33,0x33,0x00,0x00,0x00,            /* 000 ~ 007 */  /* change byte 3 & 4, MMC-5 spec */

    BT_FIRMW_INQ_VENDOR,                                   /* 008 ~ 015 */
    BT_FIRMW_INQ_PRODUCT,                                  /* 016 ~ 031 */
    BT_FIRMW_INQ_REVISION,                                 /* 032 ~ 035 */
    BT_FIRMW_ID_SN                                         /* 036 ~ 055 */
};

BYTE const Bt_Request_sense_tbl[]={
    0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,            /* 000 ~ 007*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* 008 ~ 015*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00             /* 016 ~ 023*/
};

BYTE const  Bt_GESN_nea_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x02,       /* 1: data length low */
            0x80,       /* 2: NEA */
            0x5E,       /* 3: supported class */
        };


BYTE const  Bt_GESN_external_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x03,       /* 2: notificatin class = power */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: external request event = no change */
            0x00,       /* 5: external request status = Available */
            0x00,       /* 6: external request = No request */
            0x00        /* 7: external request = No request */
        };

BYTE const  Bt_GESN_media_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x04,       /* 2: notificatin class = media */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: media event = no event */
            0x00,       /* 5: media status */
            0x00,       /* 6: reserved */
            0x00        /* 7: reserved */
        };

BYTE const  Bt_GESN_DeviceBusy_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x06,       /* 2: notificatin class = device busy */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: device busy event = no event */
            0x00,       /* 5: device busy status */
            0x00,       /* 6: reserved */
            0x00        /* 7: reserved */
        };

BYTE const  Bt_GESN_OpeChgReq_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x01,       /* 2: notificatin class = operational change request/notification */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: operational event = no event */
            0x00,       /* 5: operational status */
            0x00,       /* 6: operational request/report */
            0x00        /* 7: operational request/report */
        };


#if (ENABLE_AACS )
/*******************************************************************
    DEFINES
*******************************************************************/
#define DEC_LONG(l,a,b,c,d) { \
                                a = (l >> 24); \
                                b = (l >> 16) & 0xff; \
                                c = (l >> 8) & 0xff; \
                                d = l & 0xff; \
                            }

#define MAKE_LONG(a,b,c,d) ((ULONG)a << 24 | (ULONG)b << 16 | (ULONG)c << 8 | (ULONG)d)
// ***************************************************************************
//                               VARIABLES
// ***************************************************************************
BYTE const BT_SubBytes_tbl[16][16]={
/* 0,  1,   2,   3,   4,   5,   6,   7,   8,   9,   a,   b,   c,   d,   e,   f  */
{0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76},  //0
{0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0},  //1
{0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15},  //2
{0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75},  //3
{0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84},  //4
{0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf},  //5
{0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8},  //6
{0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2},  //7
{0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73},  //8
{0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb},  //9
{0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79},  //a
{0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08},  //b
{0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a},  //c
{0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e},  //d
{0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf},  //e
{0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16}   //f
};

BYTE const BT_InvSubBytes_tbl[16][16]={
/* 0,  1,   2,   3,   4,   5,   6,   7,   8,   9,   a,   b,   c,   d,   e,   f  */
{0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},  //0
{0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},  //1
{0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},  //2
{0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},  //3
{0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},  //4
{0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},  //5
{0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},  //6
{0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},  //7
{0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},  //8
{0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},  //9
{0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},  //a
{0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},  //b
{0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},  //c
{0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},  //d
{0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},  //e
{0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}   //f
};

ULONG const BT_Rcon[11]={
    0x00000000, //0
    0x01000000, //1
    0x02000000, //2
    0x04000000, //3
    0x08000000, //4
    0x10000000, //5
    0x20000000, //6
    0x40000000, //7
    0x80000000, //8
    0x1b000000, //9
    0x36000000, //a
};

BYTE const BT_Factor02[8]={0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b};
BYTE const BT_Factor03[8]={0x03,0x06,0x0c,0x18,0x30,0x60,0xc0,0x9b};
BYTE const BT_Factor09[8]={0x09,0x12,0x24,0x48,0x90,0x3b,0x76,0xec};
BYTE const BT_Factor0b[8]={0x0b,0x16,0x2c,0x58,0xb0,0x7b,0xf6,0xf7};
BYTE const BT_Factor0d[8]={0x0d,0x1a,0x34,0x68,0xd0,0xbb,0x6d,0xda};
BYTE const BT_Factor0e[8]={0x0e,0x1c,0x38,0x70,0xe0,0xdb,0xad,0x41};

BYTE BT_State_A[4][4];
ULONG BT_Key_Ex[44];
/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void BT_AESCbcDecrypt(BYTE *pKey, BYTE *Iv, BYTE *In, BYTE *Out, ULONG Size);
extern void BT_AESCbcEncrypt(BYTE *pKey, BYTE *Iv, BYTE *In, BYTE *Out, ULONG Size);
extern void BT_AESEncrypt(BYTE *key, BYTE *in, BYTE *output, BYTE bypass_flag);
extern void BT_AESDecrypt(BYTE *key, BYTE *in, BYTE *output, BYTE bypass_flag);
extern void BT_KeyExpansion(BYTE *key);
extern ULONG BT_Sub_Rot_Word(ULONG);
extern void BT_AddRoundKey(BYTE num);
extern void BT_SubBytes(void);
extern void BT_InvSubBytes(void);
extern void BT_ShiftRows(void);
extern void BT_InvShiftRows(void);
extern void BT_MixColumns(void);
extern void BT_InvMixColumns(void);
extern BYTE BT_Multi_A(BYTE factor, BYTE varible);
// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************
/*---------------------------------------------------------------------------
    Boot code BT_IdentifyChip
----------------------------------------------------------------------------*/

/*********************************************************************
* Name:         AESEncrypt
*
* Description:  AES encryption algorithm.
*
* Params:       Input: Cipher_Key[16], Plain_text[16], bypass_key_expansion_flag
*
* Returns:      none
*
* outputs:      output[16]
*
* Notes:
*
*********************************************************************/
void BT_AESEncrypt(BYTE *key, BYTE *in, BYTE *output, BYTE bypass_flag)
{
    BYTE i,j;

    if(bypass_flag == 0)
        BT_KeyExpansion(key);

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            BT_State_A[i][j] = in[i+j*4];
    }

    BT_AddRoundKey(0);

    for(i=1; i<10; i++)
    {
        BT_SubBytes();
        BT_ShiftRows();
        BT_MixColumns();
        BT_AddRoundKey(i*4);
/*
send_msg1('T','S','T',i);
send_msg1(State_A[0][0], State_A[0][1], State_A[0][2], State_A[0][3]);
send_msg1(State_A[1][0], State_A[1][1], State_A[1][2], State_A[1][3]);
send_msg1(State_A[2][0], State_A[2][1], State_A[2][2], State_A[2][3]);
send_msg1(State_A[3][0], State_A[3][1], State_A[3][2], State_A[3][3]);
*/
    }

    BT_SubBytes();
    BT_ShiftRows();
    BT_AddRoundKey(40);

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            output[i+j*4] = BT_State_A[i][j];
    }
}

/*********************************************************************
* Name:         AESDecrypt
*
* Description:  AES decryption algorithm.
*
* Params:       Input: Cipher_Key[16], Cipher_text[16], bypass_key_expansion_flag
*
* Returns:      none
*
* outputs:      output[16]
*
* Notes:
*
*********************************************************************/
void BT_AESDecrypt(BYTE *key, BYTE *in, BYTE *output, BYTE bypass_flag)
{
    BYTE i,j;

    if(bypass_flag == 0)
        BT_KeyExpansion(key);

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            BT_State_A[i][j] = in[i+j*4];
    }

    BT_AddRoundKey(40);

    for(i=9; i>0; i--)
    {
        BT_InvShiftRows();
        BT_InvSubBytes();
        BT_AddRoundKey(i*4);
        BT_InvMixColumns();
/*
send_msg1('D','E','C',i);
send_msg1(State_A[0][0], State_A[0][1], State_A[0][2], State_A[0][3]);
send_msg1(State_A[1][0], State_A[1][1], State_A[1][2], State_A[1][3]);
send_msg1(State_A[2][0], State_A[2][1], State_A[2][2], State_A[2][3]);
send_msg1(State_A[3][0], State_A[3][1], State_A[3][2], State_A[3][3]);
*/
    }

    BT_InvShiftRows();
    BT_InvSubBytes();
    BT_AddRoundKey(0);

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            output[i+j*4] = BT_State_A[i][j];
    }
}

/*******************************************************************************

    FUNCTION        AddRoundKey

    DESCRIPTION

********************************************************************************/
void BT_AddRoundKey(BYTE num)
{
    ULONG temp_l;
    BYTE i;

    for(i=0; i<4; i++)
    {
        temp_l = MAKE_LONG(BT_State_A[0][i], BT_State_A[1][i], BT_State_A[2][i], BT_State_A[3][i]);
        temp_l = temp_l ^ BT_Key_Ex[num+i];
        DEC_LONG(temp_l,BT_State_A[0][i], BT_State_A[1][i], BT_State_A[2][i], BT_State_A[3][i]);
    }
}

/*******************************************************************************

    FUNCTION        SubBytes

    DESCRIPTION

********************************************************************************/
void BT_SubBytes(void)
{
    BYTE i,j;

    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
            BT_State_A[i][j] = BT_SubBytes_tbl[BT_State_A[i][j]>>4][BT_State_A[i][j]&0x0f];
    }
}/*******************************************************************************

    FUNCTION        InvSubBytes

    DESCRIPTION

********************************************************************************/
void BT_InvSubBytes(void)
{
    BYTE i,j;

    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
            BT_State_A[i][j] = BT_InvSubBytes_tbl[BT_State_A[i][j]>>4][BT_State_A[i][j]&0x0f];
    }
}

/*******************************************************************************

    FUNCTION        ShiftRows

    DESCRIPTION

********************************************************************************/
void BT_ShiftRows(void)
{
    BYTE temp_b;

    temp_b = BT_State_A[1][0];
    BT_State_A[1][0] = BT_State_A[1][1];
    BT_State_A[1][1] = BT_State_A[1][2];
    BT_State_A[1][2] = BT_State_A[1][3];
    BT_State_A[1][3] = temp_b;

    temp_b = BT_State_A[2][0];
    BT_State_A[2][0] = BT_State_A[2][2];
    BT_State_A[2][2] = temp_b;
    temp_b = BT_State_A[2][1];
    BT_State_A[2][1] = BT_State_A[2][3];
    BT_State_A[2][3] = temp_b;

    temp_b = BT_State_A[3][3];
    BT_State_A[3][3] = BT_State_A[3][2];
    BT_State_A[3][2] = BT_State_A[3][1];
    BT_State_A[3][1] = BT_State_A[3][0];
    BT_State_A[3][0] = temp_b;
}

/*******************************************************************************

    FUNCTION        InvShiftRows

    DESCRIPTION

********************************************************************************/
void BT_InvShiftRows(void)
{
    BYTE temp_b;

    temp_b = BT_State_A[1][3];
    BT_State_A[1][3] = BT_State_A[1][2];
    BT_State_A[1][2] = BT_State_A[1][1];
    BT_State_A[1][1] = BT_State_A[1][0];
    BT_State_A[1][0] = temp_b;

    temp_b = BT_State_A[2][0];
    BT_State_A[2][0] = BT_State_A[2][2];
    BT_State_A[2][2] = temp_b;
    temp_b = BT_State_A[2][1];
    BT_State_A[2][1] = BT_State_A[2][3];
    BT_State_A[2][3] = temp_b;

    temp_b = BT_State_A[3][0];
    BT_State_A[3][0] = BT_State_A[3][1];
    BT_State_A[3][1] = BT_State_A[3][2];
    BT_State_A[3][2] = BT_State_A[3][3];
    BT_State_A[3][3] = temp_b;
}
/*******************************************************************************

    FUNCTION        MixColumns

    DESCRIPTION

********************************************************************************/
void BT_MixColumns(void)
{
    BYTE i, temp_a[4];

    for(i=0; i<4; i++)
    {
        temp_a[0] = BT_Multi_A(0x02,BT_State_A[0][i]) ^ BT_Multi_A(0x03,BT_State_A[1][i]) ^ BT_State_A[2][i] ^ BT_State_A[3][i];
        temp_a[1] = BT_State_A[0][i] ^ BT_Multi_A(0x02,BT_State_A[1][i]) ^ BT_Multi_A(0x03,BT_State_A[2][i]) ^ BT_State_A[3][i];
        temp_a[2] = BT_State_A[0][i] ^ BT_State_A[1][i] ^ BT_Multi_A(0x02,BT_State_A[2][i]) ^ BT_Multi_A(0x03,BT_State_A[3][i]);
        temp_a[3] = BT_Multi_A(0x03,BT_State_A[0][i]) ^ BT_State_A[1][i] ^ BT_State_A[2][i] ^ BT_Multi_A(0x02,BT_State_A[3][i]);
        BT_State_A[0][i] = temp_a[0];
        BT_State_A[1][i] = temp_a[1];
        BT_State_A[2][i] = temp_a[2];
        BT_State_A[3][i] = temp_a[3];
    }
}
/*******************************************************************************

    FUNCTION        InvMixColumns

    DESCRIPTION

********************************************************************************/
void BT_InvMixColumns(void)
{
    BYTE i, temp_a[4];

    for(i=0; i<4; i++)
    {
        temp_a[0] = BT_Multi_A(0x0e,BT_State_A[0][i]) ^ BT_Multi_A(0x0b,BT_State_A[1][i]) ^ BT_Multi_A(0x0d,BT_State_A[2][i]) ^ BT_Multi_A(0x09,BT_State_A[3][i]);
        temp_a[1] = BT_Multi_A(0x09,BT_State_A[0][i]) ^ BT_Multi_A(0x0e,BT_State_A[1][i]) ^ BT_Multi_A(0x0b,BT_State_A[2][i]) ^ BT_Multi_A(0x0d,BT_State_A[3][i]);
        temp_a[2] = BT_Multi_A(0x0d,BT_State_A[0][i]) ^ BT_Multi_A(0x09,BT_State_A[1][i]) ^ BT_Multi_A(0x0e,BT_State_A[2][i]) ^ BT_Multi_A(0x0b,BT_State_A[3][i]);
        temp_a[3] = BT_Multi_A(0x0b,BT_State_A[0][i]) ^ BT_Multi_A(0x0d,BT_State_A[1][i]) ^ BT_Multi_A(0x09,BT_State_A[2][i]) ^ BT_Multi_A(0x0e,BT_State_A[3][i]);
        BT_State_A[0][i] = temp_a[0];
        BT_State_A[1][i] = temp_a[1];
        BT_State_A[2][i] = temp_a[2];
        BT_State_A[3][i] = temp_a[3];
    }
}
/*******************************************************************************

    FUNCTION        Multi_A

    DESCRIPTION

********************************************************************************/
BYTE BT_Multi_A(BYTE factor, BYTE varible)
{
    BYTE const *ptr;
    BYTE i, outp_v;

    switch(factor)
    {
        case 0x02:
            ptr = BT_Factor02;
            break;
        case 0x03:
            ptr = BT_Factor03;
            break;
        case 0x09:
            ptr = BT_Factor09;
            break;
        case 0x0b:
            ptr = BT_Factor0b;
            break;
        case 0x0d:
            ptr = BT_Factor0d;
            break;
        case 0x0e:
            ptr = BT_Factor0e;
            break;
        default:
            return 0;
    }

    i = outp_v = 0;
    while(i<8)
    {
        if(varible & 0x01)
            outp_v = outp_v ^ ptr[i];

        varible = varible >> 1;
        i++;
    }

    return(outp_v);
}

/*********************************************************************
* Name:         KeyExpansion
*
* Description:  The AES algorithm takes the Cipher Key, K, and performs
*               a Key Expansion routine to generate a key schedule.
*
* Params:       Input: Cipher_Key[16]
*
* Returns:      none
*
* Global outputs: Key_Ex[44]
*
* Notes:
*
*********************************************************************/
void BT_KeyExpansion(BYTE *key)
{
    ULONG temp;
    BYTE i;

    i = 0;

    while(i < 4)
    {
        BT_Key_Ex[i] = MAKE_LONG(key[i*4],key[i*4+1],key[i*4+2],key[i*4+3]);
        i = i + 1;
    }

    while(i < 44)
    {
        temp = BT_Key_Ex[i-1];
        if((i%4) == 0)
        {
            temp = BT_Sub_Rot_Word(temp) ^ BT_Rcon[i/4];
        }

        BT_Key_Ex[i] = BT_Key_Ex[i-4] ^ temp;
        i++;
    }

}
/*********************************************************************
* Name:         Sub_Rot_Word
*
* Description:  SubWord() is a function that takes a four-byte input word and
*               applies the S-box to each of the four bytes to producean output word.
*               RotWord() takes a word[a0,a1,a2,a3] as input, performs a
*               cyclic permutation, and returns the word[a1,a2,a3,a0].
*
* Params:       Input: four-byte word
*
* Returns:      output: four-byte word
*
* Global outputs: none
*
* Notes:
*
*********************************************************************/
ULONG BT_Sub_Rot_Word(ULONG fourbyte)
{
    BYTE output[4],i;

    /* Rot_Word */
    output[0] = (fourbyte >> 16) & 0xff;
    output[1] = (fourbyte >> 8) & 0xff;
    output[2] = fourbyte & 0xff;
    output[3] = (fourbyte >> 24);

    for(i=0;i<4;i++)
    {
        output[i] = BT_SubBytes_tbl[output[i]>>4][output[i]&0x0f];
    }

    return(MAKE_LONG(output[0],output[1],output[2],output[3]));
}

/*******************************************************************************

    FUNCTION        AESCbcDecrypt

    DESCRIPTION     AES Cipher Block Chaining(CBC) decrytion mode, which
                    features the combining of the plaintext blocks with the
                    previous ciphertext blocks. The CBC mode requires an IV
                    to combine with the first plaintext block

********************************************************************************/
void BT_AESCbcDecrypt(BYTE *pKey, BYTE *Iv, BYTE *In, BYTE *Out, ULONG Size)
{
    ULONG idx;
    BYTE i;

    // Decrypt the first block and XOR with the IV
    BT_AESDecrypt(pKey, In, Out, 0);
    for (i = 0; i < 16; i++)
        Out[i] = Out[i] ^ Iv[i];

    for (idx = 16; idx < Size; idx+=16)
    {
        // Decrypt and XOR with the previous input.
        BT_AESDecrypt(pKey, In+idx, Out+idx, 1);
        for (i = 0; i < 16; i++)
            Out[idx+i] = Out[idx+i] ^ In[idx+i-16];
    }
}

/*******************************************************************************

    FUNCTION        AESCbcEncrypt

    DESCRIPTION     AES Cipher Block Chaining(CBC) encrytion mode, which
                    features the combining of the plaintext blocks with the
                    previous ciphertext blocks. The CBC mode requires an IV
                    to combine with the first plaintext block. The IV need
                    not to be secret, but it must be unpredictable.

********************************************************************************/
void BT_AESCbcEncrypt(BYTE *pKey, BYTE *Iv, BYTE *In, BYTE *Out, ULONG Size)
{
    ULONG  idx;
    BYTE    i, BlockIn[16];

    // For the first block: XOR the message with the IV
    for (i = 0; i < 16; i++)
        BlockIn[i] = In[i] ^ Iv[i];

    // Encrypt the first block
    BT_AESEncrypt(pKey, BlockIn, Out, 0);

    for (idx = 16; idx < Size; idx+=16)
    {
        // XOR the next block with current output
        for (i = 0; i < 16; i++)
            BlockIn[i] = In[idx+i] ^ Out[idx-16+i];

        // Encrypt the next block
        BT_AESEncrypt(pKey, BlockIn, Out+idx, 1);
    }
}

//CMAC
#define SIZE_OF_CIPHER_BLK     128     // Number of bits per cmac cipher block

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void BT_GenerateCmac(BYTE *pKey, BYTE *pMessage, ULONG length, BYTE *pMac);
extern void BT_GenerateSubkeys(BYTE *pKey, BYTE *pSubkey1, BYTE *pSubkey2);

// ***************************************************************************
//                               VARIABLES
// ***************************************************************************

// Bit string required for the subkey generation process
const BYTE BT_Rb[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x87 };

BYTE BT_SubKey1[16], BT_SubKey2[16];
BYTE BT_MiddleKey[16];

// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************

/*********************************************************************
* Name:         GenerateCmac
*
* Description:  MAC generation process of CMAC
*
* Params:       Input: *pKey, *pMessage, length
*
* Returns:      none
*
* outputs:      *pMac
*
* Notes:
*
*********************************************************************/
void BT_GenerateCmac(BYTE *pKey, BYTE *pMessage, ULONG length, BYTE *pMac)
{
    ULONG  Mlen, NrOfBlks, NrOfBitsInLastBlk;
    ULONG  i, j;
    BYTE    Cipher[16], Msg[16];
    BOOL    IsLastBlk;

    // Intialisation: Define the function constants.
    Mlen = length * 8;
    NrOfBitsInLastBlk = Mlen % SIZE_OF_CIPHER_BLK;

    // Step 1: Apply the subkey generation process to K to produce K1 and K2.
    BT_GenerateSubkeys(pKey, &BT_SubKey1[0], &BT_SubKey2[0]);

    // Step 2: If Mlen = 0, let n = 1; else let n = [Mlen/b]
    if (Mlen == 0)
    {
        NrOfBlks = 1;
    }
    else
    {
        NrOfBlks = (Mlen + SIZE_OF_CIPHER_BLK - 1) / SIZE_OF_CIPHER_BLK;
        if (NrOfBitsInLastBlk == 0)
        {
            NrOfBitsInLastBlk = SIZE_OF_CIPHER_BLK;
        }
    }

    // Step 5: Let C0 = 0b.
    for (j = 0; j < 16; j++)
        Cipher[j] = 0;

    // Step 6: For i = 1 to n, let Ci = CIPHk(Ci-1 XOR Mi).
    for (i = 0; i < NrOfBlks; ++i)
    {
        IsLastBlk = (i == NrOfBlks - 1);

        // Step 3: Let M1, M2, ... , Mn-1, Mn* denote the unique sequence of bit
        // strings such that M = M1 || M2 || ... || Mn-1 || Mn*, where M1,
        // M2,..., Mn-1 are complete blocks.

        // Step 4: If Mn* is a complete block, let Mn = K1 XOR Mn*; else, let
        // Mn = K2 XOR (Mn*||10j), where j = nb-Mlen-1.
        if ((IsLastBlk) && (NrOfBitsInLastBlk != SIZE_OF_CIPHER_BLK))
        {
            // Incomplete block: Mn = K2 XOR (Mn*||10j), where j = nb-Mlen-1.
            for (j = 0; j < 16; j++)
                Msg[j] = 0;

            for (j = 0; j < (NrOfBitsInLastBlk/8); j++)
                Msg[j] = pMessage[i*16+j];

            Msg[NrOfBitsInLastBlk/8] |= 0x80;

            for (j = 0; j < 16; j++)
                Msg[j] = BT_SubKey2[j] ^ Msg[j];
        }
        else
        {
            // Complete block: Mn = K1 XOR Mn*

            for (j = 0; j < 16; j++)
                Msg[j] = pMessage[i*16+j];

            if (IsLastBlk)
            {
                for (j = 0; j < 16; j++)
                    Msg[j] = BT_SubKey1[j] ^ Msg[j];
            }
        }

        // Ci = CIPHk(Ci-1 XOR Mi).
        for (j = 0; j < 16; j++)
            Cipher[j] = Cipher[j] ^ Msg[j];

        BT_AESEncrypt(pKey, &(Cipher[0]), pMac, 0);

        // Step 7: Let T = Cn (In our case: Cn = T = Ci).
        for (j = 0; j < 16; j++)
            Cipher[j] = pMac[j];
    }

    // Step 8: Return T.
}


/*********************************************************************
* Name:         GenerateSubkeys
*
* Description:  MAC generation process of CMAC
*
* Params:       Input: *pKey
*
* Returns:      none
*
* outputs:      *pSubkey1, *pSubkey2
*
* Notes:
*
*********************************************************************/
void BT_GenerateSubkeys(BYTE *pKey, BYTE *pSubkey1, BYTE *pSubkey2)
{
    BYTE    Temp[16];
    BYTE    i, Carry;

    // Subkey Generation:

    // Step 1: Apply the block cipher to a block that consists entirely
    // of '0' bits.

    // L = CIPHk(0b)
    for (i = 0; i < 16; i++)
        Temp[i] = 0;

    BT_AESEncrypt(pKey, &Temp[0], &BT_MiddleKey[0], 0);

    // Step 2: The first subkey is derived from the resulting string, and,
    // conditionally, by XORing a constant that depends on the block size.

    // K1 = L << 1
    Carry = 0;
    for (i = 16; i > 0; --i)
    {
        pSubkey1[i-1] = ((BT_MiddleKey[i-1] << 1) | Carry);
        Carry = (BT_MiddleKey[i-1] >> 7);
    }

    if (BT_MiddleKey[0] & 0x80)
    {
        // K1 = K1 XOR Rb
        for (i = 0; i < 16; ++i)
            pSubkey1[i] = pSubkey1[i] ^ BT_Rb[i];
    }

    // Step 3: The second subkey is derived in the same manner from the
    // first subkey.

    // K2 = K1 << 1
    Carry = 0;
    for (i = 16; i > 0; --i)
    {
        pSubkey2[i-1] = ((pSubkey1[i-1] << 1) | Carry);
        Carry = (pSubkey1[i-1] >> 7);
    }

    if (pSubkey1[0] & 0x80)
    {
        // K2 = K2 XOR Rb
        for (i = 0; i < 16; ++i)
            pSubkey2[i] = pSubkey2[i] ^ BT_Rb[i];
    }
}
#endif  /* ENABLE_AACS== 1 */

/*---------------------------------------------------------------------------
    Boot Read Write DRAM function
----------------------------------------------------------------------------*/
void BT_WriteSDRAM(BYTE v1)
{
	SCSDRAM[Bt_Buf_dram_addr] = v1;
	if (Bt_Buf_dram_addr < END_DRAM_BUFFER) 	/* increase dram address by 1 */
		Bt_Buf_dram_addr++;
}

BYTE BT_ReadSDRAM(void)
{
    BYTE v1;

    v1 = (SCSDRAM[Bt_Buf_dram_addr]);
    if (Bt_Buf_dram_addr < END_DRAM_BUFFER)       /* increase dram address by 1 */
        Bt_Buf_dram_addr++;
    return (v1);
}




/*---------------------------------------------------------------------------
    Boot code BUILD_SENSE
----------------------------------------------------------------------------*/
void BT_BUILD_SENSE(BYTE snsky,BYTE snscode,BYTE snsqual)
{
	Bt_Sense.key = snsky;
	Bt_Sense.cod = snscode;
	Bt_Sense.qul = snsqual;

	if(snsky == 0x02 && Bt_Sense.cod == 0x3A)
	{
		Bt_Sense.cod = snscode;
	}
}

/*---------------------------------------------------------------------------
    Boot code SRST
----------------------------------------------------------------------------*/
void    BT_ATASoftResetService(void)
{
    BYTE    v1;

    send_msgB1('S','R','S','T');

    WRITE_REG(HAL_HSTREG_MAP->HICTL,0);

    for(v1 = 0; v1<0xff; v1++) ;

    do
    {
        v1 = READ_REG(HAL_HSTREG_MAP->RSSTAT);
    }
    while( TEST_BIT(v1, 7) );

    /* ATAPI signature */
    WRITE_REG(HAL_HSTREG_MAP->ATINT, 0x01);         /* Interrupt reason reg */
    WRITE_REG(HAL_HSTREG_MAP->ATSPA, 0x01);         /* sector number reg   */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);       /* BYTE count reg  */
    /* SATA is always Master only */
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x01);
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);
    WRITE_FIELD(HAL_HST_CLRBSY,1);

     for(v1 = 0; v1 < 0x10; v1++);                      /* Need delay to allow BUSY to be cleared before triggering Register FIS */

    WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0x34);         /* Setup Dev to Host Register FIS */
    WRITE_REG(HAL_HSTREG_MAP->TXREQ,HAL_HST_TXFISREQ); /* Trigger FIS */
}

/*---------------------------------------------------------------------------
    Boot code ATAPI_08
----------------------------------------------------------------------------*/
void BT_atapi_08_cmd(void)
{
    BYTE v1;

    send_msgB1('A','R','S','T');

    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_CTTRG);  /* START_XFER */


    WRITE_REG(HAL_HSTREG_MAP->ATDRS, 0x00);    /* SATA signature for AHCI */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);  /* BYTE count reg  */

    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);

    WRITE_FIELD(HAL_HST_CLRBSY,1);

    for(v1 = 0; v1 < 0x10; v1++);                      /* Need delay to allow BUSY to be cleared before triggering Register FIS */

    WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0x34);         /* Setup Dev to Host Register FIS */
    WRITE_REG(HAL_HSTREG_MAP->TXREQ,HAL_HST_TXFISREQ); /* Trigger FIS */
}

/*---------------------------------------------------------------------------
    Boot code BT_SetupHostTransferRegisters
----------------------------------------------------------------------------*/
void BT_SetupHostTransferRegisters(void)
{
    BYTE  i;
	USHORT Residue_word;

	/* Clear the SATA Transmit and Recieve FIFOs */
	WRITE_FIELD(HAL_HST_TXFIFPTR, ON);
	WRITE_FIELD(HAL_HST_RXFIFPTR, ON);

	for (i=0;i<0x05;i++);

	WRITE_FIELD(HAL_HST_TXFIFPTR, OFF);
	WRITE_FIELD(HAL_HST_RXFIFPTR, OFF);


#if (CHIP_REV <= 0xB3)
    /* (in absolute addressing mode) Xfer Limit check */
    if ( Bt_shATB >= BT_XFER_LIMIT_SIZE )
    {
        Bt_Buf_hxfer_blk = Bt_shATB/BT_XFER_PAGE_SIZE;
        Bt_Xfer_Residue = Bt_shATB % BT_XFER_PAGE_SIZE;
        if (Bt_Buf_hxfer_blk & 0x0001)
        {
            Bt_Buf_hxfer_blk--;
            Bt_Xfer_Residue += BT_XFER_PAGE_SIZE;
            Bt_shATB = Bt_Buf_hxfer_blk * BT_XFER_PAGE_SIZE;
        }
        Bt_shHstWdC = BT_XFER_PAGE_SIZE >> 1;
    }
    else
    {
        Bt_Xfer_Residue = 0;
        if ((Bt_shATB%4) != 0)                                    // Check for DWord transfer length
        {
            Bt_shATB = ((Bt_shATB/4) + 1)*4;

            WRITE_FIELD(HAL_HST_ODDWRDSUP,1);                     // Enable odd word transfer logic
        }
        Bt_shHstWdC = Bt_shATB >> 1;
        Bt_Buf_hxfer_blk = 1;
    }
#else
    if((Bt_shATB%4) != 0)                                     // Check for DWord transfer length
    {
        Bt_shATB = ((Bt_shATB/4) + 1)*4;

        WRITE_FIELD(HAL_HST_ODDWRDSUP,1);                       // Enable odd word transfer logic
    }

    if(Bt_shATB >= BT_XFER_LIMIT_SIZE)
    {
        Bt_Buf_hxfer_blk = Bt_shATB/BT_XFER_LIMIT_SIZE;
        Bt_Xfer_Residue = Bt_shATB % BT_XFER_LIMIT_SIZE;
        Bt_shHstWdC = BT_XFER_LIMIT_SIZE >> 1;
    }
    else
    {
        Bt_shHstWdC = Bt_shATB>>1;
        Bt_Xfer_Residue = 0;
        Bt_Buf_hxfer_blk = 1;
    }

    if(Bt_Xfer_Residue > 0)
    {
        Residue_word = Bt_Xfer_Residue/2;
        Bt_Buf_hxfer_blk++;
    }
    else
    {
        Residue_word = 0;
    }

    WRITE_FIELD(HAL_HST_RESWDCL,Residue_word);
    WRITE_FIELD(HAL_HST_RESWDCH,Residue_word>>8);
    WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif

    WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, Bt_Buf_hxfer_blk);         /* set the number of pages */
    WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Bt_Buf_hxfer_blk);         /* Load Host Available Page Count Registers */

    WRITE_REG(HAL_HSTREG_MAP->ATBLO, Bt_shATB);                   /* Set Byte Count */

    WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, Bt_Linear_Xfer_addr);    /* work area start addr */
    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, Bt_shHstWdC);              /* words count in each page */

}

/*---------------------------------------------------------------------------
    Boot code BT_TransferToHost
----------------------------------------------------------------------------*/
void BT_TransferToHost(void)
{
    Bt_shHDDir |= HAL_HST_HBODIS;                  /* Select linear addr */
    Bt_shHDDir &= (~HAL_HST_HXDIR);                /* Set Direction: device->host */
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);
    BT_SetupHostTransferRegisters();
    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */
}
/*---------------------------------------------------------------------------
    Boot code BT_SendStatus
----------------------------------------------------------------------------*/
void BT_SendStatus(void)
{

    if (Bt_Sense.key != 0)
    {
          /* Error Cases, non-zero sense */
        WRITE_REG(HAL_HSTREG_MAP->ATFEA, (((Bt_Sense.cod == 0x28)?HAL_HST_MCRR:0)|(Bt_Sense.key<<4)));
        WRITE_REG(HAL_HSTREG_MAP->TSKCMP1,  (HAL_HST_CDRDY|HAL_HST_CCHECK));   /* set CHECK bit */
    }
    else
    {
        /* No Error, zero sense */
        WRITE_REG(HAL_HSTREG_MAP->ATFEA, 0x00);
        WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
    }
}
/*---------------------------------------------------------------------------
    Boot code BT_AutoCommandComplete
----------------------------------------------------------------------------*/
void BT_AutoCommandComplete(void)
{
    send_msgB1('B','c','o','p');
    BT_cmd_avail = FALSE;
    BT_SendStatus();

    WRITE_FIELD(HAL_HST_SCTRG, 1);  /* STATUS_COMPLETION   */

    WRITE_FIELD(HAL_HST_ODDWRDSUP, 0);
}
/*---------------------------------------------------------------------------
    Boot code BT_HostTransferEndIntProc
----------------------------------------------------------------------------*/
void BT_HostTransferEndIntProc(void)
{

}
/*---------------------------------------------------------------------------
    Boot code BT_ataCheckDMA
----------------------------------------------------------------------------*/
void BT_ataCheckDMA(void)
{
    BYTE  v1;

    WRITE_REG(HAL_HSTREG_MAP->ATFEA, 0x00); /*[21h] clear previous error message */

    v1 = READ_REG(HAL_HSTREG_MAP->ATFEA);
    if ( v1 == 0xff )
        return;

    v1 &= HAL_HST_DMA;

    if (v1 == HAL_HST_DMA)
    {

        Bt_shHDDir &=0xF3;/* Clear the mode bits first  */
        Bt_shHDDir |=  HAL_HST_DMAMODE; /* set DMA mode */
        WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);
     }
    else
    {   /* PIO modes */
        Bt_shHDDir &= 0xF3;
        WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);
    }
}

/*---------------------------------------------------------------------------
    Boot code BT_GetPacketCommand
----------------------------------------------------------------------------*/
void BT_GetPacketCommand()
{
    BYTE a;
    #if (B1_CHIP_SATA_WA == 1)
    BYTE rxfifolv;
    #endif
    BT_ataCheckDMA();                          /* check if host request DMA */

    /* read 1f4/1f5 ATA Byte Count regs:max data can be xfer in every drq */
     Bt_shATB = READ_REG(HAL_HSTREG_MAP->ATBLO) & 0xFFFE;

    /* get 12 bytes packet command */
    #if (B1_CHIP_SATA_WA == 1)
    rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
    while(rxfifolv != 0x03) //make sure we can get correct data from FIFO.
    {
        rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
//        send_msg5SValue(1,"RXFIF",4,rxfifolv);
    }
    #endif
    for (a=0; a<12; a++)                    /* get 12 bytes packet command */
    {
        BT_Cmd_block[a] = READ_REG(HAL_HSTREG_MAP->ATAPIDATA);
    }
    #if (B1_CHIP_SATA_WA == 1)
    rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
    while(rxfifolv!=0x80)//make sure the FIFO is empty.
    {
        rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
    }
    #endif

    send_msgB2();

    WRITE_REG(HAL_HSTREG_MAP->ATCMD, (HAL_HST_DRDYW|HAL_HST_DSCW)); /* [627h] status reg, Set DSC*/

    BT_cmd_avail = TRUE;                       /* Indicate new command is available. */
}
/*---------------------------------------------------------------------------
    Boot code BT_ATAGenerateHIRQ
----------------------------------------------------------------------------*/
void BT_ATAGenerateHIRQ(void)
{

    WRITE_REG(HAL_HSTREG_MAP->UMISCR,(HAL_HST_HINTRQW | bit15));

}
/*---------------------------------------------------------------------------
    Boot code BT_ATA90CMD
----------------------------------------------------------------------------*/
void BT_ATA90CMD(void)
{
    BYTE v1;

    WRITE_REG(HAL_HSTREG_MAP->HICTL,0);
    for(v1 = 0; v1 < 0xFF; v1++);

    /* ATAPI signature */
    WRITE_REG(HAL_HSTREG_MAP->ATINT, 0x01);         /* Interrupt reason reg */
    WRITE_REG(HAL_HSTREG_MAP->ATSPA, 0x01);         /* sector number reg   */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);       /* BYTE count reg  */
    /* SATA is always Master only */
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x01);
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);

    WRITE_FIELD(HAL_HST_CLRBSY,1);

    for(v1 = 0; v1 < 0x10; v1++);

    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_SCTRG);  /* STATUS_COMPLETION   */
}

/*---------------------------------------------------------------------------
    Boot code BT_ATACommandComplete
----------------------------------------------------------------------------*/
void    BT_ATACommandComplete(void)
{
    BT_cmd_avail = FALSE;

    WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);           /* No Error */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);           /* set DRDY, DSC */

    WRITE_FIELD(HAL_HST_CLRBSY,1);
    BT_ATAGenerateHIRQ();                      /* set intrq to interrupt host */

    WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);  /* STATUS_COMPLETION   */
}

/*---------------------------------------------------------------------------
    Boot code BT_ATACommandFail
----------------------------------------------------------------------------*/
void    BT_ATACommandFail(void)
{
    BT_cmd_avail = FALSE;

    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x04);           /* set ABRT bit */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x51);           /* set DRDY, DSC, & CHECK */
    WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);
}

/*---------------------------------------------------------------------------
    Boot code BT_ataIdentifyPacketDeviceCommand
----------------------------------------------------------------------------*/
void BT_ataIdentifyPacketDeviceCommand(void)
{
    UINT i;
    BYTE checksum = 0, tmp;

    Bt_shHDDir &= 0xF3;                   /* set to PIO mode */

    WRITE_REG(HAL_HSTREG_MAP->HDDIR,Bt_shHDDir);  /* multiple bits changed/removed/added */

    Bt_Linear_Xfer_addr = IDENTIFY_ADR;
    Bt_shATB  = IDENTIFY_LEN_BYTE;        /* xfer length BYTE count */
    Bt_Buf_hxfer_blk = 1;                 /* force as single block */

    Bt_Buf_dram_addr = IDENTIFY_ADR;
    for(i=0; i<IDENTIFY_LEN_BYTE-1; i++)/* byte 0 ~ byte 510 */
        checksum += SCSDRAM[Bt_Buf_dram_addr+i];

    checksum = (~checksum) + 1;
    SCSDRAM[IDENTIFY_ADR+IDENTIFY_LEN_BYTE-1] = checksum;

    BT_TransferToHost();                       /* sent data to host */

    do
    {
    	tmp = READ_REG(HAL_HSTREG_MAP->HINT0);
    } while ((tmp & HAL_HST_DTE) == 0);
    WRITE_FIELD(HAL_HST_DTE,0);


    WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);           /* No Error */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);           /* set DRDY, DSC */

    WRITE_FIELD(HAL_HST_CLRBSY,1);
    BT_cmd_avail = FALSE;
}
/*---------------------------------------------------------------------------
    Boot code BT_ATACommandIntProc
----------------------------------------------------------------------------*/
void    BT_ATACommandIntProc(void)
{
    BYTE    a;

    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);  /* No Error */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50); /* set DRDY, DSC */

    a = READ_REG(HAL_HSTREG_MAP->ATCMD);      /* get the command code */

	if (a == ATA_NOP)
	{
		WRITE_REG(HAL_HSTREG_MAP->REQTXCNTL,0x0B40);
	}

    send_msgB1('A','T','A',a);

    if(a ==  ATA_IDENTIFY_PACKET_DEVICE)    /* Identify command */
    {
        WRITE_REG(HAL_HSTREG_MAP->HICTL,0);
        BT_ataIdentifyPacketDeviceCommand();
        return;
    }
    BT_ataCheckDMA();
    BT_cmd_avail = TRUE;                   /* mark command need service */
    BT_Cmd_block[0] = a;
}

/*---------------------------------------------------------------------------
    Boot code BT_HostIRQService
----------------------------------------------------------------------------*/
void BT_HostIRQService(void)
{
    BYTE HSts0;
    BYTE HSts1;
    BYTE ATACmd;

    HSts0 = READ_REG(HAL_HSTREG_MAP->HINT0);    /* HIF interrupt */
    WRITE_REG(HAL_HSTREG_MAP->HINT0, ~HSts0);

    if (HSts0)
    {
        send_msgB1('I', 'R', 'Q',HSts0);

        if (HSts0 & HAL_HST_PCMD)   /* Move ATAPI Command Int process to front because DTE comes with it */
        {   /* ATAPI packet command interrupt */
            BT_GetPacketCommand();       /* get 12 BYTE packet command */
            WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));
        }
        else if (HSts0 & HAL_HST_SRST)
        {   /* host SRST interrupt  */
            BT_ATASoftResetService();
        }
        else if (HSts0 & HAL_HST_ARST)
        {   /* ATAPI software reset cmd(08)*/
            BT_atapi_08_cmd();
        }
        else if (HSts0 & HAL_HST_CMD)   /* ATA command interrupt */
        {
            ATACmd = READ_REG(HAL_HSTREG_MAP->ATCMD);  /* read AT command       */
            if ( ATACmd != 0xA0 )   /* if A0 packet command? */
            {   /* not A0 packet cmd     */
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));
                if ( ATACmd == 0x90 )
                    BT_ATA90CMD();   /* execute diagnostic command */
                else
                    BT_ATACommandIntProc(); /* other ATA command     */
            }
        }
        if (HSts0 & HAL_HST_DTE)
        {
            BT_HostTransferEndIntProc();
        }
    }

    HSts1 =  READ_REG(HAL_HSTREG_MAP->SATAINT1);

    if (HSts1)
    {
        send_msgB1('I', 'R', '1',HSts1);

        WRITE_REG(HAL_HSTREG_MAP->SATAINT1, (BYTE)(~ HSts1));
        if (HSts1 & HAL_HST_COMRSTINT)
        {
            send_msgB1('C','O','M','R');
            //We are in COMRESET. If SATA bus is sleeping, then wake it up.
            if ((READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL)) != 0)
            {
                USHORT i;

                WRITE_FIELD(HAL_HST_DWCLKON, 1);
                WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);

                for (i=0; i<1000; i++)
                {
                    if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                        break;
                }
            }

#if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))
            /* Auto FIS34 Work Around.*/
            if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
            {
                send_msgB1('P','H','Y','R');
                //ATAClearBusy();
                WRITE_REG(HAL_HSTREG_MAP->HICTL, HAL_HST_CLRBSY);
                WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
                WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
                WRITE_FIELD(HAL_HST_AFIS34DIS, 1);

                BT_SendFis34Already = 1;
                //St_unit_atten = TRUE;  //Indicate that BUS DEVICE RESET OCCURED
            }
#endif
        }
    }

}


/*---------------------------------------------------------------------------
    Boot code BT_ataSetFeatureCommand
----------------------------------------------------------------------------*/
void BT_ataSetFeatureCommand(void)
{
    BYTE FEAreg, SECreg, mode_num;

    FEAreg = READ_REG(HAL_HSTREG_MAP->ATFEA);   /* read feature register */
    SECreg = READ_REG(HAL_HSTREG_MAP->ATINT);   /* read Sector Count register */

    switch (FEAreg)
    {
        case SET_XFR_MODE:/* 0x03 */
            mode_num = SECreg & 0x07;           /* mode number */
            switch(SECreg & 0xF8)               /* read Sector Count reg.*/
            {
                case DEF_XFR_MODE:              /* PIO default xfer mode */
                    break;

                case DISABLE_IORDY:             /* PIO default xfer mode, diable IORDY */
                    break;

                case PIO_XFR_MODE:              /* PIO flow control xfer mode X */
                    break;

                case SINGLE_WORD_DMA:

                    Bt_Buf_dram_addr = IDENTIFY_ADR + (62*2 + 1);
                    mode_num = 0x01 << mode_num;
                    BT_WriteSDRAM(mode_num);
                    Bt_Buf_dram_addr = IDENTIFY_ADR + (63*2 + 1);
                    BT_WriteSDRAM(0x00);
                    Bt_Buf_dram_addr = IDENTIFY_ADR + (88*2 + 1);
                    BT_WriteSDRAM(0x00);
                    break;

                case MULTI_WORD_DMA:

                    Bt_Buf_dram_addr = IDENTIFY_ADR + (63*2 + 1);
                    mode_num = 0x01 << mode_num;
                    BT_WriteSDRAM(mode_num);
                    Bt_Buf_dram_addr = IDENTIFY_ADR + (62*2 + 1);
                    BT_WriteSDRAM(0x00);
                    Bt_Buf_dram_addr = IDENTIFY_ADR + (88*2 + 1);
                    BT_WriteSDRAM(0x00);

                    break;

                case UDMA_MODE:

                    switch(mode_num)
/*** SATA IF does not care about the UDMA mode. Still check for valid modes as required by ATAPI ***/
                    {
                        //start == Dell vista cannot find drive in bootmode.
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                            break;
                        //end == Dell vista cannot find drive in bootmode.

                        default:
                        {
                            BT_ATACommandFail();     /* invalid mode */
                            return;
                        }
                    }

                    Bt_Buf_dram_addr = IDENTIFY_ADR + (88*2 + 1);
                    mode_num = 0x01 << mode_num;
                    BT_WriteSDRAM(mode_num);
                    Bt_Buf_dram_addr = IDENTIFY_ADR + (62*2 + 1);
                    BT_WriteSDRAM(0x00);
                    Bt_Buf_dram_addr = IDENTIFY_ADR + (63*2 + 1);
                    BT_WriteSDRAM(0x00);

                    break;

                default:
                    BT_ATACommandFail();             /* xfer/mode value invalid */
                    return;
            }
			break;
#if 0
/* Supported Automatic Acoustic Management */
        case ENABLE_AAM:
            if (SECreg == MAX_Preformance)
            {
                AcousticSpeed(MAX_READ_SPEED);
            }
            else if ((SECreg < MAX_Preformance) && (SECreg >= IntermediateAM_lvl4))
                    {
                        AcousticSpeed(e48XCAV);
                    }
                else if (SECreg == IntermediateAM_lvl3)
                    {
                        AcousticSpeed(e48XCAV);
                    }
                    else if (SECreg == IntermediateAM_lvl2)
                        {
                            AcousticSpeed(e40XCAV);
                        }
                        else if (SECreg == IntermediateAM_lvl1)
                            {
                                AcousticSpeed(e32XCAV);
                            }
                            else if (SECreg == MIN_AE_lvl)
                                {
                                    AcousticSpeed(e24XCAV);
                                }
                                else
                                {           /* Another value */
                                    Bt_Buf_dram_addr = IDENTIFY_ADR + 173;/* word86 bit9 set */
                                    SCSDRAM[Bt_Buf_dram_addr] |= 0x02;
                                    break;
                                }
            /* changed Identfy packet deta */
            Bt_Buf_dram_addr = IDENTIFY_ADR + 173;            /* word86 bit9 set */
            SCSDRAM[Bt_Buf_dram_addr] |= 0x02;
            Bt_Buf_dram_addr = IDENTIFY_ADR + 188;            /* word94 bit7-0 Current Automatic Acoustic Management value */
            SCSDRAM[Bt_Buf_dram_addr] = SECreg;
            break;

        case DISABLE_AAM:
            AcousticSpeed(MAX_READ_SPEED);
            /* changed Identfy packet deta */
            Bt_Buf_dram_addr = IDENTIFY_ADR + 173;            /* word86 bit9 clear */
            SCSDRAM[Bt_Buf_dram_addr] &= 0xFD;
            Bt_Buf_dram_addr = IDENTIFY_ADR + 188;            /* word94 bit7-0 Current Automatic Acoustic Management value clear */
            SCSDRAM[Bt_Buf_dram_addr] = 0xFE;
            break;
/* Supported Automatic Acoustic Management */
#endif
		case ENABLE_SATA_FEATURE:
			switch(SECreg)										/* read Sector Count reg.*/
			{
				case INIT_PWR_STATE_TRANS:						/* For SATA interface power management */
					SATA_FE |= bINTFPWR;
					break;
				case ASYN_NOTIFICATION:							/* Asynchronous Notification */
					SATA_FE |= bASYNNOTI;
					break;
				case SSP:										/* Software Settings Preservation */
					SATA_FE |= bSSP;
					break;
				default:
					BT_ATACommandFail();
					return;
			}
			break;
		case DISABLE_SATA_FEATURE:
			switch(SECreg)										/* read Sector Count reg.*/
			{
				case INIT_PWR_STATE_TRANS:						/* For SATA interface power management */
					SATA_FE &= (BYTE)(~bINTFPWR);
					send_msgB1('I','P','M','d');
					break;
				case ASYN_NOTIFICATION:							/* Asynchronous Notification */
					SATA_FE &= (BYTE) (~bASYNNOTI);
					break;
				case SSP:										/* Software Settings Preservation */
					SATA_FE &= (BYTE) (~bSSP);
					break;
				default:
					BT_ATACommandFail();
					return;
			}
			break;

		case ENABLE_MEDIA_STATUS_NOTIFICATION:
			WRITE_FIELD(HAL_HST_TBL, 0x00);							/* set VER to 0x00 */
			if (SCSDRAM[IDENTIFY_ADR + (85*2)] & bit2)
				WRITE_FIELD(HAL_HST_TBH, 0x07);					/* set bit PEJ, LOCK and PENA */
			else
				WRITE_FIELD(HAL_HST_TBH, 0x06);
			SCSDRAM[IDENTIFY_ADR + (85*2)] |= bit2;
			break;

		case DISABLE_MEDIA_STATUS_NOTIFICATION:
			SCSDRAM[IDENTIFY_ADR + (85*2)] &= (BYTE)(~bit2);
			break;

        default:
            BT_ATACommandFail();									/* xfer/mode value invalid */
            return;
    }
    BT_ATACommandComplete();
}
/*---------------------------------------------------------------------------
    Boot code BT_SendParameterDataToHost
----------------------------------------------------------------------------*/
void BT_SendParameterDataToHost(ULONG Address, USHORT ByteCount, FLAG Mode)
{
	vBOOL tmp;

    Bt_Linear_Xfer_addr = Address;

    if (ByteCount & 0x01)  ByteCount++;   /* Convert to Word Count */
    Bt_shATB  = ByteCount;

    Bt_Buf_hxfer_blk = 1;

                                        /* execute xfr */
    Bt_shHDDir |= HAL_HST_HBODIS;                  /* Select linear addr */
    Bt_shHDDir &= (~HAL_HST_HXDIR);                /* Set Direction: device->host */
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);
    BT_SetupHostTransferRegisters();
    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */

    do
    {
    	tmp = READ_REG(HAL_HSTREG_MAP->HINT0);
    } while ((tmp & HAL_HST_DTE) == 0);
    WRITE_FIELD(HAL_HST_DTE,0);

    if(Bt_Xfer_Residue)
    {
        Bt_Linear_Xfer_addr += Bt_shATB;
        Bt_shATB = Bt_Xfer_Residue;

        Bt_Buf_hxfer_blk = 1;

                                           /* execute xfr */
        BT_TransferToHost();

        do
        {
        	tmp = READ_REG(HAL_HSTREG_MAP->HINT0);
        } while ((tmp & HAL_HST_DTE) == 0);
        WRITE_FIELD(HAL_HST_DTE,0);

    }

    if (Mode == TRUE)
        BT_AutoCommandComplete();         // Send Command Complete to host if requested by calling function
}

/*---------------------------------------------------------------------------
    Boot code BT_GetParameterDataFromHost
----------------------------------------------------------------------------*/
void BT_GetParameterDataFromHost(ULONG Address, USHORT ByteCount)
{
    vBOOL tmp;
        Bt_Linear_Xfer_addr = Address;
        Bt_shATB = ByteCount;

        Bt_shHDDir |= HAL_HST_HXDIR | HAL_HST_HBODIS;
        WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);

        BT_SetupHostTransferRegisters();

        WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */

        do
        {
        	tmp = READ_REG(HAL_HSTREG_MAP->HINT0);
        } while ((tmp & HAL_HST_DTE) == 0);
        WRITE_FIELD(HAL_HST_DTE,0);

        if(Bt_Xfer_Residue)
        {

            Bt_Linear_Xfer_addr += Bt_shATB;
            Bt_shATB = Bt_Xfer_Residue;

            BT_SetupHostTransferRegisters();

            WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */

            do
            {
            	tmp = READ_REG(HAL_HSTREG_MAP->HINT0);
            } while ((tmp & HAL_HST_DTE) == 0);
            WRITE_FIELD(HAL_HST_DTE,0);
        }
}

/*---------------------------------------------------------------------------
    Boot code BT_RequestSenseCommand
----------------------------------------------------------------------------*/
void    BT_RequestSenseCommand(void)
{
    USHORT  XferLength;
    USHORT  progressResult;
    BYTE    bSKSV;

    bSKSV = 0x00;
    progressResult = 0;

    /* check allocation length in command BYTE 4, return command completed
        if allocation length equal zero */

    if((BT_Cmd_block[4] & 0xfe) == 0)
    {       /* if allocation length = 0 */
        BT_BUILD_SENSE(0x00,0x00,0x00);            /* init sense key */
        BT_AutoCommandComplete();
        return;
    }

    Bt_Buf_dram_addr = REQ_SENSE_ADR + 0x0F;
    BT_WriteSDRAM( bSKSV );
    BT_WriteSDRAM( BHIGH( progressResult ) );
    BT_WriteSDRAM( BLOW ( progressResult ) );


    Bt_Buf_dram_addr = REQ_SENSE_ADR +2;      /* adjust to sense key address */
    BT_WriteSDRAM(Bt_Sense.key);                    /* write sense key data to SDRAM */

    Bt_Buf_dram_addr = REQ_SENSE_ADR + 0x0C;  /* adjust to sense code dram address */
    BT_WriteSDRAM(Bt_Sense.cod);                    /* write sense code to SDRAM */

    Bt_Buf_dram_addr = REQ_SENSE_ADR + 0x0D;  /* adjust to sense qualifier SDRAM address */
    BT_WriteSDRAM(Bt_Sense.qul);                    /* write sense code to SDRAM */

    XferLength = REQ_SENSE_LEN_BYTE;

    XferLength = MINIMUM( REQ_SENSE_LEN_BYTE, (USHORT)BT_Cmd_block[4]);  // Number of bytes to xfer. Compare, return minimum.

    send_msgB1(0xee, Bt_Sense.key, Bt_Sense.cod, Bt_Sense.qul);

    BT_BUILD_SENSE(0x00,0x00,0x00);            /* init sense key */
    BT_SendParameterDataToHost(REQ_SENSE_ADR, XferLength , TRUE);    // Send Data to host and Command Complete.
}
/*---------------------------------------------------------------------------
    Boot code BT_InquiryCommand
----------------------------------------------------------------------------*/
void BT_InquiryCommand(void)
{
    BYTE    allocLength, *TargetPtr, *SourcePtr, i;

    /* check allocation length, return command completed if allocation length
    equal zero */

    if ((BT_Cmd_block[4] & 0xfe) == 0) /* if allocation length=0 */
    {
        BT_AutoCommandComplete();
        return;
    }
    /* setup registers to return standard inquiry data to host */


    /* round down if allocation length is odd */
    allocLength = BT_Cmd_block[4];
    if ( allocLength & 0x01)
     allocLength--;

    TargetPtr = (BYTE *)(SCSDRAM_START_ADDR);
    SourcePtr = (BYTE *)(SCSDRAM_START_ADDR + INQUIRY_ADR);
    /* Initialize the returned data */
    /* Copy original Inqury data to SDSDRAM START */
    for (i = 0; i < INQUIRY_LEN_BYTE; i++)
        *TargetPtr++ = *SourcePtr++;
    /* Set the remaining data to zero */
    for (i = INQUIRY_LEN_BYTE; i < allocLength; i++)
        SCSDRAM[i] = 0x00;

    //allocLength = MINIMUM( INQUIRY_LEN_BYTE, (USHORT)allocLength);  // Number of bytes to xfer. Compare, return minimum.
                                        /* chk allocation & actual data len */

    BT_BUILD_SENSE(0x00,0x00,0x00);/* init sense key *///Dell vista cannot find drive in bootmode.
    BT_SendParameterDataToHost(0x00,allocLength, TRUE);
}
/*---------------------------------------------------------------------------
    Boot code BT_ModeSenseCommand
----------------------------------------------------------------------------*/
void BT_ModeSenseCommand(void)
{
    /* check allocation length in command BYTE 8, return command completed
       if allocation length equal zero */
    if(((BT_Cmd_block[8] & 0xfe) | BT_Cmd_block[7]) == 0)
    {
        BT_AutoCommandComplete();
        return;
    }

    /* check page control (PC) field, return error if PC is saved values (equal 3) */
    if((BT_Cmd_block[2] & 0xc0) == 0xc0)
    {
        BT_BUILD_SENSE(0x05,0x39,0x00);    /*SAVING PARAMETERS NOT SUPPORTED */
        BT_AutoCommandComplete();
        return;
    }

    BT_BUILD_SENSE(0x05,0x24,0x00);    /*SAVING PARAMETERS NOT SUPPORTED */
    BT_AutoCommandComplete();
    return;
}
/*---------------------------------------------------------------------------
    Boot code BT_TransferNotificationData
----------------------------------------------------------------------------*/
void BT_TransferNotificationData(void)
{

    USHORT      alloc_len;      /* allocation length */
    BYTE        TransferClass;

    /* get allocation length */
    alloc_len = (USHORT)BT_Cmd_block[7] * 0x100 + BT_Cmd_block[8];

    /*****************************************/
    /***** Event Change Check & Data Set *****/
    /*****************************************/
    TransferClass = 0x00;

    /* ---------------------------------------------
     *  Operational Change/Notification Class Events
     */
    if ( BT_Cmd_block[4] & 0x02 )
    {
         /*
         *  set Operational Change/Notification into SDRAM buffer
         */
        Bt_Buf_dram_addr = GESN_OPECHGREQ_ADR + 4;    /* add offset for operational status */
        BT_WriteSDRAM( 0x02 );                         /* write operational event */

        /*
        Operational Status is always 0 - Logical Unit is ready
        PersistenPrevented bit is set/cleared at Prevent/Allow Medium command
        So we don't need to update byte 1
        */

        Bt_Buf_dram_addr++;

        BT_WriteSDRAM( 0x00 );                        /* write request/report */
        BT_WriteSDRAM( 0x02 );

        TransferClass = 0x02;

    }

    /* ---------------------------------------------
     *  Power Management Class Events
     */
    if ( (BT_Cmd_block[4] & 0x04) && (TransferClass == 0x00) )
    {

        /*
         *  set Power Event & Status into DRAM buffer
         */
        Bt_Buf_dram_addr = GESN_POWER_ADR + 4;        /* add offset for event status */
        BT_WriteSDRAM( 0x00 );                     /* write power event */
        BT_WriteSDRAM( 0x02 );                    /* write power status */
    }


    /* ---------------------------------------------
     *  Device Busy Status Class Events
     */
    if ( (BT_Cmd_block[4] & 0x40) && (TransferClass == 0x00) )
    {

        Bt_Buf_dram_addr = GESN_DEVICEBUSY_ADR + 4;
        BT_WriteSDRAM( 0x00 );
        BT_WriteSDRAM( 0x00 );
        BT_WriteSDRAM( 0x00 );
        BT_WriteSDRAM( 0x00 );
    }

    /*****************************/
    /***** Transfer Data Set *****/
    /*****************************/
    /* ---------------------------------------------
     *  Operational Change/Notification Class Events
     */
    if ( (BT_Cmd_block[4] & 0x02) && ((TransferClass == 0x00) || (TransferClass == 0x02)) )
    {
        /*
         *  trasfer only Event Header with NEA to Host
         */
        Bt_Linear_Xfer_addr = GESN_OPECHGREQ_ADR;     /* SDRAM address */
        Bt_shATB = GESN_OPECHGREQ_LEN;                /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  Power Management Class Events
     */
    else if ( (BT_Cmd_block[4] & 0x04) && ((TransferClass == 0x00) || (TransferClass == 0x04)) )
    {
        /*
         *  trasfer Power Management Status data to Host
         */
        Bt_Linear_Xfer_addr = GESN_POWER_ADR;         /* SDRAM address */
        Bt_shATB = GESN_POWER_LEN;                    /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  External Request Class Events
     */
    else if ( (BT_Cmd_block[4] & 0x08) && (TransferClass == 0x00) )
    {
        /*
         *  trasfer Media Event Class data to Host
         */
        Bt_Linear_Xfer_addr = GESN_EXTERNAL_ADR;      /* SDRAM address */
        Bt_shATB = GESN_EXTERNAL_LEN;                 /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  Media Status Class Events
     */
    else if ( (BT_Cmd_block[4] & 0x10) && ((TransferClass == 0x00) || (TransferClass == 0x10)) )
    {
        /*
         *  trasfer Media Event Class data to Host
         */
        Bt_Linear_Xfer_addr = GESN_MEDIA_ADR;         /* SDRAM address */
        Bt_shATB = GESN_MEDIA_LEN;                    /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  Device Busy Status Class Events
     */
    else if ( (BT_Cmd_block[4] & 0x40) && ((TransferClass == 0x00) || (TransferClass == 0x40)) )
    {
        /*
         *  trasfer Media Event Class data to Host
         */
        Bt_Linear_Xfer_addr = GESN_DEVICEBUSY_ADR;    /* SDRAM address */
        Bt_shATB = GESN_DEVICEBUSY_LEN;               /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  NEA (No Class Events)
     */
    else
    {
        /*
         *  transfer only Event Header with NEA to Host
         */
        Bt_Linear_Xfer_addr = GESN_NEA_ADR;           /* SDRAM address */
        Bt_shATB = GESN_NEA_LEN;                      /* BYTE lenngth */
        if( Bt_shATB < alloc_len ){
            ;   /* Nothing to do */
        }else{
            Bt_shATB = alloc_len;
        }
    }

    Bt_shATB = MINIMUM( Bt_shATB, alloc_len);  // Number of bytes to xfer. Compare, return minimum.

    BT_SendParameterDataToHost(Bt_Linear_Xfer_addr,Bt_shATB, TRUE );
}

/*---------------------------------------------------------------------------
    Boot code BT_GetEventStatusNotificationCommand
----------------------------------------------------------------------------*/
void    BT_GetEventStatusNotificationCommand(void)
{
    /*
     *  check immediate bit
     *  only suport Polling mode(IMMED = 1)
     */
    if( !(BT_Cmd_block[1] & 0x01) )
    {
        BT_BUILD_SENSE(0x05,0x24,0x00);  /* invalid field in command packet */
        BT_AutoCommandComplete();
    }

    /*
     *  check allocation length
     */
    else if ( ((BT_Cmd_block[8] & 0xfe) | BT_Cmd_block[7]) == 0 )
    {
        BT_BUILD_SENSE(0x00,0x00,0x00);  /* no sense */
        BT_AutoCommandComplete();
    }

    /*
     *  transfer Notification Class Data from DRAM buffer to Host
     */
    else
    {
        BT_TransferNotificationData();

    }
}

/*---------------------------------------------------------------------------
    Boot code BT_InitializeSystem
----------------------------------------------------------------------------*/
void BT_InitializeSystem(void)
{
    BYTE    i;

    WRITE_REG(HAL_HSTREG_MAP->SATAINTEN, (HAL_HST_BISTIPEN | HAL_HST_RXPMACKEN | HAL_HST_RXPMNAKEN | HAL_HST_ERRTXRXEN));
    WRITE_REG(HAL_HSTREG_MAP->SHCTRL, HAL_HST_DWCLKON|HAL_HST_COMRSTDIS);
    //WRITE_FIELD(HAL_HST_F34RDYCNT,0x01); /* Set FIS34 Ready Count 1, PHYRDY 13.904ms issue FIS34 */
    WRITE_FIELD(HAL_HST_F34RDYCNT,0x27); /* Set FIS34 Ready Count 0x27, PHYRDY 500ms issue FIS34 */

    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG0, PLLCFG0);           // Set up parameters for SYSPLL
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG1, PLLCFG1);           // Set up parameters for SYSPLL
    WRITE_REG(HAL_GLOBALREG_MAP->SYSPLLCFG2, PLLCFG2);           // Set up parameters for SYSPLL
    WRITE_FIELD(HAL_GLOBAL_SYSPLLN, PLLCON);                     // Set up parameters for SYSPLL
    WRITE_FIELD(HAL_GLOBAL_SYSPLLM, PLLCOM);                     // Set up parameters for SYSPLL
    WRITE_FIELD(HAL_GLOBAL_SYSPLLK, PLLCOK);                     // Set up parameters for SYSPLL

    for (i = 0; i < 25; i++){ }               // Delay 25 instructions to allow SYSCLK to stablize at Frequency

    // Now set up the clock frequency for the Serial Flash Interface.
    WRITE_FIELD(HAL_AHB_SFLASH_SFCLKSEL,3);

    /******************* Work Around for ARM hang on PRST *********************
    All clocks are stopped as default setting on PRST
    asserted. The delay "for" loops below uses an SDRAM location as the loop
    variable.  If the memory clock stop (CLKSTP1.MEMCKSTP) is not disabled (=0)
    then the ARM will hang when accessing the SDRAM */
    /**************************************************************************/
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1, 0x00);          //Enable the clocks
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4, 0x00);
    WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5, 0x00);
/*************************************************************************************/

/* The SDRAM SDCONFx registers should be programmed at the default frequency of 33 MHz */
/* Always intialize these registers before changing the frequency on the SYSCLK PLL */
/*************************************************************************************/

#if (SDRAM_SIZE_MBYTE == 2)
    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF0, 0x02);     /* CAS Latency = 2, 16-bit wide, size = 2 MB */
#else
    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF0, 0x03);     /* CAS Latency = 2, 16-bit wide, size = 4 MB */
#endif
    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF1, 0x01);     /* SDRAM Configuration
                                                        TRFC (00b) - Auto Refresh command period = 7 SYSCLKs
                                                        TWR  (0b)  - Write Recovery              = 2 SYSCLKs
                                                        TRAS (01b) - Active to Precharge command = 5 SYSCLKs
                                                    */
    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF4, 0x00);     /* SDRAM Configuration
                                                        TRP  (0b)  - Precharge command           = 2 SYSCLKs
                                                        TRCD (0b)  - Active to Read/Write Delay  = 1 SYSCLKs
                                                        NOVL (0b)  - Overlap Move enabled
                                                        BUS32 (0b) - 16 bit bus
                                                    */

    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF3, 0x80);     /* Initialize SDRAM CLOCK */
    for (i = 0; i < 200; i++)                       /* Delay 200 instructions */
    {
    }

    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF2, 0x00);     /* SDRAM Configuration REFINTV - Refresh Disabled */

    WRITE_REG(HAL_SDRAMREG_MAP->SDMODE, 0x01);      /* Precharge All Banks */
    WRITE_REG(HAL_SDRAMREG_MAP->SDMODE, 0x00);      /* Auto Refresh */
    WRITE_REG(HAL_SDRAMREG_MAP->SDMODE, 0x00);      /* Auto Refresh */
    WRITE_REG(HAL_SDRAMREG_MAP->SDMODE, 0x02);      /* Load Mode Register */

    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF2, EXTERNALSDRAM_REFINTV); /* SDRAM Configuration
                                                    REFINTV  Refresh rate=  (SDRAM_REFRESH * (16 * SYSCLK)) */
    WRITE_REG(HAL_SDRAMREG_MAP->SDDELAY0, (3 << HAL_SDRAM_SDLCHDLY_LSB | 1));
    WRITE_FIELD(HAL_SDRAM_SDINCKSEL, 3);
    WRITE_REG(HAL_SDRAMREG_MAP->SDCONF3, 0x80);     /* Initialize SDRAM CLOCK */
}

/*---------------------------------------------------------------------------
    Boot code BT_InitializeIO
----------------------------------------------------------------------------*/
void BT_InitializeIO(void)
{
    WRITE_REG(HAL_PERIPHREG_MAP->PADATA,0x00);      /* 0000,0000 */
    WRITE_REG(HAL_PERIPHREG_MAP->PACTRL,0x5D);      /* 1: input, 0:output */
    WRITE_FIELD(HAL_AFE_PDFVREF,1);                 /* set power down FVREF  */

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
  #if (SET_SLOT_IN == 1) //[BTC][Q36] for InitializeIO
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x2A);	  /* 0010, 1010 */ 
  #else
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x08);
  #endif
#else
    WRITE_REG(HAL_PERIPHREG_MAP->PBDATA,0x00);      /* 0000, 1000 */
#endif
    WRITE_REG(HAL_PERIPHREG_MAP->PBCTRL,0x64);      /* 1: input, 0:output */

    WRITE_REG(HAL_GLOBALREG_MAP->PAMUX0, 0x5C);  // EINTIEN, RX, Alpha SIN, Alpha COS
    WRITE_REG(HAL_GLOBALREG_MAP->PBMUX0, 0x00);      /* Default the MIOs should be inputs to reduce IO noise */
    WRITE_REG(HAL_GLOBALREG_MAP->PBMUX1, 0x04);  // Use GPIOs as default I/O pins
}

/*---------------------------------------------------------------------------
    Boot code BT_init_HIF
----------------------------------------------------------------------------*/
void BT_init_HIF()
{
    WRITE_REG(HAL_HSTREG_MAP->UMISCR, 0x00);

    WRITE_REG(HAL_CDDREG_MAP->MEMCF, 0x02);/* Memory layout 3K */

    WRITE_REG(HAL_HSTREG_MAP->IFCTRL, HAL_HST_DOUTEN);
    WRITE_REG(HAL_HSTREG_MAP->CTRLTR, 0x00);
    WRITE_REG(HAL_HSTREG_MAP->TSKCMP0, HAL_HST_ADSC);
    WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
    WRITE_REG(HAL_HSTREG_MAP->ATCMD, 0x00);
    WRITE_REG(HAL_HSTREG_MAP->HOPT, (HAL_HST_AUTOA0|HAL_HST_AUTOAC|HAL_HST_AUTODC));
//    Bt_shHDDir = HAL_HST_HBODIS;           /* 3K Host Layout*/

    /*Automatic change from 2.5K to 3k page for C2PO*/
    Bt_shHDDir = HAL_HST_HMLY | HAL_HST_HBODIS;   /* 2.5K Host Layout*/


    Bt_shHDDir |= HAL_HST_BUFPGSEL;

    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);      /* 24-bit addr, PIO xfr type */


    WRITE_REG(HAL_HSTREG_MAP->PFIFOCS, 0x00);

   // WRITE_REG(HAL_HSTREG_MAP->HDDIR, HAL_HST_BUFPGSEL);  /* 24-biy addr, PIO xfr type */

    WRITE_REG(HAL_HSTREG_MAP->HICTL, 0x00);

    WRITE_REG(HAL_HSTREG_MAP->HSTBUFSPGL, 0x00);             /* host start blk. */
    WRITE_REG(HAL_CDDREG_MAP->DSKBUFSPGL, 0x00);             /* ring start blk. */
    WRITE_REG(HAL_CPYCMPREG_MAP->CPYSPGL, 0x00);             /* Copy Buffer start Page */

    WRITE_REG(HAL_HSTREG_MAP->HSTBUFEPGL, RING_END_PAGE_2p5K);      /* host End blk. */
    WRITE_REG(HAL_CDDREG_MAP->DSKBUFEPGL, RING_END_PAGE_2p5K);    /* ring End blk. */
    WRITE_REG(HAL_CPYCMPREG_MAP->CPYEPGL, RING_END_PAGE_2p5K);    /* Copy Buffer End Page */

    WRITE_REG(HAL_HSTREG_MAP->HINTEN0, HAL_HST_SRSTEN | HAL_HST_ARSTEN | HAL_HST_DTEEN | HAL_HST_PCMDEN | HAL_HST_CMDEN );
    WRITE_REG(HAL_HSTREG_MAP->HINT0, 0);

    WRITE_REG(HAL_HSTREG_MAP->HICTL, HAL_HST_CLRBSY);

    WRITE_REG(HAL_HSTREG_MAP->ATINT,0x01);       /* Interrupt reason reg */
    WRITE_REG(HAL_HSTREG_MAP->ATSPA, 0x01);       /* sector number reg */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);   /* BYTE count low / high reg */

    WRITE_REG(HAL_HSTREG_MAP->ATDRS, 0x00);     /* drive select reg */
    WRITE_REG(HAL_HSTREG_MAP->CTRLS,(HAL_HST_SBSYCTL |HAL_HST_SCHKC | HAL_HST_SABRTC));/* Set Shadow */
    WRITE_REG(HAL_HSTREG_MAP->CTRLTR,0x00);
    WRITE_REG(HAL_HSTREG_MAP->ATFEA, HAL_HST_ILIW);
    WRITE_REG(HAL_HSTREG_MAP->ATCMD, 0x00);

    WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x08);

   // WRITE_REG(HAL_HSTREG_MAP->HDDIR, 0xE3);

    WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);

}
#if (SEND_BOOT_DEBUG_MSG == 1)
/************************************************************************
* Module: init_ssi()
* Description:  Initialize SSI interface
* Returns:
* Params:
*************************************************************************/
void BT_init_ssi(void)
{
    /* Initialize SSI interface */

    /* Baud rate setting */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSICLKDIV,1);                    /* Clock divider */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSICFG0,HAL_APB_SSI_SSCLKINV);   /* Configure communications Format Invert clock on SSI Interface */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSIINTEN,0);                     /* disable all the interupts */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSIINT,0);                       /* Serial Channel Interrupt Clear */

#if (CHIP_REV >= 0xC0)
    #if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SMODE2, 0);
    #elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SMODE2, 1);
    #endif
#endif
}
#endif /* DEBUG_PORT_OUTPUT_MODE==SSI_INTERFACE */
/*---------------------------------------------------------------------------
    Boot code BT_InitializeInterrupts
----------------------------------------------------------------------------*/
void BT_InitializeInterrupts(void)
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


/*---------------------------------------------------------------------------
    Boot code BT_UpdateFirmwareCommand
----------------------------------------------------------------------------*/
void BT_SaveCurrentTransferSettings()
{
    BYTE i ;


    for ( i = 0; i < TRAN_MODE_INFO_LEN; i++ )
    {
        BT_tranData[i] = 0;
    }

    /* mark Flash Up execution */
    BT_tranData[0] = 'F';
    BT_tranData[1] = 'l';
    BT_tranData[2] = 'a';
    BT_tranData[3] = 's';
    BT_tranData[4] = 'h';
    BT_tranData[5] = 'U';
    BT_tranData[6] = 0;

    /* makeing save data : Flag & Variable setting */
    BT_tranData[7]  = 1;                      /* 1=DMA feature enabled, 0=DMA feature disabled */
    BT_tranData[8]  = 1;                        /* 1=multiple_word DMA, 0=single_word DMA        */
    BT_tranData[9]  = 1;                        /* 1= ultra DMA mode                             */
    BT_tranData[10] = 0x00;      /* Maximum blocks to xfr due to chip limitation  */
    BT_tranData[11] = 0x1F;

    /* makeing save data : Identify table setting */
    Bt_Buf_dram_addr = IDENTIFY_ADR + (62 * 2 + 1);
    BT_tranData[12] = BT_ReadSDRAM();                           /* Single Word DMA Transfer          */
    Bt_Buf_dram_addr = IDENTIFY_ADR + (63 * 2 + 1);
    BT_tranData[13] = BT_ReadSDRAM();                           /* Multi Word DMA Transfer           */
    Bt_Buf_dram_addr = IDENTIFY_ADR + (88 * 2 + 1);
    BT_tranData[14] = BT_ReadSDRAM();                           /* ENTER UDMA mode are not supported */
    /* save data into DRAM */
    Bt_Buf_dram_addr = TRAN_MODE_INFO_ADR;
    for ( i = 0; i < TRAN_MODE_INFO_LEN; i++ )
    {
        BT_WriteSDRAM(BT_tranData[i]);
    }
}

/*---------------------------------------------------------------------------
    Boot code BT_GetFlashUpdateFromHost
----------------------------------------------------------------------------*/
vBOOL BT_GetFlashUpdateFromHost( USHORT T_page_count )     // for receive with page mode
{

    vBOOL tmp;

    Bt_shATB = T_page_count * 0x800;
    Bt_shHstWdC = 0x400;
    Bt_shHstBytAC = 0;

    /* Set the start page */
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, (Bt_Linear_Xfer_addr / 0x800));

    /* Set Buffer Compare Page */
    WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, (Bt_Linear_Xfer_addr / 0x800));

    /* Set the offset bytes of page */
    WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, (ULONG)Bt_shHstBytAC);

    /* Set the USHORT counter */
    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, (USHORT)Bt_shHstWdC);

#if (CHIP_REV >= 0xC0)
    WRITE_FIELD(HAL_HST_RESWDCL,0x00);
    WRITE_FIELD(HAL_HST_RESWDCH,0x00);
    WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif
    /* Set the Host Trasfer Direction Register */
    Bt_shHDDir &= (~HAL_HST_HBODIS);     /* Select page/offset mode */
    Bt_shHDDir |= HAL_HST_HXDIR;         /* Set Direction: host -> device */
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Bt_shHDDir);

    //first transfer, we have to setup page count register
    WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, T_page_count);
    WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, T_page_count);

    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);    /* START_XFER */

	do
	{
		tmp = READ_REG(HAL_HSTREG_MAP->HINT0);
	} while ((tmp & HAL_HST_DTE) == 0);
    WRITE_FIELD(HAL_HST_DTE,0);

    return TRUE;
}

/*---------------------------------------------------------------------------
    Boot code BT_TransferNotificationData
----------------------------------------------------------------------------*/
void BT_ClearFlashUpWorkAreaAll( void )
{
    ULONG   addr;

    for ( addr = 0L; addr < FLASH_ROM_SIZE; addr++ )
    {
        SCSDRAM[addr] = 0xFF;
    }

}

/*---------------------------------------------------------------------------
    Boot code BT_UpdateFirmwareCommand
----------------------------------------------------------------------------*/
static void BT_UpdateFirmwareCommand(void)
{
    USHORT  i;
    USHORT  cnt;


    /* Transfer Indicator */
    switch ( BT_Cmd_block[5] )
    {
        case 0x00:                                  /* Start */
            BT_ClearFlashUpWorkAreaAll();
        case 0xFF:                                  /* Middle */
            cnt = 60;                               /* 60 * 1024Bytes = 60KBytes */
            break;

        default:
            cnt = BT_Cmd_block[5];
            break;
    }

    switch ( BT_Cmd_block[5] )
    {
        case 0x00:                                  /* Start */
            Bt_Buf_hxfer_blk = 1;
            Bt_Linear_Xfer_addr = 0L;
            break;
        default:                                    /* Middle or End */
            Bt_Linear_Xfer_addr = Bt_FlashLinearAdress;
            break;
    }

    //opReadyLED = (~opReadyLED);

    i = cnt / 2;
    if ( ! BT_GetFlashUpdateFromHost( i ) )
    {
        BT_BUILD_SENSE(0x04,0x44,0x00);           /* internal logical unit failure */
        return;
    }
    Bt_Linear_Xfer_addr += (cnt * 1024);          /* Update address */

    //opReadyLED = (~opReadyLED);

    switch ( BT_Cmd_block[5] )
     {
        case 0x00:      /* Start  */
            //vWakeUpFromPowerSaveMode();
            break;
        case 0xFF:      /* Middle */
            break;
        default:        /* End    */

            send_msgB1('U','P','D','A');
            BT_BUILD_SENSE(0x00,0x00,0x00);            // Clear Sense Bytes

            BT_SaveCurrentTransferSettings();

            FlashWriteFW();
            break;
    }

    Bt_FlashLinearAdress = Bt_Linear_Xfer_addr;
}

/*---------------------------------------------------------------------------
    Boot code BT_WriteBufferCommand
----------------------------------------------------------------------------*/
/**********************************************/
/* define for ATAPI read/write buffer command */
/**********************************************/
#define BUFFER_MODE_HDR_DATA                            0x00
#define BUFFER_VENDOR_MODE                              0x01
#define BUFFER_MODE_DATA			        0x02
#define	BUFFER_MODE_DESCRIPTOR	                        0x03
#define BUFFER_MODE_DOWNLOAD_FW_AND_SAVE                0x05

#define BUFFER_ID_0					0x00
#define BUFFER_ID_1					0x01

#define CONTROL_NORMAL                                  0x00
#define CONTROL_WRITE_FIRMWARE                          0x80
#define CONTROL_WRITE_FIRMWARE_AND_OVERWRITE_BOOTCODE   0xA0

#define RDBUFFER_LEN_BYTE			        0x04

void BT_WriteBufferCommand()
{
    ULONG StartAddress, TransferLength;

    StartAddress = (((ULONG)BT_Cmd_block[3] << 16) + ((ULONG)BT_Cmd_block[4] <<  8) + ((ULONG)BT_Cmd_block[5]));     // Get Start Address
    StartAddress &= 0xFFFFFFFE;                   // Force start to be multiple number of words  (i.e. mask Least Significant bit)

    TransferLength = (((ULONG)BT_Cmd_block[6] << 16) + ((ULONG)BT_Cmd_block[7] <<  8) + ((ULONG)BT_Cmd_block[8]));   // Get length
    TransferLength &= 0xFFFFFFFE;                   // Force length to be multiple number of words  (i.e. mask Least Significant bit)

    if ((TransferLength == 0x00) && ((BT_Cmd_block[9] & 0x80) != 0x80))
    {
        BT_AutoCommandComplete();   // Just return. Length is zero.
        return;
    }

    switch (BT_Cmd_block[1]) //Mode
    {
        case BUFFER_MODE_DATA:
        case BUFFER_MODE_DOWNLOAD_FW_AND_SAVE:
            break;

        default:
            BT_BUILD_SENSE(0x05,0x24,0x00);       // Fail. Invalid mode.
            BT_AutoCommandComplete();
            return;
    }

    switch (BT_Cmd_block[2]) //BufferID
    {
        case BUFFER_ID_0:
        case BUFFER_ID_1:
            break;

        default:
            BT_BUILD_SENSE(0x05,0x24,0x00);   // Fail. Invalid Buffer ID.
            BT_AutoCommandComplete();
            return;
    }

    if ( StartAddress > ( (SYS_START_PAGE + 1) * 0x10000) )
    {
        BT_BUILD_SENSE(0x05,0x24,0x00);
        BT_AutoCommandComplete();
        return;
    }

    // fail if start + length extends beyond phsyical buffer.
    if ( (StartAddress + TransferLength) > ((SYS_START_PAGE  + 1) * 0x10000) )
    {
        BT_BUILD_SENSE(0x05,0x24,0x00);
        BT_AutoCommandComplete();
        return;
    }

    switch (BT_Cmd_block[9]) //Control Byte
    {
        case CONTROL_NORMAL:
            BT_GetParameterDataFromHost(StartAddress, TransferLength);  // This functions transfers data from Host to Buffer.
            break;


        default:
            BT_BUILD_SENSE(0x05,0x24,0x00);
            BT_AutoCommandComplete();
            return;
    }

    BT_AutoCommandComplete();
    return;
}

void BT_ReadBufferCommand()
{
    ULONG AllocationLength, StartAddress;
    BYTE * BufferP;

    StartAddress = (((ULONG)BT_Cmd_block[3] << 16) + ((ULONG)BT_Cmd_block[4] <<  8) + ((ULONG)BT_Cmd_block[5]));     // Get Start Address
    StartAddress &= 0xFFFFFFFE;                   // Force start to be multiple number of words  (i.e. mask Least Significant bit)

    AllocationLength = (((ULONG)BT_Cmd_block[6] << 16) + ((ULONG)BT_Cmd_block[7] <<  8) + ((ULONG)BT_Cmd_block[8]));   // Get length
    AllocationLength &= 0xFFFFFE;                   // Force length to be multiple number of words  (i.e. mask Least Significant bit)

    if ( AllocationLength == 0x00 )
    {
        BT_AutoCommandComplete();       // Just return if length is zero.
        return;
    }

    switch (BT_Cmd_block[1]) //Mode
    {
        case BUFFER_VENDOR_MODE:
        case BUFFER_MODE_DATA:
        case BUFFER_MODE_DOWNLOAD_FW_AND_SAVE:
            break;

        case BUFFER_MODE_DESCRIPTOR:
            // Build descriptor image in DRAM
            BufferP = (BYTE *)(BUFFER_SCRATCH + SCSDRAM_START_ADDR);   // Point to Scratch area in the buffer to
                                                //   assemble data returned to host.
            if ( BT_Cmd_block[2] == BUFFER_ID_0 )
            {
                // valid buffer ID
                *BufferP++ = (0x01);                //  Byte 0:  Offset boundary:  even bytes
                *BufferP++ = (SYS_START_PAGE + 1);  //  Byte 1:  Buffer capacity  MSB
                *BufferP++ = (0x00);                //  Byte 2:  Buffer capacity  MID
                *BufferP   = (0x00);                //  Byte 3:  Buffer capacity  LSB
            }
            else
            {   // invalid buffer ID:  pass all 00's
                *BufferP++ = (0x00);                //  Byte 0
                *BufferP++ = (0x00);                //  Byte 1
                *BufferP++ = (0x00);                //  Byte 2
                *BufferP   = (0x00);                //  Byte 3
            }

            // Send descriptor image to host */
            BT_SendParameterDataToHost(BUFFER_SCRATCH,MINIMUM( RDBUFFER_LEN_BYTE , AllocationLength) , TRUE);  // Transfer data and do Command Complete.
            return;

        default:
            BT_BUILD_SENSE(0x05,0x24,0x00);       // Fail. Invalid mode.
            BT_AutoCommandComplete();
            return;
    }

    switch (BT_Cmd_block[2]) //BufferID
    {
        case BUFFER_ID_0:
        case BUFFER_ID_1:
            break;

        default:
            BT_BUILD_SENSE(0x05,0x24,0x00);   // Fail. Invalid Buffer ID.
            BT_AutoCommandComplete();
            return;
    }

    if ( StartAddress > ( (SYS_START_PAGE + 1) * 0x10000) )
    {
        BT_BUILD_SENSE(0x05,0x24,0x00);    // Fail. Start Address too big
        BT_AutoCommandComplete();
        return;
    }

    /* Change length if start + length extends beyond phsyical buffer */
    if ( (StartAddress + AllocationLength) > ((SYS_START_PAGE + 1) * 0x10000) )
    {
        AllocationLength = ( ( (SYS_START_PAGE + 1) * 0x10000) - StartAddress );
    }
    // Break down transfer if sending more than 8KBytes data to host
    Bt_shATB = AllocationLength;
    BT_SendParameterDataToHost(StartAddress,AllocationLength , TRUE);               // Send data to host and send Command Complete to host.
    return;
}

/*********************************************************************
    BT_CheckCommandAvail
*********************************************************************/
void BT_CheckCommandAvail (void)
{


    if( BT_cmd_avail == FALSE)
        return;

    send_msgB1('C', 'k', 'C', 'D');
    send_msgB2();
    switch (BT_Cmd_block[0])
    {
        case ATA_SET_FEATURE:
            BT_ataSetFeatureCommand();
            break;

        case FLASH_CODE:
            BT_UpdateFirmwareCommand();
            BT_AutoCommandComplete();
            break;

        case SPC_INQUIRY:
            BT_InquiryCommand();
            break;

        case SPC_REQUEST_SENSE:
            BT_RequestSenseCommand();
            break;

        case SPC_MODE_SENSE:
            BT_ModeSenseCommand();
            break;

        case MMC_GET_EVENT_STATUS_NOTIFICATION:
            BT_GetEventStatusNotificationCommand();
            break;

        case SPC_TEST_UNIT_READY:
            BT_BUILD_SENSE(0x00,0x00,0x00);
            BT_AutoCommandComplete();
            break;

        case SPC_WRITE_BUF:
            BT_WriteBufferCommand();
            break;

        case SPC_READ_BUF:
            BT_ReadBufferCommand();
            break;

        //start == Windows Vista 32 cannot find the drive after place the drive into boot mode.
        case 0xF5:
            BT_ATACommandFail();
            break;
        //end ==

        default:
            BT_BUILD_SENSE(0x05,0x20,0x00); /* Illegal Request */
            BT_AutoCommandComplete();
            break;

    }
}

BYTE BT_CheckSignature(void)
{
    vBYTE *SigStringPtr;
    BYTE rdTmp, i;

    send_msgB1('C', 'K', 'S', 'T');

    SigStringPtr = (vBYTE *)(FLASH_BASE + 0x20000L); //start of code
    for(i=0; i<16; i++)
    {
        rdTmp = *SigStringPtr;
        if(	SigStringSOF[i] != rdTmp)
            return FALSE;

        SigStringPtr++;
    }

    SigStringPtr = (vBYTE *)(FLASH_BASE + FLASH_ROM_SIZE - 0x10); //end of code
    for(i=0; i<14; i++)
    {
        rdTmp = *SigStringPtr;
        if(	SigStringEOF[i] != rdTmp)
            return FALSE;

        SigStringPtr++;
    }


    return TRUE;
}

void BT_Init_TBL(void)
{
    USHORT i;
    vBYTE const *code_ptr;

    /* Initial Identify Table*/
    code_ptr = Bt_Identify_tbl;			/* initialize code pointer */
    Bt_Buf_dram_addr = IDENTIFY_ADR;

    for(i = 0;i < IDENTIFY_LEN_BYTE;i++,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }

    /* Initial Inqry Table*/
    code_ptr = Bt_Inquiry_tbl;
    Bt_Buf_dram_addr = INQUIRY_ADR;

    for(i=0;i<INQUIRY_LEN_BYTE;i++,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }

    /* Initial Request Sense Table*/
    code_ptr = Bt_Request_sense_tbl;
    Bt_Buf_dram_addr = REQ_SENSE_ADR;

    for(i=0;i<REQ_SENSE_LEN_BYTE;i++,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }

    /* Initial GESNTable*/
    code_ptr = Bt_GESN_nea_tbl;        /* set top address */
    Bt_Buf_dram_addr = GESN_NEA_ADR;

    for( i = 0; i < GESN_NEA_LEN; i++ ,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }

    code_ptr = Bt_GESN_external_tbl;        /* set top address */
    Bt_Buf_dram_addr = GESN_EXTERNAL_ADR;

    for( i = 0; i < GESN_EXTERNAL_LEN; i++ ,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }

    code_ptr = Bt_GESN_media_tbl;        /* set top address */
    Bt_Buf_dram_addr = GESN_MEDIA_ADR;

    for( i = 0; i < GESN_MEDIA_LEN; i++ ,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }
    code_ptr = Bt_GESN_DeviceBusy_tbl;        /* set top address */
    Bt_Buf_dram_addr = GESN_DEVICEBUSY_ADR;

    for( i = 0; i < GESN_DEVICEBUSY_LEN; i++ ,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }

    code_ptr = Bt_GESN_OpeChgReq_tbl;        /* set top address */
    Bt_Buf_dram_addr = GESN_OPECHGREQ_ADR;

    for( i = 0; i < GESN_OPECHGREQ_LEN; i++ ,code_ptr++)
    {
        BT_WriteSDRAM(*code_ptr);
    }
}


void BT_Timer0Isr(void)
{
    BT_Iram_system_timer++;

    /*Auto Fis34 Hot Plug Work Around!*/
    /*This WA is for ICH5, after PHY Ready, but COMRESET is not issued case.*/
    #if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))
    if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
    {
        if (BT_Iram_system_timer % 0x1000 == 0)
            send_msgB1('a','a','a',BT_SendFis34Already);
        if(BT_SendFis34Already == 0)
        {
            /*PHY Ready and after 50ms manual issue FIS34*/
            if(BT_Iram_system_timer >= BT_BGFis34Timer + 50) //PHY Ready Over 50msec
            {
                //ATAClearBusy();
                WRITE_REG(HAL_HSTREG_MAP->HICTL, HAL_HST_CLRBSY);
                WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
                WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
                WRITE_FIELD(HAL_HST_AFIS34DIS, 1);
                BT_SendFis34Already = 1;
                send_msgB1('B','G','F','1');

            }
            else if(BT_Iram_system_timer >= (~BT_BGFis34Timer + 50))
            {
                //ATAClearBusy();
                WRITE_REG(HAL_HSTREG_MAP->HICTL, HAL_HST_CLRBSY);
                WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
                WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
                WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
                WRITE_FIELD(HAL_HST_AFIS34DIS, 1);
                BT_SendFis34Already = 1;
                send_msgB1('B','G','F','2');
            }
        }
    }
    else
    {
        if (BT_Iram_system_timer % 0x1000 == 0)
            send_msgB1('b','b','b',BT_SendFis34Already);
        BT_BGFis34Timer = BT_Iram_system_timer; //PHY Not Ready, Reset Timer.
        if(BT_SendFis34Already == 1)
        {
            BT_SendFis34Already = 0; //Reset Flag
            WRITE_FIELD(HAL_HST_AFIS34DIS, 0);
            send_msgB1('d','i','s','c');
        }
    }
    #endif
}
/*---------------------------------------------------------------------------
    Boot code main function
----------------------------------------------------------------------------*/
void BootMain(void)
{
    vULONG  Counter = 0;
    if(BT_CheckSignature() == TRUE)
    {
        return;
    }

#if (SEND_BOOT_DEBUG_MSG == 1)
    BT_init_ssi();
#endif
    BT_InitializeInterrupts();
    BT_init_HIF();

    WRITE_REG(HAL_AHB_SFLASHREG_MAP->SFCTRL1, 0x00);    /* Enable SFlash Interface */
    WRITE_FIELD(HAL_AHB_SFLASH_SFCLKSEL,3);             /* Select clock frequency for the Serial Flash Interface. */
    WRITE_REG(HAL_AHB_SFLASHREG_MAP->SFCTRL2, HAL_AHB_SFLASH_SFTAGRST);    /* Reset Cache tags */

    WRITE_FIELD(HAL_AHB_SFLASH_SFENCACH, 1);            /*  bSFCHTEST - Cache Test mode disabled */
                                                        /*  bSFSLOWRD - Slow Read Disabled */
                                                        /*  bSFTAGRST - Do not reset Cache tags */
                                                        /*  bSF2OUT   - Dual Output SPI Flash not supported */
                                                        /*  bSF8KCACH - Select 4K Cache */
                                                        /*# bSFENCACH - Serial Cache enabled */
    WRITE_FIELD(HAL_AHB_SFLASH_SFTAGRST, 0);            /* Clear Cache Tag reset bit */

    WRITE_FIELD(HAL_AHB_INTCTRL_ITEN, 1);

    send_msgB1('B', 'o', 'o', 't');
    BT_cmd_avail = FALSE;
    Bt_Linear_Xfer_addr = 0;
    Bt_FlashLinearAdress = 0;
#if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))
    BT_SendFis34Already = 1; // power on default set to 1
#endif
    BT_Init_TBL();
    /* while in the bootcode mode, the LED will always ON */

	//#if(SUN_FUNC_TEST == 0)
      //opReadyLED = LED_ON;
      //#else
      //HAL_DSP_LED(LED_ON);
      //#endif

    for(;;)
    {
        Counter++;
        BT_HostIRQService();
        BT_CheckCommandAvail();
        if ((Counter % 0x20) == 0)
            BT_Timer0Isr();
    }
}
#endif



