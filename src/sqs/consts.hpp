#pragma once

#include "eigenconsts.hpp"
#include "QOperator.hpp"

namespace sqs {

	const QOperator Eye(EyeMatrix, "I");
	const QOperator Hadamard(Hadamard_Matrix, "H");
	const QOperator PauliX(PauliXMatrix, "X");
	const QOperator PauliY(PauliYMatrix, "Y");
	const QOperator PauliZ(PauliZMatrix, "Z");
	const QOperator CX(CXMatrix, "CX");
	const QOperator CY(CYMatrix, "CY");
	const QOperator CZ(CZMatrix, "CZ");
	const QOperator Toffoli(ToffoliMatrix, "T");
}