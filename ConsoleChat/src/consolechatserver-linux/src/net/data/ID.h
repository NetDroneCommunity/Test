/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_DATA_ID_H___
#define ___SERVICE_NET_DATA_ID_H___

#include <netdrone/CommonHeader.h>

#include "../../GlobalVariable.h"

#pragma pack(4)

// Account 관리가 필요 하다면 Aid를 보내 줘야 함
struct SIdAuthorizeGsToCl {
	SIdAuthorizeGsToCl()					{ memset(this, 0, sizeof(SIdAuthorizeGsToCl)); }

	const char*	GetName()					{ return name; }
	void		SetName(const char& o)		{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }

	UINT32	key;
	UINT32	aid;

	UINT32	padding1:11;
	UINT32	padding2:1;
	UINT32	padding3:1;
	UINT32	padding4:1;
	UINT32	padding5:5;
	UINT32	padding6:1;
	UINT32	padding7:1;
	UINT32	padding8:1;
	UINT32	padding9:10;	///< 최대 1023 까지

	UINT32	public_ip;
	UINT16	public_port;
	UINT16	padding;

	UINT64	tick;

	char	name[iUNIT_NAME_LEN+1];
};

struct SIdAuthorizeClToGs {
	SIdAuthorizeClToGs()					{ memset(this, 0, sizeof(SIdAuthorizeClToGs)); }

	const char*	GetLoginId()				{ return login_id; }
	void		SetLoginId(const char& o)	{ memcpy(login_id, &o, iLOGIN_ID_LEN); login_id[iLOGIN_ID_LEN] = '\0'; }

	UINT16	major_version;
	UINT16	minor_version;
	UINT32	channel_index;
	UINT32	local_ip;
	UINT16	local_port;
	UINT16	padding;

	char	login_id[iLOGIN_ID_LEN+1];
};

struct SIdPongGsToCl {
	UINT64	tick;
};

#pragma pack()

#endif // ___SERVICE_NET_DATA_ID_H___

/* EOF */
