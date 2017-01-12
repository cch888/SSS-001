/******************************************************************************
*               (c) Copyright 2003 - 2010 SUNEXT TECHNOLOGY CO., LTD.
*                        All Right Reserved
*
* FILENAME: REG_servo.h
*
*
* DESCRIPTION: This file contains register, bit and enum definitions
*              for RegHAL analysis of the SRV core.
*
* NOTE: This file is created automatically and should NOT be modified by hand.
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef __REG_SERVO_H__
#define __REG_SERVO_H__

// Core register map
#define HAL_SRV_VERSION 7.143
#define HAL_SRV_BaseAddress 0xB0000000
#define HAL_SRVREG_MAP ((struct halSRVReg_Map*) (void *) HAL_SRV_BaseAddress)

#if (CHIP_REV <= 0xB3)

__packed struct halSRVReg_Map
{
	HAL_Reg32 SEEKTBADD;                      //00000000
	HAL_Reg32 SPEDPHVAL;                      //00000004
	HAL_Reg32 ACCINTVAL;                      //00000008
	HAL_Reg32 ReservedBlk0[1];                //0000000C
	HAL_Reg32 JUMPDAT;                        //00000010
	HAL_Reg32 ReservedBlk1[1];                //00000014
	HAL_Reg32 BRAKEDAT;                       //00000018
	HAL_Reg32 ReservedBlk2[1];                //0000001C
	HAL_Reg32 PHCHGMOD;                       //00000020
	HAL_Reg32 TIMPER;                         //00000024
	HAL_Reg32 HWSEEKCFG;                      //00000028
	HAL_Reg32 ReservedBlk3[1];                //0000002C
	HAL_Reg32 STRLOC;                         //00000030
	HAL_Reg32 ReservedBlk4[1];                //00000034
	HAL_Reg32 STRSTPOT;                       //00000038
	HAL_Reg32 ACCSTPSIZ;                      //0000003C
	HAL_Reg32 SESTAFULL;                      //00000040
	HAL_Reg32 ReservedBlk5[1];                //00000044
	HAL_Reg32 CURLOC;                         //00000048
	HAL_Reg32 ReservedBlk6[1];                //0000004C
	HAL_Reg32 SEEKEND;                        //00000050
	HAL_Reg32 FEFSLNEG;                       //00000054
	HAL_Reg32 EARZCPROT;                      //00000058
	HAL_Reg32 BRAKELVL;                       //0000005C
	HAL_Reg32 KICKLVL;                        //00000060
	HAL_Reg32 SPEEDLVL;                       //00000064
	HAL_Reg32 BRAKEHLD;                       //00000068
	HAL_Reg32 BRAKECNT;                       //0000006C
	HAL_Reg32 ReservedBlk7[4];                //00000070
	HAL_Reg32 STPGEN;                         //00000080
	HAL_Reg32 STPSIZE;                        //00000084
	HAL_Reg32 STPDCO;                         //00000088
	HAL_Reg32 STPADD;                         //0000008C
	HAL_Reg32 STPDAC;                         //00000090
	HAL_Reg32 ReservedBlk8[1];                //00000094
	HAL_Reg32 STPPOS;                         //00000098
	HAL_Reg32 STPCTRL;                        //0000009C
	HAL_Reg32 STPPWR;                         //000000A0
	HAL_Reg32 ReservedBlk9[1];                //000000A4
	HAL_Reg32 STPLOC;                         //000000A8
	HAL_Reg32 ReservedBlk10[1];               //000000AC
	HAL_Reg32 HYSLTHLD;                       //000000B0
	HAL_Reg32 HYSHTHLD;                       //000000B4
	HAL_Reg32 ReservedBlk11[18];              //000000B8
	HAL_Reg32 FOODAT;                         //00000100
	HAL_Reg32 ReservedBlk12[1];               //00000104
	HAL_Reg32 TRODAT;                         //00000108
	HAL_Reg32 ReservedBlk13[1];               //0000010C
	HAL_Reg32 TILTDAT;                        //00000110
	HAL_Reg32 ReservedBlk14[1];               //00000114
	HAL_Reg32 DACSW;                          //00000118
	HAL_Reg32 FETHR;                          //0000011C
	HAL_Reg32 FETHRCTL;                       //00000120
	HAL_Reg32 ReservedBlk15[1];               //00000124
	HAL_Reg32 SBYLVL;                         //00000128
	HAL_Reg32 AWMASK;                         //0000012C
	HAL_Reg32 ReservedBlk16[2];               //00000130
	HAL_Reg32 AFCRNG1;                        //00000138
	HAL_Reg32 AFCRNG2;                        //0000013C
	HAL_Reg32 SVMODE0;                        //00000140
	HAL_Reg32 SVMODE1;                        //00000144
	HAL_Reg32 FGCMP;                          //00000148
	HAL_Reg32 ReservedBlk17[1];               //0000014C
	HAL_Reg32 WBLCMP;                         //00000150
	HAL_Reg32 ReservedBlk18[1];               //00000154
	HAL_Reg32 DECCMP;                         //00000158
	HAL_Reg32 ReservedBlk19[1];               //0000015C
	HAL_Reg32 AKICLVL;                        //00000160
	HAL_Reg32 ReservedBlk20[1];               //00000164
	HAL_Reg32 WBLRNG;                         //00000168
	HAL_Reg32 ReservedBlk21[1];               //0000016C
	HAL_Reg32 WAITWBL;                        //00000170
	HAL_Reg32 APCGAIN;                        //00000174
	HAL_Reg32 APCNUM;                         //00000178
	HAL_Reg32 OSCFGMAX;                       //0000017C
	HAL_Reg32 TESODCTL;                       //00000180
	HAL_Reg32 TESOFMIN;                       //00000184
	HAL_Reg32 TESOFMAX;                       //00000188
	HAL_Reg32 TESOAM;                         //0000018C
	HAL_Reg32 TESOWD;                         //00000190
	HAL_Reg32 TESOCNT;                        //00000194
	HAL_Reg32 TESOCTHR;                       //00000198
	HAL_Reg32 ReservedBlk22[1];               //0000019C
	HAL_Reg32 FE1SODCTL;                      //000001A0
	HAL_Reg32 FE1SOFMIN;                      //000001A4
	HAL_Reg32 FE1SOFMAX;                      //000001A8
	HAL_Reg32 FE1SOAM;                        //000001AC
	HAL_Reg32 FE1SOWD;                        //000001B0
	HAL_Reg32 FE1SOCNT;                       //000001B4
	HAL_Reg32 FE1SOCTHR;                      //000001B8
	HAL_Reg32 ReservedBlk23[1];               //000001BC
	HAL_Reg32 FE2SODCTL;                      //000001C0
	HAL_Reg32 FE2SOFMIN;                      //000001C4
	HAL_Reg32 FE2SOFMAX;                      //000001C8
	HAL_Reg32 FE2SOAM;                        //000001CC
	HAL_Reg32 FE2SOWD;                        //000001D0
	HAL_Reg32 FE2SOCNT;                       //000001D4
	HAL_Reg32 FE2SOCTHR;                      //000001D8
	HAL_Reg32 SPINCTL;                        //000001DC
	HAL_Reg32 ReservedBlk24[2];               //000001E0
	HAL_Reg32 CAVM;                           //000001E8
	HAL_Reg32 ReservedBlk25[1];               //000001EC
	HAL_Reg32 CLVN;                           //000001F0
	HAL_Reg32 ReservedBlk26[1];               //000001F4
	HAL_Reg32 FGRNG;                          //000001F8
	HAL_Reg32 ReservedBlk27[1];               //000001FC
	HAL_Reg32 REVCNT;                         //00000200
	HAL_Reg32 ReservedBlk28[1];               //00000204
	HAL_Reg32 INITR;                          //00000208
	HAL_Reg32 PUSTP;                          //0000020C
	HAL_Reg32 PUFWD;                          //00000210
	HAL_Reg32 PUBWD;                          //00000214
	HAL_Reg32 LDON;                           //00000218
	HAL_Reg32 LDOFF;                          //0000021C
	HAL_Reg32 DMSV;                           //00000220
	HAL_Reg32 DMOFF;                          //00000224
	HAL_Reg32 FOLNSUP;                        //00000228
	HAL_Reg32 FOLNSDN;                        //0000022C
	HAL_Reg32 DMFBK;                          //00000230
	HAL_Reg32 PAC;                            //00000234
	HAL_Reg32 PAF;                            //00000238
	HAL_Reg32 SLED;                           //0000023C
	HAL_Reg32 FSCNTL;                         //00000240
	HAL_Reg32 FSTHRES;                        //00000244
	HAL_Reg32 SHCCTS;                         //00000248
	HAL_Reg32 CTTHRES;                        //0000024C
	HAL_Reg32 SYNCNT;                         //00000250
	HAL_Reg32 ReservedBlk29[1];               //00000254
	HAL_Reg32 SYNTHR;                         //00000258
	HAL_Reg32 ReservedBlk30[4];               //0000025C
	HAL_Reg32 TILTOFS;                        //0000026C
	HAL_Reg32 ReservedBlk31[4];               //00000270
	HAL_Reg32 FVKICM;                         //00000280
	HAL_Reg32 FVKICK;                         //00000284
	HAL_Reg32 ReservedBlk32[5];               //00000288
	HAL_Reg32 GPDACMUX;                       //0000029C
	HAL_Reg32 CTN;                            //000002A0
	HAL_Reg32 ReservedBlk33[1];               //000002A4
	HAL_Reg32 DACDAT;                         //000002A8
	HAL_Reg32 ReservedBlk34[3];               //000002AC
	HAL_Reg32 AFC;                            //000002B8
	HAL_Reg32 APC;                            //000002BC
	HAL_Reg32 DMODAC;                         //000002C0
	HAL_Reg32 SRCIN;                          //000002C4
	HAL_Reg32 DLCNT;                          //000002C8
	HAL_Reg32 ReservedBlk35[1];               //000002CC
	HAL_Reg32 SDLCNT;                         //000002D0
	HAL_Reg32 ReservedBlk36[1];               //000002D4
	HAL_Reg32 DLCNTF;                         //000002D8
	HAL_Reg32 ReservedBlk37[1];               //000002DC
	HAL_Reg32 SVSTATUS;                       //000002E0
	HAL_Reg32 ReservedBlk38[1];               //000002E4
	HAL_Reg32 SRCR;                           //000002E8
	HAL_Reg32 ReservedBlk39[5];               //000002EC
	HAL_Reg32 CKICH;                          //00000300
	HAL_Reg32 CKICL;                          //00000304
	HAL_Reg32 DEADZ;                          //00000308
	HAL_Reg32 WGHOLDC;                        //0000030C
	HAL_Reg32 DLTHR;                          //00000310
	HAL_Reg32 DLPRG;                          //00000314
	HAL_Reg32 ReservedBlk40[1];               //00000318
	HAL_Reg32 PNCBKR;                         //0000031C
	HAL_Reg32 TXSPEED;                        //00000320
	HAL_Reg32 ReservedBlk41[3];               //00000324
	HAL_Reg32 DFCTBL;                         //00000330
	HAL_Reg32 DFCTBT;                         //00000334
	HAL_Reg32 ReservedBlk42[2];               //00000338
	HAL_Reg32 FBA;                            //00000340
	HAL_Reg32 FGA;                            //00000344
	HAL_Reg32 ReservedBlk43[1];               //00000348
	HAL_Reg32 TGA;                            //0000034C
	HAL_Reg32 FTBAST;                         //00000350
	HAL_Reg32 FGASET;                         //00000354
	HAL_Reg32 TGASET;                         //00000358
	HAL_Reg32 ReservedBlk44[1];               //0000035C
	HAL_Reg32 DASET;                          //00000360
	HAL_Reg32 ReservedBlk45[1];               //00000364
	HAL_Reg32 RGSEL;                          //00000368
	HAL_Reg32 MASK;                           //0000036C
	HAL_Reg32 TROFFSET;                       //00000370
	HAL_Reg32 DLFSKLVL;                       //00000374
	HAL_Reg32 DLFSBLVL;                       //00000378
	HAL_Reg32 FOOFFSET;                       //0000037C
	HAL_Reg32 FSCNT;                          //00000380
	HAL_Reg32 FOCUS;                          //00000384
	HAL_Reg32 ReservedBlk46[1];               //00000388
	HAL_Reg32 SHOCK;                          //0000038C
	HAL_Reg32 SREG1;                          //00000390
	HAL_Reg32 SREG2;                          //00000394
	HAL_Reg32 FVSET;                          //00000398
	HAL_Reg32 SVSET;                          //0000039C
	HAL_Reg32 SVMIOSEL;                       //000003A0
	HAL_Reg32 ReservedBlk47[2];               //000003A4
	HAL_Reg32 DMCNT;                          //000003AC
	HAL_Reg32 DFCTSEL;                        //000003B0
	HAL_Reg32 CNTRL;                          //000003B4
	HAL_Reg32 DVDRHLD;                        //000003B8
	HAL_Reg32 CDRWSEEK;                       //000003BC
	HAL_Reg32 ReservedBlk48[4];               //000003C0
	HAL_Reg32 FOCUS1;                         //000003D0
	HAL_Reg32 SRVTEST1;                       //000003D4
	HAL_Reg32 SRVTEST2;                       //000003D8
	HAL_Reg32 ReservedBlk49[1];               //000003DC
	HAL_Reg32 SDINTEN;                        //000003E0
	HAL_Reg32 ReservedBlk50[1];               //000003E4
	HAL_Reg32 SDINT;                          //000003E8
	HAL_Reg32 ReservedBlk51[3];               //000003EC
	HAL_Reg32 SVSPARE;                        //000003F8
	HAL_Reg32 ReservedBlk52[1];               //000003FC
	HAL_Reg32 LCCTL;                          //00000400
	HAL_Reg32 ReservedBlk53[3];               //00000404
	HAL_Reg32 LCTHRESH;                       //00000410
	HAL_Reg32 LCDELTA0;                       //00000414
	HAL_Reg32 LCDELTA1;                       //00000418
	HAL_Reg32 LCDELTA2;                       //0000041C
	HAL_Reg32 LCTIMDELAY;                     //00000420
	HAL_Reg32 ReservedBlk54[7];               //00000424
	HAL_Reg32 LCINTEN;                        //00000440
	HAL_Reg32 LCIRQ;                          //00000444
	HAL_Reg32 ReservedBlk55[2];               //00000448
	HAL_Reg32 LCSTATUS;                       //00000450
	HAL_Reg32 INHIBITTX;                      //00000454
	HAL_Reg32 ReservedBlk56[2];               //00000458
	HAL_Reg32 PWMSEL;                         //00000460
	HAL_Reg32 ReservedBlk57[3];               //00000464
	HAL_Reg32 DSPMODE;                        //00000470
	HAL_Reg32 FGSRC;                          //00000474
	HAL_Reg32 SRCSTA;                         //00000478
	HAL_Reg32 PHAADDR;                        //0000047C
	HAL_Reg32 PHB1ADDR;                       //00000480
	HAL_Reg32 PHB2ADDR;                       //00000484
	HAL_Reg32 PHC1ADDR;                       //00000488
	HAL_Reg32 PHC2ADDR;                       //0000048C
	HAL_Reg32 PHC3ADDR;                       //00000490
	HAL_Reg32 PHC4ADDR;                       //00000494
	HAL_Reg32 PHDADDR;                        //00000498
	HAL_Reg32 RAMTEST;                        //0000049C
	HAL_Reg32 SEQ_PC;                         //000004A0
	HAL_Reg32 FWCTRL;                         //000004A4
	HAL_Reg32 PDMCNT;                         //000004A8
	HAL_Reg32 PDMALPHA;                       //000004AC
	HAL_Reg32 STMCTRL;                        //000004B0
	HAL_Reg32 TESRCCTL;                       //000004B4
	HAL_Reg32 PDMVREF;                        //000004B8
	HAL_Reg32 SWOFFEN;                        //000004BC
};

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

__packed struct halSRVReg_Map
{
	HAL_Reg32 SEEKTBADD;                      //00000000
	HAL_Reg32 SPEDPHVAL;                      //00000004
	HAL_Reg32 ACCINTVAL;                      //00000008
	HAL_Reg32 ReservedBlk0[1];                //0000000C
	HAL_Reg32 JUMPDAT;                        //00000010
	HAL_Reg32 ReservedBlk1[1];                //00000014
	HAL_Reg32 BRAKEDAT;                       //00000018
	HAL_Reg32 SSPDPHVAL;                      //0000001C
	HAL_Reg32 PHCHGMOD;                       //00000020
	HAL_Reg32 TIMPER;                         //00000024
	HAL_Reg32 HWSEEKCFG;                      //00000028
	HAL_Reg32 ReservedBlk2[1];                //0000002C
	HAL_Reg32 FNSTRPOS;                       //00000030
	HAL_Reg32 ReservedBlk3[1];                //00000034
	HAL_Reg32 RHSTRPOS;                       //00000038
	HAL_Reg32 ACCSTPSIZ;                      //0000003C
	HAL_Reg32 SESTAFULL;                      //00000040
	HAL_Reg32 ReservedBlk4[3];                //00000044
	HAL_Reg32 SEEKEND;                        //00000050
	HAL_Reg32 FEFSLNEG;                       //00000054
	HAL_Reg32 EARZCPROT;                      //00000058
	HAL_Reg32 BRAKELVL;                       //0000005C
	HAL_Reg32 KICKLVL;                        //00000060
	HAL_Reg32 SPEEDLVL;                       //00000064
	HAL_Reg32 BRAKEHLD;                       //00000068
	HAL_Reg32 BRAKECNT;                       //0000006C
	HAL_Reg32 DLFOOSW;                        //00000070
	HAL_Reg32 ReservedBlk5[3];                //00000074
	HAL_Reg32 STPGEN;                         //00000080
	HAL_Reg32 STPSIZE;                        //00000084
	HAL_Reg32 STPDCO;                         //00000088
	HAL_Reg32 STPADD;                         //0000008C
	HAL_Reg32 STPDAC;                         //00000090
	HAL_Reg32 ReservedBlk6[1];                //00000094
	HAL_Reg32 STPPOS;                         //00000098
	HAL_Reg32 STPCTRL;                        //0000009C
	HAL_Reg32 STPPWR;                         //000000A0
	HAL_Reg32 ReservedBlk7[3];                //000000A4
	HAL_Reg32 HYSLTHLD;                       //000000B0
	HAL_Reg32 HYSHTHLD;                       //000000B4
	HAL_Reg32 ReservedBlk8[18];               //000000B8
	HAL_Reg32 FOODAT;                         //00000100
	HAL_Reg32 DROPCTL;                        //00000104
	HAL_Reg32 TRODAT;                         //00000108
	HAL_Reg32 DROPSBY;                        //0000010C
	HAL_Reg32 TILTDAT;                        //00000110
	HAL_Reg32 ReservedBlk9[1];                //00000114
	HAL_Reg32 DACSW;                          //00000118
	HAL_Reg32 FETHR;                          //0000011C
	HAL_Reg32 FETHRCTL;                       //00000120
	HAL_Reg32 ReservedBlk10[1];               //00000124
	HAL_Reg32 SBYLVL;                         //00000128
	HAL_Reg32 AWMASK;                         //0000012C
	HAL_Reg32 ReservedBlk11[2];               //00000130
	HAL_Reg32 AFCRNG1;                        //00000138
	HAL_Reg32 AFCRNG2;                        //0000013C
	HAL_Reg32 SVMODE0;                        //00000140
	HAL_Reg32 SVMODE1;                        //00000144
	HAL_Reg32 FGCMP;                          //00000148
	HAL_Reg32 ReservedBlk12[1];               //0000014C
	HAL_Reg32 WBLCMP;                         //00000150
	HAL_Reg32 ReservedBlk13[1];               //00000154
	HAL_Reg32 DECCMP;                         //00000158
	HAL_Reg32 ReservedBlk14[1];               //0000015C
	HAL_Reg32 AKICLVL;                        //00000160
	HAL_Reg32 ReservedBlk15[1];               //00000164
	HAL_Reg32 WBLRNG;                         //00000168
	HAL_Reg32 ReservedBlk16[1];               //0000016C
	HAL_Reg32 WAITWBL;                        //00000170
	HAL_Reg32 APCGAIN;                        //00000174
	HAL_Reg32 APCNUM;                         //00000178
	HAL_Reg32 OSCFGMAX;                       //0000017C
	HAL_Reg32 TESODCTL;                       //00000180
	HAL_Reg32 TESOFMIN;                       //00000184
	HAL_Reg32 TESOFMAX;                       //00000188
	HAL_Reg32 TESOAM;                         //0000018C
	HAL_Reg32 TESOWD;                         //00000190
	HAL_Reg32 TESOCNT;                        //00000194
	HAL_Reg32 TESOCTHR;                       //00000198
	HAL_Reg32 ReservedBlk17[1];               //0000019C
	HAL_Reg32 FE1SODCTL;                      //000001A0
	HAL_Reg32 FE1SOFMIN;                      //000001A4
	HAL_Reg32 FE1SOFMAX;                      //000001A8
	HAL_Reg32 FE1SOAM;                        //000001AC
	HAL_Reg32 FE1SOWD;                        //000001B0
	HAL_Reg32 FE1SOCNT;                       //000001B4
	HAL_Reg32 FE1SOCTHR;                      //000001B8
	HAL_Reg32 ReservedBlk18[1];               //000001BC
	HAL_Reg32 FE2SODCTL;                      //000001C0
	HAL_Reg32 FE2SOFMIN;                      //000001C4
	HAL_Reg32 FE2SOFMAX;                      //000001C8
	HAL_Reg32 FE2SOAM;                        //000001CC
	HAL_Reg32 FE2SOWD;                        //000001D0
	HAL_Reg32 FE2SOCNT;                       //000001D4
	HAL_Reg32 FE2SOCTHR;                      //000001D8
	HAL_Reg32 SPINCTL;                        //000001DC
	HAL_Reg32 ReservedBlk19[2];               //000001E0
	HAL_Reg32 CAVM;                           //000001E8
	HAL_Reg32 ReservedBlk20[1];               //000001EC
	HAL_Reg32 CLVN;                           //000001F0
	HAL_Reg32 ReservedBlk21[1];               //000001F4
	HAL_Reg32 FGRNG;                          //000001F8
	HAL_Reg32 ReservedBlk22[1];               //000001FC
	HAL_Reg32 REVCNT;                         //00000200
	HAL_Reg32 ReservedBlk23[1];               //00000204
	HAL_Reg32 INITR;                          //00000208
	HAL_Reg32 PUSTP;                          //0000020C
	HAL_Reg32 PUFWD;                          //00000210
	HAL_Reg32 PUBWD;                          //00000214
	HAL_Reg32 LDON;                           //00000218
	HAL_Reg32 LDOFF;                          //0000021C
	HAL_Reg32 DMSV;                           //00000220
	HAL_Reg32 DMOFF;                          //00000224
	HAL_Reg32 FOLNSUP;                        //00000228
	HAL_Reg32 FOLNSDN;                        //0000022C
	HAL_Reg32 DMFBK;                          //00000230
	HAL_Reg32 PAC;                            //00000234
	HAL_Reg32 PAF;                            //00000238
	HAL_Reg32 SLED;                           //0000023C
	HAL_Reg32 FSCNTL;                         //00000240
	HAL_Reg32 FSTHRES;                        //00000244
	HAL_Reg32 SHCCTS;                         //00000248
	HAL_Reg32 CTTHRES;                        //0000024C
	HAL_Reg32 SYNCNT;                         //00000250
	HAL_Reg32 ReservedBlk24[1];               //00000254
	HAL_Reg32 SYNTHR;                         //00000258
	HAL_Reg32 ReservedBlk25[4];               //0000025C
	HAL_Reg32 TILTOFS;                        //0000026C
	HAL_Reg32 ReservedBlk26[4];               //00000270
	HAL_Reg32 FVKICM;                         //00000280
	HAL_Reg32 FVKICK;                         //00000284
	HAL_Reg32 ReservedBlk27[5];               //00000288
	HAL_Reg32 GPDACMUX;                       //0000029C
	HAL_Reg32 CTN;                            //000002A0
	HAL_Reg32 ReservedBlk28[1];               //000002A4
	HAL_Reg32 DACDAT;                         //000002A8
	HAL_Reg32 ReservedBlk29[3];               //000002AC
	HAL_Reg32 AFC;                            //000002B8
	HAL_Reg32 APC;                            //000002BC
	HAL_Reg32 DMODAC;                         //000002C0
	HAL_Reg32 SRCIN;                          //000002C4
	HAL_Reg32 DLCNT;                          //000002C8
	HAL_Reg32 ReservedBlk30[1];               //000002CC
	HAL_Reg32 SDLCNT;                         //000002D0
	HAL_Reg32 ReservedBlk31[1];               //000002D4
	HAL_Reg32 DLCNTF;                         //000002D8
	HAL_Reg32 ReservedBlk32[1];               //000002DC
	HAL_Reg32 SVSTATUS;                       //000002E0
	HAL_Reg32 ReservedBlk33[1];               //000002E4
	HAL_Reg32 SRCR;                           //000002E8
	HAL_Reg32 ReservedBlk34[5];               //000002EC
	HAL_Reg32 CKICH;                          //00000300
	HAL_Reg32 CKICL;                          //00000304
	HAL_Reg32 DEADZ;                          //00000308
	HAL_Reg32 WGHOLDC;                        //0000030C
	HAL_Reg32 DLTHR;                          //00000310
	HAL_Reg32 DLPRG;                          //00000314
	HAL_Reg32 SLEWRATE;                       //00000318
	HAL_Reg32 PNCBKR;                         //0000031C
	HAL_Reg32 TXSPEED;                        //00000320
	HAL_Reg32 ReservedBlk35[3];               //00000324
	HAL_Reg32 DFCTBL;                         //00000330
	HAL_Reg32 DFCTBT;                         //00000334
	HAL_Reg32 ReservedBlk36[2];               //00000338
	HAL_Reg32 FBA;                            //00000340
	HAL_Reg32 FGA;                            //00000344
	HAL_Reg32 ReservedBlk37[1];               //00000348
	HAL_Reg32 TGA;                            //0000034C
	HAL_Reg32 FTBAST;                         //00000350
	HAL_Reg32 FGASET;                         //00000354
	HAL_Reg32 TGASET;                         //00000358
	HAL_Reg32 ReservedBlk38[1];               //0000035C
	HAL_Reg32 DASET;                          //00000360
	HAL_Reg32 ReservedBlk39[1];               //00000364
	HAL_Reg32 RGSEL;                          //00000368
	HAL_Reg32 MASK;                           //0000036C
	HAL_Reg32 TROFFSET;                       //00000370
	HAL_Reg32 DLFSKLVL;                       //00000374
	HAL_Reg32 DLFSBLVL;                       //00000378
	HAL_Reg32 FOOFFSET;                       //0000037C
	HAL_Reg32 FSCNT;                          //00000380
	HAL_Reg32 FOCUS;                          //00000384
	HAL_Reg32 ReservedBlk40[1];               //00000388
	HAL_Reg32 SHOCK;                          //0000038C
	HAL_Reg32 SREG1;                          //00000390
	HAL_Reg32 SREG2;                          //00000394
	HAL_Reg32 FVSET;                          //00000398
	HAL_Reg32 SVSET;                          //0000039C
	HAL_Reg32 SVMIOSEL;                       //000003A0
	HAL_Reg32 ReservedBlk41[1];               //000003A4
	HAL_Reg32 DBGBUSSEL;                      //000003A8
	HAL_Reg32 DMCNT;                          //000003AC
	HAL_Reg32 DFCTSEL;                        //000003B0
	HAL_Reg32 CNTRL;                          //000003B4
	HAL_Reg32 DVDRHLD;                        //000003B8
	HAL_Reg32 CDRWSEEK;                       //000003BC
	HAL_Reg32 ReservedBlk42[4];               //000003C0
	HAL_Reg32 FOCUS1;                         //000003D0
	HAL_Reg32 SRVTEST1;                       //000003D4
	HAL_Reg32 SRVTEST2;                       //000003D8
	HAL_Reg32 ReservedBlk43[1];               //000003DC
	HAL_Reg32 SDINTEN;                        //000003E0
	HAL_Reg32 ReservedBlk44[1];               //000003E4
	HAL_Reg32 SDINT;                          //000003E8
	HAL_Reg32 ReservedBlk45[3];               //000003EC
	HAL_Reg32 SVSPARE;                        //000003F8
	HAL_Reg32 ReservedBlk46[1];               //000003FC
	HAL_Reg32 LCCTL;                          //00000400
	HAL_Reg32 ReservedBlk47[3];               //00000404
	HAL_Reg32 LCTHRESH;                       //00000410
	HAL_Reg32 LCDELTA0;                       //00000414
	HAL_Reg32 LCDELTA1;                       //00000418
	HAL_Reg32 LCDELTA2;                       //0000041C
	HAL_Reg32 LCTIMDELAY;                     //00000420
	HAL_Reg32 ReservedBlk48[7];               //00000424
	HAL_Reg32 LCINTEN;                        //00000440
	HAL_Reg32 LCIRQ;                          //00000444
	HAL_Reg32 ReservedBlk49[2];               //00000448
	HAL_Reg32 LCSTATUS;                       //00000450
	HAL_Reg32 INHIBITTX;                      //00000454
	HAL_Reg32 FOLNSTP;                        //00000458
	HAL_Reg32 ReservedBlk50[1];               //0000045C
	HAL_Reg32 PWMSEL;                         //00000460
	HAL_Reg32 ReservedBlk51[3];               //00000464
	HAL_Reg32 DSPMODE;                        //00000470
	HAL_Reg32 FGSRC;                          //00000474
	HAL_Reg32 SRCSTA;                         //00000478
	HAL_Reg32 PHAADDR;                        //0000047C
	HAL_Reg32 PHB1ADDR;                       //00000480
	HAL_Reg32 PHB2ADDR;                       //00000484
	HAL_Reg32 PHC1ADDR;                       //00000488
	HAL_Reg32 PHC2ADDR;                       //0000048C
	HAL_Reg32 PHC3ADDR;                       //00000490
	HAL_Reg32 PHC4ADDR;                       //00000494
	HAL_Reg32 PHDADDR;                        //00000498
	HAL_Reg32 RAMTEST;                        //0000049C
	HAL_Reg32 SEQ_PC;                         //000004A0
	HAL_Reg32 FWCTRL;                         //000004A4
	HAL_Reg32 PDMCNT;                         //000004A8
	HAL_Reg32 PDMALPHA;                       //000004AC
	HAL_Reg32 STMCTRL;                        //000004B0
	HAL_Reg32 TESRCCTL;                       //000004B4
	HAL_Reg32 PDMVREF;                        //000004B8
	HAL_Reg32 SWOFFEN;                        //000004BC
	HAL_Reg32 ReservedBlk52[4];               //000004C0
	HAL_Reg32 DSPBADELCT;                     //000004D0
	HAL_Reg32 LANDCTRL;                       //000004D4
	HAL_Reg32 LSPDMCTRL1;                     //000004D8
	HAL_Reg32 LSPDMCTRL2;                     //000004DC
	HAL_Reg32 LSMSKCTL;                       //000004E0
};

#endif // #if (CHIP_REV >= 0xC0)

// Register SEEKTBADD
#define HAL_SRV_SEKTBAD_REGISTER    (HAL_SRVREG_MAP->SEEKTBADD)
#define HAL_SRV_SEKTBAD_LSB         0
#define HAL_SRV_SEKTBAD_MSB         3
#define HAL_SRV_SEKTBAD_MASK        0x0000000F
#define HAL_SRV_SEKTBAD_TYPE        halSRV_SEKTBAD_SpdPhAcc_t
#define HAL_SRV_SEKTBAD             HAL_SRV_SEKTBAD_MASK
#define HAL_SRV_SEKTBAD_READABLE    1
#define HAL_SRV_SEKTBAD_WRITABLE    1
#define HAL_SRV_SEKTBAD_SIGNED      0
#define HAL_SRV_SEKTBAD_ADR         0xB0000000

// Register SPEDPHVAL

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_SPDVAL_REGISTER    (HAL_SRVREG_MAP->SPEDPHVAL)
#define HAL_SRV_SPDVAL_LSB         0
#define HAL_SRV_SPDVAL_MSB         5
#define HAL_SRV_SPDVAL_MASK        0x0000003F
#define HAL_SRV_SPDVAL_TYPE        UByte
#define HAL_SRV_SPDVAL             HAL_SRV_SPDVAL_MASK
#define HAL_SRV_SPDVAL_READABLE    1
#define HAL_SRV_SPDVAL_WRITABLE    1
#define HAL_SRV_SPDVAL_SIGNED      0
#define HAL_SRV_SPDVAL_ADR         0xB0000004

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_SPDVALD_REGISTER    (HAL_SRVREG_MAP->SPEDPHVAL)
#define HAL_SRV_SPDVALD_LSB         16
#define HAL_SRV_SPDVALD_MSB         23
#define HAL_SRV_SPDVALD_MASK        0x00FF0000
#define HAL_SRV_SPDVALD_TYPE        UByte
#define HAL_SRV_SPDVALD             HAL_SRV_SPDVALD_MASK
#define HAL_SRV_SPDVALD_READABLE    1
#define HAL_SRV_SPDVALD_WRITABLE    1
#define HAL_SRV_SPDVALD_SIGNED      0
#define HAL_SRV_SPDVALD_ADR         0xB0000004

#define HAL_SRV_SPDVAL_REGISTER    (HAL_SRVREG_MAP->SPEDPHVAL)
#define HAL_SRV_SPDVAL_LSB         0
#define HAL_SRV_SPDVAL_MSB         10
#define HAL_SRV_SPDVAL_MASK        0x000007FF
#define HAL_SRV_SPDVAL_TYPE        UInt16
#define HAL_SRV_SPDVAL             HAL_SRV_SPDVAL_MASK
#define HAL_SRV_SPDVAL_READABLE    1
#define HAL_SRV_SPDVAL_WRITABLE    1
#define HAL_SRV_SPDVAL_SIGNED      0
#define HAL_SRV_SPDVAL_ADR         0xB0000004

#endif // #if (CHIP_REV >= 0xC0)

// Register ACCINTVAL

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_ACCVAL_REGISTER    (HAL_SRVREG_MAP->ACCINTVAL)
#define HAL_SRV_ACCVAL_LSB         0
#define HAL_SRV_ACCVAL_MSB         7
#define HAL_SRV_ACCVAL_MASK        0x000000FF
#define HAL_SRV_ACCVAL_TYPE        UByte
#define HAL_SRV_ACCVAL             HAL_SRV_ACCVAL_MASK
#define HAL_SRV_ACCVAL_READABLE    1
#define HAL_SRV_ACCVAL_WRITABLE    1
#define HAL_SRV_ACCVAL_SIGNED      0
#define HAL_SRV_ACCVAL_ADR         0xB0000008

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_ACCVALD_REGISTER    (HAL_SRVREG_MAP->ACCINTVAL)
#define HAL_SRV_ACCVALD_LSB         16
#define HAL_SRV_ACCVALD_MSB         23
#define HAL_SRV_ACCVALD_MASK        0x00FF0000
#define HAL_SRV_ACCVALD_TYPE        UByte
#define HAL_SRV_ACCVALD             HAL_SRV_ACCVALD_MASK
#define HAL_SRV_ACCVALD_READABLE    1
#define HAL_SRV_ACCVALD_WRITABLE    1
#define HAL_SRV_ACCVALD_SIGNED      0
#define HAL_SRV_ACCVALD_ADR         0xB0000008

#define HAL_SRV_ACCVAL_REGISTER    (HAL_SRVREG_MAP->ACCINTVAL)
#define HAL_SRV_ACCVAL_LSB         0
#define HAL_SRV_ACCVAL_MSB         9
#define HAL_SRV_ACCVAL_MASK        0x000003FF
#define HAL_SRV_ACCVAL_TYPE        UInt16
#define HAL_SRV_ACCVAL             HAL_SRV_ACCVAL_MASK
#define HAL_SRV_ACCVAL_READABLE    1
#define HAL_SRV_ACCVAL_WRITABLE    1
#define HAL_SRV_ACCVAL_SIGNED      0
#define HAL_SRV_ACCVAL_ADR         0xB0000008

#endif // #if (CHIP_REV >= 0xC0)

// Register JUMPDAT
#define HAL_SRV_JUMPDAT_REGISTER    (HAL_SRVREG_MAP->JUMPDAT)
#define HAL_SRV_JUMPDAT_LSB         0
#define HAL_SRV_JUMPDAT_MSB         15
#define HAL_SRV_JUMPDAT_MASK        0x0000FFFF
#define HAL_SRV_JUMPDAT_TYPE        UInt16
#define HAL_SRV_JUMPDAT             HAL_SRV_JUMPDAT_MASK
#define HAL_SRV_JUMPDAT_READABLE    1
#define HAL_SRV_JUMPDAT_WRITABLE    1
#define HAL_SRV_JUMPDAT_SIGNED      0
#define HAL_SRV_JUMPDAT_ADR         0xB0000010

// Register BRAKEDAT
#define HAL_SRV_BAKDAT_REGISTER    (HAL_SRVREG_MAP->BRAKEDAT)
#define HAL_SRV_BAKDAT_LSB         0
#define HAL_SRV_BAKDAT_MSB         15
#define HAL_SRV_BAKDAT_MASK        0x0000FFFF
#define HAL_SRV_BAKDAT_TYPE        UInt16
#define HAL_SRV_BAKDAT             HAL_SRV_BAKDAT_MASK
#define HAL_SRV_BAKDAT_READABLE    1
#define HAL_SRV_BAKDAT_WRITABLE    1
#define HAL_SRV_BAKDAT_SIGNED      0
#define HAL_SRV_BAKDAT_ADR         0xB0000018

#if (CHIP_REV >= 0xC0)

// Register SSPDPHVAL

#define HAL_SRV_SSPDPHVAL_REGISTER    (HAL_SRVREG_MAP->SSPDPHVAL)
#define HAL_SRV_SSPDPHVAL_LSB         0
#define HAL_SRV_SSPDPHVAL_MSB         10
#define HAL_SRV_SSPDPHVAL_MASK        0x000007FF
#define HAL_SRV_SSPDPHVAL_TYPE        UInt16
#define HAL_SRV_SSPDPHVAL             HAL_SRV_SSPDPHVAL_MASK
#define HAL_SRV_SSPDPHVAL_READABLE    1
#define HAL_SRV_SSPDPHVAL_WRITABLE    1
#define HAL_SRV_SSPDPHVAL_SIGNED      0
#define HAL_SRV_SSPDPHVAL_ADR         0xB000001C

#endif // #if (CHIP_REV >= 0xC0)

// Register PHCHGMOD
#define HAL_SRV_PHCGMOD2_REGISTER    (HAL_SRVREG_MAP->PHCHGMOD)
#define HAL_SRV_PHCGMOD2_LSB         4
#define HAL_SRV_PHCGMOD2_MSB         7
#define HAL_SRV_PHCGMOD2_MASK        0x000000F0
#define HAL_SRV_PHCGMOD2_TYPE        UByte
#define HAL_SRV_PHCGMOD2             HAL_SRV_PHCGMOD2_MASK
#define HAL_SRV_PHCGMOD2_READABLE    1
#define HAL_SRV_PHCGMOD2_WRITABLE    1
#define HAL_SRV_PHCGMOD2_SIGNED      0
#define HAL_SRV_PHCGMOD2_ADR         0xB0000020

#define HAL_SRV_PHCGMOD1_REGISTER    (HAL_SRVREG_MAP->PHCHGMOD)
#define HAL_SRV_PHCGMOD1_LSB         0
#define HAL_SRV_PHCGMOD1_MSB         3
#define HAL_SRV_PHCGMOD1_MASK        0x0000000F
#define HAL_SRV_PHCGMOD1_TYPE        UByte
#define HAL_SRV_PHCGMOD1             HAL_SRV_PHCGMOD1_MASK
#define HAL_SRV_PHCGMOD1_READABLE    1
#define HAL_SRV_PHCGMOD1_WRITABLE    1
#define HAL_SRV_PHCGMOD1_SIGNED      0
#define HAL_SRV_PHCGMOD1_ADR         0xB0000020

// Register TIMPER
#define HAL_SRV_TIMPER_REGISTER    (HAL_SRVREG_MAP->TIMPER)
#define HAL_SRV_TIMPER_LSB         0
#define HAL_SRV_TIMPER_MSB         7
#define HAL_SRV_TIMPER_MASK        0x000000FF
#define HAL_SRV_TIMPER_TYPE        UByte
#define HAL_SRV_TIMPER             HAL_SRV_TIMPER_MASK
#define HAL_SRV_TIMPER_READABLE    1
#define HAL_SRV_TIMPER_WRITABLE    1
#define HAL_SRV_TIMPER_SIGNED      0
#define HAL_SRV_TIMPER_ADR         0xB0000024

// Register HWSEEKCFG
#define HAL_SRV_HWSTPSEEK_REGISTER    (HAL_SRVREG_MAP->HWSEEKCFG)
#define HAL_SRV_HWSTPSEEK_LSB         7
#define HAL_SRV_HWSTPSEEK_MSB         7
#define HAL_SRV_HWSTPSEEK_MASK        0x00000080
#define HAL_SRV_HWSTPSEEK_TYPE        Bool
#define HAL_SRV_HWSTPSEEK             HAL_SRV_HWSTPSEEK_MASK
#define HAL_SRV_HWSTPSEEK_READABLE    1
#define HAL_SRV_HWSTPSEEK_WRITABLE    1
#define HAL_SRV_HWSTPSEEK_SIGNED      0
#define HAL_SRV_HWSTPSEEK_ADR         0xB0000028

#define HAL_SRV_FBWARD_REGISTER    (HAL_SRVREG_MAP->HWSEEKCFG)
#define HAL_SRV_FBWARD_LSB         6
#define HAL_SRV_FBWARD_MSB         6
#define HAL_SRV_FBWARD_MASK        0x00000040
#define HAL_SRV_FBWARD_TYPE        Bool
#define HAL_SRV_FBWARD             HAL_SRV_FBWARD_MASK
#define HAL_SRV_FBWARD_READABLE    1
#define HAL_SRV_FBWARD_WRITABLE    1
#define HAL_SRV_FBWARD_SIGNED      0
#define HAL_SRV_FBWARD_ADR         0xB0000028

#define HAL_SRV_MAXSPEED_REGISTER    (HAL_SRVREG_MAP->HWSEEKCFG)
#define HAL_SRV_MAXSPEED_LSB         0
#define HAL_SRV_MAXSPEED_MSB         3
#define HAL_SRV_MAXSPEED_MASK        0x0000000F
#define HAL_SRV_MAXSPEED_TYPE        UByte
#define HAL_SRV_MAXSPEED             HAL_SRV_MAXSPEED_MASK
#define HAL_SRV_MAXSPEED_READABLE    1
#define HAL_SRV_MAXSPEED_WRITABLE    1
#define HAL_SRV_MAXSPEED_SIGNED      0
#define HAL_SRV_MAXSPEED_ADR         0xB0000028

#if (CHIP_REV <= 0xB3)

// Register STRLOC
#define HAL_SRV_STRLOC_REGISTER    (HAL_SRVREG_MAP->STRLOC)
#define HAL_SRV_STRLOC_LSB         0
#define HAL_SRV_STRLOC_MSB         15
#define HAL_SRV_STRLOC_MASK        0x0000FFFF
#define HAL_SRV_STRLOC_TYPE        UInt16
#define HAL_SRV_STRLOC             HAL_SRV_STRLOC_MASK
#define HAL_SRV_STRLOC_READABLE    1
#define HAL_SRV_STRLOC_WRITABLE    1
#define HAL_SRV_STRLOC_SIGNED      0
#define HAL_SRV_STRLOC_ADR         0xB0000030

// Register STRSTPOT
#define HAL_SRV_STRSTPOT_REGISTER    (HAL_SRVREG_MAP->STRSTPOT)
#define HAL_SRV_STRSTPOT_LSB         0
#define HAL_SRV_STRSTPOT_MSB         5
#define HAL_SRV_STRSTPOT_MASK        0x0000003F
#define HAL_SRV_STRSTPOT_TYPE        UByte
#define HAL_SRV_STRSTPOT             HAL_SRV_STRSTPOT_MASK
#define HAL_SRV_STRSTPOT_READABLE    1
#define HAL_SRV_STRSTPOT_WRITABLE    1
#define HAL_SRV_STRSTPOT_SIGNED      0
#define HAL_SRV_STRSTPOT_ADR         0xB0000038

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

// Register FNSTRPOS
#define HAL_SRV_FNSTRLOC_REGISTER    (HAL_SRVREG_MAP->FNSTRPOS)
#define HAL_SRV_FNSTRLOC_LSB         16
#define HAL_SRV_FNSTRLOC_MSB         31
#define HAL_SRV_FNSTRLOC_MASK        0xFFFF0000
#define HAL_SRV_FNSTRLOC_TYPE        UInt16
#define HAL_SRV_FNSTRLOC             HAL_SRV_FNSTRLOC_MASK
#define HAL_SRV_FNSTRLOC_READABLE    1
#define HAL_SRV_FNSTRLOC_WRITABLE    1
#define HAL_SRV_FNSTRLOC_SIGNED      0
#define HAL_SRV_FNSTRLOC_ADR         0xB0000030

#define HAL_SRV_FNSTRPOT_REGISTER    (HAL_SRVREG_MAP->FNSTRPOS)
#define HAL_SRV_FNSTRPOT_LSB         0
#define HAL_SRV_FNSTRPOT_MSB         6
#define HAL_SRV_FNSTRPOT_MASK        0x0000007F
#define HAL_SRV_FNSTRPOT_TYPE        UByte
#define HAL_SRV_FNSTRPOT             HAL_SRV_FNSTRPOT_MASK
#define HAL_SRV_FNSTRPOT_READABLE    1
#define HAL_SRV_FNSTRPOT_WRITABLE    1
#define HAL_SRV_FNSTRPOT_SIGNED      0
#define HAL_SRV_FNSTRPOT_ADR         0xB0000030

// Register RHSTRPOS
#define HAL_SRV_RHSTRLOC_REGISTER    (HAL_SRVREG_MAP->RHSTRPOS)
#define HAL_SRV_RHSTRLOC_LSB         16
#define HAL_SRV_RHSTRLOC_MSB         31
#define HAL_SRV_RHSTRLOC_MASK        0xFFFF0000
#define HAL_SRV_RHSTRLOC_TYPE        UInt16
#define HAL_SRV_RHSTRLOC             HAL_SRV_RHSTRLOC_MASK
#define HAL_SRV_RHSTRLOC_READABLE    1
#define HAL_SRV_RHSTRLOC_WRITABLE    1
#define HAL_SRV_RHSTRLOC_SIGNED      0
#define HAL_SRV_RHSTRLOC_ADR         0xB0000038

#define HAL_SRV_RHSTRPOT_REGISTER    (HAL_SRVREG_MAP->RHSTRPOS)
#define HAL_SRV_RHSTRPOT_LSB         0
#define HAL_SRV_RHSTRPOT_MSB         6
#define HAL_SRV_RHSTRPOT_MASK        0x0000007F
#define HAL_SRV_RHSTRPOT_TYPE        UByte
#define HAL_SRV_RHSTRPOT             HAL_SRV_RHSTRPOT_MASK
#define HAL_SRV_RHSTRPOT_READABLE    1
#define HAL_SRV_RHSTRPOT_WRITABLE    1
#define HAL_SRV_RHSTRPOT_SIGNED      0
#define HAL_SRV_RHSTRPOT_ADR         0xB0000038

#endif // #if (CHIP_REV >= 0xC0)

// Register ACCSTPSIZ
#define HAL_SRV_ACCSTP2_REGISTER    (HAL_SRVREG_MAP->ACCSTPSIZ)
#define HAL_SRV_ACCSTP2_LSB         4
#define HAL_SRV_ACCSTP2_MSB         7
#define HAL_SRV_ACCSTP2_MASK        0x000000F0
#define HAL_SRV_ACCSTP2_TYPE        UByte
#define HAL_SRV_ACCSTP2             HAL_SRV_ACCSTP2_MASK
#define HAL_SRV_ACCSTP2_READABLE    1
#define HAL_SRV_ACCSTP2_WRITABLE    1
#define HAL_SRV_ACCSTP2_SIGNED      0
#define HAL_SRV_ACCSTP2_ADR         0xB000003C

#define HAL_SRV_ACCSTP1_REGISTER    (HAL_SRVREG_MAP->ACCSTPSIZ)
#define HAL_SRV_ACCSTP1_LSB         0
#define HAL_SRV_ACCSTP1_MSB         3
#define HAL_SRV_ACCSTP1_MASK        0x0000000F
#define HAL_SRV_ACCSTP1_TYPE        UByte
#define HAL_SRV_ACCSTP1             HAL_SRV_ACCSTP1_MASK
#define HAL_SRV_ACCSTP1_READABLE    1
#define HAL_SRV_ACCSTP1_WRITABLE    1
#define HAL_SRV_ACCSTP1_SIGNED      0
#define HAL_SRV_ACCSTP1_ADR         0xB000003C

// Register SESTAFULL
#define HAL_SRV_SEEKSTART_REGISTER    (HAL_SRVREG_MAP->SESTAFULL)
#define HAL_SRV_SEEKSTART_LSB         7
#define HAL_SRV_SEEKSTART_MSB         7
#define HAL_SRV_SEEKSTART_MASK        0x00000080
#define HAL_SRV_SEEKSTART_TYPE        Bool
#define HAL_SRV_SEEKSTART             HAL_SRV_SEEKSTART_MASK
#define HAL_SRV_SEEKSTART_READABLE    1
#define HAL_SRV_SEEKSTART_WRITABLE    1
#define HAL_SRV_SEEKSTART_SIGNED      0
#define HAL_SRV_SEEKSTART_ADR         0xB0000040

#define HAL_SRV_SQUAREDIS_REGISTER    (HAL_SRVREG_MAP->SESTAFULL)
#define HAL_SRV_SQUAREDIS_LSB         6
#define HAL_SRV_SQUAREDIS_MSB         6
#define HAL_SRV_SQUAREDIS_MASK        0x00000040
#define HAL_SRV_SQUAREDIS_TYPE        Bool
#define HAL_SRV_SQUAREDIS             HAL_SRV_SQUAREDIS_MASK
#define HAL_SRV_SQUAREDIS_READABLE    1
#define HAL_SRV_SQUAREDIS_WRITABLE    1
#define HAL_SRV_SQUAREDIS_SIGNED      0
#define HAL_SRV_SQUAREDIS_ADR         0xB0000040

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_FULSTP_REGISTER    (HAL_SRVREG_MAP->SESTAFULL)
#define HAL_SRV_FULSTP_LSB         0
#define HAL_SRV_FULSTP_MSB         5
#define HAL_SRV_FULSTP_MASK        0x0000003F
#define HAL_SRV_FULSTP_TYPE        UByte
#define HAL_SRV_FULSTP             HAL_SRV_FULSTP_MASK
#define HAL_SRV_FULSTP_READABLE    1
#define HAL_SRV_FULSTP_WRITABLE    1
#define HAL_SRV_FULSTP_SIGNED      0
#define HAL_SRV_FULSTP_ADR         0xB0000040

#endif // #if (CHIP_REV >= 0xC0)

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_FULSTP_LSB         0
#define HAL_SRV_FULSTP_MSB         4
#define HAL_SRV_FULSTP_MASK        0x0000001F
#define HAL_SRV_FULSTP_TYPE        UByte
#define HAL_SRV_FULSTP             HAL_SRV_FULSTP_MASK
#define HAL_SRV_FULSTP_READABLE    1
#define HAL_SRV_FULSTP_WRITABLE    1
#define HAL_SRV_FULSTP_SIGNED      0
#define HAL_SRV_FULSTP_ADR         0xB0000040

// Register CURLOC
#define HAL_SRV_CURLOC_REGISTER    (HAL_SRVREG_MAP->CURLOC)
#define HAL_SRV_CURLOC_LSB         0
#define HAL_SRV_CURLOC_MSB         15
#define HAL_SRV_CURLOC_MASK        0x0000FFFF
#define HAL_SRV_CURLOC_TYPE        UInt16
#define HAL_SRV_CURLOC             HAL_SRV_CURLOC_MASK
#define HAL_SRV_CURLOC_READABLE    1
#define HAL_SRV_CURLOC_WRITABLE    0
#define HAL_SRV_CURLOC_SIGNED      0
#define HAL_SRV_CURLOC_ADR         0xB0000048

#endif // #if (CHIP_REV <= 0xB3)

// Register SEEKEND
#define HAL_SRV_SEEKSTOP_REGISTER    (HAL_SRVREG_MAP->SEEKEND)
#define HAL_SRV_SEEKSTOP_LSB         7
#define HAL_SRV_SEEKSTOP_MSB         7
#define HAL_SRV_SEEKSTOP_MASK        0x00000080
#define HAL_SRV_SEEKSTOP_TYPE        Bool
#define HAL_SRV_SEEKSTOP             HAL_SRV_SEEKSTOP_MASK
#define HAL_SRV_SEEKSTOP_READABLE    1
#define HAL_SRV_SEEKSTOP_WRITABLE    0
#define HAL_SRV_SEEKSTOP_SIGNED      0
#define HAL_SRV_SEEKSTOP_ADR         0xB0000050

#define HAL_SRV_PROTACT_REGISTER    (HAL_SRVREG_MAP->SEEKEND)
#define HAL_SRV_PROTACT_LSB         3
#define HAL_SRV_PROTACT_MSB         3
#define HAL_SRV_PROTACT_MASK        0x00000008
#define HAL_SRV_PROTACT_TYPE        Bool
#define HAL_SRV_PROTACT             HAL_SRV_PROTACT_MASK
#define HAL_SRV_PROTACT_READABLE    1
#define HAL_SRV_PROTACT_WRITABLE    0
#define HAL_SRV_PROTACT_SIGNED      0
#define HAL_SRV_PROTACT_ADR         0xB0000050

// Register FEFSLNEG
#define HAL_SRV_FSLNEG_REGISTER    (HAL_SRVREG_MAP->FEFSLNEG)
#define HAL_SRV_FSLNEG_LSB         0
#define HAL_SRV_FSLNEG_MSB         6
#define HAL_SRV_FSLNEG_MASK        0x0000007F
#define HAL_SRV_FSLNEG_TYPE        UByte
#define HAL_SRV_FSLNEG             HAL_SRV_FSLNEG_MASK
#define HAL_SRV_FSLNEG_READABLE    1
#define HAL_SRV_FSLNEG_WRITABLE    1
#define HAL_SRV_FSLNEG_SIGNED      0
#define HAL_SRV_FSLNEG_ADR         0xB0000054

// Register EARZCPROT
#define HAL_SRV_ZCEAREN_REGISTER    (HAL_SRVREG_MAP->EARZCPROT)
#define HAL_SRV_ZCEAREN_LSB         6
#define HAL_SRV_ZCEAREN_MSB         6
#define HAL_SRV_ZCEAREN_MASK        0x00000040
#define HAL_SRV_ZCEAREN_TYPE        Bool
#define HAL_SRV_ZCEAREN             HAL_SRV_ZCEAREN_MASK
#define HAL_SRV_ZCEAREN_READABLE    1
#define HAL_SRV_ZCEAREN_WRITABLE    1
#define HAL_SRV_ZCEAREN_SIGNED      0
#define HAL_SRV_ZCEAREN_ADR         0xB0000058

#define HAL_SRV_ZCSEL_REGISTER    (HAL_SRVREG_MAP->EARZCPROT)
#define HAL_SRV_ZCSEL_LSB         4
#define HAL_SRV_ZCSEL_MSB         5
#define HAL_SRV_ZCSEL_MASK        0x00000030
#define HAL_SRV_ZCSEL_TYPE        halSRV_ZCSELScaling_t
#define HAL_SRV_ZCSEL             HAL_SRV_ZCSEL_MASK
#define HAL_SRV_ZCSEL_READABLE    1
#define HAL_SRV_ZCSEL_WRITABLE    1
#define HAL_SRV_ZCSEL_SIGNED      0
#define HAL_SRV_ZCSEL_ADR         0xB0000058

#define HAL_SRV_PROTECT_REGISTER    (HAL_SRVREG_MAP->EARZCPROT)
#define HAL_SRV_PROTECT_LSB         2
#define HAL_SRV_PROTECT_MSB         3
#define HAL_SRV_PROTECT_MASK        0x0000000C
#define HAL_SRV_PROTECT_TYPE        halSRV_PROTECT_t
#define HAL_SRV_PROTECT             HAL_SRV_PROTECT_MASK
#define HAL_SRV_PROTECT_READABLE    1
#define HAL_SRV_PROTECT_WRITABLE    1
#define HAL_SRV_PROTECT_SIGNED      0
#define HAL_SRV_PROTECT_ADR         0xB0000058

#define HAL_SRV_KCKBRKSEL_REGISTER    (HAL_SRVREG_MAP->EARZCPROT)
#define HAL_SRV_KCKBRKSEL_LSB         0
#define HAL_SRV_KCKBRKSEL_MSB         1
#define HAL_SRV_KCKBRKSEL_MASK        0x00000003
#define HAL_SRV_KCKBRKSEL_TYPE        halSRV_KCKBRKSEL_t
#define HAL_SRV_KCKBRKSEL             HAL_SRV_KCKBRKSEL_MASK
#define HAL_SRV_KCKBRKSEL_READABLE    1
#define HAL_SRV_KCKBRKSEL_WRITABLE    1
#define HAL_SRV_KCKBRKSEL_SIGNED      0
#define HAL_SRV_KCKBRKSEL_ADR         0xB0000058

// Register BRAKELVL
#define HAL_SRV_BRAKELVL_REGISTER    (HAL_SRVREG_MAP->BRAKELVL)
#define HAL_SRV_BRAKELVL_LSB         0
#define HAL_SRV_BRAKELVL_MSB         7
#define HAL_SRV_BRAKELVL_MASK        0x000000FF
#define HAL_SRV_BRAKELVL_TYPE        SByte
#define HAL_SRV_BRAKELVL             HAL_SRV_BRAKELVL_MASK
#define HAL_SRV_BRAKELVL_READABLE    1
#define HAL_SRV_BRAKELVL_WRITABLE    1
#define HAL_SRV_BRAKELVL_SIGNED      1
#define HAL_SRV_BRAKELVL_ADR         0xB000005C

// Register KICKLVL
#define HAL_SRV_KICKLVL_REGISTER    (HAL_SRVREG_MAP->KICKLVL)
#define HAL_SRV_KICKLVL_LSB         0
#define HAL_SRV_KICKLVL_MSB         7
#define HAL_SRV_KICKLVL_MASK        0x000000FF
#define HAL_SRV_KICKLVL_TYPE        SByte
#define HAL_SRV_KICKLVL             HAL_SRV_KICKLVL_MASK
#define HAL_SRV_KICKLVL_READABLE    1
#define HAL_SRV_KICKLVL_WRITABLE    1
#define HAL_SRV_KICKLVL_SIGNED      1
#define HAL_SRV_KICKLVL_ADR         0xB0000060

// Register SPEEDLVL

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_SPDLIMSK_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_SPDLIMSK_LSB         24
#define HAL_SRV_SPDLIMSK_MSB         27
#define HAL_SRV_SPDLIMSK_MASK        0x0F000000
#define HAL_SRV_SPDLIMSK_TYPE        UByte
#define HAL_SRV_SPDLIMSK             HAL_SRV_SPDLIMSK_MASK
#define HAL_SRV_SPDLIMSK_READABLE    1
#define HAL_SRV_SPDLIMSK_WRITABLE    1
#define HAL_SRV_SPDLIMSK_SIGNED      0
#define HAL_SRV_SPDLIMSK_ADR         0xB0000064

#define HAL_SRV_BRKLIM_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_BRKLIM_LSB         16
#define HAL_SRV_BRKLIM_MSB         23
#define HAL_SRV_BRKLIM_MASK        0x00FF0000
#define HAL_SRV_BRKLIM_TYPE        UByte
#define HAL_SRV_BRKLIM             HAL_SRV_BRKLIM_MASK
#define HAL_SRV_BRKLIM_READABLE    1
#define HAL_SRV_BRKLIM_WRITABLE    1
#define HAL_SRV_BRKLIM_SIGNED      0
#define HAL_SRV_BRKLIM_ADR         0xB0000064

#define HAL_SRV_SLOWZERO_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_SLOWZERO_LSB         15
#define HAL_SRV_SLOWZERO_MSB         15
#define HAL_SRV_SLOWZERO_MASK        0x00008000
#define HAL_SRV_SLOWZERO_TYPE        Bool
#define HAL_SRV_SLOWZERO             HAL_SRV_SLOWZERO_MASK
#define HAL_SRV_SLOWZERO_READABLE    1
#define HAL_SRV_SLOWZERO_WRITABLE    1
#define HAL_SRV_SLOWZERO_SIGNED      0
#define HAL_SRV_SLOWZERO_ADR         0xB0000064

#define HAL_SRV_BRKSLWOE_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_BRKSLWOE_LSB         14
#define HAL_SRV_BRKSLWOE_MSB         14
#define HAL_SRV_BRKSLWOE_MASK        0x00004000
#define HAL_SRV_BRKSLWOE_TYPE        Bool
#define HAL_SRV_BRKSLWOE             HAL_SRV_BRKSLWOE_MASK
#define HAL_SRV_BRKSLWOE_READABLE    1
#define HAL_SRV_BRKSLWOE_WRITABLE    1
#define HAL_SRV_BRKSLWOE_SIGNED      0
#define HAL_SRV_BRKSLWOE_ADR         0xB0000064

#define HAL_SRV_BRKDEL_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_BRKDEL_LSB         13
#define HAL_SRV_BRKDEL_MSB         13
#define HAL_SRV_BRKDEL_MASK        0x00002000
#define HAL_SRV_BRKDEL_TYPE        Bool
#define HAL_SRV_BRKDEL             HAL_SRV_BRKDEL_MASK
#define HAL_SRV_BRKDEL_READABLE    1
#define HAL_SRV_BRKDEL_WRITABLE    1
#define HAL_SRV_BRKDEL_SIGNED      0
#define HAL_SRV_BRKDEL_ADR         0xB0000064

#define HAL_SRV_SPDMUTE_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_SPDMUTE_LSB         12
#define HAL_SRV_SPDMUTE_MSB         12
#define HAL_SRV_SPDMUTE_MASK        0x00001000
#define HAL_SRV_SPDMUTE_TYPE        Bool
#define HAL_SRV_SPDMUTE             HAL_SRV_SPDMUTE_MASK
#define HAL_SRV_SPDMUTE_READABLE    1
#define HAL_SRV_SPDMUTE_WRITABLE    1
#define HAL_SRV_SPDMUTE_SIGNED      0
#define HAL_SRV_SPDMUTE_ADR         0xB0000064

#define HAL_SRV_SPDLIMEN_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_SPDLIMEN_LSB         11
#define HAL_SRV_SPDLIMEN_MSB         11
#define HAL_SRV_SPDLIMEN_MASK        0x00000800
#define HAL_SRV_SPDLIMEN_TYPE        Bool
#define HAL_SRV_SPDLIMEN             HAL_SRV_SPDLIMEN_MASK
#define HAL_SRV_SPDLIMEN_READABLE    1
#define HAL_SRV_SPDLIMEN_WRITABLE    1
#define HAL_SRV_SPDLIMEN_SIGNED      0
#define HAL_SRV_SPDLIMEN_ADR         0xB0000064

#define HAL_SRV_BRKMUTE_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_BRKMUTE_LSB         10
#define HAL_SRV_BRKMUTE_MSB         10
#define HAL_SRV_BRKMUTE_MASK        0x00000400
#define HAL_SRV_BRKMUTE_TYPE        Bool
#define HAL_SRV_BRKMUTE             HAL_SRV_BRKMUTE_MASK
#define HAL_SRV_BRKMUTE_READABLE    1
#define HAL_SRV_BRKMUTE_WRITABLE    1
#define HAL_SRV_BRKMUTE_SIGNED      0
#define HAL_SRV_BRKMUTE_ADR         0xB0000064

#define HAL_SRV_BRKLIMEN_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_BRKLIMEN_LSB         9
#define HAL_SRV_BRKLIMEN_MSB         9
#define HAL_SRV_BRKLIMEN_MASK        0x00000200
#define HAL_SRV_BRKLIMEN_TYPE        Bool
#define HAL_SRV_BRKLIMEN             HAL_SRV_BRKLIMEN_MASK
#define HAL_SRV_BRKLIMEN_READABLE    1
#define HAL_SRV_BRKLIMEN_WRITABLE    1
#define HAL_SRV_BRKLIMEN_SIGNED      0
#define HAL_SRV_BRKLIMEN_ADR         0xB0000064

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_ZHOLDEN_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_ZHOLDEN_LSB         8
#define HAL_SRV_ZHOLDEN_MSB         8
#define HAL_SRV_ZHOLDEN_MASK        0x00000100
#define HAL_SRV_ZHOLDEN_TYPE        Bool
#define HAL_SRV_ZHOLDEN             HAL_SRV_ZHOLDEN_MASK
#define HAL_SRV_ZHOLDEN_READABLE    1
#define HAL_SRV_ZHOLDEN_WRITABLE    1
#define HAL_SRV_ZHOLDEN_SIGNED      0
#define HAL_SRV_ZHOLDEN_ADR         0xB0000064

#define HAL_SRV_BRAKESPD_REGISTER    (HAL_SRVREG_MAP->SPEEDLVL)
#define HAL_SRV_BRAKESPD_LSB         0
#define HAL_SRV_BRAKESPD_MSB         7
#define HAL_SRV_BRAKESPD_MASK        0x000000FF
#define HAL_SRV_BRAKESPD_TYPE        UByte
#define HAL_SRV_BRAKESPD             HAL_SRV_BRAKESPD_MASK
#define HAL_SRV_BRAKESPD_READABLE    1
#define HAL_SRV_BRAKESPD_WRITABLE    1
#define HAL_SRV_BRAKESPD_SIGNED      0
#define HAL_SRV_BRAKESPD_ADR         0xB0000064

// Register BRAKEHLD
#define HAL_SRV_BRAKEHLD_REGISTER    (HAL_SRVREG_MAP->BRAKEHLD)
#define HAL_SRV_BRAKEHLD_LSB         0
#define HAL_SRV_BRAKEHLD_MSB         7
#define HAL_SRV_BRAKEHLD_MASK        0x000000FF
#define HAL_SRV_BRAKEHLD_TYPE        UByte
#define HAL_SRV_BRAKEHLD             HAL_SRV_BRAKEHLD_MASK
#define HAL_SRV_BRAKEHLD_READABLE    1
#define HAL_SRV_BRAKEHLD_WRITABLE    1
#define HAL_SRV_BRAKEHLD_SIGNED      0
#define HAL_SRV_BRAKEHLD_ADR         0xB0000068

// Register BRAKECNT
#define HAL_SRV_BRAKEEN_REGISTER    (HAL_SRVREG_MAP->BRAKECNT)
#define HAL_SRV_BRAKEEN_LSB         7
#define HAL_SRV_BRAKEEN_MSB         7
#define HAL_SRV_BRAKEEN_MASK        0x00000080
#define HAL_SRV_BRAKEEN_TYPE        Bool
#define HAL_SRV_BRAKEEN             HAL_SRV_BRAKEEN_MASK
#define HAL_SRV_BRAKEEN_READABLE    1
#define HAL_SRV_BRAKEEN_WRITABLE    1
#define HAL_SRV_BRAKEEN_SIGNED      0
#define HAL_SRV_BRAKEEN_ADR         0xB000006C

#define HAL_SRV_BRAKESW_REGISTER    (HAL_SRVREG_MAP->BRAKECNT)
#define HAL_SRV_BRAKESW_LSB         6
#define HAL_SRV_BRAKESW_MSB         6
#define HAL_SRV_BRAKESW_MASK        0x00000040
#define HAL_SRV_BRAKESW_TYPE        Bool
#define HAL_SRV_BRAKESW             HAL_SRV_BRAKESW_MASK
#define HAL_SRV_BRAKESW_READABLE    1
#define HAL_SRV_BRAKESW_WRITABLE    1
#define HAL_SRV_BRAKESW_SIGNED      0
#define HAL_SRV_BRAKESW_ADR         0xB000006C

#define HAL_SRV_TROSW_REGISTER    (HAL_SRVREG_MAP->BRAKECNT)
#define HAL_SRV_TROSW_LSB         5
#define HAL_SRV_TROSW_MSB         5
#define HAL_SRV_TROSW_MASK        0x00000020
#define HAL_SRV_TROSW_TYPE        Bool
#define HAL_SRV_TROSW             HAL_SRV_TROSW_MASK
#define HAL_SRV_TROSW_READABLE    1
#define HAL_SRV_TROSW_WRITABLE    1
#define HAL_SRV_TROSW_SIGNED      0
#define HAL_SRV_TROSW_ADR         0xB000006C

#define HAL_SRV_ANTIBRAK_REGISTER    (HAL_SRVREG_MAP->BRAKECNT)
#define HAL_SRV_ANTIBRAK_LSB         0
#define HAL_SRV_ANTIBRAK_MSB         4
#define HAL_SRV_ANTIBRAK_MASK        0x0000001F
#define HAL_SRV_ANTIBRAK_TYPE        UByte
#define HAL_SRV_ANTIBRAK             HAL_SRV_ANTIBRAK_MASK
#define HAL_SRV_ANTIBRAK_READABLE    1
#define HAL_SRV_ANTIBRAK_WRITABLE    1
#define HAL_SRV_ANTIBRAK_SIGNED      0
#define HAL_SRV_ANTIBRAK_ADR         0xB000006C

#if (CHIP_REV >= 0xC0)

// Register DLFOOSW

#define HAL_SRV_DLFOOSW1_REGISTER    (HAL_SRVREG_MAP->DLFOOSW)
#define HAL_SRV_DLFOOSW1_LSB         0
#define HAL_SRV_DLFOOSW1_MSB         0
#define HAL_SRV_DLFOOSW1_MASK        0x00000001
#define HAL_SRV_DLFOOSW1_TYPE        Bool
#define HAL_SRV_DLFOOSW1             HAL_SRV_DLFOOSW1_MASK
#define HAL_SRV_DLFOOSW1_READABLE    1
#define HAL_SRV_DLFOOSW1_WRITABLE    1
#define HAL_SRV_DLFOOSW1_SIGNED      0
#define HAL_SRV_DLFOOSW1_ADR         0xB0000070

#endif // #if (CHIP_REV >= 0xC0)

// Register STPGEN
#define HAL_SRV_PDMCHSEL_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_PDMCHSEL_LSB         14
#define HAL_SRV_PDMCHSEL_MSB         14
#define HAL_SRV_PDMCHSEL_MASK        0x00004000
#define HAL_SRV_PDMCHSEL_TYPE        Bool
#define HAL_SRV_PDMCHSEL             HAL_SRV_PDMCHSEL_MASK
#define HAL_SRV_PDMCHSEL_READABLE    1
#define HAL_SRV_PDMCHSEL_WRITABLE    1
#define HAL_SRV_PDMCHSEL_SIGNED      0
#define HAL_SRV_PDMCHSEL_ADR         0xB0000080

#define HAL_SRV_ALPHASW_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_ALPHASW_LSB         13
#define HAL_SRV_ALPHASW_MSB         13
#define HAL_SRV_ALPHASW_MASK        0x00002000
#define HAL_SRV_ALPHASW_TYPE        Bool
#define HAL_SRV_ALPHASW             HAL_SRV_ALPHASW_MASK
#define HAL_SRV_ALPHASW_READABLE    1
#define HAL_SRV_ALPHASW_WRITABLE    1
#define HAL_SRV_ALPHASW_SIGNED      0
#define HAL_SRV_ALPHASW_ADR         0xB0000080

#define HAL_SRV_ALPHADAT2_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_ALPHADAT2_LSB         12
#define HAL_SRV_ALPHADAT2_MSB         12
#define HAL_SRV_ALPHADAT2_MASK        0x00001000
#define HAL_SRV_ALPHADAT2_TYPE        Bool
#define HAL_SRV_ALPHADAT2             HAL_SRV_ALPHADAT2_MASK
#define HAL_SRV_ALPHADAT2_READABLE    1
#define HAL_SRV_ALPHADAT2_WRITABLE    1
#define HAL_SRV_ALPHADAT2_SIGNED      0
#define HAL_SRV_ALPHADAT2_ADR         0xB0000080

#define HAL_SRV_ALPHADAT1_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_ALPHADAT1_LSB         11
#define HAL_SRV_ALPHADAT1_MSB         11
#define HAL_SRV_ALPHADAT1_MASK        0x00000800
#define HAL_SRV_ALPHADAT1_TYPE        Bool
#define HAL_SRV_ALPHADAT1             HAL_SRV_ALPHADAT1_MASK
#define HAL_SRV_ALPHADAT1_READABLE    1
#define HAL_SRV_ALPHADAT1_WRITABLE    1
#define HAL_SRV_ALPHADAT1_SIGNED      0
#define HAL_SRV_ALPHADAT1_ADR         0xB0000080

#define HAL_SRV_COLLISW_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_COLLISW_LSB         10
#define HAL_SRV_COLLISW_MSB         10
#define HAL_SRV_COLLISW_MASK        0x00000400
#define HAL_SRV_COLLISW_TYPE        Bool
#define HAL_SRV_COLLISW             HAL_SRV_COLLISW_MASK
#define HAL_SRV_COLLISW_READABLE    1
#define HAL_SRV_COLLISW_WRITABLE    1
#define HAL_SRV_COLLISW_SIGNED      0
#define HAL_SRV_COLLISW_ADR         0xB0000080

#define HAL_SRV_COLLIDAT2_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_COLLIDAT2_LSB         9
#define HAL_SRV_COLLIDAT2_MSB         9
#define HAL_SRV_COLLIDAT2_MASK        0x00000200
#define HAL_SRV_COLLIDAT2_TYPE        Bool
#define HAL_SRV_COLLIDAT2             HAL_SRV_COLLIDAT2_MASK
#define HAL_SRV_COLLIDAT2_READABLE    1
#define HAL_SRV_COLLIDAT2_WRITABLE    1
#define HAL_SRV_COLLIDAT2_SIGNED      0
#define HAL_SRV_COLLIDAT2_ADR         0xB0000080

#define HAL_SRV_COLLIDAT1_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_COLLIDAT1_LSB         8
#define HAL_SRV_COLLIDAT1_MSB         8
#define HAL_SRV_COLLIDAT1_MASK        0x00000100
#define HAL_SRV_COLLIDAT1_TYPE        Bool
#define HAL_SRV_COLLIDAT1             HAL_SRV_COLLIDAT1_MASK
#define HAL_SRV_COLLIDAT1_READABLE    1
#define HAL_SRV_COLLIDAT1_WRITABLE    1
#define HAL_SRV_COLLIDAT1_SIGNED      0
#define HAL_SRV_COLLIDAT1_ADR         0xB0000080

#define HAL_SRV_BYPSINT_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_BYPSINT_LSB         5
#define HAL_SRV_BYPSINT_MSB         5
#define HAL_SRV_BYPSINT_MASK        0x00000020
#define HAL_SRV_BYPSINT_TYPE        Bool
#define HAL_SRV_BYPSINT             HAL_SRV_BYPSINT_MASK
#define HAL_SRV_BYPSINT_READABLE    1
#define HAL_SRV_BYPSINT_WRITABLE    1
#define HAL_SRV_BYPSINT_SIGNED      0
#define HAL_SRV_BYPSINT_ADR         0xB0000080

#define HAL_SRV_PSSEL_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_PSSEL_LSB         4
#define HAL_SRV_PSSEL_MSB         4
#define HAL_SRV_PSSEL_MASK        0x00000010
#define HAL_SRV_PSSEL_TYPE        Bool
#define HAL_SRV_PSSEL             HAL_SRV_PSSEL_MASK
#define HAL_SRV_PSSEL_READABLE    1
#define HAL_SRV_PSSEL_WRITABLE    1
#define HAL_SRV_PSSEL_SIGNED      0
#define HAL_SRV_PSSEL_ADR         0xB0000080

#define HAL_SRV_KSLO_REGISTER    (HAL_SRVREG_MAP->STPGEN)
#define HAL_SRV_KSLO_LSB         0
#define HAL_SRV_KSLO_MSB         2
#define HAL_SRV_KSLO_MASK        0x00000007
#define HAL_SRV_KSLO_TYPE        halSRV_IntegratorSamplingFreq_t
#define HAL_SRV_KSLO             HAL_SRV_KSLO_MASK
#define HAL_SRV_KSLO_READABLE    1
#define HAL_SRV_KSLO_WRITABLE    1
#define HAL_SRV_KSLO_SIGNED      0
#define HAL_SRV_KSLO_ADR         0xB0000080

// Register STPSIZE
#define HAL_SRV_STPSIZE_REGISTER    (HAL_SRVREG_MAP->STPSIZE)
#define HAL_SRV_STPSIZE_LSB         0
#define HAL_SRV_STPSIZE_MSB         3
#define HAL_SRV_STPSIZE_MASK        0x0000000F
#define HAL_SRV_STPSIZE_TYPE        UByte
#define HAL_SRV_STPSIZE             HAL_SRV_STPSIZE_MASK
#define HAL_SRV_STPSIZE_READABLE    1
#define HAL_SRV_STPSIZE_WRITABLE    1
#define HAL_SRV_STPSIZE_SIGNED      0
#define HAL_SRV_STPSIZE_ADR         0xB0000084

// Register STPDCO
#define HAL_SRV_STPDCOCK_REGISTER    (HAL_SRVREG_MAP->STPDCO)
#define HAL_SRV_STPDCOCK_LSB         0
#define HAL_SRV_STPDCOCK_MSB         2
#define HAL_SRV_STPDCOCK_MASK        0x00000007
#define HAL_SRV_STPDCOCK_TYPE        halSRV_DCOClock_t
#define HAL_SRV_STPDCOCK             HAL_SRV_STPDCOCK_MASK
#define HAL_SRV_STPDCOCK_READABLE    1
#define HAL_SRV_STPDCOCK_WRITABLE    1
#define HAL_SRV_STPDCOCK_SIGNED      0
#define HAL_SRV_STPDCOCK_ADR         0xB0000088

// Register STPADD

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_STPADD_REGISTER    (HAL_SRVREG_MAP->STPADD)
#define HAL_SRV_STPADD_LSB         0
#define HAL_SRV_STPADD_MSB         5
#define HAL_SRV_STPADD_MASK        0x0000003F
#define HAL_SRV_STPADD_TYPE        UByte
#define HAL_SRV_STPADD             HAL_SRV_STPADD_MASK
#define HAL_SRV_STPADD_READABLE    1
#define HAL_SRV_STPADD_WRITABLE    1
#define HAL_SRV_STPADD_SIGNED      0
#define HAL_SRV_STPADD_ADR         0xB000008C

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_STPADD_REGISTER    (HAL_SRVREG_MAP->STPADD)
#define HAL_SRV_STPADD_LSB         0
#define HAL_SRV_STPADD_MSB         6
#define HAL_SRV_STPADD_MASK        0x0000007F
#define HAL_SRV_STPADD_TYPE        UByte
#define HAL_SRV_STPADD             HAL_SRV_STPADD_MASK
#define HAL_SRV_STPADD_READABLE    1
#define HAL_SRV_STPADD_WRITABLE    1
#define HAL_SRV_STPADD_SIGNED      0
#define HAL_SRV_STPADD_ADR         0xB000008C

#endif // #if (CHIP_REV >= 0xC0)

// Register STPDAC

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_STPDAC2_REGISTER    (HAL_SRVREG_MAP->STPDAC)
#define HAL_SRV_STPDAC2_LSB         8
#define HAL_SRV_STPDAC2_MSB         15
#define HAL_SRV_STPDAC2_MASK        0x0000FF00
#define HAL_SRV_STPDAC2_TYPE        UByte
#define HAL_SRV_STPDAC2             HAL_SRV_STPDAC2_MASK
#define HAL_SRV_STPDAC2_READABLE    1
#define HAL_SRV_STPDAC2_WRITABLE    1
#define HAL_SRV_STPDAC2_SIGNED      0
#define HAL_SRV_STPDAC2_ADR         0xB0000090

#define HAL_SRV_STPDAC1_REGISTER    (HAL_SRVREG_MAP->STPDAC)
#define HAL_SRV_STPDAC1_LSB         0
#define HAL_SRV_STPDAC1_MSB         7
#define HAL_SRV_STPDAC1_MASK        0x000000FF
#define HAL_SRV_STPDAC1_TYPE        UByte
#define HAL_SRV_STPDAC1             HAL_SRV_STPDAC1_MASK
#define HAL_SRV_STPDAC1_READABLE    1
#define HAL_SRV_STPDAC1_WRITABLE    1
#define HAL_SRV_STPDAC1_SIGNED      0
#define HAL_SRV_STPDAC1_ADR         0xB0000090

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_STPDAC_REGISTER    (HAL_SRVREG_MAP->STPDAC)
#define HAL_SRV_STPDAC_LSB         0
#define HAL_SRV_STPDAC_MSB         9
#define HAL_SRV_STPDAC_MASK        0x000003FF
#define HAL_SRV_STPDAC_TYPE        UInt16
#define HAL_SRV_STPDAC             HAL_SRV_STPDAC_MASK
#define HAL_SRV_STPDAC_READABLE    1
#define HAL_SRV_STPDAC_WRITABLE    1
#define HAL_SRV_STPDAC_SIGNED      0
#define HAL_SRV_STPDAC_ADR         0xB0000090

#endif // #if (CHIP_REV >= 0xC0)

// Register STPPOS

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_POSCNT_REGISTER    (HAL_SRVREG_MAP->STPPOS)
#define HAL_SRV_POSCNT_LSB         0
#define HAL_SRV_POSCNT_MSB         5
#define HAL_SRV_POSCNT_MASK        0x0000003F
#define HAL_SRV_POSCNT_TYPE        UByte
#define HAL_SRV_POSCNT             HAL_SRV_POSCNT_MASK
#define HAL_SRV_POSCNT_READABLE    1
#define HAL_SRV_POSCNT_WRITABLE    0
#define HAL_SRV_POSCNT_SIGNED      0
#define HAL_SRV_POSCNT_ADR         0xB0000098

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_STPLOC_REGISTER    (HAL_SRVREG_MAP->STPPOS)
#define HAL_SRV_STPLOC_LSB         16
#define HAL_SRV_STPLOC_MSB         31
#define HAL_SRV_STPLOC_MASK        0xFFFF0000
#define HAL_SRV_STPLOC_TYPE        UInt16
#define HAL_SRV_STPLOC             HAL_SRV_STPLOC_MASK
#define HAL_SRV_STPLOC_READABLE    1
#define HAL_SRV_STPLOC_WRITABLE    0
#define HAL_SRV_STPLOC_SIGNED      0
#define HAL_SRV_STPLOC_ADR         0xB0000098

#define HAL_SRV_STPPOT_REGISTER    (HAL_SRVREG_MAP->STPPOS)
#define HAL_SRV_STPPOT_LSB         0
#define HAL_SRV_STPPOT_MSB         5
#define HAL_SRV_STPPOT_MASK        0x0000003F
#define HAL_SRV_STPPOT_TYPE        UByte
#define HAL_SRV_STPPOT             HAL_SRV_STPPOT_MASK
#define HAL_SRV_STPPOT_READABLE    1
#define HAL_SRV_STPPOT_WRITABLE    0
#define HAL_SRV_STPPOT_SIGNED      0
#define HAL_SRV_STPPOT_ADR         0xB0000098

#endif // #if (CHIP_REV >= 0xC0)

// Register STPCTRL

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_STPMODE_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_STPMODE_LSB         7
#define HAL_SRV_STPMODE_MSB         7
#define HAL_SRV_STPMODE_MASK        0x00000080
#define HAL_SRV_STPMODE_TYPE        Bool
#define HAL_SRV_STPMODE             HAL_SRV_STPMODE_MASK
#define HAL_SRV_STPMODE_READABLE    1
#define HAL_SRV_STPMODE_WRITABLE    1
#define HAL_SRV_STPMODE_SIGNED      0
#define HAL_SRV_STPMODE_ADR         0xB000009C

#define HAL_SRV_INTCLR_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_INTCLR_LSB         6
#define HAL_SRV_INTCLR_MSB         6
#define HAL_SRV_INTCLR_MASK        0x00000040
#define HAL_SRV_INTCLR_TYPE        Bool
#define HAL_SRV_INTCLR             HAL_SRV_INTCLR_MASK
#define HAL_SRV_INTCLR_READABLE    1
#define HAL_SRV_INTCLR_WRITABLE    1
#define HAL_SRV_INTCLR_SIGNED      0
#define HAL_SRV_INTCLR_ADR         0xB000009C

#define HAL_SRV_POSVAL_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_POSVAL_LSB         0
#define HAL_SRV_POSVAL_MSB         5
#define HAL_SRV_POSVAL_MASK        0x0000003F
#define HAL_SRV_POSVAL_TYPE        UByte
#define HAL_SRV_POSVAL             HAL_SRV_POSVAL_MASK
#define HAL_SRV_POSVAL_READABLE    1
#define HAL_SRV_POSVAL_WRITABLE    1
#define HAL_SRV_POSVAL_SIGNED      0
#define HAL_SRV_POSVAL_ADR         0xB000009C

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_ENSTUNTK_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_ENSTUNTK_LSB         3
#define HAL_SRV_ENSTUNTK_MSB         3
#define HAL_SRV_ENSTUNTK_MASK        0x00000008
#define HAL_SRV_ENSTUNTK_TYPE        Bool
#define HAL_SRV_ENSTUNTK             HAL_SRV_ENSTUNTK_MASK
#define HAL_SRV_ENSTUNTK_READABLE    1
#define HAL_SRV_ENSTUNTK_WRITABLE    1
#define HAL_SRV_ENSTUNTK_SIGNED      0
#define HAL_SRV_ENSTUNTK_ADR         0xB000009C

#define HAL_SRV_AUTORELD_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_AUTORELD_LSB         2
#define HAL_SRV_AUTORELD_MSB         2
#define HAL_SRV_AUTORELD_MASK        0x00000004
#define HAL_SRV_AUTORELD_TYPE        Bool
#define HAL_SRV_AUTORELD             HAL_SRV_AUTORELD_MASK
#define HAL_SRV_AUTORELD_READABLE    1
#define HAL_SRV_AUTORELD_WRITABLE    1
#define HAL_SRV_AUTORELD_SIGNED      0
#define HAL_SRV_AUTORELD_ADR         0xB000009C

#define HAL_SRV_INTCLR_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_INTCLR_LSB         1
#define HAL_SRV_INTCLR_MSB         1
#define HAL_SRV_INTCLR_MASK        0x00000002
#define HAL_SRV_INTCLR_TYPE        Bool
#define HAL_SRV_INTCLR             HAL_SRV_INTCLR_MASK
#define HAL_SRV_INTCLR_READABLE    1
#define HAL_SRV_INTCLR_WRITABLE    1
#define HAL_SRV_INTCLR_SIGNED      0
#define HAL_SRV_INTCLR_ADR         0xB000009C

#define HAL_SRV_STPMODE_REGISTER    (HAL_SRVREG_MAP->STPCTRL)
#define HAL_SRV_STPMODE_LSB         0
#define HAL_SRV_STPMODE_MSB         0
#define HAL_SRV_STPMODE_MASK        0x00000001
#define HAL_SRV_STPMODE_TYPE        Bool
#define HAL_SRV_STPMODE             HAL_SRV_STPMODE_MASK
#define HAL_SRV_STPMODE_READABLE    1
#define HAL_SRV_STPMODE_WRITABLE    1
#define HAL_SRV_STPMODE_SIGNED      0
#define HAL_SRV_STPMODE_ADR         0xB000009C

#endif // #if (CHIP_REV >= 0xC0)

// Register STPPWR
#define HAL_SRV_PSTHLD_REGISTER    (HAL_SRVREG_MAP->STPPWR)
#define HAL_SRV_PSTHLD_LSB         0
#define HAL_SRV_PSTHLD_MSB         7
#define HAL_SRV_PSTHLD_MASK        0x000000FF
#define HAL_SRV_PSTHLD_TYPE        UByte
#define HAL_SRV_PSTHLD             HAL_SRV_PSTHLD_MASK
#define HAL_SRV_PSTHLD_READABLE    1
#define HAL_SRV_PSTHLD_WRITABLE    1
#define HAL_SRV_PSTHLD_SIGNED      0
#define HAL_SRV_PSTHLD_ADR         0xB00000A0

#if (CHIP_REV <= 0xB3)

// Register STPLOC
#define HAL_SRV_STPLOC_REGISTER    (HAL_SRVREG_MAP->STPLOC)
#define HAL_SRV_STPLOC_LSB         0
#define HAL_SRV_STPLOC_MSB         15
#define HAL_SRV_STPLOC_MASK        0x0000FFFF
#define HAL_SRV_STPLOC_TYPE        SInt16
#define HAL_SRV_STPLOC             HAL_SRV_STPLOC_MASK
#define HAL_SRV_STPLOC_READABLE    1
#define HAL_SRV_STPLOC_WRITABLE    1
#define HAL_SRV_STPLOC_SIGNED      1
#define HAL_SRV_STPLOC_ADR         0xB00000A8

#endif // #if (CHIP_REV <= 0xB3)

// Register HYSLTHLD
#define HAL_SRV_HYSLTHLD_REGISTER    (HAL_SRVREG_MAP->HYSLTHLD)
#define HAL_SRV_HYSLTHLD_LSB         0
#define HAL_SRV_HYSLTHLD_MSB         7
#define HAL_SRV_HYSLTHLD_MASK        0x000000FF
#define HAL_SRV_HYSLTHLD_TYPE        UByte
#define HAL_SRV_HYSLTHLD             HAL_SRV_HYSLTHLD_MASK
#define HAL_SRV_HYSLTHLD_READABLE    1
#define HAL_SRV_HYSLTHLD_WRITABLE    1
#define HAL_SRV_HYSLTHLD_SIGNED      0
#define HAL_SRV_HYSLTHLD_ADR         0xB00000B0

// Register HYSHTHLD
#define HAL_SRV_HYSHTHLD_REGISTER    (HAL_SRVREG_MAP->HYSHTHLD)
#define HAL_SRV_HYSHTHLD_LSB         0
#define HAL_SRV_HYSHTHLD_MSB         7
#define HAL_SRV_HYSHTHLD_MASK        0x000000FF
#define HAL_SRV_HYSHTHLD_TYPE        UByte
#define HAL_SRV_HYSHTHLD             HAL_SRV_HYSHTHLD_MASK
#define HAL_SRV_HYSHTHLD_READABLE    1
#define HAL_SRV_HYSHTHLD_WRITABLE    1
#define HAL_SRV_HYSHTHLD_SIGNED      0
#define HAL_SRV_HYSHTHLD_ADR         0xB00000B4

// Register FOODAT
#define HAL_SRV_FOODAT_REGISTER    (HAL_SRVREG_MAP->FOODAT)
#define HAL_SRV_FOODAT_LSB         0
#define HAL_SRV_FOODAT_MSB         9
#define HAL_SRV_FOODAT_MASK        0x000003FF
#define HAL_SRV_FOODAT_TYPE        UInt16
#define HAL_SRV_FOODAT             HAL_SRV_FOODAT_MASK
#define HAL_SRV_FOODAT_READABLE    1
#define HAL_SRV_FOODAT_WRITABLE    1
#define HAL_SRV_FOODAT_SIGNED      0
#define HAL_SRV_FOODAT_ADR         0xB0000100

#if (CHIP_REV >= 0xC0)

// Register DROPCTL
#define HAL_SRV_FCSSBYEN_REGISTER    (HAL_SRVREG_MAP->DROPCTL)
#define HAL_SRV_FCSSBYEN_LSB         13
#define HAL_SRV_FCSSBYEN_MSB         13
#define HAL_SRV_FCSSBYEN_MASK        0x00002000
#define HAL_SRV_FCSSBYEN_TYPE        Bool
#define HAL_SRV_FCSSBYEN             HAL_SRV_FCSSBYEN_MASK
#define HAL_SRV_FCSSBYEN_READABLE    1
#define HAL_SRV_FCSSBYEN_WRITABLE    1
#define HAL_SRV_FCSSBYEN_SIGNED      0
#define HAL_SRV_FCSSBYEN_ADR         0xB0000104

#define HAL_SRV_DROPPROT_REGISTER    (HAL_SRVREG_MAP->DROPCTL)
#define HAL_SRV_DROPPROT_LSB         12
#define HAL_SRV_DROPPROT_MSB         12
#define HAL_SRV_DROPPROT_MASK        0x00001000
#define HAL_SRV_DROPPROT_TYPE        Bool
#define HAL_SRV_DROPPROT             HAL_SRV_DROPPROT_MASK
#define HAL_SRV_DROPPROT_READABLE    1
#define HAL_SRV_DROPPROT_WRITABLE    1
#define HAL_SRV_DROPPROT_SIGNED      0
#define HAL_SRV_DROPPROT_ADR         0xB0000104

#define HAL_SRV_DROPFOO_REGISTER    (HAL_SRVREG_MAP->DROPCTL)
#define HAL_SRV_DROPFOO_LSB         0
#define HAL_SRV_DROPFOO_MSB         9
#define HAL_SRV_DROPFOO_MASK        0x000003FF
#define HAL_SRV_DROPFOO_TYPE        SInt16
#define HAL_SRV_DROPFOO             HAL_SRV_DROPFOO_MASK
#define HAL_SRV_DROPFOO_READABLE    1
#define HAL_SRV_DROPFOO_WRITABLE    1
#define HAL_SRV_DROPFOO_SIGNED      1
#define HAL_SRV_DROPFOO_ADR         0xB0000104

#endif // #if (CHIP_REV >= 0xC0)

// Register TRODAT
#define HAL_SRV_TRODAT_REGISTER    (HAL_SRVREG_MAP->TRODAT)
#define HAL_SRV_TRODAT_LSB         0
#define HAL_SRV_TRODAT_MSB         9
#define HAL_SRV_TRODAT_MASK        0x000003FF
#define HAL_SRV_TRODAT_TYPE        UInt16
#define HAL_SRV_TRODAT             HAL_SRV_TRODAT_MASK
#define HAL_SRV_TRODAT_READABLE    1
#define HAL_SRV_TRODAT_WRITABLE    1
#define HAL_SRV_TRODAT_SIGNED      0
#define HAL_SRV_TRODAT_ADR         0xB0000108

#if (CHIP_REV >= 0xC0)

// Register DROPSBY
#define HAL_SRV_FCS_REGISTER    (HAL_SRVREG_MAP->DROPSBY)
#define HAL_SRV_FCS_LSB         16
#define HAL_SRV_FCS_MSB         31
#define HAL_SRV_FCS_MASK        0xFFFF0000
#define HAL_SRV_FCS_TYPE        SInt16
#define HAL_SRV_FCS             HAL_SRV_FCS_MASK
#define HAL_SRV_FCS_READABLE    1
#define HAL_SRV_FCS_WRITABLE    0
#define HAL_SRV_FCS_SIGNED      1
#define HAL_SRV_FCS_ADR         0xB000010C

#define HAL_SRV_FCSSBY_REGISTER    (HAL_SRVREG_MAP->DROPSBY)
#define HAL_SRV_FCSSBY_LSB         0
#define HAL_SRV_FCSSBY_MSB         15
#define HAL_SRV_FCSSBY_MASK        0x0000FFFF
#define HAL_SRV_FCSSBY_TYPE        SInt16
#define HAL_SRV_FCSSBY             HAL_SRV_FCSSBY_MASK
#define HAL_SRV_FCSSBY_READABLE    1
#define HAL_SRV_FCSSBY_WRITABLE    1
#define HAL_SRV_FCSSBY_SIGNED      1
#define HAL_SRV_FCSSBY_ADR         0xB000010C

#endif // #if (CHIP_REV >= 0xC0)

// Register TILTDAT
#define HAL_SRV_TILTDAT_REGISTER    (HAL_SRVREG_MAP->TILTDAT)
#define HAL_SRV_TILTDAT_LSB         0
#define HAL_SRV_TILTDAT_MSB         9
#define HAL_SRV_TILTDAT_MASK        0x000003FF
#define HAL_SRV_TILTDAT_TYPE        UInt16
#define HAL_SRV_TILTDAT             HAL_SRV_TILTDAT_MASK
#define HAL_SRV_TILTDAT_READABLE    1
#define HAL_SRV_TILTDAT_WRITABLE    1
#define HAL_SRV_TILTDAT_SIGNED      0
#define HAL_SRV_TILTDAT_ADR         0xB0000110

// Register DACSW
#define HAL_SRV_MONSW_REGISTER    (HAL_SRVREG_MAP->DACSW)
#define HAL_SRV_MONSW_LSB         5
#define HAL_SRV_MONSW_MSB         7
#define HAL_SRV_MONSW_MASK        0x000000E0
#define HAL_SRV_MONSW_TYPE        halSRV_MonitorSignalsOnTiltDAC_t
#define HAL_SRV_MONSW             HAL_SRV_MONSW_MASK
#define HAL_SRV_MONSW_READABLE    1
#define HAL_SRV_MONSW_WRITABLE    1
#define HAL_SRV_MONSW_SIGNED      0
#define HAL_SRV_MONSW_ADR         0xB0000118

#define HAL_SRV_FOODIFEN_REGISTER    (HAL_SRVREG_MAP->DACSW)
#define HAL_SRV_FOODIFEN_LSB         4
#define HAL_SRV_FOODIFEN_MSB         4
#define HAL_SRV_FOODIFEN_MASK        0x00000010
#define HAL_SRV_FOODIFEN_TYPE        Bool
#define HAL_SRV_FOODIFEN             HAL_SRV_FOODIFEN_MASK
#define HAL_SRV_FOODIFEN_READABLE    1
#define HAL_SRV_FOODIFEN_WRITABLE    1
#define HAL_SRV_FOODIFEN_SIGNED      0
#define HAL_SRV_FOODIFEN_ADR         0xB0000118

#define HAL_SRV_TILTSW_REGISTER    (HAL_SRVREG_MAP->DACSW)
#define HAL_SRV_TILTSW_LSB         3
#define HAL_SRV_TILTSW_MSB         3
#define HAL_SRV_TILTSW_MASK        0x00000008
#define HAL_SRV_TILTSW_TYPE        Bool
#define HAL_SRV_TILTSW             HAL_SRV_TILTSW_MASK
#define HAL_SRV_TILTSW_READABLE    1
#define HAL_SRV_TILTSW_WRITABLE    1
#define HAL_SRV_TILTSW_SIGNED      0
#define HAL_SRV_TILTSW_ADR         0xB0000118

#define HAL_SRV_TRODACSW_REGISTER    (HAL_SRVREG_MAP->DACSW)
#define HAL_SRV_TRODACSW_LSB         1
#define HAL_SRV_TRODACSW_MSB         1
#define HAL_SRV_TRODACSW_MASK        0x00000002
#define HAL_SRV_TRODACSW_TYPE        Bool
#define HAL_SRV_TRODACSW             HAL_SRV_TRODACSW_MASK
#define HAL_SRV_TRODACSW_READABLE    1
#define HAL_SRV_TRODACSW_WRITABLE    1
#define HAL_SRV_TRODACSW_SIGNED      0
#define HAL_SRV_TRODACSW_ADR         0xB0000118

#define HAL_SRV_FOODACSW_REGISTER    (HAL_SRVREG_MAP->DACSW)
#define HAL_SRV_FOODACSW_LSB         0
#define HAL_SRV_FOODACSW_MSB         0
#define HAL_SRV_FOODACSW_MASK        0x00000001
#define HAL_SRV_FOODACSW_TYPE        Bool
#define HAL_SRV_FOODACSW             HAL_SRV_FOODACSW_MASK
#define HAL_SRV_FOODACSW_READABLE    1
#define HAL_SRV_FOODACSW_WRITABLE    1
#define HAL_SRV_FOODACSW_SIGNED      0
#define HAL_SRV_FOODACSW_ADR         0xB0000118

// Register FETHR
#define HAL_SRV_FETHR_REGISTER    (HAL_SRVREG_MAP->FETHR)
#define HAL_SRV_FETHR_LSB         0
#define HAL_SRV_FETHR_MSB         6
#define HAL_SRV_FETHR_MASK        0x0000007F
#define HAL_SRV_FETHR_TYPE        UByte
#define HAL_SRV_FETHR             HAL_SRV_FETHR_MASK
#define HAL_SRV_FETHR_READABLE    1
#define HAL_SRV_FETHR_WRITABLE    1
#define HAL_SRV_FETHR_SIGNED      0
#define HAL_SRV_FETHR_ADR         0xB000011C

// Register FETHRCTL
#define HAL_SRV_FETHREN_REGISTER    (HAL_SRVREG_MAP->FETHRCTL)
#define HAL_SRV_FETHREN_LSB         0
#define HAL_SRV_FETHREN_MSB         0
#define HAL_SRV_FETHREN_MASK        0x00000001
#define HAL_SRV_FETHREN_TYPE        Bool
#define HAL_SRV_FETHREN             HAL_SRV_FETHREN_MASK
#define HAL_SRV_FETHREN_READABLE    1
#define HAL_SRV_FETHREN_WRITABLE    1
#define HAL_SRV_FETHREN_SIGNED      0
#define HAL_SRV_FETHREN_ADR         0xB0000120

// Register SBYLVL
#define HAL_SRV_SBYLVL_REGISTER    (HAL_SRVREG_MAP->SBYLVL)
#define HAL_SRV_SBYLVL_LSB         0
#define HAL_SRV_SBYLVL_MSB         6
#define HAL_SRV_SBYLVL_MASK        0x0000007F
#define HAL_SRV_SBYLVL_TYPE        UByte
#define HAL_SRV_SBYLVL             HAL_SRV_SBYLVL_MASK
#define HAL_SRV_SBYLVL_READABLE    1
#define HAL_SRV_SBYLVL_WRITABLE    1
#define HAL_SRV_SBYLVL_SIGNED      0
#define HAL_SRV_SBYLVL_ADR         0xB0000128

// Register AWMASK
#define HAL_SRV_TON_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_TON_LSB         7
#define HAL_SRV_TON_MSB         7
#define HAL_SRV_TON_MASK        0x00000080
#define HAL_SRV_TON_TYPE        Bool
#define HAL_SRV_TON             HAL_SRV_TON_MASK
#define HAL_SRV_TON_READABLE    1
#define HAL_SRV_TON_WRITABLE    0
#define HAL_SRV_TON_SIGNED      0
#define HAL_SRV_TON_ADR         0xB000012C

#define HAL_SRV_DPLOCK_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_DPLOCK_LSB         6
#define HAL_SRV_DPLOCK_MSB         6
#define HAL_SRV_DPLOCK_MASK        0x00000040
#define HAL_SRV_DPLOCK_TYPE        Bool
#define HAL_SRV_DPLOCK             HAL_SRV_DPLOCK_MASK
#define HAL_SRV_DPLOCK_READABLE    1
#define HAL_SRV_DPLOCK_WRITABLE    0
#define HAL_SRV_DPLOCK_SIGNED      0
#define HAL_SRV_DPLOCK_ADR         0xB000012C

#define HAL_SRV_FGLOCK1_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_FGLOCK1_LSB         5
#define HAL_SRV_FGLOCK1_MSB         5
#define HAL_SRV_FGLOCK1_MASK        0x00000020
#define HAL_SRV_FGLOCK1_TYPE        Bool
#define HAL_SRV_FGLOCK1             HAL_SRV_FGLOCK1_MASK
#define HAL_SRV_FGLOCK1_READABLE    1
#define HAL_SRV_FGLOCK1_WRITABLE    0
#define HAL_SRV_FGLOCK1_SIGNED      0
#define HAL_SRV_FGLOCK1_ADR         0xB000012C

#define HAL_SRV_WBLLCK_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_WBLLCK_LSB         4
#define HAL_SRV_WBLLCK_MSB         4
#define HAL_SRV_WBLLCK_MASK        0x00000010
#define HAL_SRV_WBLLCK_TYPE        Bool
#define HAL_SRV_WBLLCK             HAL_SRV_WBLLCK_MASK
#define HAL_SRV_WBLLCK_READABLE    1
#define HAL_SRV_WBLLCK_WRITABLE    0
#define HAL_SRV_WBLLCK_SIGNED      0
#define HAL_SRV_WBLLCK_ADR         0xB000012C

#define HAL_SRV_TONIMSK_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_TONIMSK_LSB         3
#define HAL_SRV_TONIMSK_MSB         3
#define HAL_SRV_TONIMSK_MASK        0x00000008
#define HAL_SRV_TONIMSK_TYPE        Bool
#define HAL_SRV_TONIMSK             HAL_SRV_TONIMSK_MASK
#define HAL_SRV_TONIMSK_READABLE    1
#define HAL_SRV_TONIMSK_WRITABLE    1
#define HAL_SRV_TONIMSK_SIGNED      0
#define HAL_SRV_TONIMSK_ADR         0xB000012C

#define HAL_SRV_DPLIMSK_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_DPLIMSK_LSB         2
#define HAL_SRV_DPLIMSK_MSB         2
#define HAL_SRV_DPLIMSK_MASK        0x00000004
#define HAL_SRV_DPLIMSK_TYPE        Bool
#define HAL_SRV_DPLIMSK             HAL_SRV_DPLIMSK_MASK
#define HAL_SRV_DPLIMSK_READABLE    1
#define HAL_SRV_DPLIMSK_WRITABLE    1
#define HAL_SRV_DPLIMSK_SIGNED      0
#define HAL_SRV_DPLIMSK_ADR         0xB000012C

#define HAL_SRV_FGIMSK_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_FGIMSK_LSB         1
#define HAL_SRV_FGIMSK_MSB         1
#define HAL_SRV_FGIMSK_MASK        0x00000002
#define HAL_SRV_FGIMSK_TYPE        Bool
#define HAL_SRV_FGIMSK             HAL_SRV_FGIMSK_MASK
#define HAL_SRV_FGIMSK_READABLE    1
#define HAL_SRV_FGIMSK_WRITABLE    1
#define HAL_SRV_FGIMSK_SIGNED      0
#define HAL_SRV_FGIMSK_ADR         0xB000012C

#define HAL_SRV_WBLIMSK_REGISTER    (HAL_SRVREG_MAP->AWMASK)
#define HAL_SRV_WBLIMSK_LSB         0
#define HAL_SRV_WBLIMSK_MSB         0
#define HAL_SRV_WBLIMSK_MASK        0x00000001
#define HAL_SRV_WBLIMSK_TYPE        Bool
#define HAL_SRV_WBLIMSK             HAL_SRV_WBLIMSK_MASK
#define HAL_SRV_WBLIMSK_READABLE    1
#define HAL_SRV_WBLIMSK_WRITABLE    1
#define HAL_SRV_WBLIMSK_SIGNED      0
#define HAL_SRV_WBLIMSK_ADR         0xB000012C

// Register AFCRNG1
#define HAL_SRV_AFCRNG1_REGISTER    (HAL_SRVREG_MAP->AFCRNG1)
#define HAL_SRV_AFCRNG1_LSB         0
#define HAL_SRV_AFCRNG1_MSB         6
#define HAL_SRV_AFCRNG1_MASK        0x0000007F
#define HAL_SRV_AFCRNG1_TYPE        UByte
#define HAL_SRV_AFCRNG1             HAL_SRV_AFCRNG1_MASK
#define HAL_SRV_AFCRNG1_READABLE    1
#define HAL_SRV_AFCRNG1_WRITABLE    1
#define HAL_SRV_AFCRNG1_SIGNED      0
#define HAL_SRV_AFCRNG1_ADR         0xB0000138

// Register AFCRNG2
#define HAL_SRV_AFCRNG2_REGISTER    (HAL_SRVREG_MAP->AFCRNG2)
#define HAL_SRV_AFCRNG2_LSB         0
#define HAL_SRV_AFCRNG2_MSB         6
#define HAL_SRV_AFCRNG2_MASK        0x0000007F
#define HAL_SRV_AFCRNG2_TYPE        UByte
#define HAL_SRV_AFCRNG2             HAL_SRV_AFCRNG2_MASK
#define HAL_SRV_AFCRNG2_READABLE    1
#define HAL_SRV_AFCRNG2_WRITABLE    1
#define HAL_SRV_AFCRNG2_SIGNED      0
#define HAL_SRV_AFCRNG2_ADR         0xB000013C

// Register SVMODE0
#define HAL_SRV_DPLEN_REGISTER    (HAL_SRVREG_MAP->SVMODE0)
#define HAL_SRV_DPLEN_LSB         8
#define HAL_SRV_DPLEN_MSB         8
#define HAL_SRV_DPLEN_MASK        0x00000100
#define HAL_SRV_DPLEN_TYPE        Bool
#define HAL_SRV_DPLEN             HAL_SRV_DPLEN_MASK
#define HAL_SRV_DPLEN_READABLE    1
#define HAL_SRV_DPLEN_WRITABLE    1
#define HAL_SRV_DPLEN_SIGNED      0
#define HAL_SRV_DPLEN_ADR         0xB0000140

#define HAL_SRV_REGTON_REGISTER    (HAL_SRVREG_MAP->SVMODE0)
#define HAL_SRV_REGTON_LSB         7
#define HAL_SRV_REGTON_MSB         7
#define HAL_SRV_REGTON_MASK        0x00000080
#define HAL_SRV_REGTON_TYPE        Bool
#define HAL_SRV_REGTON             HAL_SRV_REGTON_MASK
#define HAL_SRV_REGTON_READABLE    1
#define HAL_SRV_REGTON_WRITABLE    1
#define HAL_SRV_REGTON_SIGNED      0
#define HAL_SRV_REGTON_ADR         0xB0000140

#define HAL_SRV_DPLMSK_REGISTER    (HAL_SRVREG_MAP->SVMODE0)
#define HAL_SRV_DPLMSK_LSB         6
#define HAL_SRV_DPLMSK_MSB         6
#define HAL_SRV_DPLMSK_MASK        0x00000040
#define HAL_SRV_DPLMSK_TYPE        Bool
#define HAL_SRV_DPLMSK             HAL_SRV_DPLMSK_MASK
#define HAL_SRV_DPLMSK_READABLE    1
#define HAL_SRV_DPLMSK_WRITABLE    1
#define HAL_SRV_DPLMSK_SIGNED      0
#define HAL_SRV_DPLMSK_ADR         0xB0000140

#define HAL_SRV_DECCHG_REGISTER    (HAL_SRVREG_MAP->SVMODE0)
#define HAL_SRV_DECCHG_LSB         3
#define HAL_SRV_DECCHG_MSB         5
#define HAL_SRV_DECCHG_MASK        0x00000038
#define HAL_SRV_DECCHG_TYPE        halSRV_DecChgDelay_t
#define HAL_SRV_DECCHG             HAL_SRV_DECCHG_MASK
#define HAL_SRV_DECCHG_READABLE    1
#define HAL_SRV_DECCHG_WRITABLE    1
#define HAL_SRV_DECCHG_SIGNED      0
#define HAL_SRV_DECCHG_ADR         0xB0000140

#define HAL_SRV_SRVMOD_REGISTER    (HAL_SRVREG_MAP->SVMODE0)
#define HAL_SRV_SRVMOD_LSB         0
#define HAL_SRV_SRVMOD_MSB         2
#define HAL_SRV_SRVMOD_MASK        0x00000007
#define HAL_SRV_SRVMOD_TYPE        halSRV_SRVMOD_t
#define HAL_SRV_SRVMOD             HAL_SRV_SRVMOD_MASK
#define HAL_SRV_SRVMOD_READABLE    1
#define HAL_SRV_SRVMOD_WRITABLE    1
#define HAL_SRV_SRVMOD_SIGNED      0
#define HAL_SRV_SRVMOD_ADR         0xB0000140

// Register SVMODE1
#define HAL_SRV_CURMOD_REGISTER    (HAL_SRVREG_MAP->SVMODE1)
#define HAL_SRV_CURMOD_LSB         0
#define HAL_SRV_CURMOD_MSB         1
#define HAL_SRV_CURMOD_MASK        0x00000003
#define HAL_SRV_CURMOD_TYPE        halSRV_CurMod_t
#define HAL_SRV_CURMOD             HAL_SRV_CURMOD_MASK
#define HAL_SRV_CURMOD_READABLE    1
#define HAL_SRV_CURMOD_WRITABLE    0
#define HAL_SRV_CURMOD_SIGNED      0
#define HAL_SRV_CURMOD_ADR         0xB0000144

// Register FGCMP
#define HAL_SRV_FGCMP_REGISTER    (HAL_SRVREG_MAP->FGCMP)
#define HAL_SRV_FGCMP_LSB         0
#define HAL_SRV_FGCMP_MSB         15
#define HAL_SRV_FGCMP_MASK        0x0000FFFF
#define HAL_SRV_FGCMP_TYPE        UInt16
#define HAL_SRV_FGCMP             HAL_SRV_FGCMP_MASK
#define HAL_SRV_FGCMP_READABLE    1
#define HAL_SRV_FGCMP_WRITABLE    1
#define HAL_SRV_FGCMP_SIGNED      0
#define HAL_SRV_FGCMP_ADR         0xB0000148

// Register WBLCMP
#define HAL_SRV_WBLCMP_REGISTER    (HAL_SRVREG_MAP->WBLCMP)
#define HAL_SRV_WBLCMP_LSB         0
#define HAL_SRV_WBLCMP_MSB         15
#define HAL_SRV_WBLCMP_MASK        0x0000FFFF
#define HAL_SRV_WBLCMP_TYPE        UInt16
#define HAL_SRV_WBLCMP             HAL_SRV_WBLCMP_MASK
#define HAL_SRV_WBLCMP_READABLE    1
#define HAL_SRV_WBLCMP_WRITABLE    1
#define HAL_SRV_WBLCMP_SIGNED      0
#define HAL_SRV_WBLCMP_ADR         0xB0000150

// Register DECCMP
#define HAL_SRV_DECCMP_REGISTER    (HAL_SRVREG_MAP->DECCMP)
#define HAL_SRV_DECCMP_LSB         0
#define HAL_SRV_DECCMP_MSB         15
#define HAL_SRV_DECCMP_MASK        0x0000FFFF
#define HAL_SRV_DECCMP_TYPE        UInt16
#define HAL_SRV_DECCMP             HAL_SRV_DECCMP_MASK
#define HAL_SRV_DECCMP_READABLE    1
#define HAL_SRV_DECCMP_WRITABLE    1
#define HAL_SRV_DECCMP_SIGNED      0
#define HAL_SRV_DECCMP_ADR         0xB0000158

// Register AKICLVL
#define HAL_SRV_AKICLVL_REGISTER    (HAL_SRVREG_MAP->AKICLVL)
#define HAL_SRV_AKICLVL_LSB         0
#define HAL_SRV_AKICLVL_MSB         7
#define HAL_SRV_AKICLVL_MASK        0x000000FF
#define HAL_SRV_AKICLVL_TYPE        SByte
#define HAL_SRV_AKICLVL             HAL_SRV_AKICLVL_MASK
#define HAL_SRV_AKICLVL_READABLE    1
#define HAL_SRV_AKICLVL_WRITABLE    1
#define HAL_SRV_AKICLVL_SIGNED      1
#define HAL_SRV_AKICLVL_ADR         0xB0000160

// Register WBLRNG
#define HAL_SRV_WBLRNG_REGISTER    (HAL_SRVREG_MAP->WBLRNG)
#define HAL_SRV_WBLRNG_LSB         0
#define HAL_SRV_WBLRNG_MSB         15
#define HAL_SRV_WBLRNG_MASK        0x0000FFFF
#define HAL_SRV_WBLRNG_TYPE        UInt16
#define HAL_SRV_WBLRNG             HAL_SRV_WBLRNG_MASK
#define HAL_SRV_WBLRNG_READABLE    1
#define HAL_SRV_WBLRNG_WRITABLE    1
#define HAL_SRV_WBLRNG_SIGNED      0
#define HAL_SRV_WBLRNG_ADR         0xB0000168

// Register WAITWBL
#define HAL_SRV_WAITWBL_REGISTER    (HAL_SRVREG_MAP->WAITWBL)
#define HAL_SRV_WAITWBL_LSB         0
#define HAL_SRV_WAITWBL_MSB         3
#define HAL_SRV_WAITWBL_MASK        0x0000000F
#define HAL_SRV_WAITWBL_TYPE        UByte
#define HAL_SRV_WAITWBL             HAL_SRV_WAITWBL_MASK
#define HAL_SRV_WAITWBL_READABLE    1
#define HAL_SRV_WAITWBL_WRITABLE    1
#define HAL_SRV_WAITWBL_SIGNED      0
#define HAL_SRV_WAITWBL_ADR         0xB0000170

// Register APCGAIN
#define HAL_SRV_AFCINRG_REGISTER    (HAL_SRVREG_MAP->APCGAIN)
#define HAL_SRV_AFCINRG_LSB         6
#define HAL_SRV_AFCINRG_MSB         6
#define HAL_SRV_AFCINRG_MASK        0x00000040
#define HAL_SRV_AFCINRG_TYPE        Bool
#define HAL_SRV_AFCINRG             HAL_SRV_AFCINRG_MASK
#define HAL_SRV_AFCINRG_READABLE    1
#define HAL_SRV_AFCINRG_WRITABLE    0
#define HAL_SRV_AFCINRG_SIGNED      0
#define HAL_SRV_AFCINRG_ADR         0xB0000174

#define HAL_SRV_AFCINRGEN_REGISTER    (HAL_SRVREG_MAP->APCGAIN)
#define HAL_SRV_AFCINRGEN_LSB         5
#define HAL_SRV_AFCINRGEN_MSB         5
#define HAL_SRV_AFCINRGEN_MASK        0x00000020
#define HAL_SRV_AFCINRGEN_TYPE        Bool
#define HAL_SRV_AFCINRGEN             HAL_SRV_AFCINRGEN_MASK
#define HAL_SRV_AFCINRGEN_READABLE    1
#define HAL_SRV_AFCINRGEN_WRITABLE    1
#define HAL_SRV_AFCINRGEN_SIGNED      0
#define HAL_SRV_AFCINRGEN_ADR         0xB0000174

#define HAL_SRV_APCEN_REGISTER    (HAL_SRVREG_MAP->APCGAIN)
#define HAL_SRV_APCEN_LSB         4
#define HAL_SRV_APCEN_MSB         4
#define HAL_SRV_APCEN_MASK        0x00000010
#define HAL_SRV_APCEN_TYPE        Bool
#define HAL_SRV_APCEN             HAL_SRV_APCEN_MASK
#define HAL_SRV_APCEN_READABLE    1
#define HAL_SRV_APCEN_WRITABLE    1
#define HAL_SRV_APCEN_SIGNED      0
#define HAL_SRV_APCEN_ADR         0xB0000174

#define HAL_SRV_APCGAIN_REGISTER    (HAL_SRVREG_MAP->APCGAIN)
#define HAL_SRV_APCGAIN_LSB         0
#define HAL_SRV_APCGAIN_MSB         3
#define HAL_SRV_APCGAIN_MASK        0x0000000F
#define HAL_SRV_APCGAIN_TYPE        halSRV_ApcGain_t
#define HAL_SRV_APCGAIN             HAL_SRV_APCGAIN_MASK
#define HAL_SRV_APCGAIN_READABLE    1
#define HAL_SRV_APCGAIN_WRITABLE    1
#define HAL_SRV_APCGAIN_SIGNED      0
#define HAL_SRV_APCGAIN_ADR         0xB0000174

// Register APCNUM
#define HAL_SRV_APCNUM_REGISTER    (HAL_SRVREG_MAP->APCNUM)
#define HAL_SRV_APCNUM_LSB         0
#define HAL_SRV_APCNUM_MSB         3
#define HAL_SRV_APCNUM_MASK        0x0000000F
#define HAL_SRV_APCNUM_TYPE        UByte
#define HAL_SRV_APCNUM             HAL_SRV_APCNUM_MASK
#define HAL_SRV_APCNUM_READABLE    1
#define HAL_SRV_APCNUM_WRITABLE    1
#define HAL_SRV_APCNUM_SIGNED      0
#define HAL_SRV_APCNUM_ADR         0xB0000178

// Register OSCFGMAX
#define HAL_SRV_OSCFGMAX_REGISTER    (HAL_SRVREG_MAP->OSCFGMAX)
#define HAL_SRV_OSCFGMAX_LSB         0
#define HAL_SRV_OSCFGMAX_MSB         7
#define HAL_SRV_OSCFGMAX_MASK        0x000000FF
#define HAL_SRV_OSCFGMAX_TYPE        UByte
#define HAL_SRV_OSCFGMAX             HAL_SRV_OSCFGMAX_MASK
#define HAL_SRV_OSCFGMAX_READABLE    1
#define HAL_SRV_OSCFGMAX_WRITABLE    1
#define HAL_SRV_OSCFGMAX_SIGNED      0
#define HAL_SRV_OSCFGMAX_ADR         0xB000017C

// Register TESODCTL
#define HAL_SRV_OSCTE_REGISTER    (HAL_SRVREG_MAP->TESODCTL)
#define HAL_SRV_OSCTE_LSB         7
#define HAL_SRV_OSCTE_MSB         7
#define HAL_SRV_OSCTE_MASK        0x00000080
#define HAL_SRV_OSCTE_TYPE        Bool
#define HAL_SRV_OSCTE             HAL_SRV_OSCTE_MASK
#define HAL_SRV_OSCTE_READABLE    1
#define HAL_SRV_OSCTE_WRITABLE    0
#define HAL_SRV_OSCTE_SIGNED      0
#define HAL_SRV_OSCTE_ADR         0xB0000180

#define HAL_SRV_TENOISEN_REGISTER    (HAL_SRVREG_MAP->TESODCTL)
#define HAL_SRV_TENOISEN_LSB         1
#define HAL_SRV_TENOISEN_MSB         2
#define HAL_SRV_TENOISEN_MASK        0x00000006
#define HAL_SRV_TENOISEN_TYPE        halSRV_TENOISEN_t
#define HAL_SRV_TENOISEN             HAL_SRV_TENOISEN_MASK
#define HAL_SRV_TENOISEN_READABLE    1
#define HAL_SRV_TENOISEN_WRITABLE    1
#define HAL_SRV_TENOISEN_SIGNED      0
#define HAL_SRV_TENOISEN_ADR         0xB0000180

#define HAL_SRV_TEOSCEN_REGISTER    (HAL_SRVREG_MAP->TESODCTL)
#define HAL_SRV_TEOSCEN_LSB         0
#define HAL_SRV_TEOSCEN_MSB         0
#define HAL_SRV_TEOSCEN_MASK        0x00000001
#define HAL_SRV_TEOSCEN_TYPE        Bool
#define HAL_SRV_TEOSCEN             HAL_SRV_TEOSCEN_MASK
#define HAL_SRV_TEOSCEN_READABLE    1
#define HAL_SRV_TEOSCEN_WRITABLE    1
#define HAL_SRV_TEOSCEN_SIGNED      0
#define HAL_SRV_TEOSCEN_ADR         0xB0000180

// Register TESOFMIN
#define HAL_SRV_TESOFMIN_REGISTER    (HAL_SRVREG_MAP->TESOFMIN)
#define HAL_SRV_TESOFMIN_LSB         0
#define HAL_SRV_TESOFMIN_MSB         7
#define HAL_SRV_TESOFMIN_MASK        0x000000FF
#define HAL_SRV_TESOFMIN_TYPE        UByte
#define HAL_SRV_TESOFMIN             HAL_SRV_TESOFMIN_MASK
#define HAL_SRV_TESOFMIN_READABLE    1
#define HAL_SRV_TESOFMIN_WRITABLE    1
#define HAL_SRV_TESOFMIN_SIGNED      0
#define HAL_SRV_TESOFMIN_ADR         0xB0000184

// Register TESOFMAX
#define HAL_SRV_TESOFMAX_REGISTER    (HAL_SRVREG_MAP->TESOFMAX)
#define HAL_SRV_TESOFMAX_LSB         0
#define HAL_SRV_TESOFMAX_MSB         7
#define HAL_SRV_TESOFMAX_MASK        0x000000FF
#define HAL_SRV_TESOFMAX_TYPE        UByte
#define HAL_SRV_TESOFMAX             HAL_SRV_TESOFMAX_MASK
#define HAL_SRV_TESOFMAX_READABLE    1
#define HAL_SRV_TESOFMAX_WRITABLE    1
#define HAL_SRV_TESOFMAX_SIGNED      0
#define HAL_SRV_TESOFMAX_ADR         0xB0000188

// Register TESOAM
#define HAL_SRV_TESOAM_REGISTER    (HAL_SRVREG_MAP->TESOAM)
#define HAL_SRV_TESOAM_LSB         0
#define HAL_SRV_TESOAM_MSB         7
#define HAL_SRV_TESOAM_MASK        0x000000FF
#define HAL_SRV_TESOAM_TYPE        SByte
#define HAL_SRV_TESOAM             HAL_SRV_TESOAM_MASK
#define HAL_SRV_TESOAM_READABLE    1
#define HAL_SRV_TESOAM_WRITABLE    1
#define HAL_SRV_TESOAM_SIGNED      1
#define HAL_SRV_TESOAM_ADR         0xB000018C

// Register TESOWD
#define HAL_SRV_TESOWD_REGISTER    (HAL_SRVREG_MAP->TESOWD)
#define HAL_SRV_TESOWD_LSB         0
#define HAL_SRV_TESOWD_MSB         7
#define HAL_SRV_TESOWD_MASK        0x000000FF
#define HAL_SRV_TESOWD_TYPE        UByte
#define HAL_SRV_TESOWD             HAL_SRV_TESOWD_MASK
#define HAL_SRV_TESOWD_READABLE    1
#define HAL_SRV_TESOWD_WRITABLE    1
#define HAL_SRV_TESOWD_SIGNED      0
#define HAL_SRV_TESOWD_ADR         0xB0000190

// Register TESOCNT
#define HAL_SRV_TESOCNT_REGISTER    (HAL_SRVREG_MAP->TESOCNT)
#define HAL_SRV_TESOCNT_LSB         0
#define HAL_SRV_TESOCNT_MSB         7
#define HAL_SRV_TESOCNT_MASK        0x000000FF
#define HAL_SRV_TESOCNT_TYPE        UByte
#define HAL_SRV_TESOCNT             HAL_SRV_TESOCNT_MASK
#define HAL_SRV_TESOCNT_READABLE    1
#define HAL_SRV_TESOCNT_WRITABLE    1
#define HAL_SRV_TESOCNT_SIGNED      0
#define HAL_SRV_TESOCNT_ADR         0xB0000194

// Register TESOCTHR
#define HAL_SRV_TESOCTHR_REGISTER    (HAL_SRVREG_MAP->TESOCTHR)
#define HAL_SRV_TESOCTHR_LSB         0
#define HAL_SRV_TESOCTHR_MSB         7
#define HAL_SRV_TESOCTHR_MASK        0x000000FF
#define HAL_SRV_TESOCTHR_TYPE        UByte
#define HAL_SRV_TESOCTHR             HAL_SRV_TESOCTHR_MASK
#define HAL_SRV_TESOCTHR_READABLE    1
#define HAL_SRV_TESOCTHR_WRITABLE    1
#define HAL_SRV_TESOCTHR_SIGNED      0
#define HAL_SRV_TESOCTHR_ADR         0xB0000198

// Register FE1SODCTL
#define HAL_SRV_OSCFE1_REGISTER    (HAL_SRVREG_MAP->FE1SODCTL)
#define HAL_SRV_OSCFE1_LSB         7
#define HAL_SRV_OSCFE1_MSB         7
#define HAL_SRV_OSCFE1_MASK        0x00000080
#define HAL_SRV_OSCFE1_TYPE        Bool
#define HAL_SRV_OSCFE1             HAL_SRV_OSCFE1_MASK
#define HAL_SRV_OSCFE1_READABLE    1
#define HAL_SRV_OSCFE1_WRITABLE    0
#define HAL_SRV_OSCFE1_SIGNED      0
#define HAL_SRV_OSCFE1_ADR         0xB00001A0

#define HAL_SRV_FE1NOISEN_REGISTER    (HAL_SRVREG_MAP->FE1SODCTL)
#define HAL_SRV_FE1NOISEN_LSB         1
#define HAL_SRV_FE1NOISEN_MSB         2
#define HAL_SRV_FE1NOISEN_MASK        0x00000006
#define HAL_SRV_FE1NOISEN_TYPE        halSRV_FE1NOISEN_t
#define HAL_SRV_FE1NOISEN             HAL_SRV_FE1NOISEN_MASK
#define HAL_SRV_FE1NOISEN_READABLE    1
#define HAL_SRV_FE1NOISEN_WRITABLE    1
#define HAL_SRV_FE1NOISEN_SIGNED      0
#define HAL_SRV_FE1NOISEN_ADR         0xB00001A0

#define HAL_SRV_FE1OSCEN_REGISTER    (HAL_SRVREG_MAP->FE1SODCTL)
#define HAL_SRV_FE1OSCEN_LSB         0
#define HAL_SRV_FE1OSCEN_MSB         0
#define HAL_SRV_FE1OSCEN_MASK        0x00000001
#define HAL_SRV_FE1OSCEN_TYPE        Bool
#define HAL_SRV_FE1OSCEN             HAL_SRV_FE1OSCEN_MASK
#define HAL_SRV_FE1OSCEN_READABLE    1
#define HAL_SRV_FE1OSCEN_WRITABLE    1
#define HAL_SRV_FE1OSCEN_SIGNED      0
#define HAL_SRV_FE1OSCEN_ADR         0xB00001A0

// Register FE1SOFMIN
#define HAL_SRV_FE1SOFMIN_REGISTER    (HAL_SRVREG_MAP->FE1SOFMIN)
#define HAL_SRV_FE1SOFMIN_LSB         0
#define HAL_SRV_FE1SOFMIN_MSB         7
#define HAL_SRV_FE1SOFMIN_MASK        0x000000FF
#define HAL_SRV_FE1SOFMIN_TYPE        UByte
#define HAL_SRV_FE1SOFMIN             HAL_SRV_FE1SOFMIN_MASK
#define HAL_SRV_FE1SOFMIN_READABLE    1
#define HAL_SRV_FE1SOFMIN_WRITABLE    1
#define HAL_SRV_FE1SOFMIN_SIGNED      0
#define HAL_SRV_FE1SOFMIN_ADR         0xB00001A4

// Register FE1SOFMAX
#define HAL_SRV_FE1SOFMAX_REGISTER    (HAL_SRVREG_MAP->FE1SOFMAX)
#define HAL_SRV_FE1SOFMAX_LSB         0
#define HAL_SRV_FE1SOFMAX_MSB         7
#define HAL_SRV_FE1SOFMAX_MASK        0x000000FF
#define HAL_SRV_FE1SOFMAX_TYPE        UByte
#define HAL_SRV_FE1SOFMAX             HAL_SRV_FE1SOFMAX_MASK
#define HAL_SRV_FE1SOFMAX_READABLE    1
#define HAL_SRV_FE1SOFMAX_WRITABLE    1
#define HAL_SRV_FE1SOFMAX_SIGNED      0
#define HAL_SRV_FE1SOFMAX_ADR         0xB00001A8

// Register FE1SOAM
#define HAL_SRV_FE1SOAM_REGISTER    (HAL_SRVREG_MAP->FE1SOAM)
#define HAL_SRV_FE1SOAM_LSB         0
#define HAL_SRV_FE1SOAM_MSB         7
#define HAL_SRV_FE1SOAM_MASK        0x000000FF
#define HAL_SRV_FE1SOAM_TYPE        SByte
#define HAL_SRV_FE1SOAM             HAL_SRV_FE1SOAM_MASK
#define HAL_SRV_FE1SOAM_READABLE    1
#define HAL_SRV_FE1SOAM_WRITABLE    1
#define HAL_SRV_FE1SOAM_SIGNED      1
#define HAL_SRV_FE1SOAM_ADR         0xB00001AC

// Register FE1SOWD
#define HAL_SRV_FE1SOWD_REGISTER    (HAL_SRVREG_MAP->FE1SOWD)
#define HAL_SRV_FE1SOWD_LSB         0
#define HAL_SRV_FE1SOWD_MSB         7
#define HAL_SRV_FE1SOWD_MASK        0x000000FF
#define HAL_SRV_FE1SOWD_TYPE        UByte
#define HAL_SRV_FE1SOWD             HAL_SRV_FE1SOWD_MASK
#define HAL_SRV_FE1SOWD_READABLE    1
#define HAL_SRV_FE1SOWD_WRITABLE    1
#define HAL_SRV_FE1SOWD_SIGNED      0
#define HAL_SRV_FE1SOWD_ADR         0xB00001B0

// Register FE1SOCNT
#define HAL_SRV_FE1SOCNT_REGISTER    (HAL_SRVREG_MAP->FE1SOCNT)
#define HAL_SRV_FE1SOCNT_LSB         0
#define HAL_SRV_FE1SOCNT_MSB         7
#define HAL_SRV_FE1SOCNT_MASK        0x000000FF
#define HAL_SRV_FE1SOCNT_TYPE        UByte
#define HAL_SRV_FE1SOCNT             HAL_SRV_FE1SOCNT_MASK
#define HAL_SRV_FE1SOCNT_READABLE    1
#define HAL_SRV_FE1SOCNT_WRITABLE    1
#define HAL_SRV_FE1SOCNT_SIGNED      0
#define HAL_SRV_FE1SOCNT_ADR         0xB00001B4

// Register FE1SOCTHR
#define HAL_SRV_FE1SOCTHR_REGISTER    (HAL_SRVREG_MAP->FE1SOCTHR)
#define HAL_SRV_FE1SOCTHR_LSB         0
#define HAL_SRV_FE1SOCTHR_MSB         7
#define HAL_SRV_FE1SOCTHR_MASK        0x000000FF
#define HAL_SRV_FE1SOCTHR_TYPE        UByte
#define HAL_SRV_FE1SOCTHR             HAL_SRV_FE1SOCTHR_MASK
#define HAL_SRV_FE1SOCTHR_READABLE    1
#define HAL_SRV_FE1SOCTHR_WRITABLE    1
#define HAL_SRV_FE1SOCTHR_SIGNED      0
#define HAL_SRV_FE1SOCTHR_ADR         0xB00001B8

// Register FE2SODCTL
#define HAL_SRV_OSCFE2_REGISTER    (HAL_SRVREG_MAP->FE2SODCTL)
#define HAL_SRV_OSCFE2_LSB         7
#define HAL_SRV_OSCFE2_MSB         7
#define HAL_SRV_OSCFE2_MASK        0x00000080
#define HAL_SRV_OSCFE2_TYPE        Bool
#define HAL_SRV_OSCFE2             HAL_SRV_OSCFE2_MASK
#define HAL_SRV_OSCFE2_READABLE    1
#define HAL_SRV_OSCFE2_WRITABLE    0
#define HAL_SRV_OSCFE2_SIGNED      0
#define HAL_SRV_OSCFE2_ADR         0xB00001C0

#define HAL_SRV_FE2NOISEN_REGISTER    (HAL_SRVREG_MAP->FE2SODCTL)
#define HAL_SRV_FE2NOISEN_LSB         1
#define HAL_SRV_FE2NOISEN_MSB         2
#define HAL_SRV_FE2NOISEN_MASK        0x00000006
#define HAL_SRV_FE2NOISEN_TYPE        halSRV_FE2NOISEN_t
#define HAL_SRV_FE2NOISEN             HAL_SRV_FE2NOISEN_MASK
#define HAL_SRV_FE2NOISEN_READABLE    1
#define HAL_SRV_FE2NOISEN_WRITABLE    1
#define HAL_SRV_FE2NOISEN_SIGNED      0
#define HAL_SRV_FE2NOISEN_ADR         0xB00001C0

#define HAL_SRV_FE2OSCEN_REGISTER    (HAL_SRVREG_MAP->FE2SODCTL)
#define HAL_SRV_FE2OSCEN_LSB         0
#define HAL_SRV_FE2OSCEN_MSB         0
#define HAL_SRV_FE2OSCEN_MASK        0x00000001
#define HAL_SRV_FE2OSCEN_TYPE        Bool
#define HAL_SRV_FE2OSCEN             HAL_SRV_FE2OSCEN_MASK
#define HAL_SRV_FE2OSCEN_READABLE    1
#define HAL_SRV_FE2OSCEN_WRITABLE    1
#define HAL_SRV_FE2OSCEN_SIGNED      0
#define HAL_SRV_FE2OSCEN_ADR         0xB00001C0

// Register FE2SOFMIN
#define HAL_SRV_FE2SOFMIN_REGISTER    (HAL_SRVREG_MAP->FE2SOFMIN)
#define HAL_SRV_FE2SOFMIN_LSB         0
#define HAL_SRV_FE2SOFMIN_MSB         7
#define HAL_SRV_FE2SOFMIN_MASK        0x000000FF
#define HAL_SRV_FE2SOFMIN_TYPE        UByte
#define HAL_SRV_FE2SOFMIN             HAL_SRV_FE2SOFMIN_MASK
#define HAL_SRV_FE2SOFMIN_READABLE    1
#define HAL_SRV_FE2SOFMIN_WRITABLE    1
#define HAL_SRV_FE2SOFMIN_SIGNED      0
#define HAL_SRV_FE2SOFMIN_ADR         0xB00001C4

// Register FE2SOFMAX
#define HAL_SRV_FE2SOFMAX_REGISTER    (HAL_SRVREG_MAP->FE2SOFMAX)
#define HAL_SRV_FE2SOFMAX_LSB         0
#define HAL_SRV_FE2SOFMAX_MSB         7
#define HAL_SRV_FE2SOFMAX_MASK        0x000000FF
#define HAL_SRV_FE2SOFMAX_TYPE        UByte
#define HAL_SRV_FE2SOFMAX             HAL_SRV_FE2SOFMAX_MASK
#define HAL_SRV_FE2SOFMAX_READABLE    1
#define HAL_SRV_FE2SOFMAX_WRITABLE    1
#define HAL_SRV_FE2SOFMAX_SIGNED      0
#define HAL_SRV_FE2SOFMAX_ADR         0xB00001C8

// Register FE2SOAM
#define HAL_SRV_FE2SOAM_REGISTER    (HAL_SRVREG_MAP->FE2SOAM)
#define HAL_SRV_FE2SOAM_LSB         0
#define HAL_SRV_FE2SOAM_MSB         7
#define HAL_SRV_FE2SOAM_MASK        0x000000FF
#define HAL_SRV_FE2SOAM_TYPE        SByte
#define HAL_SRV_FE2SOAM             HAL_SRV_FE2SOAM_MASK
#define HAL_SRV_FE2SOAM_READABLE    1
#define HAL_SRV_FE2SOAM_WRITABLE    1
#define HAL_SRV_FE2SOAM_SIGNED      1
#define HAL_SRV_FE2SOAM_ADR         0xB00001CC

// Register FE2SOWD
#define HAL_SRV_FE2SOWD_REGISTER    (HAL_SRVREG_MAP->FE2SOWD)
#define HAL_SRV_FE2SOWD_LSB         0
#define HAL_SRV_FE2SOWD_MSB         7
#define HAL_SRV_FE2SOWD_MASK        0x000000FF
#define HAL_SRV_FE2SOWD_TYPE        UByte
#define HAL_SRV_FE2SOWD             HAL_SRV_FE2SOWD_MASK
#define HAL_SRV_FE2SOWD_READABLE    1
#define HAL_SRV_FE2SOWD_WRITABLE    1
#define HAL_SRV_FE2SOWD_SIGNED      0
#define HAL_SRV_FE2SOWD_ADR         0xB00001D0

// Register FE2SOCNT
#define HAL_SRV_FE2SOCNT_REGISTER    (HAL_SRVREG_MAP->FE2SOCNT)
#define HAL_SRV_FE2SOCNT_LSB         0
#define HAL_SRV_FE2SOCNT_MSB         7
#define HAL_SRV_FE2SOCNT_MASK        0x000000FF
#define HAL_SRV_FE2SOCNT_TYPE        UByte
#define HAL_SRV_FE2SOCNT             HAL_SRV_FE2SOCNT_MASK
#define HAL_SRV_FE2SOCNT_READABLE    1
#define HAL_SRV_FE2SOCNT_WRITABLE    1
#define HAL_SRV_FE2SOCNT_SIGNED      0
#define HAL_SRV_FE2SOCNT_ADR         0xB00001D4

// Register FE2SOCTHR
#define HAL_SRV_FE2SOCTHR_REGISTER    (HAL_SRVREG_MAP->FE2SOCTHR)
#define HAL_SRV_FE2SOCTHR_LSB         0
#define HAL_SRV_FE2SOCTHR_MSB         7
#define HAL_SRV_FE2SOCTHR_MASK        0x000000FF
#define HAL_SRV_FE2SOCTHR_TYPE        UByte
#define HAL_SRV_FE2SOCTHR             HAL_SRV_FE2SOCTHR_MASK
#define HAL_SRV_FE2SOCTHR_READABLE    1
#define HAL_SRV_FE2SOCTHR_WRITABLE    1
#define HAL_SRV_FE2SOCTHR_SIGNED      0
#define HAL_SRV_FE2SOCTHR_ADR         0xB00001D8

// Register SPINCTL
#define HAL_SRV_WBLCK_REGISTER    (HAL_SRVREG_MAP->SPINCTL)
#define HAL_SRV_WBLCK_LSB         5
#define HAL_SRV_WBLCK_MSB         5
#define HAL_SRV_WBLCK_MASK        0x00000020
#define HAL_SRV_WBLCK_TYPE        Bool
#define HAL_SRV_WBLCK             HAL_SRV_WBLCK_MASK
#define HAL_SRV_WBLCK_READABLE    1
#define HAL_SRV_WBLCK_WRITABLE    0
#define HAL_SRV_WBLCK_SIGNED      0
#define HAL_SRV_WBLCK_ADR         0xB00001DC

#define HAL_SRV_FGLOCK_REGISTER    (HAL_SRVREG_MAP->SPINCTL)
#define HAL_SRV_FGLOCK_LSB         4
#define HAL_SRV_FGLOCK_MSB         4
#define HAL_SRV_FGLOCK_MASK        0x00000010
#define HAL_SRV_FGLOCK_TYPE        Bool
#define HAL_SRV_FGLOCK             HAL_SRV_FGLOCK_MASK
#define HAL_SRV_FGLOCK_READABLE    1
#define HAL_SRV_FGLOCK_WRITABLE    0
#define HAL_SRV_FGLOCK_SIGNED      0
#define HAL_SRV_FGLOCK_ADR         0xB00001DC

#define HAL_SRV_AFCG_REGISTER    (HAL_SRVREG_MAP->SPINCTL)
#define HAL_SRV_AFCG_LSB         1
#define HAL_SRV_AFCG_MSB         3
#define HAL_SRV_AFCG_MASK        0x0000000E
#define HAL_SRV_AFCG_TYPE        halSRV_AfcGain_t
#define HAL_SRV_AFCG             HAL_SRV_AFCG_MASK
#define HAL_SRV_AFCG_READABLE    1
#define HAL_SRV_AFCG_WRITABLE    1
#define HAL_SRV_AFCG_SIGNED      0
#define HAL_SRV_AFCG_ADR         0xB00001DC

#define HAL_SRV_SWITCHFG_REGISTER    (HAL_SRVREG_MAP->SPINCTL)
#define HAL_SRV_SWITCHFG_LSB         0
#define HAL_SRV_SWITCHFG_MSB         0
#define HAL_SRV_SWITCHFG_MASK        0x00000001
#define HAL_SRV_SWITCHFG_TYPE        Bool
#define HAL_SRV_SWITCHFG             HAL_SRV_SWITCHFG_MASK
#define HAL_SRV_SWITCHFG_READABLE    1
#define HAL_SRV_SWITCHFG_WRITABLE    1
#define HAL_SRV_SWITCHFG_SIGNED      0
#define HAL_SRV_SWITCHFG_ADR         0xB00001DC

// Register CAVM

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_AVGK_REGISTER    (HAL_SRVREG_MAP->CAVM)
#define HAL_SRV_AVGK_LSB         12
#define HAL_SRV_AVGK_MSB         14
#define HAL_SRV_AVGK_MASK        0x00007000
#define HAL_SRV_AVGK_TYPE        UByte
#define HAL_SRV_AVGK             HAL_SRV_AVGK_MASK
#define HAL_SRV_AVGK_READABLE    1
#define HAL_SRV_AVGK_WRITABLE    1
#define HAL_SRV_AVGK_SIGNED      0
#define HAL_SRV_AVGK_ADR         0xB00001E8

#define HAL_SRV_KDIV_REGISTER    (HAL_SRVREG_MAP->CAVM)
#define HAL_SRV_KDIV_LSB         8
#define HAL_SRV_KDIV_MSB         11
#define HAL_SRV_KDIV_MASK        0x00000F00
#define HAL_SRV_KDIV_TYPE        UByte
#define HAL_SRV_KDIV             HAL_SRV_KDIV_MASK
#define HAL_SRV_KDIV_READABLE    1
#define HAL_SRV_KDIV_WRITABLE    1
#define HAL_SRV_KDIV_SIGNED      0
#define HAL_SRV_KDIV_ADR         0xB00001E8

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_MDIV_REGISTER    (HAL_SRVREG_MAP->CAVM)
#define HAL_SRV_MDIV_LSB         0
#define HAL_SRV_MDIV_MSB         7
#define HAL_SRV_MDIV_MASK        0x000000FF
#define HAL_SRV_MDIV_TYPE        UByte
#define HAL_SRV_MDIV             HAL_SRV_MDIV_MASK
#define HAL_SRV_MDIV_READABLE    1
#define HAL_SRV_MDIV_WRITABLE    1
#define HAL_SRV_MDIV_SIGNED      0
#define HAL_SRV_MDIV_ADR         0xB00001E8

// Register CLVN
#define HAL_SRV_NDIV_REGISTER    (HAL_SRVREG_MAP->CLVN)
#define HAL_SRV_NDIV_LSB         0
#define HAL_SRV_NDIV_MSB         9
#define HAL_SRV_NDIV_MASK        0x000003FF
#define HAL_SRV_NDIV_TYPE        halSRV_NDIV_Gain_t
#define HAL_SRV_NDIV             HAL_SRV_NDIV_MASK
#define HAL_SRV_NDIV_READABLE    1
#define HAL_SRV_NDIV_WRITABLE    1
#define HAL_SRV_NDIV_SIGNED      0
#define HAL_SRV_NDIV_ADR         0xB00001F0

// Register FGRNG
#define HAL_SRV_FGRNG_REGISTER    (HAL_SRVREG_MAP->FGRNG)
#define HAL_SRV_FGRNG_LSB         0
#define HAL_SRV_FGRNG_MSB         15
#define HAL_SRV_FGRNG_MASK        0x0000FFFF
#define HAL_SRV_FGRNG_TYPE        UInt16
#define HAL_SRV_FGRNG             HAL_SRV_FGRNG_MASK
#define HAL_SRV_FGRNG_READABLE    1
#define HAL_SRV_FGRNG_WRITABLE    1
#define HAL_SRV_FGRNG_SIGNED      0
#define HAL_SRV_FGRNG_ADR         0xB00001F8

// Register REVCNT
#define HAL_SRV_REVCNT_REGISTER    (HAL_SRVREG_MAP->REVCNT)
#define HAL_SRV_REVCNT_LSB         0
#define HAL_SRV_REVCNT_MSB         15
#define HAL_SRV_REVCNT_MASK        0x0000FFFF
#define HAL_SRV_REVCNT_TYPE        UInt16
#define HAL_SRV_REVCNT             HAL_SRV_REVCNT_MASK
#define HAL_SRV_REVCNT_READABLE    1
#define HAL_SRV_REVCNT_WRITABLE    0
#define HAL_SRV_REVCNT_SIGNED      0
#define HAL_SRV_REVCNT_ADR         0xB0000200

// Register INITR
#define HAL_SRV_BRES_REGISTER    (HAL_SRVREG_MAP->INITR)
#define HAL_SRV_BRES_LSB         7
#define HAL_SRV_BRES_MSB         7
#define HAL_SRV_BRES_MASK        0x00000080
#define HAL_SRV_BRES_TYPE        Bool
#define HAL_SRV_BRES             HAL_SRV_BRES_MASK
#define HAL_SRV_BRES_READABLE    1
#define HAL_SRV_BRES_WRITABLE    1
#define HAL_SRV_BRES_SIGNED      0
#define HAL_SRV_BRES_ADR         0xB0000208

#define HAL_SRV_RRES_REGISTER    (HAL_SRVREG_MAP->INITR)
#define HAL_SRV_RRES_LSB         6
#define HAL_SRV_RRES_MSB         6
#define HAL_SRV_RRES_MASK        0x00000040
#define HAL_SRV_RRES_TYPE        Bool
#define HAL_SRV_RRES             HAL_SRV_RRES_MASK
#define HAL_SRV_RRES_READABLE    1
#define HAL_SRV_RRES_WRITABLE    1
#define HAL_SRV_RRES_SIGNED      0
#define HAL_SRV_RRES_ADR         0xB0000208

#define HAL_SRV_FRES_REGISTER    (HAL_SRVREG_MAP->INITR)
#define HAL_SRV_FRES_LSB         5
#define HAL_SRV_FRES_MSB         5
#define HAL_SRV_FRES_MASK        0x00000020
#define HAL_SRV_FRES_TYPE        Bool
#define HAL_SRV_FRES             HAL_SRV_FRES_MASK
#define HAL_SRV_FRES_READABLE    1
#define HAL_SRV_FRES_WRITABLE    1
#define HAL_SRV_FRES_SIGNED      0
#define HAL_SRV_FRES_ADR         0xB0000208

#define HAL_SRV_TRES_REGISTER    (HAL_SRVREG_MAP->INITR)
#define HAL_SRV_TRES_LSB         4
#define HAL_SRV_TRES_MSB         4
#define HAL_SRV_TRES_MASK        0x00000010
#define HAL_SRV_TRES_TYPE        Bool
#define HAL_SRV_TRES             HAL_SRV_TRES_MASK
#define HAL_SRV_TRES_READABLE    1
#define HAL_SRV_TRES_WRITABLE    1
#define HAL_SRV_TRES_SIGNED      0
#define HAL_SRV_TRES_ADR         0xB0000208

#define HAL_SRV_FMTRES_REGISTER    (HAL_SRVREG_MAP->INITR)
#define HAL_SRV_FMTRES_LSB         3
#define HAL_SRV_FMTRES_MSB         3
#define HAL_SRV_FMTRES_MASK        0x00000008
#define HAL_SRV_FMTRES_TYPE        Bool
#define HAL_SRV_FMTRES             HAL_SRV_FMTRES_MASK
#define HAL_SRV_FMTRES_READABLE    1
#define HAL_SRV_FMTRES_WRITABLE    1
#define HAL_SRV_FMTRES_SIGNED      0
#define HAL_SRV_FMTRES_ADR         0xB0000208

#define HAL_SRV_FMDRES_REGISTER    (HAL_SRVREG_MAP->INITR)
#define HAL_SRV_FMDRES_LSB         2
#define HAL_SRV_FMDRES_MSB         2
#define HAL_SRV_FMDRES_MASK        0x00000004
#define HAL_SRV_FMDRES_TYPE        Bool
#define HAL_SRV_FMDRES             HAL_SRV_FMDRES_MASK
#define HAL_SRV_FMDRES_READABLE    1
#define HAL_SRV_FMDRES_WRITABLE    1
#define HAL_SRV_FMDRES_SIGNED      0
#define HAL_SRV_FMDRES_ADR         0xB0000208

// Register PUSTP
#define HAL_SRV_TSVSTP_REGISTER    (HAL_SRVREG_MAP->PUSTP)
#define HAL_SRV_TSVSTP_LSB         7
#define HAL_SRV_TSVSTP_MSB         7
#define HAL_SRV_TSVSTP_MASK        0x00000080
#define HAL_SRV_TSVSTP_TYPE        Bool
#define HAL_SRV_TSVSTP             HAL_SRV_TSVSTP_MASK
#define HAL_SRV_TSVSTP_READABLE    1
#define HAL_SRV_TSVSTP_WRITABLE    1
#define HAL_SRV_TSVSTP_SIGNED      0
#define HAL_SRV_TSVSTP_ADR         0xB000020C

// Register PUFWD
#define HAL_SRV_TSVFWD_REGISTER    (HAL_SRVREG_MAP->PUFWD)
#define HAL_SRV_TSVFWD_LSB         7
#define HAL_SRV_TSVFWD_MSB         7
#define HAL_SRV_TSVFWD_MASK        0x00000080
#define HAL_SRV_TSVFWD_TYPE        Bool
#define HAL_SRV_TSVFWD             HAL_SRV_TSVFWD_MASK
#define HAL_SRV_TSVFWD_READABLE    1
#define HAL_SRV_TSVFWD_WRITABLE    1
#define HAL_SRV_TSVFWD_SIGNED      0
#define HAL_SRV_TSVFWD_ADR         0xB0000210

// Register PUBWD
#define HAL_SRV_TSVBWD_REGISTER    (HAL_SRVREG_MAP->PUBWD)
#define HAL_SRV_TSVBWD_LSB         7
#define HAL_SRV_TSVBWD_MSB         7
#define HAL_SRV_TSVBWD_MASK        0x00000080
#define HAL_SRV_TSVBWD_TYPE        Bool
#define HAL_SRV_TSVBWD             HAL_SRV_TSVBWD_MASK
#define HAL_SRV_TSVBWD_READABLE    1
#define HAL_SRV_TSVBWD_WRITABLE    1
#define HAL_SRV_TSVBWD_SIGNED      0
#define HAL_SRV_TSVBWD_ADR         0xB0000214

// Register LDON
#define HAL_SRV_LDON_REGISTER    (HAL_SRVREG_MAP->LDON)
#define HAL_SRV_LDON_LSB         0
#define HAL_SRV_LDON_MSB         7
#define HAL_SRV_LDON_MASK        0x000000FF
#define HAL_SRV_LDON_TYPE        UByte
#define HAL_SRV_LDON             HAL_SRV_LDON_MASK
#define HAL_SRV_LDON_READABLE    0
#define HAL_SRV_LDON_WRITABLE    1
#define HAL_SRV_LDON_SIGNED      0
#define HAL_SRV_LDON_ADR         0xB0000218

// Register LDOFF
#define HAL_SRV_LDOFF_REGISTER    (HAL_SRVREG_MAP->LDOFF)
#define HAL_SRV_LDOFF_LSB         0
#define HAL_SRV_LDOFF_MSB         7
#define HAL_SRV_LDOFF_MASK        0x000000FF
#define HAL_SRV_LDOFF_TYPE        UByte
#define HAL_SRV_LDOFF             HAL_SRV_LDOFF_MASK
#define HAL_SRV_LDOFF_READABLE    0
#define HAL_SRV_LDOFF_WRITABLE    1
#define HAL_SRV_LDOFF_SIGNED      0
#define HAL_SRV_LDOFF_ADR         0xB000021C

// Register DMSV
#define HAL_SRV_IFOK_REGISTER    (HAL_SRVREG_MAP->DMSV)
#define HAL_SRV_IFOK_LSB         1
#define HAL_SRV_IFOK_MSB         1
#define HAL_SRV_IFOK_MASK        0x00000002
#define HAL_SRV_IFOK_TYPE        Bool
#define HAL_SRV_IFOK             HAL_SRV_IFOK_MASK
#define HAL_SRV_IFOK_READABLE    1
#define HAL_SRV_IFOK_WRITABLE    1
#define HAL_SRV_IFOK_SIGNED      0
#define HAL_SRV_IFOK_ADR         0xB0000220

#define HAL_SRV_TSE_REGISTER    (HAL_SRVREG_MAP->DMSV)
#define HAL_SRV_TSE_LSB         0
#define HAL_SRV_TSE_MSB         0
#define HAL_SRV_TSE_MASK        0x00000001
#define HAL_SRV_TSE_TYPE        Bool
#define HAL_SRV_TSE             HAL_SRV_TSE_MASK
#define HAL_SRV_TSE_READABLE    1
#define HAL_SRV_TSE_WRITABLE    1
#define HAL_SRV_TSE_SIGNED      0
#define HAL_SRV_TSE_ADR         0xB0000220

// Register DMOFF
#define HAL_SRV_DMOFF_REGISTER    (HAL_SRVREG_MAP->DMOFF)
#define HAL_SRV_DMOFF_LSB         0
#define HAL_SRV_DMOFF_MSB         7
#define HAL_SRV_DMOFF_MASK        0x000000FF
#define HAL_SRV_DMOFF_TYPE        UByte
#define HAL_SRV_DMOFF             HAL_SRV_DMOFF_MASK
#define HAL_SRV_DMOFF_READABLE    0
#define HAL_SRV_DMOFF_WRITABLE    1
#define HAL_SRV_DMOFF_SIGNED      0
#define HAL_SRV_DMOFF_ADR         0xB0000224

// Register FOLNSUP
#define HAL_SRV_DLPOLSET_REGISTER    (HAL_SRVREG_MAP->FOLNSUP)
#define HAL_SRV_DLPOLSET_LSB         7
#define HAL_SRV_DLPOLSET_MSB         7
#define HAL_SRV_DLPOLSET_MASK        0x00000080
#define HAL_SRV_DLPOLSET_TYPE        Bool
#define HAL_SRV_DLPOLSET             HAL_SRV_DLPOLSET_MASK
#define HAL_SRV_DLPOLSET_READABLE    1
#define HAL_SRV_DLPOLSET_WRITABLE    1
#define HAL_SRV_DLPOLSET_SIGNED      0
#define HAL_SRV_DLPOLSET_ADR         0xB0000228

#define HAL_SRV_FGCSET_REGISTER    (HAL_SRVREG_MAP->FOLNSUP)
#define HAL_SRV_FGCSET_LSB         6
#define HAL_SRV_FGCSET_MSB         6
#define HAL_SRV_FGCSET_MASK        0x00000040
#define HAL_SRV_FGCSET_TYPE        Bool
#define HAL_SRV_FGCSET             HAL_SRV_FGCSET_MASK
#define HAL_SRV_FGCSET_READABLE    1
#define HAL_SRV_FGCSET_WRITABLE    1
#define HAL_SRV_FGCSET_SIGNED      0
#define HAL_SRV_FGCSET_ADR         0xB0000228

#define HAL_SRV_HISRSET_REGISTER    (HAL_SRVREG_MAP->FOLNSUP)
#define HAL_SRV_HISRSET_LSB         5
#define HAL_SRV_HISRSET_MSB         5
#define HAL_SRV_HISRSET_MASK        0x00000020
#define HAL_SRV_HISRSET_TYPE        Bool
#define HAL_SRV_HISRSET             HAL_SRV_HISRSET_MASK
#define HAL_SRV_HISRSET_READABLE    1
#define HAL_SRV_HISRSET_WRITABLE    1
#define HAL_SRV_HISRSET_SIGNED      0
#define HAL_SRV_HISRSET_ADR         0xB0000228

#define HAL_SRV_FSONSET_REGISTER    (HAL_SRVREG_MAP->FOLNSUP)
#define HAL_SRV_FSONSET_LSB         4
#define HAL_SRV_FSONSET_MSB         4
#define HAL_SRV_FSONSET_MASK        0x00000010
#define HAL_SRV_FSONSET_TYPE        Bool
#define HAL_SRV_FSONSET             HAL_SRV_FSONSET_MASK
#define HAL_SRV_FSONSET_READABLE    1
#define HAL_SRV_FSONSET_WRITABLE    1
#define HAL_SRV_FSONSET_SIGNED      0
#define HAL_SRV_FSONSET_ADR         0xB0000228

#define HAL_SRV_SUSRSET_REGISTER    (HAL_SRVREG_MAP->FOLNSUP)
#define HAL_SRV_SUSRSET_LSB         3
#define HAL_SRV_SUSRSET_MSB         3
#define HAL_SRV_SUSRSET_MASK        0x00000008
#define HAL_SRV_SUSRSET_TYPE        Bool
#define HAL_SRV_SUSRSET             HAL_SRV_SUSRSET_MASK
#define HAL_SRV_SUSRSET_READABLE    1
#define HAL_SRV_SUSRSET_WRITABLE    1
#define HAL_SRV_SUSRSET_SIGNED      0
#define HAL_SRV_SUSRSET_ADR         0xB0000228

// Register FOLNSDN
#define HAL_SRV_DLPOLRST_REGISTER    (HAL_SRVREG_MAP->FOLNSDN)
#define HAL_SRV_DLPOLRST_LSB         7
#define HAL_SRV_DLPOLRST_MSB         7
#define HAL_SRV_DLPOLRST_MASK        0x00000080
#define HAL_SRV_DLPOLRST_TYPE        Bool
#define HAL_SRV_DLPOLRST             HAL_SRV_DLPOLRST_MASK
#define HAL_SRV_DLPOLRST_READABLE    1
#define HAL_SRV_DLPOLRST_WRITABLE    1
#define HAL_SRV_DLPOLRST_SIGNED      0
#define HAL_SRV_DLPOLRST_ADR         0xB000022C

#define HAL_SRV_FGCRST_REGISTER    (HAL_SRVREG_MAP->FOLNSDN)
#define HAL_SRV_FGCRST_LSB         6
#define HAL_SRV_FGCRST_MSB         6
#define HAL_SRV_FGCRST_MASK        0x00000040
#define HAL_SRV_FGCRST_TYPE        Bool
#define HAL_SRV_FGCRST             HAL_SRV_FGCRST_MASK
#define HAL_SRV_FGCRST_READABLE    1
#define HAL_SRV_FGCRST_WRITABLE    1
#define HAL_SRV_FGCRST_SIGNED      0
#define HAL_SRV_FGCRST_ADR         0xB000022C

#define HAL_SRV_HISRRST_REGISTER    (HAL_SRVREG_MAP->FOLNSDN)
#define HAL_SRV_HISRRST_LSB         5
#define HAL_SRV_HISRRST_MSB         5
#define HAL_SRV_HISRRST_MASK        0x00000020
#define HAL_SRV_HISRRST_TYPE        Bool
#define HAL_SRV_HISRRST             HAL_SRV_HISRRST_MASK
#define HAL_SRV_HISRRST_READABLE    1
#define HAL_SRV_HISRRST_WRITABLE    1
#define HAL_SRV_HISRRST_SIGNED      0
#define HAL_SRV_HISRRST_ADR         0xB000022C

#define HAL_SRV_FSONRST_REGISTER    (HAL_SRVREG_MAP->FOLNSDN)
#define HAL_SRV_FSONRST_LSB         4
#define HAL_SRV_FSONRST_MSB         4
#define HAL_SRV_FSONRST_MASK        0x00000010
#define HAL_SRV_FSONRST_TYPE        Bool
#define HAL_SRV_FSONRST             HAL_SRV_FSONRST_MASK
#define HAL_SRV_FSONRST_READABLE    1
#define HAL_SRV_FSONRST_WRITABLE    1
#define HAL_SRV_FSONRST_SIGNED      0
#define HAL_SRV_FSONRST_ADR         0xB000022C

#define HAL_SRV_SUSRRST_REGISTER    (HAL_SRVREG_MAP->FOLNSDN)
#define HAL_SRV_SUSRRST_LSB         3
#define HAL_SRV_SUSRRST_MSB         3
#define HAL_SRV_SUSRRST_MASK        0x00000008
#define HAL_SRV_SUSRRST_TYPE        Bool
#define HAL_SRV_SUSRRST             HAL_SRV_SUSRRST_MASK
#define HAL_SRV_SUSRRST_READABLE    1
#define HAL_SRV_SUSRRST_WRITABLE    1
#define HAL_SRV_SUSRRST_SIGNED      0
#define HAL_SRV_SUSRRST_ADR         0xB000022C

// Register DMFBK
#define HAL_SRV_DMFK_REGISTER    (HAL_SRVREG_MAP->DMFBK)
#define HAL_SRV_DMFK_LSB         5
#define HAL_SRV_DMFK_MSB         5
#define HAL_SRV_DMFK_MASK        0x00000020
#define HAL_SRV_DMFK_TYPE        Bool
#define HAL_SRV_DMFK             HAL_SRV_DMFK_MASK
#define HAL_SRV_DMFK_READABLE    1
#define HAL_SRV_DMFK_WRITABLE    1
#define HAL_SRV_DMFK_SIGNED      0
#define HAL_SRV_DMFK_ADR         0xB0000230

#define HAL_SRV_DMBK_REGISTER    (HAL_SRVREG_MAP->DMFBK)
#define HAL_SRV_DMBK_LSB         4
#define HAL_SRV_DMBK_MSB         4
#define HAL_SRV_DMBK_MASK        0x00000010
#define HAL_SRV_DMBK_TYPE        Bool
#define HAL_SRV_DMBK             HAL_SRV_DMBK_MASK
#define HAL_SRV_DMBK_READABLE    1
#define HAL_SRV_DMBK_WRITABLE    1
#define HAL_SRV_DMBK_SIGNED      0
#define HAL_SRV_DMBK_ADR         0xB0000230

// Register PAC
#define HAL_SRV_PMO_REGISTER    (HAL_SRVREG_MAP->PAC)
#define HAL_SRV_PMO_LSB         7
#define HAL_SRV_PMO_MSB         7
#define HAL_SRV_PMO_MASK        0x00000080
#define HAL_SRV_PMO_TYPE        Bool
#define HAL_SRV_PMO             HAL_SRV_PMO_MASK
#define HAL_SRV_PMO_READABLE    1
#define HAL_SRV_PMO_WRITABLE    1
#define HAL_SRV_PMO_SIGNED      0
#define HAL_SRV_PMO_ADR         0xB0000234

#define HAL_SRV_PC_REGISTER    (HAL_SRVREG_MAP->PAC)
#define HAL_SRV_PC_LSB         0
#define HAL_SRV_PC_MSB         3
#define HAL_SRV_PC_MASK        0x0000000F
#define HAL_SRV_PC_TYPE        halSRV_PC_t
#define HAL_SRV_PC             HAL_SRV_PC_MASK
#define HAL_SRV_PC_READABLE    1
#define HAL_SRV_PC_WRITABLE    1
#define HAL_SRV_PC_SIGNED      0
#define HAL_SRV_PC_ADR         0xB0000234

// Register PAF
#define HAL_SRV_PF_REGISTER    (HAL_SRVREG_MAP->PAF)
#define HAL_SRV_PF_LSB         0
#define HAL_SRV_PF_MSB         3
#define HAL_SRV_PF_MASK        0x0000000F
#define HAL_SRV_PF_TYPE        halSRV_PF_t
#define HAL_SRV_PF             HAL_SRV_PF_MASK
#define HAL_SRV_PF_READABLE    1
#define HAL_SRV_PF_WRITABLE    1
#define HAL_SRV_PF_SIGNED      0
#define HAL_SRV_PF_ADR         0xB0000238

// Register SLED
#define HAL_SRV_SLED_REGISTER    (HAL_SRVREG_MAP->SLED)
#define HAL_SRV_SLED_LSB         0
#define HAL_SRV_SLED_MSB         7
#define HAL_SRV_SLED_MASK        0x000000FF
#define HAL_SRV_SLED_TYPE        SByte
#define HAL_SRV_SLED             HAL_SRV_SLED_MASK
#define HAL_SRV_SLED_READABLE    1
#define HAL_SRV_SLED_WRITABLE    0
#define HAL_SRV_SLED_SIGNED      1
#define HAL_SRV_SLED_ADR         0xB000023C

// Register FSCNTL
#define HAL_SRV_FEAVE_REGISTER    (HAL_SRVREG_MAP->FSCNTL)
#define HAL_SRV_FEAVE_LSB         3
#define HAL_SRV_FEAVE_MSB         3
#define HAL_SRV_FEAVE_MASK        0x00000008
#define HAL_SRV_FEAVE_TYPE        Bool
#define HAL_SRV_FEAVE             HAL_SRV_FEAVE_MASK
#define HAL_SRV_FEAVE_READABLE    1
#define HAL_SRV_FEAVE_WRITABLE    1
#define HAL_SRV_FEAVE_SIGNED      0
#define HAL_SRV_FEAVE_ADR         0xB0000240

#define HAL_SRV_WIDTH_REGISTER    (HAL_SRVREG_MAP->FSCNTL)
#define HAL_SRV_WIDTH_LSB         2
#define HAL_SRV_WIDTH_MSB         2
#define HAL_SRV_WIDTH_MASK        0x00000004
#define HAL_SRV_WIDTH_TYPE        halSRV_WIDTH_t
#define HAL_SRV_WIDTH             HAL_SRV_WIDTH_MASK
#define HAL_SRV_WIDTH_READABLE    1
#define HAL_SRV_WIDTH_WRITABLE    1
#define HAL_SRV_WIDTH_SIGNED      0
#define HAL_SRV_WIDTH_ADR         0xB0000240

#define HAL_SRV_WINDOW_REGISTER    (HAL_SRVREG_MAP->FSCNTL)
#define HAL_SRV_WINDOW_LSB         1
#define HAL_SRV_WINDOW_MSB         1
#define HAL_SRV_WINDOW_MASK        0x00000002
#define HAL_SRV_WINDOW_TYPE        Bool
#define HAL_SRV_WINDOW             HAL_SRV_WINDOW_MASK
#define HAL_SRV_WINDOW_READABLE    1
#define HAL_SRV_WINDOW_WRITABLE    1
#define HAL_SRV_WINDOW_SIGNED      0
#define HAL_SRV_WINDOW_ADR         0xB0000240

#define HAL_SRV_INTRACT_REGISTER    (HAL_SRVREG_MAP->FSCNTL)
#define HAL_SRV_INTRACT_LSB         0
#define HAL_SRV_INTRACT_MSB         0
#define HAL_SRV_INTRACT_MASK        0x00000001
#define HAL_SRV_INTRACT_TYPE        Bool
#define HAL_SRV_INTRACT             HAL_SRV_INTRACT_MASK
#define HAL_SRV_INTRACT_READABLE    1
#define HAL_SRV_INTRACT_WRITABLE    1
#define HAL_SRV_INTRACT_SIGNED      0
#define HAL_SRV_INTRACT_ADR         0xB0000240

// Register FSTHRES
#define HAL_SRV_FSTHRES_REGISTER    (HAL_SRVREG_MAP->FSTHRES)
#define HAL_SRV_FSTHRES_LSB         0
#define HAL_SRV_FSTHRES_MSB         7
#define HAL_SRV_FSTHRES_MASK        0x000000FF
#define HAL_SRV_FSTHRES_TYPE        UByte
#define HAL_SRV_FSTHRES             HAL_SRV_FSTHRES_MASK
#define HAL_SRV_FSTHRES_READABLE    1
#define HAL_SRV_FSTHRES_WRITABLE    1
#define HAL_SRV_FSTHRES_SIGNED      0
#define HAL_SRV_FSTHRES_ADR         0xB0000244

// Register SHCCTS
#define HAL_SRV_SHCCTS_REGISTER    (HAL_SRVREG_MAP->SHCCTS)
#define HAL_SRV_SHCCTS_LSB         0
#define HAL_SRV_SHCCTS_MSB         7
#define HAL_SRV_SHCCTS_MASK        0x000000FF
#define HAL_SRV_SHCCTS_TYPE        UByte
#define HAL_SRV_SHCCTS             HAL_SRV_SHCCTS_MASK
#define HAL_SRV_SHCCTS_READABLE    1
#define HAL_SRV_SHCCTS_WRITABLE    0
#define HAL_SRV_SHCCTS_SIGNED      0
#define HAL_SRV_SHCCTS_ADR         0xB0000248

// Register CTTHRES
#define HAL_SRV_CTTHRES_REGISTER    (HAL_SRVREG_MAP->CTTHRES)
#define HAL_SRV_CTTHRES_LSB         0
#define HAL_SRV_CTTHRES_MSB         7
#define HAL_SRV_CTTHRES_MASK        0x000000FF
#define HAL_SRV_CTTHRES_TYPE        UByte
#define HAL_SRV_CTTHRES             HAL_SRV_CTTHRES_MASK
#define HAL_SRV_CTTHRES_READABLE    1
#define HAL_SRV_CTTHRES_WRITABLE    1
#define HAL_SRV_CTTHRES_SIGNED      0
#define HAL_SRV_CTTHRES_ADR         0xB000024C

// Register SYNCNT

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_FGCNT_REGISTER    (HAL_SRVREG_MAP->SYNCNT)
#define HAL_SRV_FGCNT_LSB         16
#define HAL_SRV_FGCNT_MSB         31
#define HAL_SRV_FGCNT_MASK        0xFFFF0000
#define HAL_SRV_FGCNT_TYPE        UInt16
#define HAL_SRV_FGCNT             HAL_SRV_FGCNT_MASK
#define HAL_SRV_FGCNT_READABLE    1
#define HAL_SRV_FGCNT_WRITABLE    0
#define HAL_SRV_FGCNT_SIGNED      0
#define HAL_SRV_FGCNT_ADR         0xB0000250

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_SYNCNT_REGISTER    (HAL_SRVREG_MAP->SYNCNT)
#define HAL_SRV_SYNCNT_LSB         0
#define HAL_SRV_SYNCNT_MSB         15
#define HAL_SRV_SYNCNT_MASK        0x0000FFFF
#define HAL_SRV_SYNCNT_TYPE        UInt16
#define HAL_SRV_SYNCNT             HAL_SRV_SYNCNT_MASK
#define HAL_SRV_SYNCNT_READABLE    1
#define HAL_SRV_SYNCNT_WRITABLE    0
#define HAL_SRV_SYNCNT_SIGNED      0
#define HAL_SRV_SYNCNT_ADR         0xB0000250

// Register SYNTHR
#define HAL_SRV_SYNTHR_REGISTER    (HAL_SRVREG_MAP->SYNTHR)
#define HAL_SRV_SYNTHR_LSB         0
#define HAL_SRV_SYNTHR_MSB         15
#define HAL_SRV_SYNTHR_MASK        0x0000FFFF
#define HAL_SRV_SYNTHR_TYPE        UInt16
#define HAL_SRV_SYNTHR             HAL_SRV_SYNTHR_MASK
#define HAL_SRV_SYNTHR_READABLE    1
#define HAL_SRV_SYNTHR_WRITABLE    1
#define HAL_SRV_SYNTHR_SIGNED      0
#define HAL_SRV_SYNTHR_ADR         0xB0000258

// Register TILTOFS

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_TILTOFS_REGISTER    (HAL_SRVREG_MAP->TILTOFS)
#define HAL_SRV_TILTOFS_LSB         0
#define HAL_SRV_TILTOFS_MSB         7
#define HAL_SRV_TILTOFS_MASK        0x000000FF
#define HAL_SRV_TILTOFS_TYPE        SByte
#define HAL_SRV_TILTOFS             HAL_SRV_TILTOFS_MASK
#define HAL_SRV_TILTOFS_READABLE    1
#define HAL_SRV_TILTOFS_WRITABLE    1
#define HAL_SRV_TILTOFS_SIGNED      1
#define HAL_SRV_TILTOFS_ADR         0xB000026C

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_TILTOFS_REGISTER    (HAL_SRVREG_MAP->TILTOFS)
#define HAL_SRV_TILTOFS_LSB         0
#define HAL_SRV_TILTOFS_MSB         9
#define HAL_SRV_TILTOFS_MASK        0x000003FF
#define HAL_SRV_TILTOFS_TYPE        SInt16
#define HAL_SRV_TILTOFS             HAL_SRV_TILTOFS_MASK
#define HAL_SRV_TILTOFS_READABLE    1
#define HAL_SRV_TILTOFS_WRITABLE    1
#define HAL_SRV_TILTOFS_SIGNED      1
#define HAL_SRV_TILTOFS_ADR         0xB000026C

#endif // #if (CHIP_REV >= 0xC0)

// Register FVKICM
#define HAL_SRV_FVKSURF_REGISTER    (HAL_SRVREG_MAP->FVKICM)
#define HAL_SRV_FVKSURF_LSB         6
#define HAL_SRV_FVKSURF_MSB         6
#define HAL_SRV_FVKSURF_MASK        0x00000040
#define HAL_SRV_FVKSURF_TYPE        Bool
#define HAL_SRV_FVKSURF             HAL_SRV_FVKSURF_MASK
#define HAL_SRV_FVKSURF_READABLE    1
#define HAL_SRV_FVKSURF_WRITABLE    1
#define HAL_SRV_FVKSURF_SIGNED      0
#define HAL_SRV_FVKSURF_ADR         0xB0000280

#define HAL_SRV_FVKRESEK_REGISTER    (HAL_SRVREG_MAP->FVKICM)
#define HAL_SRV_FVKRESEK_LSB         5
#define HAL_SRV_FVKRESEK_MSB         5
#define HAL_SRV_FVKRESEK_MASK        0x00000020
#define HAL_SRV_FVKRESEK_TYPE        Bool
#define HAL_SRV_FVKRESEK             HAL_SRV_FVKRESEK_MASK
#define HAL_SRV_FVKRESEK_READABLE    1
#define HAL_SRV_FVKRESEK_WRITABLE    1
#define HAL_SRV_FVKRESEK_SIGNED      0
#define HAL_SRV_FVKRESEK_ADR         0xB0000280

#define HAL_SRV_FVKHYSC_REGISTER    (HAL_SRVREG_MAP->FVKICM)
#define HAL_SRV_FVKHYSC_LSB         4
#define HAL_SRV_FVKHYSC_MSB         4
#define HAL_SRV_FVKHYSC_MASK        0x00000010
#define HAL_SRV_FVKHYSC_TYPE        Bool
#define HAL_SRV_FVKHYSC             HAL_SRV_FVKHYSC_MASK
#define HAL_SRV_FVKHYSC_READABLE    1
#define HAL_SRV_FVKHYSC_WRITABLE    1
#define HAL_SRV_FVKHYSC_SIGNED      0
#define HAL_SRV_FVKHYSC_ADR         0xB0000280

#define HAL_SRV_FVKFDC_REGISTER    (HAL_SRVREG_MAP->FVKICM)
#define HAL_SRV_FVKFDC_LSB         2
#define HAL_SRV_FVKFDC_MSB         2
#define HAL_SRV_FVKFDC_MASK        0x00000004
#define HAL_SRV_FVKFDC_TYPE        Bool
#define HAL_SRV_FVKFDC             HAL_SRV_FVKFDC_MASK
#define HAL_SRV_FVKFDC_READABLE    1
#define HAL_SRV_FVKFDC_WRITABLE    1
#define HAL_SRV_FVKFDC_SIGNED      0
#define HAL_SRV_FVKFDC_ADR         0xB0000280

#define HAL_SRV_FVKTGC_REGISTER    (HAL_SRVREG_MAP->FVKICM)
#define HAL_SRV_FVKTGC_LSB         1
#define HAL_SRV_FVKTGC_MSB         1
#define HAL_SRV_FVKTGC_MASK        0x00000002
#define HAL_SRV_FVKTGC_TYPE        Bool
#define HAL_SRV_FVKTGC             HAL_SRV_FVKTGC_MASK
#define HAL_SRV_FVKTGC_READABLE    1
#define HAL_SRV_FVKTGC_WRITABLE    1
#define HAL_SRV_FVKTGC_SIGNED      0
#define HAL_SRV_FVKTGC_ADR         0xB0000280

#define HAL_SRV_FVKBF_REGISTER    (HAL_SRVREG_MAP->FVKICM)
#define HAL_SRV_FVKBF_LSB         0
#define HAL_SRV_FVKBF_MSB         0
#define HAL_SRV_FVKBF_MASK        0x00000001
#define HAL_SRV_FVKBF_TYPE        halSRV_FVKBF_t
#define HAL_SRV_FVKBF             HAL_SRV_FVKBF_MASK
#define HAL_SRV_FVKBF_READABLE    1
#define HAL_SRV_FVKBF_WRITABLE    1
#define HAL_SRV_FVKBF_SIGNED      0
#define HAL_SRV_FVKBF_ADR         0xB0000280

// Register FVKICK
#define HAL_SRV_FVKICK_REGISTER    (HAL_SRVREG_MAP->FVKICK)
#define HAL_SRV_FVKICK_LSB         0
#define HAL_SRV_FVKICK_MSB         10
#define HAL_SRV_FVKICK_MASK        0x000007FF
#define HAL_SRV_FVKICK_TYPE        UInt16
#define HAL_SRV_FVKICK             HAL_SRV_FVKICK_MASK
#define HAL_SRV_FVKICK_READABLE    1
#define HAL_SRV_FVKICK_WRITABLE    1
#define HAL_SRV_FVKICK_SIGNED      0
#define HAL_SRV_FVKICK_ADR         0xB0000284

// Register GPDACMUX
#define HAL_SRV_DMOSEL_REGISTER    (HAL_SRVREG_MAP->GPDACMUX)
#define HAL_SRV_DMOSEL_LSB         7
#define HAL_SRV_DMOSEL_MSB         7
#define HAL_SRV_DMOSEL_MASK        0x00000080
#define HAL_SRV_DMOSEL_TYPE        Bool
#define HAL_SRV_DMOSEL             HAL_SRV_DMOSEL_MASK
#define HAL_SRV_DMOSEL_READABLE    1
#define HAL_SRV_DMOSEL_WRITABLE    1
#define HAL_SRV_DMOSEL_SIGNED      0
#define HAL_SRV_DMOSEL_ADR         0xB000029C

#define HAL_SRV_DAC2SEL_REGISTER    (HAL_SRVREG_MAP->GPDACMUX)
#define HAL_SRV_DAC2SEL_LSB         4
#define HAL_SRV_DAC2SEL_MSB         5
#define HAL_SRV_DAC2SEL_MASK        0x00000030
#define HAL_SRV_DAC2SEL_TYPE        halSRV_DAC2SEL_t
#define HAL_SRV_DAC2SEL             HAL_SRV_DAC2SEL_MASK
#define HAL_SRV_DAC2SEL_READABLE    1
#define HAL_SRV_DAC2SEL_WRITABLE    1
#define HAL_SRV_DAC2SEL_SIGNED      0
#define HAL_SRV_DAC2SEL_ADR         0xB000029C

#define HAL_SRV_DAC1SEL_REGISTER    (HAL_SRVREG_MAP->GPDACMUX)
#define HAL_SRV_DAC1SEL_LSB         2
#define HAL_SRV_DAC1SEL_MSB         2
#define HAL_SRV_DAC1SEL_MASK        0x00000004
#define HAL_SRV_DAC1SEL_TYPE        halSRV_DAC1SEL_t
#define HAL_SRV_DAC1SEL             HAL_SRV_DAC1SEL_MASK
#define HAL_SRV_DAC1SEL_READABLE    1
#define HAL_SRV_DAC1SEL_WRITABLE    1
#define HAL_SRV_DAC1SEL_SIGNED      0
#define HAL_SRV_DAC1SEL_ADR         0xB000029C

// Register CTN
#define HAL_SRV_CTN_REGISTER    (HAL_SRVREG_MAP->CTN)
#define HAL_SRV_CTN_LSB         0
#define HAL_SRV_CTN_MSB         15
#define HAL_SRV_CTN_MASK        0x0000FFFF
#define HAL_SRV_CTN_TYPE        UInt16
#define HAL_SRV_CTN             HAL_SRV_CTN_MASK
#define HAL_SRV_CTN_READABLE    1
#define HAL_SRV_CTN_WRITABLE    1
#define HAL_SRV_CTN_SIGNED      0
#define HAL_SRV_CTN_ADR         0xB00002A0

// Register DACDAT
#define HAL_SRV_DACDAT2_REGISTER    (HAL_SRVREG_MAP->DACDAT)
#define HAL_SRV_DACDAT2_LSB         16
#define HAL_SRV_DACDAT2_MSB         25
#define HAL_SRV_DACDAT2_MASK        0x03FF0000
#define HAL_SRV_DACDAT2_TYPE        SInt16
#define HAL_SRV_DACDAT2             HAL_SRV_DACDAT2_MASK
#define HAL_SRV_DACDAT2_READABLE    1
#define HAL_SRV_DACDAT2_WRITABLE    1
#define HAL_SRV_DACDAT2_SIGNED      1
#define HAL_SRV_DACDAT2_ADR         0xB00002A8

#define HAL_SRV_DACDAT1_REGISTER    (HAL_SRVREG_MAP->DACDAT)
#define HAL_SRV_DACDAT1_LSB         0
#define HAL_SRV_DACDAT1_MSB         9
#define HAL_SRV_DACDAT1_MASK        0x000003FF
#define HAL_SRV_DACDAT1_TYPE        SInt16
#define HAL_SRV_DACDAT1             HAL_SRV_DACDAT1_MASK
#define HAL_SRV_DACDAT1_READABLE    1
#define HAL_SRV_DACDAT1_WRITABLE    1
#define HAL_SRV_DACDAT1_SIGNED      1
#define HAL_SRV_DACDAT1_ADR         0xB00002A8

// Register AFC
#define HAL_SRV_AFC_REGISTER    (HAL_SRVREG_MAP->AFC)
#define HAL_SRV_AFC_LSB         0
#define HAL_SRV_AFC_MSB         7
#define HAL_SRV_AFC_MASK        0x000000FF
#define HAL_SRV_AFC_TYPE        UByte
#define HAL_SRV_AFC             HAL_SRV_AFC_MASK
#define HAL_SRV_AFC_READABLE    1
#define HAL_SRV_AFC_WRITABLE    0
#define HAL_SRV_AFC_SIGNED      0
#define HAL_SRV_AFC_ADR         0xB00002B8

// Register APC
#define HAL_SRV_APC_REGISTER    (HAL_SRVREG_MAP->APC)
#define HAL_SRV_APC_LSB         0
#define HAL_SRV_APC_MSB         7
#define HAL_SRV_APC_MASK        0x000000FF
#define HAL_SRV_APC_TYPE        UByte
#define HAL_SRV_APC             HAL_SRV_APC_MASK
#define HAL_SRV_APC_READABLE    1
#define HAL_SRV_APC_WRITABLE    0
#define HAL_SRV_APC_SIGNED      0
#define HAL_SRV_APC_ADR         0xB00002BC

// Register DMODAC
#define HAL_SRV_DMODAC_REGISTER    (HAL_SRVREG_MAP->DMODAC)
#define HAL_SRV_DMODAC_LSB         0
#define HAL_SRV_DMODAC_MSB         9
#define HAL_SRV_DMODAC_MASK        0x000003FF
#define HAL_SRV_DMODAC_TYPE        SInt16
#define HAL_SRV_DMODAC             HAL_SRV_DMODAC_MASK
#define HAL_SRV_DMODAC_READABLE    1
#define HAL_SRV_DMODAC_WRITABLE    1
#define HAL_SRV_DMODAC_SIGNED      1
#define HAL_SRV_DMODAC_ADR         0xB00002C0

// Register SRCIN
#define HAL_SRV_BLANK_REGISTER    (HAL_SRVREG_MAP->SRCIN)
#define HAL_SRV_BLANK_LSB         7
#define HAL_SRV_BLANK_MSB         7
#define HAL_SRV_BLANK_MASK        0x00000080
#define HAL_SRV_BLANK_TYPE        halSRV_BLANK_t
#define HAL_SRV_BLANK             HAL_SRV_BLANK_MASK
#define HAL_SRV_BLANK_READABLE    1
#define HAL_SRV_BLANK_WRITABLE    0
#define HAL_SRV_BLANK_SIGNED      0
#define HAL_SRV_BLANK_ADR         0xB00002C4

#define HAL_SRV_STALL_REGISTER    (HAL_SRVREG_MAP->SRCIN)
#define HAL_SRV_STALL_LSB         6
#define HAL_SRV_STALL_MSB         6
#define HAL_SRV_STALL_MASK        0x00000040
#define HAL_SRV_STALL_TYPE        halSRV_STALL_t
#define HAL_SRV_STALL             HAL_SRV_STALL_MASK
#define HAL_SRV_STALL_READABLE    1
#define HAL_SRV_STALL_WRITABLE    0
#define HAL_SRV_STALL_SIGNED      0
#define HAL_SRV_STALL_ADR         0xB00002C4

#define HAL_SRV_FKDIR_REGISTER    (HAL_SRVREG_MAP->SRCIN)
#define HAL_SRV_FKDIR_LSB         5
#define HAL_SRV_FKDIR_MSB         5
#define HAL_SRV_FKDIR_MASK        0x00000020
#define HAL_SRV_FKDIR_TYPE        halSRV_FKDIR_t
#define HAL_SRV_FKDIR             HAL_SRV_FKDIR_MASK
#define HAL_SRV_FKDIR_READABLE    1
#define HAL_SRV_FKDIR_WRITABLE    0
#define HAL_SRV_FKDIR_SIGNED      0
#define HAL_SRV_FKDIR_ADR         0xB00002C4

#define HAL_SRV_FSRCH_REGISTER    (HAL_SRVREG_MAP->SRCIN)
#define HAL_SRV_FSRCH_LSB         4
#define HAL_SRV_FSRCH_MSB         4
#define HAL_SRV_FSRCH_MASK        0x00000010
#define HAL_SRV_FSRCH_TYPE        Bool
#define HAL_SRV_FSRCH             HAL_SRV_FSRCH_MASK
#define HAL_SRV_FSRCH_READABLE    1
#define HAL_SRV_FSRCH_WRITABLE    0
#define HAL_SRV_FSRCH_SIGNED      0
#define HAL_SRV_FSRCH_ADR         0xB00002C4

#define HAL_SRV_DVDLAND_REGISTER    (HAL_SRVREG_MAP->SRCIN)
#define HAL_SRV_DVDLAND_LSB         3
#define HAL_SRV_DVDLAND_MSB         3
#define HAL_SRV_DVDLAND_MASK        0x00000008
#define HAL_SRV_DVDLAND_TYPE        Bool
#define HAL_SRV_DVDLAND             HAL_SRV_DVDLAND_MASK
#define HAL_SRV_DVDLAND_READABLE    1
#define HAL_SRV_DVDLAND_WRITABLE    0
#define HAL_SRV_DVDLAND_SIGNED      0
#define HAL_SRV_DVDLAND_ADR         0xB00002C4

// Register DLCNT

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_DLCNT_REGISTER    (HAL_SRVREG_MAP->DLCNT)
#define HAL_SRV_DLCNT_LSB         0
#define HAL_SRV_DLCNT_MSB         8
#define HAL_SRV_DLCNT_MASK        0x000001FF
#define HAL_SRV_DLCNT_TYPE        UInt16
#define HAL_SRV_DLCNT             HAL_SRV_DLCNT_MASK
#define HAL_SRV_DLCNT_READABLE    1
#define HAL_SRV_DLCNT_WRITABLE    0
#define HAL_SRV_DLCNT_SIGNED      0
#define HAL_SRV_DLCNT_ADR         0xB00002C8

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_DLCNT_REGISTER    (HAL_SRVREG_MAP->DLCNT)
#define HAL_SRV_DLCNT_LSB         0
#define HAL_SRV_DLCNT_MSB         13
#define HAL_SRV_DLCNT_MASK        0x00003FFF
#define HAL_SRV_DLCNT_TYPE        UInt16
#define HAL_SRV_DLCNT             HAL_SRV_DLCNT_MASK
#define HAL_SRV_DLCNT_READABLE    1
#define HAL_SRV_DLCNT_WRITABLE    0
#define HAL_SRV_DLCNT_SIGNED      0
#define HAL_SRV_DLCNT_ADR         0xB00002C8

#endif // #if (CHIP_REV >= 0xC0)

// Register SDLCNT

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_SDLCNT_REGISTER    (HAL_SRVREG_MAP->SDLCNT)
#define HAL_SRV_SDLCNT_LSB         0
#define HAL_SRV_SDLCNT_MSB         8
#define HAL_SRV_SDLCNT_MASK        0x000001FF
#define HAL_SRV_SDLCNT_TYPE        UInt16
#define HAL_SRV_SDLCNT             HAL_SRV_SDLCNT_MASK
#define HAL_SRV_SDLCNT_READABLE    1
#define HAL_SRV_SDLCNT_WRITABLE    0
#define HAL_SRV_SDLCNT_SIGNED      0
#define HAL_SRV_SDLCNT_ADR         0xB00002D0

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_SDLCNT_REGISTER    (HAL_SRVREG_MAP->SDLCNT)
#define HAL_SRV_SDLCNT_LSB         0
#define HAL_SRV_SDLCNT_MSB         13
#define HAL_SRV_SDLCNT_MASK        0x00003FFF
#define HAL_SRV_SDLCNT_TYPE        UInt16
#define HAL_SRV_SDLCNT             HAL_SRV_SDLCNT_MASK
#define HAL_SRV_SDLCNT_READABLE    1
#define HAL_SRV_SDLCNT_WRITABLE    0
#define HAL_SRV_SDLCNT_SIGNED      0
#define HAL_SRV_SDLCNT_ADR         0xB00002D0

#endif // #if (CHIP_REV >= 0xC0)

// Register DLCNTF
#define HAL_SRV_DBSY_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_DBSY_LSB         7
#define HAL_SRV_DBSY_MSB         7
#define HAL_SRV_DBSY_MASK        0x00000080
#define HAL_SRV_DBSY_TYPE        Bool
#define HAL_SRV_DBSY             HAL_SRV_DBSY_MASK
#define HAL_SRV_DBSY_READABLE    1
#define HAL_SRV_DBSY_WRITABLE    0
#define HAL_SRV_DBSY_SIGNED      0
#define HAL_SRV_DBSY_ADR         0xB00002D8

#define HAL_SRV_CTBSY_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_CTBSY_LSB         6
#define HAL_SRV_CTBSY_MSB         6
#define HAL_SRV_CTBSY_MASK        0x00000040
#define HAL_SRV_CTBSY_TYPE        Bool
#define HAL_SRV_CTBSY             HAL_SRV_CTBSY_MASK
#define HAL_SRV_CTBSY_READABLE    1
#define HAL_SRV_CTBSY_WRITABLE    0
#define HAL_SRV_CTBSY_SIGNED      0
#define HAL_SRV_CTBSY_ADR         0xB00002D8

#define HAL_SRV_HFLG_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_HFLG_LSB         4
#define HAL_SRV_HFLG_MSB         4
#define HAL_SRV_HFLG_MASK        0x00000010
#define HAL_SRV_HFLG_TYPE        Bool
#define HAL_SRV_HFLG             HAL_SRV_HFLG_MASK
#define HAL_SRV_HFLG_READABLE    1
#define HAL_SRV_HFLG_WRITABLE    0
#define HAL_SRV_HFLG_SIGNED      0
#define HAL_SRV_HFLG_ADR         0xB00002D8

#define HAL_SRV_FFLG_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_FFLG_LSB         3
#define HAL_SRV_FFLG_MSB         3
#define HAL_SRV_FFLG_MASK        0x00000008
#define HAL_SRV_FFLG_TYPE        Bool
#define HAL_SRV_FFLG             HAL_SRV_FFLG_MASK
#define HAL_SRV_FFLG_READABLE    1
#define HAL_SRV_FFLG_WRITABLE    0
#define HAL_SRV_FFLG_SIGNED      0
#define HAL_SRV_FFLG_ADR         0xB00002D8

#define HAL_SRV_BFLG_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_BFLG_LSB         2
#define HAL_SRV_BFLG_MSB         2
#define HAL_SRV_BFLG_MASK        0x00000004
#define HAL_SRV_BFLG_TYPE        Bool
#define HAL_SRV_BFLG             HAL_SRV_BFLG_MASK
#define HAL_SRV_BFLG_READABLE    1
#define HAL_SRV_BFLG_WRITABLE    0
#define HAL_SRV_BFLG_SIGNED      0
#define HAL_SRV_BFLG_ADR         0xB00002D8

#define HAL_SRV_FBSY_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_FBSY_LSB         1
#define HAL_SRV_FBSY_MSB         1
#define HAL_SRV_FBSY_MASK        0x00000002
#define HAL_SRV_FBSY_TYPE        Bool
#define HAL_SRV_FBSY             HAL_SRV_FBSY_MASK
#define HAL_SRV_FBSY_READABLE    1
#define HAL_SRV_FBSY_WRITABLE    0
#define HAL_SRV_FBSY_SIGNED      0
#define HAL_SRV_FBSY_ADR         0xB00002D8

#define HAL_SRV_BBSY_REGISTER    (HAL_SRVREG_MAP->DLCNTF)
#define HAL_SRV_BBSY_LSB         0
#define HAL_SRV_BBSY_MSB         0
#define HAL_SRV_BBSY_MASK        0x00000001
#define HAL_SRV_BBSY_TYPE        Bool
#define HAL_SRV_BBSY             HAL_SRV_BBSY_MASK
#define HAL_SRV_BBSY_READABLE    1
#define HAL_SRV_BBSY_WRITABLE    0
#define HAL_SRV_BBSY_SIGNED      0
#define HAL_SRV_BBSY_ADR         0xB00002D8

// Register SVSTATUS
#define HAL_SRV_INTSTP_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_INTSTP_LSB         15
#define HAL_SRV_INTSTP_MSB         15
#define HAL_SRV_INTSTP_MASK        0x00008000
#define HAL_SRV_INTSTP_TYPE        Bool
#define HAL_SRV_INTSTP             HAL_SRV_INTSTP_MASK
#define HAL_SRV_INTSTP_READABLE    1
#define HAL_SRV_INTSTP_WRITABLE    0
#define HAL_SRV_INTSTP_SIGNED      0
#define HAL_SRV_INTSTP_ADR         0xB00002E0

#define HAL_SRV_FBEND_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_FBEND_LSB         11
#define HAL_SRV_FBEND_MSB         11
#define HAL_SRV_FBEND_MASK        0x00000800
#define HAL_SRV_FBEND_TYPE        Bool
#define HAL_SRV_FBEND             HAL_SRV_FBEND_MASK
#define HAL_SRV_FBEND_READABLE    1
#define HAL_SRV_FBEND_WRITABLE    0
#define HAL_SRV_FBEND_SIGNED      0
#define HAL_SRV_FBEND_ADR         0xB00002E0

#define HAL_SRV_FGEND_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_FGEND_LSB         10
#define HAL_SRV_FGEND_MSB         10
#define HAL_SRV_FGEND_MASK        0x00000400
#define HAL_SRV_FGEND_TYPE        Bool
#define HAL_SRV_FGEND             HAL_SRV_FGEND_MASK
#define HAL_SRV_FGEND_READABLE    1
#define HAL_SRV_FGEND_WRITABLE    0
#define HAL_SRV_FGEND_SIGNED      0
#define HAL_SRV_FGEND_ADR         0xB00002E0

#define HAL_SRV_TGEND_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_TGEND_LSB         8
#define HAL_SRV_TGEND_MSB         8
#define HAL_SRV_TGEND_MASK        0x00000100
#define HAL_SRV_TGEND_TYPE        Bool
#define HAL_SRV_TGEND             HAL_SRV_TGEND_MASK
#define HAL_SRV_TGEND_READABLE    1
#define HAL_SRV_TGEND_WRITABLE    0
#define HAL_SRV_TGEND_SIGNED      0
#define HAL_SRV_TGEND_ADR         0xB00002E0

#define HAL_SRV_KICKDIR_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_KICKDIR_LSB         7
#define HAL_SRV_KICKDIR_MSB         7
#define HAL_SRV_KICKDIR_MASK        0x00000080
#define HAL_SRV_KICKDIR_TYPE        halSRV_KICKDIR_t
#define HAL_SRV_KICKDIR             HAL_SRV_KICKDIR_MASK
#define HAL_SRV_KICKDIR_READABLE    1
#define HAL_SRV_KICKDIR_WRITABLE    0
#define HAL_SRV_KICKDIR_SIGNED      0
#define HAL_SRV_KICKDIR_ADR         0xB00002E0

#define HAL_SRV_TRODET_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_TRODET_LSB         6
#define HAL_SRV_TRODET_MSB         6
#define HAL_SRV_TRODET_MASK        0x00000040
#define HAL_SRV_TRODET_TYPE        Bool
#define HAL_SRV_TRODET             HAL_SRV_TRODET_MASK
#define HAL_SRV_TRODET_READABLE    1
#define HAL_SRV_TRODET_WRITABLE    0
#define HAL_SRV_TRODET_SIGNED      0
#define HAL_SRV_TRODET_ADR         0xB00002E0

#define HAL_SRV_SLOW_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_SLOW_LSB         5
#define HAL_SRV_SLOW_MSB         5
#define HAL_SRV_SLOW_MASK        0x00000020
#define HAL_SRV_SLOW_TYPE        halSRV_SLOW_t
#define HAL_SRV_SLOW             HAL_SRV_SLOW_MASK
#define HAL_SRV_SLOW_READABLE    1
#define HAL_SRV_SLOW_WRITABLE    0
#define HAL_SRV_SLOW_SIGNED      0
#define HAL_SRV_SLOW_ADR         0xB00002E0

#define HAL_SRV_BRKR_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_BRKR_LSB         4
#define HAL_SRV_BRKR_MSB         4
#define HAL_SRV_BRKR_MASK        0x00000010
#define HAL_SRV_BRKR_TYPE        Bool
#define HAL_SRV_BRKR             HAL_SRV_BRKR_MASK
#define HAL_SRV_BRKR_READABLE    1
#define HAL_SRV_BRKR_WRITABLE    0
#define HAL_SRV_BRKR_SIGNED      0
#define HAL_SRV_BRKR_ADR         0xB00002E0

#define HAL_SRV_FOK_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_FOK_LSB         3
#define HAL_SRV_FOK_MSB         3
#define HAL_SRV_FOK_MASK        0x00000008
#define HAL_SRV_FOK_TYPE        Bool
#define HAL_SRV_FOK             HAL_SRV_FOK_MASK
#define HAL_SRV_FOK_READABLE    1
#define HAL_SRV_FOK_WRITABLE    0
#define HAL_SRV_FOK_SIGNED      0
#define HAL_SRV_FOK_ADR         0xB00002E0

#define HAL_SRV_SRCH_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_SRCH_LSB         1
#define HAL_SRV_SRCH_MSB         1
#define HAL_SRV_SRCH_MASK        0x00000002
#define HAL_SRV_SRCH_TYPE        Bool
#define HAL_SRV_SRCH             HAL_SRV_SRCH_MASK
#define HAL_SRV_SRCH_READABLE    1
#define HAL_SRV_SRCH_WRITABLE    0
#define HAL_SRV_SRCH_SIGNED      0
#define HAL_SRV_SRCH_ADR         0xB00002E0

#define HAL_SRV_CNT_REGISTER    (HAL_SRVREG_MAP->SVSTATUS)
#define HAL_SRV_CNT_LSB         0
#define HAL_SRV_CNT_MSB         0
#define HAL_SRV_CNT_MASK        0x00000001
#define HAL_SRV_CNT_TYPE        Bool
#define HAL_SRV_CNT             HAL_SRV_CNT_MASK
#define HAL_SRV_CNT_READABLE    1
#define HAL_SRV_CNT_WRITABLE    0
#define HAL_SRV_CNT_SIGNED      0
#define HAL_SRV_CNT_ADR         0xB00002E0

// Register SRCR
#define HAL_SRV_DATR_REGISTER    (HAL_SRVREG_MAP->SRCR)
#define HAL_SRV_DATR_LSB         0
#define HAL_SRV_DATR_MSB         15
#define HAL_SRV_DATR_MASK        0x0000FFFF
#define HAL_SRV_DATR_TYPE        UInt16
#define HAL_SRV_DATR             HAL_SRV_DATR_MASK
#define HAL_SRV_DATR_READABLE    1
#define HAL_SRV_DATR_WRITABLE    0
#define HAL_SRV_DATR_SIGNED      0
#define HAL_SRV_DATR_ADR         0xB00002E8

// Register CKICH
#define HAL_SRV_CKSURF_REGISTER    (HAL_SRVREG_MAP->CKICH)
#define HAL_SRV_CKSURF_LSB         6
#define HAL_SRV_CKSURF_MSB         6
#define HAL_SRV_CKSURF_MASK        0x00000040
#define HAL_SRV_CKSURF_TYPE        Bool
#define HAL_SRV_CKSURF             HAL_SRV_CKSURF_MASK
#define HAL_SRV_CKSURF_READABLE    1
#define HAL_SRV_CKSURF_WRITABLE    1
#define HAL_SRV_CKSURF_SIGNED      0
#define HAL_SRV_CKSURF_ADR         0xB0000300

#define HAL_SRV_CKRESEK_REGISTER    (HAL_SRVREG_MAP->CKICH)
#define HAL_SRV_CKRESEK_LSB         5
#define HAL_SRV_CKRESEK_MSB         5
#define HAL_SRV_CKRESEK_MASK        0x00000020
#define HAL_SRV_CKRESEK_TYPE        Bool
#define HAL_SRV_CKRESEK             HAL_SRV_CKRESEK_MASK
#define HAL_SRV_CKRESEK_READABLE    1
#define HAL_SRV_CKRESEK_WRITABLE    1
#define HAL_SRV_CKRESEK_SIGNED      0
#define HAL_SRV_CKRESEK_ADR         0xB0000300

#define HAL_SRV_CKFDC_REGISTER    (HAL_SRVREG_MAP->CKICH)
#define HAL_SRV_CKFDC_LSB         2
#define HAL_SRV_CKFDC_MSB         2
#define HAL_SRV_CKFDC_MASK        0x00000004
#define HAL_SRV_CKFDC_TYPE        Bool
#define HAL_SRV_CKFDC             HAL_SRV_CKFDC_MASK
#define HAL_SRV_CKFDC_READABLE    1
#define HAL_SRV_CKFDC_WRITABLE    1
#define HAL_SRV_CKFDC_SIGNED      0
#define HAL_SRV_CKFDC_ADR         0xB0000300

#define HAL_SRV_CKTGC_REGISTER    (HAL_SRVREG_MAP->CKICH)
#define HAL_SRV_CKTGC_LSB         1
#define HAL_SRV_CKTGC_MSB         1
#define HAL_SRV_CKTGC_MASK        0x00000002
#define HAL_SRV_CKTGC_TYPE        Bool
#define HAL_SRV_CKTGC             HAL_SRV_CKTGC_MASK
#define HAL_SRV_CKTGC_READABLE    1
#define HAL_SRV_CKTGC_WRITABLE    1
#define HAL_SRV_CKTGC_SIGNED      0
#define HAL_SRV_CKTGC_ADR         0xB0000300

#define HAL_SRV_CKBF_REGISTER    (HAL_SRVREG_MAP->CKICH)
#define HAL_SRV_CKBF_LSB         0
#define HAL_SRV_CKBF_MSB         0
#define HAL_SRV_CKBF_MASK        0x00000001
#define HAL_SRV_CKBF_TYPE        halSRV_CKBF_t
#define HAL_SRV_CKBF             HAL_SRV_CKBF_MASK
#define HAL_SRV_CKBF_READABLE    1
#define HAL_SRV_CKBF_WRITABLE    1
#define HAL_SRV_CKBF_SIGNED      0
#define HAL_SRV_CKBF_ADR         0xB0000300

// Register CKICL
#define HAL_SRV_CKICT_REGISTER    (HAL_SRVREG_MAP->CKICL)
#define HAL_SRV_CKICT_LSB         4
#define HAL_SRV_CKICT_MSB         5
#define HAL_SRV_CKICT_MASK        0x00000030
#define HAL_SRV_CKICT_TYPE        halSRV_CKICT_t
#define HAL_SRV_CKICT             HAL_SRV_CKICT_MASK
#define HAL_SRV_CKICT_READABLE    1
#define HAL_SRV_CKICT_WRITABLE    1
#define HAL_SRV_CKICT_SIGNED      0
#define HAL_SRV_CKICT_ADR         0xB0000304

#define HAL_SRV_CKICC_REGISTER    (HAL_SRVREG_MAP->CKICL)
#define HAL_SRV_CKICC_LSB         0
#define HAL_SRV_CKICC_MSB         3
#define HAL_SRV_CKICC_MASK        0x0000000F
#define HAL_SRV_CKICC_TYPE        UByte
#define HAL_SRV_CKICC             HAL_SRV_CKICC_MASK
#define HAL_SRV_CKICC_READABLE    1
#define HAL_SRV_CKICC_WRITABLE    1
#define HAL_SRV_CKICC_SIGNED      0
#define HAL_SRV_CKICC_ADR         0xB0000304

// Register DEADZ
#define HAL_SRV_DZON_REGISTER    (HAL_SRVREG_MAP->DEADZ)
#define HAL_SRV_DZON_LSB         7
#define HAL_SRV_DZON_MSB         7
#define HAL_SRV_DZON_MASK        0x00000080
#define HAL_SRV_DZON_TYPE        Bool
#define HAL_SRV_DZON             HAL_SRV_DZON_MASK
#define HAL_SRV_DZON_READABLE    1
#define HAL_SRV_DZON_WRITABLE    1
#define HAL_SRV_DZON_SIGNED      0
#define HAL_SRV_DZON_ADR         0xB0000308

#define HAL_SRV_DZ_REGISTER    (HAL_SRVREG_MAP->DEADZ)
#define HAL_SRV_DZ_LSB         0
#define HAL_SRV_DZ_MSB         6
#define HAL_SRV_DZ_MASK        0x0000007F
#define HAL_SRV_DZ_TYPE        UByte
#define HAL_SRV_DZ             HAL_SRV_DZ_MASK
#define HAL_SRV_DZ_READABLE    1
#define HAL_SRV_DZ_WRITABLE    1
#define HAL_SRV_DZ_SIGNED      0
#define HAL_SRV_DZ_ADR         0xB0000308

// Register WGHOLDC
#define HAL_SRV_WGP_REGISTER    (HAL_SRVREG_MAP->WGHOLDC)
#define HAL_SRV_WGP_LSB         4
#define HAL_SRV_WGP_MSB         7
#define HAL_SRV_WGP_MASK        0x000000F0
#define HAL_SRV_WGP_TYPE        halSRV_WriteGatePosEdgeDCHold_t
#define HAL_SRV_WGP             HAL_SRV_WGP_MASK
#define HAL_SRV_WGP_READABLE    1
#define HAL_SRV_WGP_WRITABLE    1
#define HAL_SRV_WGP_SIGNED      0
#define HAL_SRV_WGP_ADR         0xB000030C

#define HAL_SRV_WGN_REGISTER    (HAL_SRVREG_MAP->WGHOLDC)
#define HAL_SRV_WGN_LSB         0
#define HAL_SRV_WGN_MSB         3
#define HAL_SRV_WGN_MASK        0x0000000F
#define HAL_SRV_WGN_TYPE        halSRV_WriteGateNegEdgeDCHold_t
#define HAL_SRV_WGN             HAL_SRV_WGN_MASK
#define HAL_SRV_WGN_READABLE    1
#define HAL_SRV_WGN_WRITABLE    1
#define HAL_SRV_WGN_SIGNED      0
#define HAL_SRV_WGN_ADR         0xB000030C

// Register DLTHR

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_DLTHR_REGISTER    (HAL_SRVREG_MAP->DLTHR)
#define HAL_SRV_DLTHR_LSB         0
#define HAL_SRV_DLTHR_MSB         7
#define HAL_SRV_DLTHR_MASK        0x000000FF
#define HAL_SRV_DLTHR_TYPE        UByte
#define HAL_SRV_DLTHR             HAL_SRV_DLTHR_MASK
#define HAL_SRV_DLTHR_READABLE    1
#define HAL_SRV_DLTHR_WRITABLE    1
#define HAL_SRV_DLTHR_SIGNED      0
#define HAL_SRV_DLTHR_ADR         0xB0000310

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_DLTHR_REGISTER    (HAL_SRVREG_MAP->DLTHR)
#define HAL_SRV_DLTHR_LSB         0
#define HAL_SRV_DLTHR_MSB         13
#define HAL_SRV_DLTHR_MASK        0x00003FFF
#define HAL_SRV_DLTHR_TYPE        UInt16
#define HAL_SRV_DLTHR             HAL_SRV_DLTHR_MASK
#define HAL_SRV_DLTHR_READABLE    1
#define HAL_SRV_DLTHR_WRITABLE    1
#define HAL_SRV_DLTHR_SIGNED      0
#define HAL_SRV_DLTHR_ADR         0xB0000310

#endif // #if (CHIP_REV >= 0xC0)

// Register DLPRG
#define HAL_SRV_MRWDFS_REGISTER    (HAL_SRVREG_MAP->DLPRG)
#define HAL_SRV_MRWDFS_LSB         6
#define HAL_SRV_MRWDFS_MSB         6
#define HAL_SRV_MRWDFS_MASK        0x00000040
#define HAL_SRV_MRWDFS_TYPE        halSRV_MRWDFS_t
#define HAL_SRV_MRWDFS             HAL_SRV_MRWDFS_MASK
#define HAL_SRV_MRWDFS_READABLE    1
#define HAL_SRV_MRWDFS_WRITABLE    1
#define HAL_SRV_MRWDFS_SIGNED      0
#define HAL_SRV_MRWDFS_ADR         0xB0000314

#define HAL_SRV_DSECSEL_REGISTER    (HAL_SRVREG_MAP->DLPRG)
#define HAL_SRV_DSECSEL_LSB         5
#define HAL_SRV_DSECSEL_MSB         5
#define HAL_SRV_DSECSEL_MASK        0x00000020
#define HAL_SRV_DSECSEL_TYPE        halSRV_DSECSEL_t
#define HAL_SRV_DSECSEL             HAL_SRV_DSECSEL_MASK
#define HAL_SRV_DSECSEL_READABLE    1
#define HAL_SRV_DSECSEL_WRITABLE    1
#define HAL_SRV_DSECSEL_SIGNED      0
#define HAL_SRV_DSECSEL_ADR         0xB0000314

#define HAL_SRV_ROMDISC_REGISTER    (HAL_SRVREG_MAP->DLPRG)
#define HAL_SRV_ROMDISC_LSB         4
#define HAL_SRV_ROMDISC_MSB         4
#define HAL_SRV_ROMDISC_MASK        0x00000010
#define HAL_SRV_ROMDISC_TYPE        halSRV_ROMDISC_t
#define HAL_SRV_ROMDISC             HAL_SRV_ROMDISC_MASK
#define HAL_SRV_ROMDISC_READABLE    1
#define HAL_SRV_ROMDISC_WRITABLE    1
#define HAL_SRV_ROMDISC_SIGNED      0
#define HAL_SRV_ROMDISC_ADR         0xB0000314

#define HAL_SRV_DIVSEL_REGISTER    (HAL_SRVREG_MAP->DLPRG)
#define HAL_SRV_DIVSEL_LSB         0
#define HAL_SRV_DIVSEL_MSB         3
#define HAL_SRV_DIVSEL_MASK        0x0000000F
#define HAL_SRV_DIVSEL_TYPE        halSRV_DIVSEL_t
#define HAL_SRV_DIVSEL             HAL_SRV_DIVSEL_MASK
#define HAL_SRV_DIVSEL_READABLE    1
#define HAL_SRV_DIVSEL_WRITABLE    1
#define HAL_SRV_DIVSEL_SIGNED      0
#define HAL_SRV_DIVSEL_ADR         0xB0000314

#if (CHIP_REV >= 0xC0)

// Register SLEWRATE
#define HAL_SRV_SHUNWIND_REGISTER    (HAL_SRVREG_MAP->SLEWRATE)
#define HAL_SRV_SHUNWIND_LSB         16
#define HAL_SRV_SHUNWIND_MSB         23
#define HAL_SRV_SHUNWIND_MASK        0x00FF0000
#define HAL_SRV_SHUNWIND_TYPE        UByte
#define HAL_SRV_SHUNWIND             HAL_SRV_SHUNWIND_MASK
#define HAL_SRV_SHUNWIND_READABLE    1
#define HAL_SRV_SHUNWIND_WRITABLE    1
#define HAL_SRV_SHUNWIND_SIGNED      0
#define HAL_SRV_SHUNWIND_ADR         0xB0000318

#define HAL_SRV_SLEWRATE_EN_REGISTER    (HAL_SRVREG_MAP->SLEWRATE)
#define HAL_SRV_SLEWRATE_EN_LSB         8
#define HAL_SRV_SLEWRATE_EN_MSB         8
#define HAL_SRV_SLEWRATE_EN_MASK        0x00000100
#define HAL_SRV_SLEWRATE_EN_TYPE        Bool
#define HAL_SRV_SLEWRATE_EN             HAL_SRV_SLEWRATE_EN_MASK
#define HAL_SRV_SLEWRATE_EN_READABLE    1
#define HAL_SRV_SLEWRATE_EN_WRITABLE    1
#define HAL_SRV_SLEWRATE_EN_SIGNED      0
#define HAL_SRV_SLEWRATE_EN_ADR         0xB0000318

#define HAL_SRV_FESLEW_REGISTER    (HAL_SRVREG_MAP->SLEWRATE)
#define HAL_SRV_FESLEW_LSB         0
#define HAL_SRV_FESLEW_MSB         7
#define HAL_SRV_FESLEW_MASK        0x000000FF
#define HAL_SRV_FESLEW_TYPE        UByte
#define HAL_SRV_FESLEW             HAL_SRV_FESLEW_MASK
#define HAL_SRV_FESLEW_READABLE    1
#define HAL_SRV_FESLEW_WRITABLE    1
#define HAL_SRV_FESLEW_SIGNED      0
#define HAL_SRV_FESLEW_ADR         0xB0000318

#endif // #if (CHIP_REV >= 0xC0)

// Register PNCBKR
#define HAL_SRV_PANICINV_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_PANICINV_LSB         7
#define HAL_SRV_PANICINV_MSB         7
#define HAL_SRV_PANICINV_MASK        0x00000080
#define HAL_SRV_PANICINV_TYPE        Bool
#define HAL_SRV_PANICINV             HAL_SRV_PANICINV_MASK
#define HAL_SRV_PANICINV_READABLE    1
#define HAL_SRV_PANICINV_WRITABLE    1
#define HAL_SRV_PANICINV_SIGNED      0
#define HAL_SRV_PANICINV_ADR         0xB000031C

#define HAL_SRV_TRERRBKEN_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_TRERRBKEN_LSB         6
#define HAL_SRV_TRERRBKEN_MSB         6
#define HAL_SRV_TRERRBKEN_MASK        0x00000040
#define HAL_SRV_TRERRBKEN_TYPE        halSRV_TRERRBKEN_t
#define HAL_SRV_TRERRBKEN             HAL_SRV_TRERRBKEN_MASK
#define HAL_SRV_TRERRBKEN_READABLE    1
#define HAL_SRV_TRERRBKEN_WRITABLE    1
#define HAL_SRV_TRERRBKEN_SIGNED      0
#define HAL_SRV_TRERRBKEN_ADR         0xB000031C

#define HAL_SRV_BLKEN_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_BLKEN_LSB         5
#define HAL_SRV_BLKEN_MSB         5
#define HAL_SRV_BLKEN_MASK        0x00000020
#define HAL_SRV_BLKEN_TYPE        Bool
#define HAL_SRV_BLKEN             HAL_SRV_BLKEN_MASK
#define HAL_SRV_BLKEN_READABLE    1
#define HAL_SRV_BLKEN_WRITABLE    1
#define HAL_SRV_BLKEN_SIGNED      0
#define HAL_SRV_BLKEN_ADR         0xB000031C

#define HAL_SRV_BLKRECD_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_BLKRECD_LSB         4
#define HAL_SRV_BLKRECD_MSB         4
#define HAL_SRV_BLKRECD_MASK        0x00000010
#define HAL_SRV_BLKRECD_TYPE        Bool
#define HAL_SRV_BLKRECD             HAL_SRV_BLKRECD_MASK
#define HAL_SRV_BLKRECD_READABLE    1
#define HAL_SRV_BLKRECD_WRITABLE    1
#define HAL_SRV_BLKRECD_SIGNED      0
#define HAL_SRV_BLKRECD_ADR         0xB000031C

#define HAL_SRV_FEEN_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_FEEN_LSB         3
#define HAL_SRV_FEEN_MSB         3
#define HAL_SRV_FEEN_MASK        0x00000008
#define HAL_SRV_FEEN_TYPE        Bool
#define HAL_SRV_FEEN             HAL_SRV_FEEN_MASK
#define HAL_SRV_FEEN_READABLE    1
#define HAL_SRV_FEEN_WRITABLE    1
#define HAL_SRV_FEEN_SIGNED      0
#define HAL_SRV_FEEN_ADR         0xB000031C

#define HAL_SRV_PANICEN_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_PANICEN_LSB         2
#define HAL_SRV_PANICEN_MSB         2
#define HAL_SRV_PANICEN_MASK        0x00000004
#define HAL_SRV_PANICEN_TYPE        Bool
#define HAL_SRV_PANICEN             HAL_SRV_PANICEN_MASK
#define HAL_SRV_PANICEN_READABLE    1
#define HAL_SRV_PANICEN_WRITABLE    1
#define HAL_SRV_PANICEN_SIGNED      0
#define HAL_SRV_PANICEN_ADR         0xB000031C

#define HAL_SRV_FOKEN_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_FOKEN_LSB         1
#define HAL_SRV_FOKEN_MSB         1
#define HAL_SRV_FOKEN_MASK        0x00000002
#define HAL_SRV_FOKEN_TYPE        Bool
#define HAL_SRV_FOKEN             HAL_SRV_FOKEN_MASK
#define HAL_SRV_FOKEN_READABLE    1
#define HAL_SRV_FOKEN_WRITABLE    1
#define HAL_SRV_FOKEN_SIGNED      0
#define HAL_SRV_FOKEN_ADR         0xB000031C

#define HAL_SRV_SHCEN_REGISTER    (HAL_SRVREG_MAP->PNCBKR)
#define HAL_SRV_SHCEN_LSB         0
#define HAL_SRV_SHCEN_MSB         0
#define HAL_SRV_SHCEN_MASK        0x00000001
#define HAL_SRV_SHCEN_TYPE        Bool
#define HAL_SRV_SHCEN             HAL_SRV_SHCEN_MASK
#define HAL_SRV_SHCEN_READABLE    1
#define HAL_SRV_SHCEN_WRITABLE    1
#define HAL_SRV_SHCEN_SIGNED      0
#define HAL_SRV_SHCEN_ADR         0xB000031C

// Register TXSPEED

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_TXSPDUP_REGISTER    (HAL_SRVREG_MAP->TXSPEED)
#define HAL_SRV_TXSPDUP_LSB         8
#define HAL_SRV_TXSPDUP_MSB         8
#define HAL_SRV_TXSPDUP_MASK        0x00000100
#define HAL_SRV_TXSPDUP_TYPE        Bool
#define HAL_SRV_TXSPDUP             HAL_SRV_TXSPDUP_MASK
#define HAL_SRV_TXSPDUP_READABLE    1
#define HAL_SRV_TXSPDUP_WRITABLE    0
#define HAL_SRV_TXSPDUP_SIGNED      0
#define HAL_SRV_TXSPDUP_ADR         0xB0000320

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_TXSPEED_REGISTER    (HAL_SRVREG_MAP->TXSPEED)
#define HAL_SRV_TXSPEED_LSB         0
#define HAL_SRV_TXSPEED_MSB         7
#define HAL_SRV_TXSPEED_MASK        0x000000FF
#define HAL_SRV_TXSPEED_TYPE        UByte
#define HAL_SRV_TXSPEED             HAL_SRV_TXSPEED_MASK
#define HAL_SRV_TXSPEED_READABLE    1
#define HAL_SRV_TXSPEED_WRITABLE    0
#define HAL_SRV_TXSPEED_SIGNED      0
#define HAL_SRV_TXSPEED_ADR         0xB0000320

// Register DFCTBL
#define HAL_SRV_DBL_REGISTER    (HAL_SRVREG_MAP->DFCTBL)
#define HAL_SRV_DBL_LSB         0
#define HAL_SRV_DBL_MSB         6
#define HAL_SRV_DBL_MASK        0x0000007F
#define HAL_SRV_DBL_TYPE        UByte
#define HAL_SRV_DBL             HAL_SRV_DBL_MASK
#define HAL_SRV_DBL_READABLE    1
#define HAL_SRV_DBL_WRITABLE    1
#define HAL_SRV_DBL_SIGNED      0
#define HAL_SRV_DBL_ADR         0xB0000330

// Register DFCTBT
#define HAL_SRV_DBTRES_REGISTER    (HAL_SRVREG_MAP->DFCTBT)
#define HAL_SRV_DBTRES_LSB         7
#define HAL_SRV_DBTRES_MSB         7
#define HAL_SRV_DBTRES_MASK        0x00000080
#define HAL_SRV_DBTRES_TYPE        halSRV_DBTRES_t
#define HAL_SRV_DBTRES             HAL_SRV_DBTRES_MASK
#define HAL_SRV_DBTRES_READABLE    1
#define HAL_SRV_DBTRES_WRITABLE    1
#define HAL_SRV_DBTRES_SIGNED      0
#define HAL_SRV_DBTRES_ADR         0xB0000334

#define HAL_SRV_DBT_REGISTER    (HAL_SRVREG_MAP->DFCTBT)
#define HAL_SRV_DBT_LSB         0
#define HAL_SRV_DBT_MSB         6
#define HAL_SRV_DBT_MASK        0x0000007F
#define HAL_SRV_DBT_TYPE        UByte
#define HAL_SRV_DBT             HAL_SRV_DBT_MASK
#define HAL_SRV_DBT_READABLE    1
#define HAL_SRV_DBT_WRITABLE    1
#define HAL_SRV_DBT_SIGNED      0
#define HAL_SRV_DBT_ADR         0xB0000334

// Register FBA
#define HAL_SRV_FBAT_REGISTER    (HAL_SRVREG_MAP->FBA)
#define HAL_SRV_FBAT_LSB         5
#define HAL_SRV_FBAT_MSB         5
#define HAL_SRV_FBAT_MASK        0x00000020
#define HAL_SRV_FBAT_TYPE        Bool
#define HAL_SRV_FBAT             HAL_SRV_FBAT_MASK
#define HAL_SRV_FBAT_READABLE    1
#define HAL_SRV_FBAT_WRITABLE    1
#define HAL_SRV_FBAT_SIGNED      0
#define HAL_SRV_FBAT_ADR         0xB0000340

#define HAL_SRV_FBST_REGISTER    (HAL_SRVREG_MAP->FBA)
#define HAL_SRV_FBST_LSB         4
#define HAL_SRV_FBST_MSB         4
#define HAL_SRV_FBST_MASK        0x00000010
#define HAL_SRV_FBST_TYPE        Bool
#define HAL_SRV_FBST             HAL_SRV_FBST_MASK
#define HAL_SRV_FBST_READABLE    1
#define HAL_SRV_FBST_WRITABLE    1
#define HAL_SRV_FBST_SIGNED      0
#define HAL_SRV_FBST_ADR         0xB0000340

#define HAL_SRV_FBW_REGISTER    (HAL_SRVREG_MAP->FBA)
#define HAL_SRV_FBW_LSB         0
#define HAL_SRV_FBW_MSB         1
#define HAL_SRV_FBW_MASK        0x00000003
#define HAL_SRV_FBW_TYPE        halSRV_FBW_t
#define HAL_SRV_FBW             HAL_SRV_FBW_MASK
#define HAL_SRV_FBW_READABLE    1
#define HAL_SRV_FBW_WRITABLE    1
#define HAL_SRV_FBW_SIGNED      0
#define HAL_SRV_FBW_ADR         0xB0000340

// Register FGA
#define HAL_SRV_FGAT_REGISTER    (HAL_SRVREG_MAP->FGA)
#define HAL_SRV_FGAT_LSB         5
#define HAL_SRV_FGAT_MSB         5
#define HAL_SRV_FGAT_MASK        0x00000020
#define HAL_SRV_FGAT_TYPE        Bool
#define HAL_SRV_FGAT             HAL_SRV_FGAT_MASK
#define HAL_SRV_FGAT_READABLE    1
#define HAL_SRV_FGAT_WRITABLE    1
#define HAL_SRV_FGAT_SIGNED      0
#define HAL_SRV_FGAT_ADR         0xB0000344

#define HAL_SRV_FGST_REGISTER    (HAL_SRVREG_MAP->FGA)
#define HAL_SRV_FGST_LSB         4
#define HAL_SRV_FGST_MSB         4
#define HAL_SRV_FGST_MASK        0x00000010
#define HAL_SRV_FGST_TYPE        Bool
#define HAL_SRV_FGST             HAL_SRV_FGST_MASK
#define HAL_SRV_FGST_READABLE    1
#define HAL_SRV_FGST_WRITABLE    1
#define HAL_SRV_FGST_SIGNED      0
#define HAL_SRV_FGST_ADR         0xB0000344

#define HAL_SRV_FGW_REGISTER    (HAL_SRVREG_MAP->FGA)
#define HAL_SRV_FGW_LSB         0
#define HAL_SRV_FGW_MSB         1
#define HAL_SRV_FGW_MASK        0x00000003
#define HAL_SRV_FGW_TYPE        halSRV_FGW_t
#define HAL_SRV_FGW             HAL_SRV_FGW_MASK
#define HAL_SRV_FGW_READABLE    1
#define HAL_SRV_FGW_WRITABLE    1
#define HAL_SRV_FGW_SIGNED      0
#define HAL_SRV_FGW_ADR         0xB0000344

// Register TGA
#define HAL_SRV_TGAT_REGISTER    (HAL_SRVREG_MAP->TGA)
#define HAL_SRV_TGAT_LSB         5
#define HAL_SRV_TGAT_MSB         5
#define HAL_SRV_TGAT_MASK        0x00000020
#define HAL_SRV_TGAT_TYPE        Bool
#define HAL_SRV_TGAT             HAL_SRV_TGAT_MASK
#define HAL_SRV_TGAT_READABLE    1
#define HAL_SRV_TGAT_WRITABLE    1
#define HAL_SRV_TGAT_SIGNED      0
#define HAL_SRV_TGAT_ADR         0xB000034C

#define HAL_SRV_TGST_REGISTER    (HAL_SRVREG_MAP->TGA)
#define HAL_SRV_TGST_LSB         4
#define HAL_SRV_TGST_MSB         4
#define HAL_SRV_TGST_MASK        0x00000010
#define HAL_SRV_TGST_TYPE        Bool
#define HAL_SRV_TGST             HAL_SRV_TGST_MASK
#define HAL_SRV_TGST_READABLE    1
#define HAL_SRV_TGST_WRITABLE    1
#define HAL_SRV_TGST_SIGNED      0
#define HAL_SRV_TGST_ADR         0xB000034C

#define HAL_SRV_TGW_REGISTER    (HAL_SRVREG_MAP->TGA)
#define HAL_SRV_TGW_LSB         0
#define HAL_SRV_TGW_MSB         1
#define HAL_SRV_TGW_MASK        0x00000003
#define HAL_SRV_TGW_TYPE        halSRV_TGW_t
#define HAL_SRV_TGW             HAL_SRV_TGW_MASK
#define HAL_SRV_TGW_READABLE    1
#define HAL_SRV_TGW_WRITABLE    1
#define HAL_SRV_TGW_SIGNED      0
#define HAL_SRV_TGW_ADR         0xB000034C

// Register FTBAST
#define HAL_SRV_FBAVS_REGISTER    (HAL_SRVREG_MAP->FTBAST)
#define HAL_SRV_FBAVS_LSB         7
#define HAL_SRV_FBAVS_MSB         7
#define HAL_SRV_FBAVS_MASK        0x00000080
#define HAL_SRV_FBAVS_TYPE        halSRV_FBAVS_t
#define HAL_SRV_FBAVS             HAL_SRV_FBAVS_MASK
#define HAL_SRV_FBAVS_READABLE    1
#define HAL_SRV_FBAVS_WRITABLE    1
#define HAL_SRV_FBAVS_SIGNED      0
#define HAL_SRV_FBAVS_ADR         0xB0000350

#define HAL_SRV_FBASE_REGISTER    (HAL_SRVREG_MAP->FTBAST)
#define HAL_SRV_FBASE_LSB         6
#define HAL_SRV_FBASE_MSB         6
#define HAL_SRV_FBASE_MASK        0x00000040
#define HAL_SRV_FBASE_TYPE        Bool
#define HAL_SRV_FBASE             HAL_SRV_FBASE_MASK
#define HAL_SRV_FBASE_READABLE    1
#define HAL_SRV_FBASE_WRITABLE    1
#define HAL_SRV_FBASE_SIGNED      0
#define HAL_SRV_FBASE_ADR         0xB0000350

#define HAL_SRV_FQWDSL_REGISTER    (HAL_SRVREG_MAP->FTBAST)
#define HAL_SRV_FQWDSL_LSB         3
#define HAL_SRV_FQWDSL_MSB         3
#define HAL_SRV_FQWDSL_MASK        0x00000008
#define HAL_SRV_FQWDSL_TYPE        halSRV_FQWDSL_t
#define HAL_SRV_FQWDSL             HAL_SRV_FQWDSL_MASK
#define HAL_SRV_FQWDSL_READABLE    1
#define HAL_SRV_FQWDSL_WRITABLE    1
#define HAL_SRV_FQWDSL_SIGNED      0
#define HAL_SRV_FQWDSL_ADR         0xB0000350

#define HAL_SRV_FBAVS4_REGISTER    (HAL_SRVREG_MAP->FTBAST)
#define HAL_SRV_FBAVS4_LSB         2
#define HAL_SRV_FBAVS4_MSB         2
#define HAL_SRV_FBAVS4_MASK        0x00000004
#define HAL_SRV_FBAVS4_TYPE        halSRV_FBAVS4_t
#define HAL_SRV_FBAVS4             HAL_SRV_FBAVS4_MASK
#define HAL_SRV_FBAVS4_READABLE    1
#define HAL_SRV_FBAVS4_WRITABLE    1
#define HAL_SRV_FBAVS4_SIGNED      0
#define HAL_SRV_FBAVS4_ADR         0xB0000350

#define HAL_SRV_FQWDS4_REGISTER    (HAL_SRVREG_MAP->FTBAST)
#define HAL_SRV_FQWDS4_LSB         1
#define HAL_SRV_FQWDS4_MSB         1
#define HAL_SRV_FQWDS4_MASK        0x00000002
#define HAL_SRV_FQWDS4_TYPE        halSRV_FQWDS4_t
#define HAL_SRV_FQWDS4             HAL_SRV_FQWDS4_MASK
#define HAL_SRV_FQWDS4_READABLE    1
#define HAL_SRV_FQWDS4_WRITABLE    1
#define HAL_SRV_FQWDS4_SIGNED      0
#define HAL_SRV_FQWDS4_ADR         0xB0000350

#define HAL_SRV_BSRPSEL_REGISTER    (HAL_SRVREG_MAP->FTBAST)
#define HAL_SRV_BSRPSEL_LSB         0
#define HAL_SRV_BSRPSEL_MSB         0
#define HAL_SRV_BSRPSEL_MASK        0x00000001
#define HAL_SRV_BSRPSEL_TYPE        halSRV_BSRPSEL_t
#define HAL_SRV_BSRPSEL             HAL_SRV_BSRPSEL_MASK
#define HAL_SRV_BSRPSEL_READABLE    1
#define HAL_SRV_BSRPSEL_WRITABLE    1
#define HAL_SRV_BSRPSEL_SIGNED      0
#define HAL_SRV_BSRPSEL_ADR         0xB0000350

// Register FGASET
#define HAL_SRV_FGAVS_REGISTER    (HAL_SRVREG_MAP->FGASET)
#define HAL_SRV_FGAVS_LSB         6
#define HAL_SRV_FGAVS_MSB         6
#define HAL_SRV_FGAVS_MASK        0x00000040
#define HAL_SRV_FGAVS_TYPE        halSRV_FGAVS_t
#define HAL_SRV_FGAVS             HAL_SRV_FGAVS_MASK
#define HAL_SRV_FGAVS_READABLE    1
#define HAL_SRV_FGAVS_WRITABLE    1
#define HAL_SRV_FGAVS_SIGNED      0
#define HAL_SRV_FGAVS_ADR         0xB0000354

#define HAL_SRV_FGASE_REGISTER    (HAL_SRVREG_MAP->FGASET)
#define HAL_SRV_FGASE_LSB         5
#define HAL_SRV_FGASE_MSB         5
#define HAL_SRV_FGASE_MASK        0x00000020
#define HAL_SRV_FGASE_TYPE        Bool
#define HAL_SRV_FGASE             HAL_SRV_FGASE_MASK
#define HAL_SRV_FGASE_READABLE    1
#define HAL_SRV_FGASE_WRITABLE    1
#define HAL_SRV_FGASE_SIGNED      0
#define HAL_SRV_FGASE_ADR         0xB0000354

#define HAL_SRV_FPHD_REGISTER    (HAL_SRVREG_MAP->FGASET)
#define HAL_SRV_FPHD_LSB         0
#define HAL_SRV_FPHD_MSB         4
#define HAL_SRV_FPHD_MASK        0x0000001F
#define HAL_SRV_FPHD_TYPE        UByte
#define HAL_SRV_FPHD             HAL_SRV_FPHD_MASK
#define HAL_SRV_FPHD_READABLE    1
#define HAL_SRV_FPHD_WRITABLE    1
#define HAL_SRV_FPHD_SIGNED      0
#define HAL_SRV_FPHD_ADR         0xB0000354

// Register TGASET
#define HAL_SRV_TGAVS_REGISTER    (HAL_SRVREG_MAP->TGASET)
#define HAL_SRV_TGAVS_LSB         6
#define HAL_SRV_TGAVS_MSB         6
#define HAL_SRV_TGAVS_MASK        0x00000040
#define HAL_SRV_TGAVS_TYPE        halSRV_TGAVS_t
#define HAL_SRV_TGAVS             HAL_SRV_TGAVS_MASK
#define HAL_SRV_TGAVS_READABLE    1
#define HAL_SRV_TGAVS_WRITABLE    1
#define HAL_SRV_TGAVS_SIGNED      0
#define HAL_SRV_TGAVS_ADR         0xB0000358

#define HAL_SRV_TGASE_REGISTER    (HAL_SRVREG_MAP->TGASET)
#define HAL_SRV_TGASE_LSB         5
#define HAL_SRV_TGASE_MSB         5
#define HAL_SRV_TGASE_MASK        0x00000020
#define HAL_SRV_TGASE_TYPE        Bool
#define HAL_SRV_TGASE             HAL_SRV_TGASE_MASK
#define HAL_SRV_TGASE_READABLE    1
#define HAL_SRV_TGASE_WRITABLE    1
#define HAL_SRV_TGASE_SIGNED      0
#define HAL_SRV_TGASE_ADR         0xB0000358

#define HAL_SRV_TPHD_REGISTER    (HAL_SRVREG_MAP->TGASET)
#define HAL_SRV_TPHD_LSB         0
#define HAL_SRV_TPHD_MSB         4
#define HAL_SRV_TPHD_MASK        0x0000001F
#define HAL_SRV_TPHD_TYPE        UByte
#define HAL_SRV_TPHD             HAL_SRV_TPHD_MASK
#define HAL_SRV_TPHD_READABLE    1
#define HAL_SRV_TPHD_WRITABLE    1
#define HAL_SRV_TPHD_SIGNED      0
#define HAL_SRV_TPHD_ADR         0xB0000358

// Register DASET
#define HAL_SRV_DATW_REGISTER    (HAL_SRVREG_MAP->DASET)
#define HAL_SRV_DATW_LSB         0
#define HAL_SRV_DATW_MSB         15
#define HAL_SRV_DATW_MASK        0x0000FFFF
#define HAL_SRV_DATW_TYPE        SInt16
#define HAL_SRV_DATW             HAL_SRV_DATW_MASK
#define HAL_SRV_DATW_READABLE    1
#define HAL_SRV_DATW_WRITABLE    1
#define HAL_SRV_DATW_SIGNED      1
#define HAL_SRV_DATW_ADR         0xB0000360

// Register RGSEL
#define HAL_SRV_RSEL_REGISTER    (HAL_SRVREG_MAP->RGSEL)
#define HAL_SRV_RSEL_LSB         0
#define HAL_SRV_RSEL_MSB         4
#define HAL_SRV_RSEL_MASK        0x0000001F
#define HAL_SRV_RSEL_TYPE        halSRV_ReadWriteSelect_t
#define HAL_SRV_RSEL             HAL_SRV_RSEL_MASK
#define HAL_SRV_RSEL_READABLE    1
#define HAL_SRV_RSEL_WRITABLE    1
#define HAL_SRV_RSEL_SIGNED      0
#define HAL_SRV_RSEL_ADR         0xB0000368

// Register MASK
#define HAL_SRV_PKBOT_REGISTER    (HAL_SRVREG_MAP->MASK)
#define HAL_SRV_PKBOT_LSB         7
#define HAL_SRV_PKBOT_MSB         7
#define HAL_SRV_PKBOT_MASK        0x00000080
#define HAL_SRV_PKBOT_TYPE        Bool
#define HAL_SRV_PKBOT             HAL_SRV_PKBOT_MASK
#define HAL_SRV_PKBOT_READABLE    1
#define HAL_SRV_PKBOT_WRITABLE    1
#define HAL_SRV_PKBOT_SIGNED      0
#define HAL_SRV_PKBOT_ADR         0xB000036C

#define HAL_SRV_POL_REGISTER    (HAL_SRVREG_MAP->MASK)
#define HAL_SRV_POL_LSB         6
#define HAL_SRV_POL_MSB         6
#define HAL_SRV_POL_MASK        0x00000040
#define HAL_SRV_POL_TYPE        Bool
#define HAL_SRV_POL             HAL_SRV_POL_MASK
#define HAL_SRV_POL_READABLE    1
#define HAL_SRV_POL_WRITABLE    1
#define HAL_SRV_POL_SIGNED      0
#define HAL_SRV_POL_ADR         0xB000036C

#define HAL_SRV_FR2X_REGISTER    (HAL_SRVREG_MAP->MASK)
#define HAL_SRV_FR2X_LSB         5
#define HAL_SRV_FR2X_MSB         5
#define HAL_SRV_FR2X_MASK        0x00000020
#define HAL_SRV_FR2X_TYPE        Bool
#define HAL_SRV_FR2X             HAL_SRV_FR2X_MASK
#define HAL_SRV_FR2X_READABLE    1
#define HAL_SRV_FR2X_WRITABLE    1
#define HAL_SRV_FR2X_SIGNED      0
#define HAL_SRV_FR2X_ADR         0xB000036C

#define HAL_SRV_MSKON_REGISTER    (HAL_SRVREG_MAP->MASK)
#define HAL_SRV_MSKON_LSB         4
#define HAL_SRV_MSKON_MSB         4
#define HAL_SRV_MSKON_MASK        0x00000010
#define HAL_SRV_MSKON_TYPE        Bool
#define HAL_SRV_MSKON             HAL_SRV_MSKON_MASK
#define HAL_SRV_MSKON_READABLE    1
#define HAL_SRV_MSKON_WRITABLE    1
#define HAL_SRV_MSKON_SIGNED      0
#define HAL_SRV_MSKON_ADR         0xB000036C

#define HAL_SRV_FILT_REGISTER    (HAL_SRVREG_MAP->MASK)
#define HAL_SRV_FILT_LSB         3
#define HAL_SRV_FILT_MSB         3
#define HAL_SRV_FILT_MASK        0x00000008
#define HAL_SRV_FILT_TYPE        halSRV_FILT_t
#define HAL_SRV_FILT             HAL_SRV_FILT_MASK
#define HAL_SRV_FILT_READABLE    1
#define HAL_SRV_FILT_WRITABLE    1
#define HAL_SRV_FILT_SIGNED      0
#define HAL_SRV_FILT_ADR         0xB000036C

#define HAL_SRV_AGC_REGISTER    (HAL_SRVREG_MAP->MASK)
#define HAL_SRV_AGC_LSB         0
#define HAL_SRV_AGC_MSB         1
#define HAL_SRV_AGC_MASK        0x00000003
#define HAL_SRV_AGC_TYPE        halSRV_AGCFreq_t
#define HAL_SRV_AGC             HAL_SRV_AGC_MASK
#define HAL_SRV_AGC_READABLE    1
#define HAL_SRV_AGC_WRITABLE    1
#define HAL_SRV_AGC_SIGNED      0
#define HAL_SRV_AGC_ADR         0xB000036C

// Register TROFFSET
#define HAL_SRV_TROFFSET_REGISTER    (HAL_SRVREG_MAP->TROFFSET)
#define HAL_SRV_TROFFSET_LSB         0
#define HAL_SRV_TROFFSET_MSB         7
#define HAL_SRV_TROFFSET_MASK        0x000000FF
#define HAL_SRV_TROFFSET_TYPE        SByte
#define HAL_SRV_TROFFSET             HAL_SRV_TROFFSET_MASK
#define HAL_SRV_TROFFSET_READABLE    1
#define HAL_SRV_TROFFSET_WRITABLE    1
#define HAL_SRV_TROFFSET_SIGNED      1
#define HAL_SRV_TROFFSET_ADR         0xB0000370

// Register DLFSKLVL
#define HAL_SRV_DKICKDAT_REGISTER    (HAL_SRVREG_MAP->DLFSKLVL)
#define HAL_SRV_DKICKDAT_LSB         0
#define HAL_SRV_DKICKDAT_MSB         7
#define HAL_SRV_DKICKDAT_MASK        0x000000FF
#define HAL_SRV_DKICKDAT_TYPE        SByte
#define HAL_SRV_DKICKDAT             HAL_SRV_DKICKDAT_MASK
#define HAL_SRV_DKICKDAT_READABLE    1
#define HAL_SRV_DKICKDAT_WRITABLE    1
#define HAL_SRV_DKICKDAT_SIGNED      1
#define HAL_SRV_DKICKDAT_ADR         0xB0000374

// Register DLFSBLVL
#define HAL_SRV_DBRKDAT_REGISTER    (HAL_SRVREG_MAP->DLFSBLVL)
#define HAL_SRV_DBRKDAT_LSB         0
#define HAL_SRV_DBRKDAT_MSB         7
#define HAL_SRV_DBRKDAT_MASK        0x000000FF
#define HAL_SRV_DBRKDAT_TYPE        SByte
#define HAL_SRV_DBRKDAT             HAL_SRV_DBRKDAT_MASK
#define HAL_SRV_DBRKDAT_READABLE    1
#define HAL_SRV_DBRKDAT_WRITABLE    1
#define HAL_SRV_DBRKDAT_SIGNED      1
#define HAL_SRV_DBRKDAT_ADR         0xB0000378

// Register FOOFFSET

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_FOOFFSET_REGISTER    (HAL_SRVREG_MAP->FOOFFSET)
#define HAL_SRV_FOOFFSET_LSB         0
#define HAL_SRV_FOOFFSET_MSB         7
#define HAL_SRV_FOOFFSET_MASK        0x000000FF
#define HAL_SRV_FOOFFSET_TYPE        SByte
#define HAL_SRV_FOOFFSET             HAL_SRV_FOOFFSET_MASK
#define HAL_SRV_FOOFFSET_READABLE    1
#define HAL_SRV_FOOFFSET_WRITABLE    1
#define HAL_SRV_FOOFFSET_SIGNED      1
#define HAL_SRV_FOOFFSET_ADR         0xB000037C

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_FOOFFSET_REGISTER    (HAL_SRVREG_MAP->FOOFFSET)
#define HAL_SRV_FOOFFSET_LSB         0
#define HAL_SRV_FOOFFSET_MSB         9
#define HAL_SRV_FOOFFSET_MASK        0x000003FF
#define HAL_SRV_FOOFFSET_TYPE        SInt16
#define HAL_SRV_FOOFFSET             HAL_SRV_FOOFFSET_MASK
#define HAL_SRV_FOOFFSET_READABLE    1
#define HAL_SRV_FOOFFSET_WRITABLE    1
#define HAL_SRV_FOOFFSET_SIGNED      1
#define HAL_SRV_FOOFFSET_ADR         0xB000037C

#endif // #if (CHIP_REV >= 0xC0)

// Register FSCNT

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_FOKT_REGISTER    (HAL_SRVREG_MAP->FSCNT)
#define HAL_SRV_FOKT_LSB         6
#define HAL_SRV_FOKT_MSB         7
#define HAL_SRV_FOKT_MASK        0x000000C0
#define HAL_SRV_FOKT_TYPE        halSRV_FOKT_t
#define HAL_SRV_FOKT             HAL_SRV_FOKT_MASK
#define HAL_SRV_FOKT_READABLE    1
#define HAL_SRV_FOKT_WRITABLE    1
#define HAL_SRV_FOKT_SIGNED      0
#define HAL_SRV_FOKT_ADR         0xB0000380

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_FNGT_REGISTER    (HAL_SRVREG_MAP->FSCNT)
#define HAL_SRV_FNGT_LSB         20
#define HAL_SRV_FNGT_MSB         31
#define HAL_SRV_FNGT_MASK        0xFFF00000
#define HAL_SRV_FNGT_TYPE        UInt16
#define HAL_SRV_FNGT             HAL_SRV_FNGT_MASK
#define HAL_SRV_FNGT_READABLE    1
#define HAL_SRV_FNGT_WRITABLE    1
#define HAL_SRV_FNGT_SIGNED      0
#define HAL_SRV_FNGT_ADR         0xB0000380

#define HAL_SRV_FOKT_REGISTER    (HAL_SRVREG_MAP->FSCNT)
#define HAL_SRV_FOKT_LSB         8
#define HAL_SRV_FOKT_MSB         19
#define HAL_SRV_FOKT_MASK        0x000FFF00
#define HAL_SRV_FOKT_TYPE        UInt16
#define HAL_SRV_FOKT             HAL_SRV_FOKT_MASK
#define HAL_SRV_FOKT_READABLE    1
#define HAL_SRV_FOKT_WRITABLE    1
#define HAL_SRV_FOKT_SIGNED      0
#define HAL_SRV_FOKT_ADR         0xB0000380

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_FSR_REGISTER    (HAL_SRVREG_MAP->FSCNT)
#define HAL_SRV_FSR_LSB         4
#define HAL_SRV_FSR_MSB         5
#define HAL_SRV_FSR_MASK        0x00000030
#define HAL_SRV_FSR_TYPE        halSRV_FSR_t
#define HAL_SRV_FSR             HAL_SRV_FSR_MASK
#define HAL_SRV_FSR_READABLE    1
#define HAL_SRV_FSR_WRITABLE    1
#define HAL_SRV_FSR_SIGNED      0
#define HAL_SRV_FSR_ADR         0xB0000380

#define HAL_SRV_FGUP_REGISTER    (HAL_SRVREG_MAP->FSCNT)
#define HAL_SRV_FGUP_LSB         3
#define HAL_SRV_FGUP_MSB         3
#define HAL_SRV_FGUP_MASK        0x00000008
#define HAL_SRV_FGUP_TYPE        Bool
#define HAL_SRV_FGUP             HAL_SRV_FGUP_MASK
#define HAL_SRV_FGUP_READABLE    1
#define HAL_SRV_FGUP_WRITABLE    1
#define HAL_SRV_FGUP_SIGNED      0
#define HAL_SRV_FGUP_ADR         0xB0000380

#define HAL_SRV_FSM_REGISTER    (HAL_SRVREG_MAP->FSCNT)
#define HAL_SRV_FSM_LSB         0
#define HAL_SRV_FSM_MSB         2
#define HAL_SRV_FSM_MASK        0x00000007
#define HAL_SRV_FSM_TYPE        halSRV_FSM_t
#define HAL_SRV_FSM             HAL_SRV_FSM_MASK
#define HAL_SRV_FSM_READABLE    1
#define HAL_SRV_FSM_WRITABLE    1
#define HAL_SRV_FSM_SIGNED      0
#define HAL_SRV_FSM_ADR         0xB0000380

// Register FOCUS

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_BYPSCNT_REGISTER    (HAL_SRVREG_MAP->FOCUS)
#define HAL_SRV_BYPSCNT_LSB         9
#define HAL_SRV_BYPSCNT_MSB         11
#define HAL_SRV_BYPSCNT_MASK        0x00000E00
#define HAL_SRV_BYPSCNT_TYPE        UByte
#define HAL_SRV_BYPSCNT             HAL_SRV_BYPSCNT_MASK
#define HAL_SRV_BYPSCNT_READABLE    1
#define HAL_SRV_BYPSCNT_WRITABLE    1
#define HAL_SRV_BYPSCNT_SIGNED      0
#define HAL_SRV_BYPSCNT_ADR         0xB0000384

#define HAL_SRV_BYPSEN_REGISTER    (HAL_SRVREG_MAP->FOCUS)
#define HAL_SRV_BYPSEN_LSB         8
#define HAL_SRV_BYPSEN_MSB         8
#define HAL_SRV_BYPSEN_MASK        0x00000100
#define HAL_SRV_BYPSEN_TYPE        Bool
#define HAL_SRV_BYPSEN             HAL_SRV_BYPSEN_MASK
#define HAL_SRV_BYPSEN_READABLE    1
#define HAL_SRV_BYPSEN_WRITABLE    1
#define HAL_SRV_BYPSEN_SIGNED      0
#define HAL_SRV_BYPSEN_ADR         0xB0000384

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_FSL_REGISTER    (HAL_SRVREG_MAP->FOCUS)
#define HAL_SRV_FSL_LSB         0
#define HAL_SRV_FSL_MSB         6
#define HAL_SRV_FSL_MASK        0x0000007F
#define HAL_SRV_FSL_TYPE        halSRV_FSL_t
#define HAL_SRV_FSL             HAL_SRV_FSL_MASK
#define HAL_SRV_FSL_READABLE    1
#define HAL_SRV_FSL_WRITABLE    1
#define HAL_SRV_FSL_SIGNED      0
#define HAL_SRV_FSL_ADR         0xB0000384

// Register SHOCK
#define HAL_SRV_SHL_REGISTER    (HAL_SRVREG_MAP->SHOCK)
#define HAL_SRV_SHL_LSB         4
#define HAL_SRV_SHL_MSB         7
#define HAL_SRV_SHL_MASK        0x000000F0
#define HAL_SRV_SHL_TYPE        halSRV_SHL_t
#define HAL_SRV_SHL             HAL_SRV_SHL_MASK
#define HAL_SRV_SHL_READABLE    1
#define HAL_SRV_SHL_WRITABLE    1
#define HAL_SRV_SHL_SIGNED      0
#define HAL_SRV_SHL_ADR         0xB000038C

#define HAL_SRV_SHT_REGISTER    (HAL_SRVREG_MAP->SHOCK)
#define HAL_SRV_SHT_LSB         0
#define HAL_SRV_SHT_MSB         3
#define HAL_SRV_SHT_MASK        0x0000000F
#define HAL_SRV_SHT_TYPE        UByte
#define HAL_SRV_SHT             HAL_SRV_SHT_MASK
#define HAL_SRV_SHT_READABLE    1
#define HAL_SRV_SHT_WRITABLE    1
#define HAL_SRV_SHT_SIGNED      0
#define HAL_SRV_SHT_ADR         0xB000038C

// Register SREG1
#define HAL_SRV_GUPL_REGISTER    (HAL_SRVREG_MAP->SREG1)
#define HAL_SRV_GUPL_LSB         6
#define HAL_SRV_GUPL_MSB         7
#define HAL_SRV_GUPL_MASK        0x000000C0
#define HAL_SRV_GUPL_TYPE        halSRV_GainUpOperation_t
#define HAL_SRV_GUPL             HAL_SRV_GUPL_MASK
#define HAL_SRV_GUPL_READABLE    1
#define HAL_SRV_GUPL_WRITABLE    1
#define HAL_SRV_GUPL_SIGNED      0
#define HAL_SRV_GUPL_ADR         0xB0000390

#define HAL_SRV_HYSL_REGISTER    (HAL_SRVREG_MAP->SREG1)
#define HAL_SRV_HYSL_LSB         4
#define HAL_SRV_HYSL_MSB         5
#define HAL_SRV_HYSL_MASK        0x00000030
#define HAL_SRV_HYSL_TYPE        halSRV_HysteresisOperation_t
#define HAL_SRV_HYSL             HAL_SRV_HYSL_MASK
#define HAL_SRV_HYSL_READABLE    1
#define HAL_SRV_HYSL_WRITABLE    1
#define HAL_SRV_HYSL_SIGNED      0
#define HAL_SRV_HYSL_ADR         0xB0000390

#define HAL_SRV_FHOLD_REGISTER    (HAL_SRVREG_MAP->SREG1)
#define HAL_SRV_FHOLD_LSB         3
#define HAL_SRV_FHOLD_MSB         3
#define HAL_SRV_FHOLD_MASK        0x00000008
#define HAL_SRV_FHOLD_TYPE        Bool
#define HAL_SRV_FHOLD             HAL_SRV_FHOLD_MASK
#define HAL_SRV_FHOLD_READABLE    1
#define HAL_SRV_FHOLD_WRITABLE    1
#define HAL_SRV_FHOLD_SIGNED      0
#define HAL_SRV_FHOLD_ADR         0xB0000390

#define HAL_SRV_THOLD_REGISTER    (HAL_SRVREG_MAP->SREG1)
#define HAL_SRV_THOLD_LSB         2
#define HAL_SRV_THOLD_MSB         2
#define HAL_SRV_THOLD_MASK        0x00000004
#define HAL_SRV_THOLD_TYPE        Bool
#define HAL_SRV_THOLD             HAL_SRV_THOLD_MASK
#define HAL_SRV_THOLD_READABLE    1
#define HAL_SRV_THOLD_WRITABLE    1
#define HAL_SRV_THOLD_SIGNED      0
#define HAL_SRV_THOLD_ADR         0xB0000390

#define HAL_SRV_GUPCL_REGISTER    (HAL_SRVREG_MAP->SREG1)
#define HAL_SRV_GUPCL_LSB         1
#define HAL_SRV_GUPCL_MSB         1
#define HAL_SRV_GUPCL_MASK        0x00000002
#define HAL_SRV_GUPCL_TYPE        Bool
#define HAL_SRV_GUPCL             HAL_SRV_GUPCL_MASK
#define HAL_SRV_GUPCL_READABLE    1
#define HAL_SRV_GUPCL_WRITABLE    1
#define HAL_SRV_GUPCL_SIGNED      0
#define HAL_SRV_GUPCL_ADR         0xB0000390

#define HAL_SRV_XCOEF_REGISTER    (HAL_SRVREG_MAP->SREG1)
#define HAL_SRV_XCOEF_LSB         0
#define HAL_SRV_XCOEF_MSB         0
#define HAL_SRV_XCOEF_MASK        0x00000001
#define HAL_SRV_XCOEF_TYPE        Bool
#define HAL_SRV_XCOEF             HAL_SRV_XCOEF_MASK
#define HAL_SRV_XCOEF_READABLE    1
#define HAL_SRV_XCOEF_WRITABLE    1
#define HAL_SRV_XCOEF_SIGNED      0
#define HAL_SRV_XCOEF_ADR         0xB0000390

// Register SREG2
#define HAL_SRV_GUP2_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_GUP2_LSB         7
#define HAL_SRV_GUP2_MSB         7
#define HAL_SRV_GUP2_MASK        0x00000080
#define HAL_SRV_GUP2_TYPE        Bool
#define HAL_SRV_GUP2             HAL_SRV_GUP2_MASK
#define HAL_SRV_GUP2_READABLE    1
#define HAL_SRV_GUP2_WRITABLE    1
#define HAL_SRV_GUP2_SIGNED      0
#define HAL_SRV_GUP2_ADR         0xB0000394

#define HAL_SRV_HYS2_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_HYS2_LSB         6
#define HAL_SRV_HYS2_MSB         6
#define HAL_SRV_HYS2_MASK        0x00000040
#define HAL_SRV_HYS2_TYPE        Bool
#define HAL_SRV_HYS2             HAL_SRV_HYS2_MASK
#define HAL_SRV_HYS2_READABLE    1
#define HAL_SRV_HYS2_WRITABLE    1
#define HAL_SRV_HYS2_SIGNED      0
#define HAL_SRV_HYS2_ADR         0xB0000394

#define HAL_SRV_GUP1_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_GUP1_LSB         5
#define HAL_SRV_GUP1_MSB         5
#define HAL_SRV_GUP1_MASK        0x00000020
#define HAL_SRV_GUP1_TYPE        Bool
#define HAL_SRV_GUP1             HAL_SRV_GUP1_MASK
#define HAL_SRV_GUP1_READABLE    1
#define HAL_SRV_GUP1_WRITABLE    1
#define HAL_SRV_GUP1_SIGNED      0
#define HAL_SRV_GUP1_ADR         0xB0000394

#define HAL_SRV_HYS1_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_HYS1_LSB         4
#define HAL_SRV_HYS1_MSB         4
#define HAL_SRV_HYS1_MASK        0x00000010
#define HAL_SRV_HYS1_TYPE        Bool
#define HAL_SRV_HYS1             HAL_SRV_HYS1_MASK
#define HAL_SRV_HYS1_READABLE    1
#define HAL_SRV_HYS1_WRITABLE    1
#define HAL_SRV_HYS1_SIGNED      0
#define HAL_SRV_HYS1_ADR         0xB0000394

#define HAL_SRV_HYSGUP_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_HYSGUP_LSB         3
#define HAL_SRV_HYSGUP_MSB         3
#define HAL_SRV_HYSGUP_MASK        0x00000008
#define HAL_SRV_HYSGUP_TYPE        Bool
#define HAL_SRV_HYSGUP             HAL_SRV_HYSGUP_MASK
#define HAL_SRV_HYSGUP_READABLE    1
#define HAL_SRV_HYSGUP_WRITABLE    1
#define HAL_SRV_HYSGUP_SIGNED      0
#define HAL_SRV_HYSGUP_ADR         0xB0000394

#define HAL_SRV_RXMUTE_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_RXMUTE_LSB         2
#define HAL_SRV_RXMUTE_MSB         2
#define HAL_SRV_RXMUTE_MASK        0x00000004
#define HAL_SRV_RXMUTE_TYPE        Bool
#define HAL_SRV_RXMUTE             HAL_SRV_RXMUTE_MASK
#define HAL_SRV_RXMUTE_READABLE    1
#define HAL_SRV_RXMUTE_WRITABLE    1
#define HAL_SRV_RXMUTE_SIGNED      0
#define HAL_SRV_RXMUTE_ADR         0xB0000394

#define HAL_SRV_LOCKON_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_LOCKON_LSB         1
#define HAL_SRV_LOCKON_MSB         1
#define HAL_SRV_LOCKON_MASK        0x00000002
#define HAL_SRV_LOCKON_TYPE        Bool
#define HAL_SRV_LOCKON             HAL_SRV_LOCKON_MASK
#define HAL_SRV_LOCKON_READABLE    1
#define HAL_SRV_LOCKON_WRITABLE    1
#define HAL_SRV_LOCKON_SIGNED      0
#define HAL_SRV_LOCKON_ADR         0xB0000394

#define HAL_SRV_LENSDN_REGISTER    (HAL_SRVREG_MAP->SREG2)
#define HAL_SRV_LENSDN_LSB         0
#define HAL_SRV_LENSDN_MSB         0
#define HAL_SRV_LENSDN_MASK        0x00000001
#define HAL_SRV_LENSDN_TYPE        Bool
#define HAL_SRV_LENSDN             HAL_SRV_LENSDN_MASK
#define HAL_SRV_LENSDN_READABLE    1
#define HAL_SRV_LENSDN_WRITABLE    1
#define HAL_SRV_LENSDN_SIGNED      0
#define HAL_SRV_LENSDN_ADR         0xB0000394

// Register FVSET

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_FVD2_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_FVD2_LSB         16
#define HAL_SRV_FVD2_MSB         22
#define HAL_SRV_FVD2_MASK        0x007F0000
#define HAL_SRV_FVD2_TYPE        UByte
#define HAL_SRV_FVD2             HAL_SRV_FVD2_MASK
#define HAL_SRV_FVD2_READABLE    1
#define HAL_SRV_FVD2_WRITABLE    1
#define HAL_SRV_FVD2_SIGNED      0
#define HAL_SRV_FVD2_ADR         0xB0000398

#define HAL_SRV_CONTSLOW_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_CONTSLOW_LSB         15
#define HAL_SRV_CONTSLOW_MSB         15
#define HAL_SRV_CONTSLOW_MASK        0x00008000
#define HAL_SRV_CONTSLOW_TYPE        Bool
#define HAL_SRV_CONTSLOW             HAL_SRV_CONTSLOW_MASK
#define HAL_SRV_CONTSLOW_READABLE    1
#define HAL_SRV_CONTSLOW_WRITABLE    1
#define HAL_SRV_CONTSLOW_SIGNED      0
#define HAL_SRV_CONTSLOW_ADR         0xB0000398

#define HAL_SRV_FVD_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_FVD_LSB         8
#define HAL_SRV_FVD_MSB         14
#define HAL_SRV_FVD_MASK        0x00007F00
#define HAL_SRV_FVD_TYPE        UByte
#define HAL_SRV_FVD             HAL_SRV_FVD_MASK
#define HAL_SRV_FVD_READABLE    1
#define HAL_SRV_FVD_WRITABLE    1
#define HAL_SRV_FVD_SIGNED      0
#define HAL_SRV_FVD_ADR         0xB0000398

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_KWIDSL_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_KWIDSL_LSB         6
#define HAL_SRV_KWIDSL_MSB         7
#define HAL_SRV_KWIDSL_MASK        0x000000C0
#define HAL_SRV_KWIDSL_TYPE        halSRV_KickWindow_t
#define HAL_SRV_KWIDSL             HAL_SRV_KWIDSL_MASK
#define HAL_SRV_KWIDSL_READABLE    1
#define HAL_SRV_KWIDSL_WRITABLE    1
#define HAL_SRV_KWIDSL_SIGNED      0
#define HAL_SRV_KWIDSL_ADR         0xB0000398

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_FVD_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_FVD_LSB         4
#define HAL_SRV_FVD_MSB         5
#define HAL_SRV_FVD_MASK        0x00000030
#define HAL_SRV_FVD_TYPE        halSRV_TrackCrossingSpeedRefFreq_t
#define HAL_SRV_FVD             HAL_SRV_FVD_MASK
#define HAL_SRV_FVD_READABLE    1
#define HAL_SRV_FVD_WRITABLE    1
#define HAL_SRV_FVD_SIGNED      0
#define HAL_SRV_FVD_ADR         0xB0000398

#endif // #if (CHIP_REV <= 0xB3)

#define HAL_SRV_NOFOKB_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_NOFOKB_LSB         3
#define HAL_SRV_NOFOKB_MSB         3
#define HAL_SRV_NOFOKB_MASK        0x00000008
#define HAL_SRV_NOFOKB_TYPE        Bool
#define HAL_SRV_NOFOKB             HAL_SRV_NOFOKB_MASK
#define HAL_SRV_NOFOKB_READABLE    1
#define HAL_SRV_NOFOKB_WRITABLE    1
#define HAL_SRV_NOFOKB_SIGNED      0
#define HAL_SRV_NOFOKB_ADR         0xB0000398

#define HAL_SRV_FORBLK_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_FORBLK_LSB         2
#define HAL_SRV_FORBLK_MSB         2
#define HAL_SRV_FORBLK_MASK        0x00000004
#define HAL_SRV_FORBLK_TYPE        Bool
#define HAL_SRV_FORBLK             HAL_SRV_FORBLK_MASK
#define HAL_SRV_FORBLK_READABLE    1
#define HAL_SRV_FORBLK_WRITABLE    1
#define HAL_SRV_FORBLK_SIGNED      0
#define HAL_SRV_FORBLK_ADR         0xB0000398

#define HAL_SRV_LANDPOL_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_LANDPOL_LSB         1
#define HAL_SRV_LANDPOL_MSB         1
#define HAL_SRV_LANDPOL_MASK        0x00000002
#define HAL_SRV_LANDPOL_TYPE        Bool
#define HAL_SRV_LANDPOL             HAL_SRV_LANDPOL_MASK
#define HAL_SRV_LANDPOL_READABLE    1
#define HAL_SRV_LANDPOL_WRITABLE    1
#define HAL_SRV_LANDPOL_SIGNED      0
#define HAL_SRV_LANDPOL_ADR         0xB0000398

#define HAL_SRV_LANDXC_REGISTER    (HAL_SRVREG_MAP->FVSET)
#define HAL_SRV_LANDXC_LSB         0
#define HAL_SRV_LANDXC_MSB         0
#define HAL_SRV_LANDXC_MASK        0x00000001
#define HAL_SRV_LANDXC_TYPE        Bool
#define HAL_SRV_LANDXC             HAL_SRV_LANDXC_MASK
#define HAL_SRV_LANDXC_READABLE    1
#define HAL_SRV_LANDXC_WRITABLE    1
#define HAL_SRV_LANDXC_SIGNED      0
#define HAL_SRV_LANDXC_ADR         0xB0000398

// Register SVSET
#define HAL_SRV_FGSW_REGISTER    (HAL_SRVREG_MAP->SVSET)
#define HAL_SRV_FGSW_LSB         7
#define HAL_SRV_FGSW_MSB         7
#define HAL_SRV_FGSW_MASK        0x00000080
#define HAL_SRV_FGSW_TYPE        halSRV_FGSW_t
#define HAL_SRV_FGSW             HAL_SRV_FGSW_MASK
#define HAL_SRV_FGSW_READABLE    1
#define HAL_SRV_FGSW_WRITABLE    1
#define HAL_SRV_FGSW_SIGNED      0
#define HAL_SRV_FGSW_ADR         0xB000039C

#define HAL_SRV_TGSW_REGISTER    (HAL_SRVREG_MAP->SVSET)
#define HAL_SRV_TGSW_LSB         6
#define HAL_SRV_TGSW_MSB         6
#define HAL_SRV_TGSW_MASK        0x00000040
#define HAL_SRV_TGSW_TYPE        halSRV_TGSW_t
#define HAL_SRV_TGSW             HAL_SRV_TGSW_MASK
#define HAL_SRV_TGSW_READABLE    1
#define HAL_SRV_TGSW_WRITABLE    1
#define HAL_SRV_TGSW_SIGNED      0
#define HAL_SRV_TGSW_ADR         0xB000039C

#define HAL_SRV_FOSW_REGISTER    (HAL_SRVREG_MAP->SVSET)
#define HAL_SRV_FOSW_LSB         3
#define HAL_SRV_FOSW_MSB         3
#define HAL_SRV_FOSW_MASK        0x00000008
#define HAL_SRV_FOSW_TYPE        Bool
#define HAL_SRV_FOSW             HAL_SRV_FOSW_MASK
#define HAL_SRV_FOSW_READABLE    1
#define HAL_SRV_FOSW_WRITABLE    1
#define HAL_SRV_FOSW_SIGNED      0
#define HAL_SRV_FOSW_ADR         0xB000039C

#define HAL_SRV_TRSW_REGISTER    (HAL_SRVREG_MAP->SVSET)
#define HAL_SRV_TRSW_LSB         2
#define HAL_SRV_TRSW_MSB         2
#define HAL_SRV_TRSW_MASK        0x00000004
#define HAL_SRV_TRSW_TYPE        Bool
#define HAL_SRV_TRSW             HAL_SRV_TRSW_MASK
#define HAL_SRV_TRSW_READABLE    1
#define HAL_SRV_TRSW_WRITABLE    1
#define HAL_SRV_TRSW_SIGNED      0
#define HAL_SRV_TRSW_ADR         0xB000039C

#define HAL_SRV_TXMUX_REGISTER    (HAL_SRVREG_MAP->SVSET)
#define HAL_SRV_TXMUX_LSB         1
#define HAL_SRV_TXMUX_MSB         1
#define HAL_SRV_TXMUX_MASK        0x00000002
#define HAL_SRV_TXMUX_TYPE        halSRV_TXMUX_t
#define HAL_SRV_TXMUX             HAL_SRV_TXMUX_MASK
#define HAL_SRV_TXMUX_READABLE    1
#define HAL_SRV_TXMUX_WRITABLE    1
#define HAL_SRV_TXMUX_SIGNED      0
#define HAL_SRV_TXMUX_ADR         0xB000039C

#define HAL_SRV_FDSW_REGISTER    (HAL_SRVREG_MAP->SVSET)
#define HAL_SRV_FDSW_LSB         0
#define HAL_SRV_FDSW_MSB         0
#define HAL_SRV_FDSW_MASK        0x00000001
#define HAL_SRV_FDSW_TYPE        Bool
#define HAL_SRV_FDSW             HAL_SRV_FDSW_MASK
#define HAL_SRV_FDSW_READABLE    1
#define HAL_SRV_FDSW_WRITABLE    1
#define HAL_SRV_FDSW_SIGNED      0
#define HAL_SRV_FDSW_ADR         0xB000039C

// Register SVMIOSEL
#define HAL_SRV_SVMIO3SEL_REGISTER    (HAL_SRVREG_MAP->SVMIOSEL)
#define HAL_SRV_SVMIO3SEL_LSB         12
#define HAL_SRV_SVMIO3SEL_MSB         15
#define HAL_SRV_SVMIO3SEL_MASK        0x0000F000
#define HAL_SRV_SVMIO3SEL_TYPE        halSRV_ServoMio3Select_t
#define HAL_SRV_SVMIO3SEL             HAL_SRV_SVMIO3SEL_MASK
#define HAL_SRV_SVMIO3SEL_READABLE    1
#define HAL_SRV_SVMIO3SEL_WRITABLE    1
#define HAL_SRV_SVMIO3SEL_SIGNED      0
#define HAL_SRV_SVMIO3SEL_ADR         0xB00003A0

#define HAL_SRV_SVMIO2SEL_REGISTER    (HAL_SRVREG_MAP->SVMIOSEL)
#define HAL_SRV_SVMIO2SEL_LSB         8
#define HAL_SRV_SVMIO2SEL_MSB         11
#define HAL_SRV_SVMIO2SEL_MASK        0x00000F00
#define HAL_SRV_SVMIO2SEL_TYPE        halSRV_ServoMio2Select_t
#define HAL_SRV_SVMIO2SEL             HAL_SRV_SVMIO2SEL_MASK
#define HAL_SRV_SVMIO2SEL_READABLE    1
#define HAL_SRV_SVMIO2SEL_WRITABLE    1
#define HAL_SRV_SVMIO2SEL_SIGNED      0
#define HAL_SRV_SVMIO2SEL_ADR         0xB00003A0

#define HAL_SRV_SVMIO1SEL_REGISTER    (HAL_SRVREG_MAP->SVMIOSEL)
#define HAL_SRV_SVMIO1SEL_LSB         4
#define HAL_SRV_SVMIO1SEL_MSB         7
#define HAL_SRV_SVMIO1SEL_MASK        0x000000F0
#define HAL_SRV_SVMIO1SEL_TYPE        halSRV_ServoMio1Select_t
#define HAL_SRV_SVMIO1SEL             HAL_SRV_SVMIO1SEL_MASK
#define HAL_SRV_SVMIO1SEL_READABLE    1
#define HAL_SRV_SVMIO1SEL_WRITABLE    1
#define HAL_SRV_SVMIO1SEL_SIGNED      0
#define HAL_SRV_SVMIO1SEL_ADR         0xB00003A0

#define HAL_SRV_SVMIO0SEL_REGISTER    (HAL_SRVREG_MAP->SVMIOSEL)
#define HAL_SRV_SVMIO0SEL_LSB         0
#define HAL_SRV_SVMIO0SEL_MSB         3
#define HAL_SRV_SVMIO0SEL_MASK        0x0000000F
#define HAL_SRV_SVMIO0SEL_TYPE        halSRV_ServoMio0Select_t
#define HAL_SRV_SVMIO0SEL             HAL_SRV_SVMIO0SEL_MASK
#define HAL_SRV_SVMIO0SEL_READABLE    1
#define HAL_SRV_SVMIO0SEL_WRITABLE    1
#define HAL_SRV_SVMIO0SEL_SIGNED      0
#define HAL_SRV_SVMIO0SEL_ADR         0xB00003A0

#if (CHIP_REV >= 0xC0)

// Register DBGBUSSEL
#define HAL_SRV_DBGBUSEN_REGISTER    (HAL_SRVREG_MAP->DBGBUSSEL)
#define HAL_SRV_DBGBUSEN_LSB         28
#define HAL_SRV_DBGBUSEN_MSB         28
#define HAL_SRV_DBGBUSEN_MASK        0x10000000
#define HAL_SRV_DBGBUSEN_TYPE        Bool
#define HAL_SRV_DBGBUSEN             HAL_SRV_DBGBUSEN_MASK
#define HAL_SRV_DBGBUSEN_READABLE    1
#define HAL_SRV_DBGBUSEN_WRITABLE    1
#define HAL_SRV_DBGBUSEN_SIGNED      0
#define HAL_SRV_DBGBUSEN_ADR         0xB00003A8

#define HAL_SRV_DBGBUSSELD_REGISTER    (HAL_SRVREG_MAP->DBGBUSSEL)
#define HAL_SRV_DBGBUSSELD_LSB         24
#define HAL_SRV_DBGBUSSELD_MSB         27
#define HAL_SRV_DBGBUSSELD_MASK        0x0F000000
#define HAL_SRV_DBGBUSSELD_TYPE        halSRV_DBGBUSSELD_t
#define HAL_SRV_DBGBUSSELD             HAL_SRV_DBGBUSSELD_MASK
#define HAL_SRV_DBGBUSSELD_READABLE    1
#define HAL_SRV_DBGBUSSELD_WRITABLE    1
#define HAL_SRV_DBGBUSSELD_SIGNED      0
#define HAL_SRV_DBGBUSSELD_ADR         0xB00003A8

#define HAL_SRV_DBGBUSSELC_REGISTER    (HAL_SRVREG_MAP->DBGBUSSEL)
#define HAL_SRV_DBGBUSSELC_LSB         16
#define HAL_SRV_DBGBUSSELC_MSB         19
#define HAL_SRV_DBGBUSSELC_MASK        0x000F0000
#define HAL_SRV_DBGBUSSELC_TYPE        halSRV_DBGBUSSELC_t
#define HAL_SRV_DBGBUSSELC             HAL_SRV_DBGBUSSELC_MASK
#define HAL_SRV_DBGBUSSELC_READABLE    1
#define HAL_SRV_DBGBUSSELC_WRITABLE    1
#define HAL_SRV_DBGBUSSELC_SIGNED      0
#define HAL_SRV_DBGBUSSELC_ADR         0xB00003A8

#define HAL_SRV_DBGBUSSELB_REGISTER    (HAL_SRVREG_MAP->DBGBUSSEL)
#define HAL_SRV_DBGBUSSELB_LSB         8
#define HAL_SRV_DBGBUSSELB_MSB         11
#define HAL_SRV_DBGBUSSELB_MASK        0x00000F00
#define HAL_SRV_DBGBUSSELB_TYPE        halSRV_DBGBUSSELB_t
#define HAL_SRV_DBGBUSSELB             HAL_SRV_DBGBUSSELB_MASK
#define HAL_SRV_DBGBUSSELB_READABLE    1
#define HAL_SRV_DBGBUSSELB_WRITABLE    1
#define HAL_SRV_DBGBUSSELB_SIGNED      0
#define HAL_SRV_DBGBUSSELB_ADR         0xB00003A8

#define HAL_SRV_DBGBUSSELA_REGISTER    (HAL_SRVREG_MAP->DBGBUSSEL)
#define HAL_SRV_DBGBUSSELA_LSB         0
#define HAL_SRV_DBGBUSSELA_MSB         3
#define HAL_SRV_DBGBUSSELA_MASK        0x0000000F
#define HAL_SRV_DBGBUSSELA_TYPE        halSRV_DBGBUSSELA_t
#define HAL_SRV_DBGBUSSELA             HAL_SRV_DBGBUSSELA_MASK
#define HAL_SRV_DBGBUSSELA_READABLE    1
#define HAL_SRV_DBGBUSSELA_WRITABLE    1
#define HAL_SRV_DBGBUSSELA_SIGNED      0
#define HAL_SRV_DBGBUSSELA_ADR         0xB00003A8

#endif // #if (CHIP_REV >= 0xC0)

// Register DMCNT
#define HAL_SRV_CLVWBLSEL_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_CLVWBLSEL_LSB         7
#define HAL_SRV_CLVWBLSEL_MSB         7
#define HAL_SRV_CLVWBLSEL_MASK        0x00000080
#define HAL_SRV_CLVWBLSEL_TYPE        Bool
#define HAL_SRV_CLVWBLSEL             HAL_SRV_CLVWBLSEL_MASK
#define HAL_SRV_CLVWBLSEL_READABLE    1
#define HAL_SRV_CLVWBLSEL_WRITABLE    1
#define HAL_SRV_CLVWBLSEL_SIGNED      0
#define HAL_SRV_CLVWBLSEL_ADR         0xB00003AC

#define HAL_SRV_DVDCDSEL_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_DVDCDSEL_LSB         6
#define HAL_SRV_DVDCDSEL_MSB         6
#define HAL_SRV_DVDCDSEL_MASK        0x00000040
#define HAL_SRV_DVDCDSEL_TYPE        Bool
#define HAL_SRV_DVDCDSEL             HAL_SRV_DVDCDSEL_MASK
#define HAL_SRV_DVDCDSEL_READABLE    1
#define HAL_SRV_DVDCDSEL_WRITABLE    1
#define HAL_SRV_DVDCDSEL_SIGNED      0
#define HAL_SRV_DVDCDSEL_ADR         0xB00003AC

#define HAL_SRV_CAVSIGSEL_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_CAVSIGSEL_LSB         5
#define HAL_SRV_CAVSIGSEL_MSB         5
#define HAL_SRV_CAVSIGSEL_MASK        0x00000020
#define HAL_SRV_CAVSIGSEL_TYPE        Bool
#define HAL_SRV_CAVSIGSEL             HAL_SRV_CAVSIGSEL_MASK
#define HAL_SRV_CAVSIGSEL_READABLE    1
#define HAL_SRV_CAVSIGSEL_WRITABLE    1
#define HAL_SRV_CAVSIGSEL_SIGNED      0
#define HAL_SRV_CAVSIGSEL_ADR         0xB00003AC

#define HAL_SRV_BKAUTO_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_BKAUTO_LSB         3
#define HAL_SRV_BKAUTO_MSB         3
#define HAL_SRV_BKAUTO_MASK        0x00000008
#define HAL_SRV_BKAUTO_TYPE        Bool
#define HAL_SRV_BKAUTO             HAL_SRV_BKAUTO_MASK
#define HAL_SRV_BKAUTO_READABLE    1
#define HAL_SRV_BKAUTO_WRITABLE    1
#define HAL_SRV_BKAUTO_SIGNED      0
#define HAL_SRV_BKAUTO_ADR         0xB00003AC

#define HAL_SRV_DFRQFG_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_DFRQFG_LSB         2
#define HAL_SRV_DFRQFG_MSB         2
#define HAL_SRV_DFRQFG_MASK        0x00000004
#define HAL_SRV_DFRQFG_TYPE        halSRV_DFRQFG_t
#define HAL_SRV_DFRQFG             HAL_SRV_DFRQFG_MASK
#define HAL_SRV_DFRQFG_READABLE    1
#define HAL_SRV_DFRQFG_WRITABLE    1
#define HAL_SRV_DFRQFG_SIGNED      0
#define HAL_SRV_DFRQFG_ADR         0xB00003AC

#define HAL_SRV_DFRQWBL_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_DFRQWBL_LSB         1
#define HAL_SRV_DFRQWBL_MSB         1
#define HAL_SRV_DFRQWBL_MASK        0x00000002
#define HAL_SRV_DFRQWBL_TYPE        halSRV_DFRQWBL_t
#define HAL_SRV_DFRQWBL             HAL_SRV_DFRQWBL_MASK
#define HAL_SRV_DFRQWBL_READABLE    1
#define HAL_SRV_DFRQWBL_WRITABLE    1
#define HAL_SRV_DFRQWBL_SIGNED      0
#define HAL_SRV_DFRQWBL_ADR         0xB00003AC

#define HAL_SRV_DFRQDEC_REGISTER    (HAL_SRVREG_MAP->DMCNT)
#define HAL_SRV_DFRQDEC_LSB         0
#define HAL_SRV_DFRQDEC_MSB         0
#define HAL_SRV_DFRQDEC_MASK        0x00000001
#define HAL_SRV_DFRQDEC_TYPE        halSRV_DFRQDEC_t
#define HAL_SRV_DFRQDEC             HAL_SRV_DFRQDEC_MASK
#define HAL_SRV_DFRQDEC_READABLE    1
#define HAL_SRV_DFRQDEC_WRITABLE    1
#define HAL_SRV_DFRQDEC_SIGNED      0
#define HAL_SRV_DFRQDEC_ADR         0xB00003AC

// Register DFCTSEL
#define HAL_SRV_DFCTTBL_REGISTER    (HAL_SRVREG_MAP->DFCTSEL)
#define HAL_SRV_DFCTTBL_LSB         4
#define HAL_SRV_DFCTTBL_MSB         5
#define HAL_SRV_DFCTTBL_MASK        0x00000030
#define HAL_SRV_DFCTTBL_TYPE        UByte
#define HAL_SRV_DFCTTBL             HAL_SRV_DFCTTBL_MASK
#define HAL_SRV_DFCTTBL_READABLE    1
#define HAL_SRV_DFCTTBL_WRITABLE    0
#define HAL_SRV_DFCTTBL_SIGNED      0
#define HAL_SRV_DFCTTBL_ADR         0xB00003B0

#define HAL_SRV_DFCTEN_REGISTER    (HAL_SRVREG_MAP->DFCTSEL)
#define HAL_SRV_DFCTEN_LSB         3
#define HAL_SRV_DFCTEN_MSB         3
#define HAL_SRV_DFCTEN_MASK        0x00000008
#define HAL_SRV_DFCTEN_TYPE        Bool
#define HAL_SRV_DFCTEN             HAL_SRV_DFCTEN_MASK
#define HAL_SRV_DFCTEN_READABLE    1
#define HAL_SRV_DFCTEN_WRITABLE    1
#define HAL_SRV_DFCTEN_SIGNED      0
#define HAL_SRV_DFCTEN_ADR         0xB00003B0

#define HAL_SRV_EDCFTEN_REGISTER    (HAL_SRVREG_MAP->DFCTSEL)
#define HAL_SRV_EDCFTEN_LSB         2
#define HAL_SRV_EDCFTEN_MSB         2
#define HAL_SRV_EDCFTEN_MASK        0x00000004
#define HAL_SRV_EDCFTEN_TYPE        Bool
#define HAL_SRV_EDCFTEN             HAL_SRV_EDCFTEN_MASK
#define HAL_SRV_EDCFTEN_READABLE    1
#define HAL_SRV_EDCFTEN_WRITABLE    1
#define HAL_SRV_EDCFTEN_SIGNED      0
#define HAL_SRV_EDCFTEN_ADR         0xB00003B0

#define HAL_SRV_DFCTOFF_REGISTER    (HAL_SRVREG_MAP->DFCTSEL)
#define HAL_SRV_DFCTOFF_LSB         1
#define HAL_SRV_DFCTOFF_MSB         1
#define HAL_SRV_DFCTOFF_MASK        0x00000002
#define HAL_SRV_DFCTOFF_TYPE        Bool
#define HAL_SRV_DFCTOFF             HAL_SRV_DFCTOFF_MASK
#define HAL_SRV_DFCTOFF_READABLE    1
#define HAL_SRV_DFCTOFF_WRITABLE    1
#define HAL_SRV_DFCTOFF_SIGNED      0
#define HAL_SRV_DFCTOFF_ADR         0xB00003B0

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_AFEDFCT_REGISTER    (HAL_SRVREG_MAP->DFCTSEL)
#define HAL_SRV_AFEDFCT_LSB         0
#define HAL_SRV_AFEDFCT_MSB         0
#define HAL_SRV_AFEDFCT_MASK        0x00000001
#define HAL_SRV_AFEDFCT_TYPE        halSRV_AFEDFCT_t
#define HAL_SRV_AFEDFCT             HAL_SRV_AFEDFCT_MASK
#define HAL_SRV_AFEDFCT_READABLE    1
#define HAL_SRV_AFEDFCT_WRITABLE    1
#define HAL_SRV_AFEDFCT_SIGNED      0
#define HAL_SRV_AFEDFCT_ADR         0xB00003B0

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_DEEPDFCT_REGISTER    (HAL_SRVREG_MAP->DFCTSEL)
#define HAL_SRV_DEEPDFCT_LSB         0
#define HAL_SRV_DEEPDFCT_MSB         0
#define HAL_SRV_DEEPDFCT_MASK        0x00000001
#define HAL_SRV_DEEPDFCT_TYPE        halSRV_DEEPDFCT_t
#define HAL_SRV_DEEPDFCT             HAL_SRV_DEEPDFCT_MASK
#define HAL_SRV_DEEPDFCT_READABLE    1
#define HAL_SRV_DEEPDFCT_WRITABLE    1
#define HAL_SRV_DEEPDFCT_SIGNED      0
#define HAL_SRV_DEEPDFCT_ADR         0xB00003B0

#endif // #if (CHIP_REV >= 0xC0)

// Register CNTRL
#define HAL_SRV_ZTK_REGISTER    (HAL_SRVREG_MAP->CNTRL)
#define HAL_SRV_ZTK_LSB         7
#define HAL_SRV_ZTK_MSB         7
#define HAL_SRV_ZTK_MASK        0x00000080
#define HAL_SRV_ZTK_TYPE        Bool
#define HAL_SRV_ZTK             HAL_SRV_ZTK_MASK
#define HAL_SRV_ZTK_READABLE    1
#define HAL_SRV_ZTK_WRITABLE    1
#define HAL_SRV_ZTK_SIGNED      0
#define HAL_SRV_ZTK_ADR         0xB00003B4

#define HAL_SRV_PGTK_REGISTER    (HAL_SRVREG_MAP->CNTRL)
#define HAL_SRV_PGTK_LSB         6
#define HAL_SRV_PGTK_MSB         6
#define HAL_SRV_PGTK_MASK        0x00000040
#define HAL_SRV_PGTK_TYPE        Bool
#define HAL_SRV_PGTK             HAL_SRV_PGTK_MASK
#define HAL_SRV_PGTK_READABLE    1
#define HAL_SRV_PGTK_WRITABLE    1
#define HAL_SRV_PGTK_SIGNED      0
#define HAL_SRV_PGTK_ADR         0xB00003B4

#define HAL_SRV_PLF_REGISTER    (HAL_SRVREG_MAP->CNTRL)
#define HAL_SRV_PLF_LSB         5
#define HAL_SRV_PLF_MSB         5
#define HAL_SRV_PLF_MASK        0x00000020
#define HAL_SRV_PLF_TYPE        Bool
#define HAL_SRV_PLF             HAL_SRV_PLF_MASK
#define HAL_SRV_PLF_READABLE    1
#define HAL_SRV_PLF_WRITABLE    1
#define HAL_SRV_PLF_SIGNED      0
#define HAL_SRV_PLF_ADR         0xB00003B4

#define HAL_SRV_RFP_REGISTER    (HAL_SRVREG_MAP->CNTRL)
#define HAL_SRV_RFP_LSB         4
#define HAL_SRV_RFP_MSB         4
#define HAL_SRV_RFP_MASK        0x00000010
#define HAL_SRV_RFP_TYPE        Bool
#define HAL_SRV_RFP             HAL_SRV_RFP_MASK
#define HAL_SRV_RFP_READABLE    1
#define HAL_SRV_RFP_WRITABLE    1
#define HAL_SRV_RFP_SIGNED      0
#define HAL_SRV_RFP_ADR         0xB00003B4

#define HAL_SRV_RXP_REGISTER    (HAL_SRVREG_MAP->CNTRL)
#define HAL_SRV_RXP_LSB         1
#define HAL_SRV_RXP_MSB         1
#define HAL_SRV_RXP_MASK        0x00000002
#define HAL_SRV_RXP_TYPE        Bool
#define HAL_SRV_RXP             HAL_SRV_RXP_MASK
#define HAL_SRV_RXP_READABLE    1
#define HAL_SRV_RXP_WRITABLE    1
#define HAL_SRV_RXP_SIGNED      0
#define HAL_SRV_RXP_ADR         0xB00003B4

#define HAL_SRV_TXP_REGISTER    (HAL_SRVREG_MAP->CNTRL)
#define HAL_SRV_TXP_LSB         0
#define HAL_SRV_TXP_MSB         0
#define HAL_SRV_TXP_MASK        0x00000001
#define HAL_SRV_TXP_TYPE        Bool
#define HAL_SRV_TXP             HAL_SRV_TXP_MASK
#define HAL_SRV_TXP_READABLE    1
#define HAL_SRV_TXP_WRITABLE    1
#define HAL_SRV_TXP_SIGNED      0
#define HAL_SRV_TXP_ADR         0xB00003B4

// Register DVDRHLD
#define HAL_SRV_DVDRHHLD_REGISTER    (HAL_SRVREG_MAP->DVDRHLD)
#define HAL_SRV_DVDRHHLD_LSB         0
#define HAL_SRV_DVDRHHLD_MSB         0
#define HAL_SRV_DVDRHHLD_MASK        0x00000001
#define HAL_SRV_DVDRHHLD_TYPE        Bool
#define HAL_SRV_DVDRHHLD             HAL_SRV_DVDRHHLD_MASK
#define HAL_SRV_DVDRHHLD_READABLE    1
#define HAL_SRV_DVDRHHLD_WRITABLE    1
#define HAL_SRV_DVDRHHLD_SIGNED      0
#define HAL_SRV_DVDRHHLD_ADR         0xB00003B8

// Register CDRWSEEK
#define HAL_SRV_WGAINUP_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_WGAINUP_LSB         8
#define HAL_SRV_WGAINUP_MSB         8
#define HAL_SRV_WGAINUP_MASK        0x00000100
#define HAL_SRV_WGAINUP_TYPE        Bool
#define HAL_SRV_WGAINUP             HAL_SRV_WGAINUP_MASK
#define HAL_SRV_WGAINUP_READABLE    1
#define HAL_SRV_WGAINUP_WRITABLE    1
#define HAL_SRV_WGAINUP_SIGNED      0
#define HAL_SRV_WGAINUP_ADR         0xB00003BC

#define HAL_SRV_WRCOEF_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_WRCOEF_LSB         7
#define HAL_SRV_WRCOEF_MSB         7
#define HAL_SRV_WRCOEF_MASK        0x00000080
#define HAL_SRV_WRCOEF_TYPE        Bool
#define HAL_SRV_WRCOEF             HAL_SRV_WRCOEF_MASK
#define HAL_SRV_WRCOEF_READABLE    1
#define HAL_SRV_WRCOEF_WRITABLE    1
#define HAL_SRV_WRCOEF_SIGNED      0
#define HAL_SRV_WRCOEF_ADR         0xB00003BC

#define HAL_SRV_STLFRQ_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_STLFRQ_LSB         6
#define HAL_SRV_STLFRQ_MSB         6
#define HAL_SRV_STLFRQ_MASK        0x00000040
#define HAL_SRV_STLFRQ_TYPE        Bool
#define HAL_SRV_STLFRQ             HAL_SRV_STLFRQ_MASK
#define HAL_SRV_STLFRQ_READABLE    1
#define HAL_SRV_STLFRQ_WRITABLE    1
#define HAL_SRV_STLFRQ_SIGNED      0
#define HAL_SRV_STLFRQ_ADR         0xB00003BC

#define HAL_SRV_STLCTL_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_STLCTL_LSB         5
#define HAL_SRV_STLCTL_MSB         5
#define HAL_SRV_STLCTL_MASK        0x00000020
#define HAL_SRV_STLCTL_TYPE        Bool
#define HAL_SRV_STLCTL             HAL_SRV_STLCTL_MASK
#define HAL_SRV_STLCTL_READABLE    1
#define HAL_SRV_STLCTL_WRITABLE    1
#define HAL_SRV_STLCTL_SIGNED      0
#define HAL_SRV_STLCTL_ADR         0xB00003BC

#define HAL_SRV_BFILT_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_BFILT_LSB         4
#define HAL_SRV_BFILT_MSB         4
#define HAL_SRV_BFILT_MASK        0x00000010
#define HAL_SRV_BFILT_TYPE        halSRV_BFILT_t
#define HAL_SRV_BFILT             HAL_SRV_BFILT_MASK
#define HAL_SRV_BFILT_READABLE    1
#define HAL_SRV_BFILT_WRITABLE    1
#define HAL_SRV_BFILT_SIGNED      0
#define HAL_SRV_BFILT_ADR         0xB00003BC

#define HAL_SRV_FAKERX_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_FAKERX_LSB         3
#define HAL_SRV_FAKERX_MSB         3
#define HAL_SRV_FAKERX_MASK        0x00000008
#define HAL_SRV_FAKERX_TYPE        halSRV_FAKERX_t
#define HAL_SRV_FAKERX             HAL_SRV_FAKERX_MASK
#define HAL_SRV_FAKERX_READABLE    1
#define HAL_SRV_FAKERX_WRITABLE    1
#define HAL_SRV_FAKERX_SIGNED      0
#define HAL_SRV_FAKERX_ADR         0xB00003BC

#define HAL_SRV_DIRAUT_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_DIRAUT_LSB         2
#define HAL_SRV_DIRAUT_MSB         2
#define HAL_SRV_DIRAUT_MASK        0x00000004
#define HAL_SRV_DIRAUT_TYPE        Bool
#define HAL_SRV_DIRAUT             HAL_SRV_DIRAUT_MASK
#define HAL_SRV_DIRAUT_READABLE    1
#define HAL_SRV_DIRAUT_WRITABLE    1
#define HAL_SRV_DIRAUT_SIGNED      0
#define HAL_SRV_DIRAUT_ADR         0xB00003BC

#define HAL_SRV_DIRPOL_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_DIRPOL_LSB         1
#define HAL_SRV_DIRPOL_MSB         1
#define HAL_SRV_DIRPOL_MASK        0x00000002
#define HAL_SRV_DIRPOL_TYPE        Bool
#define HAL_SRV_DIRPOL             HAL_SRV_DIRPOL_MASK
#define HAL_SRV_DIRPOL_READABLE    1
#define HAL_SRV_DIRPOL_WRITABLE    1
#define HAL_SRV_DIRPOL_SIGNED      0
#define HAL_SRV_DIRPOL_ADR         0xB00003BC

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_FAKETX_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_FAKETX_LSB         0
#define HAL_SRV_FAKETX_MSB         0
#define HAL_SRV_FAKETX_MASK        0x00000001
#define HAL_SRV_FAKETX_TYPE        Bool
#define HAL_SRV_FAKETX             HAL_SRV_FAKETX_MASK
#define HAL_SRV_FAKETX_READABLE    1
#define HAL_SRV_FAKETX_WRITABLE    1
#define HAL_SRV_FAKETX_SIGNED      0
#define HAL_SRV_FAKETX_ADR         0xB00003BC

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_TXRXFILT_REGISTER    (HAL_SRVREG_MAP->CDRWSEEK)
#define HAL_SRV_TXRXFILT_LSB         0
#define HAL_SRV_TXRXFILT_MSB         0
#define HAL_SRV_TXRXFILT_MASK        0x00000001
#define HAL_SRV_TXRXFILT_TYPE        Bool
#define HAL_SRV_TXRXFILT             HAL_SRV_TXRXFILT_MASK
#define HAL_SRV_TXRXFILT_READABLE    1
#define HAL_SRV_TXRXFILT_WRITABLE    1
#define HAL_SRV_TXRXFILT_SIGNED      0
#define HAL_SRV_TXRXFILT_ADR         0xB00003BC

#endif // #if (CHIP_REV >= 0xC0)

// Register FOCUS1
#define HAL_SRV_FOKL_REGISTER    (HAL_SRVREG_MAP->FOCUS1)
#define HAL_SRV_FOKL_LSB         0
#define HAL_SRV_FOKL_MSB         6
#define HAL_SRV_FOKL_MASK        0x0000007F
#define HAL_SRV_FOKL_TYPE        halSRV_FOKL_t
#define HAL_SRV_FOKL             HAL_SRV_FOKL_MASK
#define HAL_SRV_FOKL_READABLE    1
#define HAL_SRV_FOKL_WRITABLE    1
#define HAL_SRV_FOKL_SIGNED      0
#define HAL_SRV_FOKL_ADR         0xB00003D0

// Register SRVTEST1
#define HAL_SRV_SHTTST_REGISTER    (HAL_SRVREG_MAP->SRVTEST1)
#define HAL_SRV_SHTTST_LSB         7
#define HAL_SRV_SHTTST_MSB         7
#define HAL_SRV_SHTTST_MASK        0x00000080
#define HAL_SRV_SHTTST_TYPE        Bool
#define HAL_SRV_SHTTST             HAL_SRV_SHTTST_MASK
#define HAL_SRV_SHTTST_READABLE    1
#define HAL_SRV_SHTTST_WRITABLE    1
#define HAL_SRV_SHTTST_SIGNED      0
#define HAL_SRV_SHTTST_ADR         0xB00003D4

#define HAL_SRV_TMD_REGISTER    (HAL_SRVREG_MAP->SRVTEST1)
#define HAL_SRV_TMD_LSB         4
#define HAL_SRV_TMD_MSB         5
#define HAL_SRV_TMD_MASK        0x00000030
#define HAL_SRV_TMD_TYPE        halSRV_TMD_t
#define HAL_SRV_TMD             HAL_SRV_TMD_MASK
#define HAL_SRV_TMD_READABLE    1
#define HAL_SRV_TMD_WRITABLE    1
#define HAL_SRV_TMD_SIGNED      0
#define HAL_SRV_TMD_ADR         0xB00003D4

#define HAL_SRV_TST_REGISTER    (HAL_SRVREG_MAP->SRVTEST1)
#define HAL_SRV_TST_LSB         0
#define HAL_SRV_TST_MSB         3
#define HAL_SRV_TST_MASK        0x0000000F
#define HAL_SRV_TST_TYPE        UByte
#define HAL_SRV_TST             HAL_SRV_TST_MASK
#define HAL_SRV_TST_READABLE    1
#define HAL_SRV_TST_WRITABLE    1
#define HAL_SRV_TST_SIGNED      0
#define HAL_SRV_TST_ADR         0xB00003D4

// Register SRVTEST2
#define HAL_SRV_TSTEN_REGISTER    (HAL_SRVREG_MAP->SRVTEST2)
#define HAL_SRV_TSTEN_LSB         7
#define HAL_SRV_TSTEN_MSB         7
#define HAL_SRV_TSTEN_MASK        0x00000080
#define HAL_SRV_TSTEN_TYPE        Bool
#define HAL_SRV_TSTEN             HAL_SRV_TSTEN_MASK
#define HAL_SRV_TSTEN_READABLE    1
#define HAL_SRV_TSTEN_WRITABLE    1
#define HAL_SRV_TSTEN_SIGNED      0
#define HAL_SRV_TSTEN_ADR         0xB00003D8

#define HAL_SRV_OV220T_REGISTER    (HAL_SRVREG_MAP->SRVTEST2)
#define HAL_SRV_OV220T_LSB         5
#define HAL_SRV_OV220T_MSB         5
#define HAL_SRV_OV220T_MASK        0x00000020
#define HAL_SRV_OV220T_TYPE        Bool
#define HAL_SRV_OV220T             HAL_SRV_OV220T_MASK
#define HAL_SRV_OV220T_READABLE    1
#define HAL_SRV_OV220T_WRITABLE    1
#define HAL_SRV_OV220T_SIGNED      0
#define HAL_SRV_OV220T_ADR         0xB00003D8

// Register SDINTEN

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_SPMUXIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_SPMUXIEN_LSB         13
#define HAL_SRV_SPMUXIEN_MSB         13
#define HAL_SRV_SPMUXIEN_MASK        0x00002000
#define HAL_SRV_SPMUXIEN_TYPE        Bool
#define HAL_SRV_SPMUXIEN             HAL_SRV_SPMUXIEN_MASK
#define HAL_SRV_SPMUXIEN_READABLE    1
#define HAL_SRV_SPMUXIEN_WRITABLE    1
#define HAL_SRV_SPMUXIEN_SIGNED      0
#define HAL_SRV_SPMUXIEN_ADR         0xB00003E0

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_SYNCIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_SYNCIEN_LSB         12
#define HAL_SRV_SYNCIEN_MSB         12
#define HAL_SRV_SYNCIEN_MASK        0x00001000
#define HAL_SRV_SYNCIEN_TYPE        Bool
#define HAL_SRV_SYNCIEN             HAL_SRV_SYNCIEN_MASK
#define HAL_SRV_SYNCIEN_READABLE    1
#define HAL_SRV_SYNCIEN_WRITABLE    1
#define HAL_SRV_SYNCIEN_SIGNED      0
#define HAL_SRV_SYNCIEN_ADR         0xB00003E0

#define HAL_SRV_FETHRIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_FETHRIEN_LSB         11
#define HAL_SRV_FETHRIEN_MSB         11
#define HAL_SRV_FETHRIEN_MASK        0x00000800
#define HAL_SRV_FETHRIEN_TYPE        Bool
#define HAL_SRV_FETHRIEN             HAL_SRV_FETHRIEN_MASK
#define HAL_SRV_FETHRIEN_READABLE    1
#define HAL_SRV_FETHRIEN_WRITABLE    1
#define HAL_SRV_FETHRIEN_SIGNED      0
#define HAL_SRV_FETHRIEN_ADR         0xB00003E0

#define HAL_SRV_FE2OSCIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_FE2OSCIEN_LSB         10
#define HAL_SRV_FE2OSCIEN_MSB         10
#define HAL_SRV_FE2OSCIEN_MASK        0x00000400
#define HAL_SRV_FE2OSCIEN_TYPE        Bool
#define HAL_SRV_FE2OSCIEN             HAL_SRV_FE2OSCIEN_MASK
#define HAL_SRV_FE2OSCIEN_READABLE    1
#define HAL_SRV_FE2OSCIEN_WRITABLE    1
#define HAL_SRV_FE2OSCIEN_SIGNED      0
#define HAL_SRV_FE2OSCIEN_ADR         0xB00003E0

#define HAL_SRV_FE1OSCIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_FE1OSCIEN_LSB         9
#define HAL_SRV_FE1OSCIEN_MSB         9
#define HAL_SRV_FE1OSCIEN_MASK        0x00000200
#define HAL_SRV_FE1OSCIEN_TYPE        Bool
#define HAL_SRV_FE1OSCIEN             HAL_SRV_FE1OSCIEN_MASK
#define HAL_SRV_FE1OSCIEN_READABLE    1
#define HAL_SRV_FE1OSCIEN_WRITABLE    1
#define HAL_SRV_FE1OSCIEN_SIGNED      0
#define HAL_SRV_FE1OSCIEN_ADR         0xB00003E0

#define HAL_SRV_TEOSCIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_TEOSCIEN_LSB         8
#define HAL_SRV_TEOSCIEN_MSB         8
#define HAL_SRV_TEOSCIEN_MASK        0x00000100
#define HAL_SRV_TEOSCIEN_TYPE        Bool
#define HAL_SRV_TEOSCIEN             HAL_SRV_TEOSCIEN_MASK
#define HAL_SRV_TEOSCIEN_READABLE    1
#define HAL_SRV_TEOSCIEN_WRITABLE    1
#define HAL_SRV_TEOSCIEN_SIGNED      0
#define HAL_SRV_TEOSCIEN_ADR         0xB00003E0

#define HAL_SRV_AWEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_AWEN_LSB         7
#define HAL_SRV_AWEN_MSB         7
#define HAL_SRV_AWEN_MASK        0x00000080
#define HAL_SRV_AWEN_TYPE        Bool
#define HAL_SRV_AWEN             HAL_SRV_AWEN_MASK
#define HAL_SRV_AWEN_READABLE    1
#define HAL_SRV_AWEN_WRITABLE    1
#define HAL_SRV_AWEN_SIGNED      0
#define HAL_SRV_AWEN_ADR         0xB00003E0

#define HAL_SRV_MODCHGEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_MODCHGEN_LSB         6
#define HAL_SRV_MODCHGEN_MSB         6
#define HAL_SRV_MODCHGEN_MASK        0x00000040
#define HAL_SRV_MODCHGEN_TYPE        Bool
#define HAL_SRV_MODCHGEN             HAL_SRV_MODCHGEN_MASK
#define HAL_SRV_MODCHGEN_READABLE    1
#define HAL_SRV_MODCHGEN_WRITABLE    1
#define HAL_SRV_MODCHGEN_SIGNED      0
#define HAL_SRV_MODCHGEN_ADR         0xB00003E0

#define HAL_SRV_PANICIEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_PANICIEN_LSB         5
#define HAL_SRV_PANICIEN_MSB         5
#define HAL_SRV_PANICIEN_MASK        0x00000020
#define HAL_SRV_PANICIEN_TYPE        Bool
#define HAL_SRV_PANICIEN             HAL_SRV_PANICIEN_MASK
#define HAL_SRV_PANICIEN_READABLE    1
#define HAL_SRV_PANICIEN_WRITABLE    1
#define HAL_SRV_PANICIEN_SIGNED      0
#define HAL_SRV_PANICIEN_ADR         0xB00003E0

#define HAL_SRV_FSHCEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_FSHCEN_LSB         4
#define HAL_SRV_FSHCEN_MSB         4
#define HAL_SRV_FSHCEN_MASK        0x00000010
#define HAL_SRV_FSHCEN_TYPE        Bool
#define HAL_SRV_FSHCEN             HAL_SRV_FSHCEN_MASK
#define HAL_SRV_FSHCEN_READABLE    1
#define HAL_SRV_FSHCEN_WRITABLE    1
#define HAL_SRV_FSHCEN_SIGNED      0
#define HAL_SRV_FSHCEN_ADR         0xB00003E0

#define HAL_SRV_SEEKCEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_SEEKCEN_LSB         3
#define HAL_SRV_SEEKCEN_MSB         3
#define HAL_SRV_SEEKCEN_MASK        0x00000008
#define HAL_SRV_SEEKCEN_TYPE        Bool
#define HAL_SRV_SEEKCEN             HAL_SRV_SEEKCEN_MASK
#define HAL_SRV_SEEKCEN_READABLE    1
#define HAL_SRV_SEEKCEN_WRITABLE    1
#define HAL_SRV_SEEKCEN_SIGNED      0
#define HAL_SRV_SEEKCEN_ADR         0xB00003E0

#define HAL_SRV_MIRREN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_MIRREN_LSB         2
#define HAL_SRV_MIRREN_MSB         2
#define HAL_SRV_MIRREN_MASK        0x00000004
#define HAL_SRV_MIRREN_TYPE        Bool
#define HAL_SRV_MIRREN             HAL_SRV_MIRREN_MASK
#define HAL_SRV_MIRREN_READABLE    1
#define HAL_SRV_MIRREN_WRITABLE    1
#define HAL_SRV_MIRREN_SIGNED      0
#define HAL_SRV_MIRREN_ADR         0xB00003E0

#define HAL_SRV_FNOKEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_FNOKEN_LSB         1
#define HAL_SRV_FNOKEN_MSB         1
#define HAL_SRV_FNOKEN_MASK        0x00000002
#define HAL_SRV_FNOKEN_TYPE        Bool
#define HAL_SRV_FNOKEN             HAL_SRV_FNOKEN_MASK
#define HAL_SRV_FNOKEN_READABLE    1
#define HAL_SRV_FNOKEN_WRITABLE    1
#define HAL_SRV_FNOKEN_SIGNED      0
#define HAL_SRV_FNOKEN_ADR         0xB00003E0

#define HAL_SRV_DLINTEN_REGISTER    (HAL_SRVREG_MAP->SDINTEN)
#define HAL_SRV_DLINTEN_LSB         0
#define HAL_SRV_DLINTEN_MSB         0
#define HAL_SRV_DLINTEN_MASK        0x00000001
#define HAL_SRV_DLINTEN_TYPE        Bool
#define HAL_SRV_DLINTEN             HAL_SRV_DLINTEN_MASK
#define HAL_SRV_DLINTEN_READABLE    1
#define HAL_SRV_DLINTEN_WRITABLE    1
#define HAL_SRV_DLINTEN_SIGNED      0
#define HAL_SRV_DLINTEN_ADR         0xB00003E0

// Register SDINT

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_SPMUXI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_SPMUXI_LSB         13
#define HAL_SRV_SPMUXI_MSB         13
#define HAL_SRV_SPMUXI_MASK        0x00002000
#define HAL_SRV_SPMUXI_TYPE        Bool
#define HAL_SRV_SPMUXI             HAL_SRV_SPMUXI_MASK
#define HAL_SRV_SPMUXI_READABLE    1
#define HAL_SRV_SPMUXI_WRITABLE    1
#define HAL_SRV_SPMUXI_SIGNED      0
#define HAL_SRV_SPMUXI_ADR         0xB00003E8

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_SYNCI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_SYNCI_LSB         12
#define HAL_SRV_SYNCI_MSB         12
#define HAL_SRV_SYNCI_MASK        0x00001000
#define HAL_SRV_SYNCI_TYPE        Bool
#define HAL_SRV_SYNCI             HAL_SRV_SYNCI_MASK
#define HAL_SRV_SYNCI_READABLE    1
#define HAL_SRV_SYNCI_WRITABLE    1
#define HAL_SRV_SYNCI_SIGNED      0
#define HAL_SRV_SYNCI_ADR         0xB00003E8

#define HAL_SRV_FETHRI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_FETHRI_LSB         11
#define HAL_SRV_FETHRI_MSB         11
#define HAL_SRV_FETHRI_MASK        0x00000800
#define HAL_SRV_FETHRI_TYPE        Bool
#define HAL_SRV_FETHRI             HAL_SRV_FETHRI_MASK
#define HAL_SRV_FETHRI_READABLE    1
#define HAL_SRV_FETHRI_WRITABLE    1
#define HAL_SRV_FETHRI_SIGNED      0
#define HAL_SRV_FETHRI_ADR         0xB00003E8

#define HAL_SRV_FE2OSCI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_FE2OSCI_LSB         10
#define HAL_SRV_FE2OSCI_MSB         10
#define HAL_SRV_FE2OSCI_MASK        0x00000400
#define HAL_SRV_FE2OSCI_TYPE        Bool
#define HAL_SRV_FE2OSCI             HAL_SRV_FE2OSCI_MASK
#define HAL_SRV_FE2OSCI_READABLE    1
#define HAL_SRV_FE2OSCI_WRITABLE    1
#define HAL_SRV_FE2OSCI_SIGNED      0
#define HAL_SRV_FE2OSCI_ADR         0xB00003E8

#define HAL_SRV_FE1OSCI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_FE1OSCI_LSB         9
#define HAL_SRV_FE1OSCI_MSB         9
#define HAL_SRV_FE1OSCI_MASK        0x00000200
#define HAL_SRV_FE1OSCI_TYPE        Bool
#define HAL_SRV_FE1OSCI             HAL_SRV_FE1OSCI_MASK
#define HAL_SRV_FE1OSCI_READABLE    1
#define HAL_SRV_FE1OSCI_WRITABLE    1
#define HAL_SRV_FE1OSCI_SIGNED      0
#define HAL_SRV_FE1OSCI_ADR         0xB00003E8

#define HAL_SRV_TEOSCI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_TEOSCI_LSB         8
#define HAL_SRV_TEOSCI_MSB         8
#define HAL_SRV_TEOSCI_MASK        0x00000100
#define HAL_SRV_TEOSCI_TYPE        Bool
#define HAL_SRV_TEOSCI             HAL_SRV_TEOSCI_MASK
#define HAL_SRV_TEOSCI_READABLE    1
#define HAL_SRV_TEOSCI_WRITABLE    1
#define HAL_SRV_TEOSCI_SIGNED      0
#define HAL_SRV_TEOSCI_ADR         0xB00003E8

#define HAL_SRV_AWI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_AWI_LSB         7
#define HAL_SRV_AWI_MSB         7
#define HAL_SRV_AWI_MASK        0x00000080
#define HAL_SRV_AWI_TYPE        Bool
#define HAL_SRV_AWI             HAL_SRV_AWI_MASK
#define HAL_SRV_AWI_READABLE    1
#define HAL_SRV_AWI_WRITABLE    1
#define HAL_SRV_AWI_SIGNED      0
#define HAL_SRV_AWI_ADR         0xB00003E8

#define HAL_SRV_MODCHGI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_MODCHGI_LSB         6
#define HAL_SRV_MODCHGI_MSB         6
#define HAL_SRV_MODCHGI_MASK        0x00000040
#define HAL_SRV_MODCHGI_TYPE        Bool
#define HAL_SRV_MODCHGI             HAL_SRV_MODCHGI_MASK
#define HAL_SRV_MODCHGI_READABLE    1
#define HAL_SRV_MODCHGI_WRITABLE    1
#define HAL_SRV_MODCHGI_SIGNED      0
#define HAL_SRV_MODCHGI_ADR         0xB00003E8

#define HAL_SRV_PANICI_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_PANICI_LSB         5
#define HAL_SRV_PANICI_MSB         5
#define HAL_SRV_PANICI_MASK        0x00000020
#define HAL_SRV_PANICI_TYPE        Bool
#define HAL_SRV_PANICI             HAL_SRV_PANICI_MASK
#define HAL_SRV_PANICI_READABLE    1
#define HAL_SRV_PANICI_WRITABLE    1
#define HAL_SRV_PANICI_SIGNED      0
#define HAL_SRV_PANICI_ADR         0xB00003E8

#define HAL_SRV_FSHC_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_FSHC_LSB         4
#define HAL_SRV_FSHC_MSB         4
#define HAL_SRV_FSHC_MASK        0x00000010
#define HAL_SRV_FSHC_TYPE        Bool
#define HAL_SRV_FSHC             HAL_SRV_FSHC_MASK
#define HAL_SRV_FSHC_READABLE    1
#define HAL_SRV_FSHC_WRITABLE    1
#define HAL_SRV_FSHC_SIGNED      0
#define HAL_SRV_FSHC_ADR         0xB00003E8

#define HAL_SRV_SEEKC_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_SEEKC_LSB         3
#define HAL_SRV_SEEKC_MSB         3
#define HAL_SRV_SEEKC_MASK        0x00000008
#define HAL_SRV_SEEKC_TYPE        Bool
#define HAL_SRV_SEEKC             HAL_SRV_SEEKC_MASK
#define HAL_SRV_SEEKC_READABLE    1
#define HAL_SRV_SEEKC_WRITABLE    1
#define HAL_SRV_SEEKC_SIGNED      0
#define HAL_SRV_SEEKC_ADR         0xB00003E8

#define HAL_SRV_MIRR_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_MIRR_LSB         2
#define HAL_SRV_MIRR_MSB         2
#define HAL_SRV_MIRR_MASK        0x00000004
#define HAL_SRV_MIRR_TYPE        Bool
#define HAL_SRV_MIRR             HAL_SRV_MIRR_MASK
#define HAL_SRV_MIRR_READABLE    1
#define HAL_SRV_MIRR_WRITABLE    1
#define HAL_SRV_MIRR_SIGNED      0
#define HAL_SRV_MIRR_ADR         0xB00003E8

#define HAL_SRV_FNOK_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_FNOK_LSB         1
#define HAL_SRV_FNOK_MSB         1
#define HAL_SRV_FNOK_MASK        0x00000002
#define HAL_SRV_FNOK_TYPE        Bool
#define HAL_SRV_FNOK             HAL_SRV_FNOK_MASK
#define HAL_SRV_FNOK_READABLE    1
#define HAL_SRV_FNOK_WRITABLE    1
#define HAL_SRV_FNOK_SIGNED      0
#define HAL_SRV_FNOK_ADR         0xB00003E8

#define HAL_SRV_DLINTR_REGISTER    (HAL_SRVREG_MAP->SDINT)
#define HAL_SRV_DLINTR_LSB         0
#define HAL_SRV_DLINTR_MSB         0
#define HAL_SRV_DLINTR_MASK        0x00000001
#define HAL_SRV_DLINTR_TYPE        Bool
#define HAL_SRV_DLINTR             HAL_SRV_DLINTR_MASK
#define HAL_SRV_DLINTR_READABLE    1
#define HAL_SRV_DLINTR_WRITABLE    1
#define HAL_SRV_DLINTR_SIGNED      0
#define HAL_SRV_DLINTR_ADR         0xB00003E8

// Register SVSPARE

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_DLFOOSW_REGISTER    (HAL_SRVREG_MAP->SVSPARE)
#define HAL_SRV_DLFOOSW_LSB         0
#define HAL_SRV_DLFOOSW_MSB         0
#define HAL_SRV_DLFOOSW_MASK        0x00000001
#define HAL_SRV_DLFOOSW_TYPE        Bool
#define HAL_SRV_DLFOOSW             HAL_SRV_DLFOOSW_MASK
#define HAL_SRV_DLFOOSW_READABLE    1
#define HAL_SRV_DLFOOSW_WRITABLE    1
#define HAL_SRV_DLFOOSW_SIGNED      0
#define HAL_SRV_DLFOOSW_ADR         0xB00003F8

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC1)

#define HAL_SRV_FGWAROUND3_REGISTER    (HAL_SRVREG_MAP->SVSPARE)
#define HAL_SRV_FGWAROUND3_LSB         4
#define HAL_SRV_FGWAROUND3_MSB         4
#define HAL_SRV_FGWAROUND3_MASK        0x00000010
#define HAL_SRV_FGWAROUND3_TYPE        Bool
#define HAL_SRV_FGWAROUND3             HAL_SRV_FGWAROUND3_MASK
#define HAL_SRV_FGWAROUND3_READABLE    1
#define HAL_SRV_FGWAROUND3_WRITABLE    1
#define HAL_SRV_FGWAROUND3_SIGNED      0
#define HAL_SRV_FGWAROUND3_ADR         0xB00003F8

#define HAL_SRV_FGWAROUND2_REGISTER    (HAL_SRVREG_MAP->SVSPARE)
#define HAL_SRV_FGWAROUND2_LSB         3
#define HAL_SRV_FGWAROUND2_MSB         3
#define HAL_SRV_FGWAROUND2_MASK        0x00000008
#define HAL_SRV_FGWAROUND2_TYPE        Bool
#define HAL_SRV_FGWAROUND2             HAL_SRV_FGWAROUND2_MASK
#define HAL_SRV_FGWAROUND2_READABLE    1
#define HAL_SRV_FGWAROUND2_WRITABLE    1
#define HAL_SRV_FGWAROUND2_SIGNED      0
#define HAL_SRV_FGWAROUND2_ADR         0xB00003F8

#define HAL_SRV_FGWAROUND1_REGISTER    (HAL_SRVREG_MAP->SVSPARE)
#define HAL_SRV_FGWAROUND1_LSB         2
#define HAL_SRV_FGWAROUND1_MSB         2
#define HAL_SRV_FGWAROUND1_MASK        0x00000004
#define HAL_SRV_FGWAROUND1_TYPE        Bool
#define HAL_SRV_FGWAROUND1             HAL_SRV_FGWAROUND1_MASK
#define HAL_SRV_FGWAROUND1_READABLE    1
#define HAL_SRV_FGWAROUND1_WRITABLE    1
#define HAL_SRV_FGWAROUND1_SIGNED      0
#define HAL_SRV_FGWAROUND1_ADR         0xB00003F8

#define HAL_SRV_FELDCLKSW_REGISTER    (HAL_SRVREG_MAP->SVSPARE)
#define HAL_SRV_FELDCLKSW_LSB         0
#define HAL_SRV_FELDCLKSW_MSB         1
#define HAL_SRV_FELDCLKSW_MASK        0x00000003
#define HAL_SRV_FELDCLKSW_TYPE        UByte
#define HAL_SRV_FELDCLKSW             HAL_SRV_FELDCLKSW_MASK
#define HAL_SRV_FELDCLKSW_READABLE    1
#define HAL_SRV_FELDCLKSW_WRITABLE    1
#define HAL_SRV_FELDCLKSW_SIGNED      0
#define HAL_SRV_FELDCLKSW_ADR         0xB00003F8

#endif // #if (CHIP_REV >= 0xC1)

// Register LCCTL
#define HAL_SRV_ACTDSSRST_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_ACTDSSRST_LSB         22
#define HAL_SRV_ACTDSSRST_MSB         22
#define HAL_SRV_ACTDSSRST_MASK        0x00400000
#define HAL_SRV_ACTDSSRST_TYPE        Bool
#define HAL_SRV_ACTDSSRST             HAL_SRV_ACTDSSRST_MASK
#define HAL_SRV_ACTDSSRST_READABLE    1
#define HAL_SRV_ACTDSSRST_WRITABLE    1
#define HAL_SRV_ACTDSSRST_SIGNED      0
#define HAL_SRV_ACTDSSRST_ADR         0xB0000400

#define HAL_SRV_LCTSTIRQ_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LCTSTIRQ_LSB         21
#define HAL_SRV_LCTSTIRQ_MSB         21
#define HAL_SRV_LCTSTIRQ_MASK        0x00200000
#define HAL_SRV_LCTSTIRQ_TYPE        Bool
#define HAL_SRV_LCTSTIRQ             HAL_SRV_LCTSTIRQ_MASK
#define HAL_SRV_LCTSTIRQ_READABLE    1
#define HAL_SRV_LCTSTIRQ_WRITABLE    1
#define HAL_SRV_LCTSTIRQ_SIGNED      0
#define HAL_SRV_LCTSTIRQ_ADR         0xB0000400

#define HAL_SRV_LDGRSEL_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LDGRSEL_LSB         20
#define HAL_SRV_LDGRSEL_MSB         20
#define HAL_SRV_LDGRSEL_MASK        0x00100000
#define HAL_SRV_LDGRSEL_TYPE        Bool
#define HAL_SRV_LDGRSEL             HAL_SRV_LDGRSEL_MASK
#define HAL_SRV_LDGRSEL_READABLE    1
#define HAL_SRV_LDGRSEL_WRITABLE    1
#define HAL_SRV_LDGRSEL_SIGNED      0
#define HAL_SRV_LDGRSEL_ADR         0xB0000400

#define HAL_SRV_LDGRPRESET_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LDGRPRESET_LSB         19
#define HAL_SRV_LDGRPRESET_MSB         19
#define HAL_SRV_LDGRPRESET_MASK        0x00080000
#define HAL_SRV_LDGRPRESET_TYPE        Bool
#define HAL_SRV_LDGRPRESET             HAL_SRV_LDGRPRESET_MASK
#define HAL_SRV_LDGRPRESET_READABLE    1
#define HAL_SRV_LDGRPRESET_WRITABLE    1
#define HAL_SRV_LDGRPRESET_SIGNED      0
#define HAL_SRV_LDGRPRESET_ADR         0xB0000400

#define HAL_SRV_WGATESEL_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_WGATESEL_LSB         18
#define HAL_SRV_WGATESEL_MSB         18
#define HAL_SRV_WGATESEL_MASK        0x00040000
#define HAL_SRV_WGATESEL_TYPE        Bool
#define HAL_SRV_WGATESEL             HAL_SRV_WGATESEL_MASK
#define HAL_SRV_WGATESEL_READABLE    1
#define HAL_SRV_WGATESEL_WRITABLE    1
#define HAL_SRV_WGATESEL_SIGNED      0
#define HAL_SRV_WGATESEL_ADR         0xB0000400

#define HAL_SRV_WGPRESET_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_WGPRESET_LSB         17
#define HAL_SRV_WGPRESET_MSB         17
#define HAL_SRV_WGPRESET_MASK        0x00020000
#define HAL_SRV_WGPRESET_TYPE        Bool
#define HAL_SRV_WGPRESET             HAL_SRV_WGPRESET_MASK
#define HAL_SRV_WGPRESET_READABLE    1
#define HAL_SRV_WGPRESET_WRITABLE    1
#define HAL_SRV_WGPRESET_SIGNED      0
#define HAL_SRV_WGPRESET_ADR         0xB0000400

#define HAL_SRV_TTWEN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_TTWEN_LSB         16
#define HAL_SRV_TTWEN_MSB         16
#define HAL_SRV_TTWEN_MASK        0x00010000
#define HAL_SRV_TTWEN_TYPE        Bool
#define HAL_SRV_TTWEN             HAL_SRV_TTWEN_MASK
#define HAL_SRV_TTWEN_READABLE    1
#define HAL_SRV_TTWEN_WRITABLE    1
#define HAL_SRV_TTWEN_SIGNED      0
#define HAL_SRV_TTWEN_ADR         0xB0000400

#if (CHIP_REV <= 0xB2)

#define HAL_SRV_APCFNEN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_APCFNEN_LSB         15
#define HAL_SRV_APCFNEN_MSB         15
#define HAL_SRV_APCFNEN_MASK        0x00008000
#define HAL_SRV_APCFNEN_TYPE        Bool
#define HAL_SRV_APCFNEN             HAL_SRV_APCFNEN_MASK
#define HAL_SRV_APCFNEN_READABLE    1
#define HAL_SRV_APCFNEN_WRITABLE    1
#define HAL_SRV_APCFNEN_SIGNED      0
#define HAL_SRV_APCFNEN_ADR         0xB0000400

#define HAL_SRV_HDRSTREN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_HDRSTREN_LSB         14
#define HAL_SRV_HDRSTREN_MSB         14
#define HAL_SRV_HDRSTREN_MASK        0x00004000
#define HAL_SRV_HDRSTREN_TYPE        Bool
#define HAL_SRV_HDRSTREN             HAL_SRV_HDRSTREN_MASK
#define HAL_SRV_HDRSTREN_READABLE    1
#define HAL_SRV_HDRSTREN_WRITABLE    1
#define HAL_SRV_HDRSTREN_SIGNED      0
#define HAL_SRV_HDRSTREN_ADR         0xB0000400

#endif // #if (CHIP_REV <= 0xB2)

#define HAL_SRV_APCFDIS_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_APCFDIS_LSB         15
#define HAL_SRV_APCFDIS_MSB         15
#define HAL_SRV_APCFDIS_MASK        0x00008000
#define HAL_SRV_APCFDIS_TYPE        Bool
#define HAL_SRV_APCFDIS             HAL_SRV_APCFDIS_MASK
#define HAL_SRV_APCFDIS_READABLE    1
#define HAL_SRV_APCFDIS_WRITABLE    1
#define HAL_SRV_APCFDIS_SIGNED      0
#define HAL_SRV_APCFDIS_ADR         0xB0000400

#define HAL_SRV_HDRDIS_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_HDRDIS_LSB         14
#define HAL_SRV_HDRDIS_MSB         14
#define HAL_SRV_HDRDIS_MASK        0x00004000
#define HAL_SRV_HDRDIS_TYPE        Bool
#define HAL_SRV_HDRDIS             HAL_SRV_HDRDIS_MASK
#define HAL_SRV_HDRDIS_READABLE    1
#define HAL_SRV_HDRDIS_WRITABLE    1
#define HAL_SRV_HDRDIS_SIGNED      0
#define HAL_SRV_HDRDIS_ADR         0xB0000400

#define HAL_SRV_OPCEN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_OPCEN_LSB         13
#define HAL_SRV_OPCEN_MSB         13
#define HAL_SRV_OPCEN_MASK        0x00002000
#define HAL_SRV_OPCEN_TYPE        Bool
#define HAL_SRV_OPCEN             HAL_SRV_OPCEN_MASK
#define HAL_SRV_OPCEN_READABLE    1
#define HAL_SRV_OPCEN_WRITABLE    1
#define HAL_SRV_OPCEN_SIGNED      0
#define HAL_SRV_OPCEN_ADR         0xB0000400

#define HAL_SRV_OPCRESET_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_OPCRESET_LSB         12
#define HAL_SRV_OPCRESET_MSB         12
#define HAL_SRV_OPCRESET_MASK        0x00001000
#define HAL_SRV_OPCRESET_TYPE        Bool
#define HAL_SRV_OPCRESET             HAL_SRV_OPCRESET_MASK
#define HAL_SRV_OPCRESET_READABLE    1
#define HAL_SRV_OPCRESET_WRITABLE    1
#define HAL_SRV_OPCRESET_SIGNED      0
#define HAL_SRV_OPCRESET_ADR         0xB0000400

#define HAL_SRV_LPCDHRWEN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCDHRWEN_LSB         11
#define HAL_SRV_LPCDHRWEN_MSB         11
#define HAL_SRV_LPCDHRWEN_MASK        0x00000800
#define HAL_SRV_LPCDHRWEN_TYPE        Bool
#define HAL_SRV_LPCDHRWEN             HAL_SRV_LPCDHRWEN_MASK
#define HAL_SRV_LPCDHRWEN_READABLE    1
#define HAL_SRV_LPCDHRWEN_WRITABLE    1
#define HAL_SRV_LPCDHRWEN_SIGNED      0
#define HAL_SRV_LPCDHRWEN_ADR         0xB0000400

#define HAL_SRV_LPCTHWREN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCTHWREN_LSB         10
#define HAL_SRV_LPCTHWREN_MSB         10
#define HAL_SRV_LPCTHWREN_MASK        0x00000400
#define HAL_SRV_LPCTHWREN_TYPE        Bool
#define HAL_SRV_LPCTHWREN             HAL_SRV_LPCTHWREN_MASK
#define HAL_SRV_LPCTHWREN_READABLE    1
#define HAL_SRV_LPCTHWREN_WRITABLE    1
#define HAL_SRV_LPCTHWREN_SIGNED      0
#define HAL_SRV_LPCTHWREN_ADR         0xB0000400

#define HAL_SRV_LPCTHRWEN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCTHRWEN_LSB         9
#define HAL_SRV_LPCTHRWEN_MSB         9
#define HAL_SRV_LPCTHRWEN_MASK        0x00000200
#define HAL_SRV_LPCTHRWEN_TYPE        Bool
#define HAL_SRV_LPCTHRWEN             HAL_SRV_LPCTHRWEN_MASK
#define HAL_SRV_LPCTHRWEN_READABLE    1
#define HAL_SRV_LPCTHRWEN_WRITABLE    1
#define HAL_SRV_LPCTHRWEN_SIGNED      0
#define HAL_SRV_LPCTHRWEN_ADR         0xB0000400

#define HAL_SRV_LPCDRSTWN_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCDRSTWN_LSB         8
#define HAL_SRV_LPCDRSTWN_MSB         8
#define HAL_SRV_LPCDRSTWN_MASK        0x00000100
#define HAL_SRV_LPCDRSTWN_TYPE        Bool
#define HAL_SRV_LPCDRSTWN             HAL_SRV_LPCDRSTWN_MASK
#define HAL_SRV_LPCDRSTWN_READABLE    1
#define HAL_SRV_LPCDRSTWN_WRITABLE    1
#define HAL_SRV_LPCDRSTWN_SIGNED      0
#define HAL_SRV_LPCDRSTWN_ADR         0xB0000400

#define HAL_SRV_LPCDWGNHD_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCDWGNHD_LSB         7
#define HAL_SRV_LPCDWGNHD_MSB         7
#define HAL_SRV_LPCDWGNHD_MASK        0x00000080
#define HAL_SRV_LPCDWGNHD_TYPE        Bool
#define HAL_SRV_LPCDWGNHD             HAL_SRV_LPCDWGNHD_MASK
#define HAL_SRV_LPCDWGNHD_READABLE    1
#define HAL_SRV_LPCDWGNHD_WRITABLE    1
#define HAL_SRV_LPCDWGNHD_SIGNED      0
#define HAL_SRV_LPCDWGNHD_ADR         0xB0000400

#if (CHIP_REV <= 0xB2)

#define HAL_SRV_LPCTWGNHD_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCTWGNHD_LSB         6
#define HAL_SRV_LPCTWGNHD_MSB         6
#define HAL_SRV_LPCTWGNHD_MASK        0x00000040
#define HAL_SRV_LPCTWGNHD_TYPE        Bool
#define HAL_SRV_LPCTWGNHD             HAL_SRV_LPCTWGNHD_MASK
#define HAL_SRV_LPCTWGNHD_READABLE    1
#define HAL_SRV_LPCTWGNHD_WRITABLE    1
#define HAL_SRV_LPCTWGNHD_SIGNED      0
#define HAL_SRV_LPCTWGNHD_ADR         0xB0000400

#endif // #if (CHIP_REV <= 0xB2)

#define HAL_SRV_LPCTWGHD_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCTWGHD_LSB         6
#define HAL_SRV_LPCTWGHD_MSB         6
#define HAL_SRV_LPCTWGHD_MASK        0x00000040
#define HAL_SRV_LPCTWGHD_TYPE        Bool
#define HAL_SRV_LPCTWGHD             HAL_SRV_LPCTWGHD_MASK
#define HAL_SRV_LPCTWGHD_READABLE    1
#define HAL_SRV_LPCTWGHD_WRITABLE    1
#define HAL_SRV_LPCTWGHD_SIGNED      0
#define HAL_SRV_LPCTWGHD_ADR         0xB0000400

#define HAL_SRV_LPCTHOLD_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCTHOLD_LSB         5
#define HAL_SRV_LPCTHOLD_MSB         5
#define HAL_SRV_LPCTHOLD_MASK        0x00000020
#define HAL_SRV_LPCTHOLD_TYPE        Bool
#define HAL_SRV_LPCTHOLD             HAL_SRV_LPCTHOLD_MASK
#define HAL_SRV_LPCTHOLD_READABLE    1
#define HAL_SRV_LPCTHOLD_WRITABLE    1
#define HAL_SRV_LPCTHOLD_SIGNED      0
#define HAL_SRV_LPCTHOLD_ADR         0xB0000400

#define HAL_SRV_LPCDHOLD_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_LPCDHOLD_LSB         4
#define HAL_SRV_LPCDHOLD_MSB         4
#define HAL_SRV_LPCDHOLD_MASK        0x00000010
#define HAL_SRV_LPCDHOLD_TYPE        Bool
#define HAL_SRV_LPCDHOLD             HAL_SRV_LPCDHOLD_MASK
#define HAL_SRV_LPCDHOLD_READABLE    1
#define HAL_SRV_LPCDHOLD_WRITABLE    1
#define HAL_SRV_LPCDHOLD_SIGNED      0
#define HAL_SRV_LPCDHOLD_ADR         0xB0000400

#define HAL_SRV_RSTLPCD_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_RSTLPCD_LSB         3
#define HAL_SRV_RSTLPCD_MSB         3
#define HAL_SRV_RSTLPCD_MASK        0x00000008
#define HAL_SRV_RSTLPCD_TYPE        Bool
#define HAL_SRV_RSTLPCD             HAL_SRV_RSTLPCD_MASK
#define HAL_SRV_RSTLPCD_READABLE    1
#define HAL_SRV_RSTLPCD_WRITABLE    1
#define HAL_SRV_RSTLPCD_SIGNED      0
#define HAL_SRV_RSTLPCD_ADR         0xB0000400

#define HAL_SRV_RSTLPCT_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_RSTLPCT_LSB         2
#define HAL_SRV_RSTLPCT_MSB         2
#define HAL_SRV_RSTLPCT_MASK        0x00000004
#define HAL_SRV_RSTLPCT_TYPE        Bool
#define HAL_SRV_RSTLPCT             HAL_SRV_RSTLPCT_MASK
#define HAL_SRV_RSTLPCT_READABLE    1
#define HAL_SRV_RSTLPCT_WRITABLE    1
#define HAL_SRV_RSTLPCT_SIGNED      0
#define HAL_SRV_RSTLPCT_ADR         0xB0000400

#define HAL_SRV_OPENTHLP_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_OPENTHLP_LSB         1
#define HAL_SRV_OPENTHLP_MSB         1
#define HAL_SRV_OPENTHLP_MASK        0x00000002
#define HAL_SRV_OPENTHLP_TYPE        Bool
#define HAL_SRV_OPENTHLP             HAL_SRV_OPENTHLP_MASK
#define HAL_SRV_OPENTHLP_READABLE    1
#define HAL_SRV_OPENTHLP_WRITABLE    1
#define HAL_SRV_OPENTHLP_SIGNED      0
#define HAL_SRV_OPENTHLP_ADR         0xB0000400

#define HAL_SRV_OPENDLLP_REGISTER    (HAL_SRVREG_MAP->LCCTL)
#define HAL_SRV_OPENDLLP_LSB         0
#define HAL_SRV_OPENDLLP_MSB         0
#define HAL_SRV_OPENDLLP_MASK        0x00000001
#define HAL_SRV_OPENDLLP_TYPE        Bool
#define HAL_SRV_OPENDLLP             HAL_SRV_OPENDLLP_MASK
#define HAL_SRV_OPENDLLP_READABLE    1
#define HAL_SRV_OPENDLLP_WRITABLE    1
#define HAL_SRV_OPENDLLP_SIGNED      0
#define HAL_SRV_OPENDLLP_ADR         0xB0000400

// Register LCTHRESH
#define HAL_SRV_LCTHMIN_REGISTER    (HAL_SRVREG_MAP->LCTHRESH)
#define HAL_SRV_LCTHMIN_LSB         16
#define HAL_SRV_LCTHMIN_MSB         31
#define HAL_SRV_LCTHMIN_MASK        0xFFFF0000
#define HAL_SRV_LCTHMIN_TYPE        UInt16
#define HAL_SRV_LCTHMIN             HAL_SRV_LCTHMIN_MASK
#define HAL_SRV_LCTHMIN_READABLE    1
#define HAL_SRV_LCTHMIN_WRITABLE    1
#define HAL_SRV_LCTHMIN_SIGNED      0
#define HAL_SRV_LCTHMIN_ADR         0xB0000410

#define HAL_SRV_LCTHMAX_REGISTER    (HAL_SRVREG_MAP->LCTHRESH)
#define HAL_SRV_LCTHMAX_LSB         0
#define HAL_SRV_LCTHMAX_MSB         15
#define HAL_SRV_LCTHMAX_MASK        0x0000FFFF
#define HAL_SRV_LCTHMAX_TYPE        UInt16
#define HAL_SRV_LCTHMAX             HAL_SRV_LCTHMAX_MASK
#define HAL_SRV_LCTHMAX_READABLE    1
#define HAL_SRV_LCTHMAX_WRITABLE    1
#define HAL_SRV_LCTHMAX_SIGNED      0
#define HAL_SRV_LCTHMAX_ADR         0xB0000410

// Register LCDELTA0
#define HAL_SRV_DELTA0MIN_REGISTER    (HAL_SRVREG_MAP->LCDELTA0)
#define HAL_SRV_DELTA0MIN_LSB         16
#define HAL_SRV_DELTA0MIN_MSB         31
#define HAL_SRV_DELTA0MIN_MASK        0xFFFF0000
#define HAL_SRV_DELTA0MIN_TYPE        UInt16
#define HAL_SRV_DELTA0MIN             HAL_SRV_DELTA0MIN_MASK
#define HAL_SRV_DELTA0MIN_READABLE    1
#define HAL_SRV_DELTA0MIN_WRITABLE    1
#define HAL_SRV_DELTA0MIN_SIGNED      0
#define HAL_SRV_DELTA0MIN_ADR         0xB0000414

#define HAL_SRV_DELTA0MAX_REGISTER    (HAL_SRVREG_MAP->LCDELTA0)
#define HAL_SRV_DELTA0MAX_LSB         0
#define HAL_SRV_DELTA0MAX_MSB         15
#define HAL_SRV_DELTA0MAX_MASK        0x0000FFFF
#define HAL_SRV_DELTA0MAX_TYPE        UInt16
#define HAL_SRV_DELTA0MAX             HAL_SRV_DELTA0MAX_MASK
#define HAL_SRV_DELTA0MAX_READABLE    1
#define HAL_SRV_DELTA0MAX_WRITABLE    1
#define HAL_SRV_DELTA0MAX_SIGNED      0
#define HAL_SRV_DELTA0MAX_ADR         0xB0000414

// Register LCDELTA1
#define HAL_SRV_DELTA1MIN_REGISTER    (HAL_SRVREG_MAP->LCDELTA1)
#define HAL_SRV_DELTA1MIN_LSB         16
#define HAL_SRV_DELTA1MIN_MSB         31
#define HAL_SRV_DELTA1MIN_MASK        0xFFFF0000
#define HAL_SRV_DELTA1MIN_TYPE        UInt16
#define HAL_SRV_DELTA1MIN             HAL_SRV_DELTA1MIN_MASK
#define HAL_SRV_DELTA1MIN_READABLE    1
#define HAL_SRV_DELTA1MIN_WRITABLE    1
#define HAL_SRV_DELTA1MIN_SIGNED      0
#define HAL_SRV_DELTA1MIN_ADR         0xB0000418

#define HAL_SRV_DELTA1MAX_REGISTER    (HAL_SRVREG_MAP->LCDELTA1)
#define HAL_SRV_DELTA1MAX_LSB         0
#define HAL_SRV_DELTA1MAX_MSB         15
#define HAL_SRV_DELTA1MAX_MASK        0x0000FFFF
#define HAL_SRV_DELTA1MAX_TYPE        UInt16
#define HAL_SRV_DELTA1MAX             HAL_SRV_DELTA1MAX_MASK
#define HAL_SRV_DELTA1MAX_READABLE    1
#define HAL_SRV_DELTA1MAX_WRITABLE    1
#define HAL_SRV_DELTA1MAX_SIGNED      0
#define HAL_SRV_DELTA1MAX_ADR         0xB0000418

// Register LCDELTA2
#define HAL_SRV_DELTA2MIN_REGISTER    (HAL_SRVREG_MAP->LCDELTA2)
#define HAL_SRV_DELTA2MIN_LSB         16
#define HAL_SRV_DELTA2MIN_MSB         31
#define HAL_SRV_DELTA2MIN_MASK        0xFFFF0000
#define HAL_SRV_DELTA2MIN_TYPE        UInt16
#define HAL_SRV_DELTA2MIN             HAL_SRV_DELTA2MIN_MASK
#define HAL_SRV_DELTA2MIN_READABLE    1
#define HAL_SRV_DELTA2MIN_WRITABLE    1
#define HAL_SRV_DELTA2MIN_SIGNED      0
#define HAL_SRV_DELTA2MIN_ADR         0xB000041C

#define HAL_SRV_DELTA2MAX_REGISTER    (HAL_SRVREG_MAP->LCDELTA2)
#define HAL_SRV_DELTA2MAX_LSB         0
#define HAL_SRV_DELTA2MAX_MSB         15
#define HAL_SRV_DELTA2MAX_MASK        0x0000FFFF
#define HAL_SRV_DELTA2MAX_TYPE        UInt16
#define HAL_SRV_DELTA2MAX             HAL_SRV_DELTA2MAX_MASK
#define HAL_SRV_DELTA2MAX_READABLE    1
#define HAL_SRV_DELTA2MAX_WRITABLE    1
#define HAL_SRV_DELTA2MAX_SIGNED      0
#define HAL_SRV_DELTA2MAX_ADR         0xB000041C

// Register LCTIMDELAY
#define HAL_SRV_HDRSTRDLY_REGISTER    (HAL_SRVREG_MAP->LCTIMDELAY)
#define HAL_SRV_HDRSTRDLY_LSB         24
#define HAL_SRV_HDRSTRDLY_MSB         31
#define HAL_SRV_HDRSTRDLY_MASK        0xFF000000
#define HAL_SRV_HDRSTRDLY_TYPE        UByte
#define HAL_SRV_HDRSTRDLY             HAL_SRV_HDRSTRDLY_MASK
#define HAL_SRV_HDRSTRDLY_READABLE    1
#define HAL_SRV_HDRSTRDLY_WRITABLE    1
#define HAL_SRV_HDRSTRDLY_SIGNED      0
#define HAL_SRV_HDRSTRDLY_ADR         0xB0000420

#define HAL_SRV_WGHLDDLY_REGISTER    (HAL_SRVREG_MAP->LCTIMDELAY)
#define HAL_SRV_WGHLDDLY_LSB         16
#define HAL_SRV_WGHLDDLY_MSB         23
#define HAL_SRV_WGHLDDLY_MASK        0x00FF0000
#define HAL_SRV_WGHLDDLY_TYPE        UByte
#define HAL_SRV_WGHLDDLY             HAL_SRV_WGHLDDLY_MASK
#define HAL_SRV_WGHLDDLY_READABLE    1
#define HAL_SRV_WGHLDDLY_WRITABLE    1
#define HAL_SRV_WGHLDDLY_SIGNED      0
#define HAL_SRV_WGHLDDLY_ADR         0xB0000420

#define HAL_SRV_APCFNDLY_REGISTER    (HAL_SRVREG_MAP->LCTIMDELAY)
#define HAL_SRV_APCFNDLY_LSB         8
#define HAL_SRV_APCFNDLY_MSB         15
#define HAL_SRV_APCFNDLY_MASK        0x0000FF00
#define HAL_SRV_APCFNDLY_TYPE        UByte
#define HAL_SRV_APCFNDLY             HAL_SRV_APCFNDLY_MASK
#define HAL_SRV_APCFNDLY_READABLE    1
#define HAL_SRV_APCFNDLY_WRITABLE    1
#define HAL_SRV_APCFNDLY_SIGNED      0
#define HAL_SRV_APCFNDLY_ADR         0xB0000420

#define HAL_SRV_TTWDELAY_REGISTER    (HAL_SRVREG_MAP->LCTIMDELAY)
#define HAL_SRV_TTWDELAY_LSB         0
#define HAL_SRV_TTWDELAY_MSB         7
#define HAL_SRV_TTWDELAY_MASK        0x000000FF
#define HAL_SRV_TTWDELAY_TYPE        UByte
#define HAL_SRV_TTWDELAY             HAL_SRV_TTWDELAY_MASK
#define HAL_SRV_TTWDELAY_READABLE    1
#define HAL_SRV_TTWDELAY_WRITABLE    1
#define HAL_SRV_TTWDELAY_SIGNED      0
#define HAL_SRV_TTWDELAY_ADR         0xB0000420

// Register LCINTEN
#define HAL_SRV_DELTA2IEN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DELTA2IEN_LSB         12
#define HAL_SRV_DELTA2IEN_MSB         12
#define HAL_SRV_DELTA2IEN_MASK        0x00001000
#define HAL_SRV_DELTA2IEN_TYPE        Bool
#define HAL_SRV_DELTA2IEN             HAL_SRV_DELTA2IEN_MASK
#define HAL_SRV_DELTA2IEN_READABLE    1
#define HAL_SRV_DELTA2IEN_WRITABLE    1
#define HAL_SRV_DELTA2IEN_SIGNED      0
#define HAL_SRV_DELTA2IEN_ADR         0xB0000440

#define HAL_SRV_DELTA1IEN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DELTA1IEN_LSB         11
#define HAL_SRV_DELTA1IEN_MSB         11
#define HAL_SRV_DELTA1IEN_MASK        0x00000800
#define HAL_SRV_DELTA1IEN_TYPE        Bool
#define HAL_SRV_DELTA1IEN             HAL_SRV_DELTA1IEN_MASK
#define HAL_SRV_DELTA1IEN_READABLE    1
#define HAL_SRV_DELTA1IEN_WRITABLE    1
#define HAL_SRV_DELTA1IEN_SIGNED      0
#define HAL_SRV_DELTA1IEN_ADR         0xB0000440

#define HAL_SRV_DELTA0IEN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DELTA0IEN_LSB         10
#define HAL_SRV_DELTA0IEN_MSB         10
#define HAL_SRV_DELTA0IEN_MASK        0x00000400
#define HAL_SRV_DELTA0IEN_TYPE        Bool
#define HAL_SRV_DELTA0IEN             HAL_SRV_DELTA0IEN_MASK
#define HAL_SRV_DELTA0IEN_READABLE    1
#define HAL_SRV_DELTA0IEN_WRITABLE    1
#define HAL_SRV_DELTA0IEN_SIGNED      0
#define HAL_SRV_DELTA0IEN_ADR         0xB0000440

#define HAL_SRV_THRESHIEN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_THRESHIEN_LSB         9
#define HAL_SRV_THRESHIEN_MSB         9
#define HAL_SRV_THRESHIEN_MASK        0x00000200
#define HAL_SRV_THRESHIEN_TYPE        Bool
#define HAL_SRV_THRESHIEN             HAL_SRV_THRESHIEN_MASK
#define HAL_SRV_THRESHIEN_READABLE    1
#define HAL_SRV_THRESHIEN_WRITABLE    1
#define HAL_SRV_THRESHIEN_SIGNED      0
#define HAL_SRV_THRESHIEN_ADR         0xB0000440

#define HAL_SRV_TSTIEN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_TSTIEN_LSB         8
#define HAL_SRV_TSTIEN_MSB         8
#define HAL_SRV_TSTIEN_MASK        0x00000100
#define HAL_SRV_TSTIEN_TYPE        Bool
#define HAL_SRV_TSTIEN             HAL_SRV_TSTIEN_MASK
#define HAL_SRV_TSTIEN_READABLE    1
#define HAL_SRV_TSTIEN_WRITABLE    1
#define HAL_SRV_TSTIEN_SIGNED      0
#define HAL_SRV_TSTIEN_ADR         0xB0000440

#define HAL_SRV_DSPI7EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI7EN_LSB         7
#define HAL_SRV_DSPI7EN_MSB         7
#define HAL_SRV_DSPI7EN_MASK        0x00000080
#define HAL_SRV_DSPI7EN_TYPE        Bool
#define HAL_SRV_DSPI7EN             HAL_SRV_DSPI7EN_MASK
#define HAL_SRV_DSPI7EN_READABLE    1
#define HAL_SRV_DSPI7EN_WRITABLE    1
#define HAL_SRV_DSPI7EN_SIGNED      0
#define HAL_SRV_DSPI7EN_ADR         0xB0000440

#define HAL_SRV_DSPI6EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI6EN_LSB         6
#define HAL_SRV_DSPI6EN_MSB         6
#define HAL_SRV_DSPI6EN_MASK        0x00000040
#define HAL_SRV_DSPI6EN_TYPE        Bool
#define HAL_SRV_DSPI6EN             HAL_SRV_DSPI6EN_MASK
#define HAL_SRV_DSPI6EN_READABLE    1
#define HAL_SRV_DSPI6EN_WRITABLE    1
#define HAL_SRV_DSPI6EN_SIGNED      0
#define HAL_SRV_DSPI6EN_ADR         0xB0000440

#define HAL_SRV_DSPI5EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI5EN_LSB         5
#define HAL_SRV_DSPI5EN_MSB         5
#define HAL_SRV_DSPI5EN_MASK        0x00000020
#define HAL_SRV_DSPI5EN_TYPE        Bool
#define HAL_SRV_DSPI5EN             HAL_SRV_DSPI5EN_MASK
#define HAL_SRV_DSPI5EN_READABLE    1
#define HAL_SRV_DSPI5EN_WRITABLE    1
#define HAL_SRV_DSPI5EN_SIGNED      0
#define HAL_SRV_DSPI5EN_ADR         0xB0000440

#define HAL_SRV_DSPI4EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI4EN_LSB         4
#define HAL_SRV_DSPI4EN_MSB         4
#define HAL_SRV_DSPI4EN_MASK        0x00000010
#define HAL_SRV_DSPI4EN_TYPE        Bool
#define HAL_SRV_DSPI4EN             HAL_SRV_DSPI4EN_MASK
#define HAL_SRV_DSPI4EN_READABLE    1
#define HAL_SRV_DSPI4EN_WRITABLE    1
#define HAL_SRV_DSPI4EN_SIGNED      0
#define HAL_SRV_DSPI4EN_ADR         0xB0000440

#define HAL_SRV_DSPI3EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI3EN_LSB         3
#define HAL_SRV_DSPI3EN_MSB         3
#define HAL_SRV_DSPI3EN_MASK        0x00000008
#define HAL_SRV_DSPI3EN_TYPE        Bool
#define HAL_SRV_DSPI3EN             HAL_SRV_DSPI3EN_MASK
#define HAL_SRV_DSPI3EN_READABLE    1
#define HAL_SRV_DSPI3EN_WRITABLE    1
#define HAL_SRV_DSPI3EN_SIGNED      0
#define HAL_SRV_DSPI3EN_ADR         0xB0000440

#define HAL_SRV_DSPI2EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI2EN_LSB         2
#define HAL_SRV_DSPI2EN_MSB         2
#define HAL_SRV_DSPI2EN_MASK        0x00000004
#define HAL_SRV_DSPI2EN_TYPE        Bool
#define HAL_SRV_DSPI2EN             HAL_SRV_DSPI2EN_MASK
#define HAL_SRV_DSPI2EN_READABLE    1
#define HAL_SRV_DSPI2EN_WRITABLE    1
#define HAL_SRV_DSPI2EN_SIGNED      0
#define HAL_SRV_DSPI2EN_ADR         0xB0000440

#define HAL_SRV_DSPI1EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI1EN_LSB         1
#define HAL_SRV_DSPI1EN_MSB         1
#define HAL_SRV_DSPI1EN_MASK        0x00000002
#define HAL_SRV_DSPI1EN_TYPE        Bool
#define HAL_SRV_DSPI1EN             HAL_SRV_DSPI1EN_MASK
#define HAL_SRV_DSPI1EN_READABLE    1
#define HAL_SRV_DSPI1EN_WRITABLE    1
#define HAL_SRV_DSPI1EN_SIGNED      0
#define HAL_SRV_DSPI1EN_ADR         0xB0000440

#define HAL_SRV_DSPI0EN_REGISTER    (HAL_SRVREG_MAP->LCINTEN)
#define HAL_SRV_DSPI0EN_LSB         0
#define HAL_SRV_DSPI0EN_MSB         0
#define HAL_SRV_DSPI0EN_MASK        0x00000001
#define HAL_SRV_DSPI0EN_TYPE        Bool
#define HAL_SRV_DSPI0EN             HAL_SRV_DSPI0EN_MASK
#define HAL_SRV_DSPI0EN_READABLE    1
#define HAL_SRV_DSPI0EN_WRITABLE    1
#define HAL_SRV_DSPI0EN_SIGNED      0
#define HAL_SRV_DSPI0EN_ADR         0xB0000440

// Register LCIRQ
#define HAL_SRV_DELTA2I_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DELTA2I_LSB         12
#define HAL_SRV_DELTA2I_MSB         12
#define HAL_SRV_DELTA2I_MASK        0x00001000
#define HAL_SRV_DELTA2I_TYPE        Bool
#define HAL_SRV_DELTA2I             HAL_SRV_DELTA2I_MASK
#define HAL_SRV_DELTA2I_READABLE    1
#define HAL_SRV_DELTA2I_WRITABLE    1
#define HAL_SRV_DELTA2I_SIGNED      0
#define HAL_SRV_DELTA2I_ADR         0xB0000444

#define HAL_SRV_DELTA1I_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DELTA1I_LSB         11
#define HAL_SRV_DELTA1I_MSB         11
#define HAL_SRV_DELTA1I_MASK        0x00000800
#define HAL_SRV_DELTA1I_TYPE        Bool
#define HAL_SRV_DELTA1I             HAL_SRV_DELTA1I_MASK
#define HAL_SRV_DELTA1I_READABLE    1
#define HAL_SRV_DELTA1I_WRITABLE    1
#define HAL_SRV_DELTA1I_SIGNED      0
#define HAL_SRV_DELTA1I_ADR         0xB0000444

#define HAL_SRV_DELTA0I_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DELTA0I_LSB         10
#define HAL_SRV_DELTA0I_MSB         10
#define HAL_SRV_DELTA0I_MASK        0x00000400
#define HAL_SRV_DELTA0I_TYPE        Bool
#define HAL_SRV_DELTA0I             HAL_SRV_DELTA0I_MASK
#define HAL_SRV_DELTA0I_READABLE    1
#define HAL_SRV_DELTA0I_WRITABLE    1
#define HAL_SRV_DELTA0I_SIGNED      0
#define HAL_SRV_DELTA0I_ADR         0xB0000444

#define HAL_SRV_THRESHI_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_THRESHI_LSB         9
#define HAL_SRV_THRESHI_MSB         9
#define HAL_SRV_THRESHI_MASK        0x00000200
#define HAL_SRV_THRESHI_TYPE        Bool
#define HAL_SRV_THRESHI             HAL_SRV_THRESHI_MASK
#define HAL_SRV_THRESHI_READABLE    1
#define HAL_SRV_THRESHI_WRITABLE    1
#define HAL_SRV_THRESHI_SIGNED      0
#define HAL_SRV_THRESHI_ADR         0xB0000444

#define HAL_SRV_TSTI_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_TSTI_LSB         8
#define HAL_SRV_TSTI_MSB         8
#define HAL_SRV_TSTI_MASK        0x00000100
#define HAL_SRV_TSTI_TYPE        Bool
#define HAL_SRV_TSTI             HAL_SRV_TSTI_MASK
#define HAL_SRV_TSTI_READABLE    1
#define HAL_SRV_TSTI_WRITABLE    1
#define HAL_SRV_TSTI_SIGNED      0
#define HAL_SRV_TSTI_ADR         0xB0000444

#define HAL_SRV_DSPI7_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI7_LSB         7
#define HAL_SRV_DSPI7_MSB         7
#define HAL_SRV_DSPI7_MASK        0x00000080
#define HAL_SRV_DSPI7_TYPE        Bool
#define HAL_SRV_DSPI7             HAL_SRV_DSPI7_MASK
#define HAL_SRV_DSPI7_READABLE    1
#define HAL_SRV_DSPI7_WRITABLE    1
#define HAL_SRV_DSPI7_SIGNED      0
#define HAL_SRV_DSPI7_ADR         0xB0000444

#define HAL_SRV_DSPI6_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI6_LSB         6
#define HAL_SRV_DSPI6_MSB         6
#define HAL_SRV_DSPI6_MASK        0x00000040
#define HAL_SRV_DSPI6_TYPE        Bool
#define HAL_SRV_DSPI6             HAL_SRV_DSPI6_MASK
#define HAL_SRV_DSPI6_READABLE    1
#define HAL_SRV_DSPI6_WRITABLE    1
#define HAL_SRV_DSPI6_SIGNED      0
#define HAL_SRV_DSPI6_ADR         0xB0000444

#define HAL_SRV_DSPI5_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI5_LSB         5
#define HAL_SRV_DSPI5_MSB         5
#define HAL_SRV_DSPI5_MASK        0x00000020
#define HAL_SRV_DSPI5_TYPE        Bool
#define HAL_SRV_DSPI5             HAL_SRV_DSPI5_MASK
#define HAL_SRV_DSPI5_READABLE    1
#define HAL_SRV_DSPI5_WRITABLE    1
#define HAL_SRV_DSPI5_SIGNED      0
#define HAL_SRV_DSPI5_ADR         0xB0000444

#define HAL_SRV_DSPI4_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI4_LSB         4
#define HAL_SRV_DSPI4_MSB         4
#define HAL_SRV_DSPI4_MASK        0x00000010
#define HAL_SRV_DSPI4_TYPE        Bool
#define HAL_SRV_DSPI4             HAL_SRV_DSPI4_MASK
#define HAL_SRV_DSPI4_READABLE    1
#define HAL_SRV_DSPI4_WRITABLE    1
#define HAL_SRV_DSPI4_SIGNED      0
#define HAL_SRV_DSPI4_ADR         0xB0000444

#define HAL_SRV_DSPI3_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI3_LSB         3
#define HAL_SRV_DSPI3_MSB         3
#define HAL_SRV_DSPI3_MASK        0x00000008
#define HAL_SRV_DSPI3_TYPE        Bool
#define HAL_SRV_DSPI3             HAL_SRV_DSPI3_MASK
#define HAL_SRV_DSPI3_READABLE    1
#define HAL_SRV_DSPI3_WRITABLE    1
#define HAL_SRV_DSPI3_SIGNED      0
#define HAL_SRV_DSPI3_ADR         0xB0000444

#define HAL_SRV_DSPI2_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI2_LSB         2
#define HAL_SRV_DSPI2_MSB         2
#define HAL_SRV_DSPI2_MASK        0x00000004
#define HAL_SRV_DSPI2_TYPE        Bool
#define HAL_SRV_DSPI2             HAL_SRV_DSPI2_MASK
#define HAL_SRV_DSPI2_READABLE    1
#define HAL_SRV_DSPI2_WRITABLE    1
#define HAL_SRV_DSPI2_SIGNED      0
#define HAL_SRV_DSPI2_ADR         0xB0000444

#define HAL_SRV_DSPI1_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI1_LSB         1
#define HAL_SRV_DSPI1_MSB         1
#define HAL_SRV_DSPI1_MASK        0x00000002
#define HAL_SRV_DSPI1_TYPE        Bool
#define HAL_SRV_DSPI1             HAL_SRV_DSPI1_MASK
#define HAL_SRV_DSPI1_READABLE    1
#define HAL_SRV_DSPI1_WRITABLE    1
#define HAL_SRV_DSPI1_SIGNED      0
#define HAL_SRV_DSPI1_ADR         0xB0000444

#define HAL_SRV_DSPI0_REGISTER    (HAL_SRVREG_MAP->LCIRQ)
#define HAL_SRV_DSPI0_LSB         0
#define HAL_SRV_DSPI0_MSB         0
#define HAL_SRV_DSPI0_MASK        0x00000001
#define HAL_SRV_DSPI0_TYPE        Bool
#define HAL_SRV_DSPI0             HAL_SRV_DSPI0_MASK
#define HAL_SRV_DSPI0_READABLE    1
#define HAL_SRV_DSPI0_WRITABLE    1
#define HAL_SRV_DSPI0_SIGNED      0
#define HAL_SRV_DSPI0_ADR         0xB0000444

// Register LCSTATUS
#define HAL_SRV_HLDWGTRNS_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_HLDWGTRNS_LSB         8
#define HAL_SRV_HLDWGTRNS_MSB         8
#define HAL_SRV_HLDWGTRNS_MASK        0x00000100
#define HAL_SRV_HLDWGTRNS_TYPE        Bool
#define HAL_SRV_HLDWGTRNS             HAL_SRV_HLDWGTRNS_MASK
#define HAL_SRV_HLDWGTRNS_READABLE    1
#define HAL_SRV_HLDWGTRNS_WRITABLE    0
#define HAL_SRV_HLDWGTRNS_SIGNED      0
#define HAL_SRV_HLDWGTRNS_ADR         0xB0000450

#define HAL_SRV_HDRSTR_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_HDRSTR_LSB         7
#define HAL_SRV_HDRSTR_MSB         7
#define HAL_SRV_HDRSTR_MASK        0x00000080
#define HAL_SRV_HDRSTR_TYPE        Bool
#define HAL_SRV_HDRSTR             HAL_SRV_HDRSTR_MASK
#define HAL_SRV_HDRSTR_READABLE    1
#define HAL_SRV_HDRSTR_WRITABLE    0
#define HAL_SRV_HDRSTR_SIGNED      0
#define HAL_SRV_HDRSTR_ADR         0xB0000450

#define HAL_SRV_TTW_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_TTW_LSB         6
#define HAL_SRV_TTW_MSB         6
#define HAL_SRV_TTW_MASK        0x00000040
#define HAL_SRV_TTW_TYPE        Bool
#define HAL_SRV_TTW             HAL_SRV_TTW_MASK
#define HAL_SRV_TTW_READABLE    1
#define HAL_SRV_TTW_WRITABLE    0
#define HAL_SRV_TTW_SIGNED      0
#define HAL_SRV_TTW_ADR         0xB0000450

#define HAL_SRV_APCFN_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_APCFN_LSB         5
#define HAL_SRV_APCFN_MSB         5
#define HAL_SRV_APCFN_MASK        0x00000020
#define HAL_SRV_APCFN_TYPE        Bool
#define HAL_SRV_APCFN             HAL_SRV_APCFN_MASK
#define HAL_SRV_APCFN_READABLE    1
#define HAL_SRV_APCFN_WRITABLE    0
#define HAL_SRV_APCFN_SIGNED      0
#define HAL_SRV_APCFN_ADR         0xB0000450

#define HAL_SRV_LPCDHDMUX_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_LPCDHDMUX_LSB         4
#define HAL_SRV_LPCDHDMUX_MSB         4
#define HAL_SRV_LPCDHDMUX_MASK        0x00000010
#define HAL_SRV_LPCDHDMUX_TYPE        Bool
#define HAL_SRV_LPCDHDMUX             HAL_SRV_LPCDHDMUX_MASK
#define HAL_SRV_LPCDHDMUX_READABLE    1
#define HAL_SRV_LPCDHDMUX_WRITABLE    0
#define HAL_SRV_LPCDHDMUX_SIGNED      0
#define HAL_SRV_LPCDHDMUX_ADR         0xB0000450

#define HAL_SRV_LPCTHDMUX_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_LPCTHDMUX_LSB         3
#define HAL_SRV_LPCTHDMUX_MSB         3
#define HAL_SRV_LPCTHDMUX_MASK        0x00000008
#define HAL_SRV_LPCTHDMUX_TYPE        Bool
#define HAL_SRV_LPCTHDMUX             HAL_SRV_LPCTHDMUX_MASK
#define HAL_SRV_LPCTHDMUX_READABLE    1
#define HAL_SRV_LPCTHDMUX_WRITABLE    0
#define HAL_SRV_LPCTHDMUX_SIGNED      0
#define HAL_SRV_LPCTHDMUX_ADR         0xB0000450

#define HAL_SRV_LPCDRSTMX_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_LPCDRSTMX_LSB         2
#define HAL_SRV_LPCDRSTMX_MSB         2
#define HAL_SRV_LPCDRSTMX_MASK        0x00000004
#define HAL_SRV_LPCDRSTMX_TYPE        Bool
#define HAL_SRV_LPCDRSTMX             HAL_SRV_LPCDRSTMX_MASK
#define HAL_SRV_LPCDRSTMX_READABLE    1
#define HAL_SRV_LPCDRSTMX_WRITABLE    0
#define HAL_SRV_LPCDRSTMX_SIGNED      0
#define HAL_SRV_LPCDRSTMX_ADR         0xB0000450

#define HAL_SRV_LDGRMUX_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_LDGRMUX_LSB         1
#define HAL_SRV_LDGRMUX_MSB         1
#define HAL_SRV_LDGRMUX_MASK        0x00000002
#define HAL_SRV_LDGRMUX_TYPE        halSRV_LDGRMUX_t
#define HAL_SRV_LDGRMUX             HAL_SRV_LDGRMUX_MASK
#define HAL_SRV_LDGRMUX_READABLE    1
#define HAL_SRV_LDGRMUX_WRITABLE    0
#define HAL_SRV_LDGRMUX_SIGNED      0
#define HAL_SRV_LDGRMUX_ADR         0xB0000450

#define HAL_SRV_WGATEMUX_REGISTER    (HAL_SRVREG_MAP->LCSTATUS)
#define HAL_SRV_WGATEMUX_LSB         0
#define HAL_SRV_WGATEMUX_MSB         0
#define HAL_SRV_WGATEMUX_MASK        0x00000001
#define HAL_SRV_WGATEMUX_TYPE        Bool
#define HAL_SRV_WGATEMUX             HAL_SRV_WGATEMUX_MASK
#define HAL_SRV_WGATEMUX_READABLE    1
#define HAL_SRV_WGATEMUX_WRITABLE    0
#define HAL_SRV_WGATEMUX_SIGNED      0
#define HAL_SRV_WGATEMUX_ADR         0xB0000450

// Register INHIBITTX

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_INHISMP_REGISTER    (HAL_SRVREG_MAP->INHIBITTX)
#define HAL_SRV_INHISMP_LSB         16
#define HAL_SRV_INHISMP_MSB         25
#define HAL_SRV_INHISMP_MASK        0x03FF0000
#define HAL_SRV_INHISMP_TYPE        UInt16
#define HAL_SRV_INHISMP             HAL_SRV_INHISMP_MASK
#define HAL_SRV_INHISMP_READABLE    1
#define HAL_SRV_INHISMP_WRITABLE    1
#define HAL_SRV_INHISMP_SIGNED      0
#define HAL_SRV_INHISMP_ADR         0xB0000454

#endif // #if (CHIP_REV >= 0xC0)

#define HAL_SRV_INHIPOL_REGISTER    (HAL_SRVREG_MAP->INHIBITTX)
#define HAL_SRV_INHIPOL_LSB         11
#define HAL_SRV_INHIPOL_MSB         11
#define HAL_SRV_INHIPOL_MASK        0x00000800
#define HAL_SRV_INHIPOL_TYPE        Bool
#define HAL_SRV_INHIPOL             HAL_SRV_INHIPOL_MASK
#define HAL_SRV_INHIPOL_READABLE    1
#define HAL_SRV_INHIPOL_WRITABLE    1
#define HAL_SRV_INHIPOL_SIGNED      0
#define HAL_SRV_INHIPOL_ADR         0xB0000454

#define HAL_SRV_INHIEN_REGISTER    (HAL_SRVREG_MAP->INHIBITTX)
#define HAL_SRV_INHIEN_LSB         10
#define HAL_SRV_INHIEN_MSB         10
#define HAL_SRV_INHIEN_MASK        0x00000400
#define HAL_SRV_INHIEN_TYPE        Bool
#define HAL_SRV_INHIEN             HAL_SRV_INHIEN_MASK
#define HAL_SRV_INHIEN_READABLE    1
#define HAL_SRV_INHIEN_WRITABLE    1
#define HAL_SRV_INHIEN_SIGNED      0
#define HAL_SRV_INHIEN_ADR         0xB0000454

#define HAL_SRV_INHITX_REGISTER    (HAL_SRVREG_MAP->INHIBITTX)
#define HAL_SRV_INHITX_LSB         0
#define HAL_SRV_INHITX_MSB         9
#define HAL_SRV_INHITX_MASK        0x000003FF
#define HAL_SRV_INHITX_TYPE        UInt16
#define HAL_SRV_INHITX             HAL_SRV_INHITX_MASK
#define HAL_SRV_INHITX_READABLE    1
#define HAL_SRV_INHITX_WRITABLE    1
#define HAL_SRV_INHITX_SIGNED      0
#define HAL_SRV_INHITX_ADR         0xB0000454

#if (CHIP_REV >= 0xC0)

// Register FOLNSTP
#define HAL_SRV_FOLNSTP1_REGISTER    (HAL_SRVREG_MAP->FOLNSTP)
#define HAL_SRV_FOLNSTP1_LSB         0
#define HAL_SRV_FOLNSTP1_MSB         0
#define HAL_SRV_FOLNSTP1_MASK        0x00000001
#define HAL_SRV_FOLNSTP1_TYPE        Bool
#define HAL_SRV_FOLNSTP1             HAL_SRV_FOLNSTP1_MASK
#define HAL_SRV_FOLNSTP1_READABLE    1
#define HAL_SRV_FOLNSTP1_WRITABLE    1
#define HAL_SRV_FOLNSTP1_SIGNED      0
#define HAL_SRV_FOLNSTP1_ADR         0xB0000458

#endif // #if (CHIP_REV >= 0xC0)

// Register PWMSEL
#define HAL_SRV_VRFPWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_VRFPWMSEL_LSB         16
#define HAL_SRV_VRFPWMSEL_MSB         17
#define HAL_SRV_VRFPWMSEL_MASK        0x00030000
#define HAL_SRV_VRFPWMSEL_TYPE        UByte
#define HAL_SRV_VRFPWMSEL             HAL_SRV_VRFPWMSEL_MASK
#define HAL_SRV_VRFPWMSEL_READABLE    1
#define HAL_SRV_VRFPWMSEL_WRITABLE    1
#define HAL_SRV_VRFPWMSEL_SIGNED      0
#define HAL_SRV_VRFPWMSEL_ADR         0xB0000460

#define HAL_SRV_AL2PWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_AL2PWMSEL_LSB         14
#define HAL_SRV_AL2PWMSEL_MSB         15
#define HAL_SRV_AL2PWMSEL_MASK        0x0000C000
#define HAL_SRV_AL2PWMSEL_TYPE        UByte
#define HAL_SRV_AL2PWMSEL             HAL_SRV_AL2PWMSEL_MASK
#define HAL_SRV_AL2PWMSEL_READABLE    1
#define HAL_SRV_AL2PWMSEL_WRITABLE    1
#define HAL_SRV_AL2PWMSEL_SIGNED      0
#define HAL_SRV_AL2PWMSEL_ADR         0xB0000460

#define HAL_SRV_AL1PWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_AL1PWMSEL_LSB         12
#define HAL_SRV_AL1PWMSEL_MSB         13
#define HAL_SRV_AL1PWMSEL_MASK        0x00003000
#define HAL_SRV_AL1PWMSEL_TYPE        UByte
#define HAL_SRV_AL1PWMSEL             HAL_SRV_AL1PWMSEL_MASK
#define HAL_SRV_AL1PWMSEL_READABLE    1
#define HAL_SRV_AL1PWMSEL_WRITABLE    1
#define HAL_SRV_AL1PWMSEL_SIGNED      0
#define HAL_SRV_AL1PWMSEL_ADR         0xB0000460

#define HAL_SRV_DMOPWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_DMOPWMSEL_LSB         10
#define HAL_SRV_DMOPWMSEL_MSB         11
#define HAL_SRV_DMOPWMSEL_MASK        0x00000C00
#define HAL_SRV_DMOPWMSEL_TYPE        UByte
#define HAL_SRV_DMOPWMSEL             HAL_SRV_DMOPWMSEL_MASK
#define HAL_SRV_DMOPWMSEL_READABLE    1
#define HAL_SRV_DMOPWMSEL_WRITABLE    1
#define HAL_SRV_DMOPWMSEL_SIGNED      0
#define HAL_SRV_DMOPWMSEL_ADR         0xB0000460

#define HAL_SRV_SL2PWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_SL2PWMSEL_LSB         8
#define HAL_SRV_SL2PWMSEL_MSB         9
#define HAL_SRV_SL2PWMSEL_MASK        0x00000300
#define HAL_SRV_SL2PWMSEL_TYPE        UByte
#define HAL_SRV_SL2PWMSEL             HAL_SRV_SL2PWMSEL_MASK
#define HAL_SRV_SL2PWMSEL_READABLE    1
#define HAL_SRV_SL2PWMSEL_WRITABLE    1
#define HAL_SRV_SL2PWMSEL_SIGNED      0
#define HAL_SRV_SL2PWMSEL_ADR         0xB0000460

#define HAL_SRV_SL1PWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_SL1PWMSEL_LSB         6
#define HAL_SRV_SL1PWMSEL_MSB         7
#define HAL_SRV_SL1PWMSEL_MASK        0x000000C0
#define HAL_SRV_SL1PWMSEL_TYPE        UByte
#define HAL_SRV_SL1PWMSEL             HAL_SRV_SL1PWMSEL_MASK
#define HAL_SRV_SL1PWMSEL_READABLE    1
#define HAL_SRV_SL1PWMSEL_WRITABLE    1
#define HAL_SRV_SL1PWMSEL_SIGNED      0
#define HAL_SRV_SL1PWMSEL_ADR         0xB0000460

#define HAL_SRV_TILPWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_TILPWMSEL_LSB         4
#define HAL_SRV_TILPWMSEL_MSB         5
#define HAL_SRV_TILPWMSEL_MASK        0x00000030
#define HAL_SRV_TILPWMSEL_TYPE        UByte
#define HAL_SRV_TILPWMSEL             HAL_SRV_TILPWMSEL_MASK
#define HAL_SRV_TILPWMSEL_READABLE    1
#define HAL_SRV_TILPWMSEL_WRITABLE    1
#define HAL_SRV_TILPWMSEL_SIGNED      0
#define HAL_SRV_TILPWMSEL_ADR         0xB0000460

#define HAL_SRV_TROPWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_TROPWMSEL_LSB         2
#define HAL_SRV_TROPWMSEL_MSB         3
#define HAL_SRV_TROPWMSEL_MASK        0x0000000C
#define HAL_SRV_TROPWMSEL_TYPE        UByte
#define HAL_SRV_TROPWMSEL             HAL_SRV_TROPWMSEL_MASK
#define HAL_SRV_TROPWMSEL_READABLE    1
#define HAL_SRV_TROPWMSEL_WRITABLE    1
#define HAL_SRV_TROPWMSEL_SIGNED      0
#define HAL_SRV_TROPWMSEL_ADR         0xB0000460

#define HAL_SRV_FOOPWMSEL_REGISTER    (HAL_SRVREG_MAP->PWMSEL)
#define HAL_SRV_FOOPWMSEL_LSB         0
#define HAL_SRV_FOOPWMSEL_MSB         1
#define HAL_SRV_FOOPWMSEL_MASK        0x00000003
#define HAL_SRV_FOOPWMSEL_TYPE        UByte
#define HAL_SRV_FOOPWMSEL             HAL_SRV_FOOPWMSEL_MASK
#define HAL_SRV_FOOPWMSEL_READABLE    1
#define HAL_SRV_FOOPWMSEL_WRITABLE    1
#define HAL_SRV_FOOPWMSEL_SIGNED      0
#define HAL_SRV_FOOPWMSEL_ADR         0xB0000460

// Register DSPMODE
#define HAL_SRV_PWMODE_REGISTER    (HAL_SRVREG_MAP->DSPMODE)
#define HAL_SRV_PWMODE_LSB         6
#define HAL_SRV_PWMODE_MSB         7
#define HAL_SRV_PWMODE_MASK        0x000000C0
#define HAL_SRV_PWMODE_TYPE        halSRV_PWMODE_t
#define HAL_SRV_PWMODE             HAL_SRV_PWMODE_MASK
#define HAL_SRV_PWMODE_READABLE    1
#define HAL_SRV_PWMODE_WRITABLE    1
#define HAL_SRV_PWMODE_SIGNED      0
#define HAL_SRV_PWMODE_ADR         0xB0000470

#define HAL_SRV_COEFSEL_REGISTER    (HAL_SRVREG_MAP->DSPMODE)
#define HAL_SRV_COEFSEL_LSB         2
#define HAL_SRV_COEFSEL_MSB         2
#define HAL_SRV_COEFSEL_MASK        0x00000004
#define HAL_SRV_COEFSEL_TYPE        Bool
#define HAL_SRV_COEFSEL             HAL_SRV_COEFSEL_MASK
#define HAL_SRV_COEFSEL_READABLE    1
#define HAL_SRV_COEFSEL_WRITABLE    1
#define HAL_SRV_COEFSEL_SIGNED      0
#define HAL_SRV_COEFSEL_ADR         0xB0000470

#define HAL_SRV_DSPLOAD_REGISTER    (HAL_SRVREG_MAP->DSPMODE)
#define HAL_SRV_DSPLOAD_LSB         1
#define HAL_SRV_DSPLOAD_MSB         1
#define HAL_SRV_DSPLOAD_MASK        0x00000002
#define HAL_SRV_DSPLOAD_TYPE        Bool
#define HAL_SRV_DSPLOAD             HAL_SRV_DSPLOAD_MASK
#define HAL_SRV_DSPLOAD_READABLE    1
#define HAL_SRV_DSPLOAD_WRITABLE    1
#define HAL_SRV_DSPLOAD_SIGNED      0
#define HAL_SRV_DSPLOAD_ADR         0xB0000470

#define HAL_SRV_DSPSTART_REGISTER    (HAL_SRVREG_MAP->DSPMODE)
#define HAL_SRV_DSPSTART_LSB         0
#define HAL_SRV_DSPSTART_MSB         0
#define HAL_SRV_DSPSTART_MASK        0x00000001
#define HAL_SRV_DSPSTART_TYPE        Bool
#define HAL_SRV_DSPSTART             HAL_SRV_DSPSTART_MASK
#define HAL_SRV_DSPSTART_READABLE    1
#define HAL_SRV_DSPSTART_WRITABLE    1
#define HAL_SRV_DSPSTART_SIGNED      0
#define HAL_SRV_DSPSTART_ADR         0xB0000470

// Register FGSRC
#define HAL_SRV_FOCDFCHLD_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FOCDFCHLD_LSB         31
#define HAL_SRV_FOCDFCHLD_MSB         31
#define HAL_SRV_FOCDFCHLD_MASK        0x80000000
#define HAL_SRV_FOCDFCHLD_TYPE        Bool
#define HAL_SRV_FOCDFCHLD             HAL_SRV_FOCDFCHLD_MASK
#define HAL_SRV_FOCDFCHLD_READABLE    1
#define HAL_SRV_FOCDFCHLD_WRITABLE    1
#define HAL_SRV_FOCDFCHLD_SIGNED      0
#define HAL_SRV_FOCDFCHLD_ADR         0xB0000474

#define HAL_SRV_FOCHWHLD_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FOCHWHLD_LSB         30
#define HAL_SRV_FOCHWHLD_MSB         30
#define HAL_SRV_FOCHWHLD_MASK        0x40000000
#define HAL_SRV_FOCHWHLD_TYPE        Bool
#define HAL_SRV_FOCHWHLD             HAL_SRV_FOCHWHLD_MASK
#define HAL_SRV_FOCHWHLD_READABLE    1
#define HAL_SRV_FOCHWHLD_WRITABLE    1
#define HAL_SRV_FOCHWHLD_SIGNED      0
#define HAL_SRV_FOCHWHLD_ADR         0xB0000474

#define HAL_SRV_FOCFWHLD_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FOCFWHLD_LSB         29
#define HAL_SRV_FOCFWHLD_MSB         29
#define HAL_SRV_FOCFWHLD_MASK        0x20000000
#define HAL_SRV_FOCFWHLD_TYPE        Bool
#define HAL_SRV_FOCFWHLD             HAL_SRV_FOCFWHLD_MASK
#define HAL_SRV_FOCFWHLD_READABLE    1
#define HAL_SRV_FOCFWHLD_WRITABLE    1
#define HAL_SRV_FOCFWHLD_SIGNED      0
#define HAL_SRV_FOCFWHLD_ADR         0xB0000474

#define HAL_SRV_SRCFOCEN_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_SRCFOCEN_LSB         28
#define HAL_SRV_SRCFOCEN_MSB         28
#define HAL_SRV_SRCFOCEN_MASK        0x10000000
#define HAL_SRV_SRCFOCEN_TYPE        Bool
#define HAL_SRV_SRCFOCEN             HAL_SRV_SRCFOCEN_MASK
#define HAL_SRV_SRCFOCEN_READABLE    1
#define HAL_SRV_SRCFOCEN_WRITABLE    1
#define HAL_SRV_SRCFOCEN_SIGNED      0
#define HAL_SRV_SRCFOCEN_ADR         0xB0000474

#define HAL_SRV_TRKDFCHLD_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_TRKDFCHLD_LSB         27
#define HAL_SRV_TRKDFCHLD_MSB         27
#define HAL_SRV_TRKDFCHLD_MASK        0x08000000
#define HAL_SRV_TRKDFCHLD_TYPE        Bool
#define HAL_SRV_TRKDFCHLD             HAL_SRV_TRKDFCHLD_MASK
#define HAL_SRV_TRKDFCHLD_READABLE    1
#define HAL_SRV_TRKDFCHLD_WRITABLE    1
#define HAL_SRV_TRKDFCHLD_SIGNED      0
#define HAL_SRV_TRKDFCHLD_ADR         0xB0000474

#define HAL_SRV_TRKHWHLD_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_TRKHWHLD_LSB         26
#define HAL_SRV_TRKHWHLD_MSB         26
#define HAL_SRV_TRKHWHLD_MASK        0x04000000
#define HAL_SRV_TRKHWHLD_TYPE        Bool
#define HAL_SRV_TRKHWHLD             HAL_SRV_TRKHWHLD_MASK
#define HAL_SRV_TRKHWHLD_READABLE    1
#define HAL_SRV_TRKHWHLD_WRITABLE    1
#define HAL_SRV_TRKHWHLD_SIGNED      0
#define HAL_SRV_TRKHWHLD_ADR         0xB0000474

#define HAL_SRV_TRKFWHLD_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_TRKFWHLD_LSB         25
#define HAL_SRV_TRKFWHLD_MSB         25
#define HAL_SRV_TRKFWHLD_MASK        0x02000000
#define HAL_SRV_TRKFWHLD_TYPE        Bool
#define HAL_SRV_TRKFWHLD             HAL_SRV_TRKFWHLD_MASK
#define HAL_SRV_TRKFWHLD_READABLE    1
#define HAL_SRV_TRKFWHLD_WRITABLE    1
#define HAL_SRV_TRKFWHLD_SIGNED      0
#define HAL_SRV_TRKFWHLD_ADR         0xB0000474

#define HAL_SRV_SRCTRKEN_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_SRCTRKEN_LSB         24
#define HAL_SRV_SRCTRKEN_MSB         24
#define HAL_SRV_SRCTRKEN_MASK        0x01000000
#define HAL_SRV_SRCTRKEN_TYPE        Bool
#define HAL_SRV_SRCTRKEN             HAL_SRV_SRCTRKEN_MASK
#define HAL_SRV_SRCTRKEN_READABLE    1
#define HAL_SRV_SRCTRKEN_WRITABLE    1
#define HAL_SRV_SRCTRKEN_SIGNED      0
#define HAL_SRV_SRCTRKEN_ADR         0xB0000474

#define HAL_SRV_RSTFGCNT_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_RSTFGCNT_LSB         23
#define HAL_SRV_RSTFGCNT_MSB         23
#define HAL_SRV_RSTFGCNT_MASK        0x00800000
#define HAL_SRV_RSTFGCNT_TYPE        Bool
#define HAL_SRV_RSTFGCNT             HAL_SRV_RSTFGCNT_MASK
#define HAL_SRV_RSTFGCNT_READABLE    1
#define HAL_SRV_RSTFGCNT_WRITABLE    1
#define HAL_SRV_RSTFGCNT_SIGNED      0
#define HAL_SRV_RSTFGCNT_ADR         0xB0000474

#define HAL_SRV_FG3XSEL_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FG3XSEL_LSB         21
#define HAL_SRV_FG3XSEL_MSB         21
#define HAL_SRV_FG3XSEL_MASK        0x00200000
#define HAL_SRV_FG3XSEL_TYPE        halSRV_FG3XSEL_t
#define HAL_SRV_FG3XSEL             HAL_SRV_FG3XSEL_MASK
#define HAL_SRV_FG3XSEL_READABLE    1
#define HAL_SRV_FG3XSEL_WRITABLE    1
#define HAL_SRV_FG3XSEL_SIGNED      0
#define HAL_SRV_FG3XSEL_ADR         0xB0000474

#define HAL_SRV_FGSYNCSEL_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FGSYNCSEL_LSB         20
#define HAL_SRV_FGSYNCSEL_MSB         20
#define HAL_SRV_FGSYNCSEL_MASK        0x00100000
#define HAL_SRV_FGSYNCSEL_TYPE        Bool
#define HAL_SRV_FGSYNCSEL             HAL_SRV_FGSYNCSEL_MASK
#define HAL_SRV_FGSYNCSEL_READABLE    1
#define HAL_SRV_FGSYNCSEL_WRITABLE    1
#define HAL_SRV_FGSYNCSEL_SIGNED      0
#define HAL_SRV_FGSYNCSEL_ADR         0xB0000474

#define HAL_SRV_FGINTERPOL_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FGINTERPOL_LSB         19
#define HAL_SRV_FGINTERPOL_MSB         19
#define HAL_SRV_FGINTERPOL_MASK        0x00080000
#define HAL_SRV_FGINTERPOL_TYPE        Bool
#define HAL_SRV_FGINTERPOL             HAL_SRV_FGINTERPOL_MASK
#define HAL_SRV_FGINTERPOL_READABLE    1
#define HAL_SRV_FGINTERPOL_WRITABLE    1
#define HAL_SRV_FGINTERPOL_SIGNED      0
#define HAL_SRV_FGINTERPOL_ADR         0xB0000474

#define HAL_SRV_AUTOGEAR_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_AUTOGEAR_LSB         18
#define HAL_SRV_AUTOGEAR_MSB         18
#define HAL_SRV_AUTOGEAR_MASK        0x00040000
#define HAL_SRV_AUTOGEAR_TYPE        Bool
#define HAL_SRV_AUTOGEAR             HAL_SRV_AUTOGEAR_MASK
#define HAL_SRV_AUTOGEAR_READABLE    1
#define HAL_SRV_AUTOGEAR_WRITABLE    1
#define HAL_SRV_AUTOGEAR_SIGNED      0
#define HAL_SRV_AUTOGEAR_ADR         0xB0000474

#define HAL_SRV_FGMULEN_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_FGMULEN_LSB         17
#define HAL_SRV_FGMULEN_MSB         17
#define HAL_SRV_FGMULEN_MASK        0x00020000
#define HAL_SRV_FGMULEN_TYPE        Bool
#define HAL_SRV_FGMULEN             HAL_SRV_FGMULEN_MASK
#define HAL_SRV_FGMULEN_READABLE    1
#define HAL_SRV_FGMULEN_WRITABLE    1
#define HAL_SRV_FGMULEN_SIGNED      0
#define HAL_SRV_FGMULEN_ADR         0xB0000474

#define HAL_SRV_SRCQ_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_SRCQ_LSB         11
#define HAL_SRV_SRCQ_MSB         16
#define HAL_SRV_SRCQ_MASK        0x0001F800
#define HAL_SRV_SRCQ_TYPE        UByte
#define HAL_SRV_SRCQ             HAL_SRV_SRCQ_MASK
#define HAL_SRV_SRCQ_READABLE    1
#define HAL_SRV_SRCQ_WRITABLE    1
#define HAL_SRV_SRCQ_SIGNED      0
#define HAL_SRV_SRCQ_ADR         0xB0000474

#define HAL_SRV_SRCN_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_SRCN_LSB         4
#define HAL_SRV_SRCN_MSB         10
#define HAL_SRV_SRCN_MASK        0x000007F0
#define HAL_SRV_SRCN_TYPE        UByte
#define HAL_SRV_SRCN             HAL_SRV_SRCN_MASK
#define HAL_SRV_SRCN_READABLE    1
#define HAL_SRV_SRCN_WRITABLE    1
#define HAL_SRV_SRCN_SIGNED      0
#define HAL_SRV_SRCN_ADR         0xB0000474

#define HAL_SRV_SRCM_REGISTER    (HAL_SRVREG_MAP->FGSRC)
#define HAL_SRV_SRCM_LSB         0
#define HAL_SRV_SRCM_MSB         3
#define HAL_SRV_SRCM_MASK        0x0000000F
#define HAL_SRV_SRCM_TYPE        UByte
#define HAL_SRV_SRCM             HAL_SRV_SRCM_MASK
#define HAL_SRV_SRCM_READABLE    1
#define HAL_SRV_SRCM_WRITABLE    1
#define HAL_SRV_SRCM_SIGNED      0
#define HAL_SRV_SRCM_ADR         0xB0000474

// Register SRCSTA
#define HAL_SRV_FGCOUNT_REGISTER    (HAL_SRVREG_MAP->SRCSTA)
#define HAL_SRV_FGCOUNT_LSB         8
#define HAL_SRV_FGCOUNT_MSB         14
#define HAL_SRV_FGCOUNT_MASK        0x00007F00
#define HAL_SRV_FGCOUNT_TYPE        UByte
#define HAL_SRV_FGCOUNT             HAL_SRV_FGCOUNT_MASK
#define HAL_SRV_FGCOUNT_READABLE    1
#define HAL_SRV_FGCOUNT_WRITABLE    0
#define HAL_SRV_FGCOUNT_SIGNED      0
#define HAL_SRV_FGCOUNT_ADR         0xB0000478

#define HAL_SRV_SRCTRKHLD_REGISTER    (HAL_SRVREG_MAP->SRCSTA)
#define HAL_SRV_SRCTRKHLD_LSB         5
#define HAL_SRV_SRCTRKHLD_MSB         5
#define HAL_SRV_SRCTRKHLD_MASK        0x00000020
#define HAL_SRV_SRCTRKHLD_TYPE        Bool
#define HAL_SRV_SRCTRKHLD             HAL_SRV_SRCTRKHLD_MASK
#define HAL_SRV_SRCTRKHLD_READABLE    1
#define HAL_SRV_SRCTRKHLD_WRITABLE    0
#define HAL_SRV_SRCTRKHLD_SIGNED      0
#define HAL_SRV_SRCTRKHLD_ADR         0xB0000478

#define HAL_SRV_SRCFOCHLD_REGISTER    (HAL_SRVREG_MAP->SRCSTA)
#define HAL_SRV_SRCFOCHLD_LSB         4
#define HAL_SRV_SRCFOCHLD_MSB         4
#define HAL_SRV_SRCFOCHLD_MASK        0x00000010
#define HAL_SRV_SRCFOCHLD_TYPE        Bool
#define HAL_SRV_SRCFOCHLD             HAL_SRV_SRCFOCHLD_MASK
#define HAL_SRV_SRCFOCHLD_READABLE    1
#define HAL_SRV_SRCFOCHLD_WRITABLE    0
#define HAL_SRV_SRCFOCHLD_SIGNED      0
#define HAL_SRV_SRCFOCHLD_ADR         0xB0000478

#define HAL_SRV_FGLOST_REGISTER    (HAL_SRVREG_MAP->SRCSTA)
#define HAL_SRV_FGLOST_LSB         1
#define HAL_SRV_FGLOST_MSB         1
#define HAL_SRV_FGLOST_MASK        0x00000002
#define HAL_SRV_FGLOST_TYPE        Bool
#define HAL_SRV_FGLOST             HAL_SRV_FGLOST_MASK
#define HAL_SRV_FGLOST_READABLE    1
#define HAL_SRV_FGLOST_WRITABLE    0
#define HAL_SRV_FGLOST_SIGNED      0
#define HAL_SRV_FGLOST_ADR         0xB0000478

#define HAL_SRV_FGMULLCK_REGISTER    (HAL_SRVREG_MAP->SRCSTA)
#define HAL_SRV_FGMULLCK_LSB         0
#define HAL_SRV_FGMULLCK_MSB         0
#define HAL_SRV_FGMULLCK_MASK        0x00000001
#define HAL_SRV_FGMULLCK_TYPE        Bool
#define HAL_SRV_FGMULLCK             HAL_SRV_FGMULLCK_MASK
#define HAL_SRV_FGMULLCK_READABLE    1
#define HAL_SRV_FGMULLCK_WRITABLE    0
#define HAL_SRV_FGMULLCK_SIGNED      0
#define HAL_SRV_FGMULLCK_ADR         0xB0000478

// Register PHAADDR
#define HAL_SRV_PHALEN_REGISTER    (HAL_SRVREG_MAP->PHAADDR)
#define HAL_SRV_PHALEN_LSB         16
#define HAL_SRV_PHALEN_MSB         23
#define HAL_SRV_PHALEN_MASK        0x00FF0000
#define HAL_SRV_PHALEN_TYPE        UByte
#define HAL_SRV_PHALEN             HAL_SRV_PHALEN_MASK
#define HAL_SRV_PHALEN_READABLE    1
#define HAL_SRV_PHALEN_WRITABLE    1
#define HAL_SRV_PHALEN_SIGNED      0
#define HAL_SRV_PHALEN_ADR         0xB000047C

#define HAL_SRV_PHAADR_REGISTER    (HAL_SRVREG_MAP->PHAADDR)
#define HAL_SRV_PHAADR_LSB         0
#define HAL_SRV_PHAADR_MSB         9
#define HAL_SRV_PHAADR_MASK        0x000003FF
#define HAL_SRV_PHAADR_TYPE        UInt16
#define HAL_SRV_PHAADR             HAL_SRV_PHAADR_MASK
#define HAL_SRV_PHAADR_READABLE    1
#define HAL_SRV_PHAADR_WRITABLE    1
#define HAL_SRV_PHAADR_SIGNED      0
#define HAL_SRV_PHAADR_ADR         0xB000047C

// Register PHB1ADDR
#define HAL_SRV_PHB1LEN_REGISTER    (HAL_SRVREG_MAP->PHB1ADDR)
#define HAL_SRV_PHB1LEN_LSB         16
#define HAL_SRV_PHB1LEN_MSB         23
#define HAL_SRV_PHB1LEN_MASK        0x00FF0000
#define HAL_SRV_PHB1LEN_TYPE        UByte
#define HAL_SRV_PHB1LEN             HAL_SRV_PHB1LEN_MASK
#define HAL_SRV_PHB1LEN_READABLE    1
#define HAL_SRV_PHB1LEN_WRITABLE    1
#define HAL_SRV_PHB1LEN_SIGNED      0
#define HAL_SRV_PHB1LEN_ADR         0xB0000480

#define HAL_SRV_PHB1ADR_REGISTER    (HAL_SRVREG_MAP->PHB1ADDR)
#define HAL_SRV_PHB1ADR_LSB         0
#define HAL_SRV_PHB1ADR_MSB         9
#define HAL_SRV_PHB1ADR_MASK        0x000003FF
#define HAL_SRV_PHB1ADR_TYPE        UInt16
#define HAL_SRV_PHB1ADR             HAL_SRV_PHB1ADR_MASK
#define HAL_SRV_PHB1ADR_READABLE    1
#define HAL_SRV_PHB1ADR_WRITABLE    1
#define HAL_SRV_PHB1ADR_SIGNED      0
#define HAL_SRV_PHB1ADR_ADR         0xB0000480

// Register PHB2ADDR
#define HAL_SRV_PHB2LEN_REGISTER    (HAL_SRVREG_MAP->PHB2ADDR)
#define HAL_SRV_PHB2LEN_LSB         16
#define HAL_SRV_PHB2LEN_MSB         23
#define HAL_SRV_PHB2LEN_MASK        0x00FF0000
#define HAL_SRV_PHB2LEN_TYPE        UByte
#define HAL_SRV_PHB2LEN             HAL_SRV_PHB2LEN_MASK
#define HAL_SRV_PHB2LEN_READABLE    1
#define HAL_SRV_PHB2LEN_WRITABLE    1
#define HAL_SRV_PHB2LEN_SIGNED      0
#define HAL_SRV_PHB2LEN_ADR         0xB0000484

#define HAL_SRV_PHB2ADR_REGISTER    (HAL_SRVREG_MAP->PHB2ADDR)
#define HAL_SRV_PHB2ADR_LSB         0
#define HAL_SRV_PHB2ADR_MSB         9
#define HAL_SRV_PHB2ADR_MASK        0x000003FF
#define HAL_SRV_PHB2ADR_TYPE        UInt16
#define HAL_SRV_PHB2ADR             HAL_SRV_PHB2ADR_MASK
#define HAL_SRV_PHB2ADR_READABLE    1
#define HAL_SRV_PHB2ADR_WRITABLE    1
#define HAL_SRV_PHB2ADR_SIGNED      0
#define HAL_SRV_PHB2ADR_ADR         0xB0000484

// Register PHC1ADDR
#define HAL_SRV_PHC1LEN_REGISTER    (HAL_SRVREG_MAP->PHC1ADDR)
#define HAL_SRV_PHC1LEN_LSB         16
#define HAL_SRV_PHC1LEN_MSB         23
#define HAL_SRV_PHC1LEN_MASK        0x00FF0000
#define HAL_SRV_PHC1LEN_TYPE        UByte
#define HAL_SRV_PHC1LEN             HAL_SRV_PHC1LEN_MASK
#define HAL_SRV_PHC1LEN_READABLE    1
#define HAL_SRV_PHC1LEN_WRITABLE    1
#define HAL_SRV_PHC1LEN_SIGNED      0
#define HAL_SRV_PHC1LEN_ADR         0xB0000488

#define HAL_SRV_PHC1ADR_REGISTER    (HAL_SRVREG_MAP->PHC1ADDR)
#define HAL_SRV_PHC1ADR_LSB         0
#define HAL_SRV_PHC1ADR_MSB         9
#define HAL_SRV_PHC1ADR_MASK        0x000003FF
#define HAL_SRV_PHC1ADR_TYPE        UInt16
#define HAL_SRV_PHC1ADR             HAL_SRV_PHC1ADR_MASK
#define HAL_SRV_PHC1ADR_READABLE    1
#define HAL_SRV_PHC1ADR_WRITABLE    1
#define HAL_SRV_PHC1ADR_SIGNED      0
#define HAL_SRV_PHC1ADR_ADR         0xB0000488

// Register PHC2ADDR
#define HAL_SRV_PHC2LEN_REGISTER    (HAL_SRVREG_MAP->PHC2ADDR)
#define HAL_SRV_PHC2LEN_LSB         16
#define HAL_SRV_PHC2LEN_MSB         23
#define HAL_SRV_PHC2LEN_MASK        0x00FF0000
#define HAL_SRV_PHC2LEN_TYPE        UByte
#define HAL_SRV_PHC2LEN             HAL_SRV_PHC2LEN_MASK
#define HAL_SRV_PHC2LEN_READABLE    1
#define HAL_SRV_PHC2LEN_WRITABLE    1
#define HAL_SRV_PHC2LEN_SIGNED      0
#define HAL_SRV_PHC2LEN_ADR         0xB000048C

#define HAL_SRV_PHC2ADR_REGISTER    (HAL_SRVREG_MAP->PHC2ADDR)
#define HAL_SRV_PHC2ADR_LSB         0
#define HAL_SRV_PHC2ADR_MSB         9
#define HAL_SRV_PHC2ADR_MASK        0x000003FF
#define HAL_SRV_PHC2ADR_TYPE        UInt16
#define HAL_SRV_PHC2ADR             HAL_SRV_PHC2ADR_MASK
#define HAL_SRV_PHC2ADR_READABLE    1
#define HAL_SRV_PHC2ADR_WRITABLE    1
#define HAL_SRV_PHC2ADR_SIGNED      0
#define HAL_SRV_PHC2ADR_ADR         0xB000048C

// Register PHC3ADDR
#define HAL_SRV_PHC3LEN_REGISTER    (HAL_SRVREG_MAP->PHC3ADDR)
#define HAL_SRV_PHC3LEN_LSB         16
#define HAL_SRV_PHC3LEN_MSB         23
#define HAL_SRV_PHC3LEN_MASK        0x00FF0000
#define HAL_SRV_PHC3LEN_TYPE        UByte
#define HAL_SRV_PHC3LEN             HAL_SRV_PHC3LEN_MASK
#define HAL_SRV_PHC3LEN_READABLE    1
#define HAL_SRV_PHC3LEN_WRITABLE    1
#define HAL_SRV_PHC3LEN_SIGNED      0
#define HAL_SRV_PHC3LEN_ADR         0xB0000490

#define HAL_SRV_PHC3ADR_REGISTER    (HAL_SRVREG_MAP->PHC3ADDR)
#define HAL_SRV_PHC3ADR_LSB         0
#define HAL_SRV_PHC3ADR_MSB         9
#define HAL_SRV_PHC3ADR_MASK        0x000003FF
#define HAL_SRV_PHC3ADR_TYPE        UInt16
#define HAL_SRV_PHC3ADR             HAL_SRV_PHC3ADR_MASK
#define HAL_SRV_PHC3ADR_READABLE    1
#define HAL_SRV_PHC3ADR_WRITABLE    1
#define HAL_SRV_PHC3ADR_SIGNED      0
#define HAL_SRV_PHC3ADR_ADR         0xB0000490

// Register PHC4ADDR
#define HAL_SRV_PHC4LEN_REGISTER    (HAL_SRVREG_MAP->PHC4ADDR)
#define HAL_SRV_PHC4LEN_LSB         16
#define HAL_SRV_PHC4LEN_MSB         23
#define HAL_SRV_PHC4LEN_MASK        0x00FF0000
#define HAL_SRV_PHC4LEN_TYPE        UByte
#define HAL_SRV_PHC4LEN             HAL_SRV_PHC4LEN_MASK
#define HAL_SRV_PHC4LEN_READABLE    1
#define HAL_SRV_PHC4LEN_WRITABLE    1
#define HAL_SRV_PHC4LEN_SIGNED      0
#define HAL_SRV_PHC4LEN_ADR         0xB0000494

#define HAL_SRV_PHC4ADR_REGISTER    (HAL_SRVREG_MAP->PHC4ADDR)
#define HAL_SRV_PHC4ADR_LSB         0
#define HAL_SRV_PHC4ADR_MSB         9
#define HAL_SRV_PHC4ADR_MASK        0x000003FF
#define HAL_SRV_PHC4ADR_TYPE        UInt16
#define HAL_SRV_PHC4ADR             HAL_SRV_PHC4ADR_MASK
#define HAL_SRV_PHC4ADR_READABLE    1
#define HAL_SRV_PHC4ADR_WRITABLE    1
#define HAL_SRV_PHC4ADR_SIGNED      0
#define HAL_SRV_PHC4ADR_ADR         0xB0000494

// Register PHDADDR
#define HAL_SRV_PHDADR_REGISTER    (HAL_SRVREG_MAP->PHDADDR)
#define HAL_SRV_PHDADR_LSB         0
#define HAL_SRV_PHDADR_MSB         9
#define HAL_SRV_PHDADR_MASK        0x000003FF
#define HAL_SRV_PHDADR_TYPE        UInt16
#define HAL_SRV_PHDADR             HAL_SRV_PHDADR_MASK
#define HAL_SRV_PHDADR_READABLE    1
#define HAL_SRV_PHDADR_WRITABLE    1
#define HAL_SRV_PHDADR_SIGNED      0
#define HAL_SRV_PHDADR_ADR         0xB0000498

// Register RAMTEST
#define HAL_SRV_RAMWR_REGISTER    (HAL_SRVREG_MAP->RAMTEST)
#define HAL_SRV_RAMWR_LSB         13
#define HAL_SRV_RAMWR_MSB         13
#define HAL_SRV_RAMWR_MASK        0x00002000
#define HAL_SRV_RAMWR_TYPE        Bool
#define HAL_SRV_RAMWR             HAL_SRV_RAMWR_MASK
#define HAL_SRV_RAMWR_READABLE    1
#define HAL_SRV_RAMWR_WRITABLE    1
#define HAL_SRV_RAMWR_SIGNED      0
#define HAL_SRV_RAMWR_ADR         0xB000049C

#define HAL_SRV_RAMADR_REGISTER    (HAL_SRVREG_MAP->RAMTEST)
#define HAL_SRV_RAMADR_LSB         0
#define HAL_SRV_RAMADR_MSB         12
#define HAL_SRV_RAMADR_MASK        0x00001FFF
#define HAL_SRV_RAMADR_TYPE        UInt16
#define HAL_SRV_RAMADR             HAL_SRV_RAMADR_MASK
#define HAL_SRV_RAMADR_READABLE    1
#define HAL_SRV_RAMADR_WRITABLE    1
#define HAL_SRV_RAMADR_SIGNED      0
#define HAL_SRV_RAMADR_ADR         0xB000049C

// Register SEQ_PC
#define HAL_SRV_SEQPC_REGISTER    (HAL_SRVREG_MAP->SEQ_PC)
#define HAL_SRV_SEQPC_LSB         0
#define HAL_SRV_SEQPC_MSB         9
#define HAL_SRV_SEQPC_MASK        0x000003FF
#define HAL_SRV_SEQPC_TYPE        UInt16
#define HAL_SRV_SEQPC             HAL_SRV_SEQPC_MASK
#define HAL_SRV_SEQPC_READABLE    1
#define HAL_SRV_SEQPC_WRITABLE    0
#define HAL_SRV_SEQPC_SIGNED      0
#define HAL_SRV_SEQPC_ADR         0xB00004A0

// Register FWCTRL
#define HAL_SRV_FWCTRL_REGISTER    (HAL_SRVREG_MAP->FWCTRL)
#define HAL_SRV_FWCTRL_LSB         0
#define HAL_SRV_FWCTRL_MSB         10
#define HAL_SRV_FWCTRL_MASK        0x000007FF
#define HAL_SRV_FWCTRL_TYPE        UInt16
#define HAL_SRV_FWCTRL             HAL_SRV_FWCTRL_MASK
#define HAL_SRV_FWCTRL_READABLE    1
#define HAL_SRV_FWCTRL_WRITABLE    1
#define HAL_SRV_FWCTRL_SIGNED      0
#define HAL_SRV_FWCTRL_ADR         0xB00004A4

// Register PDMCNT
#define HAL_SRV_PDMOENB_REGISTER    (HAL_SRVREG_MAP->PDMCNT)
#define HAL_SRV_PDMOENB_LSB         16
#define HAL_SRV_PDMOENB_MSB         28
#define HAL_SRV_PDMOENB_MASK        0x1FFF0000
#define HAL_SRV_PDMOENB_TYPE        UInt16
#define HAL_SRV_PDMOENB             HAL_SRV_PDMOENB_MASK
#define HAL_SRV_PDMOENB_READABLE    1
#define HAL_SRV_PDMOENB_WRITABLE    1
#define HAL_SRV_PDMOENB_SIGNED      0
#define HAL_SRV_PDMOENB_ADR         0xB00004A8

#if (CHIP_REV <= 0xB3)

#define HAL_SRV_PDMLPWR_REGISTER    (HAL_SRVREG_MAP->PDMCNT)
#define HAL_SRV_PDMLPWR_LSB         0
#define HAL_SRV_PDMLPWR_MSB         12
#define HAL_SRV_PDMLPWR_MASK        0x00001FFF
#define HAL_SRV_PDMLPWR_TYPE        UInt16
#define HAL_SRV_PDMLPWR             HAL_SRV_PDMLPWR_MASK
#define HAL_SRV_PDMLPWR_READABLE    1
#define HAL_SRV_PDMLPWR_WRITABLE    1
#define HAL_SRV_PDMLPWR_SIGNED      0
#define HAL_SRV_PDMLPWR_ADR         0xB00004A8

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

#define HAL_SRV_PDMLPWR_REGISTER    (HAL_SRVREG_MAP->PDMCNT)
#define HAL_SRV_PDMLPWR_LSB         0
#define HAL_SRV_PDMLPWR_MSB         15
#define HAL_SRV_PDMLPWR_MASK        0x0000FFFF
#define HAL_SRV_PDMLPWR_TYPE        UInt16
#define HAL_SRV_PDMLPWR             HAL_SRV_PDMLPWR_MASK
#define HAL_SRV_PDMLPWR_READABLE    1
#define HAL_SRV_PDMLPWR_WRITABLE    1
#define HAL_SRV_PDMLPWR_SIGNED      0
#define HAL_SRV_PDMLPWR_ADR         0xB00004A8

#endif // #if (CHIP_REV >= 0xC0)

// Register PDMALPHA
#define HAL_SRV_PDMALPHA2_REGISTER    (HAL_SRVREG_MAP->PDMALPHA)
#define HAL_SRV_PDMALPHA2_LSB         16
#define HAL_SRV_PDMALPHA2_MSB         25
#define HAL_SRV_PDMALPHA2_MASK        0x03FF0000
#define HAL_SRV_PDMALPHA2_TYPE        UInt16
#define HAL_SRV_PDMALPHA2             HAL_SRV_PDMALPHA2_MASK
#define HAL_SRV_PDMALPHA2_READABLE    1
#define HAL_SRV_PDMALPHA2_WRITABLE    1
#define HAL_SRV_PDMALPHA2_SIGNED      0
#define HAL_SRV_PDMALPHA2_ADR         0xB00004AC

#define HAL_SRV_PDMALPHA1_REGISTER    (HAL_SRVREG_MAP->PDMALPHA)
#define HAL_SRV_PDMALPHA1_LSB         0
#define HAL_SRV_PDMALPHA1_MSB         9
#define HAL_SRV_PDMALPHA1_MASK        0x000003FF
#define HAL_SRV_PDMALPHA1_TYPE        UInt16
#define HAL_SRV_PDMALPHA1             HAL_SRV_PDMALPHA1_MASK
#define HAL_SRV_PDMALPHA1_READABLE    1
#define HAL_SRV_PDMALPHA1_WRITABLE    1
#define HAL_SRV_PDMALPHA1_SIGNED      0
#define HAL_SRV_PDMALPHA1_ADR         0xB00004AC

// Register STMCTRL
#define HAL_SRV_STMEN_REGISTER    (HAL_SRVREG_MAP->STMCTRL)
#define HAL_SRV_STMEN_LSB         8
#define HAL_SRV_STMEN_MSB         8
#define HAL_SRV_STMEN_MASK        0x00000100
#define HAL_SRV_STMEN_TYPE        Bool
#define HAL_SRV_STMEN             HAL_SRV_STMEN_MASK
#define HAL_SRV_STMEN_READABLE    1
#define HAL_SRV_STMEN_WRITABLE    1
#define HAL_SRV_STMEN_SIGNED      0
#define HAL_SRV_STMEN_ADR         0xB00004B0

#define HAL_SRV_STMCNT_REGISTER    (HAL_SRVREG_MAP->STMCTRL)
#define HAL_SRV_STMCNT_LSB         0
#define HAL_SRV_STMCNT_MSB         7
#define HAL_SRV_STMCNT_MASK        0x000000FF
#define HAL_SRV_STMCNT_TYPE        UByte
#define HAL_SRV_STMCNT             HAL_SRV_STMCNT_MASK
#define HAL_SRV_STMCNT_READABLE    1
#define HAL_SRV_STMCNT_WRITABLE    1
#define HAL_SRV_STMCNT_SIGNED      0
#define HAL_SRV_STMCNT_ADR         0xB00004B0

// Register TESRCCTL
#define HAL_SRV_TESRCDB_REGISTER    (HAL_SRVREG_MAP->TESRCCTL)
#define HAL_SRV_TESRCDB_LSB         8
#define HAL_SRV_TESRCDB_MSB         17
#define HAL_SRV_TESRCDB_MASK        0x0003FF00
#define HAL_SRV_TESRCDB_TYPE        UInt16
#define HAL_SRV_TESRCDB             HAL_SRV_TESRCDB_MASK
#define HAL_SRV_TESRCDB_READABLE    1
#define HAL_SRV_TESRCDB_WRITABLE    1
#define HAL_SRV_TESRCDB_SIGNED      0
#define HAL_SRV_TESRCDB_ADR         0xB00004B4

#define HAL_SRV_TESRCHLD_REGISTER    (HAL_SRVREG_MAP->TESRCCTL)
#define HAL_SRV_TESRCHLD_LSB         3
#define HAL_SRV_TESRCHLD_MSB         3
#define HAL_SRV_TESRCHLD_MASK        0x00000008
#define HAL_SRV_TESRCHLD_TYPE        Bool
#define HAL_SRV_TESRCHLD             HAL_SRV_TESRCHLD_MASK
#define HAL_SRV_TESRCHLD_READABLE    1
#define HAL_SRV_TESRCHLD_WRITABLE    1
#define HAL_SRV_TESRCHLD_SIGNED      0
#define HAL_SRV_TESRCHLD_ADR         0xB00004B4

#define HAL_SRV_TESRCPOL_REGISTER    (HAL_SRVREG_MAP->TESRCCTL)
#define HAL_SRV_TESRCPOL_LSB         2
#define HAL_SRV_TESRCPOL_MSB         2
#define HAL_SRV_TESRCPOL_MASK        0x00000004
#define HAL_SRV_TESRCPOL_TYPE        Bool
#define HAL_SRV_TESRCPOL             HAL_SRV_TESRCPOL_MASK
#define HAL_SRV_TESRCPOL_READABLE    1
#define HAL_SRV_TESRCPOL_WRITABLE    1
#define HAL_SRV_TESRCPOL_SIGNED      0
#define HAL_SRV_TESRCPOL_ADR         0xB00004B4

#define HAL_SRV_TESRCSW_REGISTER    (HAL_SRVREG_MAP->TESRCCTL)
#define HAL_SRV_TESRCSW_LSB         1
#define HAL_SRV_TESRCSW_MSB         1
#define HAL_SRV_TESRCSW_MASK        0x00000002
#define HAL_SRV_TESRCSW_TYPE        Bool
#define HAL_SRV_TESRCSW             HAL_SRV_TESRCSW_MASK
#define HAL_SRV_TESRCSW_READABLE    1
#define HAL_SRV_TESRCSW_WRITABLE    1
#define HAL_SRV_TESRCSW_SIGNED      0
#define HAL_SRV_TESRCSW_ADR         0xB00004B4

#define HAL_SRV_TESRCEN_REGISTER    (HAL_SRVREG_MAP->TESRCCTL)
#define HAL_SRV_TESRCEN_LSB         0
#define HAL_SRV_TESRCEN_MSB         0
#define HAL_SRV_TESRCEN_MASK        0x00000001
#define HAL_SRV_TESRCEN_TYPE        Bool
#define HAL_SRV_TESRCEN             HAL_SRV_TESRCEN_MASK
#define HAL_SRV_TESRCEN_READABLE    1
#define HAL_SRV_TESRCEN_WRITABLE    1
#define HAL_SRV_TESRCEN_SIGNED      0
#define HAL_SRV_TESRCEN_ADR         0xB00004B4

// Register PDMVREF
#define HAL_SRV_PDMAVREF_REGISTER    (HAL_SRVREG_MAP->PDMVREF)
#define HAL_SRV_PDMAVREF_LSB         0
#define HAL_SRV_PDMAVREF_MSB         9
#define HAL_SRV_PDMAVREF_MASK        0x000003FF
#define HAL_SRV_PDMAVREF_TYPE        UInt16
#define HAL_SRV_PDMAVREF             HAL_SRV_PDMAVREF_MASK
#define HAL_SRV_PDMAVREF_READABLE    1
#define HAL_SRV_PDMAVREF_WRITABLE    1
#define HAL_SRV_PDMAVREF_SIGNED      0
#define HAL_SRV_PDMAVREF_ADR         0xB00004B8

// Register SWOFFEN
#define HAL_SRV_TLOFFSUM_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TLOFFSUM_LSB         29
#define HAL_SRV_TLOFFSUM_MSB         29
#define HAL_SRV_TLOFFSUM_MASK        0x20000000
#define HAL_SRV_TLOFFSUM_TYPE        Bool
#define HAL_SRV_TLOFFSUM             HAL_SRV_TLOFFSUM_MASK
#define HAL_SRV_TLOFFSUM_READABLE    1
#define HAL_SRV_TLOFFSUM_WRITABLE    1
#define HAL_SRV_TLOFFSUM_SIGNED      0
#define HAL_SRV_TLOFFSUM_ADR         0xB00004BC

#define HAL_SRV_TLOFFTILT_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TLOFFTILT_LSB         28
#define HAL_SRV_TLOFFTILT_MSB         28
#define HAL_SRV_TLOFFTILT_MASK        0x10000000
#define HAL_SRV_TLOFFTILT_TYPE        Bool
#define HAL_SRV_TLOFFTILT             HAL_SRV_TLOFFTILT_MASK
#define HAL_SRV_TLOFFTILT_READABLE    1
#define HAL_SRV_TLOFFTILT_WRITABLE    1
#define HAL_SRV_TLOFFTILT_SIGNED      0
#define HAL_SRV_TLOFFTILT_ADR         0xB00004BC

#define HAL_SRV_TOFFFETHR_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFFETHR_LSB         27
#define HAL_SRV_TOFFFETHR_MSB         27
#define HAL_SRV_TOFFFETHR_MASK        0x08000000
#define HAL_SRV_TOFFFETHR_TYPE        Bool
#define HAL_SRV_TOFFFETHR             HAL_SRV_TOFFFETHR_MASK
#define HAL_SRV_TOFFFETHR_READABLE    1
#define HAL_SRV_TOFFFETHR_WRITABLE    1
#define HAL_SRV_TOFFFETHR_SIGNED      0
#define HAL_SRV_TOFFFETHR_ADR         0xB00004BC

#define HAL_SRV_TOFFFE2C_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFFE2C_LSB         26
#define HAL_SRV_TOFFFE2C_MSB         26
#define HAL_SRV_TOFFFE2C_MASK        0x04000000
#define HAL_SRV_TOFFFE2C_TYPE        Bool
#define HAL_SRV_TOFFFE2C             HAL_SRV_TOFFFE2C_MASK
#define HAL_SRV_TOFFFE2C_READABLE    1
#define HAL_SRV_TOFFFE2C_WRITABLE    1
#define HAL_SRV_TOFFFE2C_SIGNED      0
#define HAL_SRV_TOFFFE2C_ADR         0xB00004BC

#define HAL_SRV_TOFFFE1C_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFFE1C_LSB         25
#define HAL_SRV_TOFFFE1C_MSB         25
#define HAL_SRV_TOFFFE1C_MASK        0x02000000
#define HAL_SRV_TOFFFE1C_TYPE        Bool
#define HAL_SRV_TOFFFE1C             HAL_SRV_TOFFFE1C_MASK
#define HAL_SRV_TOFFFE1C_READABLE    1
#define HAL_SRV_TOFFFE1C_WRITABLE    1
#define HAL_SRV_TOFFFE1C_SIGNED      0
#define HAL_SRV_TOFFFE1C_ADR         0xB00004BC

#define HAL_SRV_TOFFTEOSC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFTEOSC_LSB         24
#define HAL_SRV_TOFFTEOSC_MSB         24
#define HAL_SRV_TOFFTEOSC_MASK        0x01000000
#define HAL_SRV_TOFFTEOSC_TYPE        Bool
#define HAL_SRV_TOFFTEOSC             HAL_SRV_TOFFTEOSC_MASK
#define HAL_SRV_TOFFTEOSC_READABLE    1
#define HAL_SRV_TOFFTEOSC_WRITABLE    1
#define HAL_SRV_TOFFTEOSC_SIGNED      0
#define HAL_SRV_TOFFTEOSC_ADR         0xB00004BC

#define HAL_SRV_TOFFPANIC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFPANIC_LSB         23
#define HAL_SRV_TOFFPANIC_MSB         23
#define HAL_SRV_TOFFPANIC_MASK        0x00800000
#define HAL_SRV_TOFFPANIC_TYPE        Bool
#define HAL_SRV_TOFFPANIC             HAL_SRV_TOFFPANIC_MASK
#define HAL_SRV_TOFFPANIC_READABLE    1
#define HAL_SRV_TOFFPANIC_WRITABLE    1
#define HAL_SRV_TOFFPANIC_SIGNED      0
#define HAL_SRV_TOFFPANIC_ADR         0xB00004BC

#define HAL_SRV_TOFFFSHC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFFSHC_LSB         22
#define HAL_SRV_TOFFFSHC_MSB         22
#define HAL_SRV_TOFFFSHC_MASK        0x00400000
#define HAL_SRV_TOFFFSHC_TYPE        Bool
#define HAL_SRV_TOFFFSHC             HAL_SRV_TOFFFSHC_MASK
#define HAL_SRV_TOFFFSHC_READABLE    1
#define HAL_SRV_TOFFFSHC_WRITABLE    1
#define HAL_SRV_TOFFFSHC_SIGNED      0
#define HAL_SRV_TOFFFSHC_ADR         0xB00004BC

#define HAL_SRV_TOFFMIRR_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFMIRR_LSB         21
#define HAL_SRV_TOFFMIRR_MSB         21
#define HAL_SRV_TOFFMIRR_MASK        0x00200000
#define HAL_SRV_TOFFMIRR_TYPE        Bool
#define HAL_SRV_TOFFMIRR             HAL_SRV_TOFFMIRR_MASK
#define HAL_SRV_TOFFMIRR_READABLE    1
#define HAL_SRV_TOFFMIRR_WRITABLE    1
#define HAL_SRV_TOFFMIRR_SIGNED      0
#define HAL_SRV_TOFFMIRR_ADR         0xB00004BC

#define HAL_SRV_TOFFFNOK_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFFNOK_LSB         20
#define HAL_SRV_TOFFFNOK_MSB         20
#define HAL_SRV_TOFFFNOK_MASK        0x00100000
#define HAL_SRV_TOFFFNOK_TYPE        Bool
#define HAL_SRV_TOFFFNOK             HAL_SRV_TOFFFNOK_MASK
#define HAL_SRV_TOFFFNOK_READABLE    1
#define HAL_SRV_TOFFFNOK_WRITABLE    1
#define HAL_SRV_TOFFFNOK_SIGNED      0
#define HAL_SRV_TOFFFNOK_ADR         0xB00004BC

#define HAL_SRV_TOFFSUM_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFSUM_LSB         17
#define HAL_SRV_TOFFSUM_MSB         17
#define HAL_SRV_TOFFSUM_MASK        0x00020000
#define HAL_SRV_TOFFSUM_TYPE        Bool
#define HAL_SRV_TOFFSUM             HAL_SRV_TOFFSUM_MASK
#define HAL_SRV_TOFFSUM_READABLE    1
#define HAL_SRV_TOFFSUM_WRITABLE    1
#define HAL_SRV_TOFFSUM_SIGNED      0
#define HAL_SRV_TOFFSUM_ADR         0xB00004BC

#define HAL_SRV_TOFFTILT_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFTILT_LSB         16
#define HAL_SRV_TOFFTILT_MSB         16
#define HAL_SRV_TOFFTILT_MASK        0x00010000
#define HAL_SRV_TOFFTILT_TYPE        Bool
#define HAL_SRV_TOFFTILT             HAL_SRV_TOFFTILT_MASK
#define HAL_SRV_TOFFTILT_READABLE    1
#define HAL_SRV_TOFFTILT_WRITABLE    1
#define HAL_SRV_TOFFTILT_SIGNED      0
#define HAL_SRV_TOFFTILT_ADR         0xB00004BC

#define HAL_SRV_TOFFFOC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFFOC_LSB         15
#define HAL_SRV_TOFFFOC_MSB         15
#define HAL_SRV_TOFFFOC_MASK        0x00008000
#define HAL_SRV_TOFFFOC_TYPE        Bool
#define HAL_SRV_TOFFFOC             HAL_SRV_TOFFFOC_MASK
#define HAL_SRV_TOFFFOC_READABLE    1
#define HAL_SRV_TOFFFOC_WRITABLE    1
#define HAL_SRV_TOFFFOC_SIGNED      0
#define HAL_SRV_TOFFFOC_ADR         0xB00004BC

#define HAL_SRV_TOFFTRK_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_TOFFTRK_LSB         14
#define HAL_SRV_TOFFTRK_MSB         14
#define HAL_SRV_TOFFTRK_MASK        0x00004000
#define HAL_SRV_TOFFTRK_TYPE        Bool
#define HAL_SRV_TOFFTRK             HAL_SRV_TOFFTRK_MASK
#define HAL_SRV_TOFFTRK_READABLE    1
#define HAL_SRV_TOFFTRK_WRITABLE    1
#define HAL_SRV_TOFFTRK_SIGNED      0
#define HAL_SRV_TOFFTRK_ADR         0xB00004BC

#define HAL_SRV_FOFFFETHR_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFFETHR_LSB         13
#define HAL_SRV_FOFFFETHR_MSB         13
#define HAL_SRV_FOFFFETHR_MASK        0x00002000
#define HAL_SRV_FOFFFETHR_TYPE        Bool
#define HAL_SRV_FOFFFETHR             HAL_SRV_FOFFFETHR_MASK
#define HAL_SRV_FOFFFETHR_READABLE    1
#define HAL_SRV_FOFFFETHR_WRITABLE    1
#define HAL_SRV_FOFFFETHR_SIGNED      0
#define HAL_SRV_FOFFFETHR_ADR         0xB00004BC

#define HAL_SRV_FOFFFE2C_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFFE2C_LSB         12
#define HAL_SRV_FOFFFE2C_MSB         12
#define HAL_SRV_FOFFFE2C_MASK        0x00001000
#define HAL_SRV_FOFFFE2C_TYPE        Bool
#define HAL_SRV_FOFFFE2C             HAL_SRV_FOFFFE2C_MASK
#define HAL_SRV_FOFFFE2C_READABLE    1
#define HAL_SRV_FOFFFE2C_WRITABLE    1
#define HAL_SRV_FOFFFE2C_SIGNED      0
#define HAL_SRV_FOFFFE2C_ADR         0xB00004BC

#define HAL_SRV_FOFFFE1C_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFFE1C_LSB         11
#define HAL_SRV_FOFFFE1C_MSB         11
#define HAL_SRV_FOFFFE1C_MASK        0x00000800
#define HAL_SRV_FOFFFE1C_TYPE        Bool
#define HAL_SRV_FOFFFE1C             HAL_SRV_FOFFFE1C_MASK
#define HAL_SRV_FOFFFE1C_READABLE    1
#define HAL_SRV_FOFFFE1C_WRITABLE    1
#define HAL_SRV_FOFFFE1C_SIGNED      0
#define HAL_SRV_FOFFFE1C_ADR         0xB00004BC

#define HAL_SRV_FOFFTEOSC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFTEOSC_LSB         10
#define HAL_SRV_FOFFTEOSC_MSB         10
#define HAL_SRV_FOFFTEOSC_MASK        0x00000400
#define HAL_SRV_FOFFTEOSC_TYPE        Bool
#define HAL_SRV_FOFFTEOSC             HAL_SRV_FOFFTEOSC_MASK
#define HAL_SRV_FOFFTEOSC_READABLE    1
#define HAL_SRV_FOFFTEOSC_WRITABLE    1
#define HAL_SRV_FOFFTEOSC_SIGNED      0
#define HAL_SRV_FOFFTEOSC_ADR         0xB00004BC

#define HAL_SRV_FOFFPANIC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFPANIC_LSB         9
#define HAL_SRV_FOFFPANIC_MSB         9
#define HAL_SRV_FOFFPANIC_MASK        0x00000200
#define HAL_SRV_FOFFPANIC_TYPE        Bool
#define HAL_SRV_FOFFPANIC             HAL_SRV_FOFFPANIC_MASK
#define HAL_SRV_FOFFPANIC_READABLE    1
#define HAL_SRV_FOFFPANIC_WRITABLE    1
#define HAL_SRV_FOFFPANIC_SIGNED      0
#define HAL_SRV_FOFFPANIC_ADR         0xB00004BC

#define HAL_SRV_FOFFFSHC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFFSHC_LSB         8
#define HAL_SRV_FOFFFSHC_MSB         8
#define HAL_SRV_FOFFFSHC_MASK        0x00000100
#define HAL_SRV_FOFFFSHC_TYPE        Bool
#define HAL_SRV_FOFFFSHC             HAL_SRV_FOFFFSHC_MASK
#define HAL_SRV_FOFFFSHC_READABLE    1
#define HAL_SRV_FOFFFSHC_WRITABLE    1
#define HAL_SRV_FOFFFSHC_SIGNED      0
#define HAL_SRV_FOFFFSHC_ADR         0xB00004BC

#define HAL_SRV_FOFFMIRR_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFMIRR_LSB         7
#define HAL_SRV_FOFFMIRR_MSB         7
#define HAL_SRV_FOFFMIRR_MASK        0x00000080
#define HAL_SRV_FOFFMIRR_TYPE        Bool
#define HAL_SRV_FOFFMIRR             HAL_SRV_FOFFMIRR_MASK
#define HAL_SRV_FOFFMIRR_READABLE    1
#define HAL_SRV_FOFFMIRR_WRITABLE    1
#define HAL_SRV_FOFFMIRR_SIGNED      0
#define HAL_SRV_FOFFMIRR_ADR         0xB00004BC

#define HAL_SRV_FOFFFNOK_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFFNOK_LSB         6
#define HAL_SRV_FOFFFNOK_MSB         6
#define HAL_SRV_FOFFFNOK_MASK        0x00000040
#define HAL_SRV_FOFFFNOK_TYPE        Bool
#define HAL_SRV_FOFFFNOK             HAL_SRV_FOFFFNOK_MASK
#define HAL_SRV_FOFFFNOK_READABLE    1
#define HAL_SRV_FOFFFNOK_WRITABLE    1
#define HAL_SRV_FOFFFNOK_SIGNED      0
#define HAL_SRV_FOFFFNOK_ADR         0xB00004BC

#define HAL_SRV_FOFFSUM_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFSUM_LSB         3
#define HAL_SRV_FOFFSUM_MSB         3
#define HAL_SRV_FOFFSUM_MASK        0x00000008
#define HAL_SRV_FOFFSUM_TYPE        Bool
#define HAL_SRV_FOFFSUM             HAL_SRV_FOFFSUM_MASK
#define HAL_SRV_FOFFSUM_READABLE    1
#define HAL_SRV_FOFFSUM_WRITABLE    1
#define HAL_SRV_FOFFSUM_SIGNED      0
#define HAL_SRV_FOFFSUM_ADR         0xB00004BC

#define HAL_SRV_FOFFTILT_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFTILT_LSB         2
#define HAL_SRV_FOFFTILT_MSB         2
#define HAL_SRV_FOFFTILT_MASK        0x00000004
#define HAL_SRV_FOFFTILT_TYPE        Bool
#define HAL_SRV_FOFFTILT             HAL_SRV_FOFFTILT_MASK
#define HAL_SRV_FOFFTILT_READABLE    1
#define HAL_SRV_FOFFTILT_WRITABLE    1
#define HAL_SRV_FOFFTILT_SIGNED      0
#define HAL_SRV_FOFFTILT_ADR         0xB00004BC

#define HAL_SRV_FOFFFOC_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFFOC_LSB         1
#define HAL_SRV_FOFFFOC_MSB         1
#define HAL_SRV_FOFFFOC_MASK        0x00000002
#define HAL_SRV_FOFFFOC_TYPE        Bool
#define HAL_SRV_FOFFFOC             HAL_SRV_FOFFFOC_MASK
#define HAL_SRV_FOFFFOC_READABLE    1
#define HAL_SRV_FOFFFOC_WRITABLE    1
#define HAL_SRV_FOFFFOC_SIGNED      0
#define HAL_SRV_FOFFFOC_ADR         0xB00004BC

#define HAL_SRV_FOFFTRK_REGISTER    (HAL_SRVREG_MAP->SWOFFEN)
#define HAL_SRV_FOFFTRK_LSB         0
#define HAL_SRV_FOFFTRK_MSB         0
#define HAL_SRV_FOFFTRK_MASK        0x00000001
#define HAL_SRV_FOFFTRK_TYPE        Bool
#define HAL_SRV_FOFFTRK             HAL_SRV_FOFFTRK_MASK
#define HAL_SRV_FOFFTRK_READABLE    1
#define HAL_SRV_FOFFTRK_WRITABLE    1
#define HAL_SRV_FOFFTRK_SIGNED      0
#define HAL_SRV_FOFFTRK_ADR         0xB00004BC

#if (CHIP_REV >= 0xC0)

// Register DSPBADELCT
#define HAL_SRV_DSPBASEL_REGISTER    (HAL_SRVREG_MAP->DSPBADELCT)
#define HAL_SRV_DSPBASEL_LSB         16
#define HAL_SRV_DSPBASEL_MSB         16
#define HAL_SRV_DSPBASEL_MASK        0x00010000
#define HAL_SRV_DSPBASEL_TYPE        Bool
#define HAL_SRV_DSPBASEL             HAL_SRV_DSPBASEL_MASK
#define HAL_SRV_DSPBASEL_READABLE    1
#define HAL_SRV_DSPBASEL_WRITABLE    1
#define HAL_SRV_DSPBASEL_SIGNED      0
#define HAL_SRV_DSPBASEL_ADR         0xB00004D0

#define HAL_SRV_DSPBADELCT_REGISTER    (HAL_SRVREG_MAP->DSPBADELCT)
#define HAL_SRV_DSPBADELCT_LSB         0
#define HAL_SRV_DSPBADELCT_MSB         11
#define HAL_SRV_DSPBADELCT_MASK        0x00000FFF
#define HAL_SRV_DSPBADELCT_TYPE        UInt16
#define HAL_SRV_DSPBADELCT             HAL_SRV_DSPBADELCT_MASK
#define HAL_SRV_DSPBADELCT_READABLE    1
#define HAL_SRV_DSPBADELCT_WRITABLE    1
#define HAL_SRV_DSPBADELCT_SIGNED      0
#define HAL_SRV_DSPBADELCT_ADR         0xB00004D0

// Register LANDCTRL
#define HAL_SRV_TEHOLD_REGISTER    (HAL_SRVREG_MAP->LANDCTRL)
#define HAL_SRV_TEHOLD_LSB         10
#define HAL_SRV_TEHOLD_MSB         19
#define HAL_SRV_TEHOLD_MASK        0x000FFC00
#define HAL_SRV_TEHOLD_TYPE        UInt16
#define HAL_SRV_TEHOLD             HAL_SRV_TEHOLD_MASK
#define HAL_SRV_TEHOLD_READABLE    1
#define HAL_SRV_TEHOLD_WRITABLE    1
#define HAL_SRV_TEHOLD_SIGNED      0
#define HAL_SRV_TEHOLD_ADR         0xB00004D4

#define HAL_SRV_WOBHOLD_REGISTER    (HAL_SRVREG_MAP->LANDCTRL)
#define HAL_SRV_WOBHOLD_LSB         0
#define HAL_SRV_WOBHOLD_MSB         9
#define HAL_SRV_WOBHOLD_MASK        0x000003FF
#define HAL_SRV_WOBHOLD_TYPE        UInt16
#define HAL_SRV_WOBHOLD             HAL_SRV_WOBHOLD_MASK
#define HAL_SRV_WOBHOLD_READABLE    1
#define HAL_SRV_WOBHOLD_WRITABLE    1
#define HAL_SRV_WOBHOLD_SIGNED      0
#define HAL_SRV_WOBHOLD_ADR         0xB00004D4

// Register LSPDMCTRL1
#define HAL_SRV_LSFOOPDMSW_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSFOOPDMSW_LSB         22
#define HAL_SRV_LSFOOPDMSW_MSB         22
#define HAL_SRV_LSFOOPDMSW_MASK        0x00400000
#define HAL_SRV_LSFOOPDMSW_TYPE        Bool
#define HAL_SRV_LSFOOPDMSW             HAL_SRV_LSFOOPDMSW_MASK
#define HAL_SRV_LSFOOPDMSW_READABLE    1
#define HAL_SRV_LSFOOPDMSW_WRITABLE    1
#define HAL_SRV_LSFOOPDMSW_SIGNED      0
#define HAL_SRV_LSFOOPDMSW_ADR         0xB00004D8

#define HAL_SRV_LSTILTPDMSW_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSTILTPDMSW_LSB         21
#define HAL_SRV_LSTILTPDMSW_MSB         21
#define HAL_SRV_LSTILTPDMSW_MASK        0x00200000
#define HAL_SRV_LSTILTPDMSW_TYPE        Bool
#define HAL_SRV_LSTILTPDMSW             HAL_SRV_LSTILTPDMSW_MASK
#define HAL_SRV_LSTILTPDMSW_READABLE    1
#define HAL_SRV_LSTILTPDMSW_WRITABLE    1
#define HAL_SRV_LSTILTPDMSW_SIGNED      0
#define HAL_SRV_LSTILTPDMSW_ADR         0xB00004D8

#define HAL_SRV_LSTROPDMSW_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSTROPDMSW_LSB         20
#define HAL_SRV_LSTROPDMSW_MSB         20
#define HAL_SRV_LSTROPDMSW_MASK        0x00100000
#define HAL_SRV_LSTROPDMSW_TYPE        Bool
#define HAL_SRV_LSTROPDMSW             HAL_SRV_LSTROPDMSW_MASK
#define HAL_SRV_LSTROPDMSW_READABLE    1
#define HAL_SRV_LSTROPDMSW_WRITABLE    1
#define HAL_SRV_LSTROPDMSW_SIGNED      0
#define HAL_SRV_LSTROPDMSW_ADR         0xB00004D8

#define HAL_SRV_LSFOODIFEN_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSFOODIFEN_LSB         19
#define HAL_SRV_LSFOODIFEN_MSB         19
#define HAL_SRV_LSFOODIFEN_MASK        0x00080000
#define HAL_SRV_LSFOODIFEN_TYPE        Bool
#define HAL_SRV_LSFOODIFEN             HAL_SRV_LSFOODIFEN_MASK
#define HAL_SRV_LSFOODIFEN_READABLE    1
#define HAL_SRV_LSFOODIFEN_WRITABLE    1
#define HAL_SRV_LSFOODIFEN_SIGNED      0
#define HAL_SRV_LSFOODIFEN_ADR         0xB00004D8

#define HAL_SRV_LSTRODATSW_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSTRODATSW_LSB         18
#define HAL_SRV_LSTRODATSW_MSB         18
#define HAL_SRV_LSTRODATSW_MASK        0x00040000
#define HAL_SRV_LSTRODATSW_TYPE        Bool
#define HAL_SRV_LSTRODATSW             HAL_SRV_LSTRODATSW_MASK
#define HAL_SRV_LSTRODATSW_READABLE    1
#define HAL_SRV_LSTRODATSW_WRITABLE    1
#define HAL_SRV_LSTRODATSW_SIGNED      0
#define HAL_SRV_LSTRODATSW_ADR         0xB00004D8

#define HAL_SRV_LSFOODATSW_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSFOODATSW_LSB         17
#define HAL_SRV_LSFOODATSW_MSB         17
#define HAL_SRV_LSFOODATSW_MASK        0x00020000
#define HAL_SRV_LSFOODATSW_TYPE        Bool
#define HAL_SRV_LSFOODATSW             HAL_SRV_LSFOODATSW_MASK
#define HAL_SRV_LSFOODATSW_READABLE    1
#define HAL_SRV_LSFOODATSW_WRITABLE    1
#define HAL_SRV_LSFOODATSW_SIGNED      0
#define HAL_SRV_LSFOODATSW_ADR         0xB00004D8

#define HAL_SRV_LSTILTDATSW_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSTILTDATSW_LSB         16
#define HAL_SRV_LSTILTDATSW_MSB         16
#define HAL_SRV_LSTILTDATSW_MASK        0x00010000
#define HAL_SRV_LSTILTDATSW_TYPE        Bool
#define HAL_SRV_LSTILTDATSW             HAL_SRV_LSTILTDATSW_MASK
#define HAL_SRV_LSTILTDATSW_READABLE    1
#define HAL_SRV_LSTILTDATSW_WRITABLE    1
#define HAL_SRV_LSTILTDATSW_SIGNED      0
#define HAL_SRV_LSTILTDATSW_ADR         0xB00004D8

#define HAL_SRV_LSTRODAT_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL1)
#define HAL_SRV_LSTRODAT_LSB         0
#define HAL_SRV_LSTRODAT_MSB         14
#define HAL_SRV_LSTRODAT_MASK        0x00007FFF
#define HAL_SRV_LSTRODAT_TYPE        UInt16
#define HAL_SRV_LSTRODAT             HAL_SRV_LSTRODAT_MASK
#define HAL_SRV_LSTRODAT_READABLE    1
#define HAL_SRV_LSTRODAT_WRITABLE    1
#define HAL_SRV_LSTRODAT_SIGNED      0
#define HAL_SRV_LSTRODAT_ADR         0xB00004D8

// Register LSPDMCTRL2
#define HAL_SRV_LSFOODAT_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL2)
#define HAL_SRV_LSFOODAT_LSB         16
#define HAL_SRV_LSFOODAT_MSB         30
#define HAL_SRV_LSFOODAT_MASK        0x7FFF0000
#define HAL_SRV_LSFOODAT_TYPE        UInt16
#define HAL_SRV_LSFOODAT             HAL_SRV_LSFOODAT_MASK
#define HAL_SRV_LSFOODAT_READABLE    1
#define HAL_SRV_LSFOODAT_WRITABLE    1
#define HAL_SRV_LSFOODAT_SIGNED      0
#define HAL_SRV_LSFOODAT_ADR         0xB00004DC

#define HAL_SRV_LSTILTDAT_REGISTER    (HAL_SRVREG_MAP->LSPDMCTRL2)
#define HAL_SRV_LSTILTDAT_LSB         0
#define HAL_SRV_LSTILTDAT_MSB         14
#define HAL_SRV_LSTILTDAT_MASK        0x00007FFF
#define HAL_SRV_LSTILTDAT_TYPE        UInt16
#define HAL_SRV_LSTILTDAT             HAL_SRV_LSTILTDAT_MASK
#define HAL_SRV_LSTILTDAT_READABLE    1
#define HAL_SRV_LSTILTDAT_WRITABLE    1
#define HAL_SRV_LSTILTDAT_SIGNED      0
#define HAL_SRV_LSTILTDAT_ADR         0xB00004DC

// Register LSMSKCTL
#define HAL_SRV_SPCNTMUX_REGISTER    (HAL_SRVREG_MAP->LSMSKCTL)
#define HAL_SRV_SPCNTMUX_LSB         26
#define HAL_SRV_SPCNTMUX_MSB         26
#define HAL_SRV_SPCNTMUX_MASK        0x04000000
#define HAL_SRV_SPCNTMUX_TYPE        Bool
#define HAL_SRV_SPCNTMUX             HAL_SRV_SPCNTMUX_MASK
#define HAL_SRV_SPCNTMUX_READABLE    1
#define HAL_SRV_SPCNTMUX_WRITABLE    0
#define HAL_SRV_SPCNTMUX_SIGNED      0
#define HAL_SRV_SPCNTMUX_ADR         0xB00004E0

#define HAL_SRV_LSMASK2_REGISTER    (HAL_SRVREG_MAP->LSMSKCTL)
#define HAL_SRV_LSMASK2_LSB         16
#define HAL_SRV_LSMASK2_MSB         25
#define HAL_SRV_LSMASK2_MASK        0x03FF0000
#define HAL_SRV_LSMASK2_TYPE        UInt16
#define HAL_SRV_LSMASK2             HAL_SRV_LSMASK2_MASK
#define HAL_SRV_LSMASK2_READABLE    1
#define HAL_SRV_LSMASK2_WRITABLE    1
#define HAL_SRV_LSMASK2_SIGNED      0
#define HAL_SRV_LSMASK2_ADR         0xB00004E0

#define HAL_SRV_LSMASK1_REGISTER    (HAL_SRVREG_MAP->LSMSKCTL)
#define HAL_SRV_LSMASK1_LSB         0
#define HAL_SRV_LSMASK1_MSB         9
#define HAL_SRV_LSMASK1_MASK        0x000003FF
#define HAL_SRV_LSMASK1_TYPE        UInt16
#define HAL_SRV_LSMASK1             HAL_SRV_LSMASK1_MASK
#define HAL_SRV_LSMASK1_READABLE    1
#define HAL_SRV_LSMASK1_WRITABLE    1
#define HAL_SRV_LSMASK1_SIGNED      0
#define HAL_SRV_LSMASK1_ADR         0xB00004E0

#endif // #if (CHIP_REV >= 0xC0)

// Enumerations
typedef enum
{
	halSRV_SEKTBAD_SpdPhAcc_0h = 0,    // Speed phase and acceleration value =  0
	halSRV_SEKTBAD_SpdPhAcc_1h = 1,    // Speed phase and acceleration value =  1
	halSRV_SEKTBAD_SpdPhAcc_2h = 2,    // Speed phase and acceleration value =  2
	halSRV_SEKTBAD_SpdPhAcc_3h = 3,    // Speed phase and acceleration value =  3
	halSRV_SEKTBAD_SpdPhAcc_4h = 4,    // Speed phase and acceleration value =  4
	halSRV_SEKTBAD_SpdPhAcc_5h = 5,    // Speed phase and acceleration value =  5
	halSRV_SEKTBAD_SpdPhAcc_6h = 6,    // Speed phase and acceleration value =  6
	halSRV_SEKTBAD_SpdPhAcc_7h = 7,    // Speed phase and acceleration value =  7
	halSRV_SEKTBAD_SpdPhAcc_8h = 8,    // Speed phase and acceleration value =  8
	halSRV_SEKTBAD_SpdPhAcc_9h = 9,    // Speed phase and acceleration value =  9
	halSRV_SEKTBAD_SpdPhAcc_Ah = 10,    // Speed phase and acceleration value = 10
	halSRV_SEKTBAD_SpdPhAcc_Bh = 11     // Speed phase and acceleration value = 11
} halSRV_SEKTBAD_SpdPhAcc_t;

typedef enum
{
	halSRV_ZCSELScaling_Div4  = 0,    // zero crossing level selection = 1/4 scaling
	halSRV_ZCSELScaling_Div8  = 1,    // zero crossing level selection = 1/8 scaling
	halSRV_ZCSELScaling_Div16 = 2,    // zero crossing level selection = 1/16 scaling
	halSRV_ZCSELScaling_Div32 = 3     // zero crossing level selection = 1/32 scaling
} halSRV_ZCSELScaling_t;

typedef enum
{
	halSRV_PROTECT_AllDisable  = 0,    // disable brake and kick protective function
	halSRV_PROTECT_BrakeEnable = 1,    // enable brake protective function
	halSRV_PROTECT_KickEnable  = 2     // enable kick protective function
} halSRV_PROTECT_t;

typedef enum
{
	halSRV_KICK_700us_BRAKE_500us   = 0,
	halSRV_KICK_900us_BRAKE_700us   = 1,
	halSRV_KICK_1200us_BRAKE_900us  = 2,
	halSRV_KICK_1400us_BRAKE_1200us = 3 
} halSRV_KCKBRKSEL_t;

typedef enum
{
	halSRV_KSLO_1378_X_8   = 0,    // integrator sampling Freq = 1378 x 8
	halSRV_KSLO_1378_X_4   = 1,    // integrator sampling Freq = 1378 x 4
	halSRV_KSLO_1378_X_2   = 2,    // integrator sampling Freq = 1378 x 2
	halSRV_KSLO_1378       = 3,    // integrator sampling Freq = 1378 x 1
	halSRV_KSLO_1378_DIV_2 = 4,    // integrator sampling Freq = 1378 / 2
	halSRV_KSLO_1378_DIV_4 = 5,    // integrator sampling Freq = 1378 / 4
	halSRV_KSLO_1378_DIV_8 = 6     // integrator sampling Freq = 1378 / 8
} halSRV_IntegratorSamplingFreq_t;

typedef enum
{
	halSRV_STPDCOCK_1378_X_4  = 0,    // DCO clock frequency = 4 x 1378 (default)
	halSRV_STPDCOCK_1378_X_8  = 1,    // DCO clock frequency = 8 x 1378
	halSRV_STPDCOCK_1378_X_16 = 2,    // DCO clock frequency = 16 x 1378
	halSRV_STPDCOCK_1378_X_32 = 3,    // DCO clock frequency = 32 x 1378
	halSRV_STPDCOCK_1378_X_64 = 4     // DCO clock frequency = 64 x 1378
} halSRV_DCOClock_t;

typedef enum
{
	halSRV_MonSwTilt           = 0,    // Monitor Tilt
	halSRV_MonSwFocusLpf       = 1,    // Monitor Focus LPF
	halSRV_MonSwTrackLpf       = 2,    // Monitor Tracking LPF
	halSRV_MonSwFineSrchVelErr = 3,    // Monitor Fine Search Velocity Error
	halSRV_MonSwROC            = 4,    // Monitor ROC Output
	halSRV_MonSwVref1          = 5,    // Monitor Vref
	halSRV_MonSwVref2          = 6,    // Monitor Vref
	halSRV_MonSwVref3          = 7     // Monitor Vref
} halSRV_MonitorSignalsOnTiltDAC_t;

typedef enum
{
	halSRV_DecChg_Delay_32EFMFrame    = 0,    // Dec mode change delay = 32 EFM frames
	halSRV_DecChg_Delay_64EFMFrame    = 1,    // Dec mode change delay = 64 EFM frames
	halSRV_DecChg_Delay_128EFMFrame   = 2,    // Dec mode change delay = 128 EFM frames
	halSRV_DecChg_Delay_256EFMFrame   = 3,    // Dec mode change delay = 256 EFM frames
	halSRV_DecChg_Delay_512EFMFrame   = 4,    // Dec mode change delay = 512 EFM frames
	halSRV_DecChg_Delay_1024EFMFrame  = 5,    // Dec mode change delay = 1024 EFM frames
	halSRV_DecChg_Delay_2048EFMFrame6 = 6,    // Dec mode change delay = 2048 EFM frames
	halSRV_DecChg_Delay_2048EFMFrame7 = 7     // Dec mode change delay = 2048 EFM frames
} halSRV_DecChgDelay_t;

typedef enum
{
	halSRV_SrvMod_M_KICK       = 0,    // manual kick mode
	halSRV_SrvMod_M_FG         = 1,    // manual FG mode
	halSRV_SrvMod_M_WBL        = 2,    // manual wobble mode
	halSRV_SrvMod_M_DEC        = 3,    // manual decode mode
	halSRV_SrvMod_A_KICK_FG    = 4,    // auto kick -> FG mode
	halSRV_SrvMod_A_FG_DEC     = 5,    // auto FG <-> DEC mode
	halSRV_SrvMod_A_FG_WBL     = 6,    // auto FG <-> WBL mode
	halSRV_SrvMod_A_FG_WBL_DEC = 7     // auto FG <-> WBL -> DEC -> FG mode
} halSRV_SRVMOD_t;

typedef enum
{
	halSRV_CurMod_KICK = 0,    // current servo mode -> KICK mode
	halSRV_CurMod_FG   = 1,    // current servo mode -> FG mode
	halSRV_CurMod_WBL  = 2,    // current servo mode -> WBL mode
	halSRV_CurMod_DEC  = 3     // current servo mode -> DEC mode
} halSRV_CurMod_t;

typedef enum
{
	halSRV_ApcGain_Div1     = 0,    // APC output gain control gain value = 1
	halSRV_ApcGain_Div2     = 1,    // APC output gain control gain value = 1/2
	halSRV_ApcGain_Div4     = 2,    // APC output gain control gain value = 1/4
	halSRV_ApcGain_Div8     = 3,    // APC output gain control gain value = 1/8
	halSRV_ApcGain_Div16    = 4,    // APC output gain control gain value = 1/16
	halSRV_ApcGain_Div32    = 5,    // APC output gain control gain value = 1/32
	halSRV_ApcGain_Div64    = 6,    // APC output gain control gain value = 1/64
	halSRV_ApcGain_Div128   = 7,    // APC output gain control gain value = 1/128
	halSRV_ApcGain_Div256   = 8,    // APC output gain control gain value = 1/256
	halSRV_ApcGain_Div512   = 9,    // APC output gain control gain value = 1/512
	halSRV_ApcGain_Div1024  = 10,    // APC output gain control gain value = 1/1024
	halSRV_ApcGain_Div2048  = 11,    // APC output gain control gain value = 1/2048
	halSRV_ApcGain_Div4096  = 12,    // APC output gain control gain value = 1/4096
	halSRV_ApcGain_Div8192  = 13,    // APC output gain control gain value = 1/8192
	halSRV_ApcGain_Div16384 = 14,    // APC output gain control gain value = 1/16384
	halSRV_ApcGain_Div32768 = 15     // APC output gain control gain value = 1/32768
} halSRV_ApcGain_t;

typedef enum
{
	halSRV_TENOISEN_NoAveraging = 0,    // no averaging
	halSRV_TENOISEN_Div2        = 1,    // track error servo oscillation detection noise chancel select = (TE1+TE0)/2
	halSRV_TENOISEN_Div4_2      = 2,    // track error servo oscillation detection noise chancel select = (TE3+TE2+TE1+TE0)/4
	halSRV_TENOISEN_Div4_3      = 3     // track error servo oscillation detection noise chancel select = (TE3+TE2+TE1+TE0)/4
} halSRV_TENOISEN_t;

typedef enum
{
	halSRV_FE1NOISEN_NoAveraging = 0,    // no averaging
	halSRV_FE1NOISEN_Div2        = 1,    // focus error servo oscillation detection 1 noise chancel select = (FE1+FE0)/2
	halSRV_FE1NOISEN_Div4_2      = 2,    // focus error servo oscillation detection 1 noise chancel select = (FE3+FE2+FE1+FE0)/4
	halSRV_FE1NOISEN_Div4_3      = 3     // focus error servo oscillation detection 1 noise chancel select = (FE3+FE2+FE1+FE0)/4
} halSRV_FE1NOISEN_t;

typedef enum
{
	halSRV_FE2NOISEN_NoAveraging = 0,    // no averaging
	halSRV_FE2NOISEN_Div2        = 1,    // focus error servo oscillation detection 2 noise chancel select = (FE1+FE0)/2
	halSRV_FE2NOISEN_Div4_2      = 2,    // focus error servo oscillation detection 2 noise chancel select = (FE3+FE2+FE1+FE0)/4
	halSRV_FE2NOISEN_Div4_3      = 3     // focus error servo oscillation detection 2 noise chancel select = (FE3+FE2+FE1+FE0)/4
} halSRV_FE2NOISEN_t;

typedef enum
{
	halSRV_AfcGain_x_Div1   = 0,    // AFC output gain control, gain factor x 1
	halSRV_AfcGain_x_Div2   = 1,    // AFC output gain control, gain factor x 1/2
	halSRV_AfcGain_x_Div4   = 2,    // AFC output gain control, gain factor x 1/4
	halSRV_AfcGain_x_Div8   = 3,    // AFC output gain control, gain factor x 1/8
	halSRV_AfcGain_x_Div16  = 4,    // AFC output gain control, gain factor x 1/16
	halSRV_AfcGain_x_Div32  = 5,    // AFC output gain control, gain factor x 1/32
	halSRV_AfcGain_x_Div64  = 6,    // AFC output gain control, gain factor x 1/64
	halSRV_AfcGain_x_Div128 = 7     // AFC output gain control, gain factor x 1/128
} halSRV_AfcGain_t;

typedef enum
{
	halSRV_NDIV_Gain_x_1    = 1,    // AFC input signal divide control, gain factor x 1
	halSRV_NDIV_Gain_x_2    = 2,    // AFC input signal divide control, gain factor x 1/2
	halSRV_NDIV_Gain_x_4    = 4,    // AFC input signal divide control, gain factor x 1/4
	halSRV_NDIV_Gain_x_8    = 8,    // AFC input signal divide control, gain factor x 1/8
	halSRV_NDIV_Gain_x_16   = 16,    // AFC input signal divide control, gain factor x 1/16
	halSRV_NDIV_Gain_x_32   = 32,    // AFC input signal divide control, gain factor x 1/32
	halSRV_NDIV_Gain_x_64   = 64,    // AFC input signal divide control, gain factor x 1/64
	halSRV_NDIV_Gain_x_128  = 128,    // AFC input signal divide control, gain factor x 1/128
	halSRV_NDIV_Gain_x_256  = 256,    // AFC input signal divide control, gain factor x 1/256
	halSRV_NDIV_Gain_x_512  = 512,    // AFC input signal divide control, gain factor x 1/512
	halSRV_NDIV_Gain_x_2048 = 0     // AFC input signal divide control, gain factor x 1/2048
} halSRV_NDIV_Gain_t;

typedef enum
{
	halSRV_PC_Off0_On100       = 0,    // PAM off duty cycle,off:    0%  on:  100%
	halSRV_PC_Off6p25_On93p75  = 1,    // PAM off duty cycle,off: 6.25%  on:93.75%
	halSRV_PC_Off12p50_On87p50 = 2,    // PAM off duty cycle,off:12.50%  on:87.50%
	halSRV_PC_Off18p75_On81p25 = 3,    // PAM off duty cycle,off:18.75%  on:81.25%
	halSRV_PC_Off25p00_On75p00 = 4,    // PAM off duty cycle,off:25.00%  on:75.00%
	halSRV_PC_Off31p25_On68p75 = 5,    // PAM off duty cycle,off:31.25%  on:68.75%
	halSRV_PC_Off37p50_On62p50 = 6,    // PAM off duty cycle,off:37.50%  on:62.50%
	halSRV_PC_Off43p75_On56p25 = 7,    // PAM off duty cycle,off:43.75%  on:56.25%
	halSRV_PC_Off50p00_On50p00 = 8,    // PAM off duty cycle,off:50.00%  on:50.00%
	halSRV_PC_Off56p25_On43p25 = 9,    // PAM off duty cycle,off:56.25%  on:43.25%
	halSRV_PC_Off62p50_On37p50 = 10,    // PAM off duty cycle,off:62.50%  on:37.50%
	halSRV_PC_Off68p75_On31p25 = 11,    // PAM off duty cycle,off:68.75%  on:31.25%
	halSRV_PC_Off75p00_On25p00 = 12,    // PAM off duty cycle,off:75.00%  on:25.00%
	halSRV_PC_Off81p25_On18p75 = 13,    // PAM off duty cycle,off:81.25%  on:18.75%
	halSRV_PC_Off87p50_On12p50 = 14,    // PAM off duty cycle,off:87.50%  on:12.50%
	halSRV_PC_Off93p75_On6Pp5  = 15     // PAM off duty cycle,off:93.75%  on: 6.25%
} halSRV_PC_t;

typedef enum
{
	halSRV_PF_172p200Hz = 0,    // PAM frequency control =172.200 Hz
	halSRV_PF_86p100Hz  = 1,    // PAM frequency control = 86.100 Hz
	halSRV_PF_57p400Hz  = 2,    // PAM frequency control = 57.400 Hz
	halSRV_PF_43p050Hz  = 3,    // PAM frequency control = 43.050 Hz
	halSRV_PF_34p440Hz  = 4,    // PAM frequency control = 34.440 Hz
	halSRV_PF_28p700Hz  = 5,    // PAM frequency control = 28.700 Hz
	halSRV_PF_24p600Hz  = 6,    // PAM frequency control = 24.600 Hz
	halSRV_PF_21p525Hz  = 7,    // PAM frequency control = 21.525 Hz
	halSRV_PF_19p133Hz  = 8,    // PAM frequency control = 19.133 Hz
	halSRV_PF_17p220Hz  = 9,    // PAM frequency control = 17.220 Hz
	halSRV_PF_15p655Hz  = 10,    // PAM frequency control = 15.655 Hz
	halSRV_PF_14p350Hz  = 11,    // PAM frequency control = 14.350 Hz
	halSRV_PF_13p246Hz  = 12,    // PAM frequency control = 13.246 Hz
	halSRV_PF_12p300Hz  = 13,    // PAM frequency control = 12.300 Hz
	halSRV_PF_11p480Hz  = 14,    // PAM frequency control = 11.480 Hz
	halSRV_PF_10p763Hz  = 15     // PAM frequency control = 10.763 Hz
} halSRV_PF_t;

typedef enum
{
	halSRV_WIDTH_P7ms  = 0,    // focus error disturbance width = 0.7 ms
	halSRV_WIDTH_1P4ms = 1     // focus error disturbance width = 1.4 ms
} halSRV_WIDTH_t;

typedef enum
{
	halSRV_FVKBF_Forward  = 0,    // search direction -> forward  direction
	halSRV_FVKBF_Backward = 1     // search direction -> backward direction
} halSRV_FVKBF_t;

typedef enum
{
	halSRV_DAC2SEL_OutputSLO      = 0,    // Output SLO signal for DC brushless motor controlled sled
	halSRV_DAC2SEL_OutputSTEP2    = 1,    // Output STEP2 for stepper motor controlled sled
	halSRV_DAC2SEL_OutputFromDAT2 = 2     // Output from DACDAT2(9-0)
} halSRV_DAC2SEL_t;

typedef enum
{
	halSRV_DAC1SEL_OutputFromDAT1 = 0,    // Output from DACDAT1(9-0)
	halSRV_DAC1SEL_OutputSTEP1    = 1     // Output STEP1 for stepper motor controlled sled
} halSRV_DAC1SEL_t;

typedef enum
{
	halSRV_BLANK_DataArea  = 0,    // Data  area detected during seek
	halSRV_BLANK_BlankArea = 1     // Blank area detected during seek
} halSRV_BLANK_t;

typedef enum
{
	halSRV_STALL_TrackAbove = 0,    // Track crossing above stall frequency
	halSRV_STALL_TrackBelow = 1     // Track crossing below stall frequency-Refer to STLFRQ
} halSRV_STALL_t;

typedef enum
{
	halSRV_FKDIR_RXLeadTX = 0,    // fine search kick direction -> RX leads TX
	halSRV_FKDIR_TXLeadRX = 1     // fine search kick direction -> TX leads RX
} halSRV_FKDIR_t;

typedef enum
{
	halSRV_KICKDIR_Forward  = 0,    // forward  direction during kick/searcht
	halSRV_KICKDIR_Backward = 1     // backward direction during kick/search
} halSRV_KICKDIR_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halSRV_SLOW_TrackSpd_GT_RefSpd = 0,    // track crossing speed > reference speed set by FVD(1-0)
	halSRV_SLOW_TrackSpd_LT_RefSpd = 1     // track crossing speed < reference speed set by FVD(1-0)
} halSRV_SLOW_t;


#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halSRV_SLOW_TrackSpd_GT_RefSpd = 0,    // track crossing speed > reference speed set by FVD(6-0)
	halSRV_SLOW_TrackSpd_LT_RefSpd = 1     // track crossing speed < reference speed set by FVD(6-0)
} halSRV_SLOW_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halSRV_CKBF_Forward  = 0,    // forward  direction
	halSRV_CKBF_Backward = 1     // backward direction
} halSRV_CKBF_t;

typedef enum
{
	halSRV_CKICT_62ms  = 0,    // kick time interval ->  62 ms
	halSRV_CKICT_124ms = 1,    // kick time interval -> 124 ms
	halSRV_CKICT_248ms = 2,    // kick time interval -> 248 ms
	halSRV_CKICT_495ms = 3     // kick time interval -> 495 ms
} halSRV_CKICT_t;

typedef enum
{
	halSRV_bWGP_Dis    = 0,    // disabled
	halSRV_bWGP_50us   = 1,    // 50 us
	halSRV_bWGP_100us  = 2,    // 100 us
	halSRV_bWGP_150us  = 3,    // 150 us
	halSRV_bWGP_200us  = 4,    // 200 us
	halSRV_bWGP_250us  = 5,    // 250 us
	halSRV_bWGP_300us  = 6,    // 300 us
	halSRV_bWGP_350us  = 7,    // 350 us
	halSRV_bWGP_400us  = 8,    // 400 us
	halSRV_bWGP_450us  = 9,    // 450 us
	halSRV_bWGP_500us  = 10,    // 500 us
	halSRV_bWGP_600us  = 11,    // 600 us
	halSRV_bWGP_700us  = 12,    // 700 us
	halSRV_bWGP_800us  = 13,    // 800 us
	halSRV_bWGP_900us  = 14,    // 900 us
	halSRV_bWGP_1000us = 15     // 1000 us
} halSRV_WriteGatePosEdgeDCHold_t;

typedef enum
{
	halSRV_bWGN_Dis    = 0,    // disabled
	halSRV_bWGN_50us   = 1,    // 50 us
	halSRV_bWGN_100us  = 2,    // 100 us
	halSRV_bWGN_150us  = 3,    // 150 us
	halSRV_bWGN_200us  = 4,    // 200 us
	halSRV_bWGN_250us  = 5,    // 250 us
	halSRV_bWGN_300us  = 6,    // 300 us
	halSRV_bWGN_350us  = 7,    // 350 us
	halSRV_bWGN_400us  = 8,    // 400 us
	halSRV_bWGN_450us  = 9,    // 450 us
	halSRV_bWGN_500us  = 10,    // 500 us
	halSRV_bWGN_600us  = 11,    // 600 us
	halSRV_bWGN_700us  = 12,    // 700 us
	halSRV_bWGN_800us  = 13,    // 800 us
	halSRV_bWGN_900us  = 14,    // 900 us
	halSRV_bWGN_1000us = 15     // 1000 us
} halSRV_WriteGateNegEdgeDCHold_t;

typedef enum
{
	halSRV_MRWDFS_Servo_defect = 0,    // use servo defect
	halSRV_MRWDFS_AFE_defect   = 1     // use AFE defect
} halSRV_MRWDFS_t;

typedef enum
{
	halSRV_DSECSEL_UseBlock  = 0,    // use DVD block  sync signal for defect counter logic
	halSRV_DSECSEL_UseSector = 1     // use DVD sector sync signal for defect counter logic
} halSRV_DSECSEL_t;

typedef enum
{
	halSRV_ROMDISC_R_RW_Mode = 0,    // R/RW mode
	halSRV_ROMDISC_ROM_Mode  = 1     // ROM  mode
} halSRV_ROMDISC_t;

typedef enum
{
	halSRV_DIVSEL_Wob1_Servo2_clk   = 0,    // R/RW Mode(wobble clocks)=1 ,ROM Mode(servo clocks)=2
	halSRV_DIVSEL_Wob2_Servo3_clk   = 1,    // R/RW Mode(wobble clocks)=2 ,ROM Mode(servo clocks)=3
	halSRV_DIVSEL_Wob3_Servo4_clk   = 2,    // R/RW Mode(wobble clocks)=3 ,ROM Mode(servo clocks)=4
	halSRV_DIVSEL_Wob4_Servo5_clk   = 3,    // R/RW Mode(wobble clocks)=4 ,ROM Mode(servo clocks)=5
	halSRV_DIVSEL_Wob5_Servo6_clk   = 4,    // R/RW Mode(wobble clocks)=5 ,ROM Mode(servo clocks)=6
	halSRV_DIVSEL_Wob6_Servo7_clk   = 5,    // R/RW Mode(wobble clocks)=6 ,ROM Mode(servo clocks)=7
	halSRV_DIVSEL_Wob7_Servo8_clk   = 6,    // R/RW Mode(wobble clocks)=7 ,ROM Mode(servo clocks)=8
	halSRV_DIVSEL_Wob8_Servo9_clk   = 7,    // R/RW Mode(wobble clocks)=8 ,ROM Mode(servo clocks)=9
	halSRV_DIVSEL_Wob9_Servo10_clk  = 8,    // R/RW Mode(wobble clocks)=9 ,ROM Mode(servo clocks)=10
	halSRV_DIVSEL_Wob10_Servo11_clk = 9,    // R/RW Mode(wobble clocks)=10,ROM Mode(servo clocks)=11
	halSRV_DIVSEL_Wob11_Servo12_clk = 10,    // R/RW Mode(wobble clocks)=11,ROM Mode(servo clocks)=12
	halSRV_DIVSEL_Wob12_Servo13_clk = 11,    // R/RW Mode(wobble clocks)=12,ROM Mode(servo clocks)=13
	halSRV_DIVSEL_Wob13_Servo14_clk = 12,    // R/RW Mode(wobble clocks)=13,ROM Mode(servo clocks)=14
	halSRV_DIVSEL_Wob14_Servo15_clk = 13,    // R/RW Mode(wobble clocks)=14,ROM Mode(servo clocks)=15
	halSRV_DIVSEL_Wob15_Servo16_clk = 14,    // R/RW Mode(wobble clocks)=15,ROM Mode(servo clocks)=16
	halSRV_DIVSEL_Wob1_ServoNo_clk  = 15     // R/RW Mode(wobble clocks)=1 ,ROM Mode(servo clocks)=reserved
} halSRV_DIVSEL_t;

typedef enum
{
	halSRV_TRERRBKEN_BLANK    = 0,    // BLANK
	halSRV_TRERRBKEN_NOT_RECD = 1     // !RECD
} halSRV_TRERRBKEN_t;

typedef enum
{
	halSRV_DBTRES_DBT_2p8us  = 0,    // DBT(6-0) resolution is (1 / 352.8 kHz) =  2.8 us
	halSRV_DBTRES_DBT_11p3us = 1     // DBT(6-0) resolution is (1 /  88.2 kHz) = 11.3 us
} halSRV_DBTRES_t;

typedef enum
{
	halSRV_FBW_8p2mv  = 0,    // Adjustment Window(8-bit ADC with 2.4V Full Scale)=  8.2 mv
	halSRV_FBW_16p4mv = 1,    // Adjustment Window(8-bit ADC with 2.4V Full Scale)= 16.4 mv
	halSRV_FBW_32p8mv = 2,    // Adjustment Window(8-bit ADC with 2.4V Full Scale)= 32.8 mv
	halSRV_FBW_65p6mv = 3     // Adjustment Window(8-bit ADC with 2.4V Full Scale)= 65.6 mv
} halSRV_FBW_t;

typedef enum
{
	halSRV_FGW_5p625degree = 0,    // Adjustment Window= 5.625 degree
	halSRV_FGW_11p25degree = 1,    // Adjustment Window= 11.25 degree
	halSRV_FGW_22p5degree  = 2,    // Adjustment Window= 22.5  degree
	halSRV_FGW_45degree    = 3     // Adjustment Window= 45    degree
} halSRV_FGW_t;

typedef enum
{
	halSRV_TGW_5p625degree = 0,    // Adjustment Window= 5.625 degree
	halSRV_TGW_11p25degree = 1,    // Adjustment Window= 11.25 degree
	halSRV_TGW_22p5degree  = 2,    // Adjustment Window= 22.5  degree
	halSRV_TGW_45degree    = 3     // Adjustment Window= 45    degree
} halSRV_TGW_t;

typedef enum
{
	halSRV_FBAVS_44Hz  = 0,    // need to set FBAVS4=0 -> test square wave= 44 Hz
	halSRV_FBAVS_176Hz = 0,    // need to set FBAVS4=1 -> test square wave=176 Hz
	halSRV_FBAVS_88Hz  = 1,    // need to set FBAVS4=0 -> test square wave= 88 Hz
	halSRV_FBAVS_352Hz = 1     // need to set FBAVS4=1 -> test square wave=352 Hz
} halSRV_FBAVS_t;

typedef enum
{
	halSRV_FQWDSL_2p5ms   = 0,    // need to set FQWDS4=0 -> tracking error zero crossing time window=2.5   ms
	halSRV_FQWDSL_0p625ms = 0,    // need to set FQWDS4=1 -> tracking error zero crossing time window=0.625 ms
	halSRV_FQWDSL_3p5ms   = 1,    // need to set FQWDS4=0 -> tracking error zero crossing time window=3.5   ms
	halSRV_FQWDSL_0p875ms = 1     // need to set FQWDS4=1 -> tracking error zero crossing time window=0.875 ms
} halSRV_FQWDSL_t;

typedef enum
{
	halSRV_FBAVS4_44Hz  = 0,    // need to set FBAVS=0 -> test square wave= 44 Hz
	halSRV_FBAVS4_88Hz  = 0,    // need to set FBAVS=1 -> test square wave= 88 Hz
	halSRV_FBAVS4_176Hz = 1,    // need to set FBAVS=0 -> test square wave=176 Hz
	halSRV_FBAVS4_352Hz = 1     // need to set FBAVS=1 -> test square wave=352 Hz
} halSRV_FBAVS4_t;

typedef enum
{
	halSRV_FQWDS4_2p5ms   = 0,    // need to set FQWDSL=0 -> tracking error zero crossing time window=2.5   ms
	halSRV_FQWDS4_3p5ms   = 0,    // need to set FQWDSL=1 -> tracking error zero crossing time window=3.5   ms
	halSRV_FQWDS4_0p625ms = 1,    // need to set FQWDSL=0 -> tracking error zero crossing time window=0.625 ms
	halSRV_FQWDS4_0p875ms = 1     // need to set FQWDSL=1 -> tracking error zero crossing time window=0.875 ms
} halSRV_FQWDS4_t;

typedef enum
{
	halSRV_BSRPSEL_BS   = 0,    // Select BS   for focus balance
	halSRV_BSRPSEL_RFRP = 1     // Select RFRP for focus balance
} halSRV_BSRPSEL_t;

typedef enum
{
	halSRV_FGAVS_Avg32Samples = 0,    // Average 32-block samples of fetched data of focus-gain adjustment to generate focus error signal
	halSRV_FGAVS_Avg64Samples = 1     // Average 64-block samples of fetched data of focus-gain adjustment to generate focus error signal
} halSRV_FGAVS_t;

typedef enum
{
	halSRV_TGAVS_Avg32Samples = 0,    // Average 32-block samples of fetched data of tracking-gain adjustment to generate tracking error signal
	halSRV_TGAVS_Avg64Samples = 1     // Average 64-block samples of fetched data of tracking-gain adjustment to generate tracking error signal
} halSRV_TGAVS_t;

typedef enum
{
	halSRV_RSEL_FBA       = 0,    // focus balance adjust register
	halSRV_RSEL_FGA       = 1,    // focus gain adjust register
	halSRV_RSEL_TGA       = 3,    // tracking gain adjust register
	halSRV_RSEL_FEO       = 4,    // focus offset adjust register
	halSRV_RSEL_TEO       = 5,    // tracking offset adjust register
	halSRV_RSEL_JUMP      = 6,    // jump track number register
	halSRV_RSEL_RPO       = 8,    // ripple offset adjust register
	halSRV_RSEL_BSO       = 9,    // beam strength offset adjust register
	halSRV_RSEL_BS        = 11,    // beam strength level register
	halSRV_RSEL_RFRP      = 12,    // ripple level register
	halSRV_RSEL_FE        = 13,    // focuslevel register
	halSRV_RSEL_TE        = 14,    // tracklevel register
	halSRV_RSEL_TRO       = 15,    // TRO DAC (10-bit) register
	halSRV_RSEL_FOO       = 16,    // FOO DAC (10-bit) register
	halSRV_RSEL_BSPEAK    = 17,    // BS peak
	halSRV_RSEL_BSBOTTOM  = 18,    // BS bottom
	halSRV_RSEL_RPPEAK    = 19,    // RP peak
	halSRV_RSEL_RPBOTTOM  = 20,    // RP bottom
	halSRV_RSEL_FEPEAK    = 21,    // FE peak
	halSRV_RSEL_FEBOTTOM  = 22,    // FE bottom
	halSRV_RSEL_TEPEAK    = 23,    // TE peak
	halSRV_RSEL_TEBOTTOM  = 24,    // TE bottom
	halSRV_RSEL_FMTHRESHO = 25,    // FM_THRESH offset (10-bit)
	halSRV_RSEL_FMDELTAO  = 26,    // FM_DELTA offset (10-bit)
	halSRV_RSEL_FMTHRESH  = 27,    // FM_THRESH (10-bit)
	halSRV_RSEL_FMDELTA   = 28     // FM_DELTA (10-bit)
} halSRV_ReadWriteSelect_t;

typedef enum
{
	halSRV_FILT_11MHz  = 0,    // Run-Out Compensation FG Filter Sampling Frequency =  11 MHz
	halSRV_FILT_176KHz = 1     // Run-Out Compensation FG Filter Sampling Frequency = 176 kHz
} halSRV_FILT_t;

typedef enum
{
	halSRV_AGC_1P38_KHZ = 0,    // AGC freq = 1.38 kHz
	halSRV_AGC_2P75_KHZ = 1,    // AGC freq = 2.75 kHz
	halSRV_AGC_2P00_KHZ = 2,    // AGC freq = 2.00 kHz
	halSRV_AGC_4P00_KHZ = 3     // AGC freq = 4.00 kHz
} halSRV_AGCFreq_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halSRV_FOKT_Stable10ms_Lost2ms = 0,    // Set Focus Times -> Stable Time = 10 ms, Lost Time = 2 ms
	halSRV_FOKT_Stable10ms_Lost4ms = 1,    // Set Focus Times -> Stable Time = 10 ms, Lost Time = 4 ms
	halSRV_FOKT_Stable10ms_Lost6ms = 2,    // Set Focus Times -> Stable Time = 10 ms, Lost Time = 6 ms
	halSRV_FOKT_Stable31ms_Lost8ms = 3     // Set Focus Times -> Stable Time = 31 ms, Lost Time = 8 ms
} halSRV_FOKT_t;

#endif // #if (CHIP_REV <= 0xB3)

typedef enum
{
	halSRV_FSR_UpDown278ms = 0,    // Set Focus Up/Down Time = 278 ms and Focus Search Slew Rate = 20.8 x (kf18/32767) Volts/sec
	halSRV_FSR_UpDown372ms = 1,    // Set Focus Up/Down Time = 372 ms and Focus Search Slew Rate = 15.5 x (kf18/32767) Volts/sec
	halSRV_FSR_UpDown557ms = 2,    // Set Focus Up/Down Time = 557 ms and Focus Search Slew Rate = 10.4 x (kf18/32767) Volts/sec
	halSRV_FSR_UpDown743ms = 3     // Set Focus Up/Down Time = 743 ms and Focus Search Slew Rate =  7.8 x (kf18/32767) Volts/sec
} halSRV_FSR_t;

typedef enum
{
	halSRV_FSM_MaxAmp_32Div1024  = 0,    // Maximum Focus Search Amplitude = VREF ( 32/1024 of full DAC)
	halSRV_FSM_MaxAmp_48Div1024  = 1,    // Maximum Focus Search Amplitude = VREF ( 48/1024 of full DAC)
	halSRV_FSM_MaxAmp_64Div1024  = 2,    // Maximum Focus Search Amplitude = VREF ( 64/1024 of full DAC)
	halSRV_FSM_MaxAmp_80Div1024  = 3,    // Maximum Focus Search Amplitude = VREF ( 80/1024 of full DAC)
	halSRV_FSM_MaxAmp_96Div1024  = 4,    // Maximum Focus Search Amplitude = VREF ( 96/1024 of full DAC)
	halSRV_FSM_MaxAmp_128Div1024 = 5,    // Maximum Focus Search Amplitude = VREF (128/1024 of full DAC)
	halSRV_FSM_MaxAmp_256Div1024 = 6,    // Maximum Focus Search Amplitude = VREF (256/1024 of full DAC)
	halSRV_FSM_MaxAmp_511Div1024 = 7     // Maximum Focus Search Amplitude = VREF (511/1024 of full DAC)
} halSRV_FSM_t;

typedef enum
{
	halSRV_FSL_0V     = 0,    // Stand-by Level (F1)->In Volts:0         In Digits (dec):0
	halSRV_FSL_0p009V = 1,    // Stand-by Level (F1)->In Volts:0.009     In Digits (dec):1
	halSRV_FSL_0p019V = 2,    // Stand-by Level (F1)->In Volts:0.019     In Digits (dec):2
	halSRV_FSL_0p028V = 3,    // Stand-by Level (F1)->In Volts:0.028     In Digits (dec):3
	halSRV_FSL_0p038V = 4,    // Stand-by Level (F1)->In Volts:0.038     In Digits (dec):4
	halSRV_FSL_0p047V = 5,    // Stand-by Level (F1)->In Volts:0.047     In Digits (dec):5
	halSRV_FSL_0p057V = 6,    // Stand-by Level (F1)->In Volts:0.057     In Digits (dec):6
	halSRV_FSL_0p066V = 7,    // Stand-by Level (F1)->In Volts:0.066     In Digits (dec):7
	halSRV_FSL_0p076V = 8,    // Stand-by Level (F1)->In Volts:0.076     In Digits (dec):8
	halSRV_FSL_0p085V = 9,    // Stand-by Level (F1)->In Volts:0.085     In Digits (dec):9
	halSRV_FSL_0p094V = 10,    // Stand-by Level (F1)->In Volts:0.094     In Digits (dec):10
	halSRV_FSL_0p104V = 11,    // Stand-by Level (F1)->In Volts:0.104     In Digits (dec):11
	halSRV_FSL_0p113V = 12,    // Stand-by Level (F1)->In Volts:0.113     In Digits (dec):12
	halSRV_FSL_0p123V = 13,    // Stand-by Level (F1)->In Volts:0.123     In Digits (dec):13
	halSRV_FSL_0p132V = 14,    // Stand-by Level (F1)->In Volts:0.132     In Digits (dec):14
	halSRV_FSL_0p142V = 15,    // Stand-by Level (F1)->In Volts:0.142     In Digits (dec):15
	halSRV_FSL_0p151V = 16,    // Stand-by Level (F1)->In Volts:0.151     In Digits (dec):16
	halSRV_FSL_0p161V = 17,    // Stand-by Level (F1)->In Volts:0.161     In Digits (dec):17
	halSRV_FSL_0p170V = 18,    // Stand-by Level (F1)->In Volts:0.170     In Digits (dec):18
	halSRV_FSL_0p180V = 19,    // Stand-by Level (F1)->In Volts:0.180     In Digits (dec):19
	halSRV_FSL_0p189V = 20,    // Stand-by Level (F1)->In Volts:0.189     In Digits (dec):20
	halSRV_FSL_0p198V = 21,    // Stand-by Level (F1)->In Volts:0.198     In Digits (dec):21
	halSRV_FSL_0p208V = 22,    // Stand-by Level (F1)->In Volts:0.208     In Digits (dec):22
	halSRV_FSL_0p217V = 23,    // Stand-by Level (F1)->In Volts:0.217     In Digits (dec):23
	halSRV_FSL_0p227V = 24,    // Stand-by Level (F1)->In Volts:0.227     In Digits (dec):24
	halSRV_FSL_0p236V = 25,    // Stand-by Level (F1)->In Volts:0.236     In Digits (dec):25
	halSRV_FSL_0p246V = 26,    // Stand-by Level (F1)->In Volts:0.246     In Digits (dec):26
	halSRV_FSL_0p255V = 27,    // Stand-by Level (F1)->In Volts:0.255     In Digits (dec):27
	halSRV_FSL_0p265V = 28,    // Stand-by Level (F1)->In Volts:0.265     In Digits (dec):28
	halSRV_FSL_0p274V = 29,    // Stand-by Level (F1)->In Volts:0.274     In Digits (dec):29
	halSRV_FSL_0p283V = 30,    // Stand-by Level (F1)->In Volts:0.283     In Digits (dec):30
	halSRV_FSL_0p293V = 31,    // Stand-by Level (F1)->In Volts:0.293     In Digits (dec):31
	halSRV_FSL_0p302V = 32,    // Stand-by Level (F1)->In Volts:0.302     In Digits (dec):32
	halSRV_FSL_0p312V = 33,    // Stand-by Level (F1)->In Volts:0.312     In Digits (dec):33
	halSRV_FSL_0p321V = 34,    // Stand-by Level (F1)->In Volts:0.321     In Digits (dec):34
	halSRV_FSL_0p331V = 35,    // Stand-by Level (F1)->In Volts:0.331     In Digits (dec):35
	halSRV_FSL_0p340V = 36,    // Stand-by Level (F1)->In Volts:0.340     In Digits (dec):36
	halSRV_FSL_0p350V = 37,    // Stand-by Level (F1)->In Volts:0.350     In Digits (dec):37
	halSRV_FSL_0p359V = 38,    // Stand-by Level (F1)->In Volts:0.359     In Digits (dec):38
	halSRV_FSL_0p369V = 39,    // Stand-by Level (F1)->In Volts:0.369     In Digits (dec):39
	halSRV_FSL_0p378V = 40,    // Stand-by Level (F1)->In Volts:0.378     In Digits (dec):40
	halSRV_FSL_0p387V = 41,    // Stand-by Level (F1)->In Volts:0.387     In Digits (dec):41
	halSRV_FSL_0p397V = 42,    // Stand-by Level (F1)->In Volts:0.397     In Digits (dec):42
	halSRV_FSL_0p406V = 43,    // Stand-by Level (F1)->In Volts:0.406     In Digits (dec):43
	halSRV_FSL_0p416V = 44,    // Stand-by Level (F1)->In Volts:0.416     In Digits (dec):44
	halSRV_FSL_0p425V = 45,    // Stand-by Level (F1)->In Volts:0.425     In Digits (dec):45
	halSRV_FSL_0p435V = 46,    // Stand-by Level (F1)->In Volts:0.435     In Digits (dec):46
	halSRV_FSL_0p444V = 47,    // Stand-by Level (F1)->In Volts:0.444     In Digits (dec):47
	halSRV_FSL_0p454V = 48,    // Stand-by Level (F1)->In Volts:0.454     In Digits (dec):48
	halSRV_FSL_0p463V = 49,    // Stand-by Level (F1)->In Volts:0.463     In Digits (dec):49
	halSRV_FSL_0p472V = 50,    // Stand-by Level (F1)->In Volts:0.472     In Digits (dec):50
	halSRV_FSL_0p482V = 51,    // Stand-by Level (F1)->In Volts:0.482     In Digits (dec):51
	halSRV_FSL_0p491V = 52,    // Stand-by Level (F1)->In Volts:0.491     In Digits (dec):52
	halSRV_FSL_0p501V = 53,    // Stand-by Level (F1)->In Volts:0.501     In Digits (dec):53
	halSRV_FSL_0p510V = 54,    // Stand-by Level (F1)->In Volts:0.510     In Digits (dec):54
	halSRV_FSL_0p520V = 55,    // Stand-by Level (F1)->In Volts:0.520     In Digits (dec):55
	halSRV_FSL_0p529V = 56,    // Stand-by Level (F1)->In Volts:0.529     In Digits (dec):56
	halSRV_FSL_0p539V = 57,    // Stand-by Level (F1)->In Volts:0.539     In Digits (dec):57
	halSRV_FSL_0p548V = 58,    // Stand-by Level (F1)->In Volts:0.548     In Digits (dec):58
	halSRV_FSL_0p557V = 59,    // Stand-by Level (F1)->In Volts:0.557     In Digits (dec):59
	halSRV_FSL_0p567V = 60,    // Stand-by Level (F1)->In Volts:0.567     In Digits (dec):60
	halSRV_FSL_0p576V = 61,    // Stand-by Level (F1)->In Volts:0.576     In Digits (dec):61
	halSRV_FSL_0p586V = 62,    // Stand-by Level (F1)->In Volts:0.586     In Digits (dec):62
	halSRV_FSL_0p595V = 63,    // Stand-by Level (F1)->In Volts:0.595     In Digits (dec):63
	halSRV_FSL_0p605V = 64,    // Stand-by Level (F1)->In Volts:0.605     In Digits (dec):64
	halSRV_FSL_0p614V = 65,    // Stand-by Level (F1)->In Volts:0.614     In Digits (dec):65
	halSRV_FSL_0p624V = 66,    // Stand-by Level (F1)->In Volts:0.624     In Digits (dec):66
	halSRV_FSL_0p633V = 67,    // Stand-by Level (F1)->In Volts:0.633     In Digits (dec):67
	halSRV_FSL_0p643V = 68,    // Stand-by Level (F1)->In Volts:0.643     In Digits (dec):68
	halSRV_FSL_0p652V = 69,    // Stand-by Level (F1)->In Volts:0.652     In Digits (dec):69
	halSRV_FSL_0p661V = 70,    // Stand-by Level (F1)->In Volts:0.661     In Digits (dec):70
	halSRV_FSL_0p671V = 71,    // Stand-by Level (F1)->In Volts:0.671     In Digits (dec):71
	halSRV_FSL_0p680V = 72,    // Stand-by Level (F1)->In Volts:0.680     In Digits (dec):72
	halSRV_FSL_0p690V = 73,    // Stand-by Level (F1)->In Volts:0.690     In Digits (dec):73
	halSRV_FSL_0p699V = 74,    // Stand-by Level (F1)->In Volts:0.699     In Digits (dec):74
	halSRV_FSL_0p709V = 75,    // Stand-by Level (F1)->In Volts:0.709     In Digits (dec):75
	halSRV_FSL_0p718V = 76,    // Stand-by Level (F1)->In Volts:0.718     In Digits (dec):76
	halSRV_FSL_0p728V = 77,    // Stand-by Level (F1)->In Volts:0.728     In Digits (dec):77
	halSRV_FSL_0p737V = 78,    // Stand-by Level (F1)->In Volts:0.737     In Digits (dec):78
	halSRV_FSL_0p746V = 79,    // Stand-by Level (F1)->In Volts:0.746     In Digits (dec):79
	halSRV_FSL_0p756V = 80,    // Stand-by Level (F1)->In Volts:0.756     In Digits (dec):80
	halSRV_FSL_0p765V = 81,    // Stand-by Level (F1)->In Volts:0.765     In Digits (dec):81
	halSRV_FSL_0p775V = 82,    // Stand-by Level (F1)->In Volts:0.775     In Digits (dec):82
	halSRV_FSL_0p784V = 83,    // Stand-by Level (F1)->In Volts:0.784     In Digits (dec):83
	halSRV_FSL_0p794V = 84,    // Stand-by Level (F1)->In Volts:0.794     In Digits (dec):84
	halSRV_FSL_0p803V = 85,    // Stand-by Level (F1)->In Volts:0.803     In Digits (dec):85
	halSRV_FSL_0p813V = 86,    // Stand-by Level (F1)->In Volts:0.813     In Digits (dec):86
	halSRV_FSL_0p822V = 87,    // Stand-by Level (F1)->In Volts:0.822     In Digits (dec):87
	halSRV_FSL_0p831V = 88,    // Stand-by Level (F1)->In Volts:0.831     In Digits (dec):88
	halSRV_FSL_0p841V = 89,    // Stand-by Level (F1)->In Volts:0.841     In Digits (dec):89
	halSRV_FSL_0p850V = 90,    // Stand-by Level (F1)->In Volts:0.850     In Digits (dec):90
	halSRV_FSL_0p860V = 91,    // Stand-by Level (F1)->In Volts:0.860     In Digits (dec):91
	halSRV_FSL_0p869V = 92,    // Stand-by Level (F1)->In Volts:0.869     In Digits (dec):92
	halSRV_FSL_0p879V = 93,    // Stand-by Level (F1)->In Volts:0.879     In Digits (dec):93
	halSRV_FSL_0p888V = 94,    // Stand-by Level (F1)->In Volts:0.888     In Digits (dec):94
	halSRV_FSL_0p898V = 95,    // Stand-by Level (F1)->In Volts:0.898     In Digits (dec):95
	halSRV_FSL_0p907V = 96,    // Stand-by Level (F1)->In Volts:0.907     In Digits (dec):96
	halSRV_FSL_0p917V = 97,    // Stand-by Level (F1)->In Volts:0.917     In Digits (dec):97
	halSRV_FSL_0p926V = 98,    // Stand-by Level (F1)->In Volts:0.926     In Digits (dec):98
	halSRV_FSL_0p935V = 99,    // Stand-by Level (F1)->In Volts:0.935     In Digits (dec):99
	halSRV_FSL_0p945V = 100,    // Stand-by Level (F1)->In Volts:0.945     In Digits (dec):100
	halSRV_FSL_0p954V = 101,    // Stand-by Level (F1)->In Volts:0.954     In Digits (dec):101
	halSRV_FSL_0p964V = 102,    // Stand-by Level (F1)->In Volts:0.964     In Digits (dec):102
	halSRV_FSL_0p973V = 103,    // Stand-by Level (F1)->In Volts:0.973     In Digits (dec):103
	halSRV_FSL_0p983V = 104,    // Stand-by Level (F1)->In Volts:0.983     In Digits (dec):104
	halSRV_FSL_0p992V = 105,    // Stand-by Level (F1)->In Volts:0.992     In Digits (dec):105
	halSRV_FSL_1p002V = 106,    // Stand-by Level (F1)->In Volts:1.002     In Digits (dec):106
	halSRV_FSL_1p011V = 107,    // Stand-by Level (F1)->In Volts:1.011     In Digits (dec):107
	halSRV_FSL_1p020V = 108,    // Stand-by Level (F1)->In Volts:1.020     In Digits (dec):108
	halSRV_FSL_1p030V = 109,    // Stand-by Level (F1)->In Volts:1.030     In Digits (dec):109
	halSRV_FSL_1p039V = 110,    // Stand-by Level (F1)->In Volts:1.039     In Digits (dec):110
	halSRV_FSL_1p049V = 111,    // Stand-by Level (F1)->In Volts:1.049     In Digits (dec):111
	halSRV_FSL_1p058V = 112,    // Stand-by Level (F1)->In Volts:1.058     In Digits (dec):112
	halSRV_FSL_1p068V = 113,    // Stand-by Level (F1)->In Volts:1.068     In Digits (dec):113
	halSRV_FSL_1p077V = 114,    // Stand-by Level (F1)->In Volts:1.077     In Digits (dec):114
	halSRV_FSL_1p087V = 115,    // Stand-by Level (F1)->In Volts:1.087     In Digits (dec):115
	halSRV_FSL_1p096V = 116,    // Stand-by Level (F1)->In Volts:1.096     In Digits (dec):116
	halSRV_FSL_1p106V = 117,    // Stand-by Level (F1)->In Volts:1.106     In Digits (dec):117
	halSRV_FSL_1p115V = 118,    // Stand-by Level (F1)->In Volts:1.115     In Digits (dec):118
	halSRV_FSL_1p124V = 119,    // Stand-by Level (F1)->In Volts:1.124     In Digits (dec):119
	halSRV_FSL_1p134V = 120,    // Stand-by Level (F1)->In Volts:1.134     In Digits (dec):120
	halSRV_FSL_1p143V = 121,    // Stand-by Level (F1)->In Volts:1.143     In Digits (dec):121
	halSRV_FSL_1p153V = 122,    // Stand-by Level (F1)->In Volts:1.153     In Digits (dec):122
	halSRV_FSL_1p162V = 123,    // Stand-by Level (F1)->In Volts:1.162     In Digits (dec):123
	halSRV_FSL_1p172V = 124,    // Stand-by Level (F1)->In Volts:1.172     In Digits (dec):124
	halSRV_FSL_1p181V = 125,    // Stand-by Level (F1)->In Volts:1.181     In Digits (dec):125
	halSRV_FSL_1p191V = 126,    // Stand-by Level (F1)->In Volts:1.191     In Digits (dec):126
	halSRV_FSL_1p200V = 127     // Stand-by Level (F1)->In Volts:1.200     In Digits (dec):127
} halSRV_FSL_t;

typedef enum
{
	halSRV_SHL_DetectLV_22mv  = 0,    // set shock detection level =  22 mv
	halSRV_SHL_DetectLV_33mv  = 1,    // set shock detection level =  33 mv
	halSRV_SHL_DetectLV_44mv  = 2,    // set shock detection level =  44 mv
	halSRV_SHL_DetectLV_55mv  = 3,    // set shock detection level =  55 mv
	halSRV_SHL_DetectLV_66mv  = 4,    // set shock detection level =  66 mv
	halSRV_SHL_DetectLV_88mv  = 5,    // set shock detection level =  88 mv
	halSRV_SHL_DetectLV_110mv = 6,    // set shock detection level = 110 mv
	halSRV_SHL_DetectLV_143mv = 7,    // set shock detection level = 143 mv
	halSRV_SHL_DetectLV_176mv = 8,    // set shock detection level = 176 mv
	halSRV_SHL_DetectLV_220mv = 9,    // set shock detection level = 220 mv
	halSRV_SHL_DetectLV_275mv = 10,    // set shock detection level = 275 mv
	halSRV_SHL_DetectLV_352mv = 11,    // set shock detection level = 352 mv
	halSRV_SHL_DetectLV_440mv = 12,    // set shock detection level = 440 mv
	halSRV_SHL_DetectLV_550mv = 13,    // set shock detection level = 550 mv
	halSRV_SHL_DetectLV_704mv = 14,    // set shock detection level = 704 mv
	halSRV_SHL_DetectLV_880mv = 15     // set shock detection level = 880 mv
} halSRV_SHL_t;

typedef enum
{
	halSRV_GainUp_2to3ms   = 0,    // gain-up operation time = 2 to 3 ms
	halSRV_GainUp_5to6ms   = 1,    // gain-up operation time = 5 to 6 ms
	halSRV_GainUp_11to12ms = 2,    // gain-up operation time = 11 to 12 ms
	halSRV_GainUp_23to24ms = 3     // gain-up operation time = 23 to 24 ms
} halSRV_GainUpOperation_t;

typedef enum
{
	halSRV_Hysteresis_2to3ms   = 0,    // tracking hysteresis operation time = 2 to 3 ms
	halSRV_Hysteresis_5to6ms   = 1,    // tracking hysteresis operation time = 5 to 6 ms
	halSRV_Hysteresis_11to12ms = 2,    // tracking hysteresis operation time = 11 to 12 ms
	halSRV_Hysteresis_47to48ms = 3     // tracking hysteresis operation time = 47 to 48 ms
} halSRV_HysteresisOperation_t;

typedef enum
{
	halSRV_KickWindow_0us   = 0,    // kick window width = 0 us - Default
	halSRV_KickWindow_22us  = 1,    // kick window width = 22 us
	halSRV_KickWindow_60us  = 2,    // kick window width = 60 us
	halSRV_KickWindow_110us = 3     // kick window width = 110 us
} halSRV_KickWindow_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halSRV_TrackXFreq_3p68KHZ = 0,    // track crossing reference frequency = 3.68 KHz
	halSRV_TrackXFreq_5p52KHZ = 1,    // track crossing reference frequency = 5.52 KHz
	halSRV_TrackXFreq_8p40KHZ = 2,    // track crossing reference frequency = 8.40 KHz
	halSRV_TrackXFreq_12p6KHZ = 3     // track crossing reference frequency = 12.6 KHz
} halSRV_TrackCrossingSpeedRefFreq_t;

#endif // #if (CHIP_REV <= 0xB3)

typedef enum
{
	halSRV_FGSW_InputGain256 = 0,    // Sets focus servo input gain to 256
	halSRV_FGSW_InputGain128 = 1     // Sets focus servo input gain to 128
} halSRV_FGSW_t;

typedef enum
{
	halSRV_TGSW_InputGain256 = 0,    // Sets tracking servo input gain to 256
	halSRV_TGSW_InputGain128 = 1     // Sets tracking servo input gain to 128
} halSRV_TGSW_t;

typedef enum
{
	halSRV_TXMUX_HalfTrack = 0,    // Half-track pulse
	halSRV_TXMUX_TX_Pluse  = 1     // TX pulse
} halSRV_TXMUX_t;

typedef enum
{
	halSRV_SrvMio3_SAJ_MASK = 0,    // Mask pulse
	halSRV_SrvMio3_DMONB    = 1,    // 0 = Disc motor servo On
	halSRV_SrvMio3_HYSONB   = 2,    // 0 = Hysteresis operation in tracking servo
	halSRV_SrvMio3_STK_SHCB = 3,    // 0 = Detecting a tracking shock
	halSRV_SrvMio3_SSK_TGUP = 4,    // Tracking hysteresis on
	halSRV_SrvMio3_S_DFCT   = 5     // Delayed defect
} halSRV_ServoMio3Select_t;

typedef enum
{
	halSRV_SrvMio2_TRONB          = 0,    // Indicates On/Off of tracking servo: 1 = Tracking servo On
	halSRV_SrvMio2_TRSRB          = 1,    // 0 = When tracking servo On or in tracking search
	halSRV_SrvMio2_FOKB           = 2,    // Focus servo O.K. signals: 0 = Focus O.K.
	halSRV_SrvMio2_SRCHB          = 3,    // 0 = When in tracking search
	halSRV_SrvMio2_SSK_FINES      = 4,    // Fine search status
	halSRV_SrvMio2_APC_DIFF_PHASE = 5,
	halSRV_SrvMio2_S_DEFECT2AFE   = 6     // Servo defect
} halSRV_ServoMio2Select_t;

typedef enum
{
	halSRV_SrvMio1_DFCTB        = 0,    // 0 = Detecting defects
	halSRV_SrvMio1_FOONB        = 1,    // Indicates On/Off of focus servo: 0 = focus servo On
	halSRV_SrvMio1_FMONB        = 2,    // Indicates On/Off of sled motor servo: 0 = sled motor servo On
	halSRV_SrvMio1_RFZC         = 3,    // Ripple zero cross signal of RP signal: 1 = on track
	halSRV_SrvMio1_SSK_DIR      = 4,    // Search direction
	halSRV_SrvMio1_APC_FEEDBACK = 5,    // Divided real frame sync
	halSRV_SrvMio1_SCT_TEUP     = 6     // Tracking equalizer in gainup mode
} halSRV_ServoMio1Select_t;

typedef enum
{
	halSRV_SrvMio0_TEZC          = 0,    // Zero cross signal of tracking error signal: 1 = positive polarity
	halSRV_SrvMio0_FOONB         = 1,    // Indicates On/Off of focus servo: 0 = focus servo On
	halSRV_SrvMio0_FOKB          = 2,    // Focus servo O.K. signal: 0 = focus O.K.
	halSRV_SrvMio0_RFZC          = 3,    // Ripple zero cross signal of RP signal: 1 = on track
	halSRV_SrvMio0_SSK_PULSE     = 4,    // Half-track pulse
	halSRV_SrvMio0_APC_REFERENCE = 5,    // Phase edge state
	halSRV_SrvMio0_D_LOCKB       = 6     // Sync lock
} halSRV_ServoMio0Select_t;

typedef enum
{
	halSRV_DBGBUSSELD_0  = 0,    // DBGBUSSELD = 0 ; debugbus16D = {sfr_fsrch,sfr_dljmp_q,sfr_fson,sfr_fok,fng_foo_sw,srch_standby,sfr_test[9],sfr_test[8],sfr_test[7],sfr_test[6],sfr_test[5],sfr_test[4],sfr_test[3],sfr_test[2],sfr_test[1],sfr_test[0]}
	halSRV_DBGBUSSELD_1  = 1,    // DBGBUSSELD = 1 ; debugbus16D = {src_fg_disappear,src_fg_lock,src_one_revol,src_fgcount_pulse,saj_mask,apc_feedback,apc_reference,apc_diff_phase,fg_lock,real_wblock,sbl_tx,sbl_rx,sbl_blank,sbl_newblank,sbl_tx_db,inhibit_tx}
	halSRV_DBGBUSSELD_2  = 2,    // DBGBUSSELD = 2 ; debugbus16D = {ssk_hyson,ssk_tgup,ssk_tson,ssk_trsr,ssk_srch,ssk_fines,ssk_dir,ssk_txs,ssk_rxs,ssk_pulse,ssk_roughs,ssk_ontrk,stk_shc,s_dfct,s_defect2afe,stf_slow}
	halSRV_DBGBUSSELD_3  = 3,    // DBGBUSSELD = 3 ; debugbus16D = {ssc_brake_t4,ssc_brake_t21,ssc_brake_cal,ssc_brake_t21_dly,ssc_brake_a0_dly,limit_mask,ssc_stkick_d,1'b0,1'b0,1'b0,1'b0,sct_dson,sct_teup,sct_feup,sout_on,stk_dfct}
	halSRV_DBGBUSSELD_4  = 4,    // DBGBUSSELD = 4 ; debugbus16D = {scd_fe_slewrate_en,diff_temp,time176k_enc,update_temp,new_feA_peak,feA_comp_a_qgtb,new_feA_bott,new_feB_peak,feB_comp_a_qgtb,new_feB_bott,sms_feld1_q,sms_feld2_q,feA_comp_a_qslb,feB_comp_a_qslb,feA_comp_agtb,feB_comp_agtb}
	halSRV_DBGBUSSELD_5  = 5,    // DBGBUSSELD = 5 ; debugbus16D = {srv_landgroove,adv_landgroove_d,srv_wob_hold,srv_te_hold,basic_angle,dsp_basic_angle,dsp_basic_angle_out_d,sst_phase_index[3],sst_phase_index[2],sst_phase_index[1],sst_phase_index[0],1'b0,1'b0,fake_rxedge,tx_short,fake_rx_q}
	halSRV_DBGBUSSELD_6  = 6,    // DBGBUSSELD = 6 ; debugbus16D = {time176k_q[7],time176k_q[6],time176k_q[5],time176k_q[4],time176k_q[3],time176k_q[2],time176k_q[1],time176k_q[0],ssc_error_q[7],ssc_error_q[6],ssc_error_q[5],ssc_error_q[4],ssc_error_q[3],ssc_error_q[2],ssc_error_q[1],ssc_error_q[0]}
	halSRV_DBGBUSSELD_7  = 7,    // DBGBUSSELD = 7 ; debugbus16D = {seek_start_ld,seek_end,sst_step_clk,sst_full_speed,sst_acc,sst_hold_speed,ent_fullmod,standby_entfull,stp_loc_ld,step_cnt_ld,step_cnt_up,step_cnt_dn,dco_out,1'b0,1'b0,1'b0}
	halSRV_DBGBUSSELD_8  = 8,    // DBGBUSSELD = 8 ; debugbus16D = {mux_a[15],mux_a[14],mux_a[13],mux_a[12],mux_a[11],mux_a[10],mux_a[9],mux_a[8],mux_a[7],mux_a[6],mux_a[5],mux_a[4],mux_a[3],mux_a[2],mux_a[1],mux_a[0]}
	halSRV_DBGBUSSELD_9  = 9,    // DBGBUSSELD = 9 ; debugbus16D = {mux_a[31],mux_a[30],mux_a[29],mux_a[28],mux_a[27],mux_a[26],mux_a[25],mux_a[24],mux_a[23],mux_a[22],mux_a[21],mux_a[20],mux_a[19],mux_a[18],mux_a[17],mux_a[16]}
	halSRV_DBGBUSSELD_10 = 10,    // DBGBUSSELD = 10 ; debugbus16D = {mux_a[47],mux_a[46],mux_a[45],mux_a[44],mux_a[43],mux_a[42],mux_a[41],mux_a[40],mux_a[39],mux_a[38],mux_a[37],mux_a[36],mux_a[35],mux_a[34],mux_a[33],mux_a[32]}
	halSRV_DBGBUSSELD_11 = 11,    // DBGBUSSELD = 11 ; debugbus16D = {mux_a[63],mux_a[62],mux_a[61],mux_a[60],mux_a[59],mux_a[58],mux_a[57],mux_a[56],mux_a[55],mux_a[54],mux_a[53],mux_a[52],mux_a[51],mux_a[50],mux_a[49],mux_a[48]}
	halSRV_DBGBUSSELD_12 = 12,    // DBGBUSSELD = 12 ; debugbus16D = {mux_b[15],mux_b[14],mux_b[13],mux_b[12],mux_b[11],mux_b[10],mux_b[9],mux_b[8],mux_b[7],mux_b[6],mux_b[5],mux_b[4],mux_b[3],mux_b[2],mux_b[1],mux_b[0]}
	halSRV_DBGBUSSELD_13 = 13,    // DBGBUSSELD = 13 ; debugbus16D = {mux_b[31],mux_b[30],mux_b[29],mux_b[28],mux_b[27],mux_b[26],mux_b[25],mux_b[24],mux_b[23],mux_b[22],mux_b[21],mux_b[20],mux_b[19],mux_b[18],mux_b[17],mux_b[16]}
	halSRV_DBGBUSSELD_14 = 14,    // DBGBUSSELD = 14 ; debugbus16D = {mux_b[47],mux_b[46],mux_b[45],mux_b[44],mux_b[43],mux_b[42],mux_b[41],mux_b[40],mux_b[39],mux_b[38],mux_b[37],mux_b[36],mux_b[35],mux_b[34],mux_b[33],mux_b[32]}
	halSRV_DBGBUSSELD_15 = 15     // DBGBUSSELD = 15 ; debugbus16D = {mux_b[63],mux_b[62],mux_b[61],mux_b[60],mux_b[59],mux_b[58],mux_b[57],mux_b[56],mux_b[55],mux_b[54],mux_b[53],mux_b[52],mux_b[51],mux_b[50],mux_b[49],mux_b[48]}
} halSRV_DBGBUSSELD_t;

typedef enum
{
	halSRV_DBGBUSSELC_0  = 0,    // DBGBUSSELC = 0 ; debugbus16C = {sfr_fsrch,sfr_dljmp_q,sfr_fson,sfr_fok,fng_foo_sw,srch_standby,sfr_test[9],sfr_test[8],sfr_test[7],sfr_test[6],sfr_test[5],sfr_test[4],sfr_test[3],sfr_test[2],sfr_test[1],sfr_test[0]}
	halSRV_DBGBUSSELC_1  = 1,    // DBGBUSSELC = 1 ; debugbus16C = {src_fg_disappear,src_fg_lock,src_one_revol,src_fgcount_pulse,saj_mask,apc_feedback,apc_reference,apc_diff_phase,fg_lock,real_wblock,sbl_tx,sbl_rx,sbl_blank,sbl_newblank,sbl_tx_db,inhibit_tx}
	halSRV_DBGBUSSELC_2  = 2,    // DBGBUSSELC = 2 ; debugbus16C = {ssk_hyson,ssk_tgup,ssk_tson,ssk_trsr,ssk_srch,ssk_fines,ssk_dir,ssk_txs,ssk_rxs,ssk_pulse,ssk_roughs,ssk_ontrk,stk_shc,s_dfct,s_defect2afe,stf_slow}
	halSRV_DBGBUSSELC_3  = 3,    // DBGBUSSELC = 3 ; debugbus16C = {ssc_brake_t4,ssc_brake_t21,ssc_brake_cal,ssc_brake_t21_dly,ssc_brake_a0_dly,limit_mask,ssc_stkick_d,1'b0,1'b0,1'b0,1'b0,sct_dson,sct_teup,sct_feup,sout_on,stk_dfct}
	halSRV_DBGBUSSELC_4  = 4,    // DBGBUSSELC = 4 ; debugbus16C = {scd_fe_slewrate_en,diff_temp,time176k_enc,update_temp,new_feA_peak,feA_comp_a_qgtb,new_feA_bott,new_feB_peak,feB_comp_a_qgtb,new_feB_bott,sms_feld1_q,sms_feld2_q,feA_comp_a_qslb,feB_comp_a_qslb,feA_comp_agtb,feB_comp_agtb}
	halSRV_DBGBUSSELC_5  = 5,    // DBGBUSSELC = 5 ; debugbus16C = {srv_landgroove,adv_landgroove_d,srv_wob_hold,srv_te_hold,basic_angle,dsp_basic_angle,dsp_basic_angle_out_d,sst_phase_index[3],sst_phase_index[2],sst_phase_index[1],sst_phase_index[0],1'b0,1'b0,fake_rxedge,tx_short,fake_rx_q}
	halSRV_DBGBUSSELC_6  = 6,    // DBGBUSSELC = 6 ; debugbus16C = {time176k_q[7],time176k_q[6],time176k_q[5],time176k_q[4],time176k_q[3],time176k_q[2],time176k_q[1],time176k_q[0],ssc_error_q[7],ssc_error_q[6],ssc_error_q[5],ssc_error_q[4],ssc_error_q[3],ssc_error_q[2],ssc_error_q[1],ssc_error_q[0]}
	halSRV_DBGBUSSELC_7  = 7,    // DBGBUSSELC = 7 ; debugbus16C = {seek_start_ld,seek_end,sst_step_clk,sst_full_speed,sst_acc,sst_hold_speed,ent_fullmod,standby_entfull,stp_loc_ld,step_cnt_ld,step_cnt_up,step_cnt_dn,dco_out,1'b0,1'b0,1'b0}
	halSRV_DBGBUSSELC_8  = 8,    // DBGBUSSELC = 8 ; debugbus16C = {mux_a[15],mux_a[14],mux_a[13],mux_a[12],mux_a[11],mux_a[10],mux_a[9],mux_a[8],mux_a[7],mux_a[6],mux_a[5],mux_a[4],mux_a[3],mux_a[2],mux_a[1],mux_a[0]}
	halSRV_DBGBUSSELC_9  = 9,    // DBGBUSSELC = 9 ; debugbus16C = {mux_a[31],mux_a[30],mux_a[29],mux_a[28],mux_a[27],mux_a[26],mux_a[25],mux_a[24],mux_a[23],mux_a[22],mux_a[21],mux_a[20],mux_a[19],mux_a[18],mux_a[17],mux_a[16]}
	halSRV_DBGBUSSELC_10 = 10,    // DBGBUSSELC = 10 ; debugbus16C = {mux_a[47],mux_a[46],mux_a[45],mux_a[44],mux_a[43],mux_a[42],mux_a[41],mux_a[40],mux_a[39],mux_a[38],mux_a[37],mux_a[36],mux_a[35],mux_a[34],mux_a[33],mux_a[32]}
	halSRV_DBGBUSSELC_11 = 11,    // DBGBUSSELC = 11 ; debugbus16C = {mux_a[63],mux_a[62],mux_a[61],mux_a[60],mux_a[59],mux_a[58],mux_a[57],mux_a[56],mux_a[55],mux_a[54],mux_a[53],mux_a[52],mux_a[51],mux_a[50],mux_a[49],mux_a[48]}
	halSRV_DBGBUSSELC_12 = 12,    // DBGBUSSELC = 12 ; debugbus16C = {mux_b[15],mux_b[14],mux_b[13],mux_b[12],mux_b[11],mux_b[10],mux_b[9],mux_b[8],mux_b[7],mux_b[6],mux_b[5],mux_b[4],mux_b[3],mux_b[2],mux_b[1],mux_b[0]}
	halSRV_DBGBUSSELC_13 = 13,    // DBGBUSSELC = 13 ; debugbus16C = {mux_b[31],mux_b[30],mux_b[29],mux_b[28],mux_b[27],mux_b[26],mux_b[25],mux_b[24],mux_b[23],mux_b[22],mux_b[21],mux_b[20],mux_b[19],mux_b[18],mux_b[17],mux_b[16]}
	halSRV_DBGBUSSELC_14 = 14,    // DBGBUSSELC = 14 ; debugbus16C = {mux_b[47],mux_b[46],mux_b[45],mux_b[44],mux_b[43],mux_b[42],mux_b[41],mux_b[40],mux_b[39],mux_b[38],mux_b[37],mux_b[36],mux_b[35],mux_b[34],mux_b[33],mux_b[32]}
	halSRV_DBGBUSSELC_15 = 15     // DBGBUSSELC = 15 ; debugbus16C = {mux_b[63],mux_b[62],mux_b[61],mux_b[60],mux_b[59],mux_b[58],mux_b[57],mux_b[56],mux_b[55],mux_b[54],mux_b[53],mux_b[52],mux_b[51],mux_b[50],mux_b[49],mux_b[48]}
} halSRV_DBGBUSSELC_t;

typedef enum
{
	halSRV_DBGBUSSELB_0  = 0,    // DBGBUSSELB = 0 ; debugbus16B = {sfr_fsrch,sfr_dljmp_q,sfr_fson,sfr_fok,fng_foo_sw,srch_standby,sfr_test[9],sfr_test[8],sfr_test[7],sfr_test[6],sfr_test[5],sfr_test[4],sfr_test[3],sfr_test[2],sfr_test[1],sfr_test[0]}
	halSRV_DBGBUSSELB_1  = 1,    // DBGBUSSELB = 1 ; debugbus16B = {src_fg_disappear,src_fg_lock,src_one_revol,src_fgcount_pulse,saj_mask,apc_feedback,apc_reference,apc_diff_phase,fg_lock,real_wblock,sbl_tx,sbl_rx,sbl_blank,sbl_newblank,sbl_tx_db,inhibit_tx}
	halSRV_DBGBUSSELB_2  = 2,    // DBGBUSSELB = 2 ; debugbus16B = {ssk_hyson,ssk_tgup,ssk_tson,ssk_trsr,ssk_srch,ssk_fines,ssk_dir,ssk_txs,ssk_rxs,ssk_pulse,ssk_roughs,ssk_ontrk,stk_shc,s_dfct,s_defect2afe,stf_slow}
	halSRV_DBGBUSSELB_3  = 3,    // DBGBUSSELB = 3 ; debugbus16B = {ssc_brake_t4,ssc_brake_t21,ssc_brake_cal,ssc_brake_t21_dly,ssc_brake_a0_dly,limit_mask,ssc_stkick_d,1'b0,1'b0,1'b0,1'b0,sct_dson,sct_teup,sct_feup,sout_on,stk_dfct}
	halSRV_DBGBUSSELB_4  = 4,    // DBGBUSSELB = 4 ; debugbus16B = {scd_fe_slewrate_en,diff_temp,time176k_enc,update_temp,new_feA_peak,feA_comp_a_qgtb,new_feA_bott,new_feB_peak,feB_comp_a_qgtb,new_feB_bott,sms_feld1_q,sms_feld2_q,feA_comp_a_qslb,feB_comp_a_qslb,feA_comp_agtb,feB_comp_agtb}
	halSRV_DBGBUSSELB_5  = 5,    // DBGBUSSELB = 5 ; debugbus16B = {srv_landgroove,adv_landgroove_d,srv_wob_hold,srv_te_hold,basic_angle,dsp_basic_angle,dsp_basic_angle_out_d,sst_phase_index[3],sst_phase_index[2],sst_phase_index[1],sst_phase_index[0],1'b0,1'b0,fake_rxedge,tx_short,fake_rx_q}
	halSRV_DBGBUSSELB_6  = 6,    // DBGBUSSELB = 6 ; debugbus16B = {time176k_q[7],time176k_q[6],time176k_q[5],time176k_q[4],time176k_q[3],time176k_q[2],time176k_q[1],time176k_q[0],ssc_error_q[7],ssc_error_q[6],ssc_error_q[5],ssc_error_q[4],ssc_error_q[3],ssc_error_q[2],ssc_error_q[1],ssc_error_q[0]}
	halSRV_DBGBUSSELB_7  = 7,    // DBGBUSSELB = 7 ; debugbus16B = {seek_start_ld,seek_end,sst_step_clk,sst_full_speed,sst_acc,sst_hold_speed,ent_fullmod,standby_entfull,stp_loc_ld,step_cnt_ld,step_cnt_up,step_cnt_dn,dco_out,1'b0,1'b0,1'b0}
	halSRV_DBGBUSSELB_8  = 8,    // DBGBUSSELB = 8 ; debugbus16B = {mux_a[15],mux_a[14],mux_a[13],mux_a[12],mux_a[11],mux_a[10],mux_a[9],mux_a[8],mux_a[7],mux_a[6],mux_a[5],mux_a[4],mux_a[3],mux_a[2],mux_a[1],mux_a[0]}
	halSRV_DBGBUSSELB_9  = 9,    // DBGBUSSELB = 9 ; debugbus16B = {mux_a[31],mux_a[30],mux_a[29],mux_a[28],mux_a[27],mux_a[26],mux_a[25],mux_a[24],mux_a[23],mux_a[22],mux_a[21],mux_a[20],mux_a[19],mux_a[18],mux_a[17],mux_a[16]}
	halSRV_DBGBUSSELB_10 = 10,    // DBGBUSSELB = 10 ; debugbus16B = {mux_a[47],mux_a[46],mux_a[45],mux_a[44],mux_a[43],mux_a[42],mux_a[41],mux_a[40],mux_a[39],mux_a[38],mux_a[37],mux_a[36],mux_a[35],mux_a[34],mux_a[33],mux_a[32]}
	halSRV_DBGBUSSELB_11 = 11,    // DBGBUSSELB = 11 ; debugbus16B = {mux_a[63],mux_a[62],mux_a[61],mux_a[60],mux_a[59],mux_a[58],mux_a[57],mux_a[56],mux_a[55],mux_a[54],mux_a[53],mux_a[52],mux_a[51],mux_a[50],mux_a[49],mux_a[48]}
	halSRV_DBGBUSSELB_12 = 12,    // DBGBUSSELB = 12 ; debugbus16B = {mux_b[15],mux_b[14],mux_b[13],mux_b[12],mux_b[11],mux_b[10],mux_b[9],mux_b[8],mux_b[7],mux_b[6],mux_b[5],mux_b[4],mux_b[3],mux_b[2],mux_b[1],mux_b[0]}
	halSRV_DBGBUSSELB_13 = 13,    // DBGBUSSELB = 13 ; debugbus16B = {mux_b[31],mux_b[30],mux_b[29],mux_b[28],mux_b[27],mux_b[26],mux_b[25],mux_b[24],mux_b[23],mux_b[22],mux_b[21],mux_b[20],mux_b[19],mux_b[18],mux_b[17],mux_b[16]}
	halSRV_DBGBUSSELB_14 = 14,    // DBGBUSSELB = 14 ; debugbus16B = {mux_b[47],mux_b[46],mux_b[45],mux_b[44],mux_b[43],mux_b[42],mux_b[41],mux_b[40],mux_b[39],mux_b[38],mux_b[37],mux_b[36],mux_b[35],mux_b[34],mux_b[33],mux_b[32]}
	halSRV_DBGBUSSELB_15 = 15     // DBGBUSSELB = 15 ; debugbus16B = {mux_b[63],mux_b[62],mux_b[61],mux_b[60],mux_b[59],mux_b[58],mux_b[57],mux_b[56],mux_b[55],mux_b[54],mux_b[53],mux_b[52],mux_b[51],mux_b[50],mux_b[49],mux_b[48]}
} halSRV_DBGBUSSELB_t;

typedef enum
{
	halSRV_DBGBUSSELA_0  = 0,    // DBGBUSSELA = 0 ; debugbus16A = {sfr_fsrch,sfr_dljmp_q,sfr_fson,sfr_fok,fng_foo_sw,srch_standby,sfr_test[9],sfr_test[8],sfr_test[7],sfr_test[6],sfr_test[5],sfr_test[4],sfr_test[3],sfr_test[2],sfr_test[1],sfr_test[0]}
	halSRV_DBGBUSSELA_1  = 1,    // DBGBUSSELA = 1 ; debugbus16A = {src_fg_disappear,src_fg_lock,src_one_revol,src_fgcount_pulse,saj_mask,apc_feedback,apc_reference,apc_diff_phase,fg_lock,real_wblock,sbl_tx,sbl_rx,sbl_blank,sbl_newblank,sbl_tx_db,inhibit_tx}
	halSRV_DBGBUSSELA_2  = 2,    // DBGBUSSELA = 2 ; debugbus16A = {ssk_hyson,ssk_tgup,ssk_tson,ssk_trsr,ssk_srch,ssk_fines,ssk_dir,ssk_txs,ssk_rxs,ssk_pulse,ssk_roughs,ssk_ontrk,stk_shc,s_dfct,s_defect2afe,stf_slow}
	halSRV_DBGBUSSELA_3  = 3,    // DBGBUSSELA = 3 ; debugbus16A = {ssc_brake_t4,ssc_brake_t21,ssc_brake_cal,ssc_brake_t21_dly,ssc_brake_a0_dly,limit_mask,ssc_stkick_d,1'b0,1'b0,1'b0,1'b0,sct_dson,sct_teup,sct_feup,sout_on,stk_dfct}
	halSRV_DBGBUSSELA_4  = 4,    // DBGBUSSELA = 4 ; debugbus16A = {scd_fe_slewrate_en,diff_temp,time176k_enc,update_temp,new_feA_peak,feA_comp_a_qgtb,new_feA_bott,new_feB_peak,feB_comp_a_qgtb,new_feB_bott,sms_feld1_q,sms_feld2_q,feA_comp_a_qslb,feB_comp_a_qslb,feA_comp_agtb,feB_comp_agtb}
	halSRV_DBGBUSSELA_5  = 5,    // DBGBUSSELA = 5 ; debugbus16A = {srv_landgroove,adv_landgroove_d,srv_wob_hold,srv_te_hold,basic_angle,dsp_basic_angle,dsp_basic_angle_out_d,sst_phase_index[3],sst_phase_index[2],sst_phase_index[1],sst_phase_index[0],1'b0,1'b0,fake_rxedge,tx_short,fake_rx_q}
	halSRV_DBGBUSSELA_6  = 6,    // DBGBUSSELA = 6 ; debugbus16A = {time176k_q[7],time176k_q[6],time176k_q[5],time176k_q[4],time176k_q[3],time176k_q[2],time176k_q[1],time176k_q[0],ssc_error_q[7],ssc_error_q[6],ssc_error_q[5],ssc_error_q[4],ssc_error_q[3],ssc_error_q[2],ssc_error_q[1],ssc_error_q[0]}
	halSRV_DBGBUSSELA_7  = 7,    // DBGBUSSELA = 7 ; debugbus16A = {seek_start_ld,seek_end,sst_step_clk,sst_full_speed,sst_acc,sst_hold_speed,ent_fullmod,standby_entfull,stp_loc_ld,step_cnt_ld,step_cnt_up,step_cnt_dn,dco_out,1'b0,1'b0,1'b0}
	halSRV_DBGBUSSELA_8  = 8,    // DBGBUSSELA = 8 ; debugbus16A = {mux_a[15],mux_a[14],mux_a[13],mux_a[12],mux_a[11],mux_a[10],mux_a[9],mux_a[8],mux_a[7],mux_a[6],mux_a[5],mux_a[4],mux_a[3],mux_a[2],mux_a[1],mux_a[0]}
	halSRV_DBGBUSSELA_9  = 9,    // DBGBUSSELA = 9 ; debugbus16A = {mux_a[31],mux_a[30],mux_a[29],mux_a[28],mux_a[27],mux_a[26],mux_a[25],mux_a[24],mux_a[23],mux_a[22],mux_a[21],mux_a[20],mux_a[19],mux_a[18],mux_a[17],mux_a[16]}
	halSRV_DBGBUSSELA_10 = 10,    // DBGBUSSELA = 10 ; debugbus16A = {mux_a[47],mux_a[46],mux_a[45],mux_a[44],mux_a[43],mux_a[42],mux_a[41],mux_a[40],mux_a[39],mux_a[38],mux_a[37],mux_a[36],mux_a[35],mux_a[34],mux_a[33],mux_a[32]}
	halSRV_DBGBUSSELA_11 = 11,    // DBGBUSSELA = 11 ; debugbus16A = {mux_a[63],mux_a[62],mux_a[61],mux_a[60],mux_a[59],mux_a[58],mux_a[57],mux_a[56],mux_a[55],mux_a[54],mux_a[53],mux_a[52],mux_a[51],mux_a[50],mux_a[49],mux_a[48]}
	halSRV_DBGBUSSELA_12 = 12,    // DBGBUSSELA = 12 ; debugbus16A = {mux_b[15],mux_b[14],mux_b[13],mux_b[12],mux_b[11],mux_b[10],mux_b[9],mux_b[8],mux_b[7],mux_b[6],mux_b[5],mux_b[4],mux_b[3],mux_b[2],mux_b[1],mux_b[0]}
	halSRV_DBGBUSSELA_13 = 13,    // DBGBUSSELA = 13 ; debugbus16A = {mux_b[31],mux_b[30],mux_b[29],mux_b[28],mux_b[27],mux_b[26],mux_b[25],mux_b[24],mux_b[23],mux_b[22],mux_b[21],mux_b[20],mux_b[19],mux_b[18],mux_b[17],mux_b[16]}
	halSRV_DBGBUSSELA_14 = 14,    // DBGBUSSELA = 14 ; debugbus16A = {mux_b[47],mux_b[46],mux_b[45],mux_b[44],mux_b[43],mux_b[42],mux_b[41],mux_b[40],mux_b[39],mux_b[38],mux_b[37],mux_b[36],mux_b[35],mux_b[34],mux_b[33],mux_b[32]}
	halSRV_DBGBUSSELA_15 = 15     // DBGBUSSELA = 15 ; debugbus16A = {mux_b[63],mux_b[62],mux_b[61],mux_b[60],mux_b[59],mux_b[58],mux_b[57],mux_b[56],mux_b[55],mux_b[54],mux_b[53],mux_b[52],mux_b[51],mux_b[50],mux_b[49],mux_b[48]}
} halSRV_DBGBUSSELA_t;

typedef enum
{
	halSRV_DFRQFG_SamplingRate_1p378KHz = 0,    // Sampling rate of disc equalizer = 1.378 kHz; normally used for CAV
	halSRV_DFRQFG_SamplingRate_7p35KHz  = 1     // Sampling rate of disc equalizer = 7.35  kHz; normally used for CLV
} halSRV_DFRQFG_t;

typedef enum
{
	halSRV_DFRQWBL_SamplingRate_1p378KHz = 0,    // Sampling rate of disc equalizer = 1.378 kHz; normally used for CAV
	halSRV_DFRQWBL_SamplingRate_7p35KHz  = 1     // Sampling rate of disc equalizer = 7.35  kHz; normally used for CLV
} halSRV_DFRQWBL_t;

typedef enum
{
	halSRV_DFRQDEC_SamplingRate_1p378KHz = 0,    // Sampling rate of disc equalizer = 1.378 kHz; normally used for CAV
	halSRV_DFRQDEC_SamplingRate_7p35KHz  = 1     // Sampling rate of disc equalizer = 7.35  kHz; normally used for CLV
} halSRV_DFRQDEC_t;

#if (CHIP_REV <= 0xB3)

typedef enum
{
	halSRV_AFEDFCT_Select_Servo = 0,    // Select defect signal generated by the servo
	halSRV_AFEDFCT_Select_AFE   = 1     // Select defect signal generated by the AFE
} halSRV_AFEDFCT_t;

#endif // #if (CHIP_REV <= 0xB3)

#if (CHIP_REV >= 0xC0)

typedef enum
{
	halSRV_DEEPDFCT_Select_Servo = 0,    // Select deep defect signal generated by the servo
	halSRV_DEEPDFCT_Select_DFE   = 1     // Select deep defect signal generated by the DFE
} halSRV_DEEPDFCT_t;

#endif // #if (CHIP_REV >= 0xC0)

typedef enum
{
	halSRV_BFILT_RejectFilter_44KHz = 0,    // 44kHz high frequency reject filter
	halSRV_BFILT_RejectFilter_88KHz = 1     // 88kHz high frequency reject filter for blank detector
} halSRV_BFILT_t;

typedef enum
{
	halSRV_FAKERX_Use_RealRX = 0,    // Use real RX unconditionally
	halSRV_FAKERX_Use_FakeRX = 1     // Use fake RX during a blank and real RX during a non-blank (data area)
} halSRV_FAKERX_t;

typedef enum
{
	halSRV_FOKL_0V     = 0,    // Stand-by Level (F1)->In Volts:0         In Digits (dec):0
	halSRV_FOKL_0p009V = 1,    // Stand-by Level (F1)->In Volts:0.009     In Digits (dec):1
	halSRV_FOKL_0p019V = 2,    // Stand-by Level (F1)->In Volts:0.019     In Digits (dec):2
	halSRV_FOKL_0p028V = 3,    // Stand-by Level (F1)->In Volts:0.028     In Digits (dec):3
	halSRV_FOKL_0p038V = 4,    // Stand-by Level (F1)->In Volts:0.038     In Digits (dec):4
	halSRV_FOKL_0p047V = 5,    // Stand-by Level (F1)->In Volts:0.047     In Digits (dec):5
	halSRV_FOKL_0p057V = 6,    // Stand-by Level (F1)->In Volts:0.057     In Digits (dec):6
	halSRV_FOKL_0p066V = 7,    // Stand-by Level (F1)->In Volts:0.066     In Digits (dec):7
	halSRV_FOKL_0p076V = 8,    // Stand-by Level (F1)->In Volts:0.076     In Digits (dec):8
	halSRV_FOKL_0p085V = 9,    // Stand-by Level (F1)->In Volts:0.085     In Digits (dec):9
	halSRV_FOKL_0p094V = 10,    // Stand-by Level (F1)->In Volts:0.094     In Digits (dec):10
	halSRV_FOKL_0p104V = 11,    // Stand-by Level (F1)->In Volts:0.104     In Digits (dec):11
	halSRV_FOKL_0p113V = 12,    // Stand-by Level (F1)->In Volts:0.113     In Digits (dec):12
	halSRV_FOKL_0p123V = 13,    // Stand-by Level (F1)->In Volts:0.123     In Digits (dec):13
	halSRV_FOKL_0p132V = 14,    // Stand-by Level (F1)->In Volts:0.132     In Digits (dec):14
	halSRV_FOKL_0p142V = 15,    // Stand-by Level (F1)->In Volts:0.142     In Digits (dec):15
	halSRV_FOKL_0p151V = 16,    // Stand-by Level (F1)->In Volts:0.151     In Digits (dec):16
	halSRV_FOKL_0p161V = 17,    // Stand-by Level (F1)->In Volts:0.161     In Digits (dec):17
	halSRV_FOKL_0p170V = 18,    // Stand-by Level (F1)->In Volts:0.170     In Digits (dec):18
	halSRV_FOKL_0p180V = 19,    // Stand-by Level (F1)->In Volts:0.180     In Digits (dec):19
	halSRV_FOKL_0p189V = 20,    // Stand-by Level (F1)->In Volts:0.189     In Digits (dec):20
	halSRV_FOKL_0p198V = 21,    // Stand-by Level (F1)->In Volts:0.198     In Digits (dec):21
	halSRV_FOKL_0p208V = 22,    // Stand-by Level (F1)->In Volts:0.208     In Digits (dec):22
	halSRV_FOKL_0p217V = 23,    // Stand-by Level (F1)->In Volts:0.217     In Digits (dec):23
	halSRV_FOKL_0p227V = 24,    // Stand-by Level (F1)->In Volts:0.227     In Digits (dec):24
	halSRV_FOKL_0p236V = 25,    // Stand-by Level (F1)->In Volts:0.236     In Digits (dec):25
	halSRV_FOKL_0p246V = 26,    // Stand-by Level (F1)->In Volts:0.246     In Digits (dec):26
	halSRV_FOKL_0p255V = 27,    // Stand-by Level (F1)->In Volts:0.255     In Digits (dec):27
	halSRV_FOKL_0p265V = 28,    // Stand-by Level (F1)->In Volts:0.265     In Digits (dec):28
	halSRV_FOKL_0p274V = 29,    // Stand-by Level (F1)->In Volts:0.274     In Digits (dec):29
	halSRV_FOKL_0p283V = 30,    // Stand-by Level (F1)->In Volts:0.283     In Digits (dec):30
	halSRV_FOKL_0p293V = 31,    // Stand-by Level (F1)->In Volts:0.293     In Digits (dec):31
	halSRV_FOKL_0p302V = 32,    // Stand-by Level (F1)->In Volts:0.302     In Digits (dec):32
	halSRV_FOKL_0p312V = 33,    // Stand-by Level (F1)->In Volts:0.312     In Digits (dec):33
	halSRV_FOKL_0p321V = 34,    // Stand-by Level (F1)->In Volts:0.321     In Digits (dec):34
	halSRV_FOKL_0p331V = 35,    // Stand-by Level (F1)->In Volts:0.331     In Digits (dec):35
	halSRV_FOKL_0p340V = 36,    // Stand-by Level (F1)->In Volts:0.340     In Digits (dec):36
	halSRV_FOKL_0p350V = 37,    // Stand-by Level (F1)->In Volts:0.350     In Digits (dec):37
	halSRV_FOKL_0p359V = 38,    // Stand-by Level (F1)->In Volts:0.359     In Digits (dec):38
	halSRV_FOKL_0p369V = 39,    // Stand-by Level (F1)->In Volts:0.369     In Digits (dec):39
	halSRV_FOKL_0p378V = 40,    // Stand-by Level (F1)->In Volts:0.378     In Digits (dec):40
	halSRV_FOKL_0p387V = 41,    // Stand-by Level (F1)->In Volts:0.387     In Digits (dec):41
	halSRV_FOKL_0p397V = 42,    // Stand-by Level (F1)->In Volts:0.397     In Digits (dec):42
	halSRV_FOKL_0p406V = 43,    // Stand-by Level (F1)->In Volts:0.406     In Digits (dec):43
	halSRV_FOKL_0p416V = 44,    // Stand-by Level (F1)->In Volts:0.416     In Digits (dec):44
	halSRV_FOKL_0p425V = 45,    // Stand-by Level (F1)->In Volts:0.425     In Digits (dec):45
	halSRV_FOKL_0p435V = 46,    // Stand-by Level (F1)->In Volts:0.435     In Digits (dec):46
	halSRV_FOKL_0p444V = 47,    // Stand-by Level (F1)->In Volts:0.444     In Digits (dec):47
	halSRV_FOKL_0p454V = 48,    // Stand-by Level (F1)->In Volts:0.454     In Digits (dec):48
	halSRV_FOKL_0p463V = 49,    // Stand-by Level (F1)->In Volts:0.463     In Digits (dec):49
	halSRV_FOKL_0p472V = 50,    // Stand-by Level (F1)->In Volts:0.472     In Digits (dec):50
	halSRV_FOKL_0p482V = 51,    // Stand-by Level (F1)->In Volts:0.482     In Digits (dec):51
	halSRV_FOKL_0p491V = 52,    // Stand-by Level (F1)->In Volts:0.491     In Digits (dec):52
	halSRV_FOKL_0p501V = 53,    // Stand-by Level (F1)->In Volts:0.501     In Digits (dec):53
	halSRV_FOKL_0p510V = 54,    // Stand-by Level (F1)->In Volts:0.510     In Digits (dec):54
	halSRV_FOKL_0p520V = 55,    // Stand-by Level (F1)->In Volts:0.520     In Digits (dec):55
	halSRV_FOKL_0p529V = 56,    // Stand-by Level (F1)->In Volts:0.529     In Digits (dec):56
	halSRV_FOKL_0p539V = 57,    // Stand-by Level (F1)->In Volts:0.539     In Digits (dec):57
	halSRV_FOKL_0p548V = 58,    // Stand-by Level (F1)->In Volts:0.548     In Digits (dec):58
	halSRV_FOKL_0p557V = 59,    // Stand-by Level (F1)->In Volts:0.557     In Digits (dec):59
	halSRV_FOKL_0p567V = 60,    // Stand-by Level (F1)->In Volts:0.567     In Digits (dec):60
	halSRV_FOKL_0p576V = 61,    // Stand-by Level (F1)->In Volts:0.576     In Digits (dec):61
	halSRV_FOKL_0p586V = 62,    // Stand-by Level (F1)->In Volts:0.586     In Digits (dec):62
	halSRV_FOKL_0p595V = 63,    // Stand-by Level (F1)->In Volts:0.595     In Digits (dec):63
	halSRV_FOKL_0p605V = 64,    // Stand-by Level (F1)->In Volts:0.605     In Digits (dec):64
	halSRV_FOKL_0p614V = 65,    // Stand-by Level (F1)->In Volts:0.614     In Digits (dec):65
	halSRV_FOKL_0p624V = 66,    // Stand-by Level (F1)->In Volts:0.624     In Digits (dec):66
	halSRV_FOKL_0p633V = 67,    // Stand-by Level (F1)->In Volts:0.633     In Digits (dec):67
	halSRV_FOKL_0p643V = 68,    // Stand-by Level (F1)->In Volts:0.643     In Digits (dec):68
	halSRV_FOKL_0p652V = 69,    // Stand-by Level (F1)->In Volts:0.652     In Digits (dec):69
	halSRV_FOKL_0p661V = 70,    // Stand-by Level (F1)->In Volts:0.661     In Digits (dec):70
	halSRV_FOKL_0p671V = 71,    // Stand-by Level (F1)->In Volts:0.671     In Digits (dec):71
	halSRV_FOKL_0p680V = 72,    // Stand-by Level (F1)->In Volts:0.680     In Digits (dec):72
	halSRV_FOKL_0p690V = 73,    // Stand-by Level (F1)->In Volts:0.690     In Digits (dec):73
	halSRV_FOKL_0p699V = 74,    // Stand-by Level (F1)->In Volts:0.699     In Digits (dec):74
	halSRV_FOKL_0p709V = 75,    // Stand-by Level (F1)->In Volts:0.709     In Digits (dec):75
	halSRV_FOKL_0p718V = 76,    // Stand-by Level (F1)->In Volts:0.718     In Digits (dec):76
	halSRV_FOKL_0p728V = 77,    // Stand-by Level (F1)->In Volts:0.728     In Digits (dec):77
	halSRV_FOKL_0p737V = 78,    // Stand-by Level (F1)->In Volts:0.737     In Digits (dec):78
	halSRV_FOKL_0p746V = 79,    // Stand-by Level (F1)->In Volts:0.746     In Digits (dec):79
	halSRV_FOKL_0p756V = 80,    // Stand-by Level (F1)->In Volts:0.756     In Digits (dec):80
	halSRV_FOKL_0p765V = 81,    // Stand-by Level (F1)->In Volts:0.765     In Digits (dec):81
	halSRV_FOKL_0p775V = 82,    // Stand-by Level (F1)->In Volts:0.775     In Digits (dec):82
	halSRV_FOKL_0p784V = 83,    // Stand-by Level (F1)->In Volts:0.784     In Digits (dec):83
	halSRV_FOKL_0p794V = 84,    // Stand-by Level (F1)->In Volts:0.794     In Digits (dec):84
	halSRV_FOKL_0p803V = 85,    // Stand-by Level (F1)->In Volts:0.803     In Digits (dec):85
	halSRV_FOKL_0p813V = 86,    // Stand-by Level (F1)->In Volts:0.813     In Digits (dec):86
	halSRV_FOKL_0p822V = 87,    // Stand-by Level (F1)->In Volts:0.822     In Digits (dec):87
	halSRV_FOKL_0p831V = 88,    // Stand-by Level (F1)->In Volts:0.831     In Digits (dec):88
	halSRV_FOKL_0p841V = 89,    // Stand-by Level (F1)->In Volts:0.841     In Digits (dec):89
	halSRV_FOKL_0p850V = 90,    // Stand-by Level (F1)->In Volts:0.850     In Digits (dec):90
	halSRV_FOKL_0p860V = 91,    // Stand-by Level (F1)->In Volts:0.860     In Digits (dec):91
	halSRV_FOKL_0p869V = 92,    // Stand-by Level (F1)->In Volts:0.869     In Digits (dec):92
	halSRV_FOKL_0p879V = 93,    // Stand-by Level (F1)->In Volts:0.879     In Digits (dec):93
	halSRV_FOKL_0p888V = 94,    // Stand-by Level (F1)->In Volts:0.888     In Digits (dec):94
	halSRV_FOKL_0p898V = 95,    // Stand-by Level (F1)->In Volts:0.898     In Digits (dec):95
	halSRV_FOKL_0p907V = 96,    // Stand-by Level (F1)->In Volts:0.907     In Digits (dec):96
	halSRV_FOKL_0p917V = 97,    // Stand-by Level (F1)->In Volts:0.917     In Digits (dec):97
	halSRV_FOKL_0p926V = 98,    // Stand-by Level (F1)->In Volts:0.926     In Digits (dec):98
	halSRV_FOKL_0p935V = 99,    // Stand-by Level (F1)->In Volts:0.935     In Digits (dec):99
	halSRV_FOKL_0p945V = 100,    // Stand-by Level (F1)->In Volts:0.945     In Digits (dec):100
	halSRV_FOKL_0p954V = 101,    // Stand-by Level (F1)->In Volts:0.954     In Digits (dec):101
	halSRV_FOKL_0p964V = 102,    // Stand-by Level (F1)->In Volts:0.964     In Digits (dec):102
	halSRV_FOKL_0p973V = 103,    // Stand-by Level (F1)->In Volts:0.973     In Digits (dec):103
	halSRV_FOKL_0p983V = 104,    // Stand-by Level (F1)->In Volts:0.983     In Digits (dec):104
	halSRV_FOKL_0p992V = 105,    // Stand-by Level (F1)->In Volts:0.992     In Digits (dec):105
	halSRV_FOKL_1p002V = 106,    // Stand-by Level (F1)->In Volts:1.002     In Digits (dec):106
	halSRV_FOKL_1p011V = 107,    // Stand-by Level (F1)->In Volts:1.011     In Digits (dec):107
	halSRV_FOKL_1p020V = 108,    // Stand-by Level (F1)->In Volts:1.020     In Digits (dec):108
	halSRV_FOKL_1p030V = 109,    // Stand-by Level (F1)->In Volts:1.030     In Digits (dec):109
	halSRV_FOKL_1p039V = 110,    // Stand-by Level (F1)->In Volts:1.039     In Digits (dec):110
	halSRV_FOKL_1p049V = 111,    // Stand-by Level (F1)->In Volts:1.049     In Digits (dec):111
	halSRV_FOKL_1p058V = 112,    // Stand-by Level (F1)->In Volts:1.058     In Digits (dec):112
	halSRV_FOKL_1p068V = 113,    // Stand-by Level (F1)->In Volts:1.068     In Digits (dec):113
	halSRV_FOKL_1p077V = 114,    // Stand-by Level (F1)->In Volts:1.077     In Digits (dec):114
	halSRV_FOKL_1p087V = 115,    // Stand-by Level (F1)->In Volts:1.087     In Digits (dec):115
	halSRV_FOKL_1p096V = 116,    // Stand-by Level (F1)->In Volts:1.096     In Digits (dec):116
	halSRV_FOKL_1p106V = 117,    // Stand-by Level (F1)->In Volts:1.106     In Digits (dec):117
	halSRV_FOKL_1p115V = 118,    // Stand-by Level (F1)->In Volts:1.115     In Digits (dec):118
	halSRV_FOKL_1p124V = 119,    // Stand-by Level (F1)->In Volts:1.124     In Digits (dec):119
	halSRV_FOKL_1p134V = 120,    // Stand-by Level (F1)->In Volts:1.134     In Digits (dec):120
	halSRV_FOKL_1p143V = 121,    // Stand-by Level (F1)->In Volts:1.143     In Digits (dec):121
	halSRV_FOKL_1p153V = 122,    // Stand-by Level (F1)->In Volts:1.153     In Digits (dec):122
	halSRV_FOKL_1p162V = 123,    // Stand-by Level (F1)->In Volts:1.162     In Digits (dec):123
	halSRV_FOKL_1p172V = 124,    // Stand-by Level (F1)->In Volts:1.172     In Digits (dec):124
	halSRV_FOKL_1p181V = 125,    // Stand-by Level (F1)->In Volts:1.181     In Digits (dec):125
	halSRV_FOKL_1p191V = 126,    // Stand-by Level (F1)->In Volts:1.191     In Digits (dec):126
	halSRV_FOKL_1p200V = 127     // Stand-by Level (F1)->In Volts:1.200     In Digits (dec):127
} halSRV_FOKL_t;

typedef enum
{
	halSRV_TMD_NoAction    = 0,    // No action
	halSRV_TMD_Load_OV22OT = 3     // Load OV22OT into the autobrake circuitry
} halSRV_TMD_t;

typedef enum
{
	halSRV_LDGRMUX_Groove = 0,    // Groove
	halSRV_LDGRMUX_Land   = 1     // Land
} halSRV_LDGRMUX_t;

typedef enum
{
	halSRV_PWMODE_Normal    = 0,    // Normal
	halSRV_PWMODE_LowPower1 = 1,    // Low power 1
	halSRV_PWMODE_FullSpd   = 2,    // Full speed
	halSRV_PWMODE_LowPower2 = 3     // Low power 2
} halSRV_PWMODE_t;

typedef enum
{
	halSRV_FG3XSEL_1XFG = 0,    // Select 1X FG mode
	halSRV_FG3XSEL_3XFG = 1     // Select 3X FG mode
} halSRV_FG3XSEL_t;

#endif /* __REG_SERVO_H__ */

