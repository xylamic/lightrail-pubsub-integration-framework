/*!
@file	XHostAddress.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XHOSTADDRESS_H
#define XHOSTADDRESS_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	Defines an individual host address and address type.
	*/
	class XHostAddress
	{
	public:
		/*! The types of addresses. */
		enum AddressType
		{
			/*! IP version 4 */
			IPv4 = 1,

			/*! IP version 6 */
			IPv6 = 2
		};

	public:
		/*!
		Constructs an empty address when the address is not important.
		*/
		XHostAddress();

		/*!
		Constructor for a host address.
		@param[in] address The string address.
		@param[in] type The type of address.
		*/
		XHostAddress(const char* address, AddressType type);

		/*! Get the address string. */
		const char* Address();

		/*! Get the address type. */
		AddressType Type();

		/*! Get whether this address is empty. */
		bool IsEmpty();

	private:
		bool m_isEmpty;
		std::string m_address;
		AddressType m_type;
	};
}

#endif