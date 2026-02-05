#include <cviauto.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include <CarMa.h>
#include <SMR_PCB_TMP_relationship.h>

// Standard Errorausgabe
void WriteToErrorWin(char *pFormat, ...); 

//************************************************************************************************  
//************************************************************************************************  
Boolean boGetCarrierPcbSn(char szTmpSn[],char szPcbSn[])
{
	char szPcbSnType[100];

	memset(szPcbSn, 0, sizeof(szPcbSn));
	memset(szPcbSnType, 0, sizeof(szPcbSnType));  
	
	if( !car_GetCarrierPosition("SMR_TMP_SN", szTmpSn,"PCB_SN_Position",szPcbSnType, szPcbSn )){
		return( False );
	}
	return( True );
}


//************************************************************************************************
Boolean boCarrierStorePcbSn(char szTmpSn[],char szPcbSn[])
{
	char szValue[100];
	char szCarrierType[100]; 
	char szDescription[100];
	
	memset(szValue, 0, sizeof(szValue));
	
	// Do PcbSn already exist?
	if( boGetCarrierPcbSn( szTmpSn, szValue )) {
		if( strcmp(szPcbSn, szValue) != 0 ) {
			WriteToErrorWin("SMR_TMP_SN; %s already exists with PCB_SN: %s ( try to marry with PCB_SN: %s,",szTmpSn,szValue,szPcbSn);
			return( False );
		}
		return( True );
	}
		
	// Do Carrier already exist?
	if( !car_GetCarrierInfo("SMR_TMP_SN", szTmpSn, szCarrierType, szDescription ) ) {
		if( !car_CreateCarrier(	"SMR_PCB_TMP", "SMR_TMP_SN", szTmpSn, "SMR_TMP_SN to PCB_SN relationship") ) {
			return( False );
		}
	}
 	if( !car_CarrierUsePosition("SMR_TMP_SN", szTmpSn, "PCB_SN_Position","SMR_PCB_SN", szPcbSn) ) {
		return( False );
	}

	return( True );
}
//************************************************************************************************         

