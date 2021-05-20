#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

int main2() {
	QCircuit qc(5);

	QComponent function;
	//
	//qc.add(PauliX, 0);
	//qc.add(PauliX, 1);
	//qc.add(PauliX, 0, 1);
	

	/* A or ~B */
	function.add(PauliX, 0);
	function.add(Toffoli, 0, 1, 3);
	function.add(PauliX, 0);

	/* ~A or B */
	function.add(PauliX, 1);
	function.add(Toffoli, 0, 1, 2);
	function.add(PauliX, 1);

	qc.add(function);
	qc.add(PauliX, 2, 3);

	std::cout << "Before toffoli:\n";
	qc.execute();
	qc.measureAndDisplay(100);
	qc.resetQubits();

	std::cout << "after toffoli1:\n";
	qc.add(Toffoli, {2, 3, 4});
	qc.execute();
	qc.measureAndDisplay(100);
	qc.resetQubits();


	qc.add(PauliX, 2, 3);

	std::cout << "After PauliX:\n";
	qc.execute();

	qc.measureAndDisplay(1000);
	
	return 0;
}

int main() {

	QCircuit qc(4);
	
	QComponent qcomp;
	qcomp.add(PauliX, 0, 1);

	//qcomp.add(CX, 2, 3);
	//qcomp.add(CX, 3, 2);
	//qcomp.add(CX, 2, 3);
	
	//qcomp.add(SWAP, 3, 2);
	//qcomp.add(Toffoli, 0, 1, 2);
	//qcomp.add(SWAP, 2, 3);

	qc.add(qcomp);
	qc.execute();
	qc.measureAndDisplay(1000);


	return 0;
}