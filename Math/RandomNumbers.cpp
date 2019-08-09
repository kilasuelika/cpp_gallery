#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "boost/random.hpp"
#include "boost/random/normal_distribution.hpp"
using namespace std;

int main()
{

	/*Note if we don't provide a seed, then each time run this program, we get the same data.
	To get different data, provide a seed by dre(seed) which should comes from external environment.
	For example, time*/
	chrono::time_point now = chrono::system_clock::now();
	int seed = now.time_since_epoch().count();

	default_random_engine dre(seed);
	normal_distribution<double> nr(0.0, 1.0);
	int k = 0;
	double x = 0;
	double sum = 0;

	for (int i = 1; i < 10000000; i++) {
		x = nr(dre);
		sum += x;
		if ((++k) < 10) {
			cout << x << endl;
		};
	};

	cout <<"Sum: "<< sum << endl;

	//Boost.
	boost::random::lagged_fibonacci19937 rng(seed);
	boost::random::normal_distribution<> dist;
	double sum1 = 0;
	for (int i = 1; i < 10000000; i++) {
		x = dist(rng);
		sum1 += x;
	};

	cout << "Sum: " << sum1 << endl;
	return 0;
}