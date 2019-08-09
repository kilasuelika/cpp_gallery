#pragma once
#ifndef MGIED_SIMULATION
#define MGIED_SIMULATION
#include <cmath>
#include <Eigen/Core>
#include <fstream>
#include <string>
#include<iostream>
#include<random>
#include<numeric>
#include<tuple>
#include<algorithm>
#include "src/optimization/solve.hpp"

using namespace std;
using namespace Eigen;
using namespace SciStaEBD::optimization;

double mgied_cdf_eqn(double x, double *w) {
	return 1 - 0.3 * pow(1 - exp(-1.0 / x), 1.8) - 0.7 * pow(1 - exp(-2.7 / x), 1) - *w;
};

tuple<MatrixXd, int, int> simulate_mgied(
	int m, int n, tuple<int,double> tT, VectorXi R) {
	//1. Generate standard exp distribution sample;
	VectorXd Z(m);
	default_random_engine generator;
	exponential_distribution<double> distribution;
	for (int i = 0; i < m; i++) {
		Z[i] = distribution(generator);
	};
	
	int t1 = get<0>(tT);
	double t2 = get<1>(tT);

	//2. Calculate Y and W.
	VectorXd Y(m);
	Y[0] = Z[0] / m;
	for (int i = 1; i < m; i++) {
		Y[i] = Y[i - 1] + Z[i] / (n -accumulate(&R[0],&R[i],0.0)-i+1);
	};

	for (int i = 0; i < m; i++) {
		Y[i] = 1 - exp(-Y[i]);
	};

	
	//3. Calculate quantile.
	for (int i = 0; i < m; i++) {
		Y[i] = solve_bisect<double>(mgied_cdf_eqn, 0.0, 1000.0, &Y[i]);

	};

	//T
	double T;
	if (t1 == 0) {
		T = Y[m - 1] + t2;
	}
	else {
		T = Y[t1] + t2;
	};

	MatrixXd S;
	int RD, D;
	if (Y[m - 1] <= T) {
		S.resize(m, 2);
		for (int i = 0; i < m; i++) {
			S(i, 0) = Y[i];
			S(i, 1) = R[i];
		};
		RD = 0;
		D = m;
	}
	else {
		double* J = upper_bound<double*,int>(&Y[0],&Y[m-1],T);
		int l = J - &Y[0];
		S.resize(l,2);
		for (int i = 0; i < l; i++) {
			S(i, 0) = Y[i];
			S(i, 1) = R[i];
		};
		RD = n-l-R.sum();
		D = l;
	}
	return make_tuple(Y, D, RD);
};

void write_data(string filename, MatrixXd x) {
	ofstream file(filename, ios::out);
	file << "X" << endl;
	for (int i = 0; i < x.rows(); i++) {
		file << x(i,0) << endl;
	};

	file.close();
	cout << x.size() << " data points have been written to file " << filename << "."<<endl;
	
	
}

#endif // !MGIED_SIMULATION
