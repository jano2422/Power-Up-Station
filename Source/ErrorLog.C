/***************************************************************************************/
/*                                                                                     */
/*  File : ErrorLog.C                                                                   */
/*                                                                                     */
/***************************************************************************************

		Documentation:
		--------------
		Speicherung der Meﬂergebnisse in Logfile
		
****************************************************************************************/
#include <windows.h>
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include <ErrorLog.h>

//***************************************************************************************
int iErrorFileHandle = 0;

//***************************************************************************************
extern unsigned long g_ulTempSn;
extern char g_szFinalSn[100];

//***************************************************************************************
Boolean boStartErrorLog(char *aFileName)
{
	char szFileName[256];
	
	sprintf(szFileName, "%s", aFileName);
	iErrorFileHandle = OpenFile (szFileName, VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);

	if( iErrorFileHandle == -1 ) {
		return (False);
	}
	
	return (True);
}

//***************************************************************************************
Boolean boStopErrorLog(void)
{
	if( (iErrorFileHandle == -1) || (iErrorFileHandle == 0) ) {
		return (False);
	}

	if( CloseFile (iErrorFileHandle) != 0 ) {
		return (False);
	}
	iErrorFileHandle = 0;
	
	return (True);
}

//***************************************************************************************
Boolean boErrorLog(char *aLine)
{
	int iCheck = 0;
	int iMonth, iDay, iYear;
	int iHours, iMinutes, iSeconds;
	char szLine[5120];
	memset(szLine, 0, sizeof(szLine));

	if( GetSystemDate (&iMonth, &iDay, &iYear) != 0 ) {
		return (False);
	}
	if( GetSystemTime(&iHours, &iMinutes, &iSeconds) != 0 ) {
		return (False);
	}

	if( (iErrorFileHandle == -1) || (iErrorFileHandle == 0) ) {
		return (False);
	}
	if( g_ulTempSn ) {
		sprintf(szLine, "%02d.%02d.%04d  %02d:%02d:%02d  %010lu::  %s\n",
										iDay, iMonth, iYear,
										iHours, iMinutes, iSeconds,
										g_ulTempSn,
										aLine);
	}
	else if( g_szFinalSn[0] ) {
		sprintf(szLine, "%02d.%02d.%04d  %02d:%02d:%02d  %10s::  %s\n",
										iDay, iMonth, iYear,
										iHours, iMinutes, iSeconds,
										g_szFinalSn,
										aLine);
	}
	else {
		sprintf(szLine, "%02d.%02d.%04d  %02d:%02d:%02d            ::  %s\n",
										iDay, iMonth, iYear,
										iHours, iMinutes, iSeconds,
										aLine);
	}
	
	iCheck = WriteFile (iErrorFileHandle, szLine, strlen(szLine));
	if( iCheck != strlen(szLine) ) {
		return (False);
	}

	return (True);
}

/***************************************************************************************/
