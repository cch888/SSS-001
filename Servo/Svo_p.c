/****************************************************************************
*   (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 761 $
* $Date: 11-05-03 19:38 $
*
* DESCRIPTION
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\common\adc.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svo_sta.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_step.h"
#include ".\servo\svo_afe.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_p.h"
#include ".\servo\svo_calib.h"
#include ".\servo\svo_speed.h"
#if (SRC_ENABLE == 1)
#include ".\AL\HAL_dsp.h"
#endif
#include ".\servo\svoPclib.h"
#include ".\dvd\dvmacros.h"
#include ".\common\error.h"
#include ".\player\plrDb.h"
#include ".\hif\gp_ram.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"
#if (WITHOUT_SERVO == 1)
#include ".\hif\powerctl.h"
#endif
#if (EN_ADI == 1)
#include ".\AL\HAL_dsp.h"
#endif
#if (EN_LS == 1)
#include ".\LS\ls_svo.h"
#endif // End of (EN_LS == 1)
#include ".\al\HAL_servo.h"

#define SHOW_DETAIL_SERVO_PROCESS  0
#if (SHOW_DETAIL_SERVO_PROCESS == 1)
#include "..\AL\REG_wobble.h"
USHORT lasti = 0xffff;
USHORT lastj = 0xffff;
USHORT lastk = 0xffff;
#endif

/* If absolute TRN change required: enter seq_tbl entry with (ABS_TRN|TRN_xxx|SEQ_yyy)  */
/* SERVO System Initial                                                 SEQ_EX0  SEQ_EX1  SEQ_EX2  SEQ_EX3 ESEQ_EX4  SEQ_BUSY  */
const SEQ_TBL INIT_TBL[]={
    {state_cmd_init         ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#if (SET_SLOT_IN == 1)
    {state_disc_fg_chk		,{MS_10 		,0x0000 	,0x0000 	}, { SEQ_003, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */ //[Q36]power on initial disc in
#else
    {state_disc_fg_chk      ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_003, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
#endif
    {state_disc_stop        ,{0x0000        ,STOP_TOUT  ,0x0000     }, { SEQ_004, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_disc_stop_wait   ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_005, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_disc_off         ,{0x0000        ,0x0000     ,0x0001     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_tray_in          ,{TI_KICKT      ,TI_TOUT    ,0x0000     }, { SEQ_007, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
#if (SUN710_TRAY_PROFILE == 1)  ||( EEPROM_TRAY_PROFILE ==1) 
    {state_tray_in_stage1   ,{MS_100        ,ON         ,TI_TOUT    }, { SEQ_008, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_tray_in_stage2   ,{MS_700        ,OFF        ,TI_TOUT    }, { SEQ_009, (ERR_TRN|SEQ_001),SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_tray_in_stage3   ,{0x0000        ,OFF        ,0x0000     }, { SEQ_010, (ERR_TRN|SEQ_001),SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009*/
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_init_end         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
#else
    {state_tray_in_step_up  ,{TI_STP        ,OFF        ,0x0000     }, { SEQ_008, SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_009, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_init_end         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
#endif
};

const SEQ_TBL NODISC_TBL[]={
    {state_focus_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_nodisc           ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_disc_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
};

/* AFE VREF Offset Adjust */
const SEQ_TBL VREF_TBL[]={
    {state_afe_vref_aj1     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_mbsbvrefof_cali  ,{0x0000        ,NO_WGOS   ,CALIB_FROM_0}, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

/* Focus Search */
const SEQ_TBL FOUSRCH_TBL[]={
    {state_step_home_chk    ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_focus_init       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_focus_move       ,{0x0000        ,OFF        ,UP         }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_focus_hold       ,{F_SEARCH_T*2  ,0x0000     ,UP         }, { SEQ_005, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_015, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,DOWN       }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_016, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,UP         }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_015, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_016, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_focus_count,      {0x0000,        FSRCH_RETRY,0x0000     }, { SEQ_014, (ERR_TRN|SEQ_001), (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_discedge_chk     ,{0x0002        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_END, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_END, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_focus_retry      ,{0x0000        ,FOCUS_RETRY,0x0000     }, { SEQ_018, (ERR_TRN|SEQ_001), (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_discedge_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_019 */
};

/* Focus Search Error */
const SEQ_TBL ERR_FOUSRCH_TBL[]={
    {state_focus_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_focus_err        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

/* Disc Kick */
const SEQ_TBL DISCKICK_TBL[]={
#if (Customer_A1 ==1)
    {state_disc_start       ,{0x0000        ,DISCK_TOUT ,0x0000     }, { SEQ_002, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#else
    {state_disc_start       ,{0x0000        ,DISCK_TOUT ,0x0000     }, { SEQ_002, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#endif
#if (EN_LS == 0)
    {state_disk_svoON       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL          , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
#else
    {state_disk_svoON       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL          , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_LS_Detection     ,{0x0000        ,0x0000     ,0x0000     }, { (ABS_TRN|TRN_027|SEQ_001), (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
#endif // End of (EN_LS == 0)
};

/* Disc Move Error */
const SEQ_TBL ERR_DISCKICK_TBL[]={
    {state_servo_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_disc_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

const SEQ_TBL FDP_OFFTRACK_TBL[]={
    {state_disc_chg         ,{0x0000        ,0x0000     ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_discedge_chk     ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_013, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,DOWN       }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_014, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,UP         }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_013, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_014, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_focus_count,      {0x0000,        FSRCH_RETRY,0x0000     }, { SEQ_012, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_discedge_chk     ,{0x0002        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_018, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_018, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_focus_retry,      {0x0000,        FOCUS_RETRY,0x0000     }, { SEQ_016, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_discedge_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_disc_chg         ,{MS_10         ,0x0000     ,ON         }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
};

/* Focus Recovery Error when track off */
const SEQ_TBL ERR_FDP_OFFTRACK_TBL[]={
    {state_focus_track_off_err        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

const SEQ_TBL TRON_TBL[]={
#if(ERROR_CAL_START_UP_MONITOR ==1)
#if (Customer_A1 ==1)
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, (ABS_TRN|TRN_002|SEQ_001), (ABS_TRN|TRN_007|SEQ_001), (ERR_TRN|SEQ_001) }},  /* SEQ_001 */
#else
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, (ABS_TRN|TRN_007|SEQ_001), SEQ_010 }},  /* SEQ_001 */
#endif
#else
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_010 }},  /* SEQ_001 */
#endif
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_010 }},  /* SEQ_002 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_003, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_010 }},  /* SEQ_002 */
#endif
    {state_dif_wbl_gain_calib,{0x0000       ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, (ABS_TRN|TRN_033|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL }},  /* SEQ_003 */
    {state_dif_RF_gain_calib,{MS_10         ,TRK_ON     ,0x0000     }, { SEQ_005, SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_pll_init         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_END, SEQ_END, SEQ_END }},  /* SEQ_006 */
#else
#if(Customer_A1==1)
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_002, (ERR_TRN|SEQ_001) }},  /* SEQ_006*/
#else    
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_002, SEQ_010 }},  /* SEQ_006*/
#endif
#endif
    {state_trksvo_chk       ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_008, (ERR_TRN|SEQ_001), (ABS_TRN|TRN_033|SEQ_001), SEQ_001, SEQ_011 }},  /* SEQ_007 */
    {state_disc_chk2        ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_009, SEQ_END, (ABS_TRN|TRN_033|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL }},  /* SEQ_008*/
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    {state_disc_chk3        ,{MS_1          ,MS_300     ,0x0000     }, { SEQ_END, SEQ_ILL, (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
#else
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
#endif
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_001, (ERR_TRN|SEQ_001), (ABS_TRN|TRN_030|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_focus_move       ,{F_SEARCH_T*2  ,OFF        ,DOWN       }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_focus_hold       ,{F_SEARCH_T*2  ,0x0000     ,DOWN       }, { (ABS_TRN|SVO_FOUSRCH|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
};

/* Track On Error */
const SEQ_TBL ERR_TRON_TBL[]={
    {state_servo_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_track_err        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

const SEQ_TBL FDP_DSP_TBL[]={
    {state_disc_chg         ,{0x0000        ,0x0000     ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_dsp_stop         ,{0x0000        ,0x0001     ,0x0000     }, { SEQ_004, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_discedge_chk     ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_015, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,DOWN       }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_016, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,UP         }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_015, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_016, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_focus_count,      {0x0000,        FSRCH_RETRY,0x0000     }, { SEQ_014, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_discedge_chk     ,{0x0002        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_020, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_020, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_focus_retry,      {0x0000,        FOCUS_RETRY,0x0000     }, { SEQ_018, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_discedge_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_019 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_022 }},  /* SEQ_020 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_022 }},  /* SEQ_021 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_022, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_022 }},  /* SEQ_021 */
#endif
    {state_disc_chg         ,{MS_10         ,0x0000     ,ON         }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_022 */
};

/* DSP FE Gain Adjust */
const SEQ_TBL FGDSP_TBL[]={
    {state_dsp_start        ,{MS_100        ,MS_500     ,FGAJ       }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_dsp_end_chk      ,{0x0000        ,0x0000     ,FGAJ       }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_dsp_stop         ,{OK            ,0x0000     ,FGAJ       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_dsp_stop         ,{NG            ,0x0000     ,FGAJ       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
};

/* DSP TE Gain Adjust */
const SEQ_TBL TGDSP_TBL[]={
    {state_dsp_start        ,{MS_10         ,MS_500     ,TGAJ       }, { SEQ_002, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_dsp_end_chk      ,{0x0000        ,0x0000     ,TGAJ       }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_dsp_stop         ,{OK            ,0x0000     ,TGAJ       }, { SEQ_005, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_dsp_stop         ,{NG            ,0x0000     ,TGAJ       }, { SEQ_005, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_006, SEQ_006, SEQ_ILL, SEQ_006, SEQ_006 }},  /* SEQ_005 */
    {state_layerjump_chk    ,{MS_100        ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|TRN_030|SEQ_001), SEQ_007, (ABS_TRN|SVO_FE_PP|SEQ_001), SEQ_ILL }},  /* SEQ_006 */
    {state_focus_move       ,{F_SEARCH_T*2  ,OFF        ,DOWN       }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_focus_hold       ,{F_SEARCH_T*2  ,0x0000     ,DOWN       }, { (ABS_TRN|SVO_FOUSRCH|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
};

/* Play */
const SEQ_TBL PLAYINI_TBL[]={
    {state_play_time        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

const SEQ_TBL PLAY_TBL[]={
    {state_disc_speed       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005,        SEQ_002   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_004 }},  /* SEQ_002 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_005, (ERR_TRN|SEQ_003), SEQ_ILL, SEQ_002, SEQ_004 }},  /* SEQ_003 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_002, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    {state_play_init        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL , SEQ_END, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
#else
    {state_play_init        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL , SEQ_END, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
#endif
    {state_sectorID_chk2    ,{SEC_1         ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_003), (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_002, SEQ_004 }},  /* SEQ_006 *///[SUNEXT]Modify 2011/6/2 
};

/* Play Error */
const SEQ_TBL ERR_PLAY_TBL[]={
    {state_servo_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_disc_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_disc_chg         ,{0x0000        ,1          ,OFF        }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_subq_err         ,{0x0000        ,0x0000     ,1          }, { SEQ_END, SEQ_ILL, (ABS_TRN|SVO_RECOVER|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_023), SEQ_ILL }},  /* SEQ_004 */
    {state_seek_retry       ,{SEQ_001       ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_001, SEQ_ILL }},  /* SEQ_005 */
    {state_err_trk_off      ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
};

/* Disc Stop */
const SEQ_TBL DISCSTOP_TBL[]={
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_focus_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_disc_stop        ,{0x0000        ,STOP_TOUT  ,0x0000     }, { SEQ_004, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_disc_stop_wait   ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_005, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_disc_off         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
};

/* Tray In Move */
const SEQ_TBL TRAY_IN_TBL[]={
    {state_tray_in          ,{TI_KICKT      ,TI_TOUT    ,1          }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    #if (SUN710_TRAY_PROFILE == 1) ||( EEPROM_TRAY_PROFILE ==1) 
    {state_tray_in_stage1   ,{MS_100        ,ON         ,TI_TOUT    }, { SEQ_003, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_tray_in_stage2   ,{MS_700        ,OFF        ,TI_TOUT    }, { SEQ_004, (ERR_TRN|SEQ_001),SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_tray_in_stage3   ,{0x0000        ,OFF        ,0x0000     }, { SEQ_005, (ERR_TRN|SEQ_001),SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004*/
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    #else
    {state_tray_in_step_up  ,{TI_STP        ,OFF        ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    #endif
};

/* Tray Move Error */
const SEQ_TBL ERR_TRAY_IN_TBL[]={
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_tray_err         ,{CLOSE         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

/* Tray Out Move */
const SEQ_TBL TRAY_OUT_TBL[]={
    {state_disc_stop        ,{0x0000        ,STOP_TOUT  ,0x0000     }, { SEQ_002, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001*/
    {state_disc_stop_wait   ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_003, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002*/
#if NO_TRAY_OUT_MOVE == 0
    {state_disc_off         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003*/
#else
    {state_disc_off         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003*/
#endif
    {state_tray_out_wait    ,{MS_500        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004*/
    {state_tray_out         ,{TO_KICKT      ,TO_TOUT    ,0x0000     }, { SEQ_006, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005*/
    #if (SUN710_TRAY_PROFILE == 1)
    {state_tray_out_stage1  ,{MS_200        ,ON         ,0x0000     }, { SEQ_007, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006*/
    {state_tray_out_stage2  ,{TO_TOUT       ,OFF        ,0x0000     }, { SEQ_008, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007*/
    {state_tray_out_stage3  ,{0x0000        ,OFF        ,0x0000     }, { SEQ_009, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008*/
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009*/
    #else
#if (SET_SLOT_IN == 1)
    {state_tray_out_step_dn ,{TO_STP				,OFF				,ON 	    	}, { SEQ_007, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},	/* SEQ_006*/ //[Q36]disc out stop correct place
#else
    {state_tray_out_step_dn ,{TO_STP        ,OFF        ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006*/
#endif
    {state_tray_out_hold    ,{TO_HOLD       ,0x0000     ,0x0000     }, { SEQ_008, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007*/
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008*/
    #endif
};

/* Tray Move Error */
const SEQ_TBL ERR_TRAY_OUT_TBL[]={
    {state_tray_stop        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_tray_err         ,{OPEN          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

/* AFE SPP Adjust */
const SEQ_TBL SPPAJ_TBL[]={
#if (NEW_MPPSPPTE_BG_CALI == 1) && (MPPBG_SPPBG_CALIB != 0)
    {state_mpp_sppbg_cali   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_CEBG_cali        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_lens_shift_cali  ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
#else
    {state_CEBG_cali        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_lens_shift_cali  ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    #if (MPPBG_SPPBG_CALIB != 0)
    {state_mpp_sppbg_cali   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    #endif
#endif
    {state_CEOS_cali        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
};

/* Command Decode */
const SEQ_TBL CMD_DECODE[]={
    {state_cmd_new_req      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_001, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

const SEQ_TBL FDP_TBL[]={
    {state_disc_chg         ,{0x0000        ,0x0000     ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_discedge_chk     ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_014, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,DOWN       }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_015, SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,UP         }, { SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_014, SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_015, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_focus_count,      {0x0000,        FSRCH_RETRY,0x0000     }, { SEQ_013, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_discedge_chk     ,{0x0002        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_019, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_019, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_focus_retry,      {0x0000,        FOCUS_RETRY,0x0000     }, { SEQ_017, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_discedge_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_020, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_019 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_020 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_021, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_020 */
#endif
    {state_disc_chg         ,{MS_10         ,0x0000     ,ON         }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_021 */
};

const SEQ_TBL SEEK_RECOVER_TBL[]={
    {state_disc_chg         ,{0x0000        ,0x0000     ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_trk_off          ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_discedge_chk     ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_014, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,DOWN       }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_015, SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,UP         }, { SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_014, SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_015, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_focus_count,      {0x0000,        FSRCH_RETRY,0x0000     }, { SEQ_013, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_discedge_chk     ,{0x0002        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_019, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_019, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_focus_retry,      {0x0000,        FOCUS_RETRY,0x0000     }, { SEQ_017, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_discedge_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_020, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_028 }},  /* SEQ_019 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_028 }},  /* SEQ_020 */
#else
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_021, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_028 }},  /* SEQ_020 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_022, SEQ_022, SEQ_ILL, SEQ_020, SEQ_028 }},  /* SEQ_021 */
    {state_disc_chg         ,{MS_10         ,0x0000     ,ON         }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_022 */
    //Seek recovery with focus check ok...
    {state_disc_chg         ,{0x0000        ,0x0000     ,OFF        }, { SEQ_024, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_023 */
    {state_trk_recover      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_025, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_024 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_026, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_028 }},  /* SEQ_025 */
#else
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_026, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_028 }},  /* SEQ_025 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_027, SEQ_027, SEQ_ILL, SEQ_025, SEQ_028 }},  /* SEQ_026 */
    {state_disc_chg         ,{MS_10         ,0x0000     ,ON         }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_027 */
    {state_search_rf_zone   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_024, (ABS_TRN|TRN_039|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_028 */
};

/* Pic Home Position Move */
const SEQ_TBL PIC_HOME_TBL[]={
    {state_step_init_chk    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

/* Seek */
const SEQ_TBL SEEK_TBL[]={
    {state_disc_speed       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_004 }},  /* SEQ_002 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_005, (ERR_TRN|SEQ_003), SEQ_ILL, SEQ_002, SEQ_004 }},  /* SEQ_003 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_002, (ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
#if (BD_ECC_IMPROVEMENT==1)
    {state_seek_init        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_006), SEQ_009, SEQ_006, SEQ_ILL }},  /* SEQ_005 */
#else
    {state_seek_init        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_006), SEQ_009, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
#endif
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_006 */
#else
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_006 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_008, (ERR_TRN|SEQ_003), SEQ_ILL, SEQ_006, SEQ_021 }},  /* SEQ_007 */
    {state_sectorID_chk     ,{MS_50         ,0x0000     ,0x0000     }, { SEQ_009, (ERR_TRN|SEQ_003), (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_006, SEQ_021 }},  /* SEQ_008 */   /* time check */
    /* Layer Jump */
    {state_layer_chk        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_012, SEQ_010, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_layerjump_start  ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_015, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_layerjump_err    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, (ERR_TRN|SEQ_004), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    /* Rough Search */
    {state_rough_chk        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_017, SEQ_013, SEQ_007, (ERR_TRN|SEQ_006), SEQ_ILL }},  /* SEQ_012 */
    {state_rough_start      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */   /* rough seek */
    {state_rough_end_chk    ,{RJ_TIMEOUT    ,0x0000     ,0x0000     }, { SEQ_015, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_015 */
    {state_rough_err        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    /* Fine Search */
    {state_fine_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_020, SEQ_018, SEQ_009, (ERR_TRN|SEQ_006), SEQ_ILL }},  /* SEQ_017 *///Add check counter of searching RF & detecting noRF to limit servo seek retry.
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */   /* fine seek */
    {state_fine_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_019 */
    {state_seek_end         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|TRN_061|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_020 */
    {state_search_rf_zone   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_015, (ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_021 */
};

/* Seek Error */
const SEQ_TBL ERR_SEEK_TBL[]={
    {state_servo_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_disc_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_disc_chg         ,{0x0000        ,1          ,OFF        }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_subq_err         ,{0x0000        ,0x0000     ,2          }, { SEQ_END, SEQ_ILL, (ABS_TRN|SVO_RECOVER|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_023), SEQ_ILL }},  /* SEQ_004 */
    {state_seek_retry       ,{SEQ_005       ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_004, SEQ_ILL }},  /* SEQ_005 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
#else
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_007, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
#endif
    {state_err_trk_off      ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
};

#if (EN_LS == 1)
/* LightScribe Seek */
const SEQ_TBL LS_SEEK_TBL[]={
    {state_LS_seek_init   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_LS_seek        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};
#endif // End of (EN_LS == 1)

/* Pause */
const SEQ_TBL PAUSEINI_TBL[]={
    {state_pause_start      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_003), (ABS_TRN|TRN_016|SEQ_001), (ABS_TRN|TRN_039|SEQ_001), SEQ_ILL }},  /* SEQ_001 */
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_005), SEQ_ILL, SEQ_ILL, SEQ_006 }},  /* SEQ_002 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_004, SEQ_004, SEQ_ILL, SEQ_002, SEQ_006 }},  /* SEQ_003 */
    {state_sectorID_chk     ,{MS_100        ,0x0000     ,0x0000     }, { SEQ_005, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_002, SEQ_006 }},  /* SEQ_004 */ /* time check */
    {state_pause_time       ,{MS_500        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_search_rf_zone   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, (ABS_TRN|TRN_039|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_013 }},  /* SEQ_007 */
};

/* Pause Error */
const SEQ_TBL ERR_PAUSEINI_TBL[]=
{
    {state_disc_chg         ,{0x0000        ,1          ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_subq_err         ,{0x0000        ,0x0000     ,0          }, { SEQ_END, SEQ_ILL, (ABS_TRN|SVO_RECOVER|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_023), SEQ_ILL }},  /* SEQ_002 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
#else
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_END, SEQ_END }},  /* SEQ_004 */
    {state_err_trk_off      ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
};

/* Pause */
const SEQ_TBL PAUSE_TBL[]={
    {state_disc_speed       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_004 }},  /* SEQ_002 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_005, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_002, SEQ_004 }},  /* SEQ_003 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_002, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_pause_init       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_sectorID_chk     ,{MS_200        ,0x0000     ,0x0000     }, { SEQ_007, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_002, SEQ_004 }},  /* SEQ_006 */ /* time check */
    {state_fine_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_009, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */   /* fine seek */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_013 }},  /* SEQ_009 */
#else
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_010, (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_013 }},  /* SEQ_009 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_011, SEQ_012, SEQ_ILL, SEQ_009, SEQ_013 }},  /* SEQ_010 */
    {state_sectorID_chk     ,{MS_100        ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_009, SEQ_013 }},  /* SEQ_011 */
    {state_fine_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_009, (ERR_TRN|SEQ_004), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_search_rf_zone   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_014, (ABS_TRN|TRN_039|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_013 }},  /* SEQ_014 */
};

/* Pause Error */
const SEQ_TBL ERR_PAUSE_TBL[]={
    {state_disc_chg         ,{0x0000        ,1          ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_subq_err         ,{0x0000        ,0x0000     ,0          }, { SEQ_END, SEQ_ILL, (ABS_TRN|SVO_RECOVER|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_023), SEQ_ILL }},  /* SEQ_002 */
    {state_seek_retry       ,{SEQ_002       ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_002, SEQ_ILL }},  /* SEQ_003 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,1          }, { SEQ_005, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
#else
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,1          }, { SEQ_005, SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_END, SEQ_END }},  /* SEQ_005 */
    {state_err_trk_off      ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
};

/* Jump */
const SEQ_TBL JUMP_TBL[]={
    {state_jump_init        ,{0x0000        ,SEC_2      ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_jmpcnt_set       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_jmpcnt_chk       ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_006, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_fine_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_END, SEQ_END }},  /* SEQ_006 */
};

/* Jump Error */
const SEQ_TBL ERR_JUMP_TBL[]={
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,1          }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#else
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,1          }, { SEQ_002, SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_END, SEQ_END }},  /* SEQ_002 */
};

/* Blank check */
const SEQ_TBL BLANK_TBL[]={
#if (DVD_RAM_READ == 1) || (ENABLE_DVDRAM_DETECTION == 1)
    {state_dvdram_pp_det    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, (ABS_TRN|SVO_RESTART|SEQ_001), (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_step_home_chk    ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_dif_RF_gain_calib,{MS_1          ,TRK_OFF    ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_blank_chk        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_RESTART|SEQ_001), (ERR_TRN|SEQ_001), SEQ_003, SEQ_002 }},  /* SEQ_004 */
#else
    {state_step_home_chk    ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_dif_RF_gain_calib,{MS_1          ,TRK_OFF    ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_blank_chk        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_RESTART|SEQ_001), (ERR_TRN|SEQ_001), SEQ_002, SEQ_001 }},  /* SEQ_003 */
#endif
};

const SEQ_TBL ERR_BLANK_TBL[]={
    {state_focus_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_unknowndisc      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

#if (NEW_SERVO_CALIB_FLOW == 1)
/* COLLI, TILT, FEBC cal */
const SEQ_TBL SERVO_CALIB_TBL[]={
    #if (CALIBRATION_MANAGEMENT == 1)
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_calib_needed     ,{COLLI_POS     ,RF_P2P_CAL ,0x0000     }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,RF_P2P_CAL ,SEC_2      }, { SEQ_006, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_calib_needed     ,{COLLI_POS     ,TE_P2P_CAL ,0x0000     }, { SEQ_005, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_004 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,TE_P2P_CAL ,SEC_2      }, { SEQ_006, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {state_at_layer_0       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_008, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_2      }, { SEQ_011, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_011, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_012, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_2      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_012 */
    {state_calib_needed     ,{COLLI_POS     ,TE_P2P_CAL ,0x0000     }, { SEQ_014, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,TE_P2P_CAL ,SEC_2      }, { SEQ_015, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    {state_at_layer_0       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_017, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    #else
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_calib_needed     ,{COLLI_POS     ,RF_P2P_CAL ,0x0000     }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,RF_P2P_CAL ,SEC_2      }, { SEQ_006, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_calib_needed     ,{COLLI_POS     ,TE_P2P_CAL ,0x0000     }, { SEQ_005, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_004 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,TE_P2P_CAL ,SEC_2      }, { SEQ_006, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {state_at_layer_0       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_008, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_2      }, { SEQ_011, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_011, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_012, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_2      }, { SEQ_018, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_012 */
    {state_calib_needed     ,{COLLI_POS     ,TE_P2P_CAL ,0x0000     }, { SEQ_014, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,TE_P2P_CAL ,SEC_2      }, { SEQ_015, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    {state_at_layer_0       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_017, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_018, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    {state_caltilt_stepmove1,{0x0000        ,0x0000     ,0x0000     }, { SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_018 */
    {state_caltilt_stepmove2,{MS_10         ,0x0000     ,0x0000     }, { SEQ_020, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_021, SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_020 */
    {state_caltilt_check    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_022, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_021 */
    {state_step_home_chk    ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_022 */
    #endif
};
#endif

/* AFE TE Gain Adjust */
const SEQ_TBL TGAFE_TBL[]={
#if (NEW_SERVO_CALIB_FLOW == 1)
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_002 }},  /*SEQ_001 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_001, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
#else
#if (CALIBRATION_MANAGEMENT == 1)
    {state_at_layer_0       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_003, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_004 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_008, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
#endif
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_2      }, { SEQ_009, SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_020, SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_012, SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_012 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_014, SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_015, SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    #if COLLI_SWITCH ==1
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_029, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
    #else
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
    #endif
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_018 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_020, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_020, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
#endif
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_2      }, { SEQ_021, SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_020 */
    #if COLLI_SWITCH ==1
    {state_calib_needed     ,{COLLI_POS     ,RF_P2P_CAL ,0x0000     }, { SEQ_027, SEQ_028, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_021 */
    {state_calib_needed     ,{COLLI_POS     ,RF_P2P_CAL ,0x0000     }, { SEQ_023, SEQ_029, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_022 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_024, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_023 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_026, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_024 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_025 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_027, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_026 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_027, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_026 */
#endif
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,RF_P2P_CAL ,SEC_2      }, { SEQ_028, SEQ_028, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_027 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_029, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_028 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_029 */
    #else
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_021 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_022 */
    #endif
#else//CALIBRATION_MANAGEMENT
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_002, SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_004 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_2      }, { SEQ_009, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_2      }, { SEQ_011, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_015, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_012 */
    {state_caltilt_stepmove1,{0x0000        ,0x0000     ,0x0000     }, { SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
    {state_caltilt_stepmove2,{MS_10         ,0x0000     ,0x0000     }, { SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_2      }, { SEQ_016, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
    {state_caltilt_check    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_017, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
    {state_step_home_chk    ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_018 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_020 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_022, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_021 */
    {state_teg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_023, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_022 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_026, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_023 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_024 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_025 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_028, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_026 */
    {NULL                   ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_027 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_2      }, { SEQ_029, SEQ_029, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_028 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_029 */
#endif//CALIBRATION_MANAGEMENT
#endif
};

#if (CALIBRATION_MANAGEMENT == 1)
/* FEBC TILT calibration */
const SEQ_TBL CALIBRATION_TBL[]={
    #if (DPD_COMPENSATE_OFFSET == 1)
        #if (COLLI_SWITCH == 1)//hange read retry calibration sequence
    {state_calib_needed     ,{DPD_GNOS      ,TE_P2P_CAL ,0x0000     }, { SEQ_002, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_025 }},  /*SEQ_004 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
#endif
    {state_calib_needed     ,{COLLI_POS     ,RF_P2P_CAL ,0x0000     }, { SEQ_007, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_008, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
    {state_calib_needed     ,{COLLI_POS     ,TE_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,TE_P2P_CAL ,SEC_1      }, { SEQ_012, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_025 }},  /*SEQ_012 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
#endif
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_015, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
            #if (DVDDL_TILT_CAL_PRO == 1)
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_025, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
            #else
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
            #endif
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_018, SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_018 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_1      }, { SEQ_020, SEQ_020, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_025 }},  /*SEQ_020 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_021 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_021 */
#endif
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_023, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_022 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_024, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_023 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_024 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_026, SEQ_026, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_025 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_028, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_025 }},  /*SEQ_026 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_027 */
        #else
    {state_calib_needed     ,{DPD_GNOS      ,TE_P2P_CAL ,0x0000     }, { SEQ_002, SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_017 }},  /*SEQ_004 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_007, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_008, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_007 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_1      }, { SEQ_012, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_017 }},  /*SEQ_012 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_015, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_015 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_016 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_018, SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_017 }},  /*SEQ_018 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
#endif
        #endif
    #else//#if (DPD_COMPENSATE_OFFSET == 1)
        #if (COLLI_SWITCH == 1)
    {state_calib_needed     ,{COLLI_POS     ,RF_P2P_CAL ,0x0000     }, { SEQ_002, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_calib_needed     ,{COLLI_POS     ,TE_P2P_CAL ,0x0000     }, { SEQ_005, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_004 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {state_FEBC_TILT_cal    ,{COLLI_POS     ,TE_P2P_CAL ,SEC_1      }, { SEQ_007, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_020 }},  /*SEQ_007 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#endif
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_013, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_012 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_013 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_1      }, { SEQ_015, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_014 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_020 }},  /*SEQ_015 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#endif
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_018, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_017 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_019, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_018 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_019 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_021, SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_020 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_020 }},  /*SEQ_021 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#endif
        #else
    {state_calib_needed     ,{TILT_DAT      ,RF_P2P_CAL ,0x0000     }, { SEQ_002, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_001 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_002 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_003 */
    {state_calib_needed     ,{TILT_DAT      ,TE_P2P_CAL ,0x0000     }, { SEQ_005, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_004 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_005 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL ,SEC_1      }, { SEQ_007, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_006 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_012 }},  /*SEQ_007 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#endif
    {state_calib_needed     ,{FEBC_DSP      ,RF_P2P_CAL ,0x0000     }, { SEQ_010, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_009 */
    {state_written_chk      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_010 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL ,SEC_1      }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_011 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_013, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_012 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_012 }},  /*SEQ_013 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /*SEQ_008 */
#endif
        #endif
  #endif//#if (DPD_COMPENSATE_OFFSET == 1)
};
#endif /* CALIBRATION_MANAGEMENT */

/* Pause Off (for Data Disc) */
const SEQ_TBL RELSE_TBL[]={
    {state_chk_audio        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_jmpcnt_set       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_jmpcnt_chk       ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_fine_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_END, SEQ_END }},  /* SEQ_006 */
};

/* Pause Off (for Data Disc) Error */
const SEQ_TBL ERR_RELSE_TBL[]={
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,1          }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_END, SEQ_END }},  /* SEQ_002 */
};

/* calculate velocity */
const SEQ_TBL VCALC_TBL[]={
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_002, SEQ_002, SEQ_ILL, SEQ_006, SEQ_005 }},  /* SEQ_001 */
    {state_sectorID_chk     ,{MS_50         ,0x0000     ,0x0000     }, { SEQ_003, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_006, SEQ_005 }},  /* SEQ_002 */
    {state_vcalc_init       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_velocity_meas    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_001, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_005 }},  /* SEQ_006 */
    //boundary detection
    {state_sectorID_chk     ,{MS_50         ,0x0000     ,0x0000     }, { SEQ_008, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_011, SEQ_013 }},  /* SEQ_007 */
    {state_boundary_det     ,{MS_800        ,0x0000     ,0x0000     }, { SEQ_014, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_013 }},  /* SEQ_010 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_013 }},  /* SEQ_011 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_007, SEQ_013, SEQ_ILL, SEQ_011, SEQ_013 }},  /* SEQ_012 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_010, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    //track pitch measurement
    {state_rough_start      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_rough_end_chk    ,{RJ_TIMEOUT    ,0x0000     ,0x0000     }, { SEQ_016, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_016 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_017 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_018, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_021 }},  /* SEQ_017 */
#endif
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_019, SEQ_021, SEQ_ILL, SEQ_017, SEQ_021 }},  /* SEQ_018 */
    {state_sectorID_chk     ,{MS_50         ,0x0000     ,0x0000     }, { SEQ_020, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_017, SEQ_021 }},  /* SEQ_019 */
    {state_trackpitch_meas  ,{MS_300        ,0x0000     ,0x0000     }, { SEQ_014, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_020 */
    {state_search_rf_zone   ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_016, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_021 */
};

const SEQ_TBL FDP_VELO_TBL[]={
    {state_disc_chg         ,{0x0000        ,0x0000     ,OFF        }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_vcalc_err        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_discedge_chk     ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_015, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,DOWN       }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_016, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_focus_move       ,{F_SEARCH_T*2  ,ON         ,UP         }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_015, SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_016, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_focus_count,      {0x0000,        FSRCH_RETRY,0x0000     }, { SEQ_014, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_discedge_chk     ,{0x0002        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_020, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_020, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_focus_retry,      {0x0000,        FOCUS_RETRY,0x0000     }, { SEQ_018, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_008), (ABS_TRN|SVO_FOUSRCH|ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_discedge_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_step_end_chk     ,{MS_10         ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_019 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_022 }},  /* SEQ_020 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_022 }},  /* SEQ_021 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_022, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_022 }},  /* SEQ_021 */
#endif
    {state_disc_chg         ,{MS_10         ,0x0000     ,ON         }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_022 */
};

/* Pic Down And Disc Speed Set */
const SEQ_TBL AJINIT_TBL[]={
    {state_aj_init          ,{0x0000        ,0          ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_step_home_chk    ,{MS_1          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

/*AFE MAIN BEAM Offset Adjustment*/
const SEQ_TBL MBEAMOF_TBL[]={
    {state_mbsbvrefof_cali    ,{0x0000  ,DO_WGOS ,CALIB_FROM_CURRENT}, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_ofsadjflag_check   ,{0x0000      ,0x0000     ,mbeamof_end}, { SEQ_END, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali       ,{MPP_OS      ,DO_WGOS    ,TYPE2      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_afeinof_cali       ,{FEO_OS      ,DO_WGOS    ,TYPE2      }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_afeinof_cali       ,{SRF_OS      ,DO_WGOS    ,TYPE2      }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_afeinof_cali       ,{RADO_OS     ,DO_WGOS    ,TYPE2      }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_afeinof_cali       ,{RBCO_OS     ,DO_WGOS    ,TYPE2      }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_ofsadjflag_set     ,{0x0000      ,0x0000     ,mbeamof_end}, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
};

/*AFE SIDE BEAM Offset Adjustment*/
const SEQ_TBL SBEAMOF_TBL[]={
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,sbeamof_end}, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali     ,{SPP1_OS       ,DO_WGOS    ,TYPE2      }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali     ,{SPP2_OS       ,DO_WGOS    ,TYPE2      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_afeinof_cali     ,{SBAD_OS       ,DO_WGOS    ,TYPE2      }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,sbeamof_end}, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
};

/* AFE FE Offset Adjustment */
const SEQ_TBL FEOF_TBL[]={
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,feof_end   }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali     ,{FE_OS         ,DO_WGOS    ,TYPE1      }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali     ,{FE2_OS        ,DO_WGOS    ,TYPE2      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,feof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
};

/* AFE CE Offset Adjustment */
const SEQ_TBL CEOF_TBL[]={
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,ceof_end   }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali     ,{CE_OS         ,NO_WGOS    ,TYPE3      }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,ceof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
};

/* AFE TE Offset Adjustment 1 */
const SEQ_TBL TEOF_TBL[]={
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,teof_end   }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali     ,{TE_OS         ,NO_WGOS    ,TYPE1      }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali     ,{TE2_OS        ,DO_WGOS    ,TYPE2      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_te_stepsize_meas ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
#if (CHIP_REV >= 0xC0)
    {state_afeinof_cali,     {TX_OS,         NO_WGOS,    TYPE3      }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
#endif
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,teof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
};

/* AFE BS Offset Adjustment */
const SEQ_TBL BSOF_TBL[]={
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,bsof_end   }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali     ,{BS_OS         ,DO_WGOS    ,TYPE2      }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali     ,{BS2_OS        ,DO_WGOS    ,TYPE2      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,bsof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
};

/* AFE PHBH Offset Adjustment */
const SEQ_TBL PHBHOF_TBL[]={
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,phbhof_end   }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali     ,{PHS_OS        ,NO_WGOS    ,TYPE3        }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali     ,{BHS_OS        ,NO_WGOS    ,TYPE3        }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,phbhof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
};

/* AFE RF,RP Offset Adjustment */
const SEQ_TBL RFRPOF_TBL[]={
#if (CHIP_REV < 0xC0)
    {state_ofsadjflag_check, {0x0000,        0x0000,     rfof_end   }, { SEQ_008, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {NULL,                   {0x0000,        0x0000,     0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {NULL,                   {0x0000,        0x0000,     0x0000     }, { SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
#else
    {state_ofsadjflag_check, {0x0000,        0x0000,     rfof_end   }, { SEQ_008, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_afeinof_cali,     {RFREFP_OS,     NO_WGOS,    TYPE4      }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali,     {RFREFN_OS,     NO_WGOS,    TYPE4      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
#endif
    {state_afeinof_cali,     {RFCP_OS,       NO_WGOS,    TYPE2      }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_afeinof_cali,     {RFCN_OS,       NO_WGOS,    TYPE2      }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
#if (NEW_RFPOS_CALIB == 1)
    #if (DVDRAM_USE_OLD_RFPOS_CALIB == 1)//20101022_02R7 replace 02QZ with new compiler switch DVDRAM_USE_OLD_RFPOS_CALIB and fix some code bugs
    {state_RFPOS_cali,       {0x0000,        0x0000,     0x0000     }, { SEQ_007, SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    #else
    {state_RFPOS_cali,       {0x0000,        0x0000,     0x0000     }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    #endif
#else
    {state_afeinof_cali,     {RFP_OS,        NO_WGOS,    TYPE3      }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
#endif
    {state_afeinof_cali,     {RFPGA_OS,      NO_WGOS,    TYPE3      }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    //RP related offset calibrations
    {state_ofsadjflag_check, {0x0000,        0x0000,     rpof_end   }, { SEQ_015, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_afeinof_cali,     {PHS_OS,        NO_WGOS,    TYPE3      }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_afeinof_cali,     {BHS_OS,        NO_WGOS,    TYPE3      }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_rp_bal_cali,      {OFF,           0x0000,     0x0000     }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_afeinof_cali,     {RP_OS,         NO_WGOS,    TYPE3      }, { SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_afeinof_cali,     {RP2_OS,        NO_WGOS,    TYPE3      }, { SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_ofsadjflag_set,   {0x0000,        0x0000,     rpof_end   }, { SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    //RFCOS and DPD offset calibrations
    {state_ofsadjflag_check, {0x0000,        0x0000,     rfof_end   }, { SEQ_END, SEQ_016, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_RFCOS_size_cali,  {0x0000,        0x0000,     0x0000     }, { SEQ_007, SEQ_017, SEQ_002, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_afeinof_cali,     {DPDCH1_OS,     NO_WGOS,    TYPE3      }, { SEQ_018, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_afeinof_cali,     {DPDCH2_OS,     NO_WGOS,    TYPE3      }, { SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_afeinof_cali,     {DPDCH3_OS,     NO_WGOS,    TYPE3      }, { SEQ_020, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_019 */
    {state_afeinof_cali,     {DPDCH4_OS,     NO_WGOS,    TYPE3      }, { SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_020 */
#if (CHIP_REV < 0xC1)
    {state_afeinof_cali,     {DPDGM_OS,      NO_WGOS,    TYPE4      }, { SEQ_022, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_021 */
#endif
    {state_ofsadjflag_set,   {0x0000,        0x0000,     rfof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_022 */
#if (NEW_RFPOS_CALIB == 1) && (DVDRAM_USE_OLD_RFPOS_CALIB == 1)//20101022_02R7 replace 02QZ with new compiler switch DVDRAM_USE_OLD_RFPOS_CALIB and fix some code bugs
    {state_afeinof_cali,     {RFP_OS,        NO_WGOS,    TYPE3      }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_022 */
#endif
};

/* AFE RP/RP2 Offset Adjustment */
const SEQ_TBL RPRP2OF_TBL[]={
    {state_rp_bal_cali      ,{OFF           ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_ofsadjflag_check ,{0x0000        ,0x0000     ,rpof_end   }, { SEQ_END, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_afeinof_cali     ,{RP_OS         ,NO_WGOS    ,TYPE3      }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_afeinof_cali     ,{RP2_OS        ,NO_WGOS    ,TYPE3      }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_ofsadjflag_set   ,{0x0000        ,0x0000     ,rpof_end   }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
};

/* Set DSP TE,CE,FE,BS,RP Offset */
const SEQ_TBL DSPOF_TBL[]={
    {state_dspofs_set       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

/* AFE RP Gain Adjust */
const SEQ_TBL RPGAJ_TBL[]={
    {state_rp_bal_cali      ,{ON            ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_rpg_cali         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

#if (DETECT_ECC_DISC == 1)
const SEQ_TBL ECC_CHK_TBL[]={
#if (Customer_A1 ==1)
    {state_ecc_chk1         ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, (ABS_TRN|TRN_002|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#else
    {state_ecc_chk1         ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
#endif
    {state_ecc_chk2         ,{0x0000        ,ECC_TIMEOUT,0x0000     }, { SEQ_003, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_ecc_chk3         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
};
#endif

const SEQ_TBL FE_PP_MEASURE_TBL[]={
    {state_fe_pp_meas_init  ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_AFE_InputGainCal ,{CALIB_SERVO   ,0x0000     ,0x0000     }, { SEQ_003, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_focus_move       ,{F_CAL_DN_T    ,OFF        ,DOWN       }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_focus_hold       ,{F_CAL_DN_T    ,0x0000     ,DOWN       }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_FE_p2p_init      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_mbsbvrefof_cali  ,{0x0000        ,NO_WGOS   ,CALIB_FROM_0}, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_afeinof_cali     ,{SRF_OS        ,TEMP_OS    ,TYPE2      }, { SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_afeinof_cali     ,{SBAD_OS       ,TEMP_OS    ,TYPE2      }, { SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_afeinof_cali     ,{FEO_OS        ,TEMP_OS    ,TYPE2      }, { SEQ_010, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_afeinof_cali     ,{SPP2_OS       ,TEMP_OS    ,TYPE2      }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_afeinof_cali     ,{FE_OS         ,TEMP_OS    ,TYPE1      }, { SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
#if (ONLINE_FEBG == 1)
    {state_FEBG_cal         ,{ON            ,SEC_1      ,0x0000     }, { SEQ_013, SEQ_013, SEQ_012, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_disc_chk_cunt    ,{0x0001        ,0x0000     ,0x0000     }, { SEQ_ILL, SEQ_014, SEQ_003, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_focus_move       ,{F_SEARCH_T*2  ,PP_MEAS    ,UP         }, { SEQ_015, SEQ_ILL, (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_disc_chk_cunt    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_018, SEQ_016, SEQ_003, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
#if (Customer_A1==1)    
    {state_focus_move       ,{F_SEARCH_T*2  ,PP_MEAS    ,DOWN       }, { SEQ_017, SEQ_ILL, (ABS_TRN|SVO_RESTART|SEQ_001), (ABS_TRN|SVO_PICHOME|SEQ_001), SEQ_ILL }},  /* SEQ_016 */
#else    
    {state_focus_move       ,{F_SEARCH_T*2  ,PP_MEAS    ,DOWN       }, { SEQ_017, SEQ_ILL, (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
#endif    
    {state_disc_chk_cunt    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_018, SEQ_014, SEQ_003, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
#if (Customer_A1 == 1)
    {state_RWdisc_check     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, (ERR_TRN|SEQ_001) }},  /* SEQ_018 */
#else
    {state_RWdisc_check     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
#endif
    
#else
    {state_focus_move       ,{0x0000        ,OFF        ,UP         }, { SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_012 */
    {state_focus_hold       ,{F_SEARCH_T*2  ,0x0000     ,UP         }, { SEQ_014, SEQ_014, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_focus_move       ,{F_SEARCH_T*2  ,PP_MEAS    ,UP         }, { SEQ_015, SEQ_ILL, (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_disc_chk_cunt    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_018, SEQ_016, SEQ_003, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_focus_move       ,{F_SEARCH_T*2  ,PP_MEAS    ,DOWN       }, { SEQ_017, SEQ_ILL, (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_disc_chk_cunt    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_018, SEQ_014, SEQ_003, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_RWdisc_check     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_001), (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
#endif
};

const SEQ_TBL FE_PP_ADJ_TBL[]={
    {state_FEpp_ReMeas      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_AFE_InputGainCal ,{CALIB_ALL     ,0x0000     ,0x0000     }, { SEQ_003, (ABS_TRN|SVO_RESTART|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_FE_gn_adjust     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_BS_gn_adjust     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_cal_ramp_down    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_008, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_focus_move       ,{F_CAL_DN_T    ,OFF        ,DOWN       }, { SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_focus_hold       ,{F_CAL_DN_T    ,0x0000     ,DOWN       }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_focus_move       ,{F_SEARCH_T*2  ,OFF        ,DOWN       }, { SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_focus_hold       ,{F_SEARCH_T*2  ,0x0000     ,DOWN       }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
};


const SEQ_TBL CDDVD_DET_TBL[]={
#if (SET_SLOT_IN == 1)	
    {state_STM_start        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002,  SEQ_ILL, SEQ_END, SEQ_005         , SEQ_ILL }},  /* SEQ_001 */ //[Q36]NoDisc disable laser diode
#else
    {state_STM_start        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002,  SEQ_ILL, SEQ_END, SEQ_ILL         , SEQ_ILL }},  /* SEQ_001 */
#endif
    {state_disc_stop,        {0x0000,        STOP_TOUT,  0x0000     }, { SEQ_003, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},            /* SEQ_002 */
    {state_disc_stop_wait,   {MS_10,         0x0000,     0x0000     }, { SEQ_004, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},            /* SEQ_003 */     
#if (DISC_IDENTIFY == STM_UNIVERSAL) //universal
    {state_STM_measure      ,{eForceBD      ,0x0000     ,0x0000     }, { SEQ_008,  SEQ_005, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_004 */
    {state_STM_measure      ,{eForceHDDVD   ,0x0000     ,0x0000     }, { SEQ_008,  SEQ_006, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_005 */
    {state_STM_measure      ,{eForceDVD     ,0x0000     ,0x0000     }, { SEQ_008,  SEQ_007, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_006 */
    {state_STM_measure      ,{eForceCD      ,0x0000     ,0x0000     }, { SEQ_008,  SEQ_008, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_007 */
    {state_STM_end          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END,  SEQ_ILL, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_008 */
#elif (DISC_IDENTIFY == STM_BD_TRIPLE_READER) //triple_reader_BD
    {state_STM_measure      ,{eForceBD      ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL          }},  /* SEQ_004 */
    {state_STM_measure      ,{eForceDVD     ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL          }},  /* SEQ_005 */
    #if DDisc ==1
    {state_STM_measure      ,{eForceCD      ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_005          }},  /* SEQ_006 */
    #else   //DDisc ==1
    {state_STM_measure      ,{eForceCD      ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL          }},  /* SEQ_006 */
    #endif  //DDisc == 1
    {state_STM_end          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END,  (ERR_TRN|SEQ_001), SEQ_004, SEQ_ILL, SEQ_ILL}},  /* SEQ_007 */
#elif (DISC_IDENTIFY == STM_HD_TRIPLE_READER) //triple_reader_HD
    #if (FORCE_HD_START_UP_FLOW == 0)
    {state_STM_measure      ,{eForceDVD     ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_005, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_004 */
    {state_STM_measure      ,{eForceCD      ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_006, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_005 */
    {state_STM_measure      ,{eForceHDDVD   ,0x0000     ,0x0000     }, { SEQ_007,  SEQ_007, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_006 */
    {state_STM_end          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END,  SEQ_ILL, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_007 */
    #else   // (FORCE_HD_START_UP_FLOW == 0)
    {state_STM_measure      ,{eForceHDDVD   ,0x0000     ,0x0000     }, { SEQ_005,  SEQ_005, (ERR_TRN|SEQ_001),SEQ_END, SEQ_ILL }},  /* SEQ_004 */
    {state_STM_end          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END,  SEQ_ILL, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_005 */
    #endif  // (FORCE_HD_START_UP_FLOW == 0)
#elif (DISC_IDENTIFY == STM_SUPER_MULTI) //supermulti_new
    {state_STM_measure      ,{eForceDVD     ,0x0000     ,0x0000     }, { SEQ_006,  SEQ_005, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_004 */
    #if DDisc ==1
    {state_STM_measure      ,{eForceCD      ,0x0000     ,0x0000     }, { SEQ_006,  SEQ_006, SEQ_ILL, SEQ_ILL         , SEQ_004 }},  /* SEQ_006 */
    #else   //DDisc ==1
    {state_STM_measure      ,{eForceCD      ,0x0000     ,0x0000     }, { SEQ_006,  SEQ_006, SEQ_ILL, SEQ_ILL         , SEQ_ILL }},  /* SEQ_005 */
    #endif  //DDisc == 1
    {state_STM_end          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END,  (ERR_TRN|SEQ_001), SEQ_004, SEQ_ILL, SEQ_ILL}},  /* SEQ_006 */
#endif // DISC_IDENTIFY
};

#if (BCA_ENABLE == 1)
const SEQ_TBL BCA_JUMP_TBL[]={
    {state_disc_speed       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_BCA_jump_init    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_rough_start      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_rough_end_chk    ,{RJ_TIMEOUT    ,0x0000     ,0x0000     }, { SEQ_005, SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_006, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_005 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_006 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ERR_TRN|SEQ_002), SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_006 */
#endif
};

const SEQ_TBL ERR_BCA_JUMP_TBL[]={
    {state_err_trk_off      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_err_trk_off      ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};
#endif

const SEQ_TBL DIAG_TBL[]={
    {state_select_cailMode  ,{FEBG_ADJ      ,0x0000     ,0x0000     }, { SEQ_002, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_FEBG_cal         ,{OFF           ,SEC_1      ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_select_cailMode  ,{FEBC_RRF_ADJ  ,0x0000     ,0x0000     }, { SEQ_004, SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,RF_P2P_CAL,SEC_2       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_select_cailMode  ,{RRFG_ADJ      ,0x0000     ,0x0000     }, { SEQ_006, SEQ_007, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_diaCmd_RRFGCal   ,{0x0000        ,SEC_6      ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_select_cailMode  ,{BETA_ADJ      ,0x0000     ,0x0000     }, { SEQ_008, SEQ_009, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_diaCmd_BETACal   ,{0x0000        ,SEC_6      ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    {state_select_cailMode  ,{TILT_TE_ADJ   ,0x0000     ,0x0000     }, { SEQ_010, SEQ_013, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_009 */
    {state_trk_off          ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_011, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,TE_P2P_CAL,SEC_2       }, { SEQ_012, SEQ_012, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_029, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_012 */
    {state_select_cailMode  ,{READ_BETA     ,0x0000     ,0x0000     }, { SEQ_014, SEQ_015, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_013 */
    {state_diaCmd_ReadBETA  ,{0x0000        ,MS_800     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    {state_select_cailMode  ,{MOVE_SLED_OUT ,0x0000     ,0x0000     }, { SEQ_016, SEQ_017, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_015 */
    {state_diaCmd_sled_move ,{FORW          ,SEC_1      ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */
    {state_select_cailMode  ,{FEBC_JIT_ADJ  ,0x0000     ,0x0000     }, { SEQ_018, SEQ_019, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,JITTER_CAL,SEC_2       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
    {state_select_cailMode  ,{TILT_JIT_ADJ  ,0x0000     ,0x0000     }, { SEQ_020, SEQ_021, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_019 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,JITTER_CAL,SEC_2       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_020 */
    {state_select_cailMode  ,{MOVE_SLED_IN  ,0x0000     ,0x0000     }, { SEQ_022, SEQ_023, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_021 */
    {state_diaCmd_sled_move ,{BACK          ,SEC_1      ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_022 */
    {state_select_cailMode  ,{TILT_RRF_ADJ  ,0x0000     ,0x0000     }, { SEQ_024, SEQ_025, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_023 */
    {state_FEBC_TILT_cal    ,{TILT_DAT      ,RF_P2P_CAL,SEC_2       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_024 */
    {state_select_cailMode  ,{FEBC_TE_ADJ   ,0x0000     ,0x0000     }, { SEQ_026, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_025 */
    {state_trk_off          ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_027, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_026 */
    {state_FEBC_TILT_cal    ,{FEBC_DSP      ,TE_P2P_CAL,SEC_2       }, { SEQ_028, SEQ_028, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_027 */
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_029, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_028 */
#if (FDP_TRKSLIP_MONITOR == 0)
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_029 */
#else
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, (ABS_TRN|SVO_SEEK|ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_029 */
#endif
};

const SEQ_TBL DISCKINDS_DET_TBL[]={
    {state_pp_det           ,{0x000         ,0x0000     ,0x0000     }, { SEQ_END, SEQ_002, (ABS_TRN|TRN_007|SEQ_003), (ABS_TRN|SVO_RESTART|SEQ_001), (ABS_TRN|TRN_007|SEQ_004) }},  /* SEQ_001 */
    {state_focus_move       ,{F_SEARCH_T*2  ,OFF        ,DOWN       }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_focus_hold       ,{F_SEARCH_T*2  ,0x0000     ,DOWN       }, { (ABS_TRN|SVO_FOUSRCH|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
};

#if (HORROR_DISC_IMPROVEMENT == 1)
const SEQ_TBL CHG_RFEQ_TBL[]={
    {state_RFEQ_chg         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END , SEQ_ILL, SEQ_ILL, SEQ_ILL }}, /* SEQ_001 */
};
#endif // (HORROR_DISC_IMPROVEMENT == 1)

#if (EN_LS == 1)
const SEQ_TBL LS_CALIB_TBL[]={
    {state_LS_FOO_Calib     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_LS_IM            ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_LS_TRO_Calib     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_LS_MID           ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
};
#endif // End of (EN_LS == 1)

const TRN_TBL trn_tbl[3][MAX_TRN]={
/* normal */
#if(NEW_PIC_HOME_FLOW)
    {{TRN_016,(SEQ_TBL *) INIT_TBL          },      /*TRN_001*//* SERVO System Initial */
     {TRN_003,(SEQ_TBL *) PIC_HOME_TBL      },      /*TRN_002*//* Pic Home Position Move */
#else
    {{TRN_002,(SEQ_TBL *) INIT_TBL          },      /*TRN_001*//* SERVO System Initial */
     {TRN_016,(SEQ_TBL *) PIC_HOME_TBL      },      /*TRN_002*//* Pic Home Position Move */
#endif
     {TRN_007,(SEQ_TBL *) VREF_TBL          },      /*TRN_003*//* AFE VREF Offset Adjust */
     {TRN_NOP,NULL                          },      /*TRN_004*/
     {TRN_020,(SEQ_TBL *) AJINIT_TBL        },      /*TRN_005*//* Disc Speed Set */
     {TRN_024,(SEQ_TBL *) CEOF_TBL          },      /*TRN_006*//* CE Offset Adjustment */
     {TRN_029,(SEQ_TBL *) CDDVD_DET_TBL     },      /*TRN_007*//* CD DVD disk kind detection */
     {TRN_023,(SEQ_TBL *) FEOF_TBL          },      /*TRN_008*//* AFE FE Offset Adjust */
     {TRN_010,(SEQ_TBL *) MBEAMOF_TBL       },      /*TRN_009*//* AFE MAIN BEAM Offset Adjust */
     {TRN_008,(SEQ_TBL *) SBEAMOF_TBL       },      /*TRN_010*//* AFE SIDE BEAM Offset Adjust */
     {TRN_NOP,(SEQ_TBL *) NULL              },      /*TRN_011*/
     {TRN_013,(SEQ_TBL *) DSPOF_TBL         },      /*TRN_012*//* Set DSP TE,CE,FE,BS,RP Offset */
#if (NEW_SERVO_CALIB_FLOW == 1)
     {TRN_030,(SEQ_TBL *) FOUSRCH_TBL       },      /*TRN_013*//* Focus Search */
#else
     {TRN_032,(SEQ_TBL *) FOUSRCH_TBL       },      /*TRN_013*//* Focus Search */
#endif
     {TRN_016,(SEQ_TBL *) DISCSTOP_TBL      },      /*TRN_014*//* Disc Stop */
#if(NEW_PIC_HOME_FLOW)
     {TRN_016,(SEQ_TBL *) TRAY_IN_TBL       },      /*TRN_015*//* Tray In Move */
#else
     {TRN_002,(SEQ_TBL *) TRAY_IN_TBL       },      /*TRN_015*//* Tray In Move */
#endif
     {TRN_016,(SEQ_TBL *) CMD_DECODE        },      /*TRN_016*//* Command Decode (focus off) */
     {TRN_016,(SEQ_TBL *) TRAY_OUT_TBL      },      /*TRN_017*//* Tray Out Move */
     {TRN_NOP,(SEQ_TBL *) NULL              },      /*TRN_018*/
     {TRN_016,(SEQ_TBL *) PIC_HOME_TBL      },      /*TRN_019*//* Pic Home Position Move */
     {TRN_021,(SEQ_TBL *) FE_PP_MEASURE_TBL },      /*TRN_020*//* Measure FE P2P */
     {TRN_009,(SEQ_TBL *) FE_PP_ADJ_TBL     },      /*TRN_021*//* AFE FE Gain Adjust */
     {TRN_003,(SEQ_TBL *) DISCSTOP_TBL      },      /*TRN_022*//* Disc Stop (Subq/Atip Error) */
     {TRN_025,(SEQ_TBL *) TEOF_TBL          },      /*TRN_023*//* AFE TE Offset Adjust 1 */
     {TRN_012,(SEQ_TBL *) RFRPOF_TBL          },      /*TRN_024*//* RF, RP, & DPD Offset Adjustment */
     {TRN_006,(SEQ_TBL *) BSOF_TBL          },      /*TRN_025*//* AFE BS Offset Adjust */
     {TRN_NOP,NULL                          },      /*TRN_026*/
#if (EN_LS == 1)
     {TRN_016,(SEQ_TBL *) LS_CALIB_TBL      },      /*TRN_027*/
     {TRN_016,(SEQ_TBL *) LS_SEEK_TBL       },      /*TRN_028*/
#else
     {TRN_NOP,NULL                          },      /*TRN_027*/
     {TRN_NOP,NULL                          },      /*TRN_028*/
#endif // End of (EN_LS == 1)
     {TRN_005,(SEQ_TBL *) DISCKICK_TBL      },      /*TRN_029*//* Disc Kick */
#if (NEW_SERVO_CALIB_FLOW == 1)
     {TRN_031,(SEQ_TBL *) BLANK_TBL         },      /*TRN_030*//* Blank check */
     {TRN_032,(SEQ_TBL *) SERVO_CALIB_TBL   },      /*TRN_031*//* COLLI, TILT, FEBC cal */
     {TRN_033,(SEQ_TBL *) DISCKINDS_DET_TBL },      /*TRN_032*//* Disc Kinds Detection */
     {TRN_034,(SEQ_TBL *) SPPAJ_TBL         },      /*TRN_033*//* AFE SPP Adjust */
#else
     {TRN_033,(SEQ_TBL *) BLANK_TBL         },      /*TRN_030*//* Blank check */
     {TRN_NOP,NULL                          },      /*TRN_031*//* COLLI, TILT, FEBC cal */
     {TRN_030,(SEQ_TBL *) DISCKINDS_DET_TBL },      /*TRN_032*//* Disc Kinds Detection */
     {TRN_034,(SEQ_TBL *) SPPAJ_TBL         },      /*TRN_033*//* AFE SPP Adjust */
#endif
     {TRN_041,(SEQ_TBL *) TGAFE_TBL         },      /*TRN_034*//* AFE TE Gain Adjust */
     {TRN_NOP,NULL                          },      /*TRN_035*/
     {TRN_NOP,NULL                          },      /*TRN_036*/
     {TRN_056,(SEQ_TBL *) SEEK_RECOVER_TBL  },      /*TRN_037*/
     {TRN_046,(SEQ_TBL *) TRON_TBL          },      /*TRN_038*//* Tracking On */
     {TRN_039,(SEQ_TBL *) CMD_DECODE        },      /*TRN_039*//* Command Decode (tracking off) */
#if (BCA_ENABLE == 1)
     {TRN_039,(SEQ_TBL *) BCA_JUMP_TBL      },      /*TRN_040*/ // for BCA jump.
#else
     {TRN_NOP,NULL                          },      /*TRN_040*/
#endif
#if (DETECT_ECC_DISC == 1)
     {TRN_042,(SEQ_TBL *) RPGAJ_TBL         },      /*TRN_041*/
     {TRN_038,(SEQ_TBL *) ECC_CHK_TBL       },      /*TRN_042*//* Check Eccentric Disc */
#else
     {TRN_038,(SEQ_TBL *) RPGAJ_TBL         },      /*TRN_041*/
     {TRN_NOP,NULL                          },      /*TRN_042*/
#endif
     {TRN_NOP,NULL                          },      /*TRN_043*/
     {TRN_NOP,NULL                          },      /*TRN_044*/
     {TRN_NOP,NULL                          },      /*TRN_045*/
     {TRN_047,(SEQ_TBL *) FGDSP_TBL         },      /*TRN_046*//* DSP FE Gain Adjust */
     {TRN_055,(SEQ_TBL *) TGDSP_TBL         },      /*TRN_047*//* DSP TE Gain Adjust */
     {TRN_055,(SEQ_TBL *) VCALC_TBL         },      /*TRN_048*//* calculate velocity */
     {TRN_NOP,NULL                          },      /*TRN_049*/
     {TRN_NOP,NULL                          },      /*TRN_050*/
     {TRN_051,(SEQ_TBL *) CMD_DECODE        },      /*TRN_051*//* Command Decode (tracking on)*/
#if (CALIBRATION_MANAGEMENT == 1)
     {TRN_055,(SEQ_TBL *) CALIBRATION_TBL   },      /*TRN_052*//* recalibrate servo parameters */
#else
     {TRN_NOP,NULL                          },      /*TRN_052*/
#endif
     {TRN_051,(SEQ_TBL *) DIAG_TBL          },      /*TRN_053*/ // for focus balance calibration.
#if (HORROR_DISC_IMPROVEMENT == 1)
     {TRN_051,(SEQ_TBL *) CHG_RFEQ_TBL      },      /*TRN_054*//* Change RF parameters */
#else
     {TRN_NOP,NULL                          },      /*TRN_054*/
#endif
     {TRN_056,(SEQ_TBL *) PLAYINI_TBL       },      /*TRN_055*//* Play */
     {TRN_059,(SEQ_TBL *) CMD_DECODE        },      /*TRN_056*//* Command Decode */
     {TRN_055,(SEQ_TBL *) SEEK_TBL          },      /*TRN_057*//* Seek */
     {TRN_055,(SEQ_TBL *) RELSE_TBL         },      /*TRN_058*//* Pause Off (for Data Disc) */
     {TRN_056,(SEQ_TBL *) PLAY_TBL          },      /*TRN_059*//* Play */
     {TRN_055,(SEQ_TBL *) JUMP_TBL          },      /*TRN_060*//* Jump */
     {TRN_061,(SEQ_TBL *) CMD_DECODE        },      /*TRN_061*//* Command Decode */
     {TRN_064,(SEQ_TBL *) CMD_DECODE        },      /*TRN_062*//* Command Decode */
     {TRN_062,(SEQ_TBL *) PAUSEINI_TBL      },      /*TRN_063*//* Pause */
     {TRN_062,(SEQ_TBL *) PAUSE_TBL         },      /*TRN_064*//* Pause */
     {TRN_NOP,NULL                          }},     /*TRN_065*/

/* ERROR */
    {{TRN_016,(SEQ_TBL *) ERR_TRAY_IN_TBL   },      /*TRN_001*/
     {TRN_NOP,NULL                          },      /*TRN_002*/
     {TRN_NOP,NULL                          },      /*TRN_003*/
     {TRN_NOP,NULL                          },      /*TRN_004*/
     {TRN_NOP,NULL                          },      /*TRN_005*/
     {TRN_NOP,NULL                          },      /*TRN_006*/
     {TRN_016,(SEQ_TBL *) NODISC_TBL        },      /*TRN_007*//* No Disc */
     {TRN_NOP,NULL                          },      /*TRN_008*/
     {TRN_NOP,NULL                          },      /*TRN_009*/
     {TRN_NOP,NULL                          },      /*TRN_010*/
     {TRN_NOP,NULL                          },      /*TRN_011*/
     {TRN_NOP,NULL                          },      /*TRN_012*/
     {TRN_016,(SEQ_TBL *) ERR_FOUSRCH_TBL   },      /*TRN_013*//* Focus Search Error */
     {TRN_NOP,NULL                          },      /*TRN_014*/
     {TRN_016,(SEQ_TBL *) ERR_TRAY_IN_TBL   },      /*TRN_015*//* Tray Move Error */
     {TRN_NOP,NULL                          },      /*TRN_016*/
     {TRN_016,(SEQ_TBL *) ERR_TRAY_OUT_TBL  },      /*TRN_017*//* Tray Move Error */
     {TRN_NOP,NULL                          },      /*TRN_018*/
     {TRN_NOP,NULL                          },      /*TRN_019*/
     {TRN_016,(SEQ_TBL *) NODISC_TBL        },      /*TRN_020*//* No Disc */
     {TRN_NOP,NULL                          },      /*TRN_021*/
     {TRN_NOP,NULL                          },      /*TRN_022*/
     {TRN_NOP,NULL                          },      /*TRN_023*/
#if(Customer_A1==1)
     {TRN_016,(SEQ_TBL *) NODISC_TBL          },      /*TRN_024*/
#else
     {TRN_NOP,NULL                          },      /*TRN_024*/
#endif     
     {TRN_NOP,NULL                          },      /*TRN_025*/
     {TRN_NOP,NULL                          },      /*TRN_026*/
     {TRN_NOP,NULL                          },      /*TRN_027*/
     {TRN_NOP,NULL                          },      /*TRN_028*/
     {TRN_016,(SEQ_TBL *) ERR_DISCKICK_TBL  },      /*TRN_029*//* Disc Move Error */
     {TRN_016,(SEQ_TBL *) ERR_BLANK_TBL     },      /*TRN_030*//* Blank error */
     {TRN_016,NULL                           },      /*TRN_031*/
     {TRN_NOP,NULL                          },      /*TRN_032*/
     {TRN_NOP,NULL                          },      /*TRN_033*/
     {TRN_NOP,NULL                          },      /*TRN_034*/
     {TRN_NOP,NULL                          },      /*TRN_035*/
     {TRN_NOP,NULL                          },      /*TRN_036*/
     {TRN_NOP,NULL                          },      /*TRN_037*/
     {TRN_016,(SEQ_TBL *) ERR_TRON_TBL      },      /*TRN_038*//* Track On Error */
     {TRN_NOP,NULL                          },      /*TRN_039*/
#if (BCA_ENABLE == 1)
     {TRN_039,(SEQ_TBL *) ERR_BCA_JUMP_TBL  },      /*TRN_040*/// improve BCA jump (handle focus off during jump)
#else
     {TRN_NOP,NULL                          },      /*TRN_040*/
#endif
     {TRN_NOP,NULL                          },      /*TRN_041*/
     {TRN_NOP,NULL                          },      /*TRN_042*/
     {TRN_NOP,NULL                          },      /*TRN_043*/
     {TRN_NOP,NULL                          },      /*TRN_044*/
     {TRN_NOP,NULL                          },      /*TRN_045*/
     {TRN_NOP,NULL                          },      /*TRN_046*/
     {TRN_NOP,NULL                          },      /*TRN_047*/
     {TRN_NOP,NULL                          },      /*TRN_048*/
     {TRN_NOP,NULL                          },      /*TRN_049*/
     {TRN_NOP,NULL                          },      /*TRN_050*/
     {TRN_NOP,NULL                          },      /*TRN_051*/
     {TRN_NOP,NULL                          },      /*TRN_052*/
     {TRN_NOP,NULL                          },      /*TRN_053*/
     {TRN_NOP,NULL                          },      /*TRN_054*/
     {TRN_NOP,NULL                          },      /*TRN_055*/
     {TRN_NOP,NULL                          },      /*TRN_056*/
     {TRN_016,(SEQ_TBL *) ERR_SEEK_TBL      },      /*TRN_057*//* Seek Error */
     {TRN_051,(SEQ_TBL *) ERR_RELSE_TBL     },      /*TRN_058*//* Pause Off Error */
     {TRN_056,(SEQ_TBL *) ERR_PLAY_TBL      },      /*TRN_059*//* Play Error */
     {TRN_051,(SEQ_TBL *) ERR_JUMP_TBL      },      /*TRN_060*//* Jump Error */
     {TRN_NOP,NULL                          },      /*TRN_061*/
     {TRN_NOP,NULL                          },      /*TRN_062*/
     {TRN_062,(SEQ_TBL *) ERR_PAUSEINI_TBL  },      /*TRN_063*//* Pause Error */
     {TRN_062,(SEQ_TBL *) ERR_PAUSE_TBL     },      /*TRN_064*//* Pause Error */
     {TRN_NOP,NULL                          }},     /*TRN_065*/

/* FOCUS DROP */
    {{TRN_NOP,NULL                          },      /*TRN_001*/
     {TRN_NOP,NULL                          },      /*TRN_002*/
     {TRN_NOP,NULL                          },      /*TRN_003*/
     {TRN_NOP,NULL                          },      /*TRN_004*/
     {TRN_NOP,NULL                          },      /*TRN_005*/
     {TRN_NOP,NULL                          },      /*TRN_006*/
     {TRN_NOP,NULL                          },      /*TRN_007*/
     {TRN_NOP,NULL                          },      /*TRN_008*/
     {TRN_NOP,NULL                          },      /*TRN_009*/
     {TRN_NOP,NULL                          },      /*TRN_010*/
     {TRN_NOP,NULL                          },      /*TRN_011*/
     {TRN_NOP,NULL                          },      /*TRN_012*/
     {TRN_NOP,NULL                          },      /*TRN_013*/
     {TRN_NOP,NULL                          },      /*TRN_014*/
     {TRN_NOP,NULL                          },      /*TRN_015*/
     {TRN_NOP,NULL                          },      /*TRN_016*/
     {TRN_NOP,NULL                          },      /*TRN_017*/
     {TRN_NOP,NULL                          },      /*TRN_018*/
     {TRN_NOP,NULL                          },      /*TRN_019*/
     {TRN_NOP,NULL                          },      /*TRN_020*/
     {TRN_NOP,NULL                          },      /*TRN_021*/
     {TRN_NOP,NULL                          },      /*TRN_022*/
     {TRN_NOP,NULL                          },      /*TRN_023*/
     {TRN_NOP,NULL                          },      /*TRN_024*/
     {TRN_NOP,NULL                          },      /*TRN_025*/
     {TRN_NOP,NULL                          },      /*TRN_026*/
     {TRN_NOP,NULL                          },      /*TRN_027*/
     {TRN_NOP,NULL                          },      /*TRN_028*/
     {TRN_NOP,NULL                          },      /*TRN_029*/
     {TRN_030,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_030*/
     {TRN_031,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_031*/
     {TRN_032,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_032*/
     {TRN_033,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_033*/
     {TRN_034,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_034*/
     {TRN_NOP,NULL                          },      /*TRN_035*/
     {TRN_NOP,NULL                          },      /*TRN_036*/
     {TRN_051,(SEQ_TBL *) FDP_TBL           },      /*TRN_037*/
     {TRN_038,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_038*/
     {TRN_039,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_039*/
#if (BCA_ENABLE == 1)
     {TRN_040,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_040*/
#else
     {TRN_NOP,NULL                          },      /*TRN_040*/
#endif
     {TRN_041,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_041*/
#if (DETECT_ECC_DISC == 1)
     {TRN_042,(SEQ_TBL *) FDP_OFFTRACK_TBL  },      /*TRN_042*/
#else
     {TRN_NOP,NULL                          },      /*TRN_042*/
#endif
     {TRN_NOP,NULL                          },      /*TRN_043*/
     {TRN_NOP,NULL                          },      /*TRN_044*/
     {TRN_NOP,NULL                          },      /*TRN_045*/
     {TRN_046,(SEQ_TBL *) FDP_DSP_TBL       },      /*TRN_046*/
     {TRN_047,(SEQ_TBL *) FDP_DSP_TBL       },      /*TRN_047*/
     {TRN_062,(SEQ_TBL *) FDP_VELO_TBL      },      /*TRN_048*/
     {TRN_NOP,NULL                          },      /*TRN_049*/
     {TRN_NOP,NULL                          },      /*TRN_050*/
     {TRN_051,(SEQ_TBL *) FDP_TBL           },      /*TRN_051*/
#if (CALIBRATION_MANAGEMENT == 1)
     {TRN_051,(SEQ_TBL *) FDP_TBL           },      /*TRN_052*/
#else
     {TRN_NOP,NULL                          },      /*TRN_052*/
#endif
     {TRN_053,(SEQ_TBL *) FDP_TBL           },      /*TRN_053*/
#if (HORROR_DISC_IMPROVEMENT == 1)
     {TRN_051,(SEQ_TBL *) FDP_TBL           },      /*TRN_054*/
#else
     {TRN_NOP,NULL                          },      /*TRN_054*/
#endif
     {TRN_055,(SEQ_TBL *) FDP_TBL           },      /*TRN_055*/
     {TRN_056,(SEQ_TBL *) FDP_TBL           },      /*TRN_056*/
     {TRN_057,(SEQ_TBL *) FDP_TBL           },      /*TRN_057*/
     {TRN_058,(SEQ_TBL *) FDP_TBL           },      /*TRN_058*/
     {TRN_059,(SEQ_TBL *) FDP_TBL           },      /*TRN_059*/
     {TRN_060,(SEQ_TBL *) FDP_TBL           },      /*TRN_060*/
     {TRN_061,(SEQ_TBL *) FDP_TBL           },      /*TRN_061*/
     {TRN_062,(SEQ_TBL *) FDP_TBL           },      /*TRN_062*/
     {TRN_063,(SEQ_TBL *) FDP_TBL           },      /*TRN_063*/
     {TRN_064,(SEQ_TBL *) FDP_TBL           },      /*TRN_064*/
     {TRN_NOP,NULL                          }}      /*TRN_065*/
};

#if (SERVICE_MODE==1)
/*------------------------------------------------------------------------*/
/* The beginning of a new file: svo_service                               */
/*------------------------------------------------------------------------*/
/* Command Decode */
const SEQ_TBL SERV_CMD_DECODE[]={
    {state_service_cmd_new_req,{0x0000      ,0x0000     ,0x0000     }, { SEQ_001,  SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

/* Seek */
const SEQ_TBL urSEEK_TBL[]={
    {state_disc_speed       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_004 ,        SEQ_002   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_tok_chk          ,{TOK_CHK_T_SEEK,0x0000     ,0x0000     }, { SEQ_003, (ERR_TRN|SEQ_007), SEQ_ILL, SEQ_ILL, (ERR_TRN|SEQ_001) }},  /* SEQ_002 */
    {state_pll_chk          ,{PLL_T         ,0x0000     ,0x0000     }, { SEQ_005, (ERR_TRN|SEQ_003), SEQ_ILL, SEQ_002, (ERR_TRN|SEQ_001) }},  /* SEQ_003 */
    {state_seek_init        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005 , (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 *///cchu
    {state_sectorID_chk     ,{MS_150        ,0x0000     ,0x0000     }, { SEQ_006 , (ERR_TRN|SEQ_003), (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_002, (ERR_TRN|SEQ_001) }},  /* SEQ_005 */   /* time check */
    /* Layer Jump */
    {state_layer_chk        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_009 ,        SEQ_007   , (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_layerjump_start  ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_012 ,        SEQ_008   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
    {state_layerjump_err    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005 , (ERR_TRN|SEQ_004), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_008 */
    /* Rough Search */
    {state_rough_chk        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_015 ,        SEQ_010   , SEQ_005, (ERR_TRN|SEQ_006), SEQ_ILL }},  /* SEQ_009 */
    {state_rough_start      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_011 ,        SEQ_ILL   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_010 */   /* rough seek */
    {state_rough_end_chk    ,{RJ_TIMEOUT    ,0x0000     ,0x0000     }, { SEQ_012 ,        SEQ_014   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_011 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_013 ,        SEQ_ILL   , SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_012 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_005 ,        SEQ_ILL   , SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_013 */
    {state_rough_err        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_012 ,        SEQ_ILL   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_014 */
    /* Fine Search */
    {state_fine_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_018 ,        SEQ_016   , SEQ_009, (ERR_TRN|SEQ_006), SEQ_ILL }},  /* SEQ_015 */
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005 ,        SEQ_012   , SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_016 */   /* fine seek */
    {state_fine_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005 , (ERR_TRN|SEQ_006), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_017 */
    {state_seek_end         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END , (ABS_TRN|TRN_061|SEQ_001), SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_018 */
};

/* Seek Error */
const SEQ_TBL ERR_urSEEK_TBL[]={
    {state_servo_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_disc_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_disc_chg         ,{0x0000        ,1          ,OFF        }, { SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_subq_err         ,{0x0000        ,0x0000     ,2          }, { SEQ_END, SEQ_ILL, (ABS_TRN|SVO_RECOVER|SEQ_001), (ABS_TRN|SVO_RECOVER|SEQ_023), SEQ_ILL }},  /* SEQ_004 */
    {state_seek_retry       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, (ABS_TRN|SVO_RECOVER|SEQ_001), SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_err_trksvo_on    ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_err_trk_off      ,{MS_5          ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
};

/* SHORT JUMP*/
const SEQ_TBL urSJUMP_TBL[]={
    /* fine seek */
    {state_urJump_init      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */   /* fine seek */
    {state_fine_start       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */   /* fine seek */
    {state_fine_err         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
};

/* ROUGH JUMP*/
const SEQ_TBL urRJUMP_TBL[]={
    {state_urJump_init      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */   /* fine seek */
    {state_rough_start      ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */   /* rough seek */
    {state_rough_end_chk    ,{RJ_TIMEOUT    ,0x0000     ,0x0000     }, { SEQ_004, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_slow_chk         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_004 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_005 */
};

const SEQ_TBL urSPEED_TBL[]={
    {state_disc_speed       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

const SEQ_TBL SERV_TRKON_TBL[]={
    {state_trksvo_on        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_001 */
    {state_tok_chk          ,{TOK_CHK_T     ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_END }},  /* SEQ_002 */
};


const SEQ_TBL SERV_TRKOFF_TBL[]={
    {state_trk_off          ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
};

const SEQ_TBL SERV_FOC_OFF_TBL[]={
    {state_focus_off        ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_laser_on         ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
};

const SEQ_TBL SERV_FOC_INI_TBL[]={
    {state_focus_init       ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_006, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_007, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
};

const SEQ_TBL SERV_FOC_ON_TBL[]={
    {state_serv_foc_chk     ,{0x0000        ,0x0000     ,0x0000     }, { SEQ_END, SEQ_002, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_001 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,UP         }, { SEQ_003, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_002 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,UP         }, { SEQ_006, SEQ_004, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_003 */
    {state_focus_move       ,{F_SEARCH_T    ,ON         ,DOWN       }, { SEQ_005, SEQ_ILL, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_004 */
    {state_fsrch_chk        ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_007, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_005 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,UP         }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_006 */
    {state_fok_chk          ,{0x0000        ,0x0000     ,DOWN       }, { SEQ_END, SEQ_END, SEQ_ILL, SEQ_ILL, SEQ_ILL }},  /* SEQ_007 */
};

const TRN_TBL servicetrn_tbl[3][MAX_TRN]={
/* normal */
    {{TRN_016,(SEQ_TBL *) NULL           },     /*TRN_001*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_002*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_003*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_004*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_005*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_006*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_007*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_008*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_009*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_010*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_011*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_012*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_013*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_014*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_015*/
     {TRN_016,(SEQ_TBL *) SERV_CMD_DECODE},     /*TRN_016*/
     {TRN_016,(SEQ_TBL *) SERV_TRKOFF_TBL},     /*TRN_017*//* Track_off */
     {TRN_016,(SEQ_TBL *) SERV_TRKON_TBL },     /*TRN_018*//* Track_on */
     {TRN_016,(SEQ_TBL *) SERV_FOC_OFF_TBL},    /*TRN_019*//* Focus_off */
     {TRN_021,(SEQ_TBL *) SERV_FOC_INI_TBL},    /*TRN_020*//* Focus_on */
     {TRN_022,(SEQ_TBL *) SERV_FOC_ON_TBL},     /*TRN_021*/
     {TRN_021,(SEQ_TBL *) SERV_CMD_DECODE},     /*TRN_022*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_023*//* Reserved spindle_off ?? */
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_024*//* Reserved spindle_on ?? */
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_025*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_026*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_027*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_028*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_029*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_030*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_031*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_032*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_033*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_034*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_035*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_036*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_037*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_038*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_039*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_040*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_041*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_042*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_043*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_044*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_045*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_046*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_047*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_048*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_049*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_050*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_051*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_052*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_053*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_054*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_055*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_056*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_057*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_058*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_059*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_060*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_061*/
     {TRN_016,(SEQ_TBL *) urSEEK_TBL     },     /*TRN_062*/
     {TRN_016,(SEQ_TBL *) urRJUMP_TBL    },     /*TRN_063*/
     {TRN_016,(SEQ_TBL *) urSJUMP_TBL    },     /*TRN_064*/
     {TRN_016,(SEQ_TBL *) urSPEED_TBL   }},     /*TRN_065*/

/* ERROR */
    {{TRN_001,(SEQ_TBL *) NULL           },     /*TRN_001*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_002*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_003*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_004*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_005*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_006*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_007*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_008*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_009*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_010*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_011*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_012*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_013*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_014*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_015*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_016*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_017*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_018*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_019*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_020*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_021*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_022*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_023*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_024*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_025*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_026*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_027*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_028*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_029*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_030*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_031*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_032*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_033*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_034*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_035*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_036*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_037*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_038*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_039*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_040*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_041*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_042*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_043*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_044*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_045*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_046*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_047*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_048*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_049*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_050*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_051*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_052*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_053*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_054*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_055*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_056*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_057*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_058*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_059*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_060*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_061*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_062*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_063*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_064*/
     {TRN_001,(SEQ_TBL *) NULL          }},     /*TRN_065*/

/* FOCUS DROP */
    {{TRN_001,(SEQ_TBL *) NULL           },     /*TRN_001*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_002*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_003*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_004*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_005*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_006*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_007*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_008*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_009*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_010*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_011*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_012*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_013*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_014*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_015*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_016*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_017*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_018*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_019*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_020*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_021*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_022*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_023*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_024*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_025*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_026*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_027*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_028*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_029*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_030*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_031*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_032*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_033*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_034*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_035*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_036*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_037*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_038*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_039*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_040*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_041*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_042*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_043*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_044*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_045*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_046*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_047*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_048*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_049*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_050*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_051*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_052*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_053*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_054*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_055*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_056*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_057*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_058*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_059*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_060*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_061*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_062*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_063*/
     {TRN_001,(SEQ_TBL *) NULL           },     /*TRN_064*/
     {TRN_001,(SEQ_TBL *) NULL          }},     /*TRN_065*/
};
#endif /* SERVICE_MODE */


/******************************************************************/
/*  Subroutine  |ServoMain                                       */
/*  Function    |main routine                                    */
/*  Input       |None                                            */
/*  Output      |None                                            */
/*  Global      |                                                */
/*  Editor      |                                                */
/*  Update      |                                                */
/******************************************************************/
#if (HARDCODED_FORCE_DISC == 1)
BOOL    StartCmdExecuted = FALSE;
#endif // (HARDCODED_FORCE_DISC == 1)
void ServoMain(void)
{
    SendMsg80(STATE_MACHINE_FLOW_MSG,0x300000);
    ENABLE_INTERRUPT();
/* For IC verify used only */
#if (WITHOUT_SERVO == 1)
    for(;;)
    {
        PowerControl(eStandbyPower);
        ExitProcess();
    }
#endif

    for(;;)
    {
        {
#if (SERVICE_MODE==1)
            if (SIF.ServoState == svoStServiceMode)
                svoServiceCmdDecode();
            else
#endif
            svoCmdDecode();

            for(;;)
            {
#if (REGISTER_MONITOR == 1)
                SendMsgRegMonitorSampling();
#endif

#if (SERVICE_MODE==1)
                if (SIF.ServoState == svoStServiceMode) svoServiceStateMachine();
                else
#endif
                svoStateMachine();

                if(svoIramVar.focusDropFlag == 1){
                    break;
                }
                if(SIF.ServoState != svoStAccessing){
                    break;
                }
                if(SIF.ServoExecutionState != Busy){
                    break;
                }
            }

#if (HARDCODED_FORCE_DISC == 1) // Due to no sata communication, quick and dirty enable force disckind command
            // let's try
            if ( !StartCmdExecuted )
            {
#if  0   //CDROM
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaROM;
                svoVar_discKindT.Bit.WblType  = eWblUnknown;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscCD;
#endif
#if  0  //DVDPR
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaR;
                svoVar_discKindT.Bit.WblType  = eWblPlus;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
#endif
#if  0  //DVDMR
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaR;
                svoVar_discKindT.Bit.WblType  = eWblMinus;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
#endif
#if  0  //DVDMR
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaR;
                svoVar_discKindT.Bit.WblType  = eWblMinus;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
#endif
#if  1  //DVDROM
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaROM;
                svoVar_discKindT.Bit.WblType  = eWblUnknown;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscDVD;
#endif
#if  0  //HDDVDROM
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaROM;
                svoVar_discKindT.Bit.WblType  = eWblUnknown;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscHD;
#endif
#if  0  //BDDROM
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaROM;
                svoVar_discKindT.Bit.WblType  = eWblUnknown;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscBD;
#endif
#if  1  //BDR
                DiscForce = ENABLE_DISCKIND_FORCING;
                svoVar_discKindT.Bit.Media    = eMediaR;
                svoVar_discKindT.Bit.WblType  = eWblPlus;
                svoVar_discKindT.Bit.Layers   = eLayersSingle;
                svoVar_discKindT.Bit.DiscStd  = eDiscBD;
#endif

                svoCmdStartProc();
                StartCmdExecuted = TRUE;
            }
#endif // (HARDCODED_FORCE_DISC == 1)
        }

        ExitProcess();
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: Fok_Check()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void Fok_Check(void)
{
    if (!FokStatusOK())
    {
        /* Focus Drop ! */

#if (SRC_ENABLE == 1)
    #if (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, ON);
        HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET);
    #endif
        HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
        HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET);
#endif
#if (AFE_NORMALIZER == 1)
        afeSvognDisable();
#endif
        alltimer_stop();
        track_off();
        SledStop();

        svoVar_bByPassFOKCheck = TRUE;
        Iram_EverFKNG = TRUE;
        svoSpeedVar.SpdPresetNeeded = TRUE;

        if (svoMiscVar.forceFocusOffFlag)
        {
            svoMiscVar.forceFocusOffFlag = FALSE;
        }
        else
        {
            // ***Focus_Drop*** In STAT:%d, TRN:%d, SEQ:%d
            if (svoIramVar.switchNextTrnFlag)
            {
                // ***Focus_Drop*** In STAT:%1d, TRN:%2d, SEQ:%2d
                SendMsgCn(SHOW_DEBUG_MSG,3,0x000001,((svoIramVar.svoPreInState & STAT_MASK) >> 12),((svoIramVar.svoPreInState & TRN_MASK) >> 5) + 1,(svoIramVar.svoPreInState & SEQ_MASK) + 1);
        }
            else
            {
                // ***Focus_Drop*** In STAT:%1d, TRN:%2d, SEQ:%2d
        SendMsgCn(SHOW_DEBUG_MSG,3,0x000001,((svoIramVar.svoInState & STAT_MASK) >> 12),((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1,(svoIramVar.svoInState & SEQ_MASK) + 1);
            }

            if (svoMiscVar.focusDropCount == FORCE_FCS_OFF_TH - 3)//from 720
            {
                if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Layers == eLayersDouble))
                {
                    svoMiscVar.focusDropCount++;
                    svoColli_Homing();
                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(Layer_Index));
                    svoCalColliVar[Layer_Index].status |= CAL_REQUIRED;
                    SendMsg80(SHOW_DEBUG_MSG,0x340007);  // "WARNING: Colli pos reset and re-calib"
                }
            }

            /* Reset AGC/AOC to avoid AGC/AOC hold on improper values */
            HAL_DFE_LoadCalRfAgcAoc();
//20101019_02QB ==start== Modify AGC/AOC reset message code, frank modify frank confirm
            SendMsg80(SHOW_DEBUG_MSG,0x340009);
//20101019_02QB ==end== Modify AGC/AOC reset

        if ((++svoMiscVar.focusDropCount >= FORCE_FCS_OFF_TH) || (svoIramVar.knlTimeupCount == 0) || (svoIramVar.emergencyStopFlag == TRUE))
        {
#if (CHIP_REV >= 0xC0)
            HAL_SERVO_SetFcs((FDP_FOO_OS - HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
    #if (FDP_PROT_PULSE_TIME != 0)
            Delay100uS(FDP_PROT_PULSE_TIME);
    #endif
            WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
            WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
            HAL_SERVO_SetFocDn(0);
            HAL_SERVO_SetFocStp();
#elif (NEW_FDP_DETECTION == 0)
            //hold lens at bottom
            HAL_SERVO_SetFoos(FDP_FOO_OS);
            WRITE_FIELD(HAL_SRV_LDOFF, 0x00);
            WRITE_FIELD(HAL_SRV_LDON, 0x00);
#endif
            focus_wait_stable(FDP_STABLE_TIME);

                //"ERROR: maximum allowed focusDropCount reached, CMD time last: %4u ms, cur_RPM: %u, focusDropCount: %d", 2 2 1
                SendMsgCnB(SHOW_DEBUG_MSG,5,0x33FF64,B2B(svoIramVar.knlTimeupCount*20),B2B(spdGetCurrentRPM()),B1B(svoMiscVar.focusDropCount));

            svoMiscVar.focusDropCount = 0;
            svoMiscVar.focusRampCount = 0;
            svoMiscVar.focusRetryCount = 0;
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;

                if (((svoIramVar.svoInState & TRN_MASK) >= READY_TRN) || ((svoIramVar.svoInState & TRN_MASK) == SVO_RECOVER)
#if (CALIBRATION_MANAGEMENT == 1)
                    || ((svoIramVar.svoInState & TRN_MASK) == SVO_RECALIB)
#endif
                    )
                {
                    svoMiscVar.forceFocusOffFlag = TRUE;
                    svoIramVar.svoInState = (SVO_SEEK|ERR_TRN|SEQ_008);
                }
                else
                {
                    if ((svoIramVar.svoInState & TRN_MASK) < FOK_CHK_TRN)
                    {
                        svoIramVar.svoInState = (SVO_FOUSRCH|ERR_TRN|SEQ_001);
                    }
                }
            }
        }

        switch (svoIramVar.svoInState & TRN_MASK)
        {
            case SVO_SEEK:
            case SVO_JUMP:
            case SVO_PAUSE:
                HAL_SERVO_CenterControlOFF();
#if (DVD_RAM_READ == 1)
                if (DVDRAM)
                    Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_SERVO_BS);
                else
#endif
                    Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
                break;
        }

        if ((svoIramVar.svoInState & TRN_MASK) >= READY_TRN)
            svoIramVar.focusDropFlag = 1;

        if (!svoMiscVar.forceFocusOffFlag)
        {
            if (((svoIramVar.svoInState & TRN_MASK) == SVO_SEEK) && (svoMiscVar.LayerJumpErrorFlag == 1))
            {
                svoIramVar.svoInState = (svoIramVar.svoInState & TRN_MASK)|(ERR_TRN|SEQ_004);   /* State_No = ERROR_TRN */
                svoMiscVar.LayerJumpErrorFlag = 0;
                svoIramVar.focusDropFlag = 0;
            }
            else
            {
                svoIramVar.svoInState = ((svoIramVar.switchNextTrnFlag? svoIramVar.svoPreInState : svoIramVar.svoInState) & TRN_MASK)|(FOCUS_DROP|SEQ_001);    /* State_No = FOCUS_DROP */
            }
        }

    }
    else
    {
#if (EN_ADI == 1)
        if ((ADICounter < ADI_LIMIT_COUNT))
        {
            if (ReadTimer(&ADItimer) > ADI_INTERVAL_CHECK)
            {
                StartTimer(&ADItimer);
                ADICounter = 0;
            }

        if(HAL_DSP_SetActFocDissInt())
            send_msg5S(1,"enable FocADI");
        if(HAL_DSP_SetActTrkDissInt())
            send_msg5S(1,"enable TrkADI");
        //if(HAL_DSP_SetActTiltDissInt())
        //    send_msg5S(1,"enable TiltADI");
    }
#endif
    }
}

#if (SERVICE_MODE==1)
void svoServiceStateMachine(void)
{
USHORT    i;
USHORT    j;

    if(delay_timer_check() == Busy)
    {   /* delay wait */
        return;
    }

    i = ((svoIramVar.svoInState & STAT_MASK) >> 12);
    j = ((svoIramVar.svoInState & TRN_MASK ) >> 5 );
//>>>Send Svo Machine Flow Message start<<<<
    if ( (svoIramVar.svoInState != svoIramVar.svoPreInState) && !((svoIramVar.svoInState & TRN_MASK)==TRN_016))
    {
        if (((svoIramVar.svoInState & TRN_MASK )!=(svoIramVar.svoPreInState & TRN_MASK )) || ((svoIramVar.svoInState & STAT_MASK )!=(svoIramVar.svoPreInState & STAT_MASK )))
        {
            send_msg21(STATE_MACHINE_FLOW_MSG,0x0500010+i,(j+1));
        }
        send_msg21(STATE_MACHINE_FLOW_MSG,0x50001F,((svoIramVar.svoInState & SEQ_MASK)+1));
    }

    svoSeq((servicetrn_tbl[i][j].seq + (svoIramVar.svoInState & SEQ_MASK)));
    if ((svoIramVar.svoInState & SEQ_MASK) == SEQ_END)
    {         /* Sequence End */
        svoIramVar.svoInState = (servicetrn_tbl[i][j].next_trn_no | SEQ_001);  /* YES:Next TRN_NO Set */
    }
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: svoStateMachine()
 * Description:
 *
 *----------------------------------------------------------------------------*/
extern vULONG   Iram_system_timer;

void svoStateMachine(void)
{
    USHORT    i;
    USHORT    j;
#ifdef JTT
    USHORT    bpt;
#endif
#if (MONITOR_OPU_TEMPERATURE == 1)
    SBYTE  Temperature;
#endif /* MONITOR_OPU_TEMPERATURE=1 */

    if (((svoIramVar.svoInState & STAT_MASK) == FOCUS_DROP) && (svoCmdVar.svoCommand == CSTOP))
    {
        svoIramVar.svoInState = TRN_014 | SEQ_001;
    }

#if (EN_ADI == 1)
    if (servo_status & SERVO_STATUS_ACTDISS_FOC)  //FOC ACTDISS
    {
        if (FokStatusOK())
        {
            svoFocusOff();                                    /* set LD off */
            svoVar_bByPassFOKCheck = FALSE;
        }
        send_msg5S(SHOW_DEBUG_MSG,"ACTDISS FOC INT !!");
        svoMiscVar.forceFocusOffFlag = TRUE;
        HAL_DSP_ACTDISSReset(); //temp solution to avoid large accumulation of INT
        HAL_DSP_DisDissInt();
        servo_status = servo_status & (~SERVO_STATUS_ACTDISS_FOC);
        ADICounter++;
    }
    if (servo_status & SERVO_STATUS_ACTDISS_TRK)  //TRK ACTDISS
    {
        track_off();
        svoMiscVar.forceTrackOffFlag = TRUE;
        send_msg5S(SHOW_DEBUG_MSG,"ACTDISS TRK INT !!");
        HAL_DSP_ACTDISSReset(); //temp solution to avoid large accumulation of INT
        HAL_DSP_DisDissInt();
        servo_status = servo_status & (~SERVO_STATUS_ACTDISS_TRK);
        ADICounter++;
    }
#endif

    /* FOK check */
    if (svoVar_bByPassFOKCheck == FALSE)
    {
        if (((svoIramVar.switchNextTrnFlag? svoIramVar.svoPreInState : svoIramVar.svoInState) & TRN_MASK) >= FOK_CHK_TRN)
        {
            Fok_Check();
        }
    }

    if (svoSpeedVar.SpdMonitor)
    {
        spdMonitorRPM();
        svoSpeedVar.SpdMonitor = FALSE;
    }

    svoEmergencycheck();

    if (delay_timer_check() == Busy) return;

    i = ((svoIramVar.svoInState & STAT_MASK) >> 12);
    j = ((svoIramVar.svoInState & TRN_MASK ) >>  5);

    if (svoIramVar.svoInState != svoIramVar.svoPreInState)
    {
        if ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) != (svoIramVar.svoPreInState & (STAT_MASK | TRN_MASK)))
            svoMiscVar.searchRFCount = 0;

#if (MSG_OUT_MODE != MSG_DISABLE)
    #if 0 /* DEBUG code designed to monitor in which STAT, TRN, and SEQ a register or variable is changed */
        {
            static LONG pre_value;
            LONG        cur_value;

            cur_value = READ_REG(HAL_AFEREG_MAP->FCTS36);    //register or variable you want to trace
            if (cur_value != pre_value)
            {
                pre_value = cur_value;

                // Debug monitor0 = 0x%08x, STAT: %d TRN: %d SEQ: %d
                SendMsgCnB(SHOW_DEBUG_MSG,7,0x3B0400,B4B(cur_value),B1B(((svoIramVar.svoPreInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoPreInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoPreInState & SEQ_MASK) + 1));
            }
        }
    #endif
    #if 0 /* DEBUG code designed to monitor in which STAT, TRN, and SEQ a register or variable is changed */
        {
            static LONG pre_value1;
            LONG        cur_value1;

            cur_value1 = READ_FIELD(HAL_AFE_SPP1BG);    //register or variable you want to trace
            if (cur_value1 != pre_value1)
            {
                pre_value1 = cur_value1;
                // Debug monitor1 = 0x%08x, STAT: %d TRN: %d SEQ: %d
                SendMsgCnB(SHOW_DEBUG_MSG,7,0x3B0401,B4B(cur_value1),B1B(((svoIramVar.svoPreInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoPreInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoPreInState & SEQ_MASK) + 1));
            }
        }
    #endif
    #if 0 /* DEBUG code designed to monitor in which STAT, TRN, and SEQ a register or variable is changed */
        {
            static LONG pre_value2;
            LONG        cur_value2;

            cur_value2 = READ_FIELD(HAL_AFE_RPOS);    //register or variable you want to trace
            if (cur_value2 != pre_value2)
            {
                pre_value2 = cur_value2;
                // Debug monitor2 = 0x%08x, STAT: %d TRN: %d SEQ: %d
                SendMsgCnB(SHOW_DEBUG_MSG,7,0x3B0402,B4B(cur_value2),B1B(((svoIramVar.svoPreInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoPreInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoPreInState & SEQ_MASK) + 1));
            }
        }
    #endif

    #if 0 /* DEBUG code designed to measure the static error of spindle control */
        {
            static BYTE  pre_value5;
            static ULONG meas_sum,meas_cnt;
            float lost_factor;
            BYTE         cur_afcg;

            meas_sum += READ_REG(HAL_SRVREG_MAP->REVCNT);
                meas_cnt++;

            if (pre_value5 != (BYTE)(Iram_system_timer >> 10))
            {
                pre_value5 = (BYTE)(Iram_system_timer >> 10);

                        meas_sum /= ((DISK_FR2X + 1) * DISK_NFGPLS);
                        lost_factor = READ_REG(HAL_SRVREG_MAP->FGCMP);
                cur_afcg = READ_FIELD(HAL_SRV_AFCG);
                if (cur_afcg > 0)
                {
                    lost_factor += (1 << (cur_afcg - 1));
                }

                if (meas_cnt != 0)
                {
                    lost_factor = ((float)meas_sum / meas_cnt - lost_factor) * READ_FIELD(HAL_SRV_MDIV) / CAVM_REF;
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x3B0411,F4B(lost_factor)); // lost_factor = %4.2f
                    meas_sum = 0;
                    meas_cnt = 0;
                }
            }
        }
    #endif

        //>>>Send Svo Machine Flow Message start<<<<
        if (((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) != (STAT_NORMAL | TRN_062)) &&
            ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) != (STAT_NORMAL | SVO_PAUSE)) &&
            ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) != (STAT_NORMAL | TRN_056)) &&
            ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) != (STAT_NORMAL | SVO_PLAY)))
        {
            if ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) != (svoIramVar.svoPreInState & (STAT_MASK | TRN_MASK)))
            {
                // (Pre)STAT:%d TRN:%d SEQ:%d =>(Cur)STAT:%d TRN:%d SEQ:%d
                SendMsgCnB(STATE_MACHINE_FLOW_MSG,6,0x080100,B1B(((svoIramVar.svoPreInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoPreInState & TRN_MASK) >> 5)+ 1),B1B((svoIramVar.svoPreInState & SEQ_MASK) + 1),
                                                            B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK) >> 5)+ 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));

                // i=0 (NORMAL), i=1 (ERROR), i=2(FOCUS_DROP)
                SendMsg80(STATE_MACHINE_FLOW_MSG,0x020001+(i<<8)+j);
                // System Timer: %4d (mS)
                SendMsgCnB(STATE_MACHINE_FLOW_MSG,4,0x300056,B4B(Iram_system_timer));
            }
            // >>>SEQ=%1d
            SendMsgCnB(STATE_MACHINE_FLOW_MSG,1,0x30001F,B1B((svoIramVar.svoInState & SEQ_MASK)+1));

    #ifdef JTT
            bpt=(unsigned short)((((j+1)/10)<<12)+(((j+1)%10)<<8)+((((svoIramVar.svoInState&SEQ_MASK)+1)/10)<<4)+(((svoIramVar.svoInState&SEQ_MASK)+1)%10));
            if(READ_ADR_2B(0xb00009e0) == bpt)
            {
                set_BPT(bpt);
            }
    #endif
        }
        //>>> end <<<
#endif  // #if (MSG_OUT_MODE != MSG_DISABLE)
    }

#if (SHOW_DETAIL_SERVO_PROCESS == 1)
    if (lastk != ((svoIramVar.svoInState & STAT_MASK)>>12)){
        send_msg5SValue(SHOW_DEBUG_MSG,"STA    =", 2, ((((((svoIramVar.svoInState & STAT_MASK)>>12)+1)/10)<<4)+((((svoIramVar.svoInState & STAT_MASK)>>12)+1)%10))); //andy show BCD
        lastk=((svoIramVar.svoInState & STAT_MASK)>>12);
    }
    if (lastj != ((svoIramVar.svoInState & TRN_MASK)>>5)){
        send_msg5SValue(SHOW_DEBUG_MSG,"TRN  =", 2, ((((((svoIramVar.svoInState & TRN_MASK)>>5)+1)/10)<<4)+((((svoIramVar.svoInState & TRN_MASK)>>5)+1)%10))); //andy show BCD
        send_msg5SValue(SHOW_DEBUG_MSG,"SEQ=", 2, (((((svoIramVar.svoInState & SEQ_MASK)+1)/10)<<4)+(((svoIramVar.svoInState & SEQ_MASK)+1)%10))); //andy show BCD
        lastj=((svoIramVar.svoInState & TRN_MASK)>>5);
        lasti=(svoIramVar.svoInState & SEQ_MASK);
        send_msg5L(1,READ_REG(HAL_WBLREG_MAP->ATPLLFRQ));
    }
    if (lasti != (svoIramVar.svoInState & SEQ_MASK)){
        send_msg5SValue(SHOW_DEBUG_MSG,"SEQ=", 2, (((((svoIramVar.svoInState & SEQ_MASK)+1)/10)<<4)+(((svoIramVar.svoInState & SEQ_MASK)+1)%10))); //andy show BCD
        lasti=(svoIramVar.svoInState & SEQ_MASK);
        send_msg5L(1,READ_REG(HAL_WBLREG_MAP->ATPLLFRQ));
    }
#endif   // #if (SHOW_DETAIL_SERVO_PROCESS == 1)

    svoSeq((trn_tbl[i][j].seq + (svoIramVar.svoInState & SEQ_MASK)));

    if ((svoIramVar.svoInState & SEQ_MASK) == SEQ_END)
    {
        /* Sequence End */

        svoIramVar.svoInState = (trn_tbl[i][j].next_trn_no | SEQ_001);  /* YES:Next TRN_NO Set */
        svoIramVar.switchNextTrnFlag = 1;

#if (SHOW_DETAIL_SERVO_PROCESS == 1)
        if (lastj != ((svoIramVar.svoInState & TRN_MASK)>>5)){
            send_msg5SValue(SHOW_DEBUG_MSG,"TRN  =", 2, ((((((svoIramVar.svoInState & TRN_MASK)>>5)+1)/10)<<4)+((((svoIramVar.svoInState & TRN_MASK)>>5)+1)%10))); //andy show BCD
            send_msg5SValue(SHOW_DEBUG_MSG,"SEQ=", 2, (((((svoIramVar.svoInState & SEQ_MASK)+1)/10)<<4)+(((svoIramVar.svoInState & SEQ_MASK)+1)%10))); //andy show BCD
            lastj=((svoIramVar.svoInState & TRN_MASK)>>5);
            lasti=(svoIramVar.svoInState & SEQ_MASK);
            send_msg5L(1,READ_REG(HAL_WBLREG_MAP->ATPLLFRQ));
        }
        if (lasti != (svoIramVar.svoInState & SEQ_MASK)){
            send_msg5SValue(SHOW_DEBUG_MSG,"SEQ=", 2, (((((svoIramVar.svoInState & SEQ_MASK)+1)/10)<<4)+(((svoIramVar.svoInState & SEQ_MASK)+1)%10))); //andy show BCD
            lasti=(svoIramVar.svoInState & SEQ_MASK);
            send_msg5L(1,READ_REG(HAL_WBLREG_MAP->ATPLLFRQ));
        }
#endif  // #if (SHOW_DETAIL_SERVO_PROCESS == 1)

#if (MONITOR_OPU_TEMPERATURE == 1)
        if ((ReadTimer(&svoReadTempTimer) > 30000L) && ((svoIramVar.svoInState & TRN_MASK) >= FOK_CHK_TRN) &&
            ((svoIramVar.svoInState & TRN_MASK) != SVO_PLAY) && ((svoIramVar.svoInState & TRN_MASK) != TRN_056))
        {
            StartTimer(&svoReadTempTimer);          /* Init Measure Temperature Timer */
            Temperature = GetOpuTemperature();
            if (OpuTemperature != Temperature)
            {
                OpuTemperature = Temperature;
    #if ((COLLI_SWITCH == 1) && (COLLI_TEMP_SLOPE != 0))
        #if (OPU != KEM350AAA)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        #endif
                {
                    // OPU_Temperature = %2d (degree Celsius), colli current position = %2d, colli compensated = %1d
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x38001C,B1B(OpuTemperature),B2B(svoCalColliVar[Layer_Index].opt_value),B1B((OpuTemperature - svoCalColliVar[Layer_Index].temperature) * COLLI_TEMP_SLOPE));
                    svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(Layer_Index));
        #if (OPU == BDPL2)
                    HAL_LASC_SetAPCTarget(calGetOptColliPos(Layer_Index));
        #endif
                }
    #endif
                SendMsgCn(SHOW_DEBUG_MSG,1,0x38001D,OpuTemperature);    // OPU_Temperature = %2d (degree Celsius)
            }
        }
#endif /* MONITOR_OPU_TEMPERATURE=1 */
    }
    else
    {
        svoIramVar.switchNextTrnFlag = 0;
    }

    if ((svoSpeedVar.SpdDown) && (svoSpeedVar.SpdInRange) && (svoSpeedVar.SpdHold == FALSE) &&
        ((svoIramVar.svoInState & TRN_MASK) != SVO_PLAY) && ((svoIramVar.svoInState & TRN_MASK) != TRN_056))
    {
        svoSpeedVar.SpdDown = FALSE;
        //SetDfeWblFrontOnHold();//from 720
        spdSetPara();
        SetDfeWblFrontSemiSynchrAfterTracking();
        SendMsgCnB(SHOW_DEBUG_MSG,2,0x33102E,B2B(svoSpeedVar.SpdWaitCnt * 20)); //***speed in range (down): %d (mS)***
    }

    if  (((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) == (STAT_NORMAL | TRN_062)) ||
         ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) == (STAT_NORMAL | SVO_PAUSEINI)) ||
         ((svoIramVar.svoInState & (STAT_MASK | TRN_MASK)) == (STAT_NORMAL | SVO_PAUSE)) ||
         (get_SIF(STATE) == svoStEmergencystop))
    {
        svoVar_powerSaveFlag = TRUE;
    }
    else
    {
        svoVar_powerSaveFlag = FALSE;
    }

#if (EN_LS == 1)
    if(svoVar_discKindT.Bit.DiscStd == eDiscLS)
        svoVar_powerSaveFlag = FALSE;
#endif // End of (EN_LS == 1)
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoSeq()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    svoSeq(SEQ_TBL  *p)
{
    USHORT     temp_svoInState;
    seq_exit_t result;

    temp_svoInState = svoIramVar.svoInState;

    svoMiscVar.pSeqArgs = &(p->seq_arg);

    result = p->func();            /* Call Function State_XXX_XXX() */

    svoIramVar.svoPreInState = temp_svoInState;

    if (result != SEQ_BUSY)
    {
/* If absolute TRN change required: enter seq_tbl entry with (ABS_TRN|TRN_xxx|SEQ_yyy)  */
        if ((p->next_states_no[result] & ABS_TRN) == ABS_TRN)
        {
            svoIramVar.svoInState = (p->next_states_no[result])&(~ABS_TRN);
        }
#if (MSG_OUT_MODE != MSG_DISABLE)
        else if (p->next_states_no[result] == SEQ_ILL)
        {
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3B0403,B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5)+ 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1),B1B(result));
        }
#endif
        else
        {
            svoIramVar.svoInState = ((svoIramVar.svoInState & (~ SEQ_MASK)) | (p->next_states_no[result] & (~ TRN_MASK)));
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoEmergencycheck()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    svoEmergencycheck(void)
{
    if((svoIramVar.emergencyStopFlag == FALSE) && (svoIramVar.knlTimeupCount != 0))
    {
        return;
    }
    if(svoIramVar.knlTimeupCountFlag == TRUE)
    {
        return;
    }
    if(svoStepVar.stepHomeCheck == TRUE)
    {
        return;
    }

    switch(svoIramVar.svoInState & STAT_MASK){
        case ERR_TRN:
            if ((svoIramVar.knlTimeupCount == 0)&&((svoIramVar.svoInState & TRN_MASK) >= READY_TRN))
            {
                break;
            }
            svoIramVar.svoInState = TRN_016 | SEQ_001;
            break;

        case FOCUS_DROP:
            if (((svoIramVar.knlTimeupCount == 0)&&((svoIramVar.svoInState & TRN_MASK) >= READY_TRN))
                ||((svoIramVar.emergencyStopFlag == TRUE)&&((EjectKeyInfo.PressedValid)||(TrayManagerInfo.Command == TRAY_COMMAND_TRAY_OUT))))
            {
                svoSetLDOff();
                SledStop();
                HAL_LASC_SetLaserOn(TRUE);

                svoIramVar.svoInState = SVO_PAUSEINI | SEQ_001;
                break;
            }
            return;

        case STAT_NORMAL:
            if (((svoIramVar.svoInState & TRN_MASK) == TRN_019) && (svoIramVar.knlTimeupCount == 0))
            {
                return;
            }
            if (((svoIramVar.svoInState & TRN_MASK) == TRN_014) && (svoIramVar.emergencyStopFlag == TRUE))
            {
                return;
            }
            svoEmergencystop();
            if ((svoIramVar.svoInState & TRN_MASK) >= FOK_CHK_TRN)
            {
                if((svoIramVar.svoInState & TRN_MASK) >= READY_TRN)
                {
                    svoIramVar.svoInState = TRN_061 | SEQ_001;
                }
                else
                {
                    if((svoIramVar.svoInState & TRN_MASK) >= TRK_ON_TRN)
                    {
                        svoIramVar.svoInState = TRN_051 | SEQ_001;
                    }
                    else
                    {
                        svoIramVar.svoInState = TRN_039 | SEQ_001;
                    }
                }
            }
            else
            {
                svoIramVar.svoInState = TRN_016 | SEQ_001;
            }
            break;
    }

    set_SIF(STATE, svoStEmergencystop); /* set state "svoStEmergencystop" */
    set_SIF(EXECUTIONSTATE, svoReady);  /* set ExecutionState "svoReady" */
    set_SIF(COMMAND, svoCmdIdle);       /* command clr */
    svoCmdVar.svoCommand = CNOP;

    alltimer_stop();
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoEmergencystop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void    svoEmergencystop(void)
{
    switch(svoIramVar.svoInState & TRN_MASK)
    {
        case TRN_002:
        case TRN_017:
            SledStop();
            break;
        case TRN_019:
            svoSetLDOff();
            discstop();
            SledStop();
            break;
        case TRN_029:
            discstop();
            break;
        case TRN_004:
        case TRN_016:
        case TRN_030:
        case TRN_031:
        case TRN_032:
        case TRN_033:
        case TRN_035:
        case TRN_039:
        case TRN_040:
        case TRN_041:
        case TRN_045:
        case TRN_055:
        case TRN_051:
        case TRN_056:
        case TRN_058:
        case SVO_PLAY://TRN_059
        case TRN_061:
        case TRN_062:
        case SVO_PAUSEINI://TRN_063
            break;
        case SVO_TRON://TRN_038
        case SVO_SEEK://TRN_057
        case SVO_JUMP://TRN_060
        case SVO_PAUSE://TRN_064
            //svotrkon();
            break;
        case TRN_046:
            feg_stop();
            break;
        case TRN_047:
            teg_stop();
            break;
        case TRN_003:
            svoSetLDOff();
            break;
        case SVO_INIT://TRN_001
        case TRN_005:
        case TRN_006:
        case TRN_007:
        case TRN_008:
        case TRN_009:
        case TRN_010:
        case TRN_011:
        case TRN_012:
        case SVO_FOUSRCH://TRN_013
        case TRN_020:
        case TRN_021:
        case TRN_023:
        case TRN_024:
        case TRN_025:
        case TRN_036:
            svoSetLDOff();
            discstop();
            break;
        case TRN_048:
            velo_stop();
            break;
        case TRN_015:
        case TRN_018:
            trayoff();
            break;
    }
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
