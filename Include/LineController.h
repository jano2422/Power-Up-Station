//****************************************************************************
//*                                                                          *
//*  File : LineController.h                                                  	     *
//*                                                                          *
/*****************************************************************************

	Dokumentation:

******************************************************************************/

#ifndef __LINECONTROLLER_H
#define __LINECONTROLLER_H

#include <CVITypes.h>

/*****************************************************************************/

Boolean LineControllerConnect (void);
void LineControllerDisconnect (void);
int LineControllerSend (const char *szFormat, ...);
Boolean LineControllerProcessReply (const char *szCommand, char *szR1, char *szR2, char *szR3);
int CVICALLBACK ClientTcp_CB (unsigned pHandle, int pEvent, int pError, void *pCallBackData);

/*****************************************************************************/
#endif
