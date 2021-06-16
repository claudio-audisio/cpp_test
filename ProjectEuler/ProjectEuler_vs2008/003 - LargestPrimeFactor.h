#pragma once

#include "Primes.h"

using namespace std;

void largestPrimeFactor(__int64 number) {
	__int64 largestFactor = 1;
	__int64 currentProd = 1;
	
	while (currentProd < number) {
		largestFactor++;
		if ((number % largestFactor == 0) && isPrime(largestFactor)) {
			currentProd *= largestFactor;
		}
	}

	cout << "result: " << largestFactor << endl;
}

void nThPrimes(__int64 limitIndex) {
	__int64 index = 3;

	isPrime(2);

	while (primes.size() < limitIndex) {
		isPrime(index);
		index += 2;
	}

	cout << "result: " << *primes.rbegin() << endl;
}
