/*********************************************************************
*         (c) Copyright 2001, 2002 2003, 2004 SUNEXT DESIGN, INC.
*                      All Rights Reserved
*
*
*
* FILENAME:   dvRPC.h
*
*
* DESCRIPTION:
*   Header file for file dvRpc.c Region Playback Control.
*
*   $Revision: 2 $
*   $Date: 06/04/03 5:37a $
*
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*    1.00   Jly-18-01   Wata    Created
**************************** SUNEXT CONFIDENTIAL *******************************/
/* FILE_VERSION : 1.00 2001-07-18 */
#ifndef __DVRPC_H__
#define __DVRPC_H__

extern                  void RpcSetRegion(BYTE NewRegion);
extern RPC_NON_VOLATILE_VARS RpcGetRegion(void);

#endif
