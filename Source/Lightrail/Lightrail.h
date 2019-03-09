/*!
@file	Lightrail.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef LIGHTRAIL_H
#define LIGHTRAIL_H

#include "Lightrail_global.h"
#include "ILightrail.h"
#include "ITerminal.h"
#include "IDelivery.h"
#include "ISecurityKey.h"
#include "IDiscovery.h"

namespace Xylasoft
{
	/*!
	This class provides the generation of Lightrail objects.
	*/
	class LIGHTRAILSHARED_EXPORT Lightrail
	{
	public:
		/*!
		Get a Lightrail instance. This instance must be deleted when finished. 'delete lightrail;'
		@param[in] name The name of the instance.
		@param[in] description The description of the instance.
		@param[in] instance The unique name of the instance. This must be specified and unique if using insured deliveries.
		@returns The new Lightrail instance.
		*/
		static ILightrail* GetLightrailInstance(const wchar_t* name, const wchar_t* description, const wchar_t* instance = nullptr);

		/*!
		Get a discovery instance for discovering applications on the infrastructure.
		@returns The discovery instance.
		*/
		static IDiscovery* GetDiscoveryInstance();

		/*!
		Create a new terminal for specifying a point of contact in a Lightrail instance. This can be passed into a Lightrail instance for registration.
		If the instance is NOT passed to an instance, it needs to be deleted. 'delete terminal;'
		@param[in] name The name of the terminal.
		@param[in] description The description of the terminal.
		@param[in] terminalstring The terminal string for matching deliveries.
		@param[in] securityKey The security key name if using security.
		@returns The new terminal instance.
		*/
		static const ITerminal* NewTerminal(const wchar_t* name, const wchar_t* description, const wchar_t* terminalstring, const wchar_t* securityKey = nullptr);

		/*!
		Create a new delivery for sending through a Lightrail instance. If the delivery is NOT passed to an instance, it needs to be deleted. 'delete delivery;'
		@returns The new delivery instance.
		*/
		static IDelivery* NewDelivery();

		/*!
		Create a new security key using AES 256-bit encryption for use in sending/receiving encrypted deliveries.
		@param[in] name The name of the key.
		@param[in] key The key to utilize. Ideally this will be exactly 32 bytes, but any length of
		key will work.
		@param[in] keySize The length of the key.
		@returns The new security key.
		*/
		static const ISecurityKey* NewAES256SecurityKey(const wchar_t* name, const char* key, long keySize);

	private:
		/* static class */
		Lightrail();
		~Lightrail();
	};
}

#endif
