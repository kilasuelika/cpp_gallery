// ConsoleApplication6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
using namespace std;
void fun1(int& x) {
	x = 88;
};
int fun(int x) {
	x = 7;
	fun1(x);
	return x;
};

template<typename T>
int inf_index(vector<T>& v, int n, int l, int u) {
	if (v[l] > v[n]) return -1;
	if (v[u] < v[n])return u;

	int mid;
	while (u-l>1) {
		mid = l + (u - l) / 2;
		if (v[n] <= v[mid]) {
			u = mid;
		}
		else {
			l = mid;
		};
	};
	
	if (v[l] == v[n]) return -1;
	if (v[l] < v[n] && v[u]>=v[n]) return l;
	if (v[u] < v[n]) return u;
}
int main()
{
	int y = 99;
	vector<int> v{ 4,4,5,7,8,9 };
	cout << inf_index<int>(v,0,1,5) << endl;
	v[0] = 5;
	cout << inf_index<int>(v, 0, 1, 5) << endl;
	v[0] = 8;
	cout <<"Target: "<<v[0]<<" "<< inf_index<int>(v, 0, 1, 5) << endl;
	v[0] = 10;
	cout << inf_index<int>(v, 0, 1, 5) << endl;
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
