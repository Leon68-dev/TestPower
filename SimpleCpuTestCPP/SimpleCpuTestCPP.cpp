// SimpleCpuTestCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS // Вимикає попередження про "небезпечні" функції

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

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/utsname.h>
#endif

using namespace std;

class CpuBenchmark
{
private:
    static constexpr long long INT_OPERATIONS = 500'000'000;
    static constexpr long long FLOAT_OPERATIONS = 250'000'000;
    static constexpr long long FUNC_OPERATIONS = 5'000'000;
    static constexpr long long LOGICAL_OPERATIONS = 500'000'000;

    vector<double> allMipsResults;
    chrono::steady_clock::time_point startTime;
    ofstream logFile;

    string getCpuVendor()
    {
        const char* vendor = getenv("PROCESSOR_IDENTIFIER");
        if (vendor)
        {
            return string(vendor);
        }
        return "Unknown_CPU";
    }

    void log(const string& message = "", bool newLine = true)
    {
        cout << message;
        if (newLine)
        {
            cout << endl;
        }

        if (logFile.is_open())
        {
            logFile << message;
            if (newLine)
            {
                logFile << endl;
            }
        }
    }

    template<typename T>
    double runMipsTest(const string& testName, long long operations, T testAction)
    {
        stringstream ss;
        ss << "· " << left << setw(45) << testName;
        log(ss.str(), false);

        auto start = chrono::high_resolution_clock::now();
        testAction();
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        double seconds = elapsed.count();
        double mips = (seconds > 0) ? (operations / seconds) / 1'000'000.0 : 0;

        stringstream res;
        res << ": " << fixed << setprecision(6) << mips << " MIPS";
        log(res.str());

        allMipsResults.push_back(mips);
        return mips;
    }

    template<typename T>
    void runTimeTest(const string& testName, T testAction)
    {
        log("* " + testName);
        auto start = chrono::high_resolution_clock::now();
        testAction();
        auto end = chrono::high_resolution_clock::now();

        auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        log("· Elapsed time: " + to_string(ms) + " Thousands (ms)");
    }

    long double factorial(long long n)
    {
        if (n <= 1)
        {
            return 1;
        }
        return (long double)n * factorial(n - 1);
    }

public:
    void run()
    {
        time_t now = time(nullptr);
        char buf[80];
        struct tm* timeinfo;

        timeinfo = localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", timeinfo);

        string fileName = "Benchmark_" + string(buf) + ".log";
        logFile.open(fileName);

        startTime = chrono::steady_clock::now();

        log("*** BENCHMARK TEST (C++ Version) ***");
        log("> Processor Vendor: " + getCpuVendor());
        log();

        runIntegerTests();
        runFloatingPointTests();
        runFunctionTests();
        runLogicalTests();
        runComplexTests();

        printFooter();
    }

    void runIntegerTests()
    {
        log("* INTEGER ARITHMETIC:");
        vector<double> results;

        volatile int8_t s1 = 10, s2 = 5, sRes;
        results.push_back(runMipsTest("Execute 8 bit Additions", INT_OPERATIONS, [&]()
            {
                for (long long i = 0; i < INT_OPERATIONS; i++)
                {
                    sRes = s1 + s2;
                }
            }));

        results.push_back(runMipsTest("Execute 8 bit Divisions", INT_OPERATIONS, [&]()
            {
                for (long long i = 0; i < INT_OPERATIONS; i++)
                {
                    sRes = s1 / s2;
                }
            }));

        volatile int32_t i1 = 10, i2 = 5, iRes;
        results.push_back(runMipsTest("Execute 32 bit Additions", INT_OPERATIONS, [&]()
            {
                for (long long i = 0; i < INT_OPERATIONS; i++)
                {
                    iRes = i1 + i2;
                }
            }));

        double avg = accumulate(results.begin(), results.end(), 0.0) / results.size();
        log("· Integer Average: " + to_string(avg) + " MIPS\n");
    }

    void runFloatingPointTests()
    {
        log("* FLOATING POINT ARITHMETIC:");
        vector<double> results;

        volatile float f1 = 1.23f, f2 = 4.56f, fRes;
        results.push_back(runMipsTest("Execute 32 bit (Single) Additions", FLOAT_OPERATIONS, [&]()
            {
                for (long long i = 0; i < FLOAT_OPERATIONS; i++)
                {
                    fRes = f1 + f2;
                }
            }));

        volatile double d1 = 1.23, d2 = 4.56, dRes;
        results.push_back(runMipsTest("Execute 64 bit (Double) Multiplications", FLOAT_OPERATIONS, [&]()
            {
                for (long long i = 0; i < FLOAT_OPERATIONS; i++)
                {
                    dRes = d1 * d2;
                }
            }));

        double avg = accumulate(results.begin(), results.end(), 0.0) / results.size();
        log("· Floating Point Average: " + to_string(avg) + " MIPS\n");
    }

    void runFunctionTests()
    {
        log("* ARITHMETIC FUNCTIONS:");
        volatile double num = 1234567890.0, res;

        runMipsTest("Test Square root", FUNC_OPERATIONS, [&]()
            {
                for (long long i = 0; i < FUNC_OPERATIONS; i++)
                {
                    res = sqrt(num);
                }
            });

        runMipsTest("Sinus", FUNC_OPERATIONS, [&]()
            {
                for (long long i = 0; i < FUNC_OPERATIONS; i++)
                {
                    res = sin(0.785);
                }
            });
        log();
    }

    void runLogicalTests()
    {
        log("* LOGICAL INSTRUCTIONS:");
        volatile int v1 = 0xAAAAAAAA, v2 = 0x55555555, vRes;
        runMipsTest("Logical XOR", LOGICAL_OPERATIONS, [&]()
            {
                for (long long i = 0; i < LOGICAL_OPERATIONS; i++)
                {
                    vRes = v1 ^ v2;
                }
            });
        log();
    }

    void runComplexTests()
    {
        log("* COMPLEX TESTS:");

        runMipsTest("Recursive Factorial (17!)", 1, [&]()
            {
                volatile long double f = factorial(17);
            });

        int stringCount = 80000;
        runTimeTest("QUICK SORT - " + to_string(stringCount) + " strings", [&]()
            {
                vector<string> data(stringCount);
                for (int i = 0; i < stringCount; i++)
                {
                    data[i] = "ABCDEFG" + to_string(rand() % 1000);
                }
                sort(data.begin(), data.end());
            });

        int limit = 800000;
        runTimeTest("GENERATE PRIME NUMBERS TO " + to_string(limit), [&]()
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
            });
        log();
    }

    void printFooter()
    {
        auto endTime = chrono::steady_clock::now();
        auto totalMs = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

        double totalAvg = 0;
        if (!allMipsResults.empty())
        {
            totalAvg = accumulate(allMipsResults.begin(), allMipsResults.end(), 0.0) / allMipsResults.size();
        }

        log("*** EXECUTED TESTS AVERAGE: " + to_string(totalAvg) + " MIPS ***");
        log("*** TOTAL ELAPSED TIME: " + to_string(totalMs) + " ms ***");
    }
};

int main()
{
    CpuBenchmark benchmark;
    benchmark.run();
    return 0;
}