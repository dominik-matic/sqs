#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

int main() {
	QCircuit qc(9);
	qc.add(PauliX, 8);
	qc.add(Hadamard, {0, 1, 2, 3, 8});
	

	QComponent oracle;
	QComponent function;
	
	/* A or ~B */
	function.add(PauliX, 0);
	function.add(Toffoli, 0, 1, 4);

	/* A or ~D */
	function.add(Toffoli, 0, 3, 5);
	function.add(PauliX, 0);

	/* B or ~C */
	function.add(PauliX, 1);
	function.add(Toffoli, 1, 2, 6);
	function.add(PauliX, 1);
	
	/* C or ~D */
	function.add(PauliX, 2);
	function.add(Toffoli, 2, 3, 7);
	function.add(PauliX, 2);

	//function.add(CX, 0, 4);
	//function.add(CX, 1, 4);
	//function.add(CX, 0, 5);
	//function.add(CX, 2, 5);
	//function.add(CX, 1, 6);
	//function.add(CX, 3, 6);
	//function.add(CX, 2, 7);
	//function.add(CX, 3, 7);

	oracle.add(function);
	oracle.add(PauliX, {4, 5, 6, 7});
	oracle.add(CU(4, PauliX), {4, 5, 6, 7, 8});
	oracle.add(PauliX, {4, 5, 6, 7});
	oracle.add(function);

	QComponent diffuser;
	diffuser.add(Hadamard, {0, 1, 2, 3});
	diffuser.add(PauliX, {0, 1, 2, 3});
	diffuser.add(CU(3, PauliX), {0, 1, 2, 8});
	diffuser.add(PauliX, {0, 1, 2, 3});
	diffuser.add(Hadamard, {0, 1, 2, 3});
	
	QComponent groverOperator;
	groverOperator.add(oracle);
	groverOperator.add(diffuser);
	groverOperator.setIterations(2);

	qc.add(groverOperator);
	qc.execute();

	qc.measureAndDisplay(1000);
	
	return 0;
}