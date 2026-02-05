/*****************************************************************************/
/*                                                                           */
/*  Header : CVITypes.H                                                      */
/*                                                                           */
/*****************************************************************************

	Version 1.0     06.06.2000    Juergen Bauer
	------------------------------------------------

	Version 1.1     11.10.2001    Juergen Bauer
	------------------------------------------------
	- new typedef for ulong
	
	Version 1.2     10.05.2002    Juergen Bauer
	------------------------------------------------
	- UnDef Status for TSwitch and changed syntax for typedefs

******************************************************************************

	Dokumentation:
	--------------
	base 32 bit types for LabWindows CVI

******************************************************************************/
#if !defined ( __CVITYPES_H)
#define __CVITYPES_H

typedef unsigned char  byte;
typedef unsigned long  ulong;
typedef unsigned short word;

typedef enum {False,True} Boolean;

typedef enum {Off,On,UnDef} TSwitch;


/*****************************************************************************/
#endif
