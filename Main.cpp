#include <iostream>
#include <chrono>

#include "Matrix.h"
#include "SparseMatrix.h"

using namespace std;
using namespace std::chrono;
using namespace FususMatrix;

// To print to console easier.
#define PRINT(STR)\
std::cout << STR << std::endl
////////////////////////////////////////////////////////////////

// To stop for an Enter
#define STOP\
	std::cout << "_______________________________________________________________________________\n\n        Press Enter to continue ..." << std::endl; std::getchar(); std::cout << "\n-----------\n Page " << Page << ": \n-----------" << std::endl; ++Page
////////////////////////////////////////////////////////////////

int Page{ 1 };

void logo(){
	std::cout << "_______________________________________________________________________________\n";
	std::cout << "_______________________________________________________________________________\n\n";
	std::cout << "                             Fusus Matrix                                      \n";
	std::cout << "_______________________________________________________________________________\n";
	std::cout << "_______________________________________________________________________________" << std::endl;
	STOP;
};

void tests(){
	// The matrix A.
	PRINT("\nMatrix<double, 2> A(3,3);\n");
	PRINT("This creates a 2-dimensional matrix with 3 rows and 3 columns.");
	PRINT("The matrix is initialized to zero.");
	Matrix<double> A(3, 3);
	PRINT("\nA =\n" << A << "\n");
	PRINT("The matrix was printed to screen using an overloaded operator<<\n");
	STOP;

	// The column vector B.
	PRINT("\nMatrix<double,2> B(3, 1);\n");
	PRINT("This creates a 2-dimensional matrix with 3 rows and 1 column.\n");
	Vector<> B(3, 1);
	PRINT("We can assign values to the components of the matrix using operator().");
	PRINT("\nB(0,0)=1;");
	B(0, 0) = 1;
	PRINT("B(1,0)=2;");
	B(1, 0) = 2;
	PRINT("B(2,0)=3;");
	B(2, 0) = 3;
	PRINT("\nB =\n" << B);
	STOP;

	// High dimensional matrix of bool
	PRINT("\nWe can create matrices of any dimension.");
	PRINT("And the type of the elements can be bool!");
	PRINT("\nMatrix<bool,4> X(5,6,7,8);\n");
	PRINT("Creates a 4-D matrix of size 5x6x7x8.");
	Matrix<bool, 4> X(5, 6, 7, 8);
	PRINT("We do this by using std::deque or std::vector as underlying container\ndepending on the type of the elements of the matrix.");
	PRINT("We can also access its elements with operator()\n\n X(2,2,2,2) = true;");
	X(2, 2, 2, 2) = true;
	STOP;

	// Assignation of value to matrix A.
	PRINT("Assigning nonzero values to A.");
	for (std::size_t i = 0; i < 3; ++i){
		for (std::size_t j = 0; j < 3; ++j){
			A(i, j) = 1 + i + 2 * j;
		};
	};
	PRINT("\nA =\n" << A << "\n");
	STOP;

	// Weak transposition.
	PRINT("With A.transpose() a switch is set to \nread or access the elements of A as if transposed.");
	A.transpose();
	PRINT("\nA =\n" << A << "\n");
	STOP;

	// Strong transposition.
	PRINT("With A.strongTranspose() the elements actually get transposed.");
	PRINT("The strong transposition is done by a cache oblivious algorithm.");
	PRINT("The idea is to, recursively, divide the matrix in half\n and then transpose each piece.");
	A.strongTranspose();
	PRINT("\nA =\n" << A << "\n");
	STOP;

	// Matrix C.
	PRINT("Matrix<> C(B); copy-constructs another column vector from B.");
	Matrix<double> C(B);
	PRINT("\nC =\n" << C << "\n");
	PRINT("We can use arithmetic operations.");
	PRINT("\nC = 2.0 * B + A.Multiply(B);");
	C = 2.0 * B + A.Multiply(B);
	PRINT("\nC =\n" << C << "\n");
	STOP;

	// Scalars and 1x1 matrices
	PRINT("We identify scalars and 1x1 matrices for the arithmetic operations.");
	PRINT("For example:");
	PRINT("Matrix<double,2> P(1,1);");
	PRINT("Matrix<double,2> Q(1,1);");
	PRINT("And we can do\n\n Q = 2.3 + P + 2.5;");
	Matrix<double, 2> P(1, 1);
	Matrix<double, 2> Q(1, 1);
	Q = 2.3 + P + 2.5;
	PRINT("\nQ=\n" << Q);
	STOP;

	// Lazy evaluation.
	PRINT("Expressions are computed using lazy evaluation.");
	PRINT("\nC = 5.0*(A.Multiply(B)) + 2.3*B + C/C;");
	C = 5.0*(A.Multiply(B)) + 2.3*B + C / C;
	PRINT("\nC =\n" << C << "\n");
	PRINT("Lazy evaluation is obtained using Expression Templates.");
	PRINT("This means that each operation doesn't compute the result.");
	PRINT("They return a Matrix-object containing an Operation-object\n instead of a Matrix-Container.");
	PRINT("The Operation-objects contain only references to the operands.");
	PRINT("Operation-objects also know how to compute the operation.");
	PRINT("The Operation-objects are passed in template parameters.");
	PRINT("This means that the processing of the expression is done in Compilation-Time.");
	PRINT("During Execution-Time, the components of C are computed one-by-one.");
	STOP;

	// Lazy evaluation part 2.
	PRINT("Creating a big matrix.");
	int x{ 100 }, y{ 200 }, z{ 300 };
	PRINT("Matrix<double,3> K(" << x << "," << y << "," << z << ");");
	Matrix<double, 3> K(x, y, z);
	PRINT("And computing with it.");
	PRINT("\nK = (K + K) + (K + (K + K));\n");
	std::chrono::high_resolution_clock::time_point t1 = high_resolution_clock::now();
	K = (K + K) + (K + (K + K));
	std::chrono::high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	PRINT("Done!");
	PRINT("It took us: " << duration << " microseconds.");
	PRINT("To perform the " << 4 * x*y*z << "(sums) + " << 2 * x*y*x << "(read/write) operations required.");
	PRINT("This means " << static_cast<int>(1000 * (6 * x*y*z) / static_cast<double>(duration)) << " operations/nanosecond.");
	STOP;

	// Testing matrix D with IsTriangular.
	PRINT("Testing if a matrix is lower triangular.");
	PRINT("Let's test the matrix ");
	Matrix<double, 2> D(3, 3);
	D(0, 0) = 1;
	D(0, 1) = 2;
	D(0, 2) = 3;
	D(1, 0) = 0;
	D(1, 1) = 4;
	D(1, 2) = 0;
	D(2, 0) = 5;
	D(2, 1) = 6;
	D(2, 2) = 7;
	PRINT("\nD= \n" << D << "\n");
	PRINT("With D.IsLowerTriangular();\n");
	std::getchar();
	bool IsLT = D.IsLowerTriangular();
	PRINT("The matrix D is " << (IsLT ? "" : "not ") << "lower triangular");
	STOP;

	// Testing matrix E with IsTriangular.
	PRINT("Let's take the matrix ");
	Matrix<double, 2> E(3, 3);
	E(0, 0) = 1;
	E(0, 1) = 0;
	E(0, 2) = 0;
	E(1, 0) = 2;
	E(1, 1) = 3;
	E(1, 2) = 0;
	E(2, 0) = 4;
	E(2, 1) = 5;
	E(2, 2) = 6;
	PRINT("\nE= \n" << E << "\n");
	PRINT("With E.IsLowerTriangular();\n");
	std::getchar();
	IsLT = E.IsLowerTriangular();
	PRINT("The matrix E is " << (IsLT ? "" : "not ") << "lower triangular\n");
	STOP;

	PRINT("Let us solve a lower triangular system of linear equations.");
	PRINT("\nE.span(B);\n");
	PRINT("This returns a vector. Its components are the coefficients needed to span B");
	PRINT(" as a linear combination of the columns of E.");
	PRINT("This is the same as computing E^{-1}B or solving the system Ex=B");
	Matrix<> F(E.span(B));
	PRINT("\nF =\n" << F);
	STOP;

	// Sparse Matrices
	PRINT("We also have began to code support for square sparse matrices.");
	PRINT("\nSparseMatrix<double> S(3, 3);\n");
	SparseMatrix<double> S(3, 3);
	STOP;

	PRINT("\nThat's all folks!");
	STOP;
};



int main(){

	// Logo of the presentation.
	logo();
	tests();

	return 0;
}