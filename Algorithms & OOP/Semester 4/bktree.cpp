#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int DistTable[26][26] = {
    {0, 5, 3, 2, 2, 3, 4, 5, 7, 6, 7, 8, 7, 6, 8, 9, 1, 3, 1, 4, 6, 4, 1, 2, 5, 1},
    {5, 0, 2, 3, 4, 2, 1, 1, 3, 2, 3, 4, 2, 1, 4, 5, 6, 3, 4, 2, 2, 1, 5, 3, 1, 4},
    {3, 2, 0, 1, 2, 1, 2, 3, 5, 4, 5, 6, 4, 3, 6, 7, 4, 1, 2, 2, 4, 1, 3, 1, 3, 2},
    {2, 3, 1, 0, 1, 1, 2, 3, 5, 4, 5, 6, 5, 4, 6, 7, 3, 1, 1, 2, 4, 2, 2, 1, 3, 2},
    {2, 4, 2, 1, 0, 2, 3, 4, 5, 5, 6, 7, 6, 5, 6, 7, 2, 1, 1, 2, 4, 3, 1, 1, 3, 2},
    {3, 2, 1, 1, 2, 0, 1, 2, 4, 3, 4, 5, 4, 3, 5, 6, 4, 1, 2, 1, 3, 1, 3, 2, 2, 3},
    {4, 1, 2, 2, 3, 1, 0, 1, 3, 2, 3, 4, 3, 2, 4, 5, 5, 2, 3, 1, 2, 1, 4, 3, 1, 4},
    {5, 1, 3, 3, 4, 2, 1, 0, 2, 1, 2, 3, 2, 1, 3, 4, 6, 3, 4, 2, 1, 2, 5, 4, 1, 5},
    {7, 3, 5, 5, 5, 4, 3, 2, 0, 1, 1, 2, 1, 2, 1, 2, 7, 4, 6, 3, 1, 4, 6, 6, 2, 7},
    {6, 2, 4, 4, 5, 3, 2, 1, 1, 0, 1, 2, 1, 1, 2, 3, 7, 4, 5, 3, 1, 3, 6, 5, 2, 6},
    {7, 3, 5, 5, 6, 4, 3, 2, 1, 1, 0, 1, 1, 2, 1, 2, 8, 5, 6, 4, 2, 4, 7, 6, 3, 7},
    {8, 4, 6, 6, 7, 5, 4, 3, 2, 2, 1, 0, 2, 3, 1, 1, 9, 6, 7, 5, 3, 5, 8, 7, 4, 8},
    {7, 2, 4, 5, 6, 4, 3, 2, 1, 1, 1, 2, 0, 1, 2, 3, 8, 5, 6, 4, 2, 3, 7, 5, 3, 6},
    {6, 1, 3, 4, 5, 3, 2, 1, 2, 1, 2, 3, 1, 0, 3, 4, 7, 4, 5, 3, 1, 2, 6, 4, 2, 5},
    {8, 4, 6, 6, 6, 5, 4, 3, 1, 2, 1, 1, 2, 3, 0, 1, 8, 5, 7, 4, 2, 5, 7, 7, 3, 8},
    {9, 5, 7, 7, 7, 6, 5, 4, 2, 3, 2, 1, 3, 4, 1, 0, 9, 6, 8, 5, 3, 6, 8, 8, 4, 9},
    {1, 6, 4, 3, 2, 4, 5, 6, 7, 7, 8, 9, 8, 7, 8, 9, 0, 3, 2, 4, 6, 5, 1, 3, 5, 2},
    {3, 3, 1, 1, 1, 1, 2, 3, 4, 4, 5, 6, 5, 4, 5, 6, 3, 0, 2, 1, 3, 2, 2, 2, 2, 3},
    {1, 4, 2, 1, 1, 2, 3, 4, 6, 5, 6, 7, 6, 5, 7, 8, 2, 2, 0, 3, 5, 3, 1, 1, 4, 1},
    {4, 2, 2, 2, 2, 1, 1, 2, 3, 3, 4, 5, 4, 3, 4, 5, 4, 1, 3, 0, 2, 1, 3, 3, 1, 4},
    {6, 2, 4, 4, 4, 3, 2, 1, 1, 1, 2, 3, 2, 1, 2, 3, 6, 3, 5, 2, 0, 3, 5, 5, 1, 6},
    {4, 1, 1, 2, 3, 1, 1, 2, 4, 3, 4, 5, 3, 2, 5, 6, 5, 2, 3, 1, 3, 0, 4, 2, 2, 3},
    {1, 5, 3, 2, 1, 3, 4, 5, 6, 6, 7, 8, 7, 6, 7, 8, 1, 2, 1, 3, 5, 4, 0, 2, 4, 1},
    {2, 3, 1, 1, 1, 2, 3, 4, 6, 5, 6, 7, 5, 4, 7, 8, 3, 2, 1, 3, 5, 2, 2, 0, 4, 1},
    {5, 1, 3, 3, 3, 2, 1, 1, 2, 2, 3, 4, 3, 2, 3, 4, 5, 2, 4, 1, 1, 2, 4, 4, 0, 5},
    {1, 4, 2, 2, 2, 3, 4, 5, 7, 6, 7, 8, 6, 5, 8, 9, 2, 3, 1, 4, 6, 3, 1, 1, 5, 0}
};

int CalcSubstitutionPenalty(char a, char b) {
    return DistTable[a - 'a'][b - 'a'];
}

int min(int a, int b, int c) {
        return min(min(a, b), c);
}

int Distance(const string& a, const string& b) {
        int d[a.length() + 1][b.length() + 1];
        for (size_t i = 0; i <= a.length(); i++) {
                d[i][0] = i * 2;
        }
        for (size_t j = 0; j <= b.length(); j++) {
                d[0][j] = j * 2;
        }
        for (size_t i = 1; i <= a.length(); i++) {
                for (size_t j = 1; j <= b.length(); j++) {
                        d[i][j] = min(
                                d[i][j - 1] + 2, //insertion
                                d[i - 1][j] + 2, //deletion
                                d[i - 1][j - 1] + CalcSubstitutionPenalty(a[i - 1], b[j - 1]) //substitution
                        );
                        if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                                d[i][j] = min(
                                        d[i][j],
                                        d[i - 2][j - 2] + 1 //transposition
                                );
                        }
                }
        }
        /*
        for (size_t i = 0; i <= a.length(); i++) {
                for (size_t j = 0; j <= b.length(); j++) {
                        cout << d[i][j] << ' ';
                }
                cout << endl;
        }
        */
        return d[a.length()][b.length()];
}

struct node {
        string s;
        map <int, node> children;
};

class BKtree {
public:
        
        BKtree(const string &str) {
                __root.s = str;
        }

        void Add(const string &str) {
                node* cur = &__root;
                int d = Distance(cur->s, str);
                while (cur->children[d].s != "") {
                        cur = &(cur->children[d]);
                        d = Distance(cur->s, str);
                }
                cur->children[d].s = str;
        }

        vector<string> Find(const string &str, int penalty) {
                __answer.clear();
                find(str, penalty, __root);
                return __answer;
        }

        void Print() {
                print(__root, 0);
        }

private:
        
        void print(node vertex, int tabsize) {
                for (int i = 0; i < tabsize * 2; i++) {
                        cout << ' ';
                }
                cout << vertex.s << endl;
                for (map<int, node>::iterator it = vertex.children.begin(); it != vertex.children.end(); it++) {
                        print(it->second, tabsize + 1);
                }
        }

        void find(const string &str, int penalty, node root) {
                int d = Distance(root.s, str);
                if (d <= penalty) {
                        __answer.push_back(root.s);
                }
                for (map<int, node>::iterator it = root.children.begin(); it != root.children.end(); ++it) {
                        int d1 = it->first;
                        if (d1 <= d + penalty && d1 >= d - penalty) {
                                find(str, penalty, it->second);
                        }
                }
        }

        node __root;
        vector<string> __answer;
};

int main() {
        int n = 0, m = 0, penalty = 0;
        string s;
        cin >> n;
        cin >> s;
        BKtree T(s);
        for (int i = 0; i < n - 1; i++) {
                cin >> s;
                T.Add(s);
        }
//        T.Print();
        cin >> m;
        for (int i = 0; i < m; i++) {
                cin >> s >> penalty;
                cout << s << ' ' << penalty << ':' << endl;
                vector<string> answer = T.Find(s, penalty);
                for (vector<string>::iterator it = answer.begin(); it != answer.end(); it++) {
                        cout << "   " << *it << endl;
                }
        }
        return 0;
}
