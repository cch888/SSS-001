/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_ISR.H
*
* DESCRIPTION
*
*   $Revision: 21 $
*   $Date: 10/12/20 6:20p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_ISR_
#define _SVO_ISR_

/** Prototype **/
void    disc_moter_mode_change(void);

#if DO_HW_SUPPORT_STEP == 0
void    svoSTEPInt(void);
#endif

void    svoBadSCORIsr(void);
void    svoSubqIsr(void);
void    svoPSNIsr(void);
void    svoSEEKIsr(void);
#if (DETECT_FE_DISTURB||DETECT_DVD_FE_DISTURB)
void    svoFSHCIsr(void);
#endif
#if (DETECT_TE_OFFTRACK||DVD_OFF_TRACK_TRIGGER)
void svoMIRRIsr(void);
#endif
void    svoDefectIsr(void);
#if (NEW_FDP_DETECTION != 0)
void    svoFDropIsr(void);
#endif
    #if (DVD_RAM_READ == 1) && (CHIP_REV >= 0xC0)
void land_flywheek_isr();
void seqPID_Error_Isr();
    #endif
#if (E_DIAGS == 1)
void diagC1C2(void);
#endif
    #if (DVD_RAM_READ == 1) && (CHIP_REV >= 0xC0)
void land_flywheek_isr();
void seqPID_Error_Isr();
    #endif
/** External functions **/

/** External variables **/

#endif          /* #ifndef _SVO_ISR_ */
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
