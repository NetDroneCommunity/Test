/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Net;
using System.IO;

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
		public static CCfgMgr	g_kCfgMgr { get { return CCfgMgr.GetInstance(); } }

		public class CCfgMgr : CSingleton< CCfgMgr > {
			public CCfgMgr() {}
			~CCfgMgr() { Release(); }

			public bool			Initialize()					{ return true; }

			public bool
			Release() {
				m_iChannelIndex = 0;
				m_bHeaderCrypt = false;
				m_bReliableUdp = false;
				m_szLoginId = "";

				//m_bInput = false;

				return true;
			}

			public void			SetLoginId(string o)			{ m_szLoginId = o; }
			public string		GetLoginId()					{ return m_szLoginId; }

			public void			SetReliableUdp(bool o)			{ m_bReliableUdp = o; }
			public bool			IsReliableUdp()					{ return m_bReliableUdp; }

			public void			SetHeaderCrypt(bool o)			{ m_bHeaderCrypt = o; }
			public bool			IsHeaderCrypt()					{ return m_bHeaderCrypt; }

			public void			SetChannelIndex(INT o)			{ m_iChannelIndex = o; }
			public INT			GetChannelIndex()				{ return m_iChannelIndex; }

			//public bool			IsInput()						{ return m_bInput; }
			//public void			SetInput(bool o)				{ m_bInput = o; }

			private string		m_szLoginId = "";

			private bool		m_bReliableUdp = false;
			private bool		m_bHeaderCrypt = false;

			private INT			m_iChannelIndex = 0;

			//private bool		m_bInput = false;
		}
	}
}

/* EOF */
