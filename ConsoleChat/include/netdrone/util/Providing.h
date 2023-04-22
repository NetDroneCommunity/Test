/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_PROVIDING_H___
#define ___NETDRONE_UTIL_PROVIDING_H___

#include <map>

#include "../Defines.h"

class CProviding {
	typedef void*	(*CreateCallback)();

public:
	CProviding() { m_kMap.clear(); }
	virtual ~CProviding() { m_kMap.clear(); }

	virtual void	Clear()												{ m_kMap.clear(); }

	void*			Create();
	void*			Create(INT iTypeId_);

	bool			Register(CreateCallback pvfCreate_);
	bool			Register(INT iTypeId_, CreateCallback pvfCreate_);

	bool			Unregister(INT iTypeId_)							{ return (m_kMap.erase(iTypeId_) == 1); }

	bool			Working()											{ return (false == m_kMap.empty()); }

private:
	typedef std::map< INT, CreateCallback > stl_Map;
	stl_Map			m_kMap;

};

#endif // ___NETDRONE_UTIL_PROVIDING_H___

/* EOF */
