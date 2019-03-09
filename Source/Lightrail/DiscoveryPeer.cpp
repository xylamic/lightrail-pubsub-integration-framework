/*!
@file	DiscoveryPeer.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "DiscoveryPeer.h"
#include <XUtilities.h>

#define FORMAT_V1 0

using namespace Xylasoft;

DiscoveryPeer::DiscoveryPeer(const char* hostname, long port)
{
	this->m_hostname = hostname;
	this->m_port = port;
}

DiscoveryPeer::DiscoveryPeer(XSerializer& serializer)
{
	uint16_t version;
	serializer >> version;

	if (version != FORMAT_V1)
	{
		throw DataException(L"The discovery peer format version was unrecognized.");
	}

	int32_t port;

	serializer >> this->m_hostname;
	serializer >> port;

	this->m_port = port;
}

DiscoveryPeer::DiscoveryPeer(const char* data, ActionType& action)
{
	std::string msg = data;
			
	if (msg.size() < 2)
	{
		throw DataException(L"The message was too small.");
	}

	if (msg[0] != '[' || msg[msg.size() - 1] != ']')
	{
		throw DataException(L"The message was incomplete.");
	}

	msg = msg.substr(1, msg.size() - 2);
			
	std::vector<std::string> parts;
	size_t index = msg.find_first_of('$');
	while (index != msg.npos)
	{
		parts.push_back(msg.substr(0, index));
		msg = msg.substr(index + 1);

		index = msg.find_first_of('$');
	}

	if (parts.size() < 4)
	{
		throw DataException(L"The message format was unrecognized.");
	}

	if (parts[0].compare("0") != 0)
	{
		throw DataException(L"The message version was unrecognized.");
	}

	this->m_hostname = parts[1];
	this->m_port = ::atol(parts[2].c_str());
	action = static_cast<ActionType>(::atol(parts[3].c_str()));
}

DiscoveryPeer& DiscoveryPeer::operator=(const DiscoveryPeer& other)
{
	this->m_hostname = other.m_hostname;
	this->m_lastUpdate = other.m_lastUpdate;
	this->m_port = other.m_port;

	return *this;
}

bool DiscoveryPeer::operator<(const DiscoveryPeer& other) const
{
	int compare = XUtilities::CaseInsensitiveCompare(
		this->m_hostname.c_str(),
		other.m_hostname.c_str());

	// check for differences in name
	if (compare < 0)
	{
		return true;
	}
	else if (compare > 0)
	{
		return false;
	}

	// check for differences in port
	if (this->m_port < other.m_port)
	{
		return true;
	}
	else if (this->m_port > other.m_port)
	{
		return false;
	}

	return false;
}

bool DiscoveryPeer::operator>(const DiscoveryPeer& other) const
{
	return !(this->operator<(other));
}

bool DiscoveryPeer::operator==(const DiscoveryPeer& other) const
{
	int compare = XUtilities::CaseInsensitiveCompare(
		this->m_hostname.c_str(),
		other.m_hostname.c_str());

	// check for differences in name
	if (compare < 0 || compare > 0)
	{
		return false;
	}

	// check for differences in port
	if (this->m_port < other.m_port || this->m_port > other.m_port)
	{
		return false;
	}

	return true;
}

std::string DiscoveryPeer::Hostname() const
{
	return this->m_hostname;
}

std::string DiscoveryPeer::HostnamePort() const
{
	std::ostringstream stream;

	stream << this->m_hostname << ":" << this->m_port;

	return stream.str();
}

long DiscoveryPeer::Port() const
{
	return this->m_port;
}

time_t DiscoveryPeer::LastUpdate() const
{
	return this->m_lastUpdate;
}

void DiscoveryPeer::SetLastUpdate(time_t time)
{
	this->m_lastUpdate = time;
}

std::string DiscoveryPeer::Serialize(ActionType action)
{
	std::ostringstream stream;

	stream << "[" << (long)0 << "$" << this->m_hostname.c_str() << "$" << this->m_port << "$" << (int)action << "$]";

	return stream.str();
}

void DiscoveryPeer::Serialize(XSerializer& serializer) const
{
	serializer << static_cast<uint16_t>(FORMAT_V1);

	serializer << this->m_hostname;
	serializer << static_cast<int32_t>(this->m_port);
}