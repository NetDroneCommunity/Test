/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_PACKET_H___
#define ___NETDRONE_NET_PACKET_H___

#include "Protocol.h"
#include "Command.h"

#include <stdlib.h>

#include "../GlobalVariable.h"
#include "../Defines.h"

class CTcpPacket {
public:
	CTcpPacket(UINT uiKey_, UINT uiCrypt_ =0) { ResetBogus(); m_tHeader.key = uiKey_; m_tHeader.crypt = uiCrypt_; }
	~CTcpPacket() {}

	UINT				GetKey()				{ return m_tHeader.key; }
	UINT				GetCrypt()				{ return m_tHeader.crypt; }
	UINT				GetSize()				{ return m_tHeader.size; }
	UINT				GetDataSize()			{ return (m_tHeader.size - iTCP_PACKET_HEAD_SIZE); }

	STcpPacketHeader&	GetPacketHeader()		{ return m_tHeader; }
	char*				GetHeader()				{ return (char*)&m_tHeader; }
	char*				GetData()				{ return (char*)&m_kCommand; }
	CCommand&			GetCommand()			{ return m_kCommand; }

	void				SetKey(UINT o)			{ m_tHeader.key = o; }
	void				SetSize(UINT o)			{ m_tHeader.size = o; }
	void				SetCrypt(UINT o)		{ m_tHeader.crypt = o; }

	void				ResetBogus()			{ m_tHeader.bogus = RAND(262143) + 1; }

private:
	STcpPacketHeader	m_tHeader;
	CCommand			m_kCommand;

};

class CUdpPacket {
public:
	CUdpPacket(UINT uiKey_, UINT uiSeq_, UINT uiType_ =0, UINT uiCrypt_ =0) { ResetBogus(); m_tHeader.key = uiKey_; m_tHeader.seq = uiSeq_; m_tHeader.type = uiType_; m_tHeader.crypt = uiCrypt_; }
	~CUdpPacket() {}

	UINT				GetKey()				{ return m_tHeader.key; }
	UINT				GetCrypt()				{ return m_tHeader.crypt; }
	UINT				GetSize()				{ return m_tHeader.size; }
	UINT				GetDataSize()			{ return (m_tHeader.size - iUDP_PACKET_HEAD_SIZE); }

	UINT				GetSequence()			{ return m_tHeader.seq; }
	UINT				GetPacketType()			{ return m_tHeader.type; }
	CUdpPacket&			GetFront()				{ return (CUdpPacket&)m_tHeader; }

	SUdpPacketHeader&	GetPacketHeader()		{ return m_tHeader; }
	char*				GetHeader()				{ return (char*)&m_tHeader; }
	char*				GetData()				{ return (char*)&m_kCommand; }
	CCommand&			GetCommand()			{ return m_kCommand; }

	void				SetKey(UINT o)			{ m_tHeader.key = o; }
	void				SetSize(UINT o)			{ m_tHeader.size = o; }
	void				SetCrypt(UINT o)		{ m_tHeader.crypt = o; }
	void				SetSequence(UINT o)		{ m_tHeader.seq = o; }
	void				SetPacketType(UINT o)	{ m_tHeader.type = o; }

	void				ResetBogus()			{ m_tHeader.bogus = RAND(262143) + 1; }

protected:
	SUdpPacketHeader	m_tHeader;
	CCommand			m_kCommand;
};

#endif // ___NETDRONE_NET_PACKET_H___

/* EOF */
