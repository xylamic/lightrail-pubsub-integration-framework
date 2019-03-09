/*!
@file	XSocketDevice.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XSOCKETDEVICE_H
#define XSOCKETDEVICE_H

#include "Xylasoft.h"
#include "XException.h"
#include "XByteArray.h"

#include <sstream>
#include <vector>

#define DGRAM_BUFFER_SIZE 10240
#define STREAM_BUFFER_SIZE 10240

namespace Xylasoft
{
	/*!
	A SocketDevice provides foundational logic for working with sockets.
	*/
	class XSocketDevice
	{
	public:
		/*!
		Get the list of addresses for the specified host.
		@param[in] hostname The host name.
		@returns The list of addresses
		*/
		static std::vector<std::string> GetAddressForHost(const char* hostname);

		/*!
		Get the local host name.
		@returns The host name.
		*/
		static std::string GetLocalHostName();

		/*!
		Get the default address for the local host.
		@returns The first address in the local list.
		*/
		static std::string GetDefaultAddressForLocalHost();

	protected:
		/*!
		Enumeration defining the type of socket.
		*/
		enum SocketType
		{
			/*! TCP protocol */
			TCP = 1,

			/*! UDP protocol. */
			UDP = 2
		};

		/*! Enumerationg defining the version of IP address */
		enum IPVersion
		{
			/*! Not specified */
			NOTSPECIFIED = 0,

			/*! IP Version 4 */
			V4 = 1,

			/*! IP Version 6 */
			V6 = 2
		};

		/*!
		Default constructor for a socket device.
		*/
		XSocketDevice();

		/*!
		Destructor.
		*/
		virtual ~XSocketDevice();

		/*!
		Create a socket.
		@param[in] type The type of socket.
		@returns The socket handle.
		*/
		SOCKET CreateSocket(SocketType type);

		/*!
		Bind the socket.
		@param[in] socket The socket to bind.
		@param[in] port The port to bind.
		@param[in] ifAddress The interface address to bind to. Optional.
		*/
		void BindSocket(SOCKET socket, int port, const char* ifAddress = nullptr);

		/*!
		Join a multicast group.
		@param[in] socket The socket handle.
		@param[in] address The multicast address.
		@param[in] ifAddress The interface addres to bind to. Optional.
		*/
		void JoinMulticastGroup(SOCKET socket, const char* address, const char* ifAddress = nullptr);

		/*!
		Close a socket.
		@param[in] socket The socket handle.
		*/
		void CloseSocket(SOCKET socket);

		/*!
		Send a multicast message.
		@param[in] socket The socket handle.
		@param[in] address The address to send to.
		@param[in] port The port to send to.
		@param[in] data The data to send.
		@param[in] dataSize The size of the data to send.
		@param[in] ifAddress The interface address to bind to when sending.
		@param[in] ttl The multicast time to live.
		*/
		void SendMulticast(SOCKET socket, const char* address, int port, const char* data, long dataSize, 
			const char* ifAddress = nullptr, int ttl = 1);

		/*!
		Receive a message from a connectionless socket.
		@param[in] socket The socket handle.
		@param[in] msgBuffer The buffer for the data received.
		@param[in] msgBufferSize The size of the buffer for receiving.
		@param[out] peerAddress The address of the peer that sent the data.
		@returns The size of the data received.
		*/
		void ReceiveFrom(SOCKET socket, XByteArray& buffer, char* peerAddress);

		/*!
		Send all of the specified data through a connected socket.
		@param[in] socket The socket handle.
		@param[in] data The data to send.
		@param[in] dataSize The size of the data to send.
		*/
		void SendAll(SOCKET socket, const char* data, long dataSize);

		/*!
		Receive a message from a connected socket.
		@param[in] socket The socket handle.
		@param[in] buffer The buffer to store data in.
		@param[in] bufferSize The size of the buffer.
		@returns The size of the data received.
		*/
		int Receive(SOCKET socket, char* buffer, long bufferSize);

		/*!
		Receive an acknowledgement from a connected socket.
		@param[in] socket The socket handle.
		*/
		void ReceiveAck(SOCKET socket);

		/*!
		Send an acknowledgement through a connected socket.
		@param[in] socket The socket handle
		*/
		void SendAck(SOCKET socket);

		/*!
		Start listening from a bound socket.
		@param[in] socket The socket handle.
		*/
		void Listen(SOCKET socket);

		/*!
		Connect to a host
		@param[in] socket The socket handle.
		@param[in] sendAddress The address to connect to.
		@param[in] port The port to connect to.
		*/
		void Connect(SOCKET socket, const char* sendAddress, int port);

		/*!
		Accept a connection on a listening socket.
		@param[in] socket The socket handle.
		@param[out] peerAddress The address of the peer that connected.
		@param[out] peerPort The port of the peer that connected.
		@returns The socket handle for the new connection.
		*/
		SOCKET AcceptConnection(SOCKET socket, std::string& peerAddress, int& peerPort);

		/*!
		Get the address bound to the given socket.
		@param[in] socket The socket identifier.
		@param[out] address The formatted IP address.
		@param[out] port The port number.
		*/
		void GetBoundAddressForSocket(SOCKET socket, std::string& address, int& port);

	private:
		/*!
		Parse the given address string into a network long.
		@param[in] address The string address.
		@param[out] store The location to store the address information.
		*/
		static void ParseAddress(const char* address, void* store);

		/*!
		Get the formatted form of the specified network address.
		@param[in] family The IP family.
		@param[in] addr The network address.
		@returns The formatted IP address.
		*/
		static std::string GetPresentationAddress(int family, void* addr);

		/*!
		Get the text for the error type.
		@param[in] message The message to give context to the error code.
		@returns The formatted error string.
		*/
		static std::wstring GetErrorString(const wchar_t* message);
	};
}

#endif