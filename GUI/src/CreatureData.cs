using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    public struct NNConnection
    {
        public bool SourceType { get; private set; }
        public byte SourceIndex { get; private set; }
        public bool SinkType { get; private set; }
        public byte SinkIndex { get; private set; }
        public float Value { get; private set; }

        public NNConnection(UInt32 gene)
        {
            SourceType = Convert.ToBoolean(gene & 1);
            gene >>= 1;

            SourceIndex = Convert.ToByte(gene & 0b1111111);
            gene >>= 7;

            SinkType = Convert.ToBoolean(gene & 1);
            gene >>= 1;

            SinkIndex = Convert.ToByte(gene & 0b1111111);
            gene >>= 7;

            Value = (Int16)gene / 8192.0f;
        }
    }

    public class CreatureData
    {
        public static int TotalCreatures { get; private set; } = 0;

        public SKColor Color { get; private set; }

        public int StartCycle { get; private set; }
        public int AliveCycles { get; private set; }
        public Point<UInt16>[] Position { get; private set; }

        public List<NNConnection> Connections { get; private set; }
        public byte NumInputs { get; private set; }
        public byte NumInternals { get; private set; }
        public byte NumOutputs { get; private set; }


        public static void Reset()
        {
            TotalCreatures = 0;
        }

        public CreatureData(string inputFile)
        {
            byte[] data = File.ReadAllBytes(inputFile);
            int offset = 0;

            NumInputs = data[0];
            NumInternals = data[1];
            NumOutputs = data[2];
            offset += 3;

            Int32 numGenes = BitConverter.ToInt32(data, offset);
            offset += sizeof(Int32);

            Connections = new List<NNConnection>(numGenes);
            for(int i = 0; i < numGenes; i++)
            {
                UInt32 gene = BitConverter.ToUInt32(data, offset);
                Connections.Add(new NNConnection(gene));
                offset += sizeof(UInt32);
            }

            StartCycle = BitConverter.ToInt32(data, offset);
            offset += sizeof(Int32);

            Color = new SKColor(data[offset], data[offset + 1], data[offset + 2]);
            offset += 3;

            AliveCycles = (data.Length - offset) / (2 * sizeof(UInt16));

            Position = new Point<UInt16>[AliveCycles];

            for(int i = 0; i < AliveCycles; i++)
            {
                UInt16 x = BitConverter.ToUInt16(data, offset  + i * (2 * sizeof(UInt16)));
                UInt16 y = BitConverter.ToUInt16(data, offset + i * (2 * sizeof(UInt16)) + sizeof(UInt16));

                Position[i] = new Point<UInt16>(x, y);
            }

            TotalCreatures++;
        }
    }
}
