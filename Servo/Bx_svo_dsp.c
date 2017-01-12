/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   svo_dsp.c
*
* DESCRIPTION: Servo DSP configuration data
*
* $Revision:
* DATE:    11-22-2010
* TIME:    15:30:44
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\servo\svo_dsp.h"

const ULONG Bx_ServoDSP_code[640] =
{
   0x40000500  ,
   0x20070000  ,
   0x00000000  ,
   0x80011900  ,
   0x203546D2  ,
   0xC003D900  ,
   0x201A49D2  ,
   0xC003D900  ,
   0x300C0012  ,
   0x9834081E  ,
   0x8936095E  ,
   0x2008A004  ,
   0x81005288  ,
   0x81011E88  ,
   0x81001C08  ,
   0xC13CDC08  ,
   0xC03C5C08  ,
   0x300C0012  ,
   0x99383689  ,
   0x993A0AB6  ,
   0x983A0A09  ,
   0x983A0ACA  ,
   0x30390965  ,
   0x880C0100  ,
   0xA9080A81  ,
   0x89100300  ,
   0x88180300  ,
   0x91140382  ,
   0x991C0402  ,
   0x99200602  ,
   0x99240702  ,
   0x982C0602  ,
   0x91A80685  ,
   0x2008A004  ,
   0xD840CA05  ,
   0x200C0000  ,
   0x99301C05  ,
   0x20090555  ,
   0x8001BE33  ,
   0x90070633  ,
   0x203ECE40  ,
   0xC164FD31  ,
   0x20090555  ,
   0x80481D33  ,
   0x91003871  ,
   0xB8080687  ,
   0x80481D5D  ,
   0xB8080586  ,
   0xB8080384  ,
   0xB8080283  ,
   0x48000C00  ,
   0x80012321  ,
   0x80012121  ,
   0x812B2461  ,
   0x892A3400  ,
   0x80013E00  ,
   0x983900B1  ,
   0x203CCC40  ,
   0x882F4F31  ,
   0x88013131  ,
   0x20000000  ,
   0x81382721  ,
   0x80382760  ,
   0x983420A0  ,
   0x89351D1C  ,
   0x89012020  ,
   0x88361D1C  ,
   0x91011D9D  ,
   0x9937241D  ,
   0x9837241D  ,
   0x20007275  ,
   0x983764E4  ,
   0x300009A6  ,
   0x880C1111  ,
   0xA9082491  ,
   0x89101212  ,
   0x88141212  ,
   0x200009A6  ,
   0x91011292  ,
   0x303109A6  ,
   0x99181313  ,
   0x98201313  ,
   0x911C1393  ,
   0x2000C240  ,
   0x99302A13  ,
   0x9830EA13  ,
   0x20008275  ,
   0x992CE413  ,
   0x20090246  ,
   0x80013E33  ,
   0x90071313  ,
   0x203ECC40  ,
   0xC126FF31  ,
   0x20090246  ,
   0xC9019A13  ,
   0xC1393B13  ,
   0x81271130  ,
   0x80392630  ,
   0x80392670  ,
   0x58000800  ,
   0x20000000  ,
   0x806A2200  ,
   0x896B0E00  ,
   0x886C0E00  ,
   0x91810E8E  ,
   0x90010E0E  ,
   0x90810E63  ,
   0x40000300  ,
   0x804C1A00  ,
   0x894D0C00  ,
   0x884E0C00  ,
   0x91010D8C  ,
   0x994F0D0C  ,
   0x98510D0C  ,
   0x91500D8D  ,
   0x91500D0D  ,
   0x90500D5B  ,
   0x58000A00  ,
   0x806D2300  ,
   0x896E1414  ,
   0x996F1594  ,
   0x986F1514  ,
   0x986F15D5  ,
   0x00000000  ,
   0x00000000  ,
   0x00000000  ,
   0x48000E00  ,
   0x20320000  ,
   0xC001AF00  ,
   0xC8013F00  ,
   0x20328CFA  ,
   0x8101D700  ,
   0x20320000  ,
   0x81001700  ,
   0x900117BF  ,
   0x90011751  ,
   0x3034001B  ,
   0xC06E112F  ,
   0xC001252F  ,
   0x3834001B  ,
   0xC070912F  ,
   0xC03A912F  ,
   0x2834001B  ,
   0x926A11BC  ,
   0x900011B4  ,
   0x00000000  ,
   0x30170BDB  ,
   0x80501100  ,
   0x302F001B  ,
   0xC954BC00  ,
   0x202F0000  ,
   0x81031400  ,
   0x80011100  ,
   0x202FE656  ,
   0x900078B8  ,
   0xD05C38B8  ,
   0xD05DB8B8  ,
   0x20170000  ,
   0xCD6B3900  ,
   0xC800B900  ,
   0x89003700  ,
   0x918037B7  ,
   0x92EB00B9  ,
   0x805F1100  ,
   0x8D6B3900  ,
   0x2017E656  ,
   0x89683800  ,
   0x2017C600  ,
   0x80605100  ,
   0xC05F9100  ,
   0x20170000  ,
   0x90008000  ,
   0x900011F3  ,
   0x301D0BDB  ,
   0x81561100  ,
   0x302F001B  ,
   0xC95ABC00  ,
   0x201C0000  ,
   0xC0009600  ,
   0xC1031600  ,
   0x805D1100  ,
   0x201C0000  ,
   0xD000BABA  ,
   0xD05E3ABA  ,
   0x20190000  ,
   0xD85EB8BA  ,
   0x201D0000  ,
   0xC800BA00  ,
   0xC9013B00  ,
   0x88693700  ,
   0x986937BB  ,
   0x906A3900  ,
   0x89013B00  ,
   0x816A1100  ,
   0x201E0000  ,
   0xC0619100  ,
   0x88013300  ,
   0x3036001B  ,
   0xD0623FBD  ,
   0xD062BF3D  ,
   0xD062BFBD  ,
   0x814A1100  ,
   0x88643D3D  ,
   0x814C1174  ,
   0x88663D00  ,
   0x814E1175  ,
   0x80011100  ,
   0x80011176  ,
   0x50000E00  ,
   0x20000000  ,
   0x805A1100  ,
   0x81001100  ,
   0x34000000  ,
   0x8034116C  ,
   0x8034117C  ,
   0x203E0000  ,
   0xC058913E  ,
   0xC059113E  ,
   0x897CA63E  ,
   0x8001913E  ,
   0x2027CF04  ,
   0x2127CF04  ,
   0x8259901E  ,
   0x805AD01E  ,
   0x8259FC1E  ,
   0x805AD13C  ,
   0xC107D006  ,
   0xC1075107  ,
   0x8100D107  ,
   0x825AD16C  ,
   0x805AD147  ,
   0x48000B00  ,
   0x2000E555  ,
   0x2100E555  ,
   0x882E0F0F  ,
   0x892D310F  ,
   0x80013E0F  ,
   0x902F0F8F  ,
   0x40000400  ,
   0x884A0B00  ,
   0x81491A00  ,
   0x80491A00  ,
   0x904B0B8B  ,
   0x904B0B0B  ,
   0x904B0B5A  ,
   0x58001200  ,
   0x200FE3DA  ,
   0xC0705B00  ,
   0xC070E000  ,
   0x89715E00  ,
   0x88725E00  ,
   0x91015E9E  ,
   0x99735B1E  ,
   0x99735B1E  ,
   0x90745B9B  ,
   0x90745B1B  ,
   0x91745B65  ,
   0x40000100  ,
   0x200EE2CC  ,
   0x8807CA00  ,
   0x8807CA00  ,
   0x58000100  ,
   0xD000CA00  ,
   0xD0784A00  ,
   0x89795600  ,
   0x89015600  ,
   0x90014096  ,
   0x200DE2CC  ,
   0xD07AC000  ,
   0xC97DD800  ,
   0xC87CD800  ,
   0xD17BC098  ,
   0xC900D600  ,
   0xD07EC000  ,
   0xC900CA00  ,
   0xD100D697  ,
   0x200D22CC  ,
   0xC800CA72  ,
   0x200D82CC  ,
   0xC8015700  ,
   0xC17F6D00  ,
   0xC8004A00  ,
   0xC8004A72  ,
   0x48000F00  ,
   0x200D12CC  ,
   0x8800CA96  ,
   0x8800CA97  ,
   0x8800CA98  ,
   0x8800CA72  ,
   0x58000100  ,
   0x200D42CC  ,
   0xC07F6D00  ,
   0xC07F6D00  ,
   0xC07F6D72  ,
   0x50000000  ,
   0x30100451  ,
   0xC0001200  ,
   0xC063A800  ,
   0xC160A900  ,
   0xC060A900  ,
   0xD072A9B3  ,
   0xC96F9F00  ,
   0xC866B300  ,
   0xD969999F  ,
   0xC869991F  ,
   0x20130451  ,
   0xC001AB1F  ,
   0x20120451  ,
   0xC076911F  ,
   0x20140451  ,
   0xD0019999  ,
   0xC9019F19  ,
   0x30100451  ,
   0xC0009237  ,
   0xD06C9237  ,
   0xC0009237  ,
   0xC0009277  ,
   0x40000E00  ,
   0x240C0000  ,
   0xC05F1100  ,
   0xC0019F00  ,
   0x80011F00  ,
   0x80011F51  ,
   0x38000000  ,
   0x80542C00  ,
   0x80541100  ,
   0x20000000  ,
   0x82E2115F  ,
   0x48000E00  ,
   0x98331010  ,
   0x98321010  ,
   0x81312262  ,
   0x81312222  ,
   0x90311090  ,
   0x80012100  ,
   0x81251000  ,
   0x8225123D  ,
   0x203C0000  ,
   0xD0013D3D  ,
   0xD003923D  ,
   0x8128103D  ,
   0x8029123E  ,
   0x203C8273  ,
   0xD0297D3F  ,
   0x20000000  ,
   0x8139263F  ,
   0x90393D3F  ,
   0x90033D9A  ,
   0x50000E00  ,
   0x24220F00  ,
   0x88012500  ,
   0x81781100  ,
   0x90782525  ,
   0x907825A5  ,
   0xC87AA52A  ,
   0xC0009127  ,
   0xD2FFABA7  ,
   0xD00091A5  ,
   0xC87EA727  ,
   0xC1779125  ,
   0xC0009127  ,
   0x243CC880  ,
   0xC0075125  ,
   0xC007D025  ,
   0x817AD12A  ,
   0x807AD22A  ,
   0x927AE626  ,
   0x00000000  ,
   0x847AD22A  ,
   0x8801E62A  ,
   0x907AE6A6  ,
   0x00000000  ,
   0x880EE626  ,
   0x880FE600  ,
   0x9204ED2D  ,
   0x58000E00  ,
   0x9204ED2D  ,
   0x50000E00  ,
   0x242208C0  ,
   0x88032100  ,
   0x89060A00  ,
   0x88010A00  ,
   0x90042500  ,
   0x89012100  ,
   0x99002120  ,
   0x980109A1  ,
   0x2423C880  ,
   0x8009512A  ,
   0x88016834  ,
   0x920A6327  ,
   0x92096327  ,
   0x80005100  ,
   0x80005100  ,
   0x80085150  ,
   0x3423C880  ,
   0x80105100  ,
   0x2423C880  ,
   0x80085100  ,
   0x900751A8  ,
   0x80075150  ,
   0x3423C880  ,
   0x80105134  ,
   0x80105150  ,
   0x81105134  ,
   0x2423C880  ,
   0x8109512A  ,
   0x810A5134  ,
   0x80005134  ,
   0x900C6327  ,
   0x890D6834  ,
   0x80005127  ,
   0x980161A7  ,
   0x89036250  ,
   0x89016721  ,
   0x980167A7  ,
   0x3423889D  ,
   0xC2105027  ,
   0x24230E40  ,
   0x880B2727  ,
   0x89012227  ,
   0x2423CE40  ,
   0xC800A227  ,
   0x2423C880  ,
   0x80055127  ,
   0x980362A2  ,
   0x88036250  ,
   0x34220000  ,
   0xC0109134  ,
   0xC0109134  ,
   0xD810A8A3  ,
   0x24240880  ,
   0x88031C00  ,
   0xC9002300  ,
   0xC901A300  ,
   0x88012300  ,
   0x90042500  ,
   0x89011C00  ,
   0x99001C20  ,
   0x9801099C  ,
   0x40000E00  ,
   0x20000000  ,
   0x80052C00  ,
   0x80051100  ,
   0x82E21152  ,
   0x222B0000  ,
   0xC8440000  ,
   0xC0448000  ,
   0x226C0000  ,
   0xC8450000  ,
   0xC0458000  ,
   0x20000000  ,
   0x82782C68  ,
   0x82791169  ,
   0x80062C28  ,
   0x80061129  ,
   0x82E21152  ,
   0x222B0000  ,
   0xC8010000  ,
   0xC8600000  ,
   0xC0018000  ,
   0xC0608000  ,
   0x20000000  ,
   0x8A7A002A  ,
   0x8AE2006A  ,
   0x226C0000  ,
   0xC8010000  ,
   0xC8610000  ,
   0xC0018000  ,
   0xC0618000  ,
   0x20000000  ,
   0x8A7B002B  ,
   0x8AE2006B  ,
   0x58000E00  ,
   0x24310000  ,
   0x8001BE3E  ,
   0x80659105  ,
   0x203CCE40  ,
   0x213CCE40  ,
   0xC1669B0F  ,
   0xC0669B0F  ,
   0xC265AF7D  ,
   0x24310000  ,
   0x8001BE3E  ,
   0x80619105  ,
   0x203CCC40  ,
   0x213CCC40  ,
   0xC162A10F  ,
   0xC062A10F  ,
   0xC261AF7F  ,
   0x80631105  ,
   0x80017E05  ,
   0x24000000  ,
   0x8101AF05  ,
   0x80619105  ,
   0x203E0000  ,
   0x213E0000  ,
   0xC061117E  ,
   0xC060BF7E  ,
   0x9001BE3E  ,
   0x9261BE7F  ,
   0xC065113E  ,
   0xC064BD3E  ,
   0x9001BE3E  ,
   0x9265BE7D  ,
   0x48000E00  ,
   0x200FE3DA  ,
   0x80015500  ,
   0x80015500  ,
   0xC207D500  ,
   0x40000E00  ,
   0xC2075500  ,
   0x00000000  ,
   0x48000E00  ,
   0x24090000  ,
   0x80043B05  ,
   0x80043B05  ,
   0x203CCF80  ,
   0xC5009705  ,
   0xC5062F3B  ,
   0x20004275  ,
   0x80046605  ,
   0x20008275  ,
   0x8000E605  ,
   0x24090000  ,
   0x806D3B3B  ,
   0x82ED3B7B  ,
   0x58000E00  ,
   0x24220800  ,
   0x88032B00  ,
   0x89062400  ,
   0x88012400  ,
   0x90042500  ,
   0x89012B00  ,
   0x99002B20  ,
   0x980109AB  ,
   0x2420C880  ,
   0x8009512A  ,
   0x88016934  ,
   0x920A6327  ,
   0x92096327  ,
   0x80005100  ,
   0x80005100  ,
   0x80085150  ,
   0x3420C880  ,
   0x80105100  ,
   0x2420C880  ,
   0x80085100  ,
   0x900751A9  ,
   0x80075150  ,
   0x3420C880  ,
   0x80105134  ,
   0x80105150  ,
   0x81105134  ,
   0x2420C880  ,
   0x8109512A  ,
   0x810A5134  ,
   0x80005134  ,
   0x900C6327  ,
   0x890D6934  ,
   0x80005127  ,
   0x98016BA7  ,
   0x89036C50  ,
   0x89016721  ,
   0x980167A7  ,
   0x3420889E  ,
   0xC2105027  ,
   0x2420C880  ,
   0x880B6727  ,
   0x89016C27  ,
   0x80055127  ,
   0x98036CAC  ,
   0x88036C50  ,
   0x34220000  ,
   0xC0109134  ,
   0xC0109134  ,
   0xD810A9AD  ,
   0x24210880  ,
   0x88032A00  ,
   0xC9002D00  ,
   0xC901AD00  ,
   0x88012D00  ,
   0x90042500  ,
   0x89012A00  ,
   0x99002A20  ,
   0x980109AA  ,
   0x40000E00  ,
   0x24310000  ,
   0x8001A23E  ,
   0x813E903E  ,
   0x803D913E  ,
   0x203C0000  ,
   0x213C0000  ,
   0x8801B23E  ,
   0xD00317B2  ,
   0xC102AF32  ,
   0xC1009732  ,
   0xD10097B2  ,
   0x203ECF40  ,
   0xD003D746  ,
   0x40000E00  ,
   0x203F0000  ,
   0x80653000  ,
   0x80653100  ,
   0x826B3000  ,
   0x826C3000  ,
   0x806B3000  ,
   0x806C3100  ,
   0x92FC17B5  ,
   0x92FC17AF  ,
   0x80001200  ,
   0x80001200  ,
   0xC2689230  ,
   0xC267922E  ,
   0x00000000  ,
   0x00000000  ,
   0x80652F00  ,
   0x896A3526  ,
   0x8D683000  ,
   0x80001200  ,
   0x203C0000  ,
   0x92E830B0  ,
   0xD2E8B0B0  ,
   0x80661000  ,
   0x8D683000  ,
   0x88003000  ,
   0x203C0000  ,
   0xC8002E43  ,
   0x80652F00  ,
   0x89692F26  ,
   0x8D672E00  ,
   0x80001200  ,
   0x203C0000  ,
   0x92E72EAE  ,
   0xD2E7AEAE  ,
   0x80661000  ,
   0x8D672E00  ,
   0x88002E00  ,
   0x203C0000  ,
   0xC8002E42  ,
   0x00000000 
};


const USHORT Bx_ServoDSP_coef[512] =
{
   0x0000  ,   // K0_a 
   0x7FFF  ,   // K1_a 
   0x7FFF  ,   // K1_aa 
   0x8000  ,   // K_1_a 
   0x0000  ,   // cadr_acu 
   0x9010  ,   // radr_acu1 
   0x8A0A  ,   // radr_acu2 
   0x4000  ,   // ktg 
   0x2AC1  ,   // kt11 
   0x2AC1  ,   // ktu11 
   0x2AC1  ,   // ktw11 
   0x2AC1  ,   // ktwuxc11 
   0xD6E4  ,   // kt12 
   0xD6E4  ,   // ktu12 
   0xD6E4  ,   // ktw12 
   0xD6E4  ,   // ktwuxc12 
   0x620C  ,   // kt13 
   0x620C  ,   // ktu13 
   0x620C  ,   // ktw13 
   0x620C  ,   // ktwuxc13 
   0x68F6  ,   // kt14 
   0x68F6  ,   // ktu14 
   0x68F6  ,   // ktw14 
   0x68F6  ,   // ktwuxc14 
   0x9EED  ,   // kt15 
   0x9EED  ,   // ktu15 
   0x9EED  ,   // ktw15 
   0x9EED  ,   // ktwuxc15 
   0x0000  ,   // kt16 
   0x0000  ,   // ktu16 
   0x0000  ,   // ktw16 
   0x0000  ,   // ktwuxc16 
   0x347B  ,   // kt17 
   0x347B  ,   // ktu17 
   0x347B  ,   // ktw17 
   0x347B  ,   // ktwuxc17 
   0x0000  ,   // kt18 
   0x0000  ,   // ktu18 
   0x0000  ,   // ktw18 
   0x0000  ,   // ktwuxc18 
   0x070A  ,   // kt19 
   0x070A  ,   // ktu19 
   0x070A  ,   // ktw19 
   0x070A  ,   // ktwuxc19 
   0x070A  ,   // kt1A 
   0x070A  ,   // ktu1A 
   0x070A  ,   // ktw1A 
   0x070A  ,   // ktwuxc1A 
   0x1000  ,   // src_trk_prescale 
   0x0001  ,   // vx_0001 
   0x1000  ,   // v_1000 
   0x0800  ,   // v_800 
   0x80A4  ,   // kt1c 
   0x80A4  ,   // ktc1c 
   0x7F83  ,   // kt1d 
   0x7F83  ,   // ktc1d 
   0x8534  ,   // kt1e 
   0x8534  ,   // ktc1e 
   0x7DB2  ,   // kt1f 
   0x7DB2  ,   // ktc1f 
   0x7FFF  ,   // kt1B 
   0x002C  ,   // TNG_preset 
   0x0018  ,   // DSP_FOKL 
   0x0005  ,   // v_limit2 
   0x0087  ,   // src_kt2 
   0x7FFF  ,   // kc20 
   0x0000  ,   // kc21 
   0x7FFF  ,   // kc22 
   0x7FFF  ,   // debug0_gain 
   0x7FFF  ,   // debug1_gain 
   0x0004  ,   // v_0004 
   0x0005  ,   // v_0005 
   0x7FFF  ,   // kt10 
   0x7FFF  ,   // kt20 
   0x0000  ,   // kt21 
   0x7FFF  ,   // kt22 
   0x6800  ,   // kt30 
   0x0100  ,   // kt31 
   0x8000  ,   // kt32 
   0x7f20  ,   // kt33 
   0x010a  ,   // kt34 
   0x010a  ,   // kt35 
   0x0000  ,   // ktb10 
   0x0000  ,   // krb10 
   0x0000  ,   // kspd_0 
   0x0000  ,   // kspd_4 
   0x0000  ,   // kspd_8 
   0x0000  ,   // kspd_16 
   0x0000  ,   // kspd_32 
   0x0000  ,   // kspd_64 
   0x0000  ,   // kspd_128 
   0x0000  ,   // kspd_256 
   0x0000  ,   // kspd_512 
   0x0000  ,   // kspd_1024 
   0x0000  ,   // kspd_start 
   0x000A  ,   // kspd_addr_start 
   0x0008  ,   // debug2_gain 
   0x0008  ,   // debug3_gain 
   0x0000  ,   // temp_debug 
   0x0000  ,   // lstap 
   0x0000  ,   // TE_learndfct_gain 
   0x7FFF  ,   // actdiss_timescale 
   0x26C9  ,   // actdiss_tau 
   0x0000  ,   // trk_actdiss_acc_msb 
   0x0000  ,   // foc_actdiss_acc_msb 
   0x0000  ,   // trk_pwr_limit_inv 
   0x0000  ,   // foc_pwr_limit_inv 
   0x0000  ,   // FOO 
   0x0000  ,   // TRO 
   0x0000  ,   // kspdlim_0 
   0x0000  ,   // kspdlim_4 
   0x0000  ,   // kspdlim_8 
   0x0000  ,   // kspdlim_16 
   0x0000  ,   // kspdlim_32 
   0x0000  ,   // kspdlim_64 
   0x0000  ,   // kspdlim_128 
   0x0000  ,   // kspdlim_256 
   0x0000  ,   // kspdlim_512 
   0x0000  ,   // kspdlim_1024 
   0x0000  ,   // kspdlim_start 
   0x0000  ,   // debug0 
   0x0000  ,   // debug1 
   0x0000  ,   // debug2 
   0x0000  ,   // debug3 
   0x0000  ,   // pwr_dummy 
   0x0008  ,   // v_limit3 
   0x4000  ,   // APC_hyst 
   0x0050  ,   // v_limit1 
   0x0000  ,   // K0_b 
   0x7FFF  ,   // K1_b 
   0x7FFF  ,   // K1_bb 
   0x8000  ,   // K_1_b 
   0x0008  ,   // val_0008 
   0x0001  ,   // val_0001 
   0xFFFF  ,   // val_FFFF 
   0x4000  ,   // kfg 
   0x4000  ,   // kf11 
   0x4000  ,   // kfu11 
   0x4000  ,   // kfw11 
   0x4000  ,   // kfwux11 
   0xc074  ,   // kf12 
   0xc074  ,   // kfu12 
   0xc074  ,   // kfw12 
   0xc074  ,   // kfwux12 
   0x7FC6  ,   // kf13 
   0x7FC6  ,   // kfu13 
   0x7FC6  ,   // kfw13 
   0x7FC6  ,   // kfwux13 
   0x824A  ,   // kf14 
   0x824A  ,   // kfu14 
   0x824A  ,   // kfw14 
   0x824A  ,   // kfwux14 
   0x675F  ,   // kf15 
   0x675F  ,   // kfu15 
   0x675F  ,   // kfw15 
   0x675F  ,   // kfwux15 
   0x1000  ,   // kf16 
   0x1000  ,   // kfu16 
   0x1000  ,   // kfw16 
   0x1000  ,   // kfwux16 
   0x1000  ,   // kf17 
   0x1000  ,   // kfu17 
   0x1000  ,   // kfw17 
   0x1000  ,   // kfwux17 
   0x1000  ,   // b_1000 
   0x0000  ,   // FE_prev2 
   0x0000  ,   // FE_learndfct_gain 
   0x0000  ,   // fooffset2 
   0x0800  ,   // fcs_vel_thresh 
   0x0200  ,   // fcs_vel_gain 
   0x0000  ,   // kfb20 
   0x0000  ,   // kfb21 
   0x05F2  ,   // src_kf2 
   0x7FFF  ,   // kf20 
   0x0000  ,   // kf21 
   0x7FFF  ,   // kf22 
   0x0000  ,   // src_foc_prescale 
   0x0800  ,   // kb10 
   0x7fd1  ,   // kb11 
   0x2f00  ,   // kb12 
   0x8534  ,   // kf1c 
   0x7DB2  ,   // kf1d 
   0x80A4  ,   // kf1e 
   0x7F83  ,   // kf1f 
   0x0000  ,   // kf10 
   0x7FFF  ,   // kf18 
   0x0000  ,   // febc_0 
   0x0000  ,   // febc_1 
   0x0000  ,   // febc_2 
   0x0000  ,   // febc_3 
   0x0000  ,   // febc_4 
   0x0000  ,   // febc_5 
   0x0000  ,   // febc_6 
   0x0000  ,   // febc_7 
   0x0000  ,   // febc_8 
   0x0000  ,   // febc_9 
   0x0000  ,   // febc_10 
   0x0000  ,   // febc_11 
   0x0000  ,   // fgcmp_div6 
   0x0010  ,   // APC_gain 
   0x0000  ,   // FOODAC_PWM 
   0x0000  ,   // febc_15 
   0x0000  ,   // Offset0_0 
   0x0000  ,   // Offset0_1 
   0x0000  ,   // Offset1_0 
   0x0000  ,   // Offset1_1 
   0x0000  ,   // Offset2_0 
   0x0000  ,   // Offset2_1 
   0x0000  ,   // Pread0RS 
   0x0000  ,   // Pread1RS 
   0x0000  ,   // Pbias0RS 
   0x0000  ,   // Pbias1RS 
   0x0000  ,   // Pbias0pRS 
   0x0000  ,   // Pbias1pRS 
   0x0000  ,   // PRead0WS 
   0x0000  ,   // PRead1WS 
   0x0000  ,   // Pbias0WS 
   0x0000  ,   // Pbias1WS 
   0x0000  ,   // Pbias0pWS 
   0x0000  ,   // Pbias1pWS 
   0x0000  ,   // Gthr_read 
   0x0000  ,   // Gthr_write 
   0x0000  ,   // Gdelta_write 
   0x0000  ,   // Thresh_reference 
   0x0000  ,   // Thresh_open_loop_value 
   0x0000  ,   // Delta_open_loop_value 
   0x0000  ,   // Att0_0 
   0x0000  ,   // Att0_1 
   0x0000  ,   // Att1_0 
   0x0000  ,   // Att1_1 
   0x0000  ,   // Att2_0 
   0x0000  ,   // Att2_1 
   0x0000  ,   // thresh_P_gain 
   0x0000  ,   // T2D 
   0x0000  ,   // power_in 
   0x0000  ,   // Thresh_error_msb 
   0x0000  ,   // Delta_error_msb 
   0x0000  ,   // temp_fcs2 
   0x0000  ,   // power_ref0 
   0x0000  ,   // power_ref1 
   0x0000  ,   // power_0 
   0x0000  ,   // power_1 
   0x0000  ,   // power_2 
   0x0000  ,   // power_3 
   0x0000  ,   // power_4 
   0x0000  ,   // power_5 
   0x0000  ,   // power_6 
   0x0000  ,   // power_7 
   0x0000  ,   // power_8 
   0x0000  ,   // power_9 
   0x0000  ,   // power_10 
   0x0000  ,   // power_11 
   0x0000  ,   // power_12 
   0x0000  ,   // power_13 
   0x0000  ,   // power_14 
   0x0000  ,   // power_15 
   0x0000  ,   // K0_c 
   0x7FFF  ,   // K1_c 
   0x7FFF  ,   // K1_cc 
   0x8000  ,   // K_1c 
   0x0A8C  ,   // src_trk_alfa 
   0x0002  ,   // src_trk_tap 
   0x043C  ,   // src_kt 
   0x0001  ,   // src_trk_tap1 
   0x0002  ,   // src_trk_tap2 
   0x0000  ,   // trk_roc_m1 
   0x0000  ,   // trk_roc_m2 
   0x038E  ,   // src_trk_fg 
   0x1400  ,   // src_trk_fir1 
   0x2C00  ,   // src_trk_fir2 
   0x1200  ,   // src_trk_balance 
   0x1200  ,   // src_foc_balance 
   0x0000  ,   // trk_roc_0 
   0x0000  ,   // trk_roc_1 
   0x0000  ,   // trk_roc_2 
   0x0000  ,   // trk_roc_3 
   0x0000  ,   // trk_roc_4 
   0x0000  ,   // trk_roc_5 
   0x0000  ,   // trk_roc_6 
   0x0000  ,   // trk_roc_7 
   0x0000  ,   // trk_roc_8 
   0x0000  ,   // trk_roc_9 
   0x0000  ,   // trk_roc_10 
   0x0000  ,   // trk_roc_11 
   0x0000  ,   // trk_roc_12 
   0x0000  ,   // trk_roc_13 
   0x0000  ,   // trk_roc_14 
   0x0000  ,   // trk_roc_15 
   0x0000  ,   // trk_roc_16 
   0x0000  ,   // trk_roc_17 
   0x0000  ,   // trk_roc_18 
   0x0000  ,   // trk_roc_19 
   0x0000  ,   // trk_roc_20 
   0x0000  ,   // trk_roc_21 
   0x0000  ,   // trk_roc_22 
   0x0000  ,   // trk_roc_23 
   0x0000  ,   // trk_roc_24 
   0x0000  ,   // trk_roc_25 
   0x0000  ,   // trk_roc_26 
   0x0000  ,   // trk_roc_27 
   0x0000  ,   // trk_roc_28 
   0x0000  ,   // trk_roc_29 
   0x0000  ,   // trk_roc_30 
   0x0000  ,   // trk_roc_31 
   0x0000  ,   // trk_roc_32 
   0x0000  ,   // trk_roc_33 
   0x0000  ,   // trk_roc_34 
   0x0000  ,   // trk_roc_35 
   0x0000  ,   // te_roc_0 
   0x0000  ,   // te_roc_1 
   0x0000  ,   // te_roc_2 
   0x0000  ,   // te_roc_3 
   0x0000  ,   // te_roc_4 
   0x0000  ,   // te_roc_5 
   0x0000  ,   // te_roc_6 
   0x0000  ,   // te_roc_7 
   0x0000  ,   // te_roc_8 
   0x0000  ,   // te_roc_9 
   0x0000  ,   // te_roc_10 
   0x0000  ,   // te_roc_11 
   0x0000  ,   // te_roc_12 
   0x0000  ,   // te_roc_13 
   0x0000  ,   // te_roc_14 
   0x0000  ,   // te_roc_15 
   0x0000  ,   // te_roc_16 
   0x0000  ,   // te_roc_17 
   0x0000  ,   // te_roc_18 
   0x0000  ,   // te_roc_19 
   0x0000  ,   // te_roc_20 
   0x0000  ,   // te_roc_21 
   0x0000  ,   // te_roc_22 
   0x0000  ,   // te_roc_23 
   0x0000  ,   // te_roc_24 
   0x0000  ,   // te_roc_25 
   0x0000  ,   // te_roc_26 
   0x0000  ,   // te_roc_27 
   0x0000  ,   // te_roc_28 
   0x0000  ,   // te_roc_29 
   0x0000  ,   // te_roc_30 
   0x0000  ,   // te_roc_31 
   0x0000  ,   // te_roc_32 
   0x0000  ,   // te_roc_33 
   0x0000  ,   // te_roc_34 
   0x0000  ,   // te_roc_35 
   0x0000  ,   // src_x_start 
   0x0000  ,   // src_x_actual 
   0x0000  ,   // dsp_landgroove 
   0x0000  ,   // trk_roc_75 
   0x0000  ,   // trk_roc_76 
   0x0000  ,   // trk_roc_77 
   0x0000  ,   // trk_roc_78 
   0x0000  ,   // trk_roc_79 
   0x7000  ,   // kdf10 
   0x0000  ,   // kdd10 
   0x0000  ,   // kdw10 
   0x0000  ,   // kdf11 
   0x0000  ,   // kdd11 
   0x0000  ,   // kdw11 
   0x7fff  ,   // kdf12 
   0x0000  ,   // kdd12 
   0x0000  ,   // kdw12 
   0x7E80  ,   // kdf13 
   0x0000  ,   // kdd13 
   0x0000  ,   // kdw13 
   0x0080  ,   // kdf14 
   0x0000  ,   // kdd14 
   0x0000  ,   // kdw14 
   0x7FFF  ,   // kdf15 
   0x0000  ,   // kdd15 
   0x0000  ,   // kdw15 
   0x0600  ,   // kdf16 
   0x0000  ,   // kdd16 
   0x0000  ,   // kdw16 
   0x0000  ,   // kdf17 
   0x07F8  ,   // dmfbk_level 
   0x1000  ,   // a4096 
   0x0008  ,   // src_tap8 
   0x0000  ,   // src_ipol_step 
   0x0140  ,   // frot8x 
   0x0000  ,   // src_ipol_fact 
   0xFC00  ,   // src_frot_scale 
   0x0000  ,   // DE4_msb 
   0xCA80  ,   // src_x 
   0x0000  ,   // dummy_x 
   0x0000  ,   // K0_d 
   0x7FFF  ,   // K1_d 
   0x7FFF  ,   // K1_dd 
   0x8000  ,   // K_1_d 
   0x0708  ,   // src_foc_alfa 
   0x0002  ,   // src_foc_tap 
   0x2F96  ,   // src_kf 
   0x0001  ,   // src_foc_tap1 
   0x0002  ,   // src_foc_tap2 
   0x0000  ,   // foc_roc_m1 
   0x0000  ,   // foc_roc_m2 
   0x038E  ,   // src_foc_fg 
   0x1400  ,   // src_foc_fir1 
   0x2C00  ,   // src_foc_fir2 
   0x0000  ,   // reserved_638 
   0x0001  ,   // v_0001 
   0x0000  ,   // foc_roc_0 
   0x0000  ,   // foc_roc_1 
   0x0000  ,   // foc_roc_2 
   0x0000  ,   // foc_roc_3 
   0x0000  ,   // foc_roc_4 
   0x0000  ,   // foc_roc_5 
   0x0000  ,   // foc_roc_6 
   0x0000  ,   // foc_roc_7 
   0x0000  ,   // foc_roc_8 
   0x0000  ,   // foc_roc_9 
   0x0000  ,   // foc_roc_10 
   0x0000  ,   // foc_roc_11 
   0x0000  ,   // foc_roc_12 
   0x0000  ,   // foc_roc_13 
   0x0000  ,   // foc_roc_14 
   0x0000  ,   // foc_roc_15 
   0x0000  ,   // foc_roc_16 
   0x0000  ,   // foc_roc_17 
   0x0000  ,   // foc_roc_18 
   0x0000  ,   // foc_roc_19 
   0x0000  ,   // foc_roc_20 
   0x0000  ,   // foc_roc_21 
   0x0000  ,   // foc_roc_22 
   0x0000  ,   // foc_roc_23 
   0x0000  ,   // foc_roc_24 
   0x0000  ,   // foc_roc_25 
   0x0000  ,   // foc_roc_26 
   0x0000  ,   // foc_roc_27 
   0x0000  ,   // foc_roc_28 
   0x0000  ,   // foc_roc_29 
   0x0000  ,   // foc_roc_30 
   0x0000  ,   // foc_roc_31 
   0x0000  ,   // foc_roc_32 
   0x0000  ,   // foc_roc_33 
   0x0000  ,   // foc_roc_34 
   0x0000  ,   // foc_roc_35 
   0x0000  ,   // foc_roc_36 
   0x0000  ,   // foc_roc_37 
   0x0000  ,   // foc_roc_38 
   0x0000  ,   // foc_roc_39 
   0x0000  ,   // foc_roc_40 
   0x0000  ,   // foc_roc_41 
   0x0000  ,   // foc_roc_42 
   0x0000  ,   // foc_roc_43 
   0x0000  ,   // foc_roc_44 
   0x0000  ,   // foc_roc_45 
   0x0000  ,   // foc_roc_46 
   0x0000  ,   // foc_roc_47 
   0x0000  ,   // foc_roc_48 
   0x0000  ,   // foc_roc_49 
   0x0000  ,   // foc_roc_50 
   0x0000  ,   // foc_roc_51 
   0x0000  ,   // foc_roc_52 
   0x0000  ,   // foc_roc_53 
   0x0000  ,   // foc_roc_54 
   0x0000  ,   // foc_roc_55 
   0x0000  ,   // foc_roc_56 
   0x0000  ,   // foc_roc_57 
   0x0000  ,   // foc_roc_58 
   0x0000  ,   // foc_roc_59 
   0x0000  ,   // foc_roc_60 
   0x0000  ,   // foc_roc_61 
   0x0000  ,   // foc_roc_62 
   0x0000  ,   // foc_roc_63 
   0x0000  ,   // foc_roc_64 
   0x0000  ,   // foc_roc_65 
   0x0000  ,   // foc_roc_66 
   0x0000  ,   // foc_roc_67 
   0x0000  ,   // foc_roc_68 
   0x0000  ,   // foc_roc_69 
   0x0000  ,   // foc_roc_70 
   0x0000  ,   // foc_roc_71 
   0x0000  ,   // foc_roc_72 
   0x0000  ,   // foc_roc_73 
   0x0000  ,   // foc_roc_74 
   0x0000  ,   // foc_roc_75 
   0x0000  ,   // foc_roc_76 
   0x0000  ,   // foc_roc_77 
   0x0000  ,   // foc_roc_78 
   0x0000  ,   // foc_roc_79 
   0x7FC0  ,   // FE_learndfct_leak 
   0x0000  ,   // FE_learndfct_value 
   0x1000  ,   // FE_learndfct_scale 
   0x0004  ,   // FE_learndfct_time 
   0x7FC0  ,   // TE_learndfct_leak 
   0x0000  ,   // TE_learndfct_value 
   0x0000  ,   // TE_learndfct_scale 
   0x0000  ,   // reserved_79C 
   0x0000  ,   // reserved_7A0 
   0x0000  ,   // reserved_7A4 
   0xe000  ,   // kb20 
   0x1e5c  ,   // kb21 
   0x8000  ,   // kb22 
   0x7fff  ,   // kf30 
   0x7fe9  ,   // kf31 
   0x8000  ,   // kf32 
   0x0000  ,   // klg10 
   0x0000  ,   // klg11 
   0x0000  ,   // klg12 
   0x0000  ,   // klg13 
   0x0000  ,   // klg14 
   0x1000  ,   // kb30 
   0x7000  ,   // kb31 
   0x0000  ,   // reserved_7DC 
   0x0000  ,   // ks10 
   0x0000  ,   // ks11 
   0x0000  ,   // ks12 
   0x0000  ,   // ks13 
   0x0000  ,   // ks14 
   0x0000  ,   // ks15 
   0x0000  ,   // ks16 
   0x0000      // ks17 
};

// END OF FILE
