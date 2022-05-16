using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SkiaSharp;

namespace GUI
{
    public partial class NeuralNetWindow : Form
    {
        readonly CreatureData creature;

        private readonly SKPaint neuronPaint = new SKPaint()
        {
            Color = SKColors.White,
            IsAntialias = true
        };

        public NeuralNetWindow(CreatureData data)
        {
            InitializeComponent();

            creature = data;
        }

        private void skglControl1_PaintSurface(object sender, SkiaSharp.Views.Desktop.SKPaintGLSurfaceEventArgs e)
        {
            e.Surface.Canvas.Clear(SKColors.Black);

            DrawConnections(e.Surface.Canvas);

            DrawInputNeurons(e.Surface.Canvas);

            DrawInternalNeurons(e.Surface.Canvas);

            DrawOutputNeurons(e.Surface.Canvas);
        }

        private void DrawConnections(SKCanvas canvas)
        {
            foreach (var connection in creature.Connections)
            {
                if (connection.SourceType == false && connection.SinkType == true) //input to internal
                {
                    var path = new SKPath();
                    var a = GetInputNeuronPos(connection.SourceIndex % creature.NumInputs, out _);
                    var b = GetInternalNeuronPos(connection.SinkIndex % creature.NumInternals, out _);

                    path.MoveTo(a);
                    path.LineTo(b);

                    canvas.DrawPath(path, GetConnectionPaint(connection.Value));
                }
                else if (connection.SourceType == true && connection.SinkType == false) //internal to output
                {
                    var path = new SKPath();
                    var a = GetInternalNeuronPos(connection.SourceIndex % creature.NumInternals, out _);
                    var b = GetOutputNeuronPos(connection.SinkIndex % creature.NumOutputs, out _);

                    path.MoveTo(a);
                    path.LineTo(b);

                    canvas.DrawPath(path, GetConnectionPaint(connection.Value));
                }
                else if (connection.SourceType == false && connection.SinkType == false) //input to output
                {
                    var path = new SKPath();
                    var a = GetInputNeuronPos(connection.SourceIndex % creature.NumInputs, out _);
                    var b = GetOutputNeuronPos(connection.SinkIndex % creature.NumOutputs, out _);

                    path.MoveTo(a);
                    path.LineTo(b);

                    canvas.DrawPath(path, GetConnectionPaint(connection.Value));
                }
                else //internal to internal
                {
                    int source = connection.SourceIndex % creature.NumInternals;
                    int sink = connection.SinkIndex % creature.NumInternals;

                    var a = GetInternalNeuronPos(source, out var radius);
                    var b = GetInternalNeuronPos(sink, out _);

                    var path = new SKPath();
                    SKPoint pt1, pt2;

                    if (sink == source)
                    {
                        pt1 = new SKPoint(a.X - radius * 2, a.Y - radius * 2);
                        pt2 = new SKPoint(b.X + radius * 2, b.Y - radius * 2);
                    }
                    else
                    {
                        pt1 = new SKPoint(a.X - radius * 2, a.Y);
                        pt2 = new SKPoint(b.X - radius * 2, b.Y);
                    }


                    path.MoveTo(a);
                    path.CubicTo(pt1, pt2, b);

                    canvas.DrawPath(path, GetConnectionPaint(connection.Value));
                }
            }
        }

        private SKPaint GetConnectionPaint(float strength)
        {
            SKColor color;
            if (strength == 0)
                color = SKColors.Black;
            else if (strength > 0)
                color = SKColors.Green;
            else
                color = SKColors.Red;

            const float maxWidth = 10.0f;
            const float minWidth = 3.0f;
            float width = (strength / 4.0f) * (maxWidth - minWidth) + minWidth; 

            return new SKPaint()
            {
                Color = color,
                IsAntialias = true,
                StrokeWidth = width,
                Style = SKPaintStyle.Stroke
            };
        }

        private void DrawInputNeurons(SKCanvas canvas)
        {
            for (int i = 0; i < creature.NumInputs; i++)
            {
                canvas.DrawCircle(GetInputNeuronPos(i, out var radius), radius, neuronPaint);
            }
        }

        private void DrawInternalNeurons(SKCanvas canvas)
        {
            for (int i = 0; i < creature.NumInternals; i++)
            {
                canvas.DrawCircle(GetInternalNeuronPos(i, out var radius), radius, neuronPaint);
            }
        }

        private void DrawOutputNeurons(SKCanvas canvas)
        {
            for (int i = 0; i < creature.NumOutputs; i++)
            {
                canvas.DrawCircle(GetOutputNeuronPos(i, out float radius), radius, neuronPaint);
            }
        }

        private SKPoint GetInputNeuronPos(int index, out float radius)
        {
            float margin = 0.1f;

            float maxRadius = skglControl1.Width / 10.0f;
            radius = Math.Min(maxRadius, 
                skglControl1.Height / (creature.NumInputs + (creature.NumInputs + 1) * margin) / 2);

            var yPos = GetNeuronYPos(creature.NumInputs, index, radius, margin);

            return new SKPoint(radius * (margin + 1), yPos);
        }

        private SKPoint GetInternalNeuronPos(int index, out float radius)
        {
            float margin = 1;

            float maxRadius = skglControl1.Width / 10.0f;
            radius = Math.Min(maxRadius,
                skglControl1.Height / (creature.NumInternals + (creature.NumInternals) * margin) / 2);

            var yPos = GetNeuronYPos(creature.NumInternals, index, radius, margin);

            return new SKPoint(skglControl1.Width / 2.0f, yPos);
        }

        private SKPoint GetOutputNeuronPos(int index, out float radius)
        {
            float margin = 0.5f;

            float maxRadius = skglControl1.Width / 10.0f;
            radius = Math.Min(maxRadius,
                skglControl1.Height / (creature.NumOutputs + (creature.NumOutputs + 1) * margin) / 2);

            var yPos = GetNeuronYPos(creature.NumOutputs, index, radius, margin);

            return new SKPoint(skglControl1.Width - radius * (margin + 1), yPos);
        }

        float GetNeuronYPos(int numNeurons, int index, float radius, float margin)
        {
            float centerNeuron = numNeurons / 2.0f - 0.5f;
            float yPos = skglControl1.Height / 2.0f - (centerNeuron - index) * radius * 2 * (1 + margin);

            return yPos;
        }
    }
}
