/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_KEY_H___
#define ___NETDRONE_UTIL_KEY_H___

#include "../Defines.h"

struct SBogoKey {
	SBogoKey() : trunk(0), key(0) {}
	SBogoKey(UINT16 o, UINT16 p) : trunk(o), key(p) {}
	SBogoKey(const UINT32 o) : trunk(o >> 16), key(UINT32(o) & 0x0000FFFF) {}

	UINT32 GetKey()	{ return (trunk<<16) | key; }

//		UINT32 operator()() {
//			return (trunk<<16) | key;
//		}

	void operator()(UINT16 o, UINT16 p) {
		trunk = o;
		key = p;
	}

	void operator()(UINT32 o) {
		trunk = o>>16;
		key = UINT32(o) & 0x0000FFFF;
	}

	UINT16 trunk;
	UINT16 key;
};

struct SRoomKey {
	SRoomKey() : channel(0), index(0) {}
	SRoomKey(UINT16 o, UINT16 p) : channel(o - 1), index(p) {}
	SRoomKey(const UINT16 o) : channel((o>>10) - 1), index(UINT16(o) & 0x03FF) {}

	UINT16 GetKey()	{ return ((channel+1)<<10) | index; }

//		UINT16 operator()() {
//			return ((channel+1)<<10) | index;
//		}

	void operator()(UINT16 o, UINT16 p) {
		channel = o - 1;
		index = p;
	}

	void operator()(UINT16 o) {
		channel = (o>>10) - 1;
		index = (UINT16(o) & 0x03FF);
	}

	UINT16 channel:6;
	UINT16 index:10;
};

inline SBogoKey SplitBogoKey(UINT32 o) {
	return SBogoKey(o);
}

inline SRoomKey SplitRoomKey(UINT16 o) {
	return SRoomKey(o);
}

inline UINT32 GenerateBogoKey(UINT16 o, UINT16 p) { return ((o << 16) | p); }

inline UINT16 GenerateRoomKey(UINT16 o, UINT16 p) { return (((o + 1) << 10) | p); }

#endif // ___NETDRONE_UTIL_KEY_H___

/* EOF */
