/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   globtype.h
*
* DESCRIPTION:
*   Global types definition
*
*   $Revision: 89 $
*   $Date: 10/12/21 4:16p $
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __globtype_h__
#define __globtype_h__

#include ".\common\macros.h"
 #include ".\oem\oem_info.h"

/* signed and unsigned definition */                /* ARM compiler define                      */
typedef unsigned int               bool;
typedef unsigned char              BOOL;            /* 1 BYTE  0x00 ~ 0xFF                      */
typedef unsigned char              BYTE;            /* 1 BYTE  0x00 ~ 0xFF                      */
typedef signed char                SBYTE;           /* 1 BYTE  -128 ~ +127                      */
typedef unsigned short             USHORT;          /* 2 bytes 0x0000 ~ 0xFFFF                  */
typedef unsigned short             WORD;            /* 2 bytes 0x0000 ~ 0xFFFF                  */
typedef signed short               SHORT;           /* 2 bytes -32768(0x8000) ~ +32767(0x7FFF)  */
typedef unsigned int               UINT;            /* 4 bytes 0x0000 ~ 0xFFFFFFFF              */
typedef signed int                 INT;             /* 4bytes unsigned int                      */
typedef unsigned long              ULONG;           /* 4 bytes 0x00000000 ~ 0xFFFFFFFF          */
typedef signed long                LONG;            /* 4 bytes -2147483648(0x80000000) ~ +2147483647(0x7FFFFFFF) */

/* volatile signed and unsigned definition */       /* ARM compiler define                      */
typedef volatile unsigned char     vBOOL;           /* 1 BYTE  0x00 ~ 0xFF                      */
typedef volatile unsigned char     FLAG;
typedef volatile unsigned char     vBYTE;           /* 1 BYTE  0x00 ~ 0xFF                      */
typedef volatile signed char       vSBYTE;          /* 1 BYTE  -128 ~ +127                      */
typedef volatile unsigned short    vUSHORT;         /* 2 bytes 0x0000 ~ 0xFFFF                  */
typedef volatile unsigned short    vWORD;           /* 2 bytes 0x0000 ~ 0xFFFF                  */
typedef volatile signed short      vSHORT;          /* 2 bytes -32768(0x8000) ~ +32767(0x7FFF)  */
typedef volatile unsigned int      vUINT;           /* 4 bytes 0x0000 ~ 0xFFFFFFFF              */
typedef volatile signed int        vINT;            /* 4 bytes unsigned int                     */
typedef volatile unsigned long     vULONG;          /* 4 bytes 0x00000000 ~ 0xFFFFFFFF          */
typedef volatile signed long       vLONG;           /* 4 bytes -2147483648(0x80000000) ~ +2147483647(0x7FFFFFFF) */

typedef signed char                *LPCHAR;
typedef signed short               *LPSHORT;
typedef signed long                *LPLONG;
typedef unsigned char              *LPBYTE;
typedef unsigned short             *LPWORD;
typedef unsigned long              *LPDWORD;
typedef void                       *LPVOID;

#define CONST                       const

/* BOOLEAN expressions */
#define FALSE                       0
#define TRUE                        1

#define L0                          0x00    // Layer 0
#define L1                          0x80    // Layer 1

#if (ENABLE_UART_RXD == 1)
#define WRITE_BYTE_CMD              1
#define READ_BYTE_CMD               2
#define WRITE_EQ_WORD_CMD           3
#define READ_EQ_WORD_CMD            4
#define OPEN_MSG_CMD                5
#define CLOSE_MSG_CMD               6
#define QUIT_TRAP                   9
#define SHORT_JUMP                  10
#define ROUGH_JUMP                  11
#define SEEK_TARGET                 12
#define EJECT_KEY                   13
#define SPEED_CHANGE                14
#define DC_ERASE                    15
#define JT_TEST                     100

#define SEND_MID_CMD                22
#define DUMP_EEPROM_CMD             23
#define POWER_DOWN                  24
#define GET_TEMPERATURE             25
#define SERVO_DSP_DEBUG	            26
#define LASC_PRESET_POWER           27
#define LASC_INCR_POWER             28
#define LASC_DECR_POWER             29
#define LASC_SAVE_TO_PERSIST        30
#define LASC_READ_FROM_PERSIST      31
#define BATCH_GET_MSG_CMD           32
#define BATCH_SET_MSG_CMD           33
#define RX_CMD_END                  34

#define RX_IDLE_MODE                0
#define RX_GET_SYNC_MODE            1
#define RX_GET_CNT_MODE             2
#define RX_GET_CMD_MODE             3
#endif // #if (ENABLE_UART_RXD == 1)

/* MSF Address */
typedef struct
{
    BYTE Minute;
    BYTE Second;
    BYTE Frame;
} TimeS;


typedef struct
{
    BYTE Addr[4];
} AddrParmS;

typedef struct
{
    BYTE    Con:4;
    BYTE    Addr:4;
    BYTE    Tno;
    BYTE    Point;
    BYTE    Minute;
    BYTE    Second;
    BYTE    Frame;
    BYTE    Zero;
    BYTE    PMinute;
    BYTE    PSecond;
    BYTE    PFrame;
} SubQCodeS;

/* bit handling */
#define TEST_BIT(X, BITNO)  ((X) & (1 << (BITNO)))
#define SET_BIT(X, BITNO)   ((X) |= 1 << (BITNO))
#define CPL_BIT(X, BITNO)   ((X) ^= 1 << (BITNO))
#define CLEAR_BIT(X, BITNO) ((X) &= (~(1 << (BITNO))))

/* double word split to words (Keil compiler use high BYTE first  */
typedef union
{
    struct
    {
        USHORT  l;
        USHORT  h;
    } w;
    struct
    {
        SHORT   l;
        SHORT   h;
    } sw;
} T_ULONG2USHORTS;

/* split long to words */
#define WLOW(D)     ((T_ULONG2USHORTS *)&(D))->w.l
#define SWLOW(D)    ((T_ULONG2USHORTS *)&(D))->sw.l
#define WHIGH(D)    ((T_ULONG2USHORTS *)&(D))->w.h
#define SWHIGH(D)   ((T_ULONG2USHORTS *)&(D))->sw.h

/* word split to bytes (Keil compiler use high BYTE first  */
typedef union
{
    struct
    {
        BYTE    l;
        BYTE    h;
    } b;
    struct
    {
        SBYTE   l;
        SBYTE   h;
    } sb;
} T_USHORT2BYTES;

/* split short to bytes */
#define BLOW(D)     ((T_USHORT2BYTES *)&(D))->b.l
#define SBLOW(D)    ((T_USHORT2BYTES *)&(D))->sb.l
#define BHIGH(D)    ((T_USHORT2BYTES *)&(D))->b.h
#define SBHIGH(D)   ((T_USHORT2BYTES *)&(D))->sb.h

/* split int or unsigned int to bytes */
#define GET_BLOW(D)     ((BYTE)(D))
#define GET_SBLOW(D)    ((SBYTE)(D))
#define GET_BHIGH(D)    ((BYTE)((D) >> 8))
#define GET_SBHIGH(D)   ((SBYTE)((D) >> 8))
#define GET_BYTE2(D)    ((BYTE)((D) >> 16))
#define GET_SBYTE2(D)   ((SBYTE)((D) >> 16))
#define GET_BYTE3(D)    ((BYTE)((D) >> 24))
#define GET_SBYTE3(D)   ((SBYTE)((D) >> 24))

/* split int or unsigned int to shorts */
#define GET_WLOW(D)     ((USHORT)(D))
#define GET_SWLOW(D)    ((SHORT)(D))
#define GET_WHIGH(D)    ((USHORT)((D) >> 16))
#define GET_SWHIGH(D)   ((SHORT)((D) >> 16))


typedef struct _debug
{
    SHORT val1;
    SHORT val2;
    SHORT val3;
    SHORT val4;
    SHORT val5;
    SHORT val6;
    SHORT val7;
    SHORT val8;
} debug_t;


typedef enum _errcode
{
    eNOERR = 0,                                       // 0       0x00
    eERR_FOK_ReadAverageAdcWithFOKmV,                 // 1       0x01
    eTIMEOUT_ReadAverageAdcWithFOKmV,                 // 2       0x02
    eTIMEOUT_ReadAdcSingleChannel,                    // 3       0x03
    eERR_FOK_ad_dsp_read,                             // 4       0x04
    eLIMIT_blank_calibrate_TILT,                      // 5       0x05
    eLIMIT_rpg_calibrate_RP_gain,                     // 6       0x06
    eLOOP_expire_calibrate,                           // 7       0x07
    eLIMIT_blank_tkkg_calibrate_TE_offset,            // 8       0x08
    eLIMIT_tkkg_calibrate_TE_offset,                  // 9       0x09
    eLOOP_expire_calibrate_TE_offset,                 // 10      0x0A
    eERR_parameter_expire_calibrate_TE_gain,          // 11      0x0B
    eLIMIT_blank_trkg_calibrate_TE_gain,              // 12      0x0C
    eLIMIT_trkg_calibrate_TE_gain,                    // 13      0x0D
    eLOOP_expire_calibrate_TE_gain,                   // 14      0x0E
    eLIMIT_trkg_sppg_calibration,                     // 15      0x0F
    eLIMIT_blank_trkg_sppg_calibration,               // 16      0x10
    eLIMIT_rpg_efgh_calibration,                      // 17      0x11
    eLIMIT_blank_rpg_efgh_calibration,                // 18      0x12
    eLOOP_expire_efgh_calibration,                    // 19      0x13
    eERR_FOK_peak_bottom_adc,                         // 20      0x14
    eERR_checkWobbleLock4Clv_x_focus_error,           // 21      0x15
    eERR_checkWobbleLock4Cav_x_fok,                   // 22      0x16
    eERR_checkWobbleLock4Cav_x_timeout,               // 23      0x17
    eERR_disc_wait_watch_dog_timer_expire,            // 24      0x18
    eERR_calibrate_roc_wrong_speed,                   // 25      0x19
    eERR_checkWobbleLock4Clv_x_timeout,               // 26      0x1A
    eERR_not_power_up,                                // 27      0x1B
    eERR_wrong_target,                                // 28      0x1C
    eERR_calibrate_TE_gain_bnk_cnt_outside_lim,       // 29      0x1D
    eERR_adj_next_gain_low_limit,                     // 30      0x1E
    eERR_adj_next_gain_high_limit,                    // 31      0x1F
    eERR_adj_next_offset_over_high_limit,             // 32      0x20
    eERR_adj_next_offset_over_low_limit,              // 33      0x21
    eERR_calibrate_RP_gain_blk_gain_up_lim,           // 34      0x22
    eERR_sppg_calibration_roc_end_expire,             // 35      0x23
    eERR_calibrate_offset_written_gain_low_lim,       // 36      0x24
    eERR_calibrate_offset_blank_gain_low_lim,         // 37      0x25
    eERR_calibrate_offset_loop_expire,                // 38      0x26
    eERR_svo_HavcCal_expire,                          // 39      0x27
    eERR_svo_HavcCal_upper_limit,                     // 40      0x28
    eERR_svo_HavcCal_lower_limit,                     // 41      0x29
    eERR_calibrate_offset_wo_gain_adj_non_support_channel,//42   0x2A
    eERR_calibrate_offset_wo_gain_adj_loop_expire,    // 43      0x2B
    eERR_calibrate_ecc_time_out,                      // 44      0x2C
    eERR_calibrate_ecc_FOK,                           // 45      0x2D
    eERR_meas_pho_FOK,                                // 46      0x2E
    eERR_RF_not_present,                              // 47      0x2F
    eERR_change_to_ROM_disc,                          // 48      0x30
    eERR_change_to_RW_disc,                           // 49      0x31
    eERR_calibrate_ldpwr_read_power_loop_expire,      // 50      0x32
    eERR_calibrate_ldpwr_write_power_NZ,              // 51      0x33
    eERR_calibrate_ldpwr_write_power_loop_expire,     // 52      0x34
    eERR_calibrate_ldpwr_cav_write_power_gain_limit,  // 53      0x35
    eERR_calibrate_ldpwr_cav_write_power_loop_expire, // 54      0x36
    eERR_disc_stop_reverse_brake_timeout,             // 55      0x37
    eERR_disc_stop_short_brake_timeout,               // 56      0x38
    eERR_disc_change_rpm_timeout,                     // 57      0x39
    eERR_read_adc_p2p_mid_FOK,                        // 58      0x3A
    eERR_read_adc_p2p_mid_SAT,                        // 59      0x3B
    eERR_Trk_skating,                                 // 60      0x3C
    eERR_signal_abnormal,                         //61       0x3D
} errcode_t;

typedef enum _afeoffset_state
{
    eAFEOFFSET_STATE_UNDEFINE = 200,
    eAFEOFFSET_STATE_CLR,
    eAFEOFFSET_STATE_CDR,
    eAFEOFFSET_STATE_CDRW,
    eAFEOFFSET_STATE_DVDR,
    eAFEOFFSET_STATE_DVDRDL,
    eAFEOFFSET_STATE_DVDRW,
    eAFEOFFSET_STATE_DVDRWDL,
    eAFEOFFSET_STATE_DVDRAM,
} afeoffset_state_t;

typedef enum _ecc_code
{
    eECC_normal = 0,
    eECC_070_um,
    eECC_140_um,
    eECC_210_um,
    eECC_280_um,
} ecc_code_t;

typedef struct _ushort_return
{
    USHORT     val;
    errcode_t  errcode;
} ushort_return_t;

typedef struct _uchar_return
{
    BYTE       val;
    errcode_t  errcode;
} uchar_return_t;


typedef struct _schar_return
{
    SBYTE      val;
    errcode_t  errcode;
} schar_return_t;

typedef struct ad_dsp
{
    SHORT       middle_mv;
    SHORT       top_mv;
    SHORT       bot_mv;
    USHORT      p2p_mv;
    BOOL        top_saturated;
    BOOL        bot_saturated;
    errcode_t   errcode;
}ad_dsp_t;

typedef struct calibrated_field
{
    BYTE RPG;
    BYTE BLANKRPG;
    BYTE TRKG;
    BYTE BLANKTRKG;
    BYTE TRKOS;
}calibrated_field_t;

typedef struct field_look_up
{
    BOOL find;
    BYTE val;
}field_look_up_t;

/*
 * LittleEndian Endian union for manipulation of unsigned long / 4 byte info
 * examples include three or four byte LBA values
 */
typedef union
    {
    ULONG Long;     // Note "Long" is capitalized because lower case long is a keyword
    struct
        {
        BYTE l; /* Low byte of Long (LSB) */
        BYTE m; /* Middle byte of Long */
        BYTE h; /* High byte of Long */
        BYTE u; /* Upper byte of Long (MSB) */
        } byte;
    BYTE bf[4];
    struct
        {
            WORD l; /* Low  word of Long (LSB) */
            WORD h; /* High word of Long (LSB) */
        } word;
    } StLongU;  /* Structure Long Union (general) */

typedef struct     // Used to hold a complete physical DVD/RDVD sector/block address with layer information
{
    ULONG PSN;
    BYTE  Layer;
}PSNs;

/*
 * Little Endian union for manipulation of signed/unsigned short / 2 byte info
 * examples include two byte Transfer Length values
 */
typedef union
    {
    SHORT Sword;
    USHORT Uword;
    struct
        {
        BYTE l; /* Low byte of Word  */
        BYTE h; /* High byte of Word */
        } byte;
    } StWordU;  /* Structure Word Union (general) */

extern void send_uart1(ULONG msg_gate,BYTE d1,BYTE d2,BYTE d3,BYTE d4,BYTE d5);
extern void send_uart2(ULONG msg_gate,ULONG func_code,BYTE h1,BYTE h2);
extern void send_uart3(ULONG msg_gate,ULONG func_code,BYTE dd1,BYTE dd2);

#define BYTE0(arg)   (*(BYTE *)&(arg))
#define BYTE1(arg)   (*((BYTE *)&(arg) + 1))
#define BYTE2(arg)   (*((BYTE *)&(arg) + 2))
#define BYTE3(arg)   (*((BYTE *)&(arg) + 3))

#define LBYTE(arg)  (* (BYTE *)&(arg))
#define HBYTE(arg)  (*((BYTE *)&(arg) + 1))
#define LWORD(arg)  (* (WORD *)&(arg))
#define HWORD(arg)  (*((WORD *)&(arg) + 1))

#define A4B(D)          BYTE3(D), BYTE2(D), BYTE1(D), BYTE0(D)
#define A3B(D)          BYTE2(D), BYTE1(D), BYTE0(D)
#define A2B(D)          BYTE1(D), BYTE0(D)
#define A1B(D)          BYTE0(D)

#define F4B(D)          8,D
#define B4B(D)          4,D
#define B3B(D)          3,D
#define B2B(D)          2,D
#define B1B(D)          1,D


/********************************************************/
/*********** Defines for Parallel Debug port ************/
/********************************************************/
#if (MSG_OUT_MODE == MSG_USBPORT)
extern ULONG       svoDebugMsgFlag;		    /* svoDebbugMsgFlag */
extern ULONG       hifDebugMsgFlag;		    /* svoDebbugMsgFlag */

extern void send_msg74(ULONG msg_gate,ULONG func_code,float data1);
#endif

#endif /* __globtype_h__ */
