//==============================================================================
//
// Title:       Control.PXI6528.h
// Purpose:     A short description of the interface.
//
// Created on:  7/28/2009 at 3:17:08 PM by Test Engineering.
// Copyright:   CTEPI. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants



#define niNetyDeg	0
#define	zerOdeg		1

#define READY		1
#define notREADY	0
		
#define PASS		'P' 
#define FAIL		'F'
		
#define	STATE_ON	1
#define STATE_OFF	0

int resetIndicator (int iActBay);
int showResult (int iActBay, int Result);



