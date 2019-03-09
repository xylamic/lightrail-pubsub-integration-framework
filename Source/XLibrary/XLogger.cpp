/*!
@file	XLogger.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XLogger.h"
#include "XUtilities.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>

using namespace Xylasoft;
using namespace std;

//#define LOG_TO_CONSOLE

XLogger::XLogger(const wchar_t* filepath)
	: m_file(filepath)
{
	this->m_fileStream.open(filepath, std::ios::out | std::ios::app);
	if (!this->m_fileStream.is_open())
	{
		throw IOException(L"The file could not be opened for writing.");
	}

	this->m_charsWritten = 0;
	this->m_level = IXLogger::Information;
}

XLogger::~XLogger()
{
	if (this->m_fileStream.is_open())
	{
		this->m_fileStream.close();
	}
}

void XLogger::LogException(const wchar_t* context, const wchar_t* message, LogLevel level, const std::exception& ex)
{
	wostringstream stream;

	stream << message << " [Exception]" << XUtilities::GetWStringFromString(ex.what());

	this->LogMessage(context, stream.str().c_str(), level);
}

void XLogger::LogMessage(const wchar_t* context, const wchar_t* message, LogLevel level)
{
	XMutexLocker lock(this->m_logMutex);

	if (level < this->m_level)
	{
		return;
	}

	time_t utcTimeT;
	::time(&utcTimeT);

	tm* ptm = ::gmtime(&utcTimeT);

	std::wostringstream strStream;
	strStream << L"[" << setw(4) << setfill(L'0') << (ptm->tm_year + 1900) << L"-" << setw(2) << setfill(L'0') << (ptm->tm_mon + 1) << 
		L"-" << setw(2) << setfill(L'0') << (ptm->tm_mday) << " " << setw(2) << setfill(L'0') << ptm->tm_hour << L":" << 
		setw(2) << setfill(L'0') << ptm->tm_min << L":" << setw(2) << setfill(L'0') << ptm->tm_sec << L"]" <<
		
		L"[" << GetTextForLevel(level) << L"]" <<
		L"[" << context << L"]: " << 
		message << L"\n";

	std::wstring msg = strStream.str();
	this->m_charsWritten += static_cast<long>(msg.size());

	this->m_fileStream << msg;
	this->m_fileStream.flush();

#ifdef LOG_TO_CONSOLE
	std::wcout << msg;
	std::wcout.flush();
#endif

	this->VerifyLogSize();
}

void XLogger::SetLevel(LogLevel level)
{
	XMutexLocker lock(this->m_logMutex);
	this->m_level = level;
}

IXLogger::LogLevel XLogger::Level() const
{
	XMutexLocker lock(this->m_logMutex);
	return this->m_level;
}

void XLogger::VerifyLogSize()
{
	if (this->m_charsWritten > this->m_maxLogSize)
	{
		this->m_fileStream.close();

		time_t utcTimeT;
		::time(&utcTimeT);

		wostringstream newfilenamestream;
		newfilenamestream << this->m_file.Name() << "." << utcTimeT;
		wstring newfilename = newfilenamestream.str();

		XFile file(this->m_file);
		file.Rename(newfilename.c_str());

		this->m_fileStream.open(this->m_file.Path(), ios::out | ios::trunc);
		this->m_charsWritten = 0;
	}
}

IXLogger* XLogger::GetLogger(const wchar_t* name)
{
	// get the logging directory before entering critical section to
	// keep from crossing mutexes
	std::wstring filepath = LoggingDirectory();

	XMutexLocker lock(m_instanceMutex);

	std::map<std::wstring, IXLogger*>::iterator it = m_loggers.find(name);
	if (it != m_loggers.end())
	{
		return it->second;
	}
	else
	{
		filepath.append(name);

		IXLogger* pLogger = new XLogger(m_directory.File(name).Path().append(L".log").c_str());
		m_loggers[name] = pLogger;

		return pLogger;
	}
}

void XLogger::SetGlobalLog(const wchar_t* name)
{
	IXLogger* log = GetLogger(m_globalLogName.c_str());
	m_globalLogName = name;
}

std::wstring XLogger::GlobalLogName()
{
	return m_globalLogName;
}

IXLogger* XLogger::GetGlobalLogger()
{
	return GetLogger(m_globalLogName.c_str());
}

void XLogger::CloseLogger(const wchar_t* name)
{
	std::map<std::wstring, IXLogger*>::iterator it = m_loggers.find(name);
	if (it != m_loggers.end())
	{
		delete it->second;
		m_loggers.erase(it);
	}
}

void XLogger::SetLoggingDirectory(const wchar_t* path)
{
	XMutexLocker lock(m_instanceMutex);

	m_directory = XDirectory(path);
}

std::wstring XLogger::LoggingDirectory()
{
	XMutexLocker lock(m_instanceMutex);

	if (m_directory.Path().size() == 0)
	{
		m_directory = XDirectory(XUtilities::GetDataFolder().c_str());
	}

	return m_directory.Path();
}

int XLogger::MaxLogSize()
{
	return m_maxLogSize;
}

void XLogger::SetMaxLogSize(int size)
{
	m_maxLogSize = static_cast<int>(size * 0.90);
}

std::wstring XLogger::GetTextForLevel(LogLevel level)
{
	switch (level)
	{
	case IXLogger::Debugging:
		return L"Debug";
	case IXLogger::Information:
		return L"Info";
	case IXLogger::Warning:
		return L"Warn";
	case IXLogger::Error:
		return L"Error";
	case IXLogger::Fatal:
		return L"Fatal";
	default:
		return L"Unknown";
	}
}

std::map<std::wstring, IXLogger*> XLogger::m_loggers;

XMutex XLogger::m_instanceMutex;

XDirectory XLogger::m_directory(L"");

std::wstring XLogger::m_globalLogName = L"XylasoftXLog";

// default size is 5mb * 0.90
volatile int XLogger::m_maxLogSize = 4718592;