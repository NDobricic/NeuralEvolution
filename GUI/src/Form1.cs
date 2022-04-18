using System;
using System.Windows.Forms;
using SkiaSharp.Views.Desktop;
using SkiaSharp;
using System.IO;
using System.Diagnostics;
using System.IO.Pipes;
using System.Collections.Generic;

namespace GUI
{
    public partial class Form1 : Form
    {
        Process simProcess;
        NamedPipeServerStream namedPipeServer;
        StreamWriter pipeWriter;

        SKBitmap currentFrame;
        CreatureData[] creatures;
        List<Point<UInt16>>[] food;

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

                currentFrame = new SKBitmap(Config.MapSizeX, Config.MapSizeY);

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

        private void UpdateFrame(int index)
        {
            currentFrame.Erase(SKColors.White);

            foreach(var coord in food[index])
            {
                currentFrame.SetPixel(coord.X, coord.Y, SKColors.Red);
            }

            for(int i = 0; i < CreatureData.TotalCreatures; i++)
            {
                if (index >= creatures[i].StartCycle && index < creatures[i].StartCycle + creatures[i].AliveCycles)
                {
                    int posIndex = index - creatures[i].StartCycle;
                    int x = creatures[i].Position[posIndex].X;
                    int y = creatures[i].Position[posIndex].Y;

                    currentFrame.SetPixel(x, y, SKColors.Green);
                }
            }
        }

        private void LoadData()
        {
            Cursor.Current = Cursors.WaitCursor;

            try
            {
                Console.WriteLine("Loading info file...");
                byte[] infoArr = File.ReadAllBytes(Path.Combine(Config.SimPath, Config.OutputPath, "info"));
                int numCycles = BitConverter.ToInt32(infoArr, 0);
                int totalCreatures = BitConverter.ToInt32(infoArr, 4);

                numericUpDown.Maximum = numCycles - 1;

                Console.Write("Loading food data...");
                int lineStart = 0;
                byte[] mapdata = File.ReadAllBytes(Path.Combine(Config.SimPath, Config.OutputPath, "mapdata"));
                food = new List<Point<UInt16>>[numCycles];

                for (int i = 0; i < numCycles; i++)
                {
                    Int32 numFood = BitConverter.ToInt32(mapdata, lineStart);

                    food[i] = new List<Point<UInt16>>(numFood);
                    for (int j = 0; j < numFood; j++)
                    {
                        UInt16 x = BitConverter.ToUInt16(mapdata, lineStart + 4 + j * 4);
                        UInt16 y = BitConverter.ToUInt16(mapdata, lineStart + 4 + j * 4 + 2);

                        food[i].Add(new Point<UInt16>(x, y));
                    }

                    lineStart += 4 + numFood * 4;
                }

                Console.WriteLine("Loading creature data...");

                //for (int i = 0; i < totalCreatures; i++)
                //{
                //    byte[] arr = File.ReadAllBytes(Path.Combine(Config.SimPath, "output", i.ToString()));

                //    int startCycle = BitConverter.ToInt32(arr, 0);

                //    var color = new SKColor(arr[4], arr[5], arr[6]);

                //    for (int j = 0; 7 + j * 4 < arr.Length; j++)
                //    {
                //        UInt16 posX = BitConverter.ToUInt16(arr, 7 + j * 4);
                //        UInt16 posY = BitConverter.ToUInt16(arr, 7 + j * 4 + 2);

                //        //bitmaps[j].SetPixel(posX, posY, color);
                //        bitmaps[startCycle + j].SetPixel(posX, posY, SKColors.Green);
                //    }
                //}

                creatures = new CreatureData[totalCreatures];
                for (int i = 0; i < totalCreatures; i++)
                {
                    var data = new CreatureData(Path.Combine(Config.SimPath, Config.OutputPath, i.ToString()));

                    creatures[i] = data;
                }

                UpdateFrame(0);

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
            //int index = (int)numericUpDown.Value;
            //var bitmap = bitmaps[index];

            float zoom = 4.0f;
            e.Surface.Canvas.Clear(SKColors.White);

            int minSize = Math.Min(glControl.Width, glControl.Height);
            e.Surface.Canvas.DrawBitmap(currentFrame, new SKRect(0, 0, minSize, minSize));
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            UpdateFrame((int)numericUpDown.Value);

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
