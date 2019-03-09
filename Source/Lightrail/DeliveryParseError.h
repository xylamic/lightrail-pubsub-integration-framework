/*!
@file	DeliveryParseError.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef DELIVERYPARSEERROR_H
#define DELIVERYPARSEERROR_H

#include "Lightrail_global.h"
#include "Terminal.h"
#include "IDeliveryParseError.h"

namespace Xylasoft
{
	class DeliveryParseError : public IDeliveryParseError
	{
	public:
		DeliveryParseError(const ITerminal* terminal, const std::wstring& sourceapp, const std::string& sourcehost,
			long sourceport, ParseErrorType error, std::exception& message);

		virtual const ITerminal* Route() const override;

		virtual const wchar_t* SourceApplication() const override;

		virtual const char* SourceHostname() const override;

		virtual long SourcePort() const override;

		virtual ParseErrorType ErrorType() const override;

		virtual const std::exception& Message() const override;

	private:
		const ITerminal* m_terminal;
		ParseErrorType m_error;
		std::exception m_message;
		std::wstring m_sourceapp;
		std::string m_sourcehost;
		long m_sourceport;
	};
}

#endif