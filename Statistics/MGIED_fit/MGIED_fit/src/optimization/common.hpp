#ifndef _SCISTA_OPTIMIZATION_COMMON_
#define _SCISTA_OPTIMIZATION_COMMON_

#include "../Eigen/Core"
#include <functional>
#include <iostream>
#include "..//diff/numerical.hpp"

namespace SciStaEBD {
	namespace optimization {

		using namespace std;
		using namespace Eigen;
		using namespace SciStaEBD::diff;
		using namespace std::placeholders;

		enum Algorithm{
			Q_NEWTON_BFGS
		};
		enum LinearSearch {
			GOLD, LINEAR
		};
		enum HessianAdjust {
			BFGS,LBFGS
		};

		struct OptAlgoOption {
			LinearSearch ls= LinearSearch::GOLD;
			//0: lower bound. 1: upper bound. 2: parameter change tolerance.
			double golden_search_opt[3] = {0.0,1.0, 1E-6};
			//0: upper bound. 1: upper bound. 2: step size. 3: mimimum length. If targeted alpha
			// is smaller than it, use a small step length. 5: minimum lenght scale.
			double linear_opt[5] = {0.9};
			HessianAdjust ha = HessianAdjust::BFGS;
			
			
			//Tolerance for norm of gradient.
			double g_epsilon = 1E-6;
			//Tolerance for change of function values.
			double f_epsilon = 1E-5;
			int max_iter = 1000;

			//print level.
			//0: no print. 1: function values.
			int print_level = 1;
		};
		
		OptAlgoOption default_option;

		struct Solution {
			VectorXd x;
			VectorXd grad;
			MatrixXd hessian;
			double f_val;
			double f_change;
			int n_iter=0;
		};

		inline void MAX_ITER_MESSAGE(int iter) {
			cout << "Maximum iterations " << iter 
				<< " reached. However, convergence is still unreachable." << endl;
		}

		inline void OPT_FINISH_MESSAGE(Solution sol) {
			cout << "Optimization finished." << endl;
			cout << "Function value: " << sol.f_val << endl;
			cout << "Function value change: " << sol.f_change << endl;
			cout << "Solution: " << sol.x.transpose() << endl;
		};

		//struct for solver options.
		enum SolveMethod {
			NEWTON, SECANT
		};
		enum SolveExitCode {
			MAX_ITER_REACHED,
			SUCCESSFUL,
			SOLUTION_DOESNOT_CHANGE,
		};
		struct SolveOption {
			SolveMethod method=SolveMethod::NEWTON;
			int max_iter = 100;
			double f_tol = 1e-10;
			double x_change_tol = 1e-10;
			int diff_order = 4;
			bool report_result=true;

		};
		SolveOption default_solve_option;

		template<typename T>
		struct SolveSolution {
			T x;
			T f_val;
			T f_change;
			T x_change;
			SolveExitCode exit;
			int n_iter = 0;

		};
	}
}
#endif // !_SCISTA_OPTIMIZATION_COMMON_

