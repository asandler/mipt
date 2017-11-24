#include <iostream>
#include <vector>

using namespace std;

struct item {
    int parent;
    bool if_left;
    double data;
};

typedef vector<item> SuperVector;

SuperVector a;

double RMQ(int _a, int _b) {
    if (_a > _b)
        return 1000000000;
    if (_a == _b)
        return a[_a].data;
    int lev = _a, rig = _b;
    if (!a[_a].if_left)
        lev++;
    if (a[_b].if_left)
        rig--;
    return min(RMQ(a[lev].parent, a[rig].parent), min(a[_a].data, a[_b].data));
}

item Min(const item a, const item b) {
    if (a.data < b.data)
        return a;
    else
        return b;
}

int main() {
    int n, dva = 1;
    cin >> n;
    while (dva < n)
        dva *= 2;
    item null_item;
    null_item.data = 1000000000;
    null_item.parent = -1;
    null_item.if_left = false;
    for (int i = 0; i < n; ++i) {
        item d;
        cin >> d.data;
        a.push_back(d);
    }
    for (int i = n; i < dva; ++i)
        a.push_back(null_item);
    int left = 0, right = dva;
    while (left < right - 1) {
        a.push_back(Min(a[left], a[left + 1]));
        a[left].if_left = true;
        a[left + 1].if_left = false;
        a[left].parent = a[left + 1].parent = right;
        left += 2;
        right++;
    }
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int _a, _b;
        cin >> _a >> _b;
        cout << RMQ(_a, _b - 1) << endl;
    }
    return 0;
}