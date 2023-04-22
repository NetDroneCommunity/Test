/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "Launcher.h"

#include "../CommonHeader.h"

NativeLauncher g_pbfNativeLauncher[PROTOCOL_MAX] = {0, };
ExtendLauncher g_pbfExtendLauncher[PROTOCOL_MAX] = {0, };

void
ServerLauncher(CUnit& rkActor_, CCommand& rkCommand_) {
	if((0 < rkCommand_.GetOrder()) && (rkCommand_.GetOrder() < PROTOCOL_MAX)) {
		NativeLauncher pkLauncher = g_pbfNativeLauncher[rkCommand_.GetOrder()];
		if(isptr(pkLauncher)) {
			pkLauncher(rkActor_, rkCommand_);
		} else {
			OUTPUT("["<<g_kTick.GetTime()<<"] error: order is none: "<<rkCommand_.GetOrder());
		}
	} else {
		OUTPUT("["<<g_kTick.GetTime()<<"] error: order range over: "<<rkCommand_.GetOrder());
	}
}

void
ServerLauncher(CExtendCommand& rkCommand_) {
	CCommand& rkCommand = rkCommand_.GetCommand();
	if((0 < rkCommand.GetOrder()) && (rkCommand.GetOrder() < PROTOCOL_MAX)) {
		ExtendLauncher pkLauncher = g_pbfExtendLauncher[rkCommand.GetOrder()];
		if(isptr(pkLauncher)) {
			pkLauncher(rkCommand_);
		} else {
			OUTPUT("[" << g_kTick.GetTime() << "] error: order is none: " << rkCommand.GetOrder());
		}
	} else {
		OUTPUT("[" << g_kTick.GetTime() << "] error: order range over: " << rkCommand.GetOrder());
	}
}

void
InitializeCommand() {
	InitializeIdCommand();
	InitializeInfoCommand();
	InitializeUserCommand();
	InitializeRoomCommand();
}

/* EOF */
