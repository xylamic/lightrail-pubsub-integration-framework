using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestPassExample
{
    internal delegate void UpdateText(string text);

    public partial class Form1 : Form
    {
        private Xylasoft.LightrailClient m_client = null;
        private UpdateText m_updateText;
        private EventHandler m_handler;

        public Form1()
        {
            InitializeComponent();

            m_updateText = new UpdateText(UpdateTextData);
            m_handler = new System.EventHandler(this.uxText_TextChanged);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                m_client = new Xylasoft.LightrailClient(uxName.Text, null);

                m_client.OnRequestReceived += new Xylasoft.RequestReceived(m_client_OnRequestReceived);
                m_client.OnPublishReceived += new Xylasoft.PublishReceived(m_client_OnPublishReceived);

                m_client.StartDeliveries();

                string topic = "[Test>Text>";
                if (!String.IsNullOrEmpty(uxName.Text))
                {
                    topic += uxName.Text + "]";
                }

                if (uxRadPub.Checked)
                {
                    m_client.RegisterTerminal(Xylasoft.TerminalStandpoint.Publisher, "pub", null, topic);
                    m_client.RegisterTerminal(Xylasoft.TerminalStandpoint.Responder, "res", null, topic);

                    uxText.Enabled = true;

                    this.uxText.TextChanged += m_handler;
                }
                else
                {
                    m_client.RegisterTerminal(Xylasoft.TerminalStandpoint.Subscriber, "sub", null, topic);
                    m_client.RegisterTerminal(Xylasoft.TerminalStandpoint.Requester, "req", null, topic);

                    System.Threading.Thread.Sleep(2000);

                    try
                    {
                        Xylasoft.DetailedDelivery delivery = m_client.Request("req", new Xylasoft.Delivery("Request"), 5000);
                        uxText.Text = delivery.MessageString().Substring(1);
                    }
                    catch (Exception)
                    {
                    }
                }

                button1.Enabled = false;
                button2.Enabled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to Start: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        void UpdateTextData(string value)
        {
            lock (this)
            {
                uxText.Text = value;
            }
        }

        void m_client_OnPublishReceived(Xylasoft.DetailedDelivery delivery)
        {
            uxText.BeginInvoke(m_updateText, delivery.MessageString().Substring(1));
        }

        Xylasoft.Delivery m_client_OnRequestReceived(Xylasoft.DetailedDelivery delivery)
        {
            return new Xylasoft.Delivery("A" + uxText.Text);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                m_client.StopDeliveries();
                m_client.Dispose();
                m_client = null;

                button1.Enabled = true;
                button2.Enabled = false;

                if (uxRadPub.Checked)
                {
                    this.uxText.TextChanged -= m_handler;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to Stop: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void uxText_TextChanged(object sender, EventArgs e)
        {
            m_client.Publish("pub", new Xylasoft.Delivery("A" + uxText.Text));
        }
    }
}
