/*!
@file	XDirectory.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XDIRECTORY_H
#define XDIRECTORY_H

#include "Xylasoft.h"
#include "XFile.h"

namespace Xylasoft
{
	/*!
	Container of a file directory.
	*/
	class XDirectory
	{
	public:
		/*!
		Construct a directory object from a specified path.
		@param[in] path The path of the directory.
		*/
		XDirectory(const wchar_t* path);

		/*!
		Get whether the directory exists.
		*/
		bool Exists();

		/*!
		Get the path to the directory.
		*/
		std::wstring Path();

		/*!
		Create the directory.
		@returns True if created, otherwise false.
		*/
		bool Create();

		/*!
		Move the directory to the specified path.
		@param[in] path The path to move to.
		@returns True if successful.
		*/
		bool Move(const wchar_t* path);

		/*!
		Get the directory object for a directory contained in this directory.
		@param[in] name The name of the contained directory.
		@returns The child directory.
		*/
		XDirectory Folder(const wchar_t* name);

		/*!
		Get the file object for a file contained in this directory.
		@param[in] name The name of the contained file.
		@returns The child file.
		*/
		XFile File(const wchar_t* name);

	private:
		bool m_exists;
		std::wstring m_directory;
	};
}

#endif