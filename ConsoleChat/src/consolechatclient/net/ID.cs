/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Text;
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
		public static bool
		CMD_ID_AUTHORIZE(CCommand kCommand_) {
			if(EXTRA.OK == (EXTRA)kCommand_.GetExtra()) {
				SIdAuthorizeGsToCl tRData = (SIdAuthorizeGsToCl)kCommand_.GetData(typeof(SIdAuthorizeGsToCl));
				g_kNetMgr.GetConnector().SetPublicAddress(tRData.public_ip, tRData.public_port);
				g_kTick.Reset(tRData.tick * 10);
				g_kNetMgr.SetSendPingServerTick(g_kTick.GetTick());

				OUTPUT("OK: key: " + tRData.key + ", aid: " + tRData.aid + ", name: " + ConvertToString(tRData.GetName()) + ", public ip: " + ConvertToString(g_kNetMgr.GetConnector().GetPublicAddress()) + ", public port: " + g_kNetMgr.GetConnector().GetPublicPort() + ", local ip: " + ConvertToString(g_kNetMgr.GetConnector().GetLocalAddress()) + ", local port: " + g_kNetMgr.GetConnector().GetLocalPort() + ", bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + Marshal.SizeOf(tRData)));
				return true;
			} else if(EXTRA.TIMEOUT == (EXTRA)kCommand_.GetExtra()) {
				SIdAuthorizeGsToCl tRData = (SIdAuthorizeGsToCl)kCommand_.GetData(typeof(SIdAuthorizeGsToCl));
				g_kNetMgr.GetConnector().SetPublicAddress(tRData.public_ip, tRData.public_port);
				g_kTick.Reset(tRData.tick * 10);
				g_kNetMgr.SetSendPingServerTick(g_kTick.GetTick());

				OUTPUT("TIMEOUT: key: " + tRData.key + ", aid: " + tRData.aid + ", name: " + ConvertToString(tRData.GetName()) + ", public ip: " + ConvertToString(g_kNetMgr.GetConnector().GetPublicAddress()) + ", public port: " + g_kNetMgr.GetConnector().GetPublicPort() + ", local ip: " + ConvertToString(g_kNetMgr.GetConnector().GetLocalAddress()) + ", local port: " + g_kNetMgr.GetConnector().GetLocalPort() + ", bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + Marshal.SizeOf(tRData)));
				return true;
			} else if(EXTRA.BUSY == (EXTRA)kCommand_.GetExtra()) {
				if(kCommand_.GetOption() == 0) {
					// 중복 접속.
					OUTPUT("BUSY: duplicate name, bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
				} else if(kCommand_.GetOption() == 1) {
					// 채널 꽉참
					OUTPUT("BUSY: channel is full, bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
				}
			} else if(EXTRA.DENY == (EXTRA)kCommand_.GetExtra()) {
				// 서버에서 연결 거부.
				OUTPUT("DENY: bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
			} else if(EXTRA.DATA_ERROR == (EXTRA)kCommand_.GetExtra()) {
				// 다른 버전.
				OUTPUT("DATA_ERROR: version is not valid: " + iSERVICE_MAJOR_VERSION + "." + iSERVICE_MINOR_VERSION + "." + iSERVICE_PATCH_VERSION + ", bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
			} else if(EXTRA.FAIL == (EXTRA)kCommand_.GetExtra()) {
				// 인증 실패.
				OUTPUT("FAIL: bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
			}
			return true;
		}

		public static bool
		CMD_ID_PONG(CCommand kCommand_) {
			if(EXTRA.OK == (EXTRA)kCommand_.GetExtra()) {
				// 스테이지 플레이 중이 아닐 경우만 서버 Tick 동기화.
				SIdPongGsToCl tRData = (SIdPongGsToCl)kCommand_.GetData(typeof(SIdPongGsToCl));
				if(0 < tRData.tick) {
					g_kTick.Reset(tRData.tick * 10);
				}

				FLOAT fDelaySec = 0;
				if(0 == g_kNetMgr.GetDelayPingTick()) {
					g_kNetMgr.SetDelayPingTick(g_kTick.GetTick());
				} else {
					fDelaySec = (FLOAT)((FLOAT)(g_kTick.GetTick() - g_kNetMgr.GetDelayPingTick()) - (FLOAT)(iMAX_PING_LATENCY_TICK)) / 100;
					g_kNetMgr.SetDelayPingTick(g_kTick.GetTick());
				}

				g_kNetMgr.SetInput(true);

				OUTPUT("OK: tick: " + tRData.tick + ", delay: " + String.Format("{0:F2}", fDelaySec) + " sec, bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + Marshal.SizeOf(tRData)));
			}
			return true;
		}

		public static bool
		CMD_ID_QUIT(CCommand kCommand_) {
			OUTPUT("order: bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));

			g_kNetMgr.DisconnectAll();
			return true;
		}

		public static void
		InitializeIdCommand()	{
			g_bfNativeLauncher[(INT)(PROTOCOL.ID_AUTHORIZE)] = new NativeLauncher(CMD_ID_AUTHORIZE);
			g_bfNativeLauncher[(INT)(PROTOCOL.ID_PONG)] = new NativeLauncher(CMD_ID_PONG);
			g_bfNativeLauncher[(INT)(PROTOCOL.ID_QUIT)] = new NativeLauncher(CMD_ID_QUIT);
		}
	}
}

/* EOF */
