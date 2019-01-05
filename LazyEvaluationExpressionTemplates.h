
#ifndef _FususLazyEvaluationExpressionTemplates_
#define _FususLazyEvaluationExpressionTemplates_

namespace FususMatrix{

	// Scalar class.
	// Stores the scalar.
	template<typename T>
	class Scalar{
	private:
		T const s;
	public:
		// Constructor sets the value.
		Scalar(T const& v) : s(v){
		};

		// Any access returns the same value stored.
		T operator[](std::size_t index) const {
			return s;
		};

		// It has size 0.
		std::size_t size() const {
			return 0;
		};

		std::vector<std::size_t> getSizesAlongEachDimension() const {
			return std::vector<std::size_t>(0);
		}
	};

	// Reference traits.
	// These are used to have common handling of scalars and matrices.
	// Matrices are referenced, while scalars are stored by value.
	template<typename T>
	class Traits{
	public:
		typedef T const& ExprRef;
		// Type to refer to, is constant reference.
	};
	// Partial specialization for scalars.
	template<typename T>
	class Traits < Scalar<T> > {
	public:
		typedef Scalar<T> ExprRef;
		// Type to refer to, is ordinary value.
	};

	// Addition class.
	// Stores (traited) references to the summands.
	// Returns addition of elements if asked for a value.
	template<typename T, typename Operand1, typename Operand2>
	class Addition {
	private:
		typename Traits<Operand1>::ExprRef operand1;
		typename Traits<Operand2>::ExprRef operand2;

	public:
		Addition(Operand1 const& a, Operand2 const& b)
			: operand1(a), operand2(b){
		};

		T operator[] (std::size_t index) const {
			return operand1[index] + operand2[index];
		};

		std::size_t size() const {
			return operand1.size() != 0 ? operand1.size() : operand2.size();
		};

		std::vector<std::size_t> getSizesAlongEachDimension() const {
			assert(operand1.getSizesAlongEachDimension() == operand2.getSizesAlongEachDimension());
			return operand1.getSizesAlongEachDimension();
		};
	};

	// Subtraction class
	// Stores (traited) references to the summands.
	// Returns subtraction of elements if asked for a value.
	template<typename T, typename Operand1, typename Operand2>
	class Subtraction {
	private:
		typename Traits<Operand1>::ExprRef operand1;
		typename Traits<Operand2>::ExprRef operand2;

	public:
		Subtraction(Operand1 const& a, Operand2 const& b)
			: operand1(a), operand2(b){
		};

		T operator[] (std::size_t index) const {
			return operand1[index] - operand2[index];
		};

		std::size_t size() const {
			return operand1.size() != 0 ? operand1.size() : operand2.size();
		};

		std::vector<std::size_t> getSizesAlongEachDimension() const {
			return operand1.getSizesAlongEachDimension();
		};
	};

	// Multiplication class
	// Stores (traited) references to the summands.
	// Returns multiplication of elements if asked for a value.
	template<typename T, typename Operand1, typename Operand2>
	class Multiplication{
	private:
		typename Traits<Operand1>::ExprRef operand1;
		typename Traits<Operand2>::ExprRef operand2;

	public:
		Multiplication(Operand1 const& a, Operand2 const& b)
			: operand1(a), operand2(b){
		};

		T operator[] (std::size_t index) const {
			return operand1[index] * operand2[index];
		};
		std::size_t size() const {
			return operand1.size() != 0 ? operand1.size() : operand2.size();
		};

		std::vector<std::size_t> getSizesAlongEachDimension() const {
			return operand1.size() != 0 ? operand1.getSizesAlongEachDimension() : operand2.getSizesAlongEachDimension();
		};
	};

	// Division class.
	// Stores (traited) references to the summands.
	// Returns division of elements if asked for a value.
	template<typename T, typename Operand1, typename Operand2>
	class Division{
	private:
		typename Traits<Operand1>::ExprRef operand1;
		typename Traits<Operand2>::ExprRef operand2;

	public:
		Division(Operand1 const& a, Operand2 const& b)
			: operand1(a), operand2(b){
		};

		T operator[] (std::size_t index) const {
			return operand1[index] / operand2[index];
		};
		std::size_t size() const {
			return operand1.size() != 0 ? operand1.size() : operand2.size();
		};

		std::vector<std::size_t> getSizesAlongEachDimension() const {
			return operand1.size() != 0 ? operand1.getSizesAlongEachDimension() : operand2.getSizesAlongEachDimension();
		};
	};
	//
}// END namespace FususMatrix


#endif