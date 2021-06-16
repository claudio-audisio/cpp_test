#pragma once

#include <map>

using namespace std;

map<int, __int64> fibonacciValues;

__int64 fibonacci(int i) {
	if (i == 1)
		return 1;

	if (i == 2)
		return 2;

	__int64 prev, prevPrev;

	map<int, __int64>::const_iterator itPrev = fibonacciValues.find(i - 1);

	if (itPrev != fibonacciValues.end()) {
		prev = (*itPrev).second;
	}
	else {
		prev = fibonacci(i - 1);
	}

	map<int, __int64>::const_iterator itPrevPrev = fibonacciValues.find(i - 2);

	if (itPrevPrev != fibonacciValues.end()) {
		prevPrev = (*itPrevPrev).second;
	}
	else {
		prevPrev = fibonacci(i - 2);
	}

	fibonacciValues.insert(make_pair(i, prev + prevPrev));

	return prev + prevPrev;
}

void evenFibonacciNumbers(__int64 limit) {
	__int64 sum = 0;
	__int64 fib = 0;
	int i = 1;

	while (fib < limit) {
		fib = fibonacci(i++);

		if (fib % 2 == 0) {
			sum += fib;
		}
	}

	cout << "result: " << sum << endl;
}
