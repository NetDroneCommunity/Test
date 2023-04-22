/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_CONFIG_H___
#define ___NETDRONE_UTIL_CONFIG_H___

#include "Singleton.h"

#include "../GlobalVariable.h"
#include "../Defines.h"

class CConfig {
public:
	CConfig();
	virtual ~CConfig();

	virtual	bool		Initialize(const char*, INT);
	virtual bool		Initialize(const char*, const char*, INT);
	virtual bool		Release();

	virtual bool		Load(const char*) = 0;
	virtual bool		Create();
	virtual bool		Delete();

	virtual bool		SetTcpDefaultAddress(char*, UINT16);
	const char*			GetTcpDefaultAddress()			const	{ return inet_ntoa(m_saTcpDefaultAddr_in.sin_addr); }
	const ULONG			GetTcpDefaultSinAddress()		const	{ return m_saTcpDefaultAddr_in.sin_addr.s_addr; }
	const SOCKADDR_IN&	GetTcpDefaultAddressIn()		const	{ return m_saTcpDefaultAddr_in; }
	const INT			GetTcpDefaultPort()				const	{ return ntohs(m_saTcpDefaultAddr_in.sin_port); }

	virtual bool		SetTcpBackboneAddress(char*, UINT16);
	const char*			GetTcpBackboneAddress()			const	{ return inet_ntoa(m_saTcpBackboneAddr_in.sin_addr); }
	const ULONG			GetTcpBackboneSinAddress()		const	{ return m_saTcpBackboneAddr_in.sin_addr.s_addr; }
	const SOCKADDR_IN&	GetTcpBackboneAddressIn()		const	{ return m_saTcpBackboneAddr_in; }
	const INT			GetTcpBackbonePort()			const	{ return ntohs(m_saTcpBackboneAddr_in.sin_port); }

	virtual bool		SetUdpReliableAddress(char*, UINT16);
	const char*			GetUdpReliableAddress()			const	{ return inet_ntoa(m_saUdpReliableAddr_in.sin_addr); }
	const ULONG			GetUdpReliableSinAddress()		const	{ return m_saUdpReliableAddr_in.sin_addr.s_addr; }
	const SOCKADDR_IN&	GetUdpReliableAddressIn()		const	{ return m_saUdpReliableAddr_in; }
	const INT			GetUdpReliablePort()			const	{ return ntohs(m_saUdpReliableAddr_in.sin_port); }

	const INT			GetServerId()					const	{ return m_iServerId; }
	void				SetServerId(INT o)						{ m_iServerId = o; }

	const INT			GetMaxConnection()				const	{ return m_iMaxConnection; }
	void				SetMaxConnection(INT o)					{ m_iMaxConnection = o; }

	const bool			IsHeaderCrypt()					const	{ return m_bHeaderCrypt; }
	void				SetHeaderCrypt(bool o)					{ m_bHeaderCrypt = o; }

	const char*			GetPackageName()				const	{ return m_szPackageName; }
	void				SetPackageName(const char& o)			{ memcpy(m_szPackageName, &o, iFILE_NAME_LEN); m_szPackageName[iFILE_NAME_LEN] = '\0'; }

	const char*			GetLogPath()					const	{ return m_szLogPath; }
	void				SetLogPath(const char& o)				{ memcpy(m_szLogPath, &o, iFILE_PATH_LEN); m_szLogPath[iFILE_PATH_LEN] = '\0'; }

	const char*			GetInfoPath()					const	{ return m_szInfoPath; }
	void				SetInfoPath(const char& o)				{ memcpy(m_szInfoPath, &o, iFILE_PATH_LEN); m_szInfoPath[iFILE_PATH_LEN] = '\0'; }

	const INT			GetGPid()						const	{ return m_iGPid; }
	const INT			GetPPid()						const	{ return m_iPPid; }
	const INT			GetPid()						const	{ return m_iPid; }

protected:
	SOCKADDR_IN	m_saTcpDefaultAddr_in;
	SOCKADDR_IN	m_saTcpBackboneAddr_in;
	SOCKADDR_IN	m_saUdpReliableAddr_in;

	char		m_szPackageName[iFILE_NAME_LEN+1];
	char		m_szLogPath[iFILE_PATH_LEN+1];
	char		m_szInfoPath[iFILE_PATH_LEN+1];

private:
	bool		m_bInitialized;

	INT			m_iGPid;
	INT			m_iPPid;
	INT			m_iPid;

	INT			m_iServerId;
	INT			m_iMaxConnection;

	bool		m_bHeaderCrypt;

};

#endif // ___NETDRONE_UTIL_CONFIG_H___

/* EOF */
