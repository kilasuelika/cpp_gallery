#include <iostream>
using namespace std;
int main()
{
	int arr[1000];
	arr[0] = 5;
	arr[1] = 6;
	arr[2] = 9;

	double* x = reinterpret_cast<double *>(&arr[1]);
	uint64_t u;
	memcpy(&u, x, sizeof(double));
	cout <<hex<< u << endl;

	/*
	Note that in Intel CPU, data is stored with little-endian so that real data for *x is
	"arr[2] arr[1]".
	*/
	*x = 999.2365;
	cout << arr[1] << " "<<arr[2] << endl;
	memcpy(&u, x, sizeof(double));
	cout << u << endl;
	cout << *x << endl;

	struct A {
		int x = 8;
		double y = 99.2;
	};

	A* a = reinterpret_cast<A*>(&arr[3]);
	a->x = 999;
	a->y = 2.36;
	cout << a->x << " " << a->y << endl;
}