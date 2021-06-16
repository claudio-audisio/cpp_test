#pragma once

#include <set>

#include "Primes.h"

using namespace std;

int countPrimes(set<__int64> primes, __int64 min, __int64 max) {
	set<__int64>::const_iterator itP = primes.begin();
	int counter = 0;

	while (itP != primes.end()) {
		if (*itP >= max) {
			break;
		}

		if (*itP > min) {
			counter++;
		}

		itP++;
	}

	return counter;
}

void totientMaximum(int limit) {
	set<__int64> primes = getPrimes(limit);
	__int64 currentProd = 1;
	set<__int64>::const_iterator itP = primes.begin();

	while (itP != primes.end()) {
		if (currentProd * (*itP) > limit) {
			break;
		}
		const __int64 prime = *itP;
		currentProd *= prime;
		int totient = countPrimes(primes, prime, currentProd) + 1;
		cout << currentProd << ": " << totient << endl;
		itP++;
	}

	cout << "result: " << currentProd << endl;
}
