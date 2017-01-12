/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_mchst.h
*
* DESCRIPTION
*   This file contains Mechanism Status command handler
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __GP_MCHST_H__
#define __GP_MCHST_H__

/* Notification Class */
#define GESN_NEA                0x00
#define GESN_OPERATIONAL_CLASS  0x02    /* bit 1 */
#define GESN_POWER_CLASS        0x04    /* bit 2 */
#define GESN_MEDIA_CLASS        0x10    /* bit 4 */
#define GESN_DEVICE_BUSY_CLASS  0x40    /* bit 6 */

/* Operational Change Class */
#define GESN_OP_NOCHANGE        0
#define GESN_OP_CHANGE_REQUEST  1
#define GESN_OP_DRIVE_CHANGE    2

#define GESN_OP_AVAIL           0
#define GESN_OP_TEMP_BUSY       1
#define GESN_OP_BUSY            2

#define GESN_OP_FEATURE_CHANGE  1
#define GESN_OP_FEATURE_ADD     2
#define GESN_OP_RESET           3
#define GESN_OP_FIRMWARE_CHANGE 4
#define GESN_OP_INQUIRY_CHANGE  5

/* Power Management Class */
#define GESN_PWR_NOCHANGE       0
#define GESN_PWR_CHANGE_SUCCESS 1
#define GESN_PWR_CHANGE_FAIL    2

#define GESN_PWR_ACTIVE         1
#define GESN_PWR_IDLE           2
#define GESN_PWR_STANDBY        3
#define GESN_PWR_SLEEP          4

/* Media Event Class */
#define GESN_TRAY_OPEN      1
#define GESN_MEDIA_PRESENT  2

#define GESN_NO_EVENT       0
#define GESN_EJECT_REQUEST  1
#define GESN_NEW_MEDIA      2
#define GESN_MEDIA_REMOVAL  3
#define GESN_BGF_COMPLETED  5
#define GESN_BGF_RESTARTED  6

/* Device Busy Class */
#define GESN_DEV_NOCHANGE   0
#define GESN_DEV_BUSY       1

#define GESN_DEV_NOEVENT    0
#define GESN_DEV_POWER      1
#define GESN_DEV_IMMED      2
#define GESN_DEV_DEFERRED   3


extern void MechanismStatusCommand(void);
extern void GetEventStatusNotificationCommand(void);

#endif /* __GP_MCHST_H__ */

