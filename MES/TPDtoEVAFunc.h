/*****************************************************************************/
/*                                                                           */
/*  Header : TPDtoEVA.H                                                      */
/*                                                                           */
/*****************************************************************************

	Documentation:
	--------------

	Version 1.0 vom 07.01.2003    Juergen Bauer
	------------------------------------------------

	Version 2.0 vom 10.06.2005    Juergen Bauer
	------------------------------------------------
	- added Testparameters from EVAPROD to use it for working with "char"

	Version 2.1 vom 20.06.2005    Juergen Bauer
	------------------------------------------------
	- additional function "CheckCharArray" as standalone for checking chars

	Version 2.2 vom 21.06.2005    Juergen Bauer
	------------------------------------------------
	- possibility to cancel testrun by teststep parameter configuration

	Version 2.3 vom 29.06.2005    Juergen Bauer
	------------------------------------------------
	- new function "SetCancelRunOnFail" and using parameter 0 with value 0

	Version 2.4     13.09.2005    Juergen Bauer
	------------------------------------------------
	- new function "DataStorePutString" that's using parameter 1 with string to compare
	
	******************************************************************************


******************************************************************************/

#if !defined ( __TPDTOEVAFUNC_H)
#define __TPDTOEVAFUNC_H


#ifdef __cplusplus
extern "C" {
#endif


//****************************************************************************************
// structures
typedef struct
{
	long ID;
	int ( *pFct)(int aID);
} FctPtr;

//****************************************************************************************
// constants
#define MAX_MEAS_FUNCTIONS  200

//****************************************************************************************
// typedefs


//****************************************************************************************
// variables


//****************************************************************************************
// functions

// initialization
int   _DLL_ InitTPDtoEVA(void);
void  _DLL_ RemoveTPDtoEVA(void);

int   _DLL_ SetMeasFuncArr(void *aFctPtr, int aCount);

void  _DLL_ SetCancelRunOnFail(int aActivate);

// saving results
void  _DLL_ DataStorePutFloat(unsigned int aInt, float aFloat);
void  _DLL_ DataStorePutChar(unsigned int aInt, char *aRes);
void  _DLL_ DataStorePutString(unsigned int aInt, char *aRes);
void  _DLL_ DataStorePutBool(unsigned int aInt, BOOL aRes);

// standalone functions
int   _DLL_ CheckCharArray(char *aCheck, int aLen, char *aCompare);

// execute function
float _DLL_ ExecuteMeasure(short aSlot, short aIdx, short aRun, short aTest, char *aNummer);

#ifdef __cplusplus
}
#endif


/*****************************************************************************/
#endif


