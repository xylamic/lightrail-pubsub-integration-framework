/*!
@file	XTcpMessageSender.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XTcpMessageSender.h"

using namespace Xylasoft;

XTcpMessageSender::XTcpMessageSender(const char* sendAddress, int port, IPVersion version)
{
	this->m_sendAddress = sendAddress;
	this->m_port = port;
	this->m_version = version;
}

XTcpMessageSender::~XTcpMessageSender()
{
}

void XTcpMessageSender::Send(const XByteArray& msg)
{
	// create the socket
	SOCKET socket = this->CreateSocket(XSocketDevice::TCP);

	// build the message for sending
	u_long messageSizeN = htonl(msg.Size());

	long msgSize = msg.Size() + sizeof(u_long);
	char* msgData = new char[msgSize];

	// copy the message size
	::memcpy(msgData, &messageSizeN, sizeof(u_long));

	// copy the content of the message
	::memcpy(msgData + sizeof(u_long), msg.ConstData(), msg.Size());

	try
	{
		// connect to the server
		this->Connect(socket, this->m_sendAddress.c_str(), this->m_port);

		// send all of the message
		this->SendAll(socket, msgData, msgSize);

		// receive the acknowledgement
		this->ReceiveAck(socket);

		// close the socket
		this->CloseSocket(socket);

		// clear the memory buffer
		delete[] msgData;
	}
	catch (...)
	{
		// make sure the socket gets closed and buffer deleted
		delete[] msgData;
		this->CloseSocket(socket);

		// rethrow the exception
		throw;
	}
}