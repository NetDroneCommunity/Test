/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "ListenerEx.h"

#include "../CommonHeader.h"

CTcpConnector*
CBackboneListener::SelectConnector(INT iIndex_, SOCKET skSocket_) {
	OUTPUT("[" << g_kTick.GetTime() << "] backbone: [" << iIndex_ << "] " << skSocket_);

	//return (CTcpConnector*)m_pkSensor->Empty();
	return CTcpListener::SelectConnector(iIndex_, skSocket_);
}

/* EOF */
