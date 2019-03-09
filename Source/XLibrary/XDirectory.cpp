/*!
@file	XDirectory.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XDirectory.h"

using namespace Xylasoft;

XDirectory::XDirectory(const wchar_t* path)
{
	DWORD attributes = ::GetFileAttributesW(path);
	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		this->m_exists = false;
	}
	else
	{
		// only mark the file as existing if it is a directory
		this->m_exists = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	// verify it ends in a slash
	this->m_directory = path;
	if (this->m_directory.size() > 0)
	{
		if (this->m_directory[this->m_directory.size() - 1] != L'/' &&
			this->m_directory[this->m_directory.size() - 1] != L'\\')
		{
			this->m_directory.append(L"/");
		}
	}
}

bool XDirectory::Exists()
{
	return this->m_exists;
}

std::wstring XDirectory::Path()
{
	return this->m_directory;
}

bool XDirectory::Create()
{
	if (::CreateDirectoryW(this->m_directory.c_str(), nullptr))
	{
		this->m_exists = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool XDirectory::Move(const wchar_t* path)
{
	if (::MoveFileW(this->Path().c_str(), path))
	{
		this->m_directory = path;
		return true;
	}
	else
	{
		return false;
	}
}

XDirectory XDirectory::Folder(const wchar_t* name)
{
	std::wstring path = this->m_directory;
	path.append(name);

	return XDirectory(path.c_str());
}

XFile XDirectory::File(const wchar_t* name)
{
	std::wstring path = this->m_directory;
	path.append(name);

	return XFile(path.c_str());
}