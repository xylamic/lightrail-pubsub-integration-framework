/*!
@file	XHostAddress.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Tests.h"

int doit()
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if (sd == SOCKET_ERROR) {
        cerr << "Failed to get a socket. Error " << WSAGetLastError() <<
            endl; return 1;
    }

    INTERFACE_INFO InterfaceList[20];
    unsigned long nBytesReturned;
    if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
			sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
        cerr << "Failed calling WSAIoctl: error " << WSAGetLastError() <<
				endl;
		return 1;
    }

	//char buffer[256];

    int nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
    cout << "There are " << nNumInterfaces << " interfaces:" << endl;
    for (int i = 0; i < nNumInterfaces; ++i) {
        cout << endl;

        sockaddr_in *pAddress;
		sockaddr* addr = &InterfaceList[i].iiAddress.Address;
		if (addr->sa_family == AF_INET)
		{
			sockaddr_in* addrv4 = (sockaddr_in*)addr;
			cout << " " << ::inet_ntoa(addrv4->sin_addr);
			//cout << " " << inet_ntop(AF_INET, &addrv4->sin_addr, buffer, 256);
		}
		else
		{
			/*
			sockaddr_in6* addrv6 = (sockaddr_in6*)addr;
			cout << " " << inet_ntop(AF_INET6, &addrv6->sin6_addr, buffer, 256);
			*/
		}

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiBroadcastAddress);
        cout << " has bcast " << inet_ntoa(pAddress->sin_addr);

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiNetmask);
        cout << " and netmask " << inet_ntoa(pAddress->sin_addr) << endl;

        cout << " Iface is ";
        u_long nFlags = InterfaceList[i].iiFlags;
        if (nFlags & IFF_UP) cout << "up";
        else                 cout << "down";
        if (nFlags & IFF_POINTTOPOINT) cout << ", is point-to-point";
        if (nFlags & IFF_LOOPBACK)     cout << ", is a loopback iface";
        cout << ", and can do: ";
        if (nFlags & IFF_BROADCAST) cout << "bcast ";
        if (nFlags & IFF_MULTICAST) cout << "multicast ";
        cout << endl;
    }

    return 0;
}

void InterfaceLookupTest()
{
	WSADATA WinsockData;
    if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
        cerr << "Failed to find Winsock 2.2!" << endl;
        return;
    }

    int nRetVal = doit();

    WSACleanup();
}