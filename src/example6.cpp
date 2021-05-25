#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

int main2() {

	QCircuit qc(5);

	qc.add(Hadamard, {0, 1, 2, 3});
	qc.add(PauliX, 4);














	return 0;
}

int main() {
	QCircuit qc(4);
	qc.add(PauliX, 3);
	qc.add(Hadamard, {0, 1, 2});

	QOperator ph = Phase(0.5); /* pi/2 = 2pi * (1/4) */
	QOperator cPh = CU(1, ph);

	qc.add(cPh, 0, 3);
	qc.add(cPh, 0, 3);
	qc.add(cPh, 0, 3);
	qc.add(cPh, 0, 3);
	qc.add(cPh, 1, 3);
	qc.add(cPh, 1, 3);
	qc.add(cPh, 2, 3);


	qc.add(QFTDagger(3), {0, 1, 2});
	qc.execute();

	auto m = qc.measure(1000, 3);
	for(auto a : m) {
		std::cout << a.first << "/8" << "\tp: " << a.second / 10.0 << "%\n";  
	}


	return 0;

}