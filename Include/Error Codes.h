//==============================================================================
//
// Title:		Error Codes.h
// Purpose:		This header file defines a set of error codes to be used across various test blocks, replacing the original use of "__LINE__"
//				or specific line numbers (e.g. "return(__LINE__);").
//				The goal is to simplify failure disposition by referencing error codes rather than simply line numbers, which can vary across different
//				software versions (around 50 versions in use as of writing).
// 				This approach was prompted by the challenges encountered in identifying and addressing failures in TestBlock 90: Read Error Memory,
//				specifically those not related to DEM/DTC.
//
//
// By:			J.PAGALA
// Created on:	Apr-03-2025 at 8:16:49 AM
// Copyright:	Continental AG. All Rights Reserved.
//
//==============================================================================

#ifndef __ErrorCodes_H__
	#define __ErrorCodes_H__

	#ifdef __cplusplus
	    extern "C" {
	#endif

//==============================================================================
// Include files

	#include "cvidef.h"
//==============================================================================
//
//
//	How to use: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//		for procedures with this type of error handling:
//				return(__LINE__); 
//		
//		replace it with:
//				return(errWhiteSample + __LINE__);
//				or
//				return(errBlackSample - __LINE__);
//
//		********take note of the operation, plus(+) for whitelist and minus(-) for blacklist
//		********in case the operation was wrong, blacklist will still be negative since __LINE__ is too small compared to blacklist error code, same goes for whitelist
//
//
//		for easier identification, all negative(-) error codes are blacklist, and positive(+) are whitelist
//
//
//
// Convention: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	first number: 1 for whitelist, -1 for blacklist
//	2nd & 3rd numbers: type of error, depends on the purpose of the function
//									new type of error will be under the discretion of the TE
//	last 5 numbers: reserved for the line of code where the return command was called
//
//
//
//  example error code:
//		12305123
//		|| |
//		|| |____ 05123: return command was called at line 5123, 5-digits (00000 ~ 99999)
//		||  
//		||______ 23: error type control number, 2-digits (00~99)  
//		|
//		|_______ 1: whitelist or blacklist, (+1) for whitelist and (-1) for blacklist
//
//
//	meaning of the example:
//			12305123 = error is a WHITELIST with error type #23 which was called at LINE #5123
//
//
//
//
//
//		
//############### WHITELIST #####################################

		
		#define errWhiteSample		10000000	//used for trial only
		#define errWhiteRelay		10100000	//relay switching errors (hardware)
		#define errWhitePromik		10200000	//ProMik related, initialization or closing
		#define errWhitePSupply		10300000	//Power supply related, on/off
		#define errWhiteA2LFile		10400000	//A2L file related, accessing, reading, CRC, or closing
		#define errWhiteSAPBOMrel	10500000	//errors relating to SAP BOM comparison (e.g. Nissan Spare Part Number comparison with SAP BOM)

		#define errWhiteReserve2	10600000	//
		#define errWhiteReserve3	10700000	//
		#define errWhiteReserve4	10800000	//
		#define errWhiteReserve5	10900000	//
		#define errWhiteReserve6	11000000	//
		#define errWhiteReserve7	11100000	//
		#define errWhiteReserve8	11200000	//
		#define errWhiteReserve9	11300000	//
		#define errWhiteReserve10	11400000	//
		#define errWhiteReserve11	11500000	//
		#define errWhiteReserve12	11600000	//
		#define errWhiteReserve13	11700000	//
		#define errWhiteReserve14	11800000	//
		#define errWhiteReserve15	11900000	//
		#define errWhiteReserve16	12000000	//
		#define errWhiteReserve17	12100000	//
		#define errWhiteReserve18	12200000	//
		#define errWhiteReserve19	12300000	//
		#define errWhiteReserve20	12400000	//
		#define errWhiteReserve21	12500000	//
		
		
		
		
//############### BLACKLIST #####################################
		
		
		#define errBlackSample		-10000000	//used for trial only
		#define errBlackCANrel		-10100000	//CAN related, initialization or closing
		#define errBlackXCPrel		-10200000	//XCP related, initialization or closing
		#define errBlackErrRead		-10300000	//Error during reading/writing of DTC/DEM error
		#define errBlackRuntime		-10400000	//Application runtime too short
		
		#define errBlackReserve1	-10500000	//
		#define errBlackReserve2	-10600000	//
		#define errBlackReserve3	-10700000	//
		#define errBlackReserve4	-10800000	//
		#define errBlackReserve5	-10900000	//
		#define errBlackReserve6	-11000000	//
		#define errBlackReserve7	-11100000	//
		#define errBlackReserve8	-11200000	//
		#define errBlackReserve9	-11300000	//
		#define errBlackReserve10	-11400000	//
		#define errBlackReserve11	-11500000	//
		#define errBlackReserve12	-11600000	//
		#define errBlackReserve13	-11700000	//
		#define errBlackReserve14	-11800000	//
		#define errBlackReserve15	-11900000	//
		#define errBlackReserve16	-12000000	//
		#define errBlackReserve17	-12100000	//
		#define errBlackReserve18	-12200000	//
		#define errBlackReserve19	-12300000	//
		#define errBlackReserve20	-12400000	//
		#define errBlackReserve21	-12500000	//


		
		
//==============================================================================
// Global functions

int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __ErrorCodes_H__ */		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
