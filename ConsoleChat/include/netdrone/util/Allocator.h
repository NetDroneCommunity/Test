/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_ALLOCATOR_H___
#define ___NETDRONE_UTIL_ALLOCATOR_H___

#include <cassert>

#include "Locker.h"
#include "../Defines.h"

template <typename T>
class CAllocator {
public:
	CAllocator() : m_NextOfPool(NULL) {}
	virtual ~CAllocator() {}

	static void		FreeAllPool();
	static T*		New();
	static void		Delete(T*);

	static T*		ms_HeadOfPool;
	T*				m_NextOfPool;

private:
	static CLocker	ms_kLocker;
};

//template <typename T>
//T* CAllocator<T>::ms_HeadOfPool = NULL;

template <typename T>
CLocker CAllocator<T>::ms_kLocker;

template <typename T>
void CAllocator<T>::FreeAllPool() {
	T* Instance;

	LOCKING(ms_kLocker);

	//INT iCount = 0;
	while(NULL != ms_HeadOfPool) {
		Instance = ms_HeadOfPool;
		ms_HeadOfPool = ms_HeadOfPool->m_NextOfPool;
		delete Instance;
		Instance = NULL;
		//TRACE("free: count: " << iCount);
		//++iCount;
	}

	assert(!ms_HeadOfPool);
}

template <typename T>
T* CAllocator<T>::New() {
	T* Instance;

	LOCKING(ms_kLocker);

	if(NULL != ms_HeadOfPool) {
		Instance = ms_HeadOfPool;
		ms_HeadOfPool = ms_HeadOfPool->m_NextOfPool;
		Instance->m_NextOfPool = NULL;
		//TRACE("renew: "<<Instance);
	} else {
		Instance = new T();
		//TRACE("new: "<<Instance);
	}
	return Instance;
}

template <typename T>
void CAllocator<T>::Delete(T* o) {
	LOCKING(ms_kLocker);

	if(NULL != o) {
		//T* Instance = static_cast<T*>(o);
		T* Instance = o;

		if(NULL == Instance->m_NextOfPool) {
			Instance->m_NextOfPool = ms_HeadOfPool;
			ms_HeadOfPool = Instance;
		} else {
			TRACE("Allocation is error: duplicated: ");
		}
	}
}

#endif	// ___NETDRONE_UTIL_ALLOCATOR_H___

/* EOF */
