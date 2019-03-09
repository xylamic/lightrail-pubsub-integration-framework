/*!
@file	XIni.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XINI_H
#define XINI_H

#include "Xylasoft.h"
#include <list>

namespace Xylasoft
{
	/*!
	This provides the facilities for reading and writing an INI file. This supports
	single values with an associated key, and comments on their own lines starting 
	with ; or #. Currently, comments cannot be on the same line as a section name 
	or key/value pair.
	*/
	class XIni
	{
	public:
		/*!
		Construct an empty file.
		*/
		XIni();

		/*!
		Construct an object from the specified file.
		*/
		XIni(const wchar_t* filepath);

		/*!
		Save the contents of the file.
		@param[in] filepath The path to save the file.
		*/
		void Save(const wchar_t* filepath) const;

		/*!
		Get the list of keys in the specified section.
		@param[in] section The section to look up.
		@returns The list of keys in the section.
		*/
		std::list<std::wstring> GetKeysForSection(const wchar_t* section) const;

		/*!
		Get the value for the specified key.
		@param[in] section The section to lookup.
		@param[in] key The key to lookup.
		@returns The value found or an empty string if not found.
		*/
		std::wstring GetValueForKey(const wchar_t* section, const wchar_t* key) const;

		/*!
		Set the value for the specified section and key.
		@param[in] section The section of the value.
		@param[in] key The key of the value.
		@param[in] value The value to write.
		*/
		void SetKeyValueForSection(const wchar_t* section, const wchar_t* key, const wchar_t* value);

		/*!
		Remove the specified key.
		@param[in] section The section of the key.
		@param[in] key The key to remove in the section.
		*/
		void RemoveKey(const wchar_t* section, const wchar_t* key);

	protected:
		/*!
		Verify that the given section name is valid.
		@param[in] section The section name.
		*/
		void ValidateSectionName(const wchar_t* section) const;

		/*!
		Verify that the given key/value name is valid.
		@param[in] name The key/value name.
		*/
		void ValidateKeyValueName(const wchar_t* name) const;

		/*!
		Create the line for a section.
		@param[in] section The section name.
		@returns The section line.
		*/
		std::wstring CreateSectionLine(const wchar_t* section) const;

		/*!
		Create the line for a key/value pair.
		@param[in] key The key.
		@param[in] value The value.
		@returns The key/value line.
		*/
		std::wstring CreateKeyValueLine(const wchar_t* key, const wchar_t* value) const;

		/*!
		Get whether the given line contains a key/value pair.
		@param[in] line The line to parse.
		@param[out] key The key that was parsed.
		@param[out] value The value that was parsed.
		@returns True if this was a key/value line, false otherwise.
		*/
		bool IsKey(const std::wstring& line, std::wstring& key, std::wstring& value) const;

		/*!
		Get whether the given line contains a section pair.
		@param[in] line The line to parse.
		@param[out] section The section that was parsed.
		@returns True if this was a section line, false otherwise.
		*/
		bool IsSection(const std::wstring& line, std::wstring& sectionname) const;

	private:
		std::list<std::wstring> m_content;
	};
}

#endif