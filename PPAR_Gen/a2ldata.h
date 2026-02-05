//==============================================================================
//
// Title:       a2ldata.h
// Purpose:     A short description of the interface.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __a2ldata_H__
#define __a2ldata_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"
#include "ModuleManagement.h"
#include "genericfile.h"

// Datentypen			
typedef struct SA2LBlock
{
	TGenericFile * ptFile;
	char * szBlockTag;
	char * szBlockName;
	char * szBlockComment;
} TA2LBlock;
			
typedef struct SA2LFile
{
	TGenericFile * ptFile;
	int boFileIsValid;
	TA2LBlock * ptRootBlock;
} TA2LFile;
			
			

extern void INIT_A2LData(void);

// Funktionen
extern TA2LFile * ptLoadA2LFile(const char * szFilename);
extern void FreeA2LFile(TA2LFile * ptFile);

extern TA2LBlock * ptFindFirstInfoBlock(TA2LBlock * ptSuperblock, const char * szBlockTag, const char * szBlockName);
extern void FreeA2LBlock(TA2LBlock * ptBlock);

extern int boGetParameter(TA2LBlock * ptBlock, const char * szParamName, unsigned int uParamNumber, int iBufferSize, char * szBuffer);
extern double dExtractCommentParam(const char * szComment, const char * szParamName);

E_MODULE(A2LData);
			
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __a2ldata_H__ */
