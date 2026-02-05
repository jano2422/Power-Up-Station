#ifndef __CAN_BASE_H__
#define __CAN_BASE_H__


#include "ModuleManagement.h"
#include "DLLLoader.h"
#include "GBus_Basic_transparent.h"  
#include "GBus_Basic.h"
#include "GBus_CAN.h"

#include "definitions.h"

#ifdef APPLICATION_NAME
E_MODULE(APPLICATION_NAME);
#endif//APPLICATION_NAME

extern int boCanInit(void);
extern int boCanInitAppl(void);
extern int boCanInitAppl2(void);   
extern int boCanReset(void);
extern int CanClose(void);
extern int CanCloseAppl(void);
extern int CanCloseAppl2(void);      


extern char g_szVersion[];

extern TBusConnection * ptConnection;
#ifdef CAN_PRIVATE
extern TBusConnection * ptConnection2;
#endif


#endif//__CAN_BASE_H__
