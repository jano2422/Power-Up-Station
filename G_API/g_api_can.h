#ifndef _G_API_CAN_H
#define _G_API_CAN_H


#ifndef _G_API_COMMON_H
	#include "g_api_common.h"
#endif


#ifndef G_API_CAN_DLL
	#ifdef G_API_CAN_DLL_EXPORTS
		#define G_API_CAN_DLL __declspec(dllexport)
	#else
		#define G_API_CAN_DLL __declspec(dllimport)
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


//******************************************************************************
// common parameters
//******************************************************************************
typedef G_Data_t  G_Can_Data_t;


typedef u8_t  G_Can_CyclicMsgs_Ramp_Mode_t;

#define G_CAN__CYCLIC_MSGS__RAMP__MODE__TRIANGULAR  \
	((G_Can_CyclicMsgs_Ramp_Mode_t) 0x00)

#define G_CAN__CYCLIC_MSGS__RAMP__MODE__SAWTOOTH  \
	((G_Can_CyclicMsgs_Ramp_Mode_t) 0x01)

#define G_CAN__CYCLIC_MSGS__RAMP__MODE__SAWTOOTH_WITH_OVERFLOW_BIT  \
	((G_Can_CyclicMsgs_Ramp_Mode_t) 0x02)

#define G_CAN__CYCLIC_MSGS__RAMP__MODE__STOP  \
	((G_Can_CyclicMsgs_Ramp_Mode_t) 0x03)


//******************************************************************************
// COMMON
//******************************************************************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_InitInterface
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
// command flags
typedef u32_t G_Can_InitInterface_CmdFlags_t;

#define G_CAN__INIT_INTERFACE__CMD_FLAG__NONE  \
	((G_Can_InitInterface_CmdFlags_t) 0x00)

#define G_CAN__INIT_INTERFACE__CMD_FLAG__ENABLE_ANALYZER  \
	((G_Can_InitInterface_CmdFlags_t) 0x01)

#define G_CAN__INIT_INTERFACE__CMD_FLAG__ENABLE_BLINKING  \
	((G_Can_InitInterface_CmdFlags_t) 0x02)

#define G_CAN__INIT_INTERFACE__CMD_FLAG__DISABLE_NO_ACK_PAUSES  \
	((G_Can_InitInterface_CmdFlags_t) 0x04)

//id mode
typedef u8_t G_Can_IdMode_t;

#define G_CAN__ID_MODE__STANDARD \
	((G_Can_IdMode_t) 0x00)

#define G_CAN__ID_MODE__EXTENDED \
	((G_Can_IdMode_t) 0x01)

#define G_CAN__ID_MODE__MIXED \
	((G_Can_IdMode_t) 0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_InitInterface
// description: initialises CAN interface
// parameters:  portHandle - porthandle
//              idMode - identifier mode (standard, extended, mixed)
//              cmdFlags - flags
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_InitInterface(
	const G_PortHandle_t portHandle,
	const G_Can_IdMode_t idMode,
	const G_Can_InitInterface_CmdFlags_t cmdFlags
);


//******************************************************************************
// NODE
//******************************************************************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_CalculateBaudrateRegisterValue
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_CalculateBaudrateRegisterValue
// description: calculates baudrate register value
// parameters:  portHandle - porthandle
//              baudrateRegisterValue - baudrate register value
//              baudrate - desired baudrate in hertz
//              samplePoint - sample point in percent (0=use default)
//              numberOfTimeQuants - number of timequants (0=use default)
//              tSeg1 - number of timequants before sample point (0=use default)
//              tSeg2 - number of timequants after sample point (0=use default)
//              sjw - number of timequants as syncronization jump width
// remarks:     the following rules apply:
//                  numberOfTimeQuants = 1 + tSeg1 + tSeg2
//                  samplePoint = 100.0 * ((1 + tSeg1) / numberOfTimeQuants)
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_CalculateBaudrateRegisterValue(
	const G_PortHandle_t portHandle,
	u32_t * const baudrateRegisterValue,
	const double baudrate,
	const double samplePoint,
	const u8_t  numberOfTimeQuants,
	const u8_t  tSeg1,
	const u8_t  tSeg2,
	const u8_t  sjw
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_SetBaudrateRegisterAndTransceiverType
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//transceiver type
typedef u8_t G_Can_Node_TransceiverType_t;

#define G_CAN__NODE__TRANSCEIVER_TYPE__HIGH_SPEED \
	((G_Can_Node_TransceiverType_t) 0x00)

#define G_CAN__NODE__TRANSCEIVER_TYPE__LOW_SPEED \
	((G_Can_Node_TransceiverType_t) 0x01)

#define G_CAN__NODE__TRANSCEIVER_TYPE__SINGLE_WIRE \
	((G_Can_Node_TransceiverType_t) 0x02)

#define G_CAN__NODE__TRANSCEIVER_TYPE__HIGH_LEVEL_LOW_SPEED \
	((G_Can_Node_TransceiverType_t) 0x03)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_SetBaudrateRegisterAndTransceiverType
// description: sets baudrate register
// parameters:  portHandle - porthandle
//              baudrateRegisterValue - value for baudrate register
//              transceiverType - type of transceiver
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_SetBaudrateRegisterAndTransceiverType(
	const G_PortHandle_t portHandle,
	const u32_t baudrateRegisterValue,
	const G_Can_Node_TransceiverType_t transceiverType
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_Baudrate_Set
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef struct {
	u8_t  SamplePoint_Min;  // minimum sample point, use 0 if you don't want to
	// specify this parameter

	u8_t  SamplePoint_Max;  // maximum sample point, use 0 if you don't want to
	// specify this parameter

	u8_t  NumberOfTimeQuanta_Min;  // minimum number of time quanta
	// (1 + TSeg1 + TSeg2 = 8..25), use 0 if you don't want to specify this
	// parameter

	u8_t  NumberOfTimeQuanta_Max;  // maximum number of time quanta
	// (1 + TSeg1 + TSeg2 = 8..25), use 0 if you don't want to specify this
	// parameter


	u8_t  TSeg1_Min;  // minimum number of time quanta before sample point minus
	// one (3..16), use 0 if you don't want to specify this parameter

	u8_t  TSeg1_Max;  // maximum number of time quanta before sample point minus
	// one (3..16), use 0 if you don't want to specify this parameter

	u8_t  TSeg2_Min;  // minimum number of time quanta after sample point
	// (2..8), use 0 if you don't want to specify this parameter

	u8_t  TSeg2_Max;  // maximum number of time quanta after sample point
	// (2..8), use 0 if you don't want to specify this parameter


	u8_t  Sjw_Min;  // minimum resyncronization jump width (1..4), use 0 if you
	// don't want to specify this parameter

	u8_t  Sjw_Max;  // maximum resyncronization jump width (1..4), use 0 if you
	// don't want to specify this parameter

	u8_t  reserved1;
	u8_t  reserved2;
} G_Can_Node_Baudrate_Set_ExtendedParam_t;


typedef struct {
	u32_t  Baudrate;  // baudrate in baud (e.g. 500000 for 500 KBaud)
	u32_t  CanControllerClock;  // CAN controller clock frequency in Hz

	u8_t  SamplePoint;  // sample point
	u8_t  NumberOfTimeQuanta;  // number of time quanta (1 + TSeg1 + TSeg2)
	u8_t  TSeg1;  // number of time quanta before sample point minus one
	u8_t  TSeg2;  // number of time quanta after sample point

	u8_t  Sjw;  // resyncronization jump width
	u8_t  reserved1;
	u8_t  reserved2;
	u8_t  reserved3;
} G_Can_Node_Baudrate_ActualValues_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_Baudrate_Set
// description: set baudrate
// parameters:  portHandle - porthandle
//              baudrate - baudrate in baud (e.g. 500000 for 500 KBaud)
//              extendedParam - pointer to extended baudrate parameters
//              actualValues - response with actual baudrate values
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_Baudrate_Set(
	const G_PortHandle_t portHandle,
	const u32_t  baudrate,
	const G_Can_Node_Baudrate_Set_ExtendedParam_t * const extendedParam,
	G_Can_Node_Baudrate_ActualValues_t * const actualValues
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_Baudrate_Get
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_Baudrate_Get
// description: get baudrate values
// parameters:  portHandle - porthandle
//              actualValues - response with actual baudrate values
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_Baudrate_Get(
	const G_PortHandle_t portHandle,
	G_Can_Node_Baudrate_ActualValues_t * const actualValues
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_ErrorFrameCounter_Get
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_ErrorFrameCounter_Get
// description: get error frame counter
// parameters:  portHandle - porthandle
//              errorFrameCounter - error frame counter
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_ErrorFrameCounter_Get(
	const G_PortHandle_t portHandle,
	u32_t * const errorFrameCounter
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_ErrorFrameCounter_Reset
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_ErrorFrameCounter_Reset
// description: reset error frame counter
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_ErrorFrameCounter_Reset(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_GetState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Node_GetState_Mode_t;

#define G_CAN__NODE__GET_STATE__MODE__STANDARD  \
	((G_Can_Node_GetState_Mode_t) 0x00)


typedef u8_t  G_Can_Node_InitState_t;

#define G_CAN__NODE__INIT_STATE__OK  \
	((G_Can_Node_InitState_t) 0x00)

#define G_CAN__NODE__INIT_STATE__ERROR_WARNING  \
	((G_Can_Node_InitState_t) 0x01)

#define G_CAN__NODE__INIT_STATE__NO_ACK_WAIT  \
	((G_Can_Node_InitState_t) 0x02)

#define G_CAN__NODE__INIT_STATE__BUS_OFF  \
	((G_Can_Node_InitState_t) 0x03)

#define G_CAN__NODE__INIT_STATE__BUS_OFF_WAIT  \
	((G_Can_Node_InitState_t) 0x04)


typedef u8_t  G_Can_Node_PinState_t;

#define G_CAN__NODE__PIN_STATE__LOW  \
	((G_Can_Node_PinState_t)  0x00)

#define G_CAN__NODE__PIN_STATE__HIGH  \
	((G_Can_Node_PinState_t)  0x01)

#define G_CAN__NODE__PIN_STATE__NOT_AVAILABLE  \
	((G_Can_Node_PinState_t)  0x02)


typedef u32_t  G_Can_Node_GetState_RspFlags_t;

#define G_CAN__NODE__GET_STATE__RSP_FLAG__NONE  \
	((G_Can_Node_GetState_RspFlags_t) 0x00)

#define G_CAN__NODE__GET_STATE__RSP_FLAG__ERROR_WARNING  \
	((G_Can_Node_GetState_RspFlags_t) 0x01)

#define G_CAN__NODE__GET_STATE__RSP_FLAG__BUS_OFF  \
	((G_Can_Node_GetState_RspFlags_t) 0x02)

typedef struct {
	u8_t  RxErrorCounter;
	u8_t  TxErrorCounter;
	u8_t  ErrorWarningLevel;
	G_Can_Node_InitState_t  InitState;

	G_Can_Node_GetState_RspFlags_t  Flags;

	G_Can_Node_PinState_t  TransceiverPin_Enable;
	G_Can_Node_PinState_t  TransceiverPin_NotStandby;
	G_Can_Node_PinState_t  TransceiverPin_NotWake;
	G_Can_Node_PinState_t  TransceiverPin_NotError;
} G_Can_Node_GetState_Rsp_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_GetState
// description: get node state
// parameters:  portHandle - porthandle
//              mode - command mode
//              rsp - pointer to response structure
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_GetState(
	const G_PortHandle_t portHandle,
	const G_Can_Node_GetState_Mode_t  mode,
	G_Can_Node_GetState_Rsp_t * const rsp
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_Transceiver_SetMode
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Node_Transceiver_Mode_t;

#define G_CAN__NODE__TRANSCEIVER__MODE__NORMAL  \
	((G_Can_Node_Transceiver_Mode_t)  0x00)

#define G_CAN__NODE__TRANSCEIVER__MODE__SLEEP  \
	((G_Can_Node_Transceiver_Mode_t)  0x01)

#define G_CAN__NODE__TRANSCEIVER__MODE__WAKE_UP  \
	((G_Can_Node_Transceiver_Mode_t)  0x02)

#define G_CAN__NODE__TRANSCEIVER__MODE__HIGH_SPEED  \
	((G_Can_Node_Transceiver_Mode_t)  0x03)

#define G_CAN__NODE__TRANSCEIVER__MODE__SINGLE_WIRE_ON_CAN_HIGH  \
	((G_Can_Node_Transceiver_Mode_t)  0x04)

#define G_CAN__NODE__TRANSCEIVER__MODE__SINGLE_WIRE_ON_CAN_LOW  \
	((G_Can_Node_Transceiver_Mode_t)  0x05)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_Transceiver_SetMode
// description: set transceiver mode
// parameters:  portHandle - porthandle
//              mode - transceiver mode
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_Transceiver_SetMode(
	const G_PortHandle_t portHandle,
	const G_Can_Node_Transceiver_Mode_t  mode
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_TxPath_Control
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Node_TxPath_Mode_t;

#define G_CAN__NODE__TX_PATH__MODE__DISABLE  \
	((G_Can_Node_TxPath_Mode_t)  0x00)

#define G_CAN__NODE__TX_PATH__MODE__ENABLE  \
	((G_Can_Node_TxPath_Mode_t)  0x01)

#define G_CAN__NODE__TX_PATH__MODE__NORMAL_OPERATION  \
	((G_Can_Node_TxPath_Mode_t)  0x02)

#define G_CAN__NODE__TX_PATH__MODE__RECESSIVE_LEVEL  \
	((G_Can_Node_TxPath_Mode_t)  0x03)

#define G_CAN__NODE__TX_PATH__MODE__DOMINANT_LEVEL  \
	((G_Can_Node_TxPath_Mode_t)  0x04)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_TxPath_Control
// description: control hardware tx path
// parameters:  portHandle - porthandle
//              mode - tx path mode
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_TxPath_Control(
	const G_PortHandle_t portHandle,
	const G_Can_Node_TxPath_Mode_t  mode
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_BusTermination_Enable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_BusTermination_Enable
// description: enable onboard bus termination
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_BusTermination_Enable(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_BusTermination_Disable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_BusTermination_Disable
// description: disable onboard bus termination
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_BusTermination_Disable(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_InternalVBat_Enable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_InternalVBat_Enable
// description: enable onboard transceiver supply voltage
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_InternalVBat_Enable(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Node_InternalVBat_Disable
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Node_InternalVBat_Disable
// description: disable onboard transceiver supply voltage
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Node_InternalVBat_Disable(
	const G_PortHandle_t portHandle
);


//******************************************************************************
// TXFIFO
//******************************************************************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_GetState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_GetState
// description: gets tx fifo state
// parameters:  portHandle - porthandle
//              usedEntries - number of used entries
//              freeEntries - number of free entries
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_GetState(
	const G_PortHandle_t portHandle,
	u32_t * const usedEntries,
	u32_t * const freeEntries
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_GetState_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
// callback
//lint -strong(,G_Can_TxFifo_GetState_Async_Callback_t)
typedef void
(G_API * G_Can_TxFifo_GetState_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u32_t  usedEntries,
	const u32_t  freeEntries
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_GetState_Async_AddCallback
// description: add callback for 'G_Can_TxFifo_GetState_Async'
// parameters:  portHandle - porthandle
//              callback - user callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_GetState_Async_AddCallback(
	const G_PortHandle_t  portHandle,
	const G_Can_TxFifo_GetState_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_GetState_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_GetState_Async_RemoveCallback
// description: renmove callback for ' G_Can_TxFifo_GetState_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_GetState_Async_RemoveCallback(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_GetState_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_GetState_Async
// description: gets tx fifo state (async)
// parameters:  portHandle - porthandle
//              usedEntries - number of used entries
//              freeEntries - number of free entries
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_GetState_Async(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_Reset
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_Reset
// description: resets tx fifo
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_Reset(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_QueueMsg
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_QueueMsg
// description: queues message (tx fifo)
// parameters:  portHandle - porthandle
//              id - message identifier
//              data - data bytes 0..7
//              dlc - dlc for CAN messages
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_QueueMsg(
	const G_PortHandle_t portHandle,
	const u32_t id,
	const u8_t data[8],
	const u8_t dlc
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_TxFifo_QueueMsgs
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//message item
typedef struct {
	u32_t Id;
	u8_t Dlc;
	u8_t reserved1;
	u8_t reserved2;
	u8_t reserved3;
	u8_t Data[8];
} G_Can_TxFifo_MsgItem_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_TxFifo_QueueMsgs
// description: queues messages (tx fifo)
// parameters:  portHandle - porthandle
//              timeout - timeout for queuing messages
//              numberOfItems - items count
//              msgItems - message items
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_TxFifo_QueueMsgs(
	const G_PortHandle_t portHandle,
	const u32_t timeout,
	const u32_t numberOfItems,
	const G_Can_TxFifo_MsgItem_t * const msgItems
);


//******************************************************************************
// CYCLIC MESSAGES
//******************************************************************************

//------------------------------------------------------------------------------
// common parameters
//------------------------------------------------------------------------------
//change mode
typedef u8_t G_Can_CyclicMsgs_ChangeMsgData_ChangeMode_t;

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG_DATA__CHANGE_MODE__IN_CYCLE \
	((G_Can_CyclicMsgs_ChangeMsgData_ChangeMode_t) 0x00)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG_DATA__CHANGE_MODE__IMMEDIATELY \
	((G_Can_CyclicMsgs_ChangeMsgData_ChangeMode_t) 0x01)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG_DATA__CHANGE_MODE__WITH_MINIMUM_DELAY \
	((G_Can_CyclicMsgs_ChangeMsgData_ChangeMode_t) 0x02)


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_DefineMsg
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//send state
typedef u8_t G_Can_CyclicMsgs_SendState_t;

#define G_CAN__CYCLIC_MSGS__SEND_STATE__DONT_SEND_MSG \
	((G_Can_CyclicMsgs_SendState_t) 0x00)

#define G_CAN__CYCLIC_MSGS__SEND_STATE__SEND_MSG \
	((G_Can_CyclicMsgs_SendState_t) 0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_DefineMsg
// description: defines message
// parameters:  portHandle - porthandle
//              id - message identifier
//              cycleTime - cycle time in ms
//              sendState - message output on/off
//              group - message group
//              msgCount - quantity of send repetitions (0=endless)
//              data - data bytes 0..7
//              dlc - dlc for CAN messages
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_DefineMsg(
	const G_PortHandle_t portHandle,
	const u32_t id,
	const u16_t cycleTime,
	const G_Can_CyclicMsgs_SendState_t sendState,
	const u8_t group,
	const u8_t msgCount,
	const u8_t data[8],
	const u8_t dlc
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_DeleteMsg
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_DeleteMsg
// description: deletes message
// parameters:  portHandle - porthandle
//              id - message identifier
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_DeleteMsg(
	const G_PortHandle_t portHandle,
	const u32_t id
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_DefineDisturbance
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_DefineDisturbance
// description: set temporarily failures
// parameters:  portHandle - porthandle
//              id - message identifier
//              dlc - data length (0..8)
//              count - number of faults
//              mask - mask bytes 0..7
//              data - data bytes 0..7
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_DefineDisturbance(
	const G_PortHandle_t portHandle,
	const u32_t id,
	const u8_t  dlc,
	const u16_t  count,
	const G_Can_Data_t  mask,
	const G_Can_Data_t  data
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_ChangeMsg
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command control flags
typedef u32_t G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t;

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__CMD_FLAG__NONE \
	((G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t) 0x00)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__CMD_FLAG__CHANGE_SEND_STATE \
	((G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t) 0x01)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__CMD_FLAG__CHANGE_GROUP \
	((G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t) 0x02)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__CMD_FLAG__CHANGE_MSG_COUNT \
	((G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t) 0x04)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__CMD_FLAG__CHANGE_DLC \
	((G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t) 0x08)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__CMD_FLAG__CHANGE_DATA \
	((G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t) 0x10)

//command data flags
typedef u32_t G_Can_CyclicMsgs_ChangeMsg_Data_CmdFlags_t;

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__DATA__CMD_FLAG__NONE \
	((G_Can_CyclicMsgs_ChangeMsg_Data_CmdFlags_t) 0x00)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__DATA__CMD_FLAG__USE_MIN_DELAY \
	((G_Can_CyclicMsgs_ChangeMsg_Data_CmdFlags_t) 0x01)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__DATA__CMD_FLAG__SAVE_CURRENT_DATA \
	((G_Can_CyclicMsgs_ChangeMsg_Data_CmdFlags_t) 0x02)

#define G_CAN__CYCLIC_MSGS__CHANGE_MSG__DATA__CMD_FLAG__RESTORE_SAVED_DATA \
	((G_Can_CyclicMsgs_ChangeMsg_Data_CmdFlags_t) 0x04)


//command parameters
typedef struct {
	u32_t Id;
	G_Can_CyclicMsgs_ChangeMsg_CmdFlags_t  Flags;

	G_Can_CyclicMsgs_SendState_t SendState;
	u8_t Group;
	u8_t MsgCount;
	u8_t Dlc;

	struct {
		G_Can_CyclicMsgs_ChangeMsg_Data_CmdFlags_t  Flags;
		u8_t Mask[8];
		u8_t Data[8];
		u16_t MinimumDelay;
		u16_t NumberOfMsgsBeforeRestore;
	} Data;
} G_Can_CyclicMsgs_ChangeMsg_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_ChangeMsg
// description: changes message
// parameters:  portHandle - porthandle
//              changeMsgParameters - command parameters
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_ChangeMsg(
	const G_PortHandle_t portHandle,
	const G_Can_CyclicMsgs_ChangeMsg_Parameters_t * const changeMsgParameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_ChangeMsgData
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_ChangeMsgData
// description: changes message data
// parameters:  portHandle - porthandle
//              id - message identifier
//              mask - mask bytes 0..7
//              dlc - dlc for CAN messages
//              data - data bytes 0..7
//              changeMode - cyclic, immediately, with delay
//              minimumDelay - minimum delay
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_ChangeMsgData(
	const G_PortHandle_t portHandle,
	const u32_t id,
	const u8_t mask[8],
	const u8_t dlc,
	const u8_t data[8],
	const G_Can_CyclicMsgs_ChangeMsgData_ChangeMode_t changeMode,
	const u16_t minimumDelay
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_ChangeMsgDataAndMsgCount
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_ChangeMsgDataAndMsgCount
// description: changes message data and count
// parameters:  portHandle - porthandle
//              id - message identifier
//              msgCount - send repetitions quantity (0=endless)
//              mask - mask bytes 0..7
//              dlc - dlc for CAN messages
//              data - data bytes 0..7
//              immediateChange - cyclic, immediately, with delay
//              minimumDelay - minimum delay
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_ChangeMsgDataAndMsgCount(
	const G_PortHandle_t portHandle,
	const u32_t id,
	const u8_t msgCount,
	const u8_t mask[8],
	const u8_t dlc,
	const u8_t data[8],
	const G_Can_CyclicMsgs_ChangeMsgData_ChangeMode_t changeMode,
	const u16_t minimumDelay
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_ChangeGroupNumber
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_ChangeGroupNumber
// description: changes group (changes prepare mode)
// parameters:  portHandle - porthandle
//              id - message id
//              group - number of group
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_ChangeGroupNumber(
	const G_PortHandle_t portHandle,
	const u32_t id,
	const u8_t group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_StartMsg
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_StartMsg
// description: starts message (changes message mode)
// parameters:  portHandle - porthandle
//              id - message identifier
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_StartMsg(
	const G_PortHandle_t portHandle,
	const u32_t id
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_StopMsg
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_StopMsg
// description: stops message (changes message mode)
// parameters:  portHandle - porthandle
//              id - message identifier
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_StopMsg(
	const G_PortHandle_t portHandle,
	const u32_t id
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_GetMsgState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_CyclicMsgs_MsgMode_t;

#define G_CAN__CYCLIC_MSGS__MSG_MODE__STOPPED  \
	((G_Can_CyclicMsgs_MsgMode_t) 0x00)

#define G_CAN__CYCLIC_MSGS__MSG_MODE__STARTED  \
	((G_Can_CyclicMsgs_MsgMode_t) 0x01)


typedef struct {
	u32_t  Id;

	u16_t  CycleTime;
	u8_t  Group;
	G_Can_CyclicMsgs_MsgMode_t  Mode;

	u8_t  MsgCount;
	u8_t  Dlc;
	u8_t  reserved1;
	u8_t  reserved2;

	G_Can_Data_t Data;
} G_Can_CyclicMsgs_GetMsgState_Rsp_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_GetMsgState
// description: stops message (changes message mode)
// parameters:  portHandle - porthandle
//              id - message identifier
//              rsp - pointer to response structure
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_GetMsgState(
	const G_PortHandle_t portHandle,
	const u32_t id,
	G_Can_CyclicMsgs_GetMsgState_Rsp_t * const rsp
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_StartGroup
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_StartGroup
// description: starts message group (changes message mode)
// parameters:  portHandle - porthandle
//              group - message group
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_StartGroup(
	const G_PortHandle_t portHandle,
	const u8_t group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_StopGroup
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_StopGroup
// description: stops message group (changes message mode)
// parameters:  portHandle - porthandle
//              group - message group
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_StopGroup(
	const G_PortHandle_t portHandle,
	const u8_t group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_MultiplexBytes_Define
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_MultiplexBytes_Define
// description: define multiplex bytes
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              updateRate - update rate
//              numberOfIndices - number of multiplex indices
//              mask - pointer mask bytes 0..7
//              data - pointer to data bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_MultiplexBytes_Define(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const u8_t  updateRate,
	const u8_t  numberOfIndices,
	const G_Can_Data_t * const mask,
	const G_Can_Data_t * const data
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_MultiplexBytes_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_MultiplexBytes_Stop
// description: stop multiplex bytes
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              mask - pointer to mask bytes 0..7
//              data - pointer to data bytes 0..7
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_MultiplexBytes_Stop(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Can_Data_t * const mask,
	const G_Can_Data_t * const data
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Ramp_Define
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_CyclicMsgs_Ramp_Direction_t;

#define G_CAN__CYCLIC_MSGS__RAMP__DIRECTION__DOWN  \
	((G_Can_CyclicMsgs_Ramp_Direction_t) 0x00)

#define G_CAN__CYCLIC_MSGS__RAMP__DIRECTION__UP  \
	((G_Can_CyclicMsgs_Ramp_Direction_t) 0x01)


typedef struct {
	u32_t  Id;
	G_Common_Signal_t  Signal;

	G_Can_CyclicMsgs_Ramp_Mode_t  Mode;
	G_Can_CyclicMsgs_Ramp_Direction_t  Direction;
	u16_t  MinValue;

	u16_t  MaxValue;
	u16_t  UpdateRate;

	float64_t  Increment;

	u32_t  RampSum;
	u16_t  NumberOfFlanks;
	u8_t  reserved1;
	u8_t  reserved2;
} G_Can_CyclicMsgs_Ramp_Define_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Ramp_Define
// description: ramp definition
// parameters:  portHandle - porthandle
//              parameters - ramp definition parameters
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Ramp_Define(
	const G_PortHandle_t portHandle,
	const G_Can_CyclicMsgs_Ramp_Define_Parameters_t * const parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Ramp_Delete
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Ramp_Delete
// description: delete ramp
// parameters:  portHandle - porthandle
//              id - message id
//              signal - signal definition
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Ramp_Delete(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Ramp_ChangeMode
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef struct {
	u32_t  Id;
	G_Common_Signal_t  Signal;
	G_Can_CyclicMsgs_Ramp_Mode_t  Mode;
} G_Can_CyclicMsgs_Ramp_ChangeMode_Ramps_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Ramp_ChangeMode
// description: change ramp mode
// parameters:  portHandle - porthandle
//              numberOfRamps - number of ramps
//              ramps - pointer to ramp array
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Ramp_ChangeMode(
	const G_PortHandle_t portHandle,
	const u32_t  numberOfRamps,
	const G_Can_CyclicMsgs_Ramp_ChangeMode_Ramps_t * const  ramps
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Ramp_GetState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u32_t  G_Can_CyclicMsgs_Ramp_State_Flags_t;

#define G_CAN__CYCLIC_MSGS__RAMP__STATE__FLAG__NONE  \
	((G_Can_CyclicMsgs_Ramp_State_Flags_t) 0x00)

#define G_CAN__CYCLIC_MSGS__RAMP__STATE__FLAG__RUNNING  \
	((G_Can_CyclicMsgs_Ramp_State_Flags_t) 0x01)

#define G_CAN__CYCLIC_MSGS__RAMP__STATE__FLAG__DIRECTION_IS_UP  \
	((G_Can_CyclicMsgs_Ramp_State_Flags_t) 0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Ramp_GetState
// description: get ramp state
// parameters:  portHandle - porthandle
//              id - message id
//              signal - signal definition
//              rampState - ramp state
//              incRampState - increment ramp state
//              rampSum - current internal ramp sum
//              incRampFlanks - number of already executed increment ramp flanks
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Ramp_GetState(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal,
	G_Can_CyclicMsgs_Ramp_State_Flags_t * const rampState,
	G_Can_CyclicMsgs_Ramp_State_Flags_t * const incRampState,
	u32_t * const rampSum,
	u16_t * const incRampFlanks
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Ramp_DefineIncRamp
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef struct {
	u32_t  Id;
	G_Common_Signal_t  Signal;

	G_Can_CyclicMsgs_Ramp_Mode_t  Mode;
	u8_t  reserved1;
	u8_t  reserved2;
	u8_t  reserved3;

	u16_t  MinValue;
	u16_t  MaxValue;
} G_Can_CyclicMsgs_Ramp_SyncRamp_t;


typedef struct {
	u32_t  Id;
	G_Common_Signal_t  Signal;

	u16_t  MinValue;
	u16_t  MaxValue;
	u16_t  CycleTime;
	u16_t  NumberOfFlanks;

	G_Can_CyclicMsgs_Ramp_Direction_t  Direction;
	u8_t  reserved1;
	u8_t  reserved2;
	u8_t  reserved3;

	G_Can_CyclicMsgs_Ramp_SyncRamp_t * SyncRamp;
} G_Can_CyclicMsgs_Ramp_DefineIncRamp_Parameters_t;

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Ramp_DefineIncRamp
// description: define increment ramp
// parameters:  portHandle - porthandle
//              parameters - command parameters
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Ramp_DefineIncRamp(
	const G_PortHandle_t portHandle,
	const G_Can_CyclicMsgs_Ramp_DefineIncRamp_Parameters_t * const parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Ramp_ResetRampSum
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Ramp_ResetRampSum
// description: reset ramp sum
// parameters:  portHandle - porthandle
//              id - message id
//              signal - signal definition
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Ramp_ResetRampSum(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Parity_Define
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_CyclicMsgs_Parity_Mode_t;

#define G_CAN__CYCLIC_MSGS__PARITY__MODE__EVEN  \
	((G_Can_CyclicMsgs_Parity_Mode_t) 0x00)

#define G_CAN__CYCLIC_MSGS__PARITY__MODE__ODD  \
	((G_Can_CyclicMsgs_Parity_Mode_t) 0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Parity_Define
// description: parity bit activation
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              mode - parity mode
//              signal - signal definition
//              mask - mask bytes 0..7
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Parity_Define(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Can_CyclicMsgs_Parity_Mode_t  mode,
	const G_Common_Signal_t  signal,
	const G_Can_Data_t  mask
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Parity_Delete
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Parity_Delete
// description: parity bit deactivation
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              signal - signal definition
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Parity_Delete(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Checksum_Define
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_CyclicMsgs_Checksum_Mode_t;

#define G_CAN__CYCLIC_MSGS__CHECKSUM__MODE__XOR  \
	((G_Can_CyclicMsgs_Checksum_Mode_t) 0x00)

#define G_CAN__CYCLIC_MSGS__CHECKSUM__MODE__CRC8_J1850  \
	((G_Can_CyclicMsgs_Checksum_Mode_t) 0x01)

#define G_CAN__CYCLIC_MSGS__CHECKSUM__MODE__ADD_AND_COMPLEMENT_TO_ONE  \
	((G_Can_CyclicMsgs_Checksum_Mode_t) 0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Checksum_Define
// description: define checksum
// parameters:  portHandle - porthandle
//              mode - checksum mode
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              signal - signal definition
//              mask - mask bytes for message data
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Checksum_Define(
	const G_PortHandle_t portHandle,
	const G_Can_CyclicMsgs_Checksum_Mode_t  mode,
	const u32_t  id,
	const G_Common_Signal_t  signal,
	const G_Can_Data_t * const mask
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Checksum_Define_XorWithUserBytes
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Checksum_Define_XorWithUserBytes
// description: define checksum XOR with user bytes
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              signal - signal definition
//              mask - mask bytes for message data
//              numberOfBytes - number of user bytes
//              bytes - pointer to user bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Checksum_Define_XorWithUserBytes(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal,
	const G_Can_Data_t * const mask,
	const u8_t  numberOfBytes,
	const u8_t * const bytes
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Checksum_Define_GeneralCrcWithUserBytes
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef struct {
	u8_t  Order;  // 8, 16
	u8_t  NumberOfUserBytesBefore;  // number of user bytes before
	// checksum calculation over data bytes
	u8_t  NumberOfUserBytesAfter;  // number of user bytes after
	// checksum calculation over data bytes
	u8_t  reserved;

	u32_t  Polynomial;  // e.g. 0x1D for CRC8-J1850
	u32_t  InitialValue;  // e.g. 0xFF for CRC8-J1850
	u32_t  FinalXorValue;  // e.g. 0xFF for CRC8-J1850

	u8_t * UserBytesBefore;
	u8_t * UserBytesAfter;
} G_Can_CyclicMsgs_Checksum_Define_GeneralCrc_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Checksum_Define_GeneralCrcWithUserBytes
// description: define checksum General Crc with user bytes
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              signal - signal definition
//              mask - mask bytes for message data
//              numberOfBytes - number of user bytes
//              bytes - pointer to user bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Checksum_Define_GeneralCrcWithUserBytes(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal,
	const G_Can_Data_t * const mask,
	const G_Can_CyclicMsgs_Checksum_Define_GeneralCrc_Parameters_t *
		const parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_CyclicMsgs_Checksum_Delete
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_CyclicMsgs_Checksum_Delete
// description: delete checksum
// parameters:  portHandle - porthandle
//              id - message identifier (0..0x7FF  or  0..0x1FFFFFFF)
//              signal - signal definition
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_CyclicMsgs_Checksum_Delete(
	const G_PortHandle_t portHandle,
	const u32_t  id,
	const G_Common_Signal_t  signal
);


//******************************************************************************
// MONITOR
//******************************************************************************

//------------------------------------------------------------------------------
// common parameters
//------------------------------------------------------------------------------
//response flags
typedef u32_t G_Can_Monitor_ListMode_GetItem_RspFlags_t;

#define G_CAN__MONITOR__LIST_MODE__GET_ITEM__RSP_FLAG__NONE \
	((G_Can_Monitor_ListMode_GetItem_RspFlags_t) 0x00)

#define G_CAN__MONITOR__LIST_MODE__GET_ITEM__RSP_FLAG__EXTENDED_ID \
	((G_Can_Monitor_ListMode_GetItem_RspFlags_t) 0x01)

#define G_CAN__MONITOR__LIST_MODE__GET_ITEM__RSP_FLAG__TRANSMIT \
	((G_Can_Monitor_ListMode_GetItem_RspFlags_t) 0x02)


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_DefineFilter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//mode
typedef u8_t G_Can_Monitor_DefineFilter_Mode_t;

#define G_CAN__MONITOR__DEFINE_FILTER__MODE__OFF \
	((G_Can_Monitor_DefineFilter_Mode_t) 0x00)

#define G_CAN__MONITOR__DEFINE_FILTER__MODE__RANGE \
	((G_Can_Monitor_DefineFilter_Mode_t) 0x01)

#define G_CAN__MONITOR__DEFINE_FILTER__MODE__ADD_RANGE \
	((G_Can_Monitor_DefineFilter_Mode_t) 0x02)

#define G_CAN__MONITOR__DEFINE_FILTER__MODE__REMOVE_RANGE \
	((G_Can_Monitor_DefineFilter_Mode_t) 0x03)

#define G_CAN__MONITOR__DEFINE_FILTER__MODE__REMOVE_ALL \
	((G_Can_Monitor_DefineFilter_Mode_t) 0x04)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_DefineFilter
// description: defines monitor filter
// parameters:  portHandle - porthandle
//              mode - filter mode
//              startId - id range start
//              endId - id range end
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_DefineFilter(
	const G_PortHandle_t portHandle,
	const G_Can_Monitor_DefineFilter_Mode_t mode,
	const u32_t startId,
	const u32_t endId
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//buffer mode
typedef u8_t G_Can_Monitor_BufferMode_Mode_t;

#define G_CAN__MONITOR__BUFFER_MODE__MODE__NOTHING \
	((G_Can_Monitor_BufferMode_Mode_t) 0x00)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__RX \
	((G_Can_Monitor_BufferMode_Mode_t) 0x01)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__TX \
	((G_Can_Monitor_BufferMode_Mode_t) 0x02)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__TX_AND_RX \
	((G_Can_Monitor_BufferMode_Mode_t) 0x03)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__ERROR_FRAME \
	((G_Can_Monitor_BufferMode_Mode_t) 0x04)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__ERROR_FRAME_AND_RX \
	((G_Can_Monitor_BufferMode_Mode_t) 0x05)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__ERROR_FRAME_AND_TX \
	((G_Can_Monitor_BufferMode_Mode_t) 0x06)

#define G_CAN__MONITOR__BUFFER_MODE__MODE__ERROR_FRAME_AND_TX_AND_RX \
	((G_Can_Monitor_BufferMode_Mode_t) 0x07)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_Start
// description: starts monitoring
// parameters:  portHandle - porthandle
//              mode - mode
//              bufferSize - size of api internal monitor buffer in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_Start(
	const G_PortHandle_t portHandle,
	const G_Can_Monitor_BufferMode_Mode_t mode,
	const u32_t bufferSize
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_Stop
// description: stops monitoring
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_Stop(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_GetItems
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//response flags
typedef u16_t G_Can_Monitor_BufferItem_Flags_t;

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__NONE \
	((G_Can_Monitor_BufferItem_Flags_t) 0x0000)

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__EXTENDED_ID \
	((G_Can_Monitor_BufferItem_Flags_t) 0x0001)

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__TRANSMIT \
	((G_Can_Monitor_BufferItem_Flags_t) 0x0002)

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__ERROR_FRAME \
	((G_Can_Monitor_BufferItem_Flags_t) 0x0004)

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__EVENT \
	((G_Can_Monitor_BufferItem_Flags_t) 0x0010)

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__BUFFER_OVERRUN \
	((G_Can_Monitor_BufferItem_Flags_t) 0x0080)

#define G_CAN__MONITOR__BUFFER_ITEM__FLAG__DLL_BUFFER_OVERRUN \
	((G_Can_Monitor_BufferItem_Flags_t) 0x8000)


//item
typedef struct {
	u64_t  Timestamp; //ns
	u32_t  Id;

	G_Can_Monitor_BufferItem_Flags_t  Flags;
	u8_t  Dlc;
	u8_t  reserved;

	u8_t  Data[8];
} G_Can_Monitor_BufferItem_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_GetItems
// description: gets buffer items
// parameters:  portHandle - porthandle
//              bufferSize - size of bufferItems
//              numberOfItems - items count
//              bufferItems - buffer items
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_GetItems(
	const G_PortHandle_t portHandle,
	const u32_t bufferSize,
	u32_t * const numberOfItems,
	G_Can_Monitor_BufferItem_t * const bufferItems
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_Async_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------

// callback
//lint -strong(,G_Can_Monitor_BufferMode_Async_Callback_t)
typedef void
(G_API * G_Can_Monitor_BufferMode_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u32_t  numberOfItems,
	const G_Can_Monitor_BufferItem_t * const bufferItems
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_Async_Start
// description: start async buffer monitor
// parameters:  portHandle - porthandle
//              mode - monitor mode
//              callback - callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_Async_Start(
	const G_PortHandle_t portHandle,
	const G_Can_Monitor_BufferMode_Mode_t mode,
	const G_Can_Monitor_BufferMode_Async_Callback_t callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_Async_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_Async_Stop
// description: start async buffer monitor
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_Async_Stop(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_EnableEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_EnableEvent
// description: enable monitor event notification
// parameters:  portHandle - porthandle
//              eventHandle - handle to event
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_EnableEvent(
	const G_PortHandle_t portHandle,
	const G_Common_EventHandle_t * const eventHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_BufferMode_EnableEvent
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_BufferMode_DisableEvent
// description: disable monitor event notification
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_BufferMode_DisableEvent(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_ListMode_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_ListMode_Start
// description: starts monitoring
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_ListMode_Start(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_ListMode_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_ListMode_Stop
// description: stops monitoring
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_ListMode_Stop(
	const G_PortHandle_t portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_ListMode_GetItem
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_ListMode_GetItem
// description: gets list item
// parameters:  portHandle - porthandle

//              id - message identifier
//              timestamp - timestamp (ns)
//              msgCount - message count
//              rspFlags - response flags
//              dlc - data length count
//              data - data bytes 0..7
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_ListMode_GetItem(
	const G_PortHandle_t portHandle,
	const u32_t id,
	u64_t * const timestamp,
	u32_t * const msgCount,
	G_Can_Monitor_ListMode_GetItem_RspFlags_t * const rspFlags,
	u8_t * const dlc,
	u8_t data[8]
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_ListMode_GetItem_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
// callback
//lint -strong(,G_Can_Monitor_ListMode_GetItem_Async_Callback_t)
typedef void
(G_API * G_Can_Monitor_ListMode_GetItem_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u32_t id,
	const u64_t  timestamp,
	const u32_t  msgCount,
	const G_Can_Monitor_ListMode_GetItem_RspFlags_t  rspFlags,
	const u8_t  dlc,
	const u8_t * const data
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_ListMode_GetItem_Async_AddCallback
// description: add callback for 'G_Can_Monitor_ListMode_GetItem_Async'
// parameters:  portHandle - porthandle
//              callback - callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_ListMode_GetItem_Async_AddCallback(
	const G_PortHandle_t  portHandle,
	const G_Can_Monitor_ListMode_GetItem_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_ListMode_GetItem_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_ListMode_GetItem_Async_RemoveCallback
// description: renmove callback for ' G_Can_Monitor_ListMode_GetItem_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_ListMode_GetItem_Async_RemoveCallback(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Monitor_ListMode_GetItem_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Monitor_ListMode_GetItem_Async
// description: gets list item (async)
// parameters:  portHandle - porthandle
//              id - message identifier
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Monitor_ListMode_GetItem_Async(
	const G_PortHandle_t  portHandle,
	const u32_t  id
);


//******************************************************************************
// TP
//******************************************************************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_GetChannel
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_GetChannel
// description: gets multisession channel
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_GetChannel(
	const G_PortHandle_t portHandle,
	u8_t * const channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_GetChannel_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
// callback
//lint -strong(,G_Can_Tp_GetChannel_Async_Callback_t)
typedef void
(G_API * G_Can_Tp_GetChannel_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_GetChannel_Async_AddCallback
// description: add callback for 'G_Can_Tp_GetChannel_Async'
// parameters:  portHandle - porthandle
//              callback - user callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_GetChannel_Async_AddCallback(
	const G_PortHandle_t  portHandle,
	const G_Can_Tp_GetChannel_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_GetChannel_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_GetChannel_Async_RemoveCallback
// description: renmove callback for 'G_Tp_GetChannel_Async'
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_GetChannel_Async_RemoveCallback(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_GetChannel_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_GetChannel_Async
// description: get multisession channel (async)
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_GetChannel_Async(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_FreeChannel
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_FreeChannel
// description: releases multisession channel
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_FreeChannel(
	const G_PortHandle_t portHandle,
	const u8_t channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//G_Can_Tp_Config_GmLan
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command flags
typedef u32_t G_Can_Tp_Config_GmLan_CmdFlags_t;

#define G_CAN__TP__CONFIG__GMLAN__CMD_FLAG__NONE \
	((G_Can_Tp_Config_GmLan_CmdFlags_t) 0x00)

#define G_CAN__TP__CONFIG__GMLAN__CMD_FLAG__USE_OWN_SEPARATION_TIME \
	((G_Can_Tp_Config_GmLan_CmdFlags_t) 0x01)

//addressing format
typedef u8_t G_Can_Tp_Config_GmLan_AddressingFormat_t;

#define G_CAN__TP__CONFIG__GMLAN__ADDRESSING_FORMAT__NORMAL \
	((G_Can_Tp_Config_GmLan_AddressingFormat_t) 0x00)

#define G_CAN__TP__CONFIG__GMLAN__ADDRESSING_FORMAT__EXTENDED \
	((G_Can_Tp_Config_GmLan_AddressingFormat_t) 0x01)

#define G_CAN__TP__CONFIG__GMLAN__ADDRESSING_FORMAT__MIXED \
	((G_Can_Tp_Config_GmLan_AddressingFormat_t) 0x02)


//command parameters
typedef struct {
	struct {
		u8_t  PhysicalSourceAddress;  // in the case of normal
		u8_t  PhysicalTargetAddress;  // in the case of normal
		u8_t  FunctionalSourceAddress;  // placeholder, needed
		u8_t  FunctionalTargetAddress;  // in the case of normal


		u32_t  PhysicalRequestId;

		u32_t  PhysicalResponseId;

		u32_t  AllNodeFunctionalRequestId;

		G_Can_Tp_Config_GmLan_AddressingFormat_t  PhysicalAddressingFormat;
		G_Can_Tp_Config_GmLan_AddressingFormat_t  FunctionalAddressingFormat;
		u8_t  BlockSize;  // 0..255
		u8_t  SeparationTime;  // time in ms (sent to target)

		G_Can_Tp_Config_GmLan_CmdFlags_t Flags;

		u8_t  OwnSeparationTime;  // time in milliseconds
		u8_t  reserved1;
		u8_t  reserved2;
		u8_t  reserved3;

		u16_t  TimeoutAs;  // tx timeout sender in ms
		u16_t  TimeoutAr;  // tx timeout receiver in ms
		u16_t  TimeoutBs;  // FC rx timeout sender in ms
		u16_t  TimeoutCr;  // CF rx timeout receiver in ms
	} USDT;

	struct {
		u32_t  ResponseId;  // UUDT response identifier
	} UUDT;
} G_Can_Tp_Config_GmLan_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Config_GmLan
// description: sets configuration for GM LAN
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              gmLanParameters - parameters for GM Lan
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Config_GmLan(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_Config_GmLan_Parameters_t * const gmLanParameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Config_IsoTp
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command flags
typedef u32_t G_Can_Tp_Config_IsoTp_CmdFlags_t;

#define G_CAN__TP__CONFIG__ISOTP__CMD_FLAG__NONE \
	((G_Can_Tp_Config_IsoTp_CmdFlags_t) 0x00)

#define G_CAN__TP__CONFIG__ISOTP__CMD_FLAG__USE_OWN_SEPARATION_TIME \
	((G_Can_Tp_Config_IsoTp_CmdFlags_t) 0x01)

#define G_CAN__TP__CONFIG__ISOTP__CMD_FLAG__START_SN_WITH_ZERO  \
	((G_Can_Tp_Config_IsoTp_CmdFlags_t) 0x02)

#define G_CAN__TP__CONFIG__ISOTP__CMD_FLAG__SEPARATION_TIME_BETWEEN_FC_AND_CF \
	((G_Can_Tp_Config_IsoTp_CmdFlags_t) 0x04)

#define G_CAN__TP__CONFIG__ISOTP__CMD_FLAG__SET_DLC_TO_NUMBER_OF_USED_BYTES  \
	((G_Can_Tp_Config_IsoTp_CmdFlags_t) 0x08)

//addressing format
typedef u8_t G_Can_Tp_Config_IsoTp_AddressingFormat_t;

#define G_CAN__TP__CONFIG__ISOTP__ADDRESSING_FORMAT__NORMAL \
	((G_Can_Tp_Config_IsoTp_AddressingFormat_t) 0x00)

#define G_CAN__TP__CONFIG__ISOTP__ADDRESSING_FORMAT__EXTENDED \
	((G_Can_Tp_Config_IsoTp_AddressingFormat_t) 0x01)

#define G_CAN__TP__CONFIG__ISOTP__ADDRESSING_FORMAT__MIXED \
	((G_Can_Tp_Config_IsoTp_AddressingFormat_t) 0x02)

//command parameters
typedef struct {
	u8_t  PhysicalSourceAddress;  // in the case of normal
	u8_t  PhysicalTargetAddress;  // in the case of normal
	u8_t  FunctionalSourceAddress;  // placeholder, needed for
	u8_t  FunctionalTargetAddress;  // in the case of normal
	// addressing not needed

	u32_t  PhysicalSourceId;

	u32_t  PhysicalTargetId;

	u32_t  FunctionalSourceId;

	u32_t  FunctionalTargetId;  // placeholder, needed for ECU

	G_Can_Tp_Config_IsoTp_AddressingFormat_t  PhysicalAddressingFormat;
	G_Can_Tp_Config_IsoTp_AddressingFormat_t  FunctionalAddressingFormat;
	u8_t  BlockSize;  // 0..255
	u8_t  SeparationTime;  // time in ms (sent to target)

	G_Can_Tp_Config_IsoTp_CmdFlags_t Flags;

	u8_t  OwnSeparationTime;  // time in milliseconds
	u8_t  reserved1;
	u8_t  reserved2;
	u8_t  reserved3;

	u16_t  TimeoutAs;  // tx timeout sender in ms
	u16_t  TimeoutAr;  // tx timeout receiver in ms
	u16_t  TimeoutBs;  // FC rx timeout sender in ms
	u16_t  TimeoutCr;  // CF rx timeout receiver in ms
} G_Can_Tp_Config_IsoTp_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Config_IsoTp
// description: sets configuration for ISO TP
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              isoTpParameters - parameters for ISO TP
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Config_IsoTp(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_Config_IsoTp_Parameters_t * const isoTpParameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Config_J1939
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u8_t  SourceAddress;
	u8_t  DestinationAddress;
	u8_t  reserved1;
	u8_t  reserved2;

	u32_t  TxTimeout;  // in microseconds
	u32_t  RxTimeout;  // in microseconds
	u32_t  DelayTime;  // in microseconds
	u32_t  TimeoutTr;  // in microseconds
	u32_t  TimeoutTh;  // in microseconds
	u32_t  TimeoutT1;  // in microseconds
	u32_t  TimeoutT2;  // in microseconds
	u32_t  TimeoutT3;  // in microseconds
	u32_t  TimeoutT4;  // in microseconds
} G_Can_Tp_Config_J1939_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Config_J1939
// description: sets configuration for J1939
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              j1939Parameters - parameters for J1939
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Config_J1939(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_Config_J1939_Parameters_t * const j1939Parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Config_Tp_1_6
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u8_t  SourceAddress;
	u8_t  TargetAddress;
	u8_t  BlockSize;  // 0..15
	u8_t  reserved;

	u32_t  SourceSetupId;
	u32_t  TargetSetupId;
	u32_t  SourceChannelId;
	u32_t  TargetChannelId;

	u16_t  TimeoutT1;  // acknowledge timeout in ms
	u16_t  TimeoutT2;  // max time between 2 blocks in ms
	u16_t  TimeoutT3;  // min time between 2 frames in ms
	u16_t  TimeoutT4;  // no traffic timeout in ms
} G_Can_Tp_Config_Tp_1_6_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Config_Tp_1_6
// description: sets configuration for TP 1_6
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              tp_1_6Parameters - parameters for TP 1_6
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Config_Tp_1_6(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_Config_Tp_1_6_Parameters_t * const tp_1_6Parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Config_Tp_2_0
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//application type
typedef u8_t G_Can_Tp_Config_Tp_2_0_ApplicationType_t;

#define G_CAN__TP__CONFIG__TP_2_0__APPLICATION_TYPE__DIAG \
	((G_Can_Tp_Config_Tp_2_0_ApplicationType_t) 0x01)

#define G_CAN__TP__CONFIG__TP_2_0__APPLICATION_TYPE__INFO \
	((G_Can_Tp_Config_Tp_2_0_ApplicationType_t) 0x10)

#define G_CAN__TP__CONFIG__TP_2_0__APPLICATION_TYPE__APP \
	((G_Can_Tp_Config_Tp_2_0_ApplicationType_t) 0x20)

#define G_CAN__TP__CONFIG__TP_2_0__APPLICATION_TYPE__WFS_WIV \
	((G_Can_Tp_Config_Tp_2_0_ApplicationType_t) 0x21)

//command parameters
typedef struct {
	u8_t  SourceAddress;
	u8_t  TargetAddress;
	u8_t  BlockSize;  // 0..15
	G_Can_Tp_Config_Tp_2_0_ApplicationType_t  ApplicationType;

	u32_t  SourceSetupId;  // 0xFFFFFFFF for static channels
	u32_t  TargetSetupId;  // 0xFFFFFFFF for static channels
	u32_t  SourceChannelId;
	u32_t  TargetChannelId;

	u16_t  TimeoutT1;  // time in milliseconds, acknowledge timeout
	u16_t  TimeoutT3;  // time in milliseconds, min time between two frames
} G_Can_Tp_Config_Tp_2_0_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Config_Tp_2_0
// description: sets configuration for TP 2_0
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              tp_2_0Parameters - parameters for TP 2_0
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Config_Tp_2_0(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_Config_Tp_2_0_Parameters_t * const tp_2_0Parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Config_Off
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Config_Off
// description: sets configuration off
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Config_Off(
	const G_PortHandle_t portHandle,
	const u8_t channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_BroadCast_StartTransmission
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//transmission mode
typedef u8_t G_Can_Tp_BroadCast_TransmissionMode_t;

#define G_CAN__TP__BROADCAST__TRANSMISSION_MODE__REQUEST \
	((G_Can_Tp_BroadCast_TransmissionMode_t) 0x00)

#define G_CAN__TP__BROADCAST__TRANSMISSION_MODE__REQUEST_WITH_RETRIGGER \
	((G_Can_Tp_BroadCast_TransmissionMode_t) 0x01)

#define G_CAN__TP__BROADCAST__TRANSMISSION_MODE__RESPONSE \
	((G_Can_Tp_BroadCast_TransmissionMode_t) 0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_BroadCast_StartTransmission
// description: starts broadcast
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - data mode (request,... with retrigger, response)
//              data - data bytes
//              dataLength - data length in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Broadcast_StartTransmission(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_BroadCast_TransmissionMode_t mode,
	const u8_t * const data,
	const u32_t dataLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_BroadCast_StopTransmission
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_BroadCast_StopTransmission
// description: stops broadcast
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL G_Error_t G_API
G_Can_Tp_Broadcast_StopTransmission(
	const G_PortHandle_t portHandle,
	const u8_t channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_BroadCast_GetData
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_BroadCast_GetData
// description: gets broadcast data
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              responseData - response data bytes
//              responseLength - length of response in bytes
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Broadcast_GetData(
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	u8_t * const responseData,
	u32_t * const responseLength
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Broadcast_GetData_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
// callback
//lint -strong(,G_Can_Tp_Broadcast_GetData_Async_Callback_t)
typedef void
(G_API * G_Can_Tp_Broadcast_GetData_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const u8_t * const responseData,
	const u32_t  responseLength
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Broadcast_GetData_Async_AddCallback
// description: add callback for 'G_Tp_Broadcast_GetData_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              callback - user callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Broadcast_GetData_Async_AddCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const G_Can_Tp_Broadcast_GetData_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Broadcast_GetData_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Broadcast_GetData_Async_RemoveCallback
// description: renmove callback for 'G_Tp_Broadcast_GetData_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Broadcast_GetData_Async_RemoveCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_Broadcast_GetData_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_Broadcast_GetData_Async
// description: get broadcast data (async)
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_Broadcast_GetData_Async(
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tp_SetMonitorFilter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//monitor filter mode
typedef u8_t G_Can_Tp_SetMonitorFilter_Mode_t;

#define G_CAN__TP__SET_MONITOR_FILTER__MODE__DISABLE \
	((G_Can_Tp_SetMonitorFilter_Mode_t) 0x00)

#define G_CAN__TP__SET_MONITOR_FILTER__MODE__ENABLE \
	((G_Can_Tp_SetMonitorFilter_Mode_t) 0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tp_SetMonitorFilter
// description: sets monitor filter for multisession channel
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - filter mode enable/disable
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tp_SetMonitorFilter(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Tp_SetMonitorFilter_Mode_t mode
);


//******************************************************************************
// DIAG
//******************************************************************************

//------------------------------------------------------------------------------
// common parameters
//------------------------------------------------------------------------------
///diagnostic type
typedef u8_t G_Can_Diag_Type_t;

#define G_CAN__DIAG__TYPE__OFF \
	((G_Can_Diag_Type_t) 0x00)

#define G_CAN__DIAG__TYPE__KW2000_TP_1_6 \
	((G_Can_Diag_Type_t) 0x01)

#define G_CAN__DIAG__TYPE__KW2000_TP_2_0 \
	((G_Can_Diag_Type_t) 0x02)

#define G_CAN__DIAG__TYPE__KW2000_ISOTP \
	((G_Can_Diag_Type_t) 0x03)

#define G_CAN__DIAG__TYPE__GMLAN \
	((G_Can_Diag_Type_t) 0x04)

#define G_CAN__DIAG__TYPE__UDS_ISOTP \
	((G_Can_Diag_Type_t) 0x05)

#define G_CAN__DIAG__TYPE__J1939 \
	((G_Can_Diag_Type_t) 0x06)


//state
typedef u8_t G_Can_Diag_State_t;

#define G_CAN__DIAG__STATE__NOT_INITIALIZED \
	((G_Can_Diag_State_t) 0x00)

#define G_CAN__DIAG__STATE__DISCONNECTED \
	((G_Can_Diag_State_t) 0x01)

#define G_CAN__DIAG__STATE__CONNECT_IN_PROGRESS \
	((G_Can_Diag_State_t) 0x02)

#define G_CAN__DIAG__STATE__CONNECTED \
	((G_Can_Diag_State_t) 0x03)

#define G_CAN__DIAG__STATE__DISCONNECT_IN_PROGRESS \
	((G_Can_Diag_State_t) 0x04)


//mode
typedef u8_t G_Can_Diag_Config_Mode_t;

#define G_CAN__DIAG__CONFIG__MODE__SET_DEFAULT_PARAMS \
	((G_Can_Diag_Config_Mode_t) 0x00)

#define G_CAN__DIAG__CONFIG__MODE__SET_PARAMS \
	((G_Can_Diag_Config_Mode_t) 0x01)

#define G_CAN__DIAG__CONFIG__MODE__SET_FLAGS_ONLY_WITHOUT_INIT \
	((G_Can_Diag_Config_Mode_t) 0x02)

#define G_CAN__DIAG__CONFIG__MODE__SET_PARAMS_WITHOUT_INIT \
	((G_Can_Diag_Config_Mode_t) 0x03)


//command flags
typedef u32_t G_Can_Diag_Config_CmdFlags_t;

#define G_CAN__DIAG__CONFIG__CMD_FLAG__NONE \
	((G_Can_Diag_Config_CmdFlags_t) 0x00)

#define G_CAN__DIAG__CONFIG__CMD_FLAG__DISABLE_21_HANDLING \
	((G_Can_Diag_Config_CmdFlags_t) 0x01)

#define G_CAN__DIAG__CONFIG__CMD_FLAG__DISABLE_23_HANDLING \
	((G_Can_Diag_Config_CmdFlags_t) 0x02)

#define G_CAN__DIAG__CONFIG__CMD_FLAG__DISABLE_78_HANDLING \
	((G_Can_Diag_Config_CmdFlags_t) 0x04)

#define G_CAN__DIAG__CONFIG__CMD_FLAG__ALL_RESPONSES_AS_SYNC \
	((G_Can_Diag_Config_CmdFlags_t) 0x08)


//tester present mode
typedef u8_t G_Can_Diag_TesterPresent_Mode_t;

#define G_CAN__DIAG__TESTER_PRESENT__MODE__OFF \
	((G_Can_Diag_TesterPresent_Mode_t) 0x00)

#define G_CAN__DIAG__TESTER_PRESENT__MODE__PHYSICAL \
	((G_Can_Diag_TesterPresent_Mode_t) 0x01)

#define G_CAN__DIAG__TESTER_PRESENT__MODE__FUNCTIONAL \
	((G_Can_Diag_TesterPresent_Mode_t) 0x02)


//tester present response mode
typedef u8_t G_Can_Diag_TesterPresent_RspMode_t;

#define G_CAN__DIAG__TESTER_PRESENT__RSP_MODE__NO_RSP_REQUIRED \
	((G_Can_Diag_TesterPresent_RspMode_t) 0x00)

#define G_CAN__DIAG__TESTER_PRESENT__RSP_MODE__RSP_REQUIRED \
	((G_Can_Diag_TesterPresent_RspMode_t) 0x01)


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_GetState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command flags
typedef u32_t G_Can_Diag_GetState_CmdFlags_t;

#define G_CAN__DIAG__GET_STATE__CMD_FLAG__NONE \
	((G_Can_Diag_GetState_CmdFlags_t) 0x00)

#define G_CAN__DIAG__GET_STATE__CMD_FLAG__RESET_LAST_ERROR \
	((G_Can_Diag_GetState_CmdFlags_t) 0x01)


//response flags
typedef u32_t G_Can_Diag_GetState_RspFlags_t;

#define G_CAN__DIAG__GET_STATE__RSP_FLAG__NONE \
	((G_Can_Diag_GetState_RspFlags_t) 0x00)

#define G_CAN__DIAG__GET_STATE__RSP_FLAG__BUSY \
	((G_Can_Diag_GetState_RspFlags_t) 0x01)

#define G_CAN__DIAG__GET_STATE__RSP_FLAG__SYNC_RX_BUFFER_NOT_EMPTY \
	((G_Can_Diag_GetState_RspFlags_t) 0x02)

#define G_CAN__DIAG__GET_STATE__RSP_FLAG__ASYNC_RX_BUFFER_NOT_EMPTY \
	((G_Can_Diag_GetState_RspFlags_t) 0x04)

#define G_CAN__DIAG__GET_STATE__RSP_FLAG__UUDT_RX_BUFFER_NOT_EMPTY \
	((G_Can_Diag_GetState_RspFlags_t) 0x08)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_GetState
// description: gets diagnostic state
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              cmdFlags - flags
//              lastErrorCode - last error code
//              type - diagnostic type
//              state - diagnostic state
//              rspFlags - flags
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_GetState(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_GetState_CmdFlags_t cmdFlags,
	G_Error_t * const lastErrorCode,
	G_Can_Diag_Type_t * const type,
	G_Can_Diag_State_t * const state,
	G_Can_Diag_GetState_RspFlags_t * const rspFlags
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_GetState_Async_AddCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
// callback
//lint -strong(,G_Can_Diag_GetState_Async_Callback_t)
typedef void
(G_API * G_Can_Diag_GetState_Async_Callback_t) (
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	const G_Error_t  lastErrorCode,
	const G_Can_Diag_Type_t  type,
	const G_Can_Diag_State_t  state,
	const G_Can_Diag_GetState_RspFlags_t  rspFlags
);


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_GetState_Async_AddCallback
// description: add callback for 'G_Can_Diag_GetState_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              callback - user callback function
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_GetState_Async_AddCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const G_Can_Diag_GetState_Async_Callback_t  callback
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_GetState_Async_RemoveCallback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_GetState_Async_RemoveCallback
// description: renmove callback for 'G_Can_Diag_GetState_Async'
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_GetState_Async_RemoveCallback(
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_GetState_Async
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_GetState_Async
// description: gets diagnostic state (async)
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              cmdFlags - flags
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_GetState_Async(
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const G_Can_Diag_GetState_CmdFlags_t  cmdFlags
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_GmLan
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u16_t P2max;  // normal response timeout in ms
	u16_t P3max;  // extended response timeout in ms

	u16_t Repetitions;
	u8_t reserved1;
	u8_t reserved2;

	struct {
		G_Can_Diag_TesterPresent_Mode_t  Mode;
		G_Can_Diag_TesterPresent_RspMode_t  ResponseMode;
		u16_t  CycleTime;  // time in milliseconds

		u8_t  reserved1;
		u8_t  reserved2;
		u8_t  reserved3;
		u8_t  DataLength;

		u8_t  Data[8];
	} TesterPresent;
} G_Can_Diag_Config_GmLan_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_GmLan
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - mode
//              globalTimeout - global timeout (ms)
//              cmdFlags - flags
//              gmLanParameters - parameters for GM LAN diagnosis
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_Config_GmLan(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_Config_Mode_t mode,
	const u32_t globalTimeout,
	const G_Can_Diag_Config_CmdFlags_t cmdFlags,
	const G_Can_Diag_Config_GmLan_Parameters_t * const gmLanParameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_J1939
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u32_t  T1Timeout;  // in microseconds
	u32_t  T2Timeout;  // time to wait after "busy"-rsp in microseconds

	u16_t Repetitions;
	u8_t reserved1;
	u8_t reserved2;

	struct {
		G_Can_Diag_TesterPresent_Mode_t  Mode;
		G_Can_Diag_TesterPresent_RspMode_t  ResponseMode;
		u8_t  DataLength;
		u8_t  reserved1;

		u32_t  CycleTime;  // time in microseconds

		u8_t  Data[11];
	} TesterPresent;
} G_Can_Diag_Config_J1939_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_J1939
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - mode
//              globalTimeout - global timeout (ms)
//              cmdFlags - flags
//              j1939Parameters - parameters for GM LAN diagnosis
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_Config_J1939(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_Config_Mode_t mode,
	const u32_t globalTimeout,
	const G_Can_Diag_Config_CmdFlags_t cmdFlags,
	const G_Can_Diag_Config_J1939_Parameters_t * const j1939Parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_Kw2000_IsoTp
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u16_t P2max;  // normal response timeout in ms
	u16_t P3max;  // extended response timeout in ms

	u16_t Repetitions;
	u8_t reserved1;
	u8_t reserved2;

	struct {
		G_Can_Diag_TesterPresent_Mode_t  Mode;
		G_Can_Diag_TesterPresent_RspMode_t  ResponseMode;
		u16_t  CycleTime;  // time in milliseconds

		u8_t  reserved1;
		u8_t  reserved2;
		u8_t  reserved3;
		u8_t  DataLength;

		u8_t  Data[8];
	} TesterPresent;
} G_Can_Diag_Config_Kw2000_IsoTp_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_Kw2000_IsoTp
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - mode
//              globalTimeout - global timeout (ms)
//              cmdFlags - flags
//              kw2000_IsoTpParameters - parameters for KW2000_IsoTp diagnosis
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_Config_Kw2000_IsoTp(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_Config_Mode_t mode,
	const u32_t globalTimeout,
	const G_Can_Diag_Config_CmdFlags_t cmdFlags,
	const G_Can_Diag_Config_Kw2000_IsoTp_Parameters_t \
		* const kw2000_IsoTpParameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_Kw2000_Tp_1_6
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u16_t P2max;  // normal response timeout in ms
	u16_t Repetitions;

	u8_t AddressWord;
	u8_t TargetAddress;
	u8_t SourceAddress;
	u8_t reserved1;

	u16_t TesterPresentCycle;  // time in milliseconds
	u8_t reserved2;
	u8_t reserved3;
} G_Can_Diag_Config_Kw2000_Tp_1_6_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_Kw2000_Tp_1_6
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - mode
//              globalTimeout - global timeout (ms)
//              cmdFlags - flags
//              kw2000_Tp_1_6Parameters - parameters for KW2000_TP_1_6 diagnosis
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL G_Error_t G_API
G_Can_Diag_Config_Kw2000_Tp_1_6(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_Config_Mode_t mode,
	const u32_t globalTimeout,
	const G_Can_Diag_Config_CmdFlags_t cmdFlags,
	const G_Can_Diag_Config_Kw2000_Tp_1_6_Parameters_t \
		* const kw2000_Tp_1_6Parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_Kw2000_Tp_2_0
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_Kw2000_Tp_2_0
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - mode
//              globalTimeout - global timeout (ms)
//              cmdFlags - flags
//              p2max - p2 max
//              repetitions - repetitions
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_Config_Kw2000_Tp_2_0(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_Config_Mode_t mode,
	const u32_t globalTimeout,
	const G_Can_Diag_Config_CmdFlags_t cmdFlags,
	const u16_t p2max,
	const u16_t repetitions
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_Uds_IsoTp
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
//command parameters
typedef struct {
	u16_t P2max;  // normal response timeout in ms
	u16_t P3max;  // extended response timeout in ms

	u16_t Repetitions;
	u8_t reserved1;
	u8_t reserved2;

	struct {
		G_Can_Diag_TesterPresent_Mode_t  Mode;
		G_Can_Diag_TesterPresent_RspMode_t  ResponseMode;
		u16_t  CycleTime;  // time in milliseconds

		u8_t  reserved1;
		u8_t  reserved2;
		u8_t  reserved3;
		u8_t  DataLength;

		u8_t  Data[8];
	} TesterPresent;
} G_Can_Diag_Config_Uds_IsoTp_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_Uds_IsoTp
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - mode
//              globalTimeout - global timeout (ms)
//              cmdFlags - flags
//              uds_IsoTpParameters - parameters for Uds_IsoTp diagnosis
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_Config_Uds_IsoTp(
	const G_PortHandle_t portHandle,
	const u8_t channel,
	const G_Can_Diag_Config_Mode_t mode,
	const u32_t globalTimeout,
	const G_Can_Diag_Config_CmdFlags_t cmdFlags,
	const G_Can_Diag_Config_Uds_IsoTp_Parameters_t \
		* const uds_IsoTpParameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Diag_Config_Off
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Diag_Config_Off
// description: sets diagnostic configuration
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Diag_Config_Off(
	const G_PortHandle_t portHandle,
	const u8_t channel
);


//******************************************************************************
// NM
//******************************************************************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Config_Off
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Config_Off
// description: disable network management
// parameters:  portHandle - porthandle
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Config_Off(
	const G_PortHandle_t  portHandle
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Config_Osek
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Nm_Config_Osek_Type_t;

#define G_CAN__NM__CONFIG__OSEK__TYPE__VAG  \
	((G_Can_Nm_Config_Osek_Type_t) 0x00)

#define G_CAN__NM__CONFIG__OSEK__TYPE__DAIMLER  \
	((G_Can_Nm_Config_Osek_Type_t) 0x01)

#define G_CAN__NM__CONFIG__OSEK__TYPE__PORSCHE  \
	((G_Can_Nm_Config_Osek_Type_t) 0x02)

#define G_CAN__NM__CONFIG__OSEK__TYPE__BMW  \
	((G_Can_Nm_Config_Osek_Type_t) 0x03)

#define G_CAN__NM__CONFIG__OSEK__TYPE__FORD  \
	((G_Can_Nm_Config_Osek_Type_t) 0x04)


typedef u32_t  G_Can_Nm_Config_Osek_CmdFlags_t;

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__NONE  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x00)

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__SET_DLC  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x01)

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__SET_DESTINATION_BYTE_OFFSET  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x02)

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__SET_RANGE  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x04)

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__SET_PROTOCOL_BITS  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x08)

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__SET_RING_STABLE_BIT  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x10)

#define G_CAN__NM__CONFIG__OSEK__CMD_FLAG__SET_TIMES  \
	((G_Can_Nm_Config_Osek_CmdFlags_t) 0x20)


typedef struct {
	G_Can_Nm_Config_Osek_CmdFlags_t  CmdFlags;

	G_Can_Nm_Config_Osek_Type_t  Type;
	u8_t  Dlc;
	u8_t  DestinationByteOffset;
	u8_t  reserved;

	struct {
		u32_t  IdBase;
		u32_t  IdMask;
		u16_t  NumberOfNodes;
		u8_t  reserved1;
		u8_t  reserved2;
	} Range;

	struct {
		u32_t  MessageTypeMask;
		u32_t  AliveCode;
		u32_t  RingCode;
		u32_t  LimpHomeCode;

		u32_t  SleepIndMask;
		u32_t  SleepIndCodeTrue;
		u32_t  SleepIndCodeFalse;

		u32_t  SleepAckMask;
		u32_t  SleepAckCodeTrue;
		u32_t  SleepAckCodeFalse;
	} ProtocolBits;

	struct {
		u32_t  RingStableMask;
		u32_t  RingStableCodeTrue;
		u32_t  RingStableCodeFalse;
	} RingStableBit;

	struct {
		u16_t  Typ;  // in milliseconds
		u16_t  Max;  // in milliseconds
		u16_t  Error;  // in milliseconds
		u16_t  WaitBusSleep;  // in milliseconds
	} Times;
} G_Can_Nm_Config_Osek_Parameters_t;


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Config_Osek
// description: configuration for Osek-type NM
// parameters:  portHandle - porthandle
//              parameters - parameters
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Config_Osek(
	const G_PortHandle_t  portHandle,
	const G_Can_Nm_Config_Osek_Parameters_t * const parameters
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Config_SetData
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Config_SetData
// description: set data of all NM nodes
// parameters:  portHandle - porthandle
//              mask - mask bytes 0..7
//              data - data bytes 0..7
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Config_SetData(
	const G_PortHandle_t  portHandle,
	const u8_t  mask[8],
	const u8_t  data[8]
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Nodes_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Nodes_Start
// description: start Nm nodes
// parameters:  portHandle - porthandle
//              numberOfNodes - number of nodes to start
//              nodes - pointer to array with node numbers
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Nodes_Start(
	const G_PortHandle_t  portHandle,
	const u8_t  numberOfNodes,
	const u8_t * const nodes
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Nodes_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Nodes_Stop
// description: stop Nm nodes
// parameters:  portHandle - porthandle
//              numberOfNodes - number of nodes to stop
//              nodes - pointer to array with node numbers
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Nodes_Stop(
	const G_PortHandle_t  portHandle,
	const u8_t  numberOfNodes,
	const u8_t * const nodes
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Group_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Group_Start
// description: start group of Nm nodes
// parameters:  portHandle - porthandle
//              group - group number
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Group_Start(
	const G_PortHandle_t  portHandle,
	const u8_t  group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Group_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Group_Stop
// description: stop group of Nm nodes
// parameters:  portHandle - porthandle
//              group - group number
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Group_Stop(
	const G_PortHandle_t  portHandle,
	const u8_t  group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Nodes_GoToMode_Awake
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Nodes_GoToMode_Awake
// description: trigger nodes to go to awake mode
// parameters:  portHandle - porthandle
//              numberOfNodes - number of nodes
//              nodes - pointer to array with node numbers
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Nodes_GoToMode_Awake(
	const G_PortHandle_t  portHandle,
	const u8_t  numberOfNodes,
	const u8_t * const nodes
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Nodes_GoToMode_BusSleep
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Nodes_GoToMode_BusSleep
// description: trigger nodes to go to bus sleep mode
// parameters:  portHandle - porthandle
//              numberOfNodes - number of nodes
//              nodes - pointer to array with node numbers
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Nodes_GoToMode_BusSleep(
	const G_PortHandle_t  portHandle,
	const u8_t  numberOfNodes,
	const u8_t * const nodes
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Group_GoToMode_Awake
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Group_GoToMode_Awake
// description: trigger group of nodes to go to awake mode
// parameters:  portHandle - porthandle
//              group - group number
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Group_GoToMode_Awake(
	const G_PortHandle_t  portHandle,
	const u8_t  group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Group_GoToMode_BusSleep
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Group_GoToMode_BusSleep
// description: trigger group of nodes to go to bus sleep mode
// parameters:  portHandle - porthandle
//              group - group number
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Group_GoToMode_BusSleep(
	const G_PortHandle_t  portHandle,
	const u8_t  group
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Nm_Nodes_ChangeGroup
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Nm_Nodes_ChangeGroup
// description: change group number of NM nodes
// parameters:  portHandle - porthandle
//              group - group number
//              numberOfNodes - number of nodes
//              nodes - pointer to array with node numbers
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Nm_Nodes_ChangeGroup(
	const G_PortHandle_t  portHandle,
	const u8_t  group,
	const u8_t  numberOfNodes,
	const u8_t * const nodes
);


//******************************************************************************
// TAR
//******************************************************************************

//------------------------------------------------------------------------------
// common parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Tar_TxItem_Flags_t;

#define G_CAN__TAR__TX_ITEM__FLAG__NONE  \
	((G_Can_Tar_TxItem_Flags_t) 0x00)

#define G_CAN__TAR__TX_ITEM__FLAG__SEND_NEXT  \
	((G_Can_Tar_TxItem_Flags_t) 0x01)

#define G_CAN__TAR__TX_ITEM__FLAG__RESTART  \
	((G_Can_Tar_TxItem_Flags_t) 0x02)

#define G_CAN__TAR__TX_ITEM__FLAG__BREAK  \
	((G_Can_Tar_TxItem_Flags_t) 0x04)


typedef struct {
	u16_t  Delay;  // in ms
	G_Can_Tar_TxItem_Flags_t  Flags;
	u8_t  Dlc;

	u8_t  Data[8];
} G_Can_Tar_TxItem_Simple_t;


typedef struct {
	u32_t  Id;

	u16_t  Delay;  // in ms
	G_Can_Tar_TxItem_Flags_t  Flags;
	u8_t  Dlc;

	u8_t  Data[8];
} G_Can_Tar_TxItem_Standard_t;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_Config_Simple
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_Config_Simple
// description: set simple TAR properties
// parameters:  portHandle - porthandle
//              rxId - receiving identifier (0..0x7FF or 0..0x1FFFFFFF)
//              txId - sending identifier (0..0x7FF or 0..0x1FFFFFFF)
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_Config_Simple(
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	const u32_t  rxId,
	const u32_t  txId
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_Config_Standard
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_Config_Standard
// description: set standard TAR properties
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              rxId - receiving identifier (0..0x7FF or 0..0x1FFFFFFF)
//              rxIdMask - receiving identifier mask (e.g. 0xFFFFFFFF)
//              rxDlc - receiving data length (0..8)
//              rxDlcMask - receiving data length mask (0x00..0xFF)
//              rxData - receiving data bytes 0..7
//              rxDataMask - receiving data mask bytes 0..7
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_Config_Standard(
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	const u32_t  rxId,
	const u32_t  rxIdMask,
	const u8_t  rxDlc,
	const u8_t  rxDlcMask,
	const u8_t  rxData[8],
	const u8_t  rxDataMask[8]
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_Config_Off
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_Config_Off
// description: reset TAR
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_Config_Off(
	const G_PortHandle_t  portHandle,
	const u8_t channel
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_AddTxItems_Simple
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_AddTxItems_Simple
// description: add TAR tranmsit items
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              numberOfItems - number of TAR transmit items
//              items - pointer to transmit item array
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_AddTxItems_Simple(
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	const u16_t  numberOfItems,
	const G_Can_Tar_TxItem_Simple_t * const items
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_AddTxItems_Standard
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_AddTxItems_Standard
// description: add TAR tranmsit items
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              numberOfItems - number of TAR transmit items
//              items - pointer to transmit item array
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_AddTxItems_Standard(
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	const u16_t  numberOfItems,
	const G_Can_Tar_TxItem_Standard_t * const items
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_GetState
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Tar_GetState_Type_t;

#define G_CAN__TAR__GET_STATE__TYPE__UNKNOWN  \
	((G_Can_Tar_GetState_Type_t) 0x00)

#define G_CAN__TAR__GET_STATE__TYPE__OFF  \
	((G_Can_Tar_GetState_Type_t) 0x01)

#define G_CAN__TAR__GET_STATE__TYPE__SIMPLE  \
	((G_Can_Tar_GetState_Type_t) 0x02)

#define G_CAN__TAR__GET_STATE__TYPE__STANDARD  \
	((G_Can_Tar_GetState_Type_t) 0x03)


typedef u32_t  G_Can_Tar_GetState_RspFlags_t;

#define G_CAN__TAR__GET_STATE__RSP_FLAG__NONE  \
	((G_Can_Tar_GetState_RspFlags_t) 0x00)

#define G_CAN__TAR__GET_STATE__RSP_FLAG__STARTED  \
	((G_Can_Tar_GetState_RspFlags_t) 0x01)

#define G_CAN__TAR__GET_STATE__RSP_FLAG__BUSY  \
	((G_Can_Tar_GetState_RspFlags_t) 0x02)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_GetState
// description: add TAR tranmsit items
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              type - TAR type
//              rspFlags - response flags
//              numberOfTxItems - total number of TAR transmit entries
//              currentTxItem - number of the TAR transmit entry executed next
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_GetState(
	const G_PortHandle_t  portHandle,
	const u8_t channel,
	G_Can_Tar_GetState_Type_t * const type,
	G_Can_Tar_GetState_RspFlags_t * const rspFlags,
	u32_t * const numberOfTxItems,
	u32_t * const currentTxItem
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_Start
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// parameters
//------------------------------------------------------------------------------
typedef u8_t  G_Can_Tar_Start_Mode_t;

#define G_CAN__TAR__START__MODE__RESTART  \
	((G_Can_Tar_Start_Mode_t) 0x00)

#define G_CAN__TAR__START__MODE__RESUME  \
	((G_Can_Tar_Start_Mode_t) 0x01)


//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_Start
// description: start TAR
// parameters:  portHandle - porthandle
//              channel - multisession channel
//              mode - TAR start mode
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_Start(
	const G_PortHandle_t  portHandle,
	const u8_t  channel,
	const G_Can_Tar_Start_Mode_t  mode
);


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// G_Can_Tar_Stop
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// prototype
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function:    G_Can_Tar_Stop
// description: stop TAR
// parameters:  portHandle - porthandle
//              channel - multisession channel
// returns:     G_NO_ERROR on success, G_ERROR__DLL__CAN__... on error
//------------------------------------------------------------------------------
G_API_CAN_DLL
G_Error_t
G_API
G_Can_Tar_Stop(
	const G_PortHandle_t  portHandle,
	const u8_t  channel
);


#ifdef __cplusplus
}
#endif


#endif