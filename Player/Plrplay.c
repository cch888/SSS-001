/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrPlay.c
* DESCRIPTION   : This is the subsystem for handling reading and playing the
*                 disc.
* NOTES         :
*
* $Revision: 17 $
* $Date: 09/07/01 4:19p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\error.h"
#include ".\hif\gp_ram.h"
#include ".\servo\svo.h"
#include ".\player\plrDB.h"
#include ".\player\plrPlay.h"
#include ".\player\plrMisc.h"
#include ".\player\plrWatch.h"
#include ".\player\plrServo.h"

extern void MuteOn(void);
/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
/* Play */
// private BYTE Play(void);
// private BYTE Play_01(void);     private BYTE Play_02(void);
// private BYTE Play_03(void);     private BYTE Play_04(void);
/* Stop Playing */
private BYTE Stop(void);        // private BYTE Stop_01(void);
/* Pause On Init */
private BYTE PauseOn(void);
/* Search For Address & Find ISRC Code */
private BYTE Search(void);      private BYTE Search_01(void);
// private BYTE Search_02(void);
/* Jump To Address */
private BYTE JumpAdrs(void);
/* Init Servo */
private BYTE InitServo(void);
/* Speed Change */
private BYTE SpeedChg(void);
/* Lead In */
private BYTE LeadIn(void);      private BYTE LeadIn_01(void);
/* Re Adjust Disc Type */
private BYTE ReAdjust(void);
private BYTE Seek_E(void);

private BYTE PassThrough(void);                   // Support for Player command Servo Pass Through.
private BYTE PassThrough_E(void);                 // Support for Player command Servo Pass Through.

/*************************************************************************/
/*                         Global  Variables                             */
/*************************************************************************/

/* Stop Playing */
private const tTask fStop[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {0          ,0     }
};

/* Pause On Init */
private const tTask fPauseOn[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {0          ,0     }
};

/* Search For Address & Find ISRC Code */
private const tTask fSearch[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
//  /* Ph01 */ {Search_01  ,0     },
/* Ph01 */ {Search_01  ,Seek_E},
/* Ph02 */ {0          ,0     }
};

/* Jump To Address */
private const tTask fJumpAdrs[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {0          ,Seek_E},
/* Ph02 */ {0          ,0     }
};

/* Init Servo */
private const tTask fInitServo[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {0          ,0     }
};

/* Speed Chg */
private const tTask fSpeedChg[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {0          ,0     }
};

/* Play Lead In */
private const tTask fLeadIn[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {LeadIn_01  ,0     },
/* Ph02 */ {0          ,0     }
};

/* ReAdjust Disc Type */
private const tTask fReAdjust[] = {
/*          Ready       Error */
/* Ph00 */ {0          ,0     },
/* Ph01 */ {0          ,0     }
};

/* Servo Pass Through Command */
private const tTask fPassThrough[] = {          /* Support for Player command Servo Pass Through. */
/*          Ready       Error */
/* Ph00 */ {0          ,0             },
/* Ph01 */ {0          ,PassThrough_E }
};

/* Command Table */
public const tCmd fPlayTask[] = {
    {(LPCHAR) "Idle",0        ,0         },/* Idel            */
////  {(LPCHAR) "Play",Play     ,fPlay     },/* Play            */
    {(LPCHAR) "Stop",Stop     ,fStop     },/* Stop            */
    {(LPCHAR) "PaOn",PauseOn  ,fPauseOn  },/* Pause On        */
    {(LPCHAR) "FISC",Search   ,fSearch   },/* Find ISRC Code  */
    {(LPCHAR) "JAdd",JumpAdrs ,fJumpAdrs },/* Jump Address    */
    {(LPCHAR) "IBas",InitServo,fInitServo},/* Init Basic      */
    {(LPCHAR) "SChg",SpeedChg ,fSpeedChg },/* Speed Chg       */
    {(LPCHAR) "LeIn",LeadIn   ,fLeadIn   },/* Lead In         */
    {(LPCHAR) "ReAD",ReAdjust ,fReAdjust },/* Re-Adjust       */
    {(LPCHAR) "Pass",PassThrough ,fPassThrough },/* Servo Pass-Through   */    // Support for Player command Servo Pass Through.
};

/************************************************************************
// Name        : Stop
// Description : This function is to stop current playing or reading
//               * set output format = CD_ROM
//               * set audio output = off
//               * set audio volume = mute
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE Stop(void)
{
    if(St_nx_audio == TRUE){    /* dont stop if nx audio playing */
        /* Servo Watch */
        plrWatchReq(ePhase01,eWatch_ServoStatus,0x00);
        return(Busy);
    }

    if(pdbGetOutputFormat() != eCDRom){
        pdbSetOutputFormat(eCDRom);
#if (AUDIO_PLAYBACK == 1)
        MuteOn();
#endif
    }
    return(Ready);
}

/************************************************************************
// Name        : PauseOn
// Description : This function sets the player status to pausing
//               if he is currently playing.
//               The decoder and encoder are disabled and the pause command
//               is given to the servo subsystem.
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE PauseOn(void)
{
    pdbSetPlayerStatus(eStPausingOnAdrs);
    /* Servo Watch */
    plrServoPause(ePauseOn,ePhase01);

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    return (Ready);
#endif

    return(Busy);
}

/************************************************************************
// Name        : Search
// Description : This function will search for an address on the disc.
//               It is like the "plrPlayJumpToAddress" routine.
//               The difference is this routine will calculate a offset
//               that will be subtracted from the target address before
//               the real search begins.
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE Search(void)
{
    AddrParmS  AddressLBA;
    LONG     tmpLBA;

    pdbSetPlayerStatus(eStAccessing);

    /* Misc Request */
    pdbGetSearchAddress(&AddressLBA);
    tmpLBA = ByteLen4ToLong(&AddressLBA.Addr[0]);
    plrServoSeek(tmpLBA,eDataSeek,ePhase01);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01(From Cmd)
// Description : Before this process is executed, the plrPlay process has
//               searched for the start of the requested track.
//               This function reads the catalog and the International
//               Standard Recording Code from the subcode.
-------------------------------------------------------------------------*/
//Y.97private BYTE Search_01(void)
//Y.97{
//Y.97  AddrParmS  AddressLBA;
//Y.97  LONG     tmpLBA;
//Y.97
//Y.97  /* Misc Request */
//Y.97  pdbGetSearchAddress(&AddressLBA);
//Y.97  tmpLBA = ByteLen4ToLong(&AddressLBA.Addr[0]);
//Y.97  plrServoSeek(tmpLBA,eDataSeek,ePhase02);
//Y.97  return(Busy);
//Y.97  }

/*------------------------------------------------------------------------
// Phase01(From Cmd)
// Description : Before this process is executed, the plrPlay process has
//               searched for the start of the requested track.
//               This function reads the catalog and the International
//               Standard Recording Code from the subcode.
-------------------------------------------------------------------------*/
private BYTE Search_01(void)
{
    pdbSetPlayerStatus(eStSearchISRCAndCatNum);

    //pdbSetPlayerStatus(eStPausingOnAdrs);
    plrWatchReq(ePhaseInc,eWatch_ISRC,0);
    return(Busy);
}

/************************************************************************
// Name        : JumpAdrs
// Description : This function directs the laser to the new address on disc.
//               This address must be set before this function is called.
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE JumpAdrs(void)
{
    AddrParmS  AddressLBA;
    LONG       tmpLBA;

    pdbSetPlayerStatus(eStPausingOnAdrs);
    /* Misc Request */
    pdbGetSearchAddress(&AddressLBA);
    tmpLBA = ByteLen4ToLong(&AddressLBA.Addr[0]);
//  plrServoSeek(tmpLBA,eDataSeek,ePhase01);
    plrServoSeek(tmpLBA,fJumpMode,ePhase01);
    return(Busy);
}

/************************************************************************
// Name        : InitServo
// Description : This function sends a disc out command to the servo.
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE InitServo(void)
{
    BYTE svostate;
    tWatchSC ServoCmd;  /* Servo Command   */

    pdbSetPlayerStatus(eStInitServo);
    /* if servo status is svoStIdleDiscKnown then initialize the servo status */
    svostate = pdbGetServoState();
    if(svostate == svoStIdleDiscKnown){
        /* Servo Watch */
        ServoCmd.Cmd = svoCmdTray;
        ServoCmd.Len = 1;
        ServoCmd.Para[0] = eMoveTrayOut;
        plrWatchReq(ePhase01,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    }
    else{
        if((svostate == svoStStoppingServo) || (svostate == svoStInitialized)){
            return(Ready);
        }
    }
    return(Busy);
}

/************************************************************************
// Name        : SpeedChg
// Description :
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE SpeedChg(void)
{

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    return (Ready);
#endif

    /* Servo Watch */
    plrServoSetSpeed(pdbGetSpeed(),ePhase01);
    return(Busy);
}

/************************************************************************
// Name        : LeadIn
// Description :
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE LeadIn(void)
{
    TimeS Address; /* Search Address */
    LONG  tmpLBA;

    pdbSetPlayerStatus(eStPausingOnAdrs);
    mTimeSet(Address,0,2,0);/* issue JUMP time to 0:2:0 command to servo */
    /* Misc Request */
    tmpLBA = 0L;
    plrMiscReq(ePhase01,eMiscTime,tmpLBA);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01(From Cmd)
-------------------------------------------------------------------------*/
private BYTE LeadIn_01(void)
{
    SHORT Number = -650;
    tWatchSC ServoCmd;              /* Servo Command   */

    /* Servo Watch */
    ServoCmd.Cmd = svoCmdJumpGrooves;
    ServoCmd.Len = 3;
    ServoCmd.Para[0] = (BYTE) Number;
    ServoCmd.Para[1] = (BYTE)(Number >> 8);
    ServoCmd.Para[2] = eTimeJump;
    plrWatchReq(ePhaseInc,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}



/************************************************************************
// Name        : ReAdjust
// Description : This function is to Re Adjust the Disc Type
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE ReAdjust(void)
{
    /* Servo Watch */
    plrServoReAdjust(fReAdjustType, ePhase01);
    return(Busy);
}

/************************************************************************
// Name        : PassThrough
// Description : This function is to Pass a command directly to Servo
// Returns     : Ready,Busy or Error
*************************************************************************/
private BYTE PassThrough(void)                                                  // Support for Player command Servo Pass Through.
{
    plrServoPassThrough(fReAdjustType, ePhase01);
    return(Busy);
}

/************************************************************************
// Name        : PassThrough_E
// Description : PassThrough Command Error
// Returns     :
*************************************************************************/
private BYTE PassThrough_E(void)                                                  // Support for Player command Servo Pass Through.
{
    return(Error);
}

/*------------------------------------------------------------------------
// Seek Error
-------------------------------------------------------------------------*/
private BYTE Seek_E(void)
{
    pdbSetErrorGroupRead(eErrorNoEFMAtSearchAddress);
    return(Error);
}

/*********************************************************************
* Function:     ByteLen4ToLong()
*
* Description:  Converts a BYTE address to LONG address
*
* Returns:      long address
* Params:       byte address
*********************************************************************/
long ByteLen4ToLong(BYTE *Addr)
{
    long address;

    address  = ((long)*Addr++) << 24;
    address += ((long)*Addr++) << 16;
    address += ((long)*Addr++) << 8;
    address += (long)*Addr;

    return(address);
}
