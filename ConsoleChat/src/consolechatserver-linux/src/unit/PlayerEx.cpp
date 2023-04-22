/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "PlayerEx.h"

#include "../CommonHeader.h"

bool
CPlayerEx::Initialize() {
	if(CPlayer::Initialize()) {
		if(false == isptr(m_pkRoomHandler)) {
			m_pkRoomHandler = new CRoomHandlerEx(*this);
		}

		if(false == isptr(m_pkNetIO)) {
			m_pkNetIO = new CNetIO();
		}
		return true;
	}
	return false;
}

bool
CPlayerEx::Release() {
	if(CPlayer::Release()) {
		SAFE_DELETE(m_pkNetIO);
		SAFE_DELETE(m_pkRoomHandler);

		m_iDbIndex = 0;

		return true;
	}
	return false;
}

void
CPlayerEx::Clear() {
	if(0 < GetAid()) {
		if(isptr(m_pkRoomHandler)) {
			CRoom* pkRoom = m_pkRoomHandler->GetRoom();
			if(isptr(pkRoom)) {
				CCommand kCommand;
				kCommand.SetOrder(ROOM_LEAVE);
				kCommand.SetExtra(NONE);

				ServerLauncher(*this, kCommand);
			}
		}

		g_kServerMgr.OutChannel(GetChannelIndex(), *this);
		g_kServerMgr.DecreasedUser();

		TRACE("SERVER: OUT: id: " << g_kChannelMgr.GetServerInfo().GetId() << ", max user: " << g_kChannelMgr.GetServerInfo().GetMaxUser() << ", user: " << g_kChannelMgr.GetServerInfo().GetUser());
	}

	if(isptr(m_pkNetIO)) {
		m_pkNetIO->Clear();
	}

	m_iDbIndex = 0;

	CPlayer::Clear();
}

void
CPlayerEx::Disconnect() {
	CCommand kCommand;
	kCommand.SetOrder(ID_QUIT);
	kCommand.SetExtra(CLOSE);

	Launcher(kCommand);

	CPlayer::Disconnect();
}

bool
CPlayerEx::ReCalculateData() {
	if(CPlayer::ReCalculateData()) {
		return true;
	}
	return false;
}

/* EOF */
