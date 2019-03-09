using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Xylasoft;

using System.IO;
using System.Runtime.Serialization;

namespace CSExample
{
    /// <summary>
    /// My important data.
    /// </summary>
    [DataContract()]
    class MyContract
    {
        /// <summary>
        /// Data Item String One
        /// </summary>
        [DataMember()]
        public string DataItem1 { get; set; }

        /// <summary>
        /// Data Item String Two
        /// </summary>
        [DataMember()]
        public double DataItem2 { get; set; }

        /// <summary>
        /// Data Item String Three
        /// </summary>
        [DataMember()]
        public int DataItem3 { get; set; }
    }

    class Program
    {
        #region Data Contract Usage
        static Delivery SendDataContact()
        {
            // create a serializer, it is useful to make this a static object in your class because
            // you will use it over and over and over
            DataContractSerializer serializer = new DataContractSerializer(typeof(MyContract));

            // create an object of your contact
            MyContract data = new MyContract();
            data.DataItem1 = "Item1";
            data.DataItem2 = 2.0;
            data.DataItem3 = 3;

            // create a memory stream to load the XML object into
            MemoryStream stream = new MemoryStream();

            // serilize the object to XML
            serializer.WriteObject(stream, data);

            // put the message into a delivery for sending
            Delivery msg = new Delivery(stream.ToArray(), DeliveryEncoding.Utf8);

            // send the message

            return msg;
        }

        static void ReceiveDataContract(Delivery msg)
        {
            // create a serializer, it is useful to make this a static object in your class because
            // you will use it over and over and over
            DataContractSerializer serializer = new DataContractSerializer(typeof(MyContract));

            // get the binary structure in the delivery
            byte[] msgData = msg.Message();

            // put the structure in a stream
            MemoryStream stream = new MemoryStream(msgData);
            stream.Position = 0;

            // deserialize the message
            MyContract data = (MyContract)serializer.ReadObject(stream);

            // access the object same as it was sent
            string val1 = data.DataItem1;
            double val2 = data.DataItem2;
            int val3 = data.DataItem3;
        }

        static void DataContracts()
        {
            Delivery msg = SendDataContact();
            ReceiveDataContract(msg);
        }
        #endregion

        static void Main(string[] args)
        {
            DataContracts();
        }

        /* Publisher */
        static void aMain(string[] args)
        {
            // create the cient
            LightrailClient client = new LightrailClient("name", "description");

            // start the client
            client.StartDeliveries();

            // register the publisher terminal
            client.RegisterTerminal(TerminalStandpoint.Publisher, "PubTerminal", "PubDesc", 
                "[Global>Company>Region>Domain>App>Instance>Publish]");

            // create a message
            Delivery message = new Delivery("This is a string message that will be encoded as UTF-8");

            // send the message
            client.Publish("PubTerminal", message);

            // send more and more and more messages

            // close the instance
            client.StopDeliveries();
        }

        /* Subscriber */
        static void bMain(string[] args)
        {
            // create the cient
            LightrailClient client = new LightrailClient("name", "description");

            // set up callback
            client.OnPublishReceived += new PublishReceived(client_OnPublishReceived);

            // start the client
            client.StartDeliveries();

            // register the subscriber terminal
            client.RegisterTerminal(TerminalStandpoint.Subscriber, "SubTerminal", "SubDesc",
                "[Global>Company>Region>Domain>App>Instance>Publish]");

            // wait for any and all messages to be delivered

            // close the instance
            client.StopDeliveries();
        }

        static void client_OnPublishReceived(DetailedDelivery delivery)
        {
            // get the message as a string
            string message = delivery.MessageString();
        }

        /* Requester */
        static void cMain(string[] args)
        {
            // create the cient
            LightrailClient client = new LightrailClient("name", "description");

            // start the client
            client.StartDeliveries();

            // register the requester terminal
            client.RegisterTerminal(TerminalStandpoint.Requester, "ReqTerminal", "ReqDesc",
                 "[Global>Company>Region>Domain>App>Instance>Request]");

            // create a request message
            Delivery request = new Delivery("This is a string message that will be encoded as UTF-8");

            // send the request and get the response with a 30 second timeout
            DetailedDelivery response = client.Request("ReqTerminal", request, 30000);

            // parse the response
            string responseString = response.MessageString();

            // send more and more and more requests

            // close the instance
            client.StopDeliveries();
        }

        /* Responder */
        static void dMain(string[] args)
        {
            // create the cient
            LightrailClient client = new LightrailClient("name", "description");

            // set up callback
            client.OnRequestReceived += new RequestReceived(client_OnRequestReceived);

            // start the client
            client.StartDeliveries();

            // register the responder terminal
            client.RegisterTerminal(TerminalStandpoint.Responder, "RespTerminal", "RespDesc",
                 "[Global>Company>Region>Domain>App>Instance>Request]");

            // wait for any and all messages to be requests to be delivered

            // close the instance
            client.StopDeliveries();
        }

        static Delivery client_OnRequestReceived(DetailedDelivery delivery)
        {
            // get the request message as a string
            string message = delivery.MessageString();

            Delivery response = new Delivery("The response message.");
            return response;
        }
    }
}
