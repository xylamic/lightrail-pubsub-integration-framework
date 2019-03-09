/*!
@file	LightrailDiscoveredParticipant.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#pragma once

using namespace System;

namespace Xylasoft
{
	/// <summary>
	/// This defines a participant that has been discovered.
	/// </summary>
	public ref class DiscoveredParticipant
	{
	public:
		/// <summary>
		/// Get and set the host name of the participant.
		/// </summary>
		String^ Hostname;

		/// <summary>
		/// Get and set the port of the participant.
		/// </summary>
		Int32 Port;
	};
}