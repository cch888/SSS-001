/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_mac.h
*
* DESCRIPTION
* This file contains general purpose macros
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __GP_MAC_H__
#define __GP_MAC_H__
extern ULONG PacketCommandToLBALEN4(BYTE a);
extern ULONG PacketCommandToLBALEN3(BYTE a);
extern ULONG PacketCommandToLBALEN2(BYTE a);
extern TimeS Method2LBAToMSF(long LBA2);
extern void BUILD_SENSE(BYTE snsky,BYTE snscode,BYTE snsqual,BYTE debugbyte);
extern void ZeroFillBlocks(USHORT SBLK,USHORT EBLK);

/**********************************************************************
* Macro Name: MINUS_LBA_HEADER_1_FRAME
*
* MINUS_LBA_HEADER_1_FRAME is a macro used to minus frames
* from the specified header block.
**********************************************************************/
#define MINUS_LBA_HEADER_1_FRAME(LBA,COUNT) (LBA -= COUNT)

/**********************************************************************
* Macro Name: ADD_LBA_HEADER_1_FRAME
*
* ADD_LBA_HEADER_1_FRAME is a macro used to add frames
* to the specified header block.
**********************************************************************/
#define ADD_LBA_HEADER_1_FRAME(LBA,COUNT)   (LBA += COUNT)

/**********************************************************************
* Macro Name: LBA_SUB
*
* LBA_SUB is a macro used to subtract two specified LBA vlaues.
**********************************************************************/
#define LBA_SUB(LBA1,LBA2,RES,CRY){\
    RES = LBA1 - LBA2;\
    CRY = ((LBA1<LBA2)? 1:0);\
}

#define PAGE_AUX_START_ADDR(PAGENO) (ULONG)(AUX_START_ADDR + DRAM_AUX_SIZE * (PAGENO & 0x1FFF) - 0x800)

#define MINIMUM(PARAM1, PARAM2) (((PARAM1) < (PARAM2)) ? (PARAM1) : (PARAM2)) //  Compares two parameters, return minimum.
#define MAXIMUM(PARAM1, PARAM2) (((PARAM1) > (PARAM2)) ? (PARAM1) : (PARAM2)) //  Compares two parameters, return maximum.

// The following macros work only if Port B bit 3 is configured as an output port.
//  The macros can be used as a trace help for debugging code.
#define ScopeSignalOn()      SetBitsSCReg_1B(rwPBDATA,bit3)    // Set Port B bit 0 =  1
#define ScopeSignalOff()     ClrBitsSCReg_1B(rwPBDATA,bit3)    // Set Port B bit 0 =  1
#define ScopeSignalToggle()  WriteSCReg_1B(rwPBDATA, (ReadSCReg_1B(rwPBDATA) ^ bit3))     // Toggle Port B bit 3

#define SWAP_WORD(D)        (WORD)( ((WORD)(D)>>8) + (((WORD)(D)&0xff)<<8) )
#define SWAPEND_WORD(D)     SWAP_WORD(D)
#define SWAPEND_UWORD(D)    SWAP_WORD(D)

#define SWAPEND_ULONG(D)    (ULONG)((((ULONG)(D)&0xff)<<24) +  (((ULONG)(D)&0xff000000)>>24) + (((ULONG)(D)&0xFF00)<<8)  + (((ULONG)(D)&0xFF0000)>>8) )
#define SWAPEND_DWORD(D)    SWAPEND_ULONG(D)


#endif /* __GP_MAC_H__ */
