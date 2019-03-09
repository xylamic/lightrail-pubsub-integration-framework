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

void SerializerTest()
{
	cout << "Running serialization test..." << endl;

	std::string bytestr;
	bytestr.push_back((char)0x39);
	bytestr.push_back((char)0x92);
	bytestr.push_back((char)0xAA);
	bytestr.push_back(0x00);
	bytestr.push_back(0x00);
	bytestr.push_back((char)0xFF);

	XByteArray* copydata = nullptr;
	{
		XByteArray data;
		XSerializer serializer(data);

		serializer << static_cast<uint16_t>(9459);
		serializer << static_cast<uint32_t>(294859);
		serializer << "Hello There";
		serializer << L"WHello There";
		serializer << static_cast<int32_t>(-53346);
		serializer << static_cast<int32_t>(532356);
		serializer << static_cast<double>(3820.82938);
		serializer << XByteArray(bytestr.c_str(), static_cast<long>(bytestr.size()));
		serializer << static_cast<uint16_t>(9459);
		serializer << static_cast<uint32_t>(294859);
		serializer << "Hello There";
		serializer << L"WHello There";
		serializer << static_cast<int32_t>(-53346);
		serializer << static_cast<int32_t>(532356);
		serializer << static_cast<double>(3820.82938);
		serializer << XByteArray(bytestr.c_str(), static_cast<long>(bytestr.size()));
		serializer << static_cast<uint16_t>(9459);
		serializer << static_cast<uint32_t>(294859);
		serializer << "Hello There";
		serializer << L"WHello There";
		serializer << static_cast<int32_t>(-53346);
		serializer << static_cast<int32_t>(532356);
		serializer << static_cast<double>(3820.82938);
		serializer << XByteArray(bytestr.c_str(), static_cast<long>(bytestr.size()));

		copydata = new XByteArray(data);
	}

	{
		uint16_t val16;
		uint32_t val32;
		int32_t val32s;
		XByteArray data;
		std::string ht;
		std::wstring wht;
		double valdbl;

		XSerializer serializer(copydata);

		serializer >> val16;
		assert(val16 == 9459);

		serializer >> val32;
		assert(val32 == 294859);

		serializer >> ht;
		assert(ht.compare("Hello There") == 0);

		serializer >> wht;
		assert(wht.compare(L"WHello There") == 0);

		serializer >> val32s;
		assert(val32s == -53346);

		serializer >> val32s;
		assert(val32s == 532356);

		serializer >> valdbl;
		assert(valdbl == 3820.82938);

		serializer >> data;
		assert(XUtilities::CompareByteArrays(bytestr.c_str(), static_cast<long>(bytestr.size()), 
			data.ConstData(), static_cast<long>(data.Size())));

		serializer >> val16;
		assert(val16 == 9459);

		serializer >> val32;
		assert(val32 == 294859);

		serializer >> ht;
		assert(ht.compare("Hello There") == 0);

		serializer >> wht;
		assert(wht.compare(L"WHello There") == 0);

		serializer >> val32s;
		assert(val32s == -53346);

		serializer >> val32s;
		assert(val32s == 532356);

		serializer >> valdbl;
		assert(valdbl == 3820.82938);

		serializer >> data;
		assert(XUtilities::CompareByteArrays(bytestr.c_str(), static_cast<long>(bytestr.size()), 
			data.ConstData(), static_cast<long>(data.Size())));

		serializer >> val16;
		assert(val16 == 9459);

		serializer >> val32;
		assert(val32 == 294859);

		serializer >> ht;
		assert(ht.compare("Hello There") == 0);

		serializer >> wht;
		assert(wht.compare(L"WHello There") == 0);

		serializer >> val32s;
		assert(val32s == -53346);

		serializer >> val32s;
		assert(val32s == 532356);

		serializer >> valdbl;
		assert(valdbl == 3820.82938);

		serializer >> data;
		assert(XUtilities::CompareByteArrays(bytestr.c_str(), static_cast<long>(bytestr.size()), 
			data.ConstData(), static_cast<long>(data.Size())));
	}

	cout << "Completed serialization test." << endl;
}