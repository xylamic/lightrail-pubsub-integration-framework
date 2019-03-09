/*!
@file	XTcpMessageReceiverThread.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XTCPMESSAGERECEIVERTHREAD_H
#define XTCPMESSAGERECEIVERTHREAD_H

#include "XThread.h"
#include "XSingleEventCallback.h"
#include "XSocketDevice.h"
#include "XByteArray.h"

namespace Xylasoft
{
	/*!
	This provides an individual message receiver on a specific socket. This accepts a message and 
	returns it to the creator.
	*/
	class XTcpMessageReceiverThread : public XThread, public XSocketDevice
	{
	public:
		/*!
		Construct the receiver thread.
		@param[in] socket The socket to use to receive the message.
		@param[in] callback The callback for when the message is accepted.
		*/
		XTcpMessageReceiverThread(SOCKET socket, XSingleEventCallback<const XByteArray&>& callback);

		/*!
		Destroy the receiver thread.
		*/
		virtual ~XTcpMessageReceiverThread();

		/*!
		Get the receiver socket handle.
		*/
		SOCKET SocketHandle();

		/*!
		Get whether this thread has completed execution.
		*/
		bool CompletedExecution();

		/*!
		Kill the execution of the receiver thread and close any open connection.
		*/
		void KillConnection();

	protected:
		virtual void Execution();

	private:
		SOCKET m_socket;
		volatile bool m_completed;
		volatile bool m_wasKilled;
		XSingleEventCallback<const XByteArray&>& m_callback;
	};
}

#endif