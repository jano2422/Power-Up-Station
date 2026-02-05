/********************************************************************************/
/*                                                                           		*/
/*  Header File: NI_DMM4070.h			                                      		*/
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
#ifndef _NI_DMM4070_h_
#define _NI_DMM4070_h_ 

#include <CviTypes.h>

Boolean boNiDmmInit(char szSlotName[]);
Boolean boNiDmmCfg10Volts(void);
Boolean boNiDmmCfg10VoltsHiImp(void);
Boolean boNiDmmCfg10VoltsExtended(void);
Boolean boNiDmmCfg100Volts(void); 
Boolean boNiDmmCfg100VoltsExtended(void);
Boolean boNiDmmCfg1Volts(void);
Boolean boNiDmmCfg1VoltsExtended(void);
Boolean boNiDmmCfg10VoltsMulti(void);
Boolean boNiDmmCfg100VoltsMulti(void);
Boolean boNiDmmMeas(double *pdResult);
Boolean boNiDmmMeasSlow(double *pdResult);
Boolean boNiDmmMeasAverage(double *pdResult);
Boolean boNiDmmClose(void);


#endif
