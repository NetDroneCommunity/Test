/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "UnitMgr.h"

#include "../CommonHeader.h"

template<> CUnitMgr* CSingleton< CUnitMgr >::ms_Instance = NULL;

bool
CUnitMgr::Release() {
	for(UINT i = 0; i < (UINT)(iMAX_TRUNK_CAPACITY); ++i) {
		for(UINT j = 0; j < m_auiTrunkCapacity[i]; ++j) {
			if(isptr(m_ppakUnit[i][j])) {
				m_ppakUnit[i][j]->Clear();
				m_ppakUnit[i][j]->Release();
				SAFE_DELETE(m_ppakUnit[i][j]);
			}
			m_ppakUnit[i][j] = NULL;
		}
		m_auiTrunkCapacity[i] = 0;
	}
	return true;
}

INT
CUnitMgr::NewTrunk(UINT uiTrunk_, UINT uiSize_) {
	if(uiTrunk_ < (UINT)(iMAX_TRUNK_CAPACITY)) {
		if(0 == m_auiTrunkCapacity[uiTrunk_]) {
			UINT uiSize = Min(uiSize_, UINT(iMAX_UNIT_CAPACITY));

			m_auiTrunkCapacity[uiTrunk_] = uiSize;
			CUnit* pkUnit = NULL;
			m_ppakUnit[uiTrunk_] = new CUnit*[uiSize];

			for(UINT i=0; i<uiSize; ++i) {
				pkUnit = (CUnit*)g_kPlayerProvider.Create();
				if(isptr(pkUnit)) {
					m_ppakUnit[uiTrunk_][i] = pkUnit;
					pkUnit->Initialize();
					pkUnit->Clear();
				} else {
					for(UINT j=0; j<i; ++j) {
						if(isptr(m_ppakUnit[uiTrunk_][j])) {
							m_ppakUnit[uiTrunk_][j]->Release();
							SAFE_DELETE(m_ppakUnit[uiTrunk_][j]);
						}
						m_ppakUnit[uiTrunk_][j] = NULL;
					}
					return 0;
				}
			}	///< for

			return uiSize;
		}
	}
	return -1;
}

CUnit*
CUnitMgr::GetUnit(UINT uiTrunk_, UINT uiKey_) {
	if(InTrunkCapacity(uiTrunk_, uiKey_)) {
		return m_ppakUnit[uiTrunk_][uiKey_];
	}
	return NULL;
}

CPlayerEx*
CUnitMgr::GetPlayer(UINT uiTrunk_, UINT uiKey_) {
	if(InTrunkCapacity(uiTrunk_, uiKey_)) {
		return (CPlayerEx*)(m_ppakUnit[uiTrunk_][uiKey_]->GetTypeAs(UNIT_PLAYER));
	}
	return NULL;
}

CPlayerEx*
CUnitMgr::FindPlayer(UINT uiTrunk_, const char* szName_) {
	UINT uiTrunkCapacity = GetTrunkCapacity(uiTrunk_);
	if(0 < uiTrunkCapacity) {
		for(UINT i=0; i<uiTrunkCapacity; ++i) {
			CPlayerEx* pkPlayer = GetPlayer(uiTrunk_, i);
			if(isptr(pkPlayer)) {
				if(0 == strncmp(pkPlayer->GetName(), szName_, iUNIT_NAME_LEN)) {
					return pkPlayer;
				}
			}
		}	///< for
	}
	return NULL;
}

CPlayerEx*
CUnitMgr::FindPlayer(UINT uiTrunk_, UINT uiAid_) {
	UINT uiTrunkCapacity = GetTrunkCapacity(uiTrunk_);
	if(0 < uiTrunkCapacity) {
		for(UINT i=0; i<uiTrunkCapacity; ++i) {
			CPlayerEx* pkPlayer = GetPlayer(uiTrunk_, i);
			if(isptr(pkPlayer)) {
				if(uiAid_ == pkPlayer->GetAid()) {
					return pkPlayer;
				}
			}
		}
	}
	return NULL;
}

INT
CUnitMgr::GetTrunkCapacity(UINT o) {
	if(o < (UINT)(iMAX_TRUNK_CAPACITY)) {
		return m_auiTrunkCapacity[o];
	} else {
		TRACE("Capacity is overflow: "<<o);
	}
	return -1;
}

bool
CUnitMgr::InTrunkCapacity(UINT o, UINT p) {
	if(o < (UINT)(iMAX_TRUNK_CAPACITY)) {
		if(p < m_auiTrunkCapacity[o]) {
			return true;
		} else {
			TRACE("TRUNK is "<<o<<", Capacity is overflow: "<<p<<"("<<m_auiTrunkCapacity[o]<<")");
		}
	} else {
		TRACE("TRUNK is error: maybe, this is not PC: "<<o<<":"<<p);
	}
	return false;
}

/* EOF */
