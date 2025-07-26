using System;
using System.Diagnostics;
using System.Threading;

namespace SimpleCpuTest
{
    public class Program
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("CPU Performance Test");

            // Дайте системі "прогрітися"
            RunTest("Warm-up", () => Thread.Sleep(100));

            // Запустіть ваші тести
            RunTest("Math Intensive Test", MathIntensiveTask);
            RunTest("String Manipulation Test", StringManipulationTask);

            Console.WriteLine("Test finished.");
        }

        public static void RunTest(string testName, Action testAction)
        {
            Console.WriteLine($"\nRunning: {testName}");
            Stopwatch stopwatch = Stopwatch.StartNew();

            testAction();

            stopwatch.Stop();
            Console.WriteLine($"Elapsed time: {stopwatch.ElapsedMilliseconds} ms");
        }

        public static void MathIntensiveTask()
        {
            double result = 0;
            for (int i = 0; i < 10000000; i++)
            {
                result += Math.Sqrt(i) * Math.Tan(i);
            }
        }

        public static void StringManipulationTask()
        {
            string text = "start";
            for (int i = 0; i < 100000; i++)
            {
                text += i.ToString();
                text = text.Substring(1);
            }
        }
    }
}