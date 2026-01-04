using Microsoft.Extensions.Logging;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace SimpleCpuTestAndroid;

public partial class MainPage : ContentPage
{
    public MainPage()
    {
        InitializeComponent();
    }

    private async void OnStartClicked(object sender, EventArgs e)
    {
        // Disable button and update text
        StartBtn.IsEnabled = false;
        StartBtn.Text = "Running...";
        LogLabel.Text = "Initializing...\n";

        // Run benchmark in a background thread
        await Task.Run(() =>
        {
            CpuBenchmark.Run((message) =>
            {
                // Update UI on the main thread
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    LogLabel.Text += message + "\n";
                });
            });
        });

        // Restore button state
        StartBtn.Text = "Start Benchmark";
        StartBtn.IsEnabled = true;
    }
}

public static class CpuBenchmark
{
    // Constants (reduced for mobile demo, increase for stress test)
    private const long INT_OPERATIONS = 50_000_000;
    private const long FLOAT_OPERATIONS = 25_000_000;
    private const long FUNC_OPERATIONS = 500_000;
    private const long LOGICAL_OPERATIONS = 50_000_000;

    private static readonly List<double> allMipsResults = new List<double>();
    private static readonly Stopwatch totalTimeStopwatch = new Stopwatch();

    private static StreamWriter logFileWriter;
    private static Action<string> _uiLogger;

    public static void Run(Action<string> uiLogger)
    {
        _uiLogger = uiLogger;
        allMipsResults.Clear();

        string logFileName = GenerateLogFileName();
        string fullPath = Path.Combine(FileSystem.Current.AppDataDirectory, logFileName);

        try
        {
            logFileWriter = new StreamWriter(fullPath, append: false, Encoding.UTF8);

            Log($"Logging results to file: {fullPath}");

            totalTimeStopwatch.Reset();
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
            Log($"An error occurred: {ex.Message}");
        }
        finally
        {
            logFileWriter?.Close();
            logFileWriter = null;
        }
    }

    #region Logging and Helper Methods

    private static void Log(string message = "")
    {
        _uiLogger?.Invoke(message);
        logFileWriter?.WriteLine(message);
    }

    private static string GenerateLogFileName()
    {
        string timeStamp = DateTime.Now.ToString("yyyy-MM-dd_HH-mm-ss");
        return $"Benchmark_{timeStamp}.log";
    }

    private static void PrintHeader()
    {
        Log($"*** BENCHMARK TEST AT {DateTime.Now:HH:mm:ss} ***");
        Log($"> Processor Cores: {Environment.ProcessorCount}");
        Log($"> Architecture: {RuntimeInformation.ProcessArchitecture}");
        Log($"> Runtime: {RuntimeInformation.FrameworkDescription}");
        Log($"> OS: {RuntimeInformation.OSDescription}");
        Log("");
    }

    private static void PrintFooter()
    {
        totalTimeStopwatch.Stop();
        Log($"*** EXECUTED TESTS AVERAGE: {(allMipsResults.Any() ? allMipsResults.Average() : 0):F6} MIPS ***");
        Log("");
        var ts = totalTimeStopwatch.Elapsed;
        Log($"*** ELAPSED TIME: {ts.Hours}h {ts.Minutes}m {ts.Seconds}s {ts.Milliseconds}ms ***");
    }

    private static double RunMipsTest(string testName, long operations, Action testAction)
    {
        string startMsg = $"· {testName}";
        Log(startMsg);

        Stopwatch stopwatch = Stopwatch.StartNew();
        testAction();
        stopwatch.Stop();

        double elapsedSeconds = stopwatch.Elapsed.TotalSeconds;
        double mips = (elapsedSeconds > 0) ? (operations / elapsedSeconds) / 1_000_000.0 : 0;

        Log($"  -> {mips:F6} MIPS ({elapsedSeconds:F4} sec)");
        allMipsResults.Add(mips);
        return mips;
    }

    private static void RunTimeTest(string testName, Action testAction)
    {
        Log($"* {testName}");
        Stopwatch stopwatch = Stopwatch.StartNew();
        testAction();
        stopwatch.Stop();
        Log($"· Elapsed time: {stopwatch.ElapsedMilliseconds} ms");
    }

    private static void PrintSectionHeader(string title)
    {
        Log("");
        Log($"=== {title} ===");
    }

    private static void PrintAverage(string title, IEnumerable<double> results)
    {
        Log($"· {title} Average: {(results.Any() ? results.Average() : 0):F6} MIPS.");
    }
    #endregion

    #region Test Implementations

    private static void RunIntegerTests()
    {
        PrintSectionHeader("INTEGER ARITHMETIC");
        var results = new List<double>();

        sbyte sbyteVal1 = 10, sbyteVal2 = 5; sbyte sbyteRes = 0;

        results.Add(RunMipsTest("8 bit Add", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 + sbyteVal2); }));
        results.Add(RunMipsTest("8 bit Sub", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 - sbyteVal2); }));
        results.Add(RunMipsTest("8 bit Mul", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 * sbyteVal2); }));
        results.Add(RunMipsTest("8 bit Div", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) sbyteRes = (sbyte)(sbyteVal1 / sbyteVal2); }));

        PrintAverage("8 bit Average", results);
        var overallIntResults = new List<double>(results);
        results.Clear();

        int intVal1 = 10, intVal2 = 5; int intRes = 0;

        results.Add(RunMipsTest("32 bit Add", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 + intVal2; }));
        results.Add(RunMipsTest("32 bit Sub", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 - intVal2; }));
        results.Add(RunMipsTest("32 bit Mul", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 * intVal2; }));
        results.Add(RunMipsTest("32 bit Div", INT_OPERATIONS, () => { for (long i = 0; i < INT_OPERATIONS; i++) intRes = intVal1 / intVal2; }));

        PrintAverage("32 bit Average", results);
        overallIntResults.AddRange(results);

        PrintAverage("INTEGER TOTAL AVERAGE", overallIntResults);
    }

    private static void RunFloatingPointTests()
    {
        PrintSectionHeader("FLOATING POINT ARITHMETIC");
        var results = new List<double>();

        float floatVal1 = 1.23f, floatVal2 = 4.56f; float floatRes = 0;

        results.Add(RunMipsTest("32 bit (Single) Add", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) floatRes = floatVal1 + floatVal2; }));
        results.Add(RunMipsTest("32 bit (Single) Mul", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) floatRes = floatVal1 * floatVal2; }));

        PrintAverage("32 bit Float Average", results);
        var overallFpResults = new List<double>(results);
        results.Clear();

        double doubleVal1 = 1.23, doubleVal2 = 4.56; double doubleRes = 0;
        results.Add(RunMipsTest("64 bit (Double) Add", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) doubleRes = doubleVal1 + doubleVal2; }));
        results.Add(RunMipsTest("64 bit (Double) Div", FLOAT_OPERATIONS, () => { for (long i = 0; i < FLOAT_OPERATIONS; i++) doubleRes = doubleVal1 / doubleVal2; }));

        PrintAverage("64 bit Double Average", results);
        overallFpResults.AddRange(results);

        PrintAverage("FLOATING POINT TOTAL", overallFpResults);
    }

    private static void RunFunctionTests()
    {
        PrintSectionHeader("MATH FUNCTIONS");
        var results = new List<double>();
        double num = 1234567890.0; double res = 0;

        results.Add(RunMipsTest("Math.Sqrt", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Sqrt(num); }));
        results.Add(RunMipsTest("Math.Sin", FUNC_OPERATIONS, () => { for (long i = 0; i < FUNC_OPERATIONS; i++) res = Math.Sin(0.785); }));

        PrintAverage("MATH FUNCTIONS AVERAGE", results);
    }

    private static void RunLogicalTests()
    {
        PrintSectionHeader("LOGICAL OPS");
        var results = new List<double>();
        int val1 = unchecked((int)0xAAAAAAAA), val2 = 0x55555555; int res = 0;

        results.Add(RunMipsTest("Logical AND", LOGICAL_OPERATIONS, () => { for (long i = 0; i < LOGICAL_OPERATIONS; i++) res = val1 & val2; }));

        PrintAverage("LOGICAL AVERAGE", results);
    }

    private static void RunRecursiveFactorialTest()
    {
        PrintSectionHeader("RECURSION");
        long n = 15;
        decimal result = 0;
        RunMipsTest($"Factorial({n})", 1, () => { result = Factorial(n); });
        Log($"Result: {result}");
    }

    private static decimal Factorial(long n)
    {
        if (n <= 1) return 1;
        return n * Factorial(n - 1);
    }

    private static void RunQuickSortTest()
    {
        PrintSectionHeader("SORTING");
        int stringCount = 20000;
        int stringLength = 20;

        RunTimeTest($"QuickSort {stringCount} strings", () =>
        {
            string[] data = new string[stringCount];
            Random rand = new Random();
            for (int i = 0; i < stringCount; i++)
            {
                StringBuilder sb = new StringBuilder(stringLength);
                for (int j = 0; j < stringLength; j++) sb.Append((char)rand.Next(65, 90));
                data[i] = sb.ToString();
            }
            Array.Sort(data);
        });
    }

    private static void RunPrimeGenerationTest()
    {
        PrintSectionHeader("PRIMES");
        int limit = 200000;
        int primeCount = 0;

        RunTimeTest($"Sieve of Eratosthenes to {limit}", () =>
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
            for (int i = 2; i <= limit; i++) if (isPrime[i]) primeCount++;
        });
        Log($"Found {primeCount} primes");
    }

    #endregion
}