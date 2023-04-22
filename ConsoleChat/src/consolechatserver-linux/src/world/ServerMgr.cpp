/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "../CommonHeader.h"
#include "ServerMgr.h"

template<> CServerMgr* CSingleton< CServerMgr >::ms_Instance = NULL;

bool
CServerMgr::Initialize() {
	return true;
}

bool
CServerMgr::Release() {
	return true;
}

void
CServerMgr::Update() {
	//
}

void
CServerMgr::IncreasedUser() {
	g_kChannelMgr.GetServerInfo().IncreasedUser();
}

void
CServerMgr::DecreasedUser() {
	g_kChannelMgr.GetServerInfo().DecreasedUser();
}

bool
CServerMgr::InChannel(INT iIndex_, CPlayer& rkPlayer_) {
	if(g_kChannelMgr.InUser(iIndex_, rkPlayer_)) {
		return true;
	}
	return false;
}

void
CServerMgr::OutChannel(INT iIndex_, CPlayer& rkPlayer_) {
	g_kChannelMgr.OutUser(iIndex_, rkPlayer_);
}

bool
CServerMgr::IsAvailable() {
	return false;
}

void
CServerMgr::Shutdown() {
}

/* EOF */
