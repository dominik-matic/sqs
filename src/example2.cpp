#include "sqs/sqs.h"

using namespace sqs;

int main() {
	QCircuit qc(2);

	/* dodavanje Hadamardovog operatora na qubit 0 */
	qc.add(Hadamard, 0);

	/* dodavanje CNOT operatora gdje je qubit 0 upravljacki,
	a 1 ciljni*/
	qc.add(CX, 0, 1);

	qc.execute();
	qc.measureAndDisplay(1000);
	
	return 0;
}