#include <windows.h>

typedef enum VKeyGenResultEx
{
	KGRE_Ok = 0,
	KGRE_BufferToSmall = 1,
	KGRE_SecurityLevelInvalid = 2,
	KGRE_VariantInvalid = 3,
	KGRE_UnspecifiedError = 4,
	
	XcpSkExtFncAck                      = 0, // o.k.
	XcpSkExtFncErrPrivilegeNotAvailable = 1, // the requested privilege can not be unlocked with this DLL
	XcpSkExtFncErrInvalidSeedLength     = 2, // the seed length is wrong, key could not be computed
	XcpSkExtFncErrUnsufficientKeyLength = 3
	
}VKeyGenResultEX;

typedef VKeyGenResultEX  (*KEY_SEED) /*(const unsigned char*, unsigned int,const unsigned int, const char*,unsigned char* , unsigned int,
																 unsigned int* ,const char* );	*/
															 			 (unsigned char, unsigned char, unsigned char*,unsigned char*, unsigned char*)	 ;
KEY_SEED     XCP_ComputeKeyFromSeed; 	
int LinkSeedKeyAPI(void)  ;



int LinkSeedKeyAPI(void)
{
/*
	VKeyGenResultEX ResultEx=-1;
	

  unsigned char       seed[]	= {0x1A, 0x39, 0x95,0xD1 };
  unsigned char       key[]	= {0,0,0,0};
  unsigned char		privilege = 0x10; // Ext Diag Session/BSD Development Session
  unsigned char		byteLenKey	= 4;
  unsigned char		byteLenSeed	= 4; 

*/	
  HINSTANCE       HandleSeedKeyDLL=NULL;

  HandleSeedKeyDLL = LoadLibrary("seednkeyxcp.dll");

  if (HandleSeedKeyDLL != NULL) {

    XCP_ComputeKeyFromSeed = (KEY_SEED) GetProcAddress(HandleSeedKeyDLL, "XCP_ComputeKeyFromSeed");
    if (!XCP_ComputeKeyFromSeed)  {
      FreeLibrary(HandleSeedKeyDLL);
      return (0);   
    }  		
  }	  
/*	
	ResultEx=XCP_ComputeKeyFromSeed(privilege,byteLenSeed, seed, &byteLenKey, key);
*/
	return(1)  ;
		
}
