
namespace GUI
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.glControl = new SkiaSharp.Views.Desktop.SKGLControl();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.skip100Btn = new System.Windows.Forms.Button();
            this.nextFrameBtn = new System.Windows.Forms.Button();
            this.prevFrameBtn = new System.Windows.Forms.Button();
            this.rewind100Btn = new System.Windows.Forms.Button();
            this.playBtn = new System.Windows.Forms.Button();
            this.numericUpDown = new System.Windows.Forms.NumericUpDown();
            this.stopBtn = new System.Windows.Forms.Button();
            this.startBtn = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // glControl
            // 
            this.glControl.BackColor = System.Drawing.Color.Black;
            this.glControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.glControl.Location = new System.Drawing.Point(178, 3);
            this.glControl.Name = "glControl";
            this.glControl.Size = new System.Drawing.Size(824, 824);
            this.glControl.TabIndex = 0;
            this.glControl.VSync = false;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 175F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.glControl, 1, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 302F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1005, 830);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.skip100Btn);
            this.panel1.Controls.Add(this.nextFrameBtn);
            this.panel1.Controls.Add(this.prevFrameBtn);
            this.panel1.Controls.Add(this.rewind100Btn);
            this.panel1.Controls.Add(this.playBtn);
            this.panel1.Controls.Add(this.numericUpDown);
            this.panel1.Controls.Add(this.stopBtn);
            this.panel1.Controls.Add(this.startBtn);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(175, 830);
            this.panel1.TabIndex = 1;
            // 
            // skip100Btn
            // 
            this.skip100Btn.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.skip100Btn.Location = new System.Drawing.Point(135, 109);
            this.skip100Btn.Name = "skip100Btn";
            this.skip100Btn.Size = new System.Drawing.Size(35, 25);
            this.skip100Btn.TabIndex = 8;
            this.skip100Btn.Text = ">>";
            this.skip100Btn.UseVisualStyleBackColor = true;
            this.skip100Btn.Click += new System.EventHandler(this.skip100Btn_Click);
            // 
            // nextFrameBtn
            // 
            this.nextFrameBtn.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.nextFrameBtn.Location = new System.Drawing.Point(110, 109);
            this.nextFrameBtn.Name = "nextFrameBtn";
            this.nextFrameBtn.Size = new System.Drawing.Size(25, 25);
            this.nextFrameBtn.TabIndex = 7;
            this.nextFrameBtn.Text = ">";
            this.nextFrameBtn.UseVisualStyleBackColor = true;
            this.nextFrameBtn.Click += new System.EventHandler(this.nextFrameBtn_Click);
            // 
            // prevFrameBtn
            // 
            this.prevFrameBtn.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.prevFrameBtn.Location = new System.Drawing.Point(40, 109);
            this.prevFrameBtn.Name = "prevFrameBtn";
            this.prevFrameBtn.Size = new System.Drawing.Size(25, 25);
            this.prevFrameBtn.TabIndex = 6;
            this.prevFrameBtn.Text = "<";
            this.prevFrameBtn.UseVisualStyleBackColor = true;
            this.prevFrameBtn.Click += new System.EventHandler(this.prevFrameBtn_Click);
            // 
            // rewind100Btn
            // 
            this.rewind100Btn.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rewind100Btn.Location = new System.Drawing.Point(5, 109);
            this.rewind100Btn.Name = "rewind100Btn";
            this.rewind100Btn.Size = new System.Drawing.Size(35, 25);
            this.rewind100Btn.TabIndex = 5;
            this.rewind100Btn.Text = "<<";
            this.rewind100Btn.UseVisualStyleBackColor = true;
            this.rewind100Btn.Click += new System.EventHandler(this.rewind100Btn_Click);
            // 
            // playBtn
            // 
            this.playBtn.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.playBtn.Location = new System.Drawing.Point(65, 109);
            this.playBtn.Name = "playBtn";
            this.playBtn.Size = new System.Drawing.Size(45, 25);
            this.playBtn.TabIndex = 4;
            this.playBtn.Text = "▶";
            this.playBtn.UseVisualStyleBackColor = true;
            this.playBtn.Click += new System.EventHandler(this.playBtn_Click);
            // 
            // numericUpDown
            // 
            this.numericUpDown.Enabled = false;
            this.numericUpDown.Location = new System.Drawing.Point(5, 83);
            this.numericUpDown.Maximum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.numericUpDown.Name = "numericUpDown";
            this.numericUpDown.Size = new System.Drawing.Size(165, 20);
            this.numericUpDown.TabIndex = 3;
            this.numericUpDown.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // stopBtn
            // 
            this.stopBtn.Enabled = false;
            this.stopBtn.Location = new System.Drawing.Point(3, 43);
            this.stopBtn.Name = "stopBtn";
            this.stopBtn.Size = new System.Drawing.Size(167, 34);
            this.stopBtn.TabIndex = 2;
            this.stopBtn.Text = "STOP SIMULATION";
            this.stopBtn.UseVisualStyleBackColor = true;
            this.stopBtn.Click += new System.EventHandler(this.stopBtn_Click);
            // 
            // startBtn
            // 
            this.startBtn.Location = new System.Drawing.Point(3, 3);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(167, 34);
            this.startBtn.TabIndex = 1;
            this.startBtn.Text = "START SIMULATION";
            this.startBtn.UseVisualStyleBackColor = true;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 25;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1005, 830);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        SkiaSharp.Views.Desktop.SKGLControl glControl;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button stopBtn;
        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.NumericUpDown numericUpDown;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button playBtn;
        private System.Windows.Forms.Button skip100Btn;
        private System.Windows.Forms.Button nextFrameBtn;
        private System.Windows.Forms.Button prevFrameBtn;
        private System.Windows.Forms.Button rewind100Btn;
    }
}

