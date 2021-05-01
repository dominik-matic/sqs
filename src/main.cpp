#include <iostream>
#include "sqs/sqs.h"

using namespace sqs;


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
	/***/
	qcomp.add(Hadamard, 1);
	qcomp.add(CX, 0, 1);
	qcomp.add(Hadamard, 1);
	qcomp.add(Hadamard, 0, 1);
	/***/

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