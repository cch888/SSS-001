/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   BdDfmg.c
*
* DESCRIPTION
*
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/


/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\powerctl.h"
#include ".\hif\gp_mac.h"
#include ".\defmgmt\bdwritable\BdDfmg.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"

#if (BD_ENABLE)
/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
void BdInitDflHashTable(void);
ULONG BdGetAreaSize(BYTE Layer, BYTE Area);
void BdInitDflVariables(void);
void BdInitDflEntries(void);
USHORT AllocateEntry(void);
void BdDebugPrintEntries(void);
BYTE BdFindAddressArea(ULONG address);
USHORT SearchFirstIndexOfChainedEntries(ULONG address);
USHORT BdDflSearchAddress(ULONG address);
void BdRemoveEntry(USHORT index);
void BdPoolReleaseEntry(USHORT index);
void ReleaseEntry(USHORT index);
ULONG BdGetHashAreaStartAddress(BYTE area);
USHORT* BdFindHashTables(BYTE area);
USHORT SearchPrecedingEntry(ULONG address);
USHORT SearchNextEntry(ULONG address);
USHORT BdFindNrOfHashes(BYTE area);
USHORT GetNextEntry(USHORT index);
BOOL SearchReplacementInfo(ULONG original, ReplacementInfoType* pReplacementInfo);
ULONG GetReplacementAddress(USHORT index, ULONG defectAddress);

/* BD-R */
BOOL SaveBdrDefectListContent(ULONG Adr);
void SaveBdrDflHeader(ULONG Adr);
BOOL BdrDflDaAddEntry(DflEntry* pEntry);
BOOL BdrCheckValidity(BYTE status1, BYTE status2);
BOOL BdrHashTableAddEntry(ULONG address, BYTE status1, BYTE status2, ULONG replacement);
BOOL AddCrdDefect(ULONG address, BYTE status1, BYTE status2, ULONG replacement);
void BdrInsertEntry(USHORT index);
ULONG BdrGetDefectEndAddress(USHORT index);
ULONG BdrGetReplacementEndAddress(USHORT index);
BOOL BdrReleaseEntry(ULONG address);
BOOL SplitCrd(USHORT index, ULONG address);

void BdrInsertCrdEntry(USHORT index);
USHORT MergeCrd(USHORT index1, USHORT index2);
USHORT BdrGetNextEntry(USHORT index);
void BdrSetNextEntry(USHORT index, USHORT next_index);
void BdrCollectDAEntries(BYTE area);
void BdrAssembleDefectList(void);

/* BD-RE */
BOOL SaveBdreDefectListContent(ULONG Adr);
void SaveBdreDflHeader(ULONG Adr);
BOOL BdreDflDaAddEntry(DflEntry* pEntry);
BOOL BdreDflSaAddEntry(DflEntry* pEntry);
ULONG BdreGetDefectEndAddress(USHORT index);
BOOL BdreCheckValidityDA(BYTE status1, BYTE status2, ULONG replacement);
BOOL BdreCheckValiditySA(BYTE status1, BYTE status2, ULONG address);
BOOL BdreHashTableAddEntry(ULONG address, BYTE status1, BYTE status2, ULONG replacement);
void BdreInsertEntry(USHORT index);
void BdreInsertEntryCheckPba(USHORT index);
BOOL BdreQuickInsertSpare(ULONG address, BYTE status1, BYTE status2, ULONG replacement);
BOOL BdreReleaseEntry(ULONG address);
BOOL SplitPba(USHORT index, ULONG address);
void BdreAssembleDefectList(void);
void BdreCollectDAEntries(BYTE area);
void BdreCollectSAEntries(BYTE area);

/*********************************************************************
 GLOBAL VARIABLES
*********************************************************************/
ULONG MaxDflEndPSN;
ULONG MinDflStartPSN;
ULONG MaxDflEndLBA;
ULONG MinDflStartLBA;
FLAG BdDefectTableChanged;
FLAG BdRawDflAssembled;

LinkedDflEntry *g_pEntries;
USHORT g_TotalNrOfEntries;
USHORT g_NrOfEntriesLeft;
USHORT g_FreeEntryIndex;
USHORT g_ParkedCrdStartIndex;
BYTE   g_SortMethod;
EntryLocationInfoType   g_QuickInsertionInfo;


USHORT *pHashTableDA0;
USHORT *pHashTableDA1;
USHORT *pHashTableISA0;
USHORT *pHashTableOSA0;
USHORT *pHashTableOSA1;
USHORT *pHashTableISA1;

DflEntry *pTempRAD;
DflEntry *pTempNRD;
DflEntry *pTempPBA;
DflEntry *pTempSPR;
DflEntry *pTempUNU;

ULONG   NrOfRAD;
ULONG   NrOfNRD;
ULONG   NrOfPBA;
ULONG   NrOfSPR;
ULONG   NrOfUNU;

USHORT NrOfHashesDA[2];
USHORT NrOfHashesISA[2];
USHORT NrOfHashesOSA[2];

tBdWritableDflHeaderU       DflHeader;
BYTE BdreSpareAreaOrder[4]  = {eAREA_ISA0, eAREA_OSA0, eAREA_OSA1, eAREA_ISA1};

/*********************************************************************
 FUNCTIONS
*********************************************************************/
/*********************************************************************************************************
          Functions for BD Defect Management common used
*********************************************************************************************************/
/*********************************************************************
FUNCTION    : BdInitDflHashTable
DESCRIPTION : Initialize BD Defect list Hash Table.
              There ara maximum 6 Hash Tables that we will need.
              For Userdata Area : DA0,DA1
              For Spare Area    : ISA0,OSA0,OSA1,ISA1
              We divide 64 clusters as one Hash Group for eash area.
PARAMETERS  : None
RETURNS     : None
*********************************************************************/
void BdInitDflHashTable(void)
{
    USHORT page_num;
    USHORT HashFactor;
    BYTE   i;

    // Assign the pointers of all Hash Table Areas
    pHashTableDA0   = (USHORT *)BD_HASH_TABLE_DA_0_START;
    pHashTableDA1   = (USHORT *)BD_HASH_TABLE_DA_1_START;
    pHashTableISA0  = (USHORT *)BD_HASH_TABLE_ISA_0_START;
    pHashTableOSA0  = (USHORT *)BD_HASH_TABLE_OSA_0_START;
    pHashTableOSA1  = (USHORT *)BD_HASH_TABLE_OSA_1_START;
    pHashTableISA1  = (USHORT *)BD_HASH_TABLE_ISA_1_START;

    // Initialise DFL Pool (Hash Tables and DFL Entries), fill with 0xFFFF.
    page_num = BD_TOTAL_DFL_POOL_SIZE/0x800;
    // LinearPatternFill(        USHORT trgpage, USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern)
    LinearPatternFill((USHORT)BD_DFL_POOL_START_PAGE,          0,     page_num,        0x800/2,   DFL_SENTINEL);

    // Set the address range and calculate the required number of Hash Table Entries.
    //SetAddressRange(startAddress, stopAddress);
    //TBD

    //calculate NrOfHashes
    HashFactor = 1 << eBdDflHashPower; //2048, it means one Hash group is 2048 sectors=64 clusters
    for (i = 0; i < pdbGetBDLayers(); i++)
    {
        NrOfHashesDA[i]  = (BdGetAreaSize(i,eBD_DA)  + (HashFactor - 1)) / HashFactor;
        NrOfHashesISA[i] = (BdGetAreaSize(i,eBD_ISA) + (HashFactor - 1)) / HashFactor;
        NrOfHashesOSA[i] = (BdGetAreaSize(i,eBD_OSA) + (HashFactor - 1)) / HashFactor;

        send_msg5SValue(SEND_MRW_MSG,"NrOfHashesDA",2,NrOfHashesDA[i]);
        send_msg5SValue(SEND_MRW_MSG,"NrOfHashesISA",2,NrOfHashesISA[i]);
        send_msg5SValue(SEND_MRW_MSG,"NrOfHashesOSA",2,NrOfHashesOSA[i]);
    }

    send_msg5SValue(SEND_MRW_MSG,"test = ",2,pHashTableDA0[500]);  //check for init value

    BdInitDflVariables();

}

/*********************************************************************
FUNCTION    : BdInitDflEntries
DESCRIPTION : Initialize BD Defect list Entries in POOL.
              The next index of each entry is sequential increased.
PARAMETERS  : None
RETURNS     : None
*********************************************************************/
void BdInitDflEntries(void)
{
    USHORT index;

    //Point to the start of the Entry Area
    g_pEntries = (LinkedDflEntry*) (BD_DFL_ENTRY_START);

    g_TotalNrOfEntries = BD_TOTAL_ENTRIES_SIZE/ sizeof(LinkedDflEntry);
    g_NrOfEntriesLeft  = g_TotalNrOfEntries;
    g_FreeEntryIndex   = 0;

    send_msg5SValue(SEND_MRW_MSG,"pool size",4,BD_TOTAL_DFL_POOL_SIZE);
    send_msg5SValue(SEND_MRW_MSG,"hash size",4,BD_TOTAL_HASH_TABLE_SIZE);
    send_msg5SValue(SEND_MRW_MSG,"entry size",4,sizeof(LinkedDflEntry));
    send_msg5SValue(SEND_MRW_MSG,"EntryAreaSize",4,BD_TOTAL_ENTRIES_SIZE);
    send_msg5SValue(SEND_MRW_MSG,"T-NrOfEntries",2,g_TotalNrOfEntries);

    // Initialise the entries.
    for (index = 0; index < g_TotalNrOfEntries; index++)
    {
        g_pEntries[index].Next = index + 1;
    }

    g_pEntries[g_TotalNrOfEntries - 1].Next = DFL_SENTINEL;
    MaxDflEndPSN = 0;
    MinDflStartPSN = 0xffffffff;
    MaxDflEndLBA = 0;
    MinDflStartLBA = 0;
}


/*********************************************************************
FUNCTION    : BdGetAreaSize
DESCRIPTION : When disc is mounted successful, the size of each area
              is fixed.
PARAMETERS  : 1. Layer : 0 - Layer0; 1 - Layer1
              2. Area  : DA,ISA, or OSA.
RETURNS     : Area Size (sectors).
*********************************************************************/
ULONG BdGetAreaSize(BYTE Layer, BYTE Area)
{
    ULONG AreaSize;

    switch(Area)
    {
        case eBD_DA:
            AreaSize = pdbGetBdDAEndAddress(Layer) - pdbGetBdDAStartAddress(Layer) + 1;
            break;

        case eBD_ISA:
            AreaSize = pdbGetBdISAEndAddress(Layer) - pdbGetBdISAStartAddress(Layer) + 1;
            break;

        case eBD_OSA:
            AreaSize = pdbGetBdOSAEndAddress(Layer) - pdbGetBdOSAStartAddress(Layer) + 1;
            break;

        default:
             send_msg80(1,0x710084);
    }

    return AreaSize;
}

/*********************************************************************
FUNCTION    : BdInitDflVariables
DESCRIPTION : Initialize some global variables for Defect list.
PARAMETERS  : None
RETURNS     : None
*********************************************************************/
void BdInitDflVariables(void)
{

    //Initialize table changed flag.
    //If this flag is set to 1, we will write the new table to disc before ejecting.
    BdDefectTableChanged = CLEAR;

    //Indicate SortMethod is sorting on Defect or Replacement address
    g_SortMethod = eSortOnDefectAddress; // default setting

    g_ParkedCrdStartIndex = DFL_SENTINEL;

    //For BD-RE QuickInsert
    g_QuickInsertionInfo.HashIndex  = DFL_SENTINEL;
    g_QuickInsertionInfo.EntryIndex = DFL_SENTINEL;

    //Indicate Defect List is assembled or not.
    BdRawDflAssembled = CLEAR;
}


/*********************************************************************
FUNCTION    : AllocateEntry
DESCRIPTION : Allocate a free entry for a new entry
PARAMETERS  : None
RETURNS     : free_entry_index - the entry index in POOL
*********************************************************************/
USHORT AllocateEntry(void)
{
    USHORT free_entry_index = DFL_SENTINEL;

    // Check if there are free entries available.
    if (g_FreeEntryIndex == DFL_SENTINEL)
    {
        send_msg80(1,0x710085);
    }
    else
    {
        // Get a free entry (that is not chained to a next entry).
        free_entry_index = g_FreeEntryIndex;
        g_FreeEntryIndex = g_pEntries[g_FreeEntryIndex].Next;
        g_pEntries[free_entry_index].Next = DFL_SENTINEL;

        g_NrOfEntriesLeft--;
    }

    //send_msg5SValue(SEND_ATAPI_CMD,"free index",2,free_entry_index);

    return free_entry_index;
}


/*********************************************************************
FUNCTION    : BdDebugPrintEntries
DESCRIPTION : Print the contents of defect list entries in POOL for
              debugging. Only the first 100 entries are printed.
PARAMETERS  : None
RETURNS     : None
*********************************************************************/
void BdDebugPrintEntries(void)
{
    BYTE i,num;

    num = (DflHeader.BDR.NrOfDFLEntries > 100) ? 100 : DflHeader.BDR.NrOfDFLEntries;
    for (i = 0; i < (num + 3); i++)
    {
        send_msg5SValue(SEND_MRW_MSG,"index=",1,i);
        send_msg5SValue(SEND_MRW_MSG,"Def =",4,g_pEntries[i].Entry.DefectBlock);
        send_msg5SValue(SEND_MRW_MSG,"Rep =",4,g_pEntries[i].Entry.ReplaceBlock);
        send_msg5SValue(SEND_MRW_MSG,"Next=",2,g_pEntries[i].Next);
    }
}


/*********************************************************************
FUNCTION    : BdDflSearchAddress
DESCRIPTION : Search an address in the defect list.
PARAMETERS  : address - to be searched.
RETURNS     : entry_index - if found, the index will be the position
              of the entry; if not found, the index is DFL_SENTINEL.
*********************************************************************/
USHORT BdDflSearchAddress(ULONG address)
{
    ULONG  EntryStartAddr,EntryEndAddr;
    USHORT entry_index;
    BOOL   found = FALSE;

    // Find the chain of entries where an entry with a matching
    // address may be found.
    entry_index = SearchFirstIndexOfChainedEntries(address);

    //send_msg5SValue(SEND_ATAPI_CMD,"chain index=",2,entry_index);

    // Enumerate the chained entries to find an entry with a matching address.
    while ((!found) && (entry_index != DFL_SENTINEL))
    {
        if (g_SortMethod == eSortOnDefectAddress)
        {
            // Check if the given defect address is in range of the entry.
            EntryStartAddr = DflEntryGetAddress(g_pEntries[entry_index].Entry.DefectBlock);
            EntryEndAddr   = (pdbGetBDType() == BD_R) ? BdrGetDefectEndAddress(entry_index)
                                                      : BdreGetDefectEndAddress(entry_index);
            if ((address >= EntryStartAddr) && (address <= EntryEndAddr))
                found = TRUE;
            else if(address < EntryStartAddr)
            {
                entry_index = DFL_SENTINEL;
                break;
            }
            else
                found = FALSE;
        }
        else
        {
            // Check if the given replacement address is in range of the entry.
            EntryStartAddr = DflEntryGetAddress(g_pEntries[entry_index].Entry.ReplaceBlock);
            EntryEndAddr   = EntryStartAddr + SECTORS_PER_CLUSTER - 1;
            if ((address >= EntryStartAddr) && (address <= EntryEndAddr))
                found = TRUE;
            else if(address < EntryStartAddr)
            {
                entry_index = DFL_SENTINEL;
                break;
            }
            else
                found = FALSE;
        }

        if (!found)
        {
            // Not found, check the next entry.
            entry_index = GetNextEntry(entry_index);
        }
    }

    return entry_index;
}


/*********************************************************************
FUNCTION    : SearchFirstIndexOfChainedEntries
DESCRIPTION : The first index of chained entries is stored in HashTable[]
              It is the entrance of the chain.
PARAMETERS  : address
RETURNS     : entry_index
*********************************************************************/
USHORT SearchFirstIndexOfChainedEntries(ULONG address)
{
    ULONG  StartAddress;
    USHORT *pHashTable;
    USHORT hash_index;
    USHORT entry_index = DFL_SENTINEL;
    BYTE   entry_area  = BdFindAddressArea(address);

    switch(entry_area)
    {
        case eAREA_ISA0:
            pHashTable = pHashTableISA0;
            StartAddress = pdbGetBdISAStartAddress(0);
            break;

        case eAREA_DA0:
            pHashTable = pHashTableDA0;
            StartAddress = pdbGetBdDAStartAddress(0);
            break;

        case eAREA_OSA0:
            pHashTable = pHashTableOSA0;
            StartAddress = pdbGetBdOSAStartAddress(0);
            break;

        case eAREA_OSA1:
            pHashTable = pHashTableOSA1;
            StartAddress = pdbGetBdOSAStartAddress(1);
            break;

        case eAREA_DA1:
            pHashTable = pHashTableDA1;
            StartAddress = pdbGetBdDAStartAddress(1);
            break;

        case eAREA_ISA1:
            pHashTable = pHashTableISA1;
            StartAddress = pdbGetBdISAStartAddress(1);
            break;

        default:
            send_msg80(1,0x710086);
            break;
    }

    hash_index = (USHORT)((address - StartAddress) >> eBdDflHashPower);
    entry_index = pHashTable[hash_index];

    return entry_index;
}


/*********************************************************************
FUNCTION    : BdFindAddressArea
DESCRIPTION : Find which area the address is located on.
PARAMETERS  : address
RETURNS     : area
*********************************************************************/
BYTE BdFindAddressArea(ULONG address)
{
    BYTE area;

    if (address > pdbGetBdLayer0EndAddress()) // address in layer1
    {
        if (address <= pdbGetBdOSAEndAddress(1))
            area = eAREA_OSA1;
        else if (address <= pdbGetBdDAEndAddress(1))
            area = eAREA_DA1;
        else
            area = eAREA_ISA1;
    }
    else
    {
        if (address <= pdbGetBdISAEndAddress(0))
            area = eAREA_ISA0;
        else if (address <= pdbGetBdDAEndAddress(0))
            area = eAREA_DA0;
        else
            area = eAREA_OSA0;
    }

    return area;
}


/*********************************************************************
FUNCTION    : BdPoolReleaseEntry
DESCRIPTION : Release an entry in POOL.
PARAMETERS  : entry index
RETURNS     : None
*********************************************************************/
void BdPoolReleaseEntry(USHORT index)
{
}

void ReleaseEntry(USHORT index)
{
    g_pEntries[index].Entry.DefectBlock  = 0;
    g_pEntries[index].Entry.ReplaceBlock = 0;
    if (pdbGetBDType() == BD_R)
        BdrSetNextEntry(index, DFL_SENTINEL);
    else
        g_pEntries[index].Next = DFL_SENTINEL;
}


void BdRemoveEntry(USHORT index)
{
}


USHORT SearchPrecedingEntry(ULONG address)
{
    USHORT *pHashTable;
    SHORT  hash_index;
    USHORT preceding_entry = DFL_SENTINEL;
    BYTE   entry_area  = BdFindAddressArea(address);

    pHashTable = BdFindHashTables(entry_area);
    hash_index = (SHORT)AddressToHashIndex(address, entry_area);

    // Start enumerating the entries in the Hash Table starting from the location of the
    // specified address to the beginning of the Hash Table.
    for (; (hash_index >= 0) && (preceding_entry == DFL_SENTINEL); --hash_index)
    {
        USHORT  enum_entry_index      = pHashTable[hash_index];
        USHORT  candidate_entry_index = DFL_SENTINEL;
        ULONG   enum_entry_address;

        // Enumerate the chained entries.
        while (enum_entry_index != DFL_SENTINEL)
        {
            // get the (sort) start address of the enumerated entry.
            enum_entry_address = (g_SortMethod == eSortOnDefectAddress) ? GetDefectStartAddress(enum_entry_index) :
                                                                          GetReplacementStartAddress(enum_entry_index);

            if (enum_entry_address < address)
            {
                // Candidate found, store and see if there are more addresses
                // in the chain that have a lower address that the specified one.
                candidate_entry_index = enum_entry_index;
            }

            // Check the next entry.
            enum_entry_index = GetNextEntry(enum_entry_index);
        }

        preceding_entry = candidate_entry_index;
    }

    return preceding_entry;
}


USHORT SearchNextEntry(ULONG address)
{
    USHORT *pHashTable;
    USHORT hash_index;
    USHORT NrOfHashes;
    USHORT next_entry = DFL_SENTINEL;
    BYTE   entry_area = BdFindAddressArea(address);

    pHashTable = BdFindHashTables(entry_area);
    NrOfHashes = BdFindNrOfHashes(entry_area);
    hash_index = AddressToHashIndex(address, entry_area);

    // Start enumerating the entries in the Hash Table starting from the location of the
    // specified address to the end of the Hash Table.
    for (; (hash_index < NrOfHashes) && (next_entry == DFL_SENTINEL); ++hash_index)
    {
        USHORT  enum_entry_index = pHashTable[hash_index];
        BOOL    is_next;

        // Enumerate the chained entries.
        while ((enum_entry_index != DFL_SENTINEL) && (next_entry == DFL_SENTINEL))
        {
            // Check if the address is larger than the address range of the enumerated entry.
            if (g_SortMethod == eSortOnDefectAddress)
            {
                is_next = ((GetDefectStartAddress(enum_entry_index) > address)
                           /*&& (!p_enum_entry->IsDefectAddressInRange(address))*/);
            }
            else
            {
                is_next = ((GetReplacementStartAddress(enum_entry_index) > address)
                           /*&& (!p_enum_entry->IsReplacementAddressInRange(address))*/);
            }

            if (is_next)
            {
                // Next entry found.
                next_entry = enum_entry_index;
            }
            else
            {
                // Check the next entry.
                enum_entry_index = GetNextEntry(enum_entry_index);
            }
        }
    }

    return next_entry;
}


/*********************************************************************
FUNCTION    : BdGetHashAreaStartAddress
DESCRIPTION : Return the start address of the input hash area
PARAMETERS  : area
RETURNS     : StartAddress
*********************************************************************/
ULONG BdGetHashAreaStartAddress(BYTE area)
{
    ULONG StartAddress;

    switch (area)
    {
        case eAREA_ISA0:
            StartAddress = pdbGetBdISAStartAddress(0);
            break;

        case eAREA_DA0:
            StartAddress = pdbGetBdDAStartAddress(0);
            break;

        case eAREA_OSA0:
            StartAddress = pdbGetBdOSAStartAddress(0);
            break;

        case eAREA_OSA1:
            StartAddress = pdbGetBdOSAStartAddress(1);
            break;

        case eAREA_DA1:
            StartAddress = pdbGetBdDAStartAddress(1);
            break;

        case eAREA_ISA1:
            StartAddress = pdbGetBdISAStartAddress(1);
            break;

        default:
            send_msg80(1,0x710087);
            break;
    }

    return StartAddress;
}


/*********************************************************************
FUNCTION    : BdFindHashTables
DESCRIPTION : Return the HashTable pointer of the input hash area
PARAMETERS  : area
RETURNS     : pHashTbl
*********************************************************************/
USHORT* BdFindHashTables(BYTE area)
{
    USHORT* pHashTbl;

    switch (area)
    {
        case eAREA_ISA0:
            pHashTbl = pHashTableISA0;
            break;

        case eAREA_DA0:
            pHashTbl = pHashTableDA0;
            break;

        case eAREA_OSA0:
            pHashTbl = pHashTableOSA0;
            break;

        case eAREA_OSA1:
            pHashTbl = pHashTableOSA1;
            break;

        case eAREA_DA1:
            pHashTbl = pHashTableDA1;
            break;

        case eAREA_ISA1:
            pHashTbl = pHashTableISA1;
            break;
    }

    return pHashTbl;
}


/*********************************************************************
FUNCTION    : BdFindNrOfHashes
DESCRIPTION : Return the number of Hashes for the input hash area
PARAMETERS  : area
RETURNS     : number
*********************************************************************/
USHORT BdFindNrOfHashes(BYTE area)
{
    USHORT number;

    switch (area)
    {
        case eAREA_ISA0:
            number = NrOfHashesISA[0];
            break;

        case eAREA_DA0:
            number = NrOfHashesDA[0];
            break;

        case eAREA_OSA0:
            number = NrOfHashesOSA[0];
            break;

        case eAREA_OSA1:
            number = NrOfHashesOSA[1];
            break;

        case eAREA_DA1:
            number = NrOfHashesDA[1];
            break;

        case eAREA_ISA1:
            number = NrOfHashesISA[1];
            break;
    }

    return number;
}


/*********************************************************************
FUNCTION    : GetNextEntry
DESCRIPTION : Retrun the index that points to next entry.
PARAMETERS  : index - current entry index
RETURNS     : next entry index.
NOTE        : BD-R's next entry needs to take care specially due to
              CRD entry.
*********************************************************************/
USHORT GetNextEntry(USHORT index)
{
    if (pdbGetBDType() == BD_R)
    {
        return (BdrGetNextEntry(index));
    }
    else
    {
        return g_pEntries[index].Next;
    }
}


/*********************************************************************
FUNCTION    : SearchReplacementInfo
DESCRIPTION : Give a address to find whether it is in the defect list
              or not. if yes, get the replacement information.
PARAMETERS  : original - the address to be searched.
              pReplacementInfo - the structure to store replacement info.
RETURNS     : TRUE - found
              FALSE - not found
*********************************************************************/
BOOL SearchReplacementInfo(ULONG original, ReplacementInfoType* pReplacementInfo)
{
    USHORT entry_index = DFL_SENTINEL;
    BYTE   area;

    if (BdwtDisc.CpuUsage8X)
        return FALSE;

    if (pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc)
    {
        if (DflHeader.BDR.NrOfDFLEntries == 0)
            return FALSE;
    }
    else
    {
        if (DflHeader.BDRE.NrOfDFLEntries == DflHeader.BDRE.NrOfSPREntries)
            return FALSE;
    }

    pReplacementInfo->ReplacementPba = 0;

    area = BdFindAddressArea(original);

    if ((area == eAREA_DA0) || (area == eAREA_DA1))  // Search Data Area
    {
        entry_index = BdDflSearchAddress(original);
        if (entry_index != DFL_SENTINEL)
        {
            //send_msg5SValue(SEND_ATAPI_CMD,"index",2,entry_index);
            //send_msg5SValue(SEND_ATAPI_CMD,"defblk=",4,GetDefectStartAddress(entry_index));
            //send_msg5SValue(SEND_ATAPI_CMD,"repblk=",4,GetReplacementStartAddress(entry_index));
            //send_msg5SValue(SEND_ATAPI_CMD,"status1=",1,GetStatus1(entry_index));
            //send_msg5SValue(SEND_ATAPI_CMD,"status2=",1,GetStatus2(entry_index));

            pReplacementInfo->Status1 = GetStatus1(entry_index);
            pReplacementInfo->Status2 = GetStatus2(entry_index);
            switch(pReplacementInfo->Status1)
            {
                case DFL_STAT1_RAD:
                case DFL_STAT1_RAD_NOT_RECORDED:
                    pReplacementInfo->ReplacementPba = GetReplacementAddress(entry_index, original);
                    SendMsgCn(SEND_ATAPI_CMD,4,0x800303,A4B(pReplacementInfo->ReplacementPba));
                    break;

                case DFL_STAT1_NRD:
                case DFL_STAT1_PBA:
                    // Defect without a replacement address.
                    break;

                default:
                    break;
            }
        }
        else
        {
            //send_msg5S(SEND_ATAPI_CMD,"Entry not found");
        }
    }
    else  // Search Spare Area
    {
        send_msg80(SEND_ATAPI_CMD,0x710088);
        //found = SearchDiscTableSA(pReplacementInfo);
    }

    return (entry_index != DFL_SENTINEL);
}


/*********************************************************************
FUNCTION    : GetReplacementAddress
DESCRIPTION : The function can process all types of entry.
PARAMETERS  : index - entry index
              defectAddress
RETURNS     : the replacement address relative to defectAddress
NOTE        : For CRD/PBA entry types, the offset may be not equal to 0
*********************************************************************/
ULONG GetReplacementAddress(USHORT index, ULONG defectAddress)
{
    ULONG offset;

    offset = defectAddress - GetDefectStartAddress(index);
    send_msg5SValue(SEND_MRW_MSG,"add-offset",4,offset);

    return (GetReplacementStartAddress(index) + offset);
}

/*********************************************************************************************************
          Functions for BD-RE
*********************************************************************************************************/
/*********************************************************************
FUNCTION    : SaveBdreDefectListContent
DESCRIPTION : The main routine processes BD-RE defect list saving from
              disc to DRAM's POOL
PARAMETERS  : None
RETURNS     : TRUE - success
              FALSE - failure
*********************************************************************/
BOOL SaveBdreDefectListContent(ULONG Adr)
{
    ULONG    NrOfEntries;
    DflEntry *p_dfl_entry;
    ULONG    j;

    BdInitDflHashTable();   //Initialize Hash Tables of DFL Pool
    BdInitDflEntries();     //Initialize Defect Entries of DFL Pool
#if 0//currently not be use
    BdTest(4,0);            //For debugging only
#endif //currently not be use

    //Save Defect List Header Info.
    SaveBdreDflHeader(Adr);
    SendMsgCn(SEND_ATAPI_CMD,4,0x800304,A4B(DflHeader.BDRE.NrOfDFLEntries));

    if ((DflHeader.BDRE.IdentifierH == 'D') && (DflHeader.BDRE.IdentifierL == 'L'))
    {
        send_msg80(SEND_SStpTask_MSG,0x710089);
        //Collect Defect Entries from Defect List
        NrOfEntries = DflHeader.BDRE.NrOfDFLEntries;
        p_dfl_entry = (DflEntry *)(Adr + BD_DFL_HEADER_LEN);  //point to first entry

// only RAD is used for READER drive.
        NrOfRAD = DflHeader.BDRE.NrOfRADEntries;
        for (j = 0; j < NrOfRAD; j++)
        {
            if (!BdreDflDaAddEntry(p_dfl_entry))
            {
                if (!BdreDflSaAddEntry(p_dfl_entry))
                {
                    send_msg80(SEND_SStpTask_MSG,0x71008A);
                }
            }
            p_dfl_entry++;
        }
    }
    else
    {
        send_msg80(SEND_SStpTask_MSG,0x71008B);
        return FALSE;
    }

    return TRUE;
}


/*********************************************************************
FUNCTION    : SaveBdreDflHeader
DESCRIPTION : Save BD-RE's defect list header from a Temp DRAM Area to
              a global structure.
PARAMETERS  : None
RETURNS     : None
*********************************************************************/
void SaveBdreDflHeader(ULONG Adr)
{
    tBdreDflHeaderS *BufferPtr;

    BufferPtr = (tBdreDflHeaderS *) Adr;

    DflHeader.BDRE.IdentifierH         = BufferPtr->IdentifierH;
    DflHeader.BDRE.IdentifierL         = BufferPtr->IdentifierL;
    DflHeader.BDRE.Format              = BufferPtr->Format;
    DflHeader.BDRE.Reserved1           = BufferPtr->Reserved1;
    DflHeader.BDRE.UpdateCount         = SWAPEND_ULONG(BufferPtr->UpdateCount);
    DflHeader.BDRE.NrOfDFLEntries      = SWAPEND_ULONG(BufferPtr->NrOfDFLEntries);
    DflHeader.BDRE.NrOfRADEntries      = SWAPEND_ULONG(BufferPtr->NrOfRADEntries);
    DflHeader.BDRE.NrOfNRDEntries      = SWAPEND_ULONG(BufferPtr->NrOfNRDEntries);
    DflHeader.BDRE.NrOfSPREntries      = SWAPEND_ULONG(BufferPtr->NrOfSPREntries);
    DflHeader.BDRE.NrOfPBAEntries      = SWAPEND_ULONG(BufferPtr->NrOfPBAEntries);
    DflHeader.BDRE.NrOfUnusableEntries = SWAPEND_ULONG(BufferPtr->NrOfUnusableEntries);
}


/*********************************************************************
FUNCTION    : BdreDflDaAddEntry
DESCRIPTION : Add BD-RE DA Entry (RAD/NRD/PBA) to POOL.
PARAMETERS  : pEntry - the processed entry
RETURNS     : TRUE - added OK
              FALSE - added NG
*********************************************************************/
BOOL BdreDflDaAddEntry(DflEntry* pEntry)
{
    ULONG temp,address,replacement;
    BYTE  status1,status2;
    BOOL  entry_added = FALSE;
    BOOL  valid       = FALSE;

    //Get the content of this entry
    temp          = SWAPEND_ULONG(pEntry->DefectBlock);
    status1       = DflEntryGetStatus(temp);
    address       = DflEntryGetAddress(temp);
    temp          = SWAPEND_ULONG(pEntry->ReplaceBlock);
    status2       = DflEntryGetStatus(temp);
    replacement   = DflEntryGetAddress(temp);

    // Check if the disc DFL entry is valid.
    valid = BdreCheckValidityDA(status1, status2, replacement);

    if (valid)
    {
        BYTE Layer = 0;
        BOOL found = FALSE;

        // Find the hash table to add the entry to.
        while ((!found) && (Layer < pdbGetBDLayers()))
        {
            // Check if the entry is within the range of the current hash table.
            if ((address >= pdbGetBdDAStartAddress(Layer)) && (address <= pdbGetBdDAEndAddress(Layer)))
                found = TRUE;

            if (found)
            {
                // Add the entry.
                entry_added = BdreHashTableAddEntry(address, status1, status2, replacement);
                if (!entry_added)
                {
                    // Failed to add the entry to the hash table.
                    send_msg80(1,0x71008C);
                }
                else
                {
                    if(address > MaxDflEndPSN)
                    {
                        MaxDflEndPSN = address + 0x1F;
                        MaxDflEndLBA = bdConvertPSNtoLBA(MaxDflEndPSN);
                    }

                    if(address < MinDflStartPSN)
                    {
                        MinDflStartPSN = address;
                        MinDflStartLBA = bdConvertPSNtoLBA(MinDflStartPSN);
                    }
                }
            }
            else
            {
                Layer++;
            }
        }
    }
    return entry_added;
}


/*********************************************************************
FUNCTION    : BdreDflSaAddEntry
DESCRIPTION : Add BD-RE SA Entry (SPR/UNUSABLE) to POOL.
PARAMETERS  : pEntry - the processed entry
RETURNS     : TRUE - added OK
              FALSE - added NG
*********************************************************************/
BOOL BdreDflSaAddEntry(DflEntry* pEntry)
{
    ULONG temp,address,replacement;
    BYTE  status1,status2;
    BOOL  entry_added = FALSE;
    BOOL  valid       = FALSE;

    //Get the content of this entry
    temp          = SWAPEND_ULONG(pEntry->DefectBlock);
    status1       = DflEntryGetStatus(temp);
    address       = DflEntryGetAddress(temp);
    temp          = SWAPEND_ULONG(pEntry->ReplaceBlock);
    status2       = DflEntryGetStatus(temp);
    replacement   = DflEntryGetAddress(temp);

    // Set SortMethod to indicate that we are in spare area now.
    g_SortMethod = eSortOnReplaceAddress;

    // Check if the disc DFL entry is valid.
    valid = BdreCheckValiditySA(status1, status2, address);

    if (valid)
    {
        BYTE Layer = 0;
        BOOL found = FALSE;

        // Find the hash table to add the entry to.
        while ((!found) && (Layer < pdbGetBDLayers()))
        {
            // Check if the entry is within the range of the current hash table.
            if (((replacement >= pdbGetBdISAStartAddress(Layer)) && (replacement <= pdbGetBdISAEndAddress(Layer))) ||
                ((replacement >= pdbGetBdOSAStartAddress(Layer)) && (replacement <= pdbGetBdOSAEndAddress(Layer))))
                found = TRUE;

            if (found)
            {
                // Add the entry.
                //entry_added = BdreHashTableAddEntry(address, status1, status2, replacement);
                entry_added = BdreQuickInsertSpare(address, status1, status2, replacement);
                if (!entry_added)
                {
                    // Failed to add the entry to the hash table.
                    send_msg80(1,0x71008D);
                }
            }
            else
            {
                Layer++;
            }
        }
    }

    // Restore SortMethod to default setting.
    g_SortMethod = eSortOnDefectAddress;

    if (!entry_added)
        SendMsgCn(SEND_ATAPI_CMD,4,0x800305,A4B(replacement));

    return entry_added;
}


/*********************************************************************
FUNCTION    : BdreCheckValidityDA
DESCRIPTION : Check validity for BD-RE DA entry
PARAMETERS  : status1,status2,replacement
RETURNS     : TRUE - valid
              FALSE - invalid
*********************************************************************/
BOOL BdreCheckValidityDA(BYTE status1, BYTE status2, ULONG replacement)
{
    BOOL valid = FALSE;

    // An entry already exists for the given defect address.
    switch (status1)
    {
    case DFL_STAT1_RAD:
        valid = ((status2 == DFL_STAT2_WHOLE_CLUSTER) ||
                 (status2 == DFL_STAT2_PARTIAL));
        break;

    case DFL_STAT1_RAD_NOT_RECORDED:
        valid = (status2 == DFL_STAT2_WHOLE_CLUSTER);
        break;

    case DFL_STAT1_NRD:
        valid = TRUE;
        break;

    case DFL_STAT1_PBA:
        // Note: Currently PBA with a 'Unknown defect size' (size = 0) are ignored.
        valid = (((status2 == DFL_STAT2_WHOLE_CLUSTER) ||
                  (status2 == DFL_STAT2_NO_RELEVANT_USERDATA)) &&
                  (replacement != 0));
        break;

    case DFL_STAT1_SPR:
    case DFL_STAT1_UNUSABLE:
        // Not valid: Entry does not belong in the defect hash tables.
        break;

    default:
        // Not valid: Unknown status1.
        break;
    }

    return valid;
}


/*********************************************************************
FUNCTION    : BdreCheckValiditySA
DESCRIPTION : Check validity for BD-RE SA entry
PARAMETERS  : status1,status2,address
RETURNS     : TRUE - valid
              FALSE - invalid
*********************************************************************/
BOOL BdreCheckValiditySA(BYTE status1, BYTE status2, ULONG address)
{
    BOOL valid = FALSE;

    switch (status1)
    {
        case DFL_STAT1_RAD:
        case DFL_STAT1_RAD_NOT_RECORDED:
        case DFL_STAT1_PBA:
            // Not valid: Entry does not belong in the spare hash tables.
            break;

        case DFL_STAT1_SPR:
            valid = (((status2 == DFL_STAT2_WHOLE_CLUSTER) ||
                      (status2 == DFL_STAT2_MARKED_DEFECTIVE)) && (address == 0));
            break;

        case DFL_STAT1_UNUSABLE:
            valid = ((status2 == DFL_STAT2_WHOLE_CLUSTER) && (address == 0));
            break;

        default:
            // Not valid: Unknown status1.
            break;
    }

    return valid;

}


/*********************************************************************
FUNCTION    : BdreHashTableAddEntry
DESCRIPTION : Allocate a memory space for a new entry, and Insert the
              new entry into the hash table.
PARAMETERS  : the contents of this entry
RETURNS     : TRUE - added OK
              FALSE - added NG
*********************************************************************/
BOOL BdreHashTableAddEntry(ULONG address, BYTE status1, BYTE status2, ULONG replacement)
{
    USHORT  index;
    BOOL entry_added = FALSE;

    //send_msg5SValue(SEND_ATAPI_CMD,"address",4,address);
    //send_msg5SValue(SEND_ATAPI_CMD,"replace",4,replacement);

    // If an entry already exists for the given defect address, then remove it
    // before adding the new one.
    //BdreReleaseEntry(address);

    // There is no existing entry, allocate a new entry.
    if ((index = AllocateEntry()) != DFL_SENTINEL)
    {
        // Set the values of the new entry.
        g_pEntries[index].Entry.DefectBlock  = ((((ULONG)status1) << 28) | address);
        g_pEntries[index].Entry.ReplaceBlock = ((((ULONG)status2) << 28) | replacement);

        //send_msg5SValue(SEND_ATAPI_CMD,"def block",4,g_pEntries[index].Entry.DefectBlock);
        //send_msg5SValue(SEND_ATAPI_CMD,"rep block",4,g_pEntries[index].Entry.ReplaceBlock);

        // Insert the new entry into the hash table.
        BdreInsertEntry(index);  // This is the common insert function for DA and SA

        if (g_SortMethod == eSortOnDefectAddress)  // Data Area
            BdreInsertEntryCheckPba(index);        // Check if we need to merge PBA entry

        entry_added = TRUE;
    }

    return entry_added;
}


/*********************************************************************
FUNCTION    : BdreQuickInsertSpare
DESCRIPTION : Quick insert a spare entry for BD-RE
              This process doesn't search Hash Table, just append a new
              entry to the previous one.
PARAMETERS  : the contents of this entry
RETURNS     : TRUE - added OK
              FALSE - added NG
*********************************************************************/
BOOL BdreQuickInsertSpare(ULONG address, BYTE status1, BYTE status2, ULONG replacement)
{
    USHORT  index;
    BOOL entry_added = FALSE;

    if (status2 != DFL_STAT2_WHOLE_CLUSTER)
    {
        // Only regular spare entries can be quick inserted, so add this entry the regular way.
        entry_added = BdreHashTableAddEntry(address, status1,status2,replacement);
    }
    else
    {
        if ((index = AllocateEntry()) != DFL_SENTINEL)
        {
            ULONG       prev_replacement;
            USHORT      new_entry_hash_index;
            USHORT      *pHashTable;
            BYTE        prev_entry_area, entry_area;
            BOOL        error;

            //send_msg5SValue(1,"QInsert index",2,index);
            // Set the values of the new entry.
            g_pEntries[index].Entry.DefectBlock  = ((((ULONG)status1) << 28) | address);
            g_pEntries[index].Entry.ReplaceBlock = ((((ULONG)status2) << 28) | replacement);

            // Get the hash index for the new entry and map the previous entry.
            entry_area           = BdFindAddressArea(replacement);
            new_entry_hash_index = AddressToHashIndex(replacement,entry_area);
            pHashTable           = BdFindHashTables(entry_area);

            //get prev entry's replacement here
            if (g_QuickInsertionInfo.EntryIndex != DFL_SENTINEL)
            {
                prev_replacement = GetReplacementStartAddress(g_QuickInsertionInfo.EntryIndex);
                prev_entry_area  = BdFindAddressArea(prev_replacement);
            }
            else
                prev_replacement = DFL_NULL_ADDR;  // a maximum value

            if (entry_area != prev_entry_area) // Spare area is changing, ex: from ISA0 to OSA0
            {   // reset QInsert Info
                g_QuickInsertionInfo.HashIndex  = DFL_SENTINEL;
                g_QuickInsertionInfo.EntryIndex = DFL_SENTINEL;
            }

            if (g_QuickInsertionInfo.HashIndex == new_entry_hash_index)
            {
                // Insert the new entry after the previous entry.
                if (prev_replacement < GetReplacementStartAddress(index))
                    error = FALSE;
                else
                    error = TRUE;

                if (!error)
                {
                    g_pEntries[g_QuickInsertionInfo.EntryIndex].Next = index;
                }
            }
            else
            {
                error = (pHashTable[new_entry_hash_index] != DFL_SENTINEL);
                if (!error)
                {
                    // The entry is the first one of the chain.
                    pHashTable[new_entry_hash_index] = index;
                    g_QuickInsertionInfo.HashIndex = new_entry_hash_index;
                }
            }

            if (error)
            {
                send_msg5SValue(1,"QInsert error",2,index);
                ReleaseEntry(index);
            }
            else
            {
                g_QuickInsertionInfo.EntryIndex = index;
                entry_added = TRUE;
            }
        }
    }

    return entry_added;
}


/*********************************************************************
FUNCTION    : BdreInsertEntry
DESCRIPTION : Insert a new entry for BD-RE
PARAMETERS  : entry index
RETURNS     : None
*********************************************************************/
void BdreInsertEntry(USHORT index)
{
    ULONG              start_address;
    ULONG              end_address;
    USHORT  chain_index;
    USHORT  enum_entry_index;
    USHORT  prev_entry_index = DFL_SENTINEL;
    USHORT             start_hash_index;
    USHORT             end_hash_index;
    USHORT             hash_index;
    USHORT             *pHashTable;
    BYTE               entry_area;
    BOOL               is_greater       = TRUE;


    start_address = (g_SortMethod == eSortOnDefectAddress) ? GetDefectStartAddress(index) :
                                                             GetReplacementStartAddress(index);

    end_address   = (g_SortMethod == eSortOnDefectAddress) ? BdreGetDefectEndAddress(index) :
                                 (GetReplacementStartAddress(index) + SECTORS_PER_CLUSTER - 1);

    //send_msg5SValue(SEND_ATAPI_CMD,"start_addres",4,start_address);
    //send_msg5SValue(SEND_ATAPI_CMD,"end_addres",4,end_address);

    // Get the hash table indexes of entry.
    entry_area       = BdFindAddressArea(start_address);
    start_hash_index = AddressToHashIndex(start_address, entry_area);
    end_hash_index   = AddressToHashIndex(end_address, entry_area);

    //send_msg5SValue(SEND_ATAPI_CMD,"start_hash",2,start_hash_index);
    //send_msg5SValue(SEND_ATAPI_CMD,"end_hash",2,end_hash_index);

    //Determine which hash area that we are located.
    pHashTable = BdFindHashTables(entry_area);

    // The entries are sorted on ascending addresses, insert the new entry accordingly.
    // Enumerate the entries in the chain to find the insertion location.
    chain_index = pHashTable[start_hash_index];

    //send_msg5SValue(SEND_ATAPI_CMD,"chain_index",2,chain_index);

    enum_entry_index = pHashTable[start_hash_index];
    while ((is_greater) && (enum_entry_index != DFL_SENTINEL))
    {
        // At this point there should not be any duplicate entries in the list.
        if (g_SortMethod == eSortOnDefectAddress)
            is_greater = (start_address > BdreGetDefectEndAddress(enum_entry_index));
        else
            is_greater = (start_address > (GetReplacementStartAddress(enum_entry_index) + SECTORS_PER_CLUSTER - 1));

        if (is_greater)
        {
            // Check the next entry from the chain.
            prev_entry_index = enum_entry_index;
            enum_entry_index = g_pEntries[enum_entry_index].Next;

            //send_msg5SValue(SEND_ATAPI_CMD,"prev_index",2,prev_entry_index);
            //send_msg5SValue(SEND_ATAPI_CMD,"enum_index",2,enum_entry_index);
        }
    }

    if (prev_entry_index != DFL_SENTINEL)
    {
        // A preceding entry was found, chain the new entry between the
        // previous and next chain entry.
        USHORT next_entry_index = g_pEntries[prev_entry_index].Next;
        g_pEntries[prev_entry_index].Next = index;
        g_pEntries[index].Next = next_entry_index;
        //send_msg5SValue(SEND_ATAPI_CMD,"next_index",2,next_entry_index);
    }
    else
    {
        // No preceding entry was found, set the index of the new entry
        // as chain index of the has entry.
        g_pEntries[index].Next = pHashTable[start_hash_index];

        chain_index = index;
        //send_msg5SValue(SEND_ATAPI_CMD,"BdrSetNext-1",2,index);
        //send_msg5SValue(SEND_ATAPI_CMD,"BdrSetNext-1",2,pHashTable[start_hash_index]);
    }

    // When the new entry address range covers multiple hash entries, the intermediate
    // hash entries also need to be updated.
    for (hash_index = start_hash_index; hash_index <= end_hash_index; ++hash_index)
    {
        if (hash_index != start_hash_index)
        {
            // The end hash has been reached, this one may have a valid chain entry.
            // If so, link the entry to the first chain entry.
            SendMsgCn(SEND_ATAPI_CMD,2,0x800306,A2B(hash_index));
            g_pEntries[index].Next = pHashTable[hash_index];
        }

        pHashTable[hash_index] = chain_index;
    }
}


/*********************************************************************
FUNCTION    : BdreInsertEntryCheckPba
DESCRIPTION : If a PBA entry is inserted, we need to check whether this
              entry needs to be merged or not.
PARAMETERS  : entry index
RETURNS     : None
*********************************************************************/
void BdreInsertEntryCheckPba(USHORT index)
{


}


/*********************************************************************
FUNCTION    : BdreGetDefectEndAddress
DESCRIPTION : The PBA entry may cover multiple clusters, therefore
              the DefectEndAddress needs to be processed individually.
PARAMETERS  : entry index
RETURNS     : EndAddress
*********************************************************************/
ULONG BdreGetDefectEndAddress(USHORT index)
{
    ULONG  EndAddress;
    BYTE   status1;

    status1 = DflEntryGetStatus(g_pEntries[index].Entry.DefectBlock);
    if (status1 == DFL_STAT1_PBA)
    {
        EndAddress = GetDefectStartAddress(index) + (GetReplacementStartAddress(index) * SECTORS_PER_CLUSTER) - 1;
    }
    else
    {
        EndAddress = GetDefectStartAddress(index) + SECTORS_PER_CLUSTER - 1;
    }

    return EndAddress;
}


/*********************************************************************
FUNCTION    : BdreReleaseEntry
DESCRIPTION : If the entry to be released is within a PBA entry's range,
              We will need to split the PBA entry.
PARAMETERS  : address
RETURNS     : TRUE - release OK
              FALSE - release NG
*********************************************************************/
BOOL BdreReleaseEntry(ULONG address)
{
    BOOL    entry_released = FALSE;
    USHORT  entry_index    = BdDflSearchAddress(address);
    BYTE    status1;

    //send_msg5SValue(SEND_ATAPI_CMD,"entry_index",2,entry_index);

    if (entry_index != DFL_SENTINEL)
    {
        send_msg5SValue(SEND_MRW_MSG,"Release ETY",4,address);

        status1 = DflEntryGetStatus(g_pEntries[entry_index].Entry.DefectBlock);
        switch (status1)
        {
            case DFL_STAT1_RAD:
            case DFL_STAT1_RAD_NOT_RECORDED:
            case DFL_STAT1_NRD:
                // The existing entry is a RAD or NRD entry, remove and release.
                BdRemoveEntry(entry_index);
                BdPoolReleaseEntry(entry_index);
                entry_released = TRUE;
                break;

            case DFL_STAT1_PBA:
                // The existing entry is a PBA entry, split it.
                entry_released = SplitPba(entry_index, address);
                break;

            default:
                break;
        }
    }

    return entry_released;
}


/*********************************************************************
FUNCTION    : SplitPba
DESCRIPTION : This function will be used only in BD-RE write.
              TBD.
PARAMETERS  : index - the PBA entry's index
              address - the split start position
RETURNS     : TRUE - split OK
              FALSE - split NG
*********************************************************************/
BOOL SplitPba(USHORT index, ULONG address)
{
    return TRUE;
}

/*********************************************************************
FUNCTION    : BdreAssembleDefectList
DESCRIPTION : Assemble all the entries in the POOL into DRAM start.
              For (1) - if host request drive to return Raw DFL.
              For (2) - when drive needs to update DFL to disc.
PARAMETERS  : None
RETURNS     : None
*********************************************************************/
void BdreAssembleDefectList(void)
{
    tBdreDflHeaderS *p_dfl_header;
    DflEntry *pDestination, *pSource, Temp;
    ULONG j;
    ULONG NrOfDFLEntriesCollected;
    BYTE *pTempB;
    BYTE  i,area;

    send_msg80(SEND_ATAPI_CMD,0x7E008E);
    // Clear the assembly work area
    ZeroFillDRAM(BD_ASSEMBLY_DFL_START_PAGE, BD_ASSEMBLY_DFL_AREA_SIZE);

    //Initial pointers and indexs for each type of collection
    pTempRAD = (DflEntry *)BD_TEMP_RAD_COLLECT_START;
    pTempNRD = (DflEntry *)BD_TEMP_NRD_COLLECT_START;
    pTempPBA = (DflEntry *)BD_TEMP_PBA_COLLECT_START;
    pTempSPR = (DflEntry *)BD_TEMP_SPR_COLLECT_START;
    pTempUNU = (DflEntry *)BD_TEMP_UNU_COLLECT_START;
    NrOfRAD = 0;
    NrOfNRD = 0;
    NrOfPBA = 0;
    NrOfSPR = 0;
    NrOfUNU = 0;

    // Collect the entries.
    // Collect Defect Entries first. RAD/NRD/PBA are collected and copied to assembly area. (PBA is hold)
    for (i = 0; i < pdbGetBDLayers(); i++)
    {
        if (i == 0)
            area = eAREA_DA0;
        else
            area = eAREA_DA1;
        BdreCollectDAEntries(area);
    }

    pDestination = (DflEntry *)BD_ASSEMBLY_DFL_ENTRY_START;
    // Copy and merge RAD entries from temp area to assembly area.
    // Copy and Sort by Status2 for RAD Entries
    pSource      = (DflEntry *)BD_TEMP_RAD_COLLECT_START;
    for (j = 0; j < NrOfRAD; j++)
    {
        Temp = *pSource;
        if (((BYTE)((SWAPEND_ULONG(Temp.ReplaceBlock)) >> 28)) == DFL_STAT2_WHOLE_CLUSTER)
            *pDestination++ = *pSource;

        pSource++;
    }

    pSource      = (DflEntry *)BD_TEMP_RAD_COLLECT_START;
    for (j = 0; j < NrOfRAD; j++)
    {
        Temp = *pSource;
        if (((BYTE)((SWAPEND_ULONG(Temp.ReplaceBlock)) >> 28)) == DFL_STAT2_PARTIAL)
            *pDestination++ = *pSource;

        pSource++;
    }

    // Copy and merge NRD entries from temp area to assembly area.
    pSource      = (DflEntry *)BD_TEMP_NRD_COLLECT_START;
    for (j = 0; j < NrOfNRD; j++)
        *pDestination++ = *pSource++;

    // Collect Spare Entries next. SPR/UNU are collected and copied to assembly areas.
    for (i = 0; i < 4; i++) // There are maximum 4 spare areas defined in BD Standard.
    {
        if ((i >= 2) && (pdbGetBDLayers() == BD_SINGLE_LAYER))
            break;

        area = BdreSpareAreaOrder[i];
        BdreCollectSAEntries(area);
    }

    // Copy SPR entries from temp area to assembly area.
    // Copy and Sort by Status2 for SPR Entries
    pSource      = (DflEntry *)BD_TEMP_SPR_COLLECT_START;
    for (j = 0; j < NrOfSPR; j++)
    {
        Temp = *pSource;
        if (((BYTE)((SWAPEND_ULONG(Temp.ReplaceBlock)) >> 28)) == DFL_STAT2_WHOLE_CLUSTER)
            *pDestination++ = *pSource;

        pSource++;
    }

    pSource      = (DflEntry *)BD_TEMP_SPR_COLLECT_START;
    for (j = 0; j < NrOfSPR; j++)
    {
        Temp = *pSource;
        if (((BYTE)((SWAPEND_ULONG(Temp.ReplaceBlock)) >> 28)) == DFL_STAT2_MARKED_DEFECTIVE)
            *pDestination++ = *pSource;

        pSource++;
    }

    // Copy PBA entries from temp area to assembly area.
    // Copy and Sort by Status2 for PBA Entries
    pSource      = (DflEntry *)BD_TEMP_PBA_COLLECT_START;
    for (j = 0; j < NrOfPBA; j++)
    {
        Temp = *pSource;
        if (((BYTE)((SWAPEND_ULONG(Temp.ReplaceBlock)) >> 28)) == DFL_STAT2_WHOLE_CLUSTER)
            *pDestination++ = *pSource;

        pSource++;
    }

    pSource      = (DflEntry *)BD_TEMP_PBA_COLLECT_START;
    for (j = 0; j < NrOfPBA; j++)
    {
        Temp = *pSource;
        if (((BYTE)((SWAPEND_ULONG(Temp.ReplaceBlock)) >> 28)) == DFL_STAT2_NO_RELEVANT_USERDATA)
            *pDestination++ = *pSource;

        pSource++;
    }

    // Last, Copy UNU entries from temp area to assembly area.
    pSource      = (DflEntry *)BD_TEMP_UNU_COLLECT_START;
    for (j = 0; j < NrOfUNU; j++)
        *pDestination++ = *pSource++;

    // Fill the Defect List Terminator
    // Terminator is 8 bytes; first 4 bytes sould be 0xFFFFFFFF; second part should be UpdateCounter
    pDestination->DefectBlock  = DFL_TERMINATOR;
    pDestination->ReplaceBlock = DflHeader.BDRE.UpdateCount;

    // Setup Dfl Header
    p_dfl_header = (tBdreDflHeaderS *) BD_ASSEMBLY_DFL_HEADER_START;
    p_dfl_header->IdentifierH = 'D';
    p_dfl_header->IdentifierL = 'L';
    p_dfl_header->UpdateCount = SWAPEND_ULONG(DflHeader.BDRE.UpdateCount);

    // Double check NrOfDFLEntries
    if (pdbGetBDLayers() == BD_SINGLE_LAYER)
        j = pdbGetBDIsa0Size() + pdbGetBDOsaSize();
    else
        j = pdbGetBDIsa0Size() + pdbGetBDOsaSize()*2 + pdbGetBDIsa1Size();

    if (j != (NrOfRAD + NrOfSPR + NrOfUNU))
    {
        SendMsgCn(SEND_ATAPI_CMD,4,0x800312,A4B(j));
    }

    NrOfDFLEntriesCollected = j + NrOfNRD + NrOfPBA;

    // For a read only project, NrOfDFLEntries in disc must be equal to NrOfDFLEntries we collect now.
    if (DflHeader.BDRE.NrOfDFLEntries != NrOfDFLEntriesCollected)
    {
        SendMsgCn(SEND_ATAPI_CMD,8,0x800313,A4B(DflHeader.BDRE.NrOfDFLEntries),A4B(NrOfDFLEntriesCollected));
    }

    p_dfl_header->NrOfDFLEntries    = SWAPEND_ULONG(NrOfDFLEntriesCollected);

    // Check the upper limit of NrOfDFLEntries
    if (pdbGetBDLayers() == BD_SINGLE_LAYER)
    {
        if (NrOfDFLEntriesCollected > 32759)  // Maximum DFL Entries allowed. See BD-RE Standard v2.11, page 147.
            send_msg80(SEND_ATAPI_CMD,0x710090);
    }
    else
    {
        if (NrOfDFLEntriesCollected > 65527)  // Maximum DFL Entries allowed. See BD-RE Standard v2.11, page 147.
            send_msg80(SEND_ATAPI_CMD,0x710091);
    }

    p_dfl_header->NrOfRADEntries = SWAPEND_ULONG(NrOfRAD);
    p_dfl_header->NrOfNRDEntries = SWAPEND_ULONG(NrOfNRD);
    p_dfl_header->NrOfSPREntries = SWAPEND_ULONG(NrOfSPR);
    p_dfl_header->NrOfPBAEntries = SWAPEND_ULONG(NrOfPBA);
    p_dfl_header->NrOfUnusableEntries = SWAPEND_ULONG(NrOfUNU);



    // Print part of assembly data, for debugging
    send_msg5S(SEND_MRW_MSG,"DFL Assembled");
    pTempB = (BYTE *) BD_ASSEMBLY_DFL_HEADER_START;
    for (i = 0; i < 50 ; i++)
    {
        //Please turn it on manually for debugging
        send_msg54(SEND_MRW_MSG,*pTempB,*(pTempB+1),*(pTempB+2),*(pTempB+3));
        pTempB += 4;
    }
}


/*********************************************************************
FUNCTION    : BdreCollectDAEntries
DESCRIPTION : Collect the Defect Entries in the POOL into DRAM.
PARAMETERS  : area - DA0/DA1
RETURNS     : None
*********************************************************************/
void BdreCollectDAEntries(BYTE area)
{
    USHORT *pHashTable;
    USHORT NrOfHashes;
    USHORT hash_index;
    USHORT entry_index;
    USHORT prev_chain_index = DFL_SENTINEL;
    BYTE   Status1;

    pHashTable = BdFindHashTables(area);
    NrOfHashes = BdFindNrOfHashes(area);

    SendMsgCn(SEND_ATAPI_CMD,3,0x800307,area,A2B(NrOfHashes));

    for (hash_index = 0; hash_index < NrOfHashes; hash_index++)
    {
        entry_index = pHashTable[hash_index]; //get chain index of current hash group

        // If two hash groups have the same chain index, they are in one chain.
        // Collect only one time for each chain
        if (entry_index != prev_chain_index)
        {
            // Walk through the whole chain
            while (entry_index != DFL_SENTINEL)
            {
                Status1 = GetStatus1(entry_index);
                if ((Status1 == DFL_STAT1_RAD) || (Status1 == DFL_STAT1_RAD_NOT_RECORDED))
                {
                    //copy entry to Temp RAD Area
                    pTempRAD[NrOfRAD].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempRAD[NrOfRAD].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfRAD++;
                    //send_msg5SValue(SEND_ATAPI_CMD,"RAD added",4,NrOfRAD);
                    //send_msg5SValue(SEND_ATAPI_CMD,"Ety block",4,g_pEntries[entry_index].Entry.DefectBlock);
                    //send_msg5SValue(SEND_ATAPI_CMD,"Rad block",4,(ULONG)pTempRAD[NrOfRAD].DefectBlock);
                }
                else if (Status1 == DFL_STAT1_NRD)
                {
                    //copy entry to Temp NRD Area
                    pTempNRD[NrOfNRD].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempNRD[NrOfNRD].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfNRD++;
                }
                else if (Status1 == DFL_STAT1_PBA)
                {
                    //copy entry to Temp PBA Area
                    pTempPBA[NrOfPBA].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempPBA[NrOfPBA].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfPBA++;
                }
                else
                {
                    SendMsgCn(SEND_ATAPI_CMD,2,0x800309,A2B(entry_index));
                }

                entry_index = g_pEntries[entry_index].Next; //go to next entry.
            }
        }

        prev_chain_index = pHashTable[hash_index]; // Save the chain index of the previous hash

    }
}


/*********************************************************************
FUNCTION    : BdreCollectSAEntries
DESCRIPTION : Collect the Spare Entries in the POOL into DRAM.
PARAMETERS  : area - ISA0/OSA0/OSA1/ISA1
RETURNS     : None
*********************************************************************/
void BdreCollectSAEntries(BYTE area)
{
    USHORT *pHashTable;
    USHORT NrOfHashes;
    USHORT hash_index;
    USHORT entry_index;
    USHORT prev_chain_index = DFL_SENTINEL;
    BYTE   Status1;

    pHashTable = BdFindHashTables(area);
    NrOfHashes = BdFindNrOfHashes(area);

    SendMsgCn(SEND_ATAPI_CMD,3,0x800308,area,A2B(NrOfHashes));

    for (hash_index = 0; hash_index < NrOfHashes; hash_index++)
    {
        entry_index = pHashTable[hash_index]; //get chain index of current hash group

        // If two hash groups have the same chain index, they are in one chain.
        // Collect only one time for each chain
        if (entry_index != prev_chain_index)
        {
            // Walk through the whole chain
            while (entry_index != DFL_SENTINEL)
            {
                Status1 = GetStatus1(entry_index);
                if (Status1 == DFL_STAT1_SPR)
                {
                    //copy entry to Temp SPR Area
                    pTempSPR[NrOfSPR].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempSPR[NrOfSPR].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfSPR++;
                    //send_msg5SValue(SEND_ATAPI_CMD,"RAD added",4,NrOfRAD);
                    //send_msg5SValue(SEND_ATAPI_CMD,"Ety block",4,g_pEntries[entry_index].Entry.DefectBlock);
                    //send_msg5SValue(SEND_ATAPI_CMD,"Rad block",4,(ULONG)pTempRAD[NrOfRAD].DefectBlock);
                }
                else if (Status1 == DFL_STAT1_UNUSABLE)
                {
                    //copy entry to Temp NRD Area
                    pTempUNU[NrOfUNU].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempUNU[NrOfUNU].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfUNU++;
                }
                else
                {
                    SendMsgCn(SEND_ATAPI_CMD,2,0x80030A,A2B(entry_index));
                }

                entry_index = g_pEntries[entry_index].Next; //crd_start and crd_end are 2 entries here!
            }
        }

        prev_chain_index = pHashTable[hash_index]; // Save the chain index of the previous hash

    }
}


/*********************************************************************************************************
          Functions for BD-R
*********************************************************************************************************/
BOOL SaveBdrDefectListContent(ULONG Adr)
{
    ULONG    NrOfEntries;
    DflEntry *p_dfl_entry;

    BdInitDflHashTable();   //Initialize Hash Tables of DFL Pool
    BdInitDflEntries();     //Initialize Defect Entries of DFL Pool
#if 0//currently not be use
    BdTest(4,0);            //For debugging only
#endif //currently not be use

    //Save Defect List Header Info.
    SaveBdrDflHeader(Adr);
    SendMsgCn(SEND_ATAPI_CMD,4,0x80030B,A4B(DflHeader.BDR.NrOfDFLEntries));

    if ((DflHeader.BDR.IdentifierH == 'D') && (DflHeader.BDR.IdentifierL == 'L'))
    {
        send_msg80(SEND_SStpTask_MSG,0x7100D9);  //DFL Identifier OK
        //Collect Defect Entries from Defect List
        NrOfEntries = DflHeader.BDR.NrOfDFLEntries;
        p_dfl_entry = (DflEntry *)(Adr + BD_DFL_HEADER_LEN);  //point to first entry
        while (NrOfEntries > 0)
        {
            if (!BdrDflDaAddEntry(p_dfl_entry))
            {
                send_msg80(SEND_SStpTask_MSG,0x7100D8);  //DFL Invalid
            }
            p_dfl_entry++;
            NrOfEntries--;
        }
    }
    else
    {
        send_msg80(SEND_SStpTask_MSG,0x7100DA);  //DFL Identifier Err
        return FALSE;
    }

    return TRUE;
}

void SaveBdrDflHeader(ULONG Adr)
{
    tBdrDflHeaderS *BufferPtr;

    BufferPtr = (tBdrDflHeaderS *) Adr;

    DflHeader.BDR.IdentifierH         = BufferPtr->IdentifierH;
    DflHeader.BDR.IdentifierL         = BufferPtr->IdentifierL;
    DflHeader.BDR.Format              = BufferPtr->Format;
    DflHeader.BDR.Reserved1           = BufferPtr->Reserved1;
    DflHeader.BDR.UpdateCount         = SWAPEND_ULONG(BufferPtr->UpdateCount);
    DflHeader.BDR.NrOfDFLEntries      = SWAPEND_ULONG(BufferPtr->NrOfDFLEntries);
    DflHeader.BDR.NrOfRadCrdEntries   = SWAPEND_ULONG(BufferPtr->NrOfRadCrdEntries);
    DflHeader.BDR.NrOfNRDEntries      = SWAPEND_ULONG(BufferPtr->NrOfNRDEntries);
}


BOOL BdrDflDaAddEntry(DflEntry* pEntry)
{
    ULONG temp,address,replacement;
    BYTE  status1,status2;
    BOOL  entry_added = FALSE;
    BOOL  valid       = FALSE;

    //Get the content of this entry
    temp          = SWAPEND_ULONG(pEntry->DefectBlock);
    status1       = DflEntryGetStatus(temp);
    address       = DflEntryGetAddress(temp);
    temp          = SWAPEND_ULONG(pEntry->ReplaceBlock);
    status2       = DflEntryGetStatus(temp);
    replacement   = DflEntryGetAddress(temp);

    // Check if the disc DFL entry is valid.
    valid = BdrCheckValidity(status1, status2);

    if (valid)
    {
        BYTE Layer = 0;
        BOOL found = FALSE;

        // Find the hash table to add the entry to.
        while ((!found) && (Layer < pdbGetBDLayers()))
        {
            // Check if the entry is within the range of the current hash table.
            if ((address >= pdbGetBdDAStartAddress(Layer)) && (address <= pdbGetBdDAEndAddress(Layer)))
                found = TRUE;

            if (found)
            {
                // Add the entry.
                entry_added = BdrHashTableAddEntry(address, status1, status2, replacement);
                if (!entry_added)
                {
                    // Failed to add the entry to the hash table.
                    send_msg80(1,0x710095);
                }
                else
                {
                    if(address > MaxDflEndPSN)
                    {
                        MaxDflEndPSN = address + 0x1F;
                        MaxDflEndLBA = bdConvertPSNtoLBA(MaxDflEndPSN);
                    }

                    if(address < MinDflStartPSN)
                    {
                        MinDflStartPSN = address;
                        MinDflStartLBA = bdConvertPSNtoLBA(MinDflStartPSN);
                    }
                }
            }
            else
            {
                Layer++;
            }
        }
    }
    return entry_added;
}


BOOL BdrCheckValidity(BYTE status1, BYTE status2)
{
    BOOL valid = FALSE;

    // An entry already exists for the given defect address.
    switch (status1)
    {
    case DFL_STAT1_RAD:
        switch (status2)
        {
        case DFL_STAT2_WHOLE_CLUSTER:
        case DFL_STAT2_CRD_START:
        case DFL_STAT2_CRD_END:
            valid = TRUE;
            break;

        default:
            // Invalid status2.
            break;
        }
        break;

    case DFL_STAT1_NRD:
        valid = TRUE;
        break;

    default:
        // Invalid status1.
        break;
    }
    return valid;
}




BOOL BdrHashTableAddEntry(ULONG address, BYTE status1, BYTE status2, ULONG replacement)
{
    USHORT  index;
    BOOL entry_added = FALSE;

    //send_msg5SValue(SEND_ATAPI_CMD,"address",4,address);
    //send_msg5SValue(SEND_ATAPI_CMD,"replace",4,replacement);

    if ((status1 == DFL_STAT1_RAD) && ((status2 == DFL_STAT2_CRD_START) ||(status2 == DFL_STAT2_CRD_END)))
    {
        // A CRD entry has been added. This is only allowed when the entry
        // comes from the Defect List on disc.
        entry_added = AddCrdDefect(address, status1, status2, replacement);
    }
    else
    {
        // If an entry already exists for the given defect address, then remove it
        // before adding the new one.
        BdrReleaseEntry(address);

        // There is no existing entry, allocate a new entry.
        if ((index = AllocateEntry()) != DFL_SENTINEL)
        {
            // Set the values of the new entry.
            g_pEntries[index].Entry.DefectBlock  = ((((ULONG)status1) << 28) | address);
            g_pEntries[index].Entry.ReplaceBlock = ((((ULONG)status2) << 28) | replacement);

            send_msg5SValue(SEND_MRW_MSG,"def block",4,g_pEntries[index].Entry.DefectBlock);
            send_msg5SValue(SEND_MRW_MSG,"rep block",4,g_pEntries[index].Entry.ReplaceBlock);
            // Insert the new entry into the hash table.
            BdrInsertEntry(index);
            entry_added = TRUE;
        }
    }

    return entry_added;
}




BOOL AddCrdDefect(ULONG address, BYTE status1, BYTE status2, ULONG replacement)
{
    ULONG  CrdDefectSize,CrdReplacementSize;
    USHORT crd_start_index,crd_end_index;
    BOOL   defect_added = FALSE;

    SendMsgCn(SEND_ATAPI_CMD,4,0x80030C,A4B(address));

    if ((status1 == DFL_STAT1_RAD) && (status2 == DFL_STAT2_CRD_START))
    {
        if (g_ParkedCrdStartIndex != DFL_SENTINEL)
        {
            // A CRD Start entry has been added without CRD End entry, ignore the parked entry.
            send_msg80(1,0x710098);
            ReleaseEntry(g_ParkedCrdStartIndex);
            g_ParkedCrdStartIndex = DFL_SENTINEL;
        }

        if ((g_ParkedCrdStartIndex = AllocateEntry()) != DFL_SENTINEL)
        {
            // Set the values of the new entry.
            g_pEntries[g_ParkedCrdStartIndex].Entry.DefectBlock  = ((((ULONG)status1) << 28) | address);
            g_pEntries[g_ParkedCrdStartIndex].Entry.ReplaceBlock = ((((ULONG)status2) << 28) | replacement);
            defect_added = TRUE;
        }
    }
    else if ((status1 == DFL_STAT1_RAD) && (status2 == DFL_STAT2_CRD_END))
    {
        if (g_ParkedCrdStartIndex == DFL_SENTINEL)
        {
            // A CRD End entry has been added without CRD start entry, ignore the entry.
            send_msg80(1,0x710099);
        }
        else
        {
            // A CRD End entry has been added, add the CRD entry to the defect list.
            crd_start_index = AllocateEntry();
            crd_end_index   = AllocateEntry();
            if ( (crd_start_index != DFL_SENTINEL) && (crd_end_index != DFL_SENTINEL))
            {
                // Set the CRD Start entry.
                g_pEntries[crd_start_index].Entry.DefectBlock  = g_pEntries[g_ParkedCrdStartIndex].Entry.DefectBlock;
                g_pEntries[crd_start_index].Entry.ReplaceBlock = g_pEntries[g_ParkedCrdStartIndex].Entry.ReplaceBlock;
                g_pEntries[crd_start_index].Next               = crd_end_index;
                //send_msg5SValue(SEND_ATAPI_CMD,"Set crd start",2,crd_start_index);
                //send_msg5SValue(SEND_ATAPI_CMD,"Set crd end",2,crd_end_index);

                // Set the CRD End entry.
                g_pEntries[crd_end_index].Entry.DefectBlock  = ((((ULONG)status1) << 28) | address);
                g_pEntries[crd_end_index].Entry.ReplaceBlock = ((((ULONG)status2) << 28) | replacement);

                CrdDefectSize      = address     - DflEntryGetAddress(g_pEntries[crd_start_index].Entry.DefectBlock)  + SECTORS_PER_CLUSTER;
                CrdReplacementSize = replacement - DflEntryGetAddress(g_pEntries[crd_start_index].Entry.ReplaceBlock) + SECTORS_PER_CLUSTER;
                // Check if the defect address range is equal to the replacement address range.
                if (CrdDefectSize == CrdReplacementSize)
                {
                    // Insert the entry into the Hash Table.
                    BdrInsertCrdEntry(crd_start_index);
                    defect_added = TRUE;
                    send_msg80(SEND_ATAPI_CMD,0x710096);
                }
                else
                {
                    // Error: Defect size is not equal to the replacement size, this is not
                    // possible, ignore the entry.
                    send_msg80(1,0x71009A);

                    // Release the allocated entries.
                    ReleaseEntry(crd_start_index);
                    ReleaseEntry(crd_end_index);
                }

                // Release the parked CRD Start entry.
                ReleaseEntry(g_ParkedCrdStartIndex);
                g_ParkedCrdStartIndex = DFL_SENTINEL;
            }
        }
    }

    return TRUE;
}





void BdrInsertEntry(USHORT index)
{
    ULONG              start_address;
    ULONG              end_address;
    USHORT  chain_index;
    USHORT  enum_entry_index;
    USHORT  prev_entry_index = DFL_SENTINEL;
    USHORT             start_hash_index;
    USHORT             end_hash_index;
    USHORT             hash_index;
    USHORT             *pHashTable;
    BYTE               entry_area;
    BOOL               is_greater       = TRUE;


    start_address = DflEntryGetAddress(g_pEntries[index].Entry.DefectBlock);
    end_address   = BdrGetDefectEndAddress(index);

    //send_msg5SValue(SEND_ATAPI_CMD,"start_addres",4,start_address);
    //send_msg5SValue(SEND_ATAPI_CMD,"end_addres",4,end_address);

    // Get the hash table indexes of entry.
    entry_area       = BdFindAddressArea(start_address);
    start_hash_index = AddressToHashIndex(start_address, entry_area);
    end_hash_index   = AddressToHashIndex(end_address, entry_area);

    //send_msg5SValue(SEND_ATAPI_CMD,"start_hash",2,start_hash_index);
    //send_msg5SValue(SEND_ATAPI_CMD,"end_hash",2,end_hash_index);

    //Determine which hash area that we are located.
    pHashTable = (start_address > pdbGetBdDAEndAddress(0)) ? pHashTableDA1 : pHashTableDA0;

    // The entries are sorted on ascending addresses, insert the new entry accordingly.
    // Enumerate the entries in the chain to find the insertion location.
    chain_index = pHashTable[start_hash_index];

    //send_msg5SValue(SEND_ATAPI_CMD,"chain_index",2,chain_index);

    enum_entry_index = pHashTable[start_hash_index];
    while ((is_greater) && (enum_entry_index != DFL_SENTINEL))
    {
        // At this point there should not be any duplicate entries in the list.
        is_greater = (start_address > BdrGetDefectEndAddress(enum_entry_index));

        if (is_greater)
        {
            // Check the next entry from the chain.
            prev_entry_index = enum_entry_index;
            enum_entry_index = BdrGetNextEntry(enum_entry_index);

            //send_msg5SValue(SEND_ATAPI_CMD,"prev_index",2,prev_entry_index);
            //send_msg5SValue(SEND_ATAPI_CMD,"enum_index",2,enum_entry_index);

        }
    }

    if (prev_entry_index != DFL_SENTINEL)
    {
        // A preceding entry was found, chain the new entry between the
        // previous and next chain entry.
        USHORT next_entry_index = BdrGetNextEntry(prev_entry_index);
        BdrSetNextEntry(prev_entry_index, index);
        BdrSetNextEntry(index, next_entry_index);

        //send_msg5SValue(SEND_ATAPI_CMD,"next_index",2,next_entry_index);
    }
    else
    {
        // No preceding entry was found, set the index of the new entry
        // as chain index of the has entry.
        BdrSetNextEntry(index, pHashTable[start_hash_index]);

        chain_index = index;
        //send_msg5SValue(SEND_ATAPI_CMD,"BdrSetNext-1",2,index);
        //send_msg5SValue(SEND_ATAPI_CMD,"BdrSetNext-1",2,pHashTable[start_hash_index]);
    }

    // When the new entry address range covers multiple hash entries, the intermediate
    // hash entries also need to be updated.
    for (hash_index = start_hash_index; hash_index <= end_hash_index; ++hash_index)
    {
        if (hash_index != start_hash_index)
        {
            // The end hash has been reached, this one may have a valid chain entry.
            // If so, link the entry to the first chain entry.
            SendMsgCn(SEND_ATAPI_CMD,2,0x800306,A2B(hash_index));
            BdrSetNextEntry(index, pHashTable[hash_index]);
        }

        pHashTable[hash_index] = chain_index;
    }
}




ULONG BdrGetDefectEndAddress(USHORT index)
{
    ULONG  EndAddress;
    USHORT crd_end_index;

    if (GetStatus2(index) == DFL_STAT2_CRD_START)
    {
        crd_end_index = g_pEntries[index].Next;
        EndAddress = DflEntryGetAddress(g_pEntries[crd_end_index].Entry.DefectBlock) + SECTORS_PER_CLUSTER - 1;
    }
    else
    {
        EndAddress = DflEntryGetAddress(g_pEntries[index].Entry.DefectBlock) + SECTORS_PER_CLUSTER - 1;
    }

    return EndAddress;
}


ULONG BdrGetReplacementEndAddress(USHORT index)
{
    ULONG  EndAddress;
    USHORT crd_end_index;

    if (GetStatus2(index) == DFL_STAT2_CRD_START)
    {
        crd_end_index = g_pEntries[index].Next;
        EndAddress = DflEntryGetAddress(g_pEntries[crd_end_index].Entry.ReplaceBlock) + SECTORS_PER_CLUSTER - 1;
    }
    else
    {
        EndAddress = DflEntryGetAddress(g_pEntries[index].Entry.ReplaceBlock) + SECTORS_PER_CLUSTER - 1;
    }

    return EndAddress;
}



BOOL BdrReleaseEntry(ULONG address)
{
    BOOL    entry_released = FALSE;
    USHORT  entry_index    = BdDflSearchAddress(address);
    BYTE    status1,status2;

    //send_msg5SValue(SEND_ATAPI_CMD,"entry_index",2,entry_index);

    if (entry_index != DFL_SENTINEL)
    {
        send_msg5SValue(SEND_MRW_MSG,"Release ETY",4,address);

        status1 = DflEntryGetStatus(g_pEntries[entry_index].Entry.DefectBlock);
        status2 = DflEntryGetStatus(g_pEntries[entry_index].Entry.ReplaceBlock);
        if ((status1 == DFL_STAT1_RAD) && (status2 == DFL_STAT2_CRD_START))
            entry_released = SplitCrd(entry_index, address);
        else
        {
            BdRemoveEntry(entry_index);
            BdPoolReleaseEntry(entry_index);
            entry_released = TRUE;
        }
    }

    return entry_released;
}


BOOL SplitCrd(USHORT index, ULONG address)
{
    return TRUE;
}



void BdrInsertCrdEntry(USHORT index)
{
    USHORT pre_entry_index,next_entry_index;

    // Insert the entry into the hash table.
    //send_msg5SValue(SEND_ATAPI_CMD,"crd s-index",2,index);
    BdrInsertEntry(index);

    send_msg80(SEND_ATAPI_CMD,0x710097);
    // In case of a RAD/CRD entry, the entry might need to merge with the preceding or next entry.
    if (GetStatus1(index) == DFL_STAT1_RAD)
    {
        // Check if the entry must be merged with the preceding or next entry.
        pre_entry_index = SearchPrecedingEntry(GetDefectStartAddress(index));
        //send_msg5SValue(SEND_ATAPI_CMD,"pre_e_index",2,pre_entry_index);

        if (pre_entry_index != DFL_SENTINEL)
        {
            // Preceding entry found, check if the preceding entry and the current entry are
            // contiguous RAD/CRD entries.
            ULONG defect_start_address      = GetDefectStartAddress(index);
            ULONG replacement_start_address = GetReplacementStartAddress(index);

            if ((GetStatus1(pre_entry_index) == DFL_STAT1_RAD) &&
                (BdrIsDefectAddressInRange((defect_start_address - SECTORS_PER_CLUSTER),pre_entry_index)) &&
                (BdrIsReplacementAddressInRange((replacement_start_address - SECTORS_PER_CLUSTER),pre_entry_index)))
            {
                // The entries are contiguous and must be merged.
                index = MergeCrd(pre_entry_index, index);
            }
        }

        next_entry_index = SearchNextEntry(GetDefectStartAddress(index));
        //send_msg5SValue(SEND_ATAPI_CMD,"next_e_index",2,next_entry_index);

        if (next_entry_index != DFL_SENTINEL)
        {
            // Next entry found, check if the next entry and the current entry are
            // contiguous RAD/CRD entries.
            ULONG defect_end_address      = BdrGetDefectEndAddress(index);
            ULONG replacement_end_address = BdrGetReplacementEndAddress(index);

            if ((GetStatus1(pre_entry_index) == DFL_STAT1_RAD) &&
                (BdrIsDefectAddressInRange((defect_end_address + SECTORS_PER_CLUSTER),next_entry_index)) &&
                (BdrIsReplacementAddressInRange((replacement_end_address + SECTORS_PER_CLUSTER),next_entry_index)))
            {
                // The entries are contiguous and must be merged.
                (void) MergeCrd(index, next_entry_index);
            }
        }
    }
}


USHORT MergeCrd(USHORT index1, USHORT index2)
{
    send_msg5S(1,"#MergeCrd#");
    return index1;
}


USHORT BdrGetNextEntry(USHORT index)
{
    USHORT crd_end_index;

    if (GetStatus2(index) == DFL_STAT2_CRD_START)
    {
        crd_end_index = g_pEntries[index].Next;
        return g_pEntries[crd_end_index].Next;
    }
    else
    {
        return g_pEntries[index].Next;
    }
}


void BdrSetNextEntry(USHORT index, USHORT next_index)
{
    USHORT crd_end_index;
    //BYTE   status2;

    //status2 = DflEntryGetStatus(g_pEntries[index].Entry.ReplaceBlock);

    //send_msg5SValue(SEND_ATAPI_CMD,"bdr index",2,index);
    //send_msg5SValue(SEND_ATAPI_CMD,"rep block",4,g_pEntries[index].Entry.ReplaceBlock);
    //send_msg5SValue(SEND_ATAPI_CMD,"bdr status2",1,status2);
    if (GetStatus2(index) == DFL_STAT2_CRD_START)
    {
        send_msg5S(1,"#CRD Start#");
        crd_end_index = g_pEntries[index].Next;
        g_pEntries[crd_end_index].Next = next_index;
    }
    else
    {
        g_pEntries[index].Next = next_index;
    }
}


void BdrAssembleDefectList(void)
{
    tBdrDflHeaderS *p_dfl_header;
    DflEntry *pDestination, *pSource;
    ULONG j;
    BYTE *pTempB;
    BYTE  i, area;

    send_msg80(SEND_ATAPI_CMD,0x71008E);
    // Clear the assembly work area
    ZeroFillDRAM(BD_ASSEMBLY_DFL_START_PAGE, BD_ASSEMBLY_DFL_AREA_SIZE);

    //Initial pointers and indexs for each type of collection
    pTempRAD = (DflEntry *)BD_TEMP_RAD_COLLECT_START;
    pTempNRD = (DflEntry *)BD_TEMP_NRD_COLLECT_START;
    NrOfRAD = 0;
    NrOfNRD = 0;

    // Collect the entries.
    // Collect Defect Entries. RAD/NRD are collected and put on a temp area.
    for (i = 0; i < pdbGetBDLayers(); i++)
    {
        if (i == 0)
            area = eAREA_DA0;
        else
            area = eAREA_DA1;
        BdrCollectDAEntries(area);
    }

    // Copy and merge RAD/NRD entries from temp area to assembly area.
    pDestination = (DflEntry *)BD_ASSEMBLY_DFL_ENTRY_START;
    pSource      = (DflEntry *)BD_TEMP_RAD_COLLECT_START;
    for (j = 0; j < NrOfRAD; j++)
        *pDestination++ = *pSource++;

    pSource      = (DflEntry *)BD_TEMP_NRD_COLLECT_START;
    for (j = 0; j < NrOfNRD; j++)
        *pDestination++ = *pSource++;

    // Fill the Defect List Terminator
    // Terminator is 8 bytes; first 4 bytes sould be 0xFFFFFFFF; second part should be UpdateCounter
    pDestination->DefectBlock  = DFL_TERMINATOR;
    pDestination->ReplaceBlock = DflHeader.BDR.UpdateCount;

    // Setup Dfl Header
    p_dfl_header = (tBdrDflHeaderS *) BD_ASSEMBLY_DFL_HEADER_START;
    p_dfl_header->IdentifierH = 'D';
    p_dfl_header->IdentifierL = 'L';
    p_dfl_header->UpdateCount = SWAPEND_ULONG(DflHeader.BDR.UpdateCount);
    p_dfl_header->NrOfDFLEntries    = SWAPEND_ULONG((NrOfRAD + NrOfNRD));
    p_dfl_header->NrOfRadCrdEntries = SWAPEND_ULONG(NrOfRAD);
    p_dfl_header->NrOfNRDEntries    = SWAPEND_ULONG(NrOfNRD);

    // Print part of assembly data, for debugging
    send_msg80(SEND_ATAPI_CMD,0x71008E);
    pTempB = (BYTE *) BD_ASSEMBLY_DFL_HEADER_START;
    for (i = 0; i < 50 ; i++)
    {
        send_msg54(SEND_MRW_MSG,*pTempB,*(pTempB+1),*(pTempB+2),*(pTempB+3));
        pTempB += 4;
    }
}




void BdrCollectDAEntries(BYTE area)
{
    USHORT *pHashTable;
    USHORT NrOfHashes;
    USHORT hash_index;
    USHORT entry_index;
    USHORT prev_chain_index = DFL_SENTINEL;

    pHashTable = BdFindHashTables(area);
    NrOfHashes = BdFindNrOfHashes(area);

    send_msg5SValue(SEND_MRW_MSG,"area=",1,area);
    send_msg5SValue(SEND_MRW_MSG,"NrOfHashes",2,NrOfHashes);

    for (hash_index = 0; hash_index < NrOfHashes; hash_index++)
    {
        entry_index = pHashTable[hash_index]; //get chain index of current hash group

        // If two hash groups have the same chain index, they are in one chain.
        // Collect only one time for each chain
        if (entry_index != prev_chain_index)
        {
            // Walk through the whole chain
            while (entry_index != DFL_SENTINEL)
            {
                if (GetStatus1(entry_index) == DFL_STAT1_RAD)
                {
                    //copy entry to Temp RAD Area
                    pTempRAD[NrOfRAD].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempRAD[NrOfRAD].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfRAD++;
                    //send_msg5SValue(SEND_ATAPI_CMD,"RAD added",4,NrOfRAD);
                    //send_msg5SValue(SEND_ATAPI_CMD,"Ety block",4,g_pEntries[entry_index].Entry.DefectBlock);
                    //send_msg5SValue(SEND_ATAPI_CMD,"Rad block",4,(ULONG)pTempRAD[NrOfRAD].DefectBlock);
                }
                else if (GetStatus1(entry_index) == DFL_STAT1_NRD)
                {
                    //copy entry to Temp NRD Area
                    pTempNRD[NrOfNRD].DefectBlock  = SWAPEND_ULONG(g_pEntries[entry_index].Entry.DefectBlock);
                    pTempNRD[NrOfNRD].ReplaceBlock = SWAPEND_ULONG(g_pEntries[entry_index].Entry.ReplaceBlock);
                    NrOfNRD++;
                }
                else
                {
                    SendMsgCn(SEND_ATAPI_CMD,2,0x80030A,A2B(entry_index));
                }

                entry_index = g_pEntries[entry_index].Next; //crd_start and crd_end are 2 entries here!
            }
        }

        prev_chain_index = pHashTable[hash_index]; // Save the chain index of the previous hash

    }
}



#endif /* BD_ENABLE */
