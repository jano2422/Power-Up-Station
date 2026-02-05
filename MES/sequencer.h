//////////////////////////////////////////////////////////////////////////////////////////////
//
// FraMES - Framework MES
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// $Source: /app/develop/cvsroot/frames/evaprod/cvi/sequencer.h,v $
//
// $Revision: 2.23 $
// $Name: V0215b3_beta $
// $Author: rosskopf $
// $Date: 2009/08/24 14:35:05 $
//
// $State: Exp $
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Description:
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// $Log: sequencer.h,v $
// Revision 2.23  2009/08/24 14:35:05  rosskopf
// release lock of rosskopf (done by bauerj)
//
// Revision 2.22  2009/05/06 08:10:47  loeffler
// Change to new directory of errorhandling V0204b1
//
// Revision 2.21  2009-04-07 14:56:06+02  rosskopf
// Bug Jira EP-48 testrun failed after retest of a test previous failed test
//
// Revision 2.20  2009-03-13 13:35:15+01  rosskopf
// extend number of test steps per testplan to 20000
//
// Revision 2.19  2009-03-04 15:40:38+01  rosskopf
// function LoadTestPlan:
// additional check: attributive value <1 not allowed in characterisitc list
//
// Revision 2.18  2009-02-25 14:11:23+01  rosskopf
// GetTestResult possible if test run is already ended
//
// Revision 2.17  2009-01-09 11:45:59+01  rosskopf
// evaprod.h missing functions eva_SetCAPSEVASlot and eva_SetCAPSPAASlot inserted
//
// Revision 2.16  2008-12-18 11:29:28+01  bauerj
// - now using global setting LOG_DEST from FraMES.cfg
//
// Revision 2.15  2008-10-01 13:13:00+02  bauerj
// - turning from bet to release
//
// Revision 2.14  2008-08-21 11:54:52+02  loeffler
// New function eva_GetDBTestPlanList() with new struct eva_STRUCTTESTPLAN
// new const PRPMAX = 500
//
// Revision 2.13  2008-07-28 15:05:44+02  bauerj
// - set all inputparameters to const
// - output of version and release date to logfile if logging is activated
// - output of version in error panel
// - no retry of API calls in case of network problems
//
// Revision 2.12  2008-06-13 16:34:48+02  rosskopf
// bug slow function load testplan with subdim solved
//
// Revision 2.11  2008-06-13 16:09:58+02  loeffler
// <>
//
// Revision 2.10  2008-04-25 15:13:46+02  rosskopf
// memory allocation bug in function TestPlanCloneTest sloved
//
// Revision 2.9  2008-04-03 17:57:29+02  rosskopf
// memory overflow bug in function eva_EndTestRun solved (error hint)
//
// Revision 2.8  2008-02-12 10:38:48+01  rosskopf
// build3 beta merged with improvements from V0201b6
//
// Revision 2.7  2007-12-07 13:51:52+01  rosskopf
// mpm inlcuded in RCS (missed)
//
// Revision 2.6  2007-11-22 17:24:44+01  rosskopf
// Sub dimension feature implemented
// extended testid implemented
// memory alloc test OK
//
// Revision 2.5  2007-09-25 09:12:47+02  rosskopf
// "General  Protection Fault" bug in case AUTO_INIT=0 solved
//
// Revision 2.4  2007-09-21 15:12:27+02  rosskopf
// MPM only one sample key bug solved. now two sample keys: Id and Idtype
//
// Revision 2.3  2007-07-24 14:12:41+02  rosskopf
// function eva_CloneTestPlanTest malloc bug
//
// Revision 2.2  2007-06-28 16:05:05+02  rosskopf
// <>
//
// Revision 2.1  2007-06-28 12:01:29+02  rosskopf
// new major release 2
//
// Revision 1.44  2007-06-18 14:26:07+02  rosskopf
// GetNextTest - retest only if test value is fail
//
// Revision 1.43  2007-02-27 16:55:51+01  loeffler
// Bugfix Semaphore: New external function in eva_init used for removing old testplans
//
// Revision 1.42  2007-02-26 10:01:37+01  loeffler
// Bug: 2 more glo_LogEnd if exit or return in Testrun.c
//
// Revision 1.41  2007-02-23 08:30:43+01  loeffler
// New errorhandling linkt with ProcessId in Semaphorename
//
// Revision 1.40  2007-02-20 11:46:19+01  loeffler
// One more glo_LogEnd in function eva_ShowTestPlans at the end.
//
// Revision 1.39  2007-02-19 14:37:47+01  loeffler
// glo_LogEnd in all eva_ functions incluted in each retval leave.
//
// Revision 1.38  2007-02-15 17:17:53+01  rosskopf
// errorhandling semaphore bug solved
//
// Revision 1.37  2007-01-25 12:58:19+01  rosskopf
// Windows NT support
// sequencer bug threadsave solved
//
// Revision 1.36  2007-01-09 11:24:19+01  loeffler
// <>
//
// Revision 1.35  2007-01-08 17:06:24+01  rosskopf
// thread save feature implemented
//
// Revision 1.34  2006-11-15 10:53:12+01  ROSSKOPF
// *** empty log message ***
//
// Revision 1.33  2006/10/27 11:46:10  rosskopf
// <>
//
// Revision 1.32  2006-10-16 14:14:31+02  rosskopf
// <>
//
// Revision 1.31  2006-10-11 09:34:03+02  rosskopf
// <>
//
// Revision 1.30  2006-10-11 09:06:15+02  rosskopf
// <>
//
// Revision 1.29  2006-10-10 16:46:50+02  rosskopf
// <>
//
// Revision 1.28  2006-10-10 16:36:46+02  rosskopf
// New Functions:
// eva_GetDBTestRunList
// eva_GetDBTextInfo2
// eva_GetDBTestRunValues2
// eva_GetDBTestRunTextValues2
// eva_GetDBBLOBInfo2
// eva_GetDBBLOB2
//
// Revision 1.27  2006-09-20 17:00:41+02  rosskopf
// <>
//
// Revision 1.26  2006-09-20 16:55:48+02  rosskopf
// <>
//
// Revision 1.25  2006-09-20 16:47:50+02  rosskopf
// bugfixes:
// retest counter for testruns correct counting
// update display bug for eva_TestPlanResetStatistic
// eva_SetMaterial no action if same as already set
// eva_SetOrderType no action if same as already set
// eva_SetTestProgramm no action if same as already set
// Remove display at exit invalid index solved
// new functions
// eva_SetCAPSEVASlot
// eva_SetCAPSPAASlot
//
// Revision 1.24  2006-08-22 11:01:16+02  rosskopf
// new function CloneTestplanTest
// new parameter values "A,R,L" for function SetOrderType
//
// Revision 1.23  2006-08-18 16:47:45+02  rosskopf
// new function CloneTestPlanTest
// new types SetOrderType A,R,L
//
// Revision 1.22  2006-07-31 14:24:08+02  rosskopf
// new datatypes for function eva_SetOrderType
// A - Analyse
// R - Repair
// L - Loss
// function eva_Init reinit without reconnect
//
// Revision 1.21  2006-06-01 11:44:32+02  rosskopf
// Release V0155
//
// Revision 1.20  2006-05-17 14:58:18+02  rosskopf
// new revision 1.54
//
// Revision 1.19  2006-05-17 10:20:36+02  loeffler
// bug in PAA function solved
// function: EndTestRun
// paa_connectcache
//
// Revision 1.18  2006-04-20 13:29:52+02  loeffler
// Error if STATION_NAME is only in modul section eleminated (Backward compatibility)
//
// Revision 1.17  2006-04-11 14:32:10+02  loeffler
// <>
//
// Revision 1.16  2006-04-11 14:26:44+02  loeffler
// <>
//
// Revision 1.15  2006-04-10 16:49:11+02  loeffler
// Version V0153 because V0152 was buggy
//
// Revision 1.14  2006-04-10 16:23:25+02  loeffler
// Include glo_LogFatalEnd function if exit application
//
// Revision 1.13  2006-03-29 11:44:59+02  loeffler
// New errorhandling with modified Errorwindow and locking also in file
//
// Revision 1.12  2006-03-24 08:55:04+01  rosskopf
// <>
//
// Revision 1.11  2006-02-15 09:52:32+01  rosskopf
// <>
//
// Revision 1.10  2006-02-15 09:38:21+01  rosskopf
// <>
//
// Revision 1.9  2006-02-14 16:18:54+01  rosskopf
// new functions
//
// Revision 1.8  2006-01-09 11:06:49+01  rosskopf
// add errorhint load characteristic list name
//
// Revision 1.7  2005-11-09 17:25:07+01  rosskopf
// first release with PAA integration
//
// Revision 1.6  2005-11-09 17:18:26+01  rosskopf
// <>
//
// Revision 1.5  2005-11-09 17:00:59+01  rosskopf
// <>
//
// Revision 1.4  2005-11-09 16:46:47+01  rosskopf
// PAA Integration
//
// Revision 1.3  2005-07-18 11:17:21+02  rosskopf
// <>
//
// Revision 1.2  2005-07-12 16:53:06+02  rosskopf
// <>
//
// Revision 1.1  2005-02-17 17:35:44+01  ROSSKOPF
// new Functions ClearTestRun GetVersion
// display bug windows too small at startup
//
// Revision 1.0  2003/10/27 12:33:40  rosskopf
// Initial revision
//
// Revision 1.1  2003/09/09 10:56:01  ROSSKOPF
// Initial revision
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef inc_sequencer
#define inc_sequencer 

//////////////////////////////////////////////////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////////////////////////////////////////////////

#define SEQ_SERIAL   1
#define SEQ_PARALLEL 2

#ifdef __cplusplus
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Typedef's
//////////////////////////////////////////////////////////////////////////////////////////////

#define SEQ_VERSION    "$Name: V0215b3_beta $"

//////////////////////////////////////////////////////////////////////////////////////////////
// Init Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern char* __stdcall seq_GetVersion(void);

extern short __stdcall seq_GetErrorInformation(char **szErrorText,const char **szFunctionName,char **szErrorHint,char **szCapsError);

extern short __stdcall seq_ShowErrorFL(char *szFile,short sLine);
extern short __stdcall seq_ShowErrorXY(int iTop,int iLeft, char *szFile,short sLine);

extern short __stdcall seq_Init(void);
extern short __stdcall seq_GetErrorCode(void);
extern char* __stdcall seq_GetErrorText(short sErrorCode);

#ifndef seq_ShowError
#define seq_ShowError(DUMMY) seq_ShowErrorFL(__FILE__,__LINE__)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////
// Sequencer Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall seq_SetApplication(short sSlot,void *pAppDllHndl);
extern short __stdcall seq_ReleaseApplication(short sSlot);
extern short __stdcall seq_Activate(short sSlot);
extern short __stdcall seq_Deactivate(short sSlot);
extern short __stdcall seq_Cycle(short sSlot);

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
    }
#endif

#endif
