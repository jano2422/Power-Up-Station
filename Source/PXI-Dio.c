#include <PXI-Dio.h>
//#include <PXI-Dio-Local.h>
#include <windows.h>

//************************************************************************************************

#define DIO_RESOURCE_NAME	"6528"

#define IOPort0 DIO_RESOURCE_NAME"/port0"
#define IOPort1 DIO_RESOURCE_NAME"/port1"
#define IOPort2 DIO_RESOURCE_NAME"/port2"
#define IOPort3 DIO_RESOURCE_NAME"/port3"
#define IOPort4 DIO_RESOURCE_NAME"/port4"
#define IOPort5 DIO_RESOURCE_NAME"/port5"

//************************************************************************************************
// Globale Variablen
int iDioStatus;
int iSamples;
int iDioPort = 0;

TaskHandle Port0, Port1, Port2, Port3, Port4, Port5;
uInt8 DIO_Data0[8] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
uInt8 DIO_Data1[8] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
uInt8 DIO_Data2[8] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
uInt8 DIO_Read[1] = {0};

//************************************************************************************************
extern Boolean boDioCreateTask (int PortNbr);
extern Boolean boDioStartTask (int PortNbr);
extern Boolean boDioStopTask (int PortNbr);
extern Boolean boDioClearTask (int PortNbr);

//************************************************************************************************
//************************************************************************************************
Boolean boDioInit(void)
{
	int iZaehler = 0;
	
	for( iZaehler = 0; iZaehler < 6; iZaehler++ ) {
		if( !boDioCreateTask(iZaehler) ) {
			return( False );
		}
		if( !boDioStartTask(iZaehler) ) {
			return( False );
		}
	}
	
	return( True );
}

//************************************************************************************************
Boolean boSetOn(int iOutput)
{
	return( boDioWrite( iOutput, 1 ) );
}

//************************************************************************************************
Boolean boSetOff(int iOutput)
{
	return( boDioWrite( iOutput, 0 ) );  	
}

//************************************************************************************************
Boolean boIsSet(int iInput )
{
	int iState;
	boDioRead(iInput, &iState);
	if( iState == On ){
		return( True );
	}
	else{
		return( False );
	}
}


//************************************************************************************************
Boolean boLedOn(int iInput )
{
	int iState;
	boDioRead(iInput, &iState);
	if( iState == On ){
		return( True );
	}
	else{
		return( False );
	}
}

//************************************************************************************************
Boolean boLedOff(int iInput )
{
	int iState;
	boDioRead(iInput, &iState);
	if( iState == Off ){
		return( True );
	}
	else{
		return( False );
	}
}

//************************************************************************************************               
Boolean boWaitUntilOn(int iInput, double dTimeInSeconds )
{
    double dZaehler = 0.0;
   
    while( !boIsSet(iInput) ) 
    {
    	Sleep(90);
    	dZaehler += 0.1;
    	if( dZaehler > dTimeInSeconds ){
    		return( False );
    	}
    }
	return( True );
}

//************************************************************************************************               
Boolean boWaitUntilOff(int iInput, double dTimeInSeconds )
{
    double dZaehler = 0.0;
   
    while( boIsSet(iInput) ) 
    {
    	Sleep(90);
    	dZaehler += 0.1;
    	if( dZaehler > dTimeInSeconds ){
    		return( False );
    	}
    }
	return( False );
}

//************************************************************************************************
Boolean boDioClear(void)
{
	int iZaehler = 0;
	
	iDioStatus = DAQmxWriteDigitalLines (Port3, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																		(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
	if( iDioStatus ) {
		return( False );
	}
	iDioStatus = DAQmxWriteDigitalLines (Port4, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																		(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
	if( iDioStatus ) {
		return( False );
	}
	iDioStatus = DAQmxWriteDigitalLines (Port5, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																		(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
	if( iDioStatus ) {
		return( False );
	}

	for( iZaehler = 0; iZaehler < 6; iZaehler++ ) {
		if( !boDioStopTask(iZaehler) ) {
			return( False );
		}
		if( !boDioClearTask(iZaehler) ) {
			return( False );
		}
	}

	return( True );
}

//************************************************************************************************
Boolean boDioResetAllOut(void)
{
	iDioStatus = DAQmxWriteDigitalLines (Port3, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																		(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
	if( iDioStatus ) {
		return( False );
	}
	iDioStatus = DAQmxWriteDigitalLines (Port4, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																		(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
	if( iDioStatus ) {
		return( False );
	}
	iDioStatus = DAQmxWriteDigitalLines (Port5, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																		(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
	if( iDioStatus ) {
		return( False );
	}
	
	return( True );
}

//************************************************************************************************
Boolean boDioWrite(int LineNbr, int state)
{
	if( LineNbr < 8 ) {
		if( !boDioWritePort(3, LineNbr, state) ) {
			return( False );
		}
	}
	else if( LineNbr < 16 ) {
		if( !boDioWritePort(4, LineNbr - 8, state) ) {
			return( False );
		}
	}
	else if( LineNbr < 24 ) {
		if( !boDioWritePort(5, LineNbr - 16, state) ) {
			return( False );
		}
	}
	else {
		return( False );
	}
	
	return( True );
}

//************************************************************************************************
Boolean boDioRead(int LineNbr, int *state)
{
	if( LineNbr < 8 ) {
		if( !boDioReadPort(0, LineNbr, state) ) {
			return( False );
		}
	}
	else if( LineNbr < 16 ) {
		if( !boDioReadPort(1, LineNbr - 8, state) ) {
			return( False );
		}
	}
	else if( LineNbr < 24 ) {
		if( !boDioReadPort(2, LineNbr - 16, state) ) {
			return( False );
		}
	}
	else {
		return( False );
	}
	
	return( True );
}

//************************************************************************************************
Boolean boDioGetOutputState(byte *OutputState)
{
	int iZaehler = 0;
	
	for( iZaehler = 0; iZaehler < 8; iZaehler++ ) {
		OutputState[iZaehler + 0] = DIO_Data0[iZaehler] == 0x00;
	}
	for( iZaehler = 0; iZaehler < 8; iZaehler++ ) {
		OutputState[iZaehler + 8] = DIO_Data1[iZaehler] == 0x00;
	}
	for( iZaehler = 0; iZaehler < 8; iZaehler++ ) {
		OutputState[iZaehler + 16] = DIO_Data2[iZaehler] == 0x00;
	}

	return( True );
}


/*********************************************************************
*    Create a task.
*    Create a Digital Output channel. Use one channel for all lines.
*********************************************************************/
Boolean boDioCreateTask (int PortNbr)
{
	switch( PortNbr ) {
		//Input	
		case 0:
			iDioStatus = DAQmxCreateTask ("", &Port0);
			if( iDioStatus ) {
				return( False );
			}
			iDioStatus = DAQmxCreateDIChan (Port0, IOPort0, "", DAQmx_Val_ChanForAllLines);
		break;
		case 1:
			iDioStatus = DAQmxCreateTask ("", &Port1);
			if( iDioStatus ) {
				return( False );
			}
			iDioStatus = DAQmxCreateDIChan (Port1, IOPort1, "", DAQmx_Val_ChanForAllLines);
		break;
		case 2:
			iDioStatus = DAQmxCreateTask ("", &Port2);
			if( iDioStatus ) {
				return( False );
			}
			iDioStatus = DAQmxCreateDIChan (Port2, IOPort2, "", DAQmx_Val_ChanForAllLines);
		break;
		//Output	
		case 3:
			iDioStatus = DAQmxCreateTask ("", &Port3);
			if( iDioStatus ) {
				return( False );
			}
			iDioStatus = DAQmxCreateDOChan (Port3, IOPort3, "", DAQmx_Val_ChanForAllLines);
		break;
		case 4:
			iDioStatus = DAQmxCreateTask ("", &Port4);
			if( iDioStatus ) {
				return( False );
			}
			iDioStatus = DAQmxCreateDOChan (Port4, IOPort4, "", DAQmx_Val_ChanForAllLines);
		break;
		case 5:
			iDioStatus = DAQmxCreateTask ("", &Port5);
			if( iDioStatus ) {
				return( False );
			}
			iDioStatus = DAQmxCreateDOChan (Port5, IOPort5, "", DAQmx_Val_ChanForAllLines);
		break;
		default:
			iDioStatus = -9999999999;
		break;
	}
	if( iDioStatus ) {
		return( False );
	}

	return( True );
}

/*********************************************************************
*    Call the Start function to start the task.
*********************************************************************/
Boolean boDioStartTask (int PortNbr)
{
	switch( PortNbr ) {
		//Input	
		case 0:
			iDioStatus = DAQmxStartTask(Port0);
		break;
		case 1:
			iDioStatus = DAQmxStartTask(Port1);
		break;
		case 2:
			iDioStatus = DAQmxStartTask(Port2);
		break;
		//Output	
		case 3:
			iDioStatus = DAQmxStartTask(Port3);
		break;
		case 4:
			iDioStatus = DAQmxStartTask(Port4);
		break;
		case 5:
			iDioStatus = DAQmxStartTask(Port5);
		break;
		default:
			iDioStatus = -9999999999;
		break;
	}
	if( iDioStatus ) {
		return( False );
	}

	return( True );
}

/*********************************************************************
*    Write the digital Boolean array data. This write function
*    writes a single sample of digital data on demand, so no timeout
*    is necessary.
*********************************************************************/
Boolean boDioWritePort(int PortNbr, int LineNbr, int state)
{
	switch( PortNbr ) {
		//Output	
		case 3:
			DIO_Data0[LineNbr] = !state;
			iDioStatus = DAQmxWriteDigitalLines (Port3, 1, 0, 1.0, DAQmx_Val_GroupByChannel, DIO_Data0, &iSamples,NULL);
		break;
		case 4:
			DIO_Data1[LineNbr] = !state;
			iDioStatus = DAQmxWriteDigitalLines (Port4, 1, 0, 1.0, DAQmx_Val_GroupByChannel, DIO_Data1, &iSamples,NULL);
		break;
		case 5:
			DIO_Data2[LineNbr] = !state;
			iDioStatus = DAQmxWriteDigitalLines (Port5, 1, 0, 1.0, DAQmx_Val_GroupByChannel, DIO_Data2, &iSamples,NULL);
		break;
		default:
				iDioStatus = -9999999999;
		break;
	}
	if( iDioStatus ) {
		return( False );
	}

	return( True );
}

/*********************************************************************
*    Read the digital Port
*********************************************************************/
Boolean boDioReadPort(int PortNbr, int LineNbr, int *state)
{
	switch( PortNbr ) {
		//Input
		case 0:
			iDioStatus = DAQmxReadDigitalU8 (Port0, DAQmx_Val_Auto, 1.0, DAQmx_Val_GroupByChannel, DIO_Read, 1,&iSamples, NULL);
			*state = (DIO_Read[0] & (0x01 << LineNbr)) != 0;
		break;
		case 1:
			iDioStatus = DAQmxReadDigitalU8 (Port1, DAQmx_Val_Auto, 1.0, DAQmx_Val_GroupByChannel, DIO_Read, 1,&iSamples, NULL);
			*state = (DIO_Read[0] & (0x01 << LineNbr)) != 0;
		break;
		case 2:
			iDioStatus = DAQmxReadDigitalU8 (Port2, DAQmx_Val_Auto, 1.0, DAQmx_Val_GroupByChannel, DIO_Read, 1,&iSamples, NULL);
			*state = (DIO_Read[0] & (0x01 << LineNbr)) != 0;
		break;
		default:
			iDioStatus = -9999999999;
		break;
	}
	if( iDioStatus ) {
		return( False );
	}

	return( True );
}

/*********************************************************************
*    Call the Stop function to stop the task.
*********************************************************************/
Boolean boDioStopTask (int PortNbr)
{
	switch( PortNbr ) {
		//Input	
		case 0:
			iDioStatus = DAQmxStopTask(Port0);
		break;
		case 1:
			iDioStatus = DAQmxStopTask(Port1);
		break;
		case 2:
			iDioStatus = DAQmxStopTask(Port2);
		break;
		//Output	
		case 3:
			iDioStatus = DAQmxStopTask(Port3);
		break;
		case 4:
			iDioStatus = DAQmxStopTask(Port4);
		break;
		case 5:
			iDioStatus = DAQmxStopTask(Port5);
		break;
		default:
			iDioStatus = -9999999999;
		break;
	}
	if( iDioStatus ) {
		return( False );
	}

	return( True );
}

/*********************************************************************
*    Call the Clear Task function to clear the Task.
*********************************************************************/
Boolean boDioClearTask (int PortNbr)
{
	switch( PortNbr ) {
		//Input	
		case 0:
			iDioStatus = DAQmxClearTask(Port0);
		break;
		case 1:
			iDioStatus = DAQmxClearTask(Port1);
		break;
		case 2:
			iDioStatus = DAQmxClearTask(Port2);
		break;
		//Output	
		case 3:
			iDioStatus = DAQmxClearTask(Port3);
		break;
		case 4:
			iDioStatus = DAQmxClearTask(Port4);
		break;
		case 5:
			iDioStatus = DAQmxClearTask(Port5);
		break;
		default:
			iDioStatus = -9999999999;
		break;
	}
	if( iDioStatus ) {
		return( False );
	}
	
	return( True );
}



//************************************************************************************************
//************************************************************************************************
Boolean boDioPortInitPort(int Port)
{
	if( (Port < 0) || (Port > 2) ) {
		return( False );
	}
	iDioPort = Port;
	
	if( !boDioCreateTask(iDioPort) ) {
		return( iDioStatus );
	}
	if( !boDioStartTask(iDioPort) ) {
		return( iDioStatus );
	}
	if( !boDioCreateTask(iDioPort + 3) ) {
		return( iDioStatus );
	}
	if( !boDioStartTask(iDioPort + 3) ) {
		return( iDioStatus );
	}
	
	return( True );
}


//************************************************************************************************
Boolean boDioPortClear(void)
{
	switch( iDioPort ) {
		case 0:
			iDioStatus = DAQmxWriteDigitalLines (Port3, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																				(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
			if( iDioStatus ) {
				return( False );
			}
		break;
		case 1:
			iDioStatus = DAQmxWriteDigitalLines (Port4, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																				(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
			if( iDioStatus ) {
				return( False );
			}
		break;
		case 2:
			iDioStatus = DAQmxWriteDigitalLines (Port5, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																				(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
			if( iDioStatus ) {
				return( False );
			}
		break;
	}
	
	if( !boDioStopTask(iDioPort) ) {
		return( False );
	}
	if( !boDioClearTask(iDioPort) ) {
		return( False );
	}
	if( !boDioStopTask(iDioPort + 3) ) {
		return( False );
	}
	if( !boDioClearTask(iDioPort + 3) ) {
		return( False );
	}

	return( True );
}

//************************************************************************************************
Boolean boDioPortResetAllOut(void)
{
	switch( iDioPort ) {
		case 0:
			iDioStatus = DAQmxWriteDigitalLines (Port3, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																				(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
			if( iDioStatus ) {
				return( False );
			}
		break;
		case 1:
			iDioStatus = DAQmxWriteDigitalLines (Port4, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																				(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
			if( iDioStatus ) {
				return( False );
			}
		break;
		case 2:
			iDioStatus = DAQmxWriteDigitalLines (Port5, 1, 0, 1.0, DAQmx_Val_GroupByChannel, 
																				(uInt8*)"\x01\x01\x01\x01\x01\x01\x01\x01", &iSamples, NULL);
			if( iDioStatus ) {
				return( False );
			}
		break;
	}
	
	return( True );
}

//************************************************************************************************
Boolean boDioPortWrite(int LineNbr, int state)
{
	if( !boDioWritePort(iDioPort + 3, LineNbr, state) ) {
		return( False );
	}

	return( True );
}

//************************************************************************************************
Boolean boDioPortRead(int LineNbr, int *state)
{
	if( !boDioReadPort(iDioPort, LineNbr, state) ) {
		return( False );
	}
	
	return( True );
}

//************************************************************************************************
Boolean boDioPortGetOutputState(byte *OutputState)
{
	int iZaehler = 0;
	
	switch( iDioPort + 3 ) {
		case 3:
			for( iZaehler = 0; iZaehler < 8; iZaehler++ ) {
				OutputState[iZaehler] = DIO_Data0[iZaehler] == 0x00;
			}
		break;

		case 4:
			for( iZaehler = 0; iZaehler < 8; iZaehler++ ) {
				OutputState[iZaehler] = DIO_Data1[iZaehler] == 0x00;
			}
		break;

		case 5:
			for( iZaehler = 0; iZaehler < 8; iZaehler++ ) {
				OutputState[iZaehler] = DIO_Data2[iZaehler] == 0x00;
			}
		break;
	}

	return( True );
}

