#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string bwt_encode(const string& source) {
        int p[1000] = {0}, c[1000] = {0}, cnt[1000] = {0}, pn[1000], cn[1000], n = source.length() + 1, classes = 1;
        string s = source + '~';
        for (int i = 0; i < n; i++) {
                cnt[s[i]]++;
        }
        for (char c = 32; c < 127; c++) {
                cnt[c] += cnt[c - 1];
                // the last index in p[] on which string c* could be placed
                // numbering from 1
        }
        for (int i = 0; i < n; i++) {
                p[--cnt[s[i]]] = i;
        }
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) {
                if (s[p[i]] != s[p[i - 1]]) {
                        classes++;
                }
                c[p[i]] = classes - 1;
        }
        for (int h = 0; (1 << h) < n; h++) {
                for (int i = 0; i < n; i++) {
                        pn[i] = p[i] - (1 << h);
                        if (pn[i] < 0) {
                                pn[i] += n;
                        }
                }
                for (int i = 0; i < 1000; i++) {
                        cnt[i] = 0;
                }
                for (int i = 0; i < n; i++) {
                        cnt[c[pn[i]]]++;
                }
                for (int i = 1; i < classes; i++) {
                        cnt[i] += cnt[i - 1];
                }
                for (int i = n - 1; i >= 0; i--) {
                        p[--cnt[c[pn[i]]]] = pn[i];
                }
                cn[p[0]] = 0;
                classes = 1;
                for (int i = 1; i < n; i++) {
                        int mid1 = (p[i] + (1 << h)) % n,  mid2 = (p[i - 1] + (1 << h)) % n;
                        if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2]) {
                                ++classes;
                        }
                        cn[p[i]] = classes - 1;
                }
                for (int i = 0; i < n; i++) {
                        c[i] = cn[i];
                }
        }
        string answer = "";
        for (int i = 0; i < n; i++) {
                answer += s[(p[i] + n - 1) % n];
                /* printing the BWT table */
                /*
                for (int j = 0; j < n; j++) {
                        cout << s[(p[i] + j) % n];
                }
                cout << endl;
                */
        }
        return answer;
}

bool comp(const pair<char, int> a, const pair<char, int> b) {
        return a.first < b.first;
}

string bwt_decode(const string& code) {
        pair<char, int> c[code.length() + 1];
        int n = code.length();
        string answer = "";
        for (int i = 0; i < n; i++) {
                c[i] = make_pair(code[i], i);
        }
        stable_sort(c, c + n, &comp);
        int start = 0;
        for (int i = 0; i < n && code[i] != '~'; i++, start++);
        for (int i = 0; i < n - 1; i++) {
                answer += c[start].first;
                start = c[start].second;
        }
        return answer;
}

int main() {
        string s;
        cin >> s;
        cout << bwt_decode(bwt_encode(s)) << endl;
}
