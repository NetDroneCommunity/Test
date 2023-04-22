/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_DISPATCHERLIST_H___
#define ___NETDRONE_NET_DISPATCHERLIST_H___

#include "Dispatcher.h"

#include <map>
#include <list>

#include "../GlobalVariable.h"
#include "../Defines.h"

class ISensor;

class IDispatcherList {
public:
	IDispatcherList() : m_pkCommandQueue(NULL), m_uiMaxSize(0), m_ppkDispatcherList(NULL) {}

	virtual ~IDispatcherList() {}

	virtual bool			Initialize(ICommandQueue&, ISensor&, UINT)	{ return false; }

	virtual void
	Release() {
		if(isptr(m_ppkDispatcherList)) {
			for(size_t i=0; i < GetMaxSize(); ++i) {
				if(isptr(m_ppkDispatcherList[i])) {
					m_ppkDispatcherList[i]->Disconnect();
					SAFE_DELETE_RELEASE(m_ppkDispatcherList[i]);
				} else {
					OUTPUT("critical error: [" << i << "] dispatcher is null: " << GetMaxSize());
				}
			}
			SAFE_DELETE_ARRAY(m_ppkDispatcherList);
		}
	}

	virtual void			Clear()								{}

	virtual bool			Insert(SOCKET, IDispatcher*)		{ return false; }
	virtual void			SendQueue(UINT =0)					{}

	virtual bool			CreateMainDispatcher(NETWORK_TYPE)	{ return false; }
	virtual bool			CreateDispatcher(UINT)				{ return false; }

	virtual IDispatcher*	Find(INT iIndex_)					{ if(iIndex_ < (INT)GetMaxSize()) { return m_ppkDispatcherList[iIndex_]; } return NULL; }
	virtual IDispatcher*	Empty()								{ return NULL; }

			ICommandQueue*	GetCommandQueue()					{ return m_pkCommandQueue; }
			UINT			GetMaxSize()						{ return m_uiMaxSize; }

			void
			SetHeaderCrypt(bool o) {
				if(isptr(m_ppkDispatcherList)) {
					for(UINT i=0; i < GetMaxSize(); ++i) {
						if(isptr(m_ppkDispatcherList[i])) {
							m_ppkDispatcherList[i]->SetHeaderCrypt(o);
						} else {
							OUTPUT("critical error: [" << i << "] dispatcher is null: " << GetMaxSize());
						}
					}
				}
			}

			bool
			SeparationDispatcherIndex(UINT o) {
				if((1 < o) && (o < (UINT)(iMAX_SEPARATION_INDEX)) && (0 < m_uiMaxSize) && (o < m_uiMaxSize)) {
					UINT uiSeparationNum = m_uiMaxSize / o;

					m_uiSeparationDispatcherIndex[0] = 0;
					for(UINT i=1; i < o; ++i) {
						m_uiSeparationDispatcherIndex[i] = m_uiSeparationDispatcherIndex[i-1] + uiSeparationNum;
					}
					m_uiSeparationDispatcherIndex[o] = m_uiMaxSize;
					return true;
				} else if(1 == o) {
					m_uiSeparationDispatcherIndex[0] = 0;
					m_uiSeparationDispatcherIndex[1] = m_uiMaxSize;
					return true;
				}
				return false;
			}

protected:
			ICommandQueue*	m_pkCommandQueue;
			UINT			m_uiMaxSize;

			IDispatcher**	m_ppkDispatcherList;
			UINT			m_uiSeparationDispatcherIndex[iMAX_SEPARATION_INDEX+1];

};

class CDispatcherList : public IDispatcherList {
public:
	CDispatcherList();
	virtual	~CDispatcherList();

	virtual bool			Initialize(ICommandQueue&, ISensor&, UINT);
	virtual void			Release();
	virtual void			Clear();

	virtual void			SendQueue(UINT =0);

	virtual bool			CreateMainDispatcher(NETWORK_TYPE);
	virtual bool			CreateDispatcher(UINT);

	virtual IDispatcher*	Empty();

};

class CDispatcherPair : public IDispatcherList {
public:
	CDispatcherPair();
	virtual	~CDispatcherPair();

	virtual bool			Initialize(ICommandQueue&, ISensor&, UINT);
	virtual void			Release();
	virtual void			Clear();

	virtual void			SendQueue(UINT =0);

	virtual bool			CreateMainDispatcher(NETWORK_TYPE);
	virtual bool			CreateDispatcher(UINT);

	virtual IDispatcher*	Find(INT);
	virtual IDispatcher*	Empty();

protected:
	typedef std::list< IDispatcher* >	stl_List;
	typedef stl_List::iterator			Iter;

	stl_List			m_kList;

};

#endif // ___NETDRONE_NET_DISPATCHERLIST_H___

/* EOF */
