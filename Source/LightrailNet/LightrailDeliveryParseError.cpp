/*!
@file	LightrailDeliveryParseError.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "StdAfx.h"
#include "LightrailDeliveryParseError.h"

using namespace Xylasoft;

DeliveryParseError::DeliveryParseError(const IDeliveryParseError& error)
{
	m_type = static_cast<ParseErrorType>(static_cast<int>(error.ErrorType()));
	m_route = gcnew String(error.Route()->Name());
	m_sourceapp = gcnew String(error.SourceApplication());
	m_sourcehost = gcnew String(error.SourceHostname());
	m_sourceport = error.SourcePort();
	m_exmessage = gcnew String(error.Message().what());
}

ParseErrorType DeliveryParseError::ErrorType()
{
	return this->m_type;
}

String^ DeliveryParseError::TerminalName()
{
	return this->m_route;
}

String^ DeliveryParseError::SourceApplication()
{
	return this->m_sourceapp;
}

String^ DeliveryParseError::SourceHostname()
{
	return this->m_sourcehost;
}

long DeliveryParseError::SourcePort()
{
	return this->m_sourceport;
}

String^ DeliveryParseError::Message()
{
	return this->m_exmessage;
}