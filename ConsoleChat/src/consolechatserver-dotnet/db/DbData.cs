/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

using System;
using System.Linq;
using System.Net;

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
		public struct SChannelInfo {
			public SChannelInfo(bool o) : this() {
				if(o) {
					id = 0;
					max_user = 0;
					user = 0;
					index = 0;
					server_id = 0;
					name = "";
				}
			}

			public SChannelInfo(SChannelInfo o) : this() {
				id = o.id;
				max_user = o.max_user;
				user = o.user;
				index = o.index;
				server_id = o.server_id;
				name = o.name;
			}

			public void
			Clear() {
				id = 0;
				max_user = 0;
				user = 0;
				index = 0;
				server_id = 0;
				name = "";
			}

			public UINT		GetId()				{ return id; }
			public void		SetId(UINT o)		{ id = o; }

			public UINT		GetMaxUser()		{ return max_user; }
			public void		SetMaxUser(INT o)	{ max_user = (UINT)o; }

			public UINT		GetUser()			{ return user; }
			public void		SetUser(INT o)		{ user = (UINT)o; }

			public void		IncreasedUser()		{ ++user; }
			public void		DecreasedUser()		{ if(0 < user) --user; else OUTPUT("critical error: user count: "); }

			public UINT		GetIndex()			{ return index; }
			public void		SetIndex(UINT o)	{ index = o; }

			public UINT		GetServerId()		{ return server_id; }
			public void		SetServerId(UINT o) { server_id = o; }

			public string	GetName()			{ return name; }
			public void		SetName(string o)	{ name = o; }

			public UINT		id;
			public UINT		max_user;
			public UINT		user;

			public UINT		index;
			public UINT		server_id;

			public string	name;
		}

		public struct SServerInfo {
			public SServerInfo(bool o) : this() {
				if(o) {
					id = 0;
					limited_user = 0;
					max_user = 0;
					user = 0;
					max_channel = 0;

					permission = PERMISSION_TYPE.PERMISSION_REJECT;
					status = SERVER_STATUS.SERVER_SHUTDOWN;
					server_type = SERVER_TYPE.SERVER_AUTH;

					major_version = 0;
					minor_version = 0;
					patch_version = 0;

					delay_ping = 0;

					channel_info = (new SChannelInfo[iMAX_CHANNEL_LIST]).Select(x => new SChannelInfo(true)).ToArray();
					addr_in = new IPEndPoint(IPAddress.Any, 0);

					option1 = "";
					option2 = "";
					domain = "";
					name = "";
				}
			}

			public SServerInfo(SServerInfo o) : this() {
				id = o.id;
				limited_user = o.limited_user;
				max_user = o.max_user;
				user = o.user;
				max_channel = o.max_channel;

				permission = o.permission;
				status = o.status;
				server_type = o.server_type;

				major_version = o.major_version;
				minor_version = o.minor_version;
				patch_version = o.patch_version;

				delay_ping = o.delay_ping;

				channel_info = (new SChannelInfo[iMAX_CHANNEL_LIST]).Select(x => new SChannelInfo(true)).ToArray();
				addr_in = new IPEndPoint(IPAddress.Any, 0);

				for(INT i = 0; i < iMAX_CHANNEL_LIST; ++i) {
					channel_info[i].id = o.channel_info[i].id;
					channel_info[i].max_user = o.channel_info[i].max_user;
					channel_info[i].user = o.channel_info[i].user;
					channel_info[i].index = o.channel_info[i].index;
					channel_info[i].server_id = o.channel_info[i].server_id;
					channel_info[i].name = o.channel_info[i].name;
				}

				addr_in.Address = o.addr_in.Address;
				addr_in.Port = o.addr_in.Port;

				option1 = o.option1;
				option2 = o.option2;
				domain = o.domain;
				name = o.name;
			}

			public void
			Clear() {
				id = 0;
				limited_user = 0;
				max_user = 0;
				user = 0;
				max_channel = 0;

				permission = PERMISSION_TYPE.PERMISSION_REJECT;
				status = SERVER_STATUS.SERVER_SHUTDOWN;
				server_type = SERVER_TYPE.SERVER_AUTH;

				major_version = 0;
				minor_version = 0;
				patch_version = 0;

				delay_ping = 0;

				for(INT i = 0; i < iMAX_CHANNEL_LIST; ++i) {
					channel_info[i].Clear();
				}

				addr_in.Address = IPAddress.Any;
				addr_in.Port = 0;

				option1 = "";
				option2 = "";
				domain = "";
				name = "";
			}

			public UINT				GetId()							{ return id; }
			public void				SetId(UINT o)					{ id = o; }

			public UINT				GetLimitedUser()				{ return limited_user; }
			public void				SetLimitedUser(UINT o)			{ limited_user = o; }

			public UINT				GetMaxUser()					{ return max_user; }
			public void				SetMaxUser(UINT o)				{ max_user = o; }

			public UINT				GetUser()						{ return user; }
			public void				SetUser(UINT o)					{ user = o; }

			public INT				GetMaxChannel()					{ return max_channel; }
			public void				SetMaxChannel(INT o)			{ max_channel = o; }

			public PERMISSION_TYPE	GetPermission()					{ return permission; }
			public void				SetPermission(PERMISSION_TYPE o)	{ permission = o; }

			public SERVER_STATUS	GetStatus()						{ return status; }
			public void				SetStatus(SERVER_STATUS o)		{ status = o; }

			public SERVER_TYPE		GetServerType()					{ return server_type; }
			public void				SetServerType(SERVER_TYPE o)	{ server_type = o; }

			public UINT				GetMajorVersion()				{ return major_version; }
			public void				SetMajorVersion(UINT o)			{ major_version = o; }

			public UINT				GetMinorVersion()				{ return minor_version; }
			public void				SetMinorVersion(UINT o)			{ minor_version = o; }

			public UINT				GetPatchVersion()				{ return patch_version; }
			public void				SetPatchVersion(UINT o)			{ patch_version = o; }

			public tick_t			GetDelayPing()					{ return delay_ping; }
			public void				SetDelayPing(tick_t o)			{ delay_ping = o; }

			public void				IncreasedUser()					{ ++user; }
			public void				DecreasedUser()					{ if(0 < user) --user; else OUTPUT("critical error: user count: "); }

			public SChannelInfo[]	GetChannelInfo()				{ return channel_info; }
			public SChannelInfo		GetChannelInfo(INT o)			{ return channel_info[o]; }

			public bool
			SetAddress(CHAR[] o, UINT16 p) {
				if(isptr(o)) {
					addr_in.Address = IPAddress.Parse(ConvertToString(o));
					addr_in.Port = (INT)p;
					return true;
				} else {
					TRACE("critical error: address is null");
				}
				return false;
			}

			public CHAR[]			GetAddress()					{ if(isptr(addr_in)) { return ConvertToBytes(addr_in.Address.ToString()); } return ConvertToBytes("0.0.0.0"); }
			public ULONG			GetSinAddress()					{ BYTE[] byAddr = addr_in.Address.GetAddressBytes(); if(false == BitConverter.IsLittleEndian) { Array.Reverse(byAddr); } return (ULONG)(BitConverter.ToUInt32(byAddr, 0)); }
			public IPEndPoint		GetAddressIn()					{ return addr_in; }
			public INT				GetPort()						{ if(isptr(addr_in)) { return addr_in.Port; } return 0; }

			public string			GetOption1()					{ return option1; }
			public void				SetOption1(string o)			{ option1 = o; }

			public string			GetOption2()					{ return option2; }
			public void				SetOption2(string o)			{ option2 = o; }

			public string			GetDomain()						{ return domain; }
			public void				SetDomain(string o)				{ domain = o; }

			public string			GetName()						{ return name; }
			public void				SetName(string o)				{ name = o; }

			public UINT				id;
			public UINT				limited_user;
			public UINT				max_user;
			public UINT				user;
			public INT				max_channel;

			public PERMISSION_TYPE	permission;
			public SERVER_STATUS	status;
			public SERVER_TYPE		server_type;

			public UINT				major_version;
			public UINT				minor_version;
			public UINT				patch_version;

			public tick_t			delay_ping;

			public SChannelInfo[]	channel_info;
			public IPEndPoint		addr_in;

			public string			option1;
			public string			option2;
			public string			domain;
			public string			name;
		}
	}
}

/* EOF */
