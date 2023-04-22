/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_NETWORK_H___
#define ___NETDRONE_NET_NETWORK_H___

#include "Queue.h"
#include "Sensor.h"

#include "../GlobalVariable.h"
#include "../Defines.h"
#include "../Enums.h"

class CWaitEventThread;
class CSenderThread;

class CNetwork {
public:
	CNetwork();
	virtual ~CNetwork();

	virtual bool				Initialize(SENSOR_TYPE, COMMAND_TYPE, UINT =1, NETWORK_TYPE =NETWORK_TCP_SERVER, UINT =0);
	virtual bool				Release();

	virtual void				Clear();

	virtual bool				CreateWaitEventThread(UINT =100, UINT =0);
	virtual bool				CreateSenderThread(UINT =1, UINT =0);

	virtual bool				StartThread();
	virtual bool				CancelThread();

	virtual bool				ParseCommand(CNativeCommand&)			{ return false; }
	virtual bool				ParseCommand(CExtendCommand&)			{ return false; }

	virtual void*				Read()									{ return m_pkCommandQueue->Take(); }
	virtual void				Send()									{}
	virtual INT					WaitEvent(INT o)						{ return m_pkSensor->WaitEvent(o); }
	virtual void				SendQueue(UINT o =0)					{ m_pkSensor->SendQueue(o); }

			ICommandQueue*		GetCommandQueue()						{ return m_pkCommandQueue; }

			ISensor*			GetSensor()								{ return m_pkSensor; }
			bool				SetSensor(SENSOR_TYPE, UINT);
			bool				ChangeConnector(CConnector&);
			CConnector*			Create(const char*, INT, NETWORK_TYPE);
			CConnector*			Empty();

			IDispatcherList*	GetDispatcherList()						{ return m_pkSensor->GetDispatcherList(); }

			void				SetInitialize(bool o)					{ m_bInitialized = o; }
			bool				IsInitialized()							{ return m_bInitialized; }

protected:
			ICommandQueue*		m_pkCommandQueue;

private:
			ISensor*			m_pkSensor;
			bool				m_bInitialized;

#ifdef _THREAD
			CWaitEventThread*	m_pkWaitEventThread;
			CSenderThread**		m_ppkSenderThread;
			UINT				m_uiMaxSenderThread;
#endif

};

#endif // ___NETDRONE_NET_NETWORK_H___

/* EOF */
