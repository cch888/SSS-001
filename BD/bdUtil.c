/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdUtil.c $
*
* DESCRIPTION   This file contains BD related utilities
*
* $Revision: 17 $
* $Date: 11/03/15 2:21p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globtype.h"
#include ".\hif\Gp_mac.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdUtil.h"
#include ".\oem\Oem_spd_tbl.h"

#if (BD_ENABLE)
/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/

/*********************************************************************
   FUNCTIONS
*********************************************************************/

#if 0//currently not be use
/*****************************************************************************

    FUNCTION        bdConvertADIPtoPSN

    DESCRIPTION     This function converts a Logical Block Address (LBA) to a
                    Physical Sector Number (PSN).

    PARAMETERS      lba          - Logical block address to be translated

    RETURNS         Target.PSN    - PSN that was translated from PAA
                    Target.Layer  - returned in parameter

*****************************************************************************/
PSNs  bdConvertADIPtoPSN(ULONG adip)
{
    /* Please make sure adip is at cluster start */

    PSNs Target;

    if (pdbGetBDLayers() == BD_DUAL_LAYER)
    {
        if (adip < pdbGetBDL1StartPAA())
        {
            /* Target is in Layer 0 */
            Target.Layer = eLayer0;
        }
        else
        {
            /* Target is in Layer 1 */
            Target.Layer  = eLayer1;
        }
    }
    else
    {
        Target.Layer = eLayer0;
    }

    Target.PSN = adip * PAA2PSN_FACTOR;

    return (Target);
}
#endif //currently not be use


/*****************************************************************************

    FUNCTION        bdConvertPSNtoLBA

    DESCRIPTION     This function converts a Physical Sector Number (PSN)to a
                    Logical Block Address (LBA).

    PARAMETERS      PSN

    RETURNS         LBA

    NOTES

*****************************************************************************/
ULONG bdConvertPSNtoLBA(ULONG PSN)
{
    PSN &= 0x0FFFFFFFL;

    if (PSN < BD_DATAZONE_START_PSN)
        return (0);
    else
    {
        if (pdbGetBDLayers() == BD_DUAL_LAYER)
        {
            if (PSN <= LSN0EndPSN)
                return (PSN - LSN0StartPSN);
            else if (PSN >= LSN1StartPSN)
                return (PSN - LSN1OffsetPSN);
            else
            {
                SendMsg5SValue(1, "Invalid_PSN", 4, PSN);
                return(LSN0EndPSN - LSN0StartPSN); 
            }
        }
        else
            return (PSN - LSN0StartPSN);
    }
}


/*****************************************************************************

    FUNCTION        bdConvertLBAtoPSN

    DESCRIPTION     This function converts a Logical Block Address (LBA) to a
                    Physical Sector Number (PSN).

    PARAMETERS      lba          - Logical block address to be translated

    RETURNS         Target.PSN    - PSN that was translated from lba
                    Target.Layer  - returned in parameter

*****************************************************************************/
PSNs  bdConvertLBAtoPSN(ULONG LBA)
{
    PSNs Target;

    if (pdbGetBDLayers() == BD_DUAL_LAYER)
    {
        if (LBA <= Layer0EndLBA)
        {
            /* Target is in Layer 0 */
            Target.Layer  = eLayer0;
            Target.PSN   = LBA + LSN0StartPSN;
        }
        else
        {
            /* Target is in Layer 1 */
            Target.Layer  = eLayer1;
            Target.PSN  = LBA + LSN1OffsetPSN;
        }
    }
    else
    {
        Target.Layer = eLayer0;
        Target.PSN   = LBA + LSN0StartPSN;
    }

    return (Target);
}

void bdConvert_init(void)
{
    switch(pdbGetDiscKind())
    {
        case eBDROMDisc:
        case eBDROMDLDisc:
            LSN0StartPSN = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone);
            LSN0EndPSN = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LAUOfDataZone)+1;
            Layer0EndLBA = LSN0EndPSN - LSN0StartPSN;
            LSN1StartPSN = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.FAUOfDataZone);
            LSN1EndPSN = SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.LAUOfDataZone)+1;
            LSN1OffsetPSN = LSN1StartPSN - (Layer0EndLBA + 1);
            break;
        default:
            LSN0StartPSN = pdbGetBDLsn0StartPSN();
            LSN0EndPSN = ((pdbGetBDL0EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1; // Physical L0 End PSN
            LSN0EndPSN -= (pdbGetBDOsaSize() * SECTORS_PER_CLUSTER);
            Layer0EndLBA = LSN0EndPSN - LSN0StartPSN;
            LSN1StartPSN = ConvertPSNL0ToL1(LSN0EndPSN);
            LSN1EndPSN = ((pdbGetBDL1EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1; // Physical L1 End PSN
            LSN1EndPSN -= (pdbGetBDIsa1Size() * SECTORS_PER_CLUSTER);
            LSN1OffsetPSN = LSN1StartPSN - (Layer0EndLBA + 1);
            break;
    }
    send_msg5SValue(SEND_SStpTask_MSG, "L0StartPSN", 4, LSN0StartPSN);
    send_msg5SValue(SEND_SStpTask_MSG, "L0EndPSN", 4, LSN0EndPSN);
    send_msg5SValue(SEND_SStpTask_MSG, "L0EndLBA",4, Layer0EndLBA);
    send_msg5SValue(SEND_SStpTask_MSG, "L1StartPSN", 4, LSN1StartPSN);
    send_msg5SValue(SEND_SStpTask_MSG, "L1EndPSN", 4, LSN1EndPSN);
    send_msg5SValue(SEND_SStpTask_MSG, "L1OfsPSN", 4, LSN1OffsetPSN);
}
/*****************************************************************************

    FUNCTION        VerifyPACRules

    DESCRIPTION     Determines the read/write area is allow or not.

    PARAMETERS      ePACArea/eDataArea/eCtrlDataZone, eRead/eWrite

    RETURNS         TRUE: not allow/FALSE: allow

*****************************************************************************/
BYTE VerifyPACRules(BYTE ePACRuleArea, BYTE eCtrlType)
{
    BYTE i,j,result;
    tBdPacS * tempPAC;
    PSNs bdEndPSN,bdStartPSN;

    result = FALSE;
    bdStartPSN = bdConvertLBAtoPSN(HostXfer.StartBlock.lba);
    bdEndPSN = bdConvertLBAtoPSN(HostXfer.StartBlock.lba+HostXfer.BlockCount.Long - 1L);
    for(i=1;i<PACcnt;i++)
    {
        tempPAC = (tBdPacS *)(BD_PRM_PAC_ADR + i*DRAM_PAGE_SIZE);
        switch( ePACRuleArea )
        {
            case eDataArea:
                if(tempPAC->NumofSegs == 0)
                    break;
                else
                {
                    result = (SWAPEND_ULONG(tempPAC->UnknowPACRules) >> ePACRuleArea) & eCtrlType;
                    if(result)
                    {
                        for(j=0;j<tempPAC->NumofSegs;j++)
                        {
                            if((bdStartPSN.PSN >= SWAPEND_ULONG(tempPAC->PACSegment[j].StartPSN))
                                && (bdEndPSN.PSN < SWAPEND_ULONG(tempPAC->PACSegment[j].EndPSN)))
                            {
                                send_msg5SValue(1,"NotAllow",2,result);
                                return(TRUE);// not allow read
                            }
                            else
                            {
                                result = FALSE;
                            }
                        }
                    }
                }
                break;
            case eCtrlDataZone:
                result = (SWAPEND_ULONG(tempPAC->UnknowPACRules) >> ePACRuleArea) & eCtrlType;
                break;
            default:
                return(FALSE);
        }
    }
    return (result);

}

/*****************************************************************************

    FUNCTION        BDReturnMediaOffsetForRead

    DESCRIPTION     Determines the row offset into the BDSelSpeed_tbl_R table
                    based on the media type in the drive

    PARAMETERS      None

    RETURNS         Row Offset

*****************************************************************************/
BYTE BDReturnMediaOffsetForRead(void)
{
    BYTE    media;

    /*** Determine offset (row) based on the media type into theDVDSelSpeed_tbl_R table ***/
    switch(pdbGetDiscKind())
    {
        case eBDROMDisc:
            media = eBDROMSL_R;         /* Set BD-ROM SL disc */
            break;

        case eBDROMDLDisc:
            media = eBDROMDL_R;         /* Set BD-ROM DL disc */
            break;

        case eBDRDisc:
            media = eBDRSL_R;           /* Set BD +/-R SL Closed Session disc */
            break;

        case eBDREDisc:
            media = eBDRESL_R;          /* Set BD-RE SL Closed Session disc */
            break;

        case eBDRDLDisc:
            media = eBDRDL_R;           /* Set BD +/-R DL Closed Session disc */
            break;

        case eBDREDLDisc:
            media = eBDREDL_R;          /* Set BD +/-R DL Closed Session disc */
            break;

        default:
            media = eBDROMSL_R;             /* Set Default as DVD-ROM SL disc */
            break;
    }

    return(media);
}


/*****************************************************************************

    FUNCTION        SelectBDReadSpeed

    DESCRIPTION     Selects speed for closed area

    PARAMETERS      index of speed table

    RETURNS         Selected speed

*****************************************************************************/
//needs to be checked !!
BYTE SelectBDReadSpeed(BYTE r_no)
{
    BYTE media_no,sp_c;

    media_no = BDReturnMediaOffsetForRead();  /* Determine the row offset based on the media type */
    sp_c = BDSelSpeed_tbl_R[media_no][r_no];

    if (GetSpeedValue(sp_c) > GetSpeedValue(Disc_SupportSpeed.Read))
    {
        sp_c = Disc_SupportSpeed.Read;
    }

#if (DETECT_ECC_DISC == 1)
    if (svoVar_eccDiscNum != 0)         /* Check if Eccentric Disc is in system */
    {
        /* Get Initial Speed based on Eccentricity */
        if (sp_c > ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_eccDiscNum])
           sp_c = ECCDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_eccDiscNum];;
    }
#endif

#if (BD_UNBALANCE_DISC_CHECK == 1)
    if (svoVar_unbalanceDiscNum != 0)         /* Check if Unbalanced Disc is in system */
    {
       if (sp_c > UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_unbalanceDiscNum])
          sp_c = UnBalDiscSpd_tbl[eECC_UNB_MAX_SPD_BD][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on unbalance */
    }
#endif

    return(sp_c);
}

#if 0//currently not be use
#if (MSG_OUT_MODE == MSG_USBPORT)
/*****************************************************************************

    FUNCTION        DisplayBdDataBase

    DESCRIPTION     Display player DataBase for BD disc

    PARAMETERS      None

    RETURNS         None

*****************************************************************************/
void DisplayBdDataBase(void)
{
    BYTE LastTrackNumber;
    BYTE Index;

    LastTrackNumber = pdbGetBDFinalSessLastTrackNumber();

    DelaymS(1);
    for (Index = 1; Index <= (LastTrackNumber +1) ; Index++)
    {
        send_msg5SValue(SEND_HIF_CMD, "Tk #", 1, Index);
        //send_msg5SValue(SEND_HIF_CMD, "Trk Cont(rol) Addr", 1, pdbGetDVDTrackContAddr( Index ));
        //send_msg5SValue(SEND_HIF_CMD, "Trk Con Addr. (nibble) =", 1, pdbGetDVDTrackConAddr( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk Sts", 1, pdbGetBDTrackStatus( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk Sta", 1, pdbGetBDTrackState( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk D-Sts", 1, pdbGetBDTrackDiagStatus( Index ));
        DelaymS(1);
        send_msg5SValue(SEND_HIF_CMD, "Trk D-Sts2", 1, pdbGetBDTrackDiagStatus2( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk Sess#", 1, pdbGetBDTrackSessionNumber( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk St-PSN", 4, pdbGetBDTrackStartPSN( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk St-Lyr", 4, pdbGetBDTrackStartPSNLayer( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk Ed-PSN", 4, pdbGetBDTrackEndPSN( Index ));
        DelaymS(1);
        send_msg5SValue(SEND_HIF_CMD, "Trk Ed-Lyr", 4, pdbGetBDTrackEndPSNLayer( Index ));
        send_msg5SValue(SEND_HIF_CMD, "RecEd PSN", 4, pdbGetBDTrackRecordedPSN( Index ));
        //send_msg5SValue(SEND_HIF_CMD, "Trk RecEd Lyr", 4, pdbGetDVDTrackRecordedPSNLayer( Index ));
        send_msg5SValue(SEND_HIF_CMD, "Trk NWA", 4, pdbGetBDTrackNextWritableAddress( Index ));
        //send_msg5SValue(SEND_HIF_CMD, "Trk Wri Meth", 1, pdbGetDVDTrackWriteMethod( Index ));
        //send_msg5SValue(SEND_HIF_CMD, "Trk Packet Size        =", 4, pdbGetDVDTrackPacketSize( Index ));
        //send_msg5SValue(SEND_HIF_CMD, "Trk Data Mode          =", 1, pdbGetDVDTrackDataMode( Index ));
        //send_msg5SValue(SEND_HIF_CMD, "Trk Data Form          =", 1, pdbGetDVDTrackDataForm( Index ));
        send_msg5S(SEND_HIF_CMD, "...............");
        DelaymS(1);
    }

    send_msg5S(SEND_HIF_CMD, "Final Sess Inf");
    send_msg5SValue(SEND_HIF_CMD, "Clos-Info", 1, pdbGetBDFinalSessCloseInfo());
    send_msg5SValue(SEND_HIF_CMD, "Ses #", 1, pdbGetBDFinalSessionNumber());
    send_msg5SValue(SEND_HIF_CMD, "LI PSN", 4, pdbGetBDFinalSessLeadinPSN());
    DelaymS(1);
    send_msg5SValue(SEND_HIF_CMD, "LI LBA", 4, pdbGetBDFinalSessLeadinLBA());
    send_msg5SValue(SEND_HIF_CMD, "LO PSN", 4, pdbGetBDFinalSessLeadoutPSN());
    send_msg5SValue(SEND_HIF_CMD, "LO LBA", 4, pdbGetBDFinalSessLeadoutLBA());
    send_msg5SValue(SEND_HIF_CMD, "1st Trk#", 2, pdbGetBDFinalSessFirstTrackNumber());
    DelaymS(1);
    send_msg5SValue(SEND_HIF_CMD, "Last  Trk", 2, pdbGetBDFinalSessLastTrackNumber());
    send_msg5SValue(SEND_HIF_CMD, "1Trk PSN", 4, pdbGetBDFinalSessFirstTrackPSN());
    //send_msg5SValue(SEND_HIF_CMD, "Format", 1, pdbGetBDFinalSessFormat());
    send_msg5SValue(SEND_HIF_CMD, "Logi Trk Cnt", 1, pdbGetBDHighestRecordedTrackNumber());
    send_msg5S(SEND_HIF_CMD, "***************");
    //send_msg5SValue(SEND_HIF_CMD, "* End of Session Information *");
    //send_msg5SValue(SEND_HIF_CMD, "******************************");
    DelaymS(1);
}
#endif  //(MSG_OUT_MODE == MSG_USBPORT)
#endif //currently not be use

#endif  //(BD_ENABLE)
