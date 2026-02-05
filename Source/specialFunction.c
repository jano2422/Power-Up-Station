#include <ansi_c.h>
#include <userint.h>

//==============================================================================
//
// Title:       specialFunction.c
// Purpose:     Special Function for the Display and Status.
//
// Created on:  8/27/2009 at 12:57:33 PM by Michael M. Saligao.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "specialFunction.h"
#include "Control.PXI6528.h"
#include "FixtureSlot.h"
#include "MultiDutUi.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static MultiDutState statusActionToState(int action);
static const char *statusActionToNote(int action);

//==============================================================================
// Global variables

extern char g_szSerialId[];
//==============================================================================
// Global functions


int status_init (void)
{
        return MultiDutUi_Init();

}

int setStatus (int Action)
{
        const char *activeSlotId = FixtureSlot_GetActiveId();
        MultiDutState state = statusActionToState(Action);
        const char *note = statusActionToNote(Action);
        MultiDutUi_SetSlotState(activeSlotId, state, g_szSerialId, note);
        return 0;

}

static MultiDutState statusActionToState(int action)
{
        switch (action)
        {
                case TESTING:
                        return MULTI_DUT_STATE_TESTING;
                case PASS:
                        return MULTI_DUT_STATE_PASS;
                case FAIL:
                case TIMESFAIL3:
                case UNITLOCKED:
                        return MULTI_DUT_STATE_FAIL;
                case LOADING_PLS:
                case validBarcode:
                case READY:
                        return MULTI_DUT_STATE_QUEUED;
                case noECU:
                case emptyBarcode:
                default:
                        return MULTI_DUT_STATE_IDLE;
        }
}

static const char *statusActionToNote(int action)
{
        switch (action)
        {
                case TESTING:
                        return "Test sequence in progress (view only).";
                case PASS:
                        return "Latest measurement reported PASS.";
                case FAIL:
                        return "Latest measurement reported FAIL.";
                case TIMESFAIL3:
                        return "Unit failed three times (locked).";
                case UNITLOCKED:
                        return "Unit locked pending manual review.";
                case LOADING_PLS:
                        return "Queued and preparing instrumentation.";
                case validBarcode:
                        return "Serial scanned and queued.";
                case READY:
                        return "Ready for next measurement cycle.";
                case noECU:
                        return "No ECU detected in the fixture.";
                case emptyBarcode:
                default:
                        return "Awaiting assignment.";
        }
}

