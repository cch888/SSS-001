/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
*    FILE NAME:     $Workfile: dvDefMgr.c $
*
*    DESCRIPTION:    Logical to physical addressing interface for DVD-RAM
*                    DVD_RAM Ver. 1.0 (2.6 GB) & Ver. 2.0 (4.7 GB) Defect Management and Zoning
*
*   Defect Management as specified in
*      DVD-Forum:       DVD-Specifications for Rewritable Disc (DVD-RAM)Ver. 1.0 July 1997
*      DVD-Forum:       DVD-Specifications for Rewritable Disc (DVD-RAM)Ver. 2.0 September 1999
*   Or
*      ECMA Standards:  ECMA-272    120 mm DVD Rewritable Disk (DVD-RAM)Ver  1.0  2nd Edition - June 1999
*
*
*   Capacity:
*   Ver. 1.0 2.6 Gbyte
*   Ver. 2.0 4.7 GB
*
*
*   FUNCTIONS:
*
*   $Revision: 7 $
*   $Date: 10/12/21 4:17p $
*
*
*
************************** SUNEXT CONFIDENTIAL ********************************/

/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\com_im.h"
#include ".\hif\gp_util.h"
#include ".\dvd\dvCFG.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvDiscRd.h"
#include ".\dvd\dvseek.h"
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\ramExtern.h"
#include ".\hif\gp_ram.h"
#include ".\player\plrplayr.H"
#include ".\hif\mmc.h"
#include ".\AL\REG_global.h"
#include ".\al\reg_dvd_encoder.h"


/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
void   EnableDVDRAMBuffer(void);
void   DisableDVDRAMBuffer(void);
BOOL   ReadAndCheckDMA(void);
ULONG  FindMaximum(ULONG *Array, BYTE n);
BOOL   SearchPdlEntry2(LONG SearchPSN);
BOOL   SearchSdlEntry2(LONG SearchPSN);
void   SetTotalDefectNumber(ULONG num);
BYTE   ConvertBitMap(BYTE *pBitMap);
void   InitialProtectionPage(void);
void   SaveReplaceInfo(ULONG DefBlkPSN,ULONG RepBlkPSN);
void   RamInitializeVar(void);
void   InitializeDMA(void);
void   InitializeDDS(void);
BOOL   RamCheckZoneBoundary(ULONG InputLba);
void   GetDvdramAbexIdCode(void);


/******************************************************************/
/*      Next are global variables used for DVD-RAM functionality. */
/******************************************************************/
ReplaceBlockType RepInfo[MAX_DVD_BLOCKS];
BYTE    DiscTypeIndex;                      /* A local version of DiscType. Will contain 0 for DVD RAM 1.0 Contains 1 for DVD RAM 2.0       */
BYTE    LocalZoneNumber;                    /* A local disc zone number.  Will contain 24 for DVD RAM 1.0 Contains 35 for DVD RAM 2.0       */
BYTE    RamDiscStatus;                      /* This variable indicates the status of the inserted DVD-RAM disc */
BYTE    IncreasedSdlCount;
long    format_size;


StructureOfGroupsType DVDRAMZoneTable[NUM_ZONES_RAM_20];


/* DVD-RAM Ver 1.0 */
/* This is from the DVD-RAM 1.0 Structure of Groups (zones)(Table 6), */
/* taken from ECMA-272 2nd edition June 1999.                         */
const StructureOfGroupsType DVDRAMZoneTableV10[NUM_ZONES_RAM_10] =
{
/* Number       Start            User                User              End            LSN of
   of           Sector           Area                Area              Sector         the 1st
   Sectors      Number           Start               End               Number         sector
   Per                           Sector              Sector                           in the
   Revolution                    Number              Number                           Group

SectorsPerRev   StartSectorPSN   StartUserSectorPSN  EndUserSectorPSN  EndSectorPSN   StartSectorLsn              */
   17,          0x030000L,       0x031000L,          0x0377DFL,        0x037D5FL,           0,        /* Zone  0  */
   18,          0x037D60L,       0x037D90L,          0x03FB2FL,        0x04021FL,       26592,        /* Zone  1  */
   19,          0x040220L,       0x040250L,          0x0486EFL,        0x048E3FL,       58752,        /* Zone  2  */
   20,          0x048E40L,       0x048E70L,          0x051A0FL,        0x0521BFL,       92704,        /* Zone  3  */
   21,          0x0521C0L,       0x0521F0L,          0x05B48FL,        0x05BC9FL,      128448,        /* Zone  4  */
   22,          0x05BCA0L,       0x05BCD0L,          0x06566FL,        0x065EDFL,      165984,        /* Zone  5  */
   23,          0x065EE0L,       0x065F10L,          0x06FFAFL,        0x07087FL,      205312,        /* Zone  6  */
   24,          0x070880L,       0x0708B0L,          0x07B04FL,        0x07B97FL,      246432,        /* Zone  7  */
   25,          0x07B980L,       0x07B9C0L,          0x08683FL,        0x0871DFL,      289344,        /* Zone  8  */
   26,          0x0871E0L,       0x087220L,          0x09279FL,        0x09319FL,      334016,        /* Zone  9  */
   27,          0x0931A0L,       0x0931E0L,          0x09EE5FL,        0x09F8BFL,      380480,        /* Zone 10  */
   28,          0x09F8C0L,       0x09F900L,          0x0ABC7FL,        0x0AC73FL,      428736,        /* Zone 11  */
   29,          0x0AC740L,       0x0AC780L,          0x0B91FFL,        0x0B9D1FL,      478784,        /* Zone 12  */
   30,          0x0B9D20L,       0x0B9D60L,          0x0C6EDFL,        0x0C7A5FL,      530624,        /* Zone 13  */
   31,          0x0C7A60L,       0x0C7AA0L,          0x0D531FL,        0x0D5EFFL,      584256,        /* Zone 14  */
   32,          0x0D5F00L,       0x0D5F40L,          0x0E3EBFL,        0x0E4AFFL,      639680,        /* Zone 15  */
   33,          0x0E4B00L,       0x0E4B50L,          0x0F31AFL,        0x0F3E5FL,      696896,        /* Zone 16  */
   34,          0x0F3E60L,       0x0F3EB0L,          0x102C0FL,        0x10391FL,      755872,        /* Zone 17  */
   35,          0x103920L,       0x103970L,          0x112DCFL,        0x113B3FL,      816640,        /* Zone 18  */
   36,          0x113B40L,       0x113B90L,          0x1236EFL,        0x1244BFL,      879200,        /* Zone 19  */
   37,          0x1244C0L,       0x124510L,          0x13476FL,        0x13559FL,      943552,        /* Zone 20  */
   38,          0x1355A0L,       0x1355F0L,          0x145F4FL,        0x146DDFL,     1009696,        /* Zone 21  */
   39,          0x146DE0L,       0x146E30L,          0x157E8FL,        0x158D7FL,     1077632,        /* Zone 22  */
   40,          0x158D80L,       0x158DD0L,          0x16A57FL,        0x16B47FL,     1147360         /* Zone 23  */
};

/* DVD-RAM Ver 2.0 */
/* This from the DVD-RAM 2.0 Structure of Groups (zones),                            */
/* taken from DVD Forum DVD-Specifications for Rewritable Disc (DVD-RAM) Version 2.0 */
const StructureOfGroupsType DVDRAMZoneTableV20[NUM_ZONES_RAM_20] =
{
/* Number       Start            User                User             End             LSN of          LSN of the first sector in the Group are DUMMY values for DVD-RAM 2.0
   of           Sector           Area                Area             Sector          the 1st         For DVD-RAM Ver. 2.0 LSN for the first sector in the zone is read off
   Sectors      Number           Start               End              Number          sector          the disc. These values are contained in the DDS from the disc.
   Per                           Sector              Sector                           in the
   Revolution                    Number              Number                           Group

SectorsPerRev   StartSectorPSN   StartUserSectorPSN  EndUserSectorPSN EndSectorPSN    StartSectorLsn (Start LSN for the Zone for Version 2.0)   */
   25,          0x030000L,       0x031000L,          0x0398DFL,       0x03991FL,      0x000000L,      /* Zone  0  */ /* StartUserSectorPSN shoud be set as 0x31000 here while program using */
   26,          0x039920L,       0x039960L,          0x04381FL,       0x04385FL,      0x0056E0L,      /* Zone  1  */
   27,          0x043860L,       0x0438A0L,          0x04DD7FL,       0x04DDBFL,      0x00F5A0L,      /* Zone  2  */
   28,          0x04DDC0L,       0x04DE00L,          0x0588FFL,       0x05893FL,      0x019A80L,      /* Zone  3  */
   29,          0x058940L,       0x058980L,          0x063A9FL,       0x063ADFL,      0x024580L,      /* Zone  4  */
   30,          0x063AE0L,       0x063B20L,          0x06F25FL,       0x06F29FL,      0x02F6A0L,      /* Zone  5  */
   31,          0x06F2A0L,       0x06F2E0L,          0x07B03FL,       0x07B07FL,      0x03ADE0L,      /* Zone  6  */
   32,          0x07B080L,       0x07B0C0L,          0x08743FL,       0x08747FL,      0x046B40L,      /* Zone  7  */
   33,          0x087480L,       0x0874D0L,          0x093E4FL,       0x093E9FL,      0x052EC0L,      /* Zone  8  */
   34,          0x093EA0L,       0x093EF0L,          0x0A0E8FL,       0x0A0EDFL,      0x05F840L,      /* Zone  9  */
   35,          0x0A0EE0L,       0x0A0F30L,          0x0AE4EFL,       0x0AE53FL,      0x06C7E0L,      /* Zone 10  */
   36,          0x0AE540L,       0x0AE590L,          0x0BC16FL,       0x0BC1BFL,      0x079DA0L,      /* Zone 11  */
   37,          0x0BC1C0L,       0x0BC210L,          0x0CA40FL,       0x0CA45FL,      0x087980L,      /* Zone 12  */
   38,          0x0CA460L,       0x0CA4B0L,          0x0D8CCFL,       0x0D8D1FL,      0x095B80L,      /* Zone 13  */
   39,          0x0D8D20L,       0x0D8D70L,          0x0E7BAFL,       0x0E7BFFL,      0x0A43A0L,      /* Zone 14  */
   40,          0x0E7C00L,       0x0E7C50L,          0x0F70AFL,       0x0F70FFL,      0x0B31E0L,      /* Zone 15  */
   41,          0x0F7100L,       0x0F7160L,          0x106BBFL,       0x106C1FL,      0x0C2640L,      /* Zone 16  */
   42,          0x106C20L,       0x106C80L,          0x116CFFL,       0x116D5FL,      0x0D20A0L,      /* Zone 17  */
   43,          0x116D60L,       0x116DC0L,          0x12745FL,       0x1274BFL,      0x0E2120L,      /* Zone 18  */
   44,          0x1274C0L,       0x127520L,          0x1381DFL,       0x13823FL,      0x0F27C0L,      /* Zone 19  */
   45,          0x138240L,       0x1382A0L,          0x14957FL,       0x1495DFL,      0x103480L,      /* Zone 20  */
   46,          0x1495E0L,       0x149640L,          0x15AF3FL,       0x15AF9FL,      0x114760L,      /* Zone 21  */
   47,          0x15AFA0L,       0x15B000L,          0x16CF1FL,       0x16CF7FL,      0x126060L,      /* Zone 22  */
   48,          0x16CF80L,       0x16CFE0L,          0x17F51FL,       0x17F57FL,      0x137F80L,      /* Zone 23  */
   49,          0x17F580L,       0x17F5F0L,          0x19212FL,       0x19219FL,      0x14A4C0L,      /* Zone 24  */
   50,          0x1921A0L,       0x192210L,          0x1A536FL,       0x1A53DFL,      0x15D000L,      /* Zone 25  */
   51,          0x1A53E0L,       0x1A5450L,          0x1B8BCFL,       0x1B8C3FL,      0x170160L,      /* Zone 26  */
   52,          0x1B8C40L,       0x1B8CB0L,          0x1CCA4FL,       0x1CCABFL,      0x1838E0L,      /* Zone 27  */
   53,          0x1CCAC0L,       0x1CCB30L,          0x1E0EEFL,       0x1E0F5FL,      0x197680L,      /* Zone 28  */
   54,          0x1E0F60L,       0x1E0FD0L,          0x1F59AFL,       0x1F5A1FL,      0x1ABA40L,      /* Zone 29  */
   55,          0x1F5A20L,       0x1F5A90L,          0x20AA8FL,       0x20AAFFL,      0x1C0420L,      /* Zone 30  */
   56,          0x20AB00L,       0x20AB70L,          0x22018FL,       0x2201FFL,      0x1D5420L,      /* Zone 31  */
   57,          0x220200L,       0x220280L,          0x235E9FL,       0x235F1FL,      0x1EAA40L,      /* Zone 32  */
   58,          0x235F20L,       0x235FA0L,          0x24C1DFL,       0x24C25FL,      0x200660L,      /* Zone 33  */
   59,          0x24C260L,       0x24C2E0L,          0x265F5FL,       0x265F5FL,      0x2168A0L       /* Zone 34  */
};

/* DVD-RAM 80 mm Ver 2.0*/
/* This from the 80 mm DVD-RAM 2.0 Structure of Groups (zones),                      */
/* taken from DVD Forum DVD-Specifications for Rewritable Disc (DVD-RAM) Version 2.0 */
const StructureOfGroupsType DVDRAMZoneTableV20_8CM[NUM_ZONES_RAM_20_8CM] =
{
/* Number       Start            User                User             End             LSN of          LSN of the first sector in the Group are DUMMY values for DVD-RAM 2.0
   of           Sector           Area                Area             Sector          the 1st         For DVD-RAM Ver. 2.0 LSN for the first sector in the zone is read off
   Sectors      Number           Start               End              Number          sector          the disc. These values are contained in the DDS from the disc.
   Per                           Sector              Sector                           in the
   Revolutio                     Number              Number                           Group

SectorsPerRev   StartSectorPSN   StartUserSectorPSN  EndUserSectorPSN EndSectorPSN    StartSectorLsn (Start LSN for the Zone for Version 2.0)   */
   25,          0x030000L,       0x031000L,          0x0398DFL,       0x03991FL,      0x000000L,      /* Zone  0  */ /* StartUserSectorPSN shoud be set as 0x31000 here while program using */
   26,          0x039920L,       0x039960L,          0x04381FL,       0x04385FL,      0x0074E0L,      /* Zone  1  */
   27,          0x043860L,       0x0438A0L,          0x04DD7FL,       0x04DDBFL,      0x0113A0L,      /* Zone  2  */
   28,          0x04DDC0L,       0x04DE00L,          0x0588FFL,       0x05893FL,      0x01B880L,      /* Zone  3  */
   29,          0x058940L,       0x058980L,          0x063A9FL,       0x063ADFL,      0x026380L,      /* Zone  4  */
   30,          0x063AE0L,       0x063B20L,          0x06F25FL,       0x06F29FL,      0x0314A0L,      /* Zone  5  */
   31,          0x06F2A0L,       0x06F2E0L,          0x07B03FL,       0x07B07FL,      0x03CBE0L,      /* Zone  6  */
   32,          0x07B080L,       0x07B0C0L,          0x08743FL,       0x08747FL,      0x048940L,      /* Zone  7  */
   33,          0x087480L,       0x0874D0L,          0x093E4FL,       0x093E9FL,      0x054CC0L,      /* Zone  8  */
   34,          0x093EA0L,       0x093EF0L,          0x0A0E8FL,       0x0A0EDFL,      0x061640L,      /* Zone  9  */
   35,          0x0A0EE0L,       0x0A0F30L,          0x0AE4EFL,       0x0AE53FL,      0x06E5E0L,      /* Zone 10  */
   36,          0x0AE540L,       0x0AE590L,          0x0BC16FL,       0x0BC1BFL,      0x07BBA0L,      /* Zone 11  */
   37,          0x0BC1C0L,       0x0BC210L,          0x0CA40FL,       0x0CA45FL,      0x089780L,      /* Zone 12  */
   38,          0x0CA460L,       0x0CA4B0L,          0x0E121FL,       0x0E121FL,      0x097980L       /* Zone 13  */
};


/**********************************************************************/
/* FUNCTIONS                                                          */
/*********************************************************************/


/***************************************************************************************/
/*  Function:    DvdRamStartUp()                                                       */
/*  Description:                                                                       */
/*  Parameters:  none                                                                  */
/*  Returned Value: none                                                               */
/***************************************************************************************/
BOOL DvdRamStartUp(void)
{
    send_msg80(SEND_READ_DVD_MSG,0x710210);

    if (pdbGetDVDVersion()== DVD_VER_1_0)
    {
        if  (DiscTypeIndex != LOC_DISC_TYPE_10)
        {
            /* Disc Type is DVD_RAM Ver. 1.0   2.6 GB  */
            DiscTypeIndex   = LOC_DISC_TYPE_10;
            LocalZoneNumber = NUM_ZONES_RAM_10;
            memcpy(DVDRAMZoneTable, DVDRAMZoneTableV10, sizeof(DVDRAMZoneTableV10));
        }
    }
    else
    {
        if ((St_12_cm) && (DiscTypeIndex != LOC_DISC_TYPE_20))
        {
            /* Disc Type is DVD_RAM Ver. 2.0   4.7 GB  */
            DiscTypeIndex   = LOC_DISC_TYPE_20;
            LocalZoneNumber = NUM_ZONES_RAM_20;
            memcpy(DVDRAMZoneTable, DVDRAMZoneTableV20, sizeof(DVDRAMZoneTableV20));
        }
        if ((!St_12_cm) && (DiscTypeIndex != LOC_DISC_TYPE_20_8CM))
        {
            /* Disc Type is 80 mm DVD_RAM Ver. 2.0   1.46 GB  */
            DiscTypeIndex   = LOC_DISC_TYPE_20_8CM;
            LocalZoneNumber = NUM_ZONES_RAM_20_8CM;
            memcpy(DVDRAMZoneTable, DVDRAMZoneTableV20_8CM, sizeof(DVDRAMZoneTableV20_8CM));
        }
    }

    InitialProtectionPage();

    if (ReadAndCheckDMA() == FALSE)
    {
        send_msg80(SEND_READ_DVD_MSG,0x710211);
        return FALSE;
    }

    send_msg80(SEND_READ_DVD_MSG,0x710212);

    /* Start-up is successful here */
    RamInitializeVar();
    EnableDVDRAMBuffer();   /* initialize dynamic buffer pointer for DVD-RMA disc */

    #if 0 //For debugging
    AddFakeDefectEntry();
    #endif

    //GetDvdramAbexIdCode();

    ServoSeek(0x35000L,eLayer0);
    ServoPause();                /* execute pause operation */
    return TRUE;
}


/***************************************************************************************/
/*  Function:    EnableDVDRAMBuffer()                                                  */
/*  Description: Enable the DRAM buffer layout for DVD-RAM disc                        */
/*  Parameters:  none                                                                  */
/*  Returned Value: none                                                               */
/***************************************************************************************/
void EnableDVDRAMBuffer(void)
{
    dvdStopDiscTransfer();
    DelaymS(5);
    Iram_usiRingSP  = 0;
    RING_END_PAGE  = DVDRAM_RING_END_PAGE;
    Iram_usiRingEP  = RING_END_PAGE;
    BUF_TOTAL_BLK_COUNT = RING_END_PAGE + 1;

    WRITE_FIELD(HAL_HST_HSTBUFSPG, Iram_usiRingSP);         /* Host start blk. */
    WRITE_FIELD(HAL_DVDD_STARTBK, Iram_usiRingSP);           /* DVD decoder ring start blk. */
    WRITE_FIELD(HAL_DVDD_PIPOSRT, DVDRAM_PIPO_START_BLOCK);  /* PI/PO start blk. */

    WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);         /* Host end blk. */
    WRITE_FIELD(HAL_DVDD_ENDBK, Iram_usiRingEP);             /* DVD decoder ring end blk. */

    InitCopyStartEndPages();                            /* Initialize Copy block Start and End pages */

        /* DVD encoder */
    WRITE_FIELD(HAL_DVDE_DEBUFSPG, Iram_usiRingSP);  /* DVD encoder ring start page */
    WRITE_FIELD(HAL_DVDE_DEBUFEPG,  Iram_usiRingEP);     /* DVD encoder ring end page */

    WRITE_FIELD(HAL_DVDE_DSASADR,  DVDRAM_DATA_SEGA_ADDR);    /* DVD encoder data segment A start addr */
    WRITE_FIELD(HAL_DVDE_DSBSADR,  DVDRAM_DATA_SEGB_ADDR);    /* DVD encoder data segment B start addr */

    WRITE_FIELD(HAL_DVDE_WSB1SAD, DVDRAM_WRITE_SEED1_START);    /* DVD encoder write seed 1 start addr */
    WRITE_FIELD(HAL_DVDE_WSB1EAD, DVDRAM_WRITE_SEED1_END);      /* DVD encoder write seed 1 end addr */
    WRITE_FIELD(HAL_DVDE_WSB2SAD, DVDRAM_WRITE_SEED2_START);    /* DVD encoder write seed 2 start addr */
    WRITE_FIELD(HAL_DVDE_WSB2EAD, DVDRAM_WRITE_SEED2_END);      /* DVD encoder write seed 2 end addr */
}


/*----------------------------------------------------------------------------
  Name: DisableDVDRAMBuffer
  Description: Disables the DVDRAM DRAM buffer layout
  Input:  None
  Output: None
----------------------------------------------------------------------------*/
void DisableDVDRAMBuffer(void)
{
    dvdStopDiscTransfer();  //StopDecoder();
    DelaymS(5);

    InitializeDVDSdramBuffer(); /* initial buffer as DVD */
}


/******************************************************************************************/
/*  Function:    ReadAndCheckDMA()                                                        */
/*  Description: According to DVD-RAM Version 2.1 Annex R, the drive should check whether */
/*               there is at least one good pair of DDS/PDL and SDL blocks.               */
/*               The good pair can come from the different DMA.                           */
/*               If all of the following conditions are satisfied, the pair of blocks are */
/*               regarded as good.                                                        */
/*               a) All the data is correctable by ECC.                                   */
/*               b) The DDS identifier is 0A0Ah.                                          */
/*               c) The value of the DDS/PDL update counter in each Block is identical.   */
/*  Parameters:  none                                                                     */
/*  Returned Value: none                                                                  */
/******************************************************************************************/
BOOL ReadAndCheckDMA(void)
{
    DiscAccessStatusE DiscReadStatus;   /* Status of buffer loading */
    ULONG   DDSUpdateCounterInDDSBlock[4],DDSUpdateCounterInSDLBlock[4];    /* store 8 DDS/PDL update counters within 4 DMAs */
    ULONG   SDLUpdateCounter[4];        /* store 4 SDL update counters within 4 DMAs */
    ULONG   NewSDLUpdateCounter[4];     /* store the temp SDLUpdateCounter for valid SDL Blocks */
    ULONG   MaxDDSUpdateCounter1;        /* A variable to save the maximun DDS/PDL update counter in DDS Block */
    ULONG   MaxDDSUpdateCounter2;        /* A variable to save the maximun DDS/PDL update counter in SDL Block */
    ULONG   MaxSDLUpdateCounter;        /* A variable to save the maximun SDL update counter      */
    ULONG   RecdTargetPSN;              /* A target PSN for RECD judgement */
    ULONG   CurrentPSN;
    DDSTYPE *pDDS;
    SDLTYPE *pSDL;
    BYTE    DmaCount,EccCount;          /* Total DMA number is 4, Each DMA contains 2 ECC blocks. */
    BYTE    GoodDDS[4],GoodSDL[4];      /* An Array to store the numbers of the good blocks */
    BYTE    index1,index2;              /* The total amount of the good blocks for DDS/PDL and SDL */
    BYTE    BufGoodBitMap;              /* A BitMap to show the buffer status, bit0~3 indicate DDS/PDL Block, and bit4~7 indicate SDL Block */
    BYTE    i,j;
    BOOL    rt;                         /* return value */

    send_msg80(1,0x710214);

    /* skip read ahead */
    dvdReadAheadStartPSN = 0;
    dvdLastDecodedPSN = 0;
    dvdramReadAheadStartLSN = 0;
    dvdramLastDecodedLSN = 0;

    #if 1 /*DVDRAM_BYPASS_DMA_CHECK*/
    {
    format_size = DVDRAM_TOTAL_LSN;     //debug only
    DataBuffer.CurDiscPtr  = 0; /* put into scratch buffer */

    for(i=0; i < 2; i++)    /* Read DMA 1 & 2 only */
    {
        /* Prepare parameters for Buffer Loading. */
        dvdDiscRequest.Type             = INIT_READ;                       /* LoadType; */
        dvdDiscRequest.TargetID.Long    = DmaLocation[i][DiscTypeIndex];   /* Read DMA */
        dvdDiscRequest.TargetLayer      = eLayer0;
        dvdDiscRequest.EccBlockCount    = 2L;      /* Read 2 ECC Block */
        RWDisc.dvRAM.ReadingDMA = TRUE;
        ClearDataBuffer();
        InitializeIceIndicator();

        DiscReadStatus = dvdStartDiscTransfer();    /* Start Transfer of disc data into buffer and return status */

        if (DiscReadStatus == BUF_GOOD)
        {
            if (RWDisc.dvRAM.BlankDMA)
            {
                send_msg80(1,0x710216);
                InitializeDMA();
            }
            else
            {
                send_msg80(1,0x710217);
                // Check DDS identifier - 0A0Ah
                if ((SCSDRAM[0] == 0x0A) && (SCSDRAM[0] == 0x0A))
                {
                    send_msg80(1,0x710218);
                    CopyBufferToBuffer( 0x00, 0, (USHORT) START_DDS_PAGE, 0, 0x10);
                    CopyBufferToBuffer( 0x10, 0, (USHORT) START_SDL_PAGE, 0, 0x10);
                }
                else
                {
                    send_msg80(1,0x710219);
                    InitializeDMA();    /* make a pseudo DMA */
                    RWDisc.dvRAM.SdlUpdateFlag = TRUE;
                    return TRUE;
                }
            }

            RWDisc.dvRAM.ReadingDMA = FALSE;
            return (TRUE);
        }
        else if(i == 1)     /* last DMA read fail */
        {
            send_msg80(SEND_READ_DVD_MSG,0x71021A);
        #if 0   /* for test only, do not delete it */
            InitializeDMA();    /* make a pseudo DMA */
            RWDisc.dvRAM.SdlUpdateFlag = TRUE;
            return TRUE;
        #else
            St_blank_disc = TRUE;   //start up fail -> not activate read ahead
            return FALSE;
        #endif
        }
    }
    }
    #endif /*DVDRAM_BYPASS_DMA_CHECK*/

    rt = TRUE;
    BufGoodBitMap = 0;
    format_size   = 0;

    /* Initialize the update counter */
    for (DmaCount = 0; DmaCount < 4; DmaCount++)
    {
        DDSUpdateCounterInDDSBlock[DmaCount] = 0;
        DDSUpdateCounterInSDLBlock[DmaCount] = 0;
        SDLUpdateCounter[DmaCount]           = 0;
    }

    /* Check whether DMA exist or not. We just need to look the status of the first ECC Block in DMA1. */
    RecdTargetPSN = DmaLocation[0][DiscTypeIndex];
    if( dvdSeek(RecdTargetPSN, 0) == Error)
        return (FALSE);

    CurrentPSN = 0;
    while (CurrentPSN <= RecdTargetPSN)
    {
        CurrentPSN = READ_REG(HAL_DVDDREG_MAP->PID0);  /* Now, servo is following, we just need to wait for Target-On */
        ExitProcess();
    }

    if (HAL_GLOBAL_GetRfPresent() == 0)  /* 0: UNRECORDED_PSN, The DMA does not exist */
    {
        send_msg80(SEND_READ_DVD_MSG,0x71021B);
        InitializeDMA();
        return (TRUE);

    }

    /* Now read DMAs off the disc (DMA1,2,3,4) */
    for (DmaCount = 0; DmaCount < 4; DmaCount++)
    {
        for (EccCount = 0; EccCount <= 1; EccCount++)   /* EccCount = 0 is a DDS/PDL Block, = 1 is a SDL Block */
        {

            /* Prepare Buffer Manager HW for Buffer Loading. */

            /* Set up for DRAM buffer address where DMA from disc should be written to. */
            DataBuffer.CurDiscPtr  = 0 + DmaCount*0x20 + EccCount*0x10; /* put into scratch buffer, total pages = 8 */

            /* Prepare parameters for Buffer Loading. */
            dvdDiscRequest.Type             = INIT_READ;                                  /* LoadType; */
            dvdDiscRequest.TargetID.Long    = DmaLocation[DmaCount][DiscTypeIndex] + EccCount*0x10;   /* Address of DMA on disc */
            dvdDiscRequest.TargetLayer      = eLayer0;
            dvdDiscRequest.EccBlockCount    = 1L;      /* Read 1 ECC Block each time. */

            DiscReadStatus = dvdStartDiscTransfer();    /* Start Transfer of disc data into buffer and return status */

            if (DiscReadStatus == BUF_GOOD)
            {
                send_msg5SValue(SEND_READ_DVD_MSG,"DMAx ECC OK",1,(DmaCount+1));
                if (EccCount == 0)  /* This is a DDS/PDL Block */
                {
                    send_msg5S(SEND_READ_DVD_MSG, "DDS/PDL OK");
                    pDDS = (DDSTYPE *)(0 + DmaCount*0x20*DRAM_PAGE_SIZE);
                    if (pDDS->DdsIdentifier == 0x0A0A)  /* Check DDS identifier for validation */
                    {
                         BufGoodBitMap |= (1 << DmaCount);
                         DDSUpdateCounterInDDSBlock[DmaCount] = SWAPEND_ULONG(pDDS->DdsPdlUpdateCounter);      /*  */
                    }
                }
                else                /* This is a SDL Block */
                {
                    send_msg5S(SEND_READ_DVD_MSG, "SDL OK");
                    BufGoodBitMap |= (1 << (DmaCount + 4));
                    pSDL = (SDLTYPE *)((0x10 + DmaCount*0x20)*DRAM_PAGE_SIZE);
                    DDSUpdateCounterInSDLBlock[DmaCount] = SWAPEND_ULONG(pSDL->DdsPdlUpdateCounter);      /*  */
                    SDLUpdateCounter[DmaCount] = SWAPEND_ULONG(pSDL->SdlUpdateCounter);
                }
            }
        }
    }  /* End of for loop */

    #if 0   /* Test code for good pairs judgement algorithm */
    DDSUpdateCounterInDDSBlock[0] = 5;
    DDSUpdateCounterInDDSBlock[1] = 4;
    DDSUpdateCounterInDDSBlock[2] = 5;
    DDSUpdateCounterInDDSBlock[3] = 5;
    DDSUpdateCounterInSDLBlock[0] = 4;
    DDSUpdateCounterInSDLBlock[1] = 5;
    DDSUpdateCounterInSDLBlock[2] = 4;
    DDSUpdateCounterInSDLBlock[3] = 5;
    SDLUpdateCounter[0]           = 0;
    SDLUpdateCounter[1]           = 1;
    SDLUpdateCounter[2]           = 3;
    SDLUpdateCounter[3]           = 2;
    #endif  /* Test code for good pairs judgement algorithm */

    MaxDDSUpdateCounter1 = FindMaximum(DDSUpdateCounterInDDSBlock,4);
    MaxDDSUpdateCounter2 = FindMaximum(DDSUpdateCounterInSDLBlock,4);
    /* If a disc is initialized without certification, the MaxDDSUpdateCounter may be equal to zero, we should treat the DMA as good */
    if (MaxDDSUpdateCounter1 == MaxDDSUpdateCounter2 == 0)  /* We don't know this case is all updatecounter = 0 or all Block buffer fail */
    {
        if ( (BufGoodBitMap & 0x0F) && ((BufGoodBitMap >> 4) & 0x0F) )  /* at least have a good pair */
        {
            i = ConvertBitMap(&BufGoodBitMap);
            BufGoodBitMap >>= 4;
            j = ConvertBitMap(&BufGoodBitMap);

            CopyBufferToBuffer(         i*0x20, 0, (USHORT) START_DDS_PAGE, 0, 0x10);
            CopyBufferToBuffer((0x10 + j*0x20), 0, (USHORT) START_SDL_PAGE, 0, 0x10);

            if ((GetDiscCertificationFlag() & eInProgress) == eInProgress)   /* The previous format was not completed */
            {
                RamDiscStatus = eDiscInProgress;
                return (FALSE);
            }
            return (TRUE);
        }
        else
            send_msg80(SEND_READ_DVD_MSG,0x71021C);

    }

    /* Find the maximum value of the DDS/PDL update counter */
    /* The maximum value of the DDS/PDL update counter in DDS Block and SDL Block should be identical. */
    /* If they are not identical, then there is no good pair present. */
    index1 = index2 = 0;
    if ((MaxDDSUpdateCounter1 == MaxDDSUpdateCounter2) && (MaxDDSUpdateCounter1 != 0))  /* These are the good pairs */
    {
        /* Find the index of the good pairs */
        for (DmaCount = 0; DmaCount < 4; DmaCount++)
        {
            if (DDSUpdateCounterInDDSBlock[DmaCount] == MaxDDSUpdateCounter1)
            {
                GoodDDS[index1] = DmaCount;
                index1++;
            }
            if (DDSUpdateCounterInSDLBlock[DmaCount] == MaxDDSUpdateCounter1)
            {
                GoodSDL[index2] = DmaCount;
                NewSDLUpdateCounter[index2] = SDLUpdateCounter[DmaCount];
                index2++;
            }
        }

        /* GoodDDS[0] is we want, copy the whole ECC Block from scratch area to DRAM_START_DDS */
        // copy function, source page = 0 + GoodDDS[0]*0x20,
                      //(USHORT SourcePage, USHORT SourceOffset,               USHORT TargetPage, USHORT TargetOffset, USHORT PagesToCopy)
        CopyBufferToBuffer(GoodDDS[0]*0x20,                   0, (USHORT) START_DDS_PAGE,                   0,               0x10);

        /* GoodSDL[0] is not guaranteed as the right block that we need. */
        /* If there ara different value in GoodSDL[i], we will choice the largest one. */
        MaxSDLUpdateCounter = FindMaximum(NewSDLUpdateCounter,index2);

        for (i = 0; i < index2 ; i++)
            if (NewSDLUpdateCounter[i] == MaxSDLUpdateCounter)
                break;

        /* GoodSDL[i] is we want, copy the whole ECC Block from scratch area to DRAM_START_SDL */
        // copy function, source page = 0x10 + GoodSDL[i]*0x20
                        //(USHORT SourcePage       ,USHORT SourceOffset,               USHORT TargetPage, USHORT TargetOffset, USHORT PagesToCopy)
        CopyBufferToBuffer((0x10 + GoodSDL[i]*0x20),                  0, (USHORT) START_SDL_PAGE,                   0,              0x10);

        /* Check the In-progress field of the Disc certification flag */
        if ((GetDiscCertificationFlag() & eInProgress) == eInProgress)   /* The previous format was not completed */
        {
            RamDiscStatus = eDiscInProgress;
            rt = FALSE;
        }
    }
    else
    {
        /* DMA read fails, mark the disc is NG */
        RamDiscStatus = eDiscDMAFail;
        rt = FALSE;
    }

    send_msg5SValue(SEND_READ_DVD_MSG,"RamDiscStatus",1,RamDiscStatus);

    return rt;
}


/******************************************************************************************/
/*  Function:    FindMaximum()                                                            */
/*  Description: Find the maximum value of a Array                                        */
/*  Parameters:  ULONG *Array - Start address of a Array                                  */
/*               BYTE n       - the number of the Array element                           */
/*  Returned Value: MaxValue                                                              */
/******************************************************************************************/
ULONG FindMaximum(ULONG *Array, BYTE n)
{
    ULONG MaxValue;
    BYTE  i;

    MaxValue = Array[0];
    for (i = 0; i < n -1; i++)
        if (Array[i+1] > MaxValue)
            MaxValue = Array[i+1];

    return  MaxValue;
}


/****************************************************************************************/
/*  AddToDefectList()                                                                   */
/*                                                                                   */
/*  Parameter : PSN Value                                                            */
/*  Return    : None                                                                 */
/*  Description : The Defect List is registered sequentially when certification, and    */
/*              we will store them in a temp area. When cetification is finished, we    */
/*              will register them to the PDL Entry.                                    */
/*              We also need to save the total defect number in the header of the list. */
/*              The Defect List has a Max Entry = Max_PDL_Entry + Max_SDL_Entry*16      */
/****************************************************************************************/
void AddToDefectList(ULONG AddedPSN)
{
    ULONG   *pTemp;
    ULONG   TotalDefectNumber;

    pTemp = (ULONG *) DEFECT_LIST_START_ADDR;
    TotalDefectNumber = SWAPEND_ULONG(*pTemp);

    pTemp = (ULONG *) (DEFECT_LIST_START_ADDR + TotalDefectNumber*4);  //Point to Last Entry
    pTemp++;  //Apend
    *pTemp = SWAPEND_ULONG(AddedPSN);
    SetTotalDefectNumber(TotalDefectNumber + 1);
}


/*************************************************************************************/
/*  AddToPdlEntry                                                                    */
/*                                                                                   */
/*  Description : Add an Entry to PDL                                                */
/*  Parameter : AddedPSN - sector number of the defective sector                     */
/*  Return    : TRUE - successful                                                    */
/*              FALSE - PDL Entry is full                                            */
/*  Note : DefPSN must be added by ascending order. The original PDL may contain     */
/*         P-List. So, when we need to insert a Entry, we must have a Temp space to  */
/*         save and restore data.                                                    */
/*  Example:                                                                         */
/*             PSN    Entry                                                          */
/*                   |  .  |                                                         */
/*             105   |  A  |                                                         */
/*             109   |  B  |<- we want to insert a new entry 'X' here, its PSN =107. */
/*                   |  .  |                                                         */
/*                   | END |                                                         */
/*************************************************************************************/
BOOL AddToPdlEntry(ULONG AddedPSN)  //The AddedPSN has contained ENTRY_TYPE
{
    ULONG   *pTemp;
    ULONG   *BufferPtrSrc, *BufferPtrDest;
    ULONG   LastPSN;
    USHORT  length,i;

    /* If PDL Entry is full */
    if (GetTotalPdlEntry() >= MAX_PDL_ENTRY)
        return FALSE;

    /* If the PSN to be added has been in the PDL list, then that is a duplicate input. We will ignore it. */
    if (SearchPdlEntry2(AddedPSN & 0x00FFFFFF) == FALSE)
    {
        if (GetTotalPdlEntry() == 0)   /* X is the first entry */
        {
            pTemp = (ULONG *) DRAM_FIRST_PDL_ENTRY;
            *pTemp = SWAPEND_ULONG(AddedPSN);
            SetTotalPdlEntry(1);
        }
        else
        {
            pTemp = (ULONG *) (DRAM_START_PDL + GetTotalPdlEntry()*4);
            LastPSN = SWAPEND_ULONG(*pTemp) & 0x00FFFFFF;    /* The PSN of the last Entry */
            /* If X > END, we just need to append X behind the END */
            if ((AddedPSN & 0x00FFFFFF) > LastPSN)
            {
                pTemp++;
                *pTemp = SWAPEND_ULONG(AddedPSN);
                /* Total Entry Number increase, write back to DRAM */
                SetTotalPdlEntry(GetTotalPdlEntry() + 1);
            }
            else/*  X < END */
            {
                /* Find the location of X */
                pTemp = SearchPdlEntry(AddedPSN & 0x00FFFFFF);      /* In our example, pTemp should point to 'B' */

                /* copy Entry B-End to TEMP page */
                length = (USHORT) (pTemp - (ULONG *)(DRAM_FIRST_PDL_ENTRY));   /* The Entries from first to 'B' */
                BufferPtrSrc = pTemp;
                BufferPtrDest = (ULONG *) (DVD_RAM_TEMP_ADDR);
                for (i = 0; i < (GetTotalPdlEntry() - length ); i++)
                    *BufferPtrDest++ = *BufferPtrSrc++;

                /* Replace B with X */
                *pTemp = SWAPEND_ULONG(AddedPSN);
                pTemp++;

                /* move Entry B-Last back */
                BufferPtrSrc = (ULONG *) (DVD_RAM_TEMP_ADDR);
                BufferPtrDest = pTemp;
                for (i = 0; i < (GetTotalPdlEntry() - length); i++)
                    *BufferPtrDest++ = *BufferPtrSrc++;

                /* Total Entry Number increase, write back to DRAM */
                SetTotalPdlEntry(GetTotalPdlEntry() + 1);
            }
        }
    }
    return TRUE;
}


/*************************************************************************************/
/*  SearchPdlEntry2(LONG SearchPSN)                                                  */
/*                                                                                   */
/*  Parameter : PSN Value                                                            */
/*  Return    : TRUE  - if SearchPSN in the PDL Entry                                */
/*              FALSE - if SearchPSN Not in the PDL Entry                            */
/*************************************************************************************/
BOOL SearchPdlEntry2(LONG SearchPSN)
{
    ULONG   *PDLp;
    ULONG   Entry;
    ULONG   EntryPrevious;
    USHORT  AddressMask;

    PDLp     = (ULONG *)(DRAM_START_PDL + 4);        /* Get Address of first PDL Entry */

    AddressMask = 0x1000;

    /* Do a binary search. */
    do
    {
        PDLp += AddressMask ;
        PDLp--;
        EntryPrevious = SWAPEND_ULONG(*PDLp) & 0x00FFFFFF; /*  Get Previous PDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
        PDLp++;
        Entry = SWAPEND_ULONG(*PDLp) & 0x00FFFFFF; /*  Get Current PDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
        if (( Entry >= SearchPSN) || ((ULONG)PDLp > DRAM_END_PDL) )   /* The last term of the equation is needed for discs with PDL almost full.  */
        {
            if (EntryPrevious < SearchPSN)
                break;
            PDLp -= AddressMask;
        }
        AddressMask /= 2;
    }
    while (AddressMask > 0);

    if ((SearchPSN == Entry) || (SearchPSN == EntryPrevious))
        return (TRUE);
    else
        return (FALSE);
}


/*************************************************************************************/
/*  SearchSdlEntry2(LONG SearchPSN)                                                  */
/*                                                                                   */
/*  Parameter : PSN Value                                                            */
/*  Return    : TRUE  - if SearchPSN in the SDL Entry                                */
/*              FALSE - if SearchPSN Not in the SDL Entry                            */
/*************************************************************************************/
BOOL SearchSdlEntry2(LONG SearchPSN)
{
    ULONG   *SDLp;
    ULONG   Entry;
    ULONG   EntryPrevious;
    USHORT  AddressMask;

    SDLp            = (ULONG *)(DRAM_START_SDL + 24);       /* Get Address of first SDL Entry (Byte 24) */

    AddressMask = 0x1000;

    /* Do a binary search. */
    do
    {
        SDLp += AddressMask;
        SDLp--;
        SDLp--;
        EntryPrevious = SWAPEND_ULONG(*SDLp) & 0x00FFFFFF; /*  Get Previous SDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
        SDLp++;
        SDLp++;
        Entry = SWAPEND_ULONG(*SDLp) & 0x00FFFFFF; /*  Get Current SDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
        if (( Entry >= SearchPSN) || ((ULONG)SDLp > DRAM_END_SDL) )   /* The last term of the equation is needed for discs with SDL almost full.  */
        {
            if (EntryPrevious < SearchPSN)
                break;
            SDLp -= AddressMask;
        }
        AddressMask /= 2;
    }
    while (AddressMask > 1);        /* SDL has 64 bit entries as opposed to the 32 bit entries of PDL           */
                                    /*  the (1) in the while loop assures correct function for 64 bit entries.  */
    if ((SearchPSN == Entry) || (SearchPSN == EntryPrevious))
        return (TRUE);
    else
        return (FALSE);
}


/************************************************************************
// Set - DVD-RAM Total Defect Number when certification
*************************************************************************/
void SetTotalDefectNumber(ULONG num)
{
    ULONG *BufferPtr;

    BufferPtr = (ULONG *) DEFECT_LIST_START_ADDR;
    *BufferPtr = SWAPEND_ULONG(num);
}


/******************************************************************************************/
/*  Function:    SetTotalPdlEntry()                                                       */
/*  Description: Set the current PDL Entry number into DRAM                               */
/******************************************************************************************/
void SetTotalPdlEntry(USHORT EntryNumber)
{
    BYTE *BufferPtr;

    BufferPtr = (BYTE *) (DRAM_START_PDL + 2);
    *BufferPtr++ = BHIGH(EntryNumber);
    *BufferPtr   =  BLOW(EntryNumber);
}


/******************************************************************************************/
/*  Function:    SetTotalSdlEntry()                                                       */
/*  Description: Set the current SDL Entry number into DRAM                               */
/*      If SDL Entry is full or spare area is exhausted, we would not increase SDL Entry. */
/*      This function is called only when a SDL entry is added successfully.              */
/******************************************************************************************/
void SetTotalSdlEntry(USHORT EntryNumber)
{
    BYTE *BufferPtr;

    BufferPtr = (BYTE *) (DRAM_START_SDL + 22);
    *BufferPtr++ = BHIGH(EntryNumber);
    *BufferPtr   =  BLOW(EntryNumber);

    if (RWDisc.dvRAM.PrimarySARemainBlk != 0)
        RWDisc.dvRAM.PrimarySARemainBlk--;
    else
        RWDisc.dvRAM.SupSARemainBlk--;
}


/******************************************************************************************/
/*  Function:    GetTotalPdlEntry()                                                       */
/*  Description: Get the current PDL Entry number from DRAM                               */
/******************************************************************************************/
USHORT GetTotalPdlEntry(void)
{
    USHORT num;
    BYTE   *BufferPtr;

    BufferPtr = (BYTE *) (DRAM_START_PDL + 2);
    num = 0;
    num += (*BufferPtr) << 8;
    num += (*(BufferPtr + 1));

    return num;
}

/******************************************************************************************/
/*  Function:    GetTotalSdlEntry()                                                       */
/*  Description: Get the current SDL Entry number from DRAM                               */
/******************************************************************************************/
USHORT GetTotalSdlEntry(void)
{
    USHORT num;
    BYTE   *BufferPtr;

    BufferPtr = (BYTE *) (DRAM_START_SDL + 22);
    num = 0;
    num += (*BufferPtr) << 8;
    num += (*(BufferPtr + 1));

    return num;
}


/*************************************************************************************/
/*  ConvertBitMap()                                                                  */
/*  Description:Count the location of first non-zero bit                             */
/*  Parameter : BitMap  (8 bit)                                                      */
/*  Return    : BitCnt - the location of first non-zero bit                          */
/*************************************************************************************/
BYTE ConvertBitMap(BYTE *pBitMap)
{
    BYTE BitCnt = 0;

    while(1)
    {
        if ((*pBitMap) & 0x01)  /* count '1' */
            break;
        else                    /* count '0' */
            BitCnt++;

        (*pBitMap) >>= 1;
    }
    return BitCnt;
}


/*************************************************************************************/
/*  AddToSdlEntry(ULONG DefPSN, ULONG RepPSN)                                        */
/*                                                                                   */
/*  Parameter : DefPSN - sector number of the first sector in the defective Block    */
/*              RepPSN - sector number of the first sector in the replacement Block  */
/*  Return    : None                                                                 */
/*  Note      : DefPSN must be added by ascending order                              */
/*  Example:                                                                         */
/*             PSN    Entry                                                          */
/*                   |  .  |                                                         */
/*             105   |  A  |                                                         */
/*             199   |  B  |<- we want to insert a new entry 'X' here, its PSN =157. */
/*                   |  .  |                                                         */
/*                   | END |                                                         */
/*************************************************************************************/
BOOL AddToSdlEntry(ULONG DefPSN, ULONG RepPSN)
{
    ULONG   *pTemp;
    ULONG   *BufferPtrSrc, *BufferPtrDest;
    ULONG   LastPSN;
    USHORT  i, length;
    BYTE    Status;

    /* If SDL Entry is full */
    if (GetTotalSdlEntry() >= MAX_SDL_ENTRY)
        return FALSE;

    /* Record the SLR bit */
    if (RepPSN == 0)
        Status = eNotReplaced;
    else
        Status = eReplaced;

    DefPSN |= (Status << 24);

    /* If the PSN to be added has been in the SDL list, then that is a duplicate input. We will ignore it. */
    if (SearchSdlEntry2(DefPSN) == FALSE)
    {
        SaveReplaceInfo(DefPSN,RepPSN);
        if (GetTotalSdlEntry() == 0)   /* X is the first entry */
        {
            pTemp = (ULONG *) (DRAM_START_SDL + 24);
            *pTemp = SWAPEND_ULONG(DefPSN);
            pTemp++;
            *pTemp = SWAPEND_ULONG(RepPSN);
            SetTotalSdlEntry(1);
            return TRUE;
        }
        else
        {
            pTemp = (ULONG *) (DRAM_START_SDL + GetTotalSdlEntry()*8 + 16);
            LastPSN = SWAPEND_ULONG(*pTemp) & 0x00FFFFFF;    /* The DefPSN of the last Entry */
            /* If X > END, we just need to append X behind the END */
            if ((DefPSN & 0x00FFFFFF) > LastPSN)
            {
                pTemp += 2; /* Shift to next entry */
                *pTemp = SWAPEND_ULONG(DefPSN);
                pTemp++;
                *pTemp = SWAPEND_ULONG(RepPSN);
                SetTotalSdlEntry(GetTotalSdlEntry() + 1);
                return TRUE;
            }
            else/*  X < END, we will insert this entry */
            {   /* If A < X < B, we will Copy Entry B-End to TEMP page, */
                /* then replace B with X, and then copy Entry B-End from TEMP to append after X */

                /* Find the location of X */
                pTemp = SearchSdlEntry(DefPSN);      /* In our example, pTemp should point to 'B' */

                /* copy Entry B-End to TEMP page */
                length = (USHORT) ((pTemp - (ULONG *)(DRAM_FIRST_SDL_ENTRY))/2);   /* The Entries from first to 'B' */
                BufferPtrSrc = pTemp;
                BufferPtrDest = (ULONG *) (DVD_RAM_TEMP_ADDR);
                for (i = 0; i < (GetTotalSdlEntry() - length)*2; i++)
                    *BufferPtrDest++ = *BufferPtrSrc++;

                /* Replace B with X */
                *pTemp = SWAPEND_ULONG(DefPSN);
                pTemp++;
                *pTemp = SWAPEND_ULONG(RepPSN);
                pTemp++;

                /* move Entry B-Last back */
                BufferPtrSrc = (ULONG *) (DVD_RAM_TEMP_ADDR);
                BufferPtrDest = pTemp;
                for (i = 0; i < (GetTotalSdlEntry() - length)*2; i++)
                    *BufferPtrDest++ = *BufferPtrSrc++;

                /* Total Entry Number increase, write back to DRAM */
                SetTotalSdlEntry(GetTotalSdlEntry() + 1);
                return TRUE;
            }
        }

    }
    return FALSE;
}


/*************************************************************************************/
/*  AddToSdlEntryNew(ULONG DefPSN, ULONG RepPSN)                                     */
/*                                                                                   */
/*  Description: This function is a modification from AddToSdlEntry(), the main      */
/*    difference is that this function allows the added DefPSN can exist in original */
/*    SDL list. In this case, we will regard the input as a Re-Assign action.        */
/*               If a input is a Re-Assign of SDL Entry, the total SDL Entries will  */
/*    not be changed, only Replacement PSN is changed and Free Spare Blocks will     */
/*    decrease by one in the same time.                                              */
/*************************************************************************************/
BOOL AddToSdlEntryNew(ULONG DefPSN, ULONG RepPSN)
{
    ULONG   *pTemp,*SdlPtr;
    ULONG   *BufferPtrSrc, *BufferPtrDest;
    ULONG   LastPSN;
    ULONG   SdlEntry;
    USHORT  i, length;
    BYTE    Status;

    /* If SDL Entry is full */
    if (GetTotalSdlEntry() >= MAX_SDL_ENTRY)
        return FALSE;

    /* Record the SLR bit */
    if (RepPSN == 0)
        Status = eNotReplaced;
    else
        Status = eReplaced;

    DefPSN |= (Status << 24);

    SdlPtr = SearchSdlEntry(DefPSN);
    SdlEntry  = SWAPEND_ULONG(*SdlPtr) & 0x00FFFFFF;

    if (SdlEntry != (DefPSN & 0x00FFFFFF)) //This is a new Entry.
    {
        SaveReplaceInfo(DefPSN,RepPSN);
        if (GetTotalSdlEntry() == 0)   /* X is the first entry */
        {
            pTemp = (ULONG *) (DRAM_START_SDL + 24);
            *pTemp = SWAPEND_ULONG(DefPSN);
            pTemp++;
            *pTemp = SWAPEND_ULONG(RepPSN);
            SetTotalSdlEntry(1);
            return TRUE;
        }
        else
        {
            pTemp = (ULONG *) (DRAM_START_SDL + GetTotalSdlEntry()*8 + 16);
            LastPSN = SWAPEND_ULONG(*pTemp) & 0x00FFFFFF;    /* The DefPSN of the last Entry */
            /* If X > END, we just need to append X behind the END */
            if ((DefPSN & 0x00FFFFFF) > LastPSN)
            {
                pTemp += 2; /* Shift to next entry */
                *pTemp = SWAPEND_ULONG(DefPSN);
                pTemp++;
                *pTemp = SWAPEND_ULONG(RepPSN);
                SetTotalSdlEntry(GetTotalSdlEntry() + 1);
                return TRUE;
            }
            else/*  X < END, we will insert this entry */
            {   /* If A < X < B, we will Copy Entry B-End to TEMP page, */
                /* then replace B with X, and then copy Entry B-End from TEMP to append after X */

                /* Find the location of X */
                pTemp = SdlPtr;      /* In our example, pTemp should point to 'B' */

                /* copy Entry B-End to TEMP page */
                length = (USHORT) ((pTemp - (ULONG *)(DRAM_FIRST_SDL_ENTRY))/2);   /* The Entries from first to 'B' */
                BufferPtrSrc = pTemp;
                BufferPtrDest = (ULONG *) (DVD_RAM_TEMP_ADDR);
                for (i = 0; i < (GetTotalSdlEntry() - length)*2; i++)
                    *BufferPtrDest++ = *BufferPtrSrc++;

                /* Replace B with X */
                *pTemp = SWAPEND_ULONG(DefPSN);
                pTemp++;
                *pTemp = SWAPEND_ULONG(RepPSN);
                pTemp++;

                /* move Entry B-Last back */
                BufferPtrSrc = (ULONG *) (DVD_RAM_TEMP_ADDR);
                BufferPtrDest = pTemp;
                for (i = 0; i < (GetTotalSdlEntry() - length)*2; i++)
                    *BufferPtrDest++ = *BufferPtrSrc++;

                /* Total Entry Number increase, write back to DRAM */
                SetTotalSdlEntry(GetTotalSdlEntry() + 1);
                return TRUE;
            }
        }

    }
    else
    /* The PSN to be added has been in the SDL list, then that is a Re-Assign. */
    {
        SdlPtr++;  //move to RepPSN
        *SdlPtr = SWAPEND_ULONG(RepPSN);

        if (RWDisc.dvRAM.PrimarySARemainBlk != 0)
            RWDisc.dvRAM.PrimarySARemainBlk--;
        else
            RWDisc.dvRAM.SupSARemainBlk--;

        return TRUE;
    }
}


/*************************************************************************************/
/*                                                                                   */
/*  CalcStartLSNofZone()                                                             */
/*                                                                                   */
/*  Function calculates and stores the start LSN of each zone.                       */
/*  Only DVD-RAM version 2.x needs to do this.                                       */
/*                                                                                   */
/*  According to DVD-RAM Version 2.1 SPEC., Annex R, page PHX-R16,                   */
/*  The first LSN in the zone = X - Y + [(Z+15)/16]*16 + 1                           */
/*  where X = the last LSN in the zone,                                              */
/*        Y = the total number of sectors in the zone except for the Guard area,     */
/*        Z = the number of PDL entries belonging to the zone.                       */
/*************************************************************************************/
void CalcStartLSNofZone(void)
{
    ULONG LastLSN[35];
    ULONG TotalPSN;
    ULONG TempStartPSN;
    USHORT NumofPDL;
    BYTE i;

    LastLSN[LocalZoneNumber - 1] = DVDRAM_LAST_LSN;  /* last LSN of zone 34 is a fixed value */
    for(i = (LocalZoneNumber - 1); i >= 1; i--)
    {
        TotalPSN = DVDRAMZoneTable[i].EndUserSectorPSN - DVDRAMZoneTable[i].StartUserSectorPSN +1;
        NumofPDL = CalcPDLNumberBetween2PSN(DVDRAMZoneTable[i].StartUserSectorPSN, DVDRAMZoneTable[i].EndUserSectorPSN);
        TempStartPSN = LastLSN[i] - TotalPSN + ((NumofPDL+15)/16)*16 + 1;
        RWDisc.dvRAM.StDDSp->StartLsn[i] = SWAPEND_ULONG(TempStartPSN);
        LastLSN[i-1] = TempStartPSN - 1;
    }
    RWDisc.dvRAM.StDDSp->StartLsn[0] = 0;
}


/****************************************************************************************/
/*                                                                                      */
/*  CalcPSNofLSN0()                                                                     */
/*                                                                                      */
/*  Function calculates the PSN at LSN=0                                                */
/*  Only DVD-RAM version 2.x needs to do this.                                          */
/*                                                                                      */
/*  According to DVD-RAM Version 2.1 SPEC., Annex R, page PHX-R16,                      */
/*  P = 0x31000, Q = 0x31000 - {X - Y + [(Z+15)/16]*16 + 1}                             */
/*  where X = the last LSN in Zone0,                                                    */
/*        Y = the total number of sectors in Zone0 except for the Guard area,           */
/*        Z = the number of PDL entries belonging to Zone0.                             */
/*  At Each iteration, P = Q +1 and Q = Q + R                                           */
/*  where R = the number of PDL entries between P and Q                                 */
/*  The iteration is continued until R=0, then Current Q is the sector number of LSN=0. */
/*                                                                                      */
/*      Parameter: None                                                                 */
/*      Return   : CurrentPSN                                                           */
/****************************************************************************************/
void CalcPSNofLSN0(void)
{
    ULONG StartPSN, CurrentPSN;
    USHORT NumofPDLZone0, RemainPDL;

    StartPSN = ZONE0_START_PSN;
    NumofPDLZone0 = CalcPDLNumberBetween2PSN(ZONE0_START_PSN,ZONE0_END_PSN);
    CurrentPSN = ZONE0_START_PSN - ((SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[1]) - 1)
               - PHYSICAL_SECTOR_NUMBER_OF_ZONE0 + ((NumofPDLZone0+15)/16)*16 + 1);  /* This is 'Q' */

    while(1)
    {
        RemainPDL = CalcPDLNumberBetween2PSN(StartPSN,CurrentPSN);  /* This is 'R' */
        if (RemainPDL == 0)
            break;
        StartPSN = CurrentPSN + 1;  /* P = Q +1 */
        CurrentPSN += RemainPDL;    /* Q = Q +R */
    }

    RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0 = SWAPEND_ULONG(CurrentPSN);
    /*Update sector number of the last sector in the Primary space area*/
    RWDisc.dvRAM.StDDSp->LocationOfPrimarySpareAreaLOW  = SWAPEND_ULONG(CurrentPSN -1);

}


/******************************************************************************************/
/*  Function:    GetDiscCertificationFlag()                                               */
/*  Description: Return DVD-RAM disc certification flag                                   */
/*                   b7           b6 - b2            b1                    b0             */
/*               In-progress      reserved   User certification  Disc manufacturer Cert.  */
/*  Parameters:  None                                                                     */
/*  Returned  :  DiscCertificationFlag                                                    */
/******************************************************************************************/
BYTE GetDiscCertificationFlag(void) // TBD? change to inline code or macro!
{
    return RWDisc.dvRAM.StDDSp->DiscCertificationFlag;
}


/************************************************************************
 Name:       InitialProtectionPage()

 Description: Pre-Fill Protection page as 0xFFFFFFFFL. This page is used for
              PDL search protection when PDL entry is full.
              If there is no this protection, a return pointer of SearchPdlEntry()
              may point to the head of the SDL Entry.
 Returns:     None
*************************************************************************/
void InitialProtectionPage(void)
{
    ULONG *BufferPtr;
    ULONG Count;

    BufferPtr = (ULONG *)DRAM_START_PROTECT_ADDR;
    for (Count = 0; Count < (0x800/4); Count++)
    {
       *BufferPtr++ = 0xFFFFFFFFL;
    }
}

/********************************************************************************************/
/*  DVDRAMVarsInit()                                                                        */
/*                                                                                          */
/*  Input     : None                                                                        */
/*  Return    : None                                                                        */
/*  Description : Initialize DVD-RAM variables prior to spin up                             */
/********************************************************************************************/
void DVDRAMVarsInit(void)
{
    /* Set default DVD-RAM disc as 12CM & Version 2.0 */
    DVDRAM_12CM     = TRUE;
    DiscTypeIndex   = LOC_DISC_TYPE_20;
    LocalZoneNumber = NUM_ZONES_RAM_20;
    memcpy(DVDRAMZoneTable, DVDRAMZoneTableV20, sizeof(DVDRAMZoneTableV20));

    /* Initialize DVD-RAM variables */
    RWDisc.dvRAM.StDDSp = (DDSTYPE *)DRAM_START_DDS;
    RWDisc.dvRAM.StPDLp = (PDLTYPE *)DRAM_START_PDL;
    RWDisc.dvRAM.StSDLp = (SDLTYPE *)DRAM_START_SDL;

    RWDisc.dvRAM.DecodeMode = eNOT_RAM_MODE;
    RWDisc.dvRAM.ReplaceMode = eNON_REPLACEMENT;
    RamDiscStatus = 0;

    RWDisc.dvRAM.ReadDataMode = eNOT_USERDATA;
    RWDisc.dvRAM.BlankDMA = CLEAR;

    RWDisc.dvRAM.BgPreBuffer = 0;

    RWDisc.dvRAM.BgPreBufAbort = 0;
    #if DvdRamFastCache
    RWDisc.dvRAM.CacheFull = 0;
    #endif
    RWDisc.dvRAM.AbexMid = 0;
    RWDisc.dvRAM.SdlUpdateFlag = FALSE;
    #if (DISCARD_RAM_DECODE == 1)
    RWDisc.dvRAM.ContinuteIceCount = 0;
    #endif
}

/*----------------------------------------------------------------------------
  Name: RamInitializeVar
  Description:
            Initializes all the DVD-RAM related variables
  Input:  None
  Output: None
----------------------------------------------------------------------------*/
void RamInitializeVar(void)
{
    /* initialize dvRAM variables */
    RWDisc.dvRAM.DefectiveBitMap = 0;
    RWDisc.dvRAM.CommandExe = 0;
    RWDisc.dvRAM.Stream = eRamNoStream;
    RWDisc.dvRAM.ReplaceMode = eLINEAR_REPLACEMENT;
    RWDisc.dvRAM.ReadSkip = 0;

    pdbSetDVDMaxLBA(GetDVDRAMMaxLBA()); //used for vendor command
    ClearDataBuffer();

    SpeedCount = 0;// is it ncessary?

    WRITE_FIELD(HAL_DVDD_SLIPEN, 1);   // Enable the Sector Slip mechanism in the decoder.

    RAMcmInit();
    RWDisc.dvRAM.ContinuteReadCount = 0;
    RWDisc.dvRAM.PrevCommandIsRead  = CLEAR;
    RWDisc.dvRAM.PrevCommandIsWrite = CLEAR;
    RWDisc.dvRAM.WriteSpareArea = CLEAR;
    RWDisc.dvRAM.StreamWriteDefLSN  = 0;
    RWDisc.dvRAM.BgPreBuffer = 0;

    #if DvdRamFastCache
    RWDisc.dvRAM.CacheFull = 0;
    #endif
    #if (DISCARD_RAM_DECODE == 1)
    RWDisc.dvRAM.ContinuteIceCount = 0;
    #endif

}


/*----------------------------------------------------------------------------
  Name: SaveReplaceInfo
  Description:
            When the Block is detected as defect, Save the relative info.
  Input:  DefPSN - Defect Block Start PSN
          RepPSN - Replacement Block Start PSN
  Output: None
----------------------------------------------------------------------------*/
void SaveReplaceInfo(ULONG DefBlkPSN,ULONG RepBlkPSN)
{
    if (RWDisc.dvRAM.ReplaceMode == eLINEAR_REPLACEMENT)
    {
        RepInfo[IncreasedSdlCount].DefPSN = DefBlkPSN & 0x00FFFFFF;
        RepInfo[IncreasedSdlCount].RepPSN = RepBlkPSN & 0x00FFFFFF;
        if (!(RWDisc.dvRAM.Stream & eRamWriteStream))
            IncreasedSdlCount++;
        RWDisc.dvRAM.SdlUpdateFlag = TRUE;
    }
}


/******************************************************************************************/
/*  Function:    GetDVDRAMMaxLBA()                                                        */
/*  Description: Get the MaxLBA for DVD-RAM disc                                          */
/******************************************************************************************/
ULONG GetDVDRAMMaxLBA(void)
{
    //send_msg5SValue(SEND_MRW_MSG, "RamMxLBA", 4, SWAPEND_ULONG(RWDisc.dvRAM.StSDLp->NumberOfLSN));
    return (SWAPEND_ULONG(RWDisc.dvRAM.StSDLp->NumberOfLSN) - 1);
}


/*************************************************************************************/
/*  InitializeDMA()                                                                  */
/*                                                                                   */
/*  Function    : Initialize the contents of the DMA in DRAM when DMA does not exist */
/*  Parameter   : None                                                               */
/*  Return      : None                                                               */
/*************************************************************************************/
void InitializeDMA(void)
{
    InitializeDDS();

    /* Prepare PDL sectors */
    /* We pre-fill PDL & SDL sectors with FFh pattern */
  //LinearPatternFill(        USHORT trgpage, USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern)
    LinearPatternFill((USHORT)START_PDL_PAGE,               0,           32,        0x800/2,         0xFFFF);  /* PDL + Protect + SDL = 32 pages */

    RWDisc.dvRAM.StPDLp->PdlIdentifier           = SWAPEND_WORD(0x0001);
    RWDisc.dvRAM.StPDLp->NumberOfPdlEntries      = 0x0;

    /* Prepare SDL sectors */
    RWDisc.dvRAM.StSDLp->SdlIdentifier           = SWAPEND_WORD(0x0002);
    RWDisc.dvRAM.StSDLp->reserved1               = 0x0;
    RWDisc.dvRAM.StSDLp->SdlUpdateCounter        = 0x0;
    RWDisc.dvRAM.StSDLp->StartPSNOfSupSpareArea  = 0x0;
    if (format_size == 0)
        RWDisc.dvRAM.StSDLp->NumberOfLSN         = SWAPEND_ULONG(0x222120);
    else
        RWDisc.dvRAM.StSDLp->NumberOfLSN         = SWAPEND_ULONG(format_size);
    RWDisc.dvRAM.StSDLp->DdsPdlUpdateCounter     = 0x0;

    send_msg5SValue(SEND_MRW_MSG, "FmtSize", 4, format_size);
    send_msg5SValue(SEND_MRW_MSG, "LsnSize", 4, SWAPEND_ULONG(RWDisc.dvRAM.StSDLp->NumberOfLSN));

    if (format_size < DVDRAM_TOTAL_LSN)  //SSA is allocated
    {
        RWDisc.dvRAM.StSDLp->SpareAreaFullFlag       = 0x0;
        RWDisc.dvRAM.SupSARemainBlk = (DVDRAM_TOTAL_LSN - format_size)/ONE_DVD_BLOCK;
    }
    else  //SSA is NOT allocated
    {
        RWDisc.dvRAM.StSDLp->SpareAreaFullFlag       = 0x02; //SSA full
        RWDisc.dvRAM.SupSARemainBlk = 0;
    }

    RWDisc.dvRAM.StSDLp->reserved2               = 0x0;
    RWDisc.dvRAM.StSDLp->NumberOfSdlEntries      = 0x0;
}

/*************************************************************************************/
/*  InitializeDDS()                                                                  */
/*                                                                                   */
/*  Function    : Initialize the contents of the DDS in DRAM */
/*  Parameter   : None                                                               */
/*  Return      : None                                                               */
/*************************************************************************************/
void InitializeDDS(void)//huang test 970411
{
    BYTE zone;

    /* Prepare DDS sector */
  //LinearPatternFill(        USHORT trgpage, USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern)
    LinearPatternFill((USHORT)START_DDS_PAGE,               0,            1,        0x800/2,         0x0000);  /* DDS = 1 pages */

    RWDisc.dvRAM.StDDSp->DdsIdentifier  = 0x0A0A;
    RWDisc.dvRAM.StDDSp->DiscCertificationFlag = 0x0;
    RWDisc.dvRAM.StDDSp->DdsPdlUpdateCounter = 0x0;
    RWDisc.dvRAM.StDDSp->NumberOfGroups = SWAPEND_WORD(0x0001);

    if(DVDRAM_12CM)
        RWDisc.dvRAM.StDDSp->NumberOfZones  = SWAPEND_WORD(0x0023);  /* 35 Zones */
    else
        RWDisc.dvRAM.StDDSp->NumberOfZones  = SWAPEND_WORD(0x000E);  /* 14 Zones */

    RWDisc.dvRAM.StDDSp->LocationOfPrimarySpareAreaHIGH = SWAPEND_ULONG(0x31000);

    if(DVDRAM_12CM)
        RWDisc.dvRAM.StDDSp->LocationOfPrimarySpareAreaLOW  = SWAPEND_ULONG(0x341FF);
    else
        RWDisc.dvRAM.StDDSp->LocationOfPrimarySpareAreaLOW  = SWAPEND_ULONG(0x323FF);

    /* Assume it is defect free */
    RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0 = SWAPEND_ULONG(0x34200);

    /* Initial Start LSN for the Zone*/
    for(zone = 0; zone < LocalZoneNumber; zone++)
        RWDisc.dvRAM.StDDSp->StartLsn[zone] = SWAPEND_ULONG(DVDRAMZoneTable[zone].StartSectorLsn);
}

BOOL  RamCheckZoneBoundary(ULONG InputLba)
{
    BYTE rt,zone;

    rt = FALSE;
    for(zone = 0; zone < LocalZoneNumber; zone++)
    {
        if (InputLba == DVDRAMZoneTable[zone].EndUserSectorPSN)
        {
            rt = TRUE;
            break;
        }
    }

    return rt;
}

    #if 0
void GetDvdramAbexIdCode(void)
{
    char targetMSup[9];

    memset(targetMSup,0,5);
    memcpy(targetMSup,Info.Media.DVD.PFInfoP->Disc.Ram.DiscManSupplInfo,4);
    send_msg5S(1, &targetMSup[0]);

    if (strcmp(&targetMSup[0],"M01J") == 0)
    {
        if (svoVar_discBlank == eBlankDisc)
             RWDisc.dvRAM.AbexMid = 0;
        else if (svoVar_discBlank == eRecordedDisc)
        {
            if (svoVar_eccDiscNum == eECC_140_um)
                RWDisc.dvRAM.AbexMid = eDVDRAM_TDV_RA513CW;
            else
                RWDisc.dvRAM.AbexMid = eDVDRAM_TDV_RA52x_533CW;
        }
    }
    else
        RWDisc.dvRAM.AbexMid = 0;
}
    #endif
#endif // #if (DVD_RAM_READ == 1)


/*************************************************************************************/
/*  diaCmdDvdram()                                                                   */
/*                                                                                   */
/*  Function    : Show the contents of PDL and SDL Tables                            */
/*  Parameter   : Iram_Cmd_block.bf[1] - Subfunction                                 */
/*                case 0 : PDL Table                                                 */
/*                case 1 : SDL Table                                                 */
/*  Return      : None                                                               */
/*************************************************************************************/
void diaCmdDvdram(void)
{
#if (DVD_RAM_READ == 1)
    BYTE i;

    switch (Iram_Cmd_block.bf[1] & 0x7F)
    {
        case 0:       /* Send PDL Table to Host */
           send_msg80(1,0x71021D);

            for(i = 0; i <= 14; i++)  //PDL max size = 15 sectors
                SendParameterDataToHost(START_PDL_PAGE*0x800 + i*0x800, 0x800, FALSE);     // No Command Complete
            AutoCommandComplete();
            break;

        case 1:       /* Send SDL Table to Host */
            send_msg80(1,0x71021E);
            for(i = 0; i <= 14; i++)  //SDL max size = 15 sectors
                SendParameterDataToHost(START_SDL_PAGE*0x800 + i*0x800, 0x800, FALSE);     // No Command Complete
            AutoCommandComplete();
            break;

        default:
            BUILD_SENSE(0x05,0x20,0x00,0x69);    /* INVALID COMMAND OPERATION CODE */
            AutoCommandComplete();
    }
#endif // #if (DVD_RAM_READ == 1)
}
