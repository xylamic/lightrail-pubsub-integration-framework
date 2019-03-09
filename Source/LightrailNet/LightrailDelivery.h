/*!
@file	LightrailDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

#include "IDelivery.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Xylasoft
{
	/// <summary>This enumeration defines the type of encoding for use with Lightrail</summary>
	public enum class DeliveryEncoding { None, Ascii, Utf8, Utf16, Utf32 };

	/// <summary>
	/// This defines a message to be sent through a Terminal onto the Lightrail.
	/// </summary>
	public ref class Delivery
	{
	public:
		/// <summary>
		/// Construct a message from a string. Automatically converts to UTF-8 encoding.
		/// </summary>
		/// <param name="message">The message to load.</param>
		Delivery(String^ message);

		/// <summary>
		/// Construct a message from an array to bytes with a specified encoding.
		/// </summary>
		/// <param name="message">The message to load.</param>
		/// <param name="encoding">The encoding of the message.</param>
		Delivery(array<Byte>^ message, DeliveryEncoding encoding);

		/// <summary>
		/// Get the message encoding.
		/// </summary>
		DeliveryEncoding Encoding();

		/// <summary>
		/// Get the message in binary.
		/// </summary>
		array<Byte>^ Message();

		/// <summary>
		/// Get the message as a string. Will automatically convert as specified in the message encoding.
		/// </summary>
		String^ MessageString();

		/// <summary>
		/// Set or create the specified label value. If value is NULL, then
		/// the label will be removed.
		/// </summary>
		/// <param name="name">The name of the label.</param>
		/// <param name="value">The value for the label.</param>
		void SetLabel(String^ name, String^ value);

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

	private:
		array<Byte>^ m_data;
		DeliveryEncoding m_encoding;
		Dictionary<String^, String^>^ m_labels;
	};
}
