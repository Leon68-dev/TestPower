#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <sstream>
#include <thread>

using namespace std;

class CpuBenchmark
{
private:
    static constexpr long long INT_OPERATIONS = 500'000'000;
    static constexpr long long FLOAT_OPERATIONS = 250'000'000;
    static constexpr long long FUNC_OPERATIONS = 5'000'000;
    static constexpr long long LOGICAL_OPERATIONS = 500'000'000;

    vector<double> allMipsResults;
    chrono::steady_clock::time_point totalStartTime;
    ofstream logFileWriter;

    string getCpuVendor()
    {
        const char* vendor = getenv("PROCESSOR_IDENTIFIER");
        if (vendor)
        {
            return string(vendor);
        }
        return "Unknown_CPU";
    }

    string generateLogFileName()
    {
        time_t now = time(nullptr);
        struct tm tstruct = *localtime(&now);
        char timeBuf[80];
        strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d_%H-%M-%S", &tstruct);

        string cpuInfo = getCpuVendor();
        for (char& c : cpuInfo)
        {
            if (c == ':' || c == '/' || c == '\\' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|')
            {
                c = '_';
            }
        }

        return "Benchmark_" + string(timeBuf) + "_" + cpuInfo + ".log";
    }

    // Перейменовано на Log (з великої літери), щоб не було конфлікту з математичним log()
    void Log(const string& message = "", bool newLine = true)
    {
        cout << message;
        if (newLine)
        {
            cout << endl;
        }

        if (logFileWriter.is_open())
        {
            logFileWriter << message;
            if (newLine)
            {
                logFileWriter << endl;
            }
        }
    }

    void LogInline(const string& message)
    {
        Log(message, false);
    }

    template<typename T>
    double runMipsTest(const string& testName, long long operations, T testAction)
    {
        stringstream ss;
        ss << "· " << left << setw(45) << testName;
        LogInline(ss.str());

        auto start = chrono::high_resolution_clock::now();
        testAction();
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        double seconds = elapsed.count();
        double mips = (seconds > 0) ? (operations / seconds) / 1'000'000.0 : 0;

        stringstream resSS;
        resSS << ": " << fixed << setprecision(6) << mips << " MIPS";
        Log(resSS.str());

        allMipsResults.push_back(mips);
        return mips;
    }

    template<typename T>
    void runTimeTest(const string& testName, T testAction)
    {
        Log("* " + testName);
        auto start = chrono::high_resolution_clock::now();
        testAction();
        auto end = chrono::high_resolution_clock::now();

        auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        Log("· Ellapsed time: " + to_string((long long)ms) + " Thousands");
    }

    long double factorial(long long n)
    {
        if (n <= 1)
        {
            return 1;
        }
        return (long double)n * factorial(n - 1);
    }

    void printAverage(const string& title, const vector<double>& results)
    {
        double avg = 0;
        if (!results.empty())
        {
            avg = accumulate(results.begin(), results.end(), 0.0) / results.size();
        }

        stringstream ss;
        ss << fixed << setprecision(6) << "· " << title << " Average: " << avg << " MIPS.";
        Log(ss.str());
        Log();
    }

public:
    void run()
    {
        string fileName = generateLogFileName();
        logFileWriter.open(fileName);

        totalStartTime = chrono::steady_clock::now();

        time_t now = time(nullptr);
        struct tm tstruct = *localtime(&now);
        char timeBuf[80];
        strftime(timeBuf, sizeof(timeBuf), "%H 'Hour' %M 'Min' %S 'Sec'", &tstruct);

        Log("*** BENCHMARK TEST AT " + string(timeBuf) + " 000 Thousands ***");
        Log();

        string coreCount = to_string((long long)thread::hardware_concurrency());
        Log("> Processor: " + coreCount + " Cores. Vendor: " + getCpuVendor());
        Log("> Runtime: C++ Native (MSVC)");
        Log();

        runIntegerTests();
        runFloatingPointTests();
        runFunctionTests();
        runLogicalTests();

        Log("* RECURSIVE FACTORIAL CALCULATION:");
        runMipsTest("17 Factorial is 355687428096000", 1, [&]()
            {
                volatile long double f = factorial(17);
            });
        Log();

        runComplexTests();

        printFooter();
    }

    void runIntegerTests()
    {
        Log("* INTEGER ARITHMETIC:");
        vector<double> r8, r16, r32, overall;

        volatile int8_t s1 = 10, s2 = 5, sRes;
        r8.push_back(runMipsTest("Execute 8 bit Additions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) sRes = s1 + s2; }));
        r8.push_back(runMipsTest("Execute 8 bit Substractions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) sRes = s1 - s2; }));
        r8.push_back(runMipsTest("Execute 8 bit Multiplications", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) sRes = s1 * s2; }));
        r8.push_back(runMipsTest("Execute 8 bit Divisions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) sRes = s1 / s2; }));
        printAverage("8 bit (Short Inteder) arithmetic operations", r8);

        volatile int16_t h1 = 10, h2 = 5, hRes;
        r16.push_back(runMipsTest("Execute 16 bit Additions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) hRes = h1 + h2; }));
        r16.push_back(runMipsTest("Execute 16 bit Substractions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) hRes = h1 - h2; }));
        r16.push_back(runMipsTest("Execute 16 bit Multiplications", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) hRes = h1 * h2; }));
        r16.push_back(runMipsTest("Execute 16 bit Divisions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) hRes = h1 / h2; }));
        printAverage("16 bit (Small Inteder) arithmetic operations", r16);

        volatile int32_t i1 = 10, i2 = 5, iRes;
        r32.push_back(runMipsTest("Execute 32 bit Additions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) iRes = i1 + i2; }));
        r32.push_back(runMipsTest("Execute 32 bit Substractions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) iRes = i1 - i2; }));
        r32.push_back(runMipsTest("Execute 32 bit Multiplications", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) iRes = i1 * i2; }));
        r32.push_back(runMipsTest("Execute 32 bit Divisions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; i++) iRes = i1 / i2; }));
        printAverage("32 bit (Long Inteder) arithmetic operations", r32);

        overall.insert(overall.end(), r8.begin(), r8.end());
        overall.insert(overall.end(), r16.begin(), r16.end());
        overall.insert(overall.end(), r32.begin(), r32.end());
        printAverage("INTEGER ARITHMETIC:", overall);
    }

    void runFloatingPointTests()
    {
        Log("* FLOATING POINT ARITHMETIC:");
        vector<double> r32, r64, r128, overall;

        volatile float f1 = 1.23f, f2 = 4.56f, fRes;
        r32.push_back(runMipsTest("Execute 32 bit (Single) Additions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) fRes = f1 + f2; }));
        r32.push_back(runMipsTest("Execute 32 bit (Single) Substractions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) fRes = f1 - f2; }));
        r32.push_back(runMipsTest("Execute 32 bit (Single) Multiplications", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) fRes = f1 * f2; }));
        r32.push_back(runMipsTest("Execute 32 bit (Single) Divisions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) fRes = f1 / f2; }));
        printAverage("32 bit (Single) arithmetic operations", r32);

        volatile double d1 = 1.23, d2 = 4.56, dRes;
        r64.push_back(runMipsTest("Execute 64 bit (Double) Additions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) dRes = d1 + d2; }));
        r64.push_back(runMipsTest("Execute 64 bit (Double) Substractions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) dRes = d1 - d2; }));
        r64.push_back(runMipsTest("Execute 64 bit (Double) Multiplications", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) dRes = d1 * d2; }));
        r64.push_back(runMipsTest("Execute 64 bit (Double) Divisions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; i++) dRes = d1 / d2; }));
        printAverage("64 bit (Double) arithmetic operations", r64);

        volatile long double ld1 = 1.23L, ld2 = 4.56L, ldRes;
        long long ldOps = FLOAT_OPERATIONS / 10;
        r128.push_back(runMipsTest("Execute 128 bit (Extended) Additions", ldOps, [&]() { for (long long i = 0; i < ldOps; i++) ldRes = ld1 + ld2; }));
        r128.push_back(runMipsTest("Execute 128 bit (Extended) Substractions", ldOps, [&]() { for (long long i = 0; i < ldOps; i++) ldRes = ld1 - ld2; }));
        r128.push_back(runMipsTest("Execute 128 bit (Extended) Multiplications", ldOps, [&]() { for (long long i = 0; i < ldOps; i++) ldRes = ld1 * ld2; }));
        r128.push_back(runMipsTest("Execute 128 bit (Extended) Divisions", ldOps, [&]() { for (long long i = 0; i < ldOps; i++) ldRes = ld1 / ld2; }));
        printAverage("128 bit (Extended) arithmetic operations", r128);

        overall.insert(overall.end(), r32.begin(), r32.end());
        overall.insert(overall.end(), r64.begin(), r64.end());
        overall.insert(overall.end(), r128.begin(), r128.end());
        printAverage("FLOATING POINT ARITHMETIC:", overall);
    }

    void runFunctionTests()
    {
        Log("* SQR & SQRT ARITHMETIC FUNCTIONS:");
        vector<double> results;
        volatile double num = 1234567890.0, dummyRes;
        results.push_back(runMipsTest("Test Square of 1234567890", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = num * num; }));
        results.push_back(runMipsTest("Test Square root of 1234567890", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = sqrt(num); }));
        printAverage("SQR & SQRT ARITHMETIC FUNCTIONS", results);

        Log("* TRIGONOMETRIC FUNCTIONS:");
        results.clear();
        results.push_back(runMipsTest("Sinus", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = sin(0.785); }));
        results.push_back(runMipsTest("Cosinus", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = cos(0.785); }));
        results.push_back(runMipsTest("Tangent", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = tan(0.785); }));
        results.push_back(runMipsTest("Arcus Tangent", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = atan(0.785); }));
        printAverage("TRIGONOMETRIC FUNCTIONS", results);

        Log("* LOGARITHMIC FUNCTIONS:");
        results.clear();
        // Тепер тут викликається математичний log(), бо функція класу називається Log
        results.push_back(runMipsTest("Natural Logarithm", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = log(100.0); }));
        results.push_back(runMipsTest("Exponent", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; i++) dummyRes = exp(10.0); }));
        printAverage("LOGARITHMIC FUNCTIONS", results);
    }

    void runLogicalTests()
    {
        Log("* LOGICAL INSTRUCTIONS:");
        vector<double> results;
        volatile int32_t v1 = 0xAAAAAAAA, v2 = 0x55555555, vRes;
        results.push_back(runMipsTest("Logical AND", LOGICAL_OPERATIONS, [&]() { for (long long i = 0; i < LOGICAL_OPERATIONS; i++) vRes = v1 & v2; }));
        results.push_back(runMipsTest("Logical OR", LOGICAL_OPERATIONS, [&]() { for (long long i = 0; i < LOGICAL_OPERATIONS; i++) vRes = v1 | v2; }));
        results.push_back(runMipsTest("Logical XOR", LOGICAL_OPERATIONS, [&]() { for (long long i = 0; i < LOGICAL_OPERATIONS; i++) vRes = v1 ^ v2; }));
        results.push_back(runMipsTest("Logical NOT", LOGICAL_OPERATIONS, [&]() { for (long long i = 0; i < LOGICAL_OPERATIONS; i++) vRes = ~v1; }));
        printAverage("LOGICAL INSTRUCTIONS", results);
    }

    void runComplexTests()
    {
        int stringCount = 80000;
        runTimeTest("QUICK SORT - " + to_string((long long)stringCount) + " strings with 80 length", [&]()
            {
                vector<string> data(stringCount);
                for (int i = 0; i < stringCount; i++)
                {
                    data[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ012345678901234567890123456789012345678901234567890123";
                }
                sort(data.begin(), data.end());
            });
        Log();

        int limit = 800000;
        int primeCount = 0;
        runTimeTest("GENERATE PRIME NUMBERS TO " + to_string((long long)limit), [&]()
            {
                vector<bool> isPrime(limit + 1, true);
                for (int p = 2; p * p <= limit; p++)
                {
                    if (isPrime[p])
                    {
                        for (int i = p * p; i <= limit; i += p)
                        {
                            isPrime[i] = false;
                        }
                    }
                }
                for (int i = 2; i <= limit; i++)
                {
                    if (isPrime[i]) primeCount++;
                }
            });
        Log("· " + to_string((long long)primeCount) + " prime generated");
        Log();
    }

    void printFooter()
    {
        auto endTime = chrono::steady_clock::now();
        auto totalDuration = chrono::duration_cast<chrono::milliseconds>(endTime - totalStartTime);

        double totalAvg = 0;
        if (!allMipsResults.empty())
        {
            totalAvg = accumulate(allMipsResults.begin(), allMipsResults.end(), 0.0) / allMipsResults.size();
        }

        stringstream ssFinal;
        ssFinal << fixed << setprecision(6) << "*** EXECUTED TESTS AVERAGE: " << totalAvg << " MIPS ***";
        Log(ssFinal.str());
        Log();

        time_t now = time(nullptr);
        struct tm tstruct = *localtime(&now);
        char timeBuf[80];
        strftime(timeBuf, sizeof(timeBuf), "%d.%m.%Y %H:%M:%S", &tstruct);
        Log("*** TESTS FINISHED AT " + string(timeBuf) + " ***");
        Log();

        long long ms = totalDuration.count();
        long long sec = (ms / 1000) % 60;
        long long min = (ms / (1000 * 60)) % 60;
        long long hour = (ms / (1000 * 60 * 60));

        Log("*** ELAPSED TIME DURING TEST IS " + to_string(hour) + " Hour " + to_string(min) + " Min " + to_string(sec) + " Sec " + to_string(ms % 1000) + " Thousands ***");
    }
};

int main()
{
    CpuBenchmark benchmark;
    benchmark.run();
    return 0;
}