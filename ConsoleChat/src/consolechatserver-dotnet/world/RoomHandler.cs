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
		public class CRoomHandler {
			public CRoomHandler(CUnit o) { m_kBody = o; }
			~CRoomHandler() {}

			public virtual bool	Join()						{ return true; }

			public virtual void	Leave(CUnit kLeader_ =null)	{}

			public virtual void	ChangeLeader()				{}

			public virtual void	Clear()						{}

			public void			SetOffset(INT o)			{ m_iOffset = o; }
			public INT			GetOffset()					{ return m_iOffset; }

			public void			SetRoom(CRoom o)			{ m_kRoom = o; }
			public CRoom		GetRoom()					{ return m_kRoom; }

			public bool			InRoom()					{ return (isptr(GetRoom())); }

			public virtual bool	Broadcast(CCommand kCommand_, INT iSize_ =0, CUnit kActor_ =null, CUnit kTarget_ =null)	{ return true; }

			protected CUnit		GetBody()					{ return m_kBody; }

			private CUnit		m_kBody = null;
			private CRoom		m_kRoom = null;
			private INT			m_iOffset = -1;
		}
	}
}

/* EOF */
