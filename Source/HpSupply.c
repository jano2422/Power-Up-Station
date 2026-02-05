/***************************************************************************************/
/*                                                                                     */
/*  File : HpSuply.c                                                                   */
/*                                                                                     */
/***************************************************************************************

                Documentation:
                --------------
                allgemeine Routinen fr HP Power Supply

****************************************************************************************/
#include <utility.h>
#include <ansi_c.h>
#include <gpib.h>

#include <HpSupply.h>

//***************************************************************************************
// If TMSL is definied, then the new GPIB-Commands are used. Otherwhise commend this line
#define TMSL

//***************************************************************************************
static char szHP_Str[120];
HpPsDevice g_HpSupplyPrimary = {0};
HpPsDevice g_HpSupplySecondary = {0};

static Boolean boHpPsIsReady(const HpPsDevice *device)
{
        return (device != NULL && device->gpibDev > 0);
}

static Boolean boHpPsSetOutputSingle(HpPsDevice *device, TSwitch aOnOff)
{
        if (!boHpPsIsReady(device))
                return (False);

#ifdef TMSL
        if (aOnOff == On)
        {
                sprintf(szHP_Str, "OUTP:STAT ON");
        }
        else
        {
                sprintf(szHP_Str, "OUTP:STAT OFF");
        }
#else
        if (aOnOff == On)
        {
                sprintf(szHP_Str, "OUT ON");
        }
        else
        {
                sprintf(szHP_Str, "OUT OFF");
        }
#endif
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        return (True);
}

static Boolean boHpPsSetVoltageSingle(HpPsDevice *device, float aVoltage)
{
        if (!boHpPsIsReady(device))
                return (False);

#ifdef TMSL
        sprintf(szHP_Str, "VOLT %2.3f", aVoltage);
#else
        sprintf(szHP_Str, "VSET %2.3f", aVoltage);
#endif
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        return (True);
}

static Boolean boHpPsSetCurrentSingle(HpPsDevice *device, float aCurrent)
{
        if (!boHpPsIsReady(device))
                return (False);

#ifdef TMSL
        sprintf(szHP_Str, "CURR %2.3f", aCurrent);
#else
        sprintf(szHP_Str, "ISET %2.3f", aCurrent);
#endif
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        return (True);
}

//***************************************************************************************
Boolean boHpPsInit(HpPsDevice *device, int aGpibAdr)
{
        if (!device)
                return (False);

        if (device->gpibDev > 0 && device->gpibAddr != aGpibAdr)
        {
                ibonl(device->gpibDev, 0);
                device->gpibDev = 0;
        }

        if (device->gpibDev == 0)
        {
                device->gpibDev = ibdev(0, aGpibAdr, NO_SAD, T10s, 1, 0x10);
                if (device->gpibDev <= 0)
                {
                        device->gpibDev = 0;
                        return (False);
                }
                device->gpibAddr = aGpibAdr;
                if ((ibsta & ERR) || (ibsta & TIMO))
                        return (False);
        }
        // Clear Device
        ibclr(device->gpibDev);
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

#ifdef TMSL
        sprintf(szHP_Str, "*RST");
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        sprintf(szHP_Str, "*CLS");
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);
        /*
                sprintf(szHP_Str, "DISP ON");
                ibwrt (iHpPsGpibDev, szHP_Str, strlen(szHP_Str));
                if ((ibsta & ERR) || (ibsta & TIMO))
                        return( False );
        */
        sprintf(szHP_Str, "OUTP:STAT ON");
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);
#else
        sprintf(szHP_Str, "CLR");
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        sprintf(szHP_Str, "OUT ON");
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);
#endif

        return (True);
}

//***************************************************************************************
Boolean boHpPsSetOutOn(HpPsDevice *device, float Volt, float Cur)
{
        if (!boHpPsSetCurrent(device, Cur))
        {
                return (False);
        }
        if (!boHpPsSetVoltage(device, Volt))
        {
                return (False);
        }
        if (!boHpPsSetOutput(device, 1))
        {
                return (False);
        }

        return (True);
}

//***************************************************************************************
Boolean boHpPsSetOutput(HpPsDevice *device, TSwitch aOnOff)
{
        return boHpPsSetOutputSingle(device, aOnOff);
}

//***************************************************************************************
Boolean boHpPsSetVoltage(HpPsDevice *device, float aVoltage)
{
        return boHpPsSetVoltageSingle(device, aVoltage);
}

//***************************************************************************************
Boolean boHpPsSetCurrent(HpPsDevice *device, float aCurrent)
{
        return boHpPsSetCurrentSingle(device, aCurrent);
}

//***************************************************************************************
Boolean boHpPsReadVoltage(HpPsDevice *device, float *aVoltage)
{
        int iRes;
        char szHP_Rec[256];
        char szHP_Str[120];
        float fTempVolt;

        if (!boHpPsIsReady(device))
                return (False);

#ifdef TMSL
        strcpy(szHP_Str, "MEAS:VOLT?");
#else
        strcpy(szHP_Str, "VOUT?");
#endif
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        // Einlesen des Messergebnisses
        memset(szHP_Rec, 0, sizeof(szHP_Rec));
        iRes = ibrd(device->gpibDev, szHP_Rec, sizeof(szHP_Rec));

        fTempVolt = atof(szHP_Rec);
        *aVoltage = fTempVolt;

        return (True);
}

//***************************************************************************************
Boolean boHpPsReadCurrent(HpPsDevice *device, float *aCurrent)
{
        int iRes;
        char szHP_Rec[256];
        char szHP_Str[120];
        float fTempCurr;

        if (!boHpPsIsReady(device))
                return (False);

#ifdef TMSL
        strcpy(szHP_Str, "MEAS:CURR?");
#else
        strcpy(szHP_Str, "IOUT?");
#endif
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        // Einlesen des Messergebnisses
        memset(szHP_Rec, 0, sizeof(szHP_Rec));
        iRes = ibrd(device->gpibDev, szHP_Rec, sizeof(szHP_Rec));

        fTempCurr = atof(szHP_Rec);
        *aCurrent = fTempCurr;

        return (True);
}

//***************************************************************************************
Boolean boHpPsDisplay_Local(HpPsDevice *device)
{
        if (!boHpPsIsReady(device))
                return (False);

#ifdef TMSL
        sprintf(szHP_Str, "SYST:LOC");
#else
        sprintf(szHP_Str, "LOCAL");
#endif
        ibwrt(device->gpibDev, szHP_Str, strlen(szHP_Str));
        if ((ibsta & ERR) || (ibsta & TIMO))
                return (False);

        return (True);
}

