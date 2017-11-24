#include "graph.h"
#include <set>
#include <exception>
#include <vector>

using namespace std;

class cycle_Exception: public exception {
    virtual const char* what() const throw () {
        return "Order couldn't be restored";
    }
};

class notFullOrder_Exception: public exception {
    virtual const char* what() const throw () {
        return "Order isn't full";
    }
};

void parse_vertices(TGraph* G, string s, map<string, int>& S) {
    for (size_t i = 0; i < s.length(); i++) {
        G->AddVertex(string(1, s[i]));
        if (S[string(1, s[i])] != 2) {
            S[string(1, s[i])] = 1;
        }
    }
}

string input(TGraph* G) {
    string s, t;
    map<string, int> null_degree;
    cin >> s;
    parse_vertices(G, s, null_degree);
    while (!cin.eof()) {
        cin >> t;
        parse_vertices(G, t, null_degree);
        size_t i;
        for (i = 0; i < min(s.length(), t.length()) && s[i] == t[i]; i++)
            ;
        if (i < min(s.length(), t.length())) {
            G->AddEdge(string(1, s[i]), string(1, t[i]), 1);
            null_degree[string(1, t[i])] = 2;
        }
        s = t;
    }
    s = "";
    for (map<string, int>::iterator it = null_degree.begin(); it != null_degree.end(); it++) {
        if (it->second == 1) {
            s = it->first;
        }
    }
    if (s == "") {
        throw cycle_Exception();
    }
    return s;
}

void top_sort(TGraph* G, const string& cur, map<string, int>& colors, vector<string>& answer) {
    colors[cur] = 1;
    for (TEdgeIterator it = G->ListNeighbours(cur); it.IsValid(); it.GetNext()) {
        if (colors[it.To()] == 1) {
            throw cycle_Exception();
        }
        if (colors[it.To()] != 2) {
            top_sort(G, it.To(), colors, answer);
        }
    }
    colors[cur] = 2;
    answer.push_back(cur);
}

vector<string> check(TGraph* G, const string& start) {
    map<string, int> colors;
    vector<string> sorted;
    try {
        top_sort(G, start, colors, sorted);
    } catch (cycle_Exception& e) {
        throw e;
    }
    if (sorted.size() < G->VertexCount()) {
        throw notFullOrder_Exception();
    }
    for (vector<string>::reverse_iterator it = sorted.rbegin(); it != --(sorted.rend()); it++) {
        if (!G->ExistEdge(*it, *(it + 1))) {
            throw notFullOrder_Exception();
        }
    }
    return sorted;
}

int main() {
    auto_ptr<TGraph> G(new TMapGraph());
    try {
        string s = input(G.get());
        vector<string> answer = check(G.get(), s);
        for (vector<string>::reverse_iterator it = answer.rbegin(); it != answer.rend(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    } catch (exception& e) {
        cerr << e.what() << endl;
        return -1;
    }
    return 0;
}
