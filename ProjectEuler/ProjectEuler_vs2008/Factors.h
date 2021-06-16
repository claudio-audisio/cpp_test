#pragma once

#include <set>
#include <vector>
#include <map>

#include "Primes.h";

using namespace std;

map<int, int> m_factors;

void addFactor(int factor) {
	map<int, int>::iterator itM = m_factors.find(factor);

	if (itM == m_factors.end()) {
		m_factors.insert(make_pair(factor, 1));
	}
	else {
		(itM->second)++;
	}
}

map<int, int> getFactors(__int64 number) {
	m_factors.clear();

	if (isPrime(number)) {
		addFactor(number);
	}
	else {
		set<__int64> probFactors = getPrimes(number / 2);

		for (auto probF : probFactors) {
			__int64 currentNum = number;

			while (currentNum % probF == 0) {
				addFactor(probF);
				currentNum /= probF;
			}
		}
	}

	return m_factors;
}

void printFactors(map<int, int> factors) {
	map<int, int>::const_iterator itF = factors.begin();

	while (itF != factors.end()) {
		cout << itF->first << "^" << itF->second << endl;
		itF++;
	}
}