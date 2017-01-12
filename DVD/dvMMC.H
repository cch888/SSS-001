/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME: MMC.H
*
* DESCRIPTION:
*           The MMC Commands related common defination.
*
*   $Revision: 8 $
*   $Date: 11/03/28 2:36p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _MMC_H_
#define _MMC_H_


#define FINISH_TRACK_MODE           41
#define MIN_BLOCKS_TO_WRITE         298     /* 4 sec - 2 ro blocks */
#define NO_NUMB                     0       /* move to constant.h */
#define SAME                        0x00    /* move */
#define STATE_TRACKING              5
#define NR_INT_PARAM                13
#define PMA_READ_ENTRIES_NUMBER     10

/* Lower 4 bits of the wr_method field contains the following*/
/* write method constants*/
#define WM_UNKNOWN                  0x00    /* uninitilized value*/
#define WM_TAO                      0x01    /* track at once*/
#define WM_FPKT                     0x02    /* fixed packet*/
#define WM_VPKT                     0x03    /* variable packet*/
#define WM_RESERVED                 0x0f    /* reserved track   should be WM_TAO*/


#define MMC_AVAIL_BUF_COUNT         0x1f


#define MMC_WR_TAO                  0x00
#define MMC_WR_FPKT                 0x01
#define MMC_WR_VPKT                 0x02
#define MMC_WR_RAW                  0x03
#define MMC_WR_RETRY_FPKT           0x04

#define MMC_WR_CL_SES               0x10
#define MMC_WR_CL_TRK               0x11
#define MMC_WR_RES_TRK              0x12

#define MMC_WR_CL_TRK_FF_PADDING    0x14
#define MMC_WR_CL_TRK_FF            0x15

#define MMC_WR_BUFFERING            0x20

#define MMC_WR_PAD_TAO              0x30
#define MMC_WR_PAD_FPKT             0x31
#define MMC_WR_PAD_VPKT             0x32

#define MMC_WR_BLANK                0xA1

#define MMC_WR_NOT                  0xff
#define START_WRITE_PACKET_BUF_COUNT    (BUF_TOTAL_BLK_COUNT - MMC_AVAIL_BUF_COUNT)
#define START_WRITE_BUF_COUNT           (BUF_TOTAL_BLK_COUNT - MMC_AVAIL_BUF_COUNT)


/* MMC Writing Packet*/
#define MMC_WRTYPE_PKT              0x00
/* MMC Writing Track At Once*/
#define MMC_WRTYPE_TAO              0x01
/* MMC Writing Session At Once*/
#define MMC_WRTYPE_SAO              0x02
/* MMC Writing Disc At Once*/
#define MMC_WRTYPE_DAO              0x03


#define PREGAP_WR_FPKT              0x91
#define PREGAP_WR_VPKT              0x90
#define PREGAP_WR_TAO               0x80

#define CMD_OPC_MMC_WRITE10         0x2a
#define CMD_OPC_MMC_RDTRKINFO       0x52
#define CMD_OPC_MMC_SYNCCACHE       0x35
#define CMD_OPC_MMC_READBUFCAP      0x5c
#define CMD_OPC_MECHSTATUS          0xBD
#define CMD_OPC_MMC_DIAG            0xFB
#define CMD_OPC_MMC_CLOSE_SES       0x5B
#define CMD_OPC_MMC_FORMAT          0x04
#define CMD_OPC_MMC_DCERASE         0xF4
#define CMD_OPC_MMC_BLANK           0xA1

#include ".\hif\gp_ram.h"
#include ".\player\plrplayr.H"


extern unsigned char               PlayerStatusValue;


/* Host IF Subsystem interface structure */
typedef struct
{
    unsigned char           PlayerState;
    ExecutionStateT         PlayerStatus;
    plrAcceptanceT          PlayerAccept;
    BYTE                    PlayerIDC;
    HostIfCommandT          PlayerOpcode;
    BYTE                    PlayerParameters[16];
} HostInterfaceInfoS;

extern HostInterfaceInfoS   HIF;

/* Optimized macro for setting integer, long value.*/
#define SET_INT_BYTE_HIGH(i) *(((byte *)&i)+0)
#define SET_INT_BYTE_LOW(i) *(((byte *)&i)+1)

extern void Delays( BYTE );

extern   vBYTE    Iram_Svr_player_mode;
extern   vBYTE    Iram_Rw_read_mode;
extern   vBYTE    Iram_Player_output[12];
//extern   BYTE    Rw_seek_retry;
extern   vBYTE    Iram_Rw_read_mode;        /* cd-rom's mode */
extern   BYTE    Current_read_sp;
extern   vULONG  Iram_Buf_dram_addr;
extern FLAG ServoSetWriteSpeed(BYTE speed);
extern BOOL   ServoStop(void);
extern BOOL   ServoStartUp(void);



extern void   GetParameterDataFromHost(ULONG, ULONG) ;
extern void   RequestSenseCommand(void);


extern void   UpdateTOCData(void);
extern void   ReadyLED(BYTE led_status);
extern void   WriteLED(BYTE led_status);
extern void   ErrorLED(BYTE led_status);

#endif /* _MMC_H_ */
