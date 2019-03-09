/*!
@file	DetailedDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef DETAILEDDELIVERY_H
#define DETAILEDDELIVERY_H

#include "Lightrail_global.h"
#include "Terminal.h"
#include "Delivery.h"
#include "IDetailedDelivery.h"
#include "DiscoveryPeer.h"
#include <XSerializer.h>
#include <XUtilities.h>
#include <XException.h>
#include <XUuid.h>

namespace Xylasoft
{
	class DetailedDelivery : public IDetailedDelivery
	{
	public:
		DetailedDelivery(const Delivery* delivery, const Terminal& terminal, const wchar_t* sourceApp,
			const DiscoveryPeer& source);

		DetailedDelivery(XSerializer& serializer);

		DetailedDelivery(const DetailedDelivery& other);

		virtual ~DetailedDelivery();

		virtual void Serialize(XSerializer& serializer) const;

		virtual XUuid Identifier() const;

		virtual const ITerminal& SourceTerminal() const override;

		virtual long SourcePort() const override;

		virtual const char* SourceHost() const override;

		virtual const wchar_t* SourceApplication() const override;

		virtual const time_t TimeUtc() const override;

		virtual std::wstring MessageString() const override;

		virtual Encoding::EncodingType Encoding() const override;

		virtual const char* Message() const override;

		virtual long MessageSize() const override;

		/*!
		Get the specified label value.
		@param[in] name The name of the label.
		@returns The value for the label, or nullptr if not found.
		*/
		virtual const wchar_t* Label(const wchar_t* name) const override;

		/*!
		Get the number of labels present.
		@returns The number of labels.
		*/
		virtual long GetNumberOfLabels() const override;

		/*!
		Get the label name for the specified index.
		@param[in] index The index of the name to retrieve.
		@returns The name for the given index, or NULL if invalid.
		*/
		virtual const wchar_t* GetLabelName(long index) const override;

		virtual XUuid RequestIdentifier() const;

		virtual void SetRequestIdentifier(XUuid id);

		virtual void SetExceptionMessage(const char* msg);

		virtual const char* ExceptionMessage();

		virtual void DecryptMessage(const SecurityKey* securitykey);

	private:
		XUuid m_id;
		XUuid m_requestId;
		const Delivery* m_delivery;
		Terminal m_terminal;
		std::wstring m_sourceapp;
		std::string m_sourcehost;
		int m_sourceport;
		std::string m_exmessage;
		time_t m_time;
	};
}

#endif