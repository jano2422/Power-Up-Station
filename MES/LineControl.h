//////////////////////////////////////////////////////////////////////////////////////////////
//
// FraMES - Framework MES
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: LineControl functions
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#define LINECTRL_VERSION      "$HeadURL: https://nuas111.auto.contiwan.com/svn/MES_FraMES_Core/FraMES/Core/LineCtrl/tags/V0230b2/LineControl.h $"
#define LINECTRL_RELEASE_DATE "$Date: 2011-12-21 13:10:27 +0100 (Mi, 21 Dez 2011) $"

#ifndef LINECTRL
#define LINECTRL

#ifdef __cplusplus
	extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Typedef's
//////////////////////////////////////////////////////////////////////////////////////////////

typedef enum lcr_IDTYPE      {LCR_SID, LCR_PID, LCR_SAS, LCR_PAS} lcr_IdType;
typedef enum lcr_SETUPSTATUS {LCR_SETUP_NOK, LCR_SETUP_OK}        lcr_SetupStatus;
typedef enum lcr_SERVEREVENT {LCR_SRV_DISCONNECT, LCR_SRV_RECONNECT, LCR_SETUP_CHG, LCR_DYN_PAR_CHG, LCR_LINE_ACTIVE_CHG} lcrServerEvent;

//////////////////////////////////////////////////////////////////////////////////////////////
//   Error Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall lcr_ShowErrorFL(const char *szFile, const short sLine);
extern short __stdcall lcr_ShowErrorXY(const int iTop, const int iLeft, const char *szFile, const short sLine);

extern short __stdcall lcr_GetErrorCode(void);
extern char* __stdcall lcr_GetErrorText(const short sErrorCode);
extern short __stdcall lcr_GetErrorInformation(char **szErrorText, const char **szFunctionName,
											   char **szErrorHint, char **szCapsError);
extern int   __stdcall lcr_GetMaxErrors(void);

extern short __stdcall lcr_SetFraMESCfgPath(const char *szPath);
extern char* __stdcall lcr_GetCfg(const char *szKey, const short boShowError);
extern char* __stdcall lcr_GetCfg2(const char *szSection, const char *szKey, const short boShowError);

extern void __stdcall  lcr_SetLogLevel(const short sLogLevel);
extern void __stdcall  lcr_SetLogDest(const char *szLogDest);
extern void __stdcall  lcr_GetLogLevel(short *sLogLevel);
extern void __stdcall  lcr_AddLogDest(const char cLogDest);
extern void __stdcall  lcr_GetLogDest(char *szLogDest);

extern void __stdcall  lcr_SetExtHintFct(void (__stdcall *aFctPtr)(const char*));
extern void __stdcall  lcr_SetExtShowErrFct(void (__stdcall *aFctPtr)(const char *szDateTime, const short sErrorCode, const char *szErrorText, 
                                            const char *szModule, const char *szModuleVer, const char *szFunction, const char *szErrorHint, const char *szCapsError));
extern void __stdcall  lcr_SetExtShowWarningFct(void (__stdcall *aFctPtr)(const char *szStation, const char *szModule, const char *szModuleVer, 
                                                const char *szText, const long lAutoClose));

extern short __stdcall lcr_DisableFreeMemory(void);        

#ifndef lcr_ShowError
#define lcr_ShowError(DUMMY) lcr_ShowErrorXY(0,0,__FILE__,__LINE__)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Init Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern char* __stdcall lcr_GetVersion(void);
extern char* __stdcall lcr_GetReleaseDate(void);

extern short __stdcall lcr_Init(void);
extern void  __stdcall lcr_Dispose(void);

extern short __stdcall lcr_Connect(void);
extern short __stdcall lcr_Disconnect(void);

extern short __stdcall lcr_LoadSetup(void);
extern short __stdcall lcr_RequiredStation(short *sRequiredStation);
extern short __stdcall lcr_ChangeOperationMode(const char *szOperationMode);

//////////////////////////////////////////////////////////////////////////////////////////////
// Get Functions (get own data)
//////////////////////////////////////////////////////////////////////////////////////////////
extern short __stdcall lcr_GetStationName(char *szStationName);                    // get entry from FraMES.cfg
extern short __stdcall lcr_SetStationName(const char *szStationName);			   // overwrite loaded entry from FraMES.cfg


//////////////////////////////////////////////////////////////////////////////////////////////
// Set Functions (send init data to LineControl)
//////////////////////////////////////////////////////////////////////////////////////////////
	
extern short __stdcall lcr_SetSetup(const enum lcr_SETUPSTATUS eStatus);           // Setup is "OK" or "NOK"
extern short __stdcall lcr_SetWipMaterial(const char *szMaterial);                 // send new material; used in mode 4
extern short __stdcall lcr_SetOrder(const char *szOrder);                          // send selected order


//////////////////////////////////////////////////////////////////////////////////////////////
// Get Functions (get data from LineControlServer)
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall lcr_GetConfiguration(short *sConfiguration);                   // return value is mode 1..12
extern short __stdcall lcr_GetLineActive(short *sStatus);                             // get the current status of LineActive
extern short __stdcall lcr_GetWipMaterial(char *szMaterial, char *szMaterialDescr);   // get current activated material on LiCoSrv
extern short __stdcall lcr_GetAssMaterial(char *szAssMat, char *szAssMatDescr);       // get material to be assembled
extern short __stdcall lcr_GetOrder(char *szOrder, short *sOperation, long *lQty, long *lLeftQty);    // get order, quantity and left quantity
extern short __stdcall lcr_GetOrder2(char *szOrder, char *szProdVersion, short *sOperation, long *lQty, long *lLeftQty);    // get order, prod. version, quantity and left quantity
extern short __stdcall lcr_GetIdType(const enum lcr_IDTYPE eType, char *szIdType);    // enum SID, PID, SAS, PAS
extern short __stdcall lcr_GetPanelSize(short *sPanelSize);                           // return value is qty units on panel
extern short __stdcall lcr_GetCarrierType(char *szCarrierType);

extern short __stdcall lcr_GetParameter(const char *szTestId, const short sParamId, float *fValue, char *szText, char *szDesc);

//////////////////////////////////////////////////////////////////////////////////////////////
// Get Functions (synchronise dynamic parameter with LineControlServer)
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall lcr_SetDynamicParameter(const char *szParamName, const char *szParamValue);
extern short __stdcall lcr_GetDynamicParameter(const char *szParamName, char *szParamValue);
extern short __stdcall lcr_ActivateDynParamBroadCast(const char *szParamName);

extern short __stdcall  lcr_SetBroadcastFct( void (__stdcall *aFctPtr)(const short, const char*));

//////////////////////////////////////////////////////////////////////////////////////////////
// create or validate functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall lcr_ValidateCreateUnits(char *szID1,       char (*szIDS1)[100], 
                                               const char *szID2, const char (*szIDS2)[100], 
                                               short *sMax); 


//////////////////////////////////////////////////////////////////////////////////////////////
// transaction functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall lcr_ProgressUnit(const char *szID,           const char cStatus,
                                        const char (*szIDArr)[100], const char *cStatusArr, const short sCount);
extern short __stdcall lcr_MoveToMaMa(const char *szID, const char (*szIDs)[100], const short sCount);

#ifdef __cplusplus
	}
#endif

#endif
