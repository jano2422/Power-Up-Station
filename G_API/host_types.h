#ifndef _HOST_TYPES_H
#define _HOST_TYPES_H

#define HOST_ENDIAN__LITTLE

#if defined(HOST_TYPES__ALREADY_DEFINED)
	// the host types are already defined
#elif defined(HOST_TYPES__USE_SYSTEM_TYPES)
	typedef  u8  u8_t;
	typedef  u16  u16_t;
	typedef  u32  u32_t;
	typedef  u64  u64_t;
	typedef  float32  float32_t;
	typedef  float64  float64_t;
#else
	#ifndef _U8_T_DEFINED
		#define _U8_T_DEFINED
		typedef unsigned char u8_t;
	#endif

	#ifndef _S8_T_DEFINED
		#define _S8_T_DEFINED
		typedef signed char s8_t;
	#endif

	#ifndef _U16_T_DEFINED
		#define _U16_T_DEFINED
		typedef unsigned short  u16_t;
	#endif

	#ifndef _S16_T_DEFINED
		#define _S16_T_DEFINED
		typedef short  s16_t;
	#endif

	#ifndef _U32_T_DEFINED
		#define _U32_T_DEFINED
		typedef unsigned long  u32_t;
	#endif

	#ifndef _S32_T_DEFINED
		#define _S32_T_DEFINED
		typedef long  s32_t;
	#endif

	#ifndef _U64_T_DEFINED
		#define _U64_T_DEFINED

		typedef struct {
			u32_t  Low;
			u32_t  High;
		} u64_t;
	#endif

	#ifndef _FLOAT32_T_DEFINED
		#define _FLOAT32_T_DEFINED
		typedef float  float32_t;
	#endif

	#ifndef _FLOAT64_T_DEFINED
		#define _FLOAT64_T_DEFINED
		typedef double  float64_t;
	#endif
#endif


#else
	#error "this file is included twice"
#endif
