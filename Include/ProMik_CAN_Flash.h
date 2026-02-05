/********************************************************************************/
/*                                                                           		*/
/*  Header File: ProMik_SRR3XX_CAN_Flash.h			                             		*/
/*                                                                           		*/
/********************************************************************************

	Version 		Date				Author		 Changes
	------------------------------------------------
	V1.00		   26.09.2011   fehrmanng	 Initial realization
	V1.01			 10.10.2011		fehrmanng  Adaption of SMR_Promik_Flash.c structure
	V1.10			 01.03.2012		fehrmanng  Function changed: boProMikSRR3XXEraseAreaToWrite, XCP_SMR200_Security (new parameter DLL 3.0.0.0)
																		 Function added: boProMikSRR3XXGetChecksumOnTarget, boProMikSRR3XXInitializeEEPROM()	
*********************************************************************************

	Documentation:
	--------------
	Driver layer for flash process with Promik CAN_XCP_SMR200mOnline functions  

*********************************************************************************/
#ifndef _ProMik_CAN_Flash_h_
#define _ProMik_CAN_Flash_h_ 

#include <CviTypes.h>

void __stdcall DisplayFunction(unsigned long err, LPSTR text);
int CheckResult(LPSTR text, DWORD dwError);
int CheckResult_Silent(LPSTR text, DWORD dwError);


int CVICALLBACK SwitchConnector_1ThreadFunction (void *data); 
int CVICALLBACK SwitchConnector_2ThreadFunction (void *data);
int Thread_Switch_Connector(void);
int MPC567_Switch_Connector_ON(void);

int iErrorState(unsigned int uError);
int iErrorType(unsigned int uError) ;
const char * szGetErrorString(unsigned int uError);
unsigned int uGetLastErrorCode(void);
int boProMikSRR3XXGetDllVersion(float * pfVersion);
int boProMikSRR3XXOpenInterface(void);
int boProMikSRR3XXOpenConnection(int iProgrammer);
int boProMikSRR3XXLoadFile(char * szFileName, unsigned short * pusFileChecksum);
int boProMikSRR3XXMergeWithFile(char * szFileName);
//int boProMikSRR295                                                                                              ChangeBaudrate(int iBaudrateIndex); No Baudrate Funtion in SRR3XX
int boProMikSRR3XXEraseAreaToWrite(unsigned int dwEraseArea, int iProgrammer);
int boProMikSRR3XXProgramFile(unsigned short * pusChecksum);
//int boProMikSRR3XXInitializeEEPROM(void); No function in SRR3xX
int boProMikSRR3XXGetChecksumOnTarget(unsigned int uiStartAddr, unsigned int uiEndAddr, unsigned long * pulChecksum);
int boProMikSRR3XXCloseInterface(void);
int boProMikSRR3XXCloseConnection(void);
int boProMikSRR3XXCRstSystem(void);
int boProMikSRR3XXChangeBaudrate(int iBaudrateIndex);


Boolean boProMikSRR3XXInit(void);
Boolean boProMikSRR3XXProgramAppSW(void);
Boolean boProMikSRR3XXProgramPSW(void);
Boolean boProMikSRR3XXDisconnect(void);

int boUpdatePromikFirmware(void); 
int boDowngradePromikFirmware(void);

#endif 
