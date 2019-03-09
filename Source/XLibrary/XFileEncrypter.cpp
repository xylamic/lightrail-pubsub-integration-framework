/*!
@file	XFileEncrypter.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XFileEncrypter.h"
#include "XException.h"
#include "XFile.h"
#include "XDirectory.h"
#include "XUuid.h"
#include "XUtilities.h"
#include <sstream>

using namespace Xylasoft;

#define HEADER_SIZE_CURRENT					1024
#define HEADER_SIZE_UNENCRYPTED				HEADER_SIZE_CURRENT - 32 /* we subtract 32 here for the additional block an encrypter adds */
#define HEADER_VERSION_CURRENT				1
#define BLOCK_SIZE							10240
#define BLOCK_SIZE_UNENCRYPTED				BLOCK_SIZE - 32

const std::string XFileEncrypter::m_formatdef = "%%XylasoftEncryptedFile%%";

XFileEncrypter::XFileEncrypter(const wchar_t* filepath, const std::string& key)
	: m_encrypter(key)
{
	this->m_filepath = filepath;
}

XFileEncrypter::~XFileEncrypter()
{
}

std::wstring XFileEncrypter::Encrypt(bool keepFileName)
{
	// create the file objects
	XFile inputFile(this->m_filepath.c_str());
	std::wstring tempfilepath = this->GetTempFile().c_str();
	XFile outputFile(tempfilepath.c_str());

	// verify the input file exists
	if (!inputFile.Exists())
	{
		throw FileException(L"The file does not exist.");
	}
	// try to duplicate the file
	if (!outputFile.CopyFrom(inputFile.Path().c_str()))
	{
		throw FileException(L"The temporary file failed to be created.");
	}

	// create the input/output stream
	std::fstream infile, outfile;

	try
	{
		// open the input file for read/write
		infile.open(inputFile.Path().c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		if (!infile.is_open())
		{
			throw FileException(L"The input file failed to be opened for read/write.");
		}

		// open the output file for writing
		outfile.open(outputFile.Path().c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		if (!outfile.is_open())
		{
			throw FileException(L"The output file failed to open for writing.");
		}

		// write the encrypted header
		XByteArray header = this->CreateEncryptedHeader(inputFile.Name().c_str());
		outfile.write(header.ConstData(), header.Size());

		// copy/encrypt the file
		XByteArray buffer;
		this->ReadBlock(infile, buffer, BLOCK_SIZE);
		while (buffer.Size() > 0)
		{
			XByteArray encryptedData = this->m_encrypter.Encrypt(buffer);
			outfile.write(encryptedData.ConstData(), encryptedData.Size());

			buffer.Clear();
			this->ReadBlock(infile, buffer, BLOCK_SIZE);
		}

		// close the files
		infile.close();
		outfile.flush();
		outfile.close();

		// delete the input file as it is not longer needed
		if (!inputFile.Delete())
		{
			throw FileException(L"Failed to delete the temporary file.");
		}

		// if we wanted to keep the original file name, rename the temporary file to the original name
		if (keepFileName)
		{
			outputFile.Rename(inputFile.Name().c_str());
		}

		// set the new filename
		this->m_filepath = outputFile.Path().c_str();
	}
	// if we failed in the middle of writing, the file will be corrupted
	// it would make sense to use the transactional file system here to prevent this
	// in the rare instance it occurs
	catch (...)
	{
		// verify the files were/are closed
		infile.close();
		outfile.close();

		// delete the temporary file
		outputFile.Delete();

		throw;
	}

	return outputFile.Path();
}

std::wstring XFileEncrypter::Decrypt()
{
	// create the file objects
	XFile inputFile(this->m_filepath.c_str());
	std::wstring tempfilepath = this->GetTempFile().c_str();
	XFile outputFile(tempfilepath.c_str());

	// verify the input file exists
	if (!inputFile.Exists())
	{
		throw FileException(L"The file does not exist.");
	}
	// try to duplicate the file
	if (!outputFile.CopyFrom(inputFile.Path().c_str()))
	{
		throw FileException(L"The temporary file failed to be created.");
	}

	// create the input/output stream
	std::fstream infile, outfile;

	try
	{
		// open the input file for read/write
		infile.open(inputFile.Path().c_str(), std::ios_base::in | std::ios_base::binary);
		if (!infile.is_open())
		{
			throw FileException(L"The input file failed to be opened for read.");
		}

		// open the output file for writing
		outfile.open(outputFile.Path().c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		if (!outfile.is_open())
		{
			throw FileException(L"The output file failed to open for writing.");
		}

		// read the encrypted header
		long version, blocksize;
		std::wstring origfilename;
		this->ReadEncryptedHeader(infile, version, origfilename, blocksize);

		// copy/encrypt the file
		XByteArray buffer;
		this->ReadBlock(infile, buffer, blocksize);
		while (buffer.Size() > 0)
		{
			XByteArray decryptedData = this->m_encrypter.Decrypt(buffer);
			outfile.write(decryptedData.ConstData(), decryptedData.Size());

			buffer.Clear();
			this->ReadBlock(infile, buffer, blocksize);
		}

		// close the files
		infile.close();
		outfile.flush();
		outfile.close();

		// delete the input file as it is not longer needed
		if (!inputFile.Delete())
		{
			throw FileException(L"Failed to delete the temporary file.");
		}

		// change the outfile to the original file name
		outputFile.Rename(origfilename.c_str());
	}
	// if we failed in the middle of writing, the file will be corrupted
	// it would make sense to use the transactional file system here to prevent this
	// in the rare instance it occurs
	catch (...)
	{
		// verify the files were/are closed
		infile.close();
		outfile.close();

		// delete the temporary file
		outputFile.Delete();

		throw;
	}

	return outputFile.Path();
}

void XFileEncrypter::ReadBlock(std::fstream& infile, XByteArray& data, long maxSize) const
{
	// verify we have not already exceeded our buffer size
	if (data.Size() >= maxSize)
	{
		return;
	}

	// read the file
	char c = infile.get();
	while (!infile.eof())
	{
		data.Append(c);

		if (data.Size() == maxSize)
		{
			return;
		}

		c = infile.get();
	}
}

std::wstring XFileEncrypter::GetTempFile() const
{
	// create a unique identifier
	XUuid tempfileid = XUuid::CreateNew();

	// generate a name with the unique identifier
	std::wstring filename = tempfileid.ToWString();
	filename.append(L".xye");

	// get the directory of the specified file
	XFile file(this->m_filepath.c_str());
	XDirectory dir(file.Directory().c_str());

	// create the file path
	XFile tmpfile(dir.File(filename.c_str()));

	// return the path
	return tmpfile.Path();
}

XByteArray XFileEncrypter::CreateEncryptedHeader(const wchar_t* filename) const
{
	XByteArray header(HEADER_SIZE_CURRENT, 0);

	static long versionOffset = 0;
	static long versionLength = 8;
	static long filenameOffset = versionOffset + versionLength;
	static long filenameLength = 256;
	static long blockSizeOffset = filenameOffset + filenameLength;
	static long blockSizeLength = 8;

	// write version
	std::stringstream ss;
	ss << HEADER_VERSION_CURRENT;
	::strcpy(header[versionOffset], ss.str().c_str());
	ss.str("");

	// write the file name
	std::string sfilename = XUtilities::GetUTF8BytesFromString(filename);
	if (static_cast<long>(sfilename.size()) >= filenameLength)
	{
		sfilename.resize(filenameLength - 1);
	}
	::strcpy(header[filenameOffset], sfilename.c_str());

	// write the block size
	ss << BLOCK_SIZE;
	::strcpy(header[blockSizeOffset], ss.str().c_str());
	ss.str("");

	// encrypt the header block
	header = this->m_encrypter.Encrypt(header);

	// return the header
	return header;
}

void XFileEncrypter::ReadEncryptedHeader(std::fstream& infile, long& version, std::wstring& filename, long& blockSize) const
{
	static long versionOffset = 0;
	static long versionLength = 8;
	static long filenameOffset = versionOffset + versionLength;
	static long filenameLength = 256;
	static long blockSizeOffset = filenameOffset + filenameLength;
	static long blockSizeLength = 8;

	XByteArray header;

	// read the header block
	this->ReadBlock(infile, header, HEADER_SIZE_CURRENT);
	if (header.Size() < HEADER_SIZE_CURRENT)
	{
		throw EncryptionException(L"The file was too short. The file may be corrupted.");
	}

	// decrypt the header block
	header = this->m_encrypter.Decrypt(header);

	// read the version
	version = ::atol(header[versionOffset]);

	// verify the version
	if (version < HEADER_VERSION_CURRENT)
	{
		throw EncryptionException(L"The header format version was not recognized. The file may be corrupted.");
	}
	else if (version > HEADER_VERSION_CURRENT)
	{
		throw EncryptionException(L"The header format version was not recognized. This file may have been created with a newer version.");
	}

	// read the rest of the attributes
	filename = XUtilities::GetStringFromUTF8Bytes(header[filenameOffset]);
	blockSize = ::atol(header[blockSizeOffset]);

	// verify attributes
	if (filename.size() <= 0 || static_cast<long>(filename.size()) >= filenameLength || blockSize <= 0)
	{
		throw EncryptionException(L"The file attributes were invalid. The file may be corrupted.");
	}
}