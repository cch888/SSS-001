/****************************************************************************
*                 (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME      hif_im.c
*
* DESCRIPTION   Contains functions that are placed in instruction memory to maximize
*               execution time.
*
*   $Revision: 391 $
*   $Date: 11/05/03 2:42p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\constant.h"
#include ".\common\commath.h"
#include ".\common\error.h"
#include ".\hif\at_equ.h"
#include ".\hif\rd_int.h"
#include ".\hif\rd_util.h"
#if (AUDIO_PLAYBACK == 1)
#include ".\hif\nx_audio.h"
#endif
#include ".\hif\gp_gtcfg.h"
#include ".\hif\app_if.h"
#include ".\servo\svo.h"
#include ".\player\plrDb.h"
#include ".\hif\gp_mac.h"
#include ".\dvd\dvgvars.h"
#include ".\dvd\dvISR.h"
#include ".\dvd\dvhostrd.h"
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
#include ".\dvd\dvMacros.h"
#endif

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#include ".\defmgmt\dvdram\ramDef.h"
#endif

#if BD_ENABLE
#include ".\bd\bdmacro.h"
#include ".\bd\bdVars.h"
#endif

#if BD_READ
#include ".\bd\bdisr.h"
#if (ENABLE_DISCARD_ICE == 1)
#include ".\bd\bdutil.h"
#include ".\bd\bddiag.h"
#include ".\bd\bdmacro.h"
#include ".\bd\bddiscrd.h"
#include ".\bd\bdisr.h"
#endif
#endif

#include ".\AL\HAL_wobble.h"
#include ".\AL\reg_intctrl.h"
#include ".\al\reg_cd_decoder.h"
#include ".\al\reg_cd_encoder.h"
#include ".\al\reg_cpycmp.h"
#include ".\al\reg_dvd_decoder.h"
#include ".\al\reg_dvd_encoder.h"
#include ".\al\reg_global.h"
#include ".\al\reg_servo.h"
#include ".\al\reg_wobble.h"
#include ".\al\reg_write_strategy.h"
#include ".\al\reg_dfe.h"
#include ".\al\hal_dfe.h"
#include ".\player\plrinfo.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_lib.h"
#include ".\AL\REG_demod.h"
#include ".\servo\svo_dsp.h"

#if(ENABLE_HDDVD && ENABLE_AACS)
#include ".\al\reg_demod.h"
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_function_test.h"
#endif

extern void ATACommandIntProc(void);
extern void ATAPIDeviceResetCommand(void);
extern void ATAPIDeviceResetNoFIS34(void);
extern void ataExecuteDiagnosticCommand(void);
extern void ATASoftResetService(void);
extern void GetPacketCommand(void);
extern void  ComresetIntProc(void);

extern void ESFSIntProc(void);
extern void ATIPMSFReadyIntProc(void);
extern void read_common(void);
extern void SetSCRegField_2B(ULONG, USHORT, BYTE, USHORT);
extern void SetSCRegField_1B(ULONG, BYTE, BYTE, BYTE);
extern void InquiryCommand(void);
extern vBOOL CheckValidMSF(BYTE *);
extern void ClearReadAheadSettings(void);
extern void ReadCDExit(void);
extern BYTE CheckTrackWriteMethod(long);
extern vBOOL Method2LBAToMethod1LBA(long, long *);

void CheckForReadAheadBufferingStart(void);
void ataCheckDMA(void);

long    cmd_sao_start_lba;

extern void ResetHandler(void);
extern BYTE bottom_of_heap;             /* defined in heap.s */
#if (FLASH_BY_DISC == 1)
void SaveCurrentTransferSettings(BYTE Mode);  // Mode 0: normal , Mode 1: disc flush
#else
void SaveCurrentTransferSettings(void);
#endif
extern BYTE Send_timef;
//extern vBYTE Iram_WGateUp;
extern BYTE Iram_EncoderStarted;

#if ENABLE_AACS
ULONG RomMarkKeyBuffer[4];
#endif


// Local function prototypes
void CdDecoderIrqHandler(void);
void svoSubqIsr(void);
void diagC1C2();

#if HD_READ == 1
void HdDecoderIrqHandler(void);
#endif

void DecoderISR();
void DigitalFrontEndISR();

#if ((ENABLE_DFE_ICE_DETECT == 1)&&(BD_ENABLE == 1))
ULONG IcedClusterStAddr;
#endif

#if(ENABLE_HDDVD && ENABLE_AACS)
void DemodISR();
#endif
//Start == Add switch for checking tray state when Comreset
#define CHECK_TRAY_IN_COMRESET      1
//End == Add switch for checking tray state when Comreset

#pragma arm section code = "FROHead"
/*********************************************************************
* Function:     FindRunOutHeader()
*
* Description:
*
* Returns:      none
* Params:       none
*********************************************************************/
extern  volatile TimeS  Iram_FoundRunOut1Time;
extern  volatile TimeS  Iram_FoundRunOut2Time;
void FindRunOutHeader(void)
{
    BYTE    min,sec,frm,mode,a;
    BYTE    regHMode;
    TimeS   regHTime;
    TimeS   time;

    min = ReadSCReg_1B(HAL_CDDREG_MAP->HEAD0);
    sec = ReadSCReg_1B(HAL_CDDREG_MAP->HEAD1);
    frm = ReadSCReg_1B(HAL_CDDREG_MAP->HEAD2);
    mode = ReadSCReg_1B(HAL_CDDREG_MAP->HEAD3);

    regHMode        = mode;
    regHTime.Minute = min;
    regHTime.Second = sec;
    regHTime.Frame  = frm;

    if (!(regHMode & 0x1C)){
        if ((regHMode & 0x03) != 0x03){
            a = (regHMode & 0xE0);
            if(a == 0xE0){
                if(CheckBCDTime(&regHTime) == TRUE){
                    time.Minute = BCD2HEX(regHTime.Minute);
                    time.Second = BCD2HEX(regHTime.Second);
                    time.Frame  = BCD2HEX(regHTime.Frame);

                    Iram_FoundRunOut1Time   = time;
                    Iram_FoundRunOut_valid  |= eRUNOUT_1_VALID;
                }
            }else if(a == 0xC0){
                if(CheckBCDTime(&regHTime) == TRUE){
                    time.Minute = BCD2HEX(regHTime.Minute);
                    time.Second = BCD2HEX(regHTime.Second);
                    time.Frame  = BCD2HEX(regHTime.Frame);
                    Iram_FoundRunOut2Time   = time;
                    Iram_FoundRunOut_valid  |= eRUNOUT_2_VALID;
                }
            }
        }
    }
}
#pragma arm section code

/*********************************************************************
* EncoderISR
*              The predecessor of this function was called  CDIRQHandler
*
* CD IRQ service routine process sources of interrupts:
*     (1) Encoder interrupt CD
*     (2) Encoder interrupt DVD
*     (3) Encoder interrupt BD
*     (4) Encoder interrupt HD
*     (5) Write Strategy
*     (6) Copy and Compare Function Interrupt
*********************************************************************/

#if (BD_ENABLE)
extern BOOL bd_wgatemon_workaround;
#endif

void EncoderISR(void)         //  The predecessor of this function was called  CDIRQHandler
{
    BYTE DiscLoaded;
    BYTE CopyIntStatus;
    ULONG svAddr, IntReason;

    svAddr = Iram_Buf_dram_addr;

    IntReason = READ_REG(HAL_AHB_INTCTRLREG_MAP->ICIRQST);
    DiscLoaded = READ_FIELD(HAL_GLOBAL_DISCSTD);   // Get "Disc Standard Selection" directly from register to speed up ISR code. Do not use function pdbDiscLoaded()

    if (READ_FIELDFROM(IntReason,HAL_AHB_INTCTRL_CPYINTS))
    {
        /* ------------------------------ */
        /* Copy Interrupt Status Register */
        /* ------------------------------ */
        CopyIntStatus = READ_REG(HAL_CPYCMPREG_MAP->CPYINT) & READ_REG(HAL_CPYCMPREG_MAP->CPYINTEN);
        WRITE_REG(HAL_CPYCMPREG_MAP->CPYINT, ~CopyIntStatus);

        if (CopyIntStatus & HAL_CPYCMP_CPYDONE)  /* Copy Function */
        {
            St_copy_done = TRUE;
        }
        else if (CopyIntStatus & HAL_CPYCMP_CMPDONE)  /* Compare Function */
        {
            St_comp_done = TRUE;
        }
        else if (CopyIntStatus & HAL_CPYCMP_PFILDONE) /* Pattern Fill Function */
        {
            St_pfil_done = TRUE;
        }
        else if (CopyIntStatus & HAL_CPYCMP_DCPYDONE) /* Decrementing Address Copy Function */
        {
            St_dcpy_done = TRUE;
        }
    }
    Iram_Buf_dram_addr = svAddr;
}

/*********************************************************************
* HostIRQService
*
* Process host interface interrupts:
*     (1) data transfer interrupt
*     (2) ATA command selected
*     (3) ATAPI command selected
*     (4) Diagnostic command
*     (5) ATAPI 08 Reset command
*     (6) ATA SRST
*     (7) AUdio Block int
*********************************************************************/
void HostIRQService(void)
{
BYTE HSts0;
BYTE HSts1;
BYTE ATACmd;
ULONG svAddr, IntReason;

    svAddr = Iram_Buf_dram_addr;

    IntReason = READ_REG(HAL_AHB_INTCTRLREG_MAP->ICIRQST);

    if (READ_FIELDFROM(IntReason,HAL_AHB_INTCTRL_HSTINTS))
    {
        HSts0 = READ_REG(HAL_HSTREG_MAP->HINT0) & READ_REG(HAL_HSTREG_MAP->HINTEN0);    /* HIF interrupt */


#if (AUTO_STATU_CPLT==1)
        if (HSts0)
        {
            if (HSts0 & HAL_HST_DTE)
            {
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_DTE));
                if(Iram_StartMultiBlock == TRUE)
                {
                    Iram_StartMultiBlock = FALSE;
                }
                else
                {
                    HostTransferEndIntProc();
                }

                // For Dell PC, the DTE & ATAPI & HAL_HST_CMD Cmd interrupts will happen at the same time.
                if (HSts0 & HAL_HST_PCMD)
                {
                    /* ATAPI packet command interrupt */
                    GetPacketCommand();       /* get 12 BYTE packet command */
                    //WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));
                    //show message when DTE and ATAPI interrupt at same time!!
                    if (HSts0 & HAL_HST_DTE)
                        send_msg80(1,0x710600);//send_msg5S(1,"ATAPI DTE");
                }
                // For some USB-SATA bridge board, the DTE & ATA Cmd interrupts will
                // happen at the same time. So we need to handle the remaining ATA Cmd
                // interrupt here to avoid missing ATA Cmd.
                else if (HSts0 & HAL_HST_CMD)   /* ATA command interrupt */
                {
                    WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_CMD));
                    ATACmd = READ_REG(HAL_HSTREG_MAP->ATCMD);  /* read AT command       */
                    if ( ATACmd != 0xA0 )   /* if A0 packet command? */
                    {   /* not A0 packet cmd     */
                        if ( ATACmd == ATA_EXEC_DEVICE_DIAG )
                            ataExecuteDiagnosticCommand();   /* execute diagnostic command */
                        else
                            ATACommandIntProc(); /* other ATA command     */

                        //show message when DTE and ATA interrupt at same time!!
                        if (HSts0 & HAL_HST_DTE)
                            send_msg80(1,0x710601);//send_msg5S(1,"ATA DTE");
                    }
                }
            }
            else if (HSts0 & HAL_HST_PCMD)/* Move ATAPI Command Int process to front because DTE comes with it */
            { /* ATAPI packet command interrupt */
                GetPacketCommand();       /* get 12 BYTE packet command */
                //WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));
            }
            else if (HSts0 & HAL_HST_SRST)
            {   /* host SRST interrupt  */
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_SRST));
                ATASoftResetService();
            }
            else if (HSts0 & HAL_HST_ARST)
            {   /* ATAPI software reset cmd(08)*/
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_CMD|HAL_HST_ARST)));
                ATAPIDeviceResetCommand();
            }
            else if (HSts0 & HAL_HST_CMD)   /* ATA command interrupt */
            {
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_CMD));
                ATACmd = READ_REG(HAL_HSTREG_MAP->ATCMD);  /* read AT command       */
                if ( ATACmd != 0xA0 )   /* if A0 packet command? */
                {   /* not A0 packet cmd     */
                    if ( ATACmd == ATA_EXEC_DEVICE_DIAG )
                        ataExecuteDiagnosticCommand();   /* execute diagnostic command */
                    else
                        ATACommandIntProc(); /* other ATA command     */
                }
            }
        }
#else
        if (HSts0)
        {
            Iram_SATA_Idle_counter = SATA_STANDBYCNT;
            SATA_intf_pwr_state = 0;
    #if (EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3)
            SendFis34Already = 1; //Win7 AHCI mode HIPM fail.
    #endif

            if (HSts0 & HAL_HST_DTE)
            {
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_DTE));
                if(Iram_StartMultiBlock == TRUE)
                {
                    Iram_StartMultiBlock = FALSE;
                }
                else
                {
                    HostTransferEndIntProc();
                }

                // For Dell PC, the DTE & ATAPI & HAL_HST_CMD Cmd interrupts will happen at the same time.
                if (HSts0 & HAL_HST_PCMD)
                {
                    /* ATAPI packet command interrupt */
                    GetPacketCommand();       /* get 12 BYTE packet command */
                    //WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));
                    //show message when DTE and ATAPI interrupt at same time!!
                    if (HSts0 & HAL_HST_DTE)
                        send_msg80(1,0x710600);//send_msg5S(1,"ATAPI DTE");
                }
                // For some USB-SATA bridge board, the DTE & ATA Cmd interrupts will
                // happen at the same time. So we need to handle the remaining ATA Cmd
                // interrupt here to avoid missing ATA Cmd.
                else if (HSts0 & HAL_HST_CMD)   /* ATA command interrupt */
                {
                    WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_CMD));
                    ATACmd = READ_REG(HAL_HSTREG_MAP->ATCMD);  /* read AT command       */
                    if ( ATACmd != 0xA0 )   /* if A0 packet command? */
                    {   /* not A0 packet cmd     */
                        if ( ATACmd == ATA_EXEC_DEVICE_DIAG )
                            ataExecuteDiagnosticCommand();   /* execute diagnostic command */
                        else
                            ATACommandIntProc(); /* other ATA command     */

                        //show message when DTE and ATA interrupt at same time!!
                        if (HSts0 & HAL_HST_DTE)
                            send_msg80(1,0x710601);//send_msg5S(1,"ATA DTE");
                    }
                }

            }
            else if (HSts0 & HAL_HST_PCMD)   /* Move ATAPI Command Int process to front because DTE comes with it */
            {   /* ATAPI packet command interrupt */
                GetPacketCommand();       /* get 12 BYTE packet command */
                //WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));
                //show message when DTE and ATAPI interrupt at same time!!
                if (HSts0 & HAL_HST_DTE)
                    send_msg80(1,0x710600);//send_msg5S(1,"ATAPI DTE");
            }
            else if (HSts0 & HAL_HST_SRST)
            {   /* host SRST interrupt  */
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_SRST));
                ATASoftResetService();
            }
            else if (HSts0 & HAL_HST_ARST)
            {   /* ATAPI software reset cmd(08)*/
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_CMD|HAL_HST_ARST)));
                ATAPIDeviceResetCommand();
            }
            else if (HSts0 & HAL_HST_CMD)   /* ATA command interrupt */
            {
                WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~HAL_HST_CMD));
                ATACmd = READ_REG(HAL_HSTREG_MAP->ATCMD);  /* read AT command       */
                if ( ATACmd != 0xA0 )   /* if A0 packet command? */
                {   /* not A0 packet cmd     */
                    if ( ATACmd == ATA_EXEC_DEVICE_DIAG )
                        ataExecuteDiagnosticCommand();   /* execute diagnostic command */
                    else
                        ATACommandIntProc(); /* other ATA command     */
                }
            }
        }
#endif//AUTO_STATU_CPLT

        HSts1 = READ_REG(HAL_HSTREG_MAP->SATAINT) & READ_REG(HAL_HSTREG_MAP->SATAINTEN);
        if (HSts1)
        {
            WRITE_REG(HAL_HSTREG_MAP->SATAINT, (BYTE)(~ HSts1));
            if (HSts1 & HAL_HST_BISTIP)
            {
                send_msg80(1,0x71060A);
                BIST_In_Process = 1;
            }

            if (HSts1 & HAL_HST_RXPMACK)
            {
                send_msg80(1,0x71060B);
                if (SATA_intf_pwr_state == bIPMREQ_P)
                    SATA_intf_pwr_state = bIPMACK_P;
                else if (SATA_intf_pwr_state == bIPMREQ_S)
                    SATA_intf_pwr_state = bIPMACK_S;
            }
            if (HSts1 & HAL_HST_RXPMNAK)
            {
                send_msg80(1,0x71060C);
                SATA_intf_pwr_state = 0;
            }

            if (HSts1 & HAL_HST_ERRTXRX)
            {
                send_msg80(1,0x71060D);
                LinkTxRxErr = 1;
            }

#if (ENABLE_AUTO_ABORT == 1)
            if (HSts1 & HAL_HST_ABORT)
            {
                send_msg80(1,0x71060E);
                AutoAbort = 1;
            }
#endif

        }

        HSts1 =  READ_REG(HAL_HSTREG_MAP->SATAINT1) & READ_REG(HAL_HSTREG_MAP->SATINTEN1);
        if (HSts1)
        {
            WRITE_REG(HAL_HSTREG_MAP->SATAINT1, (BYTE)(~ HSts1));
            if (HSts1 & HAL_HST_COMRSTINT)
            {
                ComresetIntProc();
            }
        }
    }   // End of if(IntReason & bHSTIntS)

#if (AUDIO_PLAYBACK == 1)
    if (READ_FIELDFROM(IntReason,HAL_AHB_INTCTRL_AUDINTS))
    {   // Audio Interrupt
        if ( READ_FIELD(HAL_AUD_ABLKINT))                     /* Check if Audio Block Interrupt is set */
        {
            CLEAR_INT_FIELD(HAL_AUD_ABLKINT, 0);  /* Clear Audio Block Interrupt */
            AudioIntProc();                  /* Process Audio interrupt */
        }
    }
#endif
    Iram_Buf_dram_addr = svAddr;
}

#if GETPACKETCOMMAND_IN_FLASH == 0
/*********************************************************************
* GetPacketCommand()
*
* Get 12 command packet bytes from ATAPIData FIFO and store all these
* bytes to command array
*********************************************************************/
extern void GetEventStatusNotificationCommand(void);
void GetPacketCommand()
{
    BYTE a;
    FLAG    FastCacheCheck;
    #if (B1_CHIP_SATA_WA == 1)
    BYTE rxfifolv;
    #endif
    ataCheckDMA();                          /* check if host request DMA */

    /* read 1f4/1f5 ATA Byte Count regs:max data can be xfer in every drq */
    Iram_Xfr_allow = Iram_shATB = READ_REG(HAL_HSTREG_MAP->ATBLO) & 0xFFFE;

    /* get 12 bytes packet command */
    #if (B1_CHIP_SATA_WA == 1)
    rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
    while(rxfifolv != 0x03) //make sure we can get correct data from FIFO.
    {
        rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
//        send_msg5SValue(1,"RXFIF",4,rxfifolv);
    }
    #endif
    for (a=0; a<12; a++)                    /* get 12 bytes packet command */
    {
        Iram_Cmd_block.bf[a] = READ_REG(HAL_HSTREG_MAP->ATAPIDATA);
    }

    WRITE_REG(HAL_HSTREG_MAP->HINT0, (BYTE)(~(HAL_HST_PCMD|HAL_HST_CMD)));

    #if (B1_CHIP_SATA_WA == 1)
    rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
    while(rxfifolv!=0x80)//make sure the FIFO is empty.
    {
        rxfifolv = READ_REG(HAL_HSTREG_MAP->RXFIFOSAT);
    }
    #endif
    send_atapi_cmd_time(SEND_ATAPI_CMD);
    //send_atapi_cmd(SEND_ATAPI_CMD);
    StartTimer(&Iram_atapi_timer);
    /* If the 12-byte CDB is corrupted, then discard this command */
    if ((READ_FIELD(HAL_HST_UCRCERR)) && (Iram_Cmd_block.bf[0] != SPC_REQUEST_SENSE))
    {
        send_msg80(1,0x710603);
        AutoCommandComplete();
        return;
    }

    if(ataPowerMode == SLEEP_MODE){
        WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);  /* No Error */
        WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x10);  /* set DRDY, DSC */
        ATAClearBusy();
        return;
    }

    /*For doing offline power calibration command do not check Unit Attention to execute this vendor command*/
#if (SUN_FUNC_TEST == 1)
    if(((Iram_Cmd_block.bf[0] == DIAG_CMD_OFFLINE_CALIBRATION) || (Iram_Cmd_block.bf[0] == DIAG_LASER)) && (St_unit_atten == TRUE))
#else
    if((Iram_Cmd_block.bf[0] == DIAG_CMD_OFFLINE_CALIBRATION) && (St_unit_atten == TRUE))
#endif        
        St_unit_atten = FALSE;



    if ( (Iram_Cmd_block.bf[0] == SBC_VERIFY_10) || (Iram_Cmd_block.bf[0] == SBC_VERIFY_12) )
    {
        Iram_Xfr_allow = 0xFFFE;
        Iram_shATB    = 0xFFFE;
    }

    if( Iram_Cmd_block.bf[0] != 0x00 || Iram_Cmd_block.bf[0] != 0x03)
    {
        a = READ_REG(HAL_HSTREG_MAP->ATAPIDATA);
    }
    switch (Iram_Cmd_block.bf[0])
    {
    /* Inquiry command */
        case SPC_INQUIRY:
        {
            InquiryCommand();
            return;
        }
        case MMC_GET_EVENT_STATUS_NOTIFICATION:
        {
            GetEventStatusNotificationCommand();
            return;
        }
        case MMC_GET_CONFIGURATION:
        {
            GetConfigurationCommand();
            return;
        }
        case SPC_REQUEST_SENSE:
        {
            RequestSenseCommand();
            return;
        }
        case MMC_MECHANISM_STATUS:
        {
            MechanismStatusCommand();
            return;
        }
    }
    /* Set Read Ahead operation Check */
    if ( (Iram_rd_ahead_chk_req == RD_AHEAD_BUF_REQ) || (Iram_rd_ahead_chk_req == RD_AHEAD_BUF_DONE) )
    {
        if ( (Iram_Cmd_block.bf[0] == SBC_READ_10) || (Iram_Cmd_block.bf[0] == SBC_READ_12) ||
             (Iram_Cmd_block.bf[0] == MMC_READ_CD) || (Iram_Cmd_block.bf[0] == MMC_READ_CD_MSF) )
            {
                CheckForReadAheadBufferingStart();
            }
    }

    /* If any disc that contains defect management, do not run Fast Cache Check */
    FastCacheCheck = TRUE;
#if(BD_ENABLE == 1)
    if ((pdbDiscLoaded() == BD_DISC) && (St_become_ready == FALSE))
    {
        #if (!BDWT_FAST_CACHE)
        if (pdbGetBDType() == BD_R || pdbGetBDType() == BD_RE)
        {
            FastCacheCheck = FALSE;
        }
        #endif
    }
    else
#endif
    if (pdbDiscLoaded() == DVD_DISC)
    {
        #if ((DVD_RAM_READ == 1) && (!DvdRamFastCache))
        if ( pdbGetDVDType() == DVD_RAM )
            FastCacheCheck = FALSE;
        #endif
    }

    if (FastCacheCheck)
    {
        if( CheckForFastCache() == TRUE)   return;
    }

    /* If it is seek, clear DSC & DRQ bits in Status register to
       indicates seek not completed yet. */
    if ( Iram_Cmd_block.bf[0] == SBC_SEEK )
        WRITE_REG(HAL_HSTREG_MAP->ATCMD, HAL_HST_DRDYW);  /* [627h] status reg, clear DSC*/
    else
        WRITE_REG(HAL_HSTREG_MAP->ATCMD, (HAL_HST_DRDYW|HAL_HST_DSCW)); /* [627h] status reg, Set DSC*/

    Knl_cmd_avail = TRUE;                       /* Indicate new command is available. */
}
#endif

/*********************************************************************
* ataCheckDMA
*
* This routine will check DMA from ATAPI Feature register
*********************************************************************/
void ataCheckDMA(void)
{
    BYTE  v1;

    WRITE_REG(HAL_HSTREG_MAP->ATFEA, 0x00); /*[21h] clear previous error message */

    v1 = READ_REG(HAL_HSTREG_MAP->ATFEA);
    if ( v1 == 0xff )
        return;

    v1 &= HAL_HST_DMA;

    if (v1 == HAL_HST_DMA)
    {
        St_dma_mode = TRUE;                 /* if DMA mode */

        if (St_ultra_dma ==FALSE)/* */
            { /* DMA mode */
                Iram_shHDDir &=0xF3;/* Clear the mode bits first  */
                Iram_shHDDir |=  HAL_HST_DMAMODE; /* set DMA mode */
                WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
            }
        else
            {   /* DMA */
                Iram_shHDDir &=0xF3;/* Clear the mode bits first  */
                Iram_shHDDir |= HAL_HST_DMAMODE;  /*  set to DMA mode */
                WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
            }
    }
    else
    {   /* PIO modes */
        St_dma_mode = FALSE;
        Iram_shHDDir &= 0xF3;
        WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
    }
}

/*********************************************************************
* Function:     CdDSPIrqHandler()
*
* Description:  Function is executed when a Decoder Interrupt has has taken place
*                 whose origin is in the CD Decoder Module.
*
* Returns:      none
* Params:       none
*********************************************************************/
void CdDSPIrqHandler(void)
{
    BYTE IntStatus,IntStatus1;
    // check the CDDSP INTERRUPT REGISTER
    IntStatus = READ_REG(HAL_CDDREG_MAP->INT1STAT);

    IntStatus1 = IntStatus & READ_REG(HAL_CDDREG_MAP->INT1EN);

    if(IntStatus1)
    {
        if ((IntStatus1 & HAL_CDD_SCOR) == HAL_CDD_SCOR)
        {
            svoSubqIsr();
        }
#if (E_DIAGS == 1)
        if ((IntStatus1 & HAL_CDD_CEPSERR) == HAL_CDD_CEPSERR)
        {
            diagC1C2();
        }
#endif
    }
    WRITE_REG(HAL_CDDREG_MAP->INT1STAT, ~IntStatus);
}

/*********************************************************************
 *
 * Module: svoSubqIsr
 * Description: servo read SUBQ interrupt service routine
 *
 **********************************************************************/
extern vULONG Iram_Buf_dram_addr;
extern void   subQmode1(SubQCodeS *subqdata);
extern void   getsubQcode(BYTE *subqdata);
WORD DMD_value; // ==  change reason modify for DQA Disc to retry DFE parameter(modify f.22)
BYTE DMD_CNT; // ==  change reason modify for DQA Disc to retry DFE parameter(modify f.22)
void svoSubqIsr(void)
{
    BYTE    subqdata[10];
    BOOL    CrcOk;

    ULONG   svAddr;

#if (SUN_FUNC_TEST == 1)
    USHORT CurC1, CurC2;
#endif   

    svAddr = Iram_Buf_dram_addr;

    CrcOk = (BOOL) READ_FIELD(HAL_CDD_SQCRCOK);

    svoIramVar.subQReadyFlag = FALSE;

    if (!CrcOk)
    {                       /* SUBQ not ready and error */
        pdbSetSubCodeOK(FALSE);
        Iram_Buf_dram_addr = svAddr;
        if (((PMANotOk & 0x80) == 0x80) && (PMANotOk  < 0xff))
        {
            PMANotOk++;
        }
        return;
    }
    // == start ==modify for DQA Disc to retry DFE parameter(modify f.22)
    DMD_CNT++;
    if(DMD_CNT==120)
    {
        DMD_CNT=0;
        DMD_value=READ_FIELD(HAL_DMOD_DMDERACNT);
    }
    //== end ==modify for DQA Disc to retry DFE parameter(modify f.22)

    getsubQcode(&subqdata[0]);
    Iram_fSubQIntFlag           = TRUE;
    Iram_DiscInfo.CD.QSubcodeValid = TRUE;
    Iram_fEFMValid              = TRUE;

#if (SUN_FUNC_TEST == 1)
    if (fStartBuffer == TRUE)
    {   
        usBlerCount++;
        CurC1 = READ_REG(HAL_CDDREG_MAP->C1CNTR);
        CurC2 = READ_REG(HAL_CDDREG_MAP->C2CNTR);
        usSUM_PI_C1 += CurC1;
        usSUM_PO_C2 += CurC2;     
       
        if (usBlerCount == usBLER_UNIT)
        {
            usUnitCount++;
            usBlerCount = 0;   
        }                
    }
#endif   

    #if (MSG_OUT_MODE == MSG_USBPORT)
    if (Send_timef && (hifDebugMsgFlag & SEND_QCODE_DATAID))
    {
        send_qcode(1);
    }
    #endif

    switch(((SubQCodeS *)subqdata)->Addr){
        case eSubcodeMode1:
            subQmode1((SubQCodeS *)subqdata);
            break;

        case eSubcodeMode2:
                    {
                        pdbSetCatalogCode((SubQCodeS *)subqdata);
                    }
            break;

        case eSubcodeMode3:
            if(svoIramVar.currentTime.Direction == PLUS)
            {
                if(pdbGetPlayerStatus() == eStSearchISRCAndCatNum)
                {
                    if ((((SubQCodeS *)subqdata)->Second & 0x03) == 0x03){
                        pdbSetRIDCode((SubQCodeS *)subqdata);
                    }
                    if (!(((SubQCodeS *)subqdata)->Second & 0x03)){

                        pdbSetISRCCode((SubQCodeS *)subqdata);
                    }
                }
            }
            break;
    }

    Iram_Buf_dram_addr = svAddr;
} /* svoSubqIsr */

#if (E_DIAGS == 1)
#pragma arm section code = "DiagC1C2"
/*-----------------------------------------------------------------------------
 *
 * Module: diagC1C2
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE   seekTestFlag;
BYTE   seekTargetMinute;
USHORT errCount1, errCount2;
USHORT storedInDRAM;            /* stored count in bytes in DRAM */
vULONG seekTestDRAMaddr;        /* stored address in DRAM */
vULONG diaBlerStartLBA;
vULONG diaBlerStopLBA;

void diagC1C2()
{
    USHORT tempShort;

    /* ------------------------------------------------------------ */
    /*                  Diagnostic BLER Seek Test                   */
    /* ------------------------------------------------------------ */
    if(seekTestFlag)
    {
        BYTE minute, second, statReg, tempByte;

        minute = svoIramVar.currentTime.Time.Minute;
        second = svoIramVar.currentTime.Time.Second;

        if ((svoIramVar.currentID.SectorNum.all32s - 150) <  diaBlerStopLBA)
        {
            // STORE ONLY WHEN NEW FRAME!
            // Read the CD-DSP Interrupt Status
            statReg= (BYTE)READ_REG(HAL_CDDREG_MAP->INT1STAT);
            // READ C1 Count
            errCount1 = (USHORT)READ_FIELD(HAL_CDD_C1EPSCNT);   /* C1EPSCR C1 Error/Sec (EPS) Frame Threshold */
            // MINUTE
            SCSDRAM[seekTestDRAMaddr++] = minute;
            // SECONDS
            SCSDRAM[seekTestDRAMaddr++] = second;
            tempByte = (BYTE)(errCount1>>8);
            // C1 ERROR COUNT
            if (errCount1 > 0xFF)
                SCSDRAM[seekTestDRAMaddr++] = 0xFF;
            else
                SCSDRAM[seekTestDRAMaddr++] = (BYTE)errCount1;

            // READ C2 Count
            errCount2 = (USHORT)READ_FIELD(HAL_CDD_C2EPSCNT);
            if (errCount2 > 0xFF)
                SCSDRAM[seekTestDRAMaddr++] = 0xFF;
            else
                SCSDRAM[seekTestDRAMaddr++] = (BYTE)errCount2;

            storedInDRAM = storedInDRAM + 4;

        }
        else
        {
            SCSDRAM[0x000000] = 0x00;
            SCSDRAM[0x000001] = 0x00;
            tempShort = (storedInDRAM >> 8);
            SCSDRAM[0x000002] = (BYTE)tempShort;
            SCSDRAM[0x000003] = (BYTE)storedInDRAM;
            seekTestFlag = FALSE;
            ReadyLED(LED_ON);
        }
    } // if(seekTestFlag)
}
#pragma arm section code
#endif /* (E_DIAGS == 1) */

/*********************************************************************
* Function:     CdDecoderIrqHandler()
*
* Description:  Function is executed when a Decoder Interrupt has has taken place
*                 whose origin is in the CD Decoder Module.
*
* Returns:      none
* Params:       none
*********************************************************************/
void CdDecoderIrqHandler(void)
{
    BYTE DSts0, DSts1, DSts2;

        DSts0 = READ_REG(HAL_CDDREG_MAP->DINT0) & READ_REG(HAL_CDDREG_MAP->DINTEN0);  /* Decoder interrupt status 0 */
        WRITE_REG(HAL_CDDREG_MAP->DINT0, ~DSts0);

        if (DSts0 & 0xF7)
            CDDecoderInt1(DSts0);

        DSts1 = READ_REG(HAL_CDDREG_MAP->DINT1) & READ_REG(HAL_CDDREG_MAP->DINTEN1);  /* Decoder interrupt status 1 */
        WRITE_REG(HAL_CDDREG_MAP->DINT1, ~DSts1);

        if (DSts1)
            CDDecoderInt2(DSts1);

        DSts2 = READ_REG(HAL_CDDREG_MAP->DINT2) & READ_REG(HAL_CDDREG_MAP->DINTEN2);     /* Decoder interrupt status 2 */
        WRITE_REG(HAL_CDDREG_MAP->DINT2, ~DSts2);

        if (DSts2)
        {
#if 0  // This interrupt is not used. Keeping structure for future use!
            if (DSts2 & HAL_CDD_BSYNC)      /* Block sync detected interrupt */
            {
                CLEAR_INT_FIELD(HAL_CDD_BSYNC, 0);
            }
#endif
            if (DSts2 & HAL_CDD_ROUT1)        /* Run-out 1 detected Interrupt */
            {
                RunOutIntProc();           /* run-out block */
            }
#if 0  // This interrupt is not used. Keeping structure for future use!
            else if (DSts2 & HAL_CDD_SUBRDY)     /* Subcode ready Interrupt */
            {
            // TBD? No Code!
            }
#endif

            if (DSts2 & HAL_CDD_HDRDY)      /* CD I/F Header ready Interrupt */
            {
                if(Iram_FindRunOutReqFlag == TRUE)
                    FindRunOutHeader();
            }

            if (DSts2 & HAL_CDD_RIN4)       /* Run-in 4 detected */
            {
                RunInEndIntProc();
            }

            if (DSts2 & HAL_CDD_TFND)       /* Target found Interrupt */
            {
                ReadStartIntProc();
            }

            if (DSts2 & HAL_CDD_DPRD)       /* Periodic decoder interrupt */
            {
                CDDecoderIntProc();
            }
#if 0  // This interrupt is not used. Keeping structure for future use!
            else if (DSts2 & HAL_CDD_QADDR23)/* sub-Q block address 2&3 interrupt */
            {
            // TBD? No Code!
            }
#endif
        }
}

/*********************************************************************
* DecoderISR
*
* Decoder IRQ service routine process sources of interrupts:
*     (1) CD  Decoder interrupt, CD DSP interrupt
*     (2) DVD Decoder interrupt
*     (3) BD  Decoder interrupt
*     (4) HD  Decoder interrupt
*
*********************************************************************/
void DecoderISR()
{
    BYTE  DiscLoaded;

    DiscLoaded = READ_FIELD(HAL_GLOBAL_DISCSTD);  // Get "Disc Standard Selection" directly from register to speed up ISR code. Do not use function pdbDiscLoaded()

    if (DiscLoaded == halGLOBAL_DiscStd_CD)
    {
        /*----------------------------------------------------------------
            CD DSP INTERRUPT SERVICE
        -----------------------------------------------------------------*/
        CdDSPIrqHandler();
        /*----------------------------------------------------------------
            CD DECODER INTERRUPT SERVICE
        -----------------------------------------------------------------*/
        CdDecoderIrqHandler();
    }

    if (DiscLoaded == halGLOBAL_DiscStd_DVD)
    {
    #if (DVD_RAM_READ == 1)
        if(svoVar_discKindT.fDiscKind == eDVDRAMDisc)
        {
            BYTE DVIRQS1, DVIRQS2, DVIRQS3, Saved_DVDSTAT;
            DVIRQS1 = READ_REG(HAL_DVDDREG_MAP->DVDIRQS1) & READ_REG(HAL_DVDDREG_MAP->DVDIRQC1);
            DVIRQS2 = READ_REG(HAL_DVDDREG_MAP->DVDIRQS2) & READ_REG(HAL_DVDDREG_MAP->DVDIRQC2);
            DVIRQS3 = READ_REG(HAL_DVDDREG_MAP->DVDIRQS3) & READ_REG(HAL_DVDDREG_MAP->DVDIRQC3);
            Saved_DVDSTAT = READ_REG(HAL_DVDDREG_MAP->DVDSTAT);

            // Check if at blank sectors.
            if ((READ_FIELD(HAL_DFE_AMPLVLAGC) < 0x50))
            {
                // Preset offset and gain at blank sectors.
                WRITE_FIELD(HAL_DFE_PREOFFDAG, 1);
                WRITE_FIELD(HAL_DFE_PREOFFDAL, 1);
                WRITE_FIELD(HAL_DFE_PREGAIDAG, 1);
                WRITE_FIELD(HAL_DFE_PREGAIDAL, 1);
            }

            if( DVIRQS3 & HAL_DVDD_PIDI )
            {
                svoIramVar.headerIDReadyFlag = FALSE;
                if (Saved_DVDSTAT & HAL_DVDD_PIDVLD)
                {
                    RWDisc.dvRAM.CurrentPID = READ_REG(HAL_DVDDREG_MAP->PID0) & PSNMask;
                    svoIramVar.headerIDReadyFlag = TRUE;
                    //send_msg5SValue(1, "PID", 4, RWDisc.dvRAM.CurrentPID);

                    //if (svoIramVar.tracksize_set)
                    //{
                    //    svoIramVar.tracksize_set--;
                    //    if (svoIramVar.tracksize_set == 0)
                    //    {
                    //        WRITE_FIELD(HAL_DVDD_TRACKSIZE,0);    // Clear TRACKSIZE if it was set during seek.
                    //    }
                    //}
                }
                else
                {
                    /* If PID is invalid, we will auto increase CurrentPID and set error bitmap */
                    RWDisc.dvRAM.CurrentPID++;
                    //send_msg5SValue(1, "invalidPID", 4, RWDisc.dvRAM.CurrentPID);
                }

                #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
                //send_msg74(SEND_DVDRAM_ICE,0xBD0000,READ_FIELD(HAL_DFE_ICEHFAMP)); //ICEHFAMP =
                if(( DVD_BUFFERING() == ON) && (PrePID != (RWDisc.dvRAM.CurrentPID & DVD_BLOCK_MASK)))
                {
                    HAL_DFE_ClusterStateEnableInt();
                    if((RWDisc.dvRAM.ContinuteIceCount >= DIS_DVDDEC_ICE_CONT) && (READ_FIELD(HAL_DFE_ICEHFAMP) >= 0x06)
                         && (RWDisc.dvRAM.ReadDataMode == ePRE_BUFFERING))   //detect NonICE in pre-buffering
                    {
                        dvdStopDiscTransfer();
                    #if E_DIAGS
                        if (RWDisc.dvRAM.ReadDataMode == eREAD_ERROR_RATE)
                            ANEW_SEEK = TRUE;
                    #endif
                    }
                    PrePID = (RWDisc.dvRAM.CurrentPID & DVD_BLOCK_MASK);
                }
                #endif

                //if ((DVIRQS3 & HAL_DVDD_DRSEQIDI) && (Iram_WGateUp == TRUE))
                //{
                //     send_msg5SValue(1, "ERRORPID", 4, RWDisc.dvRAM.CurrentPID);
                //     RWDisc.dvRAM.HeadIdNoGood = TRUE;  //huang test
                //     WRITE_FIELD(HAL_DVDD_DRSEQIDI,0);    /* Clear  status    */
                //}

            }
            WRITE_FIELD(HAL_DVDD_PIDI,0);    /* Clear  status    */

            if((DVIRQS1 == 0) && (DVIRQS2 == 0) && (DVIRQS3 == HAL_DVDD_PIDI))
                return;
        }
    #endif
        DVDDecoderIntProc();
    }
#if BD_READ == 1
    else if (DiscLoaded == halGLOBAL_DiscStd_BD)
    {
        /*----------------------------------------------------------------
            BD-DVD DECODER INTERRUPT SERVICE
        -----------------------------------------------------------------*/
        BdDecoderIrqHandler();
    }
#endif

#if HD_READ == 1
    else if (DiscLoaded == halGLOBAL_DiscStd_HDDVD)
    {
        /*----------------------------------------------------------------
            HD-DVD DECODER INTERRUPT SERVICE
        -----------------------------------------------------------------*/
        HDDecoderIntProc();
    }
#endif
}

#if HD_READ == 1
/*********************************************************************
* Function:     HdDecoderIrqHandler()
*
* Description:  Function is executed when a Decoder Interrupt has has taken place
*                 whose origin is in the HD Decoder Module.
*
* Returns:      none
* Params:       none
*********************************************************************/
void HdDecoderIrqHandler(void)
{
}
#endif

/*********************************************************************
* Function:     DigitalFrontEndISR()
*
* Description:  Function is executed when a Digital Front End Interrupt has has taken place
*
* Returns:      none
* Params:       none
*********************************************************************/
void DigitalFrontEndISR(void)
{
    // The Jitter ISR will not work with 6230. Jitter Hardware is located in Digital Front End Module.
    //   However, there is no Jitter Interrupt available for chip 6230
    // There may be need for additional code for DFE ISR at this location
    HAL_Reg32 DfeIrqStatus;

    #if ((ENABLE_DFE_ICE_DETECT == 1)&&(BD_ENABLE == 1))
    BYTE HFamp;
    BYTE ICEStat,HFap;

    #endif
    DfeIrqStatus = HAL_DFE_GetInterruptStatus();

    HAL_DFE_ClearInterruptStatus();

    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    if (pdbGetDVDType() == DVD_RAM)
    {
        if(( DVD_BUFFERING() == ON) && ((DfeIrqStatus & HAL_DFE_CLSTATEI) == HAL_DFE_CLSTATEI))
        {
            DVDRAMIceDetection();
            return;
        }
    }
    #endif

    if ((DfeIrqStatus & HAL_DFE_KEYDETI) == HAL_DFE_KEYDETI)
    {
    #if ENABLE_AACS
        // Read 128 bits ROM_MARK code
        HAL_DFE_RomMarkGetKey( RomMarkKeyBuffer );
        fRomMarkInfoGet = TRUE;

    #endif  // ENABLE_AACS

        HAL_DFE_RomMarkDisableDetection();
    }

#if ((ENABLE_DFE_ICE_DETECT == 1)&&(BD_ENABLE == 1))
    if ((DfeIrqStatus & HAL_DFE_CLSTATEI) == HAL_DFE_CLSTATEI)
    {
        if (BD_BUFFERING() == ON)
        {
            if(!READ_FIELD(HAL_AFE_RFAGCEN))
                SendMsg80(SHOW_DEBUG_MSG,0x010205);//WARNING: RF AGC/AOC is controled by AFE !!!

            if(!READ_FIELD(HAL_DFE_AGCON))
                SendMsg80(SHOW_DEBUG_MSG,0x010206);//WARNING: RF AGC/AOC is disabled !!!

//            //Output Current RF AGC/AOC & HF Defect Status & Pll Lock Status//Debug use
//            SendMsgCnB(SHOW_DEBUG_MSG,3,0x01010C,B1B(Layer_Index),B1B(READ_FIELD(HAL_DFE_GDDATGMON)),B1B(READ_FIELD(HAL_DFE_ODDATGMON)));
//            SendMsgCnB(SHOW_DEBUG_MSG,3,0x01010D,B1B(Layer_Index),B1B(READ_FIELD(HAL_AFE_RFAGCREG)),
//                       B1B(calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1)));
//            SendMsgCnB(SHOW_DEBUG_MSG,4,0x01010E,B1B(READ_FIELD(HAL_DFE_DFTDETPA)),B1B(READ_FIELD(HAL_DFE_DFTDETIA)),B1B(READ_FIELD(HAL_DFE_LKAIDFLK)),B1B(READ_FIELD(HAL_DFE_LKAIDLK)));
        }

        HFamp = READ_FIELD(HAL_DFE_ICEHFAMP);
        HFap = (BYTE)READ_FIELD(HAL_DFE_AMPLVLAGC);
        ICEStat = (BYTE)READ_FIELD(HAL_DFE_CLUSST0);
    #if 0 //for debug
        IcedClusterStAddr = (ULONG)READ_FIELD(HAL_DFE_ICESTRAD);
        send_msg5SValue(1,"HFamp",4,HFamp);
        send_msg5SValue(1,"HFap",4,HFap);
        send_msg5SValue(1,"ICEStat",4,ICEStat);
        send_msg5SValue(1,"Iced",4,IcedClusterStAddr<<3);
    #endif
        if((HFamp < 0x16)||(HFap < 0x5A)||(ICEStat == 0x02))//HFAmp < 0x16 or HFAmp(AGC) < 0x5A or Full ICE
        {
             if ((svoVar_discKindT.Bit.Layers == eLayersDouble)&&(svoIramVar.currentID.SectorNum.all32 >= 0x1C00000))
             {
                if(READ_FIELD(HAL_DFE_BITDETCFG) != 2)
                    WRITE_FIELD(HAL_DFE_BITDETCFG,   2); //RLPB seems more stable in Iced area detect
             }
            //Before reading ICE cluster address, we should make sure that buffering is ON.
            if((READ_FIELD(HAL_BDC_HLDSDS) == 0) && (READ_FIELD(HAL_BDC_HLDFDS) == 0))
            {
    #if(ENABLE_DISCARD_ICE == 1)
                ICEcnt++;
                if(ICEcnt>=MAXICECNT)
                    ICEcnt = 0;

                IcedClusterStAddr = (ULONG)READ_FIELD(HAL_DFE_ICESTRAD);
                ICEPSN[ICEcnt] = IcedClusterStAddr<<3;
    #else
                IcedClusterStAddr = (ULONG)READ_FIELD(HAL_DFE_ICESTRAD);
    #endif
            }
//    #if ((PCB == 710)||(PCB ==711) || (PCB == 692)||(PCB == 817))
//            HAL_DFE_EnDisHfDefect(halDFE_HfDefectDisable);              /* Disable HF defect */
//    #endif
            Defect_Enable(DEFECT_STATE_ALL_OFF);
    #if (HF_BASED_CUTOFF == 1)
            WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //relinking cut off base on wobble
            WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //force demod in run mode in runin area
    #endif

            ICEAreaDecoder();
        }
        else
        {
    #if (ENABLE_DISCARD_ICE == 1)
            if (ICEStat == 0x01)
                HAL_BD_CODEC_Disable_DiscardIce();
    #endif
            IcedClusterStAddr = 0x00;
    #if (HF_BASED_CUTOFF == 1)
//            WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
//            WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
    #endif
        }
    }
    else if (READ_FIELD(HAL_DFE_BKDDEN) == 0)
    {
        HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);              /* Enable HF defect */
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);/* Enable black and white defect */
        if((READ_FIELD(HAL_DFE_BITDETCFG) != 3) && (Current_read_sp >= e6XDCAV))
        {
            WRITE_FIELD(HAL_DFE_BITDETCFG, 3);
        }
#if (HF_BASED_CUTOFF == 1)
    #if(WOB_BASED_DEMODULATOR == 1)
            if(wob_based_demod==1)
            {
                WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //relinking cut off base on demodulator
                WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //don't force in run mode in runin area
            }
            else
            {
                WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
                WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
            }
    #else
            WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
            WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
    #endif
#endif
    }
#endif  /* ENABLE_DFE_ICE_DETECT == 1 */

}

#if(ENABLE_HDDVD && ENABLE_AACS)
/*********************************************************************
* Function:     DemodISR()
*
* Description:
*
* Returns:      none
* Params:       none
*********************************************************************/
extern USHORT *MMPtr;
extern BYTE OffsetIndex;
extern ULONG MMD[64];

void DemodISR(void)
{
    HAL_Reg32 DemodIrqStatus;
    USHORT MediaMarkOffset;

    DemodIrqStatus = READ_REG(HAL_DMODREG_MAP->DMODINTS);

    if(DemodIrqStatus & HAL_DMOD_MMCAPS)
    {
        WRITE_FIELD(HAL_DMOD_MMCAPS,0);
        MMD[OffsetIndex] = READ_REG(HAL_DMODREG_MAP->MMDATA); /* Read Media Mark Data Word*/
        OffsetIndex++;
       MediaMarkOffset = *(MMPtr + OffsetIndex);
        WRITE_FIELD(HAL_DMOD_MMOFFSET,MediaMarkOffset); /*Set Media Mark Offset*/
    }
}
#endif //(ENABLE_HDDVD && ENABLE_AACS)

