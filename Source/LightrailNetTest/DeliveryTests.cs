using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Xylasoft;

namespace LightrailNetTest
{
    /// <summary>
    /// Summary description for UnitTest1
    /// </summary>
    [TestClass]
    public class DeliveryTests
    {
        public DeliveryTests()
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
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext) { }
        //
        // Use ClassCleanup to run code after all tests in a class have run
        //[ClassCleanup()]
        //public static void MyClassCleanup() { }
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
        public void Object01()
        {
            Delivery d = new Delivery("Hello");

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf8, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[0], d.GetLabelNameList()));
        }

        [TestMethod]
        public void Object02()
        {
            Delivery d = new Delivery("Hello");
            d.SetLabel("name1", "value1");

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf8, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[] { "name1" }, d.GetLabelNameList()));
            Assert.AreEqual("value1", d.Label("name1"));
        }

        [TestMethod]
        public void Object03()
        {
            Delivery d = new Delivery("Hello");
            d.SetLabel("name1", "value1");
            d.SetLabel("name2", "value2");

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf8, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[] { "name1", "name2" }, d.GetLabelNameList()));
            Assert.AreEqual("value1", d.Label("name1"));
            Assert.AreEqual("value2", d.Label("name2"));
        }

        [TestMethod]
        public void Object04()
        {
            Delivery d = new Delivery("Hello");
            d.SetLabel("name1", "value1");
            d.SetLabel("name2", "value2");
            d.SetLabel("name1", "value3");

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf8, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[] { "name1", "name2" }, d.GetLabelNameList()));
            Assert.AreEqual("value3", d.Label("name1"));
            Assert.AreEqual("value2", d.Label("name2"));
        }

        [TestMethod]
        public void Object05()
        {
            Delivery d = new Delivery("Hello");
            d.SetLabel("name1", "value1");
            d.SetLabel("name2", "value2");
            d.SetLabel("name1", null);

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf8, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[] { "name2" }, d.GetLabelNameList()));
            Assert.AreEqual("value2", d.Label("name2"));
        }

        [TestMethod]
        public void Object06()
        {
            string text = "Hello";
            byte[] data = Encoding.ASCII.GetBytes(text);
            Delivery d = new Delivery(data, DeliveryEncoding.Ascii);

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Ascii, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[0], d.GetLabelNameList()));
        }

        [TestMethod]
        public void Object07()
        {
            string text = "Hello";
            byte[] data = Encoding.UTF8.GetBytes(text);
            Delivery d = new Delivery(data, DeliveryEncoding.Utf8);

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf8, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[0], d.GetLabelNameList()));
        }

        [TestMethod]
        public void Object08()
        {
            string text = "Hello";
            byte[] data = Encoding.BigEndianUnicode.GetBytes(text);
            Delivery d = new Delivery(data, DeliveryEncoding.Utf16);

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf16, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[0], d.GetLabelNameList()));
        }

        [TestMethod]
        public void Object09()
        {
            string text = "Hello";
            byte[] data = Encoding.UTF32.GetBytes(text);
            Delivery d = new Delivery(data, DeliveryEncoding.Utf32);

            Assert.AreEqual("Hello", d.MessageString());
            Assert.AreEqual(DeliveryEncoding.Utf32, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[0], d.GetLabelNameList()));
        }

        [TestMethod]
        public void Object10()
        {
            byte[] data = TestHelper.ReadTestFile();
            Delivery d = new Delivery(data, DeliveryEncoding.None);

            Assert.IsTrue(TestHelper.CompareByteArray(data, d.Message()));
            Assert.AreEqual(DeliveryEncoding.None, d.Encoding());
            Assert.IsTrue(TestHelper.CompareStringArray(new string[0], d.GetLabelNameList()));
        }
    }
}
