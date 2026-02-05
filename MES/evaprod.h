//////////////////////////////////////////////////////////////////////////////////////////////
//
// FraMES - Framework MES
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// $Source: /app/develop/cvsroot/frames/evaprod/cvi/EVAPROD.h,v $
//
// $Revision: 2.7 $
// $Name: V0215b3_beta $
// $Author: rosskopf $
// $Date: 2010/01/14 15:08:09 $
//
// $State: Exp $
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Description:
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// $Log: EVAPROD.h,v $
// Revision 2.7  2010/01/14 15:08:09  rosskopf
// 1. The function eva_GetDBTestRunList( ) and eva_GetDBTestRunList2  has now reverse ordering starting with the newest testrun. This is only running on server version V0332_01ff
// 2. New functions to support dynamic extension of characeristic lists
// eva_SetTestCharacteristicKey
// eva_GetTestCharacteristics()
// eva_GetCharacteristicGroups()
// eva_GetGroupCharacteristics(()
//
// Revision 2.6  2009/11/24 15:11:40  rosskopf
// V0215b1_alpha for Krisztian Nagy
//
// Revision 2.5  2009/10/07 09:39:09  bauerj
// Test
//
// Revision 2.4  2009/10/07 09:33:26  bauerj
// - added a blank at the end
//
// Revision 2.3  2009/10/07 09:24:38  loeffler
// Improvment increase testplan list maximum to 20000
//
// Revision 2.2  2009/10/05 14:53:43  bauerj
// - new functions that use correct structure with length (16 char 15+' \0') of testplangroup and testplanname
// - improvement of displaying testruns when having error values, +/-Inf, etc.
//
// Revision 2.1  2009/08/31 06:10:10  bauerj
// - new functions to handle dynamically the loglevel and logdestination
// - bugfix in reading config file FraMES.cfg: only the real value can be found
// - updated to new general FraMES internal library
// - switch to CVS version control
// - added Pulse functionality to send messages on differnet PULSE_LEVEL settings
// - check if the value of a teststep is +INF, -INF or NaN to avoid SQL errors when saving testrun data
//
// Revision 2.25  2009/08/24 14:16:13  rosskopf
// release lock of rosskopf (done by bauerj)
//
// Revision 2.24  2009/05/06 08:10:40  loeffler
// Change to new directory of errorhandling V0204b1
//
// Revision 2.23  2009-04-07 14:55:52+02  rosskopf
// Bug Jira EP-48 testrun failed after retest of a test previous failed test
//
// Revision 2.22  2009-03-13 13:35:01+01  rosskopf
// extend number of test steps per testplan to 20000
//
// Revision 2.21  2009-03-04 15:40:24+01  rosskopf
// function LoadTestPlan:
// additional check: attributive value <1 not allowed in characterisitc list
//
// Revision 2.20  2009-02-25 14:10:49+01  rosskopf
// GetTestResult possible if test run is already ended
//
// Revision 2.19  2009-01-09 11:45:46+01  rosskopf
// evaprod.h missing functions eva_SetCAPSEVASlot and eva_SetCAPSPAASlot inserted
//
// Revision 2.18  2008-12-18 11:24:26+01  bauerj
// - now using global setting LOG_DEST from FraMES.cfg
//
// Revision 2.17  2008-10-01 13:12:19+02  bauerj
// - turning from bet to release
//
// Revision 2.16  2008-08-21 11:54:39+02  loeffler
// New function eva_GetDBTestPlanList() with new struct eva_STRUCTTESTPLAN
// new const PRPMAX = 500
//
// Revision 2.15  2008-07-28 15:04:59+02  bauerj
// - set all inputparameters to const
// - output of version and release date to logfile if logging is activated
// - output of version in error panel
// - no retry of API calls in case of network problems
//
// Revision 2.14  2008-06-13 16:34:39+02  rosskopf
// bug slow function load testplan with subdim solved
//
// Revision 2.13  2008-06-13 16:09:50+02  loeffler
// <>
//
// Revision 2.12  2008-04-25 15:13:35+02  rosskopf
// memory allocation bug in function TestPlanCloneTest sloved
//
// Revision 2.11  2008-04-03 17:57:18+02  rosskopf
// memory overflow bug in function eva_EndTestRun solved (error hint)
//
// Revision 2.10  2008-02-12 10:38:36+01  rosskopf
// build3 beta merged with improvements from V0201b6
//
// Revision 2.9  2007-12-20 17:07:48+01  rosskopf
// <>
//
// Revision 2.8  2007-12-07 13:51:41+01  rosskopf
// mpm inlcuded in RCS (missed)
//
// Revision 2.7  2007-12-03 13:50:26+01  rosskopf
// <>
//
// Revision 2.6  2007-11-22 17:24:33+01  rosskopf
// Sub dimension feature implemented
// extended testid implemented
// memory alloc test OK
//
// Revision 2.5  2007-09-25 09:12:37+02  rosskopf
// "General  Protection Fault" bug in case AUTO_INIT=0 solved
//
// Revision 2.4  2007-09-21 15:12:17+02  rosskopf
// MPM only one sample key bug solved. now two sample keys: Id and Idtype
//
// Revision 2.3  2007-07-24 14:12:30+02  rosskopf
// function eva_CloneTestPlanTest malloc bug
//
// Revision 2.2  2007-06-28 16:04:55+02  rosskopf
// <>
//
// Revision 2.1  2007-06-28 12:01:16+02  rosskopf
// new major release 2
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef inc_evaprod
#define inc_evaprod

//////////////////////////////////////////////////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////////////////////////////////////////////////

#define EVA_VERSION    "$Name: V0215b3_beta $"
#define EVA_RELEASE_DATE   "$Date: 2010/01/14 15:08:09 $"
#define ERROR_VALUE     1.7E20
#define DONT_CARE       2.51E20

#ifdef __cplusplus
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Typedef's
//////////////////////////////////////////////////////////////////////////////////////////////

typedef struct eva_structtestvalue
 {
  short sTest;
  char  szTestId[21];
  float fValue;
  char  cResult;
  short sRetest;
 } eva_STRUCTTESTVALUE;

typedef struct eva_structtestvalue2
 {
  short sTest;
  char  szTestId[21];
  char szTestDescription[61];
  char szTestUnit[7];
  char szFormat[11];
  char  cResult;
  short sRetest;
  float fLSL;
  float fUSL;
  float fValue;
  char szLSL[31];
  char szUSL[31];
  short sSubId;
  char szValue[31];
  char szCharacteristicDescription[81];
 } eva_STRUCTTESTVALUE2;

typedef struct eva_structtesttextvalue
 {
  short sTest;
  char  szTestId[21];
  char  szText[201];
 } eva_STRUCTTESTTEXTVALUE;

typedef struct eva_structtesttextvalue2
 {
  short sTest;
  char  szTestId[21];
  char  szTestDescription[61];
  char  szText[201];
 } eva_STRUCTTESTTEXTVALUE2;

typedef struct eva_structtest
 {
  short sTest;
  char szTestId[21];
  char szTestDescription[61];
  char cTestType;	 // 'J' varible ,'N' attributive 0/1,'A' attr. characteristic
  char szTestUnit[7];
  char szFormat[11];
  float fCoeff;
  float fLSL;
  float fUSL;
 } eva_STRUCTTEST;

typedef struct eva_structtest2
 {
  short sTest;
  char szTestId[21];
  char szTestDescription[61];
  char cTestType;	 // 'J' varible ,'N' attributive 0/1,'A' attr. characteristic
  char cDataLogging; // 'N'-none ,attributive 'A'-all 'W'-no 'V'-fail,  variable 'U'-all 'F'-no 'S'-fail
  char boOptional;
  short sSubDimensionSize;
  char szTestUnit[7];
  char szFormat[11];
  float fCoeff;
  short sClasses;
  short sBlock;
  short sRetest;
  short sBin;
  float fLSL;
  float fUSL;
 } eva_STRUCTTEST2;

typedef struct eva_structparameter
 {
  short sParameterId;
  char szParameterDescription[61];
  char szFormat[11];
  float fValue;
 } eva_STRUCTPARAMETER;

typedef struct eva_structbin
 {
  short sBinId;
  char szBinDescription[61];
 } eva_STRUCTBIN;

typedef struct eva_structcharacteristic
 {
  short sCharacteristicId;
  char szCharacteristicDescription[81];
  char cCharacteristicPass;
 } eva_STRUCTCHARACTERISTIC;

typedef struct eva_structcharacteristic2
 {
  short sCharacteristicId;
  char szCharacteristicDescription[81];
  char cCharacteristicPass;
  long lCharacteristicKey;
 } eva_STRUCTCHARACTERISTIC2;

typedef struct eva_structcharacteristicgroup
 {
  char szName[16];
  char szDescription[81];
 } eva_STRUCTCHARACTERISTICGROUP;

typedef struct eva_structtestparameter
 {
  short sParameterId;
  char szParameterDescription[61];
  char szFormat[11];
  float fValue;
  char szParameterTextValue[201];
 } eva_STRUCTTESTPARAMETER;

typedef struct eva_structblobinfo
 {
  char szDataId[21];
  char szMimeType[101];
  char szDataSource[201];
  long lDataSize;
  void *pData;
 } eva_STRUCTBLOBINFO;

typedef struct eva_structtestplancomment
 {
  long iFirstDateTime;
  char szFirstDateTime[20];
  char cMaster;
  char szText[61];
 } eva_STRUCTTESTPLANCOMMENT;

typedef struct eva_structtestrun     // out of date, please use "eva_structtestrunB"
 {
   char szTestPlanGroup[15];
   char szTestPlanName[15];
   short sTestPlanVersion;
   char szRunDateTime[20];
   char cResult;
   char szEVAPRODStation[21];
   char szMaterial[21];
   char szSoftwareName[61];
   char szSoftwareVersion[7];
   char szSoftwareType[11];
 } eva_STRUCTTESTRUN;

typedef struct eva_structtestrunB
 {
   char szTestPlanGroup[16];
   char szTestPlanName[16];
   short sTestPlanVersion;
   char szRunDateTime[20];
   char cResult;
   char szEVAPRODStation[21];
   char szMaterial[21];
   char szSoftwareName[61];
   char szSoftwareVersion[7];
   char szSoftwareType[11];
 } eva_STRUCTTESTRUN_B;

typedef struct eva_structtestrun2     // out of date, please use "eva_structtestrun3"
 {
   char szTestPlanGroup[15];
   char szTestPlanName[15];
   short sTestPlanVersion;
   char szRunDateTime[20];
   long lRunDateTime;
   char cResult;
   char sBin;
   char szEVAPRODStation[21];
   char szMaterial[21];
   char szSoftwareName[61];
   char szSoftwareVersion[7];
   char szSoftwareType[11];
   char cDataType;
 } eva_STRUCTTESTRUN2;

typedef struct eva_structtestrun3
 {
   char  szTestPlanGroup[16];
   char  szTestPlanName[16];
   short sTestPlanVersion;
   char  szRunDateTime[20];
   long  lRunDateTime;
   char  cResult;
   short sBin;
   char  szEVAPRODStation[21];
   char  szMaterial[21];
   char  szSoftwareName[61];
   char  szSoftwareVersion[7];
   char  szSoftwareType[11];
   char  cDataType;
 } eva_STRUCTTESTRUN3;

typedef struct eva_structtestplan     // out of date, please use "eva_structtestplan2"
 {
   long lPrpDateID;
   char szTestPlanGroup[15];
   char szTestPlanName[15];
   short sTestPlanVersion;
   char sReleased;
   char szCreateDate[20];
 } eva_STRUCTTESTPLAN;

typedef struct eva_structtestplan2
 {
   long  lPrpDateID;
   char  szTestPlanGroup[16];
   char  szTestPlanName[16];
   short sTestPlanVersion;
   char  cReleased;      // 'J' for released or 'N' not released
   char  szCreateDate[20];
 } eva_STRUCTTESTPLAN2;
 
//////////////////////////////////////////////////////////////////////////////////////////////
// Init Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern char* __stdcall eva_GetVersion(void);
extern char* __stdcall eva_GetReleaseDate(void);

extern short __stdcall eva_ShowErrorFL(const char *szFile, const short sLine);
extern short __stdcall eva_ShowErrorXY(const int iTop, const int iLeft, const char *szFile, const short sLine);
extern char* __stdcall eva_GetCfg(const char *szKey, const short boShowError);

extern short __stdcall eva_Init(void);
extern short __stdcall eva_GetErrorCode(void);
extern char* __stdcall eva_GetErrorText(const short sErrorCode);
extern short __stdcall eva_GetErrorInformation(char **szErrorText, const char **szFunctionName, char **szErrorHint, char **szCapsError);

extern short __stdcall eva_DisableFreeMemory(void);

extern short __stdcall eva_SetCAPSEVASlot(void);
extern short __stdcall eva_SetCAPSPAASlot(void);

extern void __stdcall  eva_SetLogLevel(const short sLogLevel);
extern void __stdcall  eva_SetLogDest(const char *szLogDest);
extern void __stdcall  eva_SetExtHintFct(void __stdcall (*aFctPtr)(const char*));

#ifndef eva_ShowError
#define eva_ShowError(DUMMY) eva_ShowErrorFL(__FILE__,__LINE__)
#endif

extern short __stdcall eva_ProcessUIR(void);

//////////////////////////////////////////////////////////////////////////////////////////////
// TestPlan Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

// Testplan Handling
extern short __stdcall eva_LoadTestPlan(const short sSlot, short *sIdx, const char *szTestPlanGroup, const char *szTestPlanName, const short sTestPlanVersion);
extern short __stdcall eva_LoadTestPlanXML(const short sSlot, short *sIdx, const char *szTestPlanGroup, const char *szTestPlanName, const short sTestPlanVersion, const char *szXMLFileName);
extern short __stdcall eva_RemoveTestPlan(const short sSlot, const short sIdx);
extern short __stdcall eva_RemoveTestPlans(const short sSlot);

// Testplan Additional Data
extern short __stdcall eva_SetMaterial(const short sSlot, const char *szMaterial);
extern short __stdcall eva_SetOrderType(const short sSlot, const char *szOrder, const char cDataType);
extern short __stdcall eva_SetOrder(const short sSlot, const char *szOrder);
extern short __stdcall eva_SetDataType(const short sSlot, const short sIdx, const char cDataType);
extern short __stdcall eva_SetTestProgram(const short sSlot, const char *szName, const char *szType, const char *szVersion);

// Testplan Tests
extern short __stdcall eva_CountTestPlanTests(const short sSlot, const short sIdx);
extern short __stdcall eva_GetTestPlanTests(const short sSlot, const short sIdx, eva_STRUCTTEST **pTests);
extern short __stdcall eva_GetTestPlanTests2(const short sSlot, const short sIdx, eva_STRUCTTEST2 **pTests);
extern short __stdcall eva_GetTestPlanTest(const short sSlot, const short sIdx, const char *szTestId, eva_STRUCTTEST *pTest);
extern short __stdcall eva_GetTestPlanTest2(const short sSlot, const short sIdx, const char *szTestId, eva_STRUCTTEST2 *pTest);
extern short __stdcall eva_CloneTestPlanTest(const short sSlot, const short sIdx, const char *szTestId, const char *szCloneTestId, const char *szCloneTestDescription);

// Testplan Test Characteristics
extern short __stdcall eva_GetTestCharacteristics(const short sSlot, const short sIdx, const char *szTestId, eva_STRUCTCHARACTERISTIC **pCharacteristics);
extern short __stdcall eva_GetCharacteristicGroups(const char *szGroupFilter, eva_STRUCTCHARACTERISTICGROUP **pCharacteristicGroups);
extern short __stdcall eva_GetGroupCharacteristics(const char *szGroup, eva_STRUCTCHARACTERISTIC2 **pCharacteristics);

// TestPlan Test Parameter
extern short __stdcall eva_GetTestParameters(const short sSlot, const short sIdx, const short sTest, eva_STRUCTTESTPARAMETER **pParameters);
extern short __stdcall eva_CountTestParameters(const short sSlot, const short sIdx, const short sTest);
extern short __stdcall eva_GetTestParameter(const short sSlot, const short sIdx, const short sTest, const short sParameterId, eva_STRUCTTESTPARAMETER *pParameter);
extern short __stdcall eva_GetTestIdParameters(const short sSlot, const short sIdx, const char *szTestId, eva_STRUCTTESTPARAMETER **pParameters);
extern short __stdcall eva_CountTestIdParameters(const short sSlot, const short sIdx, const char *szTestId);
extern short __stdcall eva_GetTestIdParameter(const short sSlot, const short sIdx, const char *szTestId, const short sParameterId, eva_STRUCTTESTPARAMETER *pParameter);

// Testplan Parameters
extern short __stdcall eva_GetTestPlanParameters(const short sSlot, const short sIdx, eva_STRUCTPARAMETER **pParameters);
extern short __stdcall eva_CountTestPlanParameters(const short sSlot, const short sIdx);
extern short __stdcall eva_GetTestPlanParameter(const short sSlot, const short sIdx, const short sParameterId, eva_STRUCTPARAMETER *pParameter);

// Testplan Binning
extern short __stdcall eva_CountTestPlanBins(const short sSlot, const short sIdx);
extern short __stdcall eva_GetTestPlanBin(const short sSlot, const short sIdx, const short sBin, char *szBinDescription);
extern short __stdcall eva_GetTestPlanBins(const short sSlot, const short sIdx, eva_STRUCTBIN **pBins);

// TestPlan Statistic
extern short __stdcall eva_TestPlanSaveStatistic(const short sSlot, const short sIdx);
extern short __stdcall eva_TestPlansSaveStatistic(const short sSlot);
extern short __stdcall eva_TestPlanResetStatistic(const short sSlot, const short sIdx);
extern short __stdcall eva_TestPlanStatisticAddComment(const short sSlot, const short sIdx,const  char *szComment, const char cMaster);
extern short __stdcall eva_TestPlanStatisticGetComments(const short sSlot, const short sIdx, eva_STRUCTTESTPLANCOMMENT **pComments);

//////////////////////////////////////////////////////////////////////////////////////////////
// TestRun Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

// Testrun Handling
extern short __stdcall eva_SetMaxTestRuns(const short sSlot, const short sIdx, const short sMaxRuns);
extern short __stdcall eva_StartTestRun(const short sSlot, const short sIdx, short *sRun);
extern short __stdcall eva_EndTestRun(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_CancelTestRun(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_RestartTestRun(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_CloseTestRun(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_ClearTestRun(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_SetTestRunId(const short sSlot, const short sIdx, const short sRun, const char *szIdType, const char *szId);
extern short __stdcall eva_SetTestRunBin(const short sSlot, const short sIdx, const short sRun, const short sBin);

// Testrun Test Handling
extern short __stdcall eva_GetNextTest(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_GetNextTestId(const short sSlot, const short sIdx, const short sRun, char *szTestId);
extern short __stdcall eva_SetTestValue(const short sSlot, const short sIdx, const short sRun, const short sTest, const float fValue);
extern short __stdcall eva_SetTestIdValue(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const float fValue);
extern short __stdcall eva_SetTestSubValue(const short sSlot, const short sIdx, const short sRun, const short sTest, const short sSubDim, const float fValue);
extern short __stdcall eva_SetTestIdSubValue(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const short sSubDim, const float fValue);
extern short __stdcall eva_GetTestResult(const short sSlot, const short sIdx, const short sRun, const short sTest);
extern short __stdcall eva_GetTestIdResult(const short sSlot, const short sIdx, const short sRun, const char *szTestId);
extern short __stdcall eva_GetTestSubResult(const short sSlot, const short sIdx, const short sRun, const short sTest, const short sSubDim);
extern short __stdcall eva_GetTestIdSubResult(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const short sSubDim);
extern short __stdcall eva_ClearTestValue(const short sSlot, const short sIdx, const short sRun, const short sTest, const float fValue);
extern short __stdcall eva_ClearTestSubValue(const short sSlot, const short sIdx, const short sRun, const short sTest, const short sSubDim);
extern short __stdcall eva_ClearTestIdSubValue(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const short sSubDim);
extern short __stdcall eva_ClearTestIdValue(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const float fValue);
extern short __stdcall eva_SetTestTextValue(const short sSlot, const short sIdx, const short sRun, const short sTest, const char *szValue);
extern short __stdcall eva_SetTestIdTextValue(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const char *szValue);

extern short __stdcall eva_SetTestCharacteristicKey(const short sSlot, const short sIdx, const short sRun, const short sTest, const long lCharacteristicKey);
extern short __stdcall eva_SetTestIdCharacteristicKey(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const long lCharacteristicKey);
extern short __stdcall eva_SetTestSubCharacteristicKey(const short sSlot, const short sIdx, const short sRun, const short sTest, const short sSubDim, const long lCharacteristicKey);
extern short __stdcall eva_SetTestIdSubCharacteristicKey(const short sSlot, const short sIdx, const short sRun, const char *szTestId, const short sSubDim, const long lCharacteristicKey);

// Testrun State/Result Handling
extern short __stdcall eva_GetTestRunResult(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_GetTestRunBin(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_GetTestRunState(const short sSlot, const short sIdx, const short sRun);

// Testrun Additional Data
extern short __stdcall eva_SetTextInfo(const short sSlot, const short sIdx, const short sRun, const char *szTextInfo);
extern short __stdcall eva_SetBLOB(const short sSlot, const short sIdx,const  short sRun, const eva_STRUCTBLOBINFO tBlobInfo);

//////////////////////////////////////////////////////////////////////////////////////////////
// Database Functions
//////////////////////////////////////////////////////////////////////////////////////////////
extern short __stdcall eva_GetDBTestPlanList(const char *szTestPlanGroupSelect, const char *szTestPlanNameSelect, const int iMaxValue, const short sReleased, eva_STRUCTTESTPLAN **pTestPlans);
extern short __stdcall eva_GetDBTestPlanList2(const char *szTestPlanGroupSelect, const char *szTestPlanNameSelect, const int iMaxValue, const short sReleased, eva_STRUCTTESTPLAN2 **pTestPlans);
extern short __stdcall eva_GetDBTestRun(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, eva_STRUCTTESTRUN *tTestRun);
extern short __stdcall eva_GetDBTestRun2(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, eva_STRUCTTESTRUN_B *tTestRun);
extern short __stdcall eva_GetDBTestRunResult(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId);
extern short __stdcall eva_GetTextInfo(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, char *szTextInfo);
extern short __stdcall eva_GetDBTextInfo(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, char *szTextInfo);
extern short __stdcall eva_GetDBTestRunValues(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, eva_STRUCTTESTVALUE **pTestValues);
extern short __stdcall eva_GetDBTestRunTextValues(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, eva_STRUCTTESTTEXTVALUE **pTestTextValues);
extern short __stdcall eva_GetDBBLOBInfo(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, eva_STRUCTBLOBINFO **pBlobInfo);
extern short __stdcall eva_GetDBBLOB(const char *szTestPlanGroup, const char *szTestPlanName, const char *szRunIdType, const char *szRunId, eva_STRUCTBLOBINFO *tBlobInfo);

extern short __stdcall eva_GetDBTestRunList(const char *szRunIdType, const char *szRunId, const char *szTestPlanGroup, const char *szTestPlanName, const char cDataType, eva_STRUCTTESTRUN2 **pTestRunList);
extern short __stdcall eva_GetDBTestRunList2(const char *szRunIdType, const char *szRunId, const char *szTestPlanGroup, const char *szTestPlanName, const char cDataType, eva_STRUCTTESTRUN3 **pTestRunList);
extern short __stdcall eva_GetDBTextInfo2(const char *szRunIdType, const char *szRunId, const long lRunDateTime, char *szTextInfo);
extern short __stdcall eva_GetDBTestRunValues2(const char *szRunIdType, const char *szRunId, const long lRunDateTime, eva_STRUCTTESTVALUE2 **pTestValues);
extern short __stdcall eva_GetDBTestRunTextValues2(const char *szRunIdType, const char *szRunId, const long lRunDateTime, eva_STRUCTTESTTEXTVALUE2 **pTestTextValues);
extern short __stdcall eva_GetDBBLOBInfo2(const char *szRunIdType, const char *szRunId, const long lRunDateTime, eva_STRUCTBLOBINFO **pBlobInfo);
extern short __stdcall eva_GetDBBLOB2(const char *szRunIdType, const char *szRunId, const long lRunDateTime, eva_STRUCTBLOBINFO *tBlobInfo);


//////////////////////////////////////////////////////////////////////////////////////////////
// Display Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall eva_InitDisplay(void);
extern short __stdcall eva_ShowTestPlans(const int iX, const int iY);
extern short __stdcall eva_HideTestPlans(void);
extern short __stdcall eva_UpdateDisplay(void);
extern short __stdcall eva_ShowTestRuns(const int iX, const int iY, const short sSlot, const short sIdx);
extern short __stdcall eva_HideTestRuns(const short sSlot, const short sIdx);
extern short __stdcall eva_ShowTests(const int iX, const int iY, const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_HideTests(const short sSlot, const short sIdx, const short sRun);
extern short __stdcall eva_SetStatusText(const short sSlot, const short sIdx, const short sRun, const char *szText);

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
    }
#endif

#endif
