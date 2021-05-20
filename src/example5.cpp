#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

int main() {
	QCircuit qc(5);
	qc.add(PauliX, 4);
	qc.add(Hadamard, {0, 1, 4});
	

	QComponent oracle;
	QComponent function;
	
	/* ~A or B */
	function.add(PauliX, 1);
	function.add(Toffoli, 2, 1, 0);
	function.add(PauliX, 1);
	
	/* A or ~B */
	function.add(PauliX, 0);
	function.add(Toffoli, 3, 1, 0);
	function.add(PauliX, 0);

	oracle.add(function);
	oracle.add(PauliX, 2, 3);
	oracle.add(CU(2, PauliX), 4, 3, 2);
	oracle.add(PauliX, 2, 3);
	oracle.add(function);

	QComponent diffuser;
	diffuser.add(Hadamard, 0, 1);
	diffuser.add(PauliX, 0, 1);
	diffuser.add(CU(1, PauliZ), 0, 1);
	diffuser.add(PauliX, 0, 1);
	diffuser.add(Hadamard, 0, 1);
	
	QComponent groverOperator;
	groverOperator.add(oracle);
	groverOperator.add(diffuser);
	groverOperator.setIterations(1);

	qc.add(groverOperator);
	qc.execute();

	qc.measureAndDisplay(1000);
	
	return 0;
}