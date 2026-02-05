/*  

Functions for reading an XML-File
===================================

This functions uses the NI libraries "cvixml" and "toolbox". The Functionpanels (Instrument Files)
are necessary in the project file.


Functions:

int iOpenXmlFile (char *sXmlFilePath, CVIXMLDocument *hXmlFileHandle);
int iCloseXmlFile (CVIXMLDocument hXmlFileHandle);
int iReadUniqueXmlStringElement (char **pszElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle); // Free memory after use!
int iReadUniqueXmlDoubleElement (double *pdElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle);
int iReadUniqueXmlIntElement (int *piElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle);
int iReadUniqueXmlDoubleArrayElement (double **padElementValues, int *piNoOfValues,  char *szElementName, CVIXMLDocument hXmlFileHandle); // Free memory after use!
int iReadUniqueXmlIntArrayElement (int **paiElementValues, int *piNoOfValues,  char *szElementName, CVIXMLDocument hXmlFileHandle); // Free memory after use!

Arrays as the result of this functions are made with "calloc" and need a "free" after the use of the data!!! 
The memory is only allocated if the result of the function is XML_FILE_OK.


				
V1.00 19.07.2011  Friedrich   New created         
V1.01 25.06.2014  Anane/Höpp	Possibility to detect and interprete hex values in integer fields
*/		


#include <ansi_c.h>
#include "ReadXmlFile.h"


// String functions
// ================

/// HIFN Converts an string without leading characters to a hex value
/// HIPAR str/String to be converted; must not contain separating spaces
/// HIRET Numeric representation of the hexadecimal string
unsigned long hextoint(const char * str)
{
	unsigned long ulResult = 0;
	
	// Strip whitespaces
	while (isspace(*str)) str++;
	
	while ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F'))
	{
		ulResult = ulResult << 4;
		if (*str >= '0' && *str <= '9')
		{
			ulResult |= (*str - '0');
		}
		else if (*str >= 'a' && *str <= 'f')
		{
			ulResult |= (*str - 'a' + 10);
		}
		else if (*str >= 'A' && *str <= 'F')
		{
			ulResult |= (*str - 'A' + 10);
		}
		str++;
	}
	
	return ulResult;
}

/// HIFN Converts an string with optional leading spaces to an integer value
/// HIFN Recognizes prefixed hexadecimal values
/// HIPAR str/String to be converted
/// HIRET Numeric representation of the string
signed long strtoint(const char * str)
{
	signed long lResult = 0;
	int iPrefix = 0;
	const char * strstart = str;
	
	// Strip whitespaces
	while (isspace(*str)) str++;
	
	if (*str == '+')
	{
		iPrefix = +1;
		str++;
	}
	else if (*str == '-')
	{
		iPrefix = -1;
		str++;
	}
	else if (*str == 'x' || *str == 'h' || *str == '#' || *str == '$')
	{
		str++;
		return (signed long)hextoint(str);
	}
	else if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X'))
	{
		str+=2;
		return (signed long)hextoint(str);
	}
	
	strstart = str;
	
	if (!iPrefix) iPrefix = +1;
	
	while (*str >= '0' && *str <= '9')
	{
		lResult = lResult * 10 + *str - '0';
		str++;
	}
	
	if ((*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F') || *str == 'x' || *str == 'h')
	{ // Intermediate or suffix
		return (signed long)hextoint(strstart); // Back to start
	}
	
	return iPrefix * lResult;
}


// Open an existing XML-File
// ==========================



int iOpenXmlFile (char *szXmlFilePath, CVIXMLDocument *phXmlFileHandle)
{
	return (CVIXMLLoadDocument (szXmlFilePath, phXmlFileHandle) != 0) ? XML_FILE_ERROR_FILE_NOT_FOUND : XML_FILE_OK;
} 
	
// ------------------------------------------------------------------------------------------------


// Close an XML-File
// =================

int iCloseXmlFile (CVIXMLDocument hXmlFileHandle)
{
	CVIXMLDiscardDocument (hXmlFileHandle);
	return XML_FILE_OK;
}

// ------------------------------------------------------------------------------------------------


// Read one unique Element (String value) from the XML-File
// ========================================================

// The element name has to be unique in the whole file!
// In this function memory is allocated for the output string.
// It is necessary to free the memory after usage of the data.

int iReadUniqueXmlStringElement (char **pszElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle)
{
	int iError = -999, iNumElements = 0, iIndex = 0, iValueLength = 0;
	CVIXMLElement Element = 0, RootElement = 0;
	ListType ElementList = 0;
	
	*pszElementValue = NULL;
	
	if (CVIXMLGetRootElement (hXmlFileHandle, &RootElement) != 0)
	{
		iError =  XML_FILE_ERROR_ROOT_NOT_FOUND;
	}
	else
	{ 
		if (CVIXMLFindElements (RootElement, szElementName, &ElementList) != 0)
		{
			iError = XML_FILE_ERROR_FIND;
		}
		else
		{
			iNumElements = ListNumItems (ElementList);
			
			if (iNumElements != 1)
			{
				iError = (iNumElements == 0) ? XML_FILE_ERROR_ELEMENT_NOT_FOUND : XML_FILE_ERROR_NOT_UNIQUE;
			}
			else
			{
				ListGetItem (ElementList, &Element, 1);
				
				if (CVIXMLGetElementValueLength (Element, &iValueLength) != 0)
				{
					iError = XML_FILE_ERROR_GET_LEN;
				} 
				else
				{
					
					if ((*pszElementValue = (char *) calloc (iValueLength + 1, sizeof (char))) == NULL)
					{
						iError = XML_FILE_ERROR_NO_MEMORY;
					} 
					else
					{
						if (CVIXMLGetElementValue (Element, *pszElementValue) != 0)
						{
							iError = XML_FILE_ERROR_GET_ELEMENT;
							free (*pszElementValue);
							*pszElementValue = NULL;
						}
						else
						{ 
							iError =  XML_FILE_OK;
						} 
					}   
				}   
			}
					
			for (iIndex = 0; iIndex < iNumElements; ++ iIndex)
			{
				ListRemoveItem (ElementList, &Element, FRONT_OF_LIST);
				CVIXMLDiscardElement (Element);
			}

			ListDispose (ElementList);
		}
						
		CVIXMLDiscardElement (RootElement);
	}
	
	return iError;
}

// ------------------------------------------------------------------------------------------------
int iReadWildcardMIPartNumber(char **pszElementValue, CVIXMLDocument hXmlFileHandle)
{
    const char *prefix = "ETP_SRR320MI";
    const char *needle = "PartNumber";

    CVIXMLElement RootElement = 0;
    ListType ElementList = 0;
    CVIXMLElement Element = 0;

    int iError = -999;
    int numElements = 0;
    int matchCount = 0;
    int iValueLength = 0;
    int i;

    char szTag[256] = {0};

    *pszElementValue = NULL;

    // Get root
    if (CVIXMLGetRootElement(hXmlFileHandle, &RootElement) != 0)
        return XML_FILE_ERROR_ROOT_NOT_FOUND;

    // Get ALL descendant elements
    if (CVIXMLFindElements(RootElement, "*", &ElementList) != 0)
    {
        CVIXMLDiscardElement(RootElement);
        return XML_FILE_ERROR_FIND;
    }

    numElements = ListNumItems(ElementList);

    for (i = 1; i <= numElements; i++)
    {
        ListGetItem(ElementList, &Element, i);

        // Get tag of this element
        if (CVIXMLGetElementTag(Element, szTag) != 0)
            continue;

        // Match pattern: ETP_SRR320MI*PartNumber
        if (strncmp(szTag, prefix, strlen(prefix)) == 0 &&
            strstr(szTag, needle) != NULL)
        {
            matchCount++;

            if (matchCount > 1)
            {
                iError = XML_FILE_ERROR_NOT_UNIQUE;
                goto Cleanup;
            }

            // Read its value
            if (CVIXMLGetElementValueLength(Element, &iValueLength) != 0)
            {
                iError = XML_FILE_ERROR_GET_LEN;
                goto Cleanup;
            }

            *pszElementValue = (char *)calloc(iValueLength + 1, sizeof(char));
            if (*pszElementValue == NULL)
            {
                iError = XML_FILE_ERROR_NO_MEMORY;
                goto Cleanup;
            }

            if (CVIXMLGetElementValue(Element, *pszElementValue) != 0)
            {
                free(*pszElementValue);
                *pszElementValue = NULL;
                iError = XML_FILE_ERROR_GET_ELEMENT;
                goto Cleanup;
            }

            iError = XML_FILE_OK;
            // don’t break – we still loop but matchCount>1 would be caught
        }
    }

    if (matchCount == 0 && iError == -999)
        iError = XML_FILE_ERROR_ELEMENT_NOT_FOUND;

Cleanup:
    // Dispose list and elements
    for (i = 0; i < numElements; i++)
    {
        ListRemoveItem(ElementList, &Element, FRONT_OF_LIST);
        CVIXMLDiscardElement(Element);
    }
    ListDispose(ElementList);
    CVIXMLDiscardElement(RootElement);

    return iError;
}




// Read one unique Element (Double value) from the XML-File
// ========================================================

// The element name has to be unique in the whole file!

int iReadUniqueXmlDoubleElement (double *pdElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle)
{
	char *szTextBuffer = NULL;
	int iError = -999;

	iError = iReadUniqueXmlStringElement (&szTextBuffer, szElementName, hXmlFileHandle);
	
	if (iError == XML_FILE_OK)
	{
		*pdElementValue = atof (szTextBuffer);  
		free (szTextBuffer);
	}
	else
	{
		*pdElementValue = 0.0;
	}
		
	return iError;
}


// ------------------------------------------------------------------------------------------------


// Read one unique Element (Int value) from the XML-File
// =====================================================

// The element name has to be unique in the whole file!

int iReadUniqueXmlIntElement (int *piElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle)
{
	char *szTextBuffer = NULL;
	int iError = -999;
	
	iError = iReadUniqueXmlStringElement (&szTextBuffer, szElementName, hXmlFileHandle);
	
	
	if (iError == XML_FILE_OK)
	{ 
		*piElementValue = strtoint (szTextBuffer);
		free (szTextBuffer);
	}
	else
	{
		*piElementValue = 0.0;
	}
	
	return iError;
}


// ------------------------------------------------------------------------------------------------


// Read one unique Element (Double array value) from the XML-File
// ==============================================================

// The element name has to be unique in the whole file!
// In this function memory is allocated for the output data.
// It is necessary to free the memory after usage of the data.



int iReadUniqueXmlDoubleArrayElement (double **padElementValues, int *piNoOfValues,  char *szElementName, CVIXMLDocument hXmlFileHandle)
{
	char *szTextBuffer = NULL, *pcTxtPtr = NULL;
	int iError = -999, i = 0;

	*padElementValues = NULL;
	*piNoOfValues = 0;
	
	
	iError =  iReadUniqueXmlStringElement (&szTextBuffer, szElementName, hXmlFileHandle);

	if (iError == XML_FILE_ERROR_GET_LEN)
	{
		*piNoOfValues = 0;
		*padElementValues = NULL;
		iError = XML_FILE_OK;
	}
	else if (iError == XML_FILE_OK)
	{
		// Count values in string and replace ';' by '\0'
	
		*piNoOfValues = 0;
	
		if (strlen(szTextBuffer) == 0)
		{
			*padElementValues = NULL;
		}
		else
		{
			pcTxtPtr = szTextBuffer;
		
			while (*pcTxtPtr != '\0')
			{
				if (*pcTxtPtr == ';')
				{
					*pcTxtPtr = '\0';
					(*piNoOfValues) ++;
				}
		
				pcTxtPtr ++;
			}
		
			(*piNoOfValues) ++;
		
																									
			// Allocate memory for double array
		
			if ((*padElementValues = (double *) calloc (*piNoOfValues, sizeof(double))) == NULL)
			{
				iError = XML_FILE_ERROR_NO_MEMORY;
			}
			else
			{
			
				// Copy values from string to array
			
				pcTxtPtr = szTextBuffer;
				
				for (i = 0; i < *piNoOfValues; i++)
				{         
					(*padElementValues) [i] = atof (pcTxtPtr);
					
					while (*pcTxtPtr != '\0')
					{
						pcTxtPtr ++;
					}
					
					pcTxtPtr ++;
				}
			}
		}
		
		free (szTextBuffer);
	} 
		
	return iError;
}
	
// ------------------------------------------------------------------------------------------------


// Read one unique Element (Int array value) from the XML-File
// ===========================================================

// The element name has to be unique in the whole file!
// In this function memory is allocated for the output data.
// It is necessary to free the memory after usage of the data.



int iReadUniqueXmlIntArrayElement (int **paiElementValues, int *piNoOfValues,  char *szElementName, CVIXMLDocument hXmlFileHandle)
{
	char *szTextBuffer = NULL, *pcTxtPtr = NULL;
	int iError = -999, i = 0;

	*paiElementValues = NULL;
	*piNoOfValues = 0;
	
	
	iError =  iReadUniqueXmlStringElement (&szTextBuffer, szElementName, hXmlFileHandle);

	if (iError == XML_FILE_ERROR_GET_LEN)
	{
		*piNoOfValues = 0;
		*paiElementValues = NULL;
		iError = XML_FILE_OK;
	}
	else if (iError == XML_FILE_OK)
	{
		// Count values in string and replace ';' by '\0'
	
		*piNoOfValues = 0;
	
		if (strlen(szTextBuffer) == 0)
		{
			*paiElementValues = NULL;
		}
		else
		{
			pcTxtPtr = szTextBuffer;
		
			while (*pcTxtPtr != '\0')
			{
				if (*pcTxtPtr == ';')
				{
					*pcTxtPtr = '\0';
					(*piNoOfValues) ++;
				}
		
				pcTxtPtr ++;
			}
		
			(*piNoOfValues) ++;
		
																									
			// Allocate memory for int array
		
			if ((*paiElementValues = (int *) calloc (*piNoOfValues, sizeof(int))) == NULL)
			{
				iError = XML_FILE_ERROR_NO_MEMORY;
			}
			else
			{
			
				// Copy values from string to array
			
				pcTxtPtr = szTextBuffer;
				
				for (i = 0; i < *piNoOfValues; i++)
				{         
					(*paiElementValues) [i] = strtoint (pcTxtPtr);
					
					while (*pcTxtPtr != '\0')
					{
						pcTxtPtr ++;
					}
					
					pcTxtPtr ++;
				}
			}
		}
	
		free (szTextBuffer);
	} 
		
	return iError;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	





