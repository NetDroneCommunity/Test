/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_CONNECTOREX_H___
#define ___SERVICE_NET_CONNECTOREX_H___

#include <netdrone/CommonHeader.h>

class CDefaultConnector : public CTcpConnector {
public:
	CDefaultConnector() {}
	virtual ~CDefaultConnector() {}

	virtual void			SetConnected();

	virtual bool			Disconnect(const SOCKET);
	virtual	void			Disconnect();

	virtual bool			KeepAlive();

			static void*	New()	{ return new CDefaultConnector; }

private:

};

class CBackboneConnector : public CTcpConnector {
public:
	CBackboneConnector() {}
	virtual ~CBackboneConnector() {}

	virtual void			SetConnected();

	virtual bool			Disconnect(const SOCKET);
	virtual	void			Disconnect();

	virtual bool			KeepAlive();

			static void*	New()	{ return new CBackboneConnector; }

private:

};

class CReliableConnector : public CUdpConnector {
public:
	CReliableConnector() {}
	virtual ~CReliableConnector() {}

	virtual void			SetConnected();

	virtual bool			Disconnect(const SOCKET);
	virtual	void			Disconnect();

			static void*	New()	{ return new CReliableConnector; }

private:

};

#endif // ___SERVICE_NET_CONNECTOREX_H___

/* EOF */
