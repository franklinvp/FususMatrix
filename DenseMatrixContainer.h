#ifndef _FususDenseMatrixContainer_
#define _FususDenseMatrixContainer_

#include <numeric>
#include <deque>
#include <type_traits>

namespace FususMatrix{

	template <typename T>
	using MyContainerType = typename std::conditional<
		std::is_same<T, bool>::value,
		std::deque<T>,
		std::vector<T>
	>::type;
	//    Dense Matrix Container. 
	//////////////////////////////////////////////////
	template<typename T = double, std::size_t Dimension = 2>
	class DenseMatrixContainer{		
	private:
		MyContainerType<T> MyData; // Data of the Matrix
		bool Transposed; // Transposed or not.
		std::size_t MyDimension; // Dimension.
		std::vector<std::size_t> SizesAlongEachDimension; // Sizes along each dimension.
		std::vector<std::size_t> Strides; // Used to locate the elements of the matrix inside the 1-D vector container.
	public:
		// Constructor from the sizes along each dimension.
		template<typename... Sizes>
		DenseMatrixContainer(std::size_t dimension, Sizes... sizes)
			: Transposed(false), MyDimension(dimension), SizesAlongEachDimension(dimension), Strides(dimension, 1){
			if (sizeof...(sizes) == 0){ // If no sizes entered set them to zero.
				for (std::size_t i = 0; i < dimension; ++i){
					SizesAlongEachDimension[i] = 0;
				};
				MyData = MyContainerType<T>(1);// Such that size zero are only the scalars.
			}
			else{
				SizesAlongEachDimension = { static_cast<std::size_t>(sizes)... };
				std::size_t temp{ 1 };
				for (auto i : SizesAlongEachDimension){
					temp *= i;
				};
				MyData = MyContainerType<T>(temp);
			};
			assert(MyData.size() > 0 || dimension == 0);
			// Such that a matrix of dimension zero can be used as a Scalar.
			// Not sure yet what is the best idea to treat the degenerate cases.
			if (Dimension == 0){
				MyData = MyContainerType<T>(1);
			};
			// Initializing the strides
			for (std::size_t i = 1; i < SizesAlongEachDimension.size(); ++i){
				for (std::size_t j = 0; j < i; ++j){
					Strides[j] *= SizesAlongEachDimension[i];
				};
			};
		};

		// Move constructor.
		DenseMatrixContainer(DenseMatrixContainer&& other){
			MyData.swap(other.MyData);
			Transposed = other.Transposed;
			MyDimension = other.MyDimension;
			SizesAlongEachDimension.swap(other.SizesAlongEachDimension);
			Strides.swap(other.Strides);
		};

		// Copy constructor
		DenseMatrixContainer(const DenseMatrixContainer& other) 
			: MyData(other.MyData),	Transposed(other.Transposed), MyDimension(other.MyDimension), 
			  SizesAlongEachDimension(other.SizesAlongEachDimension), Strides(other.Strides){
		};

		// Swap.
		void swap(DenseMatrixContainer& other){
			MyData.swap(other.MyData);
			Transposed = other.Transposed;
			MyDimension = other.MyDimension;
			SizesAlongEachDimension.swap(other.SizesAlongEachDimension);
			Strides.swap(other.Strides);
		};

		// Size is size of represented data.
		std::size_t size() const {
			return MyData.size();
		};

		//Getter for all SizesAlongEachDimension
		const std::vector<std::size_t> getSizesAlongEachDimension() const {
			return SizesAlongEachDimension;
		};

		// Getter for sizes along each dimension.
		std::size_t SizeAlongDimension(std::size_t dim) const {
			return SizesAlongEachDimension[dim];
		};

		// Number of rows.
		std::size_t rows() const {
			if (Dimension > 0){
				return SizeAlongDimension(0);
			}
			else{
				return 0;
			};
		};

		// Number of columns.
		std::size_t columns() const {
			if (Dimension > 0){
				return SizeAlongDimension(1);
			}
			else{
				return 0;
			};
		};

		// Dimension getter.
		std::size_t dimension() const {
			return MyDimension;
		};

		// Index operator for constants and variables.
		// Accesses the elements according to the linear order in the 1-D vector container.
		T operator[](std::size_t index) const {
			assert(index < size());
			return MyData[index];
		};
		T& operator[](std::size_t index) {
			assert(index < size());
			return MyData[index];
		};

		// Unitary operators.
		// Additive inverse of each element.
		DenseMatrixContainer& operator-(){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] = -MyData[i];
			};
			return *this;
		};
		// Multiplicative inverse of each element.
		DenseMatrixContainer& reciprocals(){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] = 1 / MyData[i];
			};
			return *this;
		};

		// Compound assignment operators.
		DenseMatrixContainer& operator+=(const DenseMatrixContainer& X){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] += X.MyData[i];
			};
			return *this;
		};
		DenseMatrixContainer& operator-=(const DenseMatrixContainer& X){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] -= X.MyData[i];
			};
			return *this;
		};
		DenseMatrixContainer& operator*=(const DenseMatrixContainer& X){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] *= X.MyData[i];
			};
			return *this;
		};
		DenseMatrixContainer& operator*=(const T& s){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] += s;
			};
			return *this;
		};
		DenseMatrixContainer& operator/=(const DenseMatrixContainer& X){
			for (std::size_t i = 0; i < MyData.size(); ++i){
				MyData[i] /= X.MyData[i];
			};
			return *this;
		};

		// Weak transpose.
		void transpose(){
			assert(Dimension == 2);
			Transposed = !Transposed;
		};

		// Transposing a contiguous minor of a source matrix to the minor of target matrix.
		// Will be used for cache-oblivious strong transposition.
		void minortranspose(DenseMatrixContainer<T, 2>& source, DenseMatrixContainer<T, 2>& target, std::size_t firstrow, std::size_t lastrow, std::size_t firstcolumn, std::size_t lastcolumn){
			for (std::size_t i = firstrow; i < lastrow + 1; ++i){
				for (std::size_t j = firstcolumn; j < lastcolumn + 1; ++j){
					target(j, i) = source(i, j);
				};
			};
		};

		// Recursion for Cache-Oblivious transposition
		void transposeRecursion(DenseMatrixContainer<T, 2>& source, DenseMatrixContainer<T, 2>& target, std::size_t firstrow, std::size_t lastrow, std::size_t firstcolumn, std::size_t lastcolumn){
			std::size_t width{ lastrow - firstrow + 1 };
			std::size_t hight{ lastcolumn - firstcolumn + 1 };
			// Small minors get transposed with naive transposition
			if ((width <= 3) || (hight <= 3)){
				minortranspose(source, target, firstrow, lastrow, firstcolumn, lastcolumn);
			}
			else{
				// The matrix is divided along its largest dimension and the pieces get recursively transposed
				if (width >= hight){
					std::size_t halfwidth{ width / 2 };
					transposeRecursion(source, target, firstrow, halfwidth, firstcolumn, lastcolumn);
					transposeRecursion(source, target, halfwidth + 1, lastrow, firstcolumn, lastcolumn);
				}
				else{
					std::size_t halfhight{ hight / 2 };
					transposeRecursion(source, target, firstrow, lastrow, firstcolumn, halfhight);
					transposeRecursion(source, target, firstrow, lastrow, halfhight + 1, lastcolumn);
				};
			};
		};

		// Cache-oblivious algorithm for transposition.
		// The matrix gets divided in half along its larger dimension.
		// Then the two submatrices get transposed. 
		// Strong transposition. It actually changes the position of the entries of the matrix.
		void strongTranspose(){
			assert(Dimension == 2);
			DenseMatrixContainer<T, 2> temp(Dimension, SizesAlongEachDimension[0], SizesAlongEachDimension[1]);
			transposeRecursion(*this, temp, 0, SizesAlongEachDimension[0] - 1, 0, SizesAlongEachDimension[1] - 1);
			temp.Transposed = false;
			swap(temp);
		};

		// Accessing elements  
		// This computes the position of the components of the matrix within the 1-D vector container.
		template<typename... Coordinates>
		inline std::size_t ComputePosition(Coordinates... coordinates) const {
			std::vector<std::size_t> coord({ static_cast<std::size_t>(coordinates)... });
			return std::inner_product(coord.begin(), coord.end(), Strides.begin(), 0);
		};
		// Constant access.
		template<typename FirstCoordinate, typename... RemainingCoordinates>
		const T& operator()(FirstCoordinate i, RemainingCoordinates... coordinates) const {
			if (Transposed){
				return MyData[ComputePosition(coordinates..., i)];
			}
			else{
				return MyData[ComputePosition(i, coordinates...)];
			};
		};
		// Non-constant access.
		template<typename FirstCoordinate, typename... RemainingCoordinates>
		T& operator()(FirstCoordinate i, RemainingCoordinates... coordinates){
			if (Transposed){
				return MyData[ComputePosition(coordinates..., i)];
			}
			else{
				return MyData[ComputePosition(i, coordinates...)];
			};
		};

		void Multiply(DenseMatrixContainer<T> const& A, DenseMatrixContainer<T> const& B){
			T ComponentOfProduct{ 0 };
			for (std::size_t i = 0; i < A.rows(); ++i){
				for (std::size_t j = 0; j < B.columns(); ++j){
					for (std::size_t k = 0; k < A.columns(); ++k){
						ComponentOfProduct += A(i, k) * B(k, j);
					};
					(*this)(i, j) = ComponentOfProduct;
					ComponentOfProduct = 0;
				};
			};
		};

		// Checking if 'this' is a lower triangular matrix (container).
		bool IsLowerTriangular(){
			assert(Dimension == 2);
			for (std::size_t i = 0; i < SizesAlongEachDimension[0]; ++i){
				for (std::size_t j = i + 1; j<SizesAlongEachDimension[1]; ++j){
					if (this->operator()(i, j) != 0){
						return false;
					};
				};
			};
			return true;
		};

		// Checking if 'this' is an upper triangular matrix (container).
		bool IsUpperTriangular(){
			assert(Dimension == 2);
			for (std::size_t i = 1; i < SizesAlongEachDimension[0]; ++i){
				for (std::size_t j = 0; j < i; ++j){
					if (this->operator()(i, j) != 0){
						return false;
					};
				};
			};
			return true;
		};

		DenseMatrixContainer span(const DenseMatrixContainer& b){
			assert(b.size() == SizesAlongEachDimension[1]);
			bool WeCanSolveIt{false};
			DenseMatrixContainer y(b);
			if (IsLowerTriangular()){
				for (std::size_t i = 0; i < y.size(); ++i){
					for (std::size_t j = 0; j < i; ++j)
					{
						y[i] -= (*this)(i, j) * y[j];
					}
					y[i] /= (*this)(i, i);
				};
				return y;
			};
			if (IsUpperTriangular()){
				for (std::size_t i = y.size() - 1; i > -1 ; --i){
					for (std::size_t j = y.size(); j > i; --j)
					{
						y[i] -= (*this)(i, j) * y[j];
					}
					y[i] /= (*this)(i, i);
				};
				return y;
			};
			assert(WeCanSolveIt);
		};

		//
	};// END DenseMatrixContainer class

//#include "DenseMatrixContainerForBool.h"

	//
}// END namespace

#endif