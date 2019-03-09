/*!
@file	AES256SecurityKey.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef AES256SECURITYKEY_H
#define AES256SECURITYKEY_H

#include "Lightrail_global.h"
#include "SecurityKey.h"
#include <XEncrypter.h>

namespace Xylasoft
{
	/*!
	This implements a security key for AES 256-bit encryption.
	*/
	class AES256SecurityKey : public SecurityKey
	{
	public:
		/*!
		Construct a 256-bit AES encryption security key.
		@param[in] name The name of the key.
		@param[in] key The key to encrypt/decrypt with. It will be automatically resized if it is
		less or greater than the required size for encryption/decryption.
		*/
		AES256SecurityKey(const wchar_t* name, const std::string& key);

		/*!
		Destroy the security key.
		*/
		virtual ~AES256SecurityKey();

		/*!
		Encrypt the set of data.
		@param[in] originalData A copy of the data to be encrypted.
		@returns The encrypted copy.
		*/
		virtual XByteArray Encrypt(const XByteArray& originalData) const override;

		/*!
		Decrypt the set of data.
		@param[in] encryptData A copy of the encrypted data to decrypt.
		@returns The decrypted data.
		*/
		virtual XByteArray Decrypt(const XByteArray& encryptData) const override;
		
	private:
		XEncrypter m_encrypter;
	};
}

#endif