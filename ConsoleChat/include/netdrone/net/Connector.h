/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_CONNECTOR_H___
#define ___NETDRONE_NET_CONNECTOR_H___

#include "Socket.h"

#include "../util/Tick.h"

#include "../GlobalVariable.h"
#include "../Defines.h"

class IDispatcher;
class ISensor;

class CConnector {
public:
	CConnector() : m_pkSocket(NULL), m_pkCommandQueue(NULL), m_pkSensor(NULL), m_pkDispatcher(NULL), m_ullKeepAliveTick(0), m_tkSendPingTick(0), m_tkRecvPingTick(0), m_uiQueueCount(0), m_usSendKey(0), m_usRecvKey(0), m_iSerialKey(0), m_iRegisterIndex(-1), m_iRelay(-1), m_bRegister(false), m_bHeaderCrypt(false) {}
	virtual ~CConnector() {}

	virtual bool				Initialize(ICommandQueue& o, ISensor& p)				{ m_pkCommandQueue = &o; m_pkSensor = &p; return true; }
	virtual void				Release()												{}

	virtual INT					Send(CCommand&, INT =0, PACKET_TYPE =PACKET_THROW, CRYPT_TYPE = CRYPT_NONE) { return -1; }
	virtual INT					SendTrust(CCommand&, INT =0, CRYPT_TYPE =CRYPT_NONE)	{ return -1; }

	virtual bool				KeepAlive()												{ return true; }

	virtual void				ResetSendQueue()										{ m_pkSocket->ClearSendQueue(); }
	virtual void				ResetRecvQueue()										{ m_pkSocket->ClearRecvQueue(); }
	virtual void				ResetSequence()											{}
	virtual void				Clear()													{ m_ullKeepAliveTick = 0; m_tkSendPingTick = 0; m_tkRecvPingTick = 0; }

			bool				IsHeaderCrypt()											{ return m_bHeaderCrypt; }
			void				SetHeaderCrypt(bool o)									{ m_bHeaderCrypt = o; }
			void				ClearAllQueue()											{ m_pkSocket->ClearSendQueue(); m_pkSocket->ClearRecvQueue(); }
			void				RemoveSendBlockQueue(INT o)								{ ((CUdpSocket*)m_pkSocket)->RemoveSendBlockQueue(o); }

			SOCKET				Bind(const char* o, INT p)								{ return m_pkSocket->Bind(o, p); }
	virtual	INT					Listen(const char* o, INT p)							{ return -1; }

	virtual	bool				Read()													{ return false; }
//		virtual	bool				ReadFrom(char& o)										{ return false; }
	virtual	bool				ReadFrom(CUdpPacket& o)									{ return false; }

			bool				SendQueue()												{ return m_pkSocket->SendQueue(); }

			bool				IsConnected()											{ return m_pkSocket->IsConnected(); }

			void				SetRegisterIndex(INT o)									{ m_iRegisterIndex = o; }
			INT					GetRegisterIndex()										{ return m_iRegisterIndex; }

			void				SetRegister(bool o)										{ m_bRegister = o; }
			bool				IsRegister()											{ return m_bRegister; }

			void				SetSocket(const SOCKET o)								{ m_pkSocket->SetSocket(o); }

			const SOCKET
			GetSocket() {
				if(isptr(m_pkSocket)) {
					return m_pkSocket->GetSocket();
				}
				return INVALID_SOCKET;
			}

			SOCKET				CreateSocket()											{ return m_pkSocket->CreateSocket(); }

			CRawSocket*			GetRawSocket()											{ return m_pkSocket; }

	virtual	void				SetConnected()											{ m_pkSocket->SetConnected(); }

			void				SetNatType(NAT_TYPE o)									{ m_pkSocket->SetNatType(o); }
			NAT_TYPE			GetNatType()											{ return m_pkSocket->GetNatType(); }

			void				SetRemoteAddress(const char& o, INT p)					{ m_pkSocket->SetRemoteAddress(&o, p); }
			void				SetLocalAddress(const char& o, INT p)					{ m_pkSocket->SetLocalAddress(&o, p); }
			void				SetPublicAddress(const char& o, INT p)					{ m_pkSocket->SetPublicAddress(&o, p); }

			void				SetRemoteAddress(const SOCKADDR_IN& o)					{ m_pkSocket->SetRemoteAddress(o); }
			void				SetLocalAddress(const SOCKADDR_IN& o)					{ m_pkSocket->SetLocalAddress(o); }
			void				SetPublicAddress(const SOCKADDR_IN& o)					{ m_pkSocket->SetPublicAddress(o); }

			void				SetRemoteAddress(ULONG o, INT p)						{ m_pkSocket->SetRemoteAddress(o, p); }
			void				SetLocalAddress(ULONG o, INT p)							{ m_pkSocket->SetLocalAddress(o, p); }
			void				SetPublicAddress(ULONG o, INT p)						{ m_pkSocket->SetPublicAddress(o, p); }

			const char*			GetRemoteAddress()										{ return m_pkSocket->GetRemoteAddress(); }
			const ULONG			GetRemoteSinAddress()									{ return m_pkSocket->GetRemoteSinAddress(); }
			const SOCKADDR_IN&	GetRemoteAddressIn()									{ return m_pkSocket->GetRemoteAddressIn(); }
			INT					GetRemotePort()											{ return m_pkSocket->GetRemotePort(); }

			const char*			GetLocalAddress()										{ return m_pkSocket->GetLocalAddress(); }
			const ULONG			GetLocalSinAddress()									{ return m_pkSocket->GetLocalSinAddress(); }
			const SOCKADDR_IN&	GetLocalAddressIn()										{ return m_pkSocket->GetLocalAddressIn(); }
			INT					GetLocalPort()											{ return m_pkSocket->GetLocalPort(); }

			const char*			GetPublicAddress()										{ return m_pkSocket->GetPublicAddress(); }
			const ULONG			GetPublicSinAddress()									{ return m_pkSocket->GetPublicSinAddress(); }
			const SOCKADDR_IN&	GetPublicAddressIn()									{ return m_pkSocket->GetPublicAddressIn(); }
			INT					GetPublicPort()											{ return m_pkSocket->GetPublicPort(); }

			void				ClearRemoteAddress()									{ m_pkSocket->ClearRemoteAddress(); }
			void				ClearLocalAddress()										{ m_pkSocket->ClearLocalAddress(); }
			void				ClearPublicAddress()									{ m_pkSocket->ClearPublicAddress(); }

			bool				SameRemoteAddressIn(const SOCKADDR_IN& o)				{ return m_pkSocket->SameRemoteAddressIn(o); }
			bool				SameLocalAddressIn(const SOCKADDR_IN& o)				{ return m_pkSocket->SameLocalAddressIn(o); }
			bool				SamePublicAddressIn(const SOCKADDR_IN& o)				{ return m_pkSocket->SamePublicAddressIn(o); }

	virtual	bool				Connect()												{ return false; }
	virtual	bool				Connect(const char* o, INT p)							{ return false; }
	virtual	bool				Disconnect(const SOCKET o)								{ return false; }
	virtual void				Disconnect()											{}

			UINT				GetSendKey()											{ return m_usSendKey; }
			void				SetSendKey(UINT o)										{ m_usSendKey = o; }

			UINT				GetRecvKey()											{ return m_usRecvKey; }
			void				SetRecvKey(UINT o)										{ m_usRecvKey = o; }

			INT					GetSerialKey()											{ return m_iSerialKey; }
			void				SetSerialKey(INT o)										{ m_iSerialKey = o; }

			tick_t				GetSendPingTick()										{ return m_tkSendPingTick; }
			void				ResetSendPingTick()										{ m_tkSendPingTick = g_kTick.GetTick(); }

			tick_t				GetRecvPingTick()										{ return m_tkRecvPingTick; }
			void				ResetRecvPingTick()										{ m_tkRecvPingTick = g_kTick.GetTick(); }

			UINT				GetQueueCount()											{ return m_uiQueueCount; }
			void				IncreasedQueueCount()									{ ++m_uiQueueCount; }
			void				DecreasedQueueCount()									{ if(0 < m_uiQueueCount) { --m_uiQueueCount; } }

	virtual void				Encrypt(CRYPT_TYPE, const void*, void*, INT);
	virtual void				Decrypt(CRYPT_TYPE, const void*, void*, INT);

			void				SetRelay(INT o)											{ m_iRelay = o; }
			INT					GetRelay()												{ return m_iRelay; }

	virtual	bool				Queuing(CCommand& o, INT p, CRYPT_TYPE q)				{ m_tkRecvPingTick = g_kTick.GetTick(); return m_pkCommandQueue->Put(*this, o, p, q); }
//		virtual	bool				Queuing(char&)											{ return false; }

			void				SetDispatcher(IDispatcher& o)							{ m_pkDispatcher = &o; }
			IDispatcher*		GetDispatcher()											{ return m_pkDispatcher; }

			void				Lock()													{ m_kLocker.Lock(); }
			void				Unlock()												{ m_kLocker.Unlock(); }

protected:
	CLocker			m_kLocker;

	CRawSocket*		m_pkSocket;
	ICommandQueue*	m_pkCommandQueue;
	ISensor*		m_pkSensor;
	IDispatcher*	m_pkDispatcher;

	UINT64			m_ullKeepAliveTick;
	tick_t			m_tkSendPingTick;
	tick_t			m_tkRecvPingTick;
	UINT			m_uiQueueCount;

private:
	UINT16			m_usSendKey;
	UINT16			m_usRecvKey;

	INT32			m_iSerialKey;
	INT				m_iRegisterIndex;
	INT				m_iRelay;

	bool			m_bRegister;
	bool			m_bHeaderCrypt;

};

class CTcpConnector : public CConnector {
public:
	CTcpConnector();
	virtual ~CTcpConnector();

	virtual void			Clear();
	virtual void			ResetRecvQueue();

	virtual INT				Send(CCommand&, INT =0, PACKET_TYPE =PACKET_THROW, CRYPT_TYPE =CRYPT_NONE);
	virtual INT				SendTrust(CCommand&, INT =0, CRYPT_TYPE =CRYPT_NONE);

	virtual bool			KeepAlive();

			CTcpSocket*		GetTcpSocket()								{ return (CTcpSocket*)m_pkSocket; }

			static void*	New()										{ return new CTcpConnector; }

			bool			CheckRecvHeader(STcpPacketHeader&);

	virtual bool			Connect(const char*, INT);
	virtual bool			Disconnect(const SOCKET);
	virtual void			SetConnected();
	virtual bool			Read();
	virtual bool			Queuing(CCommand& o, INT p, CRYPT_TYPE q)	{ return CConnector::Queuing(o, p, q); }
//		virtual bool			Queuing(char&);

private:
			INT				SendQueue(CCommand&, INT, CRYPT_TYPE);

protected:
			CStreamBuffer	m_kStreamBuffer;
			char*			m_acRecvBuffer;

};

class CUdpConnector : public CConnector {
public:
	CUdpConnector();
	virtual ~CUdpConnector();

	virtual void				Clear();
	virtual void				ResetSequence();

	virtual INT					Send(CCommand&, INT =0, PACKET_TYPE =PACKET_THROW, CRYPT_TYPE =CRYPT_NONE);
	virtual INT					SendTrust(CCommand&, INT =0, CRYPT_TYPE =CRYPT_NONE);
	virtual bool				KeepAlive();
			bool				RecvQueueUpdate();
			bool				IsConnecting()							{ return (0 < m_iHandshakeStep); }
			INT					UdpConnect(PACKET_TYPE =PACKET_SYN);
			INT					UdpConnectAck(PACKET_TYPE =PACKET_SYN);
			void				SetUdpRemoteAddress(CConnector&);

			CUdpSocket*			GetUdpSocket()							{ return (CUdpSocket*)m_pkSocket; }

			const char*			GetRecvAddress()						{ return GetUdpSocket()->GetRecvAddress(); }
			const ULONG			GetRecvSinAddress()						{ return GetUdpSocket()->GetRecvSinAddress(); }
			const SOCKADDR_IN&	GetRecvAddressIn()						{ return GetUdpSocket()->GetRecvAddressIn(); }
			INT					GetRecvPort()							{ return GetUdpSocket()->GetRecvPort(); }

			void				SetRecvAddress(const SOCKADDR_IN& o)	{ GetUdpSocket()->SetRecvAddress(o); }
			void				SetRecvAddress(ULONG o, INT p)			{ GetUdpSocket()->SetRecvAddress(o, p); }
			void				ClearRecvAddress()						{ GetUdpSocket()->ClearRecvAddress(); }

			UINT32				GetSendSequence()						{ if((UINT)(iMAX_UDP_PACKET_SEQUENCE) == m_uiSendSequence) { m_uiSendSequence = 0; return m_uiSendSequence; } else { return ++m_uiSendSequence; } }
			UINT32				GetRecvSequence()						{ return m_uiRecvSequence; }

			UINT32				GetReliableSendBlockSequence()			{ return ++m_uiReliableSendBlockSequence; }
			UINT32				GetReliableRecvBlockSequence()			{ return m_uiReliableRecvBlockSequence; }

			UINT32				GetReliableSendOrderSequence()			{ return ++m_uiReliableSendOrderSequence; }
			UINT32				GetReliableRecvOrderSequence()			{ return m_uiReliableRecvOrderSequence; }

			void				SetRecvSequence(UINT32 o)				{ m_uiRecvSequence = o; }
			void				SetReliableRecvBlockSequence(UINT32 o)	{ m_uiReliableRecvBlockSequence = o; }
			void				SetReliableRecvOrderSequence(UINT32 o)	{ m_uiReliableRecvOrderSequence = o; }

			INT					WriteAck(INT, PACKET_TYPE);

			void				SetHandshakeStep(INT o)					{ m_iHandshakeStep = o; }
			INT					GetHandshakeStep()						{ return m_iHandshakeStep; }

			void				SetSendHole(bool o)						{ m_bSendHole = o; }

			static void*		New()									{ return new CUdpConnector; }

			bool				CheckRecvHeader(SUdpPacketHeader&);

	virtual bool				Connect(const char*, INT);
	virtual bool				Disconnect(const SOCKET);
	virtual void				SetConnected();
	virtual bool				Read();
//		virtual bool				ReadFrom(char&);
	virtual bool				ReadFrom(CUdpPacket&);
	virtual bool				Queuing(CCommand& o, INT p, CRYPT_TYPE q)	{ return CConnector::Queuing(o, p, q); }
//		virtual bool				Queuing(char&);

private:
			INT					WriteReliableBlock(CCommand&, INT, CRYPT_TYPE =CRYPT_NONE);
			INT					WriteReliableOrder(CCommand&, INT, CRYPT_TYPE =CRYPT_NONE);

			inline bool			IsSendHole()							{ return m_bSendHole; }

protected:
			INT					m_iSymmetricPort;
			INT					m_iHandshakeStep;
			bool				m_bSendHole;

private:
			UINT32				m_uiSendSequence;
			UINT32				m_uiRecvSequence;
			UINT32				m_uiReliableSendBlockSequence;
			UINT32				m_uiReliableRecvBlockSequence;
			UINT32				m_uiReliableSendOrderSequence;
			UINT32				m_uiReliableRecvOrderSequence;

			UINT				m_uiTopRecvPacketSequence;
			INT					m_iDontRecvPacketSequence[iMAX_UDP_RELIABLE_ACK_QUEUE];

			UINT64				m_ullLimitedConnectingTick;
			UINT64				m_ullLimitedHoleSendingTick;

			INT					m_iSendBlockAck;
			INT					m_iSendOrderAck;

};

#endif // ___NETDRONE_NET_CONNECTOR_H___

/* EOF */
