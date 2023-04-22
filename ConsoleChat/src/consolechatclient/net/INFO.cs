/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Text;
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
		CMD_INFO_SERVER(CCommand kCommand_) {
			if(EXTRA.OK == (EXTRA)kCommand_.GetExtra()) {
				SInfoServerGsToCl tRData = (SInfoServerGsToCl)kCommand_.GetData(typeof(SInfoServerGsToCl));

				g_kNetMgr.GetConnector().SetSendKey(tRData.key);
				g_kNetMgr.GetConnector().SetSerialKey(tRData.serial);

				OUTPUT("OK: serial: " + tRData.serial + ", key: " + tRData.key + ", bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + Marshal.SizeOf(tRData)));
			} else {
				OUTPUT("FAIL: bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));
			}
			return true;
		}

		public static bool
		CMD_INFO_CHANNEL(CCommand kCommand_) {
			OUTPUT("CHECK: order: " + kCommand_.GetOrder() + ", extra: " + kCommand_.GetExtra() + ", option: " + kCommand_.GetOption() + ", mission: " + kCommand_.GetMission() + ", bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE));

			return true;
		}

		public static void
		InitializeInfoCommand() {
			g_bfNativeLauncher[(INT)(PROTOCOL.INFO_SERVER)] = new NativeLauncher(CMD_INFO_SERVER);
			g_bfNativeLauncher[(INT)(PROTOCOL.INFO_CHANNEL)] = new NativeLauncher(CMD_INFO_CHANNEL);
		}
	}
}

/* EOF */
