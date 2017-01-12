/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: Rd_Stru.h $
*
* DESCRIPTION   Header File for Rd_Stru.c
*
* $Revision: 20 $
* $Date: 11/01/17 6:30p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __RD_STRU_H__
#define __RD_STRU_H__

/*********************************************************************
    Read Disc Structure Format type define
*********************************************************************/

#define GEN_AACS_VOLUM_ID           0x80
#define GEN_AACS_MEDIA_SN           0x81
#define GEN_AACS_MEDIA_ID           0x82
#define GEN_AACS_MKB                0x83
#define GEN_AACS_DATA_KEY           0x84
#define GEN_AACS_BEF_SECTOR_EXTENTS 0x85
#define GEN_BCM_AACS_MEDIA_SN       0x8C
#define GEN_BCM_AACS_MEDIA_ID       0x8D
#define GEN_BCM_AACS_VOLUM_ID       0x8E
#define GEN_BCM_AACS_KCD            0x8F
#define GEN_SIGMA_AACS_KCD          0x84
#define GEN_MARVELL_AACS_KCD        0x84
#define GEN_INTEL_AACS_KCD          0x8B

#define GEN_RECOG_FMT_LAYER         0x90
#define GEN_WRITE_PROTECT_STS       0xC0
#define GEN_DISC_STRU_LIST          0xFF

#define DVDHD_PHY_FMT_INFO          0x00
#define DVD_COPYRIGHT_INFO          0x01
#define DVD_DISC_KEY                0x02
#define DVDHD_BCA_INFO              0x03
#define DVDHD_DISC_MANUF_INFO       0x04
#define DVD_COPYRIGHT_MANAGE        0x05
#define DVD_MEDIA_ID                0x06
#define DVD_MEDIA_KEY_BLOCK         0x07
#define DVDHDRAM_DDS                0x08
#define DVDHDRAM_MEDIA_STS          0x09
#define DVDHDRAM_SPARE_INFO         0x0A
#define DVDHDRAM_MEDIA_REC_TYPE     0x0B
#define DVDHD_LAST_BO_RMD           0x0C
#define DVD_LAST_RMA                0x0D
#define DVD_PREREC_LI_INFO          0x0E
#define DVDHD_DISC_ID               0x0F
#define DVDHD_CTRL_DATA_INFO        0x10
#define DVD_ADIP_INFO               0x11
#define HD_COPYRIGHT_INFO           0x12
#define DVDHD_AACS_COPYRIGHT_DATA   0x15
#define HD_MEDIUM_STS               0x19
#define HD_LAST_RMD                 0x1A
#define DVDHD_LAYER_CAPACITY        0x20
#define DVD_MIDDLE_ZONE_START       0x21
#define DVD_JUMP_INTERVAL_SIZE      0x22
#define DVD_MANUAL_LJUMP_ADDR       0x23
#define DVD_REMAP_INFO              0x24
#define DVD_DISC_CTRL_BLOCK         0x30
#define DVD_READ_MTA                0x31

#define BD_DISC_INFO                0x00
#define BD_DDS                      0x08
#define BD_CARTRIDGE_STS            0x09
#define BD_SA_INFO                  0x0A
#define BD_RAW_DFL                  0x12
#define BD_PAC                      0x30

#define BD_ALL_PAC_HEADER           0x000000
#define BD_KNOWN_PAC_HEADER         0xFFFFFF
/*********************************************************************
    Read Disc Structure Format-layer type code
*********************************************************************/
#define FMT_LR_CD                   0x08
#define FMT_LR_DVD                  0x10
#define FMT_LR_BD                   0x40
#define FMT_LR_HD                   0x50


/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/

/*********************************************************************
        Physical define
*********************************************************************/
#define PHYSICAL_LEN        17
#define PHYSICAL_HDR_LEN    4

/*********************************************************************
        Global Structures
*********************************************************************/
typedef struct {
    BYTE    FmtCode;
    BYTE    SDS_RDS;
    USHORT  Length;
} STRU_FMT_LIST_T;

/*******************************************************************
    ENUMERATIONS
*******************************************************************/
/* Key Format Types */
enum {
    KfNewAgid                   = 0x00,
    KfAacsDriveChallenge        = 0x01,
    KfAacsDriveKey              = 0x02,
    KfAacsRegionCode            = 0x08,
    KfSDAacsDriveResponse       = 0x10,
    KfSDAacsReadBindingNonce    = 0x11,
    KfVendorAacsDriveResponse   = 0x10,
    KfVendorAacsReadBindingNonce = 0x11,
    KfAacsGenerateBindingNonce  = 0x20,
    KfAacsReadBindingNonce      = 0x21,
    KfBcmAacsDriveChallenge     = 0x31,
    KfBcmAacsReadBindingNonce   = 0x32,
    KfInvalidateAgid            = 0x3F
};

/* Key Format Types */
enum
{
    KfBcmAacsHostChallenge  = 0x31,
    KfSDAacsHostNonce       = 0x10,
    KfSDAacsHostResponse    = 0x11,
    KfVendorAacsHostNonce   = 0x10,
    KfVendorAacsHostResponse = 0x11,
    KfAacsHostChallenge     = 0x01,
    KfAacsHostKey           = 0x02
};

#endif
