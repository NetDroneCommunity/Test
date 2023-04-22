/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_ROOM_H___
#define ___SERVICE_WORLD_ROOM_H___

#include <netdrone/CommonHeader.h>

#include "../db/DbData.h"

class CCommand;
class CUnit;
class CPlayer;

class CRoom {
public:
	CRoom() : m_uiTopCount(0), m_usId(0), m_uiStageId(0), m_bOffset(false), m_uiMaxUser(0) {
		memset(m_pkMembers, 0, sizeof(m_pkMembers));
		memset(m_szName, 0, sizeof(m_szName));
	}

	virtual ~CRoom() {}

	virtual	bool		Initialize();
	virtual	bool		Release();
	virtual	void		Clear();

	virtual bool		Update()					{ return true; }

	virtual	bool		Create(CUnit&)				{ return true; }
	virtual	bool		Join(CUnit&)				{ return true; }
	virtual	bool		Leave(CUnit&)				{ return true; }
	virtual bool		ChangeLeader(CUnit&)		{ return true; }

	virtual	bool		Broadcast(CCommand&, INT =0, CUnit* =NULL, CUnit* =NULL)	{ return true; }

			CPlayer*	GetMember(UINT o)			{ if(o < (UINT)(iMAX_ROOM_MEMBERS)) return m_pkMembers[o]; else return NULL; }
			CPlayer*	GetLeader()					{ return m_pkMembers[0]; }

			const char*	GetName()					{ return m_szName; }
			void		SetName(const char* o)		{ memset(m_szName, 0, iROOM_NAME_LEN); memcpy(m_szName, o, iROOM_NAME_LEN); m_szName[iROOM_NAME_LEN] = '\0'; }

			void		SetId(UINT o)				{ m_usId = o; }
			UINT		GetId()						{ return m_usId; }

			INT			GetIndex()					{ return (UINT16(m_usId) & 0x03FF); }
			INT			GetChannelIndex()			{ return (UINT32(m_usId >> 10) -1); }

			UINT		GetStageId()				{ return m_uiStageId; }
			void		SetStageId(UINT o)			{ m_uiStageId = o; }

			void		SetDoing(bool o)			{ m_bOffset = o; }
			bool		IsDoing()					{ return m_bOffset; }

			bool		IsAvailable()				{ return (0 < m_uiTopCount); }

			void		SetMaxUser(UINT o)			{ if((UINT)(iMAX_ROOM_MEMBERS) > o) { m_uiMaxUser = o; } else { m_uiMaxUser = iMAX_ROOM_MEMBERS; } }
			UINT		GetMaxUser()				{ return m_uiMaxUser; }

			UINT		GetTopCount()				{ return m_uiTopCount; }

protected:
			void		IncreasedTopCount()			{ ++m_uiTopCount; }
			void		DecreasedTopCount()			{ --m_uiTopCount; }

			void		SetTopCount(UINT o)			{ m_uiTopCount = o; }

			bool		InRange(UINT o)				{ return (o < m_uiTopCount); }

protected:
			CPlayer*	m_pkMembers[iMAX_ROOM_MEMBERS];

			UINT		m_uiTopCount;

private:
			UINT16		m_usId;
			UINT		m_uiStageId;
			bool		m_bOffset;
			UINT		m_uiMaxUser;

			char		m_szName[iROOM_NAME_LEN+1];

};

#endif	// ___SERVICE_WORLD_ROOM_H___

/* EOF */
