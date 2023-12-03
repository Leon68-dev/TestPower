// TestPower_VC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include "windows.h"

extern double check()
{
	enum { size = 2048 };
	double x[size], y[size], z[size];
	double a = 0, b = -100, c = 100;
			
	for ( int times = 0; times < 1000; times++ )
	{
		for ( int i = 0; i < size; i++ )
		{
			x[i] = times * i;
			y[i] = GetTickCount() / (x[i]+1);
			z[i] = (x[i] + y[i] / 2) + 4;
		};

		for ( int i = 0; i < size; i++ )
		{
			a = x[i] + a;
			b = y[i] + b + 1;
			c = z[i] + sin( double( x[i] + z[i] ) / (a + 1) );
		};
	};
	return a + b + c;
};

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "press any key to start...\n" );
	printf( "\n" );
	getch();
	DWORD time = GetTickCount();
	check();
	time = GetTickCount() - time;
	printf( "time: %i \npress any key to exit...\n", time );
	getch();
	return 0;
};
