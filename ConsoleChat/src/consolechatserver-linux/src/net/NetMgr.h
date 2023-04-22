/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_NETMGR_H___
#define ___SERVICE_NET_NETMGR_H___

#include <netdrone/CommonHeader.h>

#include "NetworkEx.h"

#define g_kNetMgr CNetMgr::GetInstance()

class CNetMgr : public CSingleton< CNetMgr > {
public:
	CNetMgr();
	~CNetMgr();

	bool			Initialize(INT);
	bool			Release();

	bool			Update();

	bool			StartThread();
	bool			CancelThread();

	CNetwork&		GetMasterNetwork()				{ return m_kMasterNetwork; }
	CNetwork&		GetSlaveNetwork()				{ return m_kSlaveNetwork; }

	bool			CheckProtocol();

private:
	CMasterNetwork	m_kMasterNetwork;
	CSlaveNetwork	m_kSlaveNetwork;

	bool			m_bInitialized;

};

#endif // ___SERVICE_NET_NETMGR_H___

/* EOF */
