#ifndef __APPLICATION_TOOLS_H
#define __APPLICATION_TOOLS_H

#include <stdarg.h>   // REQUIRED for va_list

typedef struct {
    Boolean boPmf;
    int iMaxCount;
} StructPmf;

//****************************************************************************************
extern StructPmf g_Pmf;

extern int g_hdlToolPanel;
extern int g_hdlToolMenu;
extern int g_pnlDioTest;

// Global paths from frames.cfg
extern char g_szTemplateFolder[];
extern char g_szTempFileFolder[];
extern char g_szPowerUpTime[];

//****************************************************************************************
extern void vShowResult(char Result);

extern void ResetDataWin(void);
extern void WriteToErrorWin(char *pFormat, ...);
extern void WriteToErrorWinV(const char *pFormat, va_list lArgPointer);
extern void WriteToDataWin(char *pFormat, ...);
extern void WriteToDataWinLog(char *pFormat, ...);
extern void WriteToDataWinPass(char *pFormat, ...);
extern void WriteToDataWinFail(char *pFormat, ...);
extern void WriteToDataWinMaterial(char *pFormat, ...);
extern void WriteToStatusWin(char *pFormat, ...);

extern void ShowTime(char szText[]);

extern int iGetPathsFromFrames(char *pszIniPath);
extern int iConfigureSwFromFrames(char *pszIniPath);

// ===== ENUMS MUST ALSO BE INSIDE THE GUARD =====

typedef enum {
    sGOOD,
    sFAIL,
    sONGOING
} MasterUnitReq;

typedef enum {
    sFirstRun,
    sShiftRun,
    sFinishRun
} MasterUnitStatus;

typedef enum {
    bRequired,
    bNotRequired
} MasterUnitSwitch;

int TransactLogs(char szLog[]);
int prc_ShowMsg(char cMsg[], int iResult, short sBtnEvent); 
int prc_ShowPanel(short sTimerEvent); 
int GSMandatoryEnabled(short bdummy);
int GetGSMandatoryRunParam(short dummy);

#endif // __APPLICATION_TOOLS_H
