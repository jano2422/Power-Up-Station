#ifndef _G_API_DIAG_H
#define _G_API_DIAG_H

#ifndef _G_API_H
	#include "g_api.h"
#endif

#ifdef G_API_DIAG_DLL_EXPORTS
	#define G_API_DIAG_DLL __declspec(dllexport)
#else
	#define G_API_DIAG_DLL __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------------------
// common parameters
//------------------------------------------------------------------------------

//request mode
typedef u8_t G_Diag_RequestMode_t;

#define G_DIAG__REQUEST_MODE__PHYSICAL_WITH_RESPONSE \
	((G_Diag_RequestMode_t) 0x00)

#define G_DIAG__REQUEST_MODE__FUNCTIONAL_WITH_RESPONSE \
	((G_Diag_RequestMode_t) 0x01)

#define G_DIAG__REQUEST_MODE__STOP_AFTER_PHYSICAL_WITH_RESPONSE \
	((G_Diag_RequestMode_t) 0x40)

#define G_DIAG__REQUEST_MODE__STOP_AFTER_FUNCTIONAL_WITH_RESPONSE \
	((G_Diag_RequestMode_t) 0x41)

#define G_DIAG__REQUEST_MODE__PHYSICAL_WITHOUT_RESPONSE \
	((G_Diag_RequestMode_t) 0x80)

#define G_DIAG__REQUEST_MODE__FUNCTIONAL_WITHOUT_RESPONSE \
	((G_Diag_RequestMode_t) 0x81)

#define G_DIAG__REQUEST_MODE__STOP_AFTER_PHYSICAL_WITHOUT_RESPONSE \
	((G_Diag_RequestMode_t) 0xC0)

#define G_DIAG__REQUEST_MODE__STOP_AFTER_FUNCTIONAL_WITHOUT_RESPONSE \
	((G_Diag_RequestMode_t) 0xC1)


//state
typedef u8_t G_Diag_State_t;

#define G_DIAG__STATE__NOT_INITIALIZED \
	((G_Diag_State_t) 0x00)

#define G_DIAG__STATE__DISCONNECTED \
	((G_Diag_State_t) 0x01)

#define G_DIAG__STATE__CONNECT_IN_PROGRESS \
	((G_Diag_State_t) 0x02)

#define G_DIAG__STATE__CONNECTED \
	((G_Diag_State_t) 0x03)

#define G_DIAG__STATE__DISCONNECT_IN_PROGRESS \
	((G_Diag_State_t) 0x04)


//command flags
typedef u32_t G_Diag_GetState_CmdFlags_t;

#define G_DIAG__GET_STATE__CMD_FLAG__NONE \
	((G_Diag_GetState_CmdFlags_t) 0x00)

#define G_DIAG__GET_STATE__CMD_FLAG__RESET_LAST_ERROR \
	((G_Diag_GetState_CmdFlags_t) 0x01)


//response flags
typedef u32_t G_Diag_GetState_RspFlags_t;

#define G_DIAG__GET_STATE__RSP_FLAG__NONE \
	((G_Diag_GetState_RspFlags_t) 0x00)

#define G_DIAG__GET_STATE__RSP_FLAG__BUSY \
	((G_Diag_GetState_RspFlags_t) 0x01)

#define G_DIAG__GET_STATE__RSP_FLAG__SYNC_RX_BUFFER_NOT_EMPTY \
	((G_Diag_GetState_RspFlags_t) 0x02)

#define G_DIAG__GET_STATE__RSP_FLAG__ASYNC_RX_BUFFER_NOT_EMPTY \
	((G_Diag_GetState_RspFlags_t) 0x04)

#define G_DIAG__GET_STATE__RSP_FLAG__UUDT_RX_BUFFER_NOT_EMPTY \
	((G_Diag_GetState_RspFlags_t) 0x08)


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_Start
// description: starts diagnosis
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - request mode
//              timeout - timeout for response and connect (ms)
//              requestData - request data bytes
//              requestLength - data length in bytes
//              lastErrorCode - code for last error
//              responseData -  response data bytes
//              responseLength - length of response
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_Start(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_RequestMode_t mode,
	const u32_t timeout,
	const u8_t * const requestData,
	const u32_t requestLength,
	G_Error_t * const lastErrorCode,
	u8_t * const responseData,
	u32_t * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_InitiateStart
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_InitiateStart
// description: initiates diagnosis startup
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - request mode
//              requestData - request data bytes
//              requestLength - data length in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_InitiateStart(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_RequestMode_t mode,
	const u8_t * const requestData,
	const u32_t requestLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_Stop
// description: stops diagnosis
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - request mode
//              timeout - timeout for response and disconnect (ms)
//              requestData - request data bytes
//              requestLength - data length in bytes
//              lastErrorCode - code for last error
//              responseData -  response data bytes
//              responseLength - length of response
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_Stop(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_RequestMode_t mode,
	const u32_t timeout,
	const u8_t * const requestData,
	const u32_t requestLength,
	G_Error_t * const lastErrorCode,
	u8_t * const responseData,
	u32_t * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_InitiateStop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_InitiateStop
// description: initiates diagnosis stop
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - request mode
//              requestData - request data bytes
//              requestLength - data length in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_InitiateStop(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_RequestMode_t mode,
	const u8_t * const requestData,
	const u32_t requestLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetState
// description: gets diagnostic state
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              cmdFlags - flags
//              lastErrorCode - last error code
//              state - diagnostic state
//              rspFlags - flags
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetState(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_GetState_CmdFlags_t cmdFlags,
	G_Error_t * const lastErrorCode,
	G_Diag_State_t * const state,
	G_Diag_GetState_RspFlags_t * const rspFlags
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetState_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

// callback
//lint -strong(,G_Diag_GetState_Async_Callback_t)
typedef void
(G_API * G_Diag_GetState_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	const G_Error_t  lastErrorCode,
	const G_Diag_State_t  state,
	const G_Diag_GetState_RspFlags_t  rspFlags
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetState_Async_AddCallback
// description: add callback for 'G_Diag_GetState_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              callback - user callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetState_Async_AddCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const G_Diag_GetState_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetState_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetState_Async_RemoveCallback
// description: renmove callback for 'G_Diag_GetState_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetState_Async_RemoveCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetState_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetState_Async
// description: gets diagnostic state (async)
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              cmdFlags - flags
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetState_Async(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_GetState_CmdFlags_t cmdFlags
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_Request
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_Request
// description: sends diagnostic request
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - request mode
//              timeout - timeout for response (ms)
//              requestData - request data bytes
//              requestLength - data length in bytes
//              lastErrorCode - code for last error
//              responseData -  response data bytes
//              responseLength - length of response
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_Request(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_RequestMode_t mode,
	const u32_t timeout,
	const u8_t * const requestData,
	const u32_t requestLength,
	G_Error_t * const lastErrorCode,
	u8_t * const responseData,
	u32_t * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_QueueRequest
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_QueueRequest
// description: queues diagnostic request
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - request mode
//              requestData - request data bytes
//              requestLength - data length in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_QueueRequest(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_RequestMode_t mode,
	const u8_t * const requestData,
	const u32_t requestLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetResponse
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetResponse
// description: gets diagnostic response
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              timeout - timeout in ms
//              state - diagnostic state
//              lastErrorCode - code for last error
//              responseData -  response data bytes
//              responseLength - length of response
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetResponse(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const u32_t timeout,
	G_Diag_State_t * const state,
	G_Error_t * const lastErrorCode,
	u8_t * const responseData,
	u32_t * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetResponse_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

// callback
//lint -strong(,G_Diag_GetResponse_Async_Callback_t)
typedef void
(G_API * G_Diag_GetResponse_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const G_Diag_State_t  state,
	const G_Error_t  lastErrorCode,
	const u8_t * const responseData,
	const u32_t  responseLength
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetResponse_Async_AddCallback
// description: add callback for 'G_Diag_GetResponse_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              callback - user callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetResponse_Async_AddCallback(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Diag_GetResponse_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetResponse_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetResponse_Async_RemoveCallback
// description: renmove callback for 'G_Diag_GetResponse_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetResponse_Async_RemoveCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetResponse_Async_Enable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetResponse_Async_Enable
// description: enable async diag responses
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetResponse_Async_Enable(
	const G_PortHandle_t portHandle,
	const u8_t  channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Diag_GetResponse_Async_Disable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Diag_GetResponse_Async_Disable
// description: disable async diag responses
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__DIAG__... on error
//------------------------------------------------------------------------------
G_API_DIAG_DLL
G_Error_t
G_API
G_Diag_GetResponse_Async_Disable(
	const G_PortHandle_t portHandle,
	const u8_t  channel
);


#ifdef __cplusplus
}
#endif


#endif