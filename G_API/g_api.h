#ifndef _G_API_H
#define _G_API_H


#ifndef G_API
	#define G_API  __cdecl
#endif


#ifndef G_API_DLL
	#ifdef G_API_DLL_EXPORTS
		#define G_API_DLL __declspec(dllexport)
	#else
		#define G_API_DLL __declspec(dllimport)
	#endif
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _HOST_TYPES_H
	#include "host_types.h"
#endif


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// common parameters
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef u32_t  G_PortHandle_t;
typedef u32_t  G_Error_t;


typedef union {
	u8_t  U8[8];
	u16_t  U16[4];
	u32_t  U32[2];
	u64_t  U64;
} G_Data_t;


//lint -strong(,G_AsyncCommunication_CallbackFunction_t)
typedef void
(G_API * G_AsyncCommunication_CallbackFunction_t) (
	const G_PortHandle_t portHandle,
	void * const callbackMemory,
	const u8_t * const data,
	const u32_t dataLength
);


//lint -strong(,G_AsyncCommunication_CleanUpFunction_t)
typedef void
(G_API * G_AsyncCommunication_CleanUpFunction_t) (
	const G_PortHandle_t portHandle,
	void * const callbackMemory
);


#define G_NO_ERROR                                 ((G_Error_t) 0x00)
#define MAX_NUMBER_OF_CONTROL_DATA_BYTES                            4
#define MAX_PARAM_SIZE                                           4084


typedef u8_t  G_Fw_HeaderType_t;

#define G_FW__HEADER_TYPE__COMMAND  \
	((G_Fw_HeaderType_t)  0x00)

#define G_FW__HEADER_TYPE__RESPONSE  \
	((G_Fw_HeaderType_t)  0x01)

#define G_FW__HEADER_TYPE__ACKNOWLEDGE  \
	((G_Fw_HeaderType_t)  0x02)



typedef u8_t  G_InterfaceType_t;

#define G_INTERFACE_TYPE__UNKNOWN  \
	((G_InterfaceType_t)  0x00)

#define G_INTERFACE_TYPE__BOOT_LOADER  \
	((G_InterfaceType_t)  0x01)

#define G_INTERFACE_TYPE__SEQUENCE  \
	((G_InterfaceType_t)  0x02)

#define G_INTERFACE_TYPE__IO  \
	((G_InterfaceType_t)  0x03)

#define G_INTERFACE_TYPE__CAN  \
	((G_InterfaceType_t)  0x04)

#define G_INTERFACE_TYPE__LIN  \
	((G_InterfaceType_t)  0x05)

#define G_INTERFACE_TYPE__KLINE  \
	((G_InterfaceType_t)  0x06)

#define G_INTERFACE_TYPE__MOST  \
	((G_InterfaceType_t)  0x07)

#define G_INTERFACE_TYPE__LVDS  \
	((G_InterfaceType_t)  0x08)

#define G_INTERFACE_TYPE__FLEXRAY  \
	((G_InterfaceType_t)  0x09)

#define G_INTERFACE_TYPE__NET2RUN  \
	((G_InterfaceType_t)  0x0A)

#define G_INTERFACE_TYPE__J1850_PWM  \
	((G_InterfaceType_t)  0x0B)

#define G_INTERFACE_TYPE__FS  \
	((G_InterfaceType_t)  0x0C)


typedef union {
	struct {
		u8_t  B1;
		u8_t  B2;
		u8_t  B3;
		u8_t  B4;
	} Byte;

	u32_t  All;
} G_IpAddress_t;



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// low level functions
// !!!Attention!!!
// These functions are for internal use only
// and should not be called by the user!
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetParamSize
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_GetParamSize(
	const G_PortHandle_t  portHandle,
	u32_t  * const paramSize
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetLockStatus
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef u8_t  G_LockStatus_t;

#define G_LOCK_STATUS__NO_LOCK  \
	((G_LockStatus_t) 0x00)

#define G_LOCK_STATUS__LOCK_CONTROLLER  \
	((G_LockStatus_t) 0x01)

#define G_LOCK_STATUS__LOCK_INTERFACE  \
	((G_LockStatus_t) 0x02)

#define G_LOCK_STATUS__LOCK_MONITOR  \
	((G_LockStatus_t) 0x04)


G_API_DLL
G_LockStatus_t
G_API
G_GetLockStatus(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetLock
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_GetLock(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_ReleaseLock
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_ReleaseLock(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_SetResponseTimeout
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_SetResponseTimeout(
	const G_PortHandle_t  portHandle,
	const u32_t  responseTimeout  // in milliseconds
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetResponseTimeout
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
u32_t
G_API
G_GetResponseTimeout(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_IsMonitorEnabled
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
u8_t
G_API
G_IsMonitorEnabled(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_InitMonitor
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_InitMonitor(
	const G_PortHandle_t  portHandle,
	const u32_t  bufferSize
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_InitAsyncMonitor
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_InitAsyncMonitor(
	const G_PortHandle_t portHandle,
	const G_AsyncCommunication_CallbackFunction_t callbackFunction,
	const G_AsyncCommunication_CleanUpFunction_t cleanUpFunction,
	const u32_t callbackMemorySize,
	void ** callbackContext
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetMonitorItems
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_GetMonitorItems(
	const G_PortHandle_t  portHandle,
	u8_t  * const response,
	u32_t  * const responseLength,
	u8_t  * const bufferOverrun
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_CloseMonitor
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_CloseMonitor(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_AsyncCommunication_AddCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  command,
	const G_Fw_HeaderType_t  type,
	const u8_t * const controlData,
	const u8_t controlDataLength,
	const G_AsyncCommunication_CallbackFunction_t callbackFunction,
	const G_AsyncCommunication_CleanUpFunction_t cleanUpFunction,
	const u32_t callbackMemorySize,
	void ** callbackContext
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_AsyncCommunication_RemoveCallback(
	const G_PortHandle_t portHandle,
	const u8_t command,
	const G_Fw_HeaderType_t  type,
	const u8_t * controlData,
	const u8_t controlDataLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fpga_ReadRegister
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_Fpga_ReadRegister(
	const G_PortHandle_t  portHandle,
	const u8_t  registerNumber,
	u32_t * const value
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fpga_WriteRegister
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_Fpga_WriteRegister(
	const G_PortHandle_t  portHandle,
	const u8_t  registerNumber,
	const u32_t  value
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fpga_ReadFifo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_Fpga_ReadFifo(
	const G_PortHandle_t  portHandle,
	u8_t * const data,
	u32_t * const length
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fpga_WriteFifo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_Fpga_WriteFifo(
	const G_PortHandle_t  portHandle,
	const u8_t * const data,
	const u32_t  length
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fw_Buffer_Enable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_Fw_Buffer_Enable(
	const G_PortHandle_t  portHandle,
	const u32_t  bufferSize  // 0 = default buffer size
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fw_Buffer_Disable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G_API_DLL
G_Error_t
G_API
G_Fw_Buffer_Disable(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fw_Write
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef  u8_t  G_Fw_Write_Mode_t;

#define G_FW__WRITE__MODE__NEVER_SEND_ACK  \
	((G_Fw_Write_Mode_t)  0x00)

#define G_FW__WRITE__MODE__ALWAYS_SEND_ACK  \
	((G_Fw_Write_Mode_t)  0x01)

#define G_FW__WRITE__MODE__ACK_ONLY_ON_ERRORS  \
	((G_Fw_Write_Mode_t)  0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Fw_Write(
	const G_PortHandle_t  portHandle,
	const G_Fw_Write_Mode_t  mode,
	const u8_t  commandCode,
	const u32_t  length,
	const u8_t * const data
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Fw_Read
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef  u8_t  G_Fw_Read_ResponseType_t;

#define G_FW__READ__RESPONSE_TYPE__UNKNOWN \
	((G_Fw_Read_ResponseType_t)  0x00)

#define G_FW__READ__RESPONSE_TYPE__RESPONSE \
	((G_Fw_Read_ResponseType_t)  0x01)

#define G_FW__READ__RESPONSE_TYPE__ACK \
	((G_Fw_Read_ResponseType_t)  0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Fw_Read(
	const G_PortHandle_t  portHandle,
	u8_t * const commandCode,
	u8_t * const bufferOverrun,
	G_Fw_Read_ResponseType_t * const type,
	u32_t * const length,
	u8_t * const data
);


//******************************************************************************
// USER FUNCTIONS
//******************************************************************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_OpenInterface
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef enum {
	G_HOST_INTERFACE_TYPE__PCI3051        =  0,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI3051        =  1,  // CardNumber
	G_HOST_INTERFACE_TYPE__PCI3052        =  2,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI3052        =  3,  // CardNumber
	G_HOST_INTERFACE_TYPE__PCI3060        =  4,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI3060        =  5,  // CardNumber
	G_HOST_INTERFACE_TYPE__PCI3072        =  6,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI3072        =  7,  // CardNumber
	G_HOST_INTERFACE_TYPE__PCI3080        =  8,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI3080        =  9,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB_BASICCAN   = 10,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB3052        = 11,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB3060        = 12,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB_BASICLIN   = 13,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB3072        = 14,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB_BASICCAR   = 15,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB3080        = 16,  // CardNumber
	G_HOST_INTERFACE_TYPE__SMART_CAR_USB  = 17,  // CardNumber
	G_HOST_INTERFACE_TYPE__ETHERNET       = 18,  // Ip + Port
	G_HOST_INTERFACE_TYPE__USB4120        = 19,  // CardNumber
	G_HOST_INTERFACE_TYPE__PCI3090        = 20,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI3090        = 21,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI6153        = 22,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB6153        = 23,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI6173        = 24,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB6173        = 25,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI6191        = 26,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB6191        = 27,  // CardNumber
	G_HOST_INTERFACE_TYPE__BASICCAR3085   = 28,  // CardNumber
	G_HOST_INTERFACE_TYPE__PXI6181        = 29,  // CardNumber
	G_HOST_INTERFACE_TYPE__USB6181        = 30,  // CardNumber
	G_HOST_INTERFACE_TYPE__UNKNOWN        = 31
} G_HostInterfaceType_t;


typedef struct {
	G_HostInterfaceType_t  Type;

	struct {
		u8_t  ControllerNumber;
		u8_t  InterfaceNumber;
		u8_t  reserved1;
		u8_t  reserved2;

		union {
			u32_t  CardNumber;

			struct {
				u16_t  PortNumber;
				u8_t  reserved1;
				u8_t  reserved2;

				u8_t  IpAddress[16];
			} Ethernet;

			struct {
				u32_t  PortNumber;
				u32_t  BaudRate;
			} Serial;
		} u;
	} Parameters;
} G_InterfaceInfo_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_OpenInterface
// description: initialize connection to hardware
// parameters:  interfaceInfo - interface info
//              portHandle - pointer to porthandle variable
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_OpenInterface(
	const G_InterfaceInfo_t  * const interfaceInfo,
	G_PortHandle_t  * const portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_OpenInterface2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t G_OpenInterface2_CmdFlags_t;

#define G_OPEN_INTERFACE_2__CMD_FLAG__NONE  \
	((G_OpenInterface2_CmdFlags_t)  0x00)

#define G_OPEN_INTERFACE_2__CMD_FLAG__DISABLE_AUTOMATIC_CONFIGURATION  \
	((G_OpenInterface2_CmdFlags_t)  0x01)

#define G_OPEN_INTERFACE_2__CMD_FLAG__NO_SOFTWARE_RESET  \
	((G_OpenInterface2_CmdFlags_t)  0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_OpenInterface2
// description: initialize connection to hardware with extra options
// parameters:  cmdFlags - command flags
//              interfaceInfo - interface info
//              portHandle - pointer to porthandle variable
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_OpenInterface2(
	const G_OpenInterface2_CmdFlags_t  cmdFlags,
	const G_InterfaceInfo_t * const interfaceInfo,
	G_PortHandle_t * const portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_CloseInterface
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_CloseInterface
// description: close connection to interface
// parameters:  portHandle - port handle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_CloseInterface(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_RefreshDriverInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_RefreshDriverInfo
// description: refresh driver info
// parameters:  none
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_RefreshDriverInfo(
	void
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Command
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Command
// description: send command to server and wait for ack
// parameters:  portHandle - porthandle
//              commandCode - command code
//              parameters - pointer to parameter bytes
//              parameterLength - parameter length
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Command(
	const G_PortHandle_t  portHandle,
	const u8_t  commandCode,
	const u8_t  * const parameters,
	const u32_t  parameterLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_CommandWithResponse
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_CommandWithResponse
// description: send command to server and wait for response
// parameters:  portHandle - porthandle
//              commandCode - command code
//              parameters - pointer to parameter bytes
//              parameterLength - parameter length
//              response - pointer to response buffer
//              responseLength - pointer to var for response length
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_CommandWithResponse(
	const G_PortHandle_t  portHandle,
	const u8_t  commandCode,
	const u8_t  * const parameters,
	const u32_t  parameterLength,
	u8_t  * const response,
	u32_t  * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_CommandWithResponseEx
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_CommandWithResponseMode_t;

#define G_COMMAND_WITH_RESPONSE_MODE__SYNC             \
	((G_CommandWithResponseMode_t) 0x00)

#define G_COMMAND_WITH_RESPONSE_MODE__ASYNC            \
	((G_CommandWithResponseMode_t) 0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_CommandWithResponseEx
// description: command with response and extended parameters
// parameters:  portHandle - porthandle
//              mode - command with response mode
//              commandCode - command code
//              controlDataLength - number of bytes to check
//              parameters - pointer to parameter bytes
//              parameterLength - parameter length
//              response - pointer to response buffer
//              responseLength - pointer to var for response length
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_CommandWithResponseEx(
	const G_PortHandle_t  portHandle,
	const G_CommandWithResponseMode_t mode,
	const u8_t  commandCode,
	const u8_t  controlDataLength,
	const u8_t  * const parameters,
	const u32_t  parameterLength,
	u8_t  * const response,
	u32_t  * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetErrorDescription
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_GetErrorDescription
// description: get description for specific error code
// parameters:  errorCode - error code
// returns:     string with error description
//------------------------------------------------------------------------------
G_API_DLL
const char *
G_API
G_GetErrorDescription(
	const G_Error_t  errorCode
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetLastErrorCode
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_GetLastErrorCode
// description: get error code of last error
// parameters:  none
// returns:     error code
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_GetLastErrorCode(
	void
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetLastErrorDescription
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_GetLastErrorDescription
// description: get error description of last error
// parameters:  none
// returns:     string with error description
//------------------------------------------------------------------------------
G_API_DLL
const char *
G_API
G_GetLastErrorDescription(
	void
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_GetLastErrorDescriptionByPortHandle
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_GetLastErrorByPortHandle
// description: get error description of last error for specified port handle
// parameters:  portHandle - port handle
//              errorCode - error code
//              length - length of buffer / description
//              errorDescription - pointer to buffer for error description
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_GetLastErrorByPortHandle(
	const G_PortHandle_t  portHandle,
	G_Error_t * const errorCode,
	u32_t * const length,
	char * const errorDescription
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_Enable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

//lint -strong(,G_AsyncCommunication_ErrorCallback_t)
typedef void
(G_API * G_AsyncCommunication_ErrorCallback_t) (
	const G_PortHandle_t  portHandle,
	const G_Error_t  error,
	const u8_t * const errorInfo,
	const u32_t  errorInfoLength
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_AsyncCommunication_Enable
// description: global enable of asynchronous communication
// parameters:  portHandle - handle to interface
//              errorCallback - callback function when an error occured
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_AsyncCommunication_Enable(
	const G_PortHandle_t  portHandle,
	const G_AsyncCommunication_ErrorCallback_t  errorCallback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_Disable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_AsyncCommunication_Disable
// description: global disable of asynchronous communication
// parameters:  portHandle - handle to interface
// returns:     0 on success, not 0 on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_AsyncCommunication_Disable(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_FunctionsWithoutResponse_Sync
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_AsyncCommunication_FunctionsWithoutResponse_Sync
// description: enable synchronous execution of functions without a response
//              within asynchronous communication mode
// parameters:  portHandle - handle to interface
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_AsyncCommunication_FunctionsWithoutResponse_Sync(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_FunctionsWithoutResponse_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_AsyncCommunication_FunctionsWithoutResponse_Async
// description: enable synchronous execution of functions without a response
//              within asynchronous communication mode
// parameters:  portHandle - handle to interface
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_AsyncCommunication_FunctionsWithoutResponse_Async(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_AsyncCommunication_FunctionsWithoutResponse_IsSync
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_AsyncCommunication_FunctionsWithoutResponse_IsSync
// description: is synchronous execution of functions without a response
//              within asynchronous communication mode enabled
// parameters:  portHandle - handle to interface
// returns:     0 - not enabled, 1 - enabled
//------------------------------------------------------------------------------
G_API_DLL
u8_t
G_API
G_AsyncCommunication_FunctionsWithoutResponse_IsSync(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Ethernet_GetNetworkAdapterInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef struct {
	G_IpAddress_t  IpAddress;
	G_IpAddress_t  Mask;
} G_Ethernet_AdapterInfo_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Ethernet_GetNetworkAdapterInfo
// description: return ip network adapter information for all available adapters
// parameters:  numberOfAdapterInfos - in: size of buffer in adapter infos
//                out: number of adapter infos
//              adapterInfo - returns adapter information
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Ethernet_GetNetworkAdapterInfo(
	u32_t * const numberOfAdapterInfos,
	G_Ethernet_AdapterInfo_t * const adapterInfo
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Interface_GetInterfaceList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t  G_Interface_GetInterfaceList_CmdFlags_t;

#define G_INTERFACE__GET_INTERFACE_LIST__CMD_FLAG__ZERO  \
	((G_Interface_GetInterfaceList_CmdFlags_t) 0x00)

#define G_INTERFACE__GET_INTERFACE_LIST__CMD_FLAG__NO_ETHERNET  \
	((G_Interface_GetInterfaceList_CmdFlags_t) 0x01)


typedef u8_t  G_HostCommunicationType_t;

#define G_HOST_COMMUNICATION_TYPE__UNKNOWN  \
	((G_HostCommunicationType_t) 0x00)

#define G_HOST_COMMUNICATION_TYPE__PXI  \
	((G_HostCommunicationType_t) 0x01)

#define G_HOST_COMMUNICATION_TYPE__PCI  \
	((G_HostCommunicationType_t) 0x02)

#define G_HOST_COMMUNICATION_TYPE__USB  \
	((G_HostCommunicationType_t) 0x03)

#define G_HOST_COMMUNICATION_TYPE__ETHERNET  \
	((G_HostCommunicationType_t) 0x04)


// ATTENTION!
// For compatibility reasons, the item order must not be  changed!
typedef struct {
	G_InterfaceType_t  InterfaceType;
	u8_t  ControllerNumber;
	u8_t  InterfaceNumber;
	G_HostCommunicationType_t  HostCommunicationType;

	G_HostInterfaceType_t  HostInterfaceType;
	u32_t  Serial;
	u32_t  DeviceIndex;

	G_IpAddress_t  IpAddress;  // only relevant for ethernet devices
	u16_t  TcpPort;  // only relevant for ethernet devices
	u8_t  reserved1;
	u8_t  reserved2;
} G_InterfaceData_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Interface_GetInterfaceList
// description: query a list with all currently connected interfaces
// parameters:  cmdFlags - command flags
//              numberOfInterfaces - in: buffer size / out: number of interfaces
//              interfaceData - returns interface data
//              sizeOfInterfaceData - size of 'G_InterfaceData_t' in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Interface_GetInterfaceList(
	const G_Interface_GetInterfaceList_CmdFlags_t  cmdFlags,
	u32_t * const numberOfInterfaces,
	G_InterfaceData_t * const interfaceData,
	const u32_t  sizeOfInterfaceData
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Interface_GetInterfaceList_PciPxi
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Interface_GetInterfaceList_PciPxi
// description: query a list with all currently connected PCI/PXI interfaces
// parameters:  numberOfInterfaces - in: buffer size / out: number of interfaces
//              interfaceData - returns interface data
//              sizeOfInterfaceData - size of 'G_InterfaceData_t' in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Interface_GetInterfaceList_PciPxi(
	u32_t * const numberOfInterfaces,
	G_InterfaceData_t * const interfaceData,
	const u32_t  sizeOfInterfaceData
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Interface_GetInterfaceList_Usb
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Interface_GetInterfaceList_Usb
// description: query a list with all currently connected USB interfaces
// parameters:  numberOfInterfaces - in: buffer size / out: number of interfaces
//              interfaceData - returns interface data
//              sizeOfInterfaceData - size of 'G_InterfaceData_t' in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Interface_GetInterfaceList_Usb(
	u32_t * const numberOfInterfaces,
	G_InterfaceData_t * const interfaceData,
	const u32_t  sizeOfInterfaceData
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Interface_GetInterfaceList_Pxi61xx
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Interface_GetInterfaceList_Pxi61xx
// description: query a list with all currently connected Series61 interfaces
// parameters:  numberOfInterfaces - in: buffer size / out: number of interfaces
//              interfaceData - returns interface data
//              sizeOfInterfaceData - size of 'G_InterfaceData_t' in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Interface_GetInterfaceList_Pxi61xx(
	u32_t * const numberOfInterfaces,
	G_InterfaceData_t * const interfaceData,
	const u32_t  sizeOfInterfaceData
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Interface_GetInterfaceList_Ethernet
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t  G_Interface_GetInterfaceList_Ethernet_CmdFlags_t;

#define G_INTERFACE__GET_INTERFACE_LIST__ETHERNET__CMD_FLAG__NONE  \
	((G_Interface_GetInterfaceList_Ethernet_CmdFlags_t)  0x00)

#define G_INTERFACE__GET_INTERFACE_LIST__ETHERNET__CMD_FLAG__ALL  \
	((G_Interface_GetInterfaceList_Ethernet_CmdFlags_t)  0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Interface_GetInterfaceList_Ethernet
// description: query a list with all currently connected Ethernet interfaces
// parameters:  cmdFlags - command flags
//              numberOfInterfaces - in: buffer size / out: number of interfaces
//              interfaceData - returns interface data
//              sizeOfInterfaceData - size of 'G_InterfaceData_t' in bytes
//              adapterInfo - network adapter information
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Interface_GetInterfaceList_Ethernet(
	const G_Interface_GetInterfaceList_Ethernet_CmdFlags_t  cmdFlags,
	u32_t * const numberOfInterfaces,
	G_InterfaceData_t * const interfaceData,
	const u32_t  sizeOfInterfaceData,
	const G_Ethernet_AdapterInfo_t * const adapterInfo
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Interface_Name_Get
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Interface_Name_Get
// description: return the name for an interface
// parameters:  interfaceData - interface data
//              length - in: sizeof buffer 'name', out: name string length
//              name - returns name string
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_Interface_Name_Get(
	const G_InterfaceData_t * const interfaceData,
	u32_t * const length,
	char * const name
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_InterfaceConfig_GetInterfaceInfoByName
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_InterfaceConfig_GetInterfaceInfoByName
// description: return interface info for interface with given name
// parameters:  name - interface name
//              interfaceInfo - returns interface info
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_DLL
G_Error_t
G_API
G_InterfaceConfig_GetInterfaceInfoByName(
	const char * const name,
	G_InterfaceInfo_t * const interfaceInfo
);


#ifdef __cplusplus
	}
#endif

#endif
