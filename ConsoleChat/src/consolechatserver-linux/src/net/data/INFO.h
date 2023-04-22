/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_DATA_INFO_H___
#define ___SERVICE_NET_DATA_INFO_H___

#include <netdrone/CommonHeader.h>

#include "../../GlobalVariable.h"

#pragma pack(4)

// INFO_SERVER
// Extra : NONE
// Option : not used
struct SInfoServerClToGs {
	UINT16	major_version;
	UINT16	minor_version;
};

struct SInfoServerGsToCl {
	INT32	serial;
	UINT32	key;
};

// INFO_CHANNEL
// Extra : IN, OUT
// Option : channel index
struct SInfoChannelInGsToCl {
	SInfoChannelInGsToCl()				{ memset(this, 0, sizeof(SInfoChannelInGsToCl)); }

	void	SetName(const char& o)		{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }

	UINT32	aid;
	UINT32	key;

	char	name[iUNIT_NAME_LEN+1];
};

struct SInfoChannelOutGsToCl {
	UINT32	key;
};

// INFO_USER_LIST
// Extra : NEW, CHANGE
// Option : count
struct SInfoUserData {
	SInfoUserData()						{ memset(this, 0, sizeof(SInfoUserData)); }

	void	SetName(const char& o)		{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }

	UINT32	aid;
	UINT32	key;

	char	name[iUNIT_NAME_LEN+1];
};

const INT iMAX_PACKET_INFO_USER_LIST = iCOMMAND_DATA_SIZE / sizeof(SInfoUserData);

struct SInfoUserListGsToCl {
	SInfoUserData	list[iMAX_PACKET_INFO_USER_LIST];
};

#pragma pack()

#endif // ___SERVICE_NET_DATA_INFO_H___

/* EOF */
