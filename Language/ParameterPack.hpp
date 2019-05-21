#include <iostream>
#include <functional>
#include <cstdarg>
#include <string>
using namespace std;

//Auto deduct argument type for hash function.
template<typename T>
size_t hash_u(T x) {
	return hash<T>{}(x);
};

//Combine hash.
//Fold expression-left binary.
template<typename... Args>
size_t hash_combine(Args... args) {
	return (hash_u(args) + ...);
};

//Variadic functions.
//n is number of arguments needing hashing.
//fmt is type of each arguments: 'i' for int, 'd' for double.
size_t hash_combine_var(const string fmt, ...){
	va_list args;
	va_start(args, fmt);
	size_t sum=0;
	
	for(auto s: fmt){
		switch(s){
			case 'i':
				sum+=hash_u(va_arg(args,int));
				break;
			case 'd':
				sum+=hash_u(va_arg(args,double));
				break;
			default:
				break;
		}
	}
	
	va_end(args);
	return sum;
};

void run_parameterpack()
{
	cout << "Combine hashing with fold expression: "<<hash_combine(67, 89.9)<<endl;
	cout << "Combine hashing with variadic function: "<<hash_combine_var("id",67,89.9)<<endl;
}