/*!
@file	XTcpMessageSender.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XTCPMESSAGESENDER_H
#define XTCPMESSAGESENDER_H

#include "Xylasoft.h"
#include "XSocketDevice.h"
#include "XByteArray.h"

namespace Xylasoft
{
	/*!
	Facility for sending a single, independent messages to a TCP message receiver.
	*/
	class XTcpMessageSender : public XSocketDevice
	{
	public:
		/*!
		Construct a message sender.
		@param[in] sendAddress The address to send messages to.
		@param[in] port The port of the address to send messages to.
		@param[in] version The version of IP address specified.
		*/
		XTcpMessageSender(const char* sendAddress, int port, IPVersion version = V4);

		/*!
		Destructor.
		*/
		virtual ~XTcpMessageSender();

		/*!
		Send a message to the host.
		@param[in] msg The message to send.
		*/
		void Send(const XByteArray& msg);

	private:
		std::string m_sendAddress;
		int m_port;
		IPVersion m_version;
	};
}

#endif