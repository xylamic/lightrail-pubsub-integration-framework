using System;
using System.Collections.Generic;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Xylasoft;

namespace LightrailNetTest
{
    /// <summary>
    /// Summary description for UnitTest1
    /// </summary>
    [TestClass]
    public class TerminalTests
    {
        private static LightrailClient m_client1;
        private static readonly EventWaitHandle m_wait = new EventWaitHandle(false, EventResetMode.ManualReset);

        private DeliveryEncoding m_encoding = DeliveryEncoding.None;
        private string[] m_labelNameList = new string[0];
        private readonly Dictionary<string, string> m_labels = new Dictionary<string,string>();
        private byte[] m_message = new byte[0];
        private string[] m_receiptTerms = new string[0];
        private DateTime m_sendTimeStart = DateTime.MinValue;
        private string m_sourceApp;
        private string m_sourceTerm;
        private volatile int m_numExpected;
        private volatile int m_numReceived;

        public TerminalTests()
        {
        }

        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        //
        // You can use the following additional attributes as you write your tests:
        //
        // Use ClassInitialize to run code before running the first test in the class
        [ClassInitialize()]
        public static void MyClassInitialize(TestContext testContext)
        {
            m_client1 = new LightrailClient("Client1", "Desc");
            m_client1.StartDeliveries();
        }
        //
        // Use ClassCleanup to run code after all tests in a class have run
        [ClassCleanup()]
        public static void MyClassCleanup()
        {
            m_client1.StopDeliveries();
            m_client1 = null;
        }
        //
        // Use TestInitialize to run code before running each test 
        // [TestInitialize()]
        // public void MyTestInitialize() { }
        //
        // Use TestCleanup to run code after each test has run
        // [TestCleanup()]
        // public void MyTestCleanup() { }
        //
        #endregion

        [TestMethod]
        public void BasicTerminalTest()
        {
            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", "Desc", "[Test>Basic]");
            m_client1.RegisterTerminal(TerminalStandpoint.Subscriber, "Sub", "Desc", "[Test>");
            Thread.Sleep(1000);

            PublishReceived evnt = new PublishReceived(m_client1_OnPublishReceived);
            m_client1.OnPublishReceived += evnt;

            m_wait.Reset();
            m_numExpected = 1;

            Delivery d = new Delivery("Test");
            SetDeliveryAttributes(d, new string[] { "Sub" }, "Pub");

            m_client1.Publish("Pub", d);

            Assert.IsTrue(m_wait.WaitOne(5000));

            m_client1.OnPublishReceived -= evnt;
            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub");
            m_client1.DeregisterTerminal(TerminalStandpoint.Subscriber, "Sub");
        }

        [TestMethod]
        public void BasicTerminalFileTest()
        {
            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", "Desc", "[Test>Basic]");
            m_client1.RegisterTerminal(TerminalStandpoint.Subscriber, "Sub", "Desc", "[Test>");
            Thread.Sleep(1000);

            PublishReceived evnt = new PublishReceived(m_client1_OnPublishReceived);
            m_client1.OnPublishReceived += evnt;

            m_wait.Reset();
            m_numExpected = 1;

            Delivery d = new Delivery(TestHelper.ReadTestFile(), DeliveryEncoding.None);
            SetDeliveryAttributes(d, new string[] { "Sub" }, "Pub");

            m_client1.Publish("Pub", d);

            Assert.IsTrue(m_wait.WaitOne(5000));

            m_client1.OnPublishReceived -= evnt;
            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub");
            m_client1.DeregisterTerminal(TerminalStandpoint.Subscriber, "Sub");
        }

        [TestMethod]
        public void BasicTerminalFileTest100()
        {
            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", "Desc", "[Test>Basic]");
            m_client1.RegisterTerminal(TerminalStandpoint.Subscriber, "Sub", "Desc", "[Test>");
            Thread.Sleep(1000);

            PublishReceived evnt = new PublishReceived(m_client1_OnPublishReceived);
            m_client1.OnPublishReceived += evnt;

            m_wait.Reset();
            m_numExpected = 100;

            Delivery d = new Delivery(TestHelper.ReadTestFile(), DeliveryEncoding.None);
            SetDeliveryAttributes(d, new string[] { "Sub" }, "Pub");

            for (int i = 0; i < 100; i++)
            {
                m_client1.Publish("Pub", d);
            }

            Assert.IsTrue(m_wait.WaitOne(20000));

            m_client1.OnPublishReceived -= evnt;
            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub");
            m_client1.DeregisterTerminal(TerminalStandpoint.Subscriber, "Sub");
        }

        [TestMethod]
        public void BasicTerminalTestLabels()
        {
            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", "Desc", "[Test>Basic]");
            m_client1.RegisterTerminal(TerminalStandpoint.Subscriber, "Sub", "Desc", "[Test>");
            Thread.Sleep(1000);

            PublishReceived evnt = new PublishReceived(m_client1_OnPublishReceived);
            m_client1.OnPublishReceived += evnt;

            m_wait.Reset();
            m_numExpected = 1;

            Delivery d = new Delivery("Test");
            d.SetLabel("l2", "n2");
            d.SetLabel("l1", "n1");
            SetDeliveryAttributes(d, new string[] { "Sub" }, "Pub");

            m_client1.Publish("Pub", d);

            Assert.IsTrue(m_wait.WaitOne(5000));

            m_client1.OnPublishReceived -= evnt;
            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub");
            m_client1.DeregisterTerminal(TerminalStandpoint.Subscriber, "Sub");
        }

        [TestMethod]
        public void BasicTerminalTestLabels100()
        {
            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", "Desc", "[Test>Basic]");
            m_client1.RegisterTerminal(TerminalStandpoint.Subscriber, "Sub", "Desc", "[Test>");
            Thread.Sleep(1000);

            PublishReceived evnt = new PublishReceived(m_client1_OnPublishReceived);
            m_client1.OnPublishReceived += evnt;

            m_wait.Reset();
            m_numExpected = 100;

            Delivery d = new Delivery("Test");
            d.SetLabel("l2", "n2");
            SetDeliveryAttributes(d, new string[] { "Sub" }, "Pub");

            for (int i = 0; i < 100; i++)
            {
                m_client1.Publish("Pub", d);
            }

            Assert.IsTrue(m_wait.WaitOne(5000));

            m_client1.OnPublishReceived -= evnt;
            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub");
            m_client1.DeregisterTerminal(TerminalStandpoint.Subscriber, "Sub");
        }

        [TestMethod]
        public void BasicTerminalTest100()
        {
            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub1", "Desc", "[Test>Basic]");
            m_client1.RegisterTerminal(TerminalStandpoint.Subscriber, "Sub1", "Desc", "[Test>");
            Thread.Sleep(1000);

            PublishReceived evnt = new PublishReceived(m_client1_OnPublishReceived);
            m_client1.OnPublishReceived += evnt;

            m_wait.Reset();
            m_numExpected = 100;

            Delivery d = new Delivery("Test");
            SetDeliveryAttributes(d, new string[] { "Sub1" }, "Pub1");

            for (int i = 0; i < 100; i++)
            {
                m_client1.Publish("Pub1", d);
            }

            Assert.IsTrue(m_wait.WaitOne(10000));

            m_client1.OnPublishReceived -= evnt;
            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub1");
            m_client1.DeregisterTerminal(TerminalStandpoint.Subscriber, "Sub1");
        }

        [TestMethod()]
        public void TerminalBad()
        {
            try { m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "", "", "[a]"); Assert.IsTrue(false); }
            catch (Exception) {}

            try { m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", "", "[a"); Assert.IsTrue(false); }
            catch (Exception) { }

            m_client1.RegisterTerminal(TerminalStandpoint.Publisher, "Pub", null, "[a]");

            try { m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub2"); Assert.IsTrue(false); }
            catch (Exception) { }

            m_client1.DeregisterTerminal(TerminalStandpoint.Publisher, "Pub");
        }

        private void m_client1_OnPublishReceived(DetailedDelivery delivery)
        {
            Assert.AreEqual(m_encoding, delivery.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArrayUnstable(m_labelNameList, delivery.GetLabelNameList()));
            Assert.IsTrue(TestHelper.CompareByteArray(m_message, delivery.Message()));
            Assert.IsTrue(TestHelper.CompareStringArray(m_receiptTerms, delivery.ReceiptTerminalNames()));
            Assert.IsTrue(m_sendTimeStart <= delivery.SendTimeUtc().AddSeconds(1));
            Assert.IsTrue(m_sendTimeStart.AddMinutes(5) >= delivery.SendTimeUtc().AddSeconds(1));
            Assert.AreEqual(m_sourceApp, delivery.SourceApplication());
            Assert.AreEqual(Environment.MachineName, delivery.SourceHost());
            Assert.AreEqual(m_sourceTerm, delivery.SourceTerminalName());

            foreach (string label in m_labelNameList)
            {
                Assert.AreEqual(m_labels[label], delivery.Label(label));
            }

            m_numReceived++;

            if (m_numReceived == m_numExpected)
            {
                m_wait.Set();
            }
        }

        private void SetDeliveryAttributes(Delivery delivery, string[] receiptTerms, string sourceTerm)
        {
            m_encoding = delivery.Encoding();
            m_message = delivery.Message();
            m_labelNameList = delivery.GetLabelNameList();
            foreach (string label in m_labelNameList)
            {
                m_labels.Add(label, delivery.Label(label));
            }
            m_receiptTerms = receiptTerms;
            m_sendTimeStart = DateTime.UtcNow;
            m_sourceApp = "Client1";
            m_sourceTerm = sourceTerm;
        }
    }
}
