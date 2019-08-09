// MGIED_fit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "simulation.hpp"
#include <iostream>

int main()
{
	int m=70, n=80, T = 2;
	VectorXi v;
	v.resize(m);
	v.setZero();
	v[0] = n - m;
	tuple<MatrixXd, int, int> sim = simulate_mgied(m,n,make_tuple(0,2.1),v);
	MatrixXd data = get<0>(sim);
	write_data("MGIED simulation.csv", data);


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
