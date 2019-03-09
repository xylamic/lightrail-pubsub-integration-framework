/*!
@file	LightrailDetailedDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

#include "ITerminalDelivery.h"
#include "LightrailDelivery.h"
#include "Method.h"

namespace Xylasoft
{
	/// <summary>
	/// This contains a message to be received by a Terminal.
	/// </summary>
	public ref class DetailedDelivery
	{
	internal:
		/// <summary>
		/// Construct a message for delivery to a client.
		/// </summary>
		/// <param name="delivery">The raw message to load.</param>
		/// <param name="receiptTerminals">The list of terminals this delivery was received on.</param>
		DetailedDelivery(const Xylasoft::IDetailedDelivery* delivery, array<String^>^ receiptTerminals);

	public:
		/// <summary>
		/// Get the binary message.
		/// </summary>
		array<Byte>^ Message();

		/// <summary>
		/// Get the message encoding.
		/// </summary>
		DeliveryEncoding Encoding();

		/// <summary>
		/// Get the message as a string based on the contained encoding.
		/// </summary>
		String^ MessageString();

		/// <summary>
		/// Get the label value. If the label is not found, NULL is returned.
		/// </summary>
		/// <param name="name">The name of the label.</param>
		/// <returns>The value or NULL if not found.</returns>
		String^ Label(String^ name);

		/// <summary>
		/// Get the list of label names.
		/// </summary>
		/// <returns>The list of label names.</returns>
		array<String^>^ GetLabelNameList();

		/// <summary>
		/// Get the host of the source application.
		/// </summary>
		String^ SourceHost();

		/// <summary>
		/// Get the source application name.
		/// </summary>
		String^ SourceApplication();

		/// <summary>
		/// Get the port of the source application.
		/// </summary>
		long SourcePort();

		/// <summary>
		/// Get the name of the terminal this delivery was sent from.
		/// </summary>
		String^ SourceTerminalName();

		/// <summary>
		/// Get the list of terminals this message was received on.
		/// </summary>
		array<String^>^ ReceiptTerminalNames();

		/// <summary>
		/// Get the time the message was sent in UTC.
		/// </summary>
		DateTime SendTimeUtc();

	protected:
		array<Byte>^ ConvertNativeBytesToByteArray(const char* binary, long size);

	private:
		String^ m_sourceApplication;
		String^ m_sourceHost;
		String^ m_sourceTerminal;
		array<String^>^ m_receiptTerminalNames;
		long m_sourcePort;
		time_t m_sendTime;
		Delivery^ m_delivery;
	};
}