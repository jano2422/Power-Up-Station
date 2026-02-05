//==============================================================================
//
// Title:       motfile.c
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
	Programmschnittstelle zum Parsen von MOT-Dateien
*/

#include "toolbox.h"
#include <ansi_c.h>
#include "motfile.h"
#include "strlib.h"


#define LINE_BUFFER_SIZE 80

// Interne Datentypen
typedef struct SPartialMotData
{
	unsigned long ulStartAddress;
	TMotFileElement * ptElement;
	unsigned char * aucPartialData;
	unsigned char ucPartialDataLength;
	unsigned char ucElementDataOffset;
} TPartialMotData;



// Static vars



// SRC



int iAddressLengthOfSType(unsigned char ucSType)
{
	if (ucSType == 0 || ucSType == 5)
	{ // Konstantdatentypen
		return 2;
	}
	else if (ucSType == 1 || ucSType == 9)
	{
		return 2;
	}
	else if (ucSType == 2 || ucSType == 8)
	{
		return 3;
	}
	else if (ucSType == 3 || ucSType == 7)
	{
		return 4;
	}
	else
	{
		return 0;
	}
}


int iAddressBytes(const TMotLine * ptLine, unsigned char * aucAddressBytes)
{
	int iNum = iAddressLengthOfSType(ptLine->ucSType);
	int iCnt = 0;
	
	memset(aucAddressBytes, 0, iNum);
	for (iCnt = 0; iCnt < iNum; iCnt++)
	{
		aucAddressBytes[iCnt] = (ptLine->ulAddress >> ((iNum - iCnt - 1) * 8)) & 0xFF;
	}
	
	return iNum;
}


unsigned char ucLineChecksum(const TMotLine * ptLine)
{
	int iCnt = 0;
	unsigned char aucAddressBytes[4];
	int iAddressLength = 0;
	unsigned long ulChecksum = 0;
	
	iAddressLength = iAddressBytes(ptLine, aucAddressBytes);
	
	ulChecksum += iAddressLength + ptLine->ucDLC + 1;
	
	for (iCnt = 0; iCnt < iAddressLength; iCnt++)
	{
		ulChecksum += aucAddressBytes[iCnt];
	}
	
	for (iCnt = 0; iCnt < ptLine->ucDLC; iCnt++)
	{
		ulChecksum += ptLine->aucData[iCnt];
	}
	
	ulChecksum = ~ulChecksum;
	
	return ulChecksum & 0xFF;
}


TMotLine * ptParseLine(const char * szLine)
{
	TMotLine * ptMotLine = NULL;
	unsigned char ucLength = 0x00;
	int iAddressByteCount = 0;
	int iCnt = 0;
	
	if (!szLine) return ptMotLine;
	
	// S - 1 Char/Nibble
	if (*szLine != 'S') return ptMotLine;
	szLine++;
	
	// S-Type - 1 Nibble
	if (!*szLine) return ptMotLine;
	if (!*(szLine + 1)) return ptMotLine;
	if (!*(szLine + 2)) return ptMotLine;
	if (!*(szLine + 3)) return ptMotLine;
	if (!*(szLine + 4)) return ptMotLine;
	
	ptMotLine = (TMotLine *)malloc(sizeof(TMotLine));
	if (ptMotLine)
	{
		memset(ptMotLine, 0, sizeof(TMotLine));
	
		ptMotLine->ucSType = *szLine - '0';
		szLine++;
	
		// Längeninformation - 1 Byte
		ucLength = ucHexToChar(szLine);
		szLine += 2;
	
		// Adressdaten - Byteanzahl variabel
		iAddressByteCount = iAddressLengthOfSType(ptMotLine->ucSType);
	
		for (iCnt = 0; iCnt < iAddressByteCount; iCnt++)
		{
			if (!*szLine || !*(szLine + 1))
			{
				free(ptMotLine);
				ptMotLine = NULL;
				return ptMotLine;
			}
			ptMotLine->ulAddress = (ptMotLine->ulAddress << 8) | (ucHexToChar(szLine));
			szLine += 2;
		}
		
		// Daten - Byteanzahl variabel
		ptMotLine->ucDLC = ucLength - iAddressByteCount - 1;
		ptMotLine->aucData = (unsigned char *)malloc(ptMotLine->ucDLC);
		if (ptMotLine->aucData)
		{ // Ist NULL, wenn DLC == 0!
			memset(ptMotLine->aucData, 0, ptMotLine->ucDLC);
			
			for (iCnt = 0; iCnt < ptMotLine->ucDLC; iCnt++)
			{
				if (!*szLine || !*(szLine + 1))
				{
					free(ptMotLine->aucData);
					free(ptMotLine);
					ptMotLine = NULL;
					return ptMotLine;
				}
				ptMotLine->aucData[iCnt] = ucHexToChar(szLine);
				szLine += 2;
			}
		}
		else if (ptMotLine->ucDLC != 0)
		{ // Böse!
			free(ptMotLine);
			ptMotLine = NULL;
			return ptMotLine;
		}
			
		// Checksumme - 1 Byte (letztes)
		if (!*szLine || !*(szLine + 1))
		{
			if (ptMotLine->ucDLC) free(ptMotLine->aucData);
			free(ptMotLine);
			ptMotLine = NULL;
			return ptMotLine;
		}
		
		if (ucLineChecksum(ptMotLine) != ucHexToChar(szLine))
		{
			if (ptMotLine->ucDLC) free(ptMotLine->aucData);
			free(ptMotLine);
			ptMotLine = NULL;
			return ptMotLine;
		}
			
	}
	
	return ptMotLine;
}


void FreeMotLine(TMotLine * ptMotLine)
{
	if (ptMotLine)
	{
		if (ptMotLine->aucData)
		{
			free(ptMotLine->aucData);
			ptMotLine->aucData = NULL;
		}
		ptMotLine->ucDLC = 0;
		free(ptMotLine);
	}
}


TMotFileElement * ptInsertAfter(TMotFileElement * ptElement, TMotLine * ptLine)
{
	TMotFileElement * ptNewElement = NULL;
	
	if (!ptElement || !ptLine) return ptNewElement;
	
	ptNewElement = (TMotFileElement *)malloc(sizeof(TMotFileElement));
	if (ptNewElement)
	{
		memset(ptNewElement, 0, sizeof(TMotFileElement));
		ptNewElement->ptData = ptLine;
		ptNewElement->ptNext = ptElement->ptNext;
		ptElement->ptNext = ptNewElement;
	}
	
	return ptNewElement;
}

TMotFileElement * ptInsertFirst(TMotFile * ptFile, TMotLine * ptLine)
{
	TMotFileElement * ptNewElement = NULL;
	
	if (!ptFile || !ptLine) return ptNewElement;
	
	ptNewElement = (TMotFileElement *)malloc(sizeof(TMotFileElement));
	if (ptNewElement)
	{
		memset(ptNewElement, 0, sizeof(TMotFileElement));
		ptNewElement->ptData = ptLine;
		ptNewElement->ptNext = ptFile->ptFirst;
		ptFile->ptFirst = ptNewElement;
	}
	
	return ptNewElement;
}

TMotFileElement * ptAppend(TMotFile * ptFile, TMotLine * ptLine)
{
	TMotFileElement * ptNewElement = NULL;
	TMotFileElement * ptCurrent = NULL;
	
	if (!ptFile || !ptLine) return ptNewElement;
	
	ptNewElement = (TMotFileElement *)malloc(sizeof(TMotFileElement));
	if (ptNewElement)
	{
		memset(ptNewElement, 0, sizeof(TMotFileElement));
		ptNewElement->ptData = ptLine;
		ptNewElement->ptNext = NULL;
		if (ptFile->ptFirst)
		{ // Durchgehen bis zum Schluss
			ptCurrent = ptFile->ptFirst;
			while (ptCurrent->ptNext)
			{
				ptCurrent = ptCurrent->ptNext;
			}
			ptCurrent->ptNext = ptNewElement;
		}
		else
		{ // Ist das erste
			ptFile->ptFirst = ptNewElement;
		}
	}
	
	return ptNewElement;
}


int boLineToFile(FILE * fileMot, TMotLine * ptLine)
{
	int iCnt = 0;
	int iHeaderSize = 0;
	unsigned char ucLength = 0x00;
	unsigned char aucAddressBytes[4];
	
	if (!fileMot) return 0;
	if (!ptLine) return 0;
	
	iHeaderSize = iAddressBytes(ptLine, aucAddressBytes);
	ucLength = iHeaderSize + ptLine->ucDLC + 1;
	
	// S-Type + Length
	fprintf(fileMot, "S%1X%02X", ptLine->ucSType, ucLength);
	
	// Address
	for (iCnt = 0; iCnt < iHeaderSize; iCnt++)
	{
		fprintf(fileMot, "%02X", aucAddressBytes[iCnt]);
	}
	
	// Data
	for (iCnt = 0; iCnt < ptLine->ucDLC; iCnt++)
	{
		fprintf(fileMot, "%02X", ptLine->aucData[iCnt]);
	}
	
	// Checksumme
	fprintf(fileMot, "%02X\n", ucLineChecksum(ptLine));
	
	return 1;
}



int boMergeDataToLine(TPartialMotData * ptData)
{
	if (!ptData) return 0;
	if (!ptData->ptElement) return 0;
	if (ptData->ucPartialDataLength > ptData->ptElement->ptData->ucDLC) return 0;
	if (ptData->ucElementDataOffset > ptData->ptElement->ptData->ucDLC) return 0;
	if (ptData->ucPartialDataLength + ptData->ucElementDataOffset > ptData->ptElement->ptData->ucDLC) return 0; // Erst jetzt, um ggf. Überläufe erkennen zu können (unsigned char < 256!)
	if (!ptData->aucPartialData) return 0;
	
	memcpy(ptData->ptElement->ptData->aucData + ptData->ucElementDataOffset, ptData->aucPartialData, ptData->ucPartialDataLength);
	
	return 1;
}

int boGetDataFromLine(TPartialMotData * ptData)
{
	if (!ptData) return 0;
	if (!ptData->ptElement) return 0;
	if (ptData->ucPartialDataLength > ptData->ptElement->ptData->ucDLC) return 0;
	if (ptData->ucElementDataOffset > ptData->ptElement->ptData->ucDLC) return 0;
	if (ptData->ucPartialDataLength + ptData->ucElementDataOffset > ptData->ptElement->ptData->ucDLC) return 0; // Erst jetzt, um ggf. Überläufe erkennen zu können (unsigned char < 256!)
	if (!ptData->aucPartialData) return 0; // Muss vorhanden sein! Könnte zwar hier auch erzeugt werden, schafft aber dann kein Bewusstsein für die Freigabe des Arrays -> Gefahr von Speicherlecks!
	
	memcpy(ptData->aucPartialData, ptData->ptElement->ptData->aucData + ptData->ucElementDataOffset, ptData->ucPartialDataLength);
	
	return 1;
}

TMotFileElement * ptFindElementWithAddressInt(unsigned long ulAddress, TMotFileElement * ptStartElement, TMotFileElement * ptStopElement)
{
	while (ptStartElement != NULL && ptStartElement != ptStopElement)
	{
		if (ptStartElement->ptData->ucSType >= 1 && ptStartElement->ptData->ucSType <= 3 && ptStartElement->ptData->ulAddress <= ulAddress && ptStartElement->ptData->ulAddress + ptStartElement->ptData->ucDLC > ulAddress) return ptStartElement;
		ptStartElement = ptStartElement->ptNext;
	}
	return NULL;
}

TMotFileElement * ptFindElementWithNextAddressInt(unsigned long ulAddress, TMotFileElement * ptStartElement, TMotFileElement * ptStopElement)
{ // UPDATE
	TMotFileElement * ptBestElement = NULL;
	
	while (ptStartElement != NULL && ptStartElement != ptStopElement)
	{
		if (ptStartElement->ptData->ucSType >= 1 && ptStartElement->ptData->ucSType <= 3)
		{ // Nur Datenpakete
			if (ptStartElement->ptData->ulAddress <= ulAddress && ptStartElement->ptData->ulAddress + ptStartElement->ptData->ucDLC > ulAddress) return ptStartElement; // Direkter Treffer
			else if (ptStartElement->ptData->ulAddress > ulAddress && ptStartElement->ptData->ucDLC > 0)
			{
				if (ptBestElement)
				{
					if (ptBestElement->ptData->ulAddress > ptStartElement->ptData->ulAddress) ptBestElement = ptStartElement; // Immer den kleineren davon nehmen
				}
				else
				{ // Erster Versuch
					ptBestElement = ptStartElement;
				}
			}
		}
		ptStartElement = ptStartElement->ptNext;
	}
	return ptBestElement;
}

TMotFileElement * ptFindElementWithAddress(unsigned long ulAddress, TMotFile * ptFile, TMotFileElement * ptStartElement)
{
	TMotFileElement * ptResultElement = NULL;
	
	/*
		Die Suche startet zuerst beim StartElement bis zum Ende der Liste und fängt dann von vorne an bis zum StartElement.
		Verkürzt die Suchzeit für konsekutive Adressen!
	*/
	
	ptResultElement = ptFindElementWithAddressInt(ulAddress, ptStartElement, NULL);
	if (!ptResultElement)
	{
		if (ptFile && ptFile->ptFirst)
		{
			ptResultElement = ptFindElementWithAddressInt(ulAddress, ptFile->ptFirst, ptStartElement);
		}
	}
	
	return ptResultElement;
}

TMotFileElement * ptFindElementWithNextAddress(unsigned long ulAddress, TMotFile * ptFile, TMotFileElement * ptStartElement)
{ // UPDATE
	// Sucht das Element mit der nächsten Adresse
	TMotFileElement * ptResultElement = NULL;
	TMotFileElement * ptResultElement2 = NULL;
	
	/*
		Die Suche startet zuerst beim StartElement bis zum Ende der Liste und fängt dann von vorne an bis zum StartElement.
		Verkürzt die Suchzeit für konsekutive Adressen!
	*/
	
	ptResultElement = ptFindElementWithNextAddressInt(ulAddress, ptStartElement, NULL);
	if (!ptResultElement || ptResultElement->ptData->ulAddress != ulAddress)
	{
		if (ptFile && ptFile->ptFirst)
		{
			ptResultElement2 = ptFindElementWithNextAddressInt(ulAddress, ptFile->ptFirst, ptStartElement);
		}
	}
	
	// Return...?
	
	if (ptResultElement)
	{
		if (ptResultElement2)
		{
			if (ptResultElement->ptData->ulAddress < ptResultElement2->ptData->ulAddress) return ptResultElement;
			else return ptResultElement2;
		}
		
		return ptResultElement;
	}
	else
	{
		if (ptResultElement)
		{
			if (ptResultElement->ptData->ulAddress < ptResultElement2->ptData->ulAddress) return ptResultElement;
			else return ptResultElement2;
		}
		
		return ptResultElement2;
	}
	
}

TPartialMotData * ptCreateEmptyPartialMotData(unsigned long ulAddress, unsigned char ucMaxLength, const unsigned char * aucInitialData, TMotFile * ptFile, TPartialMotData * ptLastPartial)
{
	TPartialMotData * ptPartial = NULL;
	
	if (!ptFile) return ptPartial;
	ptPartial = (TPartialMotData *)malloc(sizeof(TPartialMotData));
	if (ptPartial)
	{
		memset(ptPartial, 0, sizeof(TPartialMotData));
		ptPartial->ulStartAddress = ulAddress;
		ptPartial->ptElement = ptFindElementWithAddress(ulAddress, ptFile, (ptLastPartial ? ptLastPartial->ptElement : NULL));
		if (ptPartial->ptElement)
		{
			ptPartial->ucElementDataOffset = (unsigned char)(ulAddress - ptPartial->ptElement->ptData->ulAddress);
			ptPartial->ucPartialDataLength = ptPartial->ptElement->ptData->ucDLC - ptPartial->ucElementDataOffset;
			if (ptPartial->ucPartialDataLength > ucMaxLength) ptPartial->ucPartialDataLength = ucMaxLength;
			ptPartial->aucPartialData = (unsigned char *)malloc(ptPartial->ucPartialDataLength);
			if (ptPartial->aucPartialData)
			{
				if (aucInitialData)
				{
					memcpy(ptPartial->aucPartialData, aucInitialData, ptPartial->ucPartialDataLength);
				}
				else
				{
					memset(ptPartial->aucPartialData, 0, ptPartial->ucPartialDataLength);
				}
			}
			else
			{
				free(ptPartial);
				ptPartial = NULL;
			}
		}
		else
		{
			free(ptPartial);
			ptPartial = NULL;
		}
	}
	
	return ptPartial;
}


void FreePartialMotData(TPartialMotData * ptPartial)
{
	if (ptPartial)
	{
		if (ptPartial->aucPartialData)
		{
			free(ptPartial->aucPartialData);
		}
		memset(ptPartial, 0, sizeof(TPartialMotData)); // Zur Sicherheit, falls noch irgendwo im Speicher verknüpft!
		free(ptPartial);
	}
}



// PUBLIC FUNCTIONS

TMotFile * ptLoadMotFile(const char * szFilename)
{
	FILE * fileMot = NULL;
	TMotFile * ptFile = NULL;
	TMotFileElement * ptElement = NULL;
	TMotLine * ptLine = NULL;
	char szLineBuffer[LINE_BUFFER_SIZE];
	
	memset(szLineBuffer, 0, sizeof(szLineBuffer));
	
	fileMot = fopen(szFilename, "r");
	if (fileMot)
	{
		ptFile = (TMotFile *)malloc(sizeof(TMotFile));
		memset(ptFile, 0, sizeof(TMotFile));
		if (ptFile)
		{
			while (!feof(fileMot))
			{
				if (!fgets(szLineBuffer, sizeof(szLineBuffer), fileMot)) break;
				ptLine = ptParseLine(szLineBuffer);
				if (ptLine)
				{
					if (!ptElement)
					{ // Erstes
						ptElement = ptAppend(ptFile, ptLine);
					}
					else
					{
						ptElement = ptInsertAfter(ptElement, ptLine);
					}
				}
			}
		}
		fclose(fileMot);
	}
	return ptFile;
}


void FreeMotFile(TMotFile * ptFile)
{
	TMotFileElement * ptCurrentElement = NULL;
	
	if (ptFile)
	{
		while (ptFile->ptFirst)
		{
			ptCurrentElement = ptFile->ptFirst;
			ptFile->ptFirst = ptCurrentElement->ptNext;
			FreeMotLine(ptCurrentElement->ptData);
			ptCurrentElement->ptData = NULL;
			ptCurrentElement->ptNext = NULL;
			free(ptCurrentElement);
		}
		free(ptFile);
	}
}


int boSaveMotFile(TMotFile * ptFile, const char * szFilename)
{
	FILE * fileMot = NULL;
	TMotFileElement * ptCurrentElement = NULL;
	
	if (!ptFile) return 0;
	if (!szFilename) return 0;
	if (!*szFilename) return 0;
	
	fileMot = fopen(szFilename, "w+");
	if (!fileMot) return 0;
	
	for (ptCurrentElement = ptFile->ptFirst; ptCurrentElement != NULL; ptCurrentElement = ptCurrentElement->ptNext)
	{
		if (!boLineToFile(fileMot, ptCurrentElement->ptData))
		{
			fflush(fileMot);
			fclose(fileMot);
			return 0;
		}
	}
	
	if (fflush(fileMot) != 0)
	{
		fclose(fileMot);
		return 0;
	}
	if (fclose(fileMot) != 0)
	{
		return 0;
	}
	
	return 1;
}

// TPartialMotData * ptCreateEmptyPartialMotData(unsigned long ulAddress, unsigned char ucMaxLength, unsigned char * aucInitialData, TMotFile * ptFile, TPartialMotData * ptLastPartial)

int boReadFromAddress(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, unsigned char * aucDataBuffer)
{
	TPartialMotData * ptPartial = NULL;
	TPartialMotData * ptPreviousPartial = NULL;
	unsigned char * pucBufferPos = aucDataBuffer;
	unsigned int uRestCount = uByteCount;
	unsigned long ulNextPartialAddress = ulAddress;
	unsigned char ucMaxLength = 0;
	int boError = 0;
	
	if (!ptFile) return 0;
	if (!aucDataBuffer) return 0;
	
	while (uRestCount > 0 && !boError)
	{
		ptPreviousPartial = ptPartial;
		ucMaxLength = (uRestCount > 0x7F ? 0x7F : (unsigned char)uRestCount);
		ptPartial = ptCreateEmptyPartialMotData(ulNextPartialAddress, ucMaxLength, NULL, ptFile, ptPreviousPartial);
		if (ptPartial)
		{
			if (boGetDataFromLine(ptPartial))
			{
				memcpy(pucBufferPos, ptPartial->aucPartialData, ptPartial->ucPartialDataLength);
				ulNextPartialAddress += ptPartial->ucPartialDataLength;
				uRestCount -= ptPartial->ucPartialDataLength;
				pucBufferPos += ptPartial->ucPartialDataLength;
			}
			else
			{
				boError = 1;
			}
		}
		else
		{
			boError = 1;
		}
			
		FreePartialMotData(ptPreviousPartial); // Macht nix, wenns NULL is, dann passiert einfach gar nix.
		ptPreviousPartial = NULL;
	}
	FreePartialMotData(ptPartial);
	
	return !boError;
}

int boWriteToAddress(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, const unsigned char * aucDataBuffer)
{
	TPartialMotData * ptPartial = NULL;
	TPartialMotData * ptPreviousPartial = NULL;
	const unsigned char * pucBufferPos = aucDataBuffer;
	unsigned int uRestCount = uByteCount;
	unsigned long ulNextPartialAddress = ulAddress;
	unsigned char ucMaxLength = 0;
	int boError = 0;
	
	if (!ptFile) return 0;
	if (!aucDataBuffer) return 0;
	
	while (uRestCount > 0 && !boError)
	{
		ptPreviousPartial = ptPartial;
		ucMaxLength = (uRestCount > 0x7F ? 0x7F : (unsigned char)uRestCount);
		ptPartial = ptCreateEmptyPartialMotData(ulNextPartialAddress, ucMaxLength, pucBufferPos, ptFile, ptPreviousPartial);
		if (ptPartial)
		{
			if (boMergeDataToLine(ptPartial))
			{
				
				ulNextPartialAddress += ptPartial->ucPartialDataLength;
				uRestCount -= ptPartial->ucPartialDataLength;
				pucBufferPos += ptPartial->ucPartialDataLength;
			}
			else
			{
				boError = 1;
			}
		}
		else
		{
			boError = 1;
		}
			
		FreePartialMotData(ptPreviousPartial); // Macht nix, wenns NULL is, dann passiert einfach gar nix.
		ptPreviousPartial = NULL;
	}
	FreePartialMotData(ptPartial);
	
	return !boError;
}


// UPDATES
int boGetMotFileAddressBoundaries(TMotFile * ptFile, unsigned long * pulLowestAddress, unsigned long * pulHighestAddress, unsigned long * pulDataCount)
{
	TMotFileElement * ptElement = NULL;
	unsigned long ulFirst = 0xFFFFFFFF;
	unsigned long ulLast = 0x00000000;
	unsigned long ulCount = 0;
	
	if (!ptFile) return 0;
	
	ptElement = ptFile->ptFirst;
	if (!ptElement) return 0;
	
	while (ptElement)
	{
		if (ptElement->ptData && ptElement->ptData->ucDLC > 0)
		{
			ulCount += ptElement->ptData->ucDLC;
			if (ptElement->ptData->ulAddress < ulFirst) ulFirst = ptElement->ptData->ulAddress;
			if (ptElement->ptData->ulAddress + ptElement->ptData->ucDLC - 1 > ulLast) ulLast = ptElement->ptData->ulAddress + ptElement->ptData->ucDLC - 1;
		}
		ptElement = ptElement->ptNext;
	}
	
	if (pulLowestAddress) *pulLowestAddress = ulFirst;
	if (pulHighestAddress) *pulHighestAddress = ulLast;
	if (pulDataCount) *pulDataCount = ulCount;
	
	
	
	return 1;
}


int boReadFromAddressFillEmpty(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, unsigned char * aucDataBuffer, unsigned char ucFillByte)
{
	TPartialMotData * ptPartial = NULL;
	TPartialMotData * ptPreviousPartial = NULL;
	unsigned char * pucBufferPos = aucDataBuffer;
	unsigned int uRestCount = uByteCount;
	unsigned long ulNextPartialAddress = ulAddress;
	unsigned char ucMaxLength = 0;
	int boError = 0;
	
	if (!ptFile) return 0;
	if (!aucDataBuffer) return 0;
	
	while (uRestCount > 0 && !boError)
	{
		ptPreviousPartial = ptPartial;
		ucMaxLength = (uRestCount > 0x7F ? 0x7F : (unsigned char)uRestCount);
		ptPartial = ptCreateEmptyPartialMotData(ulNextPartialAddress, ucMaxLength, NULL, ptFile, ptPreviousPartial);
		if (ptPartial)
		{
			if (boGetDataFromLine(ptPartial))
			{
				memcpy(pucBufferPos, ptPartial->aucPartialData, ptPartial->ucPartialDataLength);
				ulNextPartialAddress += ptPartial->ucPartialDataLength;
				uRestCount -= ptPartial->ucPartialDataLength;
				pucBufferPos += ptPartial->ucPartialDataLength;
			}
			else
			{
				boError = 1;
			}
		}
		else
		{
			TMotFileElement * ptBestElement = ptFindElementWithNextAddress(ulNextPartialAddress, ptFile, ptFile->ptFirst);
			if (ptBestElement)
			{
				if (ulNextPartialAddress + uRestCount <= ptBestElement->ptData->ulAddress)
				{ // Hinterer Rest, auffüllen
					memset(pucBufferPos, ucFillByte, uRestCount);
					pucBufferPos += uRestCount;
					uRestCount = 0;
				}
				else
				{ // Lücke, auffüllen
					memset(pucBufferPos, ucFillByte, ptBestElement->ptData->ulAddress - ulNextPartialAddress);
					pucBufferPos += ptBestElement->ptData->ulAddress - ulNextPartialAddress;
					uRestCount -= ptBestElement->ptData->ulAddress - ulNextPartialAddress;
					ulNextPartialAddress = ptBestElement->ptData->ulAddress;
				}
			}
			else
			{ // Hinterer Rest, auffüllen
				memset(pucBufferPos, ucFillByte, uRestCount);
				pucBufferPos += uRestCount;
				uRestCount = 0;
			}
		}
			
		FreePartialMotData(ptPreviousPartial); // Macht nix, wenns NULL is, dann passiert einfach gar nix.
		ptPreviousPartial = NULL;
	}
	FreePartialMotData(ptPartial);
	
	return !boError;
}

int boReadFromAddressOmitEmpty(TMotFile * ptFile, unsigned long ulAddress, unsigned int uByteCount, unsigned char * aucDataBuffer, unsigned int * puRealByteCount)
{
	TPartialMotData * ptPartial = NULL;
	TPartialMotData * ptPreviousPartial = NULL;
	unsigned char * pucBufferPos = aucDataBuffer;
	unsigned int uRestCount = uByteCount;
	unsigned long ulNextPartialAddress = ulAddress;
	unsigned char ucMaxLength = 0;
	int boError = 0;
	unsigned int uRealByteCount = uByteCount;
	
	if (!ptFile) return 0;
	if (!aucDataBuffer) return 0;
	
	while (uRestCount > 0 && !boError)
	{
		ptPreviousPartial = ptPartial;
		ucMaxLength = (uRestCount > 0x7F ? 0x7F : (unsigned char)uRestCount);
		ptPartial = ptCreateEmptyPartialMotData(ulNextPartialAddress, ucMaxLength, NULL, ptFile, ptPreviousPartial);
		if (ptPartial)
		{
			if (boGetDataFromLine(ptPartial))
			{
				memcpy(pucBufferPos, ptPartial->aucPartialData, ptPartial->ucPartialDataLength);
				ulNextPartialAddress += ptPartial->ucPartialDataLength;
				uRestCount -= ptPartial->ucPartialDataLength;
				pucBufferPos += ptPartial->ucPartialDataLength;
			}
			else
			{
				boError = 1;
			}
		}
		else
		{
			TMotFileElement * ptBestElement = ptFindElementWithNextAddress(ulNextPartialAddress, ptFile, ptFile->ptFirst);
			if (ptBestElement)
			{
				if (ulNextPartialAddress + uRestCount <= ptBestElement->ptData->ulAddress)
				{ // Hinterer Rest, weglassen
					uRealByteCount -= uRestCount;
					uRestCount = 0;
				}
				else
				{ // Lücke, auffüllen
					//fprintf(stderr, " # GAP [0x%08X .. 0x%08X] #%d\n", ulNextPartialAddress, ptBestElement->ptData->ulAddress-1, ptBestElement->ptData->ulAddress - ulNextPartialAddress);
					uRealByteCount -= ptBestElement->ptData->ulAddress - ulNextPartialAddress;
					uRestCount -= ptBestElement->ptData->ulAddress - ulNextPartialAddress;
					ulNextPartialAddress = ptBestElement->ptData->ulAddress;
				}
			}
			else
			{ // Hinterer Rest, auffüllen
				//fprintf(stderr, " # GAP [0x%08X .. 0x%08X]F #%d\n", ulNextPartialAddress, ulNextPartialAddress + uRestCount, uRestCount);
				uRealByteCount -= uRestCount;
				uRestCount = 0;
			}
		}
			
		FreePartialMotData(ptPreviousPartial); // Macht nix, wenns NULL is, dann passiert einfach gar nix.
		ptPreviousPartial = NULL;
	}
	FreePartialMotData(ptPartial);
	
	if (puRealByteCount) *puRealByteCount = uRealByteCount;
	
	return !boError;
}

int boFindSequence(TMotFile * ptFile, unsigned char * aucSearchSequence, unsigned long ulSequenceLength, unsigned long ulSearchStartAddress, unsigned long ulSearchEndAddress, unsigned long * pulFirstMatchAddress)
{
	unsigned long ulGuessStartAddress = 0x00000000;
	unsigned long ulComparePos = 0;
	unsigned long ulPartialPosCnt = 0;
	
	TPartialMotData * ptPartial = NULL;
	TPartialMotData * ptPreviousPartial = NULL;
	unsigned int uRestCount = ulSearchEndAddress - ulSearchStartAddress + 1;
	unsigned long ulNextPartialAddress = ulSearchStartAddress;
	unsigned char ucMaxLength = 0;
	int boError = 0;
	
	if (!ptFile) return 0;
	if (!aucSearchSequence) return 0;
	
	while (uRestCount > 0 && !boError && ulComparePos < ulSequenceLength)
	{
		ptPreviousPartial = ptPartial;
		ucMaxLength = (uRestCount > 0x7F ? 0x7F : (unsigned char)uRestCount);
		ptPartial = ptCreateEmptyPartialMotData(ulNextPartialAddress, ucMaxLength, NULL, ptFile, ptPreviousPartial);
		if (ptPartial)
		{
			if (boGetDataFromLine(ptPartial))
			{
				for (ulPartialPosCnt = 0; ulPartialPosCnt < ptPartial->ucPartialDataLength && ulComparePos < ulSequenceLength; ulPartialPosCnt++)
				{
					if (ulComparePos)
					{ // Mittendrin
						if (ptPartial->aucPartialData[ulPartialPosCnt] == aucSearchSequence[ulComparePos])
						{ // Nächstes Zeichen passt
							ulComparePos++;
						}
						else
						{ // Passt nicht, zurück auf Los
							ulComparePos = 0;
							if (ptPartial->aucPartialData[ulPartialPosCnt] == aucSearchSequence[ulComparePos])
							{ // Nächstes Zeichen passt
								ulGuessStartAddress = ptPartial->ulStartAddress + ulPartialPosCnt;
								ulComparePos++;
							}
						}
					}
					else
					{ // Anfang suchen
						if (ptPartial->aucPartialData[ulPartialPosCnt] == aucSearchSequence[ulComparePos])
						{ // Nächstes Zeichen passt
							ulGuessStartAddress = ptPartial->ulStartAddress + ulPartialPosCnt;
							ulComparePos++;
						}
					}
				}
				
				uRestCount -= ptPartial->ucPartialDataLength;
				ulNextPartialAddress += ptPartial->ucPartialDataLength;
			}
			else
			{
				boError = 1;
			}
		}
		else
		{
			TMotFileElement * ptBestElement = ptFindElementWithNextAddress(ulNextPartialAddress, ptFile, ptFile->ptFirst);
			ulComparePos = 0; // Weil wir einen Sprung hatten! Das bedeutet undefinierter Bereich, d. h. auch die FFs werden nicht berücksichtigt!
			if (ptBestElement)
			{
				if (ulNextPartialAddress + uRestCount <= ptBestElement->ptData->ulAddress)
				{ // Hinterer Rest, weglassen
					uRestCount = 0;
				}
				else
				{ // Lücke, auffüllen
					uRestCount -= ptBestElement->ptData->ulAddress - ulNextPartialAddress;
					ulNextPartialAddress = ptBestElement->ptData->ulAddress;
				}
			}
			else
			{ // Hinterer Rest, auffüllen
				uRestCount = 0;
			}
		}
			
		FreePartialMotData(ptPreviousPartial); // Macht nix, wenns NULL is, dann passiert einfach gar nix.
		ptPreviousPartial = NULL;
	}
	FreePartialMotData(ptPartial);
	
	if (!boError && ulComparePos >= ulSequenceLength)
	{ // Gefunden!
		if (pulFirstMatchAddress) *pulFirstMatchAddress = ulGuessStartAddress;
		
		return 1;
	}
	else return 0; // Nicht gefunden
}


