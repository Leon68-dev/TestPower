using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalcToBillionNetFrm472
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var prm = 1;
            if (args.Length >= 1 && !string.IsNullOrEmpty(args[0]))
                prm = int.Parse(args[0]);

            TestCalcToBillion testCalcToBillion = new TestCalcToBillion();

            Console.WriteLine("Start calc to billion (.net framework 4.7.2)");

            for (int i = 0; i < prm; i++)
                testCalcToBillion.runTest();

            Console.WriteLine("Finish calc to billion (.net framework 4.7.2)");
            Console.ReadLine();
        }

    }

    public class TestCalcToBillion
    {
        public void runTest()
        {
            var d1 = DateTime.Now;
            Console.WriteLine(d1);

            long clc = 0;
            for (long i = 0; i < 1000000000; i++)
                clc += 1;

            var d2 = DateTime.Now;
            TimeSpan timeSpan = d2 - d1;

            Console.WriteLine(d2);
            
            Console.WriteLine("================================");
            Console.WriteLine(timeSpan);
            Console.WriteLine("================================");
        }
    }

}
