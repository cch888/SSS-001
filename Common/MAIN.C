/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   main.c
*
* DESCRIPTION
*   This file contains function main
*
*   $Revision: 76 $
*   $Date: 11/05/03 2:42p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\powerctl.h"
#if (EN_FW_SIGNATURE == 1)
#include ".\crypt\FWSigCheck.h"
#endif
#include ".\oem\oem_tbl.h"
#include ".\hif\gp_mac.h"
#include ".\hif\flash.h"

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_oem_tbl.h"
#endif

extern void InitializeARM(void);
extern void HIFMain(void);
extern void WRProcessMain(void);
extern void TProcessMain(void);
extern void PlayerMain(void);
extern void ServoMain(void);
extern void BGProcessMain(void);

ULONG hif_stack[(HIF_STACK_SIZE) / 4];
ULONG player_stack[(PLAYER_STACK_SIZE) / 4];
ULONG servo_stack[(SERVO_STACK_SIZE) / 4];
ULONG tp_stack[(TIMER_STACK_SIZE) / 4];

const TASK_ENTRY_TBL task_tbl[PROCESS_MAX] = {
    {0,HIFMain          ,&hif_stack[0]      ,sizeof(hif_stack)   },
    {1,PlayerMain       ,&player_stack[0]   ,sizeof(player_stack)},
    {2,ServoMain        ,&servo_stack[0]    ,sizeof(servo_stack) },
    {3,TProcessMain     ,&tp_stack[0]       ,sizeof(tp_stack)    },
};

vULONG dwOrgStack;
const ULONG dwPrnStack = 0x601FF7F0;

/******************************************************************/
/*  Subroutine  |main                                             */
/*  Function    |process is entried and start                     */
/*  Input       |None                                             */
/*  Output      |None                                             */
/*  Global      |                                                 */
/*  Editor      |                                                 */
/*  Update      |                                                 */
/******************************************************************/
#if (SEEPROM ==1)
BOOL CheckEEPROMSignature(void);
void LoadSerialEEPROM(void);
#endif

int main(void)
{
    int i, j;

    ULONG dwData = 0;
    vULONG MsgMarks;
#if (MSG_OUT_MODE == MSG_USBPORT)
/*   ####Define Note Servo Part####
     NO_DEBUG_MSG                    0x00000000
 1.  SHOW_DEBUG_MSG                  0x00000001 //1. Use Msg Range 0x70,0x00,0x00~~0x70,0x00,0xFF
 2.  STATE_MACHINE_FLOW_MSG          0x00000002 //2. Use Msg Range 0x50,0x00,0x00~~0x50,0x01,0xFF
 3.  DEBUG_DISK_ID_MSG               0x00000004 //3. Use Msg Range 0x51,0x00,0x00~~0x51,0x5F,0xFF
 4.  DEBUG_AUTO_TUNING_MSG           0x00000008 //4. Use Msg Range 0x51,0x60,0x00~~0x51,0xFF,0xFF
 5.  DEBUG_KPOWER_MSG                0x00000010 //5. Use Msg Range 0x53,0x50,0x00~~0x53,0xFF,0xFF
 6.  DEBUG_SVO_SPD_MSG               0x00000020 //6. Use Msg Range 0x53,0x00,0x00~~0x53,0x13,0xFF
 7.  DEBUG_SEEK_MSG                  0x00000040 //7. Use Msg Range 0x53,0x40,0x00~~0x53,0x46,0xFF
 8.  DEBUG_VENDOR_WRITE_MSG          0x00000080 //8. Use Msg Range 0x55,0x00,0x00~~0x55,0xFF,0xFF
 9.  DEBUG_SEEK_STEP_DATA_MSG        0x00000100 //9. Use Msg Range 0x54,0x00,0x00~~0x54,0xFF,0xFF
 10. DEBUG_OPC_MSG                   0x00000200 //10.Use Msg Range 0x56,0x00,0x00~~0x56,0xFF,0xFF
 11. DEBUG_OFFLINE_CALIB_MSG         0x00000400 //11.Use Msg Range 0x53,0x80,0x00~~0x53,0x86,0xFF
 12. DEBUG_CD_WRITE_MSG              0x00000800 //12.Use Msg Range 0x57,0x00,0x00~~0x57,0xFF,0xFF
 13. DEBUG_DVD_WRITE_MSG             0x00001000 //13.Use Msg Range 0x58,0x00,0x00~~0x58,0xFF,0xFF
 14. DEBUG_WOBBLE_FLOW_MSG           0x00002000 //14.Use Msg Range 0x59,0x00,0x00~~0x59,0xFF,0xFF
 15. DEBUG_CALIB_ENTRYS_MSG          0x00004000 //15.Use Msg Range 0x5A,0x00,0x00~~0x5A,0xFF,0xFF
 16. DEBUG_CALIB_RESULT_MSG          0x00008000 //16.Use Msg Range 0x5B,0x00,0x00~~0x5B,0xFF,0xFF
 17. DEBUG_DSP_MSG                   0x00010000 //17.Use Msg Range 0x77,0x00,0x00~~0x77,0xFF,0xFF
 18. DEBUG_BD_CODEC_MSG              0x00020000 //18.Use Msg Range 0x5D,0x00,0x00~~0x5D,0xFF,0xFF
 19. DEBUG_SERVO_TRACE_MSG           0x00040000 //19.Use Msg Range 0x5E,0x00,0x00~~0x5E,0xFF,0xFF
 20. UART_COMMAND_TRACE_MSG          0x00080000 //20.Use Msg Range 0x5F,0x00,0x00~~0x5F,0xFF,0xFF
 21. DEBUG_DFE_MSG                   0x00100000 //21.Use Msg Range 0x60,0x00,0x00~~0x60,0xFF,0xFF
 22. DEBUG_WBL_MSG                   0x00200000 //22.Use Msg Range 0x61,0x00,0x00~~0x61,0xFF,0xFF
 23. DEBUG_COEF_MSG                  0x00400000 //23.Use Msg Range 0x62,0x00,0x00~~0x62,0xFF,0xFF
 24. DEBUG_LASER_MSG                 0x00800000 //24.Use Msg Range 0x63,0x00,0x00~~0x62,0xFF,0xFF
 25. HEADER_ID_MSG                   0x01000000 //25.Use Msg Range 0x5B,0x06,0x00~~0x5B,0x06,0x0F
 26. DEBUG_STREAM_MSG                0x02000000 //26.Use Msg Range 0x64,0x00,0x00~~0x64,0xFF,0xFF
 27. DEBUG_MATH2D_MSG                0x04000000 //27.Use Msg Range 0x65,0x00,0x00~~0x65,0xFF,0xFF
 28. DEBUG_DEMOD_MSG                 0x08000000 //28.Use Msg Range 0x66,0x00,0x00~~0x66,0xFF,0xFF
 29. DEBUG_BD_WRITE_MSG              0x10000000 //29.Use Msg Range 0x67,0x00,0x00~~0x67,0xFF,0xFF
 30. DEBUG_CUSTOMER_MSG1             0x20000000 //30.Use Msg Range 0x68,0x00,0x00~~0x68,0xFF,0xFF
 31. DEBUG_CUSTOMER_MSG2             0x40000000 //31.Use Msg Range 0x69,0x00,0x00~~0x69,0xFF,0xFF
*/
#if (SUN_FUNC_TEST == 1)
    MsgMarks =SWAPEND_ULONG((*(vULONG *)(FLASH_BASE + AGAIN_SL_OFFSET + OEM_AGING_SVO_MSG_MARKS_ADDR)));
#else
    MsgMarks =SWAPEND_ULONG((*(vULONG *)(FLASH_BASE + PARA_BLK_START_OFFSET + SVO_MSG_MARKS_ADDR)));
#endif

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
    set_svo_msg(MsgMarks); /* Only SHOW_DEBUG_MSG Debug Msg */
#endif
    //first piority
    open_svo_msg(SHOW_DEBUG_MSG|DEBUG_DISK_ID_MSG|DEBUG_CALIB_RESULT_MSG);
    //second piority
//    open_svo_msg(SHOW_DEBUG_MSG|STATE_MACHINE_FLOW_MSG|DEBUG_DISK_ID_MSG|DEBUG_SVO_SPD_MSG|DEBUG_SEEK_MSG|DEBUG_CALIB_RESULT_MSG);
    //third piority
//    open_svo_msg(SHOW_DEBUG_MSG|0x7FFFFFFF);


/***** HIF Message Types ******
 1.  SEND_ATAPI_CMD                  0x80000001  //1. Send ATA/ATAPI command
 2.  SEND_HIF_CMD                    0x80000002  //2. Use Msg Range 0xA0,0x00,0x00~~0xA0,0x0F,0x00
 3.  SEND_WRITE_MSG                  0x80000004  //3.
 4.  SEND_MRW_MSG                    0x80000008  //4. Use Msg Range 0xFD,0x00,0x00~~0xFD,0x03,0xFF
 5.  SEND_READ_DVD_MSG               0x80000010  //5.
 6.  SEND_DVD_MINUS_WR_MSG           0x80000020  //6. Send DVD-R Write message
 7.  SEND_SStpTask_MSG               0x80000040  //7. Use Msg Range 0xB0,0x00,0x00~~0xB0,0x02,0xFF
 8.  SEND_PlayTask_MSG               0x80000080  //8. Use Msg Range 0xB1,0x00,0x00~~0xB1,0x07,0xFF
 9.  SEND_RecTask_MSG                0x80000100  //9. Use Msg Range 0xB2,0x00,0x00~~0xB2,0x01,0xFF
 10. SEND_TrayTask_MSG               0x80000200  //10.Use Msg Range 0xB3,0x00,0x00~~0xB3,0x02,0xFF
 11. SEND_InfoTask_MSG               0x80000400  //11.Use Msg Range 0xB4,0x00,0x00~~0xB4,0x06,0xFF
 12. SEND_MiscTask_MSG               0x80000800  //12.Use Msg Range 0xB5,0x00,0x00~~0xB5,0x02,0xFF
 13. SEND_PlayerTask_MSG             0x80001000  //13.Use Msg Range 0xB7,0x00,0x00~~0xB7,0x0E,0xFF
 14. SEND_SvmnTask_MSG               0x80002000  //14.Use Msg Range 0xB8,0x00,0x00~~0xB8,0x01,0xFF
 15. SEND_OpcTask_MSG                0x80004000  //15.Use Msg Range 0xB9,0x00,0x00~~0xB9,0x01,0xFF
 16. SEND_RdTocTask_MSG              0x80008000  //16.Use Msg Range 0xBA,0x00,0x00~~0xBA,0x02,0xFF
 17. SEND_WATCH_MSG                  0x80010000  //17.Use Msg Range 0xD6,0x00,0x00~~0xD6,0x10,0xFF
 18. SEND_SRAM_EMPTY_STACK           0x80020000  //18.print sram empty stack size
 19. SEND_ATIP_WOBBLE                0x80040000  //19.print ATIP or ADIP/LPP
 20. SEND_QCODE_DATAID               0x80080000  //20.print Qcode or Data ID
 21. SEND_DVDPLUSRW_MSG              0x80100000  //21.Send DVD+RW format message. Range 0xBB,0x00,0x00~~0xBB,0x01,0xFF
 22. SEND_DVDRAM_ICE                 0x80200000  //22.print dvdram ice buffering debug message
*******************************/
#if (SUN_FUNC_TEST == 1)
    MsgMarks =SWAPEND_ULONG((*(vULONG *)(FLASH_BASE + AGAIN_SL_OFFSET + OEM_AGING_HIF_MSG_MARKS_ADDR)));
#else
    MsgMarks =SWAPEND_ULONG((*(vULONG *)(FLASH_BASE + PARA_BLK_START_OFFSET + HIF_MSG_MARKS_ADDR)));
#endif
    
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
    set_hif_msg(MsgMarks); /* No Any Debug Msg */
#endif
    //first piority 1,2,7
    open_hif_msg(SEND_ATAPI_CMD|SEND_SStpTask_MSG|SEND_HIF_CMD);
    //second piority 1,2,7,5,6
//    open_hif_msg(SEND_ATAPI_CMD|SEND_SStpTask_MSG|SEND_HIF_CMD|SEND_READ_DVD_MSG|SEND_DVD_MINUS_WR_MSG);
    //third piority all
//    open_hif_msg(SEND_ATAPI_CMD|0x803FFFFF);

//    set_hif_msg(0x803FFFFF); /* All HIF/Player Debug Messages */
#endif// (MSG_OUT_MODE == MSG_USBPORT)


#if  (BURN_IN == 1)
    Burn_In_Mode_Detection();
#endif

    InitializeARM();

#if (EN_FW_SIGNATURE == 1)
    if (StartFWSigChk() == TRUE)
    {
        send_msg5S(1,"FWSigOK");
    }
    else
    {
        send_msg5S(1,"NoFWSig");
    }
#endif

    send_msg80(1,0x710044);

#if (SEEPROM == 1)
    if (!CheckEEPROMSignature())
        {
    #if (ENABLE_OVERWRITE_EEPROM == 1)
            LoadSerialEEPROM();     // Update the SEEPROM with the new PersistantTable
    #else
            while (1);  /* Loop here!!!! This code is intended to catch an illegal write to the SEEPROM
                        You must enable the SEEPROM write when you are going to calibrate the drive. */
    #endif // ENABLE_OVERWRITE_EEPROM
        }
#endif // SEEPROM

    send_msg80(1,0x710045);

    for(i = 0;i < PROCESS_MAX;i++)
    {
        dwData=(i+1);
        dwData|=(dwData<<8);
        dwData|=dwData<<16;
        for(j = 0; j < (task_tbl[i].stack_size / 4); j++)
        {
            (task_tbl[i].stack_ptr)[j] = dwData;
        }
        MakeProcess(task_tbl[i].task_id_no,task_tbl[i].task_main,
                     task_tbl[i].stack_ptr,task_tbl[i].stack_size); /* Task entrey */
        InitStack(task_tbl[i].task_id_no,task_tbl[i].stack_ptr);    /* stack checker init */
    }
    send_msg80(1,0x710046);

    StartProcess(0);               /* Task start */
}


