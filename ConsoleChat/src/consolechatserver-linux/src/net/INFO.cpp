/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "../CommonHeader.h"

bool
CMD_INFO_USER_LIST(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			g_kChannelMgr.GetUserList(*pkPlayer);
			TRACE("OK: USER LIST: aid: " << pkPlayer->GetAid() << ", key: " << rkActor_.GetKey());
			return true;
		} else {
			OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] aid is 0: ");
			rkCommand_.SetExtra(FAIL);
		}
	} else {
		OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] player is null: ");
		rkCommand_.SetExtra(FAIL);
	}

	rkActor_.Launcher(rkCommand_);
	rkActor_.Disconnect();
	return true;
}

bool
CMD_INFO_CHANNEL(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			if(CHANGE == rkCommand_.GetExtra()) {
				CRoomHandler* pkRoomHandler = pkPlayer->GetRoomHandler();
				if(isptr(pkRoomHandler)) {
					CRoom* pkRoom = pkRoomHandler->GetRoom();
					if(isptr(pkRoom)) {
						CCommand kCommand;
						kCommand.SetOrder(ROOM_LEAVE);
						kCommand.SetExtra(NONE);

						ServerLauncher(rkActor_, kCommand);
					}
				}

#ifdef _DEBUG
				INT iPrevChannelIndex = pkPlayer->GetChannelIndex();
				INT iNextChannelIndex = -1;
#endif

				if(g_kServerMgr.InChannel(rkCommand_.GetOption(), *pkPlayer)) {
#ifdef _DEBUG
					iNextChannelIndex = pkPlayer->GetChannelIndex();
#endif

					rkCommand_.SetExtra(OK);
					pkPlayer->Launcher(rkCommand_);

					g_kChannelMgr.GetUserList(*pkPlayer);

#ifdef _DEBUG
					TRACE("OK: aid: " << pkPlayer->GetAid() << ", key: " << rkActor_.GetKey() << ", name: " << pkPlayer->GetName() << ", channel id: " << (iPrevChannelIndex+1) << " -> " << (iNextChannelIndex+1));
#endif
					return true;
				}

				rkCommand_.SetExtra(DENY);
				rkActor_.Launcher(rkCommand_);
				return true;
			} else {
				OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] unknown extra: " << rkCommand_.GetExtra());
				rkCommand_.SetExtra(FAIL);
			}
		} else {
			OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] aid is 0: ");
			rkCommand_.SetExtra(FAIL);
		}
	} else {
		OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] player is null: ");
		rkCommand_.SetExtra(FAIL);
	}

	rkActor_.Launcher(rkCommand_);
	rkActor_.Disconnect();
	return true;
}

bool
CMD_INFO_SERVER(CExtendCommand& rkCommand_) {
	CCommand& rkSelfCommand = rkCommand_.GetCommand();
	CConnector* pkConnector = rkCommand_.GetConnector();

	if(isptr(pkConnector)) {
		rkSelfCommand.SetExtra(OK);

		SInfoServerGsToCl& rtSData = (SInfoServerGsToCl&)rkSelfCommand.GetData();
		rtSData.serial = pkConnector->GetSerialKey();
		rtSData.key = pkConnector->GetRegisterIndex();

		TRACE("serial: " << rtSData.serial << ", key: " << rtSData.key);

		if(0 > pkConnector->Send(rkSelfCommand, sizeof(SInfoServerGsToCl))) {
			TRACE("FAILED: " << pkConnector->GetRemoteAddress() << " : " << pkConnector->GetRemotePort());
		} else {
			TRACE("OK: " << pkConnector->GetRemoteAddress() << " : " << pkConnector->GetRemotePort());
		}
	}

	return true;
}

void
InitializeInfoCommand() {
	g_pbfNativeLauncher[INFO_USER_LIST] = CMD_INFO_USER_LIST;
	g_pbfNativeLauncher[INFO_CHANNEL] = CMD_INFO_CHANNEL;

	g_pbfExtendLauncher[INFO_SERVER] = CMD_INFO_SERVER;
}

/* EOF */
