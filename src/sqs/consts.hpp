#pragma once

#include "eigenconsts.hpp"
#include "QOperator.hpp"

namespace sqs {

	const QOperator Eye(Private::EyeMatrix, "I");
	const QOperator Hadamard(Private::Hadamard_Matrix, "H");
	const QOperator PauliX(Private::PauliXMatrix, "X");
	const QOperator PauliY(Private::PauliYMatrix, "Y");
	const QOperator PauliZ(Private::PauliZMatrix, "Z");
	const QOperator CX(Private::CXMatrix, "CX");
	const QOperator CY(Private::CYMatrix, "CY");
	const QOperator CZ(Private::CZMatrix, "CZ");
	const QOperator Toffoli(Private::ToffoliMatrix, "CU");
}