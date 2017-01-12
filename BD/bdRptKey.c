/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRptKey.c $
*
* DESCRIPTION   This module contains functions related to AACS authentication.
*
* $Revision: 11 $
* $Date: 09/08/20 6:55p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

// ***************************************************************************
//                               HEADERS
// **************************************************************************/
#include ".\common\globtype.h"
#include ".\hif\gp_ram.h"
#include ".\oem\oem_info.h"
#include ".\bd\bdrptkey.h"
#include ".\bd\bdvars.h"
#include ".\bd\bddiscrd.h"
#include ".\bd\bdmacro.h"

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
extern void ReportKeyCmd_AACS(void);            // CE-Auth.
#endif

/*********************************************************************
* Name:         BDReportKeyCommand
*
* Description:  Report Key command handler for BD.
*
* Returns:      void
*
* Notes:
*
*********************************************************************/
void BDReportKeyCommand(void)
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
        ReportKeyCmd_AACS();                    // CE-Auth.
        return;
    }
#endif

    AACSReportKey();
#else
    BUILD_SENSE(0x05,0x6F,0x01,0x05);                           /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
    AutoCommandComplete();
#endif
}

#endif  /* BD_ENABLE == 1 */
