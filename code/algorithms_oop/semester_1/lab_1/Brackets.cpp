#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

#define CONSOLE

class ICharIterator {
public:
    virtual bool GetNextChar(char&) = 0;
};

class TArrayIterator: public ICharIterator {
public:
    TArrayIterator(const string& st)
        : s(st)
        , it(0)
    {
    }
    virtual bool GetNextChar(char& ch) {
        if (it == s.length())
            return false;
        else {
            ch = s[it];
            it++;
            return true;
        }
    }

private:
    string s;
    size_t it;
};

class TFileCharIterator: public ICharIterator {
public:
    TFileCharIterator(const string& st)
        : input(st.c_str())
    {
    }
    virtual bool GetNextChar(char& c) {
        input >> c;
        if (input.eof() || input.fail() || c < 32)
            return false;
        else
            return true;
    }

private:
    ifstream input;
};

char char_pair(char t) {
    switch (t) {
        case ')':
            return '(';
            break;
        case ']':
            return '[';
            break;
        case '>':
            return '<';
            break;
        default:
            return '!';
    }
}

bool CheckSequience(ICharIterator& it) {
    char c;
    vector<char> stack;
    stack.push_back('?');
    while (it.GetNextChar(c)) {
        if (c == '(' || c == '[' || c == '<')
            stack.push_back(c);
        else {
            char t = stack.back();
            if (t != char_pair(c)) {
                stack.push_back('e');
                break;
            } else
                stack.pop_back();
        }
    }
    if (stack.back() == '?')
        return true;
    else
        return false;
}

int main() {
#ifdef CONSOLE
    string s;
    getline(cin, s);
    TArrayIterator it(s);
#else
    TFileCharIterator it("input.txt");
#endif
    bool f = CheckSequience(it);
    f ? cout << "Pravda\n" : cout << "Loj'\n";
    return 0;
}