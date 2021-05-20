#include "sqs/sqs.h"

using namespace sqs;

int main() {
	// stvaranje kvantnog logičkog kruga
	QCircuit qc(4);

	// dodavanje Hadamardovog operatora na sve bitove
	qc.add(Hadamard, {0, 1, 2, 3});

	// pokretanje simulatora i mjerenje stanja na kraju 1000 puta
	qc.execute();
	qc.measureAndDisplay(1000);
	
	return 0;
}