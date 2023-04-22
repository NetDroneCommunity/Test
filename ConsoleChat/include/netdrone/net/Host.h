/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_NET_HOST_H___
#define ___NETDRONE_NET_HOST_H___

#include "../Defines.h"

bool	HostAddress(IN_ADDR&);
bool	HostAddress(char*);
bool	HostAddress(char*, char*);
bool	HostAddress(IN_ADDR&, char*);

#endif // ___NETDRONE_NET_HOST_H___

/* EOF */
