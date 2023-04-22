/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Text;
using System.Net;
using System.IO;
using System.Threading;
using System.Collections.Generic;
using System.Runtime.InteropServices;

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
		public static CNetMgr		g_kNetMgr		{ get { return CNetMgr.GetInstance(); } }

		public class CNetMgr : CSingleton<CNetMgr> {
			public CNetMgr() {}
			~CNetMgr() { Release(); }

			public bool
			Initialize() {
				if(false == m_bInitialized) {
					bool bCheck = true;

					if(g_kCfgMgr.IsReliableUdp()) {
						g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_UDP_CLIENT, new CreateCallback(CReliableConnector.New));
						if(false == Action("initializing network", GetNetwork().Initialize(SENSOR_TYPE.SENSOR_POLL, COMMAND_TYPE.COMMAND_NATIVE, 5, NETWORK_TYPE.NETWORK_UDP_CLIENT))) {
							bCheck = false;
						}

						if(Action("creating listener for service", isptr(m_kListener = GetNetwork().Create(ConvertToBytes("0.0.0.0"), 0, NETWORK_TYPE.NETWORK_UDP_SERVER)))) {
							IDispatcherList kDispatcherList = GetNetwork().GetDispatcherList();
							if(isptr(kDispatcherList)) {
								if(g_kCfgMgr.IsHeaderCrypt()) {
									kDispatcherList.SetHeaderCrypt(true);
								}

								m_kConnector = GetNetwork().Empty();
								if(isptr(m_kConnector)) {
									m_kConnector.SetSocket(m_kListener.GetSocket());
								}
							}
						}
					} else {
						g_kConnectorProvider.Register((INT)NETWORK_TYPE.NETWORK_TCP_CLIENT, new CreateCallback(CDefaultConnector.New));
						if(false == Action("initializing network", GetNetwork().Initialize(SENSOR_TYPE.SENSOR_POLL, COMMAND_TYPE.COMMAND_NATIVE, 5, NETWORK_TYPE.NETWORK_TCP_CLIENT))) {
							bCheck = false;
						}

						if(Action("creating connector for service", isptr(m_kConnector = GetNetwork().Create(ConvertToBytes("0.0.0.0"), 0, NETWORK_TYPE.NETWORK_TCP_CLIENT)))) {
							if(g_kCfgMgr.IsHeaderCrypt()) {
								m_kConnector.SetHeaderCrypt(true);
							}
						}
					}

					if(bCheck) {
						m_bInitialized = true;
						return true;
					}
				}
				return false;
			}

			public bool
			Release() {
				if(m_bInitialized) {
					if(IsConnected()) {
						SEND_ID_QUIT();
					}

					m_tkTimeoutLoginTick = 0;
					m_tkDelayPingTick = 0;
					m_tkSendPingServerTick = 0;

					m_kListener = null;
					m_kConnector = null;

					m_kNetwork = null;

					m_bInitialized = false;

					return true;
				}
				return false;
			}

			public bool
			Update() {
				GetNetwork().WaitEvent(100);
				GetNetwork().SendQueue();

				if(false == GetNetwork().GetCommandQueue().Empty()) {
					CNativeCommand kNativeCommand = null;
					INT iCount = 0;

					while(isptr(kNativeCommand = (CNativeCommand)(GetNetwork().GetCommandQueue().Take()))) {
						GetNetwork().ParseCommand(kNativeCommand);
						CAllocator< CNativeCommand >.Delete(kNativeCommand);

						++iCount;

						if(200 < iCount) {
							break;
						}
					}
				}

				if(IsConnected()) {
					if(g_kTick.GetTick() > GetSendPingServerTick()) {
						OUTPUT("update: check: tick: " + g_kTick.GetTick() + ", current: " + GetSendPingServerTick());
						SEND_ID_PING();
						SetSendPingServerTick(g_kTick.GetTick() + iMAX_PING_LATENCY_TICK);
					} else {
						//OUTPUT("update: check: tick: " + g_kTick.GetTick() + ", next: " + GetSendPingServerTick());
					}
				}

				return true;
			}

			public INT
			Send(CCommand kCommand_, INT iSize_ = 0, PACKET_TYPE ePacketType_ =PACKET_TYPE.PACKET_TRUST, CRYPT_TYPE eCryptType_ =CRYPT_TYPE.CRYPT_NONE) {
				if(isptr(GetConnector())) {
					INT iRet = GetConnector().Send(kCommand_, iSize_, ePacketType_, eCryptType_);
					if(0 > iRet) {
						GetConnector().SetRelay(-1);
						DisconnectAll();
					}
					return iRet;
				}
				return -1;
			}

			public bool
			Login() {
				CConnector kConnector = GetConnector();
				if(isptr(kConnector)) {
					if(kConnector.Connect(kConnector.GetRemoteAddress(), kConnector.GetRemotePort())) {
						if(g_kCfgMgr.IsReliableUdp()) {
							OUTPUT("connector type: reliable udp: connected");
						} else {
							OUTPUT("connector type: tcp: connected");
						}

						g_kTick.Update();
						m_tkTimeoutLoginTick = g_kTick.GetTick() + iMAX_TIMEOUT_LATENCY_TICK;

						while(0 >= kConnector.GetSerialKey()) {
							g_kFramework.Update();

							if(g_kTick.GetTick() >= m_tkTimeoutLoginTick) {
								OUTPUT("timeout: tick: " + (iMAX_TIMEOUT_LATENCY_TICK + (g_kTick.GetTick() - m_tkTimeoutLoginTick)));
								break;
							}
						}

						if(0 < kConnector.GetSerialKey()) {
							SEND_ID_AUTHORIZE();
							return true;
						}
					} else {
						g_kNetMgr.DisconnectAll();

						if(g_kCfgMgr.IsReliableUdp()) {
							OUTPUT("connector type: reliable udp: connecting failed");
						} else {
							OUTPUT("connector type: tcp: connecting failed");
						}
					}
				}
				return false;
			}

			public void
			DisconnectAll() {
				OUTPUT("disconnect: all: ");
				// Relay는 아직 사용 안합니다.
				// 서버에서 1로 임시 고정.
				if(isptr(m_kConnector)) {
					if(m_kConnector.IsConnected()) {
						m_kConnector.Disconnect(m_kConnector.GetSocket());

						if(isptr(m_kListener)) {
							if(isptr(m_kListener.GetSocket())) {
								m_kListener.GetSocket().Close();
							}
							m_kListener.Disconnect(m_kListener.GetSocket());
							m_kListener.Listen(ConvertToBytes("0.0.0.0"), 0);
							m_kConnector.SetSocket(m_kListener.GetSocket());
						}
					}

					m_kConnector.SetRelay(-1);
					m_kConnector.SetSerialKey(0);
				}

				Thread.Sleep(0);
			}

			public bool
			IsConnected() {
				return GetConnector().IsConnected();
			}

			public bool
			CheckProtocol() {
				InitializeCommand();

				bool bCheck = true;

				StringBuilder kBuilder = new StringBuilder();
				kBuilder.AppendLine("iMAX_PACKET_SIZE: " + iMAX_PACKET_SIZE);
				kBuilder.AppendLine("iTCP_PACKET_HEAD_SIZE: " + iTCP_PACKET_HEAD_SIZE);
				kBuilder.AppendLine("iUDP_PACKET_HEAD_SIZE: " + iUDP_PACKET_HEAD_SIZE);
				kBuilder.AppendLine("iCOMMAND_HEAD_SIZE: " + iCOMMAND_HEAD_SIZE);
				kBuilder.AppendLine("iCOMMAND_DATA_SIZE: " + iCOMMAND_DATA_SIZE);
				kBuilder.AppendLine("iPACKET_DATA_SIZE: " + iPACKET_DATA_SIZE);

				kBuilder.Append("SIdAuthorizeGsToCl: " + Marshal.SizeOf(typeof(SIdAuthorizeGsToCl)));
				if(iCOMMAND_DATA_SIZE < Marshal.SizeOf(typeof(SIdAuthorizeGsToCl))) {
					kBuilder.Append(" (overflow)");
					bCheck = false;
				}
				kBuilder.AppendLine();

				kBuilder.Append("SIdAuthorizeClToGs: " + Marshal.SizeOf(typeof(SIdAuthorizeClToGs)));
				if(iCOMMAND_DATA_SIZE < Marshal.SizeOf(typeof(SIdAuthorizeClToGs))) {
					kBuilder.Append(" (overflow)");
					bCheck = false;
				}
				kBuilder.AppendLine();

				kBuilder.Append("SIdPongGsToCl: " + Marshal.SizeOf(typeof(SIdPongGsToCl)));
				if(iCOMMAND_DATA_SIZE < Marshal.SizeOf(typeof(SIdPongGsToCl))) {
					kBuilder.Append(" (overflow)");
					bCheck = false;
				}
				kBuilder.AppendLine();

				kBuilder.Append("SInfoServerGsToCl: " + Marshal.SizeOf(typeof(SInfoServerGsToCl)));
				if(iCOMMAND_DATA_SIZE < Marshal.SizeOf(typeof(SInfoServerGsToCl))) {
					kBuilder.Append(" (overflow)");
					bCheck = false;
				}
				kBuilder.AppendLine();

				if(0 < kBuilder.Length) {
					PRINT("");
					PRINT(kBuilder.ToString());
				}

				return bCheck;
			}

			public CNetworkEx	GetNetwork()					{ return m_kNetwork; }

			public CConnector	GetConnector()					{ return m_kConnector; }
			public CConnector	GetListener()					{ return m_kListener; }

			public void			SetDelayPingTick(tick_t o)		{ m_tkDelayPingTick = o; }
			public tick_t		GetDelayPingTick()				{ return m_tkDelayPingTick; }

			public void			SetSendPingServerTick(tick_t o)	{ m_tkSendPingServerTick = o; }
			public tick_t		GetSendPingServerTick()			{ return m_tkSendPingServerTick; }

			public bool			IsInput()						{ return m_bInput; }
			public void			SetInput(bool o)				{ m_bInput = o; }

			private CNetworkEx	m_kNetwork = new CNetworkEx();

			private CConnector	m_kConnector = null;
			private CConnector	m_kListener = null;

			private tick_t		m_tkDelayPingTick = 0;
			private tick_t		m_tkSendPingServerTick = 0;
			private tick_t		m_tkTimeoutLoginTick = 0;

			private bool		m_bInput = false;

			private bool		m_bInitialized = false;

		}
	}
}

/* EOF */
