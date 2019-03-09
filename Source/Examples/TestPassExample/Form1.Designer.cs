namespace TestPassExample
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.uxName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.uxRadSub = new System.Windows.Forms.RadioButton();
            this.uxRadPub = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.uxText = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.uxName);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.uxRadSub);
            this.groupBox1.Controls.Add(this.uxRadPub);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(571, 77);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Action";
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button2.Enabled = false;
            this.button2.Location = new System.Drawing.Point(490, 42);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 5;
            this.button2.Text = "Stop";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(490, 11);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "Start";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // uxName
            // 
            this.uxName.Location = new System.Drawing.Point(131, 41);
            this.uxName.Name = "uxName";
            this.uxName.Size = new System.Drawing.Size(127, 20);
            this.uxName.TabIndex = 3;
            this.uxName.Text = "Test";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(128, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Name";
            // 
            // uxRadSub
            // 
            this.uxRadSub.AutoSize = true;
            this.uxRadSub.Location = new System.Drawing.Point(6, 42);
            this.uxRadSub.Name = "uxRadSub";
            this.uxRadSub.Size = new System.Drawing.Size(75, 17);
            this.uxRadSub.TabIndex = 1;
            this.uxRadSub.Text = "Subscriber";
            this.uxRadSub.UseVisualStyleBackColor = true;
            // 
            // uxRadPub
            // 
            this.uxRadPub.AutoSize = true;
            this.uxRadPub.Checked = true;
            this.uxRadPub.Location = new System.Drawing.Point(6, 19);
            this.uxRadPub.Name = "uxRadPub";
            this.uxRadPub.Size = new System.Drawing.Size(68, 17);
            this.uxRadPub.TabIndex = 0;
            this.uxRadPub.TabStop = true;
            this.uxRadPub.Text = "Publisher";
            this.uxRadPub.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.uxText);
            this.groupBox2.Location = new System.Drawing.Point(12, 95);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(571, 111);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Text";
            // 
            // uxText
            // 
            this.uxText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.uxText.Enabled = false;
            this.uxText.Location = new System.Drawing.Point(6, 19);
            this.uxText.Multiline = true;
            this.uxText.Name = "uxText";
            this.uxText.Size = new System.Drawing.Size(559, 86);
            this.uxText.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(595, 218);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Messaging Text Example";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox uxName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton uxRadSub;
        private System.Windows.Forms.RadioButton uxRadPub;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox uxText;
    }
}

