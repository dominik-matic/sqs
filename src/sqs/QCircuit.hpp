#pragma once

#include <list>
#include <cmath>
#include "eigenconsts.hpp"
#include "QComponent.hpp"

namespace sqs {

	class QCircuit {
		private:
		struct Stage {
			std::set<unsigned int> usedQubits;
			std::list<std::pair<QComponent, std::vector<unsigned int>>> qcomps;
		};
		std::list<Stage> stages;

		VX qvector;
		unsigned int qubits;


		inline void addQCompToStage(QComponent &qcomp, std::vector<unsigned int> &qubitRange, Stage &stage) {
			for(auto qPos : qubitRange) {
				stage.usedQubits.insert(qPos);
			}
			stage.qcomps.push_back(std::make_pair(qcomp, qubitRange));
		}

		public:
		QCircuit(unsigned int qubits) : qubits(qubits) {
			int vecLen =  int(std::pow(2.0, qubits));
			qvector.resize(vecLen);
			qvector(0) = 1;									// initialize to |00...00>
		}

		inline void add(QComponent qcomp) {
			auto qubitRange = qcomp.getQubitRange();
			bool added = false;
			for(auto stage : stages) {
				bool eligible = true;
				for(auto qPos : qubitRange) {
					auto it = stage.usedQubits.find(qPos);
					if(it != stage.usedQubits.end()) {
						eligible = false;
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
			//for(auto qcomp : qcomps) {
			//	for(auto qop : qcomp.getOperators()) {
			//		qvector = qop.getMatrix() * qvector;
			//	}
			//}
		}

		inline VX getStateVector() const {
			return qvector;
		}
		
	};

}