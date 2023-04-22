/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_COMMAND_H___
#define ___NETDRONE_NET_COMMAND_H___

#include "Protocol.h"

#include <string.h>

#include "../util/Allocator.h"

#include "../GlobalVariable.h"
#include "../Defines.h"
#include "../Enums.h"

class CConnector;

class CCommand {
public:
	CCommand() { memset(m_acData, 0, iCOMMAND_DATA_SIZE); }
	~CCommand() {}

	UINT			GetOrder()			{ return UINT(m_tHeader.order); }
	UINT			GetMission()		{ return UINT(m_tHeader.mission); }
	UINT			GetExtra()			{ return UINT(m_tHeader.extra); }
	UINT			GetOption()			{ return UINT(m_tHeader.option); }

	SCommandHeader&	GetCommandHeader()	{ return m_tHeader; }
	char&			GetData()			{ return *m_acData; }
	char&			GetHeader()			{ return (char&)m_tHeader; }

	void			SetOrder(UINT o)	{ m_tHeader.order = UINT16(o); }
	void			SetMission(UINT o)	{ m_tHeader.mission = UINT16(o); }
	void			SetExtra(UINT o)	{ m_tHeader.extra = BYTE(o); }
	void			SetOption(UINT o)	{ m_tHeader.option = BYTE(o); }

private:
	SCommandHeader	m_tHeader;
	char			m_acData[iCOMMAND_DATA_SIZE];

};

class CNativeCommand : public CCommand, public CAllocator< CNativeCommand > {
public:
	CNativeCommand() {}
	~CNativeCommand() {}

	CCommand&	GetCommand()	{ return *this; }

};

class CExtendCommand : public CCommand, public CAllocator< CExtendCommand > {
public:
	CExtendCommand() : m_pkConnector(NULL), m_iKey(0), m_iRelay(0), m_stSize(0) {}
	~CExtendCommand() {}

	CCommand&	GetCommand()					{ return *this; }

	INT			GetKey()						{ return m_iKey; }
	void		SetKey(INT o)					{ m_iKey = o; }

	size_t		GetSize()						{ return m_stSize; }
	void		SetSize(size_t o)				{ m_stSize = o; }

	INT			GetRelay()						{ return m_iRelay; }
	void		SetRelay(INT o)					{ m_iRelay = o; }

	CConnector*	GetConnector()					{ return m_pkConnector; }
	void		SetConnector(CConnector* o)		{ m_pkConnector = o; }

private:
	CConnector*	m_pkConnector;
	INT			m_iKey;
	INT			m_iRelay;
	size_t		m_stSize;

};

class CSendCommand : public CCommand, public CAllocator< CSendCommand > {
public:
	CSendCommand() : m_pkConnector(NULL), m_iSendKey(0), m_iSize(0), m_eType(PACKET_THROW), m_eCrypt(CRYPT_NONE) {}
	~CSendCommand() {}

	CCommand&	GetCommand()					{ return *this; }

	INT			GetSendKey()					{ return m_iSendKey; }
	void		SetSendKey(INT o)				{ m_iSendKey = o; }

	INT			GetSize()						{ return m_iSize; }
	void		SetSize(INT o)					{ m_iSize = o; }

	PACKET_TYPE	GetPacketType()					{ return m_eType; }
	void		SetPacketType(PACKET_TYPE o)	{ m_eType = o; }

	CRYPT_TYPE	GetCrypt()						{ return m_eCrypt; }
	void		SetCrypt(CRYPT_TYPE o)			{ m_eCrypt = o; }

	CConnector*	GetConnector()					{ return m_pkConnector; }
	void		SetConnector(CConnector* o)		{ m_pkConnector = o; }

private:
	CConnector*	m_pkConnector;
	INT			m_iSendKey;
	INT			m_iSize;
	PACKET_TYPE	m_eType;
	CRYPT_TYPE	m_eCrypt;

};

#endif // ___NETDRONE_NET_COMMAND_H___

/* EOF */
