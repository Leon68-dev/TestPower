// See https://aka.ms/new-console-template for more information
Console.WriteLine("Start calc to billion (.net 8.0)");
var d1 = DateTime.Now;
Console.WriteLine(d1);

long clc = 0;
for (long i = 0; i < 1000000000; i++)
    clc += 1;

var d2 = DateTime.Now;
Console.WriteLine(d2);

TimeSpan timeSpan = d2-d1;
Console.WriteLine(timeSpan);

Console.ReadLine();
