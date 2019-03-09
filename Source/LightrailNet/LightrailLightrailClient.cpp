/*!
@file	LightrailLightrailClient.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "stdafx.h"
#include "LightrailLightrailClient.h"

using namespace Xylasoft;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

LightrailClient::LightrailClient(String^ name, String^ description)
{
#ifdef _WIN32
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(name);
	pin_ptr<const wchar_t> wdesc = PtrToStringChars(description);

	m_client = Xylasoft::Lightrail::GetLightrailInstance(wname, wdesc);

	// create the publish callback
	m_nativePublish = gcnew NativePublishReceived(this, &LightrailClient::OnTerminalPublishReceived);
	m_nativePublishHandle = GCHandle::Alloc(m_nativePublish);
	IntPtr np = Marshal::GetFunctionPointerForDelegate(m_nativePublish);
	NATIVEPUB nativePubFunc = static_cast<NATIVEPUB>(np.ToPointer());

	// create the request callback
	m_nativeRequest = gcnew NativeRequestReceived(this, &LightrailClient::OnTerminalRequestReceived);
	m_nativeRequestHandle = GCHandle::Alloc(m_nativeRequest);
	np = Marshal::GetFunctionPointerForDelegate(m_nativeRequest);
	NATIVEREQ nativeReqFunc = static_cast<NATIVEREQ>(np.ToPointer());

	// create the request callback
	m_nativeParseError = gcnew NativeParseErrorReceived(this, &LightrailClient::OnDeliveryParseErrorReceived);
	m_nativeParseErrorHandle = GCHandle::Alloc(m_nativeParseError);
	np = Marshal::GetFunctionPointerForDelegate(m_nativeParseError);
	NATIVEPARERR nativeParErrFunc = static_cast<NATIVEPARERR>(np.ToPointer());

	m_nativeCallback = new LightrailClientCallback(this->m_client, nativePubFunc, nativeReqFunc, nativeParErrFunc);

	MANAGEDTRYEND
}

LightrailClient::~LightrailClient()
{
	MANAGEDTRYBEGIN

	delete m_nativeCallback;

	m_nativePublishHandle.Free();
	m_nativeRequestHandle.Free();
	m_nativeParseErrorHandle.Free();

	delete m_client;

	MANAGEDTRYEND
}

void LightrailClient::StartDeliveries()
{
	MANAGEDTRYBEGIN

	this->m_client->StartDeliveries();

	MANAGEDTRYEND
}

void LightrailClient::StopDeliveries()
{
	MANAGEDTRYBEGIN

	this->m_client->StopDeliveries();

	MANAGEDTRYEND
}

void LightrailClient::RegisterSecurityKey(String^ name, array<Byte>^ key)
{
	MANAGEDTRYBEGIN

	std::string keystring;
	for (int i = 0; i < key->Length; i++)
	{
		keystring.push_back(key[i]);
	}

	pin_ptr<const wchar_t> wname = PtrToStringChars(name);

	const Xylasoft::ISecurityKey* seckey = Xylasoft::Lightrail::NewAES256SecurityKey(wname, keystring.c_str(), keystring.size());

	m_client->RegisterSecurityKey(seckey);

	MANAGEDTRYEND
}

void LightrailClient::DeregisterSecurityKey(String^ name)
{
	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(name);

	this->m_client->DeregisterSecurityKey(wname);

	MANAGEDTRYEND
}

void LightrailClient::RegisterTerminal(TerminalStandpoint standpoint, String^ name, String^ description, String^ matchIdentifier)
{
	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(name);
	pin_ptr<const wchar_t> wdesc = PtrToStringChars(description);
	pin_ptr<const wchar_t> wmatch = PtrToStringChars(matchIdentifier);

	const ITerminal* terminal = Xylasoft::Lightrail::NewTerminal(wname, wdesc, wmatch);
	this->m_client->RegisterTerminal(static_cast<Xylasoft::Standpoint::StandpointType>(standpoint), terminal);
	
	MANAGEDTRYEND
}

void LightrailClient::RegisterTerminal(TerminalStandpoint standpoint, String^ name, String^ description, String^ matchIdentifier, String^ securityKeyName)
{
	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(name);
	pin_ptr<const wchar_t> wdesc = PtrToStringChars(description);
	pin_ptr<const wchar_t> wmatch = PtrToStringChars(matchIdentifier);
	pin_ptr<const wchar_t> wkey = PtrToStringChars(securityKeyName);

	const ITerminal* terminal = Xylasoft::Lightrail::NewTerminal(wname, wdesc, wmatch, wkey);
	this->m_client->RegisterTerminal(static_cast<Xylasoft::Standpoint::StandpointType>(standpoint), terminal);

	MANAGEDTRYEND
}

void LightrailClient::DeregisterTerminal(TerminalStandpoint standpoint, String^ name)
{
	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(name);

	this->m_client->DeregisterTerminal(static_cast<Xylasoft::Standpoint::StandpointType>(standpoint), wname);

	MANAGEDTRYEND
}

void LightrailClient::Publish(String^ terminalName, Delivery^ delivery)
{
	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(terminalName);

	IDelivery* idelivery = Lightrail::NewDelivery();

	const char* data = __nullptr;
	long size = this->ConvertByteArrayToBinaryArray(delivery->Message(), &data);
	Xylasoft::Encoding::EncodingType type = this->ConvertEncoding(delivery->Encoding());

	array<String^>^ labelList = delivery->GetLabelNameList();
	for (int i = 0; i < labelList->Length; i++)
	{
		pin_ptr<const wchar_t> wlabelname = PtrToStringChars(labelList[i]);
		pin_ptr<const wchar_t> wlabelval = PtrToStringChars(delivery->Label(labelList[i]));

		idelivery->SetLabel(wlabelname, wlabelval);
	}

	idelivery->SetMessage(data, size, type);
	delete[] data;

	this->m_client->Publish(wname, idelivery);

	MANAGEDTRYEND
}

DetailedDelivery^ LightrailClient::Request(String^ terminalName, Delivery^ delivery, long timeout)
{
	return this->Request(terminalName, delivery, timeout, 1);
}

DetailedDelivery^ LightrailClient::Request(String^ terminalName, Delivery^ delivery, long timeout, long reqResponders)
{
	MANAGEDTRYBEGIN

	pin_ptr<const wchar_t> wname = PtrToStringChars(terminalName);

	IDelivery* idelivery = Lightrail::NewDelivery();

	const char* data = __nullptr;
	long size = this->ConvertByteArrayToBinaryArray(delivery->Message(), &data);
	Xylasoft::Encoding::EncodingType type = this->ConvertEncoding(delivery->Encoding());

	idelivery->SetMessage(data, size, type);
	delete[] data;

	const IDetailedDelivery* response = this->m_client->Request(wname, idelivery, timeout, reqResponders);

	List<String^>^ terms = gcnew List<String^>();
	terms->Add(terminalName);
	DetailedDelivery^ managedResponse = gcnew DetailedDelivery(response, terms->ToArray());

	delete response;

	return managedResponse;

	MANAGEDTRYEND
}

void LightrailClient::OnTerminalPublishReceived(ITerminalDelivery& delivery)
{
	List<String^>^ terms = gcnew List<String^>();
	for (long i = 0; i < delivery.NumberOfReceiptTerminals(); i++)
	{
		terms->Add(gcnew String(delivery.ReceiptTerminal(i)->Name()));
	}

	DetailedDelivery^ managedDelivery = gcnew DetailedDelivery(delivery.DetailedDelivery(), terms->ToArray());

	OnPublishReceived(managedDelivery);
}

void LightrailClient::OnTerminalRequestReceived(ITerminalReturnDelivery& delivery)
{
	List<String^>^ terms = gcnew List<String^>();
	for (long i = 0; i < delivery.NumberOfReceiptTerminals(); i++)
	{
		terms->Add(gcnew String(delivery.ReceiptTerminal(i)->Name()));
	}

	DetailedDelivery^ managedDelivery = gcnew DetailedDelivery(delivery.DetailedDelivery(), terms->ToArray());

	Delivery^ response = OnRequestReceived(managedDelivery);

	const char* data = __nullptr;
	long size = this->ConvertByteArrayToBinaryArray(response->Message(), &data);
	Xylasoft::Encoding::EncodingType encoding = this->ConvertEncoding(response->Encoding());

	delivery.ReturnedDelivery()->SetMessage(data, size, encoding);
	delete[] data;
}

void LightrailClient::OnDeliveryParseErrorReceived(const IDeliveryParseError& error)
{
	DeliveryParseError^ managedError = gcnew DeliveryParseError(error);

	OnParseErrorReceived(managedError);
}

long LightrailClient::ConvertByteArrayToBinaryArray(array<Byte>^ byteArray, const char** data)
{
	char* newData = new char[byteArray->Length];

	for (long i = 0; i < byteArray->Length; i++)
	{
		newData[i] = byteArray[i];
	}

	*data = newData;
	return byteArray->Length;
}

Xylasoft::Encoding::EncodingType LightrailClient::ConvertEncoding(DeliveryEncoding encoding)
{
	Xylasoft::Encoding::EncodingType newEncoding;

	switch (encoding)
	{
	case DeliveryEncoding::Ascii:
		newEncoding = Xylasoft::Encoding::ASCII;
		break;
	case DeliveryEncoding::Utf8:
		newEncoding = Xylasoft::Encoding::UTF8;
		break;
	case DeliveryEncoding::Utf16:
		newEncoding = Xylasoft::Encoding::UTF16;
		break;
	case DeliveryEncoding::Utf32:
		newEncoding = Xylasoft::Encoding::UTF32;
		break;
	default:
		newEncoding = Xylasoft::Encoding::BINARY;
		break;
	}

	return newEncoding;
}