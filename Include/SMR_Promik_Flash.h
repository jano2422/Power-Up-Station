#ifndef __SMR_PROMIK_FLASH_H__
#define __SMR_PROMIK_FLASH_H__

//#include "MPC577xRmOnline.h"
#include "ModuleManagement.h"

extern int iErrorState(unsigned int uError);
extern int iErrorType(unsigned int uError);
extern int boFunctionActive(unsigned int uError);
extern const char * szGetErrorString(unsigned int uError);
extern unsigned int uGetLastErrorCode(void);
extern int boGetDllVersion(float * pfVersion);
extern int boOpenInterface(char cChannel);
extern int boCloseInterface(void);
extern int boOpenConnection(int iProgrammer, char cChannel);
extern int boCloseConnection(void);
extern int boResetJTAG(void);
extern int boResetMC(void);
extern int boCheckReset(void);
extern int boLoadFile(char * szFileName, unsigned short * pusFileChecksum);
extern int boMergeFile(char * szFileName, unsigned short * pusFileChecksum);
extern unsigned int uMapAddressToBlock(unsigned int uAreaAddress);
extern int boEraseArea(unsigned int uAreaAddress);
extern int boEraseAll(void);
extern int boEraseAllNonBlank(void);
extern int boBlankCheckArea(unsigned int uAreaAddress);
extern int boBlankCheckAll(void);
extern int boProgramFile(void);
extern int boProgramRange(unsigned int uStartAddress, unsigned int uEndAddress);
extern int boVerifyChecksum(unsigned short * pusFileChecksum, unsigned short * pusFlashChecksum, int boMarginVerify);
extern int boVerifyData(int boMarginVerify);
extern int boDumpToMotFile(char * szFileName);
extern int boDumpRangeToMotFile(unsigned int uStartAddress, unsigned int uEndAddress, char * szFileName);
extern int boWritePCBtoFlash (unsigned int uStartAddress, unsigned int uEndAddress, unsigned char * szSNNumber);
//#define SLOW

E_MODULE(SMR_PROMIK_FLASH);

#endif//__SMR_PROMIK_FLASH_H__
