#ifndef _G_API_COMMON_H
#define _G_API_COMMON_H


#ifndef _G_API_H
	#include "g_api.h"
#endif


#ifndef G_API_COMMON_DLL
	#ifdef G_API_COMMON_DLL_EXPORTS
		#define G_API_COMMON_DLL __declspec(dllexport)
	#else
		#define G_API_COMMON_DLL __declspec(dllimport)
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


//******************************************************************************
// common types and definitions
//******************************************************************************
typedef u8_t  G_Common_Signal_ByteOrder_t;

//lint -esym(755, G_COMMON__SIGNAL__BYTE_ORDER__LITTLE_ENDIAN)
#define G_COMMON__SIGNAL__BYTE_ORDER__LITTLE_ENDIAN  \
	((G_Common_Signal_ByteOrder_t) 0x00)

//lint -esym(755, G_COMMON__SIGNAL__BYTE_ORDER__BIG_ENDIAN)
#define G_COMMON__SIGNAL__BYTE_ORDER__BIG_ENDIAN  \
	((G_Common_Signal_ByteOrder_t) 0x01)


typedef struct {
	u8_t  Size;  // in bits
	G_Common_Signal_ByteOrder_t  ByteOrder;
	u16_t  Offset;  // in bits
} G_Common_Signal_t;


typedef u32_t  G_Common_EventHandle_t;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_OpenInterface
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_OpenInterface
// description: connect interface
// parameters:  interfaceName - user device name
//              portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_OpenInterface(
	const char * const interfaceName,
	G_PortHandle_t * const portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_OpenInterface2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t G_Common_OpenInterface2_CmdFlags_t;

#define G_COMMON__OPEN_INTERFACE_2__CMD_FLAG__NONE  \
	((G_Common_OpenInterface2_CmdFlags_t)  0x00)

#define G_COMMON__OPEN_INTERFACE_2__CMD_FLAG__DISABLE_AUTOMATIC_CONFIGURATION  \
	((G_Common_OpenInterface2_CmdFlags_t)  0x01)

#define G_COMMON__OPEN_INTERFACE_2__CMD_FLAG__NO_SOFTWARE_RESET  \
	((G_Common_OpenInterface2_CmdFlags_t)  0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_OpenInterface2
// description: connect interface with extra options
// parameters:  cmdFlags - command flags
//              interfaceName - user device name
//              portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_OpenInterface2(
	const G_Common_OpenInterface2_CmdFlags_t  cmdFlags,
	const char * const interfaceName,
	G_PortHandle_t * const portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_CloseInterface
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_CloseInterface
// description: close connection to interface
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_CloseInterface(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_InitInterface
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_InitInterface
// description: initialises interface
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_InitInterface(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetFirmwareVersion
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetFirmwareVersion
// description: get firmware version
// parameters:  portHandle - porthandle
//              response - response buffer
//              responseLength - size of response buffer / size of response
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetFirmwareVersion(
	const G_PortHandle_t portHandle,
	char * const response,
	u32_t * const responseLength

);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetFirmwareVersion_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

//lint -strong(,G_Common_GetFirmwareVersion_Async_Callback_t)
typedef void
(G_API * G_Common_GetFirmwareVersion_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const char * const version,
	const u32_t  versionLength
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetFirmwareVersion_Async_AddCallback
// description: add callback for 'G_Common_GetFirmwareVersion_Async'
// parameters:  portHandle - porthandle
//              callback - callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetFirmwareVersion_Async_AddCallback(
	const G_PortHandle_t portHandle,
	const G_Common_GetFirmwareVersion_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetFirmwareVersion_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetFirmwareVersion_Async_RemoveCallback
// description: remove callback for 'G_Common_GetFirmwareVersion_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetFirmwareVersion_Async_RemoveCallback(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetFirmwareVersion_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetFirmwareVersion_Async
// description: get firmware version in async mode
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetFirmwareVersion_Async(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetFirmwareErrorDescription
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetFirmwareErrorDescription
// description: get error description from firmware
// parameters:  portHandle - porthandle
//              errorCode - error code
//              response - response data bytes
//              responseLength - length of data in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetFirmwareErrorDescription(
	const G_PortHandle_t portHandle,
	const G_Error_t errorCode,
	char * const response,
	u32_t * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_SoftwareReset
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_SoftwareReset
// description: resets controller
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_SoftwareReset(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetControllerInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t  G_Common_GetControllerInfo_RspFlags_t;

#define G_COMMON__GET_CONTROLLER_INFO__RSP_FLAG__NONE  \
	((G_Common_GetControllerInfo_RspFlags_t)  0x00)

#define G_COMMON__GET_CONTROLLER_INFO__RSP_FLAG__IS_BOOT_LOADER  \
	((G_Common_GetControllerInfo_RspFlags_t)  0x01)


typedef u8_t  G_Common_ControllerFamily_t;

#define G_COMMON__CONTROLLER_FAMILY__UNKNOWN  \
	((G_Common_ControllerFamily_t)  0x00)

#define G_COMMON__CONTROLLER_FAMILY__C16X  \
	((G_Common_ControllerFamily_t)  0x01)

#define G_COMMON__CONTROLLER_FAMILY__TRICORE  \
	((G_Common_ControllerFamily_t)  0x02)

#define G_COMMON__CONTROLLER_FAMILY__PPC  \
	((G_Common_ControllerFamily_t)  0x03)


typedef u8_t  G_Common_ControllerType_t;

#define G_COMMON__CONTROLLER_TYPE__UNKNOWN  \
	((G_Common_ControllerType_t)  0x00)

#define G_COMMON__CONTROLLER_TYPE__C164CI  \
	((G_Common_ControllerType_t)  0x01)

#define G_COMMON__CONTROLLER_TYPE__TC1765  \
	((G_Common_ControllerType_t)  0x02)

#define G_COMMON__CONTROLLER_TYPE__TC1775B  \
	((G_Common_ControllerType_t)  0x03)

#define G_COMMON__CONTROLLER_TYPE__TC1796B  \
	((G_Common_ControllerType_t)  0x04)

#define G_COMMON__CONTROLLER_TYPE__PPC460EX  \
	((G_Common_ControllerType_t)  0x05)


typedef struct {
	G_HostInterfaceType_t  HostInterfaceType;

	G_Common_ControllerFamily_t  ControllerFamily;
	G_Common_ControllerType_t  ControllerType;
	u8_t  reserved1;
	u8_t  reserved2;

	G_Common_GetControllerInfo_RspFlags_t  Flags;
	u32_t  ResetStatusRegister;

	u32_t  HardwareVersion;
	u32_t  SoftwareVersion;

	u32_t  NumberOfInterfaces;

	struct {
		G_InterfaceType_t  InterfaceType;
		u8_t  InterfaceNumber;
	} Interfaces[MAX_PARAM_SIZE / 2];
} G_Common_GetControllerInfo_Rsp_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetControllerInfo
// description: get controller info
// parameters:  portHandle - porthandle
//              rsp - pointer to  response structure
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetControllerInfo(
	const G_PortHandle_t portHandle,
	G_Common_GetControllerInfo_Rsp_t * const rsp
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetControllerInfo_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

//lint -strong(,G_Common_GetControllerInfo_Async_Callback_t)
typedef void
(G_API * G_Common_GetControllerInfo_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const G_Common_GetControllerInfo_Rsp_t * const rsp
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetControllerInfo_Async_AddCallback
// description: add callback for 'G_Common_GetControllerInfo_Async'
// parameters:  portHandle - porthandle
//              callback - callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetControllerInfo_Async_AddCallback(
	const G_PortHandle_t portHandle,
	const G_Common_GetControllerInfo_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetControllerInfo_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetControllerInfo_Async_RemoveCallback
// description: remove callback for 'G_Common_GetControllerInfo_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetControllerInfo_Async_RemoveCallback(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetControllerInfo_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetControllerInfo_Async
// description: get controller info in async mode
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetControllerInfo_Async(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetInterfaceInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t  G_Common_Features_t;

#define G_COMMON__FEATURE__UNKNOWN  \
	((G_Common_Features_t)  0x000000)

// CAN TP
#define G_COMMON__FEATURE__CAN__TP__TP_1_6  \
	((G_Common_Features_t)  0x010000)

#define G_COMMON__FEATURE__CAN__TP__TP_2_0  \
	((G_Common_Features_t)  0x010001)

#define G_COMMON__FEATURE__CAN__TP__ISOTP  \
	((G_Common_Features_t)  0x010002)

#define G_COMMON__FEATURE__CAN__TP__GMLAN  \
	((G_Common_Features_t)  0x010003)

#define G_COMMON__FEATURE__CAN__TP__J1939  \
	((G_Common_Features_t)  0x010004)

// DIAG CAN
#define G_COMMON__FEATURE__DIAG__CAN__KWP2000__TP_1_6  \
	((G_Common_Features_t)  0x020000)

#define G_COMMON__FEATURE__DIAG__CAN__KWP2000__TP_2_0  \
	((G_Common_Features_t)  0x200001)

#define G_COMMON__FEATURE__DIAG__CAN__KWP2000__ISOTP  \
	((G_Common_Features_t)  0x200002)

#define G_COMMON__FEATURE__DIAG__CAN__GMLAN  \
	((G_Common_Features_t)  0x200003)

#define G_COMMON__FEATURE__DIAG__CAN__UDS__ISOTP  \
	((G_Common_Features_t)  0x200004)

#define G_COMMON__FEATURE__DIAG__CAN__J1939  \
	((G_Common_Features_t)  0x200005)

// DIAG LIN
#define G_COMMON__FEATURE__DIAG__LIN__RAW  \
	((G_Common_Features_t)  0x200100)

#define G_COMMON__FEATURE__DIAG__LIN__LIN_2_0  \
	((G_Common_Features_t)  0x200101)

// DIAG KLINE
#define G_COMMON__FEATURE__DIAG__KLINE__KWP2000  \
	((G_Common_Features_t)  0x200200)

#define G_COMMON__FEATURE__DIAG__KLINE__KWP1281  \
	((G_Common_Features_t)  0x200201)

#define G_COMMON__FEATURE__DIAG__KLINE__FORD  \
	((G_Common_Features_t)  0x200202)

// DDP CAN
#define G_COMMON__FEATURE__DDP__CAN__TP_1_6__ECU  \
	((G_Common_Features_t)  0x300000)

#define G_COMMON__FEATURE__DDP__CAN__TP_1_6__DISPLAY  \
	((G_Common_Features_t)  0x300001)

#define G_COMMON__FEATURE__DDP__CAN__TP_2_0__ECU  \
	((G_Common_Features_t)  0x300002)

#define G_COMMON__FEATURE__DDP__CAN__TP_2_0__DISPLAY  \
	((G_Common_Features_t)  0x300003)

// CUSTOM
#define G_COMMON__FEATURE__CUSTOM__LIN_ADVANCED_LIBRARY  \
	((G_Common_Features_t)  0x400000)

#define G_COMMON__FEATURE__CUSTOM__CCP  \
	((G_Common_Features_t)  0x400001)


typedef struct {
	G_InterfaceType_t  InterfaceType;
	u8_t  reserved1;
	u8_t  reserved2;
	u8_t  reserved3;

	u32_t  InterfaceVersion;
	u32_t  NumberOfFeatures;

	G_Common_Features_t  Features[32];
} G_Common_GetInterfaceInfo_Rsp_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetInterfaceInfo
// description: get interface info
// parameters:  portHandle - porthandle
//              rsp - pointer to response structure
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetInterfaceInfo(
	const G_PortHandle_t portHandle,
	G_Common_GetInterfaceInfo_Rsp_t * const rsp
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetInterfaceInfo_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

//lint -strong(,G_Common_GetInterfaceInfo_Async_Callback_t)
typedef void
(G_API * G_Common_GetInterfaceInfo_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const G_Common_GetInterfaceInfo_Rsp_t * const rsp
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetInterfaceInfo_Async_AddCallback
// description: add callback for 'G_Common_GetInterfaceInfo_Async'
// parameters:  portHandle - porthandle
//              callback - callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetInterfaceInfo_Async_AddCallback(
	const G_PortHandle_t portHandle,
	const G_Common_GetInterfaceInfo_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetInterfaceInfo_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetInterfaceInfo_Async_RemoveCallback
// description: remove callback for 'G_Common_GetInterfaceInfo_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetInterfaceInfo_Async_RemoveCallback(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetInterfaceInfo_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetInterfaceInfo_Async
// description: get interface info in async mode
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetInterfaceInfo_Async(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetTransceiverInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

typedef u16_t  G_Common_TransceiverType_t;

#define G_COMMON__TRANSCEIVER_TYPE__UNKNOWN  \
	((G_Common_TransceiverType_t)  0x0000)

// CAN
#define G_COMMON__TRANSCEIVER_TYPE__CAN__TJA1041  \
	((G_Common_TransceiverType_t)  0x0100)  // high speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__TJA1041A  \
	((G_Common_TransceiverType_t)  0x0101)  // high speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__82C252  \
	((G_Common_TransceiverType_t)  0x0102)  // low speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__AU5790  \
	((G_Common_TransceiverType_t)  0x0103)  // single wire

#define G_COMMON__TRANSCEIVER_TYPE__CAN__TJA1054  \
	((G_Common_TransceiverType_t)  0x0104)  // low speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__TJA1054A  \
	((G_Common_TransceiverType_t)  0x0105)  // low speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__TJA1050  \
	((G_Common_TransceiverType_t)  0x0106)  // high speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__82C250  \
	((G_Common_TransceiverType_t)  0x0107)  // high speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__82C251  \
	((G_Common_TransceiverType_t)  0x0108)  // high speed 24V

#define G_COMMON__TRANSCEIVER_TYPE__CAN__TLE6250  \
	((G_Common_TransceiverType_t)  0x0109)  // high speed

#define G_COMMON__TRANSCEIVER_TYPE__CAN__B10011S  \
	((G_Common_TransceiverType_t)  0x010A)  // low speed 24V

// LIN
#define G_COMMON__TRANSCEIVER_TYPE__LIN__TJA1020  \
	((G_Common_TransceiverType_t)  0x0200)

#define G_COMMON__TRANSCEIVER_TYPE__LIN__TLE6258  \
	((G_Common_TransceiverType_t)  0x0201)

#define G_COMMON__TRANSCEIVER_TYPE__LIN__TLE6259  \
	((G_Common_TransceiverType_t)  0x0202)

// KLINE
#define G_COMMON__TRANSCEIVER_TYPE__KLINE__L9637D  \
	((G_Common_TransceiverType_t)  0x0300)

// FLEXRAY
#define G_COMMON__TRANSCEIVER_TYPE__FLEXRAY__TJA1080  \
	((G_Common_TransceiverType_t)  0x0400)

#define G_COMMON__TRANSCEIVER_TYPE__FLEXRAY__AS8221  \
	((G_Common_TransceiverType_t)  0x0401)

// ADIO
#define G_COMMON__TRANSCEIVER_TYPE__ADIO__TYPE_1  \
	((G_Common_TransceiverType_t)  0x0500)

#define G_COMMON__TRANSCEIVER_TYPE__ADIO__TYPE_2  \
	((G_Common_TransceiverType_t)  0x0501)


typedef struct {
	u8_t  NumberOfTransceiverInfos;
	u8_t  reserved1;
	u8_t  reserved2;
	u8_t  reserved3;

	struct {
		u8_t  HardwarePosition;
		u8_t  reserved;
		G_Common_TransceiverType_t  TransceiverType;
	} TransceiverInfos[(MAX_PARAM_SIZE - 4) / 4];
} G_Common_GetTransceiverInfo_Rsp_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetTransceiverInfo
// description: get transceiver info
// parameters:  portHandle - porthandle
//              rsp - pointer to response structure
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetTransceiverInfo(
	const G_PortHandle_t portHandle,
	G_Common_GetTransceiverInfo_Rsp_t * const rsp
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetTransceiverInfo_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

//lint -strong(,G_Common_GetTransceiverInfo_Async_Callback_t)
typedef void
(G_API * G_Common_GetTransceiverInfo_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const G_Common_GetTransceiverInfo_Rsp_t * const rsp
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetTransceiverInfo_Async_AddCallback
// description: add callback for 'G_Common_GetTransceiverInfo_Async'
// parameters:  portHandle - porthandle
//              callback - callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetTransceiverInfo_Async_AddCallback(
	const G_PortHandle_t portHandle,
	const G_Common_GetTransceiverInfo_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetTransceiverInfo_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetTransceiverInfo_Async_RemoveCallback
// description: remove callback for 'G_Common_GetTransceiverInfo_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetTransceiverInfo_Async_RemoveCallback(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_GetTransceiverInfo_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_GetTransceiverInfo_Async
// description: get transceiver info in async mode
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_GetTransceiverInfo_Async(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Signal_Check
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Signal_Check
// description: check signal definition
// parameters:  portHandle - porthandle
//              signal - pointer to signal definition
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Signal_Check(
	const G_PortHandle_t  portHandle,
	const G_Common_Signal_t * const signal
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Signal_CheckAndTranslate
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Signal_CheckAndTranslate
// description: check and translate signal definition
// parameters:  portHandle - porthandle
//              signal - pointer to signal definition
//              byte - returns byte value
//              bit - returns bit value
//              bitLength - returns length in bits
//              byteOrder - returns byte order
// returns:     G_NO_ERROR on success, G_ERROR__DLL__COMMON__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Signal_CheckAndTranslate(
	const G_PortHandle_t  portHandle,
	const G_Common_Signal_t * const signal,
	u8_t * const byte,
	u8_t * const bit,
	u8_t * const bitLength,
	u8_t * const byteOrder
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Events_AllocateEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Events_AllocateEvent
// description: allocate event
// parameters:  portHandle - port handle
//              eventHandle - returns handle to allocated event
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Events_AllocateEvent(
	const G_PortHandle_t  portHandle,
	G_Common_EventHandle_t * const eventHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Events_DeallocateEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Events_DeallocateEvent
// description: free allocated event
// parameters:  portHandle - port handle
//              eventHandle - handle to event
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Events_DeallocateEvent(
	const G_PortHandle_t  portHandle,
	G_Common_EventHandle_t * const eventHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Events_WaitForSingleEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Events_WaitForSingleEvent
// description: wait for event
// parameters:  portHandle - port handle
//              eventHandle - handle to event
//              timeout - timeout in milliseconds (0 = infinite)
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Events_WaitForSingleEvent(
	const G_PortHandle_t  portHandle,
	const G_Common_EventHandle_t * const eventHandle,
	const u32_t  timeout
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Ethernet_IpAddress_Set
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Ethernet_IpAddress_Set
// description: set ip address
// parameters:  portHandle - port handle
//              ipAddress - ip address (4 bytes in little endian format)
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Ethernet_IpAddress_Set(
	const G_PortHandle_t  portHandle,
	const u8_t * const ipAddress
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Ethernet_IpAddress_Get
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Ethernet_IpAddress_Get
// description: get ip address
// parameters:  portHandle - port handle
//              ipAddress - returns ip address (4 bytes in little endian format)
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Ethernet_IpAddress_Get(
	const G_PortHandle_t  portHandle,
	u8_t * const ipAddress
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Ethernet_DHCP_Mode_Set
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Common_Ethernet_DHCP_Mode_t;

#define G_COMMON__ETHERNET__DHCP__MODE__DISABLED  \
	((G_Common_Ethernet_DHCP_Mode_t) 0x00)

#define G_COMMON__ETHERNET__DHCP__MODE__ENABLED  \
	((G_Common_Ethernet_DHCP_Mode_t) 0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Ethernet_DHCP_Mode_Set
// description: set DHCP mode
// parameters:  portHandle - port handle
//              mode - DHCP mode
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Ethernet_DHCP_Mode_Set(
	const G_PortHandle_t  portHandle,
	const G_Common_Ethernet_DHCP_Mode_t  mode
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Ethernet_DHCP_Mode_Get
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Ethernet_DHCP_Mode_Get
// description: get DHCP mode
// parameters:  portHandle - port handle
//              mode - returns DHCP mode
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Ethernet_DHCP_Mode_Get(
	const G_PortHandle_t  portHandle,
	G_Common_Ethernet_DHCP_Mode_t * const mode
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Common_Ethernet_GetNetworkAdapterInfo
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Common_Ethernet_GetNetworkAdapterInfo
// description: return ip network adapter information for all available adapters
// parameters:  numberOfAdapterInfos - in: size of buffer in adapter infos
//                out: number of adapter infos
//              adapterInfo - returns adapter information
// returns:     G_NO_ERROR on success, G_ERROR__DLL__API__... on error
//------------------------------------------------------------------------------
G_API_COMMON_DLL
G_Error_t
G_API
G_Common_Ethernet_GetNetworkAdapterInfo(
	u32_t * const numberOfAdapterInfos,
	G_Ethernet_AdapterInfo_t * const adapterInfo
);


#ifdef __cplusplus
}
#endif


#endif