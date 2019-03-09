/*!
@file	PeerActor.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef PEERACTOR_H
#define PEERACTOR_H

#include "Lightrail_global.h"
#include "DiscoveryPeer.h"
#include "DetailedDelivery.h"
#include "LightrailLogged.h"
#include "TerminalListing.h"
#include "Package.h"
#include "GlobalSettings.h"
#include <XQueue.h>
#include <XThread.h>
#include <XByteArray.h>
#include <XTcpMessageSender.h>
#include <XSerializer.h>
#include <XEvent.h>
#include <vector>

namespace Xylasoft
{
	class PeerActor : protected XThread, protected LightrailLogged
	{
	public:
		PeerActor(const DiscoveryPeer& peer);

		virtual ~PeerActor();

		void StartProcessing();

		void StopProcessing();

		void EnqueuePackage(const Package* package);

		bool MatchTerminal(Standpoint::StandpointType standpoint, const Terminal& terminal, 
			std::vector<Terminal>& matchTerminals);

		void SetTerminals(const TerminalListing* terminals);

		TerminalListing Terminals();

		bool IsActive();

	protected:
		virtual void Execution();

	private:
		DiscoveryPeer m_peer;
		const TerminalListing* m_terminals;
		XEvent m_queueEvent;
		XMutex m_terminalLock;
		volatile bool m_isActive;

		XQueue<const Package*> m_terminalQueue;
		XQueue<const Package*> m_reqrespQueue;
		XQueue<const Package*> m_subpubQueue;
	};
}

#endif