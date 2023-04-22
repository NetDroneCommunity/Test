/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "CfgMgr.h"

#include "../CommonHeader.h"

template<> CCfgMgr* CSingleton< CCfgMgr >::ms_Instance = NULL;

CCfgMgr::CCfgMgr() {
}

CCfgMgr::~CCfgMgr() {
	Release();
}

bool
CCfgMgr::Initialize(const char* szConfigPath_, const char* szPackageName_, INT iServerId_) {
	bool bCheck = false;

	if(0 == iServerId_) {
		// config 파일로 로딩하지 않을경우를 위해 서버 기본값을 설정합니다.
		char szServerAddr[15] = { 0, };
		strncpy(szServerAddr, "0.0.0.0", 15);

		SetTcpDefaultAddress(szServerAddr, 11000);
		SetTcpBackboneAddress(szServerAddr, 11001);
		SetUdpReliableAddress(szServerAddr, 11000);

		char szLogPath[iFILE_PATH_LEN+1] = { 0, };
		char szInfoPath[iFILE_PATH_LEN+1] = { 0, };
		strncpy(szLogPath, "/tmp", 4);
		strncpy(szInfoPath, "/tmp", 4);

		SetLogPath(*szLogPath);
		SetInfoPath(*szLogPath);

		// 최대 연결가능 수는 /etc/security/limits.conf 에서 아래와 같이 조정이 가능합니다.
		//
		// *                soft    nofile          65535
		// *                hard    nofile          65535
		// root             soft    nofile          65535
		// root             hard    nofile          65535
		//
		// 이렇게 수정한뒤 원하는 최대 연결수를 unsigned short 범위안에서 입력하세요.
		SetMaxConnection(1000);

		SetHeaderCrypt(true);

		if(CConfig::Initialize(szPackageName_, iServerId_)) {
			bCheck = true;
		}
	} else {
		if(CConfig::Initialize(szConfigPath_, szPackageName_, iServerId_)) {
			bCheck = true;
		}

		TRACE("server id: " << GetServerId());
		TRACE("tcp default address: " << GetTcpDefaultAddress());
		TRACE("tcp default port: " << GetTcpDefaultPort());
		TRACE("udp reliable address: " << GetUdpReliableAddress());
		TRACE("udp reliable port: " << GetUdpReliablePort());
		TRACE("tcp backbone address: " << GetTcpBackboneAddress());
		TRACE("tcp backbone port: " << GetTcpBackbonePort());
		TRACE("max connection: " << GetMaxConnection());
		TRACE("header crypt: " << IsHeaderCrypt());
		TRACE("log path: " << GetLogPath());
		TRACE("info path: " << GetInfoPath());
	}

	return bCheck;
}

bool
CCfgMgr::Release() {
	CConfig::Release();
	return true;
}

bool
CCfgMgr::Load(const char* szPath_) {
	bool bLoadingCheck = true;

	char szFilePath[iFILE_PATH_LEN] = { 0, };
	//sprintf(szFilePath, szPath_);
	memcpy(szFilePath, szPath_, iFILE_PATH_LEN);
	FILE* fp = fopen(szFilePath, "r");

	if(NULL != fp) {
		char szBuffer[iMAX_CONFIG_BUFFER_LEN] = { 0, };
		char szKey[iMAX_CONFIG_KEY_LEN] = { 0, };
		char szValue[iMAX_CONFIG_VALUE_LEN] = { 0, };

		char* pszToken = NULL;
		bool bCheck = false;

		while(fgets(szBuffer, iMAX_CONFIG_BUFFER_LEN-1, fp)) {
			//TRACE("size: " << strlen(szBuffer) << " : " << szBuffer);
			if((szBuffer[0] != '#') && (szBuffer[0] != ' ') && (szBuffer[0] != '\0') && (szBuffer[0] != '\n') && (szBuffer[0] != '\r') && (szBuffer[0] != '\t')) {
				if(!bCheck) {
					if('[' == szBuffer[0]) {
						memset(szKey, 0, sizeof(szKey));
						memset(szValue, 0, sizeof(szValue));
						pszToken = strtok(szBuffer, ":");
						//TRACE("size: " << strlen(szBuffer) << " : " << szBuffer);
						if(isptr(pszToken)) {
							int iCount = 0;
							for(size_t i=1; i<strlen(pszToken); ++i) {
								if(' ' != pszToken[i]) {
									szKey[iCount] = pszToken[i];
									++iCount;
								}
							}
							//TRACE("key : " << szKey << " : " << m_szPackageName);
							if(0 == strcmp(szKey, m_szPackageName)) {
								pszToken = strtok(NULL, "]");
								if(isptr(pszToken)) {
									//TRACE("key : "<<szKey<<" : "<<pszToken);
									if(GetServerId() == atoi(pszToken)) {
										bCheck = true;
									}
								}
							}
						}
					}
				} else {
					if('[' == szBuffer[0]) {
						return true;
					}
					bool bRowCheck = false;
					memset(szKey, 0, sizeof(szKey));
					memset(szValue, 0, sizeof(szValue));
					pszToken = strtok(szBuffer, "=\t");
					if(isptr(pszToken)) {
						INT iCount = 0;
						for(size_t i=0; i<strlen(pszToken); ++i) {
							if((' ' != pszToken[i]) && ('=' != pszToken[i]) && ('\t' != pszToken[i]) && ('\"' != pszToken[i]) && ('\n' != pszToken[i]) && ('\r' != pszToken[i])) {
								szKey[iCount] = pszToken[i];
								++iCount;
							}
						}
						if(0 < iCount) {
							bRowCheck = true;
						}
					}
					if(bRowCheck) {
						bRowCheck = false;
						pszToken = strtok(NULL, "\n");
						if(isptr(pszToken)) {
							INT iCount = 0;
							for(size_t i=0; i<strlen(pszToken); ++i) {
								if((' ' != pszToken[i]) && ('=' != pszToken[i]) && ('\t' != pszToken[i]) && ('\"' != pszToken[i]) && ('\n' != pszToken[i]) && ('\r' != pszToken[i])) {
									szValue[iCount] = pszToken[i];
									++iCount;
								}
							}
							if(0 < iCount) {
								bRowCheck = true;
							}
						}
					}

					//TRACE("key: " << szKey << ", value: " << szValue);

					if(bRowCheck) {
						if(0 == strcmp(szKey, KEY_DEFAULT_INTERFACE)) {
							if(!HostAddress(m_saTcpDefaultAddr_in.sin_addr, szValue)) {
								OUTPUT("[" << szKey << "] error, this is not a ethernet interface name or somthing unexpected occurred: [" << szValue << "]");
								bLoadingCheck = false;
							}
						} else if(0 == strcmp(szKey, KEY_DEFAULT_PORT)) {
							m_saTcpDefaultAddr_in.sin_port = htons(atoi(szValue));
							if(0 == m_saTcpDefaultAddr_in.sin_port) {
								OUTPUT("[" << szKey << "] error, port number is wrong or something unexpected occurred: [ " << szValue << "]");
								bLoadingCheck = false;
							}
						} else if(0 == strcmp(szKey, KEY_BACKBONE_INTERFACE)) {
							if(!HostAddress(m_saTcpBackboneAddr_in.sin_addr, szValue)) {
								OUTPUT("[" << szKey << "] error, this is not a ethernet interface name or somthing unexpected occurred: [" << szValue << "]");
								bLoadingCheck = false;
							}
						} else if(0 == strcmp(szKey, KEY_BACKBONE_PORT)) {
							m_saTcpBackboneAddr_in.sin_port = htons(atoi(szValue));
							if(0 == m_saTcpBackboneAddr_in.sin_port) {
								OUTPUT("[" << szKey << "] error, port number is wrong or something unexpected occurred: [ " << szValue << "]");
								bLoadingCheck = false;
							}
						} else if(0 == strcmp(szKey, KEY_RELIABLE_INTERFACE)) {
							if(!HostAddress(m_saUdpReliableAddr_in.sin_addr, szValue)) {
								OUTPUT("[" << szKey << "] error, this is not a ethernet interface name or somthing unexpected occurred: [" << szValue << "]");
								bLoadingCheck = false;
							}
						} else if(0 == strcmp(szKey, KEY_RELIABLE_PORT)) {
							m_saUdpReliableAddr_in.sin_port = htons(atoi(szValue));
							if(0 == m_saUdpReliableAddr_in.sin_port) {
								OUTPUT("[" << szKey << "] error, port number is wrong or something unexpected occurred: [ " << szValue << "]");
								bLoadingCheck = false;
							}
						} else if(0 == strcmp(szKey, KEY_MAX_CONNECTION)) {
							SetMaxConnection(atoi(szValue));
						} else if(0 == strcmp(szKey, KEY_HEADER_CRYPT)) {
							if((0 == strcmp(szValue, "true")) || (0 == strcmp(szValue, "True")) || (0 == strcmp(szValue, "TRUE"))) {
								SetHeaderCrypt(true);
							} else {
								SetHeaderCrypt(false);
							}
						} else if(0 == strcmp(szKey, KEY_LOG_PATH)) {
							memcpy(m_szLogPath, szValue, iMAX_CONFIG_VALUE_LEN);
						} else if(0 == strcmp(szKey, KEY_INFO_PATH)) {
							memcpy(m_szInfoPath, szValue, iMAX_CONFIG_VALUE_LEN);
						} else {
							OUTPUT("[" << szKey << "] error, option is not valid");
							bLoadingCheck = false;
						}
					}
				}
			}
		}
		fclose(fp);
	} else {
		OUTPUT("[" << szFilePath << "] was not found");
		return false;
	}

	return bLoadingCheck;
}

/* EOF */
