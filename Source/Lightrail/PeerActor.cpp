/*!
@file	PeerActor.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "PeerActor.h"

using namespace Xylasoft;

PeerActor::PeerActor(const DiscoveryPeer& peer)
	: LightrailLogged(std::wstring(L"PeerAction[").append(XUtilities::GetWStringFromString(peer.HostnamePort())).append(L"]").c_str()), m_peer(peer)
{
	std::vector<std::string> addresses = XSocketDevice::GetAddressForHost(peer.Hostname().c_str());
	if (addresses.size() == 0)
	{
		throw ResolutionException(L"There were not addresses found for the peer.");
	}

	std::wostringstream message;
	message << L"Found " << addresses.size() << L" addresses for actor.";
	this->LogMessage(message, IXLogger::Debugging);

	this->m_terminals = new TerminalListing(peer);
	this->m_isActive = false;

	std::wostringstream message2;
	message2 << L"Created PeerActor for host[" << XUtilities::GetWStringFromString(peer.HostnamePort()) << 
		L"] at address [" << XUtilities::GetWStringFromString(addresses[0].c_str()) << L"].";
	this->LogMessage(message2, IXLogger::Debugging);
}

PeerActor::~PeerActor()
{
	this->StopProcessing();

	// clean up any memory left in the queue
	while (this->m_terminalQueue.Size() > 0)
	{
		const Package* package = this->m_terminalQueue.Peek();

		delete package;
		this->m_terminalQueue.Dequeue();
	}
	while (this->m_reqrespQueue.Size() > 0)
	{
		const Package* package = this->m_reqrespQueue.Peek();

		delete package;
		this->m_reqrespQueue.Dequeue();
	}
	while (this->m_subpubQueue.Size() > 0)
	{
		const Package* package = this->m_subpubQueue.Peek();

		delete package;
		this->m_subpubQueue.Dequeue();
	}

	delete this->m_terminals;
}

void PeerActor::StartProcessing()
{
	if (this->IsRunning())
	{
		return;
	}

	this->Start();

	this->LogMessage(L"Started processor.", IXLogger::Debugging);
}

void PeerActor::StopProcessing()
{
	if (!this->IsRunning())
	{
		return;
	}

	this->Kill();
	this->m_queueEvent.Signal();

	this->Wait();

	this->LogMessage(L"Stopped processor.", IXLogger::Debugging);
}

void PeerActor::EnqueuePackage(const Package* package)
{
	// push the package and signal if the processor is waiting
	// these messages go into different queues to give priority to different
	// package types (ie: subscriptions are updated first, request/response
	// updated second, publications third
	if (package->Type() == Package::TERMINALLIST ||
		package->Type() == Package::TERMINALREQUEST)
	{
		this->m_terminalQueue.Enqueue(package, package->Data().Size());
	}
	else
	{
		// perform throttling if needed only on data messages (request/response/publish/subscribe)
		int throttleIndex = XUtilities::MethodThrottling(this->m_reqrespQueue.MemorySize() + this->m_subpubQueue.MemorySize());
		if (throttleIndex > 1)
		{
			this->LogMessage(L"The publisher is publishing significantly faster than messages can be sent. Slowing.", IXLogger::Warning);
		}
		else if (throttleIndex > 3)
		{
			this->LogMessage(L"The publisher is publishing extremely faster than messages can be sent. Slowing much more.", IXLogger::Warning);
		}

		if (package->Type() == Package::REQUEST ||
			package->Type() == Package::RESPONSE)
		{
			this->m_reqrespQueue.Enqueue(package, package->Data().Size());
		}
		else
		{
			this->m_subpubQueue.Enqueue(package, package->Data().Size());
		}
	}
	this->m_queueEvent.Signal();

	this->LogMessage(L"Enqueued package.", IXLogger::Debugging);
}

bool PeerActor::MatchTerminal(Standpoint::StandpointType standpoint, const Terminal& terminal, std::vector<Terminal>& matchTerminals)
{
	XMutexLocker locker(this->m_terminalLock);

	return this->m_terminals->MatchTerminal(standpoint, terminal, matchTerminals);
}

void PeerActor::SetTerminals(const TerminalListing* terminals)
{
	XMutexLocker locker(this->m_terminalLock);

	delete this->m_terminals;
	this->m_terminals = terminals;
}

TerminalListing PeerActor::Terminals()
{
	XMutexLocker locker(this->m_terminalLock);

	return *this->m_terminals;
}

/*!
Get whether this user is currently active.
@returns True if active.
*/
bool PeerActor::IsActive()
{
	return this->m_isActive;
}

void PeerActor::Execution()
{
	// execution is starting, initially set active to true
	this->m_isActive = true;

	do
	{
		try
		{
			const Package* package = nullptr;

			// see if there are any messages waiting in the specified priorities
			int queueType = 0;
			if (this->m_terminalQueue.Size() > 0)
			{
				package = this->m_terminalQueue.Peek();
				queueType = 1;
			}
			else if (this->m_reqrespQueue.Size() > 0)
			{
				package = this->m_reqrespQueue.Peek();
				queueType = 2;
			}
			else if (this->m_subpubQueue.Size() > 0)
			{
				package = this->m_subpubQueue.Peek();
				queueType = 3;
			}

			// if there were no messages waiting
			if (package == nullptr)
			{
				// wait for a signal of a new package
				if (m_queueEvent.Wait(GlobalSettings::EmptyQueueWaitTime * 1000))
				{
					this->m_queueEvent.Reset();
				}
			}
			else
			{
				try
				{
					// serialize the package
					XByteArray message;
					XSerializer serializer(message);
					package->Serialize(serializer);

					// try to send the package
					std::vector<std::string> addresses = XSocketDevice::GetAddressForHost(this->m_peer.Hostname().c_str());
					if (addresses.size() == 0)
					{
						throw ResolutionException(L"The hostname could not be resolved anymore.");
					}

					XTcpMessageSender sender(addresses[0].c_str(), this->m_peer.Port());
					sender.Send(message);

					// message was successfully sent, lock the queue and remove
					// the message
					switch (queueType)
					{
					case 1:
						this->m_terminalQueue.Dequeue();
						break;
					case 2:
						this->m_reqrespQueue.Dequeue();
						break;
					case 3:
						this->m_subpubQueue.Dequeue();
						break;
					default:
						// this should never occur
						assert(false);
					}

					// delete the package memory
					delete package;

					// make sure this user is marked as active since the delivery was successful
					this->m_isActive = true;

					// continue to checking the queue again
					continue;
				}
				catch (std::exception& ex)
				{
					// mark user as inactive until they successfully accept a message again
					this->m_isActive = false;

					this->LogException(L"Failed to send the package.", IXLogger::Warning, ex);

					// if we get here, an error occurred and we need to wait the timeout for an error
					if (m_queueEvent.Wait(GlobalSettings::ConnectionErrorWaitTime * 1000))
					{
						this->m_queueEvent.Reset();
					}
				}
			}
		}
		catch (std::exception& ex)
		{
			this->LogException(L"The process threw an exception.", IXLogger::Error, ex);
		}
		catch (...)
		{
			this->LogException(L"The process threw an exception.", IXLogger::Error, std::exception("[Unknown Exception]"));
		}

	} while (!this->ShouldDie());

	// set the user as no longer active
	this->m_isActive = false;
}