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
#include <XMutex.h>
#include <fstream>
#include <iomanip>

class TcpMessageTest
{
public:
    TcpMessageTest(int port)
        : m_rsvr(port)
    {
		m_rsvr.OnMessageReceived.SetCallback(new XEventCallback<TcpMessageTest, const XByteArray&>(
                                   this, &TcpMessageTest::MessageReceived));
        received = 0;
    }

    void Start()
    {
        m_rsvr.StartListening();
    }

	void Stop()
    {
		m_rsvr.StopListening();
    }

    volatile int received;

    void MessageReceived(const XByteArray& msg)
    {
		XMutexLocker locker(this->m_mutex);

		cout << "Message [" << setw(8) << ++received << "] Size: " << msg.Size() << endl;
    }

private:
    XTcpMessageReceiver m_rsvr;
	XMutex m_mutex;
};

void TcpMessageTester()
{		
	int port = 57384;
	int numMessages = 10000;

	int option;
	cout << "Enter option [1]Sender [2]Receiver: ";
	cin >> option;

	ifstream samplefile;
	samplefile.open("C:\\ProgramData\\Xylasoft\\XylasoftXLog.log", ios::in | ios::binary);
	if (!samplefile.is_open())
	{
		cout << "Open failed." << endl;
		return;
	}
	char* buffer = new char[10240000];
	int size = 0;
	char c = samplefile.get();
	while (!samplefile.eof())
	{
		buffer[size] = c;
		size++;
		
		c = samplefile.get();
	}
	samplefile.close();

	cout << "File size is " << size << " bytes." << endl;

	switch (option)
	{
	case 1:
		{
			cout << "Sending " << numMessages << " messages..." << endl;
			int num = 0;
			while (num < numMessages)
			{
				XTcpMessageSender sender(XSocketDevice::GetDefaultAddressForLocalHost().c_str(), port);

				/*std::ostringstream stream;
				stream << (num + 1) << "[Hello There]" << (num + 1);

				std::string msgstr = stream.str();

				RawMessage msg;
				msg.Data = msgstr.c_str();
				msg.Size = msgstr.size();*/

				XByteArray msg(buffer, size);

				sender.Send(msg);
				num++;
			}
			break;
		}
	case 2:
		{
			TcpMessageTest t(port);
			t.Start();
			cout << "Listening..." << endl;
			::Sleep(60000000);
			t.Stop();
			break;
		}
	default:
		cout << "Option not recognized." << endl;
		break;
	}
}