/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   gp_mdsns.c
*
* DESCRIPTION
* This file contains command handler for information commands
*
*   $Revision: 152 $
*   $Date: 11/03/01 6:14p $
*
*
************************ SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\comMath.h"
#include ".\common\system.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDB.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_knl.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\al\reg_intctrl.h"

extern USHORT GetTransferSpeed(BYTE spd);
#if (AUDIO_PLAYBACK == 1)
extern void SetAudioVolume(BYTE LChVolume,BYTE RChVolume);
#endif

static void AdjustPage2A(ULONG *dest);
static void CopyDefaultPageDataToSDRAM(ULONG *dest, BYTE pageCode, BYTE num);
static void CopyChangeablePageDataToSDRAM(ULONG *dest,
                                    BYTE pageCode, BYTE num);
static BYTE const *GetDefaultPointer(BYTE pageCode);
static BYTE CheckPageData(BYTE pageLength, BYTE pageCode, BYTE index);
void CheckSelectedSpeed(BYTE md, USHORT cd_Speed );
void copy_mode5_data_to_sram(void);

#if (AUDIO_PLAYBACK == 1)
static void CheckVolume(BYTE Old_left, BYTE Old_right);
#endif

static void setTimeOut( void );
BYTE SelectCDReadSpeed(BYTE);
BYTE SelectDVDReadSpeed(BYTE);
#if(ENABLE_HDDVD)
static BYTE SelectHDReadSpeed(BYTE);
#endif

extern struct dvdDiscS     volatile dvdDisc;

/****************************************************************************
*     Mode Sense/Mode Select information tables
*****************************************************************************/

BYTE const Mode_sense_def_tbl[] = {
    0x00,0x24,0x00,0x00,0x00,0x00,0x00,0x00                         /* 0 ~ 7 */
};


BYTE const mode_sense_def_1[] = {
    0x01,0x0A,0x00,0x20,0x00,0x00,0x00,0x00,                        /* 0 ~ 7 */
    0x00,0x00,0x00,0x00                                             /* 8 ~11 */
};


BYTE const mode_sense_def_5[] =  {
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /*  0 ~ 7   */  /* byte2 bit6 BUFE=1 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /*  8 ~  16 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /*  17 ~ 23 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /*  24 ~ 31 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /*  32 ~ 39 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /*  40 ~ 47 */
    0x00,0x00,0x00,0x00                                             /*  48 ~ 51 */
};

BYTE const mode_sense_def_8[] = {
    0x08,0x0A,0x04,0x00,0x00,0x00,0x00,0x00,                        /* 0 ~ 7 */
    0x00,0x00,0x00,0x00                                             /* 8 ~ 11 */
};


BYTE const mode_sense_def_d[] = {
    0x0D,0x06,0x00,0x00,0x00,0x3C,0x00,0x4B                         /* 0 ~ 7 */
};


BYTE const mode_sense_def_e[] = {
    0x0E,0x0E,0x04,0x00,0x00,0x00,0x00,0x4B,                        /* 0 ~ 7 */
    0x01,0xFF,0x02,0xFF,0x00,0x00,0x00,0x00                         /* 8 ~ 15 */
};

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
//start ==Get event status notification fail of Command set
#if (Customer_U3 == 1)||(Customer_Dune_HD == 1)//[V08] Bios 2min HDI
BYTE const mode_sense_def_1a[] = {
    0x1A, 0x0A, 0x00, 0x03, 0x00, 0x00, 0x04, 0xB0,                 /* 0 ~  7 */
    0x00, 0x00, 0x04, 0xB0                                          /* 8 ~ 11 */
};
#else
BYTE const mode_sense_def_1a[] = {
    0x1A, 0x0A, 0x00, 0x03, 0x00, 0x00, 0x0B, 0xB8,                 /* 0 ~  7 */
    0x00, 0x00, 0x0B, 0xB8                                          /* 8 ~ 11 */
};
#endif
//[S07] 6000(1770) 1200(04B0) 3000(0BB8)
//end ==Get event status notification fail of Command set
#else
#if (CEProduct == 1) //sun692,sun815
#define Idle_Cnt 600             //600  /* f:100ms, 60 seconds */
#define Standby_Cnt 6000        //3000  /* f:100ms, 600 seconds */
#else
#define Idle_Cnt    600         //600  /* f:100ms, 60 seconds */
#define Standby_Cnt 3000       //3000  /* f:100ms, 300 seconds */
#endif
BYTE const mode_sense_def_1a[] = {
    0x1A, 0x0A, 0x00, 0x03,
    0x00, 0x00, (BYTE)((Idle_Cnt >> 8) & 0xFF), (BYTE)(Idle_Cnt & 0xFF) ,                 /* 4 ~  7 */
    0x00, 0x00, (BYTE)((Standby_Cnt >> 8) & 0xFF), (BYTE)(Standby_Cnt & 0xFF)             /* 8 ~ 11 */
};
#endif

//start== Page Code 0x1D Page Length 0x0A in 8090Spec V7
BYTE const mode_sense_def_1d[] = {
    0x1D, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,                 /* 0 ~  7 */
    0x04, 0xB0, 0x00, 0x00                                                      /* 8 ~ 11 */
};
//end==

#if (DVD_RAM_READ == 1)
#define BYTE_2 0x3F    /* Support DVD-RAM Read */
#else
#define BYTE_2 0x1F
#endif

#if (SUPPORT_DVD_WRITE_ABILITY == P_SL_DISC)
#define MASKMINUSWRITE   0xEF
#else
#define MASKMINUSWRITE   0xFF
#endif

#define MASKRAMWRITE    0xDF

#define BYTE_3 0x00    /* DVD ROM */
#define BYTE_4 0x75    /* No Buffer underrun */

BYTE const mode_sense_def_2a[] = {
    0x2A,0x1C,BYTE_2,BYTE_3,BYTE_4,                                       /*  0 ~ 4  */
    0x77,                                                           /*  5      */ /*Support C2PO*/
    0x29,0x23,                                                      /*  6 ~  7 */
    (BYTE)((SHORT)((float)(MAX_READ_SPEED&0x7f)*176.4f)/256),       /*  8      */
    (BYTE)((SHORT)((float)(MAX_READ_SPEED&0x7f)*176.4f+0.5f)%256),  /*  9      */
    0x00, 0xFF,                                                     /* 10 ~ 11 */
    (BYTE)(MDSN_CACHE_SIZE/256),                                    /* 12      */
    (BYTE)(MDSN_CACHE_SIZE%256),                                    /* 13      */
    (BYTE)((SHORT)((float)(MAX_READ_SPEED&0x7f)*176.4f)/256),       /* 14      */
    (BYTE)((SHORT)((float)(MAX_READ_SPEED&0x7f)*176.4f+0.5f)%256),  /* 15      */
    0x00,0x10,                                                      /* 16 ~ 17 */
    0x00, 0x00, 0x00, 0x00,                                         /* 18 - 21 */
    0x00,0x01,0x00,0x00,                                            /* 22 ~ 25 */
    0x00,0x00,                                                      /* 26 ~ 27 */
    0x00,0x00                                                       /* 28, 29 */
};

#if (EN_LS == 1)
BYTE const mode_sense_def_31[] = {
    0x31,               // Drive labeling parameters
    42,                 // page length
    2,                  // Version
    0,0x6B,             // Drive ID
    0x28,               // 0 0 nrmsp !rvspn FSum !FFES SpTrk 0
    0x18,       		// 0 0 0 HstTS DrvTS !MTPrt 0 0
    3,                  // 0 0 0 0 TSMod=0 TrkMd=0 Norm=1 Sum=1
    0,                  // reserved
    5,                  // max read laser power (mW [5])
    45,                 // default write laser power (mW)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    // write laser power (mW [32])
    0,0xA5,0x5D,        // mark spacing (42330nm [600DPI = 42333])
    0,0xA5,0x5D,        // track spacing (pitch) (42369nm [600TPI = 42333])
    0x03,0x0D,0x40,     // spindle speed (um/s [200000])
    0x5B,0xCC,          // starting track radius (um [23500])
    0x03,0x0D,0x40,     // minimum spindle speed (um/s [200000])
    0x12,0x4F,0x80,     // maximum spindle speed (um/s [1200000])
    0,                  // reserved Iseek time extension)
    40,                 // single track seek time (ms [40 @.4m/s@ID = 86 spokes])
    0x01,0xF4,          // max seek time (ms [500])
    65,                 // maximum IR laser power (mW [60])
    0,0,                // reserved (Red, Blue laser max power)
    0,                  // reserved
    0x5B,0x68,          // IR (um [23400])
    0xE5,0x4C,          // OR (um [58700])
    0,0,                // reserved (spot size um)
    0,0                 // reserved (position/focus offset)
};
BYTE const mode_sense_def_32[] = {
    0x32,               // Media labeling parameters
    68,                 // page length
    1,                  // Version
    0,                  // flags (LSMed|MSide|IData|OData)
    0,0,                // spoke count (400)
    0,0,                // track shift count (400/rev)
    0,0,0,0,0,0,0,0,    // Outer Ring Media ID : 15 bytes
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,    // Inner Ring Media ID: block 1 (15 bytes)
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,    // Inner Data: block 2 (15 bytes)
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,    // Inner Data: block 3 (16 bytes)
    0,0,0,0,0,0,0,0,
    0,                  // reserved
};
#endif // End of (EN_LS == 1)

BYTE const mode_sense_changeable_1[] = {
    0x01,0x0A,0x01,0xFF,0x00,0x00,0x00,0x00,                        /* 0 ~ 7 */
    0x00,0x00,0x00,0x00                                             /* 8 ~11 */
};

BYTE const mode_sense_changeable_5[] = {
    0x05,0x32,0x7F,0xff,0x0f,0xFF,0x00,0x3f,        /*  0 ~ 7 */
    0x3f,0x00,0xff,0xff,0xff,0xff,0xff,0xff,                        /*  8 ~  15 */
    0x80,0xff,0xff,0xff,0xff,0xff,0xff,0xff,                        /*  16 ~ 23 */
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,                        /*  24 ~ 31 */
    0x80,0xff,0xff,0xff,0xff,0xff,0xff,0xff,                        /*  32 ~ 39 */
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,        /*  40 ~ 47 */
    0xff,0xff,0xff,0xff                                             /*  48 ~ 51 */
};

BYTE const mode_sense_changeable_d[] = {
    0x0D,0x06,0x00,0x0F,0x00,0x00,0x00,0x00                         /* 0 ~ 7 */
};

BYTE const mode_sense_changeable_e[] = {
    0x0E,0x0E,0x02,0x00,0x00,0x00,0x00,0xff,                        /* 0 ~ 7 */ /* change byte 7 to fix WIN98 CD-player auto-play function */
    0x03,0xFF,0x03,0xFF,0x00,0x00,0x00,0x00                         /* 8 ~ 15 */
};
BYTE const mode_sense_changeable_1a[] = {
    0x1A, 0x0A, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff,                 /* 0 ~  7 */
    0xff, 0xff, 0xff, 0xff                                          /* 8 ~ 11 */
};
//start== Page Code 0x1D Page Length 0x0A in 8090Spec V7
BYTE const mode_sense_changeable_1d[] = {
    0x1D, 0x0A, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,                 /* 0 ~  7 */
    0xff, 0xff, 0x00,0x00                                                      /* 8 ~  11*/
};
//end==


BYTE const mode_sense_changeable_2a[] = {
    0x2A,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,                        /* 0 ~ 7 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                        /* 8 ~ 15 */
    0x00,0x00,0x00,0x00,0x00,0x00,                                  /* 16 ~ 21 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00                         /* 22 ~ 29 */
};

BYTE const mode_sense_changeable_08[] = {
    0x08,0x0A,0x05,0x00,0x00,0x00,0x00,0x00,                        /* 0 ~ 7 */
    0x00,0x00,0x00,0x00                                             /* 8 ~ 11 */
};

#if (EN_LS == 1)
BYTE const mode_sense_changeable_31[] = {
    0x31, 42,
    0,          // Version
    0,0,        // Drive ID
    0,          // 0 0 !nrmsp !rvspn !FSum !FFES !SpTrk 0
    0,          // 0 0 0 !HstTS !DrvTS !MTPrt !Multp !Rpeat
    0x0B,       // 0 0 0 0 TSMod TrkMd Norm Sum
    0,          // reserved
    255,        // max read laser power
    255,        // write laser power
    255,255,255,// mark spacing
    255,255,255,// track pitch
    255,255,255,// spindle speed
    0,0,        // starting track radius
    0,0,0,      // minimum spindle speed
    0,0,0,      // maximum spindle speed
    0,          // reserved
    0,          // single track seek time
    0,0,        // max seek time
    0,          // maximum IR laser power
    0,          // reserved
    0,0,        // reserved
    0,0,        // IR
    0,0,        // OR
    0,0,        // reserved
    0,0         // reserved
};

BYTE const mode_sense_changeable_32[] = {
    0x32, 0x44,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
#endif // End of (EN_LS == 1)

USHORT  timeUp_Group1;
USHORT  timeUp;
BYTE    Old_left,Old_right;

/****************************************************************************
* Name: ModeSenseCommand()
* Description:
*   Processes ATAPI Mode Sense command
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
void ModeSenseCommand(void)
{
    BYTE pageCode;
    BYTE mediumType, pageControl;
    ULONG dest, src, temp_dest;
    USHORT Sdest, Ssrc, pageLength;
    //BYTE AppCode;

    /* check allocation length in command BYTE 8, return command completed
       if allocation length equal zero */
    if(((Iram_Cmd_block.bf[8] & 0xfe) | Iram_Cmd_block.bf[7]) == 0)
    {
        AutoCommandComplete();
        return;
    }

    /* check page control (PC) field, return error if PC is saved values (equal 3) */
    if((Iram_Cmd_block.bf[2] & 0xc0) == 0xc0)
    {
        BUILD_SENSE(0x05,0x39,0x00,0x3e);    /*SAVING PARAMETERS NOT SUPPORTED */
        AutoCommandComplete();
        return;
    }

    /* calculate mode data length (BYTE 0,1) for mode parameter header,
       different page codes have different data length */
    pageCode = Iram_Cmd_block.bf[2] & 0x3f;   /* get page code */

    switch(pageCode)
    {
        case 0x00:
            pageLength = 0x00;
            break;
        case 0x01:
            pageLength = MSENSE_PAGE_1_LEN;
            break;
        case 0x08:
            pageLength = MSENSE_PAGE_8_LEN;
            break;
        case 0x0D:
            pageLength = MSENSE_PAGE_D_LEN;
            break;
        case 0x0E:
            pageLength = MSENSE_PAGE_E_LEN;
            break;
        case 0x1A:
            pageLength = MSENSE_PAGE_1A_LEN;
            break;
        case 0x1D:
            pageLength = MSENSE_PAGE_1D_LEN;
            break;
        case 0x2a:
            pageLength = MSENSE_PAGE_2A_LEN;
            break;

#if (EN_LS == 1)
        case 0x31:
            pageLength = MSENSE_PAGE_31_LEN;
            break;

        case 0x32:
            pageLength = MSENSE_PAGE_32_LEN;
            break;
#endif // End of (EN_LS == 1)
        case 0x3F:
            pageLength = MSENSE_PAGE_3F_LEN;
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0x44);    /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;
    }

    /* save Mode Data Length of Mode Parameters Header into dram */
#if (EN_LS == 1)
	Iram_Buf_dram_addr = MSENSE_ADR;
	WriteSDRAM((BYTE)((pageLength + 6)>>8));
	WriteSDRAM((BYTE)(pageLength + 6));
#else
    Iram_Buf_dram_addr = MSENSE_ADR + 1;
    WriteSDRAM(pageLength + 6);                        // does not include data length field
#endif // End of (EN_LS == 1)

    /* get medium type description information */
    if(Iram_Svr_player_mode == EXECUTING_START_UP_MODE)
    {
        if ( (Iram_Cmd_block.bf[2] & 0xc0) == 0x00 )
        {
            /* check if servo in the middle of start up, return door closed/no disc */
#if (CEProduct == 1)
            mediumType = 0x71;
#else
            mediumType = 0x70;
#endif
        }
        else
        {
            mediumType = 0x00;
        }
    }
    else
    {
        if(Iram_Svr_player_mode == TRAY_IN_MODE)
        {
            if ( (Iram_Cmd_block.bf[2] & 0xc0) == 0x00 )
            {
                /* if servo in the middle way of tray in, return door closed/no disc */
#if (CEProduct == 1)
                if (St_drv_ready == FALSE && St_become_ready == TRUE)
                    mediumType = 0x71;
                else
#endif
                    mediumType = 0x70;
            }
            else
            {
                mediumType = 0x00;
            }
        }
        else
        {
          #if (SET_SLOT_IN == 1) //[BTC][Q36]Jam slot-in for backend power on disc in
            if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1))
          #else  
            if(St_door_open == TRUE)
          #endif 
            {
                mediumType = 0x71;
            }
            else
            {
                if(St_no_disc == TRUE)
                {
                    if ( (Iram_Cmd_block.bf[2] & 0xc0) == 0x00 )
                    {
                        mediumType = 0x70;
                    }
                    else
                    {
                        mediumType = 0x00;
                    }
                    mediumType = 0x70;
                }
                else
                {
                    if(St_drv_ready == FALSE)
                    {
                        mediumType = 0x71;
                    }
                    else
                    {
                        if(st_disc_change == TRUE)
                        {
                            BUILD_SENSE(0x06,0x28,0x00,0x02);/* NOT READY TO READY CHANGE, MEDIUM MAY HAVE CHANGED */
                            st_disc_change = FALSE;
                            AutoCommandComplete();
                            return;
                        }
                        else
                        {
    /* door closed, caddy inserted, check and return the correct type of cd */
                            if(St_12_cm == TRUE)
                            {
        /* 120mm cd size */
                                if ((St_cdrom_data == TRUE) || (pdbDiscLoaded() == DVD_DISC))
                                {
                                    mediumType = 1;
                                }
                                else
                                {
                                    if(St_cdrom_audio == TRUE)
                                    {
                                        mediumType = 2;
                                    }
                                    else
                                    {
                                        if(St_cdrom_mixed == TRUE)
                                        {
                                            mediumType = 3;
                                        }
                                        else
                                        {
                                            if(St_cdrom_hybrid == TRUE)
                                            {
                                                mediumType = 4;
                                            }
                                            else
                                            {
                                                mediumType = 0;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
        /* 80mm cd size */
                                if ((St_cdrom_data == TRUE) || (pdbDiscLoaded() == DVD_DISC))
                                {
                                    mediumType = 5;
                                }
                                else
                                {
                                    if(St_cdrom_audio == TRUE)
                                    {
                                        mediumType = 6;
                                    }
                                    else
                                    {
                                        if(St_cdrom_mixed == TRUE)
                                        {
                                            mediumType = 7;
                                        }
                                        else
                                        {
                                            if(St_cdrom_hybrid == TRUE)
                                            {
                                                mediumType = 8;
                                            }
                                            else
                                            {
                                                mediumType = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(pdbServoGetDiscType() == eATIPDisc)
    {
        if(pdbGetCDRW())
        {  /* if CD-R/W disc, set CD-R/W bit */
            mediumType |= 0x20;
        }
        else
        {
            mediumType |= 0x10;
        }
    }

    if (pdbDiscLoaded() == DVD_DISC)
        mediumType |= 0x40;

    /* Save Header[2] : Medium Type (Obsolete with MMC4) */
    WriteSDRAM(mediumType);

    /* write the mode parameter header data from mode sense scratch area into
       mode sense dram */

    src = MSENSE_ADR;
    dest = MSENSE_SCRATCH;

    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, 8);



    /* Copy Return Data to Mode Sense SDRAM Scratch Area Based on Page Control
       Possible Page Control Values: Current, Changeable, Default, Saved */
    pageControl = Iram_Cmd_block.bf[2] & 0xc0;    // get Page Control
    switch(pageControl)
    {
        case 0:
        /* The PC field has current values
            Input   :   pageCode
            src     :   location of dram current area
            dest    :   location of dram scratch area */

            switch(pageCode)
            {
                case 0x01:
                    src = MSENSE_PAGE_1;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_1_LEN);
                    break;
                #if 0 //For read-only drive, page 5 shall not be supported.
                case 0x05:
                    src = MSENSE_PAGE_5;
                    AppCode = pdbGetApplicationCode();
                    SCSDRAM[src+7] = (AppCode & 0x3F);                  /* For CD-RW 80min by Princo */
                    if(!(AppCode & 0x40))
                    {                               /* Disc for restricted use ? */
                        if ((GetWPM_Byte(7) & 0x3F) != 0)
                        {
                            SCSDRAM[src+7] = (GetWPM_Byte(7) & 0x3F);   /* Get Application code from Write Parameters Mode Page */
                        }
                    }
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_5_LEN);
                    break;
                #endif
                case 0x08:
                    src = MSENSE_PAGE_8;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_8_LEN);
                    break;

                case 0x0d:
                    src = MSENSE_PAGE_D;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_D_LEN);
                    break;

                case 0x0e:
                    Iram_Buf_dram_addr = MSENSE_PAGE_E + 9;
                    Old_left  = ReadSDRAM();
                    Iram_Buf_dram_addr = MSENSE_PAGE_E + 11;
                    Old_right = ReadSDRAM();
                    src = MSENSE_PAGE_E;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_E_LEN);
                    break;

                case 0x1A:
                    src = MSENSE_PAGE_1A;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_1A_LEN );
                    break;

                case 0x1D:
                    src = MSENSE_PAGE_1D;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_1D_LEN );
                    break;

                case 0x2A:
                    src = MSENSE_PAGE_2A;
                    temp_dest = dest;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_2A_LEN);
                    dest = temp_dest;
                    AdjustPage2A((ULONG *)&dest);
                    Iram_Buf_dram_addr = temp_dest + 27;        //byte 27
                    WriteSDRAM(0x00);
                    src = MSENSE_PAGE_2A+20;
                    dest = temp_dest + 28;                  //byte 28,29
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, 2);
                    Iram_Buf_dram_addr = temp_dest + 30;        //byte 30
                    WriteSDRAM(0x00);       //byte 30
                    WriteSDRAM(0x00);       //byte 31
                    break;

#if (EN_LS == 1)
                case 0x31:
                    src  = MSENSE_PAGE_31;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_31_LEN );
                    break;

                case 0x32:
                	//AdjustPage32();
                    src  = MSENSE_PAGE_32;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_32_LEN );
                    break;
#endif // End of (EN_LS == 1)

                default:
                    src = MSENSE_PAGE_1;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_1_LEN);
                    #if 0 //For read-only drive, page 5 shall not be supported.
                    src = MSENSE_PAGE_5;

                    AppCode = pdbGetApplicationCode();
                    SCSDRAM[src+7] = (AppCode & 0x3F);                  /* For CD-RW 80min by Princo */
                    if(!(AppCode & 0x40))
                    {                               /* Disc for restricted use ? */
                        if ((GetWPM_Byte(7) & 0x3F) != 0)
                        {
                            SCSDRAM[src+7] = (GetWPM_Byte(7) & 0x3F);   /* Get Application code from Write Parameters Mode Page */
                        }
                    }
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_5_LEN);
                    #endif
                    src = MSENSE_PAGE_8;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_8_LEN);
                    src = MSENSE_PAGE_D;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_D_LEN);
                    src = MSENSE_PAGE_E;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_E_LEN);
                    src = MSENSE_PAGE_1A;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_1A_LEN);
                    src = MSENSE_PAGE_1D;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_1D_LEN);
#if (EN_LS == 1)
                    src = MSENSE_PAGE_31;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_31_LEN);

                    //AdjustPage32();
                    src = MSENSE_PAGE_32;
                    CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_32_LEN);
#endif // End of (EN_LS == 1)

                    src = MSENSE_PAGE_2A;
                    temp_dest = dest;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, MSENSE_PAGE_2A_LEN);
                    dest = temp_dest;
                    AdjustPage2A((ULONG *)&dest);
            }
            break;
        case 0x80:
        /* The PC field has default values
           Input:     pageCode
                      MSENSE_SCRATCH */

            switch(pageCode)
            {
                case 0x01:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_1_LEN);
                    break;
                case 0x05:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_5_LEN);
                    break;
                case 0x08:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_8_LEN);
                    break;
                case 0x0D:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_D_LEN);
                    break;
                case 0x0E:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_E_LEN);
                    break;
                case 0x1A:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_1A_LEN);
                    break;
                case 0x1D:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_1D_LEN);
                    break;
                case 0x2A:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_2A_LEN);
                    break;

#if (EN_LS == 1)
                case 0x31:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_31_LEN);
                    break;
                case 0x32:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_32_LEN);
                    break;
#endif // End of (EN_LS == 1)

                default:
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x01, MSENSE_PAGE_1_LEN);
                    dest += MSENSE_PAGE_1_LEN;
                    #if 0 //start== HP CMPLY test TCDR704 fail.
                    //For read-only drive, page 5 shall not be supported.
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x05, MSENSE_PAGE_5_LEN);
                    dest += MSENSE_PAGE_5_LEN;
                    #endif
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x08, MSENSE_PAGE_8_LEN);
                    dest += MSENSE_PAGE_8_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x0D, MSENSE_PAGE_D_LEN);
                    dest += MSENSE_PAGE_D_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x0E, MSENSE_PAGE_E_LEN);
                    dest += MSENSE_PAGE_E_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x1A, MSENSE_PAGE_1A_LEN);
                    dest += MSENSE_PAGE_1A_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x1D, MSENSE_PAGE_1D_LEN);
                    dest += MSENSE_PAGE_1D_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x2A, MSENSE_PAGE_2A_LEN);

#if (EN_LS == 1)
                    dest += MSENSE_PAGE_2A_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x31, MSENSE_PAGE_31_LEN);
                    dest += MSENSE_PAGE_31_LEN;
                    CopyDefaultPageDataToSDRAM((ULONG *)&dest, 0x32, MSENSE_PAGE_32_LEN);
#endif // End of (EN_LS == 1)
                    pageCode = 0x3F;
            }
            break;

        default:

        /*                            Changeable Values                                 */
            switch(pageCode)
            {
                case 0x01:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_1_LEN);
                    break;
                case 0x05:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_5_LEN);
                    break;
                case 0x08:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_8_LEN);
                    break;
                case 0x0D:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_D_LEN);
                    break;
                case 0x0E:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_E_LEN);
                    break;
                case 0x1A:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_1A_LEN);
                    break;
                case 0x1D:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_1D_LEN);
                    break;
                case 0x2A:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_2A_LEN);
                    break;

#if (EN_LS == 1)
                case 0x31:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_31_LEN);
                    break;
                case 0x32:
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, pageCode, MSENSE_PAGE_32_LEN);
                    break;
#endif // End of (EN_LS == 1)

                default:    /* it must be page 3f */
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x01, MSENSE_PAGE_1_LEN);
                    dest += MSENSE_PAGE_1_LEN;
                    #if 0 //start== HP CMPLY test TCDR704 fail.
                    //For read-only drive, page 5 shall not be supported.
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x05, MSENSE_PAGE_5_LEN);
                    dest += MSENSE_PAGE_5_LEN;
                    #endif
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x08, MSENSE_PAGE_8_LEN);
                    dest += MSENSE_PAGE_8_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x0D, MSENSE_PAGE_D_LEN);
                    dest += MSENSE_PAGE_D_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x0E, MSENSE_PAGE_E_LEN);
                    dest += MSENSE_PAGE_E_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x1A, MSENSE_PAGE_1A_LEN);
                    dest += MSENSE_PAGE_1A_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x1D, MSENSE_PAGE_1D_LEN);
                    dest += MSENSE_PAGE_1D_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x2A, MSENSE_PAGE_2A_LEN);

#if (EN_LS == 1)
                    dest += MSENSE_PAGE_2A_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x31, MSENSE_PAGE_31_LEN);
                    dest += MSENSE_PAGE_31_LEN;
                    CopyChangeablePageDataToSDRAM((ULONG *)&dest, 0x32, MSENSE_PAGE_32_LEN);
#endif // End of (EN_LS == 1)

                    pageCode = 0x3F;
            }
            break;
    }

#if (EN_LS == 1)
    Ssrc = pageLength + 8;
#else
    BHIGH(Ssrc ) = 0;                           /* data length hi-BYTE */
    BLOW (Ssrc ) = pageLength + 8;              /* page length + 8 bytes header */
#endif // End of (EN_LS == 1)
    BHIGH(Sdest) = Iram_Cmd_block.bf[7];        /* allocation length */
    BLOW (Sdest) = Iram_Cmd_block.bf[8];
    SendParameterDataToHost(MSENSE_SCRATCH, MINIMUM( Ssrc, Sdest) , TRUE);   // Send data and Command Complete.
}

/****************************************************************************
* Name: AdjustPage2A()
* Description:
*   Sets Lock state bit if drive in prevent state, and resets if
*   drive is in allow state
* Inputs:
*   dest - (in/out) dram location pointer
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
static void AdjustPage2A(ULONG *dest)
{
    BYTE bTemp;

    if(TrayManagerInfo.TrayPreventAllowState == TRAY_PREVENT_ALLOW_STATE_UNLOCKED)
        return;

    /* Drive is in prevent state, set lock state bit */

    *dest += 6;

    Iram_Buf_dram_addr = *dest;
    bTemp = ReadSDRAM();  /* Iram_Buf_dram_addr increments after reading dram */

    Iram_Buf_dram_addr = *dest;

    WriteSDRAM((bTemp & 0xFD) | 0x02); /* turn on lock state bit */
}


/****************************************************************************
* Name: CopySDRAMData()
* Description:
*   Copies data from and to different area of dram
* Inputs:
*   src - source pointer
*   dest - destination pointer
*   num - number of bytes to copy
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
void CopySDRAMData(ULONG *dest, ULONG *src, BYTE num)
{
    BYTE  a;

    do
    {
        Iram_Buf_dram_addr = *src;
        a = ReadSDRAM();
        Iram_Buf_dram_addr = *dest;
        WriteSDRAM(a);
        (*src)++;
        (*dest)++;
    } while (--num);
}


/****************************************************************************
* Name: CopyDefaultPageDataToSDRAM()
* Description:
*   Copies default data from rom code to dram
* Inputs:
*   pageCode - page code
*   num - number of bytes to copy
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
static void CopyDefaultPageDataToSDRAM(ULONG *dest, BYTE pageCode, BYTE num)
{
    BYTE const *page_ptr;
    BYTE i;


    Iram_Buf_dram_addr = *dest;
    page_ptr = GetDefaultPointer(pageCode);

    i = 0;
    do
    {
        WriteSDRAM(page_ptr[i]);
        i++;
    } while (--num);
}


/****************************************************************************
* Name: CopyChangeablePageDataToSDRAM()
* Description:
*   Copies data from rom code to dram
* Inputs:
*   pageCode - page code
*   num - number of bytes to copy
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
static void CopyChangeablePageDataToSDRAM(ULONG *dest,
                                    BYTE pageCode, BYTE num)
{
    BYTE i;
    BYTE const *page_ptr;

    switch (pageCode)
    {
        case 0x01:
            page_ptr = mode_sense_changeable_1;
            break;
        case 0x05:
            page_ptr = mode_sense_changeable_5;
            break;
        case 0x0d:
            page_ptr = mode_sense_changeable_d;
            break;
        case 0x08:
            page_ptr = mode_sense_changeable_08;
            break;
        case 0x1A:
            page_ptr = mode_sense_changeable_1a;
            break;
        case 0x1D:
            page_ptr = mode_sense_changeable_1d;
            break;
        case 0x2A:
            page_ptr = mode_sense_changeable_2a;
            break;

#if (EN_LS == 1)
        case 0x31:
            page_ptr = mode_sense_changeable_31;
            break;

        case 0x32:
            page_ptr = mode_sense_changeable_32;
            break;
#endif // End of (EN_LS == 1)
        default:
            page_ptr = mode_sense_changeable_e;
    }
    Iram_Buf_dram_addr = *dest;
    i = 0;
    /* check current page value, get the right changeable pointer. */
    do
    {
        /* copy changeable data to SDRAM */
        WriteSDRAM(page_ptr[i]);
        i++;
    } while (--num);
}


/****************************************************************************
* Name: ModeSelectCommand()
* Description:
*   Processes ATAPI Mode Select command
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
void ModeSelectCommand(void)
{
    BYTE pageCode;
    USHORT Xfer_len;
    ULONG dest, src;
    BYTE    SOTC_st;
    BYTE    Idle_Standy_bit;//Get event status notification fail of Command set


    /* Check Save Pages SP bit (command array BYTE 1 bit 0)
       continue if this bit is not set, else return error */
    if( TEST_BIT(Iram_Cmd_block.bf[1], 0) )
    {
        /* return error - invalid field in command packet */
        BUILD_SENSE(0x05,0x24,0x00,0x45);    /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    /* check parameter list length, return if length is zero
       assume maximum length is FFh */
    if((Iram_Cmd_block.bf[8] & 0xfe) == 0)
    {   /* get lo-BYTE, assume hi-BYTE=0 */
        AutoCommandComplete();
        return;
    }

    #if(PCB == 710)||(PCB == 711)||(PCB == 817) // Avoid Iram_Buf_dram_addr is modified by RW_mark_blk_mode() in IBM testing softwave.
    if (pdbDiscLoaded() == CD_DISC)
    {
    	StopDecoder();
    	ClearDataBuffer();
    	Svr_pause_reqire = TRUE;
    }
    #endif

    /* parameter length is not zero
       get all data from host and save into dram scratch area */
    Iram_Buf_dram_addr = MSENSE_SCRATCH;
    Xfer_len = ((((USHORT)Iram_Cmd_block.bf[ 7 ]) << 8) | (((USHORT)Iram_Cmd_block.bf[ 8 ])));

    GetParameterDataFromHost(MSENSE_SCRATCH,Xfer_len);

    /* check host data:  page code and page length */

    //
    // Determine Page from Host Data - Page Code at offset 8
    //
    Iram_Buf_dram_addr = MSENSE_SCRATCH + MSENSE_HDR_LEN;
    pageCode = ReadSDRAM();           // get page_code

    /* initialize pointer to the mode page (not include mode header) */
    Iram_Buf_dram_addr = MSENSE_SCRATCH + MSENSE_HDR_LEN;

    //
    // Note: Mode Select does not support Page 2A (Mechanical Status Page)
    //

    switch(pageCode)
    {
        case 0x2a:
            /* return error - invalid field in command packet */
            BUILD_SENSE(0x05,0x24,0x00,0x46);    /* INVALID FIELD IN CDB */
            AutoCommandComplete();
            return;

        case 0x01:
            /* if page code = 01, check data receive from the host and saved into dram
               Input:   r6 - page code */
            if(CheckPageData(MSENSE_PAGE_1_LEN, pageCode, 0) == FALSE)
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0x40);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }
            break;

        case 0x0D:
            /* if page code = 0D, check data receive from the host and saved into dram
               Input:   r6 - page code */
            if(CheckPageData(MSENSE_PAGE_D_LEN, pageCode, 0) == FALSE)
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0x42);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }
            break;

        case 0x0E:
            /* if page code = 0E, check data receive from the host and saved into dram */
            if(CheckPageData(MSENSE_PAGE_E_LEN - 4, pageCode, 0) == FALSE)
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0x43);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }

            Iram_Buf_dram_addr  = MSENSE_SCRATCH_LO_PAGES + 2;

            SOTC_st = ReadSDRAM();
            if ( (SOTC_st & 0x02) )
            {
                Iram_Rw_SOTC_st = TRUE;
            }
            else
            {
                Iram_Rw_SOTC_st = FALSE;
            }
#if (AUDIO_PLAYBACK == 1)
            CheckVolume(Old_left, Old_right);
#endif
            break;

        case 0x1A:
            /* if page code = 1A, check data receive from the host and saved into dram */
            if ( CheckPageData( MSENSE_PAGE_1A_LEN, pageCode, 0 ) == FALSE )
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0xf2);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }

            //start == Get event status notification fail of Command set
            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 3;
	    Idle_Standy_bit = ReadSDRAM();
	    if(Idle_Standy_bit & bit1) //Idle bit
            {
                Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 6;
                CMD_IDLE_CNT =  (USHORT)ReadSDRAM() << 8;
                CMD_IDLE_CNT +=  (USHORT)ReadSDRAM();
	    }
            if(Idle_Standy_bit & bit0) //standy bit
            {
                Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 10;
                STANDBYCNT =  (USHORT)ReadSDRAM() << 8;
                STANDBYCNT +=  (USHORT)ReadSDRAM();
	    }
	    //end == Get event status notification fail of Command set

            break;

        case 0x1D:
            /* if page code = 1D, check data receive from the host and saved into dram */
            if ( CheckPageData( MSENSE_PAGE_1D_LEN, pageCode, 0 ) == FALSE )
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0xf3);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }
            setTimeOut();
            break;

#if (EN_LS == 1)
        case 0x31:
            /* if page code = 0x31, check data receive from the host and saved into dram */
            if ( CheckPageData( MSENSE_PAGE_31_LEN, pageCode, 0 ) == FALSE )
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0xf5);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }
			break;
#endif // End of (EN_LS == 1)
        case 0x08:
            if(CheckPageData(MSENSE_PAGE_8_LEN, pageCode, 0) == FALSE)
            {
                /* return error - invalid field in parameter list */
                BUILD_SENSE(0x05,0x26,0x00,0x44);    /* INVALID FIELD IN PARAMETER LIST */
                AutoCommandComplete();
                return;
            }
            break;

        default:
            /* return error - invalid field in command packet */
            BUILD_SENSE(0x05,0x26,0x00,0x45);    /* INVALID FIELD IN PARAMETER LIST */
            AutoCommandComplete();
            return;
    }

    /* save host data from dram scratch area into current mode sense dram area
       check page code, each page code has a fixed location into current dram area */

    src = MSENSE_SCRATCH_LO_PAGES;

    switch( pageCode )
    {
        case 0x01:
            dest = MSENSE_PAGE_1;
            CopySDRAMData((ULONG *)&dest,
                           (ULONG *)&src, MSENSE_PAGE_1_LEN);
            break;

        case 0x0D:
            dest = MSENSE_PAGE_D;
            CopySDRAMData((ULONG *)&dest,
                           (ULONG *)&src, MSENSE_PAGE_D_LEN);
            break;

        case 0x0E:
            dest = MSENSE_PAGE_E;
            CopySDRAMData((ULONG *)&dest,
                           (ULONG *)&src, MSENSE_PAGE_E_LEN);
           break;

        case 0x1A:
            dest = MSENSE_PAGE_1A;
            CopySDRAMData( (ULONG *)&dest,
                            (ULONG *)&src, MSENSE_PAGE_1A_LEN );
            break;

        case 0x1D:
            dest = MSENSE_PAGE_1D;
            CopySDRAMData( (ULONG *)&dest,
                            (ULONG *)&src, MSENSE_PAGE_1D_LEN );
            break;

#if (EN_LS == 1)
        case 0x31:
            dest = MSENSE_PAGE_31;
            CopySDRAMData( (ULONG *)&dest, (ULONG *)&src, MSENSE_PAGE_31_LEN );
            break;
#endif // End of (EN_LS == 1)

        case 0x08:
            dest = MSENSE_PAGE_8;
            CopySDRAMData((ULONG *)&dest,
                           (ULONG *)&src, MSENSE_PAGE_8_LEN);
           break;

        default:
            /* return error - invalid field in command packet */
            BUILD_SENSE(0x05,0x26,0x00,0x46);    /* INVALID FIELD IN PARAMETER LIST */
            AutoCommandComplete();
            return;
    }
    AutoCommandComplete();
}

/****************************************************************************
* Name: copy_mode5_data_to_sram()
* Description:
*   Copies Mode Page 5 data from SDRAM to the mode_p5_data SRAM array
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
void copy_mode5_data_to_sram(void)
{
    BYTE i;
	INT intr;

	intr = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    for ( i = 0; i < MSENSE_PAGE_5_LEN; i++ )

    {
        Iram_Buf_dram_addr = MSENSE_PAGE_5+i;
        mode_p5_data[i] = ReadSDRAM();
    }
	SET_INT_ENABLE_MASK(intr);
}

/****************************************************************************
* Name: CulPreGapLength()
* Description:
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
USHORT CulPreGapLength(BYTE curr_tno,BYTE curr_data_mode){
USHORT  pregap_len;
BYTE    befor_Data_mode,SessionNumber;

    pregap_len = 150;
    if(curr_tno > 1){
        befor_Data_mode = pdbGetTrackDataMode(curr_tno-1);
        if(curr_data_mode != befor_Data_mode){
            SessionNumber = pdbGetTrackSessionNumber(curr_tno);
            if((pdbGetFirstTrackOfSession(SessionNumber) != curr_tno)){
                pregap_len = 225;
            }
        }
    }
    return(pregap_len);
}

#if (AUDIO_PLAYBACK == 1)
/****************************************************************************
* Name: CheckVolume(BYTE Old_left, BYTE Old_right)
* Description:
*   Checks volume data from host, and save final volume data into dram
*   1. If Output Port 0 Channel Selection is zero, set left
*      channel volume to zero
*   2. If Output Port 1 Channel Selection is zero, set right
*      channel volume to zero
*   3. Else the volume = (left volume+right volume)/2
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
static void CheckVolume(BYTE Old_left, BYTE Old_right)
{
    BYTE left_vol, right_vol, final_vol, left_sel, right_sel;

    /* If Output Port 0 Channel Selection is 0, set left channel volume to 0 */
    Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 8;
    left_sel = ReadSDRAM();

    if(left_sel == 0)
    {
        left_vol = 0;
    }
    else
    {
    /* if left volume (r6) equal 0, then port0 muted, do not need to get left
       volume value */

        /* get left volume  */
        Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 9;
        left_vol = ReadSDRAM();
    }
    /* If Output Port 1 Channel Selection is 0, set right channel volume to 0 */

    Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 10;

    right_sel = ReadSDRAM();

    if(right_sel == 0)
    {
        right_vol = 0;
    }
    else
    {
    /* if right volume (r7) equal 0, then port1 muted, do not need to get right
       volume value */
        /* get right volume  */
        Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 11;
        right_vol = ReadSDRAM();
    }

    /* set left volume (r6) and right volume(r7) */
    SetSpeakerVolume(Old_left, Old_right, left_sel, right_sel);
    while ((left_vol != Old_left) ||(right_vol != Old_right))
    {
        if (left_vol > Old_left)
            Old_left++;
        else if (left_vol<Old_left)
                Old_left--;

        if (right_vol > Old_right)
            Old_right++;
        else if (right_vol<Old_right)
                Old_right--;
        SetAudioVolume(Old_left, Old_right);
        DelaymS(5);
    }

    /* if Channel_ctrl_mode=3, both speaker on, need to save the final volume
       setting into the scratch area before update the mode sense data. */
    if((Channel_ctrl_mode & 0x0F) == 3)
    {
        if(left_vol != right_vol)
        {               /* if not same volume */
            final_vol = (left_vol >> 1) + (left_vol & 1) +
                (right_vol >> 1) + (right_vol & 1);
        }
        else
        {
            final_vol = left_vol;
        }

        Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 9;

        WriteSDRAM(final_vol);

        Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 11;

        WriteSDRAM(final_vol);
    }
}
#endif // AUDIO_PLAYBACK


/****************************************************************************
* Name: GetDefaultPointer()
* Description:
*   Checks current page code , return pointer to the current page code
*   default table
* Inputs:
*   pageCode - page number
* Outputs:
*   none
* Returns:
*   pointer to default table
*****************************************************************************/
static BYTE const *GetDefaultPointer(BYTE pageCode)
{
    switch (pageCode)
    {
        case 0x01:
            return (mode_sense_def_1);
        case 0x05:
            return (mode_sense_def_5);
        case 0x0d:
            return (mode_sense_def_d);
        case 0x1A:
            return (mode_sense_def_1a);
        case 0x1D:
            return (mode_sense_def_1d);
        case 0x2A:
            return (mode_sense_def_2a);
        case 0x08:
            return (mode_sense_def_8);

#if (EN_LS == 1)
        case 0x31:
            return(mode_sense_def_31);

        case 0x32:
            return(mode_sense_def_32);
#endif // End of (EN_LS == 1)
        default:
            return (mode_sense_def_e);
    }
}


/****************************************************************************
* Name: GetChangeablePointer()
* Description:
*   Checks current page code , return pointer to the changeable page
*   code default table
* Inputs:
*   pageCode - page number
* Outputs:
*   none
* Returns:
*   pointer to the page code changeable data
*****************************************************************************/
static BYTE const *GetChangeablePointer(BYTE pageCode)
{
    switch (pageCode)
    {
        case 0x01:
            return (mode_sense_changeable_1);
        case 0x05:
            return (mode_sense_changeable_5);
        case 0x0d:
            return (mode_sense_changeable_d);
        case 0x1A:
            return( mode_sense_changeable_1a );
        case 0x1D:
            return( mode_sense_changeable_1d );
        case 0x2a:
            return (mode_sense_changeable_2a);
        case 0x08:
            return (mode_sense_changeable_08);

#if (EN_LS == 1)
        case 0x31:
            return( mode_sense_changeable_31 );
        case 0x32:
            return( mode_sense_changeable_32 );
#endif // End of (EN_LS == 1)

        default:
            return (mode_sense_changeable_e);
    }
}


/****************************************************************************
* Name: CheckPageData()
* Description:
*   Compares Mode Select data received from host with default and changeable
*   Mode page data
* Inputs:
*   pageLength - actual page length
*   pageCode - page code
*   index
* Outputs:
*   none
* Returns:
*   TRUE - data received is OK
*   FALSE - data received is not OK
*****************************************************************************/
static BYTE CheckPageData(BYTE pageLength, BYTE pageCode, BYTE index)
{
BYTE bit_mask;
BYTE def_data, chng_data, host_data;
BYTE const *page_ptr1;
BYTE const *page_ptr2;

BYTE i;
long packet_size;
BYTE packet[4];
BYTE fixed_packet,track_mode;
BYTE data_block_type;
BYTE write_type;

#if 0
//#if RAWMODE == 0
//  /* Write Type Check in Write Parameters Mode Page */
//  if ( (pageCode == 0x05) || (pageCode == 0x3F) )
//  {
//      /* Init ptr to the write type (not include mode header) */
//      if ( pageCode == 0x05 )
//      {
//          Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 2;
//      }
//      else
//      {
//          Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + MSENSE_PAGE_1_LEN + 2;
//      }

        /* get data from DRAM */
//      host_data = ReadSDRAM();

//      if ( (host_data & 0x03) == 0x03 )
//      {
//          return(FALSE);
//      }

        /* Reset ptr to the mode page (not include mode header) */
//      Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES;
//  }
//#endif /* RAWMODE */
#endif

    /* Write Type Check in Write Parameters Mode Page */
    if ( (pageCode == 0x05) || (pageCode == 0x3F) )
    {
        /* Init ptr to the write type (not include mode header) */
        if ( pageCode == 0x05 )
        {
            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 2;
            write_type = ReadSDRAM();

            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 3;
            fixed_packet = ReadSDRAM();

            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 4;
            data_block_type = ReadSDRAM();

            if (fixed_packet & 0x20)
            {
                Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 10;
            }
        }
        else
        {
            //--------------------------------------------
            //   Host Sent Mode Select for  All Pages
            //--------------------------------------------
            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + MSENSE_PAGE_1_LEN + 2;
            write_type = ReadSDRAM();

            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + MSENSE_PAGE_1_LEN + 3;
            fixed_packet = ReadSDRAM();

            Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + MSENSE_PAGE_1_LEN + 4;
            data_block_type = ReadSDRAM();

            if (fixed_packet & 0x20)
            {
                Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + MSENSE_PAGE_1_LEN + 10;
            }
        }

        write_type &= 0x0F;
        data_block_type &= 0x0F;
        track_mode = fixed_packet & 0x0F;

        // Check for invalid data block types (Reserved)
        if(((data_block_type >= 4) && (data_block_type <=7))||
            (data_block_type >=14))
            return(FALSE);


        #if 1 // TBD? Shouldn't this be changed to CD_WRITE == 1?
        // Packet Write
        if (write_type == 0x00)
        {               /* if = 0 then Packet/incrimental WR selected */
            if ((data_block_type == 0) ||       /* 2352  Raw data   */
                (data_block_type == 1) ||       /* 2368  Raw data with P and Q channel */
                (data_block_type == 2) ||       /* Reserved */
                (data_block_type == 3) ||       /* 2448  Raw data with P to W channel appended */
                (data_block_type == 9))
            {       /* 2336  mode 2 */
                return(FALSE);
            }
            if ((track_mode != 5) && (track_mode != 7))
            {
                return(FALSE);
            }

            if (fixed_packet & 0x20)
            {
                for (i = 0; i < 4; i++)
                {
                    packet[i] = ReadSDRAM();
                }
                packet_size   =  (long)packet[3] & 0xFF;
                packet_size  += ((long)packet[2] & 0xFF)<<8;
                packet_size  += ((long)packet[1] & 0xFF)<<16;
                packet_size  += ((long)packet[0] & 0xFF)<<24;
                if (packet_size == 0)
                {
                    return(FALSE);
                }
            }
        }

        // TAO Write
        else if (write_type == 0x01)
        {   /* if = 1 then TAO WR selected */
            if ((data_block_type >= 1) && (data_block_type <= 3))
            {
               return(FALSE);
            }
            if (data_block_type == 0)
            {
                if (track_mode > 3)
                {
                    return(FALSE);
                }
            }

            if ((data_block_type >= 8) && (data_block_type <= 13))
            {
                if ((track_mode != 4) && (track_mode != 6))
                {
                    return(FALSE);
                }
            }

        }

        // SAO Write
        else if (write_type == 0x02)
        {
            /* if = 2 then SAO WR selected */
            if((data_block_type == 1) || (data_block_type == 3))
            {
                return(FALSE);
            }

            if ((data_block_type == 0) || (data_block_type == 2))
            {
                if ((track_mode == 5)||(track_mode >= 7))
                {
                    return(FALSE);
                }
            }

            // if ((data_block_type >= 8) && (data_block_type <= 13))
            // {
            //     if ((track_mode != 4) && (track_mode != 6))
            //     {
            //         return(FALSE);
            //     }
            // }
        }

        // Raw Write
        else if (write_type == 0x03)
        {
            /* if = 3 then RAW WR selected */
            if ((data_block_type == 0)||        /* 2352  Raw data   */
                (data_block_type == 2))
            {       /* Reserved */
                return(FALSE);
            }
            if ((data_block_type == 1) || (data_block_type == 3))
            {
                if ((track_mode == 5)||(track_mode >= 7))
                {
                    return(FALSE);
                }
            }
            if ((data_block_type >= 8) && (data_block_type <= 13))
            {
                return(FALSE);
            }
        }
        else if(write_type == 0x04)
        {
            // TBD? No Code!
        }
        // Unknown Write
        else
        {
            return(FALSE);
        }
        #endif  /*#if 1 */

        /* Reset ptr to the mode page (not include mode header) */
        Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES;
    } // end of Write Page Check



    /* Compare Host Data vs Changeable (or Default) Values - any attempts to
       change a non-changeable value will immediately return FALSE (invalid data)*/
    page_ptr1 = GetChangeablePointer(pageCode);
    page_ptr2 = GetDefaultPointer(pageCode);

    do{
        /* get_data */
        chng_data = page_ptr1[index];
        def_data  = page_ptr2[index];
        host_data = ReadSDRAM();  /* get data from dram */

        if(chng_data != 0xFF)
        {
            if(chng_data == 0x00)
            {
                /* a BYTE cannot change, check dram data with default data */
                if(def_data != host_data)
                    return(FALSE);
            }
            else
            {
                /* some bits can change, check with default bits data */
                if(def_data != host_data)
                {
                    bit_mask = 1;
                    do
                    {
                        if( !(chng_data & bit_mask)&&((def_data & bit_mask) != (host_data & bit_mask)) )
                            return(FALSE);

                        bit_mask <<= 1;
                    }while(bit_mask != 0);
                }
            }
        } // end do-loop

        index++;
    }while(--pageLength);

    return(TRUE);
}


/****************************************************************************
* Name: setTimeOut()
* Description:
*   Check Time-out Group1 data from host, and set to timeUp_Group1
*   1. If Time-out Group1 data from host is more than 6sec,
*      set 6sec to timeUp_Group1
*   2. If Time-out Group1 data from host is less than 6sec,
*      set the data to timeUp_Group1
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
#define GR1_UPPER_LIMIT     6       /* F/W timer limitation */
#define GR1_LOWER_LIMIT     0       /* If HOST Req Time is 0, TIme-Out is Disable */
static void setTimeOut( void )
{
    USHORT  hostReqTimeGr1, hostReqTimeGr2;
    BYTE    bitTbl;

    /* Set DRAM pointer */
    Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 4;

    bitTbl = ReadSDRAM();
    if ( ! (bitTbl & bit2) )
    {
        timeUp_Group1 = TIMEUP_6S;          /* Set default Value */
        timeUp = timeUp_Group1;
        return;
    }

    /* Set DRAM pointer */
    Iram_Buf_dram_addr = MSENSE_SCRATCH_LO_PAGES + 6;

    /* load Group 1 Minimum Time-out */
    hostReqTimeGr1  = (ULONG)ReadSDRAM() << 4;
    hostReqTimeGr1 += (ULONG)ReadSDRAM();

    /* load Group 2 Minimum Time-out */
    hostReqTimeGr2  = (ULONG)ReadSDRAM() << 4;
    hostReqTimeGr2 += (ULONG)ReadSDRAM();

    if ( (hostReqTimeGr1 > GR1_UPPER_LIMIT) || (hostReqTimeGr1 == GR1_LOWER_LIMIT) )
    {
        timeUp_Group1 = TIMEUP_6S;          /* Set default Value */
    }
    else
    {
        timeUp_Group1 = (hostReqTimeGr1 * 1000) / 20;
    }
    timeUp = timeUp_Group1;
}


/****************************************************************************
* Name: setCDSpeedCommand()
* Description:
*   Processes MMC Set CD or DVD Speed Command   Opcode 0xBB
*   Allows Host selection of Read and Write speeds
*   Selected Read Speed in BYTEs 2 and 3 (MSB,LSB)
*   Selected Write Speed in BYTEs 4 and 5 (MSB, LSB)
*   SP1X        0x00B0       176.4 Kbyte
*   SP2X        0x0161       352.8 Kbyte
*   SP4X        0x02C2       705.6 Kbyte
*   SP8X        0x0583      1411.2 Kbyte
*   SP10X       0x06E4      1764.0 Kbyte
*   SP12X       0x0845      2116.8 Kbyte
*   SP16X       0x0B06      2822.4 Kbyte
*   SP20X       0x0DC8      3528.0 Kbyte
*   SP24X       0x108A      4233.6 Kbyte
*   SP32X       0x160D      5644.8 Kbyte
*   SP40X       0x1B90      7056.0 Kbyte
*   SP48X       0x2113      8467.2 Kbyte
*   SP52X       0x23D4      9172.2 Kbyte
*   SP54X       0x2536      9525.6 Kbyte
*
*   SP1XDVD,    //  1320 Kbyte/Sec 8cmm media   DVD
*   SP4XDVD,    //  5280 Kbyte/Sec 8cmm media   DVD
*   SP8XDVD,    // 10560 Kbyte/Sec              DVD
*   SP12XDVD,   // 15840 Kbyte/Sec              DVD
*   SP16XDVD,   // 21120 Kbyte/Sec              DVD
*   SP20XDVD,   // 26400 Kbyte/Sec              DVD
*   Max Speed - Vendor specific values - see vu_cfg.h
* Inputs:
*   none
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
void SetCDSpeedCommand(void)
{
    UINT wr_speed, rd_speed;

    /* ------------------------- */
    /* Check Selected Read Speed */
    /* ------------------------- */
    rd_speed = (UINT)(Iram_Cmd_block.bf[2] << 8) + (UINT)(Iram_Cmd_block.bf[3]);    /* Get Selected Read Speed*/
    CheckSelectedSpeed(RD,rd_speed);

    /* -------------------------- */
    /* Check Selected Write Speed */
    /* -------------------------- */
    wr_speed = (UINT)(Iram_Cmd_block.bf[4] << 8) + (UINT)(Iram_Cmd_block.bf[5]);    /* Get Selected Write Speed*/
    CheckSelectedSpeed(WR,wr_speed);
#if (SEQ_SEEK_SPEED_DOWN == 1)
    RandomCount = 0;
#endif
    AutoCommandComplete();
}


/****************************************************************************
* Name: CheckSelectedSpeed()
* Description:
*   Verifies if data can be written before handling new ATAPI command
* Inputs:
*   md - selected speed is read or write
*   RequestedSpeedKbPerSecond - selected speed for CD or DVD
* Outputs:
*   none
* Returns:
*   none
*****************************************************************************/
void CheckSelectedSpeed(BYTE md, USHORT RequestedSpeedKbPerSecond )
{
    BYTE req_no,req_no1,sp;
    USHORT Delta, Delta1;
    UINT spdt;
// when no disc, only display CD max read speed in Nero Info Tool.
    if (St_no_disc)
    {
        req_no = CD_SPD_COLMNS - 1;
        sp = SelectCDReadSpeed(req_no);
        Disc_Speed.Read = sp;
        Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;

        spdt = GetTransferSpeed(sp);
        WriteSDRAM((BYTE)(spdt >> 8 ));
        WriteSDRAM((BYTE) spdt       );
        return;
    }
    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            // CD type disc in the tray.
            for (req_no1 = CD_SPD_COLMNS; req_no1 > 0; req_no1--)
            {
                req_no = req_no1 - 1;
                if (RequestedSpeedKbPerSecond >= SelCDReqSpeed_tbl[req_no])    // Speed in this table in kBytes/s
                {
                    if(RequestedSpeedKbPerSecond <= SP52X)  /* for set speed over 52X protect */
                    {
                        Delta1 = SelCDReqSpeed_tbl[req_no1]-RequestedSpeedKbPerSecond;
                        Delta  = RequestedSpeedKbPerSecond - SelCDReqSpeed_tbl[req_no];
                        if (Delta1 < Delta)
                        {
                            req_no = req_no1;
                        }
                    }
                    break;
                }
            }

            if(md == RD)
            {
                sp = SelectCDReadSpeed(req_no);
                Disc_Speed.Read = sp;
                APLastAskedSpeed = sp;
                Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;

            }
            else
            {
            }
            spdt = GetTransferSpeed(sp);
            WriteSDRAM((BYTE)(spdt >> 8 ));
            WriteSDRAM((BYTE) spdt       );

            if(SpeedMD <= Standby_f)    // spindle is stopping
            {
#if EN_POWER_CONTROL
                SetDiscReadPowerMode( );
#endif
                ServoPause();   //call pause to spin up
                ServoSetSpeed(eCDSpinUpSpeed); // Sync SVO spin up speed
            }

            break;

        case DVD_DISC:
            // DVD type disc in the tray.
            // RequestedSpeedKbPerSecond contains the host requested DVD speed speed in kBytes per second.
            // SelDVDReqSpeed_tbl[] contains the list of available DVD speeds in kBytes per second.
            // Now find an available speed on the platform with a minimum of the requested speed in kBytes per second.

            if(RequestedSpeedKbPerSecond < SP1XDVD) return;

            for (req_no1 = DVD_SPD_COLMNS; req_no1 > 0; req_no1--)
            {
                req_no = req_no1 - 1;
                if (RequestedSpeedKbPerSecond >= SelDVDReqSpeed_tbl[req_no])    // Speed in this table in kBytes/s
                {
                    if(RequestedSpeedKbPerSecond <= SP16XDVD)
                    {
                        Delta1 = SelDVDReqSpeed_tbl[req_no1]-RequestedSpeedKbPerSecond;
                        Delta  = RequestedSpeedKbPerSecond - SelDVDReqSpeed_tbl[req_no];
                        if (Delta1 < Delta)
                        {
                            req_no = req_no1;
                        }
                    }
                    break;
                }
            }

            if(md == RD)
            {
                // Variable TableEntry now contains the DVD speed (in kBytes per second) we want to use.
                // This value now needs to be converted into an X-type DVD spindle speed.

                sp = SelectDVDReadSpeed(req_no);  /* DVD Disc Set Speed */

                if(GetSpeedValue(sp) > GetSpeedValue(MAX_READ_DVD_SPEED))
                    sp = MAX_READ_DVD_SPEED;

                /* select speed table */
                Disc_Speed.Read = sp;

    #if (FIX_READ_SPEED == 1)
                Host_Requested_speed = sp;
    #endif
                Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;
            }

            // Get actual Transfer Speed for DVD
            // SET CD / DVD SPEED command changes only table of MODE SENSE PAGE 2A.

            spdt = GetTransferSpeed(sp&0x0F);
            WriteSDRAM((BYTE)(spdt >> 8 ));
            WriteSDRAM((BYTE) spdt       );

            /*  When spindle is stopping, and get set speed/stream command
                But SVO can spin up on e8XDCAV only!
                So, it have to spin up first (Call Pause)
                Then set SpeedMD = Set_sp_f
            */
            if(SpeedMD <= Standby_f)    // spindle is stopping
            {
#if EN_POWER_CONTROL
                SetDiscReadPowerMode( );
#endif
                ServoPause();   //call pause to spin up
                ServoSetSpeed(eDVDSpinUpSpeed); // Sync SVO spin up speed
            }
            break;

#if (BD_ENABLE)
        case BD_DISC:
            // BD type disc in the tray.
            if(RequestedSpeedKbPerSecond < SP1XBD) return;

            for (req_no1 = BD_SPD_COLMNS; req_no1 > 0; req_no1--)
            {
                req_no = req_no1 - 1;
                if (RequestedSpeedKbPerSecond >= SelBDReqSpeed_tbl[req_no])    // Speed in this table in kBytes/s
                {
                    if(RequestedSpeedKbPerSecond <= SP6XBD)
                    {
                        Delta1 = SelBDReqSpeed_tbl[req_no1]-RequestedSpeedKbPerSecond;
                        Delta  = RequestedSpeedKbPerSecond - SelBDReqSpeed_tbl[req_no];
                        if (Delta1 < Delta)
                        {
                            req_no = req_no1;
                        }
                    }
                    break;
                }
            }

            if(md == RD)
            {
                // Variable TableEntry now contains the BD speed (in kBytes per second) we want to use.
                // This value now needs to be converted into an X-type BD spindle speed.

                sp = SelectBDReadSpeed(req_no);     /* BD Disc Set Speed */

                if(GetSpeedValue(sp) > GetSpeedValue(MAX_READ_BD_SPEED))
                    sp = MAX_READ_BD_SPEED;

                /* select speed table */
                Disc_Speed.Read = sp;
                Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;
            }

            // Get actual Transfer Speed for BD
            // SET CD / DVD SPEED command changes only table of MODE SENSE PAGE 2A.

            spdt = GetTransferSpeed(sp&0x0F);
            WriteSDRAM((BYTE)(spdt >> 8 ));
            WriteSDRAM((BYTE) spdt       );

            /*  When spindle is stopping, and get set speed/stream command
                But SVO can spin up on e8XDCAV only!
                So, it have to spin up first (Call Pause)
                Then set SpeedMD = Set_sp_f
            */
            if(SpeedMD <= Standby_f)    // spindle is stopping
            {
#if EN_POWER_CONTROL
                SetDiscReadPowerMode( );
#endif
                ServoPause();   //call pause to spin up
                ServoSetSpeed(eBDSpinUpSpeed); // Sync SVO spin up speed
            }
            break;
#endif

#if(ENABLE_HDDVD)
        case HD_DISC:
            // BD type disc in the tray.
            if(RequestedSpeedKbPerSecond < SP1XHD) return;

            for (req_no1 = HD_SPD_COLMNS; req_no1 > 0; req_no1--)
            {
                req_no = req_no1 - 1;
                if (RequestedSpeedKbPerSecond >= SelHDReqSpeed_tbl[req_no])    // Speed in this table in kBytes/s
                {
                    if(RequestedSpeedKbPerSecond <= SP4XHD)
                    {
                        Delta1 = SelHDReqSpeed_tbl[req_no1]-RequestedSpeedKbPerSecond;
                        Delta  = RequestedSpeedKbPerSecond - SelHDReqSpeed_tbl[req_no];
                        if (Delta1 < Delta)
                        {
                            req_no = req_no1;
                        }
                    }
                    break;
                }
            }

            if(md == RD)
            {
                // Variable TableEntry now contains the HD speed (in kBytes per second) we want to use.
                // This value now needs to be converted into an X-type HD spindle speed.

                sp = SelectHDReadSpeed(req_no);     /* BD Disc Set Speed */

                if(GetSpeedValue(sp) > GetSpeedValue(MAX_READ_HD_SPEED))
                    sp = MAX_READ_HD_SPEED;

                /* select speed table */
                Disc_Speed.Read = sp;
                Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;
            }

            // Get actual Transfer Speed for HD
            // SET CD / DVD SPEED command changes only table of MODE SENSE PAGE 2A.

            spdt = GetTransferSpeed(sp&0x0F);
            WriteSDRAM((BYTE)(spdt >> 8 ));
            WriteSDRAM((BYTE) spdt       );

            /*  When spindle is stopping, and get set speed/stream command
                But SVO can spin up on e8XDCAV only!
                So, it have to spin up first (Call Pause)
                Then set SpeedMD = Set_sp_f
            */
            if(SpeedMD <= Standby_f)    // spindle is stopping
            {
#if EN_POWER_CONTROL
                SetDiscReadPowerMode( );
#endif
                ServoPause();   //call pause to spin up
                ServoSetSpeed(eHDSpinUpSpeed); // Sync SVO spin up speed
            }
            break;
#endif
    }

    // Actual speed will change by next disc access command.
    SpeedMD = Set_sp_f;
    #if (SEQ_SEEK_SPEED_DOWN == 1)
    RandomCount = 0;
#endif
}


/****************************************************************************
* Name: SelectCDReadSpeed()
* Description:
*   Selects speed for closed area
* Inputs:
*   r_no - index of speed table
* Outputs:
*   none
* Returns:
*   Selected speed
*****************************************************************************/
BYTE SelectCDReadSpeed(BYTE r_no)
{
BYTE media_no,sp_c;

    media_no = CDReturnMediaOffsetForRead();   /* Determine the row offset based on the media type */
    /* fixed when no_disc can not display all support speed in Nero Info Tool Ver 2.27 */
    if (St_no_disc)
        media_no = 0;

    sp_c = CDSelSpeed_tbl_R[media_no][r_no];

    /* check Support Speed */
    if (GetSpeedValue(sp_c) > GetSpeedValue(Disc_SupportSpeed.Read))
    {
        sp_c = Disc_SupportSpeed.Read;
    }

#if (SUPPORT_DPD == 1)
    if (svoIramVar.do_cd_dpd_disc == TRUE)
    {
        if(sp_c > MAX_MCD_SPEED)
            sp_c = MAX_MCD_SPEED;
    }
#endif

#if (DETECT_ECC_DISC == 1)
    if (svoVar_eccDiscNum != 0)         /* Check if Eccentric Disc is in system */
    {
        /* Get Initial Speed based on Eccentricity */
        if (sp_c > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_eccDiscNum])
           sp_c = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_eccDiscNum];
    }
#endif

#if (CD_UNBALANCE_DISC_CHECK == 1)
    if (svoVar_unbalanceDiscNum != 0)         /* Check if Unbalanced Disc is in system */
    {
       if (sp_c > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_unbalanceDiscNum])
          sp_c = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_CD][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on unbalance */
    }
#endif

    return(sp_c);
}

/****************************************************************************
* Name: SelectDVDReadSpeed()
* Description:
*   Selects speed for closed area
* Inputs:
*   r_no - index of speed table
* Outputs:
*   none
* Returns:
*   Selected speed
*****************************************************************************/
BYTE SelectDVDReadSpeed(BYTE r_no)
{
    BYTE media_no,sp_c;

    media_no = DVDReturnMediaOffsetForRead();  /* Determine the row offset based on the media type */
    sp_c = DVDSelSpeed_tbl_R[media_no][r_no];

    if (GetSpeedValue(sp_c) > GetSpeedValue(Disc_SupportSpeed.Read))
    {
        sp_c = Disc_SupportSpeed.Read;
    }

#if (DETECT_ECC_DISC == 1)
    if (svoVar_eccDiscNum != 0)         /* Check if Eccentric Disc is in system */
    {
        if(pdbGetDiscKind() == eDVDRAMDisc)
        {
            /* Get Initial Speed based on Eccentricity */
            if (sp_c > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_eccDiscNum])
               sp_c = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_eccDiscNum];
        }
        else
        {
            /* Get Initial Speed based on Eccentricity */
            if (sp_c > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_eccDiscNum])
               sp_c = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_eccDiscNum];
        }
    }
#endif

#if (DVD_UNBALANCE_DISC_CHECK == 1)
    if (svoVar_unbalanceDiscNum != 0)         /* Check if Unbalanced Disc is in system */
    {
       if(pdbGetDiscKind() == eDVDRAMDisc)
        {
            if (sp_c > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_unbalanceDiscNum])
                sp_c = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_RAM][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on unbalance */
        }
        else
        {
            if (sp_c > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_unbalanceDiscNum])
                sp_c = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_DVD][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on unbalance */
        }
    }
#endif

    return(sp_c);
}

#if (ENABLE_HDDVD)
/****************************************************************************
* Name: SelectHDReadSpeed()
* Description:
*   Selects speed for closed area
* Inputs:
*   r_no - index of speed table
* Outputs:
*   none
* Returns:
*   Selected speed
*****************************************************************************/
BYTE SelectHDReadSpeed(BYTE r_no)
{
    BYTE media_no,sp_c;

    media_no = HDReturnMediaOffsetForRead();  /* Determine the row offset based on the media type */
    sp_c = HDSelSpeed_tbl_R[media_no][r_no];

    if (GetSpeedValue(sp_c) > GetSpeedValue(Disc_SupportSpeed.Read))
    {
        sp_c = Disc_SupportSpeed.Read;
    }

    return(sp_c);
}
#endif
