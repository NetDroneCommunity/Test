/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_THREADING_H___
#define ___NETDRONE_UTIL_THREADING_H___

#include <pthread.h>
#include <unistd.h>

#include "../Defines.h"

//
// Thread Attribute
//
class IThreadAttribute {
public:
	virtual ~IThreadAttribute() {}
	virtual bool	SetName(const char*) = 0;
	virtual char*	GetName() = 0;

	virtual bool	SetJoinable(bool) = 0;
	virtual bool	IsJoinable() = 0;

	virtual bool	SetStackSize(size_t) = 0;
	virtual size_t	GetStackSize() = 0;

};

class CThreadAttribute : IThreadAttribute {
public:
	CThreadAttribute();
	virtual ~CThreadAttribute();

	virtual bool	SetName(const char*);
	virtual char*	GetName();

	virtual bool	SetJoinable(bool);
	virtual bool	IsJoinable();

	virtual bool	SetStackSize(size_t);
	virtual size_t	GetStackSize();

	virtual INT		SetScope(INT);
	virtual INT		GetScope();

	pthread_attr_t&	GetAttribute()	{ return m_tAttribute; }

protected:
	pthread_attr_t	m_tAttribute;
	char*			m_pName;

};

//
// Thread
//
class IThreading {
public:
	IThreading() : m_bCreated(false) {}
	virtual ~IThreading() {}

			bool			IsCreated()			{ return m_bCreated; }
			void			SetCreated(bool o)	{ m_bCreated = o; }

	virtual bool			SetAffinity(void*) = 0;

	virtual	bool			Start() = 0;
	virtual bool			Detach() = 0;
	virtual bool			Join(void**) = 0;
	virtual bool			Cancel() = 0;
	virtual void			Yield() = 0;

protected:
	virtual void			Exit(void*) = 0;
	virtual void*			GetHandler() = 0;

			//static void*	CallThread(void*);

private:
	bool	m_bCreated;

};

class CPosixThread : public IThreading {
public:
	CPosixThread() : m_uiThreadId(0), m_pkAttribute(NULL) {}
	virtual ~CPosixThread() {}

	virtual bool			Initialize(CThreadAttribute* =NULL);

	virtual bool			SetAffinity(void*);

	virtual bool			Start();
	virtual bool			Detach();
	virtual bool			Join(void**);
	virtual bool			Cancel();
	virtual void			Yield();

protected:
	virtual void			Exit(void*);
	virtual void*			GetHandler();

			static void*	CallThread(void*);

private:
	pthread_t			m_uiThreadId;
	CThreadAttribute*	m_pkAttribute;

};

//
// Thread Mutex
//
class IThreadMutex {
public:
	virtual ~IThreadMutex() {}

	virtual bool	Init() = 0;
	virtual bool	Destroy() = 0;
	virtual bool	Lock() = 0;
	virtual bool	TryLock() = 0;
	virtual bool	Unlock() = 0;

};

class CThreadMutex : public IThreadMutex {
public:
	CThreadMutex() {}
	virtual ~CThreadMutex() {}

			pthread_mutex_t&	GetMutex()	{ return m_tMutex; }

	virtual bool				Init();
	virtual bool				Destroy();
	virtual bool				Lock();
	virtual bool				TryLock();
	virtual bool				Unlock();

private:
	pthread_mutex_t	m_tMutex;

};

//
// Thread Cond
//
class IThreadCond {
public:
	virtual ~IThreadCond() {}

	virtual bool	Init() = 0;
	virtual bool	Destroy() = 0;
	virtual bool	Signal() = 0;
	virtual bool	Broadcast() = 0;
	virtual bool	Wait() = 0;
	virtual INT		TimeWait(timespec*) = 0;

};

class CThreadCond : public IThreadCond {
public:
	CThreadCond(IThreadMutex& o) : m_rkMutex((CThreadMutex&)o) {}
	virtual ~CThreadCond() {}

	virtual bool	Init();
	virtual bool	Destroy();
	virtual bool	Signal();
	virtual bool	Broadcast();
	virtual bool	Wait();
	virtual INT		TimedWait(timespec*);

private:
	CThreadMutex&	m_rkMutex;
	pthread_cond_t	m_tCond;

};

typedef CPosixThread	CSystemThread;

#endif // ___NETDRONE_UTIL_THREADING_H___

/* EOF */
