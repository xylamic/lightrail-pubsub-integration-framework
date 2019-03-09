/*!
@file	XEncrypter.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XEncrypter.h"
#include "XException.h"
#include <Rijndael.h>
#include <sstream>
#include <iomanip>

using namespace Xylasoft;

XEncrypter::XEncrypter(const std::string& key, int keysize)
{
	if (keysize != 16 && keysize != 24 && keysize != 32)
	{
		throw ArgumentException(L"The key size was invalid.");
	}

	this->m_key = key;
	this->m_keySize = keysize;
	this->m_blockSize = 32;

	// make the key the correct size if its not
	while (this->m_key.size() < static_cast<size_t>(this->m_keySize))
	{
		this->m_key.push_back(0);
	}
	if (this->m_key.size() > static_cast<size_t>(this->m_keySize))
	{
		this->m_key.resize(32);
	}
}


XEncrypter::~XEncrypter()
{
}

XByteArray XEncrypter::Encrypt(XByteArray originalData) const
{
	// create the algorithm and key
	CRijndael alg;
	alg.MakeKey(this->m_key.c_str(), CRijndael::sm_chain0, this->m_keySize, this->m_blockSize);
	alg.ResetChain();

	// create a block with the key embedded to be able to check encryption on decryption (1/2 block)
	std::string checkBlock = this->m_key;
	checkBlock.resize(this->m_blockSize / 2);

	// pad the data as needed
	long originalSize = originalData.Size();
	int modPadd = (originalSize % this->m_blockSize);
	int padding = modPadd > 0 ? this->m_blockSize - modPadd : 0;
	originalData.Resize(originalSize + padding);
	::memset(&originalData.Data()[originalSize], 0, padding);

	// create the padding block (1/2 block)
	std::ostringstream paddingstr;
	paddingstr << std::setw(this->m_blockSize / 2) << std::setfill('0') << padding;
	std::string paddingBlock = paddingstr.str();

	// join with the embedded key
	checkBlock.append(paddingBlock);
	assert(checkBlock.size() == this->m_blockSize);

	// encrypt the front block
	std::string encryptedPaddingBlock;
	encryptedPaddingBlock.resize(this->m_blockSize / 2);
	::memset(&encryptedPaddingBlock[0], 0, this->m_blockSize / 2);
	alg.EncryptBlock(checkBlock.c_str(), &encryptedPaddingBlock[0]);

	// create return for encrypted data and copy padding block
	XByteArray outdata(encryptedPaddingBlock.c_str(), this->m_blockSize);
	outdata.Resize(originalData.Size() + this->m_blockSize);
	::memset(&outdata.Data()[this->m_blockSize], 0, originalData.Size());

	// encrypt the original data
	alg.ResetChain();
	alg.Encrypt(originalData.ConstData(), &outdata.Data()[this->m_blockSize], originalData.Size(), CRijndael::CFB);

	return outdata;
}

XByteArray XEncrypter::Decrypt(XByteArray encryptData) const
{
	if (encryptData.Size() % this->m_blockSize != 0)
	{
		throw DataException(L"The block size did not match for this algorithm.");
	}
			
	// create the algorithm and key
	CRijndael alg;
	alg.MakeKey(this->m_key.c_str(), CRijndael::sm_chain0, this->m_keySize, this->m_blockSize);
	alg.ResetChain();

	// create data for padding data
	XByteArray paddingData;
	paddingData.Resize(this->m_blockSize);
	::memset(paddingData.Data(), 0, this->m_blockSize);

	// decrypt the padding block
	alg.DecryptBlock(encryptData.ConstData(), paddingData.Data());

	// verify the decryption is valid
	std::string paddingstr(paddingData.Data(), this->m_blockSize);
	std::string keystring = paddingstr.substr(0, this->m_blockSize / 2);
	paddingstr = paddingstr.substr(this->m_blockSize / 2, this->m_blockSize / 2);
	if (keystring.compare(this->m_key.substr(0, this->m_blockSize / 2)) != 0)
	{
		throw DataException(L"The data could not be decrypted correctly with this key.");
	}

	// verify the padding block
	int padding = ::atoi(paddingstr.c_str());
	if (padding > this->m_blockSize - 1)
	{
		throw DataException(L"The data was in the incorrect format.");
	}

	// create the output array
	XByteArray outdata;
	outdata.Resize(encryptData.Size() - this->m_blockSize);
	::memset(outdata.Data(), 0, outdata.Size());

	// decrypt the body
	alg.ResetChain();
	alg.Decrypt(&encryptData.Data()[this->m_blockSize], outdata.Data(), outdata.Size(), CRijndael::CFB);

	// remove the padded bytes
	outdata.Resize(outdata.Size() - padding);
			
	return outdata;
}