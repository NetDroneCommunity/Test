/*
 * NetDrone Engine
 * Copyright © 2022 Origin Studio Inc.
 *
 */

#ifndef ___NETDRONE_COMMONHEADER_H___
#define ___NETDRONE_COMMONHEADER_H___

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstring>
#include <cctype>
#include <ctime>
#include <memory>
#include <typeinfo>
#include <csignal>
#include <iostream>
#include <fstream>

#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>		// inet_addr
#include <netinet/tcp.h>	// UDP_NODELAY
#include <net/if.h>
#include <errno.h>			// EWOULDBLOCK / EFAULT
#include <netdb.h>			// hostent / host / gethostbyname
#include <unistd.h>			// fdatasync
#include <fcntl.h>			// ioctl

#include "ReTypedefined.h"
#include "Defines.h"
#include "Enums.h"
#include "GlobalVariable.h"
#include "Version.h"

#include "util/Action.h"
#include "util/Allocator.h"
#include "util/Backtrace.h"
#include "util/Base64.h"
#include "util/Config.h"
#include "util/Crypt.h"
#include "util/Key.h"
#include "util/Locker.h"
#include "util/LogFile.h"
#include "util/Providing.h"
#include "util/Singleton.h"
#include "util/Threading.h"
#include "util/Tick.h"

#include "net/Protocol.h"
#include "net/Provider.h"
#include "net/Thread.h"
#include "net/Buffer.h"
#include "net/Command.h"
#include "net/Connector.h"
#include "net/Dispatcher.h"
#include "net/DispatcherList.h"
#include "net/Host.h"
#include "net/Listener.h"
#include "net/Network.h"
#include "net/Packet.h"
#include "net/Queue.h"
#include "net/Sensor.h"
#include "net/Socket.h"

#include "db/DBM.h"

#endif // ___NETDRONE_COMMONHEADER_H___

/* EOF */
