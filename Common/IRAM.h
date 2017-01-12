/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : iram.h
* DESCRIPTION   :
*
*   $Revision: 43 $
*   $Date: 11/03/16 5:59p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#ifndef _IRAM_H_
#define _IRAM_H_

#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\servo\svo.h"        /* Please include no servo file except svo.h */
#include ".\player\plrdb.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_key.h"
#include ".\common\system.h"

struct IramBoolT{
vBOOL                       fCDRW;
vBOOL                       reload;
//volatile LaserWriteModeT    fLaserWriteMode:1;
vBOOL                       fSubQIntFalg:1;
vBOOL                       fEFMValid:1;
vBOOL                       fATIPIntFalg:1;
vBOOL                       fLeadInFound:1;
vBOOL                       FindRunOutReqFlag:1;
vBOOL                       WriteStart:1;
vBOOL                       FoundRunOut_valid:1;
vBOOL                       SyncCacheExeFlag:1;
vBOOL                       StartMultiBlock:1;
vBOOL                       ReqErase:1;
vBOOL                       ReqErase10:1;
vBOOL                       ReqFormat:1;
vBOOL                       ReqClose:1;
vBOOL                       Send_Giveup_F:1;
vBOOL                       fPCATimeOffsetFlag:1;
vBOOL                       AddBufferingFlg:1;
vBOOL                       Rw_SOTC_st:1;
vBOOL                       audio_scan_st:1;
vBOOL                       St_fast_cache_hit:1;
vBOOL                       MediaTrayEvent:1;
vBOOL                       WriteComplete:1;
vBOOL                       HostDataReceiving:1;
vBOOL                       fIDIntflag:1;
};

extern struct IramBoolT IramBool;

extern vBYTE  Iram_fSubQIntFlag;
extern vBYTE  Iram_fATIPIntFlag;
extern vBYTE  Iram_StartMultiBlock;
extern vBYTE  Iram_reload;
extern vBYTE  Iram_St_fast_cache_hit;
extern vBYTE  Iram_EverFKNG;
extern vULONG Iram_Buf_dram_addr;
extern vSHORT  Iram_stp_CurrentPosition;
extern vUSHORT Iram_stp_StepPosition;
extern vBYTE  Iram_BdWriteStatus;
#if COLLI_SWITCH ==1
extern vSHORT  Iram_colli_CurrentPosition;
extern vUSHORT Iram_colli_StepPosition;
#endif
#if Monitor_FPDO_Linearity ==1
extern ULONG  SampleTimer;
#endif
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
extern vBOOL   IceFound;
#endif
extern ULONG    Iram_atapi_timer;

extern struct ButtonInfo_S volatile EjectKeyInfo;

#define Iram_fLaserWriteMode        IramBool.fLaserWriteMode
#define Iram_fCDRW                  IramBool.fCDRW
#define Iram_fEFMValid              IramBool.fEFMValid
#define Iram_fLeadInFound           IramBool.fLeadInFound
#define Iram_FindRunOutReqFlag      IramBool.FindRunOutReqFlag
#define Iram_FoundRunOut_valid      IramBool.FoundRunOut_valid
#define Iram_Send_Giveup_F          IramBool.Send_Giveup_F
#define Iram_fPCATimeOffsetFlag     IramBool.fPCATimeOffsetFlag
#define Iram_AddBufferingFlg        IramBool.AddBufferingFlg
#define Iram_Rw_SOTC_st             IramBool.Rw_SOTC_st
#define Iram_audio_scan_st          IramBool.audio_scan_st
#define Iram_MediaTrayEvent         IramBool.MediaTrayEvent
#define Iram_HostDataReceiving      IramBool.HostDataReceiving
#define Iram_fIDIntflag             IramBool.fIDIntflag

//Get event status notification fail of Command set
extern vUSHORT    STANDBYCNT;
extern vUSHORT    CMD_IDLE_CNT;                   /* no any command request from host*/


#endif /* _IRAM_H_ */
