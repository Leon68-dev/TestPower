using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestPowerSumNet
{
    internal class Program
    {
        public static void check()
        {
            long cc = 0;
            Console.WriteLine("cc: " + cc.ToString());
            for (long i = 0; i < 1000000; i++)
            {
                cc++;
            }
            Console.WriteLine("cc: " + cc.ToString());
        }

        static void runTest()
        {
            var ticks = DateTime.Now.Ticks;
            check();
            ticks = DateTime.Now.Ticks - ticks;
            Console.WriteLine("time: " + TimeSpan.FromTicks(ticks) + " press any key to exit...\n");
        }

        static void Main(string[] args)
        {
            Console.WriteLine("press any key to start...\n");
            Console.ReadLine();
            runTest();

            Console.ReadLine();
        }
    }
}
