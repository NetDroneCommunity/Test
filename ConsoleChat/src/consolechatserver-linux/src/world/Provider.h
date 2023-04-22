/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_PROVIDER_H___
#define ___SERVICE_WORLD_PROVIDER_H___

#include <netdrone/CommonHeader.h>

#define g_kRoomProvider CRoomProvider::GetInstance()

class CRoomProvider : public CProviding, public CSingleton< CRoomProvider > {
public:
	CRoomProvider() {}
	virtual ~CRoomProvider() {}

};

#endif // ___SERVICE_WORLD_PROVIDER_H___

/* EOF */
