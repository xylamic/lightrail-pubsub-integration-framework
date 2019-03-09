/*!
@file	DiscoveryServices.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef DISCOVERYSERVICES_H
#define DISCOVERYSERVICES_H

#include "Lightrail_global.h"
#include "LightrailLogged.h"
#include "DiscoveryPeer.h"

#include <XByteArray.h>
#include <XMulticaster.h>
#include <XThread.h>
#include <XSingleEventCallback.h>
#include <XMutex.h>

#include <vector>

namespace Xylasoft
{
	class LIGHTRAILSHARED_EXPORTTEST DiscoveryServices : protected XThread, protected LightrailLogged
	{
	public:
		DiscoveryServices();

		virtual ~DiscoveryServices();

		void StartServices(int port = -1);

		void StopServices();

		XSingleEventCallback<DiscoveryPeer::ActionType> OnPeersChanged;

		std::vector<DiscoveryPeer> PeerList();

		DiscoveryPeer LocalPeer();

	protected:
		virtual void Execution();

		virtual void PostExecution();

		void MessageReceived(const XByteArray& message);

		void CleanOldPeers();

		void SendMulticastUpdate(DiscoveryPeer::ActionType action);

		void TestPrintList();

	private:
		std::vector<XMulticaster*> m_multicasters;
		XMutex m_peerMutex;
		std::string m_sendString;
		DiscoveryPeer* m_localPeer;
		std::vector<DiscoveryPeer> m_peers;
	};
}

#endif