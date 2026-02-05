#include <toolbox.h>
#include <cvirte.h>		
#include <userint.h>

#include "MotCRC.h"


int boCalculateCRC32(const char * szFilepath, unsigned long * pucCRC32)
{ // CRC32 berechnen
	unsigned long ucCRC32 = 0x00000000;
	unsigned char * aucFileData = NULL;
	int iFileLength = 0;
	int boResult = 0;
	FILE * hFile = NULL;
	
	if (!szFilepath || !*szFilepath) return 0; 
	if (FileExists(szFilepath, &iFileLength) != 1) return 0;
	
	hFile = fopen(szFilepath, "rb");
	if (!hFile) return 0;
	
	aucFileData = (unsigned char *)malloc(iFileLength);
	if (!aucFileData)
	{
		fclose(hFile);
		return 0;
	}
	
	if (!fread(aucFileData, iFileLength, 1, hFile))
	{
		free(aucFileData);
		fclose(hFile);
		return 0;
	}
	
	// Daten gelesen... weiter mit den Daten im Speicher
	fclose(hFile);
	
	boResult = boCalculateCRC(iFileLength, aucFileData, 32, 0x04C11DB7, 1, 0xFFFFFFFF, 0xFFFFFFFF, 1, 1, &ucCRC32);
	
	// Brauchen wir jetzt auch nicht mehr
	free(aucFileData);

	if (!boResult)
	{
		return 0;
	}
	
	if (pucCRC32) *pucCRC32 = ucCRC32;
	return 1;
}


int boLE2BE(int iBytes, const unsigned char * aucLittleEndianData, unsigned char * aucBigEndianDataBuffer)
{
	int i;
	for (i = 0; i < iBytes; i++)
	{
		aucBigEndianDataBuffer[iBytes - 1 - i] = aucLittleEndianData[i];
	}
	
	return 1;
}

int boBE2LE(int iBytes, const unsigned char * aucBigEndianData, unsigned char * aucLittleEndianDataBuffer)
{ // Das gleiche nur in grün.
	return boLE2BE(iBytes, aucBigEndianData, aucLittleEndianDataBuffer);
}

int boCheckEmpty(unsigned int uBytes, const unsigned char * aucData)
{
	unsigned int uCnt = 0;
	for (uCnt = 0; uCnt < uBytes; uCnt++)
	{
		if (aucData[uCnt] != '\xFF') return 0;
	}
	return 1;
}


int boCalculateBlockCRC32(TMotFile * ptMotFile, TMotBlock * ptMotBlock)
{
	unsigned long ulDataLength = 0x00000000;
	unsigned long ulBlock1Start = 0x00000000;
	unsigned long ulBlock1EndEx = 0x00000000; // Exklusivadresse, einfacher zu rechnen! D. h. die erste, die NICHT mehr beachtet wird
	unsigned long ulBlock2Start = 0x00000000;
	unsigned long ulBlock2EndEx = 0x00000000;
	unsigned char * aucCRCData = NULL;
	int boResult = 0;
	
	if (!ptMotFile || !ptMotBlock) return 0;
	
	if (ptMotBlock->ulCRCTargetAddress + 4 > ptMotBlock->ulCRCStart && ptMotBlock->ulCRCTargetAddress < ptMotBlock->ulCRCEnd+1)
	{ // CRC Target im Berechnungsbereich -> weglassen
		ulBlock1Start = ptMotBlock->ulCRCStart;
		ulBlock1EndEx = ptMotBlock->ulCRCTargetAddress;
		ulBlock2Start = ptMotBlock->ulCRCTargetAddress+4;
		ulBlock2EndEx = ptMotBlock->ulCRCEnd+1;
		
		if (ulBlock1EndEx < ulBlock1Start)
		{ // CRC am Anfang
			if (ulBlock2Start < ulBlock1Start) ulBlock2Start = ulBlock1Start; // Ganz vor dem Bereich
			ulBlock1EndEx = ulBlock1Start = 0x00000000;
		}
		if (ulBlock2Start > ulBlock2EndEx)
		{
			if (ulBlock1EndEx > ulBlock2EndEx) ulBlock1EndEx = ulBlock2EndEx; // Ganz hinter dem Bereich
			ulBlock2EndEx = ulBlock2Start = 0x00000000;
		}
	}
	else
	{
		ulBlock1Start = ptMotBlock->ulCRCStart;
		ulBlock1EndEx = ptMotBlock->ulCRCEnd+1;
		ulBlock2Start = 0x00000000;
		ulBlock2EndEx = 0x00000000;
	}
	
	ulDataLength = (ulBlock1EndEx - ulBlock1Start) + (ulBlock2EndEx - ulBlock2Start);
	if (ulDataLength == 0) return 0;
	
	aucCRCData = (unsigned char *)malloc(ulDataLength);
	
	if (ulBlock1Start != ulBlock1EndEx)
	{
		boResult = boReadFromAddress(ptMotFile, ulBlock1Start, ulBlock1EndEx - ulBlock1Start, aucCRCData);
		if (!boResult)
		{
			free(aucCRCData);
			return 0;
		}
	}
	
	if (ulBlock2Start != ulBlock2EndEx)
	{
		boResult = boReadFromAddress(ptMotFile, ulBlock2Start, ulBlock2EndEx - ulBlock2Start, aucCRCData + (ulBlock1EndEx - ulBlock1Start));
		if (!boResult)
		{
			free(aucCRCData);
			return 0;
		}
	}
	
	boResult = boCalculateCRC(ulDataLength, aucCRCData, 32, 0x04C11DB7, 1, 0xFFFFFFFF, 0xFFFFFFFF, 1, 1, &ptMotBlock->ulCRCReal);
	
	free(aucCRCData);
	
	return boResult;
}


int boCalculateBlockCRC16(TMotFile * ptMotFile, unsigned long ulFirstBlockAddress, unsigned long ulLastBlockAddress, unsigned short * pusBlockCRC16)
{
	unsigned long ulBlockLength = 0;
	unsigned long ulCRC16 = 0;
	unsigned char * aucDataBuffer = NULL;
	int boResult = 0;
	
	if (ulFirstBlockAddress > ulLastBlockAddress) return 0;
	
	if (!ptMotFile) return 0;
	
	ulBlockLength = ulLastBlockAddress - ulFirstBlockAddress + 1;
	aucDataBuffer = (unsigned char *)malloc(ulBlockLength);
	if (!aucDataBuffer)
	{
		return 0;
	}
	
	boResult = boReadFromAddressFillEmpty(ptMotFile, ulFirstBlockAddress, ulBlockLength, aucDataBuffer, 0xFF);
	if (!boResult)
	{
		free(aucDataBuffer);
		return 0;
	}
	
	boResult = boCalculateCRC(ulBlockLength, aucDataBuffer, 16, 0x1021, 1, 0xFFFF, 0x0000, 0, 0, &ulCRC16);
	
	if (boResult && pusBlockCRC16) *pusBlockCRC16 = (unsigned short)ulCRC16;
	
	free(aucDataBuffer);
	
	return boResult;
}



int boCalculateMOTCRCs(const char * szFilepath, unsigned short * pusCRC16, unsigned int * puBlockCount, TMotBlock ** patStaticBlockArray, unsigned long * pulLowestAddress, unsigned long * pulHighestAddress, int * pboNoDataOutsideBlocks)
{
	static TMotBlock * atBlocks = NULL;
	static unsigned int uBlocksCount = 0;
	unsigned int uBlockCount = 0;
	TMotFile * ptMotFile = NULL;
	int iFileLength = 0;
	int boResult = 0;
	unsigned long ulLowestAddress = 0x00000000;
	unsigned long ulHighestAddress = 0x00000000;
	unsigned long ulDataCount = 0;
	unsigned long ulCRC16 = 0x0000;
	unsigned long ulRealDataLength = 0;
	unsigned char * aucDataBuffer = NULL;
	unsigned long ulMatchAddress = 0x00000000;
	unsigned long ulSearchStartAddress = 0x00000000;
	unsigned int uCurrentBlock = 0;
	
	if (!szFilepath || !*szFilepath) return 0;
	if (FileExists(szFilepath, &iFileLength) != 1) return 0;
	
	ptMotFile = ptLoadMotFile(szFilepath);
	if (!ptMotFile)
	{
		return 0;
	}
	
	boResult = boGetMotFileAddressBoundaries(ptMotFile, &ulLowestAddress, &ulHighestAddress, &ulDataCount);
	if (!boResult)
	{
		FreeMotFile(ptMotFile);
		ptMotFile = NULL;
		return 0;
	}
	
	if (pulLowestAddress) *pulLowestAddress = ulLowestAddress;
	if (pulHighestAddress) *pulHighestAddress = ulHighestAddress;
	
	/* CRC 16 CCITT (Promik) */
	
	if (pusCRC16)
	{
		aucDataBuffer = (unsigned char *)malloc(ulDataCount);
		if (!aucDataBuffer)
		{ // Fehlgeschlagen
			FreeMotFile(ptMotFile);
			ptMotFile = NULL;
			return 0;
		}
		memset(aucDataBuffer, 0, ulDataCount);
	
		boResult = boReadFromAddressOmitEmpty(ptMotFile, ulLowestAddress, ulHighestAddress - ulLowestAddress + 1, aucDataBuffer, &ulRealDataLength);
		if (!boResult)
		{
			FreeMotFile(ptMotFile);
			ptMotFile = NULL;
			return 0;
		}
	
		boResult = boCalculateCRC(ulRealDataLength, aucDataBuffer, 16, 0x1021, 1, 0xFFFF, 0x0000, 0, 0, &ulCRC16);
		if (!boResult)
		{
			free(aucDataBuffer);
			FreeMotFile(ptMotFile);
			ptMotFile = NULL;
			return 0;
		}
	
		*pusCRC16 = (unsigned short)ulCRC16;
		
		free(aucDataBuffer);
	}
	
	if (!patStaticBlockArray && !puBlockCount)
	{ // Dann nur die CRC16
		FreeMotFile(ptMotFile);
		ptMotFile = NULL;
		return 1;
	}
	
	/* Blöcke suchen und CRC berechnen */
	
	memset(atBlocks, 0, uBlocksCount * sizeof(TMotBlock));
	aucDataBuffer = (unsigned char *)malloc(INFOBLOCKSIZE);
	ulSearchStartAddress = ulLowestAddress;
	
	while (boFindSequence(ptMotFile, "GRMSTRM\0", 8, ulSearchStartAddress, ulHighestAddress, &ulMatchAddress))
	{
		memset(aucDataBuffer, 0, INFOBLOCKSIZE);
		
		if (!boReadFromAddress(ptMotFile, ulMatchAddress, INFOBLOCKSIZE, aucDataBuffer))
		{ // Irgendwas passt nicht, z. B. keine zusammenhängenden Daten
			ulSearchStartAddress = ulMatchAddress + 1;	
			continue;
		}
		
		if (memcmp(aucDataBuffer + 12, "\xAA\x5A\xA5\x55", 4) != 0)
		{ // Ist kein valider Infoblock
			ulSearchStartAddress = ulMatchAddress + 1;
			continue;
		}
		
		if (uBlockCount >= uBlocksCount)
		{ // Array vergrößern / erstellen
			if (atBlocks)
			{ // vergrößern
				atBlocks = (TMotBlock *)realloc(atBlocks, (uBlocksCount + MOTBLOCKESTIMATEDCOUNT) * sizeof(TMotBlock));
				memset(atBlocks + uBlocksCount, 0, MOTBLOCKESTIMATEDCOUNT * sizeof(TMotBlock));
				uBlocksCount += MOTBLOCKESTIMATEDCOUNT;
			}
			else
			{ // erstellen
				atBlocks = (TMotBlock *)malloc(MOTBLOCKESTIMATEDCOUNT * sizeof(TMotBlock));
				memset(atBlocks, 0, MOTBLOCKESTIMATEDCOUNT * sizeof(TMotBlock));
				uBlocksCount = MOTBLOCKESTIMATEDCOUNT;
			}
		}
		
		boBE2LE(4, aucDataBuffer + 16, (unsigned char *)&atBlocks[uBlockCount].ulCRCStart);
		boBE2LE(4, aucDataBuffer + 20, (unsigned char *)&atBlocks[uBlockCount].ulCRCEnd);
		boBE2LE(4, aucDataBuffer + 28, (unsigned char *)&atBlocks[uBlockCount].ulCRCField);
		atBlocks[uBlockCount].ulCRCTargetAddress = ulMatchAddress + 28;
		
		memcpy(atBlocks[uBlockCount].szProject, aucDataBuffer + 40, 8);
		memcpy(atBlocks[uBlockCount].szBlockname, aucDataBuffer + 48, 12);
		sprintf(atBlocks[uBlockCount].szVersion, "%d.%d.%d", aucDataBuffer[68], aucDataBuffer[69], aucDataBuffer[70]);
		sprintf(atBlocks[uBlockCount].szDate, "%d.%d.%d", aucDataBuffer[63], aucDataBuffer[62], aucDataBuffer[61] + 2000);
		
		if (atBlocks[uBlockCount].ulCRCEnd == 0x00000000)
		{ // Error Block!
			ulSearchStartAddress = ulMatchAddress + 1;
			continue; // Nicht hochzählen!
		}
		
		boCalculateBlockCRC32(ptMotFile, &atBlocks[uBlockCount]);
		boCalculateBlockCRC16(ptMotFile, atBlocks[uBlockCount].ulCRCStart, atBlocks[uBlockCount].ulCRCEnd, &atBlocks[uBlockCount].usCRC16);
		
		ulSearchStartAddress = atBlocks[uBlockCount].ulCRCEnd + 1;
		uBlockCount++;
	}
	
	free(aucDataBuffer);
	
	if (puBlockCount) *puBlockCount = uBlockCount;
	if (patStaticBlockArray) *patStaticBlockArray = atBlocks;
	
	/* Auf leere Blöcke checken */
	
	if (uBlockCount == 0)
	{
		if (pboNoDataOutsideBlocks) *pboNoDataOutsideBlocks = 0;
	}
	else if (pboNoDataOutsideBlocks)
	{
		*pboNoDataOutsideBlocks = 1;
		if (atBlocks[0].ulCRCStart > ulLowestAddress)
		{ // Block am Anfang
			boResult = 0;
			aucDataBuffer = (unsigned char *)malloc(atBlocks[0].ulCRCStart - ulLowestAddress);
			memset(aucDataBuffer, 0, atBlocks[0].ulCRCStart - ulLowestAddress);
			if (boReadFromAddressOmitEmpty(ptMotFile, ulLowestAddress, atBlocks[0].ulCRCStart - ulLowestAddress, aucDataBuffer, &ulRealDataLength))
			{
				boResult = boCheckEmpty(ulRealDataLength, aucDataBuffer);
			} // Wenns nicht geht, dann halt nicht... dann gehen wir davon aus, dass es leer ist...
			free(aucDataBuffer);
			if (!boResult)
			{
				*pboNoDataOutsideBlocks = 0;
			}
		}
	
		for (uCurrentBlock = 0; uCurrentBlock < uBlockCount-1; uCurrentBlock++)
		{ // Alles zwischen zwei Blöcken
			if (atBlocks[uCurrentBlock+1].ulCRCStart > atBlocks[uCurrentBlock].ulCRCEnd+1)
			{ // Block am Anfang
				boResult = 0;
				aucDataBuffer = (unsigned char *)malloc(atBlocks[uCurrentBlock+1].ulCRCStart - (atBlocks[uCurrentBlock].ulCRCEnd+1));
				memset(aucDataBuffer, 0, atBlocks[uCurrentBlock+1].ulCRCStart - (atBlocks[uCurrentBlock].ulCRCEnd+1));
				if (boReadFromAddressOmitEmpty(ptMotFile, atBlocks[uCurrentBlock].ulCRCEnd+1, atBlocks[uCurrentBlock+1].ulCRCStart - (atBlocks[uCurrentBlock].ulCRCEnd+1), aucDataBuffer, &ulRealDataLength))
				{
					boResult = boCheckEmpty(ulRealDataLength, aucDataBuffer);
				} // Wenns nicht geht, dann halt nicht... dann gehen wir davon aus, dass es leer ist...
				free(aucDataBuffer);
				if (!boResult)
				{
					*pboNoDataOutsideBlocks = 0;
				}
			}
		}
		
		if (atBlocks[uBlockCount-1].ulCRCEnd < ulHighestAddress)
		{ // Block am Ende
			boResult = 0;
			aucDataBuffer = (unsigned char *)malloc(ulHighestAddress - (atBlocks[uBlockCount-1].ulCRCEnd+1));
			memset(aucDataBuffer, 0, ulHighestAddress - atBlocks[uBlockCount-1].ulCRCEnd+1);
			if (boReadFromAddressOmitEmpty(ptMotFile, atBlocks[uBlockCount-1].ulCRCEnd+1, ulHighestAddress - (atBlocks[uBlockCount-1].ulCRCEnd+1), aucDataBuffer, &ulRealDataLength))
			{
				boResult = boCheckEmpty(ulRealDataLength, aucDataBuffer);
			} // Wenns nicht geht, dann halt nicht... dann gehen wir davon aus, dass es leer ist...
			free(aucDataBuffer);
			if (!boResult)
			{
				*pboNoDataOutsideBlocks = 0;
			}
		}
	
	}
	
	/* Ende Gelände */
	
	FreeMotFile(ptMotFile);
	ptMotFile = NULL;
	return 1;
}


