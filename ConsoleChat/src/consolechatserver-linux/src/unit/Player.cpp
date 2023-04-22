/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "Player.h"

#include "../CommonHeader.h"

CPlayer::CPlayer() : m_iUserIndex(-1), m_iChannelIndex(-1) {
	memset(m_szLoginId, 0, sizeof(m_szLoginId));
}

bool
CPlayer::Initialize() {
	if(false == isptr(m_ptPlayerData)) {
		m_ptPlayerData = new SPlayerData;
	}

	if(false == isptr(m_ptPlayerSums)) {
		m_ptPlayerSums = new SPlayerData;
	}

	GetPlayerData().Clear();
	GetPlayerSums().Clear();

	return true;
}

bool
CPlayer::Release() {
	Disconnect();

	SAFE_DELETE(m_ptPlayerSums);
	SAFE_DELETE(m_ptPlayerData);

	return true;
}

void
CPlayer::Clear() {
	CUnit::Clear();

	GetPlayerData().Clear();
	GetPlayerSums().Clear();

	m_iChannelIndex = -1;
	m_iUserIndex = -1;

	memset(m_szLoginId, 0, sizeof(m_szLoginId));
}

void
CPlayer::Disconnect() {
	Clear();
}

CUnit*
CPlayer::GetTypeAs(INT o) {
	if(isptr(CUnit::GetTypeAs(o)) == false) {
		if(UNIT_PLAYER == o) {
			return this;
		}
		return NULL;
	}
	return this;
}

bool
CPlayer::ReCalculateData() {
	return true;
}

/* EOF */
