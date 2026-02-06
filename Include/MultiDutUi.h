#ifndef __MULTI_DUT_UI_H__
#define __MULTI_DUT_UI_H__

#include <cvidef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    MULTI_DUT_STATE_IDLE = 0,
    MULTI_DUT_STATE_QUEUED,
    MULTI_DUT_STATE_BATCH_POWER,
    MULTI_DUT_STATE_TESTING,
    MULTI_DUT_STATE_PASS,
    MULTI_DUT_STATE_FAIL
} MultiDutState;

typedef struct
{
    char testId[128];
    char description[256];
    char value[96];
    char limits[96];
} MultiDutResultView;

typedef struct
{
    char fixtureId[32];
    char unitId[100];
    char unitIdType[100];
    char testPlanName[64];
    char unitMaterialNumber[100];
    char result[32];
} MultiDutSelectedInfo;

int MultiDutUi_Init(void);
void MultiDutUi_Terminate(void);
void MultiDutUi_SetSlotState(const char *slotId, MultiDutState state, const char *serialNumber, const char *note);
void MultiDutUi_RecordMeasurement(const char *slotId, const char *serialNumber, const MultiDutResultView *result, const char *note);
void MultiDutUi_FinalizeRun(const char *slotId, char result, const char *note);
void MultiDutUi_UpdateSlotMetadata(const char *slotId, const MultiDutSelectedInfo *info);

#ifdef __cplusplus
}
#endif

#endif
