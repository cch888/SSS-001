/****************************************************************************
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Rights Reserved
*
*
* FILENAME:   svo_para_710.H
*
* DESCRIPTION: Servo Parameters for the SUN710 Project
*
*   $Revision: 48 $
*   $Date: 11/03/29 9:41p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_PAR_
#define _SVO_PAR_

//-----------------------------------------------------------------------------;
// Parameters for TRAY_IN_TBL                                                  ;
//-----------------------------------------------------------------------------;
#define TI_KICKT                        MS_100        // time delay to create kick period @ TRAY_IN_LEVEL1
#define TI_TOUT                         SEC_3         // Timeout time for tray-in
#define TI_STP                          MS_20         // time step period to ramp up the tray in voltage
#define TI_HOLD                         MS_100

//TRAY LEVEL (-512(pull) ~ 511(push))
#if (SET_SLOT_IN == 1) //[Q36] for tray in dac
#define TRAY_IN_LEVEL1                  -224
#define TRAY_IN_LEVEL2                  -140 //-210//BP-S28 -155 //disc in level -180
#define R2_TRAY_IN_LEVEL2               -185 //[J50X] for BP-S28R2 USE
#define TRAY_IN_LEVEL3                  -130 //-180//BP-S28 -140 //disc in level -150 -130 [B10F_L] skew issue can't disc in
#define R2_TRAY_IN_LEVEL3               -165 //[J50X] for BP-S28R2 USE
#define R2_TRAY_IN_LEVEL4               -220 //[J5zY] for SLT002 disc in/out test
#elif (Customer_A1 ==1)
#define TRAY_IN_LEVEL1                  -164
#define TRAY_IN_LEVEL2                  -150
#define TRAY_IN_LEVEL3                  -120 
#elif(Customer_Dune_HD ==1)//[J010]Bios
#define TRAY_IN_LEVEL1                  -224
#define TRAY_IN_LEVEL2                  -210
#define TRAY_IN_LEVEL3                  -180 
#define TRAY_IN_LEVEL4                  -250 
#else
#define TRAY_IN_LEVEL1                  -204
#define TRAY_IN_LEVEL2                  -170
#define TRAY_IN_LEVEL3                  -120 
#endif

#define TRAY_IN_STEP_UP                 4

//-----------------------------------------------------------------------------;
// Parameters for TRAY_OUT_TBL                                                 ;
//-----------------------------------------------------------------------------;
#define TO_KICKT                        MS_100        // time delay to create kick period @ TRAY_OUT_LEVEL1
#define TO_TOUT                         SEC_3         // Timeout time for tray-out
#define TO_STP                          MS_23         // time step period to ramp up the tray in voltage
#define TO_HOLD                         MS_35 //MS_65         // Tray-out hold time

//TRAY LEVEL (-512(pull) ~ 511(push))
#if (SET_SLOT_IN == 1) //[Q36] for tray out dac
#define TRAY_OUT_LEVEL1                 160 //[J50B]kinlfd 180 /disc out level 250 265
#define R2_TRAY_OUT_LEVEL1              210 //[J50X] for BP-S28R2 USE 200
#define TRAY_OUT_LEVEL2                 110 //[J50C]110 //130 //disc out level 120
#define R2_TRAY_OUT_LEVEL2              180 //[J50X] for BP-S28R2 USE 150 160
#define R2_TRAY_OUT_LEVEL3              240 //[J5zY] for SLT002 disc in/out test
#elif (Customer_A1 ==1)
#define TRAY_OUT_LEVEL1                 190
#define TRAY_OUT_LEVEL2                 120
#elif(Customer_Dune_HD ==1)//[V08]Bios
#define TRAY_OUT_LEVEL1                 230
#define TRAY_OUT_LEVEL2                 120
#define TRAY_OUT_LEVEL3                 290
#else
#define TRAY_OUT_LEVEL1                 210//288
#define TRAY_OUT_LEVEL2                 140//224
#endif
#if (SET_SLOT_IN == 1)
#define TRAY_OUT_STEP_DN                4
#else
#define TRAY_OUT_STEP_DN                2
#endif
//-----------------------------------------------------------------------------;
// Parameters for DISCKICK_TBL                                                 ;
//-----------------------------------------------------------------------------;
#define FEpp_MEAS_RPM                   1050          // (rpm) Spindle rotation speed for FE pp measure (RPM)
#define FEpp_MEAS_RPM_DVDRAM            2400          // (rpm) Spindle speed for FE PP measurement for DVD-RAM

#define DISCK_TOUT                      MS_800        // Time to kick disc for disc detection
#define DISC_KICK_MAX                   0x4000        // 0x4000-full power
#define DISC_KICK_FORCE                 0x4000        // Kd14 (for disc detection), 0x4000-full power

#define DISC_8CM_TH                     6.5           // acceleration RPM/mS
#define NO_DISC_TH                      36.0          // acceleration RPM/mS

#define DMO_CLIP                        (0x80*DMODAC_FACTOR)  // 0x80: full power, clip both acc and dec
#define DMO_CLIP_SLED_MOVING            (0x80*DMODAC_FACTOR)  // 0x80:full power, clip both acc and dec during sledge and colli is moving

#define SPD_IN_RANGE_LIMIT_X_1000       50            // 5%
#define SPD_IN_RANGE_MAX_DIFF_RPM       240           // RPM
#define SPD_IN_RANGE_MIN_DIFF_RPM       120           // RPM//20101027_02RW ==start== Modify speed in range threshold to prevent speed overshoot at very low RPM

#define SPD_MAX_RPM_SLOPE_LIMIT         200           // RPM/Sec //20110307_036F == add SPD_MAX_RPM_SLOPE_LIMIT for spdDiscSpeedWaitStable()

// Use slower acceleration or deceleration to reduce maximum current
#define ACC_CONTROL                     0             // RPM = ACC_CONTROL * TIME(ms)^0.5, set ACC_CONTROL = 0: no ACC control
#define DEC_CONTROL                     0             // RPM = DEC_CONTROL * (K-TIME(ms))^0.5, set DEC_CONTROL = 0: no DEC control

#define SPD_MAX_BRAKE_RPM               8000          // RPM <= SPD_MAX_BRAKE_RPM:short brake; RPM > SPD_MAX_BRAKE_RPM:free run
                                                      // SPD_MAX_BRAKE_RPM = 0:always short brake

#if ((BD_2X_STARTUP == 1)&&(BD_UNBALANCE_DISC_CHECK))
#define UNB_MAX_RPM                     2800          //define UNB max rpm during start up
#endif
//-----------------------------------------------------------------------------;
// Parameters for CDDVD_DET_TBL                                                ;
//-----------------------------------------------------------------------------;
// Parameters for FOO ramping
#define FOCUS_SEARCH_KF18               0x18CC        // Kf18 (RAM address 0x28)

#define FOC_FSR                         0             // increase FOC_FSR to use higher KF18 without changing focus ramp velocity
#if (FOC_FSR == 0)
#define FOC_RAMP_FACTOR                 471
#elif (FOC_FSR == 1)
#define FOC_RAMP_FACTOR                 353
#elif (FOC_FSR == 2)
#define FOC_RAMP_FACTOR                 235
#else
#define FOC_RAMP_FACTOR                 177
#endif
//#if (FOOFFSET2_ENABLE == 1)
//#define FOO_OFFSET_LIMIT                511
//#else
//#define FOO_OFFSET_LIMIT                127
//#endif
    #if (CHIP_REV >= 0xC0)
#define FOO_OFFSET_LIMIT                511
#else
#define FOO_OFFSET_LIMIT                127           // if C0 then maximum = 511 else maximum = 127
#endif



//TBD:need check ,this function still has problem
#define STM_CD_PROTECTION_RETRY         1             // To avoid missing substrate for CD
#define STM_DVD_PROTECTION_RETRY        1             // To avoid missing substrate for DVD
#define STM_BD_PROTECTION_RETRY         1             // To avoid missing substrate for BD

#define F_IDEN_DN_T_BD                  7             // Time to move lens down for disc detection.
#define F_IDEN_UP_T_BD                  102           // Time to move lens up for disc detection.
#define F_IDEN_DN_T_DVD                 86//76 //[W.0E]           // Time to move lens down for disc detection.
#define F_IDEN_UP_T_DVD                 150//144 //[W.0E]          // Time to move lens up for disc detection.
#define F_IDEN_DN_T_CD                  85//[W.0C]72            // Time to move lens down for disc detection.
#define F_IDEN_UP_T_CD                  165//[W.0C]152           // Time to move lens up for disc detection.
#define F_IDEN_SENSITIVITY_BD           16667         // unit:nm/dac
#define F_IDEN_SENSITIVITY_HD           (3300 / 1024 * 12 * 0.22 * 2200 / 0.54 * 0.87) // unit:nm/dac
#define F_IDEN_SENSITIVITY_DVD          26000         //30000//[S08] daniel for DVD-9 judge wrong disc          // unit:nm/dac
#define F_IDEN_SENSITIVITY_CD           38000//[W.0C]34286         // unit:nm/dac
#define F_SEARCH_T                      MS_20         // Maximum time for S-curve searching
#define LAYER01_ELAPSED_TIME            MS_15         // Time to continue to search for the 2nd s-curve after finding the 1st s-curve
#define DVDDL_FE_TH                     220
#define LAYER_DIFF_T                    3             // L0&L1 difference time
#if DDisc ==1
#if(CEProduct == 1)
#define DualDisc_STM_DVDLD_TH           850
#else
#define DualDisc_STM_DVDLD_TH           670
#endif
#define DualDisc_STM_CDLD_TH            750
#define DualDisc_TE_P2P_TH              300
#define FESRFO_ratio_TH                 1.6
#endif  //DDisc ==1
#define MIN_STM_FEpp_BD                 250
#define MIN_STM_FESRFO_RATIO_BD         2.0
#define MIN_STM_FEpp_DVD                350 //0
#define MIN_STM_FESRFO_RATIO_DVD        1.4 //0
#define MIN_STM_FEpp_CD                 180 //0
#define MIN_STM_FESRFO_RATIO_CD         2.0 //0
#if (NODISC_RETRY == 1)
#define STM_MAX_RETRIES                 1
#else
#define STM_MAX_RETRIES                 1
#endif
//-----------------------------------------------------------------------------;
// Parameters for AJINIT_TBL                                                   ;
//-----------------------------------------------------------------------------;
#define F_CAL_DN_T                      MS_100        // Time to pull lens donw for servo signals offset calibration

//-----------------------------------------------------------------------------;
// Parameters for FDP_TRKSLIP_MONITOR                                                   ;
//-----------------------------------------------------------------------------;
#if (FDP_TRKSLIP_MONITOR == 1)
#define FORCE_FCS_OFF_TH               16//TBD:need to check later
#define FORCE_TRK_OFF_TH               80
#endif
//-----------------------------------------------------------------------------;
// Parameters for FE_PP_MEASURE_TBL                                            ;
//-----------------------------------------------------------------------------;
// PP measure
#define FOCUS_PP_READ_COUNT             8             // Times to measure FE PP value
#define F_CAL_DN_TOUT                   (F_CAL_DN_T + MS_50)
#define F_CAL_DN_T3                     (F_CAL_DN_T - F_SEARCH_T)
    #if (SACD == 1)
#define SACD_F_PP_DELAY_T               MS_60         // Time to continue to move lens after found the 1st S-curve
    #endif
#define F_PP_DELAY_T                    MS_9          // Time to continue to move lens after found the 1st S-curve

// Settings for noise filter
#define FILTER_POINT                    1             // Points to filter
#define FE_SMSTEP_TH                    50            // S-curve threshold, must higher than substrate's S-curve.
#define FE_SMSTEP_TH_CD                 50            // S-curve threshold, must higher than substrate's S-curve.
#define FE_SMSTEP_TH_RAM                50            // S-curve threshold for RAM, must higher than substrate's S-curve.
#define FE_SMSTEP_TH_BD                 150           // S-curve threshold, must higher than substrate's S-curve.
#define FE_SMSTEP_TH_BDDL               50            // S-curve threshold for BD DL, must higher than substrate's S-curve.
#define FE_MAX_SLEW_RATE                12            // Maximum allowable slwe-rate (only for DVD discs)
    #if (DVD_RAM_READ == 1)
        #if (FE_SLEW_RATE_LIMITER == 1)
#define FE_MAX_SLEW_RATE_RAM            22            // Maximum allowable slwe-rate (only for DVD RAM)
        #elif (FE_SLEW_RATE_LIMITER == 2)
#define HW_FE_SLEW_RATE                 0x0A0108
        #endif
    #else
#define HW_FE_SLEW_RATE                 0x0A0108
    #endif
#define RATIO_TO_MAXFEPP                4          //improve FEpp measurement for BDR(E) DL

#define FE_FACTOR_BD_L0                 2.4
// Settings to compensate BD L1 measurement results because colli position is set to L0
#define FE_FACTOR_BD_L1                 2.8
#define BS_FACTOR_BD_L1                 1.4
#define SRFO_FACTOR_BD_L1               1.2

//-----------------------------------------------------------------------------;
// Parameters for MBEAMOF_TBL, SBEAMOF_TBL, FEOF_TBL, TEOF_TBL and BSOF_TBL    ;
//-----------------------------------------------------------------------------;
#define MAX_OFFSET_LOOP                 20            // Maximum loop for offset calibration
#if (MPPBG_SPPBG_CALIB == 0)
#define MPPBG_SPP1BG_MAX_UNB            0             // (0.0 dB) Maximum unbalance for MPPBG and SPP1BG calibration
#else
#define MPPBG_SPP1BG_MAX_UNB            4             // (2.0 dB) Maximum unbalance for MPPBG and SPP1BG calibration
#endif
#if (DVD_RAM_READ == 1)
#define MPPBG_MAX_UNB                   8             // (0.5 dB) Maximum unbalance for MPPBG calibration
#define SPP1BG_MAX_UNB                  8             // (0.5 dB) Maximum unbalance for SPP1BG calibration
#else
#define MPPBG_MAX_UNB                   10            // (0.5 dB) Maximum unbalance for MPPBG calibration
#define SPP1BG_MAX_UNB                  10            // (0.5 dB) Maximum unbalance for SPP1BG calibration
#endif

//-----------------------------------------------------------------------------;
// Parameters for FOUSRCH_TBL                                                  ;
//-----------------------------------------------------------------------------;
#define FSRCH_RETRY                     3             // Focal point search retry count
#define FOCUS_RETRY                     8             // Focus servo on retry count
#define FOCUS_CAPTURE_FSL_BD            57            // BD needs larger FSL to prevent focus on substrate
#define FOCUS_CAPTURE_FSL_CD_DVD        40
#define F_RETRY_SPIN_DOWN_RPM           300
#define F_RETRY_MIN_RPM                 600

//-----------------------------------------------------------------------------;
// Parameters for FDP_TBL                                                ;
//-----------------------------------------------------------------------------;
#if (NEW_FDP_DETECTION==1)//focus drop move lens down 1.24mm
#define FDP_FOO_OS                      -66           // Focus drop pull lens down foo offset
#endif

    #if (CHIP_REV >= 0xC0)
#define FDP_PROT_PULSE_TIME             0             // (100uS)

#define SCRATCH_PROT_FOO_CD             -256
#define SCRATCH_PROT_FOO_DVD            -256
#define SCRATCH_PROT_FOO_BD             -280
#define SCRATCH_PROT_FOO_HD             -192
#define SCRATCH_PROT_TIME               50//50 TBD:need new DSP code             // (mS)
    #endif
#define F_RETRY_SPIN_DOWN_RPM           300

#define FDP_STABLE_TIME                 100           // Hold Lens ? ms before ramping

//-----------------------------------------------------------------------------;
// Parameters for DISCKINDS_DET_TBL                                            ;
//-----------------------------------------------------------------------------;
#define DVDROM_TH                       0.35          // Threshold ratio to judge DVD R discs //adjust DVDROM_TH, DVDROMDL_TH
#define DVDROMDL_TH                     0.30          // Threshold ratio to judge DVD R DL discs //adjust DVDROM_TH, DVDROMDL_TH
#if (DETECT_CDR == 1)
#define CDROM_TH                        1.20          // If CDROM/R's MPPO/SRFO is more than this threshold, then judged as CDR
#endif
#define BDROM_TH                        0.75 //0.50   // Threshold ratio to judge BD ROM discs
#define BDROMDL_TH                      0.60          // Threshold ratio to judge BD ROM DL discs
#define BD_WRONG_LAYER_TH               0.12          // Threshold ratio to judge BD wrong layer
#define BDRLTH_TH                       1.00          // Threshold ratio to judge BDR LTH discs
#if ((BD_2X_STARTUP == 1)&& (BD_UNBALANCE_DISC_CHECK==1))
#define UNBBD_TH                        500           //define UNB check threshold for 2x start_up
#endif
#if (BD_UNBALANCE_DISC_CHECK == 1)
#define BD_UNB_CHK_TH                   200           // Threshold  CE level to detect unbalance disc
#endif
//-----------------------------------------------------------------------------;
// Parameters for BLANK_TBL                                                    ;
//-----------------------------------------------------------------------------;
// Settings for DVD-RAM detection
#define DVDRAM_TH                       1.25          // Threshold ratio to judge DVD-RAM discs
#define DVDRAM_HEADER_DELTA             10            // (%) Header delta to judge DVD-RAM discs

// Settings for blank detection
#define BLANK_PRD                       MS_2          // Period to check blank
#define BLANK_CNT                       50            // Times to check blank
#define RECD_CNT                        20            // Recorded disc judge count

//-----------------------------------------------------------------------------;
// Parameters for SPPAJ_TBL                                                    ;
//-----------------------------------------------------------------------------;
// Settings for TEBG adjustment
#define LENS_SHIFT_UM                   300           // (um) Lens shift for TEBG calibration
#define LENS_SHIFT_T                    450           // (100uS) Time to shift lens
#define TEBG_CAL_BEGIN_POINT            26            // Begin-point for TEBG calibration
#define TEBG_CAL_END_POINT              6             // End-point for TEBG calibration
#define TEBG_CAL_STEP                   -4            // TEBG step

#define TROOS_LENS_LIMIT_CDDVD          70            // Maximum allowed lens shift for TRO offset calibration
#define TROOS_LENS_LIMIT_BDHD           70            // Maximum allowed lens shift for TRO offset calibration

#define CE_SEN_TARGET_MV_MM             2000          // mV/mm

#if DDisc ==1
#define CD_DUAL_TH                      0.23
#endif

#if (SUPPORT_DPD == 1)
// Settings for cd_dpd detection
#define CD_DPD_TP_TH                    0.41          // Threshold track-pitch-factor to judge as a cd_dpd disc
#define Beall_DPD_TP_TH                 0.70          // Threshold track-pitch-factor to judge as a cd_dpd disc
#define Beall_SRFOpp_0dB_MinTH          155//180
#define Beall_SRFOpp_0dB_MaxTH          500
#endif

#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
#define MIN_BLANK_CD_FESRFO_RATIO       0.7
#define MIN_BLANK_DVD_FESRFO_RATIO      0.9
#endif

//-----------------------------------------------------------------------------;
// Parameters for TGAFE_TBL                                                    ;
//-----------------------------------------------------------------------------;
// Setting for TE Gain and Offset Adjust
#define TEGNOS_RECD_CHK_CNT             10            // Count to check RECD
#define TEGNOS_RECD_CHK_TH              5             // Threshold count to judge as a recorded track
#define TEGNOS_RECD_CHK_TH_DVDDPD       8             // Threshold count to judge as a recorded track for DVDDPD//Skip DPD TEGN/OS calibration for DVD if touch blank zone.
#define TEGNOS_OFFSET_STEP              48            // Step for offset adjust (@ 0 dB)
#define TEGNOS_MAX_LOOP                 10            // Maximum loop for TE gain and offset calibration
#define TEGNOS_DB_REDUCE_AT_SAT         3             // (dB) Gain reduced when saturation happens
#define TEGNOS_MAX_SAMPLES              6             // Maximum number of sampling for TEGNOS calibration
#define TEGNOS_MIN_SAMPLES              3             // Minimum number of sampling for TEGNOS calibration

// Settings for TE Gain for embossed area
#define TRKG_EMBOSSED                   16            // (0 dB) TE Preset Gain for embossed area

// Settings for FEBC and Tilt calibration
#define FEBC_TILT_RECD_CHK_CNT          10            // Count to check RECD
#define FEBC_TILT_RECD_CHK_TH           3             // Threshold count to judge as a recorded track
#define FEBC_TILT_MAX_SAMPLES           20            // Maximum number of sampling for FEBC and TILT calibration
#define FEBC_TILT_MIN_SAMPLES           7             // Minimum number of sampling for FEBC and TILT calibration
#define FEBC_TILT_JITTER_DELTA_COST     0.125         // Delta from minimum cost
#define FEBC_TILT_TE_P2P_DELTA_COST     0.07143       // Delta from maximum cost
#define FEBC_TILT_RF_P2P_DELTA_COST     0.07143       // Delta from maximum cost
#define FEBC_TILT_RF_P2P_MIN            200           // Minimum RF P2P of written disc

#define CAL_ZONE_SWITCH_POINT01         25800L  //Switch point of Zone00 and Zone01 DVD5 0x042040 DVD9 0x04061A
#define CAL_ZONE_SWITCH_POINT02         27625L  //Switch point of Zone01 and Zone02 DVD5 0x0559D1 DVD9 0x052339
#define CAL_ZONE_SWITCH_POINT03         29942L  //Switch point of Zone02 and Zone03 DVD5 0x0706B5 DVD9 0x06A936
#define CAL_ZONE_SWITCH_POINT04         32092L  //Switch point of Zone03 and Zone04 DVD5 0x08B39A DVD9 0x082F33
#define CAL_ZONE_SWITCH_POINT05         34107L  //Switch point of Zone04 and Zone05 DVD5 0x0A607F DVD9 0x09B530
#define CAL_ZONE_SWITCH_POINT06         36009L  //Switch point of Zone05 and Zone06 DVD5 0x0C0D64 DVD9 0x0B3B2D
#define CAL_ZONE_SWITCH_POINT07         38254L  //Switch point of Zone06 and Zone07 DVD5 0x0E2582 DVD9 0x0D22AA
#define CAL_ZONE_SWITCH_POINT08         40375L  //Switch point of Zone07 and Zone08 DVD5 0x103DA0 DVD9 0x0F0A26
#define CAL_ZONE_SWITCH_POINT09         42389L  //Switch point of Zone08 and Zone09 DVD5 0x1255BE DVD9 0x10F1A2
#define CAL_ZONE_SWITCH_POINT10         44312L  //Switch point of Zone09 and Zone10 DVD5 0x146DDC DVD9 0x12D91F
#define CAL_ZONE_SWITCH_POINT11         46155L  //Switch point of Zone10 and Zone11 DVD5 0x1685FA DVD9 0x14C09B
#define CAL_ZONE_SWITCH_POINT12         47927L  //Switch point of Zone11 and Zone12 DVD5 0x189E19 DVD9 0x16A817
#define CAL_ZONE_SWITCH_POINT13         49299L  //Switch point of Zone12 and Zone13 DVD5 0x1A4AFD DVD9 0x182E14
#define CAL_ZONE_SWITCH_POINT14         50633L  //Switch point of Zone13 and Zone14 DVD5 0x1BF7E2 DVD9 0x19B411
#define CAL_ZONE_SWITCH_POINT15         51934L  //Switch point of Zone14 and Zone15 DVD5 0x1DA4C7 DVD9 0x1B3A0E
#define CAL_ZONE_SWITCH_POINT16         53202L  //Switch point of Zone15 and Zone16 DVD5 0x1F51AC DVD9 0x1CC00B
#define CAL_ZONE_SWITCH_POINT17         54441L  //Switch point of Zone16 and Zone17 DVD5 0x20FE91 DVD9 0x1E4608
#define CAL_ZONE_SWITCH_POINT18         55653L  //Switch point of Zone17 and Zone18 DVD5 0x22AB76 DVD9 0x1FCC05
#define CAL_ZONE_SWITCH_POINT19         56839L  //Switch point of Zone18 and Zone19 DVD5 0x24585B DVD9 0x215202

#define CAL_RADIUS_TEST_POINT0          26100L        // (26.000mm)  Test point 0
#define CAL_RADIUS_TEST_POINT1          33800L        // (33.800mm)  Test point 1
#define CAL_RADIUS_TEST_POINT2          40550L        // (40.550mm)  Test point 2
#define CAL_RADIUS_TEST_POINT3          48300L        // (48.300mm)  Test point 3
#define CAL_RADIUS_TEST_POINT4          55000L        // (55.000mm)  Test point 4
#define PIC_RADIUS_MIDDLE               22800L        // (22.8mm)
#define PZONE2_RADIUS_MIDDLE            23270L        // ((23.252 + 23.289) / 2 mm)
#define PIC_L0_MAX_ADDR                 0x000DA400
#define PIC_L1_MIN_ADDR                 0x01F25C00

#if (CALIBRATION_MANAGEMENT == 0)
#define CAL_DPDCTFFC_TEST_POINT0        0x88          // DPD CTF Fc setting for test point 0
#define CAL_DPDCTFFC_TEST_POINT1        0x90          // DPD CTF Fc setting for test point 1
#define CAL_DPDCTFFC_TEST_POINT2        0x96          // DPD CTF Fc setting for test point 2
#define CAL_DPDCTFFC_TEST_POINT3        0x9C          // DPD CTF Fc setting for test point 3
#define CAL_DPDCTFFC_TEST_POINT4        0x9F          // DPD CTF Fc setting for test point 4
#endif

// Settings for Tilt calibration
#define TILT_RECAL_TIME                 600000        // (10 min) Time for Redoing Tilt calibration
#define TILT_SKIP_TIME                  60000         // ( 1 min) Time for Skipping Tilt calibration
#define TILT_DIFF_PER_ZONE              20            // maximum tilt difference between neighbor zones

#if (FOOTILT_DIFFERENTIAL == 1)
#define TILT_INIT_DAC                   0x0000
#define TILT_MIN_DAC                    -0x01FF       // Minimum Tilt DAC output for Tilt calibration
#define TILT_MAX_DAC                    0x01FF        // Maximum Tilt DAC output for Tilt calibration
#else
#define TILT_INIT_DAC                   0x0200
#define TILT_MIN_DAC                    0x0140        // Minimum Tilt DAC output for Tilt calibration
#define TILT_MAX_DAC                    0x02AF        // Maximum Tilt DAC output for Tilt calibration
#endif
#define TILT_STEP_DAC                   26            // Step for Tilt calibration
#define TILT_STEP_DAC_RAM               26            // Step for Tilt calibration
#define TILT_STEP_DAC_CD                35            // Step for Tilt calibration
#define TILT_STEP_DAC_BD                28            // Step for Tilt calibration
#define MAX_TILT_STEP_PER_100uS         1

#if COLLI_SWITCH ==1
// Settings for Colli calibration
#define COLLI_RECAL_TIME                600000        // (10 min) Time for Redoing Colli calibration
#define COLLI_SKIP_TIME                 60000         // ( 1 min) Time for Skipping Colli calibration
#define COLLI_MIN_POS                   0             // Minimum Colli position for Colli calibration// set colli maximum range
#define COLLI_MAX_POS                   9600          // Maximum Colli position for Colli calibration
#define COLLI_STEP_POS                  224           // Step for Colli calibration
#endif

// Settings for Focus-balance calibration
#define FEBC_RECAL_TIME                 600000        // (10 min) Time for Redoing Focus calibration
#define FEBC_SKIP_TIME                  60000         // ( 1 min) Time for Skipping Focus calibration
#define FEBC_DIFF_PER_ZONE              1024          // maximum FEBC difference between neighbor zones

#if (REPLACE_FEBC_WITH_FEO == 0)
#define FEBC_MIN_DAC                    -16384        // Minimum FEBC DAC output for Focus balance calibration
#define FEBC_MAX_DAC                    16384         // Maximum FEBC DAC output for Focus balance calibration
#define FEBC_STEP_DAC                   1300          // Step for Focus calibration
#define FEBC_STEP_DAC_RAM               1300          // Step for Focus calibration
#define FEBC_STEP_DAC_CD                2000          // Step for Focus calibration
#define FEBC_STEP_DAC_BD                2800          // Step for Focus calibration
#define MAX_FEBC_STEP_PER_100uS         256

#define FEBC_MIN_OPT_DAC                -8000         // Minimum FEBC DAC output
#define FEBC_MAX_OPT_DAC                8000          // Maximum FEBC DAC output
#else
#define FEBC_MIN_DAC                    -256          // Minimum FEBC DAC output for Focus balance calibration
#define FEBC_MAX_DAC                    256           // Maximum FEBC DAC output for Focus balance calibration
#define FEBC_STEP_DAC                   20            // Step for Focus calibration
#define FEBC_STEP_DAC_RAM               20            // Step for Focus calibration
#define FEBC_STEP_DAC_CD                32            // Step for Focus calibration
#define FEBC_STEP_DAC_BD                44            // Step for Focus calibration
#define MAX_FEBC_STEP_PER_100uS         4

#define FEBC_MIN_OPT_DAC                -128         // Minimum FEBC DAC output
#define FEBC_MAX_OPT_DAC                128          // Maximum FEBC DAC output
#endif

#define MAX_FESPT_STEP_PER_10uS         4
// Settings for focus offset calibration for DVDRAM
#define FCSO_MIN_VAL                    -56           // Minimum focus offset value
#define FCSO_MAX_VAL                    56            // Maximum focus offset value
#define FCSO_STEP_VAL                   7             // Step for focus offset calibration
#define MAX_FCSO_STEP_PER_100uS         15

// Settings for track offset calibration
#define TESPT_MIN_VAL                   -256          // Minimum TE set-point
#define TESPT_MAX_VAL                   256           // Maximum TE set-point
#define TESPT_STEP_VAL                  24            // Step for TE set-point calibration
#define MAX_TESPT_STEP_PER_100uS        4

//-----------------------------------------------------------------------------;
// Parameters for RPGAJ_TBL                                                    ;
//-----------------------------------------------------------------------------;
#define RPG_RECD_CHK_CNT                10            // Count to check RECD
#define RPG_RECD_CHK_TH                 5             // Threshold count to judge as a recorded track
#define RPG_MAX_LOOP                    10            // Maximum loop for RP gain calibration
#define RPG_DB_REDUCE_AT_SAT            3             // (dB) Gain reduced when saturation happens

//-----------------------------------------------------------------------------;
// Parameters for ECC_CHK_TBL                                                  ;
//-----------------------------------------------------------------------------;
#if (DETECT_ECC_DISC == 1)
    #if (ELECTRONIC_DAMPER_FOR_ECC_DET == 1)
#define ELECTRONIC_DAMPER_GAIN          13            // (1/128) gain-down ratio
    #endif

#define ECC_TIMEOUT                     100           // (mS) Timeout time for ECC detection

#if (BD_ENABLE == 1)
#define BD_ECC75_TH                     50            // (um) Threshold ECC to judge as a BD ECC75 disc
#define BD_ECC100_TH                    88            // (um) Threshold ECC to judge as a BD ECC100 disc
#define BD_ECC150_TH                    125           // (um) Threshold ECC to judge as a BD ECC150 disc
#endif

#if (ENABLE_HDDVD==1)
#define HD_ECC75_TH                     50            // (um) Threshold ECC to judge as a HD ECC75 disc
#define HD_ECC100_TH                    88            // (um) Threshold ECC to judge as a HD ECC100 disc
#define HD_ECC150_TH                    125           // (um) Threshold ECC to judge as a HD ECC150 disc
#endif

#define DVD_ECC50_TH                    40            // (um) Threshold ECC to judge as a DVD ECC50 disc
#define DVD_ECC100_TH                   75            // (um) Threshold ECC to judge as a DVD ECC100 disc
#define DVD_ECC150_TH                   125           // (um) Threshold ECC to judge as a DVD ECC150 disc

#define CD_ECC070_TH                    55            // (um) Threshold ECC to judge as a CD ECC070 disc
#define CD_ECC140_TH                    105           // (um) Threshold ECC to judge as a CD ECC140 disc
#define CD_ECC210_TH                    175           // (um) Threshold ECC to judge as a CD ECC210 disc
#define CD_ECC280_TH                    245           // (um) Threshold ECC to judge as a CD ECC280 disc
#endif // #if (DETECT_ECC_DISC == 1)


//-----------------------------------------------------------------------------;
// Parameters for TRON_TBL                                                     ;
//-----------------------------------------------------------------------------;
// Settings for trackin servo on
#define TRK_ON_WAIT_SLOW_TIMEOUT        50            // (mS) Timeout time for wait for track-cross being slow
#define TRK_ON_TIMEOUT                  500           // (mS) Timeout time for tracking servo on
#define TOK_TE_TH                       500           // (mV) TE threshold level for judging Tracking OK
    #if (CHIP_REV <= 0xB3)
#define TOKLVL_TRACK_OFF halAFE_TOKLVL_VREF_300mV
#define TOKLVL_WAIT_TRACK_ON halAFE_TOKLVL_VREF_450mV
#define TOKLVL_TRACK_ON halAFE_TOKLVL_VREF_600mV
#define TOKLVL_READ_YERR halAFE_TOKLVL_VREF_75mV
    #else
#define TOKLVL_TRACK_OFF halAFE_TECP_TECN_LVL_300mV
#define TOKLVL_WAIT_TRACK_ON halAFE_TECP_TECN_LVL_450mV
#define TOKLVL_TRACK_ON halAFE_TECP_TECN_LVL_600mV
#define TOKLVL_READ_YERR halAFE_TECP_TECN_LVL_100mV
    #endif
// Settings for DVD-RAM V1.0 detection
#define DVDRAM_V10_WOB_SPD_TH           169           //  1.69X


//-----------------------------------------------------------------------------;
// Parameters for FGDSP_TBL                                                    ;
//-----------------------------------------------------------------------------;
// DVD Focus
#define FGA_FREQ_DVD                    halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define FGA_FREQ_DVDRAM                 halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define FGA_AMP_DVDROM                  0xFA00        // (Kf10) Amplitude of injected sinewave for DVD-ROM SL disc
#define FGA_AMP_DVDR                    0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+R/-R SL disc
#define FGA_AMP_DVDRW                   0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+RW/-RW SL disc
#define FGA_AMP_DVDRAM                  0xFA00        // (Kf10) Amplitude of injected sinewave for DVD-RAM
#define FGA_AMP_DVDROMDL                0xFA00        // (Kf10) Amplitude of injected sinewave for DVD-ROM DL disc
#define FGA_AMP_DVDRDL                  0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+R/-R DL disc
#define FGA_AMP_DVDRWDL                 0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+RW/-RW DL disc
#define FGA_PHASE_DVDROM                (9)
#define FGA_PHASE_DVDR                  (8)
#define FGA_PHASE_DVDRW                 (8)
#define FGA_PHASE_DVDRAM                (12 | HAL_SRV_FGAVS | HAL_SRV_FGASE)
#define FGA_PHASE_DVDROMDL              (9)
#define FGA_PHASE_DVDRDL                (8)
#define FGA_PHASE_DVDRWDL               (8)

// CD Focus
#define FGA_FREQ_CD                     halSRV_AGC_2P00_KHZ  // Frequency of injected sinewave
#define FGA_AMP_CD                      0xFA00        // (Kf10) Amplitude of injected sinewave for CD disc
#define FGA_PHASE_CDROM                 (3)
#define FGA_PHASE_CDR                   (6)
#define FGA_PHASE_CDRW                  (6)

// BD Focus
#if (BD_ENABLE == 1)
//TBD Just copied, Fill in suitable values
#define FGA_FREQ_BD                    halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define FGA_AMP_BDROM                  0xFA00        // (Kf10) Amplitude of injected sinewave for BDROM SL disc
#define FGA_AMP_BDR                    0xFA00        // (Kf10) Amplitude of injected sinewave for BDR SL disc
#define FGA_AMP_BDRE                   0xFA00        // (Kf10) Amplitude of injected sinewave for BDRE SL disc
#define FGA_AMP_BDROMDL                0xFA00        // (Kf10) Amplitude of injected sinewave for BDROM DL disc
#define FGA_AMP_BDRDL                  0xFA00        // (Kf10) Amplitude of injected sinewave for BDR DL disc
#define FGA_AMP_BDREDL                 0xFA00        // (Kf10) Amplitude of injected sinewave for BDRE DL disc
#define FGA_PHASE_BDROM                (6)
#define FGA_PHASE_BDR                  (6)
#define FGA_PHASE_BDRE                 (6)
#define FGA_PHASE_BDROMDL              (6)
#define FGA_PHASE_BDRDL                (7)
#define FGA_PHASE_BDREDL               (7)
#endif

// HD Focus
#if (ENABLE_HDDVD == 1)
#define FGA_FREQ_HD                    halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define FGA_AMP_HDROM                  0xFA00        // (Kf10) Amplitude of injected sinewave for DVD-ROM SL disc
#define FGA_AMP_HDR                    0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+R/-R SL disc
#define FGA_AMP_HDRW                   0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+RW/-RW SL disc
#define FGA_AMP_HDROMDL                0xFA00        // (Kf10) Amplitude of injected sinewave for DVD-ROM DL disc
#define FGA_AMP_HDRDL                  0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+R/-R DL disc
#define FGA_AMP_HDRWDL                 0xFA00        // (Kf10) Amplitude of injected sinewave for DVD+RW/-RW DL disc
#define FGA_PHASE_HDROM                (14)
#define FGA_PHASE_HDR                  (12)
#define FGA_PHASE_HDRW                 (12)
#define FGA_PHASE_HDROMDL              (11)
#define FGA_PHASE_HDRDL                (12)
#define FGA_PHASE_HDRWDL               (10)
#endif

// Limit
#define FGA_LIMIT_FACTOR                1.4     //This value must less than 2, (3dB = 1.4125375446)
#define FGA_KFG_UPPER_LIMIT             (0x4000*FGA_LIMIT_FACTOR)
#define FGA_KFG_LOWER_LIMIT             (0x4000-(0x4000*FGA_LIMIT_FACTOR-0x4000))

//-----------------------------------------------------------------------------;
// Parameters for TGDSP_TBL                                                    ;
//-----------------------------------------------------------------------------;
// DVD Tracking
#define TGA_FREQ_DVD                    halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define TGA_FREQ_DVDRAM                 halSRV_AGC_4P00_KHZ  // Frequency of injected sinewave
#define TGA_AMP_DVDROM                  0xFB00        // (Kt10) Amplitude of injected sinewave for DVD-ROM SL disc
#define TGA_AMP_DVDR                    0xFB00        // (Kt10) Amplitude of injected sinewave for DVD+R/-R SL disc
#define TGA_AMP_DVDRW                   0xFB00        // (Kt10) Amplitude of injected sinewave for DVD+RW/-RW SL disc
#define TGA_AMP_DVDRAM                  0xFB00        // (Kt10) Amplitude of injected sinewave for DVD-RAM
#define TGA_AMP_DVDROMDL                0xFB00        // (Kt10) Amplitude of injected sinewave for DVD-ROM DL disc
#define TGA_AMP_DVDRDL                  0xFB00        // (Kt10) Amplitude of injected sinewave for DVD+R/-R DL disc
#define TGA_AMP_DVDRWDL                 0xFB00        // (Kt10) Amplitude of injected sinewave for DVD+RW/-RW DL disc
#define TGA_PHASE_DVDROM                (10)
#define TGA_PHASE_DVDR                  (10)
#define TGA_PHASE_DVDRW                 (12)
#define TGA_PHASE_DVDRAM                (13 | HAL_SRV_TGAVS | HAL_SRV_TGASE)
#define TGA_PHASE_DVDROMDL              (12)
#define TGA_PHASE_DVDRDL                (11)
#define TGA_PHASE_DVDRWDL               (11)

// CD Tracking
#define TGA_FREQ_CD                     halSRV_AGC_2P00_KHZ  // Frequency of injected sinewave
#define TGA_AMP_CD                      0xFC80        // (Kt10) Amplitude of injected sinewave for CD disc
#define TGA_PHASE_CDROM                 (3)//(4)
#define TGA_PHASE_CDR                   (3)//(4)
#define TGA_PHASE_CDRW                  (3)//(4)

// BD Tracking
#if (BD_ENABLE == 1)
#define TGA_FREQ_BD                    halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define TGA_AMP_BDROM                  0xF800        // (Kt10) Amplitude of injected sinewave for BD-ROM SL disc
#define TGA_AMP_BDR                    0xF800        // (Kt10) Amplitude of injected sinewave for BD SL disc
#define TGA_AMP_BDRE                   0xF800        // (Kt10) Amplitude of injected sinewave for BDRE SL disc
#define TGA_AMP_BDROMDL                0xF800        // (Kt10) Amplitude of injected sinewave for BDROM DL disc
#define TGA_AMP_BDRDL                  0xF800        // (Kt10) Amplitude of injected sinewave for BDR DL disc
#define TGA_AMP_BDREDL                 0xF800        // (Kt10) Amplitude of injected sinewave for BDRE DL disc
#define TGA_PHASE_BDROM                (10) //(12)
#define TGA_PHASE_BDR                  (6)  //(8)
#define TGA_PHASE_BDRE                 (6)  //(8)
#define TGA_PHASE_BDROMDL              (10) //(12)
#define TGA_PHASE_BDRDL                (6)  //(8)
#define TGA_PHASE_BDREDL               (6)  //(8)
#endif

// HD Tracking
#if (ENABLE_HDDVD == 1)
#define TGA_FREQ_HD                    halSRV_AGC_2P75_KHZ  // Frequency of injected sinewave
#define TGA_AMP_HDROM                  0xF600        // (Kt10) Amplitude of injected sinewave for DVD-ROM SL disc
#define TGA_AMP_HDR                    0xF600        // (Kt10) Amplitude of injected sinewave for DVD+R/-R SL disc
#define TGA_AMP_HDRW                   0xF600        // (Kt10) Amplitude of injected sinewave for DVD+RW/-RW SL disc
#define TGA_AMP_HDROMDL                0xF600        // (Kt10) Amplitude of injected sinewave for DVD-ROM DL disc
#define TGA_AMP_HDRDL                  0xF600        // (Kt10) Amplitude of injected sinewave for DVD+R/-R DL disc
#define TGA_AMP_HDRWDL                 0xF600        // (Kt10) Amplitude of injected sinewave for DVD+RW/-RW DL disc
#define TGA_PHASE_HDROM                (9)
#define TGA_PHASE_HDR                  (7)
#define TGA_PHASE_HDRW                 (7)
#define TGA_PHASE_HDROMDL              (9)
#define TGA_PHASE_HDRDL                (7)
#define TGA_PHASE_HDRWDL               (8)
#endif

// Limit
#define TGA_LIMIT_FACTOR                1.4     //This value must < 2, (3dB = 1.4125375446)
#define TGA_KTG_UPPER_LIMIT             (0x4000*TGA_LIMIT_FACTOR)
#define TGA_KTG_LOWER_LIMIT             (0x4000-(0x4000*TGA_LIMIT_FACTOR-0x4000))

#define BD_TGA_LIMIT_FACTOR             1.5
#define BD_TGA_KTG_UPPER_LIMIT          (0x4000*1.9)
#define BD_TGA_KTG_LOWER_LIMIT          (0x4000-(0x4000*BD_TGA_LIMIT_FACTOR-0x4000))

//-----------------------------------------------------------------------------;
// Parameters for DEFECT Length measure                                        ;
//-----------------------------------------------------------------------------;
    #if (DEFECT_LENGTH_MEASURE == 1)
#define ALLOW_DEFECT_LENGTH             1000          // (um)
#define NO_SPD_UP_DEFECT_LENGTH         500           // (um) Don't speed UP during play when defect length is over SpdNotUpDefectLength  .
    #endif


//-----------------------------------------------------------------------------;
// Parameters for SEEK_TBL                                                     ;
//-----------------------------------------------------------------------------;
#define FEED_THRESHOLD                  10

//Maximum allowed fine jump tracks
#define MAX_FINE_JUMP                   10            // maximum allowed fine jump per seek
#define FINE_THRESHOLD_CD               120
#define FINE_THRESHOLD_DVD              260
#define FINE_THRESHOLD_DVDRAM           120
#define FINE_THRESHOLD_BD               600

// Settings for Rough-seek
#define MAX_ROUGH_JUMP                  5             // maximum allowed rough jump per seek
#define RJ_TIMEOUT                      MS_250        // Time out for rough jump
#define ROUGH_THRESHOLD_CD              UM_2_SLED_STEP(400)
#define ROUGH_THRESHOLD_DVD             UM_2_SLED_STEP(300)
#define ROUGH_THRESHOLD_BD              UM_2_SLED_STEP(200)

// Pause distance
#define PAUSE_DISTANCE                  5             // tracks//20101029_02SF pause when waiting spindle when write gate up

//-----------------------------------------------------------------------------;
// Parameters for Layer jump                                                   ;
//-----------------------------------------------------------------------------;
// Settings for Layer jump
#define LJ_RETRYCNT                     2
#define LJ_FSL                          36
#define LJ_FCS_ADD                      50
#define LJ_FCS_ADD_BD                   50
#define LJ_SB_LEVEL_HYSTERESIS          5
#define LJ_FCS_DELAY_100uS              2
#define LJ_FCS_DELAY_DVDROM_1uS         50            // Some China bad DVDROM have very thin spacer
#define LJ_FCS_DELAY_BD_100uS           2
#define LJ_WAIT_FSRCH_T                 2             // (mS)
#define LJ_WAIT_FSRCH_T_DVD             3             // (mS)

//-----------------------------------------------------------------------------;
// Parameters for Focus Lock Aid                                               ;
//-----------------------------------------------------------------------------;
// Settings for Focus Search    FCS_VEL_THRESH, FCS_VEL_GAIN
#define FCS_VEL_FS_UNKNOWN  {       0x0600    ,     0x0300      }
#define FCS_VEL_FS_CD       {       0x0600    ,     0x0300      }
#define FCS_VEL_FS_DVD      {       0x0600    ,     0x0300      }
#define FCS_VEL_FS_LS       {       0x0600    ,     0x0300      }
#define FCS_VEL_FS_BD       {       0x0800    ,     0x0C00      }
#define FCS_VEL_FS_HD       {       0x0600    ,     0x0300      }

// Settings for Layer Jump
#define FCS_VEL_LJ_UNKNOWN  {       0x0600    ,     0x0300      }
#define FCS_VEL_LJ_CD       {       0x0600    ,     0x0300      }
#define FCS_VEL_LJ_DVD      {       0x0600    ,     0x0300      }
#define FCS_VEL_LJ_LS       {       0x0600    ,     0x0300      }
#define FCS_VEL_LJ_BD       {       0x0600    ,     0x0300      }
#define FCS_VEL_LJ_HD       {       0x0600    ,     0x0300      }

//-----------------------------------------------------------------------------;
// Parameters for DISCSTOP_TBL                                                 ;
//                                                                             ;
//              RPM > RPM_STOP0: use short brake, BRAKE_FORCE0                 ;
// RPM_STOP0 >= RPM > RPM_STOP1: use short brake, BRAKE_FORCE1                 ;
// RPM_STOP1 >= RPM > RPM_STOP2: use reverse brake, BRAKE_FORCE2               ;
// RPM_STOP2 >= RPM            : use reverse brake, BRAKE_FORCE3               ;
// RPM_STOP3 >= RPM            : wait 3 FG or DISC_STOP_NO_FG_TIME ms, stopped ;
//                                                                             ;
//-----------------------------------------------------------------------------;
#define STOP_TOUT                       SEC_6
#define RPM_STOP0                       7500          // rpm
#define RPM_STOP1                       4000          // rpm
#define RPM_STOP2                       200           // rpm
#define RPM_STOP3                       100           // rpm, care not make TSTOP3 > 65535

#define DISC_KICK_LEVEL                 0x00FF
#define DISC_BRAKE_LEVEL                0xFF01
    #if (CHIP_REV >= 0xC0)
#define DMODAC_FACTOR                   4             /* 4 = increased DMO resolution (10-bit)*/
    #else
#define DMODAC_FACTOR                   1             /* 1 = normal DMO resolution (8-bit)*/
    #endif
#define DISC_STOP_BRAKE_FORCE0          0x0000        // 0x4000-full power
#define DISC_STOP_BRAKE_FORCE1          0x2000        // 0x4000-full power
#define DISC_STOP_BRAKE_FORCE2          0x2000        // 0x4000-full power
#define DISC_STOP_BRAKE_FORCE3          0x1000        // when RPM <= RPM_STOP2
#define DISC_STOP_NO_FG_TIME            100           // ms, No FG pulse detected within DISC_STOP_NO_FG_TIME means stopped

//-----------------------------------------------------------------------------;
// Parameters for Tracking Servo Equalizer Gainup                              ;
//-----------------------------------------------------------------------------;
//128=100%
#define TRACKING_GAINUP_FACTOR_CD       128
#if (SUPPORT_DPD == 1)
#define TRACKING_FACTOR_CD_DPD          128//160 //[S0A] Shin debug
#endif
#if (CD_MB_ONLY_OPU == 1)
#define TRACKING_FACTOR_MB_ONLY         136//190 //[S0A] Shin debug
#endif
#define TRACKING_GAINUP_FACTOR_DVDR     166
#define TRACKING_GAINUP_FACTOR_DVDROM   166
#if (DVD_RAM_READ == 1)
#define TRACKING_GAINUP_FACTOR_DVDRAM   128
#define TRACKING_FACTOR_DVDRAM_VER_10   154//track_pitch_v1.0 = 1.2 * track_pitch_v2.0
#define TE_SLOPE_FACTOR_DVDRAM_EMBOSS   321//DPP_TE_slope = 2.51 * DPD_TE_slope
#define TRK_PITCH_FACTOR_DVDRAM_EMBOSS  77 //track_pitch_emboss = 0.6 * track_pitch_v2.0
#define TRACKING_FACTOR_DVDRAM_EMBOSS   (TE_SLOPE_FACTOR_DVDRAM_EMBOSS * TRK_PITCH_FACTOR_DVDRAM_EMBOSS / 128)
#endif
#if (BD_ENABLE == 1)
#define TRACKING_GAINUP_FACTOR_BDR      166
#define TRACKING_GAINUP_FACTOR_BDROM    166
#endif
#if (ENABLE_HDDVD == 1)
#define TRACKING_GAINUP_FACTOR_HDR      128
#define TRACKING_GAINUP_FACTOR_HDROM    128
#endif

// Factor for KSPDLIM for fine jump TKIC clip, KSPDLIM = KSPDLIM_FACTOR * KICKLVL / 128
#define KSPDLIM_FACTOR_CD               32
#define KSPDLIM_FACTOR_DVDR             60
#define KSPDLIM_FACTOR_DVDROM           60
#define KSPDLIM_FACTOR_DVDRAM           55
#define KSPDLIM_FACTOR_BDR              60//32
#define KSPDLIM_FACTOR_BDROM            60
//-----------------------------------------------------------------------------;
// Parameters for Focus Servo Equalizer Gainup                                 ;
//-----------------------------------------------------------------------------;
#define FOCUS_GAINUP_FACTOR_CD          128//166 //[S0A] Shin debug
#define FOCUS_GAINUP_FACTOR_DVDR        128
#define FOCUS_GAINUP_FACTOR_DVDROM      128
#define FOCUS_GAINUP_FACTOR_DVDRAM      128
#if BD_ENABLE == 1
#define FOCUS_GAINUP_FACTOR_BDR         179
#define FOCUS_GAINUP_FACTOR_BDROM       179
#endif
#if ENABLE_HDDVD == 1
#define FOCUS_GAINUP_FACTOR_HDR         128
#define FOCUS_GAINUP_FACTOR_HDROM       128
#endif


//-----------------------------------------------------------------------------;
// Parameters for Focus Servo Equalizer Gain Factor during fine jump (0dB:128) ;
//-----------------------------------------------------------------------------;
#define FOCUS_FJ_GAIN_FACTOR_CD         60//80 //[S0A] Shin debug     //about-4dB 
#define FOCUS_FJ_GAIN_FACTOR_DVD        80      //about-4dB
#define FOCUS_FJ_GAIN_FACTOR_DVDRAM     40
#if BD_ENABLE == 1
#define FOCUS_FJ_GAIN_FACTOR_BD         128
#endif
#if ENABLE_HDDVD == 1
#define FOCUS_FJ_GAIN_FACTOR_HD         128
#endif

//-------------------------------------------------------------------------------;
// Parameters for Focus Servo Equalizer Gain Factor during tracking off(0dB:128) ;
//-------------------------------------------------------------------------------;
#define FOCUS_TOFF_GAIN_FACTOR_CD         40//64 //[S0A] Shin debug  //about -6dB 
#define FOCUS_TOFF_GAIN_FACTOR_DVD        64    //about -6dB
#define FOCUS_TOFF_GAIN_FACTOR_DVDRAM     96
#if BD_ENABLE == 1
#define FOCUS_TOFF_GAIN_FACTOR_BD         80    //about -4dB
#endif
#if ENABLE_HDDVD == 1
#define FOCUS_TOFF_GAIN_FACTOR_HD         128
#endif

//-----------------------------------------------------------------------------;
// Parameters for ENABLE_UP_DOWN_JUMP_CONTROL                                  ;
//-----------------------------------------------------------------------------;
#if (ENABLE_UP_DOWN_JUMP_CONTROL == 1)
    #define UP_DOWN_JUMP_LIMIT1 15                          // tracks  was 15
    #define UP_DOWN_JUMP_LIMIT2 (UP_DOWN_JUMP_LIMIT1 + 3)   // add some hysteresis
#endif // of (ENABLE_UP_DOWN_JUMP_CONTROL == 1)

//-----------------------------------------------------------------------------;
// Parameters for Stepping Motor                                               ;
//-----------------------------------------------------------------------------;
#if DO_HW_SUPPORT_STEP == 1
#define MICRO_STEP_PER_ELE_REV          64            // Number of steps per electric revolution
#else
#define MICRO_STEP_PER_ELE_REV          128           // Number of steps per electric revolution
#endif
#define DISTANCE_PER_MACH_REV           3000          // (um) Pitch of lead-screw
#define ELE_REV_PER_MACH_REV            5             // Number of poles

//-----------------------------------------------------------------------------;
// Parameters for Sledge                                                       ;
//-----------------------------------------------------------------------------;
//sledge homing related settings
//HOMING_DISTANCE & HOMING_POST_DISTANCE should be same for most loaders.
#if (OPU_HomeSW > 0)
#define HOMING_PRE_DISTANCE             640
#else
#define HOMING_POST_DISTANCE            (ELE_REV_PER_MACH_REV*MICRO_STEP_PER_ELE_REV)
#endif //OPU_HomeSW
#define HOMING_DISTANCE                 (72*MICRO_STEP_PER_ELE_REV+((STEP_OUTWARD_POSITION-STEP_HOME_POSITION_INIT)&0x7F))
#if (Customer_A1 ==1)
#define Short_HOMING_DISTANCE           (57*MICRO_STEP_PER_ELE_REV+((STEP_OUTWARD_POSITION-STEP_HOME_POSITION_INIT)&0x7F))-ToleranceDistance
#define ToleranceDistance 							107
#endif

/***********************************************************************************************************************
Sledge homing related settings tuning guide
step1:adjust STEP_OUTWARD_MICRO_STEP
a.Set STEP_HOME_MICRO_STEP_TEST = 1 in svo_sta.c to create a test firmware with USB message enabled.
b.Flash the test firmware to a drive without housing (so you can see the OPU to hit the outter wall)
c.Do a tray in without disc, the drive will perform a homing automatically.
d.During homing the sledge will stop at very outter position and move toward outside in very small step every second.
e.Each movement there will be a printf with a hex number (0x00~0x7F). The number will increase by 0x10 per movement.
f.Remember the number when the OPU hit the outside wall, it is the optimal STEP_OUTWARD_MICRO_STEP value for this drive
Do step1 on more drives, set the value close to most of drives in STEP_OUTWARD_MICRO_STEP (0x00 is close to 0x70)

step2:adjust STEP_OUTWARD_POSITION
a.Set STEP_HOME_MICRO_STEP_TEST = 0 in svo_sta.c to create a normal firmware with USB message enabled.
b.Enable USB message DEBUG_DISK_ID_MSG (SVO 03), put a standard disc and do startup.
c.Search "OUTWARD_POS" in the printf and the next line is the optimal STEP_OUTWARD_POSITION value for this drive.
Do step2 on more drives and set the average value to STEP_OUTWARD_POSITION

step3:decide STEP_HOME_POSITION
The Iram_stp_CurrentPosition is represent radius in the unit of sledge micro step(4.6875um) when step1 and step2 are
correctly done. So we only need to decide what radius we want home position to be and set it to STEP_HOME_POSITION.
Notes that the maximum error of homing is about 128 micro step + mechanical tolerance. So the homing position better
larger than 25000 + 128*4.6875 um
For example if we want to set home position to 25.7mm = 25700um, then STEP_HOME_POSITION = 25700 / 4.6875 = 5483.

step4:decide COLLI_HOMING_DISTANCE
a.We need to set COLLI_HOMING_DISTANCE step that better large than max step during homing process.

                           PI sensor position
                                  |                                                  |
                                  |                  max step                        |
                                  |<------------------------------------------------>|
               CCW                |                   CW                             |
----------------------------------|--------------------------------------------------|
b.1 sin wave=4 full steps, 1 full step=32 micro steps ,1 sin wave=4*32 micro steps,

***********************************************************************************************************************/
#if DOUBLE_LENS
#define IS_OUTER_LENS(m)                ((m == eDiscCD)||(m == eDiscDVD))
#define STEP_LENS_OFFSET                960            //  960*4.6875=4500um        //  OFFSET OPU 4.5mm
#endif

#define STEP_OUTWARD_MICRO_STEP         0x50          // 0x00~0x7F
#define STEP_OUTWARD_POSITION           12806
#define STEP_INWARD_POSITION            4010

#define STEP_HOME_POSITION_DVDRAM       UM_2_SLED_STEP(25450)   //user data start radius: 25450um (RAM)
#define STEP_HOME_POSITION_CD           UM_2_SLED_STEP(25700)   //user data start radius:25000um
#define STEP_HOME_POSITION_DVD          UM_2_SLED_STEP(24700)   //user data start radius:DVD:24000um, DVDRAM:24418
#define STEP_HOME_POSITION_BD           UM_2_SLED_STEP(25600)   //user data start radius: 24500um (BD)
#define STEP_HOME_POSITION_INIT         STEP_HOME_POSITION_BD
#define STEP_TRAY_OUT_POSITION          STEP_HOME_POSITION_INIT
#if COLLI_SWITCH ==1
    #if (COLLI_HomeSW != 0)
#define COLLI_HOMING_PRE_DISTANCE       672
#define COLLI_HOMING_POST_DISTANCE      256
    #else
#define COLLI_HOMING_PRE_DISTANCE       0
#define COLLI_HOMING_POST_DISTANCE      640
    #endif
#define COLLI_HOMING_DISTANCE           9728           //OPU                            :SFBD412// set colli maximum range
#endif                                                 //range of cover layer thinkness :87.5+-17.5um
                                                       //correction on 1 step           :0.237um
                                                       //max step                       :105/0.237=443 full steps
                                                       //homing step=500(>443 steps)x32(1 full step=32 micro steps)=16000 micro steps

//sledge gain related settings
#if (OPU_HomeSW == 0)
#define SLED_HOMING_GAIN                45            // 45 /128 =  35.2% for sledge to hit the wall
#endif
#define SLED_HOLD_GAIN                  0            // 20 /128 =  15.6% for sledge to hold still
#define SLED_FOLLOW_GAIN                60            // 60 /128 =  46.9% for following
#define SLED_JUMP_GAIN                  68            // 68 /128 =  53.1%
#define SLED_FULL_GAIN                  128           // 128/128 = 100.0%
#define SLED_DZ                         12            // unit:sled output dac. To compansate drive IC dead zone, set 0 to disable this function.

#if COLLI_SWITCH ==1
#define COLLI_HOLD_GAIN                 0             //   0/128 =   0.0% for colli to hold still
#define COLLI_JUMP_GAIN                 128           // 128/128 = 100.0%
#endif

#if (USE_EEPROM_COLLI_POS == 1)
// Collimator position(steps)
/**********************************************************************************************************
* Sensor out
*____
*    |COLLI_BD_Position_Zero   COLLI_BD_ROM_L0(COLLI_BD_R_RE_L0)
*    |<--------------------->|<---------------->|
*    |_______________________|__________________|
*                            |            COLLI_BD_ROM_L1(COLLI_BD_R_RE_L1)
*                            |<--------------------------------------------->|
**********************************************************************************************************/
#define COLLI_BD_Position_Zero      0      //From Sensor to Zero
#define COLLI_BD_ROM_L0             240      //From Zero to ROM L0
#define COLLI_BD_ROM_L1             148      //From Zero to ROM L1
#define COLLI_BD_R_RE_L0            240      //From Zero to R/RE L0
#define COLLI_BD_R_RE_L1            148      //From Zero to R/RE L1
#endif
/**********************************************************************************************************
*    BYTE VMax;   //maximum velocity.           unit:1.0 step / ms      => 4.6875     um/ms
*    BYTE Acc;    //acceleration.               unit:0.0625 step / ms^2 => 0.29296875 um/ms^2
*    BYTE Dec;    //deceleration.               unit:0.0625 step / ms^2 => 0.29296875 um/ms^2
*    BYTE VFull;  //sin,square wave transition. unit:1.0 step / ms      => 4.6875     um/ms
*    BYTE Gain;   //sin,square wave amplitude.  0~128
**********************************************************************************************************/
/* sledge jump profile                  VMax,  Acc,  Dec,VFull,  Gain  */

#if (Customer_A1 ==1)
#define SLED_HOME_JUMP_PROFILE          { 31,    12,   12,  255,  40} 
#define SLED_FULL_JUMP_PROFILE          { 16,    5,    18,  255,  40} 
#define SLED_HOME2_JUMP_PROFILE         { 31,   12,   12,   255,  40} 
#define SLED_SAFE_JUMP_PROFILE          { 18,    4,   10,   255,  40} /* STEP_SAFE_SLOW_PROFILE */
#else
#define SLED_HOME_JUMP_PROFILE          { 24,    5,    7,  255,  SLED_JUMP_GAIN} /* STEP_HOMING_PROFILE */ //this mode the SLED_GAIN will be smoothly reduced to SLED_HOMING_GAIN during jump
#define SLED_FULL_JUMP_PROFILE          { 15,    9,    9,  255,  SLED_FULL_GAIN} /* STEP_FULL_PROFILE */   //this mode use higher gain to pull OPU away from the wall
#define SLED_SAFE_JUMP_PROFILE          { 38,   12,   17,   24,  SLED_JUMP_GAIN} /* STEP_SAFE_SLOW_PROFILE */
#endif

//[S08] daniel for seek noise #define SLED_NORMAL_JUMP_PROFILE        { 80,   26,   29,   24,  SLED_JUMP_GAIN} /* STEP_NORMAL_PROFILE */
//[W0C] Shin #define SLED_NORMAL_JUMP_PROFILE        { 35,   10,   14,   35,              40} /* STEP_NORMAL_MODE */ 
#define SLED_NORMAL_JUMP_PROFILE        { 40,   18,   23,   40,              43} /* STEP_NORMAL_MODE */
//[S08] daniel for seek noise #define SLED_SHORT_JUMP_PROFILE         { 80,   20,   20,   31,  SLED_JUMP_GAIN} /* STEP_NORMAL_SHORT_PROFILE */
#define SLED_SHORT_JUMP_PROFILE         { 25,   12,   12,   255,             40} /* STEP_NORMAL_SHORT_PROFILE */ 
#define SLED_NO_CE_JUMP_PROFILE         {  2,    1,    1,  255,  SLED_JUMP_GAIN} /* STEP_NO_CE_SLOW_PROFILE */
/* collimator jump profile                  VMax,  Acc,  Dec,VFull,  Gain  */
#if COLLI_SWITCH == 1
#define COLLI_NORMAL_JUMP_PROFILE       { 25,   40,   40,    0,  COLLI_JUMP_GAIN}/* COLLI_NORMAL_PROFILE*/
#define COLLI_MICRO_JUMP_PROFILE        { 25,   40,   40,    0,  COLLI_JUMP_GAIN}/* COLLI_MICRO_PROFILE*/
#endif

#if COLLI_SWITCH == 1
#define COLLI_INIT_INTERVAL_CLK16   (CLK16_1000_US * 5)
#define COLLI_EXT_INTERVAL_CLK16    (CLK16_1000_US * 5)
#define COLLI_JUMP_MIN_VEL          13
#if (HALF_COLLI_REST_TIME == 1)
#define COLLI_REST_TIME             340     // a rest time between 2 long colli jump to guarantee the driving duty is within spec
#else
#define COLLI_REST_TIME             600     //a rest time between 2 long colli jump to guarantee the driving duty is within spec
#endif                                      //driving duty = colli_jump_time / (colli_jump_time + COLLI_REST_TIME)
                                            //set COLLI_REST_TIME = 0 to disable this feature
    #if (COLLI_REST_TIME > 0)
#define COLLI_REST_MIN_DIST         2500    //apply rest time only after colli jump distance >= COLLI_REST_MIN_DIST
    #endif
#endif
#define SLED_INIT_INTERVAL_CLK16    CLK16_200_US
#define SLED_EXT_INTERVAL_CLK16     CLK16_1000_US * 5
#define MIN_MOVE_INTERVAL_US        190

#define SLED_JUMP_MIN_VEL   2//5  //minimum velocity for sledge jump. unit:step / ms
#define SLED_JUMP_END_DIST  8  //distance moved with velocity = SLED_JUMP_MIN_VEL at the end of jump. unit:steps //improve sledge jump accuracy

#define CE_GAIN_DOWN_VEL_CD             5.0     // CE gain-down switching point, must >= SLED_JUMP_MIN_VEL. unit:step / ms
#define CE_GAIN_DOWN_VEL_DVD            5.0     // CE gain-down switching point, must >= SLED_JUMP_MIN_VEL. unit:step / ms
#define CE_GAIN_DOWN_VEL_BD             5.0     // CE gain-down switching point, must >= SLED_JUMP_MIN_VEL. unit:step / ms
#define CE_GAIN_DOWN_RATIO_CD           64      // (64/128) CE gain-down ratio
#define CE_GAIN_DOWN_RATIO_DVD          64      // (64/128) CE gain-down ratio
#define CE_GAIN_DOWN_RATIO_BD           64     //(84/128) CE gain-down ratio

//---------------------------------------------------------;
// Parameters for svoSTEPInt                               ;
//---------------------------------------------------------;
#define SLED_THRESHOLD_MAX            96              //SLO dac
#define SLED_THRESHOLD_MAX_DVDRAM     96              //SLO dac
#define SLED_THRESHOLD_DELAY_CD       8               //ms proportional to 1/track pitch
#define SLED_THRESHOLD_DELAY_DVD      16              //ms proportional to 1/track pitch
#define SLED_THRESHOLD_DELAY_BDHD     32              //ms proportional to 1/track pitch
#define SLED_THRESHOLD_INC_DEC        2               //um
#define SLED_MIN_FOLLOW_DELAY         5               //ms proportional to sled step size
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
#define SEEK_THRESHOLD_POS_DVD          ( 0x05)
#define SEEK_THRESHOLD_NEG_DVD          (-0x05)
#define SEEK_THRESHOLD_POS_DVDR         ( 0x05)
#define SEEK_THRESHOLD_NEG_DVDR         (-0x05)
#define SEEK_THRESHOLD_POS_DVDRAM       ( 0x10)
#define SEEK_THRESHOLD_NEG_DVDRAM       (-0x10)
#define SEEK_THRESHOLD_POS_CD           ( 0x06)
#define SEEK_THRESHOLD_NEG_CD           (-0x06)
#endif

#define TRACK_SLIP_DETECT_THRESHOLD     UM_2_SLED_STEP(280)     /* The threshold of TEINT that defines track slipping!! */
#define MAX_SLED_FOLLOW_RADIUS          62000         //um
#define MIN_SLED_FOLLOW_RADIUS          20000         //um

//---------------------------------------------------------;
// Parameters for TXSpeed monitor                          ;
//---------------------------------------------------------;
#define TRACK_SLIP_TRACK_CROSS_FREQ         4600      // (Hz) TXSPEED = 352800 / TRACK_CROSS_FREQ
#define TRACK_SLIP_TRACK_CROSS_FREQ_DVDRAM  17640     // (Hz) TXSPEED = 352800 / TRACK_CROSS_FREQ
#define TRACK_SLIP_TRACK_CROSS_FREQ_BD      14000     // (Hz) TXSPEED = 352800 / TRACK_CROSS_FREQ
#define TOK_CHK_T                       MS_8
#define TOK_CHK_T_SEEK                  MS_30

//---------------------------------------------------------;
// Parameters for track_slip_protection                    ;
//---------------------------------------------------------;
#define CE_WINDOW_UM                    200           //if CE shows the LENS is off center more than CE_WINDOW_UM, we'll use full CE gain to pull the LENS to center

//---------------------------------------------------------;
// Parameters for collimator                               ;
//---------------------------------------------------------;
#if(COLLI_POS_RECOVERY==1)
#define COLLI_POS_TOLERANCE             960
#endif

//---------------------------------------------------------;
// Parameters for state_pll_chk                            ;
//---------------------------------------------------------;
#if (DVD_RAM_READ == 1)
#define PLL_T                           MS_50
#else
#define PLL_T                           MS_30
#endif

//---------------------------------------------------------;
// servo table settings                                    ;
//---------------------------------------------------------;
    #if (CHIP_REV < 0xC0)
    #else
    #endif

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define svoks17Kick_UL_F          { 10,    0},\
                                  { 13, 9000},\
                                  { 17, 5933},\
                                  { 23, 5333},\
                                  { 31, 4666},\
                                  { 41, 3133},\
                                  { 51, 2666},\
                                  { 63, 1933},\
                                  { 79, 1933},\
                                  { 95, 1333},\
                                  {127, 1200},\
                                  {159,  866},\
                                  {191,  733},\
                                  {223,  666},\
                                  {255,  533}

#define svoks17Kick_UL_B          { 10,    0},\
                                  { 13, 6600},\
                                  { 17, 4500},\
                                  { 23, 4500},\
                                  { 31, 2900},\
                                  { 41, 2100},\
                                  { 51, 1700},\
                                  { 63, 1400},\
                                  { 79, 1000},\
                                  { 95,  750},\
                                  {127,  600},\
                                  {159,  550},\
                                  {191,  500},\
                                  {223,  500},\
                                  {255,  350}

#define svoks17Kick_L_F           { 10,    0},\
                                  { 13, 8666},\
                                  { 17, 6000},\
                                  { 23, 5523},\
                                  { 31, 4666},\
                                  { 41, 3666},\
                                  { 51, 2866},\
                                  { 63, 2466},\
                                  { 79, 2166},\
                                  { 95, 1533},\
                                  {127, 1200},\
                                  {159,  900},\
                                  {191,  766},\
                                  {223,  600},\
                                  {255,  500}

#define svoks17Kick_L_B           { 10,    0},\
                                  { 13, 6600},\
                                  { 17, 4800},\
                                  { 23, 4500},\
                                  { 31, 2900},\
                                  { 41, 2200},\
                                  { 51, 1750},\
                                  { 63, 1400},\
                                  { 79, 1100},\
                                  { 95,  800},\
                                  {127,  650},\
                                  {159,  600},\
                                  {191,  550},\
                                  {223,  500},\
                                  {255,  300}

#define svoks17Kick_M_F           { 10,    0},\
                                  { 13, 9000},\
                                  { 17, 7066},\
                                  { 23, 6066},\
                                  { 31, 4933},\
                                  { 41, 4333},\
                                  { 51, 3233},\
                                  { 63, 2100},\
                                  { 79, 1866},\
                                  { 95, 1400},\
                                  {127, 1150},\
                                  {159,  920},\
                                  {191,  800},\
                                  {223,  600},\
                                  {255,  475}

#define svoks17Kick_M_B           { 10,    0},\
                                  { 13, 9000},\
                                  { 17, 7700},\
                                  { 23, 6000},\
                                  { 31, 5100},\
                                  { 41, 4000},\
                                  { 51, 3000},\
                                  { 63, 2100},\
                                  { 79, 1466},\
                                  { 95, 1200},\
                                  {127, 1000},\
                                  {159,  866},\
                                  {191,  700},\
                                  {223,  533},\
                                  {255,  466}

#define svoks17Kick_H_F           { 10,    0},\
                                  { 13, 9666},\
                                  { 17, 6666},\
                                  { 23, 5533},\
                                  { 31, 4333},\
                                  { 41, 3333},\
                                  { 51, 2333},\
                                  { 63, 1933},\
                                  { 79, 1500},\
                                  { 95, 1253},\
                                  {127, 1013},\
                                  {159,  766},\
                                  {191,  646},\
                                  {223,  546},\
                                  {255,  453}

#define svoks17Kick_H_B           { 10,    0},\
                                  { 13, 7000},\
                                  { 17, 4600},\
                                  { 23, 4500},\
                                  { 31, 2900},\
                                  { 41, 2200},\
                                  { 51, 1750},\
                                  { 63, 1400},\
                                  { 79, 1100},\
                                  { 95,  800},\
                                  {127,  500},\
                                  {159,  450},\
                                  {191,  400},\
                                  {223,  300},\
                                  {255,  300}

#define svoks17Kick_UH_F          { 10,    0},\
                                  { 13,    0},\
                                  { 17,    0},\
                                  { 23,    0},\
                                  { 31,    0},\
                                  { 41,    0},\
                                  { 51, 3500},\
                                  { 63, 2250},\
                                  { 79, 2000},\
                                  { 95, 1666},\
                                  {127, 1050},\
                                  {159,  920},\
                                  {191,  880},\
                                  {223,  680},\
                                  {255,  520}

#define svoks17Kick_UH_B          { 10,    0},\
                                  { 13,    0},\
                                  { 17,    0},\
                                  { 23,    0},\
                                  { 31,    0},\
                                  { 41,    0},\
                                  { 51, 3500},\
                                  { 63, 2250},\
                                  { 79, 1600},\
                                  { 95, 1366},\
                                  {127, 1130},\
                                  {159,  950},\
                                  {191,  833},\
                                  {223,  580},\
                                  {255,  490}


/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define dvsvoks17Kick_UL_F        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvsvoks17Kick_UL_B        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvsvoks17Kick_L_F         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvsvoks17Kick_L_B         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvsvoks17Kick_M_F         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvsvoks17Kick_M_B         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvsvoks17Kick_H_F         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define dvsvoks17Kick_H_B         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define dvsvoks17Kick_UH_F        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define dvsvoks17Kick_UH_B        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#if (ENABLE_HDDVD == 1)
/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define hdsvoks17Kick_UL_F        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdsvoks17Kick_UL_B        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdsvoks17Kick_L_F         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdsvoks17Kick_L_B         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdsvoks17Kick_M_F         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdsvoks17Kick_M_B         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdsvoks17Kick_H_F         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define hdsvoks17Kick_H_B         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define hdsvoks17Kick_UH_F        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define hdsvoks17Kick_UH_B        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}
#endif

//TBD!!! Split BD and HD: This means a new hdSvo_tbl.c/h have to be made and used
#if (BD_ENABLE == 1) || (ENABLE_HDDVD == 1)
/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define bdsvoks17Kick_UL_F        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdsvoks17Kick_UL_B        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdsvoks17Kick_L_F         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdsvoks17Kick_L_B         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdsvoks17Kick_M_F         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdsvoks17Kick_M_B         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdsvoks17Kick_H_F         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define bdsvoks17Kick_H_B         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define bdsvoks17Kick_UH_F        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define bdsvoks17Kick_UH_B        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}
#endif
/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define rdvsvoks17Kick_UL_F       { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define rdvsvoks17Kick_UL_B       { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define rdvsvoks17Kick_L_F        { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define rdvsvoks17Kick_L_B        { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define rdvsvoks17Kick_M_F        { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define rdvsvoks17Kick_M_B        { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define rdvsvoks17Kick_H_F        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define rdvsvoks17Kick_H_B        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define rdvsvoks17Kick_UH_F       { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define rdvsvoks17Kick_UH_B       { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#if (ENABLE_HDDVD == 1)
/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define hdrsvoks17Kick_UL_F        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdrsvoks17Kick_UL_B        { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdrsvoks17Kick_L_F         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdrsvoks17Kick_L_B         { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdrsvoks17Kick_M_F         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdrsvoks17Kick_M_B         { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define hdrsvoks17Kick_H_F         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define hdrsvoks17Kick_H_B         { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define hdrsvoks17Kick_UH_F        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define hdrsvoks17Kick_UH_B        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}
#endif

/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define bdrsvoks17Kick_UL_F       { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdrsvoks17Kick_UL_B       { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdrsvoks17Kick_L_F        { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdrsvoks17Kick_L_B        { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdrsvoks17Kick_M_F        { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdrsvoks17Kick_M_B        { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define bdrsvoks17Kick_H_F        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define bdrsvoks17Kick_H_B        { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define bdrsvoks17Kick_UH_F       { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define bdrsvoks17Kick_UH_B       { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

/*----------------------------------------------------------------------------*/
/*                    Fine search ks17(0x3E) coefficient                      */
/*                                    No.0                                    */
/*----------------------------------------------------------------------------*/
#define dvRamsvoks17Kick_UL_F     { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvRamsvoks17Kick_UL_B     { 10,     0}, \
                                  { 13,  6700}, \
                                  { 17,  4500}, \
                                  { 23,  4500}, \
                                  { 31,  2900}, \
                                  { 41,  2200}, \
                                  { 51,  1800}, \
                                  { 63,  1300}, \
                                  { 79,  1800}, \
                                  { 95,  1600}, \
                                  {127,  1600}, \
                                  {159,   500}, \
                                  {191,   500}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvRamsvoks17Kick_L_F      { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0700}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0100}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvRamsvoks17Kick_L_B      { 10,     0}, \
                                  { 13,0x0A00}, \
                                  { 17,0x0800}, \
                                  { 23,0x0800}, \
                                  { 31,0x0300}, \
                                  { 41,0x0200}, \
                                  { 51,0x0150}, \
                                  { 63,0x00E0}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvRamsvoks17Kick_M_F      { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvRamsvoks17Kick_M_B      { 10,     0}, \
                                  { 13, 10333}, \
                                  { 17,  8000}, \
                                  { 23,  5533}, \
                                  { 31,  4333}, \
                                  { 41,  3800}, \
                                  { 51,  2933}, \
                                  { 63,  2466}, \
                                  { 79,  1500}, \
                                  { 95,  1300}, \
                                  {127,  1300}, \
                                  {159,   500}, \
                                  {191,   400}, \
                                  {223,   400}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,300}

#define dvRamsvoks17Kick_H_F      { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define dvRamsvoks17Kick_H_B      { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define dvRamsvoks17Kick_UH_F     { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}

#define dvRamsvoks17Kick_UH_B     { 10,     0}, \
                                  { 13,  5500}, \
                                  { 17,  4300}, \
                                  { 23,  3800}, \
                                  { 31,  2300}, \
                                  { 41,  1600}, \
                                  { 51,  1200}, \
                                  { 63,  1000}, \
                                  { 79,   700}, \
                                  { 95,   600}, \
                                  {127,   450}, \
                                  {159,   400}, \
                                  {191,   350}, \
                                  {223,   350}, \
                                  {255,   300}, \
                                  {355,   300}, \
                                  {0xFFFF,550}
#endif

/**************************************************************************************************************/
//For RDVD DISC

#if (LPP_AUTO_LEARN_TEST==1)
#define MRWDEF_HDRTHRSH3   0x1A    //RW blank 4XCLV 0x14~0x1f ,2XCLV 0x10~0x1f , mid 0x1A
#define MRDEF_HDRTHRSH3    0x17    //R blank 4XCLV 0x0e~0x1f ,2XCLV 0x0a~0x1f , mid 0x17
#define MRDLDEF_HDRTHRSH3  0x18    //RDL blank 2XCLV 0x0c~0x1f , mid 0x18

#define MRWDEF_HDRTHRSH4   0x16    //RW data 4XCLV 0x10~0x1C ,2XCLV 0x0A~0x1C , mid 0x16
#define MRDEF_HDRTHRSH4    0x19    //R  data 4XCLV 0x12~0x1f ,2XCLV 0x0e~0x1f , mid 0x19
#define MRDLDEF_HDRTHRSH4  0x1C    //RDLdata 2XCLVL0 0x1A~0x1f ,2XCLVL1 0x1c~0x1f , set 0x1c

#define LPP_HDRTHRSH_DIF   0x02
#endif//LPP_AUTO_LEARN_TEST

//---------------------------------------------------------;
// Table Cav_RPM_Tbl                                       ;
//---------------------------------------------------------;
    #if (NEW_SPINDLE_CONTROL == 1)
// DE4_msb = (A * rpm^2 + B * rpm + C) * 0x80 * DMODAC_FACTOR / kdf14
// A = 1 / SPD_INT_PARA_1
// B = SPD_INT_PARA_2 / SPD_INT_PARA_3
// C = SPD_INT_PARA_4
#define SPD_INT_PARA_1                  181209        // 1 / A
#define SPD_INT_PARA_2                  8521          // B * 262144
#define SPD_INT_PARA_3                  155           // C (0~400)

#define SPD_INT_PARA_1_8CM              3480517       // 1 / A
#define SPD_INT_PARA_2_8CM              7045          // B * 262144
// SPD_INT_PARA_3_8CM = SPD_INT_PARA_3
    #endif

#define CAV_RPM_MAX             10932

#define CD_PCAV_RPM              9300
#define CD_CAV_RADIUS_REF       56500 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

//------------------------------------------------
#define DVDSL_PCAV_RPM           9300
#define DVDSL_CAV_RADIUS_REF    57300 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

//------------------------------------------------
#define DVDDL_PCAV_RPM           9300
#define DVDDL_CAV_RADIUS_REF    57000 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

//------------------------------------------------
#define BDSL_PCAV_RPM            9300
#define BDSL_CAV_RADIUS_REF     57000 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

//------------------------------------------------
#define BDDL_PCAV_RPM            9300
#define BDDL_CAV_RADIUS_REF     57000 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

//------------------------------------------------
#define HDSL_PCAV_RPM            9300
#define HDSL_CAV_RADIUS_REF     57000 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

//------------------------------------------------
#define HDDL_PCAV_RPM            9300
#define HDDL_CAV_RADIUS_REF     57000 //um, smaller CAV_RADIUS_REF gives larger CAV RPM

#if (SRC_ENABLE == 1)
/*--------------------------------------------------------------------------------------------------------------*
 *  SRC operates:
 *  1. Enable:  if (speed >= MIN_SPEED_ENABLE_SRC)
 *  2. Disable: if (speed < MIN_SPEED_ENABLE_SRC)
 *--------------------------------------------------------------------------------------------------------------*/
#define MIN_SPEED_ENABLE_SRC_HD      _2LX_HD
#define MIN_SPEED_ENABLE_SRC_BD      _2LX_BD
#define MIN_SPEED_ENABLE_SRC_DVD     _4AX_DVD
#define MIN_SPEED_ENABLE_SRC_DVDRAM  _16AX_DVDRAM
#define MIN_SPEED_ENABLE_SRC_CD      _10AX
#define SRC_TRK_PRESCALE             0x2000//gain scaling for SRC, This is a scale base and equals to 2*Integer
#define SRC_FOC_PRESCALE             0x2000//gain scaling for SRC, This is a scale base and equals to 2*Integer
#endif

//-----------------------------------------------------------------------------;
// Parameters for OPU Gain Select & AFE Input gain                             ;
//-----------------------------------------------------------------------------;
#if (PDIC_BASE_ON_SRFO == 1)
//PDIC Gain (0.1dB)                0,      1,      2,      3,
#define PDIC_GAIN_CD               0
#define PDIC_GAIN_DVD              0
#define PDIC_GAIN_BD             -90,      0

#define PDIC_GAIN_MAX_CD           1
#define PDIC_GAIN_MAX_DVD          1
#define PDIC_GAIN_MAX_BD           2
#define PDIC_GAIN_MIN_CD           0
#define PDIC_GAIN_MIN_DVD          0
#define PDIC_GAIN_MIN_BD           0

#define PDIC_STM_CD                0
#define PDIC_STM_DVD               0
#define PDIC_STM_BD                1

#define PDIC_SRFO_TARGET_CD        250
#define PDIC_SRFO_TARGET_DVD       250
#define PDIC_SRFO_TARGET_BD        1500  //BD temp all use high gain
#else
// OPU Gain (0.1dB)               0,    1,    2,    3
#define OPU_GAIN                  0,    0,  -90,    0

// OPU Gain Select                mode
#define OPU_GAIN_SELECT_CDR       0             //CD-ROM/-R
#define OPU_GAIN_SELECT_CDRW      0             //CD-RW
#define OPU_GAIN_SELECT_DVDR      1             //DVD-ROM/+R/-R SL
#define OPU_GAIN_SELECT_DVDRDL    1             //DVD-ROM/+R/-R DL
#define OPU_GAIN_SELECT_DVDRW     1             //DVD+RW/-RW SL
#define OPU_GAIN_SELECT_DVDRWDL   1             //DVD+RW/-RW DL
#define OPU_GAIN_SELECT_DVDRAM    1             //DVD-RAM
#define OPU_GAIN_SELECT_BD_RO_SL  3             //BDROM SL
#define OPU_GAIN_SELECT_BD_RO_DL  3             //BDROM DL
#define OPU_GAIN_SELECT_BD_RE_SL  3             //BDR/RE SL
#define OPU_GAIN_SELECT_BD_RE_DL  3             //BDR/RE DL
#endif

#if (EN_ADI == 1)
//-----------------------------------------------------------------------------;
// Actuator dissipation protection                                             ;
//-----------------------------------------------------------------------------;
//from sun720 start//TBD:need use 711's spec
//should be definedin OPU specific definition file
#define FOC_DRIVER_GAIN                     4    // dependent on driver
#define TRK_DRIVER_GAIN                     4    // dependent on driver

#define FOC_ACT_R                           5000   // 5000, 5000:FOO:+/-0.45, 7000:FOO:+/-0.54, spec is 5000. dependent on OPU; unit in m ohm from OPU spec
#define TRK_ACT_R                           3600   // 5000, 3600:TRO:+/-0.36, 5000:TRO:+/-0.43,  7000:TRO:+/-0.5, spec is 3600. dependent on OPU; unit in m ohm from OPU spec

#define FOC_ACT_DISS_FACTOR                 (ULONG)(32767*8*((float)(FOC_DRIVER_GAIN*PDMDAC_STEPVOLTAGE))*((float)(FOC_DRIVER_GAIN*PDMDAC_STEPVOLTAGE)))
#define TRK_ACT_DISS_FACTOR                 (ULONG)(32767*8*((float)(TRK_DRIVER_GAIN*PDMDAC_STEPVOLTAGE))*((float)(TRK_DRIVER_GAIN*PDMDAC_STEPVOLTAGE)))

#define PWR_FOC                             840    //420 unit:mW from OPU spec
#define PWR_TRK                             760    //380 unit:mW from OPU spec

#define ACTDISS_TAU                         1000   // 1000 :T tro/foo:4060/6250 , 1000:T tro/foo:14500/12500 , spec is 1000. unit:ms from OPU spec
//from sun720 end//TBD:need use 711's spec
#endif

//--------------------------------------------------------------------------------------------------------------------;
// Supported HD speed                                                                                                ;
//--------------------------------------------------------------------------------------------------------------------;
//                                       0,        1,        2,        3,        4

#define SUPPORTED_HD_CLV_SPEED   (BYTE)NOP,  _1LX_HD,  _2LX_HD,(BYTE)NOP,(BYTE)NOP

#define SUPPORTED_HD_ZCLV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

#define SUPPORTED_HD_CAV_SPEED   (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,  _4AX_HD

#define SUPPORTED_HD_PCAV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

//--------------------------------------------------------------------------------------------------------------------;
// Supported BD speed                                                                                                ;
//--------------------------------------------------------------------------------------------------------------------;
//                                   0,        1,        2,        3,        4,        5,        6,        7,
//                                   8,        9,       10,       11,       12,       13,       14,
#if(BD2p2XCLV == 1)
#define SUPPORTED_BD_CLV_SPEED   (BYTE)NOP, _1LX_BD , _2LX_BD , _3LX_BD ,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,\
                                 (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
#else
#define SUPPORTED_BD_CLV_SPEED   (BYTE)NOP,  _1LX_BD,  _2LX_BD,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,\
                                 (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
#endif

#define SUPPORTED_BD_ZCLV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,\
                                 (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

#define SUPPORTED_BD_CAV_SPEED   (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,  _4AX_BD,(BYTE)NOP,  _6AX_BD,(BYTE)NOP,\
                                   _8AX_BD,(BYTE)NOP, _10AX_BD,(BYTE)NOP, _12AX_BD,(BYTE)NOP, _14AX_BD

#define SUPPORTED_BD_PCAV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,\
                                 (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

//--------------------------------------------------------------------------------------------------------------------;
// Supported DVDRAM speed                                                                                                ;
//--------------------------------------------------------------------------------------------------------------------;
//                                        0,        1,        2,        3,        4,        5,        6,        7,
//                                        8,        9,       10,       11,       12,       13,       14,       15,
//                                       16,       17,       18,       19,       20,
#define SUPPORTED_DVDRAM_CLV_SPEED   (BYTE)NOP,(BYTE)NOP,_2LX_DVDRAM,_3LX_DVDRAM,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP
#define SUPPORTED_DVDRAM_ZCLV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP
#define SUPPORTED_DVDRAM_CAV_SPEED   (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,_5AX_DVDRAM,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP
#define SUPPORTED_DVDRAM_PCAV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                     (BYTE)NOP

//--------------------------------------------------------------------------------------------------------------------;
// Supported DVD speed                                                                                                ;
//--------------------------------------------------------------------------------------------------------------------;
//                                        0,        1,        2,        3,        4,        5,        6,        7,
//                                        8,        9,       10,       11,       12,       13,       14,       15,
//                                       16,       17,       18,       19,       20,
#if(DVD_710Run815 == 1)
#define SUPPORTED_DVD_CLV_SPEED   (BYTE)NOP,(BYTE)NOP, _2LX_DVD,(BYTE)_3LX_DVD,(BYTE)_4LX_DVD,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
#else
#define SUPPORTED_DVD_CLV_SPEED   (BYTE)NOP,(BYTE)NOP, _2LX_DVD,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
#endif
#define SUPPORTED_DVD_ZCLV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

#define SUPPORTED_DVD_CAV_SPEED   (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,_4AX_DVD,(BYTE)NOP,_6AX_DVD,(BYTE)NOP,   \
                                  _8AX_DVD, (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,_12AX_DVD,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  _16AX_DVD,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
#define SUPPORTED_DVD_PCAV_SPEED  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
//--------------------------------------------------------------------------------------------------------------------;
// Supported CD speed                                                                                                 ;
//--------------------------------------------------------------------------------------------------------------------;
//                                        0,        1,        2,        3,        4,        5,        6,        7,
//                                        8,        9,       10,       11,       12,       13,       14,       15,
//                                       16,       17,       18,       19,       20,       21,       22,       23,
//                                       24,       25,       26,       27,       28,       29,       30,       31,
//                                       32,       33,       34,       35,       36,       37,       38,       39,
//                                       40,       41,       42,       43,       44,       45,       46,       47,
//                                       48,       49,       50,       51,       52
#define SUPPORTED_CD_CLV_SPEED    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,_4LX, (BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

#define SUPPORTED_CD_ZCLV_SPEED   (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

#define SUPPORTED_CD_CAV_SPEED    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,_10AX,    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  _16AX,    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  _24AX,    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  _32AX,    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  _40AX,    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  _48AX,    (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP
#define SUPPORTED_CD_PCAV_SPEED   (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP, \
                                  (BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP,(BYTE)NOP

//--------------------------------------------------------------------------------------------------------------------;
// Zone-CLV Table (CD)                                                                                                ;
//--------------------------------------------------------------------------------------------------------------------;
#define CD_ZCLV_SPEED            _16LX,_24LX,_32LX,_40LX

//--------------------------------------------------------------------------------------------------------------------;
// Zone-CLV Table (DVD)                                                                                               ;
//--------------------------------------------------------------------------------------------------------------------;
#define DVD_ZCLV_SPEED           _6LX_DVD,_8LX_DVD,_10LX_DVD,_12LX_DVD

#endif /* _SVO_PAR_ */
