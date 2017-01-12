/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   at_util.h
*
* DESCRIPTION
*   This file contains ATAPI or AT interface routine
*
*   $Revision: 8 $
*   $Date: 11/01/18 10:30a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __AT_UTIL_H__
#define __AT_UTIL_H__

extern const MODES_PARA_T mode1_para[];
extern const MODES_PARA_T mode2_para[];
extern const MODES_PARA_T mode3_para[];
extern const MODES_PARA_T mode4_para[];
extern const MODES_PARA_T mode5_para[];

extern void CheckDataTransferLimit( USHORT);
extern void GetParameterDataFromHost(ULONG, ULONG);
extern void SendParameterDataToHost(ULONG, ULONG, FLAG);
extern void ATAClearBusy(void);
extern void InitATATaskFileRegisters(void);
extern void SendStatus(void);
extern void TransferToHost(void);
extern void TransferBlocksToHost(void);
extern void SendHostBlocks(void);
extern vBOOL Residue_minus_xfr_byte(void);
extern void Send_pardata_piece_by_piece(void);
extern void AbortHostTransfer(void);
extern void ATASetDSC(void);
extern BYTE CalcDataRequestBlocks(void);
extern void ATAGenerateHIRQ(void);
extern void SetupHostTransferRegisters(void);
extern vBOOL GetFlashUpdateFromHost( USHORT  T_page_count );        // for receive with page mode

#define CHECK_BURST_DATA_TRANSFER(DATA_LENGTH) (Iram_Xfr_allow < DATA_LENGTH) ? Ata_sub_burst = TRUE : Ata_sub_burst = FALSE;
                                        // Compare # of bytes allowed per host data xfer & actual data length to see
                                        // if each block of data can be xfered in a burst or not.
                                        // Params:   data_len

extern BYTE Special_Disc;	/* record ABEX Disc ID */

/* LED timer control for Burn-in */
extern BOOL Burn_in_LED;
extern vBYTE Burn_in_LED_Blink_count;
extern vBYTE Burn_in_error_ID;
extern vBYTE Burn_in_Msg_OFF_timer;

#endif /* __AT_UTIL_H__ */
