#pragma once
#include <iostream>
#include <string.h>
#include "factorial.h"

class Permutation
{
	public:
		Permutation(int);
		Permutation(int, size_t);
		Permutation(const Permutation&);
		~Permutation();
		int operator[](int) const;
		Permutation& operator=(const Permutation&);
		bool operator<(const Permutation&) const;
		bool operator==(const Permutation&) const;
		Permutation Swap(int, int);
		size_t size() const { return n; };
		size_t Hash() const { return hash; };
	private:
		size_t n, hash;
		int *data, *h;
};

std::ostream& operator<<(std::ostream&, const Permutation&);
