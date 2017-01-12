/****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
* FILENAME:     dvCFG.h
*
* DESCRIPTION   This file contains DVD system configuration and conditional
*               switches
*
*
* $Revision: 19 $
* $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __DVCFG_H__
#define __DVCFG_H__

#include ".\common\Vu_cfg.h"


/*----------------------------------------------------------------------------
    Enable/Disable DVD caching

    TRUE    = Caching enabled
    FALSE   = Caching disabled. Always cache miss for every read
-----------------------------------------------------------------------------*/
#define DVD_READ_AHEAD_ENABLED  TRUE
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1) ||(SEQ_SEEK_SPEED_DOWN == 1) 
#define CONTINUE_RANDOM_READ_CNT    30 
#endif
/*----------------------------------------------------------------------------
    DVD-DSP to AFE interface setting
-----------------------------------------------------------------------------*/
#define DVD_AFE_INTERFACE  8            /* DVD Read Interface setting */
                                        /* 1 = 1-bit Serial           */
                                        /* 2 = 2-bit Serial           */
                                        /* 4 = 4-bit Serial           */
                                        /* 8 = 8-bit Serial           */

/*----------------------------------------------------------------------------
    SYNC ERROR COUNT THRESHOLD

    This defines the number of consecutive sync errors that can occur per ecc
    block before the DVD DSP transitions to a sync lost state
-----------------------------------------------------------------------------*/
#define DVD_SYNC_ERROR_THRESHOLD    0x20

/*----------------------------------------------------------------------------
    SYNC DETECTION WINDOW WIDTH

    This defines the window width to detect a sync pattern from the normal
    1488 bit stream. The following shows the adjustment of this detection
    window:

    Window =    [ 1488 - SYNC_DETECT_WINDOW ] to [ 1488 + SYNC_DETECT_WINDOW ]
-----------------------------------------------------------------------------*/
#define DVD_SYNC_DETECT_WINDOW  0x20

/*----------------------------------------------------------------------------
    MULTI-PASS ERROR CHECKING

    This defines the maximum number of error checks and correction attempts
    made on each DVD ecc block before posting an ecc block error. Maximum is
    8 passes

    0 = 1 error correction pass
    1 = 2 error correction passes
    ...
    Max value = 7
-----------------------------------------------------------------------------*/
#define DVD_MULTI_PASS_COUNT    0x07
#define DVDRAM_MULTI_PASS_COUNT     0x03

/*----------------------------------------------------------------------------
    DVD BLOCK PERIOD INTERRUPT

    This defines the number of corrected ecc blocks that will be loaded into
    the buffer before an ecc block interrupt (ECCI) is generated

    0 = block int every 1 ecc block
    1 = block int every 2 ecc blocks
    2 = block int every 3 ecc blocks
    ...
    Max value = 0xFF

-----------------------------------------------------------------------------*/
#define DVD_ECC_INTERRUPT_PERIOD    0x00


/*----------------------------------------------------------------------------
    DVD ERROR COUNTING

    Conditional     DVD_ERROR_COUNTING

    Description     Counting of the following DVD errors while buffering
                    DVD data: EDC, Uncorrectable ECC, Sync Lost, Sequential ID

    States          1   Enables the counting of DVD buffering errors.

                    0   Disables error counting
-----------------------------------------------------------------------------*/
#define DVD_ERROR_COUNTING  0



/*----------------------------------------------------------------------------
    GENERATED SIGNAL INPUT (AWG)

    Sets up the DVD system to recieve generated signal from an AWG. Need to
    have servo disabled by setting conditional WITHOUT_SERVO

    1   AWG debug mode
    0   Normal code operation

-----------------------------------------------------------------------------*/
#define DVD_AWG_TEST        0

/*----------------------------------------------------------------------------
    TYPE OF AWG INPUT

    This determines whether input from AWG bypasses the AFE entirely.
-----------------------------------------------------------------------------*/
#define DVD_AWG_BYPASS_AFE      1       // 1 Bypass AFE entirely.
                                        //     - This setting is chosen to feed a Digital AFE pattern into MIO0 / MIO1 inputs bypassing the AFE
                                        //     - or to feed Digital AFE patterns into the DVD Decoder using "Bootstrap DVD Test Mode"
                                        // 0 AWG pattern goes through AFE.
                                        //     - This setting is chosen to feed an analog AWG pattern through RFp input
                                        //     - or to feed a EFM (NRZ) AWG pattern into PB3. This setting bypasses the AFE Slicer.


#define DVD_AWG_TEST_SPEED      4       // DVD Test Speed / X

#define DVD_AWG_TEST_ANALOG     0       // 1 -  Use Analog Pattern and feed into RFp input of AFE
                                        // 0 -  Use NRZ Pattern and feed Pattern into Digital input PB3 of AFE
                                        // If DVD_BYPASS_AFE = 1 then this setting has no effect.


/*----------------------------------------------------------------------------
    DVD_VERIFY_ID

    Used for checking the incoming PSN. Compares the assumed PSN (LastDecodedPSN + 1)
    with the actual ID from the auxillary area of the block

    1   Enable checking
    0   Disable checking

    NOTE: For debugging only. This conditional can be removed after verification

-----------------------------------------------------------------------------*/
#define DVD_VERIFY_ID	1	// 2011.03.28 Enable it for Fixed Target mismatch under servo slip during buffering.


/*----------------------------------------------------------------------------
    DVD_ZONE_TEST

    Sets up a test for reading ecc blocks without host transferring. This
    test will read specified ranges on the disc using different CTF values
    in order to determine which is the best CTF value. The drive will
    behave normally after the test completes

    0  Disabled (Default)
    1  Enabled

    NOTES:

    The output is displayed through the serial port so the following
    conditionals in vu_cfg.h must be turned on for this test code:

        PRN_DVDHIF   = TRUE
        ENABLE_UART_TXD = TRUE

    This conditional and its related code can be removed once the CTF
    values have been proplerly adjusted for all DVD speeds

-----------------------------------------------------------------------------*/
#define DVD_ZONE_TEST  0

/*----------------------------------------------------------------------------
    DVD_DISABLE_CSS_CHECK

    Allows the buffering and transferring of scrambled data if authentication
    has not taken place and the disc is copy protectd. This conditional is
    used to test dvd buffering and transferring of data from copy protected
    discs to a host without concern for data content.

    0  Disabled (Default)
    1  Enabled

-----------------------------------------------------------------------------*/
#define DVD_DISABLE_CSS_CHECK  0

/*----------------------------------------------------------------------------
    PI Symbols Corrected Per Row Threshold
-----------------------------------------------------------------------------*/
#define DVD_PI_ROW_ERROR_THRESHOLD  0

/*----------------------------------------------------------------------------
    CID ERROR COUNTER THRESHOLD:
    Contains the maximum number of consecutive CID errors allowed. If this 
    value is exceeded,the CID flywheel is reset.
-----------------------------------------------------------------------------*/
#define DVD_CID_ERROR_THRESHOLD 0x03
#endif
