/*****************************************************************************/
/*                                                                           */
/*  File : ApplicationTools.CPP                                              */
/*                                                                           */
/*****************************************************************************

	documentation: 

	Date       Name                 Version  Text
	23.08.2005 dunzm                v1.0     Functions for additional Output-Window

******************************************************************************/
#include <cvitypes.h>
#include <windows.h>
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>   
#include <userint.h>
#include <ErrorLog.h>
#include <definitions.h>
#include <PnlTools.h>
#include <ApplicationTools.h>
#include <IniData.h> //for opening frames.cfg-file    

//****************************************************************************************
#define   MAX_SHOW_LINE    2000       // Maximum of lines to be stored in ListBox / ShowDataWin

//***************************************************************************************
int g_hdlToolPanel;								// Handle for Tool Panel
int g_hdlToolMenu;								// Handle for Tool Menu-Bar
int g_pnlDioTest;


//************************** function to get path from frames.cfg **********************************************************************   

// globale Variable for the local paths, specified in the frames.cfg - filled at ApplictionMode3	- used at test.c
 char g_szTemplateFolder[500];
 char g_szTempFileFolder[500];
 char g_szErrorFilePath [500];
 char g_szPowerUpTime[100];
 // RunSpsTestprogramm.c  
 char g_szTestPlanNameForResultFile[500];     
 extern char g_cOperatorMaterialSelection[100];
 extern char g_szMESSapMaterial[100];
 
 
//***************************************************************************************
#ifndef _WITHOUT_DIO_
	#ifndef _DIO_TEST_PORT_
		extern Boolean boDioTestInit(void);
	#else
		extern Boolean boDioTestInit(int iPort);
	#endif
#endif
//***************************************************************************************
//***************************************************************************************
void vShowResult(char Result)
{
	
	switch( Result ) {
		case 'R': 
			SetPanelAttribute (g_hdlToolPanel, ATTR_BACKCOLOR, VAL_YELLOW);
		break;

		case 'P': 
			SetPanelAttribute (g_hdlToolPanel, ATTR_BACKCOLOR, VAL_GREEN);
		break;

		case 'F': 
			SetPanelAttribute (g_hdlToolPanel, ATTR_BACKCOLOR, VAL_RED);
		break;

		case 'T': 
			SetPanelAttribute (g_hdlToolPanel, ATTR_BACKCOLOR, VAL_PANEL_GRAY);
		break;

		case 'M': 
			SetPanelAttribute (g_hdlToolPanel, ATTR_BACKCOLOR, VAL_BLUE);
		break;

		case 'E': 
			SetPanelAttribute (g_hdlToolPanel, ATTR_BACKCOLOR, VAL_MAGENTA);
		break;

	}
	ProcessDrawEvents();
}

//****************************************************************************************
void ResetDataWin(void)
{
	ClearListCtrl (g_hdlToolPanel, PNL_TOOLS_LB_SHOW_DATA);
}


//****************************************************************************************
void WriteDataWin(int sPanel, char *aInfo, int aColour)
// output to listbox
{
	char szEditColor[20];
	char szShowLine[2048];
	int iLineCount = 0;
	
	sprintf(szEditColor, "\033fg%06X", aColour);
	
	//// zur Sicherheit die Stringlänge von aInfo testen
	sprintf(szShowLine, "%s%s", szEditColor, aInfo);
	
	// Insert new line
	InsertListItem (sPanel, PNL_TOOLS_LB_SHOW_DATA, -1, szShowLine, 0);
	
	// Clear lines, if maximum was reached
	GetNumListItems (sPanel, PNL_TOOLS_LB_SHOW_DATA,  &iLineCount);
	if( iLineCount > MAX_SHOW_LINE ){
		DeleteListItem (sPanel, PNL_TOOLS_LB_SHOW_DATA, 0, 100);
	}

	// set focus to last line
	GetNumListItems (sPanel, PNL_TOOLS_LB_SHOW_DATA,  &iLineCount);
	SetCtrlIndex(sPanel, PNL_TOOLS_LB_SHOW_DATA, iLineCount - 1);
}


//****************************************************************************************
//	Note that this routine works similarly to the commonly known printf() function with variable length argument list!
void WriteToErrorWin(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	if( g_hdlToolPanel ) {
		WriteDataWin(g_hdlToolPanel, szText, VAL_MAGENTA);
	}
	else {
		MessagePopup ("Fehler", szText);
	}
	boErrorLog(szText);
}

//****************************************************************************************
//	Note that this routine works similarly to the commonly known printf() function with variable length argument list!
void WriteToErrorWinV(const char *pFormat, va_list lArgPointer)
{
	char szText[2048];

	vsprintf (szText, pFormat, lArgPointer);

	if( g_hdlToolPanel ) {
		WriteDataWin(g_hdlToolPanel, szText, VAL_MAGENTA);
	}
	else {
		MessagePopup ("Fehler", szText);
	}
	boErrorLog(szText);
}

//****************************************************************************************
void WriteToDataWin(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	WriteDataWin(g_hdlToolPanel, szText, VAL_BLACK);
}

//****************************************************************************************
void WriteToDataWinLog(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	WriteDataWin(g_hdlToolPanel, szText, VAL_DK_GREEN);
	boErrorLog(szText);
}

//****************************************************************************************
void WriteToDataWinPass(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	WriteDataWin(g_hdlToolPanel, szText, VAL_DK_GREEN);
	//boErrorLog(szText);
}

//****************************************************************************************
void WriteToDataWinFail(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	WriteDataWin(g_hdlToolPanel, szText, VAL_RED);
}

//****************************************************************************************
void WriteToDataWinMaterial(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	WriteDataWin(g_hdlToolPanel, szText, VAL_DK_CYAN);
}

//****************************************************************************************
void OpcWriteStatus(char *Message)
{
	#ifdef _CVI_DEBUG_
	WriteToDataWin("Opc-Status: %s", Message);
	#endif 
}

//****************************************************************************************
void WriteToStatusWin(char *pFormat, ...)
{
	va_list lArgPointer;
	char szText[2048];

	va_start (lArgPointer, pFormat);
	vsprintf (szText, pFormat, lArgPointer);
	va_end (lArgPointer);

	SetCtrlVal (g_hdlToolPanel, PNL_TOOLS_S_STATUS, szText);
}

/***************************************************************************/
void DisplayFraMesErrorText(char aText[])
{
	WriteToErrorWin("FramesError: %s",aText);
}

/***************************************************************************/
void G3DisplayError(char *szError)
{
	WriteToErrorWin("G3-Error: %s", szError);
}

/***************************************************************************/
void ShowTime(char szText[])
{
	static dStartTime = 0.0;
	
	if( szText[0] == 0 ) {
		dStartTime = Timer	();
	}
	else {
		char szLine[255] = {0};
		sprintf(szLine, "%s %2.3fs", szText, Timer() - dStartTime);
		//WriteToDataWin("%s %2.3fs", szText, Timer() - dStartTime);
		WriteToDataWin(szLine);
		//boErrorLog(szLine);
		ProcessDrawEvents();
	}
}

/***************************************************************************/
/***************************************************************************/
void CVICALLBACK MenuBarCallback (int menuBar, int menuItem, void *callbackData, int panel)
{
	char szLine[512] = {0}, szInput[20] = {0};
	int iCount = 0;
	static char szTaskBarButtonText[255];
	
	switch( menuItem ) {
		case MB_TOOLS_PMF_ONOFF:
			g_Pmf.boPmf ^= True;
			SetMenuBarAttribute (menuBar, menuItem, ATTR_CHECKED, g_Pmf.boPmf);
			if( g_Pmf.boPmf ) {
				memset(szTaskBarButtonText, 0, sizeof(szTaskBarButtonText));
				GetSystemAttribute (ATTR_TASKBAR_BUTTON_TEXT, szTaskBarButtonText);
				SetSystemAttribute (ATTR_TASKBAR_BUTTON_TEXT, "!!! PMFU !!!");
			}
			else {
				SetSystemAttribute (ATTR_TASKBAR_BUTTON_TEXT, szTaskBarButtonText);
			}
		break;
		case MB_TOOLS_PMF_COUNT:
			sprintf(szLine, "Please insert the amount of Test-Ability-Runs (currently: %d)", g_Pmf.iMaxCount);
			if( !PromptPopup ("Test-Ability-Runs", szLine, szInput, sizeof(szInput) - 1) ) {
				sscanf(szInput, "%d", &iCount);
				if( (iCount > 0) && (iCount < 100) ) {
					g_Pmf.iMaxCount = iCount;
				}
			}
			sprintf(szLine, "__Count (%d)", g_Pmf.iMaxCount);
			SetMenuBarAttribute (menuBar, menuItem, ATTR_ITEM_NAME, szLine);
		break;
	}
	
}



//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:  iGetPathsFromFrames
//
// Description: gets the program paths from Frames.cfg (TEMPLATE_FOLDER_PATH and TEMP_FILE_FOLDER_PATH
//							write to g_szTemplateFolder and g_szTempFileFolder 
//
// Parameters:
// char *  pszIniPath         the Path of the frames.cfg-File to read
//
// return value               0  if successful
//														-1 no file-names in frames.cfg	
//                            -2 if file is not found
//
//////////////////////////////////////////////////////////////////////////////////////////////
int iGetPathsFromFrames (char * pszIniPath)
{
		
	memset( g_szTemplateFolder, 0, sizeof(g_szTemplateFolder) );
	memset( g_szTempFileFolder, 0, sizeof(g_szTempFileFolder) );
	
	// get Pathnames
	if (!boIniOpen (pszIniPath)) {
		WriteToErrorWin("iGetPathsFromFrames: %s could not be opened", pszIniPath);
		return( -2 );
	}
	
	// get g_szTemplateFolder
	if (!boIniGetStringItem ("PATHS", "TEMPLATE_FOLDER_PATH", g_szTemplateFolder)) {
		WriteToErrorWin("Frames.cfg does not contain TEMPLATE_FOLDER_PATH, default values will be used");
		return( -1 );
	}
	
	WriteToDataWin("TEMPLATE_FOLDER_PATH    : %s", g_szTemplateFolder);
 
	// get g_szTempFileFolder
	if (!boIniGetStringItem ("PATHS", "TEMP_FILE_FOLDER_PATH", g_szTempFileFolder)) {
		WriteToErrorWin("Frames.cfg does not contain TEMP_FILE_FOLDER _PATH");
		return( -1 );
	}					 
	WriteToDataWin("TEMP_FILE_FOLDER_PATH   : %s", g_szTempFileFolder);
	
	// get g_szErrorFilePath
	if (!boIniGetStringItem ("PATHS", "ERRORLOG_PATH", g_szErrorFilePath)) {
		WriteToErrorWin("Frames.cfg does not contain ERRORLOG_PATH, default values will be used");
		return( -1 );
	}
	WriteToDataWin("ERRORLOG_PATH   : %s", g_szErrorFilePath);
	
	// get g_szPowerUpTime 
	if (!boIniGetStringItem ("CONFIGURATION_OF_SW", "POWER_UP_TIME", g_szPowerUpTime)) {
		WriteToErrorWin("Frames.cfg does not contain POWER_UP_TIME, default values will be used");
		return( -1 );
	}
	WriteToDataWin("ERRORLOG_PATH   : %i", g_szPowerUpTime);
	
	/*
	// get FIN serial 
	if (!boIniGetStringItem ("TEMP_SERIAL_CONTAINER", "SRR_FIN_SN", g_szFINserial)) {
		WriteToErrorWin("Frames.cfg does not contain SRR_FIN_SN, default values will be used");
		return( -1 );
	}
	WriteToDataWin("ERRORLOG_PATH   : %i", g_szFINserial);
	*/
	
	// close File
	boIniClose ();
	return 0;
	
}


//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:  iConfigureSwFromFrames
//
// Description: configure the program from Frames.cfg (OPC_ACTIVE and TEST_PLAN_NAME_FOR_RESULT_FILE
//							write to g_szOpcActive and g_szTestPlanNameForResultFile
//
// Parameters:
// char *  pszIniPath         the Path of the frames.cfg-File to read
//
// return value               0  if successful
//                           -2 if file is not found
//
//////////////////////////////////////////////////////////////////////////////////////////////
int iConfigureSwFromFrames (char * pszIniPath)
{
		
	memset( g_szTestPlanNameForResultFile, 0, sizeof(g_szTestPlanNameForResultFile) );
	
	// get Pathnames
	if (!boIniOpen (pszIniPath)) {
		WriteToErrorWin("iGetPathsFromFrames: %s could not be opened", pszIniPath);
		return( -2 );
	}
	
	// get g_szTestPlanNameForResultFile
	if (!boIniGetStringItem ("CONFIGURATION_OF_SW", "TEST_PLAN_NAME_FOR_RESULT_FILE", g_szTestPlanNameForResultFile)) {
		WriteToErrorWin("Frames.cfg does not contain TEST_PLAN_NAME_FOR_RESULT_FILE, <<SMR_NOISE>> is used!!");
		strcpy (g_szTestPlanNameForResultFile, "SMR_NOISE");
	}					 
	WriteToDataWin("Using the following SW configuration");
	WriteToDataWin("TEST_PLAN_NAME_FOR_RESULT_FILE  : %s", g_szTestPlanNameForResultFile);
	
	// close File
	boIniClose ();
	return 0;
	
}

//************************************************************************************************
