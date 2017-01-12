/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_rdtoc.c
*
* DESCRIPTION
*   This file contains ATAPI command handler of read TOC.
*
*   $Revision: 59 $
*   $Date: 10/12/21 4:18p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\comMath.h"
#include ".\common\System.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\app_if.h"
#include ".\hif\sv_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\gp_rdtoc.h"
#include ".\hif\gp_rdcd.h"
#include ".\common\STRING.H"
#include ".\oem\Oem_spd_tbl.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_global.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_demod.h"
#include ".\al\HAL_demod.h"
#include ".\common\intctl.h"

void WriteSDRAM_S2B_1B(BYTE);
static void GetTOCFormat0(void);
static void GetTOCFormat1(void);
static void WriteTrackInfoToSDRAM(void);
static void WriteTOCToSDRAM(BYTE b);
static void UpdateTOCFormat2Address(BYTE b);
static void InitializeTOCVariables(void);
static void CheckCDType(void);
static void CheckCDIReady(BYTE);
static void TransferTOCToHost(void);
static void sp_toc_fm0(BYTE track);
static void sp_toc_fm1(void);
static void sp_toc_fm2(BYTE start_sess);
static void AddToSDRAMAddress(BYTE num);
static void CopyTOC0ToScratch(ULONG *dest_ptr,
                                        ULONG *src_ptr);
static void ConvertTOCMSFToLBA(ULONG *dest_ptr,
                                        ULONG *src_ptr);
static void ConvertTOCLBAToMSF(ULONG *dest_ptr,
                                        ULONG *src_ptr);    /* for DVD disc toc data LBA ==> MSF */
static void CopyTOC2ToScratch(ULONG *dest_ptr,
                                        ULONG *src_ptr);
static void PrepareHeaderData(ULONG *dest_ptr,
                                        ULONG *src_ptr);
vBOOL CheckSubcodeForCDText(void);                        /* CD-text */
void ReadCDText(void);
vBOOL StartCDTextBuffering(void);
void Buffer_RAM(USHORT, USHORT, USHORT, BYTE *);
void RAM_Buffer(USHORT, USHORT, BYTE *);
void SetHeader_4B(BYTE,BYTE);
void CheckIfDiskKindIsCDText(void);
void TransferNonCDTextDataToHost(void);
void ClearCDTextData(void);
BYTE  _crc_data_check(BYTE *data); /* read CD-TEXT subcode data error */
BYTE TextFlag;
/*--------------------------------------------------------------------
  Name: CDReadTOCCommand()
  Description:
    Processes ATAPI Read TOC command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
BYTE DiscIDSubCode[10];
BYTE old_rdtoc_fmt; /* cdi_plus_audio disc */

void CDReadTOCCommand(void)
{
    BYTE toc_format;
    BYTE start;

#if (WITHOUT_SERVO == 1)
BUILD_SENSE(0x05,0x24,0x00,0x68);                /* INVALID FIELD IN CDB */
AutoCommandComplete();
return;
#endif

    if(((Iram_Cmd_block.bf[2] << 4) != 0x30) && ((Iram_Cmd_block.bf[2] << 4) != 0x40)){
        if((St_blank_disc == TRUE) || (St_no_toc_info == TRUE)){
            BUILD_SENSE(0x05,0x24,0x00,0x69);              /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
    }

    /* check allocation length, return command completed if allocation length
       equal zero */
    if(((Iram_Cmd_block.bf[8] & 0xfe) | Iram_Cmd_block.bf[7]) == 0){    /* check alloc length is zero */
        AutoCommandComplete();
        return;
    }

    if (((Iram_Cmd_block.bf[2] & 0x0F) << 4) != 0x40){
    }

    /* allocation length is not zero, process toc command */
    /* stop decoder operation to avoid read operation occurring in the background */
    if (Rw_dec_stop != TRUE){
        StopDecoder();
        Svr_pause_reqire = TRUE;
    }

    /* check for LBA or MSF format, set st_MSF flag if MSF format */
    St_MSF = TRUE;      /* assume MSF format */
    if( !TEST_BIT(Iram_Cmd_block.bf[1], 1) ){               /* if MSF format */
        St_MSF = FALSE;
    }

    /* check for requested format (0, 1, or 2) */
    /* Format Field in CDB[9] is obsolete - but will still */
    /*     be supported - legacy                           */

    toc_format = ((Iram_Cmd_block.bf[2] & 0x0F) << 4);
    old_rdtoc_fmt = 0;  /* cdi_plus_audio disc */
    if (toc_format == 0x00){     /* Format Field could still be in old CDB[9] location */
        old_rdtoc_fmt = 1;  /* for cdi_plus_audio disc */
        if ((Iram_Cmd_block.bf[9] & 0xC0) == 0xC0){
            BUILD_SENSE(0x05,0x24,0x00,0x6a);              /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }else{
            toc_format = ((Iram_Cmd_block.bf[9] & 0xC0) >> 2);
        }
    }


    switch(toc_format){
        /*------------------------------------------------------------------------*/
        /*                    CDR/W FORMAT 0 : TOC INFO                           */
        /*------------------------------------------------------------------------*/
        case TOC_FORMAT_TOC:                                      /* TOC format */
                                                        /* Process TOC format 0 */
            start = Iram_Cmd_block.bf[6];                   /* get starting track */
            if((St_MSF == TRUE)&&(start == 0 || start == 1)&&(pdbDiscLoaded() == CD_DISC)){
                /* transfer TOC format 0 data and completion the command */
                Iram_Buf_dram_addr = RDTOC_TOC_ADDR;
                Iram_Linear_Xfer_addr = RDTOC_TOC_ADDR;
                TransferTOCToHost();                          /* send data to host */
                break;
            }
            /* jump here if the starting track is not equal 0 or 1; or format 0
               requested data return in LBA format
               Input:  a = starting track */
            sp_toc_fm0(start);
            break;

    /* NOTE 'a' can be modified if the above if is true but
        the internal one fails. */
        /*------------------------------------------------------------------------*/
        /*                   CDR/W FORMAT 1 : SESSION INFO                        */
        /*------------------------------------------------------------------------*/
        case TOC_FORMAT_SESSIONINFO:                              /* Process format 1 */
            /* for format 1, command BYTE 6 (starting track/session) is reserved.*/
            if(Iram_Cmd_block.bf[6] != 0){
                BUILD_SENSE(0x05,0x24,0x00,0x6b);          /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                break;
            }

            if((St_MSF == TRUE)&&(pdbDiscLoaded() == CD_DISC )){
                Iram_Buf_dram_addr = RDTOC_SESSION_ADDR;
                Iram_Linear_Xfer_addr = RDTOC_SESSION_ADDR;
                TransferTOCToHost();                          /* send data to host */
                break;
            }
            sp_toc_fm1();
            break;

        /*------------------------------------------------------------------------*/
        /*                  CDR/W FORMAT 2 : FULLL TOC INFO                       */
        /*------------------------------------------------------------------------*/
        case  TOC_FORMAT_FULLTOC:                                     /* Process TOC format 2 */
            St_MSF = TRUE;                              /* Only MSF format */
            start = Iram_Cmd_block.bf[6];                   /* get starting session */
            if((St_MSF == TRUE)&&(start == 0 || start == 1)){
                /* transfer TOC format 2 data and completion the command */
                if (St_cdi_audio && (old_rdtoc_fmt == 0))
                {
                    ULONG src, dest;
                    USHORT len;
                    BYTE a, i;

                    Iram_Buf_dram_addr = RDTOC_FULLTOC_ADDR;
                    a = ReadSDRAM();
                    len = (a<<8);
                    a = ReadSDRAM();
                    len += a;

                    len += 11;
                    Iram_Buf_dram_addr = BUFFER_SCRATCH;
                    WriteSDRAM(BHIGH(len));
                    WriteSDRAM(BLOW(len));
                    WriteSDRAM(0x01);
                    WriteSDRAM(0x01);

                    src = RDTOC_FULLTOC_ADDR + 4;
                    dest = BUFFER_SCRATCH + 4;
                    for(i=0; i<33; i++)
                        SCSDRAM[dest+i] = SCSDRAM[src+i];

                    Iram_Buf_dram_addr = BUFFER_SCRATCH + 37;
                    WriteSDRAM(0x01);
                    WriteSDRAM(0x14);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x01);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x02);
                    WriteSDRAM(0x00);

                    src = RDTOC_FULLTOC_ADDR + 37;
                    dest = BUFFER_SCRATCH + 48;
                    for(i=0; i<(len-46); i++)
                        SCSDRAM[dest+i] = SCSDRAM[src+i];

                    Iram_Buf_dram_addr = BUFFER_SCRATCH;
                    Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
                }
                else
                {
                /* cdi_plus_audio disc */
                    Iram_Buf_dram_addr = RDTOC_FULLTOC_ADDR;
                    Iram_Linear_Xfer_addr = RDTOC_FULLTOC_ADDR;
                }
                TransferTOCToHost();                          /* send data to host */
                break;
            }

        /* Jump here if the starting session is not equal 0 or 1; or format 2
           requested data return in LBA format
           Input:  a = starting session */
            sp_toc_fm2(start);
            break;

        /*------------------------------------------------------------------------*/
        /*                    CDR/W FORMAT 3 : PMA INFO                           */
        /*------------------------------------------------------------------------*/
        case TOC_FORMAT_PMA:
            BUILD_SENSE(0x05,0x20,0x00,0x20);          /* INVALID COMMAND */
            AutoCommandComplete();
            break;

        case  TOC_FORMAT_ATIP:
            BUILD_SENSE(0x05,0x20,0x00,0x20);          /* INVALID COMMAND */
            AutoCommandComplete();
            break;

        case  TOC_FORMAT_CDTEXT:                                     /* Process TOC format 5 */
            ReadCDText();
            break;
        default:                                        /* format illegal */
            BUILD_SENSE(0x05,0x24,0x00,0x6e);                /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            break;
    }
}

/*--------------------------------------------------------------------
  Name: ClearDiscAttributes()
  Description:
    Initializes Disc Attributes type flags
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ClearDiscAttributes(void)
{
    St_cdrom_audio  = FALSE;                    /* initialize cd rom flags */
    St_cdrom_data   = FALSE;
    St_cdrom_mixed  = FALSE;
    St_cdrom_hybrid = FALSE;
    St_cdrom_cdplus = FALSE;
    St_cdi          = FALSE;
    St_cdi_ready    = FALSE;
}


void ClearCDType(void)
{
    St_cdrom_audio  = FALSE;                    /* initialize cd rom flags */
    St_cdrom_data   = FALSE;
    St_cdrom_mixed  = FALSE;
    St_cdrom_hybrid = FALSE;
    St_cdrom_cdplus = FALSE;
    St_cdi          = FALSE;
    St_cdi_ready    = FALSE;
    St_cdi_audio    = FALSE;
}


/*--------------------------------------------------------------------
  Name: UpdateTOCData()
  Description:
    This routine is called at the power-on ,tray-in to or close session
    to prepare the TOC data for format 2 and to save into SDRAM format 2 area.
    Then based on format 2 TOC data, prepares for format 0 and 1.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void UpdateTOCData(void)
{
BYTE i;

    ClearCDType();
    S2b_buf_dram_addr = (RDTOC_FULLTOC_ADDR + 4);     /* save capacity data into SDRAM */
    BuildTOCFormat2();

    i = 0;
    for( i = 0; i <= 11; i++){           /* teminate the while loop in GetTOCFormat0() when blank disk */
        WriteSDRAM_S2B_1B(0xff);
    }

    /* prepare toc for format 0 and 1 based on format 2 data */
    GetTOCFormat0();                              /* Prepare toc format 0 */
    GetTOCFormat1();                              /* Prepare toc format 1 */
    InitializeTOCVariables();
    /* return cd type flags */
    CheckCDType();

    if(St_blank_disc == FALSE)
    {
        //send_msg5S(SEND_RdTocTask_MSG,"RdTOC OK!");
        send_msg80(SEND_RdTocTask_MSG,0x710053);
        St_toc_ready = TRUE;
    }
    else
    {
        St_toc_ready = FALSE;
    }

}


/*--------------------------------------------------------------------
  Name: GetTOCFormat0()
  Description:
    Prepares toc data for format 0 based on the format 2 toc data.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void GetTOCFormat0(void)
{
BYTE first_track, track, a, i;
USHORT data_len;

    Toc_addr = RDTOC_TOC_ADDR + 4;                    /* toc fm0 pointer (destination) */
    //start == FWtester2  Item13 No.3 CD-R Multi-Session compare error   
    Iram_Buf_dram_addr = Toc_addr;
    for (i = 0; i < 0x2C; i++) // compare 0x30 byte length. 
        WriteSDRAM(0x00);
    Iram_Buf_dram_addr = 0;
    //end    
    
    Toc_fm2_addr = RDTOC_FULLTOC_ADDR + 4;                /* toc fm2 pointer (source) */
    first_track = 1;                                /* initialize first track value */

    do{
        UpdateTOCFormat2Address( 3 );
        track = ReadSDRAM();
        switch(track){
            case 0xA0:
            /* If A0h index, save first track value in first_track variable for
               format 1 used latter */
                UpdateTOCFormat2Address( 8 );
                track = ReadSDRAM();              /* get first track number */
                /* cdi disc */
                if (!St_cdi)
                first_track = track;
                break;
            case 0xA2:
            /* if lead-out, save toc fm2 lead-out SDRAM address for format 0 use latter */
                Toc_lead_out = Toc_fm2_addr;
                break;
            default:
                if( ! CheckPlayerData( track ) ){   /* chk point Axh,Bxh,Cxh or Dxh */
            /* Point to track subcode
               Get subcode data from format 2 and write into SDRAM of format 0 */
                    if( track == first_track ){
                /* Save first track SDRAM address for fomat 1 used later */
                        Toc_1st_trk_addr = Toc_fm2_addr;
                    }
                                                /* Write track descriptor into SDRAM */
                WriteTrackInfoToSDRAM();
                }
                break;
        }
        /* Update to next subcode address of toc format 2 */

        UpdateTOCFormat2Address( 11 );              /* next subcode pointer */
        Toc_fm2_addr = Iram_Buf_dram_addr;
        a = ReadSDRAM();

        if (St_cdi)
        {
            if (track == 0xA2)
            {
                if (St_cdi_audio)
                    Toc_1st_trk_addr = Toc_fm2_addr;

                WriteTOCToSDRAM(0);                          /* write reserved BYTE */
                WriteTOCToSDRAM(0x14);                       /* write addr/ctrl */
                WriteTOCToSDRAM(1);             /* track number */
                WriteTOCToSDRAM(0);                          /* write reserved BYTE */
                WriteTOCToSDRAM(0);                          /* write absolute address */
                WriteTOCToSDRAM(0);             /* AMIN */
                WriteTOCToSDRAM(2);             /* ASEC */
                WriteTOCToSDRAM(0);             /* AFRAME */
            }
        }

    }while(a != 0xFF);                          /* check for end of toc format 2 */

    /* write subcode for the last track, get lead_out address, and
       write lead_out subcode into SDRAM to complete toc format 0 */
    Toc_fm2_addr = Toc_lead_out;
    WriteTrackInfoToSDRAM();

    /* write 0FFh at the end of toc format 0 */
    Iram_Buf_dram_addr = Toc_addr;                  /* get current address */
    WriteSDRAM(0xff);

    /* Return the first 4 BYTE of toc format 0 header */
    Iram_Buf_dram_addr = RDTOC_TOC_ADDR;

    /* Toc data length = toc_addr - RDTOC_TOC_ADDR */
    data_len = Toc_addr - RDTOC_TOC_ADDR - 2;

    /* Write toc format 0 header into SDRAM */
    WriteSDRAM( BHIGH(data_len) );
    WriteSDRAM( BLOW (data_len) );

    if(St_cdi == TRUE){
        WriteSDRAM ( 1 );
    }else{
        WriteSDRAM( 1 );
    }


    Iram_Buf_dram_addr = TOC_DRAM_HI + MAX_TRACK;
    track = ReadSDRAM();                          /* get last track */
    Iram_Buf_dram_addr = RDTOC_TOC_ADDR + 3;

    if((St_cdi == TRUE) && (St_cdi_audio == FALSE)){
        WriteSDRAM ( 1 );                          /* save last track */
    }
    else{
        WriteSDRAM ( track );                      /* save last track */
    }
}


/*--------------------------------------------------------------------
  Name: GetTOCFormat1()
  Description:
    Prepares toc data for format 1 based on the format 2 toc data.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void GetTOCFormat1(void)
{
BYTE sessions;

    Iram_Buf_dram_addr = RDTOC_SESSION_ADDR;
    WriteSDRAM ( 0 );                              /* TOC data length hi-BYTE */
    WriteSDRAM ( 0x0A );                           /* TOC data length lo-BYTE */
    WriteSDRAM ( 1 );                              /*  ;first session value */


    Iram_Buf_dram_addr = TOC_DRAM_HI + NUM_OF_SESS;
    sessions = ReadSDRAM();
    Iram_Buf_dram_addr = RDTOC_SESSION_ADDR+3;
    WriteSDRAM( sessions );
    Toc_addr = Iram_Buf_dram_addr;

    if((St_cdi == TRUE) && (St_cdi_audio == FALSE)){    /* for cdi plus audio disc */
        /* Cdi disc, return leadout subcode because no data track in cd-i */
        Toc_fm2_addr = Toc_lead_out;
    }else{
        /* Not cdi disc, return the absolute address of first track in last session */
        Toc_fm2_addr = Toc_1st_trk_addr;
    }

    /* Write format 1 subcode into SDRAM */
    WriteTrackInfoToSDRAM();

    /* write 0FFh at the end of toc format 1 */
    Iram_Buf_dram_addr = Toc_addr;                  /* get current address */
    WriteSDRAM( 0xff );
}


/*--------------------------------------------------------------------
  Name: WriteTrackInfoToSDRAM()
  Description:
    This routine is used for format 0 and format 1 to write 8 bytes
    toc data into SDRAM
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
  Globals:
    Toc_fm2_addr - (in) pointer to the format 2 subcode
    Toc_addr     - (out) pointer to format 0 or format 1 SDRAM area
--------------------------------------------------------------------*/
static void WriteTrackInfoToSDRAM(void)
{
BYTE fm2;

    WriteTOCToSDRAM(0);                          /* write reserved BYTE */
    UpdateTOCFormat2Address(1);                     /* adjust source pointer */
    fm2 = ReadSDRAM();                            /* get addr/ctrl from format 2 */
    WriteTOCToSDRAM(fm2);                        /* write addr/ctrl */
    UpdateTOCFormat2Address(3);                     /* adjust source pointer */
    fm2 = ReadSDRAM();                            /* get track data from format 2 */
    if( fm2 == 0xA2 ){
        fm2 = 0xAA;                             /* in format 0, lead-out return as AAh not A2h */
    }
    WriteTOCToSDRAM(fm2);
    WriteTOCToSDRAM(0);                          /* write reserved BYTE */
    WriteTOCToSDRAM(0);                          /* write absolute address */
    UpdateTOCFormat2Address(8);                     /* adjust source pointer */
    fm2 = ReadSDRAM();                            /* get absolute minute */
    WriteTOCToSDRAM(fm2);
    UpdateTOCFormat2Address(9);
    fm2 = ReadSDRAM();                            /* get absolute second */
    WriteTOCToSDRAM(fm2);
    UpdateTOCFormat2Address(10);
    fm2 = ReadSDRAM();                            /* get absolute frame */
    WriteTOCToSDRAM(fm2);
}


/*--------------------------------------------------------------------
  Name: WriteTOCToSDRAM()
  Description:
    Writes data in b to SDRAM area and update SDRAM address
  Inputs:
    b - TOC data
  Outputs:
    none
  Returns:
    none
  Globals:
    Toc_addr - address in SDRAM
--------------------------------------------------------------------*/
static void WriteTOCToSDRAM(BYTE b)
{
    Iram_Buf_dram_addr = Toc_addr;
    WriteSDRAM( b );
    Toc_addr++;
}


/*--------------------------------------------------------------------
  Name: UpdateTOCFormat2Address()
  Description:
    Updates toc format 2 address, and save into buf_dram_addr
  Inputs:
    b - number of bytes need to update
  Outputs:
    none
  Returns:
    none
  Globals:
    Iram_Buf_dram_addr - new location in format 2 SDRAM area
--------------------------------------------------------------------*/
static void UpdateTOCFormat2Address(BYTE b)
{
    Iram_Buf_dram_addr = Toc_fm2_addr + b;
}


/*--------------------------------------------------------------------
  Name: TransferTOCToHost()
  Description:
    Send TOC data from SDRAM to host
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void TransferTOCToHost(void)
{
    //start == FWtester2  Item13 No.3 CD-R Multi-Session compare error   
    BYTE i;  
    ULONG dest, src;
    //end   
    USHORT data_len,alloc_len;

    BHIGH(data_len) = ReadSDRAM();
    BLOW (data_len) = ReadSDRAM();

    if(data_len == 0x0000){
        BUILD_SENSE(0x05,0x24,0x00,0x71);          /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    if(data_len & 0x0001){
        data_len++;
    }
    data_len += 2;
    BLOW (alloc_len) = Iram_Cmd_block.bf[8];
    BHIGH(alloc_len) = Iram_Cmd_block.bf[7];

    /* transfer paramater data to host and completion the command */
#if (LED_NUM == 1)
    ReadyLED(LED_ON);
#endif /* LED_NUM */

#if (LED_NUM == 2)
    ReadyLED(LED_OFF);
#endif /* LED_NUM == 2 */

    //start == FWtester2  Item13 No.3 CD-R Multi-Session compare error
    if(alloc_len == 0xFFFE) 
    {
        data_len = 0x30;
        if(Iram_Linear_Xfer_addr == RDTOC_TOC_ADDR)
        {
            dest = BUFFER_SCRATCH;
            src = RDTOC_TOC_ADDR;
            CopySDRAMData((ULONG *)&dest,(ULONG *)&src, data_len);
            Iram_Buf_dram_addr = BUFFER_SCRATCH;
            for(i=0; i<data_len; i++)
            {
                if(ReadSDRAM() == 0xFF)
                {
                    Iram_Buf_dram_addr -= 1;
                    WriteSDRAM(0);
                    Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
                    break;
                }
            }
            Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
        }
    }
    //end 

    SendParameterDataToHost(Iram_Linear_Xfer_addr, MINIMUM( data_len, alloc_len), TRUE);   // Send data to host. Send Command Complete

#if (LED_NUM == 1)
    ReadyLED(LED_OFF);
#endif /* LED_NUM */

#if (LED_NUM == 2)
    ReadyLED(LED_ON);
#endif /* LED_NUM == 2 */
}


/*--------------------------------------------------------------------
  Name: TransferCDTextDataToHost()
  Description:
    Returns CDText data to host
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void TransferCDTextDataToHost(void)
{
    USHORT TotalByte, alloc_len;

    Iram_Buf_dram_addr = CDTEXT_AREA;
    BHIGH(TotalByte) = ReadSDRAM();
    BLOW(TotalByte) = ReadSDRAM();

    TotalByte += 2;

    BLOW (alloc_len) = Iram_Cmd_block.bf[8];
    BHIGH(alloc_len) = Iram_Cmd_block.bf[7];

    if(Iram_Xfr_allow > 0x2000){
        Iram_Xfr_allow = 0x2000;
    }
    SendParameterDataToHost(CDTEXT_AREA, MINIMUM( TotalByte, alloc_len), TRUE);   // Send data to host. Send Command Complete.
}

/*--------------------------------------------------------------------
  Name: TransferNonCDTextDataToHost()
  Description:
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void TransferNonCDTextDataToHost(void)
{
    Iram_Buf_dram_addr = CDTEXT_AREA;
    WriteSDRAM(0x00);                              /* BYTE 0-3 */
    WriteSDRAM(0x02);
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    SendParameterDataToHost(CDTEXT_AREA, 4 , TRUE);   // Send data to host.Send Command Complete.
}

/*--------------------------------------------------------------------
  Name: ClearCDTextData()
  Description:
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ClearCDTextData(void)
{
    USHORT i;

#if(BACKGROUND_READ_AHEAD == 1)
    START_BACKGROUND_BUFFER = FALSE;
#endif

    Iram_Buf_dram_addr = CDTEXT_AREA;
    for (i = 0; i < CDTEXT_SIZE; i++)
        WriteSDRAM(0x00);

    for(i = 0; i < CDTEXT_SIZE; i++)
        SCSDRAM[CDTEXT_AREA + i] = 0x00;
}

/*--------------------------------------------------------------------
  Name: CheckCDType()
  Description:
    Checks CD type
    <MULTI SESSION DISC>
    >St_cdrom_hybrid
        - St_cdrom_cdplus   / Audio & Data
        - St_cdrom_audio    / Only Audio
        - St_cdrom_data     / Only Data

    <SINGLE SESSION DISC>
    >St_cdrom_mixed         / Audio & Data
    >St_cdrom_data          / Only Data
    >St_cdrom_audio         / Only Audio
    >St_cdi
        - St_cdrom_data     / Only Data
    >St_cdi_ready           / cdi_read
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void CheckCDType(void)
{
BYTE session_count;

    session_count = pdbGetNumberOfSessionsOnDisc();
    if (session_count != 1){
        St_cdrom_hybrid = 1;
        if ( St_cdrom_audio && St_cdrom_data){
            St_cdrom_audio = 0;
            St_cdrom_data = 0;
            St_cdrom_cdplus = 1;
            return;
        }
        if ( St_cdrom_audio && !St_cdrom_data){
            St_cdrom_audio = 1;
            St_cdrom_data = 0;
            return;
        }
        if ( !St_cdrom_audio && St_cdrom_data){
            St_cdrom_audio = 0;
            St_cdrom_data = 1;
            return;
        }
    }else{
        if ( St_cdrom_data & St_cdrom_audio){
            St_cdrom_mixed  = 1;
            St_cdrom_audio  = 0;
            St_cdrom_data   = 0;
            return;
        }
        if ( !St_cdrom_data & St_cdrom_audio){
            CheckCDIReady(session_count);
            return;
        }
    }
}


/*--------------------------------------------------------------------
  Name: InitializeTOCVariables()
  Description:
    Initialize all the temporary toc variables
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void InitializeTOCVariables(void)
{
    Toc_addr = 0;                               /* pointer used for format 0 & 1 */
    Toc_fm2_addr = 0;                           /* pointer used for format 2 */
    Toc_lead_out = 0;                           /* pointer used to save lead-out */
    Toc_1st_trk_addr = 0;                       /* pointer used to save first track */
}


/*--------------------------------------------------------------------
  Name: sp_toc_fm0()
  Description:
    This routine is called to prepare TOC format 0 data if starting
    track is not the beginning track on the disc, then copied the
    requested TOC data into scratch area and transfered data to host.
    This routine is also called if the LBA format requested.
  Inputs:
    track - starting track
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void sp_toc_fm0(BYTE track)
{
BYTE num_of_tracks = 0;
ULONG dest, src;

    if(track == 0){
        track = 1;
    }
    if((St_cdi == TRUE) && (St_cdi_audio == FALSE)){    /* for cdi plus audio disc */
        /* if cdi, only lead-out track return */
        track = 0x01;                           /* initial to lead-out track */
        num_of_tracks = 1;
    }else{
        if(track != 0xAA){                      /* check for lead-out track */
            Iram_Buf_dram_addr = RDTOC_TOC_ADDR + 3;
            num_of_tracks = ReadSDRAM();          /* get last track */
            num_of_tracks++;                    /* include lead-out */
            if(track > num_of_tracks){
                                                /* Track not found */
                BUILD_SENSE(0x05,0x24,0x00,0x72);    /* INVALID FIELD IN CDB */
                AutoCommandComplete();
                return;
            }
        }
    }

    Iram_Buf_dram_addr = RDTOC_TOC_ADDR + 6;

    while(TRUE){
        if(track == ReadSDRAM()){                 /* get track number */
            break;
        }
        AddToSDRAMAddress(7);                       /* update to next track  */
        if(--num_of_tracks == 0){
                                                /* Track not found */
            BUILD_SENSE(0x05,0x24,0x00,0x73);        /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
        }
    }

    /* copy data to scratch */
    Iram_Buf_dram_addr -= 3;
    src  = Iram_Buf_dram_addr;
    dest = BUFFER_SCRATCH + 4;
    CopyTOC0ToScratch((ULONG *)&dest,
                         (ULONG *)&src);        /* copy data from source to destination */

    /* transfer toc data from scratch area to host */
    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
    TransferTOCToHost();
}


/*--------------------------------------------------------------------
  Name: sp_toc_fm1()
  Description:
    This routine is called to prepare TOC format 1 data for LBA format
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void sp_toc_fm1(void)
{
ULONG dest, src;

    dest = BUFFER_SCRATCH;
    src = RDTOC_SESSION_ADDR;

    /*===========================================*/
    //      DVD Disc use LBA in DRAM
    //      CD   Disc use MSF in DRAM
    //      MSF mode : DVD Disc should be LBA ==> MSF
    //                        CD   Disc is already MSF
    //      LBA mode : DVD Disc is already LBA
    //                        CD   Disc should be MSF ==> LBA
    /*===========================================*/
    if (pdbDiscLoaded() == CD_DISC)
    {
        CopySDRAMData((ULONG *)&dest, (ULONG *)&src, 0x09);
        ConvertTOCMSFToLBA((ULONG *)&dest, (ULONG *)&src);
    }
    else if (pdbDiscLoaded() == DVD_DISC)
    {
        if(St_MSF == TRUE)
        {
            CopySDRAMData((ULONG *)&dest, (ULONG *)&src, 0x08);
            ConvertTOCLBAToMSF((ULONG *)&dest, (ULONG *)&src);
        }
        else
            CopySDRAMData((ULONG *)&dest, (ULONG *)&src, 0x0C);
    }

    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
    TransferTOCToHost();
}


/*--------------------------------------------------------------------
  Name: sp_toc_fm2()
  Description:
    This routine is called to prepare TOC format 2 data if starting
    session is not the beginning session on the disc, and then copied
    the requested TOC data into scratch area and transfered data to host.
    This routine is also called if the LBA format requested.
  Inputs:
    session - starting session
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void sp_toc_fm2(BYTE start_sess)
{
BYTE last_sess;
ULONG dest,src;

    if(start_sess == 0){
        start_sess = 1;
    }

    Iram_Buf_dram_addr = RDTOC_FULLTOC_ADDR + 3;
    last_sess = ReadSDRAM();

    if(start_sess > last_sess){
        BUILD_SENSE(0x05,0x24,0x00,0x74);            /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    Iram_Buf_dram_addr = RDTOC_FULLTOC_ADDR + 4;

    if(start_sess != 1){
        while(TRUE){
            last_sess = ReadSDRAM();              /* get session */
            if(start_sess == last_sess){
                break;
            }
            AddToSDRAMAddress(10);
        }
    }

    src = Iram_Buf_dram_addr - 1;
    dest = BUFFER_SCRATCH + 4;
    CopyTOC2ToScratch((ULONG *)&dest,
                         (ULONG *)&src);

    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    Iram_Linear_Xfer_addr = BUFFER_SCRATCH;
    TransferTOCToHost();
}


/*--------------------------------------------------------------------
  Name: CopyTOC0ToScratch()
  Description:
    Copy toc format 0 data from the toc format 0 SDRAM to the scratch
    area and change to lba if st_MSF flag set.
  Inputs:
    dest - destination
    src - source
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void CopyTOC0ToScratch(ULONG *dest_ptr,
                                ULONG *src_ptr)
{
BYTE a;

    while(TRUE){
        Iram_Buf_dram_addr = *src_ptr;

        a = ReadSDRAM();
        if(a == 0xFF){
            break;
        }

        /*===========================================*/
        //      DVD Disc use LBA in DRAM
        //      CD   Disc use MSF in DRAM
        //      MSF mode : DVD Disc should LBA ==> MSF
        //                        CD   Disc is already MSF
        //      LBA mode : DVD Disc is already LBA
        //                        CD   Disc should MSF ==> LBA
        /*===========================================*/
        if(St_MSF == TRUE)
        {
            if (pdbDiscLoaded() == CD_DISC)
                CopySDRAMData(dest_ptr, src_ptr, 8);
            else if (pdbDiscLoaded() == DVD_DISC)
            {
                CopySDRAMData(dest_ptr, src_ptr, 4);
                ConvertTOCLBAToMSF(dest_ptr, src_ptr);
            }
        }
        else
        {
            if (pdbDiscLoaded() == CD_DISC)
            {
                CopySDRAMData(dest_ptr, src_ptr, 5);
                ConvertTOCMSFToLBA(dest_ptr, src_ptr);
            }
            else if (pdbDiscLoaded() == DVD_DISC)
                CopySDRAMData(dest_ptr, src_ptr, 8);
        }
    }

                                                /* Prepare 4 bytes header */
    *src_ptr = RDTOC_TOC_ADDR + 2;
    PrepareHeaderData(dest_ptr, src_ptr);
}


/*--------------------------------------------------------------------
  Name: CopyTOC2ToScratch()
  Description:
    Copies toc format 2 data from the toc format 2 dram to the scratch
    area and changes to lba if st_MSF flag set.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void CopyTOC2ToScratch(ULONG *dest_ptr, ULONG *src_ptr)
{
BYTE a, point;

    while(TRUE){
        Iram_Buf_dram_addr = *src_ptr;
        a = ReadSDRAM();                                  /* get 1st BYTE */
        if(a == 0xFF){                                  /* check for end of toc area */
            break;
        }
        if(St_MSF == TRUE){
            CopySDRAMData(dest_ptr, src_ptr, 11);
        }else{
            /* Support LBA in format 2 */
            /* copy the first 3 bytes: session number, ADR/control, and TNO */
            CopySDRAMData(dest_ptr, src_ptr, 3);
            /* Get Index and save into variable.
               If Index is A0h or A1h, donot change Pmin, Psec, Pframe fields to LBA format.
               If Index is C0h, donot change min, sec, frame fields to LBA format. */
            Iram_Buf_dram_addr = *src_ptr;
            point = ReadSDRAM();                          /* get index */
            if(point != 0xB0){
                CopySDRAMData(dest_ptr, src_ptr, 4);   /* Index and relative address */
            }else{
                CopySDRAMData(dest_ptr, src_ptr, 1);   /* copy index to scratch buffer */
                ConvertTOCMSFToLBA(dest_ptr, src_ptr);      /* change relative address to   */
                                                        /* lba & copy to scratch buffer */
            }

            if(point == 0xa0 || point == 0xa1){
                CopySDRAMData(dest_ptr, src_ptr, 4);
            }else{
                CopySDRAMData(dest_ptr, src_ptr, 1);   /* copy index to scratch buffer */
                ConvertTOCMSFToLBA(dest_ptr, src_ptr);
            }
        }
    }
                                                        /* Prepare 4 bytes header */
    *src_ptr = RDTOC_FULLTOC_ADDR + 2;
    PrepareHeaderData(dest_ptr,src_ptr);
}


/*--------------------------------------------------------------------
  Name: PrepareHeaderData()
  Description:
    Prepares the 4-BYTE header: toc length, first & last track for format 0
    or first & last session for format 2
  Inputs:
    src - point to format 0 or 2 at first and last track or session
    dest - point to scratch buffer area
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void PrepareHeaderData(ULONG *dest_ptr,ULONG *src_ptr)
{
    *dest_ptr -= ( BUFFER_SCRATCH + 2);
    Iram_Buf_dram_addr = BUFFER_SCRATCH;
    WriteSDRAM(GET_BHIGH(*dest_ptr));
    WriteSDRAM(GET_BLOW (*dest_ptr));
    *dest_ptr = BUFFER_SCRATCH + 2;
    CopySDRAMData(dest_ptr, src_ptr, 2);
}


/*--------------------------------------------------------------------
  Name: ConvertTOCMSFToLBA()
  Description:
    Gets MSF from toc area, change to LBA format and saves into
    scratch buffer.
  Inputs:
    src - point to toc area
    dest - point to scratch buffer area
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void ConvertTOCMSFToLBA(ULONG*dest_ptr, ULONG *src_ptr)
{
ADDRESS_T addr;

    Iram_Buf_dram_addr = *src_ptr;
    addr.msf.Minute = ReadSDRAM();
    addr.msf.Second = ReadSDRAM();
    addr.msf.Frame = ReadSDRAM();
    *src_ptr = Iram_Buf_dram_addr;

    if((addr.msf.Minute == 0)&&(addr.msf.Second == 0)&&(addr.msf.Frame == 0)){
        addr.lba = 0;
    }else{
        addr.lba = MSF2LBA(&addr.msf);
    }

    Iram_Buf_dram_addr = *dest_ptr;
    WriteSDRAM((BYTE) (addr.lba >> 16) & 0xFF );
    WriteSDRAM((BYTE) (addr.lba >> 8 ) & 0xFF );
    WriteSDRAM((BYTE) addr.lba & 0xFF );

    *dest_ptr = Iram_Buf_dram_addr;
}


/*--------------------------------------------------------------------
  Name: AddToSDRAMAddress()
  Description:
    Update the SDRAM buffer address by number of bytes in register b
  Inputs:
    num - number of bytes need to update
  Outputs:
    none
  Returns:
    none
  Globals:  (in/out) Iram_Buf_dram_addr - point to SDRAM current address
--------------------------------------------------------------------*/
static void AddToSDRAMAddress(BYTE num)
{
    Iram_Buf_dram_addr += num;
}


/*--------------------------------------------------------------------
  Name: SaveTOCToSDRAM()
  Description:
    Saves TOC data into SDRAM
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
public void SaveTOCToSDRAM(void)
{
BYTE i;

    WriteSDRAM_S2B_1B(_Nr_of_sessions);

    if(( Iram_Player_output[0] & 0xF0) == 0x50 ){   /* chk if addr = 50h */
        for(i = 0;i < 10;i++){
            WriteSDRAM_S2B_1B(Iram_Player_output[i]);
        }
    }else{
        WriteSDRAM_S2B_1B(Iram_Player_output[0]);
        WriteSDRAM_S2B_1B(0x00);
        WriteSDRAM_S2B_1B(Iram_Player_output[2]);
        /* fake for rel min/sec/fram and zero BYTE as zero */

        WriteSDRAM_S2B_1B(Iram_Player_output[3]);
        WriteSDRAM_S2B_1B(Iram_Player_output[4]);
        WriteSDRAM_S2B_1B(Iram_Player_output[5]);

        WriteSDRAM_S2B_1B(0x00);
        WriteSDRAM_S2B_1B(Iram_Player_output[7]);
        WriteSDRAM_S2B_1B(Iram_Player_output[8]);
        WriteSDRAM_S2B_1B(Iram_Player_output[9]);
    }
}


/*--------------------------------------------------------------------
  Name: GetTOCFormat2Length()
  Description:
    Prepares toc data length for format 2
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void GetTOCFormat2Length(void)
{
    USHORT length;

    Iram_Buf_dram_addr  = RDTOC_FULLTOC_ADDR ;

    length = (USHORT)(S2b_buf_dram_addr - (RDTOC_FULLTOC_ADDR + 2));

    if ((S2b_buf_dram_addr - (RDTOC_FULLTOC_ADDR + 2)) & 0x0001){
        length++;
    }

    WriteSDRAM((length & 0xFF00) >> 8);
    WriteSDRAM(length & 0x00FF);
}


/*--------------------------------------------------------------------
  Name: WriteSDRAM_S2B_1B()
  Description:
    This routine is used to write data into SDRAM for S2B interface only.
  Inputs:
    b - data
  Outputs:
    none
  Returns:
    none
  Globals:     S2b_buf_dram_addr: address in SDRAM
--------------------------------------------------------------------*/
void WriteSDRAM_S2B_1B(BYTE b)
{
vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    Iram_Buf_dram_addr = S2b_buf_dram_addr;
    WriteSDRAM(b);

    SET_INT_ENABLE_MASK(temp_IE);

    if((S2b_buf_dram_addr != END_DRAM_BUFFER )&&        /* last addr of SDRAM buffer  */
       (S2b_buf_dram_addr != END_SCRATCH_AREA)){        /* last addr of scratch area  */
        S2b_buf_dram_addr++;
    }
}


/*--------------------------------------------------------------------
  Name: CheckCDIReady()
  Description:
    Checks for a CDi Ready disc, data before the start of track one.
    Returns st_cdrom_mixed flag set if it is CDi Ready disc.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void CheckCDIReady(BYTE session_count)
{
BYTE a;

    if( session_count != 1 ){
        return;
    }
    /* get the first track address by reading from the SDRAM toc format 1 area,
       return st_cdrom_cdi_ready flag set if the first track address is greater
       than  0:3:0 */

    Iram_Buf_dram_addr = RDTOC_SESSION_ADDR+9;
    a = ReadSDRAM();                              /* get 1st track minute */
    if(a == 0){
        a = ReadSDRAM();                          /* get 1st track second */
        if(a < 3){
            return;
        }
        if(a == 3){
            a = ReadSDRAM();                      /* get 1st track frame */
            if(a == 0){
                return;
            }
        }
    }
    St_cdrom_audio = FALSE;
    St_cdi_ready   = TRUE;
}


/*--------------------------------------------------------------------
  Name: CheckPlayerData()
  Description:
    This routine check the subcode data receive from player.
  Inputs:
    data from player (BCD format)
  Outputs:
    none
  Returns:
    1 - if data is Axh,Bxh,Cxh, Dxh (x is 0..F)
    0 - otherwise
--------------------------------------------------------------------*/
vBOOL CheckPlayerData(BYTE v1)
{
    switch(v1 & 0xF0){                          /* check hi-nible */
        case 0xA0:
        case 0xB0:
        case 0xC0:
        case 0xD0:
        case 0xE0:
        case 0xF0:
            return(1);
        default:
            return(0);
    }
}


/*--------------------------------------------------------------------
  Name: ReadCDText()
  Description:
    This routine reads cd text data
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ReadCDText(void)
{
    SubQCodeS   SubCode;
    BYTE        rw_seek_retry;

    if (St_toc_ready == FALSE)
    {
        St_CDText_ready = TRUE;
        TransferNonCDTextDataToHost();
        return;
    }

    if (St_CDText_ready == TRUE)
    {
        if (TextFlag)
            TransferCDTextDataToHost();                             /* send data to host (2a7)*/
        else
            TransferNonCDTextDataToHost();

        return;
    }

    if(!St_cdrom_audio)
    {
        St_CDText_ready = TRUE;
        TransferNonCDTextDataToHost();
        return;
    }

#if (EN_POWER_CONTROL)
    SetDiscReadPowerMode();
#endif

#if(BACKGROUND_READ_AHEAD == 1)
    START_BACKGROUND_BUFFER = FALSE;
#endif

    svoIramVar.subQEnableFlag = 1;

    rw_seek_retry = 0;
    TextFlag = 0;

    WRITE_REG(HAL_CDDREG_MAP->BUFCTL,0);
    ClearCDTextData();

    while(1)
    {
        //if(ServoSeekToLeadIn() != PASS)
        if(ServoSeek(-6750+rw_seek_retry*7*75, eDataSeek)!= PASS) //seek to 98:32:00~ each retry + 7 sec
        {
            rw_seek_retry++;
        }
        else
        {
            pdbSetSubQIntStatus(FALSE);

            /* read CD-TEXT subcode data error */
            Iram_Knl_timer_counter = 50;           // Wait about 1 s
            Knl_timeout = FALSE;                    // Assume no time out

// TBD? PORT?    SCOR Logic was removed from CDDECODER see #4388 Some parts may have moved to Demodulator module.
            while(1)    // checking for bad SCOR
            {
                if (HAL_DEMOD_GetFePllLocked())   // If both inner and outer frequency lock
                    break;                            //   then the Sync is good.

                // Sync was not good, now reset logic. TBD    see also issue 4586
                // TBD   Reset Demodulator. Do not use Demodulator Soft Reset. This would reset all Demodulator register to initial values.
                pdbSetSubQIntStatus(FALSE);

                while((pdbGetSubQIntStatus() != TRUE) && (Knl_timeout == FALSE))
                {
                    ExitProcess();
                }
                if(Knl_timeout == TRUE)             // if time out
                {
                    Knl_timeout = FALSE;
                     break;
                }

            }

            if(rw_seek_retry < 3)
                WRITE_FIELD(HAL_CDD_SRWERREN,1);
            else
                WRITE_FIELD(HAL_CDD_SRWERREN,0);

            /* read CD-TEXT subcode data error */
            if (!StartCDTextBuffering())
            {
                rw_seek_retry++;
            }
            else
            {
                Iram_Knl_timer_counter = 150;           /* Wait about 3 s */
                Knl_timeout = FALSE;                    /* Assume no time out */
                while(DataBuffer.SectorCount < 255)
                {
                    if( (Rw_dec_stop == TRUE) )         /* Check lead-in track */
                        break;
                    if((pdbGetSubQIntStatus() == TRUE) && (pdbGetSubcodeOK() == TRUE))
                    {
                        pdbSetSubQIntStatus(FALSE);
                        pdbGetSubCode(&SubCode);
                        if((SubCode.Tno != (BYTE)0x00) && (SubCode.Addr == (BYTE)0x01))     /* Check lead-in track */
                            break;
                    }
                    if(Knl_timeout == TRUE)             /* if time out */
                    {
                        Knl_timeout = FALSE;
                        break;
                    }
                    ExitProcess();
                }
                if(DataBuffer.SectorCount < 5)
                {
                    rw_seek_retry++;
                }
                else
                {
                    StopDecoder();
                    ClearDataBuffer();
                    CheckIfDiskKindIsCDText();
                    if (TextFlag)
                    {
                        if(CheckSubcodeForCDText())
                        {
                            Iram_Buf_dram_addr = CDTEXT_AREA+1;
                            if(!(ReadSDRAM()&0x80))
                            {
                                rw_seek_retry++;
                                TextFlag = 0;
                            }
                            else
                            {
                                TransferCDTextDataToHost();             /* send data to host (2a7)*/
                                break;
                            }
                        }else
                        {
                            rw_seek_retry++;
                            TextFlag = 0;

                        }
                    }
                    else
                    {
                        TransferNonCDTextDataToHost();              /* can not read media */
                        break;
                    }
                }
            }
        }
        if (rw_seek_retry == 10)
            rw_seek_retry = 0;

        if (!svoIramVar.knlTimeupCount)
        {
            TransferNonCDTextDataToHost();                          /* can not read media */
            break;
        }
    }
    WRITE_FIELD(HAL_CDD_BUFCDT, 0);     /* no read CD-G subcode data */
    svoIramVar.subQEnableFlag = 0;
    St_CDText_ready = TRUE;
    StopDecoder();
    ClearDataBuffer();
    ServoSeek(0L, eDataSeek);
    Svr_pause_reqire = TRUE;                            /* mark pause request */
    SpeedCount = 0;
}


/*--------------------------------------------------------------------
  Name: StartCDTextBuffering()
  Description:
    This routine buffers CD text data
  Inputs:
    none
  Outputs:
    none
  Returns:
    TRUE
--------------------------------------------------------------------*/
vBOOL StartCDTextBuffering(void)
{
    StopDecoder();
    ClearDataBuffer();

    DISABLE_INTERRUPT();
    EnableCDDABuffering();
    St_method_2 = FALSE;
    Rw_subc_data = TRUE;                        /* mark read subcode data */

    Iram_Knl_timer_counter = 200;               /* Wait about 4 s */
    Knl_timeout = FALSE;                        /* Assume no time out */
    ENABLE_INTERRUPT();
    return (TRUE);
}


/*--------------------------------------------------------------------
  Name: CheckSubcodeForCDText()
  Description:
  Inputs:
    none
  Outputs:
    none
  Returns:
    TRUE
    FALSE
--------------------------------------------------------------------*/
vBOOL CheckSubcodeForCDText(void)
{
    BYTE    i;
    USHORT  j;
    BYTE    LoopFlag;
    USHORT  PageCnt;
    USHORT  DataLength;
    ULONG   DataLengthA;
    BYTE    DataH,DataL;
    BYTE    WorkArea[24];
    BYTE    RESERVE_ERROR_PACK[2][18];
    BYTE    k,k1,k2,k3;
    BYTE    page_counter;
    BYTE    pre_page;
    BYTE    correct_pack_flag;
    BYTE    CORRECT_PACK[18];
    BYTE    error_occur_flag ;
    BYTE    again_read_flag;
    BYTE    error_counter;

    k=0;
    k1=0;
    k2=0;
    k3=1;
    correct_pack_flag = 0;
    page_counter = 0;
    error_occur_flag = 0; //subcode data error
    again_read_flag=0; //again read 1 page
    error_counter=0;

    LoopFlag = 0;
    j = 0;
    DataLengthA = 0;

    for (PageCnt = 0; PageCnt < 0xFF; PageCnt ++)       /* 0xC0 -> 0xFF = 255 Page 00.07.04 */
    {
        for(i = 0; i < 4; i ++)
        {
            Buffer_RAM(PageCnt,i,18,(BYTE *)WorkArea);
            //if(((WorkArea[0] & 0xf0) == 0x80) && ((WorkArea[1] & 0x7f) <= 0x63))   /*read CD-TEXT subcode data error*/
            if(((WorkArea[0] & 0xf0) == 0x80))         /* read CD-TEXT subcode data error*/
            {   // CD TEXT information(ID1=8x & ID2<=99)
                if(((WorkArea[3] & 0x70) == 0x00) && (WorkArea[2] == 0x00))
                {   // Block Number = 0 & Sequence Number Indicator = 0 */
                    if(LoopFlag == 0)
                    {                   /* first */
                        LoopFlag = 1;
                    }
                    else
                    {                               /* repeat begin */
                        if(error_occur_flag==1){//subcode data error occur
                            again_read_flag=1; //read 1 page again
                if(error_counter==1){ //only 1 pack error ,renew read page
                error_counter=0;
                error_occur_flag=0;
                again_read_flag=0;
                correct_pack_flag=0;
                page_counter = 0;
                break;
                }
                    }else{
                        SetHeader_4B(0,DataH);
                        SetHeader_4B(1,DataL);
                        SetHeader_4B(2,0x00);
                        SetHeader_4B(3,0x00);
                        return (TRUE);
                        }
                    }
                }
/*--------------------------------------------------------------------------------------------------------------
            ______________________________________________________________________
Page 2A7    | DataH | DataL |  00  |  00  |
            |_______|_______|______|______|_______________________________________
*/
                if (LoopFlag == 1)
                {
                    /*read CD-TEXT subcode data error*/
                    //if((WorkArea[0] == 0x8F) && (WorkArea[1] == 0x02))
                    //    DataLengthA = DataLengthA + WorkArea[2];
                    //DataLength = (((WorkArea[3]&0x70) >>4)  + 1 + DataLengthA ) * 18 + 2;   // Data Length calculate
                    //DataH = (DataLength >> 0x08) & 0x00FF;
                    //DataL = DataLength & 0x00FF;
                    /*mark,read CD-TEXT subcode data error*/
/*----------------------------------------------------------------------------------------------------------------
            _________________________________________________________________
Page 2A7    |   |   |   |   |ID0|ID1|ID2|ID3|D 0|D 1|D 2|D 3|D 4|D 5|D 6|D 7|
            |___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|
            |D 8|D 9|D10|D11|CRC|CRC| repeat                                |
            |___|___|___|___|___|___|_______________________________________
*/
              /* read CD-TEXT subcode data error */

            if(_crc_data_check((BYTE *)WorkArea) == FALSE){
                error_counter++;
            for(k=0;k<18;k++)
                        RESERVE_ERROR_PACK[page_counter][k]=WorkArea[k];

            page_counter++;
                    if(page_counter==2)
                        page_counter=0;
                    if(correct_pack_flag==1){ //first crc data check False, correct_pack_flag=0
                        if(PageCnt == pre_page+1){ //page continue error
                for(k=0;k<18;k++){
                    CORRECT_PACK[k]=RESERVE_ERROR_PACK[k2][k];
                    for(k1=k+1;k1<18;k1++) //Data disorder
                            CORRECT_PACK[k1]=RESERVE_ERROR_PACK[k3][k1];
                            if(_crc_data_check((BYTE *)CORRECT_PACK) == TRUE){
                                if((CORRECT_PACK[0] == 0x8F) && (CORRECT_PACK[1] == 0x02)){
                                    DataLengthA = DataLengthA + CORRECT_PACK[2];
                                    DataLength = (((CORRECT_PACK[3]&0x70) >>4)  + 1 + DataLengthA ) * 18 + 2;   // Data Length calculate
                                    DataH = (DataLength >> 0x08) & 0x00FF;
                                    DataL = DataLength & 0x00FF;
                                    }
                    j=CORRECT_PACK[2];
                            RAM_Buffer(j,18,(BYTE *)CORRECT_PACK);
                            k2=k2^0x01;
                    k3=k3^0x01;
                    break;
                            }
                        }
                if(_crc_data_check((BYTE *)CORRECT_PACK) == FALSE){ //avoid again renew compose subcode data, have error
                    return (FALSE);
                }
                }else{         //page no continue error, retry seek lead in area
                return (FALSE);
                }
            }
                pre_page=PageCnt ;
                    correct_pack_flag = 1;
                        error_occur_flag = 1;
                    }else{
                    if((WorkArea[0] == 0x8F) && (WorkArea[1] == 0x02)){
                            DataLengthA = DataLengthA + WorkArea[2];
                            DataLength = (((WorkArea[3]&0x70) >>4)  + 1 + DataLengthA ) * 18 + 2;   // Data Length calculate
                            DataH = (DataLength >> 0x08) & 0x00FF;
                            DataL = DataLength & 0x00FF;
                        }
                        //j=WorkArea[2];          //Nero Home(8.3.2.1) cannot show TCD-798 CD-TEXT information in WinXP English OS.
                        //RAM_Buffer(j,18,(BYTE *)WorkArea);
            }
                    if(again_read_flag==1){ // read 1 page again
                if(i==3){ //get total 4 pack
                    SetHeader_4B(0,DataH);
                    SetHeader_4B(1,DataL);
                    SetHeader_4B(2,0x00);
                    SetHeader_4B(3,0x00);
                    return (TRUE);
                }
            }

                    if( (j * 18 + 4)  < CDTEXT_SIZE) //CD-TEXT Pack format length:18 bytes, Header: 4.
                        RAM_Buffer(j,18,(BYTE *)WorkArea); //Nero Home(8.3.2.1) cannot show TCD-798 CD-TEXT information in WinXP English OS.
                    j = j + 1;
                }
            }
        }
    }
    return (FALSE);
}


/*--------------------------------------------------------------------
  Name: Buffer_RAM()
  Description:
  Inputs:
    PageNum
    ofset
    Len
    ram
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void Buffer_RAM(USHORT PageNum, USHORT ofset, USHORT Len, BYTE *ram)
{
    BYTE a;
    BYTE data;
    ULONG work_addr;

    work_addr = PAGE_AUX_START_ADDR(PageNum);

    for(a = 0; a < Len; a++)
    {
        data = SCSDRAM[work_addr + AUX_SUBCODEDATA_ADDR + (ofset % 4) * 18 + a];
        *ram = data;
        ram++;
    }
}


/*--------------------------------------------------------------------
  Name: RAM_Buffer()
  Description:
  Inputs:
    ofset
    Len
    ram
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void RAM_Buffer(USHORT ofset, USHORT Len, BYTE *ram)
{
    BYTE a;
    BYTE data;


    for(a = 0; a < Len; a++){
        data = *ram;
        ram ++;
        SCSDRAM[CDTEXT_AREA + ofset * 18 + 4 + a] = data;
    }
}


/*--------------------------------------------------------------------
  Name: SetHeader_4B()
  Description:
  Inputs:
    a - offset
    data
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void SetHeader_4B(BYTE a, BYTE data)
{
    SCSDRAM[CDTEXT_AREA + a] = data;
}


/*--------------------------------------------------------------------
  Name: CheckIfDiskKindIsCDText()
  Description:
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void CheckIfDiskKindIsCDText(void)
{
    USHORT  PageCnt;
    USHORT  i;
    BYTE    WorkArea[18];

    TextFlag = 0;
    for(PageCnt = 0; PageCnt < 0x05; PageCnt ++){
        for(i = 0; i < 4; i ++){
            Buffer_RAM(PageCnt,i,18,(BYTE *)WorkArea);

            if((WorkArea[0] & 0xf0) == 0x80){   /* CD TEXT information(ID1=8x) ? */
                TextFlag = 1;
                return;
            }
        }
    }
}

/*--------------------------------------------------------------------
  Name: _crc_data_check()
  Description: if SCOR invalid count is not zero,Check CRC data before get CD-TEXT subcode data,
                    avoid get incorrect subcode data
  Inputs:
    data
  Outputs:
    none
  Returns:
    TRUE
    FALSE
--------------------------------------------------------------------*/
BYTE _crc_data_check(BYTE *data)
{
    BYTE  i, j;
    USHORT crc_data, ref_data;

    ref_data = *(data + 16);
    ref_data = ref_data * 0x100 + *(data + 17);

    for(i = 0, crc_data = 0; i < 16; i ++, data ++)
    {
        for(j = 0x80; j != 0; j = j >> 1)
        {
            if(((*data & j) != 0) != ((crc_data & 0x8000) != 0))
            {
                crc_data = ((crc_data << 1) ^ 0x1020) | 0x0001;
            }
            else
            {
                crc_data = crc_data << 1;
            }
        }
    }

    crc_data = ~crc_data;
    if(crc_data != ref_data)
    {
        return FALSE;
    }
    return TRUE;

}

/*--------------------------------------------------------------------
  Name: ConvertTOCLBAToMSF()
  Description:
    This function use for DVD disc toc data LBA ==> MSF
    Gets LBA from toc area, change to MSF format and saves into
    scratch buffer.
  Inputs:
    src - point to toc area
    dest - point to scratch buffer area
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
static void ConvertTOCLBAToMSF(ULONG*dest_ptr, ULONG *src_ptr)
{
ADDRESS_T addr;
BYTE temp;

    Iram_Buf_dram_addr = *src_ptr;
    addr.lba = 0;

    temp = ReadSDRAM();
    addr.lba += temp << 24;
    temp = ReadSDRAM();
    addr.lba += temp << 16;
    temp = ReadSDRAM();
    addr.lba += temp << 8;
    temp = ReadSDRAM();
    addr.lba += temp;

    *src_ptr = Iram_Buf_dram_addr;

    if(addr.lba > 0x119369) //255:59.74
        addr.lba = 0x119369;

    addr.msf = LBA2MSF(addr.lba);

    Iram_Buf_dram_addr = *dest_ptr;
    WriteSDRAM(0x00);
    WriteSDRAM(addr.msf.Minute);
    WriteSDRAM(addr.msf.Second);
    WriteSDRAM(addr.msf.Frame);

    *dest_ptr = Iram_Buf_dram_addr;
}

/*********************************************************************
* Function:     CopySDRAMToSDRAM()
*
* Description:  DRAM copy function
*
* Returns:      none
* Params:       source address, destination address, size
*********************************************************************/
void CopySDRAMToSDRAM(ULONG src,ULONG dest,USHORT size)
{
    do
    {

        SCSDRAM[dest] = SCSDRAM[src];

        src++;
        dest++;
    } while (--size);
}


/*********************************************************************
* Function:     RestoreTOCData()
*
* Description: Restore TOC data from shadow location
*
* Returns:      None
* Params:       None
*********************************************************************/
void RestoreTOCData(void)
{
    ULONG   src,dest;
    USHORT  cnt;

    cnt  = (USHORT)(RDTOC_PMA_LEN + RDTOC_ATIP_LEN);
    src  = SHADOW_DB_INFO_ADR;
    dest = RDTOC_PMA_ADDR;
    CopySDRAMToSDRAM(src,dest,cnt);

    src += cnt;
    cnt  = (USHORT)PREGAP_TIME_LEN;
    dest = PREGAP_TIME_ADDR;
    CopySDRAMToSDRAM(src,dest,cnt);

    src += cnt;
    cnt  = (USHORT)RDTOC_TOC_LEN + (USHORT)RDTOC_SESSION_LEN + (USHORT)RDTOC_FULLTOC_LEN;
    dest = RDTOC_TOC_ADDR;
    CopySDRAMToSDRAM(src,dest,cnt);

    src += cnt;
    cnt  = (USHORT)DB_INFO_LEN;
    dest = DB_INFO_ADR;
    CopySDRAMToSDRAM(src,dest,cnt);
}

/*********************************************************************
* Function:     StoreTOCData()
*
* Description: Store TOC data to a shadow location
*
* Returns:      None
* Params:       None
*********************************************************************/
void StoreTOCData(void)
{
    ULONG   src,dest;
    USHORT  cnt;

    cnt  = (USHORT)(RDTOC_PMA_LEN + RDTOC_ATIP_LEN);
    src  = RDTOC_PMA_ADDR;
    dest = SHADOW_DB_INFO_ADR;
    CopySDRAMToSDRAM(src,dest,cnt);

    dest += cnt;
    cnt  = PREGAP_TIME_LEN;
    src  = PREGAP_TIME_ADDR;
    CopySDRAMToSDRAM(src,dest,cnt);

    dest += cnt;
    cnt  = (USHORT)RDTOC_TOC_LEN + (USHORT)RDTOC_SESSION_LEN + (USHORT)RDTOC_FULLTOC_LEN;
    src  = RDTOC_TOC_ADDR;
    CopySDRAMToSDRAM(src,dest,cnt);

    dest += cnt;
    cnt  = DB_INFO_LEN;
    src  = DB_INFO_ADR;
    CopySDRAMToSDRAM(src,dest,cnt);
}
