/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UNIT_PLAYER_H___
#define ___SERVICE_UNIT_PLAYER_H___

#include "Unit.h"

class CPlayer : public CUnit {
public:
	CPlayer();
	virtual ~CPlayer()			{}

	virtual	bool				Initialize();
	virtual	bool				Release();

	virtual	void				Clear();
	virtual void				Disconnect();

	virtual	INT					Launcher(CCommand& o, INT p=0, PACKET_TYPE q=PACKET_THROW, CRYPT_TYPE r=CRYPT_NONE)	{ return -1; }

	virtual	CUnit*				GetTypeAs(INT);

	virtual	bool				ReCalculateData();

			void				ClearPlayerData()				{ GetPlayerData().ClearPlayerData(); }
			void				SetPlayerData(SPlayerData& o)	{ GetPlayerData().Reset(o); }
			void				SetPlayerSums(SPlayerData& o)	{ GetPlayerSums().Reset(o); }

			INT					GetChannelIndex()			{ return m_iChannelIndex; }
			void				SetChannelIndex(INT o)		{ m_iChannelIndex = o; }

			INT					GetUserIndex()				{ return m_iUserIndex; }
			void				SetUserIndex(INT o)			{ m_iUserIndex = o; }

	virtual	UINT				GetAid()					{ return GetPlayerData().GetAid(); }
			void				SetAid(UINT o)				{ GetPlayerData().SetAid(o); }

	virtual	UINT				GetKey()					{ return GetPlayerData().GetKey(); }
	virtual	void				SetKey(UINT o)				{ GetPlayerData().SetKey(o); }

	virtual	STATUS_TYPE			GetStatus()					{ return GetPlayerData().GetStatus(); }
	virtual	void				SetStatus(STATUS_TYPE o)	{ GetPlayerData().SetStatus(o); }

			const char*			GetLoginId()				{ return m_szLoginId; }
			void				SetLoginId(const char& o)	{ memcpy(m_szLoginId, &o, iLOGIN_ID_LEN); m_szLoginId[iLOGIN_ID_LEN] = '\0'; }

	virtual	const char*			GetName()					{ return GetPlayerData().GetName(); }
	virtual	void				SetName(const char& o)		{ GetPlayerData().SetName(o); }

private:
			INT			m_iUserIndex;
			INT			m_iChannelIndex;

			char		m_szLoginId[iLOGIN_ID_LEN+1];

};

#endif	// ___SERVICE_UNIT_PLAYER_H___

/* EOF */
