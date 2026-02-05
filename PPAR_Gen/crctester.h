#ifndef __CRCTESTER_H__
#define __CRCTESTER_H__

extern int boCalculateCRC(int iDLC, unsigned char * aucData, int iOrder, unsigned long ulPolynom, int boDirect, unsigned long ulCRCInit, unsigned long ulCRCXOR, int boRefIn, int boRefOut, unsigned long * pulCRCResult);


#endif//__CRCTESTER_H__
