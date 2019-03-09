using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Xylasoft.LightrailDiagnostics
{
    internal delegate void LightPubReceived(DetailedDelivery delivery);
    internal delegate void LightReqReceived(DetailedDelivery delivery);
    internal delegate void LightParseErrorReceived(DeliveryParseError error);

    internal delegate void DoRequest(string terminal, Delivery message);

    public partial class LightrailDiagnostics : Form
    {
        private Xylasoft.DiscoveryClient m_discovery = new DiscoveryClient();
        private Timer m_timer = new Timer();
        private Xylasoft.LightrailClient m_client = null;

        private LightPubReceived m_lightPubReceived;
        private LightReqReceived m_lightReqReceived;
        private LightParseErrorReceived m_lightParseReceived;
        private DoRequest m_doRequest;

        private object m_response;

        private byte[] m_loadedResponse = null;

        private List<KeyValuePair<DateTime, long>> m_subReceived = new List<KeyValuePair<DateTime,long>>();
        private long m_subReceivedTotal = 0;

        public LightrailDiagnostics()
        {
            try
            {
                InitializeComponent();

                m_lightPubReceived = new LightPubReceived(LightPublishReceived);
                m_lightReqReceived = new LightReqReceived(LightRequestReceived);
                m_lightParseReceived = new LightParseErrorReceived(LightParseErrorReceived);
                m_doRequest = new DoRequest(SendRequest);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to start: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit();
            }
        }

        private void LightrailDiagnostics_Load(object sender, EventArgs e)
        {
            try
            {
                m_timer_Tick(this, null);

                m_timer.Tick += new EventHandler(m_timer_Tick);
                m_timer.Interval = 5000;
                m_timer.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to start: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit();
            }
        }

        protected void SendRequest(string terminal, Delivery message)
        {
            try
            {
                DetailedDelivery response = m_client.Request(uxReqTermName.Text, message, 30000, 
                    Convert.ToInt32(uxReqNumResponders.Value));
                m_response = response;
            }
            catch (Exception ex)
            {
                m_response = ex;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void m_timer_Tick(object sender, EventArgs e)
        {
            Xylasoft.DiscoveredParticipant[] participants = m_discovery.GetParticipants();

            uxLastDiscoveryUpdate.Text = DateTime.Now.ToString("F");

            uxDiscList.Items.Clear();

            foreach (Xylasoft.DiscoveredParticipant part in participants)
            {
                uxDiscList.Items.Add(new ListViewItem(new string[] { part.Hostname, part.Port.ToString() }));
            }

            uxStatusPart.Text = String.Format("{0} Participants", uxDiscList.Items.Count);
        }

        private void uxStart_Click(object sender, EventArgs e)
        {
            try
            {
                m_client = new LightrailClient(uxName.Text, uxDescription.Text);
                m_client.OnPublishReceived += new PublishReceived(m_client_OnPublishReceived);
                m_client.OnRequestReceived += new RequestReceived(m_client_OnRequestReceived);
                m_client.OnParseErrorReceived += new ParseErrorReceived(m_client_OnParseErrorReceived);
                m_client.StartDeliveries();

                uxStart.Enabled = false;
                uxStop.Enabled = true;
                uxTermRegister.Enabled = true;
                uxPubSend.Enabled = true;

                uxStatusState.Text = "Running";
                uxStatusText.Text = "Lightrail Started.";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to start: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        void m_client_OnParseErrorReceived(DeliveryParseError error)
        {
            this.BeginInvoke(m_lightParseReceived, error);
        }

        Delivery m_client_OnRequestReceived(DetailedDelivery delivery)
        {
            this.BeginInvoke(m_lightReqReceived, delivery);

            if (uxRespFile.Checked)
            {
                lock (this)
                {
                    if (m_loadedResponse == null)
                    {
                        return null;
                    }
                    else
                    {
                        return new Delivery(m_loadedResponse, DeliveryEncoding.None);
                    }
                }
            }
            else
            {
                return new Delivery(uxRespTextTexas.Text);
            }
        }

        void LightParseErrorReceived(DeliveryParseError error)
        {
            string[] item = new string[] {
                error.ErrorType().ToString(),
                error.TerminalName(),
                error.SourceApplication(),
                error.SourceHostname(),
                error.SourcePort().ToString(),
                error.Message()
            };

            uxListErrors.Items.Add(new ListViewItem(item));

            uxStatusText.Text = "Parse Error encountered.";
        }

        void LightPublishReceived(DetailedDelivery delivery)
        {
            long checksum = CheckSum(delivery.Message());

            // get average bytes per second
            m_subReceived.Add(new KeyValuePair<DateTime, long>(DateTime.Now, delivery.Message().Length));
            m_subReceivedTotal += delivery.Message().Length;
            DateTime watchTime = DateTime.Now.AddSeconds(-20);
            while (m_subReceived.Count > 0 && m_subReceived[0].Key < watchTime)
            {
                m_subReceivedTotal -= m_subReceived[0].Value;
                m_subReceived.RemoveAt(0);
            }
            if (m_subReceived.Count > 1)
            {
                TimeSpan span = m_subReceived[m_subReceived.Count - 1].Key - m_subReceived[0].Key;
                uxSubRcvPerSec.Text = (((double)m_subReceivedTotal / span.TotalSeconds) / 1024.0).ToString();
            }
            else
            {
                uxSubRcvPerSec.Text = "Not Enough";
            }
            GC.Collect();
            GC.WaitForPendingFinalizers();

            StringBuilder receiptTerms = new StringBuilder(delivery.ReceiptTerminalNames()[0]);
            for (int i = 1; i < delivery.ReceiptTerminalNames().Length; i++)
            {
                receiptTerms.Append(",");
                receiptTerms.Append(delivery.ReceiptTerminalNames()[i]);
            }

            string[] item = new string[] {
                DateTime.Now.ToShortTimeString(),
                delivery.Message().Length.ToString(),
                checksum.ToString(),
                receiptTerms.ToString(),
                delivery.SourceApplication(),
                delivery.SourceHost(),
                delivery.SourceTerminalName(),
                uxSubShowText.Checked ? delivery.MessageString() : ""};

            if (uxSubList.Items.Count >= Convert.ToInt32(uxSubMaxShow.Value))
            {
                uxSubList.Items.RemoveAt(0);
            }

            uxSubList.Items.Add(new ListViewItem(item));

            uxSubNumMsg.Text = (Int32.Parse(uxSubNumMsg.Text) + 1).ToString();
            uxStatusText.Text = "Publication received.";
        }

        void LightRequestReceived(DetailedDelivery delivery)
        {
            long checksum = CheckSum(delivery.Message());

            GC.Collect();
            GC.WaitForPendingFinalizers();

            StringBuilder receiptTerms = new StringBuilder(delivery.ReceiptTerminalNames()[0]);
            for (int i = 1; i < delivery.ReceiptTerminalNames().Length; i++)
            {
                receiptTerms.Append(",");
                receiptTerms.Append(delivery.ReceiptTerminalNames()[i]);
            }

            string[] item = new string[] {
                DateTime.Now.ToShortTimeString(),
                delivery.Message().Length.ToString(),
                checksum.ToString(),
                receiptTerms.ToString(),
                delivery.SourceApplication(),
                delivery.SourceHost(),
                delivery.SourceTerminalName(),
                uxRespShowReq.Checked ? delivery.MessageString() : ""};

            if (uxRespList.Items.Count >= Convert.ToInt32(uxRespMax.Value))
            {
                uxRespList.Items.RemoveAt(0);
            }

            uxRespList.Items.Add(new ListViewItem(item));

            uxStatusText.Text = "Request received.";
        }

        void m_client_OnPublishReceived(DetailedDelivery delivery)
        {
            this.BeginInvoke(m_lightPubReceived, delivery);
        }

        private void uxStop_Click(object sender, EventArgs e)
        {
            try
            {
                m_client.StopDeliveries();
                m_client.Dispose();
                m_client = null;

                uxStart.Enabled = true;
                uxStop.Enabled = false;
                uxTermRegister.Enabled = false;
                uxPubSend.Enabled = false;

                uxStatusState.Text = "Stopped";
                uxStatusText.Text = "Lightrail stopped.";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to stop: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void uxTermRegister_Click(object sender, EventArgs e)
        {
            try
            {
                TerminalStandpoint standpoint;
                if (uxPubCheck.Checked)
                {
                    standpoint = TerminalStandpoint.Publisher;
                }
                else if (uxSubCheck.Checked)
                {
                    standpoint = TerminalStandpoint.Subscriber;
                }
                else if (uxReqCheck.Checked)
                {
                    standpoint = TerminalStandpoint.Requester;
                }
                else
                {
                    standpoint = TerminalStandpoint.Responder;
                }

                Guid keyId = Guid.Empty;
                if (!String.IsNullOrEmpty(uxTermSecKey.Text))
                {
                    keyId = Guid.NewGuid();
                    m_client.RegisterSecurityKey(keyId.ToString(), Encoding.UTF8.GetBytes(uxTermSecKey.Text));
                }

                if (keyId == Guid.Empty)
                {
                    m_client.RegisterTerminal(standpoint, uxTermName.Text, "", uxTermMatch.Text);
                }
                else
                {
                    m_client.RegisterTerminal(standpoint, uxTermName.Text, "", uxTermMatch.Text, keyId.ToString());
                }

                uxStatusText.Text = String.Format("{0} Registered.", uxTermName.Text);
                uxTermName.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to register: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private long CheckSum(byte[] data)
        {
            long val = 0;
            for (int i = 0; i < data.Length; i++)
            {
                val += Convert.ToInt32(data[i]);
            }
            return val;
        }

        /// <summary>
        /// Read an entire file into memory.
        /// </summary>
        /// <param name="filepath">The path to the file to read.</param>
        /// <returns>The binary array from the file.</returns>
        private byte[] ReadEntireFile(string filepath)
        {
            System.IO.FileInfo file = new System.IO.FileInfo(filepath);
            System.IO.FileStream stream = file.OpenRead();

            byte[] buffer = new byte[10240];
            List<byte> filebody = new List<byte>();
            int size = stream.Read(buffer, 0, 10240);
            int totalsize = 0;
            while (size > 0)
            {
                for (int i = 0; i < size; i++)
                {
                    filebody.Add(buffer[i]);
                    totalsize++;
                }

                size = stream.Read(buffer, 0, 10240);
            }
            stream.Close();

            return filebody.ToArray();
        }

        private void uxPubSend_Click(object sender, EventArgs e)
        {
            try
            {
                Delivery delivery = null;
                if (uxPubMsgFile.Checked)
                {
                    byte[] filebody = ReadEntireFile(uxPubSendFile.Text);

                    delivery = new Delivery(filebody, DeliveryEncoding.None);
                }
                else
                {
                    delivery = new Delivery(uxPubSendMsg.Text);
                }

                long checksum = CheckSum(delivery.Message());
                uxPubChecksum.Text = checksum.ToString();

                int numToSend = Convert.ToInt32(uxPubNumMsgToSend.Value);
                for (int i = 0; i < numToSend; i++)
                {
                    m_client.Publish(uxPubSendTerm.Text, delivery);
                    uxPubItNum.Text = String.Format("Sent {0} of {1}", (i + 1), numToSend);
                }

                uxStatusText.Text = "Publication(s) sent.";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to publish: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void uxMenuClear_Click(object sender, EventArgs e)
        {
            uxSubList.Items.Clear();
            uxSubNumMsg.Text = "0";

            uxReqList.Items.Clear();
            uxRespList.Items.Clear();
            uxListErrors.Items.Clear();

            uxStatusText.Text = "Messages cleared.";
        }

        private void uxReqSend_Click(object sender, EventArgs e)
        {
            try
            {
                uxReqList.Items.Clear();

                Delivery delivery = new Delivery(uxReqMsg.Text);

                int numToSend = Convert.ToInt32(uxReqNumSend.Value);
                for (int i = 0; i < numToSend; i++)
                {
                    IAsyncResult result = m_doRequest.BeginInvoke(uxReqTermName.Text, delivery, null, null);
                    result.AsyncWaitHandle.WaitOne();
                    
                    if (!result.IsCompleted)
                    {
                        throw new Exception("The event wait failed?");
                    }

                    DetailedDelivery response = null;
                    if (m_response is Exception)
                    {
                        throw (Exception)m_response;
                    }
                    else
                    {
                        response = (DetailedDelivery)m_response;
                    }

                    if (uxReqList.Items.Count >= Convert.ToInt32(uxReqMaxShow.Value))
                    {
                        uxReqList.Items.RemoveAt(0);
                    }

                    uxReqList.Items.Add(new ListViewItem(new string[] {
                        DateTime.Now.ToShortTimeString(),
                        response.Message().Length.ToString(),
                        response.SourceApplication(),
                        response.SourceHost(),
                        uxRespShowMsg.Checked ? response.MessageString() : "",}));

                    uxReqNumSent.Text = String.Format("Req/Resp {0} of {1}", (i + 1), numToSend);
                }

                uxStatusText.Text = "Request/Response(s) completed.";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to request: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void uxPubFileBrowse_Click(object sender, EventArgs e)
        {
            FileDialog dialog = new OpenFileDialog();
            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                uxPubSendFile.Text = dialog.FileName;
            }
        }

        private void uxManualResp_CheckedChanged(object sender, EventArgs e)
        {
            if (uxManualResp.Checked)
            {
                uxRespond.Enabled = true;
            }
            else
            {
                uxRespond.Enabled = false;
            }
        }

        private void uxRespFileBrowse_Click(object sender, EventArgs e)
        {
            try
            {
                FileDialog dialog = new OpenFileDialog();
                if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    uxRespFileText.Text = dialog.FileName;

                    lock (this)
                    {
                        m_loadedResponse = ReadEntireFile(uxRespFileText.Text);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load response file: " + ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
