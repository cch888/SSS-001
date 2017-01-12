/************************** SUNEXT CONFIDENTIAL *********************************
*           (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
*
*   FILE NAME:     $Workfile: dvDefMgr.h $
*
*   DESCRIPTION:    Header file for Defect Management
*
*
*   FUNCTIONS:
*
*   $Revision: 4 $
*   $Date: 10/02/21 11:47a $
*
************************** SUNEXT CONFIDENTIAL *********************************/

#ifndef __DVDEFMGR_H__
#define __DVDEFMGR_H__

#include ".\hif\bf_util.h"
#include ".\dvd\dvDiscRd.h"
#include ".\defmgmt\dvdram\ramdef.h"

#if (DVD_RAM_READ == 1)

/**************************************************************************
   DEFINES
**************************************************************************/
#define FRAG_LIST_LENGTH       8

typedef struct
{
    BYTE   SectorsPerRev;                   /* Number of Sectors per revolution                               */
    ULONG  StartSectorPSN;                  /* Start Sector Number of Each Zone                               */
    ULONG  StartUserSectorPSN;              /* Physical Sector Number (PSN) of First sector # of User Area.   */
    ULONG  EndUserSectorPSN;                /* Physical Sector Number (PSN) of Last  sector # of User Area.   */
    ULONG  EndSectorPSN;                    /* End Sector Number of Each Zone                                 */
    ULONG  StartSectorLsn;                  /* Logical  Sector Number (LSN) of the first sector in the group. */
} StructureOfGroupsType;

typedef struct
{
    ULONG  ServoPSN;
    ULONG  HostLba;
    ULONG  Length;
    FLAG   SdlBlock;
} FragListEntryType;

typedef struct
{
    USHORT Index;
    USHORT ExtAvailable;
    FragListEntryType Entry[FRAG_LIST_LENGTH];
} FragListType;

typedef struct
{
    ULONG  DefPSN;
    ULONG  RepPSN;
} ReplaceBlockType;

typedef struct
{
    /* Disc Definition Structure */
    WORD   DdsIdentifier;                   /* Bp 0-1  ,DDS Identifier (ALWAYS 0A0Ah FOR Ver. 2.0) */
    BYTE   reserved1;                       /* Bp 2                                                */
    BYTE   DiscCertificationFlag;           /* Bp 3    ,Disc certification flag.                   */
    ULONG  DdsPdlUpdateCounter;             /* Bp 4-7  ,DDS/PDL update counter.                    */
    WORD   NumberOfGroups;                  /* Bp 8-9  ,Number of groups.                          */
    WORD   NumberOfZones;                   /* Bp 10-11,Number of zones (35)                       */
    BYTE   reserved2[79-12+1];              /* Bp 12-79                                            */
    ULONG  LocationOfPrimarySpareAreaHIGH;  /* Bp 80-83,Location of Primary spare area. High bytes */
    ULONG  LocationOfPrimarySpareAreaLOW;   /* Bp 84-87,Location of Primary spare area. Low bytes  */
    ULONG  PhysicalLocationOfLsn0;          /* Bp 88-91,Location of LSN 0   (a PSN address)        */
    BYTE   reserved3[255-92+1];             /* Bp 92-255                                           */
    ULONG  StartLsn[35];                    /* Bp 256-395,Start LSN for zones 0 to 34              */
} DDSTYPE;

typedef struct
{
    WORD   PdlIdentifier;                   /* Bp 0-1   */
    WORD   NumberOfPdlEntries;              /* Bp 2-3   */
} PDLTYPE;

typedef struct
{
    /* SDL */
    WORD   SdlIdentifier;                   /* Bp 0-1   */
    WORD   reserved1;
    ULONG  SdlUpdateCounter;                /* Bp 4-7   */
    ULONG  StartPSNOfSupSpareArea;          /* Bp 8-11  */
    ULONG  NumberOfLSN;                     /* Bp 12-15 */
    ULONG  DdsPdlUpdateCounter;             /* Bp 16-19 */
    BYTE   SpareAreaFullFlag;               /* Bp 20    */
    BYTE   reserved2;                       /* Bp 21    */
    WORD   NumberOfSdlEntries;              /* Bp 22-23 */
} SDLTYPE;


/*********************************************************************
 PUBLIC FUNCTION PROTOTYPES
*********************************************************************/
extern DiscAccessStatusE  FragmentizeReadForDvdRam(ULONG Lba, WORD Length);
extern BOOL               DvdRamStartUp(void);
extern void               CalcPSNofLSN0(void);
extern BOOL               AddToSdlEntry(ULONG, ULONG);
extern BOOL               AddToSdlEntryNew(ULONG DefPSN, ULONG RepPSN);
extern void               AddToDefectList(ULONG);
extern BOOL               AddToPdlEntry(ULONG AddedPSN);
extern USHORT             GetTotalPdlEntry(void);
extern USHORT             GetTotalSdlEntry(void);
extern void               SetTotalPdlEntry(USHORT EntryNumber);
extern void               SetTotalSdlEntry(USHORT EntryNumber);
extern void               CalcStartLSNofZone(void);
extern void               DVDRAMVarsInit(void);
extern ULONG              GetDVDRAMMaxLBA(void);
extern void               InitializeDMA(void);
extern void               InitializeDDS(void);


/*********************************************************************
 PUBLIC VARIABLES
*********************************************************************/
extern const StructureOfGroupsType DVDRAMZoneTableV10[NUM_ZONES_RAM_10];
extern const StructureOfGroupsType DVDRAMZoneTableV20[NUM_ZONES_RAM_20];
extern const StructureOfGroupsType DVDRAMZoneTableV20_8CM[NUM_ZONES_RAM_20_8CM];
extern CONST            StructureOfGroupsType StructureOfGroups[3][NUM_ZONES_RAM_20];
extern ReplaceBlockType RepInfo[MAX_DVD_BLOCKS];
extern BYTE             IncreasedSdlCount;
extern BYTE             DiscTypeIndex;
extern       StructureOfGroupsType DVDRAMZoneTable[NUM_ZONES_RAM_20];

    #endif // #if (DVD_RAM_READ == 1)

#endif // #ifndef __DVDEFMGR_H__
