/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_FRAMEWORK_H___
#define ___SERVICE_FRAMEWORK_H___

#include <netdrone/CommonHeader.h>

#define g_kFramework CFramework::GetInstance()

class CFramework : public CSingleton< CFramework > {
public:
	CFramework() : m_bInitialized(false), m_bDoing(true) {}
	~CFramework() { Release(); }

	bool	Initialize(const char*, const char*, INT);
	bool	Release();

	bool	Update();

	bool	IsDoing()			{ return m_bDoing; }
	void	SetDoing(bool o)	{ m_bDoing = o; }

	bool	Startup();
	bool	Shutdown();

private:
	bool	m_bInitialized;
	bool	m_bDoing;

};

#endif // ___SERVICE_FRAMEWORK_H___

/* EOF */
