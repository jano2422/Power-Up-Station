//==============================================================================
//
// Title:       res2xml.h
// Purpose:     A short description of the interface.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __res2xml_H__
#define __res2xml_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"
#include "ModuleManagement.h"
#include "genericfile.h"

// Datentypen			
			

// Funktionen
extern int boRes2XML(const char * szResultFile, const char * szTargetXMLFile);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __res2xml_H__ */
