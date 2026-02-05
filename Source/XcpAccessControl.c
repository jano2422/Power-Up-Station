/********************************************************************************/
/*                                                                              */
/*  Source File : XcpAccessControl.c                                            */
/*                                                                              */
/********************************************************************************
 *
 *  Version   Date        Author       Changes
 *  ---------------------------------------------------------------------------
 *  V1.00     20.01.2012  fehrmanng    Initial realization (PSW Reflash)
 *  V1.01     12.06.2012  fehrmanng    Change boWriteSNtoEEPROM to new
 *                                     EEPROM address and suffix PCB#
 *  V1.10     09.12.2025  anchetajc    Renamed file to XcpAccessControl.c and
 *                                     refactored XCP unlock procedure
 *
 *  Documentation:
 *  --------------
 *  XCP access control helper module.
 *
 *  - Handles enabling XCP access after PSW reflash / diagnostic start
 *  - Configures required CAN restbus simulations
 *  - Manages power supplies and Goepel YAV relay for public CAN
 *
 ********************************************************************************/

#include <windows.h>
#include <userint.h>
#include <formatio.h>
#include <utility.h>
#include <CviTypes.h>
#include <ansi_c.h>
#include <CviLib.h>

#include <ApplicationTools.h>
#include <6TL_Cards.h>
#include <HPSupply.h>
#include <definitions.h>
#include <FixtureSlot.h>
#include <CAN_Base.h>

/********************************************************************************/
/*  External functions                                                          */
/********************************************************************************/
/* StartStopTest.c */
extern char *szHexToString(unsigned char *aucData, unsigned int uSize);


/******************************************************************************
 *  Function : boXcpUnlock
 *
 *  Purpose  : Enables XCP access on the DUT.
 *             - Configures and starts CAN restbus simulations on CAN_1/CAN_2
 *             - Powers up DUT via primary/secondary PSUs
 *             - Sends diagnostic requests to enable XCP
 *             - Powers down and disconnects CAN again
 *
 *  Return   : 1 on success, 0 on error
 ******************************************************************************/
int boXcpUnlock(void)
{
    int boResult = 0;
    unsigned char aucRxData[4096];
    unsigned int uDataSize = 0;

    static TSimulationList *ptList  = NULL;
    static TSimulationList *ptList2 = NULL;
    TMessageSimulation *ptMessage   = NULL;
    TMessageSimulation *ptMessage2  = NULL;

    /* ---------------------------------------------------------------------- */
    /* 0) Make sure previous sessions are stopped                             */
    /* ---------------------------------------------------------------------- */
    boStopDiag(ptConnection);
    CanClose();

    /* ---------------------------------------------------------------------- */
    /* 1) Power down both supplies                                            */
    /* ---------------------------------------------------------------------- */
    if (!boHpPsSetOutput(&g_HpSupplyPrimary, Off))
    {
        WriteToErrorWin("Power supply 1 seems to be non-functional!");
        return __LINE__;
    }
    Delay(0.300);

    if (!boHpPsSetOutput(&g_HpSupplySecondary, Off))
    {
        WriteToErrorWin("Power supply 2 seems to be non-functional!");
        return __LINE__;
    }
    Delay(0.300);

    /* ---------------------------------------------------------------------- */
    /* 2) Configure restbus simulation for CAN_1 (once per application)       */
    /* ---------------------------------------------------------------------- */
    if (!ptList)
    {
        /* Only define once, restbus simulation remains for entire software   */
        ptList = ptCreateNewEmptySimulationList();
        if (!ptList)
        {
            WriteToErrorWin("Failed to create CAN restbus simulation list for CAN_1.");
            return __LINE__;
        }

        /* MRM_236h */
        ptMessage = ptCreateMessage("MRM_236h", 0x236, 10, 0, 8,
                                    (unsigned char *)"\x10\x00\x00\x00\x40\x00\x00\xD3", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* BS_VEH_SPD */
        ptMessage = ptCreateMessage("BS_VEH_SPD", 0x214, 20, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* GS_418h */
        ptMessage = ptCreateMessage("GS_418h", 0x418, 20, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x20\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* BS_300h */
        ptMessage = ptCreateMessage("BS_300h", 0x300, 20, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x80\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* ETACS_OUT3 */
        ptMessage = ptCreateMessage("ETACS_OUT3", 0x315, 20, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* GW_OUT_01 */
        ptMessage = ptCreateMessage("GW_OUT_01", 0x284, 50, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* ETACS_OUT1 */
        ptMessage = ptCreateMessage("ETACS_OUT1", 0x424, 40, 0, 8,
                                    (unsigned char *)"\x00\x00\x04\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* METER_OUT_03 */
        ptMessage = ptCreateMessage("METER_OUT_03", 0x154, 100, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* NET_CFG */
        ptMessage = ptCreateMessage("NET_CFG", 0x6FD, 300, 0, 8,
                                    (unsigned char *)"\x01\x85\x00\x00\x00\x00\x01\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* METER_OUT_01 */
        ptMessage = ptCreateMessage("METER_OUT_01", 0x151, 1000, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* VEH_CFG_08 */
        ptMessage = ptCreateMessage("VEH_CFG_08", 0x6F4, 50, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);

        /* VEH_CFG_05 */
        ptMessage = ptCreateMessage("VEH_CFG_05", 0x6F1, 50, 0, 8,
                                    (unsigned char *)"\x00\x00\x00\x00\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage, ptList, NULL);
    }

    /* ---------------------------------------------------------------------- */
    /* 3) Configure restbus simulation for CAN_2 (once per application)       */
    /* ---------------------------------------------------------------------- */
    if (!ptList2)
    {
        /* Only define once, restbus simulation remains for entire software   */
        ptList2 = ptCreateNewEmptySimulationList();
        if (!ptList2)
        {
            WriteToErrorWin("Failed to create CAN restbus simulation list for CAN_2.");
            return __LINE__;
        }

        ptMessage2 = ptCreateMessage("SMR_SLAVE", 0x333, 40, 0, 4,
                                     (unsigned char *)"\x00\x00\x00\x00", NULL);
        iAddMessage(ptMessage2, ptList2, NULL);
    }

    /* ---------------------------------------------------------------------- */
    /* 4) Init CAN applications                                               */
    /* ---------------------------------------------------------------------- */
    if (!boCanInitAppl())
    {
        WriteToErrorWin("Init CAN application error: %s", szGetErrorMessage(ptConnection));
        return 0;
    }

    if (!boCanInitAppl2())
    {
        WriteToErrorWin("Init CAN_2 application error: %s", szGetErrorMessage(ptConnection2));
        return 0;
    }

    /* ---------------------------------------------------------------------- */
    /* 5) Start restbus simulations                                           */
    /* ---------------------------------------------------------------------- */
    boDefineCANMessages(ptConnection, ptList);
    boStartCANMessageSimulation(ptConnection);

    boDefineCANMessages(ptConnection2, ptList2);
    boStartCANMessageSimulation(ptConnection2);

    /* ---------------------------------------------------------------------- */
    /* 6) Connect Goepel CAN to vehicle CAN                                   */
    /* ---------------------------------------------------------------------- */
    if (!boSetOnYavRel(CARD904X8_1, FixtureSlot_GetActivePublicCanRelay()))
    {
        WriteToErrorWin("Configure hardware after Reflash PSW: Error connecting Goepel CAN!");
        return 0;
    }
    Sleep(2000);

    /* ---------------------------------------------------------------------- */
    /* 7) Power up supplies                                                   */
    /* ---------------------------------------------------------------------- */
    if (!boHpPsSetOutput(&g_HpSupplyPrimary, On))
    {
        WriteToErrorWin("Power supply 1 seems to be non-functional!");
        return 0;
    }

    if (!boHpPsSetOutput(&g_HpSupplySecondary, On))
    {
        WriteToErrorWin("Power supply 2 seems to be non-functional!");
        return 0;
    }

    Sleep(1000);

    /* ---------------------------------------------------------------------- */
    /* 8) Start diagnostic session                                            */
    /* ---------------------------------------------------------------------- */
    boResult = boStartDiag(ptConnection);
    if (!boResult)
    {
        WriteToErrorWin("Start Diag error: %s", szGetErrorMessage(ptConnection));
        return __LINE__;
    }

    /* Delay due to SW routine that blocks diagnostics */
    Sleep(1000);

    /* ---------------------------------------------------------------------- */
    /* 9) Enable XCP access – first request                                   */
    /* ---------------------------------------------------------------------- */
    memset(aucRxData, 0, sizeof(aucRxData));
    uDataSize = sizeof(aucRxData);

    boResult = boRawDiagRequest(ptConnection, 2, (unsigned char *)"\x10\x92", &uDataSize, aucRxData);
    if (!boResult)
    {
        WriteToErrorWin("CAN not functional or DUT was not XCP-locked last time: %s",
                        szGetErrorMessage(ptConnection));
    }
    else if (uDataSize < 2)
    {
        WriteToErrorWin("Error while enabling XCP access: Invalid response size (%d / 2), %s",
                        uDataSize, szHexToString(aucRxData, uDataSize));
    }
    else if (memcmp("\x50\x92", aucRxData, 2) != 0)
    {
        WriteToErrorWin("Error while enabling XCP access: Negative or invalid response: %s",
                        szHexToString(aucRxData, uDataSize));
    }

    /* ---------------------------------------------------------------------- */
    /* 10) Enable XCP access – second request                                 */
    /* ---------------------------------------------------------------------- */
    memset(aucRxData, 0, sizeof(aucRxData));
    uDataSize = sizeof(aucRxData);

    boResult = boRawDiagRequest(ptConnection, 3, (unsigned char *)"\xBA\x82\x01", &uDataSize, aucRxData);
    if (!boResult)
    {
        WriteToErrorWin("CAN not functional or DUT was not XCP-locked last time: %s",
                        szGetErrorMessage(ptConnection));
    }
    else if (uDataSize < 3)
    {
        WriteToErrorWin("Error while enabling XCP access: Invalid response size (%d / 3), %s",
                        uDataSize, szHexToString(aucRxData, uDataSize));
    }
    else if (memcmp("\xFA\x82\x01", aucRxData, 3) != 0)
    {
        WriteToErrorWin("Error while enabling XCP access: Negative or invalid response: %s",
                        szHexToString(aucRxData, uDataSize));
    }

    /* ---------------------------------------------------------------------- */
    /* 11) Stop diagnostics and close CAN applications                        */
    /* ---------------------------------------------------------------------- */
    boStopDiag(ptConnection);
    CanCloseAppl();
    CanCloseAppl2();

    Sleep(40);

    /* ---------------------------------------------------------------------- */
    /* 12) Power down sensor                                                  */
    /* ---------------------------------------------------------------------- */
    if (!boHpPsSetOutput(&g_HpSupplyPrimary, Off))
    {
        WriteToErrorWin("Configure hardware for Reflash PSW: Error turning off power supply 1!");
        return 0;
    }

    if (!boHpPsSetOutput(&g_HpSupplySecondary, Off))
    {
        WriteToErrorWin("Configure hardware for Reflash PSW: Error turning off power supply 2!");
        return 0;
    }

    Sleep(300);

    /* ---------------------------------------------------------------------- */
    /* 13) Disconnect Goepel CAN                                              */
    /* ---------------------------------------------------------------------- */
    if (!boSetOffYavRel(CARD904X8_1, FixtureSlot_GetActivePublicCanRelay()))
    {
        WriteToErrorWin("Configure hardware after Reflash PSW: Error disconnecting Goepel CAN!");
        return 0;
    }

    Sleep(1000);

    return 1;
}












