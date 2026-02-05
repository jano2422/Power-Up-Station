//==============================================================================
//
// Title:       specialFunction.h
// Purpose:     A short description of the interface.
//
// Created on:  8/27/2009 at 12:57:33 PM by Harold Anthony Olympia.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __specialFunction_H__
#define __specialFunction_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

#define	emptyBarcode	0
#define	validBarcode	4
#define	noECU			2
#define	TESTING			3
#define	LOADING_PLS		5
#define	TIMESFAIL3		6
#define	UNITLOCKED		7		

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions

//int Declare_Your_Functions_Here (int x);
int status_init (void);
int setStatus (int Action);
int AndonPanel;
//int Get_cFg (void);



#ifdef __cplusplus
    }
#endif

#endif  /* ndef __specialFunction_H__ */
