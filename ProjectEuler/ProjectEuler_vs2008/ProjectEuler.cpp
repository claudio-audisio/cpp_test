// ProjectEuler.cpp : Defines the entry point for the console application.
//
#include <cstdio>
#include <iostream>

#include "../Services.h"
#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Project Euler!" << endl;

	int selection = menu();

	solveProblem(selection);

	return 0;
}
