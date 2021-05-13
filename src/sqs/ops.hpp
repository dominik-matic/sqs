#pragma once

#include "eigenconsts.hpp"
#include "QOperator.hpp"

namespace sqs {

	const QOperator Eye(Private::EyeMatrix, "I");
	const QOperator Hadamard(Private::HadamardMatrix, "H");
	const QOperator PauliX(Private::PauliXMatrix, "X");
	const QOperator PauliY(Private::PauliYMatrix, "Y");
	const QOperator PauliZ(Private::PauliZMatrix, "Z");
	const QOperator CX(Private::CXMatrix, "CX");
	const QOperator CY(Private::CYMatrix, "CY");
	const QOperator CZ(Private::CZMatrix, "CZ");
	const QOperator Toffoli(Private::ToffoliMatrix, "CU");
	const QOperator SWAP(Private::SWAPMatrix, "SWAP");

	QOperator CU(size_t controls, QOperator unitary) {
		unsigned int unitary_qbits = std::log2(unitary.dimension());
		unsigned int nqubits = controls + unitary_qbits;
		unsigned int dim = std::pow(2, nqubits);
		MX matrix;
		matrix.setIdentity(dim, dim);
		matrix.bottomRightCorner(unitary.dimension(), unitary.dimension()) = unitary.getMatrix();
		QOperator qop(matrix, "CU");
		return qop;
	}

	QOperator Phase(double phase) {
		double pi = std::acos(-1);
		std::complex<double> i(0, 1);
		M2 matrix = ((M2() << 	1, 0, 0, std::exp(i * pi * phase)).finished());
		QOperator qop(matrix, "P");
		return qop;
	}

	QOperator QFT(unsigned int qubits) {
		double pi = std::acos(-1);
		std::complex<double> im(0, 1);
		unsigned int dim = std::pow(2, qubits);
		MX matrix(dim, dim);
		for(int i = 0; i < dim; ++i) {
			for(int j = 0; j < dim; ++j) {
				matrix(i, j) = std::exp(2.0 * im * pi * (double) i * (double) j / (double) dim);
			}
		}
		
		QOperator qop(matrix, "?");
		return qop;
	}

	QOperator QFTDagger(unsigned int qubits) {
		auto matrix = QFT(qubits).getMatrix();
		matrix.adjointInPlace();
		QOperator qop(matrix, "?");
		return qop;
	}

}