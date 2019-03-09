/*!
@file	XHostLookup.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XHOSTLOOKUP_H
#define XHOSTLOOKUP_H

#include "Xylasoft.h"
#include "XException.h"
#include "XHostAddress.h"
#include <list>

namespace Xylasoft
{
	/*!
	This looks up a host and provides information about it.
	*/
	class XHostLookup
	{
	public:
		/*!
		Construct a lookup for the local system with no IP version specification.
		*/
		XHostLookup();

		/*!
		Construct a lookup for the local system with the specified IP version.
		@param[in] type The IP version.
		*/
		XHostLookup(XHostAddress::AddressType type);

		/*!
		Construct a lookup for a host machine with no IP version specification.
		@param[in] host The hostname.
		*/
		XHostLookup(const char* host);

		/*!
		Construct a lookup for a host machine with the specified IP version.
		@param[in] host The hostname.
		@param[in] type The host type.
		*/
		XHostLookup(const char* host, XHostAddress::AddressType type);

		/*!
		Get the list of addresses for the host.
		@returns The list of addresses.
		*/
		std::list<XHostAddress> BindAddresses();

		/*!
		Get the local host name.
		@returns The local hostname.
		*/
		static std::string LocalHostName();

	protected:
		void BuildAddressList(const char* host, int type);

	private:
		std::list<XHostAddress> m_addresses;
	};
}

#endif