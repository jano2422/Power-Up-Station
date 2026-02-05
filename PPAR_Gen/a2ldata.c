//==============================================================================
//
// Title:       a2ldata.c
// Purpose:     A short description of the implementation.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================
/*
	V1.00: Initialversion
*/

/*
	Programmschnittstelle zum Parsen von A2L-Dateien
*/

#include "toolbox.h"
#include <ansi_c.h>
#include "a2ldata.h"


#define TAG_BUFFER_SIZE 512
#define NAME_BUFFER_SIZE 1024
#define COMMENT_BUFFER_SIZE 1024

// Static vars

static int boInitialized = 0;



V_MODULE(A2LData, 1, 0, -4, 0, INIT_A2LData);


// SRC
TA2LFile * ptLoadA2LFile(const char * szFilename)
{
	TA2LFile * ptFile = NULL;
	char szTag[TAG_BUFFER_SIZE];
	char szName[NAME_BUFFER_SIZE];
	char szComment[COMMENT_BUFFER_SIZE];
	char szRootBlockEnd[TAG_BUFFER_SIZE + 5];
	
	unsigned long ulRootBlockStart = 0;
	unsigned long ulRootBlockEnd = 0;
	
	memset(szTag, 0, sizeof(szTag));
	memset(szName, 0, sizeof(szName));
	memset(szComment, 0, sizeof(szComment));
	
	ptFile = (TA2LFile *)malloc(sizeof(TA2LFile));
	if (ptFile)
	{
		ptFile->ptFile = ptLoadFile(szFilename);
		if (ptFile->ptFile)
		{ // Check Validity
			ptFile->ptRootBlock = NULL;
			ptFile->boFileIsValid = 0;
			
			if (!boRewindFile(ptFile->ptFile)) return ptFile;
			
			if (boReadLine(ptFile->ptFile, sizeof(szComment), szComment, 0))
			{
				if (strcmp(szComment, "ASAP2_VERSION 1 40") == 0)
				{
					ptFile->boFileIsValid = 1;
				}
			}
			
			if (boFindNextString(ptFile->ptFile, "/begin"))
			{
				if (boGoToThisLinesNextWordStart(ptFile->ptFile))
				{
					if (boReadString(ptFile->ptFile, sizeof(szTag), szTag, 1))
					{
						if (boGoToThisLinesNextWordStart(ptFile->ptFile))
						{
							if (boReadString(ptFile->ptFile, sizeof(szName), szName, 1))
							{
								if (boGoToThisLinesNextWordStart(ptFile->ptFile))
								{
									if (boReadQuotedString(ptFile->ptFile, sizeof(szComment), szComment, 1, " \t\r\n", '"', '"', '\\'))
									{ // Not mandatory!
									}
									if (boGoToNextLineStart(ptFile->ptFile))
									{
										ulRootBlockStart = ptFile->ptFile->ulPosition;
									}
								}
							}
						}
					}
				}
			}
			if (ulRootBlockStart)
			{
				sprintf(szRootBlockEnd, "/end %s", szTag);
				if (boFindNextString(ptFile->ptFile, szRootBlockEnd))
				{
					if (boGoToThisLineStart(ptFile->ptFile))
					{
						ulRootBlockEnd = ptFile->ptFile->ulPosition;
						
						ptFile->ptRootBlock = (TA2LBlock *)malloc(sizeof(TA2LBlock));
						if (ptFile->ptRootBlock)
						{
							memset(ptFile->ptRootBlock, 0, sizeof(TA2LBlock));
						
							ptFile->ptRootBlock->ptFile = ptExtractFile(ptFile->ptFile, ulRootBlockStart, ulRootBlockEnd - ulRootBlockStart);
							if (ptFile->ptRootBlock->ptFile)
							{
							
								ptFile->ptRootBlock->szBlockTag = (char *)malloc(strlen(szTag) + 1);
								if (ptFile->ptRootBlock->szBlockTag)
								{
									strcpy(ptFile->ptRootBlock->szBlockTag, szTag);
								}
							
								ptFile->ptRootBlock->szBlockName = (char *)malloc(strlen(szName) + 1);
								if (ptFile->ptRootBlock->szBlockName)
								{
									strcpy(ptFile->ptRootBlock->szBlockName, szName);
								}
							
								ptFile->ptRootBlock->szBlockComment = (char *)malloc(strlen(szComment) + 1);
								if (ptFile->ptRootBlock->szBlockComment)
								{
									strcpy(ptFile->ptRootBlock->szBlockComment, szComment);
								}
							}
							else
							{
								free(ptFile->ptRootBlock);
							}
						}
					}
				}
			}
		}
		else
		{
			free(ptFile);
			ptFile = NULL;
		}
	}
	
	return ptFile;
}


void FreeA2LBlock(TA2LBlock * ptBlock)
{
	if (ptBlock)
	{
		if (ptBlock->szBlockTag)
		{
			free(ptBlock->szBlockTag);
			ptBlock->szBlockTag = NULL;
		}
		if (ptBlock->szBlockName)
		{
			free(ptBlock->szBlockName);
			ptBlock->szBlockName = NULL;
		}
		if (ptBlock->szBlockComment)
		{
			free(ptBlock->szBlockComment);
			ptBlock->szBlockComment = NULL;
		}
		FreeFile(ptBlock->ptFile);
		free(ptBlock);
	}
}


void FreeA2LFile(TA2LFile * ptFile)
{
	if (ptFile)
	{
		ptFile->boFileIsValid = 0;
		FreeA2LBlock(ptFile->ptRootBlock);
		FreeFile(ptFile->ptFile);
		ptFile->ptFile = NULL;
		free(ptFile);
	}
}


TA2LBlock * ptFindFirstInfoBlock(TA2LBlock * ptSuperblock, const char * szBlockTag, const char * szBlockName)
{
	TA2LBlock * ptBlock = NULL;
	char szSearchTarget[TAG_BUFFER_SIZE + NAME_BUFFER_SIZE + 8];
	char szBuffer[COMMENT_BUFFER_SIZE];
	unsigned long ulSubBlockStart = 0;
	unsigned long ulSubBlockEnd = 0;
	
	boRewindFile(ptSuperblock->ptFile);
	
	sprintf(szSearchTarget, "/begin %s %s", szBlockTag, szBlockName); // Annahme: Separator = ' '!
	
	memset(szBuffer, 0, sizeof(szBuffer));
	
	while (!ptBlock && boFindNextString(ptSuperblock->ptFile, szSearchTarget))
	{ // Jetzt m¸ssen wir das nur noch verifizieren, ansonsten wird weitergesucht
		// Das ist nicht 100% safe: Im mehrzeiligen Kommentar d¸rfte in einer Zeile das /begin mit drinstehen,
		// das kˆnnte den ganzen Parser killen - also darf der Standard nicht voll ausgenutzt werden!
		if (boGoToThisLineStart(ptSuperblock->ptFile))
		{
			if (boGoToFollowingWordStart(ptSuperblock->ptFile))
			{ // /begin
				if (boReadString(ptSuperblock->ptFile, sizeof(szBuffer), szBuffer, 1))
				{
					if (strcmp(szBuffer, "/begin") == 0)
					{
						if (boGoToThisLinesNextWordStart(ptSuperblock->ptFile))
						{ // TAG
							if (boReadString(ptSuperblock->ptFile, sizeof(szBuffer), szBuffer, 1))
							{
								if (strcmp(szBuffer, szBlockTag) == 0)
								{
									if (boGoToThisLinesNextWordStart(ptSuperblock->ptFile))
									{ // NAME
										if (boReadString(ptSuperblock->ptFile, sizeof(szBuffer), szBuffer, 1))
										{
											if (strcmp(szBuffer, szBlockName) == 0)
											{
												strcpy(szBuffer, ""); // Damit der Buffer leer ist, wenn kein Comment vorhanden ist!
												if (boGoToThisLinesNextWordStart(ptSuperblock->ptFile))
												{ // COMMENT
													if (boReadQuotedString(ptSuperblock->ptFile, sizeof(szBuffer), szBuffer, 1, " \t\r\n", '"', '"', '\\'))
													{ // Not Mandatory
													}
												}
												if (boGoToNextLineStart(ptSuperblock->ptFile))
												{
													ulSubBlockStart = ptSuperblock->ptFile->ulPosition;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (ulSubBlockStart)
		{
			sprintf(szSearchTarget, "/end %s", szBlockTag);
			if (boFindNextString(ptSuperblock->ptFile, szSearchTarget))
			{
				if (boGoToThisLineStart(ptSuperblock->ptFile))
				{
					ulSubBlockEnd = ptSuperblock->ptFile->ulPosition;
					ptBlock = (TA2LBlock *)malloc(sizeof(TA2LBlock));
					if (ptBlock)
					{
						memset(ptBlock, 0, sizeof(TA2LBlock));
						ptBlock->ptFile = ptExtractFile(ptSuperblock->ptFile, ulSubBlockStart, ulSubBlockEnd - ulSubBlockStart);
						if (ptBlock->ptFile)
						{
						
							ptBlock->szBlockTag = (char *)malloc(strlen(szBlockTag) + 1);
							if (ptBlock->szBlockTag)
							{
								strcpy(ptBlock->szBlockTag, szBlockTag);
							}
						
							ptBlock->szBlockName = (char *)malloc(strlen(szBlockName) + 1);
							if (ptBlock->szBlockName)
							{
								strcpy(ptBlock->szBlockName, szBlockName);
							}
						
							ptBlock->szBlockComment = (char *)malloc(strlen(szBuffer) + 1);
							if (ptBlock->szBlockComment)
							{
								strcpy(ptBlock->szBlockComment, szBuffer);
							}
						}
						else
						{
							free(ptBlock);
							ptBlock = NULL;
						}
					}
				}
			}
		}
	}
	
	
	return ptBlock;
}


int boGetParameter(TA2LBlock * ptBlock, const char * szParamName, unsigned int uParamNumber, int iBufferSize, char * szBuffer)
{ // uParamNumber = 0: Restliche Zeile; uParamNumber = 1: erster Parameter; ...
	char szParamRead[TAG_BUFFER_SIZE];
	char szFindTarget[TAG_BUFFER_SIZE + 5];
	unsigned int uCount = 0;
	
	memset(szParamRead, 0, sizeof(szParamRead));
	memset(szBuffer, 0, iBufferSize);
	
	if (!ptBlock) return 0;
	
	if (!boRewindFile(ptBlock->ptFile)) return 0;
	
	while (boGoToFollowingWordStart(ptBlock->ptFile) && !boAtEOF(ptBlock->ptFile))
	{
		if (boReadString(ptBlock->ptFile, sizeof(szParamRead), szParamRead, 1))
		{ // Es gibt Daten
			if (strcmp(szParamRead, szParamName) == 0)
			{ // Es ist der richtige Parameter - JUHUU!
				if (uParamNumber == 0)
				{
					if (!boGoToThisLinesNextWordStart(ptBlock->ptFile)) return 0;
					if (!boReadLine(ptBlock->ptFile, iBufferSize, szBuffer, 0)) return 0;
					return 1; // Wir haben was gelesen!
				}
				else
				{
					for (uCount = 0; uCount < uParamNumber; uCount++)
					{ // Spulen bis zum angesagten Parameter
						if (!boGoToThisLinesNextWordStart(ptBlock->ptFile)) return 0;
					}
					if (!boReadQuotedString(ptBlock->ptFile, iBufferSize, szBuffer, 0, " \t\r\n", '"', '"', '\\')) return 0;
					return 1; // Wir haben was gelesen!
				}
				
			}
			else if (strcmp(szParamRead, "/begin") == 0)
			{ // Sub-Block, ¸berspringen!
				if (!boGoToThisLinesNextWordStart(ptBlock->ptFile)) return 0;
				if (!boReadString(ptBlock->ptFile, sizeof(szParamRead), szParamRead, 1)) return 0;
				sprintf(szFindTarget, "/end %s", szParamRead);
				if (!boFindNextString(ptBlock->ptFile, szFindTarget)) return 0;
				if (!boGoToNextLineStart(ptBlock->ptFile)) return 0;
			}
			else
			{ // Weitersuchen
				if (!boGoToNextLineStart(ptBlock->ptFile)) return 0;
			}
		}
		else
		{ // Notbremse
			return 0;
		}
	}
	return 0;
}


const char * szStrIFnd(const char * szHaystack, const char * szNeedle)
{
	int iPos = 0;
	int iStrlenH = 0;
	int iStrlenN = 0;
	
	iStrlenH = strlen(szHaystack);
	iStrlenN = strlen(szNeedle);
	for (iPos = 0; iPos < iStrlenH - iStrlenN; iPos++)
	{
		if (strnicmp(szHaystack + iPos, szNeedle, iStrlenN) == 0)
		{
			return szHaystack + iPos;
		}
	}
	return NULL;
}


double dExtractCommentParam(const char * szComment, const char * szParamName)
{
	const char * szPos = NULL;
	double dReturn = 1.0; // Defaultwert
	
	// "scaling" suchen
	szPos = szStrIFnd(szComment, szParamName);
	if (!szPos) return dReturn; // Heiﬂt: Nicht gefunden!
	// Ans Ende von "scaling"
	szPos += strlen(szParamName);
	// Doppelpunkt finden
	while (*szPos != ':')
	{
		if (*szPos == '\0' || *szPos == '\n') return dReturn; // Heiﬂt: Nicht gefunden!
		szPos++;
	}
	// Nach dem Doppelpunkt - Ziffer suchen
	while (*szPos < '0' || *szPos > '9')
	{
		if (*szPos == '\0' || *szPos == '\n') return dReturn; // Heiﬂt: Nicht gefunden!
		szPos++;
	}
	
	dReturn = atof(szPos);
	
	return dReturn;
}



void INIT_A2LData(void)
{
	if (boInitialized) return;
	boInitialized = 1;
	
	
}

