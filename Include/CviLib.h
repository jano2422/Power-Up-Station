#ifndef __CVILIB_H
#define __CVILIB_H

//************************************************************************************************
#include <CviTypes.h>

//************************************************************************************************
#define delay(x) Delay(x/1000.0)   

#define MAX_TEXT_INFO					2000

//************************************************************************************************
typedef struct {
	char szUnitIdType[100];
	char szUnitId[100];
	char szUnitIdTypeFin[100];
	char szUnitIdFin[100];
	char szUnitMaterial[100];
	char szOrder[100];
	char cUnitClass;
	char cUnitType;  
} StructUnitInfo;

typedef struct {
	short sSlot;
	short sIdx;
	short sRun;
} StructEvaProdData;

//************************************************************************************************
extern char g_szCustomSnData[100];
extern StructUnitInfo g_UnitInfo;
extern StructEvaProdData g_EvaProdData;
extern char g_szTextInfo[MAX_TEXT_INFO];
extern char g_szVersion[];

//************************************************************************************************
extern Boolean boWpmCheckUnitValid(StructUnitInfo *UnitInfo, char *pcUnitLocation);  
extern Boolean boWpmSwitchBackUnitId(StructUnitInfo *UnitInfo, Boolean boSwitchBack);
extern Boolean boWpmGetUnitIdByUnitIdFin(StructUnitInfo *UnitInfo);
extern Boolean boGetWpmUnitIds(char *szUnitIdTypeIn, char *szUnitIdTypeOut, char *szUnitIdIn, char *szUnitIdOut);
extern Boolean boGetTestPlanTestStepLimits(int TestId, int TestSubId, float *LSL, float *USL);
extern Boolean boGetSelectedTestValues(char *TestPlanGroup, char *TestPlanName, char *UnitIdType, char *UnitId,
																				char *TestIds[], float *Values, int Ammount, Boolean MustFind);
extern Boolean boCollectTextInfo(char *pFormat, ...);

extern unsigned long RevertCode (const char *szCode, const char *szCharSet);
extern int iGetFinalSnData(int *Day, int *Month, int *Year, int *DayCounter);
extern int iGetDateData(int Day, int Month, int Year, int *DayOfYear, int *WeekOfYear);
extern Boolean boCheckMutex(void);
extern Boolean boStoreResultData(char szFileName[], char szID[], char szKommentar[]); 
extern Boolean boStoreResultDataEx(char szFileName[], char szID[], char szKommentar[], char szMimeType[]);

//************************************************************************************************
#endif
