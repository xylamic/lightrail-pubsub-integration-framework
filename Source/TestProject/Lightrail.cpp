/*!
@file	XHostAddress.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Tests.h"

void BasicCreate()
{
	ILightrail* l1 = Lightrail::GetLightrailInstance(L"l1", nullptr);
	ILightrail* l2 = Lightrail::GetLightrailInstance(L"l2", nullptr);

	delete l1;
	delete l2;
}

void BasicPubSub()
{
	ILightrail* l1 = Lightrail::GetLightrailInstance(L"l1", nullptr);
	ILightrail* l2 = Lightrail::GetLightrailInstance(L"l2", nullptr);

	l1->StartDeliveries();
	l2->StartDeliveries();

	l1->RegisterTerminal(Standpoint::PUBLISHER, Lightrail::NewTerminal(L"pub", nullptr, L"[a]"));
	l2->RegisterTerminal(Standpoint::SUBSCRIBER, Lightrail::NewTerminal(L"sub", nullptr, L"[a]"));

	::Sleep(1000);

	//l1->Publish(Lightrail::NewDelivery()->SetMessageString(L"Hello"), L"pub");

	delete l1;
	delete l2;
}

void LightrailTests()
{
	::BasicCreate();
	::BasicPubSub();
}