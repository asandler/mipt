#include <string>
#include <iostream>
#include <vector>

using namespace std;

string reverse(const string& s) {
    string answer;
    for (int i = s.length() - 1; i >= 0; i--) {
        answer += s[i];
    }
    return answer;
}

vector<int> p(const string& s) {
    vector<int> answer;
    answer.push_back(0);
    for (int i = 1; i < s.length(); i++) {
        int temp = answer[i - 1];
        while (temp > 0 && s[i] != s[temp]) {
            temp = answer[temp - 1];
        }
        if (s[i] == s[temp]) {
            temp++;
        }
        answer.push_back(temp);
    }
    return answer;
}

int main() {
    string s, t;
    cin >> s >> t;
    string s1 = t + " " + s;
    string s2 = reverse(t) + " " + reverse(s);
    vector<int> pref = p(s1);
    vector<int> _pref = p(s2);
    for (int i = t.length() + 1; i <= t.length() + s.length(); i++) {
        if (pref[i - 1] + _pref[2 * t.length() + s.length() - i] >= t.length() - 1) {
            cout << "found at " << i - pref[i - 1] - t.length() - 1 << ": ";
            for (int j = i - pref[i - 1]; j < i; j++) {
                cout << s1[j];
            }
            cout << s1[i];
            for (int j = t.length() - _pref[2 * t.length() + s.length() - i]; j < t.length(); j++) {
                cout << s1[j];
            }
            cout << endl;
        }
    }
    return 0;
}
