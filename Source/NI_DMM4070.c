/********************************************************************************/
/*                                                                           		*/
/*  Source File: NI_DMM4070.c                                       				*/
/*                                                                           		*/
/********************************************************************************

	Version 		Date				Author		 Changes
	------------------------------------------------
	V1.00		   26.09.2011   fehrmanng	 Initial realization
*********************************************************************************

	Documentation:
	--------------
	Driver layer for NI PCI-4070 FlexDMM 

*********************************************************************************/
#include <nidmm.h>
#include <ansi_c.h>
#include <CviTypes.h>
#include <utility.h>
#include <NI_DMM4070.h>

#define SAMPLES 50


static unsigned long ulSessionHandle = 0;
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmInit(char szSlotName[])
{
	
	if( 0 != niDMM_init((ViRsrc)szSlotName, VI_TRUE, VI_TRUE, &(ViSession)ulSessionHandle) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg10Volts(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 10.0, 5.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg10VoltsHiImp(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 10.0, 5.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_GREATER_THAN_10_GIGAOHM) ) {
		return( False );
	} 
	
	return( True );
}
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg10VoltsExtended(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 10.0, 6.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg100Volts(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 100.0, 5.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg100VoltsExtended(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 100.0, 6.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg1Volts(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 1.0, 5.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg1VoltsExtended(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 1.0, 6.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	} 
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg10VoltsMulti(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 10.0, 5.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	}
	if( 0 != niDMM_ConfigureMultiPoint ((ViSession)ulSessionHandle, 1, SAMPLES, NIDMM_VAL_IMMEDIATE, -1) ) {
		return( False );
	}
	
	
	return( True );
}
/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmCfg100VoltsMulti(void)
{
	if( 0 != niDMM_ConfigurePowerLineFrequency((ViSession)ulSessionHandle, 50.0) ) {
		return( False );
	} 
	if( 0 != niDMM_ConfigureMeasurementDigits((ViSession)ulSessionHandle, NIDMM_VAL_DC_VOLTS, 100.0, 5.5) ) {
		return( False );
	}
	if( 0 != niDMM_SetAttributeViReal64 ((ViSession)ulSessionHandle, "", NIDMM_ATTR_INPUT_RESISTANCE, NIDMM_VAL_10_MEGAOHM) ) {
		return( False );
	}
	if( 0 != niDMM_ConfigureMultiPoint ((ViSession)ulSessionHandle, 1, SAMPLES, NIDMM_VAL_IMMEDIATE, -1) ) {
		return( False );
	}
	
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmMeas(double *pdResult)
{
	
	if( 0 != niDMM_Read((ViSession)ulSessionHandle, NIDMM_VAL_TIME_LIMIT_AUTO, (ViPReal64)pdResult) ) {
		return( False );
	} 
	
	
	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmMeasSlow(double *pdResult)
{
	
	if( 0 != niDMM_Read((ViSession)ulSessionHandle, NIDMM_VAL_TIME_LIMIT_AUTO, (ViPReal64)pdResult) ) {
		return( False );
	} 
	

	return( True );
}

/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmMeasAverage(double *pdResult)
{
	int iNum = 0, iCnt = 0;
	double adValues[SAMPLES];
	double dValue = 0;
	memset(adValues, 0, sizeof(adValues));
	
	
	
	if( 0 != niDMM_ReadMultiPoint ((ViSession)ulSessionHandle, NIDMM_VAL_TIME_LIMIT_AUTO, SAMPLES, adValues, &iNum) ) {
		return( False );
	}
	for(iCnt = 0; iCnt < SAMPLES; iCnt++) {
		dValue = dValue + adValues[iCnt];	
	}
	*pdResult = dValue/(double)SAMPLES;
	
	
	return( True );
}


/********************************************************************************/
/* Version 1.00                                                                 */
/* Author:fehrmanng	                                                            */
/*                */
/********************************************************************************/
Boolean boNiDmmClose(void)
{
	
	if( 0 != niDMM_close((ViSession)ulSessionHandle) ) {
		return( False );
	} 
	
	return( True );
}
