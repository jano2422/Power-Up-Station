/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2022. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_MasterUnit                 2       /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG                    3       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  4       /* control type: textMsg, callback function: (none) */
#define  PANEL_TxtStationLocked           5       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  6       /* control type: textMsg, callback function: (none) */
#define  PANEL_GOOD                       7       /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG_4                  8       /* control type: textMsg, callback function: (none) */
#define  PANEL_REJECT                     9       /* control type: deco, callback function: (none) */
#define  PANEL_COMMANDBUTTON              10      /* control type: command, callback function: CallBackOK */
#define  PANEL_PICTURE                    11      /* control type: picture, callback function: (none) */
#define  PANEL_TxtSOF2                    12      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_1                    13      /* control type: textMsg, callback function: (none) */
#define  PANEL_DoneReject                 14      /* control type: picture, callback function: (none) */
#define  PANEL_DoneGood                   15      /* control type: picture, callback function: (none) */
#define  PANEL_TxtSOF                     16      /* control type: textMsg, callback function: (none) */
#define  PANEL_TxtReady                   17      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER                      18      /* callback function: cb_timer */
		
		
#define  PANELMSG                         2       /* callback function: PanelMsgFunc */
#define  PANELMSG_TXTSHOWMSG              2       /* control type: textBox, callback function: (none) */
#define  PANELMSG_CMDOKBtn                3       /* control type: command, callback function: iCmdOKBtnPressed */
#define  PANELMSG_TIMER                   4       /* control type: timer, callback function: TimerStartFunc */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CallBackOK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK iCmdOKBtnPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelMsgFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerStartFunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
