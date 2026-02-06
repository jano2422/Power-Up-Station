#include <windows.h>
#include <ansi_c.h>
#include <cviauto.h>
#include <dataskt.h>
#include "toolbox.h"
#include <utility.h>
#include <formatio.h>
#include <cvitypes.h>
#include <SHLWAPI.H> 

#include "FixtureSlot.h"
#include "ApplicationTools.h"
#include "RunSpsTestprogramm.h"

#define FIXTURE_SLOT_MAX_ENTRIES 64

typedef struct {
    char szId[FIXTURE_SLOT_MAX_ID_LEN];
    int iPublicRelay;
    int iPrivateRelay;
    int iPowerSupplyRelay;
    int iEnabled;
} FixtureSlotEntry;

static FixtureSlotEntry g_aFixtureSlotMap[FIXTURE_SLOT_MAX_ENTRIES];
static int g_iFixtureSlotCount = 0;
static int g_iDefaultPublicRelay = -1;
static int g_iDefaultPrivateRelay = -1;
static int g_iDefaultPowerSupplyRelay = -1;
static int g_iActivePublicRelay = -1;
static int g_iActivePrivateRelay = -1;
static int g_iActivePowerSupplyRelay = -1;
static char g_szActiveFixtureId[FIXTURE_SLOT_MAX_ID_LEN] = {0};

static void FixtureSlot_UpdateActiveRelays(const char *fixtureId);
static void TrimWhitespace(char *text);
static void FixtureSlot_ResetMap(void)
{
    g_iFixtureSlotCount = 0;
    memset(g_aFixtureSlotMap, 0, sizeof(g_aFixtureSlotMap));
}

static void FixtureSlot_ApplyEntry(const char *fixtureId, int publicRelay, int privateRelay, int powerSupplyRelay, int enabled)
{
    if (g_iFixtureSlotCount >= FIXTURE_SLOT_MAX_ENTRIES)
    {
        WriteToErrorWin("[FixtureSlot] Mapping table full. Ignoring %s", fixtureId);
        return;
    }

    strncpy(g_aFixtureSlotMap[g_iFixtureSlotCount].szId, fixtureId, sizeof(g_aFixtureSlotMap[g_iFixtureSlotCount].szId) - 1);
    g_aFixtureSlotMap[g_iFixtureSlotCount].iPublicRelay = publicRelay;
    g_aFixtureSlotMap[g_iFixtureSlotCount].iPrivateRelay = privateRelay;
    g_aFixtureSlotMap[g_iFixtureSlotCount].iPowerSupplyRelay = powerSupplyRelay;
    g_aFixtureSlotMap[g_iFixtureSlotCount].iEnabled = enabled ? 1 : 0;
    g_iFixtureSlotCount++;
}

static void TrimWhitespace(char *text)
{
    char *start = text;
    char *end;

    if (text == NULL)
    {
        return;
    }

    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }

    end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1)))
    {
        *(--end) = '\0';
    }

    if (start != text)
    {
        memmove(text, start, (size_t)(end - start) + 1);
    }
}

static void FixtureSlot_ParseLine(const char *line)
{
    char szBuffer[256];
    char szId[FIXTURE_SLOT_MAX_ID_LEN] = {0};
    int publicRelay = -1;
    int privateRelay = -1;
    int powerSupplyRelay = -1;
    int enabled = 1;
	int iParsedCount = 0;
	
    strncpy(szBuffer, line, sizeof(szBuffer) - 1);
    TrimWhitespace(szBuffer);
    if (strlen(szBuffer) == 0 || szBuffer[0] == '#')
    {
        return;
    }

    
        iParsedCount = sscanf(szBuffer, "%63[^=]=%d,%d,%d,%d", szId, &publicRelay, &privateRelay, &powerSupplyRelay, &enabled);
        if (iParsedCount == 3)
        {
            powerSupplyRelay = g_iDefaultPowerSupplyRelay;
        }

        if (iParsedCount >= 3)
        {
            TrimWhitespace(szId);
            if (strlen(szId) == 0)
            {
                return;
            }
            FixtureSlot_ApplyEntry(szId, publicRelay, privateRelay, powerSupplyRelay, enabled);
        
    }
}

void FixtureSlot_Initialize(int defaultPublicRelay, int defaultPrivateRelay, int defaultPowerSupplyRelay)
{
    g_iDefaultPublicRelay = defaultPublicRelay;
    g_iDefaultPrivateRelay = defaultPrivateRelay;
    g_iDefaultPowerSupplyRelay = defaultPowerSupplyRelay;
    FixtureSlot_SetActiveId("SLOT_01");
    FixtureSlot_ReloadConfig();
}

void FixtureSlot_ReloadConfig(void)
{
    FILE *pFile = NULL;
    char szLine[256];

    FixtureSlot_ResetMap();

    pFile = fopen("FixtureSlots.cfg", "r");
    if (pFile == NULL)
    {
        WriteToErrorWin("[FixtureSlot] Could not open FixtureSlots.cfg. Using default relays.");
        return;
    }

    while (fgets(szLine, sizeof(szLine), pFile) != NULL)
    {
        FixtureSlot_ParseLine(szLine);
    }

    fclose(pFile);

    FixtureSlot_UpdateActiveRelays(g_szActiveFixtureId);
}

static void FixtureSlot_UpdateActiveRelays(const char *fixtureId)
{
    int iIndex;

    for (iIndex = 0; iIndex < g_iFixtureSlotCount; iIndex++)
    {
        if (StrICmp(fixtureId, g_aFixtureSlotMap[iIndex].szId) == 0)
        {
            if (g_aFixtureSlotMap[iIndex].iEnabled)
            {
                g_iActivePublicRelay = g_aFixtureSlotMap[iIndex].iPublicRelay;
                g_iActivePrivateRelay = g_aFixtureSlotMap[iIndex].iPrivateRelay;
                g_iActivePowerSupplyRelay = g_aFixtureSlotMap[iIndex].iPowerSupplyRelay;
            }
            else
            {
                g_iActivePublicRelay = -1;
                g_iActivePrivateRelay = -1;
                g_iActivePowerSupplyRelay = -1;
            }
            return;
        }
    }

    g_iActivePublicRelay = g_iDefaultPublicRelay;
    g_iActivePrivateRelay = g_iDefaultPrivateRelay;
    g_iActivePowerSupplyRelay = g_iDefaultPowerSupplyRelay;
}

void FixtureSlot_SetActiveId(const char *fixtureId)
{
    if (fixtureId == NULL || strlen(fixtureId) == 0)
    {
        return;
    }

    memset(g_szActiveFixtureId, 0, sizeof(g_szActiveFixtureId));
    strncpy(g_szActiveFixtureId, fixtureId, sizeof(g_szActiveFixtureId) - 1);
    FixtureSlot_UpdateActiveRelays(fixtureId);
    WriteToDataWin("[FixtureSlot] Active fixture: %s (Public:%d Private:%d Power:%d)",
                   g_szActiveFixtureId,
                   g_iActivePublicRelay,
                   g_iActivePrivateRelay,
                   g_iActivePowerSupplyRelay);
}

const char *FixtureSlot_GetActiveId(void)
{
    return g_szActiveFixtureId;
}

int FixtureSlot_GetActivePublicCanRelay(void)
{
    return g_iActivePublicRelay;
}

int FixtureSlot_GetActivePrivateCanRelay(void)
{
    return g_iActivePrivateRelay;
}

int FixtureSlot_GetActivePowerSupplyRelay(void)
{
    return g_iActivePowerSupplyRelay;
}

int FixtureSlot_GetPowerSupplyRelayById(const char *fixtureId)
{
    int i;

    if (fixtureId == NULL || fixtureId[0] == '\0')
        return -1;

    for (i = 0; i < g_iFixtureSlotCount; i++)
    {
        if (StrICmp(fixtureId, g_aFixtureSlotMap[i].szId) == 0)
            return g_aFixtureSlotMap[i].iPowerSupplyRelay;
    }

    return -1;
}



int FixtureSlot_IsEnabledById(const char *fixtureId)
{
    int i;

    if (fixtureId == NULL || fixtureId[0] == '\0')
        return 0;

    for (i = 0; i < g_iFixtureSlotCount; i++)
    {
        if (StrICmp(fixtureId, g_aFixtureSlotMap[i].szId) == 0)
            return g_aFixtureSlotMap[i].iEnabled ? 1 : 0;
    }

    return 0;
}

int FixtureSlot_SetEnabledById(const char *fixtureId, int enabled)
{
    FILE *pFile = NULL;
    int i;

    if (fixtureId == NULL || fixtureId[0] == '\0')
        return 0;

    for (i = 0; i < g_iFixtureSlotCount; i++)
    {
        if (StrICmp(fixtureId, g_aFixtureSlotMap[i].szId) == 0)
        {
            g_aFixtureSlotMap[i].iEnabled = enabled ? 1 : 0;

            pFile = fopen("FixtureSlots.cfg", "w");
            if (pFile == NULL)
            {
                WriteToErrorWin("[FixtureSlot] Could not write FixtureSlots.cfg");
                return 0;
            }

            fprintf(pFile, "# Fixture slot configuration\n");
            fprintf(pFile, "# Format: <FixtureSlotId>=<PublicCanRelay>,<PrivateCanRelay>,<PowerSupplyRelay>,<Enabled>\n");
            fprintf(pFile, "# Enabled: 1 = enabled, 0 = disabled\n\n");

            for (i = 0; i < g_iFixtureSlotCount; i++)
            {
                fprintf(pFile, "%s=%d,%d,%d,%d\n",
                        g_aFixtureSlotMap[i].szId,
                        g_aFixtureSlotMap[i].iPublicRelay,
                        g_aFixtureSlotMap[i].iPrivateRelay,
                        g_aFixtureSlotMap[i].iPowerSupplyRelay,
                        g_aFixtureSlotMap[i].iEnabled ? 1 : 0);
            }

            fclose(pFile);
            FixtureSlot_UpdateActiveRelays(g_szActiveFixtureId);
            return 1;
        }
    }

    return 0;
}

int FixtureSlot_GetConfiguredCount(void)
{
    return g_iFixtureSlotCount;
}

int FixtureSlot_GetConfiguredIdByIndex(int index, char *fixtureId, int fixtureIdBufferSize)
{
    if (fixtureId == NULL || fixtureIdBufferSize <= 1)
    {
        return 0;
    }

    if (index < 0 || index >= g_iFixtureSlotCount)
    {
        return 0;
    }

    memset(fixtureId, 0, (size_t)fixtureIdBufferSize);
    strncpy(fixtureId, g_aFixtureSlotMap[index].szId, (size_t)fixtureIdBufferSize - 1);
    return 1;
}
