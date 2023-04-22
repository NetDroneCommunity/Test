/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_BUFFER_H___
#define ___NETDRONE_NET_BUFFER_H___

#include "Packet.h"

#include "../util/Allocator.h"

#include "../GlobalVariable.h"
#include "../Defines.h"

class CStreamBuffer {
public:
	CStreamBuffer() : m_acBuffer(NULL), m_stMax(0), m_stSize(0)	{}
	~CStreamBuffer()	{}

	bool	Clear();
	size_t	Move(size_t);

	size_t	GetMax()						{ return m_stMax; }
	size_t	RestSize()						{ return (GetMax() - GetSize()); }

	size_t	GetSize()						{ return m_stSize; }
	void	SetSize(size_t o)				{ m_stSize = o; }
	size_t	AddSize(size_t o)				{ return (m_stSize += o); }

	char*	GetBuffer()						{ return m_acBuffer; }
	char*	GetBuffer(size_t o)				{ return m_acBuffer + o; }
	void	SetBuffer(char* o, size_t p)	{ m_acBuffer = o; m_stMax = p; m_stSize = 0; }

	char&	GetLast()						{ return *(m_acBuffer + GetSize()); }

private:
	char*	m_acBuffer;
	size_t	m_stMax;
	size_t	m_stSize;

};

class CTcpPacketBuffer : public CAllocator< CTcpPacketBuffer > {
public:
	CTcpPacketBuffer() : m_stFrontOffset(0), m_stRearOffset(0), m_stSize(0) {}
	~CTcpPacketBuffer() {}

	bool	Initialize();
	void	Clear();

	size_t	GetFree()					{ return iMAX_PACKET_SIZE - m_stRearOffset; }
	char*	GetFront()					{ return m_acBuffer + m_stFrontOffset; }
	char*	GetRear()					{ return m_acBuffer + m_stRearOffset; }

	size_t	GetFrontOffset()			{ return m_stFrontOffset; }
	size_t	GetRearOffset()				{ return m_stRearOffset; }
	size_t	GetSize()					{ return m_stSize; }

	void	SetFrontOffset(size_t o)	{ m_stFrontOffset = o; }
	void	SetRearOffset(size_t o)		{ m_stRearOffset = o; }
	void	SetSize(size_t o)			{ m_stSize = o; }

	bool	PutStream(const char&, size_t);
	bool	Truncate(size_t);

protected:
	char	m_acBuffer[iMAX_PACKET_SIZE];
	size_t	m_stFrontOffset;
	size_t	m_stRearOffset;
	size_t	m_stSize;

};

class CUdpPacketBuffer : public CUdpPacket, public CAllocator< CUdpPacketBuffer > {
public:
	CUdpPacketBuffer() : CUdpPacket(0, 0), m_ullTick(0), m_uiRetryCount(0), m_uiPacketSequence(0), m_uiPacketSize(0) {}
	~CUdpPacketBuffer() {}

	bool	Initialize(UINT uiKey_ =0, UINT uiSeq_ =0, UINT uiType_ =0, UINT uiCrypt_ =0);
	void	Clear();

	void	SetTick(UINT64 o)			{ m_ullTick = o; ++m_uiRetryCount; }
	UINT64	GetTick()					{ return m_ullTick; }

	UINT	GetRetryCount()				{ return m_uiRetryCount; }
	UINT	GetPacketSequence()			{ return m_uiPacketSequence; }

	void	SetPacketSize(UINT o)		{ m_uiPacketSize = o; }
	UINT	GetPacketSize()				{ return m_uiPacketSize; }

private:
	UINT64	m_ullTick;
	UINT	m_uiRetryCount;
	UINT	m_uiPacketSequence;
	UINT	m_uiPacketSize;

};

#endif // ___NETDRONE_NET_BUFFER_H___

/* EOF */
