/* ******************************************************************************************
*
*   (c) Copyright 2002 - 2006 Sunext Design, INC.
*               All Rights Reserved
*
* **************************** SUNEXT CONFIDENTIAL ******************************
*
*  Filename:
*     HAL_host.c
*
*  Description:
*   This file contains functions for the Host subsystem and includes
*   the following:
*          HAL_HOST_AesStartBusEncryption
*
* Notes:
*
*
*   $Revision: 4 $
*   $Date: 09/06/02 5:53p $
*
-----------------------------------------------------------------------------------------------------*/

#include ".\common\globals.h"
#include ".\common\vu_cfg.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_host.h"
#include ".\AL\HAL_host.h"
#if (ENABLE_AACS_BEF == 1)
#include ".\crypt\aacs.h"
#include ".\player\plrdb.h"
#endif

/*** Local defines ***/

/*** Local static variables */
#if (ENABLE_AACS_BEF == 1)
BYTE const CbcInitVector[16] = {0x0B, 0xA0, 0xF8, 0xDD, 0xFE, 0xA6, 0x1F, 0xB3,
                                0xD8, 0xDF, 0x9F, 0x56, 0x6A, 0x05, 0x0F, 0x78};
#endif


#if (ENABLE_AACS_BEF == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_StartAesBusEncryption

    Description:    Activate the bus encryption function for each transfered sector.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_HOST_StartAesBusEncryption(void)
{
    BYTE i;
    ULONG   DelayTimer;
    WRITE_FIELD(HAL_HST_AESRST, 1);
    WRITE_FIELD(HAL_HST_AESRST, 0);
    
    WRITE_REG(HAL_HSTREG_MAP->AESCONFIG, HAL_HST_SELAESKEY|HAL_HST_BUSENCPT|HAL_HST_CBCENABLE|HAL_HST_ENCRYPT);
                
    // wait for the AES module to accept a new key
    StartTimer(&DelayTimer);
    while ( (READ_REG(HAL_HSTREG_MAP->AESSTATUS) & HAL_HST_ACCEPTKEY_MASK) == FALSE ) 
    {
        if (ReadTimer(&DelayTimer) >= 100)
        {
            send_msg5S(1, "AES TOUT");
            break;
        }
    }
    
    WRITE_FIELD(HAL_HST_SUBCPUMOD, 1);
    
    // Program 128-bit read data key
    WRITE_FIELD(HAL_HST_AESPTRRST, 1);
    WRITE_FIELD(HAL_HST_AESPTRRST, 0);
    for (i = 0; i < 16; i++)
        WRITE_REG(HAL_HSTREG_MAP->AESKEY, AacsAuth.ReadDataKey[i]);
    
    // Program 128-bit CBC initial value
    WRITE_FIELD(HAL_HST_AESPTRRST, 1);
    WRITE_FIELD(HAL_HST_AESPTRRST, 0);
    for (i = 0; i < 16; i++)
        WRITE_REG(HAL_HSTREG_MAP->AESCBC, CbcInitVector[i]);
        
    WRITE_FIELD(HAL_HST_SUBCPUMOD, 0);
    WRITE_FIELD(HAL_HST_AESPTRRST, 1);
    WRITE_FIELD(HAL_HST_AESPTRRST, 0);
    
    if (pdbDiscLoaded() == BD_DISC)
    {
        WRITE_FIELD(HAL_HST_AESSTRTOF, 1);
        WRITE_REG(HAL_HSTREG_MAP->AESLENGTH, 127);
    }
    else
    {
        WRITE_FIELD(HAL_HST_AESSTRTOF, 8);
        WRITE_REG(HAL_HSTREG_MAP->AESLENGTH, 120);
    }
    
    // Enable bus encryption
    WRITE_FIELD(HAL_HST_BUSENCEN, 1);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_StopAesBusEncryption

    Description:    Stop the bus encryption function for each transfered sector.

    Inputs:         None
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void HAL_HOST_StopAesBusEncryption(void)
{    
    // Disable bus encyrption
    WRITE_FIELD(HAL_HST_BUSENCEN, 0);
    WRITE_FIELD(HAL_HST_BUSENCPT, 0);
        
    // Disable AES Module
    WRITE_FIELD(HAL_HST_AESCLKGAT, 1);
}
#endif  // ENABLE_AACS_BEF == 1


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
