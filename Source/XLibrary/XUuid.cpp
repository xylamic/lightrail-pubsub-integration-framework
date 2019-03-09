/*!
@file	XUuid.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XUuid.h"
#include "XUtilities.h"
#include "XException.h"
#include <sstream>
#include <iomanip>

using namespace Xylasoft;

XUuid::XUuid(std::string uuidstr)
{
	if (uuidstr.size() != 36)
	{
		throw ArgumentException(L"The size of the argument was incorrect.");
	}

	std::string uuidstrnoh;
	for (size_t i = 0; i < uuidstr.size(); i++)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
		{
			if (uuidstr[i] != '-')
			{
				throw ArgumentException(L"The format was invalid.");
			}
		}
		else
		{
			if (!(uuidstr[i] >= '0' && uuidstr[i] <= '9') &&
				!(uuidstr[i] >= 'a' && uuidstr[i] <= 'f') &&
				!(uuidstr[i] >= 'A' && uuidstr[i] <= 'F'))
			{
				throw ArgumentException(L"The format was invalid.");
			}

			uuidstrnoh.push_back(uuidstr[i]);
		}
	}

	std::vector<char> bytes;
	XUtilities::ConvertHexStringToByte(uuidstrnoh, bytes);

	this->m_uuid = XByteArray(&bytes[0], 16);
}

XUuid::XUuid(const char* uuid)
	: m_uuid(uuid, 16)
{
}

XUuid::XUuid(const XUuid& xuuid)
{
	this->m_uuid = xuuid.m_uuid;
}

bool XUuid::operator<(const XUuid& xuuid) const
{
	if (this->ToString().compare(xuuid.ToString()) < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XUuid::operator==(const XUuid& xuuid) const
{
	for (int i = 0; i < this->m_uuid.Size(); i++)
	{
		if (this->m_uuid.ConstData()[i] != xuuid.m_uuid.ConstData()[i])
		{
			return false;
		}
	}

	return true;
}

XUuid& XUuid::operator=(const XUuid& xuuid)
{
	this->m_uuid = xuuid.m_uuid;

	return *this;
}

std::wstring XUuid::ToWString() const
{
	return XUtilities::GetWStringFromString(this->ToString());
}

std::string XUuid::ToString() const
{
	std::ostringstream stream;

	stream.setf(std::ios_base::hex, std::ios::basefield);
	stream.setf(std::ios_base::uppercase);
	stream.fill('0'); 

	for (int i = 0; i < 16; i++)
	{
		if (i == 4 || i == 6 || i == 8 || i == 10)
		{
			stream << "-";
		}

		unsigned short upper = this->m_uuid.ConstData()[i] & 0xF0;
		upper = upper >> 4;

		unsigned short lower = this->m_uuid.ConstData()[i] & 0x0F;

		stream << upper << lower;
	}

	return stream.str();
}

bool XUuid::IsEmpty() const
{
	for (int i = 0; i < this->m_uuid.Size(); i++)
	{
		if (this->m_uuid.ConstData()[i] > 0x00)
		{
			return false;
		}
	}

	return true;
}

XUuid XUuid::CreateEmpty()
{
	// create a null byte set
	char buffer[16];
	::memset(buffer, 0x00, 16);

	return XUuid(buffer);
}

XUuid XUuid::CreateNew()
{
	// generate the string
	char buffer[16];
	for (int i = 0; i < 16; i++)
	{
		// create the top of the byte
		char highhalf = XUtilities::GenerateRandomNumber() % 16;

		// if this is byte 7, the high half must be 4
		if (i == 6)
		{
			highhalf = 4;
		}
		// else if this is byte 9, the high half must be
		// in the range of 8 to B
		else if (i == 8)
		{
			highhalf = XUtilities::GenerateRandomNumber() % 4 + 8;
		}

		// shift the high half four bits
		highhalf = highhalf << 4;

		// generate the low half
		char lowhalf = XUtilities::GenerateRandomNumber() % 16;

		// add the halves together to get the whole byte
		char wholebyte = highhalf | lowhalf;

		// apply the byte
		buffer[i] = wholebyte;
	}

	return XUuid(buffer);
}