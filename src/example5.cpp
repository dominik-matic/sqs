#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

int main() {
	QCircuit qc(7);

	qc.add(PauliX, 6);
	qc.add(Hadamard, {0, 1, 2, 6});

	QComponent groverOperator;
	QComponent oracle;
	QComponent diffuser;

	/* A or ~B */
	oracle.add(PauliX, 0);
	oracle.add(Toffoli, 0, 1, 3);
	oracle.add(PauliX, 0);
	
	/* B or C*/
	oracle.add(PauliX, 1, 2);
	oracle.add(Toffoli, 1, 2, 4);
	oracle.add(PauliX, 1, 2);

	/* ~A */
	oracle.add(CX, 0, 5);

	oracle.add(PauliX, 3, 4, 5);
	oracle.add(CU(3, PauliX), {3, 4, 5, 6});
	oracle.add(PauliX, 3, 4, 5);

	/* obrnuti smjer */
	oracle.add(CX, 0, 5);
	oracle.add(PauliX, 1, 2);
	oracle.add(Toffoli, 1, 2, 4);
	oracle.add(PauliX, 1, 2);
	oracle.add(PauliX, 0);
	oracle.add(Toffoli, 0, 1, 3);
	oracle.add(PauliX, 0);
	

	/* difuzer */
	diffuser.add(Hadamard, 0, 1, 2);
	diffuser.add(PauliX, 0, 1, 2);
	diffuser.add(CU(3, PauliX), {0, 1, 2, 6});
	diffuser.add(PauliX, 0, 1, 2);
	diffuser.add(Hadamard, 0, 1, 2);
	
	groverOperator.add(oracle);
	groverOperator.add(diffuser);

	qc.add(groverOperator);
	qc.execute();
	qc.measureAndDisplay(1000, 3);
	
	return 0;
}