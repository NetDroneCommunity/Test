/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_RETYPEDEFINED_H___
#define ___NETDRONE_RETYPEDEFINED_H___

#include <stdint.h>
//#include <boost/uuid/uuid.hpp>

// socket types
typedef signed int				SOCKET;

// Unsigned base types
typedef unsigned int			UINT, *PUINT;		// system size unsigned.	(ui)
typedef uint8_t					BYTE;				// 8-bit  unsigned.			(by)
typedef uint16_t				WORD;				// 16-bit unsigned.			(w)
typedef uint32_t				DWORD;				// 32-bit unsigned.			(dw)
typedef uint64_t				QWORD;				// 64-bit unsigned.			(qw)

typedef unsigned long			ULONG;				// (ul)
typedef uint32_t				ULONG32;			// (ul)
typedef uint64_t				ULONG64;			// (ull)

// Signed base types
typedef char					CHAR;				// 8-bit  signed.		(c)
typedef int8_t					SBYTE;				// 8-bit  signed.		(sb)

typedef int						INT, *PINT;			// system size signed.	(i)
typedef int16_t					INT16, *PINT16;		// 16-bit signed.		(s)
typedef int32_t					INT32, *PINT32;		// 32-bit signed.		(i)
typedef int64_t					INT64, *PINT64;		// 64-bit signed.		(li)

typedef uint16_t				UINT16, *PUINT16;	// 16-bit unsigned.		(us)
typedef uint32_t				UINT32, *PUINT32;	// 32-bit unsigned.		(ui)
typedef uint64_t				UINT64, *PUINT64;	// 64-bit unsigned.		(uli)

typedef int32_t					LONG32, *PLONG32;	// (l)
typedef int64_t					LONG64, *PLONG64;	// (ll)

// Other base types
typedef float					FLOAT;				// 32-bit IEEE floating point.	(f)
typedef double					DOUBLE;				// 64-bit IEEE double.			(d)
typedef unsigned int			tick_t;				// system size unsigned.		(tk)

//typedef boost::uuids::uuid		UUID;

#endif // ___NETDRONE_RETYPEDEFINED_H___

/* EOF */
