#pragma once

#include <set>
#include <math.h>

using namespace std;

set<__int64> primes;

bool isPrime(__int64 n, bool usePreviousPrimes = false) {
	if (primes.find(n) != primes.end()) {
		return true;
	}

	if (n == 2 || n == 3) {
		primes.insert(n);
		return true;
	}

	if ((n % 6) != 1 && (n % 6) != 5) {
		return false;
	}

	__int64 limit = sqrt(double(n));

	if (usePreviousPrimes) {
		set<__int64>::const_iterator itP = primes.begin();

		while (itP != primes.end()) {
			if (n % (*itP) == 0) {
				return false;
			}

			if ((*itP) > limit) {
				primes.insert(n);
				return true;
			}

			itP++;
		}
	}

	if (!usePreviousPrimes) {
		if (n % 2 == 0) {
			return false;
		}

		for (int i = 3, k = 0; i < n / 2; i += 2, k++) {
			if (n % i == 0) {
				return false;
			}
		}
	}

	primes.insert(n);
	return true;
}

set<__int64> getPrimes(int limit) {
	set<_int64> primes;
	for (int i = 2; i <= limit; i++) {
		//cout << "\revaluating " << i;

		if (isPrime(i)) {
			primes.insert(i);
		}
	}
	return primes;
}

void printPrimes(int limit) {
	for (int i = 2; i < limit; i++) {
		if (isPrime(i)) {
			cout << i << " ";
		}
	}
	cout << endl;
}