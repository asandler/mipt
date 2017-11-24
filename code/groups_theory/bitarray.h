#pragma once

#include <string.h>

class Bit {
public:
    Bit(char*, char);
    bool operator!();
    bool operator=(bool);
    operator size_t();

private:
    char *byte, delta;
};

class BitArray {
public:
    BitArray();
    BitArray(size_t);
    ~BitArray();
    void Resize(size_t);
    Bit operator[](size_t a) const;

private:
    char* array;
};
