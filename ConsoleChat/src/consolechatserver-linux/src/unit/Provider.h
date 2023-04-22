/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UNIT_PROVIDER_H___
#define ___SERVICE_UNIT_PROVIDER_H___

#include <netdrone/CommonHeader.h>

#define g_kPlayerProvider CPlayerProvider::GetInstance()

class CPlayerProvider : public CProviding, public CSingleton< CPlayerProvider > {
public:
	CPlayerProvider() {}
	virtual ~CPlayerProvider() {}

};

#endif // ___SERVICE_UNIT_PROVIDER_H___

/* EOF */
