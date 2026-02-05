//**********************************************************************
//**** POWER UP STATION
//**** JC.ANCHETA - NOVEMBER 2025
//**********************************************************************
#include <CviTypes.h>
#include <Windows.h>
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <FraMesLog.h>     // Must be before the native FraMes-Header
#include <wipmama.h>
#include <evaprod.h>
#include <CviLib.h>
#include <definitions.h>
#include <ApplicationTools.h>
#include <RunSpsTestprogramm.h>
#include <XcpAccessControl.h> 
#include <SMR_PCB_TMP_relationship.h>
#include <6TL_Cards.h>
#include <CAN_Base.h>
#include <FixtureSlot.h>
#include "MultiDutUi.h"

#include "HpSupply.h"   // for boHpPsSetOutOn / boHpPsSetOutput

// **********CALAMBA***********
#include "specialFunction.h"
#include "Control.PXI6528.h"

//////////////////////////////////////////////////////////////////////////////////////////////
#define SPS_TIME_OUT            600.0       // Timeout in seconds
#define LEN_DATAMATRIX          28          // Length to read by scanner
#define MAX_FIXTURE_SLOT        32

#define POWER_RELAY_BOARD_ADDR  CARD904X8_3


//////////////////////////////////////////////////////////////////////////////////////////////
// global

extern char g_szCustomerType[32];
extern char g_szVersion[32];

//////////////////////////////////////////////////////////////////////////////////////////////
// local

EStatus g_eRunStatus = 0;
Boolean g_boTesterOk = False;

TScanningQuality tScanningQuality = {0, 0, 0};

static char cUnitLocation = 0;
char g_szTempSn[OpcStringLength];
char g_szScanTmpSn[OpcStringLength];

static TFixtureScanRequest g_aFixtureScanQueue[MAX_FIXTURE_SLOT];
static int g_iFixtureScanHead  = 0;
static int g_iFixtureScanCount = 0;
static int g_iNextFixtureConfigIndex = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
// Batch power-up (run once per queue batch)
//
// Requirement:
// - Power-up delay must trigger ONLY ONCE for a newly started batch (when queue starts)
// - It must happen BEFORE the first DUT in the batch begins validation/init
// - It must not repeat for every DUT
//
// We define a local SPS state value here to avoid modifying shared headers.

#ifndef sBatchPowerUp
#define sBatchPowerUp 9001
#endif

static int g_bBatchPowerSequenceStarted = 0;
static int    g_bBatchPowerUpDone     = 0;     // 0 = not done for current batch
static int    g_bHasPendingRequest    = 0;     // 1 = a dequeued request is waiting for power-up
static double g_dBatchPowerUpStartTime= 0.0;
static double g_dBatchPowerUpTime     = 0.0;
static TFixtureScanRequest g_tPendingRequest;  // stored first request while power-up runs



extern HpPsDevice g_HpSupplyPrimary;
extern HpPsDevice g_HpSupplySecondary;

#define PRIMARY_VOLT   13.0f
#define PRIMARY_CURR   10.0f

#define SECONDARY_VOLT 13.0f
#define SECONDARY_CURR 10.0f

#define RELAY_SWITCH_DELAY_MS  10


//////////////////////////////////////////////////////////////////////////////////////////////
// StartStopTest.c
extern Boolean boInitDut(void);
extern Boolean boResetDut(void);
extern char *szHexToString(unsigned char *aucData, unsigned int uSize);

// ApplicationMode3.c
extern char   g_cResult;
extern double dHandlingTime;
double        dTestTime;
extern char   g_szSerialId[];

// Slot for current unit (global, defined elsewhere)
extern short g_sSlot;

//////////////////////////////////////////////////////////////////////////////////////////////
// Queue helpers

static int FixtureScan_GetTailIndex(void)
{
    return (g_iFixtureScanHead + g_iFixtureScanCount) % MAX_FIXTURE_SLOT;
}

static void FixtureScan_AdvanceNextSlot(void)
{
    int iConfiguredCount = FixtureSlot_GetConfiguredCount();

    if (iConfiguredCount <= 0)
    {
        g_iNextFixtureConfigIndex = 0;
        return;
    }

    g_iNextFixtureConfigIndex = (g_iNextFixtureConfigIndex + 1) % iConfiguredCount;
}

int FixtureScan_GetNextSlotNumber(void)
{
    char szFixtureId[FIXTURE_SLOT_MAX_ID_LEN] = {0};
    int iSlotNumber = 0;

    if (FixtureSlot_GetConfiguredIdByIndex(g_iNextFixtureConfigIndex,
                                           szFixtureId,
                                           sizeof(szFixtureId))
        && sscanf(szFixtureId, "SLOT_%d", &iSlotNumber) == 1)
    {
        return iSlotNumber;
    }

    return g_iNextFixtureConfigIndex + 1;
}

int FixtureScan_GetNextSlotId(char *slotId, int slotIdBufferSize)
{
    if (slotId == NULL || slotIdBufferSize <= 1)
    {
        return 0;
    }

    if (FixtureSlot_GetConfiguredIdByIndex(g_iNextFixtureConfigIndex,
                                           slotId,
                                           slotIdBufferSize))
    {
        return 1;
    }

    memset(slotId, 0, (size_t)slotIdBufferSize);
    Fmt(slotId, "SLOT_%02d", g_iNextFixtureConfigIndex + 1);
    return 1;
}

int FixtureScan_Enqueue(const char *fixtureId, const char *dutSerial)
{
    int iTailIndex;

    if (fixtureId == NULL || dutSerial == NULL)
    {
        return 0;
    }

    if (strlen(fixtureId) == 0 || strlen(dutSerial) == 0)
    {
        WriteToErrorWin("[QUEUE] Missing FixtureId or DUT Serial");
        return 0;
    }

    if (g_iFixtureScanCount >= MAX_FIXTURE_SLOT)
    {
        WriteToErrorWin("[QUEUE] Fixture/DUT queue is full.");
        return 0;
    }

    iTailIndex = FixtureScan_GetTailIndex();

    memset(&g_aFixtureScanQueue[iTailIndex], 0, sizeof(TFixtureScanRequest));
    strncpy(g_aFixtureScanQueue[iTailIndex].szFixtureId, fixtureId,
            sizeof(g_aFixtureScanQueue[iTailIndex].szFixtureId) - 1);
    strncpy(g_aFixtureScanQueue[iTailIndex].szDutSerial, dutSerial,
            sizeof(g_aFixtureScanQueue[iTailIndex].szDutSerial) - 1);

    MultiDutUi_SetSlotState(fixtureId,
                            MULTI_DUT_STATE_QUEUED,
                            dutSerial,
                            "Queued for next test run");

    g_iFixtureScanCount++;
    FixtureScan_AdvanceNextSlot();

    WriteToDataWin("[QUEUE] Added %s for %s (queued %d)",
                   g_aFixtureScanQueue[iTailIndex].szDutSerial,
                   g_aFixtureScanQueue[iTailIndex].szFixtureId,
                   g_iFixtureScanCount);

    return 1;
}

int FixtureScan_Dequeue(TFixtureScanRequest *request)
{
    if (g_iFixtureScanCount == 0 || request == NULL)
    {
        return 0;
    }

    memcpy(request, &g_aFixtureScanQueue[g_iFixtureScanHead], sizeof(TFixtureScanRequest));
    g_iFixtureScanHead = (g_iFixtureScanHead + 1) % MAX_FIXTURE_SLOT;
    g_iFixtureScanCount--;
    return 1;
}

int FixtureScan_Count(void)
{
    return g_iFixtureScanCount;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Internal helper: apply a dequeued request into the currently-active context

static void ApplyQueuedRequest(const TFixtureScanRequest *pRequest)
{
    if (pRequest == NULL)
    {
        return;
    }

    FixtureSlot_SetActiveId(pRequest->szFixtureId);
    strcpy(g_szScanTmpSn, pRequest->szDutSerial);
    strcpy(g_UnitInfo.szUnitId, g_szScanTmpSn);
    strcpy(g_UnitInfo.szUnitIdType, "SMR_TMP_SN");
    strcpy(g_szSerialId, g_szScanTmpSn);

    WriteToDataWin("[SYSTEM] Using queued DUT %s on %s (remaining %d)",
                   g_szScanTmpSn,
                   FixtureSlot_GetActiveId(),
                   FixtureScan_Count());

    MultiDutUi_SetSlotState(FixtureSlot_GetActiveId(),
                            MULTI_DUT_STATE_QUEUED,
                            g_szSerialId,
                            "Preparing test plan");

    {
        MultiDutSelectedInfo info = {0};

        strncpy(info.fixtureId,          FixtureSlot_GetActiveId(), sizeof(info.fixtureId) - 1);
        strncpy(info.unitId,             g_UnitInfo.szUnitId,       sizeof(info.unitId) - 1);
        strncpy(info.unitIdType,         g_UnitInfo.szUnitIdType,   sizeof(info.unitIdType) - 1);
        strncpy(info.unitMaterialNumber, g_UnitInfo.szUnitMaterial, sizeof(info.unitMaterialNumber) - 1);
        MultiDutUi_UpdateSlotMetadata(FixtureSlot_GetActiveId(), &info);
    }

    dHandlingTime = Timer();
}

static void BatchPowerUp_ExecuteForQueuedSlots(void)
{
    int i;
    int idx;
    int relay;

    WriteToDataWin("[BATCH] Turning ON power supplies...");

    // Turn ON supplies (set voltage/current + output ON)
    if (!boHpPsSetOutOn(&g_HpSupplyPrimary, PRIMARY_VOLT, PRIMARY_CURR))
        WriteToErrorWin("[BATCH] Primary PSU failed to turn ON");

    if (!boHpPsSetOutOn(&g_HpSupplySecondary, SECONDARY_VOLT, SECONDARY_CURR))
        WriteToErrorWin("[BATCH] Secondary PSU failed to turn ON");

    // Switch ON power relays for:
    // 1) the first dequeued request (pending)
    // 2) all remaining queued items
    WriteToDataWin("[BATCH] Switching ON power relays for queued slots...");

    if (g_bHasPendingRequest)
    {
        relay = FixtureSlot_GetPowerSupplyRelayById(g_tPendingRequest.szFixtureId);
        if (relay > 0)
        {
            boSetOnYavRel(POWER_RELAY_BOARD_ADDR, relay);
            Sleep(RELAY_SWITCH_DELAY_MS);
        }
    }

    for (i = 0; i < g_iFixtureScanCount; i++)
    {
        idx = (g_iFixtureScanHead + i) % MAX_FIXTURE_SLOT;

        relay = FixtureSlot_GetPowerSupplyRelayById(g_aFixtureScanQueue[idx].szFixtureId);
        if (relay > 0)
        {
            boSetOnYavRel(POWER_RELAY_BOARD_ADDR, relay);
            Sleep(RELAY_SWITCH_DELAY_MS);
        }
    }

    WriteToDataWin("[BATCH] PSU ON + queued relays ON done.");
}

static void BatchPowerDown_ExecuteForQueuedSlots(void)
{
    int i;
    int idx;
    int relay;

    WriteToDataWin("[BATCH] Turning OFF power supplies...");

    // Turn OFF supplies (output OFF)
    // NOTE: use the correct OFF API you have in HpSupply.h
    // Common choices:
    //   - boHpPsSetOutOff(&dev)
    //   - boHpPsSetOutput(&dev, 0)
    // Pick one that exists in your project.

    #if defined(boHpPsSetOutOff)
        if (!boHpPsSetOutOff(&g_HpSupplyPrimary))
            WriteToErrorWin("[BATCH] Primary PSU failed to turn OFF");

        if (!boHpPsSetOutOff(&g_HpSupplySecondary))
            WriteToErrorWin("[BATCH] Secondary PSU failed to turn OFF");
    #else
        // fallback example (if your library uses output enable)
        if (!boHpPsSetOutput(&g_HpSupplyPrimary, 0))
            WriteToErrorWin("[BATCH] Primary PSU failed to turn OFF");

        if (!boHpPsSetOutput(&g_HpSupplySecondary, 0))
            WriteToErrorWin("[BATCH] Secondary PSU failed to turn OFF");
    #endif

    // Switch OFF power relays for:
    // 1) the first dequeued request (pending)
    // 2) all remaining queued items
    WriteToDataWin("[BATCH] Switching OFF power relays for queued slots...");

    if (g_bHasPendingRequest)
    {
        relay = FixtureSlot_GetPowerSupplyRelayById(g_tPendingRequest.szFixtureId);
        if (relay > 0)
        {
            // NOTE: replace boSetOffYavRel with the real OFF function you have
            // Common names: boSetOffYavRel / boClrYavRel / boResetYavRel
            boSetOffYavRel(POWER_RELAY_BOARD_ADDR, relay);
            Sleep(RELAY_SWITCH_DELAY_MS);
        }
    }

    for (i = 0; i < g_iFixtureScanCount; i++)
    {
        idx = (g_iFixtureScanHead + i) % MAX_FIXTURE_SLOT;

        relay = FixtureSlot_GetPowerSupplyRelayById(g_aFixtureScanQueue[idx].szFixtureId);
        if (relay > 0)
        {
            // NOTE: replace boSetOffYavRel with the real OFF function you have
            boSetOffYavRel(POWER_RELAY_BOARD_ADDR, relay);
            Sleep(RELAY_SWITCH_DELAY_MS);
        }
    }

    WriteToDataWin("[BATCH] PSU OFF + queued relays OFF done.");
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Status text helper

char *cPtrGetStatusText(int Status)
{
    switch (Status)
    {
        case sBatchPowerUp:
            return "Batch Power Up (once per queue)";

        case sWaitForSpsReady:
            return "Wait for SPS ready";

        case sWaitForStartTestOn:
            return "Wait for Start Test ON";

        case sScanAndVerifyIfValidForStation:
            return "Validate unit for this station";

        case sInitDut:
            return "Initialize DUT";

        case sXcpUnlock:
            return "Enable XCP access";

        case sCheckTempSn:
            return "Finalize pretest / TMP SN check";

        case sDutPass:
            return "DUT PASS";

        case sDutFail:
            return "DUT FAIL";

        case sWaitReleaseDutFail:
            return "Wait for Release FAIL";

        case sWaitForStartTestOff:
            return "Wait for Start Test OFF";

        case sError:
            return "Error occurred (Timeout)";

        default:
            return "Undefined Status";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Main SPS runner

short sRunSps(void)
{
    // SW Version verification
    static char g_szUnitMaterialLast[100] = {0};
    short       sSIdx = 0;
    char        g_szSelectedTestPlan[50] = {0};
    static eva_STRUCTTESTPARAMETER pParameter;
    char  szParameterTextValue[201] = {0};
    char  szVersion[32]  = {0};
    char  szCustomer[32] = {0};

    static int    iRunLastStatus = -1;
    static double dStartTime     = 0.0;

    double dCheckTime;

    if (!g_boTesterOk)
    {
        return 0;
    }

    if (g_eRunStatus != iRunLastStatus)
    {
        iRunLastStatus = g_eRunStatus;
    }
    WriteToStatusWin(cPtrGetStatusText(g_eRunStatus));

    // Initial transition: test system is ready ? wait for Start/Test ON
    if (iRunLastStatus == sWaitForSpsReady)
    {
        g_eRunStatus = sWaitForStartTestOn;
    }

    //************************************************************************************************
    // State machine
    //************************************************************************************************

    switch (g_eRunStatus)
    {
        // --------------------------------------------------------------------------------------------
        // 1) Wait for operator / queue to trigger a new test
        // --------------------------------------------------------------------------------------------
        case sWaitForStartTestOn:
        {
            TFixtureScanRequest tRequest;

            if (FixtureScan_Dequeue(&tRequest))
            {
                // If this is the FIRST DUT of a newly started queue batch, run a single
                // power-up delay before any validation/init.
                if (!g_bBatchPowerUpDone)
                {
                    memcpy(&g_tPendingRequest, &tRequest, sizeof(TFixtureScanRequest));
                    g_bHasPendingRequest     = 1;
                    g_dBatchPowerUpStartTime = Timer();
                    g_dBatchPowerUpTime      = atof(g_szPowerUpTime); // seconds
                    g_bBatchPowerSequenceStarted = 0;

                    WriteToDataWin("%f minute(s) Longer Power Up Started!",
                                   g_dBatchPowerUpTime / 60.0);
                    ProcessDrawEvents();

                    g_eRunStatus = sBatchPowerUp;
                    break;
                }

                ApplyQueuedRequest(&tRequest);
                g_eRunStatus = sScanAndVerifyIfValidForStation;
            }

            break;
        }

        // --------------------------------------------------------------------------------------------
        // 1b) Batch Power-Up (runs once per newly started queue batch)
        // --------------------------------------------------------------------------------------------
        case sBatchPowerUp:
			{
				double dElapsed = Timer() - g_dBatchPowerUpStartTime;

				ProcessDrawEvents();
    
				if (!g_bBatchPowerSequenceStarted)
                {
                    g_bBatchPowerSequenceStarted = 1;
                    BatchPowerUp_ExecuteForQueuedSlots();
                }

                if (dElapsed < g_dBatchPowerUpTime)
                {
                    break;
                }

   
                WriteToDataWin("Power Up lasted for %f minute(s)!", g_dBatchPowerUpTime / 60.0);
                // run power-down ONCE
   
                if (g_bBatchPowerSequenceStarted == 1)  
                {      
                    BatchPowerDown_ExecuteForQueuedSlots();        
                    g_bBatchPowerSequenceStarted = 2; // 2 = power down done 
                }

   
                g_bBatchPowerUpDone = 1;

   
                if (g_bHasPendingRequest)   
                {      
                    ApplyQueuedRequest(&g_tPendingRequest);
                    g_bHasPendingRequest = 0;
                    memset(&g_tPendingRequest, 0, sizeof(TFixtureScanRequest));
                    g_eRunStatus = sScanAndVerifyIfValidForStation;
                }
                else
                {
                    g_eRunStatus = sWaitForStartTestOn;
                }

                break;
            }


        // --------------------------------------------------------------------------------------------
        // 2) Validate unit / MES state / parameters for this station
        // --------------------------------------------------------------------------------------------
        case sScanAndVerifyIfValidForStation:

            // Basic checks
            if (strlen(g_szScanTmpSn) == 0)
            {
                WriteToErrorWin("[ERROR] No TMP SN provided by operator.");
                g_eRunStatus = sDutFail;
                break;
            }

            WriteToDataWin("[SYSTEM] Validating TMP SN: %s", g_szScanTmpSn);

            // STEP 1  Validate Unit for this station
            if (!boWpmCheckUnitValid(&g_UnitInfo, &cUnitLocation))
            {
                wpm_ShowError();
                WriteToErrorWin("[ERROR] UNIT NOT VALID FOR THIS STATION! UnitIdType=%s  UnitId=%s",
                                g_UnitInfo.szUnitIdType,
                                g_UnitInfo.szUnitId);
                g_eRunStatus = sDutFail;
                break;
            }
            WriteToDataWin("End of boWpmCheckUnitValid  %.2f s", (Timer() - dHandlingTime));

            // STEP 2  Switch back MES Unit ID
            WriteToDataWin("Start of boWpmSwitchBackUnitId %.2f s", (Timer() - dHandlingTime));

            if (!boWpmSwitchBackUnitId(&g_UnitInfo, True))
            {
                WriteToErrorWin("[ERROR] MES status invalid during boWpmSwitchBackUnitId()");
                g_eRunStatus = sDutFail;
                break;
            }

            WriteToDataWin("End of boWpmSwitchBackUnitId %.2f s", (Timer() - dHandlingTime));

            // STEP 3  Check Software Version & Customer Type (only if material changed)
            if (strcmp(g_szUnitMaterialLast, g_UnitInfo.szUnitMaterial) != 0)
            {
                MultiDutSelectedInfo info = {0};

                g_sSlot = wpm_GetSlotForUnit(g_UnitInfo.szUnitIdType, g_UnitInfo.szUnitId);

                if (g_sSlot == 0)
                {
                    wpm_ShowError();
                    WriteToErrorWin("[ERROR] Invalid Slot returned for UnitIdType=%s UnitId=%s",
                                    g_UnitInfo.szUnitIdType,
                                    g_UnitInfo.szUnitId);
                    g_eRunStatus = sDutFail;
                    break;
                }
				
				// Load Testplan
			
        	if (!eva_LoadTestPlan(g_sSlot, &sSIdx, g_UnitInfo.szUnitMaterial, g_szTestPlan, 0))
        	{
            	eva_ShowError();
            	WriteToErrorWin("[ERROR] Failed to load Testplan: Group=%s Name=%s",
                            g_UnitInfo.szUnitMaterial,
                            g_szTestPlan);

            	g_eRunStatus = sError;
            	break;
        	}

                strncpy(info.fixtureId,          FixtureSlot_GetActiveId(),   sizeof(info.fixtureId) - 1);
                strncpy(info.unitId,             g_UnitInfo.szUnitId,         sizeof(info.unitId) - 1);
                strncpy(info.unitIdType,         g_UnitInfo.szUnitIdType,     sizeof(info.unitIdType) - 1);
                strncpy(info.unitMaterialNumber, g_UnitInfo.szUnitMaterial,   sizeof(info.unitMaterialNumber) - 1);
                strncpy(info.testPlanName,       g_szTestPlan,        sizeof(info.testPlanName) - 1);
                MultiDutUi_UpdateSlotMetadata(FixtureSlot_GetActiveId(), &info);

                // --- Check Customer Type parameter (Teststep 1,1, Parameter 1)
                if (!eva_GetTestIdParameter(g_sSlot, sSIdx, "1,1", 1, &pParameter))
                {
                    eva_ShowError();
                    WriteToErrorWin("[ERROR] Missing parameter (Teststep 1,1 Parameter 1) "
                                    "Group=%s Name=%s",
                                    g_UnitInfo.szUnitMaterial,
                                    g_szTestPlan);

                    g_eRunStatus = sError;
                    break;
                }

                strncpy(szParameterTextValue, pParameter.szParameterTextValue, (int)pParameter.fValue);
                strncpy(szCustomer,           g_szCustomerType,              (int)pParameter.fValue);

                if (strcmp(szParameterTextValue, szCustomer) != 0)
                {
                    WriteToErrorWin("[ERROR] Wrong customer software loaded! Index=%s Need=%s",
                                    g_UnitInfo.szUnitMaterial,
                                    szParameterTextValue);

                    WriteToErrorWin("You started a WRONG SOFTWARE!");
                    g_eRunStatus = sError;
                    break;
                }

                memset(szParameterTextValue, 0, sizeof(szParameterTextValue));

                // --- Check Software Version parameter (Teststep 1,2, Parameter 1)
                if (!eva_GetTestIdParameter(g_sSlot, sSIdx, "1,2", 1, &pParameter))
                {
                    eva_ShowError();
                    WriteToErrorWin("[ERROR] Missing parameter (Teststep 1,2 Parameter 1) "
                                    "Group=%s Name=%s",
                                    g_UnitInfo.szUnitMaterial,
                                    g_szTestPlan);

                    g_eRunStatus = sError;
                    break;
                }

                strncpy(szParameterTextValue, pParameter.szParameterTextValue, (int)pParameter.fValue);
                strncpy(szVersion,           g_szVersion,                    (int)pParameter.fValue);

                if (strcmp(szParameterTextValue, szVersion) != 0)
                {
                    WriteToErrorWin("[ERROR] Wrong software version! Index=%s NeedVersion=%s",
                                    g_UnitInfo.szUnitMaterial,
                                    szParameterTextValue);

                    WriteToErrorWin("You started a WRONG SOFTWARE VERSION!");
                    g_eRunStatus = sError;
                    break;
                }

                // --- Unload Testplan
                if (!eva_RemoveTestPlan(g_sSlot, sSIdx))
                {
                    eva_ShowError();
                    WriteToErrorWin("[ERROR] Failed to unload Testplan: Group=%s Name=%s",
                                    g_UnitInfo.szUnitMaterial,
                                    g_szTestPlan);

                    g_eRunStatus = sError;
                    break;
                }

                // Update Last Material
                strcpy(g_szUnitMaterialLast, g_UnitInfo.szUnitMaterial);
            }

            g_eRunStatus = sInitDut;
            break;

        // --------------------------------------------------------------------------------------------
        // 3) Initialize DUT
        // --------------------------------------------------------------------------------------------
        case sInitDut:

            if (!boInitDut())
            {
                WriteToErrorWin("Error on boInitDut");
                g_eRunStatus = sDutFail;
                break;
            }

            g_eRunStatus = sXcpUnlock;
            WriteToDataWin("End of sInitDut  %.2f s", (Timer() - dHandlingTime));
            break;

        // --------------------------------------------------------------------------------------------
        // 4) XCP Unlock / enable diagnostic access
        // --------------------------------------------------------------------------------------------
        case sXcpUnlock:

            tScanningQuality.iRead++;

            WriteToErrorWin("--- XCP Unlocking ---");

            if (!boXcpUnlock())
            {
                WriteToErrorWin("XCP unlocking failed!");
                g_eRunStatus = sDutFail;
                break;
            }

            WriteToDataWin("XCP Unlocking Time %.2f s", (Timer() - dTestTime));
            WriteToDataWin("----------------------------------------------------------------------------------------------");

            g_eRunStatus = sCheckTempSn;
            WriteToDataWin("End of sXcpUnlock  %.2f s", (Timer() - dHandlingTime));
            break;

        // --------------------------------------------------------------------------------------------
        // 5) Finalize pretest / TMP SN logic (currently simple jump)
        // --------------------------------------------------------------------------------------------
        case sCheckTempSn:

            g_eRunStatus = sResetDut;
            g_cResult    = 'X';   // initialization of global variable
            WriteToDataWin("End of sCheckTempSn %.2f s", (Timer() - dHandlingTime));
            return 1;

        // --------------------------------------------------------------------------------------------
        // 6) Decide PASS/FAIL based on g_cResult
        // --------------------------------------------------------------------------------------------
        case sResetDut:

            if (g_cResult == 'P')
            {
                g_eRunStatus = sDutPass;
                // g_cResult = 'F'; // Reset Value (if needed)
            }
            else
            {
                g_eRunStatus = sDutFail;
            }
            break;

        // --------------------------------------------------------------------------------------------
        // 7) DUT PASS handling
        // --------------------------------------------------------------------------------------------
        case sDutPass:

            dHandlingTime = Timer();

            
            WriteToDataWin("TMP:%s MAT:%s", g_szScanTmpSn, g_UnitInfo.szUnitMaterial);
            WriteToDataWin("FIN:%s PASS",    g_UnitInfo.szUnitIdFin);

            tScanningQuality.iPass++;
            dStartTime   = Timer();
            g_eRunStatus = sWaitForStartTestOff;

            WriteToDataWin("End of sDutPass %.2f s", (Timer() - dHandlingTime));
            WriteToDataWin("Total time %.2f s",      (Timer() - dTestTime));
            WriteToDataWin("----------------------------------------------------------------------------------------------");
            boResetDut();

            break;

        // --------------------------------------------------------------------------------------------
        // 8) DUT FAIL handling
        // --------------------------------------------------------------------------------------------
        case sDutFail:

            dHandlingTime = Timer();

            
            WriteToDataWin("TMP:%s MAT:%s", g_szScanTmpSn, g_UnitInfo.szUnitMaterial);
            WriteToDataWin("FIN:%s FAIL",   g_UnitInfo.szUnitIdFin);

            tScanningQuality.iFail++;
            dStartTime   = Timer();
            g_eRunStatus = sWaitForStartTestOff;

            WriteToDataWin("End of sDutFail %.2f s", (Timer() - dHandlingTime));
            WriteToDataWin("Total time %.2f s",      (Timer() - dTestTime));
            WriteToDataWin("----------------------------------------------------------------------------------------------");
            boResetDut();

            break;

        // --------------------------------------------------------------------------------------------
        // 9) Wait release fail (not used often, but keep for compatibility)
        // --------------------------------------------------------------------------------------------
        case sWaitReleaseDutFail:

            
            WriteToErrorWin("Unit:%s Mat:%s FAIL", g_UnitInfo.szUnitId, g_UnitInfo.szUnitMaterial);
            tScanningQuality.iFail++;
            dStartTime   = Timer();
            g_eRunStatus = sWaitForStartTestOff;
            break;

        // --------------------------------------------------------------------------------------------
        // 10) Wait for Start Test OFF (end of cycle, go back to ready)
        // --------------------------------------------------------------------------------------------
        case sWaitForStartTestOff:

            // Calamba
            
            //setStatus(READY);
            g_eRunStatus = sWaitForStartTestOn;

            // Reset batch-powerup gate only when the queue is empty.
            // Next time a new batch starts (queue refilled), power-up will run once again.
            if (FixtureScan_Count() == 0 && !g_bHasPendingRequest)
            {
                g_bBatchPowerUpDone = 0;
            }

            dHandlingTime = Timer();
            break;

        // --------------------------------------------------------------------------------------------
        // 11) Fatal error / tester not OK
        // --------------------------------------------------------------------------------------------
        case sError:

            boResetDut();

            WriteToErrorWin("To start again, please Stop program and Start again");

            
            g_boTesterOk = False;
            break;

        // --------------------------------------------------------------------------------------------
        default:
            return 0;
    }

    if (g_eRunStatus == sError)
    {
        return -1;
    }

    return 0;
}
