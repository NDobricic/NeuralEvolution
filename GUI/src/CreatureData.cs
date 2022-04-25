using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    class CreatureData
    {
        public static int TotalCreatures { get; private set; } = 0;

        public SKColor Color { get; private set; }

        public int StartCycle { get; private set; }
        public int AliveCycles { get; private set; }
        public Point<UInt16>[] Position { get; private set; }

        public static void Reset()
        {
            TotalCreatures = 0;
        }

        public CreatureData(string inputFile)
        {
            byte[] data = File.ReadAllBytes(inputFile);
            StartCycle = BitConverter.ToInt32(data, 0);

            Color = new SKColor(data[4], data[5], data[6]);

            AliveCycles = (data.Length - sizeof(int) - 3 * sizeof(byte)) / (2 * sizeof(UInt16));

            Position = new Point<UInt16>[AliveCycles];

            for(int i = 0; i < AliveCycles; i++)
            {
                UInt16 x = BitConverter.ToUInt16(data, 7 + i * (2 * sizeof(UInt16)));
                UInt16 y = BitConverter.ToUInt16(data, 7 + i * (2 * sizeof(UInt16)) + sizeof(UInt16));

                Position[i] = new Point<UInt16>(x, y);
            }

            TotalCreatures++;
        }
    }
}
