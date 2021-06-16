#pragma once

#include <set>

#include "Primes.h"

using namespace std;

void summationOfPrimes(int limit) {
	__int64 sum = 2;

	isPrime(2);

	for (int i = 3; i < limit; i += 2) {
		//cout << "\rcheckig " << i;
		if (isPrime(i, true)) {
			sum += i;
		}
	}

	cout << "result: " << sum << endl;
}