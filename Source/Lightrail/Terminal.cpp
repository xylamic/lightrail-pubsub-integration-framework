/*!
@file	Terminal.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Terminal.h"
#include <XException.h>
#include <XUtilities.h>
#include <XSerializer.h>
#include <vector>

#define FORMAT_V1 0

using namespace Xylasoft;

static void ParseTerminalString(const wchar_t* dir, bool& openEnd, bool& containsNonWild, std::vector<std::wstring>& components)
{
	std::wstring dirstr = dir;

	openEnd = false;
	if (dirstr[dirstr.size() - 1] == L'>')
	{
		openEnd = true;
	}

	// get the topic body
	std::wstring dirstrbody = dirstr.substr(1, dirstr.size() - 2);

	// get the topic components
	components.clear();
	containsNonWild = false;
	size_t index = 0;
	std::wstring currentComp;
	while (index < dirstrbody.size())
	{
		if (dirstrbody[index] != L'>')
		{
			currentComp.push_back(dirstrbody[index]);
		}
		else
		{
			if (currentComp.compare(L">") != 0)
			{
				containsNonWild = true;
			}
			
			components.push_back(currentComp);
			currentComp.clear();
		}

		index++;
	}

	components.push_back(currentComp);
	if (currentComp.compare(L">") != 0)
	{
		containsNonWild = true;
	}
}

Terminal::Terminal()
{
}

Terminal::Terminal(const wchar_t* name, const wchar_t* description, const wchar_t* terminal, const wchar_t* securityKey)
{
	ValidateTerminalString(terminal);

	if (name == nullptr || name[0] == '\0')
	{
		throw ArgumentException(L"The name was empty.");
	}

	this->m_terminal = terminal;
	this->m_name = name;
	this->m_description = (description == nullptr ? L"" : description);
	this->m_securityKey = (securityKey == nullptr ? L"" : securityKey);
}

Terminal::Terminal(XSerializer& serializer)
{
	uint16_t version;
	serializer >> version;

	if (version != FORMAT_V1)
	{
		throw DataException(L"The terminal format version was not recognized.");
	}

	serializer >> this->m_name;
	serializer >> this->m_description;
	serializer >> this->m_terminal;
	serializer >> this->m_securityKey;
}

void Terminal::Serialize(XSerializer& serializer) const
{
	serializer << static_cast<uint16_t>(FORMAT_V1);

	serializer << this->m_name;
	serializer << this->m_description;
	serializer << this->m_terminal;
	serializer << this->m_securityKey;
}

bool Terminal::operator==(const Terminal& other) const
{
	if (this->TerminalCompare(*this, other) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Terminal::operator!=(const Terminal& other) const
{
	return !(*this == other);
}

const wchar_t* Terminal::TerminalString() const
{
	return this->m_terminal.c_str();
}

const wchar_t* Terminal::Name() const
{
	return this->m_name.c_str();
}

const wchar_t* Terminal::Description() const
{
	return this->m_description.c_str();
}

const wchar_t* Terminal::SecurityKeyName() const
{
	if (this->m_securityKey.size() > 0)
	{
		return this->m_securityKey.c_str();
	}
	else
	{
		return nullptr;
	}
}

void Terminal::ValidateTerminalString(const wchar_t* terminal)
{
	if (terminal == nullptr)
	{
		throw ArgumentException(L"The terminal string cannot be null.");
	}

	std::wstring dirstr = terminal;

	if (dirstr.size() < 3)
	{
		throw ArgumentException(L"The minimum terminal length was not met.");
	}

	if (dirstr[0] != L'[')
	{
		throw ArgumentException(L"The terminal must begin with open character '['.");
	}

	if (dirstr[dirstr.size() - 1] != L']' && dirstr[dirstr.size() - 1] != L'>')
	{
		throw ArgumentException(L"The terminal must end with close character ']' or '>'.");
	}

	if (dirstr[dirstr.size() - 1] == L']' && dirstr[dirstr.size() - 2] == L'>')
	{
		throw ArgumentException(L"The terminal cannot end with both '>' and ']'. It must either be open with '>' or closed with ']'.");
	}

	bool diropen, dirnonwild;
	std::vector<std::wstring> components;

	ParseTerminalString(terminal, diropen, dirnonwild, components);

	if (dirnonwild == false)
	{
		throw ArgumentException(L"The terminal must contain at least one literal.");
	}
}

int Terminal::TerminalCompare(const Terminal& d1, const Terminal& d2)
{
	bool d1open, d2open, d1nonwild, d2nonwild;
	std::vector<std::wstring> d1comp, d2comp;

	ParseTerminalString(d1.TerminalString(), d1open, d1nonwild, d1comp);
	ParseTerminalString(d2.TerminalString(), d2open, d2nonwild, d2comp);

	std::vector<std::wstring>::const_iterator it1 = d1comp.cbegin();
	std::vector<std::wstring>::const_iterator it2 = d2comp.cbegin();

	while (it1 != d1comp.cend() && it2 != d2comp.cend())
	{
		if (it1->size() == 0 || it2->size() == 0)
		{
			// component matches
		}
		else
		{
			int compare = XUtilities::CaseInsensitiveCompare(*it1, *it2);
			if (compare != 0)
			{
				return compare;
			}
			else
			{
				// component matches
			}
		}

		it1++;
		it2++;
	}

	if (it1 == d1comp.cend() && it2 == d2comp.cend())
	{
		return 0;
	}
	else if (it1 == d1comp.cend() && d1open)
	{
		return 0;
	}
	else if (it2 == d2comp.cend() && d2open)
	{
		return 0;
	}
	else if (it1 == d1comp.cend())
	{
		return -1;
	}
	else
	{
		return 1;
	}
}