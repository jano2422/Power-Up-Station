#include <windows.h>
#include <userint.h>
#include <utility.h>
#include <tcpsupp.h>
#include <ansi_c.h>
#include <LineController.h>


static unsigned int uiConversationHandle = 0;
char szReceiveBuffer [2048];

//----------------------------------------------------------------------------------------------------------
Boolean LineControllerConnect (void)
{
	int iResult;
	int iZaehler = 0;
	
	// try to connect to an already running LineController
	DisableBreakOnLibraryErrors ();
	iResult = ConnectToTCPServer (
		&uiConversationHandle,			//	unsigned int *conversationHandle,
		11000,							//	unsigned int portNumber,
		"127.0.0.1",					//	char *serverHostName,
		ClientTcp_CB,					//	tcpFuncPtr clientCallbackFunction,
		NULL,							//	void *callbackData,
		100);

	
	// if first try failed, try to launch LineControl first
	//	and then to connect
	if (iResult != 0)
	{
		iResult = LaunchExecutable ("LineControlAndTpd.exe");
		if (iResult < 0)
			return False;

		do
		{
			iResult = ConnectToTCPServer (
			&uiConversationHandle,		//	unsigned int *conversationHandle,
			11000,						//	unsigned int portNumber,
			"127.0.0.1",				//	char *serverHostName,
			ClientTcp_CB,				//	tcpFuncPtr clientCallbackFunction,
			NULL,						//	void *callbackData,
			100);
			iZaehler++;
			
		} while( iResult < 0 && iZaehler < 200 ) ;
		
		if (iResult < 0)
			return False;
	}
	EnableBreakOnLibraryErrors ();   
	return True;
}

int LineControllerSend (const char *szFormat, ...)
{
	va_list vArgPointer;
	char szBuffer [2048];
	
	va_start (vArgPointer, szFormat);
	vsprintf (szBuffer, szFormat, vArgPointer);
	
	memset (szReceiveBuffer, 0, sizeof szReceiveBuffer);
	ClientTCPWrite (
		uiConversationHandle,		//	unsigned int conversationHandle,
		szBuffer,					//	void *dataPointer,
		strlen (szBuffer),			//	int dataSize,
		100);						//	unsigned int timeout);
	
	return strlen (szBuffer);
}

Boolean LineControllerProcessReply (const char *szCommand, char *szR1, char *szR2, char *szR3)
{
	int iIndex = 0;
	char *pArg = szReceiveBuffer;
	char *pPtr = szReceiveBuffer;
	char szArg [10][64];
	
	// clear receive params array
	memset (szArg, 0, sizeof szArg);
	
	// wait until response arrives (maybe we have to implement a timeout?)
	while (szReceiveBuffer [0] == '\0')
		ProcessSystemEvents ();
	
	// response has arrived: split response into parameters
	//	note:	with actual FraMES.cfg configuration (KEEP_SIZE = 1),
	//			we expect just three parameters:
	//			LC_CHK:1:<UnitID>-OK
	while ((pPtr = strchr (pPtr, ':')) != 0)
	{
		*pPtr = '\0';
		strcpy (szArg [iIndex], pArg);
		pPtr++;
		pArg = pPtr;
		iIndex++;
		
		if (iIndex >= 4)	// we have just four buffers
			break;			//	so prevent overflow
	}
	strcpy (szArg [iIndex], pArg);
	
	if (strcmp (szCommand, szArg [0]))
		return False;
	
	strcpy (szR1, szArg [1]);
	strcpy (szR2, szArg [2]);
	strcpy (szR3, szArg [3]);
	return True;
}

void LineControllerDisconnect (void)
{
	DisconnectFromTCPServer (uiConversationHandle);
	uiConversationHandle = -1;
}

int CVICALLBACK ClientTcp_CB (unsigned pHandle, int pEvent, int pError, void *pCallBackData)
{
	int lResult;

	switch (pEvent)
	{
		case TCP_DATAREADY:
			lResult = ClientTCPRead (uiConversationHandle, szReceiveBuffer, sizeof szReceiveBuffer, 1000);
			break;

		case TCP_DISCONNECT:
			MessagePopup ("FATAL ERROR", "LineController Terminated");
			exit (0);
			break;
	}
	return 0;
}

