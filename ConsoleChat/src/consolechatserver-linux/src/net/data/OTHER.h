/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_DATA_OTHER_H___
#define ___SERVICE_NET_DATA_OTHER_H___

#include <netdrone/CommonHeader.h>

#include "../../GlobalVariable.h"

#pragma pack(4)

// OTHER_STATUS
// Extra : NONE
// Option : size
struct SOtherStatusGsToCl {
	UINT32	actor;
};

// OTHER_CHAT
// Extra : NONE, CHECK, NEW
// Option : size
struct SOtherChatGsToCl {
	SOtherChatGsToCl() { memset(this, 0, sizeof(SOtherChatGsToCl)); }

	void	SetName(const char& o)		{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }
	void	SetContent(const char& o)	{ memcpy(content, &o, iMAX_CHAT_LEN); content[iMAX_CHAT_LEN] = '\0'; }

	char	name[iUNIT_NAME_LEN+1];
	char	content[iMAX_CHAT_LEN+1];
};

#pragma pack()

#endif // ___SERVICE_NET_DATA_OTHER_H___

/* EOF */
