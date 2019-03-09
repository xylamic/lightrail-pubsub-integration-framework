#ifndef LIGHTRAILCONSOLE_H
#define LIGHTRAILCONSOLE_H

#include "Lightrail.h"
#include "ILightrail.h"
#include <iostream>

using namespace std;
using namespace Xylasoft;

class LightrailConsole
{
public:
	LightrailConsole(const wchar_t* name, const wchar_t* description, const wchar_t* instance = nullptr)
	{
		m_lightrail = Lightrail::GetLightrailInstance(name, description, instance);

		m_lightrail->OnDeliveryParseError.SetCallback(new XEventCallback<LightrailConsole, const IDeliveryParseError&>(
			this, &LightrailConsole::ParseError));
		m_lightrail->OnPublicationReceived.SetCallback(new XEventCallback<LightrailConsole, ITerminalDelivery&>(
			this, &LightrailConsole::PublicationReceived));
		m_lightrail->OnRequestReceived.SetCallback(new XEventCallback<LightrailConsole, ITerminalReturnDelivery&>(
			this, &LightrailConsole::RequestReceived));
	}

	~LightrailConsole()
	{
		delete m_lightrail;
	}

	void RegisterSecurityKey()
	{
		try
		{
			std::wstring name;
			std::string key;
			cout << "Enter name: ";
			::getline(wcin, name);
			cout << "Enter key: ";
			::getline(cin, key);

			const ISecurityKey* seckey = Lightrail::NewAES256SecurityKey(name.c_str(), key.c_str(), static_cast<long>(key.size()));

			m_lightrail->RegisterSecurityKey(seckey);

			cout << "Registered." << endl;
		}
		catch (std::exception& ex)
		{
			cout << "Registration failed: " << ex.what() << endl;
		}
	}

	void DeregisterSecurityKey()
	{
		try
		{
			std::wstring name;
			cout << "Enter name: ";
			::getline(wcin, name);

			m_lightrail->DeregisterSecurityKey(name.c_str());

			cout << "Registered." << endl;
		}
		catch (std::exception& ex)
		{
			cout << "Registration failed: " << ex.what() << endl;
		}
	}

	void DeregisterTerminal()
	{
		try
		{
			std::string typestr;
			int type;
			cout << "Enter type [1]Publication [2]Subscription [3]Response [4]Request : " << endl;
			::getline(cin, typestr);
			type = ::atoi(typestr.c_str());

			if (type < 1 || type > 4)
			{
				cout << "Invalid type." << endl;
				return;
			}

			std::wstring name;
			cout << "Enter name: ";
			::getline(wcin, name);

			m_lightrail->DeregisterTerminal(static_cast<Standpoint::StandpointType>(type), name.c_str());

			cout << "Deregistered." << endl;
		}
		catch (std::exception& ex)
		{
			cout << "Registration failed: " << ex.what() << endl;
		}
	}

	void RegisterTerminal()
	{
		try
		{
			std::string typestr;
			int type;
			cout << "Enter type [1]Publication [2]Subscription [3]Response [4]Request : " << endl;
			::getline(cin, typestr);
			type = ::atoi(typestr.c_str());

			if (type < 1 || type > 4)
			{
				cout << "Invalid type." << endl;
				return;
			}

			std::wstring name, desc, terminalstring, keyname;
			cout << "Enter name: ";
			::getline(wcin, name);
			cout << "Enter description: ";
			::getline(wcin, desc);
			cout << "Enter terminal: ";
			::getline(wcin, terminalstring);
			cout << "Security key name: ";
			::getline(wcin, keyname);

			m_lightrail->RegisterTerminal(static_cast<Standpoint::StandpointType>(type), 
				Lightrail::NewTerminal(name.c_str(), desc.c_str(), terminalstring.c_str(), keyname.size() > 0 ? keyname.c_str() : nullptr));

			cout << "Registered." << endl;
		}
		catch (std::exception& ex)
		{
			cout << "Registration failed: " << ex.what() << endl;
		}
	}

	void Request()
	{
		try
		{
			std::wstring name, message;
			std::string messagenumstr;
			int messagenum;

			cout << "Enter terminal name: ";
			::getline(wcin, name);

			cout << "Enter message: ";
			::getline(wcin, message);

			cout << "Enter number of requests to send: ";
			::getline(cin, messagenumstr);
			messagenum = ::atoi(messagenumstr.c_str());

			for (int i = 0; i < messagenum; i++)
			{
				cout << "Sending request " << (i + 1) << "..." << endl;

				IDelivery* delivery = Lightrail::NewDelivery();
				delivery->SetMessageString(message);

				const IDetailedDelivery* response = m_lightrail->Request(name.c_str(), delivery);

				wcout << L"Response: " << response->MessageString() << endl;

				delete response;
			}
		}
		catch (std::exception& ex)
		{
			cout << "Request failed: " << ex.what() << endl;
		}
	}

	void Publish()
	{
		try
		{
			std::wstring name, message;
			std::string messagestr;
			int messages;

			cout << "Enter terminal name: ";
			::getline(wcin, name);

			cout << "Enter message: ";
			::getline(wcin, message);

			cout << "Enter number of messages to send: ";
			::getline(cin, messagestr);
			messages = ::atoi(messagestr.c_str());

			for (int i = 0; i < messages; i++)
			{
				cout << "Sending message " << (i + 1) << "..." << endl;

				IDelivery* delivery = Lightrail::NewDelivery();
				delivery->SetMessageString(message);
				m_lightrail->Publish(name.c_str(), delivery);
			}
		}
		catch (std::exception& ex)
		{
			cout << "Publish failed: " << ex.what() << endl;
		}
	}

	void Run()
	{
		m_lightrail->StartDeliveries();

		while (true)
		{
			int action;
			std::string actionstr;
			cout << "Enter action [1]RegTerminal [2]DeregTerminal [3]RegKey [4]DeregKey [5]Publish [6]Request [7]Exit: " << endl;
			::getline(cin, actionstr);
			action = ::atoi(actionstr.c_str());

			switch (action)
			{
			case 1:
				this->RegisterTerminal();
				break;
			case 2:
				this->DeregisterTerminal();
				break;
			case 3:
				this->RegisterSecurityKey();
				break;
			case 4:
				this->DeregisterSecurityKey();
				break;
			case 5:
				this->Publish();
				break;
			case 6:
				this->Request();
				break;
			case 7:
				m_lightrail->StopDeliveries();
				return;
			default:
				cout << "Unrecognized option." << endl;
			}
		}

		m_lightrail->StopDeliveries();
	}

protected:
	void ParseError(const IDeliveryParseError& error)
	{
		cout << "Parse error occurred with error: " << error.Message().what() << endl;
	}

	void PublicationReceived(ITerminalDelivery& delivery)
	{
		wstring msg = delivery.DetailedDelivery()->MessageString();
		wcout << L"Message Received: " << msg << endl;
	}

	void RequestReceived(ITerminalReturnDelivery& delivery)
	{
		wstring msg = delivery.DetailedDelivery()->MessageString();
		wcout << L"Message Received [";
		cout << delivery.DetailedDelivery()->SourceHost();
		wcout << L":" << delivery.DetailedDelivery()->SourcePort() << L"] : " << msg << endl;

		std::wstring response = L"The response.";
		//wcout << L"Enter response: ";
		//::getline(wcin, response);

		delivery.ReturnedDelivery()->SetMessageString(response.c_str());
	}
private:
	ILightrail* m_lightrail;
};

#endif