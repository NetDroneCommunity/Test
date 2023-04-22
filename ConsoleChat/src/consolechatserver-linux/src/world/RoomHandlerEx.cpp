/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "RoomHandlerEx.h"

#include "../CommonHeader.h"

bool
CRoomHandlerEx::Join() {
	CPlayerEx* pkBody = (CPlayerEx*)GetBody().GetTypeAs(UNIT_PLAYER);
	if(isptr(pkBody)) {
		CRoom* pkRoom = GetRoom();
		if(isptr(pkRoom)) {
			TRACE("in: ["<<pkBody->GetAid()<<":"<<pkBody->GetKey()<<"]");
			pkBody->SetStatus(STATUS_WAITING);

			g_kChannelMgr.GetRoomMemberList(*pkBody);

			CConnector* pkConnector = pkBody->GetNetIO()->GetConnector();
			if(isptr(pkConnector)) {
				CCommand kCommand;
				kCommand.SetOrder(ROOM_JOIN_OTHER);
				kCommand.SetExtra(OK);

				SRoomJoinOtherGsToCl& rtSData = (SRoomJoinOtherGsToCl&)kCommand.GetData();
				rtSData.actor = pkBody->GetKey();
				rtSData.SetName(*pkBody->GetName());

				rtSData.public_ip = pkConnector->GetPublicSinAddress();
				rtSData.public_port = pkConnector->GetPublicPort();
				rtSData.local_ip = pkConnector->GetLocalSinAddress();
				rtSData.local_port = pkConnector->GetLocalPort();

				TRACE("["<<pkBody->GetName()<<":"<<pkBody->GetKey()<<"] : public addr: "<<pkConnector->GetPublicAddress()<<":"<<pkConnector->GetPublicPort()<<", local addr: "<<pkConnector->GetLocalAddress()<<":"<<pkConnector->GetLocalPort());

				Broadcast(kCommand, sizeof(SRoomJoinOtherGsToCl), pkBody);
			}

			return true;
		} else {
			OUTPUT("critical error: room is null: ");
		}
	} else {
		OUTPUT("critical error: unit is not player: ");
	}
	return false;
}

void
CRoomHandlerEx::Leave(CUnit* pkLeader_) {
	CCommand kCommand;
	kCommand.SetOrder(ROOM_LEAVE_OTHER);
	GetBody().SetStatus(STATUS_EXIT);

	SRoomLeaveOtherGsToCl&	rtSData = (SRoomLeaveOtherGsToCl&)kCommand.GetData();
	rtSData.actor = GetBody().GetKey();

	if(isptr(pkLeader_)) {
		kCommand.SetExtra(CHECK);
		pkLeader_->SetStatus(STATUS_READY);

		rtSData.leader = pkLeader_->GetKey();

		TRACE("ROOM_LEAVE_OTHER: CHECK: body: " << GetBody().GetKey() << ", leader: " << pkLeader_->GetKey());

		Broadcast(kCommand, sizeof(SRoomLeaveOtherGsToCl), &GetBody());
	} else {
		kCommand.SetExtra(OK);

		TRACE("ROOM_LEAVE_OTHER: OK: body: " << GetBody().GetKey());

		Broadcast(kCommand, (sizeof(SRoomLeaveOtherGsToCl) - sizeof(UINT32)), &GetBody());
	}

	Clear();
}

void
CRoomHandlerEx::ChangeLeader() {
	CCommand kCommand;
	kCommand.SetOrder(ROOM_INFO);

	SRoomMemberLeaderGsToCl& rtSData = (SRoomMemberLeaderGsToCl&)kCommand.GetData();
	rtSData.leader = GetBody().GetKey();

	kCommand.SetExtra(MODIFY);

	TRACE("ROOM_INFO: MODIFY: leader: " << GetBody().GetKey());

	Broadcast(kCommand, sizeof(SRoomMemberLeaderGsToCl));
}

void
CRoomHandlerEx::Clear() {
	SetRoom(NULL);
	SetOffset(-1);

	GetBody().SetStatus(STATUS_EXIT);
}

bool
CRoomHandlerEx::Broadcast(CCommand& rkCommand_, INT iSize_, CUnit* pkActor_, CUnit* pkTarget_) {
	if(isptr(GetRoom())) {
		return GetRoom()->Broadcast(rkCommand_, iSize_, pkActor_, pkTarget_);
	}
	return false;
}

/* EOF */
