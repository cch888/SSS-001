/****************************************************************************
*         (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   SCOMM.C
*
* DESCRIPTION: Serial Communications to external devices
*
*   $Revision: 180 $
*   $Date: 11/03/28 2:36p $
*
* NOTES:
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#define _SCOMM_C_

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\servo\SVOMACRO.H"
#include ".\servo\svo_com.h"
#include ".\servo\svo.h"
#include ".\common\scomm.h"
#include ".\oem\oem_tbl.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\common\string.h"
#include ".\servo\svo_afe.h"
#include ".\player\Plrdb.h"
#include ".\AL\HAL_dsp.h"
#include ".\al\reg_intctrl.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_uart.h"
#include ".\AL\REG_ssi.h"
#include ".\HIF\diag.h"
#if (BD_ENABLE == 1)
#include ".\bd\bdDB.h"
#endif

#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\hif\at_equ.h"
#endif

#if (MSG_OUT_MODE == MSG_USBPORT) && (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\ramdef.h"
#include ".\defmgmt\dvdram\dvDefMgr.h"
#endif
#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == SSI_INTERFACE)
#include ".\AL\REG_ssi.h"
#endif

#ifdef JTT
void JT_test(void);
#endif

void WriteSerialAddress(USHORT SAddress);
void SerialWriteComplete(void);
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
void s2bBUILD_SENSE(BYTE snskey, BYTE snscode, BYTE snsqual);
void SendAtapiDataToUart(ULONG Address, USHORT ByteCount);
#endif

#if (MSG_OUT_MODE == MSG_UART)
char msgBuf[SIZEOFMSGLINE];
#endif

#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
void send_msg2uart(char*,BYTE);
char msgBuf[SIZEOFMSGLINE];
#endif

#if (ENABLE_UART_RXD == 1)
BYTE rx_ring_buff[RX_BUF_SIZE];
#endif
char tx_ring_buff[TX_BUF_SIZE];

unsigned short  tx_buf_head, tx_buf_tail;

#if (ENABLE_UART_RXD == 1)
unsigned short  rx_buf_head, rx_buf_tail;
signed short    next_head;
BYTE    uart_rx_phase;
USHORT  rx_cmd_cnt;
USHORT  s2b_byte_received;
BOOL    s2b_cmd_received;
    #if (MSG_OUT_MODE == MSG_USBPORT)
BOOL    s2b_atapi_cmd_received;
BOOL    s2b_atapi_para_received;
BOOL    s2b_atapi_para_err;
    #endif
BYTE phase;
BYTE CmdByte[RX_BUF_SIZE];
BYTE get_s2b_cmd_done;
ULONG s2b_Addr;
ULONG s2b_Data;
BYTE  s2b_Mode;
ULONG  seek_Addr;
BYTE cmd_mode;
StLongU s2b_HifMsg;
StLongU s2b_SvoMsg;
#else
signed short    rx_buf_head, rx_buf_tail, next_head;
#endif

#if (ENABLE_UART_RXD == 1) && (MSG_OUT_MODE == MSG_USBPORT)
extern BYTE const EndOfFirmWare[16];
#endif

#if (MSG_OUT_MODE == MSG_USBPORT)
#define MaxHifNumber    21
#define MaxSvoNumber    25
#endif

//*******************************************************************;
// init_UART()
// Initialize UART                                                                  ;
//*******************************************************************;
void init_UART(void)
{
    /* Initialize FIFO Control Register */
    BYTE MyBuf;

    // receive FIFO reset, FIFO enable
    WRITE_FIELDTO(MyBuf, HAL_APB_UART_RXTRIG, halAPB_UART_RXTRIGTriLv_1char);
    WRITE_FIELDTO(MyBuf, HAL_APB_UART_RXFIFORST,1);
    WRITE_FIELDTO(MyBuf, HAL_APB_UART_FIFOEN,1);
    WRITE_REG(HAL_APB_UARTREG_MAP->UARTIIR, MyBuf);

    WRITE_FIELD(HAL_APB_UART_DLAB, 1);                  // Enable Divisor loading

    WRITE_FIELD(HAL_APB_UART_UARTDLL,BAUD_DIV&0xFF);     //Load Divisor Low Byte
    WRITE_FIELD(HAL_APB_UART_UARTDLH,BAUD_DIV>>8);       //Load Divisor High Byte

    /*** Setup UART Line control ***/
    /*** unselect divisor latch register access, no break control,
    disable parity bit, odd number parity bit, and 1 stop bit ***/
#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE) && (DEBUG_BOARD_TYPE==VENDOR1_DEBUG_BOARD)
    READ_MOD_WRITE(HAL_APB_UARTREG_MAP->UARTLCR, (HAL_APB_UART_DLAB_MASK | HAL_APB_UART_UBREAK_MASK | HAL_APB_UART_EPS_MASK | HAL_APB_UART_PEN_MASK | HAL_APB_UART_STOPBITS_MASK),4); // 2 stop bits
#else
    READ_MOD_WRITE(HAL_APB_UARTREG_MAP->UARTLCR, (HAL_APB_UART_DLAB_MASK | HAL_APB_UART_UBREAK_MASK | HAL_APB_UART_EPS_MASK | HAL_APB_UART_PEN_MASK | HAL_APB_UART_STOPBITS_MASK),0);
#endif
    WRITE_FIELD(HAL_APB_UART_CLS, 3);       // Seletct 8 data bits

    /* Initialize Interrupt Enable Register */
    WRITE_FIELD(HAL_APB_UART_ELSI,0);
    WRITE_FIELD(HAL_APB_UART_ETBEI,0);
    WRITE_FIELD(HAL_APB_UART_ERBFI,0);

#if (ENABLE_UART_TXD == 1)
    WRITE_FIELD(HAL_APB_UART_ETBEI, 1);             // enable transmitter holding register empty interrupt
#endif

#if (ENABLE_UART_RXD == 1)
    WRITE_FIELD(HAL_APB_UART_ERBFI,1);              // enable received data available interrupt
#endif

    /* Reset all pointer and data number in buffer */
    tx_buf_head     = 0;
    tx_buf_tail     = tx_buf_head;                  // make the head and tail at the same location --> emty
    rx_buf_head     = 0;
#if (ENABLE_UART_RXD == 1)
    s2b_byte_received = 0;
    s2b_cmd_received = FALSE;
    #if (MSG_OUT_MODE == MSG_USBPORT)
    s2b_atapi_cmd_received = FALSE;
    s2b_atapi_para_received = FALSE;
    s2b_atapi_para_err = FALSE;
    #endif
    uart_rx_phase = 0;
    rx_cmd_cnt = 0;
    phase = 0;
    get_s2b_cmd_done = FALSE;
    cmd_mode = 0;

    rx_buf_tail     = rx_buf_head;                  // make the head and tail at the same location --> emty
#else
    rx_buf_tail     = rx_buf_head - 1;              // make the head and tail at the same location --> emty
#endif

    ENABLE_UART_INT();
}

#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == SSI_INTERFACE)
/************************************************************************
* Module: init_ssi()
* Description:  Initialize SSI interface
* Returns:
* Params:
*************************************************************************/
void init_ssi(void)
{
    /* Initialize SSI interface */

    /* Baud rate setting */
    WRITE_REG(HAL_APB_SSIREG_MAP->SSICLKDIV,bSSICLKDIV);            /* Clock divider */
         WRITE_REG(HAL_APB_SSIREG_MAP->SSICFG0,HAL_APB_SSI_SSCLKINV);   /* Configure communications Format Invert clock on SSI Interface */
         WRITE_REG(HAL_APB_SSIREG_MAP->SSIINTEN,0);                     /* disable all the interupts */
         WRITE_REG(HAL_APB_SSIREG_MAP->SSIINT,0);                       /* Serial Channel Interrupt Clear */

#if (CHIP_REV >= 0xC0)
    #if (DEBUG_BOARD_TYPE==SUNEXT_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SMODE2, 0);
    #elif (DEBUG_BOARD_TYPE == VENDOR1_DEBUG_BOARD)
    WRITE_FIELD(HAL_APB_SSI_SMODE2, 1);
    #endif
#endif

}
#endif

BOOL   uart_ring_buf_and_fifo_empty;
//*******************************************************************;
//  UART_isr
// Description: UART Interrupt Service Routine                                                                 ;
//*******************************************************************;
void UART_isr(void)
{
    unsigned short   FIFO_space;
    BYTE             irq_id;
#if (ENABLE_UART_RXD == 1) && (MSG_OUT_MODE == MSG_USBPORT)
    USHORT           i;
    BYTE             checksum;
#endif

    irq_id  = READ_FIELD(HAL_APB_UART_UINTID);     // read irq iD, and also, reset interrupt

    switch (irq_id)
    {
        //--------------------------------------------------------;
        //                                                        ;
        //--------------------------------------------------------;
        case halAPB_UART_PriLv_2ndData:
#if (ENABLE_UART_RXD == 1)
    #if (MSG_OUT_MODE == MSG_USBPORT)
            rx_ring_buff[rx_buf_head] = READ_REG(HAL_APB_UARTREG_MAP->UARTRBR);

            switch(uart_rx_phase)
            {
                case RX_IDLE_MODE:
                        if (rx_ring_buff[rx_buf_head] == 0xF1 || rx_ring_buff[rx_buf_head] == 0xF2 || rx_ring_buff[rx_buf_head] == 0xF3)
                        {
                            BYTE temp;
                            temp = rx_ring_buff[rx_buf_head];
                            rx_buf_head = 0;
                            rx_ring_buff[rx_buf_head] = temp;
                            rx_cmd_cnt = 0;
                            s2b_byte_received = 1;
                            uart_rx_phase = RX_GET_SYNC_MODE;
                            rx_buf_head++;
                        }
                        break;

                case RX_GET_SYNC_MODE:
                        if (rx_ring_buff[0] == 0xF3)
                        {
                            if (rx_buf_head == 1)
                            {
                                rx_cmd_cnt = rx_ring_buff[rx_buf_head];
                                uart_rx_phase = RX_GET_SYNC_MODE;
                            }
                            else
                            {
                                rx_cmd_cnt = (rx_cmd_cnt << 8);
                                rx_cmd_cnt += (USHORT)rx_ring_buff[rx_buf_head];
                                uart_rx_phase = RX_GET_CNT_MODE;
                            }
                            rx_buf_head++;
                            s2b_byte_received++;
                        }
                        else
                        {
                            rx_cmd_cnt = rx_ring_buff[rx_buf_head];
                            rx_buf_head++;
                            s2b_byte_received++;
                            uart_rx_phase = RX_GET_CNT_MODE;
                        }
                        break;

                case RX_GET_CNT_MODE:
                        if (rx_cmd_cnt==0)
                        {
                            rx_buf_head = 0;
                            s2b_byte_received++;
                            uart_rx_phase = RX_IDLE_MODE;

                            if (rx_ring_buff[rx_buf_head] == 0xF1)
                            {
                                if ((rx_ring_buff[2] == 'R')&&(rx_ring_buff[3] == 'E')&&(rx_ring_buff[4] == 'S')&&
                                    (rx_ring_buff[5] == 'E')&&(rx_ring_buff[6] == 'T'))
                                {
                                    checksum = 0;
                                    rx_cmd_cnt = (USHORT)rx_ring_buff[1];
                                    if (s2b_byte_received <= RX_BUF_SIZE && s2b_byte_received > 0)
                                    {
                                        for (i=0;i<=rx_cmd_cnt;i++)
                                        {
                                            checksum=checksum^rx_ring_buff[i+1];
                                        }

                                        if (checksum != rx_ring_buff[i+1])
                                        {
                                            s2bBUILD_SENSE(0xFF,0x08,0x02);
                                            return;
                                        }

                                        s2b_atapi_cmd_received = FALSE;
                                        Knl_cmd_avail = FALSE;
                                        for (i=0;i<s2b_byte_received;i++)
                                            rx_ring_buff[i] = ' ';

                                        ATAPIDeviceResetCommand();
                                        return;
                                    }
                                    else
                                    {
                                        s2bBUILD_SENSE(0xFF,0x08,0x02);
                                        return;
                                    }
                                }
                                s2b_cmd_received = TRUE;
                            }
                            else if (rx_ring_buff[rx_buf_head] == 0xF2)
                            {
                                checksum = 0;
                                rx_cmd_cnt = (USHORT)rx_ring_buff[1];
                                if (s2b_byte_received <= RX_BUF_SIZE && s2b_byte_received > 0)
                                {
                                    for (i=0;i<=rx_cmd_cnt;i++)
                                    {
                                        checksum=checksum^rx_ring_buff[i+1];
                                    }

                                    if (checksum != rx_ring_buff[i+1])
                                    {
                                        s2bBUILD_SENSE(0xFF,0x08,0x02);
                                        return;
                                    }

                                    if (Knl_cmd_avail == TRUE || Knl_atcmd_avail == TRUE || St_cmd_inproc == TRUE)
                                    {
                                        s2bBUILD_SENSE(0x02,0x04,0x01);
                                        return;
                                    }

                                    s2b_atapi_cmd_received = TRUE;
                                    DISABLE_HINT();

                                    for (i=0; i<12; i++)
                                        Iram_Cmd_block.bf[i] = rx_ring_buff[i+2];

                                        StartTimer(&Iram_atapi_timer);

                                    /*For doing offline power calibration command do not check Unit Attention to execute this vendor command*/
        #if (SUN_FUNC_TEST == 1)
                                    if(((Iram_Cmd_block.bf[0] == DIAG_CMD_OFFLINE_CALIBRATION) || (Iram_Cmd_block.bf[0] == DIAG_LASER)) && (St_unit_atten == TRUE))
        #else
                                    if ((Iram_Cmd_block.bf[0] == DIAG_CMD_OFFLINE_CALIBRATION) && (St_unit_atten == TRUE))
        #endif        
                                        St_unit_atten = FALSE;

                                    if (Iram_Cmd_block.bf[0] == MMC_MECHANISM_STATUS)
                                    {
                                        MechanismStatusCommand();
                                        return;
                                    }
                                    else if ( Iram_Cmd_block.bf[0] == MMC_GET_EVENT_STATUS_NOTIFICATION )
                                    {
                                        GetEventStatusNotificationCommand();
                                        return;
                                    }
                                    else if( Iram_Cmd_block.bf[0] == MMC_GET_CONFIGURATION)
                                    {
                                        GetConfigurationCommand();
                                        return;
                                    }
        #ifdef READ10_SSI  // Read10 command support on SSI port
                                    else if ( Iram_Cmd_block.bf[0] == SBC_VERIFY_10 || Iram_Cmd_block.bf[0] == SBC_VERIFY_12 ||
                                              Iram_Cmd_block.bf[0] == MMC_WRITE_10 || Iram_Cmd_block.bf[0] == MMC_WRITE_12 ||
                                              Iram_Cmd_block.bf[0] == MMC_WRITE_VERIFY_10)
        #else
                                    else if ( Iram_Cmd_block.bf[0] == SBC_READ_10 || Iram_Cmd_block.bf[0] == SBC_READ_12 ||
                                              Iram_Cmd_block.bf[0] == SBC_VERIFY_10 || Iram_Cmd_block.bf[0] == SBC_VERIFY_12 ||
                                              Iram_Cmd_block.bf[0] == MMC_WRITE_10 || Iram_Cmd_block.bf[0] == MMC_WRITE_12 ||
                                              Iram_Cmd_block.bf[0] == MMC_READ_CD || Iram_Cmd_block.bf[0] == MMC_READ_CD_MSF ||
                                              Iram_Cmd_block.bf[0] == MMC_WRITE_VERIFY_10)
        #endif
                                    {
                                        BUILD_SENSE(0x05,0x20,0x00,0x10);
                                        AutoCommandComplete();
                                        return;
                                    }

                                    Knl_cmd_avail = TRUE;
                                }
                                else
                                {
                                    s2bBUILD_SENSE(0xFF,0x08,0x02);
                                    return;
                                }
                            }
                            else if (rx_ring_buff[rx_buf_head] == 0xF3)
                            {
                                checksum = 0;
                                rx_cmd_cnt = ((USHORT)rx_ring_buff[1] << 8) + (USHORT)rx_ring_buff[2];
                                if (s2b_byte_received <= RX_BUF_SIZE && s2b_byte_received > 0)
                                {
                                    for (i=0;i<=(rx_cmd_cnt+1);i++)
                                    {
                                        checksum=checksum^rx_ring_buff[i+1];
                                    }

                                    if (checksum != (BYTE)rx_ring_buff[i+1])
                                    {
                                        s2b_atapi_para_err = TRUE;
                                    }
                                }
                                else
                                    s2b_atapi_para_err = TRUE;

                                if (s2b_atapi_para_err == TRUE)
                                {
                                    for (i=0;i<s2b_byte_received;i++)
                                        rx_ring_buff[i] = ' ';

                                    s2b_byte_received = 0;
                                    s2b_atapi_para_received = FALSE;
                                }
                                else
                                    s2b_atapi_para_received = TRUE;
                            }
                        }
                        else
                        {
                            rx_buf_head++;
                            s2b_byte_received++;
                            rx_cmd_cnt--;
                        }
                        break;
            }
    #else // #if (MSG_OUT_MODE == MSG_USBPORT)
            rx_ring_buff[rx_buf_head] = READ_REG(HAL_APB_UARTREG_MAP->UARTRBR);
            WRITE_FIELD(HAL_APB_UART_UARTTHR, rx_ring_buff[rx_buf_head]);

            if(rx_ring_buff[rx_buf_head] == '\r')
            {
                WRITE_FIELD(HAL_APB_UART_UARTTHR, 0x0A);    //new line
                rx_buf_head = 0;
                s2b_cmd_received = TRUE;
            }
            else if(rx_ring_buff[rx_buf_head] == 0x08)//backspace
            {
                WRITE_FIELD(HAL_APB_UART_UARTTHR, 0x20);    // space
                WRITE_FIELD(HAL_APB_UART_UARTTHR, 0x08);    // backspace
                rx_buf_head--;
                if(rx_buf_head == 0)
                    rx_buf_head = 0;

                s2b_byte_received--;
                if(s2b_byte_received == 0)
                    s2b_byte_received = 0;
            }
            else
            {
                rx_buf_head++;
                s2b_byte_received++;
            }
    #endif // #if (MSG_OUT_MODE == MSG_USBPORT)
#endif // #if (ENABLE_UART_RXD == 1)
            break;

        case halAPB_UART_PriLv_2ndTout:
            irq_id  = READ_REG(HAL_APB_UARTREG_MAP->UARTRBR);   // read UART Receive Buffer Register to reset interrupt
            break;
        //--------------------------------------------------------;
        //                                                        ;
        //--------------------------------------------------------;
        case halAPB_UART_PriLv_1st:
            irq_id  = READ_REG(HAL_APB_UARTREG_MAP->UARTLSR);  // read UART Line Status Register to reset interrupt
            break;

        //--------------------------------------------------------;
        //                                                        ;
        //--------------------------------------------------------;
        case halAPB_UART_PriLv_3rd:
            //-------------------------------------------------------------------;
            // stop sending to HW FIFO if                                        ;
            //     1) ring buffer tail catchs the head  (get a empty butter)     ;
            // or  2) HW fifo is full                                            ;
            //                                                                   ;
            //                                                                   ;
            // head ---> next avaliable byte                                     ;
            // tail ---> the byte store the oldest char                          ;
            //                                                                   ;
            // when      head == tail, ring buffer is empty                      ;
            // when next head == tail, ring buffer is full                       ;
            //-------------------------------------------------------------------;
            FIFO_space = HW_UART_FIFO_SISE;

            if (tx_buf_head == tx_buf_tail)
            {
                // ring buffer empty and FIFO empty
                uart_ring_buf_and_fifo_empty = TRUE;
                return;
            }

            while(1)
            {
                if (tx_buf_head == tx_buf_tail)
                {
                    // ring buffer empty and FIFO empty
                    // uart_ring_buf_and_fifo_empty = TRUE;
                    break;
                }

                if (FIFO_space == 0)
                {
                    // FIFO is full
                    break;   // go out and come back here when FIFO emtpy irq happen
                }

                // move chars into FIFO, set "uart_ring_buf_and_fifo_empty" to FALSE;
                WRITE_FIELD(HAL_APB_UART_UARTTHR, tx_ring_buff[tx_buf_tail++]);
                tx_buf_tail %= TX_BUF_SIZE;
                FIFO_space  -= 1;
                uart_ring_buf_and_fifo_empty = FALSE;
            }// while(1)
    }//switch
}//UART_isr

#if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)
void send_msg2uart(char *message,BYTE cnt)
{
    ULONG          temp_IE;
    USHORT         NextHead;
    BYTE           Length, Count;
    BYTE           KickedOff;

    KickedOff = FALSE;
    Length = cnt;

    for (Count = 0; Count < Length; Count++)
    {
        NextHead = ((tx_buf_head + 1) % TX_BUF_SIZE);

        //------------------------------------------------------------------------;
        // always wait if the ring buffer is full, tx_buf_tail is updated by isr  ;
        //------------------------------------------------------------------------;
        while(tx_buf_tail == NextHead)
        {
            return;              // dump the rest of the char's once ring buffer is full. don't wait.
        }
        //-----------------------------------------;
        // ring buffer is not full                 ;
        //-----------------------------------------;
        temp_IE                   = GET_INT_ENABLE_MASK();    /* disable interrupts */
        DISABLE_INTERRUPT();
        tx_ring_buff[tx_buf_head] = message[Count];
        tx_buf_head               = NextHead;
        if ((uart_ring_buf_and_fifo_empty == TRUE) && (KickedOff == FALSE))
        {
            // Kick off first character.
            // FIFO is empty, there is no ISR to kick the 1st char off.
            // Don't worry about the uart_ring_buf_and_fifo_empty,
            // the ISR will clear it once FIFO is empty
            WRITE_FIELD(HAL_APB_UART_UARTTHR, tx_ring_buff[tx_buf_tail++]);
            tx_buf_tail %= TX_BUF_SIZE;
            KickedOff = TRUE;
        }
        SET_INT_ENABLE_MASK(temp_IE);   /* restore interrupts */
    }// for Count ...

}// end of UART_out_string
#endif // #if (MSG_OUT_MODE == MSG_USBPORT) && (DEBUG_PORT_OUTPUT_MODE == UART_INTERFACE)

#if (MSG_OUT_MODE == MSG_UART)
//*******************************************************************;
//  UART_out_string                                                  ;
// Description: Output a string on UART
// head ---> next avaliable byte                                     ;
// tail ---> the byte store the oldest char                          ;
//                                                                   ;
// when      head == tail, ring buffer is empty                      ;
// when next head == tail, ring buffer is full                       ;
//                                                                   ;
//*******************************************************************;
void UART_out_string(char *message)
{
    ULONG          temp_IE;
    USHORT         NextHead;
    BYTE           Length, Count;
    BYTE           KickedOff;

    KickedOff = FALSE;
    Length = strlen(message);
    for (Count = 0; Count < Length; Count++)
    {
        NextHead = ((tx_buf_head + 1) % TX_BUF_SIZE);

        //------------------------------------------------------------------------;
        // always wait if the ring buffer is full, tx_buf_tail is updated by isr  ;
        //------------------------------------------------------------------------;
        while(tx_buf_tail == NextHead)
            return;              // dump the rest of the char's once ring buffer is full. don't wait.

        //-----------------------------------------;
        // ring buffer is not full                 ;
        //-----------------------------------------;
        temp_IE                   = GET_INT_ENABLE_MASK();    /* disable interrupts */
        DISABLE_INTERRUPT();
        tx_ring_buff[tx_buf_head] = message[Count];
        tx_buf_head               = NextHead;
        if ((uart_ring_buf_and_fifo_empty == TRUE) && (KickedOff == FALSE))
        {
            // Kick off first character.
            // FIFO is empty, there is no ISR to kick the 1st char off.
            // Don't worry about the uart_ring_buf_and_fifo_empty,
            // the ISR will clear it once FIFO is empty
            WRITE_FIELD(HAL_APB_UART_UARTTHR, tx_ring_buff[tx_buf_tail++]);
            tx_buf_tail %= TX_BUF_SIZE;
            KickedOff = TRUE;
        }
        SET_INT_ENABLE_MASK(temp_IE);   /* restore interrupts */
    }// for Count ...

}// end of UART_out_string



#if (ENABLE_UART_TXD == 1)

//*******************************************************************;
//  UART_out_0xVal
// Description:                                                      ;
//*******************************************************************;
void UART_out_0xVal(USHORT x)
{
    USHORT   img_x, tmp;
    SBYTE    i, j;
    char     buf[10];

    img_x = x;

    j        = 0;
    buf[j++] = (char)0x30;
    buf[j++] = (char)0x78;
    for (i = 12; i >= 0; i -= 4)
    {
        tmp = ((img_x >> i) & 0x000F);         // look at 4 bit at a time
        if (tmp <= 9)
            buf[j++] = (char)(tmp + 0x0030);   // ch = 0, 1, ... , 9
        else
            buf[j++] = (char)(tmp + 0x0037);   // ch = A, B, ... , F
    }
    buf[j++] = (char)0x00;   // adding null
    UART_out_string(buf);
}// end of UART_out_0xVal

//*******************************************************************;
//  UART_out_Val
// Description:                                                      ;
//*******************************************************************;
void UART_out_Val(BYTE x)
{
    BYTE     img_x, tmp;
    SBYTE    i, j;
    char     buf[10];

    img_x = x;

    j        = 0;
    //buf[j++] = (char)0x30;
    //buf[j++] = (char)0x78;
    for (i = 4; i >= 0; i -= 4)
    {
        tmp = ((img_x >> i) & 0x0F);         // look at 4 bit at a time
        if (tmp <= 9)
            buf[j++] = (char)(tmp + 0x30);   // ch = 0, 1, ... , 9
        else
            buf[j++] = (char)(tmp + 0x37);   // ch = A, B, ... , F
    }
    buf[j++] = (char)0x00;   // adding null
    UART_out_string(buf);
}// end of UART_out_0xVal

/*******************************************************************************

    FUNCTION        UARTLongOut

    DESCRIPTION     Converts a LONG value to ASCII characters and
                    sends out through UART

    PARAMETERS      v - ULONG value to be sent to UART
********************************************************************************/
void UARTLongOut(ULONG v)
{
    USHORT   tmp;
    SBYTE    i, j;
    char     buf[10];

    j        = 0;               // Initialize buffer count
    buf[j++] = (char)0x30;      // ASCII 0
    buf[j++] = (char)0x78;      // ASCII x

    for (i = 28; i >= 0; i -= 4)
    {
        tmp = (( v >> i) & 0x0000000F);        // look at 4 bit at a time
        if (tmp <= 9)
            buf[j++] = (char)(tmp + 0x0030);   // ch = 0, 1, ... , 9
        else
            buf[j++] = (char)(tmp + 0x0037);   // ch = A, B, ... , F
    }
    buf[j++] = (char)0x00;   // adding null
    UART_out_string(buf);
}
#endif // #if (ENABLE_UART_TXD == 1)

#endif //MSG_OUT_MODE == MSG_UART

#if (ENABLE_UART_RXD == 1)
/*******************************************************************************

    FUNCTION        ASC2DEC

    DESCRIPTION     Convert ASCII to DEC

********************************************************************************/
ULONG ASC2DEC(BYTE *ASC_CODE, BYTE length)
{
    BYTE    i;
    ULONG   data_temp;
    BYTE    len = length;
    ULONG   data=0x0000;

    for(i=len;i>0;i--)
    {
        data_temp = *(ASC_CODE + len - i);
        if(data_temp <= 57)
        {
            data_temp = data_temp - 48;
        }
        else if(data_temp <= 70)
        {
            data_temp = data_temp - 55;
        }
        else
        {
            data_temp = data_temp - 87;
        }

        if(i == 1)
            data += data_temp;
        else if(i == 2)
            data += (data_temp*16);
        else if(i == 3)
            data += (data_temp*256);
        else if(i == 4)
            data += (data_temp*4096);
        else if(i == 5)
            data += (data_temp*4096*16);
        else if(i == 6)
            data += (data_temp*4096*16*16);
    }

    return(data);
}


/*******************************************************************************

    FUNCTION        ASC2HEX

    DESCRIPTION     Convert ASCII to HEX

********************************************************************************/
ULONG ASC2HEX(BYTE *ASC_CODE, BYTE length)
{
    ULONG   data = 0x00L;
    BYTE    *ePtr;
    BYTE    str[11], i;

    for(i = 0; i < length; i++)
    {
        str[i] = *(ASC_CODE + i);
    }
    str[i] = 0x00;

    data = strtoul(str, &ePtr, 16);

    return(data);
}


/*******************************************************************************

    FUNCTION        s2bCmdDecoder

    DESCRIPTION     Decoder UART command

********************************************************************************/
void s2bCmdDecoder(void)
{
    BYTE i;
    #if (MSG_OUT_MODE == MSG_USBPORT)
    BYTE checksum;
    #endif

    switch (phase)
    {
        case 0: //get command set
    #if (MSG_OUT_MODE == MSG_USBPORT)
            if ((s2b_byte_received <=RX_BUF_SIZE)&&(s2b_byte_received > 0))
            {
                for (i = 0;i < s2b_byte_received; i++)
                {
                    CmdByte[i] = rx_ring_buff[i];
                    rx_ring_buff[i]=' ';
                }
                phase++;
            }
            else
            {
                s2b_byte_received = 0;
                s2b_cmd_received = FALSE;
                phase = 0;
            }

    #else // #if (MSG_OUT_MODE == MSG_USBPORT)
            if ((s2b_byte_received <=RX_BUF_SIZE)&&((s2b_byte_received== 9)||(s2b_byte_received== 6)||
                                                     (s2b_byte_received==10)||(s2b_byte_received== 5)))
            {
                for (i = 0;i < s2b_byte_received; i++)
                {
                       CmdByte[i] = rx_ring_buff[i];
                       rx_ring_buff[i]=' ';
                }
                phase++;
            }
            else
            {
                s2b_byte_received = 0;
                s2b_cmd_received = FALSE;
                phase = 0;
            }
    #endif // #if (MSG_OUT_MODE == MSG_USBPORT)
            break;

        case 1: // check sync
    #if (MSG_OUT_MODE == MSG_USBPORT)
            checksum = 0;
            if (CmdByte[0] == 0xF1)
            {
                for (i=0;i<=CmdByte[1];i++)
                {
                    checksum=checksum^CmdByte[i+1];
                }

                if (checksum!=CmdByte[i+1])
                {
                    cmd_mode = 0;
                    s2b_byte_received = 0;
                    s2b_cmd_received = FALSE;
                    phase = 0;
                    break;
                }
            }
            else
            {
                cmd_mode = 0;
                s2b_byte_received = 0;
                s2b_cmd_received = FALSE;
                phase = 0;
                break;
            }

        #if (REGISTER_MONITOR == 1)
            if(((CmdByte[2] == 0x01)&&(CmdByte[3] == 0x00))||
               ((CmdByte[2] == 0x01)&&(CmdByte[3] == 0xFF)))
            {

                fMonitoringReg = FALSE;
                if((CmdByte[2] == 0x01)&&(CmdByte[3] == 0xFF))
                {
                    HBYTE(HWORD(gMonitorReg[gTotalMonitorCH].monitor_reg_addr)) = CmdByte[4];
                    LBYTE(HWORD(gMonitorReg[gTotalMonitorCH].monitor_reg_addr)) = CmdByte[5];
                    HBYTE(LWORD(gMonitorReg[gTotalMonitorCH].monitor_reg_addr)) = CmdByte[6];
                    LBYTE(LWORD(gMonitorReg[gTotalMonitorCH].monitor_reg_addr)) = CmdByte[7];
                    gMonitorReg[gTotalMonitorCH].length                         = CmdByte[8];
                    gMonitorReg[gTotalMonitorCH].start_bit                      = CmdByte[9];
                    gMonitorReg[gTotalMonitorCH].end_bit                        = CmdByte[10];
                    gTotalMonitorCH++;
                }
                else
                {
                    gTotalMonitorCH= 0;
                }

                SendMsgRegMonitorCmdReady();

                get_s2b_cmd_done = TRUE;
                s2b_cmd_received = FALSE;
                s2b_byte_received = 0;
                phase = 0;
                return;
            }
            else if((CmdByte[2] == 0x01)&&(CmdByte[3] == 0x80))
            {
                fMonitoringReg = TRUE;
                get_s2b_cmd_done = TRUE;
                s2b_cmd_received = FALSE;
                s2b_byte_received = 0;
                phase = 0;
                return;
            }
        #endif // #if (REGISTER_MONITOR == 1)

            if((CmdByte[2] == 'N')&&(CmdByte[3] == 'R'))
            {
                cmd_mode = READ_BYTE_CMD;
                SendMsg80(UART_COMMAND_TRACE_MSG,0x3F0100+READ_BYTE_CMD); // READ_BYTE_CMD
                phase++;
            }
            else if((CmdByte[2] == 'N')&&(CmdByte[3] == 'W'))
            {
                cmd_mode = WRITE_BYTE_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'E')&&(CmdByte[3] == 'R'))
            {
                cmd_mode = READ_EQ_WORD_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'E')&&(CmdByte[3] == 'W'))
            {
                cmd_mode = WRITE_EQ_WORD_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'O')&&(CmdByte[3] == 'M'))
            {
                cmd_mode = OPEN_MSG_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'C')&&(CmdByte[3] == 'M'))
            {
                cmd_mode = CLOSE_MSG_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'E')&&(CmdByte[3] == 'K'))
            {
                cmd_mode = EJECT_KEY;
                phase++;
            }
            else if((CmdByte[2] == 'G')&&(CmdByte[3] == 'T'))
            {
                cmd_mode = GET_TEMPERATURE;
                phase++;
                SendMsgCn(SHOW_DEBUG_MSG,1,0x38001D,GetOpuTemperature());
            }
            else if((CmdByte[2] == 'S')&&(CmdByte[3] == 'D'))
            {
                cmd_mode = POWER_DOWN;
                phase++;
            }
            else if((CmdByte[2] == 'S')&&(CmdByte[3] == 'P'))
            {
                cmd_mode = SPEED_CHANGE;
                phase++;
            }
            else if((CmdByte[2] == 'D')&&(CmdByte[3] == 'C'))
            {
                cmd_mode = DC_ERASE;
                phase++;
            }
            else if((CmdByte[2] == 'D')&&(CmdByte[3] == 'I'))
            {
                cmd_mode = SEND_MID_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'D')&&(CmdByte[3] == 'E'))
            {
                cmd_mode = DUMP_EEPROM_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'S')&&(CmdByte[3] == 'J'))
            {
                cmd_mode = SHORT_JUMP;
                phase++;
            }
            else if((CmdByte[2] == 'R')&&(CmdByte[3] == 'J'))
            {
                cmd_mode = ROUGH_JUMP;
                phase++;
            }
            else if((CmdByte[2] == 'S')&&(CmdByte[3] == 'T'))
            {
                cmd_mode = SEEK_TARGET;
                phase++;
            }
            else if((CmdByte[2] == 'Q')&&(CmdByte[3] == 'T'))
            {
                cmd_mode = QUIT_TRAP;
                phase++;
            }
            else if((CmdByte[2] == 'J')&&(CmdByte[3] == 'T'))
            {
                cmd_mode = JT_TEST;
                phase++;
            }
            else if ((CmdByte[2] == 'D')&&(CmdByte[3] == 'S')&&(CmdByte[4] == 'P'))
            {
                cmd_mode = SERVO_DSP_DEBUG;
                phase++;
            }
            else if((CmdByte[2] == 'L')&&(CmdByte[3] == 'P'))
            {
                cmd_mode = LASC_PRESET_POWER;
                phase++;
            }
            else if((CmdByte[2] == 'L')&&(CmdByte[3] == 'I'))
            {
                cmd_mode = LASC_INCR_POWER;
                phase++;
            }
            else if((CmdByte[2] == 'L')&&(CmdByte[3] == 'D'))
            {
                cmd_mode = LASC_DECR_POWER;
                phase++;
            }
            else if((CmdByte[2] == 'L')&&(CmdByte[3] == 'S'))
            {
                cmd_mode = LASC_SAVE_TO_PERSIST;
                phase++;
            }
            else if((CmdByte[2] == 'L')&&(CmdByte[3] == 'G'))
            {
                cmd_mode = LASC_READ_FROM_PERSIST;
                phase++;
            }
            else if((CmdByte[2] == 'M')&&(CmdByte[3] == 'R'))
            {
                cmd_mode = BATCH_GET_MSG_CMD;
                phase++;
            }
            else if((CmdByte[2] == 'M')&&(CmdByte[3] == 'W'))
            {
                cmd_mode = BATCH_SET_MSG_CMD;
                phase++;
            }
            else
            {
                cmd_mode = 0;
                s2b_byte_received = 0;
                s2b_cmd_received = FALSE;
                phase = 0;
            }
    #else // #if (MSG_OUT_MODE == MSG_USBPORT)
            if ( ((CmdByte[0] == 'A')&&(CmdByte[1] == 'A')&&(CmdByte[2] == ' '))||
                ((CmdByte[0] == 'a')&&(CmdByte[1] == 'a')&&(CmdByte[2] == ' ')) )
            {
                cmd_mode = WRITE_BYTE_CMD;
                phase++;
            }
            else if( ((CmdByte[0] == 'B')&&(CmdByte[1] == 'B')&&(CmdByte[2] == ' '))||
                     ((CmdByte[0] == 'b')&&(CmdByte[1] == 'b')&&(CmdByte[2] == ' ')) )
            {
                cmd_mode = READ_BYTE_CMD;
                phase++;
            }
            else if( ((CmdByte[0] == 'C')&&(CmdByte[1] == 'C')&&(CmdByte[2] == ' '))||
                     ((CmdByte[0] == 'c')&&(CmdByte[1] == 'c')&&(CmdByte[2] == ' ')) )
            {
                cmd_mode = WRITE_EQ_WORD_CMD;
                phase++;
            }
            else if( ((CmdByte[0] == 'D')&&(CmdByte[1] == 'D')&&(CmdByte[2] == ' '))||
                     ((CmdByte[0] == 'd')&&(CmdByte[1] == 'd')&&(CmdByte[2] == ' ')) )
            {
                cmd_mode = READ_EQ_WORD_CMD;
                phase++;
            }
            else if( ((CmdByte[0] == 's')||(CmdByte[0] == 'S'))&&((CmdByte[1] == 's')||(CmdByte[1] == 'S'))
                    &&(CmdByte[2] == ' '))//short jump
            {
                cmd_mode = SHORT_JUMP;
                phase++;
            }
            else if( ((CmdByte[0] == 's')||(CmdByte[0] == 'S'))&&((CmdByte[1] == 'r')||(CmdByte[1] == 'R'))
                    &&(CmdByte[2] == ' '))// rough jump
            {
                cmd_mode = ROUGH_JUMP;
                phase++;
            }
            else if( ((CmdByte[0] == 's')||(CmdByte[0] == 'S'))&&((CmdByte[1] == 't')||(CmdByte[1] == 'T'))
                    &&(CmdByte[2] == ' '))// rough jump
            {
                cmd_mode = SEEK_TARGET;
                phase++;
            }
            else
            {
                cmd_mode = 0;
                s2b_byte_received = 0;
                s2b_cmd_received = FALSE;
                phase = 0;
            }

    #endif // #if (MSG_OUT_MODE == MSG_USBPORT)
            break;

        case 2: // get s2b cmd byte

    #if (MSG_OUT_MODE == MSG_USBPORT)
            switch (cmd_mode)
            {
                case WRITE_BYTE_CMD:
                   s2b_Mode = CmdByte[4];
                   s2b_Addr = (CmdByte[5]<<24)+(CmdByte[6]<<16)+(CmdByte[7]<<8)+CmdByte[8];
                   s2b_Data = (CmdByte[9]<<24)+(CmdByte[10]<<16)+(CmdByte[11]<<8)+CmdByte[12];
                   SendMsgCn(UART_COMMAND_TRACE_MSG,9,0x3F0003,A4B(s2b_Addr),A4B(s2b_Data),A1B(s2b_Mode)); //>>> WRITE_BYTE_CMD <<<
                   break;

                case READ_BYTE_CMD:
                   s2b_Mode = CmdByte[4];
                   s2b_Addr = (CmdByte[5]<<24)+(CmdByte[6]<<16)+(CmdByte[7]<<8)+CmdByte[8];
                   s2b_Data = 0;
                   SendMsgCn(UART_COMMAND_TRACE_MSG,5,0x3F0002,A4B(s2b_Addr),A1B(s2b_Mode)); //>>> READ_BYTE_CMD <<<
                   break;

                case WRITE_EQ_WORD_CMD:
                   s2b_Addr = CmdByte[4];
                   s2b_Data = (CmdByte[5]<<8) + CmdByte[6];
                   break;

                case READ_EQ_WORD_CMD:
                  s2b_Addr = CmdByte[4];
                   s2b_Data = 0;
                   break;

                case OPEN_MSG_CMD:
                   s2b_Addr = CmdByte[4];
                   s2b_Data = CmdByte[5];
                   break;

                case CLOSE_MSG_CMD:
                   s2b_Addr = CmdByte[4];
                   s2b_Data = CmdByte[5];
                   break;

                case BATCH_SET_MSG_CMD:
                    s2b_HifMsg.byte.u = CmdByte[4];
                    s2b_HifMsg.byte.h = CmdByte[5];
                    s2b_HifMsg.byte.m = CmdByte[6];
                    s2b_HifMsg.byte.l = CmdByte[7];

                    s2b_SvoMsg.byte.u = CmdByte[8];
                    s2b_SvoMsg.byte.h = CmdByte[9];
                    s2b_SvoMsg.byte.m = CmdByte[10];
                    s2b_SvoMsg.byte.l = CmdByte[11];
                    break;

                case SPEED_CHANGE:
                   s2b_Addr = 0;
                   s2b_Data = (CmdByte[4]-'0')*10+CmdByte[5]-'0';
                   break;

                case DC_ERASE:
                    s2b_Data = CmdByte[4];
                    break;

                case SHORT_JUMP:            //SJ XXXXX Y (SS = short jump, XXXXX = track number, Y = direction)
                case ROUGH_JUMP:            //RJ XXXXX Y (SR = rough jump, XXXXX = step  number, Y = direction)
                   s2b_Addr = ASC2DEC(&CmdByte[5],5);
                   s2b_Data = ASC2DEC(&CmdByte[11],1);
                   break;
                case SEEK_TARGET:            //ST XXXXXX (ST = seek target, XXXXXX = PSN/LBA)
                   s2b_Addr = ASC2DEC(&CmdByte[5],6);
                   break;
        #ifdef JTT
                case JT_TEST:
                    cmd_mode = 0;
                    JT_test();
                    break;
        #endif
            }
    #else // #if (MSG_OUT_MODE == MSG_USBPORT)
            switch (cmd_mode)
            {
                case WRITE_BYTE_CMD:        //AA XXX YY {AA=cmd,XXX=address,YY=data}
                case SHORT_JUMP:            //SS XXX YY (SS = short jump, XXX = track number, YY = direction)
                case ROUGH_JUMP:            //SR XXX YY (SR = rough jump, XXX = track number, YY = direction)
                   s2b_Addr = ASC2DEC(&CmdByte[3],4);
                   s2b_Data = ASC2DEC(&CmdByte[8],2);
                   break;
                case SEEK_TARGET:            //SR XXX YY (SR = rough jump, XXX = track number, YY = direction)
                   seek_Addr = ASC2DEC(&CmdByte[3],6);
                   break;
                case READ_BYTE_CMD:         //BB XXX {AA=cmd,XXX=address}
                   s2b_Addr = ASC2DEC(&CmdByte[3],4);
                   s2b_Data = 0;
                   break;
                case WRITE_EQ_WORD_CMD:     //CC XX YYYY {AA=cmd,XX=address,YYYY=data}
                   s2b_Addr = ASC2DEC(&CmdByte[3],2);
                   s2b_Data = ASC2DEC(&CmdByte[6],4);
                   break;
                case READ_EQ_WORD_CMD:      //DD XX{AA=cmd,XX=address}
                   s2b_Addr = ASC2DEC(&CmdByte[3],2);
                   s2b_Data = 0;
                   break;
        #ifdef JTT
                case JT_TEST:
                    cmd_mode = 0;
                    JT_test();
                    break;
        #endif
            }

    #endif // #if (MSG_OUT_MODE == MSG_USBPORT)

            get_s2b_cmd_done = TRUE;
            s2b_cmd_received = FALSE;
            s2b_byte_received = 0;
            phase = 0;
            break;
        default :
            get_s2b_cmd_done = FALSE;
            phase = 0;
            break;
    }
}

#define NOTRAP    0
#define TRAPED    1
extern vBYTE      Iram_afeRegsImage[];
BYTE   trap_status;
BYTE   uart_seek_done = TRUE;
/*******************************************************************************

    FUNCTION        exec_uart_cmd

    DESCRIPTION     Execute UART command

********************************************************************************/
void exec_uart_cmd(BOOL check_trap)
{
    ULONG data_read;

    BYTE   tr_buf[12];
    #if (MSG_OUT_MODE == MSG_USBPORT)
    BOOL   SSICmdFail;

    SSICmdFail = FALSE;
    #endif

    if(s2b_cmd_received == TRUE)
    {
        SendMsgCn(UART_COMMAND_TRACE_MSG,2,0x3F0000,A2B(s2b_byte_received)); //>>> Cmd_Received <<<
        s2bCmdDecoder();
    }

    if(get_s2b_cmd_done == TRUE)
    {
    #if (EN_POWER_CONTROL == 1)
        if (CurrentPowerMode == eStandbyPower)
        {
            /* for wake up the power from stand by mode */
            SetDiscReadPowerMode();
        }
    #endif
        get_s2b_cmd_done = FALSE;

        if(check_trap == TRUE)
        {
            if( (0x0FFF&s2b_Addr) == 0xAAA )//disable break point!!!
            {
                trap_status = NOTRAP;
                return;
            }
            else
            {
                trap_status = TRAPED;
            }
        }
        else
        {
            trap_status = NOTRAP;

            if(cmd_mode >= RX_CMD_END) return;
        }

        switch(cmd_mode)
        {
            case WRITE_BYTE_CMD:    /* BYTE Write */
                if(s2b_Mode==(BYTE)('1'))
                {
                    WRITE_ADR_1B(s2b_Addr,(BYTE)(0x000000FF&s2b_Data));
                }
                else if(s2b_Mode==(BYTE)('2'))
                {
                    WRITE_ADR_2B(s2b_Addr,(USHORT)(0x0000FFFF&s2b_Data));
                }
                else
                {
                    WRITE_ADR_4B(s2b_Addr,(ULONG)(s2b_Data));
                }
                // WRITE CMD: Addr. = 0x%04X, Data = 0x%02X
                SendMsgCnB(SHOW_DEBUG_MSG,5,0x3F0011,B4B(s2b_Addr),B1B(s2b_Data));

                if (SSICmdFail == FALSE)
                {
                    tr_buf[0]=0x00;tr_buf[1]=0x00;tr_buf[2]=0x00;tr_buf[3]=0x00;
                    SendMsgAtapiData(&tr_buf[0],4);
                }
                else
                {   /* Need to define the error code in the future */
                    tr_buf[0]=0x80;tr_buf[1]=0x05;tr_buf[2]=0x26;tr_buf[3]=0x02;
                    SendMsgAtapiData(&tr_buf[0],4);
                }

                break;

            case READ_BYTE_CMD: /* BYTE Read */
                if(s2b_Mode==(BYTE)('1'))
                    data_read = READ_ADR_1B(s2b_Addr)&0x000000FF;
                 else if(s2b_Mode==(BYTE)('2'))
                    data_read = READ_ADR_2B(s2b_Addr)&0x0000FFFF;
                else
                    data_read = READ_ADR_4B(s2b_Addr);
                    
                SendMsgCnB(SHOW_DEBUG_MSG,5,0x3F0014,B4B(s2b_Addr),B1B(data_read));

                if (SSICmdFail == FALSE)
                {
                    tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=4;
                    tr_buf[4]=(BYTE)(data_read>>24);tr_buf[5]=(BYTE)(data_read>>16);
                    tr_buf[6]=(BYTE)(data_read>>8);tr_buf[7]=(BYTE)(data_read);
                    tr_buf[8]=0x00;tr_buf[9]=0x00;tr_buf[10]=0x00;tr_buf[11]=0x00;
                    SendMsgAtapiData(&tr_buf[0],12);
                }
                else
                {   /* Need to define the error code in the future */
                    tr_buf[0]=0x80;tr_buf[1]=0x05;tr_buf[2]=0x26;tr_buf[3]=0x02;
                    SendMsgAtapiData(&tr_buf[0],4);
                }
                break;

            case READ_EQ_WORD_CMD:
                data_read = READ_DSP_COEF(s2b_Addr);
                send_msg5SValue(1, "ADDR=",  4,  s2b_Addr);
                send_msg1('D','T',BHIGH(data_read),BLOW(data_read));
                if (SSICmdFail == FALSE)
                {
                    tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=4;
                    tr_buf[4]=(BYTE)(data_read>>24);tr_buf[5]=(BYTE)(data_read>>16);
                    tr_buf[6]=(BYTE)(data_read>>8);tr_buf[7]=(BYTE)(data_read);
                    tr_buf[8]=0x00;tr_buf[9]=0x00;tr_buf[10]=0x00;tr_buf[11]=0x00;
                    SendMsgAtapiData(&tr_buf[0],12);
                }
                else
                {   /* Need to define the error code in the future */
                    tr_buf[0]=0x80;tr_buf[1]=0x05;tr_buf[2]=0x26;tr_buf[3]=0x02;
                    SendMsgAtapiData(&tr_buf[0],4);
                }
                break;

            case WRITE_EQ_WORD_CMD:
                WRITE_DSP_COEF(s2b_Addr,s2b_Data);
                send_msg5SValue(1, "ADDR=",  4,  s2b_Addr);
                send_msg1('D','T',BHIGH(data_read),BLOW(data_read));
                if (SSICmdFail == FALSE)
                {
                    tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=0;
                    SendMsgAtapiData(&tr_buf[0],4);
                }
                else
                {   /* Need to define the error code in the future */
                    tr_buf[0]=0x80;tr_buf[1]=0x05;tr_buf[2]=0x26;tr_buf[3]=0x02;
                    SendMsgAtapiData(&tr_buf[0],4);
                }
                break;

            case OPEN_MSG_CMD:
    #if (MSG_OUT_MODE == MSG_USBPORT)
                if((BYTE)s2b_Addr == 'H')
                {
                    if((BYTE)s2b_Data == 0xff)
                        open_hif_msg(0xffffffff);
                    else if(s2b_Data != 0)
                    {
                        if (s2b_Data <= MaxHifNumber)
                        {
                            open_hif_msg(1<<(BYTE)(s2b_Data - 1));
                        }
                        else
                        {
                            SSICmdFail = TRUE;
                        }
                    }
                }
                else if((BYTE)s2b_Addr == 'S')
                {
                    if((BYTE)s2b_Data == 0xff)
                    {
                        open_svo_msg(0xffffffff);
                    }
                    else if(s2b_Data != 0)
                    {
                        if (s2b_Data <= MaxSvoNumber)
                        {
                            open_svo_msg(1<<(BYTE)(s2b_Data - 1));
                        }
                        else
                        {
                            SSICmdFail = TRUE;
                        }
                    }
                }
                if (SSICmdFail == FALSE)
                {
                    tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=0;
                }
                else
                {
                    tr_buf[0]=0x80;tr_buf[1]=0x05;tr_buf[2]=0x26;tr_buf[3]=0x02;
                }
                SendMsgAtapiData(&tr_buf[0],4);
    #endif
                break;

            case CLOSE_MSG_CMD:
    #if (MSG_OUT_MODE == MSG_USBPORT)
                if((BYTE)s2b_Addr == 'H')
                {
                    if((BYTE)s2b_Data == 0xff)
                        close_hif_msg(0xffffffff);
                    else if(s2b_Data != 0)
                    {
                        if (s2b_Data <= MaxHifNumber)
                        close_hif_msg(1<<(BYTE)(s2b_Data - 1));
                        else
                            SSICmdFail = TRUE;
                    }
                }
                else if((BYTE)s2b_Addr == 'S')
                {
                    if((BYTE)s2b_Data == 0xff)
                        close_svo_msg(0xffffffff);
                    else if(s2b_Data != 0)
                    {
                        if (s2b_Data <= MaxSvoNumber)
                        close_svo_msg(1<<(BYTE)(s2b_Data - 1));
                        else
                            SSICmdFail = TRUE;
                    }
                }
                if (SSICmdFail == FALSE)
                {
                    tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=0;
                }
                else
                {
                    tr_buf[0]=0x80;tr_buf[1]=0x05;tr_buf[2]=0x26;tr_buf[3]=0x02;
                }
                SendMsgAtapiData(&tr_buf[0],4);
    #endif
                break;

            case BATCH_GET_MSG_CMD:
    #if (MSG_OUT_MODE == MSG_USBPORT)
                s2b_HifMsg.Long = hifDebugMsgFlag;
                s2b_SvoMsg.Long = svoDebugMsgFlag;

                tr_buf[0]=0;
                tr_buf[1]=0;
                tr_buf[2]=0;
                tr_buf[3]=0x08;

                tr_buf[4]=s2b_HifMsg.byte.u;
                tr_buf[5]=s2b_HifMsg.byte.h;
                tr_buf[6]=s2b_HifMsg.byte.m;
                tr_buf[7]=s2b_HifMsg.byte.l;

                tr_buf[8]=s2b_SvoMsg.byte.u;
                tr_buf[9]=s2b_SvoMsg.byte.h;
                tr_buf[10]=s2b_SvoMsg.byte.m;
                tr_buf[11]=s2b_SvoMsg.byte.l;

                SendMsgAtapiData(&tr_buf[0],12);

                tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=0;
                SendMsgAtapiData(&tr_buf[0],4);
    #endif
                break;

            case BATCH_SET_MSG_CMD:
    #if (MSG_OUT_MODE == MSG_USBPORT)
                set_hif_msg(s2b_HifMsg.Long);
                set_svo_msg(s2b_SvoMsg.Long);

                tr_buf[0]=0;tr_buf[1]=0;tr_buf[2]=0;tr_buf[3]=0;
                SendMsgAtapiData(&tr_buf[0],4);
    #endif
                break;

            case EJECT_KEY:
    #if (MSG_OUT_MODE == MSG_USBPORT)
                trap_status = NOTRAP;
                svoIramVar.emergencyStopFlag = TRUE;
                EjectKeyInfo.PressedValid = TRUE;
    #endif
                break;

            case QUIT_TRAP:
                trap_status = NOTRAP;
                send_msg5S(1, "QuitTrap");
                break;

            case SPEED_CHANGE:
                svoVar_svoTargetSpeed = s2b_Data;
                spdChangeDiscSpeed(s2b_Data);
                // ! The wobble PLL will be on hold if the speed s2b_Data is different than previous speed
                break;

            case SHORT_JUMP:
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ TRN_MASK)) | SVO_PAUSE);
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ SEQ_MASK)) | SEQ_001);
                svoSeekVar.jumpDistance = s2b_Addr;
                if(s2b_Data == 0)
                    svoSeekVar.jumpType = FORW;
                else
                    svoSeekVar.jumpType = BACK;
                svoSeekVar.jumpType |= F_J_MASK;
                uart_seek_done = FALSE;
                break;

            case ROUGH_JUMP:
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ TRN_MASK)) | TRN_063);
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ SEQ_MASK)) | SEQ_001);
                svoSeekVar.jumpDistance = s2b_Addr;
                if(s2b_Data == 0)
                    svoSeekVar.jumpType = FORW;
                else
                    svoSeekVar.jumpType = BACK;
                svoSeekVar.jumpType |= R_J_MASK;
                uart_seek_done = FALSE;
                break;

            case SEEK_TARGET:
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ TRN_MASK)) | TRN_062);
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ SEQ_MASK)) | SEQ_001);
                svoSeekVar.svoTargetID.Layer = 0;
                svoSeekVar.svoTargetID.SectorNum.all32 = seek_Addr;
                svoSeekVar.advance = 0;
                uart_seek_done = FALSE;
                break;
            case DC_ERASE:
                break;



            case POWER_DOWN:
                send_msg5S(SEND_ATAPI_CMD, "Spin down - SSI");  // for test
                SpeedMD = Standby_f;
                break;

            case SERVO_DSP_DEBUG:
                {
                    BYTE temp;
                    temp = ASC2HEX(&CmdByte[5], 2);

                    if (temp & bit1)            //Check if force servo DSP debug to be the only client of SSI
                        HAL_DSP_SetDebugForce(ON);
                    else
                        HAL_DSP_SetDebugForce(OFF);

                    if (temp & bit0)            //Check if enable servo DSP debug channel
                    {
                        HAL_DSP_SetDebugMode(ON);
                        temp = ASC2HEX(&CmdByte[7], 2);
                        HAL_DSP_SetDebug(DEBUG_CH0, (temp&bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, temp&0x7f, 0x08);
                        temp = ASC2HEX(&CmdByte[9], 2);
                        HAL_DSP_SetDebug(DEBUG_CH1, (temp&bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, temp&0x7f, 0x08);
                        temp = ASC2HEX(&CmdByte[11], 2);
                        HAL_DSP_SetDebug(DEBUG_CH2, (temp&bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, temp&0x7f, 0x08);
                        temp = ASC2HEX(&CmdByte[13], 2);
                        HAL_DSP_SetDebug(DEBUG_CH3, (temp&bit7)?DEBUG_SOURCE_RGLUE:DEBUG_SOURCE_RFILE, temp&0x7f, 0x08);
                    }
                    else
                        HAL_DSP_SetDebugMode(OFF);
                }
                break;
            case LASC_PRESET_POWER:
                diaPresetPower();
                break;

            case LASC_INCR_POWER:
                diaIncrementPower();
                break;
            case LASC_DECR_POWER:
                diaDecrementPower();
                break;
    #if (DEBUG_LASER_OPEN_LOOP == 1) || (DEBUG_LASER_CLOSE_LOOP == 1)
            case LASC_SAVE_TO_PERSIST:
                diaSavePower();
                break;
            case LASC_READ_FROM_PERSIST:
                diaReadPower();
                break;
    #endif
        }//switch(cmd_mode)
    }//if(get_s2b_cmd_done == TRUE)
}

#ifdef JTT

/************************************************************************
* Module:       TXRX_mon
* Description:  monitor TX & TX
* Returns:
* Params:       JT 01 xx
*               CmdByte[4] = 0x01
*               CmdByte[5] = 0x01, TX/RX, MIO_2/MIO_0
*               CmdByte[5] = 0x02, DPD1
*               CmdByte[5] = 0x03, DPD2
*************************************************************************/
void TXRX_mon(void)
{
    WRITE_ADR_1B(0x10000722, READ_ADR_1B(0x10000722)|0xf0);                 //select mio0~3
    WRITE_ADR_1B(0x10000790, 0x14);                                         //mio0
    WRITE_ADR_1B(0x10000791, 0x14);                                         //mio1
    WRITE_ADR_1B(0x10000792, 0x14);                                         //mio2
    WRITE_ADR_1B(0x10000793, 0x14);                                         //mio3

    switch (CmdByte[5])
    {
        case 0x01:      //TX, RX
            WRITE_ADR_1B(0x100009e4, (READ_ADR_1B(0x100009e4)&0xfc)|0x01);  //mio2:TX, mio0:RX
            break;

        case 0x02:      //DPD1
            WRITE_ADR_1B(0x10000974, READ_ADR_1B(0x10000974)|0x20);         //DPDDIGTEN enable
            WRITE_ADR_1B(0x100009e4, (READ_ADR_1B(0x100009e4)&0xfc)|0x02);  //MUXTP1SEL
            WRITE_ADR_1B(0x10000900, (READ_ADR_1B(0x10000900)&0x0f)|0x30);  //RFDIGTP1
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xcf)|0x00);  //DPDP1
            break;

        case 0x03:      //DPD2
            WRITE_ADR_1B(0x100009e4, (READ_ADR_1B(0x100009e4)&0xfc)|0x03);  //MUXTP2SEL
            WRITE_ADR_1B(0x10000900, (READ_ADR_1B(0x10000900)&0xf0)|0x03);  //RFDIGTP2
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xcf)|0x00);  //DPD12UP
            break;
    }
}

/************************************************************************
* Module:       EQDOx_mon
* Description:  monitor EQDOP & EQDON
* Returns:
* Params:       JT 02 xx
*               CmdByte[4] = 0x02
*               CmdByte[5] = 0x01, RF5VP
*               CmdByte[5] = 0x02, RFVGAP
*               CmdByte[5] = 0x03, RFAGCP
*               CmdByte[5] = 0x04, RFOP
*               CmdByte[5] = 0x05, DPD1P
*************************************************************************/
void EQDOx_mon(void)
{
    WRITE_ADR_1B(0x10000970, READ_ADR_1B(0x10000970)&0xef);                 //EQBUFLOW, normal mode
    WRITE_ADR_1B(0x10000971, READ_ADR_1B(0x10000971)&0x7f);                 //EQDOP enable
    WRITE_ADR_1B(0x10000971, READ_ADR_1B(0x10000971)|0x20);                 //RFTP1 enable

    switch (CmdByte[5])
    {
        case 0x01:      //for RF5VP
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xf8)|0x05);  //EQDOSEL = RFANATSEL
            WRITE_ADR_1B(0x10000971, (READ_ADR_1B(0x10000971)&0xfc)|0x01);  //RFANATSEL
            break;

        case 0x02:      //for RFVGAP
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xf8)|0x05);  //EQDOSEL = EQDPDSEL
            WRITE_ADR_1B(0x10000971, (READ_ADR_1B(0x10000971)&0xfc)|0x02);  //RFANATSEL
            break;

        case 0x03:      //for RFAGCP
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xf8)|0x05);  //EQDOSEL = EQDPDSEL
            WRITE_ADR_1B(0x10000971, (READ_ADR_1B(0x10000971)&0xfc)|0x02);  //RFANATSEL
            break;

        case 0x04:      //for RFOP
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xf8)|0x06);  //EQDOSEL = EQDPDSEL
            break;

        case 0x05:      //for DPD1P
            WRITE_ADR_1B(0x10000971, READ_ADR_1B(0x10000971)|0x04);         //DPDANATEN enable
            WRITE_ADR_1B(0x10000972, (READ_ADR_1B(0x10000972)&0xf8)|0x07);  //EQDOSEL = EQDPDSEL
            WRITE_ADR_1B(0x10000970, (READ_ADR_1B(0x10000970)&0xfc)|0x01);  //EQDPDSEL = DPD1P
            break;
    }
}

/************************************************************************
* Module:       load_para
* Description:  load parameter
* Returns:
* Params:       CmdByte[4] = 0x03
*               0xb00009fc
*************************************************************************/
void load_para(void)
{
    WRITE_ADR_1B(0x10000950, 0x18);
    WRITE_ADR_1B(0x1000094d, 0x90);
    WRITE_ADR_1B(0x1000094b, 0x18);
    WRITE_ADR_1B(0x10000949, 0x90);

    WRITE_ADR_1B(0x10000960, 0x87);
    WRITE_ADR_1B(0x10000962, 0x16);
    WRITE_ADR_1B(0x10000964, 0x18);
    WRITE_ADR_1B(0x10000966, 0x9e);

    WRITE_ADR_1B(0x10000912, 0x8c);
    WRITE_ADR_1B(0x10000911, 0xa0);
    WRITE_ADR_1B(0x10000916, 0x15);
    WRITE_ADR_1B(0x10000915, 0x20);
}

/************************************************************************
* Module:       JT_test
* Description:
* Returns:
* Params:       CmdByte[4] & CmdByte[5]
*               CmdByte[4]=0x01    TXRX_mon
*               CmdByte[4]=0x02    EQDOx_mon
*               CmdByte[4]=0x03    load_para
*************************************************************************/
void JT_test(void)
{
    switch(CmdByte[4])
    {
        case 0x01:
            TXRX_mon();
            break;
        case 0x02:
            EQDOx_mon();
            break;
        case 0x03:
            load_para();
            break;
    }
}

/************************************************************************
* Module:       set_BPT
* Description:  set breakpoint
* Returns:
* Params:       bpt_id
*************************************************************************/
void set_BPT(USHORT bpt_id)
{
    unsigned char  tempb=0;
    unsigned short temps=0;

    temps  = bpt_id;
    SendMsgCn(SHOW_DEBUG_MSG,2,0x3F0030,A2B(bpt_id));       // set_BPT: Set breakpoint (bpt_id = %d)
    while(bpt_id)
    {
        ExitProcess();
        exec_uart_cmd(0);       //0:don't need to check uart trap
                                //1:check uart trap
        if (cmd_mode != tempb)
        {
            tempb = cmd_mode;
        }

        switch(cmd_mode)
        {
            case QUIT_TRAP:
                cmd_mode = 0;
            case EJECT_KEY:
                bpt_id = 0;
                break;
            default:
                break;
        }
    }
    SendMsgCn(SHOW_DEBUG_MSG,2,0x3F0031,A2B(bpt_id));       // set_BPT: Released breakpoint (bpt_id = %d)
}
#endif

/*******************************************************************************

    FUNCTION        set_uart_trap

    DESCRIPTION     Set UART breakpoint

********************************************************************************/
void set_uart_trap(void)
{
    struct  svoIramVarT svoIramVarSave;

    trap_status = TRAPED;
    svoIramVarSave = svoIramVar;

    while (1)
    {

        exec_uart_cmd(1);   // 1 : check uart trap; 0 : don't need to check uart trap!!
        if (trap_status != TRAPED)
        {
            svoIramVar = svoIramVarSave;
            break;
        }
    }
}
    #if (MSG_OUT_MODE == MSG_USBPORT)
void SendAtapiDataToUart(ULONG Address, USHORT ByteCount)
{
    USHORT  DataLength, i, delay;
    BYTE    TmpBuf[15];
    BYTE    *DataPtr;
    BYTE    j;

    DataPtr = (BYTE *)Address;
    DataLength = ByteCount;

    TmpBuf[0] = 0x00;
    TmpBuf[1] = 0x00;
    TmpBuf[2] = BHIGH(DataLength);
    TmpBuf[3] = BLOW(DataLength);
    for (i = 0; i < 11; i++)
        TmpBuf[i+4] = *(DataPtr + i);

    DataLength += 4;
    i = 0;
    while (DataLength > 0)
    {
        if (DataLength >= 15)
        {
            SendMsgAtapiData(&TmpBuf[0], 15);
            i += 15;
            DataLength -= 15;
            for (j = 0; j < 15; j++)
                TmpBuf[j] = *(DataPtr + i + j - 4);
        }
        else
        {
            SendMsgAtapiData(&TmpBuf[0], DataLength);
            i += DataLength;
            DataLength = 0;
        }

        for (delay = 0; delay < 255; delay++);
    }
}

void s2bBUILD_SENSE(BYTE snskey, BYTE snscode, BYTE snsqual)
{
    BYTE TmpBuf[4];

    if (snskey == 0x00 && snscode == 0x00 && snsqual == 0x00)
        TmpBuf[0] = 0x00;
    else
        TmpBuf[0] = 0x80;

    TmpBuf[1] = snskey;
    TmpBuf[2] = snscode;
    TmpBuf[3] = snsqual;

    SendMsgAtapiData(&TmpBuf[0], 4);
}
    #endif  // #if (MSG_OUT_MODE == MSG_USBPORT)

#endif // #if (ENABLE_UART_RXD == 1)


#if (SEEPROM == 1)
/* ************************************************************************** */
/* Following are Serial EEPROM access function definitions                    */
/* ************************************************************************** */
void SerialEraseWord(USHORT Address);
/*************************************************************************
* EWENcommand()
*
* Serial EEPROM Erase/Write Enable. All programming operations must begin
* with this function. It enable the chip programming. It's already included
* in the following Write/Erase fuctions.
*
*************************************************************************/
void EWENcommand(void)
{
    USHORT Adrs = 0x2AA;    // Address pattern for EWEN command

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsOutput;

    opEECS=0;    // Instruction Start
    opEECS=1;

    opEEDAT =1;    // Start Bit
    opEECLK =1;    // Toggle Clock
    opEECLK =0;

    opEEDAT =0;    // Erase/Write Enable Opcode (00b)
    opEECLK =1;    // Toggle Clock
    opEECLK =0;
    opEEDAT =0;
    opEECLK =1;
    opEECLK =0;

    opEEDAT =1; // address A9 = 1
    opEECLK =1;
    opEECLK =0;

    opEEDAT =1;    // address A8 = 1
    opEECLK =1;
    opEECLK =0;

    WriteSerialAddress(Adrs);    // Fill out remaining 8 address bits

    opEEDAT =0;
    opEECS=0;            // End of Command

}

/*************************************************************************
* EWDScommand()
*
* Serial EEPROM Erase/Write Disable. All programming operations must ended
* with this function to provide best data protection. It's already included
* in the following Write/Erase fuctions.
*
*************************************************************************/
void EWDScommand(void)
{
USHORT Adrs = 0x255;    // Address pattern for EWDS command

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsOutput;

    opEECS=0;        // Instruction Start
    opEECS=1;

    opEEDAT =1;        // Start Bit
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // Erase/Write Disable Opcode (00b)
    opEECLK =1;
    opEECLK =0;
    opEEDAT =0;
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // address A9 = 0
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // address A8 = 0
    opEECLK =1;
    opEECLK =0;

    WriteSerialAddress(Adrs);    // Fill out remaining 8 address bits

    opEEDAT =0;
    opEECS=0;        //End of command

}


/*************************************************************************
* SerialEraseAll()
* ERAL - Erase All
* This function erase all area of the Serial EEPROM. This function is not supported
* by hardware for the time being.
* It's for future use if hardware supported.
*************************************************************************/
void SerialEraseAll(void)
{
USHORT Adrs = 0x2FF;

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsOutput;

    opEECS=0;        // Instruction Start
    opEECS=1;

    opEEDAT =1;        // Start Bit
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // Erase All Opcode (00b)
    opEECLK =1;
    opEECLK =0;
    opEEDAT =0;
    opEECLK =1;
    opEECLK =0;

    opEEDAT =1;        // address A9 = 1
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // address A8 = 0
    opEECLK =1;
    opEECLK =0;

    WriteSerialAddress(Adrs);    // Fill out remaining 8 address bits

    opEECS=0;        // End of Instruction

    SerialWriteComplete();    // Wait for Erase to complete

    opEECS = 0;        // end of Write command

    EWDScommand();    // Disable SEEPROM programming

}

/*************************************************************************
* SerialWriteAll()
* WRAL - Write All
* This function write all the Serial EEPROM location with the supplied data.
* This function is not supported by hardware for the time being.
* It's for future use if hardware supported.
*************************************************************************/
void SerialWriteAll(USHORT Data)
{
int i;
USHORT WriteData;
USHORT Adrs = 0x211;

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsOutput;

    EWENcommand();    // Issue Write Enable command

    opEECS =1;        // Instruction Start

    opEEDAT =1;        // Start Bit
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // Write All Opcode (00b)
    opEECLK =1;
    opEECLK =0;
    opEEDAT =0;
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // Address A9 = 0
    opEECLK =1;
    opEECLK =0;

    opEEDAT =1;        // Address A8 = 1
    opEECLK =1;
    opEECLK =0;

    WriteSerialAddress(Adrs);    // Fill out remaining address bits with toggling data

    WriteData = Data;

    for (i=0;i<16;i++)
        {
            if (WriteData&bit15)
            {
                opEEDAT =1;
            }
            else
            {
                opEEDAT =0;
            }
            opEECLK =1;
            opEECLK =0;
            WriteData = WriteData<<1;
        }

    opEECS = 0;        // End of Instruction

    SerialWriteComplete();    // Wait for Erase to complete

    opEECS = 0;        // end of command

    EWDScommand();    // Disable SEEPROM programming

    SetEEDATAAsOutput;
}

/*************************************************************************
* SerialWriteWord()
* This function write a word to the serial EEPROM to the specified location.
* INPUT:
*   USHORT Address      ***The byte offset aligned in 2-byte boundary
*   USHORT Data         The data to be written
* OUTPUT/RETURN         None
*************************************************************************/
void SerialWriteWord(USHORT Address, USHORT Data)
{
int i;
USHORT WriteData;
USHORT Addr;

    /*Erase the word first*/
    SerialEraseWord(Address);

    /*Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports*/
    SetEEDATAAsOutput;

    EWENcommand();    // Enable SEEPROM Write/Erase command

    opEECS =1;        // Instruction Start

    opEEDAT =1;        // Start Bit
    opEECLK =1;
    opEECLK =0;

    opEEDAT =0;        // Write Opcode (01b)
    opEECLK =1;
    opEECLK =0;
    opEEDAT =1;
    opEECLK =1;
    opEECLK =0;

    Addr = Address>>1;
    for (i=0;i<10;i++)    //Write 10 bit Address
        {
            if (Addr&bit9)
            {
                opEEDAT =1;
            }
            else
            {
                opEEDAT =0;
            }
            opEECLK =1;
            opEECLK =0;
            Addr = Addr<<1;
        }

    WriteData = Data;

    for (i=0;i<16;i++)    // 16 bit Write Data Pattern
        {
            if (WriteData&bit15)
            {
                opEEDAT =1;
            }
            else
            {
                opEEDAT =0;
            }
            opEECLK =1;
            opEECLK =0;
            WriteData = WriteData<<1;
        }

    opEECS = 0;        // End of Instruction

    SerialWriteComplete();    // Wait for Erase to complete

    opEECS = 0;        // end of command

    EWDScommand();    // Disable SEEPROM Write/Erase programming

    SetEEDATAAsOutput;
}

/*************************************************************************
* SerialReadWord()
*
* This function read a word from the serial EEPROM at the specified location.
* INPUT:
*   USHORT Address      The byte offset aligned in 2-byte boundary
* OUTPUT/RETURN         USHORT data read out (word)
*************************************************************************/
USHORT SerialReadWord(USHORT Address)
{
int i,j;
USHORT Addr;
USHORT ReadData;

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsOutput;

    opEECS =1;            // Instruction Start
    for (i=0;i<5;i++);

    opEEDAT =1;            // Start Bit
    opEECLK =1;
    opEECLK =0;

    opEEDAT =1;            // Read Opcode (10b)
    opEECLK =1;
    opEECLK =0;
    opEEDAT =0;
    opEECLK =1;
    opEECLK =0;

    Addr = Address>>1;
    for (i=0;i<10;i++)    //Write 10 bit Address
        {
            if (Addr&bit9)
            {
                opEEDAT =1;
                opEECLK =1;
                opEECLK =0;
            }
            else
            {
                opEEDAT =0;
                opEECLK =1;
                opEECLK =0;
            }
            Addr = Addr<<1;
        }
        opEEDAT =1;                                // Force Data bit high, so that FW can detect D0 going low

    SetEEDATAAsInput;

    while (1)
    {
        if (opEEDAT == 0)    break;                // Wait for EE_DAT is equal to 0, "Dummy Bit"
    }

    ReadData = 0;                            // Clear ReadData
    for (i=0;i<16;i++)
    {
        ReadData = ReadData<<1;                // Part of the loop, but first enty into loop does not do anything
        opEECLK =1;                            // Raise the Clock
        for (j=0;j<1;j++);                    // Delay to allow data to be driven on port
        opEECLK =0;                            // Lower the clock
        if (opEEDAT == 1)
        {
            ReadData = ReadData+1;        // Add 1 if port bit is high
        }
    }

    opEECS =0;    // End of Read Command

    SetEEDATAAsOutput;
    return(ReadData);
}

/*-----------------------------------------------------------------------------------------------------
   Name:
    BYTE SerialReadByte(USHORT addr)

  Description:
    Read one byte from serial eeprom

  Inputs:
    addr - the byte offset, not the eeprom address

  Outputs:
    N/A

  Global Inputs:
    N/A

  Global Outputs:
    N/A

  Returns:
    one byte data
-----------------------------------------------------------------------------------------------------*/
BYTE SerialReadByte(USHORT addr)
{

    USHORT data = SerialReadWord(addr);
    if (addr & 1)
        return ((BYTE)((data>>8)&0xff));
    else
        return ((BYTE)(data&0xff));
}

/*-----------------------------------------------------------------------------------------------------
   Name:
    void SerialWriteByte(USHORT addr, BYTE data)

  Description:
    write one byte to serial eeprom

  Inputs:
    addr - the byte offset, not the eeprom address
    data - the data to write
  Outputs:
    N/A

  Global Inputs:
    N/A

  Global Outputs:
    N/A

  Returns:
    N/A
-----------------------------------------------------------------------------------------------------*/
void SerialWriteByte(USHORT addr, BYTE data)
{
    USHORT rmw1,rmw2;

    rmw1 = SerialReadWord(addr);
    if (addr & 1)
    {
        rmw2 = data;
        rmw2 <<= 8;
        rmw1 = (rmw1 & 0x00ff) | rmw2;
    }
    else
        rmw1 = (rmw1 & 0xff00) | data;
    SerialWriteWord(addr, rmw1);
}

/*-----------------------------------------------------------------------------------------------------
   Name:
    float SerialRead4F(USHORT addr)

  Description:
    Read four byte from serial eeprom

  Inputs:
    addr - the byte offset and should be algined in 4-byte boundary

  Outputs:
    N/A

  Global Inputs:
    N/A

  Global Outputs:
    N/A

  Returns:
    one byte data
-----------------------------------------------------------------------------------------------------*/
typedef union fu{
    float fn;
    ULONG ul;
}fu_U;
float SerialRead4F(USHORT addr)
{
    fu_U data;
    data.ul = SerialReadWord(addr+2);
    data.ul <<= 16;
    data.ul |= SerialReadWord(addr);
    return (data.fn);
}

/*-----------------------------------------------------------------------------------------------------
   Name:
    void SerialWrite4F(USHORT addr, float data)

  Description:
    write 4 bytes to serial eeprom

  Inputs:
    addr - the byte offset, should be aligned in 4-byte boundary
    data - the data to write
  Outputs:
    N/A

  Global Inputs:
    N/A

  Global Outputs:
    N/A

  Returns:
    N/A
-----------------------------------------------------------------------------------------------------*/
void SerialWrite4F(USHORT addr, USHORT *data)
{
    USHORT rmw;

    rmw = (USHORT) *data;
    SerialWriteWord(addr, rmw);

    rmw = (USHORT) *(data+1);
    SerialWriteWord((addr)+2, rmw);
}

/*************************************************************************
* CheckEEPROMSignature()
*
* This function check if the PersistentTable is loaded into the serial EEPROM
* or not..
* INPUT:        None
* OUTPUT/RETURN     vBOOL TRUE--the PersistentTable is already loaded
*                        FALSE-- the PersistentTable is not loaded or damaged
*************************************************************************/
BOOL CheckEEPROMSignature(void)
{

RidData Rid;
BYTE const Vendor_Signature[8]={FIRMW_INQ_VENDOR};
BYTE Table_Revision, EEPROM_Revision;

    Table_Revision = PersistentTable[PersistentTable_ID_ADDR];
    EEPROM_Revision = SerialReadByte(PersistentTable_ID_ADDR);

    if(Table_Revision != EEPROM_Revision) return (FALSE);

    Rid.RIDReadOutW[0] = SerialReadWord(0x00);
    Rid.RIDReadOutW[1] = SerialReadWord(0x02);
    Rid.RIDReadOutW[2] = SerialReadWord(0x04);

    if (  (Rid.RIDReadOutB[0] ==Vendor_Signature[0])
        &&(Rid.RIDReadOutB[1] ==Vendor_Signature[1])
        &&(Rid.RIDReadOutB[2] ==Vendor_Signature[2])
        &&(Rid.RIDReadOutB[3] ==Vendor_Signature[3])
        &&(Rid.RIDReadOutB[4] ==Vendor_Signature[4])
        &&(Rid.RIDReadOutB[5] ==Vendor_Signature[5]))
        {
            return (TRUE);
        }
    else
        return (FALSE);
}

/*************************************************************************
* LoadSerialEEPROM()
*
* This function reads the PersistentTable from FLASH and stores it into
*    the serial EEPROM, starting from SEEPROM location 0x0000.
* or not..
* INPUT:            None
* OUTPUT/RETURN     None
*************************************************************************/
void LoadSerialEEPROM(void)
{
USHORT i,j;
USHORT DataToWrite;
BYTE const *pointer;
BYTE p1,p2;

    pointer = PersistentTable;
    j = 0;

    for(i=0; i< SIZE_PERSISTENT_TABLE; i+=2,j+=2)
    {
        p1 = *pointer;
        pointer++;
        p2 = *pointer;
        pointer++;

        DataToWrite = (USHORT)p2;
        DataToWrite <<=8;
        DataToWrite += p1;
        SerialWriteWord(j,DataToWrite);
        DelaymS(3);
    }
}

/*************************************************************************
* SerialEraseWord()
*
* This function erase the Word at the serial EEPROM specified by the address
*
* INPUT:            Address     The byte offset aligned in 2-byte boundary
* OUTPUT/RETURN     None
*************************************************************************/
void SerialEraseWord(USHORT Address)
{
int i;
USHORT Addr;

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsOutput;

    EWENcommand();        // Erase/Write Enable

    opEECS =1;            // Instruction Start

    opEEDAT =1;            // Start Bit
    opEECLK =1;
    opEECLK =0;

    opEEDAT =1;            // Opcode (11b)
    opEECLK =1;
    opEECLK =0;
    opEEDAT =1;
    opEECLK =1;
    opEECLK =0;

    Addr = Address>>1;
    for (i=0;i<10;i++)    //Write 10 bit Address
        {
            if (Addr&bit9)
            {
                opEEDAT =1;
                opEECLK =1;
                opEECLK =0;
            }
            else
            {
                opEEDAT =0;
                opEECLK =1;
                opEECLK =0;
            }
            Addr = Addr<<1;
        }

    opEECS = 0;        // end of Instruction

    SerialWriteComplete();    // Wait for Erase to complete

    opEECS = 0;        // End of command

    EWDScommand();    // Disable SEEPROM programming

    SetEEDATAAsOutput;
}


/******************************************************************
WriteSerialAddress()
This function will fill the remaining address lines with the input
8 bit data pattern.
INPUT: 10 bit address
Used with: EWEN, ERAL, WRAL, EWDS commands
*******************************************************************/
void WriteSerialAddress(USHORT SAddress)
{
int i;
USHORT Addr;

    Addr = SAddress;
    for (i=0;i<8;i++)           // Write remaining 8 bits  of Address (A7 - A0)
        {
        if (Addr&bit7)          // Check the Most Significant Bit and set Data line accordingly
            {
                opEEDAT =1;     // Set Data line to 1
            }
        else
            {
                opEEDAT =0;     // Set Data line to 0
            }
        opEECLK =1;             // Toggle Clock to latch data
        opEECLK =0;
        Addr = Addr<<1;         // Shift Address bits to the left and loop
        }
}

/****************************************************************
SerialWriteComplete()
This routine will wait for the BUSY to be asserted (DO = 0), then
wait for the Ready (DO = 1)
This routine is called after completing a write instruction in
either a Write, Erase Write All, or Erase All command

Note: EE_CS should be set to 0 to indicate the end of the write
*****************************************************************/
void SerialWriteComplete(void)
{

    opEEDAT = 1;        // Drive EE_DAT high before we switch to input (DO is tristate)
    opEECS = 1;         // Trigger Busy/Ready output from DO

// Assume that initialization has set EE_DAT, EE_CLK, and EE_CS as ports
    SetEEDATAAsInput;

    while (1)           //Wait for Busy (D0 = 0) to be valid
    {
        if (opEEDAT == 0)    break;
    }

    while (1)           //Wait for Ready (DO = 1) to be valid
    {
        if (opEEDAT == 1)    break;
    }
}

#endif //(SEEPROM ==1)


/* *****************************************************************************
 *
 *  Name: init_serial()
 *
 *    Description: Initialize LCD serial port
 *
 *    Inputs:        None
 *    Returns:    None
 *
 * ***************************************************************************** */
void init_serial(void)
{
#if (XWS == 1)
            /* initialize XWS channel */
            SerialChannelConfig(XWSCh);    /* enable XWS serial port */
#endif

#if (LCD == 1)
            /* initialize LCD channel */
            SerialChannelConfig(LCDCh);    /* enable LCD serial port */
#endif
}


/* *****************************************************************************
    Name:          SerialChannelConfig

    Description: Initialize the serial port function settings

    Inputs:        Channel
    Returns:    None
***************************************************************************** */
void SerialChannelConfig(BYTE Chnl)
{
    switch (Chnl)
    {
#if (XWS == 1)
        case XWSCh:
            /* external WS configuration setting */
            WRITE_REG(HAL_APB_SSIREG_MAP->XWSCLKDIV,    5); /* Clock divider */
            WRITE_REG(HAL_APB_SSIREG_MAP->XWSCFG0, XWSCFG); /* Configure communications Format Invert clock on SSI Interface */
            WRITE_REG(HAL_APB_SSIREG_MAP->XWSINT,    0x00); /* disable all the interupts */
            WRITE_REG(HAL_APB_SSIREG_MAP->XWSINTEN,  0x00); /* Serial Channel Interrupt Clear */
            break;
#endif

        case SSICh:
            break;

#if (LCD == 1)
        case LCDCh:
            /* external WS configuration setting */
            WRITE_REG(HAL_APB_SSIREG_MAP->LCDCLKDIV,    5); /* Clock divider */
            WRITE_REG(HAL_APB_SSIREG_MAP->LCDCFG0, LCDCFG); /* Configure communications Format Invert clock on SSI Interface */
            WRITE_REG(HAL_APB_SSIREG_MAP->LCDINT,    0x00); /* disable all the interupts */
            WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN,  0x00); /* Serial Channel Interrupt Clear */
            break;
#endif

        case DSPCh:
            break;
    }
}


#if (XWS == 1)
/********** Start of External Write Strategy Serial communications functions ***********/

/* ************************************************************************
*    Funtion: WriteXWSReg_1B
*
*    Description: Main function to write data to External Write Strategy
*    This function will check if there is a FIFO entry available.
*     When there is an entry the Address and Data will be loaded
*    into the FIFO and then the function will return.
*
*    Input:
*        Addr    The external Write Strategy register to write
*        data    The data to be written
*    Output:
*        None
* *************************************************************************/
void WriteXWSReg_1B(BYTE addr, BYTE data)
{
    #if 1 // TBD? What is this for? Debug?
    while(1)    /*wait for not full,*/
    {
        WRITE_REG(HAL_APB_SSIREG_MAP->XWSINTEN, 0xFF);       /* Serial Channel Interrupt Clear */

        if (READ_FIELD(HAL_APB_SSI_WFIFFUL))
        {
            WaitXWSFIFOEmpty();
        }
        else
        {
            WRITE_REG(*(ULONG *)(SERIAL_INTERFACE_ADDRESS + XWS_OFFSET_ADDR + (addr<<2)), data);
            return;
        }
    }
    WRITE_REG(HAL_APB_SSIREG_MAP->XWSINTEN, 0x00);       /* Serial Channel Interrupt Clear */
    #else
    while(1)    /*wait for not full,*/
    {
        DISABLE_INTERRUPT();
        if (READ_FIELD(HAL_APB_SSI_WFIFFUL))
        {
            while (READ_FIELD(HAL_APB_SSI_WFIFFUL));
        }
        else    /*not full, send*/
        {
            WRITE_REG(*(ULONG *)(SERIAL_INTERFACE_ADDRESS + XWS_OFFSET_ADDR + addr<<4), data);
            ENABLE_INTERRUPT();
            return;
        }
    }
    #endif
}


/******************************************************************************
*    Funtion: WriteXWSReg_1BWait
*
*    Description: Main function to write data to External Write Strategy,
*    This function will check if there is a FIFO entry available.
*     When there is an entry the Address and Data will be loaded
*    into the FIFO and then the function will wait until the FIFO is empty
*    before returning.
*
*    Input:
*        Addr    The external Write Strategy register to write
*        data    The data to be written
*    Output:
*        None
******************************************************************************/
void WriteXWSReg_1BWait(BYTE addr, BYTE data)
{
    WriteXWSReg_1B(addr, data);
     /* check and wait transfer buffer empty */
    WaitXWSFIFOEmpty();
}


/******************************************************************************
    Name:          WaitXWSFIFOEmpty

    Description: check and wait transfer buffer empty or maximum period of
                 (16*8/SCICh0_XFER_RATE)
                 if The transfer rate =5mbps, then the maximum waiting time is 26us
                 acounting for 260 thumb instructions
    Inputs:
            none
    Returns:
            return after the transmitting buffer is empty

******************************************************************************/
void WaitXWSFIFOEmpty(void)
{
UINT ct=65;

    while(!(READ_FIELD(HAL_APB_SSI_WFIFEMP))) /*wait until transfer buffer empty*/
    {
        ct--;
        if(!ct)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x33550A); /* wait_WRDVALD_time_out */
            return;
        }
    }
}


    #if (XSerialReadEnable == 1)        /* SCI read enable */
/******************************************************************************
    Name:          ReadXWSReg_1B

    Description: read an 8-bit data out from the external AFE channel by first
                 sending out an 8-bit address. Wait for data to be valid and then
                 reading the byte.
    Inputs:
            Addr : 8-bit address data
    Returns:
            return an 8-bit data byte from external AFE
******************************************************************************/
BYTE ReadXWSReg_1B(BYTE addr)
{
    BYTE rdaw;
    UINT ct = 65;
        #if 1   // TBD? What is this for? Debug?
    while(1)    /*wait for not full,*/
    {
        WRITE_REG(HAL_APB_SSIREG_MAP->XWSINTEN, 0xFF);  /* Serial Channel Interrupt Enable */
        WaitXWSFIFOEmpty();                             /* wait for FIFO to be empy */

        CLEAR_INT_FIELD(HAL_APB_SSI_WRDVALD, 0);            /* clear the Read Data Valid bit */
        WRITE_REG(HAL_APB_SSIREG_MAP->XWSRDADD, addr);  /* Request read of address */

        while(!(READ_FIELD(HAL_APB_SSI_WRDVALD)))      /* wait until Read Valid bit asserted */
        {
            ct--;
            if(!ct)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33550B); /* wait_WFIFEMP_time_out */
                break;  /* timed out */
            }
        }

        rdaw = READ_REG(HAL_APB_SSI_XWSRDDAT);
        WRITE_REG(HAL_APB_SSIREG_MAP->XWSINTEN, 0x00);   /* Serial Channel Interrupt Disable */

        return(rdaw);
    }
        #else
    while(1)    /*wait for not full,*/
    {
        WaitXWSFIFOEmpty();                             /* wait for FIFO to be empy */

        DISABLE_INTERRUPT();                            /* disable interrupt */
        CLEAR_INT_FIELD(HAL_APB_SSI_WRDVALD, 0);            /* clear the Read Data Valid bit */
        WRITE_REG(HAL_APB_SSIREG_MAP->XWSRDADD, addr);  /* Request read of address */

        while(!(READ_FIELD(HAL_APB_SSI_WRDVALD)))      /* wait until Read Valid bit asserted */
        {
            ct--;
            if(!ct)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33550B); /* wait_WFIFEMP_time_out */
                break;  /* timed out */
            }
        }

        rdaw = READ_REG(HAL_APB_SSI_XWSRDDAT);
        ENABLE_INTERRUPT();    /* recover interupt mask */
        return(rdaw);
    }

        #endif // if 1
}
    #endif /*XSerialReadEnable*/
#endif /*XWS*/


#if (LCD == 1)
/********** Start of LCD Serial communications functions ***********/
/**************************************************************************
*    Funtion: WriteLCDReg_1B
*
*    Description: Main function to write data to LCD.
*                 This function will check if there is a FIFO entry available.
*                 When there is an entry the Address and Data will be loaded
*                 into the FIFO and then the function will return.
*
*    Input:
*        Addr    The external Write Strategy register to write
*        data    The data to be written
*    Output:
*        None
***************************************************************************/
void WriteLCDReg_1B(BYTE addr, BYTE data)
{
    WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0xFF);       /* Serial Channel Interrupt Enable */
    while(1)    /*wait for not full,*/
    {

        if (READ_FIELD(HAL_APB_SSI_LFIFFUL))
        {
            WaitLCDFIFOEmpty();
        }
        else
        {
            WRITE_REG(*(ULONG *)(HAL_APB_SSI_BaseAddress + LCD_OFFSET_ADDR + (addr<<2)), data);
            WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0x00);       /* Serial Channel Interrupt Disable */
            return;
        }
    }
}

#if (OPU == KEM350AAA)
/**************************************************************************
*    Funtion: WriteOPU
*
*    Description: write byte to one of following devices
*                 - EEPROM
*                 - LCD control
*                 - SA sensor LED control
*                 - FPD media mode select
*                 - OEIC gain control
*                 - Thermistor Voltage monitor
*                 - LDD headroom monitor
*                 - EEPROM status
*                 - Reset Serial device
*                 - Laser select
*              Page 0
*                 - HF modulation amplitude
*                 - HF modulation frequency
*                 - Spread spectrum control
*                 - Wrire waveform shaper
*                 - LDD test mode
*                 - Head room voltage monitor
*              Page 2
*                 - IIN input resistance & bandwidth
*                 - INN input resistance & bandwidth
*                 - IIN input resistance & bandwidth
*                 - LDD test mode
*
*                 -Register page address
*
*    Input:
*        Addr    address {FF..00}
*        data    The data to be written
*    Output:
*        None
***************************************************************************/
//   WRITE_REG(HAL_APB_SSIREG_MAP->LCDRDADD, 0x01);         //0xcf001400  select read Adress 01
//   while(READ_FIELD(HAL_APB_SSI_LRDVALD) == 1);           //0xcf001410.3 SRD valid?
//   ssidata = READ_REG(HAL_APB_SSIREG_MAP->LCDRDDAT);      //0xcf001404  read data
//   if(gmodeb==0)
//       addr1 = ((ssidata&0x87)|MODEA_LOW|MODEB_HIGH);
//   else if(gmodeb==1)
//       addr1 = ((ssidata&0x87)|MODEA_LOW|MODEB_LOW);
//   else if(gmodeb==2)
//       addr1 = ((ssidata&0x87)|MODEA_HIGH|MODEB_MIDDLE);
//
//   WRITE_REG(*(BYTE *)(HAL_APB_SSI_BaseAddress+OPU_REG1), addr1);  /* Adress 01  */

__inline static void WriteOPUReg(BYTE address, BYTE data)
{
    WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0xFF);       /* Serial Channel Interrupt Enable */
    while(1)    /*wait for not full,*/
    {
        DISABLE_INTERRUPT();
        if (READ_FIELD(HAL_APB_SSI_LFIFFUL))
        {
            WaitLCDFIFOEmpty();
        }
        else    /*not full, send*/
        {
            WRITE_REG(*(ULONG *)(HAL_APB_SSI_BaseAddress + OPU_OFFSET_ADDR + (address<<2)), data);
            WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0x00);       /* Serial Channel Interrupt Disable */
            ENABLE_INTERRUPT();
            return;
        }
    }
}

void WriteOPU(BYTE address, BYTE data)
{
    //set page 1 (if needed)
    if ((address & 0x80) != 0) WriteOPUReg(ADDPAGEREG, 0x01);
    WriteOPUReg(address & 0x7F, data);
    //set page 0
    if ((address & 0x80) != 0) WriteOPUReg(ADDPAGEREG, 0x00);
    return;
}

/**************************************************************************
*    Funtion: ReadOPU
*
*    Description: write byte to one of following devices
*                 - EEPROM
*                 - LCD control
*                 - SA sensor LED control
*                 - FPD media mode select
*                 - OEIC gain control
*                 - Thermistor Voltage monitor
*                 - LDD headroom monitor
*                 - EEPROM status
*                 - Reset Serial device
*                 - Laser select
*              Page 0
*                 - HF modulation amplitude
*                 - HF modulation frequency
*                 - Spread spectrum control
*                 - Wrire waveform shaper
*                 - LDD test mode
*                 - Head room voltage monitor
*              Page 2
*                 - IIN input resistance & bandwidth
*                 - INN input resistance & bandwidth
*                 - IIN input resistance & bandwidth
*                 - LDD test mode
*
*                 -Register page address
*
*    Input:
*        Addr    address {FF..00}
*        data    The data to be written
*    Output:
*        None
***************************************************************************/
__inline static BYTE ReadOPUReg(BYTE address)
{
    BYTE ct = 60, x;

    WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0xFF);       /* Serial Channel Interrupt Enable */
    while(1)
    {
        WaitLCDFIFOEmpty();                               /* wait for FIFO to be empy */
        DISABLE_INTERRUPT();                              /* disable interrupt */
        CLEAR_INT_FIELD(HAL_APB_SSI_LRDVALD, 0);          /* clear the Read Data Valid bit */
        WRITE_REG(HAL_APB_SSIREG_MAP->LCDRDADD, address); /* Request read of address */

        while(!(READ_FIELD(HAL_APB_SSI_LRDVALD)))         /* wait until Read Valid bit asserted */
        {
            ct--;
            if(!ct) break;  /* timed out */
        }
        x = READ_REG(HAL_APB_SSIREG_MAP->LCDRDDAT);
        WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0x00);       /* Serial Channel Interrupt Disable */
        ENABLE_INTERRUPT();                               /* recover interupt mask */
        return x;
    }
}

BYTE ReadOPU(BYTE address)
{
    BYTE x;

    //set page 1 (if needed)
    if ((address & 0x80) != 0) WriteOPUReg(ADDPAGEREG, 0x01);
    x = ReadOPUReg(address & 0x7F);
    //set page 0
    if ((address & 0x80) != 0) WriteOPUReg(ADDPAGEREG, 0x0);

    return x;
}

#endif // (OPU == KEM350AAA)

/******************************************************************************
*    Funtion: WriteLCDReg_1BWait
*
*    Description: Main function to write data to LCD.
*                 This function will check if there is a FIFO entry available.
*                 When there is an entry the Address and Data will be loaded
*                 into the FIFO and then the function will wait until the FIFO is empty
*                 before returning.
*
*    Input:
*        Addr    The external Write Strategy register to write
*        data    The data to be written
*    Output:
*        None
******************************************************************************/
void WriteLCDReg_1BWait(BYTE addr, BYTE data)
{
    WriteLCDReg_1B(addr, data);
     /* check and wait transfer buffer empty */
    WaitLCDFIFOEmpty();
}

/******************************************************************************
    Name:          WaitLCDFIFOEmpty

    Description: check and wait transfer buffer empty or maximum period of
                 (16*8/SCICh0_XFER_RATE)
                 if The transfer rate =5mbps, then the maximum waiting time is 26us
                 acounting for 260 thumb instructions
    Inputs:
            none
    Returns:
            return after the transmitting buffer is empty

******************************************************************************/
void WaitLCDFIFOEmpty(void)
{
    UINT ct=65;

    while(!(READ_FIELD(HAL_APB_SSI_LFIFEMP))) /*wait until transfer buffer empty*/
    {
        ct--;
        if(!ct) return;
    }
}

    #if (LCDSerialReadEnable == 1)        /* LCD read enable */
/******************************************************************************
    Name:          ReadLCDReg_1B

    Description: read an 8-bit data out from the external AFE channel by first
                 sending out an 8-bit address. Wait for data to be valid and then
                 reading the byte.
    Inputs:
            Addr : 8-bit address data
    Returns:
            return an 8-bit data byte from external AFE
******************************************************************************/
BYTE ReadLCDReg_1B(BYTE addr)
{
    BYTE rdaw;
    UINT ct = 65;
    while(1)    /*wait for not full,*/
    {
        WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0xFF);  /* Serial Channel Interrupt Enable */
        WaitLCDFIFOEmpty();                             /* wait for FIFO to be empy */

        CLEAR_INT_FIELD(HAL_APB_SSI_LRDVALD, 0);            /* clear the Read Data Valid bit */
        WRITE_REG(HAL_APB_SSIREG_MAP->LCDRDADD, addr);  /* Request read of address */

        while(!(READ_FIELD(HAL_APB_SSI_LRDVALD)))      /* wait until Read Valid bit asserted */
        {
            ct--;
            if(!ct)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33550B); /* wait_WFIFEMP_time_out */
                break;  /* timed out */
            }
        }

        rdaw = READ_REG(HAL_APB_SSIREG_MAP->LCDRDDAT);
        WRITE_REG(HAL_APB_SSIREG_MAP->LCDINTEN, 0x00);   /* Serial Channel Interrupt Disable */

        return(rdaw);
    }
}
    #endif /*LCDSerialReadEnable*/
#endif /*LCD*/

