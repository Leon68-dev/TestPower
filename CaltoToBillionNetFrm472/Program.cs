using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalctoToBillionNetFrm472
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Start calc to billion (.net framework 4.7.2)");
            var d1 = DateTime.Now;
            Console.WriteLine(d1);

            long clc = 0;
            for (long i = 0; i < 1000000000; i++)
                clc += 1;

            var d2 = DateTime.Now;
            Console.WriteLine(d2);

            TimeSpan timeSpan = d2 - d1;
            Console.WriteLine(timeSpan);

            Console.ReadLine();
        }
    }
}
