/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_NET_LAUNCHER_H___
#define ___SERVICE_NET_LAUNCHER_H___

#include <netdrone/CommonHeader.h>

class CUnit;

typedef bool (*NativeLauncher)(CUnit&, CCommand&);
typedef bool (*ExtendLauncher)(CExtendCommand&);

extern NativeLauncher g_pbfNativeLauncher[];
extern ExtendLauncher g_pbfExtendLauncher[];

void ServerLauncher(CUnit&, CCommand&);
void ServerLauncher(CExtendCommand&);

void InitializeCommand();

void InitializeIdCommand();
void InitializeInfoCommand();
void InitializeRoomCommand();
void InitializeUserCommand();

#endif // ___SERVICE_NET_LAUNCHER_H___

/* EOF */
