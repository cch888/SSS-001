/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   svo_spdtrans.c
*
* DESCRIPTION: Translation table from Speed to actual speed value
*
*
*
* NOTES:
*
*
*
*   $Revision: 17 $
*   $Date: 11/03/16 7:03p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\servo\svo_spdtrans.h"
#if (DVD_710Run815New == 1)
#include ".\player\plrdb.h"
#endif

const USHORT SpeedIndexTableCD[] = {
/* None  */       0,
/* _4LX  */     400,
/* _8LX  */     800,
/* _12LX */    1200,
/* _16LX */    1600,
/* _10AX */    1000,
#if (ENABLE_CD_12XCAV == 1)
/* _12AX */    1200,
#else
/* _16AX */    1600,
#endif
/* _24AX */    2400,
/* _32AX */    3200,
/* _40AX */    4000,
/* _48AX */    4800,
/* _52AX */    5200,
};

const USHORT SpeedIndexTableDVD[] = {
    /* None      */     0,
    /* _2LX_DVD  */   200,
#if(DVD2p4XCLV == 1)
    /* _2.4LX_DVD  */ 240,
#else
    /* _3LX_DVD  */   300,
#endif
    /* _4LX_DVD  */   400,
#if (ENABLE_DVD_4XCAV == 1)
    /* _4AX_DVD  */   400,
#endif
    /* _6AX_DVD  */   600,
    /* _8AX_DVD  */   800,
    /* _10AX_DVD */  1000,
    /* _12AX_DVD */  1200,
    /* _16AX_DVD */  1600,
    /* _20AX_DVD */  2000,
};

const USHORT SpeedIndexTableDVDRAM[] = {
    /* None         */    00,
    /* _1LX_DVDRAM  */   100,
    /* _2LX_DVDRAM  */   200,
    /* _3LX_DVDRAM  */   300,
    /* _5LX_DVDRAM  */   500,
    /* _5AX_DVDRAM  */   500,
    /* _8AX_DVDRAM  */   800,
    /* _12AX_DVDRAM */  1200,
    /* _16AX_DVDRAM */  1600,
};

const USHORT SpeedIndexTableBD[] = {
    /* None     */      0,
    /* _1LX_BD  */    100,
#if((Customer_A2 == 1)||(Customer_China== 1)||(Customer_ZHD== 1)) //[W0K]Shin//[V08] Bios A2
    /* _2LX_BD  */    170,//200,
	#if(BD2p2XCLV == 1)
    /* _2.2LX_BD  */  200,//220, 
	#endif
#else
    /* _2LX_BD  */    200,
	#if(BD2p2XCLV == 1)
    /* _2.2LX_BD  */  220, //for BD 3D
	#endif
#endif
#if (PCB == 711)
    /* _4AX_BD  */    500, //for BD 3D
#else
    /* _4AX_BD  */    400,
#endif
    /* _6AX_BD  */    600,
    /* _8AX_BD  */    800,
    /* _10AX_BD */   1000,
    /* _12AX_BD */   1200,
    /* _14AX_BD */   1400,
};

const USHORT SpeedIndexTableHDDVD[] = {
    /* None     */     0,
    /* _1LX_HD  */   100,
    /* _2LX_HD  */   200,
    /* _4AX_HD  */   400,
};

#if (DVD_710Run815New == 1)
USHORT GetSpeedIndexTableDVD(BYTE spd)
{
    USHORT SpeedIndex;

    SpeedIndex = SpeedIndexTableDVD[spd];

    if(pdbGetServoReadyStatus() == TRUE)
    {
        if((pdbGetDVDType() == DVD_PLUS_R)||(pdbGetDVDType() == DVD_PLUS_RW)||(pdbGetDVDType() == DVD_PLUS_R_DUAL))
        {
             if(spd == _2LX_DVD)
             {
                 SpeedIndex = 240;
                 send_msg5SValue(1, "+RChg24", 1, spd);
             }
        }
    }
    return SpeedIndex;
}
#endif

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
