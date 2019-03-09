/*!
@file	Package.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef PACKAGE_H
#define PACKAGE_H

#include "Lightrail_global.h"
#include "DiscoveryPeer.h"
#include "TerminalListing.h"
#include "DetailedDelivery.h"
#include <XSerializer.h>
#include <XByteArray.h>
#include <map>

namespace Xylasoft
{
	class Package
	{
	public:
		/*!
		Enumeration defining the different types of items this can package.
		*/
		enum PackageType
		{
			/*! Message inteneded for connection testing. */
			TEST = 0,
			
			/*! Package intended for a request. */
			REQUEST = 1,

			/*! Package intended for a response. */
			RESPONSE = 2,

			/*! Package intended for a publication. */
			PUBLISH = 3,

			/*! Package intended to deliver a topic list. */
			TERMINALLIST = 4,

			/*! Package intended  to request a topic list. */
			TERMINALREQUEST = 5
		};
	public:
		/*!
		Construct a package with the given type and body.
		@param[in] type The package type.
		@param[in] data The binary data representing the package.
		*/
		Package(PackageType type, const XByteArray* data);

		/*!
		Construct a package from a serialization set of data.
		@param[in] data The serialized data.
		*/
		Package(const XByteArray& data);

		/*!
		Copy constructor. Creates a complete copy of the package.
		@param[in] package The package to copy.
		*/
		Package(const Package& package);

		/*!
		Clean up left over message memory.
		*/
		~Package();

		/*!
		Serialize the current instance to the specified byte array.
		@param[in] data The byte array to serialize to.
		*/
		void Serialize(XSerializer& serializer) const;

		/*!
		Get the package type.
		@returns The package type.
		*/
		PackageType Type() const;

		/*!
		Get the body of the package.
		@returns The binary data package.
		*/
		const XByteArray& Data() const;

		/*!
		Create a terminal request package.
		@param[in] peer The discovery peer making the request.
		@returns The new package created.
		*/
		static Package* CreatePackage(const DiscoveryPeer& peer);

		/*!
		Create a terminal listing package.
		@param[in] listing The listing to package.
		@returns The new package created.
		*/
		static Package* CreatePackage(const TerminalListing& listing);

		/*!
		Create a delivery package.
		@param[in] delivery The delivery to package.
		@returns The new package created.
		*/
		static Package* CreatePackage(Package::PackageType type, const DetailedDelivery& delivery);

	private:
		PackageType m_type;
		const XByteArray* m_data;
	};
}

#endif