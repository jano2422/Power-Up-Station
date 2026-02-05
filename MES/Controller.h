extern short __stdcall con_GetOrderPanel(char *szOrder);
extern short __stdcall con_GetOperationPanel(short sCount,char *szMaterial,struct wpm_structordoprtyp *pOperationList,struct wpm_structmaterialtyp *pMaterialList);
extern short __stdcall con_Init(short boStartPnl);
extern short __stdcall con_Exit(void);
