/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_CHANNELMGR_H___
#define ___SERVICE_WORLD_CHANNELMGR_H___

#include <netdrone/CommonHeader.h>

#include "../GlobalVariable.h"

#include "Room.h"

#define g_kChannelMgr CChannelMgr::GetInstance()

class CChannelMgr : public CSingleton< CChannelMgr > {
public:
	CChannelMgr() {
		for(UINT i = 0; i < (UINT)(iMAX_CHANNEL_LIST); ++i) {
			for(UINT j = 0; j < (UINT)(iMAX_CHANNEL_USERS); ++j) {
				m_pkUserList[i][j] = NULL;
				m_iEmptyUserList[i][j] = -1;
			}

			m_iUserMaxCount[i] = 0;

			m_iUserTopIndex[i] = 0;
			m_iEmptyUserCount[i] = 0;

			m_iUsedRoomCount[i] = 0;

			m_iRoomTopIndex[i] = 0;
			m_iEmptyRoomCount[i] = 0;
		}
	}

	virtual ~CChannelMgr() {
		for(UINT i = 0; i < (UINT)(iMAX_CHANNEL_LIST); ++i) {
			for(UINT j = 0; j < (UINT)(iMAX_ROOMS); ++j) {
				SAFE_DELETE_RELEASE(m_pkRoomList[i][j]);
			}
		}
	}

	bool			Initialize();
	bool			Release();

	CRoom*			NewRoom(INT);
	bool			DeleteRoom(UINT16);

	CRoom*			FindRoom(UINT16);

	bool			InUser(INT, CPlayer&);
	void			OutUser(INT, CPlayer&);

	void			GetRoomList(CPlayer&);
	bool			GetRoomMemberList(CPlayer&);
	void			GetUserList(CPlayer&);

	SChannelInfo*	GetChannelInfo()				{ return m_tServerInfo.GetChannelInfo(); }
	SChannelInfo&	GetChannelInfo(INT o)			{ return m_tServerInfo.GetChannelInfo(o); }
	SServerInfo&	GetServerInfo()					{ return m_tServerInfo; }

	void			SetMaxChannel(INT o)			{ m_tServerInfo.SetMaxChannel(o); }
	INT				GetMaxChannel()					{ return m_tServerInfo.GetMaxChannel(); }

	INT				GetUsedRoomCount(INT o)			{ return m_iUsedRoomCount[o]; }
	INT				GetRoomTopIndex(INT o)			{ return m_iRoomTopIndex[o]; }
	INT				GetEmptyRoomCount(INT o)		{ return m_iEmptyRoomCount[o]; }

	void			SendNotify(UINT, char&, UINT);
	void			SendNotify(CCommand&, INT =0, CPlayer* =NULL);

	void			Broadcast(CPlayer*, CCommand&, INT =0);
	void			BroadcastChannel(INT, CCommand&, INT =0, CPlayer* =NULL);
	void			BroadcastAll(CCommand&, INT =0, CPlayer* =NULL);

	void			Shutdown();

private:
	CRoom*		m_pkRoomList[iMAX_CHANNEL_LIST][iMAX_ROOMS];
	INT			m_iEmptyRoomList[iMAX_CHANNEL_LIST][iMAX_ROOMS];

	INT			m_iUsedRoomCount[iMAX_CHANNEL_LIST];
	INT			m_iRoomTopIndex[iMAX_CHANNEL_LIST];
	INT			m_iEmptyRoomCount[iMAX_CHANNEL_LIST];

	CPlayer*	m_pkUserList[iMAX_CHANNEL_LIST][iMAX_CHANNEL_USERS];
	INT			m_iEmptyUserList[iMAX_CHANNEL_LIST][iMAX_CHANNEL_USERS];

	INT			m_iUserMaxCount[iMAX_CHANNEL_LIST];
	INT			m_iUserTopIndex[iMAX_CHANNEL_LIST];
	INT			m_iEmptyUserCount[iMAX_CHANNEL_LIST];

	SServerInfo	m_tServerInfo;

};

#endif // ___SERVICE_WORLD_CHANNELMGR_H___

/* EOF */
