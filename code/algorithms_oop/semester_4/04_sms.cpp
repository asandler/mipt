#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>

using namespace std;

map<string, pair<string, int>> t9;
string current;
vector<pair<pair<int, int>, string>> found;

class AhoCorasick {
public:
    typedef void (*Callback)(const char* substr, int begin, int end);

    ~AhoCorasick() {
        queue<BorNode*> q;
        for (map<char, BorNode*>::const_iterator iter = root.links.begin();
             iter != root.links.end(); ++iter)
            q.push(iter->second);
        while (!q.empty()) {
            BorNode* current_node = q.front();
            q.pop();
            for (map<char, BorNode*>::const_iterator iter = current_node->links.begin(); iter != current_node->links.end(); ++iter) {
                q.push(iter->second);
            }
            delete current_node;
        }
    }

    void AddString(const char* str) {
        BorNode* node = &root;
        for (const char* s = str; *s; ++s) {
            map<char, BorNode*>::iterator iter = node->links.find(*s);
            if (iter != node->links.end()) {
                node = iter->second;
            } else {
                BorNode* new_node = new BorNode;
                node->links[*s] = new_node;
                node = new_node;
            }
        }
        node->out = words.size();
        words.push_back(str);
    }

    void Init() {
        root.fail = &root;
        queue<BorNode*> q;
        q.push(&root);
        while (!q.empty()) {
            BorNode* current_node = q.front();
            q.pop();
            for (map<char, BorNode*>::const_iterator iter = current_node->links.begin(); iter != current_node->links.end(); ++iter) {
                BorNode* child = iter->second;
                char symb = iter->first;
                q.push(child);

                BorNode* parent_fail = current_node->fail;
                while (true) {
                    map<char, BorNode*>::const_iterator it = parent_fail->links.find(symb);
                    if (it != parent_fail->links.end()) {
                        child->fail = it->second != child ? it->second : &root;
                        if (child->out < 0) {
                            child->out = child->fail->out;
                        }
                        break;
                    }
                    if (parent_fail == &root) {
                        child->fail = &root;
                        break;
                    } else {
                        parent_fail = parent_fail->fail;
                    }
                }
            }
        }
    }

    void Search(const char* str, Callback callback) {
        BorNode* current_node = &root;
        for (int pos = 1; *str; ++str, ++pos) {
            map<char, BorNode*>::const_iterator iter = current_node->links.find(*str);
            while (iter == current_node->links.end()) {
                current_node = current_node->fail;
                iter = current_node->links.find(*str);
                if (current_node == current_node->fail) {
                    break;
                }
            }
            if (iter != current_node->links.end()) {
                current_node = iter->second;
                if (current_node->out >= 0) {
                    callback(words[current_node->out].c_str(), pos - words[current_node->out].length(), pos - 1);
                }
            }
        }
    }

private:
    struct BorNode {
        BorNode()
            : fail(NULL)
            , out(-1)
        {
        }

        map<char, BorNode*> links;
        BorNode* fail;
        int out;
    };

    BorNode root;
    vector<string> words;
};

void count(const char* str, int start, int end) {
    found.push_back(make_pair(make_pair(start + 1, end + 1), str));
}

bool comparator(pair<pair<int, int>, string> val1, pair<pair<int, int>, string> val2) {
    return val1.first.first < val2.first.first || (val1.first.first == val2.first.first && val1.first.second < val2.first.second);
}

string ans() {
    pair<pair<int, string>, int> a[250000];
    a[0] = make_pair(make_pair(0, ""), 0);
    for (int i = 1; i < 250000; i++) {
        a[i] = make_pair(make_pair(0, ""), 1000000000);
    }
    //        sort(found.begin(), found.end(), comparator);
    for (vector<pair<pair<int, int>, string>>::iterator it = found.begin(); it != found.end(); it++) {
        if (a[it->first.second].second > a[it->first.first - 1].second + t9[it->second].second + 1) {
            a[it->first.second] = make_pair(make_pair(it->first.first - 1, it->second), a[it->first.first - 1].second + t9[it->second].second + 1);
        }
    }
    int k = current.length();
    string answer = "";
    while (k > 0) {
        answer = t9[a[k].first.second].first + "R" + answer;
        k = a[k].first.first;
    }
    answer.erase(answer.begin() + answer.length() - 1);
    return answer;
}

template <typename T>
std::string toStr(T aVal) {
    std::ostringstream _oss;
    _oss << aVal;
    return _oss.str();
}

char layout(const char c) {
    switch (c) {
        case 'a':
        case 'b':
        case 'c':
            return '2';
        case 'd':
        case 'e':
        case 'f':
            return '3';
        case 'g':
        case 'h':
        case 'i':
            return '4';
        case 'j':
        case 'k':
        case 'l':
            return '5';
        case 'm':
        case 'n':
        case 'o':
            return '6';
        case 'p':
        case 'q':
        case 'r':
        case 's':
            return '7';
        case 't':
        case 'u':
        case 'v':
            return '8';
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            return '9';
    }
    return '0';
}

map<string, pair<string, int>> T9(const vector<string>& dict) {
    map<string, vector<string>> freq;
    map<string, pair<string, int>> answer;
    for (vector<string>::const_iterator it = dict.begin(); it != dict.end(); it++) {
        string s = "";
        for (size_t i = 0; i < (*it).length(); i++) {
            s += layout((*it)[i]);
        }
        freq[s].push_back(*it);
    }
    for (map<string, vector<string>>::iterator it = freq.begin(); it != freq.end(); it++) {
        size_t k = 0;
        for (vector<string>::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {
            if (k >= (it->second.size() + 1) / 2) {
                answer[*jt] = make_pair(it->first + "D(" + toStr(it->second.size() - k) + ")", it->second.size() - k + it->first.length());
            } else {
                if (k > 0) {
                    answer[*jt] = make_pair(it->first + "U(" + toStr(k) + ")", k + it->first.length());
                } else {
                    answer[*jt] = make_pair(it->first, k + it->first.length());
                }
            }
            k++;
        }
    }
    return answer;
}

int main() {
    int n, q;
    cin >> n;
    while (n != 0) {
        AhoCorasick ak;
        vector<string> dict;
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            dict.push_back(s);
            ak.AddString(s.c_str());
        }
        t9 = T9(dict);
        ak.Init();
        cin >> q;
        for (int i = 0; i < q; i++) {
            found.clear();
            cin >> current;
            ak.Search(current.c_str(), count);
            cout << ans() << endl;
        }
        cin >> n;
    }
    return 0;
}
