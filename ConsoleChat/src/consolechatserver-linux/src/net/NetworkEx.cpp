/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "NetworkEx.h"

#include "../CommonHeader.h"

bool
CMasterNetwork::ParseCommand(CExtendCommand& rkCommand_) {
	CCommand& rkCommand = rkCommand_.GetCommand();
	CConnector* pkConnector = rkCommand_.GetConnector();

	if((PROTOCOL_COMMON_BEGIN < rkCommand.GetOrder()) && (rkCommand.GetOrder() < PROTOCOL_COMMON_END)) {
		NativeLauncher pkNativeLauncher = g_pbfNativeLauncher[rkCommand.GetOrder()];
		if(isptr(pkNativeLauncher)) {
			if(isptr(pkConnector)) {
				CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
				if(isptr(pkActor)) {
					if(0 < pkActor->GetAid()) {
						return pkNativeLauncher(*pkActor, rkCommand);
					}
				} else {
					TRACE("[" << rkCommand.GetOrder() << "] actor is null: (" << rkCommand_.GetRelay() << ":" << rkCommand_.GetKey() << ")");
				}
			}
		} else {
			ExtendLauncher pkExtendLauncher = g_pbfExtendLauncher[rkCommand.GetOrder()];
			if(isptr(pkExtendLauncher)) {
				return pkExtendLauncher(rkCommand_);
			} else {
				if(isptr(pkConnector)) {
					CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
					if(isptr(pkActor)) {
						if(0 < pkActor->GetAid()) {
							// Hack
							OUTPUT("[" << g_kTick.GetTime() << "] error: order range over [" << rkCommand_.GetOrder() << "]: server attack by protocol hack: " << pkConnector->GetRemoteAddress());
							pkActor->Disconnect();
							return false;
						}
					}
				}
			}
			OUTPUT("[" << g_kTick.GetTime() << "] error: order is none: " << rkCommand.GetOrder());
#ifdef _DEBUG
			CUnit* pkUnit = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
			if(isptr(pkUnit)) {
				TRACE("[" << g_kTick.GetTime() << "] error: [" << rkCommand.GetOrder() << "] order is none: (" << pkUnit->GetAid() << "): " << pkUnit->GetName());
			}
#endif
		}
	} else {
		if(isptr(pkConnector)) {
			CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
			if(isptr(pkActor)) {
				if(0 < pkActor->GetAid()) {
					// Hack
					OUTPUT("[" << g_kTick.GetTime() << "] error: order range over [" << rkCommand.GetOrder() << "]: server attack by protocol hack: " << pkConnector->GetRemoteAddress());
					pkActor->Disconnect();
					return false;
				}
			}
		}
		OUTPUT("[" << g_kTick.GetTime() << "] error: order range over: " << rkCommand.GetOrder());
	}
	return false;
}

bool
CSlaveNetwork::ParseCommand(CExtendCommand& rkCommand_) {
	CCommand& rkCommand = rkCommand_.GetCommand();
	CConnector* pkConnector = rkCommand_.GetConnector();

	if((PROTOCOL_COMMON_BEGIN < rkCommand.GetOrder()) && (rkCommand.GetOrder() < PROTOCOL_COMMON_END)) {
		NativeLauncher pkNativeLauncher = g_pbfNativeLauncher[rkCommand.GetOrder()];
		if(isptr(pkNativeLauncher)) {
			if(isptr(pkConnector)) {
				CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
				if(isptr(pkActor)) {
					if(0 < pkActor->GetAid()) {
						return pkNativeLauncher(*pkActor, rkCommand);
					}
				} else {
					TRACE("[" << rkCommand.GetOrder() << "] actor is null: (" << rkCommand_.GetRelay() << ":" << rkCommand_.GetKey() << ")");
				}
			}
		} else {
			ExtendLauncher pkExtendLauncher = g_pbfExtendLauncher[rkCommand.GetOrder()];
			if(isptr(pkExtendLauncher)) {
				return pkExtendLauncher(rkCommand_);
			} else {
				if(isptr(pkConnector)) {
					CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
					if(isptr(pkActor)) {
						if(0 < pkActor->GetAid()) {
							// Hack
							OUTPUT("[" << g_kTick.GetTime() << "] error: order range over [" << rkCommand_.GetOrder() << "]: server attack by protocol hack: " << pkConnector->GetRemoteAddress());
							pkActor->Disconnect();
							return false;
						}
					}
				}
			}
			OUTPUT("[" << g_kTick.GetTime() << "] error: order is none: " << rkCommand.GetOrder());
#ifdef _DEBUG
			CUnit* pkUnit = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
			if(isptr(pkUnit)) {
				TRACE("[" << g_kTick.GetTime() << "] error: [" << rkCommand.GetOrder() << "] order is none: (" << pkUnit->GetAid() << "): " << pkUnit->GetName());
			}
#endif
		}
	} else {
		if(isptr(pkConnector)) {
			CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
			if(isptr(pkActor)) {
				if(0 < pkActor->GetAid()) {
					// Hack
					OUTPUT("[" << g_kTick.GetTime() << "] error: order range over [" << rkCommand.GetOrder() << "]: server attack by protocol hack: " << pkConnector->GetRemoteAddress());
					pkActor->Disconnect();
					return false;
				}
			}
		}
		OUTPUT("[" << g_kTick.GetTime() << "] error: order range over: " << rkCommand.GetOrder());
	}
	return false;
}

/* EOF */
