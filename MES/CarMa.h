//////////////////////////////////////////////////////////////////////////////////////////////
//
// FraMES - Framework MES
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: CarMa Functions
//
//////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#define CARMA_VERSION        "$Name: V0208b2 $"
#define CARMA_RELEASE_DATE   "$Date: 2009/08/07 13:28:52 $"


#ifndef CARMA
#define CARMA

#ifdef __cplusplus
	extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Typedef's
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct car_structtime {
    int tm_sec;		  // seconds (0-59)
    int tm_min;       // minutes (0-59)
    int tm_hour;      // hour    (0-23)
    int tm_mday;      // day of month (1-31)
    int tm_mon;       // month   (0-11; January=0)
    int tm_year;      // year    (current year - 1900)
    int tm_wday;      // day of week  (0-6; Sunday=0)
    int tm_yday;      // day of year  (0-365)
    int tm_isdst;     // daylight saving time
} car_STRUCTTIME;


typedef struct car_structcarrier
 {
	char  szCarrierIdType[100];
	char  szCarrierId[100];
	long  lPosMax;
	long  lPosActual;
	short sLocked;
	char  szSupplierId[100];
	char  szLocationId[100];
	char  szExternalId1[100];
	char  szExternalId2[100]; 
	char  szDescShort[100];
	char  szDescLong[2000]; 
	char  szStatus[100];
	char  szOperator[100];
	char  szUserComment[300];
	long  lUseCounter; 
	long  lCreated;
	long  lUpdated; 
	long  lLastMaintenance;
	long  lLastInUse;
	car_STRUCTTIME tCreated;
	car_STRUCTTIME tUpdated; 
	car_STRUCTTIME tLastMaintenance;
	car_STRUCTTIME tLastInUse;
 } car_STRUCTCARRIER;


typedef struct car_structposition
 {
	char  szPosition[100];
	char  szUnitIdType[100];
	char  szUnitId[100];
	char  szStatus[100];
	char  szOperator[100];
	char  szUserComment[300];
	short sLocked;
	car_STRUCTTIME tCreated;
	car_STRUCTTIME tUpdated; 
 } car_STRUCTPOSITION;
	 

//////////////////////////////////////////////////////////////////////////////////////////////
//   Error Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall car_ShowErrorFL(const char *szFile,
									 const short sLine);
extern short __stdcall car_ShowErrorXY(const int iTop, const int iLeft, 
                                       const char *szFile, const short sLine);

extern short __stdcall car_Init(void);
extern short __stdcall car_GetErrorCode(void);
extern char* __stdcall car_GetErrorText(const short sErrorCode);
extern short __stdcall car_GetErrorInformation(char **szErrorText, const char **szFunctionName,
											   char **szErrorHint, char **szCapsError);
extern char* __stdcall car_GetCfg(const char *szKey, const short boShowError);
extern char* __stdcall car_GetCfg2(const char *szSection, const char *szKey, const short boShowError);

extern short __stdcall car_DisableFreeMemory(void);        

#ifndef car_ShowError
#define car_ShowError(DUMMY) car_ShowErrorXY(0,0,__FILE__,__LINE__)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Init Module Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern char* __stdcall car_GetVersion(void);
extern char* __stdcall car_GetReleaseDate(void);

extern void __stdcall  car_SetLogLevel(const short sLogLevel);
extern void __stdcall  car_SetLogDest(const char *szLogDest);
extern void __stdcall  car_SetExtHintFct(void __stdcall (*aFctPtr)(const char*));

extern short __stdcall car_SetBaseID(const char *szBaseID);
extern short __stdcall car_GetBaseID(char *szBaseID);


//////////////////////////////////////////////////////////////////////////////////////////////
// Information Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall car_GetCarrierType(const char *szCarrierType,
								   long  *lMaxPositions, char *szDescription,
								   char  *szFormat,
								   char  *szDefaultIdType,
								   long  *lCarTypeAttrCnt,
								   long  **lCarTypeAttrIdx,
								   char  (**szCarTypeAttr)[100]);

extern short __stdcall car_GetCarriersOfType(const char *szCarrierType,
								   const long  lMaxCarriers, 
								   char  (**szCarrierIdType)[100], char  (**szCarrierId)[100]);
extern short __stdcall car_GetCarriersOfType2(const char *szCarrierType,
								   const long  lMaxCarriers, 
								   const car_STRUCTTIME tCreated, const car_STRUCTTIME tUpdated,
								   car_STRUCTCARRIER **pCarriers);

//////////////////////////////////////////////////////////////////////////////////////////////
// Administration Functions
//////////////////////////////////////////////////////////////////////////////////////////////

// creating / deleting carrier
extern short __stdcall car_CreateCarrier(const char *szCarrierType,
								  const char *szCarrierIdType, const char *szCarrierId,
								  const char *szDescription);
extern short __stdcall car_DeleteCarrier(const char *szCarrierIdType, const char *szCarrierId);

// working with additional information
extern short __stdcall car_SetCarrierDescription(const char *szCarrierIdType, const char *szCarrierId,
										  const char *szDescription);
extern short __stdcall car_GetCarrierInfo(const char *szCarrierIdType, const char *szCarrierId,
								   char *szCarrierType, char *szDescription);
extern short __stdcall car_GetCarrierInfo2(const char *szCarrierIdType, const char *szCarrierId,
																	  char *szCarrierType, char *szDescription,
 																	  long  *lCarAttrCnt,
																	  long  **lCarAttrIdx,
																	  char  (**szCarAttr)[100]);

// operator on carrier level
extern short __stdcall car_SetCarrierOperator(const char *szCarrierIdType, const char *szCarrierId, const char *szOperator);
extern short __stdcall car_GetCarrierOperator(const char *szCarrierIdType, const char *szCarrierId, char *szOperator);

// locking on carrier level
extern short __stdcall car_LockCarrier(const char *szCarrierIdType, const char *szCarrierId);
extern short __stdcall car_LockCarrier2(const char *szCarrierIdType, const char *szCarrierId, const char *szComment, const char *szOperator);
extern short __stdcall car_UnlockCarrier(const char *szCarrierIdType, const char *szCarrierId);
extern short __stdcall car_UnlockCarrier2(const char *szCarrierIdType, const char *szCarrierId, const char *szComment, const char *szOperator);
extern short __stdcall car_IsCarrierLocked(const char *szCarrierIdType, const char *szCarrierId, short *sLockState, char *szComment, char *szOperator);


//////////////////////////////////////////////////////////////////////////////////////////////
// Working Functions
//////////////////////////////////////////////////////////////////////////////////////////////

extern short __stdcall car_CarrierUsePosition(const char *szCarrierIdType, const char *szCarrierId,
									   char *szPosition,
									   const char *szUnitIdType, const char *szUnitId);
extern short __stdcall car_CarrierUsePositions(const char *szCarrierIdType, const char *szCarrierId,
										const short sCount, char (*szPositions)[100],
										const char (*szUnitIdType)[100], const char (*szUnitId)[100]);
extern short __stdcall car_CarrierReleasePosition(const char *szCarrierIdType, const char *szCarrierId,
										   const char *szPosition);
extern short __stdcall car_CarrierReleasePositions(const char *szCarrierIdType, const char *szCarrierId,
											const short sCount, const char (*szPositions)[100]);
extern short __stdcall car_CarrierReleaseUnit(const char *szCarrierIdType, const char *szCarrierId,
									   const char *szUnitIdType, const char *szUnitId);
extern short __stdcall car_CarrierReleaseUnits(const char *szCarrierIdType, const char *szCarrierId,
										const short sCount,
										const char (*szUnitIdType)[100], const char (*szUnitId)[100]);
extern short __stdcall car_ReleaseUnit(const char *szUnitIdType, const char *szUnitId);
extern short __stdcall car_CarrierReleaseAll(const char *szCarrierIdType, const char *szCarrierId);
extern short __stdcall car_CountCarrierPositions(const char *szCarrierIdType, const char *szCarrierId,
										  short *sCount);
extern short __stdcall car_GetCarrierPosition(const char *szCarrierIdType, const char *szCarrierId,
									   const char *szPosition,
									   char *szUnitIdType, char *szUnitId);
extern short __stdcall car_GetCarrierPositions(const char *szCarrierIdType, const char *szCarrierId,
										char (**szPosition)[100],
										char (**szUnitIdType)[100], char (**szUnitId)[100]);
extern short __stdcall car_GetCarrierPositions2(const char *szCarrierIdType, const char *szCarrierId,
										long  *lCount, car_STRUCTPOSITION **pPositions);
extern short __stdcall car_GetUnitPosition(const char *szUnitIdType, const char *szUnitId,
									char *szCarrierIdType, char *szCarrierId,
									char *szPosition);

// status on position/unit level
extern short __stdcall car_SetPositionStatus(const char *szCarrierIdType, const char *szCarrierId,
                                    const char *szPosition, const char *szStatus);
extern short __stdcall car_SetUnitStatus(const char *szUnitIdType, const char *szUnitId,
                                         const char *szStatus);
extern short __stdcall car_GetPositionStatus(const char *szCarrierIdType, const char *szCarrierId,
                                             const char *szPosition, char *szStatus);
extern short __stdcall car_GetUnitStatus(const char *szUnitIdType, const char *szUnitId, char *szStatus);


extern short __stdcall car_SetStatusEntry(const char *szCarrierIdType, const char *szCarrierId,
                                          const char *szPosition, const char *szEntry);
extern short __stdcall car_CheckStatusFlagEntry(const char *szCarrierIdType, const char *szCarrierId, 
                                                const char *szPosition, const char *szEntry, short *sIsSet);
extern short __stdcall car_ClearStatusEntry(const char *szCarrierIdType, const char *szCarrierId, 
                                            const char *szPosition, const char *szEntry);
extern short __stdcall car_GetAllStatusEntries(const char *szCarrierIdType, const char *szCarrierId, 
                                               const char *szPosition, short *sEntryCount, char (**szEntry)[20]);

// operator on position/unit level
extern short __stdcall car_SetPositionOperator(const char *szCarrierIdType, const char *szCarrierId,
                                    const char *szPosition, const char *szOperator);
extern short __stdcall car_SetUnitOperator(const char *szUnitIdType, const char *szUnitId,
                                         const char *szOperator);
extern short __stdcall car_GetPositionOperator(const char *szCarrierIdType, const char *szCarrierId,
                                             const char *szPosition, char *szOperator);
extern short __stdcall car_GetUnitOperator(const char *szUnitIdType, const char *szUnitId, char *szOperator);

#ifdef __cplusplus
	}
#endif

#endif
