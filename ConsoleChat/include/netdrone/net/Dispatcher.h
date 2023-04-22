/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_DISPATCHER_H___
#define ___NETDRONE_NET_DISPATCHER_H___

#include "../GlobalVariable.h"
#include "../Defines.h"
#include "../Enums.h"

class ICommandQueue;
class ISensor;
class CConnector;

class IDispatcher {
public:
	IDispatcher() : m_pkCommandQueue(NULL), m_pkSensor(NULL), m_pkMainConnector(NULL), m_iIndex(-1) {}
	virtual ~IDispatcher() {}

	virtual bool			Initialize(ICommandQueue& o, ISensor& p)	{ m_pkCommandQueue = &o; m_pkSensor = &p; return true; }
	virtual void			Release()									{}

	virtual bool			Disconnect()								{ TRACE("disconnecting failed: "); return false; }

	virtual bool			Read()										{ TRACE("reading failed: "); return false; }
	virtual bool			SendQueue()									{ TRACE("SendQueue failed: "); return false; }

	virtual void			SetHeaderCrypt(bool)						{}

	virtual void			CreateMainConnector(NETWORK_TYPE)			{}
	virtual void			CreateConnector(UINT)						{}

	virtual bool			SetMainConnector(CConnector&)				{ return false; }
	virtual bool			SetConnector(CConnector&)					{ return false; }
	virtual CConnector*		GetConnector(SOCKET)						{ return NULL; }

			CConnector*		GetMainConnector()							{ return m_pkMainConnector; }

			void			ClearMainConnector()						{ m_pkMainConnector = NULL; }
			ICommandQueue*	GetCommandQueue()							{ return m_pkCommandQueue; }

			void			SetIndex(INT o)								{ m_iIndex = o; }
			INT				GetIndex()									{ return m_iIndex; }

protected:
			ICommandQueue*	m_pkCommandQueue;
			ISensor*		m_pkSensor;
			CConnector*		m_pkMainConnector;

private:
			INT				m_iIndex;

};

class CDispatcher : public IDispatcher {
public:
	CDispatcher() : m_ppkConnectors(NULL), m_uiMaxSize(0) {}
	virtual ~CDispatcher() {}

	virtual void			Release();

	virtual bool			Disconnect();

	virtual bool			Read();
	virtual bool			SendQueue();

	virtual void			SetHeaderCrypt(bool);

	virtual void			CreateMainConnector(NETWORK_TYPE);
	virtual void			CreateConnector(UINT);

	virtual bool			SetMainConnector(CConnector&);
	virtual bool			SetConnector(CConnector&);

	virtual CConnector*		GetConnector(SOCKET);

private:
			UINT			GetMaxSize()		{ return m_uiMaxSize; }
			void			SetMaxSize(UINT o)	{ m_uiMaxSize = o; }

protected:
			CConnector**	m_ppkConnectors;

private:
			UINT			m_uiMaxSize;

};

#endif // ___NETDRONE_NET_DISPATCHER_H___

/* EOF */
