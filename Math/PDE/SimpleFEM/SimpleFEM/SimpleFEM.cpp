// SimpleFEM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <Eigen/Sparse>
#include <filesystem>
#include <fstream>
using namespace Eigen;

using namespace std;

void write_data(VectorXd x, VectorXd y, VectorXd v) {
	ofstream file("data.csv", ios::out);
	int m = x.size(), n = y.size();

	//Write the first row as x
	file << ",";
	for (int j = 0; j < m - 1; j++) {
		file << x[j] << ",";
	}
	file << x[m - 1] << endl;
	//Write y and data.
	for (int i = 0; i < n; i++) {
		file << y[i] << ",";
		for (int j = 0; j < m-1; j++) {
			file << v[i * m + j] << ",";
		};
		file << v[i * m + m - 1] << endl;

	};
	file.close();

};

int main()
{
	constexpr double xr = 1, xl = 0, yt = 1, yb = 0;
	constexpr int M = 30, N = 30;

	auto r = [](double x, double y)->double {return 4 * pow(EIGEN_PI, 2); };
	auto f = [](double x, double y)->double {return 2 * sin(2 * EIGEN_PI * y); };
	auto g1 = [](double y)->double {return 0; };
	auto g2 = [](double y)->double {return 0; };
	auto g3 = [](double y)->double {return 0; };
	auto g4 = [](double y)->double {return sin(2 * EIGEN_PI * y); };

	constexpr int m = M + 1, n = N + 1;
	constexpr int mn = m * n;

	constexpr double h = (xr - xl) / M, k = (yt - yb) / N;
	double h2 = pow(h, 2), k2 = pow(k, 2), hk = h * k;

	VectorXd x(m), y(n);

	x.setLinSpaced(m, 0, M);
	y.setLinSpaced(n, 0, N);
	x = (xl + x.array() * h).matrix();
	y = (yb + y.array() * k).matrix();

	SparseMatrix<double> A(mn, mn);
	VectorXd b(mn);
	b.setZero();
	for (int i = 1; i <= M - 1; i++) {
		for (int j = 1; j <= N - 1; j++) {
			double rsum = r(x[i] - 2 * h / 3, y[j] - k / 3) +
				r(x[i] - h / 3, y[j] - 2 * k / 3) + r(x[i] + h / 3, y[j] - k / 3);
			rsum = rsum + r(x[i] + 2 * h / 3, y[j] + k / 3) +
				r(x[i] + h / 3, y[j] + 2 * k / 3) + r(x[i] - h / 3, y[j] + k / 3);
			A.insert(i + j * m, i + j * m) = 2 * (h2 + k2) / hk - hk * rsum / 18;
			A.insert(i + j * m, i - 1 + j * m) = -k / h - hk * (r(x[i] - h / 3, y[j] + k / 3) +
				r(x[i] - 2 * h / 3, y[j] - k / 3)) / 18;
			A.insert(i + j * m, i - 1 + (j - 1) * m) = -hk * (r(x[i] - 2 * h / 3, y[j] - k / 3) +
				r(x[i] - h / 3, y[j] - 2 * k / 3)) / 18;
			A.insert(i + j * m, i + (j - 1) * m) = -h / k - hk * (r(x[i] - h / 3, y[j] - 2 * k / 3) +
				r(x[i] + h / 3, y[j] - k / 3)) / 18;

			A.insert(i + j * m, i + 1 + j * m) = -k / h - hk * (r(x[i] + h / 3, y[j] - k / 3) +
				r(x[i] + 2 * h / 3, y[j] + k / 3)) / 18;
			A.insert(i + j * m, i + 1 + (j + 1) * m) = -hk * (r(x[i] + 2 * h / 3, y[j] + k / 3) +
				r(x[i] + h / 3, y[j] + 2 * k / 3)) / 18;
			A.insert(i + j * m, i + (j + 1) * m) = -h / k - hk * (r(x[i] + h / 3, y[j] + 2 * k / 3) +
				r(x[i] - h / 3, y[j] + k / 3)) / 18;
			double fsum = f(x[i] - 2 * h / 3, y[j] - k / 3) + f(x[i] - h / 3, y[j] - 2 * k / 3) +
				f(x[i] + h / 3, y[j] - k / 3);
			fsum += f(x[i] + 2 * h / 3, y[j] + k / 3) + f(x[i] + h / 3, y[j] + 2 * k / 3) +
				f(x[i] - h / 3, y[j] + k / 3);

			b[i + j * m] = -h * k * fsum / 6;

		};
	};
	for (int i = 0; i <= M; i++) {
		int j = 1;
		A.insert(i + (j - 1) * m, i + (j - 1) * m) = 1;
		b[i + (j - 1) * m] = g1(x[i]);
		j = n;
		A.insert(i + (j - 1) * m, i + (j - 1) * m) = 1;
		b[i + (j - 1) * m] = g2(x[i]);
	};
	for (int j = 1; j <= n - 2; j++) {
		int i = 0;
		A.insert(i + j * m, i + j * m) = 1;
		b[i + j * m] = g3(y[j]);
		i = m - 1;
		A.insert(i + j * m, i + j * m) = 1;
		b[i + j * m] = g4(y[j]);
	};
	SparseLU<SparseMatrix<double>> solver;
	solver.compute(A);
	VectorXd v = solver.solve(b);
	write_data(x, y, v);
}
