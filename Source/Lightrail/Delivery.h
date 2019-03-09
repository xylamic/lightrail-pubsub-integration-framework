/*!
@file	Delivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef DELIVERY_H
#define DELIVERY_H

#include "Lightrail_global.h"
#include "Terminal.h"
#include "IDelivery.h"
#include "SecurityKey.h"
#include <XByteArray.h>
#include <XSerializer.h>
#include <XUtilities.h>
#include <XException.h>
#include <map>

namespace Xylasoft
{
	/*!
	Concrete implementation for a delivery.
	*/
	class Delivery : public IDelivery
	{
	public:
		Delivery();

		Delivery(const Delivery& other);

		Delivery(XSerializer& serializer);

		virtual ~Delivery();

		virtual void Serialize(XSerializer& serializer) const;

		virtual std::wstring MessageString() const override;

		virtual void SetMessageString(const std::wstring& message) override;

		virtual void SetMessage(const char* data, long dataSize, Encoding::EncodingType encoding = Encoding::BINARY) override;

		virtual Encoding::EncodingType Encoding() const override;

		virtual const char* Message() const override;

		virtual long MessageSize() const override;

		virtual void EncryptMessge(const SecurityKey* securitykey);

		virtual void DecryptMessage(const SecurityKey* securitykey);

		/*!
		Set or add a label with the given value. If value is NULL, the
		label is removed.
		@param[in] name The label name.
		@param[in] value The label value.
		*/
		virtual void SetLabel(const wchar_t* name, const wchar_t* value) override;

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

	private:
		XByteArray m_data;
		Encoding::EncodingType m_encoding;
		std::map<std::wstring, std::wstring> m_labels;
	};
}

#endif