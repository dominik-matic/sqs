#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>
#include "sqs/sqs.h"

using namespace sqs;

int main() {

	QOperator h = Hadamard;
	QOperator pX = PauliX;
	std::cout << h.getMatrix() << std::endl;
	h *=  1i;
	std::cout << std::endl << h.getMatrix() << std::endl;
	std::cout << h.dimension() << std::endl;
	
	return 0;
}