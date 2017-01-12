/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_host.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the HST core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_HOST_H__
#define __REG_HOST_H__

// Core register map
#define HAL_HST_VERSION 7.96
#define HAL_HST_BaseAddress 0x10000B00
#define HAL_HSTREG_MAP ((struct halHSTReg_Map*) (void *) HAL_HST_BaseAddress)
__packed struct halHSTReg_Map
{
	HAL_Reg8  ReservedBlk0[2];                //00000000
	HAL_Reg8  CTRLTR;                         //00000002
	HAL_Reg8  CTRLS;                          //00000003
	HAL_Reg8  HICTL;                          //00000004
	HAL_Reg8  HOPT;                           //00000005
	HAL_Reg8  ReservedBlk1[1];                //00000006
	HAL_Reg8  PWR0;                           //00000007
	HAL_Reg8  TSKCMP0;                        //00000008
	HAL_Reg8  TSKCMP1;                        //00000009
	HAL_Reg8  ATAPIDATA;                      //0000000A
	HAL_Reg8  PFIFOCS;                        //0000000B
	HAL_Reg8  ReservedBlk2[4];                //0000000C
	HAL_Reg16 UMISCR;                         //00000010
	HAL_Reg16 USTATR;                         //00000012
	HAL_Reg8  AESTAT;                         //00000014
	HAL_Reg8  RSSTAT;                         //00000015
	HAL_Reg8  HINT0;                          //00000016
	HAL_Reg8  ReservedBlk3[1];                //00000017
	HAL_Reg8  HINTEN0;                        //00000018
	HAL_Reg8  ReservedBlk4[7];                //00000019
	HAL_Reg8  ATDEVCTL;                       //00000020
	HAL_Reg8  ATFEA;                          //00000021
	HAL_Reg8  ATINT;                          //00000022
	HAL_Reg8  ATSPA;                          //00000023
	HAL_Reg16 ATBLO;                          //00000024
	HAL_Reg8  ATDRS;                          //00000026
	HAL_Reg8  ATCMD;                          //00000027
	HAL_Reg8  ATSTAR;                         //00000028
	HAL_Reg8  ATERRR;                         //00000029
	HAL_Reg8  SCNTDATA;                       //0000002A
	HAL_Reg8  ReservedBlk5[21];               //0000002B
	HAL_Reg8  FWRST;                          //00000040
	HAL_Reg8  ATRG;                           //00000041
	HAL_Reg8  DTTRG;                          //00000042
	HAL_Reg8  HDDIR;                          //00000043
	HAL_Reg8  ReservedBlk6[2];                //00000044
	HAL_Reg8  HPGCNTA;                        //00000046
	HAL_Reg8  IFCTRL;                         //00000047
	HAL_Reg8  ReservedBlk7[2];                //00000048
	HAL_Reg8  RXFIFOC;                        //0000004A
	HAL_Reg8  ReservedBlk8[1];                //0000004B
	HAL_Reg16 PMIDLEPL;                       //0000004C
	HAL_Reg16 PMIDLESL;                       //0000004E
	HAL_Reg16 HSTPGCL;                        //00000050
	HAL_Reg16 HAVPGCL;                        //00000052
	HAL_Reg16 HSTWDCL;                        //00000054
	HAL_Reg8  HSTSUBC;                        //00000056
	HAL_Reg8  HSTAUXW;                        //00000057
	HAL_Reg32 HSTBYTACL;                      //00000058
	HAL_Reg8  SATASPR2;                       //0000005C
	HAL_Reg8  RESWDCL;                        //0000005D
	HAL_Reg8  RESWDCH;                        //0000005E
	HAL_Reg8  ReservedBlk9[1];                //0000005F
	HAL_Reg16 HSTBUFPGL;                      //00000060
	HAL_Reg16 HSTBUFSPGL;                     //00000062
	HAL_Reg16 HSTBUFEPGL;                     //00000064
	HAL_Reg16 BUFPGCMPL;                      //00000066
	HAL_Reg16 HSTSUBAL;                       //00000068
	HAL_Reg16 HSTAUXAL;                       //0000006A
	HAL_Reg8  ReservedBlk10[4];               //0000006C
	HAL_Reg16 HSTPGCCL;                       //00000070
	HAL_Reg16 RAMPGCCL;                       //00000072
	HAL_Reg16 HSTWDCCL;                       //00000074
	HAL_Reg16 RAMWDCCL;                       //00000076
	HAL_Reg32 HSTBACCL;                       //00000078
	HAL_Reg16 HSTBUFCCL;                      //0000007C
	HAL_Reg8  MISCTL1;                        //0000007E
	HAL_Reg8  MISCTL2;                        //0000007F
	HAL_Reg8  SHCTRL;                         //00000080
	HAL_Reg8  TXREQ;                          //00000081
	HAL_Reg8  REQTXBITS;                      //00000082
	HAL_Reg8  RQTXFSTYP;                      //00000083
	HAL_Reg16 REQTXCNTL;                      //00000084
	HAL_Reg8  SATASTAT0;                      //00000086
	HAL_Reg8  SATASTAT1;                      //00000087
	HAL_Reg8  SATAINT;                        //00000088
	HAL_Reg8  SATAINTEN;                      //00000089
	HAL_Reg16 RXDFISCCL;                      //0000008A
	HAL_Reg16 TXBCNTLO;                       //0000008C
	HAL_Reg8  SATAMCTRL;                      //0000008E
	HAL_Reg8  SATADEBUG;                      //0000008F
	HAL_Reg8  SATAINT1;                       //00000090
	HAL_Reg8  SATINTEN1;                      //00000091
	HAL_Reg8  SHCTRL1;                        //00000092
	HAL_Reg8  FIS34RDY;                       //00000093
	HAL_Reg8  TOTALWDCC;                      //00000094
	HAL_Reg8  RXFIFOSAT;                      //00000095
	HAL_Reg8  RXFISTYPE;                      //00000096
	HAL_Reg8  TXFISTYPE;                      //00000097
	HAL_Reg8  BZCTL;                          //00000098
	HAL_Reg8  BZSTRPGL;                       //00000099
	HAL_Reg8  BZSTRPGH;                       //0000009A
	HAL_Reg8  BZPGLENL;                       //0000009B
	HAL_Reg8  BZPGLENH;                       //0000009C
	HAL_Reg8  ReservedBlk11[3];               //0000009D
	HAL_Reg8  CSSSCRCTL;                      //000000A0
	HAL_Reg8  CSSENCS;                        //000000A1
	HAL_Reg16 CSSENCEL;                       //000000A2
	HAL_Reg8  ReservedBlk12[12];              //000000A4
	HAL_Reg8  AESSTATUS;                      //000000B0
	HAL_Reg8  AESIN;                          //000000B1
	HAL_Reg8  AESOUT;                         //000000B2
	HAL_Reg8  AESKEY;                         //000000B3
	HAL_Reg8  AESCBC;                         //000000B4
	HAL_Reg8  AESCONFIG;                      //000000B5
	HAL_Reg8  AESCTRL;                        //000000B6
	HAL_Reg8  AESROMKEY;                      //000000B7
	HAL_Reg8  AESLENGTH;                      //000000B8
	HAL_Reg8  AESSTRTOF;                      //000000B9
	HAL_Reg8  DBECONFIG;                      //000000BA
	HAL_Reg8  DBESADR;                        //000000BB
	HAL_Reg8  DBEWIDTH;                       //000000BC
	HAL_Reg8  DBE128KEY;                      //000000BD
	HAL_Reg8  ReservedBlk13[2];               //000000BE
	HAL_Reg8  BUSENCCTL;                      //000000C0
	HAL_Reg8  BEFST;                          //000000C1
	HAL_Reg8  ReservedBlk14[2];               //000000C2
	HAL_Reg32 BEFCDSADL;                      //000000C4
	HAL_Reg32 BEFCDEADL;                      //000000C8
	HAL_Reg8  BEFBITOFF;                      //000000CC
	HAL_Reg8  BEFMAXSEC;                      //000000CD
	HAL_Reg8  BEFSECLEN;                      //000000CE
	HAL_Reg8  ReservedBlk15[1];               //000000CF
	HAL_Reg16 BEFBLKLENL;                     //000000D0
	HAL_Reg8  ReservedBlk16[2];               //000000D2
	HAL_Reg32 BEFCDCADL;                      //000000D4
	HAL_Reg8  BEFSECOFF;                      //000000D8
	HAL_Reg8  ReservedBlk17[7];               //000000D9
	HAL_Reg8  BISTMODE;                       //000000E0
	HAL_Reg8  DATAWD1L;                       //000000E1
	HAL_Reg8  DATAWD1M;                       //000000E2
	HAL_Reg8  DATAWD1H;                       //000000E3
	HAL_Reg8  DATAWD1U;                       //000000E4
	HAL_Reg8  DATAWD2L;                       //000000E5
	HAL_Reg8  DATAWD2M;                       //000000E6
	HAL_Reg8  DATAWD2H;                       //000000E7
	HAL_Reg8  DATAWD2U;                       //000000E8
	HAL_Reg8  DTWDLENL;                       //000000E9
	HAL_Reg8  DTWDLENH;                       //000000EA
	HAL_Reg8  BISTCTL;                        //000000EB
	HAL_Reg16 ERRCNTL;                        //000000EC
	HAL_Reg8  ReservedBlk18[2];               //000000EE
	HAL_Reg8  PPHYCFGL;                       //000000F0
	HAL_Reg8  PPHYCFGM;                       //000000F1
	HAL_Reg8  PPHYCFGH;                       //000000F2
	HAL_Reg8  ReservedBlk19[1];               //000000F3
	HAL_Reg8  GPHYCFGL;                       //000000F4
	HAL_Reg8  GPHYCFGH;                       //000000F5
	HAL_Reg8  PHYCTRL;                        //000000F6
	HAL_Reg8  ReservedBlk20[5];               //000000F7
	HAL_Reg8  HSTDBGMXL;                      //000000FC
	HAL_Reg8  HSTDBGMXM;                      //000000FD
	HAL_Reg8  HSTDBGMXH;                      //000000FE
	HAL_Reg8  HSTDBGMXU;                      //000000FF
};
// Register CTRLTR
#define HAL_HST_INHTRW_REGISTER    (HAL_HSTREG_MAP->CTRLTR)
#define HAL_HST_INHTRW_LSB         0
#define HAL_HST_INHTRW_MSB         0
#define HAL_HST_INHTRW_MASK        0x00000001
#define HAL_HST_INHTRW_TYPE        Bool
#define HAL_HST_INHTRW             HAL_HST_INHTRW_MASK
#define HAL_HST_INHTRW_READABLE    0
#define HAL_HST_INHTRW_WRITABLE    1
#define HAL_HST_INHTRW_SIGNED      0
#define HAL_HST_INHTRW_ADR         0x10000B02

// Register CTRLS
#define HAL_HST_SBSYCTL_REGISTER    (HAL_HSTREG_MAP->CTRLS)
#define HAL_HST_SBSYCTL_LSB         7
#define HAL_HST_SBSYCTL_MSB         7
#define HAL_HST_SBSYCTL_MASK        0x00000080
#define HAL_HST_SBSYCTL_TYPE        Bool
#define HAL_HST_SBSYCTL             HAL_HST_SBSYCTL_MASK
#define HAL_HST_SBSYCTL_READABLE    1
#define HAL_HST_SBSYCTL_WRITABLE    1
#define HAL_HST_SBSYCTL_SIGNED      0
#define HAL_HST_SBSYCTL_ADR         0x10000B03

#define HAL_HST_SCHKC_REGISTER    (HAL_HSTREG_MAP->CTRLS)
#define HAL_HST_SCHKC_LSB         4
#define HAL_HST_SCHKC_MSB         4
#define HAL_HST_SCHKC_MASK        0x00000010
#define HAL_HST_SCHKC_TYPE        Bool
#define HAL_HST_SCHKC             HAL_HST_SCHKC_MASK
#define HAL_HST_SCHKC_READABLE    1
#define HAL_HST_SCHKC_WRITABLE    1
#define HAL_HST_SCHKC_SIGNED      0
#define HAL_HST_SCHKC_ADR         0x10000B03

#define HAL_HST_SABRTC_REGISTER    (HAL_HSTREG_MAP->CTRLS)
#define HAL_HST_SABRTC_LSB         3
#define HAL_HST_SABRTC_MSB         3
#define HAL_HST_SABRTC_MASK        0x00000008
#define HAL_HST_SABRTC_TYPE        Bool
#define HAL_HST_SABRTC             HAL_HST_SABRTC_MASK
#define HAL_HST_SABRTC_READABLE    1
#define HAL_HST_SABRTC_WRITABLE    1
#define HAL_HST_SABRTC_SIGNED      0
#define HAL_HST_SABRTC_ADR         0x10000B03

// Register HICTL
#define HAL_HST_CLRBSY_REGISTER    (HAL_HSTREG_MAP->HICTL)
#define HAL_HST_CLRBSY_LSB         4
#define HAL_HST_CLRBSY_MSB         4
#define HAL_HST_CLRBSY_MASK        0x00000010
#define HAL_HST_CLRBSY_TYPE        Bool
#define HAL_HST_CLRBSY             HAL_HST_CLRBSY_MASK
#define HAL_HST_CLRBSY_READABLE    1
#define HAL_HST_CLRBSY_WRITABLE    1
#define HAL_HST_CLRBSY_SIGNED      0
#define HAL_HST_CLRBSY_ADR         0x10000B04

#define HAL_HST_SETBSY_REGISTER    (HAL_HSTREG_MAP->HICTL)
#define HAL_HST_SETBSY_LSB         3
#define HAL_HST_SETBSY_MSB         3
#define HAL_HST_SETBSY_MASK        0x00000008
#define HAL_HST_SETBSY_TYPE        Bool
#define HAL_HST_SETBSY             HAL_HST_SETBSY_MASK
#define HAL_HST_SETBSY_READABLE    1
#define HAL_HST_SETBSY_WRITABLE    1
#define HAL_HST_SETBSY_SIGNED      0
#define HAL_HST_SETBSY_ADR         0x10000B04

// Register HOPT
#define HAL_HST_AUTOA0_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_AUTOA0_LSB         7
#define HAL_HST_AUTOA0_MSB         7
#define HAL_HST_AUTOA0_MASK        0x00000080
#define HAL_HST_AUTOA0_TYPE        Bool
#define HAL_HST_AUTOA0             HAL_HST_AUTOA0_MASK
#define HAL_HST_AUTOA0_READABLE    1
#define HAL_HST_AUTOA0_WRITABLE    1
#define HAL_HST_AUTOA0_SIGNED      0
#define HAL_HST_AUTOA0_ADR         0x10000B05

#define HAL_HST_AUTODC_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_AUTODC_LSB         6
#define HAL_HST_AUTODC_MSB         6
#define HAL_HST_AUTODC_MASK        0x00000040
#define HAL_HST_AUTODC_TYPE        Bool
#define HAL_HST_AUTODC             HAL_HST_AUTODC_MASK
#define HAL_HST_AUTODC_READABLE    1
#define HAL_HST_AUTODC_WRITABLE    1
#define HAL_HST_AUTODC_SIGNED      0
#define HAL_HST_AUTODC_ADR         0x10000B05

#define HAL_HST_AUTOSC_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_AUTOSC_LSB         5
#define HAL_HST_AUTOSC_MSB         5
#define HAL_HST_AUTOSC_MASK        0x00000020
#define HAL_HST_AUTOSC_TYPE        Bool
#define HAL_HST_AUTOSC             HAL_HST_AUTOSC_MASK
#define HAL_HST_AUTOSC_READABLE    1
#define HAL_HST_AUTOSC_WRITABLE    1
#define HAL_HST_AUTOSC_SIGNED      0
#define HAL_HST_AUTOSC_ADR         0x10000B05

#define HAL_HST_AUTOAC_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_AUTOAC_LSB         4
#define HAL_HST_AUTOAC_MSB         4
#define HAL_HST_AUTOAC_MASK        0x00000010
#define HAL_HST_AUTOAC_TYPE        Bool
#define HAL_HST_AUTOAC             HAL_HST_AUTOAC_MASK
#define HAL_HST_AUTOAC_READABLE    1
#define HAL_HST_AUTOAC_WRITABLE    1
#define HAL_HST_AUTOAC_SIGNED      0
#define HAL_HST_AUTOAC_ADR         0x10000B05

#define HAL_HST_SETDBC_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_SETDBC_LSB         3
#define HAL_HST_SETDBC_MSB         3
#define HAL_HST_SETDBC_MASK        0x00000008
#define HAL_HST_SETDBC_TYPE        Bool
#define HAL_HST_SETDBC             HAL_HST_SETDBC_MASK
#define HAL_HST_SETDBC_READABLE    1
#define HAL_HST_SETDBC_WRITABLE    1
#define HAL_HST_SETDBC_SIGNED      0
#define HAL_HST_SETDBC_ADR         0x10000B05

#define HAL_HST_AUTOBC_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_AUTOBC_LSB         2
#define HAL_HST_AUTOBC_MSB         2
#define HAL_HST_AUTOBC_MASK        0x00000004
#define HAL_HST_AUTOBC_TYPE        Bool
#define HAL_HST_AUTOBC             HAL_HST_AUTOBC_MASK
#define HAL_HST_AUTOBC_READABLE    1
#define HAL_HST_AUTOBC_WRITABLE    1
#define HAL_HST_AUTOBC_SIGNED      0
#define HAL_HST_AUTOBC_ADR         0x10000B05

#define HAL_HST_AUTOBY_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_AUTOBY_LSB         1
#define HAL_HST_AUTOBY_MSB         1
#define HAL_HST_AUTOBY_MASK        0x00000002
#define HAL_HST_AUTOBY_TYPE        Bool
#define HAL_HST_AUTOBY             HAL_HST_AUTOBY_MASK
#define HAL_HST_AUTOBY_READABLE    1
#define HAL_HST_AUTOBY_WRITABLE    1
#define HAL_HST_AUTOBY_SIGNED      0
#define HAL_HST_AUTOBY_ADR         0x10000B05

#define HAL_HST_PKTIEN_REGISTER    (HAL_HSTREG_MAP->HOPT)
#define HAL_HST_PKTIEN_LSB         0
#define HAL_HST_PKTIEN_MSB         0
#define HAL_HST_PKTIEN_MASK        0x00000001
#define HAL_HST_PKTIEN_TYPE        Bool
#define HAL_HST_PKTIEN             HAL_HST_PKTIEN_MASK
#define HAL_HST_PKTIEN_READABLE    1
#define HAL_HST_PKTIEN_WRITABLE    1
#define HAL_HST_PKTIEN_SIGNED      0
#define HAL_HST_PKTIEN_ADR         0x10000B05

// Register PWR0
#define HAL_HST_SLPES_REGISTER    (HAL_HSTREG_MAP->PWR0)
#define HAL_HST_SLPES_LSB         0
#define HAL_HST_SLPES_MSB         1
#define HAL_HST_SLPES_MASK        0x00000003
#define HAL_HST_SLPES_TYPE        halHST_SleepExit_t
#define HAL_HST_SLPES             HAL_HST_SLPES_MASK
#define HAL_HST_SLPES_READABLE    1
#define HAL_HST_SLPES_WRITABLE    1
#define HAL_HST_SLPES_SIGNED      0
#define HAL_HST_SLPES_ADR         0x10000B07

// Register TSKCMP0
#define HAL_HST_ADSC_REGISTER    (HAL_HSTREG_MAP->TSKCMP0)
#define HAL_HST_ADSC_LSB         4
#define HAL_HST_ADSC_MSB         4
#define HAL_HST_ADSC_MASK        0x00000010
#define HAL_HST_ADSC_TYPE        Bool
#define HAL_HST_ADSC             HAL_HST_ADSC_MASK
#define HAL_HST_ADSC_READABLE    0
#define HAL_HST_ADSC_WRITABLE    1
#define HAL_HST_ADSC_SIGNED      0
#define HAL_HST_ADSC_ADR         0x10000B08

// Register TSKCMP1
#define HAL_HST_CDRDY_REGISTER    (HAL_HSTREG_MAP->TSKCMP1)
#define HAL_HST_CDRDY_LSB         6
#define HAL_HST_CDRDY_MSB         6
#define HAL_HST_CDRDY_MASK        0x00000040
#define HAL_HST_CDRDY_TYPE        Bool
#define HAL_HST_CDRDY             HAL_HST_CDRDY_MASK
#define HAL_HST_CDRDY_READABLE    0
#define HAL_HST_CDRDY_WRITABLE    1
#define HAL_HST_CDRDY_SIGNED      0
#define HAL_HST_CDRDY_ADR         0x10000B09

#define HAL_HST_CCORR_REGISTER    (HAL_HSTREG_MAP->TSKCMP1)
#define HAL_HST_CCORR_LSB         2
#define HAL_HST_CCORR_MSB         2
#define HAL_HST_CCORR_MASK        0x00000004
#define HAL_HST_CCORR_TYPE        Bool
#define HAL_HST_CCORR             HAL_HST_CCORR_MASK
#define HAL_HST_CCORR_READABLE    0
#define HAL_HST_CCORR_WRITABLE    1
#define HAL_HST_CCORR_SIGNED      0
#define HAL_HST_CCORR_ADR         0x10000B09

#define HAL_HST_CCHECK_REGISTER    (HAL_HSTREG_MAP->TSKCMP1)
#define HAL_HST_CCHECK_LSB         0
#define HAL_HST_CCHECK_MSB         0
#define HAL_HST_CCHECK_MASK        0x00000001
#define HAL_HST_CCHECK_TYPE        Bool
#define HAL_HST_CCHECK             HAL_HST_CCHECK_MASK
#define HAL_HST_CCHECK_READABLE    0
#define HAL_HST_CCHECK_WRITABLE    1
#define HAL_HST_CCHECK_SIGNED      0
#define HAL_HST_CCHECK_ADR         0x10000B09

// Register ATAPIDATA
#define HAL_HST_ATAPIDATA_REGISTER    (HAL_HSTREG_MAP->ATAPIDATA)
#define HAL_HST_ATAPIDATA_LSB         0
#define HAL_HST_ATAPIDATA_MSB         7
#define HAL_HST_ATAPIDATA_MASK        0x000000FF
#define HAL_HST_ATAPIDATA_TYPE        UByte
#define HAL_HST_ATAPIDATA             HAL_HST_ATAPIDATA_MASK
#define HAL_HST_ATAPIDATA_READABLE    1
#define HAL_HST_ATAPIDATA_WRITABLE    0
#define HAL_HST_ATAPIDATA_SIGNED      0
#define HAL_HST_ATAPIDATA_ADR         0x10000B0A

// Register PFIFOCS
#define HAL_HST_PKTSIZE_REGISTER    (HAL_HSTREG_MAP->PFIFOCS)
#define HAL_HST_PKTSIZE_LSB         7
#define HAL_HST_PKTSIZE_MSB         7
#define HAL_HST_PKTSIZE_MASK        0x00000080
#define HAL_HST_PKTSIZE_TYPE        halHST_PktSize_t
#define HAL_HST_PKTSIZE             HAL_HST_PKTSIZE_MASK
#define HAL_HST_PKTSIZE_READABLE    1
#define HAL_HST_PKTSIZE_WRITABLE    1
#define HAL_HST_PKTSIZE_SIGNED      0
#define HAL_HST_PKTSIZE_ADR         0x10000B0B

// Registers UMISCR/UMISCW
#define HAL_HST_HINTRQR_REGISTER    (HAL_HSTREG_MAP->UMISCR)
#define HAL_HST_HINTRQR_LSB         3
#define HAL_HST_HINTRQR_MSB         3
#define HAL_HST_HINTRQR_MASK        0x00000008
#define HAL_HST_HINTRQR_TYPE        Bool
#define HAL_HST_HINTRQR             HAL_HST_HINTRQR_MASK
#define HAL_HST_HINTRQR_READABLE    1
#define HAL_HST_HINTRQR_WRITABLE    0
#define HAL_HST_HINTRQR_SIGNED      0
#define HAL_HST_HINTRQR_ADR         0x10000B10

#define HAL_HST_NIEN_REGISTER    (HAL_HSTREG_MAP->UMISCR)
#define HAL_HST_NIEN_LSB         2
#define HAL_HST_NIEN_MSB         2
#define HAL_HST_NIEN_MASK        0x00000004
#define HAL_HST_NIEN_TYPE        Bool
#define HAL_HST_NIEN             HAL_HST_NIEN_MASK
#define HAL_HST_NIEN_READABLE    1
#define HAL_HST_NIEN_WRITABLE    0
#define HAL_HST_NIEN_SIGNED      0
#define HAL_HST_NIEN_ADR         0x10000B10

#define HAL_HST_D0NODFLT_REGISTER    (HAL_HSTREG_MAP->UMISCR)
#define HAL_HST_D0NODFLT_LSB         14
#define HAL_HST_D0NODFLT_MSB         14
#define HAL_HST_D0NODFLT_MASK        0x00004000
#define HAL_HST_D0NODFLT_TYPE        Bool
#define HAL_HST_D0NODFLT             HAL_HST_D0NODFLT_MASK
#define HAL_HST_D0NODFLT_READABLE    0
#define HAL_HST_D0NODFLT_WRITABLE    1
#define HAL_HST_D0NODFLT_SIGNED      0
#define HAL_HST_D0NODFLT_ADR         0x10000B10

#define HAL_HST_HINTRQW_REGISTER    (HAL_HSTREG_MAP->UMISCR)
#define HAL_HST_HINTRQW_LSB         11
#define HAL_HST_HINTRQW_MSB         11
#define HAL_HST_HINTRQW_MASK        0x00000800
#define HAL_HST_HINTRQW_TYPE        Bool
#define HAL_HST_HINTRQW             HAL_HST_HINTRQW_MASK
#define HAL_HST_HINTRQW_READABLE    0
#define HAL_HST_HINTRQW_WRITABLE    1
#define HAL_HST_HINTRQW_SIGNED      0
#define HAL_HST_HINTRQW_ADR         0x10000B10

// Registers USTATR/USTATW
#define HAL_HST_USENSER_REGISTER    (HAL_HSTREG_MAP->USTATR)
#define HAL_HST_USENSER_LSB         4
#define HAL_HST_USENSER_MSB         7
#define HAL_HST_USENSER_MASK        0x000000F0
#define HAL_HST_USENSER_TYPE        UByte
#define HAL_HST_USENSER             HAL_HST_USENSER_MASK
#define HAL_HST_USENSER_READABLE    1
#define HAL_HST_USENSER_WRITABLE    0
#define HAL_HST_USENSER_SIGNED      0
#define HAL_HST_USENSER_ADR         0x10000B12

#define HAL_HST_CUCRCR_REGISTER    (HAL_HSTREG_MAP->USTATR)
#define HAL_HST_CUCRCR_LSB         2
#define HAL_HST_CUCRCR_MSB         2
#define HAL_HST_CUCRCR_MASK        0x00000004
#define HAL_HST_CUCRCR_TYPE        Bool
#define HAL_HST_CUCRCR             HAL_HST_CUCRCR_MASK
#define HAL_HST_CUCRCR_READABLE    1
#define HAL_HST_CUCRCR_WRITABLE    0
#define HAL_HST_CUCRCR_SIGNED      0
#define HAL_HST_CUCRCR_ADR         0x10000B12

#define HAL_HST_UCRCERR_REGISTER    (HAL_HSTREG_MAP->USTATR)
#define HAL_HST_UCRCERR_LSB         0
#define HAL_HST_UCRCERR_MSB         0
#define HAL_HST_UCRCERR_MASK        0x00000001
#define HAL_HST_UCRCERR_TYPE        Bool
#define HAL_HST_UCRCERR             HAL_HST_UCRCERR_MASK
#define HAL_HST_UCRCERR_READABLE    1
#define HAL_HST_UCRCERR_WRITABLE    0
#define HAL_HST_UCRCERR_SIGNED      0
#define HAL_HST_UCRCERR_ADR         0x10000B12

#define HAL_HST_USENSEW_REGISTER    (HAL_HSTREG_MAP->USTATR)
#define HAL_HST_USENSEW_LSB         12
#define HAL_HST_USENSEW_MSB         15
#define HAL_HST_USENSEW_MASK        0x0000F000
#define HAL_HST_USENSEW_TYPE        UByte
#define HAL_HST_USENSEW             HAL_HST_USENSEW_MASK
#define HAL_HST_USENSEW_READABLE    0
#define HAL_HST_USENSEW_WRITABLE    1
#define HAL_HST_USENSEW_SIGNED      0
#define HAL_HST_USENSEW_ADR         0x10000B12

#define HAL_HST_CUCRCW_REGISTER    (HAL_HSTREG_MAP->USTATR)
#define HAL_HST_CUCRCW_LSB         10
#define HAL_HST_CUCRCW_MSB         10
#define HAL_HST_CUCRCW_MASK        0x00000400
#define HAL_HST_CUCRCW_TYPE        Bool
#define HAL_HST_CUCRCW             HAL_HST_CUCRCW_MASK
#define HAL_HST_CUCRCW_READABLE    0
#define HAL_HST_CUCRCW_WRITABLE    1
#define HAL_HST_CUCRCW_SIGNED      0
#define HAL_HST_CUCRCW_ADR         0x10000B12

#define HAL_HST_CLRCRCE_REGISTER    (HAL_HSTREG_MAP->USTATR)
#define HAL_HST_CLRCRCE_LSB         8
#define HAL_HST_CLRCRCE_MSB         8
#define HAL_HST_CLRCRCE_MASK        0x00000100
#define HAL_HST_CLRCRCE_TYPE        Bool
#define HAL_HST_CLRCRCE             HAL_HST_CLRCRCE_MASK
#define HAL_HST_CLRCRCE_READABLE    0
#define HAL_HST_CLRCRCE_WRITABLE    1
#define HAL_HST_CLRCRCE_SIGNED      0
#define HAL_HST_CLRCRCE_ADR         0x10000B12

// Register AESTAT
#define HAL_HST_SRSTD_REGISTER    (HAL_HSTREG_MAP->AESTAT)
#define HAL_HST_SRSTD_LSB         7
#define HAL_HST_SRSTD_MSB         7
#define HAL_HST_SRSTD_MASK        0x00000080
#define HAL_HST_SRSTD_TYPE        Bool
#define HAL_HST_SRSTD             HAL_HST_SRSTD_MASK
#define HAL_HST_SRSTD_READABLE    1
#define HAL_HST_SRSTD_WRITABLE    0
#define HAL_HST_SRSTD_SIGNED      0
#define HAL_HST_SRSTD_ADR         0x10000B14

#define HAL_HST_CCONFL_REGISTER    (HAL_HSTREG_MAP->AESTAT)
#define HAL_HST_CCONFL_LSB         6
#define HAL_HST_CCONFL_MSB         6
#define HAL_HST_CCONFL_MASK        0x00000040
#define HAL_HST_CCONFL_TYPE        Bool
#define HAL_HST_CCONFL             HAL_HST_CCONFL_MASK
#define HAL_HST_CCONFL_READABLE    1
#define HAL_HST_CCONFL_WRITABLE    0
#define HAL_HST_CCONFL_SIGNED      0
#define HAL_HST_CCONFL_ADR         0x10000B14

#define HAL_HST_CMDLST_REGISTER    (HAL_HSTREG_MAP->AESTAT)
#define HAL_HST_CMDLST_LSB         4
#define HAL_HST_CMDLST_MSB         4
#define HAL_HST_CMDLST_MASK        0x00000010
#define HAL_HST_CMDLST_TYPE        Bool
#define HAL_HST_CMDLST             HAL_HST_CMDLST_MASK
#define HAL_HST_CMDLST_READABLE    1
#define HAL_HST_CMDLST_WRITABLE    0
#define HAL_HST_CMDLST_SIGNED      0
#define HAL_HST_CMDLST_ADR         0x10000B14

#define HAL_HST_SCABRTD_REGISTER    (HAL_HSTREG_MAP->AESTAT)
#define HAL_HST_SCABRTD_LSB         3
#define HAL_HST_SCABRTD_MSB         3
#define HAL_HST_SCABRTD_MASK        0x00000008
#define HAL_HST_SCABRTD_TYPE        Bool
#define HAL_HST_SCABRTD             HAL_HST_SCABRTD_MASK
#define HAL_HST_SCABRTD_READABLE    1
#define HAL_HST_SCABRTD_WRITABLE    0
#define HAL_HST_SCABRTD_SIGNED      0
#define HAL_HST_SCABRTD_ADR         0x10000B14

#define HAL_HST_FPKT_REGISTER    (HAL_HSTREG_MAP->AESTAT)
#define HAL_HST_FPKT_LSB         1
#define HAL_HST_FPKT_MSB         1
#define HAL_HST_FPKT_MASK        0x00000002
#define HAL_HST_FPKT_TYPE        Bool
#define HAL_HST_FPKT             HAL_HST_FPKT_MASK
#define HAL_HST_FPKT_READABLE    1
#define HAL_HST_FPKT_WRITABLE    0
#define HAL_HST_FPKT_SIGNED      0
#define HAL_HST_FPKT_ADR         0x10000B14

#define HAL_HST_DTENB_REGISTER    (HAL_HSTREG_MAP->AESTAT)
#define HAL_HST_DTENB_LSB         0
#define HAL_HST_DTENB_MSB         0
#define HAL_HST_DTENB_MASK        0x00000001
#define HAL_HST_DTENB_TYPE        Bool
#define HAL_HST_DTENB             HAL_HST_DTENB_MASK
#define HAL_HST_DTENB_READABLE    1
#define HAL_HST_DTENB_WRITABLE    0
#define HAL_HST_DTENB_SIGNED      0
#define HAL_HST_DTENB_ADR         0x10000B14

// Register RSSTAT
#define HAL_HST_SRSTF_REGISTER    (HAL_HSTREG_MAP->RSSTAT)
#define HAL_HST_SRSTF_LSB         7
#define HAL_HST_SRSTF_MSB         7
#define HAL_HST_SRSTF_MASK        0x00000080
#define HAL_HST_SRSTF_TYPE        Bool
#define HAL_HST_SRSTF             HAL_HST_SRSTF_MASK
#define HAL_HST_SRSTF_READABLE    1
#define HAL_HST_SRSTF_WRITABLE    0
#define HAL_HST_SRSTF_SIGNED      0
#define HAL_HST_SRSTF_ADR         0x10000B15

#define HAL_HST_CMDF_REGISTER    (HAL_HSTREG_MAP->RSSTAT)
#define HAL_HST_CMDF_LSB         6
#define HAL_HST_CMDF_MSB         6
#define HAL_HST_CMDF_MASK        0x00000040
#define HAL_HST_CMDF_TYPE        Bool
#define HAL_HST_CMDF             HAL_HST_CMDF_MASK
#define HAL_HST_CMDF_READABLE    1
#define HAL_HST_CMDF_WRITABLE    0
#define HAL_HST_CMDF_SIGNED      0
#define HAL_HST_CMDF_ADR         0x10000B15

#define HAL_HST_DIAGCMD_REGISTER    (HAL_HSTREG_MAP->RSSTAT)
#define HAL_HST_DIAGCMD_LSB         5
#define HAL_HST_DIAGCMD_MSB         5
#define HAL_HST_DIAGCMD_MASK        0x00000020
#define HAL_HST_DIAGCMD_TYPE        Bool
#define HAL_HST_DIAGCMD             HAL_HST_DIAGCMD_MASK
#define HAL_HST_DIAGCMD_READABLE    1
#define HAL_HST_DIAGCMD_WRITABLE    0
#define HAL_HST_DIAGCMD_SIGNED      0
#define HAL_HST_DIAGCMD_ADR         0x10000B15

// Register HINT0
#define HAL_HST_XPAUSE_REGISTER    (HAL_HSTREG_MAP->HINT0)
#define HAL_HST_XPAUSE_LSB         7
#define HAL_HST_XPAUSE_MSB         7
#define HAL_HST_XPAUSE_MASK        0x00000080
#define HAL_HST_XPAUSE_TYPE        Bool
#define HAL_HST_XPAUSE             HAL_HST_XPAUSE_MASK
#define HAL_HST_XPAUSE_READABLE    1
#define HAL_HST_XPAUSE_WRITABLE    1
#define HAL_HST_XPAUSE_SIGNED      0
#define HAL_HST_XPAUSE_ADR         0x10000B16

#define HAL_HST_SRST_REGISTER    (HAL_HSTREG_MAP->HINT0)
#define HAL_HST_SRST_LSB         6
#define HAL_HST_SRST_MSB         6
#define HAL_HST_SRST_MASK        0x00000040
#define HAL_HST_SRST_TYPE        Bool
#define HAL_HST_SRST             HAL_HST_SRST_MASK
#define HAL_HST_SRST_READABLE    1
#define HAL_HST_SRST_WRITABLE    1
#define HAL_HST_SRST_SIGNED      0
#define HAL_HST_SRST_ADR         0x10000B16

#define HAL_HST_ARST_REGISTER    (HAL_HSTREG_MAP->HINT0)
#define HAL_HST_ARST_LSB         5
#define HAL_HST_ARST_MSB         5
#define HAL_HST_ARST_MASK        0x00000020
#define HAL_HST_ARST_TYPE        Bool
#define HAL_HST_ARST             HAL_HST_ARST_MASK
#define HAL_HST_ARST_READABLE    1
#define HAL_HST_ARST_WRITABLE    1
#define HAL_HST_ARST_SIGNED      0
#define HAL_HST_ARST_ADR         0x10000B16

#define HAL_HST_DTE_REGISTER    (HAL_HSTREG_MAP->HINT0)
#define HAL_HST_DTE_LSB         2
#define HAL_HST_DTE_MSB         2
#define HAL_HST_DTE_MASK        0x00000004
#define HAL_HST_DTE_TYPE        Bool
#define HAL_HST_DTE             HAL_HST_DTE_MASK
#define HAL_HST_DTE_READABLE    1
#define HAL_HST_DTE_WRITABLE    1
#define HAL_HST_DTE_SIGNED      0
#define HAL_HST_DTE_ADR         0x10000B16

#define HAL_HST_PCMD_REGISTER    (HAL_HSTREG_MAP->HINT0)
#define HAL_HST_PCMD_LSB         1
#define HAL_HST_PCMD_MSB         1
#define HAL_HST_PCMD_MASK        0x00000002
#define HAL_HST_PCMD_TYPE        Bool
#define HAL_HST_PCMD             HAL_HST_PCMD_MASK
#define HAL_HST_PCMD_READABLE    1
#define HAL_HST_PCMD_WRITABLE    1
#define HAL_HST_PCMD_SIGNED      0
#define HAL_HST_PCMD_ADR         0x10000B16

#define HAL_HST_CMD_REGISTER    (HAL_HSTREG_MAP->HINT0)
#define HAL_HST_CMD_LSB         0
#define HAL_HST_CMD_MSB         0
#define HAL_HST_CMD_MASK        0x00000001
#define HAL_HST_CMD_TYPE        Bool
#define HAL_HST_CMD             HAL_HST_CMD_MASK
#define HAL_HST_CMD_READABLE    1
#define HAL_HST_CMD_WRITABLE    1
#define HAL_HST_CMD_SIGNED      0
#define HAL_HST_CMD_ADR         0x10000B16

// Register HINTEN0
#define HAL_HST_XPAUSEEN_REGISTER    (HAL_HSTREG_MAP->HINTEN0)
#define HAL_HST_XPAUSEEN_LSB         7
#define HAL_HST_XPAUSEEN_MSB         7
#define HAL_HST_XPAUSEEN_MASK        0x00000080
#define HAL_HST_XPAUSEEN_TYPE        Bool
#define HAL_HST_XPAUSEEN             HAL_HST_XPAUSEEN_MASK
#define HAL_HST_XPAUSEEN_READABLE    1
#define HAL_HST_XPAUSEEN_WRITABLE    1
#define HAL_HST_XPAUSEEN_SIGNED      0
#define HAL_HST_XPAUSEEN_ADR         0x10000B18

#define HAL_HST_SRSTEN_REGISTER    (HAL_HSTREG_MAP->HINTEN0)
#define HAL_HST_SRSTEN_LSB         6
#define HAL_HST_SRSTEN_MSB         6
#define HAL_HST_SRSTEN_MASK        0x00000040
#define HAL_HST_SRSTEN_TYPE        Bool
#define HAL_HST_SRSTEN             HAL_HST_SRSTEN_MASK
#define HAL_HST_SRSTEN_READABLE    1
#define HAL_HST_SRSTEN_WRITABLE    1
#define HAL_HST_SRSTEN_SIGNED      0
#define HAL_HST_SRSTEN_ADR         0x10000B18

#define HAL_HST_ARSTEN_REGISTER    (HAL_HSTREG_MAP->HINTEN0)
#define HAL_HST_ARSTEN_LSB         5
#define HAL_HST_ARSTEN_MSB         5
#define HAL_HST_ARSTEN_MASK        0x00000020
#define HAL_HST_ARSTEN_TYPE        Bool
#define HAL_HST_ARSTEN             HAL_HST_ARSTEN_MASK
#define HAL_HST_ARSTEN_READABLE    1
#define HAL_HST_ARSTEN_WRITABLE    1
#define HAL_HST_ARSTEN_SIGNED      0
#define HAL_HST_ARSTEN_ADR         0x10000B18

#define HAL_HST_DTEEN_REGISTER    (HAL_HSTREG_MAP->HINTEN0)
#define HAL_HST_DTEEN_LSB         2
#define HAL_HST_DTEEN_MSB         2
#define HAL_HST_DTEEN_MASK        0x00000004
#define HAL_HST_DTEEN_TYPE        Bool
#define HAL_HST_DTEEN             HAL_HST_DTEEN_MASK
#define HAL_HST_DTEEN_READABLE    1
#define HAL_HST_DTEEN_WRITABLE    1
#define HAL_HST_DTEEN_SIGNED      0
#define HAL_HST_DTEEN_ADR         0x10000B18

#define HAL_HST_PCMDEN_REGISTER    (HAL_HSTREG_MAP->HINTEN0)
#define HAL_HST_PCMDEN_LSB         1
#define HAL_HST_PCMDEN_MSB         1
#define HAL_HST_PCMDEN_MASK        0x00000002
#define HAL_HST_PCMDEN_TYPE        Bool
#define HAL_HST_PCMDEN             HAL_HST_PCMDEN_MASK
#define HAL_HST_PCMDEN_READABLE    1
#define HAL_HST_PCMDEN_WRITABLE    1
#define HAL_HST_PCMDEN_SIGNED      0
#define HAL_HST_PCMDEN_ADR         0x10000B18

#define HAL_HST_CMDEN_REGISTER    (HAL_HSTREG_MAP->HINTEN0)
#define HAL_HST_CMDEN_LSB         0
#define HAL_HST_CMDEN_MSB         0
#define HAL_HST_CMDEN_MASK        0x00000001
#define HAL_HST_CMDEN_TYPE        Bool
#define HAL_HST_CMDEN             HAL_HST_CMDEN_MASK
#define HAL_HST_CMDEN_READABLE    1
#define HAL_HST_CMDEN_WRITABLE    1
#define HAL_HST_CMDEN_SIGNED      0
#define HAL_HST_CMDEN_ADR         0x10000B18

// Register ATDEVCTL
#define HAL_HST_HSRST_REGISTER    (HAL_HSTREG_MAP->ATDEVCTL)
#define HAL_HST_HSRST_LSB         2
#define HAL_HST_HSRST_MSB         2
#define HAL_HST_HSRST_MASK        0x00000004
#define HAL_HST_HSRST_TYPE        Bool
#define HAL_HST_HSRST             HAL_HST_HSRST_MASK
#define HAL_HST_HSRST_READABLE    1
#define HAL_HST_HSRST_WRITABLE    0
#define HAL_HST_HSRST_SIGNED      0
#define HAL_HST_HSRST_ADR         0x10000B20

#define HAL_HST_HNIEN_REGISTER    (HAL_HSTREG_MAP->ATDEVCTL)
#define HAL_HST_HNIEN_LSB         1
#define HAL_HST_HNIEN_MSB         1
#define HAL_HST_HNIEN_MASK        0x00000002
#define HAL_HST_HNIEN_TYPE        Bool
#define HAL_HST_HNIEN             HAL_HST_HNIEN_MASK
#define HAL_HST_HNIEN_READABLE    1
#define HAL_HST_HNIEN_WRITABLE    0
#define HAL_HST_HNIEN_SIGNED      0
#define HAL_HST_HNIEN_ADR         0x10000B20

// Registers ATFEA/ATERRW
#define HAL_HST_TF_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_TF_LSB         1
#define HAL_HST_TF_MSB         7
#define HAL_HST_TF_MASK        0x000000FE
#define HAL_HST_TF_TYPE        UByte
#define HAL_HST_TF             HAL_HST_TF_MASK
#define HAL_HST_TF_READABLE    1
#define HAL_HST_TF_WRITABLE    0
#define HAL_HST_TF_SIGNED      0
#define HAL_HST_TF_ADR         0x10000B21

#define HAL_HST_DMA_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_DMA_LSB         0
#define HAL_HST_DMA_MSB         0
#define HAL_HST_DMA_MASK        0x00000001
#define HAL_HST_DMA_TYPE        Bool
#define HAL_HST_DMA             HAL_HST_DMA_MASK
#define HAL_HST_DMA_READABLE    1
#define HAL_HST_DMA_WRITABLE    0
#define HAL_HST_DMA_SIGNED      0
#define HAL_HST_DMA_ADR         0x10000B21

#define HAL_HST_SENSEW_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_SENSEW_LSB         4
#define HAL_HST_SENSEW_MSB         7
#define HAL_HST_SENSEW_MASK        0x000000F0
#define HAL_HST_SENSEW_TYPE        UByte
#define HAL_HST_SENSEW             HAL_HST_SENSEW_MASK
#define HAL_HST_SENSEW_READABLE    0
#define HAL_HST_SENSEW_WRITABLE    1
#define HAL_HST_SENSEW_SIGNED      0
#define HAL_HST_SENSEW_ADR         0x10000B21

#define HAL_HST_MCRW_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_MCRW_LSB         3
#define HAL_HST_MCRW_MSB         3
#define HAL_HST_MCRW_MASK        0x00000008
#define HAL_HST_MCRW_TYPE        Bool
#define HAL_HST_MCRW             HAL_HST_MCRW_MASK
#define HAL_HST_MCRW_READABLE    0
#define HAL_HST_MCRW_WRITABLE    1
#define HAL_HST_MCRW_SIGNED      0
#define HAL_HST_MCRW_ADR         0x10000B21

#define HAL_HST_ABRTW_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_ABRTW_LSB         2
#define HAL_HST_ABRTW_MSB         2
#define HAL_HST_ABRTW_MASK        0x00000004
#define HAL_HST_ABRTW_TYPE        Bool
#define HAL_HST_ABRTW             HAL_HST_ABRTW_MASK
#define HAL_HST_ABRTW_READABLE    0
#define HAL_HST_ABRTW_WRITABLE    1
#define HAL_HST_ABRTW_SIGNED      0
#define HAL_HST_ABRTW_ADR         0x10000B21

#define HAL_HST_EOMW_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_EOMW_LSB         1
#define HAL_HST_EOMW_MSB         1
#define HAL_HST_EOMW_MASK        0x00000002
#define HAL_HST_EOMW_TYPE        Bool
#define HAL_HST_EOMW             HAL_HST_EOMW_MASK
#define HAL_HST_EOMW_READABLE    0
#define HAL_HST_EOMW_WRITABLE    1
#define HAL_HST_EOMW_SIGNED      0
#define HAL_HST_EOMW_ADR         0x10000B21

#define HAL_HST_ILIW_REGISTER    (HAL_HSTREG_MAP->ATFEA)
#define HAL_HST_ILIW_LSB         0
#define HAL_HST_ILIW_MSB         0
#define HAL_HST_ILIW_MASK        0x00000001
#define HAL_HST_ILIW_TYPE        Bool
#define HAL_HST_ILIW             HAL_HST_ILIW_MASK
#define HAL_HST_ILIW_READABLE    0
#define HAL_HST_ILIW_WRITABLE    1
#define HAL_HST_ILIW_SIGNED      0
#define HAL_HST_ILIW_ADR         0x10000B21

// Register ATINT
#define HAL_HST_TI_REGISTER    (HAL_HSTREG_MAP->ATINT)
#define HAL_HST_TI_LSB         2
#define HAL_HST_TI_MSB         7
#define HAL_HST_TI_MASK        0x000000FC
#define HAL_HST_TI_TYPE        UByte
#define HAL_HST_TI             HAL_HST_TI_MASK
#define HAL_HST_TI_READABLE    1
#define HAL_HST_TI_WRITABLE    1
#define HAL_HST_TI_SIGNED      0
#define HAL_HST_TI_ADR         0x10000B22

#define HAL_HST_IO_REGISTER    (HAL_HSTREG_MAP->ATINT)
#define HAL_HST_IO_LSB         1
#define HAL_HST_IO_MSB         1
#define HAL_HST_IO_MASK        0x00000002
#define HAL_HST_IO_TYPE        Bool
#define HAL_HST_IO             HAL_HST_IO_MASK
#define HAL_HST_IO_READABLE    1
#define HAL_HST_IO_WRITABLE    1
#define HAL_HST_IO_SIGNED      0
#define HAL_HST_IO_ADR         0x10000B22

#define HAL_HST_COD_REGISTER    (HAL_HSTREG_MAP->ATINT)
#define HAL_HST_COD_LSB         0
#define HAL_HST_COD_MSB         0
#define HAL_HST_COD_MASK        0x00000001
#define HAL_HST_COD_TYPE        Bool
#define HAL_HST_COD             HAL_HST_COD_MASK
#define HAL_HST_COD_READABLE    1
#define HAL_HST_COD_WRITABLE    1
#define HAL_HST_COD_SIGNED      0
#define HAL_HST_COD_ADR         0x10000B22

// Register ATSPA
#define HAL_HST_TS_REGISTER    (HAL_HSTREG_MAP->ATSPA)
#define HAL_HST_TS_LSB         0
#define HAL_HST_TS_MSB         7
#define HAL_HST_TS_MASK        0x000000FF
#define HAL_HST_TS_TYPE        UByte
#define HAL_HST_TS             HAL_HST_TS_MASK
#define HAL_HST_TS_READABLE    1
#define HAL_HST_TS_WRITABLE    1
#define HAL_HST_TS_SIGNED      0
#define HAL_HST_TS_ADR         0x10000B23

// Registers ATBLO/ATBHI
#define HAL_HST_TBL_REGISTER    (HAL_HSTREG_MAP->ATBLO)
#define HAL_HST_TBL_LSB         0
#define HAL_HST_TBL_MSB         7
#define HAL_HST_TBL_MASK        0x000000FF
#define HAL_HST_TBL_TYPE        UByte
#define HAL_HST_TBL             HAL_HST_TBL_MASK
#define HAL_HST_TBL_READABLE    1
#define HAL_HST_TBL_WRITABLE    1
#define HAL_HST_TBL_SIGNED      0
#define HAL_HST_TBL_ADR         0x10000B24

#define HAL_HST_TBH_REGISTER    (HAL_HSTREG_MAP->ATBLO)
#define HAL_HST_TBH_LSB         8
#define HAL_HST_TBH_MSB         15
#define HAL_HST_TBH_MASK        0x0000FF00
#define HAL_HST_TBH_TYPE        UByte
#define HAL_HST_TBH             HAL_HST_TBH_MASK
#define HAL_HST_TBH_READABLE    1
#define HAL_HST_TBH_WRITABLE    1
#define HAL_HST_TBH_SIGNED      0
#define HAL_HST_TBH_ADR         0x10000B24

// Register ATDRS
#define HAL_HST_TD_REGISTER    (HAL_HSTREG_MAP->ATDRS)
#define HAL_HST_TD_LSB         0
#define HAL_HST_TD_MSB         7
#define HAL_HST_TD_MASK        0x000000FF
#define HAL_HST_TD_TYPE        UByte
#define HAL_HST_TD             HAL_HST_TD_MASK
#define HAL_HST_TD_READABLE    1
#define HAL_HST_TD_WRITABLE    1
#define HAL_HST_TD_SIGNED      0
#define HAL_HST_TD_ADR         0x10000B26

#define HAL_HST_DRV_REGISTER    (HAL_HSTREG_MAP->ATDRS)
#define HAL_HST_DRV_LSB         4
#define HAL_HST_DRV_MSB         4
#define HAL_HST_DRV_MASK        0x00000010
#define HAL_HST_DRV_TYPE        Bool
#define HAL_HST_DRV             HAL_HST_DRV_MASK
#define HAL_HST_DRV_READABLE    1
#define HAL_HST_DRV_WRITABLE    1
#define HAL_HST_DRV_SIGNED      0
#define HAL_HST_DRV_ADR         0x10000B26

// Registers ATCMD/ATSTAW
#define HAL_HST_ATCMD_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_ATCMD_LSB         0
#define HAL_HST_ATCMD_MSB         7
#define HAL_HST_ATCMD_MASK        0x000000FF
#define HAL_HST_ATCMD_TYPE        UByte
#define HAL_HST_ATCMD             HAL_HST_ATCMD_MASK
#define HAL_HST_ATCMD_READABLE    1
#define HAL_HST_ATCMD_WRITABLE    0
#define HAL_HST_ATCMD_SIGNED      0
#define HAL_HST_ATCMD_ADR         0x10000B27

#define HAL_HST_DRDYW_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_DRDYW_LSB         6
#define HAL_HST_DRDYW_MSB         6
#define HAL_HST_DRDYW_MASK        0x00000040
#define HAL_HST_DRDYW_TYPE        Bool
#define HAL_HST_DRDYW             HAL_HST_DRDYW_MASK
#define HAL_HST_DRDYW_READABLE    0
#define HAL_HST_DRDYW_WRITABLE    1
#define HAL_HST_DRDYW_SIGNED      0
#define HAL_HST_DRDYW_ADR         0x10000B27

#define HAL_HST_DFW_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_DFW_LSB         5
#define HAL_HST_DFW_MSB         5
#define HAL_HST_DFW_MASK        0x00000020
#define HAL_HST_DFW_TYPE        Bool
#define HAL_HST_DFW             HAL_HST_DFW_MASK
#define HAL_HST_DFW_READABLE    0
#define HAL_HST_DFW_WRITABLE    1
#define HAL_HST_DFW_SIGNED      0
#define HAL_HST_DFW_ADR         0x10000B27

#define HAL_HST_DSCW_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_DSCW_LSB         4
#define HAL_HST_DSCW_MSB         4
#define HAL_HST_DSCW_MASK        0x00000010
#define HAL_HST_DSCW_TYPE        Bool
#define HAL_HST_DSCW             HAL_HST_DSCW_MASK
#define HAL_HST_DSCW_READABLE    0
#define HAL_HST_DSCW_WRITABLE    1
#define HAL_HST_DSCW_SIGNED      0
#define HAL_HST_DSCW_ADR         0x10000B27

#define HAL_HST_DRQW_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_DRQW_LSB         3
#define HAL_HST_DRQW_MSB         3
#define HAL_HST_DRQW_MASK        0x00000008
#define HAL_HST_DRQW_TYPE        Bool
#define HAL_HST_DRQW             HAL_HST_DRQW_MASK
#define HAL_HST_DRQW_READABLE    0
#define HAL_HST_DRQW_WRITABLE    1
#define HAL_HST_DRQW_SIGNED      0
#define HAL_HST_DRQW_ADR         0x10000B27

#define HAL_HST_CORRW_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_CORRW_LSB         2
#define HAL_HST_CORRW_MSB         2
#define HAL_HST_CORRW_MASK        0x00000004
#define HAL_HST_CORRW_TYPE        Bool
#define HAL_HST_CORRW             HAL_HST_CORRW_MASK
#define HAL_HST_CORRW_READABLE    0
#define HAL_HST_CORRW_WRITABLE    1
#define HAL_HST_CORRW_SIGNED      0
#define HAL_HST_CORRW_ADR         0x10000B27

#define HAL_HST_ST1W_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_ST1W_LSB         1
#define HAL_HST_ST1W_MSB         1
#define HAL_HST_ST1W_MASK        0x00000002
#define HAL_HST_ST1W_TYPE        Bool
#define HAL_HST_ST1W             HAL_HST_ST1W_MASK
#define HAL_HST_ST1W_READABLE    0
#define HAL_HST_ST1W_WRITABLE    1
#define HAL_HST_ST1W_SIGNED      0
#define HAL_HST_ST1W_ADR         0x10000B27

#define HAL_HST_CHECKW_REGISTER    (HAL_HSTREG_MAP->ATCMD)
#define HAL_HST_CHECKW_LSB         0
#define HAL_HST_CHECKW_MSB         0
#define HAL_HST_CHECKW_MASK        0x00000001
#define HAL_HST_CHECKW_TYPE        Bool
#define HAL_HST_CHECKW             HAL_HST_CHECKW_MASK
#define HAL_HST_CHECKW_READABLE    0
#define HAL_HST_CHECKW_WRITABLE    1
#define HAL_HST_CHECKW_SIGNED      0
#define HAL_HST_CHECKW_ADR         0x10000B27

// Register ATSTAR
#define HAL_HST_BSY_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_BSY_LSB         7
#define HAL_HST_BSY_MSB         7
#define HAL_HST_BSY_MASK        0x00000080
#define HAL_HST_BSY_TYPE        Bool
#define HAL_HST_BSY             HAL_HST_BSY_MASK
#define HAL_HST_BSY_READABLE    1
#define HAL_HST_BSY_WRITABLE    0
#define HAL_HST_BSY_SIGNED      0
#define HAL_HST_BSY_ADR         0x10000B28

#define HAL_HST_DRDYR_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_DRDYR_LSB         6
#define HAL_HST_DRDYR_MSB         6
#define HAL_HST_DRDYR_MASK        0x00000040
#define HAL_HST_DRDYR_TYPE        Bool
#define HAL_HST_DRDYR             HAL_HST_DRDYR_MASK
#define HAL_HST_DRDYR_READABLE    1
#define HAL_HST_DRDYR_WRITABLE    0
#define HAL_HST_DRDYR_SIGNED      0
#define HAL_HST_DRDYR_ADR         0x10000B28

#define HAL_HST_DFR_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_DFR_LSB         5
#define HAL_HST_DFR_MSB         5
#define HAL_HST_DFR_MASK        0x00000020
#define HAL_HST_DFR_TYPE        Bool
#define HAL_HST_DFR             HAL_HST_DFR_MASK
#define HAL_HST_DFR_READABLE    1
#define HAL_HST_DFR_WRITABLE    0
#define HAL_HST_DFR_SIGNED      0
#define HAL_HST_DFR_ADR         0x10000B28

#define HAL_HST_DSCR_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_DSCR_LSB         4
#define HAL_HST_DSCR_MSB         4
#define HAL_HST_DSCR_MASK        0x00000010
#define HAL_HST_DSCR_TYPE        Bool
#define HAL_HST_DSCR             HAL_HST_DSCR_MASK
#define HAL_HST_DSCR_READABLE    1
#define HAL_HST_DSCR_WRITABLE    0
#define HAL_HST_DSCR_SIGNED      0
#define HAL_HST_DSCR_ADR         0x10000B28

#define HAL_HST_DRQR_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_DRQR_LSB         3
#define HAL_HST_DRQR_MSB         3
#define HAL_HST_DRQR_MASK        0x00000008
#define HAL_HST_DRQR_TYPE        Bool
#define HAL_HST_DRQR             HAL_HST_DRQR_MASK
#define HAL_HST_DRQR_READABLE    1
#define HAL_HST_DRQR_WRITABLE    0
#define HAL_HST_DRQR_SIGNED      0
#define HAL_HST_DRQR_ADR         0x10000B28

#define HAL_HST_CORRR_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_CORRR_LSB         2
#define HAL_HST_CORRR_MSB         2
#define HAL_HST_CORRR_MASK        0x00000004
#define HAL_HST_CORRR_TYPE        Bool
#define HAL_HST_CORRR             HAL_HST_CORRR_MASK
#define HAL_HST_CORRR_READABLE    1
#define HAL_HST_CORRR_WRITABLE    0
#define HAL_HST_CORRR_SIGNED      0
#define HAL_HST_CORRR_ADR         0x10000B28

#define HAL_HST_ST1R_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_ST1R_LSB         1
#define HAL_HST_ST1R_MSB         1
#define HAL_HST_ST1R_MASK        0x00000002
#define HAL_HST_ST1R_TYPE        Bool
#define HAL_HST_ST1R             HAL_HST_ST1R_MASK
#define HAL_HST_ST1R_READABLE    1
#define HAL_HST_ST1R_WRITABLE    0
#define HAL_HST_ST1R_SIGNED      0
#define HAL_HST_ST1R_ADR         0x10000B28

#define HAL_HST_CHECKR_REGISTER    (HAL_HSTREG_MAP->ATSTAR)
#define HAL_HST_CHECKR_LSB         0
#define HAL_HST_CHECKR_MSB         0
#define HAL_HST_CHECKR_MASK        0x00000001
#define HAL_HST_CHECKR_TYPE        Bool
#define HAL_HST_CHECKR             HAL_HST_CHECKR_MASK
#define HAL_HST_CHECKR_READABLE    1
#define HAL_HST_CHECKR_WRITABLE    0
#define HAL_HST_CHECKR_SIGNED      0
#define HAL_HST_CHECKR_ADR         0x10000B28

// Register ATERRR
#define HAL_HST_SENSER_REGISTER    (HAL_HSTREG_MAP->ATERRR)
#define HAL_HST_SENSER_LSB         4
#define HAL_HST_SENSER_MSB         7
#define HAL_HST_SENSER_MASK        0x000000F0
#define HAL_HST_SENSER_TYPE        UByte
#define HAL_HST_SENSER             HAL_HST_SENSER_MASK
#define HAL_HST_SENSER_READABLE    1
#define HAL_HST_SENSER_WRITABLE    0
#define HAL_HST_SENSER_SIGNED      0
#define HAL_HST_SENSER_ADR         0x10000B29

#define HAL_HST_MCRR_REGISTER    (HAL_HSTREG_MAP->ATERRR)
#define HAL_HST_MCRR_LSB         3
#define HAL_HST_MCRR_MSB         3
#define HAL_HST_MCRR_MASK        0x00000008
#define HAL_HST_MCRR_TYPE        Bool
#define HAL_HST_MCRR             HAL_HST_MCRR_MASK
#define HAL_HST_MCRR_READABLE    1
#define HAL_HST_MCRR_WRITABLE    0
#define HAL_HST_MCRR_SIGNED      0
#define HAL_HST_MCRR_ADR         0x10000B29

#define HAL_HST_ABRTR_REGISTER    (HAL_HSTREG_MAP->ATERRR)
#define HAL_HST_ABRTR_LSB         2
#define HAL_HST_ABRTR_MSB         2
#define HAL_HST_ABRTR_MASK        0x00000004
#define HAL_HST_ABRTR_TYPE        Bool
#define HAL_HST_ABRTR             HAL_HST_ABRTR_MASK
#define HAL_HST_ABRTR_READABLE    1
#define HAL_HST_ABRTR_WRITABLE    0
#define HAL_HST_ABRTR_SIGNED      0
#define HAL_HST_ABRTR_ADR         0x10000B29

#define HAL_HST_EOMR_REGISTER    (HAL_HSTREG_MAP->ATERRR)
#define HAL_HST_EOMR_LSB         1
#define HAL_HST_EOMR_MSB         1
#define HAL_HST_EOMR_MASK        0x00000002
#define HAL_HST_EOMR_TYPE        Bool
#define HAL_HST_EOMR             HAL_HST_EOMR_MASK
#define HAL_HST_EOMR_READABLE    1
#define HAL_HST_EOMR_WRITABLE    0
#define HAL_HST_EOMR_SIGNED      0
#define HAL_HST_EOMR_ADR         0x10000B29

#define HAL_HST_ILIR_REGISTER    (HAL_HSTREG_MAP->ATERRR)
#define HAL_HST_ILIR_LSB         0
#define HAL_HST_ILIR_MSB         0
#define HAL_HST_ILIR_MASK        0x00000001
#define HAL_HST_ILIR_TYPE        Bool
#define HAL_HST_ILIR             HAL_HST_ILIR_MASK
#define HAL_HST_ILIR_READABLE    1
#define HAL_HST_ILIR_WRITABLE    0
#define HAL_HST_ILIR_SIGNED      0
#define HAL_HST_ILIR_ADR         0x10000B29

#if (CHIP_REV >= 0xC0)

// Register SCNTDATA
#define HAL_HST_SCNTDATA_REGISTER    (HAL_HSTREG_MAP->SCNTDATA)
#define HAL_HST_SCNTDATA_LSB         0
#define HAL_HST_SCNTDATA_MSB         7
#define HAL_HST_SCNTDATA_MASK        0x000000FF
#define HAL_HST_SCNTDATA_TYPE        UByte
#define HAL_HST_SCNTDATA             HAL_HST_SCNTDATA_MASK
#define HAL_HST_SCNTDATA_READABLE    1
#define HAL_HST_SCNTDATA_WRITABLE    1
#define HAL_HST_SCNTDATA_SIGNED      0
#define HAL_HST_SCNTDATA_ADR         0x10000B2A

#endif // #if (CHIP_REV >= 0xC0)

// Register FWRST
#define HAL_HST_AESRST_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_AESRST_LSB         6
#define HAL_HST_AESRST_MSB         6
#define HAL_HST_AESRST_MASK        0x00000040
#define HAL_HST_AESRST_TYPE        Bool
#define HAL_HST_AESRST             HAL_HST_AESRST_MASK
#define HAL_HST_AESRST_READABLE    1
#define HAL_HST_AESRST_WRITABLE    1
#define HAL_HST_AESRST_SIGNED      0
#define HAL_HST_AESRST_ADR         0x10000B40

#define HAL_HST_PM_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_PM_LSB         5
#define HAL_HST_PM_MSB         5
#define HAL_HST_PM_MASK        0x00000020
#define HAL_HST_PM_TYPE        Bool
#define HAL_HST_PM             HAL_HST_PM_MASK
#define HAL_HST_PM_READABLE    1
#define HAL_HST_PM_WRITABLE    1
#define HAL_HST_PM_SIGNED      0
#define HAL_HST_PM_ADR         0x10000B40

#define HAL_HST_UP_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_UP_LSB         4
#define HAL_HST_UP_MSB         4
#define HAL_HST_UP_MASK        0x00000010
#define HAL_HST_UP_TYPE        Bool
#define HAL_HST_UP             HAL_HST_UP_MASK
#define HAL_HST_UP_READABLE    1
#define HAL_HST_UP_WRITABLE    1
#define HAL_HST_UP_SIGNED      0
#define HAL_HST_UP_ADR         0x10000B40

#define HAL_HST_TXFIFPTR_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_TXFIFPTR_LSB         3
#define HAL_HST_TXFIFPTR_MSB         3
#define HAL_HST_TXFIFPTR_MASK        0x00000008
#define HAL_HST_TXFIFPTR_TYPE        Bool
#define HAL_HST_TXFIFPTR             HAL_HST_TXFIFPTR_MASK
#define HAL_HST_TXFIFPTR_READABLE    1
#define HAL_HST_TXFIFPTR_WRITABLE    1
#define HAL_HST_TXFIFPTR_SIGNED      0
#define HAL_HST_TXFIFPTR_ADR         0x10000B40

#define HAL_HST_RXFIFPTR_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_RXFIFPTR_LSB         2
#define HAL_HST_RXFIFPTR_MSB         2
#define HAL_HST_RXFIFPTR_MASK        0x00000004
#define HAL_HST_RXFIFPTR_TYPE        Bool
#define HAL_HST_RXFIFPTR             HAL_HST_RXFIFPTR_MASK
#define HAL_HST_RXFIFPTR_READABLE    1
#define HAL_HST_RXFIFPTR_WRITABLE    1
#define HAL_HST_RXFIFPTR_SIGNED      0
#define HAL_HST_RXFIFPTR_ADR         0x10000B40

#define HAL_HST_RAM_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_RAM_LSB         1
#define HAL_HST_RAM_MSB         1
#define HAL_HST_RAM_MASK        0x00000002
#define HAL_HST_RAM_TYPE        Bool
#define HAL_HST_RAM             HAL_HST_RAM_MASK
#define HAL_HST_RAM_READABLE    1
#define HAL_HST_RAM_WRITABLE    1
#define HAL_HST_RAM_SIGNED      0
#define HAL_HST_RAM_ADR         0x10000B40

#define HAL_HST_HST_REGISTER    (HAL_HSTREG_MAP->FWRST)
#define HAL_HST_HST_LSB         0
#define HAL_HST_HST_MSB         0
#define HAL_HST_HST_MASK        0x00000001
#define HAL_HST_HST_TYPE        Bool
#define HAL_HST_HST             HAL_HST_HST_MASK
#define HAL_HST_HST_READABLE    1
#define HAL_HST_HST_WRITABLE    1
#define HAL_HST_HST_SIGNED      0
#define HAL_HST_HST_ADR         0x10000B40

// Register ATRG
#define HAL_HST_CSRTRG_REGISTER    (HAL_HSTREG_MAP->ATRG)
#define HAL_HST_CSRTRG_LSB         6
#define HAL_HST_CSRTRG_MSB         6
#define HAL_HST_CSRTRG_MASK        0x00000040
#define HAL_HST_CSRTRG_TYPE        Bool
#define HAL_HST_CSRTRG             HAL_HST_CSRTRG_MASK
#define HAL_HST_CSRTRG_READABLE    0
#define HAL_HST_CSRTRG_WRITABLE    1
#define HAL_HST_CSRTRG_SIGNED      0
#define HAL_HST_CSRTRG_ADR         0x10000B41

#define HAL_HST_DSCTRG_REGISTER    (HAL_HSTREG_MAP->ATRG)
#define HAL_HST_DSCTRG_LSB         5
#define HAL_HST_DSCTRG_MSB         5
#define HAL_HST_DSCTRG_MASK        0x00000020
#define HAL_HST_DSCTRG_TYPE        Bool
#define HAL_HST_DSCTRG             HAL_HST_DSCTRG_MASK
#define HAL_HST_DSCTRG_READABLE    0
#define HAL_HST_DSCTRG_WRITABLE    1
#define HAL_HST_DSCTRG_SIGNED      0
#define HAL_HST_DSCTRG_ADR         0x10000B41

#define HAL_HST_CTTRG_REGISTER    (HAL_HSTREG_MAP->ATRG)
#define HAL_HST_CTTRG_LSB         4
#define HAL_HST_CTTRG_MSB         4
#define HAL_HST_CTTRG_MASK        0x00000010
#define HAL_HST_CTTRG_TYPE        Bool
#define HAL_HST_CTTRG             HAL_HST_CTTRG_MASK
#define HAL_HST_CTTRG_READABLE    0
#define HAL_HST_CTTRG_WRITABLE    1
#define HAL_HST_CTTRG_SIGNED      0
#define HAL_HST_CTTRG_ADR         0x10000B41

#define HAL_HST_ADRTRG_REGISTER    (HAL_HSTREG_MAP->ATRG)
#define HAL_HST_ADRTRG_LSB         2
#define HAL_HST_ADRTRG_MSB         2
#define HAL_HST_ADRTRG_MASK        0x00000004
#define HAL_HST_ADRTRG_TYPE        Bool
#define HAL_HST_ADRTRG             HAL_HST_ADRTRG_MASK
#define HAL_HST_ADRTRG_READABLE    0
#define HAL_HST_ADRTRG_WRITABLE    1
#define HAL_HST_ADRTRG_SIGNED      0
#define HAL_HST_ADRTRG_ADR         0x10000B41

#define HAL_HST_DRQTRG_REGISTER    (HAL_HSTREG_MAP->ATRG)
#define HAL_HST_DRQTRG_LSB         1
#define HAL_HST_DRQTRG_MSB         1
#define HAL_HST_DRQTRG_MASK        0x00000002
#define HAL_HST_DRQTRG_TYPE        Bool
#define HAL_HST_DRQTRG             HAL_HST_DRQTRG_MASK
#define HAL_HST_DRQTRG_READABLE    0
#define HAL_HST_DRQTRG_WRITABLE    1
#define HAL_HST_DRQTRG_SIGNED      0
#define HAL_HST_DRQTRG_ADR         0x10000B41

#define HAL_HST_SCTRG_REGISTER    (HAL_HSTREG_MAP->ATRG)
#define HAL_HST_SCTRG_LSB         0
#define HAL_HST_SCTRG_MSB         0
#define HAL_HST_SCTRG_MASK        0x00000001
#define HAL_HST_SCTRG_TYPE        Bool
#define HAL_HST_SCTRG             HAL_HST_SCTRG_MASK
#define HAL_HST_SCTRG_READABLE    0
#define HAL_HST_SCTRG_WRITABLE    1
#define HAL_HST_SCTRG_SIGNED      0
#define HAL_HST_SCTRG_ADR         0x10000B41

// Register DTTRG
#define HAL_HST_DTTRG_REGISTER    (HAL_HSTREG_MAP->DTTRG)
#define HAL_HST_DTTRG_LSB         0
#define HAL_HST_DTTRG_MSB         7
#define HAL_HST_DTTRG_MASK        0x000000FF
#define HAL_HST_DTTRG_TYPE        UByte
#define HAL_HST_DTTRG             HAL_HST_DTTRG_MASK
#define HAL_HST_DTTRG_READABLE    0
#define HAL_HST_DTTRG_WRITABLE    1
#define HAL_HST_DTTRG_SIGNED      0
#define HAL_HST_DTTRG_ADR         0x10000B42

// Register HDDIR
#define HAL_HST_CTRSSHOT_REGISTER    (HAL_HSTREG_MAP->HDDIR)
#define HAL_HST_CTRSSHOT_LSB         7
#define HAL_HST_CTRSSHOT_MSB         7
#define HAL_HST_CTRSSHOT_MASK        0x00000080
#define HAL_HST_CTRSSHOT_TYPE        Bool
#define HAL_HST_CTRSSHOT             HAL_HST_CTRSSHOT_MASK
#define HAL_HST_CTRSSHOT_READABLE    1
#define HAL_HST_CTRSSHOT_WRITABLE    1
#define HAL_HST_CTRSSHOT_SIGNED      0
#define HAL_HST_CTRSSHOT_ADR         0x10000B43

#define HAL_HST_BUFPGSEL_REGISTER    (HAL_HSTREG_MAP->HDDIR)
#define HAL_HST_BUFPGSEL_LSB         6
#define HAL_HST_BUFPGSEL_MSB         6
#define HAL_HST_BUFPGSEL_MASK        0x00000040
#define HAL_HST_BUFPGSEL_TYPE        Bool
#define HAL_HST_BUFPGSEL             HAL_HST_BUFPGSEL_MASK
#define HAL_HST_BUFPGSEL_READABLE    1
#define HAL_HST_BUFPGSEL_WRITABLE    1
#define HAL_HST_BUFPGSEL_SIGNED      0
#define HAL_HST_BUFPGSEL_ADR         0x10000B43

#define HAL_HST_HMLY_REGISTER    (HAL_HSTREG_MAP->HDDIR)
#define HAL_HST_HMLY_LSB         5
#define HAL_HST_HMLY_MSB         5
#define HAL_HST_HMLY_MASK        0x00000020
#define HAL_HST_HMLY_TYPE        Bool
#define HAL_HST_HMLY             HAL_HST_HMLY_MASK
#define HAL_HST_HMLY_READABLE    1
#define HAL_HST_HMLY_WRITABLE    1
#define HAL_HST_HMLY_SIGNED      0
#define HAL_HST_HMLY_ADR         0x10000B43

#define HAL_HST_DMAMODE_REGISTER    (HAL_HSTREG_MAP->HDDIR)
#define HAL_HST_DMAMODE_LSB         2
#define HAL_HST_DMAMODE_MSB         3
#define HAL_HST_DMAMODE_MASK        0x0000000C
#define HAL_HST_DMAMODE_TYPE        halHST_DMAMODE_t
#define HAL_HST_DMAMODE             HAL_HST_DMAMODE_MASK
#define HAL_HST_DMAMODE_READABLE    1
#define HAL_HST_DMAMODE_WRITABLE    1
#define HAL_HST_DMAMODE_SIGNED      0
#define HAL_HST_DMAMODE_ADR         0x10000B43

#define HAL_HST_HXDIR_REGISTER    (HAL_HSTREG_MAP->HDDIR)
#define HAL_HST_HXDIR_LSB         1
#define HAL_HST_HXDIR_MSB         1
#define HAL_HST_HXDIR_MASK        0x00000002
#define HAL_HST_HXDIR_TYPE        halHST_HstTxDir_t
#define HAL_HST_HXDIR             HAL_HST_HXDIR_MASK
#define HAL_HST_HXDIR_READABLE    1
#define HAL_HST_HXDIR_WRITABLE    1
#define HAL_HST_HXDIR_SIGNED      0
#define HAL_HST_HXDIR_ADR         0x10000B43

#define HAL_HST_HBODIS_REGISTER    (HAL_HSTREG_MAP->HDDIR)
#define HAL_HST_HBODIS_LSB         0
#define HAL_HST_HBODIS_MSB         0
#define HAL_HST_HBODIS_MASK        0x00000001
#define HAL_HST_HBODIS_TYPE        halHST_AddressTyp_t
#define HAL_HST_HBODIS             HAL_HST_HBODIS_MASK
#define HAL_HST_HBODIS_READABLE    1
#define HAL_HST_HBODIS_WRITABLE    1
#define HAL_HST_HBODIS_SIGNED      0
#define HAL_HST_HBODIS_ADR         0x10000B43

// Register HPGCNTA
#define HAL_HST_XPRLDEN_REGISTER    (HAL_HSTREG_MAP->HPGCNTA)
#define HAL_HST_XPRLDEN_LSB         7
#define HAL_HST_XPRLDEN_MSB         7
#define HAL_HST_XPRLDEN_MASK        0x00000080
#define HAL_HST_XPRLDEN_TYPE        Bool
#define HAL_HST_XPRLDEN             HAL_HST_XPRLDEN_MASK
#define HAL_HST_XPRLDEN_READABLE    1
#define HAL_HST_XPRLDEN_WRITABLE    1
#define HAL_HST_XPRLDEN_SIGNED      0
#define HAL_HST_XPRLDEN_ADR         0x10000B46

#define HAL_HST_XFRAVPSE_REGISTER    (HAL_HSTREG_MAP->HPGCNTA)
#define HAL_HST_XFRAVPSE_LSB         3
#define HAL_HST_XFRAVPSE_MSB         3
#define HAL_HST_XFRAVPSE_MASK        0x00000008
#define HAL_HST_XFRAVPSE_TYPE        Bool
#define HAL_HST_XFRAVPSE             HAL_HST_XFRAVPSE_MASK
#define HAL_HST_XFRAVPSE_READABLE    1
#define HAL_HST_XFRAVPSE_WRITABLE    0
#define HAL_HST_XFRAVPSE_SIGNED      0
#define HAL_HST_XFRAVPSE_ADR         0x10000B46

#define HAL_HST_PKTAVPSE_REGISTER    (HAL_HSTREG_MAP->HPGCNTA)
#define HAL_HST_PKTAVPSE_LSB         2
#define HAL_HST_PKTAVPSE_MSB         2
#define HAL_HST_PKTAVPSE_MASK        0x00000004
#define HAL_HST_PKTAVPSE_TYPE        Bool
#define HAL_HST_PKTAVPSE             HAL_HST_PKTAVPSE_MASK
#define HAL_HST_PKTAVPSE_READABLE    1
#define HAL_HST_PKTAVPSE_WRITABLE    0
#define HAL_HST_PKTAVPSE_SIGNED      0
#define HAL_HST_PKTAVPSE_ADR         0x10000B46

// Register IFCTRL
#define HAL_HST_DOUTEN_REGISTER    (HAL_HSTREG_MAP->IFCTRL)
#define HAL_HST_DOUTEN_LSB         1
#define HAL_HST_DOUTEN_MSB         1
#define HAL_HST_DOUTEN_MASK        0x00000002
#define HAL_HST_DOUTEN_TYPE        Bool
#define HAL_HST_DOUTEN             HAL_HST_DOUTEN_MASK
#define HAL_HST_DOUTEN_READABLE    1
#define HAL_HST_DOUTEN_WRITABLE    1
#define HAL_HST_DOUTEN_SIGNED      0
#define HAL_HST_DOUTEN_ADR         0x10000B47

#define HAL_HST_HINTSRC_REGISTER    (HAL_HSTREG_MAP->IFCTRL)
#define HAL_HST_HINTSRC_LSB         0
#define HAL_HST_HINTSRC_MSB         0
#define HAL_HST_HINTSRC_MASK        0x00000001
#define HAL_HST_HINTSRC_TYPE        Bool
#define HAL_HST_HINTSRC             HAL_HST_HINTSRC_MASK
#define HAL_HST_HINTSRC_READABLE    1
#define HAL_HST_HINTSRC_WRITABLE    1
#define HAL_HST_HINTSRC_SIGNED      0
#define HAL_HST_HINTSRC_ADR         0x10000B47

// Register RXFIFOC
#define HAL_HST_RBSIZE_REGISTER    (HAL_HSTREG_MAP->RXFIFOC)
#define HAL_HST_RBSIZE_LSB         3
#define HAL_HST_RBSIZE_MSB         5
#define HAL_HST_RBSIZE_MASK        0x00000038
#define HAL_HST_RBSIZE_TYPE        halHST_RAMBurstSize_t
#define HAL_HST_RBSIZE             HAL_HST_RBSIZE_MASK
#define HAL_HST_RBSIZE_READABLE    1
#define HAL_HST_RBSIZE_WRITABLE    1
#define HAL_HST_RBSIZE_SIGNED      0
#define HAL_HST_RBSIZE_ADR         0x10000B4A

#if (CHIP_REV >= 0xC0)

// Registers PMIDLEPL/PMIDLEPH
#define HAL_HST_PMIDLEP_REGISTER    (HAL_HSTREG_MAP->PMIDLEPL)
#define HAL_HST_PMIDLEP_LSB         0
#define HAL_HST_PMIDLEP_MSB         11
#define HAL_HST_PMIDLEP_MASK        0x00000FFF
#define HAL_HST_PMIDLEP_TYPE        UInt16
#define HAL_HST_PMIDLEP             HAL_HST_PMIDLEP_MASK
#define HAL_HST_PMIDLEP_READABLE    1
#define HAL_HST_PMIDLEP_WRITABLE    1
#define HAL_HST_PMIDLEP_SIGNED      0
#define HAL_HST_PMIDLEP_ADR         0x10000B4C

#define HAL_HST_PMAUTO_EN_REGISTER    (HAL_HSTREG_MAP->PMIDLEPL)
#define HAL_HST_PMAUTO_EN_LSB         15
#define HAL_HST_PMAUTO_EN_MSB         15
#define HAL_HST_PMAUTO_EN_MASK        0x00008000
#define HAL_HST_PMAUTO_EN_TYPE        Bool
#define HAL_HST_PMAUTO_EN             HAL_HST_PMAUTO_EN_MASK
#define HAL_HST_PMAUTO_EN_READABLE    1
#define HAL_HST_PMAUTO_EN_WRITABLE    1
#define HAL_HST_PMAUTO_EN_SIGNED      0
#define HAL_HST_PMAUTO_EN_ADR         0x10000B4C

#define HAL_HST_FIS34EN_WKUP_REGISTER    (HAL_HSTREG_MAP->PMIDLEPL)
#define HAL_HST_FIS34EN_WKUP_LSB         14
#define HAL_HST_FIS34EN_WKUP_MSB         14
#define HAL_HST_FIS34EN_WKUP_MASK        0x00004000
#define HAL_HST_FIS34EN_WKUP_TYPE        Bool
#define HAL_HST_FIS34EN_WKUP             HAL_HST_FIS34EN_WKUP_MASK
#define HAL_HST_FIS34EN_WKUP_READABLE    1
#define HAL_HST_FIS34EN_WKUP_WRITABLE    1
#define HAL_HST_FIS34EN_WKUP_SIGNED      0
#define HAL_HST_FIS34EN_WKUP_ADR         0x10000B4C

#define HAL_HST_DIPM_EN_REGISTER    (HAL_HSTREG_MAP->PMIDLEPL)
#define HAL_HST_DIPM_EN_LSB         13
#define HAL_HST_DIPM_EN_MSB         13
#define HAL_HST_DIPM_EN_MASK        0x00002000
#define HAL_HST_DIPM_EN_TYPE        Bool
#define HAL_HST_DIPM_EN             HAL_HST_DIPM_EN_MASK
#define HAL_HST_DIPM_EN_READABLE    1
#define HAL_HST_DIPM_EN_WRITABLE    1
#define HAL_HST_DIPM_EN_SIGNED      0
#define HAL_HST_DIPM_EN_ADR         0x10000B4C

#define HAL_HST_DWCLK_OFF_REGISTER    (HAL_HSTREG_MAP->PMIDLEPL)
#define HAL_HST_DWCLK_OFF_LSB         12
#define HAL_HST_DWCLK_OFF_MSB         12
#define HAL_HST_DWCLK_OFF_MASK        0x00001000
#define HAL_HST_DWCLK_OFF_TYPE        Bool
#define HAL_HST_DWCLK_OFF             HAL_HST_DWCLK_OFF_MASK
#define HAL_HST_DWCLK_OFF_READABLE    1
#define HAL_HST_DWCLK_OFF_WRITABLE    1
#define HAL_HST_DWCLK_OFF_SIGNED      0
#define HAL_HST_DWCLK_OFF_ADR         0x10000B4C

// Registers PMIDLESL/PMIDLESH
#define HAL_HST_PMIDLES_REGISTER    (HAL_HSTREG_MAP->PMIDLESL)
#define HAL_HST_PMIDLES_LSB         0
#define HAL_HST_PMIDLES_MSB         11
#define HAL_HST_PMIDLES_MASK        0x00000FFF
#define HAL_HST_PMIDLES_TYPE        UInt16
#define HAL_HST_PMIDLES             HAL_HST_PMIDLES_MASK
#define HAL_HST_PMIDLES_READABLE    1
#define HAL_HST_PMIDLES_WRITABLE    1
#define HAL_HST_PMIDLES_SIGNED      0
#define HAL_HST_PMIDLES_ADR         0x10000B4E

#endif // #if (CHIP_REV >= 0xC0)

// Registers HSTPGCL/HSTPGCH
#define HAL_HST_HSTPGC_REGISTER    (HAL_HSTREG_MAP->HSTPGCL)
#define HAL_HST_HSTPGC_LSB         0
#define HAL_HST_HSTPGC_MSB         15
#define HAL_HST_HSTPGC_MASK        0x0000FFFF
#define HAL_HST_HSTPGC_TYPE        UInt16
#define HAL_HST_HSTPGC             HAL_HST_HSTPGC_MASK
#define HAL_HST_HSTPGC_READABLE    1
#define HAL_HST_HSTPGC_WRITABLE    1
#define HAL_HST_HSTPGC_SIGNED      0
#define HAL_HST_HSTPGC_ADR         0x10000B50

// Registers HAVPGCL/HAVPGCH
#define HAL_HST_HAVPGC_REGISTER    (HAL_HSTREG_MAP->HAVPGCL)
#define HAL_HST_HAVPGC_LSB         0
#define HAL_HST_HAVPGC_MSB         15
#define HAL_HST_HAVPGC_MASK        0x0000FFFF
#define HAL_HST_HAVPGC_TYPE        UInt16
#define HAL_HST_HAVPGC             HAL_HST_HAVPGC_MASK
#define HAL_HST_HAVPGC_READABLE    1
#define HAL_HST_HAVPGC_WRITABLE    1
#define HAL_HST_HAVPGC_SIGNED      0
#define HAL_HST_HAVPGC_ADR         0x10000B52

// Registers HSTWDCL/HSTWDCH

#if (CHIP_REV <= 0xB3)

#define HAL_HST_HSTWDC_REGISTER    (HAL_HSTREG_MAP->HSTWDCL)
#define HAL_HST_HSTWDC_LSB         0
#define HAL_HST_HSTWDC_MSB         11
#define HAL_HST_HSTWDC_MASK        0x00000FFF
#define HAL_HST_HSTWDC_TYPE        UInt16
#define HAL_HST_HSTWDC             HAL_HST_HSTWDC_MASK
#define HAL_HST_HSTWDC_READABLE    1
#define HAL_HST_HSTWDC_WRITABLE    1
#define HAL_HST_HSTWDC_SIGNED      0
#define HAL_HST_HSTWDC_ADR         0x10000B54

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_HST_HSTWDC_REGISTER    (HAL_HSTREG_MAP->HSTWDCL)
#define HAL_HST_HSTWDC_LSB         0
#define HAL_HST_HSTWDC_MSB         13
#define HAL_HST_HSTWDC_MASK        0x00003FFF
#define HAL_HST_HSTWDC_TYPE        UInt16
#define HAL_HST_HSTWDC             HAL_HST_HSTWDC_MASK
#define HAL_HST_HSTWDC_READABLE    1
#define HAL_HST_HSTWDC_WRITABLE    1
#define HAL_HST_HSTWDC_SIGNED      0
#define HAL_HST_HSTWDC_ADR         0x10000B54

#endif // #if (CHIP_REV >= 0xC0)

// Register HSTSUBC
#define HAL_HST_HSTSUBC_REGISTER    (HAL_HSTREG_MAP->HSTSUBC)
#define HAL_HST_HSTSUBC_LSB         0
#define HAL_HST_HSTSUBC_MSB         7
#define HAL_HST_HSTSUBC_MASK        0x000000FF
#define HAL_HST_HSTSUBC_TYPE        UByte
#define HAL_HST_HSTSUBC             HAL_HST_HSTSUBC_MASK
#define HAL_HST_HSTSUBC_READABLE    1
#define HAL_HST_HSTSUBC_WRITABLE    1
#define HAL_HST_HSTSUBC_SIGNED      0
#define HAL_HST_HSTSUBC_ADR         0x10000B56

// Register HSTAUXW
#define HAL_HST_HSTAUXW_REGISTER    (HAL_HSTREG_MAP->HSTAUXW)
#define HAL_HST_HSTAUXW_LSB         0
#define HAL_HST_HSTAUXW_MSB         7
#define HAL_HST_HSTAUXW_MASK        0x000000FF
#define HAL_HST_HSTAUXW_TYPE        UByte
#define HAL_HST_HSTAUXW             HAL_HST_HSTAUXW_MASK
#define HAL_HST_HSTAUXW_READABLE    1
#define HAL_HST_HSTAUXW_WRITABLE    1
#define HAL_HST_HSTAUXW_SIGNED      0
#define HAL_HST_HSTAUXW_ADR         0x10000B57

// Registers HSTBYTACL/HSTBYTACH/HSTBYTACU/SATASPR1
#define HAL_HST_HSTBYTAC_REGISTER    (HAL_HSTREG_MAP->HSTBYTACL)
#define HAL_HST_HSTBYTAC_LSB         0
#define HAL_HST_HSTBYTAC_MSB         23
#define HAL_HST_HSTBYTAC_MASK        0x00FFFFFF
#define HAL_HST_HSTBYTAC_TYPE        UInt32
#define HAL_HST_HSTBYTAC             HAL_HST_HSTBYTAC_MASK
#define HAL_HST_HSTBYTAC_READABLE    1
#define HAL_HST_HSTBYTAC_WRITABLE    1
#define HAL_HST_HSTBYTAC_SIGNED      0
#define HAL_HST_HSTBYTAC_ADR         0x10000B58

#if (CHIP_REV >= 0xC0)

// Register SATASPR2
// Register RESWDCL
#define HAL_HST_RESWDCL_REGISTER    (HAL_HSTREG_MAP->RESWDCL)
#define HAL_HST_RESWDCL_LSB         0
#define HAL_HST_RESWDCL_MSB         7
#define HAL_HST_RESWDCL_MASK        0x000000FF
#define HAL_HST_RESWDCL_TYPE        UByte
#define HAL_HST_RESWDCL             HAL_HST_RESWDCL_MASK
#define HAL_HST_RESWDCL_READABLE    1
#define HAL_HST_RESWDCL_WRITABLE    1
#define HAL_HST_RESWDCL_SIGNED      0
#define HAL_HST_RESWDCL_ADR         0x10000B5D

// Register RESWDCH
#define HAL_HST_RESWDCH_REGISTER    (HAL_HSTREG_MAP->RESWDCH)
#define HAL_HST_RESWDCH_LSB         0
#define HAL_HST_RESWDCH_MSB         5
#define HAL_HST_RESWDCH_MASK        0x0000003F
#define HAL_HST_RESWDCH_TYPE        UByte
#define HAL_HST_RESWDCH             HAL_HST_RESWDCH_MASK
#define HAL_HST_RESWDCH_READABLE    1
#define HAL_HST_RESWDCH_WRITABLE    1
#define HAL_HST_RESWDCH_SIGNED      0
#define HAL_HST_RESWDCH_ADR         0x10000B5E

#endif // #if (CHIP_REV >= 0xC0)

// Registers HSTBUFPGL/HSTBUFPGH
#define HAL_HST_HSTBUFPG_REGISTER    (HAL_HSTREG_MAP->HSTBUFPGL)
#define HAL_HST_HSTBUFPG_LSB         0
#define HAL_HST_HSTBUFPG_MSB         12
#define HAL_HST_HSTBUFPG_MASK        0x00001FFF
#define HAL_HST_HSTBUFPG_TYPE        UInt16
#define HAL_HST_HSTBUFPG             HAL_HST_HSTBUFPG_MASK
#define HAL_HST_HSTBUFPG_READABLE    1
#define HAL_HST_HSTBUFPG_WRITABLE    1
#define HAL_HST_HSTBUFPG_SIGNED      0
#define HAL_HST_HSTBUFPG_ADR         0x10000B60

#define HAL_HST_HSTPGMSB_REGISTER    (HAL_HSTREG_MAP->HSTBUFPGL)
#define HAL_HST_HSTPGMSB_LSB         13
#define HAL_HST_HSTPGMSB_MSB         13
#define HAL_HST_HSTPGMSB_MASK        0x00002000
#define HAL_HST_HSTPGMSB_TYPE        Bool
#define HAL_HST_HSTPGMSB             HAL_HST_HSTPGMSB_MASK
#define HAL_HST_HSTPGMSB_READABLE    1
#define HAL_HST_HSTPGMSB_WRITABLE    1
#define HAL_HST_HSTPGMSB_SIGNED      0
#define HAL_HST_HSTPGMSB_ADR         0x10000B60

// Registers HSTBUFSPGL/HSTBUFSPGH
#define HAL_HST_HSTBUFSPG_REGISTER    (HAL_HSTREG_MAP->HSTBUFSPGL)
#define HAL_HST_HSTBUFSPG_LSB         0
#define HAL_HST_HSTBUFSPG_MSB         12
#define HAL_HST_HSTBUFSPG_MASK        0x00001FFF
#define HAL_HST_HSTBUFSPG_TYPE        UInt16
#define HAL_HST_HSTBUFSPG             HAL_HST_HSTBUFSPG_MASK
#define HAL_HST_HSTBUFSPG_READABLE    1
#define HAL_HST_HSTBUFSPG_WRITABLE    1
#define HAL_HST_HSTBUFSPG_SIGNED      0
#define HAL_HST_HSTBUFSPG_ADR         0x10000B62

// Registers HSTBUFEPGL/HSTBUFEPGH
#define HAL_HST_HSTBUFEPG_REGISTER    (HAL_HSTREG_MAP->HSTBUFEPGL)
#define HAL_HST_HSTBUFEPG_LSB         0
#define HAL_HST_HSTBUFEPG_MSB         12
#define HAL_HST_HSTBUFEPG_MASK        0x00001FFF
#define HAL_HST_HSTBUFEPG_TYPE        UInt16
#define HAL_HST_HSTBUFEPG             HAL_HST_HSTBUFEPG_MASK
#define HAL_HST_HSTBUFEPG_READABLE    1
#define HAL_HST_HSTBUFEPG_WRITABLE    1
#define HAL_HST_HSTBUFEPG_SIGNED      0
#define HAL_HST_HSTBUFEPG_ADR         0x10000B64

// Registers BUFPGCMPL/BUFPGCMPH
#define HAL_HST_BUFPGCMP_REGISTER    (HAL_HSTREG_MAP->BUFPGCMPL)
#define HAL_HST_BUFPGCMP_LSB         0
#define HAL_HST_BUFPGCMP_MSB         12
#define HAL_HST_BUFPGCMP_MASK        0x00001FFF
#define HAL_HST_BUFPGCMP_TYPE        UInt16
#define HAL_HST_BUFPGCMP             HAL_HST_BUFPGCMP_MASK
#define HAL_HST_BUFPGCMP_READABLE    1
#define HAL_HST_BUFPGCMP_WRITABLE    1
#define HAL_HST_BUFPGCMP_SIGNED      0
#define HAL_HST_BUFPGCMP_ADR         0x10000B66

#define HAL_HST_BUFPGMSB_REGISTER    (HAL_HSTREG_MAP->BUFPGCMPL)
#define HAL_HST_BUFPGMSB_LSB         13
#define HAL_HST_BUFPGMSB_MSB         13
#define HAL_HST_BUFPGMSB_MASK        0x00002000
#define HAL_HST_BUFPGMSB_TYPE        Bool
#define HAL_HST_BUFPGMSB             HAL_HST_BUFPGMSB_MASK
#define HAL_HST_BUFPGMSB_READABLE    1
#define HAL_HST_BUFPGMSB_WRITABLE    1
#define HAL_HST_BUFPGMSB_SIGNED      0
#define HAL_HST_BUFPGMSB_ADR         0x10000B66

// Registers HSTSUBAL/HSTSUBAH
#define HAL_HST_HSTSUBA_REGISTER    (HAL_HSTREG_MAP->HSTSUBAL)
#define HAL_HST_HSTSUBA_LSB         0
#define HAL_HST_HSTSUBA_MSB         9
#define HAL_HST_HSTSUBA_MASK        0x000003FF
#define HAL_HST_HSTSUBA_TYPE        UInt16
#define HAL_HST_HSTSUBA             HAL_HST_HSTSUBA_MASK
#define HAL_HST_HSTSUBA_READABLE    1
#define HAL_HST_HSTSUBA_WRITABLE    1
#define HAL_HST_HSTSUBA_SIGNED      0
#define HAL_HST_HSTSUBA_ADR         0x10000B68

// Registers HSTAUXAL/HSTAUXAH
#define HAL_HST_HSTAUXA_REGISTER    (HAL_HSTREG_MAP->HSTAUXAL)
#define HAL_HST_HSTAUXA_LSB         0
#define HAL_HST_HSTAUXA_MSB         9
#define HAL_HST_HSTAUXA_MASK        0x000003FF
#define HAL_HST_HSTAUXA_TYPE        UInt16
#define HAL_HST_HSTAUXA             HAL_HST_HSTAUXA_MASK
#define HAL_HST_HSTAUXA_READABLE    1
#define HAL_HST_HSTAUXA_WRITABLE    1
#define HAL_HST_HSTAUXA_SIGNED      0
#define HAL_HST_HSTAUXA_ADR         0x10000B6A

// Registers HSTPGCCL/HSTPGCCH
#define HAL_HST_HSTPGCC_REGISTER    (HAL_HSTREG_MAP->HSTPGCCL)
#define HAL_HST_HSTPGCC_LSB         0
#define HAL_HST_HSTPGCC_MSB         15
#define HAL_HST_HSTPGCC_MASK        0x0000FFFF
#define HAL_HST_HSTPGCC_TYPE        UInt16
#define HAL_HST_HSTPGCC             HAL_HST_HSTPGCC_MASK
#define HAL_HST_HSTPGCC_READABLE    1
#define HAL_HST_HSTPGCC_WRITABLE    0
#define HAL_HST_HSTPGCC_SIGNED      0
#define HAL_HST_HSTPGCC_ADR         0x10000B70

// Registers RAMPGCCL/RAMPGCCH
#define HAL_HST_RAMPGCC_REGISTER    (HAL_HSTREG_MAP->RAMPGCCL)
#define HAL_HST_RAMPGCC_LSB         0
#define HAL_HST_RAMPGCC_MSB         15
#define HAL_HST_RAMPGCC_MASK        0x0000FFFF
#define HAL_HST_RAMPGCC_TYPE        UInt16
#define HAL_HST_RAMPGCC             HAL_HST_RAMPGCC_MASK
#define HAL_HST_RAMPGCC_READABLE    1
#define HAL_HST_RAMPGCC_WRITABLE    0
#define HAL_HST_RAMPGCC_SIGNED      0
#define HAL_HST_RAMPGCC_ADR         0x10000B72

// Registers HSTWDCCL/HSTWDCCH

#if (CHIP_REV <= 0xB3)

#define HAL_HST_HSTWDCC_REGISTER    (HAL_HSTREG_MAP->HSTWDCCL)
#define HAL_HST_HSTWDCC_LSB         0
#define HAL_HST_HSTWDCC_MSB         11
#define HAL_HST_HSTWDCC_MASK        0x00000FFF
#define HAL_HST_HSTWDCC_TYPE        UInt16
#define HAL_HST_HSTWDCC             HAL_HST_HSTWDCC_MASK
#define HAL_HST_HSTWDCC_READABLE    1
#define HAL_HST_HSTWDCC_WRITABLE    0
#define HAL_HST_HSTWDCC_SIGNED      0
#define HAL_HST_HSTWDCC_ADR         0x10000B74

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_HST_HSTWDCC_REGISTER    (HAL_HSTREG_MAP->HSTWDCCL)
#define HAL_HST_HSTWDCC_LSB         0
#define HAL_HST_HSTWDCC_MSB         13
#define HAL_HST_HSTWDCC_MASK        0x00003FFF
#define HAL_HST_HSTWDCC_TYPE        UInt16
#define HAL_HST_HSTWDCC             HAL_HST_HSTWDCC_MASK
#define HAL_HST_HSTWDCC_READABLE    1
#define HAL_HST_HSTWDCC_WRITABLE    0
#define HAL_HST_HSTWDCC_SIGNED      0
#define HAL_HST_HSTWDCC_ADR         0x10000B74

#endif // #if (CHIP_REV >= 0xC0)

// Registers RAMWDCCL/RAMWDCCH

#if (CHIP_REV <= 0xB3)

#define HAL_HST_RAMWDCC_REGISTER    (HAL_HSTREG_MAP->RAMWDCCL)
#define HAL_HST_RAMWDCC_LSB         0
#define HAL_HST_RAMWDCC_MSB         11
#define HAL_HST_RAMWDCC_MASK        0x00000FFF
#define HAL_HST_RAMWDCC_TYPE        UInt16
#define HAL_HST_RAMWDCC             HAL_HST_RAMWDCC_MASK
#define HAL_HST_RAMWDCC_READABLE    1
#define HAL_HST_RAMWDCC_WRITABLE    0
#define HAL_HST_RAMWDCC_SIGNED      0
#define HAL_HST_RAMWDCC_ADR         0x10000B76

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_HST_RAMWDCC_REGISTER    (HAL_HSTREG_MAP->RAMWDCCL)
#define HAL_HST_RAMWDCC_LSB         0
#define HAL_HST_RAMWDCC_MSB         13
#define HAL_HST_RAMWDCC_MASK        0x00003FFF
#define HAL_HST_RAMWDCC_TYPE        UInt16
#define HAL_HST_RAMWDCC             HAL_HST_RAMWDCC_MASK
#define HAL_HST_RAMWDCC_READABLE    1
#define HAL_HST_RAMWDCC_WRITABLE    0
#define HAL_HST_RAMWDCC_SIGNED      0
#define HAL_HST_RAMWDCC_ADR         0x10000B76

#endif // #if (CHIP_REV >= 0xC0)

// Registers HSTBACCL/HSTBACCH/HSTBACCU
#define HAL_HST_HSTBACC_REGISTER    (HAL_HSTREG_MAP->HSTBACCL)
#define HAL_HST_HSTBACC_LSB         0
#define HAL_HST_HSTBACC_MSB         23
#define HAL_HST_HSTBACC_MASK        0x00FFFFFF
#define HAL_HST_HSTBACC_TYPE        UInt32
#define HAL_HST_HSTBACC             HAL_HST_HSTBACC_MASK
#define HAL_HST_HSTBACC_READABLE    1
#define HAL_HST_HSTBACC_WRITABLE    0
#define HAL_HST_HSTBACC_SIGNED      0
#define HAL_HST_HSTBACC_ADR         0x10000B78

// Registers HSTBUFCCL/HSTBUFCCH
#define HAL_HST_HSTBUFCC_REGISTER    (HAL_HSTREG_MAP->HSTBUFCCL)
#define HAL_HST_HSTBUFCC_LSB         0
#define HAL_HST_HSTBUFCC_MSB         12
#define HAL_HST_HSTBUFCC_MASK        0x00001FFF
#define HAL_HST_HSTBUFCC_TYPE        UInt16
#define HAL_HST_HSTBUFCC             HAL_HST_HSTBUFCC_MASK
#define HAL_HST_HSTBUFCC_READABLE    1
#define HAL_HST_HSTBUFCC_WRITABLE    0
#define HAL_HST_HSTBUFCC_SIGNED      0
#define HAL_HST_HSTBUFCC_ADR         0x10000B7C

#define HAL_HST_HSTPGMSBC_REGISTER    (HAL_HSTREG_MAP->HSTBUFCCL)
#define HAL_HST_HSTPGMSBC_LSB         13
#define HAL_HST_HSTPGMSBC_MSB         13
#define HAL_HST_HSTPGMSBC_MASK        0x00002000
#define HAL_HST_HSTPGMSBC_TYPE        Bool
#define HAL_HST_HSTPGMSBC             HAL_HST_HSTPGMSBC_MASK
#define HAL_HST_HSTPGMSBC_READABLE    1
#define HAL_HST_HSTPGMSBC_WRITABLE    0
#define HAL_HST_HSTPGMSBC_SIGNED      0
#define HAL_HST_HSTPGMSBC_ADR         0x10000B7C

#if (CHIP_REV <= 0xB3)

// Register SATASPR1
#define HAL_HST_ODDWRDSUP_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_ODDWRDSUP_LSB         0
#define HAL_HST_ODDWRDSUP_MSB         0
#define HAL_HST_ODDWRDSUP_MASK        0x00000001
#define HAL_HST_ODDWRDSUP_TYPE        Bool
#define HAL_HST_ODDWRDSUP             HAL_HST_ODDWRDSUP_MASK
#define HAL_HST_ODDWRDSUP_READABLE    1
#define HAL_HST_ODDWRDSUP_WRITABLE    1
#define HAL_HST_ODDWRDSUP_SIGNED      0
#define HAL_HST_ODDWRDSUP_ADR         0x10000B7E

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

// Register MISCTL1
#define HAL_HST_ODDPAGEWD_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_ODDPAGEWD_LSB         7
#define HAL_HST_ODDPAGEWD_MSB         7
#define HAL_HST_ODDPAGEWD_MASK        0x00000080
#define HAL_HST_ODDPAGEWD_TYPE        halHST_ODDPAGEWD_t
#define HAL_HST_ODDPAGEWD             HAL_HST_ODDPAGEWD_MASK
#define HAL_HST_ODDPAGEWD_READABLE    1
#define HAL_HST_ODDPAGEWD_WRITABLE    1
#define HAL_HST_ODDPAGEWD_SIGNED      0
#define HAL_HST_ODDPAGEWD_ADR         0x10000B7E

#define HAL_HST_LSTDTFISEL_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_LSTDTFISEL_LSB         6
#define HAL_HST_LSTDTFISEL_MSB         6
#define HAL_HST_LSTDTFISEL_MASK        0x00000040
#define HAL_HST_LSTDTFISEL_TYPE        halHST_LSTDTFISEL_t
#define HAL_HST_LSTDTFISEL             HAL_HST_LSTDTFISEL_MASK
#define HAL_HST_LSTDTFISEL_READABLE    1
#define HAL_HST_LSTDTFISEL_WRITABLE    1
#define HAL_HST_LSTDTFISEL_SIGNED      0
#define HAL_HST_LSTDTFISEL_ADR         0x10000B7E

#define HAL_HST_CMDIP_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_CMDIP_LSB         5
#define HAL_HST_CMDIP_MSB         5
#define HAL_HST_CMDIP_MASK        0x00000020
#define HAL_HST_CMDIP_TYPE        halHST_CMDIP_t
#define HAL_HST_CMDIP             HAL_HST_CMDIP_MASK
#define HAL_HST_CMDIP_READABLE    1
#define HAL_HST_CMDIP_WRITABLE    1
#define HAL_HST_CMDIP_SIGNED      0
#define HAL_HST_CMDIP_ADR         0x10000B7E

#define HAL_HST_NDTFISEL_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_NDTFISEL_LSB         4
#define HAL_HST_NDTFISEL_MSB         4
#define HAL_HST_NDTFISEL_MASK        0x00000010
#define HAL_HST_NDTFISEL_TYPE        halHST_NDTFISEL_t
#define HAL_HST_NDTFISEL             HAL_HST_NDTFISEL_MASK
#define HAL_HST_NDTFISEL_READABLE    1
#define HAL_HST_NDTFISEL_WRITABLE    1
#define HAL_HST_NDTFISEL_SIGNED      0
#define HAL_HST_NDTFISEL_ADR         0x10000B7E

#define HAL_HST_DFRMSIZE4K_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_DFRMSIZE4K_LSB         3
#define HAL_HST_DFRMSIZE4K_MSB         3
#define HAL_HST_DFRMSIZE4K_MASK        0x00000008
#define HAL_HST_DFRMSIZE4K_TYPE        halHST_DFRMSIZE4K_t
#define HAL_HST_DFRMSIZE4K             HAL_HST_DFRMSIZE4K_MASK
#define HAL_HST_DFRMSIZE4K_READABLE    1
#define HAL_HST_DFRMSIZE4K_WRITABLE    1
#define HAL_HST_DFRMSIZE4K_SIGNED      0
#define HAL_HST_DFRMSIZE4K_ADR         0x10000B7E

#define HAL_HST_RESWDCEN_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_RESWDCEN_LSB         2
#define HAL_HST_RESWDCEN_MSB         2
#define HAL_HST_RESWDCEN_MASK        0x00000004
#define HAL_HST_RESWDCEN_TYPE        halHST_RESWDCEN_t
#define HAL_HST_RESWDCEN             HAL_HST_RESWDCEN_MASK
#define HAL_HST_RESWDCEN_READABLE    1
#define HAL_HST_RESWDCEN_WRITABLE    1
#define HAL_HST_RESWDCEN_SIGNED      0
#define HAL_HST_RESWDCEN_ADR         0x10000B7E

#define HAL_HST_SCNTOVEN_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_SCNTOVEN_LSB         1
#define HAL_HST_SCNTOVEN_MSB         1
#define HAL_HST_SCNTOVEN_MASK        0x00000002
#define HAL_HST_SCNTOVEN_TYPE        halHST_SCNTOVEN_t
#define HAL_HST_SCNTOVEN             HAL_HST_SCNTOVEN_MASK
#define HAL_HST_SCNTOVEN_READABLE    1
#define HAL_HST_SCNTOVEN_WRITABLE    1
#define HAL_HST_SCNTOVEN_SIGNED      0
#define HAL_HST_SCNTOVEN_ADR         0x10000B7E

#define HAL_HST_ODDWRDSUP_REGISTER    (HAL_HSTREG_MAP->MISCTL1)
#define HAL_HST_ODDWRDSUP_LSB         0
#define HAL_HST_ODDWRDSUP_MSB         0
#define HAL_HST_ODDWRDSUP_MASK        0x00000001
#define HAL_HST_ODDWRDSUP_TYPE        halHST_ODDWRDSUP_t
#define HAL_HST_ODDWRDSUP             HAL_HST_ODDWRDSUP_MASK
#define HAL_HST_ODDWRDSUP_READABLE    1
#define HAL_HST_ODDWRDSUP_WRITABLE    1
#define HAL_HST_ODDWRDSUP_SIGNED      0
#define HAL_HST_ODDWRDSUP_ADR         0x10000B7E

// Register MISCTL2
#if (CHIP_REV >= 0xC1)

#define HAL_HST_HOLDBEFRSTCNTDIS_REGISTER    (HAL_HSTREG_MAP->MISCTL2)
#define HAL_HST_HOLDBEFRSTCNTDIS_LSB         7
#define HAL_HST_HOLDBEFRSTCNTDIS_MSB         7
#define HAL_HST_HOLDBEFRSTCNTDIS_MASK        0x00000080
#define HAL_HST_HOLDBEFRSTCNTDIS_TYPE        halHST_HOLDBEFRSTCNTDIS_t
#define HAL_HST_HOLDBEFRSTCNTDIS             HAL_HST_HOLDBEFRSTCNTDIS_MASK
#define HAL_HST_HOLDBEFRSTCNTDIS_READABLE    1
#define HAL_HST_HOLDBEFRSTCNTDIS_WRITABLE    1
#define HAL_HST_HOLDBEFRSTCNTDIS_SIGNED      0
#define HAL_HST_HOLDBEFRSTCNTDIS_ADR         0x10000B7F

#define HAL_HST_PHYSCANTEST_REGISTER    (HAL_HSTREG_MAP->MISCTL2)
#define HAL_HST_PHYSCANTEST_LSB         4
#define HAL_HST_PHYSCANTEST_MSB         4
#define HAL_HST_PHYSCANTEST_MASK        0x00000010
#define HAL_HST_PHYSCANTEST_TYPE        halHST_PHYSCANTEST_t
#define HAL_HST_PHYSCANTEST             HAL_HST_PHYSCANTEST_MASK
#define HAL_HST_PHYSCANTEST_READABLE    1
#define HAL_HST_PHYSCANTEST_WRITABLE    1
#define HAL_HST_PHYSCANTEST_SIGNED      0
#define HAL_HST_PHYSCANTEST_ADR         0x10000B7F

#endif // #if (CHIP_REV >= 0xC1)

#define HAL_HST_AESIOSWAPDIS_REGISTER    (HAL_HSTREG_MAP->MISCTL2)
#define HAL_HST_AESIOSWAPDIS_LSB         3
#define HAL_HST_AESIOSWAPDIS_MSB         3
#define HAL_HST_AESIOSWAPDIS_MASK        0x00000008
#define HAL_HST_AESIOSWAPDIS_TYPE        halHST_AESIOSWAPDIS_t
#define HAL_HST_AESIOSWAPDIS             HAL_HST_AESIOSWAPDIS_MASK
#define HAL_HST_AESIOSWAPDIS_READABLE    1
#define HAL_HST_AESIOSWAPDIS_WRITABLE    1
#define HAL_HST_AESIOSWAPDIS_SIGNED      0
#define HAL_HST_AESIOSWAPDIS_ADR         0x10000B7F

#define HAL_HST_DLXSMRSTEN_REGISTER    (HAL_HSTREG_MAP->MISCTL2)
#define HAL_HST_DLXSMRSTEN_LSB         2
#define HAL_HST_DLXSMRSTEN_MSB         2
#define HAL_HST_DLXSMRSTEN_MASK        0x00000004
#define HAL_HST_DLXSMRSTEN_TYPE        halHST_DLXSMRSTEN_t
#define HAL_HST_DLXSMRSTEN             HAL_HST_DLXSMRSTEN_MASK
#define HAL_HST_DLXSMRSTEN_READABLE    1
#define HAL_HST_DLXSMRSTEN_WRITABLE    1
#define HAL_HST_DLXSMRSTEN_SIGNED      0
#define HAL_HST_DLXSMRSTEN_ADR         0x10000B7F

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_HST_NIENMDATA_REGISTER    (HAL_HSTREG_MAP->MISCTL2)
#define HAL_HST_NIENMDATA_LSB         1
#define HAL_HST_NIENMDATA_MSB         1
#define HAL_HST_NIENMDATA_MASK        0x00000002
#define HAL_HST_NIENMDATA_TYPE        Bool
#define HAL_HST_NIENMDATA             HAL_HST_NIENMDATA_MASK
#define HAL_HST_NIENMDATA_READABLE    1
#define HAL_HST_NIENMDATA_WRITABLE    1
#define HAL_HST_NIENMDATA_SIGNED      0
#define HAL_HST_NIENMDATA_ADR         0x10000B7F

#define HAL_HST_NIENMEN_REGISTER    (HAL_HSTREG_MAP->MISCTL2)
#define HAL_HST_NIENMEN_LSB         0
#define HAL_HST_NIENMEN_MSB         0
#define HAL_HST_NIENMEN_MASK        0x00000001
#define HAL_HST_NIENMEN_TYPE        halHST_NIENMEN_t
#define HAL_HST_NIENMEN             HAL_HST_NIENMEN_MASK
#define HAL_HST_NIENMEN_READABLE    1
#define HAL_HST_NIENMEN_WRITABLE    1
#define HAL_HST_NIENMEN_SIGNED      0
#define HAL_HST_NIENMEN_ADR         0x10000B7F

// Register SHCTRL
#define HAL_HST_CONTDIS_REGISTER    (HAL_HSTREG_MAP->SHCTRL)
#define HAL_HST_CONTDIS_LSB         7
#define HAL_HST_CONTDIS_MSB         7
#define HAL_HST_CONTDIS_MASK        0x00000080
#define HAL_HST_CONTDIS_TYPE        Bool
#define HAL_HST_CONTDIS             HAL_HST_CONTDIS_MASK
#define HAL_HST_CONTDIS_READABLE    1
#define HAL_HST_CONTDIS_WRITABLE    1
#define HAL_HST_CONTDIS_SIGNED      0
#define HAL_HST_CONTDIS_ADR         0x10000B80

#define HAL_HST_SCRMBLDIS_REGISTER    (HAL_HSTREG_MAP->SHCTRL)
#define HAL_HST_SCRMBLDIS_LSB         6
#define HAL_HST_SCRMBLDIS_MSB         6
#define HAL_HST_SCRMBLDIS_MASK        0x00000040
#define HAL_HST_SCRMBLDIS_TYPE        Bool
#define HAL_HST_SCRMBLDIS             HAL_HST_SCRMBLDIS_MASK
#define HAL_HST_SCRMBLDIS_READABLE    1
#define HAL_HST_SCRMBLDIS_WRITABLE    1
#define HAL_HST_SCRMBLDIS_SIGNED      0
#define HAL_HST_SCRMBLDIS_ADR         0x10000B80

#define HAL_HST_DWCLKON_REGISTER    (HAL_HSTREG_MAP->SHCTRL)
#define HAL_HST_DWCLKON_LSB         3
#define HAL_HST_DWCLKON_MSB         3
#define HAL_HST_DWCLKON_MASK        0x00000008
#define HAL_HST_DWCLKON_TYPE        Bool
#define HAL_HST_DWCLKON             HAL_HST_DWCLKON_MASK
#define HAL_HST_DWCLKON_READABLE    1
#define HAL_HST_DWCLKON_WRITABLE    1
#define HAL_HST_DWCLKON_SIGNED      0
#define HAL_HST_DWCLKON_ADR         0x10000B80

#define HAL_HST_COMRSTDIS_REGISTER    (HAL_HSTREG_MAP->SHCTRL)
#define HAL_HST_COMRSTDIS_LSB         2
#define HAL_HST_COMRSTDIS_MSB         2
#define HAL_HST_COMRSTDIS_MASK        0x00000004
#define HAL_HST_COMRSTDIS_TYPE        Bool
#define HAL_HST_COMRSTDIS             HAL_HST_COMRSTDIS_MASK
#define HAL_HST_COMRSTDIS_READABLE    1
#define HAL_HST_COMRSTDIS_WRITABLE    1
#define HAL_HST_COMRSTDIS_SIGNED      0
#define HAL_HST_COMRSTDIS_ADR         0x10000B80

#define HAL_HST_AUTOABRT_REGISTER    (HAL_HSTREG_MAP->SHCTRL)
#define HAL_HST_AUTOABRT_LSB         1
#define HAL_HST_AUTOABRT_MSB         1
#define HAL_HST_AUTOABRT_MASK        0x00000002
#define HAL_HST_AUTOABRT_TYPE        Bool
#define HAL_HST_AUTOABRT             HAL_HST_AUTOABRT_MASK
#define HAL_HST_AUTOABRT_READABLE    1
#define HAL_HST_AUTOABRT_WRITABLE    1
#define HAL_HST_AUTOABRT_SIGNED      0
#define HAL_HST_AUTOABRT_ADR         0x10000B80

#define HAL_HST_AFIS34DIS_REGISTER    (HAL_HSTREG_MAP->SHCTRL)
#define HAL_HST_AFIS34DIS_LSB         0
#define HAL_HST_AFIS34DIS_MSB         0
#define HAL_HST_AFIS34DIS_MASK        0x00000001
#define HAL_HST_AFIS34DIS_TYPE        Bool
#define HAL_HST_AFIS34DIS             HAL_HST_AFIS34DIS_MASK
#define HAL_HST_AFIS34DIS_READABLE    1
#define HAL_HST_AFIS34DIS_WRITABLE    1
#define HAL_HST_AFIS34DIS_SIGNED      0
#define HAL_HST_AFIS34DIS_ADR         0x10000B80

// Register TXREQ
#define HAL_HST_DATABTREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_DATABTREQ_LSB         6
#define HAL_HST_DATABTREQ_MSB         6
#define HAL_HST_DATABTREQ_MASK        0x00000040
#define HAL_HST_DATABTREQ_TYPE        Bool
#define HAL_HST_DATABTREQ             HAL_HST_DATABTREQ_MASK
#define HAL_HST_DATABTREQ_READABLE    1
#define HAL_HST_DATABTREQ_WRITABLE    1
#define HAL_HST_DATABTREQ_SIGNED      0
#define HAL_HST_DATABTREQ_ADR         0x10000B81

#define HAL_HST_TXWKUPREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_TXWKUPREQ_LSB         5
#define HAL_HST_TXWKUPREQ_MSB         5
#define HAL_HST_TXWKUPREQ_MASK        0x00000020
#define HAL_HST_TXWKUPREQ_TYPE        Bool
#define HAL_HST_TXWKUPREQ             HAL_HST_TXWKUPREQ_MASK
#define HAL_HST_TXWKUPREQ_READABLE    1
#define HAL_HST_TXWKUPREQ_WRITABLE    1
#define HAL_HST_TXWKUPREQ_SIGNED      0
#define HAL_HST_TXWKUPREQ_ADR         0x10000B81

#define HAL_HST_TXFISREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_TXFISREQ_LSB         4
#define HAL_HST_TXFISREQ_MSB         4
#define HAL_HST_TXFISREQ_MASK        0x00000010
#define HAL_HST_TXFISREQ_TYPE        Bool
#define HAL_HST_TXFISREQ             HAL_HST_TXFISREQ_MASK
#define HAL_HST_TXFISREQ_READABLE    1
#define HAL_HST_TXFISREQ_WRITABLE    1
#define HAL_HST_TXFISREQ_SIGNED      0
#define HAL_HST_TXFISREQ_ADR         0x10000B81

#define HAL_HST_ABORTREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_ABORTREQ_LSB         3
#define HAL_HST_ABORTREQ_MSB         3
#define HAL_HST_ABORTREQ_MASK        0x00000008
#define HAL_HST_ABORTREQ_TYPE        Bool
#define HAL_HST_ABORTREQ             HAL_HST_ABORTREQ_MASK
#define HAL_HST_ABORTREQ_READABLE    1
#define HAL_HST_ABORTREQ_WRITABLE    1
#define HAL_HST_ABORTREQ_SIGNED      0
#define HAL_HST_ABORTREQ_ADR         0x10000B81

#define HAL_HST_NOTIFYREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_NOTIFYREQ_LSB         2
#define HAL_HST_NOTIFYREQ_MSB         2
#define HAL_HST_NOTIFYREQ_MASK        0x00000004
#define HAL_HST_NOTIFYREQ_TYPE        Bool
#define HAL_HST_NOTIFYREQ             HAL_HST_NOTIFYREQ_MASK
#define HAL_HST_NOTIFYREQ_READABLE    1
#define HAL_HST_NOTIFYREQ_WRITABLE    1
#define HAL_HST_NOTIFYREQ_SIGNED      0
#define HAL_HST_NOTIFYREQ_ADR         0x10000B81

#define HAL_HST_SLMBERREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_SLMBERREQ_LSB         1
#define HAL_HST_SLMBERREQ_MSB         1
#define HAL_HST_SLMBERREQ_MASK        0x00000002
#define HAL_HST_SLMBERREQ_TYPE        Bool
#define HAL_HST_SLMBERREQ             HAL_HST_SLMBERREQ_MASK
#define HAL_HST_SLMBERREQ_READABLE    1
#define HAL_HST_SLMBERREQ_WRITABLE    1
#define HAL_HST_SLMBERREQ_SIGNED      0
#define HAL_HST_SLMBERREQ_ADR         0x10000B81

#define HAL_HST_PARTLREQ_REGISTER    (HAL_HSTREG_MAP->TXREQ)
#define HAL_HST_PARTLREQ_LSB         0
#define HAL_HST_PARTLREQ_MSB         0
#define HAL_HST_PARTLREQ_MASK        0x00000001
#define HAL_HST_PARTLREQ_TYPE        Bool
#define HAL_HST_PARTLREQ             HAL_HST_PARTLREQ_MASK
#define HAL_HST_PARTLREQ_READABLE    1
#define HAL_HST_PARTLREQ_WRITABLE    1
#define HAL_HST_PARTLREQ_SIGNED      0
#define HAL_HST_PARTLREQ_ADR         0x10000B81

// Register REQTXBITS
#define HAL_HST_REQEBSY_REGISTER    (HAL_HSTREG_MAP->REQTXBITS)
#define HAL_HST_REQEBSY_LSB         7
#define HAL_HST_REQEBSY_MSB         7
#define HAL_HST_REQEBSY_MASK        0x00000080
#define HAL_HST_REQEBSY_TYPE        Bool
#define HAL_HST_REQEBSY             HAL_HST_REQEBSY_MASK
#define HAL_HST_REQEBSY_READABLE    1
#define HAL_HST_REQEBSY_WRITABLE    1
#define HAL_HST_REQEBSY_SIGNED      0
#define HAL_HST_REQEBSY_ADR         0x10000B82

#define HAL_HST_REQBITN_REGISTER    (HAL_HSTREG_MAP->REQTXBITS)
#define HAL_HST_REQBITN_LSB         4
#define HAL_HST_REQBITN_MSB         4
#define HAL_HST_REQBITN_MASK        0x00000010
#define HAL_HST_REQBITN_TYPE        Bool
#define HAL_HST_REQBITN             HAL_HST_REQBITN_MASK
#define HAL_HST_REQBITN_READABLE    1
#define HAL_HST_REQBITN_WRITABLE    1
#define HAL_HST_REQBITN_SIGNED      0
#define HAL_HST_REQBITN_ADR         0x10000B82

#define HAL_HST_REQEDRQ_REGISTER    (HAL_HSTREG_MAP->REQTXBITS)
#define HAL_HST_REQEDRQ_LSB         3
#define HAL_HST_REQEDRQ_MSB         3
#define HAL_HST_REQEDRQ_MASK        0x00000008
#define HAL_HST_REQEDRQ_TYPE        Bool
#define HAL_HST_REQEDRQ             HAL_HST_REQEDRQ_MASK
#define HAL_HST_REQEDRQ_READABLE    1
#define HAL_HST_REQEDRQ_WRITABLE    1
#define HAL_HST_REQEDRQ_SIGNED      0
#define HAL_HST_REQEDRQ_ADR         0x10000B82

#define HAL_HST_UPDTBCNT_REGISTER    (HAL_HSTREG_MAP->REQTXBITS)
#define HAL_HST_UPDTBCNT_LSB         0
#define HAL_HST_UPDTBCNT_MSB         0
#define HAL_HST_UPDTBCNT_MASK        0x00000001
#define HAL_HST_UPDTBCNT_TYPE        Bool
#define HAL_HST_UPDTBCNT             HAL_HST_UPDTBCNT_MASK
#define HAL_HST_UPDTBCNT_READABLE    1
#define HAL_HST_UPDTBCNT_WRITABLE    1
#define HAL_HST_UPDTBCNT_SIGNED      0
#define HAL_HST_UPDTBCNT_ADR         0x10000B82

// Register RQTXFSTYP
#define HAL_HST_RQTXFSTYP_REGISTER    (HAL_HSTREG_MAP->RQTXFSTYP)
#define HAL_HST_RQTXFSTYP_LSB         0
#define HAL_HST_RQTXFSTYP_MSB         7
#define HAL_HST_RQTXFSTYP_MASK        0x000000FF
#define HAL_HST_RQTXFSTYP_TYPE        halHST_FISTyp_t
#define HAL_HST_RQTXFSTYP             HAL_HST_RQTXFSTYP_MASK
#define HAL_HST_RQTXFSTYP_READABLE    1
#define HAL_HST_RQTXFSTYP_WRITABLE    1
#define HAL_HST_RQTXFSTYP_SIGNED      0
#define HAL_HST_RQTXFSTYP_ADR         0x10000B83

// Registers REQTXCNTL/REQTXCNTH
#define HAL_HST_REQTXCNT_REGISTER    (HAL_HSTREG_MAP->REQTXCNTL)
#define HAL_HST_REQTXCNT_LSB         0
#define HAL_HST_REQTXCNT_MSB         15
#define HAL_HST_REQTXCNT_MASK        0x0000FFFF
#define HAL_HST_REQTXCNT_TYPE        UInt16
#define HAL_HST_REQTXCNT             HAL_HST_REQTXCNT_MASK
#define HAL_HST_REQTXCNT_READABLE    1
#define HAL_HST_REQTXCNT_WRITABLE    1
#define HAL_HST_REQTXCNT_SIGNED      0
#define HAL_HST_REQTXCNT_ADR         0x10000B84

// Register SATASTAT0
#define HAL_HST_NOTIFYPEN_REGISTER    (HAL_HSTREG_MAP->SATASTAT0)
#define HAL_HST_NOTIFYPEN_LSB         2
#define HAL_HST_NOTIFYPEN_MSB         2
#define HAL_HST_NOTIFYPEN_MASK        0x00000004
#define HAL_HST_NOTIFYPEN_TYPE        Bool
#define HAL_HST_NOTIFYPEN             HAL_HST_NOTIFYPEN_MASK
#define HAL_HST_NOTIFYPEN_READABLE    1
#define HAL_HST_NOTIFYPEN_WRITABLE    0
#define HAL_HST_NOTIFYPEN_SIGNED      0
#define HAL_HST_NOTIFYPEN_ADR         0x10000B86

#define HAL_HST_SLUMBER_REGISTER    (HAL_HSTREG_MAP->SATASTAT0)
#define HAL_HST_SLUMBER_LSB         1
#define HAL_HST_SLUMBER_MSB         1
#define HAL_HST_SLUMBER_MASK        0x00000002
#define HAL_HST_SLUMBER_TYPE        Bool
#define HAL_HST_SLUMBER             HAL_HST_SLUMBER_MASK
#define HAL_HST_SLUMBER_READABLE    1
#define HAL_HST_SLUMBER_WRITABLE    0
#define HAL_HST_SLUMBER_SIGNED      0
#define HAL_HST_SLUMBER_ADR         0x10000B86

#define HAL_HST_PARTIAL_REGISTER    (HAL_HSTREG_MAP->SATASTAT0)
#define HAL_HST_PARTIAL_LSB         0
#define HAL_HST_PARTIAL_MSB         0
#define HAL_HST_PARTIAL_MASK        0x00000001
#define HAL_HST_PARTIAL_TYPE        Bool
#define HAL_HST_PARTIAL             HAL_HST_PARTIAL_MASK
#define HAL_HST_PARTIAL_READABLE    1
#define HAL_HST_PARTIAL_WRITABLE    0
#define HAL_HST_PARTIAL_SIGNED      0
#define HAL_HST_PARTIAL_ADR         0x10000B86

// Register SATASTAT1
#define HAL_HST_EBSY_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_EBSY_LSB         7
#define HAL_HST_EBSY_MSB         7
#define HAL_HST_EBSY_MASK        0x00000080
#define HAL_HST_EBSY_TYPE        Bool
#define HAL_HST_EBSY             HAL_HST_EBSY_MASK
#define HAL_HST_EBSY_READABLE    1
#define HAL_HST_EBSY_WRITABLE    0
#define HAL_HST_EBSY_SIGNED      0
#define HAL_HST_EBSY_ADR         0x10000B87

#define HAL_HST_RXFISOK_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_RXFISOK_LSB         6
#define HAL_HST_RXFISOK_MSB         6
#define HAL_HST_RXFISOK_MASK        0x00000040
#define HAL_HST_RXFISOK_TYPE        Bool
#define HAL_HST_RXFISOK             HAL_HST_RXFISOK_MASK
#define HAL_HST_RXFISOK_READABLE    1
#define HAL_HST_RXFISOK_WRITABLE    0
#define HAL_HST_RXFISOK_SIGNED      0
#define HAL_HST_RXFISOK_ADR         0x10000B87

#define HAL_HST_TXFISOK_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_TXFISOK_LSB         5
#define HAL_HST_TXFISOK_MSB         5
#define HAL_HST_TXFISOK_MASK        0x00000020
#define HAL_HST_TXFISOK_TYPE        Bool
#define HAL_HST_TXFISOK             HAL_HST_TXFISOK_MASK
#define HAL_HST_TXFISOK_READABLE    1
#define HAL_HST_TXFISOK_WRITABLE    0
#define HAL_HST_TXFISOK_SIGNED      0
#define HAL_HST_TXFISOK_ADR         0x10000B87

#define HAL_HST_BITN_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_BITN_LSB         4
#define HAL_HST_BITN_MSB         4
#define HAL_HST_BITN_MASK        0x00000010
#define HAL_HST_BITN_TYPE        Bool
#define HAL_HST_BITN             HAL_HST_BITN_MASK
#define HAL_HST_BITN_READABLE    1
#define HAL_HST_BITN_WRITABLE    0
#define HAL_HST_BITN_SIGNED      0
#define HAL_HST_BITN_ADR         0x10000B87

#define HAL_HST_EDRQ_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_EDRQ_LSB         3
#define HAL_HST_EDRQ_MSB         3
#define HAL_HST_EDRQ_MASK        0x00000008
#define HAL_HST_EDRQ_TYPE        Bool
#define HAL_HST_EDRQ             HAL_HST_EDRQ_MASK
#define HAL_HST_EDRQ_READABLE    1
#define HAL_HST_EDRQ_WRITABLE    0
#define HAL_HST_EDRQ_SIGNED      0
#define HAL_HST_EDRQ_ADR         0x10000B87

#define HAL_HST_BITC_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_BITC_LSB         2
#define HAL_HST_BITC_MSB         2
#define HAL_HST_BITC_MASK        0x00000004
#define HAL_HST_BITC_TYPE        Bool
#define HAL_HST_BITC             HAL_HST_BITC_MASK
#define HAL_HST_BITC_READABLE    1
#define HAL_HST_BITC_WRITABLE    0
#define HAL_HST_BITC_SIGNED      0
#define HAL_HST_BITC_ADR         0x10000B87

#define HAL_HST_LINKRDY_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_LINKRDY_LSB         1
#define HAL_HST_LINKRDY_MSB         1
#define HAL_HST_LINKRDY_MASK        0x00000002
#define HAL_HST_LINKRDY_TYPE        Bool
#define HAL_HST_LINKRDY             HAL_HST_LINKRDY_MASK
#define HAL_HST_LINKRDY_READABLE    1
#define HAL_HST_LINKRDY_WRITABLE    0
#define HAL_HST_LINKRDY_SIGNED      0
#define HAL_HST_LINKRDY_ADR         0x10000B87

#define HAL_HST_PHYRDY_REGISTER    (HAL_HSTREG_MAP->SATASTAT1)
#define HAL_HST_PHYRDY_LSB         0
#define HAL_HST_PHYRDY_MSB         0
#define HAL_HST_PHYRDY_MASK        0x00000001
#define HAL_HST_PHYRDY_TYPE        Bool
#define HAL_HST_PHYRDY             HAL_HST_PHYRDY_MASK
#define HAL_HST_PHYRDY_READABLE    1
#define HAL_HST_PHYRDY_WRITABLE    0
#define HAL_HST_PHYRDY_SIGNED      0
#define HAL_HST_PHYRDY_ADR         0x10000B87

// Register SATAINT
#define HAL_HST_PHYRDYINT_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_PHYRDYINT_LSB         7
#define HAL_HST_PHYRDYINT_MSB         7
#define HAL_HST_PHYRDYINT_MASK        0x00000080
#define HAL_HST_PHYRDYINT_TYPE        Bool
#define HAL_HST_PHYRDYINT             HAL_HST_PHYRDYINT_MASK
#define HAL_HST_PHYRDYINT_READABLE    1
#define HAL_HST_PHYRDYINT_WRITABLE    1
#define HAL_HST_PHYRDYINT_SIGNED      0
#define HAL_HST_PHYRDYINT_ADR         0x10000B88

#define HAL_HST_ABORT_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_ABORT_LSB         6
#define HAL_HST_ABORT_MSB         6
#define HAL_HST_ABORT_MASK        0x00000040
#define HAL_HST_ABORT_TYPE        Bool
#define HAL_HST_ABORT             HAL_HST_ABORT_MASK
#define HAL_HST_ABORT_READABLE    1
#define HAL_HST_ABORT_WRITABLE    1
#define HAL_HST_ABORT_SIGNED      0
#define HAL_HST_ABORT_ADR         0x10000B88

#define HAL_HST_BISTIP_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_BISTIP_LSB         5
#define HAL_HST_BISTIP_MSB         5
#define HAL_HST_BISTIP_MASK        0x00000020
#define HAL_HST_BISTIP_TYPE        Bool
#define HAL_HST_BISTIP             HAL_HST_BISTIP_MASK
#define HAL_HST_BISTIP_READABLE    1
#define HAL_HST_BISTIP_WRITABLE    1
#define HAL_HST_BISTIP_SIGNED      0
#define HAL_HST_BISTIP_ADR         0x10000B88

#define HAL_HST_RXPMREQS_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_RXPMREQS_LSB         4
#define HAL_HST_RXPMREQS_MSB         4
#define HAL_HST_RXPMREQS_MASK        0x00000010
#define HAL_HST_RXPMREQS_TYPE        Bool
#define HAL_HST_RXPMREQS             HAL_HST_RXPMREQS_MASK
#define HAL_HST_RXPMREQS_READABLE    1
#define HAL_HST_RXPMREQS_WRITABLE    1
#define HAL_HST_RXPMREQS_SIGNED      0
#define HAL_HST_RXPMREQS_ADR         0x10000B88

#define HAL_HST_RXPMREQP_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_RXPMREQP_LSB         3
#define HAL_HST_RXPMREQP_MSB         3
#define HAL_HST_RXPMREQP_MASK        0x00000008
#define HAL_HST_RXPMREQP_TYPE        Bool
#define HAL_HST_RXPMREQP             HAL_HST_RXPMREQP_MASK
#define HAL_HST_RXPMREQP_READABLE    1
#define HAL_HST_RXPMREQP_WRITABLE    1
#define HAL_HST_RXPMREQP_SIGNED      0
#define HAL_HST_RXPMREQP_ADR         0x10000B88

#define HAL_HST_ERRTXRX_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_ERRTXRX_LSB         2
#define HAL_HST_ERRTXRX_MSB         2
#define HAL_HST_ERRTXRX_MASK        0x00000004
#define HAL_HST_ERRTXRX_TYPE        Bool
#define HAL_HST_ERRTXRX             HAL_HST_ERRTXRX_MASK
#define HAL_HST_ERRTXRX_READABLE    1
#define HAL_HST_ERRTXRX_WRITABLE    1
#define HAL_HST_ERRTXRX_SIGNED      0
#define HAL_HST_ERRTXRX_ADR         0x10000B88

#define HAL_HST_RXPMACK_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_RXPMACK_LSB         1
#define HAL_HST_RXPMACK_MSB         1
#define HAL_HST_RXPMACK_MASK        0x00000002
#define HAL_HST_RXPMACK_TYPE        Bool
#define HAL_HST_RXPMACK             HAL_HST_RXPMACK_MASK
#define HAL_HST_RXPMACK_READABLE    1
#define HAL_HST_RXPMACK_WRITABLE    1
#define HAL_HST_RXPMACK_SIGNED      0
#define HAL_HST_RXPMACK_ADR         0x10000B88

#define HAL_HST_RXPMNAK_REGISTER    (HAL_HSTREG_MAP->SATAINT)
#define HAL_HST_RXPMNAK_LSB         0
#define HAL_HST_RXPMNAK_MSB         0
#define HAL_HST_RXPMNAK_MASK        0x00000001
#define HAL_HST_RXPMNAK_TYPE        Bool
#define HAL_HST_RXPMNAK             HAL_HST_RXPMNAK_MASK
#define HAL_HST_RXPMNAK_READABLE    1
#define HAL_HST_RXPMNAK_WRITABLE    1
#define HAL_HST_RXPMNAK_SIGNED      0
#define HAL_HST_RXPMNAK_ADR         0x10000B88

// Register SATAINTEN
#define HAL_HST_PHYRDYIEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_PHYRDYIEN_LSB         7
#define HAL_HST_PHYRDYIEN_MSB         7
#define HAL_HST_PHYRDYIEN_MASK        0x00000080
#define HAL_HST_PHYRDYIEN_TYPE        Bool
#define HAL_HST_PHYRDYIEN             HAL_HST_PHYRDYIEN_MASK
#define HAL_HST_PHYRDYIEN_READABLE    1
#define HAL_HST_PHYRDYIEN_WRITABLE    1
#define HAL_HST_PHYRDYIEN_SIGNED      0
#define HAL_HST_PHYRDYIEN_ADR         0x10000B89

#define HAL_HST_ABORTEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_ABORTEN_LSB         6
#define HAL_HST_ABORTEN_MSB         6
#define HAL_HST_ABORTEN_MASK        0x00000040
#define HAL_HST_ABORTEN_TYPE        Bool
#define HAL_HST_ABORTEN             HAL_HST_ABORTEN_MASK
#define HAL_HST_ABORTEN_READABLE    1
#define HAL_HST_ABORTEN_WRITABLE    1
#define HAL_HST_ABORTEN_SIGNED      0
#define HAL_HST_ABORTEN_ADR         0x10000B89

#define HAL_HST_BISTIPEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_BISTIPEN_LSB         5
#define HAL_HST_BISTIPEN_MSB         5
#define HAL_HST_BISTIPEN_MASK        0x00000020
#define HAL_HST_BISTIPEN_TYPE        Bool
#define HAL_HST_BISTIPEN             HAL_HST_BISTIPEN_MASK
#define HAL_HST_BISTIPEN_READABLE    1
#define HAL_HST_BISTIPEN_WRITABLE    1
#define HAL_HST_BISTIPEN_SIGNED      0
#define HAL_HST_BISTIPEN_ADR         0x10000B89

#define HAL_HST_RXPMRQSEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_RXPMRQSEN_LSB         4
#define HAL_HST_RXPMRQSEN_MSB         4
#define HAL_HST_RXPMRQSEN_MASK        0x00000010
#define HAL_HST_RXPMRQSEN_TYPE        Bool
#define HAL_HST_RXPMRQSEN             HAL_HST_RXPMRQSEN_MASK
#define HAL_HST_RXPMRQSEN_READABLE    1
#define HAL_HST_RXPMRQSEN_WRITABLE    1
#define HAL_HST_RXPMRQSEN_SIGNED      0
#define HAL_HST_RXPMRQSEN_ADR         0x10000B89

#define HAL_HST_RXPMRQPEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_RXPMRQPEN_LSB         3
#define HAL_HST_RXPMRQPEN_MSB         3
#define HAL_HST_RXPMRQPEN_MASK        0x00000008
#define HAL_HST_RXPMRQPEN_TYPE        Bool
#define HAL_HST_RXPMRQPEN             HAL_HST_RXPMRQPEN_MASK
#define HAL_HST_RXPMRQPEN_READABLE    1
#define HAL_HST_RXPMRQPEN_WRITABLE    1
#define HAL_HST_RXPMRQPEN_SIGNED      0
#define HAL_HST_RXPMRQPEN_ADR         0x10000B89

#define HAL_HST_ERRTXRXEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_ERRTXRXEN_LSB         2
#define HAL_HST_ERRTXRXEN_MSB         2
#define HAL_HST_ERRTXRXEN_MASK        0x00000004
#define HAL_HST_ERRTXRXEN_TYPE        Bool
#define HAL_HST_ERRTXRXEN             HAL_HST_ERRTXRXEN_MASK
#define HAL_HST_ERRTXRXEN_READABLE    1
#define HAL_HST_ERRTXRXEN_WRITABLE    1
#define HAL_HST_ERRTXRXEN_SIGNED      0
#define HAL_HST_ERRTXRXEN_ADR         0x10000B89

#define HAL_HST_RXPMACKEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_RXPMACKEN_LSB         1
#define HAL_HST_RXPMACKEN_MSB         1
#define HAL_HST_RXPMACKEN_MASK        0x00000002
#define HAL_HST_RXPMACKEN_TYPE        Bool
#define HAL_HST_RXPMACKEN             HAL_HST_RXPMACKEN_MASK
#define HAL_HST_RXPMACKEN_READABLE    1
#define HAL_HST_RXPMACKEN_WRITABLE    1
#define HAL_HST_RXPMACKEN_SIGNED      0
#define HAL_HST_RXPMACKEN_ADR         0x10000B89

#define HAL_HST_RXPMNAKEN_REGISTER    (HAL_HSTREG_MAP->SATAINTEN)
#define HAL_HST_RXPMNAKEN_LSB         0
#define HAL_HST_RXPMNAKEN_MSB         0
#define HAL_HST_RXPMNAKEN_MASK        0x00000001
#define HAL_HST_RXPMNAKEN_TYPE        Bool
#define HAL_HST_RXPMNAKEN             HAL_HST_RXPMNAKEN_MASK
#define HAL_HST_RXPMNAKEN_READABLE    1
#define HAL_HST_RXPMNAKEN_WRITABLE    1
#define HAL_HST_RXPMNAKEN_SIGNED      0
#define HAL_HST_RXPMNAKEN_ADR         0x10000B89

// Registers RXDFISCCL/RXDFISCCH
#define HAL_HST_RXDFISCC_REGISTER    (HAL_HSTREG_MAP->RXDFISCCL)
#define HAL_HST_RXDFISCC_LSB         0
#define HAL_HST_RXDFISCC_MSB         12
#define HAL_HST_RXDFISCC_MASK        0x00001FFF
#define HAL_HST_RXDFISCC_TYPE        UInt16
#define HAL_HST_RXDFISCC             HAL_HST_RXDFISCC_MASK
#define HAL_HST_RXDFISCC_READABLE    1
#define HAL_HST_RXDFISCC_WRITABLE    0
#define HAL_HST_RXDFISCC_SIGNED      0
#define HAL_HST_RXDFISCC_ADR         0x10000B8A

// Registers TXBCNTLO/TXBCNTHI
#define HAL_HST_TXBCNT_REGISTER    (HAL_HSTREG_MAP->TXBCNTLO)
#define HAL_HST_TXBCNT_LSB         0
#define HAL_HST_TXBCNT_MSB         15
#define HAL_HST_TXBCNT_MASK        0x0000FFFF
#define HAL_HST_TXBCNT_TYPE        UInt16
#define HAL_HST_TXBCNT             HAL_HST_TXBCNT_MASK
#define HAL_HST_TXBCNT_READABLE    1
#define HAL_HST_TXBCNT_WRITABLE    0
#define HAL_HST_TXBCNT_SIGNED      0
#define HAL_HST_TXBCNT_ADR         0x10000B8C

// Register SATAMCTRL
#define HAL_HST_TXFISDIS_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_TXFISDIS_LSB         6
#define HAL_HST_TXFISDIS_MSB         6
#define HAL_HST_TXFISDIS_MASK        0x00000040
#define HAL_HST_TXFISDIS_TYPE        Bool
#define HAL_HST_TXFISDIS             HAL_HST_TXFISDIS_MASK
#define HAL_HST_TXFISDIS_READABLE    1
#define HAL_HST_TXFISDIS_WRITABLE    1
#define HAL_HST_TXFISDIS_SIGNED      0
#define HAL_HST_TXFISDIS_ADR         0x10000B8E

#define HAL_HST_DMATDIS_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_DMATDIS_LSB         5
#define HAL_HST_DMATDIS_MSB         5
#define HAL_HST_DMATDIS_MASK        0x00000020
#define HAL_HST_DMATDIS_TYPE        Bool
#define HAL_HST_DMATDIS             HAL_HST_DMATDIS_MASK
#define HAL_HST_DMATDIS_READABLE    1
#define HAL_HST_DMATDIS_WRITABLE    1
#define HAL_HST_DMATDIS_SIGNED      0
#define HAL_HST_DMATDIS_ADR         0x10000B8E

#define HAL_HST_ABRTBTDIS_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_ABRTBTDIS_LSB         4
#define HAL_HST_ABRTBTDIS_MSB         4
#define HAL_HST_ABRTBTDIS_MASK        0x00000010
#define HAL_HST_ABRTBTDIS_TYPE        Bool
#define HAL_HST_ABRTBTDIS             HAL_HST_ABRTBTDIS_MASK
#define HAL_HST_ABRTBTDIS_READABLE    1
#define HAL_HST_ABRTBTDIS_WRITABLE    1
#define HAL_HST_ABRTBTDIS_SIGNED      0
#define HAL_HST_ABRTBTDIS_ADR         0x10000B8E

#define HAL_HST_LSTPG1EN_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_LSTPG1EN_LSB         3
#define HAL_HST_LSTPG1EN_MSB         3
#define HAL_HST_LSTPG1EN_MASK        0x00000008
#define HAL_HST_LSTPG1EN_TYPE        Bool
#define HAL_HST_LSTPG1EN             HAL_HST_LSTPG1EN_MASK
#define HAL_HST_LSTPG1EN_READABLE    1
#define HAL_HST_LSTPG1EN_WRITABLE    1
#define HAL_HST_LSTPG1EN_SIGNED      0
#define HAL_HST_LSTPG1EN_ADR         0x10000B8E

#define HAL_HST_LSTPG1_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_LSTPG1_LSB         2
#define HAL_HST_LSTPG1_MSB         2
#define HAL_HST_LSTPG1_MASK        0x00000004
#define HAL_HST_LSTPG1_TYPE        Bool
#define HAL_HST_LSTPG1             HAL_HST_LSTPG1_MASK
#define HAL_HST_LSTPG1_READABLE    1
#define HAL_HST_LSTPG1_WRITABLE    1
#define HAL_HST_LSTPG1_SIGNED      0
#define HAL_HST_LSTPG1_ADR         0x10000B8E

#define HAL_HST_MPIOS_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_MPIOS_LSB         1
#define HAL_HST_MPIOS_MSB         1
#define HAL_HST_MPIOS_MASK        0x00000002
#define HAL_HST_MPIOS_TYPE        Bool
#define HAL_HST_MPIOS             HAL_HST_MPIOS_MASK
#define HAL_HST_MPIOS_READABLE    1
#define HAL_HST_MPIOS_WRITABLE    1
#define HAL_HST_MPIOS_SIGNED      0
#define HAL_HST_MPIOS_ADR         0x10000B8E

#define HAL_HST_MDMAA_REGISTER    (HAL_HSTREG_MAP->SATAMCTRL)
#define HAL_HST_MDMAA_LSB         0
#define HAL_HST_MDMAA_MSB         0
#define HAL_HST_MDMAA_MASK        0x00000001
#define HAL_HST_MDMAA_TYPE        Bool
#define HAL_HST_MDMAA             HAL_HST_MDMAA_MASK
#define HAL_HST_MDMAA_READABLE    1
#define HAL_HST_MDMAA_WRITABLE    1
#define HAL_HST_MDMAA_SIGNED      0
#define HAL_HST_MDMAA_ADR         0x10000B8E

// Register SATADEBUG
#define HAL_HST_TESTCLKEN_REGISTER    (HAL_HSTREG_MAP->SATADEBUG)
#define HAL_HST_TESTCLKEN_LSB         7
#define HAL_HST_TESTCLKEN_MSB         7
#define HAL_HST_TESTCLKEN_MASK        0x00000080
#define HAL_HST_TESTCLKEN_TYPE        Bool
#define HAL_HST_TESTCLKEN             HAL_HST_TESTCLKEN_MASK
#define HAL_HST_TESTCLKEN_READABLE    1
#define HAL_HST_TESTCLKEN_WRITABLE    1
#define HAL_HST_TESTCLKEN_SIGNED      0
#define HAL_HST_TESTCLKEN_ADR         0x10000B8F

#define HAL_HST_ENASLPDCK_REGISTER    (HAL_HSTREG_MAP->SATADEBUG)
#define HAL_HST_ENASLPDCK_LSB         6
#define HAL_HST_ENASLPDCK_MSB         6
#define HAL_HST_ENASLPDCK_MASK        0x00000040
#define HAL_HST_ENASLPDCK_TYPE        Bool
#define HAL_HST_ENASLPDCK             HAL_HST_ENASLPDCK_MASK
#define HAL_HST_ENASLPDCK_READABLE    1
#define HAL_HST_ENASLPDCK_WRITABLE    1
#define HAL_HST_ENASLPDCK_SIGNED      0
#define HAL_HST_ENASLPDCK_ADR         0x10000B8F

#if (CHIP_REV >= 0xC0)

#define HAL_HST_ENAHIFCK_REGISTER    (HAL_HSTREG_MAP->SATADEBUG)
#define HAL_HST_ENAHIFCK_LSB         5
#define HAL_HST_ENAHIFCK_MSB         5
#define HAL_HST_ENAHIFCK_MASK        0x00000020
#define HAL_HST_ENAHIFCK_TYPE        Bool
#define HAL_HST_ENAHIFCK             HAL_HST_ENAHIFCK_MASK
#define HAL_HST_ENAHIFCK_READABLE    1
#define HAL_HST_ENAHIFCK_WRITABLE    1
#define HAL_HST_ENAHIFCK_SIGNED      0
#define HAL_HST_ENAHIFCK_ADR         0x10000B8F

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_HST_ENADBGHI_REGISTER    (HAL_HSTREG_MAP->SATADEBUG)
#define HAL_HST_ENADBGHI_LSB         4
#define HAL_HST_ENADBGHI_MSB         4
#define HAL_HST_ENADBGHI_MASK        0x00000010
#define HAL_HST_ENADBGHI_TYPE        Bool
#define HAL_HST_ENADBGHI             HAL_HST_ENADBGHI_MASK
#define HAL_HST_ENADBGHI_READABLE    1
#define HAL_HST_ENADBGHI_WRITABLE    1
#define HAL_HST_ENADBGHI_SIGNED      0
#define HAL_HST_ENADBGHI_ADR         0x10000B8F

#define HAL_HST_SATADEBUG_REGISTER    (HAL_HSTREG_MAP->SATADEBUG)
#define HAL_HST_SATADEBUG_LSB         0
#define HAL_HST_SATADEBUG_MSB         3
#define HAL_HST_SATADEBUG_MASK        0x0000000F
#define HAL_HST_SATADEBUG_TYPE        halHST_SATADbg_t
#define HAL_HST_SATADEBUG             HAL_HST_SATADEBUG_MASK
#define HAL_HST_SATADEBUG_READABLE    1
#define HAL_HST_SATADEBUG_WRITABLE    1
#define HAL_HST_SATADEBUG_SIGNED      0
#define HAL_HST_SATADEBUG_ADR         0x10000B8F

// Register SATAINT1

#if (CHIP_REV >= 0xC0)

#define HAL_HST_RXWKUPINT_REGISTER    (HAL_HSTREG_MAP->SATAINT1)
#define HAL_HST_RXWKUPINT_LSB         5
#define HAL_HST_RXWKUPINT_MSB         5
#define HAL_HST_RXWKUPINT_MASK        0x00000020
#define HAL_HST_RXWKUPINT_TYPE        Bool
#define HAL_HST_RXWKUPINT             HAL_HST_RXWKUPINT_MASK
#define HAL_HST_RXWKUPINT_READABLE    1
#define HAL_HST_RXWKUPINT_WRITABLE    1
#define HAL_HST_RXWKUPINT_SIGNED      0
#define HAL_HST_RXWKUPINT_ADR         0x10000B90

#define HAL_HST_IDLESINT_REGISTER    (HAL_HSTREG_MAP->SATAINT1)
#define HAL_HST_IDLESINT_LSB         4
#define HAL_HST_IDLESINT_MSB         4
#define HAL_HST_IDLESINT_MASK        0x00000010
#define HAL_HST_IDLESINT_TYPE        Bool
#define HAL_HST_IDLESINT             HAL_HST_IDLESINT_MASK
#define HAL_HST_IDLESINT_READABLE    1
#define HAL_HST_IDLESINT_WRITABLE    1
#define HAL_HST_IDLESINT_SIGNED      0
#define HAL_HST_IDLESINT_ADR         0x10000B90

#define HAL_HST_IDLEPINT_REGISTER    (HAL_HSTREG_MAP->SATAINT1)
#define HAL_HST_IDLEPINT_LSB         3
#define HAL_HST_IDLEPINT_MSB         3
#define HAL_HST_IDLEPINT_MASK        0x00000008
#define HAL_HST_IDLEPINT_TYPE        Bool
#define HAL_HST_IDLEPINT             HAL_HST_IDLEPINT_MASK
#define HAL_HST_IDLEPINT_READABLE    1
#define HAL_HST_IDLEPINT_WRITABLE    1
#define HAL_HST_IDLEPINT_SIGNED      0
#define HAL_HST_IDLEPINT_ADR         0x10000B90

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_HST_SYNCINT_REGISTER    (HAL_HSTREG_MAP->SATAINT1)
#define HAL_HST_SYNCINT_LSB         2
#define HAL_HST_SYNCINT_MSB         2
#define HAL_HST_SYNCINT_MASK        0x00000004
#define HAL_HST_SYNCINT_TYPE        Bool
#define HAL_HST_SYNCINT             HAL_HST_SYNCINT_MASK
#define HAL_HST_SYNCINT_READABLE    1
#define HAL_HST_SYNCINT_WRITABLE    1
#define HAL_HST_SYNCINT_SIGNED      0
#define HAL_HST_SYNCINT_ADR         0x10000B90

#define HAL_HST_COMRSTINT_REGISTER    (HAL_HSTREG_MAP->SATAINT1)
#define HAL_HST_COMRSTINT_LSB         1
#define HAL_HST_COMRSTINT_MSB         1
#define HAL_HST_COMRSTINT_MASK        0x00000002
#define HAL_HST_COMRSTINT_TYPE        Bool
#define HAL_HST_COMRSTINT             HAL_HST_COMRSTINT_MASK
#define HAL_HST_COMRSTINT_READABLE    1
#define HAL_HST_COMRSTINT_WRITABLE    1
#define HAL_HST_COMRSTINT_SIGNED      0
#define HAL_HST_COMRSTINT_ADR         0x10000B90

#define HAL_HST_RXUNFIS_REGISTER    (HAL_HSTREG_MAP->SATAINT1)
#define HAL_HST_RXUNFIS_LSB         0
#define HAL_HST_RXUNFIS_MSB         0
#define HAL_HST_RXUNFIS_MASK        0x00000001
#define HAL_HST_RXUNFIS_TYPE        Bool
#define HAL_HST_RXUNFIS             HAL_HST_RXUNFIS_MASK
#define HAL_HST_RXUNFIS_READABLE    1
#define HAL_HST_RXUNFIS_WRITABLE    1
#define HAL_HST_RXUNFIS_SIGNED      0
#define HAL_HST_RXUNFIS_ADR         0x10000B90

// Register SATINTEN1

#if (CHIP_REV >= 0xC0)

#define HAL_HST_RXWKUPREQEN_REGISTER    (HAL_HSTREG_MAP->SATINTEN1)
#define HAL_HST_RXWKUPREQEN_LSB         5
#define HAL_HST_RXWKUPREQEN_MSB         5
#define HAL_HST_RXWKUPREQEN_MASK        0x00000020
#define HAL_HST_RXWKUPREQEN_TYPE        Bool
#define HAL_HST_RXWKUPREQEN             HAL_HST_RXWKUPREQEN_MASK
#define HAL_HST_RXWKUPREQEN_READABLE    1
#define HAL_HST_RXWKUPREQEN_WRITABLE    1
#define HAL_HST_RXWKUPREQEN_SIGNED      0
#define HAL_HST_RXWKUPREQEN_ADR         0x10000B91

#define HAL_HST_IDLESEN_REGISTER    (HAL_HSTREG_MAP->SATINTEN1)
#define HAL_HST_IDLESEN_LSB         4
#define HAL_HST_IDLESEN_MSB         4
#define HAL_HST_IDLESEN_MASK        0x00000010
#define HAL_HST_IDLESEN_TYPE        Bool
#define HAL_HST_IDLESEN             HAL_HST_IDLESEN_MASK
#define HAL_HST_IDLESEN_READABLE    1
#define HAL_HST_IDLESEN_WRITABLE    1
#define HAL_HST_IDLESEN_SIGNED      0
#define HAL_HST_IDLESEN_ADR         0x10000B91

#define HAL_HST_IDLEPEN_REGISTER    (HAL_HSTREG_MAP->SATINTEN1)
#define HAL_HST_IDLEPEN_LSB         3
#define HAL_HST_IDLEPEN_MSB         3
#define HAL_HST_IDLEPEN_MASK        0x00000008
#define HAL_HST_IDLEPEN_TYPE        Bool
#define HAL_HST_IDLEPEN             HAL_HST_IDLEPEN_MASK
#define HAL_HST_IDLEPEN_READABLE    1
#define HAL_HST_IDLEPEN_WRITABLE    1
#define HAL_HST_IDLEPEN_SIGNED      0
#define HAL_HST_IDLEPEN_ADR         0x10000B91

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_HST_SYNCEN_REGISTER    (HAL_HSTREG_MAP->SATINTEN1)
#define HAL_HST_SYNCEN_LSB         2
#define HAL_HST_SYNCEN_MSB         2
#define HAL_HST_SYNCEN_MASK        0x00000004
#define HAL_HST_SYNCEN_TYPE        Bool
#define HAL_HST_SYNCEN             HAL_HST_SYNCEN_MASK
#define HAL_HST_SYNCEN_READABLE    1
#define HAL_HST_SYNCEN_WRITABLE    1
#define HAL_HST_SYNCEN_SIGNED      0
#define HAL_HST_SYNCEN_ADR         0x10000B91

#define HAL_HST_COMRSTEN_REGISTER    (HAL_HSTREG_MAP->SATINTEN1)
#define HAL_HST_COMRSTEN_LSB         1
#define HAL_HST_COMRSTEN_MSB         1
#define HAL_HST_COMRSTEN_MASK        0x00000002
#define HAL_HST_COMRSTEN_TYPE        Bool
#define HAL_HST_COMRSTEN             HAL_HST_COMRSTEN_MASK
#define HAL_HST_COMRSTEN_READABLE    1
#define HAL_HST_COMRSTEN_WRITABLE    1
#define HAL_HST_COMRSTEN_SIGNED      0
#define HAL_HST_COMRSTEN_ADR         0x10000B91

#define HAL_HST_RXUNFISEN_REGISTER    (HAL_HSTREG_MAP->SATINTEN1)
#define HAL_HST_RXUNFISEN_LSB         0
#define HAL_HST_RXUNFISEN_MSB         0
#define HAL_HST_RXUNFISEN_MASK        0x00000001
#define HAL_HST_RXUNFISEN_TYPE        Bool
#define HAL_HST_RXUNFISEN             HAL_HST_RXUNFISEN_MASK
#define HAL_HST_RXUNFISEN_READABLE    1
#define HAL_HST_RXUNFISEN_WRITABLE    1
#define HAL_HST_RXUNFISEN_SIGNED      0
#define HAL_HST_RXUNFISEN_ADR         0x10000B91

// Register SHCTRL1
#define HAL_HST_CMDSMRST_REGISTER    (HAL_HSTREG_MAP->SHCTRL1)
#define HAL_HST_CMDSMRST_LSB         4
#define HAL_HST_CMDSMRST_MSB         4
#define HAL_HST_CMDSMRST_MASK        0x00000010
#define HAL_HST_CMDSMRST_TYPE        Bool
#define HAL_HST_CMDSMRST             HAL_HST_CMDSMRST_MASK
#define HAL_HST_CMDSMRST_READABLE    1
#define HAL_HST_CMDSMRST_WRITABLE    1
#define HAL_HST_CMDSMRST_SIGNED      0
#define HAL_HST_CMDSMRST_ADR         0x10000B92

#define HAL_HST_TRANSMRST_REGISTER    (HAL_HSTREG_MAP->SHCTRL1)
#define HAL_HST_TRANSMRST_LSB         3
#define HAL_HST_TRANSMRST_MSB         3
#define HAL_HST_TRANSMRST_MASK        0x00000008
#define HAL_HST_TRANSMRST_TYPE        Bool
#define HAL_HST_TRANSMRST             HAL_HST_TRANSMRST_MASK
#define HAL_HST_TRANSMRST_READABLE    1
#define HAL_HST_TRANSMRST_WRITABLE    1
#define HAL_HST_TRANSMRST_SIGNED      0
#define HAL_HST_TRANSMRST_ADR         0x10000B92

#define HAL_HST_TXCHOFF_REGISTER    (HAL_HSTREG_MAP->SHCTRL1)
#define HAL_HST_TXCHOFF_LSB         2
#define HAL_HST_TXCHOFF_MSB         2
#define HAL_HST_TXCHOFF_MASK        0x00000004
#define HAL_HST_TXCHOFF_TYPE        Bool
#define HAL_HST_TXCHOFF             HAL_HST_TXCHOFF_MASK
#define HAL_HST_TXCHOFF_READABLE    1
#define HAL_HST_TXCHOFF_WRITABLE    1
#define HAL_HST_TXCHOFF_SIGNED      0
#define HAL_HST_TXCHOFF_ADR         0x10000B92

#define HAL_HST_DISSYNCP_REGISTER    (HAL_HSTREG_MAP->SHCTRL1)
#define HAL_HST_DISSYNCP_LSB         1
#define HAL_HST_DISSYNCP_MSB         1
#define HAL_HST_DISSYNCP_MASK        0x00000002
#define HAL_HST_DISSYNCP_TYPE        Bool
#define HAL_HST_DISSYNCP             HAL_HST_DISSYNCP_MASK
#define HAL_HST_DISSYNCP_READABLE    1
#define HAL_HST_DISSYNCP_WRITABLE    1
#define HAL_HST_DISSYNCP_SIGNED      0
#define HAL_HST_DISSYNCP_ADR         0x10000B92

#define HAL_HST_PMREQDIS_REGISTER    (HAL_HSTREG_MAP->SHCTRL1)
#define HAL_HST_PMREQDIS_LSB         0
#define HAL_HST_PMREQDIS_MSB         0
#define HAL_HST_PMREQDIS_MASK        0x00000001
#define HAL_HST_PMREQDIS_TYPE        Bool
#define HAL_HST_PMREQDIS             HAL_HST_PMREQDIS_MASK
#define HAL_HST_PMREQDIS_READABLE    1
#define HAL_HST_PMREQDIS_WRITABLE    1
#define HAL_HST_PMREQDIS_SIGNED      0
#define HAL_HST_PMREQDIS_ADR         0x10000B92

// Register FIS34RDY
#define HAL_HST_DISF34CNT_REGISTER    (HAL_HSTREG_MAP->FIS34RDY)
#define HAL_HST_DISF34CNT_LSB         7
#define HAL_HST_DISF34CNT_MSB         7
#define HAL_HST_DISF34CNT_MASK        0x00000080
#define HAL_HST_DISF34CNT_TYPE        Bool
#define HAL_HST_DISF34CNT             HAL_HST_DISF34CNT_MASK
#define HAL_HST_DISF34CNT_READABLE    1
#define HAL_HST_DISF34CNT_WRITABLE    1
#define HAL_HST_DISF34CNT_SIGNED      0
#define HAL_HST_DISF34CNT_ADR         0x10000B93

#define HAL_HST_F34RDYCNT_REGISTER    (HAL_HSTREG_MAP->FIS34RDY)
#define HAL_HST_F34RDYCNT_LSB         0
#define HAL_HST_F34RDYCNT_MSB         6
#define HAL_HST_F34RDYCNT_MASK        0x0000007F
#define HAL_HST_F34RDYCNT_TYPE        UByte
#define HAL_HST_F34RDYCNT             HAL_HST_F34RDYCNT_MASK
#define HAL_HST_F34RDYCNT_READABLE    1
#define HAL_HST_F34RDYCNT_WRITABLE    1
#define HAL_HST_F34RDYCNT_SIGNED      0
#define HAL_HST_F34RDYCNT_ADR         0x10000B93

// Register TOTALWDCC
#define HAL_HST_TOTALWDCC_REGISTER    (HAL_HSTREG_MAP->TOTALWDCC)
#define HAL_HST_TOTALWDCC_LSB         0
#define HAL_HST_TOTALWDCC_MSB         7
#define HAL_HST_TOTALWDCC_MASK        0x000000FF
#define HAL_HST_TOTALWDCC_TYPE        UByte
#define HAL_HST_TOTALWDCC             HAL_HST_TOTALWDCC_MASK
#define HAL_HST_TOTALWDCC_READABLE    1
#define HAL_HST_TOTALWDCC_WRITABLE    0
#define HAL_HST_TOTALWDCC_SIGNED      0
#define HAL_HST_TOTALWDCC_ADR         0x10000B94

// Register RXFIFOSAT
#define HAL_HST_FIFOEMP_REGISTER    (HAL_HSTREG_MAP->RXFIFOSAT)
#define HAL_HST_FIFOEMP_LSB         7
#define HAL_HST_FIFOEMP_MSB         7
#define HAL_HST_FIFOEMP_MASK        0x00000080
#define HAL_HST_FIFOEMP_TYPE        Bool
#define HAL_HST_FIFOEMP             HAL_HST_FIFOEMP_MASK
#define HAL_HST_FIFOEMP_READABLE    1
#define HAL_HST_FIFOEMP_WRITABLE    0
#define HAL_HST_FIFOEMP_SIGNED      0
#define HAL_HST_FIFOEMP_ADR         0x10000B95

#define HAL_HST_FIFOFUL_REGISTER    (HAL_HSTREG_MAP->RXFIFOSAT)
#define HAL_HST_FIFOFUL_LSB         6
#define HAL_HST_FIFOFUL_MSB         6
#define HAL_HST_FIFOFUL_MASK        0x00000040
#define HAL_HST_FIFOFUL_TYPE        Bool
#define HAL_HST_FIFOFUL             HAL_HST_FIFOFUL_MASK
#define HAL_HST_FIFOFUL_READABLE    1
#define HAL_HST_FIFOFUL_WRITABLE    0
#define HAL_HST_FIFOFUL_SIGNED      0
#define HAL_HST_FIFOFUL_ADR         0x10000B95

#define HAL_HST_FIFOLEVEL_REGISTER    (HAL_HSTREG_MAP->RXFIFOSAT)
#define HAL_HST_FIFOLEVEL_LSB         0
#define HAL_HST_FIFOLEVEL_MSB         5
#define HAL_HST_FIFOLEVEL_MASK        0x0000003F
#define HAL_HST_FIFOLEVEL_TYPE        UByte
#define HAL_HST_FIFOLEVEL             HAL_HST_FIFOLEVEL_MASK
#define HAL_HST_FIFOLEVEL_READABLE    1
#define HAL_HST_FIFOLEVEL_WRITABLE    0
#define HAL_HST_FIFOLEVEL_SIGNED      0
#define HAL_HST_FIFOLEVEL_ADR         0x10000B95

// Register RXFISTYPE
#define HAL_HST_RXFISTYPE_REGISTER    (HAL_HSTREG_MAP->RXFISTYPE)
#define HAL_HST_RXFISTYPE_LSB         0
#define HAL_HST_RXFISTYPE_MSB         7
#define HAL_HST_RXFISTYPE_MASK        0x000000FF
#define HAL_HST_RXFISTYPE_TYPE        UByte
#define HAL_HST_RXFISTYPE             HAL_HST_RXFISTYPE_MASK
#define HAL_HST_RXFISTYPE_READABLE    1
#define HAL_HST_RXFISTYPE_WRITABLE    0
#define HAL_HST_RXFISTYPE_SIGNED      0
#define HAL_HST_RXFISTYPE_ADR         0x10000B96

// Register TXFISTYPE
#define HAL_HST_TXFISTYPE_REGISTER    (HAL_HSTREG_MAP->TXFISTYPE)
#define HAL_HST_TXFISTYPE_LSB         0
#define HAL_HST_TXFISTYPE_MSB         7
#define HAL_HST_TXFISTYPE_MASK        0x000000FF
#define HAL_HST_TXFISTYPE_TYPE        UByte
#define HAL_HST_TXFISTYPE             HAL_HST_TXFISTYPE_MASK
#define HAL_HST_TXFISTYPE_READABLE    1
#define HAL_HST_TXFISTYPE_WRITABLE    0
#define HAL_HST_TXFISTYPE_SIGNED      0
#define HAL_HST_TXFISTYPE_ADR         0x10000B97

#if (CHIP_REV <= 0xB3)

// Register HSTDBG64S
#define HAL_HST_HSTDBG64S_REGISTER    (HAL_HSTREG_MAP->BZCTL)
#define HAL_HST_HSTDBG64S_LSB         0
#define HAL_HST_HSTDBG64S_MSB         3
#define HAL_HST_HSTDBG64S_MASK        0x0000000F
#define HAL_HST_HSTDBG64S_TYPE        UByte
#define HAL_HST_HSTDBG64S             HAL_HST_HSTDBG64S_MASK
#define HAL_HST_HSTDBG64S_READABLE    1
#define HAL_HST_HSTDBG64S_WRITABLE    1
#define HAL_HST_HSTDBG64S_SIGNED      0
#define HAL_HST_HSTDBG64S_ADR         0x10000B98

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

// Register BZCTL
#define HAL_HST_BZEN_REGISTER    (HAL_HSTREG_MAP->BZCTL)
#define HAL_HST_BZEN_LSB         0
#define HAL_HST_BZEN_MSB         0
#define HAL_HST_BZEN_MASK        0x00000001
#define HAL_HST_BZEN_TYPE        Bool
#define HAL_HST_BZEN             HAL_HST_BZEN_MASK
#define HAL_HST_BZEN_READABLE    1
#define HAL_HST_BZEN_WRITABLE    1
#define HAL_HST_BZEN_SIGNED      0
#define HAL_HST_BZEN_ADR         0x10000B98

// Register BZSTRPGL
#define HAL_HST_BZSTRPGL_REGISTER    (HAL_HSTREG_MAP->BZSTRPGL)
#define HAL_HST_BZSTRPGL_LSB         0
#define HAL_HST_BZSTRPGL_MSB         7
#define HAL_HST_BZSTRPGL_MASK        0x000000FF
#define HAL_HST_BZSTRPGL_TYPE        UByte
#define HAL_HST_BZSTRPGL             HAL_HST_BZSTRPGL_MASK
#define HAL_HST_BZSTRPGL_READABLE    1
#define HAL_HST_BZSTRPGL_WRITABLE    1
#define HAL_HST_BZSTRPGL_SIGNED      0
#define HAL_HST_BZSTRPGL_ADR         0x10000B99

// Register BZSTRPGH
#define HAL_HST_BZSTRPGH_REGISTER    (HAL_HSTREG_MAP->BZSTRPGH)
#define HAL_HST_BZSTRPGH_LSB         0
#define HAL_HST_BZSTRPGH_MSB         7
#define HAL_HST_BZSTRPGH_MASK        0x000000FF
#define HAL_HST_BZSTRPGH_TYPE        UByte
#define HAL_HST_BZSTRPGH             HAL_HST_BZSTRPGH_MASK
#define HAL_HST_BZSTRPGH_READABLE    1
#define HAL_HST_BZSTRPGH_WRITABLE    1
#define HAL_HST_BZSTRPGH_SIGNED      0
#define HAL_HST_BZSTRPGH_ADR         0x10000B9A

// Register BZPGLENL
#define HAL_HST_BZPGLENL_REGISTER    (HAL_HSTREG_MAP->BZPGLENL)
#define HAL_HST_BZPGLENL_LSB         0
#define HAL_HST_BZPGLENL_MSB         7
#define HAL_HST_BZPGLENL_MASK        0x000000FF
#define HAL_HST_BZPGLENL_TYPE        UByte
#define HAL_HST_BZPGLENL             HAL_HST_BZPGLENL_MASK
#define HAL_HST_BZPGLENL_READABLE    1
#define HAL_HST_BZPGLENL_WRITABLE    1
#define HAL_HST_BZPGLENL_SIGNED      0
#define HAL_HST_BZPGLENL_ADR         0x10000B9B

// Register BZPGLENH
#define HAL_HST_BZPGLENH_REGISTER    (HAL_HSTREG_MAP->BZPGLENH)
#define HAL_HST_BZPGLENH_LSB         0
#define HAL_HST_BZPGLENH_MSB         7
#define HAL_HST_BZPGLENH_MASK        0x000000FF
#define HAL_HST_BZPGLENH_TYPE        UByte
#define HAL_HST_BZPGLENH             HAL_HST_BZPGLENH_MASK
#define HAL_HST_BZPGLENH_READABLE    1
#define HAL_HST_BZPGLENH_WRITABLE    1
#define HAL_HST_BZPGLENH_SIGNED      0
#define HAL_HST_BZPGLENH_ADR         0x10000B9C

#endif // #if (CHIP_REV >= 0xC0)

// Register CSSSCRCTL
#define HAL_HST_CSSSCREN_REGISTER    (HAL_HSTREG_MAP->CSSSCRCTL)
#define HAL_HST_CSSSCREN_LSB         0
#define HAL_HST_CSSSCREN_MSB         0
#define HAL_HST_CSSSCREN_MASK        0x00000001
#define HAL_HST_CSSSCREN_TYPE        Bool
#define HAL_HST_CSSSCREN             HAL_HST_CSSSCREN_MASK
#define HAL_HST_CSSSCREN_READABLE    1
#define HAL_HST_CSSSCREN_WRITABLE    1
#define HAL_HST_CSSSCREN_SIGNED      0
#define HAL_HST_CSSSCREN_ADR         0x10000BA0

// Register CSSENCS
#define HAL_HST_CSSENCS_REGISTER    (HAL_HSTREG_MAP->CSSENCS)
#define HAL_HST_CSSENCS_LSB         0
#define HAL_HST_CSSENCS_MSB         7
#define HAL_HST_CSSENCS_MASK        0x000000FF
#define HAL_HST_CSSENCS_TYPE        UByte
#define HAL_HST_CSSENCS             HAL_HST_CSSENCS_MASK
#define HAL_HST_CSSENCS_READABLE    1
#define HAL_HST_CSSENCS_WRITABLE    1
#define HAL_HST_CSSENCS_SIGNED      0
#define HAL_HST_CSSENCS_ADR         0x10000BA1

// Registers CSSENCEL/CSSENCEH
#define HAL_HST_CSSENCE_REGISTER    (HAL_HSTREG_MAP->CSSENCEL)
#define HAL_HST_CSSENCE_LSB         0
#define HAL_HST_CSSENCE_MSB         15
#define HAL_HST_CSSENCE_MASK        0x0000FFFF
#define HAL_HST_CSSENCE_TYPE        UInt16
#define HAL_HST_CSSENCE             HAL_HST_CSSENCE_MASK
#define HAL_HST_CSSENCE_READABLE    1
#define HAL_HST_CSSENCE_WRITABLE    1
#define HAL_HST_CSSENCE_SIGNED      0
#define HAL_HST_CSSENCE_ADR         0x10000BA2

// Register AESSTATUS
#define HAL_HST_AESOUTAVL_REGISTER    (HAL_HSTREG_MAP->AESSTATUS)
#define HAL_HST_AESOUTAVL_LSB         2
#define HAL_HST_AESOUTAVL_MSB         2
#define HAL_HST_AESOUTAVL_MASK        0x00000004
#define HAL_HST_AESOUTAVL_TYPE        Bool
#define HAL_HST_AESOUTAVL             HAL_HST_AESOUTAVL_MASK
#define HAL_HST_AESOUTAVL_READABLE    1
#define HAL_HST_AESOUTAVL_WRITABLE    0
#define HAL_HST_AESOUTAVL_SIGNED      0
#define HAL_HST_AESOUTAVL_ADR         0x10000BB0

#define HAL_HST_ACCEPTIN_REGISTER    (HAL_HSTREG_MAP->AESSTATUS)
#define HAL_HST_ACCEPTIN_LSB         1
#define HAL_HST_ACCEPTIN_MSB         1
#define HAL_HST_ACCEPTIN_MASK        0x00000002
#define HAL_HST_ACCEPTIN_TYPE        Bool
#define HAL_HST_ACCEPTIN             HAL_HST_ACCEPTIN_MASK
#define HAL_HST_ACCEPTIN_READABLE    1
#define HAL_HST_ACCEPTIN_WRITABLE    0
#define HAL_HST_ACCEPTIN_SIGNED      0
#define HAL_HST_ACCEPTIN_ADR         0x10000BB0

#define HAL_HST_ACCEPTKEY_REGISTER    (HAL_HSTREG_MAP->AESSTATUS)
#define HAL_HST_ACCEPTKEY_LSB         0
#define HAL_HST_ACCEPTKEY_MSB         0
#define HAL_HST_ACCEPTKEY_MASK        0x00000001
#define HAL_HST_ACCEPTKEY_TYPE        Bool
#define HAL_HST_ACCEPTKEY             HAL_HST_ACCEPTKEY_MASK
#define HAL_HST_ACCEPTKEY_READABLE    1
#define HAL_HST_ACCEPTKEY_WRITABLE    0
#define HAL_HST_ACCEPTKEY_SIGNED      0
#define HAL_HST_ACCEPTKEY_ADR         0x10000BB0

// Register AESIN
#define HAL_HST_AESIN_REGISTER    (HAL_HSTREG_MAP->AESIN)
#define HAL_HST_AESIN_LSB         0
#define HAL_HST_AESIN_MSB         7
#define HAL_HST_AESIN_MASK        0x000000FF
#define HAL_HST_AESIN_TYPE        UByte
#define HAL_HST_AESIN             HAL_HST_AESIN_MASK
#define HAL_HST_AESIN_READABLE    1
#define HAL_HST_AESIN_WRITABLE    1
#define HAL_HST_AESIN_SIGNED      0
#define HAL_HST_AESIN_ADR         0x10000BB1

// Register AESOUT
#define HAL_HST_AESOUT_REGISTER    (HAL_HSTREG_MAP->AESOUT)
#define HAL_HST_AESOUT_LSB         0
#define HAL_HST_AESOUT_MSB         7
#define HAL_HST_AESOUT_MASK        0x000000FF
#define HAL_HST_AESOUT_TYPE        UByte
#define HAL_HST_AESOUT             HAL_HST_AESOUT_MASK
#define HAL_HST_AESOUT_READABLE    1
#define HAL_HST_AESOUT_WRITABLE    0
#define HAL_HST_AESOUT_SIGNED      0
#define HAL_HST_AESOUT_ADR         0x10000BB2

// Register AESKEY
#define HAL_HST_AESKEY_REGISTER    (HAL_HSTREG_MAP->AESKEY)
#define HAL_HST_AESKEY_LSB         0
#define HAL_HST_AESKEY_MSB         7
#define HAL_HST_AESKEY_MASK        0x000000FF
#define HAL_HST_AESKEY_TYPE        UByte
#define HAL_HST_AESKEY             HAL_HST_AESKEY_MASK
#define HAL_HST_AESKEY_READABLE    1
#define HAL_HST_AESKEY_WRITABLE    1
#define HAL_HST_AESKEY_SIGNED      0
#define HAL_HST_AESKEY_ADR         0x10000BB3

// Register AESCBC
#define HAL_HST_AESCBC_REGISTER    (HAL_HSTREG_MAP->AESCBC)
#define HAL_HST_AESCBC_LSB         0
#define HAL_HST_AESCBC_MSB         7
#define HAL_HST_AESCBC_MASK        0x000000FF
#define HAL_HST_AESCBC_TYPE        UByte
#define HAL_HST_AESCBC             HAL_HST_AESCBC_MASK
#define HAL_HST_AESCBC_READABLE    1
#define HAL_HST_AESCBC_WRITABLE    1
#define HAL_HST_AESCBC_SIGNED      0
#define HAL_HST_AESCBC_ADR         0x10000BB4

// Register AESCONFIG
#define HAL_HST_AESPTRRST_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_AESPTRRST_LSB         7
#define HAL_HST_AESPTRRST_MSB         7
#define HAL_HST_AESPTRRST_MASK        0x00000080
#define HAL_HST_AESPTRRST_TYPE        Bool
#define HAL_HST_AESPTRRST             HAL_HST_AESPTRRST_MASK
#define HAL_HST_AESPTRRST_READABLE    1
#define HAL_HST_AESPTRRST_WRITABLE    1
#define HAL_HST_AESPTRRST_SIGNED      0
#define HAL_HST_AESPTRRST_ADR         0x10000BB5

#define HAL_HST_SELAESKEY_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_SELAESKEY_LSB         6
#define HAL_HST_SELAESKEY_MSB         6
#define HAL_HST_SELAESKEY_MASK        0x00000040
#define HAL_HST_SELAESKEY_TYPE        halHST_AESKeySRC_t
#define HAL_HST_SELAESKEY             HAL_HST_SELAESKEY_MASK
#define HAL_HST_SELAESKEY_READABLE    1
#define HAL_HST_SELAESKEY_WRITABLE    1
#define HAL_HST_SELAESKEY_SIGNED      0
#define HAL_HST_SELAESKEY_ADR         0x10000BB5

#define HAL_HST_AESCLKGAT_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_AESCLKGAT_LSB         5
#define HAL_HST_AESCLKGAT_MSB         5
#define HAL_HST_AESCLKGAT_MASK        0x00000020
#define HAL_HST_AESCLKGAT_TYPE        Bool
#define HAL_HST_AESCLKGAT             HAL_HST_AESCLKGAT_MASK
#define HAL_HST_AESCLKGAT_READABLE    1
#define HAL_HST_AESCLKGAT_WRITABLE    1
#define HAL_HST_AESCLKGAT_SIGNED      0
#define HAL_HST_AESCLKGAT_ADR         0x10000BB5

#define HAL_HST_BUSENCPT_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_BUSENCPT_LSB         4
#define HAL_HST_BUSENCPT_MSB         4
#define HAL_HST_BUSENCPT_MASK        0x00000010
#define HAL_HST_BUSENCPT_TYPE        Bool
#define HAL_HST_BUSENCPT             HAL_HST_BUSENCPT_MASK
#define HAL_HST_BUSENCPT_READABLE    1
#define HAL_HST_BUSENCPT_WRITABLE    1
#define HAL_HST_BUSENCPT_SIGNED      0
#define HAL_HST_BUSENCPT_ADR         0x10000BB5

#define HAL_HST_BYPASS_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_BYPASS_LSB         3
#define HAL_HST_BYPASS_MSB         3
#define HAL_HST_BYPASS_MASK        0x00000008
#define HAL_HST_BYPASS_TYPE        Bool
#define HAL_HST_BYPASS             HAL_HST_BYPASS_MASK
#define HAL_HST_BYPASS_READABLE    1
#define HAL_HST_BYPASS_WRITABLE    1
#define HAL_HST_BYPASS_SIGNED      0
#define HAL_HST_BYPASS_ADR         0x10000BB5

#define HAL_HST_SUBCPUMOD_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_SUBCPUMOD_LSB         2
#define HAL_HST_SUBCPUMOD_MSB         2
#define HAL_HST_SUBCPUMOD_MASK        0x00000004
#define HAL_HST_SUBCPUMOD_TYPE        halHST_SUBCPUMOD_t
#define HAL_HST_SUBCPUMOD             HAL_HST_SUBCPUMOD_MASK
#define HAL_HST_SUBCPUMOD_READABLE    1
#define HAL_HST_SUBCPUMOD_WRITABLE    1
#define HAL_HST_SUBCPUMOD_SIGNED      0
#define HAL_HST_SUBCPUMOD_ADR         0x10000BB5

#define HAL_HST_CBCENABLE_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_CBCENABLE_LSB         1
#define HAL_HST_CBCENABLE_MSB         1
#define HAL_HST_CBCENABLE_MASK        0x00000002
#define HAL_HST_CBCENABLE_TYPE        halHST_CBChainMOD_t
#define HAL_HST_CBCENABLE             HAL_HST_CBCENABLE_MASK
#define HAL_HST_CBCENABLE_READABLE    1
#define HAL_HST_CBCENABLE_WRITABLE    1
#define HAL_HST_CBCENABLE_SIGNED      0
#define HAL_HST_CBCENABLE_ADR         0x10000BB5

#define HAL_HST_ENCRYPT_REGISTER    (HAL_HSTREG_MAP->AESCONFIG)
#define HAL_HST_ENCRYPT_LSB         0
#define HAL_HST_ENCRYPT_MSB         0
#define HAL_HST_ENCRYPT_MASK        0x00000001
#define HAL_HST_ENCRYPT_TYPE        halHST_AESEncMOD_t
#define HAL_HST_ENCRYPT             HAL_HST_ENCRYPT_MASK
#define HAL_HST_ENCRYPT_READABLE    1
#define HAL_HST_ENCRYPT_WRITABLE    1
#define HAL_HST_ENCRYPT_SIGNED      0
#define HAL_HST_ENCRYPT_ADR         0x10000BB5

// Register AESCTRL
#define HAL_HST_AESRDROM_REGISTER    (HAL_HSTREG_MAP->AESCTRL)
#define HAL_HST_AESRDROM_LSB         0
#define HAL_HST_AESRDROM_MSB         0
#define HAL_HST_AESRDROM_MASK        0x00000001
#define HAL_HST_AESRDROM_TYPE        Bool
#define HAL_HST_AESRDROM             HAL_HST_AESRDROM_MASK
#define HAL_HST_AESRDROM_READABLE    1
#define HAL_HST_AESRDROM_WRITABLE    1
#define HAL_HST_AESRDROM_SIGNED      0
#define HAL_HST_AESRDROM_ADR         0x10000BB6

// Register AESROMKEY
#define HAL_HST_ROMKEYWR_REGISTER    (HAL_HSTREG_MAP->AESROMKEY)
#define HAL_HST_ROMKEYWR_LSB         3
#define HAL_HST_ROMKEYWR_MSB         5
#define HAL_HST_ROMKEYWR_MASK        0x00000038
#define HAL_HST_ROMKEYWR_TYPE        UByte
#define HAL_HST_ROMKEYWR             HAL_HST_ROMKEYWR_MASK
#define HAL_HST_ROMKEYWR_READABLE    0
#define HAL_HST_ROMKEYWR_WRITABLE    1
#define HAL_HST_ROMKEYWR_SIGNED      0
#define HAL_HST_ROMKEYWR_ADR         0x10000BB7

#define HAL_HST_ROMKEYRO_REGISTER    (HAL_HSTREG_MAP->AESROMKEY)
#define HAL_HST_ROMKEYRO_LSB         3
#define HAL_HST_ROMKEYRO_MSB         5
#define HAL_HST_ROMKEYRO_MASK        0x00000038
#define HAL_HST_ROMKEYRO_TYPE        UByte
#define HAL_HST_ROMKEYRO             HAL_HST_ROMKEYRO_MASK
#define HAL_HST_ROMKEYRO_READABLE    1
#define HAL_HST_ROMKEYRO_WRITABLE    0
#define HAL_HST_ROMKEYRO_SIGNED      0
#define HAL_HST_ROMKEYRO_ADR         0x10000BB7

#define HAL_HST_ROMKEY_REGISTER    (HAL_HSTREG_MAP->AESROMKEY)
#define HAL_HST_ROMKEY_LSB         0
#define HAL_HST_ROMKEY_MSB         2
#define HAL_HST_ROMKEY_MASK        0x00000007
#define HAL_HST_ROMKEY_TYPE        UByte
#define HAL_HST_ROMKEY             HAL_HST_ROMKEY_MASK
#define HAL_HST_ROMKEY_READABLE    1
#define HAL_HST_ROMKEY_WRITABLE    1
#define HAL_HST_ROMKEY_SIGNED      0
#define HAL_HST_ROMKEY_ADR         0x10000BB7

// Register AESLENGTH
#define HAL_HST_AESLENGTH_REGISTER    (HAL_HSTREG_MAP->AESLENGTH)
#define HAL_HST_AESLENGTH_LSB         0
#define HAL_HST_AESLENGTH_MSB         7
#define HAL_HST_AESLENGTH_MASK        0x000000FF
#define HAL_HST_AESLENGTH_TYPE        UByte
#define HAL_HST_AESLENGTH             HAL_HST_AESLENGTH_MASK
#define HAL_HST_AESLENGTH_READABLE    1
#define HAL_HST_AESLENGTH_WRITABLE    1
#define HAL_HST_AESLENGTH_SIGNED      0
#define HAL_HST_AESLENGTH_ADR         0x10000BB8

// Register AESSTRTOF
#define HAL_HST_AESSTRTOF_REGISTER    (HAL_HSTREG_MAP->AESSTRTOF)
#define HAL_HST_AESSTRTOF_LSB         0
#define HAL_HST_AESSTRTOF_MSB         6
#define HAL_HST_AESSTRTOF_MASK        0x0000007F
#define HAL_HST_AESSTRTOF_TYPE        UByte
#define HAL_HST_AESSTRTOF             HAL_HST_AESSTRTOF_MASK
#define HAL_HST_AESSTRTOF_READABLE    1
#define HAL_HST_AESSTRTOF_WRITABLE    1
#define HAL_HST_AESSTRTOF_SIGNED      0
#define HAL_HST_AESSTRTOF_ADR         0x10000BB9

// Register DBECONFIG
#define HAL_HST_DBEAUTO_REGISTER    (HAL_HSTREG_MAP->DBECONFIG)
#define HAL_HST_DBEAUTO_LSB         1
#define HAL_HST_DBEAUTO_MSB         1
#define HAL_HST_DBEAUTO_MASK        0x00000002
#define HAL_HST_DBEAUTO_TYPE        Bool
#define HAL_HST_DBEAUTO             HAL_HST_DBEAUTO_MASK
#define HAL_HST_DBEAUTO_READABLE    1
#define HAL_HST_DBEAUTO_WRITABLE    1
#define HAL_HST_DBEAUTO_SIGNED      0
#define HAL_HST_DBEAUTO_ADR         0x10000BBA

#define HAL_HST_DBEENABLE_REGISTER    (HAL_HSTREG_MAP->DBECONFIG)
#define HAL_HST_DBEENABLE_LSB         0
#define HAL_HST_DBEENABLE_MSB         0
#define HAL_HST_DBEENABLE_MASK        0x00000001
#define HAL_HST_DBEENABLE_TYPE        Bool
#define HAL_HST_DBEENABLE             HAL_HST_DBEENABLE_MASK
#define HAL_HST_DBEENABLE_READABLE    1
#define HAL_HST_DBEENABLE_WRITABLE    1
#define HAL_HST_DBEENABLE_SIGNED      0
#define HAL_HST_DBEENABLE_ADR         0x10000BBA

// Register DBESADR
#define HAL_HST_DBESADR_REGISTER    (HAL_HSTREG_MAP->DBESADR)
#define HAL_HST_DBESADR_LSB         0
#define HAL_HST_DBESADR_MSB         7
#define HAL_HST_DBESADR_MASK        0x000000FF
#define HAL_HST_DBESADR_TYPE        UByte
#define HAL_HST_DBESADR             HAL_HST_DBESADR_MASK
#define HAL_HST_DBESADR_READABLE    1
#define HAL_HST_DBESADR_WRITABLE    1
#define HAL_HST_DBESADR_SIGNED      0
#define HAL_HST_DBESADR_ADR         0x10000BBB

// Register DBEWIDTH
#define HAL_HST_DBEWIDTH_REGISTER    (HAL_HSTREG_MAP->DBEWIDTH)
#define HAL_HST_DBEWIDTH_LSB         0
#define HAL_HST_DBEWIDTH_MSB         3
#define HAL_HST_DBEWIDTH_MASK        0x0000000F
#define HAL_HST_DBEWIDTH_TYPE        UByte
#define HAL_HST_DBEWIDTH             HAL_HST_DBEWIDTH_MASK
#define HAL_HST_DBEWIDTH_READABLE    1
#define HAL_HST_DBEWIDTH_WRITABLE    1
#define HAL_HST_DBEWIDTH_SIGNED      0
#define HAL_HST_DBEWIDTH_ADR         0x10000BBC

// Register DBE128KEY
#define HAL_HST_DBE128KEY_REGISTER    (HAL_HSTREG_MAP->DBE128KEY)
#define HAL_HST_DBE128KEY_LSB         0
#define HAL_HST_DBE128KEY_MSB         7
#define HAL_HST_DBE128KEY_MASK        0x000000FF
#define HAL_HST_DBE128KEY_TYPE        UByte
#define HAL_HST_DBE128KEY             HAL_HST_DBE128KEY_MASK
#define HAL_HST_DBE128KEY_READABLE    1
#define HAL_HST_DBE128KEY_WRITABLE    1
#define HAL_HST_DBE128KEY_SIGNED      0
#define HAL_HST_DBE128KEY_ADR         0x10000BBD

// Register BUSENCCTL
#define HAL_HST_BEFMAN_REGISTER    (HAL_HSTREG_MAP->BUSENCCTL)
#define HAL_HST_BEFMAN_LSB         1
#define HAL_HST_BEFMAN_MSB         1
#define HAL_HST_BEFMAN_MASK        0x00000002
#define HAL_HST_BEFMAN_TYPE        Bool
#define HAL_HST_BEFMAN             HAL_HST_BEFMAN_MASK
#define HAL_HST_BEFMAN_READABLE    1
#define HAL_HST_BEFMAN_WRITABLE    1
#define HAL_HST_BEFMAN_SIGNED      0
#define HAL_HST_BEFMAN_ADR         0x10000BC0

#define HAL_HST_BUSENCEN_REGISTER    (HAL_HSTREG_MAP->BUSENCCTL)
#define HAL_HST_BUSENCEN_LSB         0
#define HAL_HST_BUSENCEN_MSB         0
#define HAL_HST_BUSENCEN_MASK        0x00000001
#define HAL_HST_BUSENCEN_TYPE        Bool
#define HAL_HST_BUSENCEN             HAL_HST_BUSENCEN_MASK
#define HAL_HST_BUSENCEN_READABLE    1
#define HAL_HST_BUSENCEN_WRITABLE    1
#define HAL_HST_BUSENCEN_SIGNED      0
#define HAL_HST_BUSENCEN_ADR         0x10000BC0

// Register BEFST
#define HAL_HST_BEFSTATUS_REGISTER    (HAL_HSTREG_MAP->BEFST)
#define HAL_HST_BEFSTATUS_LSB         1
#define HAL_HST_BEFSTATUS_MSB         1
#define HAL_HST_BEFSTATUS_MASK        0x00000002
#define HAL_HST_BEFSTATUS_TYPE        Bool
#define HAL_HST_BEFSTATUS             HAL_HST_BEFSTATUS_MASK
#define HAL_HST_BEFSTATUS_READABLE    1
#define HAL_HST_BEFSTATUS_WRITABLE    0
#define HAL_HST_BEFSTATUS_SIGNED      0
#define HAL_HST_BEFSTATUS_ADR         0x10000BC1

#define HAL_HST_BUSENCST_REGISTER    (HAL_HSTREG_MAP->BEFST)
#define HAL_HST_BUSENCST_LSB         0
#define HAL_HST_BUSENCST_MSB         0
#define HAL_HST_BUSENCST_MASK        0x00000001
#define HAL_HST_BUSENCST_TYPE        Bool
#define HAL_HST_BUSENCST             HAL_HST_BUSENCST_MASK
#define HAL_HST_BUSENCST_READABLE    1
#define HAL_HST_BUSENCST_WRITABLE    0
#define HAL_HST_BUSENCST_SIGNED      0
#define HAL_HST_BUSENCST_ADR         0x10000BC1

// Registers BEFCDSADL/BEFCDSADM/BEFCDSADH
#define HAL_HST_BEFCDSAD_REGISTER    (HAL_HSTREG_MAP->BEFCDSADL)
#define HAL_HST_BEFCDSAD_LSB         0
#define HAL_HST_BEFCDSAD_MSB         23
#define HAL_HST_BEFCDSAD_MASK        0x00FFFFFF
#define HAL_HST_BEFCDSAD_TYPE        UInt32
#define HAL_HST_BEFCDSAD             HAL_HST_BEFCDSAD_MASK
#define HAL_HST_BEFCDSAD_READABLE    1
#define HAL_HST_BEFCDSAD_WRITABLE    1
#define HAL_HST_BEFCDSAD_SIGNED      0
#define HAL_HST_BEFCDSAD_ADR         0x10000BC4

// Registers BEFCDEADL/BEFCDEADM/BEFCDEADH
#define HAL_HST_BEFCDEAD_REGISTER    (HAL_HSTREG_MAP->BEFCDEADL)
#define HAL_HST_BEFCDEAD_LSB         0
#define HAL_HST_BEFCDEAD_MSB         23
#define HAL_HST_BEFCDEAD_MASK        0x00FFFFFF
#define HAL_HST_BEFCDEAD_TYPE        UInt32
#define HAL_HST_BEFCDEAD             HAL_HST_BEFCDEAD_MASK
#define HAL_HST_BEFCDEAD_READABLE    1
#define HAL_HST_BEFCDEAD_WRITABLE    1
#define HAL_HST_BEFCDEAD_SIGNED      0
#define HAL_HST_BEFCDEAD_ADR         0x10000BC8

// Register BEFBITOFF
#define HAL_HST_BEFBITOFF_REGISTER    (HAL_HSTREG_MAP->BEFBITOFF)
#define HAL_HST_BEFBITOFF_LSB         0
#define HAL_HST_BEFBITOFF_MSB         2
#define HAL_HST_BEFBITOFF_MASK        0x00000007
#define HAL_HST_BEFBITOFF_TYPE        halHST_BEFBitOffset_t
#define HAL_HST_BEFBITOFF             HAL_HST_BEFBITOFF_MASK
#define HAL_HST_BEFBITOFF_READABLE    1
#define HAL_HST_BEFBITOFF_WRITABLE    1
#define HAL_HST_BEFBITOFF_SIGNED      0
#define HAL_HST_BEFBITOFF_ADR         0x10000BCC

// Register BEFMAXSEC
#define HAL_HST_BEFMAXSEC_REGISTER    (HAL_HSTREG_MAP->BEFMAXSEC)
#define HAL_HST_BEFMAXSEC_LSB         0
#define HAL_HST_BEFMAXSEC_MSB         4
#define HAL_HST_BEFMAXSEC_MASK        0x0000001F
#define HAL_HST_BEFMAXSEC_TYPE        halHST_BEFMaxSecOffset_t
#define HAL_HST_BEFMAXSEC             HAL_HST_BEFMAXSEC_MASK
#define HAL_HST_BEFMAXSEC_READABLE    1
#define HAL_HST_BEFMAXSEC_WRITABLE    1
#define HAL_HST_BEFMAXSEC_SIGNED      0
#define HAL_HST_BEFMAXSEC_ADR         0x10000BCD

// Register BEFSECLEN
#define HAL_HST_BEFSECLEN_REGISTER    (HAL_HSTREG_MAP->BEFSECLEN)
#define HAL_HST_BEFSECLEN_LSB         0
#define HAL_HST_BEFSECLEN_MSB         7
#define HAL_HST_BEFSECLEN_MASK        0x000000FF
#define HAL_HST_BEFSECLEN_TYPE        halHST_BEFSecLen_t
#define HAL_HST_BEFSECLEN             HAL_HST_BEFSECLEN_MASK
#define HAL_HST_BEFSECLEN_READABLE    1
#define HAL_HST_BEFSECLEN_WRITABLE    1
#define HAL_HST_BEFSECLEN_SIGNED      0
#define HAL_HST_BEFSECLEN_ADR         0x10000BCE

// Registers BEFBLKLENL/BEFBLKLENH
#define HAL_HST_BEFBLKLEN_REGISTER    (HAL_HSTREG_MAP->BEFBLKLENL)
#define HAL_HST_BEFBLKLEN_LSB         0
#define HAL_HST_BEFBLKLEN_MSB         11
#define HAL_HST_BEFBLKLEN_MASK        0x00000FFF
#define HAL_HST_BEFBLKLEN_TYPE        halHST_BEFBlkLen_t
#define HAL_HST_BEFBLKLEN             HAL_HST_BEFBLKLEN_MASK
#define HAL_HST_BEFBLKLEN_READABLE    1
#define HAL_HST_BEFBLKLEN_WRITABLE    1
#define HAL_HST_BEFBLKLEN_SIGNED      0
#define HAL_HST_BEFBLKLEN_ADR         0x10000BD0

// Registers BEFCDCADL/BEFCDCADM/BEFCDCADH
#define HAL_HST_BEFCDCAD_REGISTER    (HAL_HSTREG_MAP->BEFCDCADL)
#define HAL_HST_BEFCDCAD_LSB         0
#define HAL_HST_BEFCDCAD_MSB         23
#define HAL_HST_BEFCDCAD_MASK        0x00FFFFFF
#define HAL_HST_BEFCDCAD_TYPE        UInt32
#define HAL_HST_BEFCDCAD             HAL_HST_BEFCDCAD_MASK
#define HAL_HST_BEFCDCAD_READABLE    1
#define HAL_HST_BEFCDCAD_WRITABLE    1
#define HAL_HST_BEFCDCAD_SIGNED      0
#define HAL_HST_BEFCDCAD_ADR         0x10000BD4

// Register BEFSECOFF
#define HAL_HST_BEFSECOFF_REGISTER    (HAL_HSTREG_MAP->BEFSECOFF)
#define HAL_HST_BEFSECOFF_LSB         0
#define HAL_HST_BEFSECOFF_MSB         4
#define HAL_HST_BEFSECOFF_MASK        0x0000001F
#define HAL_HST_BEFSECOFF_TYPE        UByte
#define HAL_HST_BEFSECOFF             HAL_HST_BEFSECOFF_MASK
#define HAL_HST_BEFSECOFF_READABLE    1
#define HAL_HST_BEFSECOFF_WRITABLE    1
#define HAL_HST_BEFSECOFF_SIGNED      0
#define HAL_HST_BEFSECOFF_ADR         0x10000BD8

#if (CHIP_REV >= 0xC0)

// Register BISTMODE
#define HAL_HST_BIST_L_REGISTER    (HAL_HSTREG_MAP->BISTMODE)
#define HAL_HST_BIST_L_LSB         4
#define HAL_HST_BIST_L_MSB         4
#define HAL_HST_BIST_L_MASK        0x00000010
#define HAL_HST_BIST_L_TYPE        Bool
#define HAL_HST_BIST_L             HAL_HST_BIST_L_MASK
#define HAL_HST_BIST_L_READABLE    1
#define HAL_HST_BIST_L_WRITABLE    1
#define HAL_HST_BIST_L_SIGNED      0
#define HAL_HST_BIST_L_ADR         0x10000BE0

#define HAL_HST_BIST_T_REGISTER    (HAL_HSTREG_MAP->BISTMODE)
#define HAL_HST_BIST_T_LSB         3
#define HAL_HST_BIST_T_MSB         3
#define HAL_HST_BIST_T_MASK        0x00000008
#define HAL_HST_BIST_T_TYPE        Bool
#define HAL_HST_BIST_T             HAL_HST_BIST_T_MASK
#define HAL_HST_BIST_T_READABLE    1
#define HAL_HST_BIST_T_WRITABLE    1
#define HAL_HST_BIST_T_SIGNED      0
#define HAL_HST_BIST_T_ADR         0x10000BE0

#define HAL_HST_BIST_A_REGISTER    (HAL_HSTREG_MAP->BISTMODE)
#define HAL_HST_BIST_A_LSB         2
#define HAL_HST_BIST_A_MSB         2
#define HAL_HST_BIST_A_MASK        0x00000004
#define HAL_HST_BIST_A_TYPE        Bool
#define HAL_HST_BIST_A             HAL_HST_BIST_A_MASK
#define HAL_HST_BIST_A_READABLE    1
#define HAL_HST_BIST_A_WRITABLE    1
#define HAL_HST_BIST_A_SIGNED      0
#define HAL_HST_BIST_A_ADR         0x10000BE0

#define HAL_HST_BIST_S_REGISTER    (HAL_HSTREG_MAP->BISTMODE)
#define HAL_HST_BIST_S_LSB         1
#define HAL_HST_BIST_S_MSB         1
#define HAL_HST_BIST_S_MASK        0x00000002
#define HAL_HST_BIST_S_TYPE        Bool
#define HAL_HST_BIST_S             HAL_HST_BIST_S_MASK
#define HAL_HST_BIST_S_READABLE    1
#define HAL_HST_BIST_S_WRITABLE    1
#define HAL_HST_BIST_S_SIGNED      0
#define HAL_HST_BIST_S_ADR         0x10000BE0

#define HAL_HST_BIST_P_REGISTER    (HAL_HSTREG_MAP->BISTMODE)
#define HAL_HST_BIST_P_LSB         0
#define HAL_HST_BIST_P_MSB         0
#define HAL_HST_BIST_P_MASK        0x00000001
#define HAL_HST_BIST_P_TYPE        Bool
#define HAL_HST_BIST_P             HAL_HST_BIST_P_MASK
#define HAL_HST_BIST_P_READABLE    1
#define HAL_HST_BIST_P_WRITABLE    1
#define HAL_HST_BIST_P_SIGNED      0
#define HAL_HST_BIST_P_ADR         0x10000BE0

// Register DATAWD1L
#define HAL_HST_DATAWD1L_REGISTER    (HAL_HSTREG_MAP->DATAWD1L)
#define HAL_HST_DATAWD1L_LSB         0
#define HAL_HST_DATAWD1L_MSB         7
#define HAL_HST_DATAWD1L_MASK        0x000000FF
#define HAL_HST_DATAWD1L_TYPE        UByte
#define HAL_HST_DATAWD1L             HAL_HST_DATAWD1L_MASK
#define HAL_HST_DATAWD1L_READABLE    1
#define HAL_HST_DATAWD1L_WRITABLE    1
#define HAL_HST_DATAWD1L_SIGNED      0
#define HAL_HST_DATAWD1L_ADR         0x10000BE1

// Register DATAWD1M
#define HAL_HST_DATAWD1M_REGISTER    (HAL_HSTREG_MAP->DATAWD1M)
#define HAL_HST_DATAWD1M_LSB         0
#define HAL_HST_DATAWD1M_MSB         7
#define HAL_HST_DATAWD1M_MASK        0x000000FF
#define HAL_HST_DATAWD1M_TYPE        UByte
#define HAL_HST_DATAWD1M             HAL_HST_DATAWD1M_MASK
#define HAL_HST_DATAWD1M_READABLE    1
#define HAL_HST_DATAWD1M_WRITABLE    1
#define HAL_HST_DATAWD1M_SIGNED      0
#define HAL_HST_DATAWD1M_ADR         0x10000BE2

// Register DATAWD1H
#define HAL_HST_DATAWD1H_REGISTER    (HAL_HSTREG_MAP->DATAWD1H)
#define HAL_HST_DATAWD1H_LSB         0
#define HAL_HST_DATAWD1H_MSB         7
#define HAL_HST_DATAWD1H_MASK        0x000000FF
#define HAL_HST_DATAWD1H_TYPE        UByte
#define HAL_HST_DATAWD1H             HAL_HST_DATAWD1H_MASK
#define HAL_HST_DATAWD1H_READABLE    1
#define HAL_HST_DATAWD1H_WRITABLE    1
#define HAL_HST_DATAWD1H_SIGNED      0
#define HAL_HST_DATAWD1H_ADR         0x10000BE3

// Register DATAWD1U
#define HAL_HST_DATAWD1U_REGISTER    (HAL_HSTREG_MAP->DATAWD1U)
#define HAL_HST_DATAWD1U_LSB         0
#define HAL_HST_DATAWD1U_MSB         7
#define HAL_HST_DATAWD1U_MASK        0x000000FF
#define HAL_HST_DATAWD1U_TYPE        UByte
#define HAL_HST_DATAWD1U             HAL_HST_DATAWD1U_MASK
#define HAL_HST_DATAWD1U_READABLE    1
#define HAL_HST_DATAWD1U_WRITABLE    1
#define HAL_HST_DATAWD1U_SIGNED      0
#define HAL_HST_DATAWD1U_ADR         0x10000BE4

// Register DATAWD2L
#define HAL_HST_DATAWD2L_REGISTER    (HAL_HSTREG_MAP->DATAWD2L)
#define HAL_HST_DATAWD2L_LSB         0
#define HAL_HST_DATAWD2L_MSB         7
#define HAL_HST_DATAWD2L_MASK        0x000000FF
#define HAL_HST_DATAWD2L_TYPE        UByte
#define HAL_HST_DATAWD2L             HAL_HST_DATAWD2L_MASK
#define HAL_HST_DATAWD2L_READABLE    1
#define HAL_HST_DATAWD2L_WRITABLE    1
#define HAL_HST_DATAWD2L_SIGNED      0
#define HAL_HST_DATAWD2L_ADR         0x10000BE5

// Register DATAWD2M
#define HAL_HST_DATAWD2M_REGISTER    (HAL_HSTREG_MAP->DATAWD2M)
#define HAL_HST_DATAWD2M_LSB         0
#define HAL_HST_DATAWD2M_MSB         7
#define HAL_HST_DATAWD2M_MASK        0x000000FF
#define HAL_HST_DATAWD2M_TYPE        UByte
#define HAL_HST_DATAWD2M             HAL_HST_DATAWD2M_MASK
#define HAL_HST_DATAWD2M_READABLE    1
#define HAL_HST_DATAWD2M_WRITABLE    1
#define HAL_HST_DATAWD2M_SIGNED      0
#define HAL_HST_DATAWD2M_ADR         0x10000BE6

// Register DATAWD2H
#define HAL_HST_DATAWD2H_REGISTER    (HAL_HSTREG_MAP->DATAWD2H)
#define HAL_HST_DATAWD2H_LSB         0
#define HAL_HST_DATAWD2H_MSB         7
#define HAL_HST_DATAWD2H_MASK        0x000000FF
#define HAL_HST_DATAWD2H_TYPE        UByte
#define HAL_HST_DATAWD2H             HAL_HST_DATAWD2H_MASK
#define HAL_HST_DATAWD2H_READABLE    1
#define HAL_HST_DATAWD2H_WRITABLE    1
#define HAL_HST_DATAWD2H_SIGNED      0
#define HAL_HST_DATAWD2H_ADR         0x10000BE7

// Register DATAWD2U
#define HAL_HST_DATAWD2U_REGISTER    (HAL_HSTREG_MAP->DATAWD2U)
#define HAL_HST_DATAWD2U_LSB         0
#define HAL_HST_DATAWD2U_MSB         7
#define HAL_HST_DATAWD2U_MASK        0x000000FF
#define HAL_HST_DATAWD2U_TYPE        UByte
#define HAL_HST_DATAWD2U             HAL_HST_DATAWD2U_MASK
#define HAL_HST_DATAWD2U_READABLE    1
#define HAL_HST_DATAWD2U_WRITABLE    1
#define HAL_HST_DATAWD2U_SIGNED      0
#define HAL_HST_DATAWD2U_ADR         0x10000BE8

// Register DTWDLENL
#define HAL_HST_DTWDLENL_REGISTER    (HAL_HSTREG_MAP->DTWDLENL)
#define HAL_HST_DTWDLENL_LSB         0
#define HAL_HST_DTWDLENL_MSB         7
#define HAL_HST_DTWDLENL_MASK        0x000000FF
#define HAL_HST_DTWDLENL_TYPE        UByte
#define HAL_HST_DTWDLENL             HAL_HST_DTWDLENL_MASK
#define HAL_HST_DTWDLENL_READABLE    1
#define HAL_HST_DTWDLENL_WRITABLE    1
#define HAL_HST_DTWDLENL_SIGNED      0
#define HAL_HST_DTWDLENL_ADR         0x10000BE9

// Register DTWDLENH
#define HAL_HST_DTWDLENH_REGISTER    (HAL_HSTREG_MAP->DTWDLENH)
#define HAL_HST_DTWDLENH_LSB         0
#define HAL_HST_DTWDLENH_MSB         3
#define HAL_HST_DTWDLENH_MASK        0x0000000F
#define HAL_HST_DTWDLENH_TYPE        UByte
#define HAL_HST_DTWDLENH             HAL_HST_DTWDLENH_MASK
#define HAL_HST_DTWDLENH_READABLE    1
#define HAL_HST_DTWDLENH_WRITABLE    1
#define HAL_HST_DTWDLENH_SIGNED      0
#define HAL_HST_DTWDLENH_ADR         0x10000BEA

// Register BISTCTL
#define HAL_HST_FORCEBIST_REGISTER    (HAL_HSTREG_MAP->BISTCTL)
#define HAL_HST_FORCEBIST_LSB         4
#define HAL_HST_FORCEBIST_MSB         4
#define HAL_HST_FORCEBIST_MASK        0x00000010
#define HAL_HST_FORCEBIST_TYPE        Bool
#define HAL_HST_FORCEBIST             HAL_HST_FORCEBIST_MASK
#define HAL_HST_FORCEBIST_READABLE    1
#define HAL_HST_FORCEBIST_WRITABLE    1
#define HAL_HST_FORCEBIST_SIGNED      0
#define HAL_HST_FORCEBIST_ADR         0x10000BEB

#define HAL_HST_BISTSTRT_REGISTER    (HAL_HSTREG_MAP->BISTCTL)
#define HAL_HST_BISTSTRT_LSB         3
#define HAL_HST_BISTSTRT_MSB         3
#define HAL_HST_BISTSTRT_MASK        0x00000008
#define HAL_HST_BISTSTRT_TYPE        Bool
#define HAL_HST_BISTSTRT             HAL_HST_BISTSTRT_MASK
#define HAL_HST_BISTSTRT_READABLE    1
#define HAL_HST_BISTSTRT_WRITABLE    1
#define HAL_HST_BISTSTRT_SIGNED      0
#define HAL_HST_BISTSTRT_ADR         0x10000BEB

#define HAL_HST_BISTEND_REGISTER    (HAL_HSTREG_MAP->BISTCTL)
#define HAL_HST_BISTEND_LSB         2
#define HAL_HST_BISTEND_MSB         2
#define HAL_HST_BISTEND_MASK        0x00000004
#define HAL_HST_BISTEND_TYPE        Bool
#define HAL_HST_BISTEND             HAL_HST_BISTEND_MASK
#define HAL_HST_BISTEND_READABLE    1
#define HAL_HST_BISTEND_WRITABLE    1
#define HAL_HST_BISTEND_SIGNED      0
#define HAL_HST_BISTEND_ADR         0x10000BEB

#define HAL_HST_BISTLOK_REGISTER    (HAL_HSTREG_MAP->BISTCTL)
#define HAL_HST_BISTLOK_LSB         1
#define HAL_HST_BISTLOK_MSB         1
#define HAL_HST_BISTLOK_MASK        0x00000002
#define HAL_HST_BISTLOK_TYPE        Bool
#define HAL_HST_BISTLOK             HAL_HST_BISTLOK_MASK
#define HAL_HST_BISTLOK_READABLE    1
#define HAL_HST_BISTLOK_WRITABLE    1
#define HAL_HST_BISTLOK_SIGNED      0
#define HAL_HST_BISTLOK_ADR         0x10000BEB

#define HAL_HST_BISTDONE_REGISTER    (HAL_HSTREG_MAP->BISTCTL)
#define HAL_HST_BISTDONE_LSB         0
#define HAL_HST_BISTDONE_MSB         0
#define HAL_HST_BISTDONE_MASK        0x00000001
#define HAL_HST_BISTDONE_TYPE        Bool
#define HAL_HST_BISTDONE             HAL_HST_BISTDONE_MASK
#define HAL_HST_BISTDONE_READABLE    1
#define HAL_HST_BISTDONE_WRITABLE    1
#define HAL_HST_BISTDONE_SIGNED      0
#define HAL_HST_BISTDONE_ADR         0x10000BEB

// Registers ERRCNTL/ERRCNTH
#define HAL_HST_ERRCNT_REGISTER    (HAL_HSTREG_MAP->ERRCNTL)
#define HAL_HST_ERRCNT_LSB         0
#define HAL_HST_ERRCNT_MSB         15
#define HAL_HST_ERRCNT_MASK        0x0000FFFF
#define HAL_HST_ERRCNT_TYPE        UInt16
#define HAL_HST_ERRCNT             HAL_HST_ERRCNT_MASK
#define HAL_HST_ERRCNT_READABLE    1
#define HAL_HST_ERRCNT_WRITABLE    1
#define HAL_HST_ERRCNT_SIGNED      0
#define HAL_HST_ERRCNT_ADR         0x10000BEC

#endif // #if (CHIP_REV >= 0xC0)

// Register PPHYCFGL
#define HAL_HST_TXPREMPSW_REGISTER    (HAL_HSTREG_MAP->PPHYCFGL)
#define HAL_HST_TXPREMPSW_LSB         5
#define HAL_HST_TXPREMPSW_MSB         7
#define HAL_HST_TXPREMPSW_MASK        0x000000E0
#define HAL_HST_TXPREMPSW_TYPE        UByte
#define HAL_HST_TXPREMPSW             HAL_HST_TXPREMPSW_MASK
#define HAL_HST_TXPREMPSW_READABLE    1
#define HAL_HST_TXPREMPSW_WRITABLE    1
#define HAL_HST_TXPREMPSW_SIGNED      0
#define HAL_HST_TXPREMPSW_ADR         0x10000BF0

#define HAL_HST_TXMAINSW_REGISTER    (HAL_HSTREG_MAP->PPHYCFGL)
#define HAL_HST_TXMAINSW_LSB         0
#define HAL_HST_TXMAINSW_MSB         4
#define HAL_HST_TXMAINSW_MASK        0x0000001F
#define HAL_HST_TXMAINSW_TYPE        UByte
#define HAL_HST_TXMAINSW             HAL_HST_TXMAINSW_MASK
#define HAL_HST_TXMAINSW_READABLE    1
#define HAL_HST_TXMAINSW_WRITABLE    1
#define HAL_HST_TXMAINSW_SIGNED      0
#define HAL_HST_TXMAINSW_ADR         0x10000BF0

// Register PPHYCFGM
#define HAL_HST_SATTXBST_REGISTER    (HAL_HSTREG_MAP->PPHYCFGM)
#define HAL_HST_SATTXBST_LSB         7
#define HAL_HST_SATTXBST_MSB         7
#define HAL_HST_SATTXBST_MASK        0x00000080
#define HAL_HST_SATTXBST_TYPE        Bool
#define HAL_HST_SATTXBST             HAL_HST_SATTXBST_MASK
#define HAL_HST_SATTXBST_READABLE    1
#define HAL_HST_SATTXBST_WRITABLE    1
#define HAL_HST_SATTXBST_SIGNED      0
#define HAL_HST_SATTXBST_ADR         0x10000BF1

#define HAL_HST_TXSLEWCTL_REGISTER    (HAL_HSTREG_MAP->PPHYCFGM)
#define HAL_HST_TXSLEWCTL_LSB         6
#define HAL_HST_TXSLEWCTL_MSB         6
#define HAL_HST_TXSLEWCTL_MASK        0x00000040
#define HAL_HST_TXSLEWCTL_TYPE        Bool
#define HAL_HST_TXSLEWCTL             HAL_HST_TXSLEWCTL_MASK
#define HAL_HST_TXSLEWCTL_READABLE    1
#define HAL_HST_TXSLEWCTL_WRITABLE    1
#define HAL_HST_TXSLEWCTL_SIGNED      0
#define HAL_HST_TXSLEWCTL_ADR         0x10000BF1

#define HAL_HST_TXPREMPEN_REGISTER    (HAL_HSTREG_MAP->PPHYCFGM)
#define HAL_HST_TXPREMPEN_LSB         5
#define HAL_HST_TXPREMPEN_MSB         5
#define HAL_HST_TXPREMPEN_MASK        0x00000020
#define HAL_HST_TXPREMPEN_TYPE        Bool
#define HAL_HST_TXPREMPEN             HAL_HST_TXPREMPEN_MASK
#define HAL_HST_TXPREMPEN_READABLE    1
#define HAL_HST_TXPREMPEN_WRITABLE    1
#define HAL_HST_TXPREMPEN_SIGNED      0
#define HAL_HST_TXPREMPEN_ADR         0x10000BF1

#define HAL_HST_SATTXINV_REGISTER    (HAL_HSTREG_MAP->PPHYCFGM)
#define HAL_HST_SATTXINV_LSB         4
#define HAL_HST_SATTXINV_MSB         4
#define HAL_HST_SATTXINV_MASK        0x00000010
#define HAL_HST_SATTXINV_TYPE        Bool
#define HAL_HST_SATTXINV             HAL_HST_SATTXINV_MASK
#define HAL_HST_SATTXINV_READABLE    1
#define HAL_HST_SATTXINV_WRITABLE    1
#define HAL_HST_SATTXINV_SIGNED      0
#define HAL_HST_SATTXINV_ADR         0x10000BF1

#define HAL_HST_RXDPLLDEB_REGISTER    (HAL_HSTREG_MAP->PPHYCFGM)
#define HAL_HST_RXDPLLDEB_LSB         1
#define HAL_HST_RXDPLLDEB_MSB         3
#define HAL_HST_RXDPLLDEB_MASK        0x0000000E
#define HAL_HST_RXDPLLDEB_TYPE        UByte
#define HAL_HST_RXDPLLDEB             HAL_HST_RXDPLLDEB_MASK
#define HAL_HST_RXDPLLDEB_READABLE    1
#define HAL_HST_RXDPLLDEB_WRITABLE    1
#define HAL_HST_RXDPLLDEB_SIGNED      0
#define HAL_HST_RXDPLLDEB_ADR         0x10000BF1

#define HAL_HST_DLPBKFIX_REGISTER    (HAL_HSTREG_MAP->PPHYCFGM)
#define HAL_HST_DLPBKFIX_LSB         0
#define HAL_HST_DLPBKFIX_MSB         0
#define HAL_HST_DLPBKFIX_MASK        0x00000001
#define HAL_HST_DLPBKFIX_TYPE        Bool
#define HAL_HST_DLPBKFIX             HAL_HST_DLPBKFIX_MASK
#define HAL_HST_DLPBKFIX_READABLE    1
#define HAL_HST_DLPBKFIX_WRITABLE    1
#define HAL_HST_DLPBKFIX_SIGNED      0
#define HAL_HST_DLPBKFIX_ADR         0x10000BF1

// Register PPHYCFGH
#define HAL_HST_SATEQPARM_REGISTER    (HAL_HSTREG_MAP->PPHYCFGH)
#define HAL_HST_SATEQPARM_LSB         0
#define HAL_HST_SATEQPARM_MSB         7
#define HAL_HST_SATEQPARM_MASK        0x000000FF
#define HAL_HST_SATEQPARM_TYPE        UByte
#define HAL_HST_SATEQPARM             HAL_HST_SATEQPARM_MASK
#define HAL_HST_SATEQPARM_READABLE    1
#define HAL_HST_SATEQPARM_WRITABLE    1
#define HAL_HST_SATEQPARM_SIGNED      0
#define HAL_HST_SATEQPARM_ADR         0x10000BF2

// Register GPHYCFGL

#if (CHIP_REV <= 0xB3)

#define HAL_HST_SATPLLCFG_REGISTER    (HAL_HSTREG_MAP->GPHYCFGL)
#define HAL_HST_SATPLLCFG_LSB         6
#define HAL_HST_SATPLLCFG_MSB         7
#define HAL_HST_SATPLLCFG_MASK        0x000000C0
#define HAL_HST_SATPLLCFG_TYPE        UByte
#define HAL_HST_SATPLLCFG             HAL_HST_SATPLLCFG_MASK
#define HAL_HST_SATPLLCFG_READABLE    1
#define HAL_HST_SATPLLCFG_WRITABLE    1
#define HAL_HST_SATPLLCFG_SIGNED      0
#define HAL_HST_SATPLLCFG_ADR         0x10000BF4

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_HST_SATPLLBW_REGISTER    (HAL_HSTREG_MAP->GPHYCFGL)
#define HAL_HST_SATPLLBW_LSB         7
#define HAL_HST_SATPLLBW_MSB         7
#define HAL_HST_SATPLLBW_MASK        0x00000080
#define HAL_HST_SATPLLBW_TYPE        halHST_SATPLLBW_t
#define HAL_HST_SATPLLBW             HAL_HST_SATPLLBW_MASK
#define HAL_HST_SATPLLBW_READABLE    1
#define HAL_HST_SATPLLBW_WRITABLE    1
#define HAL_HST_SATPLLBW_SIGNED      0
#define HAL_HST_SATPLLBW_ADR         0x10000BF4

#define HAL_HST_SSCDEVSEL_REGISTER    (HAL_HSTREG_MAP->GPHYCFGL)
#define HAL_HST_SSCDEVSEL_LSB         6
#define HAL_HST_SSCDEVSEL_MSB         6
#define HAL_HST_SSCDEVSEL_MASK        0x00000040
#define HAL_HST_SSCDEVSEL_TYPE        halHST_SSCDEVSEL_t
#define HAL_HST_SSCDEVSEL             HAL_HST_SSCDEVSEL_MASK
#define HAL_HST_SSCDEVSEL_READABLE    1
#define HAL_HST_SSCDEVSEL_WRITABLE    1
#define HAL_HST_SSCDEVSEL_SIGNED      0
#define HAL_HST_SSCDEVSEL_ADR         0x10000BF4

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_HST_SATPLLTST_REGISTER    (HAL_HSTREG_MAP->GPHYCFGL)
#define HAL_HST_SATPLLTST_LSB         5
#define HAL_HST_SATPLLTST_MSB         5
#define HAL_HST_SATPLLTST_MASK        0x00000020
#define HAL_HST_SATPLLTST_TYPE        Bool
#define HAL_HST_SATPLLTST             HAL_HST_SATPLLTST_MASK
#define HAL_HST_SATPLLTST_READABLE    1
#define HAL_HST_SATPLLTST_WRITABLE    1
#define HAL_HST_SATPLLTST_SIGNED      0
#define HAL_HST_SATPLLTST_ADR         0x10000BF4

#define HAL_HST_REFCLKMOD_REGISTER    (HAL_HSTREG_MAP->GPHYCFGL)
#define HAL_HST_REFCLKMOD_LSB         4
#define HAL_HST_REFCLKMOD_MSB         4
#define HAL_HST_REFCLKMOD_MASK        0x00000010
#define HAL_HST_REFCLKMOD_TYPE        halHST_RefClkMOD_t
#define HAL_HST_REFCLKMOD             HAL_HST_REFCLKMOD_MASK
#define HAL_HST_REFCLKMOD_READABLE    1
#define HAL_HST_REFCLKMOD_WRITABLE    1
#define HAL_HST_REFCLKMOD_SIGNED      0
#define HAL_HST_REFCLKMOD_ADR         0x10000BF4

#define HAL_HST_SSCCTL_REGISTER    (HAL_HSTREG_MAP->GPHYCFGL)
#define HAL_HST_SSCCTL_LSB         0
#define HAL_HST_SSCCTL_MSB         3
#define HAL_HST_SSCCTL_MASK        0x0000000F
#define HAL_HST_SSCCTL_TYPE        halHST_SSCCtl_t
#define HAL_HST_SSCCTL             HAL_HST_SSCCTL_MASK
#define HAL_HST_SSCCTL_READABLE    1
#define HAL_HST_SSCCTL_WRITABLE    1
#define HAL_HST_SSCCTL_SIGNED      0
#define HAL_HST_SSCCTL_ADR         0x10000BF4

// Register GPHYCFGH

#if (CHIP_REV <= 0xB3)

#define HAL_HST_SATCLKIN_REGISTER    (HAL_HSTREG_MAP->GPHYCFGH)
#define HAL_HST_SATCLKIN_LSB         7
#define HAL_HST_SATCLKIN_MSB         7
#define HAL_HST_SATCLKIN_MASK        0x00000080
#define HAL_HST_SATCLKIN_TYPE        halHST_RefClkIN_t
#define HAL_HST_SATCLKIN             HAL_HST_SATCLKIN_MASK
#define HAL_HST_SATCLKIN_READABLE    1
#define HAL_HST_SATCLKIN_WRITABLE    1
#define HAL_HST_SATCLKIN_SIGNED      0
#define HAL_HST_SATCLKIN_ADR         0x10000BF5

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_HST_REXT_REGISTER    (HAL_HSTREG_MAP->GPHYCFGH)
#define HAL_HST_REXT_LSB         7
#define HAL_HST_REXT_MSB         7
#define HAL_HST_REXT_MASK        0x00000080
#define HAL_HST_REXT_TYPE        halHST_REXT_t
#define HAL_HST_REXT             HAL_HST_REXT_MASK
#define HAL_HST_REXT_READABLE    1
#define HAL_HST_REXT_WRITABLE    1
#define HAL_HST_REXT_SIGNED      0
#define HAL_HST_REXT_ADR         0x10000BF5

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_HST_SQBIAS_REGISTER    (HAL_HSTREG_MAP->GPHYCFGH)
#define HAL_HST_SQBIAS_LSB         5
#define HAL_HST_SQBIAS_MSB         6
#define HAL_HST_SQBIAS_MASK        0x00000060
#define HAL_HST_SQBIAS_TYPE        halHST_SqBias_t
#define HAL_HST_SQBIAS             HAL_HST_SQBIAS_MASK
#define HAL_HST_SQBIAS_READABLE    1
#define HAL_HST_SQBIAS_WRITABLE    1
#define HAL_HST_SQBIAS_SIGNED      0
#define HAL_HST_SQBIAS_ADR         0x10000BF5

#define HAL_HST_CHIPBIAS_REGISTER    (HAL_HSTREG_MAP->GPHYCFGH)
#define HAL_HST_CHIPBIAS_LSB         3
#define HAL_HST_CHIPBIAS_MSB         4
#define HAL_HST_CHIPBIAS_MASK        0x00000018
#define HAL_HST_CHIPBIAS_TYPE        halHST_ChipBias_t
#define HAL_HST_CHIPBIAS             HAL_HST_CHIPBIAS_MASK
#define HAL_HST_CHIPBIAS_READABLE    1
#define HAL_HST_CHIPBIAS_WRITABLE    1
#define HAL_HST_CHIPBIAS_SIGNED      0
#define HAL_HST_CHIPBIAS_ADR         0x10000BF5

#define HAL_HST_SATPLLBIS_REGISTER    (HAL_HSTREG_MAP->GPHYCFGH)
#define HAL_HST_SATPLLBIS_LSB         0
#define HAL_HST_SATPLLBIS_MSB         2
#define HAL_HST_SATPLLBIS_MASK        0x00000007
#define HAL_HST_SATPLLBIS_TYPE        halHST_PLLCPBias_t
#define HAL_HST_SATPLLBIS             HAL_HST_SATPLLBIS_MASK
#define HAL_HST_SATPLLBIS_READABLE    1
#define HAL_HST_SATPLLBIS_WRITABLE    1
#define HAL_HST_SATPLLBIS_SIGNED      0
#define HAL_HST_SATPLLBIS_ADR         0x10000BF5

// Register PHYCTRL
#define HAL_HST_TXSPEED_REGISTER    (HAL_HSTREG_MAP->PHYCTRL)
#define HAL_HST_TXSPEED_LSB         4
#define HAL_HST_TXSPEED_MSB         4
#define HAL_HST_TXSPEED_MASK        0x00000010
#define HAL_HST_TXSPEED_TYPE        Bool
#define HAL_HST_TXSPEED             HAL_HST_TXSPEED_MASK
#define HAL_HST_TXSPEED_READABLE    1
#define HAL_HST_TXSPEED_WRITABLE    1
#define HAL_HST_TXSPEED_SIGNED      0
#define HAL_HST_TXSPEED_ADR         0x10000BF6

#define HAL_HST_RXSPEED_REGISTER    (HAL_HSTREG_MAP->PHYCTRL)
#define HAL_HST_RXSPEED_LSB         3
#define HAL_HST_RXSPEED_MSB         3
#define HAL_HST_RXSPEED_MASK        0x00000008
#define HAL_HST_RXSPEED_TYPE        Bool
#define HAL_HST_RXSPEED             HAL_HST_RXSPEED_MASK
#define HAL_HST_RXSPEED_READABLE    1
#define HAL_HST_RXSPEED_WRITABLE    1
#define HAL_HST_RXSPEED_SIGNED      0
#define HAL_HST_RXSPEED_ADR         0x10000BF6

#define HAL_HST_CHPD_REGISTER    (HAL_HSTREG_MAP->PHYCTRL)
#define HAL_HST_CHPD_LSB         2
#define HAL_HST_CHPD_MSB         2
#define HAL_HST_CHPD_MASK        0x00000004
#define HAL_HST_CHPD_TYPE        Bool
#define HAL_HST_CHPD             HAL_HST_CHPD_MASK
#define HAL_HST_CHPD_READABLE    1
#define HAL_HST_CHPD_WRITABLE    1
#define HAL_HST_CHPD_SIGNED      0
#define HAL_HST_CHPD_ADR         0x10000BF6

#define HAL_HST_CHRESET_REGISTER    (HAL_HSTREG_MAP->PHYCTRL)
#define HAL_HST_CHRESET_LSB         1
#define HAL_HST_CHRESET_MSB         1
#define HAL_HST_CHRESET_MASK        0x00000002
#define HAL_HST_CHRESET_TYPE        Bool
#define HAL_HST_CHRESET             HAL_HST_CHRESET_MASK
#define HAL_HST_CHRESET_READABLE    1
#define HAL_HST_CHRESET_WRITABLE    1
#define HAL_HST_CHRESET_SIGNED      0
#define HAL_HST_CHRESET_ADR         0x10000BF6

#define HAL_HST_SATAMODE_REGISTER    (HAL_HSTREG_MAP->PHYCTRL)
#define HAL_HST_SATAMODE_LSB         0
#define HAL_HST_SATAMODE_MSB         0
#define HAL_HST_SATAMODE_MASK        0x00000001
#define HAL_HST_SATAMODE_TYPE        Bool
#define HAL_HST_SATAMODE             HAL_HST_SATAMODE_MASK
#define HAL_HST_SATAMODE_READABLE    1
#define HAL_HST_SATAMODE_WRITABLE    1
#define HAL_HST_SATAMODE_SIGNED      0
#define HAL_HST_SATAMODE_ADR         0x10000BF6

#if (CHIP_REV >= 0xC0)

// Register HSTDBGMXL
#define HAL_HST_HSTDBGMXLEN_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXL)
#define HAL_HST_HSTDBGMXLEN_LSB         7
#define HAL_HST_HSTDBGMXLEN_MSB         7
#define HAL_HST_HSTDBGMXLEN_MASK        0x00000080
#define HAL_HST_HSTDBGMXLEN_TYPE        Bool
#define HAL_HST_HSTDBGMXLEN             HAL_HST_HSTDBGMXLEN_MASK
#define HAL_HST_HSTDBGMXLEN_READABLE    1
#define HAL_HST_HSTDBGMXLEN_WRITABLE    1
#define HAL_HST_HSTDBGMXLEN_SIGNED      0
#define HAL_HST_HSTDBGMXLEN_ADR         0x10000BFC

#define HAL_HST_HSTDBGMXLSEL_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXL)
#define HAL_HST_HSTDBGMXLSEL_LSB         0
#define HAL_HST_HSTDBGMXLSEL_MSB         5
#define HAL_HST_HSTDBGMXLSEL_MASK        0x0000003F
#define HAL_HST_HSTDBGMXLSEL_TYPE        halHST_HSTDBGMXLSEL_t
#define HAL_HST_HSTDBGMXLSEL             HAL_HST_HSTDBGMXLSEL_MASK
#define HAL_HST_HSTDBGMXLSEL_READABLE    1
#define HAL_HST_HSTDBGMXLSEL_WRITABLE    1
#define HAL_HST_HSTDBGMXLSEL_SIGNED      0
#define HAL_HST_HSTDBGMXLSEL_ADR         0x10000BFC

// Register HSTDBGMXM
#define HAL_HST_HSTDBGMXMEN_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXM)
#define HAL_HST_HSTDBGMXMEN_LSB         7
#define HAL_HST_HSTDBGMXMEN_MSB         7
#define HAL_HST_HSTDBGMXMEN_MASK        0x00000080
#define HAL_HST_HSTDBGMXMEN_TYPE        Bool
#define HAL_HST_HSTDBGMXMEN             HAL_HST_HSTDBGMXMEN_MASK
#define HAL_HST_HSTDBGMXMEN_READABLE    1
#define HAL_HST_HSTDBGMXMEN_WRITABLE    1
#define HAL_HST_HSTDBGMXMEN_SIGNED      0
#define HAL_HST_HSTDBGMXMEN_ADR         0x10000BFD

#define HAL_HST_HSTDBGMXMSEL_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXM)
#define HAL_HST_HSTDBGMXMSEL_LSB         0
#define HAL_HST_HSTDBGMXMSEL_MSB         5
#define HAL_HST_HSTDBGMXMSEL_MASK        0x0000003F
#define HAL_HST_HSTDBGMXMSEL_TYPE        halHST_HSTDBGMXMSEL_t
#define HAL_HST_HSTDBGMXMSEL             HAL_HST_HSTDBGMXMSEL_MASK
#define HAL_HST_HSTDBGMXMSEL_READABLE    1
#define HAL_HST_HSTDBGMXMSEL_WRITABLE    1
#define HAL_HST_HSTDBGMXMSEL_SIGNED      0
#define HAL_HST_HSTDBGMXMSEL_ADR         0x10000BFD

// Register HSTDBGMXH
#define HAL_HST_HSTDBGMXHEN_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXH)
#define HAL_HST_HSTDBGMXHEN_LSB         7
#define HAL_HST_HSTDBGMXHEN_MSB         7
#define HAL_HST_HSTDBGMXHEN_MASK        0x00000080
#define HAL_HST_HSTDBGMXHEN_TYPE        Bool
#define HAL_HST_HSTDBGMXHEN             HAL_HST_HSTDBGMXHEN_MASK
#define HAL_HST_HSTDBGMXHEN_READABLE    1
#define HAL_HST_HSTDBGMXHEN_WRITABLE    1
#define HAL_HST_HSTDBGMXHEN_SIGNED      0
#define HAL_HST_HSTDBGMXHEN_ADR         0x10000BFE

#define HAL_HST_HSTDBGMXHSEL_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXH)
#define HAL_HST_HSTDBGMXHSEL_LSB         0
#define HAL_HST_HSTDBGMXHSEL_MSB         5
#define HAL_HST_HSTDBGMXHSEL_MASK        0x0000003F
#define HAL_HST_HSTDBGMXHSEL_TYPE        halHST_HSTDBGMXHSEL_t
#define HAL_HST_HSTDBGMXHSEL             HAL_HST_HSTDBGMXHSEL_MASK
#define HAL_HST_HSTDBGMXHSEL_READABLE    1
#define HAL_HST_HSTDBGMXHSEL_WRITABLE    1
#define HAL_HST_HSTDBGMXHSEL_SIGNED      0
#define HAL_HST_HSTDBGMXHSEL_ADR         0x10000BFE

// Register HSTDBGMXU
#define HAL_HST_HSTDBGMXUEN_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXU)
#define HAL_HST_HSTDBGMXUEN_LSB         7
#define HAL_HST_HSTDBGMXUEN_MSB         7
#define HAL_HST_HSTDBGMXUEN_MASK        0x00000080
#define HAL_HST_HSTDBGMXUEN_TYPE        Bool
#define HAL_HST_HSTDBGMXUEN             HAL_HST_HSTDBGMXUEN_MASK
#define HAL_HST_HSTDBGMXUEN_READABLE    1
#define HAL_HST_HSTDBGMXUEN_WRITABLE    1
#define HAL_HST_HSTDBGMXUEN_SIGNED      0
#define HAL_HST_HSTDBGMXUEN_ADR         0x10000BFF

#define HAL_HST_HSTDBGMXUSEL_REGISTER    (HAL_HSTREG_MAP->HSTDBGMXU)
#define HAL_HST_HSTDBGMXUSEL_LSB         0
#define HAL_HST_HSTDBGMXUSEL_MSB         5
#define HAL_HST_HSTDBGMXUSEL_MASK        0x0000003F
#define HAL_HST_HSTDBGMXUSEL_TYPE        halHST_HSTDBGMXUSEL_t
#define HAL_HST_HSTDBGMXUSEL             HAL_HST_HSTDBGMXUSEL_MASK
#define HAL_HST_HSTDBGMXUSEL_READABLE    1
#define HAL_HST_HSTDBGMXUSEL_WRITABLE    1
#define HAL_HST_HSTDBGMXUSEL_SIGNED      0
#define HAL_HST_HSTDBGMXUSEL_ADR         0x10000BFF

#endif // #if (CHIP_REV >= 0xC0)

// Enumerations
typedef enum
{
	halHST_SleepExit_SEL0 = 0,    // Clock Stop Cleared By Command writes, ATA soft reset, power-on reset, host reset
	halHST_SleepExit_SEL1 = 1,    // Clock Stop Cleared By ATA soft reset, power-on reset, host reset
	halHST_SleepExit_SEL2 = 2,    // Clock Stop Cleared By ATA Executive Drive Diagnostics, ATAPI reset, soft reset, power-on reset, host reset
	halHST_SleepExit_SEL3 = 3     // Clock Stop Cleared By ATAPI reset, soft reset, power-on reset, host reset
} halHST_SleepExit_t;

typedef enum
{
	halHST_PktSize_12byte = 0,    // Packet Command Size = 12 bytes
	halHST_PktSize_16byte = 1     // Packet Command Size = 16 bytes
} halHST_PktSize_t;

typedef enum
{
	halHST_DMAMODE_PIO  = 0,
	halHST_DMAMODE_DMA1 = 1,
	halHST_DMAMODE_DMA2 = 2,
	halHST_DMAMODE_DMA3 = 3 
} halHST_DMAMODE_t;

typedef enum
{
	halHST_HstTxDir_RdfromRAM = 0,    // Host Transfer Direction: Host reads from RAM
	halHST_HstTxDir_WrtoRAM   = 1     // Host Transfer Direction: Host writes to RAM
} halHST_HstTxDir_t;

typedef enum
{
	halHST_AddressTyp_PageOffset   = 0,    // Addressing Type: Page/Offset address
	halHST_AddressTyp_AbsoluteAddr = 1     // Addressing Type: Absolute address
} halHST_AddressTyp_t;

typedef enum
{
	halHST_RAMBurstSize_8B  = 0,
	halHST_RAMBurstSize_16B = 1,
	halHST_RAMBurstSize_32B = 2,
	halHST_RAMBurstSize_64B = 3 
} halHST_RAMBurstSize_t;

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_ODD_PAGE_WORD  = 1,    // original HSTWDC is odd
	halHST_EVEN_PAGE_WORD = 0     // original HSTWDC is even
} halHST_ODDPAGEWD_t;

typedef enum
{
	halHST_NEW_LSTDTFISEL = 0,    // new last_datafis to fix bug5782
	halHST_ORG_LSTDTFISEL = 1     // original last_datafis
} halHST_LSTDTFISEL_t;

typedef enum
{
	halHST_NON_CMDIP = 0,    // ATA/ATAPI command is not in processing
	halHST_IN_CMDIP  = 1     // ATA/ATAPI command is in processing
} halHST_CMDIP_t;

typedef enum
{
	halHST_NDATAFIS    = 0,    // Use tx_ndatafis for tx_fis in sata_dlxsm
	halHST_NDATAFISLAT = 1     // Use t0_txndatafislat for tx_fis in sata_dlxsm
} halHST_NDTFISEL_t;

typedef enum
{
	halHST_DFRMSIZE_3P8K = 0,    // DataFIS Size set to 3.84K
	halHST_DFRMSIZE_4K   = 1     // DataFIS Size Set to 4K
} halHST_DFRMSIZE4K_t;

typedef enum
{
	halHST_RESWDC_Dis = 0,    // Resdual Word Support Disable
	halHST_RESWDC_En  = 1     // Resdual Word Support Enable
} halHST_RESWDCEN_t;

typedef enum
{
	halHST_SCNT_Overwrite_Dis = 0,    // SECCNT Data Overwrite Disable
	halHST_SCNT_Overwirte_En  = 1     // SECCNT Data Overwrite Enable
} halHST_SCNTOVEN_t;

typedef enum
{
	halHST_ODDWRD_SUP_Dis = 0,    // Odd WORD Support Disable
	halHST_ODDWRD_SUP_En  = 1     // ODD WORD Support Enable
} halHST_ODDWRDSUP_t;

#if (CHIP_REV >= 0xC1)

typedef enum
{
	halHST_HOLDBEFRSTCNT_En  = 0,    // HOLD BEF Reset XferCnt Enable
	halHST_HOLDBEFRSTCNT_Dis = 1     // HOLD BEF Reset XferCnt Disable
} halHST_HOLDBEFRSTCNTDIS_t;

typedef enum
{
	halHST_PHYSCANTEST_DIs = 0,    // PHY Scan Test Disable
	halHST_PHYSCANTEST_En  = 1     // PHY Scan Test Enable
} halHST_PHYSCANTEST_t;

#endif // #if (CHIP_REV >= 0xC1)

typedef enum
{
	halHST_AESIO_SWAP     = 0,    // AES IO Swap Function
	halHST_AESIO_SWAP_Dis = 1     // AES IO Swap Function Disable
} halHST_AESIOSWAPDIS_t;

typedef enum
{
	halHST_DLXSM_Reset_Dis = 0,    // DLXSM Reset Disable
	halHST_DLXSM_Reset_En  = 1     // DLXSM  Reset Enable
} halHST_DLXSMRSTEN_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_NIEN_Manual_Dis = 0,    // NIEN Manual control Disable
	halHST_NIEN_Manual_En  = 1     // NIEN Manual control Enable
} halHST_NIENMEN_t;

typedef enum
{
	halHST_FISTyp_Dev2Hst          = 52,    // Device to Host Register FIS
	halHST_FISTyp_Dev2HstDMAAct    = 57,    // Device to Host DMA Activate FIS
	halHST_FISTyp_Dev2HstDMASetup  = 65,    // Device to Host DMA Setup FIS (not support)
	halHST_FISTyp_Dev2HstData      = 70,    // Device to Host Data FIS
	halHST_FISTyp_Dev2HstBISTAct   = 88,    // Device to Host BIST Activate FIS (not support)
	halHST_FISTyp_Dev2HstPIOSetup  = 95,    // Device to Host PIO Setup FIS
	halHST_FISTyp_Dev2HstSetDevBit = 161     // Device to Host Set Device Bits FIS
} halHST_FISTyp_t;

typedef enum
{
	halHST_SATADBG_DLXSMa    = 0,    // bit7:dbg_align_rst, bit6-0:dbg_dlxsm(6-0)
	halHST_SATADBG_DLXSMb    = 1,    // bit7-0:dbg_dlxsm(14-7)
	halHST_SATADBG_DLRSMa    = 2,    // bit3-0:dbg_dlrsm(3-0)
	halHST_SATADBG_DLRSMb    = 3,    // bit6-0:dbg_dlrsm(10-4)
	halHST_SATADBG_DSM       = 4,    // bit7-0:dbg_dsm(7-0)
	halHST_SATADBG_LNK_XRDY  = 5,    // bit5:dbg_t0_srst, bit4:dbg_t0_txndatafislat, bit3:dbg_tx_ndatafis, bit2:dbg_tx_datafis, bit1:dbg_t0_txrok, bit0:dbg_lnk_tx_fis
	halHST_SATADBG_DCSM      = 6,    // bit5-0:dbg_dcsm(5-0)
	halHST_SATADBG_PM        = 8,    // bit5:dbg_phy_cw, bit4:dbg_phy_cr, bit3-0:dbg_pm_state(3-0)
	halHST_SATADBG_SATA_SYNC = 9,    // bit7-4:dbg_phy_tx(3-0), bit3-0:dbg_phy_rx(3-0)
	halHST_SATADBG_SATA_GBL1 = 10,    // bit6:dbg_slumber, bit5:dbg_partial, bit4:dbg_txreq_wakeupreq, bit3:dbg_sata_lnkrdy, bit2:dbg_sata_dwclkon, bit1:dbg_sata_phyrdy, bit0:dbg_sata_comreset_n
	halHST_SATADBG_SATA_GBL2 = 11,    // bit5:dbg_tx_fis_ok, bit4:dbg_tx_fis_err, bit3:dbg_rx_fis_ok, bit2:dbg_rx_fis_err, bit1:dbg_sata_txfifo_empty, bit0:dbg_sata_rxfifo_empty
	halHST_SATADBG_SATA_CLK  = 12     // bit2:dbg_sata_dword_sleep_clock_on, bit0:dbg_sata_dword_clock_on
} halHST_SATADbg_t;

typedef enum
{
	halHST_AESKeySRC_ROMKey   = 0,    // AES Key Source: use ROM Key
	halHST_AESKeySRC_FwRegKey = 1     // AES Key Source: usefirmware register key
} halHST_AESKeySRC_t;

typedef enum
{
	halHST_SUBCPUMOD_DS = 0,    // AES block is used by the datastream and no firmware processing is done
	halHST_SUBCPUMOD_FW = 1     // AES block is used by firmware and no datastream processing is done
} halHST_SUBCPUMOD_t;

typedef enum
{
	halHST_CBChainMOD_ECodeBook = 0,    // Enable Cipher Block Chain Mode:Use electronic code book mode
	halHST_CBChainMOD_CipBlk    = 1     // Enable Cipher Block Chain Mode:Use cipher block chain mode
} halHST_CBChainMOD_t;

typedef enum
{
	halHST_AESEncMOD_Decrypt = 0,    // AES block decrypts
	halHST_AESEncMOD_Encrypt = 1     // AES block encrypts
} halHST_AESEncMOD_t;

typedef enum
{
	halHST_BEFBitOffset_BD = 7,    // BD
	halHST_BEFBitOffset_HD = 7     // HD DVD and DVD
} halHST_BEFBitOffset_t;

typedef enum
{
	halHST_BEFMaxSecOffset_HD_DVD = 15,    // BEF Maximum sector offset in an HD DVD or DVD ECC block
	halHST_BEFMaxSecOffset_BD     = 31     // Maximum sector offset in a BD Recording Cluster
} halHST_BEFMaxSecOffset_t;

typedef enum
{
	halHST_BEFSecLen_HD_DVD = 16,    // BEF Control Data Sector Byte Length for HD DVD/DVD
	halHST_BEFSecLen_BD     = 18     // BEF Control Data Sector Byte Length for BD
} halHST_BEFSecLen_t;

typedef enum
{
	halHST_BEFBlkLen_HD_DVD = 16,       // BEF Control Data Block Byte Length for HD DVD/DVD
	halHST_BEFBlkLen_BD     = 1490      // BEF Control Data Block Byte Length for BD
} halHST_BEFBlkLen_t;

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_PLLBW_NORMAL   = 0,    // Normal SATA PLL Bandwidth
	halHST_PLLBW_NARROWER = 1     // Narrower SATA PLL Bandwidth
} halHST_SATPLLBW_t;

typedef enum
{
	halHST_SSCDEV_500_TO_2500 = 0,    // SSC Deviation range: -500~-2500ppm
	halHST_SSCDEV_500_TO_4500 = 1     // SSC Deviation range: -500~-4500ppm
} halHST_SSCDEVSEL_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_RefClkMOD_PLL25MHz  = 0,    // Reference Clock Mode: PLL is locked to the  25 MHz reference clock
	halHST_RefClkMOD_PLL100MHz = 1     // Reference Clock Mode: PLL is locked to the 100 MHz reference clock
} halHST_RefClkMOD_t;

typedef enum
{
	halHST_SSCCtl_SSCOFF = 0,
	halHST_SSCCtl_SSC1   = 1,
	halHST_SSCCtl_SSC2   = 2,
	halHST_SSCCtl_SSC3   = 3,
	halHST_SSCCtl_SSC4   = 4,
	halHST_SSCCtl_SSC5   = 5,
	halHST_SSCCtl_SSC6   = 6,
	halHST_SSCCtl_SSC7   = 7,
	halHST_SSCCtl_SSC8   = 8,
	halHST_SSCCtl_SSC9   = 9,
	halHST_SSCCtl_SSC10  = 10,
	halHST_SSCCtl_SSC11  = 11,
	halHST_SSCCtl_SSC12  = 12,
	halHST_SSCCtl_SSC13  = 13,
	halHST_SSCCtl_SSC14  = 14,
	halHST_SSCCtl_SSCON  = 15 
} halHST_SSCCtl_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halHST_RefClkIN_IntClk = 1     // Reference Clock input use internal clock
} halHST_RefClkIN_t;

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_R_INTERNAL = 0,    // Use internal resistor
	halHST_R_EXTERNAL = 1     // Use external resistor at REXT pin
} halHST_REXT_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_SqBias_Inc17percent = 0,    // Changes the squelch detector bias current:  17% (Increase 17%)
	halHST_SqBias_NoEffect     = 1,    // Changes the squelch detector bias current:   0%
	halHST_SqBias_Dec20percent = 2,    // Changes the squelch detector bias current: -20% (Decrease 20%)
	halHST_SqBias_Dec40percent = 3     // Changes the squelch detector bias current: -40% (Decrease 40%)
} halHST_SqBias_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halHST_ChipBias_Inc15percent = 0,    // Changes the analog circuit bias current:+15%
	halHST_ChipBias_NoEffect     = 1,    // Changes the analog circuit bias current:  0%
	halHST_ChipBias_Inc5percent  = 2,    // Changes the analog circuit bias current: +5%
	halHST_ChipBias_Dec5percent  = 3     // Changes the analog circuit bias current: -5%
} halHST_ChipBias_t;

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_ChipBias_Inc5percent  = 0,    // Changes the analog circuit bias current: +5%
	halHST_ChipBias_NoEffect     = 1,    // Changes the analog circuit bias current:  0%
	halHST_ChipBias_Dec10percent = 2,    // Changes the analog circuit bias current:-10%
	halHST_ChipBias_Dec15percent = 3     // Changes the analog circuit bias current:-15%
} halHST_ChipBias_t;

#endif // #if (CHIP_REV >= 0xC0)

#if (CHIP_REV <= 0xC0)

typedef enum
{
	halHST_PLLCPBias_100uA = 0,    // Change the PLL charge pump bias current to 100 uA
	halHST_PLLCPBias_125uA = 1,    // Change the PLL charge pump bias current to 125 uA
	halHST_PLLCPBias_150uA = 2,    // Change the PLL charge pump bias current to 150 uA
	halHST_PLLCPBias_175uA = 3,    // Change the PLL charge pump bias current to 175 uA
	halHST_PLLCPBias_200uA = 4,    // Change the PLL charge pump bias current to 200 uA
	halHST_PLLCPBias_225uA = 5,    // Change the PLL charge pump bias current to 225 uA
	halHST_PLLCPBias_250uA = 6,    // Change the PLL charge pump bias current to 250 uA
	halHST_PLLCPBias_275uA = 7     // Change the PLL charge pump bias current to 275 uA
} halHST_PLLCPBias_t;

#endif // #if (CHIP_REV <= 0xC0)

#if (CHIP_REV >= 0xC1)

typedef enum
{
	halHST_PLLCPBias_50uA  = 0,    // Change the PLL charge pump bias current to 50 uA
	halHST_PLLCPBias_75uA  = 1,    // Change the PLL charge pump bias current to 75 uA
	halHST_PLLCPBias_100uA = 2,    // Change the PLL charge pump bias current to 100 uA
	halHST_PLLCPBias_125uA = 3,    // Change the PLL charge pump bias current to 125 uA
	halHST_PLLCPBias_150uA = 4,    // Change the PLL charge pump bias current to 150 uA
	halHST_PLLCPBias_175uA = 5,    // Change the PLL charge pump bias current to 175 uA
	halHST_PLLCPBias_200uA = 6,    // Change the PLL charge pump bias current to 200 uA
	halHST_PLLCPBias_225uA = 7     // Change the PLL charge pump bias current to 225 uA
} halHST_PLLCPBias_t;

#endif // #if (CHIP_REV >= 0xC1)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halHST_HSTDBGMXLSEL_PRIMITIVE    = 0,
	halHST_HSTDBGMXLSEL_ASEQ         = 1,
	halHST_HSTDBGMXLSEL_BISTL        = 2,
	halHST_HSTDBGMXLSEL_DLRSM        = 3,
	halHST_HSTDBGMXLSEL_DLXSM        = 4,
	halHST_HSTDBGMXLSEL_DCSM_DSM     = 5,
	halHST_HSTDBGMXLSEL_SDIF         = 6,
	halHST_HSTDBGMXLSEL_RXTXCTL      = 7,
	halHST_HSTDBGMXLSEL_RXFIFO       = 8,
	halHST_HSTDBGMXLSEL_TXFIFO       = 9,
	halHST_HSTDBGMXLSEL_RAMIF        = 10,
	halHST_HSTDBGMXLSEL_CSS          = 11,
	halHST_HSTDBGMXLSEL_AES          = 12,
	halHST_HSTDBGMXLSEL_KBE          = 13,
	halHST_HSTDBGMXLSEL_BEFBUS0      = 14,
	halHST_HSTDBGMXLSEL_BEFBUS1      = 15,
	halHST_HSTDBGMXLSEL_PM_PHYSM     = 16,
	halHST_HSTDBGMXLSEL_PM_MANUAL    = 17,
	halHST_HSTDBGMXLSEL_PM_AUTO      = 18,
	halHST_HSTDBGMXLSEL_RSV19        = 19,
	halHST_HSTDBGMXLSEL_RXDE         = 20,
	halHST_HSTDBGMXLSEL_TXDE         = 21,
	halHST_HSTDBGMXLSEL_RXD32L       = 22,
	halHST_HSTDBGMXLSEL_TXD32L       = 23,
	halHST_HSTDBGMXLSEL_RAMRL        = 24,
	halHST_HSTDBGMXLSEL_RAMWL        = 25,
	halHST_HSTDBGMXLSEL_CSSMASKL     = 26,
	halHST_HSTDBGMXLSEL_BISTTXDATAL  = 27,
	halHST_HSTDBGMXLSEL_RXFIFORDATAL = 28,
	halHST_HSTDBGMXLSEL_RSV29        = 29,
	halHST_HSTDBGMXLSEL_RSV30        = 30,
	halHST_HSTDBGMXLSEL_RSV31        = 31 
} halHST_HSTDBGMXLSEL_t;

typedef enum
{
	halHST_HSTDBGMXMSEL_PRIMITIVE    = 0,
	halHST_HSTDBGMXMSEL_ASEQ         = 1,
	halHST_HSTDBGMXMSEL_BISTL        = 2,
	halHST_HSTDBGMXMSEL_DLRSM        = 3,
	halHST_HSTDBGMXMSEL_DLXSM        = 4,
	halHST_HSTDBGMXMSEL_DCSM_DSM     = 5,
	halHST_HSTDBGMXMSEL_SDIF         = 6,
	halHST_HSTDBGMXMSEL_RXTXCTL      = 7,
	halHST_HSTDBGMXMSEL_RXFIFO       = 8,
	halHST_HSTDBGMXMSEL_TXFIFO       = 9,
	halHST_HSTDBGMXMSEL_RAMIF        = 10,
	halHST_HSTDBGMXMSEL_CSS          = 11,
	halHST_HSTDBGMXMSEL_AES          = 12,
	halHST_HSTDBGMXMSEL_KBE          = 13,
	halHST_HSTDBGMXMSEL_BEFBUS0      = 14,
	halHST_HSTDBGMXMSEL_BEFBUS1      = 15,
	halHST_HSTDBGMXMSEL_PM_PHYSM     = 16,
	halHST_HSTDBGMXMSEL_PM_MANUAL    = 17,
	halHST_HSTDBGMXMSEL_PM_AUTO      = 18,
	halHST_HSTDBGMXMSEL_RSV19        = 19,
	halHST_HSTDBGMXMSEL_RXDO         = 20,
	halHST_HSTDBGMXMSEL_TXDO         = 21,
	halHST_HSTDBGMXMSEL_RXD32H       = 22,
	halHST_HSTDBGMXMSEL_TXD32H       = 23,
	halHST_HSTDBGMXMSEL_RAMRH        = 24,
	halHST_HSTDBGMXMSEL_RAMWH        = 25,
	halHST_HSTDBGMXMSEL_CSSMASKH     = 26,
	halHST_HSTDBGMXMSEL_BISTTXDATAH  = 27,
	halHST_HSTDBGMXMSEL_RXFIFORDATAH = 28,
	halHST_HSTDBGMXMSEL_RSV29        = 29,
	halHST_HSTDBGMXMSEL_RSV30        = 30,
	halHST_HSTDBGMXMSEL_RSV31        = 31 
} halHST_HSTDBGMXMSEL_t;

typedef enum
{
	halHST_HSTDBGMXHSEL_PRIMITIVE    = 0,
	halHST_HSTDBGMXHSEL_ASEQ         = 1,
	halHST_HSTDBGMXHSEL_BISTL        = 2,
	halHST_HSTDBGMXHSEL_DLRSM        = 3,
	halHST_HSTDBGMXHSEL_DLXSM        = 4,
	halHST_HSTDBGMXHSEL_DCSM_DSM     = 5,
	halHST_HSTDBGMXHSEL_SDIF         = 6,
	halHST_HSTDBGMXHSEL_RXTXCTL      = 7,
	halHST_HSTDBGMXHSEL_RXFIFO       = 8,
	halHST_HSTDBGMXHSEL_TXFIFO       = 9,
	halHST_HSTDBGMXHSEL_RAMIF        = 10,
	halHST_HSTDBGMXHSEL_CSS          = 11,
	halHST_HSTDBGMXHSEL_AES          = 12,
	halHST_HSTDBGMXHSEL_KBE          = 13,
	halHST_HSTDBGMXHSEL_BEFBUS0      = 14,
	halHST_HSTDBGMXHSEL_BEFBUS1      = 15,
	halHST_HSTDBGMXHSEL_PM_PHYSM     = 16,
	halHST_HSTDBGMXHSEL_PM_MANUAL    = 17,
	halHST_HSTDBGMXHSEL_PM_AUTO      = 18,
	halHST_HSTDBGMXHSEL_RSV19        = 19,
	halHST_HSTDBGMXHSEL_RXDE         = 20,
	halHST_HSTDBGMXHSEL_TXDE         = 21,
	halHST_HSTDBGMXHSEL_RXD32L       = 22,
	halHST_HSTDBGMXHSEL_TXD32L       = 23,
	halHST_HSTDBGMXHSEL_RAMRL        = 24,
	halHST_HSTDBGMXHSEL_RAMWL        = 25,
	halHST_HSTDBGMXHSEL_CSSMASKL     = 26,
	halHST_HSTDBGMXHSEL_BISTTXDATAL  = 27,
	halHST_HSTDBGMXHSEL_RXFIFORDATAL = 28,
	halHST_HSTDBGMXHSEL_RSV29        = 29,
	halHST_HSTDBGMXHSEL_RSV30        = 30,
	halHST_HSTDBGMXHSEL_RSV31        = 31 
} halHST_HSTDBGMXHSEL_t;

typedef enum
{
	halHST_HSTDBGMXUSEL_PRIMITIVE    = 0,
	halHST_HSTDBGMXUSEL_ASEQ         = 1,
	halHST_HSTDBGMXUSEL_BISTL        = 2,
	halHST_HSTDBGMXUSEL_DLRSM        = 3,
	halHST_HSTDBGMXUSEL_DLXSM        = 4,
	halHST_HSTDBGMXUSEL_DCSM_DSM     = 5,
	halHST_HSTDBGMXUSEL_SDIF         = 6,
	halHST_HSTDBGMXUSEL_RXTXCTL      = 7,
	halHST_HSTDBGMXUSEL_RXFIFO       = 8,
	halHST_HSTDBGMXUSEL_TXFIFO       = 9,
	halHST_HSTDBGMXUSEL_RAMIF        = 10,
	halHST_HSTDBGMXUSEL_CSS          = 11,
	halHST_HSTDBGMXUSEL_AES          = 12,
	halHST_HSTDBGMXUSEL_KBE          = 13,
	halHST_HSTDBGMXUSEL_BEFBUS0      = 14,
	halHST_HSTDBGMXUSEL_BEFBUS1      = 15,
	halHST_HSTDBGMXUSEL_PM_PHYSM     = 16,
	halHST_HSTDBGMXUSEL_PM_MANUAL    = 17,
	halHST_HSTDBGMXUSEL_PM_AUTO      = 18,
	halHST_HSTDBGMXUSEL_RSV19        = 19,
	halHST_HSTDBGMXUSEL_RXDO         = 20,
	halHST_HSTDBGMXUSEL_TXDO         = 21,
	halHST_HSTDBGMXUSEL_RXD32H       = 22,
	halHST_HSTDBGMXUSEL_TXD32H       = 23,
	halHST_HSTDBGMXUSEL_RAMRH        = 24,
	halHST_HSTDBGMXUSEL_RAMWH        = 25,
	halHST_HSTDBGMXUSEL_CSSMASKH     = 26,
	halHST_HSTDBGMXUSEL_BISTTXDATAH  = 27,
	halHST_HSTDBGMXUSEL_RXFIFORDATAH = 28,
	halHST_HSTDBGMXUSEL_RSV29        = 29,
	halHST_HSTDBGMXUSEL_RSV30        = 30,
	halHST_HSTDBGMXUSEL_RSV31        = 31 
} halHST_HSTDBGMXUSEL_t;

#endif // #if (CHIP_REV >= 0xC0)

#endif /* __REG_HOST_H__ */

