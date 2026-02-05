//*****************************************************************************
//* Header : AS_Dll.h
//*
//* Date       Name               Changes
//* 05.04.97  G.Schmid           
//* 01.10.01  J.Bauer         -  FileName from sDLL.h to AS_Dll.h
//*                           -  Copy to PMS2001 environment
//*                           -  comments to english
//*
//*****************************************************************************

//*****************************************************************************
//                              DLL Makro Definitionen
//*****************************************************************************
// '_BUILD_DLL' -> creating DLL and its functions
// '_USE_DLL'   -> using functions of DLL

#if defined (_DLL_)
#  undef _DLL_
#endif

#if defined (_BUILD_DLL)
#  define _DLL_ __declspec( dllexport )            // for building DLL
#else
#  if defined (_USE_DLL)
#    define _DLL_ __declspec( dllimport )          // for using DLL
#  else 
#    define _DLL_                                  // dummy, if header without any dll
#  endif
#endif
