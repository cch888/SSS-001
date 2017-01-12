/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
* $Revision: 85 $
* $Date: 11/02/25 11:40a $
*
* DESCRIPTION
*   This file contains buffer related equates
*
************************ SUNEXT CONFIDENTIAL *******************************
*
*     < Reference SDRAM Mapping >
*           2MB
*
*     CD-R/RW Format
*     00000h|--------------------------| ----------
*           | Page  0             800h | "Paging Area (Data)"
*     00800h|--------------------------|
*           | Page  1               :  |
*     01000h|--------------------------|
*           | Page  2               :  |
*     01800h|--------------------------|
*           | :                     :  |
*           |--------------------------|
*           | :                     :  |
*     15F800|--------------------------|
*           | Page  2BF             :  |
*     160000|--------------------------| ----------
*           | Page  0             200h | "Paging Area (Aux)"
*     100200|--------------------------|
*           | Page  1               :  |
*     100400|--------------------------|
*           | Page  2               :  |
*     100600|--------------------------|
*           | :                     :  |
*           |--------------------------|
*           | :                     :  |
*     1B7800|--------------------------|
*           | Page  2BF             :  |
*     1B8000|--------------------------| ----------
*           | Command Pack       1000h | 2 pages
*     1B9000|--------------------------| ----------
*           | Link Area Pages    6000h | 40 pages
*           | (ROUTPG=372h)            |
*     1CD000|--------------------------| ----------
*           | Unused Area              |
*           | 21 Pages                 |
*           |                          |
*     1D7800|--------------------------|
*
*
*
*     CD-MRW Format
*     00000h|--------------------------| ----------
*           | Page  0             800h | "Paging Area (Data)"
*     00800h|--------------------------|
*           | Page  1               :  |
*     01000h|--------------------------|
*           | Page  2               :  |
*     01800h|--------------------------|
*           | :                     :  |
*           | :                     :  |
*           | :                     :  |
*     0DF800|--------------------------|
*           | Page  1BF             :  |
*     0E0000|--------------------------| ----------
*           | Page  0             200h | "Paging Area (Aux)"
*     0E0200|--------------------------|
*           | Page  1               :  |
*     0E0400|--------------------------|
*           | Page  2               :  |
*     0E0600|--------------------------|
*           | :                     :  |
*           |--------------------------|
*           | :                     :  |
*     13F800|--------------------------|
*           | Page  1BF             :  |
*     118000|--------------------------| ----------
*           | Command Pack        1000h| 2 pages
*     119000|--------------------------| ----------
*           | Link Area Pages     6000h| 40 pages
*           | (ROUTPG=382h)            |
*     12D000|--------------------------| ----------
*           | CD-MRW Area              |
*           | 341 Pages                |
*           |                          |
*           |                          |
*           |                          |
*     1D7800|--------------------------|
*
*
*
*     DVD Format
*     00000h|--------------------------| ----------
*           | Page  0             800h | "Paging Area (Data)"
*     00800h|--------------------------|
*           | Page  1               :  |
*     01000h|--------------------------|
*           | Page  2               :  |
*     01800h|--------------------------|
*           | :                     :  |
*           |--------------------------|
*           | :                     :  |
*     10F800|--------------------------|
*           | Page  21F             :  |
*     110000|--------------------------| ----------
*           | Page  0              10h | "Paging Area (Aux)"
*     150010|--------------------------|  16 Bytes/Sector
*           | Page  1               :  |
*     150020|--------------------------|
*           | Page  2               :  |
*     150030|--------------------------|
*           | :                     :  |
*           |--------------------------|
*           | :                     :  |
*     1121F0|--------------------------|
*           | Page  21F             :  |
*     112200|--------------------------| ----------
*           | Unused Area         600h |
*           |                          |
*     112800|--------------------------| ----------
*           | 122 Pages                | DVD PI/PO Area
*           |                          |
*           |                          |
*           |                          |
*     14F800|--------------------------| ----------
*           | 19 Pages                 | Encode Segment A
*           |                          |
*           |                          |
*     159000|--------------------------| ----------
*           | 19 Pages                 | Encode Segment B
*           |                          |
*           |                          |
*     162800|--------------------------| ----------
*           | 4 Pages                  | Write Seed Block 1
*           |                          |
*     164800|--------------------------| ----------
*           | 4 Pages                  | Write Seed Block 2
*           |                          |
*     166800|--------------------------| ----------
*           | 2 Pages                  | DVD CSS Working Area
*     167800|--------------------------| ----------
*           | 224 Pages                | DVD-R/RW, DVD+R/RW, DVD-RAM
*           |                          | Work Area
*           |                          |
*     1D7800|--------------------------| ----------
*           |                          | CD/DVD Common Work Area
*           |                          |
*
*
*
*
*     CD/DVD Common Work Area
*     1D7800|--------------------------| ----------
*           | Flash2 working area 1000h| 2 pages
*     1D8800|--------------------------| ----------
*           | PERSISTENT Data     800h | 1 pages
*     1D9000|--------------------------| ----------
*           | Write Strategy Table     | 16 pages
*           |                    8000h |
*     1E1000|--------------------------| ----------
*           |                          | 16 pages
*           |                    8000h | Unused Area
*     1E9800|--------------------------| ----------
*           | SDRAM Variables    6800h | 13 Pages
*           |                          |
*           |                          |
*           |                          |
*           |                          |
*           |                          |
*     1F1000|--------------------------| ----------
*           | Player DB          3670h |
*           |                          |
*     1F4670|--------------------------| ----------
*           | PreGap Start Time   190  |
*     1F4800|--------------------------| ----------
*           | PMA                 460h | Shadow DB
*           | ATIP                 20h |
*           | PreGap Start Time   190h |
*           | TOC Format0 Data    3F0h |
*           | TOC Format1 Data     10h |
*           | TOC Format2 Data    C00h |
*           | Player DB          3670h |
*           |                          |
*           | Total              4C80h |
*     1F9480|--------------------------| ----------
*           |                     300h | Gap
*     1F9780|--------------------------| ----------
*           | HIF Work RAM         80h |
*     1F9800|--------------------------|
*           | Pages Data Mode     400h |
*     1F9C00|--------------------------|
*           | Host TX Work Buffer C00h |
*     1FA800|--------------------------|
*           | Command Pack REPEAT 100h |
*     1FA900|--------------------------|
*           | Send Cue_Sheet Data 680h |
*     1FAF80|--------------------------|
*           | Host Command Data  4870h |
*     1FF7F0|--------------------------|
*           | Transfer mode Info   10h |
*     1FF800|--------------------------|
*           | Diag Working Area   800h |
*     1FFFFF|--------------------------|----------
*
*
*
*
*     <Host Command Data Detail>
*     1FAF80|------------------------------|
*           | Set Streaming Data       (32)|
*     1FAFA0|------------------------------|
*           | Read FMT Capacity        (64)|
*     1FAFE0|------------------------------| ----------
*           |                          (32)| Unused Area
*     1FB000|------------------------------| ----------
*           | Get Perform Data        (256)|
*     1FB100|------------------------------|
*           | Get Config Data         (512)|
*     1FB300|------------------------------|
*           | Capacity data            (16)|
*     1FB210|------------------------------|
*           | Identify data           (512)|
*     1FB410|------------------------------|
*           | Inquiry data             (80)|
*     1FB450|------------------------------|
*           | Read sub-q LBA data      (48)|
*     1FB480|------------------------------|
*           | Track Information data   (48)|
*     1FB4B0|------------------------------|
*           | Disc Information data    (96)|
*     1FB510|------------------------------|
*           | Send OPC Data           (128)|
*     1FB590|------------------------------|
*           | Request sense data       (32)|
*     1FB5B0|------------------------------|
*           | Buffer capacity data     (16)|
*     1FB5C0|------------------------------|
*           | MMC FORMAT command data  (16)|
*     1FB5D0|------------------------------| ----------
*           | Notification returned page(4)| "Get Event Status"
*     1FB5D4|------------------------------|
*           | Reserved Gap Page         (4)|
*     1FB5D8|------------------------------|
*           | Operational Page          (8)|
*     1FB5E0|------------------------------|
*           | Power Management page     (8)|
*     1FB5E8|------------------------------|
*           | Reserved Gap Page         (8)|
*     1FB5F0|------------------------------|
*           | Media status Page         (8)|
*     1FB5F8|------------------------------|
*           | Reserved Gap Page         (8)|
*     1FB600|------------------------------|
*           | Device Busy status Page   (8)|
*     1FB608|------------------------------|
*           | OPECHGREQ Page            (8)|
*     1FB610|------------------------------| ----------
*           |                         (496)| Unused Area
*     1FB800|------------------------------| ----------
*           | Mode Sense HDR            (8)| "Mode Sense"
*     1FB808|------------------------------|
*           | ErrorRecoveryPage(01)    (12)|
*     1FB814|------------------------------|
*           | WriteParametersPage(05)  (52)|
*     1FB848|------------------------------|
*           | Caching page(08)         (12)|
*     1FB854|------------------------------|
*           | CD-ROM Page(0D)           (8)|
*     1FB85C|------------------------------|
*           | Audio Control Page(0E)   (16)|
*     1FB86C|------------------------------|
*           | Power Condition page(1A) (12)|
*     1FB878|------------------------------|
*           | Time-out page(1D)        (10)|
*     1FB882|------------------------------|
*           | Capa./Mech. Page(2A)     (30)|
*     1FB8A0|------------------------------|
*           | MRW(2C)                   (8)|
*     1FB8A8|------------------------------|
*           | LS(31)                   (44)|
*     1FB8D4|------------------------------|
*           | LS(32)                   (70)|
*     1FB91A|------------------------------|
*           | Return all pages(3F)    (282)|
*     1FBA34|------------------------------| ----------
*           |                        (1340)| Unused Area
*     1FBF70|------------------------------| ----------
*           | TOC Format0 Data       (1008)|
*     1FC360|------------------------------|
*           | TOC Format1 Data         (16)|
*     1FC370|------------------------------|
*           | TOC Format2 Data       (3072)|
*     1FCF70|------------------------------|
*           | PMA ATIP               (1152)|
*     1FD3F0|------------------------------|
*           | CD-Text Data           (9216)|
*     1FF7F0|------------------------------|
*
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------ ---------------------------------------------
*    1.00   Aug-12-05   JesseK  Create
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __SDRAM_H__
#define __SDRAM_H__

#include ".\common\Vu_cfg.h"
#include ".\oem\Oem_info.h"

/*
;------------------------------------------------------------------------
; Logical memory layout                                                 |
;------------------------------------------------------------------------*/
 /****  System Information Area ****/
#define DRAM_PAGE_SIZE              0x800       /* DRAM Page Size  = 0x00800(= 2KB) */
#define MDSN_CACHE_SIZE            0x0800       /* Buffer size supported by drive = 2048M */

/*-----------------------------------------------------------------------
    2.5K DRAM PAGE SIZE DEFINE
-----------------------------------------------------------------------*/
#define DRAM_AUX_SIZE_2p5K          0x200       /* DRAM AUX Size = 0x200(= 0.5KB) */
#define RING_END_PAGE_2p5K          0x029F      /* last page the SP can access*/
#define BUF_TOTAL_BLK_COUNT_2p5K    0x2A0       /* total 672 pages */
#define DATA_START_ADDR_2p5K        0x000000    /* Data start address */
#define AUX_START_ADDR_2p5K         0x150000    /* Aux start address  */
#define AUX_HEADER_ADDR1_2p5K       0x09FC      /* M1 block header offset in page */
#define AUX_HEADER_ADDR2_2p5K       0x09F4      /* M2F1 block header offset in page */

/*-----------------------------------------------------------------------
    3K DRAM PAGE SIZE DEFINE
-----------------------------------------------------------------------*/
#define DRAM_AUX_SIZE_3K            0x400       /* DRAM AUX Size = 0x400(= 1KB) */
#define RING_END_PAGE_3K            0x21F       /* last page the SP can access*/
#define BUF_TOTAL_BLK_COUNT_3K      0x220       /* total 544 pages */
#define DATA_START_ADDR_3K          0x000000    /* Data start address */
#define AUX_START_ADDR_3K           0x110000    /* Aux start address  */
#define AUX_HEADER_ADDR1_3K         0x0BFC      /* M1 block header offset in page */
#define AUX_HEADER_ADDR2_3K         0x0BF4      /* M2F1 block header offset in page */

/*=======================================================================*/
#define CPACK_PAGE                  0x348
#define CPACK_DRAM_ADDR             (CPACK_PAGE*0x800)      /* command pack erea  */

#define ROUT_PAGE                   (CPACK_PAGE +2)         /* run-out start page */
#define ROUT_END_PAGE               (ROUT_PAGE + 40 - 1)    /* run-out end page   */
#define LINK_PAGE                   (ROUT_PAGE + 2)
#define ROUT_DRAM_ADDR              (ROUT_PAGE*0x800)

/***************************************************************/
/*                            Common area                      */
/***************************************************************/
#if (SDRAM_SIZE_MBYTE == 4)
/*--------------------------------------------------------------------------------------------*/
/*---------------------------------------- 4 MByte SDRAM -------------------------------------*/
/*------------------------------------ Work Information Area ---------------------------------*/
/*--------------------------------------------------------------------------------------------*/
#define SYS_START_PAGE          0x3F            /* syscon start page addr */
#define SYS_START_ADDR          0x3F0000        /* syscon start addr */

/* Flash DRAM Work Area Address Defines */
#define FLASH_DRAM_OFFSET       0x3D7800        /* DRAM start address (store SRAM) */
// Persistent Data Area
#define DRVPARA_DATA            0x3D8800        // Drive parameter area 0x1D8800 - 0x1D97FFF
#define STRATEGY_ADDRESS        0x3D9800        //Uncompressed Write Strategy Table

#if (ENABLE_AACS == 1)
#define MKBPARA_DATA            0x010000        // The flashed MKB data are always located at the address 0x10000
#endif
#else
/*--------------------------------------------------------------------------------------------*/
/*---------------------------------------- 2 MByte SDRAM -------------------------------------*/
/*------------------------------------ Work Information Area ---------------------------------*/
/*--------------------------------------------------------------------------------------------*/
#define SYS_START_PAGE          0x1F            /* syscon start page addr */
#define SYS_START_ADDR          0x1F0000        /* syscon start addr */

/* Flash DRAM Work Area Address Defines */
#define FLASH_DRAM_OFFSET       0x1D7800        /* DRAM start address (store SRAM) */

// Persistent Data Area
#if (SUN_FUNC_TEST == 0)
#define DRVPARA_DATA            0x1D8800        // Drive parameter area 0x1D8800 - 0x1D97FFF
#else
#define DRVPARA_DATA            0x1D9000        // Drive parameter area 0x1D8800 - 0x1D97FFF
#endif

#define STRATEGY_ADDRESS        0x1D9800        //Uncompressed Write Strategy Table

#if (ENABLE_AACS == 1)
#define MKBPARA_DATA            0x010000        // The flashed MKB data are always located at the address 0x10000
#endif
#endif //SDRAM_SIZE_MBYTE == 4

/***************************************************************/
/*        Shadow DB for Test write Backup and Restore          */
/***************************************************************/
#define SYS                     SYS_START_ADDR

/***************************************************************/
/*                          Player DB                          */
/***************************************************************/
#define DB_INFO_ADR             (ULONG)(SYS+0x1000)         /* 0x1F1000 - Player DB */
#define DB_INFO_LEN             0x3670

#define PREGAP_TIME_ADDR        (ULONG)(SYS+0x4670)         /* 0x1F4670 */
#define PREGAP_TIME_LEN         0x190                       /* (3+1)*99Track=0x18C */


/***************************************************************/
/*                          Shadow DB                          */
/***************************************************************/
#define SHADOW_DB_INFO_ADR      (ULONG)(SYS+0x4800) /* 0x1F4800 - For Test writing */
#define SHADOW_DB_INFO_LEN      0x4C80

#define MAXLBA_OFFSET           (ULONG)(SYS+0x9780) /* 0x1F9780 - System Variable addr */
#define AUDIO_END_HI            (ULONG)(SYS+0x9780) /* 0x1F9780 -  */
#define AUDIO_END_MIN           0x14
#define AUDIO_END_SEC           0x15
#define AUDIO_END_FRAME         0x16

#define TOC_DRAM_HI            (ULONG)(SYS+0x9780)  /* 0x1F9780 -  */
#define MAX_LEADOUT             0x19            /* TOC info saved */
#define FIRST_LEADOUT           0x1C            /* LO of session 1 data are saved(3 bytes) */
#define DISC_CAPACITY_LBA       0x17            /* offset from System Variable area */
#define NUM_OF_SESS             0x1F
#define MAX_TRACK               0x20

/***************************************************************/
/*                     Decorder Block Mode                     */
/***************************************************************/
#define DECBLKMASK_ADDR         (ULONG)(SYS+0x9800)         /* 0x1F9800 - Decoder Block Mask */
#define DECBLKMASK_LEN          0x400

/***************************************************************/
/*                       Command Scratch                       */
/***************************************************************/
#define BUFFER_SCRATCH          (ULONG)(SYS+0x9C00)         /* 0x1F9C00 - */

#define CPACK_REPEAT_ADR        (ULONG)(SYS+0xA800)         /* 0x1FA800 - repeat number of command pack */
#define CPACK_REPEAT_SIZE       0x100                       /* 256 bytes */

#define CUE_SHEET_ADR           (ULONG)(SYS+0xA900)         /* 0x1FA900 - */
#define CUE_SHEET_LEN           0x680                       /* ((99Tr + 99PreGap) + 2) * 8Byte = 0x640 */

/***************************************************************/
/*                      Host Command Data                      */
/***************************************************************/
#define STRDATA_ADR             (ULONG)(SYS+0xAF80)         /* 0x1FAF80 - */
#define STREAM_DATA_LEN         0x20

#define RDFMTCAP_ADR            (ULONG)(SYS+0xAFA0)         /* 0x1FAFA0 - */
#define RDFMTCAP_LEN            0x40

#define PERFORM_ADR             (ULONG)(SYS+0xB000)         /* 0x1FB000 - */
#define PERFORM_LEN             0x100

#define CFGDATA_ADR             (ULONG)(SYS+0xB100)         /* 0x1FB100 - */
#define CFGDATA_LEN             0x200

#define CAPACITY_ADR            (ULONG)(SYS+0xB300)         /* 0x1FB300 - Capacity data addr(15..0) */
#define CAPACITY_LEN_BYTE       0x08                        /* Total number of bytes */

#define IDENTIFY_ADR            (ULONG)(SYS+0xB310)         /* 0x1FB310 - Identify data address(15..0) */
#define IDENTIFY_LEN_BYTE       0x200                       /* Total number of bytes */

#define INQUIRY_ADR             (ULONG)(SYS+0xB510)         /* 0x1FB510 - Inquiry data address(15..0) */
#define INQUIRY_LEN_BYTE        0x38                        /* Total number of bytes */

#define RD_SUBQ_LBA_ADR         (ULONG)(SYS+0xB550)         /* read sub-channel(LBA) data address*/

#define TRACK_INFO_ADR          (ULONG)(SYS+0xB580)         /* track info data addr */
#define TRACK_INFO_LEN_BYTE     0x30

#define DISC_INFO_ADR           (ULONG)(SYS+0xB5B0)         /* disc info data addr */
#define DISC_INFO_LEN_BYTE  34                              /* To Number of OPC Table Entries */

#define SEND_OPC_ADR            (ULONG)(SYS+0xB610)         /* 0xBD20~0xBD5F:Scratch Area, 0xBD60~0xBD9F:Store Area */
#define SEND_OPC_LEN            0x40                        /* The OPC List entries Max is 8 (one entry size is 8bytes) */

#define REQ_SENSE_ADR           (ULONG)(SYS+0xB690)         /* Request Sense address(15..0) */
#define REQ_SENSE_LEN_BYTE      0x12                        /* Total number of bytes */

#define BUF_CAPACITY_ADR        (ULONG)(SYS+0xB6B0)
#define BUF_CAPACITY_LEN_BYTE   0x0C

#define MMC_FORMAT_ADDR         (ULONG)(SYS+0xB6C0)
#define MMC_FORMAT_LEN          0x10


/* Get Event/Status Group */
#define GESN_NEA_ADR            (ULONG)(SYS+0xB6D0)         /* GESN: NEA (4) */
#define GESN_NEA_LEN            4

#define GESN_OPERATIONAL_ADR    (ULONG)(SYS+0xB6D8)         /* GESN: Operational Change (8) */
#define GESN_OPERATIONAL_LEN    8

#define GESN_POWER_ADR          (ULONG)(SYS+0xB6E0)         /* GESN: power event class (8) */
#define GESN_POWER_LEN          8

#define GESN_EXTERNAL_ADR       (ULONG)(SYS+0xB6E8)         /* GESN: external request event class (8) */
#define GESN_EXTERNAL_LEN       8

#define GESN_MEDIA_ADR          (ULONG)(SYS+0xB6F0)         /* GESN: media event class (8) */
#define GESN_MEDIA_LEN          8

#define GESN_DEVICEBUSY_ADR     (ULONG)(SYS+0xB700)         /* GESN: Device busy event class (8) */
#define GESN_DEVICEBUSY_LEN     8

#define GESN_OPECHGREQ_ADR      (ULONG)(SYS+0xB708)         /* GESN: Operational Change Request/Notification event class (8) */
#define GESN_OPECHGREQ_LEN      8

/* Mode Sense Group */
#define MSENSE_ADR              (ULONG)(SYS+0xB800)         /* mode sense data addr */
#define MSENSE_HDR_LEN          0x08                /* Number of bytes in header  ( 8 bytes) */

#define MSENSE_PAGE_1           (MSENSE_ADR     + MSENSE_HDR_LEN)
#define MSENSE_PAGE_1_LEN       0x0C                /* Number of bytes in page 1  (12 bytes) */

#define MSENSE_PAGE_5           (MSENSE_PAGE_1  + MSENSE_PAGE_1_LEN)
#define MSENSE_PAGE_5_LEN       0x34                /* Number of bytes in page 5  (52 bytes) */
#define MSENSE_PAGE_8           (MSENSE_PAGE_5 + MSENSE_PAGE_5_LEN)
#define MSENSE_PAGE_8_LEN       0x0C                /* Number of bytes in page 8 ( 12 bytes) */

#define MSENSE_PAGE_D           (MSENSE_PAGE_8  + MSENSE_PAGE_8_LEN)
#define MSENSE_PAGE_D_LEN       0x08                /* Number of bytes in page d  ( 8 bytes) */

#define MSENSE_PAGE_E           (MSENSE_PAGE_D  + MSENSE_PAGE_D_LEN)
#define MSENSE_PAGE_E_LEN       0x10                /* Number of bytes in page e  (16 bytes) */

#define MSENSE_PAGE_1A          (MSENSE_PAGE_E + MSENSE_PAGE_E_LEN)
#define MSENSE_PAGE_1A_LEN      0x0C                /* Number of bytes in page 1a  (12 bytes) */

#define MSENSE_PAGE_1D          (MSENSE_PAGE_1A + MSENSE_PAGE_1A_LEN)
//start== Page Code 0x1D Page Length 0x0A in 8090Spec V7
#define MSENSE_PAGE_1D_LEN      0x0C                /* Number of bytes in page 1d  (12 bytes) */
//end==

#define MSENSE_PAGE_2A          (MSENSE_PAGE_1D  + MSENSE_PAGE_1D_LEN)
#define MSENSE_PAGE_2A_LEN      0x1E                /* Number of bytes in page 2a (30 bytes) */

#define MSENSE_PAGE_2C          (MSENSE_PAGE_2A + MSENSE_PAGE_2A_LEN)
#define MSENSE_PAGE_2C_LEN      0x00                /* Number of bytes in page 2c  ( 8 bytes) */

    #if (EN_LS == 1)
#define MSENSE_PAGE_31          (MSENSE_PAGE_2C + MSENSE_PAGE_2C_LEN)
#define MSENSE_PAGE_31_LEN      0x2C                /* Number of bytes in page 31  ( 44 bytes) */

#define MSENSE_PAGE_32          (MSENSE_PAGE_31 + MSENSE_PAGE_31_LEN)
#define MSENSE_PAGE_32_LEN      0x46                /* Number of bytes in page 32  ( 70 bytes) */
    #endif // End of (EN_LS == 1)

#define MSENSE_PAGE_3F_LEN_STA  (MSENSE_PAGE_1_LEN + MSENSE_PAGE_5_LEN  + MSENSE_PAGE_8_LEN + MSENSE_PAGE_D_LEN \
                                 + MSENSE_PAGE_E_LEN + MSENSE_PAGE_1A_LEN + MSENSE_PAGE_1D_LEN + MSENSE_PAGE_2A_LEN)
                                                    /* This length excludes Header Length(8Bytes) */

#define MSENSE_PAGE_3F_LEN_MRW  0


#if (EN_LS == 1)
#define MSENSE_PAGE_3F_LEN_LS   (MSENSE_PAGE_31_LEN + MSENSE_PAGE_32_LEN)
#else
#define MSENSE_PAGE_3F_LEN_LS   0
#endif // End of (EN_LS == 1)

#define MSENSE_PAGE_3F_LEN      (MSENSE_PAGE_3F_LEN_STA + MSENSE_PAGE_3F_LEN_MRW + MSENSE_PAGE_3F_LEN_LS)

//start== Page Code 0x1D Page Length 0x0A in 8090Spec V7
#define MSENSE_SCRATCH          (MSENSE_ADR + MSENSE_HDR_LEN + MSENSE_PAGE_3F_LEN + 2) //multiple of 4
//end==

#define MSENSE_SCRATCH_LO_PAGES (MSENSE_SCRATCH + MSENSE_HDR_LEN)       /* Lo-byte addr in scratch area */
                                                                        /* where all the pages data ara saved */
#define MSENSE_SIZE             (MSENSE_SCRATCH_LO_PAGES - MSENSE_ADR)

/* Read TOC Group */
#define RDTOC_TOC_ADDR          (ULONG)(SYS+0xBF70)     /* TOC format 0 : TOC */
#define RDTOC_TOC_LEN           0x3F0

#define RDTOC_SESSION_ADDR      (ULONG)(SYS+0xC360)     /* TOC format 1 : Session Info */
#define RDTOC_SESSION_LEN       0x10

#define RDTOC_FULLTOC_ADDR      (ULONG)(SYS+0xC370)     /* TOC format 2 : Full TOC (Raw) */
#define RDTOC_FULLTOC_LEN       0xC00

#define RDTOC_PMA_ADDR          (ULONG)(SYS+0xCF70)     /* TOC format 3 : PMA */
#define RDTOC_PMA_LEN           0x460

#define RDTOC_ATIP_ADDR         (ULONG)(SYS+0xD3D0)     /* TOC Format 4 : ATIP */
#define RDTOC_ATIP_LEN            0x20

#define CDTEXT_AREA             (ULONG)(SYS+0xD3F0)         /* CD-Text area, 3 pages */
#define CDTEXT_SIZE             0x2400

/***************************************************************/
/*                Vendor Unique Command Scratch                */
/***************************************************************/
/*----- For Boot afer Flash Up via IDE -----*/
#define TRAN_MODE_INFO_ADR      (ULONG)(SYS+0xF7F0)         /* 0xF7F0~6 : Flash Up via IDE execution Flag  */
#define TRAN_MODE_INFO_LEN      0x0F                        /* 0xF7F7   : for St_dma_feature               */
                                                            /* 0xF7F8   : for St_multi_dma                 */
                                                            /* 0xF7F9   : for St_ultra_dma                 */
                                                            /* 0xF7FA~B : for Hw_host_blks_limit           */
                                                            /* 0xF7FC   : for IDENTIFY WORD-62(Lower Byte) */
                                                            /* 0xF7FD   : for IDENTIFY WORD-63(Lower Byte) */
                                                            /* 0xF7FE   : for IDENTIFY WORD-88(Lower Byte) */

#if (DVD_RAM_READ == 1)
/***************************************************************/
/* DVDRAM TOC 0 & 1 use the Diag. Working page                 */
/***************************************************************/
#define DVDRAM_TOC_00_ADR          (ULONG)(SYS+0xF800)
#define DVDRAM_TOC_00_LEN          0x20
#define DVDRAM_TOC_01_ADR          (DVDRAM_TOC_00_ADR + DVDRAM_TOC_00_LEN)
#define DVDRAM_TOC_01_LEN          0x20
#define DVDRAM_TOC_ALL_LEN         0x40
#endif

#if (E_DIAGS == 1)
/*----- Diagnostics Scratch -----*/
#define DIAG_WORK_ADDR          (ULONG)(SYS+0xF800)     /* Diag Cmd Function working area */
#define END_DIAG_BUFFER         0x800           /* last addr of DRAM Buffer, relative offset     */
#endif /* E_DIAGS */

#define END_DRAM_BUFFER         (ULONG)(SYS+0xFFFF)     /* last addr of dram buffer, relative offset     */
#define END_SCRATCH_AREA        (ULONG)(SYS+0xFFFF)     /* last addr of scratch area, relative offset    */
/*-------------------------------- End of Work Information Area --------------------------------*/

/* The safety margin that any two of host transfer, disk write, and ECC
    operation won't collide together */
#define ALARM_SIZE              0x03
#define ID_BUF_SIZE             0x01            /* used in mode sense page 2Ah */

#define OVERRUN_PAGE            0x60            /* over run page numbers = 3 */
#define UNDERRUN_PAGE           0x20            /* underrun page numbers = 1 */

/* For Decoder */
#define USER_DATA_ADDR_MODE1    0x0000
#define USER_DATA_ADDR_MODE2    0x0008
#define AUX_YELLOW2_DATA_ADDR   0x0000          /* Yellow Mode2 */

#define AUX_DATAINFO_ADDR       0x0960          /* data info BYTE defined in Reg 0b7h */
#define AUX_DATAINFO_C2PAR_ADDR 0x0961          /* Block data info + C2PAR */

#define AUX_SHEADER_ADDR        0x0000          /* M2   block Sub header offset in page */
#define AUX_SUBQDATA_ADDR       0x0970          /* SubQ data */
#define AUX_SUBCODEDATA_ADDR    0x0980          /* SubQ data */

#define AUX_RESERVED26_ADDR     0x0956          /* Reserved 26 bytes */

/* For Encoder */
#define AUX_ENC_PAGE_SIZE       0x200           /* 0.5K */
#define AUX_ENC_HEADER_ADDR     0x09FC          /* Block header offset in page */


/************************************************************************************************
    DVD Buffer Defines for SDRAM Size = 2 MB
*************************************************************************************************/
/*************/
/*Leadin data*/
/*************/
/*DRAM data address*/
#if (BCA_ENABLE == 1)
#define DVD_BCA                         CDTEXT_AREA
#else
#define DVD_BCA                         0xF800                  /*BCA data addr*/
#endif

 /*----------------------------------------------------------------------------
    DVD BUFFER INFORMATION
 ------------------------------------------------------------------------------

    Space for each ECC block        Total area available    829 pages (1 page = 2048 bytes)
    ------------------------        Pages per ECC block     ~19.7 pages
    Data        32768 bytes
    Auxillary     256 bytes         TOTAL ECC BLOCKS         42 ECC blocks
    PI/PO data   7328 bytes
    ------------------------
    Total       40352 bytes


    Buffer area configuration   Bytes       Pages   Start Address
    -------------------------------------------------------------
    Data Area                   1376256     672     0x0000000
    Aux Area                      10752       6     0x0150000
    PI/PO Data                   307776     151     0x0153000
    -------------------------------------------------------------
    Total                       1694784     829

 ----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
    DVD DATA AREA
    TOTAL = 34 (22h) DVD BLOCKS, or 544 (220h) DVD SECTORS
 -----------------------------------------------------------------------------*/
 #define MAX_DVD_BLOCKS             0x22
 #define MAX_DVD_SECTORS            (MAX_DVD_BLOCKS * SECTORS_PER_DVDBLOCK)     // Maximum DVD sectors
 #define DVD_DATA_BUFFER_START      0x000000L                                   /* Absolute start address */
 #define DVD_START_BLOCK            0x000
 #define DVD_END_BLOCK              0x21F

 /*----------------------------------------------------------------------------
    DVD AUX AREA
 -----------------------------------------------------------------------------*/
 #define DVD_AUX_START_ADDR         0x110000L                                   // Relative Start address of DVD auxillary area.
 #define DVD_ABS_AUX_START_ADDR     SCSDRAM + DVD_AUX_START_ADDR                // Absolute start address of DVD auxillary area
 #define DVD_AUX_END_ADDR           0x112200L                                   //  DVD_AUX_START_ADDR+(16*16*MAX_DVD_BLOCKS)

 /*----------------------------------------------------------------------------
    DVD PI/PO AREA
 -----------------------------------------------------------------------------*/
 #define DVD_PIPO_START_ADDR        0x112800L
 #define DVD_PIPO_END_ADDR          0x14F7FFL
 #define DVD_PIPO_BLOCK_SHIFT       8                                           // PIPO blocks are 256 byte resolution (shift of 8 bit)
 #define DVD_PIPO_START_BLOCK       (DVD_PIPO_START_ADDR >> DVD_PIPO_BLOCK_SHIFT)
 #define DVD_CSS_SCRATCH            0x166800L                                   // This is a 2 pages work area for DVD CSS commands

 /*----------------------------------------------------------------------------
    DVD Encoder AREA
 -----------------------------------------------------------------------------*/
 #define DVD_ENC_DATA_SEGA              0x14F800L
 #define DVD_ENC_DATA_SEGB              0x159000L
 #define DVD_ENC_WRITE_SEED1_START      0x162800L
 #define DVD_ENC_WRITE_SEED1_END        0x1647FFL
 #define DVD_ENC_WRITE_SEED2_START      0x164800L
 #define DVD_ENC_WRITE_SEED2_END        0x1667FFL

 #define DVD_RD_MODIFY_WR_PAGE          0x2D0   /* pages used for read modify write */

 //DVD+R/RW Work Area Layout
 #define DVD_PLUSR_DCB_START_PAGE            0x2CF                      // Page of DVD+R/RW/DL DCB in SDRAM
 #define DVD_PLUSR_DCB_START_ADDRESS         0x167800L                  // Address of DVD+R/RW/DL DCB in SDRAM

 #define DVD_PLUSRW_FORMAT_DCB_PAGE_L1          0x2EF                   // Page of DVD+RW9 layer 1 Format DCB in SDRAM.
 #define DVD_PLUSRW_FORMAT_DCB_ADDRESS_L1       0x177800L               // Address of DVD+RW9 layer 1 Format DCB in SDRAM.
 #define DVD_PLUSRW_FORMAT_BITMAP_PAGE_L1       0x2F0                   // Page of DVD+RW9 layer 1 Format bitmap in SDRAM.
 #define DVD_PLUSRW_FORMAT_BITMAP_ADDRESS_L1    0x178000L               // Page of DVD+RW9 layer 1 Format bitmap in SDRAM.

 #define DVD_PLUSRW_FORMAT_DCB_PAGE_L0          0x2DF                   // Page of DVD+RW Format DCB in SDRAM.
 #define DVD_PLUSRW_FORMAT_DCB_ADDRESS_L0       0x16F800L               // Address of DVD+RW Format DCB in SDRAM.
 #define DVD_PLUSRW_FORMAT_BITMAP_PAGE_L0       0x2E0                   // Page of DVD+RW Format bitmap in SDRAM.
 #define DVD_PLUSRW_FORMAT_BITMAP_ADDRESS_L0    0x170000L               // Page of DVD+RW Format bitmap in SDRAM.
 #define DVD_PLUSRW_DATAZONE_END                0x26053FL               // Last maximum PSN in the data zone


 //DVD-RAM Work area Layout
 #define DMA_START_ADDR             0x1AC800   //PDL start
 #define START_DMA_PAGE             (DMA_START_ADDR/0x800UL)
 #define DVD_RAM_DMA_START          (SCSDRAM_START_ADDR + DMA_START_ADDR)           /* Absolute Address in Processor Map Note not just offset into SDRAM*/
 #define PDL_START_ADDR             (DMA_START_ADDR + 0x800UL)
 #define DVD_RAM_PDL_BLK_SHIFT      2                                   /* Resolution of DMA pointer (4 bytes) */
 #define DVD_RAM_PDL_START_BLOCK    ((PDL_START_ADDR + 4) >> DVD_RAM_PDL_BLK_SHIFT) /* Start value to load in reg PDLSRT, Note: the entry header must be jumped */


 //DVD-R/RW Work Area Layout
 #define RMD_FORMAT_1_START_PAGE             0x2CF                      // Page of DVD-R RMD Format 1
 #define RMD_FORMAT_2_START_PAGE             0x2DF                      // Page of DVD-R RMD Format 2
 #define RMD_FORMAT_3_START_PAGE             0x2EF                      // Page of DVD-R RMD Format 3
 #define RMD_FORMAT_4_START_PAGE             0x2FF                      // Page of DVD-R RMD Format 3

 #define DVD_LOGICAL_FORMAT_INFORMATION_PAGE     0x3A2
 #define DVD_LOGICAL_FORMAT_INFORMATION          0x01D1000L

 #define DVD_PHYSICAL_FORMAT_INFORMATION_PAGE    0x3A3                      // Page of Physical Format information in SDRAM.
 #define DVD_PHYSICAL_FORMAT_INFORMATION         0x01D1800L                 // Absolute Address of Physical Format information in SDRAM.

 #define DVD_DISC_MANUFACTURE_INFORMATION_PAGE   0x3A4
 #define DVD_DISC_MANUFACTURE_INFORMATION        0x01D2000L


/****  RMD Information Area ****/
// These definitions are for DVD-R/RW
 #define RMD_FMT1_LINKING_LOSS_AREA      (RMD_FORMAT_1_START_PAGE +    0)    // Sector  0
 #define RMD_FMT1_FIELD0                 (RMD_FORMAT_1_START_PAGE +    1)    // Sector  1
 #define RMD_FMT1_FIELD1                 (RMD_FORMAT_1_START_PAGE +    2)    // Sector  2
 #define RMD_FMT1_FIELD2                 (RMD_FORMAT_1_START_PAGE +    3)    // Sector  3
 #define RMD_FMT1_FIELD3                 (RMD_FORMAT_1_START_PAGE +    4)    // Sector  4
 #define RMD_FMT1_FIELD4                 (RMD_FORMAT_1_START_PAGE +    5)    // Sector  5
 #define RMD_FMT1_FIELD5                 (RMD_FORMAT_1_START_PAGE +    6)    // Sector  6
 #define RMD_FMT1_FIELD6                 (RMD_FORMAT_1_START_PAGE +    7)    // Sector  7
 #define RMD_FMT1_FIELD7                 (RMD_FORMAT_1_START_PAGE +    8)    // Sector  8
 #define RMD_FMT1_FIELD8                 (RMD_FORMAT_1_START_PAGE +    9)    // Sector  9
 #define RMD_FMT1_FIELD9                 (RMD_FORMAT_1_START_PAGE +   10)    // Sector 10
 #define RMD_FMT1_FIELD10                (RMD_FORMAT_1_START_PAGE +   11)    // Sector 11
 #define RMD_FMT1_FIELD11                (RMD_FORMAT_1_START_PAGE +   12)    // Sector 12
 #define RMD_FMT1_FIELD12                (RMD_FORMAT_1_START_PAGE +   13)    // Sector 13
 #define RMD_FMT1_FIELD13                (RMD_FORMAT_1_START_PAGE +   14)    // Sector 14
 #define RMD_FMT1_FIELD14                (RMD_FORMAT_1_START_PAGE +   15)    // Sector 15


/*==============================DVD-RAM=========================================*/
#if (DVD_RAM_READ == 1)
#define DVDRAM_RING_END_PAGE           0x21F       /* last page the SP can access*/
#define DVD_RAM_END_BLOCK              DVDRAM_RING_END_PAGE   // Start of last DVD ECC Data Block in buffer for DVD-RAM.
#define DVDRAM_BUF_TOTAL_BLK_COUNT     0x220       /* total 544 pages */
#define DVDRAM_DATA_START_ADDR         0x000000    /* Data start address */
#define DVDRAM_AUX_START_ADDR          0x110000    /* Aux start address  */
#define DVDRAM_PIPO_START_PAGE         0x225       /* PIPO start page */
#define DVDRAM_DATASEG_A_PAGE          0x29F       /* Data segment A start page */
#define DVDRAM_DATASEG_B_PAGE          0x2B2       /* Data segment B start page */
#define DVDRAM_WRITE_SEED_1_SPAGE      0x2C5       /* Write seed 1 start page */
#define DVDRAM_WRITE_SEED_1_EPAGE      0x2C8       /* Write seed 1 end page */
#define DVDRAM_WRITE_SEED_2_SPAGE      0x2C9       /* Write seed 2 start page */
#define DVDRAM_WRITE_SEED_2_EPAGE      0x2CC       /* Write seed 2 end page */
#define DVDRAM_PAGE                    0x2CF       /* DVD-RAM Area start page */
    #if (DVD_RAM_CERTIFIED_FLAG_DETECTION == 1)
#define DVDRAM_PI_CHK_START_ADDR           DVD_PIPO_START_ADDR
#define DVDRAM_PI_CHK_TOTAL_BYTE           0x11440                           /*34blocks * 2080 bytes*/
#define DVDRAM_PO_CHK_START_ADDR           (DVDRAM_PI_CHK_START_ADDR + DVDRAM_PI_CHK_TOTAL_BYTE)
#define DVDRAM_PO_CHK_TOTAL_BYTE           0x16D80                           /*34blocks * 2752 bytes*/
#define DVDRAM_ERASURE_FLAGS_START_ADDR    (DVDRAM_PO_CHK_START_ADDR + DVDRAM_PO_CHK_TOTAL_BYTE)
#define DVDRAM_ELSI_SIZE                   0x9C0                               /* 208rows * (10 Erasure Location + Flag+ Misc) = 2496 */
#define DVDRAM_ELSI_TOTAL_BYTE             0x14B80                            /*34blocks * 2496 bytes*/
#define DVDRAM_ERROR_CODE_END_ADDR         (DVDRAM_ERASURE_FLAGS_START_ADDR + DVDRAM_ELSI_TOTAL_BYTE -1)
    #endif
#endif  // #if (DVD_RAM_READ == 1)

#if (SDRAM_SIZE_MBYTE == 4)
/************************************************************************************************
    BD Buffer Defines for SDRAM Size = 4 MB
*************************************************************************************************/

/*----------------------------------------------------------------------------
   BD DATA AREA
   FOR BD-ROM,  TOTAL = 52 (34h) CLUSTERS, or 1664 (680h) BD SECTORS
   FOR BD-R/RE, TOTAL = 36 (24h) CLUSTERS, or 1152 (480h) BD SECTORS
-----------------------------------------------------------------------------*/
#define MAX_BDROM_CLUSTERS                 0x34
#define MAX_BDROM_SECTORS                  (MAX_BDROM_CLUSTERS * SECTORS_PER_CLUSTER)
#define BD_FDHOST_LDC_START                0x000000L
#define BD_FDHOST_LDC_START_ADD            (BD_FDHOST_LDC_START >> 16)

#define MAX_BDWRITABLE_CLUSTERS            0x24
#define MAX_BDWRITABLE_SECTORS             (MAX_BDWRITABLE_CLUSTERS * SECTORS_PER_CLUSTER)

/*----------------------------------------------------------------------------
   BD AUX AREA
-----------------------------------------------------------------------------*/
#define BDROM_FDHOST_BIS_START             0x340000L                               // Relative Start address of BD-ROM auxillary area.
#define BDWRITABLE_FDHOST_BIS_START        0x240000L                               // Relative Start address of BD-R/RE auxillary area.
#define BDROM_FDHOST_BIS_START_ADD         (BDROM_FDHOST_BIS_START >> 11)
#define BDWRITABLE_FDHOST_BIS_START_ADD    (BDWRITABLE_FDHOST_BIS_START >> 11)

/*----------------------------------------------------------------------------
   BD SDFD AREA
-----------------------------------------------------------------------------*/
#define BDROM_SDFD_LDC_START               0x35A000L                               // Relative Start address of BD-ROM SDFD LDC area.
#define BDROM_SDFD_BIS_START               0x391800L                               // Relative Start address of BD-ROM SDFD BIS area.
#define BDWRITABLE_SDFD_LDC_START          0x252000L                               // Relative Start address of BD-R/RE SDFD LDC area.
#define BDWRITABLE_SDFD_BIS_START          0x289800L                               // Relative Start address of BD-R/RE SDFD BIS area.
#define SDFD_CLUSTERS                      3

/* BD-ROM definition */

/* BD-R/RE definition */
#define BD_DFL_ADR                  (SCSDRAM_ADDRESS + 0x2A0000)
#define BD_WRT_VFY_ADR              (SCSDRAM_ADDRESS + 0x336000)
#define BD_WRT_VFY_AUX_ADR          (SCSDRAM_ADDRESS + 0x366000)
#define BD_RD_MODIFY_WRT_ADR        (SCSDRAM_ADDRESS + 0x367800)
#define BD_RD_MODIFY_WRT_AUX_ADR    (SCSDRAM_ADDRESS + 0x377800)
#define BD_SRRI_SBM0_ADR            (SCSDRAM_ADDRESS + 0x378000)
#define BD_SRRI_SBM0_PAGE           0x6F0
#define BD_TDDS_ADR                 (SCSDRAM_ADDRESS + 0x387800)
#define BD_TDDS_PAGE                0x70F
#define BD_SBM1_ADR                 (SCSDRAM_ADDRESS + 0x388000)
#define BD_SBM1_PAGE                0x710
#define BD_DI_ADR                   (SCSDRAM_ADDRESS + 0x3A0000)
#define BD_DI_PAGE                  0x740
#define BD_PRM_PAC_ADR              (SCSDRAM_ADDRESS + 0x3A1000)
#define BD_PRM_PAC_PAGE             0x742
#define BD_DWP_PAC_ADR              (SCSDRAM_ADDRESS + 0x3A1800)
#define BD_DWP_PAC_PAGE             0x743

#define SRRI_ENTRY_ADR              (BD_SRRI_SBM0_ADR + 64)
#define BD_ICE_INDICATOR_ADDR       (SCSDRAM_ADDRESS + 0x3A9800)    /* 1 page */

#define BD_DDS_START_ADDR                    (SCSDRAM + 0x387800)
#define BD_DDS_START_PAGE                    0x70F
#define BD_FRAGMENT_ENTRY_AREA_START         (SCSDRAM + 0x397800)
#define BD_FRAGMENT_ENTRY_PAGE               0x72F
#else
/************************************************************************************************
    BD Buffer Defines for SDRAM Size = 2 MB
*************************************************************************************************/

/*----------------------------------------------------------------------------
   BD DATA AREA
   FOR BD-ROM,  TOTAL = 22 (16h) CLUSTERS, or 704  (2C0h) BD SECTORS
   FOR BD-R/RE, TOTAL = 16 (10h) CLUSTERS, or 512  (200h) BD SECTORS
-----------------------------------------------------------------------------*/
#define MAX_BDROM_CLUSTERS                 0x16
#define MAX_BDROM_SECTORS                  (MAX_BDROM_CLUSTERS * SECTORS_PER_CLUSTER)
#define BD_FDHOST_LDC_START                0x000000L
#define BD_FDHOST_LDC_START_ADD            (BD_FDHOST_LDC_START >> 16)

#define MAX_BDWRITABLE_CLUSTERS            0x10
#define MAX_BDWRITABLE_SECTORS             (MAX_BDWRITABLE_CLUSTERS * SECTORS_PER_CLUSTER)

/*----------------------------------------------------------------------------
   BD AUX AREA
-----------------------------------------------------------------------------*/
#define BDROM_FDHOST_BIS_START             0x160000L                               // Relative Start address of BD-ROM auxillary area.
#define BDWRITABLE_FDHOST_BIS_START        0x100000L                               // Relative Start address of BD-R/RE auxillary area.
#define BDROM_FDHOST_BIS_START_ADD         (BDROM_FDHOST_BIS_START >> 11)
#define BDWRITABLE_FDHOST_BIS_START_ADD    (BDWRITABLE_FDHOST_BIS_START >> 11)

/*----------------------------------------------------------------------------
   BD SDFD AREA
-----------------------------------------------------------------------------*/
#define BDROM_SDFD_LDC_START               0x16B000L                               // Relative Start address of BD-ROM SDFD LDC area.
#define BDROM_SDFD_BIS_START               0x1A2800L                               // Relative Start address of BD-ROM SDFD BIS area.
#define BDWRITABLE_SDFD_LDC_START          0x108000L                               // Relative Start address of BD-R/RE SDFD LDC area.
#define BDWRITABLE_SDFD_BIS_START          0x13F800L                               // Relative Start address of BD-R/RE SDFD BIS area.
#define SDFD_CLUSTERS                      3

/* BD-ROM definition */

/* BD-R/RE definition */
#define BD_DFL_ADR                  (SCSDRAM_ADDRESS + 0x147800)
#define BD_SRRI_SBM0_PAGE           0x28F

#define BD_SRRI_SBM0_ADR            (SCSDRAM_ADDRESS + 0x18D800)

#define BD_TDDS_ADR                 (SCSDRAM_ADDRESS + 0x19D000)
#define BD_TDDS_PAGE                0x33A

#define BD_SBM1_ADR                 (SCSDRAM_ADDRESS + 0x19D800)
#define BD_SBM1_PAGE                0x33B

#define BD_DI_ADR                   (SCSDRAM_ADDRESS + 0x1D1800)
#define BD_DI_PAGE                  0x3A3
#define BD_PRM_PAC_ADR              (SCSDRAM_ADDRESS + 0x1AD000)
#define BD_PRM_PAC_PAGE             0x35A
#define BD_DWP_PAC_ADR              (SCSDRAM_ADDRESS + 0x1AD800)
#define BD_DWP_PAC_PAGE             0x35B

#define SRRI_ENTRY_ADR              (BD_SRRI_SBM0_ADR + 64)
#define BD_ICE_INDICATOR_ADDR       (SCSDRAM_ADDRESS + 0x1D2800)    /* 1 page */

#define BD_DDS_START_ADDR                    BD_TDDS_ADR
#define BD_DDS_START_PAGE                    0x33A
#define BD_FRAGMENT_ENTRY_AREA_START         (SCSDRAM + 0x1D1000)
#define BD_FRAGMENT_ENTRY_PAGE               0x3A2
#endif //SDRAM_SIZE_MBYTE == 4

#if (ENABLE_HDDVD == 1)
/************************************************************************************************
    HD DVD Buffer Defines for SDRAM Size = 4 MB
*************************************************************************************************/
 /*----------------------------------------------------------------------------
    HD DVD DATA AREA
    TOTAL = 40 (28h) HD BLOCKS, or 1280 (500h) HD SECTORS
 -----------------------------------------------------------------------------*/
 #define MAX_HDDVD_BLOCKS             0x14//0x28                                   // 20/40 Clusters
 #define MAX_HDDVD_SECTORS            (MAX_HDDVD_BLOCKS * SECTORS_PER_HDDVDBLOCK)  // Maximum HD sectors(1280 pages)
 #define HDDVD_DATA_BUFFER_START      0x000000L                                    // Absolute start address
 #define HDDVD_START_BLOCK            0x000
 #define HDDVD_END_BLOCK              0x27F//0x4FF

 /*----------------------------------------------------------------------------
    HD DVD AUX AREA
 -----------------------------------------------------------------------------*/
 #define HDDVD_AUX_START_ADDR         0x140000L //0x280000L
 #define HDDVD_ABS_AUX_START_ADDR     SCSDRAM + HDDVD_AUX_START_ADDR

 /*----------------------------------------------------------------------------
    HD DVD PI/PO AREA
 -----------------------------------------------------------------------------*/
 #define HDDVD_PIPO_START_ADDR        0x285000L
 #define HDDVD_PIPO_END_ADDR          0x3141FFL
 #define HDDVD_PIPO_BLOCK_SHIFT       9                                           // HD PIPO blocks are 512 byte resolution (shift of 9 bit)
 #define HDDVD_PIPO_START_BLOCK       (HDDVD_PIPO_START_ADDR >> HDDVD_PIPO_BLOCK_SHIFT)

 /*----------------------------------------------------------------------------
    HD DVD Encoder AREA
 -----------------------------------------------------------------------------*/
// add future

 /*----------------------------------------------------------------------------
    HD DVD Work AREA
 -----------------------------------------------------------------------------*/
 #define HDDVD_LOGICAL_FORMAT_INFORMATION_PAGE         0x639
 #define HDDVD_LOGICAL_FORMAT_INFORMATION              0x031C800L

 #define HDDVD_PHYSICAL_FORMAT_INFORMATION_PAGE        0x63A                      // Page of Physical Format information in SDRAM.
 #define HDDVD_PHYSICAL_FORMAT_INFORMATION             0x031D000L                 // Absolute Address of Physical Format information in SDRAM.

 #define HDDVD_DISC_MANUFACTURE_INFORMATION_PAGE       0x63B
 #define HDDVD_DISC_MANUFACTURE_INFORMATION            0x031D800L

 #define HDDVD_COPYRIGHT_PROTECTION_INFORMATION_PAGE   0x63C
 #define HDDVD_COPYRIGHT_PROTECTION_INFORMATION        0x031E000L

#endif
#if (STORE_MSG_TO_SDRAM == 1)
#define LOGGING_MSG_SPAGE                 0x37A
#define LOGGING_MSG_START_ADDR            0x1BD000L
#define LOGGING_MSG_EPAGE                 0x391
#define LOGGING_MSG_END_ADDR              0x1C8FFFL
#define LOGGING_MSG_PAGE_SIZE             0x18

#if (STORE_MSG_TO_FLASHROM == 1)
// Message Data Area
#define MSG_DATA            0x0L       // Drive Message 0x0000000 - 0x000C0000
#endif
#endif
#endif /* __SDRAM_H__ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
