#ifndef _SCISTA_OPTIMIZATION_COMMON_
#define _SCISTA_OPTIMIZATION_COMMON_

#include "Eigen/Core"
#include <functional>

namespace SciStaEBD {
	namespace optimization {

		using namespace std;
		using namespace Eigen;

		enum Algorithm{
			Q_NEWTON_BFGS
		};
		enum LinearSearch {
			GOLD, GOLD_MOD
		};
		enum HessianAdjust {
			BFGS,LBFGS
		};
		struct Option {
			LinearSearch ls= LinearSearch::GOLD;
			//0: lower bound. 1: upper bound. 2: parameter change tolerance.
			double golden_search_opt[3] = {0.0,1.0, 1E-6};

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
		
		Option default_option;

		struct Solution {
			VectorXd x;
			VectorXd grad;
			MatrixXd hessian;
			double f_val;
			double f_change;
			int n_iter=0;
		};

		inline void MAX_ITER_MESSAGE(int iter) {
			cout << "Maximum iterations " << iter << " reached. However, convergence is still unreachable." << endl;
		}

		inline void OPT_FINISH_MESSAGE(Solution sol) {
			cout << "Optimization finished." << endl;
			cout << "Function value: " << sol.f_val << endl;
			cout << "Function value change: " << sol.f_change << endl;
			cout << "Solution: " << sol.x.transpose() << endl;
		}
	}
}
#endif // !_SCISTA_OPTIMIZATION_COMMON_

