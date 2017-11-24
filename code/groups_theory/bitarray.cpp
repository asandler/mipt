#include "bitarray.h"

Bit::Bit(char *_byte, char _delta):
	byte(_byte), delta(_delta)
{
	
}

bool Bit::operator!()
{
	return !(*byte & delta);
}

bool Bit::operator=(bool t)
{
	if (t)
		*byte |= delta;
	else
		*byte &= ~delta;
	return t;
}

Bit::operator size_t()
{
	return !(*this) ? 0 : 1;
}

BitArray::BitArray()
{
	array = new char[0];
}

BitArray::BitArray(size_t n)
{
	array = new char[n / 8 + 1];
}

BitArray::~BitArray()
{
	delete []array;
}

void BitArray::Resize(size_t n)
{
	delete []array;
	array = new char[n / 8 + 1];
}

Bit BitArray::operator[](size_t a) const
{
	return Bit(array + a / 8, 1 << a % 8);
}
