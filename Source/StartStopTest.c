#include <Windows.h>
#include <userint.h>
#include <CviTypes.h>
#include <ansi_c.h>
#include <utility.h>
#include <definitions.h>
#include <CviLib.h>
#include <ApplicationTools.h>
#include <RunSpsTestprogramm.h> 

#include <HpSupply.h>
#include <6TL_Cards.h>
#include <NI_DMM4070.h>

#include <CAN_Base.h>
#include <FixtureSlot.h>


//************************************************************************************************
char * szHexToString(unsigned char * aucData, unsigned int uSize)
{
	static char szData[3079];
	int iCnt;
	
	memset(szData, 0, sizeof(szData));
	
	if (uSize > 1024)
	{
		uSize = 1024;
		sprintf(szData, "[LIM!]");
	}
	else
	{
		sprintf(szData, "[%d]", uSize);
	}
	
	for (iCnt = 0; iCnt < uSize; iCnt++)
	{
		sprintf(szData, "%s %02X", szData, aucData[iCnt]);
	}
	
	return szData;
}

//************************************************************************************************
Boolean boInitDut(void)
{
	float fCurrent = 0.0;
	float fVoltage = 0.0;
	
	/************************************************/
	// CAN Göpel Card 
	if (!boCanInit())
	{
		WriteToErrorWin("Error on boCanInit");
		return( 0 );
	}
	
	if (!boCanReset())
	{
		WriteToErrorWin("Error on boCanReset");
		return( 0 );
	}
	
	if (FixtureSlot_GetActivePowerSupplyRelay() > 0)
	{
		if (!boSetOnYavRel(CARD904X8_3, FixtureSlot_GetActivePowerSupplyRelay()))
		{
			WriteToErrorWin("Power supply relay seems to be unfunctional!");
			Sleep(20);
			return( False );					
		}

	}
	/************************************************/
	// Power supply
        if (!boHpPsSetOutOn(&g_HpSupplyPrimary, 13.0, 20))
	{
		WriteToErrorWin("Error on boInitDut, boHpPsSetOutOn");
		return( False );
	}
		
	        
		if (!boHpPsSetOutOn(&g_HpSupplySecondary, 13.0, 20))
	{
		WriteToErrorWin("Error on boInitDut, boHpPsSetOutOn");
		return( False );
	}			

	// 6TL Cards
	if (!boResetYavBoard(CARD904X8_1))
	{
		WriteToErrorWin("Error on boInitDut, boResetYavBoard");
		return( False );
	}
		
	if (!boResetYavBoard(CARD904X8_2))
	{
		WriteToErrorWin("Error on boInitDut, boResetYavBoard");
		return( False );
	}

	Sleep(300); // Time for power on of the sensor ? 
	
		// CAN1 Goepel to vehicle CAN switch
	if (!boSetOnYavRel(CARD904X8_1, FixtureSlot_GetActivePublicCanRelay()))
	{
		WriteToErrorWin("Error on boInitDut, boSetOnYavRel");
		return( False );
	}
	
	Sleep(200);
	
	/************************************************/
	// check Spannung und Strom are valid!!
        if( !boHpPsReadVoltage(&g_HpSupplyPrimary, &fVoltage) ){
		WriteToErrorWin("Error on read voltage!");
		return( False );
	}
       if( !boHpPsReadCurrent(&g_HpSupplyPrimary, &fCurrent) ){
		WriteToErrorWin("Error on read current!");
		return( False );
	}
	if( fVoltage < 12.5 || fCurrent > 0.6 ){
		WriteToErrorWin("Error, Power consumption is not valdi! current:%.2f, voltage: %.2f", fVoltage,fCurrent);
		return( False );
	}
	
	        
	if( !boHpPsReadVoltage(&g_HpSupplySecondary, &fVoltage) ){
		WriteToErrorWin("Error on read voltage!");
		return( False );
	}
        if( !boHpPsReadCurrent(&g_HpSupplySecondary, &fCurrent) ){
		WriteToErrorWin("Error on read current!");
		return( False );
	}
	if( fVoltage < 12.5 || fCurrent > 0.6 ){
		WriteToErrorWin("Error, Power consumption is not valdi! current:%.2f, voltage: %.2f", fVoltage,fCurrent);
		return( False );
	}		   
	
	
	return( True );
	
}

//************************************************************************************************
Boolean boResetDut(void)
{
	Boolean boErr = False;
	
	// Power supply
        if( !boHpPsSetVoltage(&g_HpSupplyPrimary, 0.0) ) {
		WriteToErrorWin("Error on boResetDut, boHpPsSetVoltage");
		boErr = True;
	}
	       
		if( !boHpPsSetVoltage(&g_HpSupplySecondary, 0.0) ) {
		WriteToErrorWin("Error on boResetDut, boHpPsSetVoltage");
		boErr = True;
	}
	
	if (!boSetOffYavRel(CARD904X8_3, FixtureSlot_GetActivePowerSupplyRelay()))
    {        
		WriteToErrorWin("Power supply relay seems to be unfunctional!");
		boErr = True;
	}
	Sleep(20);


	// Output lines load off
	if( !boResetYavBoard(CARD904X8_1) ) {
		WriteToErrorWin("Error on boResetDut, boSetOffYavRel");
		boErr = True;
	}
		if( !boResetYavBoard(CARD904X8_2) ) {
		WriteToErrorWin("Error on boResetDut, boSetOffYavRel");
		boErr = True;
	}
	
			
		if( !boResetYavBoard(CARD904X8_3) ) {
		WriteToErrorWin("Error on boResetDut, boSetOffYavRel");
		boErr = True;
	}
	
	
	
	if( boErr == True ) {
		return( False );
	}
	
	return( True );
}




