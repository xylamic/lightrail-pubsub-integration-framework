/*!
@file	XHostLookup.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XHostLookup.h"

using namespace Xylasoft;

void VerifyPlatformCommunications()
{
	static bool isStarted = false;

	if (!isStarted)
	{
#if _WIN32
		WSAData wsaData;
	
		if (::WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		{
			throw NetworkException(L"The windows communication layer could not be started.");
		}

		isStarted = true;
#endif
	}
}

XHostLookup::XHostLookup()
{
	this->BuildAddressList("localhost", 0);
}


XHostLookup::XHostLookup(XHostAddress::AddressType type)
{
	this->BuildAddressList("localhost", type);
}

XHostLookup::XHostLookup(const char* host)
{
	this->BuildAddressList(host, 0);
}

XHostLookup::XHostLookup(const char* host, XHostAddress::AddressType type)
{
	this->BuildAddressList(host, type);
}

void XHostLookup::BuildAddressList(const char* host, int type)
{
	::VerifyPlatformCommunications();

	struct addrinfo hints;
	struct addrinfo* servinfo = nullptr;
	struct addrinfo* servinfoit = nullptr;

	//char ipAddrStr[INET6_ADDRSTRLEN];

	try
	{
		::memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		if (type == XHostAddress::IPv4)
		{
			hints.ai_family = AF_INET;
		}
		else if (type == XHostAddress::IPv6)
		{
			hints.ai_family = AF_INET6;
		}

		if (::getaddrinfo(host, nullptr, &hints, &servinfo) != 0)
		{
			throw ResolutionException(L"The host information could not be resolved.");
		}

		for (servinfoit = servinfo; servinfoit != nullptr; servinfoit = servinfoit->ai_next)
		{
			void* addr;

			if (servinfoit->ai_family == AF_INET)
			{
				struct sockaddr_in* ipv4 = (struct sockaddr_in*)servinfoit->ai_addr;
				addr = &(ipv4->sin_addr);

				const char* address = ::inet_ntoa(*static_cast<in_addr*>(addr));
				this->m_addresses.push_back(XHostAddress(address, XHostAddress::IPv4));
				//inet_ntop(servinfoit->ai_family, addr, ipAddrStr, sizeof ipAddrStr);
				//this->m_addresses.push_back(XHostAddress(ipAddrStr, XHostAddress::IPv4));
			}
			else
			{
				/*
				struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)servinfoit->ai_addr;
				addr = &(ipv6->sin6_addr);

				inet_ntop(servinfoit->ai_family, addr, ipAddrStr, sizeof ipAddrStr);
				this->m_addresses.push_back(XHostAddress(ipAddrStr, XHostAddress::IPv6));
				*/
			}
		}

		::freeaddrinfo(servinfo);
	}
	catch (...)
	{
		::freeaddrinfo(servinfo);
		throw;
	}
}

std::list<XHostAddress> XHostLookup::BindAddresses()
{
	return this->m_addresses;
}

std::string XHostLookup::LocalHostName()
{
	::VerifyPlatformCommunications();

	char name[1024];
	
	if (::gethostname(name, 1024) != 0)
	{
		throw ResolutionException(L"The local host name could not be retrieved.");
	}

	return std::string(name);
}