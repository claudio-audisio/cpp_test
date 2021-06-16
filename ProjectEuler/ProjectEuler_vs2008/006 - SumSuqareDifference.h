#pragma once

using namespace std;

void sumSquareDifference(int limit) {
	__int64 sum = 0;
	__int64 squares = 0;

	for (int i = 1; i <= limit; i++) {
		squares += i * i;
		sum += i;
		cout << i << "\t" << squares << "\t" << sum * sum << "\t" << sum * sum - squares << endl;
	}
}