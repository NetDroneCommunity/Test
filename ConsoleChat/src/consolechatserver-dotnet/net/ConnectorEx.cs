﻿/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
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
		public class CDefaultConnector : CTcpConnector {
			public CDefaultConnector() {}
			~CDefaultConnector() {}

			public override void
			SetConnected() {
				base.SetConnected();

				if(INVALID_SOCKET != GetSocket()) {
					SetRelay((INT)RELAY_TYPE.RELAY_CLIENT);

					INT iRand = (INT)RAND(1000000) + 1;
					SetSerialKey(iRand * 999 + iRand * 99 + iRand);

					if(0 == GetSerialKey()) {
						SetSerialKey(1234567890);
					}

					CCommand kCommand = new CCommand();
					kCommand.SetOrder((UINT)PROTOCOL.INFO_SERVER);
					kCommand.SetExtra((UINT)EXTRA.NONE);
					Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_TYPE.CRYPT_NONE);
				}
			}

			public override bool
			Disconnect(Socket skSocket_) {
				if((INVALID_SOCKET != skSocket_) && (GetSocket() == skSocket_) && IsConnected() && (0 != GetSerialKey())) {
					SetSerialKey(0);

					CCommand kCommand = new CCommand();
					kCommand.SetOrder((UINT)PROTOCOL.ID_QUIT);
					kCommand.SetExtra((UINT)EXTRA.REQUEST);
					Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_TYPE.CRYPT_NONE);
					return true;
				} else {
					SetSerialKey(0);
					if(base.Disconnect(skSocket_)) {
						return true;
					}
				}
				return false;
			}

			public override void
			Disconnect() {
				if(isptr(GetSocket())) {
					TRACE("disconnect: socket: " + SOCKDESC(GetSocket()) + ", serial: " + GetSerialKey());
				} else {
					TRACE("disconnect: socket: INVALID_SOCKET, serial: " + GetSerialKey());
				}
				SetSerialKey(0);
				base.Disconnect(GetSocket());
			}

			public override bool
			KeepAlive() {
				if(IsConnected()) {
					if(0 == GetRecvPingTick()) {
						OUTPUT("[" + g_kTick.GetTime() + "] RECV PING: recv tick is 0");
						return false;
					}

					if(g_kTick.GetTick() > GetRecvPingTick() + iRECV_PING_TICK) {
						CPlayerEx kActor = g_kUnitMgr.GetPlayer((UINT)UNIT_TYPE.UNIT_PLAYER, GetRecvKey());
						if(isptr(kActor)) {
							if(isptr(kActor.GetNetIO())) {
								if(kActor.GetNetIO().GetConnector() != this) {
									if(0 < kActor.GetAid()) {
										OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: connector is wrong: [" + kActor.GetAid() + ":" + kActor.GetKey() + ":" + (INT)kActor.GetStatus() + "] delay: " + (g_kTick.GetTick() - GetRecvPingTick()) + ", addr : " + ConvertToString(GetRemoteAddress()));
									}
									return false;
								}

								OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: time is too late: [" + kActor.GetAid() + ":" + kActor.GetKey() + ":" + (INT)kActor.GetStatus() + "] delay: " + (g_kTick.GetTick() - GetRecvPingTick()) + ", addr : " + ConvertToString(GetRemoteAddress()));
								return false;
							} else {
								OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: Net IO is null: [" + kActor.GetAid() + ":" + kActor.GetKey() + "] : " + ConvertToString(GetRemoteAddress()));
								return false;
							}
						} else {
							OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: actor is null: ");
							return false;
						}
					}
				}
				return true;
			}

			public new static object	New()	{ return new CDefaultConnector(); }
		}

		public class CBackboneConnector : CTcpConnector {
			public CBackboneConnector() {}
			~CBackboneConnector() {}

			public override void
			SetConnected() {
				base.SetConnected();

				if(INVALID_SOCKET != GetSocket()) {
					SetRelay((INT)RELAY_TYPE.RELAY_CLIENT);

					INT iRand = (INT)RAND(1000000) + 1;
					SetSerialKey(iRand * 999 + iRand * 99 + iRand);

					if(0 == GetSerialKey()) {
						SetSerialKey(1234567890);
					}

					CCommand kCommand = new CCommand();
					kCommand.SetOrder((UINT)PROTOCOL.INFO_SERVER);
					kCommand.SetExtra((UINT)EXTRA.NONE);
					Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_TYPE.CRYPT_NONE);
				}
			}

			public override bool
			Disconnect(Socket skSocket_) {
				if((INVALID_SOCKET != skSocket_) && (GetSocket() == skSocket_) && IsConnected() && (0 != GetSerialKey())) {
					SetSerialKey(0);

					CCommand kCommand = new CCommand();
					kCommand.SetOrder((UINT)PROTOCOL.ID_QUIT);
					kCommand.SetExtra((UINT)EXTRA.REQUEST);
					Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_TYPE.CRYPT_NONE);
					return true;
				} else {
					SetSerialKey(0);
					if(base.Disconnect(skSocket_)) {
						return true;
					}
				}
				return false;
			}

			public override void
			Disconnect() {
				if(isptr(GetSocket())) {
					TRACE("disconnect: socket: " + SOCKDESC(GetSocket()) + ", serial: " + GetSerialKey());
				} else {
					TRACE("disconnect: socket: INVALID_SOCKET, serial: " + GetSerialKey());
				}
				SetSerialKey(0);
				base.Disconnect(GetSocket());
			}

			public override bool
			KeepAlive() {
				if(IsConnected()) {
					if(0 == GetRecvPingTick()) {
						OUTPUT("[" + g_kTick.GetTime() + "] RECV PING: recv tick is 0");
						return false;
					}

					if(g_kTick.GetTick() > GetRecvPingTick() + iRECV_PING_TICK) {
						CPlayerEx kActor = g_kUnitMgr.GetPlayer((UINT)UNIT_TYPE.UNIT_PLAYER, GetRecvKey());
						if(isptr(kActor)) {
							if(isptr(kActor.GetNetIO())) {
								if(kActor.GetNetIO().GetConnector() != this) {
									if(0 < kActor.GetAid()) {
										OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: connector is wrong: [" + kActor.GetAid() + ":" + kActor.GetKey() + ":" + (INT)kActor.GetStatus() + "] delay: " + (g_kTick.GetTick() - GetRecvPingTick()) + ", addr : " + ConvertToString(GetRemoteAddress()));
									}
									return false;
								}

								OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: time is too late: [" + kActor.GetAid() + ":" + kActor.GetKey() + ":" + (INT)kActor.GetStatus() + "] delay: " + (g_kTick.GetTick() - GetRecvPingTick()) + ", addr : " + ConvertToString(GetRemoteAddress()));
								return false;
							} else {
								OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: Net IO is null: [" + kActor.GetAid() + ":" + kActor.GetKey() + "] : " + ConvertToString(GetRemoteAddress()));
								return false;
							}
						} else {
							OUTPUT("[" + g_kTick.GetTime() + "] [" + SOCKDESC(GetSocket()) + "] RECV PING: actor is null: ");
							return false;
						}
					}
				}
				return true;
			}

			public new static object	New()	{ return new CBackboneConnector(); }
		}

		public class CReliableConnector : CUdpConnector {
			public CReliableConnector() {}
			~CReliableConnector() {}

			public override void
			SetConnected() {
				base.SetConnected();

				if(INVALID_SOCKET != GetSocket()) {
					SetRelay((INT)RELAY_TYPE.RELAY_CLIENT);

					INT iRand = (INT)RAND(1000000) + 1;
					SetSerialKey(iRand * 999 + iRand * 99 + iRand);

					if(0 == GetSerialKey()) {
						SetSerialKey(1234567890);
					}

					CCommand kCommand = new CCommand();
					kCommand.SetOrder((UINT)PROTOCOL.INFO_SERVER);
					kCommand.SetExtra((UINT)EXTRA.NONE);
					Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_TYPE.CRYPT_NONE);
				}
			}

			public override bool
			Disconnect(Socket skSocket_) {
				if((INVALID_SOCKET != skSocket_) && (GetSocket() == skSocket_) && IsConnected() && (0 != GetSerialKey())) {
					SetSerialKey(0);

					CCommand kCommand = new CCommand();
					kCommand.SetOrder((UINT)PROTOCOL.ID_QUIT);
					kCommand.SetExtra((UINT)EXTRA.REQUEST);
					Queuing(kCommand, iCOMMAND_HEAD_SIZE, CRYPT_TYPE.CRYPT_NONE);
					return true;
				} else {
					SetSerialKey(0);
					if(base.Disconnect(skSocket_)) {
						return true;
					}
				}
				return false;
			}

			public override void
			Disconnect() {
				if(isptr(GetSocket())) {
					TRACE("disconnect: socket: " + SOCKDESC(GetSocket()) + ", serial: " + GetSerialKey());
				} else {
					TRACE("disconnect: socket: INVALID_SOCKET, serial: " + GetSerialKey());
				}
				SetSerialKey(0);
				base.Disconnect(GetSocket());
			}

			public new static object	New()	{ return new CReliableConnector(); }
		}

	}
}

/* EOF */
