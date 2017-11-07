#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector <size_t> p;

string KMP(const string& s, const string& t) {
        string k = t + " " + s;
        p.push_back(0);
        for (size_t i = 1; i < k.length(); i++) {
                size_t temp = p[i - 1];
                while (temp > 0 && k[i] != k[temp]) {
                        temp = p[temp - 1];
                }
                if (k[i] == k[temp]) {
                        temp++;
                }
                p.push_back(temp);
                if (temp == t.length()) {
                        return "YES";
                }
        }
        return "NO";
}

int main() {
        string s, t;
        cin >> s >> t;
        cout << KMP(s, t) << endl;
        return 0;
}
