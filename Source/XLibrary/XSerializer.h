/*!
@file	XSerializer.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XSERIALIZER_H
#define XSERIALIZER_H

#include "Xylasoft.h"
#include "XByteArray.h"
#include <cstdint>

#define XSERIALIZER_VERSION_10 1

namespace Xylasoft
{
	/*!
	Provider of basic, cross-platform serialization functionality. The data produced from
	this serializer can be read and written from any platform that supports the code base.
	*/
	class XSerializer
	{
	public:
		/*!
		Uses the given array to serialize information and initializes in write mode. No read operations can
		be performed (ie: << only).
		*/
		XSerializer(XByteArray& data);

		/*!
		Intiailizers a serializer in read mode. No write operations can
		be performed (ie: >> only).
		@param[in] data The byte array to read from.
		*/
		XSerializer(const XByteArray* data);

		// writing members
	public:
		/*!
		Write a 16-bit unsigned integer.
		@param[in] value The integer to write.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(uint16_t value);

		/*!
		Write a 32-bit unsigned integer.
		@param[in] value The integer to write.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(uint32_t value);

		/*!
		Write a 32-bit signed integer.
		@param[in] value The integer to write.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(int32_t value);

		/*!
		Write a double value.
		@param[in] value The double to write.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(const double& value);

		/*!
		Write a binary string. This will write the full size of the std::string
		irrespective of whether it contains null characters in the string.
		@param[in] value The binary string to write.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(const std::string& value);

		/*!
		Write a wide character string. The string will be converted to UTF-16 bytes
		and then written to the serializer.
		@param[in] value The wide character string.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(const std::wstring& value);

		/*!
		Write a binary array.
		@param[in] value The binary array to write.
		@returns The serializer for chaining.
		*/
		XSerializer& operator<<(const XByteArray& value);

		// reading members
	public:
		/*!
		Read a 16-bit unsigned integer.
		@param[in] value The integer to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(uint16_t& value);

		/*!
		Read a 32-bit unsigned integer.
		@param[in] value The integer to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(uint32_t& value);

		/*!
		Read a 32-bit signed integer.
		@param[in] value The integer to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(int32_t& value);

		/*!
		Read a double value.
		@param[in] value The double to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(double& value);

		/*!
		Read a binary string. This can be used to read anything written as a std::string,
		std::wstring, or binary array.
		@param[in] value The string to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(std::string& value);

		/*!
		Read a wide character string. This reads the UTF-16 byte array from the array and
		converts to a wide character string.
		@param[in] value The wide character string to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(std::wstring& value);

		/*!
		Read a binary array.
		@param[in] value The array to read to.
		@returns The serializer for chaining.
		*/
		XSerializer& operator>>(XByteArray& value);

	protected:
		/*!
		Validate that this instance is in read mode and that there is enough size left in the array to
		read the specified size.
		@param[in] size The size intended to read.
		*/
		void ValidateReadMode(int size);

		/*!
		Validate that this instance is in write mode.
		*/
		void ValidateWriteMode();

	private:
		XSerializer(const XSerializer&);

	private:
		bool m_isReading;
		int m_version;

		long m_readerPtr;
		const XByteArray* m_dataReader;

		XByteArray* m_dataWriter;

	};
}

#endif