using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace SimpleCpuTest
{
    public class Program
    {
        public static void Main(string[] args)
        {
            // Запускаємо сам бенчмарк
            CpuBenchmark.Run();
        }
    }

    /// <summary>
    /// Клас, що містить всю логіку для тестування продуктивності процесора.
    /// </summary>
    public static class CpuBenchmark
    {
        // Кількість операцій для різних типів тестів
        private const long INT_OPERATIONS = 500_000_000;
        private const long FLOAT_OPERATIONS = 250_000_000;
        private const long FUNC_OPERATIONS = 5_000_000;
        private const long LOGICAL_OPERATIONS = 500_000_000;

        private static readonly List<double> allMipsResults = new List<double>();
        private static readonly Stopwatch totalTimeStopwatch = new Stopwatch();

        // Поле для запису у файл
        private static StreamWriter logFileWriter;

        public static void Run()
        {
            string logFileName = GenerateLogFileName();
            try
            {
                logFileWriter = new StreamWriter(logFileName, append: false, Encoding.UTF8);

                Console.OutputEncoding = Encoding.UTF8;
                //Log($"Logging results to file: {Path.GetFullPath(logFileName)}\n");

                totalTimeStopwatch.Start();

                PrintHeader();

                RunIntegerTests();
                RunFloatingPointTests();
                RunFunctionTests();
                RunLogicalTests();
                RunRecursiveFactorialTest();
                RunQuickSortTest();
                RunPrimeGenerationTest();

                PrintFooter();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
            }
            finally
            {
                logFileWriter?.Close();
            }
        }

        #region Logging and Helper Methods

        private static void Log(string message = "")
        {
            Console.WriteLine(message);
            logFileWriter?.WriteLine(message);
        }

        private static void LogInline(string message)
        {
            Console.Write(message);
            logFileWriter?.Write(message);
        }

        private static string GenerateLogFileName()
        {
            string timeStamp = DateTime.Now.ToString("yyyy-MM-dd_HH-mm-ss");
            string cpuInfo = GetCpuVendor();

            foreach (char c in Path.GetInvalidFileNameChars())
                cpuInfo = cpuInfo.Replace(c, '_');

            return $"Benchmark_{timeStamp}_{cpuInfo}.log";
        }


        private static void PrintHeader()
        {
            Log($"*** BENCHMARK TEST AT {DateTime.Now:HH 'Hour' mm 'Min' ss 'Sec' fff 'Thousands'} ***");
            Log();
            Log($"> Processor: {Environment.ProcessorCount} Cores, {RuntimeInformation.ProcessArchitecture}. Vendor: {GetCpuVendor()}");
            Log($"> Runtime: {RuntimeInformation.FrameworkDescription}");
            Log();
        }

        private static void PrintFooter()
        {
            totalTimeStopwatch.Stop();
            Log($"*** EXECUTED TESTS AVERAGE: {(allMipsResults.Any() ? allMipsResults.Average() : 0):F6} MIPS ***");
            Log();
            Log($"*** TESTS FINISHED AT {DateTime.Now:dd/MM/yyyy HH:mm:ss} ***");
            Log();
            var ts = totalTimeStopwatch.Elapsed;
            Log($"*** ELAPSED TIME DURING TEST IS {ts.Hours} Hour {ts.Minutes} Min {ts.Seconds} Sec {ts.Milliseconds} Thousands ***");
        }

        private static double RunMipsTest(string testName, long operations, Action testAction)
        {
            LogInline($"· {testName,-45}");
            Stopwatch stopwatch = Stopwatch.StartNew();
            testAction();
            stopwatch.Stop();

            double elapsedSeconds = stopwatch.Elapsed.TotalSeconds;
            double mips = (elapsedSeconds > 0) ? (operations / elapsedSeconds) / 1_000_000.0 : 0;

            Log($": {mips:F6} MIPS");
            allMipsResults.Add(mips);
            return mips;
        }

        private static void RunTimeTest(string testName, Action testAction)
        {
            Log($"* {testName}");
            Stopwatch stopwatch = Stopwatch.StartNew();
            testAction();
            stopwatch.Stop();
            Log($"· Ellapsed time: {stopwatch.ElapsedMilliseconds} Thousands");
        }

        private static void PrintSectionHeader(string title)
        {
            Log($"* {title}:");
        }

        private static void PrintAverage(string title, IEnumerable<double> results)
        {
            Log($"· {title} Average: {(results.Any() ? results.Average() : 0):F6} MIPS.");
            Log();
        }

        private static string GetCpuVendor()
        {
            var identifier = Environment.GetEnvironmentVariable("PROCESSOR_IDENTIFIER");
            if (!string.IsNullOrEmpty(identifier))
                return identifier.Split(',')[0].Trim();
            return "UnknownCPU";
        }

        #endregion

        #region Test Implementations

        private static void RunIntegerTests()
        {
            PrintSectionHeader("INTEGER ARITHMETIC");
            var results = new List<double>();

            sbyte sbyteVal1 = 10, sbyteVal2 = 5; sbyte sbyteRes = 0;
            results.Add(RunMipsTest("Execute 8 bit Additions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 + sbyteVal2); }));
            results.Add(RunMipsTest("Execute 8 bit Substractions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 - sbyteVal2); }));
            results.Add(RunMipsTest("Execute 8 bit Multiplications", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 * sbyteVal2); }));
            results.Add(RunMipsTest("Execute 8 bit Divisions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 / sbyteVal2); }));
            PrintAverage("8 bit (Short Inteder) arithmetic operations", results);
            var overallIntResults = new List<double>(results);
            results.Clear();

            short shortVal1 = 10, shortVal2 = 5; short shortRes = 0;
            results.Add(RunMipsTest("Execute 16 bit Additions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) shortRes = (short)(shortVal1 + shortVal2); }));
            results.Add(RunMipsTest("Execute 16 bit Substractions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) shortRes = (short)(shortVal1 - shortVal2); }));
            results.Add(RunMipsTest("Execute 16 bit Multiplications", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) shortRes = (short)(shortVal1 * shortVal2); }));
            results.Add(RunMipsTest("Execute 16 bit Divisions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) shortRes = (short)(shortVal1 / shortVal2); }));
            PrintAverage("16 bit (Small Inteder) arithmetic operations", results);
            overallIntResults.AddRange(results);
            results.Clear();

            int intVal1 = 10, intVal2 = 5; int intRes = 0;
            results.Add(RunMipsTest("Execute 32 bit Additions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 + intVal2; }));
            results.Add(RunMipsTest("Execute 32 bit Substractions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 - intVal2; }));
            results.Add(RunMipsTest("Execute 32 bit Multiplications", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 * intVal2; }));
            results.Add(RunMipsTest("Execute 32 bit Divisions", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 / intVal2; }));
            PrintAverage("32 bit (Long Inteder) arithmetic operations", results);
            overallIntResults.AddRange(results);

            PrintAverage("INTEGER ARITHMETIC: AVERAGE", overallIntResults);
        }

        private static void RunFloatingPointTests()
        {
            PrintSectionHeader("FLOATING POINT ARITHMETIC");
            var results = new List<double>();

            float floatVal1 = 1.23f, floatVal2 = 4.56f; float floatRes = 0;
            results.Add(RunMipsTest("Execute 32 bit (Single) Additions", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) floatRes = floatVal1 + floatVal2; }));
            results.Add(RunMipsTest("Execute 32 bit (Single) Substractions", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) floatRes = floatVal1 - floatVal2; }));
            results.Add(RunMipsTest("Execute 32 bit (Single) Multiplications", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) floatRes = floatVal1 * floatVal2; }));
            results.Add(RunMipsTest("Execute 32 bit (Single) Divisions", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) floatRes = floatVal1 / floatVal2; }));
            PrintAverage("32 bit (Single) arithmetic operations", results);
            var overallFpResults = new List<double>(results);
            results.Clear();

            double doubleVal1 = 1.23, doubleVal2 = 4.56; double doubleRes = 0;
            results.Add(RunMipsTest("Execute 64 bit (Double) Additions", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) doubleRes = doubleVal1 + doubleVal2; }));
            results.Add(RunMipsTest("Execute 64 bit (Double) Substractions", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) doubleRes = doubleVal1 - doubleVal2; }));
            results.Add(RunMipsTest("Execute 64 bit (Double) Multiplications", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) doubleRes = doubleVal1 * doubleVal2; }));
            results.Add(RunMipsTest("Execute 64 bit (Double) Divisions", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) doubleRes = doubleVal1 / doubleVal2; }));
            PrintAverage("64 bit (Double) arithmetic operations", results);
            overallFpResults.AddRange(results);
            results.Clear();

            decimal decimalVal1 = 1.23m, decimalVal2 = 4.56m; decimal decimalRes = 0;
            results.Add(RunMipsTest("Execute 128 bit (Extended) Additions", FLOAT_OPERATIONS / 10, () => { for (long i = 0; i < FLOAT_OPERATIONS / 10; i++) decimalRes = decimalVal1 + decimalVal2; }));
            results.Add(RunMipsTest("Execute 128 bit (Extended) Substractions", FLOAT_OPERATIONS / 10, () => { for (long i = 0; i < FLOAT_OPERATIONS / 10; i++) decimalRes = decimalVal1 - decimalVal2; }));
            results.Add(RunMipsTest("Execute 128 bit (Extended) Multiplications", FLOAT_OPERATIONS / 10, () => { for (long i = 0; i < FLOAT_OPERATIONS / 10; i++) decimalRes = decimalVal1 * decimalVal2; }));
            results.Add(RunMipsTest("Execute 128 bit (Extended) Divisions", FLOAT_OPERATIONS / 10, () => { for (long i = 0; i < FLOAT_OPERATIONS / 10; i++) decimalRes = decimalVal1 / decimalVal2; }));
            PrintAverage("128 bit (Extended) arithmetic operations", results);
            overallFpResults.AddRange(results);

            PrintAverage("FLOATING POINT ARITHMETIC:", overallFpResults);
        }

        private static void RunFunctionTests()
        {
            PrintSectionHeader("SQR & SQRT ARITHMETIC FUNCTIONS");
            var results = new List<double>();
            double num = 1234567890.0; double res = 0;
            results.Add(RunMipsTest("Test Square of 1234567890", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = num * num; }));
            results.Add(RunMipsTest("Test Square root of 1234567890", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Sqrt(num); }));
            PrintAverage("SQR & SQRT ARITHMETIC FUNCTIONS AVERAGE", results);

            PrintSectionHeader("TRIGONOMETRIC FUNCTIONS");
            results.Clear();
            double angle = 0.785; // 45 degrees in radians
            results.Add(RunMipsTest("Sinus", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Sin(angle); }));
            results.Add(RunMipsTest("Cosinus", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Cos(angle); }));
            results.Add(RunMipsTest("Tangent", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Tan(angle); }));
            results.Add(RunMipsTest("Arcus Tangent", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Atan(angle); }));
            PrintAverage("TRIGONOMETRIC FUNCTIONS AVERAGE", results);

            PrintSectionHeader("LOGARITHMIC FUNCTIONS");
            results.Clear();
            double logVal = 100.0;
            results.Add(RunMipsTest("Natural Logarithm", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Log(logVal); }));
            results.Add(RunMipsTest("Exponent", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Exp(logVal); }));
            PrintAverage("LOGARITHMIC FUNCTIONS AVERAGE", results);
        }

        private static void RunLogicalTests()
        {
            PrintSectionHeader("LOGICAL INSTRUCTIONS");
            var results = new List<double>();
            int val1 = unchecked((int)0xAAAAAAAA), val2 = 0x55555555; int res = 0;

            results.Add(RunMipsTest("Logical And Instruction", LOGICAL_OPERATIONS, () => { for (long i = 0; i < LOGICAL_OPERATIONS; i++) res = val1 & val2; }));
            results.Add(RunMipsTest("Logical Or Instruction", LOGICAL_OPERATIONS, () => { for (long i = 0; i < LOGICAL_OPERATIONS; i++) res = val1 | val2; }));
            results.Add(RunMipsTest("Logical Xor Instruction", LOGICAL_OPERATIONS, () => { for (long i = 0; i < LOGICAL_OPERATIONS; i++) res = val1 ^ val2; }));
            results.Add(RunMipsTest("Logical Not Instruction", LOGICAL_OPERATIONS, () => { for (long i = 0; i < LOGICAL_OPERATIONS; i++) res = ~val1; }));
            PrintAverage("LOGICAL INSTRUCTIONS AVERAGE", results);
        }

        private static void RunRecursiveFactorialTest()
        {
            PrintSectionHeader("RECURSIVE FACTORIAL CALCULATION");
            long n = 17;
            decimal result = 0;
            RunMipsTest($"17 Factorial is 355687428096000", 1, () => { result = Factorial(n); });
            if (result == 0) Log("Factorial error");
            Log();
        }

        private static decimal Factorial(long n)
        {
            if (n <= 1) return 1;
            return n * Factorial(n - 1);
        }

        private static void RunQuickSortTest()
        {
            int stringCount = 70000;
            int stringLength = 70;

            RunTimeTest($"QUICK SORT - {stringCount} strings with {stringLength} length", () =>
            {
                string[] data = new string[stringCount];
                Random rand = new Random();
                for (int i = 0; i < stringCount; i++)
                {
                    StringBuilder sb = new StringBuilder(stringLength);
                    for (int j = 0; j < stringLength; j++)
                    {
                        sb.Append((char)rand.Next(65, 90));
                    }
                    data[i] = sb.ToString();
                }
                Array.Sort(data);
            });
            Log();
        }

        private static void RunPrimeGenerationTest()
        {
            int limit = 700000;
            int primeCount = 0;

            RunTimeTest($"GENERATE PRIME NUMBERS TO {limit}", () =>
            {
                bool[] isPrime = new bool[limit + 1];
                for (int i = 2; i <= limit; i++) isPrime[i] = true;
                for (int p = 2; p * p <= limit; p++)
                {
                    if (isPrime[p])
                    {
                        for (int i = p * p; i <= limit; i += p)
                            isPrime[i] = false;
                    }
                }
                for (int i = 2; i <= limit; i++)
                {
                    if (isPrime[i]) primeCount++;
                }
            });
            Log($"· {primeCount} prime generated");
            Log();
        }

        #endregion
    }
}