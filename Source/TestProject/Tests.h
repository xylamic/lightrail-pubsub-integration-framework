/*!
@file	XHostAddress.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef TESTS_H
#define TESTS_H

#include <iostream>
using namespace std;

#include <XMulticaster.h>
#include <XUtilities.h>
#include <XHostAddress.h>
#include <DiscoveryServices.h>
#include <XIni.h>
#include <XTcpMessageReceiver.h>
#include <XTcpMessageSender.h>
#include <XSocketDevice.h>
#include <XEventCallback.h>
#include <XSingleEventCallback.h>
#include <Terminal.h>
#include <SecurityKey.h>
#include <Lightrail.h>
#include <XSerializer.h>
#include <XUuid.h>
#include <ILightrail.h>
#include <XEncrypter.h>
#include <XLicenseFile.h>
#include <XFileEncrypter.h>

using namespace Xylasoft;

#define EXCLUDETESTLIBS

void HostLookupTest();
void TcpMessageTester();
void MulticastTester();
void XIniTest();
void DiscoveryServicesTest();
void InterfaceLookupTest();
void TerminalTest();
void SerializerTest();
void UuidTest();
void EncryptionTest();
void LightrailTests();
void LicenseFileTest();
void FileEncrypterTest();

#endif