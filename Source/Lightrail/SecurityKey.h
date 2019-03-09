/*!
@file	SecurityKey.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef SECURITYKEY_H
#define SECURITYKEY_H

#include "Lightrail_global.h"
#include "Method.h"
#include "ISecurityKey.h"
#include <XByteArray.h>

namespace Xylasoft
{
	/*!
	This class defines the basic functionality of a security key and implements standard naming.
	*/
	class SecurityKey : public ISecurityKey
	{
	public:
		/*!
		Construct a security key.
		@param[in] name The name of the key.
		@param[in] type The type of security.
		*/
		SecurityKey(const wchar_t* name, Security::SecurityType type);

		/*!
		Destroy the security key.
		*/
		virtual ~SecurityKey();

		/*!
		Get the name of the security key.
		*/
		virtual const wchar_t* Name() const override;

		/*!
		Get the type of security.
		*/
		virtual Security::SecurityType Type() const override;

		/*!
		Encrypt the set of data.
		@param[in] originalData A copy of the data to be encrypted.
		@returns The encrypted copy.
		*/
		virtual XByteArray Encrypt(const XByteArray& originalData) const = 0;

		/*!
		Decrypt the set of data.
		@param[in] encryptData A copy of the encrypted data to decrypt.
		@returns The decrypted data.
		*/
		virtual XByteArray Decrypt(const XByteArray& encryptData) const = 0;

	private:
		std::wstring m_name;
		Security::SecurityType m_type;
	};
}

#endif