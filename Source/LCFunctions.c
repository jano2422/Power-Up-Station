#include <formatio.h>
#include "toolbox.h"
#include <ansi_c.h>
#include "linecontrol.h"
#include "lcfunctions.h"
#include "wipmama.h"

short InitLCR(char *szDummy) {

	sprintf(szDummy, "LineControl: No error");
	if (!lcr_Init())
	{
		sprintf(szDummy, "LineControl error: %s (code %d)", lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}
	
	if(!lcr_Connect())
	{
		sprintf(szDummy, "LineControl error: %s (code %d)", 
						lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}

	if(!lcr_LoadSetup())
	{
		sprintf(szDummy, "LineControl error: %s (code %d)", 
						lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}

	if (!lcr_SetSetup(LCR_SETUP_OK))            
	{
		sprintf(szDummy, "LineControl error: %s (code %d)", 
					lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}

	// Get current Order, Material and Unit ID Type from LineControl
	if(!getLineControlInfo(szDummy))
	{
		return 0;
	}
	
	return 1;
}


int getLineControlInfo(char *szErrorMsg)
{
	struct wpm_structordoprtyp *pOperationList = NULL;
	wpm_STRUCTPART2 *pParts;
	long lQty = 0;
	long lLeftQty = 0;
	char szDummy[256] = {0};
	char szTemp[256];
	float f;
	char sTemp[256];
	char szAssMat;
	char szAssMatDescr;
	char **szProbe;
	int cnt = 0,x=0;
	
	// Get current Order, Material from LineControl
	if(!lcr_GetOrder(gMES.szOrder, &gMES.sOperation, &lQty, &lLeftQty))
	{
		sprintf(szErrorMsg, "LineControl error: %s (code %d)", 
						lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}
	else
	{
	  gMES.sOrderLeftQty = lLeftQty;
	}
			

	if(!wpm_GetOperations(gMES.szOrder, &pOperationList))
	{
		sprintf(szErrorMsg, "%s (code %d)", wpm_GetErrorText(wpm_GetErrorCode()), wpm_GetErrorCode());
		return 0;
	
	}else{
		
		gMES.sOperation = pOperationList->sOperation;
	}
	
	
	if(!lcr_GetWipMaterial(gMES.szMaterial, szDummy))
	{
		sprintf(szErrorMsg, "LineControl error: %s (code %d)", 
						lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}
	

	
	if(!lcr_GetAssMaterial(&szAssMat, &szAssMatDescr))
	{
		sprintf(szErrorMsg, "LineControl error: %s (code %d)", 
						lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
		return 0;
	}
	
	//wpm_GetOrderPartList(const char *szOrder, const short sOperation, struct wpm_structpart2 **pParts);
	if((cnt = wpm_GetOrderPartList(gMES.szOrder,500,&pParts))>0)
	{
	for (x=0;x <= cnt;x++)
	 {
	   if (strcmp(pParts[x].szType,"SEMI")==0)
	   {
	       strcpy(gMES.szSEMIComponent,pParts[x].szMaterial);
	   	   break;
	   }
	 
	 }
	}
	else
	{
	  	sprintf(szErrorMsg, "SEMI Component Error: %s (code %d)", wpm_GetErrorText(wpm_GetErrorCode()), wpm_GetErrorCode());
		return 0; 
	}
	
	return 1;
}


int ValidateCreateUnit(char unitSN[],char *szErrorMsg)
{
   short iRetVal=0,iMax = 1;
   char szId[100]={0};
   
   
   strcpy(szId,unitSN);
   
   //lcr_ValidateCreateUnits(char *szID1,       char (*szIDS1)[100], 
                                               //const char *szID2, const char (*szIDS2)[100], 
                                               //short *sMax);
   iRetVal =  lcr_ValidateCreateUnits(szId,0,0,0,&iMax);
   if (!iRetVal)
   {
     sprintf(szErrorMsg, "LineControl error: %s (code %d)", 
						lcr_GetErrorText(lcr_GetErrorCode()), lcr_GetErrorCode());
   
     return 0;
   }
   
	
	
return 1;
}
