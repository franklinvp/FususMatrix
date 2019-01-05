#ifndef _FususSparseMatrixContainer_
#define _FususSparseMatrixContainer_

#include <numeric>
#include <map>

namespace FususMatrix{
	//
	template<typename T = double>
	class SparseMatrixContainer{
	private:
		std::vector<T> vals; // Non-zero components.
		std::vector<std::size_t> cindx; // Column indices of the non-zero components.
		std::vector<std::size_t> rindx; // Index in vals and cindx where the pivot of the corresponding row lies.
		//
		bool Transposed; // Transposed or not.
		std::size_t Rows;
		std::size_t Columns;
		std::size_t NNZ; // Number of Non-Zero elements.
	public:
		// Constructor from the sizes along each dimension.
		SparseMatrixContainer(std::size_t Dimension, std::size_t rows, std::size_t columns)
			: Transposed(false), Rows(rows), Columns(columns), rindx(columns + 1), NNZ(0){
		};

		// Swap.
		void swap(SparseMatrixContainer& other){
			vals.swap(other.vals);
			cindx.swap(other.cindx);
			rindx.swap(other.rindx);
			Transposed = other.Transposed;
			Rows = other.Rows;
			Columns = other.Columns;
			NNZ = other.NNZ;
		};

		// Size is size of represented data.
		std::size_t size() const {
			return Rows * Columns;
		};

		// Getter for sizes along each dimension.
		std::size_t SizeAlongDimension(std::size_t dim) const {
			assert(dim == 0 || dim == 1);
			if (dim == 0){
				return Rows;
			}
			else{
				return Columns;
			};
		};

		// Dimension getter.
		std::size_t dimension() const {
			return 2;
		};

		// Index operator for constants and variables.
		// Accesses the elements according to the linear order in the 1-D vector container.
		T operator[](std::size_t index) const {
			return vals[index]; // This one is not this simple.
		};
		T& operator[](std::size_t index) {
			return vals[index];
		};

		// Unitary operators.
		// Additive inverse of each element.
		SparseMatrixContainer& operator-(){
			for (std::size_t i = 0; i < vals.size(); ++i){
				vals[i] = -vals[i];
			};
			return *this;
		};
		// Multiplicative inverse of each element.
		SparseMatrixContainer& reciprocals(){
			for (std::size_t i = 0; i < vals.size(); ++i){
				vals[i] = 1 / vals[i];
			};
			return *this;
		};


		// Accessing elements
		std::size_t ComputePosition(std::size_t row, std::size_t column){
			return 0;
		};

		T& operator()(std::size_t row, std::size_t column){
			return vals[ComputePosition(row, column)];// much more complicated.
		};

		const T& operator()(std::size_t row, std::size_t column) const {
			for (std::size_t i = rindx[row]; i < rindx[row + 1]; ++i){
				if (cindx[i] == column){
					return vals[i];
				}
			};
			return T{ 0 };
		};
		//
	};// END SparseMatrixContainer class
	//
}// END namespace

#endif
