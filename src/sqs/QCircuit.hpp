#pragma once

#include <list>
#include <map>
#include <cmath>
#include <random>
#include "QComponent.hpp"
#include "eigenconsts.hpp"

namespace sqs {

	class QCircuit {
		private:
		struct Stage {
			std::set<unsigned int> usedQubits;
			std::set<unsigned int> lockedQubits;
			std::list<std::pair<QComponent, std::vector<unsigned int>>> qcomps;
		};
		std::list<Stage> stages;
		VX qvector;
		unsigned int qubits;

		inline std::vector<unsigned int> getStageRange(Stage& stage) {
			std::vector<unsigned int> result;
			for(auto it = stage.usedQubits.begin(); it != stage.usedQubits.end(); ++it) {
				result.push_back(*it);
			}
			std::sort(result.begin(), result.end());
			return result;
		}


		inline void addQCompToStage(QComponent &qcomp, std::vector<unsigned int> &qubitRange, Stage &stage) {
			for(auto qPos : qubitRange) {
				stage.usedQubits.insert(qPos);
				stage.lockedQubits.insert(qPos);
			}
			stage.qcomps.push_back(std::make_pair(qcomp, qubitRange));
		}

		inline MX EyeToCrossedPower(unsigned int power) {
			MX returnMatrix = EyeMatrix;
			MX tempMatrix = EyeMatrix;
			for(size_t i = 1; i < power; ++i) {
				returnMatrix = kroneckerProduct(returnMatrix, tempMatrix).eval();
			}
			return returnMatrix;
		}

		inline std::string unsignedToBin(unsigned int num) {
			std::string bin = "";
			for(size_t i = 0; i < qubits; ++i) {
				if(num % 2 == 0) {
					bin = bin + '0';
				} else {
					bin = bin + '1';
				}
				num /= 2;
			}
			return bin;
		}




		public:
		QCircuit(unsigned int qubits) : qubits(qubits) {
			
			int vecLen =  int(std::pow(2.0, qubits));
			qvector.resize(vecLen);
			qvector.setZero();
			qvector(0) = 1;									// initialize to |00...00>
		}

		inline void add(QComponent qcomp) {
			auto qubitRange = qcomp.getQubitRange();
			bool added = false;
			for(auto& stage : stages) {
				bool eligible = true;
				for(auto qPos : qubitRange) {
					auto it = stage.lockedQubits.find(qPos);
					if(it != stage.lockedQubits.end()) {
						eligible = false;
						for(auto pos : qubitRange) { stage.lockedQubits.insert(pos); }
						break;
					}
				}
				if(eligible) {
					addQCompToStage(qcomp, qubitRange, stage);
					added = true;
					break;
				}
			}
			if(!added) {
				Stage newStage;
				addQCompToStage(qcomp, qubitRange, newStage);
				stages.push_back(newStage);
			}
			
		}

		inline void reverse() {
			stages.reverse();
			for(auto stage : stages) {
				stage.qcomps.reverse();
			}
		}

		inline void execute() {
			MX resultingMatrix;
			unsigned int dimension = int(std::pow(2, qubits));
			resultingMatrix.setIdentity(dimension, dimension);
			for(auto stageIt = stages.begin(); stageIt != stages.end(); ++stageIt) {				
				auto stageRange = getStageRange(*stageIt);
				unsigned int stageLowestQubit = stageRange.front();
				unsigned int stageHighestQubit = stageRange.back();
				stageIt->qcomps.sort([](auto const& a, auto const& b){
					return a.second[0] < b.second[0];
				});

				MX tempMatrix;
				unsigned int currentLower;
				unsigned int prevUpper;
				unsigned int difference;
				auto qcompIt = stageIt->qcomps.begin();
				if(stageLowestQubit != 0) {
					tempMatrix = EyeToCrossedPower(stageLowestQubit);
					prevUpper = stageLowestQubit - 1;
				} else {
					tempMatrix = qcompIt->first.calculateMatrix();
					prevUpper = qcompIt->second.back();
					++qcompIt;
				}

				for(; qcompIt != stageIt->qcomps.end(); ++qcompIt) {
					currentLower = qcompIt->second.front();
					if(currentLower != (prevUpper + 1)) {
						difference = currentLower - prevUpper - 1;
						tempMatrix = kroneckerProduct(tempMatrix, EyeToCrossedPower(difference)).eval();
					}
					tempMatrix = kroneckerProduct(tempMatrix, qcompIt->first.calculateMatrix()).eval();
					prevUpper = qcompIt->second.back();
				}

				if(prevUpper != (qubits - 1)) {
					difference = (qubits - 1) - prevUpper;
					tempMatrix = kroneckerProduct(tempMatrix, EyeToCrossedPower(difference)).eval();
				}
				resultingMatrix = tempMatrix * resultingMatrix;
			}
			qvector = resultingMatrix * qvector;
		}

		inline void resetQubits() {
			qvector.setZero();
			qvector(0) = 1;
		}

		inline std::vector<double> probabilityVector() const {
			unsigned int dimension = qvector.size();
			std::vector<double> probVect(dimension);

			for(unsigned int i = 0; i < dimension; ++i) {
				auto abs = std::abs(qvector(i));
				probVect[i] = double(abs * abs);
			}
			return probVect;
		}

		inline std::map<unsigned int, unsigned int> measure(unsigned int times) const {
			auto probs = probabilityVector();
			double sum = 0;
			for(auto p = probs.begin(); p != probs.end(); ++p) {
				double oldValue = *p;
				*p += sum;
				sum += oldValue;
			} 
			std::random_device rd;
			std::mt19937 e2(rd());
			std::uniform_real_distribution<> dist(0, 1);
			std::map<unsigned int, unsigned int> hist;
			for(size_t i = 0; i < times; ++i) {
				double got = dist(e2);
				unsigned int index = 0;
				for(; probs[index] <= got; ++index);
				++hist[index];
			}
			return hist;
		}

		inline VX getStateVector() const {
			return qvector;
		}

		inline void measureAndDisplay(unsigned int times) {
			auto measurements = measure(times);
			for(auto m : measurements) {
				std::cout << "|" << unsignedToBin(m.first) << ">: " << m.second << "\n";
			}
		}

		inline void printStages() {
			for(auto s : stages) {
				std::cout << "STAGE:\n";
				for(auto qcomp : s.qcomps) {
					std::cout << "\tCOMPONENT:\n";
					std::cout << qcomp.first.calculateMatrix() << "\n";
				}
			}
		}
		
	};

}