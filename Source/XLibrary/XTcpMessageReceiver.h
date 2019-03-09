/*!
@file	XTcpMessageReceiver.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XTCPMESSAGERECEIVER_H
#define XTCPMESSAGERECEIVER_H

#include "Xylasoft.h"
#include "XThread.h"
#include "XMutex.h"
#include "XEvent.h"
#include "XException.h"
#include "XEventCallback.h"
#include "XSingleEventCallback.h"
#include "XSocketDevice.h"
#include "XTcpMessageReceiverThread.h"
#include "XByteArray.h"
#include <vector>

namespace Xylasoft
{
	/*!
	This provides the ability to receive a message through TCP in a block. It can run up to N threads
	at a time sending/receiving on different sockets.
	*/
	class XTcpMessageReceiver : protected XThread, protected XSocketDevice
	{
	public:
		/*!
		  Constructor for a receiver.
		  @param[in] port The bind port.
		  @param[in] bindAddress The address to bind to, or nullptr for default.
		*/
		XTcpMessageReceiver(int port = 0, const char* bindAddress = nullptr);

		/*!
		  Destructor. Stops listening (if listening) and cleans
		  up before deletion.
		*/
		virtual ~XTcpMessageReceiver();

		/*!
		  Start listening for messagesp.
		*/
		void StartListening();

		/*!
		  Stop listening for messages.
		*/
		void StopListening();

		/*!
		Get the bound port number.
		@returns The bound port number.
		*/
		long GetBindPort();

		/*!
		Get the address information the socket is/will be bound to.
		@returns The IP address string.
		*/
		std::string GetInterfaceAddress();

		/*!
		Event for a message being received.
		*/
		XSingleEventCallback<const XByteArray&> OnMessageReceived;

	protected:
		/*!
		Execution body for the thread.
		*/
		virtual void Execution();

		/*!
		Get the bind address.
		@returns nullptr if not specified, string otherwise.
		*/
		const char* GetBindAddress();

		/*!
		Clean any old connections.
		*/
		void CleanConnections();

	private:
		std::string m_bindAddress;
		int m_port;

		SOCKET m_socketHandle;
		XMutex m_startMutex;
		std::vector<XTcpMessageReceiverThread*> m_receivers;
	};
}

#endif