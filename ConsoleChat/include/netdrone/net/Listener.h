/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_LISTENER_H___
#define ___NETDRONE_NET_LISTENER_H___

#include "Connector.h"

#include "../GlobalVariable.h"
#include "../Defines.h"

class CTcpListener : public CTcpConnector {
public:
	CTcpListener() {}
	virtual ~CTcpListener() {}

	virtual bool			KeepAlive()						{ return true; }

			static void*	New()							{ return new CTcpListener; }

	virtual INT				Listen(const char*, INT);
	virtual bool			Read();
	virtual bool			Disconnect(const SOCKET)		{ return true; }

	virtual	CTcpConnector*	SelectConnector(INT, SOCKET);

private:

};

class CUdpListener : public CUdpConnector {
public:
	CUdpListener() {}
	virtual ~CUdpListener() {}

	virtual void			Clear();

	virtual bool			KeepAlive()	{ return true; }

			static void*	New()		{ return new CUdpListener; }

	virtual INT				Listen(const char*, INT);
	virtual bool			Read();

	virtual	CUdpConnector*	SelectConnector(INT, SOCKET);
};

#endif // ___NETDRONE_NET_LISTENER_H___

/* EOF */
