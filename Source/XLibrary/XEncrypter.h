/*!
@file	XEncrypter.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XENCRYPTER_H
#define XENCRYPTER_H

#include "XByteArray.h"

namespace Xylasoft
{
	/*!
	An encrypter allows the callback to encrypt and decrypt entire blocks of information using
	AES encryption.
	*/
	class XEncrypter
	{
	public:
		/*!
		Construct an encrypter with the given key.
		@param[in] key The key to use for encryption/decryption.
		@param[in] keysize The size of the key, must be 16, 24, or 32.
		*/
		XEncrypter(const std::string& key, int keysize = 32);

		/*!
		Destroy the encrypter.
		*/
		~XEncrypter();

		/*!
		Encrypt the set of data. The data attached will be expanded to the next multiple
		of the block size. In addition, one additional block size will be added for additional
		encryption information. (ie: if originalData has a length 1016, the block size is 32, then 
		the length of the returned array will be 1024 + 32 = 1056)
		@param[in] originalData A copy of the data to be encrypted.
		@returns The encrypted copy.
		*/
		XByteArray Encrypt(XByteArray originalData) const;

		/*!
		Decrypt the set of data.
		@param[in] encryptData A copy of the encrypted data to decrypt.
		@returns The decrypted data.
		*/
		XByteArray Decrypt(XByteArray encryptData) const;

	private:
		std::string m_key;
		int m_keySize;
		int m_blockSize;
	};
}

#endif