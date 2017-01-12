/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   at_util.c
*
* DESCRIPTION
* This file contains ATAPI or AT interface routine
*
*   $Revision: 98 $
*   $Date: 11/03/31 2:43p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\bf_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\at_equ.h"
#include ".\hif\rd_int.h"
#include ".\al\reg_HAL.h"
#include ".\al\reg_global.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_host.h"
#include ".\al\reg_bd_codec.h"
#include ".\al\hal_host.h"

#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

static void Lookup( MODES_PARA_T const *base, BYTE b);
static void SendBlockError(void);
void SetupHostTransferRegisters(void);
#if (AUTO_STATU_CPLT==1)
void SetAutoStatusComplete(void);
#endif//(AUTO_STATU_CPLT ==1)


/*********************************************************************
    mode1_para  (Yellow book mode 1)

    mode1_para is a table support the xfer para when the value of rw_read_mode
    is 1. This table only use in READ CD or READ CD MSF command.
*********************************************************************/
const MODES_PARA_T mode1_para[] =
{
/*  vBOOL   HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x08,   0xFE00, 0x0120, 0x0090,         /* EDC                  */
    0x10,   0,      0x0800, 0x0400,         /* UserData             */
    0x18,   0,      0x0920, 0x0490,         /* UserData+ EDC        */
    0x20,   0xFFFC, 4,      0x0002,         /* Header               */
    0x30,   0xFFFC, 0x0804, 0x0402,         /* Header+UserData      */
    0x38,   0xFFFC, 0x0924, 0x0492,         /* Head+UserData+EDC    */
    0x50,   0,      0x0800, 0x0400,         /* SH+UserData          */
    0x58,   0,      0x0920, 0x0490,         /* SH+UserData+EDC      */
    0x60,   0xFFFC, 0x0004, 0x0002,         /* Head+SH(NA)          */
    0x70,   0xFFFC, 0x0804, 0x0402,         /* Head+SH+UserData     */
    0x78,   0xFFFC, 0x0924, 0x0492,         /* AHead+UserData+EDC   */
    0x80,   0xFFF0, 0x0C,   0x0006,         /* Sync                 */
    0xB0,   0xFFF0, 0x0810, 0x0408,         /* Sync+Header+UserData */
    0xB8,   0xFFF0, 0x0930, 0x0498,         /* Sync+Header+UData+EDC*/
    0xE0,   0xFFF0, 0x10,   0x0008,         /* Sync+H+SH            */
    0xF0,   0xFFF0, 0x0810, 0x0408,         /* Sync+AHead+UserData  */
    0xF8,   0xFFF0, 0x0930, 0x0498,         /* All fields           */
    0xFF,   0,      0,      0               /* ---End of Table---   */
};

/*********************************************************************
    mode2_para (XA mode 2 form 1)

    mode2_para is a table support the xfer para when the value of the rw_read_mode
    is 2. This table only use in READ CD or READ CD MSF command.

*********************************************************************/
const MODES_PARA_T mode2_para[] =
{
/*  vBOOL   HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x08,   0xFE08, 0x0118, 0x008C,         /* EDC                  */
    0x10,   8,      0x0800, 0x0400,         /* UserData             */
    0x18,   8,      0x0918, 0x048C,         /* UserData+EDC         */
    0x20,   0xFFFC, 4,      2,              /* Header               */
    0x40,   0     , 8,      4,              /* SHeader              */
    0x50,   0     , 0x0808, 0x0404,         /* SHeader+UserData     */
    0x58,   0     , 0x0920, 0x0490,         /* SHeader+UserData+EDC */
    0x60,   0xFFFC, 0x000C, 0x0006,         /* AHeader              */
    0x70,   0xFFFC, 0x080C, 0x0406,         /* AHeader+UserData     */
    0x78,   0xFFFC, 0x0924, 0x0492,         /* AHeader+UserData+EDC */
    0x80,   0xFFF0, 0x0C,   6,              /* Sync                 */
    0xA0,   0xFFF0, 0x10,   8,              /* Sync+Header          */
    0xB0,   0xFFF0, 0x0930, 0x0498,         /* Sync+Header+UserData */ //WIN Media Player can't play karaoke VCD,  
    0xE0,   0xFFF0, 0x0018, 0x000C,         /* Sync+AHeader         */
    0xF0,   0xFFF0, 0x0818, 0x040C,         /* Sync+AHeader+UserData*/
    0xF8,   0xFFF0, 0x0930, 0x0498,         /* All fields           */
    0xFF,   0,      0,      0               /*  ---End of Table---  */
};

/*********************************************************************
    mode3_para (XA mode 2 form 2)

    mode3_para is a table support the xfer para when the value of rw_read_mode
    is 2. This table only use in READ CD or READ CD MSF command.
*********************************************************************/
const MODES_PARA_T mode3_para[] =
{
/*  vBOOL   HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x10,   0x0008, 0x0918, 0x048C,         /* UserData             */
    0x18,   0x0008, 0x0918, 0x048C,         /* UserData+EDC         */
    0x20,   0xFFFC, 4,      2,              /* Header               */
    0x40,   0    , 8,       4,              /* SHeader              */
    0x50,   0    , 0x0920, 0x0490,          /* SHeader+UserData     */
    0x58,   0    , 0x0920, 0x0490,          /* SHeader+UserData+EDC */
    0x60,   0xFFFC, 0x0C,   6,              /* AHeader              */
    0x70,   0xFFFC, 0x0924, 0x0492,         /* AHeader+UserData     */
    0x78,   0xFFFC, 0x0924, 0x0492,         /* AHeader+UserData+EDC */
    0x80,   0xFFF0, 0x0C,   6,              /* Sync                 */
    0xA0,   0xFFF0, 0x10,   8,              /* Sync+Header          */
    0xB0,   0xFFF0, 0x0930, 0x0498,         /* Sync+Header+UserData */ //WIN Media Player can't play karaoke VCD, 
    0xE0,   0xFFF0, 0x0018, 0x000C,         /* Sync+AHeader         */
    0xF0,   0xFFF0, 0x0930, 0x0498,         /* Sync+AHeader+UserData*/ //Refer to 8090 spec, POWER DVD 10 can't play VCD
    0xF8,   0xFFF0, 0x0930, 0x0498,         /* All fields           */
    0xFF,   0,      0,      0               /*  ---End of Table---  */
};

/*********************************************************************
    mode4_para (Yellow book mode 2)

    mode4_para is a table support the xfer para when the value of rw_read_mode
    is 4. This table only use in READ CD or READ CD MSF command.
*********************************************************************/
const MODES_PARA_T mode4_para[] =
{
/*  vBOOL   HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x10,   0,      0x0920, 0x0490,         /* UserData             */
    0x18,   0,      0x0920, 0x0490,         /* UserData+EDC         */
    0x20,   0xFFFC, 4,      2,              /* Header               */
    0x30,   0xFFFC, 0x0924, 0x0492,         /* Header+UserData      */
    0x38,   0xFFFC, 0x0924, 0x0492,         /* Header+UserData+EDC  */
    0x50,   0     , 0x0920, 0x0490,         /* SHeader+UserData     */
    0x58,   0     , 0x0920, 0x0490,         /* SHeader+UserData+EDC */
    0x60,   0xFFFC, 0x0004, 0x0002,         /* AHeader              */
    0x70,   0xFFFC, 0x0924, 0x0492,         /* AHeader+UserData     */
    0x78,   0xFFFC, 0x0924, 0x0492,         /* AHeader+UserData+EDC */
    0x80,   0xFFF0, 0x0C, 6,                /* Sync                 */
    0xA0,   0xFFF0, 0x10, 8,                /* Sync+Header          */
    0xB0,   0xFFF0, 0x930,  0x0498,         /* Sync+Header+UserData */
    0xB8,   0xFFF0, 0x930,  0x0498,         /* Sync+Header+Data+EDC */
    0xE0,   0xFFF0, 0x0010, 8,              /* Sync+AHeader         */
    0xF0,   0xFFF0, 0x0930, 0x0498,         /* Sync+AHeader+UserData*/
    0xF8,   0xFFF0, 0x0930, 0x0498,         /* All fields           */
    0xFF,   0,      0,      0               /*  ---End of Table---  */
};
/*********************************************************************
    mode5_para  (CD-DA, sub channel info)

    mode5_para is a table support the xfer para for read CD-DA or sub
    channel info. This table is only used in READ CD or READ CD MSF command.
*********************************************************************/
const MODES_PARA_T mode5_para[] =
{
/*  vBOOL   HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x04,   0x0980, 0x0060, 0x0030,         /* PW SubC              */
    0x02,   0x0970, 0x10,   0x08,           /* SubQ                 */
    0xF8,   0xFFF0, 0x0930, 0x0498,         /* CDDA                 */
    0xFF,   0,      0,      0               /*  ---End of Table---  */
};

/*********************************************************************
 dvdMode_para  (DVD)

 dvdMode_para table contains transfer parameters for DVD.
 This table is used only in DVD read commands.
**********************************************************************/
static MODES_PARA_T const dvdMode_para[] = {
/*  FLAG    HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x10,   0,      0x0800, 0x0400,         /* UserData             */
    0xFF,   0,      0,      0               /*  ---End of Table---  */
};

#if (BD_ENABLE == 1)
/*********************************************************************
 bdMode_para  (BD)

 bdMode_para table contains transfer parameters for BD.
 This table is used only in BD read commands.
**********************************************************************/
static MODES_PARA_T const bdMode_para[] = {
/*  FLAG    HSTBYTA ATABC   HSTWDCNT
    ----    ------- ------  --------   */
    0x10,   0,      0x0800, 0x0400,         /* UserData             */
    0xFF,   0,      0,      0               /*  ---End of Table---  */
};
#endif  // BD_ENABLE

#if (AUTO_STATU_CPLT==1)
/*********************************************************************
* SetAutoComplete()
*
* This routine will setup auto-completion feature. The controller chip
* will post completion status after transfer data.
*
* Returns:      void
* Params:       none
*********************************************************************/
void SetAutoStatusComplete(void)
{
    fKernelSendCompletionStatus = TRUE; /* mark send completion */
    WRITE_FIELD(HAL_HST_AUTOSC, 1);
}

/*********************************************************************
* ClearAutoStatusComplete()
*
* This routine will setup auto-completion feature. The controller chip
* will post completion status after transfer data.
*
* Returns:      void
* Params:       none
*********************************************************************/
void ClearAutoStatusComplete(void)
{
    fKernelSendCompletionStatus = FALSE; /* mark send completion */
    WRITE_FIELD(HAL_HST_AUTOSC, 0);
}
#endif//(AUTO_STATU_CPLT==1)



/*********************************************************************
* SendParameterDataToHost(ULONG Address, USHORT ByteCount, FLAG Mode)
*
*  This function transfers data from the SDRAM Buffer memory to the host.
*   It is used to send parameter data such as TOC data or Diagnostics data to the host.
*   This is not the function used to transfer regular read data from the disc.
* Parameters:
*   Address:     A linear address in DRAM Buffer
*   ByteCount    Number of bytes to transfer (must be an even number)
*   Mode         TRUE  =        Send a Command Complete to the host after parameter data has been transfered.
*                FALSE = DO NOT Send a Command Complete to the host after parameter data has been transfered.
*
*********************************************************************/
void SendParameterDataToHost(ULONG Address, ULONG ByteCount, FLAG Mode)
{
    ULONG Xfer_timer;
    vBOOL _temp_IE;
    vUSHORT  backup;
    BYTE Retry_Counter;
    Retry_Counter = 0;

    if(ByteCount == 0)
    {
        if(Mode==TRUE)
        {
            AutoCommandComplete();         // Send Command Complete to host if requested by calling function
        }   
        return;
    }
    Iram_Linear_Xfer_addr = Address;

    if(Iram_Linear_Xfer_addr & 0x03)
    {
        send_msg5SValue(1,"AdrSHIFT",4,Iram_Linear_Xfer_addr);
    }
    if (ByteCount & 0x01)  ByteCount++;   /* Convert to Word Count */
    Iram_shATB  = ByteCount;

    CheckDataTransferLimit(Iram_shATB);
    Knl_send_compl = TRUE;
    Iram_Buf_hxfer_blk = 1;
                                        /* setup H/W xfr */
    if(Ata_sub_burst == TRUE)
    {
        Send_pardata_piece_by_piece();
    }

    St_send_pardata = TRUE;
                                        /* execute xfr */
    TransferToHost();

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        if (Mode == TRUE)
            AutoCommandComplete();

        return;
    }
#endif

    StartTimer(&Xfer_timer);    /* Xfer Error protect */
    while ( 1 )
    {                                   /* wait loop for data xfer done */
        if(St_send_pardata == FALSE)
        {
            break;
        }

        if(Retry_Counter == 3)
            break;

        // Wait 200ms, but drive is not Xfer End yet, then Clear HIntRq and Set it again.
        if(ReadTimer(&Xfer_timer) > 200L)
        {
            Retry_Counter++;

            _temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            backup = Iram_shUMisc;
            Iram_shUMisc &= (~(HAL_HST_HINTRQW));
            WRITE_REG(HAL_HSTREG_MAP->UMISCR,Iram_shUMisc); // Clear HIntRq
            Iram_shUMisc |= (HAL_HST_HINTRQW);
            WRITE_REG(HAL_HSTREG_MAP->UMISCR,Iram_shUMisc); // Set HIntRq
            Iram_shUMisc = backup;

            SET_INT_ENABLE_MASK(_temp_IE);
            StartTimer(&Xfer_timer);
        }

            ExitProcess();
    }                                   /* If eject occurred just finish this command as normal
                                        and let kernel report eject event */
#if (CHIP_REV <= 0xB3)

    if(Xfer_Residue)
    {
        Iram_Linear_Xfer_addr += Iram_shATB;
        Iram_shATB = Xfer_Residue;

        Knl_send_compl = TRUE;
        Iram_Buf_hxfer_blk = 1;

        St_send_pardata = TRUE;
                                            /* execute xfr */
        TransferToHost();

        StartTimer(&Xfer_timer);            /* Xfer Error protect */
        while ( 1 )
        {                                   /* wait loop for data xfer done */
            if(St_send_pardata == FALSE)
            {
                break;
            }
            // Wait 200ms, but drive is not Xfer End yet, then Clear HIntRq and Set it again.
            if(ReadTimer(&Xfer_timer) > 200L)
            {
                _temp_IE = GET_INT_ENABLE_MASK();
                DISABLE_INTERRUPT();

                backup = Iram_shUMisc;
                Iram_shUMisc &= (~(HAL_HST_HINTRQW));
                WRITE_REG(HAL_HSTREG_MAP->UMISCR,Iram_shUMisc);// Clear HIntRq
                Iram_shUMisc |= (HAL_HST_HINTRQW);
                WRITE_REG(HAL_HSTREG_MAP->UMISCR,Iram_shUMisc);// Set HIntRq
                Iram_shUMisc = backup;

                SET_INT_ENABLE_MASK(_temp_IE);
                StartTimer(&Xfer_timer);
            }
                ExitProcess();
        }                                   /* If eject occurred just finish this command as normal
                                            and let kernel report eject event */
	}
#endif            

    if (Mode == TRUE)
        AutoCommandComplete();         // Send Command Complete to host if requested by calling function

}


/*********************************************************************
* GetParameterDataFromHost(ULONG Address, USHORT ByteCount)
*
*  This function transfers data from the host to SDRAM Buffer memory.
*   It is used to receive parameter data such as TOC data or Diagnostics data from the host.
*   This is not the function used to transfer regular write data to the disc.
*   Get parameter data from host . (ex: mode sense data)
*
* Returns:  PASS or FAIL
*   Address:     A linear address in DRAM Buffer to get data into.
*   ByteCount    Number of bytes to transfer (must be an even number)
*
*********************************************************************/
void GetParameterDataFromHost(ULONG Address, ULONG ByteCount)
{
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    ULONG   RecvTimer;
    USHORT  DataLength;
    USHORT  i;
    BYTE    *DataPtr;
    BOOL    XferDone;
#endif

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        if (Address >= SCSDRAM_START_ADDR)
            Address -= SCSDRAM_START_ADDR;

        DataPtr = (BYTE *)(SCSDRAM + Address);
        DataLength = ByteCount;
        s2b_atapi_para_received = FALSE;
        s2b_atapi_para_err = FALSE;
        XferDone = FALSE;

        if (DataLength > (RX_BUF_SIZE - 4))
        {
            SendMsgSpecifyXferLength(RX_BUF_SIZE - 4);
            DataLength -= (RX_BUF_SIZE - 4);
        }
        else
        {
            SendMsgSpecifyXferLength(DataLength);
            DataLength = 0;
        }

        StartTimer(&RecvTimer);
        while (ReadTimer(&RecvTimer) <= 20000L)
        {
            if (s2b_atapi_para_received == TRUE)
            {
                for (i = 0; i < s2b_byte_received; i++)
                {
                    if (i >= 3 && i < (s2b_byte_received - 1))
                        *DataPtr++ = rx_ring_buff[i];
                    rx_ring_buff[i] = ' ';
                }

                s2b_atapi_para_received = FALSE;

                if (DataLength == 0)
                {
                    XferDone = TRUE;
                    break;
                }
                else if (DataLength > (RX_BUF_SIZE - 4))
                {
                    SendMsgSpecifyXferLength((RX_BUF_SIZE - 4));
                    DataLength -= (RX_BUF_SIZE - 4);
                }
                else
                {
                    SendMsgSpecifyXferLength(DataLength);
                    DataLength = 0;
                }
                StartTimer(&RecvTimer);
            }

            if (s2b_atapi_para_err == TRUE)
            {
                s2b_atapi_para_err = FALSE;
                s2bBUILD_SENSE(0xFF,0x08,0x02);
            }
        }

        if (XferDone == FALSE)
            BUILD_SENSE(0x04,0x44,0x00,0x00);
    }
    else
#endif
    {
        if(ByteCount == 0)
        {
            return;
        }
        Iram_Linear_Xfer_addr = Address;
        Iram_shATB = ByteCount;

        Iram_shHDDir |= HAL_HST_HXDIR | HAL_HST_HBODIS;
        WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);

        SetupHostTransferRegisters();
        Get_para_data = TRUE;

        WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */

        while ( 1 )
        {
            if (Get_para_data == FALSE)
                break;
            ExitProcess();
        }

#if (CHIP_REV <= 0xB3)
        if(Xfer_Residue)
        {

#if (ENABLE_AUTO_ABORT == 1)
            /*----------------------------------------------------------
                If auto abort is occured, do not set auto data trigger,
                and abort this command!
            -----------------------------------------------------------*/
            if(AutoAbort)
            {
                send_msg80(1,0x710082);
                return;
            }
#endif

            Iram_Linear_Xfer_addr += Iram_shATB;
            Iram_shATB = Xfer_Residue;
            Get_para_data = TRUE;
            SetupHostTransferRegisters();
            
            WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */
            
            while ( 1 )
            {                                   /* wait loop for data xfer done */
                if(Get_para_data == FALSE)
                    break;
                ExitProcess();
            }
        }
#endif            
    }
}


/*********************************************************************
* TransferToHost()
*
* This routine is used to setup ATA interface  and necessary transfer
* control registers with variables which should be setup by the caller.
*********************************************************************/
void    TransferToHost(void)
{
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        if (Iram_Linear_Xfer_addr >= SCSDRAM_ADDRESS)
            Iram_Linear_Xfer_addr -= SCSDRAM_ADDRESS;

        SendAtapiDataToUart(SCSDRAM_ADDRESS+Iram_Linear_Xfer_addr, Iram_shATB);
        St_send_pardata = FALSE;
        return;
    }
#endif

    Iram_shHDDir |= HAL_HST_HBODIS;                  /* Select linear addr */
    Iram_shHDDir &= (~HAL_HST_HXDIR);                /* Set Direction: device->host */
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
    SetupHostTransferRegisters();
    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);  /* START_XFER */
}

/*********************************************************************
* SetupHostTransferRegisters()
*
* This function is to wait for xfr done in sending paramater data to host.
* Routine will wait until xfr complete
*********************************************************************/
#define SC_XFER_LIMIT_SIZE          0x2000
#define SC_XFER_PAGE_SIZE           0x1000

void SetupHostTransferRegisters(void)
{
#if (CHIP_REV >= 0xC0)
    USHORT Residue_word;
#endif
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

	{
		BYTE  i;
		/* Clear the SATA Transmit and Recieve FIFOs */
  		WRITE_FIELD(HAL_HST_TXFIFPTR, ON);
  		WRITE_FIELD(HAL_HST_RXFIFPTR, ON);

		for (i=0;i<0x05;i++);

  		WRITE_FIELD(HAL_HST_TXFIFPTR, OFF);
  		WRITE_FIELD(HAL_HST_RXFIFPTR, OFF);
	}

#if (CHIP_REV <= 0xB3)
    /* (in absolute addressing mode) Xfer Limit check */
    if ( Iram_shATB >= SC_XFER_LIMIT_SIZE )
    {
        Iram_Buf_hxfer_blk = Iram_shATB/SC_XFER_PAGE_SIZE;
        Xfer_Residue = Iram_shATB % SC_XFER_PAGE_SIZE;
        if (Iram_Buf_hxfer_blk & 0x0001)
        {
            Iram_Buf_hxfer_blk--;
            Xfer_Residue += SC_XFER_PAGE_SIZE;
        }
        Iram_shATB = Iram_Buf_hxfer_blk * SC_XFER_PAGE_SIZE;
        Iram_shHstWdC = SC_XFER_PAGE_SIZE >> 1;
    }
    else
    {
        Xfer_Residue = 0;
        if((Iram_shATB%4) != 0)                                     // Check for DWord transfer length
        {
            Iram_shATB = ((Iram_shATB/4) + 1)*4;
            
            WRITE_FIELD(HAL_HST_ODDWRDSUP,1);                       // Enable odd word transfer logic
        }
        Iram_shHstWdC = Iram_shATB >> 1;
        Iram_Buf_hxfer_blk = 1;
    }
#else //(CHIP_REV >= 0xC0)

    if((Iram_shATB%4) != 0)                                     // Check for DWord transfer length
    {
        Iram_shATB = ((Iram_shATB/4) + 1)*4;

        WRITE_FIELD(HAL_HST_ODDWRDSUP,1);                       // Enable odd word transfer logic
    }

    if(Iram_shATB >= SC_XFER_LIMIT_SIZE)
    {
        Iram_Buf_hxfer_blk = Iram_shATB/SC_XFER_LIMIT_SIZE;
        Xfer_Residue = Iram_shATB % SC_XFER_LIMIT_SIZE;
        Iram_shHstWdC = SC_XFER_LIMIT_SIZE >> 1;
    }
    else
    {
        Iram_shHstWdC = Iram_shATB>>1;
        Xfer_Residue = 0;
        Iram_Buf_hxfer_blk = 1;
    }
    
    if(Xfer_Residue > 0)
    {  
        Residue_word = Xfer_Residue/2;
        Iram_Buf_hxfer_blk++;
    }
    else
    {
        Residue_word = 0;
    }
    
    WRITE_FIELD(HAL_HST_RESWDCL,Residue_word);
    WRITE_FIELD(HAL_HST_RESWDCH,Residue_word>>8);
    WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif
    WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, Iram_Buf_hxfer_blk);         /* set the number of pages */
    WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Iram_Buf_hxfer_blk);         /* Load Host Available Page Count Registers */

    if(St_dma_mode == FALSE) //PIO mode
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, Iram_shATB);                   /* Set Byte Count */
    if(Iram_Linear_Xfer_addr & 0x03)
    {
        //Check SC6230/SC6530 datasheet book1 "HSTBYTACL" register.
        //HSTBYTACL (1¡V0) (1000_0B58h.1¡V0) should always be programmed to 0 because the SC6230/SC6530 does not support transfers that begin at odd-byte addresses
        SendMsg5SValue(1,"Xf_addr_ER",4,Iram_Linear_Xfer_addr);
    }

    WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, Iram_Linear_Xfer_addr);    /* work area start addr */
    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, Iram_shHstWdC);              /* words count in each page */

    SET_INT_ENABLE_MASK(temp_IE);
}


/*********************************************************************
* ATAClearBusy
*
* Clear busy bit in ATA Status Register.
*********************************************************************/
void ATAClearBusy( void )
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    Iram_shHICtl |= HAL_HST_CLRBSY;
    WRITE_REG(HAL_HSTREG_MAP->HICTL,Iram_shHICtl);
    Iram_shHICtl &= (~ HAL_HST_CLRBSY);

    SET_INT_ENABLE_MASK(temp_IE);
}

/*********************************************************************
* InitATATaskFileRegisters
*
* Initial ATA task file at POR/Hard Reset/SRST
*********************************************************************/
void InitATATaskFileRegisters(void)
{
    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_CTTRG);  /* START_XFER */
}


/*********************************************************************
* SendStatus
*
* Setup completion status information at ATAPI Error and Status Register
* Routine will switch to status in phase by setting
*
* Status In Phase:    IO=1 CoD=1 drq=0
*
* If sense_key is non-zero, check condition will be returned to host
* Otherwise, no check condition will be returned.
*
* This routine also sets up mcr bit in error register if necessary.
*********************************************************************/
void SendStatus(void)
{
    BYTE v1;
//  send_msg5SValue(SEND_ATAPI_CMD, "Sense to Host:",  1, Iram_Sense.key  );
    if (Iram_Sense.key != 0)
    {
        PreATAPICMD = 0x00;	/* clear the previous ATAPI Command */

        /* Error Cases, non-zero sense */
        WRITE_REG(HAL_HSTREG_MAP->ATFEA, (((Iram_Sense.cod == 0x28)?HAL_HST_MCRR:0)|(Iram_Sense.key<<4)));
        WRITE_REG(HAL_HSTREG_MAP->TSKCMP1,  (HAL_HST_CDRDY|HAL_HST_CCHECK));   /* set CHECK bit */
    }
    else
    {
        PreATAPICMD = Iram_Cmd_block.bf[0];	/* save the previous ATAPI Command */

        /* No Error, zero sense */
        WRITE_REG(HAL_HSTREG_MAP->ATFEA, 0x00);
        WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
        v1 = (READ_REG(HAL_HSTREG_MAP->USTATR)&0x00ff);
        /* UDMA CRC error occurs, just need to set CHECK bit.
         * Sense Data will be handled by ReqSense command */
        if (v1 & HAL_HST_UCRCERR)
        {
            WRITE_FIELD(HAL_HST_CLRCRCE, 1);
            WRITE_FIELD(HAL_HST_CLRCRCE, 0);
            if (Iram_Cmd_block.bf[0] == SPC_REQUEST_SENSE)
            {
                BUILD_SENSE(0x0B,0x08,0x03,0x01);
            }
            else
            {
                BUILD_SENSE(0x04,0x08,0x03,0x0d);
            }
            WRITE_REG(HAL_HSTREG_MAP->ATFEA, (Iram_Sense.key<<4));
            WRITE_REG(HAL_HSTREG_MAP->TSKCMP1,  (HAL_HST_CDRDY|HAL_HST_CCHECK));
        }
    }
}


/*********************************************************************
* AbortHostTransfer()
*
* This routine will abort host xfer. Also drq of AT status regsiter
* will be cleared
*********************************************************************/
void AbortHostTransfer(void)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    WRITE_REG(HAL_HST_DRDYW_REGISTER, 0x50);  /* status reg, clear drq bit*/

    SET_INT_ENABLE_MASK(temp_IE);
}



/*********************************************************************
* ATASetDSC
*
* This routine will set DRY? and DSC bit in the ATA status register.
*  It is needed after seek completed for immediate seek operation.
*********************************************************************/
void ATASetDSC(void)
{
    WORD data;

    data = READ_REG(HAL_HSTREG_MAP->ATSTAR);          /* Read Status Reg */
    WRITE_REG(HAL_HST_DRDYW_REGISTER, (data | HAL_HST_DSCW));  /* Set Disk Seek Complete*/
    ReadyLED(LED_OFF);
}

/**********************************************************************
* SendHostBlocks
*
* This routine setup data transfer related shadow registers so that
* user data can be transfered to host
*
* Returns:  void
* Params:   none
* Globals:  Buf_hst_bptr - block address will transfer to host
**********************************************************************/
extern USHORT   timeUp_Group1;
void SendHostBlocks(void)
{
    USHORT subq_ptr;
    ULONG work_addr;

    if (DataBuffer.SectorCount == 0)  return;
    if(HostXfer.BlocksLeft == 0)    return;
    Iram_Knl_save_counter = STANDBYCNT;
    //do not reset svoIramVar.knlTimeupCount in SendHostBlocks() for prevent that timeout do not happen in dvdTransferDataToBuffer() 
    //svoIramVar.knlTimeupCount = timeUp_Group1;

    switch(pdbDiscLoaded())
    {
        case DVD_DISC:
#if (ENABLE_HDDVD == 1)
        case HD_DISC:
#endif
            Lookup(dvdMode_para,0x10);
            return;

#if (BD_ENABLE == 1)
        case BD_DISC:
            Lookup(bdMode_para,0x10);
            return;
#endif

        default:
            break;
    }

   //ClrBitsSCReg_1B (rwSubQC, bBufSBC);/*no read CD-G subcode data*/

    /* Check if send R-W/Q subcode to host */
    if((Rw_subc_data == TRUE)&&(Rw_rdcd_cmd == FALSE))
    {
        if (Iram_Cmd_block.bf[10] == 2)
        {
            Lookup(mode5_para,0x02);
            return;
        }
        /* sub-code */
        Lookup(mode5_para,0x04);
        return;
    }

    /* Check if send 2048 bytes to host, or send user specified bytes to
       host. */
    if(Rw_rdcd_cmd == FALSE)
    {
        BYTE a,tno;

        a = Rw_get_blk_mode();

        if (a == eMode1)
        {
            Lookup(mode1_para,0x10);
            return;
        }
        #if(CEProduct == 1)
            else if ((a == eMode2Form1)||(a == eMode2Form2))
        #else
        else if (a == eMode2Form1)
        #endif
        {
            Lookup(mode2_para,0x10);
            return;
        }

        tno = FindTrackNumber(HostXfer.StartBlock.lba);
        if((pdbGetTrackContAddr(tno) & 0x40)) //data track.
            Lookup(mode1_para,0x10);
        else
            SendBlockError();   
        return;
    }

/* read_cd Xfer Q/SubChennal only */
    if((Iram_Cmd_block.bf[9] == 0)&&(Rw_subc_data == TRUE)&&(Rw_rdcd_cmd == TRUE))
    {
#if (SUN_FUNC_TEST == 1)
        subq_ptr = DataBuffer.CurHostPtr;
#else
        subq_ptr = READ_REG(HAL_AUDREG_MAP->ACURPGL);
#endif

        BHIGH(subq_ptr) = (BHIGH(subq_ptr) & 0x1F);
        work_addr = PAGE_AUX_START_ADDR(subq_ptr);

        if(Iram_Cmd_block.bf[10] == 2)
            SendParameterDataToHost(work_addr + AUX_SUBQDATA_ADDR, 0x10, FALSE);
        else
            SendParameterDataToHost(work_addr + AUX_SUBCODEDATA_ADDR, 0x60, FALSE);

        St_send_blkdata = FALSE;
        St_cmd_inproc    = FALSE;
        return;
    }

    /* if user specified data format has been enable
       That is for Read CD or Read CD MSF commands */

    switch (Rw_get_blk_mode())
    {
        case eMode1:
            if (Iram_Cmd_block.bf[1] != 0 && Iram_Cmd_block.bf[1] != 0x08)
                break;

            Lookup(mode1_para,Iram_Cmd_block.bf[9]);
            return;

        case eMode2Form1:
            if ( Iram_Cmd_block.bf[1] != 0 && Iram_Cmd_block.bf[1] != 0x10 )
            {
                if (Iram_Cmd_block.bf[1] == 0x0C && Rw_rdcd_cmd == TRUE)
                {
                    /* Allow using READ_CD formless command to read mode2 form1 data */
                    Lookup(mode2_para,Iram_Cmd_block.bf[9]);
                    return;
                }
                break;
            }
            /* mode2_type: */
            Lookup(mode2_para,Iram_Cmd_block.bf[9]);
            return;

        case eMode2Form2:
            if ( Iram_Cmd_block.bf[1] != 0 && Iram_Cmd_block.bf[1] != 0x14 )
            {
                if (Iram_Cmd_block.bf[1] == 0x0C && Rw_rdcd_cmd == TRUE)
                {
                    /* Allow using READ_CD formless command to read mode2 form2 data */
                    Lookup(mode3_para,Iram_Cmd_block.bf[9]);
                    return;
                }
                break;
            }
            /* mode3_type: */
            Lookup(mode3_para,Iram_Cmd_block.bf[9]);
            return;

        case eYellowMode2:
            if ( Iram_Cmd_block.bf[1] != 0 && Iram_Cmd_block.bf[1] != 0x0c )
                break;
            /* mode3_type: */
            Lookup(mode4_para,Iram_Cmd_block.bf[9]);
            return;

        case eCDDAMode:                     /* CD-DA type */
            /* for Nero read lead-out */
            if ( (Iram_Cmd_block.bf[1] == 0) && ((long)HostXfer.StartBlock.lba >= (Maxlba+1)) )
            {
                Lookup(mode5_para, 0xF8);
                return;
            }

            if ( Iram_Cmd_block.bf[1] != 0 && Iram_Cmd_block.bf[1] != 0x04 )
                break;

            Lookup(mode5_para, 0xF8);
            return;
    } /* end of switch */

    SendBlockError();
}

/***************************************************************************

    FUNCTION        SendBlockError

    DESCRIPTION     Reports Error to host on block transfer
***************************************************************************/
static void SendBlockError(void)
{
    HostXfer.BlocksLeft = 0;
    St_cmd_inproc = FALSE;
    St_send_blkdata = FALSE;
    BUILD_SENSE(0x05,0x64,0x00,0x2a);        /* ILLEGAL MODE FOR THE TRACK */

    if ( Iram_St_fast_cache_hit == TRUE )
    {
        Iram_St_fast_cache_hit = FALSE;
        Rw_C2PO_data = FALSE;                /* mark no read c2po data */
        Rw_subc_data = FALSE;
        Read_subc_only = FALSE;
        Rw_rdcd_cmd  = FALSE;
#if (SUN_FUNC_TEST == 1)
        RdC2POData_296 = FALSE;
        RdC2PODataTaggle = FALSE;
#endif
        AutoCommandComplete();
    }
}


/***************************************************************************

    FUNCTION        Lookup

    DESCRIPTION     Looks up the trasfer parameters
***************************************************************************/
static void Lookup(MODES_PARA_T const *base, BYTE b)
{
    BYTE drqblk;

    Iram_Buf_hxfer_blk = 1;
    Iram_shHstWdC = 1;                          /* transfer 1 page */

    for (; base->flag != b && base->flag != 0xFF; base++)
        ;                               /* do nothing */

    if (base->flag != 0xff)
    {
        /* Flag type is supported, we begin get the DACL, DACH and ATBLO, ATBHI
           from the table. */
        Iram_shHstBytAC  = base->hstbytac;
        Iram_shATB  = base->atabc;
        Iram_shHstWdC = base->hstwdcnt;

        WRITE_REG(HAL_HSTREG_MAP->HSTSUBC, 0x00);     /* XFER 0 BYTES DATA */
        WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x00);     /* XFER C2PR 294 BYTES DATA */

        if(Rw_rdcd_cmd == TRUE)
        {
            if(Rw_subc_data == TRUE)
            {
                if (Iram_Cmd_block.bf[10] == 2)
                {
                    Iram_shATB += 16;
                    Iram_shHstWdC = Iram_shATB/2;

                    WRITE_REG(HAL_HSTREG_MAP->HSTSUBC, 0x08);   /* XFER PQ 16 BYTES DATA */
                    WRITE_REG(HAL_HSTREG_MAP->HSTSUBAL, 0x170); /* Offset Address for PQ (0x970) */
                }
                else
                {
                    Iram_shATB += 96;
                    Iram_shHstWdC = Iram_shATB/2 ;

                    WRITE_REG(HAL_HSTREG_MAP->HSTSUBC, 0x30);   /* XFER P-W 96 BYTES DATA */
                    WRITE_REG(HAL_HSTREG_MAP->HSTSUBAL, 0x180); /* Offset Address for P-W (0x980) */
                }
            }

            if (Iram_Page_Size_3K_Flag == 1)
            { /*Automatic change from 2.5K to 3k page for C2PO*/
                if (Rw_C2PO_data == TRUE)
                {
#if (SUN_FUNC_TEST == 1)
                    if(RdC2POData_296 == FALSE)
                    {
                        Iram_shATB += 294;
                        WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x93);     /* XFER C2PR 294 BYTES DATA */
                        WRITE_REG(HAL_HSTREG_MAP->HSTAUXAL, 0x1FE);   /* Offset Address for C2PR (0x1FE) */
                    }
                    else
                    {
                        Iram_shATB += 296;
                        WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x94);     /* XFER C2PR 296 BYTES DATA */
                        WRITE_REG(HAL_HSTREG_MAP->HSTAUXAL, 0x1FC);   /* Offset Address for C2PR (0x1FC) */
                    }
                    Iram_shHstWdC = Iram_shATB/2 ;
#else
                    /* need transfer 294 bytes C2PR data */
                    Iram_shATB += 294;
                    Iram_shHstWdC = Iram_shATB/2 ;

                    WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x93);     /* XFER C2PR 294 BYTES DATA */
                    WRITE_REG(HAL_HSTREG_MAP->HSTAUXAL, 0x1FE);   /* Offset Address for C2PR (0x1FE) */
#endif
                }
            }

        }/*   (Rw_rdcd_cmd == TRUE)*/
#if (SUN_FUNC_TEST == 1)
        if((Rw_rdcd_cmd == TRUE) && ((Iram_shATB & 0x03) != 0) && (Rw_C2PO_data == TRUE))
         {
             ULONG work_addr, temp_Offset;
             USHORT i;
        
             work_addr = PAGE_AUX_START_ADDR(DataBuffer.CurHostPtr);
        
             if((RdC2PODataTaggle == FALSE) && (HostXfer.BlocksLeft != 1))
             {
                 if(DataBuffer.SectorCount == 1)
                 {
                     HostXfer.State = WAITING_FOR_TRANSFER;
                     return;
                 }

                 if(DataBuffer.CurHostPtr == Iram_usiRingEP)
                 {
                     if(Iram_shHstBytAC > 0xFFEF)
                         temp_Offset = AUX_START_ADDR + (Iram_shHstBytAC & 0x00FF) + 0xB00 - 0x800;
                     else if(Iram_shHstBytAC > 0x07FF)
                         temp_Offset = AUX_START_ADDR + Iram_shHstBytAC - 0x800;
                     else
                        temp_Offset = Iram_shHstBytAC;
                 }
                 else
                 {
                     if(Iram_shHstBytAC > 0xFFEF)
                         temp_Offset = work_addr + 0x400 + (Iram_shHstBytAC & 0x00FF) + 0xB00;
                     else if(Iram_shHstBytAC > 0x07FF)
                         temp_Offset = work_addr + 0x400 + Iram_shHstBytAC;
                     else
                        temp_Offset = Iram_shHstBytAC + ((DataBuffer.CurHostPtr + 1) * 0x800);
                 }
        
                 SCSDRAM[work_addr + 0xB22] = SCSDRAM[temp_Offset];
                 SCSDRAM[work_addr + 0xB23] = SCSDRAM[temp_Offset + 1];
        
                 Iram_shATB += 2;
                 Iram_shHstWdC = Iram_shATB/2 ;
                 WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x94);
                 WRITE_REG(HAL_HSTREG_MAP->HSTAUXAL, 0x1FC);
                 RdC2PODataTaggle = TRUE;
             }
             else if(RdC2PODataTaggle == TRUE)
             {
                 work_addr >>= 1;
                 for(i = 0x5F8 ; i < 0x600; i++)
                     SCSDRAM16[work_addr + i] = SCSDRAM16[work_addr + i + 1];
        
                 SCSDRAM16[work_addr + 0x5FF] = SCSDRAM16[(DataBuffer.CurHostPtr * 0x400)];
        
                 work_addr = DataBuffer.CurHostPtr * 0x400;
                 for(i = 0 ; i < 0x400; i++)
                     SCSDRAM16[work_addr + i] = SCSDRAM16[work_addr + i + 1];
        
                 work_addr = PAGE_AUX_START_ADDR(DataBuffer.CurHostPtr);
                 work_addr >>= 1;
                 SCSDRAM16[(DataBuffer.CurHostPtr * 0x400) + 0x3FF] = SCSDRAM16[work_addr + 0x400];
        
                 for(i = 0x400; i < 0x490; i++)
                     SCSDRAM16[work_addr + i] = SCSDRAM16[work_addr + i + 1];
        
                 SCSDRAM16[work_addr + 0x48F] = 0x0000;
        
                 Iram_shATB -= 2;
                 Iram_shHstWdC = Iram_shATB/2 ;
                 WRITE_REG(HAL_HSTREG_MAP->HSTAUXW, 0x92);
                 WRITE_REG(HAL_HSTREG_MAP->HSTAUXAL, 0x200);
                 RdC2PODataTaggle = FALSE;
             }
        
             Iram_Buf_hxfer_blk = 1;
         }
        else
#endif
        {
        drqblk = CalcDataRequestBlocks();

/* DMA transfer burst rate */
        if(READ_FIELD(HAL_CDD_SDBUF))
        {
            // Decoder is running
            if(St_dma_mode == FALSE)
            {
            if(drqblk > 3)
                drqblk = 3;

                CalcHostTransferBlocks(drqblk);
                Iram_shATB *= Iram_Buf_hxfer_blk;
            }
            else// DMA
                CalcHostTransferBlocks(32);
        }
        else
        {
            // Decorder is stop
            if(St_dma_mode == FALSE)
            {
        CalcHostTransferBlocks(drqblk);
                Iram_shATB *= Iram_Buf_hxfer_blk;
            }
            else// DMA
                CalcHostTransferBlocks(32);
        }
        }
// --------------------------------------
//      this is original code
//---------------------------------------
//        if(ReadSCReg_1B(rwCtrlAM1) & 0x08)
//        {
//            // Decoder is running
//            if(drqblk > 3)
//                drqblk = 3;
//        }
//
//        CalcHostTransferBlocks(drqblk);
//        Iram_shATB *= Iram_Buf_hxfer_blk;

        Iram_shHstBufPg = DataBuffer.CurHostPtr;
        St_send_blkdata = TRUE;
        TransferBlocksToHost();
    }
    else
    {                                   /* look_err */
        HostXfer.BlocksLeft = 0;
        St_cmd_inproc = FALSE;
        St_send_blkdata = FALSE;
        BUILD_SENSE(0x05,0x24,0x00,0x66);    /* INVALID FIELD IN CDB */
    }
}



/*******************************************************************************

    FUNCTION        TransferBlocksToHost

    DESCRIPTION     Sets up the ATA interface and transfer control then
                    triggers the auto transfer of block data to host.
********************************************************************************/
#if (CDPlayability == 1)
extern BYTE CDBufferAlmostEmpty;
#endif

void TransferBlocksToHost(void)
{
vBOOL temp_IE;

#ifdef READ10_SSI  // Read10 command support on SSI port
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    long tmp;
    ULONG   tmp_hxfer_blk;

    if (s2b_atapi_cmd_received == TRUE)
    {
        if (HostXfer.BlocksLeft > (ULONG)DataBuffer.SectorCount)
            Iram_Buf_hxfer_blk = DataBuffer.SectorCount;
        else
            Iram_Buf_hxfer_blk = (USHORT)HostXfer.BlocksLeft;

        /* decrease the buffer block count by number of blocks transferred*/
        DataBuffer.SectorCount -= Iram_Buf_hxfer_blk;

        /* update the start of the cache */
        DataBuffer.CacheStartLBA += Iram_Buf_hxfer_blk;

        /* update the host buffer pointer by the number of blocks transferred to the host */
        DataBuffer.CurHostPtr += Iram_Buf_hxfer_blk;

        /* Insure the host buffer pointer wraps accordingly if it excedes the end of buffer */
        DataBuffer.CurHostPtr %=(Iram_usiRingEP + 1);

        /* decrease the count for remainig blocks to transfer by the number of blocks just transferred */
        HostXfer.BlocksLeft -= (ULONG)Iram_Buf_hxfer_blk;

        /*-------------------------------------------------------
            Determine if all requested blocks have been transferred
        --------------------------------------------------------*/
        if ( HostXfer.BlocksLeft == 0 )
        {

            /*-------------------------------------------------------
                All requested blocks have been transferred
            --------------------------------------------------------*/

            /*-------------------------------------------------------
                The following firmware readjusts the the block count,
                start of the cache, and the host buffer pointer to
                preserve data that has been transferred for the
                current read command. This improves the transfer
                rate in the event that the next command requests the
                same data.

                NOTES:
                When BURST RATE MODE is on (=1) maximum
                adjustment is 32 (64K) blocks, else it is 16 blocks

                This may not apply for DVD
            --------------------------------------------------------*/
            HostXfer.State = TRANSFER_COMPLETE; /* set host transfer state to complete */

    #if (BypassDecoderEr == 1)
            IGNORE_DECODER_ERROR = FALSE;
    #endif

            if( pdbDiscLoaded() == CD_DISC )
            {
                /*-------------------------------------------------------
                   Get the original transfer count of the current
                   read command. Adjust for the appropriate burst rate
                   mode if needed.
                --------------------------------------------------------*/
                tmp = HostXfer.BlockCount.Long;         /* Get original transfer count of the read command*/

                if (HostXfer.BlockCount.Long > 0x20)
                {
                    tmp = 0x20;             /* Set max adjustment to 32 sectors */
                }

                /*-------------------------------------------------------
                    Increase the buffer block count and readjust the
                    start of the cache
                --------------------------------------------------------*/
                DataBuffer.SectorCount += tmp;
                DataBuffer.CacheStartLBA -= tmp;

                /*-------------------------------------------------------
                    Set back the host buffer pointer position
                --------------------------------------------------------*/
                if ( ((SHORT)DataBuffer.CurHostPtr - (SHORT)tmp) < 0)
                {
                    /*-------------------------------------------------------
                        Insure the buffer pointer remains within the ring
                        buffer
                    --------------------------------------------------------*/
                    DataBuffer.CurHostPtr -= tmp;
                    DataBuffer.CurHostPtr += (Iram_usiRingEP + 1);
                }
                else
                {
                    DataBuffer.CurHostPtr -= tmp;
                }

            }
	        else
	        {  /* To store the previous request LBA */
	           PreReadCMDLba = (DataBuffer.CacheStartLBA-HostXfer.BlockCount.Long)&0xFFFFFFF0;
	        }

            /* Reset the current transfer count */
            Iram_Buf_hxfer_blk = 0;

            /* Reset the read verify commmand flag if necessary */
            if ( fReadVerifyCommand == TRUE )
            {
                fReadVerifyCommand = FALSE;
            }


            /*-------------------------------------------------------
                Complete command if from a fast cache process
            --------------------------------------------------------*/
            if (Iram_St_fast_cache_hit == TRUE)
            {
                /*-------------------------------------------------------
                   Update flags
                --------------------------------------------------------*/
                Knl_send_compl = TRUE;          /* mark send completion */
                St_send_blkdata = FALSE;        /* mark no data out */
                Iram_St_fast_cache_hit = FALSE; /* reset fast cache flag */
                St_cmd_inproc = FALSE;          /* mark command not in process */
                Ata_sub_burst = FALSE;          /* clear ata sub burst */

                AutoCommandComplete();
                return;
            }

            /*-------------------------------------------------------
              Report end status
            --------------------------------------------------------*/
            if(Knl_send_compl == FALSE)
            {
                SendStatus();
                Knl_send_compl = TRUE;                      /* mark send completion */
            }

            St_cmd_inproc = FALSE;              /* mark no cmd in proc */
            St_send_blkdata = FALSE;            /* mark no data out */

#if (BY_PASS_C2Error_For_VCD == 1)
            C2ErrorCount = 0;
#endif

#if (CDPlayability == 1)
            if(DataBuffer.SectorCount > 26)
                CDBufferAlmostEmpty = FALSE;
#endif

        } /* End if transfer is complete */
        else
        {
            /*-------------------------------------------------------
              All requested blocks have not been transferred.
              (Command is not finished yet.) Determine if there
              are blocks in the buffer that can be transferred
            --------------------------------------------------------*/

            if (DataBuffer.SectorCount == 0)
            {
                /*-------------------------------------------------------
                    Buffer is empty so no transfer can be initiated.
                    Wait for the next decoder interrupt to start transfer
                --------------------------------------------------------*/
                St_send_blkdata = FALSE;    /* mark that no data is transferring */
                HostXfer.State = WAITING_FOR_TRANSFER;
            }
            else
            {
                /*-------------------------------------------------------
                    There is data in the buffer which is available to xfer
                    to host. Start transferring available blocks to host
                --------------------------------------------------------*/
                if(EjectKeyInfo.PressedValid)
                {
                    /* Eject is occuring so do not transfer */
                    St_send_blkdata = FALSE;    /* mark no data out */
                }
                else
                {
                    if ( fReadVerifyCommand == TRUE )
                    {
                        tmp_hxfer_blk = (ULONG)DataBuffer.SectorCount;

                        if ( HostXfer.BlocksLeft < tmp_hxfer_blk )
                        {

                            /* part of cache data block is transferred */
                            DataBuffer.SectorCount    = (WORD)(tmp_hxfer_blk - HostXfer.BlocksLeft);
                            tmp_hxfer_blk = HostXfer.BlocksLeft;
                            HostXfer.BlocksLeft  = 0;
                            Knl_send_compl = TRUE;      /* mark send completion */
                            if ( Iram_St_fast_cache_hit == TRUE )
                            {
                                Iram_St_fast_cache_hit = FALSE;
                            }
                            fReadVerifyCommand = FALSE;
                            AutoCommandComplete();             /* report end status */
                        }
                        else
                        {
                        /* whole data block is transferred */
                            DataBuffer.SectorCount    = 0;
                            HostXfer.BlocksLeft -= tmp_hxfer_blk;
                        }

                        /* update start of cache to indicate data xfered done */
                        DataBuffer.CacheStartLBA += tmp_hxfer_blk;

                        /* Advance the host buffer pointer */
                        DataBuffer.CurHostPtr += tmp_hxfer_blk;

                        /*-------------------------------------------------------
                            Insure the buffer pointer remains within the ring
                            buffer
                        --------------------------------------------------------*/
                        DataBuffer.CurHostPtr %=(Iram_usiRingEP + 1);

                        St_send_blkdata = FALSE;    /* mark no data out */
                    }
                    else
                    {
                        St_send_blkdata = FALSE;    /* mark no data out */
                    }
                }
            }
        }
        return;
    }
#endif
#endif

    /*-------------------------------------------------------
        Determine if Read Verify command
    --------------------------------------------------------*/
    if ( fReadVerifyCommand == TRUE )
    {
        WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, Iram_shHstBufPg);  /* start page address */
        WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, Iram_shHstBufPg);

        HostTransferEndIntProc();
        return;
    }

    //send_msg5SValue(SEND_MRW_MSG, "Sd", 4, Iram_shHstBufPg<<16 | Iram_Buf_hxfer_blk);

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

	{
		BYTE  i;
		/* Clear the SATA Transmit and Recieve FIFOs */
		WRITE_FIELD(HAL_HST_TXFIFPTR,1);
		WRITE_FIELD(HAL_HST_RXFIFPTR,1);
		for (i=0;i<0x05;i++);
		WRITE_FIELD(HAL_HST_TXFIFPTR,0);
		WRITE_FIELD(HAL_HST_RXFIFPTR,0);

	}

    /*-------------------------------------------------------
        Set the starting page address
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, Iram_shHstBufPg);      /* start page address */

    /*-------------------------------------------------------
        Set up the page compare addresses
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, Iram_shHstBufPg);

    #if( APPEND_RD_RVD==1)
    /*-------------------------------------------------------
        Host Append set up for DMA/UDMA modes only
    --------------------------------------------------------*/
    if (St_ultra_dma || St_dma_mode)
    {
        /* Total block xfer number */
        WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, (USHORT)HostXfer.BlocksLeft);
        /* Load Host Available Page Count Registers */
        WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Iram_Buf_hxfer_blk);

        HostXfer.BlocksLeft-=(ULONG)Iram_Buf_hxfer_blk;

        DataBuffer.SectorCount -=Iram_Buf_hxfer_blk;

    }
    else
    {
        /*-------------------------------------------------------
            Set up the transfer block count
        --------------------------------------------------------*/
        WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, Iram_Buf_hxfer_blk); /* block xfer number */
        WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Iram_Buf_hxfer_blk); /* Load Host Available Page Count Registers */
    }
    #else /* APPEND_RD_RVD */
    {
        /*-------------------------------------------------------
            Set up the transfer block count
        --------------------------------------------------------*/
        WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, Iram_Buf_hxfer_blk); /* block xfer number */
        WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Iram_Buf_hxfer_blk); /* Load Host Available Page Count Registers */
    }
    #endif /* APPEND_RD_RVD */


    /*-------------------------------------------------------
        Set up the byte offset into the page and
        word per page count
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, Iram_shHstBytAC); /* offset */
    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, Iram_shHstWdC);     /* words count in each page */

#if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_HST_RESWDCL,0x00);
            WRITE_FIELD(HAL_HST_RESWDCH,0x00);
            WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif
    /*-------------------------------------------------------
        Set up byte count for non DMA/UDMA transfers
    --------------------------------------------------------*/
    if(St_dma_mode == FALSE)
        WRITE_REG(HAL_HSTREG_MAP->ATBLO, Iram_shATB);  /* Set Byte Count */

    /*-------------------------------------------------------
        Set page offset mode and transfer direction
        (device ---> host)
    --------------------------------------------------------*/
    Iram_shHDDir &=  (~(HAL_HST_HXDIR | HAL_HST_HBODIS));
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);

#if (ENABLE_AACS_BEF == 1)
    if (pdbDiscLoaded() == BD_DISC)
    {
        HAL_HOST_AesSetBEFBufferXferStart( ((ULONG)READ_FIELD(HAL_BDC_FDHSTBISS) << 11) + ((ULONG)Iram_shHstBufPg << 11) + 80 );
        HAL_HOST_AesSetBEFSecOffset((Iram_shHstBufPg & 0x001F));
    }
    else if (pdbDiscLoaded() == HD_DISC)
    {
    // TBD? No Code!
    }

    HAL_HOST_StartAesBusEncryption();
#endif  // ENABLE_AACS_BEF

    /*-------------------------------------------------------
        Set up status completion
    --------------------------------------------------------*/
    #if (AUTO_STATU_CPLT==1)
    if (fKernelAutoCompletionStatus==TRUE)
    {
        fKernelAutoCompletionStatus = FALSE;
        SetAutoStatusComplete();
    }
    else
        ClearAutoStatusComplete();
    #endif

    /*-------------------------------------------------------
        Start the transfer
    --------------------------------------------------------*/
    HostXfer.State = TRANSFERRING;

    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);

    /*-------------------------------------------------------
        LED set up
    --------------------------------------------------------*/
    #if (LED_NUM == 1)
    ReadyLED(LED_ON);   
    #endif /* LED_NUM == 1 */

    #if (LED_NUM == 2)
    ReadyLED(LED_FLASH);
    #endif /* LED_NUM == 2 */

    SET_INT_ENABLE_MASK(temp_IE);
}


/**********************************************************************
* Residue_minus_xfr_byte
*
* This routine calculates residue = residue - xfr_allow
*
* Returns:  FALSE - Residue > Xfr_allow
*           FRUE - Otherwize
* Params:   none
**********************************************************************/
vBOOL Residue_minus_xfr_byte(void)
{
    if (Iram_Residue > Iram_Xfr_allow)
    {
        Iram_Residue -= Iram_Xfr_allow;
        return FALSE;
    }
    else
        return TRUE;
}


/**********************************************************************
* IncATAPIByteCount
*
* This routine calculate SH_DACL/H = SH_DACL/H + SH_ATBLO/HI
**********************************************************************/
void IncATAPIByteCount(void)
{
    Iram_Linear_Xfer_addr += Iram_shATB;
}


/*********************************************************************
* Min_residue_xfr_allowed
*
* This routine setup data transfer related shadow registers for
* transfer one block (2048 bytes) of user data to host
*
* Returns:  void
* Params:   none
*********************************************************************/
void Min_residue_xfr_allowed(void)
{
    if (Iram_Xfr_allow >= Iram_Residue)
    {
        Iram_shATB = Iram_Residue;
    }
}


/**********************************************************************
* Send_pardata_piece_by_piece
*
* This routine prepare shadow variables to issue a parameter data xfer.
**********************************************************************/
void Send_pardata_piece_by_piece(void)
{
    Iram_Residue = Iram_shATB;
    Iram_shATB = MINIMUM( Iram_Xfr_allow,Iram_shATB);  // Number of bytes to xfer. Compare, return minimum.
}

/*********************************************************************
* CheckDataTransferLimit
*
* Compare # of bytes allowed per xfer & actual data length to see
* if each block of data can be xfered in a burst or not.
*
* Params:   data_len
*********************************************************************/
void CheckDataTransferLimit(USHORT data_len)
{
    /* Assume burst data xfer mode, if DMA mode transfer is ON, always use
       burst data xfer mode. */

#ifdef READ10_SSI  // Read10 command support on SSI port
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
        return;
#endif
#endif

    Ata_sub_burst = FALSE;
}


/**********************************************************************
* CalcDataRequestBlocks
*
* Compare allocation & actual data length. Return whichever has smaller
* data length.
*
* Returns:  buf_drqblk
* Params:   none
**********************************************************************/
BYTE CalcDataRequestBlocks(void)
{
    BYTE buf_drqblk;

    /* calculate min value of (1F4/1F5,HW_MAX_MULTB)
       the result will be saved in buf_drqblk as unit of block */

    if(St_dma_mode == TRUE)
        Iram_Xfr_allow = 0xFFFF;

        buf_drqblk = (BYTE)(Iram_Xfr_allow / Iram_shATB);

    if(buf_drqblk > Iram_Hw_host_blks_limit)
        buf_drqblk = Iram_Hw_host_blks_limit;

    if(buf_drqblk == 0)
        return(1);
    else
        return(buf_drqblk);
}




/*********************************************************************
* ATAGenerateHIRQ
*
* This routine will generate ATA INTRQ interrupt to HOST
*********************************************************************/
void ATAGenerateHIRQ(void)
{
    BYTE temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    Iram_shUMisc |= (HAL_HST_HINTRQW | bit15);

    WRITE_REG(HAL_HSTREG_MAP->UMISCR,Iram_shUMisc);

    Iram_shUMisc &= (~(HAL_HST_HINTRQW));

    SET_INT_ENABLE_MASK(temp_IE);
}


/*********************************************************************
* GetFlashUpdateFromHost(void)
*
* Get Update program data from host . (ex: mode sense data)
*
* Returns:  PASS or FAIL
* Params:   byte_count
*********************************************************************/
vBOOL GetFlashUpdateFromHost( USHORT T_page_count )     // for receive with page mode
{
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    ULONG   RecvTimer;
    USHORT  i, DataLength;
    BYTE    *DataPtr;
#endif
    vBOOL   temp_IE;

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        DataPtr = (BYTE *)(SCSDRAM + Iram_Linear_Xfer_addr);
        DataLength = T_page_count * 2048;
        s2b_atapi_para_received = FALSE;
        s2b_atapi_para_err = FALSE;

        if (DataLength > (RX_BUF_SIZE - 4))
        {
            SendMsgSpecifyXferLength(RX_BUF_SIZE - 4);
            DataLength -= (RX_BUF_SIZE - 4);
        }
        else
        {
            SendMsgSpecifyXferLength(DataLength);
            DataLength = 0;
        }

        StartTimer(&RecvTimer);
        while (ReadTimer(&RecvTimer) <= 2000L)
        {
            if (s2b_atapi_para_received == TRUE)
            {
                for (i = 0; i < s2b_byte_received; i++)
                {
                    if (i >= 3 && i < (s2b_byte_received - 1))
                        *DataPtr++ = rx_ring_buff[i];
                    rx_ring_buff[i] = ' ';
                }

                s2b_atapi_para_received = FALSE;

                if (DataLength == 0)
                {
                    return TRUE;
                }
                else if (DataLength > (RX_BUF_SIZE - 4))
                {
                    SendMsgSpecifyXferLength((RX_BUF_SIZE - 4));
                    DataLength -= (RX_BUF_SIZE - 4);
                }
                else
                {
                    SendMsgSpecifyXferLength(DataLength);
                    DataLength = 0;
                }
                StartTimer(&RecvTimer);
            }

            if (s2b_atapi_para_err == TRUE)
            {
                s2b_atapi_para_err = FALSE;
                s2bBUILD_SENSE(0xFF,0x08,0x02);
            }
        }

        return FALSE;
    }
#endif

    temp_IE = GET_INT_ENABLE_MASK();                    /* interrupt disables */
    DISABLE_INTERRUPT();

    Iram_shATB = T_page_count * 0x800;
    Iram_shHstWdC = 0x400;
    Iram_shHstBytAC = 0;

    /* Set the start page */
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, (Iram_Linear_Xfer_addr / 0x800));

    /* Set Buffer Compare Page */
    WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, (Iram_Linear_Xfer_addr / 0x800));

    /* Set the BYTE count */
    if ( St_dma_mode == FALSE )
    {
        WRITE_REG(HAL_HSTREG_MAP->ATBLO, Iram_shATB);
    }

    /* Set the offset bytes of page */
    WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, (ULONG)Iram_shHstBytAC);

    /* Set the USHORT counter */
    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, (USHORT)Iram_shHstWdC);

#if (CHIP_REV >= 0xC0)
            WRITE_FIELD(HAL_HST_RESWDCL,0x00);
            WRITE_FIELD(HAL_HST_RESWDCH,0x00);
            WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif
    /* Set the Host Trasfer Direction Register */
    Iram_shHDDir &= (~HAL_HST_HBODIS);     /* Select page/offset mode */
    Iram_shHDDir |= HAL_HST_HXDIR;         /* Set Direction: host -> device */
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);

    //first transfer, we have to setup page count register
    WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, T_page_count);
    WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, T_page_count);

    Get_para_data = TRUE;

    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);    /* START_XFER */

    SET_INT_ENABLE_MASK(temp_IE);   /* interrupt enables */

    while ( 1 )
    {   /* wait loop for data xfer */
        if ( Get_para_data == FALSE )
        {
            return TRUE;
        }
    }
    return FALSE;
}
