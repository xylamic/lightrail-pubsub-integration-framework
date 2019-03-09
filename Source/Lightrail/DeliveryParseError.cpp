/*!
@file	DeliveryParseError.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "DeliveryParseError.h"

using namespace Xylasoft;

DeliveryParseError::DeliveryParseError(const ITerminal* terminal, const std::wstring& sourceapp, const std::string& sourcehost,
	long sourceport, ParseErrorType error, std::exception& message)
	: m_terminal(terminal), m_error(error), m_message(message)
{
	this->m_sourceapp = sourceapp;
	this->m_sourcehost = sourcehost;
	this->m_sourceport = sourceport;
}

const ITerminal* DeliveryParseError::Route() const
{
	return this->m_terminal;
}

DeliveryParseError::ParseErrorType DeliveryParseError::ErrorType() const
{
	return this->m_error;
}

const std::exception& DeliveryParseError::Message() const
{
	return this->m_message;
}

const wchar_t* DeliveryParseError::SourceApplication() const
{
	return this->m_sourceapp.c_str();
}

const char* DeliveryParseError::SourceHostname() const
{
	return this->m_sourcehost.c_str();
}

long DeliveryParseError::SourcePort() const
{
	return this->m_sourceport;
}