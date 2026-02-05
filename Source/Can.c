#include <CviTypes.h>
#include <windows.h>
#include <ansi_c.h>
#include <toolbox.h>

#include <g_api.h>
#include <g_api_can.h>
#include <g_api_common.h>
//#include <g_api_diag.h>

#include <Can.h>


extern void CanWriteError(char szError[]);
//************************************************************************************************

//************************************************************************************************
static char szCanHexString[256];

G_PortHandle_t g_hCanPort = 0;
G_Error_t g_uCanError = 0;
char g_szCanErrorText[_MAX_CAN_ERR_LEN_];


//************************************************************************************************
//char * pszGapiGetFirmwareErrorDescription(unsigned int uReturnCode, unsigned int hPort)
//char * pszGapiGetFirmwareErrorDescription(void)
int iCanCheckError(void)
{
	static char szCanErrorDescription[_MAX_CAN_ERR_LEN_];
	static char szCanErrorFirmwareDescription[_MAX_CAN_ERR_LEN_];
	u32_t uSize = _MAX_CAN_ERR_LEN_;
	

	memset(g_szCanErrorText, 0, sizeof(g_szCanErrorText));

	// No Error occured
	if( g_uCanError == 0 ) {
		return( 0 );
	}
	
	memset(szCanErrorDescription, 0, sizeof(szCanErrorDescription));
	strcpy(szCanErrorDescription, G_GetErrorDescription(g_uCanError));
	memset(szCanErrorFirmwareDescription, 0, sizeof(szCanErrorFirmwareDescription));
	G_Common_GetFirmwareErrorDescription(g_hCanPort, g_uCanError, szCanErrorFirmwareDescription, &uSize);
	sprintf(g_szCanErrorText, "Descr: \"%s\", Error: \"%s\"", szCanErrorDescription, szCanErrorFirmwareDescription);
	return( g_uCanError );
}

//************************************************************************************************
void CanShowErrorNew(char szFile[], int iLine)
{
	static char szErrorText[_MAX_CAN_ERR_LEN_] = {0};
	
	sprintf(szErrorText, "CAN:  %d // %s // In File: %s In Line: %d", g_uCanError, g_szCanErrorText, szFile, iLine);
	CanWriteError(szErrorText);
}

//************************************************************************************************
char *pHex2String(unsigned char uszHex[], int iAnz)
{
	int i = 0;
	char szTemp[5];
	
	memset(szCanHexString, 0, sizeof(szCanHexString));
	for( i = 0; i < iAnz; i++ ) {
		sprintf(szTemp, "%02X ", (unsigned char)uszHex[i]);
		strcat(szCanHexString, szTemp);
	}
	return( szCanHexString );
}


//************************************************************************************************

