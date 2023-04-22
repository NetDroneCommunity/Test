/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_SENSOR_H___
#define ___NETDRONE_NET_SENSOR_H___

#include "DispatcherList.h"

#include <sys/epoll.h>

#include "../Defines.h"
#include "../Enums.h"

class ISensor {
public:
	ISensor() : m_pkDispatcherList(NULL), m_uiCount(0), m_bInitialized(false) {}
	virtual ~ISensor() {}

	virtual bool				Initialize(UINT, ICommandQueue&)				{ return true; }
	virtual void				Release()										{}
	virtual void				Clear()											{ m_uiCount = 0; }

	virtual INT					WaitEvent(INT)									{ return -1; }
	virtual void				SendQueue(UINT =0)								{}

	virtual bool				CreateMainDispatcher(NETWORK_TYPE)				{ return false; }
	virtual bool				CreateDispatcher(UINT)							{ return false; }

	virtual bool				Register(const INT, const SOCKET)				{ return false; }
	virtual bool				Unregister(const INT, const SOCKET)				{ return false; }

	virtual SOCKET				GetSubSocket(UINT)								{ return INVALID_SOCKET; }

	virtual CConnector*			Empty()											{ return NULL; }
	virtual CConnector*			Find(UINT, SOCKET)								{ return NULL; }
	virtual CConnector*			Find(INT, SOCKET)								{ return NULL; }

	virtual CConnector*			CreateListen(const char*, INT, NETWORK_TYPE)	{ return NULL; }
	virtual CConnector*			CreateConnector(const char*, INT, NETWORK_TYPE)	{ return NULL; }
	virtual bool				ChangeConnector(CConnector&)					{ return false; }

			IDispatcherList*	GetDispatcherList()								{ return m_pkDispatcherList; }

			UINT				GetCount()										{ return m_uiCount; }
			void				SetCount(UINT o)								{ m_uiCount = o; }

protected:
			void				SetInitialize(bool o)							{ m_bInitialized = o; }
			bool				IsInitialized()									{ return m_bInitialized; }

			void				IncreasedCount()								{ ++m_uiCount; }
			void				DecreasedCount()								{ --m_uiCount; }

protected:
			IDispatcherList*	m_pkDispatcherList;

private:
			UINT				m_uiCount;
			bool				m_bInitialized;

};

class CNetworkSensor : public ISensor {
public:
	CNetworkSensor();
	virtual ~CNetworkSensor();

	virtual void		Clear();

	virtual void		SendQueue(UINT o =0)	{ m_pkDispatcherList->SendQueue(o); }

	virtual bool		CreateMainDispatcher(NETWORK_TYPE);
	virtual bool		CreateDispatcher(UINT);

	virtual CConnector*	Empty();
	virtual CConnector*	Find(INT, SOCKET);

	virtual CConnector*	CreateListen(const char*, INT, NETWORK_TYPE);
	virtual CConnector*	CreateConnector(const char*, INT, NETWORK_TYPE);
	virtual bool		ChangeConnector(CConnector&);

private:
			//UINT		m_uiConnectorCount;

};

class CEpollSensor : public CNetworkSensor {
public:
	CEpollSensor();
	virtual ~CEpollSensor();

	virtual bool	Initialize(UINT, ICommandQueue&);
	virtual void	Release();
	virtual void	Clear();
	virtual INT		WaitEvent(INT);

	virtual bool	Register(const INT, const SOCKET);
	virtual bool	Unregister(const INT, const SOCKET);

private:
	inline void		SetMaxConnection(UINT o)	{ m_uiMaxConnection = o; }
	inline UINT		GetMaxConnection()			{ return m_uiMaxConnection; }

	inline bool		IsEPOLLIN(INT o)			{ return (EPOLLIN == (m_pstEvents[o].events & EPOLLIN)); }
	inline bool		IsEPOLLPRI(INT o)			{ return (EPOLLPRI == (m_pstEvents[o].events & EPOLLPRI)); }
	inline bool		IsEPOLLHUP(INT o)			{ return (EPOLLHUP == (m_pstEvents[o].events & EPOLLHUP)); }
	inline bool		IsEPOLLERR(INT o)			{ return (EPOLLERR == (m_pstEvents[o].events & EPOLLERR)); }
	inline bool		IsEPOLLOUT(INT o)			{ return (EPOLLOUT == (m_pstEvents[o].events & EPOLLOUT)); }

	inline bool		InRange(UINT o)				{ return (o < GetMaxConnection()); }

private:
			UINT			m_uiMaxConnection;
			SOCKET			m_skHandle;
			epoll_event*	m_pstEvents;

};

#endif // ___NETDRONE_NET_SENSOR_H___

/* EOF */
