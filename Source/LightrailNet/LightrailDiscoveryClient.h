/*!
@file	LightrailDiscoveryClient.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

#include "Lightrail.h"
#include "IDiscovery.h"
#include "LightrailDiscoveredParticipant.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Xylasoft
{
	/// <summary>
	/// Defines a client that has been discovered on the network.
	/// </summary>
	public ref class DiscoveryClient
	{
	public:
		/// <summary>
		/// Create a discovery client.
		/// </summary>
		DiscoveryClient();

		/// <summary>
		/// Clean up the discovery client.
		/// </summary>
		~DiscoveryClient();

		/// <summary>
		/// Get the list of active participants on the network.
		/// </summary>
		/// <returns>The list of participants.</returns>
		array<DiscoveredParticipant^>^ GetParticipants();
			
	private:
		Xylasoft::IDiscovery* m_client;
	};
}