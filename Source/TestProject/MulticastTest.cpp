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

class MulticastTest
{
public:
	MulticastTest()
		: m_m1("239.3.29.3", 38502),
		  m_m2("239.3.29.3", 38502)
	{
		m_m1.OnMessageReceived.SetCallback(new XEventCallback<MulticastTest, const XByteArray&>(this, &MulticastTest::MessageReceived1));
		m_m2.OnMessageReceived.SetCallback(new XEventCallback<MulticastTest, const XByteArray&>(this, &MulticastTest::MessageReceived2));
	}

protected:
	void MessageReceived1(const XByteArray& message)
	{
		if (!XUtilities::CompareByteArrays(this->m_originalMsg.ConstData(), this->m_originalMsg.Size(),
			message.ConstData(), message.Size()))
		{
			cout << "Messages didn't match on Rsvr1." << endl;
		}

		this->m_numMsg1++;
		
		if (this->m_numTotal == this->m_numMsg1)
		{
			this->m_eventWait1.Signal();
		}
	}

	void MessageReceived2(const XByteArray& message)
	{
		if (!XUtilities::CompareByteArrays(this->m_originalMsg.ConstData(), this->m_originalMsg.Size(),
			message.ConstData(), message.Size()))
		{
			cout << "Messages didn't match on Rsvr2." << endl;
		}

		this->m_numMsg2++;

		if (this->m_numTotal == this->m_numMsg2)
		{
			this->m_eventWait2.Signal();
		}
	}

public:
	void Test()
	{
		m_m1.StartListening();
		m_m2.StartListening();

		cout << "Starting multicast tests..." << endl;

		::Sleep(2000);

		this->SimpleTest();
		this->SimpleMultiTest();
		this->LargeMultiTest();
		this->FullLargeMultiTest();
		this->FullLargeMsgMultiTest();

		cout << "Completed multicast tests." << endl;
	}

protected:
	void SimpleTest()
	{
		this->m_numMsg1 = 0;
		this->m_numMsg2 = 0;
		this->m_numTotal = 1;

		this->m_eventWait1.Reset();
		this->m_eventWait2.Reset();

		this->m_originalMsg = XByteArray("Hello there", static_cast<long>(strlen("Hello there")));
		m_m1.Send(this->m_originalMsg);

		if (m_eventWait1.Wait(3000) && m_eventWait2.Wait(3000))
		{
			cout << "All messages received for simple test." << endl;
		}
	}

	void SimpleMultiTest()
	{
		this->m_numMsg1 = 0;
		this->m_numMsg2 = 0;
		this->m_numTotal = 10;

		this->m_eventWait1.Reset();
		this->m_eventWait2.Reset();

		int count = 0;

		this->m_originalMsg = XByteArray("Hello there", static_cast<long>(strlen("Hello there")));

		while (count < this->m_numTotal)
		{
			m_m1.Send(this->m_originalMsg);
			count++;
		}

		if (m_eventWait1.Wait(3000) && m_eventWait2.Wait(3000))
		{
			cout << "All messages received for simple multi test." << endl;
		}
	}

	void LargeMultiTest()
	{
		this->m_numMsg1 = 0;
		this->m_numMsg2 = 0;
		this->m_numTotal = 100;

		this->m_eventWait1.Reset();
		this->m_eventWait2.Reset();

		int count = 0;

		this->m_originalMsg = XByteArray("Hello there", static_cast<long>(strlen("Hello there")));

		while (count < this->m_numTotal)
		{
			m_m2.Send(this->m_originalMsg);

			count++;
		}

		if (m_eventWait1.Wait(10000) && m_eventWait2.Wait(10000))
		{
			cout << "All messages received for large multi test." << endl;
		}
	}

	void FullLargeMultiTest()
	{
		this->m_numMsg1 = 0;
		this->m_numMsg2 = 0;
		this->m_numTotal = 100;

		this->m_eventWait1.Reset();
		this->m_eventWait2.Reset();

		int count = 0;

		this->m_originalMsg = XByteArray("Hello there", static_cast<long>(strlen("Hello there")));

		while (count < (this->m_numTotal / 2))
		{
			m_m1.Send(this->m_originalMsg);
			m_m2.Send(this->m_originalMsg);

			count++;
		}

		if (m_eventWait1.Wait(10000) && m_eventWait2.Wait(10000))
		{
			cout << "All messages received for full, large multi test." << endl;
		}
	}

	void FullLargeMsgMultiTest()
	{
		this->m_numMsg1 = 0;
		this->m_numMsg2 = 0;
		this->m_numTotal = 100;

		this->m_eventWait1.Reset();
		this->m_eventWait2.Reset();

		int count = 0;

		this->m_originalMsg = XByteArray(std::string(200, 'x').c_str(), 200);

		while (count < (this->m_numTotal / 2))
		{
			m_m1.Send(this->m_originalMsg);
			m_m2.Send(this->m_originalMsg);

			count++;
		}

		if (m_eventWait1.Wait(10000) && m_eventWait2.Wait(10000))
		{
			cout << "All messages received for full, large multi msg test." << endl;
		}
	}

private:
	XMulticaster m_m1;
	XMulticaster m_m2;

	XByteArray m_originalMsg;

	volatile int m_numTotal;
	volatile int m_numMsg1;
	volatile int m_numMsg2;

	XEvent m_eventWait1;
	XEvent m_eventWait2;
};

void MulticastTester()
{
	MulticastTest t;
	t.Test();
}
