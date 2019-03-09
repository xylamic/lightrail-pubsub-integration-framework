/*!
@file	ILightrail.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef ILIGHTRAIL_H
#define ILIGHTRAIL_H

#include "Lightrail_global.h"
#include "Method.h"
#include "ITerminal.h"
#include "IDelivery.h"
#include "IDetailedDelivery.h"
#include "ITerminalDelivery.h"
#include "ISecurityKey.h"
#include "IDeliveryParseError.h"
#include <XSingleEventCallback.h>

namespace Xylasoft
{
	template class LIGHTRAILSHARED_EXPORT XSingleEventCallback<ITerminalDelivery&>;
	template class LIGHTRAILSHARED_EXPORT XSingleEventCallback<ITerminalReturnDelivery&>;
	template class LIGHTRAILSHARED_EXPORT XSingleEventCallback<const IDeliveryParseError&>;

	/*!
	This defines the interface for utilizing the Lightrail communication system.
	*/
	class LIGHTRAILSHARED_EXPORT ILightrail
	{
	public:
		/*!
		Destroy the lightrail instance.
		*/
		virtual ~ILightrail() { }

	public:
		/*!
		Get the name.
		@returns The name.
		*/
		virtual const wchar_t* Name() = 0;

		/*!
		Get the instance.
		@returns The instance.
		*/
		virtual const wchar_t* Instance() = 0;

		/*!
		Get the description.
		@returns The description.
		*/
		virtual const wchar_t* Description() = 0;

	public:
		/*!
		Start deliveries and kick off the Lightrail.
		*/
		virtual void StartDeliveries() = 0;

		/*!
		Stop deliveries and pull Lightrail off the lines.
		*/
		virtual void StopDeliveries() = 0;

		/*!
		Register a security for secure deliveries.
		@param[in] key The key pass for encryption/decryption.
		*/
		virtual void RegisterSecurityKey(const ISecurityKey* securityKey) = 0;

		/*
		Deregister a security key from use. The key must not be currently used by a terminal.
		@param[in] name The name of the key to remove.
		*/
		virtual void DeregisterSecurityKey(const wchar_t* name) = 0;

		/*!
		Register a local terminal for deliveries to/from.
		@param[in] standpoint The purpose of this terminal.
		@param[in] terminal The terminal to assign.
		*/
		virtual void RegisterTerminal(Standpoint::StandpointType standpoint, const ITerminal* terminal) = 0;

		/*!
		Deregister a local terminal and decommission it.
		@param[in] standpoint The purpose of the terminal.
		@param[in] name The name of the terminal.
		*/
		virtual void DeregisterTerminal(Standpoint::StandpointType standpoint, const wchar_t* name) = 0;

	public:
		/*!
		Publish a delivery.
		@param[in] delivery The delivery to publish.
		@param[in] terminalName The terminal to send it from.
		*/
		virtual void Publish(const wchar_t* terminalName, const IDelivery* delivery) = 0;

		/*!
		Send a delivery request and wait for a return delivery.
		@param[in] deivery The delivery request to send.
		@param[in] terminalName The terminal to send from.
		@param[in] timeout The timeout to wait for a return.
		@returns The returned delivery.
		*/
		virtual const IDetailedDelivery* Request(const wchar_t* terminalName, const IDelivery* delivery, long timeout = 30000, long reqResponders = 1) = 0;

		/*!
		Event for receiving a published delivery.
		*/
		XSingleEventCallback<ITerminalDelivery&> OnPublicationReceived;

		/*!
		Event for received a request and returned a response delivery.
		*/
		XSingleEventCallback<ITerminalReturnDelivery&> OnRequestReceived;

		/*!
		Event for when a message was received, but could not be accepted because it
		could not be parsed or deciphered.
		*/
		XSingleEventCallback<const IDeliveryParseError&> OnDeliveryParseError;
	};
}

#endif