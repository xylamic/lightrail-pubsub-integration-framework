/*!
@file	XMulticaster.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XMulticaster.h"
#include <iostream>

using namespace Xylasoft;

XMulticaster::XMulticaster(const char* sendAddress, int port, const char* bindAddress, int ttl)
	: XSocketDevice()
{
	this->m_sendAddress = sendAddress;
    this->m_port = port;
	this->m_ttl = ttl;

	if (bindAddress == nullptr)
	{
		this->m_bindAddress = "";
	}
	else
	{
		this->m_bindAddress = bindAddress;
	}
}

XMulticaster::~XMulticaster()
{
    this->StopListening();
	if (this->IsRunning())
    {
		this->Wait(10000);
    }
}

void XMulticaster::Send(const XByteArray& message)
{
	this->SendMulticast(this->m_socketHandle, this->m_sendAddress.c_str(), this->m_port,
		message.ConstData(), message.Size(), this->GetBindAddress(), this->m_ttl);
}

void XMulticaster::StartListening()
{
	XMutexLocker locker(this->m_startMutex);

	if (this->IsRunning())
	{
		return;
	}

	// create the socke
	this->m_socketHandle = this->CreateSocket(UDP);

	// bind the socket
	this->BindSocket(this->m_socketHandle, this->m_port, this->GetBindAddress());

	// join the multicast group
	this->JoinMulticastGroup(this->m_socketHandle, this->m_sendAddress.c_str(), this->GetBindAddress());

	// start the listener
	this->Start();
}

void XMulticaster::StopListening()
{
	if (!this->IsRunning())
	{
		return;
	}

	this->Kill();
	::closesocket(this->m_socketHandle);

    this->Wait(10000);
}

const char* XMulticaster::GetBindAddress()
{
	if (this->m_bindAddress.size() == 0)
	{
		return nullptr;
	}
	else
	{
		return this->m_bindAddress.c_str();
	}
}

void XMulticaster::Execution()
{
	char peerAddress[256];

	XByteArray msg;

	while (!this->ShouldDie())
    {
		try
		{
			this->ReceiveFrom(this->m_socketHandle, msg, peerAddress);
		}
		// if there was a problem in receiving the message, then we will just discard it since there
		// is no benefit of knowing that there was an intermittent problem
		catch (...)
		{
			continue;
		}

		if (msg.Size() > 0)
		{
			try
			{
				// send the information via the callback
				this->OnMessageReceived.Execute(msg);
			}
			// if the receive throws an exception, we want to make sure it does not
			// affect our thread
			catch (...)
			{
			}
		}
    }
}