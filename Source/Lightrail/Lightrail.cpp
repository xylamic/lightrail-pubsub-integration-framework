/*!
@file	Lightrail.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Lightrail.h"
#include "LightrailClient.h"
#include "Terminal.h"
#include "GlobalSettings.h"
#include "Delivery.h"
#include "AES256SecurityKey.h"
#include "Discovery.h"
#include <XLicenseFile.h>
#include <XFile.h>
#include <XDirectory.h>
#include <sstream>
#include <fstream>

using namespace Xylasoft;

#define LIGHTRAIL_VERSION_10 0

void CheckLicense()
{
	// Xylasoft Lightrail is now open source (2011-12-10), so this code has been disabled from use
	return;

	static std::wstring company = L"Xylasoft";
	static std::wstring product = L"Lightrail";
	static int licensetype_guest = 0;
	static int licensetype_comm = 1;

	XDirectory directory(XUtilities::GetDataFolder().c_str());
	if (!directory.Exists())
	{
		directory.Create();
	}

	XFile licenseFile = directory.File(L"LightrailLicense.xyl");

	// if the license file does not exist, create a guest file for 40 days
	if (!licenseFile.Exists())
	{
		long fourtyDays = 60 * 60 * 24 * 40;
		XLicenseFile guestFile(company.c_str(), product.c_str(), licensetype_guest, 0, ::time(nullptr) + fourtyDays, 
			::time(nullptr), L"Guest", L"Guest", 1);

		XByteArray guestFileData;
		guestFile.Serialize(guestFileData);

		std::ofstream filestream(XUtilities::GetStringFromWString(licenseFile.Path()), 
			std::ios::out | std::ios::binary);
		if (!filestream.is_open())
		{
			throw LicenseException(L"The license file could not be accessed.");
		}

		filestream.write(guestFileData.ConstData(), guestFileData.Size());
		filestream.flush();
		filestream.close();
	}

	// try to read the file
	XByteArray licenseFileDataArray;
	std::ifstream licenseFileStream(XUtilities::GetStringFromWString(licenseFile.Path()), std::ios::in | std::ios::binary);
	if (!licenseFileStream.is_open())
	{
		throw LicenseException(L"The license file could not be opened.");
	}
	char c = licenseFileStream.get();
	while (!licenseFileStream.eof())
	{
		licenseFileDataArray.Append(c);
		c = licenseFileStream.get();
	}
	licenseFileStream.close();

	// parse and check the file
	XLicenseFile xlicenseFile(licenseFileDataArray);
	if (company.compare(xlicenseFile.Company()) != 0 || product.compare(xlicenseFile.Product()) != 0)
	{
		throw LicenseException(L"The license was not valid for this product.");
	}
	else if (xlicenseFile.Expiration() < ::time(NULL))
	{
		throw LicenseException(L"The license is expired. Visit http://www.xylasoft.com for more information on a new license.");
	}
}

ILightrail* Lightrail::GetLightrailInstance(const wchar_t* name, const wchar_t* description, const wchar_t* instance)
{
	static bool initialized = false;

	if (!initialized)
	{
		GlobalSettings::ReadConfiguation();
		initialized = true;

		long id = ::GetCurrentProcessId();
		std::wostringstream namestream;
		namestream << L"Lightrail_PID" << id;
		XLogger::SetGlobalLog(namestream.str().c_str());

		IXLogger* logger = XLogger::GetGlobalLogger();
		logger->SetLevel(GlobalSettings::LogLevel);
	}

	CheckLicense();

	return new LightrailClient(name, description, instance);
}

IDiscovery* Lightrail::GetDiscoveryInstance()
{
	return new Discovery();
}

const ITerminal* Lightrail::NewTerminal(const wchar_t* name, const wchar_t* description, const wchar_t* terminal, const wchar_t* securityKey)
{
	// make sure the terminal string does not being with Xylasoft, since this is a reserved string start
	if (terminal != nullptr)
	{
		std::wstring termstr(terminal);
		if (termstr.size() > 9)
		{
			termstr.resize(9);
		}

		if (XUtilities::CaseInsensitiveCompare(std::wstring(L"[Xylasoft"), termstr) == 0)
		{
			throw ArgumentException(L"The terminal string cannot begin with 'Xylasoft', this is reserved.");
		}
	}

	return new Terminal(name, description, terminal, securityKey);
}

IDelivery* Lightrail::NewDelivery()
{
	return new Delivery();
}

const ISecurityKey* Lightrail::NewAES256SecurityKey(const wchar_t* name, const char* key, long keySize)
{
	std::string keystring;
	keystring.resize(keySize);
	::memcpy(&keystring[0], key, keySize);

	return new AES256SecurityKey(name, keystring);
}