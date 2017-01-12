/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved

* FILENAME: svoMath.h
*
* DESCRIPTION
*   This file supports the math funtions
*
* NOTES:
*
*
* CHANGES:
* REF VERSION DATE      WHO     DETAIL
* --- ------- ----      ---     ------
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _svoMATH_H_
#define _svoMATH_H_

#define EQUAL   0
#define SMALLER 1
#define GREATER 2

/* Prototypes */
long        svoMSF2LBA(SVOTimeS *msf);
SVOTimeS    svoLBA2MSF(long lba);
long        svoMSF2ADR(SVOTimeS *msf);
SVOTimeS    svoADR2MSF(long lba);

/* External Functions  */
extern long     svoMSF2LBA(SVOTimeS *msf);
extern SVOTimeS svoLBA2MSF(long lba);
extern long     svoMSF2ADR(SVOTimeS *msf);
extern SVOTimeS svoADR2MSF(long lba);
#endif /* _svoMATH_H_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
