#include <cstdio>
#include <iostream>

#include "Services.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Project Euler!" << endl;

	int selection = menu();

	solveProblem(selection);

	return 0;
}