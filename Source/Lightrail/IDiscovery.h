/*!
@file	IDiscovery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef IDISCOVERY_H
#define IDISCOVERY_H

#include "Lightrail_global.h"
#include "SimpleListContainer.h"

namespace Xylasoft
{
	/*!
	This defines a particular participating application on the network.
	*/
	class LIGHTRAILSHARED_EXPORT Participant
	{
	public:
		/*!
		Constructor an empty participant.
		*/
		Participant()
		{
		}

		/*!
		Construct a participant.
		@param[in] hostname The name of the host.
		@param[in] port The port number.
		*/
		Participant(const char* hostname, long port)
		{
			::strcpy(this->Hostname, hostname);
			this->Port = port;
		}

		/*!
		Copy constructor. Creates a deep copy.
		@param[in] other The participant to copy.
		*/
		Participant(const Participant& other)
		{
			::strcpy(this->Hostname, other.Hostname);
			this->Port = other.Port;
		}

		/*!
		Assignment overload. Created a deep copy.
		@param[in] other the participant to copy.
		@returns This object for chaining.
		*/
		Participant& operator=(const Participant& other)
		{
			::strcpy(this->Hostname, other.Hostname);
			this->Port = other.Port;

			return *this;
		}

		/*!
		The host name.
		*/
		char Hostname[256];

		/*!
		The port number.
		*/
		long Port;
	};

	/*!
	Export of the simple list container for exposing lists of participants.
	*/
	template class LIGHTRAILSHARED_EXPORT SimpleListContainer<Participant>;

	/*!
	Interface for discovery services.
	*/
	class LIGHTRAILSHARED_EXPORT IDiscovery
	{
	public:
		/*!
		Destroy the discovery object.
		*/
		virtual ~IDiscovery() { }

		/*!
		Start discovery.
		*/
		virtual void StartDiscovery() = 0;

		/*!
		Stop discovery.
		*/
		virtual void StopDiscovery() = 0;

		/*!
		Get the list of participants that are within multicast range.
		@returns The list of particpants.
		*/
		virtual SimpleListContainer<Participant>* GetSubnetParticipants() = 0;
	};
}

#endif