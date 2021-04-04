#pragma once

#include "eigenconsts.hpp"

namespace sqs{

	class QOperator {
		private:
		unsigned int dim;
		MX matrix;


		inline void checkDimension(const QOperator& qop) {
			if(qop.dimension() != dim) {
				throw "Dimensions don't match srry";
			}
		}

		void checkDimension(const MX& m) {
			if(m.rows() != dim) {
				throw "Dimensions don't match srry";
			}
		}

		public:
		QOperator(): matrix(ZeroMatrix), dim(2) {}
		QOperator(MX matrix): matrix(matrix), dim(matrix.rows()) {}
		

		inline MX getMatrix() const { return matrix; }
		inline unsigned int dimension() const { return dim; }


		inline void operator=(const QOperator& qop) {
			matrix = qop.matrix;
		}

		inline void operator=(const MX& m) {
			matrix = m;
		}

		inline QOperator operator+(const QOperator& qop) {
			checkDimension(qop);
			QOperator temp;
			temp.matrix = matrix + qop.matrix;
			return temp;
		}

		inline QOperator operator+(const MX& m) {
			checkDimension(m);
			QOperator temp;
			temp.matrix = matrix + m;
			return temp;
		}

		inline QOperator operator-(const QOperator& qop) {
			checkDimension(qop);
			QOperator temp;
			temp.matrix = matrix - qop.matrix;
			return temp;
		}

		inline QOperator operator-(const MX& m) {
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

		inline QOperator operator*(const QOperator& qop) {
			// TODO: check dimensions
			QOperator temp;
			temp.matrix = matrix * qop.matrix;
			return temp;
		}

		inline QOperator operator*(std::complex<double> i) {
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