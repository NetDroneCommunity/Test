/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_PROVIDER_H___
#define ___NETDRONE_NET_PROVIDER_H___

#include "../util/Providing.h"
#include "../util/Singleton.h"

#define g_kConnectorProvider CConnectorProvider::GetInstance()

class CConnectorProvider : public CProviding, public CSingleton< CConnectorProvider > {
public:
	CConnectorProvider() {}
	virtual ~CConnectorProvider() {}

};

#endif // ___NETDRONE_NET_PROVIDER_H___

/* EOF */
