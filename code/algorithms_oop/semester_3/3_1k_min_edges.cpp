#include <queue>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "graph.h"

using namespace std;

int SPECIAL_CONSTANT = 0;
string start, finish;
map<pair<string, string>, bool> Answer;

void input(TGraph* G) {
    string s, t;
    int w, E;
    cin >> start >> finish;
    cin >> E;
    for (int i = 0; i < E; i++) {
        cin >> s >> t >> w;
        G->AddEdge(s, t, w);
    }
}

void unzip(TGraph* G, const string& from, const string& to, const string& cur, int weight) {
    if (weight > 1) {
        ostringstream stream;
        stream << "_" << SPECIAL_CONSTANT++;
        string s = stream.str();
        G->AddVertex(s);
        G->AddEdge(cur, s, 1);
        unzip(G, from, to, s, weight - 1);
    } else {
        G->DeleteEdge(from, to);
        G->AddEdge(cur, to, 1);
    }
}

void zip(TGraph* G, const string& from, const string& cur, const string& to, int k, bool answer) {
    if (to[0] == '_') {
        G->DeleteEdge(cur, to);
        zip(G, from, to, G->ListNeighbours(to).To(), k + 1, answer);
    } else {
        G->DeleteEdge(cur, to);
        G->AddEdge(from, to, k);
        Answer[make_pair(from, to)] = answer;
    }
}

map<string, double> BFS(TGraph* G, string start) {
    queue<string> Q;
    map<string, bool> visited;
    map<string, double> answer;
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext())
        answer[it.Get()] = inf;
    answer[start] = 0;
    Q.push(start);
    while (!Q.empty()) {
        string top = Q.front();
        visited[top] = true;
        for (TEdgeIterator it = G->ListNeighbours(top); it.IsValid(); it.GetNext()) {
            if (!visited[it.To()]) {
                Q.push(it.To());
                answer[it.To()] = min(answer[it.To()], answer[it.From()] + 1);
            }
        }
        Q.pop();
    }
    return answer;
}

void ReverseGraph(TGraph* G) {
    map<pair<string, string>, bool> used;
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
            if (!used[make_pair(jt.From(), jt.To())] && !G->ExistEdge(jt.To(), jt.From())) {
                G->AddEdge(jt.To(), jt.From(), jt.Weight());
                G->DeleteEdge(jt.From(), jt.To());
                used[make_pair(jt.From(), jt.To())] = true;
                used[make_pair(jt.To(), jt.From())] = true;
            }
        }
    }
}

void DrawGraph(TGraph* G) {
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext())
        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext())
            cout << jt.From() << ' ' << jt.To() << ' ' << jt.Weight() << endl;
    cout << endl;
}

int main() {
    auto_ptr<TGraph> G(new TMapGraph());
    input(G.get());
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
            unzip(G.get(), jt.From(), jt.To(), jt.From(), jt.Weight());
        }
    }
    map<string, double> b = BFS(G.get(), start);
    ReverseGraph(G.get());
    map<string, double> d = BFS(G.get(), finish);
    ReverseGraph(G.get());
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
            if (it.Get()[0] != '_' && jt.To()[0] == '_') {
                zip(G.get(), jt.From(), jt.From(), jt.To(), 1, Answer[make_pair(jt.From(), jt.To())]);
            }
        }
    }
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
            Answer[make_pair(jt.From(), jt.To())] = (b[jt.From()] + d[jt.To()] + jt.Weight() == b[finish]);
        }
    }
    for (TVertexIterator it = G->ListVertices(); it.IsValid(); it.GetNext()) {
        for (TEdgeIterator jt = G->ListNeighbours(it.Get()); jt.IsValid(); jt.GetNext()) {
            if (Answer[make_pair(jt.From(), jt.To())]) {
                cout << jt.From() << ' ' << jt.To() << ' ' << jt.Weight() << endl;
            }
        }
    }
    return 0;
}
