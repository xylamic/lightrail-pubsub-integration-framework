/*!
@file	XException.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XEXCEPTION_H
#define XEXCEPTION_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*! This base exception for use in Xylasoft software. */
	class XException : public std::exception
	{
	public:
		/*!
		Construct a base exception.
		@param[in] message The text of the message.
		*/
		XException(const wchar_t* message);

		/*!
		Copy a base exception.
		@param[in] other The other to copy.
		*/
		XException(const XException& other);

		/*!
		Copy a base exception.
		@param[in] other The other to copy.
		@returns This instance.
		*/
		XException& operator=(const XException& other);

		/*!
		Destroy the exception.
		*/
		virtual ~XException();

		/*!
		Get the text of the exception.
		@returns The char string.
		*/
		virtual const char* what() const override;

		/*!
		Get the text of the exception.
		@returns The wchar_t string.
		*/
		virtual const wchar_t* wwhat() const;

	private:
		std::wstring m_message;
		std::string m_smessage;
	};

	/*! A licensing exception. */
	class LicenseException : public XException
	{
	public:
		LicenseException(const wchar_t* message);
	};

	/*! An IO exception. */
	class IOException : public XException
	{
	public:
		IOException(const wchar_t* message);
	};

	/*! A file exception */
	class FileException : public IOException
	{
	public:
		FileException(const wchar_t* message);
	};

	/*! A encryption exception */
	class EncryptionException : public IOException
	{
	public:
		EncryptionException(const wchar_t* message);
	};

	/*! A timeout exception */
	class TimeoutException : public IOException
	{
	public:
		TimeoutException(const wchar_t* message);
	};

	/*! Networking exception. Problem with external system. */
	class NetworkException : public XException
	{
	public:
		NetworkException(const wchar_t* message);
	};

	/*! A network resolution exception. */
	class ResolutionException : public NetworkException
	{
	public:
		ResolutionException(const wchar_t* message);
	};

	/*! A network bind exception. */
	class BindException : public NetworkException
	{
	public:
		BindException(const wchar_t* message);
	};

	/*! A network communication exception. */
	class CommunicationException : public NetworkException
	{
	public:
		CommunicationException(const wchar_t* message);
	};

	/*! A network address parse exception. */
	class AddressException : public NetworkException
	{
	public:
		AddressException(const wchar_t* message);
	};

	/*! An error that occurred because a data problem. */
	class DataException : public XException
	{
	public:
		DataException(const wchar_t* message);
	};

	/*! A data encoding exception. */
	class EncodingException : public DataException
	{
	public:
		EncodingException(const wchar_t* message);
	};

	/*! A exception caused by an invalid argument. */
	class ArgumentException : public DataException
	{
	public:
		ArgumentException(const wchar_t* message);
	};
}
#endif // XEXCEPTION_H
