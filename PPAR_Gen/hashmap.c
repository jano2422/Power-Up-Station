#include "hashmap.h"
#include <ansi_c.h>

/*!
	@brief Erstellt eine neue Hashmap im Heap.
	
	@param[in] uArraySize Länge des Basisarrays; darf nicht 0 sein.
	@param[in] HashFunction Hashfunktion, die genutzt werden soll.
	
	@return Neu erstellte Hashmap, NULL bei Fehler
*/
THashmap * NEW_Hashmap(unsigned int uArraySize, FHash HashFunction)
{
	THashmap * tHashmap;
	
	if (uArraySize == 0) return NULL;
	if (!HashFunction) return NULL;
	
	tHashmap = (THashmap *)malloc(sizeof(THashmap));
	tHashmap->HashFunction = HashFunction;
	tHashmap->uArraySize = uArraySize;
	tHashmap->aptHashmapList = (THashmapList **)malloc(sizeof(THashmapList *) * uArraySize);
	memset(tHashmap->aptHashmapList, 0, sizeof(THashmapList *) * uArraySize);
	
	return tHashmap;
}

/*!
	@brief Leert eine Hashmap und legt ggf. eine neue Basisarray-Größe fest.
	
	@param[in,out] ptHashmap Hashmap, auf die die Operation ausgeführt werden soll.
	@param[in] uArraySize Neue Länge des Basisarrays, 0 zum behalten.
*/
void CLEAR_Hashmap(THashmap * ptHashmap, unsigned int uArraySize)
{
	int iIndex;
	THashmapList * ptElement;
	for (iIndex = 0; iIndex < ptHashmap->uArraySize; iIndex++)
	{
		while (ptHashmap->aptHashmapList[iIndex])
		{
			ptElement = ptHashmap->aptHashmapList[iIndex];
			ptHashmap->aptHashmapList[iIndex] = ptElement->ptNext;
			ptElement->ptNext = NULL;
			ptElement->pData = NULL; // Nur Link löschen, nicht freigeben!
			free(ptElement->szKey);
			ptElement->szKey = NULL;
			free(ptElement);
		}
	}
	if (uArraySize)
	{
		free(ptHashmap->aptHashmapList);
		ptHashmap->uArraySize = uArraySize;
		ptHashmap->aptHashmapList = (THashmapList **)malloc(sizeof(THashmapList *) * uArraySize);
		memset(ptHashmap->aptHashmapList, 0, sizeof(THashmapList *) * uArraySize);
	}
}

/*!
	@brief Löscht eine Hashmap.
	
	Die Variable (Zeiger auf Hashmap) wird zusätzlich auf NULL gesetzt.
	
	@param[in,out] pptHashmap Adresse des Zeigers auf die Hashmap, die gelöscht werden soll.
*/
void DESTOY_Hashmap(THashmap ** pptHashmap)
{
	CLEAR_Hashmap(*pptHashmap, 0);
	free((*pptHashmap)->aptHashmapList);
	(*pptHashmap)->uArraySize = 0;
	free(*pptHashmap);
	*pptHashmap = NULL;
}

/*!
	@brief Fügt der Hashmap ein Element hinzu.
	
	@param[in,out] ptHashmap Hashmap, auf die die Operation ausgeführt werden soll.
	@param[in] szKey Schlüsselwert des neuen Eintrags.
	@param[in] pData Zeiger auf den Datenwert. Die Daten werden nur verlinkt!
*/
void Hashmap_AddElement(THashmap * ptHashmap, const char * szKey, void * pData)
{
	THashmapList * ptElement;
	unsigned int iIndex = ptHashmap->HashFunction(szKey, ptHashmap->uArraySize);
	ptElement = ptHashmap->aptHashmapList[iIndex];
	while (ptElement)
	{
		if (strcmp(szKey, ptElement->szKey) == 0)
		{ // Gefunden -> Ersetzen
			ptElement->pData = pData; // Nur der Link wird geändert, die Daten bleiben da!
			return;
		}
		ptElement = ptElement->ptNext;
	}
	// Element vorne einfügen
	ptElement = (THashmapList *)malloc(sizeof(THashmapList));
	ptElement->ptNext = ptHashmap->aptHashmapList[iIndex];
	ptElement->szKey = (char *)malloc(sizeof(char) * (strlen(szKey)+1));
	strcpy(ptElement->szKey, szKey);
	ptElement->pData = pData;
	ptHashmap->aptHashmapList[iIndex] = ptElement;
}

/*!
	@brief Liest ein Element aus der Hashmap.
	
	@param[in] ptHashmap Hashmap, auf die die Operation ausgeführt werden soll.
	@param[in] szKey Schlüsselwert des Eintrags.
	
	@return Zeiger auf den Datenwert (Link), oder NULL, falls kein Eintrag.
*/
void * Hashmap_GetElement(THashmap * ptHashmap, const char * szKey)
{
	THashmapList * ptElement;
	unsigned int iIndex = ptHashmap->HashFunction(szKey, ptHashmap->uArraySize);
	ptElement = ptHashmap->aptHashmapList[iIndex];
	while (ptElement)
	{
		if (strcmp(szKey, ptElement->szKey) == 0)
		{ // Gefunden -> Ausgeben
			return ptElement->pData;
		}
		ptElement = ptElement->ptNext;
	}
	return NULL;
}

/*!
	@brief Löscht ein Element aus der Hashmap, falls vorhanden.
	
	@param[in,out] ptHashmap Hashmap, auf die die Operation ausgeführt werden soll.
	@param[in] szKey Schlüsselwert des zu löschenden Eintrags.
*/
void Hashmap_DeleteElement(THashmap * ptHashmap, const char * szKey)
{
	THashmapList * ptElement;
	THashmapList ** pptBeforeLink;
	unsigned int iIndex = ptHashmap->HashFunction(szKey, ptHashmap->uArraySize);
	pptBeforeLink = &ptHashmap->aptHashmapList[iIndex];
	ptElement = ptHashmap->aptHashmapList[iIndex];
	while (ptElement)
	{
		if (strcmp(szKey, ptElement->szKey) == 0)
		{ // Gefunden -> Löschen
			ptElement->ptNext = NULL;
			ptElement->pData = NULL; // Nur Link löschen, nicht freigeben!
			free(ptElement->szKey);
			ptElement->szKey = NULL;
			free(ptElement);
			*pptBeforeLink = ptElement->ptNext;
			return;
		}
		pptBeforeLink = &ptElement->ptNext;
		ptElement = ptElement->ptNext;
	}
	// Nix gefunden
}

/* HASH FUNCTIONS */

/*!
	@brief Ermittelt den Hash über das erste Zeichen.
	Einfache Modulo-Operation des ersten Zeichens.
	
	@param[in] szKey Schlüsselwert, aus dem der Hash berechnet wird.
	@param[in] uArraySize Größe des Basisarrays und damit erster zu verbotener Ausgabewert.
	
	@return Wert im Bereich [0;uArraySize-1]
*/
unsigned int Hash_Firstchar(const char * szKey, unsigned int uArraySize)
{
	return (*szKey % uArraySize);
}

/*!
	@brief Ermittelt den Hash über die Summe aller Zeichen.
	Modulo-Operation über die Summe aller Zeichenwerte.
	
	@param[in] szKey Schlüsselwert, aus dem der Hash berechnet wird.
	@param[in] uArraySize Größe des Basisarrays und damit erster zu verbotener Ausgabewert.
	
	@return Wert im Bereich [0;uArraySize-1]
*/
unsigned int Hash_Charsum(const char * szKey, unsigned int uArraySize)
{
	const char * pszPos = szKey;
	unsigned int uRet = 0;
	while (*pszPos)
	{
		uRet = (uRet + *pszPos) % uArraySize;
		pszPos++;
	}
	return uRet;
}
