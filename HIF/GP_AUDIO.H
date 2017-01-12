/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_audio.h
*
* DESCRIPTION:
*   This file contains play audio command handler of ATAPI commands
*
* $Revision: 4 $
* $Date: 06/05/25 2:50a $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_AUDIO_H__
#define __GP_AUDIO_H__

#include ".\common\error.h"
#include ".\player\plrdb.h"


extern void PlayAudioMSFCommand(void);
extern void PlayAudio10Command(void);
extern void ScanCommand(void);
extern void StopPlayScanCommand(void);

#if (AUDIO_PLAYBACK == 1)
extern void AudioTransferDone(void);
extern void SetSpeakerVolume(BYTE, BYTE, BYTE, BYTE);
extern void SetAudioVolume(BYTE LChVolume,BYTE RChVolume);
extern void SetAudioDacMode(AudioDACOutputControlT dac);
extern BOOL SetAudioChannelMode(BYTE);
extern void MuteOn(void);
extern void MuteOff(void);
extern void SetDefaultVolume(BYTE l_vol,BYTE r_vol);
extern void RampUpVolume(void);
extern void RampDownVolume(void);

#define EnableCdAudioBuffering()
#define DisableCdAudioBuffering()
#endif // AUDIO_PLAYBACK
#define PlayAudio12Command  PlayAudio10Command

#endif /* __GP_AUDIO_H__ */

