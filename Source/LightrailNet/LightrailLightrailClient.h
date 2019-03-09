/*!
@file	LightrailLightrailClient.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

#include "Lightrail.h"
#include "ILightrail.h"
#include "LightrailDelivery.h"
#include "LightrailDetailedDelivery.h"
#include "LightrailClientCallback.h"
#include "LightrailDeliveryParseError.h"
#include <string>
#include <vcclr.h>

using namespace System;

namespace Xylasoft
{
	/// <summary>A delegate for receipt of a publication.</summary>
	public delegate void PublishReceived(DetailedDelivery^ delivery);

	/// <summary>A delegate for receipt of a native publication callback.</summary>
	public delegate void NativePublishReceived(ITerminalDelivery& delivery);

	/// <summary>A delegate for receipt of a request.</summary>
	public delegate Delivery^ RequestReceived(DetailedDelivery^ delivery);

	/// <summary>A delegate for receipt of a native request callback.</summary>
	public delegate void NativeRequestReceived(ITerminalReturnDelivery& delivery);

	/// <summary>A delegate for receipt of a parse error.</summary>
	public delegate void ParseErrorReceived(DeliveryParseError^ error);

	/// <summary>A delegate for receipt of a native parse error callback.</summary>
	public delegate void NativeParseErrorReceived(const IDeliveryParseError& error);

	/// <summary>The defined of the different types of standpoints a terminal can take.</summary>
	public enum class TerminalStandpoint { Publisher = 1, Subscriber, Responder, Requester };

	/// <summary>
	/// The LightrailClient provides all the functionality of the communication framework side of
	/// Xylasoft Lightrail.
	/// </summary>
	public ref class LightrailClient
	{
	public:
		/// <summary>
		/// Construct a Lightrail client.
		/// </summary>
		/// <param name="name">The name of the client.</param>
		/// <param name="description">The description of the client.</param>
		LightrailClient(String^ name, String^ description);
			
		/// <summary>
		/// Clean up the native memory.
		/// </summary>
		~LightrailClient();

		/// <summary>
		/// Start the Lightrail sending and receiving deliveries.
		/// </summary>
		void StartDeliveries();

		/// <summary>
		/// Stop the Lightrail from sending and receiving deliveries.
		/// </summary>
		void StopDeliveries();

		/// <summary>
		/// Register a security for use with Terminals.
		/// </summary>
		/// <param name="name">The name of the key.</param>
		/// <param name="key">The key string to use.</param>
		void RegisterSecurityKey(String^ name, array<Byte>^ key);

		/// <summary>
		/// Remove a security key from use. It must not currently be used by a Terminal.
		/// </summary>
		/// <param name="name">The name of the security key.</param>
		void DeregisterSecurityKey(String^ name);

		/// <summary>
		/// Register a Terminal for message deliveries.
		/// </summary>
		/// <param name="standpoint">The standpoint to take for this Terminal.</param>
		/// <param name="name">The name of the Terminal.</param>
		/// <param name="description">The description for the Terminal.</param>
		/// <param name="matchIdentifier">The string for matching to this Terminal.</param>
		void RegisterTerminal(TerminalStandpoint standpoint, String^ name, String^ description, 
			String^ matchIdentifier);

		/// <summary>
		/// Register a Terminal for message deliveries with security.
		/// </summary>
		/// <param name="standpoint">The standpoint to take for this Terminal.</param>
		/// <param name="name">The name of the Terminal.</param>
		/// <param name="description">The description for the Terminal.</param>
		/// <param name="matchIdentifier">The string for matching to this Terminal.</param>
		/// <param name="securityKeyName">The security key name.</param>
		void RegisterTerminal(TerminalStandpoint standpoint, String^ name, String^ description, 
			String^ matchIdentifier, String^ securityKeyName);

		/// <summary>
		/// Deregister a Terminal from use.
		/// </summary>
		/// <param name="standpoint">The standpoint of this Terminal.</param>
		/// <param name="name">The name of the Terminal.</param>
		void DeregisterTerminal(TerminalStandpoint standpoint, String^ name);

		/// <summary>
		/// Publish a message.
		/// </summary>
		/// <param name="terminalName">The name of the Terminal to send from.</param>
		/// <param name="delivery">The delivery to send.</param>
		void Publish(String^ terminalName, Delivery^ delivery);

		/// <summary>
		/// Send a rquest message to one available responder.
		/// </summary>
		/// <param name="terminalName">The name of the Terminal to send from.</param>
		/// <param name="delivery">The delivery to send.</param>
		/// <param name="timeout">The time to wait for a response in milliseconds.</param>
		DetailedDelivery^ Request(String^ terminalName, Delivery^ delivery, long timeout);

		/// <summary>
		/// Send a rquest message.
		/// </summary>
		/// <param name="terminalName">The name of the Terminal to send from.</param>
		/// <param name="delivery">The delivery to send.</param>
		/// <param name="timeout">The time to wait for a response in milliseconds.</param>
		/// <param name="reqResponders">The requested number of responders to send the request to.</param>
		DetailedDelivery^ Request(String^ terminalName, Delivery^ delivery, long timeout, long reqResponders);

		/// <summary>A publication was received.</summary>
		event PublishReceived^ OnPublishReceived;

		/// <summary>A request was received.</summary>
		event RequestReceived^ OnRequestReceived;

		/// <summary>A message was received, but could not be parsed to be deliveried to you. </summary>
		event ParseErrorReceived^ OnParseErrorReceived;

	protected:
		/// <summary>Event for receipt of a native publication.</summary>
		void OnTerminalPublishReceived(ITerminalDelivery& delivery);

		/// <summary>Event for receipt of a native request for response.</summary>
		void OnTerminalRequestReceived(ITerminalReturnDelivery& delivery);

		/// <summary>Event for receipt of a native parse error.</summary>
		void OnDeliveryParseErrorReceived(const IDeliveryParseError& error);

	private:
		/// <summary>
		/// Convert a managed byte array to a native byte array.
		/// </summary>
		/// <param name="byteArray">The byte array to convert. </param>
		/// <param name="data">The pointer to a new array of native bytes.</param>
		/// <returns>The length of the binary array.</returns>
		long ConvertByteArrayToBinaryArray(array<Byte>^ byteArray, const char** data);

		/// <summary>
		/// Convert the managed encoding to a native encoding.
		/// </summary>
		/// <param name="encoding">The managed encoding to convert.</param>
		/// <returns>The native encoding equivelent.</returns>
		Xylasoft::Encoding::EncodingType ConvertEncoding(DeliveryEncoding encoding);

	private:
		Xylasoft::ILightrail* m_client;
		LightrailClientCallback* m_nativeCallback;

		NativePublishReceived^ m_nativePublish;
		System::Runtime::InteropServices::GCHandle m_nativePublishHandle;

		NativeRequestReceived^ m_nativeRequest;
		System::Runtime::InteropServices::GCHandle m_nativeRequestHandle;

		NativeParseErrorReceived^ m_nativeParseError;
		System::Runtime::InteropServices::GCHandle m_nativeParseErrorHandle;
	};
}
