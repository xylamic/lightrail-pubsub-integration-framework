/*!
@file	GlobalSettings.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include "Lightrail_global.h"
#include "IXLogger.h"

namespace Xylasoft
{
	class GlobalSettings
	{
		// configurable items
	public:
		static unsigned int SendTimeout;

		static unsigned int ReceiveTimeout;

		static unsigned int ConnectTimeout;

		static std::string MulticastAddress;

		static int MulticastTimeToLive;

		static int PeerTimeout;

		static int MulticastPort;

		static IXLogger::LogLevel LogLevel;

		// bound constants
	public:
		static const unsigned int TopicUpdateTime;

		static const unsigned int ThreadWaitTime;

		static const unsigned int ThreadTimeout;

		static const unsigned int EmptyQueueWaitTime;

		static const unsigned int ConnectionErrorWaitTime;

		static const unsigned int ConnectionListUpdateTimeout;

		static const unsigned int ConnectionListUpdateCheckTime;

	public:
		static void ReadConfiguation();

	protected:
		static bool GenerateSettingsFile(const wchar_t* path);

	private:
		GlobalSettings();
		GlobalSettings(const GlobalSettings& other);
		GlobalSettings& operator=(const GlobalSettings& other);
	};
}
#endif // GLOBALSETTINGS_H
