#ifndef __DLLLOADER_H
#define __DLLLOADER_H

#include "Windows.h"
#include "ModuleManagement.h"

E_MODULE(DLLLoader);


typedef struct s_Dll
{
	char * szFileName;
	HMODULE hLibrary;
	Module * pModule;
	void * pRawIdAndConfigData;
	unsigned short asPublishedVersion[4];
} TDll;


typedef Module * (*DLL_pGetModuleData)(void);
typedef void * (*DLL_pRawIdAndConfigData)(void);
typedef void (*DLL_ReadDLLLoaderVersion)(int aiVersion[4]);


int boCheckCompatibleDLL(int aiLoaderVersionOfDLL[4]);
int iLoadDynamicDlls(char * szDirectory, char * szFileWithWildcard, TDll * apDllData[], int iMaxCount);
TDll * pLoadDll(char * szFileName);
int boFreeDll(TDll ** ppDll);

int boGetAppVersion(char * szDllName, int aiDLLVersion[4]);


#endif//__DLLLOADER_H
