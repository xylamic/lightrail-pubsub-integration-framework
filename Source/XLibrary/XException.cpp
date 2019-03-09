/*!
@file	XException.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XException.h"
#include "XUtilities.h"

using namespace Xylasoft;

XException::XException(const wchar_t* message)
	: std::exception()
{
	this->m_message = message;
	this->m_smessage = XUtilities::GetStringFromWString(this->m_message);
}

XException::XException(const XException& other)
{
	this->m_message = other.m_message;
}

XException& XException::operator=(const XException& other)
{
	this->m_message = other.m_message;
	return *this;
}

XException::~XException()
{
}

const char* XException::what() const
{
	return this->m_smessage.c_str();
}

const wchar_t* XException::wwhat() const
{
	return this->m_message.c_str();
}

LicenseException::LicenseException(const wchar_t* message)
	: XException(message)
{ }

IOException::IOException(const wchar_t* message)
	: XException(message)
{ }

FileException::FileException(const wchar_t* message)
	: IOException(message)
{ }

EncryptionException::EncryptionException(const wchar_t* message)
	: IOException(message)
{ }

TimeoutException::TimeoutException(const wchar_t* message)
	: IOException(message)
{ }

NetworkException::NetworkException(const wchar_t* message)
	: XException(message)
{ }

ResolutionException::ResolutionException(const wchar_t* message)
	: NetworkException(message)
{ }

BindException::BindException(const wchar_t* message)
	: NetworkException(message)
{ }

CommunicationException::CommunicationException(const wchar_t* message)
	: NetworkException(message)
{ }

AddressException::AddressException(const wchar_t* message)
	: NetworkException(message)
{ }

DataException::DataException(const wchar_t* message)
	: XException(message)
{ }

EncodingException::EncodingException(const wchar_t* message)
	: DataException(message)
{ }

ArgumentException::ArgumentException(const wchar_t* message)
	: DataException(message)
{ }