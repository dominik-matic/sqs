#include "sqs/sqs.h"

using namespace sqs;

int main() {
	QCircuit qc(3);

	qc.add(Hadamard, 0);

	qc.add(CX, 0, 1);
	qc.add(CX, 1, 2);

	qc.execute();
	qc.measureAndDisplay(1000);
	
	return 0;
}