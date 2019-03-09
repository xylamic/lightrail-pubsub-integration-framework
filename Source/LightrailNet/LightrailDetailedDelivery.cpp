/*!
@file	LightrailDetailedDelivery.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "StdAfx.h"
#include "LightrailDetailedDelivery.h"

using namespace Xylasoft;

DetailedDelivery::DetailedDelivery(const Xylasoft::IDetailedDelivery* delivery, array<String^>^ receiptTerminals)
{
	DeliveryEncoding encoding;
	switch (delivery->Encoding())
	{
	case Xylasoft::Encoding::ASCII:
		encoding = DeliveryEncoding::Ascii;
		break;
	case Xylasoft::Encoding::UTF8:
		encoding = DeliveryEncoding::Utf8;
		break;
	case Xylasoft::Encoding::UTF16:
		encoding = DeliveryEncoding::Utf16;
		break;
	case Xylasoft::Encoding::UTF32:
		encoding = DeliveryEncoding::Utf32;
		break;
	default:
		encoding = DeliveryEncoding::None;
		break;
	}

	m_delivery = gcnew Delivery(ConvertNativeBytesToByteArray(delivery->Message(),
		delivery->MessageSize()), encoding);
	for (long i = 0; i < delivery->GetNumberOfLabels(); i++)
	{
		const wchar_t* name = delivery->GetLabelName(i);
		m_delivery->SetLabel(gcnew String(name), gcnew String(delivery->Label(name)));
	}

	m_sourceApplication = gcnew String(delivery->SourceApplication());
	m_sourceTerminal = gcnew String(delivery->SourceTerminal().Name());
	m_sourceHost = gcnew String(delivery->SourceHost());
	m_sourcePort = delivery->SourcePort();
	m_sendTime = delivery->TimeUtc();
	m_receiptTerminalNames = receiptTerminals;
}

array<Byte>^ DetailedDelivery::Message()
{
	return this->m_delivery->Message();
}

DeliveryEncoding DetailedDelivery::Encoding()
{
	return this->m_delivery->Encoding();
}

String^ DetailedDelivery::MessageString()
{
	return this->m_delivery->MessageString();
}

String^ DetailedDelivery::Label(String^ name)
{
	return this->m_delivery->Label(name);
}

array<String^>^ DetailedDelivery::GetLabelNameList()
{
	return this->m_delivery->GetLabelNameList();
}

String^ DetailedDelivery::SourceTerminalName()
{
	return this->m_sourceTerminal;
}

array<String^>^ DetailedDelivery::ReceiptTerminalNames()
{
	return this->m_receiptTerminalNames;
}

/*array<String^>^ ReceivingTerminals()
{
	return gcnew String(this->m_detaileddelivery->Terminals().Name());
}*/

String^ DetailedDelivery::SourceHost()
{
	return this->m_sourceHost;
}

String^ DetailedDelivery::SourceApplication()
{
	return this->m_sourceApplication;
}

long DetailedDelivery::SourcePort()
{
	return this->m_sourcePort;
}

DateTime DetailedDelivery::SendTimeUtc()
{
	DateTime^ tutc = gcnew DateTime(1970, 1, 1, 0, 0, 0);
	tutc = tutc->AddSeconds(static_cast<double>(this->m_sendTime));

	return *tutc;
}

array<Byte>^ DetailedDelivery::ConvertNativeBytesToByteArray(const char* binary, long size)
{
	System::Collections::Generic::List<Byte>^ byteList = 
		gcnew System::Collections::Generic::List<Byte>();

	for (long i = 0; i < size; i++)
	{
		byteList->Add(binary[i]);
	}

	return byteList->ToArray();
}