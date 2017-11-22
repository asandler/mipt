#include "factorial.h"

Factorial::Factorial(size_t n)
{
	data = new size_t[n + 1];
	data[0] = 1;
	for (size_t i = 1; i <= n; ++i)
	{
		data[i] = data[i - 1] * i;
	}
}

Factorial::~Factorial()
{
	delete []data;
}

size_t Factorial::operator[](int i) const
{
	return data[i];
}
