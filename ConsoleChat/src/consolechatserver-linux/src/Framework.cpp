/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "Framework.h"

#include "CommonHeader.h"

template<> CFramework* CSingleton< CFramework >::ms_Instance = NULL;

bool
CFramework::Initialize(const char* szConfigPath_, const char* szPackageName_, INT iServerId_) {
	g_kConnectorProvider.Register(NETWORK_TCP_CLIENT, CTcpConnector::New);
	g_kConnectorProvider.Register(NETWORK_UDP_CLIENT, CUdpConnector::New);
	g_kConnectorProvider.Register(NETWORK_TCP_SERVER, CTcpListener::New);
	g_kConnectorProvider.Register(NETWORK_UDP_SERVER, CUdpListener::New);

	g_kRoomProvider.Register(CRoomEx::New);
	g_kPlayerProvider.Register(CPlayerEx::New);

	m_bInitialized = false;

	if(Action("initializing configure", g_kCfgMgr.Initialize(szConfigPath_, szPackageName_, iServerId_))) {
		if(Action("initializing protocol", g_kNetMgr.CheckProtocol())) {
			INT iMaxUser = g_kCfgMgr.GetMaxConnection();

			//TRACE("max user: " << iMaxUser);

			if(getdtablesize() < iMaxUser) {
				OUTPUT("["<<g_kTick.GetTime()<<"] socket is not enough: "<<getdtablesize()<<" : "<<iMaxUser);
				return false;
			}

			if(Action("initializing server", g_kServerMgr.Initialize())) {
				if(Action("initializing channel", g_kChannelMgr.Initialize())) {
					if(10 <= iMaxUser) {
						g_kChannelMgr.GetServerInfo().SetId(g_kCfgMgr.GetServerId());
						g_kChannelMgr.GetServerInfo().SetMaxUser(iMaxUser);

						// 채널별 최대 동시 접속자수를 설정합니다.
						for(UINT i = 0; i < (UINT)(iMAX_CHANNEL_LIST); ++i) {
							UINT uiChannelId = i+1;
							UINT uiChannelMaxUser = iMaxUser / iMAX_CHANNEL_LIST;

							g_kChannelMgr.GetChannelInfo(i).SetId(uiChannelId);
							g_kChannelMgr.GetChannelInfo(i).SetMaxUser(uiChannelMaxUser);
						}

						g_kChannelMgr.SetMaxChannel(iMAX_CHANNEL_LIST);

						if(Action("initializing network", g_kNetMgr.Initialize(iMaxUser))) {
							if(Action("initializing player's trunk created", g_kUnitMgr.NewTrunk(UNIT_PLAYER, iMaxUser+10))) {
								m_bInitialized = true;
								m_bDoing = true;

								char szFileLogPath[iFILE_PATH_LEN+1] = { 0, };
								snprintf(szFileLogPath, iFILE_PATH_LEN, "%s/%d", g_kCfgMgr.GetLogPath(), 1000000 + g_kCfgMgr.GetServerId());

								char szHackLogPath[iFILE_PATH_LEN+1] = { 0, };
								snprintf(szHackLogPath, iFILE_PATH_LEN, "%s/%d", g_kCfgMgr.GetLogPath(), 2000000 + g_kCfgMgr.GetServerId());

								g_kFileLog = new CLogFile(szFileLogPath);
								g_kHackLog = new CLogFile(szHackLogPath);

								Action("Starting service", Startup());
							}
						}
					} else {
						OUTPUT("["<<g_kTick.GetTime()<<"] max user < 10");
					}
				}
			}
		}
	}
	return m_bInitialized;
}

bool
CFramework::Release() {
	if(m_bInitialized) {
		Action("shutdown immediately", Shutdown());

		m_bInitialized = false;
		m_bDoing = false;

		//OUTPUT("["<<g_kTick.GetTime()<<"] ["<<getpid()<<"] server shutdown now");

		CAllocator< CNativeCommand >::FreeAllPool();
		CAllocator< CExtendCommand >::FreeAllPool();
		CAllocator< CSendCommand >::FreeAllPool();
		CAllocator< CTcpPacketBuffer >::FreeAllPool();
		CAllocator< CUdpPacketBuffer >::FreeAllPool();

		g_kUnitMgr.ReleaseInstance();
		g_kNetMgr.ReleaseInstance();
		g_kChannelMgr.ReleaseInstance();
		g_kServerMgr.ReleaseInstance();

		g_kCfgMgr.ReleaseInstance();

		g_kRoomProvider.ReleaseInstance();
		g_kPlayerProvider.ReleaseInstance();
		g_kConnectorProvider.ReleaseInstance();
	}
	return true;
}

bool
CFramework::Update() {
	g_kTick.Update();
	g_kNetMgr.Update();
	g_kServerMgr.Update();

	return true;
}

bool
CFramework::Startup() {
	if(m_bInitialized) {
		if(g_kNetMgr.StartThread()) {
			//TRACE("network manager: sender thread: start");
		} else {
			//TRACE("network manager: sender thread: not used");
		}

		FILELOG(LE_SERVER_START);
		return true;
	}
	return false;
}

bool
CFramework::Shutdown() {
	if(m_bInitialized) {
		TRACE("[" << g_kTick.GetTime() << "] [" << g_kCfgMgr.GetServerId() << "] shutdown doing.....");

		g_kChannelMgr.Shutdown();
		time_t tmTimeout = g_kTick.GetTime() + 300;
		while(0 < g_kChannelMgr.GetServerInfo().GetUser()) {
			Update();
			TRACE("timeout: " << tmTimeout << ", now: " << g_kTick.GetTime() << ", user: " << g_kChannelMgr.GetServerInfo().GetUser());

			usleep(0);
			if(tmTimeout < g_kTick.GetTime()) {
				break;
			}
		}

		g_kServerMgr.Shutdown();
		tmTimeout = g_kTick.GetTime() + 3;
		while(g_kServerMgr.IsAvailable()) {
			Update();
			TRACE("timeout: " << tmTimeout << ", now: " << g_kTick.GetTime());

			usleep(0);
			if(tmTimeout < g_kTick.GetTime()) {
				break;
			}
		}

		usleep(1000);
		g_kNetMgr.Update();
		usleep(1000);

		TRACE("[" << g_kTick.GetTime() << "] [" << g_kCfgMgr.GetServerId() << "] shutdown doing: all user were disconnected: ok: ");

		if(g_kNetMgr.CancelThread()) {
			//TRACE("network manager: sender thread: cancel");
		} else {
			//TRACE("network manager: sender thread: not used");
		}

		FILELOG(LE_SERVER_STOP);
		usleep(1000);
		return true;
	}
	return false;
}

/* EOF */
