#ifndef _FususBinaryOperatorsForLazyEvaluation_
#define _FususBinaryOperatorsForLazyEvaluation_

namespace FususMatrix{

	// The operators.

	// Addition of two Matrices.
	// Returns a Matrix which container is an Addition object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R1, typename R2>
	inline Matrix<T, Dimension, Addition<T, R1, R2>>
		operator+(Matrix<T, Dimension, R1> const& a, Matrix<T, Dimension, R2> const& b){
		return Matrix<T, Dimension, Addition<T, R1, R2>>(Addition<T, R1, R2>(a.rep(), b.rep()));
	};

	// Addition of a Scalar to a Matrix.
	// Returns a Matrix which container is an Addition object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R2>
	inline Matrix<T, Dimension, Addition<T, Scalar<T>, R2>>
		operator+(T const& a, Matrix<T, Dimension, R2> const& b){
		return Matrix<T, Dimension, Addition<T, Scalar<T>, R2>>(Addition<T, Scalar<T>, R2>(Scalar<T>(a), b.rep()));
	};

	// Addition of a Matrix to a Scalar.
	// Returns a Matrix which container is an Addition object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R1>
	inline Matrix<T, Dimension, Addition<T, R1, Scalar<T>>>
		operator+(Matrix<T, Dimension, R1> const& a, T const& b){
		return Matrix<T, Dimension, Addition<T, R1, Scalar<T>>>(Addition<T, R1, Scalar<T>>(a.rep(), Scalar<T>(b)));
	};

	// Subtraction of two Matrices.
	// Returns a Matrix which container is a Subtraction object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R1, typename R2>
	inline Matrix<T, Dimension, Subtraction<T, R1, R2> >
		operator-(Matrix<T, Dimension, R1> const& a, Matrix<T, Dimension, R2> const& b){
		return Matrix<T, Dimension, Subtraction<T, R1, R2> >(Subtraction<T, R1, R2>(a.rep(), b.rep()));
	};

	// Multiplication of two Matrices.
	// Returns a Matrix which container is a Multiplication object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R1, typename R2>
	inline Matrix<T, Dimension, Multiplication<T, R1, R2> >
		operator*(Matrix<T, Dimension, R1> const& a, Matrix<T, Dimension, R2> const& b){
		return Matrix<T, Dimension, Multiplication<T, R1, R2> >(Multiplication<T, R1, R2>(a.rep(), b.rep()));
	};

	// Multiplication of a scalar and an Matrix.
	template<typename T, std::size_t Dimension = 2, typename R2>
	inline Matrix<T, Dimension, Multiplication<T, Scalar<T>, R2> >
		operator*(T const& s, Matrix<T, Dimension, R2> const& b){
		return Matrix<T, Dimension, Multiplication<T, Scalar<T>, R2> >(Multiplication<T, Scalar<T>, R2>(Scalar<T>(s), b.rep()));
	};

	// Multiplication of a Matrix and a scalar.
	// Returns a Matrix which container is a Multiplication object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R1>
	inline Matrix<T, Dimension, Multiplication<T, R1, Scalar<T> > >
		operator*(Matrix<T, Dimension, R1> const& a, T const& s){
		return Matrix<T, Dimension, Multiplication<T, R1, Scalar<T> > >(Multiplication<T, R1, Scalar<T> >(a.rep(), Scalar<T>(s)));
	};

	// Division of two Matrices
	// Returns a Matrix which container is a Division object, an expression.
	template<typename T, std::size_t Dimension = 2, typename R1, typename R2>
	inline Matrix<T, Dimension, Division<T, R1, R2> >
		operator/(Matrix<T, Dimension, R1> const& a, Matrix<T, Dimension, R2> const& b){
		return Matrix<T, Dimension, Division<T, R1, R2> >(Division<T, R1, R2>(a.rep(), b.rep()));
	};

}// END namespace

#endif