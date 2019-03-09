/*!
@file	XSerializer.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XSerializer.h"
#include "XException.h"
#include "XUtilities.h"
#include <sstream>

using namespace Xylasoft;

XSerializer::XSerializer(XByteArray& data)
{
	this->m_isReading = false;
	this->m_dataReader = nullptr;
	this->m_dataWriter = &data;

	this->m_version = XSERIALIZER_VERSION_10;
	*this << static_cast<uint16_t>(this->m_version);
}

XSerializer::XSerializer(const XByteArray* data)
	: m_dataReader(data)
{
	this->m_isReading = true;
	this->m_readerPtr = 0;
	this->m_dataReader = data;
	this->m_dataWriter = nullptr;

	uint16_t version;
	*this >> version;
	this->m_version = version;

	if (version != XSERIALIZER_VERSION_10)
	{
		throw IOException(L"The data version wasn't recognized.");
	}
}

XSerializer& XSerializer::operator<<(uint16_t value)
{
	static long size = sizeof(uint16_t);
	this->ValidateWriteMode();

	// convert to network short
	value = ::htons(value);

	// write the binary of the short
	this->m_dataWriter->Append(reinterpret_cast<const char*>(&value), size);

	return *this;
}

XSerializer& XSerializer::operator<<(uint32_t value)
{
	static long size = sizeof(uint32_t);
	this->ValidateWriteMode();

	// conver to network long
	value = ::htonl(value);

	// write the binary of the long
	this->m_dataWriter->Append(reinterpret_cast<const char*>(&value), size);

	return *this;
}

XSerializer& XSerializer::operator<<(int32_t value)
{
	uint16_t isPositive = value >= 0 ? 1 : 0;
	*this << isPositive;

	value = ::abs(value);
	*this << static_cast<uint32_t>(value);

	return *this;
}

XSerializer& XSerializer::operator<<(const double& value)
{
	this->ValidateWriteMode();

	// convert the double to a string
	std::ostringstream stream;
	stream.precision(10);
	stream << value;
	std::string valuestring = stream.str();

	// write the string
	*this << valuestring.c_str();

	return *this;
}

XSerializer& XSerializer::operator<<(const std::string& value)
{
	this->ValidateWriteMode();

	// write the length of the string
	*this << static_cast<uint32_t>(value.size());

	// write the string
	this->m_dataWriter->Append(value.c_str(), static_cast<long>(value.size()));

	return *this;
}

XSerializer& XSerializer::operator<<(const std::wstring& value)
{
	// convert to UTF-16 bytes
	std::string bytestr = XUtilities::GetUTF16BytesFromString(value);

	// write the string
	*this << bytestr;

	return *this;
}

XSerializer& XSerializer::operator<<(const XByteArray& value)
{
	this->ValidateWriteMode();

	// write the length of the data
	*this << static_cast<uint32_t>(value.Size());

	// write the data
	this->m_dataWriter->Append(value);

	return *this;
}

XSerializer& XSerializer::operator>>(uint16_t& value)
{
	static long size = sizeof(uint16_t);
	this->ValidateReadMode(size);

	::memcpy(reinterpret_cast<char*>(&value), &this->m_dataReader->ConstData()[this->m_readerPtr], size);
	value = ::ntohs(value);

	this->m_readerPtr += size;
	return *this;
}

XSerializer& XSerializer::operator>>(uint32_t& value)
{
	static long size = sizeof(uint32_t);
	this->ValidateReadMode(size);

	::memcpy(reinterpret_cast<char*>(&value), &this->m_dataReader->ConstData()[this->m_readerPtr], size);
	value = ::ntohl(value);

	this->m_readerPtr += size;
	return *this;
}

XSerializer& XSerializer::operator>>(int32_t& value)
{
	// read whether the value as positive
	uint16_t isPositive;
	*this >> isPositive;
	
	// read the absolute value
	uint32_t uvalue;
	*this >> uvalue;
	
	// read the value and apply the appropriate sign
	value = static_cast<int32_t>(uvalue);
	if (!isPositive)
	{
		value = -value;
	}

	return *this;
}

XSerializer& XSerializer::operator>>(double& value)
{
	// read the string
	std::string valuestring;
	*this >> valuestring;

	// convert the string to double
	value = ::atof(valuestring.c_str());

	return *this;
}

XSerializer& XSerializer::operator>>(std::string& value)
{
	// read the size of the string
	uint32_t size;
	*this >> size;

	// read the string
	this->ValidateReadMode(size);
	value.resize(size);

	if (size > 0)
	{
		::memcpy(&value[0], &this->m_dataReader->ConstData()[this->m_readerPtr], size);
		this->m_readerPtr += size;
	}

	return *this;
}

XSerializer& XSerializer::operator>>(std::wstring& value)
{
	// read the string of bytes
	std::string utf16bytes;
	*this >> utf16bytes;

	// convert to wstring
	value = XUtilities::GetStringFromUTF16Bytes(utf16bytes);

	return *this;
}

XSerializer& XSerializer::operator>>(XByteArray& value)
{
	// read the size of the array
	uint32_t size;
	*this >> size;

	// read the array
	this->ValidateReadMode(size);
	value.Resize(size);
	if (size > 0)
	{
		::memcpy(value.Data(), &this->m_dataReader->ConstData()[this->m_readerPtr], size);
	}

	this->m_readerPtr += size;
	return *this;
}

void XSerializer::ValidateReadMode(int size)
{
	if (!this->m_isReading || this->m_dataReader == nullptr)
	{
		throw IOException(L"The serialize must be in read mode in order to perform this action.");
	}

	if (this->m_dataReader->Size() < this->m_readerPtr + size)
	{
		throw IOException(L"There is not enough data left in the stream to read this.");
	}
}

void XSerializer::ValidateWriteMode()
{
	if (this->m_isReading)
	{
		throw IOException(L"The serialize must be in write mode in order to perform this action.");
	}
}