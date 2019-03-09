/*!
@file	AES256SecurityKey.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "AES256SecurityKey.h"

using namespace Xylasoft;

AES256SecurityKey::AES256SecurityKey(const wchar_t* name, const std::string& key)
			: SecurityKey(name, Security::AES_256), m_encrypter(key)
{
}

/*!
Destroy the security key.
*/
AES256SecurityKey::~AES256SecurityKey()
{
}

/*!
Encrypt the set of data.
@param[in] originalData A copy of the data to be encrypted.
@returns The encrypted copy.
*/
XByteArray AES256SecurityKey::Encrypt(const XByteArray& originalData) const
{
	return this->m_encrypter.Encrypt(originalData);
}

/*!
Decrypt the set of data.
@param[in] encryptData A copy of the encrypted data to decrypt.
@returns The decrypted data.
*/
XByteArray AES256SecurityKey::Decrypt(const XByteArray& encryptData) const
{
	return this->m_encrypter.Decrypt(encryptData);
}