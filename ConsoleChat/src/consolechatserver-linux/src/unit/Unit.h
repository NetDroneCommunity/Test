/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UNIT_UNIT_H___
#define ___SERVICE_UNIT_UNIT_H___

#include <netdrone/CommonHeader.h>

#include "UnitData.h"

#include "../world/RoomHandler.h"

class CNetIO;

class CUnit {
public:
	CUnit();
	virtual ~CUnit() {}

	virtual	bool				Initialize()						{ return false; }
	virtual	bool				Release()							{ return false; }

	virtual	bool				Create()							{ return true; }
	virtual	bool				Terminate()							{ return true; }

	virtual	bool				Update();

	virtual	void				Clear();
	virtual void				Disconnect()						{}

	virtual bool				ReCalculateData()					{ return true; }

	virtual	INT					Launcher(CCommand& o, INT p=0, PACKET_TYPE q=PACKET_THROW, CRYPT_TYPE r=CRYPT_NONE)	{ return -1; }
	virtual	bool				Broadcast(CCommand&, INT =0, CUnit* =NULL, CUnit* =NULL);

	virtual	CUnit*				GetTypeAs(INT);

			CRoomHandler*		GetRoomHandler()					{ return m_pkRoomHandler; }

	virtual	UINT				GetAid()							{ return 0; }

	virtual	UINT				GetKey()							{ return 0; }
	virtual	void				SetKey(UINT o)						{}

	virtual	STATUS_TYPE			GetStatus()							{ return STATUS_EXIT; }
	virtual	void				SetStatus(STATUS_TYPE o)			{}

	virtual	const char*			GetName()							{ return NULL; }
	virtual	void				SetName(const char& o)				{}

			SPlayerData&		GetPlayerData()						{ return *m_ptPlayerData; }
			SPlayerData&		GetPlayerSums()						{ return *m_ptPlayerSums; }

			void				ResetPlayingTick()					{ m_tkPlayingTick = 0; }
			void				SetPlayingTick()					{ m_tkPlayingTick = g_kTick.GetTick(); }
			tick_t				GetPlayingTick()					{ if((0 < m_tkPlayingTick) && (g_kTick.GetTick() > m_tkPlayingTick)) return (g_kTick.GetTick() - m_tkPlayingTick); else return 0; }

protected:
			SPlayerData*		m_ptPlayerData;
			SPlayerData*		m_ptPlayerSums;

			CRoomHandler*		m_pkRoomHandler;

private:
			tick_t				m_tkPlayingTick;

};

#endif // ___SERVICE_UNIT_UNIT_H___

/* EOF */
