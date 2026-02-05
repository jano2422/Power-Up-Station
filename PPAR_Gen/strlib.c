#include "strlib.h"

#include <ansi_c.h>

int strend(char * s, char * end)
{ // Vergleicht das Ende von s mit end. 1 für success, 0 für fail
	int sl = strlen(s);
	int el = strlen(end);
	if (el > sl) return 0;
	return (strcmp(s + sl - el, end) == 0);
}

int striend(char * s, char * end)
{ // Vergleicht das Ende von s mit end, ignore case. 1 für success, 0 für fail
	int sl = strlen(s);
	int el = strlen(end);
	if (el > sl) return 0;
	return (stricmp(s + sl - el, end) == 0);
}

int strstart(char * s, char * start)
{ // Vergleicht den Anfang von s mit start. 1 für success, 0 für fail
	return (strncmp(s, start, strlen(start)) == 0);
}

int stristart(char * s, char * start)
{ // Vergleicht den Anfang von s mit start, ignore case. 1 für success, 0 für fail
	return (strnicmp(s, start, strlen(start)) == 0);
}



unsigned char ucHexToNibble(char cData)
{
	if (cData >= '0' && cData <= '9')
	{
		return (unsigned char)(cData - '0');
	}
	else if (cData >= 'A' && cData <= 'F')
	{
		return (unsigned char)(cData - 'A' + 10);
	}
	else if (cData >= 'a' && cData <= 'f')
	{
		return (unsigned char)(cData - 'a' + 10);
	}
	else
	{
		return 0;	
	}
}

unsigned char ucHexToChar(const char * szData)
{
	return (ucHexToNibble(*szData) << 4) | ucHexToNibble(*(szData + 1));
}


unsigned long ulHexToLong(const char * szData)
{
	unsigned long ulData = 0;
	
	if (szData && *szData)
	{ // NULL? -> 0; Leerstring -> 0; ansonsten:
		// Datenformat
		if (*szData == '0' && *(szData+1) == 'x' || *(szData+1) == 'X')
		{ // 0x...
			szData += 2;
		}
		else if (*szData == '$' || *szData == '#')
		{ // #... / $...
			szData += 1;
		}
		
		while ((*szData >= '0' && *szData <= '9') || (*szData >= 'A' && *szData <= 'F') || (*szData >= 'a' && *szData <= 'f'))
		{
			ulData = (ulData << 4) | ucHexToNibble(*szData);
			szData++;
		}
	}
	
	return ulData;
}

