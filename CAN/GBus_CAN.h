#ifndef __GBUS_CAN_H__
#define __GBUS_CAN_H__

// Requires
#include "ModuleManagement.h"
#include "GBus_Basic.h"


E_MODULE(GBus_CAN)


#define CONNECTION_CAN 'C';
//#define CONNECTION_FLEXRAY 'F';

#define EXTENDED_ID(ulIdentifier) ((ulIdentifier) | 0x80000000)

/* INTERFACES */

/* Interface-Strukturen */
typedef TBusInterface TCanBusInterface; // Keine zusätzlichen Elemente
typedef TBusConnection TCanBusConnection; // Keine zusätzlichen Elemente


/* FUNKTIONEN */

// Interface
int iRefreshCANInterfaces(void);
int boAutosetCANInterface(TBusConnection * ptConnection, unsigned int uTries);
int boSetCANInterface(TBusConnection * ptConnection, const char * szInterfaceName);
int iGetCANInterfaces(const char * aszList[], unsigned int uListMaxElements);

// Hardware-Spezifisch für CAN
int boOpenCANCard(TCanBusConnection * ptConnection, const char * szCardInterfaceName);
int boCloseCANCard(TCanBusConnection * ptConnection);
int boResetCANCard(TCanBusConnection * ptConnection);
// Flexray hat hier zum Beispiel Fibex-Funktionen


int boRawCANMessage(TBusConnection * ptConnection, unsigned int uId, unsigned int uDlc, unsigned char * aucTx);


int boStartCANMessageSimulation(TBusConnection * ptConnection);
int boStopCANMessageSimulation(TBusConnection * ptConnection);
int boStartCANAutoresponse(TBusConnection * ptConnection);
int boStopCANAutoresponse(TBusConnection * ptConnection);


// Dispatched für CAN
// Messages Busbezogen
int boDefineCANMessages(TBusConnection * ptConnection, TSimulationList * ptMessages);
int boRemoveCANMessages(TBusConnection * ptConnection, TSimulationList * ptMessages); // Bei ptMessages = NULL wird alles gelöscht, sonst nur die Einträge der Liste
int boSetCANAutoresponse(TBusConnection * ptConnection, unsigned int uRxId, unsigned int uRxDlc, unsigned char aucRxData[MSG_LEN], TMessageSimulation * ptMessage); // Bei NULL oder uId = 0 wird der Eintrag entfernt

// Log and Listen
int boSetCANListeningIDs(TBusConnection * ptConnection, char * szFilter);
int boStartCANListening(TBusConnection * ptConnection, unsigned int uBufferSize, int boLogRx, int boLogTx, int boLogError);
int boStopCANListening(TBusConnection * ptConnection);
int boWaitForCANMessage(TBusConnection * ptConnection, TMessageMask * ptMessageMask, unsigned int uTimeout, TMessageSimulation * ptResponseMessage);
int boGetCANMessages(TBusConnection * ptConnection, TSimulationList * ptReceivedMessages);


#endif//__GBUS_CAN_H__
