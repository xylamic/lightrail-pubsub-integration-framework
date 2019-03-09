/*!
@file	TerminalListing.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "TerminalListing.h"

#define FORMAT_V1 0

using namespace Xylasoft;

TerminalListing::TerminalListing(const DiscoveryPeer& peer)
{
	this->m_peer = new DiscoveryPeer(peer);
}

TerminalListing::TerminalListing(XSerializer& serializer)
{
	uint16_t version;
	serializer >> version;

	if (version != FORMAT_V1)
	{
		throw DataException(L"The terminal listing format version was unrecognized.");
	}

	std::vector<Standpoint::StandpointType> standpoints;
	standpoints.push_back(Standpoint::PUBLISHER);
	standpoints.push_back(Standpoint::REQUESTER);
	standpoints.push_back(Standpoint::RESPONDER);
	standpoints.push_back(Standpoint::SUBSCRIBER);

	// get the peer
	this->m_peer = new DiscoveryPeer(serializer);

	// get the terminals
	for (size_t i = 0; i < standpoints.size(); i++)
	{
		uint16_t size;
		serializer >> size;

		for (int j = 0; j < size; j++)
		{
			Terminal d(serializer);

			this->m_terminals[standpoints[i]][d.Name()] = d;
		}
	}
}

TerminalListing::TerminalListing(const TerminalListing& other)
{
	this->m_terminals = other.m_terminals;
	this->m_peer = new DiscoveryPeer(*other.m_peer);
}

TerminalListing::~TerminalListing()
{
	delete this->m_peer;
}

void TerminalListing::AddTerminal(Standpoint::StandpointType type, const Terminal& terminal)
{
	XMutexLocker locker(this->m_terminalLock);

	this->m_terminals[type][terminal.Name()] = terminal;
}

bool TerminalListing::CheckForSecurityKeyUse(const wchar_t* name)
{
	std::wstring keyname = name;

	XMutexLocker locker(this->m_terminalLock);

	for (auto standpointit = this->m_terminals.cbegin(); standpointit != this->m_terminals.cend(); ++standpointit)
	{
		for (auto terminalit = standpointit->second.cbegin(); terminalit != standpointit->second.cend(); ++terminalit)
		{
			if (terminalit->second.SecurityKeyName() != nullptr && keyname.compare(terminalit->second.SecurityKeyName()) == 0)
			{
				return true;
			}
		}
	}

	return false;
}

void TerminalListing::RemoveTerminal(Standpoint::StandpointType type, const wchar_t* name)
{
	XMutexLocker locker(this->m_terminalLock);

	auto it = this->m_terminals.find(type);
	if (it == this->m_terminals.end())
	{
		return;
	}

	this->m_terminals[type].erase(name);
}

bool TerminalListing::MatchTerminal(Standpoint::StandpointType type, const Terminal& terminal, std::vector<Terminal>& matchTerminals) const
{
	XMutexLocker locker(this->m_terminalLock);

	auto it = this->m_terminals.find(type);
	if (it == this->m_terminals.cend())
	{
		return false;
	}

	bool found = false;
	for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
	{
		if (terminal == it2->second)
		{
			matchTerminals.push_back(it2->second);
			found = true;
		}
	}

	return found;
}

bool TerminalListing::FindTerminal(Standpoint::StandpointType standpoint, const wchar_t* name, Terminal& terminal) const
{
	XMutexLocker locker(this->m_terminalLock);

	std::wstring wname(name);

	auto standpointit = this->m_terminals.find(standpoint);
	if (standpointit == this->m_terminals.cend())
	{
		return false;
	}

	auto terminalit = standpointit->second.find(wname);
	if (terminalit == standpointit->second.cend())
	{
		return false;
	}

	terminal = terminalit->second;
	return true;
}

void TerminalListing::UpdateOwner(const DiscoveryPeer& peer)
{
	delete this->m_peer;
	this->m_peer = new DiscoveryPeer(peer);
}

const DiscoveryPeer& TerminalListing::Owner()
{
	return *this->m_peer;
}

void TerminalListing::Serialize(XSerializer& serializer) const
{
	XMutexLocker locker(this->m_terminalLock);

	serializer << static_cast<uint16_t>(FORMAT_V1);

	std::vector<Standpoint::StandpointType> standpoints;
	standpoints.push_back(Standpoint::PUBLISHER);
	standpoints.push_back(Standpoint::REQUESTER);
	standpoints.push_back(Standpoint::RESPONDER);
	standpoints.push_back(Standpoint::SUBSCRIBER);

	// set the peer
	this->m_peer->Serialize(serializer);

	// set the terminals
	for (size_t i = 0; i < standpoints.size(); i++)
	{
		// serialize this standpoint list
		auto it = this->m_terminals.find(standpoints[i]);
		if (it != this->m_terminals.cend())
		{
			serializer << static_cast<uint16_t>(it->second.size());
			for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
			{
				it2->second.Serialize(serializer);
			}
		}
		else
		{
			serializer << static_cast<uint16_t>(0);
		}
	}
}
