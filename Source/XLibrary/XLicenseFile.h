/*!
@file	XLicenseFile.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XLICENSEFILE_H
#define XLICENSEFILE_H

#include "Xylasoft.h"
#include "XSerializer.h"

namespace Xylasoft
{
	/*!
	This class provides the ability to generate or read an encrypted license file.
	*/
	class XLicenseFile
	{
	public:
		/*!
		Create a new license file object.
		@param[in] company The company (or individual name).
		@param[in] product The product name.
		@param[in] type The type of license.
		@param[in] version The license version.
		@param[in] expiration The date of license expiration.
		@param[in] updateExpiration The expiration date for receiving new product updates.
		@param[in] user The user name (e-mail address) of the license holder.
		@param[in] userLicenseId The user license identifier.
		@param[in] numberLicense The number of licenses covered by this license.
		*/
		XLicenseFile(const wchar_t* company, const wchar_t* product, uint16_t type, uint16_t version, 
			time_t expiration, time_t updateExpiration, const wchar_t* user, const wchar_t* userLicenseId,
			uint16_t numberLicenses);

		/*!
		Construct a license file from an existing file's content.
		@param[in] filedata The file content.
		*/
		XLicenseFile(const XByteArray& filedata);

		/*!
		Destroy the license file object.
		*/
		~XLicenseFile();

		/*!
		Get the company name (or individual name).
		@returns The string.
		*/
		const wchar_t* Company() const;

		/*!
		Get the product name.
		@returns The string.
		*/
		const wchar_t* Product() const;

		/*!
		Get the license type number.
		@returns The type number.
		*/
		uint16_t Type() const;

		/*!
		Get the version of the license.
		@return The version number.
		*/
		uint16_t Version() const;

		/*!
		Get the expiration time of the license.
		@returns The time.
		*/
		time_t Expiration() const;

		/*!
		Get the expiration time which software updates will expire.
		@returns True
		*/
		time_t UpdateExpiration() const;

		/*!
		Get the user name.
		@returns The string.
		*/
		const wchar_t* User() const;

		/*!
		Get the user license identifier.
		@returns The string.
		*/
		const wchar_t* UserLicenseId() const;

		/*!
		Get the number of licenses contained by this file.
		@return The number of licenses.
		*/
		uint16_t NumberLicenses() const;

		/*!
		Serialize the license file to binary content.
		@param[in] filedata The array to write to.
		*/
		void Serialize(XByteArray& filedata) const;

	private:
		std::wstring m_company;
		std::wstring m_product;
		uint16_t m_type;
		uint16_t m_version;
		time_t m_expiration;
		time_t m_updateExpiration;
		std::wstring m_user;
		std::wstring m_userlicenseId;
		uint16_t m_number;
	};
}

#endif