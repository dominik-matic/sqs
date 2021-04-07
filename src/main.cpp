#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>
#include "sqs/sqs.h"
#include "sqs/Eigen/unsupported/Eigen/KroneckerProduct"

using namespace sqs;

int main() {

	/*QCircuit circuit(1);

	circuit.add(QComponent(Hadamard, 0));
	circuit.add(QComponent(PauliX, 0));

	std::cout << "Quantum bits before applying Hadamard gate:" << std::endl;
	std::cout << circuit.getStateVector() << std::endl;
	
	circuit.execute();
	
	std::cout << "Quantum bits after applying Hadamard gate:" << std:: endl;
	std::cout << circuit.getStateVector() << std::endl;
	
	circuit.reverse();
	circuit.execute();

	std::cout << "Quantum bits after applying Hadamard gate once more:" << std:: endl;
	std::cout << circuit.getStateVector() << std::endl;*/

	MX mat1 = Hadamard_Matrix;
	MX mat2 = EyeMatrix;
	std::cout << mat1 << std::endl << std::endl;
	std::cout << mat2 << std:: endl << std::endl;
	MX mat3 = kroneckerProduct(mat1, mat2);
	std::cout << mat3 << std:: endl;
	
		
	return 0;
}