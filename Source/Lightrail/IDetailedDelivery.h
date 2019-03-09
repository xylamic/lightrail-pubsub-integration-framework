/*!
@file	IDetailedDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef IDETAILEDDELIVERY_H
#define IDETAILEDDELIVERY_H

#include "Lightrail_global.h"
#include "ITerminal.h"
#include "IDelivery.h"

namespace Xylasoft
{
	/*!
	This provides the interface for getting detailed delivery information.
	*/
	class LIGHTRAILSHARED_EXPORT IDetailedDelivery
	{
	public:
		/*!
		Destroy the detailed delivery.
		*/
		virtual ~IDetailedDelivery() { }

		/*!
		Get the message as a string. Decoding will be attempted by using the
		included message encoding.
		*/
		virtual std::wstring MessageString() const = 0;

		/*!
		Get the message encoding.
		*/
		virtual Encoding::EncodingType Encoding() const = 0;

		/*!
		Get the message as a binary array.
		*/
		virtual const char* Message() const = 0;

		/*!
		Get the size of the binary message array.
		*/
		virtual long MessageSize() const = 0;

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

		/*!
		Get the Terminal this message was received on.
		*/
		virtual const ITerminal& SourceTerminal() const = 0;

		/*!
		Get the source port number.
		*/
		virtual long SourcePort() const = 0;

		/*!
		Get the source host name.
		*/
		virtual const char* SourceHost() const = 0;

		/*!
		Get the source application name.
		*/
		virtual const wchar_t* SourceApplication() const = 0;

		/*!
		Get the time the delivery was sent.
		*/
		virtual const time_t TimeUtc() const = 0;
	};
}

#endif