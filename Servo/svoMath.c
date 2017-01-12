/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
* FILENAME:   svoMath.c
*
* DESCRIPTION
*   This file supports the math funtions
*
*   $Revision: 9 $
*   $Date: 07/05/10 8:12a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\string.h"
#include ".\common\commath.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svomath.h"
#include ".\common\chip.h"
#include ".\common\hwdef.h"
#include ".\servo\svoVar.h"

long svoMSF2LBA(SVOTimeS *msf)   /* MSF time -> LBA      */
{
    long lba;

    lba = svoMSF2ADR(msf);
    lba = lba - 150L;
    return(lba);
}

SVOTimeS svoLBA2MSF(long lba)   /* LBA      -> MSF time */
{
    long addr;

    addr = lba + 150L;
    return(svoADR2MSF(addr));
}

long svoMSF2ADR(SVOTimeS *msf)   /* MSF time -> ADDRESS  */
{
    long addr;

    addr  = (((long)msf->Time.Minute) * 4500L);
    addr += (((long)msf->Time.Second) * 75L);
    addr +=  ((long)msf->Time.Frame);

    if(msf->Direction == MINUS){
        addr -= svoLIBoundary;
    }

    return addr;
}


SVOTimeS svoADR2MSF(long lba)   /* ADDRESS -> MSF time */
{
    SVOTimeS    msf;
    long        min,sec,frm,addr;

    addr = lba;
    if(addr < 0L){
        addr += svoLIBoundary;
        msf.Direction = MINUS;
    }else{
        msf.Direction = PLUS;
    }

    min     = addr / 4500L;
    addr   -= (min * 4500L);
    sec     = addr / 75L;
    addr   -= (sec * 75L);
    frm     = addr;

    msf.Time.Minute = (BYTE)min;
    msf.Time.Second = (BYTE)sec;
    msf.Time.Frame  = (BYTE)frm;

    return(msf);
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
