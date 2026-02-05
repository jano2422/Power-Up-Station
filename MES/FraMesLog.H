/*****************************************************************************/
/*                                                                           */
/*  Header : FraMesLog.H                                                     */
/*                                                                           */
/*****************************************************************************

	Version		Date			Author	Changes
	------------------------------------------------
	1.0    07.07.2005		dunzm:	Initial-Realization
	1.1    23.12.2005		dunzm:	Added __FILE__ and __LINE__ on all xxx_ShowErrorNew 
															function parameterlist
	1.2    10.11.2006		dunzm:	boStartFraMesLog(const char *FileName)
															boStartFraMesLog(NULL) ==>	no logging in a file will be done
															boStartFraMesLog("")   ==>	default file name FraMesLog.txt will be used
	1.3    22.08.2007		dunzm:	Seq_GetErrorInformation has new parameter-list implementet it

******************************************************************************

	Description and Hints:
	--------------
	-	Saving WIP-, EVA-Errors in LogFile "FraMesLog.txt" onto Exe-Location
	
	-	You only have to add "FraMesLog.c" on your project and then 
		wpm_ShowError(), eva_ShowError(), seq_ShowError(), ser_ShowError(), car_ShowError()
		are replaced with more sofisticated ones.
	- You have to copy the File FraMesLogLocal.h to your Project-Path and edit it respectively
	-	You have to define the function " void DisplayFraMesErrorText(char aText[]) ".
		If you do not want to display the Error-Message in your Error-Window, then
		you have to insert " void DisplayFraMesErrorText(char aText[]) { } " onto a Source-File
	-	You have to define the variable " char *g_szPtrFraMesLogSerNr "
		If you do not want to share your variable, then insert "char *g_szPtrFraMesLogSerNr = NULL;"
		to your Source-File otherwhise for example "char *g_szPtrFraMesLogSerNr = gszUnitId;"

******************************************************************************/
#if !defined ( __FRA_MES_LOG_H )
#define __FRA_MES_LOG_H

#include <cvitypes.h>
#include <FraMesLogLocal.h>

//***************************************************************************************
#ifdef _SHOW_ERROR_WIN_ 
	#define wpm_ShowError(DUMMY) wpm_ShowErrorNew(__FILE__,__LINE__);\
					wpm_ShowErrorXY(0,0,__FILE__,__LINE__)

	#ifdef _USE_EVA_
		#define eva_ShowError(DUMMY) eva_ShowErrorNew(__FILE__,__LINE__);\
						eva_ShowErrorXY(0,0,__FILE__,__LINE__)
	#endif
	#ifdef _USE_SEQUENCER_
		#define seq_ShowError(DUMMY) seq_ShowErrorNew(__FILE__,__LINE__);\
						seq_ShowErrorXY(0,0,__FILE__,__LINE__)
	#endif
	#ifdef _USE_CARMA_
		#define car_ShowError(DUMMY) car_ShowErrorNew(__FILE__,__LINE__);\
						car_ShowErrorXY(0,0,__FILE__,__LINE__)
	#endif
	#ifdef _USE_SERGEN_
		#define ser_ShowError(DUMMY) ser_ShowErrorNew(__FILE__,__LINE__);\
						ser_ShowErrorXY(0,0,__FILE__,__LINE__)
	#endif
#else
	#define wpm_ShowError(DUMMY) wpm_ShowErrorNew(__FILE__,__LINE__)

	#ifdef _USE_EVA_
		#define eva_ShowError(DUMMY) eva_ShowErrorNew(__FILE__,__LINE__)
	#endif
	#ifdef _USE_SEQUENCER_
		#define seq_ShowError(DUMMY) seq_ShowErrorNew(__FILE__,__LINE__)
	#endif
	#ifdef _USE_CARMA_
		#define car_ShowError(DUMMY) car_ShowErrorNew(__FILE__,__LINE__)
	#endif
	#ifdef _USE_SERGEN_
		#define ser_ShowError(DUMMY) ser_ShowErrorNew(__FILE__,__LINE__)
	#endif
#endif

//***************************************************************************************
//***************************************************************************************
// It's up to you to define this function to display the ErrorMessage of FraMes-Errors
extern void DisplayFraMesErrorText(char aText[]);
// It's up to you to define this Pointer which is pointing to the Variable that is containing the SerialNr of the DUT
extern char *g_szPtrFraMesLogSerNr;


//***************************************************************************************
//***************************************************************************************
extern Boolean boStartFraMesLog(const char *FileName);
extern Boolean boStopFraMesLog(void);
extern Boolean boFraMesLog(const char *aLine);

extern void wpm_ShowErrorNew(char *szFile,short ssLine);
extern void eva_ShowErrorNew(char *szFile,short ssLine);
extern void seq_ShowErrorNew(char *szFile,short ssLine);
extern void car_ShowErrorNew(char *szFile,short ssLine);
extern void ser_ShowErrorNew(char *szFile,short ssLine);

/*****************************************************************************/
#endif
