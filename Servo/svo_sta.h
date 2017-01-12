/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_STA.H
*
* DESCRIPTION
*
*   $Revision: 204 $
*   $Date: 11-03-11 19:20 $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_STA_
#define _SVO_STA_

/* Prototype */
extern seq_exit_t state_cmd_init(void);
extern seq_exit_t state_disc_fg_chk(void);
extern seq_exit_t state_init_end(void);
extern seq_exit_t state_step_init_chk(void);
extern seq_exit_t state_step_home_chk(void);
extern seq_exit_t state_diaCmd_sled_move(void);
extern seq_exit_t state_discedge_chk(void);
extern seq_exit_t state_step_end_chk(void);
#if (CALIBRATION_MANAGEMENT == 1)
extern seq_exit_t state_at_layer_0(void);
#else
extern seq_exit_t state_caltilt_stepmove1(void);
extern seq_exit_t state_caltilt_stepmove2(void);
extern seq_exit_t state_caltilt_check(void);
#endif
extern seq_exit_t state_aj_init(void);
extern seq_exit_t state_dspofs_set(void);
extern seq_exit_t state_focus_init(void);
extern seq_exit_t state_disc_stop(void);
extern seq_exit_t state_trk_recover(void);
extern seq_exit_t state_disc_chg(void);
extern seq_exit_t state_seek_retry(void);
extern seq_exit_t state_nodisc(void);
extern seq_exit_t state_unknowndisc(void);
extern seq_exit_t state_cal_ramp_down(void);
extern seq_exit_t state_focus_move(void);
extern seq_exit_t state_fsrch_chk(void);
extern seq_exit_t state_fok_chk(void);
extern seq_exit_t state_focus_count(void);
extern seq_exit_t state_focus_retry(void);
extern seq_exit_t state_focus_hold(void);
extern seq_exit_t state_disc_chk2(void);
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
extern seq_exit_t state_disc_chk3(void);
#endif
extern seq_exit_t state_focus_err(void);
extern seq_exit_t state_focus_track_off_err(void);
extern seq_exit_t state_focus_off(void);
extern seq_exit_t state_servo_off(void);
extern seq_exit_t state_disc_start(void);
extern seq_exit_t state_disc_err(void);
extern seq_exit_t state_disc_stop_wait(void);
extern seq_exit_t state_disk_svoON(void);
extern seq_exit_t state_disc_off(void);
extern seq_exit_t state_dsp_start(void);
extern seq_exit_t state_dsp_end_chk(void);
extern seq_exit_t state_dsp_stop(void);
extern seq_exit_t state_trksvo_recover(void);
extern seq_exit_t state_track_err(void);
extern seq_exit_t state_trksvo_on(void);
extern seq_exit_t state_pll_init(void);
extern seq_exit_t state_trksvo_chk(void);
extern seq_exit_t state_play_time(void);
extern seq_exit_t state_play_init(void);
extern seq_exit_t state_subq_err(void);
extern seq_exit_t state_sectorID_chk(void);
extern seq_exit_t state_sectorID_chk2(void);
extern seq_exit_t state_dvdram_pp_det(void);
extern seq_exit_t state_dvdram_header_det(void);
extern seq_exit_t state_subq_chk(void);
extern seq_exit_t state_subq_chk2(void);
extern seq_exit_t state_afe_vref_aj1(void);
extern seq_exit_t state_search_rf_zone(void);
extern seq_exit_t state_err_trk_off(void);
extern seq_exit_t state_trk_off(void);
extern seq_exit_t state_tray_in(void);
extern seq_exit_t state_tray_out_wait(void);
extern seq_exit_t state_tray_out(void);
extern seq_exit_t state_tray_in_step_up(void);
extern seq_exit_t state_tray_out_step_dn(void);
extern seq_exit_t state_tray_out_hold(void);
#if(SUN710_TRAY_PROFILE)
extern seq_exit_t state_tray_in_stage1(void);
extern seq_exit_t state_tray_in_stage2(void);
extern seq_exit_t state_tray_in_stage3(void);
extern seq_exit_t state_tray_out_stage1(void);
extern seq_exit_t state_tray_out_stage2(void);
extern seq_exit_t state_tray_out_stage3(void);
#endif
extern seq_exit_t state_tray_stop(void);
extern seq_exit_t state_tray_err(void);
extern seq_exit_t state_lens_shift_cali(void);
#if (MPPBG_SPPBG_CALIB != 0)
extern seq_exit_t state_mpp_sppbg_cali(void);
#endif
extern seq_exit_t state_cmd_new_req(void);
extern seq_exit_t state_fine_start(void);
extern seq_exit_t state_fine_err(void);
extern seq_exit_t state_err_trksvo_on(void);
extern seq_exit_t state_jmpcnt_set(void);
extern seq_exit_t state_jmpcnt_chk(void);
extern seq_exit_t state_jump_init(void);
extern seq_exit_t state_seek_init(void);
extern seq_exit_t state_pause_start(void);
extern seq_exit_t state_pause_time(void);
extern seq_exit_t state_pause_init(void);
#if (BCA_ENABLE == 1)
extern seq_exit_t state_BCA_jump_init(void);
#endif
extern seq_exit_t state_rough_start(void);
extern seq_exit_t state_rough_end_chk(void);
extern seq_exit_t state_slow_chk(void);
extern seq_exit_t state_tok_chk(void);
extern seq_exit_t state_rough_err(void);
extern seq_exit_t state_disc_speed(void);
extern seq_exit_t state_seek_end(void);
extern seq_exit_t state_calib_needed(void);
extern seq_exit_t state_written_chk(void);
extern seq_exit_t state_dif_RF_gain_calib(void);
extern seq_exit_t state_pll_chk(void);
extern seq_exit_t state_layerjump_chk(void);
extern seq_exit_t state_dif_wbl_gain_calib(void);
#if (EN_WOBBLE == 1)
extern void wbl_gain_calib(void);
#endif
extern seq_exit_t state_blank_chk(void);
extern seq_exit_t state_rp_bal_cali(void);
extern seq_exit_t state_rpg_cali(void);
extern seq_exit_t state_teg_cali(void);
extern seq_exit_t state_te_stepsize_meas(void);
extern seq_exit_t state_chk_audio(void);
extern seq_exit_t state_CEBG_cali(void);
extern seq_exit_t state_CEOS_cali(void);
extern seq_exit_t state_vcalc_init(void);
extern seq_exit_t state_velocity_meas(void);
extern seq_exit_t state_boundary_det(void);
extern seq_exit_t state_trackpitch_meas(void);
extern seq_exit_t state_vcalc_err(void);
#if (NEW_RFPOS_CALIB == 1)
extern seq_exit_t state_RFPOS_cali(void);
#endif
extern seq_exit_t state_RFCOS_size_cali(void);
extern seq_exit_t state_afeinof_cali(void);
extern seq_exit_t state_mbsbvrefof_cali(void);
extern seq_exit_t state_ofsadjflag_check(void);
extern seq_exit_t state_ofsadjflag_set(void);
extern seq_exit_t state_fe_pp_meas_init(void);
extern seq_exit_t state_RWdisc_check(void);
#if (DETECT_ECC_DISC == 1)
extern seq_exit_t state_ecc_chk1(void);
extern seq_exit_t state_ecc_chk2(void);
extern seq_exit_t state_ecc_chk3(void);
#endif
extern seq_exit_t state_AFE_InputGainCal(void);
extern seq_exit_t state_STM_start(void);
extern seq_exit_t state_STM_measure(void);
extern seq_exit_t state_STM_end(void);
extern seq_exit_t state_FE_p2p_init(void);
extern seq_exit_t state_FE_gn_adjust(void);
extern seq_exit_t state_BS_gn_adjust(void);
extern seq_exit_t state_FEpp_ReMeas(void);
extern seq_exit_t state_disc_chk_cunt(void);
extern seq_exit_t state_headerID_chk(void);
extern seq_exit_t state_headerID_chk2(void);
extern seq_exit_t state_layer_chk(void);
extern seq_exit_t state_rough_chk(void);
extern seq_exit_t state_fine_chk(void);
extern seq_exit_t state_layerjump_start(void);
extern seq_exit_t state_layerjump_err(void);
extern seq_exit_t state_pp_det(void);

// for Servo Offline calibration.
extern seq_exit_t state_select_cailMode(void);
extern seq_exit_t state_FEBG_cal(void);
extern seq_exit_t state_FEBC_TILT_cal(void);
extern seq_exit_t state_diaCmd_RRFGCal(void);
extern seq_exit_t state_diaCmd_BETACal(void);
extern seq_exit_t state_diaCmd_ReadBETA(void);

#if (HORROR_DISC_IMPROVEMENT == 1)
extern USHORT state_RFEQ_chg(void);
#endif

#if (SERVICE_MODE == 1)
extern seq_exit_t state_urJump_init(void);
extern seq_exit_t state_service_cmd_new_req(void);
extern seq_exit_t state_laser_on(void);
extern seq_exit_t state_serv_foc_chk(void);
#endif

#endif /* _SVO_STA_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
