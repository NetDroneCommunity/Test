/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "../CommonHeader.h"

bool
CMD_ROOM_CREATE(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = rkActor_.GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				CRoom* pkRoom = pkRoomHandler->GetRoom();
				if(isptr(pkRoom)) {
					CCommand kCommand;
					kCommand.SetOrder(ROOM_LEAVE);
					kCommand.SetExtra(NONE);

					ServerLauncher(rkActor_, kCommand);
				}

				pkRoom = g_kChannelMgr.NewRoom(pkPlayer->GetChannelIndex());
				if(isptr(pkRoom)) {
					SRoomCreateClToGs& rtRData = (SRoomCreateClToGs&)rkCommand_.GetData();
					pkRoom->SetName(rtRData.name);
					pkRoom->SetMaxUser(rtRData.max);
					pkRoom->SetStageId(rtRData.stage_id);

					if(pkRoom->Create(*pkPlayer)) {
						rkCommand_.SetExtra(OK);
						SRoomCreateGsToCl& rtSData = (SRoomCreateGsToCl&)rkCommand_.GetData();
						rtSData.id = pkRoom->GetId();

						rkActor_.Launcher(rkCommand_, sizeof(SRoomCreateGsToCl));

						FILELOG(LE_ROOM_CREATE << LF_AID << rkActor_.GetAid() << LF_ROOM_ID << pkRoom->GetId() << LF_STAGE_ID << pkRoom->GetStageId() << LF_STRING << pkRoom->GetName());

						TRACE("OK: key: " << rkActor_.GetKey() << ", name: " << pkPlayer->GetName() << ", room id: " << pkRoom->GetId() << ", room stage id: " << pkRoom->GetStageId() << ", room max user: " << pkRoom->GetMaxUser() << ", room name: " << pkRoom->GetName());
						return true;
					} else {
						OUTPUT("["<<g_kTick.GetTime()<<":"<<rkActor_.GetAid()<<"] room create failed: ");
						HACKLOG(LE_BUG << LF_AID << rkActor_.GetAid() << LF_DEBUG << __LINE__ << LF_STRING << " ROOM_CREATE: room create failed");
					}

					rkCommand_.SetExtra(DENY);
					g_kChannelMgr.DeleteRoom(pkRoom->GetId());
				} else {
					rkCommand_.SetExtra(NOT_ENOUGH);
					rkActor_.Launcher(rkCommand_);
					HACKLOG(LE_BUG << LF_AID << rkActor_.GetAid() << LF_DEBUG << __LINE__ << LF_STRING << " ROOM_CREATE: room is not enough:");
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
CMD_ROOM_JOIN(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = pkPlayer->GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				CRoom* pkRoom = pkRoomHandler->GetRoom();
				if(isptr(pkRoom)) {
					CCommand kCommand;
					kCommand.SetOrder(ROOM_LEAVE);
					kCommand.SetExtra(NONE);

					ServerLauncher(rkActor_, kCommand);
				}

				SRoomJoinClToGs& rtRData = (SRoomJoinClToGs&)rkCommand_.GetData();

				pkRoom = g_kChannelMgr.FindRoom(rtRData.id);
				if(isptr(pkRoom)) {
					if(pkRoom->Join(*pkPlayer)) {
						TRACE("OK: key: " << pkPlayer->GetKey() << ", room id: " << rtRData.id);
					} else {
						if(1 == pkRoom->GetTopCount()) {
							if(isptr(pkRoom->GetLeader())) {
								if(pkRoom != pkRoom->GetLeader()->GetRoomHandler()->GetRoom()) {
									OUTPUT("critical error: unit is not in this room: ");
									g_kChannelMgr.DeleteRoom(pkRoom->GetId());
								}
							}
						}
						TRACE("error: join failed: " << rtRData.id);
					}
					return true;
				} else {
					TRACE("error: join failed: EMPTY");
					rkCommand_.SetExtra(EMPTY);
					rkActor_.Launcher(rkCommand_);
					return true;
				}
			} else {
				OUTPUT("["<<g_kTick.GetTime()<<":"<<rkActor_.GetAid()<<"] room handler is null: "<<rkActor_.GetKey());
				HACKLOG(LE_BUG << LF_AID << rkActor_.GetAid() << LF_DEBUG << __LINE__ << LF_STRING << " ROOM_JOIN: room handler is null:");
				rkCommand_.SetExtra(FAIL);
			}
		} else {
			OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] aid is 0: ");
			HACKLOG(LE_BUG << LF_AID << rkActor_.GetAid() << LF_DEBUG << __LINE__ << LF_STRING << " ROOM_JOIN: aid is 0:");
			rkCommand_.SetExtra(FAIL);
		}
	} else {
		OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] player is null: ");
		HACKLOG(LE_BUG << LF_AID << rkActor_.GetAid() << LF_DEBUG << __LINE__ << LF_STRING << " ROOM_JOIN: player is null:");
		rkCommand_.SetExtra(FAIL);
	}

	rkActor_.Launcher(rkCommand_);
	rkActor_.Disconnect();
	return true;
}

bool
CMD_ROOM_LEAVE(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = rkActor_.GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				CRoom* pkRoom = pkRoomHandler->GetRoom();
				if(isptr(pkRoom)) {
					if(pkRoomHandler->InRoom()) {
						pkRoom->Leave(*pkPlayer);
					}

					rkCommand_.SetExtra(OK);
					pkPlayer->Launcher(rkCommand_);

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
CMD_ROOM_START(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = rkActor_.GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				CRoom* pkRoom = pkRoomHandler->GetRoom();
				if(isptr(pkRoom)) {
					if(pkPlayer == pkRoom->GetLeader()) {
						if(false == pkRoom->IsDoing()) {
							bool bCheck = true;
							for(UINT i=0; i<pkRoom->GetTopCount(); ++i) {
								CPlayer* pkMember = pkRoom->GetMember(i);
								if(isptr(pkMember)) {
									if(STATUS_WAITING == pkMember->GetStatus()) {
										bCheck = false;
										break;
									}
								}
							}

							if(bCheck) {
								pkRoom->SetDoing(true);

								for(UINT i = 0; i < pkRoom->GetTopCount(); ++i) {
									CPlayer* pkMember = pkRoom->GetMember(i);
									if(isptr(pkMember)) {
										pkMember->SetStatus(STATUS_NORMAL);
									}
								}

								rkCommand_.SetExtra(OK);
								pkPlayer->Launcher(rkCommand_);
								TRACE("OK: key: " << pkPlayer->GetKey() << ", aid: " << pkPlayer->GetAid() << ", room id: " << pkRoom->GetId());

								rkCommand_.SetOrder(ROOM_START_OTHER);

								SRoomStartOtherGsToCl& rtSData = (SRoomStartOtherGsToCl&)rkCommand_.GetData();
								rtSData.actor = pkPlayer->GetKey();

								pkRoom->Broadcast(rkCommand_, sizeof(SRoomStartOtherGsToCl), pkPlayer);

								return true;
							}

							rkCommand_.SetExtra(CANT_DO);
							rkActor_.Launcher(rkCommand_);
						} else {
							rkCommand_.SetExtra(NO_PERMISSION);
							rkActor_.Launcher(rkCommand_);
						}
						return true;
					} else {
						if(pkRoom->IsDoing()) {
							pkPlayer->SetStatus(STATUS_NORMAL);

							rkCommand_.SetExtra(NEW);
							pkPlayer->Launcher(rkCommand_);
							TRACE("NEW: key: " << pkPlayer->GetKey() << ", aid: " << pkPlayer->GetAid() << ", room id: " << pkRoom->GetId());

							rkCommand_.SetOrder(ROOM_START_OTHER);

							SRoomStartOtherGsToCl& rtSData = (SRoomStartOtherGsToCl&)rkCommand_.GetData();
							rtSData.actor = pkPlayer->GetKey();

							pkRoom->Broadcast(rkCommand_, sizeof(SRoomStartOtherGsToCl), pkPlayer);

							return true;
						} else {
							rkCommand_.SetExtra(NO_PERMISSION);
							rkActor_.Launcher(rkCommand_);
						}
						return true;
					}
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
CMD_ROOM_STOP(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = rkActor_.GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				CRoom* pkRoom = pkRoomHandler->GetRoom();
				if(isptr(pkRoom)) {
					if(pkPlayer == pkRoom->GetLeader()) {
						if(pkRoom->IsDoing()) {
							pkRoom->SetDoing(false);

							for(UINT i = 0; i < pkRoom->GetTopCount(); ++i) {
								CPlayer* pkMember = pkRoom->GetMember(i);
								if(isptr(pkMember)) {
									if(pkRoom->GetLeader() == pkMember) {
										pkMember->SetStatus(STATUS_READY);
									} else {
										pkMember->SetStatus(STATUS_WAITING);
									}
								}
							}

							rkCommand_.SetExtra(OK);
							pkPlayer->Launcher(rkCommand_);

							TRACE("OK: key: " << pkPlayer->GetKey() << ", aid: " << pkPlayer->GetAid() << ", room id: " << pkRoom->GetId());

							rkCommand_.SetOrder(ROOM_STOP_OTHER);
							SRoomStopOtherGsToCl& rtSData = (SRoomStopOtherGsToCl&)rkCommand_.GetData();
							rtSData.leader = pkPlayer->GetKey();

							pkRoom->Broadcast(rkCommand_, sizeof(SRoomStopOtherGsToCl), pkPlayer);
						}
					} else {
						rkCommand_.SetExtra(NO_PERMISSION);
						rkActor_.Launcher(rkCommand_);
					}
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
CMD_ROOM_LIST(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*)rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			CRoomHandler* pkRoomHandler = rkActor_.GetRoomHandler();
			if(isptr(pkRoomHandler)) {
				if(1 == rkCommand_.GetMission()) {
					g_kChannelMgr.GetRoomMemberList(*pkPlayer);
					TRACE("OK: ROOM LIST: NONE: member: aid: " << pkPlayer->GetAid() << ", key: " << rkActor_.GetKey());
				} else {
					g_kChannelMgr.GetRoomList(*pkPlayer);
					TRACE("OK: ROOM LIST: NONE: aid: " << pkPlayer->GetAid() << ", key: " << rkActor_.GetKey());
				}
				return true;
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

void
InitializeRoomCommand() {
	g_pbfNativeLauncher[ROOM_CREATE] = CMD_ROOM_CREATE;
	g_pbfNativeLauncher[ROOM_JOIN] = CMD_ROOM_JOIN;
	g_pbfNativeLauncher[ROOM_LEAVE] = CMD_ROOM_LEAVE;
	g_pbfNativeLauncher[ROOM_START] = CMD_ROOM_START;
	g_pbfNativeLauncher[ROOM_STOP] = CMD_ROOM_STOP;
	g_pbfNativeLauncher[ROOM_LIST] = CMD_ROOM_LIST;
}

/* EOF */
