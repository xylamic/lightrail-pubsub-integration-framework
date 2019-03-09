/*!
@file	LightrailDeliveryParseError.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

#include <IDeliveryParseError.h>

using namespace System;

namespace Xylasoft
{
	public enum class ParseErrorType
	{
		None = 0,

		Corruption = 1,
		
		Decryption = 2,

		SecurityMismatch = 3
	};

	public ref class DeliveryParseError
	{
	public:
		DeliveryParseError(const IDeliveryParseError& error);

		ParseErrorType ErrorType();

		String^ TerminalName();

		String^ SourceApplication();

		String^ SourceHostname();

		long SourcePort();

		String^ Message();

	private:
		ParseErrorType m_type;
		String^ m_route;
		String^ m_sourceapp;
		String^ m_sourcehost;
		long m_sourceport;
		String^ m_exmessage;
	};
}