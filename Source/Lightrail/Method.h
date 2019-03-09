/*!
@file	Method.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef METHOD_H
#define METHOD_H

#include "Lightrail_global.h"

namespace Xylasoft
{
	/*!
	This enumeration defines the types of encoding.
	*/
	class LIGHTRAILSHARED_EXPORT Encoding
	{
	public:
		/*!
		This enumeration defines the types of encoding.
		*/
		enum EncodingType
		{
			/*! Binary encoding (none). */
			BINARY = 0,

			/*! ASCII encoding. */
			ASCII = 1,

			/*! UTF-8 encoding. */
			UTF8 = 2,

			/*! UTF-16 encoding. */
			UTF16 = 3,

			/*! UTF-32 encoding */
			UTF32 = 4
		};

	private:
		Encoding();
		Encoding(const Encoding&);
	};

	/*!
	This defines the types of security supported.
	*/
	class LIGHTRAILSHARED_EXPORT Security
	{
	public:
		/*!
		This enumeration defines the types of security supported.
		*/
		enum SecurityType
		{
			/*! No security. */
			NONE = 0,

			/*! AES 256-bit security. */
			AES_256 = 1
		};

	private:
		Security();
		Security(const Security&);
	};

	/*!
	This enumeration defines the different standpoints a Terminal can take.
	*/
	class LIGHTRAILSHARED_EXPORT Standpoint
	{
	public:
		/*!
		This enumeration defines the different standpoints a Terminal can take.
		*/
		enum StandpointType
		{
			/*! Publish messages. */
			PUBLISHER = 1,
			
			/*! Subscribe to messages. */
			SUBSCRIBER = 2,
			
			/*! Respond to messages. */
			RESPONDER = 3,

			/*! Send requests for a response. */
			REQUESTER = 4
		};
	};
}

#endif