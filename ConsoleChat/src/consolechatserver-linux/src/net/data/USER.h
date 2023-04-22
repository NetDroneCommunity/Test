/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_DATA_USER_H___
#define ___SERVICE_NET_DATA_USER_H___

#include <netdrone/CommonHeader.h>

#include "../../GlobalVariable.h"

#pragma pack(4)

// USER_CHAT
// Extra : NONE, CHECK, NEW
// Option : size
struct SUserChatClToGs {
	SUserChatClToGs() { memset(this, 0, sizeof(SUserChatClToGs)); }

	void	SetContent(const char* o)	{ memcpy(content, o, iMAX_CHAT_LEN); content[iMAX_CHAT_LEN] = '\0'; }

	char	content[iMAX_CHAT_LEN+1];
};

#pragma pack()

#endif // ___SERVICE_NET_DATA_USER_H___

/* EOF */
