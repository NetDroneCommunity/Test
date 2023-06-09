﻿/*
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
		public class CRoomHandlerEx : CRoomHandler {
			public CRoomHandlerEx(CUnit o) : base(o) {}
			~CRoomHandlerEx() {}

			public override bool
			Join() {
				CPlayerEx kBody = (CPlayerEx)GetBody().GetTypeAs(UNIT_TYPE.UNIT_PLAYER);
				if(isptr(kBody)) {
					CRoom kRoom = GetRoom();
					if(isptr(kRoom)) {
						TRACE("in: [" + kBody.GetAid() + ":" + kBody.GetKey() + "]");
						kBody.SetStatus(STATUS_TYPE.STATUS_WAITING);

						g_kChannelMgr.GetRoomMemberList(kBody);

						CConnector kConnector = kBody.GetNetIO().GetConnector();
						if(isptr(kConnector)) {
							CCommand kCommand = new CCommand();
							kCommand.SetOrder((UINT)PROTOCOL.ROOM_JOIN_OTHER);
							kCommand.SetExtra((UINT)EXTRA.OK);

							SRoomJoinOtherGsToCl tSData = new SRoomJoinOtherGsToCl(true);
							tSData.actor = kBody.GetKey();
							tSData.SetName(kBody.GetName());

							tSData.public_ip = kConnector.GetPublicSinAddress();
							tSData.public_port = (UINT16)kConnector.GetPublicPort();
							tSData.local_ip = kConnector.GetLocalSinAddress();
							tSData.local_port = (UINT16)kConnector.GetLocalPort();

							TRACE("[" + ConvertToString(kBody.GetName()) + ":" + kBody.GetKey() + "] : public addr: " + ConvertToString(kConnector.GetPublicAddress()) + ":" + kConnector.GetPublicPort() + ", local addr: " + ConvertToString(kConnector.GetLocalAddress()) + ":" + kConnector.GetLocalPort());

							INT iSize = Marshal.SizeOf(typeof(SRoomJoinOtherGsToCl));
							kCommand.SetData(tSData, iSize);

							Broadcast(kCommand, iSize, kBody);
						}

						return true;
					} else {
						OUTPUT("critical error: room is null: ");
					}
				} else {
					OUTPUT("critical error: unit is not player: ");
				}
				return false;
			}

			public override void
			Leave(CUnit kLeader_ =null) {
				CCommand kCommand = new CCommand();
				kCommand.SetOrder((UINT)PROTOCOL.ROOM_LEAVE_OTHER);
				GetBody().SetStatus((UINT)STATUS_TYPE.STATUS_EXIT);

				SRoomLeaveOtherGsToCl tSData = new SRoomLeaveOtherGsToCl(true);
				tSData.actor = GetBody().GetKey();

				if(isptr(kLeader_)) {
					kCommand.SetExtra((UINT)EXTRA.CHECK);
					kLeader_.SetStatus(STATUS_TYPE.STATUS_READY);

					tSData.leader = kLeader_.GetKey();

					TRACE("ROOM_LEAVE_OTHER: CHECK: body: " + GetBody().GetKey() + ", leader: " + kLeader_.GetKey());

					INT iSize = Marshal.SizeOf(typeof(SRoomLeaveOtherGsToCl));
					kCommand.SetData(tSData, iSize);

					Broadcast(kCommand, iSize, GetBody());
				} else {
					kCommand.SetExtra((UINT)EXTRA.OK);

					TRACE("ROOM_LEAVE_OTHER: OK: body: " + GetBody().GetKey());

					INT iSize = (Marshal.SizeOf(typeof(SRoomLeaveOtherGsToCl)) - Marshal.SizeOf(typeof(UINT32)));
					kCommand.SetData(tSData, iSize);

					Broadcast(kCommand, iSize, GetBody());
				}

				Clear();
			}

			public override void
			ChangeLeader() {
				CCommand kCommand = new CCommand();
				kCommand.SetOrder((UINT)PROTOCOL.ROOM_INFO);

				SRoomMemberLeaderGsToCl tSData = new SRoomMemberLeaderGsToCl(true);
				tSData.leader = GetBody().GetKey();

				kCommand.SetExtra((UINT)EXTRA.MODIFY);

				TRACE("ROOM_INFO: MODIFY: leader: " + GetBody().GetKey());

				INT iSize = Marshal.SizeOf(typeof(SRoomMemberLeaderGsToCl));
				kCommand.SetData(tSData, iSize);

				Broadcast(kCommand, iSize);
			}

			public override void
			Clear() {
				SetRoom(null);
				SetOffset(-1);

				GetBody().SetStatus(STATUS_TYPE.STATUS_EXIT);
			}
			
			public override bool
			Broadcast(CCommand kCommand_, INT iSize_ =0, CUnit kActor_ =null, CUnit kTarget_ =null) {
				if(isptr(GetRoom())) {
					return GetRoom().Broadcast(kCommand_, iSize_, kActor_, kTarget_);
				}
				return false;
			}
		}
	}
}

/* EOF */
