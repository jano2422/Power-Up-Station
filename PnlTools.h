/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2025. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PNL_TOOLS                        1
#define  PNL_TOOLS_LB_SHOW_DATA           2
#define  PNL_TOOLS_S_STATUS               3
#define  PNL_TOOLS_SCAN_UNIT_BTN          4       /* callback function: UI_ScanDutSerial */
#define  PNL_TOOLS_LED_SLOT_32            5
#define  PNL_TOOLS_LED_SLOT_31            6
#define  PNL_TOOLS_LED_SLOT_30            7
#define  PNL_TOOLS_LED_SLOT_29            8
#define  PNL_TOOLS_LED_SLOT_28            9
#define  PNL_TOOLS_LED_SLOT_27            10
#define  PNL_TOOLS_LED_SLOT_26            11
#define  PNL_TOOLS_LED_SLOT_25            12
#define  PNL_TOOLS_LED_SLOT_24            13
#define  PNL_TOOLS_LED_SLOT_23            14
#define  PNL_TOOLS_LED_SLOT_22            15
#define  PNL_TOOLS_LED_SLOT_21            16
#define  PNL_TOOLS_LED_SLOT_20            17
#define  PNL_TOOLS_LED_SLOT_19            18
#define  PNL_TOOLS_LED_SLOT_18            19
#define  PNL_TOOLS_LED_SLOT_17            20
#define  PNL_TOOLS_LED_SLOT_16            21
#define  PNL_TOOLS_LED_SLOT_15            22
#define  PNL_TOOLS_LED_SLOT_14            23
#define  PNL_TOOLS_LED_SLOT_13            24
#define  PNL_TOOLS_LED_SLOT_12            25
#define  PNL_TOOLS_LED_SLOT_11            26
#define  PNL_TOOLS_LED_SLOT_10            27
#define  PNL_TOOLS_LED_SLOT_09            28
#define  PNL_TOOLS_LED_SLOT_08            29
#define  PNL_TOOLS_LED_SLOT_07            30
#define  PNL_TOOLS_LED_SLOT_06            31
#define  PNL_TOOLS_LED_SLOT_05            32
#define  PNL_TOOLS_LED_SLOT_04            33
#define  PNL_TOOLS_LED_SLOT_03            34
#define  PNL_TOOLS_LED_SLOT_02            35
#define  PNL_TOOLS_LED_SLOT_01            36
#define  PNL_TOOLS_TBL_DUT_RESULTS        37
#define  PNL_TOOLS_TBL_SELECTED_DUT_INFO  38


     /* Menu Bars, Menus, and Menu Items: */

#define  MB_TOOLS                         1
#define  MB_TOOLS_PMF                     2
#define  MB_TOOLS_PMF_ONOFF               3       /* callback function: MenuBarCallback */
#define  MB_TOOLS_PMF_COUNT               4       /* callback function: MenuBarCallback */


     /* Callback Prototypes: */

void CVICALLBACK MenuBarCallback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK UI_ScanDutSerial(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
