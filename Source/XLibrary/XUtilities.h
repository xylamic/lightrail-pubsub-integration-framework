/*!
@file	XUtilities.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XUTILITIES_H
#define XUTILITIES_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	This is a static class that provides general functionality.
	*/
	class XUtilities
	{
	public:
		/*!
		Compare two arrays of byte (or raw data) for equivelence.
		@param[in] data1 The first array of data.
		@param[in] data1Size The length of the first array.
		@param[in] data2 The second array of data.
		@param[in] data2Size The length of the second array.
		@returns True if the array are equal.
		*/
		static bool CompareByteArrays(const char* data1, long data1Size,
							   const char* data2, long data2Size);

		/*!
		Get the data folder for storage of data for Lightrail.
		@returns The full path to the folder.
		@throws IOException If there is a problem in creating the data directory.
		*/
		static std::wstring GetDataFolder();

		/*!
		Cause the current sleep to sleep for the specified amount of time.
		@param[in] timeout The time to sleep in milliseconds.
		*/
		static void ThreadSleep(long timeout);

		/*!
		Cause memory throttling to take place. The thread will be slowed based
		on the amount of waiting memory, between 0 and 20 seconds. There are categories
		indexed between 0 and 4, where 0 is no throttling and 4 is maximum throttling.
		@param[in] memorySize The memory usage currently
		@returns The throttling category index.
		*/
		static int MethodThrottling(long memorySize);

		/*!
		Convert from a std::wstring to std::string.
		@param[in] ws The std::wstring to convert.
		@returns The std::string format.
		*/
		static std::string GetStringFromWString(const std::wstring& ws);

		/*!
		Convert from a std::string to std::wstring.
		@param[in] s The std::string to convert.
		@returns The std::wstring format.
		*/
		static std::wstring GetWStringFromString(const std::string& s);

		/*!
		Trim a std::string.
		@param[in] s The string to trim.
		@returns The trimmed string.
		*/
		static std::string TrimString(const std::string& s);

		/*!
		Trim a std::wstring.
		@param[in] ws The string to trim.
		@returns The trimmed string.
		*/
		static std::wstring TrimWString(const std::wstring& ws);

		/*!
		Perform a case-insensitive compare of Strings.
		@param[in] s1 The first string.
		@param[in] s2 The second string.
		@returns 0 if strings are equal.
		*/
		static int CaseInsensitiveCompare(const std::string& s1, const std::string& s2);

		/*!
		Perform a case-insensitive compare of WStrings.
		@param[in] ws1 The first wstring.
		@param[in] ws2 The second wstring.
		@returns 0 if strings are equal.
		*/
		static int CaseInsensitiveCompare(const std::wstring& ws1, const std::wstring& ws2);

		/*!
		Remove a character from the given string.
		@param[in] ws The string to remove characters from.
		@param[in] c The character to remove.
		*/
		static void RemoveCharacterFromString(std::wstring& ws, wchar_t c);

		/*!
		Get bytes from a wstring (UCS-2 to UTF-8 bytes).
		@param[in] ws The wide string to convert.
		@returns The byte string.
		*/
		static std::string GetUTF8BytesFromString(const std::wstring& ws);

		/*!
		Get bytes from a wstring (UCS-2 to UTF-16 bytes).
		@param[in] ws The wide string to convert.
		@returns The byte string.
		*/
		static std::string GetUTF16BytesFromString(const std::wstring& ws);

		/*!
		Get wstring from byte string (bytes to UCS-2).
		@param[in] s The byte string to convert.
		@returns The wide string.
		*/
		static std::wstring GetStringFromUTF16Bytes(const std::string& s);

		/*!
		Get wstring from byte string (bytes to UCS-2).
		@param[in] s The byte string to convert.
		@returns The wide string.
		*/
		static std::wstring GetStringFromUTF8Bytes(const std::string& s);

		/*!
		Convert the given hexidecimal string to an array of bytes.
		@param[in] s The string to convert.
		@param[in] bytes The array to load the bytes into.
		*/
		static void ConvertHexStringToByte(std::string& s, std::vector<char>& bytes);

		/*!
		Generate a pseudo-random number.
		@returns The number.
		*/
		static int GenerateRandomNumber();

		// make all these methods private, this is a static class
	private:
		XUtilities();
		XUtilities& operator=(const XUtilities&);
		XUtilities(const XUtilities& other);

	private:
		static const double m_doubleDiffValue;
	};
}
#endif // UTILITIES_H
