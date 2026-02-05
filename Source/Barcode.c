#include <windows.h>    
#include <cvitypes.h>
#include <utility.h>
#include <ansi_c.h>
#include <V24.h>   
#include <ApplicationTools.h>      
#include <Barcode.h>   

//***************************************************************************************
Boolean boReadBarcode(int iScannerPort, int iBcLenToScan, char *aBarCode)
{
	V24HANDLE hScanner_V24 = NULL;
	char szSend[] = "<s>";
	int iCount        = strlen(szSend);
	int iRecCnt       = 0;
	double dStartTime  = 0.0, dCheckTime  = 0.0;
	char szBarcode[100] = {0};
	Boolean bBarcodeRead = False;

	// Schnittstelle úffnen
	hScanner_V24 = OpenV24(iScannerPort);
	
	if( hScanner_V24 == NULL ) {
		return( False );
	}
	// Schnittstelle konfigurieren
	if( !ConfigV24(hScanner_V24, 115000, paNone, 8, 1) ) {
		CloseV24(hScanner_V24);
		hScanner_V24 = NULL;
		return( False );
	}
	// Sende und Empfangspuffer lúschen 
	if( !ClearBuffer(hScanner_V24) ) {
		CloseV24(hScanner_V24);
		hScanner_V24 = NULL;
		return( False );
	}
	
	// Triggerzeichen senden
	if( !SendV24(hScanner_V24, szSend, iCount) ) {
		CloseV24(hScanner_V24);
		hScanner_V24 = NULL;
		return( False );
	}
	Sleep(100);
	
	// String vom Scanner hat mit CheckDigit und CR (0x0D) 12 Zeichen
	dStartTime = Timer();
	memset(szBarcode,0,sizeof(szBarcode));
	do {
		iRecCnt = ReceiveCount(hScanner_V24);
		if( iRecCnt >= iBcLenToScan ) {
			Sleep(100);  
			iRecCnt = ReceiveCount(hScanner_V24);   
			if( !ReadV24(hScanner_V24, &szBarcode, iRecCnt) ) {
				CloseV24(hScanner_V24);
				hScanner_V24 = NULL;
				return( False );
			}
			bBarcodeRead = True; 
		}
		dCheckTime = Timer();
		Sleep(10);
	} 
	while( dCheckTime - dStartTime < 5.0 && !bBarcodeRead);
	if(	iRecCnt == 4){	
	if( !ReadV24(hScanner_V24, &szBarcode, iRecCnt) ) {
				CloseV24(hScanner_V24);
				hScanner_V24 = NULL;
				return( False );
	}}	
	if(iRecCnt != 4 && iRecCnt < iBcLenToScan){
		CloseV24(hScanner_V24); 
		return( False );	
	}
	 
	//Schlie t die serielle Schnittstelle
	CloseV24(hScanner_V24);
	szBarcode[iBcLenToScan] = '\0' ;
	strcpy(aBarCode, szBarcode);
	return( True );
}

