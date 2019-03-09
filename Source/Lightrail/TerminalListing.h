/*!
@file	TerminalListing.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef TERMINALLISTING_H
#define TERMINALLISTING_H

#include "Xylasoft.h"
#include "Terminal.h"
#include "DiscoveryPeer.h"
#include "Method.h"
#include <XMutex.h>
#include <XSerializer.h>
#include <map>

namespace Xylasoft
{
	class TerminalListing
	{
	public:
		TerminalListing(const DiscoveryPeer& peer);

		TerminalListing(XSerializer& serializer);

		TerminalListing(const TerminalListing& other);

		~TerminalListing();

		void AddTerminal(Standpoint::StandpointType type, const Terminal& terminal);

		bool CheckForSecurityKeyUse(const wchar_t* name);

		void RemoveTerminal(Standpoint::StandpointType type, const wchar_t* name);;

		bool MatchTerminal(Standpoint::StandpointType type, const Terminal& terminal, 
			std::vector<Terminal>& matchTerminals) const;

		bool FindTerminal(Standpoint::StandpointType standpoint, const wchar_t* name, 
			Terminal& terminal) const;

		void UpdateOwner(const DiscoveryPeer& peer);

		const DiscoveryPeer& Owner();

		void Serialize(XSerializer& serializer) const;

	private:
		std::map<Standpoint::StandpointType, std::map<std::wstring, Terminal> > m_terminals;
		DiscoveryPeer* m_peer;
		mutable XMutex m_terminalLock;
	};
}

#endif