/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UTIL_CFGMGR_H___
#define ___SERVICE_UTIL_CFGMGR_H___

#include <netdrone/CommonHeader.h>

#define g_kCfgMgr CCfgMgr::GetInstance()

class CCfgMgr : public CConfig, public CSingleton< CCfgMgr > {
public:
	CCfgMgr();
	virtual ~CCfgMgr();

	virtual bool		Initialize(const char*, const char*, INT);
	virtual bool		Release();

	virtual bool		Load(const char*);

private:

};

#endif // ___SERVICE_UTIL_CFGMGR_H___

/* EOF */
