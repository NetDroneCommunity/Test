/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___SERVICE_UNIT_USERDATA_H___
#define ___SERVICE_UNIT_USERDATA_H___

#include <netdrone/CommonHeader.h>

#include "../GlobalVariable.h"
#include "../Enums.h"

#pragma pack(4)

struct SUnitSelfData {
	SUnitSelfData&		GetUnitSelfData()			{ return *this; }

	const UINT			GetKey()					{ return key; }
	void				SetKey(UINT o)				{ key = o; }

	SUnitSelfData&		operator =(const SUnitSelfData& rval_) {
							key = rval_.key;
							return *this;
						}

	SUnitSelfData&		operator +=(const SUnitSelfData& rval_) {
							return *this;
						}

	UINT32	key;
};

struct SUnitOtherData {
	SUnitOtherData&		GetUnitOtherData()			{ return *this; }

	const STATUS_TYPE	GetStatus()					{ return STATUS_TYPE(status); }
	void				SetStatus(STATUS_TYPE o)	{ status = o; }

	const char*			GetName()					{ return name; }
	void				SetName(const char& o)		{ memcpy(name, &o, iUNIT_NAME_LEN); name[iUNIT_NAME_LEN] = '\0'; }

	SUnitOtherData&		operator =(const SUnitOtherData& rval_) {
							status = rval_.status;

							SetName(*rval_.name);

							return *this;
						}

	SUnitOtherData&		operator +=(const SUnitOtherData& rval_) {
							return *this;
						}

	UINT32	status:4;
	UINT32	padding3:4;
	UINT32	padding2:4;
	UINT32	padding1:20;

	char	name[iUNIT_NAME_LEN+1];
};

struct SPlayerData : SUnitSelfData, SUnitOtherData {
	SPlayerData() { memset(this, 0, sizeof(SPlayerData)); }

	SPlayerData&		GetPlayerData()					{ return *this; }

	const UINT			GetAid()						{ return aid; }
	void				SetAid(UINT o)					{ aid = o; }

	void				Reset(SPlayerData& o)			{ (*this) = o; }

	void				ClearUnitSelfData()				{ key = 0; }
	void				ClearUnitOtherData()			{ memset(name, 0, sizeof(name)); status = 0; }

	void
	ClearPlayerData() {

	}

	void			Clear()							{ memset(this, 0, sizeof(SPlayerData)); }

	SPlayerData&	operator =(const SPlayerData& rval_) {
						SUnitSelfData::operator = (rval_);
						SUnitOtherData::operator = (rval_);

						aid = rval_.aid;

						return *this;
					}

	SPlayerData&	operator +=(const SPlayerData& rval_) {
						SUnitSelfData::operator += (rval_);
						SUnitOtherData::operator += (rval_);

						return *this;
					}

	UINT32	aid;

};

#pragma pack()

#endif // ___SERVICE_UNIT_USERDATA_H___

/* EOF */
