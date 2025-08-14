using System;

namespace TestPowerSumNetCore
{
    class TestCalcSum
    {
        private void check()
        {
            long cc = 0;
            for (long i = 0; i < 1000000; i++)
            {
                cc++;
            }
        }

        private double runTest()
        {
            var ticks = DateTime.Now.Ticks;
            check();
            ticks = DateTime.Now.Ticks - ticks;

            TimeSpan ts = TimeSpan.FromTicks(ticks);
            return ts.TotalSeconds;
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            TestCalcSum testPower = new TestCalcSum();
            
            Console.WriteLine("Test calculate sum 1 to 1 000 000\n");

            Console.WriteLine("press any key to start...");
            Console.ReadLine();

            Console.WriteLine($"Time (sec): {testPower.runTest().ToString("0.00000")} \n");

            Console.WriteLine("press any key to exit...\n");
            Console.ReadLine();
        }
    }
}
