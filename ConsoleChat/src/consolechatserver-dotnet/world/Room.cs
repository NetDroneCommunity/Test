/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Collections.Generic;

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
		public class CRoom {
			public CRoom() {}
			~CRoom() {}

			public virtual bool
			Initialize() {
				Clear();
				return true;
			}

			public virtual bool
			Release() {
				for(UINT i = 0; i < GetTopCount(); ++i) {
					if(isptr(m_kMembers[i])) {
						m_kMembers[i] = null;
					} else {
						TRACE("critical error: unit[" + i + "] is null: " + GetTopCount());
					}
				}

				Array.Clear(m_szName, 0, m_szName.Length);

				m_uiTopCount = 0;

				m_usId = 0;
				m_bOffset = false;
				m_uiStageId = 0;
				m_uiMaxUser = 0;

				return true;
			}

			public virtual void
			Clear() {
				for(UINT i = 0; i < GetTopCount(); ++i) {
					CPlayer kPlayer = m_kMembers[i];
					if(isptr(kPlayer)) {
						if(isptr(kPlayer.GetRoomHandler())) {
							kPlayer.GetRoomHandler().Clear();
						}
					}
				}

				for(INT i = 0; i < iMAX_ROOM_MEMBERS; ++i) {
					m_kMembers[i] = null;
				}

				Array.Clear(m_szName, 0, m_szName.Length);

				m_uiTopCount = 0;

				m_bOffset = false;
				m_uiStageId = 0;
				m_uiMaxUser = 0;
			}

			public virtual bool		Update()					{ return true; }

			public virtual bool		Create(CUnit kUnit_)		{ return true; }

			public virtual bool		Join(CUnit kUnit_)			{ return true; }

			public virtual bool		Leave(CUnit kUnit_)			{ return true; }

			public virtual bool		ChangeLeader(CUnit kUnit_)	{ return true; }

			public virtual bool		Broadcast(CCommand kCommand_, INT iSize_ =0, CUnit kActor_ =null, CUnit kTarget_ =null) { return true; }

			public CPlayer
			GetMember(UINT o) {
				if(o < (UINT)(iMAX_ROOM_MEMBERS)) {
					return m_kMembers[(INT)o];
				}
				return null;
			}

			public CPlayer
			GetLeader() {
				return m_kMembers[0];
			}

			public CHAR[]
			GetName() {
				return m_szName;
			}

			public void
			SetName(CHAR[] o) {
				INT iLength = o.Length;
				if(iROOM_NAME_LEN < iLength) {
					iLength = iROOM_NAME_LEN;
				}
				Array.Copy(o, 0, m_szName, 0, iLength);
				m_szName[iLength] = (CHAR)('\0');
			}

			public void				SetId(UINT o)			{ m_usId = (UINT16)o; }
			public UINT				GetId()					{ return m_usId; }

			public INT
			GetIndex() {
				return (INT)((UINT16)(m_usId) & 0x03FF);
			}

			public INT
			GetChannelIndex() {
				//return (UINT)((m_usId >> 10) & 0x3F) - 1;
				return (INT)((UINT32)(m_usId >> 10) - 1);
			}

			public void				SetDoing(bool o)		{ m_bOffset = o; }
			public bool				IsDoing()				{ return m_bOffset; }

			public bool				IsAvailable()			{ return (0 < m_uiTopCount); }

			public UINT				GetStageId()			{ return m_uiStageId; }
			public void				SetStageId(UINT o)		{ m_uiStageId = o; }

			public void
			SetMaxUser(UINT o) {
				if((UINT)(iMAX_ROOM_MEMBERS) > o) {
					m_uiMaxUser = o;
				} else {
					m_uiMaxUser = iMAX_ROOM_MEMBERS;
				}
			}

			public UINT				GetMaxUser()			{ return m_uiMaxUser; }

			public UINT				GetTopCount()			{ return m_uiTopCount; }

			protected void			IncreasedTopCount()		{ ++m_uiTopCount; }
			protected void			DecreasedTopCount()		{ --m_uiTopCount; }

			protected void			SetTopCount(UINT o)		{ m_uiTopCount = o; }
			protected bool			InRange(UINT o)			{ return (o < m_uiTopCount); }

			protected CPlayer[]	m_kMembers = new CPlayer[iMAX_ROOM_MEMBERS];
			protected UINT		m_uiTopCount = 0;

			private UINT16		m_usId = 0;
			private UINT		m_uiStageId = 0;
			private bool		m_bOffset = false;
			private UINT		m_uiMaxUser = 0;

			private CHAR[]		m_szName = new CHAR[iROOM_NAME_LEN + 1];
		}
	}
}

/* EOF */
