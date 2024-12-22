// See https://aka.ms/new-console-template for more information
using System.Runtime.InteropServices;

class TestPowerSumNetCore
{
    public static void check()
    {
        long cc = 0;
        //Console.WriteLine("cc: " + cc.ToString());  
        for (long i = 0; i < 1000000; i++)
        {
            cc++;
        }
        //Console.WriteLine("cc: " + cc.ToString());
    }

    static void runTest()
    {
        check();
        var ticks = DateTime.Now.Ticks;
        check();
        ticks = DateTime.Now.Ticks - ticks;
        Console.WriteLine("time: " + TimeSpan.FromTicks(ticks) + " press any key to exit...\n");
    }

    [STAThread]
    static void Main(string[] args)
    {
        Console.WriteLine("press any key to start...\n");
        Console.ReadLine();
        runTest();

        Console.ReadLine();
    }

}
