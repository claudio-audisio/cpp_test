#pragma once
#include <string>

using namespace std;

bool isPalindromic(__int64 number) {
	string stringNumber = to_string(number);
	int len = stringNumber.length();

	if (len == 1) {
		return true;
	}

	for (int i = 0; i < len / 2; i++) {
		if (stringNumber.at(i) != stringNumber.at(len - i - 1)) {
			return false;
		}
	}

	return true;
}

void largestPalindromeProduct() {
	cout << "\n*** Largest Palindrome Product ***";
	cout << "\nEnter the number of digits: ";

	int digits;
	cin >> digits;

	__int64 limitDown = (__int64)pow(10, digits - 1) - 1;
	__int64 limitUp = (__int64)pow(10, digits) - 1;
	__int64 max = 0;
	__int64 factorA, factorB;
	
	for (__int64 i = limitUp; i > limitDown ; i--) {
		for (__int64 j = i; j > limitDown; j--) {
			if (i * j < max) {
				break;
			}

			if (isPalindromic(i * j)) {
				if (i * j > max) {
					max = i * j;
					factorA = i;
					factorB = j;
				}
			}
		}
	}

	cout << "Largest palindromic number of " << digits << " digits is: " << max << " (" << factorA << " x " << factorB << ")" << endl;
}