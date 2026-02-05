/*****************************************************************************/
/*                                                                           */
/*  Header : ErrorLog.H                                                      */
/*                                                                           */
/*****************************************************************************

	Version 1.0    20.11.2001    dunzm
	------------------------------------------------

******************************************************************************

	Documentation:
	--------------
	Saving Errors in LogFile: ErrorLog.txt

******************************************************************************/
#if !defined ( __ERRORLOG_H)
#define __ERRORLOG_H

#include <cvitypes.h>

extern Boolean boStartErrorLog(char *aFileName);
extern Boolean boStopErrorLog(void);
extern Boolean boErrorLog(char *aLine);


/*****************************************************************************/
#endif
