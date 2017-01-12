/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   AFE_TBL.H
*
* DESCRIPTION
*
*   $Revision: 39 $
*   $Date: 10/11/23 7:59p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _AFE_TBL_
#define _AFE_TBL_

typedef struct data_field_s
{
    ULONG   addr;
    BYTE    mask;
    BYTE    start_bit;
}data_field_t;

typedef struct afe_offset_info_type1_s
{
    BYTE            total_Bits;
    ULONG           field_write_ofs;
#if (CHIP_REV >= 0xC0)
    ULONG           field_land_write_ofs;
#endif    
    ULONG           field_groove_ofs[2];
    ULONG           field_land_ofs[2];
}afe_offset_info_type1_t;

typedef struct afe_offset_info_type2_s
{
    BYTE            total_Bits;
    ULONG           field_write_ofs;
    ULONG           field_normal_ofs;
}afe_offset_info_type2_t;

typedef struct afe_offset_info_type3_s
{
    BYTE            total_Bits;
    ULONG           field_normal_ofs;
}afe_offset_info_type3_t;

typedef struct afe_offset_info_type4_s
{
    BYTE            total_Bits;
    data_field_t    field_normal_ofs;
}afe_offset_info_type4_t;

typedef struct afe_gain_info_type1_s
{
    BYTE            MaxGainCnt;
    data_field_t    field_write_gn;
    data_field_t    field_groove_gn[2];
    data_field_t    field_land_gn[2];
}afe_gain_info_type1_t;

typedef struct afe_gain_info_type2_s
{
    BYTE            MaxGainCnt;
    data_field_t    field_write_gn;
    data_field_t    field_normal_gn;
}afe_gain_info_type2_t;

typedef struct afe_gain_info_type3_s
{
    BYTE            MaxGainCnt;
    data_field_t    field_normal_gn;
}afe_gain_info_type3_t;

#if (AFE_NORMALIZER == 1)
typedef struct afe_bs_info_nor_s
{
    BYTE            MaxBsCnt;
    data_field_t    field_normal_bs;
}afe_bs_info_nor_t;
#endif

/**********************************************
       External Tables
***********************************************/
extern afe_offset_info_type1_t const AfeOffsetInfoT1[MAX_TYPE1_OFS_NO];
extern afe_offset_info_type2_t const AfeOffsetInfoT2[MAX_TYPE2_OFS_NO];
extern afe_offset_info_type3_t const AfeOffsetInfoT3[MAX_TYPE3_OFS_NO];
extern afe_offset_info_type4_t const AfeOffsetInfoT4[MAX_TYPE4_OFS_NO];
extern afe_gain_info_type1_t const AfeGainInfoT1[MAX_TYPE1_GN_NO];
extern afe_gain_info_type2_t const AfeGainInfoT2[MAX_TYPE2_GN_NO];
extern afe_gain_info_type3_t const AfeGainInfoT3[MAX_TYPE3_GN_NO];
#if (AFE_NORMALIZER == 1)
extern afe_bs_info_nor_t const AfeBsInfoNor[MAX_NOR_BS_NO];
#endif
#endif /* _AFE_TBL_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
