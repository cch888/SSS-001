/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   gp_mchst.c
*
* DESCRIPTION
*   This file contains Mechanism Status command handler
*
* $Revision: 42 $
* $Date: 09/10/30 10:47a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\common\sdram.h"
#include ".\hif\gp_ram.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_key.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_rdsub.h"
#include ".\hif\gp_mchst.h"
#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

void    TransferNotificationData(void);
BYTE    getPowerStatus(void);
BYTE    GetPowerEvent(BYTE);
BYTE    getMediaStatus(void);
BYTE    GetMediaEvent(BYTE);

BYTE    GetOperationalChangeStatus(void);
BYTE    GetOperationalChangeEvent(void);

extern BYTE gPowerEvent;        /* power event */
extern BYTE gPowerStatus;       /* current power status */
extern BYTE gMediaEvent;        /* media event */
extern BYTE gMediaStatus;       /* current media status */
extern ULONG dvdGetCurrentPSN(BYTE * DVDDecoder_ID);

/*********************************************************************
* MechanismStatusCommand
*
* Entry point for that the device return the mechanism status data.
* (Based on MMC5r04, most fields of return data are obsoleted)
* Returns:  void
* Params:   none
*********************************************************************/
void MechanismStatusCommand()
{
    USHORT      AllocationLength;
    
    /* POWER ON, RESET, OR BUS DEVICE RESET OCCURED */
    if(St_unit_atten == TRUE)
    {
        BUILD_SENSE(0x06,0x29,0x00,0x06);
        St_unit_atten = FALSE;
        st_disc_change = FALSE;
        AutoCommandComplete();
        return;
    } 
    
    /* check if allocation length equal zero */
    BHIGH(AllocationLength) = Iram_Cmd_block.bf[8];  /* get allocation length */
    BLOW(AllocationLength)  = Iram_Cmd_block.bf[9];

    if(AllocationLength <= 1){                 /* take care odd BYTE */
        AutoCommandComplete();                 /* zero length, command complete */
        return;
    }

    AllocationLength &= (~ 0x01);               /* even value only */

    /* BYTE 0 */
    Iram_Linear_Xfer_addr = Iram_Buf_dram_addr = BUFFER_SCRATCH;
    WriteSDRAM(0);

    /* BYTE 1 */
    if ((St_door_open == TRUE)||(Iram_Svr_player_mode == EXECUTING_TRAY_OUT_MODE))
        WriteSDRAM(0x10);
    else
        WriteSDRAM(0);

    /* BYTE 2-7 */
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);

    Iram_shATB  = MINIMUM( 8, AllocationLength);

    /* check if transfer data in burst mode */
    CheckDataTransferLimit(Iram_shATB);

    Knl_send_compl = TRUE;

    Iram_Buf_hxfer_blk = 1;

    if(Ata_sub_burst == TRUE)
    {
        Send_pardata_piece_by_piece();
    }

    St_send_pardata = TRUE;
    Knl_Atapi_Avail = TRUE;

    TransferToHost();

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        Knl_Atapi_Avail = FALSE;
        BUILD_SENSE(0x00,0x00,0x00,0x19);    // NO ADDITIONAL SENSE INFO
        AutoCommandComplete();
    }
#endif
}


/* ==========================================================================
 *
 *    <power management status data>        <media event status data>
 *
 * ------------------------------------  ------------------------------------
 * BYTE| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0   BYTE| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
 * ------------------------------------  ------------------------------------
 *   0 |    event data length high         0 |    event data length high
 * ------------------------------------  ------------------------------------
 *   1 |    event data length low          1 |    event data length low
 * ------------------------------------  ------------------------------------
 *   2 |NEA|    reserved   |   class       2 |NEA|    reserved   |   class
 * ------------------------------------  ------------------------------------
 *   3 |    supported event class          3 |    supported event class
 * ------------------------------------  ------------------------------------
 *   4 |    reserved   |  power event      4 |    reserved   |  media event
 * ------------------------------------  ------------------------------------
 *   5 |          power status             5 |          media status
 * ------------------------------------  ------------------------------------
 *   6 |            reserved               6 |     start slot(reserved)
 * ------------------------------------  ------------------------------------
 *   7 |            reserved               7 |      end slot(reserved)
 * ------------------------------------  ------------------------------------
 *
 * ==========================================================================
 */
BYTE const  GESN_nea_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x02,       /* 1: data length low */
            0x80,       /* 2: NEA */
            0x5E,       /* 3: supported class */
        };

BYTE const  GESN_power_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x02,       /* 2: notificatin class = power */
            0x5E,       /* 3: supported class */

            0x00,       /* 4: power event = no change */
            0x00,       /* 5: power status = standby */
            0x00,       /* 6: reserved */
            0x00        /* 7: reserved */
        };

BYTE const  GESN_external_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x03,       /* 2: notificatin class = power */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: external request event = no change */
            0x00,       /* 5: external request status = Available */
            0x00,       /* 6: external request = No request */
            0x00        /* 7: external request = No request */
        };

BYTE const  GESN_media_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x04,       /* 2: notificatin class = media */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: media event = no event */
            0x00,       /* 5: media status */
            0x00,       /* 6: reserved */
            0x00        /* 7: reserved */
        };

BYTE const  GESN_DeviceBusy_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x06,       /* 2: notificatin class = device busy */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: device busy event = no event */
            0x00,       /* 5: device busy status */
            0x00,       /* 6: reserved */
            0x00        /* 7: reserved */
        };

BYTE const  GESN_OpeChgReq_tbl[] =
        {
            0x00,       /* 0: data length high */
            0x06,       /* 1: data length low */
            0x01,       /* 2: notificatin class = operational change request/notification */
            0x5E,       /* 3: supported class */
            0x00,       /* 4: operational event = no event */
            0x00,       /* 5: operational status */
            0x00,       /* 6: operational request/report */
            0x00        /* 7: operational request/report */
        };

/*********************************************************************
 *  [4Ah] Get Event Status Notification
 *********************************************************************
 */
void    GetEventStatusNotificationCommand(void)
{
    /*
     *  check immediate bit
     *  only suport Polling mode(IMMED = 1)
     */
    if( !(Iram_Cmd_block.bf[1] & 0x01) )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x67);  /* invalid field in command packet */
        AutoCommandComplete();
    }

    /*
     *  check allocation length
     */
    else if ( ((Iram_Cmd_block.bf[8] & 0xfe) | Iram_Cmd_block.bf[7]) == 0 )
    {
        BUILD_SENSE(0x00,0x00,0x00,0x0A);  /* no sense */
        AutoCommandComplete();
    }

    /*
     *  transfer Notification Class Data from DRAM buffer to Host
     */
    else
    {
        TransferNotificationData();
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
        if (s2b_atapi_cmd_received == TRUE)
        {
            Knl_Atapi_Avail = FALSE;
            BUILD_SENSE(0x00,0x00,0x00,0x17);    // NO ADDITIONAL SENSE INFO
            AutoCommandComplete();
        }
#endif
    }
}



/* --------------------------------------------------------------------------
 *  transfer Notification Class Data from DRAM buffer to Host
 *
 *  !!! Only one class of event per this command(GESN) shall be reported.!!!
 *  !!! There is A Priority of event reporting by event class number.!!!
 * --------------------------------------------------------------------------
 */
void    TransferNotificationData(void)
{

    USHORT      alloc_len;      /* allocation length */
    BYTE        MediaNow;       /* Now, what state of media? */
    BYTE        OpeChgEvent;    /* Operational Change event */
    BYTE        TransferClass;

    /* get allocation length */
    alloc_len = (USHORT)Iram_Cmd_block.bf[7] * 0x100 + Iram_Cmd_block.bf[8];

    /*****************************************/
    /***** Event Change Check & Data Set *****/
    /*****************************************/
    TransferClass = 0x00;

    /* ---------------------------------------------
     *  Operational Change/Notification Class Events
     */
    if ( Iram_Cmd_block.bf[4] & 0x02 )
    {
        OpeChgEvent  = GetOperationalChangeEvent();                /* get operational Event */

        /*
         *  set Operational Change/Notification into SDRAM buffer
         */
        Iram_Buf_dram_addr = GESN_OPECHGREQ_ADR + 4;    /* add offset for operational status */
        WriteSDRAM( OpeChgEvent );                         /* write operational event */

        /*
        Operational Status is always 0 - Logical Unit is ready
        PersistenPrevented bit is set/cleared at Prevent/Allow Medium command
        So we don't need to update byte 1
        */

        Iram_Buf_dram_addr++;
        if ( OpeChgEvent != 0x00 )
        {
            if ( OperationalChangeTrayEvent == GSEN_OPE_TRAY_IN )
            {
                WriteSDRAM( 0x00 );                        /* write request/report */
                WriteSDRAM( 0x02 );
            }
            else
            {
                WriteSDRAM( 0x00 );                        /* write request/report */
                WriteSDRAM( 0x01 );
            }
            OperationalChangeTrayEvent = GSEN_OPE_NOTHING;
            TransferClass = 0x02;
        }
        else
        {
            if ( (St_become_ready == TRUE) || (St_door_open == TRUE) )
            {
                /* Nothing to do */
            }
            else
            {
                WriteSDRAM( 0x00 );                        /* write request/report */
                WriteSDRAM( 0x00 );
            }
        }
    }

    /* ---------------------------------------------
     *  Power Management Class Events
     */
    if ( (Iram_Cmd_block.bf[4] & 0x04) && (TransferClass == 0x00) )
    {
        #if (EN_POWER_CONTROL == 0)
        BYTE        PowerNow;       /* current power status */

        PowerNow = getPowerStatus();                    /* get current Power status */
        gPowerEvent = GetPowerEvent(PowerNow);      /* get Power Event */
        gPowerStatus = PowerNow;                    /* update power status */
        #endif

        /*
         *  set Power Event & Status into DRAM buffer
         */
        Iram_Buf_dram_addr = GESN_POWER_ADR + 4;        /* add offset for event status */
        WriteSDRAM( gPowerEvent );                     /* write power event */
        WriteSDRAM( gPowerStatus );                    /* write power status */

        if ( gPowerEvent != 0x00 )
        {
            TransferClass = 0x04;
        }
        gPowerEvent = 0;
    }

    /* ---------------------------------------------
     *  Media Status Class Events
     */
    if ( (Iram_Cmd_block.bf[4] & 0x10) && (TransferClass == 0x00) )
    {
        MediaNow = getMediaStatus();                /* get current Media status */
        gMediaEvent = GetMediaEvent(MediaNow);  /* get Media Event */
        gMediaStatus = MediaNow;                /* update media status */

        /*
         *  set Meida Event & Status into DRAM buffer
         */
        Iram_Buf_dram_addr = GESN_MEDIA_ADR + 4;    /* add offset for event status */
        WriteSDRAM( gMediaEvent );                 /* write media event */
        WriteSDRAM( gMediaStatus );                /* write media status */

        if ( gMediaEvent != 0x00 )
        {
            TransferClass = 0x10;
        }
    }

    /* ---------------------------------------------
     *  Device Busy Status Class Events
     */
    if ( (Iram_Cmd_block.bf[4] & 0x40) && (TransferClass == 0x00) )
    {
        USHORT      BusyTimeNow;

        BusyTimeNow = 0;

        if ( BusyTimeNow != 0 )
        {
            DeviceBusyEvent  = 0x00;
            DeviceBusyStatus = 0x03;
        }
        else if (DeviceBusyTime != 0)
        {
            DeviceBusyEvent  = 0x01;
            DeviceBusyStatus = 0x00;
        }
        else
        {
            DeviceBusyEvent  = 0x00;
            DeviceBusyStatus = 0x00;
        }

        DeviceBusyTime = BusyTimeNow;

        Iram_Buf_dram_addr = GESN_DEVICEBUSY_ADR + 4;
        WriteSDRAM( DeviceBusyEvent );
        WriteSDRAM( DeviceBusyStatus );
        WriteSDRAM( BHIGH(DeviceBusyTime) );
        WriteSDRAM( BLOW (DeviceBusyTime) );

        if ( DeviceBusyEvent != 0x00 )
        {
            TransferClass = 0x40;
        }
    }

    /*****************************/
    /***** Transfer Data Set *****/
    /*****************************/
    /* ---------------------------------------------
     *  Operational Change/Notification Class Events
     */
    if ( (Iram_Cmd_block.bf[4] & 0x02) && ((TransferClass == 0x00) || (TransferClass == 0x02)) )
    {
        /*
         *  trasfer only Event Header with NEA to Host
         */
        Iram_Linear_Xfer_addr = GESN_OPECHGREQ_ADR;     /* SDRAM address */
        Iram_shATB = GESN_OPECHGREQ_LEN;                /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  Power Management Class Events
     */
    else if ( (Iram_Cmd_block.bf[4] & 0x04) && ((TransferClass == 0x00) || (TransferClass == 0x04)) )
    {
        /*
         *  trasfer Power Management Status data to Host
         */
        Iram_Linear_Xfer_addr = GESN_POWER_ADR;         /* SDRAM address */
        Iram_shATB = GESN_POWER_LEN;                    /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  External Request Class Events
     */
    else if ( (Iram_Cmd_block.bf[4] & 0x08) && (TransferClass == 0x00) )
    {
        /*
         *  trasfer Media Event Class data to Host
         */
        Iram_Linear_Xfer_addr = GESN_EXTERNAL_ADR;      /* SDRAM address */
        Iram_shATB = GESN_EXTERNAL_LEN;                 /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  Media Status Class Events
     */
    else if ( (Iram_Cmd_block.bf[4] & 0x10) && ((TransferClass == 0x00) || (TransferClass == 0x10)) )
    {
        /*
         *  trasfer Media Event Class data to Host
         */
        Iram_Linear_Xfer_addr = GESN_MEDIA_ADR;         /* SDRAM address */
        Iram_shATB = GESN_MEDIA_LEN;                    /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  Device Busy Status Class Events
     */
    else if ( (Iram_Cmd_block.bf[4] & 0x40) && ((TransferClass == 0x00) || (TransferClass == 0x40)) )
    {
        /*
         *  trasfer Media Event Class data to Host
         */
        Iram_Linear_Xfer_addr = GESN_DEVICEBUSY_ADR;    /* SDRAM address */
        Iram_shATB = GESN_DEVICEBUSY_LEN;               /* BYTE lenngth */
    }

    /* ---------------------------------------------
     *  NEA (No Class Events)
     */
    else
    {
        /*
         *  transfer only Event Header with NEA to Host
         */
        Iram_Linear_Xfer_addr = GESN_NEA_ADR;           /* SDRAM address */
        Iram_shATB = GESN_NEA_LEN;                      /* BYTE lenngth */
        if( Iram_shATB < alloc_len ){
            ;   /* Nothing to do */
        }else{
            Iram_shATB = alloc_len;
        }
    }

    Iram_shATB = MINIMUM( Iram_shATB, alloc_len);  // Number of bytes to xfer. Compare, return minimum.
    CheckDataTransferLimit( Iram_shATB );       /* check if transfer data in burst mode */

    Knl_send_compl = TRUE;

    Iram_Buf_hxfer_blk = 1;
                                        /* setup H/W xfr */
    if ( Ata_sub_burst == TRUE ) {
        Send_pardata_piece_by_piece();
    }
    St_send_pardata = TRUE;
                                        /* execute xfr */
    Knl_Atapi_Avail = TRUE;
    TransferToHost();
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        Knl_Atapi_Avail = FALSE;
        BUILD_SENSE(0x00,0x00,0x00,0x16);    // NO ADDITIONAL SENSE INFO
        AutoCommandComplete();
    }
#endif
}

/* --------------------------------------------------------------------------
 *  initialize Notification Class Data in DRAM buffer
 * --------------------------------------------------------------------------
 */
void    initNotificationData(void)
{
    BYTE    Index;
    BYTE    *TableP;


    /* --------------------------------------------------------
     *  intialize No Event Header
     */

    TableP = (BYTE *) GESN_nea_tbl;        /* set top address */
    Iram_Buf_dram_addr = GESN_NEA_ADR;

    for( Index = 0; Index < GESN_NEA_LEN; Index++ )
    {
        WriteSDRAM(*TableP);
        TableP++;                          /* increment address */
    }

    /* --------------------------------------------------------
     *  intialize Power Management Event Class
     */

    TableP = (BYTE *) GESN_power_tbl;      /* set top address */
    Iram_Buf_dram_addr = GESN_POWER_ADR;

    for( Index = 0; Index < GESN_POWER_LEN; Index++ )
    {
        WriteSDRAM(*TableP);
        TableP++;                              /* increment address */
    }

    gPowerEvent = 1;
    gPowerStatus = 1; // power status = active
    Iram_Buf_dram_addr = GESN_POWER_ADR + 5;    /* add offset for power status */
    WriteSDRAM( gPowerStatus );                /* write power status */

    /* --------------------------------------------------------
     *  intialize External Request Event Class
     */

    TableP = (BYTE *) GESN_external_tbl;       /* set top address */
    Iram_Buf_dram_addr = GESN_EXTERNAL_ADR;

    for( Index = 0; Index < GESN_EXTERNAL_LEN; Index++ )
    {
        WriteSDRAM(*TableP);
        TableP++;                              /* increment address */
    }

    /* --------------------------------------------------------
     *  intialize Media Event Class
     */

    TableP = (BYTE *) GESN_media_tbl;          /* set top address */
    Iram_Buf_dram_addr = GESN_MEDIA_ADR;

    for( Index = 0; Index < GESN_MEDIA_LEN; Index++ )
    {
        WriteSDRAM(*TableP);
        TableP++;                              /* increment address */
    }

    gMediaEvent = 0x00;                     /* no event */
    gMediaStatus = getMediaStatus();        /* current media status */
    Iram_Buf_dram_addr = GESN_MEDIA_ADR + 5;    /* add offset for media status */
    WriteSDRAM( gMediaStatus );                /* write media status */

    /* ---------------------------------------------------------
     *  initialize Device Busy Status Class
     */
    TableP = (BYTE *) GESN_DeviceBusy_tbl;         /* set top address */
    Iram_Buf_dram_addr = GESN_DEVICEBUSY_ADR;

    for( Index = 0; Index < GESN_DEVICEBUSY_LEN; Index++ )
    {
        WriteSDRAM(*TableP);
        TableP++;                          /* increment address */
    }

    DeviceBusyEvent = 0x00;                 /* no event */
    DeviceBusyStatus = 0x00;                /* current device status */
    Iram_Buf_dram_addr = GESN_DEVICEBUSY_ADR + 5;/* add offset for media status */
    WriteSDRAM( DeviceBusyStatus );            /* write device status */

    /* ---------------------------------------------------------
     *  initialize Operational Change Request/notification Class
     */
    TableP = (BYTE *) GESN_OpeChgReq_tbl;          /* set top address */
    Iram_Buf_dram_addr = GESN_OPECHGREQ_ADR;

    for ( Index = 0; Index < GESN_OPECHGREQ_LEN; Index++ )
    {
        WriteSDRAM(*TableP);
        TableP++;                          /* increment address */
    }
}


/* --------------------------------------------------------------------------
 *  get current Power status
 * --------------------------------------------------------------------------
 *  IN:     none
 *  OUT:    ps - current power status(01:active,02:idle,03:standby)
 * --------------------------------------------------------------------------
 */
#if (EN_POWER_CONTROL == 0)
BYTE    getPowerStatus(void)
{
    BYTE    PowerStatus;         /* power status */

    //Get event status notification fail of Command set
    if((Iram_Svr_player_mode == PLAY_TRACK_MODE)||(Iram_Svr_player_mode == READING_MODE)
    	||(Iram_Svr_player_mode == EXECUTING_SEEK_MODE))
    {
        PowerStatus = 0x01;      /* in active state */
    }
    else if(ataPowerMode == IDLE_MODE)
    {
        PowerStatus = 0x02;      /* in idle state */
    }
    else
    {
        PowerStatus = 0x03;      /* in standby state */
    }

    return( PowerStatus );
}

/* --------------------------------------------------------------------------
 *  get Power event
 * --------------------------------------------------------------------------
 *  IN:     ps - current power status
 *  OUT:    pe - power event(00:no changed, 01:successfully changed)
 * --------------------------------------------------------------------------
 */
BYTE    GetPowerEvent(BYTE PowerStatus)
{
    BYTE    PowerEvent;         /* power event */

    if( gPowerStatus == PowerStatus )
    {
        PowerEvent = 0x00;      /* no changes */
    }
    else
    {
        PowerEvent = 0x01;      /* successfully changed */
    }

    return ( PowerEvent );
}
#endif
/* --------------------------------------------------------------------------
 *  get current Media status, tray open, close, disc in or not.
 * --------------------------------------------------------------------------
 *  IN:     none
 *  OUT:    ms - current media status
 * --------------------------------------------------------------------------
 */
BYTE    getMediaStatus(void)
{
    BYTE        MediaStatus;         /* current media status */

    if(St_door_open == TRUE)
    {
        MediaStatus = 0x01;      /* tray open */
    }
    else
    {
        if (St_drv_ready)  /* Test_Unit_Ready */
        {
            if ( eUnknownKind == pdbGetDiscKind() ) /* get Disc kind */
                MediaStatus = 0x00;      /* tray close, no disc */
            else
                MediaStatus = 0x02;      /* tray close, disc in */
        }
        else
        {
#if (SIGMA_SINGLE_BOARD == 1)
            if (St_become_ready == TRUE && TrayManagerInfo.TrayState == TRAY_STATE_IN_OK)
                MediaStatus = 0x02;
            else
#endif
                MediaStatus = 0x00;
        }
    }
    return( MediaStatus );
}

/* --------------------------------------------------------------------------
 *  get Media event
 * --------------------------------------------------------------------------
 *  IN:     ms - current media status
 *  OUT:    me - media event
 * --------------------------------------------------------------------------
 */
BYTE    GetMediaEvent(BYTE MediaStatus)
{
    BYTE    MediaEvent;         /* media event */

    if ( Iram_MediaTrayEvent == TRUE ) {
        if ( MediaStatus == 0x02 ) {
            /* media is present */
            MediaEvent = 0x02;      /* new media */
        } else {
            /* media is Not present */
            if ((gMediaStatus & 0x02) == 0x02)
                MediaEvent = 0x03;  /* Previous status is media present */
            else
                MediaEvent = 0x00;  /* Previous statue is no media present */
        }
        Iram_MediaTrayEvent = FALSE;
        return( MediaEvent );
    }

    if((gMediaStatus & 0x02) == (MediaStatus & 0x02))
    {
        if( TrayManagerInfo.EjectRequestEvent == TRUE)
        {
            TrayManagerInfo.EjectRequestEvent = FALSE;
            MediaEvent = 0x01;      /* tray open Request */
        }else{
            MediaEvent = 0x00;          /* no event */
        }
    }
    else
    {
        if( (MediaStatus & 0x02) == 0x02 )
        {
            /* media is present */
            MediaEvent = 0x02;      /* new media */
        }
        else
        {
            /* media is Not present */
            MediaEvent = 0x03;      /* media removal */
        }
    }

    return( MediaEvent );
}

/* --------------------------------------------------------------------------
 *  get current Media status, tray open, close, disc in or not.
 * --------------------------------------------------------------------------
 *  IN:     none
 *  OUT:    ms - current media status
 * --------------------------------------------------------------------------
 */
BYTE GetOperationalChangeStatus(void)
{
    BYTE    OperationalChangeStatus;         /* current operational status */

    if ( St_cd_prevent == TRUE )
    {
        OperationalChangeStatus = 0x80;
    }
    else
    {
        OperationalChangeStatus = 0x00;
    }

    return( OperationalChangeStatus );
}
/* --------------------------------------------------------------------------
 *  get Media event
 * --------------------------------------------------------------------------
 *  IN:     ms - current media status
 *  OUT:    me - media event
 * --------------------------------------------------------------------------
 */
BYTE GetOperationalChangeEvent(void)
{
    BYTE    OperationalChangeEvent;         /* operational event */

    if ( OperationalChangeTrayEvent != GSEN_OPE_NOTHING )
    {
        OperationalChangeEvent = 0x02;
    }
    else
    {
        OperationalChangeEvent = 0x00;
    }

    return( OperationalChangeEvent );
}
