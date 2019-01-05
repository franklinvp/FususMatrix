#ifndef _FususMatrixInitializer_
#define _FususMatrixInitializer_

namespace FususMatrix{

	template<typename T, std::size_t N>
	using Matrix_initializer = typename Matrix_init<T, N>::typer;

	template<typename T, std::size_t N>
	struct Matrix_init {
		using typer = std::initializer_list<typename Matrix_init<T, N−1>::type>;
	};

	template<typename T>
	struct Matrix_init<T, 1> {
		using typer = std::initializer_list<T>;
	};

	template<typename T>
	struct Matrix_init<T, 0>; // undefined on purpose
}

#endif