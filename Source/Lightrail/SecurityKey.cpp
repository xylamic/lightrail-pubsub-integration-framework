/*!
@file	SecurityKey.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "SecurityKey.h"
#include <XException.h>

using namespace Xylasoft;

SecurityKey::SecurityKey(const wchar_t* name, Security::SecurityType type)
{
	if (name == nullptr || name[0] == '\0')
	{
		throw ArgumentException(L"The name was empty.");
	}

	this->m_name = name;
	this->m_type = type;
}

SecurityKey::~SecurityKey()
{
}

const wchar_t* SecurityKey::Name() const
{
	return this->m_name.c_str();
}

Security::SecurityType SecurityKey::Type() const
{
	return this->m_type;
}