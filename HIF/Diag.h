/****************************************************************************
*                (c) Copyright 2001 - 2006  SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   Diag.h
*
* DESCRIPTION
*   Header file for file Diag.c
*
*   $Revision: 132 $
*   $Date: 11/03/28 2:37p $
*
* NOTES:
*
**************************** SUNEXT  CONFIDENTIAL *******************************/
#ifndef _Diag_H_
#define _Diag_H_

#define bFcsBal     1
#define bLaser      2

#define bFeed       4
#define bOPC        8
#define bLDC        0x10

#define eReadPersistent     0
#define eWritePersistent    1
#define eMeasureBETA        2
#define eLaserCalibration   2
#define eVRDCOffsetCalib    0
#define eVWDCOffsetCalib    1
#define eVRDCDacCalib       2
#define eVWDCDacCalib       3
#define eVWDC2Calib         4
#define eConvertCalib1      5
#define eConvertCalib2      6
#define eWriteAllToFlash    0xFF
#define eSetDiscType        3
#define eDryWriteCdr        9
#define eDryWriteCdrw       10
#define eSetOPU2OD          0x80

#define eStrategyA          0
#define eStrategyB          1
#define eStrategyC          2

#define ConLoadLogging   0x11
#define ResetLogging     0x12
#define GetDRAMLogStatus 0x13
#define GetROMLogStatus  0x14
#define SaveLogToRom     0x15
#define GetKeyParaStatus 0x16
#define GetMsgMarks      0x1E
#define SetMsgMarks      0x1F


#define EnaLog            0x02
#define DisLog            0x01
#define ConLogSuccess     0x02
#define ConlogFail        0x01
#define New_sunWTOOL 0

void    diaPresetPower(void);
void    diaIncrementPower(void);
void    diaDecrementPower(void);

#if (DEBUG_LASER_OPEN_LOOP == 1)||(DEBUG_LASER_CLOSE_LOOP == 1)
void    diaSavePower();
void    diaReadPower();
#endif

void    diaVendorSetSpeedSeek(void);
#if(BCA_ENABLE == 1)
void    diaReadBCA(void);
#endif
void    diaBLERCommand(void);
void    diaServoAdjustCommand(void);
void    diaServoKeyPara(void);
void    z_calLoad_EepromMap(void);
void    z_calFlash_EepromMap(void);
void    z_calWrite_PowerCalibrationParam(void);
void    z_calWrite_SvoWordParam(BYTE addr,BYTE *bp);
void    z_calWrite_PcalParam(USHORT*addr,BYTE len,BYTE *bp);
BOOL    z_calRead_fgSvoWordParam(BYTE addr,BYTE *bp);
USHORT  z_calRead_PcalUintParam(BYTE *addr);
void    z_calWrite_PcalParamSum(void);

/* Sub-Function for OffLine Calbration Vendor Command */
USHORT  z_cmd0xF800(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF80004(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF80002(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF801(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF802(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF801XX01(USHORT Xfer_Num,int num,int ctimes);
USHORT  uSendFloNum(USHORT Xfer_Num,float fnum);
void    CovDVDRStaticWs(BYTE speed_index);
void    CovDVDRDynamicWs(BYTE speed_index);
void    CovDVDRWStaticWs(BYTE speed_index);

/* Sub-Function for Vendor Write Command */
USHORT  z_cmd0xF500(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF501(USHORT Xfer_Num,USHORT Xfer_Len);

USHORT  z_cmd0xF502CD(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF502DVD(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF503CD(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF503DVD(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF504(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF50401(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF50402(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF505(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF506(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF507(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  z_cmd0xF508(USHORT Xfer_Num,USHORT Xfer_Len);
USHORT  Convert2NewDiscKindT(BYTE disctype);

void    z_SetCDStrategy(BYTE ws_type);
long    Method1MSFToLBA(BYTE min, BYTE sec, BYTE frm);
void    z_SetPcaData(unsigned char PCAData);
void    z_SetWriteInt(USHORT mode);
void    z_EncStop(void);
vBOOL   Get_parameter_data(USHORT T_byte_count);
//CD8_WriteStrategy *GetCD8Addr(BYTE speed);
void    GetWriteSpeedParameter(void);
void    GetWritePowerParameter(void);
void    GetStaticWSParameter(void);
BOOL    isStandardWsType(BYTE ws_type);
void    GetDynamicWSParameter(void);
void    GetSampleHoldParameter(void);
void    GetSupportMediaInfo(void);

/********************* Diagnostic Functions *******************/
#if (SUN_FUNC_TEST == 1)
extern void diaLaserMeasurment(void);       /* EE: diaLaserMeasurment */
#endif
extern void diaCmdEnable(void);             /* F0: Enable/Disable Diagnostics */
extern void diaCmdSEEPROMAccess(void);      // F1:
extern void diaCmdMRW(void);                /* F2: MRW Test Suites */
extern void diaCmdJitterScan(void);         /* F3: Jitter Scan */
extern void diaVendorRead(void);            /* F4: */
extern void diaVendorWrite(void);           /* F5: Vendor Write Command */
extern void diaCmdDvdDiagnostic(void);      /* F6: */
extern void diaReadWriteDevice(void);       /* F7: READ/WRITE DEVICE COMMAND*/
extern void diaCmdOfflineCalibration(void); /* F8: OFF Line Calbration Vendor Command */
extern void diaCmdDCErase(void);            /* F9: DC Erase */
extern void diaDebugLogCtrl(void);           /* FE: DRAM debugging log area control Command*/
extern void diaCalFlashCheckSumCommand(void);
#if(PCB == 710)||(PCB == 711)||(PCB == 817)

extern BOOL diaMainCodeSelfCheck(void);// Add Main Code Check Sum protect

#endif

//extern WORD    wParamBuff[110];
extern BYTE    bDynamicBuff[80];

void LoaderMonitoringLog(void);
#if (STORE_MSG_TO_FLASHROM == 1)
void CopyMsgFromDataAreaToROM(void);
#endif
typedef union        /* Return Data structure for Diagnostic Transparent Receive (Send) Command  */
{
    BYTE  bytes[20];
    struct
    {
        BYTE  CurrentPlayerState;
        BYTE  CurrentServoState;
        BYTE  PlayerStateAtEndOfTransparentSend;
        BYTE  ServoStateAtEndOfTransparentSend;
        BYTE  Reserved_1;
        BYTE  Reserved_2;
        BYTE  ReturnParameters[14];
    } byte;
}TPRRDs;

#if P_DIAGS == 1
extern void diaTransparentSend(void);
extern void ClearPosReturnData(void);
extern TPRRDs TRReturn;
#define REMAP_ATAPI_COMMANDS    1
#endif

extern BOOL diaHifBufTransSetup(BOOL Dev2HostSel, BOOL PageModeSel, ULONG StartAddr, USHORT Total_Pages,USHORT PageLength);

extern void RDVDServoDryWrite(void);

extern BYTE Send_timef;
extern SHORT    bOPCArea;
extern ULONG    MDataAreaAddr;
extern BYTE     bEnableJitterOPC;
#if (SUN_FUNC_TEST == 1)
extern BYTE         READ_C1_C2;
#endif
#if (REGISTER_MONITOR == 1)
typedef struct
{
    ULONG       monitor_reg_addr;
    BYTE        length;
    BYTE        start_bit;
    BYTE        end_bit;
} RegMonitorT;

extern RegMonitorT gMonitorReg[16];
extern BYTE        gTotalMonitorCH;
extern BOOL        fMonitoringReg;
#endif //REGISTER_MONITOR == 1

/* Defines for sub commands for Vendor Read */
#define VR_SetSpeedSeek             0
#define VR_BLER_CMD                 1
#define VR_Servo_Adjust             2
#define VR_Flash_Checksum           3

#if (USE_REALTIME_JITTER == 1)
#define VR_GENERIC_MEASURE_CMD      4
typedef union _jitter_result
{
    USHORT us;
    struct
    {
        ULONG land;
        ULONG groove;
        ULONG totalNrLands;
        ULONG totalNrGrooves;
    } extResult;
} jitterResult_t;
extern USHORT hifRealtimeJitterMeasurement(jitterResult_t * extJitterResult);
typedef enum
{
    eGENERIC_JITTER_CMD,
    eGENERIC_BLER_CMD,          //not implemented yet!!!
    eGENERIC_BETA_CMD,          //not implemented yet!!!
    eGENERIC_ADER_CMD           //not implemented yet!!!
} eMeasureSortT;
typedef enum
{
    eGENERIC_CONTINUOUS_MODE,
    eGENERIC_TIMER_MODE,        //not implemented yet!!!
    eGENERIC_ADDRESS_MODE       //not implemented yet!!!
} eMeasureModeT;
/**
 *  hifRealtimeJitterMeasurement()
 *
 *  pre  : realtime jitter circuitry enabled
 *  post : jitter value read from hw, converted to jitter %
 *
 *  return : jitter %
 *           extJitter in case of DVDRAM
 *
 */
//extern USHORT hifRealtimeJitterMeasurement(jitterResult_t * extJitterResult);
//extern USHORT hifRealtimeJitterMeasurement(void);
#endif  /* USE_REALTIME_JITTER */
#define VR_ServoDSPDebug			5
#define VR_SledMoving			    6
#define VR_ServoKeyPara  			7


#if(BCA_ENABLE == 1)
#define VR_ReadBCA                            9
#endif
/* Defines for sub commands for Enable Diag Command (F0h) */
#define ENABLE_DIAG_COMMAND             bit0
#define VENDOR_RESET_DEVICE_COMMAND     bit1



/* Defines for sub commands for Vendor Write */
#define SEND_WRITE_PARAMETER        0
#define GET_WRITE_PARAMETER         1

#define READ_WRITE_STATUS           2
#define FORCE_WRITE                 3
#define WRITE_STRATEGY_BLOCK_TEST   4
#define VENDOR_OPC_TEST             5
#define GET_WRITE_STRATEGY_CONVERT  6
#define GET_SAMPLE_HOLD_CONVERT     7
#define GET_WRITE_STRATEGY_EDGES    8

/* Defines Sub Function for OFFLine Calbration Vendor Command */
#define POWER_CALIBRATION           0
#define SERVO_OFFLINE_CALIBRATION   1
#define ACCESS_PERSISTENT_TABLE     2

// Defines for sub commands for DVD diagnostics
#define DVD_SUBCMD_READ_PSN    0x00
#define DVD_SUBCMD_READ_LONG   0x01
#define DVD_SUBCMD_RESET_REGION   0x02

// For Vendor Write Mode Control
#define VENDOR_WRITE_MODE           0x00    /* don't update power for each PSN,and update a fix Tilt Dac */
#define REAL_WRITE_MODE             0x01    /* update power and Tilt DAC for each PSN */
#define DOW_WITH_DCERASE_MODE       0x02    /* do one time DC erase and one time real wrie for ecah loop */
#define RANDOM_MOVE_WRITE_SKEW_MODE 0x03    /* Random move Write skew value for DVDRW DOW Test */

/***********************************************
*      Vendor Write Parameter Memory Map       *
***********************************************/
//#define mNVWP_DISC_TYPE    (wParamBuff[0])
//#define mNVWP_SUB_TYPE     (wParamBuff[1])
//#define mNVWP_TABLE_NO     (wParamBuff[2])
//#define mNVWP_WS_TYPE      (wParamBuff[3])
//#define mNVWP_LAYER        (wParamBuff[4])
//#define mNVWP_MANUAL       (wParamBuff[5])
//#define mNVWP_MAX_SPEED    (wParamBuff[6])
//#define mNVWP_DM_SPEED     (wParamBuff[7])
//#define mNVWP_STGY_SPEED   (wParamBuff[8])
//#define mNVWP_POWER        (wParamBuff[9])
//#define mNVWP_RATIO        (wParamBuff[10])
//#define mNVWP_BETA_GAMMA   (wParamBuff[11])
//#define mNVWP_STEP         (wParamBuff[12])
//#define mNVWP_CENTER       (wParamBuff[13])
//#define mNVWP_RW_MULTI     (wParamBuff[14])
//
//#define mNVWP_WSPARAM(i)        (wParamBuff[15+i])
///* CDR Standard Castle WS format */
//#define mNVWP_CDR_dT3           (wParamBuff[15])
//#define mNVWP_CDR_dT4           (wParamBuff[16])
//#define mNVWP_CDR_dT5           (wParamBuff[17])
//#define mNVWP_CDR_dT            (wParamBuff[18])
//#define mNVWP_CDR_TLP           (wParamBuff[19])
//#define mNVWP_CDR_TDP3          (wParamBuff[20])
//#define mNVWP_CDR_TDP4          (wParamBuff[21])
//#define mNVWP_CDR_TDP           (wParamBuff[22])
///* CDRW Standard 1T Multi-Pulse WS format */
//#define mNVWP_CDRW_1T_dTTop3    (wParamBuff[15])
//#define mNVWP_CDRW_1T_dTTop     (wParamBuff[16])
//#define mNVWP_CDRW_1T_TMP       (wParamBuff[17])
//#define mNVWP_CDRW_1T_TLP       (wParamBuff[18])
//#define mNVWP_CDRW_1T_TC3       (wParamBuff[19])
//#define mNVWP_CDRW_1T_TC        (wParamBuff[20])
///* CDRW Standard 2T Multi-Pulse WS format */
//#define mNVWP_CDRW_2T_dT3       (wParamBuff[15])
//#define mNVWP_CDRW_2T_dTTop     (wParamBuff[16])
//#define mNVWP_CDRW_2T_T3        (wParamBuff[17])
//#define mNVWP_CDRW_2T_TMP       (wParamBuff[18])
//#define mNVWP_CDRW_2T_TLP       (wParamBuff[19])
//#define mNVWP_CDRW_2T_TC3       (wParamBuff[20])
//#define mNVWP_CDRW_2T_TC        (wParamBuff[21])
//#define mNVWP_CDRW_2T_delta1    (wParamBuff[22])
//#define mNVWP_CDRW_2T_delta2    (wParamBuff[23])
///* DVDR Standard Multi-Pulse SL WS format */
//#define mNVWP_DVDR_SLMP_TTop    (wParamBuff[15])
//#define mNVWP_DVDR_SLMP_dTTop   (wParamBuff[16])
//#define mNVWP_DVDR_SLMP_dTTop3  (wParamBuff[17])
//#define mNVWP_DVDR_SLMP_TLP     (wParamBuff[18])
//#define mNVWP_DVDR_SLMP_dTLE    (wParamBuff[19])
//#define mNVWP_DVDR_SLMP_TMP     (wParamBuff[20])
//#define mNVWP_DVDR_SLMP_TTop3   (wParamBuff[21])
///* DVDR Standard Multi-Pulse DL WS format */
//#define mNVWP_DVDR_DLMP_TTop    (wParamBuff[15])
//#define mNVWP_DVDR_DLMP_dTTop   (wParamBuff[16])
//#define mNVWP_DVDR_DLMP_dTTop3  (wParamBuff[17])
//#define mNVWP_DVDR_DLMP_TLP     (wParamBuff[18])
//#define mNVWP_DVDR_DLMP_TC      (wParamBuff[19])
//#define mNVWP_DVDR_DLMP_dTLE3   (wParamBuff[20])
//#define mNVWP_DVDR_DLMP_dTLE4   (wParamBuff[21])
//#define mNVWP_DVDR_DLMP_dTLE5   (wParamBuff[22])
//#define mNVWP_DVDR_DLMP_TTop3   (wParamBuff[23])
//#define mNVWP_DVDR_DLMP_TMP     (wParamBuff[24])
///* DVDR Standard Castle SL WS format */
//#define mNVWP_DVDR_SLCA_TTop    (wParamBuff[15])
//#define mNVWP_DVDR_SLCA_dTTop   (wParamBuff[16])
//#define mNVWP_DVDR_SLCA_dTTop3  (wParamBuff[17])
//#define mNVWP_DVDR_SLCA_TLP     (wParamBuff[18])
//#define mNVWP_DVDR_SLCA_dTLE    (wParamBuff[19])
//#define mNVWP_DVDR_SLCA_TC      (wParamBuff[20])
//#define mNVWP_DVDR_SLCA_TLP4    (wParamBuff[21])
//#define mNVWP_DVDR_SLCA_dTTop4  (wParamBuff[22])
//#define mNVWP_DVDR_SLCA_TI3     (wParamBuff[23])
///* DVDR Standard Castle DL WS format */
//#define mNVWP_DVDR_DLCA_TTop    (wParamBuff[15])
//#define mNVWP_DVDR_DLCA_dTTop   (wParamBuff[16])
//#define mNVWP_DVDR_DLCA_dTTop3  (wParamBuff[17])
//#define mNVWP_DVDR_DLCA_TLP     (wParamBuff[18])
//#define mNVWP_DVDR_DLCA_TC      (wParamBuff[19])
//#define mNVWP_DVDR_DLCA_dTLE3   (wParamBuff[20])
//#define mNVWP_DVDR_DLCA_dTLE4   (wParamBuff[21])
//#define mNVWP_DVDR_DLCA_dTLE5   (wParamBuff[22])
//#define mNVWP_DVDR_DLCA_TLP4    (wParamBuff[23])
//#define mNVWP_DVDR_DLCA_dTTop4  (wParamBuff[24])
//#define mNVWP_DVDR_DLCA_TI3     (wParamBuff[25])
///* DVDRW Standard 1T Multi-Pulse WS format */
//#define mNVWP_DVDRW_1T_TTop3    (wParamBuff[15])
//#define mNVWP_DVDRW_1T_TTop     (wParamBuff[16])
//#define mNVWP_DVDRW_1T_dTTop3   (wParamBuff[17])
//#define mNVWP_DVDRW_1T_dTTop    (wParamBuff[18])
//#define mNVWP_DVDRW_1T_TLP3     (wParamBuff[19])
//#define mNVWP_DVDRW_1T_TLP4     (wParamBuff[20])
//#define mNVWP_DVDRW_1T_TLP5     (wParamBuff[21])
//#define mNVWP_DVDRW_1T_TLP      (wParamBuff[22])
//#define mNVWP_DVDRW_1T_TMP      (wParamBuff[23])
//#define mNVWP_DVDRW_1T_dTera3   (wParamBuff[24])
//#define mNVWP_DVDRW_1T_dTera    (wParamBuff[25])
///* DVDRW Standard 2T Multi-Pulse WS format */
//#define mNVWP_DVDRW_2T_T3       (wParamBuff[15])
//#define mNVWP_DVDRW_2T_TTop     (wParamBuff[16])
//#define mNVWP_DVDRW_2T_dTTop3   (wParamBuff[17])
//#define mNVWP_DVDRW_2T_dTTop4   (wParamBuff[18])
//#define mNVWP_DVDRW_2T_dTTop5   (wParamBuff[19])
//#define mNVWP_DVDRW_2T_dTTop    (wParamBuff[20])
//#define mNVWP_DVDRW_2T_TLPO     (wParamBuff[21])
//#define mNVWP_DVDRW_2T_TLPE     (wParamBuff[22])
//#define mNVWP_DVDRW_2T_dTLPO    (wParamBuff[23])
//#define mNVWP_DVDRW_2T_dTLPE    (wParamBuff[24])
//#define mNVWP_DVDRW_2T_TMP      (wParamBuff[25])
//#define mNVWP_DVDRW_2T_dTera3   (wParamBuff[26])
//#define mNVWP_DVDRW_2T_dTeraO   (wParamBuff[27])
//#define mNVWP_DVDRW_2T_dTeraE   (wParamBuff[28])
//
//#define mNVWP_TD1(i)       (wParamBuff[15+i])
//#define mNVWP_TD2(i)       (wParamBuff[25+i])
//#define mNVWP_TD3(i)       (wParamBuff[35+i])
//#define mNVWP_TD4(i)       (wParamBuff[45+i])
//#define mNVWP_TD5(i)       (wParamBuff[55+i])
//#define mNVWP_TD6(i)       (wParamBuff[65+i])
//#define mNVWP_OPUDlyV      (wParamBuff[75])
//#define mNVWP_SMP1Ctrl     (wParamBuff[76])
//#define mNVWP_SMP1Pattern  (wParamBuff[77])
//#define mNVWP_SMP1Ldly     (wParamBuff[78])
//#define mNVWP_SMP1Wid      (wParamBuff[79])
//#define mNVWP_SMP2Ctrl     (wParamBuff[80])
//#define mNVWP_SMP2Pattern  (wParamBuff[81])
//#define mNVWP_SMP2Ldly     (wParamBuff[82])
//#define mNVWP_SMP2Wid      (wParamBuff[83])
//#define mNVWP_SMP3Ctrl     (wParamBuff[84])
//#define mNVWP_SMP3Pattern  (wParamBuff[85])
//#define mNVWP_SMP3Ldly     (wParamBuff[86])
//#define mNVWP_SMP3Wid      (wParamBuff[87])
//#define mNVWP_SMP4Ctrl     (wParamBuff[88])
//#define mNVWP_SMP4Pattern  (wParamBuff[89])
//#define mNVWP_SMP4Ldly     (wParamBuff[90])
//#define mNVWP_SMP4Wid      (wParamBuff[91])
//#define mNVWP_SMP5Ctrl     (wParamBuff[92])
//#define mNVWP_SMP5Pattern  (wParamBuff[93])
//#define mNVWP_SMP5Ldly     (wParamBuff[94])
//#define mNVWP_SMP5Wid      (wParamBuff[95])
//#define mNVWP_TiltDac1     (wParamBuff[96])
//#define mNVWP_TiltDac2     (wParamBuff[97])
//#define mNVWP_TiltDac3     (wParamBuff[98])
//#define mNVWP_TiltDac4     (wParamBuff[99])
//#define mNVWP_TiltDac5     (wParamBuff[100])
//#define mNVWP_WFocOffset   (wParamBuff[101])
//#define mNVWP_WTrkOffset   (wParamBuff[102])

/* For get information for support media table */
#define MEDIA_DISC_TYPE         (bDynamicBuff[0])
#define MEDIA_TABLE_NO          (bDynamicBuff[1])
#define MEDIA_MID(i)            (bDynamicBuff[2+i])
#define MEDIA_MAX_SPEED         (bDynamicBuff[15])
#define MEDIA_OPC_TYPE          (bDynamicBuff[16])
#define MEDIA_TUNED_SPEED(i)    (bDynamicBuff[17+i])

//#define mNVWP_TD1B(i)        wParamBuff[53+i]
//#define mNVWP_TD2B(i)        wParamBuff[63+i]


    #if (MSG_OUT_MODE == MSG_USBPORT)
void SendMsg(BYTE d1);
void SendMsg1(BYTE d1,BYTE d2,BYTE d3,BYTE d4);
void SendMsgCn(ULONG msg_gate, BYTE nArg, ULONG func, ...);
void SendMsgCnB(ULONG msg_gate, BYTE nArg, ULONG func, ...);
void send_msgA1(ULONG msg_gate,ULONG func_code,char data1);
void send_msgA2(ULONG msg_gate,ULONG func_code,short data1);
void send_msgA4(ULONG msg_gate,ULONG func_code,long data1);
void SendMsg80(ULONG msg_gate,ULONG func_code);
void SendMsg81(ULONG msg_gate,ULONG func_code,BYTE h1);
void SendMsg84(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2,BYTE h3,BYTE h4);
void SendMsg82(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2);
void send_msg74(ULONG msg_gate,ULONG func_code,float data1);
void SendMsg6C(ULONG msg_gate);
void SendMsg54(ULONG msg_gate,BYTE d1,BYTE d2,BYTE d3,BYTE d4);
void SendMsg5S(ULONG msg_gate, char *message);
void SendMsg4USHORTs(ULONG msg_gate, USHORT value1, USHORT value2, USHORT value3, USHORT value4);
void SendMsg5SValue(ULONG msg_gate, char *message, char size, ULONG value);
void SendMsg5L(ULONG msg_gate, ULONG value);
void SendMsg5F(ULONG msg_gate,BYTE h1,BYTE h2 ,BYTE h3 ,BYTE h4 ,BYTE h5 ,BYTE h6 ,BYTE h7 ,BYTE h8,
                              BYTE h9,BYTE h10,BYTE h11,BYTE h12,BYTE h13,BYTE h14,BYTE h15);
void SendMsg44(ULONG msg_gate,BYTE d1,BYTE d2,BYTE d3,BYTE d4);
void SendMsg3A(ULONG msg_gate);
void SendMsg21(ULONG msg_gate,ULONG func_code,BYTE dd1);
void SendMsg24(ULONG msg_gate,ULONG func_code,BYTE dd1,BYTE dd2,BYTE dd3,BYTE dd4);
        #if (ENABLE_UART_RXD == 1)
            #if (REGISTER_MONITOR == 1)
void SendMsgRegMonitorCmdReady(void);
void SendMsgRegMonitorSampling(void);
            #endif
void SendMsgAtapiData(BYTE *message, BYTE size);
void SendMsgSpecifyXferLength(USHORT size);
        #endif

/* va_list.  Clearly the type __va_list there must keep in step.          */
typedef int *va_list[1];
   /*
    * an array type suitable for holding information needed by the macro va_arg
    * and the function va_end. The called function shall declare a variable
    * (referred to as ap) having type va_list. The variable ap may be passed as
    * an argument to another function.
    * Note: va_list is an array type so that when an object of that type
    * is passed as an argument it gets passed by reference.
    */

#define va_start(ap, parmN) (void)(*(ap) = __va_start(parmN))
   /*
    * The va_start macro shall be executed before any access to the unnamed
    * arguments. The parameter ap points to an object that has type va_list.
    * The va_start macro initialises ap for subsequent use by va_arg and
    * va_end. The parameter parmN is the identifier of the rightmost parameter
    * in the variable parameter list in the function definition (the one just
    * before the '...'). If the parameter parmN is declared with the register
    * storage class an error is given.
    * If parmN is a narrow type (char, short, float) an error is given in
    * strict ANSI mode, or a warning otherwise.
    * Returns: no value.
    */

#define va_arg(ap, type) __va_arg(*(ap), type)
   /*
    * The va_arg macro expands to an expression that has the type and value of
    * the next argument in the call. The parameter ap shall be the same as the
    * va_list ap initialised by va_start. Each invocation of va_arg modifies
    * ap so that successive arguments are returned in turn. The parameter
    * 'type' is a type name such that the type of a pointer to an object that
    * has the specified type can be obtained simply by postfixing a * to
    * 'type'. If type is a narrow type, an error is given in strict ANSI
    * mode, or a warning otherwise. If the type is an array or function type,
    * an error is given.
    * In non-strict ANSI mode, 'type' is allowed to be any expression.
    * Returns: The first invocation of the va_arg macro after that of the
    *          va_start macro returns the value of the argument after that
    *          specified by parmN. Successive invocations return the values of
    *          the remaining arguments in succession.
    *          The result is cast to 'type', even if 'type' is narrow.
    */

#define va_end(ap) ((void)(*(ap) = 0))
   /*
    * The va_end macro facilitates a normal return from the function whose
    * variable argument list was referenced by the expansion of va_start that
    * initialised the va_list ap. If the va_end macro is not invoked before
    * the return, the behaviour is undefined.
    * Returns: no value.
    */

    #endif  // #if (MSG_OUT_MODE == MSG_USBPORT)

#if (MODULE_TEST == 1)
/*********************************************/
/*  DVD Module test parameters               */
/*********************************************/
#define MOD_WRT_LEN     20032   //0x4E40 ECC blocks
#define MOD_WRT_SPEED   e2XDCLV //write speed for mudule test
#define MOD_PIPO_STR    0x100   //offset from write start
#define MOD_PIPO_END    0x1000  //end from PIPO start
#define MOD_PIPO_SPEED  e2XDCLV //PIPO read speed
#define MOD_PI_MAX      200     //

/* LED definition for module test */
#define ModTest_LED_ST          0x50
#define ModTest_LED_END         0x5f
#define ModTest_LED_OFF         (ModTest_LED_ST + LED_OFF)
#define ModTest_LED_ON          (ModTest_LED_ST+ LED_ON)
#define ModTest_STARTUP_ERR     2
#define ModTest_WRT_ERR         3
#define ModTest_PIPO_ERR        4
#define ModTest_SEEK_ERR        5
#define ModTest_TIMEOUT_ERR     6
#define ModTest_PIPO_SP_ERR     7
#define ModTest_SYNC_LOST_ERR   8

#define ModTest_WRT_FOCUS_ERR   13
#define ModTest_PO_FOCUS_ERR    14
#define ModTest_OTHER_ERR  15
#endif  //(MODULE_TEST == 1)

#define BARCODE_OFFSET          BD_BARCODE_START_ADDR
#endif /* _Diag_H_ */
