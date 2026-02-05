/******************************************************************************************
*                                                                                         *
*            Header: V24.H                                                                *
*                                                                                         *
* Documentation:                                                                          *
* --------------                                                                          *
* functions for serial communication                                                      *
*                                                                                         *
*                                                                                         *
*******************************************************************************************/
#if !defined ( __V24_H)
#define __V24_H


enum _TParity  {paNone, paOdd, paEven, paMark, paSpace};
typedef enum _TParity TParity;


typedef void   *V24HANDLE;

//*******************************************************************************************

extern V24HANDLE OpenV24(int aComport);                                                               

extern Boolean SetTimeOutsV24 (V24HANDLE aHandle, DWORD aReadMultipl, DWORD aReadConst,
																					 DWORD aSendMultipl, DWORD aSendConst);
extern Boolean ConfigV24 (V24HANDLE aHandle, long aBaud, TParity aPar, byte aDataBits, byte aStopBits);
extern Boolean ConfigV24Buf (V24HANDLE aHandle, long aBaud, TParity aPar, byte aDataBits, byte aStopBits, 
											int aInBufSize, int aOutBufSize);
extern Boolean ActivateRtsCts(V24HANDLE aHandle, TSwitch aSwitch);

extern Boolean CloseV24(V24HANDLE aHandle);

extern Boolean ClearBuffer(V24HANDLE aHandle);

extern Boolean SendV24(V24HANDLE aHandle, const void* aData, unsigned long aSize);
extern int iSendV24(V24HANDLE aHandle, const void* aData, unsigned long aSize);

extern unsigned long ReceiveCount(V24HANDLE aHandle);

extern Boolean ReadV24(V24HANDLE aHandle, void *aData, unsigned long aSize);
extern int iReadV24(V24HANDLE aHandle, void *aData, unsigned long aSize);

//*******************************************************************************************
																			 
#endif

/*******************************************************************************************/
