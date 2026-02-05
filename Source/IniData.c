#include <CviTypes.h>

#include <utility.h>
#include <userint.h>
#include <inifile.h>

#include <IniData.h>

/*--------------------------------------------------------------------------*/
/* Module-globals                                                           */
/*--------------------------------------------------------------------------*/
Boolean boChangesMade = False;
char    szIniFileName[MAX_PATHNAME_LEN];
IniText myIniFile = 0;

//****************************************************************************************
extern void WriteToErrorWin(char *pFormat, ...);

//****************************************************************************************
Boolean boIniOpen (char aFileName[])
{
	strcpy (szIniFileName, aFileName);
	
	/* Create a new Inifile object and read it from a file */
	if( (myIniFile = Ini_New (1)) == 0 ) {
		MessagePopup("Inifile","Error allocating memory for Inifile");
		return( False );
	}   
	if( Ini_ReadFromFile (myIniFile, szIniFileName) ) {
		MessagePopup("Inifile","Error reading Inifile");
		return( False );
	}   

	return( True );
}

//****************************************************************************************
Boolean boIniClose(void)
{
	/* Write the file back out if the user has made changes */
	if( myIniFile && boChangesMade ) {
		//if( ConfirmPopup ("Save Changes", "Do you want to save changes?") ) {
			Ini_WriteToFile (myIniFile, szIniFileName);
		//}
	}
		
	/* Destroy the Inifile memory object*/
	if( myIniFile ) {
		Ini_Dispose (myIniFile);
		myIniFile = 0;
	}    

	return( True );
}

//****************************************************************************************
Boolean boIniGetFloatItem (char szSectionName[], char szItemName[], float *fValue)
{
	double dValue = 0.0;
	
	*fValue = 0.0;
	if( !Ini_ItemExists (myIniFile, szSectionName, szItemName) ) {
		//MessagePopup ("Inifile", "Error, item do not exist");
		WriteToErrorWin("Error, item do not exist");
		return( False );
	}
	
	if( Ini_GetDouble (myIniFile, szSectionName, szItemName, &dValue) <= 0 ) {
		return( False );
	}
	*fValue = (float)dValue;

	return( True );
}

//****************************************************************************************
Boolean boIniGetStringItem (char szSectionName[], char szItemName[], char *szValue)
{
	if( !Ini_ItemExists (myIniFile, szSectionName, szItemName) ) {
		//MessagePopup ("Inifile", "Error, item do not exist");
		WriteToErrorWin("Error, item do not exist");
		return( False );
	}
	
	if( Ini_GetStringIntoBuffer (myIniFile, szSectionName, szItemName, szValue, 100) <= 0 ) {
		return( False );
	}

	return( True );
}

//****************************************************************************************
Boolean boIniSetFloatItem (char szSectionName[], char szItemName[], float fValue)
{
	if( !Ini_ItemExists (myIniFile, szSectionName, szItemName) ) {
		//MessagePopup ("Inifile", "Error, item do not exist");
		WriteToErrorWin("Error, item do not exist");
	return( False );
	}
	
	if( Ini_PutDouble (myIniFile, szSectionName, szItemName, (double)fValue) != 0 ) {
		return( False );
	}
	boChangesMade = True;

	return( True );
}

//****************************************************************************************
Boolean boIniSetStringItem (char szSectionName[], char szItemName[], char szValue[])
{
	if( !Ini_ItemExists (myIniFile, szSectionName, szItemName) ) {
		//MessagePopup ("Inifile", "Error, item do not exist");
		WriteToErrorWin("Error, item do not exist");
	return( False );
	}
	
	if( Ini_PutString (myIniFile, szSectionName, szItemName, szValue) != 0 ) {
		return( False );
	}
	boChangesMade = True;

	return( True );
}

//****************************************************************************************
Boolean boIniFindSectionName(char szSectionName[])
{
	int  iAnzSections;
	int  iZaehler;
	Boolean boFound;
	char *sectionName;

	iAnzSections = Ini_NumberOfSections (myIniFile);
	boFound = False;
	for( iZaehler = 1; (iZaehler <= iAnzSections) && (!boFound); iZaehler++) {
		if( (Ini_NthSectionName (myIniFile, iZaehler, &sectionName) > 0) 
				&& (!strcmp(sectionName, szSectionName)) ) {
				boFound = True;
		}
	}            

	return( boFound );
}

//****************************************************************************************
Boolean boIniNewSection (char szSectionName[])
{
	if( Ini_SectionExists (myIniFile, szSectionName) ) {
		//MessagePopup("Inifile","Error, section already exists");
		WriteToErrorWin("Error, section already exists");
		return( False );
	}

	/* We have to add a dummy item and then remove to create a new section */
	Ini_PutString (myIniFile, szSectionName, "Dummy_tag_name", "");
	Ini_RemoveItem (myIniFile, szSectionName, "Dummy_tag_name");
	boChangesMade = True;

	return( boIniFindSectionName(szSectionName) );
}

//****************************************************************************************
Boolean boIniFindItemName(char szSectionName[], char szItemName[])
{
	int  iAnzItems;
	int  iZaehler;
	Boolean boFound;
	char *itemName;

	iAnzItems = Ini_NumberOfItems (myIniFile, szSectionName);
	boFound = False;
	for( iZaehler = 1; (iZaehler <= iAnzItems) && (!boFound); iZaehler++ ) {
		if( (Ini_NthItemName (myIniFile, szSectionName, iZaehler, &itemName) > 0) 
				&& (!strcmp (itemName, szItemName)) ) {
			boFound = True;
		}
	}            

	return( boFound );
}

//****************************************************************************************
Boolean boIniNewFloatItem (char szSectionName[], char szItemName[], float fValue)
{
	if( Ini_ItemExists (myIniFile, szSectionName, szItemName) ) {
		//MessagePopup ("Inifile", "Error, item already exists");
		WriteToErrorWin("Error, item already exists");
		return( False );
	}
	
	// Create new Section if it does not already exist
	boIniNewSection (szSectionName);

	Ini_PutDouble (myIniFile, szSectionName, szItemName, (double)fValue);
	boChangesMade = True;

	return( boIniFindItemName(szSectionName, szItemName) );
}

//****************************************************************************************
Boolean boIniNewStringItem (char szSectionName[], char szItemName[], char szValue[])
{
	if( Ini_ItemExists (myIniFile, szSectionName, szItemName) ) {
		//MessagePopup ("Inifile", "Error, item already exists");
		WriteToErrorWin("Error, item already exists");
		return( False );
	}
	
	// Create new Section if it does not already exist
	boIniNewSection (szSectionName);

	Ini_PutString (myIniFile, szSectionName, szItemName, szValue);
	boChangesMade = True;

	return( boIniFindItemName(szSectionName, szItemName) );
}

