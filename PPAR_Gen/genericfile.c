//==============================================================================
//
// Title:       genericfile.c
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
	Programmschnittstelle zum Erfassen von Dateien im Memory
*/

#include "toolbox.h"
#include <ansi_c.h>
#include "genericfile.h"



// Static vars



// SRC
TGenericFile * ptLoadFile(const char * szFilename)
{
	TGenericFile * ptFile = NULL;
	void * pData = NULL;
	unsigned long ulFileSize = 0;
	unsigned long ulLength = 0;
	FILE * file = NULL;
	int iFileSize;
	
	if (FileExists(szFilename, &iFileSize))
	{
		ulFileSize = iFileSize; // warning beseitigen
		pData = malloc(ulFileSize);
		if (pData)
		{
			file = fopen(szFilename, "rb");
			if (file)
			{
				ulLength = fread(pData, ulFileSize, 1, file);
				if (ulLength == 1) // Anzahl der Elemente
				{
					ptFile = (TGenericFile *)malloc(sizeof(TGenericFile));
					if (ptFile)
					{
						ptFile->pData = pData;
						ptFile->ulFileSize = ulFileSize;
						ptFile->ulPosition = 0;
						ptFile->boDataLink = 0;
					}
					else
					{
						free(pData);
					}
				}
				else
				{
					free(pData);
				}
				fclose(file);
			}
			else
			{
				free(pData);
			}
		}
	}
	
	return ptFile;
}


TGenericFile * ptExtractFile(TGenericFile * ptFile, unsigned long ulStartPosition, unsigned long ulLength)
{
	TGenericFile * ptExtract = NULL;
	
	if (!ptFile) return NULL;
	if (ulStartPosition + ulLength > ptFile->ulFileSize) return NULL;
	
	ptExtract = (TGenericFile *)malloc(sizeof(TGenericFile));
	if (ptExtract)
	{
		ptExtract->pData = (void *)((char *)ptFile->pData + ulStartPosition);
		ptExtract->ulFileSize = ulLength;
		ptExtract->ulPosition = 0;
		ptExtract->boDataLink = 1;
	}
	
	return ptExtract;
}


void FreeFile(TGenericFile * ptFile)
{
	if (ptFile)
	{
		if (ptFile->pData && !ptFile->boDataLink) free(ptFile->pData);
		free(ptFile);
	}
}


int boRewindFile(TGenericFile * ptFile)
{
	if (!ptFile) return 0;
	ptFile->ulPosition = 0;
	return 1;
}

int boSeekFile(TGenericFile * ptFile, unsigned long ulAbsolutePosition)
{
	if (!ptFile) return 0;
	if (ulAbsolutePosition > ptFile->ulFileSize) return 0;
	ptFile->ulPosition = ulAbsolutePosition;
	return 1;
}

int boSeekFileRelative(TGenericFile * ptFile, long lRelativePosition)
{
	if (!ptFile) return 0;
	if ((unsigned long)(ptFile->ulPosition + lRelativePosition) > ptFile->ulFileSize) return 0;
	if ((lRelativePosition < 0) && ((unsigned long)(-lRelativePosition) > ptFile->ulPosition)) return 0;
	ptFile->ulPosition += lRelativePosition;
	return 1;
}

int boAtEOF(TGenericFile * ptFile)
{
	if (!ptFile) return 1; // Kein File = Permanent EOF
	if (ptFile->ulPosition == ptFile->ulFileSize) return 1;
	return 0;
}

int boIsLineEndChar(char c)
{
	return (c == '\0' || c == '\n' || c == '\r');
}

int boReadLine(TGenericFile * ptFile, int iBufferSize, char * szBuffer, int boMovePosition)
{
	int ulStartPos = 0;
	int ulLength = 0;
	int ulPos = 0;
	
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	if (iBufferSize == 0) return 0; // könnte underflow-Probleme verursachen
	if (!szBuffer) return 0;
	
	ulStartPos = ptFile->ulPosition;
	szDataAsStr = (char *)(ptFile->pData);
	
	for (ulPos = ulStartPos; ulPos < ptFile->ulFileSize && ulPos < ulStartPos + (iBufferSize - 1) && !boIsLineEndChar(szDataAsStr[ulPos]); ulPos++)
	{
		// Narp?
	} 
	ulLength = ulPos - ulStartPos;
	memset(szBuffer, 0, iBufferSize);
	memcpy(szBuffer, szDataAsStr + ulStartPos, ulLength);

	if (boMovePosition)
	{
		ptFile->ulPosition = ulPos;
	}
	
	return 1;
}


int boFindNextString(TGenericFile * ptFile, const char * szStringToFind)
{
	int ulPos = 0;
	int iInStrPos = 0;
	int iStrlen = 0;
	
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	if (!szStringToFind) return 0;
	if (!*szStringToFind) return 0;
	
	ulPos = ptFile->ulPosition;
	szDataAsStr = (char *)(ptFile->pData);
	iStrlen = strlen(szStringToFind);
	
	for (ulPos = ptFile->ulPosition; ulPos + iStrlen < ptFile->ulFileSize; ulPos++)
	{
		if (szDataAsStr[ulPos] == szStringToFind[0])
		{
			for (iInStrPos = 0; iInStrPos < iStrlen; iInStrPos++)
			{
				if (szDataAsStr[ulPos + iInStrPos] != szStringToFind[iInStrPos]) break;
			}
			if (iInStrPos == iStrlen)
			{ // Alles hat gepasst
				ptFile->ulPosition = ulPos;
				return 1;
			}
		}
	}
	
	// Nicht gefunden
	return 0;
}

int boFindPreviousString(TGenericFile * ptFile, const char * szStringToFind)
{
	int ulPos = 0;
	int iInStrPos = 0;
	int iStrlen = 0;
	
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	if (!szStringToFind) return 0;
	if (!*szStringToFind) return 0;
	
	ulPos = ptFile->ulPosition;
	szDataAsStr = (char *)(ptFile->pData);
	iStrlen = strlen(szStringToFind);
	
	if (iStrlen > ptFile->ulFileSize) return 0;
	if (ulPos + iStrlen > ptFile->ulFileSize)
	{ // Repositionieren!
		ulPos = ptFile->ulFileSize - iStrlen + 1;
	}
	
	for (ulPos = ulPos; ulPos > 0; ulPos--)
	{
		if (szDataAsStr[ulPos - 1] == szStringToFind[0])
		{
			for (iInStrPos = 0; iInStrPos < iStrlen; iInStrPos++)
			{
				if (szDataAsStr[ulPos - 1 + iInStrPos] != szStringToFind[iInStrPos]) break;
			}
			if (iInStrPos == iStrlen)
			{ // Alles hat gepasst
				ptFile->ulPosition = ulPos - 1;
				return 1;
			}
		}
	}
	
	// Nicht gefunden
	return 0;
}

int boGoToThisLineStart(TGenericFile * ptFile)
{
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	
	szDataAsStr = (char *)(ptFile->pData);
	
	while (ptFile->ulPosition > 0 && szDataAsStr[ptFile->ulPosition - 1] != '\n') // Ist gar nicht so einfach mit unsigned!
	{
		ptFile->ulPosition--;
	}
	
	return 1; // Zeilenanfang ist leicht, im Zweifelsfall ist der Dateibeginn der Zeilenbeginn
}

int boGoToNextLineStart(TGenericFile * ptFile)
{
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	
	szDataAsStr = (char *)(ptFile->pData);
	
	while (ptFile->ulPosition < ptFile->ulFileSize)
	{
		ptFile->ulPosition++;
		if (szDataAsStr[ptFile->ulPosition - 1] == '\n')
		{
			return 1;
		}
	}
	
	return 0; // Keine neue Zeile mehr! Aber gespult wurde noch!
}

int boGoToNextWordStart(TGenericFile * ptFile)
{
	unsigned char * szDataAsStr = NULL; // Wir haben hier Bereiche...
	
	if (!ptFile) return 0;
	
	szDataAsStr = (unsigned char *)(ptFile->pData);
	
	while (ptFile->ulPosition + 1 < ptFile->ulFileSize)
	{
		ptFile->ulPosition++;
		if (szDataAsStr[ptFile->ulPosition - 1] <= ' ' && szDataAsStr[ptFile->ulPosition] > ' ')
		{
			return 1;
		}
	}
	
	return 0; // Keine neues Wort mehr! Aber gespult wurde noch!
}

int boGoToFollowingWordStart(TGenericFile * ptFile)
{
	unsigned char * szDataAsStr = NULL; // Wir haben hier Bereiche...
	
	if (!ptFile) return 0;
	
	szDataAsStr = (unsigned char *)(ptFile->pData);
	
	if (ptFile->ulPosition == 0 && szDataAsStr[0] > ' ')
	{ // Sonderfall: Dateianfang
		return 1;
	}
	
	while (ptFile->ulPosition < ptFile->ulFileSize)
	{
		if (szDataAsStr[ptFile->ulPosition - 1] <= ' ' && szDataAsStr[ptFile->ulPosition] > ' ')
		{
			return 1;
		}
		ptFile->ulPosition++;
	}
	
	return 0; // Keine neues Wort mehr! Aber gespult wurde noch!
}

int boGoToThisLinesNextWordStart(TGenericFile * ptFile)
{
	unsigned char * szDataAsStr = NULL; // Wir haben hier Bereiche...
	
	if (!ptFile) return 0;
	
	szDataAsStr = (unsigned char *)(ptFile->pData);
	
	while (ptFile->ulPosition + 1 < ptFile->ulFileSize && szDataAsStr[ptFile->ulPosition] != '\n' && szDataAsStr[ptFile->ulPosition] != '\r')
	{
		ptFile->ulPosition++;
		if (szDataAsStr[ptFile->ulPosition - 1] <= ' ' && szDataAsStr[ptFile->ulPosition] > ' ')
		{
			return 1;
		}
	}
	
	return 0; // Keine neues Wort mehr! Aber gespult wurde noch!
}


int boReadString(TGenericFile * ptFile, int iBufferSize, char * szBuffer, int boMovePosition)
{ 
	int ulStartPos = 0;
	int ulIntPos = 0;
	int ulPos = 0;
	
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	if (iBufferSize == 0) return 0; // könnte underflow-Probleme verursachen
	if (!szBuffer) return 0;
	
	ulStartPos = ptFile->ulPosition;
	szDataAsStr = (char *)(ptFile->pData);
	ulPos = ulStartPos;
	
	memset(szBuffer, 0, iBufferSize);
	
	
	while (ulPos < ptFile->ulFileSize && ulIntPos < (iBufferSize - 1) && szDataAsStr[ulPos] > ' ')
	{
		szBuffer[ulIntPos] = szDataAsStr[ulPos];
		ulIntPos++;
		ulPos++;
	} 
	
	if (boMovePosition)
	{
		ptFile->ulPosition = ulPos;
	}
	
	return 1;
}


int boReadQuotedString(TGenericFile * ptFile, int iBufferSize, char * szBuffer, int boMovePosition, char * szStringEndSigns, char cSequenceStart, char cSequenceEnd, char cEscape)
{ // Escapte szStringEndSigns zählen nicht, ebenso innerhalb des Quotes; Quotesigns werden nicht mitgenommen; Escape-Symbole escapen Spezialzeichen!
	int boInSequence = 0;
	int boIsEscape = 0;
	int ulStartPos = 0;
	int ulIntPos = 0;
	int ulPos = 0;
	
	char * szDataAsStr = NULL;
	
	if (!ptFile) return 0;
	if (iBufferSize == 0) return 0; // könnte underflow-Probleme verursachen
	if (!szBuffer) return 0;
	
	ulStartPos = ptFile->ulPosition;
	szDataAsStr = (char *)(ptFile->pData);
	ulPos = ulStartPos;
	
	memset(szBuffer, 0, iBufferSize);
	
	
	while (ulPos < ptFile->ulFileSize && ulIntPos < (iBufferSize - 1))
	{
		if (boIsEscape)
		{ // Gilt auch für escapte escapes
			szBuffer[ulIntPos] = szDataAsStr[ulPos];
			ulIntPos++;
			boIsEscape = 0;
		}
		else
		{
			if (szDataAsStr[ulPos] == cEscape)
			{
				boIsEscape = 1;
			}
			else
			{
				if (boInSequence)
				{
					if (szDataAsStr[ulPos] == cSequenceEnd)
					{
						boInSequence = 0;
					}
					else
					{
						szBuffer[ulIntPos] = szDataAsStr[ulPos];
						ulIntPos++;
					}
				}
				else
				{
					if (szDataAsStr[ulPos] == cSequenceStart)
					{
						boInSequence = 1;
					}
					else if (strchr(szStringEndSigns, szDataAsStr[ulPos]))
					{
						break;
					}
					else
					{
						szBuffer[ulIntPos] = szDataAsStr[ulPos];
						ulIntPos++;
					}
				}
			}
		}
		
		ulPos++;
	} 
	
	if (boMovePosition)
	{
		ptFile->ulPosition = ulPos;
	}
	
	return 1;
}


