using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SkiaSharp.Views.Desktop;
using SkiaSharp;
using System.IO;
using System.Diagnostics;
using System.IO.Pipes;

namespace GUI
{
    public partial class Form1 : Form
    {
        const string simPath = "..\\Simulator";
        const string logPath = "gui.log";

        SKBitmap[] bitmaps;
        Process simProcess;
        NamedPipeServerStream namedPipeServer;
        StreamWriter pipeWriter;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FileStream ostrm;
            StreamWriter writer;

            try
            {
                ostrm = new FileStream(logPath, FileMode.OpenOrCreate, FileAccess.Write);
                writer = new StreamWriter(ostrm);

                Console.SetOut(writer);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Cannot open {logPath} for logging: {ex.Message}. Exiting...", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
        }

        private void startBtn_Click(object sender, EventArgs e)
        {
            try
            {
                Console.WriteLine("Creating a pipe server...");
                namedPipeServer = new NamedPipeServerStream("evolpipe", PipeDirection.InOut, 1, PipeTransmissionMode.Byte);

                Console.WriteLine("Starting the simulation process...");
                var simProcessInfo = new ProcessStartInfo(Path.Combine(simPath, "Simulator.exe"));
                simProcessInfo.WorkingDirectory = simPath;

                simProcess = new Process();
                simProcess.StartInfo = simProcessInfo;
                simProcess.Start();

                Console.WriteLine("Waiting for pipe connection...");
                namedPipeServer.WaitForConnection();

                Console.WriteLine("Pipe successfully connected. Sending 'success' signal...");
                pipeWriter = new StreamWriter(namedPipeServer);

                pipeWriter.Write("success\0");
                pipeWriter.Flush();

                startBtn.Enabled = false;
                stopBtn.Enabled = true;
                numericUpDown.Enabled = false;
            }
            catch (Exception ex)
            {
                string msg = $"An error has occured while trying to start the simulation process";
                MessageBox.Show(msg, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Console.WriteLine(msg + ": { ex.Message}");
            }
        }

        private void stopBtn_Click(object sender, EventArgs e)
        {
            try
            {
                Console.WriteLine("Sending the 'stop' signal...");
                pipeWriter.Write("stop\0");
                pipeWriter.Flush();

                Console.WriteLine("Waiting for pipe drain...");
                namedPipeServer.WaitForPipeDrain();

                Console.WriteLine("Pipe successfully drained.");

                startBtn.Enabled = true;
                stopBtn.Enabled = false;
                numericUpDown.Enabled = true;

                LoadData();
            }
            catch (Exception ex)
            {
                string msg = $"Couldn't send the stop signal to child process. Is the simulation closed?";
                MessageBox.Show(msg, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                Console.WriteLine($"Couldn't send the stop signal to child process: {ex.Message}");
            }
            finally
            {
                namedPipeServer.Dispose();
            }
        }

        private void LoadData()
        {
            Cursor.Current = Cursors.WaitCursor;

            try
            {
                Console.WriteLine("Loading info file...");
                byte[] infoArr = File.ReadAllBytes(Path.Combine(simPath, "output", "info"));
                int numCycles = Math.Min(BitConverter.ToInt32(infoArr, 0), 30000);

                Console.WriteLine("Generating bitmaps...");
                bitmaps = new SKBitmap[numCycles];
                for (int i = 0; i < numCycles; i++)
                {
                    bitmaps[i] = new SKBitmap(256, 256);
                    bitmaps[i].Erase(SKColors.White);
                }

                numericUpDown.Maximum = numCycles - 1;

                for (int i = 0; i < 50; i++)
                {
                    byte[] arr = File.ReadAllBytes(Path.Combine(simPath, "output", i.ToString()));

                    int startCycle = BitConverter.ToInt32(arr, 0);

                    for (int j = 0; j < numCycles; j++)
                    {
                        Int16 posX = BitConverter.ToInt16(arr, 7 + j * 4);
                        Int16 posY = BitConverter.ToInt16(arr, 7 + j * 4 + 2);

                        bitmaps[j].SetPixel(posX, posY, SKColors.Green);
                    }
                }

                Console.WriteLine("Setting up draw events...");
                glControl.PaintSurface += GlControl_PaintSurface;

                glControl.Refresh();

                Console.WriteLine("Data successfully loaded.");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error while loading data. Check {logPath} for more details.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Console.WriteLine($"Error while loading data: {ex.Message}");
            }
            finally
            {
                Cursor.Current = Cursors.Default;
            }
        }

        private void GlControl_PaintSurface(object sender, SKPaintGLSurfaceEventArgs e)
        {
            int index = (int)numericUpDown.Value;
            var bitmap = bitmaps[index];

            float zoom = 4.0f;
            e.Surface.Canvas.Clear(SKColors.White);
            e.Surface.Canvas.DrawBitmap(bitmap, new SKRect(0, 0, bitmap.Width * zoom, bitmap.Height * zoom));
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            glControl.Refresh();
        }
    }
}
