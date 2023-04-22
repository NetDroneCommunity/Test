/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_SERVERMGR_H___
#define ___SERVICE_WORLD_SERVERMGR_H___

#include <netdrone/CommonHeader.h>

#define g_kServerMgr CServerMgr::GetInstance()

class CServerMgr : public CSingleton< CServerMgr > {
public:
	CServerMgr() {}
	~CServerMgr() {}

	bool		Initialize();
	bool		Release();

	void		Update();

	void		IncreasedUser();
	void		DecreasedUser();

	bool		InChannel(INT, CPlayer&);
	void		OutChannel(INT, CPlayer&);

	bool		IsAvailable();

	void		Shutdown();

private:

};

#endif	// ___SERVICE_WORLD_SERVERMGR_H___

/* EOF */
