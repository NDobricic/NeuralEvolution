using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using SonicPlot.Core;
using System.Linq;

namespace GUI
{
    public partial class StatsWindow : Form
    {
        double[] ys;

        public StatsWindow(string inputFile)
        {
            InitializeComponent();

            byte[] statsBinary = File.ReadAllBytes(inputFile);
            int size = statsBinary.Length / sizeof(UInt32);

            ys = new double[size];
            for (int i = 0; i < size; i++)
            {
                ys[i] = BitConverter.ToUInt32(statsBinary, i * sizeof(UInt32));
            }

            var signal = new PlottableSignal(ys);
            formsPlot1.Plottables.Add(signal);

            formsPlot1.XAxis.Bounds.Minimum = 0;
            formsPlot1.XAxis.Bounds.Maximum = size;
            formsPlot1.YAxis.Bounds.Minimum = 0;
            formsPlot1.YAxis.Bounds.Maximum = 1.2 * ys.Max();

            formsPlot1.XAxis.Bounds.AbsoluteMinimum = 0;
            formsPlot1.XAxis.Bounds.AbsoluteMaximum = size;

            formsPlot1.Refresh();
        }
    }
}
