#ifndef _SCISTAEBD_OPTIMIZATION_SOLVE_
#define _SCISTAEBD_OPTIMIZATION_SOLVE_

#include "common.hpp"
#include <stdexcept>
#include <cmath>
namespace SciStaEBD {
	namespace optimization {
		using namespace std;

		template<typename T>
		void check_solution(SolveSolution<T> & solution, SolveOption& option) {
			//Calculate exit code.
			if (abs(solution.f_val)<=option.f_tol) {
				solution.exit = SolveExitCode::SUCCESSFUL;
			}
			else if (abs(solution.x_change) <= option.x_change_tol) {
				solution.exit = SolveExitCode::SOLUTION_DOESNOT_CHANGE;
			}
			else {
				solution.exit = SolveExitCode::MAX_ITER_REACHED;
			};

			//Print result.
			if (option.report_result) {
				switch (solution.exit) {
					case SolveExitCode::SUCCESSFUL: {
						cout << "Algorithm successfully stops after "
							<< solution.n_iter << " iterations." << endl;
						cout << "Solution: " << solution.x << endl;
						cout << "Function value: " << solution.f_val << endl;
						break;
					}
					case SolveExitCode::SOLUTION_DOESNOT_CHANGE: {
						cout << "Solution change is close to 0. However function value is not close to 0. More iterations may help converge." << endl;
						cout << "Current solution: " << solution.x << endl;
						cout << "Current function value: " << solution.f_val << endl;
						break;
					}
					case SolveExitCode::MAX_ITER_REACHED: {
						cout << "Maximum " << option.max_iter << " iterations reached. However solution still has large change. Algorithm may diverge.";
						break;
					}
					default:
						break;

				};

			};
		};

		template<typename T1, typename T2>
		SolveSolution<T1> solve_1d(function<T1(T1, T2*)> fun, T1 x0,
			T2* data = nullptr,
			function<T1(T1,T2*)> grad_fun = nullptr,
			SolveOption option = default_solve_option) {

			SolveSolution<T1> solution;

			if (grad_fun == nullptr) {
				//auto g_fun= bind(&ndiff_1d<T1, T2>, fun, _2, _3, option.diff_order);
				grad_fun = bind(&ndiff_1d<T1, T2>, fun, _1, _2, option.diff_order);
			};

			switch (option.method) {
				case SolveMethod::NEWTON: {
					solution = solve_1d_newton<T1, T2>(fun, x0, grad_fun, data, option);
					break;

				}
				default:
					break;
			};

			return solution;
		};

		template<typename T1, typename T2>
		SolveSolution<T1> solve_1d_newton(function<T1(T1, T2*)> fun, T1 x0,
			function<T1(T1, T2*)> grad_fun,
			T2* data = nullptr,
			SolveOption option = default_solve_option) {

			SolveSolution<T1> solution;
			int iter = 1;
			double x_change = 0.0, f_old = fun(x0, data), f_new = 0.0, f_change = 0.0;
			for (; iter <= option.max_iter; iter++) {

				x_change = fun(x0, data) / grad_fun(x0, data);
				x0 = x0 - x_change;

				f_new = fun(x0, data);
				f_change = f_new - f_old;
				f_old = f_new;

				if (abs(f_old) < option.f_tol) {
					break;
				};
			};

			solution.x = x0;
			solution.f_change = f_change;
			solution.f_val = f_old;
			solution.n_iter = iter;
			solution.x_change = x_change;

			check_solution<T1>(solution, option);

			return solution;
		};

		template<typename T>
		double solve_bisect(function<double(double, T*)> fun, 
			double lower, double upper, T* data=nullptr, 
			double tol=1e-13, int maxiter=100) {

			//Check sign at bounds.
			double lfunv = fun(lower, data), rfunv = fun(upper, data);

			if (abs(lfunv) < tol) {
				return lower;
			};
			if (rfunv < tol) {
				return upper;
			};
			if ( lfunv*rfunv > 0) {
				throw invalid_argument("Equal function sign at lower and upper bounds. No solution.");
			};

			int niter = 1;
			double mval= lower + (upper - lower) / 2, 
				mfunv = fun(lower+(upper-lower)/2, data);

			while (abs(mfunv) > tol) {
				if (lfunv * mfunv < 0) {
					upper = mval;
				}
				else {
					lower = mval;
				};

				mval = lower + (upper - lower) / 2;
				mfunv = fun(mval, data);

				niter++;
				if (niter >= maxiter) {
					throw invalid_argument("Max iteration reached.");
					break;
				};

				
			};
			return mval;
		}
	};
};

#endif