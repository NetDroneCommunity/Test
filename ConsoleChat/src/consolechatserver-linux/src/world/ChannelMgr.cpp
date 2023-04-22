/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "ChannelMgr.h"

#include "../CommonHeader.h"

template<> CChannelMgr* CSingleton< CChannelMgr >::ms_Instance = NULL;

bool
CChannelMgr::Initialize() {
	for(UINT i = 0; i < (UINT)(iMAX_CHANNEL_LIST); ++i) {
		for(UINT j = 0; j < (UINT)(iMAX_ROOMS);++j) {
			m_iEmptyRoomList[i][j] = -1;
		}
	}

	for(UINT i = 0; i < (UINT)(iMAX_CHANNEL_LIST); ++i) {
		for(UINT j = 0; j < (UINT)(iMAX_ROOMS); ++j) {
			m_iEmptyRoomList[i][j] = -1;
			m_pkRoomList[i][j] = (CRoom*)g_kRoomProvider.Create();
			if(isptr(m_pkRoomList[i][j])) {
				if(m_pkRoomList[i][j]->Initialize()) {
					m_pkRoomList[i][j]->SetId(GenerateRoomKey(i,j));
				} else {
					OUTPUT("["<<i<<"]["<<j<<"] critical error: room created: failed: system memory is not enough");
					for(UINT k=0; k<=i; ++k) {
						for(UINT l=0; l<j; ++l) {
							SAFE_DELETE_RELEASE(m_pkRoomList[k][l]);
						}
					}
					return false;
				}
			} else {
				OUTPUT("["<<i<<"]["<<j<<"] critical error: room created failed: system memory is not enough");
				for(UINT k=0; k<=i; ++k) {
					for(UINT l=0; l<j; ++l) {
						SAFE_DELETE_RELEASE(m_pkRoomList[k][l]);
					}
				}
				return false;
			}
		}
	}

	return true;
}

bool
CChannelMgr::Release() {
	for(UINT i = 0; i < (UINT)(iMAX_CHANNEL_LIST); ++i) {
		for(UINT j = 0; j < (UINT)(iMAX_ROOMS); ++j) {
			m_iEmptyRoomList[i][j] = -1;
			SAFE_DELETE_RELEASE(m_pkRoomList[i][j]);
		}
	}
	return true;
}

CRoom*
CChannelMgr::NewRoom(INT o) {
	if((0 <= o) && (o < INT(GetMaxChannel()))) {
		if(0 < m_iEmptyRoomCount[o]) {
			--m_iEmptyRoomCount[o];
			INT iIndex = m_iEmptyRoomList[o][m_iEmptyRoomCount[o]];
			m_iEmptyRoomList[o][m_iEmptyRoomCount[o]] = -1;

			if((0 <= iIndex) && (m_iRoomTopIndex[o] > iIndex)) {
				++m_iUsedRoomCount[o];
				TRACE("new room["<<iIndex<<"]: OK: count: "<<m_iUsedRoomCount[o]<<", id: "<<m_pkRoomList[o][iIndex]->GetId()<<" : "<<m_pkRoomList[o][iIndex]->GetTopCount());
				return m_pkRoomList[o][iIndex];
			}
		}

		if((0 <= m_iRoomTopIndex[o]) && (iMAX_ROOMS > m_iRoomTopIndex[o])) {
			INT iIndex = m_iRoomTopIndex[o];

			++m_iRoomTopIndex[o];
			++m_iUsedRoomCount[o];

			return m_pkRoomList[o][iIndex];
		} else {
			OUTPUT("critical error: room is full: U: "<<m_iUsedRoomCount[o]<<" - T: "<<m_iRoomTopIndex[o]<<" - E: "<<m_iEmptyRoomCount[o]);
		}
	} else {
		OUTPUT("critical error: channel info of the user is wrong: ("<<o<<")");
	}
	return NULL;
}

bool
CChannelMgr::DeleteRoom(UINT16 o) {
	SRoomKey tKey(o);

	if(tKey.channel < GetMaxChannel()) {
		if(tKey.index < m_iRoomTopIndex[tKey.channel]) {
			if(tKey.index == UINT16(m_iRoomTopIndex[tKey.channel]-1)) {
				--m_iRoomTopIndex[tKey.channel];
			} else {
				m_iEmptyRoomList[tKey.channel][m_iEmptyRoomCount[tKey.channel]] = tKey.index;
				++m_iEmptyRoomCount[tKey.channel];
			}

			--m_iUsedRoomCount[tKey.channel];
			TRACE("room is deleted: OK: ");

			m_pkRoomList[tKey.channel][tKey.index]->Clear();
			return true;
		} else {
			OUTPUT("room delete: failed: room index is wrong: ("<<tKey.index<<") - U: "<<m_iUsedRoomCount[tKey.channel]<<" - T: "<<m_iRoomTopIndex[tKey.channel]<<" - E: "<<m_iEmptyRoomCount[tKey.channel]);
		}
	} else {
		OUTPUT("room delete: failed: room index is wrong: ("<<tKey.channel<<":"<<tKey.index<<")");
	}
	return false;
}

CRoom*
CChannelMgr::FindRoom(UINT16 usId_) {
	if(0 < usId_) {
		SRoomKey tKey(usId_);

		if(tKey.channel < GetMaxChannel()) {
			if(tKey.index < iMAX_ROOMS) {
				if(isptr(m_pkRoomList[tKey.channel][tKey.index])) {
					return m_pkRoomList[tKey.channel][tKey.index];
				} else {
					TRACE("error: room is null: id: " << usId_ << ", channel: " << tKey.channel << ", index: " << tKey.index);
				}
			} else {
				TRACE("error: invalid condition: id: " << usId_ << ", channel: " << tKey.channel << ", index: " << tKey.index);
			}
		} else {
			TRACE("critical error: range over");
		}
	}
	return NULL;
}

bool
CChannelMgr::InUser(INT o, CPlayer& rkPlayer_) {
	if(o == rkPlayer_.GetChannelIndex()) {
		return true;
	} else {
		if(o < GetMaxChannel()) {
			if(GetChannelInfo(o).GetUser() < GetChannelInfo(o).GetMaxUser()) {
				if(0 <= rkPlayer_.GetChannelIndex()) {
					g_kServerMgr.OutChannel(rkPlayer_.GetChannelIndex(), rkPlayer_);
				}

				if(0 < m_iEmptyUserCount[o]) {
					if(0 <= m_iEmptyUserList[o][m_iEmptyUserCount[o]-1]) {
						m_pkUserList[o][m_iEmptyUserList[o][m_iEmptyUserCount[o]-1]] = &rkPlayer_;
						rkPlayer_.SetUserIndex(m_iEmptyUserList[o][m_iEmptyUserCount[o]-1]);
						m_iEmptyUserList[o][m_iEmptyUserCount[o]-1] = -1;
						--m_iEmptyUserCount[o];
					} else {
						OUTPUT("critical error: index is wrong: ");
						return false;
					}
				} else {
					m_pkUserList[o][m_iUserTopIndex[o]] = &rkPlayer_;
					rkPlayer_.SetUserIndex(m_iUserTopIndex[o]);
					++m_iUserTopIndex[o];
				}
				rkPlayer_.SetChannelIndex(o);
				GetChannelInfo(o).IncreasedUser();

				CCommand kCommand;
				kCommand.SetOrder(INFO_CHANNEL);
				kCommand.SetExtra(IN);
				kCommand.SetOption(o);

				SInfoChannelInGsToCl& rtData = (SInfoChannelInGsToCl&)kCommand.GetData();
				rtData.aid = rkPlayer_.GetAid();
				rtData.key = rkPlayer_.GetKey();
				rtData.SetName(*((CPlayerEx&)rkPlayer_).GetName());

				BroadcastChannel(o, kCommand, sizeof(SInfoChannelInGsToCl), &rkPlayer_);

				TRACE("CHANNEL: IN: aid: " << rkPlayer_.GetAid() << ", key: " << rkPlayer_.GetKey() << ", channel index: " << rkPlayer_.GetChannelIndex() << ", user index: " << rkPlayer_.GetUserIndex() << ", max user: " << GetChannelInfo(o).GetMaxUser() << ", user: " << GetChannelInfo(o).GetUser());

				FILELOG(LE_CLIENT_IN << LF_AID << rkPlayer_.GetAid() << LF_LOGIN_ID << rkPlayer_.GetLoginId());

				return true;
			} else {
				TRACE("CHANNEL: FULL: aid: " << rkPlayer_.GetAid() << ", key: " << rkPlayer_.GetKey() << ", max user: " << GetChannelInfo(o).GetMaxUser()<<", user: " << GetChannelInfo(o).GetUser());
			}
		} else {
			OUTPUT("critical error: wrong channel: [" << o << " : " << GetMaxChannel() <<"]");
		}
	}
	return false;
}

void
CChannelMgr::OutUser(INT o, CPlayer& rkPlayer_) {
	if(o < GetMaxChannel()) {
		if(0 < GetChannelInfo(o).GetUser()) {
			INT iIndex = rkPlayer_.GetUserIndex();
			if((0 <= iIndex) && (iIndex < INT(GetChannelInfo(o).GetMaxUser()))) {
				m_pkUserList[o][iIndex] = NULL;
				GetChannelInfo(o).DecreasedUser();

				rkPlayer_.SetChannelIndex(-1);
				rkPlayer_.SetUserIndex(-1);

				if(iIndex == m_iUserTopIndex[o]-1) {
					--m_iUserTopIndex[o];
				} else {
					m_iEmptyUserList[o][m_iEmptyUserCount[o]] = iIndex;
					++m_iEmptyUserCount[o];
				}

				CCommand kCommand;
				kCommand.SetOrder(INFO_CHANNEL);
				kCommand.SetExtra(OUT);
				kCommand.SetOption(o);

				SInfoChannelOutGsToCl& rtData = (SInfoChannelOutGsToCl&)kCommand.GetData();
				rtData.key = rkPlayer_.GetKey();

				BroadcastChannel(o, kCommand, sizeof(SInfoChannelOutGsToCl), &rkPlayer_);

				TRACE("CHANNEL: OUT: aid: " << rkPlayer_.GetAid() << ", key: " << rkPlayer_.GetKey() << ", channel index: " << o << ", user index: " << iIndex << ", max user: " << GetChannelInfo(o).GetMaxUser() << ", user: " << GetChannelInfo(o).GetUser());

				FILELOG(LE_CLIENT_OUT << LF_AID << rkPlayer_.GetAid() << LF_LOGIN_ID << rkPlayer_.GetLoginId());
			} else {
				OUTPUT("critical error: index is wrong: "<<rkPlayer_.GetKey()<<" : "<<rkPlayer_.GetChannelIndex());
			}
		} else {
			OUTPUT("critical error: user count error: "<<rkPlayer_.GetKey()<<" : "<<rkPlayer_.GetChannelIndex());
		}
	}
}

void
CChannelMgr::GetRoomList(CPlayer& rkPlayer_) {
	if((0 <= rkPlayer_.GetChannelIndex()) && (rkPlayer_.GetChannelIndex() < GetMaxChannel())) {
		CCommand kCommand;
		kCommand.SetOrder(ROOM_LIST);
		kCommand.SetMission(0);
		SRoomListGsToCl& rtSData = (SRoomListGsToCl&)kCommand.GetData();

		kCommand.SetExtra(NEW);
		INT iCount = 0;
		for(INT i=0; i<m_iRoomTopIndex[rkPlayer_.GetChannelIndex()]; ++i) {
			if(m_pkRoomList[rkPlayer_.GetChannelIndex()][i]->IsAvailable()) {
				if(iCount >= iMAX_PACKET_ROOM_LIST) {
					TRACE("iMAX_PACKET_ROOM_LIST: count: " << iMAX_PACKET_ROOM_LIST << ", overflow: " << iCount);
					kCommand.SetOption(iMAX_PACKET_ROOM_LIST);
					rkPlayer_.Launcher(kCommand, sizeof(SRoomListGsToCl));
					iCount = 0;
					kCommand.SetExtra(CHANGE);
				}

				rtSData.list[iCount].id = m_pkRoomList[rkPlayer_.GetChannelIndex()][i]->GetId();
				rtSData.list[iCount].stage_id = m_pkRoomList[rkPlayer_.GetChannelIndex()][i]->GetStageId();
				rtSData.list[iCount].max = m_pkRoomList[rkPlayer_.GetChannelIndex()][i]->GetMaxUser();

				rtSData.list[iCount].SetName(*m_pkRoomList[rkPlayer_.GetChannelIndex()][i]->GetName());

				if(m_pkRoomList[rkPlayer_.GetChannelIndex()][i]->IsDoing()) {
					rtSData.list[iCount].offset = 1;
				} else {
					rtSData.list[iCount].offset = 0;
				}

				++iCount;
			}
		}

		if(0 < iCount) {
			kCommand.SetOption(iCount);
			rkPlayer_.Launcher(kCommand, sizeof(SRoomInfo) * iCount);
			TRACE("NEW or CHANGE: ");

			kCommand.SetExtra(DONE);
			kCommand.SetOption(0);
			rkPlayer_.Launcher(kCommand);
		} else {
			if(NEW == kCommand.GetExtra()) {
				kCommand.SetExtra(EMPTY);
				kCommand.SetOption(0);
				rkPlayer_.Launcher(kCommand);
			}
		}
	}
}

bool
CChannelMgr::GetRoomMemberList(CPlayer& rkPlayer_) {
	if((0 <= rkPlayer_.GetChannelIndex()) && (rkPlayer_.GetChannelIndex() < GetMaxChannel())) {
		CRoomHandler* pkRoomHandler = rkPlayer_.GetRoomHandler();
		if(isptr(pkRoomHandler)) {
			CRoom* pkRoom = pkRoomHandler->GetRoom();
			if(isptr(pkRoom)) {
				INT iCount = 0;
				CPlayerEx* pkPlayer = NULL;
				CConnector*	pkConnector = NULL;

				CCommand kCommand;
				kCommand.SetOrder(ROOM_LIST);
				kCommand.SetExtra(NEW);
				kCommand.SetMission(1);	// 1번은 맴버 목록.

				SRoomMemberListGsToCl& rtSData = (SRoomMemberListGsToCl&)kCommand.GetData();

				for(UINT i = 0; i < pkRoom->GetTopCount(); ++i) {
					pkPlayer = (CPlayerEx*)pkRoom->GetMember(i);
					if(isptr(pkPlayer)) {
						if(pkRoom != pkPlayer->GetRoomHandler()->GetRoom()) {
							OUTPUT("critical error: unit is not in this room: "<<pkRoom->GetTopCount());
							return false;
						}

						pkConnector = pkPlayer->GetNetIO()->GetConnector();
						if(isptr(pkConnector)) {
							if(iCount >= iMAX_PACKET_ROOM_MEMBER_LIST) {
								TRACE("iMAX_PACKET_ROOM_MEMBER_LIST: count: " << iMAX_PACKET_ROOM_MEMBER_LIST << ", overflow: "<<iCount);
								kCommand.SetOption(iMAX_PACKET_ROOM_MEMBER_LIST);
								rkPlayer_.Launcher(kCommand, iCount * sizeof(SRoomMember));
								iCount = 0;
								kCommand.SetExtra(CHANGE);
							}

							rtSData.list[iCount].actor = pkPlayer->GetKey();
							rtSData.list[iCount].SetName(*pkPlayer->GetName());
							rtSData.list[iCount].status = pkPlayer->GetStatus();

							rtSData.list[iCount].public_ip = pkConnector->GetPublicSinAddress();
							rtSData.list[iCount].public_port = pkConnector->GetPublicPort();
							rtSData.list[iCount].local_ip = pkConnector->GetLocalSinAddress();
							rtSData.list[iCount].local_port = pkConnector->GetLocalPort();

							TRACE("["<<pkPlayer->GetName()<<":"<<pkPlayer->GetKey()<<":"<<pkPlayer->GetStatus()<<"] : public addr: "<<pkConnector->GetPublicAddress()<<":"<<pkConnector->GetPublicPort()<<", local addr: "<<pkConnector->GetLocalAddress()<<", "<<pkConnector->GetLocalPort());

							++iCount;
						}
					} else {
						OUTPUT("["<<i<<"] room's member is null: "<<pkRoom->GetTopCount());
						break;
					}
				}

				if(0 < iCount) {
					kCommand.SetOption(iCount);
					rkPlayer_.Launcher(kCommand, iCount * sizeof(SRoomMember));
					TRACE("NEW or CHANGE: ");

					kCommand.SetExtra(DONE);
					kCommand.SetOption(0);
					SRoomMemberLeaderGsToCl& rtLeaderData = (SRoomMemberLeaderGsToCl&)kCommand.GetData();
					if(isptr(pkRoom->GetLeader())) {
						rtLeaderData.leader = pkRoom->GetLeader()->GetKey();
					}

					rkPlayer_.Launcher(kCommand, sizeof(SRoomMemberLeaderGsToCl));
				} else {
					if(NEW == kCommand.GetExtra()) {
						kCommand.SetExtra(EMPTY);
						kCommand.SetOption(0);
						rkPlayer_.Launcher(kCommand);
					}
				}
			}
		}
	}
	return true;
}

void
CChannelMgr::GetUserList(CPlayer& rkPlayer_) {
	if((0 <= rkPlayer_.GetChannelIndex()) && (rkPlayer_.GetChannelIndex() < GetMaxChannel())) {
		CCommand kCommand;
		kCommand.SetOrder(INFO_USER_LIST);
		kCommand.SetExtra(NEW);

		SInfoUserListGsToCl& rtSData = (SInfoUserListGsToCl&)kCommand.GetData();

		INT iCount = 0;
		for(INT i = 0; i < m_iUserTopIndex[rkPlayer_.GetChannelIndex()]; ++i) {
			if(isptr(m_pkUserList[rkPlayer_.GetChannelIndex()][i])) {
				if(iCount >= iMAX_PACKET_INFO_USER_LIST) {
					TRACE("iMAX_PACKET_INFO_USER_LIST: count: " << iMAX_PACKET_INFO_USER_LIST << ", overflow: " << iCount);
					kCommand.SetOption(iMAX_PACKET_INFO_USER_LIST);
					rkPlayer_.Launcher(kCommand, sizeof(SInfoUserListGsToCl));
					iCount = 0;
					kCommand.SetExtra(CHANGE);
				}

				rtSData.list[iCount].aid = m_pkUserList[rkPlayer_.GetChannelIndex()][i]->GetAid();
				rtSData.list[iCount].key = m_pkUserList[rkPlayer_.GetChannelIndex()][i]->GetKey();
				rtSData.list[iCount].SetName(*((CPlayerEx*)m_pkUserList[rkPlayer_.GetChannelIndex()][i])->GetName());

				++iCount;
			}
		}

		if(0 < iCount) {
			kCommand.SetOption(iCount);
			rkPlayer_.Launcher(kCommand, sizeof(SInfoUserData) * iCount);
			TRACE("NEW or CHANGE: ");

			kCommand.SetExtra(DONE);
			kCommand.SetOption(0);
			rkPlayer_.Launcher(kCommand);
		} else {
			if(NEW == kCommand.GetExtra()) {
				kCommand.SetExtra(EMPTY);
				kCommand.SetOption(0);
				rkPlayer_.Launcher(kCommand);
			}
		}
	}
}

void
CChannelMgr::SendNotify(UINT uiChannel_, char& szData_, UINT uiType_) {
	CCommand kCommand;

	//SInfoNofityGsToCl& rSData = (SInfoNofityGsToCl&)kCommand.GetData();
	// rSData.SetContent(szData_);

	kCommand.SetOrder(INFO_NOTIFY);
	//kCommand.SetOption(strlen(rSData.content));
	kCommand.SetExtra(uiType_);

	if(2 < kCommand.GetOption()) {
		TRACE("notify message: "<<&szData_<<", size: "<<kCommand.GetOption());
		if(uiChannel_ == (UINT)(iMAX_CHANNEL_LIST)) {
			for(INT i = 0; i < GetMaxChannel(); ++i) {
				for(INT j = 0; j < m_iUserTopIndex[i]; ++j) {
					if(isptr(m_pkUserList[i][j])) {
						if(0 < m_pkUserList[i][j]->GetKey()) {
							TRACE("send notify: "<<m_pkUserList[i][j]->GetKey());
							//m_pkUserList[i][j]->Launcher(kCommand, sizeof(SInfoNofityGsToCl) - iMAX_CHAT_LEN-1 + kCommand.GetOption());
							TRACE("send notify: OK: "<<m_pkUserList[uiChannel_][j]->GetKey());
						}
					}
				}
			}
		}
	} else {
		OUTPUT("critical error: message size is 0");
	}
}

void
CChannelMgr::SendNotify(CCommand& rkCommand_, INT iSize_, CPlayer* pkPlayer_) {
	for(INT i = 0; i < GetMaxChannel(); ++i) {
		for(INT j = 0; j < m_iUserTopIndex[i]; ++j) {
			if(isptr(m_pkUserList[i][j])) {
				if(0 < m_pkUserList[i][j]->GetKey()) {
					if(m_pkUserList[i][j] != pkPlayer_) {
						TRACE("send notify: command: "<<m_pkUserList[i][j]->GetKey());
						m_pkUserList[i][j]->Launcher(rkCommand_, iSize_);
						TRACE("send notify: command: OK: "<<m_pkUserList[i][j]->GetKey());
					}
				}
			}
		}
	}
}

void
CChannelMgr::Broadcast(CPlayer* pkPlayer_, CCommand& rkCommand_, INT iSize_) {
	if(isptr(pkPlayer_)) {
		if(isptr(pkPlayer_->GetRoomHandler())) {
			if(-1 == pkPlayer_->GetRoomHandler()->GetOffset()) {
				if(pkPlayer_->GetChannelIndex() < INT(GetMaxChannel())) {
					for(INT i=0; i<m_iUserTopIndex[pkPlayer_->GetChannelIndex()]; ++i) {
						if(isptr(m_pkUserList[pkPlayer_->GetChannelIndex()][i])) {
							if(m_pkUserList[pkPlayer_->GetChannelIndex()][i] != pkPlayer_) {
								if(0 < m_pkUserList[pkPlayer_->GetChannelIndex()][i]->GetKey()) {
									if(-1 == m_pkUserList[pkPlayer_->GetChannelIndex()][i]->GetRoomHandler()->GetOffset()) {
										TRACE("send: aid: " << m_pkUserList[pkPlayer_->GetChannelIndex()][i]->GetAid() << ", key: " << m_pkUserList[pkPlayer_->GetChannelIndex()][i]->GetKey());
										m_pkUserList[pkPlayer_->GetChannelIndex()][i]->Launcher(rkCommand_, iSize_);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void
CChannelMgr::BroadcastChannel(INT iChannelIndex_, CCommand& rkCommand_, INT iSize_, CPlayer* pkPlayer_) {
	if(iChannelIndex_ < GetMaxChannel()) {
		for(INT i=0; i<m_iUserTopIndex[iChannelIndex_]; ++i) {
			if(isptr(m_pkUserList[iChannelIndex_][i])) {
				if(m_pkUserList[iChannelIndex_][i] != pkPlayer_) {
					if(0 < m_pkUserList[iChannelIndex_][i]->GetKey()) {
						TRACE("send: aid: " << m_pkUserList[iChannelIndex_][i]->GetAid() << ", key: " << m_pkUserList[iChannelIndex_][i]->GetKey());
						m_pkUserList[iChannelIndex_][i]->Launcher(rkCommand_, iSize_);
					}
				}
			}
		}
	}
}

void
CChannelMgr::BroadcastAll(CCommand& rkCommand_, INT iSize_, CPlayer* pkPlayer_) {
	for(INT i = 0; i < GetMaxChannel(); ++i) {
		for(INT j = 0; j < m_iUserTopIndex[i]; ++j) {
			if(isptr(m_pkUserList[i][j])) {
				if(m_pkUserList[i][j] != pkPlayer_) {
					if(0 < m_pkUserList[i][j]->GetKey()) {
						m_pkUserList[i][j]->Launcher(rkCommand_, iSize_);
					}
				}
			}
		}
	}
}

void
CChannelMgr::Shutdown() {
	for(INT i = 0; i < GetMaxChannel(); ++i) {
		for(INT j = 0; j < m_iUserTopIndex[i]; ++j) {
			if(isptr(m_pkUserList[i][j])) {
				if(0 < m_pkUserList[i][j]->GetKey()) {
					if(0 < m_pkUserList[i][j]->GetAid()) {
						m_pkUserList[i][j]->Disconnect();
					}
				}
			}
		}
	}
}

/* EOF */
