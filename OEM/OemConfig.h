/********************************************************************************
*               (c) Copyright 2003 - 2009 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME: OemConfig.h
*
* PROJECT NAME: sun710(DH4O3S)
*
* OEM     NAME: SUNEXT
*
* DESCRIPTION: This file contains OEM config information.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
* $Revision: 8 $
* $Date: 11/02/25 5:52p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __OEM_CONFIG_H__
#define __OEM_CONFIG_H__
#if(PCB == 815)
#define FIRMW_DISC_UPDATE_STR    'S','U','N','8','1','5','F','W','U','P','D','A','T','E'
#define FIRMW_INQ_VENDOR         'S','U','N','E','X','T',' ',' '
#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','8','1','5'
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'U','S','E','N','T','X',' ',' ','D','B','R',' ','A','E','E','D',' ','R','U','S','8','N','5','1',\
                                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#elif (PCB == 710)
#define FIRMW_DISC_UPDATE_STR    'S','U','N','7','1','0','F','W','U','P','D','A','T','E'
#define FIRMW_INQ_VENDOR         'A','T','A','P','I',' ',' ',' '
#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','7','1','0'
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'U','S','E','N','T','X',' ',' ','D','B','R',' ','A','E','E','D',' ','R','U','S','7','N','0','1',\
                                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#elif (PCB == 711)
#define FIRMW_DISC_UPDATE_STR    'B','P','2','8','V','F','W','U','P',' ',' ',' ',' ',' '
#define FIRMW_INQ_VENDOR         'B','D','R','O','M',' ',' ',' '
#define FIRMW_INQ_PRODUCT        'B','P','-','2','8','V',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'D','B','O','R',' ','M',' ',' ','P','B','2','-','V','8',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',\
                                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
//#define FIRMW_DISC_UPDATE_STR    'S','U','N','7','1','1','F','W','U','P','D','A','T','E'
//#define FIRMW_INQ_VENDOR         'A','T','A','P','I',' ',' ',' '
//#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','7','1','1'
//#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
//#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
//#define FIRMW_ID_MODEL           'U','S','E','N','T','X',' ',' ','D','B','R',' ','A','E','E','D',' ','R','U','S','7','N','0','1',
//                                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#elif (PCB == 690)
#define FIRMW_DISC_UPDATE_STR    'S','U','N','6','9','0','F','W','U','P','D','A','T','E'
#define FIRMW_INQ_VENDOR         'S','U','N','E','X','T',' ',' '
#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','6','9','0'
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'U','S','E','N','T','X','B',' ',' ','D','R','T','I','E','L','R',' ','A','E','E','D',' ','R','U',\
                                 'S','6','N','0','9',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#elif (PCB == 692)
#define FIRMW_DISC_UPDATE_STR    'S','U','N','6','9','2','F','W','U','P','D','A','T','E'
#define FIRMW_INQ_VENDOR         'S','U','N','E','X','T',' ',' '
#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','6','9','2'
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'U','S','E','N','T','X','B',' ',' ','D','R','T','I','E','L','R',' ','A','E','E','D',' ','R','U',\
                                 'S','6','N','2','9',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#elif (PCB == 812)
#define FIRMW_DISC_UPDATE_STR    'S','U','N','6','9','2','F','W','U','P','D','A','T','E'
#define FIRMW_INQ_VENDOR         'S','U','N','E','X','T',' ',' '
#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','6','9','2'
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'U','S','E','N','T','X','B',' ',' ','D','R','T','I','E','L','R',' ','A','E','E','D',' ','R','U',\
                                 'S','6','N','2','9',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#elif (PCB == 817)
#define FIRMW_DISC_UPDATE_STR    'S','U','N','8','1','7','F','W','U','P','D','A','T','E'
#define FIRMW_INQ_VENDOR         'A','T','A','P','I',' ',' ',' '
#define FIRMW_INQ_PRODUCT        'B','D',' ','R','E','A','D','E','R',' ','S','U','N','8','1','7'
#define FIRMW_INQ_DATETIME       ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#define FIRMW_INQ_SUBVER         ' ',' ',' ',' '
#define FIRMW_ID_MODEL           'U','S','E','N','T','X',' ',' ','D','B','R',' ','A','E','E','D',' ','R','U','S','8','N','7','1',\
                                 ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
#endif
#define BIOS_MODEL              0
#if (CEProduct == 0)
#define HIPM_ENABLED            1
#define DIPM_ENABLED            1
#else
#define HIPM_ENABLED            0
#define DIPM_ENABLED            0
#endif
#define SSP_ENABLED             1
#define SATA_AN_ENABLED         0
#define SSC_ENABLED             0
#define READ_LOG_EXT_ENABLED    0
#define FLUSH_CACHE_SUPPORTED   1
#define AAM_SUPPORTED           0
#define RMSN_SUPPORTED          0
#define PA0_WITH_LED            0
#define IDVER_SUPPORTED         0
#define CMPLY_SUPPORTED         0
#define DRIVE_SN_LENGTH         12
#endif
