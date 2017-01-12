/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   ERROR.H
*
* DESCRIPTION
*   This file contains error definitions
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _ERROR_H_
#define _ERROR_H_

typedef unsigned char ErrorGroupServoT;
#define eErrorFocus                     0X80
#define eErrorRadialLost                0X40
#define eErrorRadialSkating             0X20
#define eErrorSledge                    0X10
#define eErrorPLLNoLocked               0X08
#define eErrorDiscSpeed                 0X04
#define eErrorOffTrack                  0X02
#define eErrorRadialInitialization      0X01
#define eNoErrorServo                   0x00


typedef unsigned char ErrorGroupPlayerT;
#define eErrorAccess                    0X80
#define eErrorLink                      0X40
#define eErrorWindows                   0X20
#define eNoErrorPlayer                  0x00


typedef unsigned char ErrorGroupDiscT;
#define eErrorSubcodeTimeOut            0x80
#define eErrorATIPTimeOut               0x40
#define eErrorSync                      0x20
#define eErrorUnknownDisc               0x10
#define eNoErrorDisc                    0x00


typedef unsigned char ErrorGroupCommandT;
#define eErrorCommandTimeout            0X80
#define eErrorCommandNotAccepted        0X40
#define eErrorRangeCheck                0X20
#define eNoErrorCommand                 0x00


typedef unsigned char ErrorGroupTrayT;
#define eErrorTrayIn                    0X80
#define eErrorTrayOut                   0X40
#define eNoErrorTray                    0x00


typedef unsigned char ErrorGroupDiscInfoT;
#define eErrorNoDiscPresent             0X80
#define eErrorPMA                       0X40
#define eErrorTOC                       0X20
#define eErrorDiscInfo                  0X10
#define eErrorHFAfterHRTR               0X08
#define eErrorWritingFailure            0X04
#define eNoErrorDiscInfo                0x00


typedef unsigned char ErrorGroupReadT;
#define eErrorCheckReservedHFOnAddress  0X80
#define eErrorModeOfLastTrack           0X40
#define eErrorNoEFMAtSearchAddress      0X20
#define eErrorNoDataTrack               0X08
#define eErrorReadInLeadin              0X04
#define eErrorReadInLeadout             0X02
#define eErrorHFFoundOnBlankArea        0X01
#define eNoErrorRead                    0x00


typedef unsigned char ErrorGroupWriteT;
#define eErrorDataWrite                 0X80
#define eErrorFixUpEmptySession         0X40
#define eErrorNotAllTracksWritten       0X20
#define eErrorDiscFull                  0X10
#define eErrorRecordOnNonCDR            0X08
#define eErrorDummyBlocksAdded          0X04
#define eErrorNotAllBlocksWritten       0X02
#define eErrorStartLink                 0x01
#define eNoErrorWrite                   0x00

typedef unsigned char ErrorGroupPacketT;
#define eErrorPacketNoPregap            0X40
#define eErrorPacketNotAllBlocksWritten 0X10
#define eErrorPacketTrackToShort        0X08
#define eErrorNoPacketTrack             0X04
#define eErrorPacketNoBlank             0X02
#define eNoErrorPacket                  0X00

typedef unsigned char ErrorGroupOPCT;

#define eErrorRereadOpc	                0x10
#define eErrorSeek                      0x40
#define eErrorOpcFocusDrop              0x80
#define eErrorIPModeDVDRAM              0x50
#define eErrorPSNPassed                 0x30
#define eErrorCycleTimeOut              0x20
#define eErrorPhaseSeekRetry            0x10
#define eErrorPhaseSetSpeed             0x0F
#define eErrorPhaseSeek                 0x0E
#define eErrorPhaseTimeOut              0x08
#define eErrorRWJitter                  0x06
#define eErrorRedoOpc                   0x05
#define eErrorCountAreaFull             0x04
#define eErrorCountArea                 0x03
#define eErrorRecordPowerLimitWarning   0x02
#define eErrorRecordPowerLowWarning     0x01
#define eNoErrorOPC					    0X00

#endif /* _ERROR_H_ */

