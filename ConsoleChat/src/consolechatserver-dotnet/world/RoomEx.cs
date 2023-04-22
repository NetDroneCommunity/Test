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
		public class CRoomEx : CRoom {
			public CRoomEx() {}
			~CRoomEx() { /* Release(); */ }

			public override bool
			Initialize() {
				if(base.Initialize()) {
					return true;
				}
				return false;
			}

			public override bool
			Release() {
				if(base.Release()) {
					return true;
				}
				return false;
			}

			public override void
			Clear() {
				base.Clear();
			}

			public override bool
			Update() {
				return false;
			}

			public override bool
			Create(CUnit kUnit_) {
				CRoomHandler kRoomHandler = kUnit_.GetRoomHandler();
				if(isptr(kRoomHandler)) {
					if(null == kRoomHandler.GetRoom()) {
						m_kMembers[0] = (CPlayer)kUnit_;

						kRoomHandler.SetRoom(this);
						kRoomHandler.SetOffset(0);
						kUnit_.SetStatus(STATUS_TYPE.STATUS_READY);

						IncreasedTopCount();

						CCommand kCommand = new CCommand();
						kCommand.SetOrder((UINT)PROTOCOL.ROOM_INFO);
						kCommand.SetExtra((UINT)EXTRA.IN);

						SRoomInfo tOtherData = new SRoomInfo(true);
						tOtherData.id = GetId();
						tOtherData.max = GetMaxUser();
						tOtherData.stage_id = GetStageId();
						if(IsDoing()) {
							tOtherData.offset = 1;
						} else {
							tOtherData.offset = 0;
						}
						tOtherData.SetName(GetName());

						INT iSize = Marshal.SizeOf(tOtherData);
						kCommand.SetData(tOtherData, iSize);

						SRoomKey tKey = new SRoomKey((UINT16)GetId());
						g_kChannelMgr.BroadcastChannel(tKey.channel, kCommand, iSize, (CPlayer)kUnit_);

						return true;
					} else {
						TRACE("critical error: in room");
					}
				} else {
					OUTPUT("critical error: handler is null: ");
				}
				return false;
			}

			public override bool
			Join(CUnit kUnit_) {
				CCommand kCommand = new CCommand();
				kCommand.SetOrder((UINT)PROTOCOL.ROOM_JOIN);

				if(0 < GetTopCount() && (GetTopCount() < GetMaxUser())) {
					CRoomHandler kRoomHandler = kUnit_.GetRoomHandler();
					if(isptr(kRoomHandler)) {
						if(null == kRoomHandler.GetRoom()) {
							SRoomKey tKey = new SRoomKey((UINT16)GetId());
							CPlayer kPlayer = (CPlayer)kUnit_;
							if(isptr(kPlayer)) {
								if(tKey.channel != kPlayer.GetChannelIndex()) {
									g_kChannelMgr.InUser(tKey.channel, kPlayer);
								}
							}

							kRoomHandler.SetRoom(this);
							kRoomHandler.SetOffset((INT)GetTopCount());
							// Join 한 캐릭터 정보를 클라이언트에 보냅니다.
							if(kRoomHandler.Join()) {
								m_kMembers[GetTopCount()] = (CPlayer)kUnit_;
								IncreasedTopCount();

								TRACE("Join OK: [" + kUnit_.GetAid() + " (" + kUnit_.GetKey() + ")] : " + GetTopCount() + " < " + iMAX_ROOM_MEMBERS);

								kCommand.SetExtra((UINT)EXTRA.OK);
								SRoomJoinGsToCl tSData = new SRoomJoinGsToCl(true);
								tSData.id = GetId();

								tSData.max = GetMaxUser();
								tSData.stage_id = GetStageId();
								if(IsDoing()) {
									tSData.offset = 1;
								} else {
									tSData.offset = 0;
								}
								tSData.SetName(GetName());

								INT iSize = Marshal.SizeOf(tSData);
								kCommand.SetData(tSData, iSize);

								kUnit_.Launcher(kCommand, iSize);

								return true;
							} else {
								kRoomHandler.Clear();
								kCommand.SetExtra((UINT)EXTRA.DENY);
								TRACE("critical error: in room");
							}
						} else {
							kCommand.SetExtra((UINT)EXTRA.DENY);
							TRACE("critical error: in room");
						}
					} else {
						TRACE("critical error: RoomHandler is null: ");
						kCommand.SetExtra((UINT)EXTRA.FAIL);
					}
				} else {
					TRACE("error: room is full. or empty: " + GetTopCount() + "[" + iMAX_ROOM_MEMBERS + "] " + kUnit_.GetKey());
					kCommand.SetExtra((UINT)EXTRA.FULL);
				}
				kUnit_.Launcher(kCommand);

				return false;
			}

			public override bool
			Leave(CUnit kUnit_) {
				CRoomHandler kRoomHandler = kUnit_.GetRoomHandler();
				if(isptr(kRoomHandler)) {
					UINT uiOutIndex = (UINT)kRoomHandler.GetOffset();
					if(InRange(uiOutIndex)) {
						if(kUnit_ == m_kMembers[uiOutIndex]) {
							DecreasedTopCount();

							if(0 >= GetTopCount()) {
								TRACE("room clear: ");
								g_kChannelMgr.DeleteRoom((UINT16)GetId());
								kRoomHandler.Clear();

								CCommand kCommand = new CCommand();
								kCommand.SetOrder((UINT)PROTOCOL.ROOM_INFO);
								kCommand.SetExtra((UINT)EXTRA.OUT);

								SRoomInfo tOtherData = new SRoomInfo(true);
								tOtherData.id = (UINT16)GetId();

								SRoomKey tKey = new SRoomKey((UINT16)GetId());

								INT iSize = Marshal.SizeOf(typeof(UINT32));
								kCommand.SetData(tOtherData, iSize);

								g_kChannelMgr.BroadcastChannel(tKey.channel, kCommand, iSize, (CPlayer)kUnit_);
							} else {
								if(uiOutIndex == GetTopCount()) {
									m_kMembers[GetTopCount()] = null;
									kRoomHandler.Leave();
									TRACE("out unit index: " + GetTopCount());
								} else {
									CRoomHandler kNewRoomHandler = null;
									if(kUnit_ == m_kMembers[0]) {
										// 방장이 나갈 경우.
										// 응답 지연시간 비교로 가장빠른 맴버를 방장으로 정하는 코드를 넣으려면 여기서합니다.
										// 아래는 무조건 다음 맴버로 고정하거나 종료처리됩니다.
										UINT uiIndex = 1;

										if(3 > GetTopCount()) {
											if(2 != GetTopCount()) {
												uiIndex = 0;
											}
										}

										if((0 < uiIndex) && (uiIndex <= GetTopCount())) {
											m_kMembers[0] = m_kMembers[uiIndex];
											m_kMembers[uiIndex] = null;
											kNewRoomHandler = m_kMembers[0].GetRoomHandler();
											if(isptr(kNewRoomHandler)) {
												kNewRoomHandler.SetOffset(0);
											}

											if(uiIndex < GetTopCount()) {
												m_kMembers[uiIndex] = m_kMembers[GetTopCount()];
												m_kMembers[GetTopCount()] = null;
												kNewRoomHandler = m_kMembers[uiIndex].GetRoomHandler();
												if(isptr(kNewRoomHandler)) {
													kNewRoomHandler.SetOffset((INT)uiIndex);
												}
											}

											TRACE("Leave OK: ");
											TRACE("leader: out unit: name: " + ConvertToString(kUnit_.GetName()));
											TRACE("leader: in unit: name: " + ConvertToString(m_kMembers[0].GetName()) + " (" + uiIndex + " -> 0)");
											TRACE("leader: move unit: name: " + ConvertToString(m_kMembers[uiIndex].GetName()) + " (" + GetTopCount() + " -> " + uiIndex + ")");

											// 빠져나간 유닛과 새로운 방장을 함께 전달할경우 각 클라이언트들은 동일한 갱신이 가능합니다.
											kRoomHandler.Leave(m_kMembers[0]);
										} else {
											// 방안에 아무도 없음.

											m_kMembers[0] = m_kMembers[GetTopCount()];
											m_kMembers[GetTopCount()] = null;

											kNewRoomHandler = m_kMembers[0].GetRoomHandler();
											if(isptr(kNewRoomHandler)) {
												kNewRoomHandler.SetOffset(0);
											}

											TRACE("Leave OK: ");
											kRoomHandler.Leave(m_kMembers[0]);
										}
										return true;
									}

									TRACE("switch a member position in room: " + uiOutIndex + " <-> " + GetTopCount());
									m_kMembers[uiOutIndex] = m_kMembers[GetTopCount()];
									m_kMembers[GetTopCount()] = null;
									kNewRoomHandler = m_kMembers[uiOutIndex].GetRoomHandler();
									if(isptr(kNewRoomHandler)) {
										kNewRoomHandler.SetOffset((INT)uiOutIndex);
									}
									kRoomHandler.Leave();
								}
							}
							return true;
						}
					} else {
						OUTPUT("critical error: offset is error: ");
					}
				}
				return false;
			}

			public override bool
			ChangeLeader(CUnit kUnit_) {
				CRoomHandler kRoomHandler = kUnit_.GetRoomHandler();
				if(isptr(kRoomHandler)) {
					UINT uiIndex = (UINT)kRoomHandler.GetOffset();
					if(InRange(uiIndex)) {
						if(kUnit_ == m_kMembers[uiIndex]) {
							if((0 < uiIndex) && (uiIndex <= GetTopCount())) {
								CPlayer kLeader = m_kMembers[uiIndex];
								if(isptr(kLeader)) {
									m_kMembers[uiIndex] = m_kMembers[0];
									m_kMembers[0] = kLeader;
									CRoomHandler kNewRoomHandler = m_kMembers[0].GetRoomHandler();
									if(isptr(kNewRoomHandler)) {
										kNewRoomHandler.SetOffset(0);
									}
									CRoomHandler kOldRoomHandler = m_kMembers[uiIndex].GetRoomHandler();
									if(isptr(kOldRoomHandler)) {
										kOldRoomHandler.SetOffset((INT)uiIndex);
									}

									TRACE("Change OK: ");
									TRACE("leader: change unit: name: " + ConvertToString(m_kMembers[0].GetName()) + " (" + uiIndex + " -> 0)");

									kRoomHandler.ChangeLeader();
								}
							}
							return true;
						}
					} else {
						OUTPUT("critical error: offset is error: ");
					}
				}
				return false;
			}

			public override bool
			Broadcast(CCommand kCommand_, INT iSize_ =0, CUnit kActor_ =null, CUnit kTarget_ =null) {
				if(0 < GetTopCount()) {
					CUnit kUnit = null;
					for(UINT i = 0; i < GetTopCount(); ++i) {
						kUnit = m_kMembers[i];
						if(isptr(kUnit)) {
							if((kUnit != kActor_) && (kUnit != kTarget_)) {
								kUnit.Launcher(kCommand_, iSize_);
							}
						} else {
							OUTPUT("critical error: unit[" + i + "] is null: " + GetTopCount());
						}
					}
					return true;
				} else {
					OUTPUT("room is empty");
				}
				return false;
			}

			public static object	New()	{ return new CRoomEx(); }
		}
	}
}

/* EOF */
