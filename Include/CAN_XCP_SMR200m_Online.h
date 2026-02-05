#ifndef CAN_XCP_SMR200mOnline_H
#define CAN_XCP_SMR200mOnline_H

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

/*
typedef struct tag_ip_address {
	DWORD Ip_Address;
	WORD  Ip_Port;
} IP_ADDRESS;

typedef struct tag_ExcludeArea
{
	DWORD dwStartAddr;
	DWORD dwEndAddr;
} TExcludeArea, *PTExcludeArea;
*/
#ifdef __cplusplus
extern "C" {
#endif

		DWERROR XCP_SMR200_AbortThread( BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_ConfigureDll( IP_ADDRESS IPAddr[12],
											void (__stdcall *pfnInformation)( DWORD dwError, LPSTR szText ),
											BYTE bUseCallback,
											BYTE bMcuType );


		DWERROR XCP_SMR200_DataToRam( DWORD  dwStartAddr,
											DWORD  dwEndAddr,
											LPBYTE pbBuffer,
											BYTE   bOption,
											BYTE   bFillValue,
											BYTE   bProgrammerNumber );

		DWERROR XCP_SMR200_DownloadBootloader( BYTE bIndex, BYTE bProgrammerNumber);

		DWERROR XCP_SMR200_EepromFill( DWORD dwStartAddress,
											DWORD dwEndAddress,
											BYTE bValue,
											BYTE bProgrammerNumber );
											
		DWERROR XCP_SMR200_EraseFlash( DWORD dwStartAddress,
											DWORD dwLength,
											BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_FillRam( DWORD dwFillValue,
											BYTE bFillValueSize,
											BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_Function(BYTE bLen,
											BYTE * pbCmd,
											BYTE bLenReturn,
											BYTE * pbReturn,
											WORD wTimeout,
											BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_GetCheckSum( LPDWORD pdwCs,
											DWORD dwStartAddr,
											DWORD dwEndAddr,
											BYTE bOption,
											BYTE progNum);

		DWERROR XCP_SMR200_GetDllVersion( WORD *wMajor,
											WORD *wMinor,
											WORD *wMajorBuild,
											WORD *wMinorBuild );

		DWERROR XCP_SMR200_GetFileChecksum(int iOption);

		DWERROR XCP_SMR200_GetRamData( DWORD  dwStartAddr,
											DWORD  dwEndAddr,
											LPBYTE pbBuffer,
											BYTE   bOption,
											BOOL   iUseThread,
											BYTE   bProgrammerNumber);

		DWERROR XCP_SMR200_InitBase(BYTE bResetIdent,
											BYTE bChannel,
											BYTE bCANBaudrateIndex,
											DWORD dwCanRxId,
											DWORD dwCanTxId,
											BYTE progNum);

		DWERROR XCP_SMR200_LoadFile( LPSTR szFileName,
											BYTE  bOption,
											DWORD dwFill);

		DWERROR XCP_SMR200_MergeFile( LPSTR szFileName,
											int   iInsertOffset );

		DWERROR XCP_SMR200_ProgramArea( DWORD dwStartAddr,
											DWORD dwEndAddr,
											BYTE  bOption,
											WORD * pCS,
											BYTE  bProgrammerNumber );

		DWERROR XCP_SMR200_ProgramRam( DWORD dwStartAddr,
											DWORD dwEndAddr,
											WORD * pCS,
											BYTE  bProgrammerNumber );

		DWERROR XCP_SMR200_ReadData( DWORD  dwStartAddress,
											DWORD  dwEndAddress,
											LPBYTE pbBuffer,
											BOOL   iUseThread,
											BYTE   bProgrammerNumber );

		DWERROR XCP_SMR200_ReadDataToFile( DWORD dwStartAddress,
											DWORD dwEndAddress,
											LPSTR szFileName,
											BYTE  bFileType,
											BYTE  bRecordLength,
											BYTE  bProgrammerNumber );

		DWERROR XCP_SMR200_RoutineControl( BYTE bType,
											WORD RoutineID,
											DWORD dwAddress,
											BYTE bProgrammerNumber);

		DWERROR XCP_SMR200_Security( BYTE  bAction,
											BYTE  bType,
											DWORD *pdwKeySeed,
					  BOOL  iUseThread,
											BYTE  bProgrammerNumber);

		DWERROR XCP_SMR200_SelectTarget( BYTE bTargetInterface,
											BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_SetBaudRate( BYTE bCANBaudrate, BYTE progNum);

		DWERROR XCP_SMR200_SetExcludeAreas(PTExcludeArea pExcludeAreas,
											int  iNumber,
											BOOL bRead );

		DWERROR XCP_SMR200_SetThreadsPriority( int iPriority );

		DWERROR XCP_SMR200_SoftwareUpdate( WORD wDevice,
											BYTE bUpdate,
											BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_SystemReset( BYTE bProgrammerNumber );

		DWERROR XCP_SMR200_SwitchConnector( WORD wSwitches,
											BYTE   bProgrammerNumber );

		DWERROR XCP_SMR200_VerifyCheckSum( DWORD  dwStartAddress,
											DWORD  dwEndAddress,
											LPBYTE pbBuffer,
											BYTE   bOption,
											BYTE   bFillValue,
											BYTE   bProgrammerNumber );

		DWERROR XCP_SMR200_VerifyData( DWORD  dwStartAddress,
											DWORD  dwEndAddress,
											LPBYTE pbBuffer,
											BYTE   bOption,
											BYTE   bFillValue,
											BYTE   bProgrammerNumber );

		DWERROR XCP_SMR200_VoltageMonitor( BYTE bTask,
											WORD wMin,
											WORD wMax,
											BYTE bProgrammerNumber);
#ifdef __cplusplus
}
#endif

#endif

