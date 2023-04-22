/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_NETWORKEX_H___
#define ___SERVICE_NET_NETWORKEX_H___

#include <netdrone/CommonHeader.h>

class CMasterNetwork : public CNetwork {
public:
	CMasterNetwork() {}
	virtual ~CMasterNetwork() {}

	virtual bool			ParseCommand(CExtendCommand&);

private:

};

class CSlaveNetwork : public CNetwork {
public:
	CSlaveNetwork() {}
	virtual ~CSlaveNetwork() {}

	virtual bool			ParseCommand(CExtendCommand&);

private:

};

#endif // ___SERVICE_NET_NETWORKEX_H___

/* EOF */
