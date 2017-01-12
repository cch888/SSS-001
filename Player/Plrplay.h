/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrPlay.h
* DESCRIPTION   : This file contains the Play module definitions
* NOTES         :
*
* $Revision: 3 $
* $Date: 06/04/03 5:37a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _PLRPLAY_H_
#define _PLRPLAY_H_

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\player\plrmon.h"
#include ".\player\plrdb.h"

/*************************************************************************/
/*                        Data type definitions                          */
/*************************************************************************/
/* Command */
enum {
    ePlayIdle = eIdle, /* Idel            */
//  ePlayPlay        , /* Play            */
    ePlayStop        , /* Stop            */
    ePlayPauseOn     , /* Pause On        */
    ePlayFindISRCCode, /* Find ISRC Code  */
    ePlayJumpAddress , /* Jump Address    */
    ePlayInitBasic   , /* Init Basic      */
    ePlaySpeedChg    , /* Speed Chg       */
    ePlayLeadIn      , /* Lead In         */
    ePlayReAdj       , /* Readjust disc type */
    ePlayPassThrough , /* Readjust disc type */          /* Support for Player command Servo Pass Through.  */
    ePlayEnd           /* End             */
};

#endif /* _PLRPLAY_H_ */
