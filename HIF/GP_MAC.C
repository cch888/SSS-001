/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_mac.c
*
* DESCRIPTION
* This file contains general purouse macros
*
* NOTES:
*
*
* $Revision: 34 $
* $Date: 11/03/15 1:26p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_util.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_global.h"
#include ".\al\reg_cpycmp.h"

#if  SERVOKEYPARAMETER == 1
extern BYTE ServoParaSwitch;
#endif
#if (STORE_MSG_TO_SDRAM == 1)
extern BYTE    LogStopCondition;
#endif
ULONG PacketCommandToLBALEN4(BYTE a);
ULONG PacketCommandToLBALEN3(BYTE a);
ULONG PacketCommandToLBALEN2(BYTE a);
TimeS Method2LBAToMSF(long LBA2);
void ZeroFillBlocks(USHORT SBLK,USHORT EBLK);

ULONG PacketCommandToLBALEN4(BYTE a)
{
    return(((ULONG)Iram_Cmd_block.bf[a  ] << 24) +
           ((ULONG)Iram_Cmd_block.bf[a+1] << 16) +
           ((ULONG)Iram_Cmd_block.bf[a+2] <<  8) +
           ((ULONG)Iram_Cmd_block.bf[a+3]      ));
}

ULONG PacketCommandToLBALEN3(BYTE a)
{
    return(((ULONG)Iram_Cmd_block.bf[a  ] << 16) +
           ((ULONG)Iram_Cmd_block.bf[a+1] <<  8) +
           ((ULONG)Iram_Cmd_block.bf[a+2]      ));
}

ULONG PacketCommandToLBALEN2(BYTE a)
{
    return(((ULONG)Iram_Cmd_block.bf[a  ] << 8) +
           ((ULONG)Iram_Cmd_block.bf[a+1]     ));
}

/*********************************************************************
* Function Name: BUILD_SENSE
*
* BUILD_SENSE set up the specified sense error
*********************************************************************/
#if  SERVOKEYPARAMETER == 1
SENSE_T KeyPara_Sense;            /* Request Sense return data */
#endif
void BUILD_SENSE(BYTE snsky,BYTE snscode,BYTE snsqual,BYTE debugbyte)
{
    Iram_Sense.key = snsky;
    Iram_Sense.cod = snscode;
    Iram_Sense.qul = snsqual;

    if((debugbyte) && (snsky))
    {
        SendMsgCn(SEND_ATAPI_CMD,4,0x800315,snsky,snscode,snsqual,debugbyte);   // BUILD_SENSE = (%02X, %02X, %02X, %02X)
    }

    if(snsky == 0x02 && Iram_Sense.cod == 0x3A)
    {
    	  if(svoVar_discKindT.fDiscKind == eDVDRAMDisc)
    	  {
    	 		Iram_Sense.cod = 0x06;
    		}
    		else
    		{
        Iram_Sense.cod = snscode;
        }
    }
#if  SERVOKEYPARAMETER == 1
    if((snsky == 0x03)||(snsky == 0x04)||((snsky ==0x06)&&(snscode==0x2E)))
    {
        send_msg5S(1,"KeySsCode");//
        ServoParaSwitch = TRUE;
        KeyPara_Sense.key = snsky;
        KeyPara_Sense.cod = snscode;
        KeyPara_Sense.qul = snsqual;
    }
#endif

#if (STORE_MSG_TO_FLASHROM==1)
    if((snsky == 0x03)||(snsky == 0x04))
    {
        if(EnableAutoSaveLog == TRUE)
        {
            SaveLogToROMSwitch = TRUE;
        }
    }
#endif
#if (STORE_MSG_TO_SDRAM == 1)
    if((snsky == 0x03)||(snsky == 0x04)||(snsky == 0x05)||(snsky == 0x06))
    {
        if((LogStopCondition>>snsky)&bit0)
        {
            EnableLogging = FALSE;
        }
    }
#endif
    if(snsky != 0x00)
        ReadyLED(LED_OFF);

    #if(PCB == 710)||(PCB == 711)||(PCB == 817) //TestUnitReadyCommand fail set LED_OFF when servo Start up.
    if (St_become_ready == TRUE)
        ReadyLED(LED_ON);
    #endif
}

/*********************************************************************
* Function Name: Method2LBAToMSF
*
* This new macro transfer LBA TO MSF
*********************************************************************/
TimeS Method2LBAToMSF(long LBA2)
{
    long LBA1;

    if(!(Method2LBAToMethod1LBA(LBA2, &LBA1)))
    {
        return(LBA2MSF(LBA1));          //LBA2 is converted
    }
    else
    {
        return(LBA2MSF(LBA2));          //LBA2 cannot converted, use as it
    }
}

/*********************************************************************
* Function Name: ZeroFillBlocks
*
* This Function will fill all bytes between the start and end address
* with 00h
* Input: SBLK - Starting Page Number, EBLK - Ending Page Number
*********************************************************************/
void ZeroFillBlocks(USHORT SBLK,USHORT EBLK)
{
    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,0);

    WRITE_FIELD(HAL_CPYCMP_CPYTGT,SBLK);                    /* Target Start Page */
    WRITE_FIELD(HAL_CPYCMP_CPYTOF, 0x00);                   /* Target Offset */
    WRITE_FIELD(HAL_CPYCMP_CPYPCNT, (EBLK - SBLK + 1) -1);  /* Spec: Total Page Count - 1 */
    WRITE_FIELD(HAL_CPYCMP_CPYWCNT, 0x500);                 /* Target Word Count = 400h Page Words + 100h Aux Words */
    WRITE_FIELD(HAL_CPYCMP_CPYPAT, 0x00);                   /* Zero Pattern */

    WRITE_FIELD(HAL_CPYCMP_SFLAG, 1);                       /* Page/Aux Addressing SFlag = 1 */
    WRITE_FIELD(HAL_CPYCMP_TFLAG, 1);                       /* Page/Aux Addressing TFlag = 1 */

    St_pfil_done = FALSE;                                   /* reset flag to 0 */

    WRITE_FIELD(HAL_CPYCMP_PFILSTRT, 1);                    /* start pattern fill */

    while (!St_pfil_done)
    {                               /* poll flag */
        ExitProcess();
    }

    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,1);                 /* turn off Copy clock */
}
