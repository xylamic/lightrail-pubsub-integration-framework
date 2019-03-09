/*!
@file	XFileEncrypter.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XFILEENCRYPTER_H
#define XFILEENCRYPTER_H

#include "Xylasoft.h"
#include "XEncrypter.h"
#include "XByteArray.h"
#include <fstream>

namespace Xylasoft
{
	class XFileEncrypter
	{
	public:
		XFileEncrypter(const wchar_t* filepath, const std::string& key);

		~XFileEncrypter();

		std::wstring Encrypt(bool keepFileName = true);
		
		std::wstring Decrypt();

	private:
		void ReadBlock(std::fstream& infile, XByteArray& data, long maxSize) const;

		std::wstring GetTempFile() const;

		XByteArray CreateEncryptedHeader(const wchar_t* filename) const;

		void ReadEncryptedHeader(std::fstream& infile, long& version, std::wstring& filename, long& blockSize) const;

	private:
		XFileEncrypter(const XFileEncrypter&);
		XFileEncrypter& operator=(const XFileEncrypter&);

	private:
		std::wstring m_filepath;
		XEncrypter m_encrypter;
		static const std::string m_formatdef;
	};
}

#endif