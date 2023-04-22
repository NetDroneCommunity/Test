/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_QUEUE_H___
#define ___NETDRONE_NET_QUEUE_H___

#include <queue>
#include <list>

#include "Buffer.h"

#include "../util/Locker.h"

#include "../Defines.h"
#include "../Enums.h"

class ICommandQueue {
public:
	ICommandQueue() {}
	virtual ~ICommandQueue() {}

	virtual void		Clear()											{}
	virtual bool		Empty()											{ return true; }

	virtual size_t		Size()											{ return 0; }

	virtual void*		Take()											{ return NULL; }
	virtual bool		Put(CConnector&, CCommand&, INT, CRYPT_TYPE)	{ return true; }

			void		Lock()											{ m_kLocker.Lock(); }
			void		Unlock()										{ m_kLocker.Unlock(); }

protected:
	CLocker				m_kLocker;

};

class CNativeCommandQueue : public ICommandQueue {
public:
	CNativeCommandQueue();
	virtual ~CNativeCommandQueue();

	virtual void		Clear();

	virtual bool		Empty();

	virtual size_t		Size();

	virtual void*		Take();
	virtual bool		Put(CConnector&, CCommand&, INT, CRYPT_TYPE);

private:
	typedef std::deque< CNativeCommand* >	stl_Deque;
	stl_Deque			m_kDeque;

};

class CExtendCommandQueue : public ICommandQueue {
public:
	CExtendCommandQueue();
	virtual ~CExtendCommandQueue();

	virtual void		Clear();

	virtual bool		Empty();

	virtual size_t		Size();

	virtual void*		Take();
	virtual bool		Put(CConnector&, CCommand&, INT, CRYPT_TYPE);

private:
	typedef std::deque< CExtendCommand* >	stl_Deque;
	stl_Deque			m_kDeque;

};

class CBlockQueue {
public:
	CBlockQueue();
	~CBlockQueue();

	bool				Clear();

	size_t				Size();

	bool				PutStream(const char&, size_t);
	bool				Pop();

	CTcpPacketBuffer*	Peek();

private:
	typedef std::deque< CTcpPacketBuffer* >	stl_Deque;
	stl_Deque			m_kDeque;

};

class CSendBlockQueue {
public:
	CSendBlockQueue();
	~CSendBlockQueue();

	bool				Clear();

	size_t				Size();
	bool				ResetIterator();
	bool				Push(CUdpPacketBuffer&);
	void				Pop(UINT32);
	void				PopAll(UINT32);
	void				DelayedTick(UINT);

	CUdpPacketBuffer*	PeekNext();

private:
	typedef std::list< CUdpPacketBuffer* > stl_List;
	stl_List			m_kList;
	stl_List::iterator	m_Iter;

};

class CSendOrderQueue {
public:
	CSendOrderQueue();
	~CSendOrderQueue();

	bool				Clear();

	size_t				Size();
	bool				ResetIterator();
	bool				Push(CUdpPacketBuffer&);
	void				Pop(UINT32);
	void				PopAll(UINT32);
	void				DelayedTick(UINT);

	CUdpPacketBuffer*	PeekNext();

private:
	typedef std::list< CUdpPacketBuffer* > stl_List;
	stl_List			m_kList;
	stl_List::iterator	m_Iter;
};

class CRecvBlockQueue {
public:
	CRecvBlockQueue();
	~CRecvBlockQueue();

	bool				Clear();

	size_t				Size();
	bool				Empty();
	INT					Push(CUdpPacket&);
	CUdpPacketBuffer*	PopFront(UINT);

private:
	typedef std::list< CUdpPacketBuffer* > stl_List;
	stl_List			m_kList;
	//size_t				m_stFrontIndex;
	size_t				m_stBackIndex;

};

class CRecvOrderQueue {
public:
	CRecvOrderQueue();
	~CRecvOrderQueue();

	bool				Clear();

	size_t				Size();
	bool				Empty();
	INT					Push(CUdpPacket&);
	CUdpPacketBuffer*	PopFront(UINT);

private:
	typedef std::list< CUdpPacketBuffer* > stl_List;
	stl_List			m_kList;
	//size_t				m_stFrontIndex;
	size_t				m_stBackIndex;

};

#endif // ___NETDRONE_NET_QUEUE_H___

/* EOF */
