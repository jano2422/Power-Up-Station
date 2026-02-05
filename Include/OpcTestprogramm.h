#ifndef __OpcTestprogramm_H  
#define __OpcTestprogramm_H

//////////////////////////////////////////////////////////////////////////////////////////////////
#include <dataskt.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
#define OpcStringLength		100

//////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	DSHandle Handle;
	Boolean boOut;
	DSEnum_Status Status;
	DSEnum_AccessModes AccessMode;
	unsigned DataType;
	short sValue;
	double dValue; 
	char *szValue;
	char Url[255];
} TOpcData;

typedef enum {
	//********************************************** 
	// static
	SPS_Ready = 0, 
	Start_Test, 
	TempSN,
	FinalSN,
	Tester_Ready,
	Test_PASS,
	Test_FAIL,
	//**********************************************
	// variable
	Adaptor_Id,		// angepasst
	Connector_Id,   // angepasst
	CustomSN,		// angepasst
	SapMaterial,
	//**********************************************
	eSignalAmount	 // nur davor einfuegen, wird zum durchitterien der drueber stehenden variablen bentzt!!!
} ESignal;

//////////////////////////////////////////////////////////////////////////////////////////////////
extern TOpcData g_tOpcData[];

//////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
// Standard **********************************************************
extern Boolean boOpcInit(void);
extern void OpcClose(void);
extern Boolean boOpcReset(void);
extern Boolean boOpcIsQualityGood(void);

extern Boolean boOpcSetSpsReady(TSwitch tOnOff);
extern Boolean boOpcSetStartTest(TSwitch tOnOff);
extern Boolean boOpcSetTesterReady(TSwitch tOnOff);
extern Boolean boOpcSetTestPass(TSwitch tOnOff);
extern Boolean boOpcSetTestFail(TSwitch tOnOff);
extern Boolean boOpcSetTempSn(const char *TempSn);

extern TSwitch tOpcIsSpsReady(void);
extern TSwitch tOpcIsStartTest(void);
extern TSwitch tOpcIsTesterReady(void);
extern TSwitch tOpcIsTestPass(void);
extern TSwitch tOpcIsTestFail(void);
extern Boolean boOpcGetTempSn(char *szTempSn);
extern Boolean boOpcGetSapMaterial(char *szSapMaterial);    

//********************************************************************
// Boolean ***********************************************************
extern Boolean boOpcSetBoolean( ESignal eSignal , TSwitch tOnOff ); 
extern TSwitch tOpcGetBoolean( ESignal eSignal );
// Byte for Boolean (0=undefined; 1= false; 255= true) ***************
extern Boolean boOpcGetByteBoolean(ESignal eSignal, Boolean *bReturn);  
extern Boolean boOpcSetByteBoolean(ESignal eSignal, Boolean bBoolean);   
// Byte **************************************************************
extern Boolean boOpcGetByte(ESignal eSignal, byte *bReturn);  
extern Boolean boOpcSetByte(ESignal eSignal, byte bByte);
extern byte bOpcGetByte(ESignal eSignal);  
// Integer ***********************************************************
extern Boolean boOpcSetInt(ESignal eSignal, int iVariable);
extern Boolean boOpcGetInt(ESignal eSignal, int *iVariable); 
extern int iOpcGetInt(ESignal eSignal); 
// Integer Array *****************************************************
extern Boolean boOpcSetIntArray(ESignal eSignal, int iPosition, int iVariable);
extern Boolean boOpcGetIntArray(ESignal eSignal, int iPosition, int *iVariable);
extern int iOpcGetIntArray(ESignal eSignal, int iPosition);  
// String ************************************************************
extern Boolean boOpcSetString( ESignal eSignal, const char *String ); 
extern Boolean boOpcGetString( ESignal eSignal, char *szString );
extern char *szOpcGetString( ESignal eSignal);
//Double *************************************************************
extern Boolean boOpcGetDoubleArray( ESignal eSignal, int iPosition, double *dReturn );
//******************************************************************** 
//******************************************************************** 

//////////////////////////////////////////////////////////////////////////////////////////////////
#endif
