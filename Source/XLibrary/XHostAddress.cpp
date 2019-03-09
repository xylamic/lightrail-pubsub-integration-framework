/*!
@file	XHostAddress.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XHostAddress.h"

using namespace Xylasoft;

XHostAddress::XHostAddress()
{
	this->m_isEmpty = true;
}

XHostAddress::XHostAddress(const char* address, AddressType type)
{
	this->m_address = address;
	this->m_type = type;
	this->m_isEmpty = false;
}

const char* XHostAddress::Address()
{
	return this->m_address.c_str();
}

XHostAddress::AddressType XHostAddress::Type()
{
	return this->m_type;
}

bool XHostAddress::IsEmpty()
{
	return this->m_isEmpty;
}