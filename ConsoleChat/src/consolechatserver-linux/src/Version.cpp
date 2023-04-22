/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "Version.h"

#ifdef _BUILD_H
	#include "build.h"
#endif

const char*
GetServiceVersion() {
	return PACKAGE_STRING "(" BUILD_TIME ")";
}

/* EOF */
