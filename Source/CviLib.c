/********************************************************************************/
/*                                                                           		*/
/*  Source File: CviLib_V110.c    		                                   				*/
/*                                                                           		*/
/********************************************************************************

	Version 		Date				Author		 Changes
	------------------------------------------------
	V1.00		   						   					 Initial realization
	V1.10				05.12.2012  fehrmanng  Add function boWpmCheckUnitValid
*********************************************************************************

	Documentation:
	--------------
	WPM/EvaProd utilities

*********************************************************************************/
#include <cviauto.h>
#include <formatio.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include <definitions.h>
#include <FraMesLog.h>		// Must be before the native FraMes-Header
#include <WipMama.h>

#define _USE_EVA_ 1

#ifdef _USE_EVA_
	#include <EvaProd.h>
#endif

#include <ApplicationTools.h>
#include <CviLib.h>

extern short 	g_sSlot, g_sIdx, g_sRun;

//************************************************************************************************
StructUnitInfo g_UnitInfo = {"SMR_TMP_SN", {0}, "SMR_FIN_SN", {0}, {0}, {0}, '-', '-'};	
StructEvaProdData g_EvaProdData = {0, 0, 0};

//************************************************************************************************
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boWpmCheckUnitValid(StructUnitInfo *UnitInfo, char *pcUnitLocation) {
	
	//char cUnitLocation = 0;
	long lUnitQuantity = 0;
	//char szOrder[100];
	//char szUnitMaterial[100];
	
	char (*szpUnitIdTypeNext)[100] = {0}, (*szpUnitIdNext)[100] = {0};	// Pointer-Array of 100 Pointers

	memset(g_UnitInfo.szOrder,			0, sizeof(g_UnitInfo.szOrder));
	memset(g_UnitInfo.szUnitMaterial,	0, sizeof(g_UnitInfo.szUnitMaterial));
	memset(g_UnitInfo.szUnitIdFin,		0, sizeof(g_UnitInfo.szUnitIdFin));
	
	if( !wpm_GetUnitInfo(	UnitInfo->szUnitIdType, UnitInfo->szUnitId, UnitInfo->szUnitMaterial, UnitInfo->szOrder, pcUnitLocation, &lUnitQuantity) ) {
		/*************************** Rework branch *****************************************************/
		// Check SlotForUnit with UnitIdNext = usually FIN ID
		if( !wpm_UnitGetNextIds(UnitInfo->szUnitIdType, UnitInfo->szUnitId, &szpUnitIdTypeNext , &szpUnitIdNext) ) {
			WriteToErrorWin("boWpmCheckUnitValid: No UnitIdNext for UnitId: %s available", UnitInfo->szUnitId); 
			return( False );
		}
		if( strcmp(UnitInfo->szUnitIdTypeFin, szpUnitIdTypeNext[0]) != 0 ) {
			WriteToErrorWin("boWpmCheckUnitValid: UnitIdTypeNext \"%s\" does not match expected UnitIdType \"%s\".", szpUnitIdTypeNext[0], UnitInfo->szUnitIdTypeFin);
			return( False );
		}
		if( !wpm_GetUnitInfo(	szpUnitIdTypeNext[0], szpUnitIdNext[0], UnitInfo->szUnitMaterial, UnitInfo->szOrder, pcUnitLocation, &lUnitQuantity) ) {
			WriteToErrorWin("boWpmCheckUnitValid: Unit isn't active neiter on UnitId nor on UnitIdNext ");
			return( False );
		}
		if( !wpm_GetSlotForUnit(	szpUnitIdTypeNext[0], szpUnitIdNext[0]) ) {
			WriteToErrorWin("boWpmCheckUnitValid: No Slot for Unit with UnitIdNext: %s", szpUnitIdNext[0]);
			return( False );
		}
		strcpy(UnitInfo->szUnitIdFin, szpUnitIdNext[0]);
		
		UnitInfo->cUnitType = 'f';
		/*************************** Rework branch *****************************************************/ 
	}
	/*************************** Regular branch *****************************************************/
	// Check SlotForUnit with UnitId = TMP ID
	else {
		if( !wpm_GetSlotForUnit(UnitInfo->szUnitIdType, UnitInfo->szUnitId)) {
											//MuellerM02 25-02-2015: not neccessary @ SRR3xx??					
			//	/*************************** Rework branch with SEMI Rückbau *****************************************************/ 
				if( *pcUnitLocation == 'M' ){
					WriteToDataWinLog("boWpmCheckUnitValid: Unit is in MaMa, no Slot for Unit with UnitId: %s \nUnit will be processed until Line Controller", UnitInfo->szUnitId); 
					
					return( True );
				}
			//	/*************************** Rework branch with SEMI Rückbau *****************************************************/ 
				
			WriteToErrorWin("boWpmCheckUnitValid: No Slot for Unit with UnitId: %s", UnitInfo->szUnitId);
			return( False );
		}
		UnitInfo->cUnitType = 't';  
	}
	/*************************** Regular branch *****************************************************/
	
	if( strncmp(UnitInfo->szOrder, "T_", 2) == 0 ) {
		UnitInfo->cUnitClass = 'T';
	}
	else if( strncmp(UnitInfo->szOrder, "Q_", 2) == 0 ) {
		UnitInfo->cUnitClass = 'Q';
	}
	else {
		UnitInfo->cUnitClass = 'P';
	}
	
	
	
	return( True );	
}
/********************************************************************************/    
Boolean boWpmSwitchBackUnitId(StructUnitInfo *UnitInfo, Boolean boSwitchBack)
{
	char cUnitLocation = 0;
	long lUnitQuantity = 0;
	char (*szpUnitIdTypeNext)[100] = {0}, (*szpUnitIdNext)[100] = {0};	// Pointer-Array of 100 Pointers

	memset(g_UnitInfo.szOrder,			0, sizeof(g_UnitInfo.szOrder));
	memset(g_UnitInfo.szUnitMaterial,	0, sizeof(g_UnitInfo.szUnitMaterial));
	memset(g_UnitInfo.szUnitIdFin,		0, sizeof(g_UnitInfo.szUnitIdFin));

	if( !wpm_GetUnitInfo(	UnitInfo->szUnitIdType, UnitInfo->szUnitId,
												UnitInfo->szUnitMaterial, UnitInfo->szOrder, &cUnitLocation, &lUnitQuantity) ) {
		if( !wpm_UnitGetNextIds(UnitInfo->szUnitIdType, UnitInfo->szUnitId, &szpUnitIdTypeNext , &szpUnitIdNext) ) {		
			return( False );
		}
		if( strcmp(UnitInfo->szUnitIdTypeFin, szpUnitIdTypeNext[0]) != 0 ) {
			WriteToErrorWin("It is not possible to Switch back to \"%s\", because UnitIdTypes do not match", UnitInfo->szUnitIdType);
			return( False );
		}
		if( boSwitchBack ) {
			if( !wpm_UnitChangeId(szpUnitIdTypeNext[0], szpUnitIdNext[0], UnitInfo->szUnitIdType, UnitInfo->szUnitId) ) {
				wpm_ShowError();
				return( False );
			}
			if( !wpm_GetUnitInfo(	UnitInfo->szUnitIdType, UnitInfo->szUnitId,UnitInfo->szUnitMaterial, UnitInfo->szOrder, &cUnitLocation, &lUnitQuantity) ) {
				return( False );
			}
		}
		else {
			strcpy(UnitInfo->szUnitIdFin, szpUnitIdNext[0]);
			if( !wpm_GetUnitInfo(	UnitInfo->szUnitIdTypeFin, UnitInfo->szUnitIdFin,UnitInfo->szUnitMaterial, UnitInfo->szOrder, &cUnitLocation, &lUnitQuantity) ) {
				return( False );
			}
		}
	}
	 
	if( strncmp(UnitInfo->szOrder, "T_", 2) == 0 ) {
		UnitInfo->cUnitClass = 'T';
	}
	else if( strncmp(UnitInfo->szOrder, "Q_", 2) == 0 ) {
		UnitInfo->cUnitClass = 'Q';
	}
	else {
		UnitInfo->cUnitClass = 'P';
	} 
	
	return( True );
}

//************************************************************************************************
Boolean boWpmGetUnitIdByUnitIdFin(StructUnitInfo *UnitInfo)
{
	char cUnitLocation = 0;
	long lUnitQuantity = 0;
	char (*szpUnitIdTypeNext)[100] = {0}, (*szpUnitIdNext)[100] = {0};	// Pointer-Array of 100 Pointers

	if( !wpm_UnitGetLastIds(UnitInfo->szUnitIdTypeFin, UnitInfo->szUnitIdFin, &szpUnitIdTypeNext , &szpUnitIdNext) ) {		
		wpm_ShowError();
		return( False );
	}
	if( strcmp(UnitInfo->szUnitIdType, szpUnitIdTypeNext[0]) != 0 ) {
		WriteToErrorWin("It is not possible to read back \"%s\", because UnitIdTypes do not match", UnitInfo->szUnitIdType);
		return( False );
	}
	strcpy(UnitInfo->szUnitId, szpUnitIdNext[0]);
	if( !wpm_GetUnitInfo(	UnitInfo->szUnitIdTypeFin, UnitInfo->szUnitIdFin,
												UnitInfo->szUnitMaterial, UnitInfo->szOrder, &cUnitLocation, &lUnitQuantity) ) {
		return( False );
	}
	
	return( True );
}

//************************************************************************************************
Boolean boGetWpmUnitIds(char *szUnitIdTypeIn, char *szUnitIdTypeOut, char *szUnitIdIn, char *szUnitIdOut)
{
	char (*szpUnitIdTypeNext)[100] = {0}, (*szpUnitIdNext)[100] = {0};	// Pointer-Array of 100 Pointers

	if( !wpm_UnitGetLastIds(szUnitIdTypeIn, szUnitIdIn, &szpUnitIdTypeNext , &szpUnitIdNext) ) {		
		wpm_ShowError();
		return( False );
	}
	if( strcmp(szUnitIdTypeOut, szpUnitIdTypeNext[0]) != 0 ) {
		WriteToErrorWin("It is not possible to read back \"%s\", because UnitIdTypes do not match", szUnitIdTypeOut);
		return( False );
	}
	strcpy(szUnitIdOut, szpUnitIdNext[0]);
	
	return( True );
}

//************************************************************************************************
//************************************************************************************************
#ifdef _USE_EVA_
//************************************************************************************************
Boolean boGetTestPlanTestStepLimits(int TestId, int TestSubId, float *LSL, float *USL)
{
	eva_STRUCTTEST Test = {0};
	char szTestId[50] = {0};

	sprintf(szTestId, "%d,%d", TestId, TestSubId);
	if( !eva_GetTestPlanTest(g_EvaProdData.sSlot, g_EvaProdData.sIdx, szTestId, &Test) ) {
		eva_ShowError();
		return( False );
	}
	*LSL = Test.fLSL;
	*USL = Test.fUSL;
	
	return( True );
}



//************************************************************************************************
Boolean boGetTestValue(	const eva_STRUCTTESTVALUE *AllTestValues, short TestValues, 
												const char *TestId, float *TestValue)
{
	int iCount = 0;
	
	for( iCount = 0; iCount < TestValues; iCount++ ) {
		if( strcmp(AllTestValues[iCount].szTestId, TestId) == 0 ) {
			*TestValue = AllTestValues[iCount].fValue;
			return( True );
		}
	}
	
	return( False );
}

//************************************************************************************************
Boolean boGetSelectedTestValues(char *TestPlanGroup, char *TestPlanName, char *UnitIdType, char *UnitId,
																char *TestIds[], float *Values, int Ammount, Boolean MustFind)
{
	short sTestValueCount = 0;
	eva_STRUCTTESTVALUE *pAllTestValues = NULL;
	int iZaehler = 0;

	// Only the last Test-Plan test-value is returned by this function
	// extern short __stdcall eva_GetDBTestRunValues(char *szTestPlanGroup, char *szTestPlanName, char *szRunIdType, char *szRunId, eva_STRUCTTESTVALUE **pTestValues);
	//sTestValueCount = eva_GetDBTestRunValues(g_szUnitMaterial, "FCU_ICT", g_szUnitIdType, g_szUnitId, &pAllTestValues);
	//sTestValueCount = eva_GetDBTestRunValues(TestPlanGroup, TestPlanName, UnitIdType, "00000013", &pAllTestValues);
	sTestValueCount = eva_GetDBTestRunValues(TestPlanGroup, TestPlanName, UnitIdType, UnitId, &pAllTestValues);
	if( !sTestValueCount ) {
		if( MustFind ) {
			eva_ShowError();
			return( False );
		}
		else {
			return( True );
		}
	}
	for( iZaehler = 0; iZaehler < Ammount; iZaehler++ ) {
		// ACHTUNG, da die Werte sortiert nach szTestId kommen, also NICHT in der Reihenfolge vom Prüfplan
		//if( !boGetTestValue(pAllTestValues, sTestValueCount, "10,2", &fWert1) ) {
		if( !boGetTestValue(pAllTestValues, sTestValueCount, TestIds[iZaehler], &Values[iZaehler]) ) {
			WriteToErrorWin("Error, because no TestId \"%s\" found", TestIds[iZaehler]);
			return( False );
		}
	}
	
	return( True );
}

//************************************************************************************************
Boolean boCollectTextInfo(char *pFormat, ...)
{
	char szText[2*MAX_TEXT_INFO] = {0};
	int iTextInfoLen = strlen(g_szTextInfo);
	int iTextLen = 0;
	va_list lArgPointer;

	if( strlen(g_szTextInfo) > MAX_TEXT_INFO - 1 ) { 
		return( False );
	}
	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	iTextLen = strlen(szText);
	
	if( iTextInfoLen + iTextLen > MAX_TEXT_INFO - 1 ) {
		strncat(g_szTextInfo, szText, MAX_TEXT_INFO - iTextInfoLen - 1);
		return( False );
	}
	strcat(g_szTextInfo, szText);
	
	return( True );
}
#endif


//************************************************************************************************
//************************************************************************************************
/*
**	convert a given code back to integer (reverse of ConvertCode)
**
**	parameters:
**
**		const char *szCode
**			a pointer to the code to be converted back to integer
**
**		const char *szCharSet
**			a pointer to a string constant which must define the desired character set
**
**	returns:
**
**		the integer result of the conversion
**
**	example:
**
**		unsigned long ulValue;
**
**		ulValue = RevertCode (
**			"0012D67",							// the code to be converted back
**			"0123456789ABCDEF");		// let's take a simple HEX char set
**															// ulValue will be 1234567
*/
unsigned long RevertCode (const char *szCode, const char *szCharSet)
{
	unsigned long ulValue = 0;										// value of code (result)
	unsigned long ulRadix = strlen(szCharSet);		// radix of code
	unsigned long ulPos;													// digit position
	char szDigit [2] = {0};												// ASCII representation of digit
	unsigned long ulDigit;												// binary representation of digit
	unsigned long ulLen = strlen(szCode);				// length of code
	
	for( ulPos = 0; ulPos < ulLen; ulPos++ ) {
		szDigit[0] = szCode[ulPos];
		ulDigit = strstr(szCharSet, szDigit) - szCharSet;
		ulValue *= ulRadix;
		ulValue += ulDigit;
	}
	
	return( ulValue );
}

//************************************************************************************************
int iGetFinalSnData(int *Day, int *Month, int *Year, int *DayCounter)
{
	int iDay = 0, iMonth = 0, iYear = 0, i1Year = 0, iDayCounter = 0;
	char szCharSet[] = "0123456789ABCDEFGHJKLMNPRSTVWXYZ";
	char szCodedNumber[2] = {0, 0};
	char szOffset[] = "9";
	
	if( GetSystemDate (&iMonth, &iDay, &iYear) != 0 ) {
		return( 2 );
	}
	if( sscanf(&g_UnitInfo.szUnitIdFin[3], "%1d", &i1Year) != 1 ) {
		return( 4 );
	}
	if( i1Year > (iYear % 10) ) {
		iYear -= 10;
	}
	iYear /= 10;			// remove one
	iYear *= 10;
	iYear += i1Year;
	if( sscanf(&g_UnitInfo.szUnitIdFin[4], "%c", szCodedNumber) != 1 ) {
		return( 6 );
	}
	iMonth 	= RevertCode(szCodedNumber, szCharSet);
	iMonth -= RevertCode(szOffset, szCharSet);
	iDayCounter = 	RevertCode(&g_UnitInfo.szUnitIdFin[7], szCharSet);
	iDayCounter -=  RevertCode("9ZZ", szCharSet);
	if( sscanf(&g_UnitInfo.szUnitIdFin[5], "%02d", &iDay) != 1 ) {
		return( 8 );
	}

	*Day = iDay;
	*Month = iMonth;
	*Year = iYear;
	*DayCounter = iDayCounter;
	return( 0 );
}

#ifdef _WITH_MS_MONTH_VIEW
//************************************************************************************************
//************************************************************************************************
int iGetWeekOfYear(int Day, int Month, int Year, int *Week)
{
	static iStatus = 0;
	static CAObjHandle  ohMonthView = 0;
	static char szLine[256];

	if( ohMonthView == 0 ) {
		// Get the ObjectHandle of the MsMonthView-Object
		iStatus = GetObjHandleFromActiveXCtrl (g_hdlToolPanel, PNL_TOOLS_MS_MONTH_VIEW, &ohMonthView);
		if( iStatus < 0 ) {
			CA_GetAutomationErrorString (iStatus, szLine, 255);
			MessagePopup ("Error", szLine);
			return( 4 );
		}

	}


	//Set the Current-Date of the MonthView-Object to the System Date
	iStatus = MSComCtl2_SetIMonthViewProperty (ohMonthView, NULL, MSComCtl2_IMonthViewDay, CAVT_SHORT, Day);
	if( iStatus < 0 ) {
		CA_GetAutomationErrorString (iStatus, szLine, 255);
		MessagePopup ("Error", szLine);
		return( 6 );
	}
	iStatus = MSComCtl2_SetIMonthViewProperty (ohMonthView, NULL, MSComCtl2_IMonthViewMonth, CAVT_LONG, Month);
	if( iStatus < 0 ) {
		CA_GetAutomationErrorString (iStatus, szLine, 255);
		MessagePopup ("Error", szLine);
		return( 8 );
	}
	iStatus = MSComCtl2_SetIMonthViewProperty (ohMonthView, NULL, MSComCtl2_IMonthViewYear, CAVT_SHORT, Year);
	if( iStatus < 0 ) {
		CA_GetAutomationErrorString (iStatus, szLine, 255);
		MessagePopup ("Error", szLine);
		return( 10 );
	}

	// Obwohl nicht sichtbar, kann damit die z.B. Kalenderwoche vom Datum erfragt werden
	iStatus = MSComCtl2_GetIMonthViewProperty (ohMonthView, NULL, MSComCtl2_IMonthViewWeek, CAVT_SHORT, Week);
	if( iStatus < 0 ) {
		CA_GetAutomationErrorString (iStatus, szLine, 255);
		MessagePopup ("Error", szLine);
		return( 12 );
	}

	// Release the reference to the application object
	//if( ohMonthView != 0 ) {
	//	iStatus = CA_DiscardObjHandle (ohMonthView); 
	//}
	//ohMonthView = 0;
	
	return 0;
}

//************************************************************************************************
int iGetDateData(int Day, int Month, int Year, int *DayOfYear, int *WeekOfYear)
{
	int iRet = 0;
	int iDayOfYear = 0, iWeekOfYear = 0;
	time_t tTime = {0};
	struct tm tmData = {0};
	

	// Create DayOfYear (julian-calendar)
	//tmData = gmtime (&tTime);
	memset(&tmData, 0, sizeof(tmData));
	tmData.tm_mday = Day;
	tmData.tm_mon = Month - 1;
	tmData.tm_year = Year - 1900;
	tTime = mktime(&tmData);
	if( tTime == -1 ) {
		return( 2 );
	}
	iDayOfYear = tmData.tm_yday + 1;

	// Create WeekOfYear
	iRet = iGetWeekOfYear(Day, Month, Year, &iWeekOfYear);
	if( iRet != 0 ) {
		return( iRet );
	}
	
	*DayOfYear = iDayOfYear;
	*WeekOfYear = iWeekOfYear;
	return( 0 );
}
#endif

//************************************************************************************************
Boolean boCheckMutex(void)
{
	static HANDLE mutexHdl = NULL;
	char szStationId[100];

	if( mutexHdl != 0 ) {
		return( True );
	}
	//detect a previous instance of the application 
	// A named mutex is created
	strcpy(szStationId, wpm_GetCfg("STATION_ID", 1));
	mutexHdl = CreateMutex( NULL, TRUE, szStationId);
	// See if a previous mutex of the same name exists 
	if( GetLastError() == ERROR_ALREADY_EXISTS ) {
		// If so, notify the user and return 
	  MessageBox(NULL, TEXT("Another instance is running!"), TEXT("Application Message"), MB_OK);
		return( False );
	}
 
	return( True );
}
//************************************************************************************************
// Stores file to testrun
//************************************************************************************************
Boolean boStoreResultData(char szFileName[], char szID[], char szKommentar[])        
{
	//char 	szBlobData[4000]; // Auskommentiert, DJH@20111025
	int 	iRead;
	eva_STRUCTBLOBINFO BlobInfo;
	char 	*cResultData 	= NULL;
	int 	iFileSize 		= 0;
	int 	iFileHande 		= 0;

	//sprintf(szBlobData, "Binary BLOB Data Header and additional infos from MEAS_02 function");

	if( GetFileSize ( szFileName, &iFileSize) != 0 ){
		return( False );
	}
	cResultData = malloc( iFileSize * sizeof(char));     
	iFileHande = OpenFile ( szFileName, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);
	if ( !iFileHande) {
		free(cResultData);
		return( False );
	}
	iRead = ReadFile (iFileHande, cResultData, iFileSize);	
	strcpy(BlobInfo.szDataId,    szID);
	strcpy(BlobInfo.szMimeType,   "text/plain");
	strcpy(BlobInfo.szDataSource, szKommentar);
	BlobInfo.lDataSize = iRead;
	BlobInfo.pData = cResultData;
	if( !eva_SetBLOB(g_sSlot, g_sIdx, g_sRun, BlobInfo) ) {
		eva_ShowError();
		free(cResultData);
		return( False );
	}
	CloseFile(iFileHande);
	free(cResultData);
	
	return ( True );
}

Boolean boStoreResultDataEx(char szFileName[], char szID[], char szKommentar[], char szMimeType[])        
{
	//char 	szBlobData[4000]; // Auskommentiert, DJH@20111025
	int 	iRead;
	eva_STRUCTBLOBINFO BlobInfo;
	char 	*cResultData 	= NULL;
	int 	iFileSize 		= 0;
	int 	iFileHande 		= 0;

	//sprintf(szBlobData, "Binary BLOB Data Header and additional infos from MEAS_02 function");

	if( GetFileSize ( szFileName, &iFileSize) != 0 ){
		return( False );
	}
	cResultData = malloc( iFileSize * sizeof(char));     
	iFileHande = OpenFile ( szFileName, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);
	if ( !iFileHande) {
		free(cResultData);
		return( False );
	}
	iRead = ReadFile (iFileHande, cResultData, iFileSize);	
	strcpy(BlobInfo.szDataId,    szID);
	strcpy(BlobInfo.szMimeType,   szMimeType);
	strcpy(BlobInfo.szDataSource, szKommentar);
	BlobInfo.lDataSize = iRead;
	BlobInfo.pData = cResultData;
	if( !eva_SetBLOB(g_sSlot, g_sIdx, g_sRun, BlobInfo) ) {
		eva_ShowError();
		free(cResultData);
		return( False );
	}
	CloseFile(iFileHande);
	free(cResultData);
	
	return ( True );
}

