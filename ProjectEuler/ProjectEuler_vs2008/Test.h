#pragma once

#include <cmath>
#include "Factors.h"

using namespace std;

void test() {
	/*
	double num = 45 / 9 * pow(10, 11);

	for (int i = 0; i < 10; i++) {
		num += 4.5 * pow(10, i);
	}

	cout << "result: " << __int64(num) << endl;
	*/

	printFactors(getFactors(9825414));

	cout << endl;
}