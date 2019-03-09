// include Lightrail[D].lib

#include <ILightrail.h>
#include <Lightrail.h>

using namespace Xylasoft;

class LightrailExample
{
public:
	void Publisher()
	{
		// create the client
		ILightrail* client = Lightrail::GetLightrailInstance(L"Name", L"Description");

		// start the client
		client->StartDeliveries();

		// register the publisher terminal
		const ITerminal* terminal = Lightrail::NewTerminal(L"PubTerminal", L"PubDesc",
			L"[Global>Company>Region>Domain>App>Instance>Publish]");
		client->RegisterTerminal(Standpoint::PUBLISHER, terminal);

		// create a message
		IDelivery* message = Lightrail::NewDelivery();
		message->SetMessageString(L"This is a string message that will be encoded as UTF-8");

		// send the message
		client->Publish(L"PubTerminal", message);

		// send more and more and more messages

		// close the instance
		client->StopDeliveries();

		// remove the instance
		delete client;
	}

	void Subscriber()
	{
		// create the client
		ILightrail* client = Lightrail::GetLightrailInstance(L"Name", L"Description");

			// set up callback
			client->OnPublicationReceived.SetCallback(new XEventCallback<LightrailExample, ITerminalDelivery&>(this,
				&LightrailExample::PublicationReceived));

		// start the client
		client->StartDeliveries();

		// register the subscriber terminal
		const ITerminal* terminal = Lightrail::NewTerminal(L"SubTerminal", L"SubDesc",
			L"[Global>Company>Region>Domain>App>Instance>Publish]");
		client->RegisterTerminal(Standpoint::SUBSCRIBER, terminal);

		// wait for any and all messages to be delivered

		// close the instance
		client->StopDeliveries();

		// remove the instance
		delete client;
	}

	void Requester()
	{
		// create the client
		ILightrail* client = Lightrail::GetLightrailInstance(L"Name", L"Description");

		// start the client
		client->StartDeliveries();

		// register the requester terminal
		const ITerminal* terminal = Lightrail::NewTerminal(L"ReqTerminal", L"ReqDesc",
			L"[Global>Company>Region>Domain>App>Instance>Request]");
		client->RegisterTerminal(Standpoint::REQUESTER, terminal);

		// create a message
		IDelivery* message = Lightrail::NewDelivery();
		message->SetMessageString(L"This is a string message that will be encoded as UTF-8");

		// send the request and get the response
		const IDetailedDelivery* response = client->Request(L"ReqTerminal", message);
		std::wstring responsestr = response->MessageString();

		// send more and more and more request messages

		// close the instance
		client->StopDeliveries();

		// remove the instance
		delete client;
	}

	void Responder()
	{
		// create the client
		ILightrail* client = Lightrail::GetLightrailInstance(L"Name", L"Description");

		// set up callback
		client->OnRequestReceived.SetCallback(new XEventCallback<LightrailExample, ITerminalReturnDelivery&>(
			this, &LightrailExample::RequestReceived));

		// start the client
		client->StartDeliveries();

		// register the responder terminal
		const ITerminal* terminal = Lightrail::NewTerminal(L"RespTerminal", L"RespDesc",
			L"[Global>Company>Region>Domain>App>Instance>Request]");
		client->RegisterTerminal(Standpoint::RESPONDER, terminal);

		// wait for any and all request messages to be delivered

		// close the instance
		client->StopDeliveries();

		// remove the instance
		delete client;
	}

	void LightrailInstance()
	{
		// create the Lightrail instance
		ILightrail* client = Lightrail::GetLightrailInstance(L"Name", L"Description");

		// setup any callbacks for subscription/response
		client->OnPublicationReceived.SetCallback(new XEventCallback<LightrailExample, ITerminalDelivery&>(this,
			&LightrailExample::PublicationReceived));
		client->OnRequestReceived.SetCallback(new XEventCallback<LightrailExample, ITerminalReturnDelivery&>(this,
			&LightrailExample::RequestReceived));
		client->OnDeliveryParseError.SetCallback(new XEventCallback<LightrailExample, const IDeliveryParseError&>(this,
			&LightrailExample::DeliveryParseError));

		// start the Lightrail so it can receive messages and send messages
		client->StartDeliveries();

		// register a key using an ASCII string
		const char* key = "ThisIsMyKey!";
		const ISecurityKey* seckey = Lightrail::NewAES256SecurityKey(L"SecKey1", key, strlen(key));
		client->RegisterSecurityKey(seckey);

		const ITerminal* terminal = Lightrail::NewTerminal(L"PubTerminal", L"PubDesc",
			L"[Global>Company>Region>Domain>App>Instance>Publish]", L"SecKey1");
		client->RegisterTerminal(Standpoint::PUBLISHER, terminal);

		const ITerminal* terminal2 = Lightrail::NewTerminal(L"ReqTerminal", L"ReqDesc",
			L"[Global>Company>Region>Domain>App>Instance>Request]");
		client->RegisterTerminal(Standpoint::REQUESTER, terminal2);

		// send and receive deliveries
		IDelivery* message = Lightrail::NewDelivery();
		message->SetMessageString(L"This is a string message that will be encoded as UTF-8");

		client->Publish(L"PubTerminal", message);

		const IDetailedDelivery* response2 = client->Request(L"ReqTerminal", message, 30000);

		// when you are closing your application, stop the client
		client->StopDeliveries();

		// when all work is completed, remove the instance
		delete client;
	}

protected:
	void RequestReceived(ITerminalReturnDelivery& delivery)
	{
		std::wstring request_msg = delivery.DetailedDelivery()->MessageString();
		
		delivery.ReturnedDelivery()->SetMessageString(L"The is a response text.");
	}

	void PublicationReceived(ITerminalDelivery& delivery)
	{
		std::wstring msg = delivery.DetailedDelivery()->MessageString();
	}

	void DeliveryParseError(const IDeliveryParseError& error)
	{
		const std::exception& stdex = error.Message();
	}
};

void main()
{
	ILightrail* client = Lightrail::GetLightrailInstance(L"name", L"description");
	client->StartDeliveries();

	const ITerminal* terminal = Lightrail::NewTerminal(L"PubTerminal", L"PubDesc",
		L"[Global>Company>Region>Domain>App>Instance>Publish]");
	client->RegisterTerminal(Standpoint::PUBLISHER, terminal);

	IDelivery* message = Lightrail::NewDelivery();
	message->SetMessageString(L"This is a string message that will be encoded as UTF-8.");

	client->Publish(L"PubTerminal", message);

	client->StopDeliveries();
	delete client;
}