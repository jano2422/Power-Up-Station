#ifndef __STRLIB_H
#define __STRLIB_H




int strend(char * s, char * end);
int striend(char * s, char * end);
int strstart(char * s, char * start);
int stristart(char * s, char * start);


unsigned char ucHexToNibble(char cData);
unsigned char ucHexToChar(const char * szData);
unsigned long ulHexToLong(const char * szData);


#endif//__STRLIB_H
