#ifndef _SCISTAEBD_MATH_MATH_FUN_
#define _SCISTAEBD_MATH_MATH_FUN_
#include <cmath>
#include "../Eigen/Core"

namespace SciStaEBD {
	namespace math {
		using namespace std;
		using namespace Eigen;

		template<typename T>
		inline T logistic(T x, T x0 , T L = 1, T k = 1) {
			return L / (1 + exp(-k * (x - x0)));
		};
		
		inline double logistic(double x) {
			return 0.5 + 0.5 * tanh(x);
		};
		
		inline double logit(double x) {
			return log(x / (1 - x));
		};

		template<typename EigenV>
		inline EigenV softmax(EigenV x) {
			return x / x.array().exp().sum();
		};

		template<typename EigenV>
		inline EigenV sigmoid(EigenV x) {
			return 0.5 * x.tanh() + 0.5;
		}
	}
}
#endif // !_SCISTAEBD_MATH_MATH_FUN_
