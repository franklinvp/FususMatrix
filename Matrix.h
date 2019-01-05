#ifndef _FususMatrix_
#define _FususMatrix_

#include <cassert>
#include <vector>
#include <iostream>

#include "DenseMatrixContainer.h"
#include "LazyEvaluationExpressionTemplates.h"


namespace FususMatrix{

	// A function to input sizes of a matrix and return the number of its elements.
	std::size_t VectorProduct(std::vector<std::size_t> vector){
		std::size_t temp{ 1 };
		for (auto i : vector){
			temp *= i;
		};
		return temp;
	};

	//
	//                      The Matrix Class.
	// Template parameter Rep is used to implement lazy evaluation using Expression Templates.
	// For concrete matrices Rep is an DenseMatrixContainer<T, Dimension> >, the type of the container for the elements.
	template<typename T = double, std::size_t Dimension = 2, typename Rep = DenseMatrixContainer<T, Dimension> >
	class Matrix {
	protected:
		Rep Expression_MyMatrixContainer; // (Access to) the data of the Matrix.
	public:
		// Constructor from the sizes along each dimension.
		template<typename... Sizes>
		Matrix(Sizes... sizes)
			: Expression_MyMatrixContainer(Dimension, sizes...){
		};

		// Creates Matrix from possible representation.
		Matrix(Rep const& rb)
			: Expression_MyMatrixContainer(rb){
		};

		// Assignment operator for the same type.
		Matrix& operator=(Matrix const& other){
			assert(getSizesAlongEachDimension() == other.getSizesAlongEachDimension());
			for (std::size_t index = 0; index < other.size(); ++index){
				Expression_MyMatrixContainer[index] = other[index];
			};
			return *this;
		};

		// Assignment operator for Matrices of different types.
		template<typename T2, std::size_t Dimension, typename Rep2>
		Matrix& operator=(Matrix<T2, Dimension, Rep2> const& b){
			if (size()>1 && b.size() > 1){
				assert(getSizesAlongEachDimension() == b.getSizesAlongEachDimension());
			};
			for (std::size_t index = 0; index < b.size(); ++index){
				Expression_MyMatrixContainer[index] = b[index];
			};
			return *this;
		};

		// Size returns the sizes along each dimension (a vector).
		std::size_t size() const {
			return Expression_MyMatrixContainer.size();
		};

		// Returns the SizesAlongEachDimension
		const std::vector<std::size_t> getSizesAlongEachDimension() const {
			return Expression_MyMatrixContainer.getSizesAlongEachDimension();
		};

		// Dimension getter.
		std::size_t dimension() const {
			return Expression_MyMatrixContainer.dimension();
		};

		// Rows getter.
		std::size_t rows() const {
			static_assert(Dimension == 2, "Method 'rows()' only defined for square matrices.");
			return Expression_MyMatrixContainer.rows();
		};

		// Columns getter.
		std::size_t columns() const {
			static_assert(Dimension == 2, "Method 'rows()' only defined for square matrices.");
			return Expression_MyMatrixContainer.columns();
		};

		// Index operator for constants and variables.
		// It access the elements in the order they are stored according to Expression_MyMatrixContainer.
		T operator[](std::size_t index) const {
			assert(index < size());
			return Expression_MyMatrixContainer[index];
		};
		T& operator[](std::size_t index) {
			assert(index < size());
			return Expression_MyMatrixContainer[index];
		};

		// Returning what the Matrix currently represents.
		Rep const& rep() const {
			return Expression_MyMatrixContainer;
		};
		Rep& rep(){
			return Expression_MyMatrixContainer;
		};

		// Unitary operators. 
		// Additive inverse of each component.
		Matrix& operator-(){
			Expression_MyMatrixContainer.operator-();
			return *this;
		};
		// Multiplicative inverse of each component, as defined by Expression_MyMatrixContainer.
		Matrix& reciprocals(){
			Expression_MyMatrixContainer.reciprocals();
			return *this;
		};

		// Compound assignment operators.
		Matrix& operator+=(const Matrix& X){
			assert(this->getSizesAlongEachDimension() == X.getSizesAlongEachDimension());
			Expression_MyMatrixContainer += X.Expression_MyMatrixContainer;
			return *this;
		};
		Matrix& operator-=(const Matrix& X){
			assert(this->getSizesAlongEachDimension() == X.getSizesAlongEachDimension());
			Expression_MyMatrixContainer -= X.Expression_MyMatrixContainer;
			return *this;
		};
		Matrix& operator*=(const Matrix& X){
			assert(this->getSizesAlongEachDimension() == X.getSizesAlongEachDimension());
			Expression_MyMatrixContainer *= X.Expression_MyMatrixContainer;
			return *this;
		};
		Matrix& operator*=(const T& s){
			Expression_MyMatrixContainer *= s;
			return *this;
		};
		Matrix& operator/=(const Matrix& X){
			assert(this->getSizesAlongEachDimension() == X.getSizesAlongEachDimension());
			Expression_MyMatrixContainer /= X.Expression_MyMatrixContainer;
			return *this;
		};

		// Weak transpose.
		void transpose(){
			Expression_MyMatrixContainer.transpose();
		};

		// Strong transpose.
		void strongTranspose(){
			Expression_MyMatrixContainer.strongTranspose();
		};


		// Accessing elements.
		template<typename... Coordinates>
		const T& operator()(Coordinates... coordinates) const {
			return Expression_MyMatrixContainer(coordinates...);
		};

		template<typename... Coordinates>
		T& operator()(Coordinates... coordinates){
			return Expression_MyMatrixContainer(coordinates...);
		};


		// Matrix Multiplication
		// Multiplication only for 2x2 matrices with the right sizes
		Matrix<T, 2> Multiply(Matrix<T, 2>& secondFactor){
			assert((Dimension == 2) && (columns() == secondFactor.rows()));
			Matrix<T, 2> temp(rows(), secondFactor.columns());
			temp.Expression_MyMatrixContainer.Multiply((*this).Expression_MyMatrixContainer, secondFactor.Expression_MyMatrixContainer);
			return temp;
		};

		bool IsLowerTriangular(){
			return Expression_MyMatrixContainer.IsLowerTriangular();
		};

		// Span computes coefficients for a linear combination of the columns of 'this' to obtain the vector in the input.
		Matrix span(const Matrix& B){
				Matrix temp(Expression_MyMatrixContainer.span(B.Expression_MyMatrixContainer));
				return temp;
		};


	}; // END Matrix class.

	template<typename T = double>
	using Vector = Matrix < T > ;

	// To print two dimensional matrices. 
	template<typename T = double, std::size_t Dimension = 2, typename Rep = DenseMatrixContainer<T, Dimension> >
	std::ostream& operator<<(std::ostream& os, const Matrix<T, Dimension, Rep>& matrix){
		for (std::size_t i = 0; i != matrix.rows(); ++i) {
			for (std::size_t j = 0; j != matrix.columns(); ++j){
				os << matrix(i, j);
				if (j + 1 != matrix.columns()){
					os << ',';
				};
			};
			if (i + 1 != matrix.rows()){
				os << "\n";
			}
			else{
				os << std::endl;
			};
		}
		return os;
	};// END of operator<<
	//
}; // END namespace.

#include "BinaryOperatorsForLazyEvaluation.h"

#endif
