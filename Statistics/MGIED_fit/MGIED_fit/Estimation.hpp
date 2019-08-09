#pragma once
#ifndef MGIED_ESTIMATION
#define MGIED_ESTIMATION
#include <cmath>
#include <Eigen/Core>
#include <fstream>
#include <string>
#include<iostream>
#include<random>
#include<numeric>
#include<tuple>
#include<algorithm>
#include <limits>
#include <functional>
#include "src/integral/quadrature.hpp"

using namespace std;
using namespace std::placeholders;
using namespace Eigen;
using namespace SciStaEBD::integral;

struct MGIEDEstimator {
	VectorXd old_p, new_p;
	VectorXd old_alpha, new_alpha;
	VectorXd old_lambda, new_lambda;
	VectorXd x;
	VectorXd R;

	double p_change_norm, alpha_change_norm, lambda_change_norm;

	int iter;
	int max_iter;
	
	int K;
	double T;

	double p_i(int i, double z) {
		double x1 = (old_p.array() * old_alpha.array() * old_lambda.array() *
			(-old_lambda.array() / z).exp() *
			(1 - (-old_lambda.array() / z).exp()).pow(old_alpha.array() - 1)).sum();
		double x2 = (old_p.array() * (1 - (-old_lambda.array() / x[i])).pow(old_alpha.array() - 1)).sum();
		return x1 / (x2 * pow(z, 2));
	};

	double p_T( double z) {
		double x1 = (old_p.array() * old_alpha.array() * old_lambda.array() *
			(-old_lambda.array() / z).exp() *
			(1 - (-old_lambda.array() / z).exp()).pow(old_alpha.array() - 1)).sum();
		double x2 = (old_p.array() * 
			(1 - (-old_lambda.array() / T)).pow(old_alpha.array() - 1)).sum();
		return x1 / (x2 * pow(z, 2));
	};

	inline double s_ki(int k, int i, double x) {
		return pow(1 - exp(-old_lambda[k] / x), old_alpha[k]);
	};

	double s_i(int i, double x) {
		double res=0;
		for (int k = 0; k < K; i++) {
			res += old_p[k] * s_ki(k, i, x);
		};
		return res;
	}

	inline double b_ki(int i, int k, double x) {
		return old_p[k] * s_ki(k, i, x) / s_i(i, x);
	};

	inline double f_ki(int k, int i, double x) {
		return old_alpha[k] * old_lambda[k] * exp(-old_lambda[k] / x) *
			pow(1 - exp(-old_lambda[k] / x), old_alpha[k] - 1) / pow(x, 2);
	};

	double f_i(int i, double x) {
		double res=0;
		for (int k = 0; k < K; k++) {
			res += old_p[k] * f_ki(k, i, x);
		};
		return res;
	};

	inline double a_ki(int k, int i, double x) {
		return old_p[k] * f_ki(k, i, x) / f_i(i, x);
	};

	inline double D1_ki(int k, int i) {
		return a_ki(k, i, x[i]);
	};

	double D2_ki(int k, int i) {
		function<double(double)> f = [this, k, i](double x)->double {
			return b_ki(k, i, x) * p_i(i, x);
		};
		return quadrature(f, x[i], numeric_limits<double>::infinity());
	};

	double D4_ki(int k, int i) {
		function<double(double)> f = [this, k, i](double x)->double {
			return b_ki(k, i, x) * p_i(i, x)/x;
		};
		return quadrature(f, x[i], numeric_limits<double>::infinity());
	};

	double D5_ki(int k, int i) {
		function<double(double)> f = [this, k, i](double x)->double {
			return log(1-exp(-old_lambda[k]/x))*b_ki(k, i, x) * p_i(i, x) / x;
		};
		return quadrature(f, x[i], numeric_limits<double>::infinity());
	};

	double D6_ki(int k, int i) {
		function<double(double)> f = [this, k, i](double x)->double {
			return b_ki(k, i, x) * p_T(x) / x;
		};
		return quadrature(f, T, numeric_limits<double>::infinity());
	};

	double D8_ki(int k, int i) {
		function<double(double)> f = [this, k, i](double x)->double {
			return b_ki(k, i, x) * p_T(x) / x;
		};
		return quadrature(f, T, numeric_limits<double>::infinity());
	};

	double D9_ki(int k, int i) {
		function<double(double)> f = [this, k, i](double x)->double {
			return log(1 - exp(-old_lambda[k] / x)) * b_ki(k, i, x) * p_T(x) / x;
		};
		return quadrature(f, T, numeric_limits<double>::infinity());
	};

	void update_alpha() {

	};

	void update_lambda() {

	};

	void update_p() {


	};

	void print_vec(string name, VectorXd v) {
		cout << name;
		for (int i = 0; i < K; i++) {
			cout << v[i] << " ";
		};
		cout << endl;
	};

	void run() {
		for (int i = 1; i <= max_iter; i++) {

			update_alpha();
			update_lambda();
			update_p();

			p_change_norm = (new_p - old_p).norm();
			alpha_change_norm = (new_alpha - old_alpha).norm();
			lambda_change_norm = (new_lambda - old_lambda).norm();

			cout << "Iteration " << i << " , norm change: p " << p_change_norm <<
				" , alpha " << alpha_change_norm << " , lambda " << lambda_change_norm << endl;
		};

		cout << "Maximum iteration reached. Result: " << endl;
		print_vec("p : ", new_p);
		print_vec("alpha : ", new_alpha);
		print_vec("lambda : ", new_lambda);

	};

};
#endif