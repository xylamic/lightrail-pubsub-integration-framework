/*!
@file	XUuid.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XUUID_H
#define XUUID_H

#include "Xylasoft.h"
#include "XByteArray.h"

namespace Xylasoft
{
	/*!
	This class provides UUID management and generation of version 4 universally unique
	identifiers.
	*
	The UUID format followed the standard of XXXXXXXX-XXXX-4XXX-YXXX-XXXXXXXX, where Y
	is of the range 8-B.
	*/
	class XUuid
	{
	public:
		/*!
		Construct a UUID from a UUID hexidecimal string.
		@param[in] uuidstr The UUID string.
		*/
		XUuid(std::string uuidstr);

		/*!
		Construct a UUID from an array of 16 bytes.
		@param[in] uuid The array of 16 bytes.
		*/
		XUuid(const char* uuid);

		/*!
		Copy a UUID.
		@param[in] xuuid The UUID to copy.
		*/
		XUuid(const XUuid& xuuid);

		/*!
		Check if this UUID is less than another.
		@param[in] xuuid The UUID to compare.
		@returns True if less than.
		*/
		bool operator<(const XUuid& xuuid) const;

		/*!
		Check if this UUID is equal to another.
		@param[in] xuuid The UUID to compare.
		@returns True if they are equal.
		*/
		bool operator==(const XUuid& xuuid) const;

		/*!
		Assignment another UUID value to this one.
		@param[in] xuuid The UUID to copy.
		@returns This instance for chaining.
		*/
		XUuid& operator=(const XUuid& xuuid);

		/*!
		Get the UUID formatted string.
		@returns The UUID string.
		*/
		std::string ToString() const;

		/*!
		Get the UUID formatted wstring.
		@returns The UUID string.
		*/
		std::wstring ToWString() const;

		/*!
		Check whether this UUID is empty.
		@returns True if empty.
		*/
		bool IsEmpty() const;

		/*!
		Create an empty UUID.
		*
		00000000-0000-0000-0000-000000000000
		@returns The empty UUID.
		*/
		static XUuid CreateEmpty();

		/*!
		Create a new, unique UUID.
		@returns A new UUID.
		*/
		static XUuid CreateNew();

	private:
		XByteArray m_uuid;
	};
}

#endif