
/*************************************************************************************/
/*                                                                                   */
/*   driver:                                                                         */
/*                                                                                   */
/*   header:        definitions.h                                                    */
/*   sorce-Code:                                                                     */
/*                                                                                   */
/*   purpose:       definition of project specific constants and variables           */
/*                                                                                   */
/*                                                                                   */
/*   possibilities:                                                                  */
/*                                                                                   */
/*                                                                                   */
/*   autor:         ArzbergerM                                                       */
/*   version:       1.0     for the Produktion Tester                                */
/*   date:          20.11.2006                                                       */
/*                                                                                   */
/*                                                                                   */
/*************************************************************************************/

#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#include <CVITypes.h>




/*****************************************************************************/
// Software Locking for Customer
/*****************************************************************************/

#define _SOFTWARE_LOCKING				"MI38"

/*****************************************************************************/
// Versionsinformationen:
/*****************************************************************************/

//#define _TESTSOFTWARE_VERSION		"V104a"
//#define _TESTSOFTWARE_VERSION		"V202"
//#define _TESTSOFTWARE_VERSION		"V205" //new line control and auto promik
//#define _TESTSOFTWARE_VERSION		"V422" //new sergen
//#define _TESTSOFTWARE_VERSION		"V425"	//16-AUG-23 JC.ANCHETA IMPLEMENTATION FOR REM CA
//#define _TESTSOFTWARE_VERSION		"V42B"	//J.PAGALA (MAR-17-2025) MANDATORY GS RUN
//#define _TESTSOFTWARE_VERSION		"V42E"	//J.PAGALA (MAR-18-2025) CHANGE TEST SEQUENCE FOR DETECTION OF DEM:"OMI_DRV_FAULT_ma"
#define _TESTSOFTWARE_VERSION		"V42H"	//J.PAGALA (MAY-05-2025) ADDED DELAY FOR LONGER RESTBUS SIMULATION & ERROR CODE IMPLEMENTATION

#define _STATION_NAME 					"SRR3xx_EOL - SRR320MI38 - SRR_05 - MI38"  
#define _STATION_STARTUP_NAME   "Station: SRR3xx_EOL - SRR320MI38 - SRR_05 - MI38"




/*****************************************************************************/
#define CAN_PRIVATE
/*****************************************************************************/
/**************************** SW Memory addresses ****************************/ 
/*
// actual address of SW-Modules
#define M32_START_ADDR_BRES 0x000000
#define M32_STOP_ADDR_BRES  0x003df3

//#define M32_START_ADDR_PPAR 0x004000
//#define M32_STOP_ADDR_PPAR    0x007fff

#define M32_START_ADDR_UFBL 0x014000
#define M32_STOP_ADDR_UFBL  0x028f1f

#define M32_START_ADDR_APPL_SW 0x030000
#define M32_STOP_ADDR_APPL_SW  0x0fc32b

#define M32_START_ADDR_PSW 0x030000
#define M32_STOP_ADDR_PSW  0x138983

// address of memory block corresponding to each SW-module
//**********************************    3T0		//	2XX
#define M32_START_ADDR_BRES_BLOCK 	0x00F98000	//0x000000
#define M32_STOP_ADDR_BRES_BLOCK    0x00F9BFFF	//0x003fff

#define M32_START_ADDR_PPAR_BLOCK 	0x00F9C000	//0x004000
#define M32_STOP_ADDR_PPAR_BLOCK    0x00F9FFFF	//0x007fff

#define M32_START_ADDR_UFBL_BLOCK 	0x00FA0000	//0x014000
#define M32_STOP_ADDR_UFBL_BLOCK    0x00FBFFFF	//0x02ffff

#define M32_START_ADDR_APPL_SW_BLOCK 0x00FD0000	//0x030000
#define M32_STOP_ADDR_APPL_SW_BLOCK  0x00FFFFFF	//0x0fffff

#define M32_START_ADDR_APAR_BLOCK 	0x00FC0000  //NO APAR IN 2XX
#define M32_STOP_ADDR_APAR_BLOCK  	0x00FCFFFF	//NO APAR IN 2XX



/*****************************************************************************/

//#define EEPROM_TEMPSN_ADDRESS_OLD 0x1FF0  
#define EEPROM_TEMPSN_ADDRESS 0x0300  

#define FLASH_START_ADDRESS 0xF9FFB0//B0   //since SRR300   PCB_SN		  
#define FLASH_END_ADDRESS 0xF9FFEF


//#define TIME
/*****************************************************************************/
/*****************************************************************************/
// INPUTS
#define PLI_IN_RELEASE_UNIT 	 	  0		// Manual Realease Button
#define PLI_IN_FIXTURE_CLOSED		  1		// Fixture Close Detect
#define PLI_IN_SENSOR_EXTEND	  	  2		// Solinoid Extend Sensor
#define PLI_IN_SENSOR_RETRACT	  	  3		// Solinoid Retract Sensor	
#define PLI_IN_UNIT_PRESENT		  	  4		// Unit Present Sensor
// OUTPUTS
// FIXTURE TYPE
#define PLI_IN_FIXTURE_B0  	  	      8		// Port ID B0-B3 Fixture Type
#define PLI_IN_FIXTURE_B1             9
#define PLI_IN_FIXTURE_B2			 10
#define PLI_IN_FIXTURE_B3			 11
#define PLI_IN_FIXTURE_B4			 12		// Port ID B4-B7 Fixture Number
#define PLI_IN_FIXTURE_B5			 13
#define PLI_IN_FIXTURE_B6			 14
#define PLI_IN_FIXTURE_B7			 14

#define PLI_OUT_LED_RED		 		  0		// Red Light Toggle
#define PLI_OUT_LED_GREEN	 		  1		// Green Light Toggle
#define PLI_OUT_LED_YELLOW	 		  2		// Yellow Light Toggle	
#define PLI_OUT_UNIT_LOCK	 		  3		// Unit Lock while in test	
#define PLI_OUT_SOL_EXT 		  	  	  4		// Solinoid Extend after test 
#define PLI_OUT_MAIN_AIR 	 	 	 16		// Main Air
// SCANNER
#define PORT_SCANNER          		  1     // Scanner Port DM100
#endif
