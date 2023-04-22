/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UNIT_PLAYEREX_H___
#define ___SERVICE_UNIT_PLAYEREX_H___

#include "Player.h"

#include "../net/NetIO.h"

class CPlayerEx : public CPlayer {
public:
	CPlayerEx() : m_pkNetIO(NULL), m_iDbIndex(0) {
	}

	virtual ~CPlayerEx() {}

	virtual	bool				Initialize();
	virtual	bool				Release();

	virtual	void				Clear();
	virtual void				Disconnect();

	virtual	INT					Launcher(CCommand& o, INT p=0, PACKET_TYPE q=PACKET_THROW, CRYPT_TYPE r=CRYPT_NONE)	{ if(isptr(m_pkNetIO)) { return m_pkNetIO->Send(o, p, q, r); } return -1; }

	virtual	bool				ReCalculateData();

	INT							GetDbIndex()					{ return m_iDbIndex; }
	void						SetDbIndex(INT o)				{ m_iDbIndex = o; }

	CNetIO*						GetNetIO()						{ return m_pkNetIO; }

	static void*				New()							{ return new CPlayerEx; }

protected:
			CNetIO*				m_pkNetIO;

private:
			INT					m_iDbIndex;

};

#endif	// ___SERVICE_UNIT_PLAYEREX_H___

/* EOF */
