
namespace GUI
{
    partial class StatsWindow
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
            SonicPlot.Core.PlotLayout plotLayout1 = new SonicPlot.Core.PlotLayout();
            SonicPlot.Core.Margin margin1 = new SonicPlot.Core.Margin();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(StatsWindow));
            SonicPlot.Core.PlotStyle plotStyle1 = new SonicPlot.Core.PlotStyle();
            SonicPlot.Core.Axis axis1 = new SonicPlot.Core.Axis();
            SonicPlot.Core.AxisBounds axisBounds1 = new SonicPlot.Core.AxisBounds();
            SonicPlot.Core.AxisStyle axisStyle1 = new SonicPlot.Core.AxisStyle();
            SonicPlot.Core.TickCollection tickCollection1 = new SonicPlot.Core.TickCollection();
            SonicPlot.Core.Axis axis2 = new SonicPlot.Core.Axis();
            SonicPlot.Core.AxisBounds axisBounds2 = new SonicPlot.Core.AxisBounds();
            SonicPlot.Core.AxisStyle axisStyle2 = new SonicPlot.Core.AxisStyle();
            SonicPlot.Core.TickCollection tickCollection2 = new SonicPlot.Core.TickCollection();
            this.formsPlot1 = new SonicPlot.WinForms.Skia.FormsPlot();
            this.SuspendLayout();
            // 
            // formsPlot1
            // 
            this.formsPlot1.Dock = System.Windows.Forms.DockStyle.Fill;
            margin1.Bottom = 30;
            margin1.Left = 50;
            margin1.Right = 10;
            margin1.Top = 10;
            plotLayout1.Padding = margin1;
            this.formsPlot1.Layout = plotLayout1;
            this.formsPlot1.Location = new System.Drawing.Point(0, 0);
            this.formsPlot1.Name = "formsPlot1";
            this.formsPlot1.Size = new System.Drawing.Size(584, 362);
            this.formsPlot1.Style = plotStyle1;
            this.formsPlot1.TabIndex = 0;
            axisBounds1.AbsoluteMaximum = double.PositiveInfinity;
            axisBounds1.AbsoluteMinimum = double.NegativeInfinity;
            axisBounds1.Maximum = 1D;
            axisBounds1.Minimum = -1D;
            axis1.Bounds = axisBounds1;
            axis1.Parent = null;
            axisStyle1.LabelAntialias = true;
            axisStyle1.LabelFontName = "Segoe UI";
            axisStyle1.MajorTickSize = 7;
            axisStyle1.MinorTickSize = 4;
            axisStyle1.Thickness = 1;
            axisStyle1.TickThickness = 1;
            axis1.Style = axisStyle1;
            axis1.Ticks = tickCollection1;
            this.formsPlot1.XAxis = axis1;
            axisBounds2.AbsoluteMaximum = double.PositiveInfinity;
            axisBounds2.AbsoluteMinimum = double.NegativeInfinity;
            axisBounds2.Maximum = 1D;
            axisBounds2.Minimum = -1D;
            axis2.Bounds = axisBounds2;
            axis2.Parent = null;
            axisStyle2.LabelAntialias = true;
            axisStyle2.LabelFontName = "Segoe UI";
            axisStyle2.MajorTickSize = 7;
            axisStyle2.MinorTickSize = 4;
            axisStyle2.Thickness = 1;
            axisStyle2.TickThickness = 1;
            axis2.Style = axisStyle2;
            axis2.Ticks = tickCollection2;
            this.formsPlot1.YAxis = axis2;
            // 
            // StatsWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 362);
            this.Controls.Add(this.formsPlot1);
            this.MinimumSize = new System.Drawing.Size(200, 100);
            this.Name = "StatsWindow";
            this.Text = "StatsWindow";
            this.ResumeLayout(false);

        }

        #endregion

        private SonicPlot.WinForms.Skia.FormsPlot formsPlot1;
    }
}