/*!
@file	IXLogger.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef IXLOGGER_H
#define IXLOGGER_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	Interface definition for logging.
	*/
	class IXLogger
	{
	public:
		/*!
		  Enumeration that defines the logging levels.
		*/
		enum LogLevel
		{
			/*! Debug information. */
			Debugging = 0,

			/*! Informational message. */
			Information = 1,

			/*! Warning that an error may occur. */
			Warning = 2,
		
			/*! Error that prevents functionality from executing. */
			Error = 3,

			/*! Critical error that results in the system stopping. */
			Fatal = 4
		};

	public:
		/*!
		Log a message.
		@param[in] context The context of the message (location, method, etc).
		@param[in] message The message to log.
		@param[in] level The level of the message.
		*/
		virtual void LogMessage(const wchar_t* context, const wchar_t* message, LogLevel level) = 0;

		/*!
		Set the logging level. No messages will be logged below the specified level.
		@param[in] level The minimum level to log.
		*/
		virtual void SetLevel(LogLevel level) = 0;

		/*!
		Get the logging level.
		@returns The log level.
		*/
		virtual LogLevel Level() const = 0;

		/*!
		Log a message with an exception.
		@param[in] context The context of the message.
		@param[in] message The message to log.
		@param[in] level The level of the message.
		@param[in] ex The exception to log.
		*/
		virtual void LogException(const wchar_t* context, const wchar_t* message, LogLevel level, const std::exception& ex) = 0;
	};
}

#endif // ILOGGER_H
