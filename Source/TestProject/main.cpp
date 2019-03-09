/*!
@file	XHostAddress.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include <iostream>
#include <exception>
#include <string>
#include <vector>

#include "Tests.h"

#include <XUtilities.h>

#include <Lightrail.h>
#include <ILightrail.h>

using namespace std;
using namespace Xylasoft;

void main()
{
#ifdef _WIN32
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		cout << "Starting Test Program..." << endl;
		cout << "------------------------" << endl;

		::FileEncrypterTest();

		cout << "------------------------" << endl;
		cout << "Completed Test Program. Press any key to exit." << endl;

		cin.get();
	}
	catch (std::exception& ex)
	{
		cout << "------------------------" << endl;
		cout << "Error occurred: " << ex.what() << endl;
	}

	cin.get();
}