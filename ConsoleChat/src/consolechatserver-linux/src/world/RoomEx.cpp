/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "RoomEx.h"

#include "../CommonHeader.h"

bool
CRoomEx::Initialize() {
	if(CRoom::Initialize()) {
		return true;
	}
	return false;
}

bool
CRoomEx::Release() {
	if(CRoom::Release()) {
		return true;
	}
	return false;
}

void
CRoomEx::Clear() {
	CRoom::Clear();
}

bool
CRoomEx::Update() {
	return false;
}

bool
CRoomEx::Create(CUnit& rkUnit_) {
	CRoomHandler* pkRoomHandler = rkUnit_.GetRoomHandler();
	if(isptr(pkRoomHandler)) {
		if(NULL == pkRoomHandler->GetRoom()) {
			m_pkMembers[0] = (CPlayer*)&rkUnit_;

			pkRoomHandler->SetRoom(this);
			pkRoomHandler->SetOffset(0);
			rkUnit_.SetStatus(STATUS_READY);

			IncreasedTopCount();

			CCommand kCommand;
			kCommand.SetOrder(ROOM_INFO);
			kCommand.SetExtra(IN);

			SRoomInfo& rtOtherData = (SRoomInfo&)kCommand.GetData();
			rtOtherData.id = GetId();
			rtOtherData.max = GetMaxUser();
			rtOtherData.stage_id = GetStageId();
			if(IsDoing()) {
				rtOtherData.offset = 1;
			} else {
				rtOtherData.offset = 0;
			}
			rtOtherData.SetName(*GetName());

			SRoomKey tKey(GetId());
			g_kChannelMgr.BroadcastChannel(tKey.channel, kCommand, sizeof(SRoomInfo), (CPlayer*)&rkUnit_);

			return true;
		} else {
			TRACE("critical error: in room");
		}
	} else {
		OUTPUT("critical error: handler is null: ");
	}
	return false;
}

bool
CRoomEx::Join(CUnit& rkUnit_) {
	CCommand kCommand;
	kCommand.SetOrder(ROOM_JOIN);

	if(0 < GetTopCount() && (GetTopCount() < GetMaxUser())) {
		CRoomHandler* pkRoomHandler = rkUnit_.GetRoomHandler();
		if(isptr(pkRoomHandler)) {
			if(NULL == pkRoomHandler->GetRoom()) {
				SRoomKey tKey(GetId());
				CPlayer* pkPlayer = (CPlayer*)&rkUnit_;
				if(isptr(pkPlayer)) {
					if(tKey.channel != pkPlayer->GetChannelIndex()) {
						g_kChannelMgr.InUser(tKey.channel, *pkPlayer);
					}
				}

				pkRoomHandler->SetRoom(this);
				pkRoomHandler->SetOffset(GetTopCount());
				// Join 한 캐릭터 정보를 클라이언트에 보냅니다.
				if(pkRoomHandler->Join()) {
					m_pkMembers[GetTopCount()] = (CPlayer*)&rkUnit_;
					IncreasedTopCount();

					TRACE("Join OK: ["<<rkUnit_.GetAid()<<" ("<<rkUnit_.GetKey()<<")] : "<<GetTopCount()<<" < " << iMAX_ROOM_MEMBERS);

					kCommand.SetExtra(OK);
					SRoomJoinGsToCl& rtSData = (SRoomJoinGsToCl&)kCommand.GetData();
					rtSData.id = GetId();

					rtSData.max = GetMaxUser();
					rtSData.stage_id = GetStageId();
					if(IsDoing()) {
						rtSData.offset = 1;
					} else {
						rtSData.offset = 0;
					}
					rtSData.SetName(*GetName());

					rkUnit_.Launcher(kCommand, sizeof(SRoomJoinGsToCl));

					return true;
				} else {
					pkRoomHandler->Clear();
					kCommand.SetExtra(DENY);
					TRACE("critical error: in room");
				}
			} else {
				kCommand.SetExtra(DENY);
				TRACE("critical error: in room");
			}
		} else {
			TRACE("critical error: RoomHandler is null: ");
			kCommand.SetExtra(FAIL);
		}
	} else {
		TRACE("error: room is full. or empty: "<<GetTopCount()<<"["<< iMAX_ROOM_MEMBERS<<"] "<<rkUnit_.GetKey());
		kCommand.SetExtra(FULL);
	}
	rkUnit_.Launcher(kCommand);

	return false;
}

bool
CRoomEx::Leave(CUnit& rkUnit_) {
	CRoomHandler* pkRoomHandler = rkUnit_.GetRoomHandler();
	if(isptr(pkRoomHandler)) {
		UINT uiOutIndex = pkRoomHandler->GetOffset();
		if(InRange(uiOutIndex)) {
			if(&rkUnit_ == m_pkMembers[uiOutIndex]) {
				DecreasedTopCount();

				if(0 >= GetTopCount()) {
					TRACE("room clear: ");
					g_kChannelMgr.DeleteRoom(GetId());
					pkRoomHandler->Clear();

					CCommand kCommand;
					kCommand.SetOrder(ROOM_INFO);
					kCommand.SetExtra(OUT);

					SRoomInfo& rtOtherData = (SRoomInfo&)kCommand.GetData();
					rtOtherData.id = GetId();

					SRoomKey tKey(GetId());
					g_kChannelMgr.BroadcastChannel(tKey.channel, kCommand, sizeof(UINT32), (CPlayer*)&rkUnit_);
				} else {
					if(uiOutIndex == GetTopCount()) {
						m_pkMembers[GetTopCount()] = NULL;
						pkRoomHandler->Leave();
						TRACE("out unit index: "<<GetTopCount());
					} else {
						CRoomHandler* pkNewRoomHandler = NULL;
						if(&rkUnit_ == m_pkMembers[0]) {
							// 방장이 나갈 경우.
							// 응답 지연시간 비교로 가장빠른 맴버를 방장으로 정하는 코드를 넣으려면 여기서합니다.
							// 아래는 무조건 다음 맴버로 고정하거나 종료처리됩니다.
							UINT uiIndex = 1;

							if(3 > GetTopCount()) {
								if(2 != GetTopCount()) {
									uiIndex = 0;
								}
							}

							if((0 < uiIndex) && (uiIndex <= GetTopCount())) {
								m_pkMembers[0] = m_pkMembers[uiIndex];
								m_pkMembers[uiIndex] = NULL;
								pkNewRoomHandler = m_pkMembers[0]->GetRoomHandler();
								if(isptr(pkNewRoomHandler)) {
									pkNewRoomHandler->SetOffset(0);
								}

								if(uiIndex < GetTopCount()) {
									m_pkMembers[uiIndex] = m_pkMembers[GetTopCount()];
									m_pkMembers[GetTopCount()] = NULL;
									pkNewRoomHandler = m_pkMembers[uiIndex]->GetRoomHandler();
									if(isptr(pkNewRoomHandler)) {
										pkNewRoomHandler->SetOffset(uiIndex);
									}
								}

								TRACE("Leave OK: ");
								TRACE("leader: out unit: name: " << rkUnit_.GetName());
								TRACE("leader: in unit: name: " << m_pkMembers[0]->GetName() << " (" <<uiIndex << " -> 0)");
								TRACE("leader: move unit: name: " << m_pkMembers[uiIndex]->GetName() << " (" << GetTopCount() << " -> " << uiIndex << ")");

								// 빠져나간 유닛과 새로운 방장을 함께 전달할경우 각 클라이언트들은 동일한 갱신이 가능합니다.
								pkRoomHandler->Leave(m_pkMembers[0]);
							} else {
								// 방안에 아무도 없음.

								m_pkMembers[0] = m_pkMembers[GetTopCount()];
								m_pkMembers[GetTopCount()] = NULL;

								pkNewRoomHandler = m_pkMembers[0]->GetRoomHandler();
								if(isptr(pkNewRoomHandler)) {
									pkNewRoomHandler->SetOffset(0);
								}

								TRACE("Leave OK: ");
								pkRoomHandler->Leave(m_pkMembers[0]);
							}
							return true;
						}

						TRACE("switch a member position in room: "<<uiOutIndex<<" <-> "<<GetTopCount());
						m_pkMembers[uiOutIndex] = m_pkMembers[GetTopCount()];
						m_pkMembers[GetTopCount()] = NULL;
						pkNewRoomHandler = m_pkMembers[uiOutIndex]->GetRoomHandler();
						if(isptr(pkNewRoomHandler)) {
							pkNewRoomHandler->SetOffset(uiOutIndex);
						}
						pkRoomHandler->Leave();
					}
				}
				return true;
			}
		} else {
			OUTPUT("critical error: offset is error: ");
		}
	}
	return false;
}

bool
CRoomEx::ChangeLeader(CUnit& rkUnit_) {
	CRoomHandler* pkRoomHandler = rkUnit_.GetRoomHandler();
	if(isptr(pkRoomHandler)) {
		UINT uiIndex = (UINT)pkRoomHandler->GetOffset();
		if(InRange(uiIndex)) {
			if(&rkUnit_ == m_pkMembers[uiIndex]) {
				if((0 < uiIndex) && (uiIndex <= GetTopCount())) {
					CPlayer* pkLeader = m_pkMembers[uiIndex];
					if(isptr(pkLeader)) {
						m_pkMembers[uiIndex] = m_pkMembers[0];
						m_pkMembers[0] = pkLeader;
						CRoomHandler* pkNewRoomHandler = m_pkMembers[0]->GetRoomHandler();
						if(isptr(pkNewRoomHandler)) {
							pkNewRoomHandler->SetOffset(0);
						}
						CRoomHandler* pkOldRoomHandler = m_pkMembers[uiIndex]->GetRoomHandler();
						if(isptr(pkOldRoomHandler)) {
							pkOldRoomHandler->SetOffset((INT)uiIndex);
						}

						TRACE("Change OK: ");
						TRACE("leader: change unit: name: " << m_pkMembers[0]->GetName() << " (" << uiIndex << " -> 0)");

						pkRoomHandler->ChangeLeader();
					}
				}
				return true;
			}
		} else {
			OUTPUT("critical error: offset is error: ");
		}
	}
	return false;
}

bool
CRoomEx::Broadcast(CCommand& rkCommand_, INT iSize_, CUnit* pkActor_, CUnit* pkTarget_) {
	if(0 < GetTopCount()) {
		CUnit* pkUnit = NULL;
		for(UINT i=0; i<GetTopCount(); ++i) {
			pkUnit = m_pkMembers[i];
			if(isptr(pkUnit)) {
				if((pkUnit != pkActor_) && (pkUnit != pkTarget_)) {
					pkUnit->Launcher(rkCommand_, iSize_);
				}
			} else {
				OUTPUT("critical error: unit["<<i<<"] is null: "<<GetTopCount());
			}
		}
		return true;
	} else {
		OUTPUT("room is empty");
	}
	return false;
}

/* EOF */
