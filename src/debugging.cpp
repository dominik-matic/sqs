#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

int main() {
	QCircuit qc(6);

	qc.add(PauliX, 5);
	qc.add(Hadamard, {0, 1, 2, 5});
	
	// 00
	//qc.add(PauliX, 0); // 01
	//qc.add(PauliX, 1); // 10
	//qc.add(PauliX, 0, 1);	// 11
	
	QComponent groverOperator;
	//QComponent oracle;
	QComponent function;
	QComponent diffuser;

	/* A or ~B */
	//function.add(PauliX, 0);
	//function.add(Toffoli, 0, 1, 3);
	//function.add(PauliX, 0);

	///* ~A or B */
	//function.add(PauliX, 1);
	//function.add(Toffoli, 0, 1, 2);
	//function.add(PauliX, 1);

	function.add(PauliX, 0, 1);
	function.add(Toffoli, 0, 1, 3);
	function.add(PauliX, 0, 1);
	
	function.add(CX, 1, 4);

	function.add(PauliX, 3, 4);
	function.add(Toffoli, 3, 4, 5);
	function.add(PauliX, 3, 4);


	function.add(CX, 1, 4);

	function.add(PauliX, 0, 1);
	function.add(Toffoli, 0, 1, 3);
	function.add(PauliX, 0, 1);
	
	diffuser.add(Hadamard, 0, 1, 2);
	diffuser.add(PauliX, 0, 1, 2);
	diffuser.add(CU(3, PauliX), {0, 1, 2, 5});
	diffuser.add(PauliX, 0, 1, 2);
	diffuser.add(Hadamard, 0, 1, 2);
	

	groverOperator.add(function);
	groverOperator.add(diffuser);

	qc.add(groverOperator);
	qc.execute();
	qc.measureAndDisplay(1000);
	
	return 0;
}

//int main() {
//
//	QCircuit qc(5);
//
//	qc.add(PauliX, 0, 1, 4);
//	qc.add(Hadamard, 4);
//	//qc.add(PauliX, 0);
//	//qc.add(Hadamard, 0, 1, 4);
//	
//	QComponent oracle;
//	QComponent function;
//
//	/* A or ~B */
//	function.add(PauliX, 0);
//	function.add(Toffoli, 0, 1, 3);
//	function.add(PauliX, 0);
//
//	/* ~A or B */
//	function.add(PauliX, 1);
//	function.add(Toffoli, 0, 1, 2);
//	function.add(PauliX, 1);
//
//	oracle.add(function);
//	oracle.add(PauliX, 2, 3);
//	oracle.add(Toffoli, {2, 3, 4});
//	oracle.add(PauliX, 2, 3);
//	oracle.add(function);
//
//	qc.add(oracle);
//	qc.execute();
//
//	auto sv = qc.getStateVector();
//	for(int i = 0; i < sv.size(); ++i) {
//		std::cout << i << ": " << sv(i) << "\n";
//	}
//	qc.measureAndDisplay(1000);
//
//
//	return 0;
//}