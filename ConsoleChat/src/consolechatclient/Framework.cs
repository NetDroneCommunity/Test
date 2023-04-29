/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Text;
using System.Net.Sockets;

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
		public GameFramework() {}
		~GameFramework() {}

		public static CFramework	g_kFramework		{ get { return CFramework.GetInstance(); } }

		public class CFramework : CSingleton< CFramework > {
			public CFramework() {}
			~CFramework() { Release(); }

			public bool
			Initialize() {
				if(false == m_bInitialized) {
					g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_TCP_CLIENT, new CreateCallback(CTcpConnector.New));
					g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_TCP_SERVER, new CreateCallback(CTcpListener.New));
					g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_UDP_CLIENT, new CreateCallback(CUdpConnector.New));
					g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_UDP_SERVER, new CreateCallback(CUdpListener.New));

					g_kCfgMgr.Initialize();
					g_kNetMgr.Initialize();
					g_kNetMgr.CheckProtocol();

					m_bInitialized = true;
					m_bDoing = true;

					Startup();

					return true;
				}
				return false;
			}

			public bool
			Release() {
				if(m_bInitialized) {
					Shutdown();

					m_bInitialized = false;
					m_bDoing = false;

					CAllocator< CNativeCommand >.FreeAllPool();
					CAllocator< CTcpPacketBuffer >.FreeAllPool();
					CAllocator< CUdpPacketBuffer >.FreeAllPool();

					g_kConnectorProvider.ReleaseInstance();

					g_kNetMgr.Release();
					g_kNetMgr.ReleaseInstance();
					g_kCfgMgr.ReleaseInstance();
				}
				return true;
			}

			public virtual bool
			CreateFrameworkThread(UINT uiSleepTime_ =0) {
#if _THREAD
				m_kFrameworkThread = new CFrameworkThread(this, (INT)uiSleepTime_);
				return true;
#else
				return false;
#endif
			}

			public virtual bool
			StartThread() {
#if _THREAD
				if(isptr(m_kFrameworkThread)) {
					Action("starting framework thread", m_kFrameworkThread.Start());
				}
				return true;
#else
				return false;
#endif
			}

			public virtual bool
			CancelThread() {
#if _THREAD
				if(isptr(m_kFrameworkThread)) {
					Action("stopping framework thread", m_kFrameworkThread.Cancel());
				}
				return true;
#else
				return false;
#endif
			}

			public bool
			Update() {
				if(false == m_bDoing) {
					return false;
				}

				g_kTick.Update();
				g_kNetMgr.Update();

				return true;
			}

			public bool
			Startup() {
				if(m_bInitialized) {
					if(g_kNetMgr.StartThread()) {
						//TRACE("network manager: sender thread: start");
					} else {
						//TRACE("network manager: sender thread: not used");
					}
					return true;
				}
				return false;
			}

			public bool
			Shutdown() {
				if(m_bInitialized) {
					if(g_kNetMgr.CancelThread()) {
						//TRACE("network manager: sender thread: cancel");
					} else {
						//TRACE("network manager: sender thread: not used");
					}

					Thread.Sleep(1);
					return true;
				}
				return false;
			}

			public bool		IsDoing()			{ return m_bDoing; }
			public void		SetDoing(bool o)	{ m_bDoing = o; }

			private bool	m_bInitialized = false;
			private bool	m_bDoing = true;

#if _THREAD
			private CFrameworkThread		m_kFrameworkThread = null;
#endif
		}
	}
}

/* EOF */
