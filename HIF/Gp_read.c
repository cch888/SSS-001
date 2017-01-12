/****************************************************************************
*                (c) Copyright 2001 -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_read.c
*
* DESCRIPTION
*   This file contains read command handler
*
*   $Revision: 124 $
*   $Date: 11/03/31 2:43p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\constant.h"
#include ".\common\error.h"
#include ".\common\system.h"
#include ".\common\commath.h"
#include ".\common\sdram.h"

#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_rdcd.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\s2b_main.h"
#include ".\hif\gp_read.h"
#include ".\hif\gp_audio.h"
#include ".\dvd\dvhostrd.h"

#include ".\oem\oem_spd_tbl.h"

 #if (HORROR_DISC_IMPROVEMENT == 1)
#include "..\al\REG_dfe.h"
#endif
#include ".\al\REG_global.h"
#include ".\al\REG_cpycmp.h"
#include ".\al\REG_audio.h"
#include ".\al\LAL_cd_dvd_codec.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#if (BD_ENABLE == 1)
#include ".\bd\bdBufMgr.h"
#include ".\defmgmt\bdwritable\bdranaccdef.h"
#endif

static void CheckAUXFormat(void);
void read_common(void);
static void ReadCDDACommand(void);
void ReadCDExit(void);
void ProcessEjectRequest(void);
void CdTransferHostLeftData(void);

extern BOOL CheckTrackMode(void);

extern vBYTE Track_No;
vBYTE rd_method;
vBYTE Read_Lead_in;
#if (AUTO_STATU_CPLT==1)
vBOOL Read_Zero_Length;
#endif

/********************************************************
* CDRead10Command/12
*
* Read (10) Command Handler
* Read (12) Command Handler
*
* Returns:      void
* Params:       none
********************************************************/
void CDRead10Command(void)
{
    BYTE state;
    long h2go;
    Read_Lead_in = FALSE;

    if(St_blank_disc == TRUE)
    {
        BUILD_SENSE(0x05,0x64,0x00,0x69);
        ReadCDExit();
        return;
    }

    if ( Iram_Cmd_block.bf[0] == SBC_READ_12 )
    {
        /* If both FUA bit & Streaming bit are set to one, the drive shall terminate the Command. */
        if ( (Iram_Cmd_block.bf[1] & 0x08) && (Iram_Cmd_block.bf[10] & 0x80) )
        {
            BUILD_SENSE(0x05,0x24,0x00,0xee);
            ReadCDExit();
            return;
        }
    }

    Rw_C2PO_data = FALSE;
    Rw_subc_data = FALSE;
    Read_subc_only = FALSE;
    Iram_Rd_subc_flag = FALSE;
    Rw_rdcd_cmd = FALSE;
#if (SUN_FUNC_TEST == 1)
    RdC2POData_296 = FALSE;
    RdC2PODataTaggle = FALSE;    
#endif

/*** save host request start address to buf_start_blk ***/
    h2go = HostXfer.StartBlock.lba = PacketCommandToLBALEN4(2);

    if (h2go<0)
        Read_Lead_in = TRUE;

    if((Read_Lead_in == FALSE) && (Iram_Cmd_block.bf[2] != 0 || (HostXfer.StartBlock.lba > Maxlba)))
    {
        /* exceeded leadout */
        BUILD_SENSE(0x05,0x21,0x00,0x6a);            /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        ReadCDExit();
        return;
    }

/*** check requested block's track writing method ***/
    rd_method = CheckTrackWriteMethod(HostXfer.StartBlock.lba) & 0x0F;

    if((pdbGetTrackContAddr(Track_No) & 0x40) != 0x40)
    {
            /*** Audio track ***/
            BUILD_SENSE(0x05,0x64,0x00,0x6c);    /* ILLEGAL MODE FOR THIS TRACK */
            ReadCDExit();
            return;
    }

    if(rd_method == WR_METHOD_FP)
    {
        if(!St_method_2 ==TRUE)
        {
            St_method_2 = TRUE;
            StopDecoder();
            ClearDataBuffer();
        }
    }
    else
    {
        if(St_method_2 == TRUE)
        {
            St_method_2 = FALSE;
            StopDecoder();
            ClearDataBuffer();
        }
    }

    if(pdbServoGetDiscType() == eATIPDisc)
    {
        /*** check blank track ***/
        if (Track_No != 0)
        {
            state = pdbGetTrackState(Track_No);
            if(state == eTRACK_STATE_EMPTY || state == eTRACK_STATE_BLANK)
            {
                BUILD_SENSE(0x05,0x21,0x00,0x6b);    /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                ReadCDExit();
                return;
            }
        }
    }

    HostXfer.StartBlock.lba = h2go;

/*** Assign Remained Host Request Length ***/
    if ( (Iram_Cmd_block.bf[0] == SBC_READ_10) || (Iram_Cmd_block.bf[0] == SBC_VERIFY_10) )     /* READ/VERIFY (10) Command */
    {                                       /* host request length */
        HostXfer.BlocksLeft = h2go = PacketCommandToLBALEN2(7);
    }
    else                                        /* READ (12) Command */
    {                                           /* host request length */
        HostXfer.BlocksLeft = h2go = PacketCommandToLBALEN3(7); /* !! Byte 6 Ignored */
    }

    if ( h2go != 0 )
    {
        /* Calculate the ending address, subtract one blk to get the real
           ending lba address. */
        h2go += HostXfer.StartBlock.lba;                /* add req len to start lba */
        h2go--;
        Tg_endlba = h2go;

        if((Read_Lead_in == FALSE) &&  ( h2go > Maxlba ))
        {                                       /* exceeded leadout */
            BUILD_SENSE(0x05,0x21,0x00,0x6d);        /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            ReadCDExit();
            return;
        }
    }

    read_common();

    return;
}


/*********************************************************************
* ReadCDCommand()
*
* Read CD command handler
*
* Returns:      void
* Params:       none
*********************************************************************/
void ReadCDCommand(void)
{
    long h2go;
    BYTE state;

    Read_Lead_in = FALSE;
    if(St_blank_disc == TRUE)
    {
        BUILD_SENSE(0x05,0x21,0x00,0x6f);
        ReadCDExit();
        return;
    }

/*** save host request LBA  ****/
    h2go = HostXfer.StartBlock.lba = PacketCommandToLBALEN4(2);

    if (h2go<0)
        Read_Lead_in = TRUE;

/*** check subcode request ***/
    if (Iram_Cmd_block.bf[9] != 0)
    {
        if ((Iram_Cmd_block.bf[9]==0x10)&&(Iram_Cmd_block.bf[1]==0x04))
        {
            //if(/*(Iram_Rw_read_mode == eCDDAMode)||*/(St_cdrom_audio == TRUE))
            {
                if(APLastAskedSpeed == 0)
                {
#if (SYSCLK == 100)
                    if(GetSpeedValue(MAX_READ_SPEED) > GetSpeedValue(e40XCAV))  //Only allow using lower speed
                        SetReadSpeed(e40XCAV);
                    else
#endif
                        SetReadSpeed(MAX_READ_SPEED);
                }
                else
                {
#if (SYSCLK == 100)
                    if(GetSpeedValue(APLastAskedSpeed) > GetSpeedValue(e40XCAV))  //Only allow using lower speed
                        SetReadSpeed(e40XCAV);
                    else
#endif
                    SetReadSpeed(APLastAskedSpeed);
                }
                SpeedCount = (CD_SPEED_UP_READY + CD_SPEED_DOWN_READY)/2;               //Prevent changing speed
            }
        }

        if (((Iram_Cmd_block.bf[9] & 0x6) == 0x02)&&(Iram_Page_Size_3K_Flag == FALSE))
        {/*Automatic change from 2.5K to 3k page for C2PO*/
            StopDecoder();
            ClearDataBuffer();

            Iram_Page_Size_3K_Flag = TRUE;
            DRAM_AUX_SIZE = DRAM_AUX_SIZE_3K;   /* DRAM AUX Size = 0x400(= 1KB) */
            RING_END_PAGE = RING_END_PAGE_3K;   /* last page the SP can access*/
            BUF_TOTAL_BLK_COUNT = BUF_TOTAL_BLK_COUNT_3K;   /* total 672 pages */
            DATA_START_ADDR = DATA_START_ADDR_3K;   /* Data start address */
            AUX_START_ADDR = AUX_START_ADDR_3K; /* Aux start address  */
            AUX_HEADER_ADDR1 = AUX_HEADER_ADDR1_3K;    /* M1 block header offset in page */
            AUX_HEADER_ADDR2 = AUX_HEADER_ADDR2_3K;    /* M2F1 block header offset in page */

            Iram_usiRingEP  = RING_END_PAGE;

            WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);     /* Host end blk. */
            WRITE_FIELD(HAL_CDD_DSKBUFEPG, Iram_usiRingEP);     /* ring end blk. */

    #if (AUDIO_PLAYBACK == 1)
        #if (SUN_FUNC_TEST == 1)
            if(READ_FIELD(HAL_GLOBAL_AUDCKSTP))
            {
                WRITE_FIELD(HAL_GLOBAL_AUDCKSTP, 0);                /* Enable copy clock */
                for (state=0;state<0x05;state++);
                WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
                WRITE_FIELD(HAL_GLOBAL_AUDCKSTP, 1);                /* Disable copy clock */
            }
            else
        #endif
            WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
    #endif
            if(READ_FIELD(HAL_GLOBAL_CPYCKSTP))
            {
                WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0);                /* Enable copy clock */
                DelaymS(5);
                WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */
                WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 1);                /* Disable copy clock */
            }
            else
                WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */

            WRITE_FIELD(HAL_CDD_MLY, e_Page3K);                         /* Memory layout 3K */
            WRITE_FIELD(HAL_CDD_C2P, e_C2_ERROR_COUNT);
            WRITE_FIELD(HAL_CDD_C2WEN, SET);

            Iram_shHDDir &= ~HAL_HST_HMLY;          /* 3K Host Layout*/
            WRITE_FIELD(HAL_HST_HMLY, 0);           /* 3K Host Layout*/
        }

    /* read data with subcode */
    if ((Iram_Cmd_block.bf[10] != 0) && (Iram_Rd_subc_flag == FALSE))
    {
        Iram_Rd_subc_flag = TRUE;
        StopDecoder();
        ClearDataBuffer();
    }

        //if ((Read_Lead_in == FALSE) && ((Iram_Cmd_block.bf[2] != 0) || (HostXfer.StartBlock.lba > Maxlba)))
        if ((Read_Lead_in == FALSE) && ((Iram_Cmd_block.bf[2] != 0) || ((long)HostXfer.StartBlock.lba > (long)(Maxlba+1))))

        {/* exceeded leadout */
            Svr_pause_reqire = TRUE;  /* spin down in the end of Nero DAE test */

            BUILD_SENSE(0x05,0x21,0x00,0x70);    /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            ReadCDExit();
            return;
        }

/*** check requested block's track writing method ***/
        rd_method = CheckTrackWriteMethod(HostXfer.StartBlock.lba);
        if(rd_method == TRKNO_NOTFOUND)
        {
            BUILD_SENSE(0x05,0x21,0x00,0x71);    /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            ReadCDExit();
            return;
        }
        else
            rd_method &= 0x0F;

        if(rd_method == WR_METHOD_FP)
        {
            if(!St_method_2 == TRUE)
            {
                St_method_2 = TRUE;
                StopDecoder();
                ClearDataBuffer();
            }
        }
        else
        {
            if(St_method_2 == TRUE)
            {
                St_method_2 = FALSE;
                StopDecoder();
                ClearDataBuffer();
            }
        }
/**** check blank track ***/
        if(pdbServoGetDiscType() == eATIPDisc){
            if (Track_No != 0){
                state = pdbGetTrackState(Track_No);
                if(state == eTRACK_STATE_EMPTY || state == eTRACK_STATE_BLANK){
                    BUILD_SENSE(0x05,0x21,0x00,0x72);
                    ReadCDExit();
                    return;
                }
            }
        }
/**** check track size***/
        if(St_cdrom_audio)
        {
            ULONG TrkEndLBA;
            TimeS TrkStopMSF;
            BYTE trknum;
            trknum = FindTrackNumber(h2go);
            TrkStopMSF = pdbGetStopTime(trknum);
            TrkEndLBA = MSF2LBA(&TrkStopMSF);
            if(TrkEndLBA<h2go)
            {
                    BUILD_SENSE(0x05,0x64,0x00,0x80);
                    ReadCDExit();
                    return;
            }
        }
        HostXfer.StartBlock.lba = h2go;
/*** get request block number in BYTE 7 and 8 BYTE 7 (msb) BYTE 8 (lsb) ****/
        HostXfer.BlocksLeft = h2go = PacketCommandToLBALEN3(6);
/*** check lba boundary ***/
        if((long)(HostXfer.StartBlock.lba + h2go) > (long)(Maxlba + 2)  /* exceed lead-out + 1, for Nero CD Speed read LeadOut test */
            || (long)(HostXfer.StartBlock.lba) < -1L)       /* can read pre-gap 1 block */
        {
            /* exceeded leadout */
            BUILD_SENSE(0x05,0x21,0x00,0x73);    /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            ReadCDExit();
            return;
        }

        if (h2go != 0)  /* if req xfer length is not zero */
        {
            /* To calculate the ending address, after add the request length to the
               start lba, we need subtract one blk to get the real ending lba address. */
            h2go += HostXfer.StartBlock.lba;
            h2go--;         /* subtract one from result */
            Tg_endlba = h2go;

            //if ((Read_Lead_in == FALSE)&&(h2go > Maxlba))
            if ((Read_Lead_in == FALSE)&&(h2go > (Maxlba+1)))
            {
                BUILD_SENSE(0x05,0x21,0x00,0x74);    /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                ReadCDExit();
                return;
            }
        }
        CheckAUXFormat();                   /* not exceed(); */
        return;
    }
    if ( Iram_Cmd_block.bf[10] != 0 )               /* sub-channel data selected */
    {
        if ( CheckForFF((BYTE *)&Iram_Cmd_block.bf[2], 4) == TRUE )
        {
            /* if request current position */
            if (( Iram_Svr_player_mode != PLAY_TRACK_MODE ) &&
                ( St_nx_audio == FALSE ))
            {
                /* if not in audio mode, not playing audio now */
                BUILD_SENSE(0x05,0xB9,0x00,0x75);    /* PLAY OPERATION ABORTED */
                ReadCDExit();
                return;
            }
        }
        else
        {

            //if (HostXfer.StartBlock.lba > Maxlba)
            if (HostXfer.StartBlock.lba > (Maxlba+1))
            {
                BUILD_SENSE(0x05,0x21,0x00,0x76);    /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                ReadCDExit();
                return;
            }

        }
        /* setup host request block to xfr length */
        HostXfer.BlocksLeft = PacketCommandToLBALEN3(6);
        CheckAUXFormat();
        return;
    }
    ReadCDExit();
    return;
}

/********************************************************
* ReadCDMSFCommand
*
* Read CD MSF command handler
*
* Returns:      void
* Params:       none
*********************************************************/
void ReadCDMSFCommand(void)
{
    long h2go;
    BYTE    state;

    if(St_blank_disc == TRUE)
    {
        BUILD_SENSE(0x05,0x21,0x00,0x77);
        ReadCDExit();
        return;
    }

/*** Check for valid MSF in starting and ending address fields ***/
    if ( CheckValidMSF( (BYTE *)&Iram_Cmd_block.bf[3]) ||
         CheckValidMSF( (BYTE *)&Iram_Cmd_block.bf[6]) )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x75);          /* INVALID FIELD IN CDB */
        ReadCDExit();
        return;
    }


/*** Get Start lba ***/
    DISABLE_INTERRUPT();
    HostXfer.StartBlock.lba = MSF2LBA(&Iram_Cmd_block.bf[3]);
    ENABLE_INTERRUPT();

    /* check requested block's track writing method */
    h2go = HostXfer.StartBlock.lba;                 /* save HostXfer.StartBlock.lba */

#if (SUN_FUNC_TEST == 1)
    if (((Iram_Cmd_block.bf[9] & 0x6) != 0)&&(Iram_Page_Size_3K_Flag == FALSE)) //20101122_02WB == Drive loss issue in Gear2, Tonyh modify, Elvis confirm
    {/*Automatic change from 2.5K to 3k page for C2PO*/
        StopDecoder();
        ClearDataBuffer();

        Iram_Page_Size_3K_Flag = TRUE;
        DRAM_AUX_SIZE = DRAM_AUX_SIZE_3K;   /* DRAM AUX Size = 0x400(= 1KB) */
        RING_END_PAGE = RING_END_PAGE_3K;   /* last page the SP can access*/
        BUF_TOTAL_BLK_COUNT = BUF_TOTAL_BLK_COUNT_3K;   /* total 672 pages */
        DATA_START_ADDR = DATA_START_ADDR_3K;   /* Data start address */
        AUX_START_ADDR = AUX_START_ADDR_3K; /* Aux start address  */
        AUX_HEADER_ADDR1 = AUX_HEADER_ADDR1_3K;    /* M1 block header offset in page */
        AUX_HEADER_ADDR2 = AUX_HEADER_ADDR2_3K;    /* M2F1 block header offset in page */

        Iram_usiRingEP  = RING_END_PAGE;

        WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);     /* Host end blk. */
        WRITE_FIELD(HAL_CDD_DSKBUFEPG, Iram_usiRingEP);     /* ring end blk. */

    #if (AUDIO_PLAYBACK == 1)
        if(READ_FIELD(HAL_GLOBAL_AUDCKSTP))
        {
            WRITE_FIELD(HAL_GLOBAL_AUDCKSTP, 0);                /* Enable copy clock */
            for (state=0;state<0x05;state++);
            WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
            WRITE_FIELD(HAL_GLOBAL_AUDCKSTP, 1);                /* Disable copy clock */
        }
        else
            WRITE_FIELD(HAL_AUD_AENDPG, Iram_usiRingEP);
    #endif

        if(READ_FIELD(HAL_GLOBAL_CPYCKSTP))
        {
            WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 0);                /* Enable copy clock */
            for (state=0;state<0x05;state++);
            WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */
            WRITE_FIELD(HAL_GLOBAL_CPYCKSTP, 1);                /* Disable copy clock */
        }
        else
            WRITE_FIELD(HAL_CPYCMP_CPYEPG, Iram_usiRingEP);     /* Copy Buffer End Page */

        WRITE_FIELD(HAL_CDD_MLY, e_Page3K);                         /* Memory layout 3K */
        WRITE_FIELD(HAL_CDD_C2P, e_C2_ERROR_COUNT);
        WRITE_FIELD(HAL_CDD_C2WEN, SET);

        Iram_shHDDir &= ~HAL_HST_HMLY;          /* 3K Host Layout*/
        WRITE_FIELD(HAL_HST_HMLY, 0);           /* 3K Host Layout*/
    }
#endif
/*** check requested block's track writing method ***/
        rd_method = CheckTrackWriteMethod(HostXfer.StartBlock.lba) & 0x0F;
        if(rd_method == WR_METHOD_FP)
        {
            if(!St_method_2 == TRUE)
            {
                St_method_2 = TRUE;
                StopDecoder();
                ClearDataBuffer();
            }
        }
        else
        {
            if(St_method_2 == TRUE)
            {
                St_method_2 = FALSE;
                StopDecoder();
                ClearDataBuffer();
            }
        }
/**** check blank track ***/
        if(pdbServoGetDiscType() == eATIPDisc){
            if (Track_No != 0){
                state = pdbGetTrackState(Track_No);
                if(state == eTRACK_STATE_EMPTY || state == eTRACK_STATE_BLANK){
                    BUILD_SENSE(0x05,0x21,0x00,0x78);
                    ReadCDExit();
                    return;
                }
            }
        }
/**** check track size***/
        if(St_cdrom_audio)
        {
            ULONG TrkEndLBA;
            TimeS TrkStopMSF;
            BYTE trknum;
            trknum = FindTrackNumber(h2go);
            TrkStopMSF = pdbGetStopTime(trknum);
            TrkEndLBA = MSF2LBA(&TrkStopMSF);
            if(TrkEndLBA<h2go)
            {
                    BUILD_SENSE(0x05,0x64,0x00,0x81);
                    ReadCDExit();
                    return;
            }
        }

    rd_method = CheckTrackWriteMethod(HostXfer.StartBlock.lba);

    if(St_method_2 == TRUE)
    {
        St_method_2 = FALSE;
        StopDecoder();
        ClearDataBuffer();
    }

/*** save host request msf(bcd)  ****/
    HostXfer.StartBlock.lba = h2go;
/*** Get End lba ***/
    DISABLE_INTERRUPT();
    h2go = MSF2LBA(&Iram_Cmd_block.bf[6]);
    Tg_endlba = h2go;
    ENABLE_INTERRUPT();
                                                /* calculate the xfer length */
    if (h2go > Maxlba)                          /* if end MSF exceed Maxlba */
    {
        BUILD_SENSE(0x05,0x21,0x00,0x7a);            /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        ReadCDExit();
        return;
    }

    if ( h2go < HostXfer.StartBlock.lba )               /* calculate the xfer length */
    {
        BUILD_SENSE(0x05,0x24,0x00,0x76);            /* INVALID FIELD IN CDB */
        ReadCDExit();
        return;
    }
    HostXfer.BlocksLeft = (ULONG)(h2go - HostXfer.StartBlock.lba);
    CheckAUXFormat();
}

/********************************************************
* CheckAUXFormat
*
* common handler for Read CD MSF command and Read CD
*
* Returns:      void
* Params:       none
*********************************************************/
static void CheckAUXFormat(void)
{
    /*** Chk if request Error Flag ***/
    Rw_C2PO_data = FALSE;
#if (SUN_FUNC_TEST == 1)
    RdC2POData_296 = FALSE;
    RdC2PODataTaggle = FALSE;
#endif
    //SetBitsSCReg_1B(rwSubQC, bBufSBC);    /* enable sub channel buffering *//* no read CD-G subcode data */


if (Iram_Page_Size_3K_Flag == 1){/*Automatic change from 2.5K to 3k page for C2PO*/
#if (SUN_FUNC_TEST == 1)
    if (((Iram_Cmd_block.bf[9] & 0x6) == 0x02)||((Iram_Cmd_block.bf[9] & 0x6) == 0x04))
#else
    if ((Iram_Cmd_block.bf[9] & 0x6) == 0x02)
#endif
    {
        Rw_C2PO_data = TRUE;
        Iram_Cmd_block.bf[9] &= 0xf8;
#if (SUN_FUNC_TEST == 1)
        if((Iram_Cmd_block.bf[9] & 0x6) == 0x04)
            RdC2POData_296 = TRUE;        
#endif
    }
    else if ((Iram_Cmd_block.bf[9] & 0x4) != 0)
            {
        /*** req. c2 and block err flag ***/
        BUILD_SENSE(0x05,0x24,0x00,0x77);    /* INVALID FIELD IN CDB */
                ReadCDExit();
                return;
            }

}else{
            if ((Iram_Cmd_block.bf[9] & 0x6) != 0)
            {
/*** req. err flag ***/
                BUILD_SENSE(0x05,0x24,0x00,0x78);    /* INVALID FIELD IN CDB */
                ReadCDExit();
                return;
            }
}




    /*** chk if request raw r-w subcode ***/
    switch (Iram_Cmd_block.bf[10])
    {
        case 1:             /* raw sub-channel data */
        case 4:             /* r-w data */
        case 2:             /* q data */
            if ( HostXfer.BlocksLeft == 0 ) /* if it is zero */
            {
                ReadCDExit();
                return;
            }

            /* read subcode only */
            if (Iram_Cmd_block.bf[9] == 0)
                Read_subc_only = TRUE;
            else
                Read_subc_only = FALSE;

            Rw_subc_data = TRUE;                /* mark read subcode data */
            Rw_rdcd_cmd = TRUE;                 /* mark read cd command */

#if ENABLE_CDG == 1
            /* CD-G, In case play audio command in progress and only sub-channel
               data requested, read audio sub-channel data from current location */
            if ( St_audio_complete == FALSE )
            {
                Rw_rdcd_cmd = FALSE;    /* mark not read cd command */
                ReadAudioSubCode();
                return;
            }
#endif /* ENABLE_CDG */
            if (!St_method_2)
            {
                Rw_seek_blk.msf = LBA2MSF(HostXfer.StartBlock.lba);
            }
            else
            {
                Rw_seek_blk.msf = Method2LBAToMSF(HostXfer.StartBlock.lba);
            }

            if (CheckTrackMode() == FALSE ) /* non-audio trk read */
            {
/*** Data track ***/
                Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                read_common();
                return;
            }else{
/*** Audio track ***/
                //Iram_Cmd_block.bf[9] = 0xf8;
                Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                ReadCDDACommand();
                return;
            }

        case 0:                 /* No sub-channel data */
            if ((HostXfer.BlocksLeft == 0) || (Iram_Cmd_block.bf[9] == 0))
            {
                ReadCDExit();
                return;
            }
            if ((Iram_Cmd_block.bf[1] & 0x1c) >= 0x18)
            {
                BUILD_SENSE(0x05,0x24,0x00,0x79);    /* INVALID FIELD IN CDB */
                ReadCDExit();
                return;
            }

            Rw_subc_data = FALSE;               /* mark not read subcode data */
            Read_subc_only = FALSE;             /* read subcode only */
            Rw_rdcd_cmd = TRUE;                 /* mark read cd command */

                if (!St_method_2)
                {
                    Rw_seek_blk.msf = LBA2MSF(HostXfer.StartBlock.lba);
                }
                else
                {
                    Rw_seek_blk.msf = Method2LBAToMSF(HostXfer.StartBlock.lba);
                }

#if WITHOUT_SERVO == 0
                if (CheckTrackMode() == FALSE)
                {
/*** Data track ***/
                    Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                    read_common();
                    return;
                }
                else
#endif /* WITHOUT_SERVO */
                {
/*** Audio track ***/
                    //Iram_Cmd_block.bf[9] = 0xf8;
                    Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                    ReadCDDACommand();    /* WITHOUT_SERVO read CD-DA data testing */
                    return;
                }

        default:
            //start== HP CMPLY test TCDR702 fail.
            //BUILD_SENSE(0x05,0x24,0x00,0x7a);        /* INVALID FIELD IN CDB */
            //end==
            ReadCDExit();
            return;
    }
}


/********************************************************
* read_common
*
* Common read commands Handler
*
* Returns:      void
* Params:       none
********************************************************/
BYTE retry_flag;    /* for read mix mode disc */
#if (BY_PASS_C2Error_For_VCD)
ULONG ReadCDSeekTimer;        /* For reduse the seek time */
#endif
#if ((CEProduct == 1)&&(DelayCDSpeedDown == 0))
BYTE SeekCounterForEDCN;   /* Modify for play ability */
#endif
#if (CDPlayability == 1)
BYTE CDBufferAlmostEmpty;
#endif
#if (CHIP_REV <= 0xB2)
BYTE UseC1CG1;
#endif
void read_common(void)
{
BYTE a;
USHORT Sa;
vBOOL flg;
BYTE rw_seek_retry;
long lba;
USHORT w_temp;
SHORT  ExitPer256ms, ExitCount;

    /*****************************    FW Bug    ******************************/
    /* The read_timer is used instead of svoIramVar.knlTimeupCount.
        The voIramVar.knlTimeupCount is reset by some called functions in this function,
        and that causes voIramVar.knlTimeupCount is incorrect for this read command..*/

ULONG read_timer;

    /**************************** End FW Bug  2007.05.15 ********************/

#if (BY_PASS_C2Error_For_VCD)
    ULONG read_time_limit;        /* For time limit define */
    ULONG read_reduce_time;    /* For reduce the time define */
#endif

#if (CEProduct == 1)
    /* Return error to backend quickly for bad VCD*/
    BYTE RdMisCnt = 0;
#endif

#if (EN_POWER_CONTROL)
    SetDiscReadPowerMode();
#endif

    StartTimer(&read_timer);

#if (AUTO_STATU_CPLT==1)
    Read_Zero_Length = FALSE;
#endif

#if (CDPlayability == 1)
    CDBufferAlmostEmpty = FALSE;
#endif
#if (CHIP_REV <= 0xB2)
    UseC1CG1 = FALSE;
#endif

    if ( Rw_cdda_disc == TRUE )
    {
        StopCDDADecoder();
        DataBuffer.SectorCount = 0;
        Rw_cdda_disc = FALSE;
    }

    if(SpeedMD == Set_sp_f)
    {
        SpeedMD=Special_f;
    }
#if(CEProduct == 0)
    // bypass speed up/down when CDSpeed test.
    if((SpeedMD == AutoMode) && (HostXfer.BlockCount.Long == 0x19))
    {
        SpeedMD = Special_for_CDSpeed_f;
    }
    else if((SpeedMD == Special_for_CDSpeed_f)&&(HostXfer.BlockCount.Long != 0x19))
    {
        RandomRead = TRUE; //restart the automode.
        SpeedMD = AutoMode;
    }
#endif
/**** Entry point for read zero length. ****/

#if (AUTO_STATU_CPLT==1)
    if ( HostXfer.BlocksLeft == 0 )
        Read_Zero_Length=TRUE;
#endif

    if ( HostXfer.BlocksLeft == 0 )
    {                                           /* if it is zero */
        /* Entry point for read zero length.
           In this routine, we issue a seek then buffer the block.
           If any error occurs, we exit the read without return error. */

        rw_seek_retry = 0;
        Rw_seek_blk.lba = HostXfer.StartBlock.lba;  /* initial seek block */
        AdjustSeekOffset();                      /* execute adjust rw_seek_blk operation */
                                                /* call player module for seek. */
        do
        {
            if ( SeekToTarget() == PASS )
            {
                rw_seek_retry++;
                /* seek successfully completed */
                if ( StartBufferingFromTarget() )        /* buffer first block */
                {
                    /* buffering failed */
                    Svr_pause_reqire = TRUE;    /* mark pause request */
                    StopDecoder();
                    ClearDataBuffer();
                    #if (CEProduct == 1)
                    /* Return error to backend quickly for bad VCD*/
                        if (fRdMiss) RdMisCnt++;
                    #endif
                }
                #if (CEProduct == 1)
                /* Return error to backend quickly for bad VCD*/
                    if (RdMisCnt > 3) break;
                #endif
                ReadCDExit();
                return;
            }
            /* seek has been failed and reported by lower level module */
            rw_seek_retry++;
        } while (ReadTimer(&read_timer) < TIME_6S);
        Svr_pause_reqire = TRUE;                /* mark pause request */
        ReadCDExit();                            /* retry exhausted */
        return;
    }

    HostXfer.BlockCount.Long = HostXfer.BlocksLeft;
/*** check cache hit ***/
    /* dec_block    : latest valid block in fifo
    start_block : first host request block
    if start_block > dec_block
    cache miss, it needs seek and start_buffering
    if start_block <= dec_block
    we need check if start block is < xfer_block */

#if (BY_PASS_C2Error_For_VCD)
    StartTimer(&ReadCDSeekTimer); /* For reduce the seek time */
#endif

    DISABLE_INTERRUPT();                        /* Disable all interrupt */
    LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheEndLBA, lba, flg);

    if (!flg )                                  /* if start > dec */
    {
        if(!READ_FIELD(HAL_CDD_SDBUF))
        {
            goto cache_missx;
        }

        if (lba == 0)
        {
            /* HostXfer.StartBlock = DataBuffer.CacheEndLBA,
             adjust the DataBuffer.CurHostPtr to DataBuffer.CurDiscPtr. */

            if ( DataBuffer.SectorCount != 0 )  /* if buffer is empty */
            {
                DataBuffer.CurHostPtr = DataBuffer.CurDiscPtr;
                if ( DataBuffer.CurHostPtr == 0)
                    DataBuffer.CurHostPtr = RING_END_PAGE;
                else
                    DataBuffer.CurHostPtr --;

                DataBuffer.SectorCount   = 1;

                ENABLE_INTERRUPT();             /* Enable all interrupt */
                DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
                MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA , 1);
                SendHostBlocks();               /* xfr blk to host */
                goto start_read;
            }
        }
        else
        {                                       /* chk_wait_fifo; */
            /* start_block is greater than dec_block. It needs to
                decide to make a new seek or just wait it comes
                If dec_block will come within 8 blocks, we wait it comes.
                Otherwise, make a new seek. */
            /* check if decoder is on. if it is waiting for cache hit
                otherwise just start seek */
            if((lba < 58)&&(Rw_dec_stop == FALSE))
            {
                /* decode block will be coming soon, just wait for it */
                ENABLE_INTERRUPT();
                a = WaitForCacheHit();           /* wait it come */
                if(EjectKeyInfo.PressedValid)
                {                               /* key pushed */
                    BUILD_SENSE(0x02,0x3A,0x00,0x14);    /* MEDIUM NOT PRESENT */
                    ProcessEjectRequest();
                    return;
                }
                if ( a == 0 )
                {                               /* and no error occur in target block */
                    SendHostBlocks();
                    goto start_read;
                }
            }
        }

        /* cache miss, just start seek */
    }
    else
    {
        /* first request block is below last valid block in buffer
        It may possible get partial or full hit
        (start block <= dec block)

        Check if xfred_block <= start_block ==> cache hit*/
        if ( DataBuffer.SectorCount != 0 )                  /* if buffer is empty */
        {
            /* calculate if first request block in inside buffer
            calculation start block - xfred block */

            /* if start_block <= xfred block  ===> cache miss */
            LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheStartLBA, lba, flg);

            if ( lba > 0 )                      /* if start > xfer */
            {
                /* start_block-xfred block >=0 cases
                    check start_block-xfred block=0
                    if yes, it is cache miss */

                if (lba < DataBuffer.SectorCount)
                {
                    /* Entry point of cache hit, it may be partial hit of
                    full hit first request block is inside buffer
                    already know that xfer_block < start_block < dec_block
                    adjust fifo_out pointer and xfred_block
                    calculate start_block-xfer_block */

                    if( (DataBuffer.SectorCount-lba)< HostXfer.BlockCount.Long )
                    {
                        if(!READ_FIELD(HAL_CDD_SDBUF))
                        {
                            goto cache_missx;
                        }
                    }

                    w_temp = (USHORT) lba;

                                                /* cache_hit */
                    w_temp--;
                    if (w_temp != 0)
                    {
                        if (DataBuffer.SectorCount < w_temp )
                            goto cache_missx;

                        DataBuffer.CurHostPtr += w_temp;

                        if ( DataBuffer.CurHostPtr > RING_END_PAGE)
                        {
                            DataBuffer.CurHostPtr %=BUF_TOTAL_BLK_COUNT;
                        }
                        DataBuffer.SectorCount -= w_temp;
                    }
                                                /* adj_xfer_to_start */
                    ENABLE_INTERRUPT();         /* Enable all interrupt */
                    DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
                    MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA, 1);
                    SendHostBlocks();
                    goto start_read;
                }
            }
        }
    }

/*** cache miss, just start seek ***/
cache_missx:
    ENABLE_INTERRUPT();                         /* Enable all interrupt */

    while (Iram_reload == TRUE){
        ExitProcess();
    }

    if (ReadTimer(&read_timer) > TIME_6S){                    /* time out */
        Svr_pause_reqire = TRUE;                /* mark pause request */
        BUILD_SENSE(0x03,0x02,0x00,0x1e);            /* NO SEEK COMPLETE */
        ReadCDExit();
        return;
    }
                                                /* cache_missx: or cache_miss: */
    Iram_Knl_timer_counter = 300;             /* Wait about 6s */
    Knl_timeout = FALSE;                        /* Assume no time out */
#if ((CEProduct == 1)&&(DelayCDSpeedDown == 0))
    SeekCounterForEDCN = 0;
#endif
start_seek:

#if (CDPlayability == 1)
    /* If buffer is almost empty, by pass C2 error for keeping data in buffer*/
    if ((Iram_Cmd_block.bf[0] == MMC_READ_CD)&&(DataBuffer.SectorCount < 26) &&(St_cdrom_video))
        CDBufferAlmostEmpty = TRUE;
#endif

    /* Entry point of seek operation start block is not in buffer,
        we need a seek operation */
    rw_seek_retry = 0;
    Rw_seek_blk.lba = HostXfer.StartBlock.lba;      /* initial seek block */
    AdjustSeekOffset();                          /* execute adjust rw_seek_blk operation */

#if (BY_PASS_C2Error_For_VCD)
    if (St_cdrom_video){
    read_time_limit = 2300;  /* The defult time limit for Video*/
    read_reduce_time = 2000; /* After seek retry 2 times and read_reduce_time will subtract this value */
    }else{
    read_time_limit = 5000;
    read_reduce_time = 3000;
    }
#endif

    while (1)
    {                                           /* seek loop */
        if(EjectKeyInfo.PressedValid){
            BUILD_SENSE(0x02,0x3A,0x00,0x15);        /* MEDIUM NOT PRESENT */
            ProcessEjectRequest();
            return;
        }
        if(Iram_Rw_read_mode == eCDDAMode)  //  audio track return fail.
        {
            if(St_cdrom_audio == 1)//pure Audio
            {
                Svr_pause_reqire = TRUE;
                BUILD_SENSE(0x05,0x64,0x00,0x7c);    /* ILLEGAL MODE FOR THIS TRACK */
                ReadCDExit();
                return;
            }
            else
            {
                Iram_Rw_read_mode = eUnknownMode;
            }
        }
        /*
            Added the time limit (read_time_limit seconds)to prevent the host send the reset command
            ReadGiveUpCounter, to check if the read command fail is continue over 2 times
            And the time limit will reduce to (read_time_limit-read_reduce_time)/1000 seconds.
            Once the following read command success, the time limit will become read_time_limit again.
        */
#if ((BY_PASS_C2Error_For_VCD) && (CEProduct))
        //if (ReadTimer(&read_timer)>(read_time_limit-((ReadGiveUpCounter/2)*read_reduce_time)))
        if (ReadTimer(&read_timer) > (TIME_6S/3)) //over 2 Sec 
#else
        if (ReadTimer(&read_timer) > TIME_6S)
#endif
        {
            Svr_pause_reqire = TRUE;            /* mark pause request */
            if (Iram_Rw_read_mode != eUnknownMode){
                if(flg == PASS){
                    BUILD_SENSE(0x03,0x02,0x11,0x02);    /* UNRECOVERED READ ERROR */
                }else{
                    BUILD_SENSE(0x03,0x02,0x00,0x22);    /* NO SEEK COMPLETE */
                }
            }else{
                BUILD_SENSE(0x05,0x64,0x00,0x7f);    /* ILLEGAL MODE FOR THIS TRACK */
            }
#if (BY_PASS_C2Error_For_VCD)
            if (ReadGiveUpCounter < 2)
                ReadGiveUpCounter++;
#endif
            ReadCDExit();
            return;
        }

/* If the current speed is the lowest speed and the down speed condition is reached,
    just return the the no seek complete to the host. */
#if (DelayCDSpeedDown == 1)
    if ((Current_read_sp == MIN_READ_SPEED)&&(SpeedCount <= SpeedDownReady))
    {
            //send_msg5S(SEND_ATAPI_CMD, "Reach the lowest SP");
            send_msg80(SEND_ATAPI_CMD,0x710052);
            BUILD_SENSE(0x03,0x02,0x00,0x23);    /* NO SEEK COMPLETE */
            ReadCDExit();
            return;
    }
#endif

    if(ATA_SRST_rcv == TRUE)//WarnBoot fail in Southbridge is ICH5
    {
        BUILD_SENSE(0x03,0x02,0x00,0x41);
        ProcessEjectRequest();
        return;
    }

#if WITHOUT_SERVO == 0
        flg = SeekToTarget();
        rw_seek_retry++;
        if (flg == FAIL)
        {
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    //* speed down */
        }
        else if((rw_seek_retry>2) && ((Iram_RW_ECC_Status & OVERSHOOTERROR) != OVERSHOOTERROR )
                &&(Iram_RW_ECC_Status != 0x00))  /* avoiding illegal speed-down action */
        {
            rw_seek_retry = 0;
            RecoverDecoderError();// == change reason modfiy the CD speed control for CDSpeed test.
        }
        else
#endif /* WITHOUT_SERVO */
        {

/* for read mix mode disc */
/* mix mode disc seek retry 3 times then set retry_flag*/
            retry_flag = 0;
            if((St_cdrom_mixed == 1 || St_cdi) && (rw_seek_retry > 3))  //St_cdi for bad quality CD-I
                retry_flag = 1;

#if (CHIP_REV <= 0xB2)
            if (READ_FIELD(HAL_GLOBAL_CHIPREV) <= 0xB2)
            {
                if ( (Iram_RW_ECC_Status&EDCERROR) && ((Iram_Rw_read_mode == eMode1) || (Iram_Rw_read_mode == eMode2Form1)) )
                {
                    send_msg54(SEND_HIF_CMD,'C','G',rw_seek_retry,Iram_Rw_read_mode);
                    UseC1CG1 = TRUE;
                }
            }
#endif

/*** seek successfully ***/

/*** buffering start ***/
            a = StartBufferingFromTarget();
            if (a == 0){  /*** no error ***/
                break;
            }
            else if (a == 0x81){
                if(rw_seek_retry > 3){
                    if ( ServoReadSubcode(1,0xFF) == PASS ){
                        if (!TEST_BIT(((BYTE *)PlayerAddrsPtr)[0], 6)){
                            Iram_Cmd_block.bf[9] = 0xf8;
                            Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                            ReadCDDACommand();
                            return;
                        }
                    }
                    SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    //* speed down */
                }
            }
        }

#if (HORROR_DISC_IMPROVEMENT == 1)
        //if(rw_seek_retry>=1)//from 720
        {
            (void)ServoChgRFParameter();//change RF EQ parameter
             //read_retry_option|=(READ_RETRY_ENABLE|READ_RETRY_TIMER_ENABLE|DFE_RETRY_ENABLE);//TBD:maybe CD can also use DFE retry
            //Sometimes CD-RW reading will become very bad, we need to reset some DFE settings at that time.        
            {        
                BYTE i;
                
                WRITE_FIELD(HAL_DFE_BETAMSEN,1);
                for (i=0; i<25; i++) //a small delay
                WRITE_FIELD(HAL_DFE_BETAMSEN,0);            
            }
            //Sometimes CD-RW reading will become very bad, we need to reset some DFE settings at that time.   
        }
         
#endif /* HORROR_DISC_IMPROVEMENT */
/*** error ***/

        if (((Iram_RW_ECC_Status & MSBERROR) == MSBERROR ) && (rw_seek_retry > 3)){      /* time out */
            if ( ServoReadSubcode(1,0xFF) == PASS ){
                if (!TEST_BIT(((BYTE *)PlayerAddrsPtr)[0], 6)){
                    Iram_Cmd_block.bf[9] = 0xf8;
                    Rw_seek_blk.lba = HostXfer.StartBlock.lba;
                    ReadCDDACommand();
                    return;
                }
            }
        }
        if (((Iram_RW_ECC_Status & EDCERROR) == EDCERROR ) && (rw_seek_retry == 10)){
            Iram_Rw_read_mode = eUnknownMode;
        }
        if ((rd_method == WR_METHOD_VP) && ((Iram_RW_ECC_Status & MODERROR) == MODERROR)     /* time out */
            && (rw_seek_retry == 10)){
            BUILD_SENSE(0x05,0x64,0x00,0x7d);            /* ILLEGAL MODE FOR THIS TRACK */
            ReadCDExit();
            return;
        }

#if (BY_PASS_C2Error_For_VCD)
#if(CEProduct == 1)
        if (ReadTimer(&read_timer) > (TIME_6S/4)) //over 1.5 Sec
#else
        if (ReadTimer(&read_timer) > (TIME_6S/2)) //over 1.5 Sec
#endif
        {
            if ((Iram_Rw_read_mode != eMode2Form2) &&(St_cdrom_video) && (HostXfer.StartBlock.lba > 0x4500L))
            //VCD, address is over than 1 min, and disc is no good.
            {
                ULONG  track_1_stop_lba;
                TimeS  track_1_stop_time;

                track_1_stop_time  = pdbGetStopTime(1);
                track_1_stop_lba   = MSF2LBA_Plus(&track_1_stop_time);
                send_msg5SValue(1,"track1 stop", 4, track_1_stop_lba);
                //The track 2 should be M2F2 for VCD.
                if (HostXfer.StartBlock.lba > track_1_stop_lba)
                {
                    send_msg80(SEND_HIF_CMD,0x71003C);
                    Iram_Rw_read_mode = eMode2Form2;
                    //Set block mode back
                    SCSDRAM[DECBLKMASK_ADDR + DataBuffer.CurHostPtr] = Iram_Rw_read_mode;
                }
            }

            if ((Iram_Rw_read_mode == eMode2Form2) || (Iram_Rw_read_mode == eYellowMode2))
            {
                //send_msg5SValue(1,"Disc Ptr", 2, DataBuffer.CurDiscPtr);
                //send_msg5SValue(1,"Host Ptr", 2, DataBuffer.CurHostPtr);
                //send_msg5SValue(1,"PreHost", 2, PreCurHostPtr);
                //send_msg5SValue(1,"BlockLeft", 4, HostXfer.BlocksLeft);
                send_msg54(SEND_HIF_CMD,'b','p','2',(BYTE)HostXfer.BlocksLeft);
                if (HostXfer.BlocksLeft != 0)
                {
                    ZeroFillDRAM(DataBuffer.CurHostPtr, HostXfer.BlocksLeft);
                    CdTransferHostLeftData();
                    ReadCDExit();
                    return;
                }
            }
        }
#endif

#if ((BY_PASS_C2Error_For_VCD) && (CEProduct))
        //if (ReadTimer(&read_timer)>(read_time_limit-((ReadGiveUpCounter/2)*read_reduce_time)))
        if (ReadTimer(&read_timer) > (TIME_6S/3)) //over 2 Sec
    #else
        if (ReadTimer(&read_timer) > TIME_6S)
    #endif
        {
            Svr_pause_reqire = TRUE;            /* mark pause request */
            if (Iram_Rw_read_mode != eUnknownMode){
                if(flg == PASS){
                    BUILD_SENSE(0x03,0x11,0x00,0x01);    /* UNRECOVERED READ ERROR */
                }else{
                    BUILD_SENSE(0x03,0x02,0x00,0x1f);    /* NO SEEK COMPLETE */
                }
            }else{
                BUILD_SENSE(0x05,0x64,0x00,0x7e);    /* ILLEGAL MODE FOR THIS TRACK */
            }
#if (BY_PASS_C2Error_For_VCD)
            if (ReadGiveUpCounter < 2)
                ReadGiveUpCounter++;
#endif
            ReadCDExit();
            return;
        }
/* Modify for play ability */

#if (CDPlayability == 1)
        /*Read Miss, Return Zero to Host to avoid screen freeze*/
        if((St_cdrom_video) && (Iram_Cmd_block.bf[0] == MMC_READ_CD)&&(rw_seek_retry > 2) && (Iram_RW_ECC_Status & OVERSHOOTERROR))
        {
            send_msg54(SEND_HIF_CMD,'b','p','1',(BYTE)HostXfer.BlocksLeft);
            if (HostXfer.BlocksLeft != 0)
            {
                ZeroFillDRAM(DataBuffer.CurHostPtr, HostXfer.BlocksLeft);
                CdTransferHostLeftData();
            }
#if (BY_PASS_C2Error_For_VCD)
                if (ReadGiveUpCounter < 2)
                    ReadGiveUpCounter++;
#endif
                ReadCDExit();
                return;
        }
#endif
        if ((Iram_RW_ECC_Status & OVERSHOOTERROR) == OVERSHOOTERROR )
        {
            if ((Rw_seek_blk.lba > 10) && ((HostXfer.StartBlock.lba - Rw_seek_blk.lba) < 300)) {
                MINUS_LBA_HEADER_1_FRAME(Rw_seek_blk.lba , 10); /*seek to 2 more block ahead */
            }
        }
/* Prevent the seek operation too many times while the EDCN occured */
#if ((CEProduct == 1)&&(DelayCDSpeedDown == 0))
         if (St_cdrom_video){
             if(Iram_RW_ECC_Status & EDCERROR )
                SeekCounterForEDCN++;
             if ((Iram_RW_ECC_Status & EDCERROR )&&((SeekCounterForEDCN%2) == 0)&&(Iram_Cmd_block.bf[0]==MMC_READ_CD)){
                        HostXfer.StartBlock.lba ++;
                        SeekCounterForEDCN = 0;
                        Rw_seek_blk.lba = HostXfer.StartBlock.lba;  /* initial seek block */
             }
         }
#endif

    } /* end seek loop */

/*** first block is successfully loaded into fifo xfer this block to host now ***/
    SendHostBlocks();                           /* xfr blk to host */

#if (BY_PASS_C2Error_For_VCD)
    if (ReadGiveUpCounter >0)
        ReadGiveUpCounter--;
#endif

start_read:
    Iram_Knl_timer_counter = 40;    /* Wait about 800 ms */
    a = GET_INT_ENABLE_MASK();     /* Improved sometime seek(Access time) test fail */
    DISABLE_INTERRUPT();
    Knl_timeout = FALSE;            /* Assume no time out */
    Rw_rd_monitor = TRUE;
    SET_INT_ENABLE_MASK(a);
    rw_seek_retry = 0x00;

/*** readloop ***/
    ExitPer256ms = ExitCount = 0;
    while (1)
    {
        //ExitProcess();
        ExitPer256ms = ReadTimer(&read_timer)>>8;
        if(ReadTimer(&read_timer) > TIME_6S)
        {
            Iram_Send_Giveup_F = TRUE;
            BUILD_SENSE(0x03,0x02,0x00,0x20);
            ProcessEjectRequest();
            return;
        }
        else if(ExitCount < ExitPer256ms)//ExitProcess Pre 256msec
        {
            ExitCount = ExitPer256ms;
            ExitProcess();
        }

        if(ATA_SRST_rcv == TRUE)//WarnBoot fail in Southbridge is ICH5
        {
            BUILD_SENSE(0x03,0x02,0x00,0x40);
            ProcessEjectRequest();
            return;
        }

        if ( St_send_blkdata == TRUE)
        {
            if (Rw_dec_stop == FALSE)
            continue;
        }
        else if (St_cmd_inproc == FALSE)
        {
            ReadCDExit();
        #if(BACKGROUND_READ_AHEAD == 1)
            if(Rw_dec_stop) {
                START_BACKGROUND_BUFFER = TRUE;
            }
        #endif
            return;
        }
        else
        {
            if(EjectKeyInfo.PressedValid){
                BUILD_SENSE(0x02,0x3A,0x00,0x16);    /* MEDIUM NOT PRESENT */
                ProcessEjectRequest();
                return;
            }

            if(Rw_init_buffer)
            {
                if ( Knl_timeout == TRUE )
                {
                    StopDecoder();     /* stop decoder */
                }
            }

            if(Iram_reload == TRUE)
                continue;                       /* readloop, if dec is running */

            if ( Rw_dec_stop == FALSE)
                continue;                       /* readloop, if dec is running */

            /* Entry point of continued in reading last decoder block
                where fifo is empty and decoder stopped */
            /* feedup_buf: */
            if (DataBuffer.SectorCount == 0)
            {
                HostXfer.StartBlock.lba = DataBuffer.CacheEndLBA + 1L;
                /* reload start_block with dec_block and start seek again */
                Rw_rd_monitor = FALSE;          /* mark monitor not read */
                HostXfer.BlockCount.Long = 0;
                goto start_seek;
            }
        }
        /* chk_paralle_buffering: */
        /* check if buffer less than MAX-5, if it is do parallel buffer */
        { /* Local */
            WLOW(lba) =  DataBuffer.SectorCount;
            if (St_send_blkdata == TRUE)
            {
                Sa = Rw_get_remain_xfer_blk();
                if ((Sa = Iram_Buf_hxfer_blk - Sa) != 0)
                {
                    WLOW(lba) -= Sa;
                }
            }
            if (WLOW(lba) >= ((RING_END_PAGE + 1)/3))
                continue;
        }

        /* reload start_block with dec_block and start seek again */
        if ( DataBuffer.CacheEndLBA <= Maxlba ){
            Rw_seek_blk.lba = DataBuffer.CacheEndLBA;
            AdjustSeekOffset();                  /* execute adjust rw_seek_blk operation */

#if (CDPlayability == 1)
    /* If buffer is almost empty, by pass C2 error for keeping data in buffer*/
            if ((DataBuffer.SectorCount < 26) &&(St_cdrom_video)&&(Iram_Cmd_block.bf[0] == MMC_READ_CD))
                CDBufferAlmostEmpty = TRUE;
#endif

            if (SeekToTarget()==PASS)        /* execute seek operation */
            {
                InitBuffering();               /* setup decoder for parallel buffering */
                Iram_Knl_timer_counter = 40;    /* Wait about 800 ms */
                Knl_timeout = FALSE;            /* Assume no time out */
            }
        }
    }                                           /* end readloop */
}

/*********************************************************************
* ReadCDDACommand
*
* Read cdda command handler
*
* Returns:      void
* Params:       none
*********************************************************************/
static void ReadCDDACommand(void)
{
    HostXfer.BlockCount.Long = HostXfer.BlocksLeft;

#if (AUDIO_PLAYBACK == 1)
    SetAudioChannelMode(0);
#endif

    if(SpeedMD==Set_sp_f)
    {
        SpeedMD=Special_f;
    }
#if(CEProduct == 0)
    // bypass speed up/down when CDSpeed test.
    if((SpeedMD == AutoMode) && (HostXfer.BlockCount.Long == 0x19))
    {
        SpeedMD = Special_for_CDSpeed_f;
    }
    else if((SpeedMD == Special_for_CDSpeed_f)&&(HostXfer.BlockCount.Long != 0x19))
    {
        RandomRead = TRUE; //restart the automode.
        SpeedMD = AutoMode;
    }
#endif
    ReadCDDAData();
}

/*********************************************************************
* ReadCDExit
*
* Exit read command
*
* Returns:      void
* Params:       none
*********************************************************************/
void ReadCDExit(void)
{
    Rw_C2PO_data = FALSE;           /* mark no read c2po data */
    Rw_subc_data = FALSE;
    Read_subc_only = FALSE;
    Rw_rdcd_cmd = FALSE;
#if (SUN_FUNC_TEST == 1)
    RdC2POData_296 = FALSE;
    RdC2PODataTaggle = FALSE;    
#endif


#if (CDPlayability == 1)
    CDBufferAlmostEmpty = FALSE;
#endif

    if ( fReadVerifyCommand == TRUE ) {
        fReadVerifyCommand = FALSE;
    }

    ENABLE_INTERRUPT();                 /* Enable all interrupt */

#if (BY_PASS_C2Error_For_VCD)
    C2ErrorCount = 0;
#endif

#if (LED_NUM == 1)
    ReadyLED(LED_OFF);
#endif /* LED_NUM */

#if (LED_NUM == 2)
    ReadyLED(LED_AXferOn);
#endif /* LED_NUM == 2 */
#if (AUTO_STATU_CPLT==1)
        if ((Read_Zero_Length==TRUE)||(Iram_Sense.key !=0))
        {
            AutoCommandComplete();
            Read_Zero_Length = FALSE;
        }
#else
    AutoCommandComplete();
#endif
    Rw_rd_monitor = FALSE;
    /* If Current Target Position is LEAD-IN, Pause on HOME Position */
    if ( Rw_seek_blk.lba < 0 )
    {
        Rw_seek_blk.lba = 0L;           /* Seek to 00:02:00 */
        SeekToTarget();

        Svr_pause_reqire = TRUE;        /* Pause Request */
    }
}

/*******************************************************************************
* ProcessEjectRequest
*
* Handles key press
*
* Returns:      void
* Params:       none
*******************************************************************************/
void ProcessEjectRequest(void)
{
    StopDecoder();
    if(St_send_blkdata == TRUE)
    {
        AbortHostTransfer();
        St_send_blkdata = FALSE;
        fKernelSendCompletionStatus = FALSE;
    }
    SpeedCount = 0; //clear speed count when eject press
    DataBuffer.SectorCount = 0;
    St_cmd_inproc = FALSE;
    HostXfer.BlocksLeft = 0;
    ReadCDExit();
}

/********************************************************
*
*   BGCheckCommandAvalible()
*
********************************************************/
void BGCheckCommandAvalible(void)
{
    if ( St_become_ready )
    {
        CheckCommandAvail();
    }
    else if ((Iram_Svr_player_mode == EXECUTING_TRAY_OUT_MODE) || (Iram_Svr_player_mode == EXECUTING_SINGLE_SPEED_MODE))
    {
        switch(Iram_Cmd_block.bf[0]){
            case SPC_TEST_UNIT_READY:
            case SPC_REQUEST_SENSE:
            case SPC_PREVENT_ALLOW_MEDIUM_REMOVAL:
                CheckCommandAvail();
        }
    }

    if (Iram_reload)
    {
        Iram_reload = FALSE;
        if ( DataBuffer.CacheEndLBA <= Maxlba ){
        /* reload start_block with dec_block and start seek again */
            Rw_seek_blk.lba = DataBuffer.CacheEndLBA;

            if (SeekToTarget()==PASS)        /* execute seek operation */
            {
                InitBuffering();               /* setup decoder for parallel buffering */
            }else{
                Svr_pause_reqire = TRUE;        /* mark pause request */
            }
        }
    }
}

/*********************************************************************
*   Function :
*       SetReadAheadCommand()
*
*   Description :
*       This is the command handler for the Set Read Ahead command
*
*********************************************************************/
void SetReadAheadCommand( void )
{
    BYTE    trkState, TrNo;

    /*---------------------------------------------------------------
        Check Blank Disc
    ---------------------------------------------------------------*/
    if ( St_blank_disc == TRUE ) {
        BUILD_SENSE(0x05,0x21,0x00,0x7f);
        AutoCommandComplete();
        return;
    }

    /*---------------------------------------------------------------
        Parse the command block
    ---------------------------------------------------------------*/
    Iram_Buf_trg_blk.lba   = PacketCommandToLBALEN4( 2 );
    Iram_Buf_ahead_blk.lba = PacketCommandToLBALEN4( 6 );

    /*---------------------------------------------------------------
        HOST Request LBA relation check
    ---------------------------------------------------------------*/
    if ( Iram_Buf_trg_blk.lba > Iram_Buf_ahead_blk.lba ) {
        BUILD_SENSE(0x05,0x24,0x00,0xed);        /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    /*---------------------------------------------------------------
        HOST Request LBA validity check
    ---------------------------------------------------------------*/
    if ( (Iram_Buf_trg_blk.lba > Maxlba) || (Iram_Buf_ahead_blk.lba > Maxlba) ) {
        /* exceeded leadout */
        BUILD_SENSE(0x05,0x21,0x00,0x65);        /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        return;
    }


    //start== HP CMPLY test TDR840 fail.
    if(pdbDiscLoaded() == BD_DISC)
    {
        BUILD_SENSE(0x05,0x24,0x00,0x41);   /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }
    else if(pdbDiscLoaded() == DVD_DISC)
    {
        if((Iram_Buf_trg_blk.lba > pdbGetDVDMaxLBA()) ||(Iram_Buf_ahead_blk.lba > pdbGetDVDMaxLBA()))
        {
            BUILD_SENSE(0x05,0x21,0x00,0x40);  // LOGICAL BLOCK ADDRESS OUT OF RANGE
            AutoCommandComplete();
            return;
        }
    }
    else
    {
    /*---------------------------------------------------------------
        Check Blank Track (Trigger LBA)
    ---------------------------------------------------------------*/
    TrNo = FindTrackNumber( Iram_Buf_trg_blk.lba );
    if ( TrNo ) {
        trkState = pdbGetTrackState( TrNo );
        if ( trkState == eTRACK_STATE_EMPTY || trkState == eTRACK_STATE_BLANK ) {
            BUILD_SENSE(0x05,0x64,0x00,0x66);    /* ILLEGAL MODE FOR THE TRACK */
            AutoCommandComplete();
            return;
        }
    } else {
        BUILD_SENSE(0x05,0x64,0x00,0x67);        /* ILLEGAL MODE FOR THE TRACK */
        AutoCommandComplete();
        return;
    }

    /*---------------------------------------------------------------
        Check Blank Track (Target LBA)
    ---------------------------------------------------------------*/
    TrNo = FindTrackNumber( Iram_Buf_ahead_blk.lba );
    if ( TrNo ) {
        trkState = pdbGetTrackState( TrNo );
        if ( trkState == eTRACK_STATE_EMPTY || trkState == eTRACK_STATE_BLANK ) {
            BUILD_SENSE(0x05,0x64,0x00,0x68);    /* ILLEGAL MODE FOR THE TRACK */
            AutoCommandComplete();
            return;
        }
    } else {
        BUILD_SENSE(0x05,0x64,0x00,0x69);        /* ILLEGAL MODE FOR THE TRACK */
        AutoCommandComplete();
        return;
    }
    }
    //end==

    /*---------------------------------------------------------------
        Request Command Stauts OK Return
    ---------------------------------------------------------------*/
    if ( Iram_Buf_trg_blk.lba != Iram_Buf_ahead_blk.lba ) {
        Iram_rd_ahead_chk_req = RD_AHEAD_BUF_REQ;
    } else {
        ClearReadAheadSettings();               /* Read Ahead Setting clear */
    }
    AutoCommandComplete();
}


/*******************************************************************************
* VerifyCommand
*
* Description: Verify10 and Verify12 Command Handler
*
* Output:
*       fReadVerifyCommand  = TRUE
*
*       - C12CCR reg. programmed to count all or only uncorrectable C1 and C2
*       - Cache is flushed
*
*******************************************************************************/
void VerifyCommand( void )
{
    fReadVerifyCommand = TRUE;
    CDRead10Command();
}

/*************************************************************************
* CheckForReadAheadBufferingStart
*
* This function is Ckecking "Set Read Ahead" Command operation start.
*
* Returns:      none
* Params:       none
*************************************************************************/
void CheckForReadAheadBufferingStart( void )
{
    long    startLBA, endLBA, reqSize, adjLBA;

    /*-------------------------------------------------------------------------------*/
    /* Get HOST requestted start LBA & end LBA                                       */
    /*-------------------------------------------------------------------------------*/
    switch ( Iram_Cmd_block.bf[0] ) {
        case SBC_READ_10:
        case SBC_READ_12:
            if ( Iram_Cmd_block.bf[0] == SBC_READ_10 )
            {        /* READ (10) Command */
                reqSize = PacketCommandToLBALEN2( 7 );
            }
            else
            {                                        /* READ (12) Command */
                reqSize = PacketCommandToLBALEN3( 7 );      /* !! Byte 6 Ignored */
            }
            startLBA = PacketCommandToLBALEN3( 3 );
            endLBA   = (startLBA + reqSize) - 1;
            break;
        case MMC_READ_CD:
            startLBA = PacketCommandToLBALEN3( 3 );
            reqSize  = PacketCommandToLBALEN3( 6 );
            endLBA   = (startLBA + reqSize) - 1;
            break;
        case MMC_READ_CD_MSF:
            /*** Check for valid MSF in starting and ending address fields ***/
            if ( CheckValidMSF( (BYTE *)&Iram_Cmd_block.bf[3] ) ||
                 CheckValidMSF( (BYTE *)&Iram_Cmd_block.bf[6] ) )
            {
                BUILD_SENSE(0x05,0x24,0x00,0x6a);    /* INVALID FIELD IN CDB */
                return;
            }
            startLBA = MSF2LBA(&Iram_Cmd_block.bf[3]);
            endLBA   = MSF2LBA(&Iram_Cmd_block.bf[6]);
            break;
    }

    /*-------------------------------------------------------------------------------*/
    /* If HOST Read Requested LBA is illegal position, cache & ahead setting reset!! */
    /*                   TriggerLBA                  AheadLBA                        */
    /*                    |                            |                             */
    /* Disc IN |--+----+--+---------+-+------------+-+-+----------+--+---------| OUT */
    /*            |    |<-- Req1 -->| |<-- Req2 -->| |<-- Req3 -->|  |               */
    /*            |<--------------------- Req4 --------------------->|               */
    /* Req1~4 : All illegal request !!!                                              */
    /*-------------------------------------------------------------------------------*/
    if ( ((startLBA < Iram_Buf_trg_blk.lba)   && (endLBA > Iram_Buf_trg_blk.lba))   ||      /* Req 1 */
         ((startLBA > Iram_Buf_trg_blk.lba)   && (endLBA < Iram_Buf_ahead_blk.lba)) ||      /* Req 2 */
         ((startLBA < Iram_Buf_ahead_blk.lba) && (endLBA > Iram_Buf_ahead_blk.lba)) ||      /* Req 3 */
         ((startLBA < Iram_Buf_trg_blk.lba)   && (endLBA > Iram_Buf_ahead_blk.lba)) )      /* Req 4 */
    {
        /* cache DATA clear */
        StopDecoder();
        ClearDataBuffer();
        /* Read Ahead Setting clear */
        ClearReadAheadSettings();
        return;
    }

    /*-------------------------------------------------------------------------------*/
    /* If HOST Read Requested LBA hit ahead area, check ahead cacheing done          */
    /*-------------------------------------------------------------------------------*/
    if ( startLBA >= Iram_Buf_ahead_blk.lba )
    {
        if ( Iram_rd_ahead_chk_req == RD_AHEAD_BUF_DONE )
        {
            /* Buffer HOST Pointer adjustment */
            adjLBA = startLBA - Iram_Buf_ahead_blk.lba;
            /* set dummy Tranfer done */
            DataBuffer.CurHostPtr     = Iram_Buf_ahead_bptr + adjLBA;
            DataBuffer.CacheStartLBA = (Iram_Buf_ahead_blk.lba - 1) + adjLBA;
        }
        /* Read Ahead Setting clear */
        ClearReadAheadSettings();
        return;
    }

    /* Read Ahead Buffering Request */
    if ( Iram_rd_ahead_chk_req == RD_AHEAD_BUF_REQ )
    {
        Iram_rd_ahead_chk_req = RD_AHEAD_BUF_CHK;
    }
}

/*********************************************************************
* CheckForFastCache()
*
* Check Fast Cache Check for Read(10)
*
* Returns:  none
* Params:   none
*********************************************************************/
vBOOL CheckForFastCache(void)
{
    long start_lba;
    long h2go;
    long temp;

    #if(BACKGROUND_READ_AHEAD == 1)
        START_BACKGROUND_BUFFER = FALSE;
    #endif

    if ( St_become_ready == TRUE )
        return(FALSE);

    if ( st_disc_change == TRUE )
        return(FALSE);
    if (St_dma_mode == FALSE)
    {
        if ( (Iram_Xfr_allow < 2048) && (Iram_Xfr_allow > 0) )
            return(FALSE);
    }

    Iram_St_fast_cache_hit = FALSE;

#if BD_ENABLE && BDWT_FAST_CACHE
    if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
    {
        if ((St_cmd_inproc == TRUE) || (DataBuffer.SectorCount == 0))
            return(FALSE);
    }
    else
    {
        if (St_cmd_inproc == TRUE)  //check cache usage in BdwtSearchCache()
            return(FALSE);
    }
#else
    if ((St_cmd_inproc == TRUE) || (DataBuffer.SectorCount == 0))
        return(FALSE);
#endif

    if(pdbDiscLoaded()!=CD_DISC)
    {
        if ( (Iram_Cmd_block.bf[0] == SBC_READ_10) || (Iram_Cmd_block.bf[0] == SBC_READ_12))
        {
            switch(pdbDiscLoaded())
            {
    #if (HD_READ)
                case HD_DISC:
    #endif
                case DVD_DISC:

    #if (DVD_READ_AHEAD_ENABLED == FALSE )
                    return (FALSE);
    #endif

                    Iram_St_fast_cache_hit = (vBYTE) dvdFastCacheTransfer();
                    break;

    #if (BD_READ)
                case BD_DISC:
                    /*--------------------------------------------------------
                        If BD read ahead is disabled never check the cache
                    ---------------------------------------------------------*/
        #if (BD_READ_AHEAD_ENABLED == FALSE )
                    return (FALSE);
        #endif

                    Iram_St_fast_cache_hit = (vBYTE) bdFastCacheTransfer();
                    break;
    #endif
                default:
                    return (FALSE);
            }

            if( Iram_St_fast_cache_hit == TRUE )
                return TRUE;
            else
                return FALSE;

        }
        else
        {
            return(FALSE);
        }
    }
    start_lba = PacketCommandToLBALEN4(2);

    /* Fast cache command is effective for Read 10/12 command */
    switch (Iram_Cmd_block.bf[0])
    {
        case SBC_READ_10:
        case SBC_VERIFY_10:
            h2go =  PacketCommandToLBALEN2(7);
            Rw_rdcd_cmd = FALSE;
            Rw_subc_data = FALSE;
            Read_subc_only = FALSE;
            Iram_Rd_subc_flag = FALSE;
            //start== HP CMPLY test TCDR704 fail.
            if(Maxlba<(start_lba +h2go))
                return(FALSE);
            //end==
            break;
        case SBC_READ_12:
            h2go =  PacketCommandToLBALEN3(7);
            Rw_rdcd_cmd = FALSE;
            Rw_subc_data = FALSE;
            Read_subc_only = FALSE;
            Iram_Rd_subc_flag = FALSE;
            //start== HP CMPLY test TCDR704 fail.
            if(Maxlba<(start_lba +h2go))
                return(FALSE);
            //end==
            break;
        case MMC_READ_CD:
            /* for Nero disc quality test */
            if (Iram_Page_Size_3K_Flag == 1){
#if (SUN_FUNC_TEST == 1)
                if (((Iram_Cmd_block.bf[9] & 0x6) == 0x02) || ((Iram_Cmd_block.bf[9] & 0x6) == 0x04))
#else
                if ((Iram_Cmd_block.bf[9] & 0x6) == 0x02)
#endif
                {
                    Rw_C2PO_data = TRUE;
#if (SUN_FUNC_TEST == 1)
                     if((Iram_Cmd_block.bf[9] & 0x6) == 0x04)
                         RdC2POData_296 = TRUE;
#endif
                }
            }
            else
            {
            if((Iram_Cmd_block.bf[9] & 0x6) != 0)
            {
                    //if request C2PO, do not use fast cache
                    Rw_C2PO_data = FALSE;
                return(FALSE);
            }
            }

            if (Iram_Cmd_block.bf[10] != 0)
            {
                // If request Subcode, do not use fast cache
                return(FALSE);
            }
/**** check track size***/
        if(St_cdrom_audio)
        {
            ULONG TrkEndLBA;
            TimeS TrkStopMSF;
            BYTE trknum;
            trknum = FindTrackNumber(start_lba);
            TrkStopMSF = pdbGetStopTime(trknum);
            TrkEndLBA = MSF2LBA(&TrkStopMSF);
            if(TrkEndLBA<start_lba)
            {
                    return(FALSE);
            }
        }

            Rw_rdcd_cmd = TRUE;
            Rw_subc_data = FALSE;
            Read_subc_only = FALSE;
            h2go =  PacketCommandToLBALEN3(6);
            break;
        default:
            return(FALSE);
    }

    Iram_Knl_save_counter = STANDBYCNT;

    /* Check if random read for CD. */
    /* If the require data of current read cmd is overlaping pre-read cmd, */
    /* it isn't RandomRead because of the pre-read cmd required data is still in the buffer possibly. */
    /* Example: The previous and current read cmd overlap a lba when WinXP copys VCD files. */
    temp = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long;    // current cmd start lba of sequential read = pre-cmd start lba + pre-required length

    if((start_lba > temp) || (start_lba <= DataBuffer.CacheStartLBA))
        RandomRead = TRUE;
    else
        RandomRead = FALSE;
    if ((h2go == 0x00) || (DataBuffer.SectorCount < h2go ))
            return(FALSE);

    HostXfer.BlocksLeft = (ULONG)h2go;

    /* check requested block's track writing method */
    /* In case of full hit , fast cache is used */
    if( DataBuffer.CacheEndLBA >= start_lba )
    {
        if(start_lba > DataBuffer.CacheStartLBA)
        {
            if( (HostXfer.BlocksLeft + start_lba - 1) <= DataBuffer.CacheEndLBA )
            {
                HostXfer.BlockCount.Long = HostXfer.BlocksLeft;
                HostXfer.StartBlock.lba = start_lba;

                temp = HostXfer.StartBlock.lba - DataBuffer.CacheStartLBA - 1;

                DataBuffer.CurHostPtr += (USHORT)temp;
                if ( DataBuffer.CurHostPtr > RING_END_PAGE)
                {
                    DataBuffer.CurHostPtr %=BUF_TOTAL_BLK_COUNT;
                }
                DataBuffer.SectorCount -= (USHORT)temp;

                DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
                MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA, 1);

                if(DataBuffer.SectorCount < HostXfer.BlocksLeft)
                    return(FALSE);

                Iram_St_fast_cache_hit = TRUE;
                BUILD_SENSE(0x00,0x00,0x00,0x0d);        /* NO ADDITIONAL SENSE INFO */

                Iram_Cmd_block.bf[9] &= 0xf8;   /*for Nero disc quality test */
                SendHostBlocks();           /* xfr next aval blocks */

            #if(BACKGROUND_READ_AHEAD == 1)
                if(Iram_rd_ahead_chk_req == RD_AHEAD_BUF_NOP)
                {
                    if(Rw_dec_stop)
                    {
                        START_BACKGROUND_BUFFER = TRUE;
                    }
                }
            #endif

                return(TRUE);
            }
        }
    }
    return(FALSE);
}

void CdTransferHostLeftData(void)
{
    HostXfer.BlockCount.Long = 0;
    DataBuffer.SectorCount = HostXfer.BlocksLeft;

    SendHostBlocks();

    while (1)
    {
        if (St_cmd_inproc == FALSE)
            break;

        ExitProcess();
    }
}
