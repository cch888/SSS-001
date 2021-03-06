/****************************************************************************
*                (c) Copyright 2000 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME:     $Workfile: bdISR.h $
*
*   DESCRIPTION:   Header File for bdISR.c
*
*
*   $Revision: 6 $
*   $Date: 08/09/17 11:43p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __BDISR_H__
#define __BDISR_H__

/*******************************************************************
    TYPE AND STRUCTURE DEFINITIONS
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    DEFINES
*******************************************************************/

/*******************************************************************
    EXTERNAL MODULE VARIABLES
*******************************************************************/

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
#if (BD_READ == 1)
extern void BdDecoderIrqHandler(void);
#endif
#if (ENABLE_DISCARD_ICE == 1)
extern void ICEAreaDecoder(void);
#endif
#endif
