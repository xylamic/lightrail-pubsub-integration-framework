/*!
@file	DiscoveryPeer.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef DISCOVERYPEER_H
#define DISCOVERYPEER_H

#include "Lightrail_global.h"
#include <XException.h>
#include <XSerializer.h>

#include <sstream>
#include <vector>

namespace Xylasoft
{
	class LIGHTRAILSHARED_EXPORTTEST DiscoveryPeer
	{
	public:
		/*! The action assocaited with the peer */
		enum ActionType
		{
			/*! No additional action was performed (general update). */
			None = 0,

			/*! The peer is joining the communication */
			Joining = 1,

			/*! The peer is leaving the communication. */
			Leaving = 2
		};

		/*!
		Construct a new peer.
		@param[in] hostname The host name for the peer.
		@param[in] port The port for the peer.
		*/
		DiscoveryPeer(const char* hostname, long port);

		/*!
		Construct an object from a data string.
		@param[in] data The data to parse.
		@param[out] action The action associated with the peer.
		*/
		DiscoveryPeer(const char* data, ActionType& action);

		/*!
		Construct the peer from a serialization.
		@param[in] serializer The serializer to deserialize from.
		*/
		DiscoveryPeer(XSerializer& serializer);

		/*!
		Assignment operator. Creates a complete copy.
		@param[in] other The other instance to copy.
		@returns This instance for chaining.
		*/
		DiscoveryPeer& operator=(const DiscoveryPeer& other);

		/*!
		Less-than operator. Checks if the current instance is less
		than the given instance.
		@param[in] other The other to compare.
		@returns True if this instance is less than the other.
		*/
		bool operator<(const DiscoveryPeer& other) const;

		/*!
		greater-than operator. Checks if the current instance is less
		than the given instance.
		@param[in] other The other to compare.
		@returns True if this instance is less than the other.
		*/
		bool operator>(const DiscoveryPeer& other) const;

		/*!
		Equivelence operator. Checks if the two instances have the same
		host name and port.
		@param[in] other The other to compare.
		@returns True if they have the same hostname and port.
		*/
		bool operator==(const DiscoveryPeer& other) const;

		/*!
		Get host/port string.
		@returns The host/port string.
		*/
		std::string HostnamePort() const;

		/*!
		Get the hostname.
		*/
		std::string Hostname() const;

		/*!
		Get the port.
		*/
		long Port() const;

		/*!
		Get the last update time.
		*/
		time_t LastUpdate() const;

		/*!
		Set the last time this was updated.
		@param[in] time The time to update with.
		*/
		void SetLastUpdate(time_t time);

		/*!
		Serialize the data structure to a sendable string.
		@param[in] action The action to associate with the peer.
		*/
		std::string Serialize(ActionType action);

		/*!
		Serialize the peer.
		@param[in] serializer The serializer to serialize to.
		*/
		void Serialize(XSerializer& serializer) const;

	private:
		std::string m_hostname;
		long m_port;
		time_t m_lastUpdate;
	};
}

#endif