#define SER_VERSION    	   "$Name: V0202b3 $"

#ifdef __cplusplus
    extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Typedef's
//////////////////////////////////////////////////////////////////////////////////////////////

typedef struct ser_structidinfo
{
	char szId[100];
	char cSrc;
	char szStation[100];
	char szComment[100];
	char szGenDate[20];
} ser_STRUCTIDINFO;
		
//
//   Error Functions
//
#ifndef ser_ShowError
#define ser_ShowError(DUMMY) ser_ShowErrorXY(0,0,__FILE__,__LINE__)
#endif

extern short __stdcall ser_Init(void);

extern short __stdcall ser_GetErrorCode(void);
extern char __stdcall *ser_GetErrorText(short sErrorCode);
extern short __stdcall ser_ShowErrorXY(int siTop,int siLeft, char *szFile,short ssLine);
extern short __stdcall ser_GetErrorInformation(char **szErrorText,const char **szFunctionName,char **szErrorHint, char **szCapsError);

extern short __stdcall ser_DisableFreeMemory(void);

extern char *ser_GetCfg(char *szKey, short boShowError);

//
//   Sergen Functions
//

extern char *__stdcall ser_GetVersion(void);
extern short __stdcall ser_GetSrvVersion(char *szversion);
extern short __stdcall ser_GetIdType(const char *szIdType,char *szIdTypeDef, char *szIdTypeInfo,char *szLastId);
extern short __stdcall ser_CreateId(const char *szIdType,char *szId);
extern short __stdcall ser_CreateIds(const char *szIdType,short scount, char (*szId)[100]);
extern short __stdcall ser_SaveId(const char *szIdType,const char *szId,const char *szCheck, char *szSource ,char *szComment);
extern short __stdcall ser_ReadId(const char *szIdType,const char *szId,const char *szCheck, char *szSource, char *szStation, char *szComment, char * szDateTime);
extern short __stdcall ser_SaveIds(const char *szIdType, char (*szId)[100], short sSize, const char *szCheck, char *szSource ,char *szComment);
extern short __stdcall ser_GetIds (char *szIdType, const char *IdMask, short *sCount, struct ser_structidinfo **pIdList);


#ifdef __cplusplus
    }
#endif
