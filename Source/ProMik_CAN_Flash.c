/********************************************************************************/
/*                                                                           		*/
/*  Source File: ProMik_CAN_Flash.c			                          				*/
/*                                                                           		*/
/********************************************************************************

	Version 		Date				Author		 	Changes
	------------------------------------------------
	V1.00		   	26.09.2011			fehrmanng		Initial realization
	V1.01			10.10.2011			fehrmanng  		Adaption of SMR_Promik_Flash.c structure
	V1.10			01.03.2012			fehrmanng  		Function changed: boProMikSRR295EraseAreaToWrite, XCP_SMR200_Security (new parameter DLL 3.0.0.0)
														Function added: boProMikSRR295GetChecksumOnTarget, boProMikSRR295InitializeEEPROM()
	V1.10			01.03.2012			MedinaA2  		Renamed For Standarization. replaced all 295 for 3XX
	V1.11			23.01.2015			MuellerM02		Optimased Error Loging for Promik (-> copied from Jeanette)
*********************************************************************************

	Documentation:
	--------------
	Driver layer for flash process with Promik CAN_XCP_SRR300mOnline functions.
	Based on SMR_Promik_Flash.c of HoeppD and Promik_FlexRay_Flash.c of AnthoferA

*********************************************************************************/
//********************************************************************
  /* Modified Upgrade and Downgrade Promik function with Addnl delays
     Calamba - N.Virtucio Dec. 2020
*/
//********************************************************************	 

// one programmer(IP 192.168.1.1, Port 15504) connected to the host
#include <stdio.h>
#include <Windows.h>
#include <userint.h>
#include <utility.h>
#include <cvitypes.h> 
#include <formatio.h>
#include <ProMik_CAN_Flash.h>
#include <CAN_XCP_SRR300m_Online.h>
#include <CAN_XCP_SMR200m_Online.h>
#include <HpSupply.h>
#include "ApplicationTools.h"
#include "definitions.h"
/*********************************************************************************/
Boolean boThreadSwitchConnector;
Boolean boThreadSwitchPower;
int g_hThreadPool;
int g_hThreadPool2;
static int boThreadPool2Finished = 0; 
DWORD dwErrSwitchConnector;

#define PROMIKTIME 60000 


int iStartThreads(void);
enum Error {eOK = 0, eWarning = 1, eError = 2};
static int iFlashController = 1;
static unsigned int uLastError = 0;
static int boTargetConnected = 0;
//static int boResetSystem = 0;



typedef enum VKeyGenResultEx
{
	KGRE_Ok = 0,
	KGRE_BufferToSmall = 1,
	KGRE_SecurityLevelInvalid = 2,
	KGRE_VariantInvalid = 3,
	KGRE_UnspecifiedError = 4,
}VKeyGenResultEX;

typedef VKeyGenResultEX (*KEY_SEED) (unsigned char, unsigned char, unsigned char*,unsigned char*, unsigned char*);
extern int LinkSeedKeyAPI(void);
extern KEY_SEED XCP_ComputeKeyFromSeed;

#define IPADR(a,b,c,d) ((a<<24)+(b<<16)+(c<<8)+(d<<0))
IP_ADDRESS IPAddr[12] =		// CAN
{
{IPADR(192,168,1,1), 15504}, {0,0},{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
};

DWORD dwErr;
DWORD dwError; 

#define TARGET 'A' // Target Interface
#define SUBTYPE 1 //
//#define FILE_MASTER "..."
//#define FILE_CHKSUM 0xBA47 ???
#define FILL_OPTION 0
//#define FILL_OPTION 1
#define FILL_VALUE 0xFF
#define VERIFY_OPTION 2 // do normal verify
//#define VERIFY_OPTION 3
#define LEN_SEED_KEY 4

#define XCP_CRO_ID_PSW 0x6C2	//SRR3XX  PSW   new 0x6C2  old 0x5F4
#define XCP_DTO_ID_PSW 0x6C9 	//SRR3XX  PSW   new 0x6C9  old 0x5F6

extern char szTargetPPARFile[]; 
#define CAN_BD_INDEX 1





/********************************************************************************/
/********************************Utility Functions*******************************/
/********************************************************************************/
/// HIFN Document your function here.
/// HIFN You may use multiple lines for documentation.
/// HIRET Document return value here.
/// HIRET You may use multiple lines for documentation.
/// HIPAR text/Document parameter here.
/// HIPAR dwError/Document parameter here.
int CheckResult_Silent(LPSTR text, DWORD dwError)
{
	if (dwError & 0xC0000000) {
			//printf("%s Error: %08X\n", text, dwError);
		return 1;
	}
	return 0;
}
/*********************************************************************************/  
/*********************************************************************************/  
int iErrorState(unsigned int uError)
{ // 0: OK, 1: Warning, 2: Error
	return (uError & 0xC0000000) >> 30; // Führende 2 Bits
	/*
	if (uError & 0x80000000)
	{
		return 2; // Error
	}
	else if (uError & 0x40000000)
	{
		return 1; // Warning
	}
	else
	{
		return 0;
	}
	*/
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int iErrorType(unsigned int uError)
{
	return ((uError & 0x3E000000) >> 25);
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
const char * szGetErrorString(unsigned int uError)
{
	int iState = iErrorState(uError);
	static char szTextBuffer[1024];
	memset(szTextBuffer, 0, sizeof(szTextBuffer));
	if (iState == 0)
	{
		sprintf(szTextBuffer, "No Error (0x%08X)", uError);
		return szTextBuffer;
	}
	else if (iState == 1)
	{
		sprintf(szTextBuffer, "Warning (0x%08X): ", uError);
	}
	else if (iState == 2)
	{
		sprintf(szTextBuffer, "Error (0x%08X): ", uError);
	}
	else if (iState == 3)
	{
		sprintf(szTextBuffer, "Werror?! (0x%08X): ", uError);
		//strcpy(szTextBuffer, "Interface error: ");
		//strcat(szTextBuffer, szLastInterfaceError);
	}
	
	switch (iErrorType(uError))
	{
		case 1:
			strcat(szTextBuffer, "General error");
			break;
		case 2:
			strcat(szTextBuffer, "Communication error");
			break;
		case 3:
			strcat(szTextBuffer, "Protocol or function execution error");
			break;
		case 4:
			strcat(szTextBuffer, "Communication error with programmer");
			break;
		case 5:
			strcat(szTextBuffer, "Hardware error");
			break;
		case 6:
			strcat(szTextBuffer, "Unexpected answer");
			break;
		case 7:
			strcat(szTextBuffer, "Unexpected answer from Adapter");
			break;
		case 8:
			strcat(szTextBuffer, "not used");
			break;
		case 9:
			strcat(szTextBuffer, "No answer from Adapter");
			break;
		case 10:
			strcat(szTextBuffer, "not used");
			break;
		case 11:
			strcat(szTextBuffer, "Illegal parameter");
			break;
		case 12:
			strcat(szTextBuffer, "Value out of limits");
			break;
		case 13:
			strcat(szTextBuffer, "Object has wrong format");
			break;
		case 14:
			strcat(szTextBuffer, "Time-out error");
			break;
		case 15:
			strcat(szTextBuffer, "Device was busy");
			break;
		case 16:
			strcat(szTextBuffer, "Memory issue");
			break;
		case 17:
			strcat(szTextBuffer, "Function not implemented");
			break;
		case 18:
			strcat(szTextBuffer, "Function aborted");
			break;
		case 19:
			strcat(szTextBuffer, "Specified error");
			break;
		case 20:
			strcat(szTextBuffer, "File error");
			break;
		case 21:
			strcat(szTextBuffer, "Not initialized");
			break;
		case 22:
			strcat(szTextBuffer, "Error of the target MCU");
			break;
		case 23:
			strcat(szTextBuffer, "Flash access error");
			break;
		case 24:
			strcat(szTextBuffer, "Windows socket error");
			break;
		default:
			strcat(szTextBuffer, "Unknown error");
			break;
	}

	switch (uError & 0xFF0FFFFF)
	{
		case 0x42110021:
			strcat(szTextBuffer, ": Firmware not compatible");
			break;
		case 0x4201001D:
			strcat(szTextBuffer, ": Firmware update done");
			break;
		case 0x420C0008:
			strcat(szTextBuffer, ": Programmer Firmware newer than DLL Firmware");
			break;
		case 0x680AC000:
			strcat(szTextBuffer, ": Not all data loaded");
			break;
		case 0x60084009:
			strcat(szTextBuffer, ": Alignment violation");
			break;
		case 0xA0084008:
			strcat(szTextBuffer, ": File data corrupted");
			break;
		case 0x8A084033:
			strcat(szTextBuffer, ": Not connected");
			break;
	}
	
	return szTextBuffer;
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
unsigned int uGetLastErrorCode(void)
{
	return uLastError;
}

/************************************************************************/
/* Funktion SwitchConnector_1ThreadFunction	- Switch Connector			*/ 
/************************************************************************/
int CVICALLBACK SwitchConnector_1ThreadFunction (void *data)
{ 
	// Promik Switch Connector
	uLastError = XCP_SRR300_SwitchConnector( 	0x010, //Switches Off Voltage monitoring, Promik tool use 0x0768
									 iFlashController	//Programmer  
											); 
	
	if (CheckResult_Silent("SwitchConnector", uLastError)) {
		boThreadSwitchConnector = False;
	}
	else {
		boThreadSwitchConnector = True;
	}
	
	return 1;	
}


/************************************************************************/
/* Funktion SwitchConnector_2ThreadFunction	- Power On				   	*/ 
/************************************************************************/
int CVICALLBACK SwitchConnector_2ThreadFunction (void *data)
{
	// Spannung einschalten  
	Sleep(100);		
	if (boHpPsSetOutput(&g_HpSupplyPrimary, On)){
		boThreadSwitchPower = True;
	}
	else {
		boThreadSwitchPower = False;
	}
	
	return 1;	
}

//*****************************************
// Wait for all threads to complete
//*****************************************
int Thread_Switch_Connector(void)
{
	int iZaehler = 0,
	iThreadId[2] = {0};

// Hier kann über ThreadPool einiges parallelisiert werden, wobei in app_Start mit CmtNewThreadPool ein Pool angelegt werden muß:
// CmtScheduleThreadPoolFunction, CmtWaitForThreadPoolFunctionCompletion
	
	// Variablen zurücksetzen
	boThreadSwitchConnector = False;
	boThreadSwitchPower = False;	
	
	// Starten des Threads
	CmtScheduleThreadPoolFunction(g_hThreadPool, SwitchConnector_1ThreadFunction, 0, &iThreadId[0]);
		
	// Starten des Threads
	CmtScheduleThreadPoolFunction(g_hThreadPool, SwitchConnector_2ThreadFunction, 0, &iThreadId[1]);
			
	// wait for all the Thread finished
	for( iZaehler = 0; iZaehler < 2; iZaehler++ ) {
		
		if( CmtWaitForThreadPoolFunctionCompletion(g_hThreadPool, iThreadId[iZaehler], OPT_TP_PROCESS_EVENTS_WHILE_WAITING) ) {
			continue;
		}
	}
		
	if ((boThreadSwitchConnector == True) && (boThreadSwitchPower == True)) {
		return 1;
	}
	else if((boThreadSwitchConnector == True) && (boThreadSwitchPower == False)) { 
		return 2;
	}
	else if((boThreadSwitchConnector == False) && (boThreadSwitchPower == True)) { 
		return 3;
	}
	else {
		return 0;
	}
		
}

/********************************************************************************/
/****************************Application Functions*******************************/
/********************************************************************************/

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXGetDllVersion(float * pfVersion)
{
	unsigned short ausVersion[4];
	uLastError = XCP_SRR300_GetDllVersion(&ausVersion[0], &ausVersion[1], &ausVersion[2], &ausVersion[3]);
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
	*pfVersion = ausVersion[0] + (float)ausVersion[1] / 10.0 + (float)ausVersion[2] / 100.0 + (float)ausVersion[3] / 1000.0;
	
	return 1;
}


/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXOpenInterface(void)
{
	float fCurrent = 0;
	int boResult = 0;
	
	if (boTargetConnected)
	{
		boResult = boProMikSRR3XXCloseInterface();
			if (!boResult) {
			WriteToErrorWin("Error open/closing interface: %s", szGetErrorString(uGetLastErrorCode())); 
			return(0);
		}
	}
	
	uLastError = XCP_SRR300_ConfigureDll(IPAddr, //MSP2K IP-Address Normally 92,168,1,1:15504
										   NULL, //allow to submit additional tex and inform about the thread callback
										      0, //callback routin used
										SUBTYPE	 //Family Device 1=SRR3XX
											  ); 
	if (iErrorState(uLastError))
	{
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	
	return 1;
}

/************************************************************************************/
/* Version 1.00                                                                 	*/
/* Author:fehrmanng	                                                            	*/
/*																                	*/															
/* Avoid Thread_Switch_Connector use XCP_SRR300_SwitchConnector directly instead 	*/
/************************************************************************************/
int boProMikSRR3XXOpenConnection(int iProgrammer)
{

	DWORD dwSeedKey;
	unsigned char ac_Seed[LEN_SEED_KEY];
	unsigned char ac_Key[LEN_SEED_KEY];
	unsigned char byteLenKey = LEN_SEED_KEY;
	VKeyGenResultEX ResultEx;
//	unsigned int ul_KeySize = 0;
	unsigned char privilege;
	
	float fCurrent = 0;  
	int iResult = 0;
	//double dConnectionTime = 0;
	
	if (!XCP_ComputeKeyFromSeed)
	{
		LinkSeedKeyAPI();
		if (!XCP_ComputeKeyFromSeed)
		{
			return 0;
		}
	}
	
	if (boTargetConnected)
	{
		boProMikSRR3XXCloseConnection();
	}
	
	iFlashController = iProgrammer;
	
	//------- Initialize programmer ------- 
	uLastError = XCP_SRR300_InitBase(   0xC8, 				   //Reset Ident when MSP2K is Reseted value is 0, Values Allowed Fom 1 to 255
										0, 				   // No Meaning in SRR3XX V1.0
										1, 				   // 0=1 MB/s // 1=500 kB/s // 2= 250 kB/s // 3=125kB/s	   //  320MA->125
										XCP_DTO_ID_PSW,   //CAN ID of Target Device which receives commands
										XCP_CRO_ID_PSW,   //CAN ID of Target Device which sends Responses
										iFlashController); // Programmer Number from 1 to 12
	if (iErrorState(uLastError) == eError)
	{
		WriteToErrorWin("XCP_SRR300_InitBase: Error opening Connection: %s", szGetErrorString(uGetLastErrorCode())); 
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	
	uLastError = XCP_SRR300_SelectTarget( TARGET,  iFlashController);  
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("XCP_SRR300_SelectTarget: Error opening Connection: %s", szGetErrorString(uGetLastErrorCode())); 
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	
	//------- Catch Bootloader --------

	//dConnectionTime = Timer();

	
	iResult = Thread_Switch_Connector(); // No thread Used For Debug Purpouse
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("Thread_Switch_Connector: uLastError = 0x%X; iResult = %i", uLastError, iResult);
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Stromaufnahme: %f", fCurrent);
		return 0;
	}
	if( iResult == 2 ) {	 // If power supply didn't work
		WriteToErrorWin("Thread_Switch_Connector: power supply doesn't work");
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}

	//WriteToDataWin("Catch Bootloader: %.3f s", Timer() - dConnectionTime); 
else if( iResult == 3){
		WriteToErrorWin("Thread_Switch_Connector: XCP_SRR300_SwitchConnector doesn't work");
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}

		//------- Enter Programming mode -------
	privilege = 0x10; // PGM
	uLastError = XCP_SRR300_Security( 0, privilege, &dwSeedKey, 0, iFlashController); //Request Seed, privilege to enter, Seed at returning, Programmer
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("XCP_SRR300_Security 0x10: Error opening Connection: %s", szGetErrorString(uGetLastErrorCode()));
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}	 
	
	ac_Seed[0] = (unsigned char)(dwSeedKey >> 24);
	ac_Seed[1] = (unsigned char)(dwSeedKey >> 16);
	ac_Seed[2] = (unsigned char)(dwSeedKey >> 8);
	ac_Seed[3] = (unsigned char)dwSeedKey;
	
	ResultEx = XCP_ComputeKeyFromSeed(privilege, LEN_SEED_KEY, ac_Seed, &byteLenKey, ac_Key);
	dwSeedKey = (ac_Key[0] << 24) | (ac_Key[1] << 16) | (ac_Key[2] << 8) | ac_Key[3];
	
	uLastError = XCP_SRR300_Security( 1, privilege, &dwSeedKey, 0, iFlashController); //Send Key, privilege to enter, Key, Programmer
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("XCP_SRR300_Security 0x10 (2): Error opening Connection: %s", szGetErrorString(uGetLastErrorCode())); 
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	
	privilege = 0x01; // CAL/PAG
	
	uLastError = XCP_SRR300_Security( 0, privilege, &dwSeedKey, 0, iFlashController); //Request Seed, privilege to enter, Seed at returning, Programmer
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("XCP_SRR300_Security 0x01: Error opening Connection: %s", szGetErrorString(uGetLastErrorCode()));
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	
	ac_Seed[0] = (unsigned char)(dwSeedKey >> 24);
	ac_Seed[1] = (unsigned char)(dwSeedKey >> 16);
	ac_Seed[2] = (unsigned char)(dwSeedKey >> 8);
	ac_Seed[3] = (unsigned char)dwSeedKey;
	
	ResultEx = XCP_ComputeKeyFromSeed(privilege, LEN_SEED_KEY, ac_Seed, &byteLenKey, ac_Key);
	dwSeedKey = (ac_Key[0] << 24) | (ac_Key[1] << 16) | (ac_Key[2] << 8) | ac_Key[3];
	
	uLastError = XCP_SRR300_Security( 1, privilege, &dwSeedKey, 0, iFlashController); //Send Key, privilege to enter, Key, Programmer
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("XCP_SRR300_Security 0x01 (2): Error opening Connection: %s", szGetErrorString(uGetLastErrorCode()));
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	
	//------- Load SWIL Bootloader -------

	//dConnectionTime = Timer();

	uLastError = XCP_SRR300_DownloadBootloader( 6, iFlashController); // 3 == 320MA (Mazda) ; Programmer
	if (iErrorState(uLastError))
	{
		WriteToErrorWin(" XCP_SRR300_DownloadBootloader: Error opening Connection: %s", szGetErrorString(uGetLastErrorCode())); 
		boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent);
		WriteToErrorWin("Current Consumption: %f", fCurrent);
		return 0;
	}
	

	//WriteToDataWin("Load SWIL Bootloader: %.3f s", Timer() - dConnectionTime);

	
	boTargetConnected = 1;
	
	return 1;
}


/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXLoadFile(char * szFileName, unsigned short * pusFileChecksum)
{
	uLastError = XCP_SRR300_LoadFile(szFileName, 1, FILL_VALUE);
	if (iErrorState(uLastError) == eError)
	{
		return 0;
	}
	
	if (pusFileChecksum != NULL)
	{
		uLastError = XCP_SRR300_GetFileChecksum(0);
		if (iErrorState(uLastError))
		{
			return 0;
		}
		
		*pusFileChecksum = uLastError & 0xFFFF;
	}
	
	return 1;
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXMergeWithFile(char * szFileName)
{
	uLastError = XCP_SRR300_MergeFile(szFileName , 0); 
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
 return 1;
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXChangeBaudrate(int iBaudrateIndex)
{
		
	uLastError = XCP_SRR300_SetBaudRate(iBaudrateIndex, iFlashController);
 	if (iErrorState(uLastError) == eError)
		{
			return 0;
		}
	 
 	return 1;
}
	 
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/*U:\PMB-DOKU\ADC\SRR3xx\Pomik_CAN_Flash\PROMIK ERROR_Description.xlsx			*/
/*For 3XX is a nue change for the Address to erase based on CUT1 and the memory	*/
/*mapping, in the document and the DLL document is a help about this address	*/
/*	nAS	0000000000000111			0007										*/							
/*	HAS	00111100					3C											*/					
/*	MAS	0000						0											*/					
/*	LAS	1100						C											*/
/*			Resulting Value			00073C0C									*/		
/*			DWORD:					0x00073C0C									*/	
/********************************************************************************/
int boProMikSRR3XXEraseAreaToWrite(unsigned int dwEraseArea, int iFlashController)
{
	if (!boTargetConnected)
	{ // Nicht verbunden -> false
		return 0;
	}
	
	//------- Erase sector ------- 
	//uLastError = XCP_SRR300_EraseFlash(uiStartAddr , /* any address inside first sector to erase*/ uiEndAddr - uiStartAddr, /* bytes to erase from given address at least*/ iFlashController ); // Programmer 
	uLastError = XCP_SRR300_EraseFlash(dwEraseArea,			//DWord to Erase 
										iFlashController ); // Programmer 
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
	return 1;
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXProgramFile(unsigned short * pusChecksum)
{
	if (!boTargetConnected)
	{ // Nicht verbunden -> false
		return 0;
	}
	
	uLastError = XCP_SRR300_ProgramArea( 0x0, /* Start address*/ 0x0, /* End address*/ FILL_OPTION, /* Fill Option*/ pusChecksum, /* Output: Checksum*/ iFlashController); // Programmer 
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("Program File: SRR300_ProgramArea: %s,%s,%s,%i,%s", "0x0","0x0",FILL_OPTION,pusChecksum,iFlashController);
		return 0;
	}
	
	uLastError = XCP_SRR300_VerifyCheckSum(0x0, /* Start address*/ 0x0, /* End address*/ NULL, /* Source memory*/ VERIFY_OPTION, /* Flag byte*/ FILL_VALUE, /* Fill value*/ iFlashController ); // Programmer      
	if (iErrorState(uLastError))
	{
		WriteToErrorWin("Program File: SRR300_VerifyCheckSum: %s,%s,%s,%s,%s", "0x0","0x0",VERIFY_OPTION,FILL_VALUE,iFlashController);
		return 0;
	}
	
	return 1;
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
/*int boProMikSRR3XXInitializeEEPROM()
{
	if (!boTargetConnected)
	{ // Nicht verbunden -> false
		return 0;
	}
	
	//uLastError = XCP_SRR300_EepromFill(Start_address,End_address,Fill_Value,iFlashController); // Programmer
	uLastError = XCP_SRR300_EepromFill( 0x0000, 0x1FFF, 0x00, iFlashController); // Programmer 
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
	return 1;
}
*/
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXGetChecksumOnTarget(unsigned int uiStartAddr, unsigned int uiEndAddr, unsigned long * pulChecksum)
{
	if (!boTargetConnected)
	{ // Nicht verbunden -> false
		return 0;
	}
	uLastError = XCP_SRR300_GetCheckSum( pulChecksum, uiStartAddr, uiEndAddr, 0x02, iFlashController);
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
	return 1;
}
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXCloseInterface(void)
{
	
	if (boTargetConnected)
	{
		boProMikSRR3XXCloseConnection();
	}
	
	XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
	return 1;
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
int boProMikSRR3XXCloseConnection(void)
{
	if (iFlashController == 0)
	{
		return 0;
	}
	
	boTargetConnected = 0;
	
	XCP_SRR300_SwitchConnector(0x0, iFlashController);
	
	return 1;
}

/********************************************************************************/
/********************************************************************************/
/*********************************  OLD  ****************************************/
/********************************************************************************/
/********************************************************************************/


/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:MedinaA	                                                            */
/*   Not implemented, only implemented in TestOnlineCanXCPm             */
/********************************************************************************/
/*int boProMikSRR3XXCRstSystem(void)
{
	
	uLastError = XCP_SRR300_SystemReset( iFlashController );
	
	if (iErrorState(uLastError))
	{
		return 0;
	}
	
	return 1;
}   */

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:MuellerM02	                                                          */
/* Update Firmware of ProMik to SRR3xx  												               */
/********************************************************************************/
int boUpdatePromikFirmware(void) {
	
	float fCurrent = 0;
	WORD  wMajor;
	WORD wMinor;
	WORD wMajbuild;
	WORD wMinbuild;
	WORD FirmwareVersion;
	float fDLLversion;
	 char statmsg[100];
	//uLastError =  XCP_SRR300_GetDllVersion(&wMajor,&wMinor,&wMajbuild,&wMinbuild);
	
	Sleep(100); 
	XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);

	
	Sleep(100); 
	XCP_SRR300_ConfigureDll(IPAddr, NULL, 1, SUBTYPE);
	
	Sleep(300);
	uLastError = XCP_SRR300_SoftwareUpdate(2, 0, 1);
	
	
	szGetErrorString(uLastError);
	
	if(uLastError != 0x42110021){ //this error code for checking on V version
		if(iErrorState(uLastError) == 0)
		{
			
	  			//MessagePopup("Promik ok","Promik FirmwareVersion is compatible: No Upgrade needed");
				WriteToErrorWin("Promik FirmwareVersion is compatible: No Upgrade needed");
				return( 1 );
		}
		else
		{
			MessagePopup("Promik:","szGetErrorString(uLastError)"); 
			return( 0 );
		}
			MessagePopup("Promik:","start Promik Upgrade");
	}
	
	

	
	 XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);  
	 
	 sprintf(statmsg,"Ongoing Promik update! please wait...");
	 WriteToStatusWin(statmsg); 
	 

	
	 if (!boDowngradePromikFirmware())
	 {
		 return 0;
	
	 }
	return 1;

	// Set up the thread pool for SwitchConnector
	//if( !g_hThreadPool2 ) {
	//	if( CmtNewThreadPool(2, &g_hThreadPool2) ) {
	//		return( 0 );
	//	}
	//}
	 
	 //Flash Firmware
	//iStartThreads();

}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:MuellerM02	                                                          */
/* Downgrade Firmware of ProMik to SRR2xx   													               */
/********************************************************************************/
int boDowngradePromikFirmware(void) 
{

//from SRR2xx --- int boProMikSRR295OpenInterface(void)
// Verbindung zum Gerät herstellen
	short IsSuccess = 1;
	
	  Sleep(200);
	  uLastError = XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
	  
	  Sleep(200);
	  uLastError = XCP_SRR300_ConfigureDll(IPAddr, NULL, 1, SUBTYPE); 
	  	if (iErrorState(uLastError))
			{   
	
			}
	
	  Sleep(1000);
	  uLastError = XCP_SRR300_SoftwareUpdate(2, 2, 1); 
	  Sleep(50);
	  if( (uLastError & 0x80000000) ) {				   //Bit 32 means ERROR 
		//MessagePopup("ERROR", "ERROR at: >>  XCP_SMR200_SoftwareUpdate(2, 2, 1) <<");        
		
			Sleep(200);
		
			uLastError = XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
		
			Sleep(200);
		
			uLastError = XCP_SRR300_ConfigureDll(IPAddr, NULL, 1, SUBTYPE); 
	  		if (iErrorState(uLastError))
			{   
	
			}
	
	  		Sleep(1000);
	  		uLastError = XCP_SRR300_SoftwareUpdate(2, 2, 1); 
	  		Sleep(50);
			if( (uLastError & 0x80000000) ) {				   //Bit 32 means ERROR   
			          	Sleep(200);
	  					uLastError = XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
	  
	  					Sleep(200);
	  					uLastError = XCP_SRR300_ConfigureDll(IPAddr, NULL, 1, SUBTYPE); 
	  
	
	  					Sleep(500);
	  					uLastError = XCP_SRR300_SoftwareUpdate(2, 0, 1); 
	  					Sleep(50); 
						szGetErrorString(uLastError);
						
						 if(uLastError != 0x42110021){
							if(iErrorState(uLastError) == 0){
									//MessagePopup("Promik ok","Promik Firmwareversion is compatible: No Downgrade needed");
								   WriteToErrorWin("Promik FirmwareVersion is upgraded"); 
								}
								else
								{
						   			IsSuccess = 0;
								}
								}
						 else if(uLastError != 0x4201001D){  
					   		if(iErrorState(uLastError) == 0){ 
									//MessagePopup("Promik ok","Promik Firmwareversion  update is done");
									WriteToErrorWin("Promik FirmwareVersion is upgraded"); 
								}
								else
								{
						   			IsSuccess = 0;
								}
						  	    }
					     else 
						        {
						   	          IsSuccess = 0;
						        }
			
			
			   	         		if (!IsSuccess)  //last try of updating
				 				{
				    				Sleep(300);
										uLastError = XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
									Sleep(300);
			    						uLastError = XCP_SRR300_ConfigureDll(IPAddr, NULL, 1, SUBTYPE); // IP-Adresses, Callback, Using Callback, Family Device  
	
									Sleep(1000);
										uLastError = XCP_SRR300_SoftwareUpdate(2, 2, 1);   
										//Sleep(100);    
				 
										/*if( (uLastError & 0x80000000) ) {				   //Bit 32 means ERROR 
											//MessagePopup("ERROR", "ERROR at: >>  XCP_SMR200_SoftwareUpdate(2, 2, 1) <<");        
											uLastError = XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
										}*/
			 
				 				}
			
			}
		
	  }		
			
			
	  
	  Sleep(100); 
	  
	  uLastError = XCP_SRR300_ConfigureDll(NULL, NULL, 0, 0);
	  	if (iErrorState(uLastError))
		{
		MessagePopup("ERROR", "ERROR at: >> XCP_SMR200_ConfigureDll(NULL, NULL, 0, 0) <<");
		return 0;
		}
	
	return 1;
}

int CVICALLBACK Thread_doPromik (void *data)
{ 

	//Sleep(PROMIKTIME); //Simulate the Promik Firmware flash
if (boDowngradePromikFirmware())
	{		
	boThreadPool2Finished = 1;
	}
	SetPanelAttribute (g_hdlToolPanel, ATTR_MOUSE_CURSOR, VAL_DEFAULT_CURSOR);  
	return 1;	
}

int CVICALLBACK Thread_doMousePointer (void *data)
{ 
	 	int counter=0;int iTimeout = PROMIKTIME;  
		char statmsg[100];
		
		while(!boThreadPool2Finished){
			Sleep(100);
			if(counter > iTimeout){
				break;
			}
			else{
				counter++;
			}
			//Do change mouse pointer till thread has finished    
			SetPanelAttribute (g_hdlToolPanel, ATTR_MOUSE_CURSOR, VAL_HOUR_GLASS_CURSOR);
			sprintf(statmsg,"Ongoing Promik update! please wait...");
			WriteToStatusWin(statmsg); 
	}
	
	return 1;	
}


int iStartThreads(void){
	
	
	int iThreadId[2] = {0};
	int iZaehler = 0;
	
	boThreadPool2Finished = 0;   
	CmtScheduleThreadPoolFunction(g_hThreadPool2, Thread_doPromik, 0, &iThreadId[0]);
	CmtScheduleThreadPoolFunction(g_hThreadPool2, Thread_doMousePointer, 0, &iThreadId[1]);
	
	
	//Ende
	// wait for all the Thread finished
	for( iZaehler = 0; iZaehler < 2; iZaehler++ ) {
		
		if( CmtWaitForThreadPoolFunctionCompletion(g_hThreadPool2, iThreadId[iZaehler], OPT_TP_PROCESS_EVENTS_WHILE_WAITING) ) {
			continue;
		}
	}
	

	
	return 1;
}
