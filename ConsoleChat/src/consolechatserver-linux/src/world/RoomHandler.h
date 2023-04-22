/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_WORLD_ROOMHANDLER_H___
#define ___SERVICE_WORLD_ROOMHANDLER_H___

#include <netdrone/CommonHeader.h>

class CRoom;
class CUnit;

class CRoomHandler {
public:
	CRoomHandler(CUnit& o) : m_rkBody(o), m_pkRoom(NULL), m_iOffset(-1) {}
	virtual ~CRoomHandler() {}

	virtual bool	Join()				{ return true; }
	virtual void	Leave(CUnit* =NULL)	{}

	virtual void	ChangeLeader()		{}

	virtual void	Clear()				{}

	void	SetOffset(INT o)			{ m_iOffset = o; }
	INT		GetOffset()					{ return m_iOffset; }

	void	SetRoom(CRoom* o)			{ m_pkRoom = o; }
	CRoom*	GetRoom()					{ return m_pkRoom; }

	bool	InRoom()					{ return (isptr(GetRoom())); }

	virtual bool	Broadcast(CCommand&, INT =0, CUnit* =NULL, CUnit* =NULL)	{ return true; }

protected:
			CUnit&	GetBody()			{ return m_rkBody; }

private:
			CUnit&	m_rkBody;
			CRoom*	m_pkRoom;
			INT		m_iOffset;

};

#endif	// ___SERVICE_WORLD_ROOMHANDLER_H___

/* EOF */
