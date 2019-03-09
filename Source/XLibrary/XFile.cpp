/*!
@file	XFile.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XFile.h"

using namespace Xylasoft;

XFile::XFile(const wchar_t* path)
{
	DWORD attributes = ::GetFileAttributesW(path);
	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		this->m_exists = false;
	}
	else
	{
		// only mark the file as existing if it is not a directory
		this->m_exists = (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	std::wstring pathString = path;
	size_t index1 = pathString.find_last_of(L'/');
	size_t index2 = pathString.find_last_of(L'\\');

	if (index1 == pathString.npos && index2 == pathString.npos)
	{
		this->m_directory = L"";
		this->m_filename = pathString;
	}
	else
	{
		if (index1 != pathString.npos && index2 != pathString.npos)
		{
			index1 = max(index1, index2);
		}
		else if (index1 == pathString.npos)
		{
			index1 = index2;
		}

		this->m_directory = pathString.substr(0, index1 + 1);
		this->m_filename = pathString.substr(index1 + 1);
	}
}

bool XFile::Exists()
{
	return this->m_exists;
}

bool XFile::Delete()
{
	if (::DeleteFileW(this->Path().c_str()) != 0)
	{
		this->m_exists = false;
		return true;
	}
	else
	{
		return false;
	}
}

void XFile::Shred()
{
	// need to add functionality to zero-out file before deletion TODO

	// delete the file
	this->Delete();
}

std::wstring XFile::Directory()
{
	return this->m_directory;
}

std::wstring XFile::Name()
{
	return this->m_filename;
}

std::wstring XFile::Path()
{
	std::wstring path = this->m_directory;
	path.append(this->m_filename);

	return path;
}

bool XFile::Move(const wchar_t* directorypath)
{
	std::wstring newFilePath = directorypath;
	newFilePath.append(this->m_filename);

	if (::MoveFileW(this->Path().c_str(), newFilePath.c_str()))
	{
		this->m_directory = directorypath;
		return true;
	}
	else
	{
		return false;
	}
}

bool XFile::CopyFrom(const wchar_t* filepath)
{
	return ::CopyFileW(filepath, this->Path().c_str(), true) != 0;
}

bool XFile::Rename(const wchar_t* name)
{
	std::wstring newFilePath = this->m_directory;
	newFilePath.append(name);

	if (::MoveFileW(this->Path().c_str(), newFilePath.c_str()))
	{
		this->m_filename = name;
		return true;
	}
	else
	{
		return false;
	}
}