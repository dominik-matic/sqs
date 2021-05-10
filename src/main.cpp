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

int main2() {
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

int main3() {
	QCircuit qc1(2);
	QCircuit qc2(2);
	QCircuit qc3(2);
	QCircuit qc4(2);

	QComponent qcomp1;
	QComponent qcomp2;
	QComponent qcomp3;
	QComponent qcomp4;

	qcomp2.add(PauliX, 0);
	qcomp3.add(PauliX, 1);
	qcomp4.add(PauliX, 0, 1);

	qcomp1.add(Hadamard, 0, 1);
	qcomp2.add(Hadamard, 0, 1);
	qcomp3.add(Hadamard, 0, 1);
	qcomp4.add(Hadamard, 0, 1);


	qcomp1.add(CX, 0, 1);
	qcomp2.add(CX, 0, 1);
	qcomp3.add(CX, 0, 1);
	qcomp4.add(CX, 0, 1);

	qc1.add(qcomp1);
	qc2.add(qcomp2);
	qc3.add(qcomp3);
	qc4.add(qcomp4);
	
	qc1.execute();
	qc2.execute();
	qc3.execute();
	qc4.execute();
	
	std::cout << qc1.getStateVector() << std::endl << std::endl;
	std::cout << qc2.getStateVector() << std::endl << std::endl;
	std::cout << qc3.getStateVector() << std::endl << std::endl;
	std::cout << qc4.getStateVector() << std::endl << std::endl;

	QCircuit qcplusplus(2);
	QCircuit qcplusminus(2);
	QCircuit qcminusplus(2);
	QCircuit qcminusminus(2);

	QComponent comp1;
	QComponent comp2;
	QComponent comp3;
	QComponent comp4;

	comp2.add(PauliX, 0);
	comp3.add(PauliX, 1);
	comp4.add(PauliX, 0, 1);

	comp1.add(Hadamard, 0, 1);
	comp2.add(Hadamard, 0, 1);
	comp3.add(Hadamard, 0, 1);
	comp4.add(Hadamard, 0, 1);
	
	qcplusplus.add(comp1);
	qcplusminus.add(comp2);
	qcminusplus.add(comp3);
	qcminusminus.add(comp4);
	
	qcplusplus.execute();
	qcplusminus.execute();
	qcminusplus.execute();
	qcminusminus.execute();

	std::cout << "|++>: " << std::endl;
	std::cout << qcplusplus.getStateVector() << std::endl;
	std::cout << "|+->: " << std::endl;
	std::cout << qcplusminus.getStateVector() << std::endl;
	std::cout << "|-+>: " << std::endl;
	std::cout << qcminusplus.getStateVector() << std::endl;
	std::cout << "|-->: " << std::endl;
	std::cout << qcminusminus.getStateVector() << std::endl;
	
	
	
	
	return 0;
}

int main7() {
	QCircuit qc1(2);
	QCircuit qc2(2);
	QCircuit qc3(2);
	QCircuit qc4(2);

	QComponent qcomp1;
	QComponent qcomp2;
	QComponent qcomp3;
	QComponent qcomp4;

	qcomp2.add(PauliX, 0);
	qcomp3.add(PauliX, 1);
	qcomp4.add(PauliX, 0, 1);

	qcomp1.add(CX, 0, 1);
	qcomp2.add(CX, 0, 1);
	qcomp3.add(CX, 0, 1);
	qcomp4.add(CX, 0, 1);

	qc1.add(qcomp1);
	qc2.add(qcomp2);
	qc3.add(qcomp3);
	qc4.add(qcomp4);

	qc1.execute();
	qc2.execute();
	qc3.execute();
	qc4.execute();
	
	std::cout << "CNOT|00> = " << std::endl;
	qc1.measureAndDisplay(100);
	std::cout << "CNOT|01> = " << std::endl;
	qc2.measureAndDisplay(100);
	std::cout << "CNOT|10> = " << std::endl;
	qc3.measureAndDisplay(100);
	std::cout << "CNOT|11> = " << std::endl;
	qc4.measureAndDisplay(100);


	return 0;
	

}


int main() {
	QCircuit qc(2);
	QComponent qcomp;

	qcomp.add(PauliX, 0, 1);
	qcomp.add(Hadamard, 1);
	//qcomp.add(CX, 0, 1);

	qc.add(qcomp);
	qc.execute();
	std::cout << qc.getStateVector() << std::endl;

}