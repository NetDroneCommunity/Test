/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#include "CommonHeader.h"

#ifdef _BUILD_H
	#include "build.h"
#endif

const INT iDUMMY_OPTION_LEN = 16;
INT g_iForkPid = -1;

void
CallbackSigint(INT iSignal_) {
	//TRACE("CallbackSigint: ");

	g_kFramework.Release();

	if(g_iForkPid == 0) {
		alarm(60);
		TRACE("[" << getpid() << "] SIGINT: We'll wait for only 60 seconds." << iSignal_);
	} else {
		TRACE("[" << getpid() << "] SIGINT: exit." << iSignal_);
		exit(EXIT_FAILURE);
	}
}

void
CallbackSigterm(INT iSignal_) {
	//LOG_GAME(LE_SERVER_ERR<<LF_STR<<"shutdown: terminate signal: pid["<<g_iPid<<"] : sid["<<g_iServerId<<"]");
	TRACE("["<<getpid()<<"] SIGTERM: "<<iSignal_);
	CallbackSigint(iSignal_);
}

void
CallbackSigalrm(INT iSignal_) {
	TRACE("[" << getpid() << "] SIGALRM: ");
	exit(EXIT_FAILURE);
}

void
CallbackSigsegv(INT iSignal_) {
	//LOG_GAME(LE_SERVER_ERR<<LF_STR<<"shutdown: segment fault signal: pid["<<g_iPid<<"] : sid["<<g_iServerId<<"]");
	TRACE("["<<getpid()<<"] SIGSEGV: "<<iSignal_);
	Backtrace();
	exit(EXIT_FAILURE);
}

void
Version() {
	PRINT(GetNetDroneVersion());
	PRINT(GetServiceVersion());

#ifdef _THREAD
	PRINT("posix thread");
#else
	PRINT("non-thread");
#endif

	exit(EXIT_SUCCESS);
}

char* g_pcOptions[iMAX_OPTIONS];

static struct option const g_stLongOptions[] = {
	{"version",		no_argument,		NULL,	'v'},
	{"serverid",	required_argument,	NULL,	's'},
	{"config",		required_argument,	NULL,	'c'},
	{"daemon",		required_argument,	NULL,	'd'},
	{NULL,			0,					NULL,	0}
};

INT
main(INT iArgc_, char* pcArgv_[]) {
	INT iServerId = 0;
	bool bDaemon = false;
	char szConfigPath[iFILE_NAME_LEN] = {0, };
	char szMessagePath[iFILE_NAME_LEN] = {0, };
	char dummy_argument[iDUMMY_OPTION_LEN] = {0, };

	sprintf(dummy_argument, "%s", "no-argument");

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, CallbackSigint);
	signal(SIGTERM, CallbackSigterm);
	signal(SIGALRM, CallbackSigalrm);
	signal(SIGSEGV, CallbackSigsegv);

	INT i;
	while((i = getopt_long(iArgc_, pcArgv_, "vs:c:d", g_stLongOptions, NULL)) != -1) {
		g_pcOptions[i] = dummy_argument;

		switch(i) {
		case 'v':
			{
				Version();
			}
			break;

		default:
			{
				if(isptr(optarg)) {
					g_pcOptions[i] = optarg;
				}
			}
			break;
		}
	}

	for(INT i=0; i < iMAX_OPTIONS; ++i) {
		optarg = g_pcOptions[i];
		if(isptr(optarg)) {
			switch(i) {
			case 's':
				{
					iServerId = atoi(optarg);
				}
				break;

			case 'c':
				{
					memset(szConfigPath, 0, sizeof(szConfigPath));
					sprintf(szConfigPath, "%s", optarg);
				}
				break;

			case 'd':
				{
					sprintf(szMessagePath, "%s", optarg);
					bDaemon = true;

					if(0 < strlen(szMessagePath)) {
						// file exist 검사나 경로 검사가 필요한지 테스트 해야함.
						// 표준출력 위치 변경.
						// 테스트 필요.
//						FILE* fp = freopen(szMessagePath,"w+",stdout);
//						if(isptr(fp)) {
//
//						}
					}
				}
				break;

			default:
				break;
			}
		}
	}

	//TRACE("process started: " << getpid());

	if(false == bDaemon) {
		// remote script, local에서 개발 할 때 사용함.
		Action("process mode: frontend", true);

		//TRACE("option s: " << iServerId);
		//TRACE("option c: " << szConfigPath);
		//TRACE("option d: " << szMessagePath);

		if(g_kFramework.Initialize(szConfigPath, PACKAGE_NAME, iServerId)) {
			while(g_kFramework.IsDoing()) {
				g_kFramework.Update();
				usleep(0);
			}
		}

		g_kFramework.Release();
	} else {
		// init script, console에서 직접 실행 할 때 사용함.
		// remote script는 control script에서 파일 백업이 선행 되는지 확인 필요.
		g_iForkPid = fork();
		if(0 == g_iForkPid) {
			// child process
			Action("process mode: daemon", true);

			if(g_kFramework.Initialize(szConfigPath, PACKAGE_NAME, iServerId)) {
				while(g_kFramework.IsDoing()) {
					g_kFramework.Update();
					usleep(0);
				}
			}

			g_kFramework.Release();

			TRACE("child: exit: " << getpid());
			return EXIT_SUCCESS;
		} else {
			// parent process
			TRACE("parent: exit: " << getpid());
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}

/* EOF */
