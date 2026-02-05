/*****************************************************************************

	Version		Date			Author	Changes
	------------------------------------------------
	1.3    22.08.2007		dunzm:	see FraMesLog.h

******************************************************************************/

#include <windows.h>
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>

// Note, that "#include <FraMesLog.h>" must be placed before WipMama.h, EvaProd.h, Sequencer.h, SerGen.h, MaMa.h

#include <FraMesLog.h>

#include <WipMama.h>
#ifdef _USE_EVA_
	#include <EvaProd.h>
#endif
#ifdef _USE_SEQUENCER_
	#include <Sequencer.h>
#endif
#ifdef _USE_CARMA_
	#include <CarMa.h>
#endif
#ifdef _USE_SERGEN_
	#include <SerGen.h>
#endif


//***************************************************************************************
static int iFraMesFileHandle = 0;

//***************************************************************************************
Boolean boStartFraMesLog(const char *FileName)
{
	char szFileName[] = {"FraMesLog.txt"};
	
	// No separate Log-File for FraMes-Errors are created
	if( FileName == NULL ) {
		iFraMesFileHandle = 0;
		return( True );
	}

	if( FileName[0] != 0 ) {
		iFraMesFileHandle = OpenFile (FileName, VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
	}
	else {
		iFraMesFileHandle = OpenFile (szFileName, VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
	}

	if( iFraMesFileHandle == -1 ) {
		return( False );
	}
	
	return( True );
}

//***************************************************************************************
Boolean boStopFraMesLog(void)
{
	if( iFraMesFileHandle == 0 ) {
		return( True );
	}

	if( iFraMesFileHandle == -1 ) {
		return( False );
	}

	if( CloseFile (iFraMesFileHandle) != 0 ) {
		return( False );
	}
	iFraMesFileHandle = 0;
	
	return( True );
}

//***************************************************************************************
Boolean boFraMesLog(const char *aLine)
{
	int iCheck = 0, iStrLen = 0;
	int iMonth, iDay, iYear;
	int iHours, iMinutes, iSeconds;
	char szLine[5120];
	memset(szLine, 0, sizeof(szLine));

	if( iFraMesFileHandle == 0 ) {
		return( True ) ;
	}
	
	if( GetSystemDate (&iMonth, &iDay, &iYear) != 0 ) {
		return( False ) ;
	}
	if( GetSystemTime(&iHours, &iMinutes, &iSeconds) != 0 ) {
		return( False ) ;
	}
	if( iFraMesFileHandle == -1 ) {
		return( False ) ;
	}
	// It's up to you to define this Pointer which is pointing to the Variable that is containing the SerialNr of the DUT
	if( g_szPtrFraMesLogSerNr && strlen(g_szPtrFraMesLogSerNr) ) {
		sprintf(szLine, "%02d.%02d.%04d  %02d:%02d:%02d\t%s::\t%s\n",
										iDay, iMonth, iYear,
										iHours, iMinutes, iSeconds,
										g_szPtrFraMesLogSerNr,
										aLine);
	}
	else {
		sprintf(szLine, "%02d.%02d.%04d  %02d:%02d:%02d\t           ::\t%s\n",
										iDay, iMonth, iYear,
										iHours, iMinutes, iSeconds,
										aLine);
	}
	
	iStrLen = strlen(szLine);
	iCheck = WriteFile (iFraMesFileHandle, szLine, iStrLen);
	if( iCheck != iStrLen ) {
		return( False ) ;
	}

	return( True );
}

//***************************************************************************************
void WriteFraMesLog(char *pFormat, ...)
{
	va_list pArgPointer;
	char szText[4096];
	char *szPtrFound = NULL;

	va_start(pArgPointer, pFormat);
	vsprintf(szText, pFormat, pArgPointer);
	
	// replace \n with ¶ to place the ErrorText within a single Line
	while( (szPtrFound = strchr(szText, '\n')) != NULL ) {
		*szPtrFound = 0xB6;  // ¶
	}

	boFraMesLog(szText);
	// It's up to you to define this function to display the ErrorMessage of FraMes-Errors
	DisplayFraMesErrorText(szText);
}

//***************************************************************************************
//***************************************************************************************
void wpm_ShowErrorNew(char *szFile,short ssLine)
{
	char 	*szPtrErrorText,
				*szPtrFunctionName,
				*szPtrErrorHint,
				*szPtrCapsError;
	
	wpm_GetErrorInformation(&szPtrErrorText, &szPtrFunctionName, &szPtrErrorHint, &szPtrCapsError);
	WriteFraMesLog("WIP:  %s // %s // %s // %s // In File: %s In Line: %d", szPtrErrorText, szPtrFunctionName, szPtrErrorHint, szPtrCapsError, szFile, ssLine);
}

//***************************************************************************************
#ifdef _USE_EVA_
void eva_ShowErrorNew(char *szFile,short ssLine)
{
	char 	*szPtrErrorText,
				*szPtrFunctionName,
				*szPtrErrorHint,
				*szPtrCapsError;
	
	eva_GetErrorInformation(&szPtrErrorText, &szPtrFunctionName, &szPtrErrorHint, &szPtrCapsError);
	WriteFraMesLog("EVA:  %s // %s // %s // %s // In File: %s In Line: %d", szPtrErrorText, szPtrFunctionName, szPtrErrorHint, szPtrCapsError, szFile, ssLine);
}
#endif

//***************************************************************************************
#ifdef _USE_SEQUENCER_
void seq_ShowErrorNew(char *szFile,short ssLine)
{
	char 	*szErrorText,
				*szFunctionName,
				*szErrorHint,
				*szCapsError;
	
	seq_GetErrorInformation(&szErrorText, &szFunctionName, &szErrorHint, &szCapsError);
	WriteFraMesLog("SEQ:  %s // %s // %s // %s // In File: %s In Line: %d", szErrorText, szFunctionName, szErrorHint, szCapsError, szFile, ssLine);
}
#endif

//***************************************************************************************
#ifdef _USE_CARMA_
void car_ShowErrorNew(char *szFile,short ssLine)
{
	char 	*szPtrErrorText,
				*szPtrFunctionName,
				*szPtrErrorHint,
				*szPtrCapsError;
	
	car_GetErrorInformation(&szPtrErrorText, &szPtrFunctionName, &szPtrErrorHint, &szPtrCapsError);
	WriteFraMesLog("CAR:  %s // %s // %s // %s // In File: %s In Line: %d", szPtrErrorText, szPtrFunctionName, szPtrErrorHint, szPtrCapsError, szFile, ssLine);
}
#endif

//***************************************************************************************
#ifdef _USE_SERGEN_
void ser_ShowErrorNew(char *szFile,short ssLine)
{
	char 	*szPtrErrorText,
				*szPtrFunctionName,
				*szPtrErrorHint,
				*szPtrCapsError;
	
	ser_GetErrorInformation(&szPtrErrorText, &szPtrFunctionName, &szPtrErrorHint, &szPtrCapsError);
	WriteFraMesLog("SER:  %s // %s // %s // %s // In File: %s In Line: %d", szPtrErrorText, szPtrFunctionName, szPtrErrorHint, szPtrCapsError, szFile, ssLine);
}
#endif

//***************************************************************************************

