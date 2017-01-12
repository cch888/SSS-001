/*********************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                      All Rights Reserved
*
*
*
* FILENAME:   dvRPC.c
*
* DESCRIPTION:
*   Code for DVD Region Playback Control.
*
*
*   $Revision: 27 $
*   $Date: 11/03/16 6:28p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
 INCLUDES
*********************************************************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\macros.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\bf_util.h"
#include ".\hif\flash.h"
#include ".\oem\oem_tbl.h"
#include ".\DVD\dvGvars.h"
#include ".\DVD\dvdiscinit.h"
#include ".\DVD\dvMMC.h"
#include ".\DVD\dvRPC.h"
#include ".\al\reg_intctrl.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\common\intctl.h"

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_oem_tbl.h"
#endif

/* The following defines are for Region Playback Control. */

#define REGION_DEFAULT REGION_1

#define RPC_State_NONE          0
#define RPC_State_SET           1
#define RPC_State_LAST_CHANCE   2
#define RPC_State_PERM          3

#if (SUN_FUNC_TEST == 1)
/*******************************************************************************************
*   RpcSetRegion()
*
*   Set a new Region code if new region code is same as current region code then do nothing.
*   Change RPC State and Count according to Specifications for Region Code Playback Control.
*
*******************************************************************************************/
void RpcSetRegion(BYTE NewRegion)
{
    BYTE temp_IE ;
    BYTE i,regioncnt;

    if(NewRegion == 0xFF)   return;                     // Region code free.

    temp_IE = GET_INT_ENABLE_MASK();    // Disable interrupts
    DISABLE_INTERRUPT();                //   while messing with FLASH

    OpenWritableSegmentTable(eEEPROM_Formal_Table);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM

    switch (ReadEepromMap_1B(OEM_RPC_TYPE_ADDR))
    {
        case RPC_State_NONE:
            if(( pdbServoGetDiscDiameter() != eNoDisc )&&((Info.Media.DVD.RegionControl | NewRegion)!= NewRegion))
            {
                if((Info.Media.DVD.RegionControl != 0x00) && (Info.Media.DVD.RegionControl != 0xC0))//all region
                {
                    BUILD_SENSE(0x05,0x6F,0x04,0x04);
                    break;
                }
            }

            if((ReadEepromMap_1B(OEM_RPC_REGION_ADDR) == 0xFF) || (ReadEepromMap_1B(OEM_RPC_REGION_ADDR) == NewRegion))
            {
                SetEEPROMTable1B(OEM_RPC_REGION_ADDR, NewRegion);    // Set Logical Unit Region to the new Region.
                SetEEPROMTable1B(OEM_RPC_USER_ADDR, ReadEepromMap_1B(OEM_RPC_USER_ADDR)-1);    // Decrement # of re-programmings left.
                SetEEPROMTable1B(OEM_RPC_TYPE_ADDR, RPC_State_SET);   // Set next machine state.
                CloseWritableSegmentTable(eEEPROM_Formal_Table);    // Re-write all "Persistent" variables to FLASH to update the new settings.
                //CloseWritableSegmentTable(eEEPROM_Backup_Table);    // Update backup table in the same time
            }
            else  if (ReadEepromMap_1B(OEM_RPC_REGION_ADDR) != NewRegion)
            {
                SetEEPROMTable1B(OEM_RPC_REGION_ADDR, NewRegion);        // else set region to new region
                SetEEPROMTable1B(OEM_RPC_USER_ADDR, ReadEepromMap_1B(OEM_RPC_USER_ADDR)-1);    // Decrement # of re-programmings left.
                SetEEPROMTable1B(OEM_RPC_TYPE_ADDR, RPC_State_SET);   // Set next machine state.
                CloseWritableSegmentTable(eEEPROM_Formal_Table);    // Re-write all "Persistent" variables to FLASH to update the new settings.
                //CloseWritableSegmentTable(eEEPROM_Backup_Table);    // Update backup table in the same time 
            }
            break;
        case RPC_State_SET:
            if( pdbServoGetDiscDiameter() == eNoDisc )      /* No disc */ 
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x01);
                break;
            }

            if((Info.Media.DVD.RegionControl | NewRegion)!= NewRegion)
            {
                if((Info.Media.DVD.RegionControl != 0x00) && (Info.Media.DVD.RegionControl != 0xC0))
                {
                    BUILD_SENSE(0x05,0x6F,0x04,0x04);
                    break;
                }
            }

            if (ReadEepromMap_1B(OEM_RPC_REGION_ADDR) != NewRegion)       // If new region is the same as old region, ignore, do nothing.
            {
                SetEEPROMTable1B(OEM_RPC_REGION_ADDR, NewRegion);        // else set region to new region
                SetEEPROMTable1B(OEM_RPC_USER_ADDR, ReadEepromMap_1B(OEM_RPC_USER_ADDR)-1);    // Decrement # of re-programmings left.
                if (ReadEepromMap_1B(OEM_RPC_USER_ADDR) == 1)//read from SDRAM
                    SetEEPROMTable1B(OEM_RPC_TYPE_ADDR, RPC_State_LAST_CHANCE);   // Count has reached 1 set up for next Machine State LAST CHANCE.
                CloseWritableSegmentTable(eEEPROM_Formal_Table);    // Re-write all "Persistent" variables to FLASH to update the new settings.
                //CloseWritableSegmentTable(eEEPROM_Backup_Table);    // Update backup table in the same time 
            }
            break;
        case RPC_State_LAST_CHANCE:
            if( pdbServoGetDiscDiameter() == eNoDisc )      /* No disc */ 
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x02);
                break;
            }

            if((Info.Media.DVD.RegionControl | NewRegion)!= NewRegion)
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x03);
                break;
            }
//Don't update RPC at state = RPC_State_LAST_CHANCE with multi region disc.
	    regioncnt = 0;
	    for(i=0;i<8;i++)
	    {
		if(((Info.Media.DVD.RegionControl>>i)& bit0)==0)
		    regioncnt++;
	    }
            if(regioncnt>1)// only can be set by single region disc.
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x04);
                break;
            }

            if (ReadEepromMap_1B(OEM_RPC_REGION_ADDR) != NewRegion)       // If new region is the same as old region, ignore, do nothing.
            {
                SetEEPROMTable1B(OEM_RPC_REGION_ADDR, NewRegion);
                SetEEPROMTable1B(OEM_RPC_USER_ADDR, ReadEepromMap_1B(OEM_RPC_USER_ADDR)-1);    // Decrement # of re-programmings left.
                SetEEPROMTable1B(OEM_RPC_TYPE_ADDR, RPC_State_PERM);
                CloseWritableSegmentTable(eEEPROM_Formal_Table);    // Re-write all "Persistent" variables to FLASH to update the new settings.
                //CloseWritableSegmentTable(eEEPROM_Backup_Table);    // Update backup table in the same time 
            }
            break;
        case RPC_State_PERM:
            if (ReadEepromMap_1B(OEM_RPC_REGION_ADDR) != NewRegion)       // If new region is the same as old region, ignore, do nothing.
            {                                                  //   else Check Condition.
                BUILD_SENSE(0x05,0x6F,0x05,0x03);                   // DRIVE REGION MUST BE PERMANENT/REGION RESET COUNT ERROR
            }
            else
            {
                // New region is same as old region, do nothing.
            }
            break;
        default:
            break;   //  Non existing state.
    }
    SET_INT_ENABLE_MASK(temp_IE);   // restore interrupts
}


/*******************************************************************************************
*   RpcGetRegion()
*
*   Read RPC Region settings and return all settings to caller
*
*
*******************************************************************************************/
RPC_NON_VOLATILE_VARS RpcGetRegion(void)

{
    BYTE temp_IE ;
    RPC_NON_VOLATILE_VARS Settings;


    // The RPC Variables are non volatile parameters. They reside in the FLASH memory.
    // They are part of the "PersistentTable"(non-volatile parameters)
    // To modify them they need to copied from FLASH to an area in SDRAM
    // dedicated to temporarily hold these vars.
    // After the vars in SDRAM have been modified, they are all together written back to the FLASH Memory.

    temp_IE = GET_INT_ENABLE_MASK();    // Disable interrupts
    DISABLE_INTERRUPT();                //   while messing with FLASH

    OpenWritableSegmentTable(eEEPROM_Formal_Table);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM

    Settings.RPC.State    = ReadEepromMap_1B(OEM_RPC_TYPE_ADDR);       // Get current RPC State.
    Settings.RPC.nCount   = ReadEepromMap_1B(OEM_RPC_USER_ADDR);      // Get number of changes remaining.
    Settings.RPC.LuRegion = ReadEepromMap_1B(OEM_RPC_REGION_ADDR);    // Get region mask.
    Settings.RPC.VendorResetsAvailable = ReadEepromMap_1B(OEM_RPC_VENDOR_ADDR);    // Get number of Vendor Resets Available.

    SET_INT_ENABLE_MASK(temp_IE);   // restore interrupts
    return (Settings);
}
#else

/*******************************************************************************************
*   RpcSetRegion()
*
*   Set a new Region code if new region code is same as current region code then do nothing.
*   Change RPC State and Count according to Specifications for Region Code Playback Control.
*
*******************************************************************************************/
void RpcSetRegion(BYTE NewRegion)
{
    BYTE temp_IE ;
    BYTE i,regioncnt;

    if(NewRegion == 0xFF)   return;                     // Region code free.

    // The RPC Variables are non volatile parameters. They reside either in the FLASH
    // memory or serial eeprom. They are part of the "PersistentTable"(non-volatile parameters)

    temp_IE = GET_INT_ENABLE_MASK();    // Disable interrupts
    DISABLE_INTERRUPT();                //   while messing with FLASH

    OpenWritableSegmentTable(ePersistent_Talbe);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM

    switch (GetPersistentTable1B(RPC_STATE_OFFSET))
    {
        case RPC_State_NONE:
            
            if((pdbServoGetDiscDiameter() != eNoDisc )&&((Info.Media.DVD.RegionControl | NewRegion)!= NewRegion))
            {
                if((Info.Media.DVD.RegionControl != 0x00) && (Info.Media.DVD.RegionControl != 0xC0))
                {
                    BUILD_SENSE(0x05,0x6F,0x04,0x04);
                    break;
                }
            }

            if((GetPersistentTable1B(RPC_REGION_OFFSET) == 0xFF) || (GetPersistentTable1B(RPC_REGION_OFFSET) == NewRegion))
            {
                SetPersistentTable1B(RPC_REGION_OFFSET, NewRegion);    // Set Logical Unit Region to the new Region.
                SetPersistentTable1B(RPC_COUNT_OFFSET, GetPersistentTable1B(RPC_COUNT_OFFSET)-1);    // Decrement # of re-programmings left.
                SetPersistentTable1B(RPC_STATE_OFFSET, RPC_State_SET);   // Set next machine state.
                CloseWritableSegmentTable(ePersistent_Talbe);    // Re-write all "Persistent" variables to FLASH to update the new settings.
            }
            else  if (GetPersistentTable1B(RPC_REGION_OFFSET) != NewRegion)
            {
                SetPersistentTable1B(RPC_REGION_OFFSET, NewRegion);        // else set region to new region
                SetPersistentTable1B(RPC_COUNT_OFFSET, GetPersistentTable1B(RPC_COUNT_OFFSET)-1);    // Decrement # of re-programmings left.
                SetPersistentTable1B(RPC_STATE_OFFSET, RPC_State_SET);   // Set next machine state.
                CloseWritableSegmentTable(ePersistent_Talbe);    // Re-write all "Persistent" variables to FLASH to update the new settings.
            }
            break;
        case RPC_State_SET:
            if( pdbServoGetDiscDiameter() == eNoDisc )      /* No disc */ 
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x01);
                break;
            }

            if((Info.Media.DVD.RegionControl | NewRegion)!= NewRegion)
            {
                if((Info.Media.DVD.RegionControl != 0x00) && (Info.Media.DVD.RegionControl != 0xC0))
                {
                    BUILD_SENSE(0x05,0x6F,0x04,0x04);
                    break;
                }
            }

            if (GetPersistentTable1B(RPC_REGION_OFFSET) != NewRegion)       // If new region is the same as old region, ignore, do nothing.
            {
                SetPersistentTable1B(RPC_REGION_OFFSET, NewRegion);        // else set region to new region
                SetPersistentTable1B(RPC_COUNT_OFFSET, GetPersistentTable1B(RPC_COUNT_OFFSET)-1);    // Decrement # of re-programmings left.
                if (GetPersistentTable1B(RPC_COUNT_OFFSET) == 2)//read from flashrom
                    SetPersistentTable1B(RPC_STATE_OFFSET, RPC_State_LAST_CHANCE);   // Count has reached 1 set up for next Machine State LAST CHANCE.
                CloseWritableSegmentTable(ePersistent_Talbe);    // Re-write all "Persistent" variables to FLASH to update the new settings.
            }
            break;
        case RPC_State_LAST_CHANCE:
            if( pdbServoGetDiscDiameter() == eNoDisc )      /* No disc */ 
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x02);
                break;
            }

            if((Info.Media.DVD.RegionControl | NewRegion)!= NewRegion)
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x03);
                break;
            }
            regioncnt = 0;
	    for(i=0;i<8;i++)
	    {
	        if(((Info.Media.DVD.RegionControl>>i)& bit0)==0)
		    regioncnt++;
	    }
            if(regioncnt>1)// only can be set by single region disc.
            {
                BUILD_SENSE(0x05,0x6F,0x04,0x04);
                break;
            }
            if (GetPersistentTable1B(RPC_REGION_OFFSET) != NewRegion)       // If new region is the same as old region, ignore, do nothing.
            {
                SetPersistentTable1B(RPC_REGION_OFFSET, NewRegion);
                SetPersistentTable1B(RPC_COUNT_OFFSET, GetPersistentTable1B(RPC_COUNT_OFFSET)-1);    // Decrement # of re-programmings left.
                SetPersistentTable1B(RPC_STATE_OFFSET, RPC_State_PERM);
                CloseWritableSegmentTable(ePersistent_Talbe);    // Re-write all "Persistent" variables to FLASH to update the new settings.
            }
            break;
        case RPC_State_PERM:
            if (GetPersistentTable1B(RPC_REGION_OFFSET) != NewRegion)       // If new region is the same as old region, ignore, do nothing.
            {                                                  //   else Check Condition.
                BUILD_SENSE(0x05,0x6F,0x05,0x03);                   // DRIVE REGION MUST BE PERMANENT/REGION RESET COUNT ERROR
            }
            else
            {
                // New region is same as old region, do nothing.
            }
            break;
        default:
            break;   //  Non existing state.
    }
    SET_INT_ENABLE_MASK(temp_IE);   // restore interrupts
}


/*******************************************************************************************
*   RpcGetRegion()
*
*   Read RPC Region settings and return all settings to caller
*
*
*******************************************************************************************/
RPC_NON_VOLATILE_VARS RpcGetRegion(void)

{
    BYTE temp_IE ;
    RPC_NON_VOLATILE_VARS Settings;


    // The RPC Variables are non volatile parameters. They reside in the FLASH memory.
    // They are part of the "PersistentTable"(non-volatile parameters)
    // To modify them they need to copied from FLASH to an area in SDRAM
    // dedicated to temporarily hold these vars.
    // After the vars in SDRAM have been modified, they are all together written back to the FLASH Memory.

    temp_IE = GET_INT_ENABLE_MASK();    // Disable interrupts
    DISABLE_INTERRUPT();                //   while messing with FLASH

    OpenWritableSegmentTable(ePersistent_Talbe);      // Copy "Persistent Variables" to Drive Parameter Area in SDRAM

    Settings.RPC.State    = GetPersistentTable1B(RPC_STATE_OFFSET);       // Get current RPC State.
    Settings.RPC.nCount   = GetPersistentTable1B(RPC_COUNT_OFFSET);      // Get number of changes remaining.
    Settings.RPC.LuRegion = GetPersistentTable1B(RPC_REGION_OFFSET);    // Get region mask.
    Settings.RPC.VendorResetsAvailable = GetPersistentTable1B(RPC_VENDOR_RESET_OFFSET);    // Get number of Vendor Resets Available.

    SET_INT_ENABLE_MASK(temp_IE);   // restore interrupts
    return (Settings);
}
#endif
