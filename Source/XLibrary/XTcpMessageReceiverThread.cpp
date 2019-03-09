/*!
@file	XTcpMessageReceiverThread.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XTcpMessageReceiverThread.h"

using namespace Xylasoft;

XTcpMessageReceiverThread::XTcpMessageReceiverThread(SOCKET socket, XSingleEventCallback<const XByteArray&>& callback)
	: m_callback(callback)
{
	this->m_socket = socket;
	this->m_completed = false;
	this->m_wasKilled = false;
}

XTcpMessageReceiverThread::~XTcpMessageReceiverThread()
{
}

SOCKET XTcpMessageReceiverThread::SocketHandle()
{
	return this->m_socket;
}

bool XTcpMessageReceiverThread::CompletedExecution()
{
	return this->m_completed;
}

void XTcpMessageReceiverThread::KillConnection()
{
	this->CloseSocket(this->m_socket);
	this->m_wasKilled = true;
}

void XTcpMessageReceiverThread::Execution()
{
	char msgBuffer[STREAM_BUFFER_SIZE];

	XByteArray message;
	long msgSize = -1;

	try
	{
		// continue while the message size has not yet been set or the amount of data received is
		// less than the specified message size
		while (msgSize == -1 || message.Size() < msgSize)
		{
			int packetSize = this->Receive(this->m_socket, msgBuffer, STREAM_BUFFER_SIZE);

			if (msgSize < 0)
			{
				if (packetSize < sizeof(u_long))
				{
					throw CommunicationException(L"Packet size was too small to understand.");
				}

				u_long* netlong = (u_long*)msgBuffer;

				// parse the total message size
				msgSize = ntohl(*netlong);

				// copy the rest of the packet
				message.Append(msgBuffer + sizeof(u_long), packetSize - sizeof(u_long));
			}
			else
			{
				message.Append(msgBuffer, packetSize);
			}
		}

		// send acknowledgement
		this->SendAck(this->m_socket);

		try
		{
			this->m_callback.Execute(message);
		}
		// if the event receiver throws an exception, we don't care, except for debugging
#ifdef _DEBUG
		catch (std::exception& ex)
		{
			std::cout << "TCP Receiver event failed: " << ex.what() << std::endl;
			assert(false);
		}
		catch (...)
		{
			assert(false);
		}
#else
		catch (...)
		{
		}
#endif
	}
	// if there was an error in receiving, then close the socket and move on
	catch (...)
	{
	}

	// mark that this thread has executed
	this->m_completed = true;

	// if this thread was not explicitly killed, then close the socket
	if (!this->m_wasKilled)
	{
		this->CloseSocket(this->m_socket);
	}
}
