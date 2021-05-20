#include "sqs/sqs.h"

using namespace sqs;

void Deutsch(int i) {
	QCircuit qc(2);

	qc.add(PauliX, 1);
	qc.add(Hadamard, 0, 1);

	switch(i) {
		case 0: // f(x) = 0
			break;
		case 1: // f(x) = 1
			qc.add(PauliX, 1);
			break;
		case 2: // f(x) = x
			qc.add(CX, 0, 1);
			break;
		case 3: // f(x) = ~x
			qc.add(CX, 0, 1);
			qc.add(PauliX, 1);
			break;
		default:
			return;
	}

	qc.add(Hadamard, 0);

	qc.execute();
	qc.measureAndDisplay(1000);
}

int main() {

	std::string funcs[] = {"f(x) = 0", "f(x) = 1", "f(x) = x", "f(x) = ~x"};

	for(int i = 0; i < 4; ++i) {
		std::cout << funcs[i] << std::endl;
		Deutsch(i);
	}

	return 0;
}