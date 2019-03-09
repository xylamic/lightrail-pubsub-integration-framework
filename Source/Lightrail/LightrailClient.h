/*!
@file	LightrailClient.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef LIGHTRAILCLIENT_H
#define LIGHTRAILCLIENT_H

#include "Lightrail_global.h"
#include "ILightrail.h"
#include "DiscoveryServices.h"
#include "XTcpMessageReceiver.h"
#include "LightrailLogged.h"
#include "PeerActor.h"
#include "TerminalListing.h"
#include "Delivery.h"
#include "DetailedDelivery.h"
#include "TerminalDelivery.h"
#include "SecurityKey.h"
#include "DeliveryParseError.h"
#include <XThread.h>
#include <XMutex.h>
#include <XEvent.h>
#include <XQueue.h>
#include <vector>

namespace Xylasoft
{
	class LightrailClient : public ILightrail, protected LightrailLogged, protected XThread
	{
	public:
		LightrailClient(const wchar_t* name, const wchar_t* description, const wchar_t* instance = nullptr);

		virtual ~LightrailClient();

		// Implementation of ILightrail
	public:
		virtual const wchar_t* Name() override;

		virtual const wchar_t* Instance() override;

		virtual const wchar_t* Description() override;

		virtual void StartDeliveries() override;

		virtual void StopDeliveries() override;

		virtual void RegisterSecurityKey(const ISecurityKey* securityKey) override;

		virtual void DeregisterSecurityKey(const wchar_t* name) override;

		virtual void RegisterTerminal(Standpoint::StandpointType standpoint, const ITerminal* terminal) override;

		virtual void DeregisterTerminal(Standpoint::StandpointType standpoint, const wchar_t* name) override;

		virtual void Publish(const wchar_t* terminalName, const IDelivery* delivery) override;

		virtual const IDetailedDelivery* Request(const wchar_t* terminalName, const IDelivery* delivery, long timeout = 30000, long reqResponders = 1);

	protected:
		void EncryptMessage(IDelivery* delivery, const wchar_t* keyname);

		void SendTerminalListingToActor(const DiscoveryPeer& peer);

		void OnMessageReceived(const XByteArray& message);

		PeerActor* FindActor(const DiscoveryPeer& peer);

		void OnPeersChanged(DiscoveryPeer::ActionType action);

		virtual void Execution();

		bool ForwardPublishMessage();

		bool ForwardResponseMessage();

		bool ForwardRequestMessage();

	private:
		std::wstring m_name;
		std::wstring m_instance;
		std::wstring m_description;
		TerminalListing m_terminals;
		XMutex m_keyLock;
		std::map<std::wstring, const SecurityKey*> m_securityKeys;
		volatile bool m_isStarted;

		DiscoveryServices m_discovery;
		XTcpMessageReceiver m_receiver;

		std::map<DiscoveryPeer, PeerActor*> m_actors;

		XMutex m_requestLock;
		std::map<XUuid, std::pair<XEvent*, DetailedDelivery*> > m_requests;
		 
		volatile bool m_retrieveTerminalUpdateFromActors;
		volatile bool m_sendTerminalUpdateToActors;
		XMutex m_peerLock;
		XEvent m_localThreadTrigger;

		XQueue<TerminalSubscriptionDelivery*> m_localSubQueue;
		XQueue<TerminalRequestDelivery*> m_localReqQueue;
		XQueue<TerminalResponseDelivery*> m_localRespQueue;
	};
}

#endif // LIGHTRAILCLIENT_H
