#include <ansi_c.h>
#include "cvitypes.h"
#include "6TL_Cards.h" 
#include "Nican.h" 


static NCTYPE_OBJH 			TxHandle=0; 
static NCTYPE_STATUS 		Status;
static NCTYPE_CAN_FRAME 	Transmit;
static NCTYPE_CAN_STRUCT 	Answer;
static NCTYPE_ATTRID 		AttrIdList[8];
static NCTYPE_UINT32 		AttrValueList[8];
static NCTYPE_UINT32 		Baudrate = 50000;
static char 				Interface[7] = "CAN0";
static NCTYPE_STATE_P 		StatePtr;

void PrintStat(NCTYPE_STATUS status,char *source)
{
	char statusString[2048];
	if(status !=0)
	{
		ncStatusToString(status, sizeof (statusString), statusString);
		printf("\n%s\nSource = %s\n", statusString, source);
		if (status < 0)
		{
			ncCloseObject(TxHandle); exit(1);
		}
	}
}

Boolean boInitYAV(void)
{
	// Configure the CAN Network Interface Object */
	AttrIdList[0] = NC_ATTR_BAUD_RATE;
	AttrValueList[0] = Baudrate;
	AttrIdList[1] = NC_ATTR_START_ON_OPEN;
	AttrValueList[1] = NC_TRUE;
	AttrIdList[2] = NC_ATTR_READ_Q_LEN;
	AttrValueList[2] = 0;
	AttrIdList[3] = NC_ATTR_WRITE_Q_LEN;
	AttrValueList[3] = 1;
	AttrIdList[4] = NC_ATTR_CAN_COMP_STD;
	AttrValueList[4] = 0;
	AttrIdList[5] = NC_ATTR_CAN_MASK_STD;
	AttrValueList[5] = NC_CAN_MASK_STD_DONTCARE;
	AttrIdList[6] = NC_ATTR_CAN_COMP_XTD;
	AttrValueList[6] = 0;
	AttrIdList[7] = NC_ATTR_CAN_MASK_XTD;
	AttrValueList[7] = NC_CAN_MASK_XTD_DONTCARE;
	
	Status = ncConfig(Interface, 8, AttrIdList, AttrValueList);
	
	if (Status < 0){
		return( False );
	}
	/* open the CAN Network Interface Object */
	Status = ncOpenObject(Interface, &TxHandle);
	if (Status < 0){
		return( False );
	}
	return( True );
}

Boolean boSetOnYavRel(long lAdress,int iRelay) // Relays von 1 - 32
{
	long lBoard = 0;
	
	iRelay--;
	if(iRelay < 0 ) return (False);

	lBoard = lAdress & 0xF00;
	if( lBoard == 0x200 ) Transmit.Data[0] = 0x02;
	if( lBoard == 0x300 ) Transmit.Data[0] = 0x03;
	Transmit.Data[1] = 0x01; // YAV command prefix
	Transmit.Data[2] = (char)iRelay; // relay number   
	Transmit.Data[3] = 0x01; // YAV set relay ON command
	Transmit.DataLength = 4; // Set CAN frame lenght
	Transmit.IsRemote = 0; // This is not a remote frame
	Transmit.ArbitrationId = lAdress; //   0x15510200 -> assume board address is 0    
	Transmit.ArbitrationId |= 0x20000000; // NICAN specific, set 29 bit id length
	Status= ncWrite(TxHandle, sizeof(Transmit), &Transmit);
	if (Status < 0){
		return( False );   
	}
	return( True );
}
 

Boolean boSetOffYavRel(long lAdress,int iRelay) // Relays von 1 - 32   
{
	long lBoard = 0;
	
	iRelay--;
	if(iRelay < 0 ) return (False);

	lBoard = lAdress & 0xF00;
	if( lBoard == 0x200 ) Transmit.Data[0] = 0x02;
	if( lBoard == 0x300 ) Transmit.Data[0] = 0x03;
	Transmit.Data[1] = 0x01; // YAV command prefix
	Transmit.Data[2] = (char)iRelay ; // relay number
	Transmit.Data[3] = 0x00; // YAV set relay OFF command
	Transmit.DataLength = 4; // Set CAN frame lenght
	Transmit.IsRemote = 0; // This is not a remote frame
	Transmit.ArbitrationId = lAdress; //   0x15510200 -> assume board address is 0
	Transmit.ArbitrationId |= 0x20000000; // NICAN specific, set 29 bit id length
	Status= ncWrite(TxHandle, sizeof(Transmit), &Transmit);
	if (Status < 0){
		return( False );   
	}	
	return( True );	
}
Boolean boResetYavBoard(long lAdress) // Relays von 1 - 32   
{
	long lBoard = 0;
	
	lBoard = lAdress & 0xF00;
	if( lBoard == 0x200 ) Transmit.Data[0] = 0x02;
	if( lBoard == 0x300 ) Transmit.Data[0] = 0x03;
	Transmit.Data[1] = 0x03; // YAV command prefix
	Transmit.Data[2] = 0 ; // relay number
	Transmit.Data[3] = 0 ; // relay number
	Transmit.Data[4] = 0 ; // relay number
	Transmit.Data[5] = 0 ; // relay number
	Transmit.Data[6] = 0x06; // YAV set relay OFF command
	Transmit.DataLength = 7; // Set CAN frame lenght
	Transmit.IsRemote = 0; // This is not a remote frame
	Transmit.ArbitrationId = lAdress; //   0x15510200 -> assume board address is 0
	Transmit.ArbitrationId |= 0x20000000; // NICAN specific, set 29 bit id length
	Status= ncWrite(TxHandle, sizeof(Transmit), &Transmit);
	if (Status < 0){
		return( False );   
	}	
	return( True );	
}
	
Boolean boAskYavRel(long lAdress, int iRelay,long lStatus[])
{
	long lBoard = 0;
	
	iRelay--;
	if(iRelay < 0 ) return (False);
	
	lBoard = lAdress & 0xF00;
	if( lBoard == 0x200 ) Transmit.Data[0] = 0x02;
	if( lBoard == 0x300 ) Transmit.Data[0] = 0x03;
	Transmit.Data[1] = 0x02; // YAV command prefix
	Transmit.Data[2] = (char)iRelay ; // relay number
	Transmit.DataLength = 3; // Set CAN frame lenght
	Transmit.IsRemote = 0; // This is not a remote frame
	Transmit.ArbitrationId = lAdress; //   0x15510200 -> assume board address is 0
	Transmit.ArbitrationId |= 0x20000000; // NICAN specific, set 29 bit id length
	
	Status= ncWrite(TxHandle, sizeof(Transmit), &Transmit);
	ncWaitForState(TxHandle,NC_ST_READ_AVAIL,100, StatePtr);
	Status=ncRead(TxHandle, sizeof(Answer), &Answer);
	lStatus[0]=Answer.Data[3];
	if (Status < 0){
		return ( False );   
	}
	else if(Status > 0) {
		
		return ( False );
		//PrintStat (Status, "RelayStatus");	 
	}
	return( True );		
	
}

Boolean boCloseYAV(void)
{
	Status = ncCloseObject(TxHandle);
	if (Status < 0){
		return( False );
	}	
	return( True );	
}









