/*!
@file	XSocketDevice.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XSocketDevice.h"

#define MAX_CONNECTIONS 10

using namespace Xylasoft;

const std::string ACK = "ack";

XSocketDevice::XSocketDevice()
{
	::VerifyPlatformCommunications();
}

XSocketDevice::~XSocketDevice()
{
}

SOCKET XSocketDevice::CreateSocket(SocketType type)
{
	SOCKET socket = -1;

	switch (type)
	{
	case TCP:
		socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		break;
	case UDP:
		socket = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		break;
	}

	if (socket < 0)
	{
		throw BindException(this->GetErrorString(L"The socket could not be created.").c_str());
	}

	return socket;
}

void XSocketDevice::BindSocket(SOCKET socket, int port, const char* ifAddress)
{
	sockaddr* addr;
	int addrSize;

	sockaddr_in addrin;
	::memset(&addrin, 0, sizeof(sockaddr_in));

	addrSize = sizeof(sockaddr_in);
	addr = (sockaddr*)&addrin;

	addrin.sin_family = AF_INET;

	if (port <= 0)
	{
		addrin.sin_port = htons(0);
	}
	else
	{
		addrin.sin_port = htons(port);
	}

	if (ifAddress == nullptr)
	{
		addrin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	}
	else
	{
		this->ParseAddress(ifAddress, &addrin.sin_addr);
	}

	char yes = '1';
	if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0)
	{
		throw BindException(this->GetErrorString(L"The socket option for address reuse failed.").c_str());
	}

	if (::bind(socket, addr, addrSize) != 0)
	{
		throw BindException(this->GetErrorString(L"The connection could not be bound to the address and port.").c_str());
	}
}

void XSocketDevice::JoinMulticastGroup(SOCKET socket, const char* address, const char* ifAddress)
{
	/*
	** Saved in case TTL becomes important at some point **
	int mc_ttl = 31;
	if (::setsockopt(this->m_socketHandle, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &mc_ttl, sizeof(mc_ttl)) < 0)
	{
		throw BindException(L"Failed to set the multicast TTL.");
	}
	*/

	ip_mreq mreq;
	memset(&mreq, 0, sizeof(ip_mreq));

	if (ifAddress == nullptr)
	{
		mreq.imr_interface.S_un.S_addr = htonl(INADDR_ANY);
	}
	else
	{
		this->ParseAddress(ifAddress, &mreq.imr_interface);
	}

	this->ParseAddress(address, &mreq.imr_multiaddr);

	if (::setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(ip_mreq)) != 0)
	{
		throw BindException(this->GetErrorString(L"Failed to join the multicast group, invalid address.").c_str());
	}
}

void XSocketDevice::CloseSocket(SOCKET socket)
{
	try
	{
		linger l;
		l.l_onoff = 1;
		l.l_linger = 10000;

		::setsockopt(socket, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof(linger));

#ifdef _WIN32
		::closesocket(socket);
#endif
#ifdef linux
		::close(socket);
#endif
	}
	catch (...)
	{ }
}

void XSocketDevice::SendMulticast(SOCKET socket, const char* address, int port, const char* data, long dataSize, const char* ifAddress,
	int ttl)
{
	SOCKET sendSocket = this->CreateSocket(XSocketDevice::UDP);

	// set the interface to send from if specified
	if (ifAddress != nullptr)
	{
		struct in_addr bindAddr;

		this->ParseAddress(ifAddress, &bindAddr);

 		if (::setsockopt(sendSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&bindAddr, sizeof(in_addr)) != 0)
		{
			throw BindException(L"The socket could not interface with the specified address.");
		}
	}

	// set the multicast time to live
	char cttl = ttl;
	if (::setsockopt(sendSocket, IPPROTO_IP, IP_MULTICAST_TTL, &cttl, sizeof(char)) != 0)
	{
		throw BindException(L"The time to live could not be set.");
	}

	struct sockaddr_in sendAdd;
	::memset(&sendAdd, 0, sizeof(sockaddr_in));

	this->ParseAddress(address, &sendAdd.sin_addr);
	sendAdd.sin_family = AF_INET;
	sendAdd.sin_port = htons(port);

	// send the message
	int size = sizeof(sockaddr_in);
	if (::sendto(sendSocket, data, dataSize, 0, (sockaddr*)&sendAdd, size) < 0)
	{
		throw CommunicationException(this->GetErrorString(L"Failed to send the message.").c_str());
	}

	::closesocket(sendSocket);
}

void XSocketDevice::ReceiveFrom(SOCKET socket, XByteArray& buffer, char* peerAddress)
{
	struct sockaddr_storage peerAddr;
	::memset(&peerAddr, 0, sizeof(sockaddr_storage));

	int size = sizeof(sockaddr_storage);
	buffer.Resize(4000);
	int msgSize = ::recvfrom(socket, buffer.Data(), buffer.Size(), 0, (struct sockaddr*)&peerAddr, &size);

	if (msgSize < 0)
	{
		throw CommunicationException(this->GetErrorString(L"The receive was interrupted.").c_str());
	}
	else
	{
		// resize to the actual size retrieved
		buffer.Resize(msgSize);
	}
}

void XSocketDevice::ParseAddress(const char* address, void* store)
{
	::VerifyPlatformCommunications();

	unsigned long addr = ::inet_addr(address);
	if (addr == INADDR_NONE)
	{
		throw BindException(L"The address could not be parsed.");
	}

	::memcpy(store, &addr, sizeof (unsigned long));
	/*
	if (!inet_pton(AF_INET, address, store))
	{
		throw BindException(L"The address could not be parsed.");
	}
	*/
}

int XSocketDevice::Receive(SOCKET socket, char* buffer, long bufferSize)
{
	long bytesrecv = ::recv(socket, buffer, bufferSize, 0);
	if (bytesrecv < 0)
	{
		throw CommunicationException(this->GetErrorString(L"The receipt of the message failed.").c_str());
	}

	return bytesrecv;
}

std::vector<std::string> XSocketDevice::GetAddressForHost(const char* hostname)
{
	::VerifyPlatformCommunications();

	hostent* hostinfo = gethostbyname(hostname);
	if (hostinfo == nullptr) 
	{
		throw ResolutionException(GetErrorString(L"The name could not be resolved.").c_str());
	}

	std::vector<std::string> addresses;

	int index = 0;
	while (hostinfo->h_addr_list[index] != nullptr)
	{
		char* address = hostinfo->h_addr_list[index];
		in_addr addr = *(struct in_addr *) address;

		std::string addressstring = GetPresentationAddress(AF_INET, &addr);
		addresses.push_back(addressstring);

		index++;
	}

	return addresses;
}

std::string XSocketDevice::GetLocalHostName()
{
	::VerifyPlatformCommunications();

	char name[NI_MAXHOST];
	
	if (::gethostname(name, NI_MAXHOST) != 0)
	{
		throw ResolutionException(L"The local host name could not be retrieved.");
	}

	return std::string(name);
}

std::string XSocketDevice::GetDefaultAddressForLocalHost()
{
	::VerifyPlatformCommunications();

	std::string host = GetLocalHostName();

	std::vector<std::string> addresses = GetAddressForHost(host.c_str());

	if (addresses.size() == 0)
	{
		throw ResolutionException(L"No addresses were found.");
	}

	return addresses[0];
}

void XSocketDevice::SendAll(SOCKET socket, const char* data, long dataSize)
{
	long total = 0;
	long bytesleft = dataSize;
	long bytessent;

	while(total < dataSize)
	{
		bytessent = ::send(socket, data + total, bytesleft, 0);
		if (bytessent < 0)
		{
			throw CommunicationException(this->GetErrorString(L"Failed to send the message.").c_str());
		}

		total += bytessent;
		bytesleft -= bytessent;
	}
}

std::string XSocketDevice::GetPresentationAddress(int family, void* addr)
{
	::VerifyPlatformCommunications();

	//char buffer[256];

	if (const char* address = ::inet_ntoa(*static_cast<in_addr*>(addr)) /*::inet_ntop(family, addr, buffer, 256)*/)
	{
		return std::string(address);
	}
	else
	{
		return std::string();
	}
}

void XSocketDevice::SendAck(SOCKET socket)
{
	this->SendAll(socket, ACK.c_str(), static_cast<long>(ACK.size()));
}

void XSocketDevice::ReceiveAck(SOCKET socket)
{
	char buffer[1024];

	// receive the message with buffer-1 in length to make sure we have space for a '\0'
	int bytesrecv = ::recv(socket, buffer, 1023, 0);

	// if the call failed
	if (bytesrecv < 0)
	{
		throw CommunicationException(this->GetErrorString(L"The acknowledgement was incomplete.").c_str());
	}
	// if we did not receive the whole message
	else if (bytesrecv < static_cast<int>(ACK.size()))
	{
		throw CommunicationException(L"The acknowledgement was incomplete.");
	}
	else
	{
		// make sure we have a string end for the message
		buffer[bytesrecv] = '\0';
				
		// make sure the acknowledgement matches
		if (ACK.compare(buffer) != 0)
		{
			throw CommunicationException(L"The acknowledgement was corrupt.");
		}
	}
}

void XSocketDevice::Listen(SOCKET socket)
{
	if (::listen(socket, MAX_CONNECTIONS) != 0)
	{
		throw BindException(this->GetErrorString(L"The socket is not listening.").c_str());
	}
}

void XSocketDevice::Connect(SOCKET socket, const char* sendAddress, int port)
{
	sockaddr* addr;
	int addrSize;

	sockaddr_in addrin;
	::memset(&addrin, 0, sizeof(sockaddr_in));

	addrSize = sizeof(sockaddr_in);
	addr = (sockaddr*)&addrin;

	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(port);
	this->ParseAddress(sendAddress, &addrin.sin_addr);

	if (::connect(socket, addr, addrSize) != 0)
	{
		throw BindException(this->GetErrorString(L"The connection could not be established.").c_str());
	}
}

SOCKET XSocketDevice::AcceptConnection(SOCKET socket, std::string& peerAddress, int& peerPort)
{
	struct sockaddr_storage peerAddr;
	socklen_t peerAddrSize = sizeof(sockaddr_in6);

	// accept the connection
	SOCKET newSocket = ::accept(socket, (sockaddr*)&peerAddr, &peerAddrSize);
	if (newSocket < 0)
	{
		throw BindException(this->GetErrorString(L"A connection could not be established.").c_str());
	}

	// get the peer information
	if (peerAddr.ss_family == AF_INET)
	{
		sockaddr_in* addr = (sockaddr_in*)&peerAddr;
		peerAddress = this->GetPresentationAddress(AF_INET, &addr->sin_addr);
		peerPort = addr->sin_port;
	}
	else
	{
		sockaddr_in6* addr = (sockaddr_in6*)&peerAddr;
		peerAddress = this->GetPresentationAddress(AF_INET6, &addr->sin6_addr);
		peerPort = addr->sin6_port;
	}

	return newSocket;
}

void XSocketDevice::GetBoundAddressForSocket(SOCKET socket, std::string& address, int& port)
{
	struct sockaddr_storage lclAddr;
	socklen_t lclAddrSize = sizeof(sockaddr_in6);

	if (::getsockname(socket, (sockaddr*)&lclAddr, &lclAddrSize) != 0)
	{
		throw ResolutionException(this->GetErrorString(L"The socket could not be resolved.").c_str());
	}

	// get the socket information
	if (lclAddr.ss_family == AF_INET)
	{
		sockaddr_in* addr = (sockaddr_in*)&lclAddr;
		address = this->GetPresentationAddress(AF_INET, &addr->sin_addr);
		port = ::ntohs(addr->sin_port);
	}
	else
	{
		sockaddr_in6* addr = (sockaddr_in6*)&lclAddr;
		address = this->GetPresentationAddress(AF_INET6, &addr->sin6_addr);
		port = ::htons(addr->sin6_port);
	}
}

std::wstring XSocketDevice::GetErrorString(const wchar_t* message)
{
	std::wstring msg(message);
	msg.append(L" Error: ");

#ifdef _WIN32
	errno_t error = ::WSAGetLastError();
	switch (error)
	{
	case WSA_INVALID_HANDLE:
		msg.append(L"The socket handle was invalid.");
		break;
	case WSANOTINITIALISED:
		msg.append(L"Winsock was not initialized.");
		break;
	case WSAENETDOWN:
		msg.append(L"The network interface was down.");
		break;
	case WSAEADDRINUSE:
		msg.append(L"The address is already in use.");
		break;
	case WSAEADDRNOTAVAIL:
		msg.append(L"The address was invalid.");
		break;
	case WSAEINPROGRESS:
		msg.append(L"The socket is already in use in a blocking call.");
		break;
	case WSAEINVAL:
		msg.append(L"The socket is already bound or invalid argument was given.");
		break;
	case WSAENOTSOCK:
		msg.append(L"The handle was not a socket.");
		break;
	case WSAEAFNOSUPPORT:
		msg.append(L"The IP family is not supported.");
		break;
	case WSAEMFILE:
		msg.append(L"No sockets available.");
		break;
	case WSAENOBUFS:
		msg.append(L"No buffer space available or not connections available.");
		break;
	case WSAEPROTONOSUPPORT:
		msg.append(L"The specified protocol is unsupported.");
		break;
	case WSAEPROTOTYPE:
		msg.append(L"The protocol did not match the socket.");
		break;
	case WSAEPROVIDERFAILEDINIT:
		msg.append(L"The service provider failed to initialize.");
		break;
	case WSAENETRESET:
		msg.append(L"The connection timed out.");
		break;
	case WSAENOPROTOOPT:
		msg.append(L"The option is unknown or unsupported.");
		break;
	case WSAENOTCONN:
		msg.append(L"The socket is not connected/reset.");
		break;
	case WSAEOPNOTSUPP:
		msg.append(L"The operation is not supported by the socket type.");
		break;
	case WSAESHUTDOWN:
		msg.append(L"The socket was shut down.");
		break;
	case WSAEWOULDBLOCK:
		msg.append(L"The socket is in unblocking mode, a blocking call cannot be made.");
		break;
	case WSAEMSGSIZE:
		msg.append(L"The message size exceeded the supporting transport.");
		break;
	case WSAEHOSTUNREACH:
		msg.append(L"The host could not be reached.");
		break;
	case WSAEDESTADDRREQ:
		msg.append(L"The destination address is required.");
		break;
	case WSAENETUNREACH:
		msg.append(L"The network device couldn't be reached.");
		break;
	case WSAETIMEDOUT:
		msg.append(L"The connection was timed out.");
		break;
	case WSAECONNREFUSED:
		msg.append(L"The connetion was refused.");
		break;
	case WSAEFAULT:
		msg.append(L"There was invalid argument given to the socket call.");
		break;
	case WSAEINTR:
		msg.append(L"A blocking call was interrupted.");
		break;
	case WSANO_DATA:
		msg.append(L"There was no information available.");
		break;
	case WSAHOST_NOT_FOUND:
		msg.append(L"The host was not found.");
		break;
	default:
		std::wostringstream stream;
		stream << error;

		msg.append(stream.str());
		break;
	}
#endif
#ifdef linux
	errno_t error = ::errorno();
	msg.append(L"Linux error.");
#endif
	return msg;
}