/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "../CommonHeader.h"

bool
CMD_ID_AUTHORIZE(CExtendCommand& rkCommand_) {
	CCommand& rkSelfCommand = rkCommand_.GetCommand();
	CConnector* pkConnector = rkCommand_.GetConnector();

	if (isptr(pkConnector)) {
		CUnit* pkActor = g_kUnitMgr.GetUnit(UNIT_PLAYER, rkCommand_.GetKey());
		if(isptr(pkActor)) {
			CPlayerEx* pkPlayer = (CPlayerEx*)pkActor->GetTypeAs(UNIT_PLAYER);
			if(isptr(pkPlayer)) {
				if(0 == pkPlayer->GetAid()) {
					CNetIO* pkNetIO = pkPlayer->GetNetIO();
					if(isptr(pkNetIO)) {
						UINT uiKey = GenerateBogoKey(UNIT_PLAYER, rkCommand_.GetKey());
						pkPlayer->SetKey(uiKey);

						pkNetIO->SetConnector(pkConnector);

						if(NONE != rkSelfCommand.GetExtra()) {
							if(TIMEOUT != rkSelfCommand.GetExtra()) {
								rkSelfCommand.SetExtra(DENY);
								pkActor->Launcher(rkSelfCommand);
								pkActor->Disconnect();
								return true;
							}
						}

						UINT uiChannelIndex = 0;

						SIdAuthorizeClToGs& rtRData = (SIdAuthorizeClToGs&)rkSelfCommand.GetData();
						if((rtRData.major_version == iSERVICE_MAJOR_VERSION) && (rtRData.minor_version == iSERVICE_MINOR_VERSION)) {
							uiChannelIndex = rtRData.channel_index;

							char szTempLoginId[iLOGIN_ID_LEN] = {0,};
							memcpy(szTempLoginId, rtRData.GetLoginId(), iLOGIN_ID_LEN);

							char szTempName[iUNIT_NAME_LEN] = {0,};
							memcpy(szTempName, szTempLoginId, iUNIT_NAME_LEN);

							CPlayer* pkTargetPlayer = g_kUnitMgr.FindPlayer(UNIT_PLAYER, szTempName);
							if(isptr(pkTargetPlayer)) {
								// 접속 중인 유저가 있을 때.
								TRACE("BUSY: " << pkPlayer->GetName() << " : " << pkPlayer->GetKey());

								// 같은 서버 일 때는 이렇게 해결 되지만 다른 서버 일 때는 즉시 종료가 안됨.
								// 이럴 땐 Client로 종료 대기 중인 서버 채널 정보를 알려 주고 그쪽으로 접속 할 수 있도록 해야 함.
								CCommand kCommand;
								kCommand.SetOrder(ID_AUTHORIZE);
								kCommand.SetExtra(BUSY);
								kCommand.SetOption(0);
								pkTargetPlayer->Launcher(kCommand);
								pkTargetPlayer->Disconnect();
							}

							pkPlayer->SetLoginId(*szTempLoginId);
							pkPlayer->SetName(*szTempName);
							pkPlayer->SetAid(rkCommand_.GetKey());	// DB 인증을 따로 하고있지 않아 인증되었다는 표시를 임의값으로 지정합니다.

							if(0 < rtRData.local_ip) {
								pkPlayer->GetNetIO()->GetConnector()->SetLocalAddress((ULONG)(rtRData.local_ip), (INT)(rtRData.local_port));
							}
							pkPlayer->GetNetIO()->GetConnector()->SetPublicAddress(pkPlayer->GetNetIO()->GetConnector()->GetRemoteSinAddress(), pkPlayer->GetNetIO()->GetConnector()->GetRemotePort());

							g_kServerMgr.IncreasedUser();

							TRACE("SERVER: IN: id: " << g_kChannelMgr.GetServerInfo().GetId() << ", max user: " << g_kChannelMgr.GetServerInfo().GetMaxUser() << ", user: " << g_kChannelMgr.GetServerInfo().GetUser());

							if(false == g_kServerMgr.InChannel(uiChannelIndex, *pkPlayer)) {
								CCommand kCommand;
								kCommand.SetOrder(ID_AUTHORIZE);
								kCommand.SetExtra(BUSY);
								kCommand.SetOption(1);
								pkActor->Launcher(rkSelfCommand);
								pkActor->Disconnect();
								return true;
							}

							SIdAuthorizeGsToCl& rtSData = (SIdAuthorizeGsToCl&)rkSelfCommand.GetData();
							rtSData.key = pkPlayer->GetKey();
							rtSData.aid = pkPlayer->GetAid();
							rtSData.SetName(*pkPlayer->GetName());
							rtSData.tick = g_kTick.GetTick();

							rtSData.public_ip = pkPlayer->GetNetIO()->GetConnector()->GetPublicSinAddress();
							rtSData.public_port = pkPlayer->GetNetIO()->GetConnector()->GetPublicPort();

							if(TIMEOUT == rkSelfCommand.GetExtra()) {
								TRACE("TIMEOUT: aid: "<<pkPlayer->GetAid() << ", key: " << pkPlayer->GetKey() << ", name: " << rtSData.GetName());
								rkSelfCommand.SetExtra(TIMEOUT);
							} else {
								TRACE("OK: aid: "<<pkPlayer->GetAid() << ", key: " << pkPlayer->GetKey() << ", name: " << rtSData.GetName());
								rkSelfCommand.SetExtra(OK);
							}

							pkPlayer->Launcher(rkSelfCommand, sizeof(SIdAuthorizeGsToCl));

							rkSelfCommand.SetOrder(INFO_CHANNEL);
							rkSelfCommand.SetExtra(OK);
							pkPlayer->Launcher(rkSelfCommand);

							return true;
						} else {
							TRACE("error: version is not valid: aid: " << pkPlayer->GetAid() << ", key: " << pkActor->GetKey() << ", client version: " << rtRData.major_version << "." << rtRData.minor_version << ", server version: " << iSERVICE_MAJOR_VERSION << "." << iSERVICE_MINOR_VERSION);
							rkSelfCommand.SetExtra(DATA_ERROR);
						}
					} else {
						OUTPUT("[" << g_kTick.GetTime() << ":" << pkActor->GetAid() << "] netio is null: ");
						rkSelfCommand.SetExtra(FAIL);
					}
				} else {
					OUTPUT("[" << g_kTick.GetTime() << ":" << pkActor->GetAid() << "] aid is not 0: ");
					rkSelfCommand.SetExtra(FAIL);
				}
			} else {
				OUTPUT("[" << g_kTick.GetTime() << ":" << pkActor->GetAid() << "] player is null: ");
				rkSelfCommand.SetExtra(FAIL);
			}
		} else {
			OUTPUT("[" << g_kTick.GetTime() << "] unit is null: ");
			rkSelfCommand.SetExtra(FAIL);
		}

		pkConnector->Send(rkSelfCommand);
		pkConnector->Disconnect();
	} else {
		OUTPUT("[" << g_kTick.GetTime() << "] connector is null: ");
	}
	return true;
}

bool
CMD_ID_PING(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*) rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
//			INT iDelayTick = 0;
//
//			if(isptr(rkActor_.GetNetIO())) {
//				if(0 == rkActor_.GetNetIO()->GetDelayPingTick()) {
//					rkActor_.GetNetIO()->SetDelayPingTick(g_kTick.GetTick());
//				} else {
//					iDelayTick = g_kTick.GetTick() - rkActor_.GetNetIO()->GetDelayPingTick();
//					rkActor_.GetNetIO()->SetDelayPingTick(g_kTick.GetTick());
//				}
//			}

			rkCommand_.SetOrder(ID_PONG);
			rkCommand_.SetExtra(OK);

			SIdPongGsToCl& rtSData = (SIdPongGsToCl&) rkCommand_.GetData();
			rtSData.tick = g_kTick.GetTick();

			//TRACE("OK: aid: " << pkPlayer->GetAid() << ", key: " << pkPlayer->GetKey() << ", tick: " << rtSData.tick << ", delay: " << iDelayTick);

			pkPlayer->Launcher(rkCommand_, sizeof(SIdPongGsToCl));

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
CMD_ID_QUIT(CUnit& rkActor_, CCommand& rkCommand_) {
	CPlayer* pkPlayer = (CPlayer*) rkActor_.GetTypeAs(UNIT_PLAYER);
	if(isptr(pkPlayer)) {
		if(0 < pkPlayer->GetAid()) {
			TRACE("OK: aid: " << pkPlayer->GetAid() << ", key: " << pkPlayer->GetKey());
		} else {
			OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] aid is 0: ");
		}
	} else {
		OUTPUT("[" << g_kTick.GetTime() << ":" << rkActor_.GetAid() << "] player is null: ");
	}

	rkActor_.Disconnect();
	return true;
}

void
InitializeIdCommand() {
	g_pbfExtendLauncher[ID_AUTHORIZE] = CMD_ID_AUTHORIZE;

	g_pbfNativeLauncher[ID_PING] = CMD_ID_PING;
	g_pbfNativeLauncher[ID_QUIT] = CMD_ID_QUIT;
}

/* EOF */
