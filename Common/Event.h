/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   event.h
*
* DESCRIPTION
*
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#ifndef _Event_H_
#define _Event_H_

/********************************************************/
/*                                                      */
/*  the definition of system events                     */
/*                                                      */
/********************************************************/

#define CLEAR_EVENT         0x00000000L     /* prcon.c uses */

/* system common event */

#define EV_TIMEOUT          0x80000000L         /* timeout to wait event  */

/* HIF system event    */


/* Mount rainier events HIF <-> FGP */

#define EJECT               0x00000001L     /* user pressed the eject button */
#define EJECT_DONE          0x00000002L     /* eject request is handled */
#define STARTUP_MRW         0x00000004L     /* MRW startup */
#define STARTUP_MRW_DONE    0x00000008L     /* MRW Startup end */
#define FLUSH_CACHE_DONE    0x20000000L     /* Flash Cache end */

#define MAX_STARTUP_MRW_TIM 30000           /* MRW STARTUP MAX TIME */

/* Mount rainier events FGP <-> BGP */

#define READ_EXECUTOR       0x00000010L     /* a read from disc is initiated */
#define WRITE_EXECUTOR      0x00000020L     /* a write to disc is initiated */
#define INIT_FORMAT         0x00000040L     /* start initial format */
#define FINALIZE_FORMAT     0x00000080L     /* finalize format */
#define BACKGROUND_FORMAT   0x00000100L     /* start a background format */
#define UPDATE_DT           0x00000200L     /* update the defect tables */
#define PRESCAN             0x40000000L     /* start prescan */
#define ABORT_BGP           0x00000400L     /* abort the background process */

#define HT_DATA             0x00000800L     /* a packet is freed from cache */
#define HT_2K_DONE          0x00001000L     /* a read or write is finished */
#define BGP_DONE            0x00002000L     /* BGP finished or aborted successfully */
#define BGP_FAILED          0x00004000L     /* BGP finished because of an error */

#define DT_RESTART          0x00008000L     /* BGP finished because of an error */

/* Mount rainier events BGP <-> WRP */

#define WRITE_PACKETLIST    0x00010000L     /* a packetlist must be written */
#define WRITE_DT            0x00020000L     /* defect tables must be written */
#define WRITE_IP            0x00040000L     /* information packet must be written */
#define WRITE_PREGAP        0x00080000L     /* pregap must be written */
#define WRITE_PMA           0x00100000L     /* PMA must be written */
#define WRITE_LI            0x00200000L     /* Lead in must be written */
#define WRITE_LO            0x00400000L     /* Lead out must be written */
#define WRITE_DEICE         0x00800000L     /* de-ice pattern must be written */
#define ABORT_DEICE         0x01000000L     /* abort write of de-ice pattern */
#define DO_OPC              0x02000000L     /* perform OPC */

#define WRITE_DONE          0x04000000L     /* write is finished */
#define WRITE_FAILED        0x08000000L     /* write failed */

/* Mount rainier event  Enocode interrupt -> WRP */

#define ONE_PKT_WRITTEN     0x10000000L     /* actual one packet is written */

#define EV_WRP_COMMAND      0x08000000L     /* WRP command  */
#define EV_WRP_COMPLETE     0x04000000L     /* WRP complete */

#endif /* _Event_H_ */

