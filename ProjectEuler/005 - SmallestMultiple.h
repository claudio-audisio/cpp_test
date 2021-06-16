#pragma once

#include <map>

#include "Factors.h"

using namespace std;

void smallestMultiple(int limit) {
	set<__int64> primes = getPrimes(limit);
	set<__int64>::const_iterator itP = primes.begin();
	__int64 multiple = 1;
	bool checkPower = true;

	while (itP != primes.end()) {
		const __int64 prime = *itP;

		if (checkPower && prime < sqrt(double(limit))) {
			multiple *= pow(prime, floor(log(double(limit)) / log(double(prime))));
		}
		else {
			checkPower = false;
			multiple *= prime;
		}
		itP++;
	}
	
	cout << "result: " << multiple << endl;
}

void mySmallestMultiple(int limit) {
	__int64 multiple = 1;
	map<int, int> factors;

	
	for (int i = 2; i <= limit; i++) {
		map<int, int> currentFactors = getFactors(i);
		map<int, int>::const_iterator itF = currentFactors.begin();

		while (itF != currentFactors.end()) {
			const int currentF = itF->first;
			const int fFreq = itF->second;

			if (factors.find(currentF) == factors.end()) {
				factors.insert(make_pair(currentF, fFreq));
			}
			else if (fFreq > factors[currentF]) {
				factors[currentF] = fFreq;
			}
			itF++;
		}
	}

	printFactors(factors);
		
	map<int, int>::const_iterator itF = factors.begin();

	while (itF != factors.end()) {
		multiple *= pow(double(itF->first), double(itF->second));
		itF++;
	}

	cout << "result: " << multiple << endl;
}