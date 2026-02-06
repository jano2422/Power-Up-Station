//****************************************************************************
//*                                                                          *
//*  File : RunSpsTestprogramm.h                                                  	     *
//*                                                                          *
/*****************************************************************************

	Dokumentation:

******************************************************************************/

#include <OpcTestprogramm.h>

#ifndef __RUN_SPS_H
#define __RUN_SPS_H


/*****************************************************************************/
typedef enum
{
    /* System Initialization */
    sWaitForSpsReady,              // Tester is powering up / initializing
    sWaitForStartTestOn,           // Waiting for Start Test signal or queue entry

    /* Pre-Validation & MES Checks */
    sScanAndVerifyIfValidForStation,  // Validate TMP SN, MES checks, customer SW, version checks
    sInitDut,                         // Initialize DUT hardware

    /* Pre-Test Access Control */
    sXcpUnlock,                       // Enable XCP/diagnostic access (replaces old sReadTempSn)

    /* Pretest Finalization */
    sCheckTempSn,                     // Final TMP SN / pretest handling

    /* Result Processing */
    sResetDut,                        // Reset DUT before determining pass/fail
    sDutPass,                         // PASS handling
    sDutFail,                         // FAIL handling
    sWaitReleaseDutFail,              // Waiting for operator to release failure

    /* Test Cycle End */
    sWaitForStartTestOff,             // Waiting for cycle end ? return to StartTestOn state

    /* Fatal Error */
    sError                             // Critical error requiring restart
} EStatus;


//////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	int iRead;
	int iPass;
	int iFail;
} TScanningQuality;

extern TScanningQuality tScanningQuality;

extern Boolean g_boTesterOk;

/*****************************************************************************/
extern short sRunSps(void);
typedef struct
{
        char szFixtureId[OpcStringLength];
        char szDutSerial[OpcStringLength];
} TFixtureScanRequest;

extern int FixtureScan_Enqueue(const char *fixtureId, const char *dutSerial);
extern int FixtureScan_Dequeue(TFixtureScanRequest *request);
extern int FixtureScan_Count(void);
extern void FixtureScan_Clear(void);
extern int FixtureScan_GetByIndex(int index, TFixtureScanRequest *request);
extern int FixtureScan_GetNextSlotNumber(void);
extern int FixtureScan_GetNextSlotId(char *slotId, int slotIdBufferSize);
extern void RunSps_SetStartAuthorized(int boAuthorized);
extern int RunSps_IsStartAuthorized(void);

extern int g_iPublicCanRelay;
extern int g_iPrivateCanRelay;
extern int g_iPowerSupplyRelay;

extern short g_sSlot;
extern char g_szTestPlan[20];
/*****************************************************************************/
#endif
