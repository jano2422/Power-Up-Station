#include <windows.h>
#include <formatio.h>
#include <utility.h>
#include <CviTypes.h>
#include <ansi_c.h>
#include <ApplicationTools.h>
#include <inifile.h>
#include <IniData.h> 

#define _USE_DLL		// Hilft wahrscheinlich nichts, da CVI keine Makros in C-Files auch in H-Files expandiert ???
#include <AS_DLL.h>
#include <TPDtoEVAFunc.h>
#include <definitions.h>  
#include <RunSpsTestprogramm.h>
#include <CviLib.h>
#include <evaprod.h>  
#include <wipmama.h>

#include <HpSupply.h>
#include <6TL_Cards.h>
#include <NI_DMM4070.h>
#include <ProMik_CAN_Flash.h>


#include <CAN_Base.h>

#include "a2ldata.h"
#include "strlib.h"
#include "a2lppar.h"
#include "res2xml.h"
#include "ReadXmlFile.h"
#include "MotCRC.h"
#include "Error Codes.h"

#include <XcpViaCan.h>
#include <Can.h> 
#include <FixtureSlot.h>  




#include "KeyGenResultExInterface.h" 





#define DisplayError(x) WriteToErrorWin(x)
#ifdef TIME
double sumOfTestTime = 0 ;
#endif


////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////

char g_szVersion[32] = { _TESTSOFTWARE_VERSION };  
char g_szCustomerType[32] = { _SOFTWARE_LOCKING };

FctPtr g_MeasArray[MAX_MEAS_FUNCTIONS];



// ApplicationMode3.c
extern char 	g_szOrder[100];
extern short 	g_sSlot, g_sIdx, g_sRun;
extern Boolean g_boOPC;
extern Boolean g_boIO;
extern Boolean g_boMANUAL;

// RunSpsTestprogramm.c	 

extern char g_szScanTmpSn[OpcStringLength]; 
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
// Materialconfigurationen
//***************************************************************************************
//***************************************************************************************




//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

static unsigned short g_usPPARCRC16 = 0;
static unsigned long g_ulPPARStartAdr = 0;
static unsigned long g_ulPPAREndAdr = 0;

//***************************************************************************************
// Pfadconfigurationen
//***************************************************************************************

static char * g_szEndtestparameterXMLFileName = "SRR3XX_Endtestparameter.xml";

static char * g_szResultTXTFileName = "NoiseResults.txt";
static char * g_szResultXMLFileName = "NoiseResults.xml";

static char * g_szPPARTemplateMOTFileName = "SRR3XX_PPAR-Template.mot";
static char * g_szPPARA2LFileName = "SRR3XX_PPAR.a2l";		 
static char * g_szTargetPPARMOTFileName = "SRR3XX_PPAR_Filled.mot";
	 
static char * g_szAPPLMOTFileName = "SRR3XX_APPL_Production.mot"; // "SRR3T0_M11_APPL_Release.mot"; wrong file, PTS says use PRODUCITON file    
static char * g_szAPPLA2LFileName = "SRR3XX_APPL_Release.a2l";
static char * g_szAPARFileName = "SRR3XX_APAR_Release.mot";
static char * g_szNvmClearMOTFileName = "SRR3XX_Nvm_Image_to_Clear_xR.mot";    


static char * g_szErrCodesFileName = "ErrorMemoryContent.txt"; 

struct
{	
	char g_szEndtestparameterXML[MAX_PATHNAME_LEN]; 
	char g_szEndtestparameterXML_LOCAL[MAX_PATHNAME_LEN];  
	
	char g_szResultTXT[MAX_PATHNAME_LEN];
	char g_szResultXML[MAX_PATHNAME_LEN];
	
	char g_szPPARTemplateMOT[MAX_PATHNAME_LEN];
	char g_szPPARA2L[MAX_PATHNAME_LEN];
 	char g_szTargetPPARMOT[MAX_PATHNAME_LEN];	 
	
	char g_szAPARMOT[MAX_PATHNAME_LEN];   //APAR MotFile 
	
	char g_szAPPLMOT[MAX_PATHNAME_LEN];						//Application SW
	char g_szAPPLMOT_LOCAL[MAX_PATHNAME_LEN];						//Application SW 
					
	
	char g_szAPPLA2L[MAX_PATHNAME_LEN];	
	
	char g_szNvmClearMOT[MAX_PATHNAME_LEN];  
	char g_szNvmClearMOT_LOCAL[MAX_PATHNAME_LEN];
	
	char g_szBlobFile[MAX_FILENAME_LEN];

} g_tEtPathName;


static char g_szAppSW_LAST[MAX_PATHNAME_LEN]					= "";
static char g_szXMLEndtestParameterFile_LAST[MAX_PATHNAME_LEN] 	= "";
static char g_szNvmClearMOT_LAST[MAX_PATHNAME_LEN]				= "";

// globale Variable for the local paths, specified in the frames.cfg - filled at ApplictionMode3
extern char g_szTemplateFolder[];   
extern char g_szTempFileFolder[];
extern char g_szTestPlanNameForResultFile[];
extern char g_szPowerUpTime[];


struct
{
	unsigned long ulStartBRES;
	unsigned long ulEndBRES;
	unsigned long ulStartVFBL;
	unsigned long ulEndVFBL;
	unsigned long ulStartPSW;
	unsigned long ulEndPSW;
	unsigned long ulStartAPPL;
	unsigned long ulEndAPPL;	
	unsigned long ulStartAPAR;
	unsigned long ulEndAPAR;
	unsigned long ulStartPPAR;
	unsigned long ulEndPPAR;
	unsigned long ulStartNVM;
	unsigned long ulEndNVM;
} g_tSWAddresses;


// Parameter
struct
{
	int g_iEtpHwCompatId;
	int *g_aiETP_CustomerHwNumber; 
	int g_iValCntCustomerHWNumber; 
	int g_iEtpCustumerId;
	int g_iEtpSubCompatIdmin;
	int g_iEtpSubCompatIdmax;
	int g_iEtpApplRunTimeSec;
	int g_iEtpSupplyVoltage;
	char *g_szEtpSrr320MIPartNumber;
	int *g_aiEtpAllowedErrorCodes;
	int *g_aiEtpRequiredErrorCodes;
	int g_iValCntAllowed;
	int g_iValCntRequired;
} g_tEtFileParameter;	





//***************************************************************************************
//Utility functions
//***************************************************************************************
int boGetTestplanParameter(const short sSlot, const short sIdx, const char * szName, const char ** pszStringLink, float * pfValue)
{ // Sucht einen Testplanparameter der Form PARAMNAME=PARAMTEXT über PARAMNAME; Liefert ggf. PARAMTEXT und/oder VALUE; 1, wenn gefunden, 0, wenn nicht.
	static eva_STRUCTPARAMETER * ptParameters = NULL;
	int iNameLen = strlen(szName);
	short sResult = 0;
	int i = 0;
	
	sResult = eva_GetTestPlanParameters(sSlot, sIdx, &ptParameters);
	
	for (i = 0; i < sResult; i++)
	{
		if (strncmp(ptParameters[i].szParameterDescription, szName, iNameLen) == 0)
		{
			if (ptParameters[i].szParameterDescription[iNameLen] == '=')
			{
				if (pszStringLink) *pszStringLink = ptParameters[i].szParameterDescription + iNameLen + 1;
				if (pfValue) *pfValue = ptParameters[i].fValue;
				return 1;
			}
			else if (ptParameters[i].szParameterDescription[iNameLen] == '\0')
			{
				if (pszStringLink) *pszStringLink = NULL;
				if (pfValue) *pfValue = ptParameters[i].fValue;
				return 1;
			}
		}
	}
	
	return 0;
}


/*float fGetTestplanParameterValue(const short sSlot, const short sIdx, const char * szName)
{
	float fTransfer = 0.0;
	
	if (!boGetTestplanParameter(sSlot, sIdx, szName, NULL, &fTransfer))
	{ // Nicht gefunden
		return NotANumber();
	}
	else
	{
		return fTransfer;
	}
} */

const char * szGetTestplanParameterText(const short sSlot, const short sIdx, const char * szName, const char * szDefault)
{
	const char * szTransfer = szDefault;
	
	if (!boGetTestplanParameter(sSlot, sIdx, szName, &szTransfer, NULL))
	{ // Nicht gefunden
		return szDefault;
	}
	else
	{
		return szTransfer;
	}
}

unsigned long ulLongFromHex(const char * szHexValue)
{
	unsigned long ulValue = 0x00000000;
	
	if (!szHexValue || !*szHexValue) return 0x00000000;
	
	// Preconditions: 0x, $, # ...
	if (szHexValue[0] == '0' && (szHexValue[1] == 'x' || szHexValue[1] == 'X'))
	{
		szHexValue += 2;
	}
	
	if (szHexValue[0] == 'x' || szHexValue[0] == 'X' || szHexValue[0] == '#' || szHexValue[0] == '$')
	{
		szHexValue += 1;
	}
	
	// Char by char
	while ((*szHexValue >= '0' && *szHexValue <= '9') || (*szHexValue >= 'A' && *szHexValue <= 'F') || (*szHexValue >= 'a' && *szHexValue <= 'f'))
	{
		if ((*szHexValue >= '0' && *szHexValue <= '9'))
		{
			ulValue = (ulValue << 4) + (*szHexValue - '0'); // Klammer ist eigtl. unnötig, aber vereinfacht die Lesbarkeit
		}
		else if ((*szHexValue >= 'A' && *szHexValue <= 'F'))
		{
			ulValue = (ulValue << 4) + (*szHexValue - 'A' + 10); // Klammer ist eigtl. unnötig, aber vereinfacht die Lesbarkeit
		}
		else if ((*szHexValue >= 'A' && *szHexValue <= 'f'))
		{
			ulValue = (ulValue << 4) + (*szHexValue - 'a' + 10); // Klammer ist eigtl. unnötig, aber vereinfacht die Lesbarkeit
		}
		szHexValue++;
	}
	
	return ulValue;
}


void CanWriteError(char szError[])
{
	WriteToErrorWin(szError);
}

	
extern char * szHexToString(unsigned char * aucData, unsigned int uSize);
/*{
	static char szData[3079];
	int iCnt;
	
	memset(szData, 0, sizeof(szData));
	
	if (uSize > 1024)
	{
		uSize = 1024;
		sprintf(szData, "[LIM!]");
	}
	else
	{
		sprintf(szData, "[%d]", uSize);
	}
	
	for (iCnt = 0; iCnt < uSize; iCnt++)
	{
		sprintf(szData, "%s %02X", szData, aucData[iCnt]);
	}
	
	return szData;
}		*/


void SetTestplanTextInfoV(int aStep, int aPoint, const char * szMessage, va_list vlArgs)
{ // since V1.00-1 as of 20110822
	char szNumBuffer[16];
	
	static char szWriteMessage[1024];
	
	vsnprintf(szWriteMessage, 1024, szMessage, vlArgs);
	
	if (aStep == 0)
	{
		eva_SetTextInfo(g_EvaProdData.sSlot, g_EvaProdData.sIdx, g_EvaProdData.sRun, szWriteMessage);
	}
	else																																																					  
	{
		if (aPoint == 0)
		{
			sprintf(szNumBuffer, "%d", aStep);
		}
		else
		{
			sprintf(szNumBuffer, "%d,%d", aStep, aPoint);
		}
		eva_SetTestIdTextValue(g_EvaProdData.sSlot, g_EvaProdData.sIdx, g_EvaProdData.sRun, szNumBuffer, szWriteMessage); 
	}
}

void SetTestplanTextInfo(int aStep, int aPoint, const char * szMessage, ...)
{ // since V1.00-1 as of 20110822
	va_list vlArgs;
	
	va_start(vlArgs, szMessage);
	SetTestplanTextInfoV(aStep, aPoint, szMessage, vlArgs)
	va_end(vlArgs);
}

void SetErrorTestStep(int aStep, int aPoint, const char * szMessage, ...)
{ // since V1.00-1 as of 20110822
	va_list vlArgs;
	
	va_start(vlArgs, szMessage);
	SetTestplanTextInfoV(aStep, aPoint, szMessage, vlArgs);
	WriteToErrorWinV(szMessage, vlArgs);
	va_end(vlArgs);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:  boLoadResultFile
//
// Description: gets last Result File
//
// Parameters:
// char szTemp[]        
//
// return value               iErrorState
//														
//
//////////////////////////////////////////////////////////////////////////////////////////////
//************************************************************************************************
// hole die Zeit des neusesten Testergebnisses
Boolean boLoadLastTestRunTime( char szUnitID[], char szUnitIDType[], const char szTestplanName[], long *lRunDateTime)
{
	char cDataType = 0;
	int iFound = 0;
	Boolean bRet = True;
	static eva_STRUCTTESTRUN2 *pTestRunList = NULL;   


	// lade eine Liste mit allen Beampattern Testprotokollen
	iFound = eva_GetDBTestRunList(szUnitIDType, szUnitID, "", szTestplanName, cDataType, &pTestRunList);
	if(0 == iFound) {
		WriteToErrorWin("Error on get DBTestRunList %s!",szUnitID);
		bRet = False ;
		*lRunDateTime = -1;
	}
	else {
		if(pTestRunList[0].lRunDateTime > pTestRunList[1].lRunDateTime ) {
			// Prüfe ob Testrun PASS war
			if( pTestRunList[0].cResult != 'P' ) {
				WriteToErrorWin("Error last Beam Testrun was not PASS %s!",szUnitID);
				bRet = False ;
				*lRunDateTime = pTestRunList[0].lRunDateTime;
			}
			else {
				// verwendet wird der erste Testlauf
				*lRunDateTime = pTestRunList[0].lRunDateTime;
			}
		}
		else {
			// Prüfe ob Testrun PASS war
			if( pTestRunList[iFound-1].cResult != 'P' ) {
				WriteToErrorWin("Error last Beam Testrun was not PASS %s!",szUnitID);
				bRet = False ;
				*lRunDateTime = pTestRunList[iFound-1].lRunDateTime;
			}
			else {
				// verwendet wird der letzte Testlauf
				*lRunDateTime = pTestRunList[iFound-1].lRunDateTime;
			}
		}

		if (bRet == False ) {
			*lRunDateTime = -1;
		}
	}

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////




/***************************************************************************************
* -------------------------------------------------------------------------
*  function name:   F01 
*  comment:       	Load Parameter
* -------------------------------------------------------------------------
*  Version  Date        Name    	Text
*  1.0        			 
* -------------------------------------------------------------------------
/**************************************************************************************/
int i_F01(int iId)		//Load Parameter
{

	DataStorePutString(1, g_szCustomerType);
	
	DataStorePutString(2, g_szVersion);
	
	
	return( 0 );
}


/***************************************************************************************
* -------------------------------------------------------------------------
*  function name:   F10 
*  comment:       	Load Parameter
* -------------------------------------------------------------------------
*  Version  Date        Name    	Text
*  1.0        			 
* -------------------------------------------------------------------------
/**************************************************************************************/
int i_F10(int iId)		//Load Parameter
{
	
	CVIXMLDocument 		hETXML = 0;
	int 							iError = 0;
	int 							boErrorOnWrite = 0;
	int 							boResult = 0;
	unsigned long			ulCRC32 = 0;

	char szChecksumAPAR[11] = {0};
	char szChecksumAPPLMOT[11] = {0};
	char szChecksumETParameterXML[11] = {0};
	char szChecksumPPARMOT[11] = {0};
	char szChecksumPPARA2L[11] = {0};
	char szChecksumAPPLA2L[11] = {0};
	char szChecksumNvmClearMOT[11] = {0};  
	
	#ifdef TIME
	double 	act_time;
	act_time = Timer();
	#endif
	
	
	//**************************************** Load paths (defined in FRAMES.CFG-File)   ************************************************************** // Get Pathnames of Files
	memset( &g_tEtPathName, '\0', sizeof(g_tEtPathName) ); 
	
	sprintf(g_tEtPathName.g_szAPARMOT, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szAPARFileName);
	sprintf(g_tEtPathName.g_szEndtestparameterXML, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szEndtestparameterXMLFileName);
	sprintf(g_tEtPathName.g_szEndtestparameterXML_LOCAL, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szEndtestparameterXMLFileName);

 	sprintf(g_tEtPathName.g_szResultTXT, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szResultTXTFileName);
	sprintf(g_tEtPathName.g_szResultXML, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szResultXMLFileName);

	sprintf(g_tEtPathName.g_szPPARTemplateMOT, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szPPARTemplateMOTFileName); // Wehe einer memmt jetz rum wegen dem ternären Operator. Wehe. ;)
	sprintf(g_tEtPathName.g_szPPARA2L, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szPPARA2LFileName);
 	sprintf(g_tEtPathName.g_szTargetPPARMOT, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szTargetPPARMOTFileName);

	sprintf(g_tEtPathName.g_szAPPLMOT, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szAPPLMOTFileName);
	sprintf(g_tEtPathName.g_szAPPLMOT_LOCAL, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szAPPLMOTFileName);

	sprintf(g_tEtPathName.g_szAPPLA2L, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szAPPLA2LFileName);

	sprintf(g_tEtPathName.g_szNvmClearMOT, "%s%s%s\\%s", g_szTemplateFolder, (g_szTemplateFolder[strlen(g_szTemplateFolder)-1] == '\\' ? "" : "\\"), g_UnitInfo.szUnitMaterial, g_szNvmClearMOTFileName);
	sprintf(g_tEtPathName.g_szNvmClearMOT_LOCAL, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szNvmClearMOTFileName);

	
	//File local copy if necessary		g_szEndtestparameterXML
	if( strcmp( g_szXMLEndtestParameterFile_LAST, g_tEtPathName.g_szEndtestparameterXML) !=0 ) {
		if( DeleteFile(g_tEtPathName.g_szEndtestparameterXML_LOCAL) < -1 ){
				WriteToErrorWin("Error on deleting Endtestparameter-file on local drive");
				return __LINE__;
		}		
		if( CopyFile(g_tEtPathName.g_szEndtestparameterXML, g_tEtPathName.g_szEndtestparameterXML_LOCAL) != 0 ) {
				WriteToErrorWin("Error on copy Endtestparameter-file to local drive");
				return __LINE__;
		}
		strcpy( g_szXMLEndtestParameterFile_LAST, g_tEtPathName.g_szEndtestparameterXML);
	}
	
	//File local copy if necessary		g_szAppSW
	if( strcmp( g_szAppSW_LAST, g_tEtPathName.g_szAPPLMOT) !=0 ) {
		if( DeleteFile(g_tEtPathName.g_szAPPLMOT_LOCAL) < -1 ){
				WriteToErrorWin("Error on deleting AppSW-file on local drive");
				return __LINE__;
		}		
		if( CopyFile(g_tEtPathName.g_szAPPLMOT, g_tEtPathName.g_szAPPLMOT_LOCAL) != 0 ) {
				WriteToErrorWin("Error on copy AppSW-file to local drive");
				return __LINE__;
		}
		strcpy( g_szAppSW_LAST, g_tEtPathName.g_szAPPLMOT);
	}
	
	//File local copy if necessary		g_szNvmClearMOT
	/*if( strcmp( g_szNvmClearMOT_LAST, g_tEtPathName.g_szNvmClearMOT) !=0 ) {
		if( DeleteFile(g_tEtPathName.g_szNvmClearMOT_LOCAL) < -1 ){
				WriteToErrorWin("Error on deleting NvmClearMOT-file on local drive");
				return __LINE__;
		}		
		if( CopyFile(g_tEtPathName.g_szNvmClearMOT, g_tEtPathName.g_szNvmClearMOT_LOCAL) != 0 ) {
				WriteToErrorWin("Error on copy NvmClearMOT-file to local drive");
				return __LINE__;
		}
		strcpy( g_szNvmClearMOT_LAST, g_tEtPathName.g_szNvmClearMOT);
	}*/
	
	//**************************************** Load Data from XML-File ************************************************************** 
	
	memset(&g_tEtFileParameter.g_iEtpHwCompatId, -1, sizeof(g_tEtFileParameter.g_iEtpHwCompatId)); // reset all global variables!    
	if( g_tEtFileParameter.g_aiETP_CustomerHwNumber != NULL ) {
		free(g_tEtFileParameter.g_aiETP_CustomerHwNumber);
	}
	g_tEtFileParameter.g_aiETP_CustomerHwNumber = NULL; // reset all global variables!    
	memset(&g_tEtFileParameter.g_iValCntCustomerHWNumber, -1, sizeof(g_tEtFileParameter.g_iValCntCustomerHWNumber)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_iEtpCustumerId, -1, sizeof(g_tEtFileParameter.g_iEtpCustumerId)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_iEtpSubCompatIdmin, -1, sizeof(g_tEtFileParameter.g_iEtpSubCompatIdmin)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_iEtpSubCompatIdmax, -1, sizeof(g_tEtFileParameter.g_iEtpSubCompatIdmax)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_iEtpApplRunTimeSec, -1, sizeof(g_tEtFileParameter.g_iEtpApplRunTimeSec)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_iEtpSupplyVoltage, -1, sizeof(g_tEtFileParameter.g_iEtpSupplyVoltage)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_szEtpSrr320MIPartNumber, '\0', sizeof(g_tEtFileParameter.g_szEtpSrr320MIPartNumber)); // reset all global variables!    
	if( g_tEtFileParameter.g_aiEtpAllowedErrorCodes != NULL ) {
		free(g_tEtFileParameter.g_aiEtpAllowedErrorCodes);
	}
	g_tEtFileParameter.g_aiEtpAllowedErrorCodes = NULL;
	if( g_tEtFileParameter.g_aiEtpRequiredErrorCodes != NULL ) {
		free(g_tEtFileParameter.g_aiEtpRequiredErrorCodes);
	}
	g_tEtFileParameter.g_aiEtpRequiredErrorCodes = NULL;
	memset(&g_tEtFileParameter.g_iValCntAllowed, -1, sizeof(g_tEtFileParameter.g_iValCntAllowed)); // reset all global variables!    
	memset(&g_tEtFileParameter.g_iValCntRequired, -1, sizeof(g_tEtFileParameter.g_iValCntRequired)); // reset all global variables!    

	
	
	// Open Endtest-Parameter-File
	iError = iOpenXmlFile(g_tEtPathName.g_szEndtestparameterXML_LOCAL, &hETXML);
	if (iError) {
		DataStorePutBool(1, !iError); 
		SetErrorTestStep(iId, 1, "Error opening Endtest-XML-File");
		return __LINE__;
	}
	//DataStorePutBool(x, !iError);
	
		
	
	// Load 5.13.1 ETP_CustomerHwNumber Array!!   aiETP_CustomerHwNumber     
	iError = iReadUniqueXmlIntArrayElement(&g_tEtFileParameter.g_aiETP_CustomerHwNumber,  &g_tEtFileParameter.g_iValCntCustomerHWNumber, "ETP_CustomerHwNumber", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_aiETP_CustomerHwNumber == NULL) boErrorOnWrite = 1; 			// check if something is writen in global variable! 
	if (g_tEtFileParameter.g_iValCntCustomerHWNumber < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!
	
	// Load ETP_CustomerID
	iError = iReadUniqueXmlIntElement(&g_tEtFileParameter.g_iEtpCustumerId, "ETP_CustomerID", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_iEtpCustumerId < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!   
	
	
	// Load 5.13.2 ETP_HwCompatId 
	iError = iReadUniqueXmlIntElement(&g_tEtFileParameter.g_iEtpHwCompatId, "ETP_HwCompatId", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_iEtpHwCompatId < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!      
		
	// Load 5.13.3 ETP_SubCompatIdMin
	iError = iReadUniqueXmlIntElement(&g_tEtFileParameter.g_iEtpSubCompatIdmin, "ETP_SubCompatIdmin", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_iEtpSubCompatIdmin < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!  
	
	// Load 5.13.3 ETP_SubCompatIdMax
	iError = iReadUniqueXmlIntElement(&g_tEtFileParameter.g_iEtpSubCompatIdmax, "ETP_SubCompatIdmax", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_iEtpSubCompatIdmax < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!      
	
	// Load ETP_ApplRunTime_sec
	iError = iReadUniqueXmlIntElement(&g_tEtFileParameter.g_iEtpApplRunTimeSec, "ETP_ApplRunTime_sec", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_iEtpApplRunTimeSec < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!      
	
	// Load ETP_Supply_Voltages
	iError = iReadUniqueXmlIntElement(&g_tEtFileParameter.g_iEtpSupplyVoltage, "ETP_Supply_Voltage", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_iEtpSupplyVoltage < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!      
	
	// Load ETP_AllowedErrorCodes
	iError = iReadUniqueXmlIntArrayElement (&g_tEtFileParameter.g_aiEtpAllowedErrorCodes, &g_tEtFileParameter.g_iValCntAllowed,  "ETP_AllowedErrorCodes", hETXML);
	if (iError) boErrorOnWrite = 1;
	if (g_tEtFileParameter.g_aiEtpAllowedErrorCodes == NULL) boErrorOnWrite = 1; 			// check if something is writen in global variable!         
	if (g_tEtFileParameter.g_iValCntAllowed < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!

	// Load ETP_RequiredErrorCodes
	iError = iReadUniqueXmlIntArrayElement (&g_tEtFileParameter.g_aiEtpRequiredErrorCodes, &g_tEtFileParameter.g_iValCntRequired,  "ETP_RequiredErrorCodes", hETXML);
	if (iError) boErrorOnWrite = 1;
	// not neccessary, because there are no Required Errors specified --> (g_tEtFileParameter.g_aiEtpRequiredErrorCodes == NULL) boErrorOnWrite = 1; 			// check if something is writen in global variable!         
	if (g_tEtFileParameter.g_iValCntRequired < 0) boErrorOnWrite = 1; 			// check if something is writen in global variable!        

	// Vehicle/Platform specific PartNumber (MITSUBIHI specific)

	if (g_tEtFileParameter.g_szEtpSrr320MIPartNumber != NULL)
	{
    	free(g_tEtFileParameter.g_szEtpSrr320MIPartNumber);
    	g_tEtFileParameter.g_szEtpSrr320MIPartNumber = NULL;
	}					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     

	iError = iReadWildcardMIPartNumber(&g_tEtFileParameter.g_szEtpSrr320MIPartNumber, hETXML);

	if (iError != XML_FILE_OK ||
    	g_tEtFileParameter.g_szEtpSrr320MIPartNumber == NULL ||
    	g_tEtFileParameter.g_szEtpSrr320MIPartNumber[0] == '\0')
	{
    	boErrorOnWrite = 1;   // nothing valid written
	}


	
	// Close Endtest-Parameter-File
	iError = iCloseXmlFile(hETXML);
	if (iError) {
		DataStorePutBool(1, !iError);
		SetErrorTestStep(iId, 1, "Error closing Endtest-XML-File");
		return __LINE__;
	}
	
	DataStorePutBool(1, !boErrorOnWrite);
	if ( boErrorOnWrite ) {
		SetErrorTestStep(iId, 1, "Error reading Endtest-XML-File");
		return (__LINE__);
	}
	
	
 	//********************************************SW-Block From Files loaded from Testplanparameter <-> MES******************************************************** 

	memset(&g_tSWAddresses, 0, sizeof(g_tSWAddresses));
	
	g_tSWAddresses.ulStartBRES = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_BRES", NULL));
	g_tSWAddresses.ulEndBRES = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_BRES", NULL));
	g_tSWAddresses.ulStartVFBL = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_VFBL", NULL));
	g_tSWAddresses.ulEndVFBL = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_VFBL", NULL));
	//g_tSWAddresses.ulStartPSW = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_PSW", NULL));
	//g_tSWAddresses.ulEndPSW = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_PSW", NULL));
	g_tSWAddresses.ulStartAPPL = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_APPL", NULL));
	g_tSWAddresses.ulEndAPPL = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_APPL", NULL));
	//g_tSWAddresses.ulStartPPAR = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_PPAR", NULL));
	//g_tSWAddresses.ulEndPPAR = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_PPAR", NULL));
	g_tSWAddresses.ulStartAPAR = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_APAR", NULL));
	g_tSWAddresses.ulEndAPAR = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_APAR", NULL));	
	//g_tSWAddresses.ulStartNVM = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "START_ADDR_NVM", NULL));
	//g_tSWAddresses.ulEndNVM = ulLongFromHex(szGetTestplanParameterText(g_sSlot, g_sIdx, "STOP_ADDR_NVM", NULL));	
	
	
	
	
	// =================
	// 4.1 Preparation
	// =================
		 
	//********************************************CRC32 Checksum File Validation********************************************************
	
	// 51005: Check CRC32
	// ===============================================================
	// APAR-File checksumverifiction Apar				   //in betrieb..
	boResult = boCalculateCRC32(g_tEtPathName.g_szAPARMOT, &ulCRC32);
	if ( !boResult ) {
		DataStorePutBool(5, boResult);
		SetErrorTestStep(iId, 5, "Error calculating CRC32");
		return (__LINE__);
	}
	memset(szChecksumAPAR, 0, sizeof(szChecksumAPAR));
	sprintf(szChecksumAPAR, "0x%08X", ulCRC32);
	DataStorePutString(5, szChecksumAPAR);
	
	// 51010: Check CRC32
	// ===============================================================
	// ApplicationSw-File checksumverification  APPL
	boResult = boCalculateCRC32(g_tEtPathName.g_szAPPLMOT_LOCAL, &ulCRC32);
	if ( !boResult ) {
		DataStorePutBool(6, boResult);
		SetErrorTestStep(iId, 6, "Error calculating CRC32");
		return (__LINE__);
	}
	memset(szChecksumAPPLMOT, 0, sizeof(szChecksumAPPLMOT));
	sprintf(szChecksumAPPLMOT, "0x%08X", ulCRC32);
	DataStorePutString(6, szChecksumAPPLMOT);

	// 51011: Check CRC32
	// ===============================================================
	// Endtest-Parameter-File checksumverification  
	boResult = boCalculateCRC32(g_tEtPathName.g_szEndtestparameterXML_LOCAL, &ulCRC32);
	if ( !boResult ) {
		DataStorePutBool(7, boResult);
		SetErrorTestStep(iId, 7, "Error calculating CRC32");
		return (__LINE__);
	}
	memset(szChecksumETParameterXML, 0, sizeof(szChecksumETParameterXML));
	sprintf(szChecksumETParameterXML, "0x%08X", ulCRC32);
	DataStorePutString(7, szChecksumETParameterXML);

	// 51012: Check CRC32 of PPAR-Template-file.mot file
	// ===============================================================
	// PPAR-Template Parameter-File checksumverification
	boResult = boCalculateCRC32(g_tEtPathName.g_szPPARTemplateMOT, &ulCRC32);
	if ( !boResult ) {
		DataStorePutBool(8, boResult);
		SetErrorTestStep(iId, 8, "Error calculating CRC32");
		return (__LINE__);
	}
	memset(szChecksumPPARMOT, 0, sizeof(szChecksumPPARMOT));
	sprintf(szChecksumPPARMOT, "0x%08X", ulCRC32);
	DataStorePutString(8, szChecksumPPARMOT);

	// 51013: Check CRC32 of a2l-file
	// ===============================================================
	// A2L-Parameter-File checksumverification  
	boResult = boCalculateCRC32(g_tEtPathName.g_szPPARA2L, &ulCRC32);
	if ( !boResult ) {
		DataStorePutBool(9, boResult);
		SetErrorTestStep(iId,9, "Error calculating CRC32");
		return (__LINE__);
	}
	memset(szChecksumPPARA2L, 0, sizeof(szChecksumPPARA2L));
	sprintf(szChecksumPPARA2L, "0x%08X", ulCRC32);
	DataStorePutString(9, szChecksumPPARA2L);
	

	// 51014: A2L file checksum verification
	// ===============================================================
	boResult = boCalculateCRC32(g_tEtPathName.g_szAPPLA2L, &ulCRC32);
	if( !boResult ) {
		DataStorePutBool(10, boResult); 
		SetErrorTestStep(iId, 10, "Error calculating CRC32 checksum of APPL SW A2l-file");
		return( __LINE__);
	}
	memset(szChecksumAPPLA2L, 0, sizeof(szChecksumAPPLA2L));
	sprintf(szChecksumAPPLA2L, "0x%08X", ulCRC32);
	DataStorePutString(10, szChecksumAPPLA2L);
	
	

	#ifdef TIME
	// calculate modultime
	WriteToDataWin("F10: %.3f s", Timer() - act_time);
	sumOfTestTime += Timer() - act_time; 
	#endif
	
	return( 0 );
}



/***************************************************************************************
* -------------------------------------------------------------------------
*  function name:   F90 
*  comment:       	Read error memory
* -------------------------------------------------------------------------
*  Version  Date        Name    	Text
*  1.0        			 
* -------------------------------------------------------------------------
/**************************************************************************************/
int i_F90(int iId)		//Read error memory      
{
	Boolean boErrorOnConnect = 0;
	Boolean boErrorOnRead = 0;
	Boolean boErrorOnDisconnect = 0;
	Boolean boErrorOnCheck = 0;
	Boolean boAllowed = 0;
	Boolean boRequired = 0;


	int boResult = 0;
	int iError = 0;
	unsigned char aucRxData[4096];
	unsigned char aucTxData[128];
	unsigned int uDataSize = 0;
	//int iApplActRunTime = 0;
	
	double dApplActRunTime = 0.0;
	
	char szErrMemContentTxt[MAX_PATHNAME_LEN];
	
	char * szErrCodeIdFormat = "Dem_NvmCSEventList[%d].NvM_ui16_EventID";
	char szErrCodeId[100]= {0};
	char szErrMemAddr[100]= {0};
	unsigned int auiErrMemAddr[30] = {0};
	unsigned short ausErrMemEvents[30] = {0};
	unsigned short ausNotAllowedErrorCodes[30] = {0};
	unsigned short usEvent = 0;
	
	int iCnt = 0;
	int iCntOne = 0;
	int iCntTwo = 0;
	
	static TSimulationList * ptList = NULL;
	TMessageSimulation * ptMessage;

	static TSimulationList * ptList2 = NULL;
	TMessageSimulation * ptMessage2;
	
	
	double dApplStartTime = 0.0;
	double dPowerUpStartTime = 0.0;
	double dPowerUpElapsedTime;
	double dPowerUpTime = 0.0;
	
	FILE * tmpFile = NULL;
	
	
	double delayFromParameter; //V42H - added delay for longer restbus
	
	TA2LFile * ptA2LFile = NULL;
	TA2LBlock * ptA2LBlock = NULL;
	
	
	char szDiagRes[1024] = {0};
	int iResCnt = 0;
	
	char szHWVersion[6] = {0};
	char szSWVersion[9] = {0};
	char szHWPartNo[9] = {0};
	char szSWPartNo[9] = {0};
	char szECUSN[15] = {0};
	
	
	#ifdef TIME
	double 	act_time;
	act_time = Timer();
	#endif
	
	








        if (!boSetOnYavRel(CARD904X8_2, FixtureSlot_GetActivePrivateCanRelay()))
        {
                return( __LINE__);
        }
	Sleep(20);
	
	
	
	
	
	
	
	memset(aucTxData, 0, sizeof(aucTxData));
	memset(aucRxData, 0, sizeof(aucRxData));  
	
		
	memset(szErrMemContentTxt, 0, sizeof(szErrMemContentTxt));    
	sprintf(szErrMemContentTxt, "%s%s%s", g_szTempFileFolder, (g_szTempFileFolder[strlen(g_szTempFileFolder)-1] == '\\' ? "" : "\\"), g_szErrCodesFileName);
	
	
	
												 
	
	// Configure restbus simulation
	// 57000
	if (!ptList)
	{ // Nur beim ersten Mal definieren, dann bleibt die Restbussimulation für die gesamte Software bestehen
		ptList = ptCreateNewEmptySimulationList();
	
		// MRM_236h
		ptMessage = ptCreateMessage("MRM_236h", 0x236, 10, 0, 8,  (unsigned char *) "\x10\x00\x00\x00\x40\x00\x00\xD3", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// VEH_SPEED
		ptMessage = ptCreateMessage("VEH_SPEED", 0x215, 20, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// GS_418h
		ptMessage = ptCreateMessage("GS_418h", 0x418, 20, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x20\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// BS_300h
		ptMessage = ptCreateMessage("BS_300h", 0x300, 20, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x80\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// FCM_01
		ptMessage = ptCreateMessage("FCM_01", 0x315, 20, 0, 7,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// GW_OUT_01
		ptMessage = ptCreateMessage("GW_OUT_01", 0x284, 50, 0, 8,  (unsigned char *) "\x10\x00\x00\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// ETACS_OUT1
		ptMessage = ptCreateMessage("ETACS_OUT1", 0x424, 40, 0, 8,  (unsigned char *) "\x00\x00\x04\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// METER_OUT_03
		ptMessage = ptCreateMessage("METER_OUT_03", 0x154, 100, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x19\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// NET_CFG
		ptMessage = ptCreateMessage("NET_CFG", 0x6FD, 300, 0, 8,  (unsigned char *) "\x01\x85\x00\x00\x00\x00\x01\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// ETACS_CGW
		ptMessage = ptCreateMessage("ETACS_CGW", 0x412, 20, 0, 6,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// VEH_CFG_08
		ptMessage = ptCreateMessage("VEH_CFG_08", 0x6F4, 500, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL);
		
		// VEH_CFG_05
		ptMessage = ptCreateMessage("VEH_CFG_05", 0x6F1, 500, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL); 
		
		// ZGW_248h
		ptMessage = ptCreateMessage("ZGW_248h", 0x248, 20, 0, 8,  (unsigned char *) "\x00\x00\x00\x00\x00\x00\x40\x00", NULL);
		iAddMessage(ptMessage, ptList, NULL); 
	}	
	
	// Configure restbus simulation CAN_2
	if (!ptList2)
	{ // Nur beim ersten Mal definieren, dann bleibt die Restbussimulation für die gesamte Software bestehen
		ptList2 = ptCreateNewEmptySimulationList();
	
		// 
		ptMessage2 = ptCreateMessage("SMR_SLAVE", 0x333, 40, 0, 4,  (unsigned char *) "\x00\x00\x00\x00", NULL);
		iAddMessage(ptMessage2, ptList2, NULL);
	}	
	
	
	// --- start restbus before power up --- 
	// == CAN Start diag ==
	if (!boCanInitAppl())
	{
		DataStorePutBool(2, 0);
		WriteToErrorWin("Init CAN Appl Error: %s", szGetErrorMessage(ptConnection));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	if (!boCanInitAppl2())
	{
		DataStorePutBool(2, 0);
		WriteToErrorWin("Init CAN_2 Appl Error: %s", szGetErrorMessage(ptConnection2));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	
	//  57000 Start restbus simulation
	boDefineCANMessages(ptConnection, ptList);
	boStartCANMessageSimulation(ptConnection);
	
	// CAN_2
	boDefineCANMessages(ptConnection2, ptList2);
	boStartCANMessageSimulation(ptConnection2);
	
	DataStorePutBool(2, 1);  
	

	// CAN1 Goepel to vehicle CAN switch  
	if (!boSetOnYavRel(CARD904X8_1, FixtureSlot_GetActivePublicCanRelay()))
	{
		WriteToErrorWin("Relay card YAV904X8 seems to be unfunctional!");
		return(errWhiteRelay + __LINE__); //whitelist - relay switching
	}
	Sleep(20);
	
	
	// 57010 Power up sensor
	// =====================
   if (!boHpPsSetOutput(&g_HpSupplyPrimary, On) || !boHpPsSetOutput(&g_HpSupplySecondary, On))
	{
		WriteToErrorWin("Power supply seems to be unfunctional!");  
		DataStorePutBool(10, 0); 
		return(errWhitePSupply + __LINE__); //whitelist - power supply related
	}
	

	
	// Start Application 
	dApplStartTime = Timer();		// in s

	DataStorePutBool(10, 1); 
	
	
	
	// 57020 Delay damit BG sicher da is - spezifiziert im Endtestparameter File - 3 s
	// ====================================
	Delay(g_tEtFileParameter.g_iEtpApplRunTimeSec);
		
	// Check for correct application runtime
	dApplActRunTime = (Timer() - dApplStartTime);
	
	if( dApplActRunTime >= g_tEtFileParameter.g_iEtpApplRunTimeSec ) 
	{
		DataStorePutBool(11, 1); 
	} 
	else 
	{
		SetErrorTestStep(iId, 11, "Error!!! Application runtime to short!");
		DataStorePutBool(11, 0); 
		return(errBlackRuntime - __LINE__); //blacklist - application runtime too short
	}
	
	
	// Get error memory adresses from application-a2l-file
	ptA2LFile = ptLoadA2LFile(g_tEtPathName.g_szAPPLA2L);
	for(iCnt = 0; iCnt < 30; iCnt++) {
		sprintf(szErrCodeId, szErrCodeIdFormat, iCnt);
		ptA2LBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "CHARACTERISTIC", szErrCodeId);
		if(!ptA2LBlock) {// Block not found
			FreeA2LBlock(ptA2LBlock);
			FreeA2LFile(ptA2LFile);
			return(errWhiteA2LFile + __LINE__); //whitelist - A2L file related
		}
		memset(szErrMemAddr, 0, sizeof(szErrMemAddr));
		boResult = boGetParameter(ptA2LBlock, "VALUE", 1, sizeof(szErrMemAddr), szErrMemAddr);
		if (!boResult) { // Parameter not found
			FreeA2LBlock(ptA2LBlock);
			FreeA2LFile(ptA2LFile);
			return(errWhiteA2LFile + __LINE__); //whitelist - A2L file related
		}
		auiErrMemAddr[iCnt] = ulHexToLong(szErrMemAddr);
		FreeA2LBlock(ptA2LBlock);
		
	}
	
	FreeA2LFile(ptA2LFile);

	
	
	// 57030: Verify HW version / SW version / HW part number	 
	// ====================================
	boResult = boStartDiag(ptConnection);
	if (!boResult)
	{
		DataStorePutBool(20, 0);
		WriteToErrorWin("Start Diag Error: %s", szGetErrorMessage(ptConnection));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	
	// Delay wegen SW routine die Diag blockiert
	// Sleep(1000); // from SRR2xx -> is it still necessary??
	
	
	//57030 Verify HW version / SW Version / HW Part Number
	// ====================================
	uDataSize = sizeof(aucRxData);
	memset(aucRxData, 0, sizeof(aucRxData));
	boResult = boRawDiagRequest(ptConnection, 2, (unsigned char *)"\x1A\x87", &uDataSize, aucRxData);
	if (!boResult){
		DataStorePutBool(20, 0);
		DataStorePutBool(21, 0);
		DataStorePutBool(22, 0);
		SetErrorTestStep(iId, 20, "Error on CAN communication");
		WriteToErrorWin("CAN seems to be unfunctional!");
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else if (uDataSize < 7){
		DataStorePutBool(20, 0);
		DataStorePutBool(21, 0);
		DataStorePutBool(22, 0);
		SetErrorTestStep(iId, 20, "Error on CAN communication: Invalid response size (%d < 4), %s", uDataSize, szHexToString(aucRxData, uDataSize));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else if (memcmp(szHexToString("\x5A\x87\x04\0x9D\0x00\0x00\0xFF",7), szHexToString(aucRxData, uDataSize), 7) != 0){
		DataStorePutBool(20, 0);
		DataStorePutBool(21, 0);
		DataStorePutBool(22, 0);
		SetErrorTestStep(iId, 20, "Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else {
		sprintf(szHWVersion, "%02X.%02X", aucRxData[7], aucRxData[8]);
		DataStorePutString(20, szHWVersion);
		
		sprintf(szSWVersion, "%02X.%02X.%02X", aucRxData[9], aucRxData[10], aucRxData[11]);
		DataStorePutString(21, szSWVersion);
		
		sprintf(szHWPartNo, "%c%c%c%c%c%c%c%c", aucRxData[12], aucRxData[13], aucRxData[14], aucRxData[15], aucRxData[16], aucRxData[17], aucRxData[18], aucRxData[19]);
		DataStorePutString(22, szHWPartNo);
	}

	//57040 Verify SW Part Number 
	// ====================================
	uDataSize = sizeof(aucRxData);
	memset(aucRxData, 0, sizeof(aucRxData));
	boResult = boRawDiagRequest(ptConnection, 2, (unsigned char *)"\x1A\x9C", &uDataSize, aucRxData);
	if (!boResult){
		DataStorePutBool(23, 0);
		SetErrorTestStep(iId, 23, "Error on CAN communication");
		WriteToErrorWin("CAN seems to be unfunctional!");
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else if (uDataSize < 5){
		DataStorePutBool(23, 0);
		SetErrorTestStep(iId, 23, "Error on CAN communication: Invalid response size (%d < 4), %s", uDataSize, szHexToString(aucRxData, uDataSize));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else if (memcmp(szHexToString("\x5A\x9C\x01\0x04\0x9D",5), szHexToString(aucRxData, uDataSize), 5) != 0){
		DataStorePutBool(23, 0);
		SetErrorTestStep(iId, 23, "Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else {
		sprintf(szSWPartNo, "%c%c%c%c%c%c%c%c", aucRxData[11], aucRxData[12], aucRxData[13], aucRxData[14], aucRxData[15], aucRxData[16], aucRxData[17], aucRxData[18]);
		DataStorePutString(23, szSWPartNo);
	}
	
	//57050 Verify ECU serial number 
	// ====================================
	uDataSize = sizeof(aucRxData);
	memset(aucRxData, 0, sizeof(aucRxData));
	boResult = boRawDiagRequest(ptConnection, 2, (unsigned char *)"\x21\xE1", &uDataSize, aucRxData);
	if (!boResult){
		DataStorePutBool(24, 0);
		SetErrorTestStep(iId, 24, "Error on CAN communication");
		WriteToErrorWin("CAN seems to be unfunctional!");
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else if (uDataSize < 2){
		DataStorePutBool(24, 0);
		SetErrorTestStep(iId, 24, "Error on CAN communication: Invalid response size (%d < 4), %s", uDataSize, szHexToString(aucRxData, uDataSize));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else if (memcmp(szHexToString("\x61\x1E",2), szHexToString(aucRxData, uDataSize), 2) != 0){
		DataStorePutBool(24, 0);
		SetErrorTestStep(iId, 24, "Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	else {
		sprintf(szECUSN, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", aucRxData[2], aucRxData[3], aucRxData[4], aucRxData[5], aucRxData[6], aucRxData[7], aucRxData[8], aucRxData[9], aucRxData[10], aucRxData[11], aucRxData[12], aucRxData[13], aucRxData[14], aucRxData[15]);
	
		if (strncmp(szHexToString(&(g_UnitInfo.szUnitIdFin[12]),14), szHexToString(szECUSN,14), 14)!= 0){
			DataStorePutBool(24, 0);
			SetErrorTestStep(iId, 24, "Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
			return(errBlackCANrel - __LINE__); //blacklist - CAN-related
		}
		else {
			 DataStorePutBool(24, 1); 
		}
		
		memset(szDiagRes, 0, sizeof(szDiagRes) );

		iResCnt = 0;
		for (iCnt = 0; iCnt < uDataSize; iCnt++) {
			sprintf(&szDiagRes[iResCnt], "0x%02X ", aucRxData[iCnt]);
			iResCnt = iResCnt + 5;
		}
		szDiagRes[iResCnt-1] = '\0';

		eva_SetTestIdTextValue(g_sSlot, g_sIdx, g_sRun, "90,24", szDiagRes);
	
	}
	
	
	//V42H - delay from Test Plan Parameters for longer restbus simulation
	// if "DELAY_RESTBUS" parameter is not found, default return is "0" = no delay
	delayFromParameter = atof(szGetTestplanParameterText( g_sSlot, g_sIdx, "DELAY_RESTBUS", "0" ));
	WriteToDataWin("Delay is: %.3f s", delayFromParameter);
	Delay(delayFromParameter); 
	
	// 57040 Stop Rest bus Simulation 
	boResult = boStopCANMessageSimulation(ptConnection);
	if(!boResult)
	{
		DataStorePutBool(12, 0);
		return (__LINE__);
	}
	boResult = boStopCANMessageSimulation(ptConnection2);
	if(!boResult)
	{
		DataStorePutBool(12, 0);
		return (__LINE__);
	}
	 
	DataStorePutBool(12, 1); 
	
	// 57060: Read Error Memory via XCP commands
	// ========================
	boStopDiag(ptConnection);
	
	// Setup XCP Connection
	iError = iXcpOpen(ptConnection->hPort, 500000);
	if( iError ) {
		CanShowError();
		iXcpClose();
		boErrorOnConnect = 1;
	}
	iError = iXcpInit(0x6C2, 0x6C9, False);
	if( iError ) {
		CanShowError();
		iXcpClose();
		boErrorOnConnect = 1;
	}
	iError = iXcpConnect();
	if( iError ) {
		CanShowError();
		iXcpClose();
		boErrorOnConnect = 1;
	}
	
	if(boErrorOnConnect) {
		return(errBlackXCPrel - __LINE__); //blacklist - XCP-related
	}
	
	
	
	
	for(iCnt = 0; iCnt < 30 ; iCnt++) {
		
		
		

		
		iError = iXcpSetMta(auiErrMemAddr[iCnt]);
		if( iError ) {
			CanShowError();
			iXcpClose();
			boErrorOnRead = 1;
			break;
		}
		
		
		

		
		
		iError = iXcpUploadWord(aucRxData);
		if( iError ) {
			CanShowError();
			iXcpClose();
			boErrorOnRead = 1;
			break;
		}
		usEvent = (aucRxData[0] * 256) + aucRxData[1];
		ausErrMemEvents[iCnt] = usEvent;
	}
	
	if(boErrorOnRead) {
		return(errBlackErrRead - __LINE__); //blacklist - error reading related
	}

	DataStorePutBool(30, 1);
	

	iError = iXcpDisConnect();
	if( iError ) {
		CanShowError();
		iXcpClose();
		boErrorOnDisconnect = 1;
	}
	
	iError = iXcpReset();
	if( iError ) {
		CanShowError();
		iXcpClose();
		boErrorOnDisconnect = 1;
	}
	
	iError = iXcpClose();
	if( iError ) {
		CanShowError();
		boErrorOnDisconnect = 1;
	}
	
	
	if(boErrorOnDisconnect) {
		return(errBlackXCPrel - __LINE__); //blacklist - XCP-related  
	}
	

	// 57070: Check allowance of Event IDs / Check allowed error codes
	// ========================
	boErrorOnCheck = 0;
	for(iCnt = 0; iCnt < 30; iCnt++) {
		boAllowed = 0;
		for(iCntOne = 0; iCntOne < g_tEtFileParameter.g_iValCntAllowed; iCntOne++) {
			if( ausErrMemEvents[iCnt] == g_tEtFileParameter.g_aiEtpAllowedErrorCodes[iCntOne] ) {
				boAllowed = 1;
				break;
			} 
		}
		if (!boAllowed) {
			ausNotAllowedErrorCodes[iCntTwo] = ausErrMemEvents[iCnt];
			boErrorOnCheck = 1;
			WriteToErrorWin("Not allowed error code: %d, 0x%3x", ausNotAllowedErrorCodes[iCntTwo], ausNotAllowedErrorCodes[iCntTwo]);
			//fprintf(tmpFile, "%d; ", ausNotAllowedErrorCodes[iCntTwo]);
			//iCntTwo++;
			
		}
	}

	if( boErrorOnCheck == 0 ) 
	{
		DataStorePutBool(40, 1);
	}
	else
	{
		DataStorePutBool(40, 0);
	}
	
	

	// 57080: Check required Event IDs / Check required error codes
	// ============================		 
	boErrorOnCheck = 0;
	if( g_tEtFileParameter.g_iValCntRequired != 0 ) {  
		for( iCnt = 0; iCnt < g_tEtFileParameter.g_iValCntRequired; iCnt++ ) {
			boRequired = 0;
			for( iCntOne = 0; iCntOne < 30; iCntOne++ ) {
				if( g_tEtFileParameter.g_aiEtpRequiredErrorCodes[iCnt] == ausErrMemEvents[iCntOne] ) {
					boRequired = 1;
					break;
				}
			}
		}
		if( !boRequired ) {
			boErrorOnCheck = 1;
			WriteToErrorWin("Not all required error codes present");
		}
	}
	
	if( boErrorOnCheck == 0 ) 
	{
		DataStorePutBool(45, 1);
	}
	else
	{
		DataStorePutBool(45, 0);
	}
	
	
	// 57090: Save all Event IDs read from the error memory / Write error memory content to file and store to MES
	// ================================================
	tmpFile = fopen(szErrMemContentTxt, "w+");
	if(!tmpFile) {
		DataStorePutBool(50, 0);
		SetErrorTestStep(iId, 50, "Not able to open or create file: %s", szErrMemContentTxt);
		return(errBlackErrRead - __LINE__); //blacklist - error reading related  
	}
	for(iCnt = 0; iCnt < 30; iCnt++) {
		fprintf(tmpFile, "%d; ", ausErrMemEvents[iCnt]);
	}
	
	fclose(tmpFile);

	boResult = boStoreResultData(szErrMemContentTxt, "01", "Error memory content");
	DataStorePutBool(50, boResult);
	if(!boResult) {
		SetErrorTestStep(iId, 50, "Not able to write error memory content to MES: %s", szErrMemContentTxt);
		return(errBlackErrRead - __LINE__); //blacklist - error reading related  
	}
	
	DeleteFile(szErrMemContentTxt);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	boResult = boStartDiag(ptConnection);
	if (!boResult)
	{
		WriteToErrorWin("Start Diag Error: %s", szGetErrorMessage(ptConnection));
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related
	}
	
	
	

	
	// 57100: Disable XCP access
	// ================================================
	uDataSize = sizeof(aucRxData);
	memset(aucRxData, 0, sizeof(aucRxData));
	boResult = boRawDiagRequest(ptConnection, 2,  (unsigned char *) "\x10\x92", &uDataSize, aucRxData);
	if (!boResult)
	{
		WriteToErrorWin("CAN seems to be unfunctional: %s", szGetErrorMessage(ptConnection));
		DataStorePutBool(80, 0);
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related  
	}
	else if (uDataSize != 2)
	{
		WriteToErrorWin("Error while disabling XCP Accsess: Invalid response size (%d != 2), %s", uDataSize, szHexToString(aucRxData, uDataSize));
		DataStorePutBool(80, 0);
		return(errBlackXCPrel - __LINE__); //blacklist - XCP-related  
	}
	else if (memcmp(szHexToString("\x50\x92",2), szHexToString(aucRxData, uDataSize), 2) != 0)
	{
		WriteToErrorWin("Error while disabling XCP Accsess: Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
		DataStorePutBool(80, 0);
		return(errBlackXCPrel - __LINE__); //blacklist - XCP-related  
	}
	else
	{	
		DataStorePutBool(80, 1); 	
	}
	
	memset(szDiagRes, 0, sizeof(szDiagRes) );

	iResCnt = 0;
	for (iCnt = 0; iCnt < uDataSize; iCnt++) {
		sprintf(&szDiagRes[iResCnt], "0x%02x ", aucRxData[iCnt]);
		iResCnt = iResCnt + 5;
	}
	szDiagRes[iResCnt-1] = '\0';
	
	eva_SetTestIdTextValue(g_sSlot, g_sIdx, g_sRun, "90,80", szDiagRes);

	
	
	
	
	uDataSize = sizeof(aucRxData);
	memset(aucRxData, 0, sizeof(aucRxData));
	boResult = boRawDiagRequest(ptConnection, 3,  (unsigned char *) "\xBA\x82\x00", &uDataSize, aucRxData);
	if (!boResult)
	{
		WriteToErrorWin("CAN seems to be unfunctional: %s", szGetErrorMessage(ptConnection));
		DataStorePutBool(81, 0);
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related  
	}
	else if (uDataSize != 3)
	{
		WriteToErrorWin("Error while disabling XCP Accsess: Invalid response size (%d != 3), %s", uDataSize, szHexToString(aucRxData, uDataSize));
		DataStorePutBool(81, 0);
		return(errBlackXCPrel - __LINE__); //blacklist - XCP-related  
	}
	else if (memcmp(szHexToString("\xFA\x82\x00",3), szHexToString(aucRxData, uDataSize), 3) != 0)
	{
		WriteToErrorWin("Error while disabling XCP Accsess: Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
		DataStorePutBool(81, 0);
		return(errBlackXCPrel - __LINE__); //blacklist - XCP-related  
	}
	else
	{	
		DataStorePutBool(81, 1); 	
	}
	
	memset(szDiagRes, 0, sizeof(szDiagRes) );

	iResCnt = 0;
	for (iCnt = 0; iCnt < uDataSize; iCnt++) {
		sprintf(&szDiagRes[iResCnt], "0x%02X ", aucRxData[iCnt]);
		iResCnt = iResCnt + 5;
	}
	szDiagRes[iResCnt-1] = '\0';
	
	eva_SetTestIdTextValue(g_sSlot, g_sIdx, g_sRun, "90,81", szDiagRes);
	
	
	
	
	
	
	// 57110: Clear DTC using Diagnostics
	// ================================================
	uDataSize = sizeof(aucRxData);
	memset(aucRxData, 0, sizeof(aucRxData));
	boResult = boRawDiagRequest(ptConnection, 3,  (unsigned char *) "\x14\xFF\x00", &uDataSize, aucRxData);
	
	//-------------------------------------------------------------------------
	// Time from after receiveing the Positive Response from “Clear Fault” request to Switch off the power to the sensor 
	// Must be shorter than 1 s
	dApplStartTime = 0.0;
	dApplStartTime = Timer();		// in s
	//-------------------------------------------------------------------------
	
	if (!boResult)
	{
		WriteToErrorWin("CAN seems to be unfunctional: %s", szGetErrorMessage(ptConnection));
		DataStorePutBool(90, 0);
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related  
	}
	else if (uDataSize != 3)
	{
		WriteToErrorWin("Error while disabling deleting DTCs: Invalid response size (%d != 3), %s", uDataSize, szHexToString(aucRxData, uDataSize));
		DataStorePutBool(90, 0);
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related  
	}
	else if (memcmp(szHexToString("\x54\xFF\x00",3), szHexToString(aucRxData, uDataSize), 3) != 0)
	{
		WriteToErrorWin("Error while disabling deleting DTCs: Error on CAN communication: Negative or invalid response: %s", szHexToString(aucRxData, uDataSize));
		DataStorePutBool(90, 0);
		return(errBlackCANrel - __LINE__); //blacklist - CAN-related  
	}
	else
	{	
		DataStorePutBool(90, 1); 	
	}
	
	memset(szDiagRes, 0, sizeof(szDiagRes) );

	iResCnt = 0;
	for (iCnt = 0; iCnt < uDataSize; iCnt++) {
		sprintf(&szDiagRes[iResCnt], "0x%02X ", aucRxData[iCnt]);
		iResCnt = iResCnt + 5;
	}
	szDiagRes[iResCnt-1] = '\0';
	
	eva_SetTestIdTextValue(g_sSlot, g_sIdx, g_sRun, "90,90", szDiagRes);
	
	boStopDiag(ptConnection);
	
	// 57120 Power down sensor
	// =====================
   if (!boHpPsSetOutput(&g_HpSupplyPrimary, Off)|| !boHpPsSetOutput(&g_HpSupplySecondary, Off) )
	{
		WriteToErrorWin("Power supply seems to be unfunctional!");  
		DataStorePutBool(98, 0); 
		return(errWhitePSupply + __LINE__); //whitelist - power supply related
	}
		
	//-----------------------------------------------------------------------
	// Check for correct power-down-time
	dApplActRunTime = 0.0;
	dApplActRunTime = (Timer() - dApplStartTime);
	
	if( dApplActRunTime < 0.5 ) 
	{
		DataStorePutBool(98, 1);
	} 
	else 
	{
		WriteToErrorWin("Power down took to long: %f -> Switch off the power to the sensor, within 0,5 second after receiveing the Positive Response from 'Clear Fault' request", dApplActRunTime);  
		DataStorePutBool(98, 0);
		return(errWhitePSupply + __LINE__); //whitelist - power supply related
	}
	//-----------------------------------------------------------------------
	
	
	Delay(0.300);
	

	// == Configure Hardware ==
	//  vehicle CAN switch off
	if (!boSetOffYavRel(CARD904X8_1, FixtureSlot_GetActivePublicCanRelay())) {
		DataStorePutBool(99, 0);
		SetErrorTestStep(iId, 99, "Error on disconnecting Goepel CAN!");
		return(errWhiteRelay + __LINE__); //whitelist - relay switching
	}
	Sleep(20);

	
	
	
	// =====================
	// --- close CAN after power down ---
	CanCloseAppl();
	CanCloseAppl2();   
	DataStorePutBool(99, 1);
	
	if (!boSetOffYavRel(CARD904X8_2, FixtureSlot_GetActivePrivateCanRelay()))
	{
		return( errWhiteRelay + __LINE__);
	}	
	
	#ifdef TIME
	// calculate modultime
	WriteToDataWin("F90: %.3f s", Timer() - act_time);   
	sumOfTestTime += Timer() - act_time;
	WriteToDataWin("sum of TestTime for F10....F90: %.3f s", sumOfTestTime);   
	#endif

	return( 0 );
}
//**********************************************************************************************************************************************

int DefineMeasArray (void)
{
	int iFctCnt = 0;
	
	memset (g_MeasArray, 0, sizeof g_MeasArray);
	
	g_MeasArray [iFctCnt].ID = 1 ;	g_MeasArray [iFctCnt++].pFct = i_F01;  
	g_MeasArray [iFctCnt].ID = 10;	g_MeasArray [iFctCnt++].pFct = i_F10;
	g_MeasArray [iFctCnt].ID = 90;	g_MeasArray [iFctCnt++].pFct = i_F90;
	
	return SetMeasFuncArr (g_MeasArray, iFctCnt);
}
