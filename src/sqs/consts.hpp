#pragma once

#include "eigenconsts.hpp"
#include "QOperator.hpp"

namespace sqs {

	const QOperator Eye(EyeMatrix);
	const QOperator Hadamard(Hadamard_Matrix);
	const QOperator PauliX(PauliXMatrix);
	const QOperator PauliY(PauliYMatrix);
	const QOperator PauliZ(PauliZMatrix);
	

}