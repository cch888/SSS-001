/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:  oem_cfg_tbl.h
*
* DESCRIPTION: "Get Configuration" command table
*
*
* $Revision: 29 $
* $Date: 09/06/09 4:52p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __OEM_GFG_TBL_H__
#define __OEM_GFG_TBL_H__

/*********************************************************************
 LOCAL DEFINITION
*********************************************************************/
/* Cuurent Bit Status */
#define CUR_BIT_OFF                 0x00
#define CUR_BIT_ON                  0x01
#define CUR_BIT_FIX                 0x03

/* Current Medium Disc Type */
#define CUR_CD_NO                   0x0000
#define CUR_CD_ROM                  0x0001    /* CD-ROM */
#define CUR_CD_R                    0x0002    /* CD-R */
#define CUR_CD_RW                   0x0004    /* CD-RW */
#define CUR_CD_HRW                  0x0008    /* HS-RW (High Speed) */
#define CUR_CD_USRW                 0x0010    /* US-RW (Ultra Speed) */
#define CUR_DVD_ROM                 0x0100
#define CUR_DVD_RECORDABLE          0x0200
#define CUR_DVD_REWRITABLE          0x0400
#define CUR_DVD_RAM                 0x0800

#define RW_GROUP                    (CUR_CD_RW  | CUR_CD_HRW | CUR_CD_USRW | CUR_DVD_REWRITABLE)
#define NOT_STAMP_GROUP             (CUR_CD_R   | CUR_CD_RW  | CUR_CD_HRW | CUR_CD_USRW | CUR_DVD_RECORDABLE | CUR_DVD_REWRITABLE)
#define ALL_GROUP                   (CUR_CD_ROM | CUR_CD_R   | CUR_CD_RW  | CUR_CD_HRW | CUR_CD_USRW | CUR_DVD_ROM | CUR_DVD_RECORDABLE | CUR_DVD_REWRITABLE)

/* Current Medium Data Type */
#define CUR_DISC_DATA               0x00    /* Data Disc */
#define CUR_DISC_AUDIO              0x80    /* CD-DA Disc */

/*********************************************************************
 CONFIGURATION DATA

    For more detail of each feature refer to the Mt. Fuji
    or Mt Rainier specification for Get Configuration Command.
*********************************************************************/
/*---------------------------------------------------------------
    Features List

    This array contains all the implemented feature numbers
---------------------------------------------------------------*/
#define NUMBER_OF_FEATURES          (sizeof( Features ) / sizeof( USHORT ))

static const USHORT Features[] =
{
                                     FL_PROFILE_LIST                        //0x0000
                                    ,FL_CORE                                //0x0001
                                    ,FL_MORPHING                            //0x0002
                                    ,FL_REMOVABLE_MEDIA                     //0x0003
                                    ,FL_RANDOM_READABLE                     //0x0010
                                    ,FL_MULTI_READ                          //0x001D
                                    ,FL_CD_READ                             //0x001E
                                    ,FL_DVD_READ                            //0x001F
                                    ,FL_DEFECT_MGT                          //0x0024
                                    //,FL_ENHANCED_DEFECT_REPORTING           //0x0029
                                    ,FL_DVD_PLUS_RW                         //0x002A
                                    ,FL_DVD_PLUS_R                          //0x002B
                                    ,FL_DVD_PLUS_R_DUAL                     //0x003B
#if (BD_ENABLE)
                                    ,FL_BD_READ                            //0x0040
#endif /*BD_ENABLE*/
#if (ENABLE_HDDVD)
                                    ,FL_HD_READ                            //0x0050
#endif /* ENABLE_HDDVD */
                                    ,FL_POWER_MGT                           //0x0100
//                                    ,FL_SMART                               //0x0101
                                    //,FL_EMBEDDED_CHANGER                    //0x0102
                                    ,FL_CD_AUDIO_ANALOG                     //0x0103
                                    ,FL_MICROCODE_UPGRADE                   //0x0104
                                    ,FL_TIME_OUT                            //0x0105
                                    ,FL_DVD_CSS                             //0x0106
                                    ,FL_REALTIME_STREAMING                  //0x0107
                                    ,FL_SERIAL_NUMBER                       //0x0108
                                    //,FL_MEDIA_SERIAL_NUM                    //0x0109

#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
                                    ,FL_DVD_CPRM                            //0x010B
#endif
                                    ,FL_FIRMWARE_INFO                       //0x010C
#if (ENABLE_AACS)
                                    ,FL_AACS                                //0x010D
#endif
#if (VCPS_TEST == 1)
                                    ,FL_DVD_VCPS                            //0x0110
#endif
#if (EN_LS == TRUE)
                                    ,FL_LS                             //0xFF33
#endif // End of (EN_LS == 1)
};

//---------------------------------------------------------------
//  Module defines
//---------------------------------------------------------------
#define PERSISTENT  0x02
#define CURRENT     0x01

//---------------------------------------------------------------
//  Feature 0h : Profile List
//---------------------------------------------------------------
static const BYTE ProfileList[] =
{
                                    0x00, 0x00,                     // Feature code "Profile List" 0x00
                            (PERSISTENT | CURRENT),                 // Feature status
#if((BD_READ == 1)||(HD_READ == 1))&&(DVD_RAM_READ == 1)
                                    (4*15),                         // Additional length. Support for 15 different types of discs.
#else
    #if (SUPPORT_REMOVABLE_DISK_PROFILE == 1) 
                                    (4*4),  // Additional length. Support for 4 different types of discs.
    #else                                                                      
                                    (4*3),  // Additional length. Support for 3 different types of discs.
    #endif                                       
#endif
#if(BD_READ == 1)    
    // For WHQL, exchange BDROM profile and CDROM profile                               
                                    0x00, PROFILE_BDROM,            // Profile number [CD-ROM PROFILE]
                                    0x00,                           // Profile status bit 0   - CurrentP  (1 = medium is present)
                                    0x00,                           // Reserved
#endif                                    
                                    0x00, PROFILE_DVDROM,           // Profile number [Read only DVD-ROM PROFILE]
                                    0x00,                           // Profile status bit 0   - CurrentP  (1 = medium is present)
                                    0x00,                           // Reserved

                                    0x00, PROFILE_CDROM,            // Profile number [Read only DVD-ROM PROFILE]
                                    0x00,                           // Profile status bit 0   - CurrentP  (1 = medium is present)
                                    0x00,                           // Reserved
#if (SUPPORT_REMOVABLE_DISK_PROFILE == 1)    
                                    0x00, 0x02,                     //Profile number [Removable disk PROFILE]
                                    0x00,
                                    0x00,
#endif

#if(HD_READ == 1)

                                    0x00, PROFILE_HDROM,            // Profile number [Read only DVD-ROM PROFILE]
                                    0x00,                           // Profile status bit 0   - CurrentP  (1 = medium is present)
                                    0x00,                           // Reserved
#endif
};

//---------------------------------------------------------------
//  Feature 1h : Core
//---------------------------------------------------------------
static const BYTE Core[] =
{
                            //For WHQL Optical test, the version of
                            //Core Feature shall be at least 2.
                                    0x00, 0x01,                     // Feature Code "Core"
                            (0x08 | PERSISTENT | CURRENT),          // Version=2, Feature Status
                                    (4*2),                          // Additional Length

                                    0x00, 0x00, 0x00, 0x07,         // Physical Interface Serial ATAPI
                                    0x01,                           // INQ2=0, DBEvent=1
                                    0x00, 0x00, 0x00
                            
};

//---------------------------------------------------------------
//  Feature 2h : Morphing
//---------------------------------------------------------------
static const BYTE Morphing[] =
{
                                    0x00, 0x02,                     // Feature Code "Morphing"
                            //For WHQL Optical test, the version of
                            //Morphing Feature shall be at least 1.
                            (0x04 | PERSISTENT | CURRENT),          // Feature Status
                                    (4*1),                          // Additional Length
                                    0x02,                               //bit1-OCEvent shall be set to one when version is not 0, bit 0   - Async (support only polling)
                                    0x00, 0x00, 0x00                // Reserved
};

//---------------------------------------------------------------
//  Feature 3h : Removable Medium
//  ----------------------------------------------------------
static const BYTE RemovableMedium[] =
{
                                    //For WHQL Optical test, the version of
                                    //Removable Medium Feature shall be at least 1
                                    //and the bit, Load, shall be set to 1.
                                    0x00, 0x03,                     // Feature Removable Media
                                    (0x04 | PERSISTENT | CURRENT),  // Feature Status
                                    (4*1),                          // Additional Length
                                #if(SET_SLOT_IN == 0)//[S07][S513]INTEL diference mechanism type
                                    0x39,
                                                                    //     bit 5-7 - Mechanism type (001 = Tray Type)
                                                                    //     bit 4   - Load
                                                                    //     bit 3   - Eject     ( 1 = device can eject media )
                                                                    //     bit 2   - PrvntJmpr ( This setting same ModePage 2Ah Byte 6 bit2 )
                                                                    //     bit 1   - Reserved
                                                                    //     bit 0   - Lock      ( 1 = Prevent/Allow cmd can lock media )
								#else
									0x19,                           //     bit 5-7 - Mechanism type (000 = Slot-in Type)
								#endif
                                    0x00, 0x00, 0x00                // Reserved
                                    
};

//  ----------------------------------------------------------
//  Feature 10h : Random Readable
//  ----------------------------------------------------------
static  BYTE RandomReadable[]=
{
                                    0x00, 0x10,
                                    0x00,                           // Feature Status
                                    (4*2),                          // Additional Length
                                    0x00, 0x00, 0x08, 0x00,         // Logical Block Size
                                    0x00, 0x01,                     // Blocking
                                    0x01,                           // Page Not Present
                                    0x00                            // Reserved
};

//---------------------------------------------------------------
//  Feature 1Dh : MultiRead
//  ----------------------------------------------------------
static const BYTE MultiRead[] =
{
                                    0x00, 0x1D,                     // Feature Multi Read
                                    (PERSISTENT),                
                                    (4*0)                           // Additional Length
};

//---------------------------------------------------------------
//  Feature 1Eh : CD Read
//  ----------------------------------------------------------
static const BYTE cdRead[] =
{
                                    0x00, 0x1E,                     // Feature CD Read
                                    //For WHQL Optical test, the version of
                                    //CD Read Feature shall be at least 2
                                    0x08,                           // Version = 2
                                    (4*1),                          // Additional Length
                                    0x03,    // Bit 0 CD Text, bit 1 C2  /* support read Features C2 errors */
                                    0x00, 0x00, 0x00                // Reserved
};

//---------------------------------------------------------------
//  Feature 1Fh : DVD Read
//  ----------------------------------------------------------
static const BYTE dvdRead[] =
{
                                    0x00, 0x1F,                     // Feature DVD Read
                                    0x04,                           // Feature Status
                                    0x04,                           // Additional Length
                                    0x00, 0x00, 0x01, 0x00          // Physical Interface Serial ATAPI
};

//---------------------------------------------------------------
//  Feature 24h : Defect Management
//  ----------------------------------------------------------
static const BYTE DefectMgt[] =
{
                                    0x00, 0x24,                     // Feature Defect Management
                                    0x04,                           // Version 1
                                    (4*1),                          // Additional Length
                                    0x00,                           // DVD Support for READ DVD STRUCTURE
                                    0x00, 0x00, 0x00                // Reserved
};

//---------------------------------------------------------------
//  Feature 2Ah : DVD+RW :  The ability to recognize, read and optionally write DVD+RW media
//  ----------------------------------------------------------
static const BYTE DvdPlusRW[] =
{
                                    0x00, 0x2A,                     // Feature DVD+RW
                                    //For WHQL Optical test, the version of
                                    //DVD+RW Feature shall be at least 1
                                    0x04,                           // Version = 1
                                    (4*1),                          // Additional Length
                                    0x00,                           // Write bit = 0
                                    0x00,                           // Quick Start = 1, Close Only = 0
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
};

//---------------------------------------------------------------
//  Feature 2Bh : DVD+R :  The ability to read DVD+R recorded media formats.
//  ----------------------------------------------------------
static const BYTE DvdPlusR[] =
{
                                    0x00, 0x2B,                     // Feature DVD+R
                                    0x00,                           // Feature Status Version = 0
                                    (4*1),                          // Additional Length
                                    0x00,                           // Write bit = 0
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
};

//---------------------------------------------------------------
//  Feature 3Bh : DVD+R DL:  The ability to read DVD+R DL recorded media formats.
//  ----------------------------------------------------------
static const BYTE DvdPlusRDL[] =
{
                                    0x00, 0x3B,                     // Feature DVD+R
                                    0x00,                           // Feature Status Version = 0
                                    (4*1),                          // Additional Length
                                    0x00,                           // Write bit = 0
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
};

#if (BD_READ ==1)
//---------------------------------------------------------------
//  Feature 40h : BD READ
//---------------------------------------------------------------
static const BYTE BdRead[] =
{
                                    0x00, 0x40,                     // Feature code
                                    0x00,                           // Reserved 7-6| Version 5-2| Persistent | Current
                                    0x1C,                           // Additional Length
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved

                                                                    // Version read support
                                    0x00,                           // BD-RE class 0 version 8-15
                                    0x06,                           // BD-RE class 0 version 0-7
                                    0x00,                           // BD-RE class 1 version 8-15
                                    0x00,                           // BD-RE class 1 version 0-7
                                    0x00,                           // BD-RE class 2 version 8-15
                                    0x00,                           // BD-RE class 2 version 0-7
                                    0x00,                           // BD-RE class 3 version 8-15
                                    0x00,                           // BD-RE class 3 version 0-7

                                    0x00,                           // BD-R class 0 version 8-15
                                    0x02,                           // BD-R class 0 version 0-7
                                    0x00,                           // BD-R class 1 version 8-15
                                    0x00,                           // BD-R class 1 version 0-7
                                    0x00,                           // BD-R class 2 version 8-15
                                    0x00,                           // BD-R class 2 version 0-7
                                    0x00,                           // BD-R class 3 version 8-15
                                    0x00,                           // BD-R class 3 version 0-7

                                    0x00,                           // BD-ROM class 0 version 8-15
                                    0x02,                           // BD-ROM class 0 version 0-7
                                    0x00,                           // BD-ROM class 1 version 8-15
                                    0x00,                           // BD-ROM class 1 version 0-7
                                    0x00,                           // BD-ROM class 2 version 8-15
                                    0x00,                           // BD-ROM class 2 version 0-7
                                    0x00,                           // BD-ROM class 3 version 8-15
                                    0x00                            // BD-ROM class 3 version 0-7
};
#endif

#if(ENABLE_HDDVD == 1)
//---------------------------------------------------------------
//  Feature 50h : HD READ
//---------------------------------------------------------------
static const BYTE HdRead[] =
{
                                    0x00, 0x50,                     // Feature HD DVD Read
                                    0x04,                           // Feature Status Version = 1
                                    0x08,                           // Additional Length
                                    0x00,                           // HD DVD-R SL
                                    0x00,                           // HD DVD-R DL
                                    0x00,                           // HD DVD-RAM
                                    0x00,                           // Reserved
                                    0x00,                           // HD DVD-RW SL
                                    0x00,                           // HD DVD-RW DL
                                    0x00                            // Reserved
};
#endif

//---------------------------------------------------------------
//  Feature 100h : Power Management
//  ----------------------------------------------------------
static const BYTE PowerManagement[] =
{
                                    0x01, 0x00,                     // Feature Power Management
                            (PERSISTENT | CURRENT),                 // Feature Status
                                    (4*0),                          // Additional Length
};

//---------------------------------------------------------------
//  Feature 103h : CD Audio analog play
//  ----------------------------------------------------------
static const BYTE cdAudioPlay[] =
{
                                    0x01, 0x03,                     // Feature CD Audio Analog Play
                                    0x00,                           // Feature Status
                                    (4*1),                          // Additional Length
                                    0x07,
                                                                    //    bit 0       Separate Volume         0 = all audio channels same level
                                                                    //    bit 1       Separate Channel Mute    0 = all channels muted simultaneously
                                                                    //    bit 2       Scanbit                 0 = SCAN command is not supported
                                                                    //    bit 3 - 7   Reserved
                                    0x00,
                                    0x01, 0x00                      // Number of volume levels
};

//---------------------------------------------------------------
//  Feature 104h : Microcode upgrade
//  ----------------------------------------------------------
static const BYTE MicroCodeUpgrade[] =
{
                                    0x01, 0x04,                     // Microcode upgrade
                            //For WHQL Optical test, the version of
                            //Microcode upgrade Feature shall be at least 1 and
                            //Additional Length shall be 4
                            (0x04 | PERSISTENT | CURRENT),          // Feature Status
                                    (4*1),                          // Additional Length
                                    0x00, 0x00, 0x00, 0x00
                            
};

//---------------------------------------------------------------
//  Feature 105h : Time-Out
//  ----------------------------------------------------------
static const BYTE TimeOut[] =
{
                                    0x01, 0x05,
                            //For WHQL Optical test, the version of
                            //Time-Out Feature shall be at least 1 and
                            //Additional Length shall be 4
                            (0x04 | PERSISTENT | CURRENT),          // Feature Status
                                    (4*1),                          // Additional Length
                                    0x00,
                                    0x00,
                                    0x00, 0x00                      // Unit Length
                            
};

//---------------------------------------------------------------
//  Feature 106h : DVD CSS
//  ----------------------------------------------------------
static const BYTE DVD_CSS[] =
{
                                    0x01, 0x06,                     // Feature DVD CSS
                                    0x00,                           // Feature Status
                                    (4*1),                          // Additional Length
                                    0x00, 0x00, 0x00,               // Reserved
                                    0x01                            // CSS Version
};

//---------------------------------------------------------------
//  Feature 107h : Real-Time Streaming
//  ----------------------------------------------------------
static const BYTE RealTimeStreaming[] =
{
                                    0x01, 0x07,                     // Feature Real-Time Streaming
                                    //For WHQL Optical test, the version of
                                    //Real-Time Streaming Feature shall be at least 4
                                    0x10,                           // Version = 4
                                    (4*1),                          // Additional Length
                                    0x09,   //20090414_g.28==start==change reason,
                                            //For WIN7 WHQL Optical test->Real-Time Streaming TestCase,
                                            //SW shall be set to 1, and Read-Only Drive doesn't need to
                                            //set MP2A as 1
                                                                    //     bit 0       SW
                                                                    //     bit 1       WSPD
                                                                    //     bit 2       MP2A
                                                                    //     bit 3       SCS
                                                                    //     bit 4       RBCB
                                                                    //     bit 5 - 7   Reserved
                                    0x00, 0x00, 0x00
};

//---------------------------------------------------------------
//  Feature 108h : Logical Unit Serial Number
//  ----------------------------------------------------------
static const BYTE DefaultSerialNumber[] =
{
                                    0x01, 0x08,                     // Feature Logical Unit Serial Number
                            (PERSISTENT | CURRENT),                 // Feature Status
                                    (4*5),                          // Additional Length
                                    FIRMW_ID_SN
};

//---------------------------------------------------------------
//  Feature 10Ah : Disc Control Blocks
//---------------------------------------------------------------
static const BYTE DiscCtrlBlocks[] =
{
                                    0x01, 0x0A,                     // Feature code "Disc Control Blocks"
                                    0x00,                           // Feature Status
                                    (4*4),                          // Additional Length

                                    0x46,                           // Supported DCB entry 0
                                    0x44,                           // FDCB
                                    0x43,                           //
                                    0x00,                           //

                                    0x53,                           // Supported DCB entry 1
                                    0x44,                           // SDCB
                                    0x43,                           //
                                    0x00,                           //

                                    0x54,                           // Supported DCB entry 2
                                    0x4F,                           // TOC
                                    0x43,                           //
                                    0x00,                           //

                                    0x57,                           // Supported DCB entry 3
                                    0x44,                           // WDC
                                    0x43,                           //
                                    0x00                            //
};

#if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
//---------------------------------------------------------------
//  Feature 10Bh : DVD CPRM
//---------------------------------------------------------------
static const BYTE DvdCprm[] =
{
                                    0x01, 0x0B,                     // Feature code "DVD CPRM"
                                    0x00,                           // Feature Status
                                    (4*1),                          // Additional Length
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x01                            // CPRM Version
};
#endif


//---------------------------------------------------------------
//  Feature 10Ch : Firmware Information
//---------------------------------------------------------------
static const BYTE FirmwareInfo[] =
{
                                    0x01, 0x0C,                     // Feature code "Firmware Information"
                            (PERSISTENT | CURRENT),                 // Feature Status
                                    0x10,                           // Additional Length
                                    0x00,                           // Centry
                                    0x00,                           // Centry
                                    0x00,                           // Year
                                    0x00,                           // Year
                                    0x00,                           // Month
                                    0x00,                           // Month
                                    0x00,                           // Day
                                    0x00,                           // Day
                                    0x00,                           // Hour
                                    0x00,                           // Hour
                                    0x00,                           // Minute
                                    0x00,                           // Minute
                                    0x00,                           // Second
                                    0x00,                           // Second
                                    0x00,                           // Reserved
                                    0x00                            // Reserved
};

#if (ENABLE_AACS)
//---------------------------------------------------------------
//  Feature 010Dh : AACS
//---------------------------------------------------------------
static const BYTE Aacs[] =
{
                                    0x01, 0x0D,                     // Feature code "AACS"
                                    0x00,                           // Reserved 7-6| Version 5-2| Persistent | Current
                                    0x04,                           // Additional Length
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x03,                           // Number of AGIDs
                                    0x01                            // AACS Version
};
#endif

#if (VCPS_TEST ==1)
//---------------------------------------------------------------
//  Feature 110h : DVD VCPS
//---------------------------------------------------------------
static const BYTE DvdVcps[] =
{
                                    0x01, 0x10,                     // Feature code
                                    0x00,                           // Reserved 7-6| Version 5-2| Persistent | Current
                                    0x04,                           // Additional Length
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00,                           // Reserved
                                    0x00                            // Reserved
};
#endif

#endif /* __OEM_GFG_TBL_H__ */
