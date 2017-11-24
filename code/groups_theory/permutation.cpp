#include "permutation.h"

Permutation::Permutation(int _n) {
    n = (size_t)_n;
    data = new int[n];
    h = new int[n];
    hash = 0;
    for (size_t i = 0; i < n; ++i) {
        data[i] = i;
        h[i] = 0;
    }
}

Permutation::Permutation(int _n, size_t _hash) {
    n = (size_t)_n;
    hash = _hash;
    data = new int[n];
    h = new int[n];
    memset(h, 0, n * sizeof(int));
    bool* used = new bool[n + 1];
    memset(used, 0, (n + 1) * sizeof(bool));
    Factorial fact(n);
    for (size_t i = 0; i < n; ++i) {
        size_t j = 0, temp = _hash / fact[n - i - 1];
        for (size_t count = -1; j < n; ++j) {
            if (!used[j]) {
                ++count;
            }
            if (temp == count) {
                break;
            }
        }
        data[i] = j;
        used[j] = true;
        _hash %= fact[n - i - 1];
        for (size_t k = 0; k < i; ++k) {
            if (data[i] < data[k]) {
                ++h[k];
            }
        }
    }
    delete[] used;
}

Permutation::Permutation(const Permutation& p) {
    n = p.n;
    hash = p.hash;
    data = new int[n];
    h = new int[n];
    memcpy(data, p.data, n * sizeof(int));
    memcpy(h, p.h, n * sizeof(int));
}

Permutation::~Permutation() {
    delete[] data;
    delete[] h;
}

int Permutation::operator[](int i) const {
    return data[i];
}

Permutation& Permutation::operator=(const Permutation& p) {
    n = p.n;
    hash = p.hash;
    memcpy(data, p.data, n * sizeof(int));
    memcpy(h, p.h, n * sizeof(int));
    return *this;
}

bool Permutation::operator<(const Permutation& p) const {
    for (size_t i = 0; i < n; ++i) {
        if (data[i] < p.data[i]) {
            return true;
        }
        if (data[i] > p.data[i]) {
            return false;
        }
    }
    return false;
}

bool Permutation::operator==(const Permutation& p) const {
    for (size_t i = 0; i < n; ++i) {
        if (data[i] != p.data[i]) {
            return false;
        }
    }
    return true;
}

Permutation Permutation::Swap(int p, int q) {
    if (p > q)
        std::swap(p, q);
    Permutation result(*this);
    for (int i = p + 1; i < q; i++) {
        if (data[i] > data[q] && data[p] > data[i])
            result.h[i]--;
        if (data[i] < data[q] && data[p] < data[i])
            result.h[i]++;
        if (data[i] < data[p])
            result.h[p]--;
        if (data[i] < data[q])
            result.h[q]++;
    }
    if (data[p] < data[q])
        result.h[q]++;
    else
        result.h[p]--;
    std::swap(result.data[p], result.data[q]);
    std::swap(result.h[p], result.h[q]);
    result.hash = 0;
    Factorial F(n);
    for (size_t i = 0; i < n; i++)
        result.hash += F[n - i - 1] * result.h[i];
    return result;
}

std::ostream& operator<<(std::ostream& os, const Permutation& p) {
    for (size_t i = 0; i < p.size(); ++i) {
        os << p[i] << " ";
    }
    return os;
}
