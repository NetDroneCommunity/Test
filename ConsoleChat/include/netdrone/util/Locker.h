/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_LOCKER_H___
#define ___NETDRONE_UTIL_LOCKER_H___

#include <pthread.h>

#include "../Defines.h"

#define LOCKING(o)	CLocking locking(o)

class CMutex {
public:
	CMutex() {}
	virtual ~CMutex() {}

	pthread_mutex_t&	GetMutex()	{ return m_tMutex; }

	virtual bool
	Init() {
		pthread_mutex_init(&m_tMutex, NULL);
		return true;
	}

	virtual bool
	Destroy() {
		if(0 == pthread_mutex_destroy(&m_tMutex)) {
			return true;
		}
		return false;
	}

	virtual bool
	Lock() {
		if(0 == pthread_mutex_lock(&m_tMutex)) {
			return true;
		}
		return false;
	}

	virtual bool
	TryLock() {
		if(0 == pthread_mutex_trylock(&m_tMutex)) {
			return true;
		}
		return false;
	}

	virtual bool
	Unlock() {
		if(0 == pthread_mutex_unlock(&m_tMutex)) {
			return true;
		}
		return false;
	}

private:
	pthread_mutex_t	m_tMutex;

};

class CLocker : public CMutex {
public:
	CLocker() { Init(); }
	~CLocker() { Destroy(); }
};

class CLocking {
public:
	CLocking(CLocker& o) : m_rkLocker(o) {
		m_rkLocker.Lock();
	}

	~CLocking() {
		m_rkLocker.Unlock();
	}

private:
	CLocker&	m_rkLocker;

};

#endif // ___NETDRONE_UTIL_LOCKER_H___

/* EOF */
