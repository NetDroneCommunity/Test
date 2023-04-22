/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
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
		//public static bool
		//CMD_USER_STATUS(CUnit kActor_, CCommand kCommand_) {
		//	CPlayer kPlayer = (CPlayer)kActor_.GetTypeAs(UNIT_TYPE.UNIT_PLAYER);
		//	if(isptr(kPlayer)) {
		//		if(0 < kPlayer.GetAid()) {
		//			CRoomHandler kRoomHandler = kPlayer.GetRoomHandler();
		//			if(isptr(kRoomHandler)) {
		//				CRoom kRoom = kRoomHandler.GetRoom();
		//				if(isptr(kRoom)) {
		//					kPlayer.SetStatus((STATUS_TYPE)kCommand_.GetOption());

		//					kCommand_.SetExtra((UINT)EXTRA.OK);
		//					kPlayer.Launcher(kCommand_);

		//					TRACE("OK: key: " + kPlayer.GetKey() + ", aid: " + kPlayer.GetAid() + ", room id: " + kRoom.GetId() + ", status: " + kPlayer.GetStatus());

		//					kCommand_.SetOrder((UINT)PROTOCOL.OTHER_STATUS);
		//					SOtherStatusGsToCl tSData = new SOtherStatusGsToCl(true);
		//					tSData.actor = kPlayer.GetKey();

		//					INT iSize = Marshal.SizeOf(tSData);
		//					kCommand_.SetData(tSData, iSize);

		//					kRoom.Broadcast(kCommand_, iSize);
		//					return true;
		//				} else {
		//					OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] room is null: " + kActor_.GetKey());
		//					kCommand_.SetExtra((UINT)EXTRA.FAIL);
		//				}
		//			} else {
		//				OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] room handler is null: " + kActor_.GetKey());
		//				kCommand_.SetExtra((UINT)EXTRA.FAIL);
		//			}
		//		} else {
		//			OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] aid is 0: ");
		//			kCommand_.SetExtra((UINT)EXTRA.FAIL);
		//		}
		//	} else {
		//		OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] player is null: ");
		//		kCommand_.SetExtra((UINT)EXTRA.FAIL);
		//	}

		//	kActor_.Launcher(kCommand_);
		//	kActor_.Disconnect();
		//	return true;
		//}

		public static bool
		CMD_USER_CHAT(CUnit kActor_, CCommand kCommand_) {
			CPlayerEx kPlayer = (CPlayerEx)kActor_.GetTypeAs(UNIT_TYPE.UNIT_PLAYER);
			if(isptr(kPlayer)) {
				if(0 < kPlayer.GetAid()) {
					//if(CHAT_TYPE.CHAT_SHOUT == (CHAT_TYPE)kCommand_.GetMission()) {
					//	SUserChatClToGs tRData = (SUserChatClToGs)kCommand_.GetData(typeof(SUserChatClToGs));
					//	tRData.content[kCommand_.GetOption()] = (CHAR)'\0';

					//	CCommand kCommand = new CCommand();
					//	kCommand.SetOrder((UINT)PROTOCOL.OTHER_CHAT);
					//	kCommand.SetExtra((UINT)EXTRA.NONE);
					//	kCommand.SetMission(kCommand_.GetMission());
					//	kCommand.SetOption(kCommand_.GetOption());

					//	SOtherChatGsToCl tSData = new SOtherChatGsToCl(true);
					//	tSData.SetName(kPlayer.GetName());
					//	tSData.SetContent(tRData.content);

					//	INT iSize = Marshal.SizeOf(typeof(SOtherChatGsToCl)) - (iMAX_CHAT_LEN + 1) + (INT)kCommand.GetOption();
					//	kCommand.SetData(tSData, iSize);

					//	g_kChannelMgr.BroadcastAll(kCommand, iSize);

					//	TRACE("SHOUT: actor name: " + ConvertToString(kPlayer.GetName()) + ", content: " + ConvertToString(tSData.GetContent()));

					//	return true;
					//} else if(CHAT_TYPE.CHAT_ALERT == (CHAT_TYPE)kCommand_.GetMission()) {
					//	SUserChatClToGs tRData = (SUserChatClToGs)kCommand_.GetData(typeof(SUserChatClToGs));
					//	tRData.content[kCommand_.GetOption()] = (CHAR)'\0';

					//	CCommand kCommand = new CCommand();
					//	kCommand.SetOrder((UINT)PROTOCOL.OTHER_CHAT);
					//	kCommand.SetExtra((UINT)EXTRA.NONE);
					//	kCommand.SetMission(kCommand_.GetMission());
					//	kCommand.SetOption(kCommand_.GetOption());

					//	SOtherChatGsToCl tSData = new SOtherChatGsToCl(true);
					//	tSData.SetName(kPlayer.GetName());
					//	tSData.SetContent(tRData.content);

					//	INT iSize = Marshal.SizeOf(typeof(SOtherChatGsToCl)) - (iMAX_CHAT_LEN + 1) + (INT)kCommand.GetOption();
					//	kCommand.SetData(tSData, iSize);

					//	g_kChannelMgr.BroadcastAll(kCommand, iSize);

					//	TRACE("ALERT: actor name: " + ConvertToString(kPlayer.GetName()) + ", content: " + ConvertToString(tSData.GetContent()));

					//	return true;
					//} else if(CHAT_TYPE.CHAT_NOTIFY == (CHAT_TYPE)kCommand_.GetMission()) {
					//	SUserChatClToGs tRData = (SUserChatClToGs)kCommand_.GetData(typeof(SUserChatClToGs));
					//	tRData.content[kCommand_.GetOption()] = (CHAR)'\0';

					//	CCommand kCommand = new CCommand();
					//	kCommand.SetOrder((UINT)PROTOCOL.OTHER_CHAT);
					//	kCommand.SetExtra((UINT)EXTRA.NONE);
					//	kCommand.SetMission(kCommand_.GetMission());
					//	kCommand.SetOption(kCommand_.GetOption());

					//	SOtherChatGsToCl tSData = new SOtherChatGsToCl(true);
					//	tSData.SetName(kPlayer.GetName());
					//	tSData.SetContent(tRData.content);

					//	INT iSize = Marshal.SizeOf(typeof(SOtherChatGsToCl)) - (iMAX_CHAT_LEN + 1) + (INT)kCommand.GetOption();
					//	kCommand.SetData(tSData, iSize);

					//	g_kChannelMgr.BroadcastAll(kCommand, iSize);

					//	TRACE("NOTIFY: actor name: " + ConvertToString(kPlayer.GetName()) + ", content: " + ConvertToString(tSData.GetContent()));

					//	return true;
					//} else {
						SUserChatClToGs tRData = (SUserChatClToGs)kCommand_.GetData(typeof(SUserChatClToGs));
						tRData.content[kCommand_.GetOption()] = (CHAR)'\0';

						CCommand kCommand = new CCommand();
						kCommand.SetOrder((UINT)PROTOCOL.OTHER_CHAT);
						kCommand.SetExtra((UINT)EXTRA.NONE);
						//kCommand.SetMission(kCommand_.GetMission());
						//kCommand.SetOption(kCommand_.GetOption());

						SOtherChatGsToCl tSData = new SOtherChatGsToCl(true);
						tSData.SetName(kPlayer.GetName());
						tSData.SetContent(tRData.content);

						INT iSize = Marshal.SizeOf(typeof(SOtherChatGsToCl)) - (iMAX_CHAT_LEN + 1) + (INT)kCommand.GetOption();
						kCommand.SetData(tSData, iSize);

						//CRoomHandlerEx kRoomHandler = (CRoomHandlerEx)kPlayer.GetRoomHandler();
						//if(isptr(kRoomHandler)) {
						//	CRoomEx kRoom = (CRoomEx)kRoomHandler.GetRoom();
						//	if(isptr(kRoom)) {
						//		//kRoom.Broadcast(kCommand, iSize, kActor_);
						//		kRoom.Broadcast(kCommand, iSize);
						//		TRACE("NORMAL: channel: " + (kPlayer.GetChannelIndex() + 1) + ", room: " + kRoom.GetId() + ", actor name: " + ConvertToString(kPlayer.GetName()) + ", content: " + ConvertToString(tSData.GetContent()));

						//		return true;
						//	} else {
								g_kChannelMgr.Broadcast(kPlayer, kCommand, iSize);
								TRACE("NORMAL: channel: " + (kPlayer.GetChannelIndex() + 1) + ", actor name: " + ConvertToString(kPlayer.GetName()) + ", content: " + ConvertToString(tSData.GetContent()));

								return true;
						//	}
						//} else {
						//	OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] room handler is null: ");
						//	kCommand.SetExtra((UINT)EXTRA.FAIL);
						//}
					//}
				} else {
					OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] aid is 0: ");
					kCommand_.SetExtra((UINT)EXTRA.FAIL);
				}
			} else {
				OUTPUT("[" + g_kTick.GetTime() + ":" + kActor_.GetAid() + "] player is null: ");
				kCommand_.SetExtra((UINT)EXTRA.FAIL);
			}

			kActor_.Launcher(kCommand_);
			kActor_.Disconnect();
			return true;
		}

		public static void
		InitializeUserCommand() {
			//g_bfNativeLauncher[(INT)(PROTOCOL.USER_STATUS)] = new NativeLauncher(CMD_USER_STATUS);
			g_bfNativeLauncher[(INT)(PROTOCOL.USER_CHAT)] = new NativeLauncher(CMD_USER_CHAT);
		}
	}
}

/* EOF */
