/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrDB.h
*
* DESCRIPTION   : This file contains all player module database definitions and
*                 typedefs that are to be used by the creators of a CD-R/RW application.
*
* $Revision: 166 $
* $Date: 11/04/25 6:13p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _PLRDB_H_
#define _PLRDB_H_

#include ".\common\sdram.h"
#include ".\servo\svo.h"
#include ".\common\chip.h"
#include ".\dvd\dvdef.h"
#include ".\common\error.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_cd_decoder.h"
#include ".\al\REG_host.h"
#include ".\al\REG_cd_encoder.h"
#include ".\al\REG_audio.h"

//***************************************************************************
//                        DATA TYPE DEFINITIONS
//***************************************************************************
//#define Info     (*(volatile tDiscDataS *)(SCSDRAM+DB_INFO_ADR))
//#define CueSheet (*(volatile tCueSheetS *)(SCSDRAM+PREGAP_TIME_ADDR))

#define  MaxNumberOfTracks     99 /* Max Num of Tracks per Disc             */
#define eMaxNumberOfSessions   50 /* Max Num of entries in Session per Disc */
/* Define relative offset from LeadIn start time 00:13:25 */
#define eFirstLeadOutLength  6750 /* Equivalent to 01:30:00   */
#define eLeadOutLength       2250 /* Equivalent to 30 seconds */
#define eLeadInLength        4500 /* Equivalent to 60 seconds */
#define eTimeEmpty           0xFF /* Indicates Empty Time     */
#define  Empty               0xFF /* Indicates Empty          */
#define  MaxNumOfBoard       156     /* Max Num of board */

// ------------------------------------------------------
// DVD definitions
// ------------------------------------------------------
#define DVD_DISC_FINALIZED      0x01
#define DVD_DISC_NOT_FINALIZED  0x00


//***************************************************************************
//                            ENUMERATIONS
//***************************************************************************
/* BYTE FindSubcode(long target_lba) */

/* Track Statement Info.Media.CD.Track[tno-1].TrackState */
enum {
    eTRACK_STATE_UNKNOWN      , /* unknown statement          */
    eTRACK_STATE_EMPTY        , /* no PMA, no pregap, no data */
    eTRACK_STATE_BLANK        , /* PMA , no pregap, no data   */
    eTRACK_STATE_PREGAP_ONLY  , /* PMA ,pregap , no data      */
    eTRACK_STATE_INCOMPLETE   , /* PMA , pregap, data         */
    eTRACK_STATE_COMPLETE     , /* PMA , pregap, full data    */
    eTRACK_STATE_DAMAGE_DATA  , /* PMA , pregap, damage data  */
    eTRACK_STATE_DAMAGE_PREGAP  /* PMA , no pregap, data      */
};

/* Track Status Info.Media.CD.Track[tno-1].TrackStatus */
enum {
    eTS_ReservedTrack = 0x80,   /* this track is reserved  */
    eTS_BlankTrack    = 0x40,   /* this track is blank     */
    eTS_Packet        = 0x20,   /* packet write            */
    eTS_FixedPacket   = 0x10,   /* fixed packet            */
    eTS_InvisibleTrack = 0x08,  /* this track is invisible */
    eTS_None          = 0x00
};

/* Track Giag status Info.Media.CD.Track[tno-1].DiagStatus[0] */
enum {
    eTS_Diag_Pregap_done = 0x80, /* diagnostic pregap is done          */
    eTS_Diag_Start_done  = 0x40, /* start position is done             */
    eTS_Diag_Stop_done   = 0x20, /* stop positionp is done             */
    eTS_Diag_TDB_done    = 0x10, /* read TDB is done                   */
    eTS_Diag_Pregap      = 0x08, /* subcode of pregap is found         */
    eTS_Diag_Start       = 0x04, /* subcode of start position is found */
    eTS_Diag_Stop        = 0x02, /* subcode of stop position is found  */
    eTS_Diag_TDB         = 0x01, /* TDB is found                       */
    eTS_Diag_All_done    = 0xF0,
    eTS_Diag_All_valid   = 0x0F
};

/* Track Giag status Info.Media.CD.Track[tno-1].DiagStatus[1] */
enum {
    eTS_NWA_done         = 0x80, /* Search NWA is done */
    eTS_NWA_valid        = 0x08  /* NWA is valid       */
};

enum {
    ePointA0, /* A0 */
    ePointA1, /* A1 */
    ePointA2  /* A2 */
};

enum {
    eNOTRead, /* Not Read-INFO */
    eReadPMA, /* Done Read-PMA */
    eReadTOC  /* Done Read-TOC */
};

enum {
    PointC0, /* C0 */
    PointC1, /* C1 */
    PointB0  /* B0 */
};

enum {
    eSubcodeMode0,
    eSubcodeMode1,
    eSubcodeMode2,
    eSubcodeMode3,
    eSubcodeMode4,
    eSubcodeMode5
};

enum {
    eSubcodeCopyBit    = 0x02,
    eSuboceControlMASK = 0x0D,
    eSubcodePacketMask = 0x05,
    eSubcodeTrackMask  = 0x04,
    eSubcodeLeadinTno  = 0x00,
    eSubcodeLeadoutTno = 0xAA
};


enum {
    eMode0Found,
    eNoMode0Found,
    eSubcodeError
};

typedef unsigned char CopyModeT;
enum {
    eCopyUnknown       ,
    eCopyProtected     ,
    eNotCopyProtected  ,
    eHomeCopyOfOriginal
};

typedef unsigned char DiscModeTypeT;
enum {
    eCDRomCDDA = 0x00,
    eCDIMode   = 0x10,
    eCDRomXA   = 0x20
};

typedef unsigned char DiscProtectionStatusT;
enum {
    eDiscIsNotProtected,
    eDiscIsProtected
};

typedef unsigned char DiscStatusT;
enum {
    eNonFinalizedDisc,
    eFinalizedDisc
};

typedef unsigned char HybridStatusT;
enum {
    eNoHybrid,
    eHybrid
};

enum {
    eType1,
    eType2
};

typedef unsigned char   PlayerStatesT;
enum {
    eStInitializing       ,
    eStTrayIn             ,
    eStTrayMovingIn       ,
    eStTrayMovingOut      ,
    eStTrayOut            ,
    eStStartingUp         ,
    eStStopping           ,
    eStPausingOnAdrs      ,
    eStSearchISRCAndCatNum,
    eStAccessing          ,
    eStCalibrating        ,
    eStInitServo          ,
    eStWriteStart         ,
    eStCalibratingDone
};

enum {
    eMZone0Full = 0x01,
    eMZone1Full = 0x10
};

typedef unsigned char RMAEmptyStatusT;
enum {
    eRMAEmpty,
    eRMANotEmpty
};

typedef unsigned char RMAUpdateStatusT;
enum {
    eNoRMAUpdate,
    eRMAUpdate
};

#if (AUDIO_PLAYBACK == 1)
typedef unsigned char AudioSourceT;
enum {                            // For register AROUTE.CHASEL bits
    eALeftBRight  = 0x00,
    eALeftBLeft   = 0x01,
    eALeftBMono   = 0x02,
    eALeftBMute   = 0x03,
    eARightBRight = 0x04,
    eARightBLeft  = 0x05,
    eARightBMono  = 0x06,
    eARightBMute  = 0x07,
    eAMonoBRight  = 0x08,
    eAMonoBLeft   = 0x09,
    eAMonoBMono   = 0x0A,
    eAMonoBMute   = 0x0B,
    eAMuteBRight  = 0x0C,
    eAMuteBLeft   = 0x0D,
    eAMuteBMono   = 0x0E,
    eAMuteBMute   = 0x0F
};

typedef unsigned char AudioDACOutputControlT;
enum {
    eMuteOffAB = 0x00,
    eMuteAOffB = 0x0C,
    eMuteBOffA = 0x03,
    eMuteAB    = 0x0F
};

    #if DIG_AUDIO == 1
typedef unsigned char AudioDigitalOutputT;
enum {                          // For register AMUTE.DMUT
    eDOut     = 0x00,
    eMuteDOut = bDMute
};
    #endif // DIG_AUDIO
#endif // AUDIO_PLAYBACK

typedef unsigned char tDiscIDStatusT;
#define DiscIDStatusT tDiscIDStatusT

typedef unsigned char tTrackDataModeT;

typedef unsigned char SessionFormatTypeT;
enum {
    eSFTypeCDda_CDRom = 0x00,
    eSFTypeCDI        = 0x10,
    eSFTypeCDRomXA    = 0x20
};

typedef unsigned char SessionCloseT;
enum {
    eEmptySession    ,
    eNotSessionClosed,
    eSessionClosed   ,
    eSessionFinalized
};

typedef unsigned char DataBlockTypeT;
enum {
    eDBTUnknownMode = 0x00,
    eDBTRawData     = 0x01,
    eDBTMode1Data   = 0x08,
    eDBTMode2Form1  = 0x10,
    eDBTMode2Form2  = 0x13
};

//#if (ENABLE_CPRM && BCA_ENABLE)
enum {
    eNO_CPS     = 0x00,
    eCSS_CPS    = 0x01,
    eCPRM_CPS   = 0x02,
    eAACS_CPS   = 0x10
};
//#endif

//***************************************************************************
//               D V D    D A T A B A S E    S T R U C T U R E S
//***************************************************************************
//                 DVD LPP INFORMATION STRUCTURE (DVD-R, DVD-RW)
//***************************************************************************
typedef struct {
        ULONG   PSN;
        BYTE    Layer;
}tPSNS;

//typedef struct
//{
//        //DVD-R
//
//        BYTE    ApplicationCode;                    // Field ID1
//        BYTE    DiscPhysicalCode;
//        ULONG   LastDataRecordableAreaAddress;
//        BYTE    PartVerExtCode;
//        BYTE    ID1Found;
//        BYTE    OPCSuggestedCode;             // Field ID2
//        BYTE    WaveLengthCode;
//        BYTE    WriteStartegyType1[4];
//        BYTE    ID2Found;
//        BYTE    ManufactureID[12];                  // Field ID3~4
//        BYTE    ID3Found;
//        BYTE    ID4Found;
//        BYTE    WriteStartegyType2[6];              // Field ID5
//        BYTE    ID5Found;
//        BYTE    OPCSuggestedBetaValue_2x;           // Field ID6
//        BYTE    OPCSuggestedRecordingPower_2x;
//        BYTE    WriteStartegyType1_2x[4];
//        BYTE    ID6Found;
//        BYTE    WriteStartegyType2_2x[6];           // Field ID7
//        BYTE    ID7Found;
//        BYTE    OPCSuggestedBetaValue_4x;           // Field ID8
//        BYTE    OPCSuggestedRecordingPower_4x;
//        BYTE    WriteStartegyType1_4x[4];
//        BYTE    ID8Found;
//        BYTE    WriteStartegyType2_4x[6];           // Field ID9
//        BYTE    ID9Found;
//        BYTE    WriteStartegyType3_4x[6];           // Field ID10
//        BYTE    ID10Found;
//        BYTE    OPCSuggestedBetaValue_4x_MP;        // Field ID11
//        BYTE    OPCSuggestedRecordingPower_4x_MP;
//        BYTE    WriteStartegyType1_4x_MP[4];
//        BYTE    ID11Found;
//        BYTE    WriteStartegyType2_4x_MP[6];        // Field ID12
//        BYTE    ID12Found;
//        BYTE    WriteStartegyType3_4x_MP[6];        // Field ID13
//        BYTE    ID13Found;
//} tLPPS;

//typedef struct
//{
//        //DVD-RW
//        BYTE    ApplicationCode;                    // Field ID1
//        BYTE    DiscPhysicalCode;
//        ULONG   LastDataRecordableAreaAddress;
//        BYTE    PartVerExtCode;
//        BYTE    ID1Found;
//        BYTE    OPCSuggestedRecordingPower1st;             // Field ID2
//        BYTE    OPCSuggestedErasingPower1st;
//        BYTE    WriteStartegy1st[4];
//        BYTE    ID2Found;
//        BYTE    ManufactureID[12];                  // Field ID3~4
//        BYTE    ID3Found;
//        BYTE    ID4Found;
//        BYTE    WriteStartegy2nd[6];              // Field ID5
//        BYTE    ID5Found;
//        BYTE    OPCSuggestedRecordingPower1st_2x;           // Field ID6
//        BYTE    OPCSuggestedErasingPower1st_2x;
//        BYTE    WriteStartegy1st_2x[4];
//        BYTE    ID6Found;
//        BYTE    OPCSuggestedRecordingPower2nd_2x;           // Field ID7
//        BYTE    OPCSuggestedErasingPower2nd_2x;
//        BYTE    WriteStartegy2nd_2x[4];
//        BYTE    ID7Found;
//} tRWLPPS;
//typedef struct
//{
//        //DVD-R DL
//        BYTE    ApplicationCode;                    // Field ID1
//        BYTE    DiscPhysicalCode;
//        ULONG   LastDataRecordableAreaAddress_L0;
//        BYTE    PartVerExtCode;
//        BYTE    ID1Found;
//        BYTE    Reserved_ID2[2];             // Field ID2
//        ULONG   LastDataRecordableAreaAddress_L1;
//        BYTE    Reserved1_ID2;
//        BYTE    ID2Found;
//        BYTE    ManufactureID[12];                  // Field ID3~4
//        BYTE    ID3Found;
//        BYTE    ID4Found;
//        BYTE    Reserved_ID5[6];              // Field ID5
//        BYTE    ID5Found;
//} tDLLPPS;
//typedef struct
//{
//        //DVD-RW DL
//        BYTE    ApplicationCode;                    // Field ID1
//        BYTE    DiscPhysicalCode;
//        ULONG   LastDataRecordableAreaAddress_L0;
//        BYTE    PartVerExtCode;
//        BYTE    ID1Found;
//        BYTE    Reserved_ID2[2];                    // Field ID2
//        ULONG   LastDataRecordableAreaAddress_L1;
//        BYTE    Reserved1_ID2;
//        BYTE    ID2Found;
//        BYTE    ManufactureID[12];                  // Field ID3~4
//        BYTE    ID3Found;
//        BYTE    ID4Found;
//        BYTE    Reserved_ID5[6];                    // Field ID5
//        BYTE    ID5Found;
//} tRWDLLPPS;

//***************************************************************************
//  Wobble Data for ADIP and LPP
//  We use either ADIP for DVD+R or LPP for DVD-R
//  defining a union to use overlapping variable space for both ADIP and LPP saves variable space
//***************************************************************************
//typedef union
//{
//        BYTE    ADIPArray[256];     // ADIP Raw data array
//        tLPPS   LPPData;            // LPP Data
//        tDLLPPS DLLPPData;          // DL LPP Data
//        tRWLPPS RWLPPData;          // RW LPP Data
//        tRWDLLPPS RWDLLPPData;      // RW DL LPP Data
//} WobbleU;  /* Structure Wobble Union  */

//***************************************************************************
//  DVD-R/-RW RMA Simple Information
//***************************************************************************

typedef struct
{
    BYTE    DiscStatus;
    StWordU InvisibleRZone;
    StWordU FirstOpenRZone;
    StWordU SecondOpenRZone;
    StWordU ThirdOpenRZone;
}Format1RMD;

typedef struct
{
    BYTE    DiscStatus;
    BYTE    RBGInformation;

    StLongU UpdateCounter;
    StLongU Format3RMDSetPointer;
    StWordU EraseOperation;
    StLongU RSDS;

    BYTE    EraseOperationCode;
    StLongU EraseInformation1;
    StLongU EraseInformation2;

}Format2RMD;

typedef struct
{
    BYTE    RBGInformation;

    BYTE    FormatOperationCode;
    StLongU FormatInformation1;
    StLongU FormatInformation2;
    StWordU LastRZoneNumber;
    StLongU LJAOfLayer0;
    StLongU LRA;
    StLongU PreLJAOfLayer0;
    StLongU JumpInterval;
    StLongU ORAddrOfLayer0;
    StLongU ORAddrOfLayer1;

    StLongU PSNDSBRMDSet;
    StLongU CertStartPSN;
    StLongU CertEndPSN;

}Format3RMD;

typedef struct
{
    StWordU InvisibleRZone;
    StWordU FirstOpenRZone;
    StWordU SecondOpenRZone;
    StWordU ThirdOpenRZone;

    StLongU SSNInvisibleRzone;
    StLongU LJAInvisibleRzone;
    StLongU ESNInvisibleRzone;
    StLongU LRAInvisibleRzone;
    StLongU PreLJAInvisibleRzone;
    StWordU JumpInterval;
}Format4RMD;

typedef struct
{
      Format1RMD Format1;
      Format2RMD Format2;
      Format3RMD Format3;
      Format4RMD Format4;
}tRMDs;


typedef struct
{
    BYTE  RMD_format;
    BYTE  Disc_status;
    ULONG SSNShiftMiddleArea;               /*DVD-R DL*/
    BYTE PreRecordedInfoCode;               /*DVD-R DL*/
    ULONG EndAddrPreRecordedLeadinArea;     /*DVD-R DL*/
    ULONG EndAddrPreRecordedMiddleAreaL0;   /*DVD-R DL*/
    ULONG EndAddrPreRecordedMiddleAreaL1;   /*DVD-R DL*/
    ULONG StartAddrPreRecordedLeadoutArea;  /*DVD-R DL*/
    tRMDs RMD;

} tRMAs;

  //*******************************************************************************************
//               DVD PHYSICAL FORMAT INFORMATION STRUCTURE (DVD-ROM DVD+R, DVD+RW DVD-RAM)
//*******************************************************************************************

typedef __packed struct
{
        // DVD-ROM
        BYTE    Reserved;                        // Byte 17-2047 reserved. No need to define here.
} tPhysFormatRom;

typedef __packed struct
{
        //DVD-R/RW
        BYTE    RevisionNumberMaxRecSpeed;      // Byte 17 of Physical Format Information
        BYTE    RevisionNumberMinRecSpeed;      // Byte 18
        char    RevisionNumberTableRecSpeed[7]; // Byte 19-25
        BYTE    Class;                          // Byte 26
        BYTE    ExtendedPartVersion;            // Byte 27
        BYTE    Reserved_0[4];                  // Byte 28-31
        ULONG   StartSectorCurrentBorderOut;    // Byte 32-35
        ULONG   StartSectorNextBorderIn;        // Byte 36-39
        BYTE    PreRecordedInfoCode;            // Byte 40  for DVD-R DL
        BYTE    TrackingPolarityARFlag;         // Byte 41  for DVD-R DL
        BYTE    RBVF;                           // Byte 42  for DVD-R DL
        BYTE    Reserved_1[469];                // Byte 43-511
        BYTE    ExtendedPreRecordedInfoCode;    // Byte 512
        BYTE    Reserved_2[7];                  // Byte 513-519

        __packed struct
        {
            BYTE PFIID0[8];
            BYTE PFIID1[8];
            BYTE PFIID2[8];

            __packed struct
            {
                BYTE  PFINum;
                char  DiscManufacturerID[6];
                BYTE  Reserved;
            }PFIID3;

            __packed struct
            {
                BYTE  PFINum;
                char  DiscManufacturerID[6];
                BYTE  Reserved;
            }PFIID4;

            BYTE PFIID5[8];
            BYTE PFIID6[8];
            BYTE PFIID7[8];
            BYTE PFIID8[8];
            BYTE PFIID9[8];
            BYTE PFIID10[8];
            BYTE PFIID11[8];
            BYTE PFIID12[8];
            BYTE PFIID13[8];
            BYTE PFIID14[8];
            BYTE PFIID15[8];
            BYTE PFIID16[8];
            BYTE PFIID17[8];
            BYTE PFIID18[8];
            BYTE PFIID19[8];
            BYTE PFIID20[8];
            BYTE PFIID21[8];
            BYTE PFIID22[8];
            BYTE PFIID23[8];
            BYTE PFIID24[8];
            BYTE PFIID25[8];
            BYTE PFIID26[8];
            BYTE PFIID27[8];
            BYTE PFIID28[8];
        }ExtPreRecInfo;                         // Byte 43-2047 No need to define here
} tPhysFormatMinus;

typedef __packed struct
{
        //DVD+R/RW
        BYTE    DiscApplicationCode;          // Byte 17 of Physical Format Information
        BYTE    ExtendedInfoIndicators;       // Byte 18
        char    DiscManufacturerID[8];        //  etc.
        BYTE    MediaTypeID[3];
        BYTE    ProductRevisionNumber;
        BYTE    NumberPhysicalFormatBytesInUse;
        BYTE    BasicWriteStrategyParameters[32];
        BYTE    ExtendedInformationBlock0[32];
        BYTE    ExtendedInformationBlock1[32];
        BYTE    ExtendedInformationBlock2[32];
        BYTE    ExtendedInformationBlock3[32];
        BYTE    ExtendedInformationBlock4[32];
        BYTE    ExtendedInformationBlock5[24];
        ULONG   StartOfSession;                  // Byte 248-251
        ULONG   EndOfSession;                    // Byte 252-255
                                                 // Byte 255-2047 reserved. No need to define here.
} tPhysFormatPlus;

typedef __packed struct
{
        //DVD-RAM
        BYTE    Reserved1[15];                  // Byte 17-31 of Physical Format Information
        BYTE    DisTypeID;                      // Byte 32 Disc Type Identification
        BYTE    Reserved2[467];                 //  etc.
        BYTE    Velocity;                       // Byte 500
        BYTE    ReadPower;                      // Byte 501
        BYTE    Reserved3[47];                  // Byte 502-548
        char    DiscManufacturerName[48];       // Byte 549-596 DiscManufacturer's name
        char    DiscManSupplInfo[16];           // Byte 597-612 DiscManufacturer's supplemenentary info
                                                // Byte 613-2047  Define as code development requires.
} tPhysFormatRam;

#if (ENABLE_HDDVD == 1)
typedef __packed struct
{
        // HDDVD-ROM
        BYTE    Reserved_0[10];                  // Byte 17-26 of reserved
        BYTE    ExtendedPartVersion;            // Byte 27
        BYTE    Reserved_1[4];                   // Byte 28-31 of reserved
        BYTE    ActualNumberOfMaxReadSpeed;     // Byte 32
        BYTE    LayerFormatTable;               // Byte 33
        BYTE    Reserved3;                      // Byte 34-2047 reserved. No need to define here.
} tPhysFormatHDRom;

typedef __packed struct
{
        //HDDVD-R/RW
        BYTE    Reserved_0[10];                  // Byte 17-26 of reserved
        BYTE    ExtendedPartVersion;            // Byte 27
        BYTE    Reserved_1[4];                   // Byte 28-31 of reserved
        BYTE    ActualNumberOfMaxReadSpeed;     // Byte 32
        BYTE    LayerFormatTable;               // Byte 33
        BYTE    Reserved_2[222];                  // Byte 34-255 no used informatoin.
        ULONG   StartSectorCurrentBorderOut;    // Byte 256-259
        ULONG   StartSectorNextBorderIn;        // Byte 260-263
        BYTE    Reserved;                       // Byte 264-2047
} tPhysFormatHDR;
#endif//(ENABLE_HDDVD == 1)

typedef __packed union
    {
            tPhysFormatRom   Rom;
            tPhysFormatPlus  Plus;
            tPhysFormatMinus Minus;
            tPhysFormatRam   Ram;
#if (ENABLE_HDDVD == 1)
            tPhysFormatHDRom HDRom;
            tPhysFormatHDR   HDR;
#endif
    } tDiscU;

typedef __packed struct
{
        BYTE    DiscTypeVersion;     // Byte 0 of Physical Format Information
        BYTE    DiscSize;            // Byte 1 of Disc size and Max transfer rate
        BYTE    DiscStructure;       // Byte 2 of Layer num and Track path and Layer type
        BYTE    RecordingDensity;    // Byte 3 of Linear density and Track dessity

        ULONG   StartPSN;            // Byte 4-7      Start PSN of data zone for all formats.
                                     //                 NOTE FOR DUAL LAYER PTP DISCS: StartPSN for layer 0 and 1 is the same
        ULONG   MaxPSN;              // Byte 8-11     End PSN of data zone for single layer or dual layer OTP discs
                                     //                 NOTE FOR DUAL LAYER PTP DISCS: MaxPSN is the end psn of layer 1
        ULONG   EndPSNLayer0;        // Byte 12-15    End PSN of layer 0 for dual layer discs

        BYTE    FlagBits;            // Byte 16       Physical Specifications documents use different names for this byte.
                                     //               For the different disc types they are named as follows:
                                     //                  DVD-ROM    Flag Bits
                                     //                  DVD+R/RW   Flag Bits
                                     //                  DVD-R/RW   NBCA Descriptor
                                     //                  DVD-RAM    BCA Descriptor
                                     //                  HD-ROM     BCA Flag

        tDiscU  Disc;                // Byte 16-2047  Disc Type specific information for DVD-ROM, DVD-R, DVD+R, DVD-RAM

} tPhysicalFormatInformationS;

//***************************************************
// End of  DVD Physical Format Information Structure
//***************************************************


//***************************************************
// Start of BD Disc Information(DI) Structure
//***************************************************
typedef __packed struct
{
    BYTE        Reserved1;                      // Byte 19
    ULONG       LastUserDataAddress;            // Byte 20-23: Last user data address
    ULONG       FAUOfDataZone;                  // Byte 24-27: first AUN of Data Zone
    ULONG       LAUOfDataZone;                  // Byte 28-31: last AUN of Data Zone
    BYTE        Reserved2[32];                  // Byte 32-63
} tBdRomDiscInformationS;

typedef __packed struct
{
    BYTE        Reserved1[5];                   // Byte 19-23
    ULONG       FPAAOfDataZone;                 // Byte 24-27: first PAA of Data Zone
    ULONG       LPAAOfDataZone;                 // Byte 28-31: last PAA of Data Zone
    BYTE        Reserved2[68];                  // Byte 32-99
    char        DiscManufacturerID[6];          // Byte 100-105
    char        MediaTypeID[3];                 // Byte 106-108
    BYTE        Reserved3[2];                   // Byte 109-110
    BYTE        ProductRevisionNumber;          // Byte 111
} tBdWritableDiscInformationS;

typedef __packed union
{
    tBdRomDiscInformationS          Bdrom;
    tBdWritableDiscInformationS     BdWritable;
} tBdDisc;

typedef __packed struct
{
    BYTE        DIIdH;                          // Byte 0 of DI unit identifier = "D"
    BYTE        DIIdL;                          // Byte 1 of DI unit identifier = "I"
    BYTE        DIFormatNumber;                 // Byte 2: Format number
    BYTE        NumberOfDIUnits;                // Byte 3: b7-b3: # of DI Units, b2-b0: layer #
    BYTE        Reserved1;                      // Byte 4
    BYTE        SeqNoInDIBlock;                 // Byte 5: DI sequence #
    BYTE        SizeOfUsedDIBytes;              // Byte 6: b7: Continuation flag, b6-b0: Number of DI bytes in use
    BYTE        Reserved2;                      // Byte 7
    BYTE        LayerTypeIdH;                   // Byte 8: BD layer type identifier = "B"
    BYTE        LayerTypeIdM;                   // Byte 9: BD layer type identifier = "D"
    BYTE        LayerTypeIdL;                   // Byte 10: BD layer type identifier = "O" / "1"
    BYTE        DiscSizeClassVersion;           // Byte 11: b7-b6: disc size, b5-b4: class number, b3-b0: version
    BYTE        BDStructure;                    // Byte 12: b7-b4: total layers, b3-b0: layer type
    BYTE        HybridDiscIdentifier;           // Byte 13: b7-b6: presence of DVD layer, b5-b4: presence of CD layer, b3-b0: channel bit
    BYTE        PushPullPolarityFlag;           // Byte 14: Push-Poll polarity flag bits
    BYTE        Reserved3;                      // Byte 15
    BYTE        BCADescriptor;                  // Byte 16: BCA descriptor
    BYTE        MaxTransferRate;                // Byte 17: Max transfer rate
    BYTE        LayerSetIdentifier;             // Byte 18: b7-b5: Logical layer number, b4-b2: Present physical layer number, b1-b0: Layer set number

    tBdDisc     Disc;                           // Byte 19 ~ 63(Bdrom) or 111(Bd-r/re)
} tBdDiscInformationS;

//***************************************************
// End of BD Disc Information(DI) Structure
//***************************************************

typedef struct
{
    // ------------------------------------------------------
    // Fields Specific to TOC Item Structure (See DVD+R Spec)
    // ------------------------------------------------------

    tPSNS   StartPSN;               // Track Start Address
    tPSNS   EndPSN;                 // Track End Address

    ULONG   LayerJumpAddress;       // Rzone Layer Jump Address
    // ------------------------------------
    // Fields Specific to Track Information
    // ------------------------------------
    BYTE    TrackStatus;            // session/track status
                                    //  eTS_ReservedTrack = 0x80
                                    //  eTS_BlankTrack    = 0x40
                                    //  eTS_Packet        = 0x20
                                    //  eTS_FixedPacket   = 0x10
    BYTE    TrackState;             // used during Track Writing Process
                                    // eTRACK_STATE_UNKNOWN      , /* unknown statement          */
                                    // eTRACK_STATE_EMPTY        , /* no PMA, no pregap, no data */
                                    // eTRACK_STATE_BLANK        , /* PMA , no pregap, no data   */
                                    // eTRACK_STATE_PREGAP_ONLY  , /* PMA ,pregap , no data      */
                                    // eTRACK_STATE_INCOMPLETE   , /* PMA , pregap, data         */
                                    // eTRACK_STATE_COMPLETE     , /* PMA , pregap, full data    */
                                    // eTRACK_STATE_DAMAGE_DATA  , /* PMA , pregap, damage data  */
                                    // eTRACK_STATE_DAMAGE_PREGAP  /* PMA , no pregap, data      */
    BYTE    TrackSessionNumber;     // session/track number
    BYTE    TrackDiagStatus0;       // used for session/track info analysis... ?
    BYTE    TrackDiagStatus1;       // used for session/track info analysis... ?
    BYTE    WriteMethod;            //
    USHORT  Con:4;                  // Control
    USHORT  DataMode:4;             // Track Data mode
    BYTE    ContAddr;               //
    CopyModeT CopyrightStatus;
    tPSNS   RecordedPSN;            // actual recorded psn
    LONG    FixedPacketSize;
    ULONG   NextWritableLBA;        // next writable address in LBA format of session/track

}tDVDTrackDataS;

//***************************************************************************
//                        END DVD DATA BASE STRUCTURES
//***************************************************************************

//***************************************************************************
//                     LEFT/RIGHT CHANNEL VOLUME STRUCTURE
//***************************************************************************
typedef struct {
    BYTE LChVolume;
    BYTE RChVolume;
} tLeftRightChannelVolumeS;
#define LeftRightChannelVolumeS tLeftRightChannelVolumeS

//***************************************************************************
//                       ISRC INFORMATION STRUCTURE
//***************************************************************************
typedef union
{
    struct
    {
        BOOL      ATIPValid;
        TimeS     ATIP;          /* BCD */
        BOOL      QSubcodeValid;
        SubQCodeS Q;
        BYTE      PFlag;
    } CD;

    struct
    {
        BOOL    IDValid;
        ULONG   ECCBLKAddress;
    } DVD;

#if (ENABLE_HDDVD == 1)
    struct
    {
        BOOL    IDValid;
        ULONG   ECCBLKAddress;
    } HD;
#endif
#if (BD_ENABLE == 1)
    struct
    {
        BOOL    IDValid;
        ULONG   ClusterAddress;
    } BD;
#endif
} tDiscInfoS;

//***************************************************************************
//                      CATALOG INFORMATION STRUCTURE
//***************************************************************************
typedef struct {          /* Extracted RAW Disc Catalog Number         */
    BOOL Valid;           /* Indicates that DiscCatalogNumber is valid */
    BYTE CatalogCode[10]; /* Extracted RAW Disc Catalog Number         */
} tCatalogInfoS;

//***************************************************************************
//                      ISRC INFORMATION STRUCTURE
//***************************************************************************
typedef struct {          /* Extracted RAW Disc ISRC code          */
    BOOL Valid;           /* Indicates if ISRCCode is Valid        */
    BYTE ISRCTrack;       /* Track Number for following ISRC bytes */
    BYTE ISRCCode[10];    /* RAW ISRC code bytes                   */
} tISRCInfoS;

//***************************************************************************
//                       RID INFORMATION STRUCTURE
//***************************************************************************
typedef struct                           /* Extracted RAW Disc ISRC code */
{
    BOOL Valid;                       /* Indicates if RIDCode is Valid */
    BYTE    RIDTrack;                    /* Track Number for following RID bytes */
    BYTE    RIDCode[10];                 /* RAW RID code bytes */
} tRIDInfoS;

//***************************************************************************
//                     LASER WRITE PARAMETERS STRUCTURE
//***************************************************************************
typedef __packed struct
{
    USHORT Power;            /* Optimum Write Power Pwo (10*mW)    */
    USHORT Bias;             /* Bias Power  (10*mW)                */
    USHORT  ID0WrPower;         /* Optimum Write Power Pwo (10*mW) in ID Layer0 */
    USHORT  ID1WrPower;         /* Optimum Write Power Pwo (10*mW) in ID Layer1 */
    USHORT  OD0WrPower;         /* Optimum Write Power Pwo (10*mW) in OD Layer0 */
    USHORT  OD1WrPower;         /* Optimum Write Power Pwo (10*mW) in OD Layer1 */
    USHORT OPCSPDInx;
    USHORT EPower;           /* Erase Power (10*mW)                */
    USHORT Vwdc1;
    USHORT  InitPlateau;      /* Initial Plateau sample at Pwo       */
    USHORT  InitReflectivity; /* Initial Reflectivity sample at Pwo  */
    /* CDR = Bata ratio; CDRW = Gamma ratio (100*Ratio)             */
    SHORT   BGamma;
    /* CDR = DeltaPower; CDRW = Optimum Erase Power Peo (100*Ratio) */
    BYTE    Ratio;
    BYTE biasp2ratio;
    BYTE biasp3ratio;
} tLaserWriteParametersS;

#define LaserWriteParametersS (tLaserWriteParametersS)

//***************************************************************************
//                        SUBCODE HEADER STRUCTURE
//***************************************************************************
typedef struct {
    BOOL Detected;       /* Offset between SubQ MSF and Header MSF detected */
    BYTE NumberOfBlocks; /* Offset in Blocks */
} tSubcodeHeaderOffsetS;

/* CDRW Write/Erase Parameters from ATIP Additional Information 1 */
//***************************************************************************
//                       ATIP ADDITIONAL INFO #1 STRUCTURE
//                          CDRW Write/Erase Parameters
//***************************************************************************
typedef struct {
    BYTE LowestCLVRecordingSpeed;   /* Lowest CLV Recording Speed      */
    BYTE HighestCLVRecordingSpeed;  /* Highest CLV Recording Speed     */
    BYTE MultiFactor;               /* Power Multiplication factor (r) */
    BYTE TargetGamma;               /* Modulation/Power Function (g)   */
    BYTE PowerRatio;                /* Erase/Write Power ratio (e)     */
} tCDRWParamsS;
#define CDRWParamsS tCDRWParamsS

//***************************************************************************
//                     ATIP ADDITIONAL INFO #2 STRUCTURE
//                              Speed Settings
//***************************************************************************
typedef struct {
    BYTE RecordPowerAtLowestSpeed;  /* Target writing power Pind for Low     */
    BYTE RecordPowerAtHighestSpeed; /* Target writing power Pind for High    */
    BYTE MultiFactorAtLowestSpeed;  /* Power Multiplication factor (r), Low  */
    BYTE MultiFactorAtHighestSpeed; /* Power Multiplication factor (r), High */
    BYTE PowerRatioAtLowestSpeed;   /* Erase/Write Power ratio (e), for Low  */
    BYTE PowerRatioAtHighestSpeed;  /* Erase/Write Power ratio (e), for High */
} tATIPAdditionalInfo2S;

//***************************************************************************
//                    ATIP ADDITIONAL INFO #3 STRUCTURE
//                            Media Information
//***************************************************************************
typedef struct {
    BYTE MediaTechnologyType;       /* Media technology type                       */
    WORD MediaIDentificationCode1;  /* Media IDentification (MID) code first part  */
    BYTE MediaIDentificationCode2;  /* Media IDentification (MID) code second part */
    BYTE ProductRevisionNumber;     /* Product revision number                     */
} tATIPAdditionalInfo3S;

//***************************************************************************
//                         ATIP INFORMATION STRUCTURE
//***************************************************************************
typedef struct {
    TimeS LeadInStartTime;        /* SI2,Absolute Minute, Second,Frame in BCD */
    TimeS LeadOutStartTime;       /* SI2,Absolute Minute, Second,Frame in BCD */
    BYTE  ApplicationCode;        /* SI1,Disc Application code                */
    BYTE  RecordPower;            /* SI1,Target Writing Power Pind            */
    tCDRWParamsS CDRWParameters;  /* Additional Info 1, Recording Parameters  */
    BYTE  AdditionalCapacity;     /* Additional Capacity in Additional Info 1    */
    BYTE  ReferenceSpeed;         /* SI1,Pind CLV Reference Speed             */
    BYTE  DiscSubType;            /* SI1,Disc Sub Type                        */
    BYTE  AdditionalInfo;         /* Addi1,2,3 present                        */
                                  /* Add2,new in Orange book part II,ve2.0    */
    tATIPAdditionalInfo2S AdditionalInfo2;
                                  /* Add3,new in Orange book part II,vol2 ,ver1.2 */
    tATIPAdditionalInfo3S AdditionalInfo3;
    BOOL  CDRW;                   /* SI1,Disc Type                            */
    BOOL  AdditionalInfo1Present; /* SI1,Additional Info 1 Present on disc    */
    BOOL  AdditionalInfo1Found;   /* Additional Info Field Found              */
    BOOL  LeadInFound;            /* Lead In  Start Time Found                */
    BOOL  LeadOutFound;           /* Lead Out Start Time Found                */
    BOOL  RecordPowerFound;       /* Record Power found                       */
    BOOL  AdditionalInfo2Present; /* AI2 is present in leadin                 */
    BOOL  AdditionalInfo2Found;   /* AI2 found                                */
    BOOL  AdditionalInfo3Present; /* AI3 is present in leadin area                */
    BOOL  AdditionalInfo3Found;   /* AI3 found                                    */
    TimeS AdditionalInfo1Pure;
    TimeS AdditionalInfo2Pure;
    TimeS AdditionalInfo3Pure;
} tATIPInfoS;

//***************************************************************************
//                        TABLE OF CONTENTS STRUCTURE
//***************************************************************************
typedef struct {
    BYTE      TrackSessionNumber;
    USHORT      Con:4;              /* Control               */
    USHORT      DataMode:4;         /* Track Data mode       */
    CopyModeT CopyrightStatus;
    BYTE      ContAddr;             /* Control/Address       */
    TimeS     StartTime;
    TimeS     StopTime;
    TimeS     RecordedTime;         /* Actual Recorded Time  */
    BYTE      WriteMethod;          /* Write method          */
    /* RawData(1),Mode1(8),Mode2From1(10),Mode2From2(13) */
    BYTE      DataBlockType;
    BYTE      DataForm;             /* Set with Cu sheet     */
    LONG      FixedPacketSize;      /* fixed packet size     */
    ULONG     NextWritableAddress;  /* Next writable address */
    /* RT(bit7),Blank(bit6),Packet(bit56),FP(bit4),TDB(bit3) */
    BYTE      TrackStatus;
    BYTE      TrackState;
    BYTE      TrackDiagStatus0;
    BYTE      TrackDiagStatus1;
    BOOL      ISRCValid;
    BYTE      ISRC[10];
} tTOCDataS;

//***************************************************************************
//                       PRE-GAP TIME INFORMATION STRUCTURE
//***************************************************************************
typedef struct {
    BOOL  PreGapStartTimeValid; /* Start Time valid                       */
    TimeS PreGapStartTime;      /* Start Time of Pregap by Send Cue Sheet */
} tPreGapTimeS;


//***************************************************************************
//                           DISC CONTROL STRUCTURE
//***************************************************************************
typedef struct {
    BYTE ContA0;
    BYTE ContA1;
    BYTE ContA2;
} tDiscContAxS;

#define DiscContAxS tDiscContAxS

//***************************************************************************
//                           DCB INFORMATION STRUCTURE
//***************************************************************************
typedef union {
    BYTE All;
    struct {
        BYTE FDCB:2;
        BYTE SDCB:1;
        BYTE TOC:1;
        BYTE WDCB:1;
    } Bit;
} tDCBExistS;

typedef __packed struct
{
    // FDCB Related Fields
    ULONG   ContentDescriptor;                  // Identifies Format DCB - set to 46444300 ("FDC")
    ULONG   UnknownContentDescriptor;           // Shall be set to (0000000D)
    BYTE    DriveID[32];                        // Drive ID that has written the DCB
    ULONG   FDCBUpdateCount;                    // FDCB update count
    BYTE    FormatStatus[4];                    // FDCB Format status
    ULONG   LastWrittenAddress;                 // Last written address
    ULONG   LastVerifiedAddress;                // Last verified address
    ULONG   BitMapStartAddress;                 // Format bitmap start address
    ULONG   BitMapLength;                       // Bitmap length
    BYTE    DiscID[32];                         // Nonzero in Lead-in area, Zero in Inner Session area
    BYTE    Application[32];                    // For copyright protection data etc
    ULONG   DCBList[16];                        // List of DCBs
    BYTE    Reserved[1856];                     // 0x00
    BYTE    FormatBitMap[18432];                // Format Bitmap
}tFDCBS;


typedef __packed struct
{
    // WDCB Related Fields
    ULONG   ContentDescriptor;                  // Identifies Session DCB - set to 57444300 ("WDC")
    ULONG   UnknownContentDescriptor;           // Shall be set to (0000000F)
    BYTE    DriveID[32];                        // Drive ID that has written the DCB
    ULONG   WDCBUpdateCount;                    // WDCB update count
    ULONG   WDCBWriteProtection;                // WDCB write protect actions
    BYTE    Reserved1[16];                      // 0x00
    BYTE    WDCBPassword[32];                   // WDCB password
    BYTE    Reserved2[1952];                    // 0x00
}tWDCBS;

//***************************************************************************
//                     DISC DATA STRUCTURE (ATIP/LPP/ADIP)
//***************************************************************************
typedef struct
{
    BYTE                    ServoReadyStatus;
    DiscStatusT             DiscStatus;      /* Current Disc Type             */
    DiscProtectionStatusT   DiscProtectionStatus;
    tDiscIDStatusT          DiscIDStatus;

    union
    {
        struct {
            /* Player has succesfully spunup disc and extracted all disc info */
            BYTE                   LowestRecordedTrackNumber;  /* Hex,Low  RecTrack Num on the Curdisc */
            BYTE                   HighestRecordedTrackNumber; /* Hex,High RecTrack Num on the Curdisc */
            TimeS                  LeadOutStartTime;       /* LeadOut StartTime;Abs MSFtime in Hex */
            TimeS                  ATIPLeadOutStartTime;          /* LeadOut StartTime for 90min disc */

            /* Indicates if a update to PMA needs to be done */
            HybridStatusT          HybridStatus;    /* Disc Hybrid status            */
            DiscModeTypeT          DiscModeType[eMaxNumberOfSessions];
            BYTE                   SessionCounter;  /* Number of Sessions on the disc*/

            /* 1st Track in Open Session, or 1st Track in Last Closed Session */
            BYTE                   FirstTrackInLastSession;
            tTOCDataS              TOCData[MaxNumberOfTracks];/* TOC Data Stru for up to 99 trks */
                                           /* control of A0,A1,A2 in TOC on Disc */
            tDiscContAxS           DiscContAx[eMaxNumberOfSessions];
        } CD;

        struct
        {
        //---------------------------------------------------------------
        // Note: DVD Variable Naming Convention
        //       "PSN" is of type ULONG of the structure tPSNS
        //       "Address" is of type LONG (aka LBA)
        //---------------------------------------------------------------
            BYTE    DiscKind; //seems never used?

            tRMAs   RMAData;            // Field Names


        // ----------------------------------------------------
        //      POINTER to permanent PHYSICAL FORMAT INFORMATION IN SDRAM
        // ----------------------------------------------------
            //WobbleU Wobble;          // Shared space: Wobble data for ADIP (DVD+R)
            tPhysicalFormatInformationS  *PFInfoP;
            BYTE    fPFInfoValid;               // Flag to indicate PFInfo data is valid
            ULONG   PTPMaxPSNLayer1;            // Maximum PSN of layer 1 for a dual layer PTP disc
                                            // Note:    The physical format info for layer 0 and 1
                                            //          on a dual layer PTP disc is the same
                                            //          except the max psn. The max psn for layer 1
                                            //          will be stored in in PTPMaxPSNLayer1
            BYTE    fBorderInfoValid;
            BYTE    fDiscInfoValid;
        // ------------------------------------------
        //        DVD STATIC DISC INFORMATION
        // ------------------------------------------
            tPSNS   LeadOutStartPSN;
            tPSNS   LeadInStartPSN;

            BYTE    DiscID[32];                         // Nonzero in Lead-in area, Zero in Inner Session area


        // ------------------------------------------
        //         DVD CURRENT DISC STATUS
        // ------------------------------------------
            BYTE    LowestRecordedTrackNumber;          // Lowest  Recorded Logical Track Number on Disc
            BYTE    LogicalTracks;                      // Number of recorded Logical Tracks



            BYTE    SessionCounter;                     // Number of Sessions on the disc

        // RMA = Recording Management Area
        // DVD +R - SDCB
            BYTE   BorderZoneCount;

        /* 1st Track in Open Session, or 1st Track in Last Closed Session */
            BYTE                    FirstTrackInLastSession;
            BYTE                    FirstTrackNumberNotInRMA;          /* First Track Number not recorded in PMA */
            ULONG                   NewLaserRecordPSN;

        // ------------------------------------------
        //          CSS
        // ------------------------------------------
            BYTE    fCSS_PROTECTED;         // CSS Protection flag
            BYTE    RegionControl;          // Region control information

        #if (ENABLE_CPRM && BCA_ENABLE)
        // ------------------------------------------
        //          CPRM
        // ------------------------------------------
            BYTE    TotalMKBPacks;          // Total MKB packs used
        #endif

        // ------------------------------------------
        //          DVD TRACK INFORMATION
        // ------------------------------------------
             tDVDTrackDataS Track[257]; // may change to support dvd-r
             ULONG APRemappingPSN[4];
             BYTE  BorderCount;
             tPSNS BorderOutPSN[MaxNumOfBoard];
             tPSNS BorderInPSN[MaxNumOfBoard];

        // ------------------------------------------
        //          LOGICAL INFORMATION
        // ------------------------------------------
            ULONG   MaxLBA;          // Last logical sector of the disc
            ULONG   EndLBALayer0;    // Last logical sector of layer 0

        /*--------------------------------------------
            For DVD-R DL only
        ---------------------------------------------*/
            ULONG LayerJumpLBA;

        /* For DVD+R DL only */
                                                        // going on during writing

            ULONG   NextBorderInPSN;  /* for DVD-R */
            BYTE    CPR_MAI;          // Copyright management Information
        } DVD;


        struct
        {
            // ------------------------------------------
            //         BD CURRENT DISC STATUS
            // ------------------------------------------
            BYTE    LowestRecordedTrackNumber;          // Lowest  Recorded Logical Track Number on Disc
            BYTE    LogicalTracks;                      // Number of recorded Logical Tracks


            BYTE    SessionCounter;                     // Number of Sessions on the disc

            // 1st Track in Open Session, or 1st Track in Last Closed Session
            BYTE    FirstTrackInLastSession;

            // ------------------------------------------
            //          BD TRACK INFORMATION
            // ------------------------------------------
             tDVDTrackDataS Track[257];     // use tDVDTrackDataS for BD track strcture

            // ------------------------------------------
            //          LOGICAL INFORMATION
            // ------------------------------------------
            ULONG   AvailPSNTestZoneLayer0;   // Next Available PSN of Test Zone Layer 0
            ULONG   AvailPSNTestZoneLayer1;   // Next Available PSN of Test Zone Layer 1
            ULONG   MaxLBA;          // Last logical sector of the disc
            ULONG   EndLBALayer0;    // Last logical sector of layer 0

            ULONG   EndPSNLayer0;    // Last PSN of layer 0
            ULONG   EndPSNLayer1;    // Last PSN of layer 1

            // Disc Information (DI)
            BYTE    fDIInfoValid;
            tBdDiscInformationS     *DiscInfoP_L0_Fmt1; //BD-ROM/R/RE
            tBdDiscInformationS     *DiscInfoP_L1_Fmt1; //BD-ROM/R/RE
            tBdDiscInformationS     *DiscInfoP_L0_Fmt2; //BD-R/RE
            tBdDiscInformationS     *DiscInfoP_L1_Fmt2; //BD-R/RE
        } BD;
    } Media;
} tDiscDataS;



//***************************************************************************
//                        DISC STATUS STRUCTURE
//***************************************************************************
typedef struct {
    BOOL           Valid;
    eDiscDiameter  DiscDiameter; /* Disc Diameter */
    discKindT      DiscKind;     /* Disc Kind     */
    eDiscType      DiscType;     /* Disc Type     */
} tStatusInfoS;

//***************************************************************************
//                     ERROR INFORMATION STRUCTURE
//***************************************************************************
typedef struct {
    BOOL               Valid;
    ErrorGroupServoT   ServoError;
    ErrorGroupPlayerT  PlayerError;
    ErrorGroupDiscT    DiscError;
    ErrorGroupCommandT CommandError;
    ErrorGroupTrayT    TrayError;
} tErrorInfoS;


//***************************************************************************
//                   INFORMATION DATABASE STRUCTURE
//***************************************************************************
typedef struct {
    BYTE             ServoState;
    BYTE             ServoExecutionState;
    tStatusInfoS     StatusInfo;
    tErrorInfoS      ErrorInfo;
    tCatalogInfoS    Catalog;
    tISRCInfoS       ISRC;
    tRIDInfoS        RID;
    SubQCodeS        SubQ;
} tInfoDBaseS;

//***************************************************************************
//                  FINAL SESSION INFORMATION STRUCTURE
//***************************************************************************
typedef union
{
    struct
    {
        BYTE  SessionNumber;        /* Final Session Number             */
        BYTE  FirstTrackNumber;     /* First Track Number               */
        BYTE  LastTrackNumber;      /* Last Track Number                */
        SessionFormatTypeT Format;  /* CD-DA/CD-ROM/,CD-I,CD-ROM XA     */
        SessionCloseT CloseInfo;    /* eNotSessionClosed,eSessionClosed */
        tCatalogInfoS Catalog;      /* Catalog                          */
        TimeS LeadinTime;           /* Leadin Start Time         (HEX)  */
        TimeS LeadoutTime;          /* Leadout End Time          (HEX)  */
        TimeS NextSessionStartTime; /* Next Session Program Start Time  */
        TimeS FirstTrackTime;       /* First Track Start Time           */
    } CD;

    struct
    {
        BYTE  SessionNumber;        /* Final Session Number             */
        BYTE  FirstTrackNumber;     /* First Track Number               */
        BYTE  LastTrackNumber;      /* Last Track Number                */
        SessionFormatTypeT Format;  /* CD-DA/CD-ROM/,CD-I,CD-ROM XA     */
        SessionCloseT CloseInfo;    /* eEmptySession,
                                       eNotSessionClosed,
                                       eSessionClosed,
                                       eSessionFinalized,
                                       eNotSessionClosed,
                                       eSessionClosed */
        ULONG  LeadinPSN;            /* Leadin Start Time         (HEX)  */
        ULONG  LeadoutPSN;           /* Leadout End Time          (HEX)  */
        ULONG  NextSessionStartPSN;  /* Next Session Program Start Time  */
        ULONG  FirstTrackPSN;        /* First Track Start Time           */
    } DVD;


    struct
    {
        BYTE  SessionNumber;        /* Final Session Number             */
        BYTE  FirstTrackNumber;     /* First Track Number               */
        BYTE  LastTrackNumber;      /* Last Track Number                */
        SessionFormatTypeT Format;  /* CD-DA/CD-ROM/,CD-I,CD-ROM XA     */
        SessionCloseT CloseInfo;    /* eEmptySession,
                                       eNotSessionClosed,
                                       eSessionClosed,
                                       eSessionFinalized,
                                       eNotSessionClosed,
                                       eSessionClosed */
        ULONG  LeadinPSN;            /* Leadin Start Time         (HEX)  */
        ULONG  LeadoutPSN;           /* Leadout End Time          (HEX)  */
        ULONG  NextSessionStartPSN;  /* Next Session Program Start Time  */
        ULONG  FirstTrackPSN;        /* First Track Start Time           */
    } BD;

} tFinalSessionInfoS;

/**************** Public Variables ****************/
extern tDiscDataS Info;
extern tInfoDBaseS        BufInfo;
extern discKindT      LoadedDiscKind;     /* Disc Kind     */
extern discKindT      ForBCALoadedDiscKind;     /* For BCA Disc Kind     */
extern volatile tDiscInfoS  Iram_DiscInfo;

extern tFinalSessionInfoS Iram_FinalSessionInfo; /* Last Session Information */
extern BYTE               ReadInfoMethod;



extern BOOL fRawModeWritten;

extern BYTE fEnableROC;
extern BYTE fRecoverLevel;

extern discKindT fReAdjustType;

extern BYTE fJumpMode;

extern BOOL C0FoundTOC;
extern BOOL C1FoundTOC;
extern BYTE Mode5SubQ[2][10];
extern BOOL fAtipInfoGet;
extern BOOL fADIPLPPInfoGet;
#if (VCPS_TEST == 1)
extern BOOL fVCPSInfoGet;
extern BOOL fVCPSInfoValid;
#endif  /* VCPS_TEST == 1 */
#if (ENABLE_AACS == 1)
extern BOOL fRomMarkInfoGet;
#endif  /* ENABLE_AACS == 1 */


extern BOOL fB0valid;
//======================================================
extern ErrorGroupDiscInfoT      fErrorGroupDiscInfo; /* Disc Info */
extern volatile DiscSpeedT     Iram_fRequestedSpeed;
extern BOOL        fNextSessionAvailable;
extern ErrorGroupReadT     fErrorGroupRead;     /* Read      */
extern ErrorGroupTrayT     fErrorGroupTray;     /* Tray      */
extern eOutputFormat fOutPutFormat;


/************************************************************************
// Get - Disc Status
*************************************************************************/
DiscStatusT pdbGetDiscStatus(void);
/************************************************************************
// Get LeadOut Start Time
*************************************************************************/
TimeS pdbGetLeadOutStartTime(void);
/************************************************************************
// Set/Get - Track Session Number
*************************************************************************/
void pdbSetTrackSessionNumber(BYTE TrackNumber,BYTE SessionNumber);
BYTE pdbGetTrackSessionNumber(BYTE TrackNumber);
/************************************************************************
// Set/Get - Track ConAd
*************************************************************************/
//void pdbSetTrackConAd(BYTE TrackNumber,BYTE ControlAddress);
BYTE pdbGetTrackConAd(BYTE TrackNumber);
/************************************************************************
// Set/Get - Track Data Mode
*************************************************************************/
void pdbSetTrackDataMode(BYTE TrackNumber,tTrackDataModeT DataNibble);
tTrackDataModeT pdbGetTrackDataMode(BYTE TrackNumber);
/************************************************************************
// Set - Start Time
*************************************************************************/
void pdbSetStartTime(TimeS TimeFrame,BYTE TrackNumber);
TimeS pdbGetStartTime(BYTE TrackNumber);
/************************************************************************
// Set/Get - Stop Time
*************************************************************************/
void pdbSetStopTime(TimeS TimeFrame,BYTE TrackNumber);
TimeS pdbGetStopTime(BYTE TrackNumber);
/************************************************************************
// Get - Number Of Sessions On Disc
*************************************************************************/
BYTE pdbGetNumberOfSessionsOnDisc(void);
/************************************************************************
// Get - First Track Of Session
*************************************************************************/
BYTE pdbGetFirstTrackOfSession(BYTE SessionNumber);
/************************************************************************
// Get LastTrack Of Session
*************************************************************************/
BYTE pdbGetLastTrackOfSession(BYTE SessionNumber);
/************************************************************************
// Get Session LeadOut Start
*************************************************************************/
TimeS pdbGetSessionLeadOutStart(BYTE SessionNumber);
/************************************************************************
// Set Clear All
*************************************************************************/
void pdbSetClearAll(void);
/************************************************************************
// Set Clear All BufInfo
*************************************************************************/
void pdbSetClearBufInfoAll( void );
/************************************************************************
// Set Disc Protection Status
*************************************************************************/
#define pdbSetDiscProtectionStatus(x)      (Info.DiscProtectionStatus = x)
#define pdbGetDiscProtectionStatus(x)      (Info.DiscProtectionStatus)
/************************************************************************
// Set Clear Errors
*************************************************************************/
void pdbSetClearErrors(void);
/************************************************************************
// Set/Get - Actual Speed
*************************************************************************/
void pdbSetActualSpeed(DiscSpeedT Speed);
DiscSpeedT pdbGetActualSpeed(void);
/************************************************************************
// Servo Clear Errors
*************************************************************************/
void pdbServoClearErrors(void);
/************************************************************************
// Set - ABSTime
*************************************************************************/
void pdbSetABSTime(TimeS *ABSTime);
/************************************************************************
// Get - ATIP Time
*************************************************************************/
void pdbGetATIPTime(TimeS *time);
/************************************************************************
// Get - ATIP TimeBin
*************************************************************************/
TimeS pdbGetATIPTimeBin(void);
/************************************************************************
// Get - Mode0 On Disc
*************************************************************************/
BYTE pdbGetMode0OnDisc(void);
/************************************************************************
// Set/Get - Catalog Code
*************************************************************************/
void pdbSetCatalogCode(SubQCodeS *SubCode);
BOOL pdbGetCatalogCode(BYTE *pCatalogCode);

/************************************************************************
// Set/Get ISRC Code
*************************************************************************/
void pdbSetISRCCode(SubQCodeS *SubCode);
BYTE pdbGetISRCCode(BYTE *pISRCCode);

/************************************************************************
// Set/Get - RID Code
*************************************************************************/
void pdbSetRIDCode(SubQCodeS *SubCode);
BYTE pdbGetRIDCode(BYTE *pRIDCode);
/************************************************************************
// Get - Subcode Information
*************************************************************************/
BOOL pdbGetSubcodeInformation(SubQCodeS *subc);
/************************************************************************
// Get - Atip Ok
*************************************************************************/
BOOL pdbGetAtipOk(void);
/************************************************************************
// Convert- Power Ref/MultiFactor
*************************************************************************/
USHORT pdbConvertPowerRef(BYTE Pind);
USHORT pdbConvertMultiFactor(BYTE Multif);
/************************************************************************
// Get - CDRW Parameters
*************************************************************************/
void pdbGetCDRWParameters(SpeedT speed,tLaserWriteParametersS *lwp);
//////////////////////////////////////////////////////////////////////////
//
//      FROM PLRADDB.H
//
/////////////////////////////////////////////////////////////////////////

/*************************************************************************/
/*                        Function Prototypes from plraddb.h             */
/*************************************************************************/
/************************************************************************
// Set - Track Cont Addr
*************************************************************************/
void pdbSetTrackContAddr(BYTE tno,BYTE ContAddr);
/************************************************************************
// Set - Track Recorded Time
*************************************************************************/
void pdbSetTrackRecordedTime(BYTE tno,TimeS time);

/************************************************************************
// Set - Track Data Block Type
*************************************************************************/
void pdbSetTrackDataBlockType(BYTE tno,BYTE BlockType);
/************************************************************************
// Set - Track Data Form
*************************************************************************/
void pdbSetTrackDataForm(BYTE tno,BYTE DataForm);
/************************************************************************
// Set - Track Packet Size
*************************************************************************/
void pdbSetTrackPacketSize(BYTE tno,LONG PacketSize);
/************************************************************************
// Set - Track State
*************************************************************************/
void pdbSetTrackState(BYTE tno,BYTE TrackState);
/************************************************************************
// Set - Track Status
*************************************************************************/
void pdbSetTrackStatus(BYTE tno,BYTE TrackStatus);
/************************************************************************
// BitSet/BitReset - Track Status
*************************************************************************/
void pdbBitSetTrackStatus(BYTE tno,BYTE TrackStatus);
void pdbBitResetTrackStatus(BYTE tno,BYTE TrackStatus);
/************************************************************************
// Set - Track Diag Status
*************************************************************************/
void pdbSetTrackDiagStatus(BYTE tno,BYTE Status);
/************************************************************************
// Set - Track Diag Status2
*************************************************************************/
void pdbSetTrackDiagStatus2(BYTE tno,BYTE Status);
/************************************************************************
// Set - Track Write Method
*************************************************************************/
void pdbSetTrackWriteMethod(BYTE tno,BYTE method);
/************************************************************************
// Set - Track NWA
*************************************************************************/
void pdbSetTrackNWA(BYTE tno,LONG addr);
/************************************************************************
// Get - EndTime Of Last CloseSession
*************************************************************************/
LONG pdbGetEndTimeOfLastCloseSession(void);
/************************************************************************
// Set - Disc Cont Ax
*************************************************************************/
void pdbSetDiscContAx(BYTE sno,BYTE ax,BYTE Cont);
/************************************************************************
// Set/Get - ISRC
*************************************************************************/
void pdbSetISRC(BOOL Valid,BYTE tno, BYTE *code,BYTE src);
void pdbGetISRC(BYTE tno,BYTE *code);
/************************************************************************
// Copy - Subcode<->Mem
*************************************************************************/
void pdbCopySubcodeToMem(SubQCodeS *subcode,BYTE *dest);
void pdbCopyMemToSubcode(BYTE *src,SubQCodeS *subcode);
/************************************************************************
// Set/Get/ - SubCodeMode5
*************************************************************************/
extern void pdbSetSubCodeMode5(BYTE point,SubQCodeS *subcode,BOOL found);
extern BOOL pdbGetSubCodeMode5(BYTE point,SubQCodeS *subcode);

//////////////////////////////////////////////////////////////////////////

extern volatile BOOL            Iram_fCDRW;
extern volatile BOOL            Iram_fSubQIntFlag;
extern volatile BOOL            Iram_fATIPIntFlag;
extern PlayerStatesT            fPlayerStatusValue;
extern AddrParmS                fSearchAddressLBA;
extern BYTE                     fCommandTrack;
//extern volatile LaserWriteModeT   Iram_fLaserWriteMode;
#include ".\common\iram.h"

///************************************************************************
//// Name        : pdbSetSubcodeOK/pdbGetSubcodeOK
//// Description : Set/Get subcode ok status.
//*************************************************************************/
#define pdbSetSubCodeOK(x)                 (Iram_DiscInfo.CD.QSubcodeValid = x)
#define pdbGetSubcodeOK()                  (Iram_DiscInfo.CD.QSubcodeValid)
#define pdbGetCDRW()                       (Iram_fCDRW)
#define pdbSetSubQIntStatus(x)             (Iram_fSubQIntFlag = x)
#define pdbGetSubQIntStatus()              (Iram_fSubQIntFlag)
/************************************************************************
// Name        : pdbSetSubcode/pdbGetSubCode
// Description : Set/Get 10 bytes subcode.
*************************************************************************/
#define pdbSetSubCode(x)                    (Iram_DiscInfo.CD.Q = *(x))
#define pdbGetSubCode(x)                    (*(x) = Iram_DiscInfo.CD.Q)
/************************************************************************
// Name        : pdbSetSubQcode/pdbGetSubQCode
// Description : Set/Get 10 bytes Mode 1 subcode.
*************************************************************************/
#define pdbSetSubQCode(x)                   (BufInfo.SubQ = *(x))
#define pdbGetSubQCode(x)                   (*(x) = BufInfo.SubQ)
#define pdbGetSubQTno()                    (BufInfo.SubQ.Tno)
/************************************************************************
// Name        : pdbSetServoState/pdbGetServoState
// Description : Set/Get BufInfo.ServoState.
*************************************************************************/
#define pdbSetServoState(x)                (BufInfo.ServoState = x)
#define pdbGetServoState()                 (BufInfo.ServoState)
/************************************************************************
// Name        : pdbSetBlocksToWrite/pdbGetBlocksToWrite
// Description : Set/Get BlocksToWrite as input value.
*************************************************************************/
/************************************************************************
// Name        : pdbServoSetfDiscKind/pdbServoGetDiscKind
// Description : Set/Get disc Information.
*************************************************************************/
#define pdbServoSetfDiscKind(x)             (BufInfo.StatusInfo.DiscKind.fDiscKind = x)
#define pdbServoGetfDiscKind()              (BufInfo.StatusInfo.DiscKind.fDiscKind)
#define pdbServoSetDiscKind(x)             (BufInfo.StatusInfo.DiscKind = x)
#define pdbServoGetDiscKind()              (BufInfo.StatusInfo.DiscKind)
/************************************************************************
// Name        : pdbSetDiscKind/pdbGetDiscKind
// Description : Set/Get disc Information.
*************************************************************************/
#define pdbSetDiscKind(x)             (LoadedDiscKind.fDiscKind = x)
#define pdbGetDiscKind()              (LoadedDiscKind.fDiscKind)
/************************************************************************
// Name        : pdbSetPlayerStatus/pdbGetPlayerStatus
// Description : The define is used to update the current Player status.
*************************************************************************/
#define pdbSetPlayerStatus(x)              (fPlayerStatusValue = x)
#define pdbGetPlayerStatus()               (fPlayerStatusValue)
/************************************************************************
// Name        : pdbSetDiscStatus
// Description : This define is used to update the current disc's disc type.
*************************************************************************/
#define pdbSetDiscStatus(x)                (Info.DiscStatus = x)
/************************************************************************
// Name        : pdbSetDiscIDStatus/pdbGetDiscIDStatus
// Description : pdbSetDiscIDStatus, This define is used to set the disc
//               ID status.
//               pdbGetDiscIDStatus, This define is used to get the disc
//               ID status.
*************************************************************************/
#define pdbSetDiscIDStatus(x)              (Info.DiscIDStatus = x)
#define pdbGetDiscIDStatus()               (Info.DiscIDStatus)
/************************************************************************
// Set/Get - Disc Mode Type
*************************************************************************/
#define pdbSetDiscModeType(i,x)              (Info.Media.CD.DiscModeType[i] = x)
#define pdbGetDiscModeType(i)               (Info.Media.CD.DiscModeType[i])
#define pdbSetHybridStatus(x)              (Info.Media.CD.HybridStatus = x)
/************************************************************************
// Set/Get - Session Counter
*************************************************************************/
#define pdbSetSessionCounter(x)             (Info.Media.CD.SessionCounter = x)
#define pdbGetSessionCounter()              (Info.Media.CD.SessionCounter)
/************************************************************************
// Set/Get - First Track In Last Session
*************************************************************************/
#define pdbSetFirstTrackInLastSession(x)    (Info.Media.CD.FirstTrackInLastSession = x)
#define pdbGetFirstTrackInLastSession()     (Info.Media.CD.FirstTrackInLastSession)
/************************************************************************
// Name        : pdbSetServoReadyStatus/pdbGetServoReadyStatus
// Description : This define returns the status-value of the Servo Subsystem
//               ready status.
//               The status is set to TRUE after finishing the start-up command.
//               When this status is TRUE all the disc data is valid
//               (whith the exeption of the track data mode).
*************************************************************************/
#define pdbSetServoReadyStatus(x)           (Info.ServoReadyStatus = x)
#define pdbGetServoReadyStatus()            (Info.ServoReadyStatus)
/************************************************************************
// Name        : pdbSetATIPLeadOut/pdbGetATIPLeadOut
// Description : These set defines will store the leadin, leadout and
//               record power information found at the atip into the general
//               database.
//               This informtion will read during the starting up of the disc
//               (reading disc info).
//               The get defines for retrieving this information for other
//               purposes are also available.
*************************************************************************/
#define pdbSetATIPLeadOut(x)                (Info.Media.CD.ATIPLeadOutStartTime = x)
#define pdbGetATIPLeadOut()                 (Info.Media.CD.ATIPLeadOutStartTime)
/************************************************************************
// Set/Get Error Group DiscInfo
*************************************************************************/
#define pdbSetErrorGroupDiscInfo(x)         (fErrorGroupDiscInfo |= x)
#define pdbGetErrorGroupDiscInfo()          (fErrorGroupDiscInfo)
/************************************************************************
// Name        : pdbSetErrorGroupRead/pdbGetErrorGroupRead
// Description : Set  ErrorGroupRead |= pError
//               Get  ErrorGroupRead
*************************************************************************/
#define pdbSetErrorGroupRead(x)             (fErrorGroupRead |= x)
#define pdbGetErrorGroupRead()              (fErrorGroupRead)
/************************************************************************
// Name        : pdbSetNextSessionAvailable / pdbGetNextSessionAvailable
// Description : This routine will sets/gets the next session available flag.
*************************************************************************/
#define pdbSetNextSessionAvailable(x)       (fNextSessionAvailable = x)
#define pdbGetNextSessionAvailable()        (fNextSessionAvailable)
/************************************************************************
// Set/Get - Speed
*************************************************************************/
#define pdbSetSpeed(x)                      (Iram_fRequestedSpeed = x)
#define pdbGetSpeed()                       (Iram_fRequestedSpeed)
/************************************************************************
// ServoSet/Get - Disc Diameter
*************************************************************************/
#define pdbServoSetDiscDiameter(x)          (BufInfo.StatusInfo.DiscDiameter = x)
#define pdbServoGetDiscDiameter()           (BufInfo.StatusInfo.DiscDiameter)
/************************************************************************
// ServoSet/Get - Disc Type
*************************************************************************/
#define pdbServoSetDiscType(x)              (BufInfo.StatusInfo.DiscType = x)
#define pdbServoGetDiscType()               (BufInfo.StatusInfo.DiscType)
/************************************************************************
// ServoSet/Get - Servo Error
*************************************************************************/
#define pdbServoSetServoError(x)            (BufInfo.ErrorInfo.ServoError = x)
#define pdbServoGetServoError()             (BufInfo.ErrorInfo.ServoError)
/************************************************************************
// ServoSet/Get - PlayerError
*************************************************************************/
#define pdbServoSetPlayerError(x)           (BufInfo.ErrorInfo.PlayerError = x)
#define pdbServoGetPlayerError()            (BufInfo.ErrorInfo.PlayerError)
/************************************************************************
// ServoSetGet- Disc Error
*************************************************************************/
#define pdbServoSetDiscError(x)             (BufInfo.ErrorInfo.DiscError = x)
#define pdbServoGetDiscError()              (BufInfo.ErrorInfo.DiscError)
/************************************************************************
// Name        : pdbGetABSTime
// Description : Get current subcode ABS time.
*************************************************************************/
#define pdbGetABSTime(x)                    (*(x) = *((TimeS *)&(BufInfo.SubQ.PMinute)))
/************************************************************************
// Name        : pdbSetEFMOnDisc/pdbGetEFMOnDisc
// Description : TRUE, if the subcode are valid.
*************************************************************************/
#define pdbSetEFMOnDisc(x)                  (Iram_fEFMValid = x)
#define pdbGetEFMOnDisc()                   (Iram_fEFMValid)
/************************************************************************
// Name        : pdbServoGetSelftestError
// Description : Get Selftest Error.
*************************************************************************/
#define pdbServoGetSelftestError()          (0)
/************************************************************************
// Name        : pdbSetErrorGroupTray/pdbGetErrorGroupTray
// Description : This functions set/get the high level error status information.
*************************************************************************/
#define pdbSetErrorGroupTray(x)             (fErrorGroupTray |= x)
#define pdbGetErrorGroupTray()              (fErrorGroupTray)
/************************************************************************
// Name        : pdbSetOutputFormat/pdbGetOutputFormat
// Description : The data form of lead-in is set/get by send Cue Sheet Command
*************************************************************************/
#define pdbSetOutputFormat(x)               (fOutPutFormat = x)
#define pdbGetOutputFormat()                (fOutPutFormat)
/************************************************************************
// Get - Track Cont Addr
*************************************************************************/
#define pdbGetTrackContAddr(x)              (Info.Media.CD.TOCData[x-1].ContAddr)
/************************************************************************
// Get - Track Data Form
*************************************************************************/
#define pdbGetTrackDataForm(x)              (Info.Media.CD.TOCData[x-1].DataForm)
/************************************************************************
// Get - Track Packet Size
*************************************************************************/
#define pdbGetTrackPacketSize(x)            (Info.Media.CD.TOCData[x-1].FixedPacketSize)
/************************************************************************
// Get - Track State
*************************************************************************/
#define pdbGetTrackState(x)                 (Info.Media.CD.TOCData[x-1].TrackState)
/************************************************************************
// Get - Track Status
*************************************************************************/
#define pdbGetTrackStatus(x)                (Info.Media.CD.TOCData[x-1].TrackStatus)
/************************************************************************
// Get - Track Diag Status
*************************************************************************/
#define pdbGetTrackDiagStatus(x)            (Info.Media.CD.TOCData[x-1].TrackDiagStatus0)
/************************************************************************
// Get - Track Diag Status2
*************************************************************************/
#define pdbGetTrackDiagStatus2(x)           (Info.Media.CD.TOCData[x-1].TrackDiagStatus1)
/************************************************************************
// Get - Track Write Method
*************************************************************************/
#define pdbGetTrackWriteMethod(x)           (Info.Media.CD.TOCData[x-1].WriteMethod)
/************************************************************************
// Set/Get - Final Session Number
*************************************************************************/
#define pdbSetFinalSessNumber(x)            (Iram_FinalSessionInfo.CD.SessionNumber = x)
#define pdbGetFinalSessNumber()             (Iram_FinalSessionInfo.CD.SessionNumber)
/************************************************************************
// Set/Get - Final Session Leadin Time
*************************************************************************/
#define pdbSetFinalSessLeadinTime(x)        (Iram_FinalSessionInfo.CD.LeadinTime = x)
#define pdbGetFinalSessLeadinTime()         (Iram_FinalSessionInfo.CD.LeadinTime)
/************************************************************************
// Set/Get - Final Session Leadout Time
*************************************************************************/
#define pdbSetFinalSessLeadoutTime(x)       (Iram_FinalSessionInfo.CD.LeadoutTime = x)
#define pdbGetFinalSessLeadoutTime()        (Iram_FinalSessionInfo.CD.LeadoutTime)
/************************************************************************
// Set/Get - Final Session First Track Number
*************************************************************************/
#define pdbSetFinalSessFirstTrackNumber(x)  (Iram_FinalSessionInfo.CD.FirstTrackNumber = x)
#define pdbGetFinalSessFirstTrackNumber()   (Iram_FinalSessionInfo.CD.FirstTrackNumber)
/************************************************************************
// Set/Get - Final Session Last Track Number
*************************************************************************/
#define pdbSetFinalSessLastTrackNumber(x)   (Iram_FinalSessionInfo.CD.LastTrackNumber = x)
#define pdbGetFinalSessLastTrackNumber()    (Iram_FinalSessionInfo.CD.LastTrackNumber)
/************************************************************************
// Set/Get - Final Session First Track Number
*************************************************************************/
#define pdbSetFinalSessFirstTrackTime(x)    (Iram_FinalSessionInfo.CD.FirstTrackTime = x)
#define pdbGetFinalSessFirstTrackTime()     (Iram_FinalSessionInfo.CD.FirstTrackTime)
/************************************************************************
// Set/Get - Final Session Format
*************************************************************************/
#define pdbSetFinalSessFormat(x)            (Iram_FinalSessionInfo.CD.Format= x)
#define pdbGetFinalSessFormat()             (Iram_FinalSessionInfo.CD.Format)
/************************************************************************
// Set/Get - Final Session Close Info
*************************************************************************/
#define pdbSetFinalSessCloseInfo(x)         (Iram_FinalSessionInfo.CD.CloseInfo= x)
#define pdbGetFinalSessCloseInfo()          (Iram_FinalSessionInfo.CD.CloseInfo)
/************************************************************************
// Set/Get - Next Session Start Time
*************************************************************************/
#define pdbSetNextSessStartTime(x)              (Iram_FinalSessionInfo.CD.NextSessionStartTime = x)
#define pdbGetNextSessStartTime()               (Iram_FinalSessionInfo.CD.NextSessionStartTime)
/************************************************************************
// Set/Get - Track NWA
*************************************************************************/
#define pdbGetTrackNWA(x)                       (Info.Media.CD.TOCData[x-1].NextWritableAddress)
/************************************************************************
// Set/Get - Read Info Method
*************************************************************************/
#define pdbSetReadInfoMethod(x)                 (ReadInfoMethod = x)
#define pdbGetReadInfoMethod()                  (ReadInfoMethod)
/************************************************************************
// Get - Disc Cont Ax
*************************************************************************/
#define pdbGetDiscContAx(x)                     (Info.Media.CD.DiscContAx[x-1])
/************************************************************************
// Get - Blank Disc Info
*************************************************************************/
#define pdbGetBlankDiscInfo()                   ((pdbGetFinalSessCloseInfo() == eEmptySession) && (pdbGetFinalSessFirstTrackNumber() == 1))

#define pdbSetSearchAddress(x)             (fSearchAddressLBA = x)
#define pdbGetSearchAddress(x)             (*x = fSearchAddressLBA)
#define pdbSetCommandTrack(x)              (fCommandTrack = x)
#define pdbGetCommandTrack()               (fCommandTrack)
#define pdbSetHighestRecordedTrkNumber(x)  (Info.Media.CD.HighestRecordedTrackNumber = x)
#define pdbGetHighestRecordedTrkNumber()   (Info.Media.CD.HighestRecordedTrackNumber)
#define pdbSetLowestRecordedTrackNumber(x) (Info.Media.CD.LowestRecordedTrackNumber = x)
#define pdbGetLowestRecordedTrackNumber()  (Info.Media.CD.LowestRecordedTrackNumber)
#define pdbSetLeadOutStartTime(x)          (Info.Media.CD.LeadOutStartTime = x)

#define DISC_IDENTIFIER         0x80
/**************************************************************************/
/*                 Variable externs              */
/**************************************************************************/
extern tFinalSessionInfoS  Iram_FinalSessionInfo;
/**************************************************************************/
/*                          Extern                                        */
/**************************************************************************/
extern void  pdbSetDVDMaxPSN(BYTE, ULONG);
extern ULONG pdbGetDVDMaxPSN(BYTE);
extern BOOL  pdbGetDVDBlankDiscInfo(void);
extern BYTE  pdbDiscLoaded(void);
extern void  pdbInitDiscSelect(void);
extern void  pdbClearDiscSelect(void);

/*-------------------------------------------------------------------------
   Disc Identification Macro

   mask     DISC_IDENTIFIER     = Bit 7 of eDiscKind. 1 = DVD disc, 0 = CD disc
            LAYER_INDENTIFIER   = Bit 6 of eDiscKind. 1 = dual layer, 0 = single layer
   enum     DiscClassE
   macro    pdbDiscLoaded() returns type DiscClassE
--------------------------------------------------------------------------*/
#define DUAL_LAYER_IDENTIFIER   0x40

typedef enum
{
    NON_CD_DVD_DISC,
    CD_DISC,
    DVD_DISC,
    HD_DISC,
    BD_DISC
} DiscClassE;

// #define pdbDiscLoaded()      ((BufInfo.StatusInfo.DiscKind.Bit.bDVDs) ? DVD_DISC : CD_DISC)


//***************************************************************************
//                       DVD/BD FUNCTIONS/MACROS/PROTOTYPES
//                               FOR PLAYER DATABASE
//***************************************************************************
#if (BD_ENABLE == 1)
#define pdbSetIDOk(x)                   (pdbServoGetDiscKind().Bit.DiscStd == eDiscDVD ? (Iram_DiscInfo.DVD.IDValid = x) : (Iram_DiscInfo.BD.IDValid = x))
#define pdbGetIDOk()                    (pdbServoGetDiscKind().Bit.DiscStd == eDiscDVD ? Iram_DiscInfo.DVD.IDValid : Iram_DiscInfo.BD.IDValid)
#define pdbSetWblAddress(x)             (pdbServoGetDiscKind().Bit.DiscStd == eDiscDVD ? (Iram_DiscInfo.DVD.ECCBLKAddress = x) : (Iram_DiscInfo.BD.ClusterAddress = x))
#define pdbGetWblAddress()              (pdbServoGetDiscKind().Bit.DiscStd == eDiscDVD ? Iram_DiscInfo.DVD.ECCBLKAddress : Iram_DiscInfo.BD.ClusterAddress)
#else
#define pdbSetIDOk(x)                   (Iram_DiscInfo.DVD.IDValid = x)
#define pdbGetIDOk()                    (Iram_DiscInfo.DVD.IDValid)
#define pdbSetWblAddress(x)             (Iram_DiscInfo.DVD.ECCBLKAddress = x)
#define pdbGetWblAddress()              (Iram_DiscInfo.DVD.ECCBLKAddress)
#endif

#define pdbSetIDIntStatus(x)            (Iram_fIDIntflag = x)
#define pdbGetIDIntStatus()             (Iram_fIDIntflag)

#define pdbSetDVDRMAEmptyStatus(x)      (Info.Media.DVD.RMAEmptyStatus = x)
#define pdbGetDVDRMAEmptyStatus()       (Info.Media.DVD.RMAEmptyStatus)

//------------------------------------------------------------------------------
// Macros to set and get physical format information
//------------------------------------------------------------------------------

// Disc Category and Version
#define pdbGetDVDType()     (Info.Media.DVD.PFInfoP->DiscTypeVersion & DVDHD_TYPE_MASK)
#define pdbGetDVDVersion()  (Info.Media.DVD.PFInfoP->DiscTypeVersion & DVDHD_VERSION_MASK)

#define pdbGetDVDMinusRPartVersion() ((Info.Media.DVD.Wobble.LPPData.PartVerExtCode & 0xF0) >> 4)

#define pdbSetDVDType(x)    (Info.Media.DVD.PFInfoP->DiscTypeVersion =  (x | (Info.Media.DVD.PFInfoP->DiscTypeVersion & DVDHD_VERSION_MASK)) )

#define pdbSetDVDVersion(x) (Info.Media.DVD.PFInfoP->DiscTypeVersion =  ((Info.Media.DVD.PFInfoP->DiscTypeVersion & DVDHD_TYPE_MASK) | x))

// Disc Size and Transfer Rate
#define pdbGetDVDDiscSize()         (Info.Media.DVD.PFInfoP->DiscSize & DVDHD_DISC_SIZE_MASK)
#define pdbGetDVDMaxTransferRate()  (Info.Media.DVD.PFInfoP->DiscSize & DVDHD_XFER_RATE_MASK)
#define pdbSetDVDDiscSize(x)        (Info.Media.DVD.PFInfoP->DiscSize = x)

// Disc Structure (layer information)
#define pdbGetDVDLayers()       ((Info.Media.DVD.PFInfoP->DiscStructure & DVDHD_LAYER_MASK) >> 5)
#define pdbGetDVDTrackPath()    (Info.Media.DVD.PFInfoP->DiscStructure & DVDHD_TRACK_PATH_MASK)
#define pdbGetDVDLayerType()    (Info.Media.DVD.PFInfoP->DiscStructure & DVDHD_LAYER_TYPE_MASK)
#define pdbSetDVDDiscStructure(x) (Info.Media.DVD.PFInfoP->DiscStructure = x)

// Recording Density
#define pdbGetDVDLinearDensity()    (Info.Media.DVD.PFInfoP->RecordingDensity & DVDHD_LINEAR_DENSITY_MASK)
#define pdbGetDVDTrackDensity()     (Info.Media.DVD.PFInfoP->RecordingDensity & DVDHD_TRACK_DENSITY_MASK)
#define pdbSetDVDRecordingDensity(x) (Info.Media.DVD.PFInfoP->RecordingDensity = x)

// Flag Bits
#define pdbDVDDiscBCA()   (Info.Media.DVD.PFInfoP->FlagBits & DVDHD_BCA_FLAG ? TRUE : FALSE)
#define pdbDVDDiscNBCA()  (Info.Media.DVD.PFInfoP->FlagBits & DVDHD_BCA_FLAG ? TRUE : FALSE)
#define pdbDVDDiscVCPS()  (Info.Media.DVD.PFInfoP->FlagBits & DVD_VCPS_FLAG ? TRUE : FALSE)
#define pdbDVDSetFlagBits(x) (Info.Media.DVD.PFInfoP->FlagBits = x)

//------------------------------------------------------------------------------
// Macros to access DVD logical information
//------------------------------------------------------------------------------
#define pdbGetDVDMaxLBA()       (Info.Media.DVD.MaxLBA)
#define pdbSetDVDMaxLBA(x)      (Info.Media.DVD.MaxLBA = x)
#define pdbGetDVDEndLBALayer0() (Info.Media.DVD.EndLBALayer0)
#define pdbSetDVDEndLBALayer0(x) (Info.Media.DVD.EndLBALayer0 = x)

//------------------------------------------------------------------------------
// Macros to access DVD Copyright Management information
//------------------------------------------------------------------------------
#define pdbGetDVDCPR_MAI()       (Info.Media.DVD.CPR_MAI)
#define pdbSetDVDCPR_MAI(x)      (Info.Media.DVD.CPR_MAI = x)

//------------------------------------------------------------------------------
// Macros to access DVD CSS information
//------------------------------------------------------------------------------
#define pdbDVDDiscCSS()             Info.Media.DVD.fCSS_PROTECTED
#define pdbSetDVDDiscCSS(x)         (Info.Media.DVD.fCSS_PROTECTED = x)
#define pdbGetDVDRegionControl()    (Info.Media.DVD.RegionControl)

//------------------------------------------------------------------------------
// Macros to access Last Session/Track information
//------------------------------------------------------------------------------
#define pdbSetDVDFinalSessCloseInfo(x)   (Iram_FinalSessionInfo.DVD.CloseInfo = x)
#define pdbGetDVDFinalSessCloseInfo()    (Iram_FinalSessionInfo.DVD.CloseInfo)
#define pdbSetDVDFinalSessionNumber(x)   (Iram_FinalSessionInfo.DVD.SessionNumber = x)
#define pdbGetDVDFinalSessionNumber()    (Iram_FinalSessionInfo.DVD.SessionNumber)
#define pdbSetDVDFinalSessFirstTrackNumber(Tno)     (Iram_FinalSessionInfo.DVD.FirstTrackNumber = Tno)
#define pdbGetDVDFinalSessFirstTrackNumber()        (Iram_FinalSessionInfo.DVD.FirstTrackNumber)
#define pdbSetDVDFinalSessLastTrackNumber(Tno)      (Iram_FinalSessionInfo.DVD.LastTrackNumber = Tno)
#define pdbGetDVDFinalSessLastTrackNumber()         (Iram_FinalSessionInfo.DVD.LastTrackNumber)
#define pdbSetDVDFinalSessFirstTrackPSN(ADDR)       (Iram_FinalSessionInfo.DVD.FirstTrackPSN = ADDR)
#define pdbGetDVDFinalSessFirstTrackPSN()           (Iram_FinalSessionInfo.DVD.FirstTrackPSN)
#define pdbSetDVDFinalSessFormat(format)            (Iram_FinalSessionInfo.DVD.Format = format)
#define pdbGetDVDFinalSessFormat()                  (Iram_FinalSessionInfo.DVD.Format)

//------------------------------------------------------------------------------
// Macros to access Session information
//------------------------------------------------------------------------------
#define pdbDVDIncrementSessionCount()  (++Info.Media.DVD.SessionCounter)
#define pdbSetDVDSessionCount(x)       (Info.Media.DVD.SessionCounter = x)
#define pdbGetDVDSessionCount()        (Info.Media.DVD.SessionCounter)
//------------------------------------------------------------------------------
// Macros to access Track information
//------------------------------------------------------------------------------
#define pdbDVDIncreaseHighestRecordedTrackNum()     (++Info.Media.DVD.LogicalTracks)
#define pdbSetDVDHighestRecordedTrackNumber(x)      Info.Media.DVD.LogicalTracks = x
#define pdbGetDVDHighestRecordedTrackNumber()       (Info.Media.DVD.LogicalTracks)
#define pdbSetDVDLowestRecordedTrackNumber(x)       (Info.Media.DVD.LowestRecordedTrackNumber = x)
#define pdbGetDVDLowestRecordedTrackNumber()        (Info.Media.DVD.LowestRecordedTrackNumber)

// Miscellaneous macros


//------------------------------------------------------------------------------
// Macros to access DVD+RW FDCB information
//------------------------------------------------------------------------------
/* FDCB Format status */

//------------------------------------------------------------------------------
// Macros to access DVD+RW WDCB information
//------------------------------------------------------------------------------
/* WDCB Exist status */


//***************************************************************************
//                       PENDING FUNCTION PROTOTYPES
//***************************************************************************

//***************************************************************************
//                        DVD DISC DATA INFORMATION
//***************************************************************************
void    pdbInitDvdDataBase(void);
#if (ENABLE_HDDVD == 1)
void    pdbInitHdDvdDataBase(void);
#endif
//***************************************************************************
//                       DVD STATIC DISC INFORMATION
//***************************************************************************
ULONG   pdbGetDVDLeadOutStartPSN(void);

//***************************************************************************
//                       DVD CURRENT DISC INFORMATION
//***************************************************************************
BYTE    pdbGetDVDClosedSessions(void);

//***************************************************************************
//                       DVD FINAL SESSION INFORMATION
//***************************************************************************
void    pdbSetDVDFinalSessLeadinPSN(ULONG ADDR);
LONG    pdbGetDVDFinalSessLeadinLBA(void);
void    pdbSetDVDFinalSessLeadoutPSN(ULONG ADDR);
LONG    pdbGetDVDFinalSessLeadoutLBA(void);
void    pdbSetDVDNextSessStartPSN(ULONG ADDR);

//***************************************************************************
//  DVD PHYSICAL FORMAT INFORMATION
//***************************************************************************
void    pdbSetDVDPhysicalFormatInfo(BYTE *);



//***************************************************************************
//                          DVD TRACK DATABASE INFORMATION
//***************************************************************************
void    pdbSetDVDTrackRecordedPSN(BYTE trackNum, ULONG info);

BYTE    pdbGetDVDRW(void);
ULONG   pdbGetDVDStartPSN(BYTE layer);
ULONG   pdbGetDVDBorderZoneSize(BYTE BorderNum, ULONG info);


//***************************************************************************
//                       MISCELLANEOUS INFORMATIONAL FUNCTIONS
//***************************************************************************
#if GET_LAST_TRACK_INFO_BASED_ON_RAI
extern ULONG pdbGetLastRecordedRegion(void);
#endif

void    pdbSetDVDLogicalFormatInfo(BYTE *);
void    pdbSetDVDLogicalData(void);

/***** Common Macros *****/
/* Get - Final Session Leadin/Leadout PSN */
#define pdbGetDVDFinalSessLeadinPSN()               (Iram_FinalSessionInfo.DVD.LeadinPSN)
#define pdbGetDVDFinalSessLeadoutPSN()              (Iram_FinalSessionInfo.DVD.LeadoutPSN)

/* Get - Next Session Start PSN */
#define pdbGetDVDNextSessStartPSN()                 (Iram_FinalSessionInfo.DVD.NextSessionStartPSN)

/* Set/Get - DVD Track Status */
#define pdbSetDVDTrackStatus(Tno, info)             (Info.Media.DVD.Track[Tno-1].TrackStatus = info)
#define pdbGetDVDTrackStatus(Tno)                   (Info.Media.DVD.Track[Tno-1].TrackStatus)

/* Set/Get - DVD Track State */
#define pdbSetDVDTrackState(Tno, info)              (Info.Media.DVD.Track[Tno-1].TrackState = info)
#define pdbGetDVDTrackState(Tno)                    (Info.Media.DVD.Track[Tno-1].TrackState)

/* Set/Get - DVD Track Diag Status 0 */
#define pdbSetDVDTrackDiagStatus(Tno, info)         (Info.Media.DVD.Track[Tno-1].TrackDiagStatus0 = info)
#define pdbGetDVDTrackDiagStatus(Tno)               (Info.Media.DVD.Track[Tno-1].TrackDiagStatus0)

/* Set/Get - DVD Track Diag Status 1 */
#define pdbSetDVDTrackDiagStatus2(Tno, info)        (Info.Media.DVD.Track[Tno-1].TrackDiagStatus1 = info)
#define pdbGetDVDTrackDiagStatus2(Tno)              (Info.Media.DVD.Track[Tno-1].TrackDiagStatus1)

/* Set/Get - DVD Track Session Number */
#define pdbSetDVDTrackSessionNumber(Tno, info)      (Info.Media.DVD.Track[Tno-1].TrackSessionNumber = info)
#define pdbGetDVDTrackSessionNumber(Tno)            (Info.Media.DVD.Track[Tno-1].TrackSessionNumber)

/* Set/Get - DVD Track Start PSN */
#define pdbSetDVDTrackStartPSN(Tno, info)           (Info.Media.DVD.Track[Tno-1].StartPSN.PSN = info)
#define pdbGetDVDTrackStartPSN(Tno)                 (Info.Media.DVD.Track[Tno-1].StartPSN.PSN)

/* Set/Get - DVD Track Start PSN Layer */
#define pdbSetDVDTrackStartPSNLayer(Tno, info)      (Info.Media.DVD.Track[Tno-1].StartPSN.Layer = info)
#define pdbGetDVDTrackStartPSNLayer(Tno)            (Info.Media.DVD.Track[Tno-1].StartPSN.Layer)

/* Set/Get - DVD Track End PSN */
#define pdbSetDVDTrackEndPSN(Tno, info)             (Info.Media.DVD.Track[Tno-1].EndPSN.PSN = info)
#define pdbGetDVDTrackEndPSN(Tno)                   (Info.Media.DVD.Track[Tno-1].EndPSN.PSN)

/* Set/Get - DVD Track End PSN Layer */
#define pdbSetDVDTrackEndPSNLayer(Tno, info)        (Info.Media.DVD.Track[Tno-1].EndPSN.Layer = info)
#define pdbGetDVDTrackEndPSNLayer(Tno)              (Info.Media.DVD.Track[Tno-1].EndPSN.Layer)

/* Set/Get - DVD Layer Jump Address */
#define pdbSetDVDRZoneLJA(Tno, info)                (Info.Media.DVD.Track[Tno-1].LayerJumpAddress= info)
#define pdbGetDVDRZoneLJA(Tno)                      (Info.Media.DVD.Track[Tno-1].LayerJumpAddress)

/* Get - DVD Track Recorded PSN */
#define pdbGetDVDTrackRecordedPSN(Tno)              (Info.Media.DVD.Track[Tno-1].RecordedPSN.PSN)

/* Set/Get - DVD Track Next Writable Address (LBA) */
#define pdbSetDVDTrackNextWritableAddress(Tno, info)    (Info.Media.DVD.Track[Tno-1].NextWritableLBA = info)
#define pdbGetDVDTrackNextWritableAddress(Tno)      (Info.Media.DVD.Track[Tno-1].NextWritableLBA)

/* Set/Get - DVD-R Next Board-in PSN */
#define pdbSetDVDNextBorderInPSN(info)              (Info.Media.DVD.NextBorderInPSN= info)
#define pdbGetDVDNextBorderInPSN()                  (Info.Media.DVD.NextBorderInPSN)

/* Set/Get - DVD Track End PSN */
#define pdbSetDVDBorderOutPSN(BorderNum, info)      (Info.Media.DVD.BorderOutPSN[BorderNum].PSN = info)
#define pdbGetDVDBorderOutPSN(BorderNum)            (Info.Media.DVD.BorderOutPSN[BorderNum].PSN)

/* Set/Get - DVD Track End PSN */
#define pdbSetDVDBorderInPSN(BorderNum, info)       (Info.Media.DVD.BorderInPSN[BorderNum].PSN = info)
#define pdbGetDVDBorderInPSN(BorderNum)             (Info.Media.DVD.BorderInPSN[BorderNum].PSN)

/* Set/Get - DVD Track End PSN */
#define pdbSetDVDBorderNumber(BorderNum)            (Info.Media.DVD.BorderCount = BorderNum)
#define pdbGetDVDBorderNumber()                     (Info.Media.DVD.BorderCount)

/* Set/Get - DVD RMA RMD Format */
#define pdbSetDVDRMARMDFormat(Format)               (Info.Media.DVD.RMAData.RMD_format = Format)
#define pdbGetDVDRMARMDFormat()                     (Info.Media.DVD.RMAData.RMD_format)

/* Set/Get - DVD RMA Disc Status */
#define pdbSetDVDRMADiscStatus(DiscStatus)          (Info.Media.DVD.RMAData.Disc_status = DiscStatus)
#define pdbGetDVDRMADiscStatus()                    (Info.Media.DVD.RMAData.Disc_status)

/* Set/Get - DVD First Open RZone */
#define pdbSetDVDFirstOpenRZone(RZoneNum)           (Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword = RZoneNum)
#define pdbGetDVDFirstOpenRZone()                   (Info.Media.DVD.RMAData.RMD.Format1.FirstOpenRZone.Uword)

/* Set/Get - DVD Second Open RZone */
#define pdbSetDVDSecondOpenRZone(RZoneNum)          (Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword = RZoneNum)
#define pdbGetDVDSecondOpenRZone()                  (Info.Media.DVD.RMAData.RMD.Format1.SecondOpenRZone.Uword)

/* Set/Get - DVD Third Open RZone */
#define pdbSetDVDThirdOpenRZone(RZoneNum)           (Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword = RZoneNum)
#define pdbGetDVDThirdOpenRZone()                   (Info.Media.DVD.RMAData.RMD.Format1.ThirdOpenRZone.Uword)

//Read DVD-RW CPRM video Format3 RMD for FWtester2  Item7 DVD CPRM Feature Check
/* Get - DVD Minus Erase Operation */
#define pdbGetDVDMinusFormat3LastRZoneNumber()      (Info.Media.DVD.RMAData.RMD.Format3.LastRZoneNumber.Uword)
/* Set/Get - DVD Invisible RZone */
#define pdbSetDVDInvisibleRZone(RZoneNum)           (Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword = RZoneNum)
#define pdbGetDVDInvisibleRZone()                   (Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword)
#define pdbUpdDVDInvisibleRZoneInc()                (Info.Media.DVD.RMAData.RMD.Format1.InvisibleRZone.Uword++)
ULONG   pdbGetDVDLeadInStartPSN(void);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**************************************************************************/
/*                 Externs for accessing dvdDisc information              */
/**************************************************************************/

#endif /* _PLRDB_H_ */
