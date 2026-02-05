//****************************************************************************************
//   File name: KeyGenResultExInterface.h
// Description: Abstract base class defines the interface for the key generation algorithm
//        Date: 2005-07-13
//   Copyright: Vector Informatik GmbH - 2005
//****************************************************************************************

#ifndef KEY_GEN_RESULT_EX_INTERFACE_H
#define KEY_GEN_RESULT_EX_INTERFACE_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
#define EXT_C	extern "C"
#else
#define EXT_C	extern
#endif

#ifdef KEYGENALGO_EXPORTS
#define KEYGENALGO_API extern "C" __declspec(dllexport)
#else
#define KEYGENALGO_API __declspec(dllimport)
#endif



typedef enum VKeyGenResultEx
{
  KGR_Ok = 0,
  KGR_BufferToSmall = 1,
  KGR_SecurityLevelInvalid = 2,
  KGR_VariantInvalid = 3,
  KGR_UnspecifiedError = 4
}VKeyGenResultEX;


// The client has to provide a keyArray buffer and has to transfer this buffer - 
// including its max size - to the GenerateKeyEx method. The method checks, if the size is
// sufficient. The client can discover the required size by examinig the service used
// to transfer the key to the ECU. If the key size is calculated during runtime, it's
// possible to return that value via the 'oActualKeyArraySize' parameter.
// Returns false if the key could not be generated:
//  -> keyArraySize to small
//  -> generation for specified security level not possible
//  -> variant unknown
// This interface is supported by 
//  - CANape 5.6 SP2 and higher
//  - CANdito 1.6 SP2 and higher

typedef KEYGENALGO_API VKeyGenResultEX   GenerateKeyEx(const unsigned char* , unsigned int ,const unsigned int , const char* ,unsigned char* , unsigned int,unsigned int/*&*/,
   const char* );

KEYGENALGO_API     GenerateKeyEx;


#endif // KEY_GEN_RESULT_EX_INTERFACE_H
