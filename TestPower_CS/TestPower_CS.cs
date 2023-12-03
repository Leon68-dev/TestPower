using System;

namespace TestPower
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
    class TestPower_CS
	{
		public const int size = 2048;
		public static double check()
		{
			double[] x, y, z;
			x = new double[size];
			y = new double[size];
			z = new double[size];
			double a = 0, b = -100, c = 100;
			for ( int times = 0; times < 1000; times++ )
			{
				for ( int i = 0; i < size; i++ )
				{
					x[i] = times * i;
					y[i] = Environment.TickCount / (x[i]+1);
					z[i] = (x[i] + y[i] / 2) + 4;
				};
				for ( int i = 0; i < size; i++ )
				{
					a = x[i] + a;
					b = y[i] + b + 1;
					c = z[i] + Math.Sin( ( x[i] + z[i] ) / (a + 1) );
				};
			};
			return a + b + c;
		}

		static void runTest()
		{
            int time = Environment.TickCount;
            double val = check();
            time = Environment.TickCount - time;
            //Console.WriteLine( "time: " + time + " value = " + val + ". press any key to exit...\n" );
            Console.WriteLine("time: " + time + " press any key to exit...\n");
        }


        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
		static void Main(string[] args)
		{
			//
			// TODO: Add code to start application here
			//
			Console.WriteLine( "press any key to start...\n" );
			Console.ReadLine();
            runTest();

            Console.ReadLine();
		}
	}
}
