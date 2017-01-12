/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   BdDfmg.c
*
* DESCRIPTION
*
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/


/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\powerctl.h"
#include ".\hif\gp_mac.h"
#if (BD_ENABLE)
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#include ".\defmgmt\bdwritable\bdTest.h"
#include ".\defmgmt\bdwritable\bddfmg.h"


/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
void BdTest(BYTE TestItem, BYTE SubItem);


ULONG FakeBdrDefectEntry[10][2] = {{0x00120000, 0x00125700},
                                   {0x00120100, 0x00125720},
                                   {0x00120780, 0x10125800},
                                   {0x00120880, 0x20125900},
                                   {0x00120920, 0x00125780},
                                   {0x00400000, 0x00130000},
                                   {0x00C67300, 0x10196E80},
                                   {0x00C673E0, 0x20196F60},
                                   {0x10140000, 0x00000000},
                                   {0xFFFFFFFF, 0x00000000}
                                  };

ULONG FakeBdreDefectEntry[10][2] ={{0x001AC640, 0x00100000},
                                   {0x001AC6E0, 0x00100020},
                                   {0x001B0040, 0x00100040},
                                   {0x0020CA00, 0x00C67400},
                                   {0x0020CA60, 0x00C67420},
                                   {0x20000000, 0x00100060},
                                   {0x20000000, 0x00100080},
                                   {0x20000000, 0x00C67440},
                                   {0xFFFFFFFF, 0x00000000}
                                  };
/*********************************************************************
 FUNCTIONS
*********************************************************************/
/*********************************************************************
FUNCTION    : BdTest
DESCRIPTION : Verify basic BD functionalities with simulation.
PARAMETERS  : 1. TestItem : given by user.
              2. SubItem  : SubItems under TestItem.
              Example: BdTest(3,2) will install a fake disc information for BD-R.
                       BdTest(3,4) will install a fake disc information for BD-RE.
RETURNS     : None
NOTES       : No real disc is needed.
*********************************************************************/
#if 0//currently not be use
void BdTest(BYTE TestItem, BYTE SubItem)
{
    tBdDiscInformationS *pTempDI_L0;
    //tBdDiscInformationS *pTempDI_L1;
    tBdDdsS *pTempDDS;
    ULONG *pEntry;
    BYTE  i;

#if 0
    //temp setting for Disc Information
    Info.Media.BD.DiscInfoP_L0_Fmt1 = (tBdDiscInformationS *) (BD_WRITABLE_DIUNIT_STRAT_ADDR);
    Info.Media.BD.DiscInfoP_L1_Fmt1 = (tBdDiscInformationS *) (BD_WRITABLE_DIUNIT_STRAT_ADDR + BD_WRITABLE_DIUNIT_LENGTH);

    pDDS = (tBdDdsS *) (BD_DDS_START_ADDR);
#endif

    switch(TestItem)
    {
        ReplacementInfoType     ReplacementInfo;

        case eBdrDflCollection:
            Info.Media.BD.DiscInfoP_L0_Fmt1->BDStructure = 0x12;  //BD-R SL
            send_msg5SValue(SEND_MRW_MSG,"Layer=",1,pdbGetBDLayers());

            //Install a fake BD-R dfl
            for (i = 0; i < 64; i++)
                SCSDRAM[i] = 0x00;
            SCSDRAM[0] = 'D';
            SCSDRAM[1] = 'L';
            SCSDRAM[15] = 0x9;  //Entry Number
            pEntry = (ULONG *) (SCSDRAM + 64);
            for (i = 0; i < (SCSDRAM[15] + 1); i++)
            {
                *pEntry++ = SWAPEND_ULONG(FakeBdrDefectEntry[i][0]);
                *pEntry++ = SWAPEND_ULONG(FakeBdrDefectEntry[i][1]);
            }
            //Install a fake BD-R dfl

            //Initialize Hash Tables of DFL Pool
            BdInitDflHashTable();
            //Initialize Defect Entries of DFL Pool
            BdInitDflEntries();


            SaveBdrDefectListContent(SCSDRAM_ADDRESS);

            //BdDebugPrintEntries();

            //BdrAssembleDefectList();

            //BdWritableRead(0x5, 0x2);
            //BdWritableRead(0x5, 0x20);
            //BdWritableRead(0x5, 0x40);
            //BdWritableRead(0x000775, 0x40);
            //BdWritableRead(0x0007C5, 0x40);
            //BdWritableRead(0x000875, 0x40);
            if (SearchReplacementInfo(0x120000,&ReplacementInfo))
            {
                send_msg5SValue(SEND_MRW_MSG,"rep = ",4,ReplacementInfo.ReplacementPba);
            }
            else
            {
                send_msg5SValue(SEND_MRW_MSG,"def = ",4,bdConvertLBAtoPSN(0x0));
            }



            break;

        case eBdreDflCollection:
            Info.Media.BD.DiscInfoP_L0_Fmt1->BDStructure = 0x14;  //BD-RE SL

            //Install a fake BD-RE dfl
            //BD-RE DFL is loaded into SDRAM_START by a vendor command,
            //If you want to use the DFL, you have to run the command (0xFB) first.

            //Initialize Hash Tables of DFL Pool
            BdInitDflHashTable();
            //Initialize Defect Entries of DFL Pool
            BdInitDflEntries();

            SaveBdreDefectListContent(SCSDRAM_ADDRESS);

            //BdDebugPrintEntries();
            if (SearchReplacementInfo(0x0020CA60,&ReplacementInfo))
            {
                send_msg5SValue(SEND_MRW_MSG,"rep = ",4,ReplacementInfo.ReplacementPba);
            }
            else
            {
                send_msg5SValue(SEND_MRW_MSG,"def = ",4,0x0020CA60);
            }

            BdreAssembleDefectList();

            break;

        case eBdInstallFakeDiscInfo:
            send_msg5S(1,"InstallFakeDI");

            //Reset the work area for DI and DDS
            // LinearPatternFill(        USHORT trgpage, USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern)
            LinearPatternFill((USHORT)BD_WRITABLE_DIUNIT_STRAT_PAGE,   0,            1,        0x800/2,              0);
            LinearPatternFill((USHORT)BD_DDS_START_PAGE,               0,            1,        0x800/2,              0);

            switch(SubItem)
            {
                case BD_ROM :
                    send_msg5S(1,"BD_ROM");
                    break;

                case BD_R :
                    send_msg5S(1,"BD_R");
                    // BD Common Info.
                    pTempDI_L0 = (tBdDiscInformationS *) (BD_WRITABLE_DIUNIT_STRAT_ADDR);
                    pTempDI_L0->DIIdH = 'D';
                    pTempDI_L0->DIIdL = 'I';
                    pTempDI_L0->LayerTypeIdH = 'B';
                    pTempDI_L0->LayerTypeIdM = 'D';
                    pTempDI_L0->LayerTypeIdL = 'R';   //BD-R
                    pTempDI_L0->BDStructure  = 0x12;  //BD-R SL

                    // BD Writable discs specific info.
                    pTempDI_L0->Disc.BdWritable.FPAAOfDataZone = SWAPEND_ULONG(0x20000);
                    pTempDI_L0->Disc.BdWritable.LPAAOfDataZone = SWAPEND_ULONG(0x194E7E);

                    // BD-R DDS INfo
                    pTempDDS = (tBdDdsS *) (BD_DDS_START_ADDR);
                    pTempDDS ->BdRReDds.DDSIdH = 'D';
                    pTempDDS ->BdRReDds.DDSIdL = 'S';
                    pTempDDS ->BdRReDds.PSN_DA = 0x0;
                    pTempDDS ->BdRReDds.Start_LSN0_PSN = SWAPEND_ULONG(0x120000);
                    pTempDDS ->BdRReDds.Last_LSN       = SWAPEND_ULONG(0xb473ff);
                    pTempDDS ->BdRReDds.ISA0_Size      = SWAPEND_ULONG(0x1000);
                    pTempDDS ->BdRReDds.OSA_Size       = SWAPEND_ULONG(0x2000);

                    break;

                case BD_RE :
                    send_msg5S(1,"BD_RE");
                    // BD Common Info. layer0
                    pTempDI_L0 = (tBdDiscInformationS *) (BD_WRITABLE_DIUNIT_STRAT_ADDR);
                    pTempDI_L0->DIIdH = 'D';
                    pTempDI_L0->DIIdL = 'I';
                    pTempDI_L0->LayerTypeIdH = 'B';
                    pTempDI_L0->LayerTypeIdM = 'D';
                    pTempDI_L0->LayerTypeIdL = 'W';   //BD-RE
                    pTempDI_L0->BDStructure  = 0x14;  //BD-RE SL

                    // BD Writable discs specific info.
                    pTempDI_L0->Disc.BdWritable.FPAAOfDataZone = SWAPEND_ULONG(0x20000);
                    pTempDI_L0->Disc.BdWritable.LPAAOfDataZone = SWAPEND_ULONG(0x194E7E);

                    // Disc Info. layer1
                    //pTempDI_L1 = (tBdDiscInformationS *) (BD_WRITABLE_DIUNIT_STRAT_ADDR + BD_WRITABLE_DIUNIT_LENGTH);
                    //pTempDI_L1->Disc.BdWritable.FPAAOfDataZone = SWAPEND_ULONG(0x26B180);
                    //pTempDI_L1->Disc.BdWritable.LPAAOfDataZone = SWAPEND_ULONG(0x3DFFFE);

                    // BD-RE DDS INfo
                    pTempDDS = (tBdDdsS *) (BD_DDS_START_ADDR);
                    pTempDDS ->BdRReDds.DDSIdH = 'D';
                    pTempDDS ->BdRReDds.DDSIdL = 'S';
                    pTempDDS ->BdRReDds.PSN_DA = 0x0;
                    pTempDDS ->BdRReDds.Start_LSN0_PSN = SWAPEND_ULONG(0x120000);
                    pTempDDS ->BdRReDds.Last_LSN       = SWAPEND_ULONG(0xb473ff);
                    pTempDDS ->BdRReDds.ISA0_Size      = SWAPEND_ULONG(0x1000);
                    pTempDDS ->BdRReDds.OSA_Size       = SWAPEND_ULONG(0x2000);
                    //pTempDDS ->ISA1_Size      = SWAPEND_ULONG(0x4000);


                    //Install a simple fake BD-RE dfl
                    for (i = 0; i < 64; i++)
                        SCSDRAM[i] = 0x00;
                    SCSDRAM[0] = 'D';
                    SCSDRAM[1] = 'L';
                    SCSDRAM[15] = 0x8;  //Entry Number
                    pEntry = (ULONG *) (SCSDRAM + 64);
                    for (i = 0; i < (SCSDRAM[15] + 1); i++)
                    {
                        *pEntry++ = SWAPEND_ULONG(FakeBdreDefectEntry[i][0]);
                        *pEntry++ = SWAPEND_ULONG(FakeBdreDefectEntry[i][1]);
                    }
                    //Install a simple fake BD-RE dfl

                    break;

                default :
                    break;
            }
            break;

        case eBdCommonFunctionVerify:
            send_msg5S(SEND_MRW_MSG,"FunctionVerify");

            send_msg5SValue(SEND_MRW_MSG,"Lsn0_PSN",4,pdbGetBDLsn0StartPSN());
            send_msg5SValue(SEND_MRW_MSG,"SPSN_L0",4,pdbGetBDStartPSN(0));
            send_msg5SValue(SEND_MRW_MSG,"LeadOut_S",4,pdbGetBDLeadOutStartPSN());
            send_msg5SValue(SEND_MRW_MSG,"MaxPSN_L0",4,pdbGetBDMaxPSN(0));
            send_msg5SValue(SEND_MRW_MSG,"EndLSN",4,pdbGetBDEndLSN());
            send_msg5SValue(SEND_MRW_MSG,"ISA0 Size",4,pdbGetBDIsa0Size());
            send_msg5SValue(SEND_MRW_MSG,"OSA Size",4,pdbGetBDOsaSize());
            send_msg5SValue(SEND_MRW_MSG,"ISA1 Size",4,pdbGetBDIsa1Size());

            send_msg5S(SEND_MRW_MSG,"*************");
            send_msg5SValue(SEND_MRW_MSG,"DA0_start",4,pdbGetBdDAStartAddress(0));
            send_msg5SValue(SEND_MRW_MSG,"DA0_end",4,pdbGetBdDAEndAddress(0));
            send_msg5SValue(SEND_MRW_MSG,"DA1_start",4,pdbGetBdDAStartAddress(1));
            send_msg5SValue(SEND_MRW_MSG,"DA1_end",4,pdbGetBdDAEndAddress(1));
            send_msg5SValue(SEND_MRW_MSG,"ISA0_start",4,pdbGetBdISAStartAddress(0));
            send_msg5SValue(SEND_MRW_MSG,"ISA0_end",4,pdbGetBdISAEndAddress(0));
            send_msg5SValue(SEND_MRW_MSG,"OSA0_start",4,pdbGetBdOSAStartAddress(0));
            send_msg5SValue(SEND_MRW_MSG,"OSA0_end",4,pdbGetBdOSAEndAddress(0));
            send_msg5SValue(SEND_MRW_MSG,"OSA1_start",4,pdbGetBdOSAStartAddress(1));
            send_msg5SValue(SEND_MRW_MSG,"OSA1_end",4,pdbGetBdOSAEndAddress(1));
            send_msg5SValue(SEND_MRW_MSG,"ISA1_start",4,pdbGetBdISAStartAddress(1));
            send_msg5SValue(SEND_MRW_MSG,"ISA1_end",4,pdbGetBdISAEndAddress(1));

            break;
        case eBdOtherTest:
            pTempDDS->BdRReDds.Disc.BDR.PreWriteFlag = 0x00;
            break;

        default:
            send_msg5S(1,"TestItem Invalid");
            break;
    }
    send_msg5S(SEND_MRW_MSG,"#BD Test Done#");
}
#endif //currently not be use


#endif /* BD_ENABLE */
