/****************************************************************************
*                (c) Copyright 2001 -  2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   s2b_main.c
*
* DESCRIPTION
* This file contains the interface to the Player.
*
*
*   $Revision: 7 $
*   $Date: 07/10/09 10:28a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrdb.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\s2b_main.h"
#include ".\hif\app_if.h"

#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

/*--------------------------------------------------------------------
  Name: PlayerMain()
  Description:
    Player main routine
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void PlayerMain(void)
{
    ENABLE_INTERRUPT();
    for(;;){
        #if (WITHOUT_SERVO == FALSE)
        plrMon();
        #endif
        ExitProcess();
    }
}


/*--------------------------------------------------------------------
  Name: InitializePlayer()
  Description:
    Initializes Player variables and Player/Servo modules.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void InitializePlayer(void)
{
    #if (WITHOUT_SERVO == FALSE)
    plrPlayerInit();
    plrMonInit();
    #endif
}

