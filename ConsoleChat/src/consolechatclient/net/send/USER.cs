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
		SEND_USER_CHAT(string szConetnt_) {
			CCommand kCommand = new CCommand ();
			kCommand.SetOrder((UINT)PROTOCOL.USER_CHAT);
			kCommand.SetExtra((UINT)EXTRA.NONE);
			//kCommand.SetMission(0);

			SUserChatClToGs tSData = new SUserChatClToGs(true);
			tSData.SetContent(ConvertToBytes(szConetnt_));
			kCommand.SetOption((UINT)ConvertToBytes(szConetnt_).Length);

			INT iSize = Marshal.SizeOf(tSData);
			kCommand.SetData(tSData, iSize);

			iSize = iSize - (iMAX_CHAT_LEN+1) + (INT)kCommand.GetOption();

			if(0 > g_kNetMgr.Send(kCommand, iSize)) {
				OUTPUT("SEND_USER_CHAT: NONE: sending failed: ");
				return false;
			}

			//CONSOLE(ConvertToString(g_kUnitMgr.GetMainPlayer().GetName()) + ": " + ConvertToString(tSData.GetContent()) + ", bytes: " + (iTCP_PACKET_HEAD_SIZE + iCOMMAND_HEAD_SIZE + (Marshal.SizeOf(tSData) - (iMAX_CHAT_LEN+1)) + (INT)kCommand.GetOption()));

			return true;
		}
	}
}

/* EOF */
