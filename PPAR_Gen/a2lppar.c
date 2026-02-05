//==============================================================================
//
// Title:       a2lppar.c
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
#include "a2lppar.h"
#include "a2ldata.h"
#include "motfile.h"
#include "strlib.h"
#include "crctester.h"



#define IDENTIFIER_MAX_SIZE 1024
#define PARAMETER_MAX_SIZE 1024
#define DATATYPE_MAX_SIZE 32

// Interne Datentypen



// Static vars
static TA2LFile * ptA2LFile = NULL;
static TMotFile * ptMotFile = NULL;


// SRC - PRIVATE
int boGetDataTypeInformation(const char * szDataTypeDescription, unsigned int * piSize, unsigned int * pboIsSigned, unsigned int * pboIsFloat)
{
	if (!szDataTypeDescription || !piSize || !pboIsSigned || !pboIsFloat) return 0;
	
	if (strcmp(szDataTypeDescription, "__UBYTE") == 0)
	{
		*piSize = 1;
		*pboIsSigned = 0;
		*pboIsFloat = 0;
	}
	else if (strcmp(szDataTypeDescription, "__SBYTE") == 0)
	{
		*piSize = 1;
		*pboIsSigned = 1;
		*pboIsFloat = 0;
	}
	else if (strcmp(szDataTypeDescription, "__UWORD") == 0)
	{
		*piSize = 2;
		*pboIsSigned = 0;
		*pboIsFloat = 0;
	}
	else if (strcmp(szDataTypeDescription, "__SWORD") == 0)
	{
		*piSize = 2;
		*pboIsSigned = 1;
		*pboIsFloat = 0;
	}
	else if (strcmp(szDataTypeDescription, "__ULONG") == 0)
	{
		*piSize = 4;
		*pboIsSigned = 0;
		*pboIsFloat = 0;
	}
	else if (strcmp(szDataTypeDescription, "__SLONG") == 0)
	{
		*piSize = 4;
		*pboIsSigned = 1;
		*pboIsFloat = 0;
	}
	else if (strcmp(szDataTypeDescription, "__FLOAT32") == 0)
	{
		*piSize = 4;
		*pboIsSigned = 1;
		*pboIsFloat = 1;
	}
	else if (strcmp(szDataTypeDescription, "__FLOAT64") == 0)
	{
		*piSize = 8;
		*pboIsSigned = 1;
		*pboIsFloat = 1;
	}
	else
	{
		return 0;
	}
	
	return 1;
}


double dRound(double d)
{
	return floor(d + 0.5);
}


double dXToDouble(void * pValue, unsigned int uSizeOfValue, int boValueIsSigned, int boValueIsFloat)
{
	switch (uSizeOfValue)
	{
		case 1:
			// 8 bit
			if (boValueIsFloat) return 0.0; //Hamma ned!
			if (boValueIsSigned)
			{ // signed char
				return (double)*(signed char *)pValue;
			}
			else
			{ // unsigned char
				return (double)*(unsigned char *)pValue;
			}
		case 2:
			// 16 bit
			if (boValueIsFloat) return 0; //Hamma ned!
			if (boValueIsSigned)
			{ // signed short
				return (double)*(signed short *)pValue;
			}
			else
			{ // unsigned short
				return (double)*(unsigned short *)pValue;
			}
		case 4:
			// 32 bit
			if (boValueIsFloat)
			{ // float
				return (double)*(float *)pValue;
			}
			else if (boValueIsSigned)
			{ // signed long
				return (double)*(signed long *)pValue;
			}
			else
			{ // unsigned long
				return (double)*(unsigned long *)pValue;
			}
		case 8:
			// 64 bit
			if (!boValueIsFloat) return 0; // Hamma ned!
			// double
			return (double)*(double *)pValue; // Der Cast ist sinnlos, aber was solls...
		default:
			// Hamma ned!
			return 0;
	}
}



#define ConvToX(Target, Source, datatype) *(datatype *)Target = (datatype)Source

int boDoubleToX(double dSource, void * pTarget, unsigned int uSizeOfTarget, int boTargetIsSigned, int boTargetIsFloat)
{
	if (!boTargetIsFloat)
	{ // Runden, wenn nicht float hinten
		dSource = dRound(dSource);
		// Datentypprüfung
		if (boTargetIsSigned)
		{
			if (dSource < (~0 << (uSizeOfTarget) * 8 - 1)) dSource = (~0 << (uSizeOfTarget) * 8 - 1);
			if (dSource > ~(~0 << (uSizeOfTarget) * 8 - 1)) dSource = ~(~0 >> (uSizeOfTarget) * 8 - 1);
		}
		else
		{ // Unsigned limits
			if (dSource < (0UL)) dSource = (0UL);
			if (dSource > (~0UL >> (sizeof(unsigned long)-uSizeOfTarget) * 8)) dSource = (~0UL >> (sizeof(unsigned long)-uSizeOfTarget) * 8);
		}
	}
	
	switch (uSizeOfTarget)
	{
		case 1:
			// 8 bit
			if (boTargetIsFloat) return 0; //Hamma ned!
			if (boTargetIsSigned)
			{ // signed char
				ConvToX(pTarget, dSource, signed char);
			}
			else
			{ // unsigned char
				ConvToX(pTarget, dSource, unsigned char);
			}
			break;
		case 2:
			// 16 bit
			if (boTargetIsFloat) return 0; //Hamma ned!
			if (boTargetIsSigned)
			{ // signed short
				ConvToX(pTarget, dSource, signed short);
			}
			else
			{ // unsigned short
				ConvToX(pTarget, dSource, unsigned short);
			}
			break;
		case 4:
			// 32 bit
			if (boTargetIsFloat)
			{ // float
				ConvToX(pTarget, dSource, float);
			}
			else if (boTargetIsSigned)
			{ // signed long
				ConvToX(pTarget, dSource, signed long);
			}
			else
			{ // unsigned long
				ConvToX(pTarget, dSource, unsigned long);
			}
			
			break;
		case 8:
			// 64 bit
			if (!boTargetIsFloat) return 0; // Hamma ned!
			// double
			ConvToX(pTarget, dSource, double); // Wieder sowas eher sinnbefreites... aber was solls ;)
			break;
		default:
			// Hamma ned!
			return 0;
	}
	
	return 1;
}

int boLEtoBE(int iBytes, const unsigned char * aucLittleEndianData, unsigned char * aucBigEndianDataBuffer)
{
	int i;
	for (i = 0; i < iBytes; i++)
	{
		aucBigEndianDataBuffer[iBytes - 1 - i] = aucLittleEndianData[i];
	}
	
	return 1;
}


int boLinearConvertToByteArray(void * pValue, unsigned int uSizeOfValue, int boValueIsSigned, int boValueIsFloat, unsigned char * aucDataBuffer, int iTargetSize, int boIsSigned, int boIsFloat, double dScalingFactor, double dScalingOffset)
{ // Nicht vergessen, der Schlonz ist ja auch noch im Big Endian Format gespeichert
	double dCalculation = 0.0;
	int iByteIndex = 0;
	unsigned char * aucTempBuffer = NULL;
	
	dCalculation = dScalingFactor * dXToDouble(pValue, uSizeOfValue, boValueIsSigned, boValueIsFloat) + dScalingOffset;
	
	aucTempBuffer = (unsigned char *)malloc(iTargetSize);
	if (!aucTempBuffer)
	{ // Malloc-Fehler
		return 0;
	}
	memset(aucTempBuffer, 0, iTargetSize);
	if (!boDoubleToX(dCalculation, aucTempBuffer, iTargetSize, boIsSigned, boIsFloat))
	{ // Konvertierungsfehler
		return 0;
	}
	
	// Endianess bearbeiten - make it BIIIIG my friend ;)
	for (iByteIndex = 0; iByteIndex < iTargetSize; iByteIndex++)
	{ // Wir fangen beim MSB an!
		aucDataBuffer[iByteIndex] = aucTempBuffer[iTargetSize - iByteIndex - 1];
	}
	
	return 1;
}



// SRC - PUBLIC
int boStartPParGeneration(const char * szA2LFile, const char * szTemplateMotFile)
{ // Dateien in den Speicher laden
	if (!szA2LFile || !szTemplateMotFile || !*szA2LFile || !*szTemplateMotFile) return 0;
	
	// A2L File laden...
	if (ptA2LFile)
	{
		FreeA2LFile(ptA2LFile);
		ptA2LFile = NULL;
	}
	
	ptA2LFile = ptLoadA2LFile(szA2LFile);
	if (!ptA2LFile)
	{
		return 0;
	}
	
	// MOT File laden
	if (ptMotFile)
	{
		FreeMotFile(ptMotFile);
		ptMotFile = NULL;
	}
	
	ptMotFile = ptLoadMotFile(szTemplateMotFile);
	if (!ptMotFile)
	{
		FreeA2LFile(ptA2LFile);
		ptA2LFile = NULL;
		return 0;
	}
	
	return 1;
}

int boFinishPParGeneration(const char * szTargetMotFile)
{ // Dateien ggf. speichern und schließen / aus dem Memory werfen
	int boError = 0;
	
	if (!szTargetMotFile || !*szTargetMotFile || !ptMotFile) return 0;
	
	if (!boSaveMotFile(ptMotFile, szTargetMotFile))
	{
		boError = 1;
	}
	
	FreeA2LFile(ptA2LFile);
	ptA2LFile = NULL;
	FreeMotFile(ptMotFile);
	ptMotFile = NULL;
	
	return !boError;
}



int boWriteValues(const char * szIdentifier, const void * pValue, unsigned int uSizeOfElement, int boValuesAreSigned, int boValuesAreFloat, unsigned int uNumOfElements)
{
	TA2LBlock * ptA2LBlock = NULL;
	TA2LBlock * ptA2LConvBlock = NULL;
	
	int boResult = 0;
	
	char szCompuLink[IDENTIFIER_MAX_SIZE];
	char szParamBuffer[PARAMETER_MAX_SIZE];
	char szDataType[DATATYPE_MAX_SIZE];
	
	int iArrayDims[3] = {1, 1, 1};
	int boIsArray = 0;
	double dTempValue = 0.0;
	double dScalingOffset = 0.0;
	double dScalingFactor = 1.0;
	unsigned long ulAddress = 0;
	
	unsigned char * aucDataBytes = NULL;
	unsigned int uNumDataBytes = 0;
	unsigned int uPosOffset = 0;
	
	unsigned char * pCurrentValue = pValue;
	unsigned int uValOffset = 0;
	
	int iSize = 0;
	int boIsSigned = 0;
	int boIsFloat = 0;
	
	int iCnt = 0;
	
	if (!ptMotFile || !ptA2LFile || !pValue || !szIdentifier) return 0;
	
	// 1. Identifier nachschlagen
	ptA2LBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "CHARACTERISTIC", szIdentifier);
	if (!ptA2LBlock)
	{ // Block nicht gefunden
		return 0;
	}
	
	// 2. Array oder Einzelwert?
	boResult = boGetParameter(ptA2LBlock, "VALUE", 4, sizeof(szCompuLink), szCompuLink);
	if (!boResult)
	{ // Dann is es ein Array
		boResult = boGetParameter(ptA2LBlock, "VAL_BLK", 4, sizeof(szCompuLink), szCompuLink);
		if (!boResult)
		{ // Das muss jetzt irgendein schmarrn sein...
			FreeA2LBlock(ptA2LBlock);
			return 0;
		}
		else
		{
			boIsArray = 1;
			// Dann die Arraygrößen holen
			for (iCnt = 0; iCnt < 3; iCnt++)
			{
				boResult = boGetParameter(ptA2LBlock, "MATRIX_DIM", iCnt+1, sizeof(szParamBuffer), szParamBuffer);
				if (!boResult)
				{ // Default-Wert, falls nicht gefunden
					iArrayDims[iCnt] = 1;
				}
				else
				{
					iArrayDims[iCnt] = atoi(szParamBuffer);
				}
			}
		}
	}
	else
	{ // Es ist kein Array
		iArrayDims[0] = iArrayDims[1] = iArrayDims[2] = 1;
	}
	
	// 3. Umrechnungsformel holen
	ptA2LConvBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "COMPU_METHOD", szCompuLink);
	if (!ptA2LConvBlock)
	{ // Dann gibt es keine (gültige) Umrechnung
		dScalingFactor = 1.0;
		dScalingOffset = 0.0;
	}
	else
	{
		boResult = boGetParameter(ptA2LConvBlock, "COEFFS", 2, sizeof(szParamBuffer), szParamBuffer);
		if (!boResult)
		{ // Auch keine (gültige) Umrechnung - Formeln akzeptieren wir nicht!
			dScalingFactor = 1.0;
		}
		else
		{
			dTempValue = atof(szParamBuffer);
			if (dTempValue == 0.0 || IsNotANumber(dTempValue) || IsInfinity(dTempValue))
			{ // Alles ungültiger Scheiß...
				dScalingFactor = 1.0;
			}
			else
			{
				//dScalingFactor = 1.0 / dTempValue; // Rechnet zurück!
				dScalingFactor = dTempValue;
			}
		}
	
		boResult = boGetParameter(ptA2LConvBlock, "COEFFS", 3, sizeof(szParamBuffer), szParamBuffer);
		if (!boResult)
		{ // Keine Gültige Umrechnung - Standardwert
			dScalingOffset = 0.0;
		}
		else
		{
			dTempValue = atof(szParamBuffer);
			if (IsNotANumber(dTempValue) || IsInfinity(dTempValue))
			{ // Ungültig...
				dScalingOffset = 0.0;
			}
			else
			{
				//dScalingOffset = -dTempValue * dScalingFactor; // Rechnet zurück!
				dScalingOffset = dTempValue;
			}
		}
	
		FreeA2LBlock(ptA2LConvBlock);	
	}
	
	// 4. Adresse laden
	boResult = boGetParameter(ptA2LBlock, (boIsArray ? "VAL_BLK" : "VALUE"), 1, sizeof(szParamBuffer), szParamBuffer);
	if (!boResult)
	{ // Keine Adresse, keine Kekse!
		FreeA2LBlock(ptA2LBlock);
		return 0;
	}
	else
	{
		ulAddress = ulHexToLong(szParamBuffer);
	}
	
	// 5. Zielformat bestimmen
	boResult = boGetParameter(ptA2LBlock, (boIsArray ? "VAL_BLK" : "VALUE"), 2, sizeof(szParamBuffer), szParamBuffer);
	if (!boResult)
	{ // Also Format brauchen wir schon...
		FreeA2LBlock(ptA2LBlock);
		return 0;
	}
	else
	{
		if (!boGetDataTypeInformation(szParamBuffer, &iSize, &boIsSigned, &boIsFloat))
		{ // ... und bekannt sollte es auch sein...
			FreeA2LBlock(ptA2LBlock);
			return 0;
		}
	}
	
	FreeA2LBlock(ptA2LBlock);
	
	// 6. Werte laden und konvertieren
	uNumDataBytes = iArrayDims[0] * iArrayDims[1] * iArrayDims[2] * iSize;
	aucDataBytes = (unsigned char *)malloc(uNumDataBytes);
	if (!aucDataBytes)
	{ // Allozierung fehlgeschlagen
		return 0;
	}
	memset(aucDataBytes, 0, uNumDataBytes);
	
	for (uPosOffset = 0, uValOffset = 0; (uPosOffset < uNumDataBytes) && (uValOffset < uSizeOfElement * uNumOfElements); uPosOffset += iSize, uValOffset += uSizeOfElement) // Ich will jetzt definitiv von NIEMANDEM hören, dass der Kommaoperator evil ist! Der ist da nämlich optimal eingesetzt, und ich hab hier keinen Bock, gleichwertige Bedingungen und Operationen einmal im Schleifenkopf und einmal im Rumpf mit break und so zu machen. Näh. Da ist die Kommaoperatorlösung viel viel übersichtlicher!
	{
		if (!boLinearConvertToByteArray((char *)pValue + uValOffset, uSizeOfElement, boValuesAreSigned, boValuesAreFloat, aucDataBytes + uPosOffset, iSize, boIsSigned, boIsFloat, dScalingFactor, dScalingOffset))
		{ // Konvertierungsfehler
			free(aucDataBytes);
			return 0;
		}
	}
	
	// 7. Werte ablegen
	if (!boWriteToAddress(ptMotFile, ulAddress, uNumDataBytes, aucDataBytes))
	{ // Irgendwas ist wohl schiefgelaufen
		free(aucDataBytes);
		return 0;
	}
	
	free(aucDataBytes);
	
	// Das wars!
	return 1;
}


#define CRC_LEN_BYTES 4

#define OVERRIDE_MODE_NONE 0
#define OVERRIDE_MODE_OMIT 1
#define OVERRIDE_MODE_ZEROES 2
#define OVERRIDE_MODE_ONES 3


#define OVERRIDE_MODE OVERRIDE_MODE_OMIT


int boCalculatePPARChecksum(const char * szCRCStartAddressIdentifier, const char * szCRCEndAddressIdentifier, const char * szCRCPolynomIdentifier, const char * szTargetAddressIdentifier)
{
	unsigned char * aucDataBuffer = NULL;
	
	unsigned long ulStartAddress = 0x00000000;
	unsigned long ulEndAddress = 0x00000000;
	unsigned long ulDataLength = 0;
	unsigned long ulDataLengthPart1 = 0; // von Start bis TargetAddress oder End, jenachdem was früher kommt
	unsigned long ulDataLengthPart2 = 0; // ggf. von TargetAddress + CRC_LEN_BYTES bis End, falls die TargetAddress zwischen Start und End liegt
	unsigned long ulPolynomAddress = 0x00000000;
	unsigned long ulTargetAddress = 0x00000000;

	unsigned char aucPolynomData[CRC_LEN_BYTES];
	unsigned long ulDataStart = 0x00000000;
	unsigned long ulDataEnd = 0x00000000;
	unsigned long ulPolynom = 0x00000000;
	
	unsigned long ulResultCRC = 0x00000000;
	
	int boResult = 0;
	
	TA2LBlock * ptA2LBlock = NULL;
	
	char szParamBuffer[PARAMETER_MAX_SIZE];
	
	if (!ptMotFile || !ptA2LFile) return 0;
	
	if (!szCRCStartAddressIdentifier || !szCRCEndAddressIdentifier || !szCRCPolynomIdentifier || !szTargetAddressIdentifier) return 0;
	

	
	// 1. Identifier nachschlagen
	// 1.1 Startadresse
	ptA2LBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "CHARACTERISTIC", szCRCStartAddressIdentifier);
	if (!ptA2LBlock)
	{ // Block nicht gefunden
		return 0;
	}
	
	boResult = boGetParameter(ptA2LBlock, "VALUE", 1, sizeof(szParamBuffer), szParamBuffer);
	if (!boResult)
	{ // Arrays mögen wir nicht...
		FreeA2LBlock(ptA2LBlock);
		return 0;
	}
	else
	{
		ulStartAddress = ulHexToLong(szParamBuffer);
	}
	FreeA2LBlock(ptA2LBlock);
	
	// 1.2 Startadresse
	ptA2LBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "CHARACTERISTIC", szCRCEndAddressIdentifier);
	if (!ptA2LBlock)
	{ // Block nicht gefunden
		return 0;
	}
	
	boResult = boGetParameter(ptA2LBlock, "VALUE", 1, sizeof(szParamBuffer), szParamBuffer);
	if (!boResult)
	{ // Arrays mögen wir nicht...
		FreeA2LBlock(ptA2LBlock);
		return 0;
	}
	else
	{
		ulEndAddress = ulHexToLong(szParamBuffer);
	}
	FreeA2LBlock(ptA2LBlock);
	
	// 1.3 Polynomadresse
	ptA2LBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "CHARACTERISTIC", szCRCPolynomIdentifier);
	if (!ptA2LBlock)
	{ // Block nicht gefunden
		return 0;
	}
	
	boResult = boGetParameter(ptA2LBlock, "VALUE", 1, sizeof(szParamBuffer), szParamBuffer);
	if (!boResult)
	{ // Arrays mögen wir nicht...
		FreeA2LBlock(ptA2LBlock);
		return 0;
	}
	else
	{
		ulPolynomAddress = ulHexToLong(szParamBuffer);
	}
	FreeA2LBlock(ptA2LBlock);
	
	// 1.4 Zieladresse
	ptA2LBlock = ptFindFirstInfoBlock(ptA2LFile->ptRootBlock, "CHARACTERISTIC", szTargetAddressIdentifier);
	if (!ptA2LBlock)
	{ // Block nicht gefunden
		return 0;
	}
	
	boResult = boGetParameter(ptA2LBlock, "VALUE", 1, sizeof(szParamBuffer), szParamBuffer);
	if (!boResult)
	{ // Arrays mögen wir nicht...
		FreeA2LBlock(ptA2LBlock);
		return 0;
	}
	else
	{
		ulTargetAddress = ulHexToLong(szParamBuffer);
	}
	FreeA2LBlock(ptA2LBlock);
	
	
	// 2.1 Adressen holen
	memset(aucPolynomData, 0, sizeof(aucPolynomData));
	if (!boReadFromAddress(ptMotFile, ulStartAddress, CRC_LEN_BYTES, aucPolynomData))
	{ // Möp!
		return 0;
	}
	else
	{
		ulDataStart = (((unsigned long)aucPolynomData[0]) << 24) | (((unsigned long)aucPolynomData[1]) << 16) | (((unsigned long)aucPolynomData[2]) << 8) | (((unsigned long)aucPolynomData[3]));
	}
	
	memset(aucPolynomData, 0, sizeof(aucPolynomData));
	if (!boReadFromAddress(ptMotFile, ulEndAddress, CRC_LEN_BYTES, aucPolynomData))
	{ // Möp!
		return 0;
	}
	else
	{
		ulDataEnd = (((unsigned long)aucPolynomData[0]) << 24) | (((unsigned long)aucPolynomData[1]) << 16) | (((unsigned long)aucPolynomData[2]) << 8) | (((unsigned long)aucPolynomData[3]));
		ulDataEnd++; // Gemeint ist das LETZTE Datenbyte, über das noch die CS berechnet wird!
	}
	
	// 2. Randbedingungen festlegen
#if OVERRIDE_MODE == OVERRIDE_MODE_OMIT
	// 2.1 Sonderfälle - Checksummenfeld weglassen bei der Berechnung!
	// Checksumme am Anfang
	if (ulTargetAddress <= ulDataStart && ulTargetAddress + CRC_LEN_BYTES >= ulDataStart)
	{ 
		ulDataStart = ulTargetAddress + CRC_LEN_BYTES;
	}
	
	
	if (ulTargetAddress <= ulDataEnd && ulTargetAddress + CRC_LEN_BYTES >= ulDataEnd)
	{ // Checksumme am Ende
		ulDataEnd = ulTargetAddress;
	}
	
	if (ulTargetAddress > ulDataStart && ulTargetAddress + CRC_LEN_BYTES < ulDataEnd)
	{ // Checksumme mitten drin
		ulDataLengthPart1 = ulTargetAddress - ulDataStart;
		ulDataLengthPart2 = ulDataEnd - ulTargetAddress - CRC_LEN_BYTES;
		ulDataLength = ulDataLengthPart1 + ulDataLengthPart2;
	}
	else
	{
		ulDataLength = ulDataEnd - ulDataStart;
		ulDataLengthPart1 = ulDataLength;
		ulDataLengthPart2 = 0;
	}
	
#else//OVERRIDE_MODE == OVERRIDE_MODE_OMIT
	
	ulDataLength = ulDataEnd - ulDataStart;
	ulDataLengthPart1 = ulDataLength;
	ulDataLengthPart2 = 0;
	
#endif//OVERRIDE_MODE == OVERRIDE_MODE_OMIT

	if (ulDataStart >= ulDataEnd)
	{ // Blödsinnige Anforderung ;)
		return 0;
	}
	
	// 3. Polynom holen
	memset(aucPolynomData, 0, sizeof(aucPolynomData));
	if (!boReadFromAddress(ptMotFile, ulPolynomAddress, CRC_LEN_BYTES, aucPolynomData))
	{ // Möp!
		return 0;
	}
	else
	{
		ulPolynom = (((unsigned long)aucPolynomData[0]) << 24) | (((unsigned long)aucPolynomData[1]) << 16) | (((unsigned long)aucPolynomData[2]) << 8) | (((unsigned long)aucPolynomData[3]));
	}
	
	// 4. Daten holen
#if OVERRIDE_MODE	== OVERRIDE_MODE_ZEROES || OVERRIDE_MODE == OVERRIDE_MODE_ONES
	
	// 4.1 Checksumme ausgrauen
#if OVERRIDE_MODE == OVERRIDE_MODE_ZEROES
	memset(aucPolynomData, 0x00, sizeof(aucPolynomData));
#elif OVERRIDE_MODE == OVERRIDE_MODE_ONES
	memset(aucPolynomData, 0xFF, sizeof(aucPolynomData));
#endif//OVERRIDE_MODE == OVERRIDE_MODE_ONES
	if (!boWriteToAddress(ptMotFile, ulTargetAddress, CRC_LEN_BYTES, aucPolynomData))
	{
		return 0;
	}
	
#endif//OVERRIDE_MODE	== OVERRIDE_MODE_ZEROES || OVERRIDE_MODE == OVERRIDE_MODE_ONES
	
	// 4.2 Jetz aber wirklich
	
	aucDataBuffer = (unsigned char *)malloc(ulDataLength);
	if (!aucDataBuffer)
	{ // Fehlgeschlagen
		return 0;
	}
	memset(aucDataBuffer, 0, ulDataLength);
	
	if (!boReadFromAddress(ptMotFile, ulDataStart, ulDataLengthPart1, aucDataBuffer))
	{ // Mööööp!! (z. B. Daten nicht durchgehend vorhanden und so Stuss...)
		free(aucDataBuffer);
		return 0;
	}
	
	if (ulDataLengthPart2 > 0)
	{
		if (!boReadFromAddress(ptMotFile, ulTargetAddress + CRC_LEN_BYTES, ulDataLengthPart2, aucDataBuffer + ulDataLengthPart1))
		{ // Waaah!
			free(aucDataBuffer);
			return 0;
		}
	}
	
	/*
	if (!boCalculateCRC(9, "123456789", 8 * CRC_LEN_BYTES, ulPolynom, 1, 0xFFFFFFFF, 0xFFFFFFFF, 1, 1, &ulResultCRC))
	{
		free(aucDataBuffer);
		return 0;
	}
	printf("CHECK CRC: 0x%08X\n", ulResultCRC);
	
	*/
	
	// 5. Checksumme berechnen
	if (!boCalculateCRC(ulDataLength, aucDataBuffer, 8 * CRC_LEN_BYTES, ulPolynom, 1, 0xFFFFFFFF, 0xFFFFFFFF, 1, 1, &ulResultCRC))
	{
		free(aucDataBuffer);
		return 0;
	}
	
	// 6. Checksumme schreiben
	free(aucDataBuffer); // Brauch ma nimmer.
	
	//printf("CRC: 0x%08X\n", ulResultCRC);
	
	boLEtoBE(4, (unsigned char *)(&ulResultCRC), aucPolynomData);
	
	if (!boWriteToAddress(ptMotFile, ulTargetAddress, CRC_LEN_BYTES, aucPolynomData))
	{
		return 0;
	}
	
	
	return 1;
}



void INIT_A2LPPAR(void)
{
	static int boInitialized = 0;
	
	if (boInitialized) return;
	boInitialized = 1;
	
	
}

