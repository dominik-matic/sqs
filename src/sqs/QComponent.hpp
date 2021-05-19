#pragma once

#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include "QOperator.hpp"
#include "Eigen/unsupported/Eigen/KroneckerProduct"

namespace sqs {
	class QComponent {

		public:
		QComponent() { matrixCalculated = true; calculatedMatrix = Private::EyeMatrix; };
		QComponent(QOperator qop, std::vector<unsigned int> qubitPos) { add(qop, qubitPos); }
		QComponent(QOperator qop, unsigned int qubitPos) { add(qop, { qubitPos }); }
		QComponent(QOperator qop, unsigned int qubitPos1, unsigned int qubitPos2) { add(qop, { qubitPos1, qubitPos2 }); }
		

		inline bool add(QOperator qop, std::vector<unsigned int> qubitPos) {
			matrixCalculated = false;
			if(qubitPos.size() <= 0) { throw "Invalid qubit operator size!"; }
			bool added = false;
			for(auto segment = segmentList.begin(); segment != segmentList.end(); ++segment) {
				bool eligible = true;
				for(auto qPos : qubitPos) {
					auto it = (*segment).lockedQubits.find(qPos); 
					if(it != (*segment).lockedQubits.end()) {
						eligible = false;
						for(auto pos : qubitPos) {(*segment).lockedQubits.insert(pos); }
						break;
					}
				}
				if(eligible) {
					addQOpToSegment(qop, qubitPos, (*segment));
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

		inline bool add(QOperator qop, unsigned int qPos) { return add(qop, std::vector<unsigned int>{qPos}); }
		inline bool add(QOperator qop, unsigned int qPos1, unsigned int qPos2) { return add(qop, std::vector<unsigned int>{ qPos1, qPos2 }); }
		inline bool add(QOperator qop, unsigned int qPos1, unsigned int qPos2, unsigned int qPos3) { return add(qop, std::vector<unsigned int>{ qPos1, qPos2, qPos3 }); }
		
		inline bool add(QComponent qcomp) {
			auto m = qcomp.calculateMatrix();
			auto qr = qcomp.getQubitRange();
			return add(QOperator(m, "?"), qr);
		}

		inline void setIterations(unsigned int times) {
			matrixCalculated = false;
			repeat = times;
		}

		inline void reverse() {
			matrixCalculated = false;
			segmentList.reverse();
			for(auto segment : segmentList) {
				segment.qops.reverse();
			}
		}

		inline std::vector<unsigned int> getQubitRange() const {
			std::vector<unsigned int> qubitRange;
			for(auto it = usedQubits.begin(); it != usedQubits.end(); ++it) {
				qubitRange.push_back(*it);
			}
			std::sort(qubitRange.begin(), qubitRange.end());
			return qubitRange;
		}

		inline const MX& calculateMatrix() {
			if(matrixCalculated) {
				return calculatedMatrix;
			}
			MX result;
			auto qubitRange = getQubitRange();
			unsigned int lowerBound = qubitRange[0];
			unsigned int upperBound = qubitRange[qubitRange.size() - 1];
			unsigned int range = upperBound - lowerBound + 1;
			unsigned int dimension = int(std::pow(2, range));

			result.setIdentity(dimension, dimension);
			for(auto segIt = segmentList.begin(); segIt != segmentList.end(); ++segIt) {
				auto segment = (*segIt);
				unsigned int currentLower;
				unsigned int prevUpper;
				unsigned int difference;
				segment.qops.sort([](auto const& a, auto const& b){
					// !!!!!! ako se nesto zbugga makni [0] !!!!!!
					return a.second[0] < b.second[0];
				});
				

				auto it = segment.qops.begin();
				if(it == segment.qops.end()) { continue; }
				MX tempMatrix = (*it).first.getMatrix();
				currentLower = (*it).second.front();
				prevUpper = (*it).second.back();
				if(currentLower != lowerBound) {
					difference = currentLower - lowerBound;
					tempMatrix = kroneckerProduct(EyeToCrossedPower(difference), tempMatrix).eval();
				}
				++it;

				for(; it != segment.qops.end(); ++it) {
					currentLower = (*it).second.front();
					if(currentLower != (prevUpper + 1)) {
						difference = currentLower - prevUpper - 1;
						//std::cout << currentLower << " " << prevUpper << " " << difference << "\n";
						tempMatrix = kroneckerProduct(tempMatrix, EyeToCrossedPower(difference)).eval();
					}
					tempMatrix = kroneckerProduct(tempMatrix, (*it).first.getMatrix()).eval();
					prevUpper = (*it).second.back();
				}

				if(prevUpper != upperBound) {
					difference = upperBound - prevUpper;
					tempMatrix = kroneckerProduct(tempMatrix, EyeToCrossedPower(difference)).eval();
				}
				

				result = tempMatrix * result;

			}

			if(repeat > 1) {
				auto base = result;
				for(int i = 1; i < repeat; ++i) {
					result = base * result;
				}
			}

			calculatedMatrix = result;
			return calculatedMatrix;
		}

		/*inline void printSegments() {
			for (auto s : segmentList) {
				std::cout << "segment:\n";
				for(auto a : s.qops) {
					std::cout << a.first.getType() << "\n";
				}
			}
		}*/




		private:
		struct ComponentSegment {
			std::set<unsigned int> usedQubits;
			std::set<unsigned int> lockedQubits;
			std::list<std::pair<QOperator, std::vector<unsigned int>>> qops;
		};

		std::list<ComponentSegment> segmentList;
		std::set<unsigned int> usedQubits;
		unsigned int repeat = 1;
		MX calculatedMatrix;
		bool matrixCalculated = false;

		inline void addQOpToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			std::string type = qop.getType();
			 if(type[0] == 'C' && type[1] != 'U') {
				addCUnaryToSegment(qop, qubitPos, cs);
			} else if(type == "SWAP") {
				addSWAPToSegment(qubitPos);
			} else if(type == "CU") {
				addCUnitaryToSegment(qop, qubitPos, cs);
			} else if (type[0] == '?') {
				addCustomToSegment(qop, qubitPos, cs);
			} else {
				addUnaryToSegment(qop, qubitPos, cs);
			}
		}

		inline void addCUnaryToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			if(qubitPos.size() != 2) {
				throw "Invalid qubit inputs for control operator";
			}

			MX cU = qop.getMatrix();
			MX U = cU.block(cU.rows() - 2, cU.cols() - 2, 2, 2);

			unsigned int upperBound = (qubitPos[0] > qubitPos[1] ) ? 0 : 1;
			unsigned int lowerBound = qubitPos[1 - upperBound];
			upperBound = qubitPos[upperBound];

			MX tempMatrix1;
			MX tempMatrix2;
			MX lastMatrix1;
			MX lastMatrix2;

			if(qubitPos[0] == lowerBound) {			// |0><0| x nesto x ... x nesto x I + |1><1| x nesto x ... x nesto x U 
				tempMatrix1 = Private::Projector00Matrix;
				lastMatrix1 = Private::EyeMatrix;
				tempMatrix2 = Private::Projector11Matrix;
				lastMatrix2 = U;
			} else {								// I x nesto x ... x nesto x |0><0| + U x nesto x ... x nesto x |1><1|
				tempMatrix1 = Private::EyeMatrix;
				lastMatrix1 = Private::Projector00Matrix;
				tempMatrix2 = U;
				lastMatrix2 = Private::Projector11Matrix;
			}
			
			cs.usedQubits.insert(lowerBound);
			cs.lockedQubits.insert(lowerBound);
			for(unsigned int i = (lowerBound + 1); i < upperBound; ++i) {
				auto it = cs.usedQubits.find(i);
				if(it != cs.usedQubits.end()) {
					for(auto qopIt = cs.qops.begin(); qopIt != cs.qops.end(); ++qopIt) {
						bool found = false;
						for(auto n = qopIt->second.begin(); n != qopIt->second.end(); ++n) {
							if(*n == i) {
								found = true;
								tempMatrix1 = kroneckerProduct(tempMatrix1, qopIt->first.getMatrix()).eval();
								tempMatrix2 = kroneckerProduct(tempMatrix2, qopIt->first.getMatrix()).eval();
								cs.qops.erase(qopIt);
								break;
							}
						}
						if(found) { break; }
					}
				} else {
					tempMatrix1 = kroneckerProduct(tempMatrix1, Private::EyeMatrix).eval();
					tempMatrix2 = kroneckerProduct(tempMatrix2, Private::EyeMatrix).eval();
				}
				qubitPos.push_back(i);
				cs.usedQubits.insert(i);
				cs.lockedQubits.insert(i);
			}
			cs.usedQubits.insert(upperBound);
			cs.lockedQubits.insert(upperBound);
			
			tempMatrix1 = kroneckerProduct(tempMatrix1, lastMatrix1).eval();
			tempMatrix2 = kroneckerProduct(tempMatrix2, lastMatrix2).eval();

			auto resultMatrix = tempMatrix1 + tempMatrix2;
			std::sort(qubitPos.begin(), qubitPos.end());
			cs.qops.push_back(make_pair(QOperator(resultMatrix, "CU"), qubitPos));
		}

		/**
		 * Pseudokod:
		 * 1. ako segment nije empty napravi novi -> prođi kroz sve segmente i dodaj im locked qubits, kao i globalno 
		 * 2. Stvori novi segment:
		 * 	2.1. Napravi sve swapove, zapamti redoslijed
		 * 	2.2. apply qop
		 * 	2.3. napravi sve swapove obrnutim redoslijedom
		 * 3. uzmi matricu novog segmenta i qubitrange
		 * 4. addCustomSegment()
		*/
		inline void addCUnitaryToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			
			std::list<unsigned int> sortedQubitPos;
			for(auto i : qubitPos) { sortedQubitPos.push_back(i); };
			sortedQubitPos.sort();

			
			/**
			 * Ovaj dio koda modificira used qubits na način:
			 * ako su used qubits tipa [2, 5, 3, 8, 9]
			 * stvoriti ce novu listu used qubitsa tako da postoji usedQubits.size() qubita jedan kraj drugog
			 * uz uvjet da i dalje sadrzi ove prosle qubite
			 * dakle stvorit ce listu [2, 3, 4, 5, 6, 8, 9]
			*/
			unsigned int counter = qubitPos.size() - 1;
			unsigned int prevQubit = sortedQubitPos.front();
			for(auto it = ++(sortedQubitPos.begin()); it != sortedQubitPos.end(); ++it) {
				bool done = false;
				while(*it - prevQubit != 1) {
					sortedQubitPos.emplace(it, ++prevQubit);
					if(--counter == 0) {
						done = true;
						break;
					}
				}
				if(done) {
					break;
				}
				prevQubit = *it;
				--counter;
			}

			/////////////////////////////////////////////////////////////
			//for(auto a : sortedQubitPos) { std::cout << a << " ";} std::cout << "\n";
			/////////////////////////////////////////////////////////////
			
			/**
			 * Provjeriti jel segment prazan, ako nije -> stvoriti novi
			 * Zapisati svim prethodnim segmentima nove locked qubits (trenutnom isto)
			*/
			ComponentSegment* compseg = &cs;
			ComponentSegment newcs;
			if(cs.usedQubits.size() != 0) {
				compseg = &newcs;
			}
			for(auto &s : segmentList) {
				if(&s == compseg) { break; }
				for(auto sqp : sortedQubitPos) {
					s.lockedQubits.insert(sqp);
				}
			}
			for(auto sqp : sortedQubitPos) {
				compseg->usedQubits.insert(sqp);
				compseg->lockedQubits.insert(sqp);
				usedQubits.insert(sqp);
			}


			/**
			 * Računanje svih SWAPova koje treba napraviti
			*/
			std::vector<unsigned int> sortedQubitPosVect;
			for(auto sqp : sortedQubitPos) {sortedQubitPosVect.push_back(sqp); }
			sortedQubitPos.clear();
			std::vector<std::pair<unsigned int, unsigned int>> swaps;
			unsigned int index = 0;
			for(auto qPos : qubitPos) {
				unsigned int numIndex = 0;
				unsigned int tempIndex = 0;
				for(auto sqp : sortedQubitPosVect) {
					if(qPos == sqp) {
						numIndex = tempIndex;
						break;
					}
					++tempIndex;
					
				}

				if(numIndex != index) {
					swaps.push_back(std::make_pair(index, numIndex));
					unsigned int temp = sortedQubitPosVect[index];
					sortedQubitPosVect[index] = sortedQubitPosVect[numIndex];
					sortedQubitPosVect[numIndex] = temp;

				}
				++index;
			}
			
			////////////////////////////////////////////////////////////////////
			//for(auto sw : swaps) {std::cout << sw.first << " <-> " << sw.second << "\n"; }
			////////////////////////////////////////////////////////////////////


			/**
			 * Konstrukcija matrice:
			 * SWAP QOP SWAP^-1
			 */

			QComponent qcomp;

			for(auto swap : swaps) {
				std::vector<unsigned int> qp {swap.first, swap.second};
				qcomp.addSWAPToSegment(qp);
			}

			QOperator qoperator(qop.getMatrix(), "?");
			std::vector<unsigned int> qr;
			for(int i = 0; i < qubitPos.size(); ++i) {
				qr.push_back(sortedQubitPosVect[i]);
			}
			//std::cout << qoperator.getMatrix() << std::endl;
			qcomp.add(qoperator, qr);
			for(auto it = swaps.rbegin(); it != swaps.rend(); ++it) {
				std::vector<unsigned int> qp {it->first, it->second};
				qcomp.addSWAPToSegment(qp);
			}
			qoperator = qcomp.calculateMatrix();
			qr = qcomp.getQubitRange();
			addCustomToSegment(qoperator, qr, *compseg);
		}
		
		inline void addSWAPToSegment(std::vector<unsigned int> &qubitPos) {
			if(qubitPos.size() != 2) {
				throw "Invalid operator size!";
			}
			std::vector<unsigned int> qubitPosReversed {qubitPos[1], qubitPos[0]};

			QOperator cx(Private::CXMatrix, "CX");
			add(cx, qubitPos);
			add(cx, qubitPosReversed);
			add(cx, qubitPos);
			
		}

		inline void addCustomToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			if(qop.getMatrix().rows() == 2) {
				addUnaryToSegment(qop, qubitPos, cs);
				return;
			}
			if(qubitPos.size() <= 1) {
				throw "Invalid operator size!";
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

			for(auto qPos : qubitPos) { cs.usedQubits.insert(qPos); cs.lockedQubits.insert(qPos); }
			cs.qops.push_back(std::make_pair(qop, qubitPos));
		}

		inline void addUnaryToSegment(QOperator &qop, std::vector<unsigned int> &qubitPos, ComponentSegment &cs) {
			if(qop.getMatrix().rows() != 2) { throw "Not unary?!"; }
			for(auto qPos : qubitPos) {
				cs.usedQubits.insert(qPos);
				cs.lockedQubits.insert(qPos);
				std::vector<unsigned int> vect { qPos };
				cs.qops.push_back(std::make_pair(qop, vect));
			}
		}

		inline MX EyeToCrossedPower(unsigned int power) {
			MX returnMatrix;
			unsigned int dimension = std::pow(2, power);
			returnMatrix.setIdentity(dimension, dimension);
			//MX tempMatrix = EyeMatrix;
			//for(size_t i = 1; i < power; ++i) {
			//	returnMatrix = kroneckerProduct(returnMatrix, tempMatrix).eval();
			//}
			return returnMatrix;
		}
	};
}