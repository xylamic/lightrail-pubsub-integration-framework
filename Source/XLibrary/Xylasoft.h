/*!
@file	Xylasoft.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XYLASOFT_H
#define XYLASOFT_H

#include <cstddef>
#include <exception>
#include <string>
#include <cassert>
#include <vector>
#include <ctime>
#include <memory>
#include <codecvt>

#if _DEBUG
#	define DEBUG_MODE
#	include <iostream>
#endif

// windows only section
#if _WIN32
#	pragma warning( disable : 4996 )
#	ifndef WINVER
#		define WINVER 0x0500
#	endif
#	ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0500
#	endif
#	include <WinSock2.h>
#	include <WS2tcpip.h>
#	include <Windows.h>
#	define _CRTDBG_MAP_ALLOC
#	include <cstdlib>
#	include <crtdbg.h>
#	ifdef _DEBUG
#		define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DBG_NEW
#	endif
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif	
#endif

#if linux
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h>
#endif

// verify any communication settings are done
void VerifyPlatformCommunications();

#endif // XYLASOFT_H
