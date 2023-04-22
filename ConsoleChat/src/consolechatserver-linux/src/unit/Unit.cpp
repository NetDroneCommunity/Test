/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "Unit.h"

#include "../CommonHeader.h"

CUnit::CUnit() : m_ptPlayerData(NULL), m_ptPlayerSums(NULL), m_pkRoomHandler(NULL), m_tkPlayingTick(0) {
}

bool
CUnit::Update() {
	return true;
}

void
CUnit::Clear() {
	m_tkPlayingTick = 0;
}

bool
CUnit::Broadcast(CCommand& rkCommand_, INT iSize_, CUnit* pkFrom_, CUnit* pkTo_) {
	if(isptr(m_pkRoomHandler)) {
		if(m_pkRoomHandler->InRoom()) {
			return m_pkRoomHandler->Broadcast(rkCommand_, iSize_, pkFrom_, pkTo_);
		} else {
			TRACE("user is not in room: ");
		}
	}
	return false;
}

CUnit*
CUnit::GetTypeAs(INT o) {
	if(UNIT_SELF == o) {
		return this;
	}
	return NULL;
}

/* EOF */
