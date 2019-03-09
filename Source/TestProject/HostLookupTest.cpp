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
#include "XHostLookup.h"
#include "XHostAddress.h"

using namespace Xylasoft;

void PrintHostList(list<XHostAddress> l)
{
	for (list<XHostAddress>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << "Address: " << it->Address() << " Type:" << it->Type() << endl;
	}
}

void XHostLookupTest()
{
	cout << "[Local Host, No Type]" << endl;
	XHostLookup h;
	PrintHostList(h.BindAddresses());

	cout << "\n[Local Host, IPv4]" << endl;
	XHostLookup h2(XHostAddress::IPv4);
	PrintHostList(h2.BindAddresses());

	cout << "\n[Host, No Type]" << endl;
	XHostLookup h3("xylamic-lapx64");
	PrintHostList(h3.BindAddresses());

	cout << "\n[Host, IPv4]" << endl;
	XHostLookup h4("xylamic-lapx64", XHostAddress::IPv4);
	PrintHostList(h4.BindAddresses());

	cout << "\n[Host, IPv6]" << endl;
	XHostLookup h5("xylamic-lapx64", XHostAddress::IPv6);
	PrintHostList(h5.BindAddresses());
}