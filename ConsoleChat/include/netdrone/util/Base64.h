/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_BASE64_H___
#define ___NETDRONE_UTIL_BASE64_H___

#include <string>

#include "../Defines.h"

std::string Base64Encode(const BYTE* , UINT);
std::string Base64Decode(const std::string&);

#endif // ___NETDRONE_UTIL_BASE64_H___

/* EOF */
