/********************************************************************************/
/*                                                                           		*/
/*  Source File: XcpViaCan.c     			                   	               		*/
/*                                                                           		*/
/********************************************************************************

	Version 		Date				Author		 Changes
	------------------------------------------------
	V1.00		   16.09.2011   fehrmanng	 Initial realization
	V1.01			 24.11.2011		fehrmanng  Function: int iXcpUploadWord(unsigned char aucRxBuff[] );   
*********************************************************************************

	Documentation:
	--------------
	Driver layer for XCP-Commands via Goepel-Can Hardware (G_Api)
	Based on XcpCan_V105.c by DunzM

*********************************************************************************/
#include <CviTypes.h>
#include <windows.h>
#include <utility.h>
#include <ansi_c.h>
#include <toolbox.h>

#include <g_api.h>
#include <g_api_can.h>
#include <g_api_common.h>
#include <g_api_diag.h>
	
#include <Can.h>

#include <XcpViaCan.h>



//************************************************************************************************
#define _MAX_XCP_ERR_LEN_			4096
#define _BufferSize_ 					2048*3000
#define _EXTENDED_ID_					0x80000000

#define BLOCK_SIZE						30
#define MAX_UPLOAD_LEN				(2) // (0xFF)

//************************************************************************************************
const u32_t  u32XcpTimeoutMs = 500;
const unsigned int uXcpTimeout = 1000;	// overall timeout in ms
const unsigned int uXcpTimeoutGetAcquireState = 2500;

//************************************************************************************************
static G_Common_EventHandle_t  tXcpEventHandle;

static u32_t u32XcpCtoId = 0;
static u32_t u32XcpDtoId = 0;

static Boolean	boXcpBufferModeEnableEvent = False;
static G_Can_Monitor_BufferItem_t tXcpBufferItems[3 * BLOCK_SIZE + 1];


/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*           		      												*/
/********************************************************************************/
void XcpSetError(unsigned long ulErrorNb, char *pFormat, ...)
{
	va_list lArgPointer;

	va_start (lArgPointer, pFormat);
	vsprintf (g_szCanErrorText, pFormat, lArgPointer);

	g_uCanError = 0x10000000 + ulErrorNb;
}
	
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Open Can-XCP interface								           		      												*/
/********************************************************************************/
int iXcpOpen(const unsigned int hCanPort, unsigned int uBaudRate)
{
	G_Can_Node_Baudrate_ActualValues_t tBaudrateValues = {0};

	/* -- KEINE INITIALISIERUNG DER KARTE ALS WORKAROUND FÜR EINE LAUFENDE RESTBUSSIMULATION WÄHREND DER XCP VERBINDUNG -- 
	// Karte initialisieren 
	g_uCanError = G_Common_OpenInterface(szInterfaceName, &g_hCanPort);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	
	g_uCanError = G_Can_InitInterface(g_hCanPort, G_CAN__ID_MODE__MIXED, G_CAN__INIT_INTERFACE__CMD_FLAG__NONE);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	/*/
	g_hCanPort = hCanPort;
	//*/
		
	// allocate event
	g_uCanError = G_Common_Events_AllocateEvent(g_hCanPort, &tXcpEventHandle);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	
	g_uCanError = G_Can_Node_Baudrate_Set(g_hCanPort, uBaudRate, NULL, &tBaudrateValues);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	
	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Close Can-XCP interface           		      												  						*/
/********************************************************************************/
int iXcpClose(void)
{
	if( u32XcpCtoId != 0 ) {
		iXcpReset();
		u32XcpCtoId = 0;
	}
	
	
	g_uCanError = G_Common_Events_DeallocateEvent(g_hCanPort, &tXcpEventHandle);
	if( iCanCheckError() ) {
		//return( g_uCanError );
	}
	tXcpEventHandle = 0;

	/* -- KEINE DEINITIALISIERUNG DER KARTE ALS WORKAROUND FÜR EINE LAUFENDE RESTBUSSIMULATION WÄHREND UND AUCH NACH DER XCP VERBINDUNG -- 
	g_uCanError = G_Common_SoftwareReset(g_hCanPort); 
	if( iCanCheckError() ) {
		//return( g_uCanError );
	}

	// Karte schließen
	g_uCanError = G_Common_CloseInterface(g_hCanPort);
	if( iCanCheckError() ) {
		//return( g_uCanError );
	}
	*/
	g_hCanPort = 0;
	
	/*if( pusXcpImageData != NULL ) {
		free(pusXcpImageData);
		pusXcpImageData = NULL;
	}
	if( puszXcpImage != NULL ) {
		free(puszXcpImage);
		puszXcpImage = NULL;
	} */

	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Init XCP communication           		      																	*/
/********************************************************************************/
int iXcpInit(unsigned int uCtoId, unsigned int uDtoId, Boolean boExtendedIds)
{
	u32XcpCtoId = boExtendedIds ? uCtoId | _EXTENDED_ID_ : uCtoId;
	u32XcpDtoId = boExtendedIds ? uDtoId | _EXTENDED_ID_ : uDtoId;
	
	
	
	g_uCanError = G_Can_CyclicMsgs_DefineMsg(g_hCanPort, u32XcpCtoId, 0, G_CAN__CYCLIC_MSGS__SEND_STATE__DONT_SEND_MSG, 0, 1, 
																					(unsigned char*)"\xFF\x00\x00\x00\x00\x00\x00\x00", 8);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
		

	g_uCanError = G_Can_Monitor_DefineFilter(g_hCanPort, G_CAN__MONITOR__DEFINE_FILTER__MODE__RANGE , u32XcpDtoId, u32XcpDtoId);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}


	g_uCanError = G_Can_Monitor_BufferMode_Start(g_hCanPort, G_CAN__MONITOR__BUFFER_MODE__MODE__RX, _BufferSize_);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}

	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Reset XCP communication           		      																	*/
/********************************************************************************/
int iXcpReset(void)
{
	g_uCanError = G_Can_Monitor_BufferMode_Stop(g_hCanPort);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	boXcpBufferModeEnableEvent = False;
	
	g_uCanError = G_Can_CyclicMsgs_DeleteMsg(g_hCanPort, u32XcpCtoId);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	u32XcpCtoId = 0;

	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Change baudrate           		      																					*/
/********************************************************************************/
int iXcpChangeBaudRate(EBaudRate eBaud)
{		
	unsigned char uszReq[8 + 1] = "\xF1\x00\x00\x00\x00\x00\x00\x00";

	
	uszReq[1] = eBaud;
	if( iXcpReq(uszReq) ) {
		return( g_uCanError );
	}


	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Send XCP request           		      																				*/
/********************************************************************************/
int iXcpReq(unsigned char uszReq[])
{		
	const u8_t  u8Mask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	
	g_uCanError = G_Can_CyclicMsgs_ChangeMsgData(g_hCanPort, u32XcpCtoId, u8Mask, 8, uszReq, 
							G_CAN__CYCLIC_MSGS__CHANGE_MSG_DATA__CHANGE_MODE__IMMEDIATELY, 0);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}

	g_uCanError = G_Can_CyclicMsgs_StartMsg(g_hCanPort, u32XcpCtoId);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
		

	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Send XCP request with response		           		      												*/
/********************************************************************************/
int iXcpReqRes(unsigned char uszReq[], unsigned char *uszRes)
{		
	u32_t u32NumberOfItems = 0;
	const u8_t  u8Mask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	
	if( !boXcpBufferModeEnableEvent ) {
		// enable monitor event
		g_uCanError = G_Can_Monitor_BufferMode_EnableEvent(g_hCanPort, &tXcpEventHandle);
		if( iCanCheckError() ) {
			return( g_uCanError );
		}
		boXcpBufferModeEnableEvent = True;
	}
	
	g_uCanError = G_Can_CyclicMsgs_ChangeMsgData(g_hCanPort, u32XcpCtoId, u8Mask, 8, uszReq, 
							G_CAN__CYCLIC_MSGS__CHANGE_MSG_DATA__CHANGE_MODE__IMMEDIATELY, 0);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}

	g_uCanError = G_Can_CyclicMsgs_StartMsg(g_hCanPort, u32XcpCtoId);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
		
	g_uCanError = G_Common_Events_WaitForSingleEvent(g_hCanPort, &tXcpEventHandle, u32XcpTimeoutMs);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}

	memset(tXcpBufferItems, 0, sizeof(tXcpBufferItems));
	g_uCanError = G_Can_Monitor_BufferMode_GetItems(g_hCanPort, _BufferSize_, &u32NumberOfItems, tXcpBufferItems);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}
	if( u32NumberOfItems < 1 ) {
		XcpSetError(1, "No Response for %s\n", pHex2String(uszReq, 8));
		return( g_uCanError );
	}
	
	if( tXcpBufferItems[0].Data[0] != 0xFF ) {
		XcpSetError(2, "Negative Response (%s) \n", pHex2String(tXcpBufferItems[0].Data, 8));
		return( g_uCanError );
	}
	

	memcpy(uszRes, &tXcpBufferItems[0].Data[0], 8);
	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* XCP connect           		      																							*/
/********************************************************************************/
int iXcpConnect(void)
{		
	unsigned char uszRes[8 + 1] = {0};
	
	
	// Connect
	if( iXcpReqRes((byte*)"\xFF\x00\x00\x00\x00\x00\x00\x00", uszRes) ) {	  // srr200   
		return( g_uCanError );
	}
	
	// GetCommModeInfo
	//if( iXcpReqRes((byte*)"\xFB\x00\x00\x00\x00\x00\x00\x00", uszRes) ) {
	//	return( g_uCanError );
	//}
	

	return( 0 );
}


/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* XCP disconnect           		      																					*/
/********************************************************************************/
int iXcpDisConnect(void)
{		
	
	if( iXcpReq((byte*)"\xFE\x00\x00\x00\x00\x00\x00\x00") ) {			 // srr200 
		return( g_uCanError );
	}
	
	return( 0 );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/* Set address           		      																							*/
/********************************************************************************/
int iXcpSetMta(unsigned int uAdr)
{		
	unsigned char uszRes[8 + 1] = {0};
	unsigned char uszReq[8 + 1] = "\xF6\x00\x00\x00\x00\x00\x00\x00";
	
	
	uszReq[4] = uAdr >> 24;
	uszReq[5] = uAdr >> 16;
	uszReq[6] = uAdr >> 8;
	uszReq[7] = uAdr;

	if( iXcpReqRes(uszReq, uszRes) ) {
		return( g_uCanError );
	}
	

	return( 0 );
}
/********************************************************************************/
/* Version 1.01                                                                 */
/* Author:fehrmanng	                                                            */
/* XCP upload: load data from address          		      												*/
/* Check for missing answer: 3 retries, if waiting time too short 							*/
/********************************************************************************/
int iXcpUploadWord(unsigned char aucRxBuff[] )
{
	u32_t u32NumberOfItems = 0;
	const u8_t  u8Mask[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char uszReq[8 + 1] = "\xF5\x00\x00\x00\x00\x00\x00\x00";
	unsigned int iRetryCount = 0;
	
	int iZaehler = 0;
	
	if( boXcpBufferModeEnableEvent ) {
		// disable monitor event
		g_uCanError = G_Can_Monitor_BufferMode_DisableEvent(g_hCanPort);
		if( iCanCheckError() ) {
			return( g_uCanError );
		}
		boXcpBufferModeEnableEvent = False;
	}
	
	uszReq[1] = MAX_UPLOAD_LEN;
	g_uCanError = G_Can_CyclicMsgs_ChangeMsgData(g_hCanPort, u32XcpCtoId, u8Mask, 8, uszReq, 
						G_CAN__CYCLIC_MSGS__CHANGE_MSG_DATA__CHANGE_MODE__IMMEDIATELY, 0);
	if( iCanCheckError() ) {
		return( g_uCanError );
	}

	g_uCanError = G_Can_CyclicMsgs_StartMsg(g_hCanPort, u32XcpCtoId);
		if( iCanCheckError() ) {
			return( g_uCanError );
		}
	
	while (u32NumberOfItems == 0)
	{
		Sleep(5);
		
		memset(tXcpBufferItems, 0, sizeof(tXcpBufferItems));
		g_uCanError = G_Can_Monitor_BufferMode_GetItems(g_hCanPort, _BufferSize_, &u32NumberOfItems, tXcpBufferItems);
		if( iCanCheckError() ) {
			return( g_uCanError );
		}
		
		iRetryCount++;
		if (iRetryCount > 3 && u32NumberOfItems == 0)
		{
			sprintf(g_szCanErrorText, "Descr: \"[XCP] No answer\", Error: \"No data after %d tries\"", iRetryCount);
			return( 8 );
		}

	}
	
	for( iZaehler = 0; iZaehler < u32NumberOfItems; iZaehler++ ) {
				if( tXcpBufferItems[iZaehler].Data[0] != 0xFF ) {
					XcpSetError(9, "No positiv Response received in loop %d: %s\n", iZaehler, pHex2String(tXcpBufferItems[iZaehler].Data, 8));
					return( g_uCanError );
				}
				//printf("XCP-Upload Response %03d: %s\n", iMessageCount + iLoopCounter*BLOCK_SIZE, pHex2String(tBufferItems[iZaehler].Data, 8));
				memcpy(&aucRxBuff[iZaehler*MAX_UPLOAD_LEN] , &tXcpBufferItems[iZaehler].Data[1], MAX_UPLOAD_LEN);
				
	}
	
	return( 0 );
}
