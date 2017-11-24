#pragma once
#include <stdlib.h>

class Factorial
{
	public:
		Factorial(size_t);
		~Factorial();
		size_t operator[](int) const;
	private:
		size_t *data;
};
