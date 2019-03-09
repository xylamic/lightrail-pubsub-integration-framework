/*!
@file	XIni.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XIni.h"
#include "XException.h"
#include "XUtilities.h"
#include <fstream>

using namespace Xylasoft;

XIni::XIni()
{
}

XIni::XIni(const wchar_t* filepath)
{
	std::wifstream file;
	file.open(filepath, std::ios::in);

	// try to open the file
	if (!file.is_open())
	{
		throw IOException(L"The file could not be opened for read.");
	}

	// read the whole file
	std::wstring filetext;
	wchar_t c = file.get();
	while (!file.eof())
	{
		filetext.push_back(c);
		c = file.get();
	}

	// go ahead and close the file
	file.close();

	// iterate through the file and break it up
	size_t index = 0;
	std::wstring line;
	while (index < filetext.size())
	{
		// if we get a carriage return
		if (filetext[index] == '\r')
		{
			// if it is followed by a new line, skip over the new line as a single return
			if (filetext.size() < index + 1 && filetext[index + 1] != '\n')
			{
				index++;
			}

			this->m_content.push_back(line);
			line.clear();
		}
		// if we get a new line
		else if (filetext[index] == '\n')
		{
			// if it is followed by a carriage return, skip over the return as a single return
			if (filetext.size() < index + 1 && filetext[index + 1] != '\r')
			{
				index++;
			}

			this->m_content.push_back(XUtilities::TrimWString(line));
			line.clear();
		}
		// else this is a regular character and should be added
		else
		{
			line.push_back(filetext[index]);
		}

		// move to the next character
		index++;
	}

	// if we reached the eof the file with some characters left, write them
	if (line.size() > 0)
	{
		this->m_content.push_back(XUtilities::TrimWString(line));
	}
}

void XIni::Save(const wchar_t* filepath) const
{
	std::wofstream file;
	file.open(filepath, std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		throw IOException(L"The file could not be opened for write.");
	}

	for (std::list<std::wstring>::const_iterator it = this->m_content.cbegin();
		it != this->m_content.cend(); it++)
	{
		file.write(it->c_str(), it->size());
		file.write(L"\n", 1);
	}

	file.close();
}

std::list<std::wstring> XIni::GetKeysForSection(const wchar_t* section) const
{
	this->ValidateSectionName(section);

	bool inSection = false;
	std::wstring currentSection, key, value;
	std::list<std::wstring> keys;

	for (std::list<std::wstring>::const_iterator it = this->m_content.cbegin();
		it != this->m_content.cend(); it++)
	{
		if (this->IsSection(*it, currentSection))
		{
			if (currentSection.compare(section) == 0)
			{
				inSection = true;
			}
			else
			{
				inSection = false;
			}
		}
		else if (inSection && this->IsKey(*it, key, value))
		{
			keys.push_back(key);
		}
	}

	return keys;
}

std::wstring XIni::GetValueForKey(const wchar_t* section, const wchar_t* key) const
{
	this->ValidateSectionName(section);
	this->ValidateKeyValueName(key);

	bool inSection = false;
	std::wstring currentSection, currentKey, currentValue;

	for (std::list<std::wstring>::const_iterator it = this->m_content.cbegin();
		it != this->m_content.cend(); it++)
	{
		if (this->IsSection(*it, currentSection))
		{
			if (currentSection.compare(section) == 0)
			{
				inSection = true;
			}
			else
			{
				inSection = false;
			}
		}
		else if (inSection && this->IsKey(*it, currentKey, currentValue) && currentKey.compare(key) == 0)
		{
			return currentValue;
		}
	}

	return L"";
}

void XIni::SetKeyValueForSection(const wchar_t* section, const wchar_t* key, const wchar_t* value)
{
	this->ValidateSectionName(section);
	this->ValidateKeyValueName(key);
	this->ValidateKeyValueName(value);

	std::list<std::wstring>::iterator secit = this->m_content.end();
	std::wstring currentSection, currentKey, currentValue;

	for (std::list<std::wstring>::iterator it = this->m_content.begin();
		it != this->m_content.end(); it++)
	{
		if (this->IsSection(*it, currentSection))
		{
			// if the section found was the section we're looking for
			if (currentSection.compare(section) == 0)
			{
				secit = it;
			}
			else
			{
				// if we found a section and we previously found the section we were looking for,
				// then the key/value needs to be added to the previous section
				if (secit != this->m_content.end())
				{
					this->m_content.insert(++secit, this->CreateKeyValueLine(key, value));
					return;
				}
			}
		}
		// if this is the key we're looking for inside the section, set the value
		else if (secit != this->m_content.end() && this->IsKey(*it, currentKey, currentValue) && currentKey.compare(key) == 0)
		{
			*it = this->CreateKeyValueLine(key, value);
			return;
		}
	}

	// if we never saw the section, then add the section
	if (secit == this->m_content.end())
	{
		this->m_content.push_back(this->CreateSectionLine(section));
	}

	// whether the section exists prior or not, it now exists as the last section
	// in the file, add the key/value
	this->m_content.push_back(this->CreateKeyValueLine(key, value));
}

void XIni::RemoveKey(const wchar_t* section, const wchar_t* key)
{
	this->ValidateSectionName(section);
	this->ValidateKeyValueName(key);

	bool inSection = false;
	std::wstring currentSection, currentKey, currentValue;

	for (std::list<std::wstring>::iterator it = this->m_content.begin();
		it != this->m_content.end(); it++)
	{
		if (this->IsSection(*it, currentSection))
		{
			if (currentSection.compare(section) == 0)
			{
				inSection = true;
			}
			else
			{
				inSection = false;
			}
		}
		else if (inSection && this->IsKey(*it, currentKey, currentValue) && currentKey.compare(key) == 0)
		{
			this->m_content.erase(it);
			return;
		}
	}
}

void XIni::ValidateSectionName(const wchar_t* section) const
{
	std::wstring sectionname = section;
	if (sectionname.find(L'[') != sectionname.npos || sectionname.find(L']') != sectionname.npos)
	{
		throw ArgumentException(L"The section cannot contain brackets.");
	}
}

void XIni::ValidateKeyValueName(const wchar_t* name) const
{
	std::wstring kvname = name;
	if (kvname.find(L'=') != kvname.npos)
	{
		throw ArgumentException(L"The name cannot contain equal sign.");
	}
}

std::wstring XIni::CreateSectionLine(const wchar_t* section) const
{
	std::wstring sectionline;
	sectionline.append(L"[").append(section).append(L"]");
	return sectionline;
}

std::wstring XIni::CreateKeyValueLine(const wchar_t* key, const wchar_t* value) const
{
	std::wstring keyline;
	keyline.append(key).append(L"=").append(value);
	return keyline;
}

bool XIni::IsKey(const std::wstring& line, std::wstring& key, std::wstring& value) const
{
	if (line.size() == 0 || line[0] == L';' || line[0] == L'#' || line[0] == L'[')
	{
		return false;
	}
			
	size_t index = line.find(L'=');
	if (index == line.npos)
	{
		return false;
	}

	key = XUtilities::TrimWString(line.substr(0, index));

	value = XUtilities::TrimWString(line.substr(index + 1));
	if (value.size() > 1 && value[0] == L'\"' && value[value.size() - 1] == L'\"')
	{
		value.erase(0, 1);
		value.erase(value.size() - 1, 1);
	}
			
	return true;
}

bool XIni::IsSection(const std::wstring& line, std::wstring& sectionname) const
{
	if (line.size() > 1 && line[0] == L'[' && line[line.size() - 1] == L']')
	{
		sectionname = line.substr(1, line.size() - 2);
		return true;
	}
	else
	{
		return false;
	}
}