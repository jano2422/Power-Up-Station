#ifndef __MOTCRC_H__
#define __MOTCRC_H__

#include "crctester.h"
#include "motfile.h"
#include "strlib.h"


typedef struct
{
	unsigned long ulCRCStart;
	unsigned long ulCRCEnd;
	unsigned long ulCRCTargetAddress;
	unsigned long ulCRCField;
	unsigned long ulCRCReal;
	unsigned short usCRC16;
	char szProject[9];
	char szBlockname[13];
	char szVersion[12]; // Max. "255.255.255\0"
	char szDate[11]; // Max. "31.12.2198\0"
} TMotBlock;



#define MOTBLOCKESTIMATEDCOUNT 5
#define INFOBLOCKSIZE 72

extern int boCalculateCRC32(const char * szFilepath, unsigned long * pucCRC32);
extern int boLE2BE(int iBytes, const unsigned char * aucLittleEndianData, unsigned char * aucBigEndianDataBuffer);
extern int boBE2LE(int iBytes, const unsigned char * aucBigEndianData, unsigned char * aucLittleEndianDataBuffer);
extern int boCheckEmpty(unsigned int uBytes, const unsigned char * aucData);
extern int boCalculateBlockCRC32(TMotFile * ptMotFile, TMotBlock * ptMotBlock);
extern int boCalculateBlockCRC16(TMotFile * ptMotFile, unsigned long ulFirstBlockAddress, unsigned long ulLastBlockAddress, unsigned short * pusBlockCRC16);
extern int boCalculateMOTCRCs(const char * szFilepath, unsigned short * pusCRC16, unsigned int * puBlockCount, TMotBlock ** patStaticBlockArray, unsigned long * pulLowestAddress, unsigned long * pulHighestAddress, int * pboNoDataOutsideBlocks);


#endif//__MOTCRC_H__
