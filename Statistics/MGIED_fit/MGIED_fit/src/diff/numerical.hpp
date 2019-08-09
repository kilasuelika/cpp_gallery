#ifndef _SCISTAEBD_DIFF_NUMERICAL_
#define _SCISTAEBD_DIFF_NUMERICAL_

#include "..//Eigen/Core"
#include <functional>

namespace SciStaEBD {
	namespace diff {
		using namespace Eigen;
		using namespace std;

		template<typename T1, typename T2>
		T1 ndiff_1d(function<T1(T1, T2*)> fun,
			T1 x0, T2* data = nullptr, int order = 4) {
			T1 grad=0.0;
			switch (order) {
				case 2:
					grad = (fun(x0 + 0.001, data) - fun(x0 - 0.001, data)) / 0.002;
					break;
				case 4:
					grad = (-fun(x0 + 0.002, data) + 8 * fun(x0 + 0.001, data) -
						8 * fun(x0 - 0.001, data) + fun(x0 - 0.002, data)) / 0.012;
					break;
				case 6:
					grad = (fun(x0 + 0.003, data) - 9 * fun(x0 + 0.002, data) + 45 * fun(x0 + 0.001,data)
						- 45*fun(x0 - 0.001, data) + 9 * fun(x0 - 0.002, data) - fun(x0 + 0.003, data)) / 0.06;
				default:
					break;
			};

			return grad;
		};

		template<typename T1, typename T2>
		T1 ndiff_nd(function<double(T1, T2*)> fun, T1 x0,
			T2* data = nullptr, int order = 2) {

			const int n1 = x0.size();
			T1 grad(n1);
			double temp_x;
			for (int i = 0; i < n1; i++) {

				switch (order) {
					case 2: {
						temp_x = x0[i];
						x0[i] = temp_x + 0.001;
						grad[i] = fun(x0, data);
						x0[i] = temp_x - 0.001;
						grad[i] = (grad[i] - fun(x0, data)) / 0.002;
						x0[i] = temp_x;
						break;
					}
					case 4: {
						temp_x = x0[i];
						x0[i] = temp_x + 0.002;
						grad[i] = -fun(x0, data);
						x0[i] = temp_x + 0.001;
						grad[i] = grad[i] + 8 * fun(x0, data);
						x0[i] = temp_x - 0.001;
						grad[i] = grad[i] - 8 * fun(x0, data);
						x0[i] = temp_x - 0.002;
						grad[i] = (grad[i] + fun(x0, data)) / 0.012;
						x0[i] = temp_x;
						break;
					}
					default:
						break;
				};


			};

			return grad;
		};



	};
};

#endif