#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

template <typename T, typename Cmp = std::less <T> >
class THeap {
public:
        void Push(const T& t) {
                a.push_back(t);
                heap_up(a.size() - 1);
        }
        T Pop() {
                T t = a[0];
                a[0] = a.back();
                a.pop_back();
                heap_down(0);
                return t;
        }
        const T Top() const {
                return a[0];
        }
        bool Empty() const {
                return a.empty();
        }
        size_t Find(const T& t) {
                return find(a.begin(), a.end(), t) - a.begin();
        }
        void Update(const T& t, size_t place) {
                if (place < a.size()) {
                        a[place] = t;
                        heap_up(place);
                        heap_down(place);
                } else {
                        Push(t);
                }
        }
        void Print() {
                for (typename vector<T>::iterator it = a.begin(); it != a.end(); it++) {
                        pair <string, double> s = static_cast<pair <string, double> >(*it);
                        cout << s.first << ' ' << s.second << endl;
                }
                cout << "----------" << endl;
        }

private:
        vector <T> a;
        void heap_up(size_t i) {
                while (i && Cmp()(a[(i - 1) / 2], a[i])) {
                        swap(a[i], a[(i - 1) / 2]);
                        i = (i - 1) / 2;
                }
        }
        void heap_down(size_t i) {
                while (true) {
                        size_t child = i * 2 + 1;
                        if (child >= a.size())
                                break;
                        if (child + 1 < a.size() && Cmp()(a[child], a[child + 1]))
                                child++;
                        if (Cmp()(a[i], a[child]))
                                swap(a[i], a[child]);
                        i = child;
                }
        }
};
