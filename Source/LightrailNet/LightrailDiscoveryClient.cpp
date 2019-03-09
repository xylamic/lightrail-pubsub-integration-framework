/*!
@file	LightrailDiscoveryClient.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "StdAfx.h"
#include "LightrailDiscoveryClient.h"

using namespace Xylasoft;

DiscoveryClient::DiscoveryClient()
{
	m_client = Xylasoft::Lightrail::GetDiscoveryInstance();
	m_client->StartDiscovery();
}

DiscoveryClient::~DiscoveryClient()
{
	this->m_client->StopDiscovery();
	delete this->m_client;
}

array<DiscoveredParticipant^>^ DiscoveryClient::GetParticipants()
{
	List<DiscoveredParticipant^>^ participants = gcnew List<DiscoveredParticipant^>();

	SimpleListContainer<Participant>* slist = this->m_client->GetSubnetParticipants();
	for (long i = 0; i < slist->Size(); i++)
	{
		DiscoveredParticipant^ dp = gcnew DiscoveredParticipant();
		dp->Hostname = gcnew String(slist->Elements()[i].Hostname);
		dp->Port = slist->Elements()[i].Port;

		participants->Add(dp);
	}
	delete slist;

	return participants->ToArray();
}