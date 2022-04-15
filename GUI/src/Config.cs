using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    static class Config
    {
		public const string SimPath = "..\\Simulator";
		public const string LogPath = "gui.log";

		public static UInt16 NumCreatures { get; private set; }
		public static UInt16 MapSizeX { get; private set; }
		public static UInt16 MapSizeY { get; private set; }
		public static string OutputPath { get; private set; }


		public static bool LoadFromFile(string configFile)
		{
			StreamReader file;
			try {
				file = new StreamReader(configFile);
			}
			catch (Exception ex) {
				Console.WriteLine($"Failed to open the config file: {ex.Message}");
				return false;
			}

			int lineNum = -1;
			while (!file.EndOfStream)
			{
				lineNum++;

				string line = file.ReadLine();

				if (line.Length == 0 || line[0] == '#')
					continue;

				line = new string(line.ToCharArray()
					.Where(c => !Char.IsWhiteSpace(c))
					.ToArray());

				if (line.Length == 0)
					continue;

				int equalsPos = line.IndexOf('=');
				if (equalsPos == -1)
				{
					Console.WriteLine($"Failed to load parameter at line {lineNum}: '=' is missing.");
					continue;
				}

				string paramName = line.Substring(0, equalsPos);
				string paramValue = line.Substring(equalsPos + 1);

				if (paramValue == "true") paramValue = "1";
				else if (paramValue == "false") paramValue = "0";

				Int32 paramInt; UInt32 paramUInt;
				UInt16 paramUShort;
				float paramFloat; bool paramBool;

				bool isInt = Int32.TryParse(paramValue, out paramInt);
				bool isUInt = UInt32.TryParse(paramValue, out paramUInt);
				bool isUShort = UInt16.TryParse(paramValue, out paramUShort);
				bool isFloat = float.TryParse(paramValue, out paramFloat);
				bool isBool = bool.TryParse(paramValue, out paramBool);

				if (paramName == "numCreatures" && isUShort) NumCreatures = paramUShort;
				else if (paramName == "mapSizeX" && isUShort) MapSizeX = paramUShort;
				else if (paramName == "mapSizeY" && isUShort) MapSizeY = paramUShort;
				else if (paramName == "outputPath") OutputPath = paramValue;
				else
				{
					Console.WriteLine($"Unable to load parameter '{paramName}' at line {lineNum}.");
				}
			}

			return true;
		}
    }
}
