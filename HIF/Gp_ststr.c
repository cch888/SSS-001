/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC..
*                           All Right Reserved
*
* FILENAME:  gp_ststr.c
*
* DESCRIPTION:
*    Processes Set Streaming Command
*
*   $Revision: 8 $
*   $Date: 07/03/14 4:07p $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\hif\gp_ram.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_ststr.h"
#include ".\hif\sv_util.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrdb.h"

/*********************************************************************
 GLOBAL VARIABLES
*********************************************************************/
sStreamPara     StreamSetting = { 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                  0x00000000, 0x00000000 };
BYTE            StreamFlag = 0x00;
                /*
                 *  bit7        0 :Set Streaming Operation HOST Require
                 *              1 :Set Streaming Operation HOST NOT Require
                 *  bit2(RDD)   0 :Remaining Fields are Valid
                 *              1 :ReSET to Drive's default performance setting, Remaining Fields shall be ignored
                 *  bit1(Exact) 0 :SET Drive's config to match the parameter as best as possible
                 *              1 :SET Drive's config to support the requested parameter
                 *  bit0(RA)    0 :Allow Drive to independently set the Read/Write Speed
                 *              1 :Direct Drive to set performance setting for optimized setting for random change Read/Write
                 */


/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
static void     restoreStreamSetting    ( void );
static vBOOL        setStreamingSetting     ( USHORT );
static void     setStreamSpeed          ( BYTE );
static vBOOL        chkValidityLBA          ( void );

/*********************************************************************
 LOCAL DEFINITION
*********************************************************************/
/* Streaming Performance DB Length */
#define STR_DATA_LEN        sizeof( sStreamPara )
/* Performance Descriptor Length */
#define STR_HDR_LEN         0x04
#define STR_LBA_LEN         0x0C
#define STR_RD_LEN          0x14
#define STR_WR_LEN          0x1C

/* Streaming Setting Type(Read/Write) */
#define RD_STR_SPEED        bit0
#define WR_STR_SPEED        bit1

/* conversion of transfer rate(msec -> sec) */
#define MILI_CONVERT        1000
/*********************************************************************
*   Function :
*       SetStreamingCommand()
*
*   Description :
*       This is the command handler for the Set Streaming command
*
*********************************************************************/
void SetStreamingCommand( void )
{
    USHORT  paraListLength;                     /* Streaming data size in bytes */
    BYTE    tmpLoad;

    /*---------------------------------------------------------------
        Initial local variables
    ---------------------------------------------------------------*/
    paraListLength = 0;
    tmpLoad        = 0;

    /*---------------------------------------------------------------
        Get & Check Parameter length from HOST
    ---------------------------------------------------------------*/
    BHIGH( paraListLength ) = Iram_Cmd_block.bf[9];
    BLOW ( paraListLength ) = Iram_Cmd_block.bf[10];

    if ( paraListLength > STREAM_DATA_LEN )
    {
        BUILD_SENSE(0x05,0x24,0x00,0xfa);        /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    } else if ( ! paraListLength )
    {
        AutoCommandComplete();                         /* !! NO ERROR !! */
        return;
    }
    /*---------------------------------------------------------------
        Save Streaming Parameter into DRAM
    ---------------------------------------------------------------*/
    Iram_Linear_Xfer_addr = STRDATA_ADR;
    GetParameterDataFromHost(STRDATA_ADR, paraListLength);

    /*---------------------------------------------------------------
        Load Streaming Parameter from DRAM
    ---------------------------------------------------------------*/
    Iram_Buf_dram_addr = STRDATA_ADR;
    tmpLoad = ReadSDRAM();
    /*---------------------------------------------------------------
        Check Exact & Random Access Bits Setting
    ---------------------------------------------------------------*/
    if ( tmpLoad & STR_EXACT_BIT )
    {
        StreamFlag |= STR_EXACT_BIT;        /* Save bit setting of Streaming Operation */
        BUILD_SENSE(0x05,0x26,0x00,0xf8);    /* INVALID FIELD IN PARAMETER LIST */
        AutoCommandComplete();
        return;
    }
    else
    {
        StreamFlag &= (~ STR_EXACT_BIT);    /* Reset bit setting of Streaming Operation */
    }
    if ( tmpLoad & STR_RA_BIT )
    {
        StreamFlag |= STR_RA_BIT;           /* Save bit setting of Streaming Operation */
        BUILD_SENSE(0x05,0x25,0x00,0xf6);    /* LOGICAL UNIT NOT SUPPORTED */
        AutoCommandComplete();
        return;
    }
    else
    {
        StreamFlag &= (~ STR_RA_BIT);   /* Reset bit setting of Streaming Operation */
    }
    /*---------------------------------------------------------------
        Check Restore Logical Unit Default Bit Setting
            If this bit is ON, Remaining field value is ignored !!
    ---------------------------------------------------------------*/
    if ( tmpLoad & STR_RDD_BIT )
    {
        StreamFlag |= STR_RDD_BIT;      /* Save bit setting of Streaming Operation */
        restoreStreamSetting();
        AutoCommandComplete();
        return;
    }
    else
    {
        StreamFlag &= (~ STR_RDD_BIT);  /* Reset bit setting of Streaming Operation */
    }
    /*---------------------------------------------------------------
        Start & End LBA validity check
    ---------------------------------------------------------------*/
    if ( ! chkValidityLBA() )
    {
        BUILD_SENSE(0x05,0x24,0x00,0xef);        /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }
    /*---------------------------------------------------------------
        Set Streaming Parameter
    ---------------------------------------------------------------*/
    setStreamingSetting( paraListLength );

    AutoCommandComplete();
}

/*********************************************************************
*   Function :
*       restoreStreamSetting()
*
*   Description :
*       This function reset the Streaming Data Variable.
*
*   Parameters :
*       None
*
*   Return value:
*       None
*
*********************************************************************/
static void restoreStreamSetting( void )
{
    BYTE        *paraPtr;
    BYTE        len;

    /* Disable Streaming Operation */
    StreamFlag &= (~ STR_ENABLE_BIT);
    /* set restore address */
    paraPtr = (BYTE *)&StreamSetting;
    /* Streaming Data Base valiables 0 fill(exclude RDD bit setting) */
    len = STR_DATA_LEN;
    while ( len )
    {
        *paraPtr = 0x00;
        paraPtr++;
        len--;
    }
}

/*********************************************************************
*   Function :
*       setStreamingSetting()
*
*   Description :
*       This function save the Streaming Data Variable.
*
*   Parameters :
*       len      -  Size of the saving streaming data
*
*   Return value:
*       TRUE     -  request length is valid
*       FALSE    -  request length is invalid
*
*********************************************************************/
static vBOOL setStreamingSetting( USHORT len )
{
    BYTE        *paraPtr;
    BYTE        saveType;

    /* If request length is NOT multiple of 4, not save request parameter */
    if ( len % 0x04 )
    {
        BUILD_SENSE(0x05,0x1A,0x00,0xf5);        /* PARAMETER LIST LENGTH ERROR */
        return( TRUE );
    }

    /* check requested streaming setting */
    if ( len == STR_WR_LEN )
    {
        saveType = (RD_STR_SPEED | WR_STR_SPEED);
    }
    else if ( len == STR_RD_LEN )
    {
        saveType = RD_STR_SPEED;
    }
    else if ( len == STR_LBA_LEN )
    {
        saveType = 0x00;
    }
    else
    {
        BUILD_SENSE(0x05,0x1A,0x00,0xf6);        /* PARAMETER LIST LENGTH ERROR */
        return( FALSE );
    }

    /* set streaming data saving address */
    paraPtr = (BYTE *)&StreamSetting.StartLBA;

    /* set streaming data loading DRAM Address */
    Iram_Buf_dram_addr = (STRDATA_ADR + STR_HDR_LEN);

    /* Streaming Data write to variables from DRAM area */
    len -= STR_HDR_LEN;                 /* subtract 4Bytes(Byte0~3) */
    while ( len )
    {
        *paraPtr = ReadSDRAM();
        paraPtr++;
        len--;
    }

    StreamSetting.StartLBA = SWAPEND_ULONG(StreamSetting.StartLBA);
    StreamSetting.EndLBA   = SWAPEND_ULONG(StreamSetting.EndLBA);
    StreamSetting.RD_Size  = SWAPEND_ULONG(StreamSetting.RD_Size);
    StreamSetting.RD_Time  = SWAPEND_ULONG(StreamSetting.RD_Time);
    StreamSetting.WR_Size  = SWAPEND_ULONG(StreamSetting.WR_Size);
    StreamSetting.WR_Time  = SWAPEND_ULONG(StreamSetting.WR_Time);

    /* set requested streaming setting speed */
    setStreamSpeed( saveType );

    if ( StreamSetting.StartLBA != StreamSetting.EndLBA )
    {
        /* Enable Streaming Operation */
        StreamFlag |= STR_ENABLE_BIT;
    }
    else
    {
        /* Disable Streaming Operation */
        StreamFlag &= (~ STR_ENABLE_BIT);
    }

    return( TRUE );
}

/*---------------------------------------------------------
    Function :
        setDefultReadPerform()
---------------------------------------------------------*/
static void setStreamSpeed( BYTE type )
{
    USHORT          setRate;
    sStreamPara     *strPtr;

    strPtr = &StreamSetting;

    if (( type & RD_STR_SPEED ) && (strPtr->RD_Time != 0))
    {
        /* calcurate Read taransfer rate with ReadSize & ReadTime */
        setRate = (USHORT)((MILI_CONVERT * strPtr->RD_Size) / strPtr->RD_Time);     /* transfer rate = KB/sec */
        CheckSelectedSpeed(RD,setRate);
    }

    if (( type & WR_STR_SPEED ) && (strPtr->WR_Time != 0))
    {
        /* calcurate Read taransfer rate with ReadSize & ReadTime */
        setRate = (USHORT)((MILI_CONVERT * strPtr->WR_Size) / strPtr->WR_Time);     /* transfer rate = KB/sec */
        CheckSelectedSpeed(WR,setRate);
    }
}


/*********************************************************************
*   Function :
*       chkValidityLBA()
*
*   Description :
*       This function check the Start & End LBA relationship.
*       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*       !!!!! Don't care Start/End LBA & Max LBA relationship !!!!!
*       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*
*   Parameters :
*       None
*
*   Return value:
*       TRUE     -  Start LBA =< End LBA
*       FALSE    -  Start LBA >  End LBA
*
*********************************************************************/
static vBOOL chkValidityLBA( void )
{
    ULONG       startLBA, endLBA, tmpLBA;
    BYTE        cnt;
    BYTE        *LBAptr;

    Iram_Buf_dram_addr = (STRDATA_ADR + STR_HDR_LEN);

    /* load requested Start LBA from DRAM address */
    LBAptr = (BYTE *)&tmpLBA;
    for ( cnt = 0; cnt < 4; cnt ++ )
    {
        *LBAptr = ReadSDRAM();
        LBAptr++;
    }
    startLBA = SWAPEND_ULONG(tmpLBA);

    /* load requested End LBA from DRAM address */
    LBAptr = (BYTE *)&tmpLBA;
    for ( cnt = 0; cnt < 4; cnt ++ )
    {
        *LBAptr = ReadSDRAM();
        LBAptr++;
    }
    endLBA = SWAPEND_ULONG(tmpLBA);

    /* compare Start LBA with End LBA */
    if ( startLBA > endLBA )
    {
        return( FALSE );
    }
    else
    {
        return( TRUE );
    }
}
