#ifndef _SCISTA_OPTIMIZATION_QUASI_NEWTON_
#define _SCISTA_OPTIMIZATION_QUASI_NEWTON_

#include "common.hpp"

namespace SciStaEBD {
	namespace optimization {

		using namespace Eigen;
		using namespace SciStaEBD::diff;

		template<typename T>
		double golden(function<double(VectorXd, T*)> fun,
			VectorXd& x, T* data = nullptr,
			function<VectorXd(VectorXd, T*)> grad = nullptr,
			OptAlgoOption opt = default_option) {

			double a = opt.golden_search_opt[0];
			double b = opt.golden_search_opt[1];
			double c_tol = opt.golden_search_opt[2];

			VectorXd d = -grad(x, data);

			double lambda = a + 0.382 * (b - a), mu = a + 0.618 * (b - a);
			double fl = fun(x + lambda * d, data), 
				fu = fun(x + mu * d, data);

			while (true) {
				if (fl > fu) {
					if (b - lambda <= c_tol) {
						return mu;
					};
					a = lambda;
					lambda = mu;
					fl = fun(x + mu * d, data);
					mu = a + 0.618 * (b - a);
					fu = fun(x + mu * d, data);
				}
				else {
					if (mu - a <= c_tol) {
						return lambda;
					};
					b = mu;
					mu = lambda;
					fu = fun(x + lambda * d, data);
					lambda = a + 0.382 * (b - a);
					fl = fun(x + lambda * d, data);
				};
			};

			return 0.1;
		};

		//Linear search method will be very slow when x is close to the best solution.
		//So better use it to first find a prior point then change to other method.
		template<typename T>
		double linear_backtrack(function<double(VectorXd, T*)> fun,
			VectorXd& x, T* data = nullptr,
			function<VectorXd(VectorXd, T*)> grad = nullptr,
			OptAlgoOption opt = default_option) {

			double scale = opt.linear_opt[0];
			VectorXd d = -grad(x, data);
			double alpha = 1/d.norm();
			double start_fun_val = fun(x, data);

			//When norm of gradient is small, we use a small step length.
			if (alpha > 1000) {
				alpha = 1;
			};

			for (; alpha > 0;) {

				if (fun(x + alpha * d, data) < start_fun_val) {
					break;
				}
				else {
					alpha = alpha * scale;

				};
			};

			return alpha;
		};

		template<typename T>
		Solution quasi_newton(function<double(VectorXd, T*)> fun,
			VectorXd& x0, T* data = nullptr,
			function<VectorXd(VectorXd, T*)> grad = nullptr,
			OptAlgoOption opt = default_option) {

			if (grad == nullptr) {
				grad = bind(ndiff_nd<VectorXd,T>,fun,_1,_2,2);
			};

			const int nvar = x0.size();
			const MatrixXd IMat = MatrixXd::Identity(nvar, nvar);

			Solution sol;
			sol.x = x0;
			sol.grad = grad(x0, data);
			sol.hessian = IMat;
			sol.f_val = fun(sol.x, data);

			double alpha, sty, of_val;
			VectorXd sk, yk, gk1, dk;

			while (sol.grad.norm() >= opt.g_epsilon) {
				
				dk = -sol.hessian * sol.grad;

				switch (opt.ls) {
					case LinearSearch::GOLD: {
						alpha = golden(fun, sol.x, data, grad, opt);
						break;
					}
					case LinearSearch::LINEAR: {
						alpha = linear_backtrack(fun, sol.x, data, grad, opt);

						break;
					}
					default: {
						alpha = 0.5;
						break;
					}
				};

				sk = alpha * dk;
				gk1 = grad(sol.x + sk, data);
				yk = gk1 - sol.grad;

				//Update solution.
				sol.grad = gk1;
				sol.x = sol.x + sk;
				of_val = sol.f_val;
				sol.f_val = fun(sol.x, data);
				sol.f_change = sol.f_val - of_val;
				++sol.n_iter;

				switch (opt.print_level) {
					case 1: {
						cout << "The " << sol.n_iter << "th iteration. function value: " << sol.f_val << endl;
					}
					default: {
						break;
					}
				};


				//Adjust hessian.
				switch (opt.ha) {
					case HessianAdjust::BFGS: {
						sty = sk.transpose() * yk;
						sol.hessian = (IMat - sk * yk.transpose() / sty) * sol.hessian *
							(IMat - yk * sk.transpose() / sty) + sk * sk.transpose() / sty;
						break;
					}
					default: {
						break;
					}
				};

				//Maximum iteration reaches.
				if (sol.n_iter >= opt.max_iter) {
					MAX_ITER_MESSAGE(opt.max_iter);
					OPT_FINISH_MESSAGE(sol);
					return sol;
				} else {
					
				};

			};

			OPT_FINISH_MESSAGE(sol);
			return sol;

		};
	};

};
#endif