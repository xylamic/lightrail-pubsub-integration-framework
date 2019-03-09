/*!
@file	GlobalSettings.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "GlobalSettings.h"
#include "XLogger.h"
#include "XIni.h"
#include "XDirectory.h"
#include "XUtilities.h"
#include "XFile.h"
#include <sstream>

using namespace Xylasoft;

unsigned int GlobalSettings::SendTimeout = 30;

unsigned int GlobalSettings::ReceiveTimeout = 30;

unsigned int GlobalSettings::ConnectTimeout = 30;

std::string GlobalSettings::MulticastAddress = "233.57.149.67";

int GlobalSettings::MulticastTimeToLive = 1;

int GlobalSettings::MulticastPort = 23950;

int GlobalSettings::PeerTimeout = 120;

IXLogger::LogLevel GlobalSettings::LogLevel = IXLogger::Information;


const unsigned int GlobalSettings::TopicUpdateTime = 30;

const unsigned int GlobalSettings::ThreadWaitTime = 15;

const unsigned int GlobalSettings::ThreadTimeout = 30;

const unsigned int GlobalSettings::EmptyQueueWaitTime = 20;

const unsigned int GlobalSettings::ConnectionErrorWaitTime = 30;

const unsigned int GlobalSettings::ConnectionListUpdateTimeout = 120;

const unsigned int GlobalSettings::ConnectionListUpdateCheckTime = 30;

bool GlobalSettings::GenerateSettingsFile(const wchar_t* path)
{
	std::wofstream filestream(path, std::ios::out);
	if (!filestream.is_open())
	{
		return false;
	}

	std::vector<std::wstring> lines;

	lines.push_back(L"[Discovery]");
	lines.push_back(L"; Multicast address");
	lines.push_back(L"Address\t\t= 233.57.149.67");
	lines.push_back(L"; Multicast port #");
	lines.push_back(L"Port\t\t= 23950");
	lines.push_back(L"; Multicast time to live [0 - local machine, 1 - subnet, 2-32 - site...]");
	lines.push_back(L"TimeToLive\t= 1");
	lines.push_back(L"; The amount of time to wait for a peer to reconnect before dropping them (in seconds)");
	lines.push_back(L"PeerTimeout\t= 120");
	lines.push_back(L"");
	lines.push_back(L"[Logging]");
	lines.push_back(L"; The level to log at [Fatal, Error, Warn, Info, Debug]");
	lines.push_back(L"Level\t\t= Info");

	for (auto lineit = lines.begin(); lineit != lines.end(); lineit++)
	{
		std::wstring line = *lineit;
		line.push_back(L'\n');

		filestream.write(line.c_str(), line.size());
	}

	filestream.flush();
	filestream.close();

	return true;
}

void GlobalSettings::ReadConfiguation()
{
	const wchar_t* context = L"GlobalSettings";

	IXLogger* logger = XLogger::GetGlobalLogger();
	logger->LogMessage(context, L"Attempting read of configuration file...", IXLogger::Information);

	try
	{
		XDirectory dir(XUtilities::GetDataFolder().c_str());
		XFile file = dir.File(L"Lightrail.ini");

		if (!file.Exists() && !GenerateSettingsFile(file.Path().c_str()))
		{
			throw IOException(L"The configuration file was not found and could not be created.");
		}

		XIni config(file.Path().c_str());

		std::wstring discAddress = config.GetValueForKey(L"Discovery", L"Address");
		if (discAddress.size() == 0)
		{
			logger->LogMessage(context, L"The discovery address was not found in configuration, leaving default.", IXLogger::Information);
		}
		else
		{
			MulticastAddress = XUtilities::GetStringFromWString(discAddress);
		}

		std::wstring discTtl = config.GetValueForKey(L"Discovery", L"TimeToLive");
		if (discTtl.size() == 0)
		{
			logger->LogMessage(context, L"The discovery message time to live was not found in configuration, leaving default.", IXLogger::Information);
		}
		else
		{
			MulticastTimeToLive = ::atoi(XUtilities::GetStringFromWString(discTtl).c_str());
		}

		std::wstring discPort = config.GetValueForKey(L"Discovery", L"Port");
		if (discPort.size() == 0)
		{
			logger->LogMessage(context, L"The discovery port was not found in configuration, leaving default.", IXLogger::Information);
		}
		else
		{
			MulticastPort = atoi(XUtilities::GetStringFromWString(discPort).c_str());
		}

		std::wstring discPeerTimeout = config.GetValueForKey(L"Discovery", L"PeerTimeout");
		if (discPeerTimeout.size() == 0)
		{
			logger->LogMessage(context, L"The discovery peer timeout was not found in configuration, leaving default.", IXLogger::Information);
		}
		else
		{
			PeerTimeout = atoi(XUtilities::GetStringFromWString(discPeerTimeout).c_str());
		}

		std::wstring logLevel = config.GetValueForKey(L"Logging", L"Level");
		if (logLevel.size() == 0)
		{
			logger->LogMessage(context, L"The logging level was not found in configuration, leaving default.", IXLogger::Information);
		}
		else
		{
			if (logLevel.compare(XLogger::GetTextForLevel(IXLogger::Information)) == 0)
			{
				LogLevel = IXLogger::Information;
			}
			else if (logLevel.compare(XLogger::GetTextForLevel(IXLogger::Debugging)) == 0)
			{
				LogLevel = IXLogger::Debugging;
			}
			else if (logLevel.compare(XLogger::GetTextForLevel(IXLogger::Warning)) == 0)
			{
				LogLevel = IXLogger::Warning;
			}
			else if (logLevel.compare(XLogger::GetTextForLevel(IXLogger::Error)) == 0)
			{
				LogLevel = IXLogger::Error;
			}
			else if (logLevel.compare(XLogger::GetTextForLevel(IXLogger::Fatal)) == 0)
			{
				LogLevel = IXLogger::Fatal;
			}

			std::wostringstream logstream;
			logstream << L"Changing log level now to " << LogLevel << L"...";
			logger->LogMessage(context, logstream.str().c_str(), IXLogger::Information);
			logger->SetLevel(LogLevel);
		}
		
		logger->LogMessage(context, L"Successfully read configuration file.", IXLogger::Information);
	}
	catch (std::exception& ex)
	{
		logger->LogException(context, L"Failed to read configuration file.", IXLogger::Error, ex);
	}
}