/*!
@file	XUtilities.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XUtilities.h"
#include "XDirectory.h"
#include "XException.h"
#include <locale>
#include <algorithm>
#include <wctype.h>
#include <sstream>

using namespace Xylasoft;

bool XUtilities::CompareByteArrays(const char* data1, long data1Size,
                       const char* data2, long data2Size)
{
    if (data1Size != data2Size)
    {
        return false;
    }

    for (int i = 0; i < data1Size; i++)
    {
        if (data1[i] != data2[i])
        {
            return false;
        }
    }

    return true;
}

std::wstring XUtilities::GetDataFolder()
{
	XDirectory dir(_wgetenv(L"ALLUSERSPROFILE"));
	if (!dir.Exists())
    {
		if (!dir.Create())
        {
            throw IOException(L"Failed to make application data folder.");
        }
    }

	XDirectory dirX = dir.Folder(L"Xylasoft");
	if (!dirX.Exists())
    {
		if (!dirX.Create())
        {
            throw IOException(L"Failed to make application data specific folder.");
        }
    }

	return dirX.Path();
}

void XUtilities::ThreadSleep(long timeout)
{
	::Sleep(timeout);
}

int XUtilities::MethodThrottling(long memorySize)
{
	/*
	1MB = 1048576 bytes
	*/

	static long oneMegabyte = 1048576;
	static long thirtyMegabytes = 30 * oneMegabyte;
	static long fiftyMegabytes = 40 * oneMegabyte;
	static long eightyMegabytes = 80 * oneMegabyte;
	static long oneHundredTwentyMegabytes = 120 * oneMegabyte;

	if (memorySize < thirtyMegabytes)
	{
		return 0;
	}
	else if (memorySize < fiftyMegabytes)
	{
		ThreadSleep(500);
		return 1;
	}
	else if (memorySize < eightyMegabytes)
	{
		ThreadSleep(2000);
		return 2;
	}
	else if (memorySize < oneHundredTwentyMegabytes)
	{
		ThreadSleep(5000);
		return 3;
	}
	else
	{
		ThreadSleep(20000);
		return 4;
	}

	return true;
}

std::string XUtilities::GetStringFromWString(const std::wstring& ws)
{
	std::string temp(ws.length(), ' ');
	std::copy(ws.begin(), ws.end(), temp.begin());
	return temp;
}

std::wstring XUtilities::GetWStringFromString(const std::string& s)
{
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

std::string XUtilities::TrimString(const std::string& s)
{
	int startIndex = 0;
	while (startIndex < static_cast<int>(s.size()) && ::isspace(s[startIndex]))
	{
		startIndex++;
	}

	int endIndex = static_cast<int>(s.size()) - 1;
	while (endIndex > 0 && ::isspace(s[endIndex]))
	{
		endIndex--;
	}

	if (endIndex - startIndex >= 0)
	{
		return s.substr(startIndex, endIndex - startIndex + 1);
	}

	return "";
}

std::wstring XUtilities::TrimWString(const std::wstring& ws)
{
	int startIndex = 0;
	while (startIndex < static_cast<int>(ws.size()) && ::isspace(ws[startIndex]))
	{
		startIndex++;
	}

	int endIndex = static_cast<int>(ws.size()) - 1;
	while (endIndex > 0 && ::isspace(ws[endIndex]))
	{
		endIndex--;
	}

	if (endIndex - startIndex >= 0)
	{
		return ws.substr(startIndex, endIndex - startIndex + 1);
	}

	return L"";
}

int XUtilities::CaseInsensitiveCompare(const std::wstring& ws1, const std::wstring& ws2)
{
	std::wstring wsl1 = ws1;
	std::wstring wsl2 = ws2;

	std::transform(wsl1.begin(), wsl1.end(), wsl1.begin(), ::towlower);
	std::transform(wsl2.begin(), wsl2.end(), wsl2.begin(), ::towlower);

	return wsl1.compare(wsl2);
}

int XUtilities::CaseInsensitiveCompare(const std::string& s1, const std::string& s2)
{
	std::string sl1 = s1;
	std::string sl2 = s2;

	std::transform(sl1.begin(), sl1.end(), sl1.begin(), ::tolower);
	std::transform(sl2.begin(), sl2.end(), sl2.begin(), ::tolower);

	return sl1.compare(sl2);
}

void XUtilities::RemoveCharacterFromString(std::wstring& ws, wchar_t c)
{
	size_t index = 0;
	while (index < ws.size())
	{
		if (ws[index] == c)
		{
			ws.erase(index, 1);
		}
		else
		{
			index++;
		}
	}
}

std::string XUtilities::GetUTF8BytesFromString(const std::wstring& ws)
{
	static std::wstring_convert< std::codecvt_utf8 < wchar_t > > utf8conv;
	return utf8conv.to_bytes(ws);
}

std::string XUtilities::GetUTF16BytesFromString(const std::wstring& ws)
{
	static std::wstring_convert< std::codecvt_utf16 < wchar_t > > utf16conv;
	return utf16conv.to_bytes(ws);
}

std::wstring XUtilities::GetStringFromUTF16Bytes(const std::string& s)
{
	static std::wstring_convert< std::codecvt_utf16 < wchar_t > > utf16conv;
	return utf16conv.from_bytes(s);
}

std::wstring XUtilities::GetStringFromUTF8Bytes(const std::string& s)
{
	static std::wstring_convert< std::codecvt_utf8 < wchar_t > > utf8conv;
	return utf8conv.from_bytes(s);
}

void XUtilities::ConvertHexStringToByte(std::string& s, std::vector<char>& bytes)
{
	long length = static_cast<long>(s.length());

	// make sure the input string has an even digit numbers
	if(length%2 == 1)
	{
		s = "0" + s;
		length++;
	}

	// allocate memory for the output array
	bytes.reserve(length/2);
	
	std::stringstream ss(s);
	for(int i=0; i < length/2; i++)
	{
		char ch1, ch2;
		ss >> ch1 >> ch2;

		int dig1, dig2;
		if(isdigit(ch1)) dig1 = ch1 - '0';
		else if(ch1>='A' && ch1<='F') dig1 = ch1 - 'A' + 10;
		else if(ch1>='a' && ch1<='f') dig1 = ch1 - 'a' + 10;
		if(isdigit(ch2)) dig2 = ch2 - '0';
		else if(ch2>='A' && ch2<='F') dig2 = ch2 - 'A' + 10;
		else if(ch2>='a' && ch2<='f') dig2 = ch2 - 'a' + 10;

		bytes.push_back(dig1*16 + dig2);
	}
}

int XUtilities::GenerateRandomNumber()
{
	// verify the randon number generator has been initialized
	static bool randInitialized = false;
	if (!randInitialized)
	{
		::srand(static_cast<unsigned int>(::time(nullptr)));
		randInitialized = true;
	}

	return ::rand();
}