/****************************************************************************
*                (c) Copyright 2001 -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrMisc.c
* DESCRIPTION   : The plrMisc subsystem will perform a number of different jump actions,
*                 activated by other processes.
*                 These jumps will be based on subcode or on data.
*
* $Revision: 7 $
* $Date: 11/03/16 6:00p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\oem\oem_info.h"
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\error.h"
#include ".\common\system.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h) */
#include ".\player\plrDB.h"
#include ".\player\plrMisc.h"
#include ".\player\plrWatch.h"
#include ".\player\plrServo.h"

/*************************************************************************/
/*                              Define                                   */
/*************************************************************************/
#define kMaxAllowEDSubcodeError 10
#define kMaxAllowEDEFMSyncError 3		/* search EFM sync error retry count */
#define kMaxEFMSyncErrorReSeek  2		/* search EFM sync error reseek count */
#define kJumpTimeout            2000L   /* 2000m */

/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
private BYTE Time(void);
private BYTE Time_01(void);  private BYTE Time_01E(void);
private BYTE Time_02(void);  private BYTE Time_02E(void);

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/
private ULONG       fJumpTimer; /* Jump Timer       */
private eSeekMode   fSeekMode;  /* Misc Seek Mode   */

/* Time  */
private const tTask fTime[] = {
/*          Ready   Error   */
/* Ph00 */ {0      ,0       },
/* Ph01 */ {Time_01,Time_01E},
/* Ph02 */ {Time_02,Time_02E},
};


/* Command Table */
public const tCmd fMiscTask[] = {
    {(LPCHAR) "Idle",0    ,0     }, /* Idle  */
    {(LPCHAR) "Time",Time ,fTime }, /* Time  */
};

/************************************************************************
// Name        : Time Command
*************************************************************************/
private BYTE Time()
{
    LONG tmpLBA;

    /* If the disc type is not an ATIP disc OR the speed is GT 8x CLV OR CAV */
    if((pdbServoGetDiscType() != eATIPDisc) || (pdbGetSpeed() > e12XCLV)){
        fSeekMode = eDataSeek;
    }
    else{
        fSeekMode = eTimeSeek;
    }
    /* Watch Request */
    tmpLBA = ByteLen4ToLong(&fPlrMsg.data[1]);
    plrServoSeek(tmpLBA,fSeekMode,ePhase01);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01-Servo Status Change
-------------------------------------------------------------------------*/
private BYTE Time_01()
{
    pdbSetSubQIntStatus(FALSE);
    StartTimer(&fJumpTimer);
    /* Watch Request */
    plrWatchReq(ePhase02, eWatch_Seek, (BYTE *) &fSeekMode);
    return(Busy);
}
/*----------------------------------------------------------------------*/
private BYTE Time_01E()
{
    /* Set read error group to header search error */
    pdbSetErrorGroupRead( eErrorNoEFMAtSearchAddress );
    return(Error);
}

/*------------------------------------------------------------------------
// Phase02-Time Int
-------------------------------------------------------------------------*/
private BYTE Time_02()
{
    return(Ready);
}
/*----------------------------------------------------------------------*/
private BYTE Time_02E() {
    /* if jumper timer has timed out */
    if(ReadTimer(&fJumpTimer) >kJumpTimeout){
        /* Set read error group to header search error */
        pdbSetErrorGroupRead( eErrorNoEFMAtSearchAddress );
        return(Error);
    }
    else{
        /* Watch Request */
        plrWatchReq(ePhaseSame,eWatch_Seek, (BYTE *) &fSeekMode);
        return(Busy);
    }
}


