//////////////////////////////////////////////////////////////////////////////////////////////
//
// FraMES - Framework MES
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// $Source: K:\\PROJECT\\MES\\Rcs\\frames\\software\\wipmama\\WIPMaMa.h,v $
//
// $Revision: 2.85 $
// $Name: V0218b9 $
// $Author: rosskopf $
// $Date: 2009-06-16 13:42:29+02 $
//
// $State: Rel $
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Wip Mama Functions for Information
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// $Log: WIPMaMa.h,v $
// Revision 2.85  2009-06-16 13:42:29+02  rosskopf
// version released
//
// Revision 2.84  2009-05-25 13:45:30+02  loeffler
// Bugfix: wpm_UnitMoveBack and wpm_UnitMoveBackOpt if Scrap Units on panel
//
// Revision 2.83  2009-05-20 16:18:39+02  bauerj
// - bugfix in unitfinish when input parameter "status" is not 'P' or 'F'
//
// Revision 2.82  2009-05-19 09:04:17+02  loeffler
// In Release Mode
//
// Revision 2.81  2009-05-19 09:01:56+02  loeffler
// Project now correct checked in and versioninfo is running again
//
// Revision 2.80  2009-05-12 14:00:00+02  loeffler
// Bugfix in internal function SetUnit looking in partlist if unit is allready assembled and select only those which are not deassembled.
//
// Revision 2.79  2009-05-11 11:31:48+02  loeffler
// Bugfix: In internal function SetUnit has an command deactivate the setting of subsetid on assembling semi units This command whas set on revision V0216b5_beta from 12.02.2009
//
// Revision 2.78  2009-05-06 10:01:27+02  loeffler
// Bugfix wpm_GetOrderOperations fixed WP-164
//
// Revision 2.77  2009-03-19 12:21:11+01  loeffler
// Bugfix WP-162 wpm_MoveBackOpt now with correct equipment after move
//
// Revision 2.76  2009-03-16 14:09:27+01  loeffler
// New Function wpm_SetPosition2(szPosition, szRAWNumber, boNotReserved) where now you can set if the RAWNumber is reseerved for station or not;
// For the same reason is the environment NOT_RESERVE_RAW_UNIT=YES/NO for old wpm_SetPosition
//
// Revision 2.75  2009-03-11 17:44:59+01  loeffler
// <>
//
// Revision 2.74  2009-02-25 14:33:43+01  loeffler
// Bugfix Skip lock and stated units in startup and optimize select statemant for cleanup Wipunits. Build Release Version
//
// Revision 2.73  2009-02-25 13:50:05+01  loeffler
// Make Release Version
//
// Revision 2.72  2009-02-23 15:48:38+01  loeffler
// Bugfix: Memory leak in functi wpm_ReadWipMamaCfg
//
// Revision 2.71  2009-02-20 13:50:49+01  loeffler
// Change of global structure for wpm_STRUCTMATDEFINFO with syntactical correction.(Problem with C++ syntacs) WP-158
//
// Revision 2.70  2009-02-20 11:49:09+01  loeffler
// Bugfix: WP-159 new variables for goods_receipt_nr2 and goods_receipt_pos2 fpr return valus of m_stock_info
//
// Revision 2.69  2009-02-20 09:39:46+01  loeffler
// Bugfix WP-157: If order total quantity = 0 because of MES admin update and 1 unit is already on order and ready to move than set total quantity to 1 so the stock can be created for that unit
//
// Revision 2.68  2009-02-18 10:07:44+01  rosskopf
// another bugfix general protection fault extent partlist feature
//
// Revision 2.67  2009-02-17 15:32:05+01  rosskopf
// bug memory error function wpm_UsePosition solved
//
// Revision 2.66  2009-02-17 12:13:32+01  rosskopf
// bugfix memory error wpm_SlotSetUnitCnt
//
// Revision 2.65  2009-02-12 16:29:05+01  loeffler
// Bugfix: WP-133 Fixed by modifying slot structure and adding assemblycounter for new part in partlist
//
// Revision 2.64  2009-02-06 12:00:09+01  loeffler
// Bugfix: wpm_UnitLoss: remove unit from slot if check in and only scrapp quantity on it
//
// Revision 2.63  2009-01-23 15:51:16+01  loeffler
// Bugfix wpm_GetUnitNextOperations if unit is in checkin or start state WP-138
//
// Revision 2.62  2009-01-22 12:19:20+01  loeffler
// Bugfiy also for wpm_UnitLossSAP
//
// Revision 2.61  2009-01-22 12:10:15+01  loeffler
// Bugfix wpm_UnitLoss for checkin units where quantity for scrap > quantity on slot
//
// Revision 2.60  2009-01-20 16:33:58+01  bauerj
// - new function wpm_GetPartInfo2 to retrieve addtionally lock data if object is locked
// - replaced m_cont_info by m_cont_info_3 in wpm_GetPartInfo and wpm_GetReceiptContainers
//
// Revision 2.59  2009-01-20 09:48:57+01  bauerj
// - New functions for locking and unlocking MaMa batch units (=container)
//
// Revision 2.58  2009-01-19 12:15:00+01  loeffler
// Bugfix wpm_UnitLoss for not exiting units
// Expant new epm_UnitLoss functionality to wpm_UnitLossSAP
//
// Revision 2.57  2009-01-13 13:52:21+01  loeffler
// Bugfix wpm_UnitLoss now also for units that are check in and started
//
// Revision 2.56  2008-12-18 09:53:02+01  bauerj
// - wpm_GetOrderPartList can now handle the input sOperation=0 to return whole partlist of complete order
//
// Revision 2.55  2008-12-15 10:47:48+01  bauerj
// - New function wpm_GetPartInfo to get info for MaMa containers or single semi units
// - New functions to handle extended fields on MaMa material definitions.
// - Changes on wpm_GetUnitLoss for correct handling of complete empty units and existing units that do not have loss quantity.
// - wpm_GetSlotForUnit does no longer work for orders that are locked or terminated.
// - wpm_GetOrderOperations(2) supresses orders with zero quantity.
// - bugixing with controller mode 3 and SUPRESS_ZERO_DISPATCH_JOBS=YES
//
// Revision 2.54  2008-11-25 10:23:32+01  bauerj
// - new function wpm_GetStationGrpId to retrieve the parameters STATION_GRP and STATION_ID
// - bugfix in wpm_SetStationId:  causes no longer a semaphore lock
// - new datatype "Date" for extended fields
// - now also functions for handling exteneded fields on MaMa (container) level (batch units or single units)
// - now using global setting LOG_DEST from FraMES.cfg
//
// Revision 2.53  2008-10-30 15:18:06+01  loeffler
// New function for Material Definition handling:
// - wpm_CreateMatDef
// - wpm_UpdatePartGrpMatDef
// - wpm_GetMatDefInfo
//
// Revision 2.52  2008-10-17 09:09:40+02  bauerj
// - added new function wpm_GetUnitsByAssembly to search for units where given single part was assembled.
//
// Revision 2.51  2008-10-13 15:44:33+02  rosskopf
// wpm_UnitSetScrap wip_loss_add function instead of subset_mvoe
//
// Revision 2.50  2008-10-01 11:37:11+02  bauerj
// - turning from beta to release
//
// Revision 2.49  2008-09-26 13:43:51+02  loeffler
// Change Symbolic name
//
// Revision 2.48  2008-09-26 13:13:41+02  loeffler
// Environment variable DEFAULT_ASSEMBLY_TYPE=BATCH or SINGLE implemented
//
// Revision 2.47  2008-09-11 16:33:59+02  bauerj
// - function wpm_CreateBatchUnits now makes no auto increment of stock
//
// Revision 2.46  2008-09-10 06:40:13+02  bauerj
// - new function wpm_CreateBatchUnits to create more than one container with on function call
// - new function wpm_IsSlotGoldenSample to see if loaded order is a Golden Sample order
// - function wpm_GetOrderInfo2 has now parameter to retrieve the order type 'P', 'T' or 'Q'
// - functions for handling ext. fields now check predefined fieldtype if it is set in conf. table.
// - now correct return value in wpm_IncreaseReceipt
//
// Revision 2.45  2008-09-08 08:47:45+02  bauerj
// - bugfix in wpm_IncreaserRceipt for positions that are not "1"
//
// Revision 2.44  2008-09-02 14:00:28+02  loeffler
// New function wpm_UnitMoveBackOpt inclluded
//
// Revision 2.43  2008-08-22 13:43:40+02  loeffler
// bugfix remove equipment from wpm_structordoprtyp_2 and add szOrderVersion
// assign this new struct to
// wpm_GetOperations2()
// wpm_GetOrderOperation2()
// wpm_GetOrderOperations2
//
// Revision 2.42  2008-08-21 10:28:52+02  loeffler
// New Functions:
// wpm_GetAssSingleUnits2()
// wpm_DisassSingleUnits()
// expant wpm_STRUCTORDOPRTYP with production version
// New functions with this new struct:
// wpm_GetOrderInfo2()
// wpm_GetOperations2()
// wpm_GetOrderOperation2()
//
// Revision 2.41  2008-08-14 16:12:37+02  loeffler
// - Bugfix: wpm_free(Pointer) only if Pointer != NULL
//
// Revision 2.40  2008-08-12 12:15:04+02  loeffler
// Bugfix: wpm_GetDispatchUnits set recnum_max = 100000 and loopcounter t to long
//
// Revision 2.39  2008-07-28 16:37:02+02  bauerj
// - small bugfix in wpm_SetStationId
//
// Revision 2.38  2008-07-28 15:59:11+02  loeffler
// - no retry of API calls in case of network problems
// - output of modules version in error panel
// - finished of setting all inputparameters to const
// - new function wpm_SetStationId to replace the setting of STATION_ID
//
// Revision 2.37  2008-07-17 09:57:46+02  bauerj
// - only changed the order of some function prototypes to clearify the usage
//
// Revision 2.36  2008-07-14 09:27:40+02  bauerj
// - small change in unit extended fields
//
// Revision 2.35  2008-07-11 16:00:09+02  bauerj
// - used new feature for releasing semaphore when thread died that had locked it
// - changed all inputparameters to const
// - new functions to work with extended fields on unit level
// - bugfixing on wpm_GetUnitLoss
// - wpm_MoveMAMA can now extend the stock automatically
// - new function for retrieving units next operation(s)
// - new function wpm_GetReleaseDate to see the timestamp of releasing the module
//
// Revision 2.34  2008-06-17 14:37:46+02  loeffler
// solved Jira issues
// WP-116 Wrong sorting of operation in function wpm_GetOrderOperation
// WP-115 Functions wpm_UnitCreate and wpm_MoveWIP cause UnitId changes
//
// Revision 2.33  2008-06-06 10:13:11+02  loeffler
// New functions wpm_UnitSetFail and wpm_UnitSetScrap
//
// Revision 2.32  2008-04-23 14:19:42+02  rosskopf
// function AddWIPPartlist memcpy bug solved
//
// Revision 2.31  2008-04-22 10:50:01+02  bauerj
// Bugfixes of getting assembled units and disassembling.
// Small bugfixes due to some JIRA entries.
// New parameters FORCE_BEST and FORCE_HIGHER in FraMES.cfg to be used in wpm_GetSlotForUnit
//
// Revision 2.30  2008-04-17 10:23:48+02  loeffler
// save also version info
//
// Revision 2.29  2008-04-17 10:21:48+02  loeffler
// Bugfix: wpm_UnitCheckinNext with quantite error solved.
// Bugfix SetUnit with use of sReceipt_pos in case of already assembled unit. In this cas szReceipt_pos not longer needed.
//
// Revision 2.28  2008-02-21 10:21:03+01  loeffler
// - wpm_GetUnitCurrentOperation() generates no exception if I call it with a non-existing unit.
// - the szState no longer useable because internal function w_sub_info don't give back that info
//
// Revision 2.27  2008-02-15 13:23:03+01  loeffler
// - Possibility to split also fail subset (activate via FraMES.cfg setting)
// - check, if input parameter of wpm_GetOrderInfo is valid.
// - Improved functionality for multiple equipments in operation of orders
// - modification of select statement for searching units while cleaning up on startup
// (changes done by Juergen Bauer)
//
// Revision 2.26  2008-02-06 13:43:30+01  loeffler
// - enabling splitting of subsets with existing scrapped elements
// - bugfixing of function wpm_GetReceiptContainers
// - optimzed init sequence for startup procedure when loading dll
// - new parameter FORCE_NEXT within GetSlotForUnit to force a slot when possibilities are not unique
//
// Revision 2.25  2007-12-20 16:23:24+01  loeffler
// Bugfix:
// - memory leak
// - disbatch quantity consitters virtual quantities
//
// Revision 2.24  2007-12-18 10:24:03+01  loeffler
// <>
//
// Revision 2.23  2007-11-19 10:24:25+01  loeffler
// Bugfix Unit quantity for function update_positioncounter
//
// Revision 2.22  2007-11-07 09:28:26+01  loeffler
// <>
//
// Revision 2.21  2007-11-07 09:26:41+01  loeffler
// Bugfix: position set in assembled units
//
// Revision 2.20  2007-11-05 10:54:36+01  loeffler
// <>
//
// Revision 2.19  2007-11-05 10:51:13+01  loeffler
// <>
//
// Revision 2.18  2007-11-05 10:43:37+01  loeffler
// <>
//
// Revision 2.17  2007-10-30 10:42:53+01  loeffler
// New Function wpm_ChangeWipOrderQuantity ()
//
// Revision 2.16  2007-10-29 16:49:59+01  loeffler
// New Function wpm_DisassSingleUnit () for disassembling units and bring it back to MAMA or to SCRAP it
//
// Revision 2.15  2007-10-29 16:48:48+01  loeffler
// New Function wpm_DisassSingleUnit () for disassembling units and bring it back to MAMA or to SCRAP it
//
// Revision 2.14  2007-10-29 16:43:26+01  loeffler
// New Function wpm_DisassSingleUnit () for disassembling units and bring it back to MAMA or to SCRAP it
//
// Revision 2.13  2007-10-17 16:56:13+02  loeffler
// New Function wpm_GetUnitCurrentOperation
//
// Revision 2.12  2007-09-25 14:31:57+02  loeffler
// New function wpm_GetReceiptInfo2
// Bugfix wpm_GetSlotForUnit: Handling of loss units
//
// Revision 2.11  2007-08-30 12:22:16+02  loeffler
// <>
//
// Revision 2.10  2007-08-30 12:15:14+02  loeffler
// Small bugfix wpm_GetSlotForUnit in for loop over all slots
//
// Revision 2.9  2007-08-30 11:21:33+02  loeffler
// If unit is already in exiting slot take that slot in wpm_GetSlotForUnit.
// Bugfix GetSlotForUnit to handle more than one operation on same equipment.
// Bugfix GetLastOperation and GetOperations you get only relevant states (New user_contition).
// Bugfix CheckSemiSetup worked not correct if more then one SEMI unit assembled
//
// Revision 2.8  2007-07-23 13:27:08+02  loeffler
// <>
//
// Revision 2.7  2007-07-19 11:55:24+02  loeffler
// Bugfix new function wpm_GetOrderInfo
//
// Revision 2.6  2007-07-17 09:05:22+02  loeffler
// New Function interface for function wpm_GetOrderInfo
//
// Revision 2.5  2007-07-16 16:45:17+02  loeffler
// <>
//
// Revision 2.4  2007-07-10 15:23:38+02  loeffler
// New function wpm_UpdateBatchUnitData
// Update Data for Batch Unit LocationDetail, Status and expiry_date.
//
// Revision 2.3  2007-07-06 13:04:25+02  loeffler
// <>
//
// Revision 2.2  2007-06-28 16:05:33+02  rosskopf
// <>
//
// Revision 2.1  2007-06-28 12:03:40+02  rosskopf
// new major release 2
//
// Revision 1.94  2007-06-28 11:56:41+02  rosskopf
// <>
//
// Revision 1.93  2007-06-28 11:19:27+02  loeffler
// Replace wpm_GetJobPartlist in wpm_GetOrderPartlist
//
// Revision 1.92  2007-06-26 16:10:50+02  loeffler
// <>
//
// Revision 1.90  2007-06-21 15:04:46+02  loeffler
// sAssembCount = Verbau.lQuantity * Unit.lQuantity
//
// Revision 1.89  2007-06-18 17:33:14+02  rosskopf
// function ReleasePosition
// -> Unuse Position for all slots with references to the position in tPosRef
//
// Revision 1.88  2007-06-05 10:56:01+02  loeffler
// Bugfix assebly RAW material as SEMI
//
// Revision 1.87  2007-05-22 10:18:01+02  loeffler
// <>
//
// Revision 1.86  2007-05-22 10:16:36+02  loeffler
// <>
//
// Revision 1.85  2007-05-22 10:01:26+02  loeffler
// 5. WipMaMa new function to assembly SEMI and RAW material
// Both material types SEMI and RAW can be assembled as "single" units and "batch" units
// extern short __stdcall wpm_UseBatchUnit(short sSlot,char *szBatchUnitId);
// extern short __stdcall wpm_SetSingleUnit(short sSlot,char *szUnitIdType, char *szUnitId,char *szSetUnitIdType, char *szSetUnitId,long lQuantity);
//
// Revision 1.84  2007-04-20 15:37:15+02  loeffler
// <>
//
// Revision 1.83  2007-04-20 15:35:02+02  loeffler
// <>
//
// Revision 1.82  2007-04-20 15:32:44+02  loeffler
// Remove all caps_free() from internal functions and add it to all wpm_ functions
//
// Revision 1.81  2007-04-18 09:04:23+02  loeffler
// Check if unit is in Lock state in function wpm_GetSlotFor Unit
// In that case the unit don't get a slot
//
// Revision 1.80  2007-04-02 09:23:42+02  loeffler
// <>
//
// Revision 1.79  2007-04-02 09:16:54+02  loeffler
// Bugfix caps_free() in GetMaterialInfo incluted
// Bugfix wpm_RemoveAllUnits from Slot if more that one Unit
//
// Revision 1.78  2007-03-19 16:25:19+01  loeffler
// wpm_GetUnitOperations2(3) and wpm_GetUnitLastOperation2(3) bugfix if job not found because already in history
//
// Revision 1.77  2007-03-16 14:42:53+01  loeffler
// With new wpm_GetSlotForUnit
//
// Revision 1.76  2007-03-16 10:29:31+01  loeffler
// export user_command in wrapper function w_wrapper_sub_log_info
//
// Revision 1.75  2007-03-14 14:22:16+01  loeffler
// change state from move to moveback if user_comment = wpm_UnitMoveBack in function
// w_wrapper_sub_log_info
//
// Revision 1.74  2007-03-12 14:21:16+01  loeffler
// Delete usercondition befor enter function w_se_dp_info.
//
// Revision 1.73  2007-02-19 11:05:31+01  loeffler
// Error message if STATION_ID or STATION_GRP_ID > 20 characters
// CHeck UnitId UnitIdType for functions wpm_GetUnitOperations, wpm_GetUnitOperations2, wpm_GetUnitOperations3, wpm_GetUnitLastOperation, wpm_GetUnitLastOperation2, wpm_GetUnitLastOperation3
//
// Revision 1.72  2007-02-15 17:19:46+01  rosskopf
// errorhandling semaphore bug solved
//
// Revision 1.71  2007-01-25 13:03:24+01  rosskopf
// Windows NT support
//
// Revision 1.70  2007-01-09 11:17:02+01  loeffler
// <>
//
// Revision 1.69  2007-01-08 17:19:16+01  rosskopf
// thread save feature implemented
// ErrorHandling.lib V0111
//
// Revision 1.68  2007-01-08 12:07:04+01  rosskopf
// <>
//
// Revision 1.67  2006-12-22 11:26:04+01  loeffler
// Bugfix full table scan eliminated for functions
// GetAssSEMIUnits
// UnitLoss
// UnitSplitAll
// UnitSplit
// UnitMerge
// MoveMaMa
// LockUnit
// UnLockUnit
// UnitCheckinNext
//
// Revision 1.66  2006-12-15 10:12:34+01  loeffler
// <>
//
// Revision 1.64  2006-12-13 13:10:15+01  loeffler
// Include semaphore in for threadsave functioncalls in Errorhandling
//
// Revision 1.63  2006-11-23 09:20:44+01  loeffler
// WPM_GetSlotForUnit running also if unit is already check in and mode3repaetcounter = none
//
// Revision 1.62  2006-11-17 10:02:35+01  loeffler
// <>
//
// Revision 1.61  2006-11-17 10:00:29+01  loeffler
// wpm_GetUnitLastOperation now working with w_sub_log_info and w_get_pstep_info function to get information about last operation for unit
//
// Revision 1.60  2006-11-16 11:11:24+01  loeffler
// Bug WP-25 solved
//
// Revision 1.59  2006-11-10 15:04:10+01  loeffler
// Bugfix wpm_GetUnitLoss
//
// Revision 1.58  2006-11-03 14:20:14+01  loeffler
// Bugfix in wpm_UnitLoss. Now you can scrap pass units and fail units also subunits in subset can now be scraped
//
// Revision 1.57  2006-11-02 16:18:51+01  loeffler
// Compile it with CVI 6
//
// Revision 1.56  2006-10-30 16:42:50+01  loeffler
// <>
//
// Revision 1.55  2006-10-30 16:39:54+01  loeffler
// <>
//
// Revision 1.54  2006-10-30 16:25:12+01  loeffler
// Set wpm_SetWipSlot in function wpm_UnitLoss and wpm_UnitLossSAP.
//
// Revision 1.53  2006-10-16 13:36:47+02  rosskopf
// <>
//
// Revision 1.52  2006-10-16 13:31:39+02  loeffler
// Bugfix GetSlotForUnit
//
// Revision 1.51  2006-10-13 11:57:24+02  loeffler
// New function wpm_UnitCheckinNext
// Now supress wpm_Init for nt support with an frames.cfg entry AUTO_INIT = 0 in [GLOBAL] section.
//
// Revision 1.50  2006-09-25 10:25:29+02  rosskopf
// <>
//
// Revision 1.49  2006-09-11 18:07:58+02  rosskopf
// Bug GetOperations wrong dispatch quantity solved
//
// Revision 1.48  2006-09-01 11:28:34+02  loeffler
// Modified wpm_GetOrderInfo() with error messages and change structure wpm_structoperationinfotyp
//
// Revision 1.47  2006-08-31 14:39:54+02  loeffler
// new Function wpm_GetOrderInfo incluted which show an complet routingplan
//
// Revision 1.46  2006-08-23 09:50:50+02  loeffler
// Bugfix wpm_RefreshWipMamaCFG now with functionality
// Bugfix wpm_CheckinUndo() now also for operations with more than one Fail succesors possible.
//
// Revision 1.45  2006-08-22 08:29:44+02  loeffler
// Adding new function  wpm_RefreshWipMamaCfg(void) also in WipMama.h
//
// Revision 1.44  2006-08-17 13:40:10+02  loeffler
// <>
//
// Revision 1.43  2006-08-17 13:38:06+02  loeffler
// Bugfix 2 X GetSlotforUnit functions now also for second unit
// Bugfix MoveBack with wrong equipment
// wpm_GetNextOperations2 with equipment in wpm_structoprtyp2
// wpm_GetUnitOperations2 with equipment in wpm_structunitoprtyp2
// wpm_GetUnitLastOperation2 with equipment in wpm_structunitoprtyp2
//
// Revision 1.42  2006-08-07 15:59:05+02  loeffler
// New function for getting lock status and lock text for units
// New function for rereading of WipMama section in FraMES.CFG
// Bugfix UnitCheckinUndo increases now the correct counter
// Bugfix wpm_UnitMoveBack now with correct equipment
//
// Revision 1.41  2006-04-24 14:32:55+02  loeffler
// Error in wpm_UnitGetLastID solved (spellingerror of unit_id_in_type
//
// Revision 1.40  2006-04-20 10:18:09+02  loeffler
// Error if STATION_NAME is only in modul section eleminated (Backward compatibility)
//
// Revision 1.39  2006-04-11 14:38:23+02  loeffler
// <>
//
// Revision 1.38  2006-04-11 07:28:05+02  loeffler
// Include glo_LogFatalEnd function if exit applikation abnormaly
//
// Revision 1.37  2006-03-31 09:02:43+02  loeffler
// <>
//
// Revision 1.36  2006-03-29 11:41:10+02  loeffler
// 3 new functions with wpm_structordoprtyp_2 in parameter
// 2 new functions for locking and unlocking units
// New errorhandling with modified Errorwindow and locking also in file
//
// Revision 1.35  2006-03-23 12:37:02+01  loeffler
// <>
//
// Revision 1.34  2006-03-23 11:41:14+01  loeffler
// Include logging mechanism to each wpm_ function
//
// Revision 1.33  2006-03-20 16:16:54+01  loeffler
// Eliminate all wpm_functionencalls from wpm_functions
//
// Revision 1.32  2006-02-16 15:11:46+01  loeffler
// <>
//
// Revision 1.31  2006-02-15 14:03:07+01  loeffler
// Set maxdispatch counter to 10 000.
//
// Revision 1.30  2006-01-13 08:30:57+01  loeffler
// wpm_GetUnitLoss  option_control_flag[2] increased to option_control_flag[4]
//
// Revision 1.29  2006-01-10 15:10:34+01  loeffler
// wpm_GetDispatchUnits result is now sorted
//
// Revision 1.28  2006-01-10 14:29:42+01  loeffler
// <>
//
// Revision 1.27  2006-01-02 15:48:42+01  rosskopf
// FRAMES.CFG new parameter: CHECK_RAW_PARTLIST=NO
// do not check partlist for RAW material
//
// Revision 1.26  2005-12-22 15:46:01+01  rosskopf
// change FraMES.cfg parameter:
// AUTO_EXTENT_RAW_PARTLIST=YES instead of
// CHECK_RAW_PARTLIST=NO
//
// Revision 1.25  2005-12-22 15:19:45+01  rosskopf
// logon panels at startup implemented
//
// Revision 1.24  2005-12-22 14:38:18+01  rosskopf
// new function: wpm_GetDispatchUnits
// functions: wpm_GetOperations wpm_GetOrderOperations: dipatch quantity implemented
// new FraMES.cfg parameter:
// SUPRESS_ZERO_DISPATCH_JOBS=NO do not display orders with zero dispatch quantity
//
// Revision 1.23  2005-12-20 17:16:41+01  rosskopf
// bug wpm_UnitFinish with already checked in unit in wip solved
//
// Revision 1.22  2005-12-20 14:48:35+01  rosskopf
// new function:
// wpm_AddWIPPartlist   // add parts to partlist definition
// wpm_UnitMoveBack   // move back to any former operation
// new FraMES.cfg parameter
// IMMEDIATE_RAW_UPDATE=YES  // Update MaMa and Wip immediate after FraMES function
// CHECK_RAW_PARTLIST=NO   // Do not Check partlist -> automatic partlist extension
//
// Revision 1.21  2005-10-31 11:47:17+01  loeffler
// Change wpm_RemoveMAMA to wpm_DeleteUnitInMAMA
//
// Revision 1.20  2005-10-28 11:47:32+02  loeffler
// New funtion wpm_RemoveMAMA incluted
//
// Revision 1.19  2005-10-13 14:00:19+02  loeffler
// With dll Version V0120
//
// Revision 1.18  2005-10-13 13:58:27+02  loeffler
// Bugfix replace calloc with caps_malloc (memory leak)
//
// Revision 1.17  2005-07-08 09:33:56+02  loeffler
// New Version V0119
//
// Revision 1.16  2005-06-28 14:16:07+02  loeffler
// <>
//
// Revision 1.15  2005-02-23 15:54:30+01  loeffler
// ReceiptPos included in WipMamaAssembly functions and in Pos Structure
//
// Revision 1.14  2005-02-18 14:53:46+01  loeffler
// New function wpm_GetMaterial implemented
//
// Revision 1.13  2005-02-18 11:11:17+01  loeffler
// New functions included for Modul Version, move units from MAMA to WIP,
//
// Revision 1.12  2004-11-25 15:13:28+01  ROSSKOPF
// new functions ResetError and GetErrorInformation in WipMaMa.h
//
// Revision 1.11  2004/08/06 11:09:01  ROSSKOPF
// equipment in column operator
//
// Revision 1.10  2004/05/26 11:44:24  loeffler
// New Function wpm_UnitLossSAP.
//
// Revision 1.9  2004-04-21 15:44:25+02  loeffler
// Solve some errors
//
// Revision 1.8  2004-03-08 15:18:28+01  loeffler
// Akt Quantity in frunction GetReceiptInfo added
//
// Revision 1.7  2004-03-05 14:57:25+01  loeffler
// wpm_GetReceiptInfo and wpm_ReceiptAdd added
// New global Var STATION_GRP addet
//
// Revision 1.6  2004-03-03 15:04:57+01  ROSSKOPF
// new functions CreateSEMIUnit CreateStock
//
// Revision 1.5  2003/11/14 09:21:53  LOEFFLER
// Remove some errors from betatest which are defined in an errordef.xls
//
// Revision 1.4  2003/11/11 13:12:45  LOEFFLER
// Removing Material Description from structure.
// Use Function GetMaterialInfo if you need Materialdescription.
//
// Revision 1.3  2003/10/30 14:21:55  LOEFFLER
// Bugfixes from Betatest.
//
// Revision 1.2  2003/10/27 13:05:02  loeffler
// *** empty log message ***
//
// Revision 1.1  2003-10-16 13:15:45+02  loeffler
// Tested Version wih Frames Controler
//
// Revision 1.0  2003-09-29 11:25:20+02  loeffler
// Init
//
// Revision 1.0  2003-07-30 14:01:43+02  loeffler
// started
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
#ifndef WIPMAMA
#define WIPMAMA
//////////////////////////////////////////////////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////////////////////////////////////////////////

#define WPM_VERSION    	   "$Name: V0218b9 $"
#define WPM_RELEASE_DATE   "$Date: 2009-06-16 13:42:29+02 $"

#define RAW   90
#define SEMI  91
#define ALL   92
#define FIN   93

#ifdef __cplusplus
		extern "C" {
#endif

#define szRAWUnitId szBatchUnitId   // old style RAWUnit replaced by BatchUnit

//////////////////////////////////////////////////////////////////////////////////////////////
// Typedef's
//////////////////////////////////////////////////////////////////////////////////////////////

typedef struct wpm_structtime {
    int tm_sec;		  // seconds (0-59)
    int tm_min;       // minutes (0-59)
    int tm_hour;      // hour    (0-23)
    int tm_mday;      // day of month (1-31)
    int tm_mon;       // month   (0-11; January=0)
    int tm_year;      // year    (current year - 1900)
    int tm_wday;      // day of week  (0-6; Sunday=0)
    int tm_yday;      // day of year  (0-365)
    int tm_isdst;     // daylight saving time
} wpm_STRUCTTIME;


typedef struct wpm_structlockdata {
	short sIsLocked;
	char szOperator[100];
	char szStatus[100];
	char szComment[300];
	long lLockDate;
	wpm_STRUCTTIME tLockDate;
} wpm_STRUCTLOCKDATA;

typedef struct wpm_structoperationinfotyp2
 {
	short sOperation;
	char szOperationDescription[100];
	char szEquipment[300];
	char cMandatory;
	char cSequencecheck;
	short sRepeatCounter;
	short sSuccYieldCount;
	struct wpm_structoperationinfotyp2 *pSuccYieldList;
	short sSuccFailCount;
	struct wpm_structoperationinfotyp2 *pSuccFailList;
 } wpm_STRUCTOPERATIONINFOTYP2;

typedef struct wpm_structoperationinfotyp_alt
 {
	short sOperation;
	char szOperationDescription[100];
	char szEquipment[300];
	short sYieldOperationCount;
	short sYieldNextOperation[20];
	char szYieldOperationDescription[100][20];
	char szYieldEquipment[100][20];
	char cYieldMandatory[20];
	short sFailOperationCount;
	short sFailNextOperation[20];
	char szFailOperationDescription[100][20];
	char szFailEquipment[100][20];
 } wpm_STRUCTOPERATIONINFOTYP;

typedef struct wpm_structmaterialtyp
 {
	char szNumber[100];
	char szName[100];
	char szDescription[100];
	char szGroup[100];
	char szType[100];
	char szStatus[100];
 } wpm_STRUCTMATERIALTYP;

/* lQtyYield: should be exactly: lQty (this is the notified good qty of any transaction, which is not allways qty_yield!!!)
 *            for instance for transaction w_sub_pro_start, the qty is the good started qty, and this qty is located in qty_process_start
 *            (and not in qty_yield)
 * char szState[20];    this is exactly the transaction_type of last transaction
*/
typedef struct wpm_structunitoprtyp
 {
	short sOperation;
	char szOperationDescription[100];
	char szDateTime[20];
	char szState[20];
	long lQtyYield;
	long lQtyFail;
	long lQtyLoss;
 } wpm_STRUCTUNITOPRTYP;

typedef struct wpm_structunitoprtyp2
 {
	short sOperation;
	char szOperationDescription[100];
	char szDateTime[20];
	char szState[20];
	long lQtyYield;
	long lQtyFail;
	long lQtyLoss;
	char szEquipment[300];
 } wpm_STRUCTUNITOPRTYP2;

typedef struct wpm_structoprtyp
 {
	short sOperation;
	char szOperationDescription[100];
	long lQtyYield;
	long lQtyFail;
	long lQtyLoss;
 } wpm_STRUCTOPRTYP;

typedef struct wpm_structoprtyp2
 {
	short sOperation;
	char szOperationDescription[100];
	long lQtyYield;
	long lQtyFail;
	long lQtyLoss;
	char szEquipment[300];
 } wpm_STRUCTOPRTYP2;

typedef struct wpm_structordoprtyp
 {
	char szMaterial[101];
	char szOrder[101];
	short sOperation;
	char szOperationDescription[100];
	long lJobQuantity;
	long lQtyDispatch;
	long lQtyYield;
	long lQtyFail;
	long lQtyLoss;
	long lRuns;
	long lPassRuns;
	long lFailRuns;
	char szStartDateTime[20];
	char szEndDateTime[20];
 } wpm_STRUCTORDOPRTYP;

typedef struct wpm_structordoprtyp_2
 {
	char szMaterial[101];
	char szOrder[101];
	char szOrderVersion[101];
	short sOperation;
	char szOperationDescription[100];
	long lJobQuantity;
	long lQtyDispatch;
	long lQtyYield;
	long lQtyFail;
	long lQtyLoss;
	long lRuns;
	long lPassRuns;
	long lFailRuns;
	char szStartDateTime[20];
	char szEndDateTime[20];
 } wpm_STRUCTORDOPRTYP_2;

typedef struct wpm_structapptyp
 {
	char   szAppType[100];
	char   szAppName[100];
 } wpm_STRUCTAPPTYP;

typedef struct wpm_structpart
 {
	long   lQuantity;
	short  sRetrogradePos;
	long   lPartlistDefNr;
	char   szMaterial[100];
	char   szLocation[300];
 } wpm_STRUCTPART;

typedef struct wpm_structpart2
 {
	long   lQuantity;
	short  sRetrogradePos;
	long   lPartlistDefNr;
	char   szMaterial[100];
	char   szLocation[300];
	char   szType[100];
	char   cTreat;
 } wpm_STRUCTPART2;

typedef struct wpm_structdocument
 {
	char   szDocNo[100];
	char   szDocType[100];
	char   szDocPart[100];
	char   szDocVersion[100];
 } wpm_STRUCTDOCUMENT;

typedef struct wpm_structtestplan
 {
	char   szTestplanGroup[16];
	char   szTestplanName[16];
	short  sTestplanVersion;
 } wpm_STRUCTTESTPLAN;

typedef struct wpm_structposref
 {
	char   szPosId[100];
	char   szReceiptId[100];
	short  sReceiptPos;
	char   szBatchUnitId[100];
	char   szMaterial[100];
	long   lQuantity;
	long   lMAMAQuantity;
	long   lInitQuantity;
 } wpm_STRUCTPOSREF;

typedef struct wpm_structReceiptref
 {
	char   szReceiptId[100];
	short  sReceiptPos;
	char   szDescription[100];
	char   szMaterial[100];
	float  lQtyReceived;
	float  lQtyActual;
	float  lQtyIdentified;
 } wpm_STRUCTRECEIPTREF;

typedef struct wpm_structpartlistsetupref
 {
	char   szMaterial[100];
	char   szPosId[100];
	char   szRAWUnitId[100];
	char   boErrorFlag;
 } wpm_STRUCTPARTLISTSETUP;

typedef struct wpm_structunittyp
 {
	char   szUnitId[100];
	char   szUnitIdType[100];
	long   lQuantity;
	char   cState;              // 'C' Unit checked in
								// 'S' Unit started
								// 'E' Unit ended
								// 'F' Unit finished
								// 'D' Unit dispatched
								// 'X' Unit Scrapped
 } wpm_STRUCTUNITTYP;

typedef struct wpm_structunittyp_2
 {
	char   szUnitId[100];
	char   szUnitIdType[100];
	float  fQuantity;
	char   szGoodsReceiptNr[100];
	char   szGoodsReceiptPos[100];
	char   szMaterialNr[100];
	char   szMaterialDescription[100];
 } wpm_STRUCTUNITTYP_2;

typedef struct wpm_structreceiptcont
 {
	// receiptinfo
	char   szReceiptId[100];
	short  sReceiptPos;
	// containerdata
	char   szContIdType[100];
	char   szContId[100];
	char   szLocationDetail[100];
	long   lQtyInitial;
	long   lQtyActual;
	char   szStatus[100];
	char   szOperator[100];
	char   szConsumerInfo[100];
	char   szReservationId[100];
	long   lQtyReserved;
	wpm_STRUCTTIME tReserved;
	wpm_STRUCTTIME tExpiryDate;
	wpm_STRUCTTIME tCreated;
	wpm_STRUCTTIME tUpdated;
	char   szUserComment[300];
 } wpm_STRUCTRECEIPTCONT;

typedef struct wpm_structmatdefinfo
 {
	// New Material Definition Info in syntactical right format. Sorry for that complication
	char szBaseId[10];
	long lMatDefId;
	char szNumber[100];
	char szName[100];
	char szType[100];
	char szGroup[100];
	char szRevision[100];
	char szVersion[100];
	char szUnit[100];
	char szDescription[100];
	char szStatus[100];
	char szOperatior[100];
	char szComment[300];
	char szPartGroup[10][100];
 } wpm_STRUCTMATDEFINFO;
 
/*typedef struct wpm_structmatdefinfo
 {
	// Material Definition Info
	char base_id[10];
	long mat_def_id;
	char part_number[100];
	char part_name[100];
	char part_type[100];
	char part_group[100];
	char part_revision[100];
	char part_version[100];
	char part_unit[100];
	char descr_short[100];
	char status[100];
	char operator[100];
	char user_comment[300];
	char part_group_arr[10][100];
 } wpm_STRUCTMATDEFINFO;
*/

typedef struct wpm_structcontcreate
 {
	char   szContId[100];
	char   szLocationDetail[100];
	long   lQtyInitial;
	char   szStatus[100];
	wpm_STRUCTTIME tExpiryDate;
 } wpm_STRUCTCONTCREATE;

typedef enum 
 {
	WPM_Undefined,
	WPM_Flag,
	WPM_BinFlag64,
	WPM_Long,
	WPM_Text,
	WPM_Date
 } wpm_EXTFIELDTYPE;

typedef enum 
 {
	WPM_StateDontCare = -2,
	WPM_StateUnknown,
	WPM_StateOff,
	WPM_StateOn
 } wpm_FLAGSTATE;

typedef struct wpm_structextfield
 {
	long lExtFieldIdx;
	char szExtField[100];
	wpm_STRUCTTIME tUpdated;
	wpm_EXTFIELDTYPE tType;
 } wpm_STRUCTEXTFIELD;
 
typedef struct wpm_structMaMaPartInfo
 {
    char  szReceiptId[100];
    short sReceiptPos;
    char  szLocationDetail[100];
    char  szStatus[100];
    char  szOperator[100];
    char  szConsumerInfo[100];
    char  szReservationId[100];
    char  szUserComment[300];
    float fQtyReserved;
    float fQtyInitial;
    float fQtyActual;
    long  lCreated;
    long  lUpdated;
    long  lReserved;
    long  lExpiryDate;
    wpm_STRUCTTIME tCreated;
    wpm_STRUCTTIME tUpdated;
    wpm_STRUCTTIME tReserved;
    wpm_STRUCTTIME tExpiryDate;
 } wpm_STRUCTMAMAPARTINFO;
 
//
//   Error Functions
//

#ifndef wpm_ShowError
#define wpm_ShowError(DUMMY) wpm_ShowErrorXY(0,0,__FILE__,__LINE__)
#endif
extern char *__stdcall wpm_GetVersion(void);
extern char *__stdcall wpm_GetReleaseDate(void);

extern short __stdcall wpm_GetErrorCode(void);
extern char *__stdcall wpm_GetErrorText(const short sErrorCode);
extern short __stdcall wpm_ShowErrorXY(const int siTop, const int siLeft, const char *szFile, const short ssLine);
extern short __stdcall wpm_GetErrorInformation(char **szErrorText, const char **szFunctionName, char **szErrorHint, char **szCapsError);

extern char *__stdcall wpm_GetCfg(const char *szKey, const short boShowError);
extern short __stdcall wpm_Init(void);
extern short __stdcall wpm_RefreshWipMamaCfg(void);
extern short __stdcall wpm_DisableFreeMemory(void);

extern short __stdcall wpm_SetStationId(const char *szStationId);
extern short __stdcall wpm_GetStationGrpId(char *szStationGrp, char *szStationId);

//
//   MamaInfo Functions
//
extern short __stdcall wpm_GetReceiptInfoForMat (const char *szMaterial, const char cQReceived, const char cQIdentified, const char cQActual, long *lMaxRec, wpm_STRUCTRECEIPTREF **pReceipt );
extern short __stdcall wpm_GetReceiptInfo(const char *szReceiptId, const short sReceiptPos, char *szMaterial, char *szDescription,long *lQuantity, long *lQuantityActual);
extern short __stdcall wpm_GetReceiptInfo2(const char *szReceiptId, const short sReceiptPos, char *szMaterial, char *szDescription, long *lQtyReceived, long *lQtyActual, long *lQtyIdentified);
extern long __stdcall wpm_GetReceiptContainers(const char *szReceiptId, const short sReceiptPos, wpm_STRUCTRECEIPTCONT **pContainers);

//
//  MaMa material definition handling
//
extern short __stdcall wpm_CreateMatDef (const char *szMaterialNo, const char *szDescription, const char *szTypes, 
								  const char *szProductfamiliy, const char *szUnit, const char *szRevision, 
								  const char *szVersion, const char *szDescriptionShort, const char *szStatus,
								  const char *szOperator);
extern short __stdcall wpm_GetMatDefInfo (const char *szMaterialNo, wpm_STRUCTMATDEFINFO *pMatDef);

//
//   WipMamaAssemb Functions
//
extern short __stdcall wpm_CreateReceipt(const char *szMaterial, const char *szReceiptId, const short sReceiptPos, const char *szDescription, const long lQuantity);
extern short __stdcall wpm_IncreaseReceipt(const char *szReceiptId, const short sReceiptPos, const long lAddQuantity);

extern short __stdcall wpm_CreateSingleUnit(const char *szMaterial, const char *szReceiptId, const char *szUnitIdType, const char *szUnitId, const long lQuantity);
/* old style */ extern short __stdcall wpm_CreateSEMIUnit(const char *szMaterial, const char *szReceiptId, const char *szUnitIdType, const char *szUnitId, const long lQuantity);

extern short __stdcall wpm_CreateBatchUnit(const char *szMaterial, const char *szReceiptId, const short sReceiptPos, const char *szBatchUnitId, const long slQuantity);
/* old style */ extern short __stdcall wpm_CreateRAWUnit(const char *szMaterial, const char *szReceiptId, const short sReceiptPos, const char *szRAWUnitId, const long slQuantity);
extern short __stdcall wpm_CreateBatchUnits(const char *szReceiptId, const short sReceiptPos, const long lCount, const wpm_STRUCTCONTCREATE *pContainer);

extern short __stdcall wpm_LockBatchUnit(const char *szBatchUnitId, const char *szComment);
extern short __stdcall wpm_UnlockBatchUnit(const char *szBatchUnitId, const char *szComment);

extern short __stdcall wpm_GetBatchUnitInfo(const char *szBatchUnitId, char *szReceiptId, short *sReceiptPos, char *szMaterial, long *lQuantity, long *lInitQuantity);
/* old style */ extern short __stdcall wpm_GetRAWUnitInfo(const char *szRAWUnitId, char *szReceiptId, short *sReceiptPos, char *szMaterial, long *lQuantity, long *lInitQuantity);

extern short __stdcall wpm_UpdateBatchUnitData(const char *szBatchUnitId, const char *szLocationDetail, const char *szStatus, const wpm_STRUCTTIME tExpiryDate);

extern short __stdcall wpm_SetPosition(char *szPosId, const char *szBatchUnitId);
extern short __stdcall wpm_SetPositions(const short sCount, char (*szPosId)[100], const char (*szBatchUnitId)[100]);
extern short __stdcall wpm_SetPosition2(char *szPosId, const char *szBatchUnitId, const short boReservation);
extern short __stdcall wpm_SetPositions2(const short sCount, const short boReservation, char (*szPosId)[100], const char (*szBatchUnitId)[100]);
extern short __stdcall wpm_ReleasePosition(const char *szPosId);
extern short __stdcall wpm_ReleasePositions(const short sCount, const char (*szPosId)[100]);
extern short __stdcall wpm_GetPosition(const char *szPosId, wpm_STRUCTPOSREF **pPos);
extern short __stdcall wpm_GetAllPositions(wpm_STRUCTPOSREF **pPos);
extern short __stdcall wpm_DecQtyPosition(const char *szPosId, const long lQuantity);
extern short __stdcall wpm_UseQtyPosition(const short sSlot, const char *szPosId, const long lQuantity);
extern short __stdcall wpm_ReleaseAllPositions(void);
extern short __stdcall wpm_UsePosition(const short sSlot, const char *szPosId);
extern short __stdcall wpm_UsePositions(const short sSlot, const short sCount, const char (*szPosId)[100]);
extern short __stdcall wpm_UnusePositions(const short sSlot, const short sCount, const char (*szPosId)[100]);
extern short __stdcall wpm_UnusePosition(const short sSlot, const char *szPosId);
extern short __stdcall wpm_UnuseAllPositions(const short sSlot);

extern short __stdcall wpm_UseBatchUnit(const short sSlot, const char *szBatchUnitId);
/* old style */ extern short __stdcall wpm_UseRAWUnit(const short sSlot, const char *szRAWUnitId);

extern short __stdcall wpm_UseBatchUnits(const short sSlot, const short sCount, const char (*szBatchUnitId)[100]);
/* old style */ extern short __stdcall wpm_UseRAWUnits(const short sSlot, const short sCount, const char (*szRAWUnitId)[100]);

extern short __stdcall wpm_UseQtyBatchUnit(const short sSlot, const char *szBatchUnitId, const long lQuantity);
/* old style */ extern short __stdcall wpm_UseQtyRAWUnit(const short sSlot, const char *szRAWUnitId, const long lQuantity);

extern short __stdcall wpm_DecQtyBatchUnit(const char *szBatchUnitId, const long lQuantity);
/* old style */ extern short __stdcall wpm_DecQtyRAWUnit(const char *szRAWUnitId, const long lQuantity);

extern short __stdcall wpm_ReleaseBatchUnit(const short sSlot, const char *szBatchUnitId);
/* old style */ extern short __stdcall wpm_ReleaseRAWUnit(const short sSlot, const char *szRAWUnitId);

extern short __stdcall wpm_ReleaseBatchUnits(const short sSlot, const short sCount, const char (*szBatchUnitId)[100]);
/* old style */ extern short __stdcall wpm_ReleaseRAWUnits(const short sSlot, const short sCount, const char (*szRAWUnitId)[100]);

extern short __stdcall wpm_ReleaseAllBatchUnits(const short sSlot);
/* old style */ extern short __stdcall wpm_ReleaseAllRAWUnits(const short sSlot);

extern short __stdcall wpm_UpdateBatchUnits(const short sSlot);
/* old style */ extern short __stdcall wpm_UpdateRAWUnits(const short sSlot);

extern short __stdcall wpm_UpdateAllBatchUnits(void);
/* old style */ extern short __stdcall wpm_UpdateAllRAWUnits(void);

extern short __stdcall wpm_SetSingleUnit(const short sSlot, const char *szUnitIdType, const char *szUnitId, const char *szSetUnitIdType, const char *szSetUnitId, const long lQuantity);
/* old style */ extern short __stdcall wpm_SetSEMIUnit(const short sSlot, const char *szUnitIdType, const char *szUnitId, const char *szSEMIUnitIdType, const char *szSEMIUnitId, const long lSEMIQuantity);

extern short __stdcall wpm_SetSingleUnits(const short sSlot, const char *szUnitIdType, const char *szUnitId, const short sCount, const char (*szSetUnitIdType)[100], const char (*szSetUnitId)[100], const long *lQuantity);
/* old style */ extern short __stdcall wpm_SetSEMIUnits(const short sSlot, const char *szUnitIdType, const char *szUnitId, const short sCount, const char (*szSEMIUnitIdType)[100], const char (*szSEMIUnitId)[100], const long *lSEMIQuantity);

extern short __stdcall wpm_CheckPartlistSetup(const short sSlot);
extern short __stdcall wpm_GetPartlistSetup(const short sSlot, wpm_STRUCTPARTLISTSETUP **pPartList);
extern short __stdcall wpm_AddWIPPartlist(const short sSlot, const short sCntPart, const wpm_STRUCTPART *pPart);

extern short __stdcall wpm_GetAssSingleUnits(const char *szUnitIdType, const char *szUnitId, const long lMaxCount, struct wpm_structunittyp **pUnitTypeList);
extern short __stdcall wpm_GetAssSingleUnits2(const char *szUnitIdType, const char *szUnitId, const long lMaxCount, struct wpm_structunittyp_2 **pUnitTypeList);
/* old style */ extern short __stdcall wpm_GetAssSEMIUnits(const char *szUnitIdType, const char *szUnitId, const long lMaxCount, struct wpm_structunittyp **pUnitTypeList);
extern short __stdcall wpm_GetUnitsByAssembly(const char *szPartIdType, const char *szPartId, const long lMaxCount, struct wpm_structunittyp **pUnitTypeList);

extern short __stdcall wpm_DisassSingleUnit (const char *szUnitIdType, const char *szUnitId, const char *szDisUnitIdType, const char *szDisUnitId, const long lQuantity, const short sScrap, const char *szComment);
extern short __stdcall wpm_DisassSingleUnits (const char *szUnitIdType,const char *szUnitId, const short sCount, const struct wpm_structunittyp *pUnitTypeList, const char *szComment);

extern short __stdcall wpm_GetPartInfo(const char *szPartIdType, const char *szPartId, struct wpm_structMaMaPartInfo *pMaMaPartInfo);
extern short __stdcall wpm_GetPartInfo2(const char *szPartIdType, const char *szPartId, struct wpm_structMaMaPartInfo *pMaMaPartInfo, struct wpm_structlockdata *pLockData);

//
//   WipMamaInfo Functions
//
extern short __stdcall wpm_GetOrderInfo(const char *szOrder, char *szMaterial, struct wpm_structoperationinfotyp2 **pOperationInfoList);
extern short __stdcall wpm_GetOrderInfo2(const char *szOrder, char *szOrderVersion, char *cOrderType, char *szMaterial, struct wpm_structoperationinfotyp2 **pOperationInfoList);
extern short __stdcall wpm_GetMaterialInfo(const char *szMaterial, struct wpm_structmaterialtyp *pMaterialInfo);
extern short __stdcall wpm_GetOrderOperations(struct wpm_structordoprtyp **pOperationList);
extern short __stdcall wpm_GetOperations(const char *szOrder, struct wpm_structordoprtyp **pOperationList);
extern short __stdcall wpm_GetOrderOperation(const char *szOrder, const short sOperation, struct wpm_structordoprtyp **pOperationList);
extern short __stdcall wpm_GetOrderOperations2(struct wpm_structordoprtyp_2 **pOperationList);
extern short __stdcall wpm_GetOperations2(const char *szOrder, struct wpm_structordoprtyp_2 **pOperationList);
extern short __stdcall wpm_GetOrderOperation2(const char *szOrder, const short sOperation, struct wpm_structordoprtyp_2 **pOperationList);

extern short __stdcall wpm_GetUnitLoss(const char *szUnitIdType, const char *szUnitId, char *szMaterial, char *szOrder, long *lLossQuantity);
extern short __stdcall wpm_GetUnitInfo(const char *szUnitIdType, const char *szUnitId, char *szMaterial, char *szOrder, char *cLocation, long *lQuantity);

//  wpm_structunitoprtyp old style without equipment
extern short __stdcall wpm_GetUnitOperations(const char *szUnitIdType, const char *szUnitId, struct wpm_structunitoprtyp **pOperationList);
extern short __stdcall wpm_GetUnitLastOperation(const char *szUnitIdType, const char *szUnitId, struct wpm_structunitoprtyp *pOperationList);
//  wpm_structunitoprtyp2 new style with equipment
extern short __stdcall wpm_GetUnitOperations2(const char *szUnitIdType, const char *szUnitId, struct wpm_structunitoprtyp2 **pOperationList);
extern short __stdcall wpm_GetUnitLastOperation2(const char *szUnitIdType, const char *szUnitId, struct wpm_structunitoprtyp2 *pOperationList);
//  Now with recnum_max where you can say how many operations you maximal want
extern short __stdcall wpm_GetUnitOperations3(const char *szUnitIdType, const char *szUnitId, const long recnum_max, struct wpm_structunitoprtyp2 **pOperationList);
extern short __stdcall wpm_GetUnitLastOperation3(const char *szUnitIdType, const char *szUnitId, const long recnum_max, struct wpm_structunitoprtyp2 *pOperationList);

extern short __stdcall wpm_GetUnitCurrentOperation(const char *szUnitIdType, const char *szUnitId, struct wpm_structunitoprtyp2 *pOperationList);
extern short __stdcall wpm_GetUnitNextOperations(const char *szUnitIdType, const char *szUnitId, char *szMaterial, char *szOrder, long *lQuantity, struct wpm_structoprtyp2 **pOperations);

extern short __stdcall wpm_GetDispatchUnits(const char *szOrder, const short sOperation, struct wpm_structunittyp **pUnits);
extern short __stdcall wpm_GetUnitLockInfo(const char *szUnitIdType, const char *szUnitId, short *sLocked, char *szComment);
extern short __stdcall wpm_GetOrderPartList(const char *szOrder, const short sOperation, struct wpm_structpart2 **pParts);

//
//   WipMamaSlot Functions
//
extern short __stdcall wpm_GetMaxSlot(void);
extern short __stdcall wpm_GetOldestSlot(void);
extern short __stdcall wpm_RemoveSlot(const short sSlot);
extern short __stdcall wpm_SlotSetUnitCnt(const short sSlot, const short sUnitCnt);
extern short __stdcall wpm_GetSlot(const char *szOrder, const short sOperation);
extern short __stdcall wpm_GetSlotInfo(const short sSlot, char *szOrder, short *sOperation);
extern short __stdcall wpm_GetSlotMaterial(const short sSlot, char *szMaterial);
extern short __stdcall wpm_SetSlot(short *sSlot, const char *szOrder, const short sOperation);
extern short __stdcall wpm_GetSlotForUnit(const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_GetPartListRAW(const short sSlot, struct wpm_structpart **pRAWParts);
extern short __stdcall wpm_GetPartListSEMI(const short sSlot, struct wpm_structpart **pSEMIParts);
extern short __stdcall wpm_GetPartList(const short sSlot, struct wpm_structpart2 **pParts);
extern short __stdcall wpm_GetDocumentRefs(const short sSlot, struct wpm_structdocument **pDocuments);
extern short __stdcall wpm_GetTestplanRefs(const short sSlot, struct wpm_structtestplan **pTestPlans);
extern short __stdcall wpm_GetAppRef(const short sSlot, struct wpm_structapptyp **pApplication);
//  wpm_structoprtyp old style without equipment
extern short __stdcall wpm_GetNextOperations(const short sSlot, const char cPassFail, struct wpm_structoprtyp **pOperations);
//  wpm_structoprtyp2 new style with equipment
extern short __stdcall wpm_GetNextOperations2(const short sSlot, const char cPassFail, struct wpm_structoprtyp2 **pOperations);
extern short __stdcall wpm_GetSlotUnits(const short sSlot, short *sQuantity, struct wpm_structunittyp **pUnitsList);
extern short __stdcall wpm_IsSlotGoldenSample(const short sSlot, short *sGSOrder);

//   WipMamaTransaction Functions
//
extern short __stdcall wpm_UnitCreate(const short sSlot, const char *szUnitIdType, const char *szUnitId, const long lQuantity);
extern short __stdcall wpm_UnitCheckin(const short sSlot, const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_UnitCheckinUndo(const short sSlot, const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_UnitLoss(const short sSlot, const char *szUnitIdType, const char *szUnitId, const long lQuantity);
extern short __stdcall wpm_UnitLossSAP(const short sSlot, const char *szUnitIdType, const char *szUnitId, const long lQuantity);
extern short __stdcall wpm_UnitChangeId(const char *szUnitIdTypeIn, const char *szUnitIdIn, const char *szUnitIdTypeOut, const char *szUnitIdOut);
extern short __stdcall wpm_UnitStart(const short sSlot, const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_UnitFinish(const short sSlot, const char *szUnitIdType, char const *szUnitId, const char cPassFail);
extern short __stdcall wpm_UnitValidate(const short sSlot, const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_UnitSplitAll(const char *szUnitIdType, const char *szUnitId, const short sCount, const char (*szUnitIdTypeSplit)[100], const char (*szUnitIdSplit)[100], const long *ulSplitQuantity);
extern short __stdcall wpm_UnitSplit(const char *szUnitIdType, const char *szUnitId, const char *szUnitIdTypeSplit, const char *szUnitIdSplit, const long ulSplitQuantity);
extern short __stdcall wpm_UnitMerge(const short ssCount, const char (*szUnitIdType)[100], const char (*szUnitId)[100], const char *szUnitIdTypeMerge, const char *szUnitIdMerge);
extern short __stdcall wpm_UnitGetLastIds(const char *szUnitIdType, const char *szUnitId, char (**szUnitIdTypeLast)[100], char (**szUnitIdLast)[100]);
extern short __stdcall wpm_UnitGetNextIds(const char *szUnitIdType, const char *szUnitId, char (**szUnitIdTypeNext)[100], char (**szUnitIdNext)[100]);
extern short __stdcall wpm_MoveMAMA(const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_MoveWIP(const short sSlot, const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_RemoveAllUnits(const short sSlot);
extern short __stdcall wpm_DeleteUnitInMAMA(const char *szUnitIdType, const char *szUnitId);
extern short __stdcall wpm_UnitMoveBack(const char *szUnitIdType, const char *szUnitId, const short sOperation);
extern short __stdcall wpm_UnitMoveBackOpt(const char *szUnitIdType, const char *szUnitId, const short sOperation);
extern short __stdcall wpm_LockUnit(const char *szUnitIdType, const char *szUnitId, const char *szComment);
extern short __stdcall wpm_UnlockUnit(const char *szUnitIdType, const char *szUnitId, const char *szComment);
extern short __stdcall wpm_UnitCheckinNext(const short sSlot, const char *szUnitIdType, const char *szUnitId, const short sOperation);

extern short __stdcall wpm_UnitSetScrap(const char *szUnitIdType, const char *szUnitId, const long lQuantity, const char *szComment);
extern short __stdcall wpm_UnitSetFail(const char *szUnitIdType, const char *szUnitId, const char *szComment);

extern short __stdcall wpm_ChangeWipOrderQuantity(const char *szOrder, const long lQty, long *lQtyTotal);

//  WIP unit level extended fields
//
extern short __stdcall wpm_SetUnitExtFieldText(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const char *szContent);
extern short __stdcall wpm_GetUnitExtFieldText(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, char *szContent);

extern short __stdcall wpm_SetUnitExtFieldFlag(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const wpm_FLAGSTATE iFlag, const char *szComment);
extern short __stdcall wpm_GetUnitExtFieldFlag(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, wpm_FLAGSTATE *iFlag, char *szComment);

extern short __stdcall wpm_SetUnitExtFieldLong(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const long lValue, const char *szComment);
extern short __stdcall wpm_IncUnitExtFieldLong(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const long lIncrement, const char *szComment, const short sAllowNeg);
extern short __stdcall wpm_GetUnitExtFieldLong(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, long *lValue, char *szComment);

extern short __stdcall wpm_ResetUnitExtFieldFlag64(const char *szUnitIdType, const char *szUnitId, const char *szFieldName);
extern short __stdcall wpm_SetUnitExtFieldFlag64All(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const wpm_FLAGSTATE *iFlagArray);
extern short __stdcall wpm_GetUnitExtFieldFlag64All(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, wpm_FLAGSTATE *iFlagArray);
extern short __stdcall wpm_SetUnitExtFieldFlag64(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const short sFlagNo, const wpm_FLAGSTATE iFlag);
extern short __stdcall wpm_GetUnitExtFieldFlag64(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const short sFlagNo, wpm_FLAGSTATE *iFlag);

extern short __stdcall wpm_SetUnitExtFieldDate(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, const wpm_STRUCTTIME DateTime, const char *szComment);
extern short __stdcall wpm_GetUnitExtFieldDate(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, wpm_STRUCTTIME *pDateTime, char *szComment);

extern short __stdcall wpm_GetUnitExtField(const char *szUnitIdType, const char *szUnitId, const char *szFieldName, struct wpm_structextfield *ExtendedField);
extern short __stdcall wpm_GetUnitExtFields(const char *szUnitIdType, const char *szUnitId, long *lExtFieldCnt, struct wpm_structextfield **pExtendedFields);

//  MaMa container level extended fields
//
extern short __stdcall wpm_SetPartExtFieldText(const char *szPartIdType, const char *szPartId, const char *szFieldName, const char *szContent);
extern short __stdcall wpm_GetPartExtFieldText(const char *szPartIdType, const char *szPartId, const char *szFieldName, char *szContent);

extern short __stdcall wpm_SetPartExtFieldFlag(const char *szPartIdType, const char *szPartId, const char *szFieldName, const wpm_FLAGSTATE iFlag, const char *szComment);
extern short __stdcall wpm_GetPartExtFieldFlag(const char *szPartIdType, const char *szPartId, const char *szFieldName, wpm_FLAGSTATE *iFlag, char *szComment);

extern short __stdcall wpm_SetPartExtFieldLong(const char *szPartIdType, const char *szPartId, const char *szFieldName, const long lValue, const char *szComment);
extern short __stdcall wpm_IncPartExtFieldLong(const char *szPartIdType, const char *szPartId, const char *szFieldName, const long lIncrement, const char *szComment, const short sAllowNeg);
extern short __stdcall wpm_GetPartExtFieldLong(const char *szPartIdType, const char *szPartId, const char *szFieldName, long *lValue, char *szComment);

extern short __stdcall wpm_ResetPartExtFieldFlag64(const char *szPartIdType, const char *szPartId, const char *szFieldName);
extern short __stdcall wpm_SetPartExtFieldFlag64All(const char *szPartIdType, const char *szPartId, const char *szFieldName, const wpm_FLAGSTATE *iFlagArray);
extern short __stdcall wpm_GetPartExtFieldFlag64All(const char *szPartIdType, const char *szPartId, const char *szFieldName, wpm_FLAGSTATE *iFlagArray);
extern short __stdcall wpm_SetPartExtFieldFlag64(const char *szPartIdType, const char *szPartId, const char *szFieldName, const short sFlagNo, const wpm_FLAGSTATE iFlag);
extern short __stdcall wpm_GetPartExtFieldFlag64(const char *szPartIdType, const char *szPartId, const char *szFieldName, const short sFlagNo, wpm_FLAGSTATE *iFlag);

extern short __stdcall wpm_SetPartExtFieldDate(const char *szPartIdType, const char *szPartId, const char *szFieldName, const wpm_STRUCTTIME DateTime, const char *szComment);
extern short __stdcall wpm_GetPartExtFieldDate(const char *szPartIdType, const char *szPartId, const char *szFieldName, wpm_STRUCTTIME *pDateTime, char *szComment);

extern short __stdcall wpm_GetPartExtField(const char *szPartIdType, const char *szPartId, const char *szFieldName, struct wpm_structextfield *pExtField);
extern short __stdcall wpm_GetPartExtFields(const char *szPartIdType, const char *szPartId, long *lExtFieldCnt, struct wpm_structextfield **pExtendedFields);

//  MaMa material definition level extended fields
//
extern short __stdcall wpm_SetMatExtFieldText(const char *szMaterialNo, const char *szFieldName, const char *szContent);
extern short __stdcall wpm_GetMatExtFieldText(const char *szMaterialNo, const char *szFieldName, char *szContent);

extern short __stdcall wpm_SetMatExtFieldLong(const char *szMaterialNo, const char *szFieldName, const long lValue, const char *szComment);
extern short __stdcall wpm_GetMatExtFieldLong(const char *szMaterialNo, const char *szFieldName, long *lValue, char *szComment);


#ifdef __cplusplus
		}
#endif

#endif
