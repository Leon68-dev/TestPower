using System;
using System.Security.Cryptography;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

namespace CpuBenchmark
{
    // Атрибути для конфігурації бенчмарку
    [MemoryDiagnoser] // Додає діагностику пам'яті
    [RankColumn]     // Показує ранг (місце) кожного методу
    public class CpuBenchmarks
    {
        private const int N = 10000;
        private readonly byte[] _data;

        public CpuBenchmarks()
        {
            _data = new byte[N];
            new Random(42).NextBytes(_data);
        }

        // Тест на обчислення хешу SHA256
        [Benchmark(Description = "SHA256 Hash")]
        public byte[] Sha256()
        {
            using (var sha256 = SHA256.Create())
            {
                return sha256.ComputeHash(_data);
            }
        }

        // Тест на інтенсивні математичні обчислення
        [Benchmark(Description = "Math Operations")]
        public double MathIntensive()
        {
            double result = 0;
            for (int i = 0; i < N; i++)
            {
                result += Math.Sqrt(i) * Math.Sin(i);
            }
            return result;
        }

        // Тест на сортування масиву
        [Benchmark(Description = "Array Sort")]
        public void SortArray()
        {
            var array = (byte[])_data.Clone();
            Array.Sort(array);
        }
    }

    public class Program
    {
        public static void Main(string[] args)
        {
            // Запуск бенчмарку
            var summary = BenchmarkRunner.Run<CpuBenchmarks>();
        }
    }
}