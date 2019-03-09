/*!
@file	DiscoveryServices.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "DiscoveryServices.h"
#include "GlobalSettings.h"
#include <XHostLookup.h>
#include <XUtilities.h>

#include <iostream>

using namespace Xylasoft;

DiscoveryServices::DiscoveryServices()
	: XThread(), LightrailLogged(L"DiscoveryServices")
{
	this->LogMessage(L"Constructing services...", IXLogger::Information);

	this->m_localPeer = nullptr;

	std::wostringstream mcAddressStream;
	mcAddressStream << L"Multicast address [" << XUtilities::GetWStringFromString(GlobalSettings::MulticastAddress) <<
		L"] Multicast TTL [" << GlobalSettings::MulticastTimeToLive << L"] Multicast Port [" << 
		GlobalSettings::MulticastPort << L"]";
	this->LogMessage(mcAddressStream, IXLogger::Information);

	std::vector<std::string> addresses = XSocketDevice::GetAddressForHost(XHostLookup::LocalHostName().c_str());

	// iterate through interfaces and create multicasters
	std::wostringstream message;
	message << L"There were " << addresses.size() << " interface addresses found at:";
	for (size_t i = 0; i < addresses.size(); i++)
	{
		message << L" (" << i << L") " << XUtilities::GetWStringFromString(addresses[i]);

		XMulticaster* multicaster = new XMulticaster(GlobalSettings::MulticastAddress.c_str(), GlobalSettings::MulticastPort, 
			addresses[i].c_str(), GlobalSettings::MulticastTimeToLive);
		multicaster->OnMessageReceived.SetCallback(new XEventCallback<DiscoveryServices, const XByteArray&>(
			this, &DiscoveryServices::MessageReceived));

		this->m_multicasters.push_back(multicaster);
	}
	this->LogMessage(message, IXLogger::Information);

	// create default interface multicaster
	XMulticaster* defMulticaster = new XMulticaster(GlobalSettings::MulticastAddress.c_str(), GlobalSettings::MulticastPort, 
		nullptr, GlobalSettings::MulticastTimeToLive);
	defMulticaster->OnMessageReceived.SetCallback(new XEventCallback<DiscoveryServices, const XByteArray&>(
		this, &DiscoveryServices::MessageReceived));
	this->m_multicasters.push_back(defMulticaster);

	this->LogMessage(L"Multicasters created.", IXLogger::Debugging);

	this->LogMessage(L"Constructed services.", IXLogger::Information);
}

DiscoveryServices::~DiscoveryServices()
{
	this->LogMessage(L"Destructing services...", IXLogger::Information);

	this->StopServices();

	delete this->m_localPeer;

	while (this->m_multicasters.size() > 0)
	{
		XMulticaster* multicaster = this->m_multicasters[0];
		delete multicaster;

		this->m_multicasters.erase(this->m_multicasters.begin());
	}

	this->LogMessage(L"Destructed services.", IXLogger::Information);
}

void DiscoveryServices::StartServices(int port)
{
	this->LogMessage(L"Starting services...", IXLogger::Information);

	if (this->IsRunning())
	{
		this->LogMessage(L"Services already running.", IXLogger::Information);
		return;
	}

	// build the local peer
	delete this->m_localPeer;
	this->m_localPeer = new DiscoveryPeer(XHostLookup::LocalHostName().c_str(), port);

	std::wostringstream message;
	message << L"The local host name was retrieved [" << XUtilities::GetWStringFromString(this->m_localPeer->Hostname()) << L"]";
	this->LogMessage(message, IXLogger::Debugging);

	message.clear();
	message << L"The port that is represented by this service is: " << this->m_localPeer->Port();
	this->LogMessage(message, IXLogger::Debugging);

	message.clear();
	message << L"The multicast message that will be transmitted is: " << this->m_localPeer->Serialize(DiscoveryPeer::None).c_str();
	this->LogMessage(message, IXLogger::Debugging);

	// start the multicast senders/receivers
	for (size_t i = 0; i < this->m_multicasters.size(); i++)
	{
		this->m_multicasters[i]->StartListening();
	}

	// start the discovery processor
	this->Start();

	this->LogMessage(L"Started services.", IXLogger::Information);
}

void DiscoveryServices::StopServices()
{
	this->LogMessage(L"Stopping services...", IXLogger::Information);

	if (!this->IsRunning())
	{
		this->LogMessage(L"Services already stopped.", IXLogger::Information);
		return;
	}

	// let the local thread start winding down before stopping the multicaster
	this->Kill();

	// stop the multicast listeners
	for (size_t i = 0; i < this->m_multicasters.size(); i++)
	{
		this->m_multicasters[i]->StopListening();
	}

	// clear all peers
	this->m_peers.clear();

	// wait for the local thread
	this->Wait(GlobalSettings::ThreadWaitTime * 1000);

	this->LogMessage(L"Stopped services.", IXLogger::Information);
}

void DiscoveryServices::MessageReceived(const XByteArray& message)
{
	XMutexLocker locker(this->m_peerMutex);

	DiscoveryPeer::ActionType action;
	DiscoveryPeer peer(message.ConstData(), action);
	
	// if this is a peer that has signaled that they are joining, send them an update of yourself
	if (action == DiscoveryPeer::Joining)
	{
		std::wostringstream stream;
		stream << L"Received a peer JOIN request from host[" << XUtilities::GetWStringFromString(peer.Hostname()) << 
			L"] and port[" << peer.Port() << L"], sending message...";
		this->LogMessage(stream.str().c_str(), IXLogger::Debugging);

		this->SendMulticastUpdate(DiscoveryPeer::None);
	}

	// get the current time
	time_t t;
	::time(&t);

	// iterate through existing peers
	for (std::vector<DiscoveryPeer>::iterator it = this->m_peers.begin();
		it != this->m_peers.end(); it++)
	{
		// if this peer matches one in the list
		if (it->Hostname().compare(peer.Hostname()) == 0 && it->Port() == peer.Port())
		{
			// if this was a signal of leaving, remove it from the list
			if (action == DiscoveryPeer::Leaving)
			{
				std::wostringstream stream;
				stream << L"Received a peer LEAVE request from host[" << XUtilities::GetWStringFromString(peer.Hostname()) << 
					L"] and port[" << peer.Port() << L"], removing from list...";
				this->LogMessage(stream.str().c_str(), IXLogger::Debugging);

				this->m_peers.erase(it);

				locker.Unlock();
				this->OnPeersChanged.Execute(DiscoveryPeer::Leaving);
				locker.Relock();
			}
			// if the action was none or joining, then update the time
			else
			{
				// update this item
				it->SetLastUpdate(t);
			}

			return;
		}
	}

	// if the peer was not in the list and it was not a signal of leaving, add them to the list
	if (action != DiscoveryPeer::Leaving)
	{
		std::wostringstream stream;
		stream << L"A new peer was found at host[" << XUtilities::GetWStringFromString(peer.Hostname()) << 
			L"] and port[" << peer.Port() << L"], adding to list...";
		this->LogMessage(stream.str().c_str(), IXLogger::Debugging);

		// if we get to this point, the peer is new to the list
		peer.SetLastUpdate(t);
		this->m_peers.push_back(peer);

		locker.Unlock();
		this->OnPeersChanged.Execute(DiscoveryPeer::Joining);
	}
}

std::vector<DiscoveryPeer> DiscoveryServices::PeerList()
{
	// this will only ever be called while in a PeerChanged event, so we don't
	// want to lock again because it will cause a deadlock
	return this->m_peers;
}

void DiscoveryServices::Execution()
{
	this->LogMessage(L"The background execution was started.", IXLogger::Information);

	time_t lastUpdate = 0;
	while (!this->ShouldDie())
	{
		time_t currentTime;
		::time(&currentTime);

		// only update after list update check time last elapsed
		if (lastUpdate < currentTime - GlobalSettings::ConnectionListUpdateCheckTime)
		{
			this->LogMessage(L"Sending connection update.", IXLogger::Debugging);

			try
			{
				// create the message and if this is the first call, signal that this peer is joining
				DiscoveryPeer::ActionType action = lastUpdate == 0 ? DiscoveryPeer::Joining : DiscoveryPeer::None;

				// send the message
				this->SendMulticastUpdate(action);
			}
			catch (std::exception& ex)
			{
				this->LogException(L"Error in sending discovery update.", IXLogger::Error, ex);
			}

			// set this as the last update time
			lastUpdate = currentTime;
		}

		// clean any old peers that are out of date
		this->CleanOldPeers();

		//this->TestPrintList();

		// wait 5 seconds before checks
		this->Sleep(5000);
	}
	this->LogMessage(L"The background execution was stopped.", IXLogger::Information);
}

void DiscoveryServices::PostExecution()
{
	this->LogMessage(L"Sending closing multicast.", IXLogger::Debugging);
	this->SendMulticastUpdate(DiscoveryPeer::Leaving);
}

void DiscoveryServices::TestPrintList()
{
	XMutexLocker locker(this->m_peerMutex);

	for (std::vector<DiscoveryPeer>::iterator it = this->m_peers.begin();
		it != this->m_peers.end(); it++)
	{
		std::cout << "[Host]" << it->Hostname().c_str() << " [Port]" << it->Port() << " [Last]" << it->LastUpdate() << std::endl;
	}
	std::cout << "\n";
}

DiscoveryPeer DiscoveryServices::LocalPeer()
{
	return *this->m_localPeer;
}

void DiscoveryServices::SendMulticastUpdate(DiscoveryPeer::ActionType action)
{
	// if our port is unspecified, do not send, only listen
	if (this->m_localPeer->Port() < 0)
	{
		return;
	}

	// create the message
	std::string msg = this->m_localPeer->Serialize(action);

	// prepare the message
	XByteArray updateMsg(msg.c_str(), static_cast<long>(msg.size()));

	// send the message
	for (size_t i = 0; i < this->m_multicasters.size(); i++)
	{
		try
		{
			this->m_multicasters[i]->Send(updateMsg);
		}
		catch (std::exception& ex)
		{
			const char* bindAddress = this->m_multicasters[i]->GetBindAddress();

			std::wostringstream message;
			message << L"Failed to send multicast update to [";
			message << (bindAddress == NULL) ? L"Default" : XUtilities::GetWStringFromString(std::string(bindAddress));
			message << L"]";

			this->LogException(message, IXLogger::Error, ex);
		}
	}
}

void DiscoveryServices::CleanOldPeers()
{
	XMutexLocker locker(this->m_peerMutex);

	time_t currentT;
	::time(&currentT);

	time_t validT = currentT - GlobalSettings::ConnectionListUpdateTimeout;

	size_t index = 0;
	bool peerCleaned = false;
	while (index < this->m_peers.size())
	{
		DiscoveryPeer& peer = this->m_peers[index];
		if (peer.LastUpdate() < validT)
		{
			std::wostringstream stream;
			stream << L"Removing peer with name[" << XUtilities::GetWStringFromString(peer.Hostname()) << 
				L"] and port[" << peer.Port() << L"] that has not been updated in " << 
				(currentT - peer.LastUpdate()) << L" seconds due to TIMEOUT.";
			this->LogMessage(stream.str().c_str(), IXLogger::Debugging);

			this->m_peers.erase(this->m_peers.begin() + index);
			peerCleaned = true;
		}
		else
		{
			index++;
		}
	}

	locker.Unlock();

	if (peerCleaned)
	{
		this->OnPeersChanged.Execute(DiscoveryPeer::Leaving);
	}
}