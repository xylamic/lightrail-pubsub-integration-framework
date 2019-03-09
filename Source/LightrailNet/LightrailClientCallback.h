/*!
@file	LightrailClientCallback.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

#include "ILightrail.h"
#include "LightrailDelivery.h"
#include "LightrailDetailedDelivery.h"

namespace Xylasoft
{
	/* function pointer declaration for managed publish delegate */
	typedef int (__stdcall *NATIVEPUB)(ITerminalDelivery&);

	/* function pointer declaraction for managed request delegate */
	typedef int (__stdcall *NATIVEREQ)(ITerminalReturnDelivery&);

	/* function pointer declaraction for managed parse error delegate */
	typedef int (__stdcall *NATIVEPARERR)(const IDeliveryParseError&);

	/*!
	This class is used for receiving native callbacks and retransmitting them through managed delegate function
	pointers to a managed class which can them convey them to a managed application.
	*/
	class LightrailClientCallback
	{
	public:
		/*!
		Constructor for callback.
		@param[in] instance The instance of the lightrail to bind to.
		@param[in] nativePublishDelegate The publication received function pointer.
		@param[in] nativeRequestDelegate The request received function pointer.
		@param[in] nativeParseErrorDelegate The parse error function pointer.
		*/
		LightrailClientCallback(ILightrail* instance, NATIVEPUB nativePublishDelegate, 
			NATIVEREQ nativeRequestDelegate, NATIVEPARERR nativeParseErrorDelegate);

	protected:
		/*!
		A native publish was received. Pass it to the managed delegate.
		@param[in] delivery The message received.
		*/
		void OnTerminalPublishReceived(ITerminalDelivery& delivery);

		/*!
		A native request was received. Pass it to the managed delegate.
		@param[in] delivery The message request received.
		*/
		void OnTerminalRequestReceived(ITerminalReturnDelivery& delivery);

		/*!
		A native parse error was received. Pass it to the managed delegate.
		@param[in] error The delivery parse error that occurred.
		*/
		void OnDeliveryParseError(const IDeliveryParseError& error);

	private:
		NATIVEPUB m_nativePublish;
		NATIVEREQ m_nativeRequest;
		NATIVEPARERR m_nativeParseError;
	};
}