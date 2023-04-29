/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Collections;
using System.Text;
using System.Net.Sockets;

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
		public class CFrameworkThread : CSystemThread {
			public CFrameworkThread(CFramework o, INT q=0) {
				m_kFramework = o;
				m_iSleepTime = q;

				m_kAttribute.SetName("CFrameworkThread");
				// Linux 에서 1024로 설정시 오류 발생
				//m_kAttribute.SetStackSize(1024);
				m_kAttribute.SetStackSize(0);
				m_kAttribute.SetJoinable(true);

				Initialize(m_kAttribute);
			}

			~CFrameworkThread() {}

			public void					SetName(string o)	{ if(isptr(o)) { m_kAttribute.SetName(o); } }
			public string				GetName()			{ return m_kAttribute.GetName(); }

			protected override object
			GetHandler() {
				for( ; ; ) {
					if(false == m_bCancel) {
						GetFramework().Update();
					} else {
						break;
					}
					Thread.Sleep(m_iSleepTime);
				}

				return null;
			}

			protected CFramework		GetFramework()		{ return m_kFramework; }

			protected CFramework		m_kFramework = null;
			private INT					m_iSleepTime;

			private CThreadAttribute	m_kAttribute = new CThreadAttribute();
		}
	}
}

/* EOF */
