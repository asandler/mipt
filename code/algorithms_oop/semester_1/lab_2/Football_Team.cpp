#include <iostream>
#include <algorithm>

using namespace std;

struct player {
    double ef;
    int number;
};

int comp(const void* a, const void* b) {
    const player* name1 = (const player*)a;
    const player* name2 = (const player*)b;
    if (name1->ef > name2->ef)
        return -1;
    else if (name1->ef == name2->ef)
        return 0;
    else
        return 1;
}

int main() {
    int k[100], t[100], n, sum = 0, answer = 0;
    player c[100];
    cout << "enter n" << endl;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> k[i] >> t[i];
        c[i].ef = (double)k[i] / (double)t[i];
        c[i].number = i;
    }
    qsort(c, n, sizeof(c[0]), &comp);
    for (int i = 1; i < n; ++i) {
        sum = sum + t[c[i - 1].number];
        answer = answer + k[c[i].number] * sum;
    }
    cout << "answer: " << answer << endl;
    for (int i = 0; i < n; ++i)
        cout << k[c[i].number] << ' ' << t[c[i].number] << endl;
    return 0;
}