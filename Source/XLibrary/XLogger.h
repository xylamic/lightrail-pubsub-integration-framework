/*!
@file	XLogger.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XLOGGER_H
#define XLOGGER_H

#include "Xylasoft.h"
#include "IXLogger.h"
#include "XMutex.h"
#include "XException.h"
#include "XFile.h"
#include "XDirectory.h"

#include <map>
#include <string>
#include <fstream>

namespace Xylasoft
{
	/*!
	Concrete logger and static methods for logger creation.
	*/
	class XLogger : public IXLogger
	{
		// static logger creation methods
	public:
		/*!
		Get or create the logger with the specified name.
		@param[in] name The name of the logger.
		@returns The logger instance.
		*/
		static IXLogger* GetLogger(const wchar_t* name);

		/*!
		Close the specified logger.
		@param[in] name The name of the logger.
		*/
		static void CloseLogger(const wchar_t* name);

		/*!
		Set the global logging directory.
		@param[in] path The directory path.
		*/
		static void SetLoggingDirectory(const wchar_t* path);

		/*!
		Get the logging directory.
		@returns The directory string.
		*/
		static std::wstring LoggingDirectory();

		/*!
		Get the maximum log size in bytes.
		@returns The max log size.
		*/
		static int MaxLogSize();

		/*!
		Set the maximum log size in bytes.
		@param[in] size The max log size.
		*/
		static void SetMaxLogSize(int size);

		/*!
		Get the textual name of the specified level.
		@param[in] level The level.
		@returns The name string.
		*/
		static std::wstring GetTextForLevel(LogLevel level);

		/*!
		Set the process global log name.
		@param[in] name The name of the log.
		*/
		static void SetGlobalLog(const wchar_t* name);

		/*!
		Get the global log name.
		@returns The name string.
		*/
		static std::wstring GlobalLogName();

		/*!
		Get the global logger instance.
		@returns The logger.
		*/
		static IXLogger* GetGlobalLogger();

		// IXLogger implementation
	public:
		/*!
		Log a message.
		@param[in] context The context of the message (location, method, etc).
		@param[in] message The message to log.
		@param[in] level The level of the message.
		*/
		virtual void LogMessage(const wchar_t* context, const wchar_t* message, LogLevel level);

		/*!
		Log a message with an exception.
		@param[in] context The context of the message.
		@param[in] message The message to log.
		@param[in] level The level of the message.
		@param[in] ex The exception to log.
		*/
		virtual void LogException(const wchar_t* context, const wchar_t* message, LogLevel level, const std::exception& ex);

		/*!
		Set the logging level. No messages will be logged below the specified level.
		@param[in] level The minimum level to log.
		*/
		virtual void SetLevel(LogLevel level);

		/*!
		Get the logging level.
		@returns The log level.
		*/
		virtual IXLogger::LogLevel Level() const;

		// constructor/destructor for XLogger
	protected:
		/*!
		Construct a logger.
		@param[in] filepath The path to the file to log in.
		*/
		XLogger(const wchar_t* filepath);

		/*!
		Destroy the logger.
		*/
		~XLogger();

		/*!
		Verify the log size has not exceeded its maximum size.
		*/
		void VerifyLogSize();

	private:
		/*!
		Copy a logger instance. Not allowed.
		*/
		XLogger(const XLogger&);

	private:
		mutable XMutex m_logMutex;
		std::wofstream m_fileStream;
		LogLevel m_level;
		long m_charsWritten;
		XFile m_file;

		static std::wstring m_globalLogName;
		static XDirectory m_directory;
		static XMutex m_instanceMutex;
		static volatile int m_maxLogSize;
		static std::map<std::wstring, IXLogger*> m_loggers;
	};
}
#endif // XLOGGER_H
