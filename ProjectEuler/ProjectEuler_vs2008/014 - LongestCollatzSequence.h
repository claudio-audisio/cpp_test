#pragma once

#include <map>

using namespace std;

map<__int64, __int64> sequences;

__int64 collatzSequenceSizeRecursive(__int64 n) {
	__int64 size;
	
	if (n == 1) {
		sequences.insert(make_pair(1, 1));
		return 1;
	}

	map<__int64, __int64>::const_iterator itS = sequences.find(n);

	if (itS != sequences.end()) {
		return (*itS).second;
	}

	if (n % 2 == 0) {
		size = collatzSequenceSizeRecursive(n / 2) + 1;
	}
	else {
		size = collatzSequenceSizeRecursive((n * 3) + 1) + 1;
	}
	
	sequences.insert(make_pair(n, size));

	return size;
}

void longestCollatzSequence(int limit) {
	int current;
	__int64 size, maxSize = 0;

	for (int i = 1; i < limit; i++) {
		size = collatzSequenceSizeRecursive(i);
				
		if (size > maxSize) {
			maxSize = size;
			current = i;
		}
	}

	cout << "\rresult: " << current << " --> " << maxSize << endl;
}