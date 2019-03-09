/*!
@file	IDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef IDELIVERY_H
#define IDELIVERY_H

#include "Lightrail_global.h"
#include "Method.h"

namespace Xylasoft
{
	/*!
	This class provides the interface for a delivery.
	*/
	class LIGHTRAILSHARED_EXPORT IDelivery
	{
	public:
		/*!
		Destroy the instance.
		*/
		virtual ~IDelivery() { }

		/*!
		Get the message as a string. Will try to decode the data based on the included EncodingType.
		@returns The message string.
		*/
		virtual std::wstring MessageString() const = 0;

		/*!
		Set the message body with a string. The string will be encoded as UTF-8.
		@param[in] message The message to set.
		*/
		virtual void SetMessageString(const std::wstring& message) = 0;

		/*!
		Set the message with a binary array.
		@param[in] data The binary array.
		@param[in] dataSize The size of the array.
		@param[in] encoding The encoding for the data.
		*/
		virtual void SetMessage(const char* data, long dataSize, Encoding::EncodingType encoding = Encoding::BINARY) = 0;

		/*!
		Get the type of encoding for this message.
		@returns The encoding type.
		*/
		virtual Encoding::EncodingType Encoding() const = 0;

		/*!
		Get the message binary array.
		@returns The binary array.
		*/
		virtual const char* Message() const = 0;

		/*!
		Get the message size.
		@returns The message size.
		*/
		virtual long MessageSize() const = 0;

		/*!
		Set or add a label with the given value. If value is NULL, the
		label is removed.
		@param[in] name The label name.
		@param[in] value The label value.
		*/
		virtual void SetLabel(const wchar_t* name, const wchar_t* value) = 0;

		/*!
		Get the specified label value.
		@param[in] name The name of the label.
		@returns The value for the label, or nullptr if not found.
		*/
		virtual const wchar_t* Label(const wchar_t* name) const = 0;

		/*!
		Get the number of labels present.
		@returns The number of labels.
		*/
		virtual long GetNumberOfLabels() const = 0;

		/*!
		Get the label name for the specified index.
		@param[in] index The index of the name to retrieve.
		@returns The name for the given index, or NULL if invalid.
		*/
		virtual const wchar_t* GetLabelName(long index) const = 0;
	};
}

#endif