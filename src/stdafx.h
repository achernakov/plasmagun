#pragma once

#include <string>
#include <stdexcept>
#include <new>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>

#include <cstring>
#include <cstdlib>

#ifdef _WIN32

//#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#endif

#include <unistd.h>

#include <gtk/gtk.h>

#include "error.h"


