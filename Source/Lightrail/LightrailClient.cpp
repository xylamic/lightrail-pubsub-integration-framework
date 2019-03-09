/*!
@file	LightrailClient.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "LightrailClient.h"

using namespace Xylasoft;

LightrailClient::LightrailClient(const wchar_t* name, const wchar_t* description, const wchar_t* instance)
	: LightrailLogged(L"LightrailClient"), m_terminals(DiscoveryPeer("", 0))
{
	// initialize the client
	this->m_name = name;
	this->m_description = description == nullptr ? L"" : description;
	this->m_instance = instance != nullptr ? instance : L"";
	this->m_isStarted = false;

	this->m_sendTerminalUpdateToActors = false;
	this->m_retrieveTerminalUpdateFromActors = false;

	// add callbacks
	this->m_discovery.OnPeersChanged.SetCallback(new XEventCallback<LightrailClient, DiscoveryPeer::ActionType>(
		this, &LightrailClient::OnPeersChanged));
	this->m_receiver.OnMessageReceived.SetCallback(new XEventCallback<LightrailClient, const XByteArray&>(
		this, &LightrailClient::OnMessageReceived));

	std::wostringstream message;
	message << L"Client created with name[" << this->m_name << L"] description[" << this->m_description << 
		L"] instance[" << this->m_instance << L"].";
	this->LogMessage(message, IXLogger::Information);
}

LightrailClient::~LightrailClient()
{
	// verify the client is stopped before taking memory out from underneath it
	this->StopDeliveries();

	// clear any security keys memory
	for (auto keyit = this->m_securityKeys.begin(); keyit != this->m_securityKeys.end(); ++keyit)
	{
		delete keyit->second;
	}
}

const wchar_t* LightrailClient::Name()
{
	return this->m_name.c_str();
}

const wchar_t* LightrailClient::Instance()
{
	return this->m_instance.c_str();
}

const wchar_t* LightrailClient::Description()
{
	return this->m_description.c_str();
}

void LightrailClient::StartDeliveries()
{
	if (this->m_isStarted)
	{
		return;
	}

	this->LogMessage(L"Starting...", IXLogger::Information);

	// start the TCP receiver and get the bind that it binds to 
	// (we let the OS generate an open port for us)
	this->m_receiver.StartListening();
	long port = m_receiver.GetBindPort();

	std::wostringstream message;
	message << "TCP listener started on port " << port << ".";
	this->LogMessage(message, IXLogger::Information);

	// start discovery services
	this->m_discovery.StartServices(port);

	// update our own terminals to reflect that our port has changed
	this->m_terminals.UpdateOwner(this->m_discovery.LocalPeer());

	// start the local thread for sending out our terminals and deliveries to the user
	this->Start();

	this->m_isStarted = true;

	// wait for any actors to be discovered
	this->Sleep(2000);

	this->LogMessage(L"Started.", IXLogger::Information);
}

void LightrailClient::StopDeliveries()
{
	if (!this->m_isStarted)
	{
		return;
	}

	this->LogMessage(L"Stopping...", IXLogger::Information);

	// stop discovery
	this->m_discovery.StopServices();

	// stop the receiver
	this->m_receiver.StopListening();

	// mark the local threading for stopping and signal
	this->Kill();
	this->m_localThreadTrigger.Signal();

	// stop any actors
	XMutexLocker peerLocker(this->m_peerLock);
	while (this->m_actors.size() > 0)
	{
		PeerActor* actor = this->m_actors.begin()->second;

		actor->StopProcessing();
		delete actor;

		this->m_actors.erase(this->m_actors.begin());
	}
	peerLocker.Unlock();

	// remove any waiting requests
	XMutexLocker requestLocker(this->m_requestLock);
	while (this->m_requests.size() > 0)
	{
		auto requestIt = this->m_requests.begin();
		if (requestIt->second.first != nullptr)
		{
			delete requestIt->second.first;
		}
		if (requestIt->second.second != nullptr)
		{
			delete requestIt->second.second;
		}
		this->m_requests.erase(this->m_requests.begin());
	}
	requestLocker.Unlock();

	// wait for the local thread
	this->Wait();

	this->m_isStarted = false;

	this->LogMessage(L"Stopped...", IXLogger::Information);
}

void LightrailClient::RegisterSecurityKey(const ISecurityKey* securityKey)
{
	XMutexLocker locker(this->m_keyLock);

	auto keyit = this->m_securityKeys.find(securityKey->Name());
	if (keyit != this->m_securityKeys.end())
	{
		throw ArgumentException(L"The specified key name is already in use.");
	}

	const SecurityKey* key = static_cast<const SecurityKey*>(securityKey);
	this->m_securityKeys[key->Name()] = key;
}

void LightrailClient::DeregisterSecurityKey(const wchar_t* name)
{
	if (this->m_terminals.CheckForSecurityKeyUse(name))
	{
		throw DataException(L"The key is currently in use.");
	}

	XMutexLocker locker(this->m_keyLock);

	auto keyit = this->m_securityKeys.find(name);
	if (keyit != this->m_securityKeys.end())
	{
		delete keyit->second;
		this->m_securityKeys.erase(keyit->first);
	}
}

void LightrailClient::RegisterTerminal(Standpoint::StandpointType standpoint, const ITerminal* terminal)
{
	// check for security
	if (terminal->SecurityKeyName() != nullptr)
	{
		XMutexLocker locker(this->m_keyLock);

		auto keyit = this->m_securityKeys.find(terminal->SecurityKeyName());
		if (keyit == this->m_securityKeys.end())
		{
			throw DataException(L"The terminal requires a security key that is not registered.");
		}
	}
			
	// add the terminal to the local list
	const Terminal* cterminal = dynamic_cast<const Terminal*>(terminal);
	this->m_terminals.AddTerminal(standpoint, *cterminal);

	// remove the memory from the terminal passed in
	delete terminal;

	// if running and this is a subscription/response, trigger that topics are updated across the actors
	if (standpoint == Standpoint::RESPONDER || standpoint == Standpoint::SUBSCRIBER)
	{
		this->m_sendTerminalUpdateToActors = true;
		this->m_localThreadTrigger.Signal();
	}
}

void LightrailClient::DeregisterTerminal(Standpoint::StandpointType standpoint, const wchar_t* name)
{
	// remove the terminal by the given name
	this->m_terminals.RemoveTerminal(standpoint, name);

	// if running and this is a subscription/response, trigger that topics are updated across the actors
	if (standpoint == Standpoint::RESPONDER || standpoint == Standpoint::SUBSCRIBER)
	{
		this->m_sendTerminalUpdateToActors = true;
		this->m_localThreadTrigger.Signal();
	}
}

void LightrailClient::Publish(const wchar_t* terminalName, const IDelivery* delivery)
{
	// find the terminal
	Terminal terminal;
	if (!this->m_terminals.FindTerminal(Standpoint::PUBLISHER, terminalName, terminal))
	{
		throw ArgumentException(L"The specified terminal name was not registered.");
	}

	// verify the message is not empty
	if (delivery->MessageSize() == 0)
	{
		throw DataException(L"The message cannot be empty.");
	}

	// get local peer
	DiscoveryPeer localpeer = this->m_discovery.LocalPeer();

	// encrypt the message if necessary
	IDelivery* cdelivery = const_cast<IDelivery*>(delivery);
	this->EncryptMessage(cdelivery, terminal.SecurityKeyName());

	// create the details for the delivery and serialize it
	DetailedDelivery dd(static_cast<const Delivery*>(delivery), terminal, this->m_name.c_str(), localpeer);
	std::auto_ptr<Package> package(Package::CreatePackage(Package::PUBLISH, dd));

	// finding matching terminal
	XMutexLocker locker(this->m_peerLock);
	std::vector<Terminal> matchTerminals;
	for (auto actorit = this->m_actors.begin(); actorit != this->m_actors.end(); actorit++)
	{
		if (actorit->second->MatchTerminal(Standpoint::SUBSCRIBER, terminal, matchTerminals))
		{
			// supply the matching actor with a copy of the package that they can manage themselves
			actorit->second->EnqueuePackage(new Package(*package));
		}
	}
}

const IDetailedDelivery* LightrailClient::Request(const wchar_t* terminalName, const IDelivery* delivery, long timeout, long reqResponders)
{
	// validate arguments
	if (terminalName == nullptr)
	{
		throw ArgumentException(L"The terminal must be specified.");
	}
	else if (delivery == nullptr)
	{
		throw ArgumentException(L"A delivery must be specified.");
	}
	else if (timeout <= 0)
	{
		throw ArgumentException(L"The timeout value was invalid.");
	}
	else if (reqResponders <= 0)
	{
		throw ArgumentException(L"The number of responders requested was invalid.");
	}

	// find the terminal
	Terminal terminal;
	if (!this->m_terminals.FindTerminal(Standpoint::REQUESTER, terminalName, terminal))
	{
		throw ArgumentException(L"The specified terminal name was not registered.");
	}

	// verify the message is not empty
	if (delivery->MessageSize() == 0)
	{
		throw DataException(L"The message cannot be empty.");
	}

	// get local peer
	DiscoveryPeer localpeer = this->m_discovery.LocalPeer();

	// encrypt the message if necessary
	IDelivery* cdelivery = const_cast<IDelivery*>(delivery);
	this->EncryptMessage(cdelivery, terminal.SecurityKeyName());

	// create the details for the delivery and serialize it
	DetailedDelivery dd(static_cast<const Delivery*>(delivery), terminal, this->m_name.c_str(), localpeer);
	Package* package = Package::CreatePackage(Package::REQUEST, dd);

	// get the request id
	XUuid requestId = dd.Identifier();

	// find response to enqueue message to
	{
		XMutexLocker locker(this->m_peerLock);
			
		// finding matching terminal
		std::vector<Terminal> matchTerminals;
		std::vector<PeerActor*> validActors;
		for (auto actorit = this->m_actors.begin(); actorit != this->m_actors.end(); actorit++)
		{
			if (actorit->second->MatchTerminal(Standpoint::RESPONDER, terminal, matchTerminals))
			{
				validActors.push_back(actorit->second);
			}
		}

		// if not responders are listed, go ahead and return the timeout
		if (validActors.size() == 0)
		{
			delete package;
			throw TimeoutException(L"There are no listeners for this terminal string.");
		}
		// if there are responders, select one
		else
		{
			// find all responders that are currently marked as active
			std::vector<PeerActor*> validActiveActors;
			size_t actorIndex = 0;
			while (actorIndex < validActors.size())
			{
				PeerActor* peer = validActors[actorIndex];

				// if the actor is active, add to active list and remove from valid list
				if (peer->IsActive())
				{
					validActiveActors.push_back(peer);
					validActors.erase(validActors.cbegin() + actorIndex);
				}
				// if inactive, leave in valid list and continue
				else
				{
					actorIndex++;
				}
			}

			// create the response holder as we know we will be sending the request at this time
			{
				XMutexLocker locker(this->m_requestLock);

				this->m_requests[requestId] = std::pair<XEvent*, DetailedDelivery*>(new XEvent(), static_cast<DetailedDelivery*>(nullptr));
			}

			// for each requested responder, until we reach the max or have none remaining
			for (int reqIndex = reqResponders; reqIndex > 0; reqIndex--)
			{
				// if we are sending to more than 1 responder, then duplicate the package before sending,
				// unless this is the last responder, then send the original
				Package* queuePackage = nullptr;
				if (reqIndex == 1)
				{
					queuePackage = package;
				}
				else
				{
					queuePackage = new Package(*package);
				}

				// if we have more than 1 actor that is both active and valid, then select an
				// actor from that list
				if (validActiveActors.size() > 0)
				{
					int pick = XUtilities::GenerateRandomNumber() % validActiveActors.size();
					validActiveActors[pick]->EnqueuePackage(queuePackage);

					validActiveActors.erase(validActiveActors.cbegin() + pick);
				}
				// if we have more than one valid actor, but all are inactive, select one and hope
				// it works
				else if (validActors.size() > 0)
				{
					int pick = XUtilities::GenerateRandomNumber() % validActors.size();
					validActors[pick]->EnqueuePackage(queuePackage);

					validActors.erase(validActors.cbegin() + pick);
				}
				// if we are out of responders, don't send and cleanup the package
				else
				{
					delete queuePackage;
				}
			}
		}
	}

	// wait for a response
	if (this->m_requests[requestId].first->Wait(timeout))
	{
		XMutexLocker locker(this->m_requestLock);

		if (this->m_requests[requestId].second == nullptr)
		{
			throw ArgumentException(L"The response was empty.");
		}
		else
		{
			DetailedDelivery* response = this->m_requests[requestId].second;

			delete this->m_requests[requestId].first;
			this->m_requests.erase(requestId);

			if (response->ExceptionMessage() != NULL)
			{
				// create the exception
				std::string msg = "Error received from responder: ";
				msg.append(response->ExceptionMessage());

				// delete the response to make sure no memory is leaked
				delete response;

				// throw the exception from the responder to the requester
				throw CommunicationException(XUtilities::GetWStringFromString(msg).c_str());
			}

			return response;
		}
	}
	else
	{
		XMutexLocker locker(this->m_requestLock);

		delete this->m_requests[requestId].first;
		this->m_requests.erase(requestId);

		throw TimeoutException(L"The timeout was exceeded before a response was received.");
	}
}

void LightrailClient::EncryptMessage(IDelivery* delivery, const wchar_t* keyname)
{
	if (keyname == nullptr)
	{
		return;
	}

	XMutexLocker secLocker(this->m_keyLock);

	XByteArray encryptedData = this->m_securityKeys[keyname]->Encrypt(XByteArray(delivery->Message(), delivery->MessageSize()));
	delivery->SetMessage(encryptedData.ConstData(), encryptedData.Size(), delivery->Encoding());
}

void LightrailClient::SendTerminalListingToActor(const DiscoveryPeer& peer)
{
	// serialize the terminal list of ourself and prepare package
	Package* peerPackage = Package::CreatePackage(this->m_terminals);

	// enqueue the message for the peer that made the request
	PeerActor* actor = this->FindActor(peer);
	if (actor != nullptr)
	{
		actor->EnqueuePackage(peerPackage);

		std::wostringstream enqueueStream;
		enqueueStream << L"The actor [" << XUtilities::GetWStringFromString(peer.HostnamePort()) << "] had the terminal list enqueued.";
		this->LogMessage(enqueueStream, IXLogger::Debugging);
	}
}

void LightrailClient::OnMessageReceived(const XByteArray& message)
{
	this->LogMessage(L"Message received.", IXLogger::Debugging);
			
	try
	{
		// deserialize the package
		Package package(message);

		// build a serializer for the contents of the package
		XSerializer packageSerializer(&package.Data());

		std::wostringstream packageStream;
		packageStream << L"The message received is of type: " << package.Type();
		this->LogMessage(packageStream, IXLogger::Debugging);

		// switch the type of package it is
		switch (package.Type())
		{
		// if this is a delivery of a terminal list, then store the list
		case Package::TERMINALLIST:
			{
				// deserialize the terminal listing
				TerminalListing* listing = new TerminalListing(packageSerializer);

				// lock the peers as one of them are about to be updated
				XMutexLocker locker(this->m_peerLock);

				// find the actor this list was associated with
				PeerActor* actor = this->FindActor(listing->Owner());

				// apply the topic list to the actor
				if (actor != nullptr)
				{
					actor->SetTerminals(listing);

					std::wostringstream terminalStream;
					terminalStream << L"The actor [" << XUtilities::GetWStringFromString(listing->Owner().HostnamePort()) << "] had their terminal list updated.";
					this->LogMessage(terminalStream, IXLogger::Debugging);
				}
				break;
			}
		// if this is a delivery of a terminal listing request
		case Package::TERMINALREQUEST:
			{
				XMutexLocker locker(this->m_peerLock);

				// deserialize the peer that made the request
				DiscoveryPeer peer(packageSerializer);

				// send the list to the requesting peer
				this->SendTerminalListingToActor(peer);

				break;
			}
		// if this was a publish/request/response, then queue it for delivery to the user
		case Package::PUBLISH:
		case Package::REQUEST:
		case Package::RESPONSE:
			{
				// perform throttling if needed only on data messages (request/response/publish/subscribe)
				int throttleIndex = XUtilities::MethodThrottling(this->m_localSubQueue.MemorySize() + this->m_localReqQueue.MemorySize() +
					this->m_localRespQueue.MemorySize());
				if (throttleIndex > 1)
				{
					this->LogMessage(L"Messages are being received significantly faster than messages can be deployed to the user. Slowing.", IXLogger::Warning);
				}
				else if (throttleIndex > 3)
				{
					this->LogMessage(L"Messages are being received EXTREMELY faster than messages can be deployed to the user. Slowing much more.", IXLogger::Warning);
				}

				// get the delivery for the client
				DetailedDelivery* detailedDelivery = new DetailedDelivery(packageSerializer);

				// find standpoint we would be receiving from
				Standpoint::StandpointType standpoint = Standpoint::SUBSCRIBER;
				if (package.Type() == Package::REQUEST)
				{
					standpoint = Standpoint::RESPONDER;
				}
				else if (package.Type() == Package::RESPONSE)
				{
					standpoint = Standpoint::REQUESTER;
				}

				// find the terminal this was received on
				std::vector<Terminal> receiptTerminals;
				if (!this->m_terminals.MatchTerminal(standpoint, static_cast<const Terminal&>(detailedDelivery->SourceTerminal()), receiptTerminals))
				{
					this->LogMessage(L"A message was received, but terminal was not active.", IXLogger::Warning);
					delete detailedDelivery;
				}
				else
				{
					// check for security mismatch
					bool hasMismatch = false;
					const wchar_t* securityKeyName = receiptTerminals[0].SecurityKeyName();
					for (size_t i = 1; i < receiptTerminals.size(); i++)
					{
						const wchar_t* thisKeyName = receiptTerminals[i].SecurityKeyName();
						if (securityKeyName == nullptr && thisKeyName == nullptr)
						{
							continue;
						}
						else if ((securityKeyName != nullptr && thisKeyName == nullptr) ||
									(securityKeyName == nullptr && thisKeyName != nullptr))
						{
							hasMismatch = true;
							Terminal receiptTerminal = receiptTerminals[0];
							const ITerminal* pReceiptTerminal = static_cast<const ITerminal*>(&receiptTerminal);
							DeliveryParseError parseError(pReceiptTerminal, 
								detailedDelivery->SourceApplication(),
								detailedDelivery->SourceHost(),
								detailedDelivery->SourcePort(),
								DeliveryParseError::SECURITY_MISMATCH,
								std::exception("There were multiple terminals receiving a message with differing security."));

							this->OnDeliveryParseError.Execute(parseError);
							break;
						}
					}

					// if there is a mismatch, then delete and continue
					if (hasMismatch)
					{
						delete detailedDelivery;
						return;
					}

					// decrypt if necessary
					if (securityKeyName != nullptr)
					{
						try
						{
							XMutexLocker secLocker(this->m_keyLock);

							DetailedDelivery* dd = const_cast<DetailedDelivery*>(static_cast<const DetailedDelivery*>(detailedDelivery));
							dd->DecryptMessage(this->m_securityKeys[securityKeyName]);
						}
						catch (std::exception& ex)
						{
							Terminal receiptTerminal = receiptTerminals[0];
							const ITerminal* pReceiptTerminal = static_cast<const ITerminal*>(&receiptTerminal);
							DeliveryParseError parseError(pReceiptTerminal, 
								detailedDelivery->SourceApplication(),
								detailedDelivery->SourceHost(),
								detailedDelivery->SourcePort(),
								DeliveryParseError::DECRYPTION,
								ex);

							this->OnDeliveryParseError.Execute(parseError);

							delete detailedDelivery;
							return;
						}
					}
							
					// enqueue the message
					switch (standpoint)
					{
					case Standpoint::SUBSCRIBER:
						this->m_localSubQueue.Enqueue(new TerminalSubscriptionDelivery(detailedDelivery, receiptTerminals), detailedDelivery->MessageSize());
						break;
					case Standpoint::REQUESTER:
						this->m_localRespQueue.Enqueue(new TerminalResponseDelivery(detailedDelivery, receiptTerminals), detailedDelivery->MessageSize());
						break;
					case Standpoint::RESPONDER:
						this->m_localReqQueue.Enqueue(new TerminalRequestDelivery(detailedDelivery, receiptTerminals), detailedDelivery->MessageSize());
						break;
					default:
						assert(false);
						break;
					}

					// signal the dequeue thread to retrieve the message (if not already signaled)
					this->m_localThreadTrigger.Signal();
				}

				break;
			}
		default:
			throw IOException(L"The package type requested was unrecognized.");
		}
	}
	catch (std::exception& ex)
	{
		this->LogException(L"Error occurred in message processing.", IXLogger::Error, ex);
	}
	catch (...)
	{
		this->LogException(L"Error occurred in message processing.", IXLogger::Error, std::exception("... error"));
	}
}

PeerActor* LightrailClient::FindActor(const DiscoveryPeer& peer)
{
	auto actorit = this->m_actors.find(peer);
	if (actorit == this->m_actors.end())
	{
		return nullptr;
	}
	else
	{
		return actorit->second;
	}
}

void LightrailClient::OnPeersChanged(DiscoveryPeer::ActionType action)
{
	XMutexLocker locker(this->m_peerLock);

	std::vector<DiscoveryPeer> discpeers = this->m_discovery.PeerList();

	// find any new peers
	for (auto discit = discpeers.cbegin(); discit != discpeers.cend(); discit++)
	{
		auto itactors = this->m_actors.find(*discit);
		if (itactors == this->m_actors.end())
		{

			try
			{
				this->m_actors[*discit] = new PeerActor(*discit);
				this->m_actors[*discit]->StartProcessing();

				std::wostringstream message;
				message << L"Started actor " << XUtilities::GetWStringFromString(discit->HostnamePort()) << L".";
				this->LogMessage(message, IXLogger::Debugging);
			}
			catch (std::exception& ex)
			{
				std::wostringstream message;
				message << L"Failed to start actor " << XUtilities::GetWStringFromString(discit->HostnamePort()) << L".";
				this->LogException(message, IXLogger::Error, ex);
			}
		}
	}

	// find any actors that are no longer present
	std::map<DiscoveryPeer, void*> discpeersmap;
	for (auto discit = discpeers.cbegin(); discit != discpeers.cend(); discit++)
	{
		discpeersmap[*discit] = nullptr;
	}
	std::vector<DiscoveryPeer> actorsToRemove;
	for (auto actorit = this->m_actors.begin(); actorit != this->m_actors.end(); actorit++)
	{
		auto discpeerit = discpeersmap.find(actorit->first);
		if (discpeerit == discpeersmap.end())
		{
			actorsToRemove.push_back(actorit->first);

			std::wostringstream message;
			message << L"Marked actor [" << XUtilities::GetWStringFromString(actorit->first.HostnamePort()) << L"] for removal.";
			this->LogMessage(message, IXLogger::Debugging);
		}
	}

	// remove actors that are no longer present
	while (actorsToRemove.size() > 0)
	{
		auto actorit = this->m_actors.find(actorsToRemove[0]);
		assert(actorit != this->m_actors.end());

		std::string actorname = actorsToRemove[0].HostnamePort();

		try
		{
			// stop and delete the actor
			actorit->second->StopProcessing();
			delete actorit->second;

			// remove the actor
			this->m_actors.erase(actorit);

			std::wostringstream message;
			message << L"Removed actor " << XUtilities::GetWStringFromString(actorname) << L".";
			this->LogMessage(message, IXLogger::Debugging);
		}
		catch (std::exception& ex)
		{
			std::wostringstream message;
			message << L"Failed to remove actor " << XUtilities::GetWStringFromString(actorname) << L".";
			this->LogException(message, IXLogger::Error, ex);
		}

		actorsToRemove.erase(actorsToRemove.begin());
	}

	locker.Unlock();

	// trigger that topics are updated across the actors
	this->m_retrieveTerminalUpdateFromActors = true;
	this->m_localThreadTrigger.Signal();
}

void LightrailClient::Execution()
{
	DiscoveryPeer localPeer = this->m_discovery.LocalPeer();

	time_t lastUpdate = 0, currentT;

	do
	{
		try
		{
			// get the current time
			::time(&currentT);

			// see if we have exceed the time to wait for an update from peers
			if (lastUpdate < (currentT - GlobalSettings::TopicUpdateTime) || this->m_retrieveTerminalUpdateFromActors)
			{
				// lock the peers while we queue requests
				XMutexLocker locker(this->m_peerLock);

				// create the package
				std::auto_ptr<Package> package(Package::CreatePackage(localPeer));

				// send terminal requests to all actors
				for (auto actorit = this->m_actors.begin(); actorit != this->m_actors.end(); actorit++)
				{
					actorit->second->EnqueuePackage(new Package(*package));
				}

				this->m_retrieveTerminalUpdateFromActors = false;
				lastUpdate = currentT;
			}

			// see if we need to send our terminal list out (our terminals changed)
			if (this->m_sendTerminalUpdateToActors)
			{
				XMutexLocker locker(this->m_peerLock);

				for (auto actorit = this->m_actors.begin(); actorit != this->m_actors.end(); actorit++)
				{
					this->SendTerminalListingToActor(actorit->first);
				}

				this->m_sendTerminalUpdateToActors = false;
			}
				
			// see if there are messages waiting to be delivery, requests have first priority, then
			// responses, then publications
			if (this->ForwardRequestMessage())
			{
				// message was processed, check for the next message
				continue;
			}
			else if (this->ForwardResponseMessage())
			{
				// message was processed, check for the next message
				continue;
			}
			else if (this->ForwardPublishMessage())
			{
				// message was processed, check for the next message
				continue;
			}
			// if we get to this point, no messages were waiting and we should go into a wait state
			else if (this->m_localThreadTrigger.Wait(GlobalSettings::EmptyQueueWaitTime * 1000))
			{
				this->m_localThreadTrigger.Reset();
			}
		}
		catch (std::exception& ex)
		{
			this->LogException(L"The local message processor failed. Waiting.", IXLogger::Error, ex);
			this->Sleep(GlobalSettings::ConnectionErrorWaitTime);
		}
		catch (...)
		{
			this->LogMessage(L"The local message processor failed with unknown error. Waiting.", IXLogger::Error);
			this->Sleep(GlobalSettings::ConnectionErrorWaitTime);
		}

	} while (!this->ShouldDie());
}

bool LightrailClient::ForwardPublishMessage()
{
	if (this->m_localSubQueue.Size() <= 0)
	{
		return false;
	}

	TerminalSubscriptionDelivery* delivery = this->m_localSubQueue.Peek();

	try
	{
		this->OnPublicationReceived.Execute(*delivery);
	}
	catch (std::exception& ex)
	{
		this->LogException(L"Error delivering publication to callback.", IXLogger::Error, ex);
	}
	catch (...)
	{
		this->LogMessage(L"Error delivering publication to callback with unknown error.", IXLogger::Error);
	}

	delete delivery;
	this->m_localSubQueue.Dequeue();

	return true;
}

bool LightrailClient::ForwardResponseMessage()
{
	if (this->m_localRespQueue.Size() <= 0)
	{
		return false;
	}

	TerminalResponseDelivery* delivery = this->m_localRespQueue.Peek();

	try
	{
		XMutexLocker requestLocker(this->m_requestLock);

		// try to find the matching request
		const DetailedDelivery* detailedResponse = static_cast<const DetailedDelivery*>(delivery->DetailedDelivery());
		auto requestit = this->m_requests.find(detailedResponse->RequestIdentifier());

		// if the request was found, set the response and signal
		if (requestit != this->m_requests.end())
		{
			this->m_requests[detailedResponse->RequestIdentifier()].second = new DetailedDelivery(*detailedResponse);
			this->m_requests[detailedResponse->RequestIdentifier()].first->Signal();
		}
	}
	catch (std::exception& ex)
	{
		this->LogException(L"Error delivering response to callback.", IXLogger::Error, ex);
	}
	catch (...)
	{
		this->LogMessage(L"Error delivering response to callback with unknown error.", IXLogger::Error);
	}

	delete delivery;
	this->m_localRespQueue.Dequeue();

	return true;
}

bool LightrailClient::ForwardRequestMessage()
{
	if (this->m_localReqQueue.Size() <= 0)
	{
		return false;
	}

	TerminalRequestDelivery* delivery = this->m_localReqQueue.Peek();

	try
	{
		std::string exceptionMessage;

		try
		{
			this->OnRequestReceived.Execute(*delivery);

			if (delivery->ReturnedDelivery()->MessageSize() <= 0)
			{
				throw DataException(L"The response was empty.");
			}
		}
		catch (std::exception& ex)
		{
			exceptionMessage = ex.what();
		}
		catch (...)
		{
			exceptionMessage = "Unrecognized exception occurred.";
		}

		// encrypt the message if necessary
		IDelivery* cdelivery = const_cast<IDelivery*>(delivery->ReturnedDelivery());
		this->EncryptMessage(cdelivery, delivery->ReceiptTerminal(0)->SecurityKeyName());

		XMutexLocker locker(this->m_peerLock);

		PeerActor* actor = this->FindActor(DiscoveryPeer(delivery->DetailedDelivery()->SourceHost(),
			delivery->DetailedDelivery()->SourcePort()));

		// if the actor still exists, queue the response
		if (actor != nullptr)
		{
			// build response message
			const Delivery* response = static_cast<Delivery*>(delivery->ReturnedDelivery());
			const Terminal* responseTerminal = static_cast<const Terminal*>(delivery->ReceiptTerminal(0));
			DetailedDelivery dd(response, *responseTerminal, this->m_name.c_str(), this->m_discovery.LocalPeer());

			// set exception if one occurred
			if (exceptionMessage.size() > 0)
			{
				dd.SetExceptionMessage(exceptionMessage.c_str());
			}

			// apply request identifier
			const DetailedDelivery* requestdd = static_cast<const DetailedDelivery*>(delivery->DetailedDelivery());
			dd.SetRequestIdentifier(requestdd->Identifier());

			// enqueue the response
			actor->EnqueuePackage(Package::CreatePackage(Package::RESPONSE, dd));
		}
	}
	catch (std::exception& ex)
	{
		this->LogException(L"Error delivering request to callback.", IXLogger::Error, ex);
	}
	catch (...)
	{
		this->LogMessage(L"Error delivering request to callback with unknown error.", IXLogger::Error);
	}

	delete delivery;
	this->m_localReqQueue.Dequeue();

	return true;
}