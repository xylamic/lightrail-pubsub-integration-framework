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

XByteArray ReadFileContent(const wchar_t* filepath)
{
	std::ifstream infile(filepath, std::ios_base::in | std::ios_base::binary);
	assert(infile.is_open());

	XByteArray filecontent;
	char c = infile.get();
	while (!infile.eof())
	{
		filecontent.Append(c);
		c = infile.get();
	}

	return filecontent;
}

void FileEncrypterTest()
{
	const wchar_t* filepath = L"C:\\Users\\XYLAMIC\\Downloads\\testexe.exe";

	XFileEncrypter encrypter(filepath, "TestPassword");

	XByteArray filecontent1 = ReadFileContent(filepath);

	std::wstring newfilepath = encrypter.Encrypt(false);

	XByteArray filecontent2 = ReadFileContent(newfilepath.c_str());
	assert(!XUtilities::CompareByteArrays(filecontent1.ConstData(), filecontent1.Size(), filecontent2.ConstData(), filecontent2.Size()));

	encrypter.Decrypt();

	XByteArray filecontent3 = ReadFileContent(filepath);
	assert(XUtilities::CompareByteArrays(filecontent1.ConstData(), filecontent1.Size(), filecontent3.ConstData(), filecontent3.Size()));
}