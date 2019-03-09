/*!
@file	XTcpMessageReceiver.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XTcpMessageReceiver.h"

using namespace Xylasoft;

#define MAXTHREADS 5

XTcpMessageReceiver::XTcpMessageReceiver(int port, const char* bindAddress)
	: XSocketDevice()
{
	if (bindAddress == nullptr)
	{
		this->m_bindAddress = "";
	}
	else
	{
		this->m_bindAddress = bindAddress;
	}

    this->m_port = port;
}

XTcpMessageReceiver::~XTcpMessageReceiver()
{
    this->StopListening();
	if (this->IsRunning())
    {
		this->Wait(10000);
    }
}

void XTcpMessageReceiver::StartListening()
{
	XMutexLocker locker(this->m_startMutex);

	if (this->IsRunning())
	{
		return;
	}

	// create the socke
	this->m_socketHandle = this->CreateSocket(TCP);

	// bind the socket
	this->BindSocket(this->m_socketHandle, this->m_port, this->GetBindAddress());

	// start listening
	this->Listen(this->m_socketHandle);

	// start the listener thread
	this->Start();
}

std::string XTcpMessageReceiver::GetInterfaceAddress()
{
	const char* addr = this->GetBindAddress();
	if (addr == nullptr)
	{
		return "0.0.0.0";
	}
	else
	{
		return addr;
	}
}

void XTcpMessageReceiver::StopListening()
{
	if (!this->IsRunning())
	{
		return;
	}

	// kill the local receiver thread
	this->Kill();
	::closesocket(this->m_socketHandle);

	// clean any existing old connections
	this->CleanConnections();

	// stop any remaining connections
	while (this->m_receivers.size() > 0)
	{
		XTcpMessageReceiverThread* thread = this->m_receivers[0];
		if (thread->IsRunning() || !thread->CompletedExecution())
		{
			thread->KillConnection();
		}
		thread->Wait();

		delete thread;
		this->m_receivers.erase(this->m_receivers.begin());
	}

    this->Wait(10000);
}

const char* XTcpMessageReceiver::GetBindAddress()
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

long XTcpMessageReceiver::GetBindPort()
{
	std::string address;
	int port;

	this->GetBoundAddressForSocket(this->m_socketHandle, address, port);

	return port;
}

void XTcpMessageReceiver::CleanConnections()
{
	size_t index = 0;
	while (index < this->m_receivers.size())
	{
		XTcpMessageReceiverThread* thread = this->m_receivers[index];
		if (thread->CompletedExecution() && !thread->IsRunning())
		{
			delete thread;
			this->m_receivers.erase(this->m_receivers.begin() + index);
		}
		else
		{
			index++;
		}
	}
}

void XTcpMessageReceiver::Execution()
{
	int peerPort;
	std::string peerAddress;

	time_t lastCheck = 0;
	int secUpdate = 1;

	while (!this->ShouldDie())
    {
		try
		{
			SOCKET newsocket = this->AcceptConnection(this->m_socketHandle, peerAddress, peerPort);

			XTcpMessageReceiverThread* thread = new XTcpMessageReceiverThread(newsocket, this->OnMessageReceived);
			thread->Start();

			this->m_receivers.push_back(thread);

			if (this->m_receivers.size() >= MAXTHREADS)
			{
				while (this->m_receivers.size() >= MAXTHREADS)
				{
					this->Sleep(100);
					this->CleanConnections();
				}

				time_t currentT;
				::time(&currentT);
				lastCheck = currentT;
			}
			else
			{
				time_t currentT;
				::time(&currentT);

				if (lastCheck < currentT - secUpdate)
				{
					this->CleanConnections();
					lastCheck = currentT;
				}
			}
		}
		// if something occurs during a particular run, make sure it doesnt not stop the receiver
#ifdef _DEBUG
		catch (std::exception& ex)
		{
			std::cout << "XTcpMessageReceiver threading error: " << ex.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "XTcpMessageReceiver threading error: Unknown." << std::endl;
		}
#else
		catch (...)
		{
		}
#endif
    }
}