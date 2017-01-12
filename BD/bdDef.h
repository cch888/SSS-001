/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdDef.h $
*
* DESCRIPTION   This file contains BD related #define(s) and typedefs
*
* $Revision: 32 $
* $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BD_DEF_H__
#define __BD_DEF_H__

#include ".\common\globtype.h"
//#include ".\common\globals.h"
#include ".\player\Plrdb.h"
#include ".\oem\Oem_info.h"

//**********************************************************************************************
//
//                                 #Defines for BD code
//
//**********************************************************************************************


/************************************************************************************************
    Common BD Defines
*************************************************************************************************/
#define BD_DATAZONE_START_PSN       0x100000            /* Data Zone start PSN  */
#define BD_DATAZONE_START_PAA       0x20000             /* Data Zone start PAA  */

#define BD_CLUSTER_SIZE             0x10000             /* One BD Cluster in Bytes */

#define BD_SECTOR_SIZE              0x800               /* One sector = 2048 bytes */

#define SECTORS_PER_CLUSTER         32                  /* Number of sectors in one BD cluster */
#define ONE_BD_CLUSTER              SECTORS_PER_CLUSTER

#define BD_CLUSTER_MASK             0xFFFFFFE0
#define LAST_PSN_OF_BD_CLUSTER      0x0000001F          /* Ending nibble of last lba in a BD block */

#define PAA2PSN_FACTOR              8                   /* Convert PAA -> PSN multiplication factor */

#define BD_PAD_CLUSTERS             2                                           /* Number of BD clusters to pad between disc and host ptr */
#define BD_PAD_SECTORS              (BD_PAD_CLUSTERS * SECTORS_PER_CLUSTER)     /* Number of bd sectors to pad between disc and host ptrs */

#define BD_DATA_BLK_PTR_RESOLUTION  0x800
#define BD_AUX_PTR_RESOLUTION       0x800
#define BD_AUX_UCD_OFFSET           0x50

//----------------------------------------------
// Spare area size Defines
//----------------------------------------------
#define BDR_MAX_ISA0_SIZE                4096           /* cluster size */
#define BDR_MAX_OSA_SIZE                 (768 * 256)    /* cluster size */
#define BDR_MAX_ISA1_SIZE                (64 * 256)     /* cluster size */
#define BDRE_MAX_ISA0_SIZE               4096           /* cluster size */
#define BDRESL_MAX_OSA_SIZE              (64 * 256)     /* cluster size */
#define BDREDL_MAX_OSA_SIZE              (32 * 256)     /* cluster size */
#define BDRE_MAX_ISA1_SIZE               (64 * 256)     /* cluster size */

#define BDR_DEFAULT_ISA0_SIZE             BDR_MAX_ISA0_SIZE
#define BDR_DEFAULT_OSA0_SIZE             8192
#define BDR_DEFAULT_ISA1_SIZE             BDR_DEFAULT_ISA0_SIZE
#define BDR_DEFAULT_OSA1_SIZE             BDR_DEFAULT_OSA0_SIZE
#define BDRE_FULL_FORMAT_ISA0_SIZE        BDRE_MAX_ISA0_SIZE
#define BDRE_FULL_FORMAT_ISA1_SIZE        BDRE_MAX_ISA0_SIZE
#define BDRE_FULL_FORMAT_OSA_SIZE         8192          /* cluster size */

//----------------------------------------------
// TDMA size Defines
//----------------------------------------------
#define BDR_DEFAULT_ISA0_TDMA_SIZE        2048          /* cluster size */
#define BDR_DEFAULT_OSA_TDMA_SIZE         4096          /* cluster size */
#define BDR_DEFAULT_ISA1_TDMA_SIZE        2048          /* cluster size */

//----------------------------------------------
// PIC Defines
//----------------------------------------------
#define BD_PIC_IF_NUMBER                  5

#define BD_ROM_PIC_CLUSTER_SIZE           ONE_BD_CLUSTER
#define BD_ROM_PIC_START_ADDR_L0          0xB9200
#define BD_ROM_PIC_START_ADDR_L1          0x1F25C00
#define BD_ROM_PIC_IF_SIZE                (848 * BD_ROM_PIC_CLUSTER_SIZE)

#define BD_WRITABLE_PIC_CLUSTER_SIZE      0x02
#define BD_WRITABLE_PIC_START_ADDR_L0     0xD8EC0
#define BD_WRITABLE_PIC_IF_SIZE           (544 * BD_WRITABLE_PIC_CLUSTER_SIZE)


//----------------------------------------------
// TDMA Defines
//----------------------------------------------
#define BD_TDMA0_START_ADDR     0x000EE000
#define BD_TDMAL1_START_ADDR    0x01F12000
#define BD_TDMA0_LENGTH         0x10000

//----------------------------------------------
// PAC Defines
//----------------------------------------------
#define BD_INFO1_PAC1_START_ADDR_L0       0xFFC00
#define BD_INFO2_PAC2_START_ADDR_L0       0xDDD00
#define BD_INFO1_PAC1_START_ADDR_L1       0x1F00000
#define BD_INFO2_PAC2_START_ADDR_L1       0x1F22C00
#define BD_PAC_HEADER_LENGTH              0x180
#define BD_PAC_STATUS_OFFSET              0x184
//----------------------------------------------
// TRACK Defines
//----------------------------------------------
#define MAX_BDR_TRACKS          7927
#define MAX_BDR_OPEN_TRACKS     16
#define MAX_BDRE_TRACKS         1

/************************************************************************************************
    Common DI Defines
*************************************************************************************************/
#define DI_SIZE                     0x70
//DI byte 13 Hybrid disc identifier (b7-b6) DVD Layer
#define DI_DVD_MASK                 0xC0
#define NO_DVD_ROM_LAYER            0x00
#define DVD_ROM_LAYER               0x40
#define DVD_RECORDABLE_LAYER        0x80
#define DVD_REWRITABLE_LAYER        0xC0

//DI byte 13 Hybrid disc identifier (b5-b4) CD Layer
#define DI_CD_MASK                  0x30
#define NO_CD_ROM_LAYER             0x00
#define CD_ROM_LAYER                0x10
#define CD_RECORDABLE_LAYER         0x20
#define CD_REWRITABLE_LAYER         0x30

//DI byte 13 Channel bit (b3-b0)
#define CB_RESERVED                 0x00
#define CB_74_5_NM                  0x01
#define CB_69_0_NM                  0x02

/************************************************************************************************
    Common Disc Defines
*************************************************************************************************/
// Disc Structure (disc type information)
#define BD_TYPE_MASK        0x0F
#define BD_ROM              0x01
#define BD_R                0x02
#define BD_RE               0x04

// Disc Structure (layer information)
#define BD_LAYER_MASK           0xF0
#define BD_SINGLE_LAYER         0x01
#define BD_DUAL_LAYER           0x02

//DDS
#define REC_MODE_MASK           0x03
#define SRM_NO_LOW              0x00
#define RRM_NO_LOW              0x01
#define SRM_AND_LOW             0x02
#define RRM_AND_LOW             0x03
#define NULL_VALUE              0xffffffff
#define SRRI_CNT_OFFSET         12
#define DDS_1DFL_OFFSET         1120

/*----------------------------------------------------------------------------
    GENERATED SIGNAL INPUT (AWG)

    Sets up the BD system to recieve generated signal from an AWG. Need to
    have servo disabled by setting conditional WITHOUT_SERVO

    1   AWG debug mode
    0   Normal code operation

-----------------------------------------------------------------------------*/
#define BD_AWG_TEST        0

/*----------------------------------------------------------------------------
    BD_VERIFY_ID

    Used for checking the incoming PSN. Compares the assumed PSN (bdLastDecodedPSN + 1)
    with the actual ID from the auxillary area of the block

    1   Enable checking
    0   Disable checking

    NOTE: For debugging only. This conditional can be removed after verification

-----------------------------------------------------------------------------*/
#define BD_VERIFY_ID	1
#if (ENABLE_DISCARD_ICE == 1)
#define MAXICECNT   4
#endif
/*******************************************************************
    ENUMERATIONS
*******************************************************************/
typedef enum {
    eAREA_ISA0,
    eAREA_DA0,
    eAREA_OSA0,
    eAREA_OSA1,
    eAREA_DA1,
    eAREA_ISA1
}BdWritableAreaTypeE;

/* BD Header defines */
typedef enum
{
    eHeader_None,       /* None */
    eDI,                /* DI Header: "DI" */
    ePRM,               /* PAC Header: "PRM" */
    eSRRI,              /* SRRI Header: "SR" */
    eSBM,               /* SBM Header: "SB" */
    eDDS,               /* DDS Header: "DS" */
    eDFL,               /* DFL Header: "DL" */
    eDWP                /* Disc Write Protect Header "DWP" */
} HeaderTypeE;     /* Type of BD data header Enumeration */

enum
{
    eRead = 1,       /* Read */
    eWrite = 2      /* Write */
};     /* Control type of PAC rule */

enum
{
    ePACArea = 0,       /* PAC */
    eDataArea = 2,       /* DATA */
    eCtrlDataZone = 4 
};     /* Area of PAC rule */

/*******************************************************************
    STRUCTURES
*******************************************************************/

typedef __packed union
{
    __packed struct
    {
        BYTE    reserved1[2];               // Byte 54-55 : reserved
        BYTE    PreWriteFlag;               // Byte 56: status of INFO1/PreWrite area for each layer
        BYTE    reserved2[3];               // Byte 57-59 : reserved
    } BDR;

    __packed struct
    {
        BYTE    DiscCertificationFlag;      // Byte 54
        BYTE    Reserved6;                  // Byte 55
        ULONG   LVA_Pointer;                // Byte 56-59: Last Verified Address Pointer
    } BDRE;

} tBdDdsByte54_59U;

typedef __packed struct
{
    BYTE    DDSIdH;                         // Byte 0 of DDS identifier = "D"
    BYTE    DDSIdL;                         // Byte 1 of DDS identifier = "S"
    BYTE    FormatNum;                      // Byte 2: Format number
    BYTE    Reserved1;                      // Byte 3
    ULONG   UpdateCnt;                      // Byte 4: Update Count
    BYTE    Reserved2[8];                   // Byte 8
    ULONG   PSN_DA;                         // Byte 16: First PSN of Drive Area
    ULONG   Reserved3;                      // Byte 20
    ULONG   PSN_DFL;                        // Byte 24: First PSN of Defect List
    ULONG   Reserved4;                      // Byte 28
    ULONG   Start_LSN0_PSN;                 // Byte 32: PSN of first user data frame
    ULONG   Last_LSN;                       // Byte 36: Last LSN
    ULONG   ISA0_Size;                      // Byte 40: Number of cluster at Inner Spare Area 0
    ULONG   OSA_Size;                       // Byte 44: Number of cluster at Outer Spare Area
    ULONG   ISA1_Size;                      // Byte 48: Number of cluster at Inner Spare Area 1 (Layer 1)
    BYTE    SA_FullFlag;                    // Byte 52: Spare area full flag,
                                            // b7-4: reserved, b3: ISA1, b2: OSA1, b1: OSA0, b0: ISA0
    BYTE    Reserved5;                      // Byte 53
    tBdDdsByte54_59U Disc;                  // Byte 54-59: here the defines for BD-R and BD-RE are different.
    BYTE    Reserved6[4];                   // Byte 60-63: reserved
    BYTE    InfoPac1_L0Status[8];           // Byte 64
    BYTE    InfoPac2_L0Status[8];           // Byte 72
    BYTE    InfoPac1_L1Status[8];           // Byte 80
    BYTE    InfoPac2_L1Status[8];           // Byte 88
    BYTE    Reserved7[928];                 // Byte 96

    BYTE    RecMode;                        // Byte 1024: b7-2: reserved, b1: logical overwrite, b0: SRM(0), RRM(1)
    BYTE    GenFlag;                        // Byte 1025: b7-2: reserved, b1: finalized, b0: write protection flag
    BYTE    InconsistentFlag[2];            // Byte 1026: update status of each TDMS element
    ULONG   Reserved8;                      // Byte 1028
    ULONG   LstRecordedAddr;                // Byte 1032: Highest PSN recorded by host command
    ULONG   Reserved9;                      // Byte 1036
    ULONG   ISA0_TDMASize;                  // Byte 1040: number of cluster of ISA0
    ULONG   OSA_TDMASize;                   // Byte 1044: number of cluster of OSA0/1
    ULONG   ISA1_TDMASize;                  // Byte 1048: number of cluster of ISA1
    BYTE    Reserved10[36];                 // Byte 1052
    ULONG   TestZoneL0_APSN;                // Byte 1088: first available PSN at Test Zone Layer0
    ULONG   TestZoneL1_APSN;                // Byte 1092: first available PSN at Test Zone Layer1
    BYTE    Reserved11[8];                  // Byte 1096
    ULONG   CalZoneL0_APSN;                 // Byte 1104: first available PSN at Drive Calibration Zone Layer0
    ULONG   CalZoneL1_APSN;                 // Byte 1108: first available PSN at Drive Calibration Zone Layer1
    BYTE    Reserved12[8];                  // Byte 1112
    ULONG   PSN_1DFL;                       // Byte 1120: PSN of 1st TDFL cluster at TDMA zone
    ULONG   PSN_2DFL;                       // Byte 1124: PSN of 2nd TDFL cluster at TDMA zone
    ULONG   PSN_3DFL;                       // Byte 1128: PSN of 3rd TDFL cluster at TDMA zone
    ULONG   PSN_4DFL;                       // Byte 1132: PSN of 4th TDFL cluster at TDMA zone
    ULONG   PSN_5DFL;                       // Byte 1136: PSN of 5th TDFL cluster at TDMA zone
    ULONG   PSN_6DFL;                       // Byte 1140: PSN of 6th TDFL cluster at TDMA zone
    ULONG   PSN_7DFL;                       // Byte 1144: PSN of 7th TDFL cluster at TDMA zone
    ULONG   PSN_8DFL;                       // Byte 1148: PSN of 8th TDFL cluster at TDMA zone
    BYTE    Reserved13[32];                 // Byte 1152
    ULONG   PSN_SRRI_L0;                    // Byte 1184: PSN of SRRI (SRM) / L0 Space BitMap (RRM)
    ULONG   PSN_SBM_L1;                     // Byte 1188: PSN of L1 Space BitMap (RRM)
    BYTE    Reserved14[24];                 // Byte 1192
    ULONG   ISA0_APSN;                      // Byte 1216: PSN of next usable spare cluster at ISA0
    ULONG   OSA0_APSN;                      // Byte 1220: PSN of next usable spare cluster at OSA0
    ULONG   OSA1_APSN;                      // Byte 1224: PSN of next usable spare cluster at OSA1
    ULONG   ISA1_APSN;                      // Byte 1228: PSN of next usable spare cluster at ISA1
    BYTE    Reserved15[684];                // Byte 1232
    BYTE    RecDate[4];                     // Byte 1916: TDDS recoded date (Y/M/D: 4/2/2 bits BCD)
    BYTE    ManfName[48];                   // Byte 1920: manufacturers of drive that recorded TDDS
    BYTE    AddID[48];                      // Byte 1968: additional identification of drive that recorded TDDS
    BYTE    SN[32];                         // Byte 2016: serial number of drive that recorded TDDS
} tBdRReDdsS;

typedef __packed struct
{
    BYTE    DDSIdH;                         // Byte 0 of DDS identifier = "D"
    BYTE    DDSIdL;                         // Byte 1 of DDS identifier = "S"
    BYTE    FormatNum;                      // Byte 2: Format number
    BYTE    LegacyControl[93];              // Byte 3-95
    BYTE    Reserved1[420];                 // Byte 96-515
    
    ULONG   UpdateCnt;                      // Byte 516: Update Count
    BYTE    Reserved2[8];                   // Byte 520
    ULONG   PSN_DA;                         // Byte 528: First PSN of Drive Area
    ULONG   Reserved3;                      // Byte 532
    ULONG   PSN_DFL;                        // Byte 536: First PSN of Defect List
    ULONG   Reserved4;                      // Byte 540
    ULONG   Start_LSN0_PSN;                 // Byte 544: PSN of first user data frame
    ULONG   Last_LSN;                       // Byte 548: Last LSN
    ULONG   ISA0_Size;                      // Byte 552: Number of cluster at Inner Spare Area 0
    ULONG   OSA_Size;                       // Byte 556: Number of cluster at Outer Spare Area
    ULONG   ISA1_Size;                      // Byte 560: Number of cluster at Inner Spare Area 1 (Layer 1)
    BYTE    SA_FullFlag;                    // Byte 564: Spare area full flag,
                                            // b7-4: reserved, b3: ISA1, b2: OSA1, b1: OSA0, b0: ISA0
    BYTE    Reserved5;                      // Byte 565
    tBdDdsByte54_59U Disc;                  // Byte 566-571
    BYTE    Reserved6[4];                   // Byte 572: reserved
    BYTE    InfoPac1_L0Status[8];           // Byte 576
    BYTE    InfoPac2_L0Status[8];           // Byte 584
    BYTE    InfoPac1_L1Status[8];           // Byte 592
    BYTE    InfoPac2_L1Status[8];           // Byte 600
    BYTE    Reserved7[424];                 // Byte 608-1024
} tIhBdDdsS;

typedef __packed union
{
    tBdRReDdsS   BdRReDds;
    tIhBdDdsS  IhBdDds;
} tBdDdsS;

typedef __packed struct
{
    ULONG   StartPSN;
    ULONG   LRA;
} tBdSrriEntryS;

typedef __packed struct
{
    BYTE    SRRIIdH;                        // Byte 0 of SRRI identifier = "S"
    BYTE    SRRIIdL;                        // Byte 1 of SRRI identifier = "R"
    BYTE    FormatNum;                      // Byte 2: Format number
    BYTE    Reserved1;                      // Byte 3
    ULONG   UpdateCnt;                      // Byte 4: Update Count
    BYTE    Reserved2[4];                   // Byte 8
    ULONG   SRRI_Cnt;                       // Byte 12: number of SRRI entries
    BYTE    Open_Cnt;                       // Byte 16: number of open SRRs
    BYTE    Reserved3[3];                   // Byte 17
    USHORT  OpenSRRNum[16];                 //Byte20: list of open SRR numbers
    BYTE    Reserved4[12];                  // Byte 52
    tBdSrriEntryS SRRI_Entry[7927];
} tBdSrriS;

typedef __packed struct
{
    ULONG   StartPSN;
    ULONG   EndPSN;
} tBdPACSegS;

typedef __packed struct
{
    BYTE    PACIDH;                         // Byte 0 of PAC identifier = "P"
    BYTE    PACIDM;                         // Byte 1 of PAC identifier = "R"
    BYTE    PACIDL;                         // Byte 2 of PAC identifier = "M"
    BYTE    FormatNum;                      // Byte 3 Format number
    ULONG   UpdateCnt;                      // Byte 4: Update Count
    ULONG   UnknowPACRules;                 // Byte 8: Unknow PAC Rules
    BYTE    UnknowPACDiscFlag;              // Byte 12 Unknow PAC Entire Disc Flag
    BYTE    Reserved1[2];                   // Byte 13 Reserved1
    BYTE    NumofSegs;                      // Byte 15 number of segments
    tBdPACSegS PACSegment[32];              // Byte 16 Segment_0~31;
    BYTE    Reserved2[112];                 // Byte 264 reserved;
    BYTE    KnownPACEntireDiscFlag;         // Byte 384 Known PAC Entire Disc Flag
    BYTE    Reserved3[3];                      // Byte 385 reserved;
    ULONG   StsOfPAC1Location_L0;           // Byte 388 Status bit of INFO/PAC1 locations on L0
    ULONG   StsOfPAC2Location_L0;           // Byte 392 Status bit of INFO/PAC2 locations on L0
    ULONG   StsOfPAC1Location_L1;           // Byte 396 Status bit of INFO/PAC1 locations on L1
    ULONG   StsOfPAC2Location_L1;           // Byte 400 Status bit of INFO/PAC2 locations on L1            
} tBdPacS;
typedef struct
{
    ULONG   PSN;        /* Start PSN */
    BYTE    LEN;        /* Clusters in size */
} PICInfoAddrS;

typedef struct
{
    ULONG   StartPSN;
    ULONG   EndPSN;
} AddrStEndS;

typedef struct
{
    StLongU             TargetID;           /* Starting Address of transfer             */
    ULONG               ClusterCount;       /* Total clusters to be transferred       */
    ULONG               ClustersBuffered;   /* Clusters that have been transferred    */
    BYTE                BcaLen;             /* BCA Length after a BCA read.             */
    BYTE                TargetLayer;        /* BD Disc Layer.                          */
    TransferStatesE     State;              /* State of transfer                        */
    TransferTypeE       Type;               /* The type of Read to perform.             */
    BYTE                ExtendClusters;     /* Number of clusters to extend buffering */
} BDDiscTransferS;

/*-------------------------------------------------------
    Structure for flags involving BD decoding
--------------------------------------------------------*/
typedef struct
{
    BYTE    fFIRST_HOST_CLUSTER;        // 1 = Waiting for first host cluster to be buffered
    BYTE    fREADING_PIC;               // 1 = Currently reading the PIC area
    BYTE    fREADING_LEADIN;            // 1 = Currently reading the Lead-in area
    BYTE    fEXTEND_READ;               // 1 = Extend the current buffering process
    BYTE    fREAD_AHEAD;                // 1 = Read ahead buffering
    BYTE    fDISABLE_READ_AHEAD;        // 1 = Do not start read ahead buffering
//    BYTE    fIGNORE_ECC_ERRORS;         // 1 = Ignore ECC errors when buffering
    BYTE    fSTREAMING;                 // 1 = READ12 Command Streaming bit
} bdDecFlagS;

/*-------------------------------------------------------
    Structure for BD error flags
--------------------------------------------------------*/
typedef struct
{
    /* SYSTEM ERRORS */
    BYTE fUNAUTH_READ_ERROR;
    BYTE fSEEK_FAIL;
    /* DECODE ERRORS */
    BYTE fBCA_ERROR;
    BYTE fSEQ_ID_ERROR;
    BYTE fUNCOR_LDC_ECC;
    BYTE fUNCOR_BIS_ECC;
    BYTE fTID_MISS;
    BYTE fEDC_ERROR;
    BYTE fID_ERROR;
    BYTE fID_MISMATCH;
} bdErrorFlagS;

/*-------------------------------------------------------
    Structure for BD decoder interrupt flags
--------------------------------------------------------*/
typedef struct
{
    BYTE fBCA_DONE_INT;
    BYTE fECC_CLUSTER_INT;
    BYTE fECC_ALL_DONE_INT;
    BYTE fSTP_ADDR_FOUND_INT;
} bdDecIntFlagS;

/*-----------------------------------------------------------------
    BD Timer
------------------------------------------------------------------*/
typedef struct
{
    WORD    count;
    BYTE    fTIMEOUT;
    BYTE    state;
} bdTimerS;

#endif

