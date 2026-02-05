#ifndef __GBUS_BASIC_H__
#define __GBUS_BASIC_H__


#include "ModuleManagement.h"
E_MODULE(GBus_Basic)

#include <ansi_c.h>

#ifndef HASHMAP_PTR
#define HASHMAP_PTR 1
typedef void * PHASHMAP;
#endif


#define MSG_LEN 8 // Im Moment haben wir nur CAN
#define STRING_LENGTH 512
#define MAX_PAYLOAD_LENGTH 4096

/* Automatic Messages */

typedef void TMessageSimulation;


typedef void TSimulationListElement;
typedef void TSimulationList;

typedef void TTarMessage;

typedef void TMessageMask;

/* Information Data Structures */
typedef void TCommandData;
typedef void TCommandLibrary;


typedef void TGBusTypeInformation;


typedef void TListeningConfig;



/* Interface-Functions */

typedef int FboCheckActiveInterface(unsigned int hPort, char * szError, char * szFWError);
typedef int FboInitInterface(unsigned int hPort, unsigned char * pucChannel, char * szError, char * szFWError);
typedef int FboReleaseInterface(unsigned int hPort, unsigned char * pucChannel, char * szError, char * szFWError);
typedef int FboStartDiag(unsigned int hPort, unsigned char ucChannel, char * szError, char * szFWError);
typedef int FboStopDiag(unsigned int hPort, unsigned char ucChannel, char * szError, char * szFWError);
typedef int FboDiagRequest(unsigned int hPort, unsigned char ucChannel, unsigned int uTxCount, unsigned char * aucTx, unsigned int * puRxCount, unsigned char * aucRx, char * szError, char * szFWError);
typedef int FboReadMemory(unsigned int hPort, unsigned char ucChannel, unsigned int uTargetMemory, unsigned int uStartAddress, unsigned int * puRxCount, unsigned char * aucRx, char * szError, char * szFWError);
typedef int FboWriteMemory(unsigned int hPort, unsigned char ucChannel, unsigned int uTargetMemory, unsigned int uStartAddress, unsigned int uTxCount, unsigned char * aucTx, char * szError, char * szFWError);


/* Interface Data Structure */
// BASE TYPES
/*
	Abgeleitete Typen sind zwar Pointer- aber nicht Array-Kompatibel!
	D. h. dass ein TCanBusInterface auch als TBusInterface verwendet werden kann,
	jedoch KANN (muss aber nicht) die Länge der Datenstruktur größer sein.
	Die Grundlegenden Elemente sind aber gleich!
	Damit bekomme ich so etwas wie Vererbung.
	
	Die Elemente müssten eigentlich in einem Makro definiert werden, damit dieses Makro für die
	Inherited-Elemente verwendet werden kann; damit wären Änderungen gleich zentral.
	Darauf wird vorerst verzichtet (einfacheres Debugging), möglich ist das aber ganz schnell und einfach!
*/

typedef struct SBusInterface
{ // CAN Bus Abstraction Layer Interface
	// Werte
	const char * szInterfaceName; //!< Name des Interfaces. Üblicherweise Referenz auf konstanten String.
	unsigned int uBaudrate; //!< Baudrate des Bus.
	
	// Funktionen
	FboCheckActiveInterface * boCheckActiveInterface;
	
	FboInitInterface * boInitInterface;
	FboReleaseInterface * boReleaseInterface;
	FboStartDiag * boStartDiag;
	FboStopDiag * boStopDiag;
	FboDiagRequest * boDiagRequest;
	FboReadMemory * boReadMemory;
	FboWriteMemory * boWriteMemory;
	
	
} TBusInterface;



/*!
	@brief Bus-Verbindung als Abstraktionsschicht zur G-Api.
	
	In dieser Datenstruktur werden die Parameter zur aktuellen Bus-Verbindung gespeichert.
	Die Struktur kann für einzelne Bussysteme erweitert ("abgeleitet") werden.
*/
typedef void TBusConnection;


/* Base Functions */

// Generell (auch Flexray später!)
char * G_GetFirmwareErrorDescription(unsigned int uReturnCode, unsigned int hPort);
int boRefreshDriverInfo(char * szError);
//int boGetInterfaceList(...); // Später!
TBusConnection * ptNewEmptyBusConnection(void); // Since V1.03 as of 20120314
const char * szGetLastError(TBusConnection * ptConnection); // Since V1.03 as of 20120314
const char * szGetLastFirmwareError(TBusConnection * ptConnection); // Since V1.03 as of 20120314
const char * szGetErrorMessage(TBusConnection * ptConnection); // Since V1.03 as of 20120314
int boClearErrors(TBusConnection * ptConnection); // Since V1.03 as of 20120314

// Hardware-Spezifisch
int boGetFirmwareVersion(TBusConnection * ptConnection, unsigned int uSize, char * szVersionString);
int boGetEnabledFeatures(TBusConnection * ptConnection, unsigned int uSize, char * szFeatureString);

// Dispatched
int boInitInterface(TBusConnection * ptConnection);
int boReleaseInterface(TBusConnection * ptConnection);
int boStartDiag(TBusConnection * ptConnection);
int boStopDiag(TBusConnection * ptConnection);
int boDiagRequest(TBusConnection * ptConnection, const char * szRequestName, unsigned int uTxCount, unsigned char * aucTx, unsigned int * puRxCount, unsigned char * aucRx);
int boRawDiagRequest(TBusConnection * ptConnection, unsigned int uTxCount, unsigned char * aucTx, unsigned int * puRxCount, unsigned char * aucRx);
int boReadMemory(TBusConnection * ptConnection, unsigned int uTargetMemory, unsigned int uStartAddress, unsigned int * puRxCount, unsigned char * aucRx);
int boWriteMemory(TBusConnection * ptConnection, unsigned int uTargetMemory, unsigned int uStartAddress, unsigned int uTxCount, unsigned char * aucTx);

// Statusabfragen
int boIsAutoresponseActive(TBusConnection * ptConnection);
int boIsListeningActive(TBusConnection * ptConnection);
int boIsMessageSimulationActive(TBusConnection * ptConnection);

// Messages Verwaltung
TSimulationList * ptCreateNewEmptySimulationList(void); // Since V1.03 as of 20120321
int boDeleteSimulationList(TSimulationList * ptSimulationList); // Since V1.03 as of 20120321
TMessageSimulation * ptCreateMessage(const char * szName, unsigned int uId, unsigned int uRepeatTimeMs, unsigned int uOffset, unsigned int uDlc, unsigned char aucData[MSG_LEN], ...); // Dahinter beliebig viele Message für Multiplexing!
TMessageSimulation * ptCreateSingleMessage(const char * szName, unsigned int uId, unsigned int uRepeatTimeMs, unsigned int uOffset, unsigned int uDlc, unsigned char aucData[MSG_LEN]); // Since V1.03 as of 20120321
TMessageSimulation * ptRemoveMessageMultiplex(TMessageSimulation * ptMessage); // Since V1.03 as of 20120321
TMessageSimulation * ptAddMessageMultiplex(TMessageSimulation * ptMessage, unsigned char aucMask[MSG_LEN], ...); // Since V1.03 as of 20120321
TMessageSimulation * ptChangeMessage(TMessageSimulation * ptMessage, const char * szName, unsigned int uId, unsigned int uRepeatTimeMs, unsigned int uOffset, unsigned int uDlc, unsigned char aucData[MSG_LEN]); // Since V1.03-1b1 as of 20131204
int iDeleteMessage(TMessageSimulation ** pptMessage);
int iAddMessage(TMessageSimulation * ptMessage, TSimulationList * ptList, ...); // Return: Anzahl der Listen, in die die Message eingefügt wurde; beliebig viele Listen angebbar
TMessageMask * ptCreateMessageMask(unsigned int uId, unsigned int uDlc, unsigned char aucData[MSG_LEN], unsigned char aucBitMask[MSG_LEN]);

// Command Library
TCommandLibrary * NEW_CommandLibrary(const char * szLibraryName, PHASHMAP ptInitialHashmap);
int boChangeCommandLibraryName(TCommandLibrary * ptCommandLibrary, const char * szLibraryName);

int iRefreshCommandLibraries(void);
int boSelectCommandLibrary(TBusConnection * ptConnection, const char * szLibraryName);
TCommandData * ptCreateCommandData(unsigned int uDiagTxPrefixLength, const unsigned char * aucTxDiagPrefix, unsigned int uDiagRxPrefixLength, const unsigned char * aucRxPrefixCheckData, const unsigned char * aucRxPrefixCheckMask);
TCommandData * ptAddMask(TCommandData * ptCommand, unsigned int uOffset, unsigned int uMaskLength, const unsigned char * aucMask, const unsigned char * aucDefault, int boInvertResult);
int boAddCommand(TCommandLibrary * ptLibrary, const char * szCommandName, TCommandData * ptCommandData);
TCommandData * ptGetCommandData(TBusConnection * ptConnection, const char * szCommandName);

int iGetCommandLibraries(const char * aszList[], unsigned int uListMaxElements);
int iGetCommandList(TBusConnection * ptConnection, const char * aszCommandList[], unsigned int uListMaxElements);
const TBusInterface * ptGetInterface(TBusConnection * ptConnection);

#endif//__GBUS_BASIC_H__
