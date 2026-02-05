#ifndef __GBUS_BASIC_H__
#define __GBUS_BASIC_H__
#define __GBUS_BASIC_TRANSPARENT_H__

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
#define MAX_MULTIPLEX_COUNT 16

/* Automatic Messages */

typedef struct SMessageSignal
{ // Since V1.03 as of 20120323
	const char * szName;
	unsigned int uId;
	unsigned int uBitStart;
	unsigned int uBitLength;
	int boIsBigEndian;
} TMessageSignal;

typedef struct SMessageData
{ // Since V1.03 as of 20120323
	unsigned int uId;
	unsigned int uDlc;
	unsigned char aucData[MSG_LEN];
} TMessageData;

typedef struct SMessageMultiplex
{ // Since V1.03 as of 20120323
	unsigned char aucMask[MSG_LEN];
	unsigned int uNumMessages;
	unsigned char ** aaucData; //[MSG_LEN]
} TMessageMultiplex;

typedef struct SMessageCRC
{ // Since V1.03 as of 20120323
	unsigned int uBitStart;
	unsigned int uBitLength;
	unsigned char aucCalcMask[MSG_LEN];
	unsigned char ucOrder;
	unsigned int uPolynomial;
	unsigned int uInitialValue;
	unsigned int uFinalXOR;
	int boRefIn;
	int boRefOut;
	unsigned int uUserByteCountBefore;
	unsigned char * aucUserBytesBefore;
	unsigned int uUserByteCountAfter;
	unsigned char * aucUserBytesAfter;
} TMessageCRC;

typedef struct SMessageCounter
{ // Since V1.03 as of 20120323
	unsigned int uBitStart;
	unsigned int uBitLength;
	unsigned int uCounterStart;
	unsigned int uCounterEnd;
	unsigned int uCounterIncrement;
} TMessageCounter;


typedef struct SMessageSimulation
{ // BusMessage; Auch für Flexray!
	const char * szName;
	unsigned int uRepeatTimeMs;
	unsigned int uOffset; // Insbesondere für Flexray
	TMessageData * ptData;
	TMessageMultiplex * ptMultiplex;
	TMessageCRC * ptCRC;
	TMessageCounter * ptCounter;
	unsigned int uUseCounter;
} TMessageSimulation;


typedef struct SSimulationListElement
{ // Liste von BusMessages; Die gelisteten Messages selbst können dabei nicht verändert werden!
	// Die Messages werden im AutomaticMessageLibrary einmal definiert und von dort direkt verlinkt.
	// Beim Entfernen werden nur die Elemente und Verknüpfungen gelöscht, nicht aber die Daten selbst!
	TMessageSimulation * ptMessage;
	struct SSimulationListElement * ptNext;
} TSimulationListElement;

typedef struct SSimulationList
{ // Listenanker
	TSimulationListElement * ptElements;
} TSimulationList;

typedef struct STarMessage
{
	TMessageData * ptTriggerMessage;
	TMessageSimulation * ptResponseMessage;
} TTarMessage;

typedef struct SMessageMask
{
	unsigned int uId;
	unsigned int uDlc;
	unsigned char aucData[MSG_LEN];
	unsigned char aucBitMask[MSG_LEN];
} TMessageMask;

/* Information Data Structures */
typedef struct SCommandData
{ // Aufgliederung Tx: Prefix - Data
	unsigned int uDiagTxPrefixLength; //!< Länge des Prefixes (Sendedaten); wird vornangefügt
	const unsigned char * aucTxDiagPrefix; //!< Daten des Prefixes (Sendedaten)
	unsigned int uDiagRxPrefixLength; //!< Länge des Prefixes (Empfangsdaten); wird später entfernt
	const unsigned char * aucRxPrefixCheckData; //!< Soll-Daten des Prefixes (Empfangsdaten), z. B. Prüfung auf OK-Byte. NULL bedeutet keine Prüfung (entspricht 0xFF überall).
	const unsigned char * aucRxPrefixCheckMask; //!< Bitmaske für die Soll-Daten des Prefixes (Empfangsdaten). NULL bedeutet keine Prüfung (entspricht 0x00 überall).
	unsigned int uRxDataOffset; //!< Zusätzlicher Offset der auszugebenden Daten in Bytes. Zählerstart nach Prefix. Alles davor wird weggeschnitten.
	unsigned int uRxDataMaskLength; //!< Länge einer optionalen Datenmaske.
	const unsigned char * aucRxDataMask; //!< Datenmaske der Antwortdaten (nach Offset). Übrige Bits werden auf den Standardwert aus aucRxDataDefault an dieser Stelle gesetzt. NULL bedeutet keine Maske (entspricht 0xFF überall)
	const unsigned char * aucRxDataDefault; //!< Standardwerte für nicht maskierte Antwortwerte. Länge des Arrays wie Maske, NULL bedeutet binäres Ergebnis, also Rückgabewert 1, wenn alle Maskenbits gesetzt sind, sonst 0.
	int boInvertData; //!< Gibt an, ob die Daten invertiert werden sollen. Die Daten werden erst ganz am Ende invertiert. Bei binärem Ergebnis (s. o.) wird kein Komplement gebildet, sondern boolsch invertiert.
	// Hinweis: TX-Flags sind einfach zwei verschiedene Kommandos.
} TCommandData;

typedef struct SCommandLibrary
{
	char * szName;
	PHASHMAP ptHashmap;
} TCommandLibrary;


typedef struct SGBusTypeInformation
{
	const char * szProductName;
	const char * szProjectName;
	const char * szPlatformName;
	const char * szCustomerName;
	
	// Hier muss noch mindestens ein Link zum CommandLibrary rein! Glaub ich...
	
} TGBusTypeInformation;


typedef struct SListeningConfig
{
	unsigned int uBufferSize;
	int boListeningIsRunning;
	unsigned int hListenEvent;
} TListeningConfig;



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
typedef struct SBusConnection
{
	const char * szCardInterfaceName; //!< Name der G-Api Schnittstelle (Karte), die verwendet wird.
	unsigned int hPort; //!< Interner Port, der durch die G-Api der Schnittstelle zugewiesen wurde.
	unsigned char ucChannel; //!< Multichannel-Kanal der Verbindung.
	char szLastError[STRING_LENGTH]; //!< Letzte übermittelte Fehlermeldung.
	char szLastFirmwareError[STRING_LENGTH]; //!< Letzte übermittelte Firmware-Fehlerbeschreibung.
	
	const Module * ptBaseModule; //!< Basismodul, welches die Verbindung verwaltet.
	
	const TBusInterface * ptInterface; //!< Verwendete Schnittstelle für die Verbindung.
	const TGBusTypeInformation * ptTypeInformation; //!< Informationen zur Baugruppe und zum Bus.
	const TCommandLibrary * ptCommandLibrary;
	TSimulationList * ptBusSimulation; //!< Liste der automatisch und regelmäßig generierten Bus-Nachrichten.
	TTarMessage * ptTarMessage; //!< Automatisch genierierte Antwort auf eine definierte Bus-Nachricht.
	
	int boMessageSimulationActive; //!< Gibt an, ob automatische Bus-Nachrichten generiert werden.
	int boAutoresponseActive; //!< Gibt an, ob die automatische Antwortmechanismus aktiv ist.
	
	TListeningConfig * ptListening; //!< Konfiguration des Listeners
	
} TBusConnection;


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
TMessageSimulation * ptCreateSingleMessage(const char * szName, unsigned int uId, unsigned int uRepeatTimeMs, unsigned int uOffset, unsigned int uDlc, unsigned char aucData[MSG_LEN]); // Since V1.03 as of 20120321
TMessageSimulation * ptRemoveMessageMultiplex(TMessageSimulation * ptMessage); // Since V1.03 as of 20120321
TMessageSimulation * ptAddMessageMultiplex(TMessageSimulation * ptMessage, unsigned char aucMask[MSG_LEN], ...); // Since V1.03 as of 20120321
TMessageSimulation * ptCreateMessage(const char * szName, unsigned int uId, unsigned int uRepeatTimeMs, unsigned int uOffset, unsigned int uDlc, unsigned char aucData[MSG_LEN], ...); // Dahinter beliebig viele Message für Multiplexing!
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


#ifndef __GBUS_BASIC_TRANSPARENT_H__
#pragma message You have to include GBus_Basic_transparent.h instead or before any GBus_Basic.h to be working properly.
#endif//__GBUS_BASIC_TRANSPARENT_H__
