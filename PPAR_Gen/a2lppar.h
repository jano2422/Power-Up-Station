//==============================================================================
//
// Title:       a2lppar.h
// Purpose:     A short description of the interface.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __a2lppar_H__
#define __a2lppar_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"
#include "ModuleManagement.h"

// Datentypen			
			
			

extern void INIT_A2LPPAR(void);

// Funktionen
extern int boStartPParGeneration(const char * szA2LFile, const char * szTemplateMotFile);
extern int boFinishPParGeneration(const char * szTargetMotFile);
extern int boWriteValues(const char * szIdentifier, const void * pValue, unsigned int uSizeOfElement, int boValuesAreSigned, int boValuesAreFloat, unsigned int uNumOfElements);

extern int boCalculatePPARChecksum(const char * szCRCStartAddressIdentifier, const char * szCRCEndAddressIdentifier, const char * szCRCPolynomIdentifier, const char * szTargetAddressIdentifier);

E_MODULE(A2LPPAR);
			
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __a2lppar_H__ */
