#include <iostream>
#include<string>
#include<bitset>
using namespace std;
int main()
{
	//x is a length 5 char array. one chinese symbol is two bytes.
	string x("什么");
	cout << x << endl;
	cout << sizeof(x[0]) << endl;
	//One wstring char is 2 bytes. Convert it to a two chars array.
	unsigned char b[2];
	cout << sizeof(b[0])<<endl;
	memcpy(&b, &x[2], 2);
	cout << b[0] << " "<<b[1] << endl;
	//Copy data to a string.
	cout << x << endl;
	memcpy(&x[2], &b, 2);
	cout << sizeof(x[1]) << endl;
	cout << x << endl;
}
