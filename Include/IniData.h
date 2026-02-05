/*****************************************************************************/
/*                                                                           */
/*  Header : IniData.H                                                       */
/*                                                                           */
/*****************************************************************************/

#if !defined ( 	__INI_DATA_H)
#define 		__INI_DATA_H


/*****************************************************************************/
extern Boolean boIniOpen(char aFileName[]);
extern Boolean boIniClose(void);
extern Boolean boIniGetFloatItem (char szSectionName[], char szItemName[], float *fValue);
extern Boolean boIniGetStringItem (char szSectionName[], char szItemName[], char *szValue);

extern Boolean boIniSetFloatItem (char szSectionName[], char szItemName[], float fValue);
extern Boolean boIniSetStringItem (char szSectionName[], char szItemName[], char szValue[]);

extern Boolean boIniNewSection (char szSectionName[]);
extern Boolean boIniNewFloatItem (char szSectionName[], char szItemName[], float fValue);
extern Boolean boIniNewStringItem (char szSectionName[], char szItemName[], char szValue[]);

/*****************************************************************************/
#endif
