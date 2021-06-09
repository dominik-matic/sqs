#include "sqs/sqs.h"

using namespace sqs;

int main() {

	QCircuit qc(2);
	qc.add(Hadamard, 0, 1);
	qc.add(CX, 0, 1);
	qc.execute();
	qc.measureAndDisplay(1000);

	return 0;
}