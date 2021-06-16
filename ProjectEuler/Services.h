#pragma once

#include <ctime>
#include <windows.h>

#pragma comment( lib, "winmm.lib") 

#include "001 - MultipleOf3And5.h"
#include "002 - EvenFibonacciNumbers.h"
#include "003 - LargestPrimeFactor.h"
#include "005 - SmallestMultiple.h"
#include "006 - SumSuqareDifference.h"
#include "008 - LargestProductInASeries.h"
#include "009 - SpecialPythagoreanTriplet.h"
#include "010 - SummationOfPrimes.h"
#include "011 - LargestProductInAGrid.h"
#include "014 - LongestCollatzSequence.h"
#include "069 - TotientMaximum.h"
#include "Test.h"

using namespace std;

int menu()
{
	cout << "0.\tPrint primes" << endl;
	cout << "1.\tMultiples of 3 and 5" << endl;
	cout << "2.\tEven Fibonacci numbers" << endl;
	cout << "3.\tLargest prime factor" << endl;
	cout << "5.\tSmallest multiple" << endl;
	cout << "6.\tSum square difference" << endl;
	cout << "7.\t10001st prime" << endl;
	cout << "8.\tLargest product in a series" << endl;
	cout << "9.\tSpecial Pythagorean triplet" << endl;
	cout << "10.\tSummation of primes" << endl;
	cout << "11.\tLargest product in a grid" << endl;
	cout << "14.\tLongest Collatz sequence" << endl;
	cout << "69.\tTotient maximum" << endl;

	cout << "999.\tTest" << endl;
	
	cout << "\nSelect a problem to solve: ";

	int selection;
	cin >> selection;

	return selection;
}

void solveProblem(int problem)
{
	unsigned long time = timeGetTime();

	switch (problem)
	{
		case 0:		printPrimes(10*1000); break;
		case 1:		multipleOf3And5(); break;
		case 2:		evenFibonacciNumbers(4*1000*1000); break;
		case 3:		largestPrimeFactor(600851475143); break;
		case 5:		smallestMultiple(20); break;
		case 6:		sumSquareDifference(100); break;
		case 7:		nThPrimes(10001); break;
		case 8:		largestProductInASeries(13); break;
		case 9:		specialPythagoreanTriplet(1000); break;
		case 10:	summationOfPrimes(2*1000*1000); break;
		case 11:	largestProductInAGrid(); break;
		case 14:	longestCollatzSequence(10*1000*1000); break;
		case 69:	totientMaximum(1000000); break;
		case 999:	test(); break;
		default: cout << "Problem " << problem << " not found" << endl;
			break;
	}

	std::cout << "time: " << timeGetTime() - time << " ms " << std::endl;
}
