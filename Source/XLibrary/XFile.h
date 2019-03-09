/*!
@file	XFile.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XFILE_H
#define XFILE_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	This defines a file, providing information about it and operations on it.
	*/
	class XFile
	{
	public:
		/*!
		Construct an XFile from the specified path.
		@param[in] path The file path.
		*/
		XFile(const wchar_t* path);

		/*!
		Get whether the file exists.
		@returns True if exists.
		*/
		bool Exists();

		/*!
		Delete this file.
		*/
		bool Delete();

		/*!
		Get the directory path for the file.
		@returns The directory path.
		*/
		std::wstring Directory();

		/*!
		Get the name of the file.
		@returns The file name.
		*/
		std::wstring Name();

		/*!
		Get the full path to the file.
		@returns The path.
		*/
		std::wstring Path();

		/*!
		Move the file to the specified directory. Changes this object
		to point to the file in the new path.
		@param[in] directorypath The path to the new directory.
		@returns True if succeeded.
		*/
		bool Move(const wchar_t* directorypath);

		/*!
		If this file does not exist, copy the contents of another file to create this file.
		@param[in] file The file to copy.
		@returns True if succeeded.
		*/
		bool CopyFrom(const wchar_t* filepath);

		/*!
		Rename the specified file name.
		@param[in] name The new file name.
		@returns True if succeeded.
		*/
		bool Rename(const wchar_t* name);

		/*!
		Deletes this file, but also shreds it so it can never be recovered.
		*/
		void Shred();

	private:
		bool m_exists;
		std::wstring m_directory;
		std::wstring m_filename;
	};
}

#endif