#pragma once

#include "Eigen/Dense"

using namespace std::complex_literals;

namespace sqs {
	typedef Eigen::Matrix<std::complex<double>, -1, -1> MX;
	typedef Eigen::Matrix<std::complex<double>, 2, 2> M2;
	typedef Eigen::Matrix<std::complex<double>, 4, 4> M4;

	const M2 ZeroMatrix = M2().setZero();
	const M2 EyeMatrix = M2().setIdentity();
	const M2 Hadamard_Matrix = (1.0/sqrt(2.0) * (M2() << 1, 1, 1, -1).finished());
	const M2 PauliXMatrix = ((M2() << 0, 1, 0, 1).finished());
	const M2 PauliYMatrix = ((M2() << 0, -1i, 1i, 0).finished());
	const M2 PauliZMatrix = ((M2() << 1, 0, 0, -1).finished());
}


