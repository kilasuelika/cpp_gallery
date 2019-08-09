#ifndef _SCISTA_OPTIMIZATION_OPTIMIZE_
#define _SCISTA_OPTIMIZATION_OPTIMIZE_

#include "quasi_newton.hpp"

namespace SciStaEBD {
	namespace optimization {

		//Main interface. T for external data type.
		template<typename T>
		void optimize(function<double(VectorXd, T)> fun,
			VectorXd& x0, T& data = nullptr,
			function<VectorXd(VectorXd, T)> grad = nullptr,
			function<MatrixXd(VectorXd, T)> hessian = nullptr,
			Algorithm algo = Algorithm::Q_NEWTON_BFGS,
			OptAlgoOption opt = default_option) {

		}
	}
}
#endif // !_SCISTA_OPTIMIZATION_OPTIMIZE_
