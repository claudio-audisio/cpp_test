#include <cstdio>
#include <iostream>

#include "Services.h"

using namespace std;

int main(int argc, char* argv[])
{
	int selection = 0;

	while (selection != -1) {
		selection = menu();
		solveProblem(selection);
		cout << "\n" << endl;
	}

	return 0;
}