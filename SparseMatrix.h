#ifndef _FususSparseMatrix_
#define _FususSparseMatrix_


#include <cassert>
#include <vector>
#include <iostream>

//#include "MatrixInitializer.h"
#include "SparseMatrixContainer.h"
#include "LazyEvaluationExpressionTemplates.h"

namespace FususMatrix{

	template<typename T = double, typename Rep = SparseMatrixContainer<T> >//std::vector<T> >
	class SparseMatrix : public Matrix < T, 2, Rep> {
	public:
		// Default constructor.
		SparseMatrix() : Matrix<T, 2, Rep>(0, 0){
		};

		// Default constructor.
		SparseMatrix(std::size_t rows, std::size_t columns) : Matrix<T, 2, Rep>(rows, columns){
		};

		SparseMatrix(std::initializer_list<std::initializer_list<double>> init){}
	};
	//
}// END namespace FususMatrix

#endif
