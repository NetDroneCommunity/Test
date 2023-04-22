/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_GLOBALVARIABLE_H___
#define ___SERVICE_GLOBALVARIABLE_H___

#include <netdrone/CommonHeader.h>

const INT iLOGIN_ID_LEN = 32;
const INT iUNIT_NAME_LEN = 32;

const INT iMAX_CHAT_LEN = 255;

const INT iMAX_ROOM_MEMBERS = 16;
const INT iROOM_NAME_LEN = 32;

const INT iMAX_SERVER_LIST = 20;
const INT iMAX_CHANNEL_LIST = 10;
const INT iMAX_CHANNEL_USERS = 500;
const INT iMAX_ROOMS = 100;

const INT iCHANNEL_NAME_LEN = 255;
const INT iSERVER_NAME_LEN = 255;
const INT iSERVER_OPTION_LEN = 255;
const INT iSERVER_DOMAIN_LEN = 255;

const INT iMAX_UNIT_CAPACITY = 60000;
const INT iMAX_TRUNK_CAPACITY = 10;

#ifdef _DEBUG
	const INT iRECV_PING_TICK = 60000;	///< 10 min
#else
	const INT iRECV_PING_TICK = 9000;
#endif

const INT iMAX_OPTIONS = 256;

#endif // ___SERVICE_GLOBALVARIABLE_H___

/* EOF */
