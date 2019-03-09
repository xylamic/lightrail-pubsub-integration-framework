/*!
@file	ITerminal.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef ITERMINAL_H
#define ITERMINAL_H

#include "Lightrail_global.h"
#include "Method.h"

namespace Xylasoft
{
	/*!
	  Representation of a terminal for a delivery to travel to/from. This provides the information
	  that allows the infrastructure to decide who information goes to.
	  *
	  Example: [Xylasoft>Infrastructure>Business>Test] matches [Xylasoft>>Business>
	*/
	class LIGHTRAILSHARED_EXPORT ITerminal
	{
	public:
		virtual ~ITerminal() { }

		/*!
		Get the terminal string.
		*/
		virtual const wchar_t* TerminalString() const = 0;

		/*!
		Get the name for the terminal.
		*/
		virtual const wchar_t* Name() const = 0;

		/*!
		Get the description for the terminal.
		*/
		virtual const wchar_t* Description() const = 0;

		/*!
		Get the security key name when security is enabled.
		*/
		virtual const wchar_t* SecurityKeyName() const = 0;
	};
}

#endif