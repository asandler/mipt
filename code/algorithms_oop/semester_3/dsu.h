#include <iostream>
#include <map>

using namespace std;

template <typename T>
void Swap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;
}

template <typename T> class DSU {
public:
        void Make_set(T v) {
                parent[v] = v;
                rank[v] = 0;
        }

        T Find_set(T v) {
                if (v == parent[v])
                        return v;
                return parent[v] = Find_set(parent[v]);
        }

        void Union_sets(T a, T b) {
                a = Find_set(a);
                b = Find_set(b);
                if (a != b) {
                        if (rank[a] < rank[b])
                                Swap(a, b);
                        parent[b] = a;
                        if (rank[a] == rank[b])
                                ++rank[a];
                }
        }

private:
        map<T, T> parent;
        map<T, int> rank;
};
