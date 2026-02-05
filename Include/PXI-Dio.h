#ifndef __PXI_DIO_H
#define __PXI_DIO_H

#include <CviTypes.h>

#include <NIDAQmx.h>

/***************************************************************************/
extern Boolean boDioInit(void);
extern Boolean boDioClear(void);

extern Boolean boDioResetAllOut(void);

extern Boolean boDioWrite(int LineNbr, int state); // LineNbr -> 0 - 23 // state -> 1 = ein, 0 = aus 
extern Boolean boDioRead(int LineNbr, int *state); 
extern Boolean boDioGetOutputState(byte *OutputState);

extern Boolean boDioWritePort(int PortNbr, int LineNbr, int state);
extern Boolean boDioReadPort(int PortNbr, int LineNbr, int *state);


extern Boolean boDioPortInitPort(int Port);
extern Boolean boDioPortClear(void);

extern Boolean boDioPortResetAllOut(void);

extern Boolean boDioPortWrite(int LineNbr, int state);
extern Boolean boDioPortRead(int LineNbr, int *state);
extern Boolean boDioPortGetOutputState(byte *OutputState);

// additional new funktions by ArzbergerM
extern Boolean boSetOn(int iOutput);
extern Boolean boSetOff(int iOutput);  
extern Boolean boLedOn(int iInput );  
extern Boolean boLedOff(int iInput );  
extern Boolean boIsSet(int iInput );
extern Boolean boWaitUntilOn(int iInput, double dTimeInSeconds ); 
extern Boolean boWaitUntilOff(int iInput, double dTimeInSeconds );  

/***************************************************************************/
#endif
