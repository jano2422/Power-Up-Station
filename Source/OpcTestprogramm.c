#include <CviTypes.h>
#include <cvirte.h>     /* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
#include <ansi_c.h>
#include <cviauto.h>
#include <dataskt.h>
#include <utility.h>
#include <toolbox.h>
#include <formatio.h>
#include <IniData.h>
#include <OpcTestprogramm.h>

typedef enum {eOpcUndef = 0, eOpcFalse = 1, eOpcTrue = 255} EOpcBoolean;

HRESULT hrOpcErr = S_FALSE;
static CRITICAL_SECTION OpcCriticalSection;

//***********************************************************
// static OPC Variables
static char szOpcTempSn[OpcStringLength] = {"000000000"};
static char szOpcFinalSn[OpcStringLength] = {"000000000"};  
static char szOpcCustomSn[OpcStringLength] = {"000000000"};
static char szSapMaterial[OpcStringLength] = {0};
//***********************************************************

TOpcData g_tOpcData[] = {
	//********************************************************************
	// Static OPC Variables
	0, True, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"SPS_Ready",
	0, True, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"Start_Test",
	0, False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	0, 0.0, szOpcTempSn,
	"TempSN",  
	0, False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	0, 0.0, szOpcFinalSn,
	"FinalSN",  
	0 ,False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"Tester_Ready",
	0, False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"Test_PASS",
	0, False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"Test_FAIL",
	//********************************************************************
	// specific OPC Variables   
	0, True, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"Adaptor_Id",   // angepasst
	0, True, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	False, 0.0, NULL,
	"Connector_Id",  // angepasst
	0, False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	0, 0.0, szOpcFinalSn,
	"CustomSN",  	 // angepasst
	0, False, DSConst_Unconnected, DSConst_ReadWriteAutoUpdate, CAVT_EMPTY,
	0, 0.0, szSapMaterial,
	"SapMaterial",  
	//********************************************************************      
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
extern void WriteToDataWin(char *pFormat, ...);
extern void WriteToErrorWin(char *pFormat, ...);

/* This is the callback function for the DataSocket */
extern void OpcCallback (DSHandle localDSHandle, int event, void *ptrUserData);
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void OpcShowError(HRESULT errorCode)
{
	char errBuf[300];
	char szString[300];

	DS_GetLibraryErrorString (errorCode, errBuf, 300);
	sprintf(szString, "Opc-Error: %s",errBuf); 
	WriteToErrorWin(szString);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//	Note that this routine works similarly to the commonly known printf() function with variable length argument list!
void OpcWriteError(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);

	WriteToErrorWin("Opc-Error: %s", szText);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcLoadIniData(void)
{
	static Boolean boFirstRun = False;

	int iZaehler = 0;
	char szTempFileName[MAX_PATHNAME_LEN];
	char szPcName[255], szPreFixOut[255], szPreFixIn[255], szStation[255];
	char szVariable[255];

	if( !boFirstRun ) {
		GetDir (szTempFileName);
		strcat(szTempFileName, "\\OpcConfig.ini");
		if( !boIniOpen(szTempFileName) ) {
			return( False );
		}
		if( !boIniGetStringItem("OPC", "PcName", szPcName) ) {
			boIniClose();
			return( False );
		}
		if( !boIniGetStringItem("OPC", "PreFixOut", szPreFixOut) ) {
			boIniClose();
			return( False );
		}
		if( !boIniGetStringItem("OPC", "PreFixIn", szPreFixIn) ) {
			boIniClose();
			return( False );
		}
		if( !boIniGetStringItem("OPC", "Station", szStation) ) {
			boIniClose();
			return( False );
		}
		boIniClose();

		for( iZaehler = 0; iZaehler < eSignalAmount; iZaehler++ ) {
			strcpy(szVariable, g_tOpcData[iZaehler].Url);
			if( g_tOpcData[iZaehler].boOut ) {
				sprintf(g_tOpcData[iZaehler].Url, "%s%s%s%s", szPcName, szPreFixOut, szStation, szVariable);
			}
			else {
				sprintf(g_tOpcData[iZaehler].Url, "%s%s%s%s", szPcName, szPreFixIn, szStation, szVariable);
			}
		}
		boFirstRun = True;
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcInit(void)
{
	int iZaehler = 0;

	if( !boOpcLoadIniData() ) {
		return( False );
	}

	// Initialize Critical Section
	InitializeCriticalSection(&OpcCriticalSection);

	for( iZaehler = 0; iZaehler < eSignalAmount; iZaehler++ ) {
		hrOpcErr = DS_OpenEx (	g_tOpcData[iZaehler].Url,
								g_tOpcData[iZaehler].AccessMode, OpcCallback,
								(void*)&g_tOpcData[iZaehler],
								DSConst_EventModel, 50,
								&g_tOpcData[iZaehler].Handle	);
		Sleep(5);
		ProcessSystemEvents();						
		if( !SUCCEEDED(hrOpcErr) ) {
			OpcShowError(hrOpcErr);
			return( False );
		}
		Sleep(5);
		DS_GetLastError (g_tOpcData[iZaehler].Handle, &hrOpcErr);
		if( !SUCCEEDED(hrOpcErr) ) {
			OpcShowError(hrOpcErr);
			return( False );
		}
	}

	if( !boOpcIsQualityGood() ) {
		return( False );
	}

	return( True );
	//return( boOpcReset() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void OpcClose(void)
{
	int iZaehler = 0;

	for( iZaehler = 0; iZaehler < eSignalAmount; iZaehler++ ) {
		if( g_tOpcData[iZaehler].Handle != 0 ) {
			DS_DiscardObjHandle (g_tOpcData[iZaehler].Handle);
			g_tOpcData[iZaehler].Handle = 0;
			g_tOpcData[iZaehler].Status = DSConst_Unconnected;
		}
	}
	// Delete Critical Sections
	DeleteCriticalSection(&OpcCriticalSection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcReset(void)
{
	if( !boOpcSetTesterReady(Off) ) {
		return( False );
	}
	if( !boOpcSetTestPass(Off) ) {
		return( False );
	}
	if( !boOpcSetTestFail(Off) ) {
		return( False );
	}
	return( True );
}


//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcIsQualityGood(void)
{
	long lQuality = 0;

	hrOpcErr = DS_GetAttrValue (g_tOpcData[SPS_Ready].Handle, "Quality", CAVT_LONG,
															&lQuality, sizeof(lQuality), NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( lQuality == DSConst_OPC_QUALITY_GOOD );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetSpsReady(TSwitch tOnOff)
{
	short sOnOff = (tOnOff == On) ? -1 : 0;

	hrOpcErr = DS_SetDataValue (g_tOpcData[SPS_Ready].Handle, g_tOpcData[SPS_Ready].DataType, &sOnOff, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetStartTest(TSwitch tOnOff)
{
	short sOnOff = (tOnOff == On) ? -1 : 0;

	hrOpcErr = DS_SetDataValue (g_tOpcData[Start_Test].Handle, g_tOpcData[Start_Test].DataType, &sOnOff, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetTesterReady(TSwitch tOnOff)
{
	short sOnOff = (tOnOff == On) ? -1 : 0;

	hrOpcErr = DS_SetDataValue (g_tOpcData[Tester_Ready].Handle, g_tOpcData[Tester_Ready].DataType, &sOnOff, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetTestPass(TSwitch tOnOff)
{
	short sOnOff = (tOnOff == On) ? -1 : 0;

	hrOpcErr = DS_SetDataValue (g_tOpcData[Test_PASS].Handle, g_tOpcData[Test_PASS].DataType, &sOnOff, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetTestFail(TSwitch tOnOff)
{
	short sOnOff = (tOnOff == On) ? -1 : 0;

	hrOpcErr = DS_SetDataValue (g_tOpcData[Test_FAIL].Handle, g_tOpcData[Test_FAIL].DataType, &sOnOff, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetTempSn(const char *TempSn)
{
	hrOpcErr = DS_SetDataValue (g_tOpcData[TempSN].Handle, g_tOpcData[TempSN].DataType, TempSn, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetString( ESignal eSignal, const char *szString )
{
	hrOpcErr = DS_SetDataValue (g_tOpcData[eSignal].Handle, g_tOpcData[eSignal].DataType, szString, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}
	return( True );
}
//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetString( ESignal eSignal, char *szString )
{
	strcpy(szString, g_tOpcData[eSignal].szValue);
	return( True );
}
//////////////////////////////////////////////////////////////////////////////////////////////////         
char *szOpcGetString( ESignal eSignal )
{
	return( g_tOpcData[eSignal].szValue );
}
//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetBoolean( ESignal eSignal , TSwitch tOnOff )
{
	short sOnOff = (tOnOff == On) ? -1 : 0;
	hrOpcErr = DS_SetDataValue (g_tOpcData[eSignal].Handle, g_tOpcData[eSignal].DataType, &sOnOff, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}
	return( True );
}
//////////////////////////////////////////////////////////////////////////////////////////////////
TSwitch tOpcGetBoolean( ESignal eSignal )
{
	return( (g_tOpcData[eSignal].sValue == -1) ? On : Off);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetByteBoolean( ESignal eSignal, Boolean *bReturn )
{
	byte bTemp; 
	bTemp = g_tOpcData[eSignal].sValue;
	switch( bTemp ) {
		case eOpcTrue:
			*bReturn = True;
			return( True );
		case eOpcFalse:
			*bReturn = False;
			return( True );
		default:
			ProcessSystemEvents(); 
			Sleep(100);
			ProcessSystemEvents();
		break;
	}
	bTemp = g_tOpcData[eSignal].sValue;  
	switch( bTemp ) {
		case eOpcTrue:
			*bReturn = True;
			return( True );
		case eOpcFalse:
			*bReturn = False;
			return( True );
	}
	return( False );
}
///////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetByteBoolean( ESignal eSignal, Boolean bBoolean )
{
	byte bTemp;
	if( bBoolean ){
		bTemp = 255;
	}
	else{
		bTemp = 1;
	}
	hrOpcErr = DS_SetDataValue (g_tOpcData[eSignal].Handle, g_tOpcData[eSignal].DataType, &bTemp, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}

	return( True );
}
///////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetByte( ESignal eSignal, byte bByte )
{
	hrOpcErr = DS_SetDataValue (g_tOpcData[eSignal].Handle, g_tOpcData[eSignal].DataType, &bByte, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}
	return( True );
}
///////////////////////////////////////////////////////////////////////////////////////////////      
Boolean boOpcGetByte(ESignal eSignal, byte *bReturn)  
{
	*bReturn = g_tOpcData[eSignal].sValue;
	return( True );
}
///////////////////////////////////////////////////////////////////////////////////////////////      
byte bOpcGetByte(ESignal eSignal )  
{
	return( g_tOpcData[eSignal].sValue );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetInt( ESignal eSignal, int iVariable )
{
	hrOpcErr = DS_SetDataValue (g_tOpcData[eSignal].Handle, g_tOpcData[eSignal].DataType, &iVariable, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}
	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetInt( ESignal eSignal, int *iVariable )
{
	*iVariable = g_tOpcData[eSignal].sValue;
	return( True );
}
//////////////////////////////////////////////////////////////////////////////////////////////////
int iOpcGetInt(ESignal eSignal)
{
	return( g_tOpcData[eSignal].sValue );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetIntArray( ESignal eSignal, int iPosition, int *iReturn )
{
	*iReturn = g_tOpcData[eSignal + iPosition].sValue;
	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int iOpcGetIntArray( ESignal eSignal, int iPosition )
{
	return( g_tOpcData[eSignal + iPosition].sValue );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcSetIntArray( ESignal eSignal, int iPosition, int iInteger )     
{
	hrOpcErr = DS_SetDataValue (g_tOpcData[eSignal + iPosition].Handle, g_tOpcData[eSignal + iPosition].DataType, &iInteger, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return( False );
	}
	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetDoubleArray( ESignal eSignal, int iPosition, double *dReturn )
{
	*dReturn = g_tOpcData[eSignal + iPosition].dValue;
	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TSwitch tOpcIsSpsReady(void)
{
	return( (g_tOpcData[SPS_Ready].sValue == -1) ? On : Off);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TSwitch tOpcIsStartTest(void)
{
	return( (g_tOpcData[Start_Test].sValue == -1) ? On : Off);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TSwitch tOpcIsTesterReady(void)
{
	return( (g_tOpcData[Tester_Ready].sValue == -1) ? On : Off);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TSwitch tOpcIsTestPass(void)
{
	return( (g_tOpcData[Test_PASS].sValue == -1) ? On : Off);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
TSwitch tOpcIsTestFail(void)
{
	return( (g_tOpcData[Test_FAIL].sValue == -1) ? On : Off);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetTempSn(char *szTempSn)
{
	strcpy(szTempSn, g_tOpcData[TempSN].szValue);

	return( True );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
Boolean boOpcGetSapMaterial(char *szSapMaterial)
{
	strcpy(szSapMaterial, g_tOpcData[SapMaterial].szValue);

	return( True );
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void OpcUpdateData(DSHandle dsHandle, TOpcData *tData)
{
	short 	sValue = 0;
	double 	dValue = 0.0;
	float 	fValue = 0.0;         
	int 	iValue = 0;

	hrOpcErr = DS_GetDataType (dsHandle, &tData->DataType, NULL, NULL);
	if( !SUCCEEDED(hrOpcErr) ) {
		OpcShowError(hrOpcErr);
		return;
	}
	switch( tData->DataType ) {
		case CAVT_BOOL:
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, &sValue, NULL, 0, 0);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			tData->sValue = sValue;
		break;

		case CAVT_CSTRING:
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, (char *)tData->szValue, OpcStringLength, NULL, NULL);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
		break;

		case CAVT_SHORT:
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, &sValue, NULL, 0, 0);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			tData->sValue = sValue;
		break;

		case CAVT_UCHAR:
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, &sValue, NULL, 0, 0);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}

		case CAVT_INT:
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, &iValue, NULL, 0, 0);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			tData->sValue =  (short)iValue;     
			tData->dValue = (double)iValue;
		break;
		
		case CAVT_FLOAT  :
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, &fValue, NULL, 0, 0);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			tData->dValue = (double)fValue;
		break;
		
		
		case CAVT_DOUBLE  :
			hrOpcErr = DS_GetDataValue (dsHandle, tData->DataType, &dValue, NULL, 0, 0);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			tData->dValue = dValue;
		break;
		

		default:
		WriteToErrorWin("OPC Error Url:%s Type: %i",tData->Url ,tData->DataType);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/* This is the callback function for the DataSocket */
void OpcCallback (DSHandle localDSHandle, int event, void *ptrUserData)
{
	HRESULT hrErrorNo;
	TOpcData *tData = (TOpcData *)ptrUserData;
	char szMessage[1000];

	switch( event ) {
		case DS_EVENT_DATAUPDATED:
			if( localDSHandle != 0 && tData->Status == DSConst_ConnectionActive ) {
				EnterCriticalSection(&OpcCriticalSection);
				OpcUpdateData(localDSHandle, tData);
				LeaveCriticalSection(&OpcCriticalSection);
			}
		break;

		case DS_EVENT_STATUSUPDATED:
			hrOpcErr = DS_GetStatus (localDSHandle, &tData->Status);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			hrOpcErr = DS_GetLastError (localDSHandle, &hrErrorNo);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			hrOpcErr = DS_GetLastMessage (localDSHandle, szMessage, 1000);
			if( !SUCCEEDED(hrOpcErr) ) {
				OpcShowError(hrOpcErr);
				return;
			}
			//WriteToDataWin(szMessage);
			if( (tData->Status == DSConst_ConnectionError) || (hrErrorNo < 0) ) {
				OpcWriteError("A Error occured within the URL: %s\n"
											"The Message is \"%s\"", tData->Url, szMessage);
			}
			//hrOpcErr ist 75 wenn keine Connection erzielt werden kann, während dem Startup auch 25 und 37

		break;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

