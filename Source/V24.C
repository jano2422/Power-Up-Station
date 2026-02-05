/******************************************************************************************/
/*                                                                                        */
/*  File : V24.C                                                                          */
/*                                                                                        */
/******************************************************************************************

	Documentation:
	--------------
	functions for serial communication

	Ver. 1.0     18.09.2001   J.Bauer:
			- using Windows SDK for opening, closing, sending, etc.

	Ver. 1.1     10.05.2002   J.Bauer:
			- new function for enabling HW handshake with RTS/CTS

	Ver. 1.2     02.03.2004   J.Bauer:
			- Open COM Ports larger than COM9 in OpenV24

	Ver. 1.3     24.04.2005   DunzM:
			- Added ConfigV24Buf() equivalent to ConfigV24() with InQueueBuffSize, OutQueueBuffSize
			- Added  iSendV24()
			- Added  iReadV24()


*******************************************************************************************/

//Includes
#include <CviTypes.h>
#include <Windows.h>
#include <V24.h>

#include <ansi_c.h>


//*******************************************************************************************
//Open ComPort and return handle
V24HANDLE OpenV24(int aComport)
{
	V24HANDLE hTemp = NULL;

	char szComPort[20];
	// Backslashes erfoderlich, um COM > 10 bedienen zu koennen
	sprintf(szComPort, "\\\\.\\COM%d", aComport);

	//Configuriert serielle Schnittstelle
	hTemp = CreateFile(szComPort,
										GENERIC_READ | GENERIC_WRITE,
										0,
										NULL,
										OPEN_EXISTING,
										0,
										NULL);

	// Port steht nicht zur Verfügung
	if (hTemp == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	return hTemp;
}

//*******************************************************************************************
// setting parameters
Boolean SetTimeOutsV24 (V24HANDLE aHandle, DWORD aReadMultipl, DWORD aReadConst,
																					 DWORD aSendMultipl, DWORD aSendConst)
{
	// timeouts
	COMMTIMEOUTS CommTimeOuts;

	if (GetCommTimeouts(aHandle, &CommTimeOuts ) == 0) {
		return False;
	}

	CommTimeOuts.ReadIntervalTimeout         = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier  = aReadMultipl;
	CommTimeOuts.ReadTotalTimeoutConstant    = aReadConst;
	CommTimeOuts.WriteTotalTimeoutMultiplier = aSendMultipl;
	CommTimeOuts.WriteTotalTimeoutConstant   = aSendConst;

	if (SetCommTimeouts(aHandle, &CommTimeOuts ) == 0) {
		return False;
	}

	return True;
}


//*******************************************************************************************
// setting parameters
Boolean ConfigV24 (V24HANDLE aHandle, long aBaud, TParity aPar, byte aDataBits, byte aStopBits)
{
	DCB dcb;
	//COMMTIMEOUTS CommTimeOuts;

	// load old settings
	if (!GetCommState(aHandle, &dcb)) {
		return False;
	}

	dcb.fDtrControl       = DTR_CONTROL_DISABLE;
	dcb.fOutxCtsFlow      = 0;
	dcb.fOutxDsrFlow      = 0;
	dcb.fDsrSensitivity   = 0;
	dcb.fTXContinueOnXoff = 1;
	dcb.fOutX             = 0;
	dcb.fInX              = 0;
	dcb.fErrorChar        = 0;
	dcb.fRtsControl       = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError     = 0;
	dcb.XonLim            = 0;
	dcb.XoffLim           = 1024;


	// Baudrate
	dcb.BaudRate = (DWORD) aBaud;
	// Databits, Bytesize
	dcb.ByteSize = aDataBits;
	// Parity
	switch (aPar) {
		case paNone:  dcb.Parity   = NOPARITY;    break;
		case paOdd :  dcb.Parity   = ODDPARITY;   break;
		case paEven:  dcb.Parity   = EVENPARITY;  break;
		case paMark:  dcb.Parity   = MARKPARITY;  break;
		case paSpace: dcb.Parity   = SPACEPARITY; break;
		default: dcb.Parity   = NOPARITY;
	}
	// Stopbits
	switch (aStopBits) {
		case  1:  dcb.StopBits = ONESTOPBIT;   break;
		case  2:  dcb.StopBits = TWOSTOPBITS;  break;
		case 15:  dcb.StopBits = ONE5STOPBITS; break;
		default:  dcb.StopBits = ONESTOPBIT;   break;
	}

	// new settings
	if (!SetCommState(aHandle, &dcb)) {
		return False;
	}

	if (!SetupComm(aHandle, 1024, 1024)) { // setup Device Buffer
		return False;
	}

	// timeouts
	if (!SetTimeOutsV24 (aHandle, 0, 0, 0, 0)) {
		return False;
	}

	return True;
}

//*******************************************************************************************
// setting parameters
Boolean ConfigV24Buf (V24HANDLE aHandle, long aBaud, TParity aPar, byte aDataBits, byte aStopBits,
											int aInBufSize, int aOutBufSize)
{
	DCB dcb;
	//COMMTIMEOUTS CommTimeOuts;

	// load old settings
	if (!GetCommState(aHandle, &dcb)) {
		return False;
	}

	dcb.fDtrControl       = DTR_CONTROL_DISABLE;
	dcb.fOutxCtsFlow      = 0;
	dcb.fOutxDsrFlow      = 0;
	dcb.fDsrSensitivity   = 0;
	dcb.fTXContinueOnXoff = 1;
	dcb.fOutX             = 0;
	dcb.fInX              = 0;
	dcb.fErrorChar        = 0;
	dcb.fRtsControl       = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError     = 0;
	dcb.XonLim            = 0;
	dcb.XoffLim           = 1024;


	// Baudrate
	dcb.BaudRate = (DWORD) aBaud;
	// Databits, Bytesize
	dcb.ByteSize = aDataBits;
	// Parity
	switch (aPar) {
		case paNone:  dcb.Parity   = NOPARITY;    break;
		case paOdd :  dcb.Parity   = ODDPARITY;   break;
		case paEven:  dcb.Parity   = EVENPARITY;  break;
		case paMark:  dcb.Parity   = MARKPARITY;  break;
		case paSpace: dcb.Parity   = SPACEPARITY; break;
		default: dcb.Parity   = NOPARITY;
	}
	// Stopbits
	switch (aStopBits) {
		case  1:  dcb.StopBits = ONESTOPBIT;   break;
		case  2:  dcb.StopBits = TWOSTOPBITS;  break;
		case 15:  dcb.StopBits = ONE5STOPBITS; break;
		default:  dcb.StopBits = ONESTOPBIT;   break;
	}

	// new settings
	if (!SetCommState(aHandle, &dcb)) {
		return False;
	}

	if (!SetupComm(aHandle, aInBufSize, aOutBufSize)) { // setup Device Buffer
		return False;
	}

	// timeouts
	if (!SetTimeOutsV24 (aHandle, 0, 0, 0, 0)) {
		return False;
	}

	return True;
}

//*******************************************************************************************
// setting RTS/CTS Handshake
Boolean ActivateRtsCts(V24HANDLE aHandle, TSwitch aSwitch)
{
	DCB dcb;

	// load old settings
	if (!GetCommState(aHandle, &dcb)) {
		return False;
	}

	if (aSwitch == On) {
		dcb.fOutxCtsFlow      = TRUE;
		dcb.fRtsControl       = RTS_CONTROL_HANDSHAKE;
	} else {
		dcb.fOutxCtsFlow      = FALSE;
		dcb.fRtsControl       = RTS_CONTROL_DISABLE;
	}

	// new settings
	if (!SetCommState(aHandle, &dcb)) {
		return False;
	}

	return True;
}

//*******************************************************************************************
Boolean CloseV24(V24HANDLE aHandle)
// closes serial
{
	if (aHandle) {
		return CloseHandle(aHandle);
	}
	else {
		return True;
	}
}


//*******************************************************************************************
// Clear send and receive buffer
Boolean ClearBuffer(V24HANDLE aHandle)
{
	if (PurgeComm(aHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) == 0) {
		return False;
	}

	return True;
}


//*******************************************************************************************
// Sending data
Boolean SendV24(V24HANDLE aHandle, const void* aData, unsigned long aSize)
{
	DWORD nByteWrite = 0;
  
	ClearBuffer(aHandle);	 //Sepp
	
	if (!WriteFile(aHandle,  aData, aSize, &nByteWrite, NULL)) {
		return False;
	}
	if (nByteWrite != aSize) {
		return False;
	}

	//ClearBuffer(aHandle);	 //Sepp
	
	return True;
}

//*******************************************************************************************
int iSendV24(V24HANDLE aHandle, const void* aData, unsigned long aSize)
{
	DWORD nByteWrite = 0;

	if( !WriteFile(aHandle,  aData, aSize, &nByteWrite, NULL) ) {
		return( 0 );
	}

	return( nByteWrite );
}

//*******************************************************************************************
// number of bytes in input buffer
unsigned long ReceiveCount(V24HANDLE aHandle)
{
	DWORD nError;
	COMSTAT ComState;

	if (!ClearCommError(aHandle, &nError, &ComState)) {
		return 0;
	}
	return ComState.cbInQue;
}


//*******************************************************************************************
Boolean ReadV24(V24HANDLE aHandle, void *aData, unsigned long aSize)
{
	DWORD nByteRead = 0;

	if (!ReadFile(aHandle, aData, aSize, &nByteRead, NULL)) {
		return False;
	}
	if (nByteRead != aSize ) {
		return False;
	}
	return True;
}

//*******************************************************************************************
int iReadV24(V24HANDLE aHandle, void *aData, unsigned long aSize)
{
	DWORD nByteRead = 0;

	if( !ReadFile(aHandle, aData, aSize, &nByteRead, NULL) ) {
		return( 0 );
	}

	return( nByteRead );
}


/*******************************************************************************************/
