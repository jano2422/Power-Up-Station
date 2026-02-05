#ifndef CAN_XCP_SRR300mOnline_H
#define CAN_XCP_SRR300mOnline_H

#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef LPBYTE
typedef BYTE *              LPBYTE;
#endif
#ifndef WORD
typedef unsigned short      WORD;
#endif
#ifndef DWORD
typedef unsigned long       DWORD;
#endif
#ifndef LPDWORD
typedef DWORD *              LPDWORD;
#endif
#ifndef BOOL
typedef int                 BOOL;
#endif
#ifndef LPSTR
typedef char *              LPSTR;
#endif

#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport) __stdcall
#else
#define DLL_API __declspec(dllimport) __stdcall
#endif

#define DWERROR DWORD DLL_API


typedef struct tag_ip_address {
  DWORD Ip_Address;
  WORD  Ip_Port;
} IP_ADDRESS;

typedef struct tag_ExcludeArea
{
  DWORD dwStartAddr;
  DWORD dwEndAddr;
} TExcludeArea, *PTExcludeArea;

typedef struct { LPSTR szUniversalId;
                 DWORD dwInfoBlockStart;
                 DWORD dwDataStart;
                 DWORD dwDataEnd;
                 DWORD dwCRC32;
                 BOOL iCheckSumValid;
} INFO_BLOCK;

#ifdef __cplusplus
extern "C" {
#endif

    DWERROR XCP_SRR300_AbortThread( BYTE bProgrammerNumber );

    DWERROR XCP_SRR300_ConfigureDll( IP_ADDRESS IPAddr[12],
                      void (__stdcall *pfnInformation)( DWORD dwError, LPSTR szText ),
                      BYTE bUseCallback,
                      BYTE bMcuType );


    DWERROR XCP_SRR300_DataToRam( DWORD  dwStartAddr,
                      DWORD  dwEndAddr,
                      LPBYTE pbBuffer,
                      BYTE   bOption,
                      BYTE   bFillValue,
                      BYTE   bProgrammerNumber );

    DWERROR XCP_SRR300_DownloadBootloader( BYTE bIndex, BYTE bProgrammerNumber);

    DWERROR XCP_SRR300_EraseFlash( DWORD dwEraseArea,
                      BYTE bProgrammerNumber );

    DWERROR XCP_SRR300_FillRam( DWORD dwFillValue,
                      BYTE bFillValueSize,
                      BYTE bProgrammerNumber );

    DWERROR XCP_SRR300_GetCheckSum( LPDWORD pdwCs,
                      DWORD dwStartAddr,
                      DWORD dwEndAddr,
                      BYTE bOption,
                      BYTE progNum);

    DWERROR XCP_SRR300_GetDllVersion( WORD *wMajor,
                      WORD *wMinor,
                      WORD *wMajorBuild,
                      WORD *wMinorBuild );

    DWERROR XCP_SRR300_GetFileChecksum(int iOption);

    DWERROR XCP_SRR300_GetRamData( DWORD  dwStartAddr,
                      DWORD  dwEndAddr,
                      LPBYTE pbBuffer,
                      BYTE   bOption,
                      BOOL   iUseThread,
                      BYTE   bProgrammerNumber);

    DWERROR XCP_SRR300_InitBase(BYTE bResetIdent,
                      BYTE bChannel,
                      BYTE bCANBaudrateIndex,
                      DWORD dwCanRxId,
                      DWORD dwCanTxId,
                      BYTE progNum);

    DWERROR XCP_SRR300_LoadFile( LPSTR szFileName,
                      BYTE  bOption,
                      DWORD dwFill);

    DWERROR XCP_SRR300_MergeFile( LPSTR szFileName,
                      int   iInsertOffset );

    DWERROR XCP_SRR300_ProgramArea( DWORD dwStartAddr,
                      DWORD dwEndAddr,
                      BYTE  bOption,
                      WORD * pCS,
                      BYTE  bProgrammerNumber );

    DWERROR XCP_SRR300_ProgramRam( DWORD dwStartAddr,
                      DWORD dwEndAddr,
                      WORD * pCS,
                      BYTE  bProgrammerNumber );

    DWERROR XCP_SRR300_ReadData( DWORD  dwStartAddress,
                      DWORD  dwEndAddress,
                      LPBYTE pbBuffer,
                      BOOL   iUseThread,
                      BYTE   bProgrammerNumber );

    DWERROR XCP_SRR300_ReadDataToFile( DWORD dwStartAddress,
                      DWORD dwEndAddress,
                      LPSTR szFileName,
                      BYTE  bFileType,
                      BYTE  bRecordLength,
                      BYTE  bProgrammerNumber );

    DWERROR XCP_SRR300_RoutineControl( BYTE bType,
                      WORD RoutineID,
                      DWORD dwAddress,
                      BYTE bProgrammerNumber);

    DWERROR XCP_SRR300_Security( BYTE  bAction,
                      BYTE  bType,
                      DWORD *pdwKeySeed,
					  BOOL  iUseThread,
                      BYTE  bProgrammerNumber);

    DWERROR XCP_SRR300_SelectTarget( BYTE bTargetInterface,
                      BYTE bProgrammerNumber );

    DWERROR XCP_SRR300_SetBaudRate( BYTE bCANBaudrate, BYTE progNum);                  

    DWERROR XCP_SRR300_SetExcludeAreas(PTExcludeArea pExcludeAreas,
                      int  iNumber,
                      BOOL bRead );

    DWERROR XCP_SRR300_SetThreadsPriority( int iPriority );

    DWERROR XCP_SRR300_SoftwareUpdate( WORD wDevice,
                      BYTE bUpdate,
                      BYTE bProgrammerNumber );

    DWERROR XCP_SRR300_SystemReset( BYTE bProgrammerNumber );

    DWERROR XCP_SRR300_SwitchConnector( WORD wSwitches,
                      BYTE   bProgrammerNumber );

    DWERROR XCP_SRR300_VerifyCheckSum( DWORD  dwStartAddress,
                      DWORD  dwEndAddress,
                      LPBYTE pbBuffer,
                      BYTE   bOption,
                      BYTE   bFillValue,
                      BYTE   bProgrammerNumber );

    DWERROR XCP_SRR300_VerifyData( DWORD  dwStartAddress,
                      DWORD  dwEndAddress,
                      LPBYTE pbBuffer,
                      BYTE   bOption,
                      BYTE   bFillValue,
                      BYTE   bProgrammerNumber );

    DWERROR XCP_SRR300_VoltageMonitor( BYTE bTask,
                      WORD wMin,
                      WORD wMax,
                      BYTE bProgrammerNumber);
#ifdef __cplusplus
}
#endif

#endif

