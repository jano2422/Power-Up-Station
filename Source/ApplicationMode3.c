//**********************************************************************
//**** new IO version Appmode3- For New Line Control and Handling time Display logging
//**** Promik Common version
//**** N.Virtucio - Nov.30 2020
//**********************************************************************
#include <Windows.h> 
#include <userint.h>
#include <CviTypes.h>
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>  
#include "toolbox.h"  
#include <definitions.h>  
#include <FraMesLog.h>		// Must be before the native FraMes-Header
#include <ErrorLog.h>
#include <wipmama.h>
#include <CviLib.h>
#include <PnlTools.h>
#include <ApplicationTools.h>
#include <RunSpsTestprogramm.h>
#include <OpcTestprogramm.h>
#include <CAN_Base.h>
#include <tcpsupp.h> 
#include <ctype.h>
#include <string.h>
#include <SHLWAPI.H>


#define _USE_DLL 		// Hilft wahrscheinlich nichts, da CVI keine Makros in C-Files auch in H-Files expandiert ???
#include <AS_DLL.h>
#include <TPDtoEVAFunc.h> 
#include <evaprod.h>
#include <sequencer.h>

#include <LineControl.h>
#include <HpSupply.h>
#include <6TL_Cards.h>
#include <NI_DMM4070.h>
#include "CAN_Base.h"
#include "PXI-Dio.h"
#include "GBus_Basic_transparent.h"
#include "GBus_basic.h"
#include "Barcode.h"
#include "FixtureSlot.h"

#include <RunSpsTestprogramm.h>

#include <ProMik_CAN_Flash.h> 
#include <IniData.h> 

#include "lcfunctions.h"
//#include "lineController2.h"
//#include <TCPServer.h>
//#include <TCPClient.h>

// **********CALAMBA***********  
#include "specialFunction.h" 
#include "Control.PXI6528.h"
#include "FixtureSlot.h"
#include "MultiDutUi.h"
#include "PANEL_SCAN.h"
// **********CAN G-API***********

// *****************************
//GS

//************************************************************************************************
// local
char *g_szPtrFraMesLogSerNr = g_UnitInfo.szUnitId;
int g_iManualStart = 0; 
int g_iManualTrigger = 0;   
short g_sIdx;
unsigned long g_ulTempSn = 0;
char g_szFinalSn[100] = {0};
char g_szTextInfo[MAX_TEXT_INFO] = {0};
char g_cResult;
StructPmf g_Pmf = {False, 55};
char g_szOrder[100];
char g_szUnitMaterial[100];
short g_sSlot, g_sIdx, g_sRun;
double dHandlingTime; 
char g_sPMF[100] = {0};

char g_szSerialId[50]= {0};
char g_szTestPlan [20];
char g_szStationId[20];

#define CTRL_SCAN_DUT_SERIAL 2001
int CVICALLBACK UI_ScanDutSerial (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2);

static int g_hdlPanelScan = 0;

static int CVICALLBACK PanelScan_StartCallback(int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK PanelScan_CancelCallback(int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK PanelScan_CloseCallback(int panel, int event, void *callbackData,
        int eventData1, int eventData2);
static void PanelScan_UpdateStatus(const char *statusText, const char *slotId);
static void PanelScan_RefreshQueueList(void);
static void TrimWhitespace(char *text);


int CVICALLBACK UI_ScanDutSerial (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
        char szNextSlot[64] = {0};
        char szStatus[256] = {0};

        if (event != EVENT_COMMIT)
        {
                return 0;
        }

        if (g_hdlPanelScan <= 0)
        {
                g_hdlPanelScan = LoadPanel(0, "PANEL_SCAN.uir", PANEL);
                if (g_hdlPanelScan <= 0)
                {
                        WriteToErrorWin("[PANEL_SCAN] Failed to load PANEL_SCAN.uir");
                        return 0;
                }

                InstallCtrlCallback(g_hdlPanelScan, PANEL_SCAN_BTN_START, PanelScan_StartCallback, NULL);
                InstallCtrlCallback(g_hdlPanelScan, PANEL_SCAN_BTN_CANCEL, PanelScan_CancelCallback, NULL);
                InstallPanelCallback(g_hdlPanelScan, PanelScan_CloseCallback, NULL);
        }

        FixtureScan_GetNextSlotId(szNextSlot, sizeof(szNextSlot));

        SetCtrlAttribute(g_hdlPanelScan, PANEL_SCAN_TXT_SLOT, ATTR_CTRL_MODE, VAL_INDICATOR);
        SetCtrlAttribute(g_hdlPanelScan, PANEL_SCAN_TXT_SERIAL, ATTR_CTRL_MODE, VAL_HOT);
        SetCtrlVal(g_hdlPanelScan, PANEL_SCAN_TXT_SLOT, szNextSlot);
        SetCtrlVal(g_hdlPanelScan, PANEL_SCAN_TXT_SERIAL, "");

        Fmt(szStatus, "Expected SLOT: %s | Enter DUT SERIAL then press Test Start", szNextSlot);
        PanelScan_UpdateStatus(szStatus, szNextSlot);
        PanelScan_RefreshQueueList();

        DisplayPanel(g_hdlPanelScan);

        return 0;
}


static int CVICALLBACK PanelScan_StartCallback(int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
        char szExpectedSlot[64] = {0};
        char szSlotInput[64] = {0};
        char szSerial[256] = {0};
        char szStatus[256] = {0};

        if (event != EVENT_COMMIT)
        {
                return 0;
        }

        FixtureScan_GetNextSlotId(szExpectedSlot, sizeof(szExpectedSlot));

        GetCtrlVal(panel, PANEL_SCAN_TXT_SLOT, szSlotInput);
        GetCtrlVal(panel, PANEL_SCAN_TXT_SERIAL, szSerial);

        TrimWhitespace(szSlotInput);
        TrimWhitespace(szSerial);

        if (strlen(szSlotInput) == 0 || strlen(szSerial) == 0)
        {
                PanelScan_UpdateStatus("SLOT and SERIAL are required.", szExpectedSlot);
                return 0;
        }

        if (StrICmp(szExpectedSlot, szSlotInput) != 0)
        {
                Fmt(szStatus, "Invalid SLOT. Expected: %s", szExpectedSlot);
                PanelScan_UpdateStatus(szStatus, szExpectedSlot);
                SetCtrlVal(panel, PANEL_SCAN_TXT_SLOT, szExpectedSlot);
                return 0;
        }

        if (!FixtureScan_Enqueue(szExpectedSlot, szSerial))
        {
                PanelScan_UpdateStatus("Failed to queue DUT. Check logs.", szExpectedSlot);
                return 0;
        }

        FixtureSlot_SetActiveId(szExpectedSlot);
        PanelScan_RefreshQueueList();

        FixtureScan_GetNextSlotId(szExpectedSlot, sizeof(szExpectedSlot));
        SetCtrlVal(panel, PANEL_SCAN_TXT_SLOT, szExpectedSlot);
        SetCtrlVal(panel, PANEL_SCAN_TXT_SERIAL, "");

        Fmt(szStatus, "Queued. Next expected SLOT: %s", szExpectedSlot);
        PanelScan_UpdateStatus(szStatus, szExpectedSlot);

        SetActiveCtrl(panel, PANEL_SCAN_TXT_SERIAL);
        return 0;
}

static int CVICALLBACK PanelScan_CancelCallback(int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
        char szExpectedSlot[64] = {0};

        if (event != EVENT_COMMIT)
        {
                return 0;
        }

        FixtureScan_Clear();
        FixtureScan_GetNextSlotId(szExpectedSlot, sizeof(szExpectedSlot));

        SetCtrlVal(panel, PANEL_SCAN_TXT_SLOT, szExpectedSlot);
        SetCtrlVal(panel, PANEL_SCAN_TXT_SERIAL, "");
        PanelScan_RefreshQueueList();
        PanelScan_UpdateStatus("Queue cleared.", szExpectedSlot);

        HidePanel(panel);
        return 0;
}

static int CVICALLBACK PanelScan_CloseCallback(int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
        if (event == EVENT_CLOSE)
        {
                HidePanel(panel);
                return 1;
        }

        return 0;
}

static void PanelScan_UpdateStatus(const char *statusText, const char *slotId)
{
        char szStatus[256] = {0};

        if (g_hdlPanelScan <= 0)
        {
                return;
        }

        if (slotId == NULL)
        {
                slotId = "n/a";
        }

        if (statusText == NULL)
        {
                statusText = "";
        }

        Fmt(szStatus, "%s\nExpected SLOT: %s\nQueued items: %d",
            statusText,
            slotId,
            FixtureScan_Count());

        SetCtrlVal(g_hdlPanelScan, PANEL_SCAN_TXT_STATUS, szStatus);
}

static void PanelScan_RefreshQueueList(void)
{
        int i;
        char szEntry[512] = {0};
        TFixtureScanRequest request = {0};

        if (g_hdlPanelScan <= 0)
        {
                return;
        }

        ClearListCtrl(g_hdlPanelScan, PANEL_SCAN_LIST_QUEUE);

        for (i = 0; i < FixtureScan_Count(); ++i)
        {
                if (!FixtureScan_GetByIndex(i, &request))
                {
                        continue;
                }

                Fmt(szEntry, "%s	%s", request.szFixtureId, request.szDutSerial);
                InsertListItem(g_hdlPanelScan, PANEL_SCAN_LIST_QUEUE, -1, szEntry, i + 1);
        }
}

static void TrimWhitespace(char *text)
{
        char *start = text;
        char *end;

        if (text == NULL)
        {
                return;
        }

        while (*start && isspace((unsigned char)*start))
        {
                start++;
        }

        end = start + strlen(start);
        while (end > start && isspace((unsigned char)*(end - 1)))
        {
                *(--end) = '\0';
        }

        if (start != text)
        {
                memmove(text, start, (size_t)(end - start) + 1);
        }
}


//************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_Mode3Start
//
// Description: Is called once after pressing the start-button.
//							Only if "MODE = 3" in FraMes.cfg
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
// Do all HW-/SW-Initialization
// LoadPanel, Start ErrorLogging, Initialize Global Variables, Load DLL's, ...
short __stdcall app_Mode3Start (void)
{
	int pnlFramesStart = 0;
	char szPanelName[255]= {0}; 
	char szParm1[500]= {0};
	char szParm2[500]= {0};
	char szParm3[500]= {0};
	char szTemp[500]= {0};  
	int pnlLineController = 0;
	char szErrorMessage[500]= {0};
	long lStatus[100]= {0};
	char szCWD[MAX_PATHNAME_LEN]= {0};
	char szCWD_LC[MAX_PATHNAME_LEN]= {0};
	
	char szMachineName[256] = {0};
	char szDirName[256] = {0};
	char szErrorFileName[256] = {0};
	int iResult = 0;
	
	
	int x, y;	 //Calamba 
	char szMsg[100];
	
	/******Calamba**************/
	GetScreenSize(&y, &x);
	
    if( !boCheckMutex() ) {
            exit( -1 );
    }

	
	if( SetSleepPolicy (VAL_SLEEP_SOME) != 0 ) {
		WriteToErrorWin("Error on SetSleepPolicy");
		return( 0 );
	}
	// HIGH_PRIORITY_CLASS
	if( SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS) == 0 ) {
		WriteToErrorWin("Error on SetPriorityClass");
		return( 0 );
	}
	 	// verschieben des controllers 
	pnlFramesStart = GetActivePanel ();
	
	GetPanelAttribute (pnlFramesStart, ATTR_CONSTANT_NAME, szPanelName);
	SetPanelAttribute (pnlFramesStart, ATTR_TITLE, _STATION_NAME );    
	
	// starten des normalen tool panels
	if( (g_hdlToolPanel = LoadPanel (0, "PnlTools.uir", PNL_TOOLS)) < 0 ) {
		WriteToErrorWin("Error on LoadPanel of PnlTools");
		return( 0 );
	}
	SetPanelAttribute (g_hdlToolPanel, ATTR_TITLE, _STATION_NAME);   
	
        DisplayPanel (g_hdlToolPanel);
    status_init ();

    setStatus(READY);

    if ( iConfigureSwFromFrames (".\\FRAMES.CFG") ) {
            WriteToErrorWin("Error on configuring SW from frames.cfg");
            return( 0 );
    }
	
	if ( iGetPathsFromFrames (".\\FRAMES.CFG") ) {
		WriteToErrorWin("Error on loading paths from frames.cfg"); 
		return( 0 );	
	}

        if(iGetInterfaceFromFrames (".\\Frames.cfg")){

                WriteToErrorWin("Error on GetInterfaceFromFrames");
                return( 0 );
        }

        FixtureSlot_Initialize(-1, -1, -1);
	
	
	iResult = GetCompName(szMachineName);
	if (iResult != 0)
	{
		WriteToErrorWin("Can not get the machine name."); 
		return(0);	
	}
	sprintf (szDirName,"%s\\%s",g_szErrorFilePath,szMachineName); 
	SetBOLE (0);
	iResult=MakeDir(szDirName);
	if ((iResult < 0) && (iResult != -9))
	{
		WriteToErrorWin("Error creating directory on X drive");
		return( 0 );
	}

	sprintf (szErrorFileName,"%s\\Errorlog.txt",szDirName);
	// Start ErrorLogging
	if( !boStartErrorLog(szErrorFileName) ) {
		WriteToErrorWin("Error on boStartErrorLog");
		return( 0 );
	}
	if( !boStartFraMesLog(NULL) ) {
		WriteToErrorWin("Error on boStartFraMesLog");
		return( 0 );
	}



	if( ptConnection == NULL ) {
		ptConnection = ptNewEmptyBusConnection();
	}
	

	
	//*********************************************************************
	// Hardware
        if( !boHpPsInit(&g_HpSupplyPrimary, 5) ){
                WriteToErrorWin("Error, can not init the Power Supply!");
                return( False );
        }
        

        if( !boHpPsSetOutput(&g_HpSupplyPrimary, Off) ){
                return( False );
        }

		         
		if( !boHpPsInit(&g_HpSupplySecondary, 10 ) ){
                WriteToErrorWin("Error, can not init the Power Supply!");
                return( False );
        }
        

        if( !boHpPsSetOutput(&g_HpSupplySecondary, Off) ){
                return( False );
        }					
	
	M_INIT();	 // Ganz am Anfang 
	
	
	/*// CAN Göpel Card 
	if (!boCanInit())
	{
		WriteToErrorWin("Error on boCanInit");
		return( 0 );
	}
	
	if (!boCanReset())
	{
		WriteToErrorWin("Error on boCanReset");
		return( 0 );
	}
	*/	
	// 6TL Cards
	if( !boInitYAV()){
		WriteToErrorWin("6TL cards Initialization Error!");
		return( 0 );
	}
	if( !boAskYavRel(CARD904X8_1, 1, lStatus)){ 
		WriteToErrorWin("6TL CARD904X8_1 Test Error!");
		return( 0 );
	}
	if( !boResetYavBoard(CARD904X8_1)){ 
		WriteToErrorWin("6TL CARD904X8_1 Reset Error!");
		return( 0 );
	}	 
	
	if( !boAskYavRel(CARD904X8_2, 1, lStatus)){ 
		WriteToErrorWin("6TL CARD904X8_2 test Error!");
		return( 0 );
	}
	if( !boResetYavBoard(CARD904X8_2)){ 
		WriteToErrorWin("6TL CARD904X8_2 Reset Error!");
		return( 0 );
	}
	
	if( !boAskYavRel(CARD904X8_3, 1, lStatus)){ 
		WriteToErrorWin("6TL CARD904X8_3 test Error!");
		return( 0 );
	}
	if( !boResetYavBoard(CARD904X8_3)){ 
		WriteToErrorWin("6TL CARD904X8_3 Reset Error!");
		return( 0 );
	}
	
				 
	WriteToDataWin("------------------------------------------------------------------------------------------------------------------------------------------------");
	WriteToDataWin( _STATION_STARTUP_NAME );
	WriteToDataWin("------------------------------------------------------------------------------------------------------------------------------------------------");
	dHandlingTime = Timer();
	g_boTesterOk = True;	
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_Mode3Stop
//
// Description: Is called once after pressing the exit-button.
//							Only if "MODE = 3" in FraMes.cfg
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
// ReDo all HW-/SW-Initialization
// DiscardPanel, Stop ErrorLogging, Free DLL's, ...
short __stdcall app_Mode3Stop (void)
{
//	char szErrorMessage[500];
	char szDummy[256] = {0};
	int iHandle =0;
	char szCWD_PRK[MAX_PATHNAME_LEN]= {0};
	char szMachineName[256] = {0};
	int iResult  = 0;
	g_boTesterOk = False; 
	
	
	iResult = GetCompName(szMachineName);
	if (iResult != 0) {
		WriteToErrorWin("Can not get the machine name."); 

	}
	
	 // 6TL
	if( !boResetYavBoard(CARD904X8_1)){ 
		WriteToErrorWin("6TL CARD904X8_1 Reset Error!");
	}	
	if( !boResetYavBoard(CARD904X8_2)){ 
		WriteToErrorWin("6TL CARD904X8_2 Reset Error!");
	}
	if( !boResetYavBoard(CARD904X8_3)){ 
		WriteToErrorWin("6TL CARD904X8_3 Reset Error!");
		return( 0 );
	}
	
	if (!boCloseYAV())
	{
		WriteToErrorWin("Error on app_Mode3Stop, boCloseYAV");
	}

	
	
	// CAN Card
	//boCanReset();
	if (ptConnection != NULL) {
		CanClose();
	}

	
	boStopFraMesLog();
	boStopErrorLog();
	
	if (g_hdlPanelScan > 0)
	{
		DiscardPanel(g_hdlPanelScan);
		g_hdlPanelScan = 0;
	}

	//DiscardPanel (g_hdlToolPanel);
	g_hdlToolPanel = 0;
	
	con_Exit();  

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_GetUnit
//
// Description: Is called cyclically for checking start up testing
//							Only if "MODE = 3" in FraMes.cfg
// Parameters:
//
// Returns:	0 if no UnitId for the next DUT is available
//			1 if a  UnitId for the next DUT is available
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_GetUnit(char *szUnitIdType,char *szUnitId)
{
	static sStatus = 0;
	static char szLine[255];
	
	Sleep(5);
	
	sprintf(szLine, "  Runs/Pass/Fail: %d/%d/%d",
									tScanningQuality.iRead, tScanningQuality.iPass, tScanningQuality.iFail);
	SetCtrlAttribute (g_hdlToolPanel, PNL_TOOLS_LB_SHOW_DATA, ATTR_LABEL_TEXT, szLine);
	
	//
	sStatus = sRunSps();
	if( sStatus <= 0 ) {
		return( 0 );
	}
	strcpy(szUnitIdType, g_UnitInfo.szUnitIdType);
	strcpy(szUnitId, g_UnitInfo.szUnitId);
	
	//Calamba
	strcpy(g_szSerialId,szUnitId);
	setStatus(validBarcode);

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_DischargeUnit
//
// Description: Is called once after "app_GetUnit" has finished with 1, but an error occured within WIP.
//							Only if "MODE = 3" in FraMes.cfg
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_DischargeUnit(char *szUnitIdType,char *szUnitId)
{ 
	
	g_cResult = 'F'; // to have the correct RunSps status if unit couldn't be tested because of WIP status
	wpm_ShowError();
	
	return( 1 );
}



//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_GetType
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_GetType (char *cAppType)
{
	*cAppType='S';		// 'S' = Sequencer
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_TestPlanActivate
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_TestPlanActivate(short sSlot)
{   
	
	
	//if( !eva_LoadTestPlan(sSlot, &g_sIdx, g_UnitInfo.szUnitMaterial, "SMR_EOL", 0) ) {
	if( !eva_LoadTestPlan(sSlot, &g_sIdx, g_UnitInfo.szUnitMaterial, g_szTestPlan, 0) ) {
		eva_ShowError();
		return( 0 );
	}

	// Set Material
	if( !eva_SetMaterial(sSlot, g_UnitInfo.szUnitMaterial) ) {
		eva_ShowError();
		return( 0 );
	}

	// Set order
	if( !eva_SetOrderType(sSlot, g_UnitInfo.szOrder, g_UnitInfo.cUnitClass ) ) { // P Produduction, T Test, Q Quality
		eva_ShowError();
		return( 0 );
	}

	// Set testprogram
	if( !eva_SetTestProgram(sSlot, g_UnitInfo.szUnitMaterial,g_szTestPlan, g_szVersion) ) {
		eva_ShowError();
		return( 0 );
	}
		
	
	// set maximum number of test runs ==> has NO effects 
	if( !eva_SetMaxTestRuns (sSlot, g_sIdx, 1) ) {
		eva_ShowError ();
		return( 0 );
	}
	
	// WriteToDataWinMaterial("Material: %s", g_UnitInfo.szUnitMaterial);
	DefineMeasArray ();		 //Perform The tests

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_TestPlanDeactivate
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_TestPlanDeactivate(short sSlot)
{
	if( !eva_RemoveTestPlan(sSlot, g_sIdx) ) {
		eva_ShowError();
		return( 0 );
	}

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_Start
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_Start (short sSlot)
{
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_Stop
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_Stop (short sSlot)
{
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_SequenceUnit
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_SequenceUnit(short sSlot, char *szUnitIdType,char *szUnitId)
{ 
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	short app_GetSequenceStart
//
// Description:	this function must return 1 if a unit is ready for measurement
//
// Parameters:	short sSlot			slot
//				short sIdx			index
//				short *sRunCount	count of runs
//				short *sRunType		type of run
//
// Returns:		1 if there are data ready to be sent to EVAPROD
//						0 if it is not allowed to start testing, then processed with app_GetUnit()
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_GetSequenceStart(short sSlot,short *sIdx,short *sRunCount, short *sRunType)
{
	*sRunType = SEQ_SERIAL;
	
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_SequenceEnd
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_SequenceEnd(short sSlot,short sIdx,short sRunCount, short sRunType)
{
	
	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_TestRunStart
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_TestRunStart (short sSlot, short sIdx, short sRun)
{
	// Set testrun id
	if( !eva_SetTestRunId(sSlot, sIdx, sRun, g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId) ) {
		eva_ShowError();
		return( 0 );
	}
	
	
	g_sIdx = sIdx;
	g_sRun = sRun;
	g_EvaProdData.sSlot = sSlot; // necessary for error test info to id in Evaprod
	g_EvaProdData.sIdx = sIdx;
	g_EvaProdData.sRun = sRun;


	/*// Show single test steps
	if( !eva_ShowTests (481, 19, sSlot, sIdx, sRun) ) {
		eva_ShowError();
		return( 0 );
	}	*/	  
	
	// Start unit
	if (!wpm_UnitStart(sSlot, g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId))
	{
		wpm_ShowError();
		return( 0 );
	}

	memset(g_szTextInfo, 0, sizeof(g_szTextInfo));
	
	//Calamba
	setStatus(TESTING);

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function: 	float app_GetTestValue
//
// Description:	this function works as a dispatcher to the single test steps (measurements)
//				each test step may be identified either by sTest (number from 1 to n) or
//				szTestId (unique ASCIIZ string)
//
// Parameters:	short sSlot		slot
//				short sIdx		index
//				short sRun		test run
//				short sTest		test step number
//				char *szTestId	test step identifier (unique)
//
// Returns:		the value measured (measurement result)
//
//////////////////////////////////////////////////////////////////////////////////////////////
float __stdcall app_GetTestValue(short sSlot,short sIdx,short sRun,short sTest,char *szTestId)
{
        float fResult;
        eva_STRUCTTEST testInfo;
        MultiDutResultView view = {0};
        char szLimits[96] = {0};
        char szNote[160] = {0};
        MultiDutSelectedInfo info = {0};
        const char *slotId = FixtureSlot_GetActiveId();

        memset(&testInfo, 0, sizeof(eva_STRUCTTEST));

        fResult =  ExecuteMeasure(sSlot, sIdx, sRun, sTest, szTestId);

        // EXIT on failed - test run terminate after one failed test step
        if ((eva_GetTestRunResult (sSlot, sIdx, sRun) == 'F') && (atoi(wpm_GetCfg("EXIT_ON_FAILED",0))==1)){
                eva_CancelTestRun(sSlot,sIdx, sRun);
                return (0);
        }

        if (slotId == NULL || strlen(slotId) == 0)
        {
                slotId = "SLOT_01";
        }

        strncpy(info.fixtureId, slotId, sizeof(info.fixtureId) - 1);
        strncpy(info.unitId, g_szSerialId, sizeof(info.unitId) - 1);
        strncpy(info.unitIdType, g_UnitInfo.szUnitIdType, sizeof(info.unitIdType) - 1);
        strncpy(info.testPlanName, g_szTestPlan, sizeof(info.testPlanName) - 1);
        strncpy(info.unitMaterialNumber, g_UnitInfo.szUnitMaterial, sizeof(info.unitMaterialNumber) - 1);
        MultiDutUi_UpdateSlotMetadata(slotId, &info);

        if (eva_GetTestPlanTest (sSlot, sIdx, szTestId, &testInfo) < 0)
        {
                WriteToErrorWin("Failed to get test plan for %s",szTestId);
        }
        else
        {
                memset(&view, 0, sizeof(MultiDutResultView));
                strncpy(view.testId, szTestId, sizeof(view.testId) - 1);
                strncpy(view.description, testInfo.szTestDescription, sizeof(view.description) - 1);
				Fmt(view.value, "%s<%f %s", fResult, testInfo.szTestUnit);
                Fmt(szLimits, "%s<LSL %f / USL %f", testInfo.fLSL, testInfo.fUSL);
                strncpy(view.limits, szLimits, sizeof(view.limits) - 1);
                Fmt(szNote, "%s<Last reading for %s", szTestId);
                MultiDutUi_RecordMeasurement(slotId, g_szSerialId, &view, szNote);
        }

	return( fResult );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_TestRunEnd
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_TestRunEnd(short sSlot,short sIdx,short sRun)
{
	if( g_szTextInfo[0] != 0 ) {
		if( !eva_SetTextInfo(sSlot, sIdx, sRun, g_szTextInfo) ) {
			eva_ShowError();
		}
		memset(g_szTextInfo, 0, sizeof(g_szTextInfo));
	}

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_TestRunBin
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_TestRunBin(short sSlot,short sIdx,short sRun,short sBin)
{

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_TestRunResult
//
// Description:
//
// Parameters:
//
// Returns:			Not evaluated !!!
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_TestRunResult(short sSlot,short sIdx,short sRun,char cResult)
{
        //char *g_UnitInfo;
        struct wpm_structMaMaPartInfo pMaMaPartInfo;
        short PartRes = 0;
        int iResult = 0;
        char cMsg[524];
        const char *slotId = FixtureSlot_GetActiveId();
        char szNote[160] = {0};
	
	
	if( cResult == 'P' ) {
		g_cResult = 'P';
		//Calamba
		setStatus(PASS);
	}
	else{
		g_cResult = 'F';
		//Calamba
		setStatus(FAIL);
	}
	
	        
	Fmt(szNote, "Run completed with %s", (cResult == 'P') ? "PASS" : "FAIL");
	MultiDutUi_FinalizeRun(slotId, cResult, szNote);

	// Finish unit
	if( !wpm_UnitFinish(sSlot, g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId, cResult) ) {
		wpm_ShowError();
		return( 0 );
	}
	
	
	if( !wpm_UnitChangeId(g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId, g_UnitInfo.szUnitIdTypeFin, g_UnitInfo.szUnitIdFin) ) {
		wpm_ShowError();
		return( 0 );			
	} 
		

		 //wpm_GetPartInfo(const char *szPartIdType, const char *szPartId, struct wpm_structMaMaPartInfo *pMaMaPartInfo);   
 	PartRes =  wpm_GetPartInfo(g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId,&pMaMaPartInfo);                           
  	if (PartRes > 0)                                                                                                  
  	{                                                                                                                 
 		// wpm_DeleteUnitInMAMA(const char *szUnitIdType, const char *szUnitId);                                           
	 	if (!wpm_DeleteUnitInMAMA(g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId))                                       
	 	{                                                                                                              
	  	wpm_ShowError();                                                                                              
	  	return (0);                                                                                                   
	 	}                                                                                                              
  	} 	

	

	return( 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// Function:	app_GetTestRunRestart
//
// Description:
//
// Parameters:
//
// Returns:	0 if no retest is necessary
//					1 if a retest is desired
//
//////////////////////////////////////////////////////////////////////////////////////////////
short __stdcall app_GetTestRunRestart(short sSlot,short sIdx,short sRun)
{
	
	return( 0 );
}
