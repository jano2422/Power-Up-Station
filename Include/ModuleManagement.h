//==============================================================================
//
// Title:       ModuleManagement.h
// Purpose:     A short description of the interface.
//
// Created on:  05.03.2010 at 10:11:22 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __ModuleManagement_H__
#define __ModuleManagement_H__


#include "cvidef.h"

#ifndef STR
#define STR(s) #s
#define ESTR(s) STR(s)
#define CAT(a, b) a##b
#define ECAT(a, b) CAT(a, b)
#endif//STR


typedef int (*GenericInterfaceFunction)(void * pParam);

typedef struct
{
	char * szInterfaceName;
	GenericInterfaceFunction func_call;
} ModuleInterface;

typedef struct s_ModuleInterfaceList
{
	ModuleInterface * miInterface;
	struct s_ModuleInterfaceList * next;
} ModuleInterfaceList;

typedef struct
{
	char * szName;
	int aiVersion[4];
  void (*init)(void);
	const char * szTimestamp;
	ModuleInterfaceList * lstInterfaces;
} Module;

typedef struct s_ModuleList
{
	Module * modElement;
	struct s_ModuleList * next;
} ModuleList;


typedef int (*BatchInterfaceHandleFunction)(void * pParam, Module * modModule, int iResult); // 0: Terminate Batch Process; not0: Continue Batch Process; pParam are the given parameters of the batch call, iResult is the result of the function.



#define MODULE_PREFIX mod

#define MODULE_NAME(Name) ECAT(MODULE_PREFIX, Name)
#define MODULE_CALL_DECL(Name) MODULE_NAME(Name)_call(void)
#define MODULE_CALL(Name) MODULE_NAME(Name)_call()

#define E_MODULE(Name) \
        extern Module MODULE_NAME(Name); \
		extern Module * MODULE_CALL_DECL(Name);   

#define V_MODULE(Name, VersionMajor, VersionMinor, VersionDebug, VersionBuild, Init) \
        Module MODULE_NAME(Name) = { ESTR(Name), {VersionMajor, VersionMinor, VersionDebug, VersionBuild}, Init, __DATE__ " " __TIME__, NULL }; \
				Module * MODULE_CALL_DECL(Name) { return &MODULE_NAME(Name); }

#define GET_MODULE(Name) (* MODULE_CALL(Name) )

/*
	Only one module is allowed per file. If you want to have a "superior" module (such as a program version, but without an extra file), you have to declare virtual modules (V_MODULE). The only difference is at the moment, that a V_MODULE has no THIS_MODULE reference.
	You may also use V_MODULE to avoid compiler warnings about unreferenced THIS_MODULE.
*/

#define MODULE(Name, VersionMajor, VersionMinor, VersionDebug, VersionBuild, Init) \
        V_MODULE(Name, VersionMajor, VersionMinor, VersionDebug, VersionBuild, Init) \
				static Module * THIS_MODULE = &MODULE_NAME(Name);


extern Module * M_Find(const char * szName); // Returns: Module with that name, if it is in the list
extern int M_GetModuleString(const Module * modModule, char * szVersionStringBuffer, int iBufferSize); // Returns: Length of the String, independent of iBufferSize
extern int M_GetShortVersionString(const Module * modModule, char * szVersionStringBuffer, int iBufferSize); // Returns: Length of the String, independent of iBufferSize
extern int M_Count(void); // Returns: The number of elements in the list
extern int M_Register(Module * modModule); // Returns: 1, if element is new, 0 if element was already in list
extern Module ** M_GetModulePtrArray(void); // Returns: Array of Pointers to all registered Modules; DYNAMICALLY
extern const char * M_GetModuleTimestamp(const Module * modModule); // Returns a pointer to the constant time string (compilation time)

// 1.01
extern int M_SetInterface(Module * modModule, char * szInterfaceName, GenericInterfaceFunction funcInterfaceFunction); // Returns: 1, if element is new, 0 if element was already in list
extern int M_ListInterfaces(Module * modModule, void * pDataBuffer, int iBufferLengthBytes, char *** paszModuleNames); // Returns the number of interfaces; The array is located at start of the buffer, the module names will be also written to buffer \0 separated after the array pointers; Negative return value means error.
extern GenericInterfaceFunction M_HasInterface(Module * modModule, char * szInterfaceName); // Returns: functionptr, if interface exists, NULL, if not
extern int M_CallInterface(Module * modModule, char * szInterfaceName, void * pParam); // Returns return value of Interface function or 0 if interface does not exist (! as the function may also return 0, there is no chance here to be sure if the function exists. As a interface designer, choose your return values deliberately!)
extern int M_BatchCallInterface(char * szInterfaceName, void * pParam, BatchInterfaceHandleFunction funcHandleFunction); // Returns the last returned value of funcHandleFunction, or, if not set, the value of the last called interface function.

// ----	
	
extern void M_INIT(void); // Can be used as module initializer prototype for init-functionpointer.

E_MODULE(ModuleManagement);

#endif  /* ndef __ModuleManagement_H__ */
