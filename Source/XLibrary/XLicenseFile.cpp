/*!
@file	XLicenseFile.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XLicenseFile.h"
#include "XEncrypter.h"

#define LICENSE_VERSION	1
#define LICENSE_KEY		"jf($9IOJoihorojh)(*$*$*"

using namespace Xylasoft;

XLicenseFile::XLicenseFile(const wchar_t* company, const wchar_t* product, uint16_t type, uint16_t version, 
	time_t expiration, time_t updateExpiration, const wchar_t* user, const wchar_t* userLicenseId,
	uint16_t numberLicenses)
{
	this->m_company = company;
	this->m_product = product;
	this->m_type = type;
	this->m_version = version;
	this->m_expiration = expiration;
	this->m_updateExpiration = updateExpiration;
	this->m_user = user;
	this->m_userlicenseId = userLicenseId;
	this->m_number = numberLicenses;
}

XLicenseFile::XLicenseFile(const XByteArray& filedata)
{
	XEncrypter encrypter(LICENSE_KEY);
	XByteArray decData = encrypter.Decrypt(filedata);

	XSerializer serializer(&decData);

	uint16_t fileversion;
	serializer >> fileversion;
	if (fileversion >= LICENSE_VERSION)
	{
		serializer >> this->m_company;
		serializer >> this->m_product;
		serializer >> this->m_user;
		serializer >> this->m_userlicenseId;

		serializer >> this->m_type;
		serializer >> this->m_version;
		serializer >> this->m_number;

		uint32_t expiration, updateExpiration;
		serializer >> expiration;
		serializer >> updateExpiration;
		this->m_expiration = expiration;
		this->m_updateExpiration = updateExpiration;
	}
}

XLicenseFile::~XLicenseFile()
{
}

const wchar_t* XLicenseFile::Company() const
{
	return this->m_company.c_str();
}

const wchar_t* XLicenseFile::Product() const
{
	return this->m_product.c_str();
}

uint16_t XLicenseFile::Type() const
{
	return this->m_type;
}

uint16_t XLicenseFile::Version() const
{
	return this->m_version;
}

time_t XLicenseFile::Expiration() const
{
	return this->m_expiration;
}

time_t XLicenseFile::UpdateExpiration() const
{
	return this->m_updateExpiration;
}

const wchar_t* XLicenseFile::User() const
{
	return this->m_user.c_str();
}

const wchar_t* XLicenseFile::UserLicenseId() const
{
	return this->m_userlicenseId.c_str();
}

uint16_t XLicenseFile::NumberLicenses() const
{
	return this->m_number;
}

void XLicenseFile::Serialize(XByteArray& filedata) const
{
	XSerializer serializer(filedata);

	serializer << static_cast<uint16_t>(LICENSE_VERSION);

	serializer << this->m_company;
	serializer << this->m_product;
	serializer << this->m_user;
	serializer << this->m_userlicenseId;

	serializer << this->m_type;
	serializer << this->m_version;
	serializer << this->m_number;

	serializer << static_cast<uint32_t>(this->m_expiration);
	serializer << static_cast<uint32_t>(this->m_updateExpiration);

	XEncrypter encrypter(LICENSE_KEY);
	filedata = encrypter.Encrypt(filedata);
}