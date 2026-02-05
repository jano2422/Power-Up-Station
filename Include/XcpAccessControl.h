/********************************************************************************/
/*                                                                              */
/*  Header File: XcpAccessControl.h                                             */
/*                                                                              */
/********************************************************************************
 *
 *  Version    Date         Author        Changes
 *  ---------------------------------------------------------------------------
 *  V1.00      20.01.2012   fehrmanng     Initial realization (PSW Reflash)
 *  V1.10      09.12.2025   anchetajc     Renamed module to XcpAccessControl
 *                                        Added boXcpUnlock() interface
 *                                        Updated documentation
 *
 *  Documentation:
 *  --------------
 *  XCP Access Control Module
 *
 *  - Provides functions to enable diagnostic/XCP access on DUT after power-up.
 *  - Used during pre-test initialization and before executing main testplan.
 *  - Handles CAN message simulations, PSU control, and basic communication setup.
 *
 ********************************************************************************/

#ifndef _XCP_ACCESS_CONTROL_H_
#define _XCP_ACCESS_CONTROL_H_

#include <CviTypes.h>

/**
 * Function: boXcpUnlock
 *
 * Purpose:
 *   Enables XCP access on the DUT by configuring CAN simulation, powering DUT,
 *   starting diagnostic mode, and sending XCP/UDS requests for unlock.
 *
 * Return:
 *   1 on success
 *   0 or __LINE__ on failure
 */
int boXcpUnlock(void);

#endif /* _XCP_ACCESS_CONTROL_H_ */
