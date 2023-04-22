/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_NETIO_H___
#define ___SERVICE_NET_NETIO_H___

#include <netdrone/CommonHeader.h>

#include "../GlobalVariable.h"
#include "ConnectorEx.h"

class CNetIO {
public:
	CNetIO() : m_pkConnector(NULL), m_tkDelayPingTick(0) {}
	virtual	~CNetIO() {}

	virtual void
			Clear() {
				if(isptr(m_pkConnector)) {
					m_pkConnector->Disconnect();
					m_pkConnector = NULL;
				}
				m_tkDelayPingTick = 0;
			}

	virtual INT
			Send(CCommand& rkCommand_, INT iSize_, PACKET_TYPE eType_ =PACKET_THROW, CRYPT_TYPE eCrypt_ =CRYPT_NONE) {
				if(isptr(m_pkConnector)) {
					return m_pkConnector->Send(rkCommand_, iSize_, eType_, eCrypt_);
				}
				return -1;
			}

			void		SetConnector(CConnector* o)	{ m_pkConnector = o; }
			CConnector*	GetConnector()				{ return m_pkConnector; }

			void		SetDelayPingTick(tick_t o)	{ m_tkDelayPingTick = o; }
			UINT		GetDelayPingTick()			{ return m_tkDelayPingTick; }

private:
			CConnector*	m_pkConnector;
			tick_t		m_tkDelayPingTick;

};

#endif // ___SERVICE_NET_NETIO_H___

/* EOF */
