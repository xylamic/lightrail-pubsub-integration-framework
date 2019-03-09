/*!
@file	LightrailDelivery.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "StdAfx.h"
#include "LightrailDelivery.h"

using namespace Xylasoft;

Delivery::Delivery(String^ message)
{
	m_data = System::Text::Encoding::UTF8->GetBytes(message);
	m_encoding = DeliveryEncoding::Utf8;

	m_labels = gcnew Dictionary<String^, String^>();
}

Delivery::Delivery(array<Byte>^ message, DeliveryEncoding encoding)
{
	m_data = message;
	m_encoding = encoding;

	m_labels = gcnew Dictionary<String^, String^>();
}

DeliveryEncoding Delivery::Encoding()
{
	return this->m_encoding;
}

array<Byte>^ Delivery::Message()
{
	return this->m_data;
}

String^ Delivery::MessageString()
{
	switch (m_encoding)
	{
	case DeliveryEncoding::Ascii:
		return System::Text::Encoding::ASCII->GetString(m_data);
	case DeliveryEncoding::Utf8:
		return System::Text::Encoding::UTF8->GetString(m_data);
	case DeliveryEncoding::Utf16:
		return System::Text::Encoding::BigEndianUnicode->GetString(m_data);
	case DeliveryEncoding::Utf32:
		return System::Text::Encoding::UTF32->GetString(m_data);
	default:
		return System::Text::Encoding::UTF8->GetString(m_data);
	}
}

void Delivery::SetLabel(String^ name, String^ value)
{
	if (String::IsNullOrEmpty(value))
	{
		m_labels->Remove(name);
	}
	else if (m_labels->ContainsKey(name))
	{
		m_labels[name] = value;
	}
	else
	{
		m_labels->Add(name, value);
	}
}

String^ Delivery::Label(String^ name)
{
	if (m_labels->ContainsKey(name))
	{
		return m_labels[name];
	}
	else
	{
		return nullptr;
	}
}

array<String^>^ Delivery::GetLabelNameList()
{
	List<String^>^ list = gcnew List<String^>(m_labels->Keys);

	return list->ToArray();
}