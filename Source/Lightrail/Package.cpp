/*!
@file	Package.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Package.h"

#define FORMAT_V1 0

using namespace Xylasoft;

Package::Package(PackageType type, const XByteArray* data)
{
	this->m_type = type;

	if (data == nullptr)
	{
		this->m_data = new XByteArray();
	}
	else
	{
		this->m_data = data;
	}
}

/*!
Construct a package from a serialization set of data.
@param[in] data The serialized data.
*/
Package::Package(const XByteArray& data)
{
	XByteArray* packageData = new XByteArray();

	XSerializer serializer(&data);

	uint16_t version;
	serializer >> version;

	if (version != FORMAT_V1)
	{
		throw DataException(L"The package format version was unrecognized.");
	}

	uint16_t  type;
	serializer >> type;
	this->m_type = static_cast<PackageType>(type);

	serializer >> *packageData;
	this->m_data = packageData;
}

/*!
Copy constructor. Creates a complete copy of the package.
@param[in] package The package to copy.
*/
Package::Package(const Package& package)
{
	this->m_data = new XByteArray(*package.m_data);
	this->m_type = package.m_type;
}

/*!
Clean up left over message memory.
*/
Package::~Package()
{
	delete this->m_data;
}

/*!
Serialize the current instance to the specified byte array.
@param[in] data The byte array to serialize to.
*/
void Package::Serialize(XSerializer& serializer) const
{
	serializer << static_cast<uint16_t>(FORMAT_V1);

	serializer << static_cast<uint16_t>(this->m_type);
	serializer << *this->m_data;
}

/*!
Get the package type.
@returns The package type.
*/
Package::PackageType Package::Type() const
{
	return this->m_type;
}

/*!
Get the body of the package.
@returns The binary data package.
*/
const XByteArray& Package::Data() const
{
	return *this->m_data;
}

/*!
Create a terminal request package.
@param[in] peer The discovery peer making the request.
@returns The new package created.
*/
Package* Package::CreatePackage(const DiscoveryPeer& peer)
{
	XByteArray* data = new XByteArray();
	XSerializer serializer(*data);
	peer.Serialize(serializer);

	return new Package(Package::TERMINALREQUEST, data);
}

/*!
Create a terminal listing package.
@param[in] listing The listing to package.
@returns The new package created.
*/
Package* Package::CreatePackage(const TerminalListing& listing)
{
	XByteArray* data = new XByteArray();
	XSerializer serializer(*data);
	listing.Serialize(serializer);

	return new Package(Package::TERMINALLIST, data);
}

/*!
Create a delivery package.
@param[in] delivery The delivery to package.
@returns The new package created.
*/
Package* Package::CreatePackage(Package::PackageType type, const DetailedDelivery& delivery)
{
	XByteArray* data = new XByteArray();
	XSerializer serializer(*data);
	delivery.Serialize(serializer);

	return new Package(type, data);
}