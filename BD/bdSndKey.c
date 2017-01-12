/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdSndKey.c $
*
* DESCRIPTION   This module contains functions related to AACS authentication.
*
* $Revision: 8 $
* $Date: 08/08/14 5:35a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

// ***************************************************************************
//                               HEADERS
// **************************************************************************/
#include ".\common\globtype.h"
#include ".\hif\gp_ram.h"
#include ".\oem\oem_info.h"
#include ".\bd\bdsndkey.h"
#include ".\bd\bdrptkey.h"
#include ".\bd\bdvars.h"

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

#if (BD_ENABLE == 1)
// ***************************************************************************
//                               VARIABLES
// ***************************************************************************


// ***************************************************************************
//                              EXTERNS
// ***************************************************************************


// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************
#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
extern void SendKeyCmd_AACS(void);              // Samsung CE-Auth.
#endif

/*********************************************************************
* Name:         BDSendKeyCommand
*
* Description:  Send Key command handler for BD.
*
* Returns:      void
*
* Notes:
*
*********************************************************************/
void BDSendKeyCommand(void)
{
#if (ENABLE_AACS == 1)
    /* Force to stop BD Decoder to make sure AACS authenticaton works well */
    bdStopDiscTransfer();
    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

    bdDecFlag.fDISABLE_READ_AHEAD = TRUE;

    // Disable Read Ahead will make the buffer is almost empty, and that will cause speed up.
    // Clear SpeedCount to avoid speeding up.
    SpeedCount = 0;

#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
    if(Iram_Cmd_block.bf[7] == 0x33)            /*Key Class 0x33*/
    {
        SendKeyCmd_AACS();                      // Samsung CE-Auth.
        return;
    }
#endif

    AACSSendKey();
#else
    BUILD_SENSE(0x05,0x6F,0x01,0x06);                           /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
    AutoCommandComplete();
#endif  /* ENABLE_AACS == 1 */
}

#endif  /* BD_ENABLE == 1 */
