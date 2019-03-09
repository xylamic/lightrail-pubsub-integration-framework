/*!
@file	Discovery.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Discovery.h"

using namespace Xylasoft;

Discovery::Discovery()
{
	this->m_services.OnPeersChanged.SetCallback(new XEventCallback<Discovery, DiscoveryPeer::ActionType>(
		this, &Discovery::PeersChanged));
}

Discovery::~Discovery()
{
}

void Discovery::StartDiscovery()
{
	this->m_services.StartServices();
}

void Discovery::StopDiscovery()
{
	this->m_services.StopServices();
}

SimpleListContainer<Participant>* Discovery::GetSubnetParticipants()
{
	XMutexLocker locker(this->m_peerLock);

	SimpleListLightrail<Participant>* list = new SimpleListLightrail<Participant>();

	std::vector<Participant> participants;

	for (auto peerit = this->m_peers.cbegin(); peerit != this->m_peers.cend(); peerit++)
	{
		participants.push_back(Participant(peerit->Hostname().c_str(), peerit->Port()));
	}

	list->SetElements(participants);
			
	return list;
}

void Discovery::PeersChanged(DiscoveryPeer::ActionType type)
{
	XMutexLocker locker(this->m_peerLock);

	this->m_peers = this->m_services.PeerList();
}