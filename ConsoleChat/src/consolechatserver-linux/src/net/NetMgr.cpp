/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "NetMgr.h"

#include "../CommonHeader.h"

template<> CNetMgr* CSingleton< CNetMgr >::ms_Instance = NULL;

CNetMgr::CNetMgr()
: m_bInitialized(false)
{
}

CNetMgr::~CNetMgr() {
	Release();
}

bool
CNetMgr::Initialize(INT iMaxUser_) {
	bool bCheck = true;

	if(0 >= g_kCfgMgr.GetTcpDefaultPort()) {
		OUTPUT("[" << g_kTick.GetTime() << "] tcp default port is null: ");
		return false;
	}

	if(0 >= g_kCfgMgr.GetTcpBackbonePort()) {
		OUTPUT("[" << g_kTick.GetTime() << "] tcp backbone port is null: ");
		return false;
	}

	g_kConnectorProvider.Register(NETWORK_UDP_CLIENT, CReliableConnector::New);
	if(false == Action("initializing slave network", GetSlaveNetwork().Initialize(SENSOR_EVENT_POLL, COMMAND_EXTEND, iMaxUser_ + 15, NETWORK_UDP_SERVER))) {
		bCheck = false;
	}

	g_kConnectorProvider.Register(NETWORK_TCP_CLIENT, CDefaultConnector::New);
	if(false == Action("Initializing master network", GetMasterNetwork().Initialize(SENSOR_EVENT_POLL, COMMAND_EXTEND, iMaxUser_ + 15, NETWORK_TCP_SERVER))) {
		bCheck = false;
	}

	if(bCheck) {
#ifdef _THREAD
		GetMasterNetwork().CreateWaitEventThread(500);

		INT iCPUNums = sysconf(_SC_NPROCESSORS_CONF);
		if(4 < iCPUNums) {
			//GetMasterNetwork().CreateSenderThread(iCPUNums-4);
			GetMasterNetwork().CreateSenderThread(4);
		} else if(2 < iCPUNums) {
			GetMasterNetwork().CreateSenderThread(2);
		} else {
			GetMasterNetwork().CreateSenderThread(1);
		}

		GetSlaveNetwork().CreateWaitEventThread(500);
		GetSlaveNetwork().CreateSenderThread(1);
#else
		TRACE("thread is not initialized");
#endif
		if(Action("creating listener for default service", isptr(GetMasterNetwork().Create("0.0.0.0", g_kCfgMgr.GetTcpDefaultPort(), NETWORK_TCP_SERVER)))) {
			g_kConnectorProvider.Register(NETWORK_TCP_CLIENT, CBackboneConnector::New);
			g_kConnectorProvider.Register(NETWORK_TCP_SERVER, CBackboneListener::New);
			if(Action("creating listener for backbone service", isptr(GetMasterNetwork().Create(g_kCfgMgr.GetTcpBackboneAddress(), g_kCfgMgr.GetTcpBackbonePort(), NETWORK_TCP_SERVER)))) {
				IDispatcherList* pkDispatcherList = GetMasterNetwork().GetDispatcherList();
				if(isptr(pkDispatcherList)) {
					// 패킷 해더의 암호화를 항상 하려면 true로 설정.
					// 클라이언트도 true로 설정되어야 서로 해석할수 있음.
					// 패킷 데이터의 암호화는 별개이며 Send 할때마다 설정할수 있음.
					if(g_kCfgMgr.IsHeaderCrypt()) {
						pkDispatcherList->SetHeaderCrypt(true);
					}

					if(Action("creating listener for reliable service", isptr(GetSlaveNetwork().Create("0.0.0.0", g_kCfgMgr.GetUdpReliablePort(), NETWORK_UDP_SERVER)))) {
						pkDispatcherList = GetSlaveNetwork().GetDispatcherList();
						if(isptr(pkDispatcherList)) {
							if(g_kCfgMgr.IsHeaderCrypt()) {
								pkDispatcherList->SetHeaderCrypt(true);
							}

							m_bInitialized = true;
							return true;
						} else {
							OUTPUT("[" << g_kTick.GetTime() << "] dispatcher list is null");
						}
					}
				} else {
					OUTPUT("[" << g_kTick.GetTime() << "] dispatcher list is null");
				}
			}
		}
	} else {
		TRACE("error: network manager is not initialized");
	}
	return false;
}

bool
CNetMgr::Release() {
	if(m_bInitialized) {
		return true;
	}
	return false;
}

bool
CNetMgr::Update() {
#ifndef _THREAD
	GetMasterNetwork().WaitEvent(100);
	GetSlaveNetwork().WaitEvent(100);

	GetMasterNetwork().SendQueue();
	GetSlaveNetwork().SendQueue();
#endif

	if(false == GetMasterNetwork().GetCommandQueue()->Empty()) {
		CExtendCommand* pkExtendCommand = NULL;
		INT iCount = 0;
		while(isptr(pkExtendCommand = static_cast<CExtendCommand*>(GetMasterNetwork().GetCommandQueue()->Take()))) {
			GetMasterNetwork().ParseCommand(*pkExtendCommand);
			CAllocator< CExtendCommand >::Delete(pkExtendCommand);
			++iCount;
			if(200 < iCount) {
				break;
			}
		}
	}

	if(false == GetSlaveNetwork().GetCommandQueue()->Empty()) {
		CExtendCommand* pkExtendCommand = NULL;
		INT iCount = 0;
		while(isptr(pkExtendCommand = static_cast<CExtendCommand*>(GetSlaveNetwork().GetCommandQueue()->Take()))) {
			GetSlaveNetwork().ParseCommand(*pkExtendCommand);
			CAllocator< CExtendCommand >::Delete(pkExtendCommand);
			++iCount;
			if(200 < iCount) {
				break;
			}
		}
	}
	return true;
}

bool
CNetMgr::StartThread() {
#ifdef _THREAD
	GetMasterNetwork().StartThread();
	GetSlaveNetwork().StartThread();
	return true;
#else
	return false;
#endif
}

bool
CNetMgr::CancelThread() {
#ifdef _THREAD
	GetMasterNetwork().CancelThread();
	GetSlaveNetwork().CancelThread();
	return true;
#else
	return false;
#endif
}

bool
CNetMgr::CheckProtocol() {
	InitializeCommand();

	TRACE("iCOMMAND_DATA_SIZE: " << iCOMMAND_DATA_SIZE);

	TRACE("iMAX_PACKET_INFO_USER_LIST: " << iMAX_PACKET_INFO_USER_LIST);

	TRACE("iMAX_PACKET_ROOM_LIST: " << iMAX_PACKET_ROOM_LIST);
	TRACE("iMAX_PACKET_ROOM_MEMBER_LIST: " << iMAX_PACKET_ROOM_MEMBER_LIST);

	return true;
}

/* EOF */
