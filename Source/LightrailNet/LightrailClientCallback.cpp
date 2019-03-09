/*!
@file	LightrailClientCallback.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "StdAfx.h"
#include "LightrailClientCallback.h"
#include <vcclr.h>

using namespace Xylasoft;

LightrailClientCallback::LightrailClientCallback(ILightrail* instance, NATIVEPUB nativePublishDelegate, 
	NATIVEREQ nativeRequestDelegate, NATIVEPARERR nativeParseErrorDelegate)
{
	m_nativePublish = nativePublishDelegate;
	m_nativeRequest = nativeRequestDelegate;
	m_nativeParseError = nativeParseErrorDelegate;

	instance->OnPublicationReceived.SetCallback(new XEventCallback<LightrailClientCallback, ITerminalDelivery&>(
		this, &LightrailClientCallback::OnTerminalPublishReceived));
	instance->OnRequestReceived.SetCallback(new XEventCallback<LightrailClientCallback, ITerminalReturnDelivery&>(
		this, &LightrailClientCallback::OnTerminalRequestReceived));
	instance->OnDeliveryParseError.SetCallback(new XEventCallback<LightrailClientCallback, const IDeliveryParseError&>(
		this, &LightrailClientCallback::OnDeliveryParseError));
}

void LightrailClientCallback::OnTerminalPublishReceived(ITerminalDelivery& delivery)
{
	m_nativePublish(delivery);
}

void LightrailClientCallback::OnTerminalRequestReceived(ITerminalReturnDelivery& delivery)
{
	try
	{
		m_nativeRequest(delivery);
	}
	catch (Exception^ ex)
	{
		pin_ptr<const wchar_t> wname = PtrToStringChars(ex->Message);
		throw DataException(wname);
	}
}

void LightrailClientCallback::OnDeliveryParseError(const IDeliveryParseError& error)
{
	m_nativeParseError(error);
}