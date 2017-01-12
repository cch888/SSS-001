/****************************************************************************
*               (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                             All Right Reserved
*
*
* $Revision: 1062 $
* $Date: 11/03/29 1:36p $
*
* DESCRIPTION
*   This file contains Diagnostic commands.
*
* NOTES:
*
**************************** SUNEXT  CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\com_im.h"
#include ".\common\comMath.h"
#include ".\common\system.h"
#include ".\common\scomm.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_read.h"
#include ".\hif\diag.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\gp_util.h"
#include ".\hif\RD_UTIL.h"
#include ".\hif\flash.h"
#include ".\dvd\dvgvars.h"
#include ".\dvd\dvhostrd.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvDiscRd.h"
#include ".\dvd\dvUtil.h"
#include ".\dvd\dvseek.h"
#include ".\dvd\dvRPC.h"
#include ".\dvd\dvRdStru.h"
#include ".\oem\oem_tbl.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\oem\oem_info.h"
#include ".\servo\svo.h"                    /* !include no servo file except svo.h */
#include ".\servo\svo_afe.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svovar.h"
#include ".\servo\svomacro.h"
#include ".\servo\svo_speed.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_step.h"
#include ".\servo\svoPclib.h"
#include ".\servo\svo_seek.h"

#if ((PCB == 673)||(PCB == 690)||(PCB == 830))
#include ".\servo\adjust.h"
#endif // ((PCB == 673)||(PCB == 690)||(PCB == 830))

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_oem_tbl.h"
#endif

#include ".\oem\coef_values.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_cpycmp.h"
#include ".\al\REG_dfe.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\AL\REG_dvd_encoder.h"
#include ".\al\REG_global.h"
#include ".\al\REG_intctrl.h"
#include ".\al\REG_ssi.h"
#include ".\al\REG_write_strategy.h"
#include ".\AL\LAL.h"
#include ".\AL\HAL_dsp.h"
#include ".\AL\HAL_dfe.h"
#include ".\AL\HAL_lasc.h"
#include ".\AL\LAL_stream.h"
#include ".\AL\LAL_shared_sram.h"

#if (ENABLE_HDDVD)
#include ".\hd\hdDiag.h"
#endif // (ENABLE_HDDVD)

#if (EN_WOBBLE == 1)
#include ".\al\HAL_wobble.h"
#include ".\AL\LAL_wobble.h"
#endif // EN_WOBBLE == 1

#if (EN_FW_SIGNATURE == 1)
#include ".\crypt\FWSigCheck.h"
#endif

extern tLaserWriteParametersS  lwp;    /* Write Power */
#if (CHIP_REV <= 0xB2)
extern BYTE UseC1CG1;
#endif
/* c-include files */

/*
 * positioned here because the
 * compiler switches depend on
 * project-files "vu_cfg.h"
 *
 */

    #if (RESET_PLL_FOR_HUGEPIC1==1)
private USHORT      resetmin;
private USHORT      resetsec;
    #endif

    #if (CD_DFE_IMPROVE_READ == 1)
extern  BYTE    DFE_CHG_SEL;
extern  BYTE    DFE_CHG_CASE;
private BYTE    dfe_retry_count = 0;
#define MAX_KPKI_CHG    3
    #endif

#if (HORROR_DISC_IMPROVEMENT == 1)
extern BYTE TryDFE_Param;
#endif

#if (USE_REALTIME_JITTER == 1)
#include <limits.h>  /* for USHRT_MAX value */
#endif /* USE_REALTIME_JITTER */
#if (FW_JITTER_CALC  == 1)
#include <string.h>
#endif  /* FW_JITTER_CALC */


#if (USE_REALTIME_JITTER == 1)
#define FIRST_LEVEL_AVG_SMPLNR 128
#endif /* USE_REALTIME_JITTER */

extern void set_uart_trap(void);

#if (FW_JITTER_CALC  == 1)
#include <string.h>
typedef union
{
    USHORT w;
    struct
    {
        UINT pulsewidth          : 9;
        UINT                     : 4;
        UINT prev_pulse_not_saved: 1;
        UINT glitch_in_pulse     : 1;
        UINT mark                : 1;
    } s;
} RAW_t;


typedef enum
{
   R2T = 2,          /* runlength of 2T */
   R3T,              /* runlength of 3T */
   R4T,              /*       etc...    */
   R5T,
   R6T,
   R7T,
   R8T,
   R9T,
   R10T,
   R11T,
   R12T
} RUNLENGTH_t;
#define NR_OF_RUNLENGTHS 11

typedef enum  {SPACE = 0, MARK = 1} PULSE_t;
#define NR_OF_LEVELS 2

typedef struct
{
   ULONG totalPulsewidth;
   ULONG totalRunlength;
   ULONG dataNumber[NR_OF_LEVELS][NR_OF_RUNLENGTHS];
   ULONG dataSum[NR_OF_LEVELS][NR_OF_RUNLENGTHS];
   ULONG dataSquare[NR_OF_LEVELS][NR_OF_RUNLENGTHS];
   float dataAvg[NR_OF_LEVELS][NR_OF_RUNLENGTHS];
   float dataSigmaSquare[NR_OF_LEVELS][NR_OF_RUNLENGTHS]; /* mind you, 2 + 2*11*6 = 134 Longs !!!? */
} JITTER_t;


/* CD Jitter is measured at a speed of 4X. If one ever decides to change this speed then the following
   runlength limit values should be changed as well !!! */

//#define R2T_UPPER_LIMIT               56
//#define R3T_LOWER_LIMIT               R2T_UPPER_LIMIT + 1
//#define R3T_UPPER_LIMIT               81
//#define R4T_LOWER_LIMIT               R3T_UPPER_LIMIT + 1
//#define R4T_UPPER_LIMIT               104
//#define R5T_LOWER_LIMIT               R4T_UPPER_LIMIT + 1
//#define R5T_UPPER_LIMIT               127
//#define R6T_LOWER_LIMIT               R5T_UPPER_LIMIT + 1
//#define R6T_UPPER_LIMIT               150
//#define R7T_LOWER_LIMIT               R6T_UPPER_LIMIT + 1
//#define R7T_UPPER_LIMIT               173
//#define R8T_LOWER_LIMIT               R7T_UPPER_LIMIT + 1
//#define R8T_UPPER_LIMIT               196
//#define R9T_LOWER_LIMIT               R8T_UPPER_LIMIT + 1
//#define R9T_UPPER_LIMIT               220
//#define R10T_LOWER_LIMIT              R9T_UPPER_LIMIT + 1
//#define R10T_UPPER_LIMIT              243
//#define R11T_LOWER_LIMIT              R10T_UPPER_LIMIT + 1
//#define R11T_UPPER_LIMIT              266
//#define R12T_LOWER_LIMIT              R11T_UPPER_LIMIT + 1

#define CDRW_1T_4X  ((USHORT)((5785 * PLLCO_CLOCK)/1000))   /* 4X speed => 1T count with a scale of 100
                                                               (1T = 57.85nsec) */

#define R2T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 2.5)/100)
#define R3T_LOWER_LIMIT               R2T_UPPER_LIMIT + 1
#define R3T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 3.5)/100)
#define R4T_LOWER_LIMIT               R3T_UPPER_LIMIT + 1
#define R4T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 4.5)/100)
#define R5T_LOWER_LIMIT               R4T_UPPER_LIMIT + 1
#define R5T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 5.5)/100)
#define R6T_LOWER_LIMIT               R5T_UPPER_LIMIT + 1
#define R6T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 6.5)/100)
#define R7T_LOWER_LIMIT               R6T_UPPER_LIMIT + 1
#define R7T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 7.5)/100)
#define R8T_LOWER_LIMIT               R7T_UPPER_LIMIT + 1
#define R8T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 8.5)/100)
#define R9T_LOWER_LIMIT               R8T_UPPER_LIMIT + 1
#define R9T_UPPER_LIMIT               (USHORT)((CDRW_1T_4X * 9.5)/100)
#define R10T_LOWER_LIMIT              R9T_UPPER_LIMIT + 1
#define R10T_UPPER_LIMIT              (USHORT)((CDRW_1T_4X * 10.5)/100)
#define R11T_LOWER_LIMIT              R10T_UPPER_LIMIT + 1
#define R11T_UPPER_LIMIT              (USHORT)((CDRW_1T_4X * 11.5)/100)
#define R12T_LOWER_LIMIT              R11T_UPPER_LIMIT + 1

/* end of runlength limit values */

/* JITTER MEASUREMENT */
#define MEAS_CD_DECODER               0x0000
#define MEAS_CD_ENCODER               0x0001
#define MEAS_DVD_DECODER              0x0002
#define MEAS_DVD_ENCODER              0x0003
#define CLOCKS_PER_T                  32
#define CLOCKS_PER_HALF_T             CLOCKS_PER_T / 2
#define NR_OF_SAMPLES                 3000 / 2                  //in words
#define UPPER_BOUDARY_3T              ((3 * CLOCKS_PER_T) + 3)
#define LOWER_BOUDARY_3T              ((3 * CLOCKS_PER_T) - 3)
#define CHUNK_3K                      0x00000C00/2              //in words
#define CHUNK_4K                      0x00001000/2              //in words
#define SAMPLE_LENGTH_3K              0x00000C00                //in bytes
#define SAMPLE_LENGTH_4K              0x00001000                //in bytes
#define SAMPLE_LENGTH_8K              (2 * SAMPLE_LENGTH_4K)    //in bytes
#define SAMPLE_LENGTH_64K             (16 * SAMPLE_LENGTH_4K)   //in bytes

#define UNREALISTIC_14T_PULSE_LENGTH  380
#define NILL_14T_PULSE                0
#define NILL_PULSE_LENGTH             0
#endif  // FW_JITTER_CALC

#define CD_VENDOR_WRITE_HOLDMODE      0


/****************************************************************************
*                            Variable Definition                            *
****************************************************************************/


BYTE Send_timef;
#if (STORE_MSG_TO_SDRAM == 1)
BYTE    LogStopCondition;
#endif
#if E_DIAGS == 1
BYTE    Error_Rate_Buffer_Size;
BYTE    diaOPC;
BYTE    diaOPCMode;
USHORT  PCAcnt;
BYTE    APCenter;
BYTE    APStep;
BYTE    APMulti;
BYTE    APGammaTarget;
BYTE    APRatio;
USHORT  APWritePower;
SHORT   bOPCArea;
ULONG   MDataAreaAddr;
BYTE    bEnableJitterOPC;

/* Vendor Write Command */
SHORT   AP_BETA_GAMMA;//zephyr move
//WORD    wParamBuff[110];
BYTE    bDynamicBuff[80];
BYTE    bDynamicStgyOn;
BYTE    fgRealWrite;
TimeS   timeLast;
TimeS   StartAddr;
BYTE    bDmSpeedBak;
BOOL    fgVenWSTart;
//BYTE    bBakVWSpeed;
TimeS   venTimes;
BOOL    fgVenWGATEOn;
WORD    WriteSiftCount;
BYTE    cdr_vendor_wpower;

#define T(mk) ((mk==14) ? 9 : mk - 3)              /*mark length to index*/
#define Length(idx) (((idx) < 9) ? (idx) + 3 : 14) /*index to mark length*/

#define Verdify_edges_start_addr   BUFFER_SCRATCH
#define Mark_Data_offset   177
#define EFM_Data_offset    58

#if (SEEPROM==0)
USHORT  diaCalResult[11];
#endif /* #if (SEEPROM==0) */

BYTE DiscLayer;
BOOL ResetDecoder;

#if (REGISTER_MONITOR == 1)
BOOL        fMonitoringReg = FALSE;
RegMonitorT gMonitorReg[16];
BYTE        gTotalMonitorCH;
#endif //REGISTER_MONITOR == 1
#if  SERVOKEYPARAMETER == 1
BYTE    ServoParaSwitch;
BYTE    VendorServoParaSwitch;
extern BYTE CurTblOnDram;
#endif

#if (CEProduct == 1)
USHORT TimeUpForRead;
#endif

/****************************************************************************
*                            Function Definition                            *
****************************************************************************/
#if (SEEPROM==0)
extern void FlashWriteData(ULONG address, BYTE data);
extern void FlashEraseSector(ULONG address, ULONG size);
#endif
extern void ExitProcess(void);
extern void SetupHostTransferRegisters(void);
extern void plrServoSetSpeed(BYTE speed, BYTE NextPhase);
extern void plrServoSeek(long SeekTimeLBA, eSeekMode SeekMode, BYTE NextPhase);

#if (USE_REALTIME_JITTER == 1)
static void hifDiaGenericMeasureCmd(void);
static void hifDiaGenericJitterCmd(void);
static void hifDiaGenericContinuousJitterCmd(void);
static void hifDiaGenericTimedJitterCmd(void);
static void hifDiaGenericAddressedJitterCmd(void);
#endif //USE_REALTIME_JITTER

#if (EN_DRIVE_CERT == 1)
extern void GenerateDriveCert(ULONG Adrs);
extern void ReturnDriveCert(ULONG Adrs);
#endif /* EN_DRIVE_CERT == 1 */

#if (ENABLE_AACS == 1)
extern void CheckAacsKeyExistVdlid(BYTE *DataPtr);
extern void CheckHWKeyBank(BYTE *DataPtr);
#endif


#define LASC_INTS               HAL_SRV_DELTA2IEN | HAL_SRV_DELTA1IEN | \
                                     HAL_SRV_DELTA0IEN|HAL_SRV_THRESHIEN
static USHORT lcthminmax;
#if (SUN_FUNC_TEST == 1)
BYTE READ_C1_C2;
#endif

void diaPresetPower(void)
{
    lcthminmax = 0x1FFF;
    WRITE_REG(HAL_SRVREG_MAP->PDMCNT, 0x03800000);
    WRITE_REG(HAL_SRVREG_MAP->LCINTEN, 0x00); //Disable interrupts
    WRITE_FIELD(HAL_SRV_LCTHMIN, lcthminmax);
    WRITE_FIELD(HAL_SRV_LCTHMAX, lcthminmax);
#if (PCB == 673)
    WRITE_FIELD(HAL_PERIPH_PC1DATA, 0);         //DVD laser
#elif (PCB == 830)
    svoVar_discKindT.Bit.DiscStd = eDiscDVD;
    LAL_StoreParams(svoVar_discKindT, svoIramVar.svoCurrentSpeed);
    HAL_LASC_SetReadMode();
#endif
    WRITE_FIELD(HAL_SRV_LDON, 1);     /* set LD to ON  */
    //send_msg54(1,'L','A',BHIGH(lcthminmax), lcthminmax);
    SendMsgCnB(1,2,0x800800,B2B(lcthminmax));
//    send_msg54(1,'L','A',BHIGH(lcthminmax), lcthminmax);
//    while (1) {}
}

// TBD!
void diaIncrementPower(void)
{

    lcthminmax += 0x100;
//    WRITE_REG(HAL_SRVREG_MAP->PDMCNT, 0x03800000);
//    WRITE_REG(HAL_SRVREG_MAP->LCINTEN, 0x00); //Disable interrupts
    if (lcthminmax <= 0x7fff)
    {
        WRITE_FIELD(HAL_SRV_LCTHMAX, lcthminmax);
        WRITE_FIELD(HAL_SRV_LCTHMIN,lcthminmax);
    }
//    WRITE_FIELD(HAL_PERIPH_PC1DATA, 0);         //DVD laser
//    WRITE_FIELD(HAL_SRV_LDON, 1);     /* set LD to ON  */
    //send_msg54(1,'L','A',BHIGH(lcthminmax), lcthminmax);
    SendMsgCnB(1,2,0x800803,B2B(lcthminmax));
//    while (1) {}

// TBD!
}
void diaDecrementPower(void)
{

    lcthminmax -= 0x40;
//    WRITE_REG(HAL_SRVREG_MAP->PDMCNT, 0x03800000);
//    WRITE_REG(HAL_SRVREG_MAP->LCINTEN, 0x00); //Disable interrupts
    if (lcthminmax >= 0x1fff)
    {
        WRITE_FIELD(HAL_SRV_LCTHMAX, lcthminmax);
        WRITE_FIELD(HAL_SRV_LCTHMIN,lcthminmax);
    }
//    WRITE_FIELD(HAL_PERIPH_PC1DATA, 0);         //DVD laser
//    WRITE_FIELD(HAL_SRV_LDON, 1);     /* set LD to ON  */
    //send_msg54(1,'L','A',BHIGH(lcthminmax), lcthminmax);
    SendMsgCnB(1,2,0x800804,B2B(lcthminmax));
//    while (1) {}
}
#if (DEBUG_LASER_OPEN_LOOP == 1)||(DEBUG_LASER_CLOSE_LOOP == 1)
// TBD!
void diaSavePower()
{
    OpenWritableSegmentTable(ePersistent_Talbe);
    SetPersistentTable2B(DV_OPEN_LOOP_THRESHOLD, lcthminmax );
    SetPersistentTable2B(DV_OPEN_LOOP_THRESHOLD+2, ~lcthminmax );
    CloseWritableSegmentTable(ePersistent_Talbe);
    //send_msg54(1,'L','A',BHIGH(lcthminmax), lcthminmax);
    SendMsgCnB(1,2,0x800801,B2B(lcthminmax));
//    while (1) {}
}

void diaReadPower()
{
    lcthminmax = GetPersistentTable2B(DV_OPEN_LOOP_THRESHOLD);
    if (lcthminmax == ~GetPersistentTable2B(DV_OPEN_LOOP_THRESHOLD+2) )
    {
        //send_msg54(1,'L','A',BHIGH(lcthminmax), lcthminmax);
        SendMsgCnB(1,2,0x800802,B2B(lcthminmax));
    }
    else
    {
        send_msg80(1, 0x710131);
    }
//    while (1) {}
}
#endif

typedef void (*pFunc)(void);
#define RESET_HANDLER_ADDR          0x30000000    /* ResetHandler     0x30000010   ARM Code  0  CSTART.o(Cstart) */
#define Jump_To_Reset_Handler       ((pFunc)(BYTE *)(RESET_HANDLER_ADDR))
/*************************************************************************
* diaCmdEnable() (F0)
*
* Enable Diagnostic commands
*
* Returns:  void
* Params:   none
*************************************************************************/
void diaCmdEnable(void)
{
    /* Check Signature String */
    if  ((Iram_Cmd_block.bf[2] == 'S')
     && (Iram_Cmd_block.bf[3] == 'U')
     && (Iram_Cmd_block.bf[4] == 'N')
     && (Iram_Cmd_block.bf[5] == 'D')
     && (Iram_Cmd_block.bf[6] == 'I')
     && (Iram_Cmd_block.bf[7] == 'A')
     && (Iram_Cmd_block.bf[8] == 'G')
     && (Iram_Cmd_block.bf[9] == 'S'))
    {
        /* Check Reset Device Bit */
        if (Iram_Cmd_block.bf[1] & VENDOR_RESET_DEVICE_COMMAND)
        {
            ServoStop();
        #if (EN_POWER_CONTROL == 1)
            PowerControl(eStandbyPower);
        #else
            ataPowerMode = STANDBY_MODE;
        #endif
#if (FLASH_BY_DISC == 1)
            SaveCurrentTransferSettings(0);
#else
            SaveCurrentTransferSettings();
#endif
            AutoCommandComplete();
            Jump_To_Reset_Handler();
            return;
        }
        else
        {
            /* Check Enable/Disable Bit */
            if (Iram_Cmd_block.bf[1] & ENABLE_DIAG_COMMAND)
            {
                St_diag_ena = TRUE;         /* Diagnostics Enabled */
            }
            else
            {
                St_diag_ena = FALSE;        /* Diagnostics Disabled */
            }
        }
    }
    else
    {
        BUILD_SENSE(0x05,0x24,0x00,0x51);    /* Invalid Field in CDB */
    }
    AutoCommandComplete();
}
/*************************************************************************
* diaDebugLogCtrl() (FE)
*
* Flash/DRAM Debugging log control commands
*
* Returns:  void
* Params:   none
*************************************************************************/
void diaDebugLogCtrl(void)
{
    LoaderMonitoringLog();
    AutoCommandComplete();
}
/*************************************************************************
  FUNCTION  LoaderMonitoringLog()

  DESCRIPTION 1.B/E can do loader's logging action enable
                        2.B/E can do loader's logging action disable
                        3.B/E sends vendor command to receive log data that is stored in loader

*************************************************************************/
void LoaderMonitoringLog(void)
{
#if (STORE_MSG_TO_SDRAM == 1)
    ULONG   start_addr;
    BYTE    ConLog;
    FLAG    Success;
    ULONG   msgsize,index;

#endif
    BYTE    OperationType;
    StLongU hifmsgmarks,svomsgmarks;
    StLongU Xfer_Len;

    OperationType = Iram_Cmd_block.bf[1] & 0xFF;
    Xfer_Len.byte.u = Iram_Cmd_block.bf[6];
    Xfer_Len.byte.h = Iram_Cmd_block.bf[7];
    Xfer_Len.byte.m = Iram_Cmd_block.bf[8];
    Xfer_Len.byte.l = Iram_Cmd_block.bf[9];

    switch (OperationType)
    {
#if (STORE_MSG_TO_SDRAM == 1)
        case ConLoadLogging:
        {
            ConLog = Iram_Cmd_block.bf[4] & 0xFF;
            if(ConLog == EnaLog)
            {
                EnableLogging = TRUE;
                Success = TRUE;
                LogStopCondition = Iram_Cmd_block.bf[5];
            }
            else if(ConLog == DisLog)
            {
                EnableLogging = FALSE;
                Success = TRUE;
            }
            else
            {
                Success = FALSE;
            }
            if(Success == FALSE)
                BUILD_SENSE(0x05,0x24,0x00,0x55);              /* INVALID PARAMETER IN CDB */
            break;
        }
        case ResetLogging:
        {
            LinearPatternFill((USHORT)(LOGGING_MSG_SPAGE),0,LOGGING_MSG_PAGE_SIZE,0x800/2,0); //clear
            Iram_Store_Msg_dram_addr = LOGGING_MSG_START_ADDR;
            MsgStAddr = LOGGING_MSG_START_ADDR;
            MsgEdAddr   = LOGGING_MSG_END_ADDR;
            LoggingFull = FALSE;
            break;
        }
        case GetDRAMLogStatus:
        {
            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            //header
            WriteSDRAM((Xfer_Len.Long - 4)>>8);
            WriteSDRAM((Xfer_Len.Long - 4));
            WriteSDRAM(0);
            WriteSDRAM(0);
            //transfer data
            WriteSDRAM(MsgStAddr>>24);
            WriteSDRAM(MsgStAddr>>16);
            WriteSDRAM(MsgStAddr>>8);
            WriteSDRAM(MsgStAddr);
            msgsize = LOGGING_MSG_PAGE_SIZE*DRAM_PAGE_SIZE;
            WriteSDRAM(msgsize>>24);
            WriteSDRAM(msgsize>>16);
            WriteSDRAM(msgsize>>8);
            WriteSDRAM(msgsize);
            index = Iram_Store_Msg_dram_addr - LOGGING_MSG_START_ADDR;
            WriteSDRAM(index>>24);
            WriteSDRAM(index>>16);
            WriteSDRAM(index>>8);
            WriteSDRAM(index);

            WriteSDRAM((EnableLogging<<7)|(LoggingFull<<6));

            WriteSDRAM(LogStopCondition);
            WriteSDRAM(0x00);
            WriteSDRAM(0x00);
            SendParameterDataToHost(BUFFER_SCRATCH, Xfer_Len.Long, FALSE);
            break;
        }
    #if (STORE_MSG_TO_FLASHROM == 1)
        case GetROMLogStatus:
            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            //header
            WriteSDRAM((Xfer_Len.Long - 4)>>8);
            WriteSDRAM((Xfer_Len.Long - 4));
            WriteSDRAM(0);
            WriteSDRAM(0);
            //transfer data
            start_addr = FLASH_BASE + DEBUG_MSG_OFFSET;
            WriteSDRAM(start_addr>>24);
            WriteSDRAM(start_addr>>16);
            WriteSDRAM(start_addr>>8);
            WriteSDRAM(start_addr);
            msgsize = DEBUG_MSG_SIZE;
            WriteSDRAM(msgsize>>24);
            WriteSDRAM(msgsize>>16);
            WriteSDRAM(msgsize>>8);
            WriteSDRAM(msgsize);

            SendParameterDataToHost(BUFFER_SCRATCH, Xfer_Len.Long, FALSE);
            break;
        case SaveLogToRom:
        {
            CopyMsgFromDataAreaToROM();
            break;
        }
    #endif
#endif // (STORE_MSG_TO_SDRAM == 1)
        case GetMsgMarks:
            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            //header
            WriteSDRAM((Xfer_Len.Long - 4)>>8);
            WriteSDRAM((Xfer_Len.Long - 4));
            WriteSDRAM(0);
            WriteSDRAM(0);
            //transfer data
            WriteSDRAM(hifDebugMsgFlag>>24);
            WriteSDRAM(hifDebugMsgFlag>>16);
            WriteSDRAM(hifDebugMsgFlag>>8);
            WriteSDRAM(hifDebugMsgFlag);
            WriteSDRAM(svoDebugMsgFlag>>24);
            WriteSDRAM(svoDebugMsgFlag>>16);
            WriteSDRAM(svoDebugMsgFlag>>8);
            WriteSDRAM(svoDebugMsgFlag);

            SendParameterDataToHost(BUFFER_SCRATCH, Xfer_Len.Long, FALSE);
            break;
        case SetMsgMarks:
            Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
            GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);
            Iram_Buf_dram_addr = BUFFER_SCRATCH+4;//shift header

            hifmsgmarks.byte.u = ReadSDRAM();
            hifmsgmarks.byte.h = ReadSDRAM();
            hifmsgmarks.byte.m = ReadSDRAM();
            hifmsgmarks.byte.l = ReadSDRAM();
            svomsgmarks.byte.u = ReadSDRAM();
            svomsgmarks.byte.h = ReadSDRAM();
            svomsgmarks.byte.m = ReadSDRAM();
            svomsgmarks.byte.l = ReadSDRAM();

            set_hif_msg(hifmsgmarks.Long);
            set_svo_msg(svomsgmarks.Long);

            if(Iram_Cmd_block.bf[4] == 1)//save message marks to persistent
            {
                OpenWritableSegmentTable(ePersistent_Talbe);
#if (SUN_FUNC_TEST == 0)                
                SetPersistentTable1B(HIF_MSG_MARKS_ADDR,hifmsgmarks.byte.u);
                SetPersistentTable1B(HIF_MSG_MARKS_ADDR+1,hifmsgmarks.byte.h);
                SetPersistentTable1B(HIF_MSG_MARKS_ADDR+2,hifmsgmarks.byte.m);
                SetPersistentTable1B(HIF_MSG_MARKS_ADDR+3,hifmsgmarks.byte.l);
                SetPersistentTable1B(SVO_MSG_MARKS_ADDR,svomsgmarks.byte.u);
                SetPersistentTable1B(SVO_MSG_MARKS_ADDR+1,svomsgmarks.byte.h);
                SetPersistentTable1B(SVO_MSG_MARKS_ADDR+2,svomsgmarks.byte.m);
                SetPersistentTable1B(SVO_MSG_MARKS_ADDR+3,svomsgmarks.byte.l);
#else
                SetEEPROMTable1B(OEM_AGING_HIF_MSG_MARKS_ADDR, hifmsgmarks.byte.u);
                SetEEPROMTable1B(OEM_AGING_HIF_MSG_MARKS_ADDR+1, hifmsgmarks.byte.h);
                SetEEPROMTable1B(OEM_AGING_HIF_MSG_MARKS_ADDR+2, hifmsgmarks.byte.m);
                SetEEPROMTable1B(OEM_AGING_HIF_MSG_MARKS_ADDR+3, hifmsgmarks.byte.l);
                SetEEPROMTable1B(OEM_AGING_SVO_MSG_MARKS_ADDR, svomsgmarks.byte.u);
                SetEEPROMTable1B(OEM_AGING_SVO_MSG_MARKS_ADDR+1, svomsgmarks.byte.h);
                SetEEPROMTable1B(OEM_AGING_SVO_MSG_MARKS_ADDR+2, svomsgmarks.byte.m);
                SetEEPROMTable1B(OEM_AGING_SVO_MSG_MARKS_ADDR+3, svomsgmarks.byte.l);
#endif
                CloseWritableSegmentTable(ePersistent_Talbe);
            }
            break;
        default:
        {
            BUILD_SENSE(0x05,0x20,0x00,0x02);    /* INVALID COMMAND OPERATION CODE */
            return;
        }
    }
}

#if (SUN_FUNC_TEST == 1)
/*************************************************************************
*diaLaserMeasurment() (EE)
*
*Laser Measurment
*
*Returns: void
*Params: none
*************************************************************************/
void diaLaserMeasurment(void)
{
    BYTE OpType, DkType;
    discKindT tempDiscKind;
    BYTE i,Cmd_Block;

    OpType = Iram_Cmd_block.bf[1] & 0xFF;
    DkType = Iram_Cmd_block.bf[2] & 0xFF;

    /* Check the command is vendor or not */
    Cmd_Block = 0x00;
    for (i=3;i<9;i++)
    {
        Cmd_Block |= Iram_Cmd_block.bf[i];
    }
    
    if (Cmd_Block > 0)
    {
        BUILD_SENSE(0x05,0x20,0x00,0x06);
        AutoCommandComplete();
        return;
    }

    if (OpType == 1)
    {
        LAL_InitServo();
        switch (DkType)
        {
            case 0x00:
                WRITE_DSP_COEF(Pread0RS,0x00002600);
                WRITE_DSP_COEF(Pread1RS,0x00002600);

                tempDiscKind.Bit.DiscStd = eDiscCD;  //Force CD Laser
                tempDiscKind.Bit.Layers = eLayersSingle;
                tempDiscKind.Bit.WblType= eWblUnknown;
                tempDiscKind.Bit.Media = eMediaROM;
                LAL_StoreParams( LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_CD);
                break;

            case 0x01:
                WRITE_DSP_COEF(Pread0RS,0x00002900);
                WRITE_DSP_COEF(Pread1RS,0x00002900);

                tempDiscKind.Bit.DiscStd = eDiscDVD;
                tempDiscKind.Bit.Layers = eLayersSingle;
                tempDiscKind.Bit.WblType= eWblUnknown;
                tempDiscKind.Bit.Media = eMediaROM;

                LAL_StoreParams( LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_DVD);
                break;

            case 0x02:
                WRITE_DSP_COEF(Pread0RS,0x00001B00);
                WRITE_DSP_COEF(Pread1RS,0x00001B00);

                tempDiscKind.Bit.DiscStd = eDiscBD;
                tempDiscKind.Bit.Layers = eLayersSingle;
                tempDiscKind.Bit.WblType= eWblUnknown;
                tempDiscKind.Bit.Media = eMediaROM;

                LAL_StoreParams( LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_BD);
                break;

            default:
                BUILD_SENSE(0x05,0x20,0x00,0x04);    /* INVALID COMMAND OPERATION CODE */
                break;
        }

        if (Iram_Sense.key == 0)/* No error occurred! */
        {
        LAL_SetReadMode();
        HAL_LASC_SetOscen(OSCEN_ON);
        LAL_LASC_LaserOffsetAdj();
        WRITE_FIELD(HAL_SRV_LDON, 1);     /* set LD to ON  */
        HAL_LASC_SetLaserOn(TRUE);
    }
    }
    else if (OpType == 0)
    {
        WRITE_FIELD(HAL_SRV_LDOFF, 0);     /* set LD to OFF  */
        svoSetLDOff();
    }
    else
    {
        BUILD_SENSE(0x05,0x20,0x00,0x05);    /* INVALID COMMAND OPERATION CODE */
    }

    AutoCommandComplete();
    return;
}
#endif

/*************************************************************************
* Free Command (F1)
*
* Disable Diagnostic commands
*
* Returns:  void
* Params:   none
*************************************************************************/
void diaCmdFree(void)
{
    AutoCommandComplete();
}

/*************************************************************************
* diaCmdMRW() (F2)
*
* Mt. Rainier Test Suites
*
* Returns:  void
* Params:   none
*************************************************************************/
void diaCmdMRW(void)
{
    AutoCommandComplete();
}
#if 0 //disable diaHifBufTransSetup()
/************************************************************************
* diaHifBufTransSetup()
*
* Description: Diagnostics commands Host Transfer Setup
*
* Returns:  PASS/FAIL
* Params:   Dev2HostSel, PageModeSel, StartAddr, TotalPages, PageLength
*************************************************************************/
BOOL diaHifBufTransSetup(BOOL Dev2HostSel, BOOL PageModeSel, ULONG StartAddr, USHORT TotalPages,USHORT PageLength)
{
    BYTE bda;
    ULONG TotalTransBytes;

    /*setup page characters*/
    bda= READ_REG(HAL_HSTREG_MAP->HDDIR) & HAL_HST_DMAMODE; //Read the current trans mode

    if(bda == halHST_DMAMODE_PIO)
      {
       TotalTransBytes=PageLength*TotalPages;
       if(TotalTransBytes>0xfffe)
          return(FAIL);
       else
            WRITE_REG(HAL_HSTREG_MAP->ATBLO, TotalTransBytes);  /* BYTE count reg */
      }

    if(Dev2HostSel==TRUE)
    {
        WRITE_FIELD(HAL_HST_HXDIR, 0);/* Transfer Dir: device -> host */
    }
    else
    {
        WRITE_FIELD(HAL_HST_HXDIR, 1);/* Transfer Dir: host -> device */
    }

    if(PageModeSel==TRUE)
        WRITE_FIELD(HAL_HST_HBODIS, 0); /* Select page/offset addr */
    else
        WRITE_FIELD(HAL_HST_HBODIS, 1); /* Select linear addr */


    /*setup for regsiters for transferring*/

    WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, TotalPages);/* Total_Pages contain the total number of pages need to transfer */
    WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, TotalPages);/* all the pages avaible at the begin */

    if (PageModeSel==TRUE)
    {
        WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, StartAddr);/* start page number*/
        WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, 0x0000); /* page offset=0*/
    }
    else
    {
        WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, StartAddr); /* start address of linear mode */
    }

    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, (PageLength/2)); /* Total number of words per page */
    WRITE_REG(HAL_HSTREG_MAP->HSTSUBC, 0x00);/* Clear subcode xfer */

#if (CHIP_REV >= 0xC0)
    WRITE_FIELD(HAL_HST_RESWDCL,0x00);
    WRITE_FIELD(HAL_HST_RESWDCH,0x00);
    WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif
    St_diag_HIFTransData = TRUE;                /* Buffer Data transfer is on*/

    WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_ADRTRG);/* automatic  START_XFER */

    /*wait for transfer done*/
    while (1)
    {                               /* wait loop for data xfer done */
        if (St_diag_HIFTransData == FALSE)
        {
            break;
        }
        ExitProcess();
    }                                           /* If eject occurred just finish this command as normal*/

    return(PASS);
}
#endif //disable diaHifBufTransSetup()

/* ----------------------------Jitter functions------END-----------------*/
BYTE Addr_mode;
ULONG Last_PSN;
BYTE SkipErrRateNum;
#if (DVD_RAM_READ == 1)
ULONG Last_DVD_PSN;
ULONG ANEW_SEEK_Target;
#endif
/*************************************************************************
* diaVendorRead() (F4)
*
* Vendor Read Commands
*
* Returns:  void
* Params:   none
*************************************************************************/
void diaVendorRead(void)
{
    BYTE    pre_DriverStste;

    if((St_diag_ena == FALSE)              /* Check the Diag Enter flag */
#if  SERVOKEYPARAMETER == 1
        && (Iram_Cmd_block.bf[1]!= VR_ServoKeyPara)
#endif
        )
    {
        BUILD_SENSE(0x05,0x20,0x00,0x49);    /* INVALID COMMAND OPERATION CODE */
        AutoCommandComplete();
        return;
    }

    switch (Iram_Cmd_block.bf[1] & 0x0F)
    {

        case VR_SetSpeedSeek:
                diaVendorSetSpeedSeek();
                AutoCommandComplete();
                break;
        case VR_BLER_CMD:
                diaBLERCommand();
                break;
//        case VR_Servo_Adjust:
//                diaServoAdjustCommand();
//                break;
        case VR_Flash_Checksum:
                diaCalFlashCheckSumCommand();
                break;

#if (USE_REALTIME_JITTER == 1)
        case VR_GENERIC_MEASURE_CMD:
            /* start new vendor unique command !!!!!!!!!   */
            hifDiaGenericMeasureCmd();
            break;
#endif //USE_REALTIME_JITTER

        case VR_ServoDSPDebug:
            if (Iram_Cmd_block.bf[2] & bit1)            //Check if force servo DSP debug to be the only client of SSI
            {
                HAL_DSP_SetDebugForce(ON);
            }
            else
            {
                HAL_DSP_SetDebugForce(OFF);
            }

            if (Iram_Cmd_block.bf[2] & bit0)            //Check if enable servo DSP debug channel
            {
                HAL_DSP_SetDebugMode(ON);
                HAL_DSP_SetDebug(DEBUG_CH0, (Iram_Cmd_block.bf[4] & bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, Iram_Cmd_block.bf[4]&0x7f, 0x08);
                HAL_DSP_SetDebug(DEBUG_CH1, (Iram_Cmd_block.bf[5] & bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, Iram_Cmd_block.bf[5]&0x7f, 0x08);
                HAL_DSP_SetDebug(DEBUG_CH2, (Iram_Cmd_block.bf[6] & bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, Iram_Cmd_block.bf[6]&0x7f, 0x08);
                HAL_DSP_SetDebug(DEBUG_CH3, (Iram_Cmd_block.bf[7] & bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, Iram_Cmd_block.bf[7]&0x7f, 0x08);
            }
            else
            {
                HAL_DSP_SetDebugMode(OFF);
            }

            AutoCommandComplete();
            break;

      case VR_SledMoving:
            pre_DriverStste = svo_Var_motorDriverState;
            SetMotorDriverState(SHORT_BRAKE);
            if (Iram_Cmd_block.bf[2] & bit0)
            {//offset of sled moving
                SHORT offset_cnt;
                offset_cnt = (Iram_Cmd_block.bf[6] << 8) | Iram_Cmd_block.bf[7];
                svoStp_JumpToPosition(STEP_NORMAL_MODE, offset_cnt);
                while (svoStp_busy())
                {
                    ExitProcess();
                }
                svoStp_StopControl();
                DelaymS(50);
            }
            else
            {//sled home
                svoStp_Homing();
                while (svoStp_busy())
                {
                    ExitProcess();
                }
                svoStp_StopControl();
            }
            SetMotorDriverState(pre_DriverStste);
            AutoCommandComplete();
            break;
#if  SERVOKEYPARAMETER == 1
        case VR_ServoKeyPara:
            diaServoKeyPara();
            AutoCommandComplete();
            break;
#endif
#if(BCA_ENABLE == 1)
        case VR_ReadBCA:
            diaReadBCA();
            break;
#endif
        default:
            BUILD_SENSE(0x05,0x20,0x00,0x57);        /* INVALID COMMAND OPERATION CODE */
            AutoCommandComplete();
            return;
    }
}

void diaCalFlashCheckSumCommand(void)
{
    ULONG   Index;
    vBYTE  *flash_addr;
    USHORT chksum_DRAM;

    flash_addr = (vBYTE *)FLASH_BASE;

    chksum_DRAM = 0;

#if (PCB != 710) && (PCB != 711) && (PCB != 815) && (PCB != 817)
    for (Index = 0; Index < CHECK_SUM_PROTECT_START; Index++)
    {
        chksum_DRAM += (USHORT)flash_addr[Index];     // Calculate checksum for area whrere Persistent Table resides.
        if((Index%0x100)==0)
        {
            ExitProcess();
        }
    }
#endif

    for (Index = (CHECK_SUM_PROTECT_END + 1); Index < (FLASH_ROM_SIZE - 2); Index++)
    {
        chksum_DRAM += (USHORT)flash_addr[Index];     // Calculate area from top of Persistent Table to top of Flash
        if((Index%0x100)==0)
            ExitProcess();
    }

    chksum_DRAM = chksum_DRAM + (USHORT)(flash_addr[FLASH_ROM_SIZE-2] << 8) + (USHORT)(flash_addr[FLASH_ROM_SIZE-1]);

    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    WriteSDRAM(chksum_DRAM>>8);
    WriteSDRAM(chksum_DRAM);
    /* set up parameters and xfer mechenism status data to host */
    SendParameterDataToHost(BUFFER_SCRATCH,2 , TRUE);
}
#if(PCB == 710)||(PCB == 711)||(PCB == 817)

BOOL diaMainCodeSelfCheck(void)
{
    ULONG count;
    USHORT SelfCheck;

    SelfCheck = 0;
    for (count = (CHECK_SUM_PROTECT_END + 1)/2; count < (FLASH_ROM_SIZE - 288)/2; count ++ )   // Calc checksum for area in which ARM Vectors etc. reside.
        SelfCheck += (USHORT)SCSDRAM16[count];

    count = (FLASH_ROM_SIZE - 304)/2;
    if((SelfCheck != 0x00)&&(((USHORT)SCSDRAM16[count]) != 0x0000))
    {
        send_msg5SValue(1,"SELF_CHECK", 1, SelfCheck);
        return FAIL;
    }
    return PASS;
}

#endif


void diaVendorSetSpeedSeek(void)
{
    BYTE    seek_retry_count;
#if (SEQ_SEEK_SPEED_DOWN == 1)
        RandomRead = FALSE;
        RandomCount = 0;
#endif
#if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC)
    {
        diaBDVendorSetSpeedSeek();
    }
    else
#endif
#if (ENABLE_HDDVD)
    if (pdbDiscLoaded() == HD_DISC)
    {
        diaHDVendorSetSpeedSeek();
        return;
    }
    else
#endif
    if (pdbDiscLoaded() == DVD_DISC)
    {
        BYTE    tempByte;
        ULONG   BlockRequest;
        PSNs    StartPSN;
        DiscAccessStatusE   DiscReadStatus;
#if (HORROR_DISC_IMPROVEMENT == 1)
        BYTE i;
#endif

        PI_UN_C = 0;
        PO_UN_C = 0;
        DiscLayer = 0;
        ResetDecoder = FALSE;

#if (HORROR_DISC_IMPROVEMENT == 1)
        if ((read_retry_option&READ_RETRY_ENABLE)==READ_RETRY_ENABLE)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x310560); //DiagResetSmartRetry
            if (((read_retry_option&SERVO_DEFECT_RETRY_ENABLE)==SERVO_DEFECT_RETRY_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                Defect_Enable(DEFECT_SOURCE_UNFREEZE);
            }
    #if (DROOP_RATE_DECREASE == 1)
            if (((read_retry_option&HF_DEFECT_RETRY_ENABLE)==HF_DEFECT_RETRY_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                LAL_WriteCoef(DFE_HFDETCFG1_DECFBWPA);
                LAL_WriteCoef(DFE_HFDETCFG2_DECFBWIA);
            }
    #endif

            if (((read_retry_option&DPDGN_RETRY_ENABLE)==DPDGN_RETRY_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                WRITE_FIELD(HAL_AFE_DPDGN,svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain);
    #if (CHIP_REV >= 0xC0)
                //update DPDGN1~4
                READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
                READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
                READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
    #endif
            }

            if (fgNormalValueSaved == TRUE)
            {
                for (i=0;i<Smart_Retry_Para_NO;i++)
                {
                    if (Smart_Retry_Info[i].reg_length == 4)
                    {
                        READ_MOD_WRITE(*(ULONG *)(Smart_Retry_Info[i].addr),
                                                 (Smart_Retry_Info[i].mask),
                                                 (SmartRtyNormalValue[i] << (Smart_Retry_Info[i].start_bit)));
                    }
                    else
                    {
                        READ_MOD_WRITE(*(BYTE *)(Smart_Retry_Info[i].addr),
                                                (Smart_Retry_Info[i].mask),
                                                (SmartRtyNormalValue[i] << (Smart_Retry_Info[i].start_bit)));
                    }
                }
                SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF5F,B1B(SmartRtyNormalValue[0]),B1B(SmartRtyNormalValue[1]),B1B(SmartRtyNormalValue[2]),B1B(SmartRtyNormalValue[3]),
                                    B1B(SmartRtyNormalValue[4]),B1B(SmartRtyNormalValue[5]),B1B(SmartRtyNormalValue[6]),B1B(SmartRtyNormalValue[7]));
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF75,B1B(SmartRtyNormalValue[8]),B1B(SmartRtyNormalValue[9]),B1B(SmartRtyNormalValue[10]),B1B(SmartRtyNormalValue[11]));
            }

    #if (EN_ADEQ == 1)
            if (((read_retry_option&ADEQ_RETRY_ENABLE)==ADEQ_RETRY_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
    #endif
            {
                InitAdaptEq();
            }

            if (((read_retry_option&FAST_PLL_RETRY_SPECIAL_ENABLE)==FAST_PLL_RETRY_SPECIAL_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                TryDFE_Param=TryDFE_Param&0xFE;
                LAL_WriteCoef(DFE_HFFPLLCFG_FPLLKPCFG);
                LAL_WriteCoef(DFE_HFFPLLCFG_FPLLKICFG);
            }

            if (((read_retry_option&TESPT_RETRY_ENABLE)==TESPT_RETRY_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                calSetMisc(0, TE_SPT, 0); /* write tracking Offset Data data */
                calSetMisc(1, TE_SPT, 0); /* write tracking Offset Data data */
                apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
            }

            if (((read_retry_option&TE_DPD_RETRY_ENABLE)==TE_DPD_RETRY_ENABLE)
                || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                {
                    read_retry_option &= TE_DPD_RETRY_DISABLE;
                    read_retry_status &= TE_DPD_RETRY_DISABLE;
                    SetEqRegsForASpeed(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                }
            }

            read_retry_option = READ_RETRY_ALL_OFF;
            read_retry_status = READ_RETRY_ALL_OFF;
            read_retry_restartTime= 0;
            read_retry = 0;
            dfe_retry = 0;
            FEBC_retry = 0;
            Tilt_retry = 0;
            BufferErrorForRetry = 0;
            WRITE_FIELD(HAL_DMOD_CDVCOLKSP, halDMOD_SlowConfOutLock);
        }
#endif//#if (HORROR_DISC_IMPROVEMENT == 1)

        WRITE_FIELD(HAL_DVDD_STARTBK,DVD_START_BLOCK);

        ClearDataBuffer();
        if (Iram_Cmd_block.bf[8] != 0x00)
        {
            SkipErrRateNum = 1;

            if ( (Iram_Cmd_block.bf[2] & 0x78) == 0x00)//use old definition
            {
                switch(Iram_Cmd_block.bf[8])
                {
                    case 1:
                        tempByte = e1XDCLV; break;
                    case 2:
                        tempByte = e2XDCLV; break;
                    case 3:
                        tempByte = e3XDCLV; break;
                    case 4:
                        tempByte = e4XDCLV; break;
                    case 5:
#if (ENABLE_DVD_4XCAV == 1)
                        tempByte = e4XDCAV; break;
#else
                        tempByte = e5XDCLV; break;
#endif
                    case 6:
                        tempByte = e6XDPCAV; break;
                    case 7:
                        tempByte = e6XDCAV; break;
                    case 8:
                        tempByte = e8XDPCAV; break;
                    case 9:
                        tempByte = e8XDCAV; break;
                    case 10:
                        tempByte = e10XDCAV; break;
                    case 12:
                        tempByte = e12XDCAV; break;
                    case 13:
                        tempByte = e12XDPCAV; break;
                    case 16:
                        tempByte = e16XDCAV; break;
                    case 20:
                        tempByte = e20XDCAV; break;

                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x52);                    /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x40)//PCAV mode
            {
                switch(Iram_Cmd_block.bf[8])
                {
                    case 1:
                        tempByte = e1XDPCAV; break;
                    case 2:
                        tempByte = e2XDPCAV; break;
                    case 3:
                        tempByte = e3XDPCAV; break;
                    case 4:
                        tempByte = e4XDPCAV; break;
                    case 5:
                        tempByte = e5XDPCAV; break;
                    case 6:
                        tempByte = e6XDPCAV; break;
                    case 8:
                        tempByte = e8XDPCAV; break;
                    case 12:
                        tempByte = e12XDPCAV; break;
                    case 16:
                        tempByte = e16XDPCAV; break;
                    case 20:
                        tempByte = e20XDPCAV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x30);                    /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x20)//ZCAV mode
            {
                switch(Iram_Cmd_block.bf[8])
                {
                    case 1:
                        tempByte = e1XDZCLV; break;
                    case 2:
                        tempByte = e2XDZCLV; break;
                    case 3:
                        tempByte = e3XDZCLV; break;
                    case 4:
                        tempByte = e4XDZCLV; break;
                    case 5:
                        tempByte = e5XDZCLV; break;
                    case 6:
                        tempByte = e6XDZCLV; break;
                    case 8:
                        tempByte = e8XDZCLV; break;
                    case 12:
                        tempByte = e12XDZCLV; break;
                    case 16:
                        tempByte = e16XDZCLV; break;
                    case 20:
                        tempByte = e20XDZCLV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x31);                    /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x10)//CAV mode
            {
                switch(Iram_Cmd_block.bf[8])
                {
                    case 1:
                        tempByte = e1XDCAV; break;
                    case 2:
                        tempByte = e2XDCAV; break;
                    case 3:
                        tempByte = e3XDCAV; break;
                    case 4:
                        tempByte = e4XDCAV; break;
                    case 5:
                        tempByte = e5XDCAV; break;
                    case 6:
                        tempByte = e6XDCAV; break;
                    case 8:
                        tempByte = e8XDCAV; break;
                    case 12:
                        tempByte = e12XDCAV; break;
                    case 16:
                        tempByte = e16XDCAV; break;
                    case 20:
                        tempByte = e20XDCAV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x66);                    /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x08)//CLV mode
            {
                switch(Iram_Cmd_block.bf[8])
                {
                    case 1:
                        tempByte = e1XDCLV; break;
                    case 2:
                        tempByte = e2XDCLV; break;
                    case 3:
                        tempByte = e3XDCLV; break;
                    case 4:
                        tempByte = e4XDCLV; break;
                    case 5:
                        tempByte = e5XDCLV; break;
                    case 6:
                        tempByte = e6XDCLV; break;
                    case 8:
                        tempByte = e8XDCLV; break;
                    case 12:
                        tempByte = e12XDCLV; break;
                    case 16:
                        tempByte = e16XDCLV; break;
                    case 20:
                        tempByte = e20XDCLV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x67);                    /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x78)
            {
                SkipErrRateNum = 0;
                BUILD_SENSE(0x05,0x24,0x00,0x68);                    /* INVALID PARAMETER IN CDB */
                return;
            }

            ServoPause();
            SpeedMD = BypassSpeedMD;

//            bBakVWSpeed = tempByte;                                  /* store to allow use for PCAV read */
//            mNVWP_DM_SPEED = GetSpeedValue(tempByte);
            if(ServoSetSpeed(tempByte) == FAIL)
            {
                BUILD_SENSE(0x05,0x24,0x00,0x53);                    /* INVALID PARAMETER IN CDB */
                return;
            }
            ServoPause();
        }//if (Iram_Cmd_block.bf[8] != 0x00)

        HostXfer.BlockCount.byte.u = 0;   // Get the host request starting LBA from the command block
        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[5] ;
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[6];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[7];
        if (Iram_Cmd_block.bf[2] & 0x02)
            Addr_mode = 1;   //PSN mode
        else
            Addr_mode = 0;   //LBA mode

        if ((Addr_mode == 0) || ((pdbGetDVDLayers() == DUAL_LAYER)&&(pdbGetDVDType() == DVD_ROM)))
        {
            if (Addr_mode == 1) //PSN mode in dual layer disc
            {
                if(pdbGetDVDTrackPath() == DVDHD_PTP_DISC)
                {
                    if(Iram_Cmd_block.bf[2] & 0x04)
                    {
                        HostXfer.BlockCount.Long = pdbGetDVDEndLBALayer0() + 1L + HostXfer.BlockCount.Long -0x30000;
                        DiscLayer = 1;
                    }
                    else
                    {
                        HostXfer.BlockCount.Long = HostXfer.BlockCount.Long - 0x30000L;
                        DiscLayer = 0;
                    }
                }
                else    /*OTP Disc*/
                {
                    if(HostXfer.BlockCount.Long <= pdbGetDVDMaxPSN(0))
                    {
                        HostXfer.BlockCount.Long = HostXfer.BlockCount.Long - 0x30000L;
                        DiscLayer = 0;
                    }
                    else
                    {
                        ULONG OtpStartPsnLayer1;
                        OtpStartPsnLayer1 = (~(SWAPEND_ULONG(Info.Media.DVD.PFInfoP->EndPSNLayer0))) & 0x00FFFFFFL;
                        HostXfer.BlockCount.Long = pdbGetDVDEndLBALayer0() + 1L + HostXfer.BlockCount.Long - OtpStartPsnLayer1;
                        DiscLayer = 1;
                    }
                }
            }
#if (DVD_RAM_READ == 1)
            if (pdbGetDVDType() == DVD_RAM)
            {
                StartPSN.PSN = ConvertLSNToPSN(HostXfer.BlockCount.Long);
                StartPSN.Layer = eLayer0;
                Last_PSN = 0x34200;
                send_msg5SValue(SEND_MRW_MSG,"StartPSN",4,StartPSN.PSN);

            }
            else
#endif
            {
                StartPSN = dvdConvertLBAtoPSN(HostXfer.BlockCount.Long);
                dvdReadAheadStartPSN = StartPSN.PSN - 0x10; //don't make dvdReadAheadStartPSN equal to dvdLastDecodedPSN

                if (HostXfer.BlockCount.Long > pdbGetDVDMaxLBA())
                {
                    BUILD_SENSE(0x05,0x21,0x00,0x4a);
                    return;
                }
            }
        }
        else //PSN mode in single layer disc
        {
            if (pdbGetDVDLayers() == DUAL_LAYER)
            {
                if (HostXfer.BlockCount.Long > pdbGetDVDMaxPSN(1))
                {
                    BUILD_SENSE(0x05,0x21,0x00,0x4c);
                    return;
                }
                StartPSN.PSN = HostXfer.BlockCount.Long;
                if(StartPSN.PSN > 0x800000)
                    StartPSN.Layer = eLayer1;
                else
                    StartPSN.Layer = eLayer0;
                dvdReadAheadStartPSN = pdbGetDVDMaxPSN(1); //don't make dvdReadAheadStartPSN equal to dvdLastDecodedPSN
            }
            else
            {
                if (HostXfer.BlockCount.Long > pdbGetDVDMaxPSN(0))
                {
                    BUILD_SENSE(0x05,0x21,0x00,0x4b);
                    return;
                }
#if (DVD_RAM_READ == 1)
                if (pdbGetDVDType() == DVD_RAM)
                {
                    ULONG PSN0;

                    PSN0 = ConvertPSNToLSN(HostXfer.BlockCount.Long);
                    PSN0 &= 0xfffffff0L;    //adjust to LSN boundary

                    StartPSN.PSN = HostXfer.BlockCount.Long;
                    StartPSN.Layer = eLayer0;
                    Last_PSN = StartPSN.PSN;
                    send_msg5SValue(SEND_MRW_MSG,"StartPSN",4,StartPSN.PSN);
                }
                else
            #endif
                {
                    StartPSN.PSN = HostXfer.BlockCount.Long;
                    StartPSN.Layer = eLayer0;
                    dvdReadAheadStartPSN = pdbGetDVDMaxPSN(0); //don't make dvdReadAheadStartPSN equal to dvdLastDecodedPSN
                }
            }
        }

        if (StartPSN.Layer == eLayer1)
            BlockRequest = pdbGetDVDMaxPSN(1);
        else
            BlockRequest = pdbGetDVDMaxPSN(0);

        BlockRequest = (BlockRequest - StartPSN.PSN)/ONE_DVD_BLOCK;
        if (BlockRequest == 0L)    // Read one ecc block into buffer
        {
            BUILD_SENSE(0x05,0x24,0x00,0x54);                    /* INVALID PARAMETER IN CDB */
            return;
        }

#if (DPD_COMPENSATE_OFFSET_DIAG==1)
        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            if (StartPSN.Layer != Layer_Index)
            {
                ServoSeek(StartPSN.PSN,StartPSN.Layer);
            }
            ServoCalibDpdWholeDisc();//improve whole disc pre calibration
        }
#endif//#if (DPD_COMPENSATE_OFFSET_DIAG==1)

#if (CALIBRATION_MANAGEMENT == 1)
        if ((svoVar_discDiameter == e12cm) && !DVDRAM)
        {
            if (StartPSN.Layer != Layer_Index)
            {
                ServoSeek(StartPSN.PSN,StartPSN.Layer);
            }
            ServoCalibWholeDisc();
        }
#endif//#if (CALIBRATION_MANAGEMENT == 1)

        dvdDiscRequest.Type            = INIT_READ;
        dvdDiscRequest.TargetID.Long   = StartPSN.PSN;
        dvdDiscRequest.TargetLayer     = StartPSN.Layer;
        dvdDiscRequest.EccBlockCount   = BlockRequest;

        WAIT_PI_PO = 0;
        READ_PI_PO = 1;
        WRITE_FIELD(HAL_HST_BUFPGCMP, 0x100);
        WRITE_FIELD(HAL_DVDD_ECCCNT,0);
        if ((svoVar_discKindT.Bit.Media == eMediaROM) || (svoVar_discKindT.Bit.Media == eMediaRAM))
        {
              // DVD-ROM / RAM disc
#if (DVD_RAM_READ == 1)
            if (svoVar_discKindT.Bit.Media == eMediaRAM)
            {
                RWDisc.dvRAM.IceHasBeenDetected = SET;
                dvdramDiscRequestStartLSN = ConvertPSNToLSN(dvdDiscRequest.TargetID.Long);
                #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
                dvdramDecodedStrPSN = dvdDiscRequest.TargetID.Long;
                #endif
                //dvdramLastDecodedLSN updated in dvdInitializeDiscTransfer()
                RWDisc.dvRAM.ReadDataMode = eREAD_ERROR_RATE;
            }
#endif
            spdHoldRPM(spdGetTargetRPM(svoPBA2Radius(StartPSN.PSN), svoSpeedVar.SpdCurrentSpeed));
            spdWaitSpdInRange();
            DiscReadStatus = dvdStartDiscTransfer();
            spdUnhold();
            if (DiscReadStatus != BUF_GOOD)    // Read one ecc block into buffer
            {
                READ_PI_PO = 0;
                WRITE_FIELD(HAL_DVDD_ECCCNT,DVD_MULTI_PASS_COUNT);
                BUILD_SENSE(0x03,0x11,0x0B,0x31);               // UNRECOVERED READ ERROR
                return;
            }
        }
        else
        {
            ENABLE_DVD_DECODER();    /* Turn on DVD decoder */

            WRITE_FIELD(HAL_DVDD_TIDDIS, TRUE);        /* enable target id */

            SET_DISC_BUFFER_PTR(0);
            SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);
            CLEAR_DVD_DECODER_STATUS();
            ENABLE_DVD_DECODER_INTS();
            dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;

            seek_retry_count = 0;
            while(1)
            {
                if (seek_retry_count > 2)
                {
                    spdUnhold();
                    READ_PI_PO = 0;
                    WRITE_FIELD(HAL_DVDD_ECCCNT,DVD_MULTI_PASS_COUNT);
                    BUILD_SENSE(0x03,0x02,0x00,0x31);    // NO SEEK COMPLETE
                    return;
                }

                spdHoldRPM(spdGetTargetRPM(svoPBA2Radius(StartPSN.PSN), svoSpeedVar.SpdCurrentSpeed));
                spdWaitSpdInRange();

                if (ServoSeek((StartPSN.PSN-16L),StartPSN.Layer)==PASS)
                {
                    spdUnhold();
                    ClearAddressRead();

                    /* improve atip seek performance */
                    if (Iram_Cmd_block.bf[2] & 0x01)
                    {
                        Svr_pause_reqire = TRUE;
                        WRITE_FIELD(HAL_DVDD_ECCCNT,DVD_MULTI_PASS_COUNT);
                        return;
                    }
                    else
                    {
                        if (ServoSeek(StartPSN.PSN,StartPSN.Layer)==PASS)
                        {
                            ClearAddressRead();
                            WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                            Svr_pause_reqire = FALSE;
                            break;
                        }
                    }
                }
                seek_retry_count++;
            }
        }
    }
    else    //CD_DISC
    {
        BYTE    sek_mode;
        BYTE    tempByte;
        LONG    lba_addr;
        TimeS   time;
        ULONG   clock;
        BYTE    Cur_second;
        BYTE    temp_IE;
#if (RESET_PLL_FOR_HUGEPIC1==1)
        resetmin=0;
        resetsec=0;
#endif

#if (CHIP_REV <= 0xB2)
        UseC1CG1 = FALSE;
#endif
        /* Reset CD Decoder */
        /* Prevent the Read C1/C2 vendor command from getting the wrong values */
        WRITE_FIELD(HAL_CDD_DPCLR, 1);              /* Set Decoder partial clear */
        WRITE_FIELD(HAL_CDD_DPCLR, 0);              /* Clear Decoder partial clear */

/*----------------------------------------------------------------------------
  Initialize internal memory variables and flags
  prevent cannot read NEROCDSPEED Access Time  after VendorReadC1/C2.
----------------------------------------------------------------------------*/
        temp_IE = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        Iram_usiRingSP  = 0;
        RING_END_PAGE  = RING_END_PAGE_2p5K;
        Iram_usiRingEP  = RING_END_PAGE;
        BUF_TOTAL_BLK_COUNT = RING_END_PAGE + 1;

        WRITE_FIELD(HAL_HST_HSTBUFSPG, Iram_usiRingSP);     /* host start blk. */
        WRITE_FIELD(HAL_CDD_DSKBUFSPG, Iram_usiRingSP);     /* ring start blk. */

        WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);     /* Host end blk. */
        WRITE_FIELD(HAL_CDD_DSKBUFEPG, Iram_usiRingEP);     /* ring end blk. */

        InitCopyStartEndPages();            /* Initialize Copy block Start and End pages */

        WRITE_FIELD(HAL_HST_HSTBYTAC, 0);
        WRITE_FIELD(HAL_HST_HSTBUFPG, 0);
        WRITE_FIELD(HAL_HST_BUFPGCMP, 0);

        WRITE_FIELD(HAL_CDD_AUTOEM, 1);     /* YBM2 Work Around */
        WRITE_FIELD(HAL_CDD_DINFWE, 1);
        if(Iram_Rw_read_mode == eCDDAMode)
            WRITE_FIELD(HAL_CDD_IHDRW, 0);/* Audio has no header */
        else
            WRITE_FIELD(HAL_CDD_IHDRW, 1); // YBM2 Work Around

        SET_INT_ENABLE_MASK(temp_IE);
/*----------------------------------------------------------------------------
  End of initial
----------------------------------------------------------------------------*/

        StopDecoder();

        if (EjectKeyInfo.PressedValid)
        {
            BUILD_SENSE(0x02,0x3A,0x00,0x2b);
            return;
        }

        if (Iram_Cmd_block.bf[8] != 0x00)
        {
            if ((Iram_Cmd_block.bf[8]==0xff)||(Iram_Cmd_block.bf[8]>52))
            {
                Iram_Cmd_block.bf[8] = 52;
            }

            SkipErrRateNum = 3;

            if ((Iram_Cmd_block.bf[2] & 0x78) == 0x00)//use old definition
            {
                switch (Iram_Cmd_block.bf[8])
                {
                    case 4:
                        tempByte = e4XCLV; break;
                    case 8:
                        tempByte = e8XCLV; break;
                    case 10:
                        tempByte = e10XCLV; break;
                    case 11:
                        tempByte = e10XCAV; break;
                    case 12:
                        tempByte = e12XCLV; break;
                    case 14:
                        tempByte = e12XCAV; break;
                    case 16:
                        tempByte = e16XCLV; break;
                    case 17:
                        tempByte = e16XCAV; break;
                    case 20:
                        tempByte = e20XCAV; break;
                    case 24:
                        tempByte = e24XCAV; break;
                    case 32:
                        tempByte = e32XCAV; break;
                    case 40:
                        tempByte = e40XCAV; break;
                    case 48:
                        tempByte = e48XCAV; break;
                    case 52:
                        tempByte = e52XCAV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x55);              /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x40)//PCAV mode
            {
                switch (Iram_Cmd_block.bf[8])
                {
                    case 4:
                        tempByte = e4XPCAV; break;
                    case 8:
                        tempByte = e8XPCAV; break;
                    case 10:
                        tempByte = e10XPCAV; break;
                    case 12:
                        tempByte = e12XPCAV; break;
                    case 16:
                        tempByte = e16XPCAV; break;
                    case 20:
                        tempByte = e20XPCAV; break;
                    case 24:
                        tempByte = e24XPCAV; break;
                    case 32:
                        tempByte = e32XPCAV; break;
                    case 40:
                        tempByte = e40XPCAV; break;
                    case 48:
                        tempByte = e48XPCAV; break;
                    case 52:
                        tempByte = e52XPCAV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x32);              /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x20)//ZCLV mode
            {
                switch (Iram_Cmd_block.bf[8])
                {
                    case 4:
                        tempByte = e4XZCLV; break;
                    case 8:
                        tempByte = e8XZCLV; break;
                    case 10:
                        tempByte = e10XZCLV; break;
                    case 12:
                        tempByte = e12XZCLV; break;
                    case 16:
                        tempByte = e16XZCLV; break;
                    case 20:
                        tempByte = e20XZCLV; break;
                    case 24:
                        tempByte = e24XZCLV; break;
                    case 32:
                        tempByte = e32XZCLV; break;
                    case 40:
                        tempByte = e40XZCLV; break;
                    case 48:
                        tempByte = e48XZCLV; break;
                    case 52:
                        tempByte = e52XZCLV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x33);              /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x10)//CAV mode
            {
                switch (Iram_Cmd_block.bf[8])
                {
                    case 4:
                        tempByte = e4XCAV; break;
                    case 8:
                        tempByte = e8XCAV; break;
                    case 10:
                        tempByte = e10XCAV; break;
                    case 12:
                        tempByte = e12XCAV; break;
                    case 16:
                        tempByte = e16XCAV; break;
                    case 20:
                        tempByte = e20XCAV; break;
                    case 24:
                        tempByte = e24XCAV; break;
                    case 32:
                        tempByte = e32XCAV; break;
                    case 40:
                        tempByte = e40XCAV; break;
                    case 48:
                        tempByte = e48XCAV; break;
                    case 52:
                        tempByte = e52XCAV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x69);              /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x08)//CLV mode
            {
                switch (Iram_Cmd_block.bf[8])
                {
                    case 4:
                        tempByte = e4XCLV; break;
                    case 8:
                        tempByte = e8XCLV; break;
                    case 10:
                        tempByte = e10XCLV; break;
                    case 12:
                        tempByte = e12XCLV; break;
                    case 16:
                        tempByte = e16XCLV; break;
                    case 20:
                        tempByte = e20XCLV; break;
                    case 24:
                        tempByte = e24XCLV; break;
                    case 32:
                        tempByte = e32XCLV; break;
                    case 40:
                        tempByte = e40XCLV; break;
                    case 48:
                        tempByte = e48XCLV; break;
                    case 52:
                        tempByte = e52XCLV; break;
                    default:
                        SkipErrRateNum = 0;
                        BUILD_SENSE(0x05,0x24,0x00,0x6A);              /* INVALID PARAMETER IN CDB */
                        return;
                }
            }
            else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x78)
            {
                SkipErrRateNum = 0;
                BUILD_SENSE(0x05,0x24,0x00,0x6B);              /* INVALID PARAMETER IN CDB */
                return;
            }
            SpeedMD = BypassSpeedMD;

            if (ServoSetSpeed(tempByte) == FAIL)
            {
                BUILD_SENSE(0x05,0x24,0x00,0x56);                    /* INVALID PARAMETER IN CDB */
                return;
            }
        }

    #if (DPD_COMPENSATE_OFFSET_DIAG==1)
        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            //dd Pre-do DPD calibration after change speed.
            ServoCalibDpdWholeDisc();
        }
    #endif//#if (DPD_COMPENSATE_OFFSET_DIAG==1)

        if (St_cdrom_audio == TRUE)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x420007); //AudioCD
            Iram_Rw_read_mode = eCDDAMode;
            EccCtrlMode(eAudioCD);
        }
        else
        {
            SendMsg80(SHOW_DEBUG_MSG,0x420008); //Data/MixCD
            EccCtrlMode(eDataCD);
        }

        seek_retry_count = 0;

        time.Minute=Iram_Cmd_block.bf[5];
        time.Second=Iram_Cmd_block.bf[6];
        time.Frame=Iram_Cmd_block.bf[7];
//        lba_addr = MSF2LBA(&time) - 155L;/* for skip 2 second error rate test */
        lba_addr = MSF2LBA(&time);

        sek_mode = eDataSeek;

        while (1)
        {
            if (EjectKeyInfo.PressedValid)
            {
                spdUnhold();
                BUILD_SENSE(0x02,0x3A,0x00,0x2c);        // NOT READY, MEDIUM NOT PRESENT
                return;
            }

            lba_addr = lba_addr - (seek_retry_count*10);

            seek_retry_count ++;
            spdHoldRPM(spdGetTargetRPM(svoPBA2Radius(lba_addr), svoSpeedVar.SpdCurrentSpeed));
            spdWaitSpdInRange();
            if (ServoSeek(lba_addr,sek_mode) == PASS)
            {
                spdUnhold();
                svoIramVar.subQEnableFlag = 1;
                svoEnTimeRead();
                if (Iram_Cmd_block.bf[8] != 0x00)
                {
                    if (pdbGetSpeed() != tempByte)
                    {
                        pdbSetSpeed(tempByte);
                    }
                }

                if (Iram_Cmd_block.bf[2] & 0x01)
                {
                    Svr_pause_reqire = TRUE;
                    return;
                }
                Svr_pause_reqire = FALSE;
                break;
            }
            else if (seek_retry_count > 3)
            {
                spdUnhold();
                BUILD_SENSE(0x03,0x02,0x00,0x33);    // NO SEEK COMPLETE
                return;
            }
        }//while(1)

        /* for skip 2 second error rate test */
        Cur_second = HEX2BCD(Iram_Cmd_block.bf[6]);
        StartTimer(&clock);

        sek_mode = 0; /* filter data before 0:0:0 for CD-ROM */
        do
        {
            if(ReadTimer(&clock) > 1000L)
            {
                return;
            }

            while (pdbGetSubQIntStatus())
            {
                pdbGetABSTime(&time);
                pdbSetSubQIntStatus(FALSE);
            }

            if (EjectKeyInfo.PressedValid)
            {
                BUILD_SENSE(0x02,0x3A,0x00,0x2e);        // NOT READY, MEDIUM NOT PRESENT
                return;
            }

            /* filter data before 0:0:0 for CD-ROM */
            if(pdbGetSubQTno() == 0)
                sek_mode = 1;
            else
                sek_mode = 0;
        }
        while((Cur_second != time.Second)  || (sek_mode == 1));
    }  //CD_DISC
    #if (SUN_FUNC_TEST == 1)
    READ_C1_C2 =1;
    #endif
}

#if(BCA_ENABLE == 1)
void diaReadBCA(void)
{
    BYTE        *DramAddressP;
    BYTE        BCARetryTimes;
    WORD        HostTransferLength;

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    if (HostTransferLength == 0)
    {
        AutoCommandComplete();
        return;
    }

    if(St_become_ready == FALSE)
    {
        if((St_no_disc == FALSE) && (ataPowerMode == STANDBY_MODE))
        {
            if (ServoStartUp() == PASS)
            {
                switch (pdbDiscLoaded())
                {
                    case BD_DISC:
                        SetReadSpeed(eBDSpinUpSpeed);       //sync SVO speed
                        break;
                    case DVD_DISC:
                        SetReadSpeed(eDVDSpinUpSpeed);  //sync SVO speed
                        break;
                }
            }
        }
    }

    if(pdbDiscLoaded() == DVD_DISC)
    {
        BCARetryTimes = 0;
        while (BCARetryTimes < ATAPI_BCA_RETRY_TIMES)
        {
            if (DVDReadBCA() == FAIL)
            {
                BCARetryTimes++;
            }
            else
            {
                break;
            }
        }

        if (BCARetryTimes >= ATAPI_BCA_RETRY_TIMES)
        {
            ServoPause();
            BUILD_SENSE(0x05,0x6F,0x02,0x11);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
            AutoCommandComplete();
            return;
        }
        /* Insure that maximum transfer length is not exceeded */
        HostTransferLength = MINIMUM(188,HostTransferLength);
        DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
        // Set up the BCA transfer length = BCA data length + 2 reserved bytes.
        HostTransferLength = MINIMUM(HostTransferLength, dvdDiscRequest.BcaLen);
        if (HostTransferLength >=4)
            HostTransferLength = HostTransferLength -4; /* the header length */

        *DramAddressP++ = (GET_BHIGH(HostTransferLength));     // Prepare Header
        *DramAddressP++ = (GET_BLOW(HostTransferLength));
        *DramAddressP++ = 0;
        *DramAddressP++ = 0;

        // First send Header to host.
        SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);         // Transfer data do not send Command Complete yet.
        // Now transfer the BCA data to host.
        SendParameterDataToHost(DVD_BCA, HostTransferLength, TRUE);    // Transfer data and send Command Complete.
        return;
    }
#if (BD_ENABLE == 1)
    else if (pdbDiscLoaded() == BD_DISC)
    {
        ReadBDBCAValid = FALSE;
        if (BDReadBCA() == FAIL)
        {
            BUILD_SENSE(0x05,0x6F,0x02,0x11);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
            AutoCommandComplete();
            return;
        }
        HostTransferLength = MINIMUM(132,HostTransferLength);
        DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
         if (HostTransferLength >=4)
            HostTransferLength = HostTransferLength -4; /* the header length */
        *DramAddressP++ = (GET_BHIGH(HostTransferLength));     // Prepare Header
        *DramAddressP++ = (GET_BLOW(HostTransferLength));
        *DramAddressP++ = 0;        // Reserved.
        *DramAddressP++ = 0;        // Reserved.

        SendParameterDataToHost(DVD_CSS_SCRATCH, 4 , FALSE);         // Transfer data do not send Command Complete yet.
        SendParameterDataToHost(SCSDRAM_ADDRESS, HostTransferLength, TRUE);    // Transfer data cmplete.

    }
#endif
    else
    {
        if(pdbDiscLoaded() == NON_CD_DVD_DISC)
            BUILD_SENSE(0x02,0x3A,0x00,0x21);/* MEDIUM NOT PRESENT */
        else
            BUILD_SENSE(0x05,0x39,0x00,0x87);
        AutoCommandComplete();
        return;
    }
}
#endif //(BCA_ENABLE == 1)

int iBeta_modulation[3];
#define DVD_Decode_Error    DVD_BLK_ERROR || DVD_BUFFER_ERROR
#if (BypassDecoderEr == 1)
extern BYTE IGNORE_DECODER_ERROR;
#endif

// TBD!
//#define MEAS_RF_ONLY
void diaBLERCommand()
{
    ULONG clock, clock2, clock3, CurrentWblPSN;
    SHORT temp_bktobuf;
    WORD temp_Hstbuf;
    BYTE InterruptState;
    BYTE  ftimeout;
    int     tempData;
    long    adPHO,adBHO,adREF;
    SubQCodeS subcd;
    USHORT  btmp;
    float   std_beta_modulation;
    ULONG add_length;
    BYTE decodeErr;

//#if (DVD_WRITE == 1)
    BYTE VcoResetCnt;
    #if (WOBBLE_ID == 0)
    StLongU   currentID_temp;
    ULONG CurrentDataPSN;
    #endif
//#endif

    BYTE ServoADCSignal;
    ADSignal_t sig;

#ifdef MEAS_RF_ONLY
    BYTE    modulation;
#endif
#if 0 //For you to use TA720 to monitor jitter
    Set_MIO0_TO_DRESignal(0x01);
    Set_MIO2_TO_DRESignal(0x01);

    SetBitsSCReg_1B(rwDSTEST1,bit0);
    ClrBitsSCReg_1B(rwDSTEST1,bit1);

    SetBitsSCReg_1B(0x84B,bit5);
    ClrBitsSCReg_1B(0x84B,bit6);
#endif

    #if (MSG_OUT_MODE == MSG_USBPORT)
    if (hifDebugMsgFlag & (SEND_QCODE_DATAID|SEND_ATIP_WOBBLE))
    {
        //Send_timef = 1; //Messages are too many. Please turn it on manually when you need.
    }
    #endif

    decodeErr = 0;

#if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC)
    {
    #if (BypassDecoderEr == 1)
        IGNORE_DECODER_ERROR = FALSE;
    #endif
        diaBDBLERCommand();
    }
    else
#endif
#if (ENABLE_HDDVD)
    if (pdbDiscLoaded() == HD_DISC)
    {
        diaHDBLERCommand();
        return;
    }
    else
#endif
    if (pdbDiscLoaded() == DVD_DISC)
    {
#if (BypassDecoderEr == 1)
        IGNORE_DECODER_ERROR = FALSE;
#endif
        if ((Iram_Cmd_block.bf[5] == 0x0A))
        {
            WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
            READ_PI_PO = 0;
            Send_timef = 0;
            ResetDecoder = FALSE;
            dvdStopDiscTransfer();
            dvdResetDiscFlags();
            Svr_pause_reqire = TRUE;
            St_diag_ena = FALSE;        /* Diagnostics Disabled */
         #if (DISCARD_RAM_DECODE == 1)
            RWDisc.dvRAM.ContinuteIceCount = 0;
         #endif
            ClearDataBuffer();
            AutoCommandComplete();
            return;
        }

        if(SkipErrRateNum)
        {
        #if (CHIP_REV >= 0xC0)
            sig.mux1 = halAFE_MUX1ADSEL_VREFFCT;
        #else
            sig.mux1 = halAFE_MUX1ADSEL_VREF_FCTS;
        #endif
            ServoADCSignal = AfeRouteSignalToAD(AFE_MUXTOAD1,sig);
        }

        if (svoVar_discKindT.Bit.Media == eMediaROM)
        { // DVD-ROM disc
            ULONG clk2_timeout;

            StartTimer(&clock);
            StartTimer(&clock2);
            ftimeout = FALSE;

            std_beta_modulation  = 0.0f;
            btmp  = 0;
            adBHO = 0L;
            adPHO = 0L;
            adREF = 0L;

            if(svoVar_discKindT.Bit.Media == eMediaRAM)
                clk2_timeout = 800L;
            else
                clk2_timeout = 400L;

            while(1)
            {
                if (EjectKeyInfo.PressedValid)
                {
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    Svr_pause_reqire = TRUE;
                    BUILD_SENSE(0x02,0x3A,0x00,0x2f);        // NOT READY, MEDIUM NOT PRESENT
                    AutoCommandComplete();
                    return;
                }

                if(ReadTimer(&clock) > 4000L)
                {
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    Svr_pause_reqire = TRUE;
                    BUILD_SENSE(0x03,0x02,0x00,0x35);
                    AutoCommandComplete();
                    return;
                }

                if(btmp<32)
                {   //get PHO, BHO, RRFLP 32 times
                    adPHO += ReadSingleAdcmV(ADC_PHSO);
                    adBHO += ReadSingleAdcmV(ADC_BHSO);
                    adREF += ReadSingleAdcmV(ServoADCSignal); //select VREFB
                    btmp++;
                }

                if(WAIT_PI_PO == 1)
                {
                    temp_bktobuf = READ_FIELD(HAL_DVDD_BKTOBUF);
                    if(temp_bktobuf <10)
                    {
                        WRITE_FIELD(HAL_DVDD_BLKINC, 32);
                    }
                    if(temp_bktobuf > MAX_VALUE_OF_BLKINC)
                    {
                        temp_bktobuf = MAX_VALUE_OF_BLKINC;
                        WRITE_FIELD(HAL_DVDD_BLKINC, temp_bktobuf);
                    }
                    while (btmp<32)
                    {   //get PHO, BHO, RRFLP 32 times
                       adPHO += ReadSingleAdcmV(ADC_PHSO);
                       adBHO += ReadSingleAdcmV(ADC_BHSO);
                       adREF += ReadSingleAdcmV(ADC_MUXOUT2); //select VREFB
                       btmp++;
                    }
                    break;
                }
                else if((ReadTimer(&clock2) > clk2_timeout) || DVD_Decode_Error )
                {
                    if ( DVD_BUFFERING() == ON )
                    {
                        dvdStopDiscTransfer();
                    }

                    WAIT_PI_PO = 0;

                    if(DVD_Decode_Error)
                    {
                        ftimeout = FALSE;
                        decodeErr = 1;
                    }
                    else
                    {
                        ftimeout = TRUE;
                        decodeErr = 0;
                    }

                    if(dvdErrorFlag.fUNCOR_ECC)
                    {
                        dvdErrorFlag.fUNCOR_ECC = 0;
                    }
                    else if(dvdErrorFlag.fEDC_ERROR)
                    {
                        dvdErrorFlag.fEDC_ERROR = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 1111;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if(dvdErrorFlag.fSYNC_LOST)
                    {
                        dvdErrorFlag.fSYNC_LOST = 0;
                            PO_UN_C = 2222;
                        WAIT_PI_PO = 1;
                        Last_PSN = Last_PSN + 0x10L;
                        PIPO_CURRENT_ID.Long = Last_PSN;

                    }
                    else if(dvdErrorFlag.fSEQ_ID_ERROR)
                    {
                        dvdErrorFlag.fSEQ_ID_ERROR = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 3333;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if (dvdErrorFlag.fTID_MISS)
                    {
                        dvdErrorFlag.fTID_MISS = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 4444;
                            WAIT_PI_PO = 1;
                        }

                        if(svoVar_discKindT.Bit.Media == eMediaRAM)
                            PIPO_CURRENT_ID.Long = Last_PSN;    //show correct PSN
                    }
                    else if (dvdErrorFlag.fID_MISMATCH)
                    {
                        dvdErrorFlag.fID_MISMATCH = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 6666;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if (dvdErrorFlag.fID_ERROR)
                    {
                        dvdErrorFlag.fID_ERROR = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 7777;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if (dvdErrorFlag.fSEEK_FAIL)
                    {
                        dvdErrorFlag.fSEEK_FAIL = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 8888;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if(ftimeout)
                    {
                        BOOL PIPOZero;
                        ftimeout = FALSE;
                        PIPOZero = FALSE;

                        if(svoVar_discKindT.Bit.Media == eMediaROM)
                            Last_PSN = Last_PSN + 0x10L;
                        PIPO_CURRENT_ID.Long = Last_PSN;

                        if (pdbGetDiscKind() == eDVDROMDisc)
                        {
                            if (Last_PSN > (pdbGetDVDMaxPSN(0) - 20*ONE_DVD_BLOCK))
                                PIPOZero = TRUE;
                        }
                        else if (pdbGetDiscKind() == eDVDROMDLDisc)
                        {

                            if(DiscLayer == 1)
                            {
                                if (Last_PSN > (pdbGetDVDMaxPSN(1) - 20*ONE_DVD_BLOCK))
                                    PIPOZero = TRUE;
                            }
                            else
                            {
                                if (Last_PSN > (pdbGetDVDMaxPSN(0) - 20*ONE_DVD_BLOCK))
                                    PIPOZero = TRUE;
                            }
                        }

                        if(PIPOZero)
                        {
                            PI_C = 0000;
                            PI_E = 0000;
                            PO_C = 0000;
                            PI_UN_C = 0000;
                            PO_UN_C = 0000;
                        }
                        else
                        {
                            PI_C = 9999;
                            PI_E = 9999;
                            PO_C = 9999;
                            PI_UN_C = 9999;
                            PO_UN_C = 9999;
                        }
                        WAIT_PI_PO = 1;
                    }
                    else
                    {

                    }

                    dvdStopDiscTransfer();

                    InterruptState = GET_INT_ENABLE_MASK();
                    DISABLE_INTERRUPT();

                    CurrentWblPSN = Last_PSN + 0x10L;
                    if(pdbGetDVDLayers() == DUAL_LAYER)
                    {
                        if(pdbGetDVDTrackPath() == DVDHD_OTP_DISC)
                        {
                            if((CurrentWblPSN > pdbGetDVDMaxPSN(0)) && (CurrentWblPSN < pdbGetDVDStartPSN(1)))
                            {
                                add_length = CurrentWblPSN - pdbGetDVDMaxPSN(0);
                                CurrentWblPSN = pdbGetDVDStartPSN(1) + add_length-1;
                            }
                        }
                        else /* PTP disc*/
                        {
                            if(CurrentWblPSN > pdbGetDVDMaxPSN(0))
                            {
                                add_length = CurrentWblPSN - pdbGetDVDMaxPSN(0);
                                CurrentWblPSN = pdbGetDVDStartPSN(1) + add_length-1;
                            }
                        }
                    }
                    SET_DISC_BUFFER_PTR(DataBuffer.CurDiscPtr);
                    SET_INT_ENABLE_MASK(InterruptState);

                    SET_TARGET_ID(CurrentWblPSN);
                    SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);

                    dvdResetDiscFlags();
                    CLEAR_DVD_DECODER_STATUS();
                    START_DVD_BLOCK_TIMER(FIRST_BLOCK_TIMEOUT_VALUE);
                    ENABLE_DVD_DECODER_INTS();

                    if (CurrentWblPSN > 0x800000)
                        dvdDiscRequest.TargetLayer = eLayer1;
                    else
                        dvdDiscRequest.TargetLayer = eLayer0;

                    if((pdbGetDVDLayers() == DUAL_LAYER)&&(pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
                    {
                        dvdDiscRequest.TargetLayer = svoIramVar.currentID.Layer;
                        if(CheckHeaderID() == TRUE)
                        {
                           dvdDiscRequest.TargetLayer = svoIramVar.currentID.Layer;
                        }
                    }
                    if( (dvdSeek(CurrentWblPSN ,dvdDiscRequest.TargetLayer)) == Error)
                    {
                        WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                        Svr_pause_reqire = TRUE;
                        Send_timef = 0;
                        BUILD_SENSE(0x03,0x11,0x0B,0x36);               // UNRECOVERED READ ERROR
                        AutoCommandComplete();         // Send Command Complete to host if requested by calling function
                        return;
                    }
                    else
                    {
                        dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
                        CLEAR_SYNC_LOST_STATUS();

                        if (svoVar_discKindT.Bit.Media != eMediaRAM)
                            ENABLE_DVD_SYNC_LOST_INT();
                        else
                        {
                            #if (DVD_RAM_READ == 1)
                            RWDisc.dvRAM.ReadDataMode = eREAD_ERROR_RATE;
                            #endif
                        }

                        temp_Hstbuf = READ_FIELD(HAL_DVDD_CURECC);
                        temp_Hstbuf += 5*ONE_DVD_BLOCK;     /* Advance the  pointer */
                        temp_Hstbuf %= MAX_DVD_SECTORS;   /* Insure buffer wrap */

                        WRITE_FIELD(HAL_HST_BUFPGCMP, temp_Hstbuf);
                        ENABLE_DVD_BUFFERING();
                        StartTimer(&clock2);
                    }
                }

                ExitProcess();
            }//while(1)

            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            PIPO_CURRENT_ID.byte.u = 0;


            if(DVD_Decode_Error || decodeErr)
            {
                Last_PSN = Last_PSN + 0x10L;
                PIPO_CURRENT_ID.Long = Last_PSN;
            }
            else
            {
                Last_PSN = PIPO_CURRENT_ID.Long;
            }
        }

#if (DVD_RAM_READ == 1)
        else if (svoVar_discKindT.Bit.Media == eMediaRAM)
        { // DVD-RAM disc
            ULONG clk2_timeout;

            StartTimer(&clock);
            StartTimer(&clock2);
            ftimeout = FALSE;

            std_beta_modulation  = 0.0f;
            btmp  = 0;
            adBHO = 0L;
            adPHO = 0L;
            adREF = 0L;

            if(svoVar_discKindT.Bit.Media == eMediaRAM)
                clk2_timeout = 800L;
            else
                clk2_timeout = 400L;

            while(1)
            {
                if (EjectKeyInfo.PressedValid)
                {
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    Svr_pause_reqire = TRUE;
                    BUILD_SENSE(0x02,0x3A,0x00,0x23);        // NOT READY, MEDIUM NOT PRESENT
                    AutoCommandComplete();
                    return;
                }

                if(ReadTimer(&clock) > 4000L)
                {
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    Svr_pause_reqire = TRUE;
                    BUILD_SENSE(0x03,0x02,0x00,0x36);
                    AutoCommandComplete();
                    return;
                }

                if(btmp<32)
                {   //get PHO, BHO, RRFLP 32 times
                    adPHO += ReadSingleAdcmV(ADC_PHSO);
                    adBHO += ReadSingleAdcmV(ADC_BHSO);
                    adREF += ReadSingleAdcmV(ADC_MUXOUT2); //select VREFB
                    btmp++;
                }

                if(WAIT_PI_PO == 1)
                {
                    temp_bktobuf = READ_FIELD(HAL_DVDD_BKTOBUF);

                    if(temp_bktobuf <10)
                    {
                        WRITE_FIELD(HAL_DVDD_BLKINC,32);
                    }
                    if(temp_bktobuf > MAX_VALUE_OF_BLKINC)
                    {
                        temp_bktobuf = MAX_VALUE_OF_BLKINC;
                        WRITE_FIELD(HAL_DVDD_BLKINC,temp_bktobuf);
                    }
                    while (btmp<32)
                    {   //get PHO, BHO, RRFLP 32 times
                        adPHO += ReadSingleAdcmV(ADC_PHSO);
                        adBHO += ReadSingleAdcmV(ADC_BHSO);
                        adREF += ReadSingleAdcmV(ADC_MUXOUT2); //select VREFB
                        btmp++;
                    }
                    break;
                }
                else if((ReadTimer(&clock2) > clk2_timeout) || DVD_Decode_Error || (ANEW_SEEK == TRUE))
                {
                    if ( DVD_BUFFERING() == ON )
                    {
                        dvdStopDiscTransfer();
                    }

                    WAIT_PI_PO = 0;

                    if(DVD_Decode_Error)
                    {
                        ftimeout = FALSE;
                        decodeErr = 1;
                    }
                    else
                    {
                        ftimeout = TRUE;
                        decodeErr = 0;
                    }

                    if(ANEW_SEEK == TRUE)
                    {
                        ftimeout = FALSE;
                        decodeErr = 0;
                    }

                    if(dvdErrorFlag.fUNCOR_ECC)
                    {
                        dvdErrorFlag.fUNCOR_ECC = 0;
                    }
                    else if(dvdErrorFlag.fEDC_ERROR)
                    {
                        dvdErrorFlag.fEDC_ERROR = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 1111;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if(dvdErrorFlag.fSYNC_LOST)
                    {
                        dvdErrorFlag.fSYNC_LOST = 0;
                        PO_UN_C = 2222;
                        WAIT_PI_PO = 1;
                        Last_PSN = Last_PSN + 0x10L;
                        PIPO_CURRENT_ID.Long = Last_PSN;

                    }
                    else if(dvdErrorFlag.fSEQ_ID_ERROR)
                    {
                        dvdErrorFlag.fSEQ_ID_ERROR = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 3333;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if (dvdErrorFlag.fTID_MISS)
                    {
                        dvdErrorFlag.fTID_MISS = 0;
                        if(PO_UN_C == 0)
                        {
                            WAIT_PI_PO = 1;
                        }

                        PIPO_CURRENT_ID.Long = Last_PSN;    //show correct PSN
                    }
                    else if (dvdErrorFlag.fID_MISMATCH)
                    {
                        dvdErrorFlag.fID_MISMATCH = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 6666;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if (dvdErrorFlag.fID_ERROR)
                    {
                        dvdErrorFlag.fID_ERROR = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 7777;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if (dvdErrorFlag.fSEEK_FAIL)
                    {
                        dvdErrorFlag.fSEEK_FAIL = 0;
                        if(PO_UN_C == 0)
                        {
                            PO_UN_C = 8888;
                            WAIT_PI_PO = 1;
                        }
                    }
                    else if(ftimeout)
                    {
                        BOOL PIPOZero;
                        ftimeout = FALSE;
                        PIPOZero = FALSE;
                        Last_PSN = Last_PSN + 0x10L;
                        PIPO_CURRENT_ID.Long = Last_PSN;

                        if(PIPOZero)
                        {
                            PI_C = 0000;
                            PO_C = 0000;
                            PI_UN_C = 0000;
                            PO_UN_C = 0000;
                        }
                        else
                        {
                            PI_C = 9999;
                            PO_C = 9999;
                            PI_UN_C = 9999;
                            PO_UN_C = 9999;
                        }
                        WAIT_PI_PO = 1;
                    }
                    else
                    {
                        if(ANEW_SEEK == FALSE)
                        {
                            PO_UN_C = 5555;
                            WAIT_PI_PO = 1;
                        }
                    }

                    dvdStopDiscTransfer();

                    InterruptState = GET_INT_ENABLE_MASK();
                    DISABLE_INTERRUPT();

                    if (ANEW_SEEK)
                    {
                        CurrentWblPSN = ANEW_SEEK_Target;
                        Last_PSN  = ANEW_SEEK_Target;
                    }
                    else
                        CurrentWblPSN = Last_PSN + 0x10L;

                    dvdramLastDecodedLSN= ConvertPSNToLSN(CurrentWblPSN) - 1;
                    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
                    dvdramDecodedStrPSN = CurrentWblPSN;
                    #endif

                    if((ReadTimer(&clock2) > clk2_timeout) || DVD_Decode_Error )
                    {
                        //dvdDspSystemInit();     //protection for seek in ICE area
                    }

                    if(pdbGetDVDLayers() == DUAL_LAYER)
                    {
                        if(pdbGetDVDTrackPath() == DVDHD_OTP_DISC)
                        {
                            if((CurrentWblPSN > pdbGetDVDMaxPSN(0)) && (CurrentWblPSN < pdbGetDVDStartPSN(1)))
                            {
                                add_length = CurrentWblPSN - pdbGetDVDMaxPSN(0);
                                CurrentWblPSN = pdbGetDVDStartPSN(1) + add_length-1;
                            }
                        }
                        else /* PTP disc*/
                        {
                            if(CurrentWblPSN > pdbGetDVDMaxPSN(0))
                            {
                                add_length = CurrentWblPSN - pdbGetDVDMaxPSN(0);
                                CurrentWblPSN = pdbGetDVDStartPSN(1) + add_length-1;
                            }
                        }
                    }
                    SET_DISC_BUFFER_PTR(DataBuffer.CurDiscPtr);
                    SET_INT_ENABLE_MASK(InterruptState);

                    SET_TARGET_ID(CurrentWblPSN);
                    SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);

                    dvdResetDiscFlags();
                    CLEAR_DVD_DECODER_STATUS();
                    START_DVD_BLOCK_TIMER(FIRST_BLOCK_TIMEOUT_VALUE);
                    ENABLE_DVD_DECODER_INTS();

                    if (CurrentWblPSN > 0x800000)
                        dvdDiscRequest.TargetLayer = eLayer1;
                    else
                        dvdDiscRequest.TargetLayer = eLayer0;
                    ANEW_SEEK = FALSE;
                    if( (dvdSeek(CurrentWblPSN ,dvdDiscRequest.TargetLayer)) == Error)
                    {
                        WRITE_FIELD(HAL_DVDD_ECCCNT,DVDRAM_MULTI_PASS_COUNT);
                        Svr_pause_reqire = TRUE;
                        Send_timef = 0;
                        BUILD_SENSE(0x03,0x11,0x0B,0x32);               // UNRECOVERED READ ERROR
                        AutoCommandComplete();         // Send Command Complete to host if requested by calling function
                        return;
                    }
                    else
                    {
                        ENABLE_DVD_DECODER_INTS();

                        dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
                        CLEAR_SYNC_LOST_STATUS();

                        temp_Hstbuf = READ_FIELD(HAL_DVDD_CURECC);
                        temp_Hstbuf += 5*ONE_DVD_BLOCK;     /* Advance the  pointer */
                        temp_Hstbuf %= MAX_DVD_SECTORS;   /* Insure buffer wrap */

                        WRITE_FIELD(HAL_HST_BUFPGCMP, temp_Hstbuf);

                        ENABLE_DVD_BUFFERING();
                        StartTimer(&clock2);
                    }
                }

                ExitProcess();
            }//while(1)

            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            PIPO_CURRENT_ID.byte.u = 0;

            if(DVD_Decode_Error || decodeErr)
            {
                ULONG curZoneEndPSN, nextZoneStartPSN, nextZoneStartLSN;
                BYTE CurZone;
                CurZone = DVDRAM_PSN2ZONE(Last_PSN);

                nextZoneStartLSN = SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[CurZone+1]); //next zone start
                curZoneEndPSN = ConvertLSNToPSN(nextZoneStartLSN - 1);
                nextZoneStartPSN = ConvertLSNToPSN(nextZoneStartLSN);

                //check if Last_DVD_PSN is in guard zone
                if(curZoneEndPSN < Last_PSN && Last_PSN < nextZoneStartPSN) //seek position inside guard zone
                {
                    send_msg5SValue(SEND_MRW_MSG, "Chg Tgt", 4, Last_PSN);
                    Last_PSN = nextZoneStartPSN;
                }
                else if (Last_PSN == DVDRAMZoneTable[CurZone].StartUserSectorPSN)
                {
                    Last_PSN = DVDRAMZoneTable[CurZone].StartUserSectorPSN;
                    send_msg5SValue(1, "New Zone", 4, Last_PSN);
                }
                else
                {
                    Last_PSN = Last_PSN + 0x10L;
                    send_msg5SValue(1, "LastPSN0", 4, Last_PSN);
                }
                send_msg5SValue(1, "LastPSN1", 4, Last_PSN);

                PIPO_CURRENT_ID.Long = Last_PSN;
                send_msg5SValue(SEND_MRW_MSG,"PSN-Error",4,PIPO_CURRENT_ID.Long);

                /* If this defect has been recorded in SDL, mark it as good one */
                {
                    ULONG * SDLp;
                    ULONG SdlEntry;
                    SDLp = SearchSdlEntry(Last_PSN);
                    SdlEntry = SWAPEND_ULONG(*SDLp) & 0x00FFFFFF;

                    if(SdlEntry == Last_PSN)
                    {
                        send_msg5S(SEND_MRW_MSG, "SDL Defect");
                        PI_C = 0;
                        PI_E = 0;
                        PI_UN_C = 0;
                        PO_C = 0;
                        PO_UN_C = 0;
                    }
                }
            }
            else
            {
                //PIPO_CURRENT_ID.Long is the DataFieldNumber
                if(RWDisc.dvRAM.BLER_ID == eRamData_ID)
                {
                    RWDisc.dvRAM.BLER_ID = eRamHeader_ID;
                    Last_PSN = (RWDisc.dvRAM.CurrentPID & DVD_BLOCK_MASK) - ONE_DVD_BLOCK;
                    send_msg5SValue(SEND_MRW_MSG, "LastPSN5", 4, Last_PSN);
                }
                else
                {
                    send_msg5SValue(SEND_MRW_MSG,"LSN-OK", 4, RWDisc.dvRAM.BLER_ID<<24 | (dvdramLastDecodedLSN & 0x00ffffff));
                    Last_PSN = ConvertLSNToPSN(dvdramLastDecodedLSN & DVD_BLOCK_MASK);
                    send_msg5SValue(1, "LastPSN6", 4, Last_PSN);
                }
                send_msg5SValue(SEND_MRW_MSG,"lastPSN", 4, Last_PSN);
#if (WORKAROUND_FOR_SEEDMISTHR == 1)
                if(READ_FIELD(HAL_DVDD_SEEDMISTHR) == 0)
                {
                    WRITE_FIELD(HAL_DVDD_SEEDMISTHR, 2);
                }
#endif
            }

            //detect guard zone earlier
            if(Addr_mode == 0) //LBA mode
            {
                StLongU LBA;
                LBA.Long = ConvertPSNToLSN(Last_PSN);

                WriteSDRAM(LBA.byte.h);
                WriteSDRAM(LBA.byte.m);
                WriteSDRAM(LBA.byte.l);
            }
            else
            {
                WriteSDRAM(GET_BYTE2(Last_PSN));
                WriteSDRAM(BHIGH(Last_PSN));
                WriteSDRAM(Last_PSN);
            }

            //check zone across
            dvdDiscRequest.TargetID.Long = Last_PSN + 0x10;
            DataBuffer.CacheStartLBA = ConvertPSNToLSN(dvdDiscRequest.TargetID.Long);
            HostXfer.StartBlock.lba = DataBuffer.CacheStartLBA - 1;
        }
#endif  // #if (DVD_RAM_READ == 1)
//#if DVD_WRITE == 1
        else
        {   // DVD+/-R/RW disc
            StartTimer(&clock);
            while (1)
            {
                #if (WOBBLE_ID == 1)
                if (ReadTimer(&clock) > 1000L )
                {
                    CurrentWblPSN = Last_PSN;
                    break;
                }

                if( pdbGetIDIntStatus() == TRUE )
                {
                    pdbSetIDIntStatus(FALSE);
                    if(pdbGetIDOk() == TRUE)
                    {   // Monitor Wobble(ADIP/LPP) Address
                        CurrentWblPSN = pdbGetWblAddress();
                        break;
                    }
                }
                #elif (WOBBLE_ID == 0)
                if (ReadTimer(&clock) > 1000L )
                {
                    CurrentDataPSN = Last_PSN;
                    break;
                }

                if((READ_FIELD(HAL_DVDD_CIDI) == e_READY_CIDI))
                {
                    if(READ_FIELD(HAL_DVDD_CIDVLD) == TRUE)
                    {
                        currentID_temp.Long = READ_REG(HAL_DVDDREG_MAP->CURID0)&PSNMask;
                        CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                        CurrentDataPSN = currentID_temp.Long;
                        break;
                    }
                    CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                }

                if( pdbGetIDIntStatus() == TRUE )
                {
                    pdbSetIDIntStatus(FALSE);
                    if(pdbGetIDOk() == TRUE)
                    {   // Monitor Wobble(ADIP/LPP) Address
                        CurrentDataPSN = pdbGetWblAddress();
                        break;
                    }
                }
                #endif

                ExitProcess();
            }

    #if (WOBBLE_ID == 1)
            Last_PSN = CurrentWblPSN;
            #elif (WOBBLE_ID == 0)
            Last_PSN = CurrentDataPSN;
            #endif

            dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
            CLEAR_SYNC_LOST_STATUS();
            ENABLE_DVD_SYNC_LOST_INT();
            ENABLE_DVD_BUFFERING();

            StartTimer(&clock);
            StartTimer(&clock2);

            btmp  = 0;
            std_beta_modulation  = 0.0f;
            adBHO = 0L;
            adPHO = 0L;
            adREF = 0L;
            ftimeout = 0;
            VcoResetCnt = 0;
            while(1)
            {
                if (EjectKeyInfo.PressedValid)
                {
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    Svr_pause_reqire = TRUE;
                    BUILD_SENSE(0x02,0x3A,0x00,0x30);        // NOT READY, MEDIUM NOT PRESENT
                    AutoCommandComplete();
                    return;
                }

                if(ReadTimer(&clock) > 1000L)
                {
                    ftimeout = 1;
                    StartTimer(&clock);
                }

                if(btmp<32)
                {   //get PHO, BHO, RRFLP 32 times
                    adPHO += ReadSingleAdcmV(ADC_PHSO);
                    adBHO += ReadSingleAdcmV(ADC_BHSO);
                    adREF += ReadSingleAdcmV(ADC_MUXOUT2);
                    btmp++;
                }
#ifdef MEAS_RF_ONLY
                {
                    ad_dsp_t  rf_signal;

                    rf_signal = readRFppFromAGC_1rev(TRUE);
                    modulation = rf_signal.middle_mv;
                    WAIT_PI_PO = 1;
                    break;
                }
#endif
                if((WAIT_PI_PO == 1) && (ResetDecoder == FALSE))
                {
                    SHORT temp_bktobuf;

                    temp_bktobuf = READ_FIELD(HAL_DVDD_BKTOBUF);
                    if(temp_bktobuf <10)
                    {
                        WRITE_FIELD(HAL_DVDD_BLKINC,32);
                    }
                    if(temp_bktobuf > MAX_VALUE_OF_BLKINC)
                    {
                        temp_bktobuf = MAX_VALUE_OF_BLKINC;
                        WRITE_FIELD(HAL_DVDD_BLKINC,temp_bktobuf);
                    }
                    while (btmp<32)
                    {
                        adPHO += ReadSingleAdcmV(ADC_PHSO);
                        adBHO += ReadSingleAdcmV(ADC_BHSO);
                        adREF += ReadSingleAdcmV(ADC_MUXOUT2); //select VREFB
                        btmp++;
                    }

                    if(PO_UN_C == 0)
                    {
                        if(dvdErrorFlag.fUNCOR_ECC)
                        {
                            dvdErrorFlag.fUNCOR_ECC = 0;
                        }
                        else if(dvdErrorFlag.fEDC_ERROR)
                        {
                            dvdErrorFlag.fEDC_ERROR = 0;
                            PO_UN_C = 1111;
                            WAIT_PI_PO = 1;
                        }
                        else if(dvdErrorFlag.fSEQ_ID_ERROR)
                        {
                            dvdErrorFlag.fSEQ_ID_ERROR = 0;
                            PO_UN_C = 3333;
                            WAIT_PI_PO = 1;
                        }
                        else if (dvdErrorFlag.fTID_MISS)
                        {
                            dvdErrorFlag.fTID_MISS = 0;
                            PO_UN_C = 4444;
                            WAIT_PI_PO = 1;
                        }
                        else if (dvdErrorFlag.fID_MISMATCH)
                        {
                            dvdErrorFlag.fID_MISMATCH = 0;
                            PO_UN_C = 6666;
                            WAIT_PI_PO = 1;
                        }
                        else if (dvdErrorFlag.fID_ERROR)
                        {
                            dvdErrorFlag.fID_ERROR = 0;
                            PO_UN_C = 7777;
                            WAIT_PI_PO = 1;
                        }
                        else if (dvdErrorFlag.fSEEK_FAIL)
                        {
                            dvdErrorFlag.fSEEK_FAIL = 0;
                            PO_UN_C = 8888;
                            WAIT_PI_PO = 1;
                        }
                    }

                    break;
                }
                else if((ReadTimer(&clock2) > 200L)||(dvdErrorFlag.fSYNC_LOST) || (ResetDecoder == TRUE)||(ftimeout))
                {
                    WAIT_PI_PO = 0;
                    ResetDecoder = FALSE;

                    if ( DVD_BUFFERING() == ON )
                    {
                        dvdStopDiscTransfer();
                    }

                    if (dvdErrorFlag.fSYNC_LOST == 1)
                    {
                        dvdErrorFlag.fSYNC_LOST = 0;

                        if(HAL_GLOBAL_GetRfPresent() == 1)
                        {
                            PO_UN_C = 2222;
                        }
                        else
                        {
                            PI_C = 32767;
                            PO_C = 182;
                            PI_UN_C = 32767;
                            PO_UN_C = 182;
                        }
                        WAIT_PI_PO = 1;
                    }

                    if(ftimeout)
                    {
                        ftimeout = 0;
                        if(HAL_GLOBAL_GetRfPresent() == 1)
                        {
                            PO_UN_C = 9999;
                        }
                        else
                        {
                            PI_C = 32767;
                            PI_E = 208;
                            PO_C = 182;
                            PI_UN_C = 32767;
                            PO_UN_C = 182;
                        }
                        WAIT_PI_PO = 1;
                    }

                    if(HAL_GLOBAL_GetRfPresent() == 1)
                    {
                        SetDfeWblFrontOnHoldTracking();
                        SetDfeWblFrontSemiSynchrAfterTracking();
                        Delay100uS(1);
                        SetDfeWblFrontBitSynchr();

                        VcoResetCnt++;
                        if(VcoResetCnt == 5)
                        {
                           WRITE_FIELD(HAL_DVDD_RSTDVDD, 1);
                           WRITE_FIELD(HAL_DVDD_RSTDVDD, 0);
                        }
                        send_msg80(SEND_ATAPI_CMD,0x710022);
                    }

                    ENABLE_DVD_DECODER();    /* Turn on DVD decoder */

                    WRITE_FIELD(HAL_DVDD_TIDDIS, TRUE);        /* enable target id */

                    dvdResetDiscFlags();

                    SET_DISC_BUFFER_PTR(0);
                    SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);
                    CLEAR_DVD_DECODER_STATUS();
                    ENABLE_DVD_DECODER_INTS();
                    ENABLE_DVD_BUFFERING();
                    StartTimer(&clock2);
                }

                ExitProcess();

            }//while(1)


            Iram_Buf_dram_addr = BUFFER_SCRATCH;

        }//if (svoVar_discKindT.Bit.Media == eMediaROM)
//#endif // DVD_WRITE

        if(btmp)
        {
            adPHO /= btmp; //calculate PHO 32 times average value.
            adBHO /= btmp; //calculate BHO 32 times average value.
            adREF /= btmp; //calculate RRFLP 32 times average value.

            if (0)//(svoVar_discKindT.Bit.Media == eMediaRW) //modulation can not be measured accurate due to not calculate dark level
            {
                //modulation formula : (PHO-BHO)/PHO
                //Improve method : (PHO-BHO)/(PHO-Vrefmb)
                //adREF = svoRRFVar.dark[Layer_Index];
                std_beta_modulation = ((float)adPHO - (float)adBHO)/((float)adPHO - (float)adREF);    //modulation
            }
            else//RDisc
            {
                if(adBHO<1500L)
                    std_beta_modulation = ((float)adPHO + (float)adBHO - (float)(adREF*2.0f))/((float)adPHO - (float)adBHO);    //Beta
                else
                    std_beta_modulation = -1.0f;
            }
        }
        else
            std_beta_modulation = -2.0f;

        tempData = (int)((std_beta_modulation) * 1000.0f);

        if (svoVar_discKindT.Bit.Media == eMediaR)
            tempData += svoPcalVar.dvBetaOffset;

        if(SkipErrRateNum)
        {
            iBeta_modulation[0] = iBeta_modulation[1] = iBeta_modulation[2] = tempData;
        }
        else
        {
            iBeta_modulation[0] = iBeta_modulation[1];
            iBeta_modulation[1] = iBeta_modulation[2];
            iBeta_modulation[2] = tempData;
        }
        tempData = (iBeta_modulation[0]+iBeta_modulation[1]+iBeta_modulation[2])/3;

        if (svoVar_discKindT.Bit.Media == eMediaROM)
        { // DVD-ROM disc
            if(Addr_mode == 0) //LBA mode
            {
                if(dvdDiscRequest.TargetLayer == eLayer1)
                    PIPO_CURRENT_ID.Long = PIPO_CURRENT_ID.Long - pdbGetDVDStartPSN(1) + pdbGetDVDEndLBALayer0() + 1L;
                else
                    PIPO_CURRENT_ID.byte.h -= 0x03;
            }

            WriteSDRAM( PIPO_CURRENT_ID.byte.h);
            WriteSDRAM( PIPO_CURRENT_ID.byte.m);
            WriteSDRAM( PIPO_CURRENT_ID.byte.l);
        }
#if (DVD_RAM_READ == 1)
        else if (svoVar_discKindT.Bit.Media == eMediaRAM)
        { // DVD-RAM
            /* move code to end of dvd-ram buffering.
               For disable buffering in guard zone earlier */
        }
#endif

//#if DVD_WRITE == 1
        else
        {
            if(Addr_mode == 0) //LBA mode
            {
    #if (WOBBLE_ID == 1)
                if(dvdDiscRequest.TargetLayer == eLayer1)
                    CurrentWblPSN= CurrentWblPSN - pdbGetDVDStartPSN(1) + pdbGetDVDEndLBALayer0() + 1L;
                else
                    CurrentWblPSN -= 0x030000L;
    #elif (WOBBLE_ID == 0)
                if(dvdDiscRequest.TargetLayer == eLayer1)
                    CurrentDataPSN= CurrentDataPSN - pdbGetDVDStartPSN(1) + pdbGetDVDEndLBALayer0() + 1L;
                else
                    CurrentDataPSN -= 0x030000L;
    #endif
            }

    #if (WOBBLE_ID == 1)
            WriteSDRAM( CurrentWblPSN >> 16);
            WriteSDRAM( CurrentWblPSN >> 8);
            WriteSDRAM( CurrentWblPSN);
    #elif (WOBBLE_ID == 0)
            WriteSDRAM( CurrentDataPSN >> 16);
            WriteSDRAM( CurrentDataPSN >> 8);
            WriteSDRAM( CurrentDataPSN);
    #endif
        }
//#endif

        if(SkipErrRateNum)
        {
            SkipErrRateNum--;
            WriteSDRAM( 0);
            WriteSDRAM( 0);
            WriteSDRAM( 0);
            WriteSDRAM( 0);
            WriteSDRAM( 0);
            WriteSDRAM( 0);
            WriteSDRAM( 0);
            WriteSDRAM( 0);
        }
        else
        {
            WriteSDRAM(BHIGH(PI_C));
            WriteSDRAM(BLOW(PI_C));
            WriteSDRAM(BHIGH(PO_UN_C));
            WriteSDRAM(BLOW(PO_UN_C));
            WriteSDRAM(BHIGH(PI_UN_C));
            WriteSDRAM(BLOW(PI_UN_C));
            WriteSDRAM(BHIGH(PO_C));
            WriteSDRAM(BLOW(PO_C));
        }
        WriteSDRAM(Iram_AtipAdipLppErrorAmount);
        WriteSDRAM(tempData>>8); //Beta
        WriteSDRAM(tempData);    //Beta
        WAIT_PI_PO = 0;
#if (USE_REALTIME_JITTER == 1)
    #if (DVD_RAM_READ == 1)
        if (svoVar_discKindT.Bit.Media == eMediaRAM)
        {
            jitterResult_t extJitterResult;

            //measure jitter
            tempData = hifRealtimeJitterMeasurement(&extJitterResult);
            if (extJitterResult.extResult.land > 255)
                extJitterResult.extResult.land = 0;
            if (extJitterResult.extResult.groove > 255)
                extJitterResult.extResult.groove = 0;
            disableRTJitterMeas();
            WriteSDRAM((BYTE)extJitterResult.extResult.land);
            WriteSDRAM((BYTE)extJitterResult.extResult.groove);
        }
        else
    #endif /* #if (DVD_RAM_READ == 1) */
        {
#ifdef MEAS_RF_ONLY
            tempData = modulation;
#else
            //measure jitter
            tempData = HAL_DFE_ReadJitterX10();
#endif
            WriteSDRAM(tempData>>8);
            WriteSDRAM(tempData);
        }
#else
        WriteSDRAM(0);
        WriteSDRAM(0);
#endif /* USE_REALTIME_JITTER */

        if(SkipErrRateNum)
        {
            WriteSDRAM(0);
            WriteSDRAM(0);
        }
        else
        {
            WriteSDRAM(BHIGH(PI_E));
            WriteSDRAM(BLOW(PI_E));
        }
        /* set up parameters and xfer mechenism status data to host */
        SendParameterDataToHost(BUFFER_SCRATCH, 18, TRUE);  /* xfer data and post cmd complete */
    }
    else //CD_DISC
    {
    /* for Atip /qcode time the difference (in distance) */
        BYTE  Cur_time,btmp1,btmp_time;
        TimeS Qtime,time;
        long     adTE,adFE;
        ad_dsp_t te_signal,fe_signal;

        if (Iram_Cmd_block.bf[4] > 2)
        {
            svoIramVar.subQEnableFlag = 0;
            Send_timef = 0;
            BUILD_SENSE(0x05,0x24,0x00,0x57);              /* INVALID PARAMETER IN CDB */
            AutoCommandComplete();
            return;
         }

        Svr_pause_reqire = FALSE;

        if (Iram_Cmd_block.bf[5] == 0x0A)
        {
            Send_timef = 0;
            Svr_pause_reqire = TRUE;
            #if (SUN_FUNC_TEST == 1)
            READ_C1_C2 = 0;
            #endif
            svoIramVar.subQEnableFlag = 0;
            St_diag_ena = FALSE;        /* Diagnostics Disabled */
            AutoCommandComplete();
            return;
        }

        if(SkipErrRateNum)
        {
        #if (CHIP_REV >= 0xC0)
            sig.mux1 = halAFE_MUX1ADSEL_VREFFCT;
        #else
            sig.mux1 = halAFE_MUX1ADSEL_VREF_FCTS;
        #endif
            ServoADCSignal = AfeRouteSignalToAD(AFE_MUXTOAD1, sig);
        }

    //  WriteSCReg_1B(rwEncDec, 0x00);
        WRITE_REG(HAL_CDDREG_MAP->C12CCR, HAL_CDD_C1UNC);   // count any C1 error + C2 uncorrectable only

        {   //non atip disc
            pdbSetSubQIntStatus(FALSE);
            StartTimer(&clock);

            while(1)
            {
                if(pdbGetSubQIntStatus())
                {
                    pdbGetSubQCode(&subcd);
                    if(subcd.Tno!=0)//In Data Area
                        break;
                    else//In Lead-in Area
                        pdbSetSubQIntStatus(FALSE);
                }
                if(ReadTimer(&clock) > 4000L) {
                    BUILD_SENSE(0x03,0x02,0x00,0x38);

                    Send_timef = 0;
                    Svr_pause_reqire = TRUE;
                    svoIramVar.subQEnableFlag = 0;
                    AutoCommandComplete();
                    return;
                }

                if (EjectKeyInfo.PressedValid)
                {
                    BUILD_SENSE(0x02,0x3A,0x00,0x32);        // NOT READY, MEDIUM NOT PRESENT

                    Send_timef = 0;
                    svoIramVar.subQEnableFlag = 0;
                    AutoCommandComplete();
                    return;
                }
                ExitProcess();
            }

            /* for Atip /qcode time the difference (in distance) */
            pdbGetABSTime(&time);
            pdbSetSubQIntStatus(FALSE);
            Cur_time= time.Second;
        }

        btmp_time = Cur_time;

        std_beta_modulation  = 0.0f;
        btmp  = 0;
        adBHO = 0L;
        adPHO = 0L;
        adREF = 0L;
        adTE  = 0L;
        adFE  = 0L;

        /* TE/FE vendor command */
        if (Iram_Cmd_block.bf[4] == 2)
        { //Read TE/FE signal Function
            if ((BCD2HEX(Cur_time)+30)>=60)
            {
                Cur_time=HEX2BCD(BCD2HEX(Cur_time)+30-60);
            }
            else
            {
                Cur_time=HEX2BCD(BCD2HEX(Cur_time)+30);
            }

            do
            {
                if(ReadTimer(&clock) > 6000L)
                {
                    BUILD_SENSE(0x03,0x02,0x00,0x39);
                    Svr_pause_reqire = TRUE;
                    svoIramVar.subQEnableFlag = 0;
                    AutoCommandComplete();
                    return;
                }
        /* for Atip /qcode time the difference (in distance) */
                {   //non atip disc
                    StartTimer(&clock3);
                    pdbSetSubQIntStatus(FALSE);

                    do
                    {
                        if(ReadTimer(&clock3) > 1000L)
                            break;
                        ExitProcess();
                    }while (!pdbGetSubQIntStatus());

                    pdbGetABSTime(&time);
                    Qtime = time;
                    btmp_time = time.Second;
                }

                if (EjectKeyInfo.PressedValid)
                {
                    BUILD_SENSE(0x02,0x3A,0x00,0x33);        // NOT READY, MEDIUM NOT PRESENT
                    svoIramVar.subQEnableFlag = 0;
                    AutoCommandComplete();
                    return;
                }

                if (btmp1<32)
                {   //TEp2p ,FEp2p 32 times
                    te_signal = ReadDSPADCmV(NO_REVOLUTION, TE_ALL, TRUE);
                    fe_signal = ReadDSPADCmV(NO_REVOLUTION, FE_ALL, TRUE);

                    if ((fe_signal.errcode == eNOERR) && (te_signal.errcode == eNOERR))
                    {
                        adTE += te_signal.p2p_mv;
                        adFE += fe_signal.p2p_mv;
                        btmp1++;
                    }
                }
                ExitProcess();
            }while(Cur_time != btmp_time);

            if (btmp1)
            {
                adTE /=btmp1;
                adFE /=btmp1;
            }
        }
        else
        {
            do
            {
                if(ReadTimer(&clock) > 6000L)
                {
                    BUILD_SENSE(0x03,0x02,0x00,0x3a);
                    /* for send Qcode */

                    Send_timef = 0;

                    Svr_pause_reqire = TRUE;
                    svoIramVar.subQEnableFlag = 0;
                    AutoCommandComplete();
                    return;
                }
                {   // non atip disc
                    StartTimer(&clock3);
                    pdbSetSubQIntStatus(FALSE);

                    do
                    {
                        if(ReadTimer(&clock3) > 1000L)
                            break;
                        ExitProcess();
                    }while (!pdbGetSubQIntStatus());

                    pdbGetABSTime(&time);
                    Qtime = time;
                    btmp_time = time.Second;
                }

                if (EjectKeyInfo.PressedValid)
                {
                    BUILD_SENSE(0x02,0x3A,0x00,0x34);        // NOT READY, MEDIUM NOT PRESENT

                    Send_timef = 0;

                    svoIramVar.subQEnableFlag = 0;
                    AutoCommandComplete();
                    return;
                }

                if(btmp<32)
                {   //get PHO, BHO, RRFLP 32 times
                    adPHO += ReadSingleAdcmV(ADC_PHSO);
                    adBHO += ReadSingleAdcmV(ADC_BHSO);
                    adREF += ReadSingleAdcmV(ServoADCSignal);
                    btmp++;
                }

                ExitProcess();
            }while(Cur_time == btmp_time);

            if(btmp)
            {
                adPHO /= btmp; //calculate PHO 32 times average value.
                adBHO /= btmp; //calculate BHO 32 times average value.
                adREF /= btmp; //calculate RRFLP 32 times average value.

                if (svoVar_discKindT.Bit.Media == eMediaRW)
                {
                    //modulation formula : (PHO-BHO)/PHO
                    //Improve method : (PHO-BHO)/(PHO-Vrefmb)
                    std_beta_modulation = ((float)adPHO - (float)adBHO)/((float)adPHO - (float)adREF);    //modulation
                }
                else//RDisc
                {
                    if(adBHO<1500L)
                        std_beta_modulation = ((float)adPHO + (float)adBHO - (float)(adREF*2.0f))/((float)adPHO - (float)adBHO);    //Beta
                    else
                        std_beta_modulation = -1.0f;
                }
            }
            else
                std_beta_modulation = -2.0f;

            tempData = (int)((std_beta_modulation) * 1000.0f);

            if (svoVar_discKindT.Bit.Media == eMediaR)
                tempData += svoPcalVar.cdBetaOffset;

            if(SkipErrRateNum)
            {
                iBeta_modulation[0] = iBeta_modulation[1] = iBeta_modulation[2] = tempData;
            }
            else
            {
                iBeta_modulation[0] = iBeta_modulation[1];
                iBeta_modulation[1] = iBeta_modulation[2];
                iBeta_modulation[2] = tempData;
            }

            tempData = (iBeta_modulation[0]+iBeta_modulation[1]+iBeta_modulation[2])/3;

        }

        Iram_Buf_dram_addr = BUFFER_SCRATCH;

        if(Iram_Cmd_block.bf[4] == 1)
        {
            WriteSDRAM( BCD2HEX(time.Minute));
            WriteSDRAM( BCD2HEX(time.Second));
            WriteSDRAM( BCD2HEX(time.Frame));

            if(SkipErrRateNum)
            {
                SkipErrRateNum--;
                WriteSDRAM( 0);
                WriteSDRAM( 0);
                WriteSDRAM( 0);
                WriteSDRAM( 0);
            }
            else
            {
                btmp = READ_REG(HAL_CDDREG_MAP->C1EPSCRL);
                WriteSDRAM(BHIGH(btmp));
                WriteSDRAM(BLOW(btmp));
                btmp = READ_REG(HAL_CDDREG_MAP->C2EPSCRL);
                WriteSDRAM(BHIGH(btmp));
                WriteSDRAM(BLOW(btmp));
            }

            WriteSDRAM( BCD2HEX(Qtime.Minute));
            WriteSDRAM( BCD2HEX(Qtime.Second));
            WriteSDRAM( BCD2HEX(Qtime.Frame));
            WriteSDRAM( 0 );
            WriteSDRAM( 0 );
            /* set up parameters and xfer mechenism status data to host */
            SendParameterDataToHost(BUFFER_SCRATCH,12 , TRUE);
    /* Atip /qcode time the difference (in distance) */
    /* TE /FE vendor command */
        }
        else if(Iram_Cmd_block.bf[4] == 2)
        {
            WriteSDRAM( BCD2HEX(time.Minute));
            WriteSDRAM( BCD2HEX(time.Second));
            WriteSDRAM( BCD2HEX(time.Frame));
            WriteSDRAM(adTE); //TE
            WriteSDRAM(adTE>>8);
            WriteSDRAM(adTE>>16);
            WriteSDRAM(adTE>>24);

            WriteSDRAM(adFE); //FE
            WriteSDRAM(adFE>>8);
            WriteSDRAM(adFE>>16);
            WriteSDRAM(adFE>>24);

            adTE=1200;
            WriteSDRAM(adTE);    //TEp-p=1200mV
            WriteSDRAM(adTE>>8);
            adFE=2000;    //FEp-p=2000mV
            WriteSDRAM(adFE);
            WriteSDRAM(adFE>>8);
            WriteSDRAM( 0);

            SendParameterDataToHost(BUFFER_SCRATCH,16 , TRUE);
    /* for TE /FE vendor command */
        }
        else //if(Iram_Cmd_block.bf[4] == 0)
        {
            WriteSDRAM( BCD2HEX(time.Minute));
            WriteSDRAM( BCD2HEX(time.Second));
            WriteSDRAM( BCD2HEX(time.Frame));

            if(SkipErrRateNum)
            {
                SkipErrRateNum--;
                WriteSDRAM( 0);
                WriteSDRAM( 0);
                WriteSDRAM( 0);
                WriteSDRAM( 0);
                WriteSDRAM( 0);
            }
            else
            {
                btmp = READ_REG(HAL_CDDREG_MAP->C1EPSCRL);
                WriteSDRAM(BHIGH(btmp));
                WriteSDRAM(BLOW(btmp));
                btmp = READ_REG(HAL_CDDREG_MAP->C2EPSCRL);
                WriteSDRAM(BHIGH(btmp));
                WriteSDRAM(BLOW(btmp));

    #if (RESET_PLL_FOR_HUGEPIC1==1)
                if (BCD2HEX(time.Minute) > (resetmin+1))
                {
                    resetmin=BCD2HEX(time.Minute);
                    resetsec=BCD2HEX(time.Second);
        #if (CD_DFE_IMPROVE_READ == 1)
                    dfe_retry_count = 0;
        #endif
                    if (btmp>500)//Get large C2
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x420009); //Huge C2
                        //SendMsg5SValue(SHOW_DEBUG_MSG, "MSFtotal", 4, BCD2HEX(time.Minute)<<8+BCD2HEX(time.Second)<<4+BCD2HEX(time.Frame));

                        SetDfeWblFrontOnHoldTracking();
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthHigh);
                        SetDfeWblFrontSemiSynchrAfterTracking();

                        Delay100uS(5);
                        if (HAL_DFE_GetPllInLock())
                        {
                            //unreset Fast PLL, enable Black and white defect detection and put back the HPF in the processing path
                            SetDfeWblFrontBitSynchr();
                        }
                        else
                        {
                            Delay100uS(5);
                            SetDfeWblFrontBitSynchr();
                        }
                        SkipErrRateNum = 1;
                        Iram_Buf_dram_addr = BUFFER_SCRATCH;
                        WriteSDRAM( BCD2HEX(time.Minute));
                        WriteSDRAM( BCD2HEX(time.Second));
                        WriteSDRAM( BCD2HEX(time.Frame));
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        /* send int (4 byte) to host */
                        WriteSDRAM(tempData>>8);
                        WriteSDRAM(tempData);
                #if (USE_REALTIME_JITTER == 1)
                        //measure jitter
                        tempData = 0x0A;//HAL_DFE_ReadJitterX10();
                        WriteSDRAM(tempData>>8);
                        WriteSDRAM(tempData);
                #else
                        WriteSDRAM(0);
                        WriteSDRAM(0);
                #endif /* USE_REALTIME_JITTER */
                        /* set up parameters and xfer mechenism status data to host */
                        SendParameterDataToHost(BUFFER_SCRATCH,12 , TRUE);
                        return;
                    }
                }
        #if (CD_DFE_IMPROVE_READ == 1)
                if(btmp > 0)
                {
                    if(++dfe_retry_count < MAX_KPKI_CHG)
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x42000A); //cal KPPICFG
                        if ((DFE_CHG_CASE&0x01)== 0)
                        {
                            DFE_CHG_SEL++;
                            DFE_CHG_CASE |= 0x01;
                            LAL_DFE_SetBwFastPll();
                        }
                        SkipErrRateNum = 0;
                        Iram_Buf_dram_addr = BUFFER_SCRATCH;
                        WriteSDRAM( BCD2HEX(time.Minute));
                        WriteSDRAM( BCD2HEX(time.Second));
                        WriteSDRAM( BCD2HEX(time.Frame));
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        WriteSDRAM( 0);
                        /* send int (4 byte) to host */
                        WriteSDRAM(tempData>>8);
                        WriteSDRAM(tempData);
                #if (USE_REALTIME_JITTER == 1)
                        //measure jitter
                        tempData = 0x0A;//HAL_DFE_ReadJitterX10();
                        WriteSDRAM(tempData>>8);
                        WriteSDRAM(tempData);
                #else
                        WriteSDRAM(0);
                        WriteSDRAM(0);
                #endif /* USE_REALTIME_JITTER */
                        /* set up parameters and xfer mechenism status data to host */
                        SendParameterDataToHost(BUFFER_SCRATCH,12 , TRUE);
                        return;
                    }
                }
        #endif//(CD_DFE_IMPROVE_READ == 1)

    #endif//#if (RESET_PLL_FOR_HUGEPIC1==1)
                WriteSDRAM( Iram_AtipAdipLppErrorAmount );
            }

            /* send int (4 byte) to host */
            WriteSDRAM(tempData>>8);
            WriteSDRAM(tempData);
#if (USE_REALTIME_JITTER == 1)
            //measure jitter
            tempData = HAL_DFE_ReadJitterX10();
            WriteSDRAM(tempData>>8);
            WriteSDRAM(tempData);
#else
            WriteSDRAM(0);
            WriteSDRAM(0);
#endif /* USE_REALTIME_JITTER */
            /* set up parameters and xfer mechenism status data to host */
            SendParameterDataToHost(BUFFER_SCRATCH,12 , TRUE);
        }
    }// else    //CD_DISC
}


//void diaServoAdjustCommand()
//{
//    if ( Iram_Cmd_block.bf[3]==1 )
//        Radj_enable = TRUE;
//    else
//        Radj_enable = FALSE;
//
//    switch (Iram_Cmd_block.bf[2])  //Select Command Mode
//    {
//        case 0:   //EQRF Adjustment Mode
//            if (Radj_enable == TRUE)
//            {
//
//            }
//            break;
//
//        case 1:   //Adjustment any reg Mode
//            if (Radj_enable == TRUE)
//            {
//                Radj_addr1 = (Iram_Cmd_block.bf[4] << 8) | Iram_Cmd_block.bf[5];
//                Radj_mask1 = Iram_Cmd_block.bf[6];
//                Radj_value1 = Iram_Cmd_block.bf[7];
//                Radj_addr2 = (Iram_Cmd_block.bf[8] << 8) | Iram_Cmd_block.bf[9];
//                Radj_mask2 = Iram_Cmd_block.bf[10];
//                Radj_value2 = Iram_Cmd_block.bf[11];
//            }
//            break;
//
//        default:
//            BUILD_SENSE(0x05,0x24,0x00,0x58);    /* INVALID FIELD IN COMMAND PACKET */
//            return;
//    }
//    AutoCommandComplete();
//}

#if (USE_REALTIME_JITTER == 1)


__inline static void hifDiaGenericMeasureCmd(void)
{
    switch (Iram_Cmd_block.bf[2])               /* Select Generic Measure Command  */
    {
        case eGENERIC_JITTER_CMD:
            hifDiaGenericJitterCmd();
            break;
        case eGENERIC_BLER_CMD:                 /* still to do */
        case eGENERIC_BETA_CMD:                 /* still to do */
        case eGENERIC_ADER_CMD:                 /* still to do */
            break;

        default:
            BUILD_SENSE(0x05,0x24,0x00,0x58);   /* INVALID FIELD IN COMMAND PACKET */
            AutoCommandComplete();
            break;
    }
}

__inline static void hifDiaGenericJitterCmd(void)
{
    ULONG rHFFPLLCFG, rHFFPLLCFG_sav;

    rHFFPLLCFG = rHFFPLLCFG_sav = READ_REG(HAL_DFEREG_MAP->HFFPLLCFG);
    WRITE_FIELDTO(rHFFPLLCFG, HAL_DFE_FPLLWTEN, ((Iram_Cmd_block.bf[1]>>7) & 0x01));
    WRITE_FIELDTO(rHFFPLLCFG, HAL_DFE_JITAVEEN, ((Iram_Cmd_block.bf[1]>>6) & 0x01));
    WRITE_REG(HAL_DFEREG_MAP->HFFPLLCFG, rHFFPLLCFG);

    switch (Iram_Cmd_block.bf[3])                /* Select Generic Measure Mode */
    {
        case eGENERIC_CONTINUOUS_MODE:
            hifDiaGenericContinuousJitterCmd();
            break;
        case eGENERIC_TIMER_MODE:
            hifDiaGenericTimedJitterCmd();
            break;
        case eGENERIC_ADDRESS_MODE:
            hifDiaGenericAddressedJitterCmd();
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0x58);    /* INVALID FIELD IN COMMAND PACKET */
            AutoCommandComplete();
            break;
    }

    WRITE_REG(HAL_DFEREG_MAP->HFFPLLCFG, rHFFPLLCFG_sav);
}

/**
 *  hifRealtimeJitterMeasurement()
 *
 *  pre  : realtime jitter circuitry enabled
 *  post : jitter value read from hw, converted to jitter %
 *
 *  return : jitter %
 *           + extended jitter result for DVDRAM via input param pointer
 *             to jitterResult_t structure!
 */
extern USHORT hifRealtimeJitterMeasurement(jitterResult_t * extJitterResult)
{
    ULONG result, resultGroove, i;
    BYTE nrLands, nrGrooves;
    BOOL bLand;

    if ((extJitterResult == NULL) || (svoVar_discKindT.Bit.Media != eMediaRAM))
    {
        result = 0L;

        for (i = 0; i < FIRST_LEVEL_AVG_SMPLNR; i++)
        {
            result += (ULONG)READ_FIELD(HAL_DFE_FPLLJIT);
        }

        result /= (FIRST_LEVEL_AVG_SMPLNR / 8);                          /* averaging and scaling measurement */

        return convertRTJitter(result);
    }
    else
    {
        if (svoVar_discKindT.Bit.Media == eMediaRAM)
        {
            result = resultGroove = 0L;
            nrLands = nrGrooves = 0;

            for (i = 0; i < FIRST_LEVEL_AVG_SMPLNR; i++)
            {
                if (READ_FIELD(HAL_DVDD_LANDMVD))
                {
                    bLand = READ_FIELD(HAL_DVDD_LANDMON);
                }
                else
                {
                    if (READ_FIELD(HAL_WBL_MLLSLCDET))
                    {
                        bLand = READ_FIELD(HAL_WBL_LGWBMLH);
                    }
                    else
                    {
                        bLand = READ_FIELD(HAL_WBL_LGWBSLC);
                    }
                }

                if (bLand)
                {
                    /* Land */
                    nrLands++;
                    result += (ULONG)READ_FIELD(HAL_DFE_FPLLJIT);
                }
                else
                {
                    /* Groove */
                    nrGrooves++;
                    resultGroove += (ULONG)READ_FIELD(HAL_DFE_FPLLJIT);
                }
            }

            result *= 8;                          /* scaling for convertRTJitter() */
            resultGroove *= 8;                    /* scaling for convertRTJitter() */

            extJitterResult->extResult.land             = nrLands != 0? convertRTJitter(result/nrLands)          : 0;
            extJitterResult->extResult.groove           = nrGrooves != 0? convertRTJitter(resultGroove/nrGrooves): 0;
            extJitterResult->extResult.totalNrLands     = nrLands;
            extJitterResult->extResult.totalNrGrooves   = nrGrooves;
        }
        else
        {
            //extJitterResult->extResult.land           = 0            /* do not overwrite extJitterResult->us */
            extJitterResult->extResult.groove           = 0;
            extJitterResult->extResult.totalNrLands     = 0;
            extJitterResult->extResult.totalNrGrooves   = 0;
        }
        return extJitterResult->us;
    }
}


/**
 *  hifRealtimeJitterMeasurement()
 *
 *  pre  : realtime jitter circuitry enabled
 *  post : jitter value read from hw, converted to jitter %
 *
 *  return : jitter %
 *
 */

//extern USHORT hifRealtimeJitterMeasurement(void)
//{
//    ULONG result, i;
//
//    result = 0L;
//
//    for (i = 0; i < FIRST_LEVEL_AVG_SMPLNR; i++)
//    {
//        result += (ULONG)ReadSCReg_2B(rRTJIT);
//    }
//
//    result *= 8;                          /* scaling for convertRTJitter() */
//    result /= i;                          /* averaging measurement */
//
//    return convertRTJitter(result);
//}

/*
 * hifGoToPbaStart()
 *
 * returns pbaStart in addition to positioning
 *
 */

static ULONG hifGoToPbaStart(void)
{
    ULONG pbaStart = 0L;
    TimeS msf;
    BYTE tempBYTE;

    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)   /* CD */
    {
        msf.Minute = Iram_Cmd_block.bf[6] ;
        msf.Second = Iram_Cmd_block.bf[7];
        msf.Frame  = Iram_Cmd_block.bf[8];

        pbaStart   = MSF2LBA(&msf);

        msf = MSF2BCD(&msf);

        WRITE_REG(HAL_CDDREG_MAP->THEAD0, msf.Minute);  /* set target address MIN */
        WRITE_REG(HAL_CDDREG_MAP->THEAD1, msf.Second);  /* set target address SEC */
        WRITE_REG(HAL_CDDREG_MAP->THEAD2, msf.Frame);   /* set target address BLK */

        if ( ServoSeek(pbaStart, eDataSeek) == FAIL)
        {
           BUILD_SENSE(0x03,0x02,0x00,0x2c);        /* MEDIUM ERROR, NO SEEK COMPLETE */
           AutoCommandComplete();
           return 0xFFFFFFFF;
        }
    }
    else /* DVD */
    {
        pbaStart = ((ULONG)(Iram_Cmd_block.bf[6] << 16) |
                    (ULONG)(Iram_Cmd_block.bf[7] <<  8) |
                    (ULONG)(Iram_Cmd_block.bf[8]));

        if (pbaStart <= pdbGetDVDMaxPSN(0))         /* Search only in layer 0 */
            tempBYTE = eLayer0;
        else if (pbaStart >= pdbGetDVDStartPSN(1))  /* Search only in layer 1 */
            tempBYTE = eLayer1;

        SET_TARGET_ID(pbaStart);

        /* -------------------------------------------------
         *            Seek to before target
         * -------------------------------------------------
         */

        if ( ServoSeek(pbaStart-20 , tempBYTE) == FAIL )
        {
           BUILD_SENSE(0x03,0x02,0x00,0x2e);        /* MEDIUM ERROR, NO SEEK COMPLETE */
           AutoCommandComplete();
           return 0xFFFFFFFF;
        }
    }
//TBD? BD and/or HD needed here?

    return pbaStart;
}

static ULONG hifGetPbaStop()
{
    TimeS msf;

    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)   /* CD */
    {
        msf.Minute = Iram_Cmd_block.bf[9] ;
        msf.Second = Iram_Cmd_block.bf[10];
        msf.Frame  = Iram_Cmd_block.bf[11];
        return MSF2LBA(&msf);
    }
    else   /* DVD */
    {
        return  ((ULONG)(Iram_Cmd_block.bf[9] << 16)  |
                 (ULONG)(Iram_Cmd_block.bf[10] <<  8) |
                 (ULONG)(Iram_Cmd_block.bf[11]));
    }
//TBD? BD and/or HD needed here?
}


static BOOL hifReachedPbaStop(ULONG pbaStop)
{
    return svoIramVar.currentID.SectorNum.all32 >= pbaStop;
}

static BOOL hifCheckEOB(USHORT index)
{
    ULONG ref = DIAG_WORK_ADDR + (ULONG)END_DIAG_BUFFER - 8;
    ULONG is  = DIAG_WORK_ADDR + (ULONG)index;

    /*  do not cross upper limit of
     *  DIAG_WORK_ADDR space
     */

    return is > ref;
}


static void hifValToOutputBuffer(ULONG val, USHORT * index)
{
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)(val >> 24);
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)(val >> 16);
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)(val >> 8);
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)val;
}

static void hif2ByteValToOutputBuffer(ULONG val, USHORT * index)
{
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)(val >> 8);
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)val;
}

__inline static void hifSaveJitToOutputBuffer(ULONG jit, USHORT * index)
{
    hif2ByteValToOutputBuffer(jit, index);
}

__inline static void hifSaveJitAvgToOutputBuffer(ULONG sum, USHORT * index)
{
    hifValToOutputBuffer(sum, index);
}

static void hifSaveRealisedNrOfSamplesToOutputBuffer(USHORT nrOfSamples, USHORT * index)
{
      hif2ByteValToOutputBuffer(nrOfSamples, index);
}

__inline static void hifSaveDVDRAMLandJitAvgToOutBuf(jitterResult_t * jitterResult, USHORT * index)
{
    hif2ByteValToOutputBuffer(jitterResult->extResult.land, index);
}

__inline static void hifSaveDVDRAMLandRatioAvgToOutBuf(jitterResult_t * jitterResult, USHORT * index, USHORT i)
{
    USHORT landRatio;
    landRatio = (jitterResult->extResult.totalNrLands * 100)/(i * FIRST_LEVEL_AVG_SMPLNR);

    hif2ByteValToOutputBuffer((ULONG)landRatio, index);
}

__inline static void hifSaveDVDRAMGrooveJitAvgToOutBuf(jitterResult_t * jitterResult, USHORT * index)
{
    hif2ByteValToOutputBuffer(jitterResult->extResult.groove, index);
}

__inline static void hifSaveDVDRAMGrooveRatioAvgToOutBuf(jitterResult_t * jitterResult, USHORT * index, USHORT i)
{
    USHORT grooveRatio;
    grooveRatio = (jitterResult->extResult.totalNrGrooves * 100)/(i * FIRST_LEVEL_AVG_SMPLNR);

    hif2ByteValToOutputBuffer((ULONG)grooveRatio, index);
}


static void hifPrepareOutputBuffer(USHORT * index)
{
    /* copy command buffer items */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)Iram_Cmd_block.bf[0];          /* opcode             */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)(Iram_Cmd_block.bf[1] & 0x0F); /* subfunction        */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)Iram_Cmd_block.bf[2];          /* measurement        */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)Iram_Cmd_block.bf[3];          /* mode               */
}

static void hifUpdateNrOfSamplesAndJitAvgEntryInOutputBuffer(USHORT nrOfSamples, USHORT * index)
{
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)(nrOfSamples >> 8);  /* nr of samples high */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = (BYTE)nrOfSamples;         /* nr of samples low  */
    /* reserve space for jitter average */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 1;                         /* jitter avg high */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 2;
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 3;
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 4;                         /* jitter avg low */
    /* reserved space for dvdram land jitter average */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;                         /* land jitter avg high */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;
    /* reserved space for dvdram land ratio average */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;                         /* land ratio avg high */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;
    /* reserved space for dvdram groove jitter average */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;                         /* groove jitter avg high */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;
    /* reserved space for dvdram groove ratio average */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;                         /* groove ratio avg high */
    SCSDRAM[ DIAG_WORK_ADDR + (*index)++] = 0;
}

__inline static void hifTimeDelay(USHORT delayMiliseconds)
{
    ULONG clock;

    StartTimer(&clock);
    while (ReadTimer(&clock) <= delayMiliseconds)
    {
        ExitProcess();     /* wait */
    }
    return;
}

__inline static void hifWaitArrivalNextAddress(ULONG nextPba)
{
    while (nextPba > svoIramVar.currentID.SectorNum.all32)
    {
        ExitProcess();     /* wait */
    }
    return;
}

static void hifInitdvdRamJitter(jitterResult_t * jitterResult)
{
    jitterResult->extResult.groove         = 0;
    jitterResult->extResult.land           = 0;
    jitterResult->extResult.totalNrGrooves = 0;
    jitterResult->extResult.totalNrLands   = 0;
}

static void hifSumDvdRamJitter(jitterResult_t * sumJitterResult, jitterResult_t * smplJitterResult)
{
    sumJitterResult->extResult.groove         += smplJitterResult->extResult.groove * smplJitterResult->extResult.totalNrGrooves;
    sumJitterResult->extResult.land           += smplJitterResult->extResult.land * smplJitterResult->extResult.totalNrLands;
    sumJitterResult->extResult.totalNrGrooves += smplJitterResult->extResult.totalNrGrooves;
    sumJitterResult->extResult.totalNrLands   += smplJitterResult->extResult.totalNrLands;
}

static void hifAvgDvdRamJitter(jitterResult_t * sumJitterResult)
{
    sumJitterResult->extResult.groove = sumJitterResult->extResult.totalNrGrooves != 0? sumJitterResult->extResult.groove/sumJitterResult->extResult.totalNrGrooves: 0;
    sumJitterResult->extResult.land = sumJitterResult->extResult.totalNrLands != 0? sumJitterResult->extResult.land/sumJitterResult->extResult.totalNrLands: 0;
}

/**
 *  hifDiaGenericContinuousJitterCmd()
 *
 *  seek to PBA_START
 *  wait for end of seek
 *  do measurement until PBA_STOP or nr of samples is reached
 *
 */
static void hifDiaGenericContinuousJitterCmd(void)
{
    /* local(s) */
    jitterResult_t dvdRamJitter, sumDvdRamJitter;
    ULONG jit, sum, pbaStart, pbaStop;
    USHORT nrOfSamples, i, index, indexToAvgJitterOutputBuffer, indexToNrOfSamplesOutputBuffer,
           indexToAvgDvdRamLandJitterOutputBuffer, indexToAvgDvdRamLandRatioOutputBuffer,
           indexToAvgDvdRamGrooveJitterOutputBuffer, indexToAvgDvdRamGrooveRatioOutputBuffer;

    /* local initialisation(s) */
    pbaStart = 0L; pbaStop = 0L;
    nrOfSamples = (USHORT)((Iram_Cmd_block.bf[4] << 8) | Iram_Cmd_block.bf[5]);
    nrOfSamples = nrOfSamples == 0? 1: nrOfSamples;
    index = 0; indexToAvgJitterOutputBuffer = 0; indexToNrOfSamplesOutputBuffer = 0;
    indexToAvgDvdRamLandJitterOutputBuffer =0; indexToAvgDvdRamLandRatioOutputBuffer = 0;
    indexToAvgDvdRamGrooveJitterOutputBuffer = 0; indexToAvgDvdRamGrooveRatioOutputBuffer = 0;

    /* start */

    hifPrepareOutputBuffer(&index);
    hifUpdateNrOfSamplesAndJitAvgEntryInOutputBuffer(nrOfSamples, &index);
    indexToAvgDvdRamGrooveRatioOutputBuffer  = index - 2;   /* save place to store DVDRAM groove ratio avg */
    indexToAvgDvdRamGrooveJitterOutputBuffer = index - 4;   /* save place to store DVDRAM groove jitter avg */
    indexToAvgDvdRamLandRatioOutputBuffer    = index - 6;   /* save place to store DVDRAM land ratio avg */
    indexToAvgDvdRamLandJitterOutputBuffer   = index - 8;   /* save place to store DVDRAM land jitter avg */
    indexToAvgJitterOutputBuffer             = index - 12;  /* save place to store jitter average later */
    indexToNrOfSamplesOutputBuffer           = index - 14;  /* save to change if nr of samples not realised */
    enableRTJitterMeas();

    pbaStop  = hifGetPbaStop();
    pbaStart = hifGoToPbaStart();

    sum = 0L;
    hifInitdvdRamJitter(&sumDvdRamJitter);
    i = 0;

    if (pbaStart != pbaStop)
    {
        for (; !hifReachedPbaStop(pbaStop) && i < nrOfSamples; i++)
        {
            if (hifCheckEOB(index) == FALSE)                   /* system limitation check */
            {
                jit = (ULONG)hifRealtimeJitterMeasurement(&dvdRamJitter);
                hifSaveJitToOutputBuffer(jit, &index);

                sum += jit;
                hifSumDvdRamJitter(&sumDvdRamJitter, &dvdRamJitter);
            }
            else                                               /* sytem limitation reached */
            {
                break;
            }
        }
        sum /= i;                                              /* averaging */
        hifAvgDvdRamJitter(&sumDvdRamJitter);
    }

    hifSaveDVDRAMLandJitAvgToOutBuf    (&sumDvdRamJitter, &indexToAvgDvdRamLandJitterOutputBuffer);
    hifSaveDVDRAMLandRatioAvgToOutBuf  (&sumDvdRamJitter, &indexToAvgDvdRamLandRatioOutputBuffer, i);
    hifSaveDVDRAMGrooveJitAvgToOutBuf  (&sumDvdRamJitter, &indexToAvgDvdRamGrooveJitterOutputBuffer);
    hifSaveDVDRAMGrooveRatioAvgToOutBuf(&sumDvdRamJitter, &indexToAvgDvdRamGrooveRatioOutputBuffer, i);

    hifSaveJitAvgToOutputBuffer(sum, &indexToAvgJitterOutputBuffer);
    hifSaveRealisedNrOfSamplesToOutputBuffer(i, &indexToNrOfSamplesOutputBuffer);

    disableRTJitterMeas();
    SendParameterDataToHost(DIAG_WORK_ADDR, index, TRUE);  /* Send data to host. Send Command Complete. */
}

/**
 *  hifDiaGenericTimedJitterCmd()
 *
 *  seek to PBA_START
 *  wait for end of seek
 *  do timed measurement until PBA_STOP
 *  is reached
 *
 *
 */
static void hifDiaGenericTimedJitterCmd(void)
{
    /* local(s) */
    jitterResult_t dvdRamJitter, sumDvdRamJitter;
    ULONG jit, sum, pbaStart, pbaStop;
    USHORT nrOfMiliseconds, i, index, indexToAvgJitterOutputBuffer, indexToNrOfSamplesOutputBuffer,
           indexToAvgDvdRamLandJitterOutputBuffer, indexToAvgDvdRamLandRatioOutputBuffer,
           indexToAvgDvdRamGrooveJitterOutputBuffer, indexToAvgDvdRamGrooveRatioOutputBuffer;

    /* local initialisation(s) */
    pbaStart = 0L, pbaStop = 0L;
    nrOfMiliseconds = (USHORT)((Iram_Cmd_block.bf[4] << 8) | Iram_Cmd_block.bf[5]);
    index = 0, indexToAvgJitterOutputBuffer = 0, indexToNrOfSamplesOutputBuffer = 0;
    indexToAvgDvdRamLandJitterOutputBuffer =0; indexToAvgDvdRamLandRatioOutputBuffer = 0;
    indexToAvgDvdRamGrooveJitterOutputBuffer = 0; indexToAvgDvdRamGrooveRatioOutputBuffer = 0;

    /* start */

    hifPrepareOutputBuffer(&index);
    hifUpdateNrOfSamplesAndJitAvgEntryInOutputBuffer(nrOfMiliseconds, &index);
    indexToAvgDvdRamGrooveRatioOutputBuffer  = index - 2;   /* save place to store DVDRAM groove ratio avg */
    indexToAvgDvdRamGrooveJitterOutputBuffer = index - 4;   /* save place to store DVDRAM groove jitter avg */
    indexToAvgDvdRamLandRatioOutputBuffer    = index - 6;   /* save place to store DVDRAM land ratio avg */
    indexToAvgDvdRamLandJitterOutputBuffer   = index - 8;   /* save place to store DVDRAM land jitter avg */
    indexToAvgJitterOutputBuffer             = index - 12;  /* save place to store jitter average later */
    indexToNrOfSamplesOutputBuffer           = index - 14;  /* save to change if nr of samples not realised */
    enableRTJitterMeas();

    pbaStop  = hifGetPbaStop();
    pbaStart = hifGoToPbaStart();

    sum = 0L;
    hifInitdvdRamJitter(&sumDvdRamJitter);
    i = 0;
    if (pbaStart != pbaStop)
    {
        for (; !hifReachedPbaStop(pbaStop) && i < USHRT_MAX; i++)
        {
            if (hifCheckEOB(index) == FALSE)                   /* system limitation check */
            {
                jit = (ULONG)hifRealtimeJitterMeasurement(&dvdRamJitter);
                hifSaveJitToOutputBuffer(jit, &index);
                sum += jit;
                hifSumDvdRamJitter(&sumDvdRamJitter, &dvdRamJitter);
                hifTimeDelay((USHORT)nrOfMiliseconds);         /* wait for sample time to elaps */
            }
            else                                               /* sytem limitation reached */
            {
                break;
            }
        }
        sum /= i;                                              /* averaging */
        hifAvgDvdRamJitter(&sumDvdRamJitter);
    }

    hifSaveDVDRAMLandJitAvgToOutBuf    (&sumDvdRamJitter, &indexToAvgDvdRamLandJitterOutputBuffer);
    hifSaveDVDRAMLandRatioAvgToOutBuf  (&sumDvdRamJitter, &indexToAvgDvdRamLandRatioOutputBuffer, i);
    hifSaveDVDRAMGrooveJitAvgToOutBuf  (&sumDvdRamJitter, &indexToAvgDvdRamGrooveJitterOutputBuffer);
    hifSaveDVDRAMGrooveRatioAvgToOutBuf(&sumDvdRamJitter, &indexToAvgDvdRamGrooveRatioOutputBuffer, i);

    hifSaveJitAvgToOutputBuffer(sum, &indexToAvgJitterOutputBuffer);
    hifSaveRealisedNrOfSamplesToOutputBuffer(i, &indexToNrOfSamplesOutputBuffer);

    disableRTJitterMeas();
    SendParameterDataToHost(DIAG_WORK_ADDR, index , TRUE); /* Send data to host. Send Command Complete. */
}



/**
 *  hifDiaGenericAddressedJitterCmd()
 *
 *  seek to PBA_START
 *  wait for end of seek
 *  do measurement at every address increment until PBA_STOP
 *  or nrOfSamples reached
 */
static void hifDiaGenericAddressedJitterCmd(void)
{
    /* local(s) */
    jitterResult_t dvdRamJitter, sumDvdRamJitter;
    ULONG jit, sum, pbaCount, pbaStop, addressDelta;
    USHORT nrOfSamples, i, index, indexToAvgJitterOutputBuffer, indexToNrOfSamplesOutputBuffer,
           indexToAvgDvdRamLandJitterOutputBuffer, indexToAvgDvdRamLandRatioOutputBuffer,
           indexToAvgDvdRamGrooveJitterOutputBuffer, indexToAvgDvdRamGrooveRatioOutputBuffer;

    /* local initialisation(s) */
    pbaCount = 0L; pbaStop = 0L; addressDelta = 0L;
    index = 0; indexToAvgJitterOutputBuffer = 0; indexToNrOfSamplesOutputBuffer = 0;
    nrOfSamples = (USHORT)((Iram_Cmd_block.bf[4] << 8) | Iram_Cmd_block.bf[5]);
    nrOfSamples = nrOfSamples == 0? 1: nrOfSamples;
    indexToAvgDvdRamLandJitterOutputBuffer =0; indexToAvgDvdRamLandRatioOutputBuffer = 0;
    indexToAvgDvdRamGrooveJitterOutputBuffer = 0; indexToAvgDvdRamGrooveRatioOutputBuffer = 0;

    /* start */

    hifPrepareOutputBuffer(&index);
    hifUpdateNrOfSamplesAndJitAvgEntryInOutputBuffer(nrOfSamples, &index);
    indexToAvgDvdRamGrooveRatioOutputBuffer  = index - 2;   /* save place to store DVDRAM groove ratio avg */
    indexToAvgDvdRamGrooveJitterOutputBuffer = index - 4;   /* save place to store DVDRAM groove jitter avg */
    indexToAvgDvdRamLandRatioOutputBuffer    = index - 6;   /* save place to store DVDRAM land ratio avg */
    indexToAvgDvdRamLandJitterOutputBuffer   = index - 8;   /* save place to store DVDRAM land jitter avg */
    indexToAvgJitterOutputBuffer             = index - 12;  /* save place to store jitter average later */
    indexToNrOfSamplesOutputBuffer           = index - 14;  /* save to change if nr of samples not realised */
    enableRTJitterMeas();

    pbaStop  = hifGetPbaStop();
    pbaCount = hifGoToPbaStart();

    /* calculate delta number of addresses between samples */
    addressDelta = 100 * pbaStop;                          /* upscaling */
    addressDelta -= (100 * pbaCount);
    addressDelta /= nrOfSamples;
    addressDelta /= 100;                                   /* downscaling */

    sum = 0L;
    hifInitdvdRamJitter(&sumDvdRamJitter);
    i = 0;
    if (pbaCount != pbaStop)
    {
        for (; !hifReachedPbaStop(pbaStop) && i < nrOfSamples; i++)
        {
            if (hifCheckEOB(index) == FALSE)                   /* system limitation check */
            {
                jit = (ULONG)hifRealtimeJitterMeasurement(&dvdRamJitter);
                hifSaveJitToOutputBuffer(jit, &index);
                sum += jit;
                hifSumDvdRamJitter(&sumDvdRamJitter, &dvdRamJitter);
                pbaCount += addressDelta;
                hifWaitArrivalNextAddress(pbaCount);           /* wait for next sample address to appear */
            }
            else                                               /* sytem limitation reached */
            {
                break;
            }
        }
        sum /= i;                                              /* averaging */
        hifAvgDvdRamJitter(&sumDvdRamJitter);
    }

    hifSaveDVDRAMLandJitAvgToOutBuf    (&sumDvdRamJitter, &indexToAvgDvdRamLandJitterOutputBuffer);
    hifSaveDVDRAMLandRatioAvgToOutBuf  (&sumDvdRamJitter, &indexToAvgDvdRamLandRatioOutputBuffer, i);
    hifSaveDVDRAMGrooveJitAvgToOutBuf  (&sumDvdRamJitter, &indexToAvgDvdRamGrooveJitterOutputBuffer);
    hifSaveDVDRAMGrooveRatioAvgToOutBuf(&sumDvdRamJitter, &indexToAvgDvdRamGrooveRatioOutputBuffer, i);

    hifSaveJitAvgToOutputBuffer(sum, &indexToAvgJitterOutputBuffer);
    hifSaveRealisedNrOfSamplesToOutputBuffer(i, &indexToNrOfSamplesOutputBuffer);

    disableRTJitterMeas();
    SendParameterDataToHost(DIAG_WORK_ADDR, index , TRUE);  /* Send data to host. Send Command Complete. */
}



/* other generic commands follow here  */
/* ...                                 */
#endif  //USE_REALTIME_JITTER
#if  SERVOKEYPARAMETER == 1
/*******************************************************************************

    FUNCTION        diaServoKeyPara()(F4 07)

    DESCRIPTION     DVD diagnostic ServoKeyPara commands

********************************************************************************/
void diaServoKeyPara()
{
    BYTE ctrlbyte;
    StLongU Xfer_Len;
    ULONG headerlen,datalen;


    ctrlbyte = Iram_Cmd_block.bf[2];
    Xfer_Len.byte.u = Iram_Cmd_block.bf[6];
    Xfer_Len.byte.h = Iram_Cmd_block.bf[7];
    Xfer_Len.byte.m = Iram_Cmd_block.bf[8];
    Xfer_Len.byte.l = Iram_Cmd_block.bf[9];
    headerlen =4;
    datalen = Xfer_Len.Long - headerlen;
    switch(ctrlbyte)
    {
        case 1: //Initiate the key parameter
            InitServoKeyParameter(TRUE);
            break;
        case 2: //Force the key parameter trigger
            if(Iram_Cmd_block.bf[3] == 1)
            {
                VendorServoParaSwitch = TRUE;
                send_msg5S(1,"EnsableKEYpara");
            }
            else
            {
                VendorServoParaSwitch = FALSE;
                send_msg5S(1,"DisableKEYpara");
            }
            break;
        case 3: //Get all key paramter from FlashROM
            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            WriteSDRAM(datalen>>8);
            WriteSDRAM(datalen);
            WriteSDRAM(0);
            WriteSDRAM(0);
            SendParameterDataToHost(BUFFER_SCRATCH, 4, FALSE);
            OpenWritableSegmentTable(eKey_parameter_Table);
            SendParameterDataToHost(DRVPARA_DATA, Xfer_Len.Long,FALSE);
            break;
        case 4: //Get current Disc key parameter
            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            //header
            WriteSDRAM(datalen>>8);
            WriteSDRAM(datalen);
            WriteSDRAM(0);
            WriteSDRAM(0);
            SendParameterDataToHost(BUFFER_SCRATCH, 4, FALSE);

            if(CurTblOnDram!=eKey_parameter_Table)
                OpenWritableSegmentTable(eKey_parameter_Table);
            switch(pdbGetDiscKind())
            {
                case eCDROMDisc:
                    //SetServoKeyParameterCDROM();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+CDROM_header+0x20, datalen, FALSE);
                    break;
                case eCDRDisc:
                    //SetServoKeyParameterCDR();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+CDR_header+0x20, datalen, FALSE);
                    break;
                case eCDRWDisc:
                    //SetServoKeyParameterCDRW();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+CDRW_header+0x20, datalen, FALSE);
                    break;
                case eDVDROMDisc:
                    //SetServoKeyParameterDVDROM();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+DVD5_header+0x20, datalen, FALSE);
                    break;
                case eDVDROMDLDisc:
                    //SetServoKeyParameterDVDROMDL();
                    SetServoKeyParameterDL();
                    SendParameterDataToHost(DRVPARA_DATA+DVD9_header+0x20, datalen, FALSE);
                    break;
                case eDVDPlusRDisc:
                    //SetServoKeyParameterDVDPR();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+DVDPR_header+0x20, datalen, FALSE);
                    break;
                case eDVDPlusRDLDisc:
                    //SetServoKeyParameterDVDPR9();
                    SetServoKeyParameterDL();
                    SendParameterDataToHost(DRVPARA_DATA+DVDPR9_header+0x20, datalen, FALSE);
                    break;
                case eDVDMinusRDisc:
                    //SetServoKeyParameterDVDMR();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+DVDMR_header+0x20, datalen, FALSE);
                    break;
                case eDVDMinusRDLDisc:
                    //SetServoKeyParameterDVDMR9();
                    SetServoKeyParameterDL();
                    SendParameterDataToHost(DRVPARA_DATA+DVDMR9_header+0x20, datalen, FALSE);
                    break;
                case eDVDPlusRWDisc:
                    //SetServoKeyParameterDVDPRW();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+DVDPRW_header+0x20, datalen, FALSE);
                    break;
                //case eDVDPlusRWDLDisc:
                case eDVDMinusRWDisc:
                    //SetServoKeyParameterDVDMRW();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+DVDMRW_header+0x20, datalen, FALSE);
                    break;
                case eBDROMDisc:
                    //SetServoKeyParameterBDROM();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+BDROM_header+0x20, datalen, FALSE);
                    break;
                case eBDROMDLDisc:
                    //SetServoKeyParameterBDROMDL();
                    SetServoKeyParameterDL();
                    SendParameterDataToHost(DRVPARA_DATA+BDROMDL_header+0x20, datalen, FALSE);
                    break;
                case eBDRDisc:
                    //SetServoKeyParameterBDR();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+BDR_header+0x20, datalen, FALSE);
                    break;
                case eBDRDLDisc:
                    //SetServoKeyParameterBDRDL();
                    SetServoKeyParameterDL();
                    SendParameterDataToHost(DRVPARA_DATA+BDRDL_header+0x20, datalen, FALSE);
                    break;
                case eBDREDisc:
                    //SetServoKeyParameterBDRE();
                    SetServoKeyParameterSL();
                    SendParameterDataToHost(DRVPARA_DATA+BDRE_header+0x20, datalen, FALSE);
                    break;
                case eBDREDLDisc:
                    //SetServoKeyParameterBDREDL();
                    SetServoKeyParameterDL();
                    SendParameterDataToHost(DRVPARA_DATA+BDREDL_header+0x20, datalen, FALSE);
                    break;
                default :
                     BUILD_SENSE(0x05,0x20,0x00,0x49);    /* INVALID COMMAND OPERATION CODE */
                     break;
            }
            break;

        default: //
            BUILD_SENSE(0x05,0x24,0x00,0x5f);    /* Invalid Field in CDB */
            break;
    }
}
#endif //SERVOKEYPARAMETER == 1
/*******************************************************************************

    FUNCTION        diaCmdDvdDiagnostic()(F6)

    DESCRIPTION     DVD diagnostic commands

    BYTE 0      OPCODE
    BYTE 1      SUB COMMAND

    SUB COMMAND TYPES

    ---------------------------------------------------------------------------
    READ PSN (00h)      Sends 1 ECC block (16 sectors) to host (based on PSN)
    BYTE 2 - 5  PSN     BYTE 2 = MSB
    BYTE 6      Layer   0 or 1
    BYTE 7 - 11 Reserved
    ---------------------------------------------------------------------------
    ---------------------------------------------------------------------------
    READ LONG (01h)     Sends 1 raw ECC block (19 sectors) to host (based on PSN)
    BYTE 2 - 5  PSN     BYTE 2 = MSB
    BYTE 6      Layer   0 or 1
    BYTE 7 - 11 Reserved
    ---------------------------------------------------------------------------

********************************************************************************/
void diaCmdDvdDiagnostic(void)
{

    DiscAccessStatusE   DiscReadStatus;
    RPC_NON_VOLATILE_VARS RpcSettings;
    BYTE SubCommand;
    BYTE *  DramAddressP;
    BYTE    rpcStateByte0,rpcStateByte1,rpcStateByte2;  /* RPC data variables */
    BYTE    RegionCode;

    /*---------------------------------------------------------------
        Initial buffer
    ---------------------------------------------------------------*/
    if (DVD_BUFFERING() == ON)
        dvdStopDiscTransfer();

    ClearDataBuffer();

    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;


    // Process sub command
    SubCommand = Iram_Cmd_block.bf[1] & 0x0F;
    switch (SubCommand)
    {
        case DVD_SUBCMD_READ_PSN:
        case DVD_SUBCMD_READ_LONG:

            // GET START PSN
            dvdDiscRequest.TargetID.byte.u = Iram_Cmd_block.bf[2];
            dvdDiscRequest.TargetID.byte.h = Iram_Cmd_block.bf[3];
            dvdDiscRequest.TargetID.byte.m = Iram_Cmd_block.bf[4];
            dvdDiscRequest.TargetID.byte.l = Iram_Cmd_block.bf[5];

            // Set specific info based on read type
            if(SubCommand == DVD_SUBCMD_READ_LONG)
            {
                HostXfer.BlockCount.Long = DVD_SECTORS_PER_READ_LONG_BLOCK;
                DVD_ENABLE_READ_LONG();
            }
            else
                HostXfer.BlockCount.Long = ONE_DVD_BLOCK;

            // Host sectors remaining to transfer
            HostXfer.BlocksLeft = HostXfer.BlockCount.Long;

            // Host transfer state
            HostXfer.State = WAITING_FOR_TRANSFER;

            // ECC blocks to buffer
            dvdDiscRequest.EccBlockCount = 1L;

            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters and call disc transfer
            ----------------------------------------------------------------*/
            dvdDiscRequest.Type            = INIT_READ;
            dvdDiscRequest.TargetLayer     = Iram_Cmd_block.bf[6];
            DiscReadStatus = dvdStartDiscTransfer();

            /*---------------------------------------------------------------
                If buffering failed build the sense code according
                to type of DiscReadStatus
            ---------------------------------------------------------------*/
            if (DiscReadStatus != BUF_GOOD)
            {
                HostXfer.State = TRANSFER_COMPLETE;
                dvdSetSenseCode(DiscReadStatus);
            }

            // Wait for host to complete transfer
            while(HostXfer.State != TRANSFER_COMPLETE)
                ExitProcess();

            // Disable Read Long if set
            if (SubCommand == DVD_SUBCMD_READ_LONG)
                DVD_DISABLE_READ_LONG();

            break;

        case DVD_SUBCMD_RESET_REGION:
            if(Iram_Cmd_block.bf[2] == 0xCC)
            {
                RpcSettings = RpcGetRegion();    // Get RPC settings from FLASH or Serial EEPROM
                /*****************************
                *       Get RPC Type Code
                *       Byte 0 : bits 6 & 7
                *****************************/
                rpcStateByte0 = RpcSettings.RPC.State << 6;

                /********************************************************
                *       Get number of user controlled changes available
                *       Byte 0 : bits 3 - 5
                ********************************************************/
                rpcStateByte0 |= (RpcSettings.RPC.VendorResetsAvailable & 0x07) << 3;

                /********************************************************
                *       Get number of user controlled changes available
                *       Byte 0 : bits 0 - 2
                ********************************************************/
                rpcStateByte0 |= (RpcSettings.RPC.nCount & 0x07);

                /*********************************
                *       Get region mask. Byte 1
                *********************************/
                rpcStateByte1 = RpcSettings.RPC.LuRegion;

                /********************************************
                *       Determine RPC functionality. Byte 2
                *******************************************/
                #define RPC_SCHEME_UNKNOWN      0x00
                #define RPC_SCHEME_PHASE_II     0x01
                rpcStateByte2 = RPC_SCHEME_PHASE_II;

                /***********************************
                *Set DRAM pointer to scratch area
                **********************************/
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM

                /*********************************
                *Write RPC data to scratch area
                ********************************/
                *DramAddressP++ = rpcStateByte0;
                *DramAddressP++ = rpcStateByte1;
                *DramAddressP++ = rpcStateByte2;
                *DramAddressP++ = 0;
                SendParameterDataToHost(BUFFER_SCRATCH, 4, TRUE);
                return;
            }
            else if(Iram_Cmd_block.bf[2] == 0xCD)
            {
                RegionCode = Iram_Cmd_block.bf[3] & 0xFF;
                RpcSetRegion(RegionCode);
            }
            else if(Iram_Cmd_block.bf[2] == 0xCE)
            {
                BYTE temp_IE, PreRPCState;

                if(GetPersistentTable1B(RPC_VENDOR_RESET_OFFSET) != 0)
                {
                    temp_IE = GET_INT_ENABLE_MASK();    // Disable interrupts
                    DISABLE_INTERRUPT();                //   while messing with FLASH

                    OpenWritableSegmentTable(ePersistent_Talbe);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM
                    PreRPCState = GetPersistentTable1B(RPC_STATE_OFFSET);
                    SetPersistentTable1B(RPC_STATE_OFFSET, 0);
                    SetPersistentTable1B(RPC_COUNT_OFFSET, 5);
                    SetPersistentTable1B(RPC_REGION_OFFSET,0xFF);
                    if((GetPersistentTable1B(RPC_VENDOR_RESET_OFFSET) > 0) && (PreRPCState != 0))
                        SetPersistentTable1B(RPC_VENDOR_RESET_OFFSET, GetPersistentTable1B(RPC_VENDOR_RESET_OFFSET)-1);
                    CloseWritableSegmentTable(ePersistent_Talbe);
                    SET_INT_ENABLE_MASK(temp_IE);   // restore interrupts
                }
                else
                {
                    BUILD_SENSE(0x05,0x6F,0x05,0x4c);                   // DRIVE REGION MUST BE PERMANENT/REGION RESET COUNT ERROR
                }
            }
            else if(Iram_Cmd_block.bf[2] == 0xCF)
            {
                BYTE temp_IE ;

                temp_IE = GET_INT_ENABLE_MASK();    // Disable interrupts
                DISABLE_INTERRUPT();                //   while messing with FLASH

                OpenWritableSegmentTable(ePersistent_Talbe);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM

                SetPersistentTable1B(RPC_STATE_OFFSET, 0);
                SetPersistentTable1B(RPC_COUNT_OFFSET, 5);
                SetPersistentTable1B(RPC_REGION_OFFSET,0xFF);
                SetPersistentTable1B(RPC_VENDOR_RESET_OFFSET, 4);
                CloseWritableSegmentTable(ePersistent_Talbe);
                SET_INT_ENABLE_MASK(temp_IE);   // restore interrupts
            }
            else
                BUILD_SENSE(0x05,0x24,0x00,0x5e);    /* Invalid Field in CDB */

            break;

        default:

            //--------------------------------------------------------
            //  INVALID SUB COMMAND
            //--------------------------------------------------------
            BUILD_SENSE(0x05,0x24,0x00,0x5f);    /* Invalid Field in CDB */

    }// End sub command processing

    AutoCommandComplete();

}


typedef enum
{
  eAcessBYTE = 0,
  eAccessUSHORT = 1,
  eAccessULONG = 2
} EAccess;

/*******************************************************************************

    FUNCTION        diaReadWriteDevice()

    DESCRIPTION      READ/WRITE DEVICE COMMAND

    PARAMETERS: ATAPI command block (BYTE 0 Opcode = F7)

    NOTE:
********************************************************************************/
void  diaReadWriteDevice()
{
    ULONG   start_addr, memory_addr;
    StLongU Offset_addr,Xfer_Len;
    BYTE *  AdrP;
    USHORT *  Adr2P;
    ULONG *  Adr4P;
    EAccess Access;
    BYTE    pagecode;

    Offset_addr.byte.u = Iram_Cmd_block.bf[3];      /* the offset of Starting address  */
    Offset_addr.byte.h = Iram_Cmd_block.bf[4];      /* the offset of Starting address  */
    Offset_addr.byte.m = Iram_Cmd_block.bf[5];      /* the offset of Starting address  */
    Offset_addr.byte.l = Iram_Cmd_block.bf[6];      /* the offset of Starting address  */

    Xfer_Len.byte.u =0x00;
    Xfer_Len.byte.h = Iram_Cmd_block.bf[7];
    Xfer_Len.byte.m = Iram_Cmd_block.bf[8];
    Xfer_Len.byte.l = Iram_Cmd_block.bf[9];
    // if(Xfer_Len.Long > 0x1000L)
    //    Xfer_Len.Long = 0x1000L;

    Access =  (Iram_Cmd_block.bf[1] >> 1) & 0x03;

    if (Iram_Cmd_block.bf[1] & 0x01)/* Write device */
    {
        switch(Iram_Cmd_block.bf[2])
        {
            case 0:
                start_addr = Offset_addr.Long;     /*Device start address*/
                break;
            case 1:
                start_addr = ASIC_BASE + Offset_addr.Long;     /*Register data start address*/
                break;
            case 2:// support define Table on flash (in FLASH.h)
                pagecode = Iram_Cmd_block.bf[3]; // byte3 is table number, byte 456 is offset.
                Offset_addr.byte.u = 0x00;
                OpenWritableSegmentTable(pagecode);
                Iram_Linear_Xfer_addr = SCSDRAM_ADDRESS + DRVPARA_DATA + Offset_addr.Long;
                GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);
                CloseWritableSegmentTable(pagecode);
                AutoCommandComplete();
                return;
            case 3:
                start_addr = Offset_addr.Long;     /*SDRAM start address*/
                if(start_addr&0x03 == 0)
                {
                    GetParameterDataFromHost(start_addr, Xfer_Len.Long);
                    AutoCommandComplete();
                    return;
                }
                else
                    start_addr = SCSDRAM_ADDRESS + start_addr;

                break;
#if (ENABLE_AACS == 1)
            case 9: // Store AACS Keys into the flash rom
                if (Xfer_Len.Long != 0xC0)
                {
                    BUILD_SENSE(0x05,0x20,0x00,0x4c); /* INVALID COMMAND OPERATION CODE */
                    AutoCommandComplete();
                    return;
                }
                OpenWritableSegmentTable(eAACS_Table);
                Iram_Linear_Xfer_addr = DRVPARA_DATA + PARA_AACSKEY_OFFSET;
                GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);
                if (ProcessKeysFromHost(Iram_Linear_Xfer_addr) == FALSE)
                {
                    BUILD_SENSE(0x05,0x20,0x00,0x42); /* INVALID COMMAND OPERATION CODE */
                }
                CloseWritableSegmentTable(eAACS_Table);
                AutoCommandComplete();
                return;
#endif

#if (EN_DRIVE_CERT == 1)
            case 0x0B:
                if (Xfer_Len.Long != 0x20)
                {
                    BUILD_SENSE(0x05,0x20,0x00,0x50); /* INVALID COMMAND OPERATION CODE */
                    AutoCommandComplete();
                    return;
                }
                Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
                GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);
                GenerateDriveCert(SCSDRAM_ADDRESS + BUFFER_SCRATCH);
                AutoCommandComplete();
                return;
#endif

#if (EN_FW_SIGNATURE == 1)
            case 0x0C:
                Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
                GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);
                EncryptIM(Iram_Linear_Xfer_addr + SCSDRAM_ADDRESS, Xfer_Len.Long);
                AutoCommandComplete();
                return;
#endif

            case 0x0D:// Write Barcode to Persistent Table
            {
                USHORT BarcodeLength;
                BYTE   *SourcePtr, *TargetPtr;
                BYTE   i;

                Iram_Linear_Xfer_addr = SCSDRAM_ADDRESS;
                GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);
                OpenWritableSegmentTable(ePersistent_Talbe);
                //Copy Barcode.txt from SCSDRAM_ADDRESS to DRVPARA_DATA
                SourcePtr = (BYTE *) (SCSDRAM_ADDRESS);
                TargetPtr = (BYTE *) (SCSDRAM_ADDRESS + DRVPARA_DATA + BARCODE_OFFSET - 4); //4:header length

                BarcodeLength = (USHORT)((*SourcePtr) << 16);
                BarcodeLength |= *(SourcePtr + 1);
                //send_msg5SValue(1,"B1 Length =",2,BarcodeLength);

                for (i = 0; i < (BarcodeLength + 4); i++)
                    *TargetPtr++ = *SourcePtr++;

                CloseWritableSegmentTable(ePersistent_Talbe);
                Cal_LoadOfflineCalibrationParam();
                AutoCommandComplete();
                return;
            }

        #if (CEProduct == 1)
            case 0x12:  // Set Timeout for Read10/12 commands
                BHIGH(TimeUpForRead) = Iram_Cmd_block.bf[10];
                BLOW(TimeUpForRead)  = Iram_Cmd_block.bf[11];
                if (TimeUpForRead == 0x00)
                    TimeUpForRead = TIMEUP_6S;
                else
                    TimeUpForRead = (TimeUpForRead * 100)/20;

                AutoCommandComplete();
                return;
        #endif

            default:
                BUILD_SENSE(0x05,0x20,0x00,0x4e); /* INVALID COMMAND OPERATION CODE */
                AutoCommandComplete();
                return;
        }//switch

        Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
        GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len.Long);

        Iram_Buf_dram_addr = BUFFER_SCRATCH;
        switch(Access)
        {
            case eAccessULONG:
                Adr4P = (ULONG *) (start_addr );
                for (start_addr = 0; start_addr < Xfer_Len.Long/sizeof(ULONG) ; start_addr++)
                {
                    *Adr4P++=ReadSDRAM4B();
                }
                break;
            case eAccessUSHORT:
                Adr2P = (USHORT *) (start_addr );
                for (start_addr = 0; start_addr < Xfer_Len.Long/sizeof(USHORT) ; start_addr++)
                {
                    *Adr2P++=ReadSDRAM2B();
                }
                break;
            default:
                AdrP = (BYTE *) (start_addr );
                for (start_addr = 0; start_addr < Xfer_Len.Long ; start_addr++)
                {
                    *AdrP++=ReadSDRAM();
                }
                break;
        }
        AutoCommandComplete();
    }
    else/* Read device */
    {
        switch(Iram_Cmd_block.bf[2])
        {
        case 0:
            start_addr = Offset_addr.Long;     /*Device start address*/
            break;
        case 1:
            start_addr = ASIC_BASE + Offset_addr.Long;     /*Register data start address*/
            break;
        case 2:
            // support define Table on flash (in FLASH.h)
            pagecode = Iram_Cmd_block.bf[3];
            Offset_addr.byte.u = 0x00;
            OpenWritableSegmentTable(pagecode);
            SendParameterDataToHost(DRVPARA_DATA + Offset_addr.Long, Xfer_Len.Long, TRUE);
            return;
        case 3:
            start_addr = Offset_addr.Long;     /*SDRAM start address*/
            if(start_addr&0x03 == 0)
            {
                SendParameterDataToHost(start_addr, Xfer_Len.Long,TRUE);  /* xfer data and post cmd complete */
                return;
            }
            else
                start_addr = SCSDRAM_ADDRESS + start_addr;

            break;
        //[S07]Bios [J50K] Read S/N number
        case 5:
            // support define Table on flash (in FLASH.h)
            Offset_addr.byte.u = 0x00;
            OpenWritableSegmentTable(ePersistent_Talbe);
            SendParameterDataToHost(DRVPARA_DATA + Offset_addr.Long+0x0F00, Xfer_Len.Long, TRUE);
        //end
        case 7: //Read any F-Rom data by Address and Offset
            start_addr = FLASH_BASE + Offset_addr.Long;     /* Flash Rom start address + Offset */
            if((start_addr < FLASH_BASE)||((start_addr + Xfer_Len.Long)> FLASH_BASE + FLASH_ROM_SIZE)) //Total Rom size = 0x100000 = 1 MByte
            {
                BUILD_SENSE(0x05,0x20,0x00,0x41); /* INVALID COMMAND OPERATION CODE */
                AutoCommandComplete();
                return;
            }

                    AdrP = (BYTE *) (start_addr );
                    Iram_Buf_dram_addr = BUFFER_SCRATCH;
                    memory_addr = BUFFER_SCRATCH;

                    for (start_addr = 0; start_addr < Xfer_Len.Long ; start_addr++)
                    {
                        if (Offset_addr.Long + start_addr >= (FLASH_ROM_SIZE-2))
                            WriteSDRAM(0x20);
                        else
                            WriteSDRAM(*AdrP++);
                    }
            
            SendParameterDataToHost(memory_addr, Xfer_Len.Long,TRUE);  /* xfer data and post cmd complete */
            return;

#if (ENABLE_AACS == 1)
        case 0x09:  // Check the existence/validity of AACS keys
            AdrP = (BYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH);
            CheckAacsKeyExistVdlid(AdrP);
            SendParameterDataToHost(BUFFER_SCRATCH, 4, TRUE);
            return;

        case 0x0F:
            AdrP = (BYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH);
            CheckHWKeyBank(AdrP);
            SendParameterDataToHost(BUFFER_SCRATCH, 4, TRUE);
            return;
#endif

#if (EN_DRIVE_CERT == 1)
        case 0x0B:
            if (Xfer_Len.Long != 0x14)
            {
                BUILD_SENSE(0x05,0x20,0x00,0x51); /* INVALID COMMAND OPERATION CODE */
                AutoCommandComplete();
                return;
            }
            ReturnDriveCert(SCSDRAM_ADDRESS + BUFFER_SCRATCH);
            SendParameterDataToHost(BUFFER_SCRATCH, Xfer_Len.Long, TRUE);  /* xfer data and post cmd complete */
            return;
#endif

#if (EN_FW_SIGNATURE == 1)
        case 0x0C:
            SendParameterDataToHost(BUFFER_SCRATCH, Xfer_Len.Long, TRUE);
            return;
#endif

        case 0x0D:
        {
            USHORT BarcodeLength;
            BYTE   *SourcePtr, *TargetPtr;
            BYTE   i;

            SourcePtr = (BYTE *) (FLASH_BASE + PARA_BLK_START_OFFSET + BARCODE_OFFSET - 4);  //4:header length
            TargetPtr = (BYTE *) (SCSDRAM_ADDRESS);

            BarcodeLength = (USHORT)((*SourcePtr) << 16);
            BarcodeLength |= *(SourcePtr + 1);

            for (i = 0; i < (BarcodeLength + 4); i++)
                *TargetPtr++ = *SourcePtr++;

        }
            start_addr = SCSDRAM_ADDRESS;    /* Return data start address*/
            break;

        case 0x0E:// Vendor ata Command
        {
            switch(Iram_Cmd_block.bf[9])
            {
                case ATA_IDENTIFY_PACKET_DEVICE:
                VendorATAIdentifyPacketDeviceCommand();
                break;

            case ATA_STANDBY_IMMED:
                AutoCommandComplete();
                VendorATAStandbyImmedCommand();
                return;

            case ATA_SET_FEATURE:
                VendorATASetFeatureCommand();
                break;

            case ATA_STANDBY:
                ataStandbyCommand();
                break;

            case ATA_IDLE_IMMED:
            case ATA_IDLE:
                ataIdleImmedCommand();
                break;

            case ATA_SLEEP:
                ataSleepCommand();
                break;

            case ATA_CHECK_POWER_MODE:
                ataCheckPowerModeCommand();
                break;

            case ATA_GET_MEDIA_STATUS:
                ataGetMediaStatus();
                break;
            default:
                send_msg5S(SEND_ATAPI_CMD, "Invalid V-ATA Command");
            }
            AutoCommandComplete();
            return;
        }

        default:
            BUILD_SENSE(0x05,0x20,0x00,0x4f); /* INVALID COMMAND OPERATION CODE */
            AutoCommandComplete();
            return;
        }//switch

        #if (EN_FW_SIGNATURE == 1)
        if (((start_addr & 0xF0000000) == 0) || ((start_addr & 0xF0000000) == 0x70000000))
        {
            BUILD_SENSE(0x05,0x20,0x00,0x4E); /* INVALID COMMAND OPERATION CODE */
            AutoCommandComplete();
            return;
        }
        #endif

        Iram_Buf_dram_addr = BUFFER_SCRATCH;
        switch(Access)
        {
            case eAccessULONG:
                Adr4P = (ULONG *) (start_addr );
                for (start_addr = 0; start_addr < Xfer_Len.Long/sizeof(ULONG) ; start_addr++)
                {
                    WriteSDRAM4B(*Adr4P++);
                }
                break;
            case eAccessUSHORT:
                Adr2P = (USHORT *) (start_addr );
                for (start_addr = 0; start_addr < Xfer_Len.Long/sizeof(USHORT) ; start_addr++)
                {
                    WriteSDRAM2B(*Adr2P++);
                }
                break;
            default:
                AdrP = (BYTE *) (start_addr );
                for (start_addr = 0; start_addr < Xfer_Len.Long ; start_addr++)
                {
                    WriteSDRAM(*AdrP++);
                }
                break;
        }
        SendParameterDataToHost(BUFFER_SCRATCH, Xfer_Len.Long,TRUE);  /* xfer data and post cmd complete */
    }
}

/*******************************************************************************

    FUNCTION        diaCmdOfflineCalibration()

    DESCRIPTION     OFF Line Calbration Vendor Command

    PARAMETERS: ATAPI command block (BYTE 0 Opcode = F8)
    BYTE 1.3~0  Sub Function
                0=>Power Calibration Function
                1=>Focus Balance Calibration Function
                2=>Beta Offset Calibration Function
    NOTE:
********************************************************************************/
void diaCmdOfflineCalibration()  /* F8: OFF Line Calbration Vendor Command */
{
#if 0 //disable offline calibration    
    USHORT Xfer_Num,Xfer_Len;

    Xfer_Len = ((((USHORT)Iram_Cmd_block.bf[11])<<8)|(((USHORT)Iram_Cmd_block.bf[10])));
    Xfer_Num = 0;

    send_msg74(DEBUG_OFFLINE_CALIB_MSG,0x538030,Xfer_Len);
    send_msg74(DEBUG_OFFLINE_CALIB_MSG,0x538031,Xfer_Num);

    switch ((Iram_Cmd_block.bf[1]&0x0F)) //SunFunction
    {
        case POWER_CALIBRATION:         //Power Calibration Function
            Xfer_Num = z_cmd0xF800(Xfer_Num,Xfer_Len);
            break;
        case SERVO_OFFLINE_CALIBRATION: //Servo Offline Calibration Function
            Xfer_Num = z_cmd0xF801(Xfer_Num,Xfer_Len);
            break;

        case ACCESS_PERSISTENT_TABLE:   //Acces Persistent table Information Function
            Xfer_Num = z_cmd0xF802(Xfer_Num,Xfer_Len);
            break;

        default:
            break;
    }//swicth
    SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,4,0x338030,A2B(Xfer_Len),A2B(Xfer_Num));

    if (Xfer_Num==0)
    {
        AutoCommandComplete();
        return;
    }
    if ((Xfer_Num%2)==1)
    {
        WriteSDRAM(0xa5);Xfer_Num++;
    }
    Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
    SendParameterDataToHost(Iram_Linear_Xfer_addr, MINIMUM(0xc00,Xfer_Num), TRUE);    // Send data to host. Send Command Complete.
#endif//disable offline calibration 
    
    return;
}
#if 0 //disable offline calibration    
#if PersistentTable_Revision>=8
BYTE apc_rwopen_mode, apc_rw_gain;//apc_rwopen_mode: read, write or open loop mode
#endif
/************************************************************************
* Module: z_cmd0xF800()
* Description:  ATAPI Command 0xF8 0x00,Power Calibration
* Returns:
* Params:
*************************************************************************/
USHORT z_cmd0xF800(USHORT Xfer_Num,USHORT Xfer_Len)
{
    int num,CAL_Mode;
    BYTE next_phase;

    SendMsg80(DEBUG_KPOWER_MSG,0x335000);
    next_phase = 0;
    svoDiagStatus = 0;
    svoDiagReturnValue1 = 0;
    svoDiagReturnValue2 = 0;
    svoDiagReturnValue3 = 0;

#if PersistentTable_Revision>=8
    apc_rwopen_mode = Iram_Cmd_block.bf[7];
    apc_rw_gain = Iram_Cmd_block.bf[8];
#endif

    if (Iram_Cmd_block.bf[4]==1) //Parameter Mode "1"=>Set DAC Mode, "0"=>Set Power Mode
    {
        LBYTE(svoDiagReturnValue1) = Iram_Cmd_block.bf[5];    //set DAC_L value = ATAPI command byte5.
        HBYTE(svoDiagReturnValue1) = Iram_Cmd_block.bf[6];    //set DAC_H value = ATAPI command byte6.
        SendMsgCn(DEBUG_KPOWER_MSG,2,0x335008,A2B(svoDiagReturnValue1));
    }
    else if(Iram_Cmd_block.bf[4]==2) //set Power value measured by power meter.
    {
        LBYTE(svoDiagReturnValue2) = Iram_Cmd_block.bf[5];    //set Pwr_L value = ATAPI command byte5.
        HBYTE(svoDiagReturnValue2) = Iram_Cmd_block.bf[6];    //set Pwr_H value = ATAPI command byte6.
        SendMsgCn(DEBUG_KPOWER_MSG,2,0x335009,A2B(svoDiagReturnValue2));
    }

#if (PersistentTable_Revision >= 7)
    switch (Iram_Cmd_block.bf[2]&0xC0) //LD Mode,ATAPI command byte2.7~6
    {
        case 0x00: //LD mode=CD
            svoPcalVar.fgCalDVDMode = FALSE;
            svoVar_discKindT.Bit.DiscStd = eDiscCD;
            svoPcalVar.LDMode = CD_LD;
            CAL_Mode=Iram_Cmd_block.bf[2];
            SendMsg80(DEBUG_KPOWER_MSG,0x335006); // LD_Mode==>CD
            break;

        case 0x40: //LD mode=DVD
            svoPcalVar.fgCalDVDMode = TRUE;
            svoVar_discKindT.Bit.DiscStd = eDiscDVD;
            svoPcalVar.LDMode = DVD_LD;
            CAL_Mode=(Iram_Cmd_block.bf[2] & 0x3F);
            SendMsg80(DEBUG_KPOWER_MSG,0x335007); //LD_Mode==>DVD
            break;

        case 0x80: //LD mode=BD
            svoVar_discKindT.Bit.DiscStd = eDiscBD;
            svoPcalVar.LDMode = BD_LD;
            CAL_Mode=(Iram_Cmd_block.bf[2] & 0x3F);
            SendMsg80(DEBUG_KPOWER_MSG,0x3350B4);
            break;

        case 0xC0: //LD mode=HD
            svoVar_discKindT.Bit.DiscStd = eDiscHD;
            svoPcalVar.LDMode = HD_LD;
            CAL_Mode=(Iram_Cmd_block.bf[2] & 0x3F);
            SendMsg80(DEBUG_KPOWER_MSG,0x3350D8);
            break;
    } //switch (Iram_Cmd_block.bf[2]&0xC0)
#endif // PersistentTable_Revision

    switch (CAL_Mode) //Calibration Mode,ATAPI command byte2.5~0
    {
        case 0:     /* Auto Power Calibration Mode */
            SendMsg80(DEBUG_KPOWER_MSG,0x335001);
            svoPcalVar.fgCalAutoMode = TRUE;
            break;
        case 1:     /* Manual Power Calibration Mode */
            SendMsg80(DEBUG_KPOWER_MSG,0x335002);
            svoPcalVar.fgCalAutoMode = FALSE;
            break;
        case 2:     /* Check Power Mode */
            SendMsg80(DEBUG_KPOWER_MSG,0x335003);
            return(z_cmd0xF80002(Xfer_Num,Xfer_Len));
        case 3:     /* Stop Power Calibration Mode */
            SendMsg80(DEBUG_KPOWER_MSG,0x335004);
#if PersistentTable_Revision>=8
            ServoLaserPowerCalibration(0xff,apc_rwopen_mode,apc_rw_gain);
#else
            ServoLaserPowerCalibration(0xff);
#endif
            return(0);
        case 4:     /* Save Power Calibration Data */
            SendMsg80(DEBUG_KPOWER_MSG,0x335005);
            return(z_cmd0xF80004(Xfer_Num,Xfer_Len));
        case 5:     /* Self Test Mode */
            SendMsg80(DEBUG_KPOWER_MSG,0x3350EB);
            return(z_cmd0xF80002(Xfer_Num,Xfer_Len));
        default:
            SendMsg80(DEBUG_KPOWER_MSG,0x335004);
#if PersistentTable_Revision>=8
            ServoLaserPowerCalibration(0xff,apc_rwopen_mode,apc_rw_gain);
#else
            ServoLaserPowerCalibration(0xff);
#endif
        return(0);
    }//swicth (CAL_mode)

#if (PersistentTable_Revision >= 7)
    switch (Iram_Cmd_block.bf[3]) //Power Calibration Phase byte3
    {
        case 0: //ATAPI command byte3=0x00, read channel power calibration.
            Channel_Mode = READ_CH;
            num = 0;
            break;
        case 1: //ATAPI command byte3=0x01, cooling channel power calibration.
            num = 20;
            break;
        case 2: //ATAPI command byte3=0x02, write channel power calibration R mode.
            Channel_Mode = WRITE_CH_R;
            num = 30;
            break;
        case 3: //ATAPI command byte3=0x03, write channel power calibration RW mode.
            Channel_Mode = WRITE_CH_RW;
            num = 30;
            break;
        case 4: //ATAPI command byte3=0x04, peak channel power calibration R mode.
            Channel_Mode = PEAK_CH_R;
            num = 60;
            break;
        case 5: //ATAPI command byte3=0x05,peak channel power calibration RW mode.
            Channel_Mode = PEAK_CH_RW;
            num = 60;
            break;

        case 6: //ATAPI command byte3=0x06,
            Channel_Mode = PEAK_CH_RAM;
            num = 60;
            break;
        default:
    #if PersistentTable_Revision>=8
            ServoLaserPowerCalibration(0xff,apc_rwopen_mode,apc_rw_gain);
    #else
            ServoLaserPowerCalibration(0xff);
    #endif
            return(0);
    }//swicth (Iram_Cmd_block.bf[3])
#endif // PersistentTable_Revision

    if (Iram_Cmd_block.bf[4]>3)
    {
#if PersistentTable_Revision>=8
        ServoLaserPowerCalibration(0xff,apc_rwopen_mode,apc_rw_gain);
#else
        ServoLaserPowerCalibration(0xff);
#endif
        return(0);
    }

    num = num + Iram_Cmd_block.bf[4]; //setup power calibration or power check state flow index-->svoCmdVar.cmdParam[0].
    SendMsgCn(DEBUG_KPOWER_MSG,2,0x33500A,A2B(num));
    if ((num==3)||(num==23)||(num==33)||(num==53))
        next_phase++;

    if (!svoDiagStatus)
    {
#if PersistentTable_Revision>=8
        ServoLaserPowerCalibration(num,apc_rwopen_mode,apc_rw_gain);
#else
        ServoLaserPowerCalibration(num); //setup servo command and start power calibration seqence defined in svo_p.c.
#endif
        if (svoDiagStatus) svoDiagStatus = num;
    }

    if (!svoDiagStatus)
    {
        if (next_phase)//do hold mode laser calibration
        {
            if (num==3) //Power calibration of VRDC_HOLD to DAC, after read ch. power calibration done.
            {
#if PersistentTable_Revision>=8
                ServoLaserPowerCalibration(10,apc_rwopen_mode,apc_rw_gain);
#else
                ServoLaserPowerCalibration(10);    //setup servo command and start power calibration seqence defined in svo_p.c.
#endif
                if (svoDiagStatus) svoDiagStatus = 10;
            }
             if (num==23)//Power calibration of COOLING_HOLD to DAC, after write ch. power calibration done.
            {
#if PersistentTable_Revision>=8
                ServoLaserPowerCalibration(24,apc_rwopen_mode,apc_rw_gain);
#else
                ServoLaserPowerCalibration(24);    //setup servo command and start power calibration seqence defined in svo_p.c.
#endif
                if (svoDiagStatus) svoDiagStatus = 24;
            }
            if (num==33)//Power calibration of VWDC_HOLD to DAC, after write ch. power calibration done.
            {
#if PersistentTable_Revision>=8
                ServoLaserPowerCalibration(40,apc_rwopen_mode,apc_rw_gain);
#else
                ServoLaserPowerCalibration(40);    //setup servo command and start power calibration seqence defined in svo_p.c.
#endif
                if (svoDiagStatus) svoDiagStatus = 40;
            }
            if (num==53) //Power calibration of ATT_HOLD to DAC, after write ch. power calibration done.
            {
#if PersistentTable_Revision>=8
                ServoLaserPowerCalibration(54,apc_rwopen_mode,apc_rw_gain);
#else
                ServoLaserPowerCalibration(54);    //setup servo command and start power calibration seqence defined in svo_p.c.
#endif
                if (svoDiagStatus) svoDiagStatus = 54;
            }
        }
    }

    Iram_Buf_dram_addr = BUFFER_SCRATCH;

#if (PersistentTable_Revision >= 7)
    WriteSDRAM(svoDiagStatus);Xfer_Num++;
    switch (svoPcalVar.LDMode)
    {
        case CD_LD:
            switch (Channel_Mode)
            {
                case READ_CH:
                        switch (Iram_Cmd_block.bf[4])
                        {
                            case 0://initial mode
                                WriteSDRAM(LBYTE(ReadCAL_Dac));Xfer_Num++;
                                WriteSDRAM(HBYTE(ReadCAL_Dac));Xfer_Num++;
                                break;

                            case 1://Dac mode
                                WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                                WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                                break;

                            case 3://curvefitting mode
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2rSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2rOff);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdr2sSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdr2sOff);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2c1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2c1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2c2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2c2Off);
                                WriteSDRAM(LBYTE(svoPcalVar.cdDiffDac));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.cdDiffDac));Xfer_Num++;
                                break;

                            default:
                                break;
                        }
                    break;

                case WRITE_CH_R:
                case WRITE_CH_RW:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(WriteCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(WriteCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3:
                            if (Channel_Mode == WRITE_CH_R)
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2w1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2w1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2e1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2e1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdv2eSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdv2eOff);
                            }
                            else//WRITE_CH_RW
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2w2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2w2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2e2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2e2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdv2eSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdv2eOff);
                            }
                            break;

                        default:
                            break;

                    }
                    break;

                case PEAK_CH_R:
                case PEAK_CH_RW:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(PeakCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(PeakCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3:
                            if (Channel_Mode == PEAK_CH_R)
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2pk1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2pk1Off);
                            }
                            else//PEAK_CH_RW
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2pk2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2pk2Off);
                            }
                            break;

                        default:
                            break;

                    }
                    break;

                default:
                    break;

            }
            break;

        case DVD_LD:
            switch (Channel_Mode)
            {
                case READ_CH:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(ReadCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(ReadCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3://curvefitting mode
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2rSlp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2rOff);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvr2sSlp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvr2sOff);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2c1Slp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2c1Off);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2c2Slp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2c2Off);
                            WriteSDRAM(LBYTE(svoPcalVar.dvDiffDac));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.dvDiffDac));Xfer_Num++;
                            #if (EN_Persistent_table_8 == 1)
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvbp2DiffdacSlp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvbp2Diffdacoff);
                            #endif /*(EN_Persistent_table_8 == 1)*/
                            break;

                        default:
                            break;

                        }
                    break;

                case WRITE_CH_R:
                case WRITE_CH_RW:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(WriteCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(WriteCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3://curvefitting mode
                            if (Channel_Mode == WRITE_CH_R)
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2w1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2w1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2e1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2e1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvv2e1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvv2e1Off);
                            }
                            else//WRITE_CH_RW
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2w2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2w2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2e2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2e2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvv2e2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvv2e2Off);
                            }
                            break;

                        default:
                            break;

                    }
                    break;

                case PEAK_CH_R:
                case PEAK_CH_RW:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(PeakCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(PeakCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3://curvefitting mode
                            if (Channel_Mode == PEAK_CH_R)
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2pk1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2pk1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvvwdc12pk1dacSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvvwdc12pk1dacOff);
                            }
                            else//PEAK_CH_RW
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2pk2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2pk2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvvwdc12pk2dacSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvvwdc12pk2dacOff);
                            }
                            break;

                        default:
                            break;

                    }
                    break;

                case PEAK_CH_RAM:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(PeakCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(PeakCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3://curvefitting mode
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2c3Slp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvp2c3Off);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvvwdc22cdacSlp);
                            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.dvvwdc22cdacOff);
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;

            }
            break;

        case BD_LD:
            switch (Channel_Mode)
            {
                case READ_CH:
                        switch (Iram_Cmd_block.bf[4])
                        {
                            case 0://initial mode
                                WriteSDRAM(LBYTE(ReadCAL_Dac));Xfer_Num++;
                                WriteSDRAM(HBYTE(ReadCAL_Dac));Xfer_Num++;
                                break;

                            case 1://Dac mode
                                WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                                WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                                break;

                            case 3://curvefitting mode
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2rSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2rOff);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdr2sSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdr2sOff);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2c1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2c1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2c2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2c2Off);
                                WriteSDRAM(LBYTE(svoPcalVar.bdDiffDac));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.bdDiffDac));Xfer_Num++;
                                break;

                            default:
                                break;
                        }
                    break;

                case WRITE_CH_R:
                case WRITE_CH_RW:
                    switch (Iram_Cmd_block.bf[4])
                    {
                        case 0://initial mode
                            WriteSDRAM(LBYTE(WriteCAL_Dac));Xfer_Num++;
                            WriteSDRAM(HBYTE(WriteCAL_Dac));Xfer_Num++;
                            break;

                        case 1://Dac mode
                            WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                            WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                            break;

                        case 3://curvefitting mode
                            if (Channel_Mode == WRITE_CH_R)
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2w1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2w1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2e1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2e1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdv2e1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdv2e1Off);
                            }
                            else//WRITE_CH_RW
                            {
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2w2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2w2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2e2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdp2e2Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdv2e2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.bdv2e2Off);
                            }
                            break;

                        default:
                            break;

                    }
                    break;

                case PEAK_CH_R:
                case PEAK_CH_RW:
                    break;

                default:
                    break;

            }
            break;

#if (EN_Persistent_table_8 == 1)
        case HD_LD:
            switch (Channel_Mode)
            {
                case READ_CH:
                        switch (Iram_Cmd_block.bf[4])
                        {
                            case 0://initial mode
                                WriteSDRAM(LBYTE(ReadCAL_Dac));Xfer_Num++;
                                WriteSDRAM(HBYTE(ReadCAL_Dac));Xfer_Num++;
                                break;

                            case 1://Dac mode
                                WriteSDRAM(LBYTE(svoPcalVar.FPDO));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.FPDO));Xfer_Num++;
                                WriteSDRAM(LBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.VRDC_VWDCX));Xfer_Num++;
                                break;

                            case 3://curvefitting mode
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdp2rSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdp2rOff);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdr2sSlp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdr2sOff);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdp2c1Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdp2c1Off);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdp2c2Slp);
                                Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.hdp2c2Off);
                                WriteSDRAM(LBYTE(svoPcalVar.hdDiffDac));Xfer_Num++;
                                WriteSDRAM(HBYTE(svoPcalVar.hdDiffDac));Xfer_Num++;
                                break;

                            default:
                                break;
                        }
                    break;

                case WRITE_CH_R:
                case WRITE_CH_RW:
                case PEAK_CH_R:
                case PEAK_CH_RW:
                    break;

                default:
                    break;

            }
            break;
#endif
        default:
            break;
    }
#endif // PersistentTable_Revision
    return(Xfer_Num);
}


/************************************************************************
* Module: z_cmd0xF80002()
* Description:  ATAPI Command 0xF8 0x00 0x02, Power Check
* Returns:
* Params:
*************************************************************************/
USHORT z_cmd0xF80002(USHORT Xfer_Num,USHORT Xfer_Len)
{
    int num;

#if (PersistentTable_Revision >= 7)
    switch (Iram_Cmd_block.bf[2]&0xC0) //LD Mode,ATAPI command byte2.7~6
    {
        case 0x00: //LD mode=CD
            svoPcalVar.LDMode = CD_LD;
            break;

        case 0x40: //LD mode=DVD
            svoPcalVar.LDMode = DVD_LD;
            break;

        case 0x80: //LD mode=BD
            svoPcalVar.LDMode = BD_LD;
            break;

        case 0xC0: //LD mode=HD
            svoPcalVar.LDMode = HD_LD;
            break;
    } //switch (Iram_Cmd_block.bf[2]&0xC0)

    switch (Iram_Cmd_block.bf[3]) //Power Calibration Phase byte3
    {
        case 0:
            Channel_Mode = READ_CH;
            num = 100;    //read 1ch. power check
            break;
        case 1:
            CHECK_POWER_MODE = CHECK_READ_APC_2CH;
            Channel_Mode = COOLING_CH_R;
            num = 120;    //read 2ch power check for only DVD R mode
            break;
        case 2:
            Channel_Mode = WRITE_CH_R;
            num = 130;    //write power check R Mode(OSCEN turn on)
            break;
        case 3:
            Channel_Mode = WRITE_CH_RW;
            num = 130;    //write power check RW Mode(OSCEN turn off)
            break;
        case 4:
            Channel_Mode = PEAK_CH_R;
            num = 160;    //peak power checkR Mode(OSCEN turn on)
            break;
        case 5:
            Channel_Mode = PEAK_CH_RW;
            num = 160;    //peak power check RW Mode(OSCEN turn off)
            break;
        case 6:
            Channel_Mode = PEAK_CH_RAM;
            num = 160;    //peak power check RW Mode(OSCEN turn off)
            break;

        case 7:
            CHECK_POWER_MODE = CHECK_READ_FIX_2CH;
            Channel_Mode = COOLING_CH_R;
            num = 120;    //read 2ch fix power check for only DVD R mode
            break;
        case 8:
            CHECK_POWER_MODE = CHECK_READ_FIX_1CH;
            Channel_Mode = COOLING_CH_R;
            num = 120;    //cooling power check for only DVD R mode
            break;
        case 9:
            CHECK_POWER_MODE = CHECK_READ_APC_2CH;
            Channel_Mode = COOLING_CH_RW;
            num = 120;    //read 2ch power check for only DVD R mode
            break;
        case 10:
            CHECK_POWER_MODE = CHECK_READ_FIX_2CH;
            Channel_Mode = COOLING_CH_RW;
            num = 120; //read 2ch fix power check for only DVD R mode
            break;

        case 11:
            CHECK_POWER_MODE = CHECK_READ_FIX_1CH;
            Channel_Mode = COOLING_CH_RW;
            num = 120; //cooling power check for only DVD RW mode
            break;

        case 12:
            Channel_Mode = WRITE_CH_R;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            fAPC = TRUE;
            num = 130;    //write power check R Mode(OSCEN turn on)
            break;
        case 13:
            Channel_Mode = WRITE_CH_RW;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            fAPC = TRUE;
            num = 130;    //write power check RW Mode(OSCEN turn off)
            break;

        case 14:
            Channel_Mode = WRITE_CH_R;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            fAPC = FALSE;
            num = 130;    //write power check R Mode(OSCEN turn on)
            break;

        case 15:
            Channel_Mode = WRITE_CH_RW;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            fAPC = FALSE;
            num = 130;    //write power check RW Mode(OSCEN turn off)
            break;

        case 16:
            Channel_Mode = PEAK_CH_R;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            num = 160;    //peak power checkR Mode(OSCEN turn on)
            break;

        case 17:
            Channel_Mode = PEAK_CH_RW;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            num = 160;    //peak power check RW Mode(OSCEN turn off)
            break;

        case 18:
            Channel_Mode = PEAK_CH_RAM;
            fCheck_Pluse_Power = TRUE;    //check pulse power
            num = 160;    //peak power check RW Mode(OSCEN turn off)
            break;

        case 19:
            Channel_Mode = READ_CH;
            num = 170;    //read 1ch. power check
            break;

        default:
    #if PersistentTable_Revision>=8
            ServoLaserPowerCalibration(0xff,apc_rwopen_mode,apc_rw_gain);
    #else
            ServoLaserPowerCalibration(0xff);
    #endif
            return(0);
    } //swicth(Iram_Cmd_block.bf[3])
#endif // PersistentTable_Revision

#if PersistentTable_Revision>=8
    ServoLaserPowerCalibration(num,apc_rwopen_mode,apc_rw_gain);
#else
    ServoLaserPowerCalibration(num);
#endif
    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    WriteSDRAM(svoDiagStatus);Xfer_Num++;
    WriteSDRAM(LBYTE(svoDiagReturnValue1));Xfer_Num++;
    WriteSDRAM(HBYTE(svoDiagReturnValue1));Xfer_Num++;
    WriteSDRAM(LBYTE(svoDiagReturnValue2));Xfer_Num++;
    WriteSDRAM(HBYTE(svoDiagReturnValue2));Xfer_Num++;

    switch(num)
    {
        case 100:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2rOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2rSlp);
            break;
        case 120:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2rOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2rSlp);
            break;
        case 130:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2wOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2wSlp);
            break;
        case 150:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2w2Off);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2w2Slp);
            break;
        case 160:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2pkOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2pkSlp);
            break;
        case 110:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdr2sOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdr2sSlp);
            break;
        case 131:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdv2eOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdv2eSlp);
            break;
        case 151:
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2eOff);
            Xfer_Num = uSendFloNum(Xfer_Num,svoPcalVar.cdp2eSlp);
            break;
    } //switch(num)
    return(Xfer_Num);
}

/************************************************************************
* Module: z_cmd0xF80004()
* Description:  ATAPI Command 0xF8 0x00 0x04: Save result to flash
* Returns:
* Params:
*************************************************************************/
// ATAPI Command 0xF8 0x00 0x04, Save result to flash //
USHORT z_cmd0xF80004(USHORT Xfer_Num,USHORT Xfer_Len)
{
    z_calLoad_EepromMap();
    z_calWrite_PowerCalibrationParam();
    z_calWrite_PcalParamSum();
    z_calFlash_EepromMap();
    #if PersistentTable_Revision>=8
    if(svoPcalVar.LDMode == CD_LD) //read the value for make sure the save value
    {
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(CD_OFFSET_DAC_THRESHOLD_ADDR)),
                F4B(GetPersistentTable4F(CD_SLOPE_DAC_THRESHOLD_ADDR)));
        }

        if ((apc_rwopen_mode==3)&&(apc_rw_gain==2)) //DAC_Threshhold (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(CD_OFFSET_DAC_THRESHOLD_ADDR_ON)),
                F4B(GetPersistentTable4F(CD_SLOPE_DAC_THRESHOLD_ADDR_ON)));
        }

        if ((apc_rwopen_mode==3)&&(apc_rw_gain==3)) //Delta_0 (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(CD_OFFSET_DAC_0_ADDR)),
                F4B(GetPersistentTable4F(CD_SLOPE_DAC_0_ADDR)));
        }
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==4)) //Delta_1 (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(CD_OFFSET_DAC_1_ADDR)),
                F4B(GetPersistentTable4F(CD_SLOPE_DAC_1_ADDR)));
        }
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==5)) //Delta_2 (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(CD_OFFSET_DAC_2_ADDR)),
                F4B(GetPersistentTable4F(CD_SLOPE_DAC_2_ADDR)));
        }

        if ((apc_rwopen_mode==2)&&(apc_rw_gain==1)) //VWPOW Low GAIN
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E8,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_LG_ADDR)));
        }
        if ((apc_rwopen_mode==2)&&(apc_rw_gain==2)) //VWPOW Mid GAIN
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E9,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_MG_ADDR)));
        }
        if ((apc_rwopen_mode==2)&&(apc_rw_gain==3)) //VWPOW Hi  GAIN
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350EA,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_HG_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350EA,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_HG_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E2,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G5X_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E3,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G10X_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E4,F4B(GetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G25X_ADDR)));
        }
    }
    else if (svoPcalVar.LDMode == DVD_LD)
    {
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(DV_OFFSET_DAC_THRESHOLD_ADDR)),
                F4B(GetPersistentTable4F(DV_SLOPE_DAC_THRESHOLD_ADDR)));
        }

        if ((apc_rwopen_mode==3)&&(apc_rw_gain==2)) //DAC_Threshhold (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(DV_OFFSET_DAC_THRESHOLD_ADDR_ON)),
                F4B(GetPersistentTable4F(DV_SLOPE_DAC_THRESHOLD_ADDR_ON)));
        }

        if ((apc_rwopen_mode==3)&&(apc_rw_gain==3)) //Delta_0 (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(DV_OFFSET_DAC_0_ADDR)),
                F4B(GetPersistentTable4F(DV_SLOPE_DAC_0_ADDR)));
        }
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==4)) //Delta_1 (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(DV_OFFSET_DAC_1_ADDR)),
                F4B(GetPersistentTable4F(DV_SLOPE_DAC_1_ADDR)));
        }
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==5)) //Delta_2 (oscen on)
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(DV_OFFSET_DAC_2_ADDR)),
                F4B(GetPersistentTable4F(DV_SLOPE_DAC_2_ADDR)));
        }


        if ((apc_rwopen_mode==2)&&(apc_rw_gain==1)) //VWPOW Low GAIN
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E8,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VWPOWER_LG_ADDR)));
        }
        if ((apc_rwopen_mode==2)&&(apc_rw_gain==2)) //VWPOW Mid GAIN
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E9,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VWPOWER_MG_ADDR)));
        }
        if ((apc_rwopen_mode==2)&&(apc_rw_gain==3)) //VWPOW Hi  GAIN
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350EA,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VWPOWER_HG_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E1,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G1X_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E2,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G5X_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E3,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G10X_ADDR)));
        }
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350E4,F4B(GetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G25X_ADDR)));
        }

    }
    else if (svoPcalVar.LDMode == BD_LD)
    {
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(BD_OFFSET_DAC_THRESHOLD_ADDR)),
                F4B(GetPersistentTable4F(BD_SLOPE_DAC_THRESHOLD_ADDR)));
        }

        if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D2,F4B(GetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G1X_ADDR)));
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D3,F4B(GetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G5X_ADDR)));
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D4,F4B(GetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G10X_ADDR)));
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D5,F4B(GetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G25X_ADDR)));
    }
    else
    {
        if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
        {
            SendMsgCnB(DEBUG_KPOWER_MSG,8,0x3350D0,F4B(GetPersistentTable4F(HD_OFFSET_DAC_THRESHOLD_ADDR)),
                F4B(GetPersistentTable4F(HD_SLOPE_DAC_THRESHOLD_ADDR)));
        }

        if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D2,F4B(GetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G1X_ADDR)));
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D3,F4B(GetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G5X_ADDR)));
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D4,F4B(GetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G10X_ADDR)));
        if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
            SendMsgCnB(DEBUG_KPOWER_MSG,4,0x3350D5,F4B(GetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G25X_ADDR)));
    }
    #endif
    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    WriteSDRAM(0x00);Xfer_Num++;
    return(Xfer_Num);
}


/************************************************************************
* Module: z_cmd0xF801()
* Description:  ATAPI Command 0xF8 0x01,Offline Servo Calibration Function
* Returns:
* Params:
*************************************************************************/
// ATAPI Command 0xF8 0x01, Offline Servo Calibration Function //
USHORT z_cmd0xF801(USHORT Xfer_Num,USHORT Xfer_Len)
{
    int     num,cmode,ctimes;
    BYTE    OPmode;

    svoDiagReturnValue1 = 0;
    svoDiagReturnValue2 = 0;
    LBYTE(svoDiagReturnValue1) = Iram_Cmd_block.bf[4];
    HBYTE(svoDiagReturnValue1) = Iram_Cmd_block.bf[5];
    ctimes = Iram_Cmd_block.bf[6];
    SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,2,0x335008,A2B(svoDiagReturnValue1));
    SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x338024,F4B((float)ctimes));
    switch (Iram_Cmd_block.bf[2]&0xC0) //LD Mode,ATAPI command byte2.7~6
    {
        case 0x00: //LD mode=CD
            svoPcalVar.fgCalDVDMode = FALSE;
            cmode=(Iram_Cmd_block.bf[2] & 0x3F);
            OPmode=Iram_Cmd_block.bf[3];
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x335006); // LD_Mode==>CD
            break;

        case 0x40: //LD mode=DVD
            svoPcalVar.fgCalDVDMode = TRUE;
            cmode=(Iram_Cmd_block.bf[2] & 0x3F);
            OPmode=Iram_Cmd_block.bf[3];
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x335007); //LD_Mode==>DVD
            break;

        case 0x80: //Reserved
            break;
        case 0xC0: //Reserved
            break;
    } //switch (Iram_Cmd_block.bf[2]&0xC0)

    switch (cmode) //Servo Calibration Mode,ATAPI command byte2.5~0
    {
        case 0:     /* Focus Balance K2 Calibration Mode */
            num = FEBG_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338000);//Focus_Balance_GAIN_Calibration_Mode
            break;
        case 1:     /* Focus Balance DAC Calibration Mode (based on RRFp2p)*/
            num = FEBC_RRF_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338001);//Focus_Balance_DAC_Calibration_Mode
            break;
        case 2:     /* RRF Gain Calibration Mode */
            num = RRFG_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338002);//RRF_Gain_Calibration_Mode
            break;
        case 3:     /* Beta Offset Calibration Mode */
            num = BETA_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338003);//Beta_Offset_Calibration Mode
            break;
        case 4:     /* Tilt Servo Calibration Mode */
            num = TILT_TE_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338004);//Tilt_Servo_Calibration Mode
            break;
        case 5:     /* Focus Balance DAC Calibration Mode (based on jitter)*/
            num = FEBC_JIT_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338001);//Focus_Balance_DAC_Calibration_Mode
            break;
        case 6:     /* Tilt Servo Calibration Mode (based on jitter)*/
            num = TILT_JIT_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338004);//Tilt_Servo_Calibration Mode
            break;
        case 7:     /* Focus Balance DAC Calibration Mode (based on TEp2p)*/
            num = FEBC_TE_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338001);//Focus_Balance_DAC_Calibration_Mode
            break;
        case 8:     /* Tilt Servo Calibration Mode (based on RRFp2p)*/
            num = TILT_RRF_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338004);//Tilt_Servo_Calibration Mode
            break;
        case 9:     /* SPP2BG Calibration Mode */
            num = SPP2BG_ADJ;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338006);//SPP2BG Calibration Mode
            break;
        case 15:     /* Move Sled to out side */
            num = MOVE_SLED_OUT;
//            send_msg80(DEBUG_OFFLINE_CALIB_MSG,0x53,0x80,0x05);
            break;
        case 16:     /* Move Sled to in side */
            num = MOVE_SLED_IN;
//            send_msg80(DEBUG_OFFLINE_CALIB_MSG,0x53,0x80,0x06);
            break;

        default:
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338020);//Unknow_Calibration Mode!!
            return(0);
    }//swicth (cmode)

    if(!((cmode==15)||(cmode==16)))
    {
        if (((svoPcalVar.fgCalDVDMode == FALSE)&&(svoVar_discKindT.Bit.DiscStd == eDiscDVD)) ||
            ((svoPcalVar.fgCalDVDMode == TRUE)&&(svoVar_discKindT.Bit.DiscStd == eDiscCD))  ||
            (St_no_disc))
        {
            BUILD_SENSE(0x05,0x20,0x00,0x5a);/* INVALID COMMAND OPERATION CODE */
            return(0);
        }
    }
    if (OPmode == 0) //Do_Servo_Calibration
    {
        if (num==BETA_ADJ)
        {
            if (svoPcalVar.fgCalDVDMode == FALSE)
                svoPcalVar.cdBetaOffset=svoDiagReturnValue1;
            else
                svoPcalVar.dvBetaOffset=svoDiagReturnValue1;
        }
        else
        {
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338021);//"Do_Servo_Calibration"
            svoDiagReturnValue1=0;
            svoDiagReturnValue2=0;
        }
        svoDiagStatus = 0x00;
        ServoCalibration(num);
        Iram_Buf_dram_addr = BUFFER_SCRATCH;
/***********************************************************************************
*  Return Data Note
* 1.When Focus Balance K2 Calibration Mode  ==>svoDiagReturnValue1=FeK2Value
*                                              svoDiagReturnValue2=BHO_DIFFVALUE
                                               svoDiagReturnValue3=SPP2BGValue
                                               svoDiagReturnValue4=BHO_DIFFVALUE
* 2.When Focus Balance DAC Calibration Mode ==>svoDiagReturnValue1
*                                              svoDiagReturnValue2
* 3.When RRF Gain Calibration Mode          ==>svoDiagReturnValue1
*                                              svoDiagReturnValue2
* 4.When Tilt DAC Calibration Mode          ==>svoDiagReturnValue1=
*                                              svoDiagReturnValue2
************************************************************************************ */
        WriteSDRAM(LBYTE(svoDiagReturnValue1));Xfer_Num++;
        WriteSDRAM(HBYTE(svoDiagReturnValue1));Xfer_Num++;
        WriteSDRAM(LBYTE(svoDiagReturnValue2));Xfer_Num++;
        WriteSDRAM(HBYTE(svoDiagReturnValue2));Xfer_Num++;
        #if (EN_Persistent_table_8 == 1)
        if (num==FEBG_ADJ)
        {
            WriteSDRAM(LBYTE(svoDiagReturnValue3));Xfer_Num++;
            WriteSDRAM(HBYTE(svoDiagReturnValue3));Xfer_Num++;
            WriteSDRAM(LBYTE(svoDiagReturnValue4));Xfer_Num++;
            WriteSDRAM(HBYTE(svoDiagReturnValue4));Xfer_Num++;
        }
        #endif/*(EN_Persistent_table_8 == 1)*/
        WriteSDRAM(svoDiagStatus);Xfer_Num++;
        WriteSDRAM(0x00);Xfer_Num++;
        return(Xfer_Num);
    }
    else if (OPmode == 1) //Save_Servo_Calibration
    {
        Xfer_Num = z_cmd0xF801XX01(Xfer_Num,num,ctimes);
    }
    return(0);
}

/************************************************************************
* Module: z_cmd0xF802()
* Description:  ATAPI Command 0xF8 0x02,Access Persistent table Information Function
* Returns:
* Params:
*************************************************************************/
// ATAPI Command 0xF8 0x02, Acces Persistent table Information Function //

USHORT z_cmd0xF802(USHORT Xfer_Num,USHORT Xfer_Len)
{
    int     cmode;

    ULONG   start_addr;
    StLongU Offset_addr;
    BYTE *  AdrP;
    BOOL    bAccessFlashRom;

    bAccessFlashRom = TRUE;
    svoDiagReturnValue1 = 0;
    svoDiagReturnValue2 = 0;
    SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338005);//Acces_PersistentTable_Mode
    switch (Iram_Cmd_block.bf[2]) //Acces item,ATAPI command byte2.7~0
    {
        case 0x00: //Access item = FIRMW_INQ_VENDOR
            bAccessFlashRom = TRUE;
            Offset_addr.Long = VenderID_ADDR;            /* the offset of Starting address  */
            if ( Xfer_Len > VenderID_LEN )
                Xfer_Len = VenderID_LEN;
            Xfer_Num = VenderID_LEN;
            break;

        case 0x01: //Access item = FIRMW_INQ_REVISION
            bAccessFlashRom = TRUE;
            Offset_addr.Long = FirmWareRev_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > FirmWareRev_LEN )
                Xfer_Len = FirmWareRev_LEN;
            Xfer_Num = FirmWareRev_LEN;
            break;

        case 0x02: //Access item = FIRMW_INQ_PRODUCT
            bAccessFlashRom = TRUE;
            Offset_addr.Long = ProductID_ADDR;           /* the offset of Starting address  */
            if ( Xfer_Len > ProductID_LEN )
                Xfer_Len = ProductID_LEN;
            Xfer_Num = ProductID_LEN;
            break;

        case 0x03: //Access item = AP_INQ_REVISION
            bAccessFlashRom = TRUE;
            Offset_addr.Long = APRev_ADDR;               /* the offset of Starting address  */
            if ( Xfer_Len > APRev_LEN )
                Xfer_Len = APRev_LEN;
            Xfer_Num = APRev_LEN;
            break;

        case 0x04: //Access item = Serial Number
            bAccessFlashRom = TRUE;
            Offset_addr.Long = SerialNumber_ADDR;        /* the offset of Starting address  */
            if ( Xfer_Len > SerialNumber_LEN )
                Xfer_Len = SerialNumber_LEN;
            Xfer_Num = SerialNumber_LEN;
            break;

        case 0x05: //Access item = Produce Line Number
            bAccessFlashRom = TRUE;
            Offset_addr.Long = ProduceLineNumber_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > ProduceLineNumber_LEN )
                Xfer_Len = ProduceLineNumber_LEN;
            Xfer_Num = ProduceLineNumber_LEN;
            break;

        case 0x06: //Access item = Persistent Table Number
            bAccessFlashRom = TRUE;
            Offset_addr.Long = PersistentTable_ID_ADDR;  /* the offset of Starting address  */
            if ( Xfer_Len > PersistentTableRev_LEN )
                Xfer_Len = PersistentTableRev_LEN;
            Xfer_Num = PersistentTableRev_LEN;
            break;

        case 0x07: //Access item = CONFIG_INQ_REVISION
            bAccessFlashRom = TRUE;
            Offset_addr.Long = ConfigRev_ADDR;  /* the offset of Starting address  */
            if ( Xfer_Len > ConfigRev_LEN )
                Xfer_Len = ConfigRev_LEN;
            Xfer_Num = ConfigRev_LEN;
            break;

        case 0x08: //Access item = Customer Serial Number
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CustomerSerialNumber_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CustomerSerialNumber_LEN )
                Xfer_Len = CustomerSerialNumber_LEN;
            Xfer_Num = CustomerSerialNumber_LEN;
            break;

        case 0x09: //Access item = Power meter
            bAccessFlashRom = TRUE;
            Offset_addr.Long = Power_Meter_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > Power_Meter_LEN )
                Xfer_Len = Power_Meter_LEN;
            Xfer_Num = Power_Meter_LEN;
            break;

        case 0x0A: //Access item = Disc Category
            bAccessFlashRom = TRUE;
            Offset_addr.Long = Disc_Category_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > Disc_Category_LEN )
                Xfer_Len = Disc_Category_LEN;
            Xfer_Num = Disc_Category_LEN;
            break;

        case 0x10: //Access item = CD FBDAC
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_CAL_FBDAC_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > CD_CAL_FBDAC_LEN )
                Xfer_Len = CD_CAL_FBDAC_LEN;
            Xfer_Num = CD_CAL_FBDAC_LEN;
            break;

        case 0x11: //Access item = CD FEBG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_CAL_FEOBG_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > CD_CAL_FEOBG_LEN )
               Xfer_Len = CD_CAL_FEOBG_LEN;
               Xfer_Num = CD_CAL_FEOBG_LEN;
            break;

        case 0x12: //Access item = CD RRFG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_CAL_RRFG_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > CD_CAL_RRFG_LEN )
                Xfer_Len = CD_CAL_RRFG_LEN;
            Xfer_Num = CD_CAL_RRFG_LEN;
            break;

        case 0x13: //Access item = CD Beta Offset
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_CAL_BETA_OFFSET_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > CD_CAL_BETA_OFFSET_LEN )
                Xfer_Len = CD_CAL_BETA_OFFSET_LEN;
            Xfer_Num = CD_CAL_BETA_OFFSET_LEN;
            break;

        case 0x14: //Access item = CD BIOS Power
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_BIAS_POWER_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > CD_BIAS_POWER_LEN )
                Xfer_Len = CD_BIAS_POWER_LEN;
            Xfer_Num = CD_BIAS_POWER_LEN;
            break;

        case 0x15: //Access item = CD VWDC2VRDC Gain
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_VWDC2VRDC_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > CD_VWDC2VRDC_LEN )
                Xfer_Len = CD_VWDC2VRDC_LEN;
            Xfer_Num = CD_VWDC2VRDC_LEN;
            break;

//        case 0x16: //Access item = CD VWDC2VWDC1 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = CD_VWDC2VWDC1_ADDR;         /* the offset of Starting address  */
//            if ( Xfer_Len > CD_VWDC2VWDC1_LEN )
//                Xfer_Len = CD_VWDC2VWDC1_LEN;
//            Xfer_Num = CD_VWDC2VWDC1_LEN;
//            break;

        case 0x17: //Access item = DVD FBDAC0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_FBDAC0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_FBDAC0_LEN )
                Xfer_Len = DV_CAL_FBDAC0_LEN;
            Xfer_Num = DV_CAL_FBDAC0_LEN;
            break;

        case 0x18: //Access item = DVD FEBG0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_FEOBG0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_FEOBG0_LEN )
               Xfer_Len = DV_CAL_FEOBG0_LEN;
               Xfer_Num = DV_CAL_FEOBG0_LEN;
            break;

        case 0x19: //Access item = DVD FBDAC1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_FBDAC1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_FBDAC1_LEN )
                Xfer_Len = DV_CAL_FBDAC1_LEN;
            Xfer_Num = DV_CAL_FBDAC1_LEN;
            break;

        case 0x1A: //Access item = DVD FEBG1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_FEOBG1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_FEOBG1_LEN )
                Xfer_Len = DV_CAL_FEOBG1_LEN;
            Xfer_Num = DV_CAL_FEOBG1_LEN;
            break;

        case 0x1B: //Access item = DVD Plus RRFG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_PLUS_RRFG_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_PLUS_RRFG_LEN )
                Xfer_Len = DV_CAL_PLUS_RRFG_LEN;
            Xfer_Num = DV_CAL_PLUS_RRFG_LEN;
            break;

        case 0x1C: //Access item = DVD Plus Beta Offset
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_PLUS_BETA_OFFSET_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_PLUS_BETA_OFFSET_LEN )
                Xfer_Len = DV_CAL_PLUS_BETA_OFFSET_LEN;
            Xfer_Num = DV_CAL_PLUS_BETA_OFFSET_LEN;
            break;

        case 0x1D: //Access item = DVD Minus RRFG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_MINUS_RRFG_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_MINUS_RRFG_LEN )
                Xfer_Len = DV_CAL_MINUS_RRFG_LEN;
            Xfer_Num = DV_CAL_MINUS_RRFG_LEN;
            break;

        case 0x1E: //Access item = DVD Minus Beta Offset
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_MINUS_BETA_OFFSET_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_MINUS_BETA_OFFSET_LEN )
                Xfer_Len = DV_CAL_MINUS_BETA_OFFSET_LEN;
            Xfer_Num = DV_CAL_MINUS_BETA_OFFSET_LEN;
            break;

        case 0x1F: //Access item = DVD TILT DAC0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_TILT_DAC0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_TILT_DAC0_LEN )
                Xfer_Len = DV_CAL_TILT_DAC0_LEN;
            Xfer_Num = DV_CAL_TILT_DAC0_LEN;
            break;

        case 0x20: //Access item = DVD TILT DAC1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_TILT_DAC1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_TILT_DAC1_LEN )
                Xfer_Len = DV_CAL_TILT_DAC1_LEN;
            Xfer_Num = DV_CAL_TILT_DAC1_LEN;
            break;

        case 0x21: //Access item = DVD TILT DAC2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_TILT_DAC2_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_TILT_DAC2_LEN )
                Xfer_Len = DV_CAL_TILT_DAC2_LEN;
            Xfer_Num = DV_CAL_TILT_DAC2_LEN;
            break;

        case 0x22: //Access item = DVD TILT DAC3
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_TILT_DAC3_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_TILT_DAC3_LEN )
                Xfer_Len = DV_CAL_TILT_DAC3_LEN;
            Xfer_Num = DV_CAL_TILT_DAC3_LEN;
            break;

        case 0x23: //Access item = DVD TILT DAC4
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_TILT_DAC4_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_TILT_DAC4_LEN )
                Xfer_Len = DV_CAL_TILT_DAC4_LEN;
            Xfer_Num = DV_CAL_TILT_DAC4_LEN;
            break;

        case 0x24: //Access item = DVD BIOS Power
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_BIAS_POWER_ADDR;    /* the offset of Starting address  */
            if ( Xfer_Len > DV_BIAS_POWER_LEN )
                Xfer_Len = DV_BIAS_POWER_LEN;
            Xfer_Num = DV_BIAS_POWER_LEN;
            break;

        case 0x25: //Access item = DVD VWDC2VRDC Gain
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_VWDC2VRDC_ADDR;    /* the offset of Starting address  */
            if ( Xfer_Len > DV_VWDC2VRDC_LEN  )
                Xfer_Len = DV_VWDC2VRDC_LEN ;
            Xfer_Num = DV_VWDC2VRDC_LEN ;
            break;

//        case 0x26: //Access item = DVD VWDC2VWDC1 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = DV_VWDC2VWDC1_ADDR;    /* the offset of Starting address  */
//            if ( Xfer_Len > DV_VWDC2VWDC1_LEN  )
//                Xfer_Len = DV_VWDC2VWDC1_LEN ;
//            Xfer_Num = DV_VWDC2VWDC1_LEN ;
//            break;

//        case 0x27: //Access item = DVD VWDC2VWDC2 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = DV_VWDC2VWDC2_ADDR;   /* the offset of Starting address  */
//            if ( Xfer_Len > DV_VWDC2VWDC2_LEN )
//                Xfer_Len = DV_VWDC2VWDC2_LEN;
//            Xfer_Num = DV_VWDC2VWDC2_LEN;
//            break;

        case 0x28: // Access item = CD SPP2BG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_CAL_SPP2BG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_CAL_SPP2BG_LEN )
                Xfer_Len = CD_CAL_SPP2BG_LEN;
            Xfer_Num = CD_CAL_SPP2BG_LEN;
            break;
        case 0x29: // Access item = DVD SPP2BG0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_SPP2BG0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_SPP2BG0_LEN )
                Xfer_Len = DV_CAL_SPP2BG0_LEN;
            Xfer_Num = DV_CAL_SPP2BG0_LEN;
            break;
        case 0x2A: // Access item = DVD SPP2BG1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_CAL_SPP2BG1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_CAL_SPP2BG1_LEN )
                Xfer_Len = DV_CAL_SPP2BG1_LEN;
            Xfer_Num = DV_CAL_SPP2BG1_LEN;
            break;

        case 0x2B: //Access item = HDDVD FBDAC0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_FBDAC0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_FBDAC0_LEN )
                Xfer_Len = HD_CAL_FBDAC0_LEN;
            Xfer_Num = HD_CAL_FBDAC0_LEN;
            break;

        case 0x2C: //Access item = HDDVD FEBG0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_FEOBG0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_FEOBG0_LEN )
               Xfer_Len = HD_CAL_FEOBG0_LEN;
               Xfer_Num = HD_CAL_FEOBG0_LEN;
            break;

        case 0x2D: // Access item = HDDVD SPP2BG0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_SPP2BG0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_SPP2BG0_LEN )
                Xfer_Len = HD_CAL_SPP2BG0_LEN;
            Xfer_Num = HD_CAL_SPP2BG0_LEN;
            break;

        case 0x2E: //Access item = HDDVD FBDAC1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_FBDAC1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_FBDAC1_LEN )
                Xfer_Len = HD_CAL_FBDAC1_LEN;
            Xfer_Num = HD_CAL_FBDAC1_LEN;
            break;

        case 0x2F: //Access item = HDDVD FEBG1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_FEOBG1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_FEOBG1_LEN )
                Xfer_Len = HD_CAL_FEOBG1_LEN;
            Xfer_Num = HD_CAL_FEOBG1_LEN;
            break;

        case 0x30: // Access item = HDDVD SPP2BG1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_SPP2BG1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_SPP2BG1_LEN )
                Xfer_Len = HD_CAL_SPP2BG1_LEN;
            Xfer_Num = HD_CAL_SPP2BG1_LEN;
            break;
        case 0x31: //Access item = HDDVD RRFG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_RRFG_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_RRFG_LEN )
                Xfer_Len = HD_CAL_RRFG_LEN;
            Xfer_Num = HD_CAL_RRFG_LEN;
            break;

        case 0x32: //Access item = HDDVD Beta Offset
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_BETA_OFFSET_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_BETA_OFFSET_LEN )
                Xfer_Len = HD_CAL_BETA_OFFSET_LEN;
            Xfer_Num = HD_CAL_BETA_OFFSET_LEN;
            break;

        case 0x33: //Access item = HDDVD TILT DAC0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_TILT_DAC0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_TILT_DAC0_LEN )
                Xfer_Len = HD_CAL_TILT_DAC0_LEN;
            Xfer_Num = HD_CAL_TILT_DAC0_LEN;
            break;

        case 0x34: //Access item = HDDVD TILT DAC1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_TILT_DAC1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_TILT_DAC1_LEN )
                Xfer_Len = HD_CAL_TILT_DAC1_LEN;
            Xfer_Num = HD_CAL_TILT_DAC1_LEN;
            break;

        case 0x35: //Access item = HDDVD TILT DAC2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_TILT_DAC2_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_TILT_DAC2_LEN )
                Xfer_Len = HD_CAL_TILT_DAC2_LEN;
            Xfer_Num = HD_CAL_TILT_DAC2_LEN;
            break;

        case 0x36: //Access item = HDDVD TILT DAC3
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_TILT_DAC3_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_TILT_DAC3_LEN )
                Xfer_Len = HD_CAL_TILT_DAC3_LEN;
            Xfer_Num = HD_CAL_TILT_DAC3_LEN;
            break;

        case 0x37: //Access item = HDDVD TILT DAC4
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_CAL_TILT_DAC4_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > HD_CAL_TILT_DAC4_LEN )
                Xfer_Len = HD_CAL_TILT_DAC4_LEN;
            Xfer_Num = HD_CAL_TILT_DAC4_LEN;
            break;

        case 0x38: //Access item = HDDVD BIOS Power
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_BIAS_POWER_ADDR;    /* the offset of Starting address  */
            if ( Xfer_Len > HD_BIAS_POWER_LEN )
                Xfer_Len = HD_BIAS_POWER_LEN;
            Xfer_Num = HD_BIAS_POWER_LEN;
            break;

//        case 0x39: //Access item = HDDVD VWDC2VRDC Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = HD_VWDC2VRDC_ADDR;    /* the offset of Starting address  */
//            if ( Xfer_Len > HD_VWDC2VRDC_LEN  )
//                Xfer_Len = HD_VWDC2VRDC_LEN ;
//            Xfer_Num = HD_VWDC2VRDC_LEN ;
//            break;
//
//        case 0x3A: //Access item = HDDVD VWDC2VWDC1 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = HD_VWDC2VWDC1_ADDR;    /* the offset of Starting address  */
//            if ( Xfer_Len > HD_VWDC2VWDC1_LEN  )
//                Xfer_Len = HD_VWDC2VWDC1_LEN ;
//            Xfer_Num = HD_VWDC2VWDC1_LEN ;
//            break;
//
//        case 0x3B: //Access item = HDDVD VWDC2VWDC2 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = HD_VWDC2VWDC2_ADDR;   /* the offset of Starting address  */
//            if ( Xfer_Len > HD_VWDC2VWDC2_LEN )
//                Xfer_Len = HD_VWDC2VWDC2_LEN;
//            Xfer_Num = HD_VWDC2VWDC2_LEN;
//            break;

        case 0x3C: //Access item = BD FBDAC0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_FBDAC0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_FBDAC0_LEN )
                Xfer_Len = BD_CAL_FBDAC0_LEN;
            Xfer_Num = BD_CAL_FBDAC0_LEN;
            break;

        case 0x3D: //Access item = BD FEBG0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_FEOBG0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_FEOBG0_LEN )
               Xfer_Len = BD_CAL_FEOBG0_LEN;
               Xfer_Num = BD_CAL_FEOBG0_LEN;
            break;

        case 0x3E: // Access item = BD SPP2BG0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_SPP2BG0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_SPP2BG0_LEN )
                Xfer_Len = BD_CAL_SPP2BG0_LEN;
            Xfer_Num = BD_CAL_SPP2BG0_LEN;
            break;

        case 0x3F: //Access item = BD FBDAC1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_FBDAC1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_FBDAC1_LEN )
                Xfer_Len = BD_CAL_FBDAC1_LEN;
            Xfer_Num = BD_CAL_FBDAC1_LEN;
            break;

        case 0x40: //Access item = BD FEBG1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_FEOBG1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_FEOBG1_LEN )
                Xfer_Len = BD_CAL_FEOBG1_LEN;
            Xfer_Num = BD_CAL_FEOBG1_LEN;
            break;

        case 0x41: // Access item = BD SPP2BG1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_SPP2BG1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_SPP2BG1_LEN )
                Xfer_Len = BD_CAL_SPP2BG1_LEN;
            Xfer_Num = BD_CAL_SPP2BG1_LEN;
            break;
        case 0x42: //Access item = BD RRFG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_RRFG_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_RRFG_LEN )
                Xfer_Len = BD_CAL_RRFG_LEN;
            Xfer_Num = BD_CAL_RRFG_LEN;
            break;

        case 0x43: //Access item = BD Beta Offset
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_BETA_OFFSET_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_BETA_OFFSET_LEN )
                Xfer_Len = BD_CAL_BETA_OFFSET_LEN;
            Xfer_Num = BD_CAL_BETA_OFFSET_LEN;
            break;

        case 0x44: //Access item = BD TILT DAC0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_TILT_DAC0_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_TILT_DAC0_LEN )
                Xfer_Len = BD_CAL_TILT_DAC0_LEN;
            Xfer_Num = BD_CAL_TILT_DAC0_LEN;
            break;

        case 0x45: //Access item = BD TILT DAC1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_TILT_DAC1_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_TILT_DAC1_LEN )
                Xfer_Len = BD_CAL_TILT_DAC1_LEN;
            Xfer_Num = BD_CAL_TILT_DAC1_LEN;
            break;

        case 0x46: //Access item = BD TILT DAC2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_TILT_DAC2_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_TILT_DAC2_LEN )
                Xfer_Len = BD_CAL_TILT_DAC2_LEN;
            Xfer_Num = BD_CAL_TILT_DAC2_LEN;
            break;

        case 0x47: //Access item = BD TILT DAC3
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_TILT_DAC3_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_TILT_DAC3_LEN )
                Xfer_Len = BD_CAL_TILT_DAC3_LEN;
            Xfer_Num = BD_CAL_TILT_DAC3_LEN;
            break;

        case 0x48: //Access item = BD TILT DAC4
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_CAL_TILT_DAC4_ADDR;         /* the offset of Starting address  */
            if ( Xfer_Len > BD_CAL_TILT_DAC4_LEN )
                Xfer_Len = BD_CAL_TILT_DAC4_LEN;
            Xfer_Num = BD_CAL_TILT_DAC4_LEN;
            break;

        case 0x49: //Access item = BD BIOS Power
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_BIAS_POWER_ADDR;    /* the offset of Starting address  */
            if ( Xfer_Len > BD_BIAS_POWER_LEN )
                Xfer_Len = BD_BIAS_POWER_LEN;
            Xfer_Num = BD_BIAS_POWER_LEN;
            break;

//        case 0x4A: //Access item = BD VWDC2VRDC Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = BD_VWDC2VRDC_ADDR;    /* the offset of Starting address  */
//            if ( Xfer_Len > BD_VWDC2VRDC_LEN  )
//                Xfer_Len = BD_VWDC2VRDC_LEN ;
//            Xfer_Num = BD_VWDC2VRDC_LEN ;
//            break;
//
//        case 0x4B: //Access item = BD VWDC2VWDC1 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = BD_VWDC2VWDC1_ADDR;    /* the offset of Starting address  */
//            if ( Xfer_Len > BD_VWDC2VWDC1_LEN  )
//                Xfer_Len = BD_VWDC2VWDC1_LEN ;
//            Xfer_Num = BD_VWDC2VWDC1_LEN ;
//            break;
//
//        case 0x4C: //Access item = BD VWDC2VWDC2 Gain
//            bAccessFlashRom = TRUE;
//            Offset_addr.Long = BD_VWDC2VWDC2_ADDR;   /* the offset of Starting address  */
//            if ( Xfer_Len > BD_VWDC2VWDC2_LEN )
//                Xfer_Len = BD_VWDC2VWDC2_LEN;
//            Xfer_Num = BD_VWDC2VWDC2_LEN;
//            break;

        case 0x50: //Access item = Offline Calibration Done Bit
            bAccessFlashRom = FALSE;
            svoDiagReturnValue1 = svoPcalVar.CalDoneBits;
            svoDiagReturnValue2 = (svoPcalVar.CalDoneBits >> 16);
            break;

        case 0xA0: //Access item = CD_OFFSET_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_OFFSET_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_OFFSET_DAC_LEN/4 )
                Xfer_Len = CD_OFFSET_DAC_LEN/4;
            Xfer_Num = CD_OFFSET_DAC_LEN/4;
            break;
        case 0xA1: //Access item = CD_OFFSET_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_OFFSET_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_OFFSET_DAC_LEN/4 )
                Xfer_Len = CD_OFFSET_DAC_LEN/4;
            Xfer_Num = CD_OFFSET_DAC_LEN/4;
            break;
        case 0xA2: //Access item = CD_OFFSET_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_OFFSET_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_OFFSET_DAC_LEN/4 )
                Xfer_Len = CD_OFFSET_DAC_LEN/4;
            Xfer_Num = CD_OFFSET_DAC_LEN/4;
            break;
        case 0xA3: //Access item = CD_OFFSET_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_OFFSET_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_OFFSET_DAC_LEN/4 )
                Xfer_Len = CD_OFFSET_DAC_LEN/4;
            Xfer_Num = CD_OFFSET_DAC_LEN/4;
            break;
        case 0xA4: //Access item = CD_SLOPE_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_DAC_LEN/4 )
                Xfer_Len = CD_SLOPE_DAC_LEN/4;
            Xfer_Num = CD_SLOPE_DAC_LEN/4;
            break;
        case 0xA5: //Access item = CD_SLOPE_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_DAC_LEN/4 )
                Xfer_Len = CD_SLOPE_DAC_LEN/4;
            Xfer_Num = CD_SLOPE_DAC_LEN/4;
            break;
        case 0xA6: //Access item = CD_SLOPE_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_DAC_LEN/4 )
                Xfer_Len = CD_SLOPE_DAC_LEN/4;
            Xfer_Num = CD_SLOPE_DAC_LEN/4;
            break;
        case 0xA7: //Access item = CD_SLOPE_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_DAC_LEN/4 )
                Xfer_Len = CD_SLOPE_DAC_LEN/4;
            Xfer_Num = CD_SLOPE_DAC_LEN/4;
            break;
        case 0xA8: //Access item = CD_SLOPE_ADC_VRPOWER_G1X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VRPOWER_G1X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xA9: //Access item = CD_SLOPE_ADC_VRPOWER_G5X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VRPOWER_G5X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xAA: //Access item = CD_SLOPE_ADC_VRPOWER_G10X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VRPOWER_G10X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xAB: //Access item = CD_SLOPE_ADC_VRPOWER_G25X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VRPOWER_G25X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xAC: //Access item = CD_SLOPE_ADC_VWPOWER_LG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VWPOWER_LG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xAD: //Access item = CD_SLOPE_ADC_VWPOWER_MG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VWPOWER_MG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xAE: //Access item = CD_SLOPE_ADC_VWPOWER_HG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = CD_SLOPE_ADC_VWPOWER_HG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > CD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = CD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = CD_SLOPE_ADC_VWPWR_LEN/4;
            break;

        case 0xB0: //Access item = DVD_OFFSET_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_OFFSET_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_OFFSET_DAC_LEN/4 )
                Xfer_Len = DV_OFFSET_DAC_LEN/4;
            Xfer_Num = DV_OFFSET_DAC_LEN/4;
            break;
        case 0xB1: //Access item = DVD_OFFSET_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_OFFSET_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_OFFSET_DAC_LEN/4 )
                Xfer_Len = DV_OFFSET_DAC_LEN/4;
            Xfer_Num = DV_OFFSET_DAC_LEN/4;
            break;
        case 0xB2: //Access item = DVD_OFFSET_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_OFFSET_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_OFFSET_DAC_LEN/4 )
                Xfer_Len = DV_OFFSET_DAC_LEN/4;
            Xfer_Num = DV_OFFSET_DAC_LEN/4;
            break;
        case 0xB3: //Access item = DVD_OFFSET_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_OFFSET_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_OFFSET_DAC_LEN/4 )
                Xfer_Len = DV_OFFSET_DAC_LEN/4;
            Xfer_Num = DV_OFFSET_DAC_LEN/4;
            break;
        case 0xB4: //Access item = DVD_SLOPE_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_DAC_LEN/4 )
                Xfer_Len = DV_SLOPE_DAC_LEN/4;
            Xfer_Num = DV_SLOPE_DAC_LEN/4;
            break;
        case 0xB5: //Access item = DVD_SLOPE_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_DAC_LEN/4 )
                Xfer_Len = DV_SLOPE_DAC_LEN/4;
            Xfer_Num = DV_SLOPE_DAC_LEN/4;
            break;
        case 0xB6: //Access item = DVD_SLOPE_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_DAC_LEN/4 )
                Xfer_Len = DV_SLOPE_DAC_LEN/4;
            Xfer_Num = DV_SLOPE_DAC_LEN/4;
            break;
        case 0xB7: //Access item = DVD_SLOPE_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_DAC_LEN/4 )
                Xfer_Len = DV_SLOPE_DAC_LEN/4;
            Xfer_Num = DV_SLOPE_DAC_LEN/4;
            break;
        case 0xB8: //Access item = DVD_SLOPE_ADC_VRPOWER_G1X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VRPOWER_G1X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xB9: //Access item = DVD_SLOPE_ADC_VRPOWER_G5X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VRPOWER_G5X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xBA: //Access item = DVD_SLOPE_ADC_VRPOWER_G10X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VRPOWER_G10X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xBB: //Access item = DVD_SLOPE_ADC_VRPOWER_G25X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VRPOWER_G25X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xBC: //Access item = DVD_SLOPE_ADC_VWPOWER_LG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VWPOWER_LG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xBD: //Access item = DVD_SLOPE_ADC_VWPOWER_MG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VWPOWER_MG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xBE: //Access item = DVD_SLOPE_ADC_VWPOWER_HG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_SLOPE_ADC_VWPOWER_HG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = DV_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = DV_SLOPE_ADC_VWPWR_LEN/4;
            break;

        case 0xC0: //Access item = HDDVD_OFFSET_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_OFFSET_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_OFFSET_DAC_LEN/4 )
                Xfer_Len = HD_OFFSET_DAC_LEN/4;
            Xfer_Num = HD_OFFSET_DAC_LEN/4;
            break;
        case 0xC1: //Access item = HDDVD_OFFSET_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_OFFSET_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_OFFSET_DAC_LEN/4 )
                Xfer_Len = HD_OFFSET_DAC_LEN/4;
            Xfer_Num = HD_OFFSET_DAC_LEN/4;
            break;
        case 0xC2: //Access item = HDDVD_OFFSET_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_OFFSET_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_OFFSET_DAC_LEN/4 )
                Xfer_Len = HD_OFFSET_DAC_LEN/4;
            Xfer_Num = HD_OFFSET_DAC_LEN/4;
            break;
        case 0xC3: //Access item = HDDVD_OFFSET_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_OFFSET_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_OFFSET_DAC_LEN/4 )
                Xfer_Len = HD_OFFSET_DAC_LEN/4;
            Xfer_Num = HD_OFFSET_DAC_LEN/4;
            break;
        case 0xC4: //Access item = HDDVD_SLOPE_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_DAC_LEN/4 )
                Xfer_Len = HD_SLOPE_DAC_LEN/4;
            Xfer_Num = HD_SLOPE_DAC_LEN/4;
            break;
        case 0xC5: //Access item = HDDVD_SLOPE_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_DAC_LEN/4 )
                Xfer_Len = HD_SLOPE_DAC_LEN/4;
            Xfer_Num = HD_SLOPE_DAC_LEN/4;
            break;
        case 0xC6: //Access item = HDDVD_SLOPE_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_DAC_LEN/4 )
                Xfer_Len = HD_SLOPE_DAC_LEN/4;
            Xfer_Num = HD_SLOPE_DAC_LEN/4;
            break;
        case 0xC7: //Access item = HDDVD_SLOPE_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_DAC_LEN/4 )
                Xfer_Len = HD_SLOPE_DAC_LEN/4;
            Xfer_Num = HD_SLOPE_DAC_LEN/4;
            break;
        case 0xC8: //Access item = HDDVD_SLOPE_ADC_VRPOWER_G1X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VRPOWER_G1X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xC9: //Access item = HDDVD_SLOPE_ADC_VRPOWER_G5X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VRPOWER_G5X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xCA: //Access item = HDDVD_SLOPE_ADC_VRPOWER_G10X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VRPOWER_G10X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xCB: //Access item = HDDVD_SLOPE_ADC_VRPOWER_G25X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VRPOWER_G25X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xCC: //Access item = HDDVD_SLOPE_ADC_VWPOWER_LG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VWPOWER_LG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xCD: //Access item = HDDVD_SLOPE_ADC_VWPOWER_MG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VWPOWER_MG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xCE: //Access item = HDDVD_SLOPE_ADC_VWPOWER_HG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_SLOPE_ADC_VWPOWER_HG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = HD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = HD_SLOPE_ADC_VWPWR_LEN/4;
            break;

        case 0xD0: //Access item = BD_OFFSET_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_OFFSET_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_OFFSET_DAC_LEN/4 )
                Xfer_Len = BD_OFFSET_DAC_LEN/4;
            Xfer_Num = BD_OFFSET_DAC_LEN/4;
            break;
        case 0xD1: //Access item = BD_OFFSET_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_OFFSET_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_OFFSET_DAC_LEN/4 )
                Xfer_Len = BD_OFFSET_DAC_LEN/4;
            Xfer_Num = BD_OFFSET_DAC_LEN/4;
            break;
        case 0xD2: //Access item = BD_OFFSET_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_OFFSET_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_OFFSET_DAC_LEN/4 )
                Xfer_Len = BD_OFFSET_DAC_LEN/4;
            Xfer_Num = BD_OFFSET_DAC_LEN/4;
            break;
        case 0xD3: //Access item = BD_OFFSET_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_OFFSET_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_OFFSET_DAC_LEN/4 )
                Xfer_Len = BD_OFFSET_DAC_LEN/4;
            Xfer_Num = BD_OFFSET_DAC_LEN/4;
            break;
        case 0xD4: //Access item = BD_SLOPE_DAC_THRESHOLD
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_DAC_THRESHOLD_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_DAC_LEN/4 )
                Xfer_Len = BD_SLOPE_DAC_LEN/4;
            Xfer_Num = BD_SLOPE_DAC_LEN/4;
            break;
        case 0xD5: //Access item = BD_SLOPE_DAC_0
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_DAC_0_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_DAC_LEN/4 )
                Xfer_Len = BD_SLOPE_DAC_LEN/4;
            Xfer_Num = BD_SLOPE_DAC_LEN/4;
            break;
        case 0xD6: //Access item = BD_SLOPE_DAC_1
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_DAC_1_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_DAC_LEN/4 )
                Xfer_Len = BD_SLOPE_DAC_LEN/4;
            Xfer_Num = BD_SLOPE_DAC_LEN/4;
            break;
        case 0xD7: //Access item = BD_SLOPE_DAC_2
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_DAC_2_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_DAC_LEN/4 )
                Xfer_Len = BD_SLOPE_DAC_LEN/4;
            Xfer_Num = BD_SLOPE_DAC_LEN/4;
            break;
        case 0xD8: //Access item = BD_SLOPE_ADC_VRPOWER_G1X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VRPOWER_G1X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xD9: //Access item = BD_SLOPE_ADC_VRPOWER_G5X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VRPOWER_G5X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xDA: //Access item = BD_SLOPE_ADC_VRPOWER_G10X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VRPOWER_G10X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xDB: //Access item = BD_SLOPE_ADC_VRPOWER_G25X
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VRPOWER_G25X_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VRPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VRPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VRPWR_LEN/4;
            break;
        case 0xDC: //Access item = BD_SLOPE_ADC_VWPOWER_LG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VWPOWER_LG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xDD: //Access item = BD_SLOPE_ADC_VWPOWER_MG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VWPOWER_MG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VWPWR_LEN/4;
            break;
        case 0xDE: //Access item = BD_SLOPE_ADC_VWPOWER_HG
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_SLOPE_ADC_VWPOWER_HG_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_SLOPE_ADC_VWPWR_LEN/4 )
                Xfer_Len = BD_SLOPE_ADC_VWPWR_LEN/4;
            Xfer_Num = BD_SLOPE_ADC_VWPWR_LEN/4;
            break;

        case 0xE0: //Access item = DV_BIASPOWER2DIFFHOLDDAC_OFFSET_ADDR
            bAccessFlashRom = TRUE;
            Offset_addr.Long = DV_BP2DIFFHOLDDAC_OFFSET_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > DV_BP2DIFFHOLDDAC_LEN/2 )
                Xfer_Len = DV_BP2DIFFHOLDDAC_LEN/2;
            Xfer_Num = DV_BP2DIFFHOLDDAC_LEN/2;
            break;
        case 0xE1: //Access item = HDV_BIASPOWER2DIFFHOLDDAC_OFFSET_ADDR
            bAccessFlashRom = TRUE;
            Offset_addr.Long = HD_BP2DIFFHOLDDAC_OFFSET_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > HD_BP2DIFFHOLDDAC_LEN/2 )
                Xfer_Len = HD_BP2DIFFHOLDDAC_LEN/2;
            Xfer_Num = HD_BP2DIFFHOLDDAC_LEN/2;
            break;
        case 0xE2: //Access item = BD_BIASPOWER2DIFFHOLDDAC_OFFSET_ADDR
            bAccessFlashRom = TRUE;
            Offset_addr.Long = BD_BP2DIFFHOLDDAC_OFFSET_ADDR;   /* the offset of Starting address  */
            if ( Xfer_Len > BD_BP2DIFFHOLDDAC_LEN/2 )
                Xfer_Len = BD_BP2DIFFHOLDDAC_LEN/2;
            Xfer_Num = BD_BP2DIFFHOLDDAC_LEN/2;
            break;

        default:
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338020);//Unknow_Calibration Mode!!
            return(0);
    } //switch (Iram_Cmd_block.bf[2]&0xC0)

    switch (Iram_Cmd_block.bf[3]) //Operation Mode,ATAPI command byte3.7~0
    {
        case 0:     /* Read Information form Persistent table */
            cmode = 0;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338010);
            break;
        case 1:     /* Write Information to Persistent table */
            cmode = 1;
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338011);
            break;

        default:
            SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338012);
            return(0);
    }//swicth (cmode)

    if (bAccessFlashRom==TRUE)
    {
        SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,4,0x338014,A4B(Offset_addr.Long));
        if (cmode==1)/* Write device */
        {
            start_addr = FLASH_BASE + PARA_BLK_START_OFFSET + Offset_addr.Long;    /*EEPROM start address*/
            SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,4,0x338013,A4B(start_addr));
            if (start_addr < (FLASH_BASE + PARA_BLK_START_OFFSET) || ((start_addr + Xfer_Len) > (FLASH_BASE + PARA_BLK_START_OFFSET + PARA_BLK_SIZE)) ||(0xFFL >= Iram_Cmd_block.bf[2] >= 0xA0L))
            {
                BUILD_SENSE(0x05,0x20,0x00,0x4d); /* INVALID COMMAND OPERATION CODE */
                AutoCommandComplete();
                return(0);
            }
            OpenWritableSegmentTable(ePersistent_Talbe);
            Iram_Linear_Xfer_addr = SCSDRAM_ADDRESS + DRVPARA_DATA + Offset_addr.Long;
            GetParameterDataFromHost(Iram_Linear_Xfer_addr, Xfer_Len);
            CloseWritableSegmentTable(ePersistent_Talbe);
            AutoCommandComplete();
            return(0);
        }
        else if (cmode==0)/* Read device */
        {
            start_addr = FLASH_BASE + PARA_BLK_START_OFFSET + Offset_addr.Long;    /*EEPROM start address*/
            SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,4,0x338013,A4B(start_addr));
            AdrP = (BYTE *) (start_addr );

            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            for (start_addr = 0; start_addr < Xfer_Num ; start_addr++)
            {
                WriteSDRAM(*AdrP++);
            }
            return(Xfer_Num);
        }
    }
    else
    {
        Iram_Buf_dram_addr = BUFFER_SCRATCH;
        WriteSDRAM(LBYTE(svoDiagReturnValue1));Xfer_Num++;
        WriteSDRAM(HBYTE(svoDiagReturnValue1));Xfer_Num++;
        WriteSDRAM(LBYTE(svoDiagReturnValue2));Xfer_Num++;
        WriteSDRAM(HBYTE(svoDiagReturnValue2));Xfer_Num++;
        return(Xfer_Num);
    }

    return(0);
}


/************************************************************************
* Module: z_cmd0xF801XX01()
* Description:  ATAPI Command 0xF8 0x01 0x00,Save Servo Calibration Data
* Returns:
* Params:
*************************************************************************/
// ATAPI Command 0xF8 0x01 0xXX 0x01, Save Servo Calibration Data//
USHORT z_cmd0xF801XX01(USHORT Xfer_Num,int num,int ctimes)
{
    #if (EN_Persistent_table_8 == 1)
    USHORT  temp_adr;
    #else
    USHORT  temp_adr,temp_id_adr;
    #endif/**/

    z_calLoad_EepromMap();
    if (svoPcalVar.fgCalDVDMode == FALSE) //CD Disc
    {
        SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x335006); // LD_Mode==>CD
        switch (num)
        {
            case  FEBG_ADJ:     /* Save Focus Balance K2 Data */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                #if (EN_Persistent_table_8 == 1)
                svoPcalVar.cdFeoBGValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* CD */
                SetPersistentTable2B(CD_CAL_FEOBG_ADDR,svoPcalVar.cdFeoBGValue);
                SetPersistentTable2B(CD_CAL_FEOBG_ADDR+2,~svoPcalVar.cdFeoBGValue);
                #else
                svoPcalVar.cdFeBGValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* CD */
                SetPersistentTable2B(CD_CAL_FEBG_ADDR,svoPcalVar.cdFeBGValue);
                SetPersistentTable2B(CD_CAL_FEBG_ADDR+2,~svoPcalVar.cdFeBGValue);
                #endif/* (EN_Persistent_table_8 == 1) */
                SetPersistentTable1B(CD_CAL_FEBG_ID_ADDR +0, 'F');
                SetPersistentTable1B(CD_CAL_FEBG_ID_ADDR +1, 'E');
                SetPersistentTable1B(CD_CAL_FEBG_ID_ADDR +2, 'B');
                SetPersistentTable1B(CD_CAL_FEBG_ID_ADDR +3, 'G');
                break;
            case FEBC_RRF_ADJ:     /* Save Focus Balance DAC Data */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                svoPcalVar.cdFBDacValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* CD */
                SetPersistentTable2B(CD_CAL_FBDAC_ADDR,svoPcalVar.cdFBDacValue);
                SetPersistentTable2B(CD_CAL_FBDAC_ADDR+2,~svoPcalVar.cdFBDacValue);
                #if (EN_Persistent_table_8 == 1)
                #else
                SetPersistentTable1B(CD_CAL_FBDAC_ID_ADDR+0, 'C');
                SetPersistentTable1B(CD_CAL_FBDAC_ID_ADDR+1, 'D');
                SetPersistentTable1B(CD_CAL_FBDAC_ID_ADDR+2, 'F');
                SetPersistentTable1B(CD_CAL_FBDAC_ID_ADDR+3, 'B');
                #endif/* ( EN_Persistent_table_8 == 1) */
                break;
            case RRFG_ADJ:     /* Save RRF Gain Data */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                svoPcalVar.cdRRFGValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* CD */
                SetPersistentTable2B(CD_CAL_RRFG_ADDR,svoPcalVar.cdRRFGValue);
                SetPersistentTable2B(CD_CAL_RRFG_ADDR+2,~svoPcalVar.cdRRFGValue);
                SetPersistentTable1B(CD_CAL_RRFG_ID_ADDR +0, 'C');
                SetPersistentTable1B(CD_CAL_RRFG_ID_ADDR +1, 'D');
                SetPersistentTable1B(CD_CAL_RRFG_ID_ADDR +2, 'R');
                SetPersistentTable1B(CD_CAL_RRFG_ID_ADDR +3, 'G');
                break;
            case BETA_ADJ:     /* Save Beta Offset Data */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                svoPcalVar.cdBetaOffset = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]);
                SetPersistentTable2B(CD_CAL_BETA_OFFSET_ADDR,svoPcalVar.cdBetaOffset);
                SetPersistentTable2B(CD_CAL_BETA_OFFSET_ADDR+2,~svoPcalVar.cdBetaOffset);
                SetPersistentTable1B(CD_CAL_BETA_OFFSET_ID_ADDR+0, 'B');
                SetPersistentTable1B(CD_CAL_BETA_OFFSET_ID_ADDR+1, 'E');
                SetPersistentTable1B(CD_CAL_BETA_OFFSET_ID_ADDR+2, 'T');
                SetPersistentTable1B(CD_CAL_BETA_OFFSET_ID_ADDR+3, 'A');
                break;
         #if(EN_Persistent_table_8 == 1)
            case SPP2BG_ADJ:
                svoPcalVar.cdSPP2BGValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* CD */
                SetPersistentTable2B(CD_CAL_SPP2BG_ADDR,svoPcalVar.cdSPP2BGValue);
                SetPersistentTable2B(CD_CAL_SPP2BG_ADDR+2,~svoPcalVar.cdSPP2BGValue);
                break;
         #endif/* (EN_Persistent_table_8 == 1) */
            default:
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,0xFF); //Save_Servo_Calibration_num
                return(0);
        }//swicth (num)
    }
    else //DVD Disc
    {
        SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x335007); //LD_Mode==>DVD
        switch (num)
        {
            case  FEBG_ADJ:     /* Save Focus Balance K2 Data */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                #if (EN_Persistent_table_8 == 1)
                svoPcalVar.dvFeoBG0Value = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                SetPersistentTable2B(DV_CAL_FEOBG0_ADDR,svoPcalVar.dvFeoBG0Value);
                SetPersistentTable2B(DV_CAL_FEOBG0_ADDR+2,~svoPcalVar.dvFeoBG0Value);
                #else
                svoPcalVar.dvFeBG0Value = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                SetPersistentTable2B(DV_CAL_FEBG0_ADDR,svoPcalVar.dvFeBG0Value);
                SetPersistentTable2B(DV_CAL_FEBG0_ADDR+2,~svoPcalVar.dvFeBG0Value);
                #endif/* (EN_Persistent_table_8 == 1) */
                SetPersistentTable1B(DV_CAL_FEBG0_ID_ADDR +0, 'F');
                SetPersistentTable1B(DV_CAL_FEBG0_ID_ADDR +1, 'E');
                SetPersistentTable1B(DV_CAL_FEBG0_ID_ADDR +2, 'B');
                SetPersistentTable1B(DV_CAL_FEBG0_ID_ADDR +3, 'G');
                break;

            case FEBC_RRF_ADJ:     /* Save Focus Balance DAC Data */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                svoPcalVar.dvFBDac0Value = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                SetPersistentTable2B(DV_CAL_FBDAC0_ADDR,svoPcalVar.dvFBDac0Value);
                SetPersistentTable2B(DV_CAL_FBDAC0_ADDR+2,~svoPcalVar.dvFBDac0Value);
                #if (EN_Persistent_table_8 == 1)
                #else
                SetPersistentTable1B(DV_CAL_FBDAC0_ID_ADDR+0, 'D');
                SetPersistentTable1B(DV_CAL_FBDAC0_ID_ADDR+1, 'V');
                SetPersistentTable1B(DV_CAL_FBDAC0_ID_ADDR+2, 'F');
                SetPersistentTable1B(DV_CAL_FBDAC0_ID_ADDR+3, 'B');
                #endif/* (EN_Persistent_table_8 == 1) */
                break;

            case RRFG_ADJ:     /* Save RRF Gain Data */
                if ((svoVar_discKindT.fDiscKind==eDVDPlusRDisc))
                {
                    svoPcalVar.dvRRFGValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                    SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x33FF4B,A2B(num),A2B(ctimes));
                    SetPersistentTable2B(DV_CAL_PLUS_RRFG_ADDR,svoPcalVar.dvRRFGValue);
                    SetPersistentTable2B(DV_CAL_PLUS_RRFG_ADDR+2,~svoPcalVar.dvRRFGValue);
                    SetPersistentTable1B(DV_CAL_PLUS_RRFG_ID_ADDR +0, 'P');
                    SetPersistentTable1B(DV_CAL_PLUS_RRFG_ID_ADDR +1, 'D');
                    SetPersistentTable1B(DV_CAL_PLUS_RRFG_ID_ADDR +2, 'V');
                    SetPersistentTable1B(DV_CAL_PLUS_RRFG_ID_ADDR +3, 'G');
                }
                else if ((svoVar_discKindT.fDiscKind==eDVDMinusRDisc))
                {
                    svoPcalVar.dvRRFGValue = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                    SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x33FF4B,A2B(num),A2B(ctimes));
                    SetPersistentTable2B(DV_CAL_MINUS_RRFG_ADDR,svoPcalVar.dvRRFGValue);
                    SetPersistentTable2B(DV_CAL_MINUS_RRFG_ADDR+2,~svoPcalVar.dvRRFGValue);
                    SetPersistentTable1B(DV_CAL_MINUS_RRFG_ID_ADDR +0, 'M');
                    SetPersistentTable1B(DV_CAL_MINUS_RRFG_ID_ADDR +1, 'D');
                    SetPersistentTable1B(DV_CAL_MINUS_RRFG_ID_ADDR +2, 'V');
                    SetPersistentTable1B(DV_CAL_MINUS_RRFG_ID_ADDR +3, 'G');
                }
                break;

            case BETA_ADJ:     /* Save Beta Offset Data */
                if ((pdbServoGetfDiscKind()==eDVDPlusRDisc))
                {
                    svoPcalVar.dvBetaOffset = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD +R */
                    SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x33FF4B,A2B(num),A2B(ctimes));
                    SetPersistentTable2B(DV_CAL_PLUS_BETA_OFFSET_ADDR,svoPcalVar.dvBetaOffset);
                    SetPersistentTable2B(DV_CAL_PLUS_BETA_OFFSET_ADDR+2,~svoPcalVar.dvBetaOffset);
                    SetPersistentTable1B(DV_CAL_PLUS_BETA_OFFSET_ID_ADDR+0, 'B');
                    SetPersistentTable1B(DV_CAL_PLUS_BETA_OFFSET_ID_ADDR+1, 'E');
                    SetPersistentTable1B(DV_CAL_PLUS_BETA_OFFSET_ID_ADDR+2, 'T');
                    SetPersistentTable1B(DV_CAL_PLUS_BETA_OFFSET_ID_ADDR+3, 'A');
                }
                else if (pdbServoGetfDiscKind()==eDVDMinusRDisc)
                {
                    svoPcalVar.dvBetaOffset = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD -R */
                    SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,4,0x33FF4B,A2B(num),A2B(ctimes));
                    SetPersistentTable2B(DV_CAL_MINUS_BETA_OFFSET_ADDR,svoPcalVar.dvBetaOffset);
                    SetPersistentTable2B(DV_CAL_MINUS_BETA_OFFSET_ADDR+2,~svoPcalVar.dvBetaOffset);
                    SetPersistentTable1B(DV_CAL_MINUS_BETA_OFFSET_ID_ADDR+0, 'B');
                    SetPersistentTable1B(DV_CAL_MINUS_BETA_OFFSET_ID_ADDR+1, 'E');
                    SetPersistentTable1B(DV_CAL_MINUS_BETA_OFFSET_ID_ADDR+2, 'T');
                    SetPersistentTable1B(DV_CAL_MINUS_BETA_OFFSET_ID_ADDR+3, 'A');
                }
                break;

            case TILT_TE_ADJ:     /* Save Tilt DAC Data */
                if (ctimes<1) ctimes=1;
                svoPcalVar.dvTiltDacValue[ctimes-1] = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,num);
                SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x338024,F4B((float)ctimes));
                switch (ctimes)
                {
                    case 1:
                        temp_adr = DV_CAL_TILT_DAC0_ADDR;
                        break;
                    case 2:
                        temp_adr = DV_CAL_TILT_DAC1_ADDR;
                        break;
                    case 3:
                        temp_adr = DV_CAL_TILT_DAC2_ADDR;
                        break;
                    case 4:
                        temp_adr = DV_CAL_TILT_DAC3_ADDR;
                        break;
                    case 5:
                    default:
                        temp_adr = DV_CAL_TILT_DAC4_ADDR;
                        break;
                }//swicth (ctimes)
                #if (EN_Persistent_table_8 == 1)
                #else
                temp_id_adr = DV_CAL_TILT_ID_ADDR;
                #endif/*(EN_Persistent_table_8 == 1)*/
                SetPersistentTable2B(temp_adr,svoPcalVar.dvTiltDacValue[ctimes-1]);
                SetPersistentTable2B(temp_adr+2,~svoPcalVar.dvTiltDacValue[ctimes-1]);
                SetPersistentTable2B(DV_CAL_TILT_NUM_ADDR,(USHORT)ctimes);
                SetPersistentTable2B(DV_CAL_TILT_NUM_ADDR+2,~((USHORT)ctimes));
                #if (EN_Persistent_table_8 == 1)
                #else
                SetPersistentTable1B(temp_id_adr+0, 'T');
                SetPersistentTable1B(temp_id_adr+1, 'I');
                SetPersistentTable1B(temp_id_adr+2, 'L');
                SetPersistentTable1B(temp_id_adr+3, 'T');
                SetPersistentTable1B(temp_id_adr+4, 'K');
                SetPersistentTable1B(temp_id_adr+5, 'D');
                SetPersistentTable1B(temp_id_adr+6, 'A');
                SetPersistentTable1B(temp_id_adr+7, 'C');
                #endif/*(EN_Persistent_table_8 == 1)*/
                break;
        #if(EN_Persistent_table_8 == 1)
            case SPP2BG_ADJ:
                svoPcalVar.dvSPP2BG0Value = (Iram_Cmd_block.bf[5]<<8)|(Iram_Cmd_block.bf[4]); /* DVD */
                SetPersistentTable2B(DV_CAL_SPP2BG0_ADDR,svoPcalVar.dvSPP2BG0Value);
                SetPersistentTable2B(DV_CAL_SPP2BG0_ADDR+2,~svoPcalVar.dvSPP2BG0Value);
                break;
        #endif/* (EN_Persistent_table_8 == 1) */
            default:
                SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,1,0x338023,0xFF); //Save_Servo_Calibration_num
                return(0);
        }//swicth (num)
    }
    z_calFlash_EepromMap();
    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    WriteSDRAM(0x00);Xfer_Num++;
    return(Xfer_Num);
}

/************************************************************************
* Module: z_calLoad_EepromMap()
* Description:
* Returns:
* Params:
*************************************************************************/
void z_calLoad_EepromMap(void)
{
    OpenWritableSegmentTable(ePersistent_Talbe);
    svoPcalVar.fgLoadCalEeprom = TRUE;
}


/************************************************************************
* Module: uSendFloNum()
* Description:
* Returns:
* Params:
*************************************************************************/
USHORT uSendFloNum(USHORT Xfer_Num,float fnum)
{
   BYTE *bp;

   bp = (BYTE *)(&fnum);
   WriteSDRAM(bp[0]);Xfer_Num++;
   WriteSDRAM(bp[1]);Xfer_Num++;
   WriteSDRAM(bp[2]);Xfer_Num++;
   WriteSDRAM(bp[3]);Xfer_Num++;
   return(Xfer_Num);
}


/************************************************************************
* Module: z_calWrite_PowerCalibrationParam()
* Description:
* Returns:
* Params:
*************************************************************************/
extern BYTE const Inquiry_tbl[];
void z_calWrite_PowerCalibrationParam(void)
{
    BYTE Index;

    switch (svoPcalVar.LDMode)
    {
        case CD_LD:
            /* CD */
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
            {
                SetPersistentTable4F(CD_OFFSET_DAC_THRESHOLD_ADDR,svoPcalVar.cdp2rOff);
                SetPersistentTable4F(CD_SLOPE_DAC_THRESHOLD_ADDR,svoPcalVar.cdp2rSlp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==2)) //DAC_Threshhold (oscen on)
            {
                SetPersistentTable4F(CD_OFFSET_DAC_THRESHOLD_ADDR_ON,svoPcalVar.cdp2rOff);
                SetPersistentTable4F(CD_SLOPE_DAC_THRESHOLD_ADDR_ON,svoPcalVar.cdp2rSlp);
            }

            if ((apc_rwopen_mode==3)&&(apc_rw_gain==3)) //Delta_0 (oscen on)
            {
                SetPersistentTable4F(CD_OFFSET_DAC_0_ADDR,svoPcalVar.cdp2w1Off);
                SetPersistentTable4F(CD_SLOPE_DAC_0_ADDR,svoPcalVar.cdp2w1Slp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==4)) //Delta_1 (oscen on)
            {
                SetPersistentTable4F(CD_OFFSET_DAC_1_ADDR,svoPcalVar.cdp2w1Off);
                SetPersistentTable4F(CD_SLOPE_DAC_1_ADDR,svoPcalVar.cdp2w1Slp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==5)) //Delta_2 (oscen on)
            {
                SetPersistentTable4F(CD_OFFSET_DAC_2_ADDR,svoPcalVar.cdp2w1Off);
                SetPersistentTable4F(CD_SLOPE_DAC_2_ADDR,svoPcalVar.cdp2w1Slp);
            }

            if ((apc_rwopen_mode==2)&&(apc_rw_gain==1))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VWPOWER_LG_ADDR,svoPcalVar.cdp2w1Off);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_LG_ADDR,svoPcalVar.cdp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==2))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VWPOWER_MG_ADDR,svoPcalVar.cdp2w1Off);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_MG_ADDR,svoPcalVar.cdp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==3))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VWPOWER_HG_ADDR,svoPcalVar.cdp2w1Off);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VWPOWER_HG_ADDR,svoPcalVar.cdp2w1Slp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VRPOWER_G1X_ADDR,svoPcalVar.cdp2rOff);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G1X_ADDR,svoPcalVar.cdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VRPOWER_G5X_ADDR,svoPcalVar.cdp2rOff);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G5X_ADDR,svoPcalVar.cdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VRPOWER_G10X_ADDR,svoPcalVar.cdp2rOff);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G10X_ADDR,svoPcalVar.cdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(CD_OFFSET_ADC_VRPOWER_G25X_ADDR,svoPcalVar.cdp2rOff);
#endif
                SetPersistentTable4F(CD_SLOPE_ADC_VRPOWER_G25X_ADDR,svoPcalVar.cdp2rSlp);
            }

            break;

        case DVD_LD:
            /* DVD */
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
            {
                SetPersistentTable4F(DV_OFFSET_DAC_THRESHOLD_ADDR,svoPcalVar.dvp2rOff);
                SetPersistentTable4F(DV_SLOPE_DAC_THRESHOLD_ADDR,svoPcalVar.dvp2rSlp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==2)) //DAC_Threshhold (oscen on)
            {
                SetPersistentTable4F(DV_OFFSET_DAC_THRESHOLD_ADDR_ON,svoPcalVar.dvp2rOff);
                SetPersistentTable4F(DV_SLOPE_DAC_THRESHOLD_ADDR_ON,svoPcalVar.dvp2rSlp);
            }

            if ((apc_rwopen_mode==3)&&(apc_rw_gain==3)) //Delta_0 (oscen on)
            {
                SetPersistentTable4F(DV_OFFSET_DAC_0_ADDR,svoPcalVar.dvp2w1Off);
                SetPersistentTable4F(DV_SLOPE_DAC_0_ADDR,svoPcalVar.dvp2w1Slp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==4)) //Delta_1 (oscen on)
            {
                SetPersistentTable4F(DV_OFFSET_DAC_1_ADDR,svoPcalVar.dvp2w1Off);
                SetPersistentTable4F(DV_SLOPE_DAC_1_ADDR,svoPcalVar.dvp2w1Slp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==5)) //Delta_2 (oscen on)
            {
                SetPersistentTable4F(DV_OFFSET_DAC_2_ADDR,svoPcalVar.dvp2w1Off);
                SetPersistentTable4F(DV_SLOPE_DAC_2_ADDR,svoPcalVar.dvp2w1Slp);
            }

            if ((apc_rwopen_mode==2)&&(apc_rw_gain==1))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VWPOWER_LG_ADDR,svoPcalVar.dvp2w1Off);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VWPOWER_LG_ADDR,svoPcalVar.dvp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==2))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VWPOWER_MG_ADDR,svoPcalVar.dvp2w1Off);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VWPOWER_MG_ADDR,svoPcalVar.dvp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==3))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VWPOWER_HG_ADDR,svoPcalVar.dvp2w1Off);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VWPOWER_HG_ADDR,svoPcalVar.dvp2w1Slp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VRPOWER_G1X_ADDR,svoPcalVar.dvp2rOff);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G1X_ADDR,svoPcalVar.dvp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VRPOWER_G5X_ADDR,svoPcalVar.dvp2rOff);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G5X_ADDR,svoPcalVar.dvp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VRPOWER_G10X_ADDR,svoPcalVar.dvp2rOff);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G10X_ADDR,svoPcalVar.dvp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(DV_OFFSET_ADC_VRPOWER_G25X_ADDR,svoPcalVar.dvp2rOff);
#endif
                SetPersistentTable4F(DV_SLOPE_ADC_VRPOWER_G25X_ADDR,svoPcalVar.dvp2rSlp);
            }

            break;

        case BD_LD:
            /* BD */
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
            {
                SetPersistentTable4F(BD_OFFSET_DAC_THRESHOLD_ADDR,svoPcalVar.bdp2rOff);
                SetPersistentTable4F(BD_SLOPE_DAC_THRESHOLD_ADDR,svoPcalVar.bdp2rSlp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==2)) //DAC_Threshhold (oscen on)
            {
                SetPersistentTable4F(BD_OFFSET_DAC_THRESHOLD_ADDR_ON,svoPcalVar.bdp2rOff);
                SetPersistentTable4F(BD_SLOPE_DAC_THRESHOLD_ADDR_ON,svoPcalVar.bdp2rSlp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==3)) //Delta_0 (oscen on)
            {
                SetPersistentTable4F(BD_OFFSET_DAC_0_ADDR,svoPcalVar.bdp2w1Off);
                SetPersistentTable4F(BD_SLOPE_DAC_0_ADDR,svoPcalVar.bdp2w1Slp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==4)) //Delta_1 (oscen on)
            {
                SetPersistentTable4F(BD_OFFSET_DAC_1_ADDR,svoPcalVar.bdp2w1Off);
                SetPersistentTable4F(BD_SLOPE_DAC_1_ADDR,svoPcalVar.bdp2w1Slp);
            }
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==5)) //Delta_2 (oscen on)
            {
                SetPersistentTable4F(BD_OFFSET_DAC_2_ADDR,svoPcalVar.bdp2w1Off);
                SetPersistentTable4F(BD_SLOPE_DAC_2_ADDR,svoPcalVar.bdp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==1))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VWPOWER_LG_ADDR,svoPcalVar.bdp2w1Off);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VWPOWER_LG_ADDR,svoPcalVar.bdp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==2))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VWPOWER_MG_ADDR,svoPcalVar.bdp2w1Off);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VWPOWER_MG_ADDR,svoPcalVar.bdp2w1Slp);
            }
            if ((apc_rwopen_mode==2)&&(apc_rw_gain==3))
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VWPOWER_HG_ADDR,svoPcalVar.bdp2w1Off);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VWPOWER_HG_ADDR,svoPcalVar.bdp2w1Slp);
            }

            if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VRPOWER_G1X_ADDR,svoPcalVar.bdp2rOff);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G1X_ADDR,svoPcalVar.bdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VRPOWER_G5X_ADDR,svoPcalVar.bdp2rOff);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G5X_ADDR,svoPcalVar.bdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VRPOWER_G10X_ADDR,svoPcalVar.bdp2rOff);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G10X_ADDR,svoPcalVar.bdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(BD_OFFSET_ADC_VRPOWER_G25X_ADDR,svoPcalVar.bdp2rOff);
#endif
                SetPersistentTable4F(BD_SLOPE_ADC_VRPOWER_G25X_ADDR,svoPcalVar.bdp2rSlp);
            }
            break;

        case HD_LD:
            /* HD */
            if ((apc_rwopen_mode==3)&&(apc_rw_gain==1)) //DAC_Threshhold
            {
                SetPersistentTable4F(HD_OFFSET_DAC_THRESHOLD_ADDR,svoPcalVar.hdp2rOff);
                SetPersistentTable4F(HD_SLOPE_DAC_THRESHOLD_ADDR,svoPcalVar.hdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==1)) //VRPOW GAIN 1X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(HD_OFFSET_ADC_VRPOWER_G1X_ADDR,svoPcalVar.hdp2rOff);
#endif
                SetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G1X_ADDR,svoPcalVar.hdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==2)) //VRPOW GAIN 5X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(HD_OFFSET_ADC_VRPOWER_G5X_ADDR,svoPcalVar.hdp2rOff);
#endif
                SetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G5X_ADDR,svoPcalVar.hdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==3)) //VRPOW GAIN 10X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(HD_OFFSET_ADC_VRPOWER_G10X_ADDR,svoPcalVar.hdp2rOff);
#endif
                SetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G10X_ADDR,svoPcalVar.hdp2rSlp);
            }
            if ((apc_rwopen_mode==1)&&(apc_rw_gain==4)) //VRPOW GAIN 25X
            {
#if ((PCB == 673) || (PCB == 830))
                SetPersistentTable4F(HD_OFFSET_ADC_VRPOWER_G25X_ADDR,svoPcalVar.hdp2rOff);
#endif
                SetPersistentTable4F(HD_SLOPE_ADC_VRPOWER_G25X_ADDR,svoPcalVar.hdp2rSlp);
            }
            break;

        default:
            break;

    }


    SetPersistentTable1B(PersistentTable_ID_ADDR,PersistentTable_Revision);

    for (Index = 0; Index < 8; Index++)
        SetPersistentTable1B(VenderID_ADDR+Index,Inquiry_tbl[Index+8]);     /* FIRMW_INQ_VENDOR,008 ~ 015 */

    for (Index = 0; Index < 16; Index++)
        SetPersistentTable1B(ProductID_ADDR+Index,Inquiry_tbl[Index+16]);   /* FIRMW_INQ_PRODUCT,016 ~ 031 */

    for (Index = 0; Index < 4; Index++)
        SetPersistentTable1B(FirmWareRev_ADDR+Index,Inquiry_tbl[Index+32]); /* FIRMW_INQ_REVISION,032 ~ 035 */

    for (Index = 0; Index < 16; Index++)
        SetPersistentTable1B(SerialNumber_ADDR+Index,Inquiry_tbl[Index+36]); /* FIRMW_INQ_REVISION,036 ~ 055 */

}


/************************************************************************
* Module: z_calWrite_SvoWordParam()
* Description:
* Returns:
* Params:
*************************************************************************/
void z_calWrite_SvoWordParam(BYTE addr,BYTE *bp)
{
    USHORT utmp1,utmp2;

    SetPersistentTable1B(addr+0, bp[0]);
    SetPersistentTable1B(addr+1, bp[1]);
    LBYTE(utmp1) = GetPersistentTable1B(addr+0);
    HBYTE(utmp1) = GetPersistentTable1B(addr+1);
    utmp2 = ~utmp1;
    SetPersistentTable1B(addr+2, LBYTE(utmp2));
    SetPersistentTable1B(addr+3, HBYTE(utmp2));
}

/************************************************************************
* Module: z_calWrite_PcalParam()
* Description:
* Returns:
* Params:
*************************************************************************/
void z_calWrite_PcalParam(USHORT*addr,BYTE len,BYTE *bp)
{
    BYTE i;

    for(i=0;i<len;i++)
    {
        SetPersistentTable1B(*addr, bp[i]);
        (*addr)++;
    }
}


/************************************************************************
* Module: z_calRead_fgSvoWordParam()
* Description:
* Returns:
* Params:
*************************************************************************/
BOOL z_calRead_fgSvoWordParam(BYTE addr,BYTE *bp)
{
    USHORT  utmp1,utmp2;

    bp[0] = GetPersistentTable1B(addr+0);
    bp[1] = GetPersistentTable1B(addr+1);
    LBYTE(utmp1) = GetPersistentTable1B(addr+0);
    HBYTE(utmp1) = GetPersistentTable1B(addr+1);
    LBYTE(utmp2) = GetPersistentTable1B(addr+2);
    HBYTE(utmp2) = GetPersistentTable1B(addr+3);
    utmp2 = ~utmp2;
    if (utmp1==utmp2)
        return(TRUE);
    else
        return(FALSE);
}


/************************************************************************
* Module: z_calRead_PcalUintParam()
* Description:
* Returns:
* Params:
*************************************************************************/
USHORT z_calRead_PcalUintParam(BYTE *addr)
{
    USHORT  num;
    BYTE    *bp;

    bp = (BYTE *)&num;
    bp[0] = GetPersistentTable1B(*addr);
    (*addr)++;
    bp[1] = GetPersistentTable1B(*addr);
    (*addr)++;
    return(num);
}

/************************************************************************
* Module: z_calWrite_PcalParamSum()
* Description:
* Returns:
* Params:
*************************************************************************/
void z_calWrite_PcalParamSum(void)
{
    BYTE    btmp1,btmp2;
    USHORT  i,startaddr,endaddr;

    btmp1 = 0;
    btmp2 = 0;

    switch (svoPcalVar.LDMode)
    {
        case CD_LD:
            startaddr = CD_CAL_PWR_START_ADDR;
            endaddr = CD_CAL_PWR_END_ADDR;
            break;

        case DVD_LD:
            startaddr = DV_CAL_PWR_START_ADDR;
            endaddr = DV_CAL_PWR_END_ADDR;
            break;

#if (EN_Persistent_table_8 == 1)
        case BD_LD:
            startaddr = BD_CAL_PWR_START_ADDR;
            endaddr = BD_CAL_PWR_END_ADDR;
            break;

        case HD_LD:
            startaddr = HD_CAL_PWR_START_ADDR;
            endaddr = HD_CAL_PWR_END_ADDR;
            break;
#endif

        default:
            break;
    }
#if (SEEPROM == 1)
    for(i=startaddr;i<endaddr;)
    {
        btmp1 = btmp1^GetPersistentTable1B(i);
        btmp2 = btmp2^GetPersistentTable1B(i+1);
        i += 2;
    }
#else
    for(i=startaddr;i<endaddr;)
    {
        btmp1 = btmp1^ReadEepromMap_1B(i);
        btmp2 = btmp2^ReadEepromMap_1B(i+1);
        i += 2;
    }
#endif
    SetPersistentTable1B(endaddr  ,  btmp1);
    SetPersistentTable1B(endaddr+1,  btmp2);
    SetPersistentTable1B(endaddr+2, ~btmp1);
    SetPersistentTable1B(endaddr+3, ~btmp2);

}


/************************************************************************
* Module: z_calFlash_EepromMap()
* Description:
* Returns:
* Params:
*************************************************************************/
void z_calFlash_EepromMap(void)
{
#if (SEEPROM == 1)
    CloseWritableSegmentTable(ePersistent_Talbe);
#else
    if(svoPcalVar.fgLoadCalEeprom)
    {
        if(CloseWritableSegmentTable(ePersistent_Talbe) == 1)
    {
            BUILD_SENSE(0x04,0x4C,0x00,0x17);  /* LOGICAL UNIT FAILED SELF-CONFIGURATION */
        }
    }
#endif

}
#endif //disable offline calibration    

/****************************************************************************
* diaCmdDCErase() (F9h)
*
* This command can be used to DC Erase the PMA, LeadIn or the Entire Disc of
* a CDRW disc. This command is used when a Blank command can not blank a disc.
* The DC Erase will have adverse effects on the number of overwrites on a
* disc.
* Note: this implementation pass Mode and Dac bit to player module.
*****************************************************************************/
void diaCmdDCErase(void)
{
    if((Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE)){
        BUILD_SENSE(0x02,0x3A,0x00,0x36);                        /* MEDIUM NO PRESENT */
        AutoCommandComplete();
        return;
    }
    {
        BUILD_SENSE(0x05,0x26,0x01,0x50);                            /* PARAMETER NOT SUPPORTED */
        AutoCommandComplete();
    }
}
#endif //#if E_DIAGS == 1

#pragma arm section code = "SENDMSG"
#if (MSG_OUT_MODE == MSG_USBPORT)
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//================================================================
//debug message for SunMsg
//================================================================
#define BYTE0(arg)   (*(BYTE *)&(arg))
#define BYTE1(arg)   (*((BYTE *)&(arg) + 1))
#define BYTE2(arg)   (*((BYTE *)&(arg) + 2))
#define BYTE3(arg)   (*((BYTE *)&(arg) + 3))
#if (MuteSSI  == 1 )//[S07][U3]Jam disable SCLK & SDATA & SEN for U3 EMI 17MHz from Debug Board
#define WriteSSIReg_1B(addr, data)  //(*(vBYTE *)(HAL_APB_SSI_BaseAddress + addr) = (data))
#else
#define WriteSSIReg_1B(addr, data)  (*(vBYTE *)(HAL_APB_SSI_BaseAddress + addr) = (data))
#endif

#if ((MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_BOARD_TYPE == SUNEXT_DEBUG_BOARD))
    #if (DEBUG_PORT_OUTPUT_MODE == SSI_INTERFACE)
        #if (STORE_MSG_TO_SDRAM == 1)
            #define SUNEXT_OUT(MSG,CNT)                 \
                Sunext_SSI_Out((MSG)+2,(CNT)-2);        \
                if (EnableLogging == TRUE)              \
                {                                       \
                    MsgWriteSDRAM((MSG)+2,(CNT)-2);     \
                }
        #else
            #define SUNEXT_OUT(MSG,CNT)                 \
                Sunext_SSI_Out((MSG)+2,(CNT)-2);
        #endif
    #elif (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        #if (STORE_MSG_TO_SDRAM == 1)
            #define SUNEXT_OUT(MSG,CNT)                 \
                UARTOUT(MSG, CNT);                      \
                if (EnableLogging == TRUE)              \
                {                                       \
                    MsgWriteSDRAM((MSG)+2,(CNT)-2);     \
                }
        #else
            #define SUNEXT_OUT(MSG,CNT)                 \
                UARTOUT(MSG, CNT);
        #endif
    #endif
#endif

#if ((MSG_OUT_MODE == MSG_USBPORT)&&(DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD))
#if (DEBUG_PORT_OUTPUT_MODE==SSI_INTERFACE)
        #if (STORE_MSG_TO_SDRAM == 1)
            #define VENDOR1_OUT(MSG,CNT)                \
                Vendor1_SSI_Out(MSG,CNT);               \
                if (EnableLogging == TRUE)              \
                {                                       \
                    MsgWriteSDRAM(MSG,CNT);             \
                }
        #else
            #define VENDOR1_OUT(MSG,CNT)                \
                Vendor1_SSI_Out(MSG,CNT);
        #endif
    #elif (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        #if (STORE_MSG_TO_SDRAM == 1)
            #define VENDOR1_OUT(MSG,CNT)                \
                UARTOUT(MSG, CNT);                      \
                if (EnableLogging == TRUE)              \
                {                                       \
                    MsgWriteSDRAM(MSG,CNT);             \
                }
        #else
            #define VENDOR1_OUT(MSG,CNT)                \
                UARTOUT(MSG, CNT);
        #endif
    #endif
#endif

//*************************************************************************
//   Sunext_SSI_Out(BYTE *message,BYTE cnt)
//   This function send debug message to Sunext debug board through SSI interface
//   Parameter1   message data
//   Parameter2   number of byte of message data
//*************************************************************************
#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_BOARD_TYPE == SUNEXT_DEBUG_BOARD) && (DEBUG_PORT_OUTPUT_MODE == SSI_INTERFACE)
void Sunext_SSI_Out(BYTE *message,BYTE cnt)
{
    // for C0 chip, we have 64 bytes SSI FIFO (bug 5745)
    // if FIFO is half empty, we can send message out directly
    // without polling the FIFO full status

    #if (CHIP_REV >= 0xC0)
    // if (READ_FIELD(HAL_APB_SSI_SFIFEMP))
    if (READ_FIELD(HAL_APB_SSI_SFIFHLF))
    {
        WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
        WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);   /*  SYNC_PATT      */

        for (; cnt!=0; cnt--)
        {
            WriteSSIReg_1B(ADAPTER_ADDR_2,*message++);
        }

        WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);
    }
    else
    #endif
    {
        while (READ_FIELD(HAL_APB_SSI_SFIFFUL));
        WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);

        while (READ_FIELD(HAL_APB_SSI_SFIFFUL));
        WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);   /*  SYNC_PATT      */

        for (; cnt!=0; cnt--)
        {
            while (READ_FIELD(HAL_APB_SSI_SFIFFUL));
            WriteSSIReg_1B(ADAPTER_ADDR_2,*message++);
        }

        while (READ_FIELD(HAL_APB_SSI_SFIFFUL));
        WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);
    }
}
#endif

//*************************************************************************
//   Vendor1_SSI_Out(BYTE *message,BYTE cnt)
//   This function send debug message to Vendor1 debug board through SSI interface
//   Parameter1   message data
//   Parameter2   number of byte of message data, NOTE: must be even number
//*************************************************************************
#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD) && (DEBUG_PORT_OUTPUT_MODE == SSI_INTERFACE)
void Vendor1_SSI_Out(BYTE *message,BYTE cnt)
{
    // for C0 chip, we have 64 bytes SSI FIFO (bug 5745)
    // if FIFO is half empty, we can send message out directly
    // without polling the FIFO full status

    #if (CHIP_REV >= 0xC0)
    // if (READ_FIELD(HAL_APB_SSI_SFIFEMP))
    if (READ_FIELD(HAL_APB_SSI_SFIFHLF))
    {
        for (cnt=cnt/2; cnt!=0; cnt--)
        {
            USHORT Address = 0x800 + (*message++)*4;
            WriteSSIReg_1B(Address, *message++);
        }
    }
    else
    #endif
    {
        for (cnt=cnt/2; cnt!=0; cnt--)
        {
            USHORT Address = 0x800 + (*message++)*4;
            while (READ_FIELD(HAL_APB_SSI_SFIFFUL));
            WriteSSIReg_1B(Address, *message++);
        }
    }
}
#endif

//*************************************************************************
//   SendMsg()
//   Parameter1   A single CHARACTER type value.
//     However this same Byte value is sent to the interface 3 times!
//     However this same Byte value is sent to the interface 4 times!
//     Example for calling:     SendMsg('A');
//     Message string "AAAA" will be sent to interface.
//*************************************************************************
//void SendMsg(BYTE d1)
//{
//    BYTE msgbuf[12];
//
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//    msgbuf[0]=0x0D;
//    msgbuf[1]=0x0A;              /*  SYNC_PATT      */
//    #endif
//    msgbuf[2]=0x54;              /* 6 byte data     */
//    msgbuf[3]=d1;                /* DATA MESSAGE 2  */
//    msgbuf[4]=d1;                /* DATA MESSAGE 3  */
//    msgbuf[5]=d1;                /* DATA MESSAGE 4  */
//    msgbuf[6]=d1;                /* DATA MESSAGE 5  */
//    SUNEXT_OUT(&msgbuf[0],7);    /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//    msgbuf[0]=0xFD;                         /* SYNC                 */
//    msgbuf[1]=0x54;                         /* Data Mode and Length */
//    msgbuf[2]=0x00;                         /* Dummy byte for even number message */
//    msgbuf[3]=d1;                           /* DATA Message 1       */
//    msgbuf[4]=d1;                           /* DATA Message 2       */
//    msgbuf[5]=d1;                           /* DATA Message 3       */
//    msgbuf[6]=d1;                           /* DATA Message 4       */
//    msgbuf[7]=(char)d1<' '?' ':d1;          /* DATA Message 1       */
//    msgbuf[8]=(char)d1<' '?' ':d1;          /* DATA Message 2       */
//    msgbuf[9]=(char)d1<' '?' ':d1;          /* DATA Message 3       */
//    msgbuf[10]=(char)d1<' '?' ':d1;         /* DATA Message 4       */
//    msgbuf[11]=0;                           /* End of string        */
//    VENDOR1_OUT(&msgbuf[0],12);             /* Send Messages Out    */
//#endif
//}

//*************************************************************************
//   SendMsg1()
//   Four BYTE Parameters:
//     Example for calling:     SendMsg1('S','e','e','k');
//     Message string "Seek" will be sent to interface.
//*************************************************************************
void SendMsg1(BYTE d1,BYTE d2,BYTE d3,BYTE d4)
{
    BYTE msgbuf[12];

#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
    msgbuf[0]=0x0D;
    msgbuf[1]=0x0A;             /*  SYNC_PATT      */
    #endif
    msgbuf[2]=0x54;             /* 6 byte data     */
    msgbuf[3]=d1;               /* DATA MESSAGE 2  */
    msgbuf[4]=d2;               /* DATA MESSAGE 3  */
    msgbuf[5]=d3;               /* DATA MESSAGE 4  */
    msgbuf[6]=d4;               /* DATA MESSAGE 5  */
    SUNEXT_OUT(&msgbuf[0],7);               /* Send Messages Out    */
#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
    msgbuf[0]=0xFD;                         /* SYNC                 */
    msgbuf[1]=0x54;                         /* Data Mode and Length */
    msgbuf[2]=0x00;                         /* dummy byte for even number message */
    msgbuf[3]=d1;                           /* DATA Message 1       */
    msgbuf[4]=d2;                           /* DATA Message 2       */
    msgbuf[5]=d3;                           /* DATA Message 3       */
    msgbuf[6]=d4;                           /* DATA Message 4       */
    msgbuf[7]=(char)d1<' '?' ':d1;          /* DATA Message 1       */
    msgbuf[8]=(char)d2<' '?' ':d2;          /* DATA Message 2       */
    msgbuf[9]=(char)d3<' '?' ':d3;          /* DATA Message 3       */
    msgbuf[10]=(char)d4<' '?' ':d4;         /* DATA Message 4       */
    msgbuf[11]=0;                           /* End of string        */
    VENDOR1_OUT(&msgbuf[0],12);             /* Send Messages Out    */
#endif
}

//*************************************************************************
//   SendMsgCn()
//
//
//   Output an ASCII string of characters + value of multi variables (at most
//   12 bytes)
//
//     nArg    Total size of variables (in bytes)
//
//     Example for calling:
//     SendMsgCn(1, 5, 0xAABBCC, A4B(StartPSN), A1B(bdDiscRequest.TargetLayer));
//     would produce the following output
//                 Seek PSN=00100000h, Layer=80h
//*************************************************************************
void SendMsgCn(ULONG msg_gate, BYTE nArg, ULONG func, ...)
{
    ULONG msg_flag;
    BYTE count;
    va_list str;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (nArg > 12)
        {
        send_msg5SValue(1,"WRONG MSG",3,func);
        }
    else if (msg_flag & msg_gate)
    {
#if (DEBUG_BOARD_TYPE == SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                                             /* SYNC_PATT_START      */
    #endif
        msgbuf[2]=0xC0 + 3 + nArg;                                  /* Data Mode and Length */

        msgbuf[3] = BYTE2(func);                                    /* DATA Function Code   */
        msgbuf[4] = BYTE1(func);
        msgbuf[5] = BYTE0(func);

        va_start(str, func);
        for (count = 0; count < nArg; count++)
        {
            msgbuf[6+count] = va_arg(str, BYTE);                    /* DATA Message  N      */
        }
        va_end(str);

        SUNEXT_OUT(&msgbuf[0],3+3+nArg);                            /* Send Messages Out    */

#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xF3;                                     /* Data Mode and Length */

        msgbuf[1] = BYTE2(func);                                    /* DATA Function Code   */
        msgbuf[2] = BYTE1(func);
        msgbuf[3] = BYTE0(func);

        va_start(str, func);
        count = 4;
        if (nArg & 0x01)                                            /* Odd number */
        {
            msgbuf[count++] = 0x00;                                 /* Dummy byte for even number message */
        }

        for (; nArg != 0; nArg--)
            {
            msgbuf[count++] = va_arg(str, BYTE);                    /* DATA Message  N      */
            }
        nArg = count - 4;
            va_end(str);

        VENDOR1_OUT(&msgbuf[0],1+3+nArg);
#endif /* DEUG_BOARD_TYPE */
    }
}


//*************************************************************************
//   SendMsgCnB()
//
//
//   Output an ASCII string of characters + value of multi variables (at most
//   12 bytes)
//
//     nArg    Total sizes of variables (in bytes)
//
//     Example for calling:
//     SendMsgCnB(1,5,0xAABBCC, B4B(StartPSN), B1B(bdDiscRequest.TargetLayer));
//     would produce the following output
//                 Seek PSN=00100000h, Layer=80h
//*************************************************************************
void SendMsgCnB(ULONG msg_gate, BYTE nArg, ULONG func, ...)
{
    ULONG msg_flag;
    BYTE  var_width, total_size;
    BYTE msgbuf[18];
    BYTE *pArg;

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (nArg > 12)
                {
        send_msg5SValue(1,"WRONG MSG",3,func);
        }
    else if (msg_flag & msg_gate)
        {
#if (DEBUG_BOARD_TYPE == SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                                             /* SYNC_PATT_START      */
    #endif /* DEBUG_PORT_OUTPUT_MODE */
        msgbuf[2]=0xC0 + 3 + nArg;                                  /* Data Mode and Length */
        msgbuf[3] = BYTE2(func);                                    /* Data Function Code      */
        msgbuf[4] = BYTE1(func);
        msgbuf[5] = BYTE0(func);

        pArg = ((BYTE*)&func) + 4;
        total_size = 0;
        while (total_size < nArg)
        {
            var_width = *pArg;
            pArg += 4;
            switch(var_width)
            {
                case 8:
                {
                    float  FloatValue = *(double *)pArg;

                    msgbuf[6+total_size+3] = BYTE0(FloatValue);
                    msgbuf[6+total_size+2] = BYTE1(FloatValue);
                    msgbuf[6+total_size+1] = BYTE2(FloatValue);
                    msgbuf[6+total_size]   = BYTE3(FloatValue);
                    pArg += 8;
                    var_width = 4;
                }
                    break;
                case 4:
                    msgbuf[6+total_size+3] = *pArg++;
                    msgbuf[6+total_size+2] = *pArg++;
                    msgbuf[6+total_size+1] = *pArg++;
                    msgbuf[6+total_size]   = *pArg++;
                    break;
                case 3:
                    msgbuf[6+total_size+2] = *pArg++;
                    msgbuf[6+total_size+1] = *pArg++;
                    msgbuf[6+total_size]   = *pArg++;
                    pArg += 1;
                    break;
                case 2:
                    msgbuf[6+total_size+1] = *pArg++;
                    msgbuf[6+total_size]   = *pArg++;
                    pArg += 2;
                    break;
                default:
                    msgbuf[6+total_size] = *pArg;
                    pArg += 4;
                    break;
            }
            total_size += var_width;
        }

        SUNEXT_OUT(&msgbuf[0],3+3+nArg);                            /* Send Messages Out    */

#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xF3;

        msgbuf[1] = BYTE2(func);                                    /* Data Function Code      */
        msgbuf[2] = BYTE1(func);
        msgbuf[3] = BYTE0(func);                                    /* Data Mode and Length */

        pArg = ((BYTE*)&func) + 4;
        total_size = 0;

        if (nArg & 0x01)                                            /* odd number */
            {
            msgbuf[4] = 0x00;                                       /* dummy byte for even number message */
            total_size++;                                           /* shift 1 byte to store msgbuf data */
            nArg++;
            }

            while (total_size < nArg)
            {
                var_width = *pArg;
                pArg += 4;
                switch(var_width)
                {
                    case 8:
                    {
                        float  FloatValue = *(double *)pArg;
                        msgbuf[4+total_size+3] = BYTE0(FloatValue);
                        msgbuf[4+total_size+2] = BYTE1(FloatValue);
                        msgbuf[4+total_size+1] = BYTE2(FloatValue);
                        msgbuf[4+total_size]   = BYTE3(FloatValue);
                        pArg += 8;
                        var_width = 4;
                    }
                        break;
                    case 4:
                        msgbuf[4+total_size+3] = *pArg++;
                        msgbuf[4+total_size+2] = *pArg++;
                        msgbuf[4+total_size+1] = *pArg++;
                        msgbuf[4+total_size]   = *pArg++;
                        break;
                    case 3:
                        msgbuf[4+total_size+2] = *pArg++;
                        msgbuf[4+total_size+1] = *pArg++;
                        msgbuf[4+total_size]   = *pArg++;
                        pArg += 1;
                        break;
                    case 2:
                        msgbuf[4+total_size+1] = *pArg++;
                        msgbuf[4+total_size]   = *pArg++;
                        pArg += 2;
                        break;
                    default:
                        msgbuf[4+total_size] = *pArg;
                        pArg += 4;
                        break;
                }
                total_size += var_width;
            }

       VENDOR1_OUT(&msgbuf[0],1+3+nArg);                                /* SYNC_PATT_END        */
#endif /* DEUG_BOARD_TYPE */
    }
}


//*************************************************************************
//   send_msgA1()
//
//   FUN + char (1-byte integer)
//
//   Output an ASCII string of characters + char (1-byte integer).
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                       These three Bytes correspond to an entry in Database.txt
//                       file that is part of SunMsg.exe  program
//                       SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                       for example 0x61, 0x00, 0x01,
//                       would produce output text string "zone speed="
//                       (provided this entry is contained in the database when used)
//
//      Parameter 5       Used to output the content of a char (1-byte integer) variable.
//
//   Example for calling:   char xyz = 2;
//                            send_msgA1(1, 0x610001, xyz);
//     would produce the following output
//         zone speed= :2
//*************************************************************************
//void send_msgA1(ULONG msg_gate,ULONG func_code,char data1)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[8];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0xA4;                                 /* Data Mode and Length */
//        msgbuf[3]=BYTE2(func_code);                     /* Function Message 1   */
//        msgbuf[4]=BYTE1(func_code);                     /* Function Message 2   */
//        msgbuf[5]=BYTE0(func_code);                     /* Function Message 3   */
//        msgbuf[6]=(*((BYTE *)&(data1)));                /* DATA Message 1       */
//        SUNEXT_OUT(&msgbuf[0],7);                       /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xF3;                                 /* Data Mode and Length */
//        msgbuf[1]=BYTE2(func_code);                     /* Function Message 1   */
//        msgbuf[2]=BYTE1(func_code);                     /* Function Message 2   */
//        msgbuf[3]=BYTE0(func_code);                     /* Function Message 3   */
//        msgbuf[4]=0xFD;                                 /* SYNC                 */
//        msgbuf[5]=0xA1;                                 /* INT                  */
//        msgbuf[6]=0x00;                                 /* Dummy byte for even number message */
//        msgbuf[7]=(*((BYTE *)&(data1)));                /* DATA Message 1       */
//        VENDOR1_OUT(&msgbuf[0],8);                      /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//   send_msgA2()
//
//   FUN + short (2-byte integer)
//
//   Output an ASCII string of characters + short (2-byte integer).
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                       These three Bytes correspond to an entry in Database.txt
//                       file that is part of SunMsg.exe  program
//                       SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                       for example 0x51, 0x00, 0x9A,
//                       would produce output text string "STM_FEpp_L0_mV"
//                       (provided this entry is contained in the database when used)
//
//      Parameter 5       Used to output the content of a short (2-byte integer) variable.
//
//   Example for calling:   short xyz = 600;
//                            SendMsgA2(1, 0x51009A, xyz);
//     would produce the following output
//         STM_FEpp_L0_mV :600
//*************************************************************************
//void send_msgA2(ULONG msg_gate,ULONG func_code,short data1)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[8];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0xA5;                                 /* Data Mode and Length */
//        msgbuf[3]=BYTE2(func_code);                     /* Function Message 1   */
//        msgbuf[4]=BYTE1(func_code);                     /* Function Message 2   */
//        msgbuf[5]=BYTE0(func_code);                     /* Function Message 3   */
//        msgbuf[6]=(*((BYTE *)&(data1)  ));              /* DATA Message 1       */
//        msgbuf[7]=(*((BYTE *)&(data1)+1));              /* DATA Message 2       */
//        SUNEXT_OUT(&msgbuf[0],8);                       /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xF3;                                 /* Data Mode and Length */
//        msgbuf[1]=BYTE2(func_code);                     /* Function Message 1   */
//        msgbuf[2]=BYTE1(func_code);                     /* Function Message 2   */
//        msgbuf[3]=BYTE0(func_code);                     /* Function Message 3   */
//        msgbuf[4]=0xFD;                                 /* SYNC                 */
//        msgbuf[5]=0xA2;                                 /* INT                  */
//        msgbuf[6]=(*((BYTE *)&(data1)+1));              /* DATA Message 1       */
//        msgbuf[7]=(*((BYTE *)&(data1)));                /* DATA Message 2       */
//        VENDOR1_OUT(&msgbuf[0],8);                      /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//   send_msgA4()
//
//   FUN + long (4-byte integer)
//
//   Output an ASCII string of characters + long (4-byte integer).
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                       These three Bytes correspond to an entry in Database.txt
//                       file that is part of SunMsg.exe  program
//                       SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                       for example 0x51, 0x00, 0x9A,
//                       would produce output text string "STM_FEpp_L0_mV"
//                       (provided this entry is contained in the database when used)
//
//      Parameter 5       Used to output the content of a long (4-byte integer) variable.
//
//   Example for calling:   long xyz = 600;
//                            SendMsgA4(1, 0x51009A, xyz);
//     would produce the following output
//         STM_FEpp_L0_mV :600
//*************************************************************************
//void send_msgA4(ULONG msg_gate,ULONG func_code,long data1)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[10];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0xA7;                                 /* Data Mode and Length */
//        msgbuf[3]=BYTE2(func_code);                     /* Function Message 1   */
//        msgbuf[4]=BYTE1(func_code);                     /* Function Message 2   */
//        msgbuf[5]=BYTE0(func_code);                     /* Function Message 3   */
//        msgbuf[6]=(*((BYTE *)&(data1)  ));              /* DATA Message 1       */
//        msgbuf[7]=(*((BYTE *)&(data1)+1));              /* DATA Message 2       */
//        msgbuf[8]=(*((BYTE *)&(data1)+2));              /* DATA Message 3       */
//        msgbuf[9]=(*((BYTE *)&(data1)+3));              /* DATA Message 4       */
//        SUNEXT_OUT(&msgbuf[0],10);                         /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xF3;                                 /* Data Mode and Length */
//        msgbuf[1]=BYTE2(func_code);                     /* Function Message 1   */
//        msgbuf[2]=BYTE1(func_code);                     /* Function Message 2   */
//        msgbuf[3]=BYTE0(func_code);                     /* Function Message 3   */
//        msgbuf[4]=0xFD;                                 /* SYNC                 */
//        msgbuf[5]=0xA4;                                 /* INT                  */
//        msgbuf[6]=(*((BYTE *)&(data1)+3));              /* DATA Message 1       */
//        msgbuf[7]=(*((BYTE *)&(data1)+2));              /* DATA Message 2       */
//        msgbuf[8]=(*((BYTE *)&(data1)+1));              /* DATA Message 3       */
//        msgbuf[9]=(*((BYTE *)&(data1)));                /* DATA Message 4       */
//        VENDOR1_OUT(&msgbuf[0],10);                     /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//   SendMsg80()
//
//   FUN
//
//   Output an ASCII string of characters
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                        These three Bytes correspond to an entry in Database.txt
//                        file that is part of SunMsg.exe  program
//                        SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                        for example 0x55, 0x01, 0x93,
//                        would produce output text string "Disc_Kind==>eDVDMinusRWDisc"
//                        (provided this entry is contained in the database when used)
//
//     Example for calling:     SendMsg80(1, 0x550193);
//     would produce the following output
//         Disc_Kind==>eDVDMinusRWDisc :
//*************************************************************************
void SendMsg80(ULONG msg_gate, ULONG func_code)
{
    ULONG msg_flag;
    BYTE  msgbuf[6];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
    #endif
        msgbuf[2]=0x83;                         /* Data Mode and Length */
        msgbuf[3]=BYTE2(func_code);                         /* Function Code 1      */
        msgbuf[4]=BYTE1(func_code);                         /* Function Code 2      */
        msgbuf[5]=BYTE0(func_code);                         /* Function Code 3      */
        SUNEXT_OUT(&msgbuf[0],6);                           /* Send Messages Out    */
#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xF3;
        msgbuf[1]=BYTE2(func_code);                         /* Function Code 1      */
        msgbuf[2]=BYTE1(func_code);                         /* Function Code 2      */
        msgbuf[3]=BYTE0(func_code);                         /* Function Code 3      */
        VENDOR1_OUT(&msgbuf[0],4);                          /* Send Messages Out    */
#endif /* DEUG_BOARD_TYPE */
    }
}

//*************************************************************************
//   SendMsg81()
//
//   FUN + 1 Hex
//
//   Output an ASCII string of characters + 1 BYTE value
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                        These three Bytes correspond to an entry in Database.txt
//                        file that is part of SunMsg.exe  program
//                        SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                        for example 0x55, 0x01, 0x93,
//                        would produce output text string "Disc_Kind==>eDVDMinusRWDisc"
//                        (provided this entry is contained in the database when used)
//
//      Parameter 5       Used to output the content of a single BYTE variable
//
//     Example for calling:     SendMsg81(1, 0x550193, 0x96);
//     would produce the following output
//         Disc_Kind==>eDVDMinusRWDisc :96h
//*************************************************************************
//void SendMsg81(ULONG msg_gate, ULONG func_code, BYTE h1)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[8];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0x84;                         /* Data Mode and Length */
//        msgbuf[3]=BYTE2(func_code);             /* Function Message 1   */
//        msgbuf[4]=BYTE1(func_code);             /* Function Message 2   */
//        msgbuf[5]=BYTE0(func_code);             /* Function Message 3   */
//        msgbuf[6]=h1;                           /* DATA Message 1       */
//        SUNEXT_OUT(&msgbuf[0],7);               /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xF3;                         /* Data Mode and Length */
//        msgbuf[1]=BYTE2(func_code);             /* Function Message 1   */
//        msgbuf[2]=BYTE1(func_code);             /* Function Message 2   */
//        msgbuf[3]=BYTE0(func_code);             /* Function Message 3   */
//        msgbuf[4]=0xFD;                         /* SYNC                 */
//        msgbuf[5]=0x81;                         /* HEX                  */
//        msgbuf[6]=0x00;                         /* Dummy byte for even number message */
//        msgbuf[7]=h1;                           /* DATA Message 1       */
//        VENDOR1_OUT(&msgbuf[0],8);              /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//   SendMsg82()
//
//   FUN + 2 Hex
//
//   Output an ASCII string of characters + 2 BYTE value
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                        These three Bytes correspond to an entry in Database.txt
//                        file that is part of SunMsg.exe  program
//                        SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                        for example 0x55, 0x01, 0x93,
//                        would produce output text string "Disc_Kind==>eDVDMinusRWDisc"
//                        (provided this entry is contained in the database when used)
//
//      Parameter 5,6  Used to output the content of a four single BYTE variables
//
//     Example for calling:     SendMsg82(1, 0x550193, 0x33,0x34);
//     would produce the following output
//         Disc_Kind==>eDVDMinusRWDisc :33h 34h
//*************************************************************************
//void SendMsg82(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[8];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0x85;                         /* Data Mode and Length */
//        msgbuf[3]=BYTE2(func_code);             /* Function Message 1   */
//        msgbuf[4]=BYTE1(func_code);             /* Function Message 2   */
//        msgbuf[5]=BYTE0(func_code);             /* Function Message 3   */
//        msgbuf[6]=h1;                           /* DATA Message 1       */
//        msgbuf[7]=h2;                           /* DATA Message 2       */
//        SUNEXT_OUT(&msgbuf[0],8);               /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xF3;
//        msgbuf[1]=BYTE2(func_code);             /* Function Message 1   */
//        msgbuf[2]=BYTE1(func_code);             /* Function Message 2   */
//        msgbuf[3]=BYTE0(func_code);             /* Function Message 3   */
//        msgbuf[4]=0xFD;                         /* SYNC                 */
//        msgbuf[5]=0x82;                         /* HEX                  */
//        msgbuf[6]=h1;                           /* DATA Message 1       */
//        msgbuf[7]=h2;                           /* DATA Message 2       */
//        VENDOR1_OUT(&msgbuf[0],8);              /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//   SendMsg84()
//
//   FUN + 4 Hex
//
//   Output an ASCII string of characters + 4 BYTE value
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                        These three Bytes correspond to an entry in Database.txt
//                        file that is part of SunMsg.exe  program
//                        SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                        for example 0x55, 0x01, 0x93,
//                        would produce output text string "Disc_Kind==>eDVDMinusRWDisc"
//                        (provided this entry is contained in the database when used)
//
//      Parameter 5,6,7,8  Used to output the content of a four single BYTE variables
//
//     Example for calling:     SendMsg84(1, 0x550193, 0x33,0x34,0x35,0x36);
//     would produce the following output
//         Disc_Kind==>eDVDMinusRWDisc :33h 34h 35h 36h
//*************************************************************************
//void SendMsg84(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2,BYTE h3,BYTE h4)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[10];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0x87;                         /* Data Mode and Length */
//        msgbuf[3]=BYTE2(func_code);             /* Function Message 1   */
//        msgbuf[4]=BYTE1(func_code);             /* Function Message 2   */
//        msgbuf[5]=BYTE0(func_code);             /* Function Message 3   */
//        msgbuf[6]=h1;                           /* DATA Message 1       */
//        msgbuf[7]=h2;                           /* DATA Message 2       */
//        msgbuf[8]=h3;                           /* DATA Message 3       */
//        msgbuf[9]=h4;                           /* DATA Message 4       */
//        SUNEXT_OUT(&msgbuf[0],10);              /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xF3;                         /* Data Mode and Length */
//        msgbuf[1]=BYTE2(func_code);             /* Function Message 1   */
//        msgbuf[2]=BYTE1(func_code);             /* Function Message 2   */
//        msgbuf[3]=BYTE0(func_code);             /* Function Message 3   */
//        msgbuf[4]=0xFD;                         /* SYNC                 */
//        msgbuf[5]=0x84;                         /* HEX                  */
//        msgbuf[6]=h1;                           /* DATA Message 1       */
//        msgbuf[7]=h2;                           /* DATA Message 2       */
//        msgbuf[8]=h3;                           /* DATA Message 3       */
//        msgbuf[9]=h4;                           /* DATA Message 4       */
//        VENDOR1_OUT(&msgbuf[0],10);             /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//   send_msg74()
//
//   FUN + Floating Point
//
//   Output an ASCII string of characters + 1 BYTE value
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4  Three bytes Message Code
//                        These three Bytes correspond to an entry in Database.txt
//                        file that is part of SunMsg.exe  program
//                        SunMsg is an Interpreter and display program monitoring SSI or RS232 port.
//                        for example 0x55, 0x01, 0x93,
//                        would produce output text string "Disc_Kind==>eDVDMinusRWDisc"
//                        (provided this entry is contained in the database when used)
//
//      Parameter 5       Used to output the content of a single Floating Point variable (value will be rounded)
//
//     Example for calling:   float xyz = 2.98765;
//                            send_msg74(1,  0x550193, xyz);
//     would produce the following output
//         Disc_Kind==>eDVDMinusRWDisc :2.99
//*************************************************************************
void send_msg74(ULONG msg_gate,ULONG func_code,float data1)
{
    ULONG msg_flag;
    BYTE  msgbuf[10];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
    #endif
        msgbuf[2]=0x77;                                 /* Data Mode and Length */
        msgbuf[3]=BYTE2(func_code);                     /* Function Message 1   */
        msgbuf[4]=BYTE1(func_code);                     /* Function Message 2   */
        msgbuf[5]=BYTE0(func_code);                     /* Function Message 3   */
        msgbuf[6]=(*((BYTE *)&(data1)  ));              /* DATA Message 1       */
        msgbuf[7]=(*((BYTE *)&(data1)+1));              /* DATA Message 2       */
        msgbuf[8]=(*((BYTE *)&(data1)+2));              /* DATA Message 3       */
        msgbuf[9]=(*((BYTE *)&(data1)+3));              /* DATA Message 4       */
        SUNEXT_OUT(&msgbuf[0],10);                      /* Send Messages Out    */
#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xF3;                                 /* Data Mode and Length */
        msgbuf[1]=BYTE2(func_code);                     /* Function Message 1   */
        msgbuf[2]=BYTE1(func_code);                     /* Function Message 2   */
        msgbuf[3]=BYTE0(func_code);                     /* Function Message 3   */
        msgbuf[4]=0xFD;                                 /* SYNC                 */
        msgbuf[5]=0x74;                                 /* FLO                  */
        msgbuf[6]=(*((BYTE *)&(data1)+3 ));             /* DATA Message 1       */
        msgbuf[7]=(*((BYTE *)&(data1)+2));              /* DATA Message 2       */
        msgbuf[8]=(*((BYTE *)&(data1)+1));              /* DATA Message 3       */
        msgbuf[9]=(*((BYTE *)&(data1)+0));              /* DATA Message 4       */
        VENDOR1_OUT(&msgbuf[0],10);                     /* Send Messages Out    */
#endif
    }
}


//*************************************************************************
//   SendMsg6C()
//
//   ATA Command
//
//   Output current values of all parameters in ATA Command Descriptor Block
//   containd in variable Iram_Cmd_block.bf[0..11]
//      Parameter 1      Message Gate (to enable message)
//                        No parameters other than Message Gate
//     Example for calling:     SendMsg6C();
//     could produce the following or some other output:
//     ATAPI   00 00 00 00 00 00 00 00 00 00 00 00  TEST UNIT READY
//
//*************************************************************************
//void SendMsg6C(ULONG msg_gate)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[15];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0] =0x0D;
//        msgbuf[1] =0x0A;                                /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2] =0x6C;                            /* Data Mode and Length */
//        msgbuf[3] =Iram_Cmd_block.bf[0];                /* ATA Command 1        */
//        msgbuf[4] =Iram_Cmd_block.bf[1];                /* ATA Command 2        */
//        msgbuf[5] =Iram_Cmd_block.bf[2];                /* ATA Command 3        */
//        msgbuf[6] =Iram_Cmd_block.bf[3];                /* ATA Command 4        */
//        msgbuf[7] =Iram_Cmd_block.bf[4];                /* ATA Command 5        */
//        msgbuf[8] =Iram_Cmd_block.bf[5];                /* ATA Command 6        */
//        msgbuf[9] =Iram_Cmd_block.bf[6];                /* ATA Command 7        */
//        msgbuf[10]=Iram_Cmd_block.bf[7];                /* ATA Command 8        */
//        msgbuf[11]=Iram_Cmd_block.bf[8];                /* ATA Command 9        */
//        msgbuf[12]=Iram_Cmd_block.bf[9];                /* ATA Command 10       */
//        msgbuf[13]=Iram_Cmd_block.bf[10];               /* ATA Command 11       */
//        msgbuf[14]=Iram_Cmd_block.bf[11];               /* ATA Command 12       */
//        SUNEXT_OUT(&msgbuf[0],15);                  /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0] =0xFA;								/* SYNC_PATT_START		*/
//        msgbuf[1] =0xA0;                            /* Data Mode and Length */
//        msgbuf[2] =Iram_Cmd_block.bf[0];				/* ATA Command 1		*/
//        msgbuf[3] =Iram_Cmd_block.bf[1];				/* ATA Command 2		*/
//        msgbuf[4] =Iram_Cmd_block.bf[2];				/* ATA Command 3		*/
//        msgbuf[5] =Iram_Cmd_block.bf[3];				/* ATA Command 4		*/
//        msgbuf[6] =Iram_Cmd_block.bf[4];				/* ATA Command 5		*/
//        msgbuf[7] =Iram_Cmd_block.bf[5];				/* ATA Command 6		*/
//        msgbuf[8] =Iram_Cmd_block.bf[6];				/* ATA Command 7		*/
//        msgbuf[9] =Iram_Cmd_block.bf[7];				/* ATA Command 8		*/
//        msgbuf[10]=Iram_Cmd_block.bf[8];				/* ATA Command 9		*/
//        msgbuf[11]=Iram_Cmd_block.bf[9];				/* ATA Command 10		*/
//        msgbuf[12]=Iram_Cmd_block.bf[10];				/* ATA Command 11		*/
//        msgbuf[13]=Iram_Cmd_block.bf[11];				/* ATA Command 12		*/
//        VENDOR1_OUT(&msgbuf[0],14);                 /* Send Messages Out    */
//#endif
//    }
//}
//*************************************************************************
//   SendMsg6F()
//
//   ATA Command
//
//   Output current values of all parameters in ATA Command Descriptor Block
//   containd in variable Iram_Cmd_block.bf[0..11]
//      Parameter 1      Message Gate (to enable message)
//                        No parameters other than Message Gate
//     Example for calling:     SendMsg6F();
//     could produce the following or some other output:
//     ATAPI   00 00 00 00 00 00 00 00 00 00 00 00  TEST UNIT READY Time xxxms
//
//*************************************************************************
void SendMsg6F(ULONG msg_gate)
{
    ULONG CmdTime;
    ULONG msg_flag;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
        CmdTime = ReadTimer(&Iram_atapi_timer);

#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
        msgbuf[0] =0x0D;
        msgbuf[1] =0x0A;                                /* SYNC_PATT_START      */
    #endif
        msgbuf[2] =0x6F;                                /* Data Mode and Length */
        msgbuf[3] =Iram_Cmd_block.bf[0];                /* ATA Command 1        */
        msgbuf[4] =Iram_Cmd_block.bf[1];                /* ATA Command 2        */
        msgbuf[5] =Iram_Cmd_block.bf[2];                /* ATA Command 3        */
        msgbuf[6] =Iram_Cmd_block.bf[3];                /* ATA Command 4        */
        msgbuf[7] =Iram_Cmd_block.bf[4];                /* ATA Command 5        */
        msgbuf[8] =Iram_Cmd_block.bf[5];                /* ATA Command 6        */
        msgbuf[9] =Iram_Cmd_block.bf[6];                /* ATA Command 7        */
        msgbuf[10]=Iram_Cmd_block.bf[7];                /* ATA Command 8        */
        msgbuf[11]=Iram_Cmd_block.bf[8];                /* ATA Command 9        */
        msgbuf[12]=Iram_Cmd_block.bf[9];                /* ATA Command 10       */
        msgbuf[13]=Iram_Cmd_block.bf[10];               /* ATA Command 11       */
        msgbuf[14]=Iram_Cmd_block.bf[11];               /* ATA Command 12       */
        msgbuf[15] = CmdTime>>16;
        msgbuf[16] = CmdTime>>8;
        msgbuf[17] = CmdTime;
        SUNEXT_OUT(&msgbuf[0],18);                      /* Send Messages Out    */
#elif (DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD)
        msgbuf[0] =0xFA;								/* SYNC_PATT_START		*/
        msgbuf[1] =0xA1;                                /* Data Mode and Length */
        msgbuf[2] =0x00;                                /* Dummy byte for even number message */
        msgbuf[3] =Iram_Cmd_block.bf[0];                /* ATA Command 1        */
        msgbuf[4] =Iram_Cmd_block.bf[1];                /* ATA Command 2        */
        msgbuf[5] =Iram_Cmd_block.bf[2];                /* ATA Command 3        */
        msgbuf[6] =Iram_Cmd_block.bf[3];                /* ATA Command 4        */
        msgbuf[7] =Iram_Cmd_block.bf[4];                /* ATA Command 5        */
        msgbuf[8] =Iram_Cmd_block.bf[5];                /* ATA Command 6        */
        msgbuf[9] =Iram_Cmd_block.bf[6];                /* ATA Command 7        */
        msgbuf[10]=Iram_Cmd_block.bf[7];                /* ATA Command 8        */
        msgbuf[11]=Iram_Cmd_block.bf[8];                /* ATA Command 9        */
        msgbuf[12]=Iram_Cmd_block.bf[9];                /* ATA Command 10       */
        msgbuf[13]=Iram_Cmd_block.bf[10];               /* ATA Command 11       */
        msgbuf[14]=Iram_Cmd_block.bf[11];               /* ATA Command 12       */
        msgbuf[15] = CmdTime>>16;
        msgbuf[16] = CmdTime>>8;
        msgbuf[17] = CmdTime;
        VENDOR1_OUT(&msgbuf[0],18);
#endif /* DEUG_BOARD_TYPE */
    }
}

#if 1
//*************************************************************************
//   SendMsg54()
//
//   4 Hex
//
//   Output four single bytes (or a short 4 char text)
//
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2,3,4,5 Used to output the content of a four single BYTE variables
//                        without any direct or indirect text string.
//
//     Example for calling:     SendMsg54(1   ,'E', 'D', 'C', 'E');
//     would produce the following output
//                 45 43 44 45   EDCE
//*************************************************************************
void SendMsg54(ULONG msg_gate,BYTE d1,BYTE d2,BYTE d3,BYTE d4)
{
    ULONG msg_flag;
    BYTE  msgbuf[12];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
    #endif
        msgbuf[2]=0x54;                         /* Data Mode and Length */
        msgbuf[3]=d1;                           /* DATA Message 1       */
        msgbuf[4]=d2;                           /* DATA Message 2       */
        msgbuf[5]=d3;                           /* DATA Message 3       */
        msgbuf[6]=d4;                           /* DATA Message 4       */
        SUNEXT_OUT(&msgbuf[0],7);               /* Send Messages Out    */
#endif

#if (DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xFD;                         /* SYNC                 */
        msgbuf[1]=0x54;                         /* Data Mode and Length */
        msgbuf[2]=0x00;                         /* Dummy byte for even number message */
        msgbuf[3]=d1;                           /* DATA Message 1       */
        msgbuf[4]=d2;                           /* DATA Message 2       */
        msgbuf[5]=d3;                           /* DATA Message 3       */
        msgbuf[6]=d4;                           /* DATA Message 4       */
        msgbuf[7]=(char)d1<' '?' ':d1;          /* DATA Message 1       */
        msgbuf[8]=(char)d2<' '?' ':d2;          /* DATA Message 2       */
        msgbuf[9]=(char)d3<' '?' ':d3;          /* DATA Message 3       */
        msgbuf[10]=(char)d4<' '?' ':d4;         /* DATA Message 4       */
        msgbuf[11]=0;                           /* End of string        */
        VENDOR1_OUT(&msgbuf[0],12);             /* Send Messages Out    */
#endif
    }
}
#endif


//*************************************************************************
//   SendMsg5S()
//
//
//   Output a text string
//
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2      Character pointer to a text string
//
//     Example for calling:     SendMsg5S(1   , "SEEK FAIL");
//     would produce the following output
//                 SEEK FAIL
//*************************************************************************
void SendMsg5S(ULONG msg_gate, char *message)
{
    ULONG msg_flag;
    BYTE datalength, count;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
        datalength = strlen(message);
        count = 0;
        if(datalength > 15)
        {
            datalength = 15;
        }
#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
    #endif
        msgbuf[2]=0x50 + datalength;                    /* Data Mode and Length */
        for(count = 0; count <datalength ; count++)
        {
            msgbuf[3+count] = message[count];           /* DATA Message N       */
        }
        SUNEXT_OUT(&msgbuf[0],3+datalength);
#elif (DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xFD;                                 /* SYNC                 */
        msgbuf[1]=0x50;                     /* Data Mode and Length */

        for(count = 0; count <datalength ; count++)
        {
            char Ch = message[count];
            if (Ch < ' ') Ch = ' ';
                msgbuf[2+count] = Ch;               /* DATA Message  N      */
        }
        if ((datalength & 0x01) == 0)               /* string length is even number */
        {
            msgbuf[2+datalength] = ' ';             /* Dummy space for even number message */
            datalength++;
            }
        msgbuf[2+datalength] = 0;                       /* End of string        */
        VENDOR1_OUT(&msgbuf[0],3+datalength);           /* SYNC_PATT_END        */
#endif /* DEUG_BOARD_TYPE */
    }
}
//*************************************************************************
//   SendMsg5SValue()
//
//
//   Output a text string + a ULONG
//
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2      Character pointer to a text string
//      Parameter 3      Number of bytes to output of next parameter
//      Parameter 4      ULONG value
//
//     Example for calling:     SendMsg5SValue(1   , "PSN= ", 4, 0x12345678L);
//     would produce the following output
//                 PSN= :12345678
//*************************************************************************
void SendMsg5SValue(ULONG msg_gate, char *message, char size, ULONG value)
{
    ULONG msg_flag;
    BYTE DataLen, count;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
        DataLen = strlen(message);

        if (size > 4) size = 4;
        if (DataLen > (14 - size))
        {
            DataLen = (14 - size);
        }

#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
    #endif
        // msgbuf[2]=0x50 + DataLen;                    /* Data Mode and Length */
        for(count = 0; count <DataLen ; count++)
        {
            msgbuf[3+count] = message[count];           /* DATA Message  N      */
        }
        count += 3;
        msgbuf[count++] = ':';
        if (size == 4) msgbuf[count++] = GET_BYTE3(value);
        if (size >= 3) msgbuf[count++] = GET_BYTE2(value);
        if (size >= 2) msgbuf[count++] = GET_BHIGH(value);
        msgbuf[count++] = GET_BLOW(value);
        msgbuf[2]=0x50 + count - 3;                     /* Data Mode and Length */
        SUNEXT_OUT(&msgbuf[0],count);
#elif (DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0xFD;                                 /* SYNC                 */
        msgbuf[1]=0x50;                     /* Data Mode and Length */
        for(count = 0; count <DataLen ; count++)
        {
            msgbuf[2+count] = message[count];           /* DATA Message  N      */
        }
        count += 2;
        msgbuf[count++] = ':';
        if ((count & 0x01) == 0)
        {
            msgbuf[count++] = ' ';                      /* Dummy space for even number message */
        }
        msgbuf[count++] = 0;                            /* End of string        */
        VENDOR1_OUT(&msgbuf[0],count);

        msgbuf[0]=0xFD;
        msgbuf[1]=0x80 + size;
        count = 2;
        if (size & 0x01) msgbuf[count++] = 0;
        if (size == 4) msgbuf[count++] = GET_BYTE3(value);
        if (size >= 3) msgbuf[count++] = GET_BYTE2(value);
        if (size >= 2) msgbuf[count++] = GET_BHIGH(value);
        msgbuf[count++] = GET_BLOW(value);
        VENDOR1_OUT(&msgbuf[0],count);
#endif /* DEUG_BOARD_TYPE */
    }
}

//*************************************************************************
//   SendMsg5L()
//
//
//   Output a text string + a ULONG
//
//      Parameter 1      Message Gate (to enable message)
//      Parameter 2      ULONG value
//
//     Example for calling:     SendMsg5L(1   ,  0x12345678L);
//     would produce the following output
//                 PSN= :12345678
//*************************************************************************
//void SendMsg5L(ULONG msg_gate, ULONG value)
//{
//    ULONG msg_flag;
//    BYTE msgbuf[18];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0]=0x0D;
//        msgbuf[1]=0x0A;                                 /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2]=0x54;                                 /* Data Mode and Length */
//        msgbuf[3]=GET_BYTE3(value);
//        msgbuf[4]=GET_BYTE2(value);
//        msgbuf[5]=GET_BHIGH(value);
//        msgbuf[6]=GET_BLOW(value);
//        SUNEXT_OUT(&msgbuf[0],7);                       /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0]=0xFD;                                 /* SYNC                 */
//        msgbuf[1]=0x54;                                 /* Data Mode and Length */
//        msgbuf[2]=0x00;                                 /* Dummy byte for even number message */
//        msgbuf[3]=GET_BYTE3(value);
//        msgbuf[4]=GET_BYTE2(value);
//        msgbuf[5]=GET_BHIGH(value);
//        msgbuf[6]=GET_BLOW(value);
//        msgbuf[7]=(char)msgbuf[3]<' '?' ':msgbuf[3];
//        msgbuf[8]=(char)msgbuf[4]<' '?' ':msgbuf[4];
//        msgbuf[9]=(char)msgbuf[5]<' '?' ':msgbuf[5];
//        msgbuf[10]=(char)msgbuf[6]<' '?' ':msgbuf[6];
//        msgbuf[11]=0;                                   /* End of string        */
//        VENDOR1_OUT(&msgbuf[0],12);                     /* Send Messages Out    */
//#endif
//    }
//}

//*************************************************************************
//
//     SendMsg5F
//
//     Output 15 ASCII characters
//
//      Parameter 1         Message Gate (to enable message)
//      Parameters 2..16    15 ASCII characters to output
//
//
//     Example for calling:     SendMsg5F(1L  ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O' );
//     would produce the following output
//         ABCDEFGHIJKLMNO
//
//*************************************************************************
//void SendMsg5F(ULONG msg_gate,BYTE h1,BYTE h2 ,BYTE h3 ,BYTE h4 ,BYTE h5 ,BYTE h6 ,BYTE h7 ,BYTE h8
//                              ,BYTE h9,BYTE h10,BYTE h11,BYTE h12,BYTE h13,BYTE h14,BYTE h15)
//{
//    ULONG msg_flag;
//    BYTE  msgbuf[34];
//
//    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);
//
//    if (msg_flag & msg_gate)
//    {
//#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
//    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
//        msgbuf[0] =0x0D;
//        msgbuf[1] =0x0A;                        /* SYNC_PATT_START      */
//    #endif
//        msgbuf[2] =0x5F;                        /* Data Mode and Length */
//        msgbuf[3] =h1;                          /* DATA Message 1       */
//        msgbuf[4] =h2;                          /* DATA Message 2       */
//        msgbuf[5] =h3;                          /* DATA Message 3       */
//        msgbuf[6] =h4;                          /* DATA Message 4       */
//        msgbuf[7] =h5;                          /* DATA Message 5       */
//        msgbuf[8] =h6;                          /* DATA Message 6       */
//        msgbuf[9] =h7;                          /* DATA Message 7       */
//        msgbuf[10]=h8;                          /* DATA Message 8       */
//        msgbuf[11]=h9;                          /* DATA Message 9       */
//        msgbuf[12]=h10;                         /* DATA Message 10      */
//        msgbuf[13]=h11;                         /* DATA Message 11      */
//        msgbuf[14]=h12;                         /* DATA Message 12      */
//        msgbuf[15]=h13;                         /* DATA Message 13      */
//        msgbuf[16]=h14;                         /* DATA Message 14      */
//        msgbuf[17]=h15;                         /* DATA Message 15      */
//        SUNEXT_OUT(&msgbuf[0],18);                  /* Send Messages Out    */
//#elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
//        msgbuf[0] =0xFD;                        /* SYNC                 */
//        msgbuf[1] =0x5F;                        /* Data Mode and Length */
//        msgbuf[2] =0x00;                            /* Dummy byte for even number message */
//        msgbuf[3] =h1;                              /* DATA Message 1       */
//        msgbuf[4] =h2;                              /* DATA Message 2       */
//        msgbuf[5] =h3;                              /* DATA Message 3       */
//        msgbuf[6] =h4;                              /* DATA Message 4       */
//        msgbuf[7] =h5;                              /* DATA Message 5       */
//        msgbuf[8] =h6;                              /* DATA Message 6       */
//        msgbuf[9] =h7;                              /* DATA Message 7       */
//        msgbuf[10]=h8;                              /* DATA Message 8       */
//        msgbuf[11]=h9;                              /* DATA Message 9       */
//        msgbuf[12]=h10;                             /* DATA Message 10      */
//        msgbuf[13]=h11;                             /* DATA Message 11      */
//        msgbuf[14]=h12;                             /* DATA Message 12      */
//        msgbuf[15]=h13;                             /* DATA Message 13      */
//        msgbuf[16]=h14;                             /* DATA Message 14      */
//        msgbuf[17]=h15;                             /* DATA Message 15      */
//        msgbuf[18]=(char)h1<' '?' ':h1;             /* DATA Message 1       */
//        msgbuf[19]=(char)h2<' '?' ':h2;             /* DATA Message 2       */
//        msgbuf[20]=(char)h3<' '?' ':h3;             /* DATA Message 3       */
//        msgbuf[21]=(char)h4<' '?' ':h4;             /* DATA Message 4       */
//        msgbuf[22]=(char)h5<' '?' ':h5;             /* DATA Message 5       */
//        msgbuf[23]=(char)h6<' '?' ':h6;             /* DATA Message 6       */
//        msgbuf[24]=(char)h7<' '?' ':h7;             /* DATA Message 7       */
//        msgbuf[25]=(char)h8<' '?' ':h8;             /* DATA Message 8       */
//        msgbuf[26]=(char)h9<' '?' ':h9;             /* DATA Message 9       */
//        msgbuf[27]=(char)h10<' '?' ':h10;           /* DATA Message 10      */
//        msgbuf[28]=(char)h11<' '?' ':h11;           /* DATA Message 11      */
//        msgbuf[29]=(char)h12<' '?' ':h12;           /* DATA Message 12      */
//        msgbuf[30]=(char)h13<' '?' ':h13;           /* DATA Message 13      */
//        msgbuf[31]=(char)h14<' '?' ':h14;           /* DATA Message 14      */
//        msgbuf[32]=(char)h15<' '?' ':h15;           /* DATA Message 15      */
//        msgbuf[33]=0;                               /* End of string        */
//        VENDOR1_OUT(&msgbuf[0],34);                 /* Send Messages Out    */
//#endif
//    }
//}


//*************************************************************************
//    SendMsg3A()
//
// TIME
//    Function sends content of Subcode structure to debug interface.
//      Parameter 1      Message Gate (to enable message)
//
//
//     Example for calling:     SendMsg3A(1L);
//
//      Function outputs the following items:
//      (Iram_DiscInfo.CD.Q.Con << 4) | Iram_DiscInfo.CD.Q.Addr)
//      Iram_DiscInfo.CD.Q.Tno);
//      Iram_DiscInfo.CD.Q.Point);
//      Iram_DiscInfo.CD.Q.Minute);
//      Iram_DiscInfo.CD.Q.Second);
//      Iram_DiscInfo.CD.Q.Frame);
//      Iram_DiscInfo.CD.Q.Zero);
//      Iram_DiscInfo.CD.Q.PMinute);
//      Iram_DiscInfo.CD.Q.PSecond);
//      Iram_DiscInfo.CD.Q.PFrame);
//
//*************************************************************************

void SendMsg3A(ULONG msg_gate)
{
    ULONG msg_flag;
    BYTE  msgbuf[14];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
#if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    #if (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
        msgbuf[0] =0x0D;
        msgbuf[1] =0x0A;                                                                /* SYNC_PATT_START      */
    #endif
        msgbuf[2] =0x3A;                                                                /* Data Mode and Length */
        msgbuf[3] =(Iram_DiscInfo.CD.Q.Con << 4) | Iram_DiscInfo.CD.Q.Addr;             /* Time Code Data 1     */
        msgbuf[4] =Iram_DiscInfo.CD.Q.Tno;                                              /* Time Code Data 2     */
        msgbuf[5] =Iram_DiscInfo.CD.Q.Point;                                            /* Time Code Data 3     */
        msgbuf[6] =Iram_DiscInfo.CD.Q.Minute;                                           /* Time Code Data 4     */
        msgbuf[7] =Iram_DiscInfo.CD.Q.Second;                                           /* Time Code Data 5     */
        msgbuf[8] =Iram_DiscInfo.CD.Q.Frame;                                            /* Time Code Data 6     */
        msgbuf[9] =Iram_DiscInfo.CD.Q.Zero;                                             /* Time Code Data 7     */
        msgbuf[10]=Iram_DiscInfo.CD.Q.PMinute;                                          /* Time Code Data 8     */
        msgbuf[11]=Iram_DiscInfo.CD.Q.PSecond;                                          /* Time Code Data 9     */
        msgbuf[12]=Iram_DiscInfo.CD.Q.PFrame;                                           /* Time Code Data 10    */
        SUNEXT_OUT(&msgbuf[0],13);                                                      /* Send Messages Out    */
    #elif (DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD)
        msgbuf[0]=0x0D;
    #endif
        }
}


#if (ENABLE_UART_RXD == 1)
    #if (REGISTER_MONITOR == 1)
//*************************************************************************
//   SendMsgRegMonitorCmdReady()
//
//
//*************************************************************************
void SendMsgRegMonitorCmdReady()
{
    while(READ_FIELD(HAL_APB_SSI_SFIFEMP) == 0);
    WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
    WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);                        /* SYNC_PATT_START      */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x94);          /* DATA Message N       */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
    while(READ_FIELD(HAL_APB_SSI_SFIFFUL) == 1);
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
    WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);                        /* SYNC_PATT_END        */
}
//*************************************************************************
//   SendMsgRegMonitorSampling()
//
//
//*************************************************************************
void SendMsgRegMonitorSampling()
{
    svoDWordU temp;
    BYTE      i,phase,temp1,temp2,CHXfered;

    if(fMonitoringReg != TRUE) return;

    phase = 0;
    do
    {
        switch(phase)
        {
            case 0:
                while(READ_FIELD(HAL_APB_SSI_SFIFEMP) == 0);
                WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
                WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);                        /* SYNC_PATT_START      */
                WriteSSIReg_1B(ADAPTER_ADDR_2,0xB4);          /* DATA Message N       */
                WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
                WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
                WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);          /* DATA Message N       */
                while(READ_FIELD(HAL_APB_SSI_SFIFHLF) == 0);
                WriteSSIReg_1B(ADAPTER_ADDR_2,gTotalMonitorCH*4);          /* DATA Message N       */
                WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);                        /* SYNC_PATT_END        */
                CHXfered = 0;

            case 1:
                phase = 2; //last transfer!!
                temp1 = gTotalMonitorCH - CHXfered;
                if(temp1 != 0)
                {
                    temp2 = temp1%3;
                    if((temp1/3)!=0)
                    {
                        phase = 1; //not last transfer!!
                        temp2 = 3;
                    }
                    while(READ_FIELD(HAL_APB_SSI_SFIFEMP) == 0);
                    WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
                    WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);                        /* SYNC_PATT_START      */
                    WriteSSIReg_1B(ADAPTER_ADDR_2,0xB0+temp2*4);          /* DATA Message N       */
                    for(i = 0;i < temp2;i++)
                    {
                        temp.all32  = READ_ADR_4B(gMonitorReg[CHXfered].monitor_reg_addr);
                        temp.all32 = temp.all32 <<  (31-gMonitorReg[CHXfered].start_bit);
                        temp.all32 = temp.all32 >> ((31-gMonitorReg[CHXfered].start_bit)+gMonitorReg[CHXfered].end_bit);
                        WriteSSIReg_1B(ADAPTER_ADDR_2,temp.bytes.byte3);          /* DATA Message N       */
                        WriteSSIReg_1B(ADAPTER_ADDR_2,temp.bytes.byte2);          /* DATA Message N       */
                        WriteSSIReg_1B(ADAPTER_ADDR_2,temp.bytes.byte1);          /* DATA Message N       */
                        WriteSSIReg_1B(ADAPTER_ADDR_2,temp.bytes.byte0);          /* DATA Message N       */
                        while(READ_FIELD(HAL_APB_SSI_SFIFHLF) == 0);
                        CHXfered++;
                    }
                    WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);                        /* SYNC_PATT_END        */
                }
                break;
        }
    }while(phase < 2);
}
#endif
//*************************************************************************
//   SendMsgAtapiData()
//
//
//*************************************************************************
void SendMsgAtapiData(BYTE *message, BYTE size)
{
    BYTE count, datalength;
#if (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
    BYTE msgbuf[18];
    #endif

    count = 0;
    datalength = size;
    if (datalength > 15)
    {
        datalength = 15;
    }
#if (DEBUG_PORT_OUTPUT_MODE==SSI_INTERFACE)
    while(READ_FIELD(HAL_APB_SSI_SFIFEMP) == 0);
    WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
    WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);                        /* SYNC_PATT_START      */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x90 + datalength);           /* Data Mode and Length */
    for(count = 0; count < datalength ; count++)
    {
        while(READ_FIELD(HAL_APB_SSI_SFIFFUL) == 1);
        WriteSSIReg_1B(ADAPTER_ADDR_2,message[count]);          /* DATA Message N       */
    }
    while(READ_FIELD(HAL_APB_SSI_SFIFFUL) == 1);
    WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);                        /* SYNC_PATT_END        */
#elif (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
    msgbuf[0]=0x0D;
    msgbuf[1]=0x0A;                                             /* SYNC_PATT_START      */
    msgbuf[2]=0x90 + datalength;                                /* Data Mode and Length */
    for(count = 0; count < datalength ; count++)
    {
        msgbuf[3+count] = message[count];                       /* DATA Message  N      */
    }
    UARTOUT(&msgbuf[0],3+datalength);                           /* Send Messages Out    */
    #endif
}

//*************************************************************************
//   SendMsgSpecifyXferLength()
//
//
//*************************************************************************
void SendMsgSpecifyXferLength(USHORT size)
{
    USHORT datalength;
    BYTE count;
#if (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
    BYTE msgbuf[18];
    #endif

    count = 0;
    datalength = size;
    if (datalength > (RX_BUF_SIZE - 3))
    {
        datalength = RX_BUF_SIZE - 3;
    }
#if (DEBUG_PORT_OUTPUT_MODE==SSI_INTERFACE)
    while(READ_FIELD(HAL_APB_SSI_SFIFEMP) == 0);
    WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
    WriteSSIReg_1B(ADAPTER_ADDR_2,0xAA);                        /* SYNC_PATT_START      */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x94);                        /* Data Mode and Length */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);                        /* Data Length 1        */
    WriteSSIReg_1B(ADAPTER_ADDR_2,0x00);                        /* Data Length 2        */
    WriteSSIReg_1B(ADAPTER_ADDR_2,BHIGH(datalength));           /* Data Length 3        */
    WriteSSIReg_1B(ADAPTER_ADDR_2,BLOW(datalength));            /* Data Length 4        */
    WriteSSIReg_1B(ADAPTER_ADDR_1,0x55);                        /* SYNC_PATT_END        */
#elif (DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE)
    msgbuf[0]=0x0D;
    msgbuf[1]=0x0A;                                             /* SYNC_PATT_START      */
    msgbuf[2]=0x94;                                             /* Data Mode and Length */
    msgbuf[3]=0x00;                                             /* Data Length 1        */
    msgbuf[4]=0x00;                                             /* Data Length 2        */
    msgbuf[5]=BHIGH(datalength);                                /* Data Length 3        */
    msgbuf[6]=BLOW(datalength);                                 /* Data Length 4        */
    UARTOUT(&msgbuf[0],7);                                      /* Send Messages Out    */
    #endif
}
#endif // #if (ENABLE_UART_RXD == 1)

#endif // #if (MSG_OUT_MODE == MSG_USBPORT)

//For Customer UART send message function
#if ((MSG_OUT_MODE==MSG_USBPORT)&&(DEBUG_PORT_OUTPUT_MODE==UART_INTERFACE))
//*************************************************************************
//
//  5 Hex
//
//
//*************************************************************************
void send_uart1(ULONG msg_gate,BYTE d1,BYTE d2,BYTE d3,BYTE d4,BYTE d5)
{
    ULONG msg_flag;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
        msgbuf[2]=0x55;                         /* Data Mode and Length */
        msgbuf[3]=d1;                           /* DATA Message 1       */
        msgbuf[4]=d2;                           /* DATA Message 2       */
        msgbuf[5]=d3;                           /* DATA Message 3       */
        msgbuf[6]=d4;                           /* DATA Message 4       */
        msgbuf[7]=d5;                           /* DATA Message 5       */
        UARTOUT(&msgbuf[0],8);                  /* Send Messages Out    */
    }
}

//*************************************************************************
//
//  FUN + 2 Hex
//
//
//*************************************************************************
void send_uart2(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2)
{
    ULONG msg_flag;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
        msgbuf[2]=0x85;                         /* Data Mode and Length */
        msgbuf[3]=BYTE2(func_code);             /* Function Message 1   */
        msgbuf[4]=BYTE1(func_code);             /* Function Message 2   */
        msgbuf[5]=BYTE0(func_code);             /* Function Message 3   */
        msgbuf[6]=h1;                           /* DATA Message 1       */
        msgbuf[7]=h2;                           /* DATA Message 2       */
        UARTOUT(&msgbuf[0],8);                  /* Send Messages Out    */
    }
}

//*************************************************************************
//
//  FUN + 2 BCD
//
//                           The values of these bytes are converted and displayed
//                           in BCD form by Message Monitor program SumMsg.exe
//
//*************************************************************************
void send_uart3(ULONG msg_gate,ULONG func_code,BYTE dd1,BYTE dd2)
{
    ULONG msg_flag;
    BYTE msgbuf[18];

    msg_flag = (msg_gate&0x80000000)? (hifDebugMsgFlag) : (svoDebugMsgFlag);

    if (msg_flag & msg_gate)
    {
        msgbuf[0]=0x0D;
        msgbuf[1]=0x0A;                         /* SYNC_PATT_START      */
        msgbuf[2]=0x25;                         /* Data Mode and Length */
        msgbuf[3]=BYTE2(func_code);             /* Function Message 1   */
        msgbuf[4]=BYTE1(func_code);             /* Function Message 2   */
        msgbuf[5]=BYTE0(func_code);             /* Function Message 3   */
        msgbuf[6]=dd1;                          /* BCD Data 1           */
        msgbuf[7]=dd2;                          /* BCD Data 2           */
        UARTOUT(&msgbuf[0],8);                  /* Send Messages Out    */
    }
}

#else
void send_uart1(ULONG msg_gate,BYTE d1,BYTE d2,BYTE d3,BYTE d4,BYTE d5) {}
void send_uart2(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2) {}
void send_uart3(ULONG msg_gate,ULONG func_code,BYTE dd1,BYTE dd2) {}
#endif
#pragma arm section code

#if (E_DIAGS == 1)
/*************************************************************************
* diaCmdSEEPROMAccess() (F1)  Sunext Vendor Unique Command
*
*  Read or Write to Persistent Table (Persistent Parameters)
*  This table is located either in Serial EEPROM or in the Flash area
*  If the platform has an EEPROM installed the code will update
*  locacions in the EEPROM else it will update locations
*  in the FLASH memory.
*
*
* Serial EEPROM read/write
*
   Cmd byte     Value       Description
        0       0xF1        VU command code
        1                   operation code
                0x00        Write Word
                0x01        Read Word
                0x02        Write Byte
                0x03        Read Byte
                0x04        Erase Word
                0x05        Erase Word
                0x06        Erase Byte
                0x07        Erase Byte
        2       Start Address High Byte
        3       Start Address Low Byte
        4       End Address High Byte
        5       End Address Low Byte
        6       Data Hig Byte
        7       Data Low Byte
* Returns:   for read Byte/Word, return byte or word
* Params:    None
*************************************************************************/
void diaCmdSEEPROMAccess(void)
{
USHORT  RWdataW;      // Read Write data Word
USHORT  Address;
USHORT  Data;
BYTE    Temp;
BYTE    RWdata;       // Read Write data Byte

    Temp = Iram_Cmd_block.bf[1];
    Temp &=0x07;
    switch (Temp)
    {
        case 0x00://Write Word
            OpenWritableSegmentTable(ePersistent_Talbe);
            Address = (Iram_Cmd_block.bf[2]<<8) | Iram_Cmd_block.bf[3];
            if (Address > (SIZE_PERSISTENT_TABLE -1))
            {
                BUILD_SENSE(0x05,0x24,0x00,0x60); /*  return Invalid Field In Command Packet.*/
                AutoCommandComplete();
                break;
            }
            RWdataW = (Iram_Cmd_block.bf[6]<<8) | Iram_Cmd_block.bf[7];
            SetPersistentTable2B(Address, RWdataW);
            CloseWritableSegmentTable(ePersistent_Talbe);
            AutoCommandComplete();
            break;

        case 0x01://Read Word
                //Dev2HostSel=TRUE;
            Address = (Iram_Cmd_block.bf[2]<<8) |Iram_Cmd_block.bf[3];
            if (Address > (SIZE_PERSISTENT_TABLE -1))
            {
                BUILD_SENSE(0x05,0x24,0x00,0x61); /*  return Invalid Field In Command Packet.*/
                AutoCommandComplete();
                break;
            }

            Data = GetPersistentTable2B(Address);

            /* Write readout data into dram and setup registers to transfer data to host */
            Iram_Buf_dram_addr = BUFFER_SCRATCH;/* adjust to sense key address */
            RWdata = GET_BHIGH(Data);
            WriteSDRAM(RWdata);            /* load high byte to dram */
            RWdata = GET_BLOW(Data);
            WriteSDRAM(RWdata);            /* load low byte to dram */
            SendParameterDataToHost(BUFFER_SCRATCH,2 , TRUE);                   /* wait xfr done & complete cmd */
            break;

        case 0x02://Write Byte
            OpenWritableSegmentTable(ePersistent_Talbe);
            Address = (Iram_Cmd_block.bf[2]<<8) | Iram_Cmd_block.bf[3];
            if (Address > (SIZE_PERSISTENT_TABLE -1))
                {
                   BUILD_SENSE(0x05,0x24,0x00,0x62); /*  return Invalid Field In Command Packet.*/
                   AutoCommandComplete();
                   break;
                }

            RWdata = Iram_Cmd_block.bf[7];
            SetPersistentTable1B(Address, RWdata);
            CloseWritableSegmentTable(ePersistent_Talbe);
            AutoCommandComplete();
            break;

        case 0x03://Read Byte
            Address = (Iram_Cmd_block.bf[2]<<8) |Iram_Cmd_block.bf[3];
            if (Address > (SIZE_PERSISTENT_TABLE -1))
            {
                   BUILD_SENSE(0x05,0x24,0x00,0x63); /*  return Invalid Field In Command Packet.*/
                   AutoCommandComplete();
                   break;
            }

            RWdata = GetPersistentTable1B(Address);

            /* Write readout data into dram and setup registers to transfer data to host */
            Iram_Buf_dram_addr = BUFFER_SCRATCH;/* adjust to sense key address */
            WriteSDRAM(0x00);                      /* padding */
            WriteSDRAM(RWdata);                    /* write read out data to dram */
            SendParameterDataToHost(BUFFER_SCRATCH,2 , TRUE);                   /* wait xfr done & complete cmd */
            break;


        case 0x04:
        case 0x05://Erase Word           // Not implemented at this time
             AutoCommandComplete();
             break;

        case 0x06:
        case 0x07://Erase byte           // Not implemented at this time
            AutoCommandComplete();
            break;


        default://invalid parameters
            BUILD_SENSE(0x05,0x24,0x00,0x64); /*  return Invalid Field In Command Packet.*/
            AutoCommandComplete();
            break;
    }
}

#if (MODULE_TEST == 1)
/*************************************************************************
* Used for test DVD+RW and read PIPO after TrayinInitial()
* Returns:  None
* Params:   None
*************************************************************************/
void  ModuleTest(void)
{
    ULONG clock, clock2;
    ULONG StartPSN;
    ULONG CurrentDataPSN;
    StLongU   currentID_temp;
    SHORT temp_bktobuf;
    BYTE seek_retry_count;
    BYTE ErrNum;

    if(pdbGetDiscKind() == eDVDPlusRWDisc)
    {
        StartPSN = pdbGetDVDTrackEndPSN(1) + 1;

        if(StartPSN + MOD_WRT_LEN > DVD_PLUSRW_DATAZONE_END)
            StartPSN = DVD_START_OF_DATA_ZONE;

        ErrNum = 0;

        //write zero data
        ServoSetSpeed(MOD_WRT_SPEED);
        if(DVDWriteZeroData(StartPSN, MOD_WRT_LEN, eZONE_DATA) == FALSE)
        {
            if(svoIramVar.focusDropFlag == TRUE)
                ErrNum = 0x51;
            else
                ErrNum = 0x52;
        }

        //set speed for PIPO
        if(!ErrNum)
        {
            ServoPause();
            SpeedMD = BypassSpeedMD;

            if(ServoSetSpeed(MOD_PIPO_SPEED) == FAIL)
            {
                ErrNum = 0x56;
            }
            ServoPause();
        }

        //prepare to READ PIPO
        if(!ErrNum)
        {
            PI_C = 0;
            PI_UN_C = 0;
            PO_C = 0;
            PO_UN_C = 0;
            WriteSCReg_2B(rwSTARTBK, DVD_START_BLOCK);
            ClearDataBuffer();

            dvdReadAheadStartPSN = StartPSN + MOD_PIPO_STR - 0x10; //don't make dvdReadAheadStartPSN equal to dvdLastDecodedPSN

            dvdDiscRequest.Type            = INIT_READ;
            dvdDiscRequest.TargetID.Long   = StartPSN + MOD_PIPO_STR;
            dvdDiscRequest.TargetLayer     = eLayer0;
            dvdDiscRequest.EccBlockCount   = (MOD_PIPO_END - MOD_PIPO_STR) / ONE_DVD_BLOCK;

            WAIT_PI_PO = 0;
            READ_PI_PO = 1;

            WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, 0x100);
            WriteSCReg_1B(rwECCCTRL,0);     //error correction 1 time


            /***** DVD+/-R/RW disc *****/
            ENABLE_DVD_DECODER();    /* Turn on DVD decoder */

            SetBitsSCReg_1B(rwDVDCTRL,bTIDDIS);           // disable target id

            SET_DISC_BUFFER_PTR(0);
            SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);
            CLEAR_DVD_DECODER_STATUS();
            ENABLE_DVD_DECODER_INTS();
            dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;

            seek_retry_count = 0;
            while(!ErrNum)
            {
                if (seek_retry_count > 2)
                {
                    READ_PI_PO = 0;
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    ErrNum = SEEK_FAIL;
                    break;
                }

                if (ServoSeek((dvdDiscRequest.TargetID.Long -16L), dvdDiscRequest.TargetLayer) == PASS)
                {
                    /* improve atip seek performance */
                    if (ServoSeek(dvdDiscRequest.TargetID.Long, dvdDiscRequest.TargetLayer) == PASS)
                    {
                        Svr_pause_reqire = FALSE;
                        break;
                    }
                }
                seek_retry_count++;
            }

            //monitor ID / ADIP ok
            StartTimer(&clock);
            while (!ErrNum)
            {
                if (ReadTimer(&clock) > 1000L )
                {
                    CurrentDataPSN = Last_PSN;
                    ErrNum = TIMEOUT;
                    break;
                }

                if( ((ReadSCReg_1B(rwDVDIRQS3)&bCIDI) == bCIDI) )
                {
                    if( READ_FIELD( HAL_DVDD_CIDVLD ) == 1 )
                    {
                        currentID_temp.byte.l= ReadSCReg_1B(rCURID0);
                        currentID_temp.byte.m = ReadSCReg_1B(rCURID1);
                        currentID_temp.byte.h = ReadSCReg_1B(rCURID2);
                        currentID_temp.byte.u = 00;
                        ClrBitsSCReg_1B(rwDVDIRQS3, bCIDI);
                        CurrentDataPSN = currentID_temp.Long;
                        break;
                    }
                    ClrBitsSCReg_1B(rwDVDIRQS3, bCIDI);
                }

                if( pdbGetIDIntStatus() == TRUE )
                {
                    pdbSetIDIntStatus(FALSE);
                    if(pdbGetIDOk() == TRUE)
                    {   // Monitor Wobble(ADIP/LPP) Address
                        CurrentDataPSN = pdbGetWblAddress();
                        break;
                    }
                }
                ExitProcess();
            }

            Last_PSN = CurrentDataPSN;

            dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
            CLEAR_SYNC_LOST_STATUS();
            ENABLE_DVD_SYNC_LOST_INT();

            ENABLE_DVD_BUFFERING();

            StartTimer(&clock);

            while(1)
            {
                if((dvdDiscRequest.TargetID.Long - 0x50) <= pdbGetWblAddress()
                    && pdbGetWblAddress() <= (dvdDiscRequest.TargetID.Long - 0x10))
                    break;

                if(svoIramVar.focusDropFlag == TRUE)
                {
                    ErrNum = 0x54;
                    break;
                }

                if(ReadTimer(&clock) > 2000L)
                {
                    ErrNum = TIMEOUT;
                    break;
                }

                ExitProcess();
            }

            StartTimer(&clock);
            StartTimer(&clock2);

            //check PIPO
            while (pdbGetWblAddress() <= (dvdDiscRequest.TargetID.Long + MOD_PIPO_END - MOD_PIPO_STR) && !ErrNum)
            {
                if (EjectKeyInfo.PressedValid)
                {
                    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);
                    READ_PI_PO = 0;
                    Send_timef = 0;
                    dvdStopDiscTransfer();
                    dvdResetDiscFlags();
                    Svr_pause_reqire = TRUE;

                    break;
                }

                if(ReadTimer(&clock) > 6000L)
                {
                    send_msg80(SEND_ATAPI_CMD,0x710023);
                    dvdErrorFlag.fSYNC_LOST = 1;
                    StartTimer(&clock);
                }

                if(WAIT_PI_PO == 1)
                {
                    temp_bktobuf = ReadSCReg_2B(rwBKTOBUF);
                    if(temp_bktobuf < 10)
                    {
                       WriteSCReg_1B( wBLKINC, 32);
                    }
                    if(temp_bktobuf > MAX_VALUE_OF_BLKINC)
                    {
                       temp_bktobuf = MAX_VALUE_OF_BLKINC;
                       WriteSCReg_1B( wBLKINC, temp_bktobuf);
                    }

                    WAIT_PI_PO = 0;
                }
                else if((ReadTimer(&clock2) > 200L)||dvdErrorFlag.fSYNC_LOST)
                {
                    WAIT_PI_PO = 0;

                    if ( DVD_BUFFERING() == ON )
                    {
                        dvdStopDiscTransfer();
                    }

                    if (dvdErrorFlag.fSYNC_LOST == 1)
                    {
                        dvdErrorFlag.fSYNC_LOST = 0;

                        PI_C = 32767;
                        PO_C = 182;
                        PI_UN_C = 32767;
                        PO_UN_C = 182;
                        WAIT_PI_PO = 1;

                        send_msg80(SEND_ATAPI_CMD,0x710024);

                        if(pdbGetWblAddress() >= dvdDiscRequest.TargetID.Long)
                            ErrNum = SYNC_LOST_FAIL;
                    }

                    SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);
                    dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
                    CLEAR_SYNC_LOST_STATUS();
                    ENABLE_DVD_SYNC_LOST_INT();
                    dvdResetDiscFlags();
                    CLEAR_DVD_DECODER_STATUS();
                    ENABLE_DVD_DECODER_INTS();
                    dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
                    ENABLE_DVD_BUFFERING();
                    StartTimer(&clock2);
                }

                if(pdbGetWblAddress() >= dvdDiscRequest.TargetID.Long && !ErrNum)
                {
                    if(PO_UN_C != 0 || PI_C > MOD_PI_MAX)
                    {
                        SendMsgCn(SEND_ATAPI_CMD,8,0x800311,A4B(pdbGetWblAddress()),
                        A2B(PI_C),A2B(PO_UN_C));

                        ErrNum = 0x53;
                        break;
                    }
                }

                if(svoIramVar.focusDropFlag == TRUE)
                {
                    ErrNum = 0x54;
                    break;
                }

                ExitProcess();

            } //while(1)

            WAIT_PI_PO = 0;
            READ_PI_PO = 0;
            ServoPause();
        }

        //update control data zone
        ServoSetSpeed(MOD_WRT_SPEED);
        pdbSetDVDTrackNextWritableAddress(1, StartPSN + MOD_WRT_LEN - DVD_START_OF_DATA_ZONE);
        DVDWriteCntlDataZone(1, TRUE);
    }
    else
    {
        send_msg80(SEND_ATAPI_CMD, 0x710025);
        ErrNum = 0x55;
    }

    ServoStop();

    //send_msg54(SEND_ATAPI_CMD,'E','r','r',ErrNum);
    SendMsgCnB(SEND_ATAPI_CMD,1,0x800805,B1B(ErrNum));

    //show LED
    switch(ErrNum)
    {
        case 0:
            send_msg80(SEND_ATAPI_CMD, 0x710026);
            Show_Error2LED(ModTest_LED_OFF);
            //ShowError(LED_OFF);
            break;

        case 0x51:  /* focus error during writing */
            Show_Error2LED(ModTest_LED_ST + ModTest_WRT_FOCUS_ERR);
            break;

        case 0x52:  /* other write error */
            Show_Error2LED(ModTest_LED_ST + ModTest_WRT_ERR);
            break;

        case 0x53:  /* PIPO error */
            Show_Error2LED(ModTest_LED_ST + ModTest_PIPO_ERR);
            break;

        case 0x54:  /* focus error during read PIPO */
            Show_Error2LED(ModTest_LED_ST + ModTest_PO_FOCUS_ERR);
            break;

        case 0x55:  /* start-up fail / not DVD+RW */
            Show_Error2LED(ModTest_LED_ST + ModTest_STARTUP_ERR);
            break;

        case 0x56:  /* read PIPO set speed error */
            Show_Error2LED(ModTest_LED_ST + ModTest_PIPO_SP_ERR);
            break;

        case SEEK_FAIL:
            Show_Error2LED(ModTest_LED_ST + ModTest_SEEK_ERR);
            break;

        case TIMEOUT:
            Show_Error2LED(ModTest_LED_ST + ModTest_TIMEOUT_ERR);
            break;

        case SYNC_LOST_FAIL:
            Show_Error2LED(ModTest_LED_ST + ModTest_SYNC_LOST_ERR);
            break;

        /*case GEN_FAIL:
        case ID_FAIL:
        case EDC_FAIL:
        case SEQID_FAIL:
        case TID_FAIL:
        case UNAUTH_RD_FAIL:
        case UNCOR_ECC_FAIL: */

        default:
            Show_Error2LED(ModTest_LED_ST + ModTest_OTHER_ERR);
            break;
    }
}
#endif  //(MODULE_TEST == 1)

#endif //#if E_DIAGS == 1

#if P_DIAGS == 1

// Global variables for POS (Transparent Send/Receive) commands
TPRRDs TRReturn;

// Local prototypes for POS Commands

void ExeBE_stop(void);
void ExeBE_tray_in(void);
void ExeBE_tray_out(void);
void ExeBE_read_register(void);
void ExeBE_write_register(void);
void ExeBE_read_property(void);
void ExeBE_write_property(void);
void ExeBE_enter_service_mode(void);
void ExeBE_front_command(void);
void ExeBE_focus_command(void);
void ExeBE_focus_offset_command(void);
void ExeBE_radial_command(void);
void ExeBE_sledge_command(void);
void ExeBE_tilt_command(void);
void ExeBE_ttm_command(void);
void ExeBE_set_speed(void);
void ExeBE_get_overspeed(void);
void ExeBE_jump_command(void);
void ExeBE_seek(void);
void ExeBE_recognize_disc(void);
#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
void ExeBE_adj_foc_dc_act_sens(void);
#endif
#if (FORCE_DISK_KIND == 1)
void ExeBE_configure_for_disctype(void);
#endif // (FORCE_DISK_KIND == 1)
void ExeBE_get_disctype(void);
void ExeBE_get_disctype_info(void);
void ExeBE_set_disctype(void);
void ExeBE_get_engine_status(void);
void ExeBE_get_error(void);
void ExeBE_get_selftest_result(void);
void ExeBE_get_version_number(void);
void ExeBE_pause_at(void);
void ExeBE_pause_at_on_speed(void);
void ExeBE_pause_on(void);
void ExeBE_pause_off(void);
void ExeBE_read(void);
void ExeBE_read_adip(void);
void ExeBE_read_atip(void);
void ExeBE_read_aux_bytes(void);
void ExeBE_read_header(void);
void ExeBE_measure_command(void);
void ExeBE_calibrate_record(void);
void ExeBE_modify_stop_address(void);
void ExeBE_record(void);
void ExeBE_record_pause(void);
void ExeBE_set_error_statistics_mode(void);
void ExeBE_wsg_command(void);

static void ExeBE_CoefGetValueParams(void);
static void ExeBE_CoefSetValueParams(void);
static void ExeBE_Stream(void);
static void ExeBE_Adjust(void);
static void StreamExampleSTM(halOPC_AOPCSampNum_t eNrOfSamples , USHORT Frequency , LAL_STREAM_FETypeT eFEType, LAL_STREAM_AnalogFESignalT Channel1 , LAL_STREAM_AnalogFESignalT Channel2 , LAL_STREAM_AnalogFESignalT Channel3 );
static void StreamExampleOPC(ULONG StartAddr, USHORT NrOfSteps, BYTE SectorsPerStep, halOPC_AOPCAvgSamp_t SamplesPerStep, halOPC_AOPCAvgSamp_t eAvgSamples, BYTE StartDelay, BYTE StopDelay, LAL_STREAM_FETypeT eFEType, LAL_STREAM_AnalogFESignalT Signal0 , LAL_STREAM_AnalogFESignalT Signal1 , LAL_STREAM_AnalogFESignalT Signal2 );

void PlayerCommands(void);
void TransparentReceive(void);
void ExeBE_Command(void);

// Defines for sub commands for POS diagnostics "Transparent Send Command"

#define BE_stop                       0xD103
#define BE_tray_in                    0xD101
#define BE_tray_out                   0xD100
#define BE_read_register              0xC636
#define BE_write_register             0xDA1A
#define BE_read_property              0xC6E2
#define BE_write_property             0xDAE3
#define BE_enter_service_mode         0x0002
#define BE_front_command              0x0003
#define BE_focus_command              0x0004
#define BE_focus_offset_command       0x0005
#define BE_radial_command             0x0006
#define BE_sledge_command             0x0007
#define BE_tilt_command               0x0008
#define BE_ttm_command                0x0009
#define BE_set_speed                  0x000A
#define BE_get_overspeed              0x000B
#define BE_jump_command               0x000C
#define BE_seek                       0x000D
#define BE_recognize_disc             0x000E
#define BE_configure_for_disctype     0x000F
#define BE_get_disctype               0x0010
#define BE_get_disctype_info          0x0011
#define BE_set_disctype               0x0012
#define BE_get_engine_status          0x0013
#define BE_get_error                  0x0014
#define BE_get_selftest_result        0x0015
#define BE_get_version_number         0x0016
#define BE_pause_at                   0x0017
#define BE_pause_at_on_speed          0x0018
#define BE_pause_on                   0x0019
#define BE_pause_off                  0x001A
#define BE_read                       0x001B
#define BE_read_adip                  0x001C
#define BE_read_atip                  0x001D
#define BE_read_aux_bytes             0x001E
#define BE_read_header                0x001F
#define BE_measure_command            0x0020
#define BE_calibrate_record           0x0021
#define BE_modify_stop_address        0x0022
#define BE_record                     0x0023
#define BE_record_pause               0x0024
#define BE_set_error_statistics_mode  0x0026
#define BE_wsg_command                0x0027
#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
#define BE_adj_foc_dc_act_sens        0x0028
#endif
#define BE_coef_get                   0x0029
#define BE_coef_set                   0x002A
#define BE_coef_stream                0x002B
#define BE_coef_adjust                0x002C

#define SUB_BE                        0
#define SUB_PLAYER                    1
#define SUB_SERVO_PASS_THROUGH        2
#define SUB_TRANSPARENT_RECEIVE       0x10

BYTE InServiceMode = 0;

/*******************************************************************************

    FUNCTION        diaTransparentSend()

    DESCRIPTION     "Transparent Send Command"
                    POS Diagnostic Tools for Write Strategy etc.
                    Prefix BE_ used in many places in this code stands for
                    "Basic Engine" a POS term used in their platform code.

    BYTE 0      OPCODE   0xFD
    BYTE 1      SUB COMMAND  BE_xxx
    BYTE 2      BYTE 2...n  Sub Command Parameters
         .
         .
    BYTE 6

    BYTE 7 - 11 Reserved
    ---------------------------------------------------------------------------

********************************************************************************/
void diaTransparentSend(void)
{
    BYTE    HostParameterLength;


    HostParameterLength = Iram_Cmd_block.bf[8];

    if (   ( (Iram_Cmd_block.bf[1] == SUB_PLAYER) && (HostParameterLength == 18) )
        || ( (Iram_Cmd_block.bf[1] == SUB_SERVO_PASS_THROUGH ) && (HostParameterLength == 12) )
        || ( (Iram_Cmd_block.bf[1] == SUB_BE                 ) && (HostParameterLength >  0 ) ) )
    {
        GetParameterDataFromHost(BUFFER_SCRATCH, (USHORT)Iram_Cmd_block.bf[8]);    // Get additional parameter data from host
    }
    else if ( Iram_Cmd_block.bf[1] == SUB_TRANSPARENT_RECEIVE )
    {
        // No additional parameters available
    }
    else
    {
        BUILD_SENSE(0x05,0x1A,0x00,0x59);    // PARAMETER LIST LENGTH ERROR
        AutoCommandComplete();
        return;
    }



    if      ( Iram_Cmd_block.bf[1] == SUB_BE)
        ExeBE_Command();                        // BE Commands
    else if ( Iram_Cmd_block.bf[1] == SUB_PLAYER)
        PlayerCommands();                       // Commands for Player Module.
    else if ( Iram_Cmd_block.bf[1] == SUB_SERVO_PASS_THROUGH )
        ServoPassThrough();                     // Commands for Servo   Module.
    else if ( Iram_Cmd_block.bf[1] == SUB_TRANSPARENT_RECEIVE )
        TransparentReceive();             // Get Return Data from Scratch Buffer.
    else
    {
        BUILD_SENSE(0x05,0x24,0x00,0x65);    /* Invalid Field in CDB */
        AutoCommandComplete();
    }
}

/*******************************************************************************

     Commands that can be performed by the Player Module.

********************************************************************************/
void PlayerCommands(void)
{
    BYTE RetValue;
    BYTE Temp;


    HIF.PlayerIDC    = SCSDRAM[BUFFER_SCRATCH ] & 0xF0;        // Set command IDC
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = SCSDRAM[BUFFER_SCRATCH + 1 ] & 0x3F;        // Opcode

    BUILD_SENSE(0x00,0x00,0x00,0x07);        // Clear sense code first
    for (Temp = 0; Temp < sizeof(HIF.PlayerParameters); Temp++)
    {   // Get the rest of the parameters for the Player Command.
        HIF.PlayerParameters[Temp] = SCSDRAM[BUFFER_SCRATCH + 1 + Temp];
    }

    if(WaitPlayerReady() == READY)
    {
        RetValue = PASS;
    }
    else
    {
        RetValue = FAIL;
    }
    // Player has completed Player Command.
    // Any return values are stored in HIF[]

    // Here save return values from Player Interface for a later retrieval with a Transparent Receive Command.
    ClearPosReturnData();       // Clear return data before copying new data.
    for (Temp = 0; Temp < sizeof(HIF.PlayerParameters); Temp++)
    {
        TRReturn.byte.ReturnParameters[Temp] = HIF.PlayerParameters[ Temp ];
    }
    // Update Player and Servo States in return data.
    TRReturn.byte.PlayerStateAtEndOfTransparentSend = HIF.PlayerState;
    TRReturn.byte.ServoStateAtEndOfTransparentSend  = SIF.ServoState;
    // The data in Scratch Buffer can be retrieved with a Host-issued Transparent Send Command (Get Transparent Return Data).
    if (RetValue == FAIL)
        BUILD_SENSE(0x04,0x44,0x00,0x18);    // INTERNAL TARGET FAILURE

    AutoCommandComplete();
    return;
}

/*******************************************************************************

    FUNCTION        ExeBE_Command()

    DESCRIPTION    Process BE command

    BYTE 1
    BYTE 2
         .
         .
    BYTE 6
    BYTE 7
    BYTE 8      Length (Number of platform Bytes requested.

    BYTE 9 - 11 Reserved
    ---------------------------------------------------------------------------

********************************************************************************/
void ExeBE_Command(void)
{
    // Sub Function = 00
    // Process BE command

    StWordU BE_Command;

    BE_Command.byte.h = SCSDRAM[BUFFER_SCRATCH];
    BE_Command.byte.l = SCSDRAM[(BUFFER_SCRATCH +1)];
    switch (BE_Command.Uword)
    {
        case BE_stop:
            ExeBE_stop();
            break;
        case BE_tray_in:
            ExeBE_tray_in();
            break;
        case BE_tray_out:
            ExeBE_tray_out();
            break;
        case BE_read_register:
            ExeBE_read_register();
            break;
        case BE_write_register:
            ExeBE_write_register();
            break;
        case BE_read_property:
            ExeBE_read_property();
            break;
        case BE_write_property:
            ExeBE_write_property();
            break;
        case BE_enter_service_mode:
            ExeBE_enter_service_mode();
            break;
        case BE_front_command:
            ExeBE_front_command();
            break;
        case BE_focus_command:
            ExeBE_focus_command();
            break;
        case BE_focus_offset_command:
            ExeBE_focus_offset_command();
            break;
        case BE_radial_command:
            ExeBE_radial_command();
            break;
        case BE_sledge_command:
            ExeBE_sledge_command();
            break;
        case BE_tilt_command:
            ExeBE_tilt_command();
            break;
        case BE_ttm_command:
            ExeBE_ttm_command();
            break;
        case BE_set_speed:
            ExeBE_set_speed();
            break;
        case BE_get_overspeed:
            ExeBE_get_overspeed();
            break;
        case BE_jump_command:
            ExeBE_jump_command();
            break;
        case BE_seek:
            ExeBE_seek();
            break;
        case BE_recognize_disc:
            ExeBE_recognize_disc();
            break;
#if (FORCE_DISK_KIND == 1)
        case BE_configure_for_disctype:
            ExeBE_configure_for_disctype();
            break;
#endif // (FORCE_DISK_KIND == 1)
        case BE_get_disctype:
            ExeBE_get_disctype();
            break;
        case BE_get_disctype_info:
            ExeBE_get_disctype_info();
            break;
        case BE_set_disctype:
            ExeBE_set_disctype();
            break;
        case BE_get_engine_status:
            ExeBE_get_engine_status();
            break;
        case BE_get_error:
            ExeBE_get_error();
            break;
        case BE_get_selftest_result:
            ExeBE_get_selftest_result();
            break;
        case BE_get_version_number:
            ExeBE_get_version_number();
            break;
        case BE_pause_at:
            ExeBE_pause_at();
            break;
        case BE_pause_at_on_speed:
            ExeBE_pause_at_on_speed();
            break;
        case BE_pause_on:
            ExeBE_pause_on();
            break;
        case BE_pause_off:
            ExeBE_pause_off();
            break;
        case BE_read:
            ExeBE_read();
            break;
        case BE_read_adip:
            ExeBE_read_adip();
            break;
        case BE_read_atip:
            ExeBE_read_atip();
            break;
        case BE_read_aux_bytes:
            ExeBE_read_aux_bytes();
            break;
        case BE_read_header:
            ExeBE_read_header();
            break;
        case BE_measure_command:
            ExeBE_measure_command();
            break;
        case BE_calibrate_record:
            ExeBE_calibrate_record();
            break;
        case BE_modify_stop_address:
            ExeBE_modify_stop_address();
            break;
        case BE_record:
            ExeBE_record();
            break;
        case BE_record_pause:
            ExeBE_record_pause();
            break;
        case BE_set_error_statistics_mode:
            ExeBE_set_error_statistics_mode();
            break;
        case BE_wsg_command:
            ExeBE_wsg_command();
            break;
        #if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
        case BE_adj_foc_dc_act_sens:
            ExeBE_adj_foc_dc_act_sens();
            break;
        #endif

        case BE_coef_get:
            ExeBE_CoefGetValueParams();
            break;
        case BE_coef_set:
            ExeBE_CoefSetValueParams();
            break;
#if ((PCB == 673)||(PCB == 690)||(PCB == 830))
        case BE_coef_stream:
            ExeBE_Stream();
            break;
        case BE_coef_adjust:
            ExeBE_Adjust();
            break;
#endif
        default:
            //--------------------------------------------------------
            //  INVALID SUB COMMAND
            //--------------------------------------------------------
            BUILD_SENSE(0x05,0x24,0x00,0x65);    /* Invalid Field in CDB */

        }// End sub command processing

    AutoCommandComplete();
}

/*******************************************************************************

    FUNCTION        TransparentReceive()

    DESCRIPTION     POS Diagnostic Tools for Write Strategy etc.
                    Used to return return values saved by a previous Transparent Send Command.
                    Host typically issues this command after a Transparent Send Command was
                    completed.
                    Used to retrieve either error information if Trasnsparent Send has failed
                     or to retrieve Return Information for prevously issued Transparent Send.

    CDB Bytes Transparent Send CMS
    BYTE 0 -7   N/A
    BYTE 8     Length (Number of platform Bytes requested.
    BYTE 9 - 11 N/A

********************************************************************************/
void TransparentReceive(void)
{
    BYTE Temp;

    // Update Current Player and Servo States in return data.
    TRReturn.byte.CurrentPlayerState = HIF.PlayerState;
    TRReturn.byte.CurrentServoState  = SIF.ServoState;

    // Copy Return data to Scratch Buffer for transfer to host.
    for (Temp = 0; Temp < sizeof(TRReturn); Temp++)
    {
        SCSDRAM[BUFFER_SCRATCH + Temp] = TRReturn.bytes[Temp];
    }

    SendParameterDataToHost(BUFFER_SCRATCH, sizeof(TRReturn) ,TRUE);         // Xfer data and DO post cmd complete.
}

/*******************************************************************************
     FUNCTION ClearPosReturnData()

     Clear the Return Data Array for POS
********************************************************************************/
void ClearPosReturnData(void)
{
    BYTE Temp;
    for (Temp = 0; Temp < sizeof(TRReturn); Temp++)
    {
        TRReturn.bytes[Temp] = 0;
    }

}

/*******************************************************************************

  Following are functions for the individual BE_ commands

********************************************************************************/

void ExeBE_stop(void)
{
    //  STOP switch off all servos and lasers and spins down the disc.
    //  No disc-related information is cleared.
    if (ServoStop() == FAIL)
        BUILD_SENSE(0x04,0x44,0x00,0x18);    // INTERNAL TARGET FAILURE
}

void ExeBE_tray_in(void)
{
    //  Pulls the tray in
    if (ServoTrayIn()  == FAIL)
        BUILD_SENSE(0x04,0x53,0x00,0x19);    // MEDIUM LOAD MECHANISM FAILED
}

void ExeBE_tray_out(void)
{
    //  Stop disc and push tray out.
    if (ServoTrayOut() == FAIL)
        BUILD_SENSE(0x04,0x53,0x00,0x1a);    // MEDIUM LOAD MECHANISM FAILED
}

void ExeBE_read_register(void)
{
    //  Read a register.

    StLongU RegisterAddr;
    BYTE    ByteCount;

    RegisterAddr.byte.u = SCSDRAM[BUFFER_SCRATCH + 2];   // Register address
    RegisterAddr.byte.h = SCSDRAM[BUFFER_SCRATCH + 3];   // Register address
    RegisterAddr.byte.m = SCSDRAM[BUFFER_SCRATCH + 4];   // Register address
    RegisterAddr.byte.l = SCSDRAM[BUFFER_SCRATCH + 5];   // Register address
    ByteCount           = SCSDRAM[BUFFER_SCRATCH + 6];   // Byte Count
    if (ByteCount == 1)
    {
        *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH ) = *(vBYTE *)(RegisterAddr.Long);  // Read register and put data into command return area.
    }
    else if (ByteCount == 2)
    {
        *(vUSHORT *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH) = SWAPEND_UWORD(*(vUSHORT *)(RegisterAddr.Long));   // Read register and write to Scratch area.
    }
    else
    {
        *(vULONG *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH) = SWAPEND_ULONG(*(vULONG *)(RegisterAddr.Long));   // Read register and write to Scratch area.
    }
    if (ByteCount == 4)
        SendParameterDataToHost(BUFFER_SCRATCH, 4 ,FALSE);         // Xfer data and do not post cmd complete
    else
        SendParameterDataToHost(BUFFER_SCRATCH, 2 ,FALSE);         // Xfer data and do not post cmd complete
}

void ExeBE_write_register(void)
{
    //  Write a register.
    StLongU RegisterAddr;
    StLongU WriteData;
    BYTE    ByteCount;

    RegisterAddr.byte.u = SCSDRAM[BUFFER_SCRATCH + 2];      // Register address
    RegisterAddr.byte.h = SCSDRAM[BUFFER_SCRATCH + 3];      // Register address
    RegisterAddr.byte.m = SCSDRAM[BUFFER_SCRATCH + 4];      // Register address
    RegisterAddr.byte.l = SCSDRAM[BUFFER_SCRATCH + 5];      // Register address
    ByteCount           = SCSDRAM[BUFFER_SCRATCH + 6];      // Byte Count
    WriteData.byte.u    = SCSDRAM[BUFFER_SCRATCH + 7];      // Get Write Data
    WriteData.byte.h    = SCSDRAM[BUFFER_SCRATCH + 8];      // Get Write Data
    WriteData.byte.m    = SCSDRAM[BUFFER_SCRATCH + 9];      // Get Write Data
    WriteData.byte.l    = SCSDRAM[BUFFER_SCRATCH +10];      // Get Write Data
    if (ByteCount == 1)
    {
        *(vBYTE *)(RegisterAddr.Long) = WriteData.byte.l ;  // Write to requested register.
    }
    else if (ByteCount == 2)
    {
        *(vSHORT *)(RegisterAddr.Long) =  WriteData.word.l ;  // Write to requested register.
//      WriteSCReg_2B(RegisterAddr.Long, WriteData.word.l );  // Write to requested register.
    }
    else
    {
        *(vULONG *)(RegisterAddr.Long) = WriteData.Long;  // Write to requested register.
//      WriteSCReg_4B(RegisterAddr.Long, WriteData.Long   );  // Write to requested register.
    }
}

void ExeBE_read_property(void)
{
    //  With the BE_read_property() command parts of a certain register contents
    //  (a "property" or "bit-field" as Sunext calls it) of the different devices can be read.
    //  Change POS to use Read/Write Device COMMAND (F7h)
    StWordU AddressOffset;

    AddressOffset.byte.h = SCSDRAM[BUFFER_SCRATCH + 3];      // Register address Offset ! Not Linear Address !
    AddressOffset.byte.l = SCSDRAM[BUFFER_SCRATCH + 4];      // Register address Offset ! Not Linear Address !
}

void ExeBE_write_property(void)
{
    //  With the BE_write_property() command parts of a certain register contents
    //  (a "property" or "bit-field" as Sunext calls it) of the different devices can be updated.
    //  The not selected bits of the register contents remain unchanged
    //  (however, they could change if the read-modify-write action of the register causes hardware side-effects).
    //  Basically the original contents of a register is read, the remainder is masked out, property is changed and written back.
    //  Change POS to use Read/Write Device COMMAND (F7h)
    StLongU WriteData;
    StWordU AddressOffset;

    AddressOffset.byte.h = SCSDRAM[BUFFER_SCRATCH + 3];      // Register address Offset ! Not Linear Address !
    AddressOffset.byte.l = SCSDRAM[BUFFER_SCRATCH + 4];      // Register address Offset ! Not Linear Address !
    WriteData.byte.u     = SCSDRAM[BUFFER_SCRATCH + 7];        // Get Write Data
    WriteData.byte.h     = SCSDRAM[BUFFER_SCRATCH + 8];        // Get Write Data
    WriteData.byte.m     = SCSDRAM[BUFFER_SCRATCH + 9];        // Get Write Data
    WriteData.byte.l     = SCSDRAM[BUFFER_SCRATCH +10];        // Get Write Data
}

void ExeBE_enter_service_mode(void)
{
    //  This command is not meant to be used during normal operation.
    //  It will put the engine into service mode.
    //  All monitoring and recoveries are disabled and most normal mode commands are disabled.
}

void ExeBE_front_command(void)
{
    //  1.9.1 Set_laser
    //  The Set_laser(0x01) command will switch on the CD-laser (780nm) or DVD-laser (650nm).
    //  1.9.2 Set_mode
    //  The Set_mode() command sets up the front-end registers according for a particular disctype.
    //  1.9.3 Calibrate_offset
    //  This command will remove any offset in the system as selected by the parameter arg1. (segment offset, wobble balance fen or rf offset).
    //
}

void ExeBE_focus_command(void)
{
    //  1.10.1 Set_focus
    //  The Set_focus(on) sub-command will turn on the focus loop with a time-out of 2 seconds.
    //  When the focus is not found the actuator keeps on ramping
    //  1.10.2 Set_mode
    //  The Set_mode() command sets up the servo registers of the focus loop according to the arguments.
    //  Mostly for one particular disc type.
    //  1.10.3 Set_agc
    //  The Set_agc() sub-command is used to control the focus AGC.
    //  1.10.4 Set_offset
    //  The Set_offset() sets a focus offset. sub-command will shift the lens with respect to the optimal focus point.
    //  1.10.5 Measure_reflectivity
    //  Measures the reflectivity of the disc. It returns the maximum CA (Central Aperture) value.
    //  During the reflectivity measurement, the focus catching is disabled.
    //  The lens will make one full range ramp during the reflectivity measurement; as to make sure that the maximum CA level is detected.
    //  1.10.6 Set_offset_canceling
    //  The error/diode signal inputs from a pre-amplifier can have some DC offset.
    //  This causes an offset in the focus position, which is undesirable.
    //  This commands switches this mechanism on or off:
    //  1.10.7 Get_hw_status
    //  Get focus hardware status.
    //  Possible values of the focus status are: off (0), ramping up (1), ramping down (2), in focus (3) or waiting for focus (4).
    //
}

void ExeBE_focus_offset_command(void)
{
    //  1.11.1 Optimize_focus_offset
    //  The Optimize_focus_offset() sub-command performs a focus-offset optimization at the current position and current speed.
    //  1.11.2 Get_optimize_focus_offset_result
    //  The Get_optimize_focus_offset_result() sub-command can be used to retrieve the result of the previous focus-offset optimization.
    //
}

void ExeBE_radial_command(void)
{
    //  The BE_radial_command() enables to choose one sub-command from a family of radial related commands.
    //  The first argument opcode denotes the sub-command that should be performed.
    //  Before applying these radial related commands it is advised that the following conditions are met:
    //  ?  the laser is on
    //  ?  the system is in focus
    //  ?  the turn-table-motor is spinning.
    //  The different radial related sub-commands are shown below.
    //  1.12.1 Set_radial
    //  The Set_radial(on/off) command turns on/off the radial loop..
    //  1.12.2 Set_mode
    //  The Set_mode() command sets up the servo registers for the radial loop for a particular disc type.
    //  1.12.3 Set_agc
    //  The Set_agc() sub-command is used to control the radial AGC.
    //  1.12.4 Set_calibration
    //  The Set_calibration() command is used to calibrate the radial loop and can be controlled by the argument.
    //  The radial initialization takes care of the proper scaling (offset and gains) of the radial error signals.
    //  1.12.5 Set_damping
    //  The Set_damping(on) command turns on the radial damping, which can be turned off again by the Set_damping(off) sub-command.
    //
}

void ExeBE_sledge_command(void)
{
    //  The BE_sledge_command() controls the sledge.
    //  The turn-table-motor may be on or off as well as the focus loop.
    //  The first argument opcode denotes the sub-command that should be performed,
    //  and the next arguments arg1 and arg2 serve as (optional) arguments to the sub-command. Some sub-commands return information..
    //
    //  1.13.1 Set_sledge
    //  The Set_sledge(on/off) sub-command will turn on/off the sledge control.
    //    Before the sledge can be switched on, the calibration should have been performed.
    //  1.13.2 Set_mode
    //  This sub-command adjusts the sledge for a particular disc type/speed.
    //  1.13.3 Sledge_home
    //  The Sledge_home() sub-command will send the sledge to its home position.
    //  1.13.4 Get_location
    //  The Get_location() sub-command returns the current software location counter.
    //  1.13.5 Reset_location
    //  The Reset_location() sub-command presets the software location counter to zero for the current sledge position.
    //
}

void ExeBE_tilt_command(void)
{
    //  The BE_tilt_command() enables to choose one from a family of tilt related commands.
    //   The pre-condition for this command is laser-on, in-focus and radial on-trackThe first argument opcode
    //     denotes the sub-command that should be performed, and the next arguments arg1, arg2, arg3 and arg4 serve as (optional) arguments to the sub-command.
    //     Some sub-commands return information. These data is copied into buffer.
    //  1.14.1 Set_tilt
    //  The Set_tilt() sub-command will set the tilt angle according the arguments.
    //     The command is executed immediately. The user should be aware that there is a certain time needed until the new tilt angle is settled.
    //  1.14.2 Preset_tilt_jitter_based
    //  The Preset_tilt_jitter_based() sub-command performs a tilt optimization at the current position and current speed, based on jitter measurement.
    //    Afterwards, the optimal tilt (or the current tilt value, if the tilt optimization failed) is set.
    //    The result of the optimization can be checked via the sub-command Get_preset_tilt_result().
}

void ExeBE_ttm_command(void)
{
    //  The BE_ttm_command() controls the turn-table-motor. There are no pre-conditions to meet with this command,
    //    this means that a disc might be present or not. However it could be that the motor control loop is unstable if no disc is present.
    //  Table 1: Format of the BE_ttm_command opcodes and arguments.
    //  opcode  sub-command arg action by BE    buffer[0]
    //  0x00    Turn_table_motor_off    -   Brake the ttm and if stopped switch the ttm off -
    //  0x01    Turn_table_motor_on CAVfrequency    Switches the ttm on and starts spinning at the supplied CAV frequency (unit Hz).
    //    The Hall sensor motor controlling will be used. -
    //  0x02    Get_spinning_frequency  -   Returns the current spinning speed in Hz    spinning frequency [Hz]
    //
}

void ExeBE_set_speed(void)
{
    //  This command adjusts the spinning speed of the disc. The command is allowed on all discs,
    //    but not all speeds are supported for all discs. (for instance set to CD speed CAV 80 Hz or DVD speed ZCAV1-2X).
    //    If an unsupported read speed is selected for the currently loaded disc then an error message will be issued.
}

void ExeBE_get_overspeed(void)
{
    //  This command can be used to inquire the actual overspeed factor.
    //
}

void ExeBE_jump_command(void)
{
    //  The BE_jump_command() enables to choose one sub-command from a family of jump related commands.
    //  Table 2: Format of the BE_jump_command opcodes.
    //  opcode  sub-command action by BE
    //  0x00    Jump_act_tracks Moves the laser over jumpsize amount of tracks(the conventional TrackLoss signal is used during the jump)
    //  0x01    Jump_tlls_tracks    Moves the laser over jumpsize amount of tracks(the TrackLossLessServo is used during the jump)
    //  0x02    Jump_sledge Moves the sledge over jumpsize amount of µm
    //  0x03    Jump_layer  If jumpsize=+1 the system will jump one layer upIf jumpsize=-1 the system will jump one layer down
    //
    //
}

void ExeBE_seek(void)
{
    //  This function will seek to the physical block address specified by the argument address, and, when found, go into the TRACKING player state.
    //    When the command is completed the BE will enter player state TRACKING.
}

void ExeBE_recognize_disc(void)
{
    //  BE_recognize_disc() should be the first player command from the caller to the BE after the BE_tray_in() command is issued.
    //  The BE will first try to determine the physical properties of the current disc.
    //     Physical properties are the disc diameter, disc reflection, wobble frequency and HF frequency.
    //     From these properties it can be concluded whether it is an 8/12 cm disc or no disc and what disc type.
    //  During the disc recognition process, several calibrations are performed.
    //     These calibrations usually consist of an offset elimination and/or error signals scaling.
    //  They are:
    //  ?  Front-end calibration
    //  ?  Radial calibration
    //  ?  Focus AGC
    //  ?  Radial AGC
    //  ?  Tilt position calibration
    //  ?  Focus offset calibration
    //  ?  Preset the sled position counter to 0 at position 0x96 (CD) or 0x30000 (DVD).
    //  When all these calibrations are performed the system is tracking at about 25mm from the centre of the disc with a spinning speed of 40Hz in CAV mode.

    /* ****** This command is not necessary, when Tray is in, all above action is done,
              see "Trayininitial()" ****** */
}

//  After a known disc being inserted, this command should execute the normal startup flow until TRN7.
//  For TRN 7 the state_STM_start and the state STM_measure should be done with parameter disc_type optics.
//  At the end of the execution the foc_act_DC_sens should be calculated and stored in the persistent table.
#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
void ExeBE_adj_foc_dc_act_sens(void)
{
    BYTE disc_type,ExeCmd;

    disc_type = Iram_Cmd_block.bf[2];             /* choose disc type */
    ExeCmd    = Iram_Cmd_block.bf[3];             /* execute adjust focus dc act command */

    ServoAdjFocDcActSens(disc_type,ExeCmd);
}
#endif

#if (FORCE_DISK_KIND == 1)
//  This command will tell the BE to perform all calibrations necessary for the given disc-type.
void ExeBE_configure_for_disctype(void)
{
    BYTE force_flag,blank;
    USHORT disc_kind;

    force_flag  = Iram_Cmd_block.bf[2];                             /* force disc kind switch */
    disc_kind   = (Iram_Cmd_block.bf[3]<<8)|Iram_Cmd_block.bf[4];   /* disk kind */

    ServoForceDiscKind(force_flag,disc_kind);
}
#endif //(FORCE_DISK_KIND == 1)

void ExeBE_get_disctype(void)
{
    //  This function returns 2 bytes of information about the currently loaded disc.
    BYTE DiscType=0, DiscType1= 0;
    switch (pdbServoGetDiscDiameter())
    {
        case e12cm:
            DiscType |= 0x02;
            break;

        case e8cm:
            DiscType |= 0x01;
            break;

        case eNoDisc:
            DiscType |= 0x03;
            break;

        default:
            break;
    }

    switch (pdbGetDiscKind())
    {
        case eCDROMDisc:
            DiscType |= 0x04;
            break;

        case eCDRDisc:
        case eCDRWDisc:
            DiscType |= 0x08;
            break;

        case eDVDROMDisc:
        case eDVDROMDLDisc:
            DiscType |= 0x0C;
            break;

        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRDLDisc:
            DiscType |= 0x10;
            break;

        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRDLDisc:
            DiscType |= 0x14;
            break;

        default:
            break;
    }

    if (pdbGetDiscKind() == eLSDisc)
        DiscType |= 0xC0;
    else
        DiscType |= 0x40;

    switch (pdbGetDiscKind())
    {
        case eDVDROMDisc:
            DiscType1 |= 0x01;
            break;

        case eDVDROMDLDisc:
            if (pdbGetDVDTrackPath() == DVDHD_OTP_DISC)
                DiscType1 |= 0x02;
            else
                DiscType1 |= 0x03;
            break;

        case eDVDMinusRDisc:
        case eDVDPlusRDisc:
            DiscType1 |= 0x04;
            break;

        case eDVDPlusRDLDisc:
        case eDVDMinusRDLDisc:
            DiscType1 |= 0x05;
            break;

        case eDVDMinusRWDisc:
        case eDVDPlusRWDisc:
            DiscType1 |= 0x06;
            break;

    //  case eDVDMinusRWDLDisc:
    //  case eDVDPlusRWDLDisc:
    //      DiscType1 |= 0x07;
    //

        default:
            break;
    }

    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH ) = DiscType;
    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH + 1) = DiscType1;
    SendParameterDataToHost(BUFFER_SCRATCH, 2 ,FALSE);
}

void ExeBE_get_disctype_info(void)
{
    //  This function returns 4 bytes of detailed information about the currently loaded recordable/rewritable disc.
    BYTE info0 = 0, info1 = 0, info2 = 0, info3 = 0;
    BYTE i, dcvn, eivn1, eivn2;
    if (pdbGetDiscKind()==eCDRDisc)
    {
        info0 |= 0x03;                      //force it to multispeed CD-R
        switch (Disc_SupportSpeed.Write)
        {
            case e8XCLV:
                info3 |= 0x03;
                break;
            case e12XCLV:
                info3 |= 0x07;
                break;
            case e16XCLV:
                info3 |= 0x0F;
                break;
            case e24XCLV:
                info3 |= 0x1F;
                break;
            case e32XCLV:
                info3 |= 0x3F;
                break;
            case e40XCLV:
                info3 |= 0x7F;
                break;
            case e48XCLV:
                info3 |= 0xFF;
                break;
            default:
                break;
        }
    }
    else if (pdbGetDiscKind()==eCDRWDisc)
    {
        switch (pdbGetATIPDiscSubType())
        {
            case 0:
                info0 |= 0x20;
                break;
            case 1:
                info0 |= 0x30;
                break;
            case 2:
                info0 |= 0x40;
                break;
            case 3:
                info0 |= 0x50;
                break;
            default:
                info0 |= 0xF0;
                break;
        }
    }
    else if ((pdbGetDiscKind()==eDVDPlusRDisc) || (pdbGetDiscKind()==eDVDPlusRDLDisc))
    {
        dcvn = Info.Media.DVD.Wobble.ADIPArray[0];
        if ((dcvn & 0xF0) == 0xA0)          // Single Layer
        {
            if ((dcvn & 0x0F) == 0x01)
            {
                if (Info.Media.DVD.Wobble.ADIPArray[18] == 0)
                    info1 |= 0x02;
                else
                {   eivn1 = eivn2 = 0;
                    for ( i=0; i < 5; i++)
                    {
                        if (Info.Media.DVD.Wobble.ADIPArray[64+i*32] == 1)
                            eivn1 = 1;
                        if (Info.Media.DVD.Wobble.ADIPArray[64+i*32] == 2)
                            eivn2 = 1;
                    }

                    if (eivn1 != 1 && eivn2 != 2)
                        info1 |= 0x02;
                    else if (eivn1 == 1 && eivn2 != 2)
                        info1 |= 0x03;
                    else if (eivn1 == 1 && eivn2 == 2)
                    {
                        if (Info.Media.DVD.Wobble.ADIPArray[67] <= 8)
                            info1 |= 0x04;
                        else
                            info1 |= 0x06;
                    }
                }
            }
            else
                info1 |= 0x0F;
        }
        else if ((dcvn & 0xF0) == 0xE0)     // Dual Layer
        {
            if ((dcvn & 0x0F) == 0x01)
                info1 |= 0x08;
            else
                info1 |= 0x0F;
        }
        else
            info1 |= 0x01;
    }
    else if (pdbGetDiscKind()==eDVDPlusRWDisc)
    {
        dcvn = Info.Media.DVD.Wobble.ADIPArray[0];
        if ((dcvn & 0xF0) != 0x90)
            info1 |= 0x10;
        else if ((dcvn & 0x0F) != 2)
            info1 |= 0xF0;
        else if (Info.Media.DVD.Wobble.ADIPArray[18] == 0)
                    info1 |= 0x20;
            else
            {
                eivn1 = eivn2 = 0;
                for ( i=0; i < 5; i++)
                {
                    if (Info.Media.DVD.Wobble.ADIPArray[64+i*32] == 1)
                        eivn1 = 1;
                    if (Info.Media.DVD.Wobble.ADIPArray[64+i*32] == 2)
                        eivn2 = 1;
                }

                if (eivn1 != 1)
                    info1 |= 0x20;
                else
                    info1 |= 0x30;
            }
    }
    else if ((pdbGetDiscKind()==eDVDMinusRDisc) || (pdbGetDiscKind()==eDVDMinusRDLDisc))
    {
        if (pdbGetDiscKind()==eDVDMinusRDLDisc)
            info2 |= 0x06;
        else if (Info.Media.DVD.Wobble.LPPData.DiscPhysicalCode & 0x40 == 0)
            info2 |= 0x01;
        else if (Info.Media.DVD.Wobble.LPPData.PartVerExtCode == 0x25)
            info2 |= 0x03;
        else
            info2 |= 0x0F;
    }
    else if (pdbGetDiscKind()==eDVDMinusRWDisc)
    {
        if (Info.Media.DVD.Wobble.RWLPPData.DiscPhysicalCode & 0x40 == 0)
            info2 |= 0x10;
        else if (Info.Media.DVD.Wobble.RWLPPData.PartVerExtCode == 0x12)
            info2 |= 0x20;
        else
            info2 |= 0xF0;
    }

    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH) = info0;
    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH + 1) = info1;
    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH + 2) = info2;
    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH + 3) = info3;
    SendParameterDataToHost(BUFFER_SCRATCH, 4 ,FALSE);
}

void ExeBE_set_disctype(void)
{
    //  This command alters the way the engine handles and reads data from a disc, as a CD / DVD / recordable / non-recordable.
    //    This setting will remain until the engine is reset again or when a BE_recognize_disc() or a BE_tray_out() command is received.
    //
}

void ExeBE_get_engine_status(void)
{
    //  With this function 2 bytes of BE status information can be retrieved.
    //    The first byte contains the current player state (search, tracking, pause etc) and information about the position of the tray.
    //    The next byte contains information about the currently selected spinning speed.
    //
    BYTE info0 =0, info1 = 0;

    switch (Iram_Svr_player_mode)
    {
        case TRAY_OUT_MODE:
            info0 |= 0x01;
            break;
        case STOP_MODE:
            info0 |= 0x02;
            break;
        case EXECUTING_START_UP_MODE:
            info0 |= 0x04;
            break;
        case SEARCH_ISRC_CATALOG_MODE:
            info0 |= 0x05;
            break;
        case READING_MODE:
            info0 |= 0x06;
            break;
        case PAUSING_MODE:
        case PAUSE_ON_MODE:
            info0 |= 0x07;
            break;
        case CALIBRATE_MODE:
            info0 |= 0x09;
            break;
        case EXECUTING_WRITE_MODE:
            info0 |= 0x0B;
            break;
        default:
            break;
    }
    switch (ataPowerMode)
    {
        case STANDBY_MODE:
            info0 |= 0x10;
            break;
        case IDLE_MODE:
            info0 |= 0x03;
            break;
        case SLEEP_MODE:
            info0 |= 0x0C;
            break;
        default:
            break;
    }

    if ((ipCloseSW == 1) && (Iram_Svr_player_mode != TRAY_OUT_MODE))
        info0 |= 0x20;

    info1 = pdbGetSpeed();

    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH) = info0;
    *(vBYTE *)(SCSDRAM_ADDRESS + BUFFER_SCRATCH + 1) = info1;
    SendParameterDataToHost(BUFFER_SCRATCH, 2 ,FALSE);
}

void ExeBE_get_error(void)
{
    //  This command returns error information regarding the BE.
    //    If the last player/service command ended with execution status ERROR then the first byte contains the error
    //    code of the first error detected in the system (the "reason" of execution status = ERROR).
}

void ExeBE_get_selftest_result(void)
{
    //  This command does not execute the engine self-test but just returns the result values that were established by the self-test routine just after power-on.
    //     Note that it mentions the specific chips used by the Basic Engine.
}

void ExeBE_get_version_number(void)
{
    //  This command returns the BE version number (3 bytes) and the BE software label (40 bytes).
}

void ExeBE_pause_at(void)
{
    //  On reception of this command the engine enters player state PAUSE in which the user data stream is stopped.
    //    It will position the laser spot just in front of the address denoted by the argument.
    StLongU seekAddr;
    BYTE layer;

    seekAddr.byte.u = 0;
    seekAddr.byte.h = SCSDRAM[BUFFER_SCRATCH + 2];   // Register address
    seekAddr.byte.m = SCSDRAM[BUFFER_SCRATCH + 3];   // Register address
    seekAddr.byte.l = SCSDRAM[BUFFER_SCRATCH + 4];   // Register address

    layer = eLayer0;
    if (pdbDiscLoaded() == DVD_DISC)
    {
        if (pdbGetDVDLayers() == DUAL_LAYER)
        {
            if( (pdbServoGetDiscType() == eLPPDisc)||(pdbServoGetDiscType() == eADIPDisc) )
                layer = (seekAddr.Long >= 0x800000)? eLayer1 : eLayer0;
            else
                layer = (seekAddr.Long >= pdbGetDVDStartPSN(1))? eLayer1 : eLayer0;
        }
    }

    if (ServoSeek(seekAddr.Long , layer) == FAIL )
    {
        BUILD_SENSE(0x03,0x02,0x00,0x2f);        /* MEDIUM ERROR, NO SEEK COMPLETE */
    }

    ServoPause();
}

void ExeBE_pause_at_on_speed(void)
{
    //  This command behaves exactly the same as the BE_pause_at() command, with the exception that this command also waits,
    //     before returning the READY status, until the spindle motor has reached the final target speed.
}

void ExeBE_pause_on(void)
{
    //  This command will set the BE to player state PAUSE on the current disc position, thereby switching off the user data stream.
    //    This state remains unless the pause timer expires, after which the IDLE player state is entered.

    ServoPause();
}

void ExeBE_pause_off(void)
{
    //  This command will release a previously set BE_pause_on() or BE_pause_at() command and causes the BE to continue tracking from the pause position
}

void ExeBE_read(void)
{
    //  This function will seek to the physical block address specified by the argument address. On a read-only disc the main
    //     data flow to the block decoder is enabled if the system is tracking within 1 revolution before the target address.
    //  In case of a recordable/rewritable disc, the engine uses pre-groove addresses (ATIP, ADIP or LPP) to perform the seek,
    //      because pre-groove addresses are present everywhere on the disc. After the seek is finished the BE will first check whether HF is present.
    //  This command will let the BE enter player state SEARCH. When the command is completed the BE will enter player state TRACKING.
    //  1.33.1 DVD reading
    //  With this command, the BE jumps to the requested sector address, specified by the address parameter. Layer jumps are executed as part of a seek command.
    //
    //  1.33.2 CD reading
    //  The parameter to this command shall be converted by the caller from MSF to PBA format.
    //  The BE will enable the main data stream to leave the engine if the target address is within 1 revolution of the disc.
}

void ExeBE_read_adip(void)
{
    //  This command will transfer ADIP information to the caller.
    //  The last byte of the ADIP address information specifies whether the auxiliary sector information byte is reliable.
    //    Since the address can be generated by a hardware flywheel this aux sector info byte could be unreliable.
}

void ExeBE_read_atip(void)
{
    //  This command will transfer ATIP information to the caller.
    //  Using the argument atip_mode, the user can select the kind of ATIP information he would like to have).
    //
    //  If atip_mode=ATIP_RAW the BE doesn't do any filtering on the read ATIP. It will just copy the first ATIP that is received from the disc.
    //     So it is possible that it contains the special information bytes (if tracking in the lead-in area).
    //  If atip_mode=ATIP_ADDRESS the BE will check whether the read ATIP contains time code information.
    //     It will continue reading until an absolute disc position is read. Note that the returned information could contain an address in the PCA,
    //     PMA or lead-in area (Minutes =  90BCD).
    //  If atip_mode=ATIP_SPEC_INFO the BE will check whether the read ATIP contains either special information or
    //     additional information (Mbit7Sbit7Fbit7?0b000 and Mbit7Sbit7Fbit7?0b100). It will continue reading until this information is read.
}

void ExeBE_read_aux_bytes(void)
{
    //  This command is only valid for a DVD+R/RW disc or a DVD-R/RW disc.
    //  In case a DVD+R/RW disc is loaded this command will gather ADIP auxiliary sector information bytes and transfer them to the caller.
    //   In case a DVD-R/RW is loaded it will gather LPP auxiliary sector information bytes and transfer them to the caller.
    //  1.36.1 DVD+R/RW Auxiliary ADIP information
    //  The ADIP information, as read from the disc, consists of 3 bytes for the sector address and 1 byte of auxiliary sector information.
    //  See command BE_read_adip() for a description of the format of the ADIP information.
    //  The ADIP sector address contains information about the current absolute position on the disc.
    //  The Datapath uses the auxiliary data (256 bytes) to write it as main data in the lead-in area of a DVD+R/RW disc.
    //  The Basic Engine uses this auxiliary data to calculate the optimum write strategy.
    //  On reception of this command the BE will: first seek to the lead-in zone and then gather the 256 bytes of aux sector info,
    //  by reading multiple adip frames. If an aux sector info byte seems to be unreliable
    //  (since the address can be generated by a hardware flywheel this aux sector info byte could be unreliable) it will do a re-read at a different disc position,
    //  until the complete aux frame is read without error. For gathering the 256 bytes the BE will need about 2 seconds.
    //  If the command is completed successful buffer[0] contains the Disc Category and Version Number, buffer[1]=Disc Size, buffer[2]=?refer to [DVD+RW] and/or [DVD+R].
    //  The buffer contains the physical disc information as stored in the ADIP, without any conversions.
    //  1.36.2 DVD-R/RW Auxiliary LPP information
    //  The LPP information, as read from the disc, consists of 7 bytes of information for every field ID.
    //  The 7 bytes per field ID consist of 6 bytes of disc information and 1 reserved byte, the field ID itself is not returned.
    //
    //
    //
    //  The number of returned bytes by the BE_read_aux_bytes() command depends on the speed rating of the disc, see Table 3 below.
    //
    //  Table 3: Number of returned bytes LPP information
    //  Disc speed rating   Supported field ID's    # of returned bytes ( 7 x # of field ID's)
    //  1x  1 to 5  35
    //  2x  1 to 7  49
    //  4x  1 to 13 91
    //
    //  On reception of this command the BE will first seek to the lead-in zone and then gather all auxiliary sector info,
    //   by reading multiple LPP frames. If an aux sector info byte seems to be unreliable (since the address can be generated by a
    //   hardware flywheel this aux sector info byte could be unreliable) it will do a re-read at a different disc position,
    //   until the complete aux frame is read without error. For gathering all bytes the BE will need about 2 seconds.
    //   If the command is completed successfully then buffer[0] to buffer[n] contain the disc information as stored in the LPP
    //   without any conversions (see [DVD-R_4X). The player state is always changed to TRACKING.
}

void ExeBE_read_header(void)
{
    //  This command reads header information from the current position of a DVD disc.
    //
}

void ExeBE_measure_command(void)
{
    //  This command performs various measurements as denoted by the mode parameter.
    //  When the measurement is finished, the average data (like for instance I3 pit jitter, I3 land jitter, I11 pit jitter,
    //  I11 land jitter, beta, bler etc? is returned via buffer. Besides the average, the maximum and minimum values are also collected and returned.
}

void ExeBE_calibrate_record(void)
{
    //  This command will let the BE enter player state CALIBRATE_RECORD. The player will do an OPC. When the command is completed the BE will enter player state TRACKING.
}

void ExeBE_modify_stop_address(void)
{
    //  This command adjusts the stop address, expressed as a PBA, of the recording currently in progress.
    //  It can be used to overrule the stop address which was programmed by a previously issued BE_record() command.
}

void ExeBE_record(void)
{
    //  This command will start a recording from the current start position (which is the pause address specified by the
    //  BE_record_pause() command) up to the supplied stop address.
    //  The command may only be given in the RECORD_PAUSE player state, hence shall be preceded by a call to BE_record_pause(),
    //  which brings the laser to the required starting position. If the whole interval between the programmed start and stop address has been recorded,
    //  the engine will enter the RECORD_PAUSE player state again and signal with its SW IRQ (if so programmed) the cause of the transition.
    //  During the process of recording it is possible to modify the area to record by reprogramming the stop address by means of the BE_modify_stop_address() command.
    //  1.41.1 DVD recording
    //  This command will start a recording from the current start position (which is the pause address specified by the BE_record_pause() command)
    //  up to and excluding the supplied stop address.
    //  1.41.2 CD recording
    //  This command will start a recording from the current start position (which is the pause address specified by the BE_record_pause() command)
    //  up to and excluding the supplied stop address.
}

void ExeBE_record_pause(void)
{
    //  The BE_record_pause() command will place the BE in the RECORD_PAUSE player state.
    //  This command is the only way to start a recording. One should only issue the BE_record_pause() command after the
    //  optimum recording power has been determined by means of the BE_calibrate_record().
}

void ExeBE_set_error_statistics_mode(void)
{
    //  With the BE_set_error_statistics_mode() command the error statistics logging can be enabled or disabled. Also 2 different output channels can be selected:
    //  IDE output and/or UART2 output.
    //  Error statistics output is only generated in case the statistics_mode of the mode parameter is set to 0b001.
    //  If this is enabled then all available data will be output. The Playability PC needs to do the selection and/or addition of the "interesting flags".
    //  Via "b5" of mode it can be selected to add the average jitter to the error statistics output.
    //  The calculation of the average jitter is defined as: average jitter = 15/16 * average jitter + HFZeroXJitter(7:0).
    //  The playability PC needs to convert the output value into [%].
    //
    //  Note that the Playability PC can only issue this command via the transparent IDE channel (see description of BE_transparent_send() command.
}

void ExeBE_wsg_command(void)
{
    //  With the BE_wsg_command() the write-strategy-generator of the Basic Engine can be programmed.
    //  It is intended for debugging the write strategy calculation and the verification of the laser front-end settings.
    //  It enables to choose one sub-command from a family of WSG related commands according to next table:
    //
}


void ExeBE_CoefGetValueParams(void)
{
    COEFS_t       Coef;
    COEFS_MEDIA_t CoefDiscKind;
    COEFS_SPEED_t CoefSpeed;
    ULONG         Value;

    CoefDiscKind = (COEFS_MEDIA_t) SCSDRAM[(BUFFER_SCRATCH + 2)];
    CoefSpeed    = (COEFS_SPEED_t) SCSDRAM[(BUFFER_SCRATCH + 3)];
    Coef         = (COEFS_t) ((SCSDRAM[(BUFFER_SCRATCH + 4)]<<8)|SCSDRAM[(BUFFER_SCRATCH + 5)]);
    Value        = COEF_GetValueParamsDiag(CoefDiscKind, CoefSpeed, Coef);
    SendMsgCn(SHOW_DEBUG_MSG,4,0x42000B,A4B(Value)); //Value1

    TRReturn.byte.ReturnParameters[0] = (BYTE)((Value & 0xFF000000)>>24);
    TRReturn.byte.ReturnParameters[1] = (BYTE)((Value & 0x00FF0000)>>16);
    TRReturn.byte.ReturnParameters[2] = (BYTE)((Value & 0x0000FF00)>>8);
    TRReturn.byte.ReturnParameters[3] = (BYTE)(Value & 0x000000FF);

}

void ExeBE_CoefSetValueParams(void)
{
    COEFS_t       Coef;
    COEFS_MEDIA_t CoefDiscKind;
    COEFS_SPEED_t CoefSpeed;
    ULONG         Value;

    CoefDiscKind = (COEFS_MEDIA_t) SCSDRAM[(BUFFER_SCRATCH + 2)];
    CoefSpeed    = (COEFS_SPEED_t) SCSDRAM[(BUFFER_SCRATCH + 32)];
    Coef         = (COEFS_t) ((SCSDRAM[(BUFFER_SCRATCH + 4)]<<8)|SCSDRAM[(BUFFER_SCRATCH + 5)]);
    Value        = (SCSDRAM[(BUFFER_SCRATCH + 6)]<<24)|(SCSDRAM[(BUFFER_SCRATCH + 7)]<<16)|(SCSDRAM[(BUFFER_SCRATCH + 8)]<<8)|SCSDRAM[(BUFFER_SCRATCH + 9)]; //length ? some cores 1byte, 2bytes and 4bytes (unsigned - singed)
    SendMsgCn(SHOW_DEBUG_MSG,4,0x42000C,A4B(Value)); //Value1
    COEF_SetValueParamsDiag(CoefDiscKind, CoefSpeed, Coef, Value);

}
#if ((PCB == 673)||(PCB == 690)||(PCB == 830))

void ExeBE_Stream(void)
{
    LAL_STREAM_ModeT eStreamMode;
    halOPC_AOPCAvgSamp_t eNrOfSample;
    USHORT NrOfSteps;
    ULONG Frequency;
    ULONG StartAddr;
    BYTE SectorsPerStep;
    halOPC_AOPCAvgSamp_t eSamplesPerStep;
    halOPC_AOPCSampNum_t eAvgSamples;
    BYTE StartDelay;
    BYTE StopDelay;
    LAL_STREAM_AnalogFESignalT Channel1;
    LAL_STREAM_AnalogFESignalT Channel2;
    LAL_STREAM_AnalogFESignalT Channel3;
    LAL_STREAM_FETypeT eFEType;
    BYTE    seek_retry_count;
    BOOL    time_out_occurred;
    ULONG   TimeOut_Timer;
    eSeekMode eMode;
    halWBL_LimitPllT LimitPLL;
    BYTE FreqFactor;
    SBYTE LockAid;

    eStreamMode = (LAL_STREAM_ModeT) SCSDRAM[BUFFER_SCRATCH + 2];

    send_msg5SValue(DEBUG_STREAM_MSG,"BE_stream",4,eStreamMode);

    seek_retry_count = 0;

    StartAddr       = (ULONG) ( ( (ULONG) SCSDRAM[BUFFER_SCRATCH + 3] ) << 24 | ( (ULONG) SCSDRAM[BUFFER_SCRATCH + 4] ) << 16 | ( (ULONG) SCSDRAM[BUFFER_SCRATCH + 5] ) << 8 | SCSDRAM[BUFFER_SCRATCH + 6] );
    send_msg5SValue(DEBUG_STREAM_MSG,"StartAddr",4,StartAddr);
#if 1
    if (StartAddr != 0 )
    {
       switch ( svoVar_discKindT.Bit.DiscStd )
       {
           case eDiscDVD:
         #if (DVD_RAM_READ == 1)
             if ((svoVar_discKindT.Bit.Media == eMediaROM) ||
                 ((svoVar_discKindT.Bit.Media == eMediaRAM) && (StartAddr >= SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0))))
         #else
             if (svoVar_discKindT.Bit.Media == eMediaROM)
         #endif
             { // DVD-ROM disc
                 dvdDiscRequest.Type            = INIT_READ;
                 dvdDiscRequest.TargetID.Long   = StartAddr;
                 if(StartAddr > 0x800000)
                     dvdDiscRequest.TargetLayer = eLayer1;
                 else
                     dvdDiscRequest.TargetLayer = eLayer0;
                 dvdDiscRequest.EccBlockCount   = 1;
                 (void) dvdStartDiscTransfer();
             }
             else
             {
                 WRITE_FIELD(HAL_DVDD_ENDVDD, ON); //Enable DVD Decoder before Seek//303h[0]
                 ENABLE_DVD_DECODER();    /* Turn on DVD decoder */

                 WRITE_FIELD(HAL_DVDD_TIDDIS, TRUE);        /* enable target id */

                 SET_DISC_BUFFER_PTR(0);
                 SET_DVD_BLOCKS_TO_BUFFER(MAX_VALUE_OF_BKTOBUF);
                 CLEAR_DVD_DECODER_STATUS();
                 ENABLE_DVD_DECODER_INTS();

                 while(StartAddr != 0 )
                 {
                     if (seek_retry_count > 2)
                     {
             #if 0
                         BUILD_SENSE(0x03,0x02,0x00,0x32);    // NO SEEK COMPLETE
                         AutoCommandComplete();         // Send Command Complete to host if requested by calling function
                         return(0x00);
             #else
                         send_msg5S(DEBUG_STREAM_MSG,"Seek error");
                         break;
             #endif
                     }
                     send_msg5S(DEBUG_STREAM_MSG,"Seek");
                     if(StartAddr > 0x800000)
                         eMode = eLayer1;
                     else
                         eMode = eLayer0;
                     if (ServoSeek(StartAddr - 100L,eMode)==PASS)
                     {
                         send_msg5S(DEBUG_STREAM_MSG,"Seek RDY");

                         ClearAddressRead();

                         time_out_occurred = FALSE;
                         StartTimer(&TimeOut_Timer);
                         {
                             /* wait spindle stable 0~250ms in CLV Mode*/
                             while (READ_FIELD(HAL_SRV_FGLOCK) == 0)
                                 {
                                     if (ReadTimer(&TimeOut_Timer) > 2000)
                                     {
                                         send_msg5S(DEBUG_STREAM_MSG,"No FGLock");
                                         time_out_occurred = TRUE;
                                         break;
                                     }
                                 }

                              while (HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz()) == FALSE) //old WBLCK flag not used anymore
                                 {
                                     if (ReadTimer(&TimeOut_Timer) > 2000)
                                     {
                                         send_msg5S(DEBUG_STREAM_MSG,"No WBLCK");
                                         time_out_occurred = TRUE;
                                         break;
                                     }
                                 }
                         }
                         while ((CheckHeaderID() != TRUE) && !time_out_occurred)
                         {
                             if (ReadTimer(&TimeOut_Timer) > 2000)
                             {
                                 send_msg5S(DEBUG_STREAM_MSG,"No Address");
                                 time_out_occurred = TRUE;
                                 break;
                             }
                         }
                         send_msg5SValue(DEBUG_STREAM_MSG, "SEEK at:", 4, svoIramVar.currentID.SectorNum.all32);
                         if ((svoIramVar.currentID.SectorNum.all32 < StartAddr - 60L) && !time_out_occurred)
                             break;
                     }
                     seek_retry_count++;
                 }
             }
             break;
          case eDiscBD:
             break;
       }
    }
#if (DVD_RAM_READ == 1)
    if ((svoVar_discKindT.Bit.Media == eMediaROM) ||
        ((svoVar_discKindT.Bit.Media == eMediaRAM) && (StartAddr >= SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0))))
#else
    if (svoVar_discKindT.Bit.Media == eMediaROM)
#endif
    {
       StartAddr = 0; // no wobble-addresses available so encoder and target-match won't work
       WRITE_FIELD(HAL_GLOBAL_DISCFMT,halGLOBAL_DiscFmt_DVD_mRW);
       HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);        /*hold Wobble PLL */
       FreqFactor = 100;
       LockAid = 0x00;
       LimitPLL.Upper = 100;
       LimitPLL.Lower = 100;
       HAL_WOBBLE_SetWobbleFreq((svoSpeedVar.SpdOverspd + 5) / 10, FreqFactor, LockAid, HAL_PLL_GetWblClkKHz(), &LimitPLL);
       send_msg5SValue(DEBUG_STREAM_MSG,"WblClk",1, HAL_PLL_GetWblClkKHz());
       WRITE_FIELD(HAL_AFE_AGCHOLD, ON);
       WRITE_FIELD(HAL_DFE_AVFLTBW, 0x03);
    }
#endif
    if ( eStreamMode == LAL_STREAM_MODE_WOBBLE )
    {
       NrOfSteps       = (USHORT) ( ( (USHORT) SCSDRAM[BUFFER_SCRATCH + 7] ) << 8 | SCSDRAM[BUFFER_SCRATCH + 8] );
       SectorsPerStep  = (BYTE) SCSDRAM[BUFFER_SCRATCH + 9];
       eSamplesPerStep = (halOPC_AOPCSampNum_t) SCSDRAM[BUFFER_SCRATCH + 10];
       eAvgSamples     = (halOPC_AOPCAvgSamp_t) SCSDRAM[BUFFER_SCRATCH + 11];
       StartDelay      = (BYTE) SCSDRAM[BUFFER_SCRATCH + 12];
       StopDelay       = (BYTE) SCSDRAM[BUFFER_SCRATCH + 13];
       eFEType         = (LAL_STREAM_FETypeT) SCSDRAM[BUFFER_SCRATCH + 14];
       send_msg5SValue(DEBUG_STREAM_MSG,"StartAddr",4,StartAddr);
       send_msg5SValue(DEBUG_STREAM_MSG,"NrOfSteps",2,NrOfSteps);
       send_msg5SValue(DEBUG_STREAM_MSG,"SectorsPerStep",1,SectorsPerStep);
       send_msg5SValue(DEBUG_STREAM_MSG,"SamplesPerStep",1,eSamplesPerStep);
       send_msg5SValue(DEBUG_STREAM_MSG,"AvgSamples",1,eAvgSamples);
       send_msg5SValue(DEBUG_STREAM_MSG,"StartDelay",1,StartDelay);
       send_msg5SValue(DEBUG_STREAM_MSG,"StopDelay",1,StopDelay);
       send_msg5SValue(DEBUG_STREAM_MSG,"eFEType",1,eFEType);
       if ( eFEType == LAL_STREAM_FE_DIGITAL )
       {
         StreamExampleOPC(StartAddr, NrOfSteps, SectorsPerStep, eSamplesPerStep, eAvgSamples, StartDelay, StopDelay, eFEType, 0 , 0 , 0 );
       }
       else
       {
         Channel1      = (LAL_STREAM_AnalogFESignalT) SCSDRAM[BUFFER_SCRATCH + 14];
         Channel2      = (LAL_STREAM_AnalogFESignalT) SCSDRAM[BUFFER_SCRATCH + 15];
         Channel3      = (LAL_STREAM_AnalogFESignalT) SCSDRAM[BUFFER_SCRATCH + 16];
         send_msg5SValue(DEBUG_STREAM_MSG,"Channel1",2,Channel1);
         send_msg5SValue(DEBUG_STREAM_MSG,"Channel2",2,Channel2);
         send_msg5SValue(DEBUG_STREAM_MSG,"Channel3",2,Channel3);
         StreamExampleOPC(StartAddr, NrOfSteps, SectorsPerStep, eSamplesPerStep, eAvgSamples, StartDelay, StopDelay, eFEType, Channel1 , Channel2 , Channel3 );
       }
    }
    else
    {
       eNrOfSample = (halOPC_AOPCSampNum_t) SCSDRAM[BUFFER_SCRATCH + 7];
       Frequency   = (ULONG) ( ( (ULONG) SCSDRAM[BUFFER_SCRATCH + 8] ) << 24 | ( (ULONG) SCSDRAM[BUFFER_SCRATCH + 9] ) << 16 | ( (ULONG) SCSDRAM[BUFFER_SCRATCH + 10] ) << 8 | SCSDRAM[BUFFER_SCRATCH + 11] );
       eFEType         = (LAL_STREAM_FETypeT) SCSDRAM[BUFFER_SCRATCH + 12];
       if ( eFEType == LAL_STREAM_FE_DIGITAL )
       {
          DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED); // Not possible in STM mode to get DFE registers (TR/TD/TC pulses required to generate A1/A2/CALF/JITTER values)
       }
       else
       {
          Channel1    = (LAL_STREAM_AnalogFESignalT) SCSDRAM[BUFFER_SCRATCH + 13];
          Channel2    = (LAL_STREAM_AnalogFESignalT) SCSDRAM[BUFFER_SCRATCH + 14];
          Channel3    = (LAL_STREAM_AnalogFESignalT) SCSDRAM[BUFFER_SCRATCH + 15];
          send_msg5SValue(DEBUG_STREAM_MSG,"eNrOfSample",4,eNrOfSample);
          send_msg5SValue(DEBUG_STREAM_MSG,"Frequency",4,Frequency);
          send_msg5SValue(DEBUG_STREAM_MSG,"Channel1",4,Channel1);
          send_msg5SValue(DEBUG_STREAM_MSG,"Channel2",4,Channel2);
          send_msg5SValue(DEBUG_STREAM_MSG,"Channel3",4,Channel3);
          StreamExampleSTM( eNrOfSample , Frequency , eFEType, Channel1 , Channel2 , Channel3 );
       }
    }
}

static void adjust_callback_func (void)
{
   send_msg5S(1,"adjust_callback_func");
}

#define  TEST_BUF_SIZE  20
BYTE           test_buf[TEST_BUF_SIZE];
USHORT         test_nrof_bytes_copied;

void ExeBE_Adjust()
{
   discKindT tempDiscKind;

   tempDiscKind.fDiscKind = eDVDROMDisc;

   ADJUST_Init();
   ADJUST_Start (tempDiscKind,
                 SVO_CAL_SPEED_DVD,
                 adjust_callback_func,
                 test_buf,
                 TEST_BUF_SIZE,
                 &test_nrof_bytes_copied,
                 ADJUST_ITEM_SPHER_ABER,
                 ADJUST_ITEM_FOCUS_OFFSET,
                 ADJUST_COST_STREAM_SAMPLED_JITTER,
                 0, //BYTE            new_config,
                 0  //ULONG           new_start
                );
   ADJUST_Exec(); //required to update the status

   while ( ADJUST_GetStatus() == BUSY )
   {
      ADJUST_Exec();
      ExitProcess();
   }
   send_msg5S(1,"ExeBE_Adjust finished");

}

/* TBD? NOTE: This code is an example how to use the LAL_stream module; it should eventually be removed */
#include ".\AL\LAL_stream.h"
#define NR_OF_SIGNALS   3

static void StreamExampleSTM( halOPC_AOPCSampNum_t eNrOfSamples , USHORT Frequency , LAL_STREAM_FETypeT eFEType, LAL_STREAM_AnalogFESignalT Channel1 , LAL_STREAM_AnalogFESignalT Channel2 , LAL_STREAM_AnalogFESignalT Channel3 )
{
    LAL_STREAM_InputSignalsDescriptorT InputSignalsDescriptor;
    LAL_STREAM_SamplingDescriptorT SamplingDescriptor;
    LAL_STREAM_ErrorT StreamError;
    USHORT i;
    USHORT Channel[NR_OF_SIGNALS];
    BYTE PowerStep;
    LAL_STREAM_StatusT status;
    USHORT NrOfSamples;

    send_msg5S(DEBUG_STREAM_MSG,"StreamSTM");

    /* init */
    LAL_STREAM_Init();
    /* configure */
    if ( eFEType == LAL_STREAM_FE_DIGITAL )
    {
       // Digital beta-config
       //WRITE_FIELD(HAL_DFE_BYPLPF,1);   // Set in bypass
       WRITE_FIELD(HAL_DFE_AMAXLPFBW,0x01);
       WRITE_FIELD(HAL_DFE_AMINLPFBW,0x01);

       WRITE_FIELD(HAL_DFE_BETAMSEN,1); // Enable beta-measurement
       LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FE_DIGITAL, &InputSignalsDescriptor); //InputSignalsDescriptor is not required for DIGITAL
    }
    else
    {
       InputSignalsDescriptor.Afe.NrOfSignals = NR_OF_SIGNALS;
       InputSignalsDescriptor.Afe.Signal[0] = Channel1;
       InputSignalsDescriptor.Afe.Signal[1] = Channel2;
       InputSignalsDescriptor.Afe.Signal[2] = Channel3;
       LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FE_ANALOG, &InputSignalsDescriptor);
    }
    SamplingDescriptor.timer.eNrOfSamples = eNrOfSamples;
    SamplingDescriptor.timer.Frequency = Frequency;
    LAL_STREAM_ConfigureSampling(LAL_STREAM_MODE_TIMER, &SamplingDescriptor);
//    LAL_STREAM_ConfigureMode(LAL_STREAM_MODE_TIMER, 0);

    LAL_STREAM_ConfigMioDebug(LAL_STREAM_MODE_TIMER);

#if 0
   Set_MIO3_TO_GPIO();
   GIO3(1);
   DelaymS(1);
   GIO3(0);
   DelaymS(1);
   GIO3(1);
   DelaymS(1);
   GIO3(0);
   DelaymS(1);
   GIO3(1);
#endif

  // OPC-MIO Debug --> HACK

    LAL_STREAM_Start();

    SetTimeout(2, SEC_2);

    status = LAL_STREAM_STATUS_BUSY; //LAL_STREAM_GetStatus();

    while (status == LAL_STREAM_STATUS_BUSY)
    {
        status = LAL_STREAM_GetStatus();

        switch (status)
        {
           case LAL_STREAM_STATUS_BUSY:
#if 1
               if(timeout_chk(2) == eTimerTimeout)     /* time out check */
               {
                  /* abnormal operation */
                  LAL_STREAM_Abort();
                  send_msg5S(DEBUG_STREAM_MSG,"Stream timeout");
               }
               else
#endif
               {
                  send_msg5S(DEBUG_STREAM_MSG,"Streaming...");
                  send_msg5SValue(DEBUG_STREAM_MSG,"OPC_WBLSTM",4,READ_FIELD(HAL_OPC_WBLSTM));
                  ExitProcess();
               }
               break;

           case LAL_STREAM_STATUS_READY:
#if 0
               GIO3(0);
#endif

               NrOfSamples = LAL_STREAM_GetNrOfSamples();
               send_msg5SValue (DEBUG_STREAM_MSG,"Stream data",2, NrOfSamples);
               /* do something with the data here.. */
               for (i= 0; i < NrOfSamples; i++)
               {
                   Channel[0] = LAL_STREAM_DATA_CHANNEL1(i);
                   Channel[1] = LAL_STREAM_DATA_CHANNEL2(i);
                   Channel[2] = LAL_STREAM_DATA_CHANNEL3(i);
                   PowerStep = LAL_STREAM_DATA_SAMPLE(i);
                   send_msg5SValue(DEBUG_STREAM_MSG,"SAMPLE",2,PowerStep);
                   send_msg5SValue(DEBUG_STREAM_MSG,"CH 1",2,Channel[0]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"CH 2",2,Channel[1]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"CH 3",2,Channel[2]);
#if 0
                   send_msg5SValue(DEBUG_STREAM_MSG,"data addr",4,(ULONG)&(HAL_OPC_pSRAM[i].data[0]));
                   send_msg5SValue(DEBUG_STREAM_MSG,"data0",1,HAL_OPC_pSRAM[i].data[0]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data1",1,HAL_OPC_pSRAM[i].data[1]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data2",1,HAL_OPC_pSRAM[i].data[2]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data3",1,HAL_OPC_pSRAM[i].data[3]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data4",1,HAL_OPC_pSRAM[i].data[4]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data5",1,HAL_OPC_pSRAM[i].data[5]);
#endif
               }
               LAL_STREAM_Stop();
               break;

           case LAL_STREAM_STATUS_ERROR:
               StreamError = LAL_STREAM_GetError();
               send_msg5SValue(DEBUG_STREAM_MSG,"Stream error",2,StreamError);
               /* do something with the error here.. */
               LAL_STREAM_Stop();
               break;

           case LAL_STREAM_STATUS_IDLE:
           default:
               send_msg5S(DEBUG_STREAM_MSG,"Stream oeps...");
               ExitProcess();
               break;
        }
    }
    SetTimeout(2, TIMER_STOP);

#if 0
   DelaymS(1);
   GIO3(1);
   DelaymS(1);
   GIO3(0);
#endif
}

static void z_SetDVDEncStreamStart(ULONG cmd_test_start_pba)
{
#if 0
    ULONG   clock;
    SHORT   WrPattern;
#endif

    cmd_test_start_pba &= 0xfffffff0L;

    //SV: normally this line can be skipped
    HAL_WOBBLE_SelectWPLLMode(eNormalMode);

#if 0
    WRITE_REG(HAL_WSREG_MAP->PRINTS,0);
    WRITE_REG(HAL_WSREG_MAP->CUSINTS ,0x00);    //clear P&R interrupts status flags

    READ_MOD_WRITE(HAL_WSREG_MAP->WSMISC1,HAL_WS_WGINV|HAL_WS_WGEN|HAL_WS_WGLVL|HAL_WS_TMODE, 0);

    WRITE_REG(HAL_WSREG_MAP->PRINTEN, HAL_WS_WGUPEN|HAL_WS_WGDNEN);
    WRITE_REG(HAL_WSREG_MAP->CUSINTEN,0x00);

    WrPattern=0x00FF&(SHORT)RandomNumberGenerator(0);
    WrPattern=WrPattern|((SHORT)((RandomNumberGenerator(1))<<4)&0xFF00);
#endif

#if 0 //required !!! ==> how to bypass ???
    // Pattern Filled the user data blocks
//    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, OFF);      /* turn on Copy clock */
//    WRITE_FIELD(HAL_CPYCMP_CPYTGT, 0x0000);     /* page, target register set */
//    WRITE_FIELD(HAL_CPYCMP_CPYTOF, 0x0000);     /* arget offset register set*/
//    WRITE_FIELD(HAL_CPYCMP_CPYPCNT, 0x021F);    /* page count set*/
//    WRITE_FIELD(HAL_CPYCMP_CPYWCNT, 0x0400);    /* set word count */
//    WRITE_FIELD(HAL_CPYCMP_CPYPAT, WrPattern);  /* set pattern w. page number */

//    WRITE_FIELD(HAL_CPYCMP_PFILSTRT, ON);       /* start pattern fill, will wrap around */
//    while(READ_FIELD(HAL_CPYCMP_PFILSTRT));

//    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, ON);       /* turn off Copy clock */
#endif

    WRITE_REG(HAL_DVDDREG_MAP->MEDCTRL, 0x00); /* Turn Off DVD Decoder */

#if 0
    WRITE_FIELD(HAL_DVDE_ENCRST, ON);
    while(READ_FIELD(HAL_DVDE_ENCRST));

    WRITE_FIELD(HAL_DVDE_ENDVDE,0);//WRITE_REG(HAL_DVDEREG_MAP->WRTCTRL, 0x00);
    WRITE_FIELD(HAL_DVDE_WCMDST,0);
    WRITE_FIELD(HAL_DVDE_WGATEN,0);//WRITE_REG(HAL_DVDEREG_MAP->WGCTRL, 0x00);       // Force to write from next valid ID
    WRITE_FIELD(HAL_DVDE_TGTMBYP,0);
#endif

#if 0
    ENABLE_WSINT();
#endif

    // Enable DVD Encoder
#if 0
    WRITE_FIELD(HAL_DVDE_DEBUFSPG, 0x0000);
    WRITE_FIELD(HAL_DVDE_DEBUFEPG, 0x021F);

    WRITE_FIELD(HAL_DVDE_DSASADR, 0x14F800);

    WRITE_FIELD(HAL_DVDE_DSBSADR, 0x159000);

    WRITE_FIELD(HAL_DVDE_WSB1SAD, 0x163800);

    WRITE_FIELD(HAL_DVDE_WSB1EAD, 0x163810);

    WRITE_FIELD(HAL_DVDE_WSB2SAD, 0x165800);

    WRITE_FIELD(HAL_DVDE_WSB2EAD, 0x165810);
#endif
    if(pdbServoGetDiscType() == eLPPDisc)
    {
        WRITE_FIELD(HAL_DVDE_DVDETYP, halDVDE_DVDminus);        // Select DVD-R/RW
    }
    else
    {
        {
            WRITE_FIELD(HAL_DVDE_DVDETYP, halDVDE_DVDplus);  // Select DVD+R/RW
        }
    }
#if 0
    WRITE_REG(HAL_DVDEREG_MAP->DVDEIRQS0, 0x00);
    WRITE_REG(HAL_DVDEREG_MAP->DVDEIRQS1, 0x00);
    WRITE_REG(HAL_DVDEREG_MAP->DVDEIRQS2, 0x00);

    WRITE_REG(HAL_DVDEREG_MAP->HEADERL, cmd_test_start_pba);

    WRITE_FIELD(HAL_DVDE_CPRMAI0, 0x00);
    WRITE_FIELD(HAL_DVDE_CPRMAI1, 0x01);
    WRITE_FIELD(HAL_DVDE_CPRMAI2, 0x00);
    WRITE_FIELD(HAL_DVDE_CPRMAI3, 0x02);
    WRITE_FIELD(HAL_DVDE_CPRMAI4, 0x00);
    WRITE_FIELD(HAL_DVDE_CPRMAI5, 0x03);

    WRITE_FIELD(HAL_DVDE_DEBUFPG, 0x0000);
#endif

#if 0
    WRITE_REG(HAL_DVDEREG_MAP->WMCTRL, 0x00);      //Write Mode
    WRITE_REG(HAL_DVDEREG_MAP->WACTRL, 0x00);
#endif

    WRITE_FIELD(HAL_DVDE_STRADRSEL, 0); //Start address is in TGTADDR
    WRITE_FIELD(HAL_DVDE_TGTHDR, cmd_test_start_pba); //required

#if 1 //required
    WRITE_FIELD(HAL_DVDE_STPADRSEL, OFF);    // Stop address controlled by WRCNT
    WRITE_FIELD(HAL_DVDE_WRCNT,1);//WRITE_FIELD(HAL_DVDE_WRCNT, cmd_test_write_length>>4);
#endif

#if 0
    WRITE_REG(HAL_DVDEREG_MAP->ADDRCTRL, 0x00);          //Stop when WRCNT is 0
#endif

#if 0
    WRITE_FIELD(HAL_DVDE_STPADDR, (cmd_test_start_pba+cmd_test_write_length));     // Stop Location Address Registers. Stop Write Address.
    WRITE_ADR_1B(HAL_DVDE_STPADDR_ADR, 0x00);
    WriteSCReg_1B(HAL_DVDE_STPFRAME, 0x00);
    WRITE_FIELD(HAL_DVDE_STPBIT, 0x120);

    WRITE_REG(HAL_DVDEREG_MAP->STRTATR, 0x00);
    WRITE_REG(HAL_DVDEREG_MAP->STPATR0, 0x00);

    WRITE_REG(HAL_DVDEREG_MAP->STPATR1, 0x00);
#endif

#if 0
    WRITE_REG(HAL_DVDEREG_MAP->WRTCTRL, HAL_DVDE_ENDVDE | HAL_DVDE_WCMDST);  // Enable DVD Encoder Block, Write Command Start.
#else
    //WRITE_REG(HAL_DVDEREG_MAP->WRTCTRL, HAL_DVDE_ENDVDE | HAL_DVDE_WCMDST);  // Enable DVD Encoder Block, Write Command Start.
    WRITE_FIELD(HAL_DVDE_ENDVDE, 1);
//    WRITE_FIELD(HAL_DVDE_WCMDST, 1);
#endif

#if 0
   // while(READ_FIELD(HAL_DVDE_BLKENCI) != 0x01);
    WRITE_REG(HAL_DVDEREG_MAP->DVDEIRQS0, 0x00);
#endif

#if 1
    WRITE_FIELD(HAL_DVDE_WGATEN, ON);//Set_DVD_WGATEN();
#endif

#if 0
    Disp_Write_Parameters();
#endif

#if 0
    StartTimer(&clock);
#endif

#if 0
    WRITE_FIELD(HAL_WS_TRERREN, ON);    //Enable custom interrupt
    CLEAR_INT_FIELD(HAL_WS_TRERRS , 0 );
#endif

#if 0
    send_msg5S(DEBUG_STREAM_MSG,"Waiting WGUP");
    while (READ_FIELD(HAL_DVDE_SYSWRT) == OFF)
    {
       send_msg5L(DEBUG_STREAM_MSG,pdbGetWblAddress());
       if (ReadTimer(&clock) > 6000L)  /*1s*/
       {
          send_msg5S(DEBUG_STREAM_MSG,"Timeout WGUP");
          break;
       }
       DelaymS(20);//ExitProcess();
    }
#endif
#if 0
    WRITE_FIELD(HAL_DVDE_ENDVDE, 0);  // Disable DVD Encoder Block, otherwise it will overwrite sram.
#endif
}
static void z_SetBDEncStreamStart(ULONG cmd_test_start_pba)
{
// TBD? No Code!
}

static void StreamExampleOPC(ULONG StartAddr, USHORT NrOfSteps, BYTE SectorsPerStep, halOPC_AOPCSampNum_t eSamplesPerStep, halOPC_AOPCAvgSamp_t eAvgSamples, BYTE StartDelay, BYTE StopDelay, LAL_STREAM_FETypeT eFEType, LAL_STREAM_AnalogFESignalT Channel1 , LAL_STREAM_AnalogFESignalT Channel2 , LAL_STREAM_AnalogFESignalT Channel3 )
{
    LAL_STREAM_InputSignalsDescriptorT InputSignalsDescriptor;
    LAL_STREAM_SamplingDescriptorT SamplingDescriptor;
    LAL_STREAM_ErrorT StreamError;
    USHORT i;
    LAL_STREAM_StatusT status;
    USHORT Beta;
    USHORT DataCalf;
    USHORT DataA1;
    USHORT DataA2;
    USHORT DataJitter;
    USHORT PowerStep;
    USHORT NrOfSamples;
    ULONG OverspeedX100;

    send_msg5S(DEBUG_STREAM_MSG,"StreamOPC");

    OverspeedX100 = svoSpeedVar.SpdOverspd;

    /* init */
    LAL_STREAM_Init();
    /* configure */
    if ( eFEType == LAL_STREAM_FE_DIGITAL )
    {
       // Digital beta-config
       LAL_DFE_SetDigiBetaMode(OverspeedX100);
       LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FE_DIGITAL, &InputSignalsDescriptor); //InputSignalsDescriptor is not required for DIGITAL
    }
    else
    {
       InputSignalsDescriptor.Afe.NrOfSignals = NR_OF_SIGNALS;
       InputSignalsDescriptor.Afe.Signal[0] = Channel1;
       InputSignalsDescriptor.Afe.Signal[1] = Channel2;
       InputSignalsDescriptor.Afe.Signal[2] = Channel3;
       LAL_STREAM_ConfigureInputSignals(LAL_STREAM_FE_ANALOG, &InputSignalsDescriptor);
    }
    SamplingDescriptor.wobble.OverspeedX100 = OverspeedX100;
    SamplingDescriptor.wobble.NrOfSteps = NrOfSteps;
    SamplingDescriptor.wobble.StartAddr = StartAddr;
    SamplingDescriptor.wobble.SectorsPerStep = SectorsPerStep;
    SamplingDescriptor.wobble.eSamplesPerStep = eSamplesPerStep;
    SamplingDescriptor.wobble.StartDelay = StartDelay;
    SamplingDescriptor.wobble.StopDelay  = StopDelay;
    SamplingDescriptor.wobble.eAvgSamples = eAvgSamples;
    LAL_STREAM_ConfigureSampling(LAL_STREAM_MODE_WOBBLE, &SamplingDescriptor);

    LAL_STREAM_ConfigMioDebug(LAL_STREAM_MODE_WOBBLE);

#if 0
   Set_MIO3_TO_GPIO();
   GIO3(1);
   DelaymS(1);
   GIO3(0);
   DelaymS(1);
   GIO3(1);
   DelaymS(1);
   GIO3(0);
   DelaymS(1);
   GIO3(1);
#endif

    //First program STREAM-start then the start-address (don't know why this dependency exists)
    LAL_STREAM_Start();

    if ( StartAddr == 0 )
    {
      LAL_STREAM_ConfigureDebugMode(TRUE);
    }
    else
    {
      LAL_STREAM_ConfigureDebugMode(FALSE);
      switch ( svoVar_discKindT.Bit.DiscStd )
      {
          case eDiscDVD:
             z_SetDVDEncStreamStart(StartAddr);
             break;
          case eDiscBD:
             z_SetBDEncStreamStart(StartAddr);
             break;
      }
   }

    SetTimeout(2, SEC_6);

    status = LAL_STREAM_STATUS_BUSY; //LAL_STREAM_GetStatus();

    while (status == LAL_STREAM_STATUS_BUSY)
    {
        status = LAL_STREAM_GetStatus();

        switch (status)
        {
           case LAL_STREAM_STATUS_BUSY:
#if 1
               if(timeout_chk(2) == eTimerTimeout)     /* time out check */
               {
                  /* abnormal operation */
                  LAL_STREAM_Abort();
                  send_msg5S(DEBUG_STREAM_MSG,"Stream timeout");
               }
               else
#endif
               {
                  send_msg5L(DEBUG_STREAM_MSG,pdbGetWblAddress());
                  send_msg5S(DEBUG_STREAM_MSG,"Streaming...");
                  send_msg5SValue(DEBUG_STREAM_MSG,"OPC_WBLSTM",4,READ_FIELD(HAL_OPC_WBLSTM));
                  ExitProcess();
               }
               break;

           case LAL_STREAM_STATUS_READY:
#if 0
               GIO3(0);
#endif

               NrOfSamples = LAL_STREAM_GetNrOfSamples();
               send_msg5SValue (1,"Stream data",2, NrOfSamples);
               /* do something with the data here.. */
               for (i= 0; i < NrOfSamples; i++)
               {
                   DataCalf = LAL_STREAM_DATA_CALF(i);
                   DataA1 = LAL_STREAM_DATA_A1(i);
                   DataA2 = LAL_STREAM_DATA_A2(i);
                   DataJitter = LAL_STREAM_DATA_JITTER(i);
                   PowerStep = LAL_STREAM_DATA_POWERSTEP(i);
                   send_msg5SValue(DEBUG_STREAM_MSG,"PowerStep",1,PowerStep);
                   send_msg5SValue(DEBUG_STREAM_MSG,"A1",2,DataA1);
                   send_msg5SValue(DEBUG_STREAM_MSG,"A2",2,DataA2);
                   send_msg5SValue(DEBUG_STREAM_MSG,"Calf",2,DataCalf);
                   send_msg5SValue(DEBUG_STREAM_MSG,"Jitter",2,DataJitter);
                   Beta = HAL_DFE_CalculateBetaX10(DataA1, DataA2, DataCalf );
                   send_msg5SValue(DEBUG_STREAM_MSG,"Beta",2,Beta);
#if 0
                   send_msg5SValue(DEBUG_STREAM_MSG,"data addr",4,(ULONG)&(HAL_OPC_pSRAM[i].data[0]));
                   send_msg5SValue(DEBUG_STREAM_MSG,"data0",1,HAL_OPC_pSRAM[i].data[0]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data1",1,HAL_OPC_pSRAM[i].data[1]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data2",1,HAL_OPC_pSRAM[i].data[2]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data3",1,HAL_OPC_pSRAM[i].data[3]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data4",1,HAL_OPC_pSRAM[i].data[4]);
                   send_msg5SValue(DEBUG_STREAM_MSG,"data5",1,HAL_OPC_pSRAM[i].data[5]);
#endif
               }
               send_msg5SValue(DEBUG_STREAM_MSG,"REG A1",2,READ_FIELD(HAL_DFE_A1MAXPEAK));
               send_msg5SValue(DEBUG_STREAM_MSG,"REG A2",2,READ_FIELD(HAL_DFE_A2MINPEAK));
               send_msg5SValue(DEBUG_STREAM_MSG,"REG CALF",2,READ_FIELD(HAL_DFE_CALF));
               LAL_STREAM_Stop();
               break;

           case LAL_STREAM_STATUS_ERROR:
               StreamError = LAL_STREAM_GetError();
               send_msg5SValue(DEBUG_STREAM_MSG,"Stream error",2,StreamError);
               /* do something with the error here.. */
               LAL_STREAM_Stop();
               break;

           case LAL_STREAM_STATUS_IDLE:
           default:
               send_msg5S(DEBUG_STREAM_MSG,"Stream oeps...");
               ExitProcess();
               break;
        }
    }
    SetTimeout(2, TIMER_STOP);

#if 0
   DelaymS(1);
   GIO3(1);
   DelaymS(1);
   GIO3(0);
#endif
}
#endif //#if ((PCB == 673)||(PCB == 690)||(PCB == 830))

#endif //#if P_DIAGS == 1

USHORT Convert2NewDiscKindT(BYTE disctype)
{
USHORT temp;
temp = disctype & 0x1F; //wbltype and mediatype is the same
if (disctype & bit6) //layer = DL
    temp = temp | 0x040;
else
    temp = temp | 0x020;
if (disctype & bit7) //std = DVD
    temp = temp | 0x100;
else
    temp = temp | 0x080;
return(temp);
}


#if (STORE_MSG_TO_FLASHROM == 1)
void CopyMsgFromDataAreaToROM(void)
{
    BYTE  *dest;
    ULONG index;
    LONG tempsize;
    LinearPatternFill((USHORT)0,0,LOGGING_MSG_PAGE_SIZE,0x800/2,0); //clear
    dest = (BYTE *)SCSDRAM;
    index = Iram_Store_Msg_dram_addr - LOGGING_MSG_START_ADDR;
    if(LoggingFull == TRUE)
    {
       for( tempsize = index;  tempsize < (LONG)(DEBUG_MSG_SIZE); tempsize++)
        {
            *dest = SCSDRAM[LOGGING_MSG_START_ADDR+tempsize];
            dest++;
        }
    }
    for( tempsize = 0;  tempsize < index; tempsize++)
    {
        *dest = SCSDRAM[LOGGING_MSG_START_ADDR+tempsize];
        dest++;
    }
        flashpara_Store_msg_addr = Iram_Store_Msg_dram_addr;
        flashpara_LoggingFull = LoggingFull;

    CloseWritableSegmentTable(eFlash_Debugging_Table);
}
#endif

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
