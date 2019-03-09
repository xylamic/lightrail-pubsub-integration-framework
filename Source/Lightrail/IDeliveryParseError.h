/*!
@file	IDeliveryParseError.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef IDELIVERYPARSEERROR_H
#define IDELIVERYPARSEERROR_H

#include "Lightrail_global.h"
#include "ITerminal.h"

namespace Xylasoft
{
	/*!
	This provides the interface for a delivery parse error.
	*/
	class LIGHTRAILSHARED_EXPORT IDeliveryParseError
	{
	public:
		/*!
		This enumeration defines the types of parse errors.
		*/
		enum ParseErrorType
		{
			/*! No specific error. */
			NONE = 0,

			/*! The data is corrupted. */
			CORRUPTION = 1,

			/*! The decryption failed. */
			DECRYPTION = 2,

			/*! The security types were mismatched. */
			SECURITY_MISMATCH = 3
		};

	public:
		/*!
		Destroy the parse error.
		*/
		virtual ~IDeliveryParseError() { }

		/*!
		Get the route the message was arriving from.
		@returns The Terminal of arrival.
		*/
		virtual const ITerminal* Route() const = 0;

		/*!
		Get the source application name.
		*/
		virtual const wchar_t* SourceApplication() const = 0;

		/*!
		Get the source host name.
		*/
		virtual const char* SourceHostname() const = 0;

		/*!
		Get the source port number.
		*/
		virtual long SourcePort() const = 0;

		/*!
		Get the error type that occurred.
		*/
		virtual ParseErrorType ErrorType() const = 0;

		/*!
		Get the exception that occurred.
		*/
		virtual const std::exception& Message() const = 0;
	};
}

#endif