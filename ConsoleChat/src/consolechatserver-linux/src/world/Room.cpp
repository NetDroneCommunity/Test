/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "Room.h"

#include "../CommonHeader.h"

bool
CRoom::Initialize() {
	Clear();
	return true;
}

bool
CRoom::Release() {
	for(UINT i=0; i<GetTopCount(); ++i) {
		if(isptr(m_pkMembers[i])) {
			m_pkMembers[i] = NULL;
		} else {
			TRACE("critical error: unit[" << i << "] is null: " << GetTopCount());
		}
	}

	memset(m_szName, 0, sizeof(m_szName));

	m_uiTopCount = 0;

	m_usId = 0;
	m_bOffset = false;
	m_uiStageId = 0;
	m_uiMaxUser = 0;

	return true;
}

void
CRoom::Clear() {
	//TRACE("Clear: ROOM_LEAVE: ");
	CCommand kCommand;
	kCommand.SetOrder(ROOM_LEAVE);
	kCommand.SetExtra(CLOSE);

	for(UINT i = 0; i < GetTopCount(); ++i) {
		CPlayer* pkPlayer = m_pkMembers[i];
		if(isptr(pkPlayer)) {
			if(isptr(pkPlayer->GetRoomHandler())) {
				pkPlayer->GetRoomHandler()->Clear();
			}
			pkPlayer->Launcher(kCommand);
		}
	}

	memset(m_pkMembers, 0, sizeof(m_pkMembers));
	m_uiTopCount = 0;

	m_bOffset = false;
	m_uiStageId = 0;
	m_uiMaxUser = 0;
}

/* EOF */
