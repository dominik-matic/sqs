#pragma once

#include "eigenconsts.hpp"

namespace sqs{

	class QOperator {
		private:
		MX matrix;
		std::string type;

		inline void checkDimension(const QOperator& qop) const {
			if(qop.dimension() != dimension()) {
				throw "Dimensions don't match";
			}
		}

		void checkDimension(const MX& m) const {
			if(m.rows() != dimension()) {
				throw "Dimensions don't match";
			}
		}


		public:
		QOperator(): matrix(Private::EyeMatrix), type("I") {}
		QOperator(MX matrix): matrix(matrix), type("?") {}
		QOperator(MX matrix, std::string type): matrix(matrix), type(type) {}
		

		inline MX getMatrix() const { return matrix; }
		inline unsigned int dimension() const { return matrix.rows(); }
		inline std::string getType() const { return type; }

		inline void operator=(const QOperator& qop) {
			matrix = qop.matrix;
		}

		inline void operator=(const MX& m) {
			matrix = m;
		}

		inline QOperator operator+(const QOperator& qop) const {
			checkDimension(qop);
			QOperator temp;
			temp.matrix = matrix + qop.matrix;
			return temp;
		}

		inline QOperator operator+(const MX& m) const {
			checkDimension(m);
			QOperator temp;
			temp.matrix = matrix + m;
			return temp;
		}

		inline QOperator operator-(const QOperator& qop) const {
			checkDimension(qop);
			QOperator temp;
			temp.matrix = matrix - qop.matrix;
			return temp;
		}

		inline QOperator operator-(const MX& m) const {
			checkDimension(m);
			QOperator temp;
			temp.matrix = matrix - m;
			return temp;
		}

		inline QOperator& operator+=(const QOperator& qop) {
			checkDimension(qop);
			matrix += qop.matrix;
			return *this;
		}

		inline QOperator& operator+=(const MX& m) {
			checkDimension(m);
			matrix += m;
			return *this;
		}

		inline QOperator& operator-=(const QOperator& qop) {
			checkDimension(qop);
			matrix -= qop.matrix;
			return *this;
		}

		inline QOperator& operator-=(const MX& m) {
			checkDimension(m);
			matrix -= m;
			return *this;
		}

		inline QOperator operator*(const QOperator& qop) const {
			// TODO: check dimensions
			QOperator temp;
			temp.matrix = matrix * qop.matrix;
			return temp;
		}

		inline QOperator operator*(std::complex<double> i) const {
			QOperator temp;
			temp.matrix = i * matrix;
			return temp;
		}

		inline friend QOperator operator*(std::complex<double> i, const QOperator qop) {
			// TODO: check dimensions
			QOperator temp;
			temp.matrix = i * qop.matrix;
			return temp;
		}

		inline QOperator& operator*=(QOperator& qop) {
			// TODO: check dimensions
			matrix = matrix * qop.matrix;
			return *this;
		}

		inline QOperator& operator*=(std::complex<double> i) {
			matrix = matrix * i;
			return *this;
		}

	};

}