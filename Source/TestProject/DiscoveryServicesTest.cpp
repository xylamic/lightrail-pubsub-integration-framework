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

#ifndef EXCLUDETESTLIBS

void DiscoveryServicesTest()
{
	int localPort;

	cout << "Enter the local port for discovery: ";
	cin >> localPort;
	cout << "Setting for port " << localPort << endl;

	DiscoveryServices ds;
	ds.StartServices(localPort);

	cin.get(); cin.get();

	ds.StopServices();
}

#else
void DiscoveryServicesTest()
{
}
#endif