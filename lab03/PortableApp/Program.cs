using System;

namespace PortableConsoleApp
{
    class Program
    {
        static void Main()
        {
            Console.WriteLine($"Hi! This is a console app." +
                $"\n\n" +
                $"Username: {Environment.UserName}\n" +
                $"Machine: {Environment.MachineName}\n" +
                $"OS: {Environment.OSVersion}\n" +
                $"Is x64 OS? {Environment.Is64BitOperatingSystem}\n" +
                $"Is x64 process? {Environment.Is64BitProcess}\n" +
                $"Processor count: {Environment.ProcessorCount}\n" +
                $"\n\n" +
                $"Press any button to exit.\n");
            Console.ReadKey();
        }
    }
}
