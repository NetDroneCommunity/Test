﻿/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace CompatibilityStandards {
	#region User-Defined Types
	using UINT = System.UInt32;
	using BYTE = System.Byte;
	using SBYTE = System.SByte;
	using WORD = System.UInt16;
	using DWORD = System.UInt32;
	using QWORD = System.UInt64;
	using ULONG = System.UInt32;
	using ULONG32 = System.UInt32;
	using ULONG64 = System.UInt64;
	using CHAR = System.Byte;
	using INT = System.Int32;
	using INT16 = System.Int16;
	using INT32 = System.Int32;
	using INT64 = System.Int64;
	using UINT16 = System.UInt16;
	using UINT32 = System.UInt32;
	using UINT64 = System.UInt64;
	using LONG32 = System.Int32;
	using LONG64 = System.Int64;
	using FLOAT = System.Single;
	using DOUBLE = System.Double;
	using tick_t = System.UInt64;
	using time_t = System.UInt64;
	using size_t = System.UInt64;
	using wchar_t = System.Char;
	#endregion

	public partial class GameFramework {
		public static CFramework	g_kFramework { get { return CFramework.GetInstance(); } }

		public class CFramework : CSingleton< CFramework > {
			public CFramework() {}
			~CFramework() { Release(); }

			public bool
			Initialize(CHAR[] szConfigPath_, CHAR[] szPackageName_, INT iServerId_) {
				g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_TCP_CLIENT, new CreateCallback(CTcpConnector.New));
				g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_TCP_SERVER, new CreateCallback(CTcpListener.New));
				g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_UDP_CLIENT, new CreateCallback(CUdpConnector.New));
				g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_UDP_SERVER, new CreateCallback(CUdpListener.New));

				g_kRoomProvider.Register(new CreateCallback(CRoomEx.New));
				g_kPlayerProvider.Register(new CreateCallback(CPlayerEx.New));

				m_bInitialized = false;

				if(Action("initializing configure", g_kCfgMgr.Initialize(szConfigPath_, szPackageName_, iServerId_))) {
					if(Action("initializing protocol", g_kNetMgr.CheckProtocol())) {
						INT iMaxUser = g_kCfgMgr.GetMaxConnection();

						//TRACE("max user: " + iMaxUser);

						// getdtablesize 검사를 대체할 방법을 아직 못찾았습니다.
						// 이식성 및 호환성을 위해 unsafe 코드는 사용하지 않습니다.
						//if(getdtablesize() < iMaxUser) {
						//	OUTPUT("[" + g_kTick.GetTime() + "] socket is not enough: " + getdtablesize() + " : " + iMaxUser);
						//	return false;
						//}

						if(Action("initializing server", g_kServerMgr.Initialize())) {
							if(Action("initializing channel", g_kChannelMgr.Initialize())) {
								if(10 <= iMaxUser) {
									g_kChannelMgr.SetId(g_kCfgMgr.GetServerId());
									g_kChannelMgr.SetMaxUser(iMaxUser);

									// 채널별 최대 동시 접속자수를 설정합니다.
									for(INT i = 0; i < iMAX_CHANNEL_LIST; ++i) {
										INT uiChannelId = i + 1;
										INT uiChannelMaxUser = iMaxUser / iMAX_CHANNEL_LIST;

										g_kChannelMgr.SetId(i, uiChannelId);
										g_kChannelMgr.SetMaxUser(i, uiChannelMaxUser);
									}

									g_kChannelMgr.SetMaxChannel(iMAX_CHANNEL_LIST);

									if(Action("initializing network", g_kNetMgr.Initialize(iMaxUser))) {
										if(Action("initializing player's trunk created", (0 < g_kUnitMgr.NewTrunk((UINT)UNIT_TYPE.UNIT_PLAYER, (UINT)iMaxUser + 10)))) {
											m_bInitialized = true;
											m_bDoing = true;

											string szFileLogPath = String.Format("{0}{1}{2:D}", ConvertToString(g_kCfgMgr.GetLogPath()), Path.DirectorySeparatorChar, 1000000 + g_kCfgMgr.GetServerId());
											string szHackLogPath = String.Format("{0}{1}{2:D}", ConvertToString(g_kCfgMgr.GetLogPath()), Path.DirectorySeparatorChar, 2000000 + g_kCfgMgr.GetServerId());

											g_kFileLog = new CLogFile(szFileLogPath);
											g_kHackLog = new CLogFile(szHackLogPath);

											Action("Starting service", Startup());
										}
									}
								} else {
									OUTPUT("[" + g_kTick.GetTime() + "] max user < 10");
								}
							}
						}
					}
				}
				return m_bInitialized;
			}

			public bool
			Release() {
				if(m_bInitialized) {
					Action("shutdown immediately", Shutdown());

					m_bInitialized = false;
					m_bDoing = false;

					//OUTPUT("[" + g_kTick.GetTime() + "] [" + Process.GetCurrentProcess().Id + "] server shutdown now");

					CAllocator< CNativeCommand >.FreeAllPool();
					CAllocator< CExtendCommand >.FreeAllPool();
					CAllocator< CSendCommand >.FreeAllPool();
					CAllocator< CTcpPacketBuffer >.FreeAllPool();
					CAllocator< CUdpPacketBuffer >.FreeAllPool();

					g_kUnitMgr.ReleaseInstance();
					g_kNetMgr.ReleaseInstance();
					
					g_kChannelMgr.ReleaseInstance();
					g_kServerMgr.ReleaseInstance();

					g_kCfgMgr.ReleaseInstance();

					g_kPlayerProvider.ReleaseInstance();
					g_kRoomProvider.ReleaseInstance();
					g_kConnectorProvider.ReleaseInstance();
				}
				return true;
			}

			public bool
			Update() {
				g_kTick.Update();
				g_kNetMgr.Update();
				g_kServerMgr.Update();

				return true;
			}

			public bool		IsDoing()			{ return m_bDoing; }
			public void		SetDoing(bool o)	{ m_bDoing = o; }

			public bool
			Startup() {
				if(m_bInitialized) {
					if(g_kNetMgr.StartThread()) {
						//TRACE("network manager: sender thread: start");
					} else {
						//TRACE("network manager: sender thread: not used");
					}

					FILELOG(LOG_EVENT_TYPE.LE_SERVER_START);
					return true;
				}
				return false;
			}

			public bool
			Shutdown() {
				if(m_bInitialized) {
					TRACE("[" + g_kTick.GetTime() + "] [" + g_kCfgMgr.GetServerId() + "] shutdown doing.....");

					g_kChannelMgr.Shutdown();
					time_t tmTimeout = g_kTick.GetTime() + 300;
					while(0 < g_kChannelMgr.GetServerInfo().GetUser()) {
						Update();
						TRACE("timeout: " + tmTimeout + ", now: " + g_kTick.GetTime() + ", user: " + g_kChannelMgr.GetServerInfo().GetUser());

						Thread.Sleep(0);
						if(tmTimeout < g_kTick.GetTime()) {
							break;
						}
					}

					g_kServerMgr.Shutdown();
					tmTimeout = g_kTick.GetTime() + 3;
					while(g_kServerMgr.IsAvailable()) {
						Update();
						TRACE("timeout: " + tmTimeout + ", now: " + g_kTick.GetTime());

						Thread.Sleep(0);
						if(tmTimeout < g_kTick.GetTime()) {
							break;
						}
					}

					Thread.Sleep(1);
					g_kNetMgr.Update();
					Thread.Sleep(1);

					TRACE("[" + g_kTick.GetTime() + "] [" + g_kCfgMgr.GetServerId() + "] shutdown doing: all user were disconnected: ok: ");

					if(g_kNetMgr.CancelThread()) {
						//TRACE("network manager: sender thread: cancel");
					} else {
						//TRACE("network manager: sender thread: not used");
					}

					FILELOG(LOG_EVENT_TYPE.LE_SERVER_STOP);
					Thread.Sleep(1);
					return true;
				}
				return false;
			}

			private bool	m_bInitialized = false;
			private bool	m_bDoing = true;
		}
	}
}

/* EOF */
