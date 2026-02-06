#include <ansi_c.h>
#include <time.h>
#include <userint.h>
#include <utility.h>
#include <cvitypes.h>

#include "ApplicationTools.h"
#include "MultiDutUi.h"
#include "PnlTools.h"
#include "FixtureSlot.h"

#define MULTI_DUT_SLOT_COUNT 32
#define MULTI_DUT_RESULT_COLUMNS 9
#define MULTI_DUT_INFO_ROWS 1
#define MULTI_DUT_INFO_COLUMNS 7

typedef struct
{
    MultiDutResultView view;
    char note[160];
    char timestamp[64];
    char runResult[32];
    char serial[64];
    MultiDutState state;
} SlotResultEntry;

typedef struct
{
    MultiDutState state;
    char slotLabel[16];
    char serial[64];
    char note[160];
    char timestamp[64];
    char lastTestId[128];
    char lastDescription[256];
    char lastValue[96];
    char lastLimits[96];
    char lastRunResult[32];
    char unitId[100];
    char unitIdType[100];
    char testPlanName[64];
    char testPlanGroup[64];
    char unitMaterialNumber[100];
    SlotResultEntry *results;
    int resultCount;
    int resultCapacity;
} SlotUiState;

static int g_iPanelHandle = 0;
static int g_aSlotLedCtrls[MULTI_DUT_SLOT_COUNT] = {0};
static int g_iResultTable = 0;
static int g_iSelectedInfoTable = 0;
static int g_iSelectedSlot = 0;
static int g_iResultTableRows = 0;
static SlotUiState g_aSlotData[MULTI_DUT_SLOT_COUNT];

static const int g_aSlotCtrlIds[MULTI_DUT_SLOT_COUNT] = {
    PNL_TOOLS_LED_SLOT_01,
    PNL_TOOLS_LED_SLOT_02,
    PNL_TOOLS_LED_SLOT_03,
    PNL_TOOLS_LED_SLOT_04,
    PNL_TOOLS_LED_SLOT_05,
    PNL_TOOLS_LED_SLOT_06,
    PNL_TOOLS_LED_SLOT_07,
    PNL_TOOLS_LED_SLOT_08,
    PNL_TOOLS_LED_SLOT_09,
    PNL_TOOLS_LED_SLOT_10,
    PNL_TOOLS_LED_SLOT_11,
    PNL_TOOLS_LED_SLOT_12,
    PNL_TOOLS_LED_SLOT_13,
    PNL_TOOLS_LED_SLOT_14,
    PNL_TOOLS_LED_SLOT_15,
    PNL_TOOLS_LED_SLOT_16,
    PNL_TOOLS_LED_SLOT_17,
    PNL_TOOLS_LED_SLOT_18,
    PNL_TOOLS_LED_SLOT_19,
    PNL_TOOLS_LED_SLOT_20,
    PNL_TOOLS_LED_SLOT_21,
    PNL_TOOLS_LED_SLOT_22,
    PNL_TOOLS_LED_SLOT_23,
    PNL_TOOLS_LED_SLOT_24,
    PNL_TOOLS_LED_SLOT_25,
    PNL_TOOLS_LED_SLOT_26,
    PNL_TOOLS_LED_SLOT_27,
    PNL_TOOLS_LED_SLOT_28,
    PNL_TOOLS_LED_SLOT_29,
    PNL_TOOLS_LED_SLOT_30,
    PNL_TOOLS_LED_SLOT_31,
    PNL_TOOLS_LED_SLOT_32
};

static void MultiDutUi_UpdateLedColor(int slotIndex);
static void MultiDutUi_UpdateResultTable(void);
static void MultiDutUi_UpdateSelectedInfoTable(void);
static int CVICALLBACK SlotLedCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static int MultiDutUi_ToggleSlotEnabled(int slotIndex);
static int MultiDutUi_SlotIdToIndex(const char *slotId);
static const char *MultiDutUi_StateText(MultiDutState state);
static void MultiDutUi_UpdateTimestamp(SlotUiState *slotData);
static void MultiDutUi_BuildTableHeaders(void);
static void MultiDutUi_BuildSelectedInfoHeaders(void);
static void MultiDutUi_ClearResultRow(int rowIndex);
static void MultiDutUi_ClearInfoRow(int rowIndex);
static void MultiDutUi_ResizeResultTable(int rowCount);
static int MultiDutUi_EnsureResultCapacity(SlotUiState *slotData, int neededRows);
static void MultiDutUi_ResetSlotMeasurements(SlotUiState *slotData);
static void MultiDutUi_SetResultRowPlaceholder(const SlotUiState *pSlot);
static void MultiDutUi_SetResultRowFromEntry(int rowIndex, const SlotUiState *pSlot, const SlotResultEntry *pEntry);
static void MultiDutUi_EnsureInfoTableRow(void);
static void MultiDutUi_SetSelectedInfoRow(const SlotUiState *pSlot);



int MultiDutUi_Init(void)
{
    int iSlot;
    char szLabel[32];

    if (g_hdlToolPanel <= 0)
    {
        return -1;
    }

    g_iPanelHandle = g_hdlToolPanel;
    g_iResultTable = PNL_TOOLS_TBL_DUT_RESULTS;
    g_iSelectedInfoTable = PNL_TOOLS_TBL_SELECTED_DUT_INFO;
    g_iSelectedSlot = 0;

    for (iSlot = 0; iSlot < MULTI_DUT_SLOT_COUNT; iSlot++)
    {
        sprintf(szLabel, "%02d", iSlot + 1);
        g_aSlotLedCtrls[iSlot] = g_aSlotCtrlIds[iSlot];

        if (g_aSlotLedCtrls[iSlot] > 0)
        {
	
        	SetCtrlAttribute(g_iPanelHandle, g_aSlotLedCtrls[iSlot],ATTR_CTRL_MODE,VAL_HOT);
            InstallCtrlCallback(g_iPanelHandle, g_aSlotLedCtrls[iSlot], SlotLedCallback, (void *)(intptr_t)iSlot);
            SetCtrlAttribute(g_iPanelHandle, g_aSlotLedCtrls[iSlot], ATTR_LABEL_TEXT, szLabel);
        }

        memset(&g_aSlotData[iSlot], 0, sizeof(SlotUiState));
        strncpy(g_aSlotData[iSlot].slotLabel, szLabel, sizeof(g_aSlotData[iSlot].slotLabel) - 1);
        g_aSlotData[iSlot].state = MULTI_DUT_STATE_IDLE;
        strcpy(g_aSlotData[iSlot].note, "Awaiting assignment");
        strcpy(g_aSlotData[iSlot].lastTestId, "n/a");
        strcpy(g_aSlotData[iSlot].lastDescription, "No measurements recorded");
        strcpy(g_aSlotData[iSlot].lastValue, "n/a");
        strcpy(g_aSlotData[iSlot].lastLimits, "n/a");
        strcpy(g_aSlotData[iSlot].lastRunResult, "Pending");
        strcpy(g_aSlotData[iSlot].unitId, "n/a");
        strcpy(g_aSlotData[iSlot].unitIdType, "n/a");
        strcpy(g_aSlotData[iSlot].testPlanName, "n/a");
        strcpy(g_aSlotData[iSlot].testPlanGroup, "n/a");
        strcpy(g_aSlotData[iSlot].unitMaterialNumber, "n/a");
        g_aSlotData[iSlot].results = NULL;
        g_aSlotData[iSlot].resultCount = 0;
        g_aSlotData[iSlot].resultCapacity = 0;
        MultiDutUi_UpdateTimestamp(&g_aSlotData[iSlot]);
        MultiDutUi_UpdateLedColor(iSlot);
    }

    MultiDutUi_BuildTableHeaders();
    MultiDutUi_BuildSelectedInfoHeaders();
    MultiDutUi_UpdateResultTable();
    MultiDutUi_UpdateSelectedInfoTable();

    return 0;
}

void MultiDutUi_Terminate(void)
{
    if (g_iPanelHandle)
    {
        int iSlot;

        for (iSlot = 0; iSlot < MULTI_DUT_SLOT_COUNT; ++iSlot)
        {
            if (g_aSlotData[iSlot].results != NULL)
            {
                free(g_aSlotData[iSlot].results);
                g_aSlotData[iSlot].results = NULL;
                g_aSlotData[iSlot].resultCapacity = 0;
                g_aSlotData[iSlot].resultCount = 0;
            }
        }

        g_iPanelHandle = 0;
    }
}

void MultiDutUi_SetSlotState(const char *slotId, MultiDutState state, const char *serialNumber, const char *note)
{
    int slotIndex;

    if (slotId == NULL)
    {
        return;
    }

    slotIndex = MultiDutUi_SlotIdToIndex(slotId);
    if (slotIndex < 0 || slotIndex >= MULTI_DUT_SLOT_COUNT)
    {
        return;
    }

    g_aSlotData[slotIndex].state = state;

    if (serialNumber != NULL)
    {
        memset(g_aSlotData[slotIndex].serial, 0, sizeof(g_aSlotData[slotIndex].serial));
        strncpy(g_aSlotData[slotIndex].serial, serialNumber, sizeof(g_aSlotData[slotIndex].serial) - 1);
        memset(g_aSlotData[slotIndex].unitId, 0, sizeof(g_aSlotData[slotIndex].unitId));
        strncpy(g_aSlotData[slotIndex].unitId, serialNumber, sizeof(g_aSlotData[slotIndex].unitId) - 1);
    }

    if (note != NULL)
    {
        memset(g_aSlotData[slotIndex].note, 0, sizeof(g_aSlotData[slotIndex].note));
        strncpy(g_aSlotData[slotIndex].note, note, sizeof(g_aSlotData[slotIndex].note) - 1);
    }

    MultiDutUi_UpdateTimestamp(&g_aSlotData[slotIndex]);
    MultiDutUi_UpdateLedColor(slotIndex);

    if (g_iSelectedSlot == slotIndex)
    {
        MultiDutUi_UpdateResultTable();
        MultiDutUi_UpdateSelectedInfoTable();
    }
}

void MultiDutUi_RecordMeasurement(const char *slotId, const char *serialNumber, const MultiDutResultView *result, const char *note)
{
    int slotIndex = MultiDutUi_SlotIdToIndex(slotId);
    SlotUiState *pSlot;
    SlotResultEntry *pEntry;

    if (slotIndex < 0 || slotIndex >= MULTI_DUT_SLOT_COUNT)
    {
        return;
    }

    pSlot = &g_aSlotData[slotIndex];

    if (serialNumber != NULL)
    {
        memset(pSlot->serial, 0, sizeof(pSlot->serial));
        strncpy(pSlot->serial, serialNumber, sizeof(pSlot->serial) - 1);
    }

    if (result != NULL)
    {
        memset(pSlot->lastTestId, 0, sizeof(pSlot->lastTestId));
        strncpy(pSlot->lastTestId, result->testId, sizeof(pSlot->lastTestId) - 1);

        memset(pSlot->lastDescription, 0, sizeof(pSlot->lastDescription));
        strncpy(pSlot->lastDescription, result->description, sizeof(pSlot->lastDescription) - 1);

        memset(pSlot->lastValue, 0, sizeof(pSlot->lastValue));
        strncpy(pSlot->lastValue, result->value, sizeof(pSlot->lastValue) - 1);

        memset(pSlot->lastLimits, 0, sizeof(pSlot->lastLimits));
        strncpy(pSlot->lastLimits, result->limits, sizeof(pSlot->lastLimits) - 1);
    }

    if (note != NULL)
    {
        memset(pSlot->note, 0, sizeof(pSlot->note));
        strncpy(pSlot->note, note, sizeof(pSlot->note) - 1);
    }

    MultiDutUi_UpdateTimestamp(pSlot);

    if (MultiDutUi_EnsureResultCapacity(pSlot, pSlot->resultCount + 1) == 0)
    {
        pEntry = &pSlot->results[pSlot->resultCount++];
        memset(pEntry, 0, sizeof(SlotResultEntry));

        pEntry->state = pSlot->state;
        strncpy(pEntry->serial, pSlot->serial, sizeof(pEntry->serial) - 1);

        if (result != NULL)
        {
            memcpy(&pEntry->view, result, sizeof(MultiDutResultView));
        }

        strncpy(pEntry->note, pSlot->note, sizeof(pEntry->note) - 1);
        strncpy(pEntry->timestamp, pSlot->timestamp, sizeof(pEntry->timestamp) - 1);
        strncpy(pEntry->runResult, pSlot->lastRunResult, sizeof(pEntry->runResult) - 1);
    }

    if (g_iSelectedSlot == slotIndex)
    {
        MultiDutUi_UpdateResultTable();
        MultiDutUi_UpdateSelectedInfoTable();
    }
}

void MultiDutUi_FinalizeRun(const char *slotId, char result, const char *note)
{
    int slotIndex = MultiDutUi_SlotIdToIndex(slotId);

    if (slotIndex < 0 || slotIndex >= MULTI_DUT_SLOT_COUNT)
    {
        return;
    }

    switch (result)
    {
        case 'P':
            strcpy(g_aSlotData[slotIndex].lastRunResult, "PASS");
            break;
        case 'F':
            strcpy(g_aSlotData[slotIndex].lastRunResult, "FAIL");
            break;
        default:
            strcpy(g_aSlotData[slotIndex].lastRunResult, "Pending");
            break;
    }

    if (note != NULL)
    {
        memset(g_aSlotData[slotIndex].note, 0, sizeof(g_aSlotData[slotIndex].note));
        strncpy(g_aSlotData[slotIndex].note, note, sizeof(g_aSlotData[slotIndex].note) - 1);
    }

    MultiDutUi_UpdateTimestamp(&g_aSlotData[slotIndex]);

    if (g_aSlotData[slotIndex].resultCount > 0)
    {
        SlotResultEntry *pEntry = &g_aSlotData[slotIndex].results[g_aSlotData[slotIndex].resultCount - 1];
        strncpy(pEntry->runResult, g_aSlotData[slotIndex].lastRunResult, sizeof(pEntry->runResult) - 1);
        if (note != NULL)
        {
            memset(pEntry->note, 0, sizeof(pEntry->note));
            strncpy(pEntry->note, note, sizeof(pEntry->note) - 1);
        }
        strncpy(pEntry->timestamp, g_aSlotData[slotIndex].timestamp, sizeof(pEntry->timestamp) - 1);
    }

    if (g_iSelectedSlot == slotIndex)
    {
        MultiDutUi_UpdateResultTable();
        MultiDutUi_UpdateSelectedInfoTable();
    }
}

void MultiDutUi_UpdateSlotMetadata(const char *slotId, const MultiDutSelectedInfo *info)
{
    int slotIndex;

    if (info == NULL)
    {
        return;
    }

    slotIndex = MultiDutUi_SlotIdToIndex(slotId);
    if (slotIndex < 0 || slotIndex >= MULTI_DUT_SLOT_COUNT)
    {
        return;
    }

    if (strlen(info->fixtureId) > 0)
    {
        memset(g_aSlotData[slotIndex].slotLabel, 0, sizeof(g_aSlotData[slotIndex].slotLabel));
        strncpy(g_aSlotData[slotIndex].slotLabel, info->fixtureId, sizeof(g_aSlotData[slotIndex].slotLabel) - 1);
    }

    if (strlen(info->unitId) > 0)
    {
        if (strcmp(g_aSlotData[slotIndex].unitId, info->unitId) != 0)
        {
            MultiDutUi_ResetSlotMeasurements(&g_aSlotData[slotIndex]);
        }

        memset(g_aSlotData[slotIndex].unitId, 0, sizeof(g_aSlotData[slotIndex].unitId));
        strncpy(g_aSlotData[slotIndex].unitId, info->unitId, sizeof(g_aSlotData[slotIndex].unitId) - 1);
        memset(g_aSlotData[slotIndex].serial, 0, sizeof(g_aSlotData[slotIndex].serial));
        strncpy(g_aSlotData[slotIndex].serial, info->unitId, sizeof(g_aSlotData[slotIndex].serial) - 1);
    }

    if (strlen(info->unitIdType) > 0)
    {
        memset(g_aSlotData[slotIndex].unitIdType, 0, sizeof(g_aSlotData[slotIndex].unitIdType));
        strncpy(g_aSlotData[slotIndex].unitIdType, info->unitIdType, sizeof(g_aSlotData[slotIndex].unitIdType) - 1);
    }

    if (strlen(info->testPlanName) > 0)
    {
        memset(g_aSlotData[slotIndex].testPlanName, 0, sizeof(g_aSlotData[slotIndex].testPlanName));
        strncpy(g_aSlotData[slotIndex].testPlanName, info->testPlanName, sizeof(g_aSlotData[slotIndex].testPlanName) - 1);
    }

    if (strlen(info->unitMaterialNumber) > 0)
    {
        memset(g_aSlotData[slotIndex].unitMaterialNumber, 0, sizeof(g_aSlotData[slotIndex].unitMaterialNumber));
        strncpy(g_aSlotData[slotIndex].unitMaterialNumber, info->unitMaterialNumber, sizeof(g_aSlotData[slotIndex].unitMaterialNumber) - 1);
    }

    if (strlen(info->result) > 0)
    {
        memset(g_aSlotData[slotIndex].lastRunResult, 0, sizeof(g_aSlotData[slotIndex].lastRunResult));
        strncpy(g_aSlotData[slotIndex].lastRunResult, info->result, sizeof(g_aSlotData[slotIndex].lastRunResult) - 1);
    }

    MultiDutUi_UpdateTimestamp(&g_aSlotData[slotIndex]);

    if (g_iSelectedSlot == slotIndex)
    {
        MultiDutUi_UpdateSelectedInfoTable();
        MultiDutUi_UpdateResultTable();
    }
}

static int MultiDutUi_SlotIdToIndex(const char *slotId)
{
    int slotNumber = 0;

    if (slotId == NULL)
    {
        return -1;
    }

    if (sscanf(slotId, "SLOT_%d", &slotNumber) != 1)
    {
        return -1;
    }

    return slotNumber - 1;
}

static void MultiDutUi_UpdateLedColor(int slotIndex)
{
    int iCtrl;
    int color = VAL_GRAY;

    if (g_iPanelHandle <= 0)
    {
        return;
    }

    if (slotIndex < 0 || slotIndex >= MULTI_DUT_SLOT_COUNT)
    {
        return;
    }

    iCtrl = g_aSlotLedCtrls[slotIndex];
    if (!iCtrl)
    {
        return;
    }

    switch (g_aSlotData[slotIndex].state)
    {
        case MULTI_DUT_STATE_QUEUED:
            color = VAL_BLUE;
            break;
        case MULTI_DUT_STATE_BATCH_POWER:
            color = VAL_MAGENTA;
            break;
        case MULTI_DUT_STATE_TESTING:
            color = VAL_YELLOW;
            break;
        case MULTI_DUT_STATE_PASS:
            color = VAL_GREEN;
            break;
        case MULTI_DUT_STATE_FAIL:
            color = VAL_RED;
            break;
        case MULTI_DUT_STATE_IDLE:
        default:
            color = VAL_DK_GRAY;
            break;
    }

    SetCtrlAttribute(g_iPanelHandle, iCtrl, ATTR_ON_COLOR, color);
    SetCtrlAttribute(g_iPanelHandle, iCtrl, ATTR_OFF_COLOR, color);
    SetCtrlVal(g_iPanelHandle, iCtrl, 1);
}

static void MultiDutUi_UpdateTimestamp(SlotUiState *slotData)
{
    time_t now = time(NULL);
    struct tm *pNow = localtime(&now);

    if (slotData == NULL)
    {
        return;
    }

    if (pNow != NULL)
    {
        strftime(slotData->timestamp, sizeof(slotData->timestamp), "%Y-%m-%d %H:%M:%S", pNow);
    }
    else
    {
        strcpy(slotData->timestamp, "n/a");
    }
}

static void MultiDutUi_BuildTableHeaders(void)
{
    if (g_iPanelHandle <= 0 || g_iResultTable <= 0)
    {
        return;
    }

    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 1, ATTR_COLUMN_WIDTH, 96);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 2, ATTR_COLUMN_WIDTH, 120);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 3, ATTR_COLUMN_WIDTH, 90);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 4, ATTR_COLUMN_WIDTH, 160);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 5, ATTR_COLUMN_WIDTH, 90);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 6, ATTR_COLUMN_WIDTH, 120);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 7, ATTR_COLUMN_WIDTH, 90);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 8, ATTR_COLUMN_WIDTH, 200);
    SetTableColumnAttribute(g_iPanelHandle, g_iResultTable, 9, ATTR_COLUMN_WIDTH, 140);
}

static void MultiDutUi_BuildSelectedInfoHeaders(void)
{
    if (g_iPanelHandle <= 0 || g_iSelectedInfoTable <= 0)
    {
        return;
    }

    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 1, ATTR_COLUMN_WIDTH, 96);
    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 2, ATTR_COLUMN_WIDTH, 120);
    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 3, ATTR_COLUMN_WIDTH, 110);
    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 4, ATTR_COLUMN_WIDTH, 140);
    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 5, ATTR_COLUMN_WIDTH, 140);
    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 6, ATTR_COLUMN_WIDTH, 140);
    SetTableColumnAttribute(g_iPanelHandle, g_iSelectedInfoTable, 7, ATTR_COLUMN_WIDTH, 90);
}

static void MultiDutUi_ClearResultRow(int rowIndex)
{
    int rows = 0;
	int col = 1;

    if (g_iPanelHandle <= 0 || g_iResultTable <= 0)
        return;

    if (GetNumTableRows(g_iPanelHandle, g_iResultTable, &rows) < 0)
        return;

    if (rowIndex < 1 || rowIndex > rows)
        return;

    for ( col = 1; col <= MULTI_DUT_RESULT_COLUMNS; col++)
    {
        SetTableCellVal(g_iPanelHandle, g_iResultTable, MakePoint(col, rowIndex), "");
    }
}

static void MultiDutUi_EnsureInfoTableRow(void)
{
    int rows = 0;

    if (g_iPanelHandle <= 0 || g_iSelectedInfoTable <= 0)
        return;

    if (GetNumTableRows(g_iPanelHandle, g_iSelectedInfoTable, &rows) < 0)
        return;

    if (rows == 0)
    {
        // Insert one string row at row index 1
        InsertTableRows(g_iPanelHandle, g_iSelectedInfoTable, 1, 1, VAL_CELL_STRING);
    }
}
static void MultiDutUi_ClearInfoRow(int rowIndex)
{
    int rows = 0;
    int col = 1;

    if (g_iPanelHandle <= 0 || g_iSelectedInfoTable <= 0)
        return;

    if (GetNumTableRows(g_iPanelHandle, g_iSelectedInfoTable, &rows) < 0)
        return;

    if (rowIndex < 1 || rowIndex > rows)
        return;

    for (col = 1; col <= MULTI_DUT_INFO_COLUMNS; col++)
    {
        SetTableCellVal(g_iPanelHandle, g_iSelectedInfoTable, MakePoint(col, rowIndex), "");
    }
}



static const char *MultiDutUi_StateText(MultiDutState state)
{
    switch (state)
    {
        case MULTI_DUT_STATE_IDLE:
            return "Idle";
        case MULTI_DUT_STATE_QUEUED:
            return "Queued";
        case MULTI_DUT_STATE_BATCH_POWER:
            return "Batch Power";
        case MULTI_DUT_STATE_TESTING:
            return "Testing";
        case MULTI_DUT_STATE_PASS:
            return "Pass";
        case MULTI_DUT_STATE_FAIL:
            return "Fail";
        default:
            return "Unknown";
    }
}

static void MultiDutUi_SetResultRowFromEntry(int rowIndex, const SlotUiState *pSlot, const SlotResultEntry *pEntry)
{
    const char *values[MULTI_DUT_RESULT_COLUMNS];
    char szLastTest[400] = {0};

    /* Build "TestId - Description" text */
    sprintf(szLastTest, "%s%s%s",
            pEntry->view.testId,
            strlen(pEntry->view.description) ? " - " : "",
            pEntry->view.description);

    /* Fill array of string pointers (rowIndex is 1-based for CVI tables) */
    values[0] = pSlot->slotLabel;
    values[1] = strlen(pEntry->serial) ? pEntry->serial : pSlot->serial;
    values[2] = MultiDutUi_StateText(pEntry->state);
    values[3] = strlen(szLastTest) ? szLastTest : "n/a";
    values[4] = strlen(pEntry->view.value) ? pEntry->view.value : "n/a";
    values[5] = strlen(pEntry->view.limits) ? pEntry->view.limits : "n/a";
    values[6] = strlen(pEntry->runResult) ? pEntry->runResult : "Pending";
    values[7] = strlen(pEntry->note) ? pEntry->note : pSlot->note;
    values[8] = strlen(pEntry->timestamp) ? pEntry->timestamp : pSlot->timestamp;

    /* One call updates the whole row: 1 row x MULTI_DUT_RESULT_COLUMNS cols */
    SetTableCellRangeVals(
        g_iPanelHandle,
        g_iResultTable,
        MakeRect(rowIndex, 1, 1, MULTI_DUT_RESULT_COLUMNS),
        (void *)values,
        VAL_ROW_MAJOR);
}


static void MultiDutUi_UpdateResultTable(void)
{
    SlotUiState *pSlot;
    int iRow;

    if (g_iPanelHandle <= 0 || g_iResultTable <= 0)
        return;

    pSlot = &g_aSlotData[g_iSelectedSlot];

    /* Batch UI changes to avoid flicker */
    SetCtrlAttribute(g_iPanelHandle, g_iResultTable, ATTR_VISIBLE, 0);

    /* Ensure the correct number of rows (at least 1 placeholder row) */
    MultiDutUi_ResizeResultTable(pSlot->resultCount > 0 ? pSlot->resultCount : 1);

    /* IMPORTANT: do NOT clear all rows anymore  that was causing flicker */

    if (pSlot->resultCount <= 0)
    {
        /* Just fill row 1 as placeholder */
        MultiDutUi_SetResultRowPlaceholder(pSlot);
    }
    else
    {
        /* Fill each row from SlotResultEntry using SetTableCellRangeVals */
        for (iRow = 0; iRow < pSlot->resultCount; ++iRow)
        {
            SlotResultEntry *pEntry = &pSlot->results[iRow];
            MultiDutUi_SetResultRowFromEntry(iRow + 1, pSlot, pEntry);
        }
    }

    /* Now let CVI repaint once */
    SetCtrlAttribute(g_iPanelHandle, g_iResultTable, ATTR_VISIBLE, 1);
}


static void MultiDutUi_UpdateSelectedInfoTable(void)
{
    SlotUiState *pSlot;

    if (g_iPanelHandle <= 0 || g_iSelectedInfoTable <= 0)
        return;

    /* Avoid flicker while updating */
    SetCtrlAttribute(g_iPanelHandle, g_iSelectedInfoTable, ATTR_VISIBLE, 0);

    MultiDutUi_EnsureInfoTableRow();

    pSlot = &g_aSlotData[g_iSelectedSlot];

    /* No need to clear: we overwrite all 7 cells */
    MultiDutUi_SetSelectedInfoRow(pSlot);

    SetCtrlAttribute(g_iPanelHandle, g_iSelectedInfoTable, ATTR_VISIBLE, 1);
}


static void MultiDutUi_SetResultRowPlaceholder(const SlotUiState *pSlot)
{
    const char *values[MULTI_DUT_RESULT_COLUMNS];

    values[0] = pSlot->slotLabel;
    values[1] = strlen(pSlot->serial) ? pSlot->serial : "Waiting for serial";
    values[2] = MultiDutUi_StateText(pSlot->state);
    values[3] = strlen(pSlot->lastDescription) ? pSlot->lastDescription : "n/a";
    values[4] = strlen(pSlot->lastValue) ? pSlot->lastValue : "n/a";
    values[5] = strlen(pSlot->lastLimits) ? pSlot->lastLimits : "n/a";
    values[6] = strlen(pSlot->lastRunResult) ? pSlot->lastRunResult : "Pending";
    values[7] = strlen(pSlot->note) ? pSlot->note : "Placeholder for measurement summary";
    values[8] = strlen(pSlot->timestamp) ? pSlot->timestamp : "n/a";

    SetTableCellRangeVals(
        g_iPanelHandle,
        g_iResultTable,
        MakeRect(1, 1, 1, MULTI_DUT_RESULT_COLUMNS),
        (void *)values,
        VAL_ROW_MAJOR);
}

static void MultiDutUi_SetSelectedInfoRow(const SlotUiState *pSlot)
{
    const char *values[MULTI_DUT_INFO_COLUMNS];

    values[0] = strlen(pSlot->slotLabel)          ? pSlot->slotLabel          : "n/a";
    values[1] = strlen(pSlot->unitId)             ? pSlot->unitId             : "n/a";
    values[2] = strlen(pSlot->unitIdType)         ? pSlot->unitIdType         : "n/a";
    values[3] = strlen(pSlot->testPlanName)       ? pSlot->testPlanName       : "n/a";
    values[4] = strlen(pSlot->testPlanGroup)      ? pSlot->testPlanGroup      : "n/a";
    values[5] = strlen(pSlot->unitMaterialNumber) ? pSlot->unitMaterialNumber : "n/a";
    values[6] = strlen(pSlot->lastRunResult)      ? pSlot->lastRunResult      : "Pending";

    SetTableCellRangeVals(
        g_iPanelHandle,
        g_iSelectedInfoTable,
        MakeRect(1, 1, 1, MULTI_DUT_INFO_COLUMNS),
        (void *)values,
        VAL_ROW_MAJOR);
}



static int CVICALLBACK SlotLedCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int slotIndex;

    slotIndex = (int)(intptr_t)callbackData;
    if (slotIndex < 0 || slotIndex >= MULTI_DUT_SLOT_COUNT)
    {
        return 0;
    }

    if (event == EVENT_RIGHT_CLICK)
    {
        MultiDutUi_ToggleSlotEnabled(slotIndex);
        return 0;
    }

    if (event != EVENT_COMMIT)
    {
        return 0;
    }

    g_iSelectedSlot = slotIndex;
    MultiDutUi_UpdateResultTable();
    MultiDutUi_UpdateSelectedInfoTable();

    return 0;
}

static int MultiDutUi_ToggleSlotEnabled(int slotIndex)
{
    char szSlotId[32] = {0};
    char szPassword[64] = {0};
    int iCurrentEnabled;
    int iNewEnabled;

    sprintf(szSlotId, "SLOT_%02d", slotIndex + 1);

    iCurrentEnabled = FixtureSlot_IsEnabledById(szSlotId);
    iNewEnabled = iCurrentEnabled ? 0 : 1;

    if (iNewEnabled)
    {
        if (!ConfirmPopup("Slot Access", "Enable this slot?"))
        {
            return 0;
        }
    }
    else
    {
        if (!ConfirmPopup("Slot Access", "Disable this slot?"))
        {
            return 0;
        }
    }

    PromptPopup("Slot Access", "Enter password to enable/disable slot", szPassword, sizeof(szPassword) - 1);
   

    if (strcmp(szPassword, "ADASSCR2026") != 0)
    {
        MessagePopup("Slot Access", "Invalid password");
        return -1;
    }

    if (!FixtureSlot_SetEnabledById(szSlotId, iNewEnabled))
    {
        MessagePopup("Slot Access", "Failed to update slot status");
        return -1;
    }

    if (iNewEnabled)
    {
        WriteToDataWin("[FixtureSlot] %s enabled", szSlotId);
        MessagePopup("Slot Access", "Slot enabled");
    }
    else
    {
        WriteToDataWin("[FixtureSlot] %s disabled", szSlotId);
        MessagePopup("Slot Access", "Slot disabled");
    }

    return 0;
}

static void MultiDutUi_ResizeResultTable(int rowCount)
{
    int target = rowCount < 1 ? 1 : rowCount;

    if (g_iPanelHandle <= 0 || g_iResultTable <= 0)
    {
        return;
    }

    if (target > g_iResultTableRows)
    {
        InsertTableRows(g_iPanelHandle, g_iResultTable, g_iResultTableRows + 1, target - g_iResultTableRows, VAL_CELL_STRING);
    }
    else if (target < g_iResultTableRows)
    {
        DeleteTableRows(g_iPanelHandle, g_iResultTable, target + 1, g_iResultTableRows - target);
    }

    g_iResultTableRows = target;
}

static int MultiDutUi_EnsureResultCapacity(SlotUiState *slotData, int neededRows)
{
    int newCapacity;
    SlotResultEntry *pNewBuffer;

    if (slotData == NULL)
    {
        return -1;
    }

    if (slotData->resultCapacity >= neededRows)
    {
        return 0;
    }

    newCapacity = slotData->resultCapacity > 0 ? slotData->resultCapacity : 8;
    while (newCapacity < neededRows)
    {
        newCapacity *= 2;
    }

    pNewBuffer = (SlotResultEntry *)realloc(slotData->results, sizeof(SlotResultEntry) * newCapacity);
    if (pNewBuffer == NULL)
    {
        return -1;
    }

    slotData->results = pNewBuffer;
    slotData->resultCapacity = newCapacity;

    return 0;
}

static void MultiDutUi_ResetSlotMeasurements(SlotUiState *slotData)
{
    if (slotData == NULL)
    {
        return;
    }

    slotData->resultCount = 0;

    strcpy(slotData->lastTestId, "n/a");
    strcpy(slotData->lastDescription, "No measurements recorded");
    strcpy(slotData->lastValue, "n/a");
    strcpy(slotData->lastLimits, "n/a");
    strcpy(slotData->lastRunResult, "Pending");
    slotData->timestamp[0] = '\0';
    strcpy(slotData->note, "Awaiting assignment");
}
