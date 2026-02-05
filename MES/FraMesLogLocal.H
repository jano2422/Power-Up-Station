/*****************************************************************************/
/*                                                                           */
/*  Header : FraMesLogLocal.H                                                */
/*                                                                           */
/*****************************************************************************
	-	If you define the following makro then the native ShowError-Window from FraMes
		is displayed too:
		#define _SHOW_ERROR_WIN_
	-	If you are using EvaProd, Sequencer, SerGen or CraMa then you have to define
		the macros _USE_EVA_, _USE_SEQUENCER_, _USE_SERGEN_ or _USE_CARMA_ respectively

/*****************************************************************************/
#if !defined ( __FRA_MES_LOG_LOCAL_H )
#define __FRA_MES_LOG_LOCAL_H

/*****************************************************************************/
#ifdef _CVI_DEBUG_
	// On Offline-Equipment it is better to show Error-Message in the Popup
//	#define _SHOW_ERROR_WIN_ 
#endif

//#define _USE_EVA_
//#define _USE_SEQUENCER_
//#define _USE_SERGEN_
//#define _USE_CARMA_

/*****************************************************************************/
#endif
