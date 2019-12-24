// test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<functional>

using namespace std;
using namespace std::placeholders;

double D2_fun(int k, int i, double x) {
	//return b_ki(k, i, x) * p_i(i, x);
	return 7;
};
double D2_ki(int k, int i) {
	double res;
	function<double(double)> f = bind(&D2_fun, k, i, _1);
	//return quadrature(f, x[i], numeric_limits<double>::infinity);
	return 7;
};

int main()
{
	std::cout << D2_ki(2,3);
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
