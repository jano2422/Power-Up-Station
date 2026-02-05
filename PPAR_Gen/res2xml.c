//==============================================================================
//
// Title:       res2xml.c
// Purpose:     A short description of the implementation.
//
// Created on:  24.03.2010 at 10:25:09 by Continental AG.
// Copyright:   Continental AG. All Rights Reserved.
//
//==============================================================================
/*
	V1.00: Initialversion
*/

/*
	Konvertiert ein Text-Resultfile in ein XML-File
*/

#include "toolbox.h"
#include <ansi_c.h>
#include "res2xml.h"


#define LINE_BUFFER_SIZE 4096
#define TAG_BUFFER_SIZE 128

// Static vars


// SRC
int boRes2XML(const char * szResultFile, const char * szTargetXMLFile)
{ // Es muss nicht schön sein, es muss nur funzen...
	FILE * fileTargetXML = NULL;
	TGenericFile * ptResultFile = NULL;
	int boResult = 0;
	char szLineBuffer[LINE_BUFFER_SIZE];
	char szGroupTag[TAG_BUFFER_SIZE];
	char szCurrentTag[TAG_BUFFER_SIZE];
	
	ptResultFile = ptLoadFile(szResultFile);
	
	if (!ptResultFile)
	{ // Datei gibts nicht oder kann nicht geladen werden
		return 0;
	}
	
	fileTargetXML = fopen(szTargetXMLFile, "w+");
	if (!fileTargetXML)
	{
		FreeFile(ptResultFile);
		return 0;
	}
	
	memset(szLineBuffer, 0, sizeof(szLineBuffer));
	memset(szGroupTag, 0, sizeof(szGroupTag));
	memset(szCurrentTag, 0, sizeof(szCurrentTag));
	
	fprintf(fileTargetXML, "<ResultFile>\n");
	
	while (!boAtEOF(ptResultFile))
	{
		// Zum ersten Wort springen
		boResult = boGoToFollowingWordStart(ptResultFile);
		if (!boResult)
		{ // Es gibt kein nächstes Wort -> EOF?
			break;
		}
		
		// Erstes Wort lesen -> Tag
		boResult = boReadString(ptResultFile, sizeof(szCurrentTag), szCurrentTag, 0); // Nicht moven, sonst kann ich die Kommentare net gscheid rausklauben...
		if (!boResult)
		{ // Fehler
			break;
		}
		
		// FORMATFEHLER KOMPENSIEREN!!
		if (szCurrentTag[strlen(szCurrentTag)-1] == ':') szCurrentTag[strlen(szCurrentTag)-1] = '\0';
		
		if (strncmp(szCurrentTag, "//", 2) == 0)
		{ // Kommentare stinken!!
			boResult = boSeekFileRelative(ptResultFile, 2);
			if (!boResult)
			{ // Narf
				break;
			}
			
			boResult = boReadLine(ptResultFile, sizeof(szLineBuffer), szLineBuffer, 1);
			if (!boResult)
			{ // ...
				break;
			}
			
			fprintf(fileTargetXML, "<!-- %s -->\n", szLineBuffer); // Für ungültige Zeichen keine Pump Gun... äh... Gewähr.
		}
		else
		{
			// Zum nächsten Wort springen und Zeile Lesen -> Daten
			boResult = boGoToThisLinesNextWordStart(ptResultFile);
			if (!boResult)
			{ // Kein Wort mehr in der Zeile: GroupTag
				if (*szGroupTag)
				{
					fprintf(fileTargetXML, "</%s>\n", szGroupTag);
				}
				strcpy(szGroupTag, szCurrentTag);
				fprintf(fileTargetXML, "<%s>\n", szGroupTag);
			}
			else
			{ // Zeile Lesen
				boResult = boReadLine(ptResultFile, sizeof(szLineBuffer), szLineBuffer, 1);
				if (!boResult)
				{ // -.-
					break;
				}
				// XML schreiben
				fprintf(fileTargetXML, "<%s>%s</%s>\n", szCurrentTag, szLineBuffer, szCurrentTag);
			}
		}
		// Zur nächsten Zeile springen
		boResult = boGoToNextLineStart(ptResultFile);
	}
	
	boResult = 1;
	
	if (*szGroupTag)
	{
		fprintf(fileTargetXML, "</%s>\n", szGroupTag);
	}
	
	// KONSTANTE PPAR WERTE!!!
	//fprintf(fileTargetXML, "<QSTATUS><R_VGA1>128</R_VGA1><R_VGA2>128</R_VGA2></QSTATUS>\n");
	
	fprintf(fileTargetXML, "</ResultFile>\n");
	
	if (fflush(fileTargetXML))
	{
		boResult = 0;
	}
	
	if (fclose(fileTargetXML))
	{
		boResult = 0;
	}
	
	FreeFile(ptResultFile);
	
	return boResult;
}
