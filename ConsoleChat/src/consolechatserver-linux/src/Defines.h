/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_DEFINES_H___
#define ___SERVICE_DEFINES_H___

#define KEY_DEFAULT_INTERFACE	"DefaultInterface"
#define KEY_DEFAULT_PORT		"DefaultPort"
#define KEY_BACKBONE_INTERFACE	"BackboneInterface"
#define KEY_BACKBONE_PORT		"BackbonePort"
#define KEY_RELIABLE_INTERFACE	"ReliableInterface"
#define KEY_RELIABLE_PORT		"ReliablePort"
#define KEY_MAX_CONNECTION		"MaxConnection"
#define KEY_HEADER_CRYPT		"HeaderCrypt"
#define KEY_LOG_PATH			"LogPath"
#define KEY_INFO_PATH			"InfoPath"

template <typename T>
inline const T Max(T x, T y) {
	return (x > y) ? x : y;
}

template <typename T>
inline const T Min(T x, T y) {
	return (x < y) ? x : y;
}

#endif // ___SERVICE_DEFINES_H___

/* EOF */
