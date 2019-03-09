/*!
@file	Discovery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "Lightrail_global.h"
#include "IDiscovery.h"
#include "DiscoveryServices.h"
#include "XMutex.h"
#include "SimpleListLightrail.h"

namespace Xylasoft
{
	class Discovery : public IDiscovery
	{
	public:
		Discovery();

		virtual ~Discovery();

		virtual void StartDiscovery() override;

		virtual void StopDiscovery() override;

		virtual SimpleListContainer<Participant>* GetSubnetParticipants() override;

	protected:
		void PeersChanged(DiscoveryPeer::ActionType type);
		
	private:
		DiscoveryServices m_services;
		std::vector<DiscoveryPeer> m_peers;
		XMutex m_peerLock;
	};
}

#endif