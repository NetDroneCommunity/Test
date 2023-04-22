/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_SINGLETON_H___
#define ___NETDRONE_UTIL_SINGLETON_H___

#include "Locker.h"

#include "../Defines.h"

template <typename T>
class CSingleton {
public:
	CSingleton() {}
	virtual ~CSingleton() {}

	static T* GetInstancePtr() {
		if(false == isptr(ms_Instance)) {
			LOCKING(ms_kLocker);
			if(false == isptr(ms_Instance)) {
				ms_Instance = new T();
			}
		}

		return (ms_Instance);
	}

	static T& GetInstance() {
		if(false == isptr(ms_Instance)) {
			LOCKING(ms_kLocker);
			if(false == isptr(ms_Instance)) {
				ms_Instance = new T();
			}
		}

		return (*ms_Instance);
	}

	static void ReleaseInstance() {
		if(isptr(ms_Instance)) {
			LOCKING(ms_kLocker);
			if(isptr(ms_Instance)) {
				delete ms_Instance;
				ms_Instance = NULL;
			}
		}
	}

private:
	static T*		ms_Instance;
	static CLocker	ms_kLocker;
};

//template <typename T>
//T* CSingleton<T>::ms_Instance = NULL;

template <typename T>
CLocker CSingleton<T>::ms_kLocker;

#endif // ___NETDRONE_UTIL_SINGLETON_H___

/* EOF */
