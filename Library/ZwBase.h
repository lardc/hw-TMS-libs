// -----------------------------------------
// General definitions
// -----------------------------------------

#ifndef __ZW_BASE_H
#define __ZW_BASE_H

// Common definitions
//
#define MHZ           *1000000L
#define KHZ           *1000L
#define HZ            *1L

#ifndef FALSE
#define FALSE         (1 == 0)
#endif

#ifndef TRUE
#define TRUE          (1 == 1)
#endif

#ifndef NULL
#define NULL          ((void*)0)
#endif

// Type definitions
//
typedef long double           Float64;  // Double precision floating point
typedef long double         * pFloat64;
typedef float                 Float32;  // Single precision floating point
typedef float               * pFloat32;
typedef signed   long long    Int64S;   // Signed   64 bit quantity
typedef signed   long long  * pInt64S;
typedef unsigned long long    Int64U;   // Unsigned 64 bit quantity
typedef unsigned long long  * pInt64U;
typedef signed   long         Int32S;   // Signed   32 bit quantity
typedef signed   long       * pInt32S;
typedef unsigned long         Int32U;   // Unsigned 32 bit quantity
typedef unsigned long       * pInt32U;
typedef signed   int          Int16S;   // Signed   16 bit quantity
typedef signed   int        * pInt16S;
typedef unsigned int          Int16U;   // Unsigned 16 bit quantity
typedef unsigned int        * pInt16U;
typedef unsigned int          Boolean;  // Boolean
typedef unsigned int        * pBoolean;
//
// System aliases
typedef Int16U				Uint16;
typedef Int16S				int16;
typedef Int32U				Uint32;
typedef Int32S				int32;
//
// Compatible aliases
typedef Int16U				UI_16;
typedef Int16S				SI_16;
typedef Int16U				UI_8;
typedef Int16S				SI_8;
typedef Int32U				UI_32;
typedef Int32S				SI_32;

// Limits
//
#define INT16U_MAX			65535u
#define INT16S_MAX			32767
#define INT16S_MIN			(-INT16S_MAX-1)
#define INT32U_MAX			4294967295ul
#define INT32S_MAX			2147483647
#define INT32S_MIN			(-INT32S_MAX-1)
#define INT64U_MAX			18446744073709551615
#define INT64S_MAX			9223372036854775807
#define INT64S_MIN			(-INT64S_MAX-1)

// Bits
//
#define BIT0  0x1u
#define BIT1  0x2u
#define BIT2  0x4u
#define BIT3  0x8u
#define BIT4  0x10u
#define BIT5  0x20u
#define BIT6  0x40u
#define BIT7  0x80u
#define BIT8  0x100u
#define BIT9  0x200u
#define BIT10 0x400u
#define BIT11 0x800u
#define BIT12 0x1000u
#define BIT13 0x2000u
#define BIT14 0x4000u
#define BIT15 0x8000u
#define BIT16 0x10000u
#define BIT17 0x20000u
#define BIT18 0x40000u
#define BIT19 0x80000u
#define BIT20 0x100000u
#define BIT21 0x200000u
#define BIT22 0x400000u
#define BIT23 0x800000u
#define BIT24 0x1000000u
#define BIT25 0x2000000u
#define BIT26 0x4000000u
#define BIT27 0x8000000u
#define BIT28 0x10000000u
#define BIT29 0x20000000u
#define BIT30 0x40000000u
#define BIT31 0x80000000u

#endif // __ZW_BASE_H
