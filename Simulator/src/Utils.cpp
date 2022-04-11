#include "Utils.h"
#include "Log.h"

namespace utils
{
    char* ReadFromPipe(HANDLE fileHandle, bool waitForInput)
	{
		ULONG read = 0;
		int index = 0;
		char* output = new char[1024];

		do {
			if(waitForInput)
				ReadFile(fileHandle, output + index++, 1, &read, NULL);
			else
			{
				DWORD bytesAvail = 0;
				PeekNamedPipe(fileHandle, NULL, 0, NULL, &bytesAvail, NULL);

				if (bytesAvail)
					ReadFile(fileHandle, output + index++, 1, &read, NULL);
			}
		} while (read > 0 && *(output + index - 1) != 0);

		return output;
	}
}