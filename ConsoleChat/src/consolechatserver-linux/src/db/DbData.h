/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_DB_DBDATA_H___
#define ___SERVICE_DB_DBDATA_H___

#include <netdrone/CommonHeader.h>

#include "../GlobalVariable.h"
#include "../Enums.h"

struct SChannelInfo {
	SChannelInfo() { memset(this, 0, sizeof(SChannelInfo)); }

	void			Clear()						{ memset(this, 0, sizeof(SChannelInfo)); }

	UINT			GetId()						{ return id; }
	void			SetId(UINT o)				{ id = o; }

	UINT			GetMaxUser()				{ return max_user; }
	void			SetMaxUser(INT o)			{ max_user = o; }

	UINT			GetUser()					{ return user; }
	void			SetUser(INT o)				{ user = o; }

	void			IncreasedUser()				{ ++user; }
	void			DecreasedUser()				{ if(0 < user) --user; else OUTPUT("critical error: user count: "); }

	UINT			GetIndex()					{ return index; }
	void			SetIndex(UINT o)			{ index = o; }

	UINT			GetServerId()				{ return server_id; }
	void			SetServerId(UINT o)			{ server_id = o; }

	const char*		GetName()					{ return name; }
	void			SetName(const char& o)		{ memcpy(name, &o, iCHANNEL_NAME_LEN); name[iCHANNEL_NAME_LEN] = '\0'; }

	UINT	id;
	UINT	max_user;
	UINT	user;

	UINT	index;
	UINT	server_id;

	char	name[iCHANNEL_NAME_LEN+1];
};

struct SServerInfo {
	SServerInfo() { memset(this, 0, sizeof(SServerInfo)); }

	void			Clear()							{ memset(this, 0, sizeof(SServerInfo)); }

	UINT			GetId()							{ return id; }
	void			SetId(UINT o)					{ id = o; }

	UINT			GetLimitedUser()				{ return limited_user; }
	void			SetLimitedUser(UINT o)			{ limited_user = o; }

	UINT			GetMaxUser()					{ return max_user; }
	void			SetMaxUser(UINT o)				{ max_user = o; }

	UINT			GetUser()						{ return user; }
	void			SetUser(UINT o)					{ user = o; }

	INT				GetMaxChannel()					{ return max_channel; }
	void			SetMaxChannel(INT o)			{ max_channel = o; }

	PERMISSION_TYPE	GetPermission()					{ return permission; }
	void			SetPermission(PERMISSION_TYPE o)	{ permission = o; }

	SERVER_STATUS	GetStatus()						{ return status; }
	void			SetStatus(SERVER_STATUS o)		{ status = o; }

	SERVER_TYPE		GetServerType()					{ return server_type; }
	void			SetServerType(SERVER_TYPE o)	{ server_type = o; }

	LANGUAGE_TYPE	GetLanguage()					{ return language; }
	void			SetLanguage(LANGUAGE_TYPE o)	{ language = o; }

	UINT			GetMajorVersion()				{ return major_version; }
	void			SetMajorVersion(UINT o)			{ major_version = o; }

	UINT			GetMinorVersion()				{ return minor_version; }
	void			SetMinorVersion(UINT o)			{ minor_version = o; }

	UINT			GetPatchVersion()				{ return patch_version; }
	void			SetPatchVersion(UINT o)			{ patch_version = o; }

	tick_t			GetDelayPing()					{ return delay_ping; }
	void			SetDelayPing(tick_t o)			{ delay_ping = o; }

	void			IncreasedUser()					{ ++user; }
	void			DecreasedUser()					{ if(0 < user) --user; else OUTPUT("critical error: user count: "); }

	SChannelInfo*	GetChannelInfo()				{ return channel_info; }
	SChannelInfo&	GetChannelInfo(INT o)			{ return channel_info[o]; }

	bool
	SetAddress(char* o, UINT16 p) {
		if(isptr(o)) {
			addr_in.sin_family = AF_INET;
			addr_in.sin_addr.s_addr = ::inet_addr(o);
			addr_in.sin_port = htons(p);
			return true;
		} else {
			TRACE("critical error: address is null");
		}
		return false;
	}

	const char*		GetAddress()		const		{ return inet_ntoa(addr_in.sin_addr); }
	const ULONG		GetSinAddress()		const		{ return addr_in.sin_addr.s_addr; }
	const SOCKADDR_IN&	GetAddressIn()	const		{ return addr_in; }
	const INT		GetPort()			const		{ return ntohs(addr_in.sin_port); }

	const char*		GetOption1()					{ return option1; }
	void			SetOption1(const char& o)		{ memcpy(option1, &o, iSERVER_OPTION_LEN); option1[iSERVER_OPTION_LEN] = '\0'; }

	const char*		GetOption2()					{ return option2; }
	void			SetOption2(const char& o)		{ memcpy(option2, &o, iSERVER_OPTION_LEN); option2[iSERVER_OPTION_LEN] = '\0'; }

	const char*		GetDomain()						{ return domain; }
	void			SetDomain(const char& o)		{ memcpy(domain, &o, iSERVER_DOMAIN_LEN); domain[iSERVER_DOMAIN_LEN] = '\0'; }

	const char*		GetName()						{ return name; }
	void			SetName(const char& o)			{ memcpy(name, &o, iSERVER_NAME_LEN); name[iSERVER_NAME_LEN] = '\0'; }

	UINT			id;
	UINT			limited_user;
	UINT			max_user;
	UINT			user;
	INT				max_channel;

	PERMISSION_TYPE	permission;
	SERVER_STATUS	status;
	SERVER_TYPE		server_type;
	LANGUAGE_TYPE	language;

	UINT			major_version;
	UINT			minor_version;
	UINT			patch_version;

	tick_t			delay_ping;

	SChannelInfo	channel_info[iMAX_CHANNEL_LIST];

	SOCKADDR_IN		addr_in;

	char			option1[iSERVER_OPTION_LEN+1];
	char			option2[iSERVER_OPTION_LEN+1];
	char			domain[iSERVER_DOMAIN_LEN+1];
	char			name[iSERVER_NAME_LEN+1];
};

#endif // ___SERVICE_DB_DBDATA_H___

/* EOF */
