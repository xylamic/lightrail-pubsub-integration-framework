/*!
@file	ISecurityKey.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef ISECURITYKEY_H
#define ISECURITYKEY_H

#include "Lightrail_global.h"
#include "Method.h"

namespace Xylasoft
{
	/*!
	This defines a security for use with encryption and decryption of messages that match key names.
	*/
	class ISecurityKey
	{
	public:
		/*!
		Virtual destructor.
		*/
		virtual ~ISecurityKey() { }

		/*!
		Get the name of the security key.
		*/
		virtual const wchar_t* Name() const = 0;

		/*!
		Get the type of security.
		*/
		virtual Security::SecurityType Type() const = 0;
	};
}

#endif