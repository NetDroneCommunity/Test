/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "../CommonHeader.h"

bool
CMD_USER_STATUS(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer =(CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = pkPlayer->GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				CRoom* pkRoom = pkRoomHandler->GetRoom();
				if(isptr(pkRoom)) {
					pkPlayer->SetStatus((STATUS_TYPE)(rkCommand_.GetOption()));

					rkCommand_.SetExtra(OK);
					pkPlayer->Launcher(rkCommand_);

					TRACE("OK: key: " << pkPlayer->GetKey() << ", aid: " << pkPlayer->GetAid() << ", room id: " << pkRoom->GetId() << ", status: " << pkPlayer->GetStatus());

					rkCommand_.SetOrder(OTHER_STATUS);
					SOtherStatusGsToCl& rtSData = (SOtherStatusGsToCl&)rkCommand_.GetData();
					rtSData.actor = pkPlayer->GetKey();

					pkRoom->Broadcast(rkCommand_, sizeof(SOtherStatusGsToCl), pkPlayer);
					return true;
				} else {
					OUTPUT("["<<g_kTick.GetTime()<<":"<<rkActor_.GetAid()<<"] room is null: "<<rkActor_.GetKey());
					rkCommand_.SetExtra(FAIL);
				}
			} else {
				OUTPUT("["<<g_kTick.GetTime()<<":"<<rkActor_.GetAid()<<"] room handler is null: "<<rkActor_.GetKey());
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
CMD_USER_CHAT(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			SUserChatClToGs& rtRData = (SUserChatClToGs&)rkCommand_.GetData();
			rtRData.content[rkCommand_.GetOption()] = '\0';

			CCommand kCommand;
			kCommand.SetOrder(OTHER_CHAT);
			kCommand.SetExtra(rkCommand_.GetExtra());
			kCommand.SetOption(rkCommand_.GetOption());

			SOtherChatGsToCl& rtSData = (SOtherChatGsToCl&)kCommand.GetData();
			rtSData.SetName(*pkPlayer->GetName());
			rtSData.SetContent(*rtRData.content);

			TRACE("channel: " << (pkPlayer->GetChannelIndex()+1) << ", name: " << pkPlayer->GetName() << ", message: " << rtSData.content);
			//TRACE("bytes: " << (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + (sizeof(SOtherChatGsToCl) - (iMAX_CHAT_LEN+1)) + kCommand.GetOption()));

			FILELOG(LE_CHAT_CHANNEL << LF_AID << pkPlayer->GetAid() << LF_LOGIN_ID << pkPlayer->GetLoginId() << LF_CHANNEL_ID << (pkPlayer->GetChannelIndex()+1) << LF_STRING << rtSData.content);

			g_kChannelMgr.BroadcastChannel((UINT)pkPlayer->GetChannelIndex(), kCommand, sizeof(SOtherChatGsToCl) - (iMAX_CHAT_LEN+1) + kCommand.GetOption());
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

void
InitializeUserCommand() {
	g_pbfNativeLauncher[USER_STATUS] = CMD_USER_STATUS;
	g_pbfNativeLauncher[USER_CHAT] = CMD_USER_CHAT;
}

/* EOF */
