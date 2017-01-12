/****************************************************************************
*                (c) Copyright 2001-2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   firmw_id.h
*
* DESCRIPTION
* This file contains the Firmware ID string for Identify and Inquiry command
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __FIRMW_ID_H__
#define __FIRMW_ID_H__
#include ".\oem\oem_info.h"


    #if (SET_SLOT_IN == 1)
#define FIRMW_ID_0 'W'
    #else
#define FIRMW_ID_0 'V'
    #endif
	#if (Customer_Dune_HD == 1)
#define FIRMW_ID_1 '7'
	#elif (Customer_A1== 1)
#define FIRMW_ID_1 '1' 
	#elif (Customer_A2== 1)
#define FIRMW_ID_1 '2' 
	#elif (Customer_A4== 1)
#define FIRMW_ID_1 '3'
	#elif (Customer_China == 1)  
#define FIRMW_ID_1 '6'
    #elif (Customer_U3 == 1)
#define FIRMW_ID_1 '.'	
	#else
#define FIRMW_ID_1 '0'
    #endif
#define FIRMW_ID_2 '0'
#define FIRMW_ID_3 'S'
   


/*
 * 1. Identify Device Table - Firmware Revision ( byte swapped word 23 ~ 26)
 */

#define FIRMW_ID_REVISION   FIRMW_ID_1, FIRMW_ID_0, FIRMW_ID_3, FIRMW_ID_2, ' ', ' ', ' ', ' '


/*
 *  2. Inquiry Table - Product Revision (byte 32 ~ 35)
 */

#define FIRMW_INQ_REVISION   FIRMW_ID_0, FIRMW_ID_1, FIRMW_ID_2, FIRMW_ID_3

#endif
