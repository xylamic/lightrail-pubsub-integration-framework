/*!
@file	LightrailLogged.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef LIGHTRAILLOGGED_H
#define LIGHTRAILLOGGED_H

#include "Lightrail_global.h"
#include "IXLogger.h"
#include "XLogger.h"

#include <sstream>

namespace Xylasoft
{
	/*!
	Base class used for generic across-the-board internal logging.
	*/
	class LIGHTRAILSHARED_EXPORTTEST LightrailLogged
	{
	public:
		/*!
		Constructors the lightrail logger object.
		@param[in] logContext The logged context for this object.
		*/
		LightrailLogged(const wchar_t* logContext);

		/*!
		Destructor.
		*/
		virtual ~LightrailLogged();

	protected:
		/*!
		Log a message.
		@param[in] message The message to log.
		@param[in] level The level of the message.
		*/
		void LogMessage(const wchar_t* message, IXLogger::LogLevel level);

		/*!
		Log a message.
		@param[in] message The stream to log from.
		@param[in] level The level of the message.
		*/
		void LogMessage(const std::wostringstream& message, IXLogger::LogLevel level);

		/*!
		Log a message with an exception.
		@param[in] message The message to log.
		@param[in] level The level of the message.
		@param[in] ex The exception to log.
		*/
		virtual void LogException(const wchar_t* message, IXLogger::LogLevel level, const std::exception& ex);

		/*!
		Log a message with an exception.
		@param[in] message The message to log.
		@param[in] level The level of the message.
		@param[in] ex The exception to log.
		*/
		virtual void LogException(const std::wostringstream& message, IXLogger::LogLevel level, const std::exception& ex);

	private:
		IXLogger* m_logger;
		std::wstring m_context;
	};
}

#endif