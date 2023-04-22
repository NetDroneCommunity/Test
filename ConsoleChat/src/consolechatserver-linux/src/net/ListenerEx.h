/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_LISTENEREX_H___
#define ___SERVICE_NET_LISTENEREX_H___

#include <netdrone/CommonHeader.h>

class CDefaultListener : public CTcpListener {
public:
	CDefaultListener() {}
	virtual ~CDefaultListener() {}

			static void*	New()	{ return new CDefaultListener; }

private:

};

class CBackboneListener : public CTcpListener {
public:
	CBackboneListener() {}
	virtual ~CBackboneListener() {}

			static void*	New()	{ return new CBackboneListener; }

	virtual CTcpConnector*	SelectConnector(INT, SOCKET);

private:

};

#endif // ___SERVICE_NET_LISTENEREX_H___

/* EOF */
