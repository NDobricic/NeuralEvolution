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
        SKBitmap[] bitmaps;
        Process simProcess;
        NamedPipeServerStream namedPipeServer;
        StreamWriter pipeWriter;

        bool animPlaying = false;

        public Form1()
        {
            InitializeComponent();

            numericUpDown.Controls[0].Visible = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FileStream ostrm;
            StreamWriter writer;

            try
            {
                ostrm = new FileStream(Config.LogPath, FileMode.OpenOrCreate, FileAccess.Write);
                writer = new StreamWriter(ostrm);
                writer.AutoFlush = true;

                Console.SetOut(writer);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Cannot open {Config.LogPath} for logging: {ex.Message}. Exiting...", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            bool success = Config.LoadFromFile("..\\config.ini");
            if(!success)
            {
                MessageBox.Show("An error has occured while loading settings. Exiting...", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Console.WriteLine("An error has occured while loading settings. Exiting...");
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
                var simProcessInfo = new ProcessStartInfo(Path.Combine(Config.SimPath, "Simulator.exe"));
                simProcessInfo.WorkingDirectory = Config.SimPath;

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

                simProcess.WaitForExit();

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
                byte[] infoArr = File.ReadAllBytes(Path.Combine(Config.SimPath, Config.OutputPath, "info"));
                int numCycles = Math.Min(BitConverter.ToInt32(infoArr, 0), 30000);
                int totalCreatures = BitConverter.ToInt32(infoArr, 4);

                Console.WriteLine("Generating bitmaps...");
                bitmaps = new SKBitmap[numCycles];
                for (int i = 0; i < numCycles; i++)
                {
                    bitmaps[i] = new SKBitmap(Config.MapSizeX, Config.MapSizeY);
                    bitmaps[i].Erase(SKColors.White);
                }

                numericUpDown.Maximum = numCycles - 1;

                Console.Write("Loading food data...");
                int lineStart = 0;
                byte[] mapdata = File.ReadAllBytes(Path.Combine(Config.SimPath, "output", "mapdata"));

                for (int i = 0; i < numCycles; i++)
                {
                    Int32 numFood = BitConverter.ToInt32(mapdata, lineStart);

                    for (int j = 0; j < numFood; j++)
                    {
                        UInt16 posX = BitConverter.ToUInt16(mapdata, lineStart + 4 + j * 4);
                        UInt16 posY = BitConverter.ToUInt16(mapdata, lineStart + 4 + j * 4 + 2);

                        bitmaps[i].SetPixel(posX, posY, SKColors.Red);
                    }

                    lineStart += 4 + numFood * 4;
                }

                Console.Write("Loading creature data...");
                for (int i = 0; i < totalCreatures; i++)
                {
                    byte[] arr = File.ReadAllBytes(Path.Combine(Config.SimPath, "output", i.ToString()));

                    int startCycle = BitConverter.ToInt32(arr, 0);

                    var color = new SKColor(arr[4], arr[5], arr[6]);

                    for (int j = 0; 7 + j * 4 < arr.Length; j++)
                    {
                        UInt16 posX = BitConverter.ToUInt16(arr, 7 + j * 4);
                        UInt16 posY = BitConverter.ToUInt16(arr, 7 + j * 4 + 2);

                        //bitmaps[j].SetPixel(posX, posY, color);
                        bitmaps[startCycle + j].SetPixel(posX, posY, SKColors.Green);
                    }
                }

                Console.WriteLine("Setting up draw events...");
                glControl.PaintSurface += GlControl_PaintSurface;

                glControl.Refresh();

                Console.WriteLine("Data successfully loaded.");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error while loading data. Check {Config.LogPath} for more details.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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

            int minSize = Math.Min(glControl.Width, glControl.Height);
            e.Surface.Canvas.DrawBitmap(bitmap, new SKRect(0, 0, minSize, minSize));
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            glControl.Refresh();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (animPlaying)
                numericUpDown.Value = Math.Min(numericUpDown.Value + 1, numericUpDown.Maximum);
        }

        private void playBtn_Click(object sender, EventArgs e)
        {
            animPlaying = !animPlaying;
            if (animPlaying)
                playBtn.Text = "■";
            else
                playBtn.Text = "▶";
        }

        private void rewind100Btn_Click(object sender, EventArgs e)
        {
            numericUpDown.Value = Math.Max(numericUpDown.Value - 100, numericUpDown.Minimum);
        }

        private void prevFrameBtn_Click(object sender, EventArgs e)
        {
            numericUpDown.Value = Math.Max(numericUpDown.Value - 1, numericUpDown.Minimum);
        }

        private void nextFrameBtn_Click(object sender, EventArgs e)
        {
            numericUpDown.Value = Math.Min(numericUpDown.Value + 1, numericUpDown.Maximum);
        }

        private void skip100Btn_Click(object sender, EventArgs e)
        {
            numericUpDown.Value = Math.Min(numericUpDown.Value + 100, numericUpDown.Maximum);
        }
    }
}
