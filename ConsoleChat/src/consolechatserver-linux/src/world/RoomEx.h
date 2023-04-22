/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_ROOMEX_H___
#define ___SERVICE_WORLD_ROOMEX_H___

#include <netdrone/CommonHeader.h>

#include "Room.h"

class CRoomEx : public CRoom {
public:
	CRoomEx() {}
	virtual ~CRoomEx() {}

	virtual	bool						Initialize();
	virtual	bool						Release();
	virtual	void						Clear();

	virtual bool						Update();

	virtual	bool						Create(CUnit&);
	virtual	bool						Join(CUnit&);
	virtual	bool						Leave(CUnit&);

	virtual bool						ChangeLeader(CUnit&);

	virtual	bool						Broadcast(CCommand&, INT =0, CUnit* =NULL, CUnit* =NULL);

			static void*				New()			{ return new CRoomEx; }

private:

};

#endif	// ___SERVICE_WORLD_ROOMEX_H___

/* EOF */
