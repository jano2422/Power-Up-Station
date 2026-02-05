/********************************************************************************/
/*                                                                           		*/
/*  Header File: XcpViaCan.h				                 		                     		*/
/*                                                                           		*/
/********************************************************************************

	Version 		Date				Author		 Changes
	------------------------------------------------
	V1.00		   16.09.2011   fehrmanng	 Initial realization
*********************************************************************************

	Documentation:
	--------------
	Driver layer for XCP-Commands via Goepel-Can Hardware (G_Api)
	Based on XcpCan_V105.c by DunzM

*********************************************************************************/
#ifndef __XCP_CAN_H
#define __XCP_CAN_H


//************************************************************************************************
#define CanShowError(DUMMY) CanShowErrorNew(__FILE__,__LINE__)


//************************************************************************************************
typedef enum {
	eBaudRate500K 			= 0xF1,
	eBaudRate1M 				= 0xF2
} EBaudRate;


//************************************************************************************************
//************************************************************************************************
extern void CanShowErrorNew(char szFile[], int iLine);
extern void CanWriteError(char szError[]);

extern char *pHex2String(unsigned char uszHex[], int iAnz);

extern int iXcpOpen(const unsigned int hCanPort, unsigned int uBaudRate);
extern int iXcpClose(void);

extern int iXcpInit(unsigned int uCtoId, unsigned int uDtoId, Boolean boExtendedIds);
extern int iXcpReset(void);

extern int iXcpChangeBaudRate(EBaudRate eBaud);

extern int iXcpReq(unsigned char uszReq[]);
extern int iXcpReqRes(unsigned char uszReq[], unsigned char *uszRes);

extern int iXcpConnect(void);
extern int iXcpDisConnect(void);

extern int iXcpSetMta(unsigned int uAdr);
extern int iXcpUploadWord(unsigned char aucRxBuff[]);


//************************************************************************************************
#endif
