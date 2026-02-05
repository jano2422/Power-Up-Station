#ifndef _READXMLFILE_H_
#define _READXMLFILE_H_


#include "cvixml.h"  // C:\Program Files\National Instruments\CVI90\toolslib\toolbox\cvixml.fp
#include "toolbox.h" // C:\Program Files\National Instruments\CVI90\toolslib\toolbox\toolbox.fp


int iOpenXmlFile (char *sXmlFilePath, CVIXMLDocument *hXmlFileHandle);
int iCloseXmlFile (CVIXMLDocument hXmlFileHandle);
int iReadUniqueXmlStringElement (char **pszElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle); // Free memory after use!
int iReadUniqueXmlDoubleElement (double *pdElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle);
int iReadUniqueXmlIntElement (int *piElementValue, char *szElementName, CVIXMLDocument hXmlFileHandle);
int iReadUniqueXmlDoubleArrayElement (double **padElementValues, int *piNoOfValues,  char *szElementName, CVIXMLDocument hXmlFileHandle); // Free memory after use!
int iReadUniqueXmlIntArrayElement (int **paiElementValues, int *piNoOfValues,  char *szElementName, CVIXMLDocument hXmlFileHandle); // Free memory after use!

// Wildcard reader for Mitsubishi SRR3 ET parameter files:
// Finds exactly one element whose tag starts with "ETP_SRR320MI" and contains "PartNumber",
// then returns its value as a dynamically allocated string (caller must free).
int iReadWildcardMIPartNumber (char **pszElementValue, CVIXMLDocument hXmlFileHandle); // Free memory after use!


// Errorcodes (Results)

#define XML_FILE_OK                        0
#define XML_FILE_ERROR_FILE_NOT_FOUND     -1
#define XML_FILE_ERROR_ROOT_NOT_FOUND     -2
#define XML_FILE_ERROR_FIND               -3
#define XML_FILE_ERROR_ELEMENT_NOT_FOUND  -4
#define XML_FILE_ERROR_NOT_UNIQUE         -5
#define XML_FILE_ERROR_GET_LEN            -6
#define XML_FILE_ERROR_NO_MEMORY          -7
#define XML_FILE_ERROR_GET_ELEMENT        -8

#endif
