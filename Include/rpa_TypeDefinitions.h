/* SOURCE FILE <rpa_TypeDefinitions.h>
 **************************************************************************
 *  			                                                          *
 **************************************************************************
 *  COPYRIGHT (C) CONTINENTAL AUTOMOTIVE SYSTEMS 2009                     *
 *  AUTOMOTIVE ELECTRONICS REGENSBURG                                     *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 *  The reproduction, transmission or use of this document or its         *
 *  contents is not permitted without express written authority.          *
 *  Offenders will be liable for damages. All rights, including rights    *
 *  created by patent grant or registration of a utility model or design, *
 *  are reserved.                                                         *
 **************************************************************************
 *  PROJECT       MAZDA BSD
 *  MODULE        RPA
 *  RESPONSIBLE   Sirghie Daniela
 **************************************************************************
 *  Information automatically provided by PVCS:
 *  -------------------------------------------
 *  FILE          $Workfile:   rpa_TypeDefinitions.h  $
 *  LAST REVISION $Revision:   $
 *  LAST AUTHOR   $Author:     $
 *  LAST MODIFIED $Modtime:    $
 *  ARCHIVE       $Archive:    $
 **************************************************************************
 *  PURPOSE:
 *  Define special structs for using in RPA module 
 *   
 *
 **************************************************************************
 *  REMARKS:
 *   
 *
 **************************************************************************
 *  PVCS CHANGE HISTORY:
 *   Log of all revisions so far:
 *   $Log:   $
 *
 *
 *************************************************************************/
#include "rttypes.h" 

#ifndef _RPA_TYPEDEFINITIONS_H_
#define _RPA_TYPEDEFINITIONS_H_

typedef struct {

T_REAL64 * prl64_freq;
T_REAL64 ** pprl64_eirp;
T_REAL64 * prl64_eirp;
T_REAL64 * prl64_pwm;
T_REAL64 * prl64_temp;
T_REAL64 rl64_angle;
T_SLONG sl_m;
T_SLONG sl_n;

}struct_EIRPdata;
		

typedef struct {

T_SLONG     ** ppsl_outputmatrix;
T_SLONG     *  psl_outputmatrix;
T_SLONG        sl_n_temperaturevector;
T_SLONG		   sl_n_timevector;
T_REAL64	* prl64_meansquared_error;
T_REAL64	* prl64_targetpower;
T_REAL64	** pprl64_maxfreqvector;
T_REAL64	** pprl64_linearfitvector;

}struct_POWCtrlResult;


typedef struct {

T_UWORD auw_PWM_PWR_ovr_Step[2][28][280]; //currently not used 
T_REAL32 rl_offs_Tx_NB;
T_REAL32 rl_slope_Tx;
T_REAL32 rl_NB_lower_Limit_dBm;
T_REAL32 rl_NB_upper_Limit_dBm;
T_REAL32 rl_offs_Tx_BB;
T_REAL32 rl_slope_Tx_BB;
T_REAL32 rl_BB_lower_Limit_dBm;
T_REAL32 rl_BB_upper_Limit_dBm;
T_UWORD uw_LO_lower_Limit_raw;
T_UWORD uw_LO_upper_Limit_raw;
T_REAL32 rl_offs_Temp;
T_REAL32 rl_slope_Temp;
T_UWORD uw_RF_HW_cfg_flags;

}CAL_HW_s_Radar;

typedef struct
{
	T_ULONG *pSamplValTX;
	T_ULONG *pSamplValLO;
	T_REAL64 *pEIRP_mid;
	T_REAL64 dSlope;
	T_REAL64 dOffset;
	T_REAL64 dMeanSamplValLO;
	T_REAL64 *pdLinearFitting;
	
}struct_SampValEirpMid;

#endif
