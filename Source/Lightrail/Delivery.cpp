/*!
@file	Delivery.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Delivery.h"

#define FORMAT_V1 0

using namespace Xylasoft;

Delivery::Delivery()
{
}

Delivery::Delivery(const Delivery& other)
{
	this->m_encoding = other.m_encoding;
	this->m_data = other.m_data;
}

Delivery::Delivery(XSerializer& serializer)
{
	uint16_t version;
	serializer >> version;

	if (version != FORMAT_V1)
	{
		throw DataException(L"The delivery format version was unrecognized.");
	}

	int32_t encoding;
	serializer >> encoding;
	serializer >> this->m_data;
	this->m_encoding = static_cast<Encoding::EncodingType>(encoding);

	uint32_t labelNum;
	serializer >> labelNum;
	for (size_t labelIndex = 0; labelIndex < labelNum; labelIndex++)
	{
		std::wstring name, value;
		serializer >> name >> value;

		this->m_labels[name] = value;
	}
}

Delivery::~Delivery()
{
}

void Delivery::Serialize(XSerializer& serializer) const
{
	serializer << static_cast<uint16_t>(FORMAT_V1);

	serializer << static_cast<int32_t>(this->m_encoding);
	serializer << this->m_data;

	serializer << static_cast<uint32_t>(this->m_labels.size());
	for (auto labelit = this->m_labels.cbegin(); labelit != this->m_labels.cend(); labelit++)
	{
		serializer << labelit->first << labelit->second;
	}
}

std::wstring Delivery::MessageString() const
{
	switch (this->m_encoding)
	{
	case Encoding::ASCII:
		return XUtilities::GetWStringFromString(std::string(this->m_data.ConstData(), this->m_data.Size()));
		break;
	case Encoding::UTF8:
		return XUtilities::GetStringFromUTF8Bytes(std::string(this->m_data.ConstData(), this->m_data.Size()));
		break;
	case Encoding::UTF16:
		return XUtilities::GetStringFromUTF16Bytes(std::string(this->m_data.ConstData(), this->m_data.Size()));
		break;
	default:
		throw EncodingException(L"A string can only be generated from ASCII, UTF-8, and UTF-16 encodings.");
	}
}

void Delivery::SetMessageString(const std::wstring& message)
{
	std::string stdmsg = XUtilities::GetUTF8BytesFromString(message);
	this->SetMessage(stdmsg.c_str(), static_cast<long>(stdmsg.size()), Encoding::UTF8);
}

void Delivery::SetMessage(const char* data, long dataSize, Encoding::EncodingType encoding)
{
	this->m_data = XByteArray(data, dataSize);

	this->m_encoding = encoding;
}

Encoding::EncodingType Delivery::Encoding() const
{
	return this->m_encoding;
}

const char* Delivery::Message() const
{
	return this->m_data.ConstData();
}

long Delivery::MessageSize() const
{
	return this->m_data.Size();
}

void Delivery::EncryptMessge(const SecurityKey* securitykey)
{
	m_data = securitykey->Encrypt(this->m_data);
}

void Delivery::DecryptMessage(const SecurityKey* securitykey)
{
	m_data = securitykey->Decrypt(this->m_data);
}

void Delivery::SetLabel(const wchar_t* name, const wchar_t* value)
{
	if (value == nullptr)
	{
		this->m_labels.erase(name);
	}
	else
	{
		this->m_labels[name] = value;
	}
}

const wchar_t* Delivery::Label(const wchar_t* name) const
{
	auto labelit = this->m_labels.find(name);
	if (labelit == this->m_labels.cend())
	{
		return nullptr;
	}
	else
	{
		return labelit->second.c_str();
	}
}

long Delivery::GetNumberOfLabels() const
{
	return this->m_labels.size();
}

const wchar_t* Delivery::GetLabelName(long index) const
{
	if (index < 0 || index >= static_cast<long>(this->m_labels.size()))
	{
		return NULL;
	}
	else
	{
		auto labelit = this->m_labels.cbegin();
		for (long i = 0; i < index; i++)
		{
			labelit++;
		}

		return labelit->first.c_str();
	}
}