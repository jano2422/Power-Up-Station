#ifndef __FIXTURE_SLOT_H
#define __FIXTURE_SLOT_H

#include <cvidef.h>

#define FIXTURE_SLOT_MAX_ID_LEN 64

void FixtureSlot_Initialize(int defaultPublicRelay, int defaultPrivateRelay, int defaultPowerSupplyRelay);
void FixtureSlot_ReloadConfig(void);
void FixtureSlot_SetActiveId(const char *fixtureId);
const char *FixtureSlot_GetActiveId(void);
int FixtureSlot_GetActivePublicCanRelay(void);
int FixtureSlot_GetActivePrivateCanRelay(void);
int FixtureSlot_GetActivePowerSupplyRelay(void);
int FixtureSlot_GetPowerSupplyRelayById(const char *fixtureId);
int FixtureSlot_IsEnabledById(const char *fixtureId);
int FixtureSlot_SetEnabledById(const char *fixtureId, int enabled);
int FixtureSlot_GetConfiguredCount(void);
int FixtureSlot_GetConfiguredIdByIndex(int index, char *fixtureId, int fixtureIdBufferSize);

#endif
