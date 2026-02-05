//==============================================================================
//
// Title:       tools.c
// Purpose:     Compilation of re-usable functions
//
// Created on:  8/21/2009 at 8:29:52 AM by Clifford Marquez.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __tools_H__
#define __tools_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "cvidef.h"
#ifdef __cplusplus
    }
#endif
#endif  /* ndef __tools_H__ */
	
int dll_Retval;
	

//==============================================================================
//		Listed below are the return value for the mes functions
//==============================================================================

#define Master_Unit_dll                  1	  // DUT is master Unit
#define	MasterPassed_dll				 2	  // Master Unit Passed the Test
#define MasterFailed_dll				 3	  // Master Unit Failed the Test
#define UnitValidateOk_dll				 0	  // Unit is OK to be process
#define UnitValidateNOk_dll				 5	  // Unit is not OK to be process
#define UnitPassed_dll					 6	  // Production monitoring is update correctly, Unit passed
#define UnitFailed_dll					 7	  // Production unit failed
#define UnitLock_dll					 8	  // unit is lock in Wip
#define UnitCheckIn_dll					 9	  // unit is check-in, undergone A&R station
#define UnitAlreadyPass_dll				10    // unit already passed at the current station
#define UnitEvaProdPass                 11    // unit passed only at evaprod, but re-testable.
#define UnitBCError                     12    // unit BC error
#define Unit5xBCError                   14    // unit 5x BC error 
#define UnitTraceIDError                15    // unit Trace ID error 
#define UnitCMBlankError                16    // unit Current Material contain Blank value error    
#define IncorrectMatrialError           17    // Incorrect Material error 
#define MultipleTestedError             18    // Multiple Tested error    
#define NeedToTestMasterUnitError       19    // Need to Test Master Unit error      
#define UnitUnknown_dll				   -99    // unknown error


//==============================================================================
//		Listed below are the return value for the "GetShiftingTime" function
//==============================================================================
#define A_SHIFT_dll	 1
#define B_SHIFT_dll  2
#define C_SHIFT_dll  3
	
//==============================================================================
//		Listed below are the Possible Evaprod.dll Version can be used.
//==============================================================================

#define V0212b8_beta  1
#define V0232b3       2 

//==============================================================================
//		Listed below are the Typedef's used by the Tools.dll
//==============================================================================
typedef struct cb_unitcheck
 {
   char cbTestPlanGroup[15];
   char cbTestPlanName[15];
   short cbTestPlanVersion[100];
   char cbMaterial[21];
   char cbCurrentMaterial[21];
   char cbUnitId[100];
   char cbOrder[100]; 
   char cbOrderVersion[100];
   char cbOrderType[100];
   char cbLocation[100]; 
   char cbMasterUnitSN[100];
   char cbTraceId[100]; 
   char cbUnitIdOut[100];
   long cblQuantity;
   long cbChangeIdCnt;
   int  cbMasterUnitStatus;
   int  cbScanCtr;
   int  cbFailedCtr;
   int  cbMaxRetest;
   int  cbLockStatus;
   int  cbCheckinStatus; 
   int  cbTraceIdStatus;
   int  cbFirstRunFlag;
   int  cbStatusReturn;  
   int 	cbBay; 
 } cb_UNITCHECK;

typedef struct cb_unitidlock
 {
	int 	cbLockIdStatus;
	short 	cbLock_TestID;
	char	cbLock_Description[100];
 } cb_UNITIDLOCK;


//==============================================================================
// Constants
//==============================================================================  

#define	emptyBarcode	0
#define	validBarcode	4
#define	noECU			2
#define	TESTING			3
#define	LOADING_PLS		5
#define	TIMESFAIL3		6
#define	UNITLOCKED		7	
#define	QUIZZERLOCKED	8
#define	DISABLE			9    
#define READY			1
#define PASS			'P'
#define FAIL			'F'
#define	LOADING_DTS		10 
#define	LOADING_FLEXRAY 11

//==============================================================================
//		Listed below are the variables used by the Tools.dll
//==============================================================================


//==============================================================================
//		Listed below are the available functions included in the dll
//==============================================================================

extern int DLLEXPORT __stdcall  ReadIniFile (char sFile[20], char sSection[20], char sItem[20], char *retvalue);      
extern short DLLEXPORT __stdcall SetStatistics(int iStatus); 
extern short DLLEXPORT __stdcall FPYLog (char sDataBuffer[500]);
extern short DLLEXPORT __stdcall writedatatolog (char szFname[100], char sDataBuffer[500]);
extern short DLLEXPORT __stdcall WriteProtocol (int iType,char *PathFile,char *szSerialNo,char *szMatNum,char *szTestID,char *sDataBuffer);
extern short DLLEXPORT __stdcall ReadProtocol (char *PathFile, char *szSerialNo, long iTestID, float *retValue);
extern char*  DLLEXPORT  __stdcall GetCfg_dirValue(char szFileName[500],char *sectionName, char *szKey);
extern char*  DLLEXPORT  __stdcall GetCfg_Value(char szFileName[500],char *sectionName, char *szKey);
extern short DLLEXPORT __stdcall GetShiftingTime(void);
extern short DLLEXPORT __stdcall ErrorLog (char sDataBuffer[500]); 
extern short DLLEXPORT __stdcall TestStatusbarUpdate (int bay, double level); 
extern short DLLEXPORT __stdcall SetAndonStatus (int bay, char Result); 
extern int DLLEXPORT __stdcall status_init (void);
extern int DLLEXPORT __stdcall setStatus (int PANEL, int Action);
extern int DLLEXPORT __stdcall MsgPopup (char *heading, char *txtmsg);  

//with mes functions
extern short  DLLEXPORT  __stdcall mes_PreUnitValidate(char szSerialNumber[50]);
extern short  DLLEXPORT  __stdcall mes_PostUnitValidate(char szSerialNumber[50]);
extern  short DLLEXPORT  __stdcall Check_MPM_Locking(void);  
extern short DLLEXPORT __stdcall mes_UnitCheck(char dll_SN[50], void *Par);  
extern short DLLEXPORT __stdcall mes_UnitIdLockSet(char *cbgszUnitIdType, char *cbgszUnitId, char cbcResult, void *Par);
extern short DLLEXPORT __stdcall mes_UnitIdLockParaCheck(short cbsSlot,short cbsIdx,short cbsRun, char *cbszTestId, short cbTestIdParam, void *Par); 

