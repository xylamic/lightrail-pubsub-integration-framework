/*!
@file	XLicenseFileTest.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Tests.h"

void LicenseFileTest()
{
	XLicenseFile file(L"Company", L"Product", 2, 3, 12345678, 23456789, L"User", L"UserId", 4);

	assert(std::wstring(file.Company()).compare(L"Company") == 0);
	assert(std::wstring(file.Product()).compare(L"Product") == 0);
	assert(std::wstring(file.User()).compare(L"User") == 0);
	assert(std::wstring(file.UserLicenseId()).compare(L"UserId") == 0);
	assert(file.Expiration() == 12345678);
	assert(file.UpdateExpiration() == 23456789);
	assert(file.NumberLicenses() == 4);
	assert(file.Version() == 3);
	assert(file.Type() == 2);

	XByteArray filedata;
	file.Serialize(filedata);

	XByteArray filedata2(filedata);
	XLicenseFile file2(filedata2);

	assert(std::wstring(file.Company()).compare(file2.Company()) == 0);
	assert(std::wstring(file.Product()).compare(file2.Product()) == 0);
	assert(std::wstring(file.User()).compare(file2.User()) == 0);
	assert(std::wstring(file.UserLicenseId()).compare(file2.UserLicenseId()) == 0);
	assert(file.Expiration() == file2.Expiration());
	assert(file.UpdateExpiration() == file2.UpdateExpiration());
	assert(file.NumberLicenses() == file2.NumberLicenses());
	assert(file.Version() == file2.Version());
	assert(file.Type() == file2.Type());
}