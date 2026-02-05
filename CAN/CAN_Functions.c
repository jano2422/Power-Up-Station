/*************************************************************
	CAN Funktionslibrary
	
	Beinhaltet Verwendung und Konfiguration des Module Management
	
*************************************************************/


#include "CAN_Base.h"
#include "ApplicationTools.h"

//#include "SMR_Promik_Flash.h"


#ifdef APPLICATION_NAME
	V_MODULE(APPLICATION_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_BUGFIX, VERSION_BUILD, M_INIT);
	static char szVersionStringBuffer[32];
#endif//APPLICATION_NAME


TBusConnection * ptConnection = NULL;
#ifdef CAN_PRIVATE
TBusConnection * ptConnection2 = NULL;
#endif

void print_versions(void)
{
	char szLine[256];
	Module ** amodTmp = M_GetModulePtrArray();
	int iCnt = M_Count();
	int i;
	
	WriteToDataWinLog("Geladene Programmmodule:");
	
	for (i = 0; i < iCnt; i++)
	{
		M_GetModuleString(amodTmp[i], szLine, 256);
		WriteToDataWinLog("%s [c: %s]", szLine, M_GetModuleTimestamp(amodTmp[i]));
	}
	free(amodTmp);
}


void M_INIT(void)
{ // Modul-Initialisierungsfunktion
	static int boInitialized = 0;
	if (boInitialized) return;
	boInitialized = 1;
	
	// Module registrieren
	M_Register(&GET_MODULE(ModuleManagement));
	M_Register(&GET_MODULE(DLLLoader));
	
	{ // GBus-DLLs dynamisch einbinden und deren Module automatisch registrieren
		TDll * apDllData[100];
		int iDllCount = 0;
		iDllCount = iLoadDynamicDlls(".", "GBus_*.dll", apDllData, 100);
	}
	
	M_Register(&GET_MODULE(GBus_CAN)); // Falls CAN-Kommunikation benötigt
	M_Register(&GET_MODULE(GBus_Basic));
	
	// Liste der vorhanden Abstraction Layer und Command Libraries aktualisieren
	iRefreshCANInterfaces(); // Falls CAN-Kommunikation benötigt
	
	iRefreshCommandLibraries();

	// FLASH LIBRARY
//	M_Register(&GET_MODULE(SMR_PROMIK_FLASH));
	
	
#ifdef APPLICATION_NAME
	M_Register(&GET_MODULE(APPLICATION_NAME));
	
	M_GetShortVersionString(&GET_MODULE(APPLICATION_NAME), szVersionStringBuffer, 32);
	strncpy(g_szVersion, szVersionStringBuffer, 31);
	g_szVersion[31] = '\0';
#endif//APPLICATION_NAME
	
	
	print_versions();
	
}

int boCanInit(void)
{
	int boResult = 0;
		//JC.Ancheta May-24-2023 | To ensure the previous CAN port handle is closed before initializing it, include canClose at the start of canInit.
	if(ptConnection != NULL)
	{
	 CanClose();
	 Sleep(100);
	}
	
	ptConnection=NULL;
	#ifdef CAN_PRIVATE
	ptConnection2=NULL; 
	#endif//CAN_PRIVATE
	M_INIT(); // Ganz am Anfang
	
	if (!ptConnection)
	{
		ptConnection = ptNewEmptyBusConnection();
	}
#ifdef CAN_PRIVATE
//JC.Ancheta May-24-2023 | To ensure the previous CAN port handle is closed before initializing it, include canClose at the start of canInit.
	 
	if(ptConnection2 != NULL)
	{
		CanClose();
	 Sleep(100);}
	if (!ptConnection2)
	{
		ptConnection2 = ptNewEmptyBusConnection();
	}
#endif//CAN_PRIVATE
	
	boResult = boOpenCANCard(ptConnection, "CAN1");
	if (!boResult)
	{
		WriteToErrorWin("Error on boOpenCANCard - CAN1:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection));
		return 0;
	}
	
#ifdef CAN_PRIVATE
	boResult = boOpenCANCard(ptConnection2, "CAN2");
	if (!boResult)
	{
		WriteToErrorWin("Error on boOpenCANCard - CAN2:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection2));
		return 0;
	}
#endif//CAN_PRIVATE
	
	
	boResult = boSetCANInterface(ptConnection, "ADAS PSW");
	if (!boResult)
	{
		WriteToErrorWin("Error on boSetCANInterface ADAS PSW - CAN1:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection));
		return 0;
	}
	
#ifdef CAN_PRIVATE
	boResult = boSetCANInterface(ptConnection2, "ADAS PSW");
	if (!boResult)
	{
		WriteToErrorWin("Error on boSetCANInterface ADAS PSW - CAN2:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection2));
		return 0;
	}
#endif//CAN_PRIVATE
	
	
	boResult = boInitInterface(ptConnection);
	WriteToErrorWin( "CAN1 Port boCanInit:%d", ptConnection->hPort );
	if (!boResult)
	{
		WriteToErrorWin("Error on boInitInterface - CAN1:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection));
		return 0;
	}
	
#ifdef CAN_PRIVATE
	boResult = boInitInterface(ptConnection2);
	WriteToErrorWin( "CAN2 Port boCanInit:%d", ptConnection2->hPort );
	if (!boResult)
	{
		WriteToErrorWin("Error on boInitInterface - CAN2:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection2));
		return 0;
	}
#endif//CAN_PRIVATE
	

#ifdef CAN_COMMAND_LIBRARY
	boResult = boSelectCommandLibrary(ptConnection, CAN_COMMAND_LIBRARY);
	if (!boResult)
	{
		WriteToErrorWin("Error on boSelectCommandLibrary " ## CAN_COMMAND_LIBRARY ## " - CAN1:");
		WriteToErrorWin("%s", szGetErrorMessage(ptConnection));
		return 0;
	}
#endif//CAN_COMMAND_LIBRARY
	
	
	return 1;
}

int boCanInitAppl(void)
{
	int boResult = 0;
	
	//JC.Ancheta May-24-2023 | To ensure the previous CAN port handle is closed before initializing it, include canClose at the start of canInit.
	
	if(ptConnection != NULL)
	{
	CanCloseAppl();
	Sleep(100);	 }
	
	if (!ptConnection)
	{
		ptConnection = ptNewEmptyBusConnection();
	}
	
	M_INIT(); // Ganz am Anfang


	boResult = boOpenCANCard(ptConnection, "CAN1");
	if (!boResult) {
		WriteToErrorWin("Error on boOpenCANCard - CAN1:");
		// new CAN IF
		WriteToErrorWin("CAN IF Error: %s", szGetErrorMessage (ptConnection));
		if (strlen (szGetLastFirmwareError (ptConnection)) > 0) {
			WriteToErrorWin("CAN FW Error: %s", szGetLastFirmwareError(ptConnection));
		}
		return 0;
	}

	boResult = boSetCANInterface(ptConnection, "SRR Mitsubishi");
	if (!boResult) {
		WriteToErrorWin("Error on boSetCANInterface SRR Mitsubishi - CAN1:");
		// new CAN IF
		WriteToErrorWin("CAN IF Error: %s", szGetErrorMessage (ptConnection));
		if (strlen (szGetLastFirmwareError (ptConnection)) > 0) {
			WriteToErrorWin("CAN FW Error: %s", szGetLastFirmwareError(ptConnection));
		}
		return 0;
	}

	boResult = boInitInterface(ptConnection);
	WriteToErrorWin( "CAN1 Port boCanInit:%d", ptConnection->hPort );
	if (!boResult) {
		WriteToErrorWin("Error on boInitInterface - CAN1:");
		WriteToErrorWin("CAN IF Error: %s", szGetErrorMessage (ptConnection));
		if (strlen (szGetLastFirmwareError (ptConnection)) > 0) {
			WriteToErrorWin("CAN FW Error: %s", szGetLastFirmwareError(ptConnection));
		}
		return 0;
	}

	return 1;
}

int boCanInitAppl2(void)
{
	int boResult = 0;
	 //JC.Ancheta May-24-2023 | To ensure the previous CAN port handle is closed before initializing it, include canClose at the start of canInit.
	
	if(ptConnection2 != NULL)
	{
	CanCloseAppl2();
	Sleep(100);	 }
	
	if (!ptConnection2)
	{
		ptConnection2 = ptNewEmptyBusConnection();
	}
	
	M_INIT(); // Ganz am Anfang


	boResult = boOpenCANCard(ptConnection2, "CAN2");
	if (!boResult) {
		WriteToErrorWin("Error on boOpenCANCard - CAN2:");
		// new CAN IF
		WriteToErrorWin("CAN2 IF Error: %s", szGetErrorMessage (ptConnection2));
		if (strlen (szGetLastFirmwareError (ptConnection2)) > 0) {
			WriteToErrorWin("CAN2 FW Error: %s", szGetLastFirmwareError(ptConnection2));
		}
		return 0;
	}

	boResult = boSetCANInterface(ptConnection2, "SRR Mitsubishi");
	if (!boResult) {
		WriteToErrorWin("Error on boSetCANInterface SRR Mitsubishi - CAN2:");
		// new CAN IF
		WriteToErrorWin("CAN2 IF Error: %s", szGetErrorMessage (ptConnection2));
		if (strlen (szGetLastFirmwareError (ptConnection2)) > 0) {
			WriteToErrorWin("CAN2 FW Error: %s", szGetLastFirmwareError(ptConnection2));
		}
		return 0;
	}

	boResult = boInitInterface(ptConnection2);
	WriteToErrorWin( "CAN2 Port boCanInit:%d", ptConnection2->hPort );
	if (!boResult) {
		WriteToErrorWin("Error on boInitInterface - CAN2:");
		WriteToErrorWin("CAN2 IF Error: %s", szGetErrorMessage (ptConnection2));
		if (strlen (szGetLastFirmwareError (ptConnection2)) > 0) {
			WriteToErrorWin("CAN2 FW Error: %s", szGetLastFirmwareError(ptConnection2));
		}
		return 0;
	}

	return 1;
}

int boCanReset(void)
{
	CanClose();
	
	return boCanInit();
}


int CanClose(void)
{
	int boResult = 0; 
#ifdef CAN_PRIVATE
	if(ptConnection2 != NULL) //JC.Ancheta May-24-2023 | To prevent errors, ensure that only the connection with the pointer value is closed.
	{
	WriteToErrorWin( "CAN2 port CanClose:%d", ptConnection2->hPort );
	boResult = boReleaseInterface(ptConnection2);
	
	if (!boResult) {
		WriteToErrorWin("CanClose: Error on boReleaseInterface - CAN2:");
		return 0;
	}
	}
	
#endif//CAN_PRIVATE
if(ptConnection != NULL) //JC.Ancheta May-24-2023 | To prevent errors, ensure that only the connection with the pointer value is closed. 
	{
	WriteToErrorWin( "CAN1 port CanClose:%d", ptConnection->hPort );
	boResult = boReleaseInterface(ptConnection);
	if (!boResult) {
		WriteToErrorWin("CanClose: Error on boReleaseInterface - CAN1:");
		return 0;
	}
	}
	
#ifdef CAN_PRIVATE
if(ptConnection2 != NULL) //JC.Ancheta May-24-2023 | To prevent errors, ensure that only the connection with the pointer value is closed.
	{
	
	boResult = boCloseCANCard(ptConnection2);
	if (!boResult) {
		WriteToErrorWin("CanClose: Error on boCloseCANCard - CAN2:");
		return 0;
	}
	}
	
#endif//CAN_PRIVATE
	if(ptConnection != NULL) //JC.Ancheta May-24-2023 | To prevent errors, ensure that only the connection with the pointer value is closed. 
	{
	boResult = boCloseCANCard(ptConnection);
	if (!boResult) {
		WriteToErrorWin("CanClose: Error on boCloseCANCard - CAN1:");
		return 0;
	}
	}
	return 1;  
}

int CanCloseAppl2(void)
{
	int boResult = 0; 
	if(ptConnection2 != NULL) //JC.Ancheta May-24-2023 | To prevent errors, ensure that only the connection with the pointer value is closed. 
	{
	WriteToErrorWin( "CAN2 port CanCloseAppl:%d", ptConnection2->hPort );
	boResult = boReleaseInterface(ptConnection2);
	if (!boResult) {
		WriteToErrorWin("CanCloseAppl: Error on boReleaseInterface - CAN2:");
		return 0;
	}

	boResult = boCloseCANCard(ptConnection2);
	if (!boResult) {
		WriteToErrorWin("CanCloseAppl: Error on boCloseCANCard - CAN2:");
		return 0;
	}
	}	  
	return 1;  
}

int CanCloseAppl(void)
{
	int boResult = 0; 
	if(ptConnection != NULL) //JC.Ancheta May-24-2023 | To prevent errors, ensure that only the connection with the pointer value is closed. 
	{
	WriteToErrorWin( "CAN1 port CanCloseAppl:%d", ptConnection->hPort );
	boResult = boReleaseInterface(ptConnection);
	if (!boResult) {
		WriteToErrorWin("CanCloseAppl: Error on boReleaseInterface - CAN1:");
		return 0;
	}

	boResult = boCloseCANCard(ptConnection);
	if (!boResult) {
		WriteToErrorWin("CanCloseAppl: Error on boCloseCANCard - CAN1:");
		return 0;
	}
	}	  
	return 1;  
}
