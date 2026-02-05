//==============================================================================
//
// Title:       genericfile.h
// Purpose:     A short description of the interface.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __GENERICFILE_H__
#define __GENERICFILE_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"

// Datentypen			
typedef struct SGenericFile
{
	void * pData;
	unsigned long ulFileSize;
	unsigned long ulPosition;
	int boDataLink;
} TGenericFile;


			
			

// Funktionen

extern TGenericFile * ptLoadFile(const char * szFilename);
extern TGenericFile * ptExtractFile(TGenericFile * ptFile, unsigned long ulStartPosition, unsigned long ulLength);
extern void FreeFile(TGenericFile * ptFile);

extern int boRewindFile(TGenericFile * ptFile);
extern int boSeekFile(TGenericFile * ptFile, unsigned long ulAbsolutePosition);
extern int boSeekFileRelative(TGenericFile * ptFile, long lRelativePosition);

extern int boAtEOF(TGenericFile * ptFile);

extern int boReadLine(TGenericFile * ptFile, int iBufferSize, char * szBuffer, int boMovePosition);
extern int boReadString(TGenericFile * ptFile, int iBufferSize, char * szBuffer, int boMovePosition);
extern int boReadQuotedString(TGenericFile * ptFile, int iBufferSize, char * szBuffer, int boMovePosition, char * szStringEndSigns, char cSequenceStart, char cSequenceEnd, char cEscape);

extern int boFindNextString(TGenericFile * ptFile, const char * szStringToFind);
extern int boFindPreviousString(TGenericFile * ptFile, const char * szStringToFind);

extern int boGoToThisLineStart(TGenericFile * ptFile);
extern int boGoToNextLineStart(TGenericFile * ptFile);
extern int boGoToNextWordStart(TGenericFile * ptFile);
extern int boGoToThisLinesNextWordStart(TGenericFile * ptFile);
extern int boGoToFollowingWordStart(TGenericFile * ptFile);


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __GENERICFILE_H__ */
