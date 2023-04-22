/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_ROOMHANDLEREX_H___
#define ___SERVICE_WORLD_ROOMHANDLEREX_H___

#include <netdrone/CommonHeader.h>

#include "RoomHandler.h"

class CRoomHandlerEx : public CRoomHandler {
public:
	CRoomHandlerEx(CUnit& o) : CRoomHandler(o) {}
	virtual ~CRoomHandlerEx() {}

	virtual bool	Join();
	virtual void	Leave(CUnit* =NULL);

	virtual void	ChangeLeader();

	virtual void	Clear();

	virtual bool	Broadcast(CCommand&, INT =0, CUnit* =NULL, CUnit* =NULL);

protected:

private:

};

#endif	// ___SERVICE_WORLD_ROOMHANDLEREX_H___

/* EOF */
