/*!
@file	XIniTest.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Tests.h"

void XIniTest()
{
	XIni ini(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	ini.SetKeyValueForSection(L"sec1", L"key1", L"val1");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	ini.SetKeyValueForSection(L"sec2", L"key1", L"val2");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	ini.SetKeyValueForSection(L"sec1", L"key1", L"val3");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	ini.SetKeyValueForSection(L"sec1", L"key2", L"val4");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	std::list<std::wstring> keys = ini.GetKeysForSection(L"sec1");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
	std::wstring value = ini.GetValueForKey(L"sectest", L"key1");
	ini.Save(L"C:\\Users\\Xylamic\\Desktop\\test.ini");
}