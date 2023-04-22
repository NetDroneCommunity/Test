/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "ConnectorEx.h"

#include "../CommonHeader.h"

//
// Default Connector
//
void
CDefaultConnector::SetConnected() {
	CTcpConnector::SetConnected();

	if(INVALID_SOCKET != GetSocket()) {
		SetRelay(RELAY_CLIENT);

		INT iRand = RAND(1000000) + 1;
		SetSerialKey(iRand*999+iRand*99+iRand);

		if(0 == GetSerialKey()) {
			SetSerialKey(1234567890);
		}

		CCommand kCommand;
		kCommand.SetOrder(INFO_SERVER);
		kCommand.SetExtra(NONE);
		Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_NONE);
	}
}

bool
CDefaultConnector::Disconnect(const SOCKET skSocket_) {
	if((INVALID_SOCKET != skSocket_) && (GetSocket() == skSocket_) && IsConnected() && (0 != GetSerialKey())) {
		SetSerialKey(0);

		CCommand kCommand;
		kCommand.SetOrder(ID_QUIT);
		kCommand.SetExtra(REQUEST);
		Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_NONE);
		return true;
	} else {
		SetSerialKey(0);
		if(CTcpConnector::Disconnect(skSocket_)) {
			return true;
		}
	}
	return false;
}

void
CDefaultConnector::Disconnect() {
	TRACE("disconnect: socket: " << GetSocket() << ", serial: " << GetSerialKey());
	SetSerialKey(0);
	CTcpConnector::Disconnect(GetSocket());
}

bool
CDefaultConnector::KeepAlive() {
	if(IsConnected()) {
		if(0 == GetRecvPingTick()) {
			OUTPUT("[" << g_kTick.GetTime() << "] RECV PING: recv tick is 0");
			return false;
		}

		if(g_kTick.GetTick() > GetRecvPingTick() + iRECV_PING_TICK) {
			CPlayerEx* pkActor = g_kUnitMgr.GetPlayer(UNIT_PLAYER, GetRecvKey());
			if(isptr(pkActor)) {
				if(isptr(pkActor->GetNetIO())) {
					if(pkActor->GetNetIO()->GetConnector() != this) {
						if(0 < pkActor->GetAid()) {
							OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: connector is wrong: ["<<pkActor->GetAid()<<":"<<pkActor->GetKey()<<":"<<(INT)pkActor->GetStatus()<<"] delay: "<<(g_kTick.GetTick()-GetRecvPingTick())<<", addr : "<<GetRemoteAddress());
						}
						return false;
					}

					OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: time is too late: ["<<pkActor->GetAid()<<":"<<pkActor->GetKey()<<":"<<(INT)pkActor->GetStatus()<<"] delay: "<<(g_kTick.GetTick()-GetRecvPingTick())<<", addr : "<<GetRemoteAddress());
					return false;
				} else {
					OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: Net IO is null: ["<<pkActor->GetAid()<<":"<<pkActor->GetKey()<<"] : "<<GetRemoteAddress());
					return false;
				}
			} else {
				OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: actor is null: ");
				return false;
			}
		}
	}
	return true;
}

//
// Backbone Connector
//
void
CBackboneConnector::SetConnected() {
	CTcpConnector::SetConnected();

	if(INVALID_SOCKET != GetSocket()) {
		SetRelay(RELAY_CLIENT);

		INT iRand = RAND(1000000) + 1;
		SetSerialKey(iRand*999+iRand*99+iRand);

		if(0 == GetSerialKey()) {
			SetSerialKey(1234567890);
		}

		CCommand kCommand;
		kCommand.SetOrder(INFO_SERVER);
		kCommand.SetExtra(NONE);
		Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_NONE);
	}
}

bool
CBackboneConnector::Disconnect(const SOCKET skSocket_) {
	if((INVALID_SOCKET != skSocket_) && (GetSocket() == skSocket_) && IsConnected() && (0 != GetSerialKey())) {
		SetSerialKey(0);

		CCommand kCommand;
		kCommand.SetOrder(ID_QUIT);
		kCommand.SetExtra(REQUEST);
		Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_NONE);
		return true;
	} else {
		SetSerialKey(0);
		if(CTcpConnector::Disconnect(skSocket_)) {
			return true;
		}
	}
	return false;
}

void
CBackboneConnector::Disconnect() {
	TRACE("disconnect: socket: " << GetSocket() << ", serial: " << GetSerialKey());
	SetSerialKey(0);
	CTcpConnector::Disconnect(GetSocket());
}

bool
CBackboneConnector::KeepAlive() {
	if(IsConnected()) {
		if(0 == GetRecvPingTick()) {
			OUTPUT("[" << g_kTick.GetTime() << "] RECV PING: recv tick is 0");
			return false;
		}

		if(g_kTick.GetTick() > GetRecvPingTick() + iRECV_PING_TICK) {
			CPlayerEx* pkActor = g_kUnitMgr.GetPlayer(UNIT_PLAYER, GetRecvKey());
			if(isptr(pkActor)) {
				if(isptr(pkActor->GetNetIO())) {
					if(pkActor->GetNetIO()->GetConnector() != this) {
						if(0 < pkActor->GetAid()) {
							OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: connector is wrong: ["<<pkActor->GetAid()<<":"<<pkActor->GetKey()<<":"<<(INT)pkActor->GetStatus()<<"] delay: "<<(g_kTick.GetTick()-GetRecvPingTick())<<", addr : "<<GetRemoteAddress());
						}
						return false;
					}

					OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: time is too late: ["<<pkActor->GetAid()<<":"<<pkActor->GetKey()<<":"<<(INT)pkActor->GetStatus()<<"] delay: "<<(g_kTick.GetTick()-GetRecvPingTick())<<", addr : "<<GetRemoteAddress());
					return false;
				} else {
					OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: Net IO is null: ["<<pkActor->GetAid()<<":"<<pkActor->GetKey()<<"] : "<<GetRemoteAddress());
					return false;
				}
			} else {
				OUTPUT("["<<g_kTick.GetTime()<<"] ["<<GetSocket()<<"] RECV PING: actor is null: ");
				return false;
			}
		}
	}
	return true;
}

//
// Reliable Connector
//
void
CReliableConnector::SetConnected() {
	CUdpConnector::SetConnected();

	if(INVALID_SOCKET != GetSocket()) {
		SetRelay(RELAY_CLIENT);

		INT iRand = RAND(1000000) + 1;
		SetSerialKey(iRand*999+iRand*99+iRand);

		if(0 == GetSerialKey()) {
			SetSerialKey(1234567890);
		}

		CCommand kCommand;
		kCommand.SetOrder(INFO_SERVER);
		kCommand.SetExtra(NONE);
		Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_NONE);
	}
}

bool
CReliableConnector::Disconnect(const SOCKET skSocket_) {
	if((INVALID_SOCKET != skSocket_) && (GetSocket() == skSocket_) && IsConnected() && (0 != GetSerialKey())) {
		SetSerialKey(0);

		CCommand kCommand;
		kCommand.SetOrder(ID_QUIT);
		kCommand.SetExtra(REQUEST);
		Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_NONE);
		return true;
	} else {
		SetSerialKey(0);
		if(CUdpConnector::Disconnect(skSocket_)) {
			return true;
		}
	}
	return false;
}

void
CReliableConnector::Disconnect() {
	TRACE("disconnect: socket: " << GetSocket() << ", serial: " << GetSerialKey());
	SetSerialKey(0);
	CUdpConnector::Disconnect(GetSocket());
}

/* EOF */
