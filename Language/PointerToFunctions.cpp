
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include<chrono>

using namespace std;
int fun1(int x) {
	return x;
};

class A {
public:
	int fun2(int x) {
		return x;
	};

	static int fun3(int x) {
		return x;
	};
};

int fun4(int (*fun)(int) , int x) {
	return fun(x);
};

int main()
{
	//1. Pointer to function.
	int (*fp)(int x) = fun1;
	cout << fp(3) << endl;

	typedef int (*fpx)(int x);
	fpx fp1 = fun1;
	cout << fp1(3) << endl;

	//2. Pointer to ordinary class method.
	A a;
	int (A:: * fp2)(int) = &A::fun2;
	cout << (a.*fp2)(3) << endl;

	//3. Pointer to static class method.
	//We can see that pointer to static member is just like pointer to normal function.
	int (*fp3)(int x) = &A::fun3;
	cout << fp3(3) << endl;

	//4. Pass a function pointer to a function.
	cout << fun4(fp, 3) << endl;
}