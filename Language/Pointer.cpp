// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class vectorImp {
public:
	vector<int> data;

	vector<int> operator[](vector<int> idx) {
		vector<int> temp;
		for (int x : idx) {
			temp.push_back(this->data[x]);
		};
		return temp;
	};
	vectorImp(vector<int> x) :data(x) {};
};

int main()
{
	vectorImp v({ 1,2,5,6,7 });
	vector<int> vsub = v[{2, 1}];
	for_each(vsub.begin(), vsub.end(), [](int x) {cout << x << " "; });
	cout << endl;

	//Relocate memory when insert elements to a vector.
	vector<int> v1{ 1 };
	cout << "Address of first element: " << &v1[0] << endl;
	v1.push_back(10);
	v1.push_back(20);
	cout << "Address of first element: " << &v1[0] << endl;

	//Reallocating and pointer.
	cout << endl;
	vector<int> v2{ 1 };
	cout << "First element: " << v2[0] << endl;
	int* x = &v2[0];
	*x = 9;
	cout << "First element after change value by pointer: " << v2[0] << endl;
	v2.push_back(20);
	v2.push_back(90);
	*x = 99;
	cout << *x << endl;
	cout << "First element: " << v2[0] << endl;
	cout << "It can be shown that when reallocating happend. We can not change element in a vector by a pointer" << endl;

	//Reallocating and smart pointer.
	cout << endl;
	vector<int> v3{ 1 };
	cout << "First element: " << v3[0] << endl;
	int *p=&v3[0];
	*p = 9;
	cout << "First element after change value by pointer: " << v3[0] << endl;
	v3.push_back(20);
	v3.push_back(90);
	*p = 99;
	cout << *p << endl;
	cout << "First element: " << v3[0] << endl;
	cout << "It can be shown that when reallocating happend. We can not change element in a vector by a pointer" << endl;

}