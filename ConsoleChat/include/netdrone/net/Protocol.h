/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_PROTOCOL_H___
#define ___NETDRONE_NET_PROTOCOL_H___

#include <string.h>

#include "../Defines.h"

struct STcpPacketHeader {
	STcpPacketHeader()	{ memset(this, 0, sizeof(STcpPacketHeader)); }

	UINT32	key;			// 송신자 구별 용 Key
	UINT32	bogus:18;		// Hidden dummy
	UINT32	size:11;		// 사용자 헤더를 포함한 패킷 전체 크기 정보 (min: 4, max: 2047)
	UINT32	crypt:3;		// 패킷 타입 정의
};

struct SUdpPacketHeader {
	SUdpPacketHeader() { memset(this, 0, sizeof(SUdpPacketHeader)); }

	UINT32	key;			// 송신자 구별 용 Key
	UINT32	bogus:18;		// Hidden dummy
	UINT32	size:11;		// 사용자 헤더를 포함한 패킷 정체 크기 정보 (man: 4, max: 2047)
	UINT32	crypt:3;		// 패킷 타입 정의

	UINT32	seq:28;			// 패킷 순서
	UINT32	type:4;			// UDP 동작 Type
};

struct SCommandHeader {
	SCommandHeader() { memset(this, 0, sizeof(SCommandHeader)); }

	UINT16	order:10;		// 명령 (max: 1023)
	UINT16	mission:6;		// 추가 정보 (max: 63)
	BYTE	extra;			// 결과 처리 (max: 255)
	BYTE	option;			// 옵션 (max: 255)
};

#endif // ___NETDRONE_NET_PROTOCOL_H___

/* EOF */
