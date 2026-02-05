//==============================================================================
//
// Title:       motfile.h
// Purpose:     A short description of the interface.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __MOTFILE_H__
#define __MOTFILE_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"

// Datentypen			
typedef struct SMotLine
{
	unsigned char ucSType;
	unsigned long ulAddress;
	unsigned char ucDLC;
	unsigned char * aucData;
} TMotLine;
			
typedef struct SMotFileElement
{
	TMotLine * ptData;
	struct SMotFileElement * ptNext;
} TMotFileElement;

typedef struct SMotFile
{
	TMotFileElement * ptFirst;
} TMotFile;
			
			


// Funktionen
extern TMotFile * ptLoadMotFile(const char * szFilename);
extern void FreeMotFile(TMotFile * ptFile);
extern int boSaveMotFile(TMotFile * ptFile, const char * szFilename);

extern int boReadFromAddress(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, unsigned char * aucDataBuffer);
extern int boWriteToAddress(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, const unsigned char * aucDataBuffer);

extern int boGetMotFileAddressBoundaries(TMotFile * ptFile, unsigned long * pulLowestAddress, unsigned long * pulHighestAddress, unsigned long * pulDataCount);
extern int boReadFromAddressFillEmpty(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, unsigned char * aucDataBuffer, unsigned char ucFillByte);
extern int boReadFromAddressOmitEmpty(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, unsigned char * aucDataBuffer, unsigned int * puRealByteCount);			

extern int boFindSequence(TMotFile * ptFile, unsigned char * aucSearchSequence, unsigned long ulSequenceLength, unsigned long ulSearchStartAddress, unsigned long ulSearchEndAddress, unsigned long * pulFirstMatchAddress);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __MOTFILE_H__ */
