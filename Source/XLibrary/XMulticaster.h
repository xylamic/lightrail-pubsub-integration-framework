/*!
@file	XMulticaster.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XMULTICASTER_H
#define XMULTICASTER_H

#include "Xylasoft.h"
#include "XException.h"
#include "XThread.h"
#include "XMutex.h"
#include "XEvent.h"
#include "XEventCallback.h"
#include "XSingleEventCallback.h"
#include "XSocketDevice.h"
#include "XByteArray.h"

namespace Xylasoft
{
	/*!
	  Class for sending and receiving multicast messages.
	*/
	class XMulticaster : protected XThread, protected XSocketDevice
	{
	public:
		/*!
		Constructor for a XMulticaster.
		@param[in] sendAddress The multicast address.
		@param[in] port The multicast port.
		@param[in] bindAddress The address to bind to, or nullptr for default.
		@param[in] ttyl The time to live inside the network, default is 1 which is the subnetwork.
		*/
		XMulticaster(const char* sendAddress, int port, const char* bindAddress = nullptr, int ttl = 1);

		/*!
		Destructor. Stops listening (if listening) and cleans
		up before deletion.
		*/
		virtual ~XMulticaster();

		/*
		Send a multicast message.
		@param[in] message The message to send.
		*/
		void Send(const XByteArray& message);

		/*!
		Start listening to the multicast group.
		*/
		void StartListening();

		/*!
		Stop listening to the multicast group.
		*/
		void StopListening();

		/*!
		Get the bind address.
		@returns nullptr if not specified, string otherwise.
		*/
		const char* GetBindAddress();

		/*!
		Event triggered when a message is received.
		*/
		XSingleEventCallback<const XByteArray&> OnMessageReceived;

	protected:
		/*!
		Execution body for the thread.
		*/
		virtual void Execution();

	private:
		std::string m_sendAddress;
		std::string m_bindAddress;
		int m_port;
		int m_ttl;

		SOCKET m_socketHandle;
		XMutex m_startMutex;
	};
}
#endif // XMulticaster_H
