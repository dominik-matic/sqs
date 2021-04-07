#pragma once

#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include "QOperator.hpp"
#include "Eigen/unsupported/Eigen/KroneckerProduct"

namespace sqs {
	class QComponent {
		private:
		struct ComponentSegment {
			std::set<unsigned int> usedQubits;
			std::list<std::pair<QOperator, std::vector<unsigned int>>> qops;
		};

		std::list<ComponentSegment> segmentList;
		std::set<unsigned int> usedQubits;
		unsigned int repeat = 1;

		inline void addQOpToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			std::string type = qop.getType();
			if(type == "T") {
				addToffoliToSegment(qop, qubitPos, cs);
			} else if(type[0] == 'C') {
				addControlToSegment(qop, qubitPos, cs);
			} else if (type[0] == '?') {
				addCustomToSegment(qop, qubitPos, cs);
			} else {
				addUnitaryToSegment(qop, qubitPos, cs);
			}
		}

		inline void addToffoliToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {

		}
		inline void addControlToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			

		}
		inline void addCustomToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			if(qop.getMatrix().rows() == 2) {
				addUnitaryToSegment(qop, qubitPos, cs);
				return;
			}
			if(qubitPos.size() <= 1) {
				throw "Invalid operator size!!";
			}
			
			std::sort(qubitPos.begin(), qubitPos.end());
			unsigned int prev = qubitPos[0];
			unsigned int current;
			for(size_t i = 1; i < qubitPos.size(); ++i) {
				current = qubitPos[i];
				if(current == prev) {
					throw "Cannot have two of the same input qubit";
				}
				if(current == prev || current - prev > 1) {
					throw "Custom operators must be constructed to include all inner (unused) qubits.";
				}
				prev = current;
			}

			for(auto qPos : qubitPos) { cs.usedQubits.insert(qPos); }
			cs.qops.push_back(std::make_pair(qop, qubitPos));
		}

		inline void addUnitaryToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			if(qop.getMatrix().rows() != 2) { throw "Not unitary?!"; }
			for(auto pos : qubitPos) { cs.usedQubits.insert(pos); }
			for(auto qPos : qubitPos) {
				std::vector<unsigned int> vect { qPos };
				cs.qops.push_back(std::make_pair(qop, vect));
			}
		}

		inline MX EyeToCrossedPower(unsigned int power) {
			MX returnMatrix = EyeMatrix;
			MX tempMatrix = EyeMatrix;
			for(size_t i = 1; i < power; ++i) {
				returnMatrix = kroneckerProduct(returnMatrix, tempMatrix).eval();
			}
			return returnMatrix;
		}

		public:
		//QComponent() {}
		QComponent(QOperator qop, std::vector<unsigned int> qubitPos) { add(qop, qubitPos); }
		QComponent(QOperator qop, unsigned int qubitPos) { add(qop, { qubitPos }); }
		QComponent(QOperator qop, unsigned int qubitPos1, unsigned int qubitPos2) { add(qop, { qubitPos1, qubitPos2 }); }
		

		inline bool add(QOperator qop, std::vector<unsigned int> qubitPos) {
			if(qubitPos.size() <= 0) { throw "Invalid qubit operator size!!!"; }
			for(auto qPos : qubitPos) {
				if(usedQubits.find(qPos) != usedQubits.end()) {
					return false;
				}
			}

			bool added = false;
			for(auto segment : segmentList) {
				bool eligible = true;
				for(auto qPos : qubitPos) {
					auto it = segment.usedQubits.find(qPos); 
					if(it != segment.usedQubits.end()) {
						eligible = false;
						break;
					}
				}
				if(eligible) {
					addQOpToSegment(qop, qubitPos, segment);
					added = true;
					break;	
				}
			}
			if(!added) {
				ComponentSegment cs;
				addQOpToSegment(qop, qubitPos, cs);
				segmentList.push_back(cs);
			}

			for(auto qPos : qubitPos) { usedQubits.insert(qPos); }
			
			return true;
		}

		inline void add(QOperator qop, unsigned int qPos) { add(qop, { qPos }); }
		inline void add(QOperator qop, unsigned int qPos1, unsigned int qPos2) { add(qop, { qPos1, qPos2 }); }
		inline void add(QOperator qop, unsigned int qPos1, unsigned int qPos2, unsigned int qPos3) { add(qop, { qPos1, qPos2, qPos3 }); }
		

		inline void reverse(QOperator qop) {
			segmentList.reverse();
			for(auto segment : segmentList) {
				segment.qops.reverse();
			}
		}

		inline std::vector<unsigned int> getQubitRange() {
			std::vector<unsigned int> qubitRange;
			for(auto it = usedQubits.begin(); it != usedQubits.end(); ++it) {
				qubitRange.push_back(*it);
			}
			std::sort(qubitRange.begin(), qubitRange.end());
			return qubitRange;
		}

		inline MX getMatrix() {
			MX result;
			auto qubitRange = getQubitRange();
			unsigned int range = qubitRange.size();
			unsigned int dimension = int(std::pow(2, range));
			unsigned int lowerBound = qubitRange[0];
			unsigned int upperBound = qubitRange[range - 1];

			result.setIdentity(dimension, dimension);
			
			for(auto segIt = segmentList.rbegin(); segIt != segmentList.rend(); ++segIt) {
				auto segment = (*segIt);
				unsigned int currentLower;
				segment.qops.sort([](auto const& a, auto const& b){
					return a.second > b.second;
				});

				auto it = segment.qops.begin();
				MX tempMatrix = (*it).first.getMatrix();
				currentLower = (*it).second.front();
				if(currentLower != lowerBound) {
					unsigned int difference = currentLower - lowerBound;
					tempMatrix = kroneckerProduct(EyeToCrossedPower(difference), tempMatrix).eval();
				}
				
				++it;
				/**
				 * Za sada imamo:
				 	I x I x I x ... lowerBound
					TODO:
					x (svi ostali) x I x I x I (do kraja upperbounda)
				*/
				for(; it != segment.qops.end(); ++it) {
					//auto difference = (*it).second[0] - ;
				}



				result = tempMatrix * result;
			}
		}

	};
}