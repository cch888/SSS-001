/****************************************************************************
*                (c) Copyright 2004 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   oem_spd_tbl.h
*
* DESCRIPTION
* Header file for oem_spd_tbl.c, contains speed defines which can be updated
* for each individual drive.
*
* $Revision: 142 $
* $Date: 11/03/17 12:02p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __OEM_SPD_TBL_H__
#define __OEM_SPD_TBL_H__

/***************************************************
*                 Include Files                    *
****************************************************/

/*** Row and Column defines for DVD/CD Write/Read Speed tables ***/

#define CD_SPD_COLMNS  11                       /* Number of Columns in the SelCDSpeed_Tbl */
                                                /* WARNING: This number MUST match the number
                                                   of columns in the CDSelSpeed_tbl_R & CDSelSpeed_tbl_W tables
                                                   and the number of entries in the SelCDReqSpeed_tbl table */
#define DVD_SPD_COLMNS 11                       /* Number of Columns in the SelDVDSpeed_Tbl */
                                                /* 10 Speed columns + 1 Init speed offset column */
                                                /* WARNING: This number MUST match the number
                                                   of columns in the DVDSelSpeed_tbl_R & DVDSelSpeed_tbl_W tables
                                                   and the number of entries in the SelDVDReqSpeed_tbl table */

#if(BD2p2XCLV == 1)
#define BD_SPD_COLMNS  6                        // Number of Columns in the SelBDSpeed_Tbl
#else                                           // WARNING: This number MUST match the number
#define BD_SPD_COLMNS  5                        // of columns in the BDSelSpeed_tbl_R & BDSelSpeed_tbl_W tables
#endif                                          // and the number of entries in the SelBDReqSpeed_tbl table



#define HD_SPD_COLMNS  3                        /* Number of Columns in the SelHDSpeed_Tbl */
                                                /* WARNING: This number MUST match the number
                                                   of columns in the HDSelSpeed_tbl_R & HDSelSpeed_tbl_W tables
                                                   and the number of entries in the SelHDReqSpeed_tbl table */

#if (BD_READ)
#define BD_READ_MEDIA       6                   /* Number of Rows (media types) in the BDSelSpeed_tbl_R Table */
#endif  //(BD_READ)

#if (HD_READ)
#define HD_READ_MEDIA       7                   /* Number of Rows (media types) in the HDSelSpeed_tbl_R Table */
#endif  //(HD_READ)

#define DVD_READ_MEDIA      6                   /* Number of Rows (media types) in the DVDSelSpeed_tbl_R Table */

#define DVD_WRITE_MEDIA     6                   /* Number of Rows (media types) in the DVDSelSpeed_tbl_W Table */

#define CD_READ_MEDIA       5                   /* Number of Rows (media types) in the CDSelSpeed_tbl_R Table */

enum {          // BDSelSpeed_tbl_R Row Offset - This offset must match the BD Disc Type in the table
    eBDROMSL_R,         // Row 0 - BD ROM Single Layer
    eBDROMDL_R,         // Row 1 - BD ROM Dual Layer
    eBDRSL_R,           // Row 2 - BD-R Single Layer
    eBDRESL_R,          // Row 3 - BD-RE Single Layer
    eBDRDL_R,           // Row 4 - BD-R Dual Layer
    eBDREDL_R           // Row 5 - BD-RE Dual Layer
};

enum {          // HDSelSpeed_tbl_R Row Offset - This offset must match the BD Disc Type in the table
    eHDROMSL_R,         // Row 0 - HD ROM Single Layer
    eHDROMDL_R,         // Row 1 - HD ROM Dual Layer
    eHDRSL_R,           // Row 2 - HD-R Single Layer
    eHDRWSL_R,          // Row 3 - HD-RW Single Layer
    eHDRDL_R,           // Row 4 - HD-R Dual Layer
    eHDRWDL_R,          // Row 5 - HD-RW Dual Layer
    eHDRAM_R            // Row 6 - HD-RAM
};

enum {          // DVDSelSpeed_tbl_R Row Offset - This offset must match the DVD Disc Type in the table
    eDVDROMSL_R,        // Row 0 - DVD ROM Single Layer
    eDVDROMDL_R,        // Row 1 - DVD ROM Dual Layer
    eDVDRSL_R,          // Row 2 - DVD+/-R Single Layer
    eDVDRWSL_R,         // Row 3 - DVD+/-RW Single Layer
    eDVDRDL_R,          // Row 4 - DVD+/-R Dual Layer
    eDVDRAM_R           // Row 5 - DVD-RAM
};

enum {          // DVDSelSpeed_tbl_W Row Offset - This offset must match the DVD Disc Type in the table
    eDVDMRSL_W,         // Row 0 - DVD-R Single Layer
    eDVDPRSL_W,         // Row 1 - DVD+R Single Layer
    eDVDMRWSL_W,        // Row 2 - DVD-RW Single Layer
    eDVDPRWSL_W,        // Row 3 - DVD+RW Single Layer
    eDVDRDL_W,          // Row 4 - DVD+/-R Dual Layer
    eDVDRAM_W           // Row 5 - DVD-RAM
};

enum {          // CDSelSpeed_tbl_R Row Offset - This offset must match the CD Disc Type in the table
    eCDROM_R,           // Row 0 - CD-ROM and CD-R Closed Session
    eCDRW_R,            // Row 1 - CD-RW Closed Session
    eCDRWPKT_R,         // Row 2 - CD-RW Packet Written disc
    eCDRWOpen_R,        // Row 3 - CD-RW Open Session disc
    eCDDAExt_R          // Row 4 - Audio Extraction
};

enum {          // CDSelSpeed_tbl_W Row Offset - This offset must match the CD Disc Type in the table
    eCDR_W,             // Row 0 - CD-R
    eCDRW_W,            // Row 1 - CD-RW
    eCDRWUS_W,          // Row 2 - CD-US
};

enum {          // AudioPlay_Speed_tbl Row Offset - This offset must match the CD Disc Type in the table
    eCDDA_R,             // Row 0 - CD-R
    eCDDA_R_OPEN_R,      // Row 1 - CD-RW
    eCDDA_RW_OPEN_R,     // Row 2 - CD-US
};

#if ((DETECT_ECC_DISC == 1)||(DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)|| (BD_UNBALANCE_DISC_CHECK == 1))
enum {  /* ECC/Unblance Table's ROW */
    eECC_UNB_MAX_SPD_CD = 0,    /* ROW 0: CD */
    eECC_UNB_MAX_SPD_DVD,       /* ROW 1: DVD */
    eECC_UNB_MAX_SPD_BD,        /* ROW 2: BD */
    eECC_UNB_MAX_SPD_RAM        /* ROW 3: DVDRAM */
};
#endif

/***************************************************
*              External Declarations               *
****************************************************/
#if (DETECT_ECC_DISC == 1)
extern const BYTE ECCDiscSpd_tbl[4][5];                                   /* Eccentric Discs Speed Table */
#endif
#if ((DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)|| (BD_UNBALANCE_DISC_CHECK == 1))
extern const BYTE UnBalDiscSpd_tbl[4][4];                                   /* Unbalanced Discs Speed Table */
#endif
extern const BYTE   CDSelSpeed_tbl_R   [CD_READ_MEDIA][CD_SPD_COLMNS];      /* CD Read Speed table */

#if (BD_ENABLE)
extern const BYTE   BDSelSpeed_tbl_R  [BD_READ_MEDIA][BD_SPD_COLMNS];    /* Read DVD table */
#endif  //(BD_ENABLE)

extern const BYTE   DVDSelSpeed_tbl_R  [DVD_READ_MEDIA][DVD_SPD_COLMNS];    /* Read DVD table */

#if (ENABLE_HDDVD)
extern const BYTE   HDSelSpeed_tbl_R  [HD_READ_MEDIA][HD_SPD_COLMNS];    /* Read HD table */
#endif

extern const BYTE   DVDRead_Init_Speed_tbl[DVD_READ_MEDIA];                 /* DVD Read Initial Speed table */
extern const BYTE   CDRead_Init_Speed_tbl[CD_READ_MEDIA];                   /* CD Read Initial Speed table */


/***************************************************
*                  Speed Defines                   *
****************************************************/

/***************************************************
 Maximum Speed Defines for different disc types
 ---------------------------------------------------
 Update each of the defines to determine the maximum
 operational speeds for each specific media type
 ---------------------------------------------------
 ***************************************************/

/******************************************************/
/***      These values are maximum disc speeds      ***/
/******************************************************/
/**** Read CD Discs Speed Defines ****/
    #if(PCB == 710)||(PCB == 711)||(PCB == 817)
       #if (CEProduct == 0)   //IT

#define MAX_READ_SPEED          e48XCAV     /* Max Read speed supported by drive */
#define MIN_READ_SPEED          e4XCLV
#define eMAXCD                  MAX_READ_SPEED
#define MAX_CDI_READ_SPEED      e10XCAV     /* Max Read speed supported for CDI discs */
#define eCDSpinUpSpeed          e10XCAV      /* Sync Servo CD Start Up Speed */
#define eCDIdleSpeed            e10XCAV     /* Idle speed for CD */
#define MAX_MCD_SPEED           MAX_READ_SPEED//e32XCAV     /* Marginal CD's Max read Speed */
            #if(DVD_710Run815 == 1)
#define eDVDIdleSpeed           e2XDCLV     /* Idle speed for DVD */
            #else
#define eDVDIdleSpeed           e4XDCAV     /* Idle speed for DVD */
            #endif
#define eDVDRAMIdleSpeed        e2XDCLV     /* Idle speed for DVD-RAM */

/**** Audio CD Discs Speed Defines ****/
#define MAX_READ_CDDA_SPEED     e10XCAV     /* Max CD-DA Read (Extraction) speed */
#define PLAY_AUDIO_MSF_SPD      e10XCAV
#define MIN_CD_PACKET_READ_SPD  e32XCAV

#define MAX_CDRW_SPD            e8XCLV     /* Maximum Write Speed allowed on Normal CD-RW Media */
#define MAX_HSRW_SPD            e24XCAV     /* Maximum Write Speed allowed on High Speed HS-RW Media */
#define MIN_USRW_SPD            e12XCLV     /* Minimum Write Speed allowed on Ultra Speed US-RW Media */

/**** Special CD Discs Speed Defines ****/
//#define MAX_SCRATCH_SPEED       e24XCAV     /* Maximum CD Speed for a Scratch Disc */

/**** 8CM Disc Speed Defines ****/
#define MAX_FASHION_DISC_SPD    e10XCAV     /* 8CM Fashion Disc maximum speed */
#define MAX_8CM_CD_DISC_SPD     e10XCAV     /* 8CM Disc maximum speed */
            #if(DVD_710Run815 == 1)
#define MAX_8CM_DVD_DISC_SPD    e2XDCLV     /* 8CM Disc maximum speed */
            #else
#define MAX_8CM_DVD_DISC_SPD    e4XDCAV     /* 8CM Disc maximum speed */
            #endif

#define MAX_80_MIN_DISC_SPD     e10XCAV     /* 80 Minute Disc maximum speed */

/**** Video CD Disc Speed Defines ****/
#define eReadVCDPlaySpeed       e10XCAV     /* Set Read vcd play Speed limit */
#define eReadRVCDCopySpeed      e10XCAV     /* Set Read CDR vcd copy Speed limit */
#define eReadRWVCDCopySpeed     e10XCAV     /* Set Read CDRW vcd copy Speed limit */

/**** DVD Disc Speed Defines ****/
            #if(DVD_710Run815 == 1)
#define MAX_READ_DVD_SPEED      e4XDCLV    /* Max DVD Read (Extraction) speed */
            #else
#define MAX_READ_DVD_SPEED      e16XDCAV    /* Max DVD Read (Extraction) speed */
            #endif

#define MAX_READ_RAM_SPEED      e5XDCAV     /* Max DVD-RAM Read speed */

#define eDVDSpinUpSpeed         e2XDCLV     /* Sync Servo DVD Start Up Speed */

#if (Detect_AVCHD_DISC == 1)
#define MAX_READ_AVCHD_SPEED  e4XDCLV
#endif

/**** BD Disc Speed Defines ****/
#define MAX_READ_BD_SPEED       e6XDCAV     /* Max BD Read (Extraction) speed */  //TBD ??
#define eBDSpinUpSpeed          e4XDCAV     /* Sync Servo BD Start Up Speed */  //TBD ??

/**** HD Disc Speed Defines ****/
#define MAX_READ_HD_SPEED       e1XDCLV     /* Max HD Read (Extraction) speed */  //TBD ??
#define eHDSpinUpSpeed          e1XDCLV     /* Sync Servo HD Start Up Speed */  //TBD ??
#if (SEQ_SEEK_SPEED_DOWN == 1)
#define CD_SEEK_DOWN_SPEED      e32XCAV     /* Max CD sequence Seek/Access speed */
            #if(DVD_710Run815 == 1)
#define DVD_SEEK_DOWN_SPEED     e4XDCLV     /* Max DVD sequence Seek/Access speed */
            #else
#define DVD_SEEK_DOWN_SPEED     e8XDCAV     /* Max DVD sequence Seek/Access speed */
            #endif
#define BD_SEEK_DOWN_SPEED      e4XDCAV     /* Max BD sequence Seek/Access speed */
#endif

        #else  // CEProduct
	#if(Customer_A2 == 1 || Customer_Dune_HD == 1)//[V08] Bios
#define MAX_READ_SPEED          e10XCAV     /* Max Read speed supported by drive */
            #else
#define MAX_READ_SPEED          e16XCAV//[S0A] Shin e16XCAV     /* Max Read speed supported by drive */  //[S09] Shin debug
            #endif

#define MIN_READ_SPEED          e10XCAV//e4XCLV
#define eMAXCD                  MAX_READ_SPEED
#define MAX_CDI_READ_SPEED      e10XCAV     /* Max Read speed supported for CDI discs */
#define eCDSpinUpSpeed          e10XCAV      /* Sync Servo CD Start Up Speed */
#define eCDIdleSpeed            e10XCAV     /* Idle speed for CD */
#define MAX_MCD_SPEED           e16XCAV     /* Marginal CD's Max read Speed */
            #if(DVD_710Run815 == 1)
#define eDVDIdleSpeed           e2XDCLV     /* Idle speed for DVD */
            #else
#define eDVDIdleSpeed           e4XDCAV     /* Idle speed for DVD */
            #endif
#define eDVDRAMIdleSpeed        e2XDCLV     /* Idle speed for DVD-RAM */

/**** Audio CD Discs Speed Defines ****/
#define MAX_READ_CDDA_SPEED     e10XCAV     /* Max CD-DA Read (Extraction) speed */
#define PLAY_AUDIO_MSF_SPD      e10XCAV
#define MIN_CD_PACKET_READ_SPD  e32XCAV

#define MAX_CDRW_SPD            e8XCLV     /* Maximum Write Speed allowed on Normal CD-RW Media */
#define MAX_HSRW_SPD            e24XCAV     /* Maximum Write Speed allowed on High Speed HS-RW Media */
#define MIN_USRW_SPD            e12XCLV     /* Minimum Write Speed allowed on Ultra Speed US-RW Media */

/**** Special CD Discs Speed Defines ****/
//#define MAX_SCRATCH_SPEED       e24XCAV     /* Maximum CD Speed for a Scratch Disc */

/**** 8CM Disc Speed Defines ****/
#define MAX_FASHION_DISC_SPD    e10XCAV     /* 8CM Fashion Disc maximum speed */
#define MAX_8CM_CD_DISC_SPD     e10XCAV     /* 8CM Disc maximum speed */
            #if(DVD_710Run815 == 1)
#define MAX_8CM_DVD_DISC_SPD    e2XDCLV     /* 8CM Disc maximum speed */
            #else
#define MAX_8CM_DVD_DISC_SPD    e4XDCAV     /* 8CM Disc maximum speed */
            #endif

#define MAX_80_MIN_DISC_SPD     e10XCAV     /* 80 Minute Disc maximum speed */

/**** Video CD Disc Speed Defines ****/
#define eReadVCDPlaySpeed       e10XCAV     /* Set Read vcd play Speed limit */
#define eReadRVCDCopySpeed      e10XCAV     /* Set Read CDR vcd copy Speed limit */
#define eReadRWVCDCopySpeed     e10XCAV     /* Set Read CDRW vcd copy Speed limit */

/**** DVD Disc Speed Defines ****/
            #if(DVD_710Run815 == 1)
#define MAX_READ_DVD_SPEED      e4XDCLV    /* Max DVD Read (Extraction) speed */
            #else
	#if(Customer_A2 == 1)//[V08] Bios
#define MAX_READ_DVD_SPEED      e4XDCAV    /* Max DVD Read (Extraction) speed */
	#else            
#define MAX_READ_DVD_SPEED      e8XDCAV    /* Max DVD Read (Extraction) speed */
            #endif
            #endif

#define MAX_READ_RAM_SPEED      e5XDCAV     /* Max DVD-RAM Read speed */

#define eDVDSpinUpSpeed         e2XDCLV     /* Sync Servo DVD Start Up Speed */

#if (Detect_AVCHD_DISC == 1)
#define MAX_READ_AVCHD_SPEED  e6XDCAV
#endif

/**** BD Disc Speed Defines ****/
//[V08] Bios BD speed 3CLV = 2 CLV ,2CLV = 1.7CLV
#if((Customer_A2 == 1)||(Customer_ZHD== 1)||(Customer_China== 1))//[W0K]
#define MAX_READ_BD_SPEED       e3XDCLV     /* Max BD Read (Extraction) speed */  //TBD ??
#define eBDSpinUpSpeed          e3XDCLV     /* Sync Servo BD Start Up Speed */  //TBD ??
#elif(Customer_Dune_HD == 1)//[V08] Bios 2x CLV
#define MAX_READ_BD_SPEED       e2XDCLV     /* Max BD Read (Extraction) speed */  //TBD ??
#define eBDSpinUpSpeed          e2XDCLV     /* Sync Servo BD Start Up Speed */  //TBD ??
#else
#define MAX_READ_BD_SPEED       e4XDCAV     /* Max BD Read (Extraction) speed */  //TBD ??
#define eBDSpinUpSpeed          e2XDCLV     /* Sync Servo BD Start Up Speed */  //TBD ??
#endif
/**** HD Disc Speed Defines ****/
#define MAX_READ_HD_SPEED       e1XDCLV     /* Max HD Read (Extraction) speed */  //TBD ??
#define eHDSpinUpSpeed          e1XDCLV     /* Sync Servo HD Start Up Speed */  //TBD ??

#if (SEQ_SEEK_SPEED_DOWN == 1)
#define CD_SEEK_DOWN_SPEED      e16XCAV     /* Max CD sequence Seek/Access speed */
            #if(DVD_710Run815 == 1)
#define DVD_SEEK_DOWN_SPEED     e4XDCLV     /* Max DVD sequence Seek/Access speed */
            #else
#define DVD_SEEK_DOWN_SPEED     e8XDCAV     /* Max DVD sequence Seek/Access speed */
            #endif
#define BD_SEEK_DOWN_SPEED      e4XDCAV     /* Max BD sequence Seek/Access speed */
#endif
        #endif  //CEProduct

    #elif (PCB == 815)

#define MAX_READ_SPEED          e10XCAV     /* Max Read speed supported by drive */
#define MIN_READ_SPEED          e4XCLV
#define eMAXCD                  MAX_READ_SPEED
#define MAX_CDI_READ_SPEED      e10XCAV     /* Max Read speed supported for CDI discs */
#define eCDSpinUpSpeed          e4XCLV      /* Sync Servo CD Start Up Speed */
#define eCDIdleSpeed            e10XCAV     /* Idle speed for CD */
#define MAX_MCD_SPEED           e32XCAV     /* Marginal CD's Max read Speed */
            #if(DVD_710Run815 == 1)
#define eDVDIdleSpeed           e2XDCLV     /* Idle speed for DVD */
            #else
#define eDVDIdleSpeed           e4XDCAV     /* Idle speed for DVD */
            #endif
#define eDVDRAMIdleSpeed        e2XDCLV     /* Idle speed for DVD-RAM */

/**** Audio CD Discs Speed Defines ****/
#define MAX_READ_CDDA_SPEED     e10XCAV     /* Max CD-DA Read (Extraction) speed */
#define PLAY_AUDIO_MSF_SPD      e10XCAV
#define MIN_CD_PACKET_READ_SPD  e32XCAV

#define MAX_CDRW_SPD            e8XCLV     /* Maximum Write Speed allowed on Normal CD-RW Media */
#define MAX_HSRW_SPD            e24XCAV     /* Maximum Write Speed allowed on High Speed HS-RW Media */
#define MIN_USRW_SPD            e12XCLV     /* Minimum Write Speed allowed on Ultra Speed US-RW Media */

/**** Special CD Discs Speed Defines ****/
//#define MAX_SCRATCH_SPEED       e24XCAV     /* Maximum CD Speed for a Scratch Disc */

/**** 8CM Disc Speed Defines ****/
#define MAX_FASHION_DISC_SPD    e10XCAV     /* 8CM Fashion Disc maximum speed */
#define MAX_8CM_CD_DISC_SPD     e10XCAV     /* 8CM Disc maximum speed */
            #if(DVD_710Run815 == 1)
#define MAX_8CM_DVD_DISC_SPD    e2XDCLV     /* 8CM Disc maximum speed */
            #else
#define MAX_8CM_DVD_DISC_SPD    e4XDCAV     /* 8CM Disc maximum speed */
            #endif

#define MAX_80_MIN_DISC_SPD     e10XCAV     /* 80 Minute Disc maximum speed */

/**** Video CD Disc Speed Defines ****/
#define eReadVCDPlaySpeed       e10XCAV     /* Set Read vcd play Speed limit */
#define eReadRVCDCopySpeed      e10XCAV     /* Set Read CDR vcd copy Speed limit */
#define eReadRWVCDCopySpeed     e10XCAV     /* Set Read CDRW vcd copy Speed limit */

/**** DVD Disc Speed Defines ****/
            #if(DVD_710Run815 == 1)
#define MAX_READ_DVD_SPEED      e4XDCLV    /* Max DVD Read (Extraction) speed */
            #else
#define MAX_READ_DVD_SPEED      e8XDCAV    /* Max DVD Read (Extraction) speed */
            #endif

#define MAX_READ_RAM_SPEED      e5XDCAV     /* Max DVD-RAM Read speed */

#define eDVDSpinUpSpeed         e2XDCLV     /* Sync Servo DVD Start Up Speed */

#if (Detect_AVCHD_DISC == 1)
#define MAX_READ_AVCHD_SPEED  e4XDCLV
#endif

/**** BD Disc Speed Defines ****/
#define MAX_READ_BD_SPEED       e2XDCLV     /* Max BD Read (Extraction) speed */
#define eBDSpinUpSpeed          e2XDCLV     /* Sync Servo BD Start Up Speed */

/**** HD Disc Speed Defines ****/
#define MAX_READ_HD_SPEED       e1XDCLV     /* Max HD Read (Extraction) speed */
#define eHDSpinUpSpeed          e1XDCLV     /* Sync Servo HD Start Up Speed */

#if (SEQ_SEEK_SPEED_DOWN == 1)
#define CD_SEEK_DOWN_SPEED      e10XCAV     /* Max CD sequence Seek/Access speed */
            #if(DVD_710Run815 == 1)
#define DVD_SEEK_DOWN_SPEED     e4XDCLV     /* Max DVD sequence Seek/Access speed */
            #else
#define DVD_SEEK_DOWN_SPEED     e8XDCAV     /* Max DVD sequence Seek/Access speed */
            #endif
#define BD_SEEK_DOWN_SPEED      e2XDCLV     /* Max BD sequence Seek/Access speed */
#endif

    #else//PCB

#define MAX_READ_SPEED          e10XCAV     /* Max Read speed supported by drive */
#define MIN_READ_SPEED          e4XCLV
#define eMAXCD                  MAX_READ_SPEED
#define MAX_CDI_READ_SPEED      e10XCAV     /* Max Read speed supported for CDI discs */
#define eCDSpinUpSpeed          e10XCAV      /* Sync Servo CD Start Up Speed */
#define eCDIdleSpeed            e10XCAV     /* Idle speed for CD */
#define MAX_MCD_SPEED           e32XCAV     /* Marginal CD's Max read Speed */
#define eDVDIdleSpeed           e4XDCAV     /* Idle speed for DVD */
#define eDVDRAMIdleSpeed        e2XDCLV     /* Idle speed for DVD-RAM */

/**** Audio CD Discs Speed Defines ****/
#define MAX_READ_CDDA_SPEED     e10XCAV     /* Max CD-DA Read (Extraction) speed */
#define PLAY_AUDIO_MSF_SPD      e10XCAV

#define MIN_CD_PACKET_READ_SPD  e32XCAV

#define MAX_CDRW_SPD            e8XCLV     /* Maximum Write Speed allowed on Normal CD-RW Media */
#define MAX_HSRW_SPD            e24XCAV     /* Maximum Write Speed allowed on High Speed HS-RW Media */
#define MIN_USRW_SPD            e12XCLV     /* Minimum Write Speed allowed on Ultra Speed US-RW Media */

/**** Special CD Discs Speed Defines ****/
//#define MAX_SCRATCH_SPEED       e24XCAV     /* Maximum CD Speed for a Scratch Disc */

/**** 8CM Disc Speed Defines ****/
#define MAX_FASHION_DISC_SPD    e10XCAV     /* 8CM Fashion Disc maximum speed */
#define MAX_8CM_CD_DISC_SPD     e10XCAV     /* 8CM Disc maximum speed */
#define MAX_8CM_DVD_DISC_SPD    e4XDCAV     /* 8CM Disc maximum speed */

#define MAX_80_MIN_DISC_SPD     e10XCAV     /* 80 Minute Disc maximum speed */

/**** Video CD Disc Speed Defines ****/
#define eReadVCDPlaySpeed       e10XCAV     /* Set Read vcd play Speed limit */
#define eReadRVCDCopySpeed      e10XCAV     /* Set Read CDR vcd copy Speed limit */
#define eReadRWVCDCopySpeed     e10XCAV     /* Set Read CDRW vcd copy Speed limit */

/**** DVD Disc Speed Defines ****/
#if (DVD_692RUN4XCLV == 1)
#define MAX_READ_DVD_SPEED      e8XDCAV     /* Max DVD Read (Extraction) speed */
#else
#define MAX_READ_DVD_SPEED      e4XDCAV     /* Max DVD Read (Extraction) speed */
#endif

#define MAX_READ_RAM_SPEED      e12XDPCAV   /* Max DVD-RAM Read speed */

#define eDVDSpinUpSpeed         e2XDCLV     /* Sync Servo DVD Start Up Speed */

#if (Detect_AVCHD_DISC == 1)
#define MAX_READ_AVCHD_SPEED  e4XDCLV
#endif

/**** BD Disc Speed Defines ****/
#define MAX_READ_BD_SPEED       e2XDCLV     /* Max BD Read (Extraction) speed */  //TBD ??
#define eBDSpinUpSpeed          e2XDCLV     /* Sync Servo BD Start Up Speed */  //TBD ??

/**** HD Disc Speed Defines ****/
#define MAX_READ_HD_SPEED       e1XDCLV     /* Max HD Read (Extraction) speed */  //TBD ??
#define eHDSpinUpSpeed          e1XDCLV     /* Sync Servo HD Start Up Speed */  //TBD ??
#if (SEQ_SEEK_SPEED_DOWN == 1)
#define CD_SEEK_DOWN_SPEED      e32XCAV     /* Max CD sequence Seek/Access speed */
#define DVD_SEEK_DOWN_SPEED     e8XDCAV     /* Max DVD sequence Seek/Access speed */
#define BD_SEEK_DOWN_SPEED      e4XDCAV     /* Max BD sequence Seek/Access speed */
#endif
    #endif//PCB
/*********************************/
/*** RDVD Maximum Write Speeds ***/ //TBD? should come from table
/*********************************/

#define SPEED_RECOVERY_COUNT    2           /* This value determines the number of times the Read
                                            Recovery algorithm will allow the speed to increase
                                            after lowering the speed due to errors.  When the count
                                            is reached the drive will maintain the lower speed until
                                            the disc is ejected. */

#define FIX_READ_SPEED          0           /* if 1, the FIX_SPEED setting will use for all Read CAV speed */

#if (FIX_READ_SPEED == 1)
extern BYTE Host_Requested_speed;
#endif

#endif  /* #ifndef __OEM_SPD_TBL_H__*/
