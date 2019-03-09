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

#ifndef EXCLUDETESTLIBS

void KeyTest()
{
	const char* key = "this is my test key";
	const ISecurityKey* isecurityKey = Lightrail::NewAES256SecurityKey(L"name", key, strlen(key));

	const char* text = "This is a set of test text that extends past a single block of data and therefore will provide as a better test!!";
	XByteArray originalText(text, strlen(text));
	originalText.Append(0x00);
	originalText.Append(0x50);
	originalText.Append(0xFF);
	originalText.Append(0xAA);
	originalText.Append(0x11);

	const SecurityKey* securityKey = static_cast<const SecurityKey*>(isecurityKey);
	XByteArray encryptedText = securityKey->Encrypt(originalText);

	assert(!XUtilities::CompareByteArrays(originalText.ConstData(), originalText.Size(), encryptedText.ConstData(), encryptedText.Size()));

	XByteArray decryptedText = securityKey->Decrypt(encryptedText);

	assert(XUtilities::CompareByteArrays(originalText.ConstData(), originalText.Size(), decryptedText.ConstData(), decryptedText.Size()));

	cout << "New Text: " << decryptedText.ConstData() << endl;
}

#endif

void XEncrypterTest()
{
	const char* key = "this is my test key";
	XEncrypter security(key);

	const char* text = "This is a set of test text that extends past a single block of data and therefore will provide as a better test!!";
	XByteArray originalText(text, static_cast<long>(strlen(text)));
	originalText.Append(0x00);
	originalText.Append(0x50);
	originalText.Append((char)0xFF);
	originalText.Append((char)0xAA);
	originalText.Append(0x11);

	XByteArray encryptedText = security.Encrypt(originalText);

	assert(!XUtilities::CompareByteArrays(originalText.ConstData(), originalText.Size(), encryptedText.ConstData(), encryptedText.Size()));

	XByteArray decryptedText = security.Decrypt(encryptedText);

	assert(XUtilities::CompareByteArrays(originalText.ConstData(), originalText.Size(), decryptedText.ConstData(), decryptedText.Size()));

	cout << "New Text: " << decryptedText.ConstData() << endl;
}

void EncryptionTest()
{
	for (int i = 0; i < 5; i++)
	{
		XEncrypterTest();
	}

#ifndef EXCLUDETESTLIBS
	KeyTest();
#endif
}