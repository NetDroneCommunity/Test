/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
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
		public static bool
		SEND_ID_AUTHORIZE() {
			// 인증.
			if(0 >= g_kCfgMgr.GetLoginId().Length) {
				OUTPUT("login id is empty: ");
				return false;
			}

			if(0 > g_kCfgMgr.GetChannelIndex()) {
				OUTPUT("channel id is empty: ");
				return false;
			}

			CCommand kCommand = new CCommand();
			kCommand.SetOrder((UINT)PROTOCOL.ID_AUTHORIZE);
			kCommand.SetExtra((UINT)EXTRA.NONE);

			SIdAuthorizeClToGs tSData = new SIdAuthorizeClToGs(true);
			tSData.major_version = iSERVICE_MAJOR_VERSION;
			tSData.minor_version = iSERVICE_MINOR_VERSION;
			tSData.channel_index = (UINT)(g_kCfgMgr.GetChannelIndex());

			tSData.SetLoginId(ConvertToBytes(g_kCfgMgr.GetLoginId()));

			IPAddress[] kLocalAddr = Dns.GetHostAddresses(Dns.GetHostName());
			foreach(IPAddress kAddr in kLocalAddr) {
				if(kAddr.AddressFamily == AddressFamily.InterNetwork) {
					if(false == IPAddress.IsLoopback(kAddr)) {
						g_kNetMgr.GetConnector().SetLocalAddress(ConvertToBytes(kAddr.ToString()), 0);
					}
				}
			}

			tSData.local_ip = g_kNetMgr.GetConnector().GetLocalSinAddress();
			tSData.local_port = (UINT16)g_kNetMgr.GetConnector().GetLocalPort();

			INT iSize = Marshal.SizeOf(tSData);
			kCommand.SetData(tSData, iSize);

			if(0 > g_kNetMgr.Send(kCommand, iSize)) {
				OUTPUT("sending failed: ");
				return false;
			}

			OUTPUT((EXTRA)kCommand.GetExtra() + ", local ip: " + tSData.local_ip + ", local port: " + tSData.local_port + ": bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + iSize));
			return true;
		}

		public static bool
		SEND_ID_QUIT() {
			// 종료.
			// 게임 종료시 보냄.
			CCommand kCommand = new CCommand();
			kCommand.SetOrder((UINT)PROTOCOL.ID_QUIT);
			kCommand.SetExtra((UINT)EXTRA.NONE);

			if(0 > g_kNetMgr.Send(kCommand)) {
				OUTPUT("sending failed: ");
				return false;
			}
			OUTPUT("NONE: bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
			return true;
		}

		public static bool
		SEND_ID_PING() {
			// 서버로 보내는 PING.
			CCommand kCommand = new CCommand();
			kCommand.SetOrder((UINT)PROTOCOL.ID_PING);
			kCommand.SetExtra((UINT)EXTRA.NONE);

			if(0 > g_kNetMgr.Send(kCommand)) {
				OUTPUT("sending failed: ");
				return false;
			}
			OUTPUT("NONE: bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
			return true;
		}
	}
}

/* EOF */
