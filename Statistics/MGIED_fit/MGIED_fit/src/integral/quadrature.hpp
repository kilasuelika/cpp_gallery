#ifndef _SCISTA_INTEGRAL_QUADRATURE_
#define _SCISTA_INTEGRAL_QUADRATURE_

#include <cmath>
#include "common.hpp"
#include <functional>
#include "../math/math_constant.hpp"

namespace SciStaEBD {
	namespace integral {
		using namespace std;

		template<typename T>
		double quadrature(function<double(T)> fun, double a, double b, int points=50,
			Quadrature_method method= Quadrature_method::Double_exponential) {

			double value;

			switch (method) {
				case Quadrature_method::Double_exponential:{
					if (!isinf(a) && !isinf(b)) {
						value = quadrature_double_exp(fun, a, b, points);
						break;
					};
					if (isinf(a) && !isinf(b)) {
						value = quadrature_double_exp_hinf(fun, points);
						value = -value + quadrature_double_exp(fun, 0, b, points);
						break;
					};
					if (!isinf(a) && isinf(b)) {
						value = quadrature_double_exp_hinf(fun, points)-
							quadrature_double_exp(fun, 0, a, points);
						break;
					};
					if (isinf(a) && isinf(b)) {
						value = quadrature_double_exp(fun, a, b, points);
						break;
					};
				}
				default:
					break;

			};

			return value;
		};

		//Double exponential quadrature for interval [-1,1].
		template<typename T>
		double quadrature_double_exp(function<double(T)> fun, double a=-1, double b=1, int points = 50) {

			const double h = 6.0 / points;
			const int np = points / 2;
			double value = 0.0, temp = 0.0, v=0.0, x;

			for (int i = -np; i <= np; i++) {
				temp = i * h;
				x = tanh(M_HPI * sinh(temp));
				x = (x+1)/2.0*(b-a)+a;
				v = fun(x);

#ifdef _INTEGRAL_CHECK_INF_
				if (isinf(v)) {
					continue;
				}
#endif // _INTEGRAL_CHECK_INF_

				value += v * cosh(temp) / pow(cosh(M_HPI * sinh(temp)), 2);
			}
			value = value * M_HPI * h * (b - a) / 2.0;
			return value;
		};

		//Double exponential quadrature for interval [a,inf).
		template<typename T>
		double quadrature_double_exp_hinf(function<double(T)> fun, int points = 50) {

			const double h = 6.0 / points;
			const int np = points / 2;
			double value = 0.0, ih = 0.0, v = 0.0, x;

			for (int i = -np; i <= np; i++) {
				ih = i * h;
				x = exp(M_HPI * sinh(ih));
				v = fun(x);

#ifdef _INTEGRAL_CHECK_INF_
				if (isinf(v)) {
					continue;
				}
#endif // _INTEGRAL_CHECK_INF_

				value += v *cosh(ih) * exp(M_HPI * sinh(ih));
			};

			value = value * M_HPI * h;
			return value;
		};

		//Double exponential for interval (-inf, inf).
		template<typename T>
		double quadrature_double_exp_inf(function<double(T)> fun, int points = 50) {
			const double h = 2.0 / points;
			const int np = points / 2;
			double value = 0.0, temp = 0.0, v = 0.0, x;

			for (int i = -np; i <= np; i++) {
				ih = i * h;
				x = sinh(M_HPI * sinh(ih));
				v = fun(x);

#ifdef _INTEGRAL_CHECK_INF_
				if (isinf(v)) {
					continue;
				}
#endif // _INTEGRAL_CHECK_INF_

				value += v * cosh(ih) * cosh(M_HPI * sinh(ih));
			};
			value = value * M_HPI * h;
			return value;
		};

	};
};

#endif