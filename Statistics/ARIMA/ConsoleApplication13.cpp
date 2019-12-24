/*
 * Empty C++ Application
 */

#include <iostream>
#include <random>
#include <cmath>
#include <memory>
//#include "xil_printf.h"
#include "Eigen/Core"
#include "quasi_newton.hpp"

using namespace std;
using namespace Eigen;
using namespace SciStaEBD::optimization;
using namespace std::placeholders;

const double PI = 3.141592653589793238463;

double ar_fun(VectorXd parm, VectorXd* data) {
	double lik = 0;
	const int n = data->size();

	double residual =0, yhat = parm[0];

	for (int i = 0; i < n; i++) {
		yhat = yhat + parm[1] * residual;
		residual = (*data)[i] - yhat;
		lik = lik - 0.5 * log(2 * PI) - log(parm[2]) -
			0.5 / (parm[2] * parm[2]) * residual * residual;
	}
	return -lik;
};
double s2_fun(VectorXd parm, VectorXd* data) {
	return parm[0] * parm[0];
};
VectorXd s2_fun_g(VectorXd parm, VectorXd* data) {
	VectorXd grad(1);
	grad[0] = 2 * parm[0];
	return grad;
};

double arma_fun(VectorXd parm, VectorXd* data) {
	double lik = 0;
	const int n = data->size();

	VectorXd residual(n);
	residual[0] = 0;
	residual[1] = 0;
	for (int i = 2; i < n; i++) {
		residual[i] = (*data)[i] - (parm[0]+ (*data)[i - 1] * parm[1] +
			residual[i - 1] * parm[2] + residual[i - 2] * parm[3]);
		lik = lik - 0.5 * log(2 * PI) - log(parm[4]) -
			0.5 / (parm[4] * parm[4]) * residual[i] * residual[i];
	}
	return lik;
};

VectorXd finite_diff(VectorXd parm, VectorXd* data, function<double(VectorXd, VectorXd*)> fun) {
	const int n1 = parm.size();
	VectorXd grad(n1);
	for (int i = 0; i < n1; i++) {
		VectorXd parmx = parm;
		VectorXd parmy = parm;
		parmx[i] = parmx[i] + 0.0000001;
		parmy[i] = parmy[i] - 0.0000001;
		grad[i] = (fun(parmx, data) - fun(parmy, data)) / 0.0000002;
	}
	return grad;
};

function<VectorXd(VectorXd, VectorXd*)> ar_grad = bind(finite_diff, _1, _2, ar_fun);
function<VectorXd(VectorXd, VectorXd*)> s2_grad_f = bind(finite_diff, _1, _2, s2_fun);

int main()
{
	VectorXd v(100);
	v << 9.78, 11.23, 10.42, 9.7, 8.8, 9.5, 9.19, 8.21, 8.45, 7.57, 6.35, 7.22, 7.4, 7.45, 7.26, 5.82, 7.12, 7.55, 6.65, 7.07, 6.5, 6.43, 6.31, 5.4, 5.35, 4.95, 4.85, 5.32, 4.94, 5.37, 5.57, 5.1, 4.99, 5.07, 5.15, 4.65, 4.07, 3.73, 3.82, 3.07, 3.29, 3.06, 2.78, 2.92, 3.53, 3.4, 2.99, 3.43, 3.4, 3.5, 3.23, 3.14, 3.27, 3.53, 3.87, 4.38, 4.52, 4.53, 4.59, 4.98, 4.93, 5.1, 5.44, 5.57, 5.52, 5.28, 5.1, 5.42, 6.04, 5.82, 5.68, 5.93, 6.65, 7.34, 7.18, 7.84, 7.57, 7.8, 7.54, 8.7, 7.82, 7.54, 7.34, 6.37, 6.89, 7.25, 6.81, 6.7, 6.65, 7, 7.08, 7.58, 7.47, 8.34, 8.44, 8.51, 7.75, 8.41, 8.31, 7.95;
	cout << "size of data: " << v.size() << endl;
	VectorXd x0(5);
	x0 << 7, 0.9, 1, 1, 0.5;

	//Solution s = quasi_newton<VectorXd>(arma_fun, x0, &v, finite_diff);
	VectorXd x1(3);
	x1 << 7, 0.9, 1;
	//Solution s = quasi_newton<VectorXd>(ar_fun, x1, &v, ar_grad);
	VectorXd x2(1);
	x2 << 5;
	Solution s1 = quasi_newton<VectorXd>(s2_fun, x2, nullptr, s2_grad_f);
	cout << s1.grad << endl;
	return 0;
}
