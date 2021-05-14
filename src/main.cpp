#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;

/*
void superposition() {
	QCircuit circuit(3);
	QComponent qcomp(Hadamard, {0, 1, 2});

	circuit.add(qcomp);
	circuit.execute();
	circuit.measureAndDisplay(1000);

	circuit.resetQubits();

	circuit.add(qcomp);
	circuit.execute();

	std::cout << "\n";
	circuit.measureAndDisplay(1000);
}



void entaglement2q() {
	QCircuit circuit(2);
	QComponent qcomp;

	qcomp.add(Hadamard, 0);
	qcomp.add(CX, 0, 1);

	circuit.add(qcomp);
	circuit.execute();
	circuit.measureAndDisplay(1000);

}

void entaglement3q() {
	QCircuit circuit(3);
	QComponent qcomp;

	qcomp.add(Hadamard, 0);
	qcomp.add(CX, 0, 1);
	qcomp.add(CX, 1, 2);

	circuit.add(qcomp);
	circuit.execute();
	circuit.measureAndDisplay(1000);

}

void Deutsch(int function) {
	QCircuit circuit(2);
	QComponent qcomp;

	qcomp.add(Hadamard, 0);
	qcomp.add(PauliX, 1);
	qcomp.add(Hadamard, 1);

	switch(function) {
		case 0: // f(x) = 0
			break;
		case 1: // f(x) = 1
			qcomp.add(PauliX, 1);
			break;
		case 2: // f(x) = x
			qcomp.add(CX, 0, 1);
			break;
		case 3: // f(x) = ~x
			qcomp.add(CX, 0, 1);
			qcomp.add(PauliX, 1);
			break;
	}

	qcomp.add(Hadamard, 0);

	circuit.add(qcomp);
	circuit.execute();
	circuit.measureAndDisplay(1000);
}

void Grover() {
	QCircuit circuit(2);
	QComponent qcomp;

	qcomp.add(Hadamard, {0, 1});
	qcomp.add(Hadamard, 1);
	qcomp.add(CX, 0, 1);
	qcomp.add(Hadamard, 1);
	qcomp.add(Hadamard, 0, 1);
	qcomp.add(PauliZ, 0, 1);


	qcomp.add(Hadamard, 1);
	qcomp.add(CX, 0, 1);
	qcomp.add(Hadamard, 1);
	qcomp.add(Hadamard, 0, 1);


	circuit.add(qcomp);
	circuit.execute();
	circuit.measureAndDisplay(1000);
}

int main() {
	std::cout << "Superposition:\n";
	superposition();

	std::cout << "\nEntaglement with 2 qubits:\n";
	entaglement2q();

	std::cout << "\nEntaglement with 3 qubits:\n";
	entaglement3q();


	std::cout << "Deutsch algorithm returns x0 for const functions and x1 for balanced functions\n";
	std::cout << "\nDeutsch algorithm for checking f(x) = 0:\n";
	Deutsch(0);

	std::cout << "\nDeutsch algorithm for checking f(x) = 1:\n";
	Deutsch(1);

	std::cout << "\nDeutsch algorithm for checking f(x) = x:\n";
	Deutsch(2);

	std::cout << "\nDeutsch algorithm for checking f(x) = ~x:\n";
	Deutsch(3);

	std::cout << "Groverov algoritam, traži 11:\n";
	Grover();

	return 0;
}
*/

void GroverSAT() {
	QCircuit qc(9);
	qc.add(Hadamard, {0, 1, 2, 3});
	qc.add(PauliX, 8);
	qc.add(Hadamard, 8);

	QComponent oracle;
	QComponent function;
	function.add(CX, 0, 4);
	function.add(CX, 1, 4);
	function.add(CX, 0, 5);
	function.add(CX, 2, 5);
	function.add(CX, 1, 6);
	function.add(CX, 3, 6);
	function.add(CX, 2, 7);
	function.add(CX, 3, 7);

	oracle.add(function);
	oracle.add(CU(4, PauliX), {4, 5, 6, 7, 8});
	oracle.add(function);

	QComponent diffuser;
	diffuser.add(Hadamard, {0, 1, 2, 3});
	diffuser.add(PauliX, {0, 1, 2, 3});
	diffuser.add(CU(3, PauliZ), {1, 2, 3, 0});
	diffuser.add(PauliX, {0, 1, 2, 3});
	diffuser.add(Hadamard, {0, 1, 2, 3});
	
	QComponent groverOperator;
	groverOperator.add(oracle);
	groverOperator.add(diffuser);
	groverOperator.setIterations(2);

	qc.add(groverOperator);
	qc.execute();

	qc.measureAndDisplay(1000, 4);
}

int QPE() {

	QCircuit qc(4);
	qc.add(PauliX, 3);
	qc.add(Hadamard, 0, 1, 2);

	QOperator ph = Phase(1.0); // theta =  1/2
	auto cPh = CU(1, ph);

	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < pow(2, i); ++j) {
			qc.add(cPh, i, 3);
		}
	}

	qc.add(QFTDagger(3), {0, 1, 2});
	qc.execute();
	
	///for(auto a : qc.probabilityVector()) {std::cout << a << " \\ "; }std::cout <<"\n";
	
	auto m = qc.measure(1000, 3);
	for(auto a : m) {
		std::cout << a.first << "/8" << "\tp: " << a.second / 10.0 << "%\n";  
	}


	return 0;
}

int phases() {
	QCircuit qc(2);
	qc.add(PauliX, 0, 1);
	qc.execute();
	std::cout << qc.getStateVector() << "\n";

	qc.add(CU(1, Phase(1.0 / 2.0)), 0, 1);


	qc.resetQubits();
	qc.execute();
	std::cout << "\n" <<qc.getStateVector() << "\n";



	return 0;
}

int main() {
	GroverSAT();
	return 0;
}