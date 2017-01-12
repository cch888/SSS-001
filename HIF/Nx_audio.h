/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
*
* FILENAME:   nx_audio.h
*
* DESCRIPTION
* This file contains buffer audio data in NX and play back in 1x
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __NX_AUDIO_H__
#define __NX_AUDIO_H__

extern void PlayAudioNx1x(void);
extern void MonitorAudioNx(void);
extern void AudioIntProc(void);
extern void EnterNxTo1x(void);

#endif /* __NX_AUDIO_H__ */
