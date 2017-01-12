/****************************************************************************
*                (c) Copyright 2004 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:  oem_spd_tbl.c
*
* DESCRIPTION:
*   This file contains all the customizable speed tables.
*   Changing these tables will change the operational speeds
*   of the drive.
*
*   $Revision: 44 $
*   $Date: 11/03/22 5:54p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

/***************************************************
*                  Include Files                   *
****************************************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\oem\oem_tbl.h"
#include ".\servo\svo.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\oem\oem_spd_tbl.h"

#if (FIX_READ_SPEED == 1)
BYTE Host_Requested_speed;
#endif

/***************************************************
*              Speed Control Tables                *
****************************************************/
#if (DETECT_ECC_DISC == 1)
/*--------------------------------------------------------------------------------
Maximum Speed defines for use with InitializeReadSpeed and Set_SpeedMDCont functions
These define the maximum speeds to be used with Eccentric discs
----------------------------------------------------------------------------------*/
const BYTE ECCDiscSpd_tbl[4][5]={
                            /*    #0       #1         #2       #3        #4   */
/* Eccentric Discs*/    //CD:   Normal   ECC70um  ECC140um   ECC210um  ECC280um
                        //DVD:  Normal   ECC50um  ECC100um   ECC150um
                        //BD:   Normal   ECC75um  ECC100um   ECC150um
                        //RAM:  Normal   ECC75um  ECC100um   ECC150um
    #if((MAX_READ_SPEED == e16XCAV)&&(MAX_READ_DVD_SPEED == e8XDCAV)&&(MAX_READ_BD_SPEED==e4XDCAV))//711 use
/* 0. Max CD Read */       {  e16XCAV,  e16XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e8XDCAV,  e8XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #elif((MAX_READ_SPEED == e10XCAV)&&(MAX_READ_DVD_SPEED == e4XDCAV)&&(MAX_READ_BD_SPEED==e2XDCLV))//692 use
/* 0. Max CD Read */       {  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #elif((MAX_READ_SPEED == e10XCAV)&&(MAX_READ_DVD_SPEED == e8XDCAV)&&(MAX_READ_BD_SPEED==e2XDCLV)&&(DVD_692RUN4XCLV == 1))
/* 0. Max CD Read */       {  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #elif((MAX_READ_SPEED == e10XCAV)&&(MAX_READ_DVD_SPEED == e4XDCLV)&&(MAX_READ_BD_SPEED==e2XDCLV))//815 use
/* 0. Max CD Read */       {  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},
/* 2. Max BD Read */       {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #else//710 use
/* 0. Max CD Read */       {  MAX_READ_SPEED,      MAX_READ_SPEED,      e32XCAV,  e24XCAV,  e16XCAV},
/* 1. Max DVD Read */      {  MAX_READ_DVD_SPEED,  MAX_READ_DVD_SPEED,  e8XDCAV,  e8XDCAV,  e8XDCAV},
/* 2. Max BD Read */       {  MAX_READ_BD_SPEED,   MAX_READ_BD_SPEED,   e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 3. Max RAM Read */      {  MAX_READ_RAM_SPEED,  MAX_READ_RAM_SPEED,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #endif
};
#endif//#if (DETECT_ECC_DISC == 1)

#if ((DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)|| (BD_UNBALANCE_DISC_CHECK == 1))
/*--------------------------------------------------------------------------------
Maximum Speed defines for use with InitializeReadSpeed and Set_SpeedMDCont functions
These define the maximum speeds to be used with unbalanced discs
----------------------------------------------------------------------------------*/
const BYTE UnBalDiscSpd_tbl[4][4]={
                            /*   #0         #1        #2        #3    */
            //Unbalance CD:    <0.5g      0.5g       0.75g     1.0g
            //Unbalance DVD:<=0.5g/0.5g  0.5g/0.75g  1.0g
/* Unbalanced Discs            <3gpc      0p3gpc    0p5gpc    0p7gpc */
    #if((MAX_READ_SPEED == e16XCAV)&&(MAX_READ_DVD_SPEED == e8XDCAV)&&(MAX_READ_BD_SPEED==e4XDCAV))//711 use
#if(PCB == 711)
/* 0. Max CD Read */       {  e16XCAV,  e16XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e8XDCAV,  e8XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e4XDCAV,  e4XDCAV,  e3XDCLV,  e3XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
#else
/* 0. Max CD Read */       {  e16XCAV,  e16XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e8XDCAV,  e8XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
#endif
    #elif((MAX_READ_SPEED == e10XCAV)&&(MAX_READ_DVD_SPEED == e4XDCAV)&&(MAX_READ_BD_SPEED==e2XDCLV))//692 use
/* 0. Max CD Read */       {  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #elif((MAX_READ_SPEED == e10XCAV)&&(MAX_READ_DVD_SPEED == e8XDCAV)&&(MAX_READ_BD_SPEED==e2XDCLV)&&(DVD_692RUN4XCLV == 1))
/* 0. Max CD Read */       {  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},
/* 2. Max BD Read */       {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #elif((MAX_READ_SPEED == e10XCAV)&&(MAX_READ_DVD_SPEED == e4XDCLV)&&(MAX_READ_BD_SPEED==e2XDCLV))//815 use
/* 0. Max CD Read */       {  e10XCAV,  e10XCAV,  e10XCAV,  e10XCAV},
/* 1. Max DVD Read */      {  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},
/* 2. Max BD Read */       {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV},
/* 3. Max RAM Read */      {  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}
    #else//710 use
/* 0. Max CD Read */       {  MAX_READ_SPEED,     MAX_READ_SPEED,      e16XCAV,  e16XCAV},
/* 1. Max DVD Read */      {  MAX_READ_DVD_SPEED, MAX_READ_DVD_SPEED,  e8XDCAV,  e8XDCAV},
/* 2. Max BD Read */       {  MAX_READ_BD_SPEED,  MAX_READ_BD_SPEED,   e4XDCAV,  e4XDCAV},
/* 3. Max RAM Read */      {  MAX_READ_RAM_SPEED, MAX_READ_RAM_SPEED,  e5XDCAV,  e5XDCAV}
    #endif
};
#endif//#if ((DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)|| (BD_UNBALANCE_DISC_CHECK == 1))

/***************************************************
*          Host Speed Selection Tables             *
****************************************************/


/****************************************************************************
*   MMC Set CD Speed Command
*   Allows Host selection of Read and Write speeds
*   Selected Read Speed in BYTEs 2 and 3 (MSB,LSB)
*   Selected Write Speed in BYTEs 4 and 5 (MSB, LSB)
*   Max Speed - Vendor specific values - see vu_cfg.h
*****************************************************************************/
const USHORT SelCDReqSpeed_tbl[CD_SPD_COLMNS]={
    SP4X,       /* 0x02C2   705.6 Kbyte */  /* 8cmm media */
    SP8X,       /* 0x0583  1411.2 Kbyte */
    SP10X,      /* 0x06E4  1764.0 Kbyte */
    SP12X,      /* 0x0845  2116.8 Kbyte */
    SP16X,      /* 0x0B06  2822.4 Kbyte */
    SP20X,      /* 0x0DC8  3528.0 Kbyte */
    SP24X,      /* 0x108A  4233.6 Kbyte */
    SP32X,      /* 0x160D  5644.8 Kbyte */
    SP40X,      /* 0x1B90  7056.0 Kbyte */
    SP48X,      /* 0x2113  8467.2 Kbyte */
    SP52X       /* 0X23D4  9172.8 Kbyte */
};
/****************************************************************************
*  MMC Set DVD Speed Command
*  Allows Host selection of Read and Write speeds
*  Selected Read Speed in BYTEs 2 and 3 (MSB,LSB)
*  Selected Write Speed in BYTEs 4 and 5 (MSB, LSB)
*  Max Speed - Vendor specific values - see vu_cfg.h
*
*
*  Note!  If you add entries them please also update symbol DVD_SPD_COLMNS
*         and also add needed columns in  DVDSelSpeed_tbl_R[]
*         and also add needed columns in  DVDSelSpeed_tbl_W[]
*****************************************************************************/
const USHORT SelDVDReqSpeed_tbl[]={
    SP1XDVD,    //  1385 Kbyte/Sec
    SP2XDVD,    //  2770 Kbyte/Sec
#if(DVD2p4XCLV == 1)
    SP2_4XDVD,  //  3324 Kbyte/Sec
#else
    SP3XDVD,  //  4155 Kbyte/Sec
#endif
    SP4XDVD,    //  5540 Kbyte/Sec
    SP5XDVD,    //  6925 Kbyte/Sec
    SP6XDVD,    //  8310 Kbyte/Sec
    SP8XDVD,    // 11080 Kbyte/Sec
    SP10XDVD,   // 13850 Kbyte/Sec
    SP12XDVD,   // 16620 Kbyte/Sec
    SP16XDVD,   // 22160 Kbyte/Sec
    SP20XDVD,   // 27700 Kbyte/Sec
};

const USHORT SelDVDReqWrSpeed_tbl[]={
    SP1XDVD,    //  1385 Kbyte/Sec
    SP2XDVD,    //  2770 Kbyte/Sec
#if(DVD2p4XCLV == 1)
    SP2_4XDVD,  //  3324 Kbyte/Sec
#else
    SP3XDVD,  //  4155 Kbyte/Sec
#endif
    SP4XDVD,    //  5540 Kbyte/Sec
    SP5XDVD,    //  6925 Kbyte/Sec
    SP6XDVD,    //  8310 Kbyte/Sec
    SP8XDVD,    // 11080 Kbyte/Sec
    SP8XDVD,    // 11080 Kbyte/Sec
    SP12XDVD,   // 16620 Kbyte/Sec
    SP16XDVD,   // 22160 Kbyte/Sec
    SP20XDVD,   // 27700 Kbyte/Sec
};


/****************************************************************************
*  MMC Set BD Speed Command
*  Allows Host selection of Read and Write speeds
*  Selected Read Speed in BYTEs 2 and 3 (MSB,LSB)
*  Selected Write Speed in BYTEs 4 and 5 (MSB, LSB)
*  Max Speed - Vendor specific values - see vu_cfg.h
*
*
*  Note!  If you add entries them please also update symbol BD_SPD_COLMNS
*         and also add needed columns in  BDSelSpeed_tbl_R[]
*         and also add needed columns in  BDSelSpeed_tbl_W[]
*****************************************************************************/
const USHORT SelBDReqSpeed_tbl[]={
    SP1XBD,    //  4495 Kbyte/Sec
    SP2XBD,    //  8891 Kbyte/Sec
#if(BD2p2XCLV == 1)
    SP2_2XBD,  //  9889 Kbyte/Sec
#endif     
    SP4XBD,    //  17982 Kbyte/Sec
    SP6XBD,    //  26970 Kbyte/Sec
    SP8XBD,    //  35960 Kbyte/Sec
    SP10XBD,   //  44950 Kbyte/Sec
    SP12XBD,   //  53940 Kbyte/Sec
    SP14XBD,   //  62930 Kbyte/Sec
};

const USHORT SelBDReqWrSpeed_tbl[]={
    SP1XBD,    //  4495 Kbyte/Sec
    SP2XBD,    //  8891 Kbyte/Sec
#if(BD2p2XCLV == 1)
    SP2_2XBD,  //  9889 Kbyte/Sec
#endif      
    SP4XBD,    //  17982 Kbyte/Sec
    SP6XBD,    //  26970 Kbyte/Sec
    SP8XBD,    //  35960 Kbyte/Sec
    SP10XBD,   //  44950 Kbyte/Sec
    SP12XBD,   //  53940 Kbyte/Sec
    SP14XBD,   //  62930 Kbyte/Sec
};

/****************************************************************************
*  MMC Set HD Speed Command
*  Allows Host selection of Read and Write speeds
*  Selected Read Speed in BYTEs 2 and 3 (MSB,LSB)
*  Selected Write Speed in BYTEs 4 and 5 (MSB, LSB)
*  Max Speed - Vendor specific values - see vu_cfg.h
*
*
*  Note!  If you add entries them please also update symbol BD_SPD_COLMNS
*         and also add needed columns in  HDSelSpeed_tbl_R[]
*         and also add needed columns in  HDSelSpeed_tbl_W[]
*****************************************************************************/
const USHORT SelHDReqSpeed_tbl[]={
    SP1XHD,    //  4678 Kbyte/Sec
    SP2XHD,    //  9356 Kbyte/Sec
    SP4XHD,    //  18712 Kbyte/Sec
};

const USHORT SelHDReqWrSpeed_tbl[]={
    SP1XHD,    //  4678 Kbyte/Sec
    SP2XHD,    //  9356 Kbyte/Sec
    SP4XHD,    //  18712 Kbyte/Sec
};

/*
These tables allow the remapping of the Host Requested speeds to the closest supported speed for a specific disc type
and operation.

The CDSelSpeed_tbl or DVDSelSpeed_tbl functions are used to index into one of these tables along with Media type
and operational mode (read/write). Typically the CD and DVD read tables use the Read0_f define as the first table entry.
For example:    speed = DVDSelSpeed_tbl_R[media][Read_DVDMax_f-Read0_f];
    The [Read_DVDMax_f-Read0_f] term defines the column
    The [media] term defines the row

The CD and DVD write tables use 0 as the first table entry
*/

#if (BD_ENABLE == 1)
/**********************************************************************
*   BD Read Speed table
***********************************************************************/
const BYTE  BDSelSpeed_tbl_R[BD_READ_MEDIA][BD_SPD_COLMNS]={
/*******************************************************************************************************************************************/
/*  Read0_f   Read1_f   Read2_f   Read3_f   Read4_f   Read5_f                                                                                                            */
/*     1X        2X        3X        4X        6X        8X        |    BD Disc Type     */
/*******************************************************************************************************************************************/
#if(BD2p2XCLV == 1)
#if (MAX_READ_BD_SPEED == e2XDCLV)
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  }   /* 5.READ BD-RE DL   */
#elif (MAX_READ_BD_SPEED == e3XDCLV)//[V08] Bios for A2 BD speed 
 #if(Customer_A2 == 1)
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 0.READ BD-ROM     */
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV   }   /* 5.READ BD-RE DL   */
 #elif((Customer_ZHD== 1)||(Customer_China== 1))//[W0K]
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 0.READ BD-ROM     */
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 1.READ BD-ROM DL  */
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 2.READ BD-R SL    */
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 3.READ BD-RE SL   */
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   },  /* 4.READ BD-R DL    */
 {  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV,  e3XDCLV   }   /* 5.READ BD-RE DL   */
 #else
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  }   /* 5.READ BD-RE DL   */
 #endif
#elif( MAX_READ_BD_SPEED == e4XDCAV)//711 use
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   }   /* 5.READ BD-RE DL   */
#elif (MAX_READ_BD_SPEED == e6XDCAV)
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   }   /* 5.READ BD-RE DL   */
#else//e8XDCAV
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e3XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   }   /* 5.READ BD-RE DL   */
#endif
#else  //#if(BD2p2XCLV == 1)
/*******************************************************************************************************************************************/
/*  Read0_f   Read1_f   Read3_f   Read4_f   Read5_f                                                                                                            */
/*     1X        2X        4X        6X        8X        |    BD Disc Type     */
/*******************************************************************************************************************************************/
#if (MAX_READ_BD_SPEED == e2XDCLV)
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV,  e2XDCLV  }   /* 5.READ BD-RE DL   */
#elif( MAX_READ_BD_SPEED == e4XDCAV)
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV   }   /* 5.READ BD-RE DL   */
#elif (MAX_READ_BD_SPEED == e6XDCAV)
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e6XDCAV   }   /* 5.READ BD-RE DL   */
#else//e8XDCAV
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 0.READ BD-ROM     */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 1.READ BD-ROM DL  */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 2.READ BD-R SL    */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 3.READ BD-RE SL   */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   },  /* 4.READ BD-R DL    */
 {  e2XDCLV,  e2XDCLV,  e4XDCAV,  e6XDCAV,  e8XDCAV   }   /* 5.READ BD-RE DL   */
#endif
#endif  //#if(BD2p2XCLV == 1)
};
#endif//#if (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
/**********************************************************************
*   HD Read Speed table
***********************************************************************/
const BYTE  HDSelSpeed_tbl_R[HD_READ_MEDIA][HD_SPD_COLMNS]={
/*******************************************************************************************************************************************/
/*  Read0_f   Read1_f   Read2_f                                                                                                            */
/*     1X        2X      4X       |    BD Disc Type     */
/*******************************************************************************************************************************************/
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  },  /* 0.READ HD-ROM     */
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  },  /* 1.READ HD-ROM DL  */
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  },  /* 2.READ HD-R SL    */
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  },  /* 3.READ HD-RW SL   */
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  },  /* 4.READ HD-R DL    */
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  },  /* 5.READ HD-RW DL   */
 {  e1XDCLV,  e1XDCLV,  e1XDCLV  }   /* 6.READ HD-RAM */
};
#endif
/**********************************************************************
*   DVD Read Speed table
***********************************************************************/
const BYTE  DVDSelSpeed_tbl_R[DVD_READ_MEDIA][DVD_SPD_COLMNS]={
/*******************************************************************************************************************************************/
/*  Read0_f   Read1_f   Read2_f   Read3_f   Read4_f   Read5_f   Read6_f   Read7_f   Read8_f   Read9_f   Read10_f    |                      */
/*     1X        2X      2.4X        4X       5X        6X        8X        10X      12X        16X       20X       |    DVD Disc Type     */
/*******************************************************************************************************************************************/
#if (MAX_READ_DVD_SPEED == e6XDCAV)
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV,  e6XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
#elif (MAX_READ_DVD_SPEED == e4XDCAV)
 #if(Customer_A2 == 1)//[V08] Bios A2
 {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 0.READ DVD-ROM       */
 {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
 #else
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV,  e4XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
 #endif
 #elif ((MAX_READ_DVD_SPEED == e8XDCAV) && (PCB == 711))//711 use
 #if(Customer_Dune_HD == 1)//[V08]Bios HDI DVD 8x CAV
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
 #else 
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
 #endif
#elif (MAX_READ_DVD_SPEED == e8XDCAV)//710 use
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
#elif ((MAX_READ_DVD_SPEED == e8XDCAV)&&(DVD_692RUN4XCLV == 1))
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCLV,  e4XDCLV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCLV,  e4XDCLV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCLV,  e4XDCLV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCLV,  e4XDCLV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCLV,  e4XDCLV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
 #elif (MAX_READ_DVD_SPEED == e4XDCLV)//815 use
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,   e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,   e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,   e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,   e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},  /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,   e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV,  e4XDCLV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}    /* 5.READ DVD-RAM       */
#else//(MAX_READ_DVD_SPEED == e16XDCAV)
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV, e12XDCAV, e16XDCAV, e16XDCAV},  /* 0.READ DVD-ROM       */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV, e12XDCAV, e12XDCAV, e12XDCAV},  /* 1.READ DVD-ROM DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV, e12XDCAV, e16XDCAV, e16XDCAV},  /* 2.READ DVD+/-R SL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},   /* 3.READ DVD+/-RW SL   */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e4XDCAV,  e4XDCAV,  e6XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV,  e8XDCAV},  /* 4.READ DVD+/-R DL    */
 {  e2XDCLV,  e2XDCLV,  e2XDCLV,  e3XDCLV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV,  e5XDCAV}   /* 5.READ DVD-RAM       */
#endif
};

/**********************************************************************
*   CD Read Speed table
***********************************************************************/

const BYTE  CDSelSpeed_tbl_R[CD_READ_MEDIA][CD_SPD_COLMNS]={
/****************************************************************************************************************************************************/
/*  Read0_f  Read1_f Read2_f  Read3_f  Read4_f  Read5_f  Read6_f  Read7_f  Read8_f  Read9_f  Read10_f    |                                          */
/*     4X      8X       10X      12X      16X      20X     24X      32X      40X      48X      52X       |              CD Disc Type                */
/****************************************************************************************************************************************************/
#if(MAX_READ_SPEED == e52XCAV)
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e48XCAV, e52XCAV },  /* 0.READ CDROM,R,mixcd                    */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e40XCAV, e40XCAV },  /* 1.READ RW Closed Session                */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e24XCAV, e24XCAV, e24XCAV, e24XCAV },  /* 2.READ RW After pk wr                   */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e40XCAV, e40XCAV },  /* 3.READ OPEN RW data                     */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e48XCAV, e52XCAV }   /* 4.READ CDROM/R Audio Extraction         */
#elif (MAX_READ_SPEED == e48XCAV)
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e48XCAV, e48XCAV },  /* 0.READ CDROM,R,mixcd                    */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e40XCAV, e40XCAV },  /* 1.READ RW Closed Session                */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e24XCAV, e24XCAV, e24XCAV, e24XCAV },  /* 2.READ RW After pk wr                   */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e40XCAV, e40XCAV },  /* 3.READ OPEN RW data                     */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e40XCAV, e48XCAV, e48XCAV }   /* 4.READ CDROM/R Audio Extraction         */
#elif (MAX_READ_SPEED == e32XCAV)
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e32XCAV, e32XCAV, e32XCAV },  /* 0.READ CDROM,R,mixcd                    */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e32XCAV, e32XCAV, e32XCAV },  /* 1.READ RW Closed Session                */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e24XCAV, e24XCAV, e24XCAV, e24XCAV },  /* 2.READ RW After pk wr                   */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e32XCAV, e32XCAV, e32XCAV },  /* 3.READ OPEN RW data                     */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e16XCAV, e16XCAV, e24XCAV, e32XCAV, e32XCAV, e32XCAV, e32XCAV }   /* 4.READ CDROM/R Audio Extraction         */
#else//(MAX_READ_SPEED == e16XCAV)
 #if(Customer_A2 == 1)//[V08] Bios A2
 {  e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 0.READ CDROM,R,mixcd                    */
 {  e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 1.READ RW Closed Session                */
 {  e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 2.READ RW After pk wr                   */
 {  e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 3.READ OPEN RW data                     */
 {  e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV }   /* 4.READ CDROM/R Audio Extraction         */
 #else //[S0A] Shin debug
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 0.READ CDROM,R,mixcd                    */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 1.READ RW Closed Session                */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 2.READ RW After pk wr                   */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV },  /* 3.READ OPEN RW data                     */
 {  e4XCLV, e4XCLV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV, e10XCAV }   /* 4.READ CDROM/R Audio Extraction         */
 #endif
#endif  // MAX_READ_SPEED
};
