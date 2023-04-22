/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_SOCKET_H___
#define ___NETDRONE_NET_SOCKET_H___

#include <sys/socket.h>

#include "Queue.h"

#include "../util/Locker.h"

#include "../Defines.h"
#include "../Enums.h"

class CRawSocket {
public:
	CRawSocket();
	virtual ~CRawSocket();

	virtual SOCKET				CreateSocket()					{ return INVALID_SOCKET; }

	virtual bool				Clear();
	virtual void				ClearSendQueue()				{}
	virtual void				ClearRecvQueue()				{}
			void				ClearAllQueue()					{ ClearSendQueue(); ClearRecvQueue(); }

	virtual bool				Connect(const char*, INT);
	virtual bool				Disconnect();

	virtual INT					Read(char&, INT)				{ return -1; }

	virtual INT					Send(const char&, INT)			{ return -1; }
	virtual bool				SendQueue()						{ return false; }
	virtual INT					SendTrust(const char&, INT)		{ return -1; }
	virtual INT					SendBlock(const char&, INT)		{ return -1; }
	virtual INT					SendOrder(const char&, INT)		{ return -1; }

			SOCKET				Bind(const char*, INT);

			bool				SetSocketOption(INT, INT);
			INT					GetSocketOption(INT);

			bool				IsConnected()					{ return m_bConnected; }
			void				SetConnect(bool o)				{ m_bConnected = o; }

			const SOCKET		GetSocket() const				{ return m_skSocket; }
			const SOCKET		GetDescriptor() const			{ return m_skSocket; }

			void				SetSocket(const SOCKET o)		{ m_skSocket = o; }
			void				SetConnected()					{ m_bConnected = true; }

			const char*			GetRemoteAddress() const		{ return inet_ntoa(m_saRemoteAddr_in.sin_addr); }
			const ULONG			GetRemoteSinAddress() const		{ return m_saRemoteAddr_in.sin_addr.s_addr; }
			const SOCKADDR_IN&	GetRemoteAddressIn()			{ return m_saRemoteAddr_in; }
			INT					GetRemotePort()					{ return ntohs(m_saRemoteAddr_in.sin_port); }

			const char*			GetLocalAddress() const			{ return inet_ntoa(m_saLocalAddr_in.sin_addr); }
			const ULONG			GetLocalSinAddress() const		{ return m_saLocalAddr_in.sin_addr.s_addr; }
			const SOCKADDR_IN&	GetLocalAddressIn()				{ return m_saLocalAddr_in; }
			INT					GetLocalPort()					{ return ntohs(m_saLocalAddr_in.sin_port); }

			const char*			GetPublicAddress() const		{ return inet_ntoa(m_saPublicAddr_in.sin_addr); }
			const ULONG			GetPublicSinAddress() const		{ return m_saPublicAddr_in.sin_addr.s_addr; }
			const SOCKADDR_IN&	GetPublicAddressIn()			{ return m_saPublicAddr_in; }
			INT					GetPublicPort()					{ return ntohs(m_saPublicAddr_in.sin_port); }

			bool				SameRemoteAddressIn(const SOCKADDR_IN&);
			bool				SameLocalAddressIn(const SOCKADDR_IN&);
			bool				SamePublicAddressIn(const SOCKADDR_IN&);

			void				SetRemoteAddress(const char*, INT);
			void				SetLocalAddress(const char*, INT);
			void				SetPublicAddress(const char*, INT);

			void				SetRemoteAddress(const SOCKADDR_IN&);
			void				SetLocalAddress(const SOCKADDR_IN&);
			void				SetPublicAddress(const SOCKADDR_IN&);

			void				SetRemoteAddress(ULONG, INT);
			void				SetLocalAddress(ULONG, INT);
			void				SetPublicAddress(ULONG, INT);

			void				ClearRemoteAddress()			{ memset(&m_saRemoteAddr_in, 0, sizeof(SOCKADDR_IN)); }
			void				ClearLocalAddress()				{ memset(&m_saLocalAddr_in, 0, sizeof(SOCKADDR_IN)); }
			void				ClearPublicAddress()			{ memset(&m_saPublicAddr_in, 0, sizeof(SOCKADDR_IN)); }

			void				SetNatType(NAT_TYPE o)			{ m_eNatType = o; }
			NAT_TYPE			GetNatType()					{ return m_eNatType; }

			void				SetNotConnectedSocket(bool o)	{ m_bNotConnectedSocket = o; }
			bool				IsNotConnectedSocket()			{ return m_bNotConnectedSocket; }

protected:
			SOCKADDR_IN			m_saRemoteAddr_in;
			SOCKADDR_IN			m_saLocalAddr_in;
			SOCKADDR_IN			m_saPublicAddr_in;
			socklen_t			m_slSize;
			SOCKET				m_skSocket;
			CLocker				m_kLocker;

private:
			NAT_TYPE			m_eNatType;
			bool				m_bConnected;
			bool				m_bNotConnectedSocket;

};

class CTcpSocket : public CRawSocket {
public:
	CTcpSocket();
	virtual ~CTcpSocket();

	virtual SOCKET		CreateSocket();

	virtual void		ClearSendQueue();

	virtual INT			Read(char&, INT);

	virtual INT			Send(const char&, INT);
	virtual bool		SendQueue();
	virtual INT			SendTrust(const char&, INT);
	virtual INT			SendBlock(const char&, INT);
	virtual INT			SendOrder(const char& o, INT p)	{ return SendBlock(o, p); }

			SOCKET		Listen(const char*, INT);
			SOCKET		Accept();

			bool		SetTcpOption(INT, INT);

private:
			INT			m_iRecvFlags;
			INT			m_iSendFlags;

			CBlockQueue	m_kBlockQueue;

};

class CUdpSocket : public CRawSocket {
public:
	CUdpSocket();
	virtual ~CUdpSocket();

	virtual SOCKET				CreateSocket();

	virtual bool				Clear();
	virtual void				ClearSendQueue();
	virtual void				ClearRecvQueue();

	virtual bool				Connect(const char*, INT);

	virtual INT					Read(char&, INT);

	virtual INT					Send(const char&, INT);
	virtual bool				SendQueue();
	virtual INT					SendTrust(const char&, INT);
	virtual INT					SendBlock(const char&, INT);
	virtual INT					SendOrder(const char&, INT);

			INT					GetRecvBlockSize()								{ return m_kRecvBlockQueue.Size(); }
			INT					GetRecvOrderSize()								{ return m_kRecvOrderQueue.Size(); }

			bool				IsRecvBlockEmpty()								{ return m_kRecvBlockQueue.Empty(); }
			bool				IsRecvOrderEmpty()								{ return m_kRecvOrderQueue.Empty(); }

			void				RemoveSendBlockQueue(UINT32);
			void				RemoveSendOrderQueue(UINT32);

			void				RemoveSendBlockQueueAll(UINT32);
			void				RemoveSendOrderQueueAll(UINT32);

			const char*			GetRecvAddress()								{ return inet_ntoa(m_saRecvAddr_in.sin_addr); }
			const ULONG			GetRecvSinAddress()								{ return m_saRecvAddr_in.sin_addr.s_addr; }
			const SOCKADDR_IN&	GetRecvAddressIn()								{ return m_saRecvAddr_in; }
			INT					GetRecvPort()									{ return ntohs(m_saRecvAddr_in.sin_port); }

			void				SetRecvAddress(const SOCKADDR_IN& rsaSockAddr_)	{ memcpy(&m_saRecvAddr_in, &rsaSockAddr_, sizeof(SOCKADDR_IN)); }
			void				SetRecvAddress(ULONG ulAddr_, INT iPort_)		{ memset(&m_saRecvAddr_in, 0, sizeof(SOCKADDR_IN)); m_saRecvAddr_in.sin_family = AF_INET; m_saRecvAddr_in.sin_addr.s_addr = ulAddr_; m_saRecvAddr_in.sin_port = iPort_; }

			void				ClearRecvAddress()								{ memset(&m_saRecvAddr_in, 0, sizeof(SOCKADDR_IN)); }

			INT					SaveRecvOrder(CUdpPacket&);
			CUdpPacketBuffer*	PopRecvOrder(UINT);

			INT					SaveRecvBlock(CUdpPacket&);
			CUdpPacketBuffer*	PopRecvBlock(UINT);

private:
			INT					CallbackReadFromA(char&, INT);
			INT					CallbackSendToA(const char&, INT);

			INT					CallbackReadFromB(char&, INT);
			INT					CallbackSendToB(const char&, INT);

private:
			typedef INT	(CUdpSocket::*PIF_WRITE)(const char&, INT);
			typedef INT	(CUdpSocket::*PIF_READ)(char&, INT);
			PIF_WRITE			m_pifWrite;
			PIF_READ			m_pifRead;

			INT					m_iRecvFlags;
			INT					m_iSendFlags;
			UINT64				m_ullNextSend;

			SOCKADDR_IN			m_saRecvAddr_in;
			CSendBlockQueue		m_kSendBlockQueue;
			CSendOrderQueue		m_kSendOrderQueue;
			CRecvBlockQueue		m_kRecvBlockQueue;
			CRecvOrderQueue		m_kRecvOrderQueue;

};

#endif // ___NETDRONE_NET_SOCKET_H___

/* EOF */
