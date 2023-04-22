/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_DATA_ROOM_H___
#define ___SERVICE_NET_DATA_ROOM_H___

#include <netdrone/CommonHeader.h>

#include "../../GlobalVariable.h"

#pragma pack(4)

// ROOM_CREATE
// Extra : OK, NOT_ENGOUGH
// Option : channel
struct SRoomCreateClToGs {
	SRoomCreateClToGs() { memset(this, 0, sizeof(SRoomCreateClToGs)); }

	const char*	GetName()				{ return name; }
	void		SetName(const char& o)	{ memcpy(name, &o, iROOM_NAME_LEN); name[iROOM_NAME_LEN] = '\0'; }

	UINT32	padding:1;
	UINT32	max:6;
	UINT32	stage_id:25;

	char	name[iROOM_NAME_LEN+1];

};

struct SRoomCreateGsToCl {
	UINT32	id;
};

// ROOM_JOIN
// Extra : OK, DENY, FAIL, FULL
// Option : not used
struct SRoomJoinClToGs {
	UINT32	id;
};

struct SRoomJoinGsToCl {
	SRoomJoinGsToCl() { memset(this, 0, sizeof(SRoomJoinGsToCl)); }

	const char*	GetName()				{ return name; }
	void		SetName(const char& o)	{ memcpy(name, &o, iROOM_NAME_LEN); name[iROOM_NAME_LEN] = '\0'; }

	UINT32	id;

	UINT32	offset:1;
	UINT32	max:6;
	UINT32	stage_id:25;

	char	name[iROOM_NAME_LEN+1];
};

// ROOM_INFO
// Extra : IN, OUT
// Option : not used
struct SRoomInfo {
	SRoomInfo() { memset(this, 0, sizeof(SRoomInfo)); }

	const char*	GetName()				{ return name; }
	void		SetName(const char& o)	{ memcpy(name, &o, iROOM_NAME_LEN); name[iROOM_NAME_LEN] = '\0'; }

	UINT32	id;

	UINT32	offset:1;
	UINT32	max:6;
	UINT32	stage_id:25;

	char	name[iROOM_NAME_LEN+1];
};

//
// ROOM_LIST
// Extra: NEW, CHANGE, DONE, EMPTY
// Option : count
//const INT iMAX_PACKET_ROOM_LIST = (iCOMMAND_DATA_SIZE - sizeof(UINT32)) / sizeof(SRoomInfo);
const INT iMAX_PACKET_ROOM_LIST = (iCOMMAND_DATA_SIZE) / sizeof(SRoomInfo);

struct SRoomListGsToCl {
	SRoomInfo	list[iMAX_PACKET_ROOM_LIST];
};

struct SRoomMember {
	SRoomMember()					{ memset(this, 0, sizeof(SRoomMember)); }

	void	SetName(const char& o)	{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }

	UINT32	actor;

	UINT32	public_ip;
	UINT32	local_ip;
	UINT16	public_port;
	UINT16	local_port;

	UINT32	status;

	char	name[iUNIT_NAME_LEN+1];
};

const INT iMAX_PACKET_ROOM_MEMBER_LIST = (iCOMMAND_DATA_SIZE) / sizeof(SRoomMember);

struct SRoomMemberListGsToCl {
	SRoomMember	list[iMAX_PACKET_ROOM_MEMBER_LIST];
};

struct SRoomMemberLeaderGsToCl {
	UINT32	leader;
};

// ROOM_JOIN_OTHER
// Extra : OK
// Option : not used
struct SRoomJoinOtherGsToCl {
	SRoomJoinOtherGsToCl()			{ memset(this, 0, sizeof(SRoomJoinOtherGsToCl)); }

	void	SetName(const char& o)	{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }

	UINT32	actor;

	UINT32	public_ip;
	UINT32	local_ip;
	UINT16	public_port;
	UINT16	local_port;

	char	name[iUNIT_NAME_LEN+1];
};

// ROOM_LEAVE_OTHER
// Extra : OK, CHECK
// Option : not used
struct SRoomLeaveOtherGsToCl {
	UINT32	actor;
	UINT32	leader;
};

// ROOM_START_OTHER
// Extra : OK
// Option : not used
struct SRoomStartOtherGsToCl {
	UINT32	actor;
};

// ROOM_STOP_OTHER
// Extra : OK
// Option : not used
struct SRoomStopOtherGsToCl {
	UINT32	leader;
};

#pragma pack()

#endif // ___SERVICE_NET_DATA_ROOM_H___

/* EOF */
