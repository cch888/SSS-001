/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gnrl_mac.h
*
* DESCRIPTION
* This file contains general purpose macros for the
* implementations of assembly like commands in "C".
*
* NOTES:
*
************************ SUNEXT CONFIDENTIAL *******************************/

#ifndef __GNRL_MAC_H__
#define __GNRL_MAC_H__


/* Rotate right */
#define RR(X)   (BYTE)(((X) & 1) ? (0x80 | ((BYTE)(X) >> 1)) : ((BYTE)(X) >> 1))

/* Rotate left */
#define RL(X)   (BYTE)(((X) & 0x80) ? (0x01 | ((BYTE)(X) << 1)) : ((BYTE)(X) << 1))

/* swap hi 4 bits with low 4 bits */
#define SWAP(X) (BYTE)(((BYTE)(X) >> 4) | ((X) << 4))


#endif /* __GNRL_MAC_H__ */
