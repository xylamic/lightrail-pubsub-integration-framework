/*!
@file	UUID.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Tests.h"

void RunUuidTest()
{
	XUuid uuid = XUuid::CreateNew();

	cout << "Generated UUID: " << uuid.ToString() << endl;

	XUuid uuid2(uuid);

	cout << "Copied UUID   : " << uuid2.ToString() << endl;

	XUuid uuid3(uuid.ToString());

	cout << "Binary UUID   : " << uuid3.ToString() << endl;

	assert(uuid == uuid2);
	assert(!uuid.IsEmpty());
	assert(!uuid2.IsEmpty());
}

void UuidTest()
{
	for (int i = 0; i < 5; i++)
	{
		RunUuidTest();
	}

	XUuid emptyuuid = XUuid::CreateEmpty();
	cout << "Empty UUID    : " << emptyuuid.ToString() << endl;
	assert(emptyuuid.ToString() == "00000000-0000-0000-0000-000000000000");
	assert(emptyuuid.IsEmpty());
}