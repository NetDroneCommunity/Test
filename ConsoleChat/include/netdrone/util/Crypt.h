/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_UTIL_CRYPT_H___
#define ___NETDRONE_UTIL_CRYPT_H___

#include "../GlobalVariable.h"
#include "../Defines.h"
#include "../Enums.h"

class CCrypt {
public:
	// RSA - Magic constants for key setup
	#define _RSA_P16 0x5163
	#define _RSA_Q16 0x79B9
	#define _RSA_P32 0xB7E15163			// e -2
	#define _RSA_Q32 0x9E3779B9			// Golden Ratio -1

	// Rotats
	#define _RSA_ROTR16(x,n)	((x) >> (n) | (x) << (16 - (n)))
	#define _RSA_ROTL16(x,n)	((x) << (n) | (x) >> (16 - (n)))
	#define _RSA_ROTR32(x,n)	((x) >> (n) | (x) << (32 - (n)))
	#define _RSA_ROTL32(x,n)	((x) << (n) | (x) >> (32 - (n)))

	// RC5-32/12/16
	#define _RC5_ROUND		12						// number of rounds.
	#define _RC5_KEYTABLES	2*(_RC5_ROUND+1)		// size of table S = 2*(ROUND+1) words.

	// RC6-128/40/16,24,32
	// derived constants
	#define _RC6_ROUND	20 * 2				// number of rounds.
	#define _RC6_R22	(2 * _RC6_ROUND + 2)
	#define _RC6_R23	(2 * _RC6_ROUND + 3)
	#define _RC6_R24	(2 * _RC6_ROUND + 4)
	#define _RC6_KEYTABLES	_RC6_R24	// size of table S = 2*(ROUND+4) words.
	#define _RC6_LGW	5					// log2(w) -- wussed out.

//		enum CRYPT_TYPE {
//			RC5 = 0,			// 64bit crypt
//			RC6,				// 128bit crypt
//			CRYPT_TYPE_MAX
//		};

	CCrypt(CRYPT_TYPE =CRYPT_RC5, INT =0);
	virtual ~CCrypt();

	void	ChangeType(CRYPT_TYPE =CRYPT_RC5, INT =0);
	void	Setup(INT =0);
	bool	Encrypt(const void*, void*, INT);		// 암호화 알고리즘 수행.
	bool	Decrypt(const void*, void*, INT);		// 복호화 알고리즘 수행.

private:
	typedef void (CCrypt::*PVF_SETUP) ();
	typedef bool (CCrypt::*PBF_ENCRYPT) (const void*, void*, INT);
	typedef bool (CCrypt::*PBF_DECRYPT) (const void*, void*, INT);

	PVF_SETUP		m_pvfSetup;
	PBF_ENCRYPT		m_pbfEncrypt;
	PBF_DECRYPT		m_pbfDecrypt;

	void	RC5_Setup();
	void	RC5_KeySetup(const BYTE*);
	bool	RC5_Encrypt(const void*, void*, INT);	// 암호화 알고리즘 수행.
	bool	RC5_Decrypt(const void*, void*, INT);	// 암호화 알고리즘 수행.

	bool	RC5_Encrypt64(const void*, void*, INT);	// 암호화 알고리즘 수행.
	bool	RC5_Encrypt32(const void*, void*, INT);	// 암호화 알고리즘 수행.
	bool	RC5_Decrypt64(const void*, void*, INT);	// 복호화 알고리즘 수행.
	bool	RC5_Decrypt32(const void*, void*, INT);	// 복호화 알고리즘 수행.

	void	RC6_Setup();
	void	RC6_KeySetup(const BYTE*, const UINT16);
	bool	RC6_Encrypt(const void*, void*, INT);	// 암호화 알고리즘 수행.
	bool	RC6_Decrypt(const void*, void*, INT);	// 복호화 알고리즘 수행.

	UINT16	m_usKey_RC5[_RC5_KEYTABLES];
	UINT32	m_uiKey_RC5[_RC5_KEYTABLES];
	UINT32	m_uiKey_RC6[_RC6_KEYTABLES];
public:
	INT32	m_iSerialKey;
};

#endif // ___NETDRONE_UTIL_CRYPT_H___
