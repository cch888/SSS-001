/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   macros.h
*
* DESCRIPTION
* This file contains common general purpose macros used by all sub systems
*
* $Revision: 105 $
* $Date: 11/04/27 11:01a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __MACROS_H__
#define __MACROS_H__

#include ".\common\vu_cfg.h"
#include ".\oem\oem_info.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_global.h"
#if (DEBUG_MIO_SETTING==1)
#include ".\al\REG_periph.h"
#include ".\al\REG_wobble.h"
#include ".\al\REG_opc.h"
#include ".\al\REG_afe.h"
#include ".\al\REG_servo.h"
#include ".\al\REG_lightscribe.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\al\REG_dvd_encoder.h"
#endif

#if (MSG_OUT_MODE != MSG_DISABLE)
#include <stdio.h>
#endif /* MSG_OUT_MODE != MSG_DISABLE */

#define ReadSCReg_1B(ADDR)  (*(vBYTE *)(ASIC_BASE+ADDR))
#define ReadSCReg_2B(ADDR) (*(vUSHORT *)(ASIC_BASE+ADDR))
#define ReadSCReg_4B(ADDR)  (*(vULONG *)(ASIC_BASE+ADDR))
#define WriteSCReg_1B(ADDR, DATA)   (*(vBYTE *)(ASIC_BASE+ADDR) = (DATA))
#define WriteSCReg_2B(ADDR, DATA)  (*(vUSHORT *)(ASIC_BASE+ADDR) = (DATA))
#define WriteSCReg_4B(ADDR, DATA)  (*(vULONG *)(ASIC_BASE+ADDR) = (DATA))
#define SetBitsSCReg_1B(ADDR, MASK) (*(vBYTE *)(ASIC_BASE+ADDR) |= (BYTE)(MASK))
#define SetBitsSCReg_2B(ADDR, MASK) (*(vUSHORT *)(ASIC_BASE+ADDR) |= (WORD)(MASK))
#define ClrBitsSCReg_1B(ADDR, MASK) (*(vBYTE *)(ASIC_BASE+ADDR) &= (BYTE)(~(MASK)))
#define ClrBitsSCReg_2B(ADDR, MASK) (*(vUSHORT *)(ASIC_BASE+ADDR) &= (WORD)(~(MASK)))
#define ReadBitsSCReg_1B(ADDR, MASK)   ((*(vBYTE *)(ASIC_BASE+ADDR)) & (BYTE)(MASK))
#define ReadBitsSCReg_2B(ADDR, MASK)   ((*(vUSHORT *)(ASIC_BASE+ADDR)) & (WORD)(MASK))
#define MaskSetSCReg_1B(ADDR, MASK, DATA)   (*(vBYTE *)(ASIC_BASE+ADDR) = ((*(vBYTE *)(ASIC_BASE+ADDR)) & ((BYTE)(~(MASK)))) | (DATA))
#define MaskSetSCReg_2B(ADDR, MASK, DATA)   (*(vUSHORT *)(ASIC_BASE+ADDR) = ((*(vWORD *)(ASIC_BASE+ADDR)) & ((WORD)(~(MASK)))) | (DATA))
#define MaskSetSCReg_4B(ADDR, MASK, DATA)   (*(vULONG *)(ASIC_BASE+ADDR) = ((*(vULONG *)(ASIC_BASE+ADDR)) & ((ULONG)(~(MASK)))) | (DATA))
#define SetBitsSCReg_4B(ADDR, MASK) (*(vULONG *)(ASIC_BASE+ADDR) |= (ULONG)(MASK))
#define ClrBitsSCReg_4B(ADDR, MASK) (*(vULONG *)(ASIC_BASE+ADDR) &= (ULONG)(~(MASK)))


#define ReadMem_1B(ADDR)    (*(vBYTE *)(ADDR))
#define ReadMem_2B(ADDR)    (*(vUSHORT *)(ADDR))
#define ReadMem_4B(ADDR)    (*(vULONG *)(ADDR))
#define WriteMem_1B(ADDR, DATA) (*(vBYTE *)(ADDR) = (DATA))

// Access macros for PersistentTable independent of location
#if (SEEPROM == 1)
#define GetPersistentTable1B(ADDR)          SerialReadByte((ADDR))
#define GetPersistentTable2B(ADDR)          SerialReadWord((ADDR))
#define GetPersistentTable4F(ADDR)          SerialRead4F((ADDR))
#define SetPersistentTable1B(ADDR, DATA)    SerialWriteByte((ADDR), (DATA))
#define SetPersistentTable2B(ADDR, DATA)    SerialWriteWord((ADDR), (DATA))
#define SetPersistentTable4F(ADDR, DATA)    SerialWrite4F((ADDR), (USHORT *)&(DATA))
#define OpenWritableSegmentTable(eTable)
#define CloseWritableSegmentTable(eTable)
#else
#define GetPersistentTable1B(ADDR)          PersistentTable[(ADDR)]
#define GetPersistentTable2B(ADDR)          (*(USHORT *)(PersistentTable + (ADDR)))
#define GetPersistentTable4F(ADDR)          (*(float  *)(PersistentTable + (ADDR)))
#define SetPersistentTable1B(ADDR, DATA)    WriteEepromMap_1B((ADDR), (DATA))
#define SetPersistentTable2B(ADDR, DATA)    WriteEepromMap_2B((ADDR), (DATA))
#define SetPersistentTable4F(ADDR, DATA)    WriteEepromMap_4F((ADDR), (DATA))
#define OpenWritableSegmentTable(eTable)    CopyPersistentTableToDram(eTable)
#define CloseWritableSegmentTable(eTable)              FlashWriteParaTbl(eTable)
#endif

#if (SUN_FUNC_TEST == 1)
#define GetEEPROMTable1B(ADDR)          EEPROM_Table[(ADDR)]
#define GetEEPROMTable2B(ADDR)          (*(USHORT *)(EEPROM_Table + (ADDR)))
#define GetEEPROMTable4F(ADDR)          (*(float  *)(EEPROM_Table + (ADDR)))
#define bE2R1B(ADDR)                        GetEEPROMTable1B((ADDR)); 
#endif

// Access macros for PersistentTable in SDRAM
#define ReadEepromMap_1B(ADDR)  (*(vBYTE *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR))
#define ReadEepromMap_2B(ADDR)    (*(vUSHORT *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR))
#define ReadEepromMap_4F(ADDR)    (*(float *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR))
#define WriteEepromMap_1B(ADDR, DATA) (*(vBYTE *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR) = (DATA))
#define WriteEepromMap_2B(ADDR, DATA) (*(vUSHORT *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR) = (DATA))
#define WriteEepromMap_4F(ADDR, DATA) (*(float *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR) = (DATA))
#define DecEepromMap(ADDR)  ((*(vBYTE *)(SCSDRAM_ADDRESS + DRVPARA_DATA + ADDR))--)

// Access macros for Encoder-Decoder-OPC SRAM
#define ReadEncDecOpcSram_1B(ADDR)    (*(vBYTE *)  (ENC_DEC_OPC_SRAM + ADDR))
#define ReadEncDecOpcSram_2B(ADDR)    (*(vUSHORT *)(ENC_DEC_OPC_SRAM + ADDR))
#define ReadEncDecOpcSram_4B(ADDR)    (*(vULONG *) (ENC_DEC_OPC_SRAM + ADDR))

#define WriteEncDecOpcSram_1B(ADDR, DATA)     (*(vBYTE *)  (ENC_DEC_OPC_SRAM + ADDR)) = DATA
#define WriteEncDecOpcSram_2B(ADDR, DATA)     (*(vUSHORT *)(ENC_DEC_OPC_SRAM + ADDR)) = DATA
#define WriteEncDecOpcSram_4B(ADDR, DATA)     (*(vULONG *) (ENC_DEC_OPC_SRAM + ADDR)) = DATA

// The Interrupt Controller Registers are manipulated with the following macros:

#define WriteINTReg_4B(ADDR, DATA)  (*(volatile unsigned long *)(INTERRUPT_CONTROLLER + ADDR) = (ULONG)DATA)
#define ReadINTReg_4B(ADDR)         (*(volatile unsigned long *)(INTERRUPT_CONTROLLER + ADDR))
#define SetBitINTReg_4B(ADDR,DATA)  (*(volatile unsigned long *)(INTERRUPT_CONTROLLER + ADDR) |= (ULONG)DATA)
#define ClrBitINTReg_4B(ADDR,DATA)  (*(volatile unsigned long *)(INTERRUPT_CONTROLLER + ADDR) &= ~(ULONG)DATA)


#if (MSG_OUT_MODE == MSG_UART)
#define SIZEOFMSGLINE   120
extern char msgBuf[SIZEOFMSGLINE];
#endif

#if ((MSG_OUT_MODE == MSG_USBPORT)&&(DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE))
#define SIZEOFMSGLINE   120
extern char msgBuf[SIZEOFMSGLINE];
#endif /* MSG_OUT_MODE=MSG_USBPORT and DEBUG_PORT_OUTPUT_MODE=UART_INTERFACE */

#if ((MSG_OUT_MODE == MSG_DISABLE)||(MSG_OUT_MODE == MSG_USBPORT))

#define MSGOUT1(LEVEL, MSG) {}
#define MSGOUT2(LEVEL, MSG, P1) {}
#define MSGOUT3(LEVEL, MSG, P1, P2) {}
#define MSGOUT4(LEVEL, MSG, P1, P2, P3) {}
#define MSGOUT5(LEVEL, MSG, P1, P2, P3, P4) {}

#define DUMP_AFE_REGISTERS()    {}

#endif /* (MSG_OUT_MODE == MSG_DISABLE) */

#if ((MSG_OUT_MODE == MSG_USBPORT)&&(DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE))
#define UARTOUT(MSG,CNT)    send_msg2uart(MSG,CNT);
#else
#define UARTOUT(MSG,CNT)   {}
#endif /* MSG_OUT_MODE=MSG_USBPORT and DEBUG_PORT_OUTPUT_MODE=UART_INTERFACE */

#if (MSG_OUT_MODE == MSG_UART)

#define MSGOUT1(LEVEL, MSG)   { \
                                if (LEVEL == 1)\
                                { \
                                UART_out_string(MSG); \
                                } \
                             }

#define MSGOUT2(LEVEL, MSG, P1) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                sprintf(msgBuf, MSG, P1); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                UART_out_string(msgBuf); \
                                } \
                             }

#define MSGOUT3(LEVEL, MSG, P1, P2) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                sprintf(msgBuf, MSG, P1, P2); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                UART_out_string(msgBuf); \
                                } \
                             }

#define MSGOUT4(LEVEL, MSG,P1,P2,P3) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                sprintf(msgBuf, MSG, P1, P2, P3); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                UART_out_string(msgBuf); \
                                } \
                             }

#define MSGOUT5(LEVEL, MSG,P1,P2,P3,P4) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                sprintf(msgBuf, MSG, P1, P2, P3, P4); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                UART_out_string(msgBuf); \
                                } \
                             }

#endif /* MSG_OUT_MODE == MSG_UART */

/*------------------------------------------------------------------------------*/
/* redo orginal rs232 output                                                    */
/*------------------------------------------------------------------------------*/

#if (ENABLE_UART_TXD == 1)
#define RS232OUT1(LEVEL, MSG1)  {                                               \
                                if (LEVEL == 1)                                 \
                                    {   UART_out_string(MSG1);                  \
                                    }                                           \
                                }

#define RS232OUT2(LEVEL, MSG1, P1,P2,P3, MSG2) {if (LEVEL == 1)                 \
                                         {                                      \
                                             UART_out_string(MSG1);             \
                                             UART_out_Val((BYTE)P1);            \
                                             UART_out_Val((BYTE)P2);            \
                                             UART_out_Val((BYTE)P3);            \
                                             UART_out_string(MSG2);             \
                                         }                                      \
                                        }

#define RS232OUT3(LEVEL, MSG1, P1, MSG2) {if (LEVEL == 1)                       \
                                         {                                      \
                                             UART_out_string(MSG1);             \
                                             UART_out_0xVal((USHORT)P1);        \
                                             UART_out_string(MSG2);             \
                                         }                                      \
                                        }

#define RS232OUT4(LEVEL, MSG1, P1, MSG2) {if (LEVEL == 1)                       \
                                         {                                      \
                                             UART_out_string(MSG1);             \
                                             UART_out_Val((USHORT)P1);          \
                                             UART_out_string(MSG2);             \
                                         }                                      \
                                        }
#define RS232OUT5(LEVEL, MSG1, P1,P2, MSG2) {if (LEVEL == 1)                 \
    {                               \
        UART_out_string(MSG1);      \
                                             UART_out_Val((BYTE)P1);            \
                                             UART_out_Val((BYTE)P2);            \
                                             UART_out_string(MSG2);             \
    }                               \
}

/*******************************************************************************
    DVD MESSAGING
*******************************************************************************/
#define DVD_MESSAGE(LEVEL, MSG1){   \
    if (LEVEL == 1)                 \
    {                               \
        UART_out_string("\n\r");    \
        UART_out_string(MSG1);      \
    }                               \
}

#define DVD_MESSAGE1(LEVEL, MSG1, SIZE, P1){            \
    if (LEVEL == 1)                                     \
    {                                                   \
        UART_out_string("\n\r");                        \
        UART_out_string(MSG1);                          \
        if (SIZE == 4)  {UARTLongOut( (ULONG) P1);}     \
        if (SIZE == 2)  {UART_out_0xVal((USHORT)P1);}   \
        if (SIZE == 1)  {UART_out_Val((BYTE)P1);}       \
    }                                                   \
}

#define DVD_MESSAGE2(LEVEL, MSG1, SIZE, P1,MSG2,SIZE1,P2){            \
    if (LEVEL == 1)                                     \
    {                                                   \
        UART_out_string("\n\r");                        \
        UART_out_string(MSG1);                          \
        if (SIZE == 4)  {UARTLongOut( (ULONG) P1);}     \
        if (SIZE == 2)  {UART_out_0xVal((USHORT)P1);}   \
        if (SIZE == 1)  {UART_out_Val((BYTE)P1);}       \
        UART_out_string(MSG2);                          \
        if (SIZE1 == 4)  {UARTLongOut( (ULONG) P2);}     \
        if (SIZE1 == 2)  {UART_out_0xVal((USHORT)P2);}   \
        if (SIZE1 == 1)  {UART_out_Val((BYTE)P2);}       \
    }                                                       \
}

#define DVD_MESSAGE3(LEVEL, MSG1, SIZE1,P1, MSG2,SIZE2,P2, MSG3,SIZE3,P3){  \
    if (LEVEL == 1)                                         \
    {                                                       \
        UART_out_string("\n\r");                            \
        UART_out_string(MSG1);                              \
        if (SIZE1 == 4)  {UARTLongOut( (ULONG) P1);}         \
        if (SIZE1 == 2)  {UART_out_0xVal((USHORT)P1);}       \
        if (SIZE1 == 1)  {UART_out_Val((BYTE)P1);}           \
        UART_out_string(MSG2);                              \
        if (SIZE2 == 4)  {UARTLongOut( (ULONG) P2);}         \
        if (SIZE2 == 2)  {UART_out_0xVal((USHORT)P2);}       \
        if (SIZE2 == 1)  {UART_out_Val((BYTE)P2);}           \
        UART_out_string(MSG3);                              \
        if (SIZE3 == 4)  {UARTLongOut( (ULONG) P3);}        \
        if (SIZE3 == 2)  {UART_out_0xVal((USHORT)P3);}      \
        if (SIZE3 == 1)  {UART_out_Val((BYTE)P3);}          \
    }                                                   \
}


/*******************************************************************************
    HIF TO SIF MESSAGING
*******************************************************************************/
#define HIF_CMD_MESSAGE(LEVEL,TYPE){                \
    if (LEVEL == 1)                             \
    {                                           \
        UART_out_string("\n\r");                \
        if (TYPE == 0)                              \
        {                                           \
        UART_out_string("HIF CMD: ");           \
        UART_out_Val((BYTE)HIF.PlayerOpcode);   \
    }                                           \
        else if (TYPE == 1)                         \
        {                                           \
            UART_out_string("HIF STATE: ");         \
            UART_out_Val((BYTE)HIF.PlayerStatus);   \
        }                                           \
    }                                               \
}


#define SIF_CMD_MESSAGE(LEVEL,TYPE){                \
    if (LEVEL == 1)                             \
    {                                           \
        UART_out_string("\n\r");                \
        if (TYPE == 0)                              \
        {                                           \
            UART_out_string("SVO CMD: ");           \
            UART_out_Val((BYTE)SIF.ServoCommand);   \
        }                                           \
        else if (TYPE == 1)                         \
        {                                           \
            UART_out_string("SVO STATE: ");         \
            UART_out_Val((BYTE)SIF.ServoExecutionState);   \
        }                                           \
        else if (TYPE == 2)                         \
        {                                           \
        UART_out_string("SVO CMD: ");           \
        UART_out_Val((BYTE)SIF.ServoCommand);   \
            UART_out_string(" | SVO STATE: ");      \
            UART_out_Val((BYTE)SIF.ServoExecutionState);   \
        }                                           \
    }                                           \
}


#else

#define RS232OUT1(LEVEL, MSG1)  {}
#define RS232OUT2(LEVEL, MSG1, P1,P2,P3, MSG2) {}
#define RS232OUT3(LEVEL, MSG1, P1, MSG2) {}
#define RS232OUT4(LEVEL, MSG1, P1, MSG2) {}
#define RS232OUT5(LEVEL, MSG1, P1,P2, MSG2) {}

#define DVD_MESSAGE(LEVEL, MSG1){}
#define DVD_MESSAGE1(LEVEL, MSG1, SIZE, P1){}
#define DVD_MESSAGE2(LEVEL, MSG1, SIZE, P1,MSG2,SIZE1,P2){}
#define DVD_MESSAGE3(LEVEL, MSG1, SIZE, P1,  MSG2,SIZE1,P2,  MSG3,SIZE2,P3){}
#define DVD_PRINT_BUFFER(){}
#define DVD_PRINT_DISC_REQUEST(){}
#define DVD_PRINT_HOST_REQUEST(){}

#define HIF_CMD_MESSAGE(LEVEL,TYPE){}
#define SIF_CMD_MESSAGE(LEVEL,TYPE){}

#endif


#if (MSG_OUT_MODE == MSG_MICE)

#define MSGOUT1(LEVEL, MSG)   { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                printf(MSG); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                } \
                             }

#define MSGOUT2(LEVEL, MSG, P1) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                printf(MSG, P1); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                } \
                             }

#define MSGOUT3(LEVEL, MSG, P1, P2) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                printf(MSG, P1, P2); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                } \
                             }

#define MSGOUT4(LEVEL, MSG,P1,P2,P3) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                printf(MSG, P1, P2, P3); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                } \
                             }

#define MSGOUT5(LEVEL, MSG,P1,P2,P3,P4) { \
                                if (LEVEL == 1)\
                                { \
                                vULONG dwPRNIRQStatus;  \
                                dwPRNIRQStatus = GET_INT_ENABLE_MASK();  \
                                DISABLE_INTERRUPT(); \
                                Save_SP_register(); \
                                printf(MSG, P1, P2, P3, P4); \
                                Restore_SP_register(); \
                                SET_INT_ENABLE_MASK(dwPRNIRQStatus); \
                                } \
                             }

#endif /* MSG_OUT_MODE == MSG_MICE */

//================= Servo Message Types ==========================
#define NO_DEBUG_MSG                    0x00000000
#define SHOW_DEBUG_MSG                  0x00000001 //1. Use Msg Range 0x70,0x00,0x00~~0x70,0x00,0xFF
#define STATE_MACHINE_FLOW_MSG          0x00000002 //2. Use Msg Range 0x50,0x00,0x00~~0x50,0x01,0xFF
#define DEBUG_DISK_ID_MSG               0x00000004 //3. Use Msg Range 0x51,0x00,0x00~~0x51,0x5F,0xFF
#define DEBUG_AUTO_TUNING_MSG           0x00000008 //4. Use Msg Range 0x51,0x60,0x00~~0x51,0xFF,0xFF
#define DEBUG_KPOWER_MSG                0x00000010 //5. Use Msg Range 0x53,0x50,0x00~~0x53,0xFF,0xFF
#define DEBUG_SVO_SPD_MSG               0x00000020 //6. Use Msg Range 0x53,0x00,0x00~~0x53,0x13,0xFF
#define DEBUG_SEEK_MSG                  0x00000040 //7. Use Msg Range 0x53,0x40,0x00~~0x53,0x46,0xFF
#define DEBUG_VENDOR_WRITE_MSG          0x00000080 //8. Use Msg Range 0x55,0x00,0x00~~0x55,0xFF,0xFF
#define DEBUG_SEEK_STEP_DATA_MSG        0x00000100 //9. Use Msg Range 0x54,0x00,0x00~~0x54,0xFF,0xFF
#define DEBUG_OPC_MSG                   0x00000200 //10.Use Msg Range 0x56,0x00,0x00~~0x56,0xFF,0xFF
#define DEBUG_OFFLINE_CALIB_MSG         0x00000400 //11.Use Msg Range 0x53,0x80,0x00~~0x53,0x86,0xFF
#define DEBUG_CD_WRITE_MSG              0x00000800 //12.Use Msg Range 0x57,0x00,0x00~~0x57,0xFF,0xFF
#define DEBUG_DVD_WRITE_MSG             0x00001000 //13.Use Msg Range 0x58,0x00,0x00~~0x58,0xFF,0xFF
#define DEBUG_WOBBLE_FLOW_MSG           0x00002000 //14.Use Msg Range 0x59,0x00,0x00~~0x59,0xFF,0xFF
#define DEBUG_CALIB_ENTRYS_MSG          0x00004000 //15.Use Msg Range 0x5A,0x00,0x00~~0x5A,0xFF,0xFF
#define DEBUG_CALIB_RESULT_MSG          0x00008000 //16.Use Msg Range 0x5B,0x00,0x00~~0x5B,0xFF,0xFF
#define DEBUG_DSP_MSG                   0x00010000 //17.Use Msg Range 0x77,0x00,0x00~~0x77,0xFF,0xFF
#define DEBUG_BD_CODEC_MSG              0x00020000 //18.Use Msg Range 0x5D,0x00,0x00~~0x5D,0xFF,0xFF
#define DEBUG_SERVO_TRACE_MSG           0x00040000 //19.Use Msg Range 0x5E,0x00,0x00~~0x5E,0xFF,0xFF
#define UART_COMMAND_TRACE_MSG          0x00080000 //20.Use Msg Range 0x5F,0x00,0x00~~0x5F,0xFF,0xFF
#define DEBUG_DFE_MSG                   0x00100000 //21.Use Msg Range 0x60,0x00,0x00~~0x60,0xFF,0xFF
#define DEBUG_WBL_MSG                   0x00200000 //22.Use Msg Range 0x61,0x00,0x00~~0x61,0xFF,0xFF
#define DEBUG_COEF_MSG                  0x00400000 //23.Use Msg Range 0x62,0x00,0x00~~0x62,0xFF,0xFF
#define DEBUG_LASER_MSG                 0x00800000 //24.Use Msg Range 0x63,0x00,0x00~~0x62,0xFF,0xFF
#define HEADER_ID_MSG                   0x01000000 //25.Use Msg Range 0x5B,0x06,0x00~~0x5B,0x06,0x0F
#define DEBUG_STREAM_MSG                0x02000000 //26.Use Msg Range 0x64,0x00,0x00~~0x64,0xFF,0xFF
#define DEBUG_MATH2D_MSG                0x04000000 //27.Use Msg Range 0x65,0x00,0x00~~0x65,0xFF,0xFF
#define DEBUG_DEMOD_MSG                 0x08000000 //28.Use Msg Range 0x66,0x00,0x00~~0x66,0xFF,0xFF
#define DEBUG_BD_WRITE_MSG              0x10000000 //29.Use Msg Range 0x67,0x00,0x00~~0x67,0xFF,0xFF
#define DEBUG_CUSTOMER_MSG1             0x20000000 //30.Use Msg Range 0x68,0x00,0x00~~0x68,0xFF,0xFF
#define DEBUG_CUSTOMER_MSG2             0x40000000 //31.Use Msg Range 0x69,0x00,0x00~~0x69,0xFF,0xFF

//================= HIF Message Types ==========================
#define SEND_ATAPI_CMD                  0x80000001  //1. Send ATA/ATAPI command
#define SEND_HIF_CMD                    0x80000002  //2. Use Msg Range 0xA0,0x00,0x00~~0xA0,0x0F,0x00
#define SEND_WRITE_MSG                  0x80000004  //3.
#define SEND_MRW_MSG                    0x80000008  //4. Use Msg Range 0xFD,0x00,0x00~~0xFD,0x03,0xFF
#define SEND_READ_DVD_MSG               0x80000010  //5.
#define SEND_DVD_MINUS_WR_MSG           0x80000020  //6. Send DVD-R Write message
#define SEND_SStpTask_MSG               0x80000040  //7. Use Msg Range 0xB0,0x00,0x00~~0xB0,0x02,0xFF
#define SEND_PlayTask_MSG               0x80000080  //8. Use Msg Range 0xB1,0x00,0x00~~0xB1,0x07,0xFF
#define SEND_RecTask_MSG                0x80000100  //9. Use Msg Range 0xB2,0x00,0x00~~0xB2,0x01,0xFF
#define SEND_TrayTask_MSG               0x80000200  //10.Use Msg Range 0xB3,0x00,0x00~~0xB3,0x02,0xFF
#define SEND_InfoTask_MSG               0x80000400  //11.Use Msg Range 0xB4,0x00,0x00~~0xB4,0x06,0xFF
#define SEND_MiscTask_MSG               0x80000800  //12.Use Msg Range 0xB5,0x00,0x00~~0xB5,0x02,0xFF
#define SEND_PlayerTask_MSG             0x80001000  //13.Use Msg Range 0xB7,0x00,0x00~~0xB7,0x0E,0xFF
#define SEND_SvmnTask_MSG               0x80002000  //14.Use Msg Range 0xB8,0x00,0x00~~0xB8,0x01,0xFF
#define SEND_OpcTask_MSG                0x80004000  //15.Use Msg Range 0xB9,0x00,0x00~~0xB9,0x01,0xFF
#define SEND_RdTocTask_MSG              0x80008000  //16.Use Msg Range 0xBA,0x00,0x00~~0xBA,0x02,0xFF
#define SEND_WATCH_MSG                  0x80010000  //17.Use Msg Range 0xD6,0x00,0x00~~0xD6,0x10,0xFF
#define SEND_SRAM_EMPTY_STACK           0x80020000  //18.print sram empty stack size
#define SEND_ATIP_WOBBLE                0x80040000  //19.print ATIP or ADIP/LPP
#define SEND_QCODE_DATAID               0x80080000  //20.print Qcode or Data ID
#define SEND_DVDPLUSRW_MSG              0x80100000  //21.Send DVD+RW format message. Range 0xBB,0x00,0x00~~0xBB,0x01,0xFF
#define SEND_DVDRAM_ICE                 0x80200000  //22.print dvdram ice buffering debug message
//#define NOT_USE_22                      0x80200000
//#define NOT_USE_23                      0x80400000
//#define NOT_USE_24                      0x80800000
//#define NOT_USE_25                      0x81000000
//#define NOT_USE_26                      0x82000000
//#define NOT_USE_27                      0x84000000
//#define NOT_USE_28                      0x88000000
//#define NOT_USE_29                      0x900040000
//#define NOT_USE_30                      0xA0000000

#if (MSG_OUT_MODE == MSG_USBPORT)
//#define send_atapi_cmd(msg_gate) {send_msg6C(msg_gate);}
#define send_atapi_cmd_time(msg_gate) {send_msg6F(msg_gate);}
#define send_qcode(msg_gate) {send_msg3A(msg_gate);}

#define set_hif_msg(msg_gate)   hifDebugMsgFlag = ((msg_gate) & 0x7FFFFFFF)
#define open_hif_msg(msg_gate)  hifDebugMsgFlag = ((hifDebugMsgFlag | (msg_gate)) & 0x7FFFFFFF)
#define close_hif_msg(msg_gate) hifDebugMsgFlag = (hifDebugMsgFlag & (~(msg_gate)))
#define set_svo_msg(msg_gate)   svoDebugMsgFlag = (msg_gate)
#define open_svo_msg(msg_gate)  svoDebugMsgFlag = (svoDebugMsgFlag | (msg_gate))
#define close_svo_msg(msg_gate) svoDebugMsgFlag = (svoDebugMsgFlag & (~(msg_gate)))
#else
#define send_atapi_cmd(msg_gate) {}
#define send_qcode(msg_gate) {}
#define set_hif_msg(msg_gate)
#define open_hif_msg(msg_gate)
#define close_hif_msg(msg_gate)
#define set_svo_msg(msg_gate)
#define open_svo_msg(msg_gate)
#define close_svo_msg(msg_gate)
#endif /* #if (MSG_OUT_MODE==MSG_USBPORT)  */

#if (PCB == 710)||(PCB == 711)
#define SWB1(value) {WRITE_FIELD(HAL_APB_SSI_EXTSEN3,1);WRITE_FIELD(HAL_APB_SSI_PSEN3,value);}
#define SWB2(value) {WRITE_FIELD(HAL_APB_SSI_EXTSEN,1);WRITE_FIELD(HAL_APB_SSI_PSEN,value);}
#define SW_OUTPUT() {WRITE_FIELD(HAL_GLOBAL_MIO3SEL,FALSE);WRITE_FIELD(HAL_PERIPH_PB7DIR,FALSE);}
#define SW_INPUT()  {WRITE_FIELD(HAL_GLOBAL_MIO3SEL,FALSE);WRITE_FIELD(HAL_PERIPH_PB7DIR,TRUE);}
#endif
#if (PCB == 815)
#define OEICSW1H() {WRITE_FIELD(HAL_PERIPH_PA0DIR,FALSE);WRITE_FIELD(HAL_PERIPH_PA0DATA,TRUE);}
#define OEICSW1M()  {WRITE_FIELD(HAL_PERIPH_PA0DIR,TRUE);WRITE_FIELD(HAL_PERIPH_PA0DATA,FALSE);}
#define OEICSW1L() {WRITE_FIELD(HAL_PERIPH_PA0DIR,FALSE);WRITE_FIELD(HAL_PERIPH_PA0DATA,FALSE);}
#define OEICSW2H() {WRITE_FIELD(HAL_PERIPH_PB1DIR,FALSE);WRITE_FIELD(HAL_PERIPH_PB1DATA,TRUE);}
#define OEICSW2M()  {WRITE_FIELD(HAL_PERIPH_PB1DIR,TRUE);WRITE_FIELD(HAL_PERIPH_PB1DATA,FALSE);}
#define OEICSW2L() {WRITE_FIELD(HAL_PERIPH_PB1DIR,FALSE);WRITE_FIELD(HAL_PERIPH_PB1DATA,FALSE);}
#endif

#if (DEBUG_MIO_SETTING==1)
//----------MIO Use Note----------
//MIO to GPIO Output selection
#define Set_MIO0_TO_GPIO() {WRITE_FIELD(HAL_GLOBAL_MIO0SEL,FALSE);WRITE_FIELD(HAL_PERIPH_PB4DIR,FALSE);}
#define Set_MIO1_TO_GPIO() {WRITE_FIELD(HAL_GLOBAL_MIO1SEL,FALSE);WRITE_FIELD(HAL_PERIPH_PB5DIR,FALSE);}
#define Set_MIO2_TO_GPIO() {WRITE_FIELD(HAL_GLOBAL_MIO2SEL,FALSE);WRITE_FIELD(HAL_PERIPH_PB6DIR,FALSE);}
#if PCB == 830
#define Set_MIO3_TO_GPIO() {}
#else
#define Set_MIO3_TO_GPIO() {WRITE_FIELD(HAL_GLOBAL_MIO3SEL,FALSE);WRITE_FIELD(HAL_PERIPH_PB7DIR,FALSE);}
#endif

//GIO selection
#define GIO0(value) {PB.DATA.BIT.B4=(BOOL)value;}
#define GIO1(value) {PB.DATA.BIT.B5=(BOOL)value;}
#define GIO2(value) {PB.DATA.BIT.B6=(BOOL)value;}
#if PCB == 830
#define GIO3(value) {}
#else
#define GIO3(value) {PB.DATA.BIT.B7=(BOOL)value;}
#endif

//MIO setting
#define Set_MIO0_TO_MIO()  WRITE_FIELD(HAL_GLOBAL_MIO0SEL,TRUE)
#define Set_MIO1_TO_MIO()  WRITE_FIELD(HAL_GLOBAL_MIO1SEL,TRUE)
#define Set_MIO2_TO_MIO()  WRITE_FIELD(HAL_GLOBAL_MIO2SEL,TRUE)
#if PCB == 830
#define Set_MIO3_TO_MIO() {}
#else
#define Set_MIO3_TO_MIO()  WRITE_FIELD(HAL_GLOBAL_MIO3SEL,TRUE)
#endif

//MIO selection
#define Set_MIO0_TO_DSPSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,CH);}
#define Set_MIO1_TO_DSPSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,CH);}
#define Set_MIO2_TO_DSPSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,CH);}
#if PCB == 830
#define Set_MIO3_TO_DSPSignal(CH) {}
#else
#define Set_MIO3_TO_DSPSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,CH);}
#endif
//00h testopt

//01h def_subld
//03h c2_frame_unc
//04h cd_d_scor
//05h enc_esfs
//06h cde_write_clk

//08h sv_mio
#define Set_MIO0_TO_SVOSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_sv_mio0);WRITE_FIELD(HAL_SRV_SVMIO0SEL,CH);}
#define Set_MIO1_TO_SVOSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_sv_mio1);WRITE_FIELD(HAL_SRV_SVMIO1SEL,CH);}
#define Set_MIO2_TO_SVOSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_sv_mio2);WRITE_FIELD(HAL_SRV_SVMIO2SEL,CH);}
#if PCB == 830
#define Set_MIO3_TO_SVOSignal(CH) {}
#else
#define Set_MIO3_TO_SVOSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_sv_mio3);WRITE_FIELD(HAL_SRV_SVMIO3SEL,CH);}
#endif

//09h wobble_mio_debug
#define Set_MIO0_TO_WOBSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_wobble_mio_debug0);WRITE_FIELD(HAL_WBL_WBLMIOSEL,CH);}
#define Set_MIO1_TO_WOBSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_wobble_mio_debug1);WRITE_FIELD(HAL_WBL_WBLMIOSEL,CH);}
#define Set_MIO2_TO_WOBSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_wobble_mio_debug2);WRITE_FIELD(HAL_WBL_WBLMIOSEL,CH);}
#if PCB == 830
#define Set_MIO3_TO_WOBSignal(CH) {}
#else
#define Set_MIO3_TO_WOBSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_wobble_mio_debug3);WRITE_FIELD(HAL_WBL_WBLMIOSEL,CH);}
#endif

//0Eh opc_mio(already in HAL_OPC_ConfigMioDebug())
#define Set_MIO0_TO_OPCSignal(RST,WST,MIO,CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_opc_mio0);WRITE_FIELD(HAL_OPC_RSTRT,RST);WRITE_FIELD(HAL_OPC_WSTRT,WST);WRITE_FIELD(HAL_OPC_OPCMIO,MIO);WRITE_FIELD(HAL_OPC_SAMPNUM,CH);}
#define Set_MIO1_TO_OPCSignal(RST,WST,MIO,CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_opc_mio1);WRITE_FIELD(HAL_OPC_RSTRT,RST);WRITE_FIELD(HAL_OPC_WSTRT,WST);WRITE_FIELD(HAL_OPC_OPCMIO,MIO);WRITE_FIELD(HAL_OPC_SAMPNUM,CH);}
#define Set_MIO2_TO_OPCSignal(RST,WST,MIO,CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_opc_mio2);WRITE_FIELD(HAL_OPC_RSTRT,RST);WRITE_FIELD(HAL_OPC_WSTRT,WST);WRITE_FIELD(HAL_OPC_OPCMIO,MIO);WRITE_FIELD(HAL_OPC_SAMPNUM,CH);}
#if PCB == 830
#define Set_MIO3_TO_OPCSignal(RST,WST,MIO,CH) {}
#else
#define Set_MIO3_TO_OPCSignal(RST,WST,MIO,CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_opc_mio3);WRITE_FIELD(HAL_OPC_RSTRT,RST);WRITE_FIELD(HAL_OPC_WSTRT,WST);WRITE_FIELD(HAL_OPC_OPCMIO,MIO);WRITE_FIELD(HAL_OPC_SAMPNUM,CH);}
#endif

//0Fh sata_dbg(need check)
#define Set_MIO0_TO_SATASignal(SEL,CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_sata_dbg0);WRITE_FIELD(HAL_HST_ENADBGHI,SEL);WRITE_FIELD(HAL_HST_SATADEBUG,CH);}
#define Set_MIO1_TO_SATASignal(SEL,CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_sata_dbg1);WRITE_FIELD(HAL_HST_ENADBGHI,SEL);WRITE_FIELD(HAL_HST_SATADEBUG,CH);}
#define Set_MIO2_TO_SATASignal(SEL,CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_sata_dbg2);WRITE_FIELD(HAL_HST_ENADBGHI,SEL);WRITE_FIELD(HAL_HST_SATADEBUG,CH);}
#if PCB == 830
#define Set_MIO3_TO_SATASignal(SEL,CH) {}
#else
#define Set_MIO3_TO_SATASignal(SEL,CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_sata_dbg3);WRITE_FIELD(HAL_HST_ENADBGHI,SEL);WRITE_FIELD(HAL_HST_SATADEBUG,CH);}
#endif

//10h xin25_clk
//11h bhaddr31280
//12h mema_armt_rCmdAck

//14h afe_mio
#define Set_MIO0_TO_AFESignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_afe_mio0);WRITE_FIELD(HAL_AFE_AFEMIOSEL,CH);}
#define Set_MIO1_TO_AFESignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_afe_mio1);WRITE_FIELD(HAL_AFE_AFEMIOSEL,CH);}
#define Set_MIO2_TO_AFESignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_afe_mio2);WRITE_FIELD(HAL_AFE_AFEMIOSEL,CH);}
#if PCB == 830
#define Set_MIO3_TO_AFESignal(CH) {}
#else
#define Set_MIO3_TO_AFESignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_afe_mio3);WRITE_FIELD(HAL_AFE_AFEMIOSEL,CH);}
#endif

//15h ws_mio
#define Set_MIO0_TO_WSCSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_ws_mio0);WRITE_FIELD(HAL_WS_WSMIO0,CH);}
#define Set_MIO1_TO_WSCSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_ws_mio1);WRITE_FIELD(HAL_WS_WSMIO1,CH);}
#define Set_MIO2_TO_WSCSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_ws_mio2);WRITE_FIELD(HAL_WS_WSMIO2,CH);}
#if PCB == 830
#define Set_MIO3_TO_WSCSignal(CH) {}
#else
#define Set_MIO3_TO_WSCSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_ws_mio3);WRITE_FIELD(HAL_WS_WSMIO3,CH);}
#endif

//17h lsmio
#define Set_MIO0_TO_LSSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_lsmio);WRITE_FIELD(HAL_LS_LSMIOSEL,CH);}
#define Set_MIO1_TO_LSSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_lsmio);WRITE_FIELD(HAL_LS_LSMIOSEL,CH);}
#define Set_MIO2_TO_LSSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_lsmio);WRITE_FIELD(HAL_LS_LSMIOSEL,CH);}
#if PCB == 830
#define Set_MIO3_TO_LSSignal(CH) {}
#else
#define Set_MIO3_TO_LSSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_lsmio);WRITE_FIELD(HAL_LS_LSMIOSEL,CH);}
#endif
//18h s_defect2afe

//1Ah dvdd_test0
#define Set_MIO0_TO_DVDSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_dvdd_test0);WRITE_FIELD(HAL_DVDD_TPMUX,CH);}
#define Set_MIO1_TO_DVDSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_dvdd_test1);WRITE_FIELD(HAL_DVDD_TPMUX,CH);}
#define Set_MIO2_TO_DVDSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_dvdd_test2);WRITE_FIELD(HAL_DVDD_TPMUX,CH);}
#if PCB == 830
#define Set_MIO3_TO_DVDSignal(CH) {}
#else
#define Set_MIO3_TO_DVDSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_dvdd_test3);WRITE_FIELD(HAL_DVDD_TPMUX,CH);}
#endif

//1Bh dvd_pipo_info
#define Set_MIO0_TO_DVDPIPOSignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_dvd_pipo_info);}
#define Set_MIO1_TO_DVDPIPOSignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_dvd_pipo_bst);}
#define Set_MIO2_TO_DVDPIPOSignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_dvd_pipo_reset);}
#if PCB == 830
#define Set_MIO3_TO_DVDPIPOSignal(CH) {}
#else
#define Set_MIO3_TO_DVDPIPOSignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_dvd_pipo_sel);}
#endif

//1Ch dvde_test_mio
#define Set_MIO0_TO_DVESignal(CH) {Set_MIO0_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M0SEL,halGLOBAL_MIO0_dvde_test_mio0);WRITE_FIELD(HAL_DVDE_DVDEMUX,CH);}
#define Set_MIO1_TO_DVESignal(CH) {Set_MIO1_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M1SEL,halGLOBAL_MIO1_dvde_test_mio1);WRITE_FIELD(HAL_DVDE_DVDEMUX,CH);}
#define Set_MIO2_TO_DVESignal(CH) {Set_MIO2_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M2SEL,halGLOBAL_MIO2_dvde_test_mio2);WRITE_FIELD(HAL_DVDE_DVDEMUX,CH);}
#if PCB == 830
#define Set_MIO3_TO_DVESignal(CH) {}
#else
#define Set_MIO3_TO_DVESignal(CH) {Set_MIO3_TO_MIO();WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_dvde_test_mio3);WRITE_FIELD(HAL_DVDE_DVDEMUX,CH);}
#endif

//1Fh cache_monitor


#else // (DEBUG_MIO_SETTING != 1)
#define Set_MIO0_TO_GPIO() {}
#define Set_MIO1_TO_GPIO() {}
#define Set_MIO2_TO_GPIO() {}
#define Set_MIO3_TO_GPIO() {}

#define GIO0(value) {}
#define GIO1(value) {}
#define GIO2(value) {}
#define GIO3(value) {}

#define Set_MIO0_TO_MIO()  {}
#define Set_MIO1_TO_MIO()  {}
#define Set_MIO2_TO_MIO()  {}
#define Set_MIO3_TO_MIO()  {}

#define Set_MIO0_TO_DSPSignal(CH) {}
#define Set_MIO1_TO_DSPSignal(CH) {}
#define Set_MIO2_TO_DSPSignal(CH) {}
#define Set_MIO3_TO_DSPSignal(CH) {}

#define Set_MIO0_TO_WOBSignal(CH) {}
#define Set_MIO1_TO_WOBSignal(CH) {}
#define Set_MIO2_TO_WOBSignal(CH) {}
#define Set_MIO3_TO_WOBSignal(CH) {}

#define Set_MIO0_TO_DRESignal(CH) {}
#define Set_MIO1_TO_DRESignal(CH) {}
#define Set_MIO2_TO_DRESignal(CH) {}
#define Set_MIO3_TO_DRESignal(CH) {}

#define Set_MIO0_TO_JITSignal(CH) {}
#define Set_MIO1_TO_JITSignal(CH) {}
#define Set_MIO2_TO_JITSignal(CH) {}
#define Set_MIO3_TO_JITSignal(CH) {}

#define Set_MIO0_TO_OPCSignal(CH) {}
#define Set_MIO1_TO_OPCSignal(CH) {}
#define Set_MIO2_TO_OPCSignal(CH) {}
#define Set_MIO3_TO_OPCSignal(CH) {}

#define Set_MIO0_TO_AFESignal(CH) {}
#define Set_MIO1_TO_AFESignal(CH) {}
#define Set_MIO2_TO_AFESignal(CH) {}
#define Set_MIO3_TO_AFESignal(CH) {}

#define Set_MIO0_TO_LSCSignal(CH) {}
#define Set_MIO1_TO_LSCSignal(CH) {}
#define Set_MIO2_TO_LSCSignal(CH) {}
#define Set_MIO3_TO_LSCSignal(CH) {}

#define Set_MIO0_TO_DVDSignal(CH) {}
#define Set_MIO1_TO_DVDSignal(CH) {}
#define Set_MIO2_TO_DVDSignal(CH) {}
#define Set_MIO3_TO_DVDSignal(CH) {}

#define Set_MIO0_TO_DVESignal(CH) {}
#define Set_MIO1_TO_DVESignal(CH) {}
#define Set_MIO2_TO_DVESignal(CH) {}
#define Set_MIO3_TO_DVESignal(CH) {}

#define SetEQDO_WOBBLE_OUT(CH)  {}
#endif /* DEBUG_MIO_SETTING */


//***********************************************************************************************
//
//   Messaging macros
//
//   Function of these macros are described in the individual functions called
//
//***********************************************************************************************

    #if (MSG_OUT_MODE == MSG_USBPORT)
#define send_msg1(d1, d2, d3, d4)                   /* See description in SendMsg1()   */   \
        {                                                                                   \
            SendMsg1(d1, d2, d3, d4);                                                       \
        }

#define send_msg80(msg_gate, func_code)                                                     \
        {                                                                                   \
            SendMsg80(msg_gate,func_code);                                                  \
        }

#define send_msg6F(msg_gate)                                                                \
        {                                                                                   \
            SendMsg6F(msg_gate);                                                            \
        }
        #if 1
#define send_msg54(msg_gate, d1, d2, d3, d4)                                                \
        {                                                                                   \
            SendMsg54(msg_gate, d1, d2, d3, d4);                                            \
        }
        #endif

#define send_msg5S(msg_gate, message)                                                       \
        {                                                                                   \
            SendMsg5S(msg_gate, message);                                                   \
        }

#define send_msg4USHORTs(msg_gate, value1, value2, value3, value4)                          \
        {                                                                                   \
            SendMsg4USHORTs(msg_gate, value1, value2, value3, value4);                      \
        }

#define send_msg5SValue(msg_gate, message,  size,  value)                                   \
        {                                                                                   \
            SendMsg5SValue(msg_gate, message,  size,  value);                               \
        }


#define send_msg5F(msg_gate, h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12,h13,h14,h15)            \
        {                                                                                   \
            SendMsg5F(msg_gate,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12,h13,h14,h15);         \
        }

#define send_msg44(msg_gate, d1, d2, d3, d4)                                                \
        {                                                                                   \
            SendMsg44(msg_gate, d1, d2, d3, d4);                                            \
        }

#define send_msg3A(msg_gate)                                                                \
        {                                                                                   \
            SendMsg3A(msg_gate);                                                            \
        }

    #else // #if (MSG_OUT_MODE == MSG_USBPORT)

#define send_msg(d1)
#define send_msg1(d1, d2, d3, d4)
#define send_msg80(msg_gate, func_code)
#define send_msg81(msg_gate, func_code, h1)
#define send_msg82(msg_gate, func_code, h1, h2)
#define send_msg82S(msg_gate, func_code, value)
#define send_msg84(msg_gate, func_code, h1, h2, h3, h4)
#define send_msg84L(msg_gate, func_code, value)
#define send_msg74(msg_gate, func_code, data1)
#define send_msg6C(msg_gate)
#define send_msg6F(msg_gate)                                                                \
#define send_msg54(msg_gate, d1, d2, d3, d4)
#define send_msg5S(msg_gate, message)
#define send_msg4USHORTs( msg_gate, value1, value2, value3, value4)
#define send_msg5SValue( msg_gate, message,  size,  value)
#define send_msg5L(msg_gate,  value)
#define send_msg5F(Fmsg_gate, h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12,h13,h14,h15)
#define send_msg44(msg_gate, d1, d2, d3, d4)
#define send_msg3A(msg_gate)
#define send_msg21(msg_gate, func_code, dd1)
#define send_msg24(msg_gate, func_code, dd1, dd2, dd3, dd4)

    #endif // #if (MSG_OUT_MODE == MSG_USBPORT)

    #if (MSG_OUT_MODE == MSG_USBPORT)
//#define send_atapi_cmd(msg_gate)    {send_msg6C(msg_gate);}
#define send_qcode(msg_gate)        {send_msg3A(msg_gate);}
    #else
#define send_atapi_cmd(msg_gate)    {}
#define send_qcode(msg_gate)        {}
    #endif

    #if (ASSERT_MODE == MSG_ASSERT_CONTINUE)
#define DEBUG_ASSERT(t,c)                                                   \
        if (!(t)) {                                                         \
                     SendMsg80(SHOW_DEBUG_MSG,0x700000+c);                  \
                     send_msg5SValue(SHOW_DEBUG_MSG,__MODULE__,2,__LINE__); \
                  }
    #endif

    #if (ASSERT_MODE == MSG_ASSERT_HALT)
#define DEBUG_ASSERT(t,c)                                                   \
        if (!(t)) {                                                         \
                     SendMsg80(SHOW_DEBUG_MSG,0x700000+c);                  \
                     send_msg5SValue(SHOW_DEBUG_MSG,__MODULE__,2,__LINE__); \
                     DelaymS(5);                                            \
                     while (1) {};                                          \
                  }
    #endif
    #if (ASSERT_MODE == MSG_ASSERT_RESET)
#define DEBUG_ASSERT(t,c)                                                   \
        if (!(t)) {                                                         \
                     SendMsg80(SHOW_DEBUG_MSG,0x700000+c);                  \
                     send_msg5SValue(SHOW_DEBUG_MSG,__MODULE__,2,__LINE__); \
                     DelaymS(5);
                     /* TBD Perform shutdown/reset actions here */
                  }
    #endif
    #if (ASSERT_MODE == MSG_ASSERT_NONE)
#define DEBUG_ASSERT(t,c)
    #endif

#if FOOTILT_DIFFERENTIAL == 1
#define CONVERT_SIGNED_10b_TO_16b(x) ((SHORT)(x & 0x200? (x - 0x400): x))
#define CONVERT_TILTDAT_TO_FOO(x) ((SHORT)(x & 0x200? (((x + 1) >> 1) - 0x200): ((x + 1) >> 1)))
#endif

#if(PCB == 710)
#define DVDLD_EN   (opDVD_EN = 1)
#define DVDLD_DIS  (opDVD_EN = 0)
#define CDLD_EN    (opCD_EN = 1)
#define CDLD_DIS   (opCD_EN = 0)
#elif (PCB == 711)
#define DVDLD_EN   (opDVD_EN = 1)
#define DVDLD_DIS  (opDVD_EN = 0)
#define CDLD_EN    {WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One);WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_One);}
#define CDLD_DIS   {WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_Zero);}
#elif (PCB == 817)
#define DVDLD_EN   (opDVD_RLD = 1)
#define DVDLD_DIS  (opDVD_RLD = 0)
#define CDLD_EN    (opDVD_RLD = 0)
#define CDLD_DIS   (opDVD_RLD = 1)
#define DVDCD_STB_DVD    {WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_Zero);} //<0.8V
#define DVDCD_STB_CD     {WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_One);WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_One);}   //>2.6V
#define DVDCD_STB_STB    {WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_HiZ1);WRITE_FIELD(HAL_WS_GAINSWW, halWS_GAINSWW_HiZ1);} //1.25~2.05V
#define BD_SLEEP_EN      {WRITE_FIELD(HAL_APB_SSI_EXTSEN3,1);WRITE_FIELD(HAL_APB_SSI_PSEN3,1);} //sleep
#define BD_SLEEP_DIS     {WRITE_FIELD(HAL_APB_SSI_EXTSEN3,1);WRITE_FIELD(HAL_APB_SSI_PSEN3,0);} //highgain 10.8db
#else
#define DVDLD_EN   (opDVD_EN = 1)
#define DVDLD_DIS  (opDVD_EN = 0)
#define CDLD_EN    (opCD_EN = 1)
#define CDLD_DIS   (opCD_EN = 0)
#endif

#endif // __MACROS__
