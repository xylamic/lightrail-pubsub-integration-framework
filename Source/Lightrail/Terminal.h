/*!
@file	Terminal.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef TERMINAL_H
#define TERMINAL_H

#include "Lightrail_global.h"
#include "Method.h"
#include "ITerminal.h"
#include <XSerializer.h>

namespace Xylasoft
{
	/*!
	  Representation of a terminal for a delivery to arrive at or depart from. This provides the information
	  that allows the infrastructure to decide who information goes to.
	  *
	  Example: [Xylasoft>Infrastructure>Business>Test] matches [Xylasoft>>Business>
	*/
	class LIGHTRAILSHARED_EXPORTTEST Terminal : public ITerminal
	{
	public:
		/*!
		Default constructor for copying other instances.
		*/
		Terminal();

		/*!
		Constructor for a terminal.
		@param[in] name The name of the terminal.
		@param[in] description The description for the terminal.
		@param[in] terminal The terminal string.
		@param[in] securityKey The security key name if security is to be used, otherwise nullptr. If
								a security key is specified, then AES security will be enabled. The
								security key must be registered prior to registration of the topic.
		*/
		Terminal(const wchar_t* name, const wchar_t* description, const wchar_t* terminal, const wchar_t* securityKey = nullptr);

		/*!
		Create the terminal from a serialization.
		@param[in] serializer The serializer to build from.
		*/
		Terminal(XSerializer& serializer);

		/*!
		Compares this terminal with another for matching.
		@param[in] other The terminal to match with.
		@returns True if the terminals match.
		*/
		bool operator==(const Terminal& other) const;

		/*!
		Compares this terminal with another for non-matching.
		@see operator==
		*/
		bool operator!=(const Terminal& other) const;

		/*!
		Serialize the terminal to binary.
		@param[in] data The array to serialize to.
		*/
		void Serialize(XSerializer& serializer) const;

		// ITerminal implementation
	public:
		/*!
		Get the terminal string.
		*/
		virtual const wchar_t* TerminalString() const;

		/*!
		Get the name for the terminal.
		*/
		virtual const wchar_t* Name() const;

		/*!
		Get the description for the terminal.
		*/
		virtual const wchar_t* Description() const;

		/*!
		Get the security key name when security is enabled.
		*/
		virtual const wchar_t* SecurityKeyName() const;

	public:
		/*!
		Validate that the terminal string is valid.
		@param[in] terminal The terminal string.
		*/
		static void ValidateTerminalString(const wchar_t* terminal);

		/*!
		Compare two Terminals for matching.
		@param[in] t1 The first terminal to match.
		@param[in] t2 The second terminal to match.
		@returns Zero if the terminals match.
		*/
		static int TerminalCompare(const Terminal& d1, const Terminal& d2);

	private:
		std::wstring m_name;
		std::wstring m_description;
		std::wstring m_terminal;
		std::wstring m_securityKey;
	};
}

#endif