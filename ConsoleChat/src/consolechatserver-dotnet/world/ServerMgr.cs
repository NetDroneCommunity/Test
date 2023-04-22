/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
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
		public static CServerMgr g_kServerMgr { get { return CServerMgr.GetInstance(); } }

		public class CServerMgr : CSingleton<CServerMgr> {
			public CServerMgr() { }
			~CServerMgr() { }

			public bool
			Initialize() {
				return true;
			}

			public bool
			Release() {
				return true;
			}

			public void
			Update() {
				//
			}

			public void
			IncreasedUser() {
				g_kChannelMgr.IncreasedUser();
			}

			public void
			DecreasedUser() {
				g_kChannelMgr.DecreasedUser();
			}

			public bool
			InChannel(INT iIndex_, CPlayer kPlayer_) {
				if(g_kChannelMgr.InUser(iIndex_, kPlayer_)) {
					return true;
				}
				return false;
			}

			public void
			OutChannel(INT iIndex_, CPlayer kPlayer_) {
				g_kChannelMgr.OutUser(iIndex_, kPlayer_);
			}

			public bool
			IsAvailable() {
				return false;
			}

			public void
			Shutdown() {
			}
		}
	}
}

/* EOF */
