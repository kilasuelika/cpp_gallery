#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <armadillo>
#include <chrono>

using namespace std;
using namespace arma;

enum step_method {
	Armijo,
	Golden
};

// Function 1.
double fun1(vec x) {
	return exp(x[0] + 3 * x[1] - 0.1) + exp(x[0] - 3 * x[1] - 0.1) + exp(-x[0] - 0.1);
};
vec d1(vec x) {
	vec grad(2);
	grad[0] = exp(x[0] + 3 * x[1] - 0.1) + exp(x[0] - 3 * x[1] - 0.1) - exp(-x[0] - 0.1);
	grad[1] = exp(x[0] + 3 * x[1] - 0.1) * 3 - exp(x[0] - 3 * x[1] - 0.1) * 3;
	return grad;
};
//Function 2.
double fun2(vec x) {
	return x[0] - x[1] + 2 * pow(x[0], 2) + 2 * x[0] * x[1] + pow(x[1], 2);
};
vec d2(vec x) {
	vec grad(2);
	grad[0] = 1 + 4 * x[0] + 2 * x[1];
	grad[1] = -1 + 2 * x[0] + 2 * x[1];
	return grad;
};
//Function 3.
double fun3(vec x) {
	return 5 * pow(x[0], 2) + pow(x[1], 2) + 4 * x[0] * x[1] - 6 * x[0] - 4 * x[1] + 15;
};
vec d3(vec x) {
	vec grad(2);
	grad[0] = 10 * x[0] + 4 * x[1] - 6;
	grad[1] = 2 * x[1] + 4 * x[0] - 4;
	return grad;
};
//Function 4.
double fun4(vec x) {
	return -log(1 - x[0] - x[1]) - log(x[0]) - log(x[1]);
};
vec d4(vec x) {
	vec grad(2);
	grad[0] = 1 / (1 - x[0] - x[1]) - 1 / x[0];
	grad[1] = 1 / (1 - x[0] - x[1]) - 1 / x[1];
	return grad;
};
//Function 5.
double fun5(vec x) {
	return x[0] - 0.6 * x[1] + 4 * x[2] + 0.25 * x[3] -
		accu(log(x)) -
		log(5 - accu(x));
};
vec d5(vec x) {
	vec grad(4);
	grad[0] = 1 - 1 / x[0] + 1 / (5 - accu(x));
	grad[1] = -0.6 - 1 / x[1] + 1 / (5 - accu(x));
	grad[2] = 4 - 1 / x[2] + 1 / (5 - accu(x));
	grad[3] = 0.25 - 1 / x[3] + 1 / (5 - accu(x));
	return grad;
};

//Armijo Step.
int armijo(function<double(vec)> fun,
	function<vec(vec)> grad,
	vec x, double beta, double sigma, int maxm) {

	for (int i = 0; i <= maxm; i++) {
		if (fun((vec)(x - pow(beta, i)*pow(beta, i))) <=
			(fun(x) - sigma * pow(beta, i) * dot(grad(x), grad(x)))) {
			return i;
		};
	};
	return maxm;
};
//Golden step.
double golden(function<double(vec)> fun,
	function<vec(vec)> grad,
	vec x, double a, double b) {
	
	vec d= -grad(x);

	double lambda = a + 0.382 * (b - a), mu = a + 0.618 * (b - a);
	double fl = fun(x+lambda*d), fu = fun(x+mu*d);

	while (true) {
		if (fl > fu) {
			if (b - lambda <= pow(10, -6)) {
				return mu;
			};
			a = lambda;
			lambda = mu;
			fl = fun(x + mu * d);
			mu = a + 0.618 * (b - a);
			fu = fun(x + mu * d);
		}
		else {
			if (mu - a <= pow(10, -6)) {
				return lambda;
			};
			b = mu;
			mu = lambda;
			fu = fun(x + lambda * d);
			lambda = a + 0.382 * (b - a);
			fl = fun(x + lambda * d);
		};
	}
};


//Optimization.
//A struct for storing parameters.
struct parmset {
	step_method step;
	double beta = 0.7;
	double sigma = 0.2;
	int armijo_max_iter = 10;
	double a_1=0;
	double b_1=1;
	int gtol = 4;
	int stol = 4;
};
tuple<vec,double> optimization(function<double(vec)>fun,
	function<vec (vec)> grad,
	vec x0,
	parmset ps) {
	
	auto start = chrono::system_clock::now();
	//Count number of iterations.
	int iter=0;
	
	//Main loop.
	switch (ps.step) {
	case step_method::Armijo: {
		int mk;
		while (norm(grad(x0)) > pow(10, -ps.gtol)) {
			mk = armijo(fun, grad, x0, ps.beta, ps.sigma, ps.armijo_max_iter);
			x0 = x0 -pow(ps.beta, mk)*grad(x0);
			++iter;
		};

		break;
	}
	case step_method::Golden: {
		double direction;
		while (norm(grad(x0)) >= pow(10, -ps.gtol)){
			direction = golden(fun,grad,x0,ps.a_1,ps.b_1);
			x0 = x0 - direction * grad(x0);
			++iter;
		};

		break;
	}
	default:
		break;
	};

	//Calculate elapsed time.
	auto ela = chrono::system_clock::now() - start;
	auto msc = chrono::duration_cast<chrono::milliseconds>(ela);
	if (ps.step == step_method::Armijo) {
		printf("Armijo: Optimization stops after %d iterations and %f seconds.\n", iter, (double)(msc.count() / 1000.0));
	}
	else if (ps.step == step_method::Golden) {
		printf("Golden search: Optimization stops after %d iterations and %f seconds.\n", iter, (double)(msc.count() / 1000.0));
	};
		
	//Print result.
	cout << "x: [";
	for (int i = 0; i < size(x0) - 1; ++i) {
		cout <<fixed<< x0[i] << " ";
	};
	cout <<fixed<< x0[size(x0) - 1] << "], " << "f: " << fun(x0) << endl;

	return make_tuple(x0, fun(x0));
}
int main()
{
	cout.precision(4);

	vec x0{ 0.25,0.25 };
	parmset ps1,ps2;
	ps1.step = step_method::Armijo;
	ps2.step = step_method::Golden;

	cout << "Problem 1:" << endl;
	optimization(fun1, d1, x0, ps1);
	optimization(fun1, d1, x0, ps2);
	cout << endl;

	cout << "Problem 2:" << endl;
	optimization(fun2, d2, x0, ps1);
	optimization(fun2, d2, x0, ps2);
	cout << endl;

	cout << "Problem 3:" << endl;
	optimization(fun3, d3, x0, ps1);
	optimization(fun3, d3, x0, ps2);
	cout << endl;

	cout << "Problem 4:" << endl;
	optimization(fun4, d4, x0, ps1);
	optimization(fun4, d4, x0, ps2);
	cout << endl;

	x0 = { 0.25,0.25,0.1,0.1 };
	cout << "Problem 5:" << endl;
	optimization(fun5, d5, x0, ps1);
	optimization(fun5, d5, x0, ps2);
	cout << endl;
}
