/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_TICK_H___
#define ___NETDRONE_UTIL_TICK_H___

#include "Singleton.h"

#include "../Defines.h"

#define g_kTick CTick::GetInstance()

class CTick : public CSingleton< CTick > {
public:
	CTick();
	virtual ~CTick();

			INT		Update();
			void	Reset(UINT64);

			UINT64	GetServerTick();
			tick_t	GetTick();
			INT		GetDeltaMSec();
			bool	IsUpdate();
			UINT	GetCSec();
			UINT64	GetMSec();
			time_t	GetTime();

protected:
			UINT64	m_uliTick;
			time_t	m_tmTime;
			INT		m_iDeltaTick;

			UINT64	m_uliServerTick;

			time_t	m_tmStartedTime;

};

#endif // ___NETDRONE_UTIL_TICK_H___

/* EOF */
