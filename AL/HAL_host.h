/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_host.h
*
* DESCRIPTION
*   This file supports the HAL Host funtions implemented in the Host HAL layer
*
* NOTES:
*
* $Revision: 1 $
* $Date: 08/01/02 2:22p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_HOST_H_
#define _HAL_HOST_H_


//*** External function prototypes
extern void HAL_HOST_StartAesBusEncryption(void);

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFBufferStart
    Description:    Set the address of the byte of the first sector in SDRAM
                    that contains a BEF bit.
    Inputs:         ULONG StartAddr
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFBufferStart(ULONG StartAddr)
{
    WRITE_REG(HAL_HSTREG_MAP->BEFCDSADL, StartAddr);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFBufferEnd
    Description:    Set the address of the byte of the last sector in SDRAM
                    that contains a BEF bit.
    Inputs:         ULONG EndAddr
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFBufferEnd(ULONG EndAddr)
{
    WRITE_REG(HAL_HSTREG_MAP->BEFCDEADL, EndAddr);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFBufferXferStart
    Description:    Specify the data transfer start address in SDRAM
    Inputs:         ULONG Addr
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFBufferXferStart(ULONG Addr)
{
    WRITE_REG(HAL_HSTREG_MAP->BEFCDCADL, Addr);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFBitOffset
    Description:    Specify the BEF bit's offset within the byte
    Inputs:         halHST_BEFBitOffset_t offset
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFBitOffset(halHST_BEFBitOffset_t offset)
{
    WRITE_FIELD(HAL_HST_BEFBITOFF, offset);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFSecOffset
    Description:    Specify the sector offset within the ECC Block
    Inputs:         BYTE offset
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFSecOffset(BYTE offset)
{
    WRITE_FIELD(HAL_HST_BEFSECOFF, offset);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFSecLength
    Description:    Specify the number of bytes from BEF bit to the next sector's
                    BEF bit within the same ECC block.
    Inputs:         halHST_BEFSecLen_t Length
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFSecLength(halHST_BEFSecLen_t Length)
{
    WRITE_REG(HAL_HSTREG_MAP->BEFSECLEN, Length);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFBlkLength
    Description:    Specify the number of bytes from the first BEF bit in a block 
                    to the first BEF bit in the next block.
    Inputs:         halHST_BEFBlkLen_t Length
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFBlkLength(halHST_BEFBlkLen_t Length)
{
    WRITE_FIELD(HAL_HST_BEFBLKLEN, Length);
}

/*-----------------------------------------------------------------------------------------------------
    Name:           HAL_HOST_AesSetBEFMaxSec
    Description:    Specify the maximum sector offset in an ECC block.
    Inputs:         halHST_BEFMaxSecOffset_t offset
    Outputs:        None
    Global Inputs:  None
    Global Outputs: None
    Returns:        void
-----------------------------------------------------------------------------------------------------*/
extern __inline void HAL_HOST_AesSetBEFMaxSec(halHST_BEFMaxSecOffset_t offset)
{
    WRITE_FIELD(HAL_HST_BEFMAXSEC, offset);
}

#endif /* _HAL_HOST_H_ */
