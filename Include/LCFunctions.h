#ifdef _CVI_DEBUG_
//#define _NO_HARDWARE_
#endif



typedef struct tagMES_SETUP{
	char szUnitIdType[256];						// Unit ID Type
	char szOrder[256];							// Order
	char szMaterial[256];						// Material configuration
	char szBatchUnitId[2][256];					// Good receipt IDs
	char szTestPlanName[256];					// Testplan name
	char szTestProgramName[256];				// Program name
	char szTestProgramType[256];				// Program type
	char szTestProgramVersion[256];			// Program version
	short sSkipPartNumber;							// Skip first part number characters
	short sOperation;								// Current operation number
	short sMaxSlotUnits;							// Max slot for units
	short sComponentCounter;						// Counter for control 2 COMPONENT_VERIFY/ACTIVE inputs
	int sOrderLeftQty;							//PO left qty
	char szSEMIComponent[100];					 //SEMI Material BOM
	BOOL bShowTests;								// Show test
	BOOL bUseLineControl;							// Get info from LineControl
}MES_SETUP, *LPMES_SETUP;



MES_SETUP gMES;




char lcErrorCode[256];

short ReadCfg(void);
short InitLCR(char *szDummy);
int getLineControlInfo(char *szErrorMsg);
int CheckNewRouting(char iOrder[]) ;
int ValidateCreateUnit(char unitSN[],char *szErrorMsg);
