/*!
@file	DetailedDelivery.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "DetailedDelivery.h"

#define FORMAT_V1 0

using namespace Xylasoft;

DetailedDelivery::DetailedDelivery(const Delivery* delivery, const Terminal& terminal, const wchar_t* sourceApp,
	const DiscoveryPeer& source)
	: m_id(XUuid::CreateNew()), m_requestId(XUuid::CreateEmpty()), m_delivery(delivery), m_terminal(terminal)
{
	this->m_sourceapp = sourceApp;
	this->m_sourcehost = source.Hostname();
	this->m_sourceport = source.Port();
	::time(&this->m_time);
}

DetailedDelivery::DetailedDelivery(XSerializer& serializer)
	: m_id(XUuid::CreateEmpty()), m_requestId(XUuid::CreateEmpty()), m_delivery(new Delivery(serializer)), 
	m_terminal(serializer)
{
	uint16_t version;
	serializer >> version;

	if (version != FORMAT_V1)
	{
		throw DataException(L"The detailed delivery format version was unrecognized.");
	}

	uint32_t time;
	int32_t port;
	std::string uuid, requuid;

	serializer >> uuid;
	serializer >> requuid;
	serializer >> this->m_sourceapp;
	serializer >> this->m_sourcehost;
	serializer >> port;
	serializer >> time;
	serializer >> this->m_exmessage;

	this->m_id = XUuid(uuid);
	this->m_requestId = XUuid(requuid);
	this->m_time = time;
	this->m_sourceport = port;
}

DetailedDelivery::DetailedDelivery(const DetailedDelivery& other)
	: m_id(other.m_id), m_requestId(other.m_requestId), 
	m_delivery(new Delivery(*other.m_delivery)), m_terminal(other.m_terminal)
{
	this->m_sourceapp = other.m_sourceapp;
	this->m_sourcehost = other.m_sourcehost;
	this->m_sourceport = other.m_sourceport;
	this->m_time = other.m_time;
	this->m_exmessage = other.m_exmessage;
}

DetailedDelivery::~DetailedDelivery()
{
	delete this->m_delivery;
}

void DetailedDelivery::Serialize(XSerializer& serializer) const
{
	this->m_delivery->Serialize(serializer);
	this->m_terminal.Serialize(serializer);

	serializer << static_cast<uint16_t>(FORMAT_V1);

	serializer << this->m_id.ToString();
	serializer << this->m_requestId.ToString();
	serializer << this->m_sourceapp;
	serializer << this->m_sourcehost;
	serializer << static_cast<int32_t>(this->m_sourceport);
	serializer << static_cast<uint32_t>(this->m_time);
	serializer << this->m_exmessage;
}

XUuid DetailedDelivery::Identifier() const
{
	return this->m_id;
}

const ITerminal& DetailedDelivery::SourceTerminal() const
{
	return this->m_terminal;
}

long DetailedDelivery::SourcePort() const
{
	return this->m_sourceport;
}

const char* DetailedDelivery::SourceHost() const
{
	return this->m_sourcehost.c_str();
}

const wchar_t* DetailedDelivery::SourceApplication() const
{
	return this->m_sourceapp.c_str();
}

const time_t DetailedDelivery::TimeUtc() const
{
	return this->m_time;
}

std::wstring DetailedDelivery::MessageString() const
{
	return this->m_delivery->MessageString();
}

Encoding::EncodingType DetailedDelivery::Encoding() const
{
	return this->m_delivery->Encoding();
}

const char* DetailedDelivery::Message() const
{
	return this->m_delivery->Message();
}

long DetailedDelivery::MessageSize() const
{
	return this->m_delivery->MessageSize();
}

const wchar_t* DetailedDelivery::Label(const wchar_t* name) const
{
	return this->m_delivery->Label(name);
}

long DetailedDelivery::GetNumberOfLabels() const
{
	return this->m_delivery->GetNumberOfLabels();
}

const wchar_t* DetailedDelivery::GetLabelName(long index) const
{
	return this->m_delivery->GetLabelName(index);
}

XUuid DetailedDelivery::RequestIdentifier() const
{
	return this->m_requestId;
}

void DetailedDelivery::SetRequestIdentifier(XUuid id)
{
	this->m_requestId = id;
}

void DetailedDelivery::SetExceptionMessage(const char* msg)
{
	this->m_exmessage = msg;
}

const char* DetailedDelivery::ExceptionMessage()
{
	if (this->m_exmessage.size() <= 0)
	{
		return NULL;
	}
	else
	{
		return this->m_exmessage.c_str();
	}
}

void DetailedDelivery::DecryptMessage(const SecurityKey* securitykey)
{
	Delivery* delivery = const_cast<Delivery*>(this->m_delivery);
	delivery->DecryptMessage(securitykey);
}