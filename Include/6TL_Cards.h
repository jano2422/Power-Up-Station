#include "cvitypes.h"

#define CARD904X8_1 0x15510201 //Public CAN
#define CARD904X8_2 0x15510200 //Private CAN
#define CARD904X8_3 0x15510202 //HpSupply
#define CARD904X8_4 0x15510203
#define CARD90132_1 0x15510300
#define CARD90132_2 0x15510301
#define CARD90132_3 0x15510302  
#define CARD90132_4 0x15510303


extern Boolean boInitYAV(void);
extern Boolean boSetOnYavRel(long lAdress,int iRelay); // Relays von 1 - 32
extern Boolean boSetOffYavRel(long lAdress,int iRelay); // Relays von 1 - 32   
extern Boolean boResetYavBoard(long lAdress); // Relays von 1 - 32   
extern Boolean boCloseYAV(void); 
extern Boolean boAskYavRel(long lAdress, int iRelay, long lStatus[]);
extern Boolean boAskAllRelays(long lAdress);
