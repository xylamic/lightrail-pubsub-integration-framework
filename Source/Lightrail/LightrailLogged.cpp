/*!
@file	LightrailLogged.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "LightrailLogged.h"

using namespace Xylasoft;

LightrailLogged::LightrailLogged(const wchar_t* logContext)
{
	this->m_logger = XLogger::GetGlobalLogger();
	this->m_context = logContext;
}

LightrailLogged::~LightrailLogged()
{
}

void LightrailLogged::LogMessage(const wchar_t* message, IXLogger::LogLevel level)
{
	this->m_logger->LogMessage(this->m_context.c_str(), message, level);
}

void LightrailLogged::LogMessage(const std::wostringstream& message, IXLogger::LogLevel level)
{
	this->LogMessage(message.str().c_str(), level);
}

void LightrailLogged::LogException(const wchar_t* message, IXLogger::LogLevel level, const std::exception& ex)
{
	this->m_logger->LogException(this->m_context.c_str(), message, level, ex);
}

void LightrailLogged::LogException(const std::wostringstream& message, IXLogger::LogLevel level, const std::exception& ex)
{
	this->LogException(message.str().c_str(), level, ex);
}