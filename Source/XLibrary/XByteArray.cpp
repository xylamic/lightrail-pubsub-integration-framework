/*!
@file	XByteArray.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XByteArray.h"
#include "XUtilities.h"
#include <sstream>

using namespace Xylasoft;

XByteArray::XByteArray()
{
}

XByteArray::XByteArray(long size, char value)
{
	this->m_buffer.resize(size);
	::memset(&this->m_buffer[0], value, size);
}

XByteArray::XByteArray(const char* data, long dataSize)
{
	m_buffer.resize(dataSize);

	if (dataSize > 0)
	{
		::memcpy(&this->m_buffer[0], data, dataSize);
	}
}

void XByteArray::Append(const XByteArray& data)
{
	this->Append(data.ConstData(), data.Size());
}

void XByteArray::Append(const char* data, long dataSize)
{
	if (dataSize > 0)
	{
		long currentSize = static_cast<long>(this->m_buffer.size());

		this->m_buffer.resize(currentSize + dataSize);
		::memcpy(&this->m_buffer[currentSize], data, dataSize);
	}
}

void XByteArray::Append(char c)
{
	this->m_buffer.push_back(c);
}

void XByteArray::Clear()
{
	this->m_buffer.clear();
}

void XByteArray::Resize(long size, char value)
{
	long initialSize = this->Size();
	this->m_buffer.resize(size);

	if (this->Size() > initialSize)
	{
		::memset(&this->m_buffer[initialSize], value, this->Size() - initialSize);
	}
}

const char* XByteArray::ConstData() const
{
	return &this->m_buffer[0];
}

char* XByteArray::Data()
{
	return &this->m_buffer[0];
}

long XByteArray::Size() const
{
	return static_cast<long>(this->m_buffer.size());
}

char* XByteArray::operator[](long index)
{
	return &this->m_buffer[index];
}

const char* XByteArray::operator[](long index) const
{
	return &this->m_buffer[index];
}