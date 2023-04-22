/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UNIT_UNITMGR_H___
#define ___SERVICE_UNIT_UNITMGR_H___

#include <netdrone/CommonHeader.h>

#include "PlayerEx.h"

#define g_kUnitMgr CUnitMgr::GetInstance()

class CUnitMgr : public CSingleton< CUnitMgr > {
public:
	CUnitMgr() { memset(m_ppakUnit, 0, sizeof(m_ppakUnit)); memset(m_auiTrunkCapacity, 0, sizeof(m_auiTrunkCapacity)); }
	~CUnitMgr() {}

	bool		Release();

	INT			NewTrunk(UINT, UINT);

	CUnit*		GetUnit(UINT, UINT);
	CPlayerEx*	GetPlayer(UINT, UINT);

	CUnit*		GetUnit(SBogoKey o)		{ return GetUnit(o.trunk, o.key); }
	CPlayerEx*	GetPlayer(SBogoKey o)	{ return GetPlayer(o.trunk, o.key); }

	CUnit*		GetUnit(UINT o)			{ return GetUnit(SplitBogoKey(o)); }
	CPlayerEx*	GetPlayer(UINT o)		{ return GetPlayer(SplitBogoKey(o)); }

	CPlayerEx*	FindPlayer(UINT, const char*);
	CPlayerEx*	FindPlayer(UINT, UINT);

	INT			GetTrunkCapacity(UINT);

private:
	bool		InTrunkCapacity(UINT, UINT);

private:
	CUnit**		m_ppakUnit[iMAX_TRUNK_CAPACITY];
	UINT		m_auiTrunkCapacity[iMAX_TRUNK_CAPACITY];

};

#endif	// ___SERVICE_UNIT_UNITMGR_H___

/* EOF */
