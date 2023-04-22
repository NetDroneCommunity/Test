/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_THREAD_H___
#define ___NETDRONE_NET_THREAD_H___

#include "Network.h"

#include "../util/Threading.h"

#include "../Defines.h"

class CNetworkThread : public CSystemThread {
public:
	CNetworkThread(CNetwork& o) : m_rkNetwork(o) {
		m_kAttribute.SetName("CNetworkThread");
		m_kAttribute.SetStackSize(1024);
		m_kAttribute.SetJoinable(true);

		Initialize(&m_kAttribute);
	}

	virtual ~CNetworkThread() {}

			void		SetName(char* o)	{ if(isptr(o)) { m_kAttribute.SetName(o); } }
			const char*	GetName()			{ return m_kAttribute.GetName(); }

protected:
	virtual void*		GetHandler()		{ return NULL; }

			CNetwork&	GetNetwork()		{ return m_rkNetwork; }

private:
	CNetwork&			m_rkNetwork;
	CThreadAttribute	m_kAttribute;

};

class CWaitEventThread : public CNetworkThread {
public:
	CWaitEventThread(CNetwork& o, INT p =50, INT q =0) : CNetworkThread(o), m_iWaitTime(p), m_iSleepTime(q) {}
	virtual ~CWaitEventThread() {}

protected:
	virtual void*
	GetHandler() {
		for( ; ; ) {
			if(0 > GetNetwork().WaitEvent(m_iWaitTime)) {
				TRACE("waitevent: false");
				usleep(2000000);
			}
			usleep(m_iSleepTime);
		}
		return NULL;
	}

private:
	INT		m_iWaitTime;
	INT		m_iSleepTime;

};

class CSenderThread : public CNetworkThread {
public:
	CSenderThread(CNetwork& o, UINT p =0, INT q =0) : CNetworkThread(o), m_uiThreadNumber(p), m_iSleepTime(q) {}
	virtual ~CSenderThread() {}

protected:
	virtual void*	GetHandler() {
		for( ; ; ) {
			INT iSleepTime = m_iSleepTime;

			if(iSleepTime <= 0) {
				ISensor* pkSensor = GetNetwork().GetSensor();
				if(isptr(pkSensor)) {
					if(10 > pkSensor->GetCount()) {
						iSleepTime = 10000;
					} else if(50 > pkSensor->GetCount()) {
						iSleepTime = 1000;
					}
				}
			}

			GetNetwork().SendQueue(m_uiThreadNumber);
			usleep(iSleepTime);
		}
		return NULL;
	}

private:
	UINT	m_uiThreadNumber;
	INT		m_iSleepTime;
};

#endif // ___NETDRONE_NET_THREAD_H___

/* EOF */
