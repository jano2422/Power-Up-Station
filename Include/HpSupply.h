#ifndef __HB_SUPPLY
#define __HB_SUPPLY

#include <CviTypes.h>

typedef struct
{
    int gpibDev;
    int gpibAddr;
} HpPsDevice;

extern HpPsDevice g_HpSupplyPrimary;
extern HpPsDevice g_HpSupplySecondary;

extern Boolean boHpPsInit(HpPsDevice *device, int aGpibAdr);
extern Boolean boHpPsSetOutOn(HpPsDevice *device, float Volt, float Cur);
extern Boolean boHpPsSetOutput(HpPsDevice *device, TSwitch aOnOff);
extern Boolean boHpPsSetVoltage(HpPsDevice *device, float aVoltage);
extern Boolean boHpPsSetCurrent(HpPsDevice *device, float aCurrent);
extern Boolean boHpPsReadVoltage(HpPsDevice *device, float *aVoltage);
extern Boolean boHpPsReadCurrent(HpPsDevice *device, float *aCurrent);
extern Boolean boHpPsDisplay_Local(HpPsDevice *device);

#endif
