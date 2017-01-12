/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   BdDfmg.H
*
* DESCRIPTION:  Header file for BdDfmg.C
*
* NOTES:
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _BDDFMG_H_
#define _BDDFMG_H_



#include ".\common\sdram.h"
#include ".\common\chip.h"
#include ".\common\globals.h"
#include ".\bd\bdDef.h"



/*********************************************************************************************************
           BD DFMG Memory Layout
*********************************************************************************************************/
/*
Assembly Dfl        |   page = 0h   -  FFh (8 clusters = 8*20h = 100h pages)
Temp RAD/SPR Area   |   page = 100h - 1FFh
Temp NRD/UNU Area   |   page = 200h - 2FFh
Temp PBA Area       |   page = 300h - 3FFh
Work Area           |   page = 540h - 75Fh
Dfl Pool            |   page = 540h - 66Bh
Common Area         |   page = 760h - 7FF
*/


/*********************************************************************************************************
           BD DFL POOL
*********************************************************************************************************/
#define BD_DFL_POOL_START_ADDR           BD_DFL_ADR
#define BD_DFL_POOL_START_PAGE           ((BD_DFL_POOL_START_ADDR - SCSDRAM_ADDRESS) / 0x800)
#define BD_HASH_TABLE_DA_SIZE            (7*0x800)                 //7 pages
#define BD_HASH_TABLE_SA_SIZE            (1*0x800)                 //1 pages
#define BD_HASH_TABLE_DA_0_START         BD_DFL_POOL_START_ADDR
#define BD_HASH_TABLE_DA_1_START         (BD_HASH_TABLE_DA_0_START + BD_HASH_TABLE_DA_SIZE)
#define BD_HASH_TABLE_ISA_0_START        (BD_HASH_TABLE_DA_1_START + BD_HASH_TABLE_DA_SIZE)
#define BD_HASH_TABLE_OSA_0_START        (BD_HASH_TABLE_ISA_0_START + BD_HASH_TABLE_SA_SIZE)
#define BD_HASH_TABLE_OSA_1_START        (BD_HASH_TABLE_OSA_0_START + BD_HASH_TABLE_SA_SIZE)
#define BD_HASH_TABLE_ISA_1_START        (BD_HASH_TABLE_OSA_1_START + BD_HASH_TABLE_SA_SIZE)
#define BD_DFL_ENTRY_START               (BD_HASH_TABLE_ISA_1_START + BD_HASH_TABLE_SA_SIZE)

#if (SDRAM_SIZE_MBYTE == 4)
#define BD_TOTAL_DFL_POOL_SIZE           (300*2048)     //300 pages
#else
#define BD_TOTAL_DFL_POOL_SIZE           (140*2048)     //140 pages
#endif

#define BD_TOTAL_HASH_TABLE_SIZE         (BD_HASH_TABLE_DA_SIZE*2 + BD_HASH_TABLE_SA_SIZE*4) //2 DA and 4 SA
#define BD_TOTAL_ENTRIES_SIZE            (BD_TOTAL_DFL_POOL_SIZE - BD_TOTAL_HASH_TABLE_SIZE) //(300-18)*2048 bytes

#define eBdDflHashPower                        11

/*********************************************************************************************************
           BD DFL ASSEMBLY
*********************************************************************************************************/
#define BD_ASSEMBLY_DFL_HEADER_START     (SCSDRAM)
#define BD_ASSEMBLY_DFL_START_PAGE       0
#define BD_ASSEMBLY_DFL_AREA_SIZE        0x100     // 8 clusters
#define BD_ASSEMBLY_DFL_ENTRY_START      (BD_ASSEMBLY_DFL_HEADER_START + BD_DFL_HEADER_LEN)
#define BD_TEMP_RAD_COLLECT_START        (SCSDRAM + 0x080000) //100h page
#define BD_TEMP_NRD_COLLECT_START        (SCSDRAM + 0x100000) //200h page
#define BD_TEMP_PBA_COLLECT_START        (SCSDRAM + 0x180000) //300th page
#define BD_TEMP_SPR_COLLECT_START        BD_TEMP_RAD_COLLECT_START  // RAD Area is re-used for SPR
#define BD_TEMP_UNU_COLLECT_START        BD_TEMP_NRD_COLLECT_START  // NRD Area is re-used for UNU

/*********************************************************************************************************
           B D D F L T Y P E S . H
*********************************************************************************************************/
// Status1 defines.
#define DFL_STAT1_RAD                   0x00  /* 0000 BD-R/E Defect Cluster with Replacement             */
#define DFL_STAT1_RAD_NOT_RECORDED      0x08  /* 1000 BD-RE  Defect Cluster with unrecorded Replacement  */
#define DFL_STAT1_NRD                   0x01  /* 0001 BD-R/E Defect Cluster without Replacement          */
#define DFL_STAT1_SPR                   0x02  /* 0010 BD-RE  Spare Cluster for future replacement        */
#define DFL_STAT1_PBA                   0x04  /* 0100 BD-RE  Possible Bad Area                           */
#define DFL_STAT1_UNUSABLE              0x07  /* 0111 BD-RE  Unusable Spare Cluster                      */

#define DFL_STAT1_MASK                  0x07  /* Mask for sorting/counting DFL entries                   */
#define DFL_STAT1_SENTINEL              0xFF  /* Marker for end of Status1 lists                         */

// Status2 defines.
#define DFL_STAT2_WHOLE_CLUSTER         0x00  /* 0000 BD-R/E Single Cluster (Default)                    */
#define DFL_STAT2_CRD_START             0x01  /* 0001 BD-R   Start of Contiguous Replacement Clusters    */
#define DFL_STAT2_CRD_END               0x02  /* 0010 BD-R   End of Contiguous Replacement Clusters      */
#define DFL_STAT2_NO_RELEVANT_USERDATA  0x04  /* 0100 BD-RE  PBA: No relevant user data                  */
#define DFL_STAT2_MARKED_DEFECTIVE      0x04  /* 0100 BD-RE  SPR: Previously marked as defective Cluster */
#define DFL_STAT2_PARTIAL               0x08  /* 1000 BD-RE  RAD: partially recorderd                    */
#define DFL_STAT2_SENTINEL              0xFF  /* Marker for end of Status2 lists                         */

#define DFL_SENTINEL                (0xFFFF)  /* Marker for end of DFL entry list                        */
#define DFL_TERMINATOR          (0xFFFFFFFF)
#define DFL_NULL_ADDR           (0xFFFFFFFF)

#define BD_DFL_ALMOST_FULL_THRESHOLD       5  /* DFL Almost Full threshold in precentages ( = 5% )       */
#define BD_DFL_HEADER_LEN               0x40  /* 64 bytes                                                */

#define DflEntryGetStatus(value)        ((BYTE)(value >> 28))
#define DflEntryGetAddress(value)       ((ULONG)(value & 0x0FFFFFFF))
#define GetDefectStartAddress(index)        (DflEntryGetAddress(g_pEntries[index].Entry.DefectBlock))
#define GetReplacementStartAddress(index)   (DflEntryGetAddress(g_pEntries[index].Entry.ReplaceBlock))
#define GetStatus1(index)                   (DflEntryGetStatus(g_pEntries[index].Entry.DefectBlock))
#define GetStatus2(index)                   (DflEntryGetStatus(g_pEntries[index].Entry.ReplaceBlock))

#define AddressToHashIndex(address, entry_area)          ((USHORT)((address - BdGetHashAreaStartAddress(entry_area)) >> eBdDflHashPower))

#define BdrIsDefectAddressInRange(address, index)        ((address >= GetDefectStartAddress(index)) && (address <= BdrGetDefectEndAddress(index)))

#define BdrIsReplacementAddressInRange(address, index)   ((address >= GetReplacementStartAddress(index)) && (address <= BdrGetReplacementEndAddress(index)))

/*********************************************************************************************************
           Type define for BD DFMG
*********************************************************************************************************/

typedef USHORT DflEntryIndexType;

/// Platform DFL entry.
typedef __packed struct DflEntry
{
    ULONG DefectBlock;
    ULONG ReplaceBlock;
} DflEntry;

/// Linked DFL entry.
typedef __packed struct LinkedDflEntry
{
    DflEntry            Entry;
    DflEntryIndexType   Next;
} LinkedDflEntry;


// Criteria for sorting DFL entries.
typedef struct DflEntrySortCriteriaType
{
    BYTE Status1;
    BYTE Status2;
} DflEntrySortCriteriaType;

/// Information for collection of DFL entries.
typedef struct DflEntryCollectionInfoType
{
    USHORT                     SortIndex;
    DflEntrySortCriteriaType   SortCriteria;
    USHORT                     SpareAreaIndex;
    USHORT                     LayerIndex;
    DflEntryIndexType          EntryIndex;
} DflEntryCollectionInfoType;

/// Information for extracting/injecting entries.
typedef struct DflEntryExtractionInfoType
{
    USHORT              SpareAreaIndex;
    USHORT              LayerIndex;
    DflEntryIndexType   FirstEntry;
    DflEntryIndexType   LastEntry;
} DflEntryExtractionInfoType;


typedef struct EntryLocationInfoType
{
    USHORT  HashIndex;
    USHORT  EntryIndex;
} EntryLocationInfoType;

typedef struct
{
    ULONG           ReplacementPba;
    BYTE            Status1;
    BYTE            Status2;
} ReplacementInfoType;

typedef __packed struct
{
        BYTE    IdentifierH;                    // Byte 0 of Dfl Header
        BYTE    IdentifierL;                    // Byte 1
        BYTE    Format;                         // Byte 2
        BYTE    Reserved1;                      // Byte 3
        ULONG   UpdateCount;                    // Byte 4-7
        ULONG   Reserved2;                      // Byte 8-11
        ULONG   NrOfDFLEntries;                 // Byte 12-15
        ULONG   NrOfRADEntries;                 // Byte 16-19
        ULONG   NrOfNRDEntries;                 // Byte 20-23
        ULONG   NrOfSPREntries;                 // Byte 24-27
        ULONG   NrOfPBAEntries;                 // Byte 28-31
        ULONG   NrOfUnusableEntries;            // Byte 32-35
} tBdreDflHeaderS;


typedef __packed struct
{
        BYTE    IdentifierH;                    // Byte 0 of Dfl Header
        BYTE    IdentifierL;                    // Byte 1
        BYTE    Format;                         // Byte 2
        BYTE    Reserved1;                      // Byte 3
        ULONG   UpdateCount;                    // Byte 4-7
        ULONG   Reserved2;                      // Byte 8-11
        ULONG   NrOfDFLEntries;                 // Byte 12-15
        ULONG   NrOfRadCrdEntries;              // Byte 16-19
        ULONG   NrOfNRDEntries;                 // Byte 20-23
} tBdrDflHeaderS;

typedef union
{
    tBdrDflHeaderS      BDR;
    tBdreDflHeaderS     BDRE;
} tBdWritableDflHeaderU;

/*********************************************************************************************************
          enum
*********************************************************************************************************/
enum {
    eBD_DA,
    eBD_ISA,
    eBD_OSA
};


enum {
    eBdrDflCollection       = 1,
    eBdreDflCollection      = 2,
    eBdInstallFakeDiscInfo  = 3,
    eBdCommonFunctionVerify = 4,
    eBdOtherTest            = 5
};

enum {
    eSortOnDefectAddress,
    eSortOnReplaceAddress
};


enum {
    eNotReplacedCluster,
    eReplacedCluster
};

enum {
    eBD_READ = 0,
    eBD_WRITE,
    eBD_STREAM_READ = 0x80,
    eBD_STREAM_WRITE
};

/*********************************************************************************************************
           Macro for BD DFMG
*********************************************************************************************************/

#if 0 // Don't delete it. It is useful reference for debugging.
#define pdbGetBdDAStartAddress(layer)  ((layer == 0) ? 0x120000 : 0xFFFFFF)
#define pdbGetBdDAEndAddress(layer)    ((layer == 0) ? 0xC673FF : 0xFFFFFF)
#define pdbGetBdISAStartAddress(layer) ((layer == 0) ? 0x100000 : 0xFFFFFF)
#define pdbGetBdISAEndAddress(layer)   ((layer == 0) ? 0x11FFFF : 0xFFFFFF)
#define pdbGetBdOSAStartAddress(layer) ((layer == 0) ? 0xC67400 : 0xFFFFFF)
#define pdbGetBdOSAEndAddress(layer)   ((layer == 0) ? 0xCA73FF : 0xFFFFFF)
#define pdbGetBdLayer0EndAddress()     0xCA73FF
#else
#define pdbGetBdDAStartAddress(layer)  ((layer == 0) ? pdbGetBDLsn0StartPSN() : ConvertPSNL0ToL1(pdbGetBDMaxPSN(0)))
#define pdbGetBdDAEndAddress(layer)    ((layer == 0) ? pdbGetBDMaxPSN(0) : pdbGetBDMaxPSN(1))
#define pdbGetBdISAStartAddress(layer) ((layer == 0) ? 0x100000 : (pdbGetBDMaxPSN(1) + 1))
#define pdbGetBdISAEndAddress(layer)   ((layer == 0) ? (pdbGetBDLsn0StartPSN() - 1) : pdbGetBDL1PhysicalEndAddr())
#define pdbGetBdOSAStartAddress(layer) ((layer == 0) ? (pdbGetBDMaxPSN(0) + 1) : ConvertPSNL0ToL1(pdbGetBDL0PhysicalEndAddr()))
#define pdbGetBdOSAEndAddress(layer)   ((layer == 0) ? pdbGetBDL0PhysicalEndAddr() : ConvertPSNL0ToL1(pdbGetBdOSAStartAddress(0)))
#define pdbGetBdLayer0EndAddress()     pdbGetBDL0PhysicalEndAddr()
#endif


/* Macro for BD-R defect list */
#define pdbGetBdrDflCnt()               (DflHeader.BDR.NrOfDFLEntries)
#define pdbGetBdrRadCnt()               (DflHeader.BDR.NrOfRadCrdEntries)
#define pdbGetBdrNrdCnt()               (DflHeader.BDR.NrOfNRDEntries)
/* Macro for BD-RE defect list */
#define pdbGetBdreDflCnt()              (DflHeader.BDRE.NrOfDFLEntries)
#define pdbGetBdreRadCnt()              (DflHeader.BDRE.NrOfRADEntries)
#define pdbGetBdreNrdCnt()              (DflHeader.BDRE.NrOfNRDEntries)
#define pdbGetBdreSpareCnt()            (DflHeader.BDRE.NrOfSPREntries)
#define pdbGetBdrePbaCnt()              (DflHeader.BDRE.NrOfPBAEntries)
#define pdbGetBdreUnusableCnt()         (DflHeader.BDRE.NrOfUnusableEntries)
/*********************************************************************************************************
           BD Work Area
*********************************************************************************************************/
#define BD_WRITABLE_DIUNIT_STRAT_ADDR        BD_DI_ADR
#define BD_WRITABLE_DIUNIT_STRAT_PAGE        BD_DI_PAGE
#define BD_WRITABLE_DIUNIT_LENGTH            112     //the size of each DI Unit for BD_WRITABLE disc is 112 bytes
//#define BD_DDS_START_ADDR                    (SCSDRAM + 0x387800)
//#define BD_DDS_START_PAGE                    0x70F

/*********************************************************************************************************
           extern declaration
*********************************************************************************************************/
extern FLAG BdRawDflAssembled;
extern tBdWritableDflHeaderU       DflHeader;
extern BOOL SearchReplacementInfo(ULONG original, ReplacementInfoType* pReplacementInfo);
extern ULONG MaxDflEndPSN;
extern ULONG MinDflStartPSN;
extern ULONG MaxDflEndLBA;
extern ULONG MinDflStartLBA;
#endif /* _BDDFMG_H_ */
