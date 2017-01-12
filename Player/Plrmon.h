/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrmon.h
* DESCRIPTION   : This is Player Moniter
* NOTES         :
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*    1.00
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _PLRMON_H_
#define _PLRMON_H_
/*************************************************************************/
/*                        Logginng                                       */
/*************************************************************************/
/*
           | 0 |   1   |  2-3  | 4 |   5   |      6-9     | 10 - 15 |
           +---+-------+-------+---+-------+--------------+---------+
  Command  |'C'| Mytsk | Phase |'>'| Stsk  | Command Name | Message |
           +---+-------+-------+---+-------+--------------+---------+
  Response |'R'| Stsk  | Phase |'<'| Mytsk | Command Name | Message |
           +---+-------+-------+---+-------+--------------+---------+
  Get(Cmd) |'G'| Mytsk | Phase |   | '='   | Command Name | Message |
           +---+-------+-------+---+-------+--------------+---------+
  Get(Res) |'G'| Mytsk | Phase |   | Result Message                 |
           +---+-------+-------+---+-------+--------------+---------+

      Mytsk          = My   Task Code                     (1Byte ASCII)
      Pahse          = My   Task Phase                    (2Byte ASCII)
      Stsk           = Send Task Code                     (1Byte ASCII)
      Command Name   = Command Change Logging Code        (4Byte ASCII)
      Message        = Message(Max 6BYTE)                 (6Byte Binary)
      Result Message = Ready or Error

          >Mytsk/Stsk
            's' - Start Stop
            'p' - Play
            'r' - Record
            't' - Tray
            'i' - Infomation
            'm' - Misc
            'w' - Watch
            '+' - Player
            'v' - Servo Monitor
            'o' - IOPC
            '*' - Non Task

*/

#define SAVE_LOG                0

/*************************************************************************/
/*                        Data type difinitions                          */
/*************************************************************************/

/* Messgae ID */
enum {
    eMsgID_SStp      , /* Start Stop    */
    eMsgID_Play      , /* Play          */
    eMsgID_Tray      , /* Tray          */
    eMsgID_Info      , /* Infomation    */
    eMsgID_Misc      , /* Misc          */
    eMsgID_Watch     , /* Watch         */
    eMsgID_Player    , /* Player        */
    eMsgID_Svmn      , /* Servo Monitor */
    eMsgID_Non       , /* Non Task      */
    eMsgID_HIF         /* HIF           */
};

/* Task Type */
enum {
    eNonTask, /* Non      Task */
    eMsgTask, /* Messsage Task */
    eReqTask  /* Request  Task */
};

/* Command */
enum {
    eIdle = 0
};

/* Phase */
enum {
    ePhase00,ePhase01,ePhase02,ePhase03,ePhase04,ePhase05,ePhase06,ePhase07,
    ePhase08,ePhase09,ePhase10,ePhase11,ePhase12,ePhase13,ePhase14,ePhase15,
    ePhase16,ePhase17,ePhase18,ePhase19,ePhase20,ePhase21,ePhase22,ePhase23,
    ePhase24,ePhase25,ePhase26,ePhase27,ePhase28,ePhase29,ePhase30,ePhase31,
    ePhase32,ePhase33,ePhase34,ePhase35,ePhase36,ePhase37,ePhase38,ePhase39,
    ePhase40,ePhase41,ePhase42,ePhase43,ePhase44,ePhase45,ePhase46,ePhase47,
    ePhase48,ePhase49,ePhase50,ePhase51,ePhase52,ePhase53,ePhase54,ePhase55,
    ePhase56,ePhase57,
    ePhaseSame =  98,ePhaseInc = 99
};

#if SAVE_LOG
/* Log Type   */
enum {
    eLogSnd,
    eLogGet
};
#endif /* SAVE_LOG */

/* Task ID  */
#define mTid(x) (1 << x)

/* Time Set */
#define mTimeSet(x,m,s,f) x.Minute=m; x.Second=s; x.Frame=f;

/*************************************************************************/
/*                         Typedef                                       */
/*************************************************************************/
/* Base */
typedef struct {
    BYTE  len;      /* Length           */
    BYTE  cmdres;   /* Command/Response */
    BYTE  data[6];  /* data             */
} tMsg;

/* Command */
typedef struct {
    BYTE  len;      /* Length   */
    BYTE  cmd;      /* Command  */
    BYTE  msgid;    /* MsgId    */
    BYTE  reserved; /* Reserved */
} tMsg_Cmd;

/* Response */
typedef struct {
    BYTE  len;      /* Length   */
    BYTE  res;      /* Response */
    BYTE  result;   /* Result   */
    BYTE  data[5];  /* Data     */
} tMsg_Res;

/* Messgae */
typedef struct {
    BYTE  Size;                   /* Size          */
    BYTE  WrPtr;                  /* Write Pointer */
    BYTE  RdPtr;                  /* Read  Pointer */
    BOOL  NonSpace;               /* Non Space     */
    BYTE  Data[sizeof(tMsg) * 3]; /* Message Data  */
} tMsgBox;

/* Task Table */
typedef struct {
    BYTE (*ready)(void); /* Ready Task */
    BYTE (*erorr)(void); /* Error Task */
} tTask;

/* Cmd Table */
typedef struct {
    LPCHAR logmsg;       /* Log Msg  */
    BYTE   (*cmd)(void); /* Cmd Task */
    const  tTask  *task; /*     Task */
} tCmd;

#if (SERVICE_MODE==1)
/* Task Infomation */
typedef struct {
    BYTE cmdReq;      /* 04:Command Request */
    BYTE cmd;         /* 05:Command         */
    BYTE phase;       /* 06:Phase           */
    BYTE state;       /* 07:State           */
    BYTE msgId;       /* 08:Message ID      */
    tMsgBox msgBox;   /* 09:Msg Box         */
} tTaskInfo;
#endif
/*************************************************************************/
/*                        Function Prototypes                            */
/*************************************************************************/
extern void plrMemcpy(LPBYTE Dst,LPBYTE Src,USHORT Len);
extern void plrMemset(LPBYTE Dst,BYTE data,USHORT Len);

extern void plrMonInit(void);                             /* Init         */
extern void plrTaskInit(BYTE MsgId);                      /* Task Init    */
extern void plrMon(void);                                 /* Main         */
extern void plrTaskReq(BYTE MsgId);                       /* Task Req     */
extern void plrTaskCmdReq(BYTE MsgId,BYTE cmd);           /* Task Cmd Req */
extern void plrTaskClr(BYTE MsgId);                       /* Task Clr     */
extern BOOL plrSndMsg(BYTE MsgId,BYTE *Msg,BYTE   Size); /* Send Msg     */
extern BOOL plrGetMsg(BYTE MsgId,BYTE *Msg,LPBYTE Size); /* Get  Msg     */

#if SAVE_LOG
extern void plrLogStart(void);
extern void plrLogStop(void);
extern void plrMsgLog(BYTE Type,BYTE MsgId,BYTE *Msg);
extern void plrWriteLog(LPCHAR logmsg,BYTE len);          /* Write Log    */
#endif /* SAVE_LOG */

extern void plrWatchReq(BYTE NextPhase,BYTE Cmd,BYTE *Data);
extern void plrMiscReq(BYTE NextPhase,BYTE Cmd,LONG Address);
extern void plrInfoReq(BYTE nextPhase,BYTE cmd);
extern void plrMeReq(BYTE NextPhase);

extern void plrSetPhase(BYTE NextPhase);
extern BYTE plrGetPhase(void);

extern void plrMsgBoxClr(BYTE MsgId);

extern tMsg fPlrMsg; /* Messgae */

#endif /* _PLRMON_H_ */
